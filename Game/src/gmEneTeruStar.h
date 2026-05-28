// ==========================================================================
/*!
  @file gmEneTStarrin.h
  @brief エネミー テルスター

  @author Yurita
				Copyright(c) 2009 Dimps

  $Id: gmEneTeruStar.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_ENE_T_STAR_H_
#define GM_ENE_T_STAR_H_


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
// GmEneTStarBuild
/*!
 *	エネミー テルスター データ構築
 */
// ==========================================================================
extern void GmEneTStarBuild(void);

// ==========================================================================
// GmEneTStarFlush
/*!
 *	エネミー テルスター データ片付け
 */
// ==========================================================================
extern void GmEneTStarFlush(void);

// ==========================================================================
// GmEneTStarInit
/*!
 *	エネミー テルスター 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *		user_work	: 左移動限界\n
 *		usre_flag	: 右移動限界
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmEneTStarInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


// ==========================================================================
// GmEneTStarInit
/*!
 *	エネミー テルスター(とげ) 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *		user_work	: 左移動限界\n
 *		usre_flag	: 右移動限界
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmEneTStarNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);


#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_ENE_T_STAR_H_

//----- Include Files -------------------------------------------------------
