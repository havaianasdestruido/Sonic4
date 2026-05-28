// ==========================================================================
/*!
  @file gmGmkItem.cpp
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGmkItem.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmObj.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmPlayer.h"
#include "gmPlySeq.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmComEfct.h"
#include "gmEffectZone.h"
#include "gmSound.h"
#include "gmPadVib.h"

#include "gmGmkItem.h"

// データヘッダ
#include "common/model/gmk_item_mdl.hmb"

//----- Definitions ---------------------------------------------------------
/* eve_rec->left */
/* eve_rec->top */
/* eve_rec->width */
/* eve_rec->height */
/* eve_rec->flag */
#define GMD_GMK_ITEM_EVE_FLAG_NO_GROUND_COL	(0x0001)	//!< 地面とのあたり, 落下無し

#define GMD_GMK_ITEM_BODY_RECT_LEFT		(-20 - 10)	//!< アイテム矩形サイズ
#define GMD_GMK_ITEM_BODY_RECT_TOP		(-40 - 10)
#define GMD_GMK_ITEM_BODY_RECT_RIGHT	(20 + 10)
#define GMD_GMK_ITEM_BODY_RECT_BOTTOM	(0 + 10)

#define GMD_GMK_ITEM_DEF_RECT_LEFT		(-30)		//!< アイテムくらい矩形サイズ
#define GMD_GMK_ITEM_DEF_RECT_TOP		(-GMD_GMK_ITEM_COL_RECT_HEIGHT - 16)
#define GMD_GMK_ITEM_DEF_RECT_RIGHT		(30)
#define GMD_GMK_ITEM_DEF_RECT_BOTTOM	(0)

#define GMD_GMK_ITEM_FIELD_RECT_LEFT	(-8)		//!< 地形あたり形サイズ
#define GMD_GMK_ITEM_FIELD_RECT_TOP		(-8)
#define GMD_GMK_ITEM_FIELD_RECT_RIGHT	(8)
#define GMD_GMK_ITEM_FIELD_RECT_BOTTOM	(0)

#define GMD_GMK_ITEM_COL_RECT_WIDTH		(8*5)
#define GMD_GMK_ITEM_COL_RECT_HEIGHT	(8*4)
#define GMD_GMK_ITEM_COL_RECT_OFST_X	(-GMD_GMK_ITEM_COL_RECT_WIDTH/2)
#define GMD_GMK_ITEM_COL_RECT_OFST_Y	(-GMD_GMK_ITEM_COL_RECT_HEIGHT)

#define GMD_GMK_ITEM_JUMP_SPD_Y			(-0x2000)	//!< 跳ね上げ速度

#define GMD_GMK_ITEM_EFFECT_WAIT_TIME	(60)		//!< アイテム効果発揮待機

#define GMD_GMK_ITEM_POPUP_EFCT_OFST_Y			(-21*FX32_ONE)		//!< 取得アイテム表示位置オフセット Y
#define GMD_GMK_ITEM_POPUP_EFCT_SCALE			(0x1800)			//!< 取得アイテムスケール
#define GMD_GMK_ITEM_POPUP_EFCT_SPD_Y			(-0x2800)			//!< 取得アイテム表示移動速度Y
#define GMD_GMK_ITEM_POPUP_EFCT_SPD_ADD_Y		(0x0140)			//!< 取得アイテム表示移動減速度Y	
#define GMD_GMK_ITEM_POPUP_EFCT_END_DISP_TIME	(30)				//!< 取得アイテム表示時間		

#define GMD_GMK_ITEM_BREAK_EFCT_NUM				(8)					//!< 破片数
#define GMD_GMK_ITEM_BREAK_EFCT_OFST_Y			(-14*FX32_ONE)		//!< 破片エフェクト表示位置オフセット Y
#define GMD_GMK_ITEM_BREAK_EFCT_BASE_SPD_X		(2*FX32_ONE)		//!< 破片エフェクト移動速度X
#define GMD_GMK_ITEM_BREAK_EFCT_BASE_SPD_Y		(-4*FX32_ONE)		//!< 破片エフェクト移動速度Y
#define GMD_GMK_ITEM_BREAK_EFCT_SPD_DEC_X		(0x080)				//!< 破片エフェクト減速度	

