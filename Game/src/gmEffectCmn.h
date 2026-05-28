// =======================================================================
/*!
  @file	gmEffectCmn.h
  @brief 共通エフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmEffectCmn.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_EFFECT_COMMON_H_
#define GM_EFFECT_COMMON_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! 共通エフェクトインデックス
typedef enum
{
	GME_EFCT_CMN_IDX_AURA_00	= 0,
	GME_EFCT_CMN_IDX_AURA_01,
	GME_EFCT_CMN_IDX_AURA_02,
	GME_EFCT_CMN_IDX_AURA_03,
	GME_EFCT_CMN_IDX_BARRIER,
	GME_EFCT_CMN_IDX_BARRIER_01,
	GME_EFCT_CMN_IDX_BARRIER_LOST,
	GME_EFCT_CMN_IDX_BOMB,
	GME_EFCT_CMN_IDX_BOMB_BIG,
	GME_EFCT_CMN_IDX_BOMB_KAMA,
	GME_EFCT_CMN_IDX_BOMB_SMOKE,
	GME_EFCT_CMN_IDX_BRAKE,
	GME_EFCT_CMN_IDX_BRAKE_S,
	GME_EFCT_CMN_IDX_BUBBLE,
	GME_EFCT_CMN_IDX_BULLET,
	GME_EFCT_CMN_IDX_BULLET_CORE,
	GME_EFCT_CMN_IDX_BUMPER,
	GME_EFCT_CMN_IDX_BUMPER_1,
	GME_EFCT_CMN_IDX_BUMPER_2,
	GME_EFCT_CMN_IDX_BUMPER_LOST,
	GME_EFCT_CMN_IDX_CANON,
	GME_EFCT_CMN_IDX_CAP_KEY1,
	GME_EFCT_CMN_IDX_CAP_KEY2,
	GME_EFCT_CMN_IDX_CAP_OPEN,
	GME_EFCT_CMN_IDX_COUNT0,
	GME_EFCT_CMN_IDX_COUNT1,
	GME_EFCT_CMN_IDX_COUNT2,
	GME_EFCT_CMN_IDX_COUNT3,
	GME_EFCT_CMN_IDX_COUNT4,
	GME_EFCT_CMN_IDX_COUNT5,
	GME_EFCT_CMN_IDX_DASH,
	GME_EFCT_CMN_IDX_DEATH,
	GME_EFCT_CMN_IDX_GOAL,
	GME_EFCT_CMN_IDX_H_ATTACK_00,
	GME_EFCT_CMN_IDX_H_ATTACK_01,
	GME_EFCT_CMN_IDX_H_ATTACK_02,
	GME_EFCT_CMN_IDX_H_ATTACK_03,
	GME_EFCT_CMN_IDX_HIT_E,
	GME_EFCT_CMN_IDX_HIT_S,
	GME_EFCT_CMN_IDX_ITEM,
	GME_EFCT_CMN_IDX_ITEM_01,
	GME_EFCT_CMN_IDX_JUMP,
	GME_EFCT_CMN_IDX_MUTEKI,
	GME_EFCT_CMN_IDX_MUTEKI2,
	GME_EFCT_CMN_IDX_PILLAR_F_01,
	GME_EFCT_CMN_IDX_PILLAR_F_02,
	GME_EFCT_CMN_IDX_PILLAR_F_03,
	GME_EFCT_CMN_IDX_PILLAR_F_HIT,
	GME_EFCT_CMN_IDX_PISTON,
	GME_EFCT_CMN_IDX_POINT,
	GME_EFCT_CMN_IDX_RING,
	GME_EFCT_CMN_IDX_ROLLDASH,
	GME_EFCT_CMN_IDX_ROLLDASH_L,
	GME_EFCT_CMN_IDX_ROLLDASH_R,
	GME_EFCT_CMN_IDX_ROLLDASH_S,
	GME_EFCT_CMN_IDX_RUN,
	GME_EFCT_CMN_IDX_SCORE_0,
	GME_EFCT_CMN_IDX_SCORE_1,
	GME_EFCT_CMN_IDX_SCORE_2,
	GME_EFCT_CMN_IDX_SCORE_3,
	GME_EFCT_CMN_IDX_SCORE_4,
	GME_EFCT_CMN_IDX_SCORE_5,
	GME_EFCT_CMN_IDX_SCORE_6,
	GME_EFCT_CMN_IDX_SCORE_7,
	GME_EFCT_CMN_IDX_SCORE_8,
	GME_EFCT_CMN_IDX_SCORE_9,
	GME_EFCT_CMN_IDX_SMORK_B_Z2,
	GME_EFCT_CMN_IDX_SMORK_B_Z4,
	GME_EFCT_CMN_IDX_SMORK_S_Z2,
	GME_EFCT_CMN_IDX_SMORK_S_Z4,
	GME_EFCT_CMN_IDX_SPIN,
	GME_EFCT_CMN_IDX_SPIN_00,
	GME_EFCT_CMN_IDX_SPIN_01,
	GME_EFCT_CMN_IDX_SPIN_D,
	GME_EFCT_CMN_IDX_SPIN_D_B,
	GME_EFCT_CMN_IDX_SPIN_START,
	GME_EFCT_CMN_IDX_SPRAY,
	GME_EFCT_CMN_IDX_SPRING,
	GME_EFCT_CMN_IDX_SPRING_00,
	GME_EFCT_CMN_IDX_SPRING_01,
	GME_EFCT_CMN_IDX_SS_END,
	GME_EFCT_CMN_IDX_SS_SPIN,
	GME_EFCT_CMN_IDX_SS_SPIN_D,
	GME_EFCT_CMN_IDX_SS_SPIN_D_B,
	GME_EFCT_CMN_IDX_SS_SPIN_START,
	GME_EFCT_CMN_IDX_SS_START,
	GME_EFCT_CMN_IDX_STEAM,
	GME_EFCT_CMN_IDX_STEAM_L,
	GME_EFCT_CMN_IDX_STEAM_M,
	GME_EFCT_CMN_IDX_STEAM_S,
	GME_EFCT_CMN_IDX_STEAM_SET,
	GME_EFCT_CMN_IDX_STEAM_SET_SS,
	GME_EFCT_CMN_IDX_STEAM_SHOT,
	GME_EFCT_CMN_IDX_SWEAT,
	GME_EFCT_CMN_IDX_TARGET_E,
	GME_EFCT_CMN_IDX_TARGET_S,
	GME_EFCT_CMN_IDX_TOGEBALL,
	
	GME_EFCT_CMN_IDX_MAX,
	GME_EFCT_CMN_IDX_NONE	= -1
} GME_EFCT_CMN_IDX;

/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GmEfctCmnBuildDataInit
/*!
  共通エフェクトデータ構築 初期化
 */
