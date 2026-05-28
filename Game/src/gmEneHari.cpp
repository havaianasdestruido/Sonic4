// ==========================================================================
/*!
  @file gmEneHari.cpp
  @brief エネミー ハリセンボ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEneHari.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: なし
 *		top			: なし
 *		width		: なし
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
#include "gmEffect.h"
#include "gmEffectEnemy.h"
#include "gmEffectCmn.h"
#include "gmComEfct.h"
#include "gmBossCommon.h"

#include "gmEneCom.h"
#include "gmEneHari.h"

// データヘッダ
#include "common/model/ene_hari_mtn.hmb"
#include "common/model/ene_hari_mdl.hmb"


//----- Definitions ---------------------------------------------------------
#define GMD_ENE_HARI_TEST  (GMD_ENEMY_TEST)

#define GMD_ENE_HARI_ATK_BLINK_ENABLE		(0)		//!< 攻撃時点滅演出

/* ハリセンボ 共通 */
// GMS_EVE_RECORD_EVENT.flag
#define GMD_ENE_HARI_EVE_FLAG_RIGHT	(0x0001)	//!< 右向き開始

#define GMD_ENE_HARI_ATK_RECT_LEFT		(-12)	//!< 通常時攻撃矩形
#define GMD_ENE_HARI_ATK_RECT_TOP		(-12)
#define GMD_ENE_HARI_ATK_RECT_RIGHT		(12)
#define GMD_ENE_HARI_ATK_RECT_BOTTOM	(12)
#define GMD_ENE_HARI_DEF_RECT_LEFT		(-22)	//!< 防御矩形
#define GMD_ENE_HARI_DEF_RECT_TOP		(-22)
#define GMD_ENE_HARI_DEF_RECT_RIGHT		(22)
#define GMD_ENE_HARI_DEF_RECT_BOTTOM	(22)
#define GMD_ENE_HARI_BODY_RECT_LEFT		(-22)	//!< ボディ矩形
#define GMD_ENE_HARI_BODY_RECT_TOP		(-22)
#define GMD_ENE_HARI_BODY_RECT_RIGHT	(22)
#define GMD_ENE_HARI_BODY_RECT_BOTTOM	(22)

/* ハリセンボ 赤(強) */
// GMS_EVE_RECORD_EVENT.width
// 攻撃待機時間 (*0.5 秒) 0 で GMD_ENE_HARI_DEF_WAIT_TIME
// GMS_EVE_RECORD_EVENT.height
// 攻撃時間 (*0.5 秒) 0 で GMD_ENE_HARI_DEF_ATK_TIME

#define GMD_ENE_HARI_R_ATK_ALERT_TIME	(60)		//!< 警告時間 

#define GMD_ENE_HARI_R_DEF_ATKWAIT_TIME	(5*60)		//!< 標準攻撃待機時間
#define GMD_ENE_HARI_R_DEF_ATK_TIME		(5*60)		//!< 標準攻撃時間


#define GMD_ENE_HARI_R_ATK_RECT_LEFT		(-24)		//!< 攻撃時の攻撃矩形
#define GMD_ENE_HARI_R_ATK_RECT_TOP			(-24)
#define GMD_ENE_HARI_R_ATK_RECT_RIGHT		(24)
#define GMD_ENE_HARI_R_ATK_RECT_BOTTOM		(24)

#if GMD_ENE_HARI_ATK_BLINK_ENABLE
/// フェードアニメーションパターンデータ
typedef struct tag_GMS_ENE_HARI_FADE_ANIME_PAT {
	NNS_RGB		col;
	float		intensity;
	fx32		frame;
} GMS_ENE_HARI_FADE_ANIME_PAT;
/// フェードアニメーションデータ
typedef struct tag_GMS_ENE_HARI_FADE_ANIME {
	u32								pat_num;	//!< パターン数
	GMS_ENE_HARI_FADE_ANIME_PAT		*anime_pat;	//!< アニメーションパターンデータ
} GMS_ENE_HARI_FADE_ANIME;
#endif // #if GMD_ENE_HARI_ATK_BLINK_ENABLE