/// アイテムボックスマテリアルユーザーデータ設定
enum {
	GMD_GMK_ITEM_MAT_USER_DATA_BARRIA	= 1,
	GMD_GMK_ITEM_MAT_USER_DATA_HISPEED,
	GMD_GMK_ITEM_MAT_USER_DATA_INVINCIBLE,
	GMD_GMK_ITEM_MAT_USER_DATA_RING10,
	GMD_GMK_ITEM_MAT_USER_DATA_1UP,

	GMD_GMK_ITEM_MAT_USER_DATA_NORMAL_END,

	
	GMD_GMK_ITEM_MAT_USER_DATA_BODY		= 64		//!< ボックス本体
};

/// マテリアルコールバックユーザーパラメータ
typedef struct tag_GMS_GMK_ITEM_MAT_CB_PARAM {
	u32		draw_id;		//!< 描画するID(本体を除く)
} GMS_GMK_ITEM_MAT_CB_PARAM;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkItemMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkItemEffectWatiMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkItemFallCheckMain(OBS_OBJECT_WORK *obj_work);
static u16 gmGmkItemConvEvtId(u16 eve_id);
static void gmGmkItemOut(OBS_OBJECT_WORK *obj_work);
static void gmGmkItemBodyDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static void gmGmkItemDamageDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static NNE_BOOL gmGmkItemMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);

static void gmGmkItemCreatePopUpEffect(OBS_OBJECT_WORK *parent_obj, s32 mat_id);
static void gmGmkItemPopUpEffectMain(OBS_OBJECT_WORK *obj_work);

//static void gmGmkItemCreateBreakEffect(OBS_OBJECT_WORK *parent_obj, u32 efct_no, fx32 spd_x, fx32 spd_y, fx32 ofst_x, fx32 ofst_y);
//static void gmGmkItemBreakEffectMain(OBS_OBJECT_WORK *obj_work);
//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_item_obj_3d_list = NULL;

/// アイテム マテリアルユーザーデータ番号テーブル
static u32 gm_gmk_item_matrial_user_data_tbl[] = {
	GMD_GMK_ITEM_MAT_USER_DATA_HISPEED,		// ハイスピード
	GMD_GMK_ITEM_MAT_USER_DATA_INVINCIBLE,	// 無敵
	GMD_GMK_ITEM_MAT_USER_DATA_RING10,		// リング10
	GMD_GMK_ITEM_MAT_USER_DATA_BARRIA,		// バリア
	GMD_GMK_ITEM_MAT_USER_DATA_1UP,			// 1UP
};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkItemBuild
/*!
 *	ギミック アイテム データ構築
 */
