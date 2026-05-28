// =======================================================================
/*!
  @file	gmEffectCmnTbl.inc
  @brief 共通エフェクト テーブル

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
const GMS_EFCT_CMN_CREATE_PARAM gm_efct_cmn_create_param_tbl[GME_EFCT_CMN_IDX_MAX]	= {
	// GME_EFCT_CMN_IDX_AURA_00
	{
		{
			IDB_EFF_CMN_EFF_AURA_00_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	6.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_AURA_01
	{
		{
			IDB_EFF_CMN_EFF_AURA_01_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_AURA_02
	{
		{
			IDB_EFF_CMN_EFF_AURA_02_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_AURA_03
	{
		{
			IDB_EFF_CMN_EFF_AURA_03_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BARRIER
	{
		{
			IDB_EFF_CMN_EFF_BARRIER_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	6.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BARRIER_01
	{
		{
			IDB_EFF_CMN_EFF_BARRIER_01_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX),	//init_flag
			{.0f,	6.0f/3.2f,	.0f},	//disp_ofst // SCALE_BY_MTXの場合はスケール前の値を設定
			{0,		0,		0},		//disp_rot
			3.2f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			IDB_EFF_CMN_EFF_BARRIER_ZNO,
		},
		IDB_EFF_CMN_EFF_CMN_TEX_MD_BA_AMB,
	},
	// GME_EFCT_CMN_IDX_BARRIER_LOST
	{
		{
			IDB_EFF_CMN_EFF_BARRIER_LOST_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	6.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BOMB
	{
		{
			IDB_EFF_CMN_EFF_BOMB_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BOMB_BIG
	{
		{
			IDB_EFF_CMN_EFF_BOMB_BIG_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BOMB_KAMA
	{
		{
			IDB_EFF_CMN_EFF_BOMB_KAMA_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BOMB_SMOKE
	{
		{
			IDB_EFF_CMN_EFF_BOMB_SMOKE_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BRAKE
	{
		{
			IDB_EFF_CMN_EFF_BRAKE_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	3.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BRAKE_S
	{
		{
			IDB_EFF_CMN_EFF_BRAKE_S_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	3.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BUBBLE
	{
		{
			IDB_EFF_CMN_EFF_BUBBLE_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	8.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BULLET
	{
		{
			IDB_EFF_CMN_EFF_BULLET_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BULLET_CORE
	{
		{
			IDB_EFF_CMN_EFF_BULLET_CORE_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BUMPER
	{
		{
			IDB_EFF_CMN_EFF_BUMPER_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BUMPER_1
	{
		{
			IDB_EFF_CMN_EFF_BUMPER_1_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BUMPER_2
	{
		{
			IDB_EFF_CMN_EFF_BUMPER_2_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_BUMPER_LOST
	{
		{
			IDB_EFF_CMN_EFF_BUMPER_LOST_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_CANON
	{
		{
			IDB_EFF_CMN_EFF_CANON_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_CAP_KEY1
	{
		{
			IDB_EFF_CMN_EFF_CAP_KEY1_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_CAP_KEY2
	{
		{
			IDB_EFF_CMN_EFF_CAP_KEY2_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_CAP_OPEN
	{
		{
			IDB_EFF_CMN_EFF_CAP_OPEN_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	6.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_COUNT0
	{
		{
			IDB_EFF_CMN_EFF_COUNT0_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	15.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_COUNT1
	{
		{
			IDB_EFF_CMN_EFF_COUNT1_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	15.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_COUNT2
	{
		{
			IDB_EFF_CMN_EFF_COUNT2_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	15.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_COUNT3
	{
		{
			IDB_EFF_CMN_EFF_COUNT3_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	15.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_COUNT4
	{
		{
			IDB_EFF_CMN_EFF_COUNT4_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	15.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_COUNT5
	{
		{
			IDB_EFF_CMN_EFF_COUNT5_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	15.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_DASH
	{
		{
			IDB_EFF_CMN_EFF_DASH_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_DEATH
	{
		{
			IDB_EFF_CMN_EFF_DEATH_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	8.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_GOAL
	{
		{
			IDB_EFF_CMN_EFF_GOAL_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	7.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_H_ATTACK_00
	{
		{
			IDB_EFF_CMN_EFF_H_ATTACK_00_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
			3.2f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			IDB_EFF_CMN_EFF_HATTACK_01_ZNO,
		},
		IDB_EFF_CMN_EFF_CMN_TEX_MD_HA_AMB,
	},
	// GME_EFCT_CMN_IDX_H_ATTACK_01
	{
		{
			IDB_EFF_CMN_EFF_H_ATTACK_01_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			3.2f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_H_ATTACK_02
	{
		{
			IDB_EFF_CMN_EFF_H_ATTACK_02_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
			1.0f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_H_ATTACK_03
	{
		{
			IDB_EFF_CMN_EFF_H_ATTACK_03_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
			1.0f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_HIT_E
	{
		{
			IDB_EFF_CMN_EFF_HIT_E_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_HIT_S
	{
		{
			IDB_EFF_CMN_EFF_HIT_S_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_ITEM
	{
		{
			IDB_EFF_CMN_EFF_ITEM_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_ITEM_01
	{
		{
			IDB_EFF_CMN_EFF_ITEM_01_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	6.0f,	2.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_JUMP
	{
		{
			IDB_EFF_CMN_EFF_JUMP_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_MUTEKI
	{
		{
			IDB_EFF_CMN_EFF_MUTEKI_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	0.f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_MUTEKI2
	{
		{
			IDB_EFF_CMN_EFF_MUTEKI2_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_PILLAR_F_01
	{
		{
			IDB_EFF_CMN_EFF_PILLAR_F_01_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_PILLAR_F_02
	{
		{
			IDB_EFF_CMN_EFF_PILLAR_F_02_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_PILLAR_F_03
	{
		{
			IDB_EFF_CMN_EFF_PILLAR_F_03_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_PILLAR_F_HIT
	{
		{
			IDB_EFF_CMN_EFF_PILLAR_F_HIT_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_PISTON
	{
		{
			IDB_EFF_CMN_EFF_PISTON_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_POINT
	{
		{
			IDB_EFF_CMN_EFF_POINT_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	17.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_RING
	{
		{
			IDB_EFF_CMN_EFF_RING_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_ROLLDASH
	{
		{
			IDB_EFF_CMN_EFF_ROLLDASH_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-3.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_ROLLDASH_L
	{
		{
			IDB_EFF_CMN_EFF_ROLLDASH_L_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_ENABLE_DIR | GMD_EFFECT_3DES_FLAG_COPY_NODISP),	//init_flag
			{.0f,	-5.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			3.2f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			IDB_EFF_CMN_EFF_SFOOT_L_ZNO,
		},
		IDB_EFF_CMN_EFF_CMN_TEX_MD_RD_AMB,
	},
	// GME_EFCT_CMN_IDX_ROLLDASH_R
	{
		{
			IDB_EFF_CMN_EFF_ROLLDASH_R_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_ENABLE_DIR | GMD_EFFECT_3DES_FLAG_COPY_NODISP),	//init_flag
			{.0f,	-5.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			3.2f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			IDB_EFF_CMN_EFF_SFOOT_R_ZNO,
		},
		IDB_EFF_CMN_EFF_CMN_TEX_MD_RD_AMB,
	},
	// GME_EFCT_CMN_IDX_ROLLDASH_S
	{
		{
			IDB_EFF_CMN_EFF_ROLLDASH_S_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-3.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_RUN
	{
		{
			IDB_EFF_CMN_EFF_RUN_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	
	// GME_EFCT_CMN_IDX_SCORE_0
	{
		{
			IDB_EFF_CMN_EFF_SCORE_0_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SCORE_1
	{
		{
			IDB_EFF_CMN_EFF_SCORE_1_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SCORE_2
	{
		{
			IDB_EFF_CMN_EFF_SCORE_2_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SCORE_3
	{
		{
			IDB_EFF_CMN_EFF_SCORE_3_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SCORE_4
	{
		{
			IDB_EFF_CMN_EFF_SCORE_4_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SCORE_5
	{
		{
			IDB_EFF_CMN_EFF_SCORE_5_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SCORE_6
	{
		{
			IDB_EFF_CMN_EFF_SCORE_6_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SCORE_7
	{
		{
			IDB_EFF_CMN_EFF_SCORE_7_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SCORE_8
	{
		{
			IDB_EFF_CMN_EFF_SCORE_8_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SCORE_9
	{
		{
			IDB_EFF_CMN_EFF_SCORE_9_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SMORK_B_Z2
	{
		{
			IDB_EFF_CMN_EFF_SMORK_B_Z2_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SMORK_B_Z4
	{
		{
			IDB_EFF_CMN_EFF_SMORK_B_Z4_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SMORK_S_Z2
	{
		{
			IDB_EFF_CMN_EFF_SMORK_S_Z2_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SMORK_S_Z4
	{
		{
			IDB_EFF_CMN_EFF_SMORK_S_Z4_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SPIN
	{
		{
			IDB_EFF_CMN_EFF_SPIN_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SPIN_00
	{
		{
			IDB_EFF_CMN_EFF_SPIN_00_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SPIN_01
	{
		{
			IDB_EFF_CMN_EFF_SPIN_01_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-2.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SPIN_D
	{
		{
			IDB_EFF_CMN_EFF_SPIN_D_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SPIN_D_B
	{
		{
			IDB_EFF_CMN_EFF_SPIN_D_B_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SPIN_START
	{
		{
			IDB_EFF_CMN_EFF_SPIN_START_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SPRAY
	{
		{
			IDB_EFF_CMN_EFF_SPRAY_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SPRING
	{
		{
			IDB_EFF_CMN_EFF_SPRING_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SPRING_00
	{
		{
			IDB_EFF_CMN_EFF_SPRING_00_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			3.2f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			IDB_EFF_CMN_EFF_SPRING1_ZNO,
		},
		IDB_EFF_CMN_EFF_CMN_TEX_MD_SP_AMB,
	},
	// GME_EFCT_CMN_IDX_SPRING_01
	{
		{
			IDB_EFF_CMN_EFF_SPRING_01_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SS_END
	{
		{
			IDB_EFF_CMN_EFF_SS_END_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SS_SPIN
	{
		{
			IDB_EFF_CMN_EFF_SS_SPIN_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SS_SPIN_D
	{
		{
			IDB_EFF_CMN_EFF_SS_SPIN_D_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SS_SPIN_D_B
	{
		{
			IDB_EFF_CMN_EFF_SS_SPIN_D_B_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SS_SPIN_START
	{
		{
			IDB_EFF_CMN_EFF_SS_SPIN_START_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SS_START
	{
		{
			IDB_EFF_CMN_EFF_SS_START_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_STEAM
	{
		{
			IDB_EFF_CMN_EFF_STEAM_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_STEAM_L
	{
		{
			IDB_EFF_CMN_EFF_STEAM_L_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_STEAM_M
	{
		{
			IDB_EFF_CMN_EFF_STEAM_M_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_STEAM_S
	{
		{
			IDB_EFF_CMN_EFF_STEAM_S_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_STEAM_SET
	{
		{
			IDB_EFF_CMN_EFF_STEAM_SET_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_STEAM_SET_SS
	{
		{
			IDB_EFF_CMN_EFF_STEAM_SET_SS_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_STEAM_SHOT
	{
		{
			IDB_EFF_CMN_EFF_STEAM_SHOT_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_SWEAT
	{
		{
			IDB_EFF_CMN_EFF_SWEAT_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_TARGET_E
	{
		{
			IDB_EFF_CMN_EFF_TARGET_E_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_TARGET_S
	{
		{
			IDB_EFF_CMN_EFF_TARGET_S_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_CMN_IDX_TOGEBALL
	{
		{
			IDB_EFF_CMN_EFF_TOGEBALL_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
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