/// ハリセンボワーク
typedef struct tag_GMS_ENE_HARI_WORK {
	GMS_ENEMY_3D_WORK	ene_3d;

	GMS_EFFECT_3DES_WORK	*efct_jet;			//!< バーニア

	NNS_MATRIX				jet_mtx;

#if GMD_ENE_HARI_ATK_BLINK_ENABLE
	// ハリセンボ強アニメーション
	GMS_ENE_HARI_FADE_ANIME	*anime_data;
	u32		anime_pat_no;
	fx32	anime_frame;
#endif // #if GMD_ENE_HARI_ATK_BLINK_ENABLE

} GMS_ENE_HARI_WORK;
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmEneHarisenboFwInit(OBS_OBJECT_WORK *obj_work);
static void gmEneHarisenboFwMain(OBS_OBJECT_WORK *obj_work);

static void gmEneHarisenboRedAtkWaitInit(OBS_OBJECT_WORK *obj_work);
static void gmEneHarisenboRedAtkWaitMain(OBS_OBJECT_WORK *obj_work);
static void gmEneHarisenboRedAtkInit(OBS_OBJECT_WORK *obj_work);
static void gmEneHarisenboRedAtkMain(OBS_OBJECT_WORK *obj_work);

static void gmEneHariMotionCallback(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param);

#if GMD_ENE_HARI_ATK_BLINK_ENABLE
static void gmEneHariFadeAnimeSet(GMS_ENE_HARI_WORK *hari_work, GMS_ENE_HARI_FADE_ANIME *anime_data);
static void gmEneHariFadeAnimeUpdate(GMS_ENE_HARI_WORK *hari_work, fx32 speed, BOOL repeat);
#endif // #if GMD_ENE_HARI_ATK_BLINK_ENABLE

static void gmEneHariCreateJetEfct(GMS_ENE_HARI_WORK *hari_work);
static void gmEneHariJetEfctMain(OBS_OBJECT_WORK *obj_work);
//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_harisenbo_obj_3d_list = NULL;
static OBS_ACTION3D_NN_WORK *gm_ene_harisenbo_r_obj_3d_list = NULL;

#if GMD_ENE_HARI_ATK_BLINK_ENABLE
/// 強タイプ 攻撃前アニメーション
static GMS_ENE_HARI_FADE_ANIME_PAT gm_ene_harisenbo_r_atk_blink_anime_pat[] = {
#if 0
	{1.f, 1.f, 1.f, 0.5, 8*FX32_ONE},
	{1.f, 0.f, 0.f, 0.5, 8*FX32_ONE},
#else
	{1.f, 1.f*(1.f*4.f/4.f), 1.f*(1.f*4.f/4.f), 0.7, 1*FX32_ONE},
	{1.f, 1.f*(1.f*3.f/4.f), 1.f*(1.f*3.f/4.f), 0.7, 1*FX32_ONE},
	{1.f, 1.f*(1.f*2.f/4.f), 1.f*(1.f*2.f/4.f), 0.7, 1*FX32_ONE},
	{1.f, 1.f*(1.f*1.f/4.f), 1.f*(1.f*1.f/4.f), 0.7, 1*FX32_ONE},

	{1.f, 1.f*(1.f*0.f/4.f), 1.f*(1.f*0.f/4.f), 0.7, 1*FX32_ONE},
	{1.f, 1.f*(1.f*1.f/4.f), 1.f*(1.f*1.f/4.f), 0.7, 1*FX32_ONE},
	{1.f, 1.f*(1.f*2.f/4.f), 1.f*(1.f*2.f/4.f), 0.7, 1*FX32_ONE},
	{1.f, 1.f*(1.f*3.f/4.f), 1.f*(1.f*3.f/4.f), 0.7, 1*FX32_ONE},
#endif
};
static GMS_ENE_HARI_FADE_ANIME gm_ene_harisenbo_r_atk_blink_anime = {
	sizeof(gm_ene_harisenbo_r_atk_blink_anime_pat) / sizeof(GMS_ENE_HARI_FADE_ANIME_PAT),
	gm_ene_harisenbo_r_atk_blink_anime_pat,
};
#endif // #if GMD_ENE_HARI_ATK_BLINK_ENABLE

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneHariSenboBuild
/*!
 *	エネミー ハリセンボ データ構築
 */