// ==========================================================================
void GmGmkItemBuild(void)
{
	gm_gmk_item_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_ITEM_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_ITEM_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkItemFlush
/*!
 *	ギミック アイテム データ片付け
 */
// ==========================================================================
void GmGmkItemFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_ITEM_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_item_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkItemInit
/*!
 *	ギミック アイテム 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *		user_work	: 表示するマテリアルユーザーデータ番号\n
 *		user_flag	: モニターエフェクトワークアドレス
 */
// ==========================================================================
//float	item_ofst_x = 0;
//float	item_ofst_y = -13.5f;
//float	item_ofst_z = 10.f;
OBS_OBJECT_WORK* GmGmkItemInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_ITEM");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	if (eve_rec->byte_param[1]) {
		// 破壊されている
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_item_obj_3d_list[IDB_GMK_ITEM_MDL_GMK_ITEM_BREAK_ZNO],
						&gmk_work->obj_3d);
	}
	else {
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_item_obj_3d_list[IDB_GMK_ITEM_MDL_GMK_ITEM_ZNO],
						&gmk_work->obj_3d);
	}

	// 優先設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_C_BACK;//GMD_OBJ_DEFAULT_POS_Z_B;

	// マテリアルコールバック設定
	gmk_work->obj_3d.material_cb_func = gmGmkItemMaterialCallback;

	// 描画処理設定
	obj_work->ppOut = gmGmkItemOut;

	// 描画対象マテリアルユーザーデータ保存
	obj_work->user_work = gm_gmk_item_matrial_user_data_tbl[gmGmkItemConvEvtId(eve_rec->id) - GMD_EVENT_ID_GMK_ITEM_HISPEED];

	// 表示位置調整
	obj_work->disp_flag |= OBD_DISP_USERMTX_RIGHT;
	nnMakeUnitMatrix(&obj_work->obj_3d->user_obj_mtx_r);
	nnTranslateMatrix(&obj_work->obj_3d->user_obj_mtx_r, &obj_work->obj_3d->user_obj_mtx_r,
			0,
			(-1.f / FXM_FX32_TO_FLOAT(g_obj.draw_scale.y)),
			0);

	// 矩形設定
	// 対プレイヤー
	//gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;

	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = gmGmkItemBodyDefFunc;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work,
						GMD_GMK_ITEM_BODY_RECT_LEFT, GMD_GMK_ITEM_BODY_RECT_TOP,
						GMD_GMK_ITEM_BODY_RECT_RIGHT, GMD_GMK_ITEM_BODY_RECT_BOTTOM);
	//rect_work->flag |= OBD_RECT_OUT;

	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	rect_work->ppDef = gmGmkItemDamageDefFunc;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work,
						GMD_GMK_ITEM_DEF_RECT_LEFT, GMD_GMK_ITEM_DEF_RECT_TOP,
						GMD_GMK_ITEM_DEF_RECT_RIGHT, GMD_GMK_ITEM_DEF_RECT_BOTTOM);

	// 地形設定
	gmk_work->ene_com.col_work.obj_col.obj			= obj_work;
	gmk_work->ene_com.col_work.obj_col.width		= GMD_GMK_ITEM_COL_RECT_WIDTH;		// 地形サイズ設定(ドット)
	gmk_work->ene_com.col_work.obj_col.height		= GMD_GMK_ITEM_COL_RECT_HEIGHT;
	gmk_work->ene_com.col_work.obj_col.ofst_x		= GMD_GMK_ITEM_COL_RECT_OFST_X;
	gmk_work->ene_com.col_work.obj_col.ofst_y		= GMD_GMK_ITEM_COL_RECT_OFST_Y;

	// 地形あたり設定
	ObjObjectFieldRectSet(obj_work, GMD_GMK_ITEM_FIELD_RECT_LEFT, GMD_GMK_ITEM_FIELD_RECT_TOP,
						GMD_GMK_ITEM_FIELD_RECT_RIGHT, GMD_GMK_ITEM_FIELD_RECT_BOTTOM);

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag &= ~OBD_MOVE_FALL;		// はじめは落下なし
	obj_work->move_flag |= OBD_MOVE_NOCOLOBJ;	// オブジェクト地形は判定しない
	gmk_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOPRESSDIE;	// プレイヤーを圧死させない


	// メイン処理設定
	if (eve_rec->byte_param[1]) {
		// 壊れ済み
		gmk_work->ene_com.col_work.obj_col.obj = NULL;
		obj_work->flag |= OBD_OBJECT_NOHIT;

#if 1
		// 落下OFF
		obj_work->move_flag &= ~(OBD_MOVE_FALL | OBD_MOVE_JUMP);

		if (eve_rec->flag & GMD_GMK_ITEM_EVE_FLAG_NO_GROUND_COL) {
		// 地形無視(落下無し)
			// 移動無し 地形あたり無し
			obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;
		}
		// 通常 : 落下はしないが地形は判定する
#else
		if (eve_rec->flag & GMD_GMK_ITEM_EVE_FLAG_NO_GROUND_COL) {
		// 地形無視(落下無し)
			// 落下OFF
			obj_work->move_flag &= ~(OBD_MOVE_FALL | OBD_MOVE_JUMP);
			// 移動無し 地形あたり無し
			obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;
		}
		else {
		// 通常
			// 壊れたものは落下させる
			obj_work->move_flag |= OBD_MOVE_FALL | OBD_MOVE_JUMP;
			obj_work->move_flag &= ~OBD_MOVE_UNDER;

		}
#endif

		// 落下チェック処理
		obj_work->ppFunc = gmGmkItemFallCheckMain;
	}
	else {
		GMS_EFFECT_3DES_WORK	*efct_work;

		// 通常
		obj_work->ppFunc = gmGmkItemMain;

		// エフェクト モニター
		efct_work = GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_ITEM_01);
		//GmComEfctAddDispOffsetF(efct_work, item_ofst_x, item_ofst_y, item_ofst_z);
		GmComEfctAddDispOffsetF(efct_work, 0.f, -12.5f, 10.f);
		//efct_work->efct_com.obj_work.pos.z = (s32)item_ofst_z*FX32_ONE;
		efct_work->efct_com.obj_work.flag |= OBD_OBJECT_NOCLIP;
		obj_work->user_flag = (u32)efct_work;

		// 地形無視(落下無し)
		if (eve_rec->flag & GMD_GMK_ITEM_EVE_FLAG_NO_GROUND_COL) {
			// 落下OFF
			obj_work->move_flag &= ~(OBD_MOVE_FALL | OBD_MOVE_JUMP);

			// 移動無し 地形あたり無し
			obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

			// 跳ね上げ用矩形OFF
			gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;
		}
	}
	
