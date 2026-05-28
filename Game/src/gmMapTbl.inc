// ==========================================================================
/*!
  @file gmMapTbl.inc
  @brief MAP管理設定 データテーブル

  @author Satoshi Akitomi
				Copyright(c) 2010 Dimps

  $Id: gmMapTbl.inc 9635 2009-03-03 09:27:00Z use1081 $
  $Date:: 2009-03-03 18:27:00 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*                               ZONE 2                                       */
/*                                                                            */
/*                                                                            */
/* ========================================================================== */

/// Z2用アルファ描画テクスチャ設定
static const s32 gm_map_prim_draw_tvx_alpha_set_z2[IDB_ZONE2_T_PC_Z2_ILM_BUILA_DDS] = {
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_TEX4_DDS
	GMD_MAP_PRIM_DRAW_OP_ADD, // IDB_ZONE2_T_PC_Z2_BALLOON_ILMA_DDS
	GMD_MAP_PRIM_DRAW_OP_ADD, // IDB_ZONE2_T_PC_Z2_BALLOON_ILMB_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_ILM_BUILB_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_ILM_FLOWER_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_TEX1_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_TEX3_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_TEX2_DDS
	GMD_MAP_PRIM_DRAW_OP_ADD, // IDB_ZONE2_T_PC_Z2_ILM_ASHIBA_DDS
	GMD_MAP_PRIM_DRAW_OP_ADD, // IDB_ZONE2_T_PC_Z2_CODE_ILM_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_ILMBELT_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_3MARK_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_PALM_DDS
	GMD_MAP_PRIM_DRAW_OP_BLEND, // IDB_ZONE2_T_PC_Z2_TEX7_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_ROLL_ILM_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_ILM_V_DDS
	GMD_MAP_PRIM_DRAW_OP_ADD, // IDB_ZONE2_T_PC_Z2_ILMC_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_ILMA_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_PANEL_DDS
	GMD_MAP_PRIM_DRAW_OP_ADD, // IDB_ZONE2_T_PC_Z2_ILMB_DDS
		GMD_MAP_PRIM_DRAW_OP_NON, // IDB_ZONE2_T_PC_Z2_ILM_BUILA_DDS
};


/// Z2用テクスチャ数
static const s32 gm_map_prim_draw_tvx_texture_z2_num = IDB_ZONE2_T_PC_Z2_ILM_BUILA_DDS;

// ==========================================================================
// TVXUVモーション テクスチャID-描画管理インデックス
// ==========================================================================
/// Z2用index
static const GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX gm_map_prim_draw_tvx_mgr_index_tbl_z2[] = {
	// テクスチャID					描画管理ID
	{IDB_ZONE2_T_PC_Z2_PALM_DDS - 1,	0},
	{IDB_ZONE2_T_PC_Z2_ILMA_DDS - 1,	1},
	{IDB_ZONE2_T_PC_Z2_ILMB_DDS - 1,	2},
	{IDB_ZONE2_T_PC_Z2_ILMC_DDS - 1,	3},
	{IDB_ZONE2_T_PC_Z2_PANEL_DDS - 1,	4},
	{IDB_ZONE2_T_PC_Z2_ILM_BUILA_DDS - 1,	5},
	{IDB_ZONE2_T_PC_Z2_ILM_BUILB_DDS - 1,	6},
	{IDB_ZONE2_T_PC_Z2_ILMBELT_DDS - 1,	7},
	{IDB_ZONE2_T_PC_Z2_3MARK_DDS - 1,	8},
	{IDB_ZONE2_T_PC_Z2_ILM_V_DDS - 1,	9},
	{IDB_ZONE2_T_PC_Z2_BALLOON_ILMA_DDS - 1,	10},
	{IDB_ZONE2_T_PC_Z2_BALLOON_ILMB_DDS - 1,	11},
	{IDB_ZONE2_T_PC_Z2_CODE_ILM_DDS - 1,	12},
	{IDB_ZONE2_T_PC_Z2_ILM_FLOWER_DDS - 1,	13},
	{IDB_ZONE2_T_PC_Z2_ROLL_ILM_DDS - 1,	14},
	{IDB_ZONE2_T_PC_Z2_ILM_ASHIBA_DDS - 1,	15},
};

