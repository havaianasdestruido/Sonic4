// =======================================================================
/*!
  @file	hgTrophy.h
  @brief 実績・トロフィーシステム

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: hgTrophy.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */


/* 重複インクルード回避手法 */

#ifndef HG_TROPHY_H_
#define HG_TROPHY_H_

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

//! トロフィー獲得判定タイミング列挙型
typedef enum
{
	HGE_TROPHY_CHECK_TIMING_CLEAR_DEMO	= 0,	//!< クリアデモ時 諸々の情報更新後
	
	HGE_TROPHY_CHECK_TIMING_GOAL_IN,			//!< ゴール時（ゴールパネル通過）
	
	HGE_TROPHY_CHECK_TIMING_DEFEAT_BOSS,		//!< ボス撃破時
	
	HGE_TROPHY_CHECK_TIMING_INC_ENE_KILL_COUNT,	//!< エネミー撃破数増加時
	
	HGE_TROPHY_CHECK_TIMING_END_CREDITS_FINISHED,	//!< スタッフロール終了時
	
	HGE_TROPHY_CHECK_TIMING_UPLOAD_RECORD,		//!< 記録（スコア, タイム）アップロード時
	
	HGE_TROPHY_CHECK_TIMING_INC_CHALLENGE,		//!< チャレンジ数増加時
	
	HGE_TROPHY_CHECK_TIMING_CLEAR_ENDING_STAGE,	//!< エンディングステージクリア時
	
	HGE_TROPHY_CHECK_TIMING_MAX
} HGE_TROPHY_CHECK_TIMING;


/*------ External Declarations -----------------------------------------*/
// =======================================================================
// HgTrophyTryAcquisition
/*!
  トロフィー獲得判定＆更新
  
  @param timing	[in]	判定タイミング種別(HGE_TROPHY_CHECK_TIMING_XXX)
  
  @note
  指定タイミング種別に対応した解除判定・更新処理を行います。
 */
// =======================================================================
extern void HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING timing);

// =======================================================================
// HgTrophyIncEnemyKillCount
/*!
  エネミー撃破数増加
  
  @param ene_obj	[in]	エネミーオブジェクト
  
  @note
  エネミーを撃破したときに呼び出してください。
  対象となるエネミーオブジェクトの時のみカウントされます。
 */
// =======================================================================
extern void HgTrophyIncEnemyKillCount(const OBS_OBJECT_WORK *ene_obj);

// =======================================================================
// HgTrophyIncPlayerDamageCount
/*!
  プレイヤーダメージ回数増加
  
  @param ply_work	[io]	プレイヤーワーク
  
  @note
  プレイヤーがダメージを受けた際に呼び出してください。
 */
// =======================================================================
extern void HgTrophyIncPlayerDamageCount(const GMS_PLAYER_WORK *ply_work);

// =======================================================================
// HgTrophyIncFinalClearCount
/*!
  ファイナルゾーンクリア回数増加
  
  @note
  ファイナルゾーンクリア時に呼び出してください。
 */
// =======================================================================
extern void HgTrophyIncFinalClearCount(void);

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

#endif /* GS_TROPHY_H_ */