#if OBD_OBJECT_USE_NOEXIST
	obj_work->flag |= OBD_OBJECT_NOEXIST_ENABLE;
#endif // OBD_OBJECT_USE_NOEXIST
	return (obj_work);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkItemMain
/*!
 *	ギミック アイテム メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkItemMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	if (gmk_work->ene_com.eve_rec->byte_param[1]) {
		//s32		i, parts_no;
		//fx32	spd_x, spd_y;
		//u16		rand;

		// 破壊された
		// 破壊モデルに切り替え
		ObjObjectAction3dNNModelReleaseCopy(obj_work);
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_item_obj_3d_list[IDB_GMK_ITEM_MDL_GMK_ITEM_BREAK_ZNO],
						&gmk_work->obj_3d);
		// イベント生成時もCopyで初期化しているので前モデル開放不要
		// OBS_ACTION3D_NN_WORK を自動取得している場合は開放が必要
		// 表示位置調整
		obj_work->disp_flag |= OBD_DISP_USERMTX_RIGHT;
		nnMakeUnitMatrix(&obj_work->obj_3d->user_obj_mtx_r);
		nnTranslateMatrix(&obj_work->obj_3d->user_obj_mtx_r, &obj_work->obj_3d->user_obj_mtx_r,
				0,
				(-1.f / FXM_FX32_TO_FLOAT(g_obj.draw_scale.y)),
				0);

		// 破片生成
#if 0
		for (i = 0, parts_no = 0; i < GMD_GMK_ITEM_BREAK_EFCT_NUM; i++, parts_no++) {
			if (parts_no > 2) {
				parts_no = 0;
			}

			rand = mtMathRand();
			spd_x = GMD_GMK_ITEM_BREAK_EFCT_BASE_SPD_X * ((rand & 0x8000) ? 1 : -1) +
							((rand & 0x0FFF) - 0x7FF);
			spd_y = GMD_GMK_ITEM_BREAK_EFCT_BASE_SPD_Y + ((rand & 0x0FFF) - 0x7FF);
					
			gmGmkItemCreateBreakEffect(obj_work, parts_no,
						spd_x, spd_y, (s32)mtMathRand() - 0x7FFF, GMD_GMK_ITEM_BREAK_EFCT_OFST_Y);
		}
#endif
	
		// エフェクト 破壊
		if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3) {
			// 1-3専用
			GmEfctZoneEsCreate(obj_work, GSD_MAIN_ZONE_TYPE_1, GME_EFCT_Z01_IDX_ITEM_Z1);
		}
		else if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_3) {
			if (GmMainIsWaterLevel() &&
					((obj_work->pos.y -48*FX32_ONE) >> FX32_SHIFT) > g_gm_main_system.water_level) {
				GmEfctZoneEsCreate(obj_work, GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_ITEM_Z3);
			}
			else {
				GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_ITEM);
			}
		}
		else {
			GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_ITEM);
		}

		// アイテムポップアップ
		gmGmkItemCreatePopUpEffect(obj_work, obj_work->user_work/*mat_id*/);

		// 効果発生待機処理へ移行
		obj_work->ppFunc = gmGmkItemEffectWatiMain;
		obj_work->user_timer = GMD_GMK_ITEM_EFFECT_WAIT_TIME*FX32_ONE;
		if (gmGmkItemConvEvtId(gmk_work->ene_com.eve_rec->id) == GMD_EVENT_ID_GMK_ITEM_1UP) {
			// 即時発動
			obj_work->user_timer = 1;
		}

		// 効果発生までクリッピングなし
		obj_work->flag |= OBD_OBJECT_NOCLIP;

		// モニターエフェクトを破棄
		if (obj_work->user_flag) {
			GMS_EFFECT_3DES_WORK	*efct_work;

			efct_work = (GMS_EFFECT_3DES_WORK*)obj_work->user_flag;
			efct_work->efct_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		}
	}

	if (obj_work->move_flag & OBD_MOVE_FALL) {
		// 接地したらFALLを切る
		if (obj_work->move_flag & OBD_MOVE_UNDER) {
			obj_work->move_flag &= ~(OBD_MOVE_FALL | OBD_MOVE_JUMP);
			obj_work->spd.y = obj_work->spd_add.y = 0;
			// 矩形HIT ON
			obj_work->flag &= ~OBD_OBJECT_NOHIT;
		}
	}
}