/// Z2用index数
static const s32 gm_map_prim_draw_tvx_mgr_index_tbl_z2_num = sizeof(gm_map_prim_draw_tvx_mgr_index_tbl_z2) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX);

// ==========================================================================
// TVXUVモーション 描画ID-FRAME管理データ
// ==========================================================================
/// Z2用 IDB_ZONE2_T_PC_Z2_PALM_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_PALM_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	{	 7,					3},
	{	 8,					3},
	{	 9,					3},
	{	10,					3},
	{	11,					3},
	{	12,					3},
	{	13,					3},
	{	14,					3},
	{	15,					3},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM*_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 7,					12},
	
	//  1
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	//  2
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	//  3
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	//  4
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	//  5
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	//  6
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	//  7
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	//  8
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	//  9
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	// 10
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	// 11
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	// 12
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	// 13
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	// 14
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	// 15
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	// 16
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	
	//  1
	{	 9,					6},
	{	 7,					6},
	{	 8,					6},
	{	 7,					6},
	//  2
	{	 9,					6},
	{	 7,					6},
	{	 8,					6},
	{	 7,					6},
	//  3
	{	 9,					6},
	{	 7,					6},
	{	 8,					6},
	{	 7,					6},
	//  4
	{	 9,					6},
	{	 7,					6},
	{	 8,					6},
	{	 7,					6},
	
	//  1
	{	10,					6},
	{	 7,					6},
	//  2
	{	10,					6},
	{	 7,					6},
	//  3
	{	10,					6},
	{	 7,					6},
	//  4
	{	10,					6},
	{	 7,					6},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_PANEL_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_PANEL_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	{	 7,					3},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM_BUILA_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_BUILA_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					4},
	{	 1,					4},
	{	 2,					4},
	{	 3,					4},
	{	 4,					4},
	{	 5,					4},
	{	 6,					4},
	{	 7,					4},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM_BUILB_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_BUILB_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					4},
	{	 1,					4},
	{	 2,					4},
	{	 3,					4},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILMBELT_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILMBELT_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					5},
	{	 1,					5},
	{	 2,					5},
	{	 3,					5},
	{	 4,					5},
	{	 5,					5},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_3MARK_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_3MARK_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					4},
	{	 1,					4},
	{	 2,					4},
	{	 3,					4},
	{	 4,					4},
	{	 5,					4},
	{	 6,					4},
	{	 7,					4},
	{	 8,					4},
	{	 9,					4},
	{	10,					4},
	{	11,					4},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM_V_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_V_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 3,					3},
	{	 4,					3},
	{	 5,					3},
	{	 6,					3},
	{	 7,					3},
	{	 8,					3},
	{	 9,					3},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_BALLOON_ILM*_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_BALLOON_ILM_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					3},
	{	 1,					3},
	{	 2,					3},
	{	 1,					3},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_CODE_ILM_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_CODE_ILM_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					6},
	{	 1,					6},
	{	 2,					6},
	{	 3,					6},
	{	 4,					6},
	{	 5,					6},
	{	 6,					6},
	{	 7,					6},
	{	 8,					6},
	{	 9,					6},
	{	10,					6},
	{	11,					6},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM_FLOWER_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_FLOWER_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	
	// 1
	{	 0,					30},
	{	 1,					30},
	{	 2,					30},
	{	 3,					30},
	// 2
	{	 0,					30},
	{	 1,					30},
	{	 2,					30},
	{	 3,					30},
	// 3
	{	 0,					30},
	{	 1,					30},
	{	 2,					30},
	{	 3,					30},
	// 4
	{	 0,					30},
	{	 1,					30},
	{	 2,					30},
	{	 3,					30},
	// 5
	{	 0,					30},
	{	 1,					30},
	{	 2,					30},
	{	 3,					30},
	
	// 1
	{	 4,					30},
	{	 5,					30},
	{	 6,					30},
	{	 7,					30},
	// 2
	{	 4,					30},
	{	 5,					30},
	{	 6,					30},
	{	 7,					30},
	// 3
	{	 4,					30},
	{	 5,					30},
	{	 6,					30},
	{	 7,					30},
	// 4
	{	 4,					30},
	{	 5,					30},
	{	 6,					30},
	{	 7,					30},
	// 5
	{	 4,					30},
	{	 5,					30},
	{	 6,					30},
	{	 7,					30},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ROLL_ILM_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ROLL_ILM_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					4},
	{	 1,					4},
	{	 2,					4},
	{	 3,					4},
	{	 4,					4},
	{	 5,					4},
	{	 6,					4},
	{	 7,					4},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM_ASHIBA_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_ASHIBA_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					4},
	{	 1,					4},
	{	 2,					4},
	{	 3,					4},
	{	 4,					4},
	{	 5,					4},
	{	 6,					4},
	{	 7,					4},
};