// ==========================================================================
void GmEneHariSenboBuild(void)
{
	AMS_AMB_HEADER	*amb;
	void			*txb;

	// 通常
	gm_ene_harisenbo_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_HARISENBO_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_HARISENBO_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
#if !GMD_ENE_HARI_TEST
	// 強
	amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_HARISENBO_TEX);
	amBindConv((u8*)amb);		// 中のデータはコンバートしない

	txb = amBindGet(amb, amb->file_num - 1/*一番最後がTXB*/);

	gm_ene_harisenbo_r_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_HARISENBO_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_HARISENBO_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/,
								txb);
#endif // !GMD_ENE_HARI_TEST
}

// ==========================================================================
// GmEneHariSenboFlush
/*!
 *	エネミー ハリセンボ データ片付け
 */
// ==========================================================================
void GmEneHariSenboFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_HARISENBO_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_harisenbo_obj_3d_list, amb->file_num);
#if !GMD_ENE_HARI_TEST
	GmGameDBuildRegFlushModel(gm_ene_harisenbo_r_obj_3d_list, amb->file_num);
#endif // !GMD_ENE_HARI_TEST
}

// ==========================================================================
// GmEneHariSenboInit
/*!
 *	エネミー ハリセンボ 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *		赤タイプ
 *		user_work	: 攻撃待機時間
 *		user_flag	: 攻撃時間
 */
// ==========================================================================
OBS_OBJECT_WORK* GmEneHariSenboInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_HARI_WORK), "ENE_HARI");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;


	// モデル初期化
	if (eve_rec->id == GMD_EVENT_ID_ENE_HARISENBO) {
		// 通常
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_ene_harisenbo_obj_3d_list[IDB_ENE_HARI_MDL_ENE_HARI_ZNO],
						&ene_work->obj_3d);
	}
	else {
		// 強
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_ene_harisenbo_r_obj_3d_list[IDB_ENE_HARI_MDL_ENE_HARI_ZNO],
						&ene_work->obj_3d);
	}
#if !GMD_ENE_HARI_TEST
	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_HARISENBO_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

	// アクション先行設定
	ObjDrawObjectActionSet(obj_work, IDB_ENE_HARI_MTN_ENE_HARI_MOVE_ZNM);
#endif // !GMD_ENE_HARI_TEST
	// 優先設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;

	// モーションコールバック設定
	ene_work->obj_3d.mtn_cb_func = gmEneHariMotionCallback;
	ene_work->obj_3d.mtn_cb_param= obj_work;

	// 矩形設定
	// 対プレイヤー
	// 攻撃
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, GMD_ENE_HARI_ATK_RECT_LEFT, GMD_ENE_HARI_ATK_RECT_TOP,
							GMD_ENE_HARI_ATK_RECT_RIGHT, GMD_ENE_HARI_ATK_RECT_BOTTOM);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// くらい
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, GMD_ENE_HARI_DEF_RECT_LEFT, GMD_ENE_HARI_DEF_RECT_TOP,
					GMD_ENE_HARI_DEF_RECT_RIGHT, GMD_ENE_HARI_DEF_RECT_BOTTOM);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// BODY
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work, GMD_ENE_HARI_BODY_RECT_LEFT, GMD_ENE_HARI_BODY_RECT_TOP,
						GMD_ENE_HARI_BODY_RECT_RIGHT, GMD_ENE_HARI_BODY_RECT_BOTTOM);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// 地形あたり
	ObjObjectFieldRectSet(obj_work, -4, -8, 4, 0);

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
#if !GMD_ENE_HARI_TEST
	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_HARI_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}
