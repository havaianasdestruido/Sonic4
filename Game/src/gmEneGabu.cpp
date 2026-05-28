// ==========================================================================
/*!
  @file gmEneGabu.cpp
  @brief エネミー ガブッチョ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEneGabu.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: なし
 *		top			: 移動上限 *2で使用
 *		width		: なし
 *		height		: なし
 *
 *		flag
 *			1		: 移動速度設定
 *			2		: 移動速度設定
 *			3		: 移動速度設定
 *			8		: 落下終了時に少しウェイトを入れる
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
#include "gmEneGabu.h"

// データヘッダ
#include "common/model/ENE_GABU_MTN.HMB"
#include "common/model/ENE_GABU_MDL.HMB"


//----- Definitions ---------------------------------------------------------
#define GMD_ENE_GABU_TEST (GMD_ENEMY_TEST)
#if 0
typedef struct tag_GMS_ENE_GABU_WORK {
	GMS_ENEMY_3D_WORK	ene_3d_work;

	fx32				top;			//!< 移動上限
	fx32				bottom;			//!< 移動下限

	fx32				spd_y;			//!< ジャンプ速度
	fx32				spd_add_y;		//!< 減速度(fall設定？)
} GMS_ENE_GABU_WORK;
#endif


// GMS_EVE_RECORD_EVENT.flag
//#define GMD_ENE_GABU_EVE_FLAG_RIGHT	(0x0001)	//!< 右向き開始
#define GMD_ENE_GABU_EVE_FLAG_SPD_MASK			(0x0007)	//!< 速度設定用フラグ
#define GMD_ENE_GABU_EVE_FLAG_JUMP_WAIT_FLAG	(0x0080)	//!< ジャンプ前待機処理ありフラグ

// GMS_EVE_RECORD_EVENT.top
// GMS_EVE_RECORD_EVENT.height
// ジャンプ距離
// 但し、下限はイベント設置位置

#define GMD_ENE_GABU_JUMP_DIST_DEF		(64*1.5)	//!< 標準ジャンプ距離

#define GMD_ENE_GABU_JUMP_SPD_DEF_Y		(-0x6000)	//!< 基本ジャンプ速度
#define GMD_ENE_GABU_JUMP_SPD_ADD_Y		(-0x0400)	//!< 加算ジャンプ速度

#define GMD_ENE_GABU_JUMP_WAIT_TIME		(15)		//!< 落下後のジャンプ前ウェイト

#define GMD_ENE_GABU_ATK_MTN_FRAME		(40)		//!< 攻撃モーションフレーム

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmEneGabuJumpInit(OBS_OBJECT_WORK *obj_work);
static void gmEneGabuJumpMain(OBS_OBJECT_WORK *obj_work);
static void gmEneGabuJumpWaitInit(OBS_OBJECT_WORK *obj_work);
static void gmEneGabuJumpWaitMain(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_gabu_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneGabuBuild
/*!
 *	エネミー ガブッチョ データ構築
 */
// ==========================================================================
void GmEneGabuBuild(void)
{
	gm_ene_gabu_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_GABU_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_GABU_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneGabuFlush
/*!
 *	エネミー ガブッチョ データ片付け
 */
// ==========================================================================
void GmEneGabuFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_GABU_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_gabu_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneGabuInit
/*!
 *	エネミー ガブッチョ 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *		user_timer	: ジャンプ速度 \n
 *		user_work	: 移動下限
 */
// ==========================================================================
OBS_OBJECT_WORK* GmEneGabuInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;

	fx32				dist, time;
	s32					spd_no;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "ENE_GABU");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_gabu_obj_3d_list[IDB_ENE_GABU_MDL_ENE_GABU_ZNO],
					&ene_work->obj_3d);
#if !GMD_ENE_GABU_TEST
	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_GABU_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);
#endif // !GMD_ENE_GABU_TEST
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
	ObjRectWorkSet(rect_work, -11, -16, 11, 16);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// くらい
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, -24, 19, 24);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// BODY◆
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, -24, 19, 24);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// 地形あたり
	//ObjObjectFieldRectSet(obj_work, -4, -8, 4, 0);

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_FALL;	// 地形あたり無し 落下あり
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// 方向設定
//	if (!(eve_rec->flag & GMD_ENE_GABU_EVE_FLAG_RIGHT)) {
//		obj_work->disp_flag |= OBD_DISP_HFLIP;
//	}

	/* 上方向クリッピング範囲拡張 */
	obj_work->view_out_ofst_plus[OBD_TOP] = -256;

	/* 移動設定 */
	// 距離取得
	dist = ((fx32)-eve_rec->top << (FX32_SHIFT + 1/* *2で使用 */));
	if (dist <= 0) {
		dist = (fx32)(GMD_ENE_GABU_JUMP_DIST_DEF * FX32_ONE);
	}

	// 速度取得
	spd_no = eve_rec->flag & GMD_ENE_GABU_EVE_FLAG_SPD_MASK;
	if (spd_no <= 3) {
		// 速く
		obj_work->user_timer = GMD_ENE_GABU_JUMP_SPD_DEF_Y +
							GMD_ENE_GABU_JUMP_SPD_ADD_Y * spd_no;
	}
	else {
		// 遅く
		obj_work->user_timer = GMD_ENE_GABU_JUMP_SPD_DEF_Y -
							GMD_ENE_GABU_JUMP_SPD_ADD_Y * (spd_no - 3);
	}

	// 重力値取得
	time = FX_Div(dist*2, -obj_work->user_timer);				// 簡易計算
	obj_work->spd_fall = FX_Div(-obj_work->user_timer, time);

	// 移動下限(出現位置)保存
	obj_work->user_work = (u32)obj_work->pos.y;

	// アクション初期設定
	ObjDrawObjectActionSet(obj_work, IDB_ENE_GABU_MTN_ENE_GABU_UP_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// ジャンプ初期化
	gmEneGabuJumpInit(obj_work);
	
#if GMD_ENE_GABU_TEST
	obj_work->dir.y = 0xc000;
#endif

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}