/// Z2用モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR* gm_map_prim_draw_tvx_mgr_tbl_z2[] = {
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_PALM_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_PANEL_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_BUILA_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_BUILB_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILMBELT_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_3MARK_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_V_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_BALLOON_ILM_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_BALLOON_ILM_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_CODE_ILM_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_FLOWER_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ROLL_ILM_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_ASHIBA_DDS,
};

/// Z2用モーション管理テーブル 数
static const s32 gm_map_prim_draw_tvx_mgr_tbl_z2_num[] = {
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_PALM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_PANEL_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_BUILA_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_BUILB_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILMBELT_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_3MARK_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_V_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_BALLOON_ILM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_BALLOON_ILM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_CODE_ILM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_FLOWER_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ROLL_ILM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z2_Z2_ILM_ASHIBA_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
};

// ==========================================================================
// UVモーション 描画UV管理データ
// ==========================================================================
/// Z2用 IDB_ZONE2_T_PC_Z2_PALM_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_PALM_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},

/*04*/	{0.00f,0.25f},
/*05*/	{0.25f,0.25f},
/*06*/	{0.50f,0.25f},
/*07*/	{0.75f,0.25f},

/*08*/	{0.00f,0.50f},
/*09*/	{0.25f,0.50f},
/*10*/	{0.50f,0.50f},
/*11*/	{0.75f,0.50f},

/*12*/	{0.00f,0.75f},
/*13*/	{0.25f,0.75f},
/*14*/	{0.50f,0.75f},
/*15*/	{0.75f,0.75f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM*_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},
/*04*/	{0.00f,0.25f},
/*05*/	{0.25f,0.25f},
/*06*/	{0.50f,0.25f},
/*07*/	{0.75f,0.25f}, // vanish
/*08*/	{0.00f,0.50f}, // 1
/*09*/	{0.25f,0.50f}, // 2
/*10*/	{0.50f,0.50f}, // 3
};