#endif // !GMD_ENE_HARI_TEST
	if (eve_rec->id == GMD_EVENT_ID_ENE_HARISENBO) {
		// 通常
		gmEneHarisenboFwInit(obj_work);
#if OBD_OBJECT_USE_NOEXIST
		obj_work->flag |= OBD_OBJECT_NOEXIST_ENABLE;
#endif // OBD_OBJECT_USE_NOEXIST
	}
	else {
		// 強
		// 設定取得
		// 攻撃待機時間
		obj_work->user_work = (u32)(eve_rec->width * 30 * FX32_ONE);
		if (obj_work->user_work == 0) {
			obj_work->user_work = GMD_ENE_HARI_R_DEF_ATKWAIT_TIME * FX32_ONE;
		}
		// 攻撃時間
		obj_work->user_flag = (u32)(eve_rec->height * 30 * FX32_ONE);
		if (obj_work->user_flag == 0) {
			obj_work->user_flag = GMD_ENE_HARI_R_DEF_ATK_TIME * FX32_ONE;
		}

		gmEneHarisenboRedAtkWaitInit(obj_work);
	}

	// バーニア生成
	gmEneHariCreateJetEfct((GMS_ENE_HARI_WORK*)obj_work);

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}



//----- Local Functions -----------------------------------------------------
// ==========================================================================
// ハリセンボ
// ==========================================================================
// ==========================================================================
// gmEneHarisenboFwInit
/*!
 *	エネミー ハリセンボ FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneHarisenboFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
#if !GMD_ENE_HARI_TEST
	ObjDrawObjectActionSet(obj_work, IDB_ENE_HARI_MTN_ENE_HARI_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;
#else // !GMD_ENE_HARI_TEST
	obj_work->scale.x = 2 * FX32_ONE;
	obj_work->scale.y = 2 * FX32_ONE;
	obj_work->scale.z = 2 * FX32_ONE;
	obj_work->dir.y = 0xc000;
#endif // !GMD_ENE_HARI_TEST

	// メイン処理
	obj_work->ppFunc = gmEneHarisenboFwMain;

	// 移動速度設定
	//obj_work->spd.x = 0;

	// 演出時間
	//obj_work->user_timer = GMD_ENE_MOTORA_FW_TIME;
}

// ==========================================================================
// gmEneHarisenboFwMain
/*!
 *	エネミー ハリセンボ FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneHarisenboFwMain(OBS_OBJECT_WORK *obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);
}


// ==========================================================================
// ハリセンボ赤(強)
// ==========================================================================
// ==========================================================================
// gmEneHarisenboRedAtkWaitInit
/*!
 *	エネミー ハリセンボ 赤 攻撃待機初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_work	: 攻撃待機時間
 *		user_flag	: 攻撃時間
 */
// ==========================================================================
void gmEneHarisenboRedAtkWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_ENE_HARI_MTN_ENE_HARI_MOVE_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_HARI_MTN_ENE_HARI_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// タイマクリア
	obj_work->user_timer = 0;

	// メイン処理
	obj_work->ppFunc = gmEneHarisenboRedAtkWaitMain;
}

// ==========================================================================
// gmEneHarisenboRedAtkWaitInit
/*!
 *	エネミー ハリセンボ 攻撃待機 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_work	: 攻撃待機時間
 *		user_flag	: 攻撃時間
 */
// ==========================================================================
void gmEneHarisenboRedAtkWaitMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountUp(obj_work->user_timer);

	if ((u32)obj_work->user_timer >= obj_work->user_work) {
		// 攻撃開始
		gmEneHarisenboRedAtkInit(obj_work);
	}
}

// ==========================================================================
// gmEneHarisenboRedAtkInit
/*!
 *	エネミー ハリセンボ 赤 攻撃初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_work	: 攻撃待機時間
 *		user_flag	: 攻撃時間
 */
// ==========================================================================
void gmEneHarisenboRedAtkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	GMS_ENE_HARI_WORK	*hari_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	hari_work= (GMS_ENE_HARI_WORK*)obj_work;

	// アクション設定
	ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_ENE_HARI_MTN_ENE_HARI_ATK_01_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// タイマクリア
	obj_work->user_timer = GMD_ENE_HARI_R_ATK_ALERT_TIME * FX32_ONE;

	// メイン処理
	obj_work->ppFunc = gmEneHarisenboRedAtkMain;

