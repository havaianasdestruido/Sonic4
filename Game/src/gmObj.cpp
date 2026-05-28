// ==========================================================================
/*!
  @file gmObj.c
  @brief メインゲーム オブジェクト関連処理

  @author Ishizaki
				Copyright(c) 2008 Dimps

  $Id: gmObj.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmMain.h"
#include "gsMainSys.h"
#include "objObject.h"
//#include "gmStage.h"
//#include "gmSound.h"
#include "gmPlayer.h"
//#include "gmEnemy.h"
//#include "gmDeco.h"

#include "gmObj.h"

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------
#if 0
/// 黒パレット
u16 const g_gm_obj_plt_black[16] = {
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
/// 白パレット
u16 const g_gm_obj_plt_white[16] = {
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,};
#endif

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// 汎用処理
// ==========================================================================
// ==========================================================================
// GmObjCollision
/*!
 *	あたりチェック
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		g_obj.ppCollision に設定
 */
// ==========================================================================
void GmObjCollision(OBS_OBJECT_WORK *obj_work)
{
	// 接触オブジェクトクリア
	obj_work->ride_obj = NULL;
	obj_work->touch_obj = NULL;

	// 地形判定
	ObjObjectCollision(obj_work);
	//objDebugRectDispObject(obj_work);
}

#if 0
// ==========================================================================
// GmObjDrawSort
/*!
 *	描画ソート処理
 *
 *	@note
 *		g_obj.ppDrawSort に設定
 */