// ==========================================================================
// gmGmkItemEffectWatiMain
/*!
 *	ギミック アイテム 効果発揮待機 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkItemEffectWatiMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);
	if (obj_work->user_timer <= 0) {
		GMS_ENEMY_COM_WORK	*ene_com = (GMS_ENEMY_COM_WORK*)obj_work;
		// プレイヤー効果発揮
		if (ene_com->target_obj) {	// 念のため
			switch (gmGmkItemConvEvtId(ene_com->eve_rec->id)) {
			case GMD_EVENT_ID_GMK_ITEM_HISPEED:
				GmPlayerItemHiSpeedSet((GMS_PLAYER_WORK*)ene_com->target_obj);
				break;
			case GMD_EVENT_ID_GMK_ITEM_INVINCIBLE:
				GmPlayerItemInvincibleSet((GMS_PLAYER_WORK*)ene_com->target_obj);
				break;
			case GMD_EVENT_ID_GMK_ITEM_RING_10:
				GmPlayerItemRing10Set((GMS_PLAYER_WORK*)ene_com->target_obj);
				break;
			case GMD_EVENT_ID_GMK_ITEM_BARRIER:
				GmPlayerItemBarrierSet((GMS_PLAYER_WORK*)ene_com->target_obj);
				break;
			case GMD_EVENT_ID_GMK_ITEM_1UP:
				GmPlayerItem1UPSet((GMS_PLAYER_WORK*)ene_com->target_obj);
				break;
			default:
				;
				break;
			}
		}

		// ターゲットクリア
		ene_com->target_obj = NULL;

		// クリッピング再開
		obj_work->flag &= ~OBD_OBJECT_NOCLIP;

		// メイン処理移行
		obj_work->ppFunc = gmGmkItemFallCheckMain;
	}

	// 落下チェック
	gmGmkItemFallCheckMain(obj_work);
}

// ==========================================================================
// gmGmkItemFallCheckMain
/*!
 *	ギミック アイテム 落下終了チェック メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkItemFallCheckMain(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->move_flag & OBD_MOVE_FALL) {
		// 接地したらFALLを切る
		if (obj_work->move_flag & OBD_MOVE_UNDER) {
			obj_work->move_flag &= ~(OBD_MOVE_FALL | OBD_MOVE_JUMP);
			obj_work->spd.y = obj_work->spd_add.y = 0;
		}
	}
}

// ==========================================================================
// gmGmkItemGetEvtId
/*!
 *	ギミック アイテム イベントID変換
 *
 *	@param eve_id	[in] イベントID
 *
 *	@return	イベントID
 *
 *	@note
 *		モードにしたがってイベントIDを変換します。
 */
// ==========================================================================
u16 gmGmkItemConvEvtId(u16 eve_id)
{
	if (g_gs_main_sys_info.game_mode == GSD_GAME_MODE_TIME_ATTACK &&
			eve_id == GMD_EVENT_ID_GMK_ITEM_1UP) {
		eve_id = GMD_EVENT_ID_GMK_ITEM_RING_10;
	}

	return (eve_id);
}