/// Z2用 IDB_ZONE2_T_PC_Z2_PANEL_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_PANEL_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},
/*04*/	{0.00f,0.25f},
/*05*/	{0.25f,0.25f},
/*06*/	{0.50f,0.25f},
/*07*/	{0.75f,0.25f},
/*08*/	{0.00f,0.50f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM_BUILA_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_BUILA_DDS[] = {
/*00*/	{0.00f,0.00f},
/*04*/	{0.25f,0.00f},
/*05*/	{0.50f,0.00f},
/*01*/	{0.75f,0.00f},
/*02*/	{0.00f,0.50f},
/*06*/	{0.25f,0.50f},
/*07*/	{0.50f,0.50f},
/*03*/	{0.75f,0.50f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM_BUILB_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_BUILB_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILMBELT_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILMBELT_DDS[] = {
/*00*/	{0.00f,0.000f},
/*01*/	{0.00f,0.125f},
/*02*/	{0.00f,0.250f},
/*03*/	{0.00f,0.375f},
/*04*/	{0.00f,0.500f},
/*05*/	{0.00f,0.625f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_3MARK_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_3MARK_DDS[] = {
/*00*/	{0.00000000f,0.00f},
/*01*/	{0.33203125f,0.00f},
/*02*/	{0.66406250f,0.00f},
/*03*/	{0.00000000f,0.25f},
/*04*/	{0.33203125f,0.25f},
/*05*/	{0.66406250f,0.25f},
/*06*/	{0.00000000f,0.50f},
/*07*/	{0.33203125f,0.50f},
/*08*/	{0.66406250f,0.50f},
/*09*/	{0.00000000f,0.75f},
/*10*/	{0.33203125f,0.75f},
/*11*/	{0.66406250f,0.75f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM_V_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_V_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.20f,0.00f},
/*02*/	{0.40f,0.00f},
/*03*/	{0.60f,0.00f},
/*04*/	{0.80f,0.00f},
/*05*/	{0.00f,0.50f},
/*06*/	{0.20f,0.50f},
/*07*/	{0.40f,0.50f},
/*08*/	{0.60f,0.50f},
/*09*/	{0.80f,0.50f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_BALLOON_ILM*_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_BALLOON_ILM_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.50f,0.00f},
/*02*/	{0.00f,0.50f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_CODE_ILM_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_CODE_ILM_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},
/*04*/	{0.00f,0.25f},
/*05*/	{0.25f,0.25f},
/*06*/	{0.50f,0.25f},
/*07*/	{0.75f,0.25f},
/*08*/	{0.00f,0.50f},
/*09*/	{0.25f,0.50f},
/*10*/	{0.50f,0.50f},
/*11*/	{0.75f,0.50f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM_FLOWER_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_FLOWER_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},
/*04*/	{0.00f,0.50f},
/*05*/	{0.25f,0.50f},
/*06*/	{0.50f,0.50f},
/*07*/	{0.75f,0.50f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ROLL_ILM_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ROLL_ILM_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},
/*04*/	{0.00f,0.50f},
/*05*/	{0.25f,0.50f},
/*06*/	{0.50f,0.50f},
/*07*/	{0.75f,0.50f},
};

/// Z2用 IDB_ZONE2_T_PC_Z2_ILM_ASHIBA_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_ASHIBA_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.50f,0.00f},
/*02*/	{0.00f,0.25f},
/*03*/	{0.50f,0.25f},
/*04*/	{0.00f,0.50f},
/*05*/	{0.50f,0.50f},
/*06*/	{0.00f,0.75f},
/*07*/	{0.50f,0.75f},
};

/// Z2用UV管理テーブル
static const NNS_TEXCOORD* gm_map_prim_draw_tvx_uv_mgr_tbl_z2[] = {
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_PALM_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_PANEL_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_BUILA_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_BUILB_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILMBELT_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_3MARK_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_V_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_BALLOON_ILM_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_BALLOON_ILM_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_CODE_ILM_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_FLOWER_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ROLL_ILM_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z2_Z2_ILM_ASHIBA_DDS,
};


/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*                               ZONE 3                                       */
/*                                                                            */
/*                                                                            */
/* ========================================================================== */

/// Z3用テクスチャ数
static const s32 gm_map_prim_draw_tvx_texture_z3_num = IDB_ZONE3_T_PC_Z3_ANIM_B_DDS;

// ==========================================================================
// TVXUVモーション テクスチャID-描画管理インデックス
// ==========================================================================
/// Z3用index
static const GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX gm_map_prim_draw_tvx_mgr_index_tbl_z3[] = {
	// テクスチャID					描画管理ID
	{IDB_ZONE3_T_PC_Z3_ANIM_B_DDS - 1,	0},
};

/// Z3用index数
static const s32 gm_map_prim_draw_tvx_mgr_index_tbl_z3_num = sizeof(gm_map_prim_draw_tvx_mgr_index_tbl_z3) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX);

// ==========================================================================
// TVXUVモーション 描画ID-FRAME管理データ
// ==========================================================================
/// Z3用 IDB_ZONE3_T_PC_Z3_ANIM_B_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z3_Z3_ANIM_B_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					2},
	{	 1,					2},
	{	 2,					2},
	{	 3,					2},
	{	 2,					2},
	{	 3,					2},
	{	 2,					2},
	{	 1,					2},
	{	 0,					2},
	{	 1,					2},
	{	 2,					2},
	{	 3,					2},
	{	 2,					2},
	{	 1,					2},
	{	 0,					2},
	{	 2,					2},
};

/// Z3用モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR* gm_map_prim_draw_tvx_mgr_tbl_z3[] = {
	gm_map_prim_draw_tvx_mgr_tbl_z3_Z3_ANIM_B_DDS,
};

/// Z3用モーション管理テーブル 数
static const s32 gm_map_prim_draw_tvx_mgr_tbl_z3_num[] = {
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z3_Z3_ANIM_B_DDS)  / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
};

