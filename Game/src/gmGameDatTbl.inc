// ==========================================================================
/*!
  @file gmGameDatTbl.inc
  @brief ゲームデータロード設定 データテーブル

  @author Ishizaki
				Copyright(c) 2008 Dimps

  $Id: gmGameDatTbl.inc 9635 2009-03-03 09:27:00Z use1081 $
  $Date:: 2009-03-03 18:27:00 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
// データヘッダ
//#include "/bb/gmenemy.h"
//#include "/bb/gmstage.h"
//#include "/bb/gmdeco.h"

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------
// ==========================================================================
// ステージタイプ設定
// ==========================================================================
const GSE_MAIN_STAGE_TYPE g_gm_gamedat_stage_type_tbl[GSD_MAIN_STAGE_ID_MAX]	= {
	// Zone1
	GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_BOSS,
	// Zone2
	GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_BOSS,
	// Zone3
	GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_BOSS,
	// Zone4
	GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_ACT, GSD_MAIN_STAGE_TYPE_BOSS,
	// ZoneFinal
	GSD_MAIN_STAGE_TYPE_BOSS, GSD_MAIN_STAGE_TYPE_BOSS, GSD_MAIN_STAGE_TYPE_BOSS, GSD_MAIN_STAGE_TYPE_BOSS, GSD_MAIN_STAGE_TYPE_BOSS,
	// SpecialStage
	GSD_MAIN_STAGE_TYPE_SS, GSD_MAIN_STAGE_TYPE_SS, GSD_MAIN_STAGE_TYPE_SS, GSD_MAIN_STAGE_TYPE_SS, GSD_MAIN_STAGE_TYPE_SS, GSD_MAIN_STAGE_TYPE_SS, GSD_MAIN_STAGE_TYPE_SS, 
};


// ==========================================================================
/// ステージゾーンタイプ設定
// ==========================================================================
const GSE_MAIN_ZONE_TYPE g_gm_gamedat_zone_type_tbl[GSD_MAIN_STAGE_ID_MAX] = {
	GSD_MAIN_ZONE_TYPE_1, GSD_MAIN_ZONE_TYPE_1, GSD_MAIN_ZONE_TYPE_1, GSD_MAIN_ZONE_TYPE_1,
	GSD_MAIN_ZONE_TYPE_2, GSD_MAIN_ZONE_TYPE_2, GSD_MAIN_ZONE_TYPE_2, GSD_MAIN_ZONE_TYPE_2,
	GSD_MAIN_ZONE_TYPE_3, GSD_MAIN_ZONE_TYPE_3, GSD_MAIN_ZONE_TYPE_3, GSD_MAIN_ZONE_TYPE_3,
	GSD_MAIN_ZONE_TYPE_4, GSD_MAIN_ZONE_TYPE_4, GSD_MAIN_ZONE_TYPE_4, GSD_MAIN_ZONE_TYPE_4,
	GSD_MAIN_ZONE_TYPE_FINAL, GSD_MAIN_ZONE_TYPE_FINAL, GSD_MAIN_ZONE_TYPE_FINAL, GSD_MAIN_ZONE_TYPE_FINAL, GSD_MAIN_ZONE_TYPE_FINAL,
	GSD_MAIN_ZONE_TYPE_SS, GSD_MAIN_ZONE_TYPE_SS, GSD_MAIN_ZONE_TYPE_SS, GSD_MAIN_ZONE_TYPE_SS, GSD_MAIN_ZONE_TYPE_SS, GSD_MAIN_ZONE_TYPE_SS, GSD_MAIN_ZONE_TYPE_SS, 
	GSD_MAIN_ZONE_TYPE_1,
};


//----- Local Variables -----------------------------------------------------
// ==========================================================================
// 共通データ
// ==========================================================================
/* ロードデータ構造体 */
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_common[] = {
	// パス											アロケータ						前処理							後処理									ユーザーデータ
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostRing,				GMD_DWORK_NO_RING_MODEL},
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostRing,				GMD_DWORK_NO_RING_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_COM/RING/RING_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostRing,				GMD_DWORK_NO_RING_MAT},
#endif
	{ GSS_BASE_PATH "G_COM/CPIT/CPIT_MAIN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostCockpit,			-1},
};

/* ロード情報構造体 */
static const GMS_GAMEDAT_LOAD_INFO gm_gamedat_tbl_common_info_tbl[1] = {
    {gm_gamedat_tbl_common,	sizeof(gm_gamedat_tbl_common) / sizeof(gm_gamedat_tbl_common[0]),},			// 全共通
};


