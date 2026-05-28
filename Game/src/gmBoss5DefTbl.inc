// =======================================================================
/*!
  @file	gmBoss5DefTbl.inc
  @brief ボスファイナル テーブル

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

//! 汎用振動テーブル
const static fx32 gm_boss5_vib_tbl[GMD_BOSS5_VIB_PHASE_NUM][MTD_XY]	= {
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * -1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * -1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * -1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * -1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * -1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * -1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
};

//! 振動パラメータテーブル
const static fx32 gm_boss5_vib_param_tbl[GME_BOSS5_VIB_IDX_MAX][MTD_XYZ]	= {
	// GME_BOSS5_VIB_IDX_WALK_STEP
	{	(fx32)(FX32_ONE * 0),	(fx32)(FX32_ONE * 4), (fx32)(FX32_ONE * 0),	},
	// GME_BOSS5_VIB_IDX_RUN_STEP
	{	(fx32)(FX32_ONE * 0),	(fx32)(FX32_ONE * 4), (fx32)(FX32_ONE * 0),	},
	// GME_BOSS5_VIB_IDX_BERSERK_STAMP
	{	(fx32)(FX32_ONE * 0),	(fx32)(FX32_ONE * 8), (fx32)(FX32_ONE * 0),	},
	// GME_BOSS5_VIB_IDX_STOMP_LANDING
	{	(fx32)(FX32_ONE * 0),	(fx32)(FX32_ONE * 8), (fx32)(FX32_ONE * 0),	},
	// GME_BOSS5_VIB_IDX_CRASH_LANDING
	{	(fx32)(FX32_ONE * 0),	(fx32)(FX32_ONE * 8), (fx32)(FX32_ONE * 0),	},
	// GME_BOSS5_VIB_IDX_CRASH_STRIKE
	{	(fx32)(FX32_ONE * 0),	(fx32)(FX32_ONE * 16), (fx32)(FX32_ONE * 0),	},
};

//! ボス全体アクションIDテーブル
const static GMS_BOSS5_PART_ACT_INFO gm_boss5_act_id_tbl[GME_BOSS5_ACT_ID_MAX][GME_BOSS5_PART_IDX_MAX]	= {
	// ACT_ID											IS_MAINTAIN		IS_REPEAT	MTN_SPD	IS_BLEND	BLEND_SPD
	// GME_BOSS5_ACT_ID_START
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT01_01B_ZNM,		FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_FW
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT01_01B_ZNM,		FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_NML
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT02_01B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_NML_ABORT
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT01_01B_ZNM,		FALSE,			TRUE,		1.f,	TRUE,		0.05f,							FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_FWD_PREP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT02_01B_ZNM,		FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_FWD_R_JUMP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT02_02B_ZNM,		FALSE,			FALSE,		6.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_FWD_R_AIR
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT02_03B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_FWD_R_LAND
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT02_04B_ZNM,		FALSE,			FALSE,		4.0f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_FWD_L_JUMP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT02_05B_ZNM,		FALSE,			FALSE,		6.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_FWD_L_AIR
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT02_06B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_FWD_L_LAND
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT02_07B_ZNM,		FALSE,			FALSE,		4.0f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_BWD_PREP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT03_01B_ZNM,		FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_BWD_R_JUMP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT03_02B_ZNM,		FALSE,			FALSE,		6.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_BWD_R_AIR
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT03_03B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_BWD_R_LAND
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT03_04B_ZNM,		FALSE,			FALSE,		4.f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_BWD_L_JUMP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT03_05B_ZNM,		FALSE,			FALSE,		6.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_BWD_L_AIR
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT03_06B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_BWD_L_LAND
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT03_07B_ZNM,		FALSE,			FALSE,		4.f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_FWD_CRASH
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT04_01B_ZNM,		FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_BWD_CRASH
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT05_01B_ZNM,		FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_MOVE_FAST_RECOVER
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT01_01B_ZNM,		FALSE,			TRUE,		1.f,	TRUE,		0.0625f,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_STOMP_IGNITE
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT06_01B_ZNM,		FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_STOMP_HOVER
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT06_02B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_STOMP_FLYUP
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT06_03B_ZNM,		FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_STOMP_FALL
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT07_01B_ZNM,		FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_STOMP_LAND
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT08_01B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_CRASH_PREP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT06_01B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_CRASH_HOVER
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT07_01B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_CRASH_FLYUP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT08_01B_ZNM,		FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_CRASH_FALL
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT09_01B_ZNM,		FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_CRASH_LAND
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT09_02B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_CRASH_SINK
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT09_03B_ZNM,		FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_RPUNCH_PREP
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT09_01B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_RPUNCH_LAUNCH_FIRST
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT09_02B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_RPUNCH_LAUNCH_SECOND
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT09_03B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_RPUNCH_RETURN
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT09_04B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_RPC_STR_DMG_START
	{
		{IDB_BOSS05_BODY_MTN_B05_DMG01_01B_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_RPC_STR_DMG_LOOP
	{
		{IDB_BOSS05_BODY_MTN_B05_DMG01_02B_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_RPC_STR_DMG_RECOVER
	{
		{IDB_BOSS05_BODY_MTN_B05_DMG01_03B_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_BERSERK_BREAKDOWN
	{
		{IDB_BOSS05_BODY_MTN_B05_1_ATT01_01B_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_BERSERK_SHAKE
	{
		{IDB_BOSS05_BODY_MTN_B05_2_ATT01_01B_ZNM,		FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_BERSERK_ROAR_PREP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_STA_02B_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_BERSERK_ROAR_START
	{
		{IDB_BOSS05_BODY_MTN_B05_2_STA_03B_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_BERSERK_ROAR_LOOP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_STA_04B_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_BERSERK_STAMP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_STA_05B_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
	// GME_BOSS5_ACT_ID_BERSERK_KICKUP
	{
		{IDB_BOSS05_BODY_MTN_B05_2_STA_06B_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS5_DEFAULT_BLEND_SPD,	FALSE},		// BODY
	},
};


//! 本体 腕アニメーション情報テーブル
const static GMS_BOSS5_ARM_ANIM_INFO gm_boss5_arm_anim_info_tbl[GMD_BOSS5_ARM_POKE_ANIM_PHASE_NUM]	= {
	// phase 0	肘を引く
	{
		0,
		0.05f,
		{
			{	TRUE,	{	0,	AKM_DEGtoA32(0), 	0,	},	{	0,	AKM_DEGtoA32(30), 	0,	},	},
			{	TRUE,	{	0,	AKM_DEGtoA32(0), 	0,	},	{	0,	AKM_DEGtoA32(-35), 	0,	},	},
			{	TRUE,	{	0,	AKM_DEGtoA32(0), 	0,	},	{	0,	AKM_DEGtoA32(-40), 	0,	},	},
		},
	},
	// phase 1	肘を引いた状態で待機
	{
		10,
		0,
		{
			{	FALSE,	{	0,	AKM_DEGtoA32(30), 	0,	},	{	0,	AKM_DEGtoA32(30), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(-35), 	0,	},	{	0,	AKM_DEGtoA32(-35), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(-40), 	0,	},	{	0,	AKM_DEGtoA32(-40), 	0,	},	},
		},
	},
	// phase 2	前に突く
	{
		0,
		0.2f,
		{
			{	TRUE,	{	0,	AKM_DEGtoA32(30), 	0,	},	{	0,	AKM_DEGtoA32(-60), 	0,	},	},
			{	TRUE,	{	0,	AKM_DEGtoA32(-35), 	0,	},	{	0,	AKM_DEGtoA32(0), 	0,	},	},
			{	TRUE,	{	0,	AKM_DEGtoA32(-40), 	0,	},	{	0,	AKM_DEGtoA32(0), 	0,	},	},
		},
	},
	// phase 3	上に突き上げる
	{
		0,
		0.2f,
		{
			{	TRUE,	{	0,	AKM_DEGtoA32(-60), 	0,	},	{	AKM_DEGtoA32(-30),	AKM_DEGtoA32(-90), 	0,	},	},
			{	TRUE,	{	0,	AKM_DEGtoA32(0), 	0,	},	{	0,	AKM_DEGtoA32(10), 	0,	},	},
			{	TRUE,	{	0,	AKM_DEGtoA32(0), 	0,	},	{	0,	AKM_DEGtoA32(0), 	0,	},	},
		},
	},
	// phase 4	停止振動 前
	{
		0,
		0.5f,
		{
			{	FALSE,	{	AKM_DEGtoA32(-30),	AKM_DEGtoA32(-90), 	0,	},	{	AKM_DEGtoA32(-30),	AKM_DEGtoA32(-100), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(10), 	0,	},	{	0,	AKM_DEGtoA32(10), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(0), 	0,	},	{	0,	AKM_DEGtoA32(0), 	0,	},	},
		},
	},
	// phase 5	停止振動 後
	{
		0,
		0.5f,
		{
			{	FALSE,	{	AKM_DEGtoA32(-30),	AKM_DEGtoA32(-100), 	0,	},	{	AKM_DEGtoA32(-30),	AKM_DEGtoA32(-86), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(10), 	0,	},	{	0,	AKM_DEGtoA32(10), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(0), 	0,	},	{	0,	AKM_DEGtoA32(0), 	0,	},	},
		},
	},
	// phase 6	停止振動 戻る
	{
		0,
		0.5f,
		{
			{	FALSE,	{	AKM_DEGtoA32(-30),	AKM_DEGtoA32(-86), 	0,	},	{	AKM_DEGtoA32(-30),	AKM_DEGtoA32(-90), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(10), 	0,	},	{	0,	AKM_DEGtoA32(10), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(0), 	0,	},	{	0,	AKM_DEGtoA32(0), 	0,	},	},
		},
	},
	// phase 7	突き上げた状態で待機
	{
		60,
		0.f,
		{
			{	FALSE,	{	AKM_DEGtoA32(-30),	AKM_DEGtoA32(-90), 	0,	},	{	AKM_DEGtoA32(-30),	AKM_DEGtoA32(-90), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(10), 	0,	},	{	0,	AKM_DEGtoA32(10), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(0), 	0,	},	{	0,	AKM_DEGtoA32(0), 	0,	},	},
		},
	},
	// phase 8	元に戻す
	{
		0,
		0.05f,
		{
			{	TRUE,	{	AKM_DEGtoA32(-30),	AKM_DEGtoA32(-90), 	0,	},	{	0,	AKM_DEGtoA32(0), 	0,	},	},
			{	TRUE,	{	0,	AKM_DEGtoA32(10), 	0,	},	{	0,	AKM_DEGtoA32(0), 	0,	},	},
			{	FALSE,	{	0,	AKM_DEGtoA32(0), 	0,	},	{	0,	AKM_DEGtoA32(0), 	0,	},	},
		},
	},
};

/* サイズ定義 */