// ==========================================================================
// UVモーション 描画UV管理データ
// ==========================================================================
/// Z3用 IDB_ZONE3_T_PC_Z3_ANIM_B_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z3_Z3_ANIM_B_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.50f,0.00f},
/*02*/	{0.00f,0.50f},
/*03*/	{0.50f,0.50f},
};

/// Z3用UV管理テーブル
static const NNS_TEXCOORD* gm_map_prim_draw_tvx_uv_mgr_tbl_z3[] = {
	gm_map_prim_draw_tvx_uv_mgr_tbl_z3_Z3_ANIM_B_DDS,
};


/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*                               ZONE 4                                       */
/*                                                                            */
/*                                                                            */
/* ========================================================================== */

/// Z4用テクスチャ数
static const s32 gm_map_prim_draw_tvx_texture_z4_num = IDB_ZONE4_T_PC_Z4_TANK_02_STOP_I_DDS;

// ==========================================================================
// TVXUVモーション テクスチャID-描画管理インデックス
// ==========================================================================
/// Z4用index
static const GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX gm_map_prim_draw_tvx_mgr_index_tbl_z4[] = {
	// テクスチャID					描画管理ID
	{IDB_ZONE4_T_PC_MID_2_DRM_A_I_DDS    - 1,	0},
	{IDB_ZONE4_T_PC_MID_2_PIPE_I_DDS     - 1,	1},
	{IDB_ZONE4_T_PC_MID_2_PISTON_I_DDS   - 1,	2},
	{IDB_ZONE4_T_PC_Z4_TANK_02_ACT_I_DDS - 1,	3},
};

/// Z4用index数
static const s32 gm_map_prim_draw_tvx_mgr_index_tbl_z4_num = sizeof(gm_map_prim_draw_tvx_mgr_index_tbl_z4) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX);

// ==========================================================================
// TVXUVモーション 描画ID-FRAME管理データ
// ==========================================================================
/// Z4用 IDB_ZONE4_T_PC_MID_2_DRM_A_I_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z4_MID_2_DRM_A_I_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					2},
	{	 1,					2},
	{	 2,					2},
	{	 3,					2},
	{	 4,					2},
	{	 5,					2},
	{	 6,					2},
	{	 7,					2},
	{	 8,					2},
	{	 9,					2},
	{	10,					2},
	{	11,					2},
	{	12,					2},
	{	13,					2},
	{	14,					2},
	{	15,					2},
};

