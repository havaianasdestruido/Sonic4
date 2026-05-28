// =======================================================================
/*!
  @file	gmEffectBoss.h
  @brief ボスエフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmEffectBoss.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_EFFECT_BOSS_H_
#define GM_EFFECT_BOSS_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ボス共通エフェクトインデックス
typedef enum
{
	GME_EFCT_BOSS_CMN_IDX_BOSS_DM	= 0,
	GME_EFCT_BOSS_CMN_IDX_BOSS_PARTS,
	GME_EFCT_BOSS_CMN_IDX_BOSS_SMOKE02,
	GME_EFCT_BOSS_CMN_IDX_BOSS_SMORK,
	GME_EFCT_BOSS_CMN_IDX_JET_B,
	GME_EFCT_BOSS_CMN_IDX_JET_B_SMORK,
	
	GME_EFCT_BOSS_CMN_IDX_MAX,
	GME_EFCT_BOSS_CMN_IDX_NONE	= -1
} GME_EFCT_BOSS_CMN_IDX;

/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GmEfctBosssCmnBuildDataInit
/*!
  ボス共通エフェクトデータ構築 開始
 */
// =======================================================================
extern void GmEfctBossCmnBuildDataInit(void);

// =======================================================================
// GmEfctBossCmnBuildDataLoop
/*!
  ボス共通エフェクトデータ構築 ループ
  
  @retval TRUE	構築完了
  @retval FALSE	構築中
 */
// =======================================================================
extern BOOL GmEfctBossCmnBuildDataLoop(void);

// =======================================================================
// GmEfctBossCmnFlushDataInit
/*!
  ボス共通エフェクトデータ後片付け 開始
 */
// =======================================================================
extern void GmEfctBossCmnFlushDataInit(void);

// =======================================================================
// GmEfctBossCmnFlushDataLoop
/*!
  ボス共通エフェクトデータ後片付け ループ
  
  @retval TRUE	後片付け完了
  @retval FALSE	後片付け中
 */
// =======================================================================
extern BOOL GmEfctBossCmnFlushDataLoop(void);

// =======================================================================
// GmEfctBossBuildSingleDataInit
/*!
  ボスエフェクトデータ単体構築 初期化
 */
// =======================================================================
extern void GmEfctBossBuildSingleDataInit(void);

// =======================================================================
// GmEfctBossBuildSingleDataReg
/*!
  ボスエフェクトデータ単体構築 開始登録
  
  @param tex_index		[in]	テクスチャAMBのAMBインデックス
  @param ambtex_dwork	[io]	テクスチャAMB格納先データワーク
  @param texlist_dwork	[io]	テクスチャリスト格納先データワーク
  @param model_index	[in]	モデルデータのAMBインデックス
  @param model_dwork	[io]	モデルデータ格納先データワーク(NULL可)
  @param object_dwork	[io]	NNオブジェクト格納先データワーク(NULL可)
  @param arc			[io]	テクスチャAMB,モデルデータが格納されているAMB
 */
// =======================================================================
extern void GmEfctBossBuildSingleDataReg(Sint32 tex_index,
										  OBS_DATA_WORK *ambtex_dwork,
										  OBS_DATA_WORK *texlist_dwork,
										  Sint32 model_index,
										  OBS_DATA_WORK *model_dwork,
										  OBS_DATA_WORK *object_dwork,
										  void *arc);

// =======================================================================
// GmEfctBossBuildSingleDataReg
/*!
  ボスエフェクトデータ単体構築 ループ
  
  @retval TRUE	構築完了
  @retval FALSE	構築中
 */
// =======================================================================
extern BOOL GmEfctBossBuildSingleDataLoop(void);

// =======================================================================
// GmEfctBossFlushSingleDataInit
/*!
  ボスエフェクト単体構築データ 解放初期化・開始
 */
// =======================================================================
extern void GmEfctBossFlushSingleDataInit(void);

// =======================================================================
// GmEfctBossFlushSingleDataLoop
/*!
  ボスエフェクト単体構築データ 解放 ループ
  
  @retval TRUE	解放済み
  @retval FALSE	解放中
 */
// =======================================================================
extern BOOL GmEfctBossFlushSingleDataLoop(void);

// =======================================================================
// GmEfctBossCmnEsCreate
/*!
  ボス共通エフェクト（ESタイプ）生成
  
  @param parent_obj		[in]	親オブジェクト（NULL可）
  @param efct_bscmn_idx	[in]	ボス共通エフェクトインデックス
  
  @return エフェクト3DESワーク
 */
// =======================================================================
extern GMS_EFFECT_3DES_WORK* GmEfctBossCmnEsCreate(OBS_OBJECT_WORK *parent_obj, GME_EFCT_BOSS_CMN_IDX efct_bscmn_idx);

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

#endif /* GM_EFFECT_BOSS_H_ */
