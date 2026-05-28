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
const GMS_EFCT_BOSS_CMN_CREATE_PARAM gm_efct_boss_cmn_create_param_tbl[GME_EFCT_BOSS_CMN_IDX_MAX]	= {
	// GME_EFCT_BOSS_CMN_IDX_BOSS_DM
	{
		{
			IDB_EFF_BS_CMN_EFF_BOSS_DM_AME,
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
	// GME_EFCT_BOSS_CMN_IDX_BOSS_PARTS
	{
		{
			IDB_EFF_BS_CMN_EFF_BOSS_PARTS_AME,
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
	// GME_EFCT_BOSS_CMN_IDX_BOSS_SMOKE02
	{
		{
			IDB_EFF_BS_CMN_EFF_BOSS_SMOKE02_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_BOSS_CMN_IDX_BOSS_SMORK
	{
		{
			IDB_EFF_BS_CMN_EFF_BOSS_SMORK_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-8.0f},	//disp_ofst
			{AKM_DEGtoA16(-60),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_BOSS_CMN_IDX_JET_B
	{
		{
			IDB_EFF_BS_CMN_EFF_JET_B_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR ),	//init_flag
			{.0f,	.0f,	-14.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,
	},
	// GME_EFCT_BOSS_CMN_IDX_JET_B_SMORK
	{
		{
			IDB_EFF_BS_CMN_EFF_JET_B_SMORK_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-17.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
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

