// =======================================================================
/*!
  @file	gmEffectZone.h
  @brief ゾーン専用エフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmEffectZone.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_EFFECT_ZONE_H_
#define GM_EFFECT_ZONE_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ゾーン１専用エフェクトインデックス
typedef enum
{
	GME_EFCT_Z01_IDX_ASHIBA	= 0,
	GME_EFCT_Z01_IDX_ITEM_Z1,
	GME_EFCT_Z01_IDX_OBJECT_Z1,
	GME_EFCT_Z01_IDX_OBJECT_Z1_A3,
	GME_EFCT_Z01_IDX_SMORK_B_Z1,
	GME_EFCT_Z01_IDX_SMORK_S_Z1,
	GME_EFCT_Z01_IDX_SPARK_K,
	GME_EFCT_Z01_IDX_TAKI_Z1,
	GME_EFCT_Z01_IDX_WALL_Z1,
	
	GME_EFCT_Z01_IDX_MAX
} GME_EFCT_Z01_IDX;

//! ゾーン２専用エフェクトインデックス
typedef enum
{
	GME_EFCT_Z02_IDX_OBJECT_Z2	= 0,
	GME_EFCT_Z02_IDX_WALL_Z2,
	
	GME_EFCT_Z02_IDX_MAX
} GME_EFCT_Z02_IDX;

//! ゾーン３専用エフェクトインデックス
typedef enum
{
	GME_EFCT_Z03_IDX_BOMB_SMOKE_Z3	= 0,
	GME_EFCT_Z03_IDX_BUBBLE_BIG,
	GME_EFCT_Z03_IDX_BUBBLE_LOST,
	GME_EFCT_Z03_IDX_BUBBLE_LOST_2,
	GME_EFCT_Z03_IDX_BUBBLE_POINT,
	GME_EFCT_Z03_IDX_CANDLE_01,
	GME_EFCT_Z03_IDX_CANDLE_02,
	GME_EFCT_Z03_IDX_CANDLE_03,
	GME_EFCT_Z03_IDX_DASH_Z3,
	GME_EFCT_Z03_IDX_ITEM_Z3,
	GME_EFCT_Z03_IDX_LIGHT_01,
	GME_EFCT_Z03_IDX_OBJECT_W_Z3,
	GME_EFCT_Z03_IDX_OBJECT_Z3,
	GME_EFCT_Z03_IDX_PILLAR_01,
	GME_EFCT_Z03_IDX_PILLAR_02,
	GME_EFCT_Z03_IDX_PILLAR_03,
	GME_EFCT_Z03_IDX_PILLAR_HIT,
	GME_EFCT_Z03_IDX_ROCK_01,
	GME_EFCT_Z03_IDX_ROCK_02,
	GME_EFCT_Z03_IDX_ROLLDASH_S_Z3,
	GME_EFCT_Z03_IDX_RUN_Z3,
	GME_EFCT_Z03_IDX_SLIDER,
	GME_EFCT_Z03_IDX_SLIDER_02,
	GME_EFCT_Z03_IDX_SLIDER_SONIC,
	GME_EFCT_Z03_IDX_SMORK_B_Z3,
	GME_EFCT_Z03_IDX_SMORK_S_Z3,
	GME_EFCT_Z03_IDX_SPARK,
	GME_EFCT_Z03_IDX_SPARK_S,
	GME_EFCT_Z03_IDX_SPIN_02,
	GME_EFCT_Z03_IDX_SPIN_03,
	GME_EFCT_Z03_IDX_SPRAY_L_Z3,
	GME_EFCT_Z03_IDX_SPRAY_S_Z3,
	GME_EFCT_Z03_IDX_WALL_M_Z3,
	GME_EFCT_Z03_IDX_WALL_Z3,
	GME_EFCT_Z03_IDX_WATER,
	GME_EFCT_Z03_IDX_WATER_02,
	
	GME_EFCT_Z03_IDX_MAX
} GME_EFCT_Z03_IDX;

//! ゾーン４専用エフェクトインデックス
typedef enum
{
	GME_EFCT_Z04_IDX_LIGHT_Z4	= 0,
	GME_EFCT_Z04_IDX_PILLAR_Z4,
	GME_EFCT_Z04_IDX_WALL_M_Z4,
	GME_EFCT_Z04_IDX_WALL_Z4,
	
	GME_EFCT_Z04_IDX_MAX
} GME_EFCT_Z04_IDX;

//! ゾーンFINAL専用エフェクトインデックス
typedef enum
{
	GME_EFCT_ZFINAL_IDX_LIGHT_Z5	= 0,
	
	GME_EFCT_ZFINAL_IDX_MAX
} GME_EFCT_ZFINAL_IDX;

//! ゾーンSS（スペステ）専用エフェクトインデックス
typedef enum
{
	GME_EFCT_ZSS_IDX_1UP	= 0,
	GME_EFCT_ZSS_IDX_CHAOS_E_1,
	GME_EFCT_ZSS_IDX_CHAOS_E_2,
	GME_EFCT_ZSS_IDX_CHAOS_E_3,
	GME_EFCT_ZSS_IDX_CHAOS_E_4,
	GME_EFCT_ZSS_IDX_CHAOS_E_5,
	GME_EFCT_ZSS_IDX_CHAOS_E_6,
	GME_EFCT_ZSS_IDX_CHAOS_E_7,
	GME_EFCT_ZSS_IDX_LOST,
	GME_EFCT_ZSS_IDX_RING_GATE,
	GME_EFCT_ZSS_IDX_TIME_DE,
	GME_EFCT_ZSS_IDX_TIME_DOWN,
	GME_EFCT_ZSS_IDX_TIME_EN,
	GME_EFCT_ZSS_IDX_TIME_ES,
	GME_EFCT_ZSS_IDX_TIME_FR,
	GME_EFCT_ZSS_IDX_TIME_IT,
	GME_EFCT_ZSS_IDX_TIME_JP,
	GME_EFCT_ZSS_IDX_TIME_UP,
	
	GME_EFCT_ZSS_IDX_MAX
} GME_EFCT_ZSS_IDX;

/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GmEfctZoneBuildDataInit
/*!
  ゾーン専用エフェクトデータ構築 開始
 */