// ==========================================================================
// プレイヤーデータ
// ==========================================================================
/* ロードデータ構造体 */
// ソニック
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_player_sonic[] = {
	// パス												アロケータ							前処理					後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_COM/PLY/SON_MDL.AMB",			gmGameDatLoadAllocHeadSub,		NULL,					gmGameDatLoadProcPostPlayer,	-1},
	{ GSS_BASE_PATH "G_COM/PLY/SON_TEX.AMB",			gmGameDatLoadAllocHead,			NULL,					gmGameDatLoadProcPostPlayer,	-1},
	{ GSS_BASE_PATH "G_COM/PLY/SSON_MDL.AMB",			gmGameDatLoadAllocHeadSub,		NULL,					gmGameDatLoadProcPostPlayer,	-1},
	{ GSS_BASE_PATH "G_COM/PLY/SSON_TEX.AMB",			gmGameDatLoadAllocHeadSub,		NULL,					gmGameDatLoadProcPostPlayer,	-1},
	{ GSS_BASE_PATH "G_COM/PLY/SON_MTN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,					gmGameDatLoadProcPostPlayer,	-1},
#if _WII
	{ GSS_BASE_PATH "G_COM/PLY/SON_HAND.LOD",			gmGameDatLoadAllocHeadSub,		NULL,					gmGameDatLoadProcPostPlayer,	-1},
#endif
};

/* ロード情報構造体 */
static const GMS_GAMEDAT_LOAD_INFO gm_gamedat_tbl_player_info_tbl[GSD_CHAR_ID_MAX] = {
	// ロードデータ構造体		ロードデータ構造体数
    {gm_gamedat_tbl_player_sonic,	sizeof(gm_gamedat_tbl_player_sonic) / sizeof(gm_gamedat_tbl_player_sonic[0])},		// ソニック
    {gm_gamedat_tbl_player_sonic,	sizeof(gm_gamedat_tbl_player_sonic) / sizeof(gm_gamedat_tbl_player_sonic[0])},		// スーパーソニック
    {gm_gamedat_tbl_player_sonic,	sizeof(gm_gamedat_tbl_player_sonic) / sizeof(gm_gamedat_tbl_player_sonic[0])},		// スペステソニック
    {gm_gamedat_tbl_player_sonic,	sizeof(gm_gamedat_tbl_player_sonic) / sizeof(gm_gamedat_tbl_player_sonic[0])},		// ピンボールソニック(途中切り替えの為参照はされない)
    {gm_gamedat_tbl_player_sonic,	sizeof(gm_gamedat_tbl_player_sonic) / sizeof(gm_gamedat_tbl_player_sonic[0])},		// ピンボールスーパーソニック(途中切り替えの為参照はされない)
    {gm_gamedat_tbl_player_sonic,	sizeof(gm_gamedat_tbl_player_sonic) / sizeof(gm_gamedat_tbl_player_sonic[0])},		// トロッコソニック(途中切り替えの為参照はされない)
    {gm_gamedat_tbl_player_sonic,	sizeof(gm_gamedat_tbl_player_sonic) / sizeof(gm_gamedat_tbl_player_sonic[0])},		// トロッコスーパーソニック(途中切り替えの為参照はされない)
};


// ==========================================================================
// 背景
// ==========================================================================
/* ロードデータ構造体 */
/*
  キャラクタ, パレット, ブロック, マップ, イベント, 遠景BG, 装飾データ(あれば)
  の並び順で登録してください。
 */


// ステージZone1
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map01_01[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE11_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION

//以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE1/MAPFAR/ZONE1_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,				gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE1/DECO/DECO_ZONE1.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
// 以下テスト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map01_02[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE12_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE1/MAPFAR/ZONE1_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE1/DECO/DECO_ZONE1.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map01_03[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE13_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE1/MAPFAR/ZONE13_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE1/DECO/DECO_ZONE1.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map01_BOSS[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1BOSS_MAP.AMB",gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE1/MAPFAR/ZONE13_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE1/DECO/DECO_ZONE1.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};

// ステージZone2
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map02_01[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE21_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE2/MAPFAR/ZONE2_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE2/DECO/DECO_ZONE2.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map02_02[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE22_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE2/MAPFAR/ZONE2_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE2/DECO/DECO_ZONE2.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map02_03[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE23_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE2/MAPFAR/ZONE2_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE2/DECO/DECO_ZONE2.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map02_BOSS[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2BOSS_MAP.AMB",	gmGameDatLoadAllocHeadSub,	NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE2/MAP/ZONE2_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE2/MAPFAR/ZONE2_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE2/DECO/DECO_ZONE2.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};

// ステージZone3
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map03_01[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE31_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
#if defined (MTD_DEBUG)
//	{ GSS_BASE_PATH "G_ZONE3/MAP/z3_debug_txb.txb",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif
	// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE3/MAPFAR/ZONE3_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE3/DECO/DECO_ZONE3.AMB",	gmGameDatLoadAllocHeadSub,		NULL,					gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
	//水面
#if GMD_WATER_SURFACE_USE_RENDER
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SF_MDL.AMB",	gmGameDatLoadAllocHeadSub,		NULL,			gmGameDatLoadProcPostWaterSurface,	-1},
#endif	// GMD_WATER_SURFACE_USE_RENDER
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map03_02[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE32_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
#if defined (MTD_DEBUG)
//	{ GSS_BASE_PATH "G_ZONE3/MAP/z3_debug_txb.txb",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE3/MAPFAR/ZONE3_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE3/DECO/DECO_ZONE3.AMB",	gmGameDatLoadAllocHeadSub,		NULL,					gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
	//水面
#if GMD_WATER_SURFACE_USE_RENDER
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SF_MDL.AMB",	gmGameDatLoadAllocHeadSub,		NULL,			gmGameDatLoadProcPostWaterSurface,	-1},
#endif	// GMD_WATER_SURFACE_USE_RENDER
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map03_03[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE33_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
#if defined (MTD_DEBUG)
//	{ GSS_BASE_PATH "G_ZONE3/MAP/z3_debug_txb.txb",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE3/MAPFAR/ZONE3_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE3/DECO/DECO_ZONE3.AMB",	gmGameDatLoadAllocHeadSub,		NULL,					gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
	//水面
#if GMD_WATER_SURFACE_USE_RENDER
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SF_MDL.AMB",	gmGameDatLoadAllocHeadSub,		NULL,			gmGameDatLoadProcPostWaterSurface,	-1},
#endif	// GMD_WATER_SURFACE_USE_RENDER
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map03_BOSS[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3BOSS_MAP.AMB",	gmGameDatLoadAllocHeadSub,	NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE3/MAP/ZONE3_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
#if defined (MTD_DEBUG)
//	{ GSS_BASE_PATH "G_ZONE3/MAP/z3_debug_txb.txb",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE3/MAPFAR/ZONE3_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE3/DECO/DECO_ZONE3.AMB",	gmGameDatLoadAllocHeadSub,		NULL,					gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
	//水面
#if GMD_WATER_SURFACE_USE_RENDER
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SF_MDL.AMB",	gmGameDatLoadAllocHeadSub,		NULL,			gmGameDatLoadProcPostWaterSurface,	-1},
#endif	// GMD_WATER_SURFACE_USE_RENDER
};

// ステージZone4
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map04_01[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE41_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE1/MAPFAR/ZONE1_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE4/DECO/DECO_ZONE4.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map04_02[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE42_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE1/MAPFAR/ZONE1_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE4/DECO/DECO_ZONE4.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map04_03[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE43_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE1/MAPFAR/ZONE1_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE4/DECO/DECO_ZONE4.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map04_BOSS[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4BOSS_MAP.AMB",	gmGameDatLoadAllocHeadSub,	NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE4/MAP/ZONE4_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE1/MAPFAR/ZONE1_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE4/DECO/DECO_ZONE4.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};

// ステージZoneFinal
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_final_BOSS01[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEFB_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONEF/MAPFAR/ZONEF_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST

// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONEF/DECO/DECO_ZONEF.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_final_BOSS02[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEFB_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONEF/MAPFAR/ZONEF_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST

// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONEF/DECO/DECO_ZONEF.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_final_BOSS03[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEFB_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONEF/MAPFAR/ZONEF_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST

// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONEF/DECO/DECO_ZONEF.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_final_BOSS04[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ "/bb/gmstage.bb",					gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostMap,		-1},
// 以下装飾
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_final_BOSS05[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEFB_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONEF/MAP/ZONEF_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONEF/MAPFAR/ZONEF_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,					gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST

// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONEF/DECO/DECO_ZONEF.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
};

// ステージSpecialStage
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_SS01[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_SS/MAP/SS1_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_T.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_SS/MAPFAR/SS01_MAPFAR.AMB",gmGameDatLoadAllocHeadSub,NULL,				gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_SS02[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_SS/MAP/SS2_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_T.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_SS/MAPFAR/SS02_MAPFAR.AMB",gmGameDatLoadAllocHeadSub,NULL,				gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_SS03[] = {
	// パス								アロケータ					前処理						後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_SS/MAP/SS3_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_T.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_SS/MAPFAR/SS03_MAPFAR.AMB",gmGameDatLoadAllocHeadSub,NULL,				gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_SS04[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_SS/MAP/SS4_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_T.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_SS/MAPFAR/SS04_MAPFAR.AMB",gmGameDatLoadAllocHeadSub,NULL,				gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_SS05[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_SS/MAP/SS5_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_T.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_SS/MAPFAR/SS05_MAPFAR.AMB",gmGameDatLoadAllocHeadSub,NULL,				gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_SS06[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_SS/MAP/SS6_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_T.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_SS/MAPFAR/SS06_MAPFAR.AMB",gmGameDatLoadAllocHeadSub,NULL,				gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_SS07[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_SS/MAP/SS7_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_T.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_SS/MAP/SS_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
// 以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_SS/MAPFAR/SS07_MAPFAR.AMB",gmGameDatLoadAllocHeadSub,NULL,				gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
};

// Ending
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_map_ending[] = {
	// パス								アロケータ					前処理							後処理							ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1E_MAP.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_T.AMB",		gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_ATTR.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M_M.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
	{ GSS_BASE_PATH "G_ZONE1/MAP/ZONE1_M_V.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostMap,		-1},
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
//以下遠景
#if GMD_MAP_FAR_TEST
	{ GSS_BASE_PATH "G_ZONE1/MAPFAR/ZONE1_MAPFAR.AMB",	gmGameDatLoadAllocHeadSub,	NULL,				gmGameDatLoadProcPostMapFar,	-1},
#endif	//GMD_MAP_FAR_TEST
// 以下装飾
#if GMD_DECO_TEST
	{ GSS_BASE_PATH "G_ZONE1/DECO/DECO_ZONE1.AMB",	gmGameDatLoadAllocHeadSub,		NULL,				gmGameDatLoadProcPostDeco,	-1},
#endif	//GMD_DECO_TEST
// 以下テスト
};


/* ロード情報構造体 */
static const GMS_GAMEDAT_LOAD_INFO gm_gamedat_tbl_map_info_tbl[GSD_MAIN_STAGE_ID_MAX] = {
	// Zone1
	{gm_gamedat_tbl_map01_01,	sizeof(gm_gamedat_tbl_map01_01) / sizeof(gm_gamedat_tbl_map01_01[0])},
	{gm_gamedat_tbl_map01_02,	sizeof(gm_gamedat_tbl_map01_02) / sizeof(gm_gamedat_tbl_map01_02[0])},
	{gm_gamedat_tbl_map01_03,	sizeof(gm_gamedat_tbl_map01_03) / sizeof(gm_gamedat_tbl_map01_03[0])},
	{gm_gamedat_tbl_map01_BOSS,	sizeof(gm_gamedat_tbl_map01_BOSS) / sizeof(gm_gamedat_tbl_map01_BOSS[0])},
	
	// Zone2
	{gm_gamedat_tbl_map02_01,	sizeof(gm_gamedat_tbl_map02_01) / sizeof(gm_gamedat_tbl_map02_01[0])},
	{gm_gamedat_tbl_map02_02,	sizeof(gm_gamedat_tbl_map02_02) / sizeof(gm_gamedat_tbl_map02_02[0])},
	{gm_gamedat_tbl_map02_03,	sizeof(gm_gamedat_tbl_map02_03) / sizeof(gm_gamedat_tbl_map02_03[0])},
	{gm_gamedat_tbl_map02_BOSS,	sizeof(gm_gamedat_tbl_map02_BOSS) / sizeof(gm_gamedat_tbl_map02_BOSS[0])},
	
	// Zone3
	{gm_gamedat_tbl_map03_01,	sizeof(gm_gamedat_tbl_map03_01) / sizeof(gm_gamedat_tbl_map03_01[0])},
	{gm_gamedat_tbl_map03_02,	sizeof(gm_gamedat_tbl_map03_02) / sizeof(gm_gamedat_tbl_map03_02[0])},
	{gm_gamedat_tbl_map03_03,	sizeof(gm_gamedat_tbl_map03_03) / sizeof(gm_gamedat_tbl_map03_03[0])},
	{gm_gamedat_tbl_map03_BOSS,	sizeof(gm_gamedat_tbl_map03_BOSS) / sizeof(gm_gamedat_tbl_map03_BOSS[0])},
	
	// Zone4
	{gm_gamedat_tbl_map04_01,	sizeof(gm_gamedat_tbl_map04_01) / sizeof(gm_gamedat_tbl_map04_01[0])},
	{gm_gamedat_tbl_map04_02,	sizeof(gm_gamedat_tbl_map04_02) / sizeof(gm_gamedat_tbl_map04_02[0])},
	{gm_gamedat_tbl_map04_03,	sizeof(gm_gamedat_tbl_map04_03) / sizeof(gm_gamedat_tbl_map04_03[0])},
	{gm_gamedat_tbl_map04_BOSS,	sizeof(gm_gamedat_tbl_map04_BOSS) / sizeof(gm_gamedat_tbl_map04_BOSS[0])},
	
	// Final
	{gm_gamedat_tbl_map_final_BOSS01,	sizeof(gm_gamedat_tbl_map_final_BOSS01) / sizeof(gm_gamedat_tbl_map_final_BOSS01[0])},
	{gm_gamedat_tbl_map_final_BOSS02,	sizeof(gm_gamedat_tbl_map_final_BOSS02) / sizeof(gm_gamedat_tbl_map_final_BOSS02[0])},
	{gm_gamedat_tbl_map_final_BOSS03,	sizeof(gm_gamedat_tbl_map_final_BOSS03) / sizeof(gm_gamedat_tbl_map_final_BOSS03[0])},
	{gm_gamedat_tbl_map_final_BOSS04,	sizeof(gm_gamedat_tbl_map_final_BOSS04) / sizeof(gm_gamedat_tbl_map_final_BOSS04[0])},
	{gm_gamedat_tbl_map_final_BOSS05,	sizeof(gm_gamedat_tbl_map_final_BOSS05) / sizeof(gm_gamedat_tbl_map_final_BOSS05[0])},

	// SpecialStage
	{gm_gamedat_tbl_map_SS01, sizeof(gm_gamedat_tbl_map_SS01) / sizeof(gm_gamedat_tbl_map_SS01[0])},
	{gm_gamedat_tbl_map_SS02, sizeof(gm_gamedat_tbl_map_SS02) / sizeof(gm_gamedat_tbl_map_SS02[0])},
	{gm_gamedat_tbl_map_SS03, sizeof(gm_gamedat_tbl_map_SS03) / sizeof(gm_gamedat_tbl_map_SS03[0])},
	{gm_gamedat_tbl_map_SS04, sizeof(gm_gamedat_tbl_map_SS04) / sizeof(gm_gamedat_tbl_map_SS04[0])},
	{gm_gamedat_tbl_map_SS05, sizeof(gm_gamedat_tbl_map_SS05) / sizeof(gm_gamedat_tbl_map_SS05[0])},
	{gm_gamedat_tbl_map_SS06, sizeof(gm_gamedat_tbl_map_SS06) / sizeof(gm_gamedat_tbl_map_SS06[0])},
	{gm_gamedat_tbl_map_SS07, sizeof(gm_gamedat_tbl_map_SS07) / sizeof(gm_gamedat_tbl_map_SS07[0])},

	// エンディング
	{gm_gamedat_tbl_map_ending,	sizeof(gm_gamedat_tbl_map_ending) / sizeof(gm_gamedat_tbl_map_ending[0])},
};

// ==========================================================================
// エフェクト
// ==========================================================================
/* エフェクト用 データワークNOリスト */
//static const u16 gm_gamedat_effect_data_work_no_list[]	= {
//	GMD_DWORK_NO_EFFECT_CMN_ARC,
//	0xFFFF,				// 終端
//};

/* ロードデータ構造体 */
// Zone1
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect01_01[]	= {
	// パス											アロケータ							前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E002.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E02_ARC},	// エネミー002エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E004.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E04_ARC},	// エネミー004エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE1/EFF/EFF_ZONE1.AMB",	gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン１エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect01_02[]	= {
	// パス											アロケータ							前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E002.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E02_ARC},	// エネミー002エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E004.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E04_ARC},	// エネミー004エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE1/EFF/EFF_ZONE1.AMB",	gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン１エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect01_03[]	= {
	// パス											アロケータ							前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E002.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E02_ARC},	// エネミー002エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E004.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E04_ARC},	// エネミー004エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE1/EFF/EFF_ZONE1.AMB",	gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン１エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect01_BOSS[]	= {
	// パス											アロケータ							前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_ZONE1/EFF/EFF_ZONE1.AMB",	gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン１エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_BS_CMN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC},	// ボス共通エフェクト
};

// Zone2
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect02_01[]	= {
	// パス											アロケータ							前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",		gmGameDatLoadAllocHead,			NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E005.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E05_ARC},	// エネミー005エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E006.AMB",		gmGameDatLoadAllocHead,			NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E06_ARC},	// エネミー006エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE2/EFF/EFF_ZONE2.AMB",	gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン２エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect02_02[]	= {
	// パス											アロケータ							前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",		gmGameDatLoadAllocHead,			NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E005.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E05_ARC},	// エネミー005エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E006.AMB",		gmGameDatLoadAllocHead,			NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E06_ARC},	// エネミー006エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE2/EFF/EFF_ZONE2.AMB",	gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン２エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect02_03[]	= {
	// パス											アロケータ							前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",		gmGameDatLoadAllocHead,			NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E005.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E05_ARC},	// エネミー005エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E006.AMB",		gmGameDatLoadAllocHead,			NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E06_ARC},	// エネミー006エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE2/EFF/EFF_ZONE2.AMB",	gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン２エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect02_BOSS[]	= {
	// パス												アロケータ						前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_BS_CMN.AMB",			gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC},	// ボス共通エフェクト
	{ GSS_BASE_PATH "G_ZONE2/EFF/EFF_ZONE2.AMB",		gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン２エフェクト
};

// Zone3
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect03_01[]	= {
	// パス												アロケータ						前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E007.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E07_ARC},	// エネミー007エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E010.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E10_ARC},	// エネミー010エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE3/EFF/EFF_ZONE3.AMB",		gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン３エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect03_02[]	= {
	// パス												アロケータ						前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E007.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E07_ARC},	// エネミー007エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E010.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E10_ARC},	// エネミー010エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE3/EFF/EFF_ZONE3.AMB",		gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン３エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect03_03[]	= {
	// パス												アロケータ						前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E007.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E07_ARC},	// エネミー007エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E010.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E10_ARC},	// エネミー010エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",			gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE3/EFF/EFF_ZONE3.AMB",		gmGameDatLoadAllocHeadSub,	NULL,							gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン３エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect03_BOSS[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E007.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E07_ARC},	// エネミー007エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E010.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E10_ARC},	// エネミー010エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE3/EFF/EFF_ZONE3.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン３エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_BS_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC},	// ボス共通エフェクト
};

// Zone4
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect04_01[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E013.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E13_ARC},	// エネミー013エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE4/EFF/EFF_ZONE4.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン４エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect04_02[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E013.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E13_ARC},	// エネミー013エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE4/EFF/EFF_ZONE4.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン４エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect04_03[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E013.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E13_ARC},	// エネミー013エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E014.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E14_ARC},	// エネミー014エフェクト
	{ GSS_BASE_PATH "G_ZONE4/EFF/EFF_ZONE4.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン４エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect04_BOSS[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_BS_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC},	// ボス共通エフェクト
	{ GSS_BASE_PATH "G_ZONE4/EFF/EFF_ZONE4.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン４エフェクト
};

// ZoneFinal
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect_final_BOSS01[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_ZONEF/EFF/EFF_ZONE5.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン５エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_BS_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC},	// ボス共通エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect_final_BOSS02[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_ZONEF/EFF/EFF_ZONE5.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン５エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_BS_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC},	// ボス共通エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect_final_BOSS03[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_ZONEF/EFF/EFF_ZONE5.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン５エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_BS_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC},	// ボス共通エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect_final_BOSS04[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_ZONEF/EFF/EFF_ZONE5.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン５エフェクト
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect_final_BOSS05[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_ZONEF/EFF/EFF_ZONE5.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン５エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_BS_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC},	// ボス共通エフェクト
};

// SpecialStage
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect_ss01[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_SS/EFF/EFF_ZONESS.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーンSSエフェクト
};

// Ending
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_effect_ending[]	= {
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/EFF/EFF_CMN.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_CMN_ARC},	// 共通エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E002.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E02_ARC},	// エネミー002エフェクト
	{ GSS_BASE_PATH "G_COM/EFF/EFF_E004.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ENE_E04_ARC},	// エネミー004エフェクト
	{ GSS_BASE_PATH "G_ZONE1/EFF/EFF_ZONE1.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEffect,			GMD_DWORK_NO_EFFECT_ZONE_ARC},	// ゾーン１エフェクト
};

/* ロード情報構造体 */
static const GMS_GAMEDAT_LOAD_INFO gm_gamedat_tbl_effect_info_tbl[GSD_MAIN_STAGE_ID_MAX]	= {
	// Zone1
	{gm_gamedat_tbl_effect01_01,	sizeof(gm_gamedat_tbl_effect01_01) / sizeof(gm_gamedat_tbl_effect01_01[0])},
	{gm_gamedat_tbl_effect01_02,	sizeof(gm_gamedat_tbl_effect01_02) / sizeof(gm_gamedat_tbl_effect01_02[0])},
	{gm_gamedat_tbl_effect01_03,	sizeof(gm_gamedat_tbl_effect01_03) / sizeof(gm_gamedat_tbl_effect01_03[0])},
	{gm_gamedat_tbl_effect01_BOSS,	sizeof(gm_gamedat_tbl_effect01_BOSS) / sizeof(gm_gamedat_tbl_effect01_BOSS[0])},
	
	// Zone2
	{gm_gamedat_tbl_effect02_01,	sizeof(gm_gamedat_tbl_effect02_01) / sizeof(gm_gamedat_tbl_effect02_01[0])},
	{gm_gamedat_tbl_effect02_02,	sizeof(gm_gamedat_tbl_effect02_02) / sizeof(gm_gamedat_tbl_effect02_02[0])},
	{gm_gamedat_tbl_effect02_03,	sizeof(gm_gamedat_tbl_effect02_03) / sizeof(gm_gamedat_tbl_effect02_03[0])},
	{gm_gamedat_tbl_effect02_BOSS,	sizeof(gm_gamedat_tbl_effect02_BOSS) / sizeof(gm_gamedat_tbl_effect02_BOSS[0])},
	
	// Zone3
	{gm_gamedat_tbl_effect03_01,	sizeof(gm_gamedat_tbl_effect03_01) / sizeof(gm_gamedat_tbl_effect03_01[0])},
	{gm_gamedat_tbl_effect03_02,	sizeof(gm_gamedat_tbl_effect03_02) / sizeof(gm_gamedat_tbl_effect03_02[0])},
	{gm_gamedat_tbl_effect03_03,	sizeof(gm_gamedat_tbl_effect03_03) / sizeof(gm_gamedat_tbl_effect03_03[0])},
	{gm_gamedat_tbl_effect03_BOSS,	sizeof(gm_gamedat_tbl_effect03_BOSS) / sizeof(gm_gamedat_tbl_effect03_BOSS[0])},
	
	// Zone4
	{gm_gamedat_tbl_effect04_01,	sizeof(gm_gamedat_tbl_effect04_01) / sizeof(gm_gamedat_tbl_effect04_01[0])},
	{gm_gamedat_tbl_effect04_02,	sizeof(gm_gamedat_tbl_effect04_02) / sizeof(gm_gamedat_tbl_effect04_02[0])},
	{gm_gamedat_tbl_effect04_03,	sizeof(gm_gamedat_tbl_effect04_03) / sizeof(gm_gamedat_tbl_effect04_03[0])},
	{gm_gamedat_tbl_effect04_BOSS,	sizeof(gm_gamedat_tbl_effect04_BOSS) / sizeof(gm_gamedat_tbl_effect04_BOSS[0])},
	
	// ZoneFinal
	{gm_gamedat_tbl_effect_final_BOSS01,	sizeof(gm_gamedat_tbl_effect_final_BOSS01) / sizeof(gm_gamedat_tbl_effect_final_BOSS01[0])},
	{gm_gamedat_tbl_effect_final_BOSS02,	sizeof(gm_gamedat_tbl_effect_final_BOSS02) / sizeof(gm_gamedat_tbl_effect_final_BOSS02[0])},
	{gm_gamedat_tbl_effect_final_BOSS03,	sizeof(gm_gamedat_tbl_effect_final_BOSS03) / sizeof(gm_gamedat_tbl_effect_final_BOSS03[0])},
	{gm_gamedat_tbl_effect_final_BOSS04,	sizeof(gm_gamedat_tbl_effect_final_BOSS04) / sizeof(gm_gamedat_tbl_effect_final_BOSS04[0])},
	{gm_gamedat_tbl_effect_final_BOSS05,	sizeof(gm_gamedat_tbl_effect_final_BOSS05) / sizeof(gm_gamedat_tbl_effect_final_BOSS05[0])},

	// SpecialStage
	{gm_gamedat_tbl_effect_ss01, sizeof(gm_gamedat_tbl_effect_ss01) / sizeof(gm_gamedat_tbl_effect_ss01[0])},
	{gm_gamedat_tbl_effect_ss01, sizeof(gm_gamedat_tbl_effect_ss01) / sizeof(gm_gamedat_tbl_effect_ss01[0])},
	{gm_gamedat_tbl_effect_ss01, sizeof(gm_gamedat_tbl_effect_ss01) / sizeof(gm_gamedat_tbl_effect_ss01[0])},
	{gm_gamedat_tbl_effect_ss01, sizeof(gm_gamedat_tbl_effect_ss01) / sizeof(gm_gamedat_tbl_effect_ss01[0])},
	{gm_gamedat_tbl_effect_ss01, sizeof(gm_gamedat_tbl_effect_ss01) / sizeof(gm_gamedat_tbl_effect_ss01[0])},
	{gm_gamedat_tbl_effect_ss01, sizeof(gm_gamedat_tbl_effect_ss01) / sizeof(gm_gamedat_tbl_effect_ss01[0])},
	{gm_gamedat_tbl_effect_ss01, sizeof(gm_gamedat_tbl_effect_ss01) / sizeof(gm_gamedat_tbl_effect_ss01[0])},

	// Ending
	{gm_gamedat_tbl_effect_ending,	sizeof(gm_gamedat_tbl_effect_ending) / sizeof(gm_gamedat_tbl_effect_ending[0])},
};


// ==========================================================================
// 敵
// ==========================================================================
///* ザコ敵用 BBデータ順 データワークNOリスト */
//static const u16 gm_gamedat_enemy_data_work_no_list[] = {
//	//GMD_DWORK_NO_ENE_,
//	0xFFFF,				// 終端
//};

/* ロードデータ構造体 */
// Zone1
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy01_01[] = {
#if GMD_ENEMY_TEST
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_MTN.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MTN},		 // モトラ

	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MODEL}, // ガブッチョ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_TEX},	 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_MTN.AMB",		gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MTN},		 // ガブッチョ
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_MTN.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MTN},		 // スティンガー
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",			gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_MTN.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MTN},		 // メレオン
#else
	// パス												アロケータ							前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_MDL.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MODEL},	 // モトラ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_TEX.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_TEX},		 // モトラ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_MTN.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MTN},		 // モトラ

	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_MDL.AMB",		gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MODEL},		 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_TEX.AMB",		gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_TEX},		 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_MTN.AMB",		gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MTN},		 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_MDL.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MODEL},	 // スティンガー
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_TEX.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_TEX},		 // スティンガー
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_MTN.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MTN},		 // スティンガー
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_MDL.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MODEL},	 // メレオン
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_R_MDL.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_R_MODEL},	 // メレオン
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_TEX.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_TEX},		 // メレオン
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_MTN.AMB",	gmGameDatLoadAllocHead,			NULL,						gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MTN},		 // メレオン
#endif
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy01_02[] = {
#if GMD_ENEMY_TEST
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MTN},		 // モトラ

	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MODEL}, // ガブッチョ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_TEX},	 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MTN},		 // ガブッチョ
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MTN},		 // スティンガー
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MTN},		 // メレオン
#else
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MODEL},	 // モトラ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_TEX},		 // モトラ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MTN},		 // モトラ

	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MODEL},		 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_TEX},		 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MTN},		 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MODEL},	 // スティンガー
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_TEX},		 // スティンガー
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MTN},		 // スティンガー
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MODEL},	 // メレオン
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_R_MDL.AMB",	gmGameDatLoadAllocHead,	NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_R_MODEL},	 // メレオン
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_TEX},		 // メレオン
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MTN},		 // メレオン
#endif
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy01_03[] = {
#if GMD_ENEMY_TEST
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MTN},		 // モトラ

	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MODEL}, // ガブッチョ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_TEX},	 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MTN},		 // ガブッチョ
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MTN},		 // スティンガー
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MTN},		 // メレオン
#else
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHeadSub,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MODEL},	 // モトラ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_TEX},		 // モトラ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MOTORA_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOTORA_MTN},		 // モトラ

	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MODEL},		 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_TEX},		 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_GABU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GABU_MTN},		 // ガブッチョ
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MODEL},	 // スティンガー
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_TEX},		 // スティンガー
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_STING_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_STING_MTN},		 // スティンガー
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MODEL},	 // メレオン
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_R_MDL.AMB",	gmGameDatLoadAllocHead,	NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_R_MODEL},	 // メレオン
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_TEX},		 // メレオン
	{ GSS_BASE_PATH "G_ZONE1/ENE/ENE_MEREON_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MEREON_MTN},		 // メレオン
#endif
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy01_BOSS[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/BOSS/BOSS01.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostBoss,			-1},	 // 
};

