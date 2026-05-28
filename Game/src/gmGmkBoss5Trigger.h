// =======================================================================
/*!
  @file	gmGmkBoss5Trigger.h
  @brief ボスFINAL発動トリガ

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmGmkBoss5Trigger.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_GMK_BOSS5_TRIGGER_H_
#define GM_GMK_BOSS5_TRIGGER_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GmGmkBoss5TriggerInit
/*!
  ボスFINAL 発動トリガ 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmGmkBoss5TriggerInit(GMS_EVE_RECORD_EVENT *eve_rec,
											  fx32 pos_x, fx32 pos_y, u8 type);


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

#endif /* GM_GMK_BOSS5_TRIGGER_H_ */
