// ==========================================================================
/*!
  @file dbgDmyFunc.h
  @brief デバック用ダミー処理

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dbgDmyFunc.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#include "mt.h"

// #if defined(MTD_DEBUG) ※いくつか未実装イベントがあるためリリース版でも有効にする
// 全文デバックコード

#ifndef DBG_DMY_FUNC_H_
#define DBG_DMY_FUNC_H_



//----- Include Files -------------------------------------------------------


#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// DbgDummyInitLogo
// DbgDummyInitTitle
// DbgDummyInitMainMenu
// DbgDummyInitMap
// DbgDummyInitMainGame
// DbgDummyInitResult
// DbgDummyInitOption
// DbgDummyInitEnding
// DbgDummyInitStaffRoll
/*!
 *	ダミールーチン 初期化
 */
// ==========================================================================
extern void DbgDummyInitLogo(void *arg);
extern void DbgDummyInitTitle(void *arg);
extern void DbgDummyInitMainMenu(void *arg);
extern void DbgDummyInitMap(void *arg);
extern void DbgDummyInitMainGame(void *arg);
extern void DbgDummyInitResult(void *arg);
extern void DbgDummyInitOption(void *arg);
extern void DbgDummyInitEnding(void *arg);
extern void DbgDummyInitStaffRoll(void *arg);


#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // DBG_DMY_FUNC_H_

// #endif // #if defined(MTD_DEBUG)

//----- Include Files -------------------------------------------------------