// Zone2
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy02_01[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ

	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_GARDON_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GARDON_MODEL},	 // ガードン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_GARDON_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GARDON_TEX},		 // ガードン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_GARDON_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GARDON_MTN},		 // ガードン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_HARO_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARO_MODEL},		 // ハロゲン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_HARO_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARO_TEX},		 // ハロゲン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_HARO_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARO_MTN},		 // ハロゲン

};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy02_02[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ

#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_GARDON_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GARDON_MODEL},	 // ガードン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_GARDON_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GARDON_TEX},		 // ガードン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_GARDON_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GARDON_MTN},		 // ガードン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_HARO_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARO_MODEL},		 // ハロゲン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_HARO_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARO_TEX},		 // ハロゲン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_HARO_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARO_MTN},		 // ハロゲン
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy02_03[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ

	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_GARDON_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GARDON_MODEL},	 // ガードン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_GARDON_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GARDON_TEX},		 // ガードン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_GARDON_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_GARDON_MTN},		 // ガードン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_HARO_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARO_MODEL},		 // ハロゲン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_HARO_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARO_TEX},		 // ハロゲン
	{ GSS_BASE_PATH "G_ZONE2/ENE/ENE_HARO_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARO_MTN},		 // ハロゲン
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy02_BOSS[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE2/BOSS/BOSS02.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostBoss,			-1},	 // 
};

// Zone3
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy03_01[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_MODEL},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_TEX},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_MTN},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_MODEL},	 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_TEX},		 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_MTN},		 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIUNI_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIUNI_MODEL},	 // ウニウニ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIUNI_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIUNI_TEX},		 // ウニウニ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIUNI_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIUNI_MTN},		 // ウニウニ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_BUKU_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_BUKU_MODEL},		 // ブクブク
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_BUKU_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_BUKU_TEX},		 // ブクブク
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_BUKU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_BUKU_MTN},		 // ブクブク
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy03_02[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_MODEL},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_TEX},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_MTN},		 // モグリン
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_MODEL},	 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_TEX},		 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_MTN},		 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIUNI_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIUNI_MODEL},	 // ウニウニ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIUNI_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIUNI_TEX},		 // ウニウニ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIUNI_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIUNI_MTN},		 // ウニウニ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_BUKU_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_BUKU_MODEL},		 // ブクブク
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_BUKU_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_BUKU_TEX},		 // ブクブク
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_BUKU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_BUKU_MTN},		 // ブクブク
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy03_03[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_MODEL},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_TEX},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_MTN},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_MODEL},	 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_TEX},		 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_MTN},		 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIUNI_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIUNI_MODEL},	 // ウニウニ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIUNI_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIUNI_TEX},		 // ウニウニ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIUNI_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIUNI_MTN},		 // ウニウニ
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_BUKU_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_BUKU_MODEL},		 // ブクブク
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_BUKU_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_BUKU_TEX},		 // ブクブク
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_BUKU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_BUKU_MTN},		 // ブクブク
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy03_BOSS[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ

	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ

	{ GSS_BASE_PATH "G_ZONE3/BOSS/BOSS03.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostBoss,			-1},	 // 
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_MODEL},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_TEX},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_MOGU_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_MOGU_MTN},		 // モグリン
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_MODEL},	 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_TEX},		 // ウニデス
	{ GSS_BASE_PATH "G_ZONE3/ENE/ENE_UNIDES_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_UNIDES_MTN},		 // ウニデス
};
#if _IPHONE
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy03_final_BOSS[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE3/BOSS/BOSS03.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostBoss,			-1},	 // 
};
#endif // _IPHONE

