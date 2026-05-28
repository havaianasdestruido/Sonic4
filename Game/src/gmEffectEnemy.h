// =======================================================================
/*!
  @file	gmEffectEnemy.h
  @brief エネミー専用エフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmEffectEnemy.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_EFFECT_ENEMY_H_
#define GM_EFFECT_ENEMY_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! エネミー専用エフェクトインデックス
typedef enum
{
	GME_EFCT_ENE_IDX_E02_JET_S	= 0,	// E002 スティンガー バーニア（小）
	GME_EFCT_ENE_IDX_E02_JET_S_SMORK,	// E002 スティンガー バーニア（小）煙
	
	GME_EFCT_ENE_IDX_E04_DUMMY1,		// E004 メレオン
	GME_EFCT_ENE_IDX_E04_DUMMY2,		// E004 メレオン 
	GME_EFCT_ENE_IDX_E04_MEREON_MISS,	// E004 メレオン ミサイル変形
	
	GME_EFCT_ENE_IDX_E05_GUARD,			// E005 ガードン
	
	GME_EFCT_ENE_IDX_E06_HARO,			// E006 ハロゲン ランプ明滅
	
	GME_EFCT_ENE_IDX_E07_MOGU_E,		// E007 モグリン 地面破砕エフェクト
	GME_EFCT_ENE_IDX_E07_MOGU_W,		// E007 モグリン 地面破砕エフェクト（水中）
	
	GME_EFCT_ENE_IDX_E10_BUKUBUKU,		// E010 ブクブク スクリュー泡
	
	GME_EFCT_ENE_IDX_E13_CROW,			// E013 テルスター ツメエフェクト
	GME_EFCT_ENE_IDX_E13_T_STAR,		// E013 テルスター 本体爆発
	
	GME_EFCT_ENE_IDX_E14_JET_H,			// E014 ハリセンボ バーニア
	
	GME_EFCT_ENE_IDX_MAX
} GME_EFCT_ENE_IDX;



/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GmEfctEneBuildDataInit
/*!
  エネミー専用エフェクトデータ構築 開始
 */
// =======================================================================
extern void GmEfctEneBuildDataInit(GSE_MAIN_ZONE_TYPE zone_no);

// =======================================================================
// GmEfctEneBuildDataLoop
/*!
  エネミー専用エフェクトデータ構築 ループ
  
  @retval TRUE	構築完了
  @retval FALSE	構築中
 */
// =======================================================================
extern BOOL GmEfctEneBuildDataLoop(void);

// =======================================================================
// GmEfctEneFlushDataInit
/*!
  エネミー専用エフェクトデータ後片付け 開始
 */
// =======================================================================
extern void GmEfctEneFlushDataInit(GSE_MAIN_ZONE_TYPE zone_no);

// =======================================================================
// GmEfctEneFlushDataLoop
/*!
  エネミー専用エフェクトデータ後片付け ループ
  
  @retval TRUE	後片付け完了
  @retval FALSE	後片付け中
 */
// =======================================================================
extern BOOL GmEfctEneFlushDataLoop(void);

// =======================================================================
// GmEfctEneEsCreate
/*!
  エネミー専用エフェクト（ESタイプ）生成
  
  @param parent_obj		[io]	親オブジェクト（NULL可）
  @param ene_type		[in]	対象エネミーの番号
  @param efct_ene_idx	[in]	エフェクトインデックス
  
  
  @return エフェクト3DESワーク
 
  @note
  指定したエフェクトが現在のゾーンで呼び出せないものの場合はアサートします。
 */
// =======================================================================
extern GMS_EFFECT_3DES_WORK* GmEfctEneEsCreate(OBS_OBJECT_WORK *parent_obj,
											   Sint32 efct_ene_idx);



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

#endif /* GM_ENEMY_EFFECT_H_ */