// ==========================================================================
void GmObjDrawSort(void)
{
	OBS_OBJECT_WORK	*obj_work, *obj_work_cmp;

	g_obj.obj_draw_list_head = g_obj.obj_draw_list_tail = NULL;
//		g_obj.obj_draw_alpha_list_head = g_obj.obj_draw_alpha_list_tail = NULL;

	if (g_obj.obj_list_head == NULL) {
		// 描画するオブジェクトがなかった
		return;
	}

	for (obj_work = g_obj.obj_list_head; obj_work; obj_work = obj_work->next) {
		if (obj_work->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) {
			continue;
		}
#if 0
		if (obj_work->obj_3dss) {
			if (obj_work->obj_3dss->act_ss.poly_attr.alpha != 31) {
			// アルファタイプ
				if (g_obj.obj_draw_alpha_list_tail) {
					// Z座標ソート
					obj_work_cmp = g_obj.obj_draw_alpha_list_head;

					while (obj_work_cmp) {
						if (obj_work_cmp->pos.z > obj_work->pos.z) {
							break;
						}
						obj_work_cmp = obj_work_cmp->draw_next;
					}

					if (obj_work_cmp) {
						obj_work->draw_prev = obj_work_cmp->draw_prev;
						obj_work->draw_next = obj_work_cmp;
						obj_work_cmp->draw_prev = obj_work;
						if (obj_work->draw_prev) {
							obj_work->draw_prev->draw_next = obj_work;
						}
						else {
							g_obj.obj_draw_alpha_list_head = obj_work;
						}
					}
					else {
						// ソートの結果最後尾に追加
						obj_work->draw_prev = g_obj.obj_draw_alpha_list_tail;
						g_obj.obj_draw_alpha_list_tail->draw_next = obj_work;
						g_obj.obj_draw_alpha_list_tail = obj_work;
						obj_work->draw_next = NULL;
					}
				}
				else {
					g_obj.obj_draw_alpha_list_head = g_obj.obj_draw_alpha_list_tail = obj_work;
					obj_work->draw_prev = obj_work->draw_next = NULL;
				}
			}
			else {
			// 通常タイプ
				if (g_obj.obj_draw_list_tail) {
					// Z座標ソート
					obj_work_cmp = g_obj.obj_draw_list_head;

					while (obj_work_cmp) {
						if (obj_work_cmp->pos.z < obj_work->pos.z) {
							break;
						}
						obj_work_cmp = obj_work_cmp->draw_next;
					}

					if (obj_work_cmp) {
						obj_work->draw_prev = obj_work_cmp->draw_prev;
						obj_work->draw_next = obj_work_cmp;
						obj_work_cmp->draw_prev = obj_work;
						if (obj_work->draw_prev) {
							obj_work->draw_prev->draw_next = obj_work;
						}
						else {
							g_obj.obj_draw_list_head = obj_work;
						}
					}
					else {
						// ソートの結果最後尾に追加
						obj_work->draw_prev = g_obj.obj_draw_list_tail;
						g_obj.obj_draw_list_tail->draw_next = obj_work;
						g_obj.obj_draw_list_tail = obj_work;
						obj_work->draw_next = NULL;
					}
				}
				else {
					g_obj.obj_draw_list_head = g_obj.obj_draw_list_tail = obj_work;
					obj_work->draw_prev = obj_work->draw_next = NULL;
				}
			}
		}
		else {
			// SSでないのでソートの必要なし or
			// ソートの結果最後尾に追加
			if (g_obj.obj_draw_list_tail) {
				obj_work->draw_prev = g_obj.obj_draw_list_tail;
				g_obj.obj_draw_list_tail->draw_next = obj_work;
				g_obj.obj_draw_list_tail = obj_work;
				obj_work->draw_next = NULL;
			}
			else {
				g_obj.obj_draw_list_head = g_obj.obj_draw_list_tail = obj_work;
				obj_work->draw_prev = obj_work->draw_next = NULL;
			}
		}
#else

		if (g_obj.obj_draw_list_tail) {
			if (obj_work->ppOut || obj_work->ppOutSub) {
				// Z座標ソート
				obj_work_cmp = g_obj.obj_draw_list_head;

				while (obj_work_cmp) {
					if (obj_work_cmp->pos.z > obj_work->pos.z) {
						break;
					}
					obj_work_cmp = obj_work_cmp->draw_next;
				}

				if (obj_work_cmp) {
					obj_work->draw_prev = obj_work_cmp->draw_prev;
					obj_work->draw_next = obj_work_cmp;
					obj_work_cmp->draw_prev = obj_work;
					if (obj_work->draw_prev) {
						obj_work->draw_prev->draw_next = obj_work;
					}
					else {
						g_obj.obj_draw_list_head = obj_work;
					}
					continue;
				}
			}
			// 描画がないのでソートの必要なし or
			// ソートの結果最後尾に追加
			obj_work->draw_prev = g_obj.obj_draw_list_tail;
			g_obj.obj_draw_list_tail->draw_next = obj_work;
			g_obj.obj_draw_list_tail = obj_work;
			obj_work->draw_next = NULL;
		}
		else {
			g_obj.obj_draw_list_head = g_obj.obj_draw_list_tail = obj_work;
			obj_work->draw_prev = obj_work->draw_next = NULL;
		}
#endif
	}


}
#endif

// ==========================================================================
// GmObjPreFunc
/*!
 *	オブジェクトシステム 前処理
 *
 *	@note
 *		g_obj.ppPre に設定
 */
