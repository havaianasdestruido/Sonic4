// ==========================================================================
/*!
  @file gmEneMotora.cpp
  @brief エネミー モトラ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEneMotora.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: 移動範囲左
 *		top			: なし
 *		width		: 移動範囲幅
 *		height		: なし
 *
 *		flag
 *			1		: 左右反転設定 ONで右向き
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"

#include "gmEneCom.h"
#include "gmEneMotora.h"

// データヘッダ
#include "common/model/ene_motora_mtn.hmb"
#include "common/model/ene_motora_mdl.hmb"


//----- Definitions ---------------------------------------------------------

// GMS_EVE_RECORD_EVENT.flag
#define GMD_ENE_MOTORA_EVE_FLAG_RIGHT	(0x0001)	//!< 右向き開始


#define GMD_ENE_MOTORA_MOVE_SPD_X		(0x0800)			//!< 移動速度
#define GMD_ENE_MOTORA_FW_TIME			(15*FX32_ONE)		//!< FW時間


#define GMD_ENE_MOTORA_TURN_FRAME		(40)	//!< ターンモーションフレーム

/// モトラワーク
typedef struct tag_GMS_ENE_MOTORA_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離

} GMS_ENE_MOTORA_WORK;
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmEneMotoraWalkInit(OBS_OBJECT_WORK *obj_work);
static void gmEneMotoraWalkMain(OBS_OBJECT_WORK *obj_work);
//static void gmEneMotoraFwInit(OBS_OBJECT_WORK *obj_work);
//static void gmEneMotoraFwMain(OBS_OBJECT_WORK *obj_work);
static void gmEneMotoraFlipInit(OBS_OBJECT_WORK *obj_work);
static void gmEneMotoraFlipMain(OBS_OBJECT_WORK *obj_work);

static BOOL gmEneMotoraSetWalkSpeed(GMS_ENE_MOTORA_WORK *motora_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_motora_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneMotoraBuild
/*!
 *	エネミー モトラ データ構築
 */
// ==========================================================================
void GmEneMotoraBuild(void)
{
	gm_ene_motora_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MOTORA_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MOTORA_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneMotoraFlush
/*!
 *	エネミー モトラ データ片付け
 */
// ==========================================================================
void GmEneMotoraFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MOTORA_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_motora_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneMotoraInit
/*!
 *	エネミー モトラ 初期化関数
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
OBS_OBJECT_WORK* GmEneMotoraInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_MOTORA_WORK	*motora_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_MOTORA_WORK), "ENE_MOTORA");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	motora_work = (GMS_ENE_MOTORA_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_motora_obj_3d_list[IDB_ENE_MOTORA_MDL_ENE_MOTORA_ZNO],
					&ene_work->obj_3d);
	//ObjObjectAction3dNNModelLoad(obj_work, &ene_work->obj_3d,
	//			NULL/*data_work*/, NULL/*model_data_path*/,
	//			IDB_ENE_MOTORA_MDL_ENE_MOTORA_ZNO/*index*/,
	//			ObjDataGet(GMD_DWORK_NO_ENEMY_MOTORA_MODEL)->pData/*archive*/,
	//			NULL/*tex_data_path*/, ObjDataGet(GMD_DWORK_NO_ENEMY_MOTORA_TEX)->pData,
	//			NND_DRAWOBJ_SHADER_USER_PROFILE_TOON);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_MOTORA_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

	// 優先設定
	obj_work->pos.z = GMD_OBJ_ENEMY_POS_Z;

	// 矩形設定
	// 対プレイヤー
	// 攻撃
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -11, -24, 11, 0);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// くらい
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, -32, 19, 0);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// ボディ
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, -32, 19, 0);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// 地形あたり
	ObjObjectFieldRectSet(obj_work, -4, -8, 4, -2);

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag |= OBD_MOVE_FALL;

	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_MOTORA_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
	motora_work->spd_dec		 = GMD_ENE_MOTORA_MOVE_SPD_X / (GMD_ENE_MOTORA_TURN_FRAME/2);
	motora_work->spd_dec_dist = GMD_ENE_MOTORA_MOVE_SPD_X * (GMD_ENE_MOTORA_TURN_FRAME/2) / 2;

	gmEneMotoraWalkInit(obj_work);

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}



