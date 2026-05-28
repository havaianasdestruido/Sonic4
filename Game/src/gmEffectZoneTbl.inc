// =======================================================================
/*!
  @file	gmEffectZoneTbl.inc
  @brief ゾーン専用エフェクト テーブル

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id$
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! ゾーン毎のテクスチャAMBのインデックステーブル
const static Sint32 gm_efct_zone_texamb_index_tbl[GSD_MAIN_ZONE_TYPE_MAX]	= {
	IDB_EFF_ZONE1_EFF_ZONE1_TEX_AMB,
	IDB_EFF_ZONE2_EFF_ZONE2_TEX_AMB,
	IDB_EFF_ZONE3_EFF_ZONE3_TEX_AMB,
	IDB_EFF_ZONE4_EFF_ZONE4_TEX_AMB,
	IDB_EFF_ZONE5_EFF_ZONE5_TEX_AMB,
	IDB_EFF_ZONESS_EFF_ZONESS_TEX_AMB,
};

//! ゾーン１専用エフェクト生成パラメータテーブル
const GMS_EFCT_ZONE_CREATE_PARAM gm_efct_zone01_create_param_tbl[GME_EFCT_Z01_IDX_MAX]	= {
	// GME_EFCT_Z01_IDX_ASHIBA
	{
		{
			IDB_EFF_ZONE1_EFF_ASHIBA_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z01_IDX_ITEM_Z1
	{
		{
			IDB_EFF_ZONE1_EFF_ITEM_Z1_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z01_IDX_OBJECT_Z1
	{
		{
			IDB_EFF_ZONE1_EFF_OBJECT_Z1_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z01_IDX_OBJECT_Z1_A3
	{
		{
			IDB_EFF_ZONE1_EFF_OBJECT_Z1_A3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z01_IDX_SMORK_B_Z1
	{
		{
			IDB_EFF_ZONE1_EFF_SMORK_B_Z1_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z01_IDX_SMORK_S_Z1
	{
		{
			IDB_EFF_ZONE1_EFF_SMORK_S_Z1_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z01_IDX_SPARK_K
	{
		{
			IDB_EFF_ZONE1_EFF_SPARK_K_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z01_IDX_TAKI_Z1
	{
		{
			IDB_EFF_ZONE1_EFF_TAKI_Z1_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z01_IDX_WALL_Z1
	{
		{
			IDB_EFF_ZONE1_EFF_WALL_Z1_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(0),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
};

//! ゾーン２専用エフェクト生成パラメータテーブル
const GMS_EFCT_ZONE_CREATE_PARAM gm_efct_zone02_create_param_tbl[GME_EFCT_Z02_IDX_MAX]	= {
	// GME_EFCT_Z02_IDX_OBJECT_Z2
	{
		{
			IDB_EFF_ZONE2_EFF_OBJECT_Z2_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z02_IDX_WLL_Z2
	{
		{
			IDB_EFF_ZONE2_EFF_WLL_Z2_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
};

//! ゾーン３専用エフェクト生成パラメータテーブル
const GMS_EFCT_ZONE_CREATE_PARAM gm_efct_zone03_create_param_tbl[GME_EFCT_Z03_IDX_MAX]	= {
	// GME_EFCT_Z03_IDX_BOMB_SMOKE_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_BOMB_SMOKE_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_BUBBLE_BIG
	{
		{
			IDB_EFF_ZONE3_EFF_BUBBLE_BIG_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_BUBBLE_LOST
	{
		{
			IDB_EFF_ZONE3_EFF_BUBBLE_LOST_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_BUBBLE_LOST_2
	{
		{
			IDB_EFF_ZONE3_EFF_BUBBLE_LOST_2_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_BUBBLE_POINT
	{
		{
			IDB_EFF_ZONE3_EFF_BUBBLE_POINT_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_CANDLE_01
	{
		{
			IDB_EFF_ZONE3_EFF_CANDLE_01_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_CANDLE_02
	{
		{
			IDB_EFF_ZONE3_EFF_CANDLE_02_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_CANDLE_03
	{
		{
			IDB_EFF_ZONE3_EFF_CANDLE_03_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_DASH_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_DASH_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_ITEM_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_ITEM_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_LIGHT_01
	{
		{
			IDB_EFF_ZONE3_EFF_LIGHT_01_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(+90),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_OBJECT_W_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_OBJECT_W_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_OBJECT_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_OBJECT_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	//GME_EFCT_Z03_IDX_PILLAR_01
	{
		{
			IDB_EFF_ZONE3_EFF_PILLAR_01_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	//GME_EFCT_Z03_IDX_PILLAR_02
	{
		{
			IDB_EFF_ZONE3_EFF_PILLAR_02_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	//GME_EFCT_Z03_IDX_PILLAR_03
	{
		{
			IDB_EFF_ZONE3_EFF_PILLAR_03_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	//GME_EFCT_Z03_IDX_PILLAR_HIT
	{
		{
			IDB_EFF_ZONE3_EFF_PILLAR_HIT_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_ROCK_01
	{
		{
			IDB_EFF_ZONE3_EFF_ROCK_01_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_ROCK_02
	{
		{
			IDB_EFF_ZONE3_EFF_ROCK_02_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_ROLLDASH_S_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_ROLLDASH_S_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-3.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_RUN_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_RUN_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SLIDER
	{
		{
			IDB_EFF_ZONE3_EFF_SLIDER_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	32.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SLIDER_02
	{
		{
			IDB_EFF_ZONE3_EFF_SLIDER_02_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	32.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SLIDER_SONIC
	{
		{
			IDB_EFF_ZONE3_EFF_SLIDER_SONIC_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	0.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SMORK_B_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_SMORK_B_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SMORK_S_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_SMORK_S_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SPARK
	{
		{
			IDB_EFF_ZONE3_EFF_SPARK_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SPARK_S
	{
		{
			IDB_EFF_ZONE3_EFF_SPARK_S_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SPIN_02
	{
		{
			IDB_EFF_ZONE3_EFF_SPIN_02_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SPIN_03
	{
		{
			IDB_EFF_ZONE3_EFF_SPIN_03_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SPRAY_L_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_SPRAY_L_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_SPRAY_S_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_SPRAY_S_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_WALL_M_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_WALL_M_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_WALL_Z3
	{
		{
			IDB_EFF_ZONE3_EFF_WALL_Z3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_WATER
	{
		{
			IDB_EFF_ZONE3_EFF_WATER_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z03_IDX_WATER_02
	{
		{
			IDB_EFF_ZONE3_EFF_WATER_02_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
};

//! ゾーン４専用エフェクト生成パラメータテーブル
const GMS_EFCT_ZONE_CREATE_PARAM gm_efct_zone04_create_param_tbl[GME_EFCT_Z04_IDX_MAX]	= {
	// GME_EFCT_Z04_IDX_LIGHT_Z4
	{
		{
			IDB_EFF_ZONE4_EFF_LIGHT_Z4_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z04_IDX_PILLAR_Z4
	{
		{
			IDB_EFF_ZONE4_EFF_PILLAR_Z4_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z04_IDX_WALL_M_Z4
	{
		{
			IDB_EFF_ZONE4_EFF_WALL_M_Z4_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_Z04_IDX_WLL_Z4
	{
		{
			IDB_EFF_ZONE4_EFF_WLL_Z4_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
};

//! ゾーンファイナル専用エフェクト生成パラメータテーブル
const GMS_EFCT_ZONE_CREATE_PARAM gm_efct_zone_final_create_param_tbl[GME_EFCT_ZFINAL_IDX_MAX]	= {
	// GME_EFCT_ZFINAL_IDX_LIGHT_Z5
	{
		{
			IDB_EFF_ZONE5_EFF_LIGHT_Z5_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
};

//! ゾーンスペシャルステージ専用エフェクト生成パラメータテーブル
const GMS_EFCT_ZONE_CREATE_PARAM gm_efct_zone_ss_create_param_tbl[GME_EFCT_ZSS_IDX_MAX]	= {
	// GME_EFCT_ZSS_IDX_1UP
	{
		{
			IDB_EFF_ZONESS_EFF_1UP_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_CHAOS_E_1
	{
		{
			IDB_EFF_ZONESS_EFF_CHAOS_E_1_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_CHAOS_E_2
	{
		{
			IDB_EFF_ZONESS_EFF_CHAOS_E_2_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_CHAOS_E_3
	{
		{
			IDB_EFF_ZONESS_EFF_CHAOS_E_3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_CHAOS_E_4
	{
		{
			IDB_EFF_ZONESS_EFF_CHAOS_E_4_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_CHAOS_E_5
	{
		{
			IDB_EFF_ZONESS_EFF_CHAOS_E_5_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_CHAOS_E_6
	{
		{
			IDB_EFF_ZONESS_EFF_CHAOS_E_6_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_CHAOS_E_7
	{
		{
			IDB_EFF_ZONESS_EFF_CHAOS_E_7_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_LOST
	{
		{
			IDB_EFF_ZONESS_EFF_LOST_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_RING_GATE
	{
		{
			IDB_EFF_ZONESS_EFF_RING_GATE_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_TIME_DE
	{
		{
			IDB_EFF_ZONESS_EFF_TIME_DE_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_TIME_DOWN
	{
		{
			IDB_EFF_ZONESS_EFF_TIME_DOWN_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_TIME_EN
	{
		{
			IDB_EFF_ZONESS_EFF_TIME_EN_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_TIME_ES
	{
		{
			IDB_EFF_ZONESS_EFF_TIME_ES_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_TIME_FR
	{
		{
			IDB_EFF_ZONESS_EFF_TIME_FR_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_TIME_IT
	{
		{
			IDB_EFF_ZONESS_EFF_TIME_IT_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_TIME_JP
	{
		{
			IDB_EFF_ZONESS_EFF_TIME_JP_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
	// GME_EFCT_ZSS_IDX_TIME_UP
	{
		{
			IDB_EFF_ZONESS_EFF_TIME_UP_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//model_dwork_no
		-1,//mdl_ambtex_idx
	},
};

//! ゾーン専用エフェクト生成情報テーブル
const GMS_EFCT_ZONE_CREATE_INFO gm_efct_zone_create_info[GSD_MAIN_ZONE_TYPE_MAX]	= {
	{	gm_efct_zone01_create_param_tbl,	GME_EFCT_Z01_IDX_MAX,	},
	{	gm_efct_zone02_create_param_tbl,	GME_EFCT_Z02_IDX_MAX,	},
	{	gm_efct_zone03_create_param_tbl,	GME_EFCT_Z03_IDX_MAX,	},
	{	gm_efct_zone04_create_param_tbl,	GME_EFCT_Z04_IDX_MAX,	},
	{	gm_efct_zone_final_create_param_tbl,	GME_EFCT_ZFINAL_IDX_MAX,	},
	{	gm_efct_zone_ss_create_param_tbl,	GME_EFCT_ZSS_IDX_MAX,	},	// SpecialStage
};


/*------ Global Functions ----------------------------------------------*/
/*------ Static Functions ----------------------------------------------*/
// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================

