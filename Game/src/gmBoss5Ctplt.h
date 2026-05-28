// =======================================================================
/*!
  @file	gmBoss5Ctplt.h
  @brief ボスファイナル カタパルト

  @author Keisuke Tanaka
 				Copyright(c) 2008 Dimps
  $Id: gmBoss5Ctplt.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS5_CTPLT_H_
#define GM_BOSS5_CTPLT_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GmBoss5CtpltInit
/*!
  ボスFINAL カタパルト初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss5CtpltInit(GMS_EVE_RECORD_EVENT *eve_rec,
										 fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss5CtpltCreate
/*!
  ボスFINALカタパルト生成
  
  @param body_work	[io]	本体ワーク
  
  @note
  カタパルトを生成する際はこの関数を呼び出してください。
 */
// =======================================================================
extern void GmBoss5CtpltCreate(GMS_BOSS5_BODY_WORK *body_work);



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

#endif /* GM_BOSS5_CTPLT_H_ */