// ==========================================================================
// gmGmkItemOut
/*!
 *	ギミック アイテム 描画処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		ppOutに登録
 */
// ==========================================================================
void gmGmkItemOut(OBS_OBJECT_WORK *obj_work)
{
	OBS_ACTION3D_NN_WORK		*obj_3d;
	GMS_GMK_ITEM_MAT_CB_PARAM	*item_mat_cb_param;

	obj_3d		= obj_work->obj_3d;

	// マテリアルコールバック設定
	item_mat_cb_param = (GMS_GMK_ITEM_MAT_CB_PARAM *)amDrawMallocDataBuffer(sizeof(GMS_GMK_ITEM_MAT_CB_PARAM));
	item_mat_cb_param->draw_id = obj_work->user_work;
	obj_3d->material_cb_param = item_mat_cb_param;

	ObjDrawActionSummary(obj_work);
}

// ==========================================================================
// gmGmkItemBodyDefFunc
/*!
 *	ギミック アイテム 矩形 体くらい処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 *		ppDefに登録
 */
// ==========================================================================
void gmGmkItemBodyDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_ENEMY_COM_WORK	*com_work = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	s16		ply_field_rect_top;
	fx32	item_col_under;

	if (!com_work || !ply_work) {
		// どちらか矩形の親がない
		return;
	}

	if (ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		// プレイヤーでない
		return;
	}

	if (ply_work->obj_work.touch_obj == &com_work->obj_work) {
		ply_field_rect_top	= ply_work->obj_work.field_rect[MTD_TOP];	// ◆上下反転状況等がある場合は調整
		item_col_under		= ((com_work->col_work.obj_col.height +
											com_work->col_work.obj_col.ofst_y) << FX32_SHIFT);

		if (ply_work->obj_work.pos.y + ply_field_rect_top >=
					com_work->obj_work.pos.y + item_col_under &&
					ply_work->obj_work.move.y <= 0) {
			// 下から突き上げをくらった場合は、上にはねる
			com_work->obj_work.spd.y = GMD_GMK_ITEM_JUMP_SPD_Y;
			com_work->obj_work.move_flag |= OBD_MOVE_FALL | OBD_MOVE_JUMP;		// ジャンプ開始
			com_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

			// 矩形HIT OFF
			com_work->obj_work.flag |= OBD_OBJECT_NOHIT;
		}
	}
}

// ==========================================================================
// gmGmkItemDamageDefFunc
/*!
 *	ギミック アイテム 矩形 ダメージくらい処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 *		ppDefに登録
 */
// ==========================================================================
void gmGmkItemDamageDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_ENEMY_COM_WORK	*com_work = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	if (!com_work || !ply_work) {
		// どちらか矩形の親がない
		return;
	}

	if (ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		// プレイヤーでない
		return;
	}

	if (ply_work->seq_state != GME_PLY_SEQ_STATE_HOMING &&
			ply_work->seq_state != GME_PLY_SEQ_STATE_SPIN &&
			ply_work->obj_work.touch_obj == &com_work->obj_work) {
		// ホーミング中でなく
		// スピンダッシュ中でなく
		// プレイヤーが自分に触れている場合はキャンセル
		return;
	}

	if (!(ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_SPIN)) {

		if (ply_work->obj_work.move.y <= 0) {
			// プレイヤーが上昇中はキャンセル
			return;
		}

		if (((ply_work->obj_work.pos.x + (ply_work->obj_work.field_rect[MTD_RIGHT]<<FX32_SHIFT)) <
					(com_work->obj_work.pos.x - GMD_GMK_ITEM_COL_RECT_WIDTH/2*FX32_ONE)) ||
				((ply_work->obj_work.pos.x + (ply_work->obj_work.field_rect[MTD_LEFT]<<FX32_SHIFT)) >
					(com_work->obj_work.pos.x + GMD_GMK_ITEM_COL_RECT_WIDTH/2*FX32_ONE))) {
			// 範囲外
			return;
		}

	}

	/* 破壊 */
	// 破壊状態を記録
	com_work->eve_rec->byte_param[1] = 1;

	// HIT OFF
	com_work->obj_work.flag |= OBD_OBJECT_NOHIT;
	com_work->col_work.obj_col.obj = NULL;

	// 壊れたものは落下させる
	com_work->obj_work.move_flag |= OBD_MOVE_FALL | OBD_MOVE_JUMP;
	com_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

	// プレイヤーを保存
	com_work->target_obj = &ply_work->obj_work;

	// プレイヤーをはじく
	GmPlySeqAtkReactionInit(ply_work);

	// SE
	GmSoundPlaySE("Enemy");

	// コントローラー振動
	GMM_PAD_VIB_SMALL();
}