#if GMD_ENE_HARI_ATK_BLINK_ENABLE
	// フェードクリア
	GmBsCmnClearObject3DNNFadedColor(obj_work);
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;
	// フェードアニメーション用意
	gmEneHariFadeAnimeSet(hari_work, &gm_ene_harisenbo_r_atk_blink_anime);
#endif // #if GMD_ENE_HARI_ATK_BLINK_ENABLE
}

// ==========================================================================
// gmEneHarisenboRedAtkMain
/*!
 *	エネミー ハリセンボ 攻撃 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_work	: 攻撃待機時間
 *		user_flag	: 攻撃時間
 */
// ==========================================================================
void gmEneHarisenboRedAtkMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_RECT_WORK		*rect_work;

	if (obj_work->obj_3d->act_id[0] == IDB_ENE_HARI_MTN_ENE_HARI_ATK_01_ZNM) {
		// 警告中
		obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

		if (!obj_work->user_timer) {
			// 攻撃開始
#if GMD_ENE_HARI_ATK_BLINK_ENABLE
			// フェードアニメーションクリア
			GmBsCmnClearObject3DNNFadedColor(obj_work);
			obj_work->disp_flag &= ~OBD_DISP_DRAWSTATE;
#endif // #if GMD_ENE_HARI_ATK_BLINK_ENABLE

			// 先に矩形パワーを変更
			// 防御矩形調整
			rect_work = &((GMS_ENEMY_3D_WORK*)obj_work)->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
			rect_work->def_power = GMD_OBJ_RECT_DEF_POWER_GUARD;

			// アクション設定
			ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_ENE_HARI_MTN_ENE_HARI_ATK_02_ZNM);
		}
#if GMD_ENE_HARI_ATK_BLINK_ENABLE
		else {
			// フェードアニメーション更新
			gmEneHariFadeAnimeUpdate((GMS_ENE_HARI_WORK*)obj_work, FX32_ONE, TRUE);
		}
#endif // #if GMD_ENE_HARI_ATK_BLINK_ENABLE
	}
	else if (obj_work->obj_3d->act_id[0] == IDB_ENE_HARI_MTN_ENE_HARI_ATK_02_ZNM) {
		if (obj_work->disp_flag & OBD_DISP_END) {

			// 攻撃矩形調整
			rect_work = &((GMS_ENEMY_3D_WORK*)obj_work)->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
			// 矩形
			ObjRectWorkSet(rect_work, GMD_ENE_HARI_R_ATK_RECT_LEFT, GMD_ENE_HARI_R_ATK_RECT_TOP,
									GMD_ENE_HARI_R_ATK_RECT_RIGHT, GMD_ENE_HARI_R_ATK_RECT_BOTTOM);
			rect_work->flag |= OBD_RECT_ENABLE;

			// 防御矩形調整
			//rect_work = &((GMS_ENEMY_3D_WORK*)obj_work)->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
			//rect_work->def_power = GMD_OBJ_RECT_DEF_POWER_GUARD;

			// アクション設定
			ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_ENE_HARI_MTN_ENE_HARI_N_MOVE_ZNM);
			obj_work->disp_flag |= OBD_DISP_REPEAT;

			// タイマクリア
			obj_work->user_timer = 0;
		}
	}
	else {
		// 攻撃中
		obj_work->user_timer = ObjTimeCountUp(obj_work->user_timer);

		if ((u32)obj_work->user_timer >= obj_work->user_flag) {
			// 攻撃終了

			// 矩形復帰
			// 攻撃矩形調整
			rect_work = &((GMS_ENEMY_3D_WORK*)obj_work)->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
			// 矩形
			ObjRectWorkSet(rect_work, GMD_ENE_HARI_ATK_RECT_LEFT, GMD_ENE_HARI_ATK_RECT_TOP,
									GMD_ENE_HARI_ATK_RECT_RIGHT, GMD_ENE_HARI_ATK_RECT_BOTTOM);
			rect_work->flag |= OBD_RECT_ENABLE;

			// 防御矩形調整
			rect_work = &((GMS_ENEMY_3D_WORK*)obj_work)->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
			rect_work->def_power = GMD_OBJ_RECT_DEF_POWER_DEFAULT;

			// 攻撃待機へ
			gmEneHarisenboRedAtkWaitInit(obj_work);
		}
	}
}