// Zone4
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy04_01[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_MODEL},	 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_TEX},		 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_MTN},		 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_MAT},		 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KANI_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KANI_MODEL},		 // かにパンチ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KANI_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KANI_TEX},		 // かにパンチ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KANI_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KANI_MTN},		 // かにパンチ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KAMA_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KAMA_MODEL},		 // カマキラー
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KAMA_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KAMA_TEX},		 // カマキラー
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KAMA_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KAMA_MTN},		 // カマキラー
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy04_02[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_MODEL},	 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_TEX},		 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_MTN},		 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_MAT},		 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KANI_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KANI_MODEL},		 // かにパンチ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KANI_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KANI_TEX},		 // かにパンチ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KANI_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KANI_MTN},		 // かにパンチ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KAMA_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KAMA_MODEL},		 // カマキラー
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KAMA_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KAMA_TEX},		 // カマキラー
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KAMA_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KAMA_MTN},		 // カマキラー
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy04_03[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_MODEL},	 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_TEX},		 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_MTN},		 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_T_STAR_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_T_STAR_MAT},		 // テルスター
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KANI_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KANI_MODEL},		// かにパンチ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KANI_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KANI_TEX},		 // かにパンチ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KANI_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KANI_MTN},		 // かにパンチ
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KAMA_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KAMA_MODEL},		 // カマキラー
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KAMA_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KAMA_TEX},		 // カマキラー
	{ GSS_BASE_PATH "G_ZONE4/ENE/ENE_KAMA_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_KAMA_MTN},		 // カマキラー
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy04_BOSS[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE4/BOSS/BOSS04.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostBoss,			-1},	 // 

//	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
//	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
//	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
};