// ==========================================================================
// gmGmkItemMaterialCallback
/*!
 *	ギミック アイテム マテリアルコールバック(OBF_MATERIAL_CB)
 *
 *	@param	val		[in]	ドローコールバック変数
 *	@param	param	[in]	ユーザーパラメータ
 */
// ==========================================================================
NNE_BOOL gmGmkItemMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	u32							user_data;
	GMS_GMK_ITEM_MAT_CB_PARAM	*user_param;

	if (param) {
		user_param = (GMS_GMK_ITEM_MAT_CB_PARAM*)param;

		// ユーザーデータ取得
		user_data = ObjDraw3DNNGetMaterialUserData(val);

		if (!user_data ||
				user_data == GMD_GMK_ITEM_MAT_USER_DATA_BODY ||
				user_data == user_param->draw_id) {
			return (nnPutMaterialCore(val));
		}

#if defined (MTD_DEBUG)
		if (user_data >= GMD_GMK_ITEM_MAT_USER_DATA_NORMAL_END) {
			// パラメータミス
			MTM_ASSERT(0);
		}
#endif
	}

	return (NNE_FALSE);

	//return (nnPutMaterialCore(draw_cb_val));
}

// ==========================================================================
// エフェクト
// ==========================================================================
// ==========================================================================
// gmGmkItemCreatePopUpEffect
/*!
 *	アイテム 取得時エフェクト生成
 *
 *	@param	mat_id	[in]	表示するマテリアルNO
 *
 *	@note
 *		user_work	: 描画対象マテリアルユーザーデータ保存
 */
// ==========================================================================
void gmGmkItemCreatePopUpEffect(OBS_OBJECT_WORK *parent_obj, s32 mat_id)
{
	OBS_OBJECT_WORK			*obj_work;
	GMS_EFFECT_3DNN_WORK	*efct_work;

	obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), NULL/*parent_obj*/, 0/*sort_prio*/, "GMK_ITEM_POP");
	efct_work = (GMS_EFFECT_3DNN_WORK*)obj_work;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_item_obj_3d_list[IDB_GMK_ITEM_MDL_GMK_ITEM_I_ZNO],
					&efct_work->obj_3d);

	// 座標設定
	obj_work->pos.x = parent_obj->pos.x;
	obj_work->pos.y = parent_obj->pos.y + GMD_GMK_ITEM_POPUP_EFCT_OFST_Y;

	// 優先設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B;

	// マテリアルコールバック設定
	efct_work->obj_3d.material_cb_func = gmGmkItemMaterialCallback;

	// 描画処理設定
	obj_work->ppOut = gmGmkItemOut;

	// 描画対象マテリアルユーザーデータ保存
	obj_work->user_work = mat_id;

	// フラグ設定
	obj_work->flag		|= OBD_OBJECT_NOHIT | OBD_OBJECT_NOCLIP;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag &= ~OBD_MOVE_FALL;

	// スケール調整
	obj_work->scale.x = obj_work->scale.y = GMD_GMK_ITEM_POPUP_EFCT_SCALE;

	// メイン処理設定
	obj_work->ppFunc = gmGmkItemPopUpEffectMain;

	// 初期速度
	obj_work->spd.y = GMD_GMK_ITEM_POPUP_EFCT_SPD_Y;
	obj_work->spd_add.y = GMD_GMK_ITEM_POPUP_EFCT_SPD_ADD_Y;
}

