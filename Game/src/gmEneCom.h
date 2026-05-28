// ==========================================================================
/*!
  @file gmEneCom.h
  @brief 敵 共通処理

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEneCom.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef _PT_H_
#define _PT_H_


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
// GmEneComCheckMoveLimit
/*!
 *	エネミー 移動限界チェック
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@return	TRUE : 限界内
 *
 *	@note
 *		境界は範囲外とします
 */
// ==========================================================================
extern BOOL GmEneComCheckMoveLimit(OBS_OBJECT_WORK *obj_work, fx32 limit_left, fx32 limit_right);

// ==========================================================================
// GmEneComActionSetDependHFlip
/*!
 *	エネミー アクション設定 Hフリップ依存
 *
 *	@param obj_work	[in] オブジェクトワーク
 *	@param act_id_r	[in] 右向きアクション
 *	@param act_id_l	[in] 左向きアクション
 */
// ==========================================================================
inline void GmEneComActionSetDependHFlip(OBS_OBJECT_WORK *obj_work, s32 act_id_r, s32 act_id_l)
{
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		ObjDrawObjectActionSet(obj_work, act_id_l);
	}
	else {
		ObjDrawObjectActionSet(obj_work, act_id_r);
	}
}

// ==========================================================================
// GmEneComActionSet3DNNBlendDependHFlip
/*!
 *	エネミー アクション設定 3DNN ブレンド Hフリップ依存
 *
 *	@param obj_work	[in] オブジェクトワーク
 *	@param act_id_r	[in] 右向きアクション
 *	@param act_id_l	[in] 左向きアクション
 */
// ==========================================================================
inline void GmEneComActionSet3DNNBlendDependHFlip(OBS_OBJECT_WORK *obj_work, s32 act_id_r, s32 act_id_l)
{
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		ObjDrawObjectActionSet3DNNBlend(obj_work, act_id_l);
	}
	else {
		ObjDrawObjectActionSet3DNNBlend(obj_work, act_id_r);
	}
}

// ==========================================================================
// GmEneComCreateAtkObject
/*!
 *	エネミー 攻撃用オブジェクト生成
 *
 *	@param parent_obj	[in] 親オブジェクトワーク
 *	@param	view_out_ofst	[in] クリッピングオフセット
 *
 *	@note
 *		攻撃用のオブジェクトを生成\n
 *		エフェクトとして生成します。\n
 *		返り値のワークは GMS_EFFECT_COM_WORK です。
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmEneComCreateAtkObject(OBS_OBJECT_WORK *parent_obj, s16 view_out_ofst);


// ==========================================================================
// GmEneComTargetIsLeft
/*!
 *	ターゲット位置チェック
 *
 *	@param mine_obj		[in] 自オブジェクトワーク
 *	@param target_obj	[in] ターゲットオブジェクトワーク
 *
 *	@return	TRUE : ターゲットが左にいる
 */
// ==========================================================================
inline BOOL GmEneComTargetIsLeft(OBS_OBJECT_WORK *mine_obj, OBS_OBJECT_WORK *target_obj)
{
	if (target_obj->pos.x < mine_obj->pos.x) {
		return (TRUE);
	}
	return (FALSE);
}

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _PT_H_

//----- Include Files -------------------------------------------------------