// ==========================================================================
void GmObjPreFunc(void)
{
#if 0
	OBS_DATA_WORK	*obj_work;
	NNS_TEXLIST		*texlist;
	// 強制オブジェクトポーズの時は判定しない
//	if (!(g_obj.flag & (OBD_OBJ_FORCE_PAUSE|OBD_OBJ_ACTER_FORCE_PAUSE))) {
//		// 体押し合いチェック
//		GmHitCheckBodyPush();
//	}
	if (!ObjObjectPauseCheck(0)) {
		// 軌跡エフェクト更新
		amTrailEFUpdate( AMTRE_HANDLE_ACCELL );
	}
	if (g_obj.glb_camera_id != -1) {
		// 仮カメラ
		NNS_VECTOR	sort_cam_pos;
		NNS_VECTOR	cam_ofst;
		NNS_MATRIX	obj_mtx;

		nnMakeUnitMatrix(&obj_mtx);	// 変更必要？

		// 3DNNのカメラ設定
		ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, g_obj.glb_camera_type, OBD_DRAW_CMD_STATE_3DNN);
		// カメラ座標を取得
		ObjCameraDispPosGet(g_obj.glb_camera_id, &sort_cam_pos);
		// カメラとエフェクトとの本来の位置関係になるように
		// ソート用カメラの位置を調整する
		amVectorSet(&cam_ofst,
					-NNM_MTX(obj_mtx, 0, 3),
					-NNM_MTX(obj_mtx, 1, 3),
					-NNM_MTX(obj_mtx, 2, 3));
		nnAddVector(&sort_cam_pos, &cam_ofst, &sort_cam_pos);
		
		// ソート用カメラ座標設定
		amEffectSetCameraPos(&sort_cam_pos);
	}

	// 軌跡描画
	NNS_RGBA		diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
	NNS_RGB			ambient = { 1.0f, 1.0f, 1.0f};

	nnSetPrimitive3DMaterial(&diffuse, &ambient, 1.0f);
	obj_work = ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_TEXLIST);
	texlist = (NNS_TEXLIST*)obj_work->pData;
	amTrailEFDraw( AMTRE_HANDLE_ACCELL, texlist, OBD_DRAW_CMD_STATE_3DNN);
	//amTrailEFDraw( AMTRE_HANDLE_ACCELL, texlist, OBD_DRAW_CMD_STATE_POST_WATER);
#endif
}

// ==========================================================================
// GmObjRegistRectAuto
/*!
 *	自動矩形登録
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		g_obj.ppRegRecAuto に設定
 */
// ==========================================================================
void GmObjRegistRectAuto(OBS_OBJECT_WORK *obj_work)
{
	s32				i;
	OBS_RECT_WORK	*rect_work;

	/* 矩形自動登録 */
	//if (!pWork->rect_work || !pWork->rect_num) {
	if (obj_work->rect_work && obj_work->rect_num) {
		for (i = 0, rect_work = obj_work->rect_work; i < (s32)obj_work->rect_num; i++, rect_work++) {
		//	if (rect_work->attr_flag & (GMD_OBJ_RECT_ATTR_FLAG_HIT | GMD_OBJ_RECT_ATTR_FLAG_NOAUTOREG)) {
		//		// すでにあたっている場合は登録しない
		//		//rect_work->flag &= ~OBD_RECT_ENABLE;
		//		continue;
		//	}

			ObjObjectRectRegist(obj_work, rect_work);
		}
	}

	/* オブジェクト地形自動登録 */
	if (obj_work->col_work && obj_work->col_work->obj_col.obj) {
		if (!ObjObjectViewOutCheck(obj_work)) {
			// 登録
			ObjCollisionObjectRegist(&obj_work->col_work->obj_col);
		}
	}
}

// ==========================================================================
// GmObjObjPreFunc
/*!
 *	オブジェクトシステム オブジェクト共通前処理
 *
 *	@note
 *		g_obj.ppObjPre に設定
 */
// ==========================================================================
void GmObjObjPreFunc(OBS_OBJECT_WORK *obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);
}

// ==========================================================================
// GmObjObjPostFunc
/*!
 *	オブジェクトシステム オブジェクト共通後処理
 *
 *	@note
 *		g_obj.ppObjPost に設定
 */
// ==========================================================================
void GmObjObjPostFunc(OBS_OBJECT_WORK *obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);
}

#if 0
// ==========================================================================
// GmObjActionCallBackSetRect
/*!
 *	アクションコールバックでの矩形情報設定
 *
 *	@param	cmd_rect	[in]	矩形コマンド情報
 *	@param	rect_work	[in]	設定する矩形ワーク
 */