// ==========================================================================
// gmGmkItemPopUpEffectMain
/*!
 *	アイテム 取得時エフェクト メイン処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkItemPopUpEffectMain(OBS_OBJECT_WORK *obj_work)
{
	if (!obj_work->spd.y) {
		obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);
		if (obj_work->user_timer <= 0) {
			// 演出終了
			obj_work->flag |= OBD_OBJECT_TASKCLEAR;
			return;
		}
	}
	else if (obj_work->spd.y + obj_work->spd_add.y >= 0) {
		// 移動終了
		obj_work->spd.y = obj_work->spd_add.y = 0;
		obj_work->user_timer = GMD_GMK_ITEM_POPUP_EFCT_END_DISP_TIME*FX32_ONE;
		obj_work->move_flag |= OBD_MOVE_NOMOVE;
	}
}

// ==========================================================================
// gmGmkItemCreateBreakEffect
/*!
 *	アイテム 破片エフェクト生成
 *
 *	@param	parent_obj	[in]	親オブジェクト
 *	@param	efct_no		[in]	エフェクトNO (0 ～ 2)
 *	@param	ofst_x		[in]	オフセットX
 *	@param	ofst_y		[in]	オフセットY
 *
 *	@note
 *		user_timer	: dir x 速度 \n
 *		user_work	: dir y 速度 \n
 *		user_flag	: dir z 速度
 */
// ==========================================================================
#if 0
void gmGmkItemCreateBreakEffect(OBS_OBJECT_WORK *parent_obj, u32 efct_no, fx32 spd_x, fx32 spd_y, fx32 ofst_x, fx32 ofst_y)
{
	OBS_OBJECT_WORK			*obj_work;
	GMS_EFFECT_3DNN_WORK	*efct_work;
	u16						rand;

	MTM_ASSERT(efct_no <= 2);

	obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), NULL/*parent_obj*/, 0/*sort_prio*/, "GMK_ITEM_POP");
	efct_work = (GMS_EFFECT_3DNN_WORK*)obj_work;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_item_obj_3d_list[IDB_GMK_ITEM_MDL_GMK_ITEM_PA_ZNO + efct_no],
					&efct_work->obj_3d);

	// 座標設定
	obj_work->pos.x = parent_obj->pos.x + ofst_x;
	obj_work->pos.y = parent_obj->pos.y + ofst_y;

	// 優先設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_FRONT;

	// フラグ設定
	obj_work->flag		|= OBD_OBJECT_NOHIT;
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->disp_flag &= ~OBD_DISP_NODIR;
	obj_work->move_flag |= OBD_MOVE_FALL;

	// クリッピング設定
	GmObjSetClip(obj_work, 32, 0, 0, 0, 0);

	// 初期速度
	obj_work->spd.x = spd_x;
	obj_work->spd.y = spd_y;

	// メイン処理設定
	obj_work->ppFunc = gmGmkItemBreakEffectMain;

	// 回転速度設定
	rand = mtMathRand();
	switch (rand & 0x07) {
	case 0:
	case 6:
		obj_work->user_timer = 0x400 + (mtMathRand() & 0x3FF);
		break;
	case 1:
	case 7:
		obj_work->user_work = 0x400 + (mtMathRand() & 0x3FF);
		break;
	case 2:
		obj_work->user_flag = 0x400 + (mtMathRand() & 0x3FF);
		break;
	case 3:
		obj_work->user_timer = 0x400 + (mtMathRand() & 0x3FF);
		obj_work->user_work = 0x400 + (mtMathRand() & 0x3FF);
		break;
	case 4:
		obj_work->user_work = 0x400 + (mtMathRand() & 0x3FF);
		obj_work->user_flag = 0x400 + (mtMathRand() & 0x3FF);
		break;
	case 5:
		obj_work->user_flag = 0x400 + (mtMathRand() & 0x3FF);
		obj_work->user_timer = 0x400 + (mtMathRand() & 0x3FF);
		break;
	}
}

// ==========================================================================
// gmGmkItemBreakEffectMain
/*!
 *	アイテム 破片エフェクト メイン処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkItemBreakEffectMain(OBS_OBJECT_WORK *obj_work)
{
	// 回転速度設定
	obj_work->dir.x += (u16)obj_work->user_timer;
	obj_work->dir.y += (u16)obj_work->user_work;
	obj_work->dir.z += (u16)obj_work->user_flag;

	// X減速
	obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, GMD_GMK_ITEM_BREAK_EFCT_SPD_DEC_X);
}
#endif


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