// =======================================================================
extern void GmEfctCmnBuildDataInit(void);

// =======================================================================
// GmEfctCmnBuildDataLoopInit
/*!
  共通エフェクトデータ構築 開始
  
  @note
  実際にロードコマンドを発行します。
 */
// =======================================================================
extern void GmEfctCmnBuildDataLoopInit(void);

// =======================================================================
// GmEfctCmnBuildDataLoop
/*!
  共通エフェクトデータ構築 ループ
  
  @retval TRUE	構築完了
  @retval FALSE	構築中
 */
// =======================================================================
extern BOOL GmEfctCmnBuildDataLoop(void);

// =======================================================================
// GmEfctCmnFlushDataInit
/*!
  共通エフェクトデータ後片付け 初期化
 */
// =======================================================================
extern void GmEfctCmnFlushDataInit(void);

// =======================================================================
// GmEfctCmnFlushDataLoopInit
/*!
  共通エフェクトデータ後片付け 開始
  
  @note
  実際に解放コマンドを発行します。
 */
// =======================================================================
extern void GmEfctCmnFlushDataLoopInit(void);

// =======================================================================
// GmEfctCmnFlushDataLoop
/*!
  共通エフェクトデータ後片付け ループ
  
  @retval TRUE	後片付け完了
  @retval FALSE	後片付け中
 */
// =======================================================================
extern BOOL GmEfctCmnFlushDataLoop(void);

// =======================================================================
// GmEfctCmnEsCreate
/*!
  共通エフェクト（ESタイプ）生成
  
  @param parent_obj		[in]	親オブジェクト（NULL可）
  @param efct_cmn_idx	[in]	共通エフェクトインデックス
  
  @return エフェクト3DESワーク
 */
// =======================================================================
extern GMS_EFFECT_3DES_WORK* GmEfctCmnEsCreate(OBS_OBJECT_WORK *parent_obj, GME_EFCT_CMN_IDX efct_cmn_idx);

// =======================================================================
// GmEfctCmnUpdateInvincibleMainPart
/*!
  無敵エフェクト メインパーツ 更新
  
  @param efct_3des	[io]	3DESエフェクトワーク
  
  @note
  無敵エフェクトの、プレイヤー本体側パーツの更新処理を行います。
  終了チェック等は行いません。毎フレーム呼び出してください。
 */
// =======================================================================
extern void GmEfctCmnUpdateInvincibleMainPart(GMS_EFFECT_3DES_WORK *efct_3des);

// =======================================================================
// GmEfctCmnUpdateInvincibleSubPart
/*!
  無敵エフェクト サブパーツ 更新
  
  @param efct_3des	[io]	3DESエフェクトワーク
  @param ply_obj	[in]	プレイヤーオブジェクト
  
  @note
  無敵エフェクトの、プレイヤーに遅れて付いてくるパーツの更新処理を行います。
  終了チェック等は行いません。毎フレーム呼び出してください。
 */
// =======================================================================
extern void GmEfctCmnUpdateInvincibleSubPart(GMS_EFFECT_3DES_WORK *efct_3des,
											 const OBS_OBJECT_WORK *ply_obj);

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

#endif /* GM_EFFECT_COMMON_ */
