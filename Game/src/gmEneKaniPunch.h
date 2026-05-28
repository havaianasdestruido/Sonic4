// ==========================================================================
/*!
  @file gmEneKanirin.h
  @brief エネミー かにパンチ

  @author Yurita
				Copyright(c) 2009 Dimps

  $Id: gmEneKaniPunch.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_ENE_KANI_H_
#define GM_ENE_KANI_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

#include "gmEffect.h"
#include "gmBossCommon.h"		// ボスノードシステム組み込み

//----- Definitions ---------------------------------------------------------

//===========================================================================
//	エネミーノードシステム
//===========================================================================
#define	GMD_ENE_SNM_NO_MAX			(32)
typedef	struct	tag_GMS_ENE_NODE_MATRIX
{
	char					_id[8];			//!< 規定の文字が入る(開放処理用)
	Sint32					initCount;
	Sint32					useCount;
	GMS_BS_CMN_BMCB_MGR		mtn_mgr;		//!< モーションCB管理ワーク
	GMS_BS_CMN_SNM_WORK		snm_work;		//!< SNMワーク
	Sint32					work[GMD_ENE_SNM_NO_MAX];
	OBS_OBJECT_WORK*		obj_work;
} GMS_ENE_NODE_MATRIX;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// GmEneKaniBuild
/*!
 *	エネミー かにパンチ データ構築
 */
// ==========================================================================
extern void GmEneKaniBuild(void);

// ==========================================================================
// GmEneKaniFlush
/*!
 *	エネミー かにパンチ データ片付け
 */
// ==========================================================================
extern void GmEneKaniFlush(void);

// ==========================================================================
// GmEneKaniInit
/*!
 *	エネミー かにパンチ 初期化関数
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
extern OBS_OBJECT_WORK* GmEneKaniInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_ENE_KANI_H_

//----- Include Files -------------------------------------------------------