/// Z4用 IDB_ZONE4_T_PC_MID_2_PIPE_I_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z4_MID_2_PIPE_I_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					5},
	{	 1,					5},
	{	 2,					5},
	{	 3,					5},
	{	 4,					5},
	{	 5,					5},
	{	 6,					5},
	{	 7,					5},
	{	 8,					5},
	{	 9,					5},
	{	10,					5},
	{	11,					5},
	{	12,					5},
	{	13,					5},
	{	14,					5},
	{	15,					5},
	{	16,					5},
	{	17,					5},
	{	18,					5},
	{	19,					5},
	{	20,					5},
	{	21,					5},
	{	22,					5},
	{	23,					5},
	{	24,					5},
	{	25,					5},
	{	26,					5},
	{	27,					5},
	{	28,					5},
	{	29,					5},
	{	30,					5},
	{	31,					5},
};

/// Z4用 IDB_ZONE4_T_PC_MID_2_PISTON_I_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z4_MID_2_PISTON_I_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					1},
	{	 1,					1},
	{	 2,					1},
	{	 3,					1},
	{	 4,					1},
	{	 5,					1},
	{	 6,					1},
	{	 7,					1},
	{	 8,					1},
	{	 9,					1},
	{	10,					1},
	{	11,					1},
	{	12,					1},
	{	13,					1},
	{	14,					1},
	{	15,					1},
};

/// Z4用 IDB_ZONE4_T_PC_Z4_TANK_02_ACT_I_DDS モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_z4_Z4_TANK_02_ACT_I_DDS[] = {
	// 使用UVモーションID	描画フレーム数
	{	 0,					2},
	{	 1,					2},
	{	 2,					2},
	{	 3,					2},
	{	 4,					2},
	{	 5,					2},
	{	 6,					2},
	{	 7,					2},
	{	 6,					2},
	{	 5,					2},
	{	 4,					2},
	{	 3,					2},
	{	 2,					2},
	{	 1,					2},
};

/// Z4用モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR* gm_map_prim_draw_tvx_mgr_tbl_z4[] = {
	gm_map_prim_draw_tvx_mgr_tbl_z4_MID_2_DRM_A_I_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z4_MID_2_PIPE_I_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_z4_MID_2_DRM_A_I_DDS, // 共用
	gm_map_prim_draw_tvx_mgr_tbl_z4_Z4_TANK_02_ACT_I_DDS,
};

/// Z4用モーション管理テーブル 数
static const s32 gm_map_prim_draw_tvx_mgr_tbl_z4_num[] = {
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z4_MID_2_DRM_A_I_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z4_MID_2_PIPE_I_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z4_MID_2_PISTON_I_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_z4_Z4_TANK_02_ACT_I_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
};

// ==========================================================================
// UVモーション 描画UV管理データ
// ==========================================================================
/// Z4用 IDB_ZONE4_T_PC_MID_2_DRM_A_I_DDS UV管理テーブル
/// Z4用 IDB_ZONE4_T_PC_MID_2_PISTON_I_DDS UV管理テーブル(共用)
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z4_MID_2_DRM_A_I_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},
/*04*/	{0.00f,0.25f},
/*05*/	{0.25f,0.25f},
/*06*/	{0.50f,0.25f},
/*07*/	{0.75f,0.25f},
/*08*/	{0.00f,0.50f},
/*09*/	{0.25f,0.50f},
/*10*/	{0.50f,0.50f},
/*11*/	{0.75f,0.50f},
/*12*/	{0.00f,0.75f},
/*13*/	{0.25f,0.75f},
/*14*/	{0.50f,0.75f},
/*15*/	{0.75f,0.75f},
};

