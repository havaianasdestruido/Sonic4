// =======================================================================
/*!
  @file	gmEffectEnemyTbl.inc
  @brief エネミー専用エフェクト テーブル

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

//! エネミー専用エフェクト生成パラメータテーブル
const GMS_EFCT_ENE_CREATE_PARAM gm_efct_ene_create_param_tbl[GME_EFCT_ENE_IDX_MAX]	= {
	
	/* =================== E002 スティンガー =================== */
	
	// GME_EFCT_ENE_IDX_E02_JET_S
	{
		{
			IDB_EFF_E002_EFF_JET_S_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-8.0f},	//disp_ofst
			{AKM_DEGtoA16(-90),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E02_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E02,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E02_JET_S,	//ame_dwork_no
		IDB_EFF_E002_EFF_E002_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_1) ),
	},
	// GME_EFCT_ENE_IDX_E02_JET_S_SMORK
	{
		{
			IDB_EFF_E002_EFF_JET_S_SMORK_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	-13.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E02_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E02,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E02_JET_S_SMORK,	//ame_dwork_no
		IDB_EFF_E002_EFF_E002_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_1) ),
	},
	
	
	/* =================== E004 メレオン =================== */
	
	// GME_EFCT_ENE_IDX_E04_*****
	{
		{
			0,//仮
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//arc_dwork_no
		-1,	//ambtex_dwork_no
		-1,	//ame_dwork_no
		-1,	//ambtex_idx
		0,//( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_1) ),
	},
	// GME_EFCT_ENE_IDX_E04_*****
	{
		{
			0,//仮
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		-1,	//arc_dwork_no
		-1,	//ambtex_dwork_no
		-1,	//ame_dwork_no
		-1,	//ambtex_idx
		0,//( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_1) ),
	},
	// GME_EFCT_ENE_IDX_E04_MEREON_MISS
	{
		{
			IDB_EFF_E004_EFF_MEREON_MISS_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E04_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E04,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E04_MEREON_MISS,	//ame_dwork_no
		IDB_EFF_E004_EFF_E004_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_1) ),
	},
	
	
	/* =================== E005 ガードン =================== */
	
	// GME_EFCT_ENE_IDX_E05_GUARD
	{
		{
			IDB_EFF_E005_EFF_GUARD_AME,
			GME_EFFECT_3DES_POS_TYPE_MTX,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT| GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E05_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E05,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E05_GUARD,	//ame_dwork_no
		IDB_EFF_E005_EFF_E005_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_2) ),
	},
	
	
	/* =================== E006 ハロゲン =================== */
	
	// GME_EFCT_ENE_IDX_E06_HARO
	{
		{
			IDB_EFF_E006_EFF_HARO_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	-0.5f,	8.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E06_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E06,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E06_HARO,	//ame_dwork_no
		IDB_EFF_E006_EFF_E006_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_2) ),
	},
	
	/* =================== E007 モグリン =================== */
	
	// GME_EFCT_ENE_IDX_E07_MOGU_E
	{
		{
			IDB_EFF_E007_EFF_MOGU_E_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E07_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E07,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E07_MOGU_E,	//ame_dwork_no
		IDB_EFF_E007_EFF_E007_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_3) ),
	},
	// GME_EFCT_ENE_IDX_E07_MOGU_W
	{
		{
			IDB_EFF_E007_EFF_MOGU_W_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E07_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E07,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E07_MOGU_W,	//ame_dwork_no
		IDB_EFF_E007_EFF_E007_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_3) ),
	},
	
	
	/* =================== E010 ブクブク =================== */
	
	// GME_EFCT_ENE_IDX_E10_BUKUBUKU
	{
		{
			IDB_EFF_E010_EFF_BUKUBUKU_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(-45),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E10_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E10,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E10_BUKUBUKU,	//ame_dwork_no
		IDB_EFF_E010_EFF_E010_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_3) ),
	},
	
	
	/* =================== E013 テルスター =================== */
	
	// GME_EFCT_ENE_IDX_E13_CROW
	{
		{
			IDB_EFF_E013_EFF_CROW_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_NOFLIP | GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(180),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E13_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E13,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E13_CROW,	//ame_dwork_no
		IDB_EFF_E013_EFF_E013_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_4) ),
	},
	// GME_EFCT_ENE_IDX_E13_T_STAR
	{
		{
			IDB_EFF_E013_EFF_T_STAR_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT,
			(GMD_EFFECT_3DES_FLAG_NOFLIP),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{0,		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEnd,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E13_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E13,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E13_T_STAR,	//ame_dwork_no
		IDB_EFF_E013_EFF_E013_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_4) ),
	},
	
	
	/* =================== E014 ハリセンボ =================== */
	
	// GME_EFCT_ENE_IDX_E14_JET_H
	{
		{
			IDB_EFF_E014_EFF_JET_H_AME,
			GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
			(GMD_EFFECT_3DES_FLAG_STICKPARENT | GMD_EFFECT_3DES_FLAG_ENABLE_DIR),	//init_flag
			{.0f,	.0f,	.0f},	//disp_ofst
			{AKM_DEGtoA16(180),		0,		0},		//disp_rot
			1.f,//scale
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ,
			GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE,
		},
		GMD_DWORK_NO_EFFECT_ENE_E14_ARC,	//arc_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E14,	//ambtex_dwork_no
		GMD_DWORK_NO_EFFECT_ENE_AME_E14_JET_H,	//ame_dwork_no
		IDB_EFF_E014_EFF_E014_TEX_AMB,	//ambtex_idx
		( GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_1) |
		  GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_2) |
		  GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_3) |
		  GMM_EFCT_ENE_STAGE_FLAG(GSD_MAIN_ZONE_TYPE_4) ),
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

