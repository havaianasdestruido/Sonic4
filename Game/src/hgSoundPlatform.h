// =======================================================================
/*!
  @file	hgSoundPlatform.h
  @brief HOGサウンド プラットフォーム専用処理

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: hgSoundPlatform.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef HG_SOUND_PLATFORM_H_
#define HG_SOUND_PLATFORM_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/
#if _WII
// =======================================================================
// HgSoundPfWiiCallbackEnterHBM
/*!
  Wii HBMへ移行時サウンド操作 コールバック関数
  
  @param arg	[io]	引数
 */
// =======================================================================
extern void HgSoundPfWiiCallbackEnterHBM(void *arg);

// =======================================================================
// HgSoundPfWiiCallbackLeaveHBM
/*!
  Wii HBMから復帰時サウンド操作 コールバック関数
  
  @param arg	[io]	引数
 */
// =======================================================================
extern void HgSoundPfWiiCallbackLeaveHBM(void *arg);

// =======================================================================
// HgSoundPfWiiCallbackFadeHBM
/*!
  Wii HBMフェード処理中 サウンド操作 コールバック関数
  
  @param arg	[io]	引数
  
  @retval 1		フェード中
  @retval -1	フェード中ではない
 */
// =======================================================================
extern Sint32 HgSoundPfWiiCallbackFadeHBM(void *arg);
#endif /* _WII */

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

#endif /* HG_SOUND_PLATFORM_H_ */