//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneGabuJumpInit
/*!
 *	エネミー ガブッチョ Jump初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGabuJumpInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
#if !GMD_ENE_GABU_TEST
	// アクション設定
	if (obj_work->obj_3d->act_id[0] != IDB_ENE_GABU_MTN_ENE_GABU_UP_ZNM) {
		ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_ENE_GABU_MTN_ENE_GABU_UP_ZNM);
		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}
#endif
	// メイン処理
	obj_work->ppFunc = gmEneGabuJumpMain;

	//obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	obj_work->spd.y = obj_work->user_timer;
}

// ==========================================================================
// gmEneGabuJumpMain
/*!
 *	エネミー ガブッチョ ジャンプ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGabuJumpMain(OBS_OBJECT_WORK *obj_work)
{
#if 1
#if !GMD_ENE_GABU_TEST
	// 攻撃アクション切り替えチェック
	if (obj_work->obj_3d->act_id[0] != IDB_ENE_GABU_MTN_ENE_GABU_ATK_ZNM &&
			obj_work->spd.y < 0 &&
			((-obj_work->spd.y / obj_work->spd_fall) <= GMD_ENE_GABU_ATK_MTN_FRAME/2)) {
		ObjDrawObjectActionSet(obj_work, IDB_ENE_GABU_MTN_ENE_GABU_ATK_ZNM);
	}

	// 落下アクション切り替えチェック
	if (obj_work->obj_3d->act_id[0] == IDB_ENE_GABU_MTN_ENE_GABU_ATK_ZNM &&
				(obj_work->disp_flag & OBD_DISP_END)) {
		// アクション設定
		ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_ENE_GABU_MTN_ENE_GABU_DOWN_ZNM);
		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}
#endif // !GMD_ENE_GABU_TEST
	if (obj_work->pos.y >= (fx32)obj_work->user_work) {
		// ジャンプ終了
		if (((GMS_ENEMY_COM_WORK*)obj_work)->eve_rec->flag & GMD_ENE_GABU_EVE_FLAG_JUMP_WAIT_FLAG) {
			// ジャンプ待機
			gmEneGabuJumpWaitInit(obj_work);
		}
		else {
			// 再ジャンプ
			gmEneGabuJumpInit(obj_work);
		}
	}
#else
	// 落下アクション切り替えチェック
	if (obj_work->obj_3d->act_id[0] != IDB_ENE_GABU_MTN_ENE_GABU_DOWN_ZNM &&
			obj_work->spd.y >= 0) {
		// アクション設定
		ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_ENE_GABU_MTN_ENE_GABU_DOWN_ZNM);
		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}

	if (obj_work->pos.y >= (fx32)obj_work->user_work) {
		// ジャンプ終了
		if (((GMS_ENEMY_COM_WORK*)obj_work)->eve_rec->flag & GMD_ENE_GABU_EVE_FLAG_JUMP_WAIT_FLAG) {
			// ジャンプ待機
			gmEneGabuJumpWaitInit(obj_work);
		}
		else {
			// 再ジャンプ
			gmEneGabuJumpInit(obj_work);
		}
	}
#endif
}


// ==========================================================================
// gmEneGabuJumpWaitInit
/*!
 *	エネミー ガブッチョ Jump待機初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_flag : 演出タイマー
 */
// ==========================================================================
void gmEneGabuJumpWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
#if !GMD_ENE_GABU_TEST
	// アクション設定
	ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_ENE_GABU_MTN_ENE_GABU_UP_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;
#endif
	// 待機時間設定
	obj_work->user_flag = GMD_ENE_GABU_JUMP_WAIT_TIME * FX32_ONE;

	// メイン処理
	obj_work->ppFunc = gmEneGabuJumpWaitMain;

	//obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	obj_work->spd.y = 0;//obj_work->user_timer;
	obj_work->move_flag &= ~OBD_MOVE_FALL;
}

// ==========================================================================
// gmEneGabuJumpWaitMain
/*!
 *	エネミー ガブッチョ ジャンプ待機 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_flag : 演出タイマー
 */
// ==========================================================================
void gmEneGabuJumpWaitMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_flag = (u32)ObjTimeCountDown((fx32)obj_work->user_flag);

	if (!obj_work->user_flag) {
		// 待機終了
		obj_work->move_flag |= OBD_MOVE_FALL;	// 落下再開

		// ジャンプ
		gmEneGabuJumpInit(obj_work);
	}
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
