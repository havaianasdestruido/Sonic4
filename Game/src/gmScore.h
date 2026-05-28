// ==========================================================================
/*!
  @file gmScore.h
  @brief スコア表示関連

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmScore.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_SCORE_H_
#define GM_SCORE_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
/// 振動レベル
typedef enum tag_GME_SCORE_VIB_LEVEL {
	GME_SCORE_VIB_LEVEL_0	= 0,		//!< 振動なし
	GME_SCORE_VIB_LEVEL_1,				//!< 振動強度1
	GME_SCORE_VIB_LEVEL_2,				//!< 振動強度2
	GME_SCORE_VIB_LEVEL_3,				//!< 振動強度3

	GME_SCORE_VIB_LEVEL_MAX
} GME_SCORE_VIB_LEVEL;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// GmScoreCreateScore
/*!
 *	スコア表示生成
 *
 *	@param	score		[in]	スコア
 *	@param	pos_x		[in]	表示位置X
 *	@param	pos_y		[in]	表示位置Y
 *	@param	scale		[in]	表示スケール
 *	@pamra	vib_level	[in]	振動レベル
 */
// ==========================================================================
extern void GmScoreCreateScore(s32 score, fx32 pos_x, fx32 pos_y, fx32 scale, GME_SCORE_VIB_LEVEL vib_level);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_SCORE_H_

//----- Include Files -------------------------------------------------------