// ZoneFinal
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy_final_BOSS01[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/BOSS/BOSS05.AMB"/*仮*/,		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostBoss,			-1},	 // 
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy_final_BOSS02[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy_final_BOSS03[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy_final_BOSS04[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MODEL}, // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_TEX},	 // ハリセンボ
	{ GSS_BASE_PATH "G_COM/ENE/ENE_HARI_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostEnemy,			GMD_DWORK_NO_ENEMY_HARISENBO_MTN},	 // ハリセンボ
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy_final_BOSS05[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/BOSS/BOSS05.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostBoss,			-1},	 // 
};

static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_enemy_ending[] = {
	// パス								アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE3/BOSS/BOSS03.AMB",		gmGameDatLoadAllocHead,		NULL,							gmGameDatLoadProcPostBoss,			-1},	 // 
};

/* ロード情報構造体 */
static const GMS_GAMEDAT_LOAD_INFO gm_gamedat_tbl_enemy_info_tbl[GSD_MAIN_STAGE_ID_MAX] = {
	// Zone1
	{gm_gamedat_tbl_enemy01_01,	sizeof(gm_gamedat_tbl_enemy01_01) / sizeof(gm_gamedat_tbl_enemy01_01[0])},
	{gm_gamedat_tbl_enemy01_02,	sizeof(gm_gamedat_tbl_enemy01_02) / sizeof(gm_gamedat_tbl_enemy01_02[0])},
	{gm_gamedat_tbl_enemy01_03,	sizeof(gm_gamedat_tbl_enemy01_03) / sizeof(gm_gamedat_tbl_enemy01_03[0])},
	{gm_gamedat_tbl_enemy01_BOSS,	sizeof(gm_gamedat_tbl_enemy01_BOSS) / sizeof(gm_gamedat_tbl_enemy01_BOSS[0])},
	
	// Zone2
	{gm_gamedat_tbl_enemy02_01,	sizeof(gm_gamedat_tbl_enemy02_01) / sizeof(gm_gamedat_tbl_enemy02_01[0])},
	{gm_gamedat_tbl_enemy02_02,	sizeof(gm_gamedat_tbl_enemy02_02) / sizeof(gm_gamedat_tbl_enemy02_02[0])},
	{gm_gamedat_tbl_enemy02_03,	sizeof(gm_gamedat_tbl_enemy02_03) / sizeof(gm_gamedat_tbl_enemy02_03[0])},
	{gm_gamedat_tbl_enemy02_BOSS,	sizeof(gm_gamedat_tbl_enemy02_BOSS) / sizeof(gm_gamedat_tbl_enemy02_BOSS[0])},
	
	// Zone3
	{gm_gamedat_tbl_enemy03_01,	sizeof(gm_gamedat_tbl_enemy03_01) / sizeof(gm_gamedat_tbl_enemy03_01[0])},
	{gm_gamedat_tbl_enemy03_02,	sizeof(gm_gamedat_tbl_enemy03_02) / sizeof(gm_gamedat_tbl_enemy03_02[0])},
	{gm_gamedat_tbl_enemy03_03,	sizeof(gm_gamedat_tbl_enemy03_03) / sizeof(gm_gamedat_tbl_enemy03_03[0])},
	{gm_gamedat_tbl_enemy03_BOSS,	sizeof(gm_gamedat_tbl_enemy03_BOSS) / sizeof(gm_gamedat_tbl_enemy03_BOSS[0])},
	
	// Zone4
	{gm_gamedat_tbl_enemy04_01,	sizeof(gm_gamedat_tbl_enemy04_01) / sizeof(gm_gamedat_tbl_enemy04_01[0])},
	{gm_gamedat_tbl_enemy04_02,	sizeof(gm_gamedat_tbl_enemy04_02) / sizeof(gm_gamedat_tbl_enemy04_02[0])},
	{gm_gamedat_tbl_enemy04_03,	sizeof(gm_gamedat_tbl_enemy04_03) / sizeof(gm_gamedat_tbl_enemy04_03[0])},
	{gm_gamedat_tbl_enemy04_BOSS,	sizeof(gm_gamedat_tbl_enemy04_BOSS) / sizeof(gm_gamedat_tbl_enemy04_BOSS[0])},
	
	// ZoneFinal
	{gm_gamedat_tbl_enemy_final_BOSS01,	sizeof(gm_gamedat_tbl_enemy_final_BOSS01) / sizeof(gm_gamedat_tbl_enemy_final_BOSS01[0])},
	{gm_gamedat_tbl_enemy_final_BOSS02,	sizeof(gm_gamedat_tbl_enemy_final_BOSS02) / sizeof(gm_gamedat_tbl_enemy_final_BOSS02[0])},
	{gm_gamedat_tbl_enemy_final_BOSS03,	sizeof(gm_gamedat_tbl_enemy_final_BOSS03) / sizeof(gm_gamedat_tbl_enemy_final_BOSS03[0])},
	{gm_gamedat_tbl_enemy_final_BOSS04,	sizeof(gm_gamedat_tbl_enemy_final_BOSS04) / sizeof(gm_gamedat_tbl_enemy_final_BOSS04[0])},
	{gm_gamedat_tbl_enemy_final_BOSS05,	sizeof(gm_gamedat_tbl_enemy_final_BOSS05) / sizeof(gm_gamedat_tbl_enemy_final_BOSS05[0])},
	
	// SpecialStage
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},

	// Ending
	{gm_gamedat_tbl_enemy_ending, sizeof(gm_gamedat_tbl_enemy_ending) / sizeof(gm_gamedat_tbl_enemy_ending[0])},
};

#if _IPHONE
/* ボスロード情報構造体 */
static const GMS_GAMEDAT_LOAD_INFO gm_gamedat_tbl_enemy_final_info_tbl[GSD_MAIN_STAGE_ID_MAX] = {
	// Zone1
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{gm_gamedat_tbl_enemy01_BOSS,	sizeof(gm_gamedat_tbl_enemy01_BOSS) / sizeof(gm_gamedat_tbl_enemy01_BOSS[0])},
	
	// Zone2
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{gm_gamedat_tbl_enemy02_BOSS,	sizeof(gm_gamedat_tbl_enemy02_BOSS) / sizeof(gm_gamedat_tbl_enemy02_BOSS[0])},
	
	// Zone3
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{gm_gamedat_tbl_enemy03_final_BOSS,	sizeof(gm_gamedat_tbl_enemy03_final_BOSS) / sizeof(gm_gamedat_tbl_enemy03_final_BOSS[0])},
	
	// Zone4
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{gm_gamedat_tbl_enemy04_BOSS,	sizeof(gm_gamedat_tbl_enemy04_BOSS) / sizeof(gm_gamedat_tbl_enemy04_BOSS[0])},
	
	// ZoneFinal
	{gm_gamedat_tbl_enemy_final_BOSS01,	sizeof(gm_gamedat_tbl_enemy_final_BOSS01) / sizeof(gm_gamedat_tbl_enemy_final_BOSS01[0])},
	{gm_gamedat_tbl_enemy_final_BOSS02,	sizeof(gm_gamedat_tbl_enemy_final_BOSS02) / sizeof(gm_gamedat_tbl_enemy_final_BOSS02[0])},
	{gm_gamedat_tbl_enemy_final_BOSS03,	sizeof(gm_gamedat_tbl_enemy_final_BOSS03) / sizeof(gm_gamedat_tbl_enemy_final_BOSS03[0])},
	{gm_gamedat_tbl_enemy_final_BOSS04,	sizeof(gm_gamedat_tbl_enemy_final_BOSS04) / sizeof(gm_gamedat_tbl_enemy_final_BOSS04[0])},
	{gm_gamedat_tbl_enemy_final_BOSS05,	sizeof(gm_gamedat_tbl_enemy_final_BOSS05) / sizeof(gm_gamedat_tbl_enemy_final_BOSS05[0])},
	
	// SpecialStage
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},
	{NULL, 0},

	// Ending
	{NULL, 0},
};
#endif // _IPHONE

// ==========================================================================
// ギミック
// ==========================================================================
/* ロードデータ構造体 */
// 共通
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_com_gimmick[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_COM/GMK/GMK_SPRING_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPRING_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_SPRING_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPRING_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_SPRING_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPRING_MTN},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_NEEDLE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_NEEDLE_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_NEEDLE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_NEEDLE_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_COM/GMK/GMK_NEEDLE_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_NEEDLE_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_COM/GMK/GMK_DASH_P_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DASH_P_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_DASH_P_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DASH_P_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_DASH_P_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DASH_P_MTN},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_DASH_P_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DASH_P_MAT},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_GOAL_PNL_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GOAL_PNL_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_GOAL_PNL_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GOAL_PNL_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_P_MARKER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_P_MARKER_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_P_MARKER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_P_MARKER_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_P_MARKER_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_P_MARKER_MTN},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_P_MARKER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_P_MARKER_MAT},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_ITEM_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ITEM_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_ITEM_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ITEM_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_ANIMAL_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ANIMAL_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_ANIMAL_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ANIMAL_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_ANIMAL_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ANIMAL_MTN},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_SS_RING_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPL_RING_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_SS_RING_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPL_RING_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_COM/GMK/GMK_SS_RING_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPL_RING_MAT},
#endif
};