// ==========================================================================
// モーションコールバック
// ==========================================================================
// ==========================================================================
// gmEneHariMotionCallback
/*!
 *	エネミー ハリセンボ モーションコールバック
 *
 *	@param motion	[in] モーション
 *	@param object	[in] オブジェクト
 *	@param param	[in] パラメータ
 */
// ==========================================================================
#define GMD_ENE_STING_NODE_ID_JET		(7)
void gmEneHariMotionCallback(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param)
{
	/* Node 7 : jet */
	NNS_MATRIX			node_mtx, base_mtx;
	GMS_ENE_HARI_WORK	*hari_work = (GMS_ENE_HARI_WORK*)param;

	// ベースマトリクス取得
	nnMakeUnitMatrix(&base_mtx);
	nnMultiplyMatrix(&base_mtx, &base_mtx, amMatrixGetCurrent());
	
	// 階層マトリクスを求める
	// jet
	nnCalcNodeMatrixTRSList(&node_mtx, object, 
			GMD_ENE_STING_NODE_ID_JET, 
			motion->data, &base_mtx);
	MI_CpuCopy8(&node_mtx, &hari_work->jet_mtx, sizeof(NNS_MATRIX));
}


#if GMD_ENE_HARI_ATK_BLINK_ENABLE
// ==========================================================================
// フェードアニメーション
// ==========================================================================
// ==========================================================================
// gmEneHariFadeAnimeSet
/*!
 *	フェードアニメーション設定
 *
 *	@param	hari_work	[in]	ハリセンボワーク
 *	@param	anime_data	[in]	アニメーションデータ
 */
// ==========================================================================
void gmEneHariFadeAnimeSet(GMS_ENE_HARI_WORK *hari_work, GMS_ENE_HARI_FADE_ANIME *anime_data)
{
	// フェードアニメーション用意
	hari_work->anime_data	= anime_data;
	hari_work->anime_pat_no	= 0;
	hari_work->anime_frame	= 0;
}

// ==========================================================================
// gmEneHariFadeAnimeUpdate
/*!
 *	フェードアニメーション更新
 *
 *	@param	hari_work	[in]	ハリセンボワーク
 *	@param	speed		[in]	アニメーション速度
 *	@param	repeat		[in]	リピート設定
 */
// ==========================================================================
void gmEneHariFadeAnimeUpdate(GMS_ENE_HARI_WORK *hari_work, fx32 speed, BOOL repeat)
{
	GMS_ENE_HARI_FADE_ANIME		*anime_data;
	GMS_ENE_HARI_FADE_ANIME_PAT	*pat_data;

	anime_data	= hari_work->anime_data;
	pat_data	= &anime_data->anime_pat[hari_work->anime_pat_no];

	hari_work->anime_frame += speed;
	// パターン繰り
	while (hari_work->anime_frame >= pat_data->frame) {
		hari_work->anime_frame -= pat_data->frame;

		hari_work->anime_pat_no++;
		if (hari_work->anime_pat_no < anime_data->pat_num) {
			// 次のパターン
			pat_data = &anime_data->anime_pat[hari_work->anime_pat_no];
		}
		else {
			if (repeat) {
				// リピート
				hari_work->anime_pat_no = 0;
				pat_data = &anime_data->anime_pat[hari_work->anime_pat_no];
			}
			else {
				// 終了
				hari_work->anime_pat_no = anime_data->pat_num - 1;
				hari_work->anime_frame = pat_data->frame - 1;
			}
		}
	}

	// カラーセット
	GmBsCmnSetObject3DNNFadedColor((OBS_OBJECT_WORK*)hari_work,
									   &pat_data->col,
									   pat_data->intensity);
}
#endif // #if GMD_ENE_HARI_ATK_BLINK_ENABLE


