// ==========================================================================
/*!
  @file gmGmkSpipe.cpp
  @brief ギミック Ｓ字パイプ

  @author K.Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkSpipe.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ==========================================================================
/*
 * $Log: $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmEnemy.h"
#include "gmMain.h"
#include "gmTask.h"
#include "gmPlySeq.h"
#include "gmGmkSpipe.h"
#include "gmPlySeqGmk.h"

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSpipeDefFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------

// ==========================================================================
// GmGmkSpipeInit
/*!
 *	ギミック Ｓ字パイプ 初期化関数
 *
 *	@param	eve_rec	[inout]	レコードポインタ
 *	@param	pos_x	[in]	出現座標
 *	@param	pos_y	[in]
 *	@param	type	[in]	処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSpipeInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_COM_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_COM_WORK), "GMK_S_PIPE");
	gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;

	// 個別設定
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_DISP_NODISP;

	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

    // 矩形設定
	rect_work = &gmk_work->rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectGroupSet(rect_work,
						GMD_OBJ_RECT_GROUP_ENEMY,
						GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
	ObjRectAtkSet(rect_work, 0, GMD_OBJ_RECT_ATK_POWER_DEFAULT);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);	// 身体のみHIT可
	ObjRectSet(&rect_work->rect,
			(s16)(eve_rec->left*2),
			(s16)(eve_rec->top*2),
			(s16)((u16)(eve_rec->width*2) + (s16)(eve_rec->left*2)),
			(s16)((u16)(eve_rec->height*2) + (s16)(eve_rec->top*2)));
	rect_work->ppDef = gmGmkSpipeDefFunc;
	rect_work->parent_obj = obj_work;
	rect_work->flag |= OBD_RECT_NODAMAGE | OBD_RECT_NOHIT_UP;

	return (obj_work);
}



//----- Local Functions -----------------------------------------------------

// ================================================================
// gmGmkSpipeDefFunc
/*!
  ヒット時処理

	@param	mine_rect		[in]	自分矩形ワークポインタ
	@param	match_rect		[in]	相手矩形ワークポインタ

  @note
	ppDefへ登録
 */
// ================================================================
void gmGmkSpipeDefFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect)
{
	GMS_ENEMY_COM_WORK	*gmk_work = (GMS_ENEMY_COM_WORK *)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK *)match_rect->parent_obj;
//	OBS_OBJECT_WORK		*ply_obj = (OBS_OBJECT_WORK *)ply_work;
    
    if ( gmk_work == NULL ) return;
    if ( ply_work == NULL ) return;
	if (ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		return;
	}

	if (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_SPIPE) {
		GmPlySeqInitSpipe(ply_work);
	}
	ply_work->gmk_flag |= GMD_PLGF_GMK_S_PIPE;
#if 0
	switch ( gmk_work->eve_rec->id ) {
	default:
		MTM_ASSERT(!"gmGmkFlagChangeDefFunc:eve_id error\n");
	case GMD_EVENT_ID_GMK_TOUCH_EARTH:
		// 接地フラグ
		if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER && ply_obj->move_flag & OBD_MOVE_JUMP) {
			ply_work->gmk_flag |= GMD_PLGF_TOUCH;
			if ( gmk_work->eve_rec->flag & GMD_GMK_TOUCH_EARTH_LAND_FLIP ) {
				ply_work->gmk_flag |= GMD_PLGF_TOUCH_FLIP;
			}
			// トリックコンボクリア
			//GmPlayerStateClearTrickCombo(ply_work);
        }
        break;
	case GMD_EVENT_ID_GMK_A:
		// A面へ移行
		ply_obj->flag &= ~OBD_OBJECT_B;
		if ( ply_obj->obj_type == GMD_OBJTYPE_PLAYER ) {
			ply_work->graind_prev_ride = 0;
		}
		break;
	case GMD_EVENT_ID_GMK_B:
		// B面へ移行
		ply_obj->flag |= OBD_OBJECT_B;
		if ( ply_obj->obj_type == GMD_OBJTYPE_PLAYER ) {
			ply_work->graind_prev_ride = 0;
		}
		break;
	case GMD_EVENT_ID_FALLDIE:
		// 落下死亡判定
		//if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER && !(ply_obj->move_flag & OBD_MOVE_NOCOL)) {
		if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER) {
			GmPlySeqChangeDeath(ply_work);
		}
		break;
	case GMD_EVENT_ID_CHANGE_CAM_CENTER:
		// カメラセンター変更
		if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER) {
			u16	flag = gmk_work->eve_rec->flag;
			s16	camera_ofst;

			camera_ofst = (s16)(flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_X_MASK);
			if (flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_X_SIGN) {
				camera_ofst = (s16)-camera_ofst;
			}
			ply_work->gmk_camera_center_ofst_x = (s16)(camera_ofst << GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_MUL_SHIFT);

			camera_ofst = (s16)((flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_MASK) >> GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_SHIFT);
			if (flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_SIGN) {
				camera_ofst = (s16)-camera_ofst;
			}
			ply_work->gmk_camera_center_ofst_y = (s16)(camera_ofst << GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_MUL_SHIFT);
		}
		break;
	}
#endif
}
// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