// Zone1
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick01_01[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_LAND_1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_1_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_LAND_1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_1_TEX},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_MTN},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_LAND1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_LAND1_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_LAND1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_LAND1_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_LAND1_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_LAND1_MTN},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_WALL1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_WALL1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_BRIDGE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BRIDGE_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_BRIDGE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BRIDGE_TEX},
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick01_02[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_LAND_1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_1_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_LAND_1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_1_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_MTN},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_LAND1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_LAND1_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_LAND1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_LAND1_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_LAND1_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_LAND1_MTN},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_WALL1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_WALL1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_BRIDGE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BRIDGE_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_BRIDGE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BRIDGE_TEX},
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick01_03[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_LAND_1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_1_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_LAND_1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_1_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_PULLEY_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PULLEY_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_PULLEY_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PULLEY_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_PULLEY_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PULLEY_MTN},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_MTN},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_LAND1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_LAND1_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_LAND1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_LAND1_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_LAND1_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_LAND1_MTN},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_WALL1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_WALL1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_BRIDGE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BRIDGE_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_BRIDGE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BRIDGE_TEX},
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick01_BOSS[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_LAND_1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_1_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_LAND_1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_1_TEX},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_TEX},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_T_ROPE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_T_ROPE_MTN},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MTN},
};

// Zone2
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick02_01[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_CANNON_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CANNON_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_CANNON_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CANNON_TEX},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MAT},

	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_OBJ_2_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_OBJ_2_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_WALL2_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_WALL2_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick02_02[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MAT},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_CANNON_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CANNON_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_CANNON_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CANNON_TEX},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_OBJ_2_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_OBJ_2_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_WALL2_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_WALL2_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ARROW_MDL.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ARROW_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ARROW_TEX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ARROW_TEX},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ARROW_MAT.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ARROW_MAT},

	// ゲーム開始時操作説明
	{ GSS_BASE_PATH "G_ZONE2/MSG/G_MSG_Z2.AMB",			gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_START_EXP_MSG},
	{ GSS_BASE_PATH "DEMO/CMN/D_CMN_WIN.AMB",				gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_START_EXP_WIN},

};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick02_03[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_MAT},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_CANNON_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CANNON_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_CANNON_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CANNON_TEX},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_MAT},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MAT},

	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_OBJ_2_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_OBJ_2_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_WALL2_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_B_WALL2_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ARROW_MDL.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ARROW_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ARROW_TEX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ARROW_TEX},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ARROW_MAT.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ARROW_MAT},
#endif // _IPHONE
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick02_BOSS[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_LAND_2_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_2_MAT},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MAT},
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MAT},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MAT},
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_STOPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STOPPER_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_CANNON_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CANNON_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_CANNON_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CANNON_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SLOT_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SLOT_MAT},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SP_CTPLT_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPCTPLT_MAT},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SHUTTER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_SHUTTER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_TEX},

	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MTN},
};

// Zone3
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick03_01[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_ROPE_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_ROCK_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ROCK_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_ROCK_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ROCK_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_ROCK_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ROCK_MTN},
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_MDL.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_TEX.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_MTN.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_MTN},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_MAT.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_MAT},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_TEX},
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_PRESSWALL_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_PRESSWALL_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_TEX},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA

	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_OBJ_3_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_OBJ_3_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_WALL3_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_WALL3_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_D_TANK_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DRAIN_TANK_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_D_TANK_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DRAIN_TANK_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_D_TANK_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DRAIN_TANK_MAT},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SWITCH3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SWITCH3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SWITCH3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_MAT},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_TVX},
#endif // _IPHONE
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick03_02[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_ROPE_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_TVX},
#endif // _IPHONE
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_ROCK_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ROCK_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_ROCK_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ROCK_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_ROCK_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ROCK_MTN},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_MDL.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_TEX.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_MTN.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_MTN},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_MAT.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_MAT},
#endif  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_TEX},
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_PRESSWALL_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_PRESSWALL_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_TEX},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA

	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_OBJ_3_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_OBJ_3_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_WALL3_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_WALL3_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_D_TANK_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DRAIN_TANK_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_D_TANK_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DRAIN_TANK_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_D_TANK_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DRAIN_TANK_MAT},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_TRUCK_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_TRUCK_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_TRUCK_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_TRUCK_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_TRUCK_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_TRUCK_MTN},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_DSIGN_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DSIGN_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_DSIGN_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DSIGN_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SWITCH3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SWITCH3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SWITCH3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_MAT},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_TVX},
#endif // _IPHONE

	// ゲーム開始時操作説明
	{ GSS_BASE_PATH "G_ZONE3/MSG/G_MSG_Z3.AMB",			gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_START_EXP_MSG},
	{ GSS_BASE_PATH "DEMO/CMN/D_CMN_WIN.AMB",				gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_START_EXP_WIN},
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick03_03[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_ROPE_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_TVX},
#endif // _IPHONE
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_ROCK_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ROCK_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_ROCK_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ROCK_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_ROCK_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_ROCK_MTN},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_MDL.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_TEX.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_MTN.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_MTN},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_WATER_SLIDER_MAT.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_WATER_SLIDER_MAT},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_PRESSWALL_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_PRESSWALL_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_TEX},

	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_OBJ_3_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_OBJ_3_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_WALL3_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B_WALL3_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_D_TANK_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DRAIN_TANK_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_D_TANK_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DRAIN_TANK_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_D_TANK_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_DRAIN_TANK_MAT},

	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SWITCH3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SWITCH3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SWITCH3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_MAT},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_TVX},
#endif // _IPHONE

};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick03_BOSS[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_ROPE_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_LAND_3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_3_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B3_PILLAR_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B3_PILLAR_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_B3_PILLAR_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_TVX},
#endif // _IPHONE

	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MTN},
};