// ==========================================================================
// エフェクト
// ==========================================================================
// ==========================================================================
// gmEneHariCreateJetEfct
/*!
 *	エネミー ハリセンボ ジェットエフェクト生成
 *
 *	@param sting_work	[in] スティンガーワーク
 */
// ==========================================================================
void gmEneHariCreateJetEfct(GMS_ENE_HARI_WORK *hari_work)
{
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// バーニアエフェクト生成
	if (!hari_work->efct_jet) {
		hari_work->efct_jet = GmEfctEneEsCreate((OBS_OBJECT_WORK*)hari_work, GME_EFCT_ENE_IDX_E14_JET_H);
		//GmComEfctAddDispOffsetF(hari_work->efct_jet, -11.f, -9.f, 0);
		hari_work->efct_jet->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_FIX_NODISP | OBD_OBJECT_NOCLIP;
		hari_work->efct_jet->efct_com.obj_work.user_work = (u32)&hari_work->jet_mtx;
		// メイン処理差し替え
		hari_work->efct_jet->efct_com.obj_work.ppFunc = gmEneHariJetEfctMain;
		// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
}

#if 0
// ==========================================================================
// gmEneStingClearJetEfct
/*!
 *	エネミー スティンガー ジェットエフェクト破棄
 *
 *	@param sting_work	[in] スティンガーワーク
 */
// ==========================================================================
void gmEneStingClearJetEfct(GMS_ENE_STING_WORK *sting_work)
{
	// バーニアエフェクト破棄
	if (sting_work->efct_r_jet) {
		ObjDrawKillAction3DES(&sting_work->efct_r_jet->efct_com.obj_work);
		//sting_work->efct_jet->efct_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		sting_work->efct_r_jet = NULL;
	}
	if (sting_work->efct_l_jet) {
		ObjDrawKillAction3DES(&sting_work->efct_l_jet->efct_com.obj_work);
		//sting_work->efct_jet->efct_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		sting_work->efct_l_jet = NULL;
	}

#if GMD_ENE_STING_JET_S_SMOKE_ON
	if (sting_work->efct_smoke) {
		ObjDrawKillAction3DES(&sting_work->efct_smoke->efct_com.obj_work);
		//sting_work->efct_smoke->efct_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		sting_work->efct_smoke = NULL;
	}
#endif
}
#endif

// ==========================================================================
// gmEneHariJetEfctMain
/*!
 *	ハリセンボジェットエフェクト
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_work	: NNS_MATRIX*
 *		動くようになったら描画処理で位置を設定するように変更
 */
// ==========================================================================
void gmEneHariJetEfctMain(OBS_OBJECT_WORK *obj_work)
{
	NNS_MATRIX		*mtx = (NNS_MATRIX*)obj_work->user_work;
	NNS_VECTOR		vec;

	MTM_ASSERT(mtx);

	if (obj_work->parent_obj == NULL) {
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

	vec.x = NNM_MTX(*mtx, 0, 3) - FXM_FX32_TO_FLOAT(obj_work->parent_obj->pos.x);
	vec.y = -NNM_MTX(*mtx, 1, 3) - FXM_FX32_TO_FLOAT(obj_work->parent_obj->pos.y);
	vec.z = NNM_MTX(*mtx, 2, 3) - FXM_FX32_TO_FLOAT(obj_work->parent_obj->pos.z);


	if (obj_work->parent_obj->disp_flag & OBD_DISP_HFLIP) {
		vec.x = -vec.x;
		vec.z = -vec.z;
	}

	vec.y += 5.f;

	GmComEfctSetDispOffsetF((GMS_EFFECT_3DES_WORK*)obj_work,
				vec.x, vec.y, vec.z);

	// 汎用処理
	//GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
	GmEffectDefaultMainFuncDeleteAtEnd(obj_work);	// Zはこちらで設定する
}




// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