/// Z4用 IDB_ZONE4_T_PC_MID_2_PIPE_I_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z4_MID_2_PIPE_I_DDS[] = {
/*00*/	{0.00f,0.000f},
/*01*/	{0.25f,0.000f},
/*02*/	{0.50f,0.000f},
/*03*/	{0.75f,0.000f},
/*00*/	{0.00f,0.125f},
/*01*/	{0.25f,0.125f},
/*02*/	{0.50f,0.125f},
/*03*/	{0.75f,0.125f},
/*04*/	{0.00f,0.250f},
/*05*/	{0.25f,0.250f},
/*06*/	{0.50f,0.250f},
/*07*/	{0.75f,0.250f},
/*04*/	{0.00f,0.375f},
/*05*/	{0.25f,0.375f},
/*06*/	{0.50f,0.375f},
/*07*/	{0.75f,0.375f},
/*08*/	{0.00f,0.500f},
/*09*/	{0.25f,0.500f},
/*10*/	{0.50f,0.500f},
/*11*/	{0.75f,0.500f},
/*08*/	{0.00f,0.625f},
/*09*/	{0.25f,0.625f},
/*10*/	{0.50f,0.625f},
/*11*/	{0.75f,0.625f},
/*12*/	{0.00f,0.750f},
/*13*/	{0.25f,0.750f},
/*14*/	{0.50f,0.750f},
/*15*/	{0.75f,0.750f},
/*12*/	{0.00f,0.875f},
/*13*/	{0.25f,0.875f},
/*14*/	{0.50f,0.875f},
/*15*/	{0.75f,0.875f},
};

#if 0
/// Z4用 IDB_ZONE4_T_PC_MID_2_PISTON_I_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z4_MID_2_PISTON_I_DDS[] = {
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},
/*04*/	{0.00f,0.25f},
/*05*/	{0.25f,0.25f},
/*06*/	{0.50f,0.25f},
/*07*/	{0.75f,0.25f},
/*08*/	{0.00f,0.50f},
/*09*/	{0.25f,0.50f},
};
#endif // 0

/// Z4用 IDB_ZONE4_T_PC_Z4_TANK_02_ACT_I_DDS UV管理テーブル
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_z4_Z4_TANK_02_ACT_I_DDS[] = {
/*00*/	{0.000f,0.00f},
/*01*/	{0.250f,0.00f},
/*02*/	{0.500f,0.00f},
/*03*/	{0.750f,0.00f},
/*04*/	{0.000f,0.50f},
/*05*/	{0.250f,0.50f},
/*06*/	{0.500f,0.50f},
/*07*/	{0.750f,0.50f},
};

/// Z4用UV管理テーブル
static const NNS_TEXCOORD* gm_map_prim_draw_tvx_uv_mgr_tbl_z4[] = {
	gm_map_prim_draw_tvx_uv_mgr_tbl_z4_MID_2_DRM_A_I_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z4_MID_2_PIPE_I_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_z4_MID_2_DRM_A_I_DDS, // 共用
	gm_map_prim_draw_tvx_uv_mgr_tbl_z4_Z4_TANK_02_ACT_I_DDS,
};


/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*                               ZONE F                                       */
/*                                                                            */
/*                                                                            */
/* ========================================================================== */

/// ZF用テクスチャ数
static const s32 gm_map_prim_draw_tvx_texture_zf_num = IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM08_DDS;

// ==========================================================================
// TVXUVモーション テクスチャID-描画管理インデックス
// ==========================================================================
/// ZF用index
static const GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX gm_map_prim_draw_tvx_mgr_index_tbl_zf[] = {
	// テクスチャID					描画管理ID
	{IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM01_DDS - 1,	0},
	{IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM03_DDS - 1,	1},
	{IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM04_DDS - 1,	2},
	{IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM05_DDS - 1,	3},
	{IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM06_DDS - 1,	4},
	{IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM07_DDS - 1,	5},
	{IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM08_DDS - 1,	6},
};

/// ZF用index数
static const s32 gm_map_prim_draw_tvx_mgr_index_tbl_zf_num = sizeof(gm_map_prim_draw_tvx_mgr_index_tbl_zf) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX);