//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneMotoraWalkInit
/*!
 *	エネミー モトラ Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMotoraWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MOTORA_MTN_ENE_MOTORA_MOVE_ZNM, IDB_ENE_MOTORA_MTN_ENE_MOTORA_MOVE_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_MOTORA_MTN_ENE_MOTORA_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneMotoraWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_MOTORA_MOVE_SPD_X;
#if GMD_ENEMY_TEST
		obj_work->dir.y = 0x4000;
#endif
	}
	else {
		obj_work->spd.x = GMD_ENE_MOTORA_MOVE_SPD_X;
#if GMD_ENEMY_TEST
		obj_work->dir.y = 0xc000;
#endif
	}
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	エネミー モトラ Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMotoraWalkMain(OBS_OBJECT_WORK *obj_work)
{
#if 1
	BOOL				b_dec;
	GMS_ENE_MOTORA_WORK	*motora_work;

	motora_work	= (GMS_ENE_MOTORA_WORK*)obj_work;

	b_dec = gmEneMotoraSetWalkSpeed(motora_work);

	if (b_dec) {
		// 減速を開始した

		// フリップへ移行
		gmEneMotoraFlipInit(obj_work);
	}
#else
	if (obj_work->move_flag & OBD_MOVE_FRONT ||
			!GmEneComCheckMoveLimit(obj_work, (fx32)obj_work->user_work, (fx32)obj_work->user_flag)) {
		// 前方衝突・移動範囲外

		// フリップへ移行
		gmEneMotoraFlipInit(obj_work);
		// FWへ移行
		//gmEneMotoraFwInit(obj_work);
	}
#endif
}


#if 0
// ==========================================================================
// gmEneMotoraFwInit
/*!
 *	エネミー モトラ FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMotoraFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MOTORA_MTN_ENE_MOTORA_FW_ZNM, IDB_ENE_MOTORA_MTN_ENE_MOTORA_FW_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_MOTORA_MTN_ENE_MOTORA_FW_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneMotoraFwMain;

	// 移動速度設定
	obj_work->spd.x = 0;

	// 演出時間
	obj_work->user_timer = GMD_ENE_MOTORA_FW_TIME;
}

// ==========================================================================
// gmEneMotoraFwMain
/*!
 *	エネミー モトラ FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMotoraFwMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	if (obj_work->user_timer <= 0) {
		// フリップへ移行
		gmEneMotoraFlipInit(obj_work);
	}
}
#endif



// ==========================================================================
// gmEneMotoraFlipInit
/*!
 *	エネミー モトラ フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMotoraFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MOTORA_MTN_ENE_MOTORA_FRIP_ZNM, IDB_ENE_MOTORA_MTN_ENE_MOTORA_FRIP_L_ZNM);
	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_MOTORA_MTN_ENE_MOTORA_FRIP_ZNM, IDB_ENE_MOTORA_MTN_ENE_MOTORA_FRIP_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_MOTORA_MTN_ENE_MOTORA_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneMotoraFlipMain;

	// 移動速度設定
	//obj_work->spd.x = 0;
}

// ==========================================================================
// gmEneMotoraFlipMain
/*!
 *	エネミー モトラ フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMotoraFlipMain(OBS_OBJECT_WORK *obj_work)
{
	// 移動速度設定
	gmEneMotoraSetWalkSpeed((GMS_ENE_MOTORA_WORK*)obj_work);

	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		// Walkへ
		gmEneMotoraWalkInit(obj_work);
	}
}


// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneMotoraSetWalkSpeed
/*!
 *	エネミー モトラ 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneMotoraSetWalkSpeed(GMS_ENE_MOTORA_WORK *motora_work)
{
	BOOL			b_dec = FALSE;
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)motora_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// 左向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_MOTORA_MTN_ENE_MOTORA_FRIP_L_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_MOTORA_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, motora_work->spd_dec, GMD_ENE_MOTORA_MOVE_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + motora_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, motora_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -motora_work->spd_dec) {
					obj_work->spd.x = -motora_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_MOTORA_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -motora_work->spd_dec, GMD_ENE_MOTORA_MOVE_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_MOTORA_MTN_ENE_MOTORA_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_MOTORA_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -motora_work->spd_dec, GMD_ENE_MOTORA_MOVE_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - motora_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, motora_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > motora_work->spd_dec) {
					obj_work->spd.x = motora_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_MOTORA_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, motora_work->spd_dec, GMD_ENE_MOTORA_MOVE_SPD_X);
		}
	}

	return (b_dec);
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