// Zone4
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick04_01[] = {
	// パス									アロケータ					前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_LAND_4_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_4_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_LAND_4_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_4_TEX},

	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PISTON_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PISTON_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PISTON_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PISTON_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_UP_BMPR_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_UPBUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_UP_BMPR_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_UPBUMPER_TEX},
#if  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SEESAW_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SEESAW_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SEESAW_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SEESAW_TEX},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_STEAMPIPE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_STEAMPIPE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_STEAM_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_POPSTEAM_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_STEAM_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_POPSTEAM_TEX},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	//	ゾーン４用スピアまだ7/28現在
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_TEX},
	//	ゾーン４用迫る壁
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MTN},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MAT},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
#endif //  !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_B_WALL4_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_B_WALL4_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR.AMB",			gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR2_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR2_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR2_OPT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR2_OPT_MODEL},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SWITCH4_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SWITCH4_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SW_WALL4_MDL.AMB",	gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SW_WALL4_TEX.AMB",	gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_TEX},
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick04_02[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_LAND_4_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_4_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_LAND_4_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_4_TEX},

	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PISTON_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PISTON_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PISTON_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PISTON_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_UP_BMPR_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_UPBUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_UP_BMPR_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_UPBUMPER_TEX},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SEESAW_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SEESAW_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SEESAW_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SEESAW_TEX},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_STEAMPIPE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_STEAMPIPE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_STEAM_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_POPSTEAM_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_STEAM_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_POPSTEAM_TEX},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	//	ゾーン４用スピアまだ7/28現在
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_TEX},
	//	ゾーン４用迫る壁
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MTN},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MAT},

	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_B_WALL4_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_B_WALL4_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR.AMB",			gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR2_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR2_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR2_OPT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR2_OPT_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SWITCH4_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SWITCH4_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SW_WALL4_MDL.AMB",	gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SW_WALL4_TEX.AMB",	gmGameDatLoadAllocHeadSub,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_TEX},
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick04_03[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_LAND_4_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_4_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_LAND_4_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_4_TEX},

	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PISTON_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PISTON_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PISTON_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PISTON_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_UP_BMPR_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_UPBUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_UP_BMPR_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_UPBUMPER_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SEESAW_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SEESAW_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SEESAW_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SEESAW_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_STEAMPIPE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_STEAMPIPE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_STEAM_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_POPSTEAM_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_STEAM_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_POPSTEAM_TEX},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	//	ゾーン４用スピアまだ7/28現在
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_TEX},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	//	ゾーン４用迫る壁
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MTN},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_B_OBJ_1_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_OBJ_TEX},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_B_WALL4_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_B_WALL4_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_B_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR.AMB",			gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR2_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR2_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR2_OPT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR2_OPT_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SWITCH4_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SWITCH4_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SWITCH_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SW_WALL4_MDL.AMB",	gmGameDatLoadAllocHeadSub,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SW_WALL4_TEX.AMB",	gmGameDatLoadAllocHeadSub,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SW_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_PILLAR_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESS_PILLAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_PILLAR_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESS_PILLAR_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_PILLAR_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESS_PILLAR_MAT},
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick04_BOSS[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_LAND_4_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_4_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_LAND_4_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_4_TEX},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PISTON_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PISTON_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PISTON_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PISTON_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_BELT_CONV_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BELTCONV_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_UP_BMPR_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_UPBUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_UP_BMPR_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_UPBUMPER_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SEESAW_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SEESAW_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_SEESAW_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SEESAW_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_STEAMPIPE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_STEAMPIPE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_STEAM_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_POPSTEAM_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_P_STEAM_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_POPSTEAM_TEX},

	//	ゾーン４用スピアまだ7/28現在
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SPEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SPEAR_TEX},

	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MTN},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_PRESSWALL4_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_PRESSWALL_MAT},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MTN},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR.AMB",			gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR_TEX},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR2_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR2_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_GEAR2_OPT_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_GEAR2_OPT_MODEL},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
};

// ZoneFinal
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick_Final_BOSS01[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_MAT},
#if 1	// 仮設置
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_STEAMPIPE_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_STEAMPIPE_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MAT},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MTN},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_MAT},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_MAT},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_TVX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_TVX},
#endif // _IPHONE
#endif /* 1 */
	};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick_Final_BOSS02[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_MAT},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_STEAMPIPE_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_STEAMPIPE_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MAT},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MTN},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_MAT},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_MAT},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_TVX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_TVX},
#endif // _IPHONE
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick_Final_BOSS03[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_MAT},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_STEAMPIPE_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_STEAMPIPE_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MAT},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MTN},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_MAT},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_MAT},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_TVX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_TVX},
#endif // _IPHONE
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick_Final_BOSS04[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_MAT},
};
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick_Final_BOSS05[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_LAND_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_F_MAT},
#if 1	// 仮設置
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_STEAMPIPE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_MODEL},
	{ GSS_BASE_PATH "G_ZONE4/GMK/GMK_STEAMPIPE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_STEAMPIPE_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BUMPER_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BUMPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MTN.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MAT.AMB",		gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MAT},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_FLIPPER_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_FLIPPER_MAT},
#if !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MODEL},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_TEX},
	{ GSS_BASE_PATH "G_COM/GMK/GMK_CAPSULE_MTN.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_CAPSULE_MTN},
#endif // !GMD_GAMEDAT_NOLOAD_NOUSE_DATA
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_MAT},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_TEX},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MAT.AMB",	gmGameDatLoadAllocHead,		NULL,					gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_MAT},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_MDL.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_MODEL},
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_SHUTTER_F_TEX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SHUTTER_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_ZONEF/GMK/GMK_B3_PILLAR_F_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_PILLAR_TVX},
	{ GSS_BASE_PATH "G_ZONE3/GMK/GMK_SW_WALL3_TVX.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOSS3_WALL_TVX},
#endif // _IPHONE
#endif /* 1 */
};

// SpecialStage
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick_ss01[] = {
	// パス									アロケータ				前処理						後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_SQUARE_MDL.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_SQUARE_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_SQUARE_TEX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_SQUARE_TEX},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_SQUARE_MAT.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_SQUARE_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_SQUARE_TVX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_SQUARE_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_CIRCLE_MDL.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_CIRCLE_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_CIRCLE_TEX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_CIRCLE_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_CIRCLE_TVX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_CIRCLE_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ENDURANCE_MDL.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ENDURANCE_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ENDURANCE_TEX.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ENDURANCE_TEX},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ENDURANCE_MAT.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ENDURANCE_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ENDURANCE_TVX.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ENDURANCE_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_GOAL_MDL.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_GOAL_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_GOAL_TEX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_GOAL_TEX},
#if _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_GOAL_TVX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_GOAL_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_EMERALD_MDL.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_EMERALD_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_EMERALD_TEX.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_EMERALD_TEX},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_EMERALD_MTN.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_EMERALD_MTN},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_1UP_MDL.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_1UP_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_1UP_TEX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_1UP_TEX},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_TIME_MDL.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_TIME_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_TIME_TEX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_TIME_TEX},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_RINGGATE_MDL.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_RINGGATE_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_RINGGATE_TEX.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_RINGGATE_TEX},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_RINGGATE_MAT.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_RINGGATE_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_RINGGATE_TVX.AMB",	gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_RINGGATE_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ARROW_MDL.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ARROW_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ARROW_TEX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ARROW_TEX},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_ARROW_MAT.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_ARROW_MAT},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_OBLONG_MDL.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_OBLONG_MODEL},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_OBLONG_TEX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_OBLONG_TEX},
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_OBLONG_MAT.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_OBLONG_MAT},
#if _IPHONE
	{ GSS_BASE_PATH "G_SS/GMK/GMK_SS_OBLONG_TVX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_SS_OBLONG_TVX},
#endif // _IPHONE
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MDL.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MODEL},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_TEX.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_TEX},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MTN.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MTN},
	{ GSS_BASE_PATH "G_ZONE2/GMK/GMK_BOBBIN_MAT.AMB",		gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_BOBBIN_MAT},

	// ゲーム開始時操作説明
	{ GSS_BASE_PATH "G_SS/MSG/G_MSG_SS.AMB",				gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_START_EXP_MSG},
	{ GSS_BASE_PATH "DEMO/CMN/D_CMN_WIN.AMB",				gmGameDatLoadAllocHead,	NULL,						gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_START_EXP_WIN},
};

// Ending
static const GMS_GAMEDAT_LOAD_DATA gm_gamedat_tbl_gimmick_ending[] = {
	// パス							アロケータ					前処理							後処理								ユーザーデータ
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_LAND_1_MDL.AMB",	gmGameDatLoadAllocHead,	NULL,							gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_1_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/GMK_LAND_1_TEX.AMB",	gmGameDatLoadAllocHead,	NULL,							gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_LAND_1_TEX},
#if _WII  | _IPHONE
	{ GSS_BASE_PATH "G_ZONE1/GMK/END_SONIC_MDL.AMB",	gmGameDatLoadAllocHead,	NULL,							gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_END_SONIC_MODEL},
	{ GSS_BASE_PATH "G_ZONE1/GMK/END_SONIC_TEX.AMB",	gmGameDatLoadAllocHead,	NULL,							gmGameDatLoadProcPostGimmick,		GMD_DWORK_NO_GMK_END_SONIC_TEX},