// ==========================================================================
void GmObjActionCallBackSetRect(const MTS_ACT_COMMAND_RECT *cmd_rect, OBS_RECT_WORK *rect_work)
{
	GMA_OBJ_RECT_WORK_USER_DATA	*user_data;

	MTM_ASSERT(cmd_rect);
	MTM_ASSERT(rect_work);

	if ((cmd_rect->rect[MTD_LEFT] == cmd_rect->rect[MTD_RIGHT]) && (cmd_rect->rect[MTD_TOP] == cmd_rect->rect[MTD_BOTTOM])) {
		// OBD_RECT_ENABLE OFF
		rect_work->flag &= ~OBD_RECT_ENABLE;
	}
	else {
		// HITフラグを落とす
		rect_work->flag &= ~(OBD_RECT_HIT | OBD_RECT_DAMAGE);
		rect_work->attr_flag &= ~(GMD_OBJ_RECT_ATTR_FLAG_HIT | GMD_OBJ_RECT_ATTR_FLAG_DEF);

		// OBD_RECT_ENABLE も ON になる
		// 奥行きは現在の値を保持
		ObjRectWorkZSet(rect_work,
				cmd_rect->rect[MTD_LEFT], cmd_rect->rect[MTD_TOP], -GMD_OBJ_RECT_DEFAULT_DEPTH,
				cmd_rect->rect[MTD_RIGHT], cmd_rect->rect[MTD_BOTTOM], GMD_OBJ_RECT_DEFAULT_DEPTH);

		// 矩形タイプ保存
		user_data = (GMA_OBJ_RECT_WORK_USER_DATA*)&rect_work->user_data;
		user_data->rect_type = (u8)cmd_rect->no;
	}
}
#endif


// ==========================================================================
// Utility
// ==========================================================================
// ==========================================================================
// GmObjCheckMapLeftLimit
/*!
 *	オブジェクトがMAP左端にあるかチェック
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *	@param	ofst		[in]	MAP端判定オフセット
 *
 *	@return	TRUE : オブジェクトが画面左端にある
 */
// ==========================================================================
BOOL GmObjCheckMapLeftLimit(OBS_OBJECT_WORK *obj_work, s32 ofst)
{
	if (obj_work->move_flag & (OBD_MOVE_FRONT | OBD_MOVE_BACK) &&
			((obj_work->pos.x >> FX32_SHIFT) <= g_gm_main_system.map_fcol.left + ofst)) {
		return (TRUE);
	}
	return (FALSE);
}

// ==========================================================================
// GmObjCheckMapRightLimit
/*!
 *	オブジェクトがMAP右端にあるかチェック
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *	@param	ofst		[in]	MAP端判定オフセット
 *
 *	@return	TRUE : オブジェクトが画面右端にある
 */
// ==========================================================================
BOOL GmObjCheckMapRightLimit(OBS_OBJECT_WORK *obj_work, s32 ofst)
{
	if (obj_work->move_flag & (OBD_MOVE_FRONT | OBD_MOVE_BACK) &&
			((obj_work->pos.x >> FX32_SHIFT) >= g_gm_main_system.map_fcol.right - ofst)) {
		return (TRUE);
	}
	return (FALSE);
}


// ==========================================================================
// GmObjSetClip
/*!
 *	画面外クリッピング設定
 *
 *	@param	obj_work	[inout]	オブジェクトワーク
 *	@param	out_ofst	[in]	クリップ範囲オフセット
 *	@parma	plus_top	[in]	クリップ範囲オフセット加算値 LEFT
 *	@parma	plus_left	[in]	クリップ範囲オフセット加算値 TOP
 *	@parma	plus_right	[in]	クリップ範囲オフセット加算値 RIGHT
 *	@parma	plus_bottom	[in]	クリップ範囲オフセット加算値 BOTTOM
 */
