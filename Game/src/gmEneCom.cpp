// ==========================================================================
/*!
  @file gmEneCom.cpp
  @brief 敵 共通処理

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEneCom.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmEffect.h"

#include "gmEneCom.h"

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
const static u16 gm_ene_com_atk_obj_atk_flag_tbl[GME_EFFECT_RECT_NUM]	= {
	0,		// 喰らい
	(GMD_OBJ_RECT_ATK_FLAG_NORMALATK | GMD_OBJ_RECT_ATK_FLAG_EFCTATK),	// 攻撃
};

const static u16 gm_ene_com_atk_obj_def_flag_tbl[GME_EFFECT_RECT_NUM]	= {
	GMD_OBJ_RECT_DEF_FLAG_EFCTDEF,		// 喰らい
	GMD_OBJ_RECT_DEF_FLAG_EFCTATK,		// 攻撃
};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// Util
// ==========================================================================
// ==========================================================================
// GmEneComCheckMoveLimit
/*!
 *	エネミー 移動限界チェック
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@return	TRUE : 限界内
 *
 *	@note
 *		左向きの時は左境界を、右向きの時は右境界を確認します。\n
 *		境界値の上は範囲外とします
 */
// ==========================================================================
BOOL GmEneComCheckMoveLimit(OBS_OBJECT_WORK *obj_work, fx32 limit_left, fx32 limit_right)
{
	if (((obj_work->disp_flag & OBD_DISP_HFLIP) && obj_work->pos.x <= limit_left) ||
			(!(obj_work->disp_flag & OBD_DISP_HFLIP) && obj_work->pos.x >= limit_right)) {
		return (FALSE);
	}
	return (TRUE);
}

// ==========================================================================
// GmEneComCreateAtkObject
/*!
 *	エネミー 攻撃用オブジェクト生成
 *
 *	@param	parent_obj		[in] 親オブジェクトワーク
 *	@param	view_out_ofst	[in] クリッピングオフセット
 *
 *	@note
 *		攻撃用のオブジェクトを生成\n
 *		エフェクトとして生成します。
 */
// ==========================================================================
OBS_OBJECT_WORK* GmEneComCreateAtkObject(OBS_OBJECT_WORK *parent_obj, s16 view_out_ofst)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_EFFECT_COM_WORK	*efct_com;

	obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_COM_WORK), parent_obj,
						0/*sort_prio*/, parent_obj->tcb->am_tcb->name);
	efct_com = (GMS_EFFECT_COM_WORK*)obj_work;

	obj_work->flag &= ~(OBD_OBJECT_NOHIT | OBD_OBJECT_NOCLIP);
	obj_work->move_flag |= OBD_MOVE_NOCOL;

	obj_work->view_out_ofst = view_out_ofst;

	GmEffectRectInit(efct_com,
					 gm_ene_com_atk_obj_atk_flag_tbl,
					 gm_ene_com_atk_obj_def_flag_tbl,
					 GMD_OBJ_RECT_GROUP_ENEMY,
					 GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);

	return (obj_work);
}

//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