#endif//_WII | _IPHONE
	{ GSS_BASE_PATH "G_COM/RING/RING_MDL.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_RING_MODEL},
	{ GSS_BASE_PATH "G_COM/RING/RING_TEX.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_RING_TEX},

	{ GSS_BASE_PATH "DEMO/STFRL/D_STFRL_FONT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_FONT_TEX},
	{ GSS_BASE_PATH "DEMO/CMN/D_CMN_WIN.AMB",			gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_CMN_WIN_TEX},
//	{ GSS_BASE_PATH "DEMO/STFRL/STAFF_LIST.YSD",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_RING_MODEL},
	{ GSS_BASE_PATH "DEMO/STFRL/D_STFRL_IMG000.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_SCR_IMG_TEX},
	{ GSS_BASE_PATH "DEMO/CMN/D_CMN_MSG_JP.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_CMN_MSG_JP},
	{ GSS_BASE_PATH "DEMO/CMN/D_CMN_MSG_US.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_CMN_MSG_US},
	{ GSS_BASE_PATH "DEMO/CMN/D_CMN_MSG_FR.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_CMN_MSG_FR},
	{ GSS_BASE_PATH "DEMO/CMN/D_CMN_MSG_IT.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_CMN_MSG_IT},
	{ GSS_BASE_PATH "DEMO/CMN/D_CMN_MSG_GE.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_CMN_MSG_GE},
	{ GSS_BASE_PATH "DEMO/CMN/D_CMN_MSG_SP.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_CMN_MSG_SP},
	{ GSS_BASE_PATH "DEMO/STFRL/D_STFRL_END.AMB",		gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_END_TEX},
	{ GSS_BASE_PATH "DEMO/STFRL/D_STFRL_END_JP.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_END_TEX_JP},
	{ GSS_BASE_PATH "DEMO/STFRL/D_STFRL_END_US.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_END_TEX_US},
	{ GSS_BASE_PATH "DEMO/STFRL/D_STFRL_END_FR.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_END_TEX_FR},
	{ GSS_BASE_PATH "DEMO/STFRL/D_STFRL_END_IT.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_END_TEX_IT},
	{ GSS_BASE_PATH "DEMO/STFRL/D_STFRL_END_GE.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_END_TEX_GE},
	{ GSS_BASE_PATH "DEMO/STFRL/D_STFRL_END_SP.AMB",	gmGameDatLoadAllocHead,		NULL,						gmGameDatLoadProcPostGimmick,			GMD_DWORK_NO_GMK_STFRL_END_TEX_SP},
};

/* ロード情報構造体 */
static const GMS_GAMEDAT_LOAD_INFO gm_gamedat_tbl_gimmick_common_info_tbl[1] = {
	{gm_gamedat_tbl_com_gimmick,	sizeof(gm_gamedat_tbl_com_gimmick) / sizeof(gm_gamedat_tbl_com_gimmick[0])},
};
static const GMS_GAMEDAT_LOAD_INFO gm_gamedat_tbl_gimmick_info_tbl[GSD_MAIN_STAGE_ID_MAX] = {
	// Zone1
	{gm_gamedat_tbl_gimmick01_01,	sizeof(gm_gamedat_tbl_gimmick01_01) / sizeof(gm_gamedat_tbl_gimmick01_01[0])},
	{gm_gamedat_tbl_gimmick01_02,	sizeof(gm_gamedat_tbl_gimmick01_02) / sizeof(gm_gamedat_tbl_gimmick01_02[0])},
	{gm_gamedat_tbl_gimmick01_03,	sizeof(gm_gamedat_tbl_gimmick01_03) / sizeof(gm_gamedat_tbl_gimmick01_03[0])},
	{gm_gamedat_tbl_gimmick01_BOSS,	sizeof(gm_gamedat_tbl_gimmick01_BOSS) / sizeof(gm_gamedat_tbl_gimmick01_BOSS[0])},
	
	// Zone2
	{gm_gamedat_tbl_gimmick02_01,	sizeof(gm_gamedat_tbl_gimmick02_01) / sizeof(gm_gamedat_tbl_gimmick02_01[0])},
	{gm_gamedat_tbl_gimmick02_02,	sizeof(gm_gamedat_tbl_gimmick02_02) / sizeof(gm_gamedat_tbl_gimmick02_02[0])},
	{gm_gamedat_tbl_gimmick02_03,	sizeof(gm_gamedat_tbl_gimmick02_03) / sizeof(gm_gamedat_tbl_gimmick02_03[0])},
	{gm_gamedat_tbl_gimmick02_BOSS,	sizeof(gm_gamedat_tbl_gimmick02_BOSS) / sizeof(gm_gamedat_tbl_gimmick02_BOSS[0])},
	
	// Zone3
	{gm_gamedat_tbl_gimmick03_01,	sizeof(gm_gamedat_tbl_gimmick03_01) / sizeof(gm_gamedat_tbl_gimmick03_01[0])},
	{gm_gamedat_tbl_gimmick03_02,	sizeof(gm_gamedat_tbl_gimmick03_02) / sizeof(gm_gamedat_tbl_gimmick03_02[0])},
	{gm_gamedat_tbl_gimmick03_03,	sizeof(gm_gamedat_tbl_gimmick03_03) / sizeof(gm_gamedat_tbl_gimmick03_03[0])},
	{gm_gamedat_tbl_gimmick03_BOSS,	sizeof(gm_gamedat_tbl_gimmick03_BOSS) / sizeof(gm_gamedat_tbl_gimmick03_BOSS[0])},
	
	// Zone4
	{gm_gamedat_tbl_gimmick04_01,	sizeof(gm_gamedat_tbl_gimmick04_01) / sizeof(gm_gamedat_tbl_gimmick04_01[0])},
	{gm_gamedat_tbl_gimmick04_02,	sizeof(gm_gamedat_tbl_gimmick04_02) / sizeof(gm_gamedat_tbl_gimmick04_02[0])},
	{gm_gamedat_tbl_gimmick04_03,	sizeof(gm_gamedat_tbl_gimmick04_03) / sizeof(gm_gamedat_tbl_gimmick04_03[0])},
	{gm_gamedat_tbl_gimmick04_BOSS,	sizeof(gm_gamedat_tbl_gimmick04_BOSS) / sizeof(gm_gamedat_tbl_gimmick04_BOSS[0])},
	
	// ZoneFinal
	{gm_gamedat_tbl_gimmick_Final_BOSS01,	sizeof(gm_gamedat_tbl_gimmick_Final_BOSS01) / sizeof(gm_gamedat_tbl_gimmick_Final_BOSS01[0])},
	{gm_gamedat_tbl_gimmick_Final_BOSS02,	sizeof(gm_gamedat_tbl_gimmick_Final_BOSS02) / sizeof(gm_gamedat_tbl_gimmick_Final_BOSS02[0])},
	{gm_gamedat_tbl_gimmick_Final_BOSS03,	sizeof(gm_gamedat_tbl_gimmick_Final_BOSS03) / sizeof(gm_gamedat_tbl_gimmick_Final_BOSS03[0])},
	{gm_gamedat_tbl_gimmick_Final_BOSS04,	sizeof(gm_gamedat_tbl_gimmick_Final_BOSS04) / sizeof(gm_gamedat_tbl_gimmick_Final_BOSS04[0])},
	{gm_gamedat_tbl_gimmick_Final_BOSS05,	sizeof(gm_gamedat_tbl_gimmick_Final_BOSS05) / sizeof(gm_gamedat_tbl_gimmick_Final_BOSS05[0])},
	
	// SpecialStage
	{gm_gamedat_tbl_gimmick_ss01,	sizeof(gm_gamedat_tbl_gimmick_ss01) / sizeof(gm_gamedat_tbl_gimmick_ss01[0])},
	{gm_gamedat_tbl_gimmick_ss01,	sizeof(gm_gamedat_tbl_gimmick_ss01) / sizeof(gm_gamedat_tbl_gimmick_ss01[0])},
	{gm_gamedat_tbl_gimmick_ss01,	sizeof(gm_gamedat_tbl_gimmick_ss01) / sizeof(gm_gamedat_tbl_gimmick_ss01[0])},
	{gm_gamedat_tbl_gimmick_ss01,	sizeof(gm_gamedat_tbl_gimmick_ss01) / sizeof(gm_gamedat_tbl_gimmick_ss01[0])},
	{gm_gamedat_tbl_gimmick_ss01,	sizeof(gm_gamedat_tbl_gimmick_ss01) / sizeof(gm_gamedat_tbl_gimmick_ss01[0])},
	{gm_gamedat_tbl_gimmick_ss01,	sizeof(gm_gamedat_tbl_gimmick_ss01) / sizeof(gm_gamedat_tbl_gimmick_ss01[0])},
	{gm_gamedat_tbl_gimmick_ss01,	sizeof(gm_gamedat_tbl_gimmick_ss01) / sizeof(gm_gamedat_tbl_gimmick_ss01[0])},

	// Ending
	{gm_gamedat_tbl_gimmick_ending,	sizeof(gm_gamedat_tbl_gimmick_ending) / sizeof(gm_gamedat_tbl_gimmick_ending[0])},
};



//----- Global Functions ----------------------------------------------------
// ==========================================================================
// pt
/*!
 */
// ==========================================================================

//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