//! 本体矩形デフォルトサイズ
#define GMD_BOSS5_RECT_BODY_DEFAULT_SIZE	{-32, -32, 32, 32}
//! 本体矩形内側サイズ
// （複数の矩形が重なっている場合にデフォルトサイズに設定した矩形を優先させる場合の、非優先矩形のサイズ）
#define GMD_BOSS5_RECT_BODY_INSIDE_SIZE		{-16, -16, 16, 16}
//! 本体矩形ストンプ攻撃時サイズ
#define GMD_BOSS5_RECT_BODY_STOMP_SIZE		{-32, -32, 32, 48}	// REMINDER:デフォルトサイズの横幅と同じにすること
//! 本体矩形地球割り攻撃時サイズ
#define GMD_BOSS5_RECT_BODY_CRASH_SIZE		{-32, -32, 32, 48}
//! 脚矩形デフォルトサイズ
#define GMD_BOSS5_RECT_LEG_DEFAULT_SIZE		{-24, -24, 24, 24}

//! 本体 矩形設定情報テーブル
const static GMS_BOSS5_BODY_RECT_SETTING_INFO gm_boss5_body_rect_setting_info_tbl[GME_BOSS5_BODY_RECT_SETTING_MAX]	= {
	// GME_BOSS5_BODY_RECT_SETTING_DEFAULT_NML
	{
		FALSE,
		FALSE,
		{
			{	// left foot
				(GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	{0, 0, 0, 0},	GMD_BOSS5_RECT_LEG_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
			{	// right foot
				(GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	{0, 0, 0, 0},	GMD_BOSS5_RECT_LEG_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
			{	// body
				(GMD_BOSS5_RECT_ENABLE_FLAG_DEF),
				{	GMD_BOSS5_RECT_BODY_DEFAULT_SIZE,	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
		}
	},
	// GME_BOSS5_BODY_RECT_SETTING_DEFAULT_STR
	{
		TRUE,
		TRUE,
		{
			{	// left foot
				(GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	{0, 0, 0, 0},	GMD_BOSS5_RECT_LEG_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
			{	// right foot
				(GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	{0, 0, 0, 0},	GMD_BOSS5_RECT_LEG_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
			{	// body
				(GMD_BOSS5_RECT_ENABLE_FLAG_DEF | GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	GMD_BOSS5_RECT_BODY_INSIDE_SIZE,	GMD_BOSS5_RECT_BODY_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
		}
	},
	// GME_BOSS5_BODY_RECT_SETTING_START
	{
		FALSE,
		FALSE,
		{
			{	// left foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// right foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// body
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
		},
	},
	// GME_BOSS5_BODY_RECT_SETTING_STOMP_NML_FALL
	{
		FALSE,
		FALSE,
		{
			{	// left foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// right foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// body
				(GMD_BOSS5_RECT_ENABLE_FLAG_DEF | GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	GMD_BOSS5_RECT_BODY_INSIDE_SIZE,	GMD_BOSS5_RECT_BODY_STOMP_SIZE,	{0, 0, 0, 0},	},
			},
		}
	},
	// GME_BOSS5_BODY_RECT_SETTING_STOMP_STR_FALL
	{
		TRUE,
		TRUE,
		{
			{	// left foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// right foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// body
				(GMD_BOSS5_RECT_ENABLE_FLAG_DEF | GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	GMD_BOSS5_RECT_BODY_INSIDE_SIZE,	GMD_BOSS5_RECT_BODY_STOMP_SIZE,	{0, 0, 0, 0},	},
			},
		}
	},
	// GME_BOSS5_BODY_RECT_SETTING_CRASH_FALL
	{
		TRUE,
		TRUE,
		{
			{	// left foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// right foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// body
				(GMD_BOSS5_RECT_ENABLE_FLAG_DEF | GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	GMD_BOSS5_RECT_BODY_INSIDE_SIZE,	GMD_BOSS5_RECT_BODY_CRASH_SIZE,	{0, 0, 0, 0},	},
			},
		}
	},
	// GME_BOSS5_BODY_RECT_SETTING_CRASH_GROUND
	{
		FALSE,
		FALSE,
		{
			{	// left foot
				(GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	{0, 0, 0, 0},	GMD_BOSS5_RECT_LEG_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
			{	// right foot
				(GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	{0, 0, 0, 0},	GMD_BOSS5_RECT_LEG_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
			{	// body
				(GMD_BOSS5_RECT_ENABLE_FLAG_DEF),
				{	GMD_BOSS5_RECT_BODY_DEFAULT_SIZE,	GMD_BOSS5_RECT_BODY_INSIDE_SIZE,	{0, 0, 0, 0},	},
			},
		}
	},
	// GME_BOSS5_BODY_RECT_SETTING_CRASH_SINK
	{
		FALSE,
		FALSE,
		{
			{	// left foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// right foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// body
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
		}
	},
	// GME_BOSS5_BODY_RECT_SETTING_RPC_STR_DMG
	{
		FALSE,
		FALSE,
		{
			{	// left foot
				(GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	{0, 0, 0, 0},	GMD_BOSS5_RECT_LEG_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
			{	// right foot
				(GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	{0, 0, 0, 0},	GMD_BOSS5_RECT_LEG_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
			{	// body
				(GMD_BOSS5_RECT_ENABLE_FLAG_DEF),
				{	GMD_BOSS5_RECT_BODY_DEFAULT_SIZE,	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
		}
	},
	// GME_BOSS5_BODY_RECT_SETTING_BERSERK_START
	{
		TRUE,
		FALSE,
		{
			{	// left foot
				(GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	{0, 0, 0, 0},	GMD_BOSS5_RECT_LEG_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
			{	// right foot
				(GMD_BOSS5_RECT_ENABLE_FLAG_ATK),
				{	{0, 0, 0, 0},	GMD_BOSS5_RECT_LEG_DEFAULT_SIZE,	{0, 0, 0, 0},	},
			},
			{	// body
				(GMD_BOSS5_RECT_ENABLE_FLAG_DEF),
				{	GMD_BOSS5_RECT_BODY_DEFAULT_SIZE,	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
		}
	},
	// GME_BOSS5_BODY_RECT_SETTING_DEFEAT
	{
		FALSE,
		FALSE,
		{
			{	// left foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// right foot
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
			{	// body
				(0),
				{	{0, 0, 0, 0},	{0, 0, 0, 0},	{0, 0, 0, 0},	},
			},
		}
	},
};

//############ 動作シーケンス #################################################

//! 戦略フロー分岐テーブル
const static GMS_BOSS5_STRAT_PROB_INFO gm_boss5_body_seq_strat_branch_prob_tbl[GME_BOSS5_BODY_STRAT_BRANCH_MAX][GMD_BOSS5_BODY_STRAT_BRANCH_NUM]	= {
	// GME_BOSS5_BODY_STRAT_BRANCH_NML_A
	{
		{	GME_BOSS5_STRAT_STATE_NML_MOVE_B,		((fx32)(FX32_ONE * 0.5f)),	FALSE	},
		{	GME_BOSS5_STRAT_STATE_NML_STOMP_B,		((fx32)(FX32_ONE * 0.2f)),	FALSE	},
		{	GME_BOSS5_STRAT_STATE_NML_RPUNCH,		((fx32)(FX32_ONE * 0.3f)),	TRUE	},
	},
	// GME_BOSS5_BODY_STRAT_BRANCH_NML_B
	{
		{	GME_BOSS5_STRAT_STATE_NML_MOVE_C,		((fx32)(FX32_ONE * 0.5f)),	FALSE	},
		{	GME_BOSS5_STRAT_STATE_NML_STOMP_A,		((fx32)(FX32_ONE * 0.5f)),	FALSE	},
		{	GME_BOSS5_STRAT_STATE_NONE,				((fx32)(FX32_ONE * 0)),		FALSE	},
	},
	// GME_BOSS5_BODY_STRAT_BRANCH_STR_A
	{
		{	GME_BOSS5_STRAT_STATE_STR_MOVE_B,		((fx32)(FX32_ONE * 0.1f)),	FALSE	},
		{	GME_BOSS5_STRAT_STATE_STR_STOMP_B,		((fx32)(FX32_ONE * 0.1f)),	FALSE	},
		{	GME_BOSS5_STRAT_STATE_STR_RPUNCH_A,		((fx32)(FX32_ONE * 0.8f)),	TRUE	},
	},
	// GME_BOSS5_BODY_STRAT_BRANCH_STR_B
	{
		{	GME_BOSS5_STRAT_STATE_STR_MOVE_C,		((fx32)(FX32_ONE * 0.5f)),	FALSE	},
		{	GME_BOSS5_STRAT_STATE_STR_STOMP_A,		((fx32)(FX32_ONE * 0.5f)),	FALSE	},
		{	GME_BOSS5_STRAT_STATE_NONE,				((fx32)(FX32_ONE * 0)),		FALSE	},
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