// ==========================================================================
// TVXUVモーション 描画ID-FRAME管理データ
// ==========================================================================
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM01_DDS モーション管理テーブル
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM03_DDS モーション管理テーブル(共用)
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM04_DDS モーション管理テーブル(共用)
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM05_DDS モーション管理テーブル(共用)
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM06_DDS モーション管理テーブル(共用)
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM07_DDS モーション管理テーブル(共用)
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM08_DDS モーション管理テーブル(共用)
static const GMS_MAP_PRIM_DRAW_TVX_MGR gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS[] = {
	// 使用UVモーションID	描画フレーム数
#if 1
	{	 0,					4},
	{	 1,					4},
	{	 2,					4},
	{	 3,					4},
	{	 4,					4},
	{	 5,					4},
	{	 6,					4},
	{	 7,					4},
#else
	{	 0,					2},
	{	 1,					2},
	{	 2,					2},
	{	 3,					2},
	{	 4,					2},
	{	 5,					2},
	{	 6,					2},
	{	 7,					2},
	{	 8,					2},
	{	 9,					2},
	{	10,					2},
	{	11,					2},
	{	12,					2},
	{	13,					2},
	{	14,					2},
	{	15,					2},
#endif // 0
};


/// ZF用モーション管理テーブル
static const GMS_MAP_PRIM_DRAW_TVX_MGR* gm_map_prim_draw_tvx_mgr_tbl_zf[] = {
	gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS,
	gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
	gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
	gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
	gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
	gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
	gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
};

/// ZF用モーション管理テーブル 数
static const s32 gm_map_prim_draw_tvx_mgr_tbl_zf_num[] = {
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR),
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR), // 共用
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR), // 共用
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR), // 共用
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR), // 共用
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR), // 共用
	sizeof(gm_map_prim_draw_tvx_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS) / sizeof(GMS_MAP_PRIM_DRAW_TVX_MGR), // 共用
};

// ==========================================================================
// UVモーション 描画UV管理データ
// ==========================================================================
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM01_DDS UV管理テーブル
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM03_DDS UV管理テーブル(共用)
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM04_DDS UV管理テーブル(共用)
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM05_DDS UV管理テーブル(共用)
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM06_DDS UV管理テーブル(共用)
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM07_DDS UV管理テーブル(共用)
/// ZF用 IDB_ZONEF_T_PC_ZF_MAP_IPHONE_TEXANIM08_DDS UV管理テーブル(共用)
static const NNS_TEXCOORD gm_map_prim_draw_tvx_uv_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS[] = {
#if 1
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},
/*04*/	{0.00f,0.50f},
/*05*/	{0.25f,0.50f},
/*06*/	{0.50f,0.50f},
/*07*/	{0.75f,0.50f},
#else
/*00*/	{0.00f,0.00f},
/*01*/	{0.25f,0.00f},
/*02*/	{0.50f,0.00f},
/*03*/	{0.75f,0.00f},
/*04*/	{0.00f,0.25f},
/*05*/	{0.25f,0.25f},
/*06*/	{0.50f,0.25f},
/*07*/	{0.75f,0.25f},
/*08*/	{0.00f,0.50f},
/*09*/	{0.25f,0.50f},
/*10*/	{0.50f,0.50f},
/*11*/	{0.75f,0.50f},
/*12*/	{0.00f,0.75f},
/*13*/	{0.25f,0.75f},
/*14*/	{0.50f,0.75f},
/*15*/	{0.75f,0.75f},
#endif // 0
};


/// ZF用UV管理テーブル
static const NNS_TEXCOORD* gm_map_prim_draw_tvx_uv_mgr_tbl_zf[] = {
	gm_map_prim_draw_tvx_uv_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS,
	gm_map_prim_draw_tvx_uv_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
	gm_map_prim_draw_tvx_uv_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
	gm_map_prim_draw_tvx_uv_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
	gm_map_prim_draw_tvx_uv_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
	gm_map_prim_draw_tvx_uv_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
	gm_map_prim_draw_tvx_uv_mgr_tbl_zf_ZF_MAP_IPHONE_TEXANIM_DDS, // 共用
};


