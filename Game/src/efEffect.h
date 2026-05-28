// ================================================================
/*!
  @file efEffect.h
  @brief 特殊効果まとめ

  @author mana
                Copyright(c) 2004 Dimps

  $Id: efEffect.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 *
 */

#ifndef _H_EFEFFECT
#define _H_EFEFFECT


#ifndef _DS
#include "fx.h"
#include "typedef.h"
#endif

#include "efQuake.h"
#if defined _DS
#include "efFade.h"
#include "efFlash.h"
#include "efPltAnime.h"
#endif

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

/*----- Definitions ----------------------------------------------------*/
#define EFD_TASK_PRIO      ( 0x3E00 ) ///< エフェクトタスク優先度
#define EFD_TASK_GROUP      ( 3 )     ///< エフェクトタスクグループ番号

//----- External Declarations ------------------------------------------



#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _H_EFEFFECT