// =======================================================================
extern void GmEfctZoneBuildDataInit(GSE_MAIN_ZONE_TYPE zone_no);

// =======================================================================
// GmEfctZoneBuildDataLoopInit
/*!
  ゾーン専用エフェクトデータ構築 開始
  
  @note
  実際にロードコマンドを発行します。
 */
// =======================================================================
extern void GmEfctZoneBuildDataLoopInit(void);

// =======================================================================
// GmEfctZoneBuildDataLoop
/*!
  ゾーン専用エフェクトデータ構築 ループ
  
  @retval TRUE	構築完了
  @retval FALSE	構築中
 */
// =======================================================================
extern BOOL GmEfctZoneBuildDataLoop(void);

// =======================================================================
// GmEfctZoneFlushDataInit
/*!
  ゾーン専用エフェクトデータ後片付け 開始
 */
// =======================================================================
extern void GmEfctZoneFlushDataInit(GSE_MAIN_ZONE_TYPE zone_no);

// =======================================================================
// GmEfctZoneFlushDataLoopInit
/*!
  ゾーン専用エフェクトデータ後片付け 開始
  
  @note
  実際に解放コマンドを発行します。
 */
// =======================================================================
extern void GmEfctZoneFlushDataLoopInit(void);

// =======================================================================
// GmEfctZoneFlushDataLoop
/*!
  ゾーン専用エフェクトデータ後片付け ループ
  
  @retval TRUE	後片付け完了
  @retval FALSE	後片付け中
 */
// =======================================================================
extern BOOL GmEfctZoneFlushDataLoop(void);

// =======================================================================
// GmEfctZoneEsCreate
/*!
  ゾーン専用エフェクト（ESタイプ）生成
  
  @param parent_obj		[io]	親オブジェクト（NULL可）
  @param zone_no		[in]	ゾーン番号
  @param efct_zone_idx	[in]	エフェクトインデックス(GME_EFCT_Z**_IDX_**)
  
  @return エフェクト3DESワーク
  
  @note
  ゾーン毎のインデックス範囲を超えたefct_zone_idxを指定するとアサートします。
 */
// =======================================================================
extern GMS_EFFECT_3DES_WORK* GmEfctZoneEsCreate(OBS_OBJECT_WORK *parent_obj,
												GSE_MAIN_ZONE_TYPE zone_no, Sint32 efct_zone_idx);

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

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* GM_EFFECT_ZONE_ */