// ==========================================================================
void GmObjSetClip(OBS_OBJECT_WORK *obj_work, s16 out_ofst, s16 plus_left, s16 plus_top, s16 plus_right, s16 plus_bottom)
{
	MTM_ASSERT(obj_work);

	// 汎用画面外チェック関数設定
	obj_work->ppViewCheck = ObjObjectViewOutCheck;

	obj_work->view_out_ofst = out_ofst;
	obj_work->view_out_ofst_plus[MTD_LEFT]		= plus_left;
	obj_work->view_out_ofst_plus[MTD_TOP]		= plus_top;
	obj_work->view_out_ofst_plus[MTD_RIGHT]		= plus_right;
	obj_work->view_out_ofst_plus[MTD_BOTTOM]	= plus_bottom;

	// NOCLIP OFF
	obj_work->flag &= ~OBD_OBJECT_NOCLIP;
}

// ==========================================================================
// GmObjGetRotPosXY
/*!
 *	XY座標を指定の角度で回転した座標を取得する
 *
 *	@param	pos_x		[in]	回転する元座標X
 *	@param	pos_y		[in]	回転する元座標Y
 *	@param	dest_x		[in]	回転後の座標Xを格納するバッファ(NULL不可)
 *	@param	dest_y		[in]	回転後の座標Yを格納するバッファ(NULL不可)
 *	@param	dir			[in]	回転角度
 *
 *	@note
 *		dest_x, dest_y は必ず指定する事\n
 *		dir == 0 でも計算するので、必要ならば外でチェックする事
 */
// ==========================================================================
void GmObjGetRotPosXY(fx32 pos_x, fx32 pos_y, fx32 *dest_x, fx32 *dest_y, u16 dir)
{
	fx32	x_sin, x_cos, y_sin, y_cos;

	MTM_ASSERT(dest_x && dest_y);

	x_sin = FX_Mul(pos_x, mtMathSin(dir));
	x_cos = FX_Mul(pos_x, mtMathCos(dir));
	y_sin = FX_Mul(pos_y, mtMathSin(dir));
	y_cos = FX_Mul(pos_y, mtMathCos(dir));

	*dest_x = x_cos - y_sin;
	*dest_y = x_sin + y_cos;
}

// ==========================================================================
// GmObjSetAllObjectNoDisp
/*!
 *	存在するすべてのオブジェクトをNODISPにする
 */
// ==========================================================================
void GmObjSetAllObjectNoDisp(void)
{
	OBS_OBJECT_WORK	*obj_work;
	
	obj_work = ObjObjectSearchRegistObject(NULL, 0xFFFF);
	while (obj_work) {
		if (!(obj_work->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST))) {
			obj_work->disp_flag |= OBD_DISP_NODISP;
		}

		obj_work = ObjObjectSearchRegistObject(obj_work, 0xFFFF);
	}
}

// ==========================================================================
// GmObjSetObjectNoFunc
/*!
 *	指定のオブジェクトタイプのオブジェクトの処理をNOFUNCにする
 *
 *	@param	obj_type_flag [in]	オブジェクトタイプフラグ
 *
 *	@note
 *		オブジェクトタイプフラグは、obj_typeでシフトしたフラグを使う\n
 *		(1 << obj_type1) | (1 << obj_type2) | (1 << obj_type3) ....\n
 *		最上位ビットは使えません
 */
// ==========================================================================
void GmObjSetObjectNoFunc(u32 obj_type_flag)
{
	u32				obj_type_flag_work;
	u16				i;
	OBS_OBJECT_WORK	*obj_work;

	MTM_ASSERT(!(obj_type_flag & 0x80000000));

	obj_type_flag_work = obj_type_flag & 0x7FFFFFFF;

	for (i = 0; i < 31 && obj_type_flag_work; i++, obj_type_flag_work >>= 1) {
		if (!(obj_type_flag_work & 0x00000001)) {
			continue;
		}

		obj_work = ObjObjectSearchRegistObject(NULL, i);
		while (obj_work) {
			if (!(obj_work->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST))) {
				obj_work->flag |= OBD_OBJECT_NOFUNC | OBD_OBJECT_NOHIT;
				obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;
			}

			obj_work = ObjObjectSearchRegistObject(obj_work, i);
		}
	}
}

// ==========================================================================
// 矩形
// ==========================================================================

//----- Local Functions -----------------------------------------------------

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
