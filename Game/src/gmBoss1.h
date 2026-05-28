// =======================================================================
/*!
  @file	gmBoss1.h
  @brief ボス1

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss1.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_1_H_
#define GM_BOSS_1_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/
//############ 切り替えマクロ #################################################
#define GMD_BOSS1_BOOL_USE_OBSOLETE_FUNCTION		(0)		//!< 0なら参照されていない関数を無効化

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

// =======================================================================
// GmBoss1Build
/*!
  ボス１ データ構築
 */
// =======================================================================
extern void GmBoss1Build(void);

// =======================================================================
// GmBoss1Flush
/*!
  ボス１ データ片付け
 */
// =======================================================================
extern void GmBoss1Flush(void);

// =======================================================================
// GmBoss1Init
/*!
  ボス１（管理）初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss1Init(GMS_EVE_RECORD_EVENT *eve_rec,
									fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss1BodyInit
/*!
  ボス１本体初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss1BodyInit(GMS_EVE_RECORD_EVENT *eve_rec,
										fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss1ChainInit
/*!
  ボス１ 鎖 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss1ChainInit(GMS_EVE_RECORD_EVENT *eve_rec,
										 fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss1EggInit
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss1EggInit(GMS_EVE_RECORD_EVENT *eve_rec,
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

#endif /* GM_BOSS_1_H_ */
