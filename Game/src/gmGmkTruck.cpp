// ==========================================================================
/*!
  @file gmGmkTruck.cpp
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGmkTruck.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmPlySeqGmk.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmComEfct.h"
#include "gmEffectZone.h"
#include "gmSound.h"

#include "gmGmkTruck.h"

// データヘッダ
#include "common/model/GMK_TRUCK_MDL.HMB"
#include "common/model/GMK_TRUCK_MTN.HMB"

//----- Definitions ---------------------------------------------------------
#define GMD_GMK_TRUCK_SPARK_EFCT_SMALL_MIN_SPD	(0x1000)	//!< 火花小エフェクト 最小速度
#define GMD_GMK_TRUCK_SPARK_EFCT_BIG_MIN_SPD	(0x8000)	//!< 火花大エフェクト 最大速度

#define GMD_GMK_TRUCK_SE_MIN_SPD				(0x0080)	//!< トロッコSE 最小速度
#define GMD_GMK_TRUCK_SE_MAX_SPD				(0x9000)	//!< トロッコSE 最大速度

#define GMD_GMK_TRUCK_SE_GOAL_MIN_DIST			(300.f)
#define GMD_GMK_TRUCK_SE_GOAL_MAX_DIST			(1500.f)


typedef struct tag_GMS_GMK_TRUCK_WORK {
	GMS_ENEMY_3D_WORK		gmk_work;
	OBS_ACTION3D_NN_WORK	obj_3d_tire;		// 車輪描画用

	void				(*seq)(struct tag_GMS_GMK_TRUCK_WORK*, GMS_PLAYER_WORK*);
	//s16				param_type;		//!< 使用パラメータータイプ
	GMS_PLAYER_WORK		*target_player;	//!< ターゲットプレイヤー (target_objに保持できない為)

	fx32				tire_spd_for_dir;	//!< 車輪回転速度算出用速度
	s32					tire_dir_spd;	//!< 車輪回転速度
	u16					tire_dir;		//!< 車輪回転量

	// タイヤ接着用
	NNS_MATRIX			tire_pos_f;
	NNS_MATRIX			tire_pos_b;
	// ライト接着用
	NNS_MATRIX			light_pos;

	NNS_VECTOR			trans_r;			// user_obj_mtx_r設定用トランス値

	// ぶら下がり時
	u16					slope_z_dir;	//!< 傾き角度保存
	u16					slope_f_y_dir;	//!< 手前傾き用
	u16					slope_f_z_dir;	//!< 手前傾き用

	// 火花
	GMS_EFFECT_3DES_WORK	*efct_f_spark;			//!< 火花前
	GMS_EFFECT_3DES_WORK	*efct_b_spark;			//!< 火花後

	// トロッコ転がりSE用ハンドル
	GSS_SND_SE_HANDLE		*h_snd_lorry;	//!< トロッコ転がり音

} GMS_GMK_TRUCK_WORK;


#define GMD_GMK_TRUCK_FIELD_RECT_LEFT		(-14)		//!< 地形あたり用矩形
#define GMD_GMK_TRUCK_FIELD_RECT_TOP		(-19)
#define GMD_GMK_TRUCK_FIELD_RECT_RIGHT		(14)
#define GMD_GMK_TRUCK_FIELD_RECT_BOTTOM		(19)



#if 0
/* 移動設定 */
// 移動加速度
#define GMD_GMK_TRUCK_SPDAD				(0x0091)				//!< 通常
#define GMD_GMK_TRUCK_SPDAD_SP			((fx32)(0x0091*3.0))	//!< スーパー
// 移動減速度
#define GMD_GMK_TRUCK_SPDDO				(0x0400)
#define GMD_GMK_TRUCK_SPDDO_SP			((fx32)(0x0400*2.0))
// 移動最大速度
#define GMD_GMK_TRUCK_SPDMA				(0x9000)
#define GMD_GMK_TRUCK_SPDMA_SP			(0xF000)
// 坂道加算速度
#define GMD_GMK_TRUCK_SLOPE_SPD			(0x2000)
#define GMD_GMK_TRUCK_SLOPE_SPD_SP		(0x2000)

// 傾斜速度
#define GMD_GMK_TRUCK_KEI_SPD			(0x0100*3/4)
#define GMD_GMK_TRUCK_KEI_SPD_SP		((fx32)(0x0100*3/4*1.5))
// 傾斜加速最大
#define GMD_GMK_TRUCK_KEI_SPDMA			(0xD000)
#define GMD_GMK_TRUCK_KEI_SPDMA_SP		(0xF000)

// ジャンプ速度
#define GMD_GMK_TRUCK_JUMP_SPD			(0x5A5A)		//!< 通常
#define GMD_GMK_TRUCK_JUMP_SPD_SP		(0x7FBF)		//!< スーパー
// ジャンプ時横方向加速度
#define GMD_GMK_TRUCK_JUMP_SPDAD		(0x0100)
#define GMD_GMK_TRUCK_JUMP_SPDAD_SP		((fx32)(0x0100*3.0))
// ジャンプ時横方向現速度
#define GMD_GMK_TRUCK_JUMP_SPDDO		(0x0800)
#define GMD_GMK_TRUCK_JUMP_SPDDO_SP		((fx32)(0x0800*2.0))
// ジャンプ時横方向最大速度
#define GMD_GMK_TRUCK_JUMP_SPDMA		(0x9000)
#define GMD_GMK_TRUCK_JUMP_SPDMA_SP		(0xF000)

// 傾斜角度
#define GMD_GMK_TRUCK_KEI_DIR			(0x2000-0x400)//(0x0100*3/4)//(0x20 - 4) // 坂道判定角度 45°
// 落下加速度
#define GMD_GMK_TRUCK_FALL_SPDAD		(0x02A8)
//#define GMD_GMK_TRUCK_FALL_WATER_SPDAD	(0x00c0)
// 落下最大速度
#define GMD_GMK_TRUCK_FALL_SPDMA		(0xF000)

// 落下待ち時間
//#define GMD_PL_SONIC_FALL_TIME		( 24 )
#endif


// 車輪描画オフセット
#define GMD_GMK_TRUCK_TIRE_FRONT_OFST_X	((fx32)(5.0 * 64/20*FX32_ONE))
#define GMD_GMK_TRUCK_TIRE_FRONT_OFST_Y	((fx32)(4.1188 * 64/20*FX32_ONE))
#define GMD_GMK_TRUCK_TIRE_FRONT_OFST_Z	((fx32)(5.151 * 64/20*FX32_ONE))
#define GMD_GMK_TRUCK_TIRE_BACK_OFST_X	((fx32)(-5.0 * 64/20*FX32_ONE))
#define GMD_GMK_TRUCK_TIRE_BACK_OFST_Y	((fx32)(4.1188 * 64/20*FX32_ONE))
#define GMD_GMK_TRUCK_TIRE_BACK_OFST_Z	((fx32)(5.151 * 64/20*FX32_ONE))

// エフェクト
//static float test_ofst = 9.f;
#define GMD_GMK_TRUCK_EFCT_SPRAK_OFST_DIST	(9.0f)	//!< 火花エフェクト表示オフセット

#if 0
/// トロッコパラメータ
typedef struct tag_GMS_GMK_TRUCK_PARAM {
	fx32	spd_add;
	fx32	spd_dec;
	fx32	spd_max;

	fx32	spd_slope;

	fx32	spd_kei;
	fx32	spd_kei_max;

	fx32	spd_jump;
	fx32	spd_jump_add;
	fx32	spd_jump_dec;
	fx32	spd_jump_max;

} GMS_GMK_TRUCK_PARAM;
#endif

// トロッコ重力
/* eve_rec->left */
// 平タイプ 影響範囲
/* eve_rec->top */
// 平タイプ 影響範囲
/* eve_rec->width */
/* eve_rec->height */
/* eve_rec->flag */
#define GMD_GMK_T_GRAVITY_A					(0x0001)		//!< A面の時だけ影響
#define GMD_GMK_T_GRAVITY_B					(0x0002)		//!< B面の時だけ影響
#define GMD_GMK_T_CLEAR_PSEUDOFALL_DIR_FIX	(0x0004)		//!< 擬似ジャンプ重力固定を解除



//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------
static void gmGmkTruckDest(MTS_TASK_TCB *tcb);
static void gmGmkTruckInitMain(OBS_OBJECT_WORK *obj_work, GMS_PLAYER_WORK *ply_work);
static void gmGmkTruckMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkTruckInitFree(OBS_OBJECT_WORK *obj_work, GMS_PLAYER_WORK *ply_work);
static void gmGmkTruckFreeMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkTruckInitDeathFall(OBS_OBJECT_WORK *obj_work, GMS_PLAYER_WORK *ply_work);
static void gmGmkTruckDeathFallMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkTruckDispFunc(OBS_OBJECT_WORK *obj_work);
static void gmGmkTruckBodyDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static void gmGmkTruckMotionCallback(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param);
static void gmGmkTruckSetMoveSeParam(OBS_OBJECT_WORK *obj_work, GSS_SND_SE_HANDLE *h_snd, GMS_PLAYER_WORK *ply_work, BOOL b_goal);

static void gmGmkTGravityChangeDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static void gmGmkTGravityForceChangeDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static void gmGmkTNoLandingDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);

static void gmGmkTruckCreateLightEfct(GMS_GMK_TRUCK_WORK *truck_work);
static void gmGmkTruckLightEfctMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkTruckLightEfctDispFunc(OBS_OBJECT_WORK *obj_work);

static void gmGmkTruckCreateSparkEfct(GMS_GMK_TRUCK_WORK *truck_work, s32 efct_type);
static void gmGmkTruckSparkEfctMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkTruckSparkEfctDispFunc(OBS_OBJECT_WORK *obj_work);
//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_truck_obj_3d_list = NULL;				//!< トロッコモデルワーク

#if 0
/// トロッコパラメータ
static GMS_GMK_TRUCK_PARAM gm_gmk_truck_param[2] = {
	// ソニック用
	{
		GMD_GMK_TRUCK_SPDAD,			// 移動加速度
		GMD_GMK_TRUCK_SPDDO,			// 移動減速度
		GMD_GMK_TRUCK_SPDMA,			// 最大速度
		GMD_GMK_TRUCK_SLOPE_SPD,		// 坂道加算速度
		GMD_GMK_TRUCK_KEI_SPD,			// 傾斜速度
		GMD_GMK_TRUCK_KEI_SPDMA,		// 傾斜最大速度
		GMD_GMK_TRUCK_JUMP_SPD,			// ジャンプ速度
		GMD_GMK_TRUCK_JUMP_SPDAD,		// ジャンプ時横方向加速度
		GMD_GMK_TRUCK_JUMP_SPDDO,		// ジャンプ時横方向減速度
		GMD_GMK_TRUCK_JUMP_SPDMA,		// ジャンプ時横方向最大速度
	},
	// スーパーソニック用
	{
		GMD_GMK_TRUCK_SPDAD_SP,
		GMD_GMK_TRUCK_SPDDO_SP,
		GMD_GMK_TRUCK_SPDMA_SP,
		GMD_GMK_TRUCK_SLOPE_SPD_SP,
		GMD_GMK_TRUCK_KEI_SPD_SP,
		GMD_GMK_TRUCK_KEI_SPDMA_SP,
		GMD_GMK_TRUCK_JUMP_SPD_SP,
		GMD_GMK_TRUCK_JUMP_SPDAD_SP,
		GMD_GMK_TRUCK_JUMP_SPDDO_SP,
		GMD_GMK_TRUCK_JUMP_SPDMA_SP,
	},
};
#endif

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkTruckBuild
/*!
 *	ギミック トロッコ データ構築
 */
// ==========================================================================
void GmGmkTruckBuild(void)
{
	// オブジェクト構築
	gm_gmk_truck_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_TRUCK_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_TRUCK_TEX),
								0/*NND_DRAWOBJ_MATCTRL_SPECULAR draw_flag*/);
}

// ==========================================================================
// GmGmkTruckFlush
/*!
 *	ギミック トロッコ データ片付け
 */
// ==========================================================================
void GmGmkTruckFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_TRUCK_MODEL);

	// オブジェクト開放
	GmGameDBuildRegFlushModel(gm_gmk_truck_obj_3d_list, amb->file_num);
}


// ==========================================================================
// GmGmkTruckInit
/*!
 *	ギミック トロッコ 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *			
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkTruckInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_TRUCK_WORK	*truck_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	// オブジェクト生成
	//obj_work = GMM_ENEMY_CREATE_RIDE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_TRUCK_WORK), "GMK_TRUCK");
	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_TRUCK_WORK), "GMK_TRUCK");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	truck_work = (GMS_GMK_TRUCK_WORK*)obj_work;

	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkTruckDest);

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_truck_obj_3d_list[IDB_GMK_TRUCK_MDL_GMK_TRUCK_ZNO],
					&gmk_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
										ObjDataGet(GMD_DWORK_NO_GMK_TRUCK_MTN), NULL/*mtn_data_path*/,
										0/*index*/, NULL/*archive*/);
	// アクション設定
	ObjDrawObjectActionSet(obj_work, IDB_GMK_TRUCK_MTN_GMK_TRUCK_RUN_ZNM);

	// 車輪モデル初期化
	ObjCopyAction3dNNModel(&gm_gmk_truck_obj_3d_list[IDB_GMK_TRUCK_MDL_GMK_TRUCK_TIRE_ZNO], &truck_work->obj_3d_tire);
			// コピーのみなので、デストラクタでの開放は不要

	// 描画フラグ設定
//	gmk_work->obj_3d.drawflag |= NND_DRAWOBJ_MATCTRL_SPECULAR;
//	truck_work->obj_3d_tire.drawflag |= NND_DRAWOBJ_MATCTRL_SPECULAR;

	// モーションコールバック設定
	gmk_work->obj_3d.mtn_cb_func = gmGmkTruckMotionCallback;
	gmk_work->obj_3d.mtn_cb_param= truck_work;

	// ppOut差し替え
	obj_work->ppOut = gmGmkTruckDispFunc;

	/* フラグ設定 */
	obj_work->flag |= OBD_OBJECT_B;
	obj_work->move_flag |= OBD_MOVE_FALL;
	obj_work->disp_flag |= OBD_DISP_USERMTX_RIGHT | OBD_DISP_REPEAT;

	// モーション速度設定
	obj_work->disp_flag |= OBD_DISP_STOP;	// はじめは停止で
	//obj_work->obj_3d->speed[0] = 0.f;	// はじめは0で
	//obj_work->obj_3d->speed[1] = 0.f;
	obj_work->obj_3d->blend_spd = 1.f/8;


	// 描画中心位置調整
	truck_work->trans_r.x = 0.f;
	truck_work->trans_r.y = 0.f;
	truck_work->trans_r.z = 4.f / FXM_FX32_TO_FLOAT(g_obj.draw_scale.x);	// X : -4
	nnMakeUnitMatrix(&gmk_work->obj_3d.user_obj_mtx_r);
	nnTranslateMatrix(&gmk_work->obj_3d.user_obj_mtx_r, &gmk_work->obj_3d.user_obj_mtx_r,
			//-4.f / FXM_FX32_TO_FLOAT(g_obj.draw_scale.x), 0.f, 0.f);
			truck_work->trans_r.x, truck_work->trans_r.y, truck_work->trans_r.z);


	/* 地形あたり矩形設定 */
	ObjObjectFieldRectSet(obj_work, GMD_GMK_TRUCK_FIELD_RECT_LEFT,
									GMD_GMK_TRUCK_FIELD_RECT_TOP,
									GMD_GMK_TRUCK_FIELD_RECT_RIGHT,
									GMD_GMK_TRUCK_FIELD_RECT_BOTTOM);

	// 矩形設定
	// 対プレイヤー
	gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = gmGmkTruckBodyDefFunc;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	ObjRectWorkSet(rect_work,
						-64, -64,
						64, 64);

	// 専用ライト設定
	{
		NNS_RGBA	light_col = {
			1.0f, 1.0f, 1.0f, 1.0f,
		};
		NNS_VECTOR	light_vec;

		light_vec.x = -0.85f;
		light_vec.y = -0.45f;
		light_vec.z = -3.05f;
		nnNormalizeVector(&light_vec, &light_vec);
		ObjDrawSetParallelLight(NNE_LIGHT_1, &light_col, 1.f, &light_vec);

		gmk_work->obj_3d.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
		truck_work->obj_3d_tire.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
		gmk_work->obj_3d.use_light_flag |= OBD_LIGHT_USE_FLAG_1;
		truck_work->obj_3d_tire.use_light_flag |= OBD_LIGHT_USE_FLAG_1;
	}

	// トロッコライト設定
	gmGmkTruckCreateLightEfct(truck_work);

	// 画面外破棄時に再生成しないようにする
	gmk_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_DIE;

#if _IPHONE
	// ライトが消えるので後から描く
	obj_work->obj_3d->command_state = OBD_DRAW_CMD_STATE_3DNN_POST;
	//obj_work->disp_flag |= OBD_DISP_NODISP;
#endif // _IPHONE
	return (obj_work);
}


// ==========================================================================
// トロッコ重力, 強制重力変換
// ==========================================================================
// ==========================================================================
// GmGmkTruckGravityInit
/*!
 *	ギミック トロッコ重力, 強制重力変換 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *			
 */
// ==========================================================================
/// Rタイプ矩形テーブル
static const s16 gm_gmk_t_gravity_r_rect_tbl[4] = {
	// left, top, right, bottom
	-128, -128,  128,  128,		// GMD_EVENT_ID_GMK_T_GRAVITY_R_20 ～ E0 共通
};
/// 逆Rタイプ矩形テーブル
static const s16 gm_gmk_t_gravity_rr_rect_tbl[4][MTD_RECT] = {
	// left, top, right, bottom
	{   0, -128,  128,    0},		// GMD_EVENT_ID_GMK_T_GRAVITY_RR_20
	{   0,    0,  128,  128},		// GMD_EVENT_ID_GMK_T_GRAVITY_RR_60
	{-128,    0,    0,  128},		// GMD_EVENT_ID_GMK_T_GRAVITY_RR_A0
	{-128, -128,    0,    0},		// GMD_EVENT_ID_GMK_T_GRAVITY_RR_E0
};
OBS_OBJECT_WORK* GmGmkTruckGravityInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_COM_WORK	*ene_com;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	// オブジェクト生成
	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_COM_WORK), "GMK_T_GRAVITY");
	ene_com = (GMS_ENEMY_COM_WORK*)obj_work;


	/* フラグ設定 */
	obj_work->move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE;
	obj_work->disp_flag |= OBD_DISP_NODISP;


    /* 矩形設定 */
	rect_work = &ene_com->rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectGroupSet(rect_work,
						GMD_OBJ_RECT_GROUP_ENEMY,
						GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
	ObjRectAtkSet(rect_work, 0, GMD_OBJ_RECT_ATK_POWER_DEFAULT);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);	// 身体のみHIT可
	if (GMD_EVENT_ID_GMK_T_GRAVITY_R_20 <= eve_rec->id &&
				eve_rec->id <= GMD_EVENT_ID_GMK_T_GRAVITY_RR_E0) {
		// Rタイプ, RRタイプ
		// 矩形サイズ固定タイプ
		const s16	*rect_data;
		if (GMD_EVENT_ID_GMK_T_GRAVITY_R_20 <= eve_rec->id &&
				eve_rec->id <= GMD_EVENT_ID_GMK_T_GRAVITY_R_E0)  {
			// Rタイプ
			rect_data = &gm_gmk_t_gravity_r_rect_tbl [0];
		}
		else {
			// 逆Rタイプ
			rect_data = &gm_gmk_t_gravity_rr_rect_tbl[eve_rec->id - GMD_EVENT_ID_GMK_T_GRAVITY_RR_20][0];
		}
		// 矩形サイズ設定タイプ
		ObjRectSet(&rect_work->rect,
				rect_data[MTD_LEFT],  rect_data[MTD_TOP],
				rect_data[MTD_RIGHT],  rect_data[MTD_BOTTOM]);
	}
	else {
		// 矩形サイズ設定タイプ
		ObjRectSet(&rect_work->rect,
				(s16)(eve_rec->left << 1),  (s16)(eve_rec->top << 1),
				(s16)((eve_rec->width + eve_rec->left) << 1), (s16)((eve_rec->height + eve_rec->top) << 1));
	}
	rect_work->parent_obj = obj_work;
	rect_work->flag |= OBD_RECT_NODAMAGE | OBD_RECT_NOHIT_UP;
	if (GMD_EVENT_ID_GMK_T_FC_GRAVITY_D <= eve_rec->id &&
				eve_rec->id <= GMD_EVENT_ID_GMK_T_FC_GRAVITY_R) {
		// 強制重力変換
		rect_work->ppDef = gmGmkTGravityForceChangeDefFunc;
	}
	else {
		// トロッコ重力
		rect_work->ppDef = gmGmkTGravityChangeDefFunc;
	}

	// 他の矩形は無効に
	ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;

	return (obj_work);
}



// ==========================================================================
// トロッコ接地不可
// ==========================================================================
// ==========================================================================
// GmGmkTruckNoLandingInit
/*!
 *	ギミック トロッコ接地不可 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *			
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkTruckNoLandingInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_COM_WORK	*ene_com;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	// オブジェクト生成
	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_COM_WORK), "GMK_T_NOLANDING");
	ene_com = (GMS_ENEMY_COM_WORK*)obj_work;


	/* フラグ設定 */
	obj_work->move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE;
	obj_work->disp_flag |= OBD_DISP_NODISP;


    /* 矩形設定 */
	rect_work = &ene_com->rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectGroupSet(rect_work,
						GMD_OBJ_RECT_GROUP_ENEMY,
						GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
	ObjRectAtkSet(rect_work, 0, GMD_OBJ_RECT_ATK_POWER_DEFAULT);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);	// 身体のみHIT可

	// 矩形サイズ設定
	ObjRectSet(&rect_work->rect,
			(s16)(eve_rec->left << 1),  (s16)(eve_rec->top << 1),
			(s16)((eve_rec->width + eve_rec->left) << 1), (s16)((eve_rec->height + eve_rec->top) << 1));

	rect_work->ppDef = gmGmkTNoLandingDefFunc;
	rect_work->parent_obj = obj_work;
	rect_work->flag |= OBD_RECT_NODAMAGE | OBD_RECT_NOHIT_UP;

	// 他の矩形は無効に
	ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;

	return (obj_work);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// トロッコ
// ==========================================================================
// ==========================================================================
// gmGmkTruckDest
/*!
 *	ギミック トロッコ デストラクタ
 *
 *	@param tcb	[in] TCBワーク
 *
 *	@note
 */
// ==========================================================================
void gmGmkTruckDest(MTS_TASK_TCB *tcb)
{
	GMS_GMK_TRUCK_WORK	*truck_work;

	truck_work = (GMS_GMK_TRUCK_WORK*)mtTaskGetTcbWork(tcb);

	// サウンドハンドル解放
	if (truck_work->h_snd_lorry) {
		GmSoundStopSE(truck_work->h_snd_lorry);
		GsSoundFreeSeHandle(truck_work->h_snd_lorry);
		truck_work->h_snd_lorry = NULL;
	}
	
	// 汎用終了処理
	GmEnemyDefaultExit(tcb);
}

// ==========================================================================
// gmGmkTruckInitMain
/*!
 *	ギミック トロッコ メイン初期化
 *
 *	@param obj_work	[in] トロッコオブジェクトワーク
 *	@param obj_work	[in] プレイヤーワーク
 *
 *	@note
 */
// ==========================================================================
void gmGmkTruckInitMain(OBS_OBJECT_WORK *obj_work, GMS_PLAYER_WORK *ply_work)
{
	GMS_GMK_TRUCK_WORK	*truck_work;

	UNREFERENCED_PARAMETER(ply_work);

	truck_work	= (GMS_GMK_TRUCK_WORK*)obj_work;

	// フラグ設定
	obj_work->flag |= OBD_OBJECT_NOHIT;
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

	// メイン処理設定
	obj_work->ppFunc = gmGmkTruckMain;

	// 火花エフェクト生成
	gmGmkTruckCreateSparkEfct(truck_work, GME_EFCT_Z03_IDX_SPARK_S);

	// サウンドハンドル取得
	truck_work->h_snd_lorry = GsSoundAllocSeHandle();

	// SE再生開始
	GmSoundPlaySEForce("Lorry", truck_work->h_snd_lorry);

	// シーケンス初期化
//	truck_work->param_type = 0;		// 使用パラメータタイプ
	//gmGmkTruckSeqRunInit(truck_work, ply_work);
}

// ==========================================================================
// gmGmkTruckMain
/*!
 *	ギミック トロッコ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 *	@note
 */
// ==========================================================================
void gmGmkTruckMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_COM_WORK	*ene_com;
	GMS_GMK_TRUCK_WORK	*truck_work;
	GMS_PLAYER_WORK		*ply_work;
	s32					act_id;
	u32					disp_flag;
	float				truck_c_x, truck_c_y, truck_c_z;

	truck_work	= (GMS_GMK_TRUCK_WORK*)obj_work;
	ene_com		= (GMS_ENEMY_COM_WORK*)obj_work;

	if (truck_work->target_player == NULL) {
		MTM_ASSERT(0);		// 本来は発生しない
		gmGmkTruckInitDeathFall(obj_work, NULL);
		obj_work->ppFunc(obj_work);
		return;
	}

	if (!(truck_work->target_player->player_flag & GMD_PLF_TRUCK_RIDE)) {
		// トロッコが外れた
#if 1
		// 単に外れた
		gmGmkTruckInitFree(obj_work, truck_work->target_player);
		obj_work->ppFunc(obj_work);
		return;
#else
		if (truck_work->target_player->player_flag & GMD_PLF_DIE) {
			// プレイヤー死んでる
			gmGmkTruckInitDeathFall(obj_work, truck_work->target_player);
			obj_work->ppFunc(obj_work);
			return;
		}
		else {
			// 単に外れた
			gmGmkTruckInitFree(obj_work, truck_work->target_player);
			obj_work->ppFunc(obj_work);
			return;
		}
#endif
	}
	else if (truck_work->target_player->gmk_flag2 & GMD_PLGF2_TRUCK_FALL_DEATH) {
		// プレイヤー落下死亡
		gmGmkTruckInitDeathFall(obj_work, truck_work->target_player);
		obj_work->ppFunc(obj_work);
		return;
	}
	else if (truck_work->target_player->player_flag & GMD_PLF_DIE) {
		// プレイヤー死亡時はZ座標調整
		obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_N_BACK + 16*FX32_ONE;
	}

	ply_work = (GMS_PLAYER_WORK*)truck_work->target_player;

	// プレイヤーへの接着
	obj_work->prev_pos = obj_work->pos;
//	obj_work->pos = ply_work->obj_work.pos;
	obj_work->pos.x = ply_work->obj_work.pos.x;
	obj_work->pos.y = ply_work->obj_work.pos.y;			// Zはコピーしない
	obj_work->move.x = obj_work->pos.x - obj_work->prev_pos.x;
	obj_work->move.y = obj_work->pos.y - obj_work->prev_pos.y;
	obj_work->move.z = obj_work->pos.z - obj_work->prev_pos.z;
	obj_work->dir = ply_work->obj_work.dir;
	obj_work->dir.z +=  + ply_work->obj_work.dir_fall;
	
#if GMD_PLY_SEQ_GMK_TRUCK_DANGER_VIB_PLAYER && GMD_PLY_SEQ_GMK_TRUCK_DANGER_VIB_TRUCK
	obj_work->vib_timer = ply_work->obj_work.vib_timer;
#endif

	// アニメーション停止をクリア
	obj_work->disp_flag &= ~OBD_DISP_STOP;
	
	/* 車輪の回転 */
	if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
		// 接地中
		truck_work->tire_dir_spd = ply_work->obj_work.spd_m;
	}
	else {
		// 段々減速
		truck_work->tire_dir_spd = ObjSpdDownSet(truck_work->tire_dir_spd, 0x0080);
	}
	// 車輪回転
#if _IPHONE
#define GMD_GMK_TRUCK_ROT_DIV (16*FX32_ONE)
#else
#define GMD_GMK_TRUCK_ROT_DIV (4*FX32_ONE)
#endif // _IPHONE
	truck_work->tire_dir += (u16)FX_Div(truck_work->tire_dir_spd, (fx32)(GMD_GMK_TRUCK_ROT_DIV));


	/* アクション切り替えチェック */
	act_id = -1;
	disp_flag = 0;
	if ((GME_PLY_ACT_STATE_FW <= ply_work->act_state &&
				ply_work->act_state <= GME_PLY_ACT_STATE_WAIT_2_2) ||
			ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_FW ||
			ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_FW_L ||
			ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_DANGER ||
			ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_DANGER_RE ||
			ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_DANGER_ST) {
		// 停止中
		act_id = IDB_GMK_TRUCK_MTN_GMK_TRUCK_ST_ZNM;
		disp_flag = OBD_DISP_REPEAT;
	}
	else if (GME_PLY_ACT_STATE_GMK_TRUCK_RUN <= ply_work->act_state &&
				ply_work->act_state <= GME_PLY_ACT_STATE_GMK_TRUCK_RUN_L) {
		// 歩き中
		act_id = IDB_GMK_TRUCK_MTN_GMK_TRUCK_RUN_ZNM;
		disp_flag = OBD_DISP_REPEAT;
	}
	else if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
		// ジャンプ
		act_id = IDB_GMK_TRUCK_MTN_GMK_TRUCK_JP_ZNM;
		disp_flag = OBD_DISP_REPEAT;
	}
	else if ((ply_work->obj_work.move_flag & OBD_MOVE_UNDER) &&
				!(ply_work->obj_work.move_flag & OBD_MOVE_UNDERPREV)) {
		// 着地
		act_id = IDB_GMK_TRUCK_MTN_GMK_TRUCK_DOWN_ZNM;
	}
	else if (obj_work->obj_3d->act_id[0] == IDB_GMK_TRUCK_MTN_GMK_TRUCK_DOWN_ZNM &&
			obj_work->disp_flag & OBD_DISP_END) {
		// 着地終了
		if (ply_work->obj_work.spd_m) {
			act_id = IDB_GMK_TRUCK_MTN_GMK_TRUCK_RUN_ZNM;
		}
		else {
			act_id = IDB_GMK_TRUCK_MTN_GMK_TRUCK_ST_ZNM;
		}
		disp_flag = OBD_DISP_REPEAT;
	}
	else if (GME_PLY_ACT_STATE_LOOKUP1 <= ply_work->act_state &&
				ply_work->act_state <= GME_PLY_ACT_STATE_SQUATDOWN3) {
		if (obj_work->obj_3d->act_id[0] != IDB_GMK_TRUCK_MTN_GMK_TRUCK_DOWN_ZNM ||
				obj_work->disp_flag & OBD_DISP_END) {
			// 停止中
			act_id = IDB_GMK_TRUCK_MTN_GMK_TRUCK_ST_ZNM;
			disp_flag = OBD_DISP_REPEAT;
		}
	}

	if (act_id != -1) {
		if (obj_work->obj_3d->act_id[0] != act_id) {
			// アクション設定
			ObjDrawObjectActionSet3DNNBlend(obj_work, act_id);
			obj_work->disp_flag |= disp_flag;
		}
	}

	// 停止中でない. しゃがみ, 見上げ中の着地以外はプレイヤーのフレームにあわせる
	if (obj_work->obj_3d->act_id[0] != IDB_GMK_TRUCK_MTN_GMK_TRUCK_ST_ZNM &&
			!(GME_PLY_ACT_STATE_LOOKUP1 <= ply_work->act_state &&
				ply_work->act_state <= GME_PLY_ACT_STATE_SQUATDOWN3 &&
				obj_work->obj_3d->act_id[0] == IDB_GMK_TRUCK_MTN_GMK_TRUCK_DOWN_ZNM)) {
		obj_work->obj_3d->frame[0] = ply_work->obj_work.obj_3d->frame[0];
	}

	// 傾き角度クリア
	truck_work->slope_f_y_dir = 0;
	truck_work->slope_f_z_dir = 0;
	truck_work->slope_z_dir = 0;
	
	// プレイヤー角度対応
#if 1
	if (!(ply_work->player_flag & GMD_PLF_USER3)) {
		truck_c_x = GMD_GMK_TRUCK_FW_CENTER_X;
		truck_c_y = GMD_GMK_TRUCK_FW_CENTER_Y;
		truck_c_z = GMD_GMK_TRUCK_FW_CENTER_Z;
	}
	else {
		truck_c_x = GMD_GMK_TRUCK_BW_CENTER_X;
		truck_c_y = GMD_GMK_TRUCK_BW_CENTER_Y;
		truck_c_z = GMD_GMK_TRUCK_BW_CENTER_Z;
	}
#endif
	nnMakeUnitMatrix(&obj_work->obj_3d->user_obj_mtx_r);
	nnTranslateMatrix(&obj_work->obj_3d->user_obj_mtx_r, &obj_work->obj_3d->user_obj_mtx_r,
			truck_work->trans_r.x, truck_work->trans_r.y, truck_work->trans_r.z);
	if ((ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_STICK) && ply_work->gmk_work3) {
		// 踏ん張り中は角度を反映
		NNS_MATRIX	mat;
#if 1
		// 傾き角度取得
		truck_work->slope_z_dir = (u16)ply_work->gmk_work3;
		truck_work->slope_f_z_dir = (u16)(MTM_MATH_ABS(ply_work->gmk_work3) >> 2);
		truck_work->slope_f_y_dir = (u16)(ply_work->gmk_work3 >> 2);

		nnMakeUnitMatrix(&mat);
		nnTranslateMatrix(&mat, &mat, 
					-truck_c_x, -truck_c_y, -truck_c_z); 
		nnRotateXMatrix(&mat, &mat, truck_work->slope_z_dir);
		nnRotateYMatrix(&mat, &mat, truck_work->slope_f_y_dir);
		nnRotateZMatrix(&mat, &mat, truck_work->slope_f_z_dir);
		nnTranslateMatrix(&mat, &mat, 
					truck_c_x, truck_c_y, truck_c_z); 
		nnMultiplyMatrix(&obj_work->obj_3d->user_obj_mtx_r, &obj_work->obj_3d->user_obj_mtx_r, &mat);
#else
		nnMakeUnitMatrix(&mat);
		nnTranslateMatrix(&mat, &mat, 
					-GMD_GMK_TRUCK_FW_CENTER_X, -GMD_GMK_TRUCK_FW_CENTER_Y, -GMD_GMK_TRUCK_FW_CENTER_Z); 
		nnRotateXMatrix(&mat, &mat, ply_work->gmk_work3);
		nnTranslateMatrix(&mat, &mat, 
					GMD_GMK_TRUCK_FW_CENTER_X, GMD_GMK_TRUCK_FW_CENTER_Y, GMD_GMK_TRUCK_FW_CENTER_Z); 
		nnMultiplyMatrix(&obj_work->obj_3d->user_obj_mtx_r, &obj_work->obj_3d->user_obj_mtx_r, &mat);
#endif
	}
#if 1
	// エフェクト再生成チェック
	if ((ply_work->obj_work.move_flag & OBD_MOVE_UNDER) &&
			(MTM_MATH_ABS(ply_work->obj_work.spd_m) >= GMD_GMK_TRUCK_SPARK_EFCT_SMALL_MIN_SPD) &&
			(truck_work->efct_f_spark == NULL || truck_work->efct_b_spark == NULL)) {
		// エフェクトが消えているので再生成
#if !_IPHONE
		if (MTM_MATH_ABS(ply_work->obj_work.spd_m) >= GMD_GMK_TRUCK_SPARK_EFCT_BIG_MIN_SPD) {
			// 大生成
			gmGmkTruckCreateSparkEfct(truck_work, GME_EFCT_Z03_IDX_SPARK);
		}
		else
#endif // !_IPHONE
		{
			// 小生成
			gmGmkTruckCreateSparkEfct(truck_work, GME_EFCT_Z03_IDX_SPARK_S);
		}
	}
#endif 
	// SE調整
#if 1
	gmGmkTruckSetMoveSeParam(obj_work,
							truck_work->h_snd_lorry,
							ply_work,
							ply_work->player_flag & GMD_PLF_GOAL ? TRUE : FALSE);
#else
	{
		CriFloat32	ctrl_val = 0.f;
		fx32		abs_spd_m = MTM_MATH_ABS(ply_work->obj_work.spd_m);

		if ((ply_work->obj_work.move_flag & OBD_MOVE_UNDER) &&
				abs_spd_m >= GMD_GMK_TRUCK_SE_MIN_SPD) {
			if (abs_spd_m >= GMD_GMK_TRUCK_SE_MAX_SPD) {
				ctrl_val = 1.0f;
			}
			else {
				ctrl_val = FXM_FX32_TO_FLOAT(FX_Div(abs_spd_m - GMD_GMK_TRUCK_SE_MIN_SPD,
											GMD_GMK_TRUCK_SE_MAX_SPD - GMD_GMK_TRUCK_SE_MIN_SPD));
				if (ctrl_val > 1.0f) {
					ctrl_val = 1.0f;
				}
			}
		}
		truck_work->h_snd_lorry->au_player->SetAisac("Speed", ctrl_val);  

		if (ply_work->player_flag & GMD_PLF_GOAL) {
			float		volume = 1.0f;
			float		dist_x, dist_y, dist;
			OBS_CAMERA	*camera = ObjCameraGet(g_obj.glb_camera_id);

			dist_x = (FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.x) - camera->disp_pos.x);
			dist_y = (FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.y) - (-camera->disp_pos.y));

			if (dist_x < GMD_GMK_TRUCK_SE_GOAL_MAX_DIST && dist_y < GMD_GMK_TRUCK_SE_GOAL_MAX_DIST) {
				dist = dist_x*dist_x + dist_y*dist_y;
				if (dist <= GMD_GMK_TRUCK_SE_GOAL_MIN_DIST*GMD_GMK_TRUCK_SE_GOAL_MIN_DIST) {
					volume = 1.0f;
				}
				else if (dist <= GMD_GMK_TRUCK_SE_GOAL_MAX_DIST*GMD_GMK_TRUCK_SE_GOAL_MAX_DIST) {
					volume = (GMD_GMK_TRUCK_SE_GOAL_MAX_DIST*GMD_GMK_TRUCK_SE_GOAL_MAX_DIST - dist) /
									((GMD_GMK_TRUCK_SE_GOAL_MAX_DIST - GMD_GMK_TRUCK_SE_GOAL_MIN_DIST)*(GMD_GMK_TRUCK_SE_GOAL_MAX_DIST - GMD_GMK_TRUCK_SE_GOAL_MIN_DIST));
					if (volume > 1.0f) {
						volume = 1.0f;
					}
					else if (volume < 0.0f) {
						volume = 0.0f;
					}
				}
				else {
					volume = 0.0f;
				}
			}
			else {
				volume = 0.0f;
			}

			truck_work->h_snd_lorry->snd_ctrl_param.volume = volume;
		}
	}
#endif

#if 0
#if defined (MTD_DEBUG)
	// 専用ライト設定
	{
		NNS_RGBA	light_col = {
			1.0f, 1.0f, 1.0f, 1.0f,
		};
		static NNS_VECTOR	debug_light_vec = {-0.85f, -0.45f, -3.05f};
		NNS_VECTOR	light_vec;

		if (AoPadDirect() & KEY_R1) {
			if (AoPadStand() & KEY_L_LEFT) {
				debug_light_vec.x += 0.05;
			}
			else if (AoPadStand() & KEY_L_RIGHT) {
				debug_light_vec.x -= 0.05;
			}
			else if (AoPadStand() & KEY_L_UP) {
				debug_light_vec.y -= 0.05;
			}
			else if (AoPadStand() & KEY_L_DOWN) {
				debug_light_vec.y += 0.05;
			}
			else if (AoPadStand() & KEY_R_UP) {
				debug_light_vec.z -= 0.05;
			}
			else if (AoPadStand() & KEY_R_LEFT) {
				debug_light_vec.z += 0.05;
			}
		}

	//	light_vec.x = 0.4f;
	//	light_vec.y = 0.0f;
	//	light_vec.z = -1.0f;
		nnNormalizeVector(&light_vec, &debug_light_vec);
		ObjDrawSetParallelLight(NNE_LIGHT_1, &light_col, 1.f, &light_vec);
	}
#endif
#endif
}

// ==========================================================================
// gmGmkTruckInitFree
/*!
 *	ギミック トロッコ プレイヤーから外れた
 *
 *	@param obj_work	[in] トロッコオブジェクトワーク
 *	@param obj_work	[in] プレイヤーワーク
 *
 *	@note
 */
// ==========================================================================
void gmGmkTruckInitFree(OBS_OBJECT_WORK *obj_work, GMS_PLAYER_WORK *ply_work)
{
	GMS_GMK_TRUCK_WORK	*truck_work = (GMS_GMK_TRUCK_WORK*)obj_work;
	u32					flag, move_flag;

	truck_work->target_player = NULL;		// ターゲットプレイヤークリア

	if (ply_work) {
		// 速度を引き継ぐ
		obj_work->spd	= ply_work->obj_work.spd;
		obj_work->spd_m	= ply_work->obj_work.spd_m;

		flag		= ply_work->obj_work.flag;
		move_flag	= ply_work->obj_work.move_flag;
	}
	else {
		obj_work->spd.x = 0;
		obj_work->spd.y = 0;
		obj_work->spd.z = 0;

		flag = 0;
		move_flag = 0;
	}
	obj_work->flag &= ~OBD_OBJECT_B;
	obj_work->flag |= OBD_OBJECT_NOHIT | (flag & OBD_OBJECT_B);			// 矩形あたりなし 面引継ぎ
	obj_work->flag &= ~OBD_OBJECT_NOCLIP;								// クリッピング再開
	obj_work->move_flag |= OBD_MOVE_FALL | OBD_MOVE_DIR;				// 落下あり
	obj_work->move_flag &= ~(OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE |			// 移動開始 地形チェック開始
							OBD_MOVE_NOSPDM | OBD_MOVE_LIMIT_OUT);		// spd_m使用 画面外地形OFF


	if (move_flag & OBD_MOVE_JUMP) {
		obj_work->move_flag |= OBD_MOVE_JUMP;
	}
	else {
		// X速度クリア
		if (obj_work->spd.x > obj_work->spd_m) {
			obj_work->spd_m = obj_work->spd.x;
		}
		obj_work->spd.x = 0;
		// アクション設定
		if (obj_work->obj_3d->act_id[0] != IDB_GMK_TRUCK_MTN_GMK_TRUCK_RUN_ZNM) {
			ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_GMK_TRUCK_MTN_GMK_TRUCK_RUN_ZNM);
			obj_work->disp_flag |= OBD_DISP_REPEAT;
		}
	}

	// メイン処理
	obj_work->ppFunc = gmGmkTruckFreeMain;
}

// ==========================================================================
// gmGmkTruckFreeMain
/*!
 *	ギミック トロッコ プレイヤーから外れた
 *
 *	@param obj_work	[in] オブジェクトワーク
 *	@note
 */
// ==========================================================================
void gmGmkTruckFreeMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_TRUCK_WORK	*truck_work = (GMS_GMK_TRUCK_WORK*)obj_work;

	// 右へはける
	if (obj_work->move_flag & OBD_MOVE_UNDER &&
			obj_work->move_flag & OBD_MOVE_JUMP) {
		// 着地
		if (obj_work->spd.x > obj_work->spd_m) {
			obj_work->spd_m = obj_work->spd.x;
		}
		obj_work->spd.x = 0;
		obj_work->move_flag &= ~OBD_MOVE_JUMP;

		// アクション設定
		if (obj_work->obj_3d->act_id[0] != IDB_GMK_TRUCK_MTN_GMK_TRUCK_RUN_ZNM) {
			ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_GMK_TRUCK_MTN_GMK_TRUCK_RUN_ZNM);
			obj_work->disp_flag |= OBD_DISP_REPEAT;
		}

		// 着地SE
		GmSoundPlaySE("Lorry4");
	}
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		obj_work->spd_m = ObjSpdUpSet(obj_work->spd_m, 0x080, 0xA000);
	}
	else {
		obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, 0x080, 0xA000);
	}

	/* 車輪の回転 */
	if (obj_work->move_flag & OBD_MOVE_UNDER) {
		// 接地中
		truck_work->tire_dir_spd = obj_work->spd_m;
	}
	else {
		// 段々減速
		truck_work->tire_dir_spd = ObjSpdDownSet(truck_work->tire_dir_spd, 0x0080);
	}
	// 車輪回転
	truck_work->tire_dir += (u16)FX_Div(truck_work->tire_dir_spd, (fx32)(4*FX32_ONE));

	// エフェクト再生成チェック
	if ((obj_work->move_flag & OBD_MOVE_UNDER) &&
			(MTM_MATH_ABS(obj_work->spd_m) >= GMD_GMK_TRUCK_SPARK_EFCT_SMALL_MIN_SPD) &&
			(truck_work->efct_f_spark == NULL || truck_work->efct_b_spark == NULL)) {
		// エフェクトが消えているので再生成
#if !_IPHONE
		if (MTM_MATH_ABS(obj_work->spd_m) >= GMD_GMK_TRUCK_SPARK_EFCT_BIG_MIN_SPD) {
			// 大生成
			gmGmkTruckCreateSparkEfct(truck_work, GME_EFCT_Z03_IDX_SPARK);
		}
		else
#endif // !_IPHONE
		{
			// 小生成
			gmGmkTruckCreateSparkEfct(truck_work, GME_EFCT_Z03_IDX_SPARK_S);
		}
	}

	// SE調整
	gmGmkTruckSetMoveSeParam(obj_work,
							truck_work->h_snd_lorry,
							NULL,
							TRUE);
}

// ==========================================================================
// gmGmkTruckInitDeathFall
/*!
 *	ギミック トロッコ プレイヤー死亡時落下
 *
 *	@param obj_work	[in] トロッコオブジェクトワーク
 *	@param obj_work	[in] プレイヤーワーク
 *
 *	@note
 */
// ==========================================================================
void gmGmkTruckInitDeathFall(OBS_OBJECT_WORK *obj_work, GMS_PLAYER_WORK *ply_work)
{
	GMS_GMK_TRUCK_WORK	*truck_work = (GMS_GMK_TRUCK_WORK*)obj_work;

	truck_work->target_player = NULL;		// ターゲットプレイヤークリア

	if (ply_work) {
		// 速度を引き継ぐ
		obj_work->spd	= ply_work->obj_work.spd;
		obj_work->spd_m	= ply_work->obj_work.spd_m;
	}
	else {
		obj_work->spd.x = 0;
		obj_work->spd.y = 0;
		obj_work->spd.z = 0;
	}
	ObjObjectSpdDirFall(&obj_work->spd.x, &obj_work->spd.y, g_gm_main_system.pseudofall_dir);
	// 速度を少し大きめに
	obj_work->spd.x = FX_Mul(obj_work->spd.x, 0x1200);
	obj_work->spd.y = FX_Mul(obj_work->spd.y, 0x1200);

	// 落下速度をspd_addで設定する
	obj_work->spd_add.x = 0;
	obj_work->spd_add.y = obj_work->spd_fall;
	ObjObjectSpdDirFall(&obj_work->spd_add.x, &obj_work->spd_add.y, g_gm_main_system.pseudofall_dir);


	obj_work->flag |= OBD_OBJECT_NOHIT;										// 矩形あたりなし
	obj_work->flag &= ~OBD_OBJECT_NOCLIP;									// クリッピング再開
	//obj_work->move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_FALL | OBD_MOVE_JUMP;	// 地形あたりなし 落下あり
	obj_work->move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_JUMP;	// 地形あたりなし
	obj_work->move_flag &= ~(OBD_MOVE_NOMOVE | OBD_MOVE_FALL);				// 移動開始 落下なし
	//obj_work->move_flag &= ~OBD_MOVE_NOMOVE;								// 移動開始

	// Z位置調整
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_N_BACK + 16*FX32_ONE;

	// メイン処理
	obj_work->ppFunc = gmGmkTruckDeathFallMain;
}

// ==========================================================================
// gmGmkTruckDeathFallMain
/*!
 *	ギミック トロッコ プレイヤー死亡時落下 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 *	@note
 */
// ==========================================================================
void gmGmkTruckDeathFallMain(OBS_OBJECT_WORK *obj_work)
{
	// 死亡演出
	// 回転する
	obj_work->dir.z += 0x400;
}

// ==========================================================================
// gmGmkTruckDispFunc
/*!
 *	ギミック トロッコ 描画
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 */
// ==========================================================================
void gmGmkTruckDispFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_TRUCK_WORK	*truck_work;
	VecFx32				pos;
	VecU16				dir;
//	fx32				ofst_x, ofst_y;
	u32					disp_flag;

	truck_work = (GMS_GMK_TRUCK_WORK*)obj_work;

	// 本体描画
	ObjDrawActionSummary(obj_work);

	// 車輪描画
#if 1
	dir = obj_work->dir;
	disp_flag = obj_work->disp_flag | OBD_DISP_USERMTX_RIGHT;

	// 回転
	nnMakeRotateXYZMatrix(&truck_work->obj_3d_tire.user_obj_mtx_r, truck_work->tire_dir/*Z軸*/, truck_work->slope_f_y_dir, truck_work->slope_f_z_dir);

	// 前輪
	pos.x = FXM_FLOAT_TO_FX32(NNM_MTX(truck_work->tire_pos_f, 0, 3));
	pos.y = FXM_FLOAT_TO_FX32(-NNM_MTX(truck_work->tire_pos_f, 1, 3));
	pos.z = FXM_FLOAT_TO_FX32(NNM_MTX(truck_work->tire_pos_f, 2, 3));

	ObjDrawAction3DNN(&truck_work->obj_3d_tire, &pos, &dir, &obj_work->scale, &disp_flag);

	// 後輪
	pos.x = FXM_FLOAT_TO_FX32(NNM_MTX(truck_work->tire_pos_b, 0, 3));
	pos.y = FXM_FLOAT_TO_FX32(-NNM_MTX(truck_work->tire_pos_b, 1, 3));
	pos.z = FXM_FLOAT_TO_FX32(NNM_MTX(truck_work->tire_pos_b, 2, 3));

	ObjDrawAction3DNN(&truck_work->obj_3d_tire, &pos, &dir, &obj_work->scale, &disp_flag);
#else
	dir = obj_work->dir;
	disp_flag = obj_work->disp_flag | OBD_DISP_USERMTX_RIGHT;

	// 回転
	//nnMakeUnitMatrix(&obj_3d->user_obj_mtx_r);
	nnMakeRotateXYZMatrix(&truck_work->obj_3d_tire.user_obj_mtx_r, truck_work->tire_dir/*Z軸*/, 0, 0);

	// 前輪
	ObjUtilGetRotPosXY(GMD_GMK_TRUCK_TIRE_FRONT_OFST_X, GMD_GMK_TRUCK_TIRE_FRONT_OFST_Y,
			&ofst_x, &ofst_y, dir.z);
	pos.x = obj_work->pos.x + ofst_x + obj_work->ofst.x;
	pos.y = obj_work->pos.y + ofst_y + obj_work->ofst.y;
	pos.z = obj_work->pos.z + GMD_GMK_TRUCK_TIRE_FRONT_OFST_Z + obj_work->ofst.z;

	ObjDrawAction3DNN(&truck_work->obj_3d_tire, &pos, &dir, &obj_work->scale, &disp_flag);

	// 後輪
	ObjUtilGetRotPosXY(GMD_GMK_TRUCK_TIRE_BACK_OFST_X, GMD_GMK_TRUCK_TIRE_BACK_OFST_Y,
			&ofst_x, &ofst_y, dir.z);
	pos.x = obj_work->pos.x + ofst_x + obj_work->ofst.x;
	pos.y = obj_work->pos.y + ofst_y + obj_work->ofst.y;
	pos.z = obj_work->pos.z + GMD_GMK_TRUCK_TIRE_BACK_OFST_Z + obj_work->ofst.z;

	ObjDrawAction3DNN(&truck_work->obj_3d_tire, &pos, &dir, &obj_work->scale, &disp_flag);
#endif
//	// 逆重力対応
//	if (obj_work->dir_fall) {
//		dir.z += obj_work->dir_fall;
//	}

}

// ==========================================================================
// gmGmkTruckBodyDefFunc
/*!
 *	ギミック トロッコ 矩形 BODY くらい処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 */
// ==========================================================================
void gmGmkTruckBodyDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_ENEMY_COM_WORK	*ene_com = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;
	GMS_GMK_TRUCK_WORK	*truck_work;

	if (ene_com == NULL) {
		return;
	}
	if (ply_work == NULL || ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		return;
	}

	truck_work = (GMS_GMK_TRUCK_WORK*)ene_com;

	// クリッピングOFF
	ene_com->obj_work.flag |= OBD_OBJECT_NOCLIP;

	// プレイヤーギミックシーケンス発動
	//GmPlySeqInitTruck(ply_work, obj_work);

	// プレイヤーをトロッコ状態に
	GmPlayerSetTruckRide(ply_work, &ene_com->obj_work,
			ene_com->obj_work.field_rect[MTD_LEFT], ene_com->obj_work.field_rect[MTD_TOP],
			ene_com->obj_work.field_rect[MTD_RIGHT], ene_com->obj_work.field_rect[MTD_BOTTOM]);

	// ターゲットプレイヤー保存
	truck_work->target_player = ply_work;

	// ギミックトロッコ発動
	gmGmkTruckInitMain(&ene_com->obj_work, ply_work);

}

// ==========================================================================
// モーションコールバック
// ==========================================================================
// ==========================================================================
// gmGmkTruckMotionCallback
/*!
 *	ギミック トロッコ モーションコールバック
 *
 *	@param motion	[in] モーション
 *	@param object	[in] オブジェクト
 *	@param param	[in] パラメータ
 */
// ==========================================================================
#define GMD_GMK_TRUCK_NODE_ID_TIRE_POS_F		(2)
#define GMD_GMK_TRUCK_NODE_ID_TIRE_POS_B		(3)
#define GMD_GMK_TRUCK_NODE_ID_LIGHT_POS			(6)
void gmGmkTruckMotionCallback(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param)
{
	/* Node 2 : tire_pos_f */
	/* Node 3 : tire_pos_b */
	/* Node 8 : light_pos */

	NNS_MATRIX			node_mtx, base_mtx;
	GMS_GMK_TRUCK_WORK	*truck_work = (GMS_GMK_TRUCK_WORK*)param;

	// ベースマトリクス取得
	nnMakeUnitMatrix(&base_mtx);
	nnMultiplyMatrix(&base_mtx, &base_mtx, amMatrixGetCurrent());
	
	// 階層マトリクスを求める
	// tire_pos_f
	nnCalcNodeMatrixTRSList(&node_mtx, object, 
			GMD_GMK_TRUCK_NODE_ID_TIRE_POS_F, 
			motion->data, &base_mtx);
	MI_CpuCopy8(&node_mtx, &truck_work->tire_pos_f, sizeof(NNS_MATRIX));

	// tire_pos_b
	nnCalcNodeMatrixTRSList(&node_mtx, object, 
			GMD_GMK_TRUCK_NODE_ID_TIRE_POS_B, 
			motion->data, &base_mtx);
	MI_CpuCopy8(&node_mtx, &truck_work->tire_pos_b, sizeof(NNS_MATRIX));

	// light_pos
	nnCalcNodeMatrixTRSList(&node_mtx, object, 
			GMD_GMK_TRUCK_NODE_ID_LIGHT_POS, 
			motion->data, &base_mtx);
	MI_CpuCopy8(&node_mtx, &truck_work->light_pos, sizeof(NNS_MATRIX));
}

// ==========================================================================
// gmGmkTruckSetMoveSeParam
/*!
 *	ギミック トロッコ 移動SEパラメータ設定
 *
 *	@param	obj_work	[in]	トロッコオブジェクトワーク
 *	@param	h_snd		[in]	サウンドハンドル
 *	@param	ply_work	[in]	プレイヤー(NULL可)
 *	@param	b_goal		[in]	ゴール状態
 *
 *	@note
 */
// ==========================================================================
void gmGmkTruckSetMoveSeParam(OBS_OBJECT_WORK *obj_work, GSS_SND_SE_HANDLE *h_snd, GMS_PLAYER_WORK *ply_work, BOOL b_goal)
{
	CriFloat32			ctrl_val = 0.f;
	fx32				abs_spd_m;
	OBS_OBJECT_WORK		*se_target_obj;

	MTM_ASSERT(obj_work);

	if (!h_snd) {
		return;
	}
	
	if (ply_work) {
		se_target_obj = &ply_work->obj_work;
	}
	else {
		se_target_obj = obj_work;
	}

	abs_spd_m = MTM_MATH_ABS(se_target_obj->spd_m);
	
	if ((se_target_obj->move_flag & OBD_MOVE_UNDER) &&
			abs_spd_m >= GMD_GMK_TRUCK_SE_MIN_SPD) {
		if (abs_spd_m >= GMD_GMK_TRUCK_SE_MAX_SPD) {
			ctrl_val = 1.0f;
		}
		else {
			ctrl_val = FXM_FX32_TO_FLOAT(FX_Div(abs_spd_m - GMD_GMK_TRUCK_SE_MIN_SPD,
										GMD_GMK_TRUCK_SE_MAX_SPD - GMD_GMK_TRUCK_SE_MIN_SPD));
			if (ctrl_val > 1.0f) {
				ctrl_val = 1.0f;
			}
		}
	}
	h_snd->au_player->SetAisac("Speed", ctrl_val);

	if (b_goal) {
		float		volume = 1.0f;
		float		dist_x, dist_y, dist;
		OBS_CAMERA	*camera = ObjCameraGet(g_obj.glb_camera_id);

		dist_x = (FXM_FX32_TO_FLOAT(se_target_obj->pos.x) - camera->disp_pos.x);
		dist_y = (FXM_FX32_TO_FLOAT(se_target_obj->pos.y) - (-camera->disp_pos.y));

		if (dist_x < GMD_GMK_TRUCK_SE_GOAL_MAX_DIST && dist_y < GMD_GMK_TRUCK_SE_GOAL_MAX_DIST) {
			dist = dist_x*dist_x + dist_y*dist_y;
			if (dist <= GMD_GMK_TRUCK_SE_GOAL_MIN_DIST*GMD_GMK_TRUCK_SE_GOAL_MIN_DIST) {
				volume = 1.0f;
			}
			else if (dist <= GMD_GMK_TRUCK_SE_GOAL_MAX_DIST*GMD_GMK_TRUCK_SE_GOAL_MAX_DIST) {
				volume = (GMD_GMK_TRUCK_SE_GOAL_MAX_DIST*GMD_GMK_TRUCK_SE_GOAL_MAX_DIST - dist) /
								((GMD_GMK_TRUCK_SE_GOAL_MAX_DIST - GMD_GMK_TRUCK_SE_GOAL_MIN_DIST)*(GMD_GMK_TRUCK_SE_GOAL_MAX_DIST - GMD_GMK_TRUCK_SE_GOAL_MIN_DIST));
				if (volume > 1.0f) {
					volume = 1.0f;
				}
				else if (volume < 0.0f) {
					volume = 0.0f;
				}
			}
			else {
				volume = 0.0f;
			}
		}
		else {
			volume = 0.0f;
		}

		h_snd->snd_ctrl_param.volume = volume;
	}
}

// ==========================================================================
// トロッコ重力
// ==========================================================================
// ==========================================================================
// gmGmkTGravityChangeDefFunc
/*!
 *	ギミック トロッコ重力 矩形 BODY くらい処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 */
// ==========================================================================
/// 平地タイプ落下角度テーブル
static const u16 gm_gmk_t_gravity_flat_dir_tbl[] = {
	0x0000,		// GMD_EVENT_ID_GMK_T_GRAVITY_00_00
	0x4000,		// GMD_EVENT_ID_GMK_T_GRAVITY_00_40
	0x8000,		// GMD_EVENT_ID_GMK_T_GRAVITY_00_80
	0xC000,		// GMD_EVENT_ID_GMK_T_GRAVITY_00_C0

	0x1300,		// GMD_EVENT_ID_GMK_T_GRAVITY_30_20
	0x6D00,		// GMD_EVENT_ID_GMK_T_GRAVITY_30_60
	0x9300,		// GMD_EVENT_ID_GMK_T_GRAVITY_30_A0
	0xED00,		// GMD_EVENT_ID_GMK_T_GRAVITY_30_E0

	0x1F00,		// GMD_EVENT_ID_GMK_T_GRAVITY_45_20
	0x6100,		// GMD_EVENT_ID_GMK_T_GRAVITY_45_60
	0x9F00,		// GMD_EVENT_ID_GMK_T_GRAVITY_45_A0
	0xE100,		// GMD_EVENT_ID_GMK_T_GRAVITY_45_E0

	0x2D00,		// GMD_EVENT_ID_GMK_T_GRAVITY_60_20
	0x5300,		// GMD_EVENT_ID_GMK_T_GRAVITY_60_60
	0xAD00,		// GMD_EVENT_ID_GMK_T_GRAVITY_60_A0
	0xD300,		// GMD_EVENT_ID_GMK_T_GRAVITY_60_E0
};
void gmGmkTGravityChangeDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_ENEMY_COM_WORK	*ene_com = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;
	u16					dir;
	float				dist_x, dist_y;
	s32					r_type;
	fx32				r_c_pos_x, r_c_pos_y;

	if (ene_com == NULL) {
		return;
	}
	if (ply_work == NULL ||											// 相手矩形の親がない
			ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {	// 親がプレイヤーでない
		return;
	}

	if (((ene_com->eve_rec->flag & GMD_GMK_T_GRAVITY_A) && (ply_work->obj_work.flag & OBD_OBJECT_B)) ||
			((ene_com->eve_rec->flag & GMD_GMK_T_GRAVITY_B) && !(ply_work->obj_work.flag & OBD_OBJECT_B))) {
		// 影響範囲面にいない
		return;
	}

	// 落下重力方向を設定
	if (GMD_EVENT_ID_GMK_T_GRAVITY_R_20 <= ene_com->eve_rec->id &&
			ene_com->eve_rec->id <= GMD_EVENT_ID_GMK_T_GRAVITY_RR_E0) {

		if (GMD_EVENT_ID_GMK_T_GRAVITY_R_20 <= ene_com->eve_rec->id &&
				ene_com->eve_rec->id <= GMD_EVENT_ID_GMK_T_GRAVITY_R_E0) {
		// Rタイプ		ギミックから放射状に落下
			r_type = ene_com->eve_rec->id - GMD_EVENT_ID_GMK_T_GRAVITY_R_20;

			// 基点座標を取得
			if (r_type & 0x02) {
				r_c_pos_x = ene_com->obj_work.pos.x + 128*FX32_ONE;
			}
			else {
				r_c_pos_x = ene_com->obj_work.pos.x - 128*FX32_ONE;
			}
			if ((r_type + 1) & 0x02) {
				r_c_pos_y = ene_com->obj_work.pos.y - 128*FX32_ONE;
			}
			else {
				r_c_pos_y = ene_com->obj_work.pos.y + 128*FX32_ONE;
			}

			// R地形範囲にいるフラグセット
			ply_work->gmk_flag2 |= GMD_PLGF2_TRUCK_R_AREA;
		}
		else {
		// 逆Rタイプ	ギミック方向へ落下
			r_type = ene_com->eve_rec->id - GMD_EVENT_ID_GMK_T_GRAVITY_RR_20;

			// 基点座標を取得
			if (r_type & 0x02) {
				r_c_pos_x = ene_com->obj_work.pos.x - 128*FX32_ONE;
			}
			else {
				r_c_pos_x = ene_com->obj_work.pos.x + 128*FX32_ONE;
			}
			if ((r_type + 1) & 0x02) {
				r_c_pos_y = ene_com->obj_work.pos.y + 128*FX32_ONE;
			}
			else {
				r_c_pos_y = ene_com->obj_work.pos.y - 128*FX32_ONE;
			}
		}

		dist_x = FXM_FX32_TO_FLOAT(r_c_pos_x - ply_work->obj_work.pos.x);
		dist_y = FXM_FX32_TO_FLOAT(r_c_pos_y - ply_work->obj_work.pos.y);

		// 基点範囲内チェック

		if (GMD_EVENT_ID_GMK_T_GRAVITY_R_20 <= ene_com->eve_rec->id &&
				ene_com->eve_rec->id <= GMD_EVENT_ID_GMK_T_GRAVITY_R_E0) {
		// Rタイプ
			if (r_type & 0x02) {
				if (dist_x < 0) {
					// 無効範囲
					return;
				}
			}
			else {
				if (dist_x > 0) {
					// 無効範囲
					return;
				}
			}
			if ((r_type + 1) & 0x02) {
				if (dist_y > 0) {
					// 無効範囲
					return;
				}
			}
			else {
				if (dist_y < 0) {
					// 無効範囲
					return;
				}
			}
		}
		else {
		// 逆Rタイプ
			if (r_type & 0x02) {
				if (dist_x > 0) {
					// 無効範囲
					return;
				}
			}
			else {
				if (dist_x < 0) {
					// 無効範囲
					return;
				}
			}
			if ((r_type + 1) & 0x02) {
				if (dist_y < 0) {
					// 無効範囲
					return;
				}
			}
			else {
				if (dist_y > 0) {
					// 無効範囲
					return;
				}
			}
		}

		// 角度取得
		dir = (u16)(nnArcTan2(-dist_y, dist_x) - 0x4000);

		dir = (u16)(0x10000 - dir);
		if (GMD_EVENT_ID_GMK_T_GRAVITY_R_20 <= ene_com->eve_rec->id &&
				ene_com->eve_rec->id <= GMD_EVENT_ID_GMK_T_GRAVITY_R_E0) {
			dir -= 0x8000;
		}
	}
	else {
		// その他
		// 固定方向
		MTM_ASSERT(GMD_EVENT_ID_GMK_T_GRAVITY_00_00 <= ene_com->eve_rec->id);
		MTM_ASSERT(ene_com->eve_rec->id <= GMD_EVENT_ID_GMK_T_GRAVITY_60_E0);
		dir = gm_gmk_t_gravity_flat_dir_tbl[ene_com->eve_rec->id - GMD_EVENT_ID_GMK_T_GRAVITY_00_00];
	}

	if (ply_work->jump_pseudofall_eve_id_cur != ene_com->eve_rec->id) {
		// 現在有効なタイプと違う為、すぐには切り替えない
		ply_work->jump_pseudofall_eve_id_wait = ene_com->eve_rec->id;	// 待機
		return;
	}

	// プレイヤーの擬似重力固定を解除
	if (ene_com->eve_rec->flag & GMD_GMK_T_CLEAR_PSEUDOFALL_DIR_FIX) {
		ply_work->gmk_flag &= ~GMD_PLGF_GMK_JUMP_PSEUDOFALL_DIR_FIX;
	}

	if (ply_work->gmk_flag & GMD_PLGF_GMK_JUMP_PSEUDOFALL_DIR_FIX) {
		// 擬似ジャンプ重力固定中
		return;
	}

	// 速度を変更
	ObjObjectSpdDirFall(&ply_work->obj_work.spd.x, &ply_work->obj_work.spd.y, (u16)-(dir - ply_work->jump_pseudofall_dir));

	// ジャンプ時重力方向保存
	ply_work->jump_pseudofall_dir = dir;
	// セットしたイベントIDを格納(セット済み告知)
	ply_work->jump_pseudofall_eve_id_set = ene_com->eve_rec->id;
}


// ==========================================================================
// 重力強制変換
// ==========================================================================
// ==========================================================================
// gmGmkTGravityForceChangeDefFunc
/*!
 *	ギミック 重力強制変換 矩形 BODY くらい処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 */
// ==========================================================================
void gmGmkTGravityForceChangeDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
#if (_IPHONE & 0)
	UNREFERENCED_PARAMETER(mine_rect);
	UNREFERENCED_PARAMETER(match_rect);
#else
	GMS_ENEMY_COM_WORK	*ene_com = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	Angle32				fall_dir, dir_dist;
	u16					jump_dir_sub;

	if (ene_com == NULL) {
		return;
	}
	if (ply_work == NULL ||											// 相手矩形の親がない
			ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER ||	// 親がプレイヤーでない
			(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {		// プレイヤーが接地している
		return;
	}

	if (((ene_com->eve_rec->flag & GMD_GMK_T_GRAVITY_A) && (ply_work->obj_work.flag & OBD_OBJECT_B)) ||
			((ene_com->eve_rec->flag & GMD_GMK_T_GRAVITY_B) && !(ply_work->obj_work.flag & OBD_OBJECT_B))) {
		// 影響範囲面にいない
		return;
	}

	// 重力方向取得
	fall_dir = (ene_com->eve_rec->id - GMD_EVENT_ID_GMK_T_FC_GRAVITY_D) * 0x4000;
	
	// ジャンプ重力差分取得
	jump_dir_sub = (u16)(fall_dir - ply_work->jump_pseudofall_dir);

	// 重力方向変更
	// ジャンプ時重力方向変更
	ply_work->jump_pseudofall_dir = (u16)fall_dir;
	// 擬似重力方向変更
	dir_dist = fall_dir - ply_work->ply_pseudofall_dir;
	if ((u16)(MTM_MATH_ABS(dir_dist)) > 0x8000) {
		if (dir_dist < 0) {
			ply_work->ply_pseudofall_dir += 0x10000 + dir_dist;
		}
		else {
			ply_work->ply_pseudofall_dir += dir_dist - 0x10000;
		}
	}
	else { 
		ply_work->ply_pseudofall_dir = fall_dir;
	}

	// ジャンプ速度変換
	ObjObjectSpdDirFall(&ply_work->obj_work.spd.x, &ply_work->obj_work.spd.y, (u16)-jump_dir_sub);

	// 接地まで擬似ジャンプ重力固定
	ply_work->gmk_flag |= GMD_PLGF_GMK_JUMP_PSEUDOFALL_DIR_FIX;
#endif // _IPHONE
}

// ==========================================================================
// トロッコ接地無効
// ==========================================================================
// ==========================================================================
// gmGmkTNoLandingDefFunc
/*!
 *	ギミック トロッコ接地無効 矩形 BODY くらい処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 */
// ==========================================================================
void gmGmkTNoLandingDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_ENEMY_COM_WORK	*ene_com = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	if (ene_com == NULL) {
		return;
	}
	if (ply_work == NULL ||											// 相手矩形の親がない
			ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {	// 親がプレイヤーでない
		return;
	}

	// 接地不可フラグON
	ply_work->obj_work.sys_flag |= OBD_SYSF_NOLANDING_UNDER << (ene_com->eve_rec->id - GMD_EVENT_ID_GMK_T_NO_LANDING_D);
}


// ==========================================================================
// gmGmkTruckCreateLightEfct
/*!
 *	ギミック トロッコ ライトエフェクト生成
 *
 *	@param truck_work	[in] トロッコワーク
 */
// ==========================================================================
void gmGmkTruckCreateLightEfct(GMS_GMK_TRUCK_WORK *truck_work)
{
	// ライトエフェクト生成
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctZoneEsCreate((OBS_OBJECT_WORK*)truck_work, GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_LIGHT_01);

	// 接着用matrixアドレス保存
	efct_work->efct_com.obj_work.user_work = (u32)&truck_work->light_pos;
	
	// メイン処理差し替え
	efct_work->efct_com.obj_work.ppFunc = gmGmkTruckLightEfctMain;

	// 描画処理差し替え
	efct_work->efct_com.obj_work.ppOut = gmGmkTruckLightEfctDispFunc;
	// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
}

// ==========================================================================
// gmGmkTruckLightEfctMain
/*!
 *	ギミック トロッコ ライトエフェクト
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_work	: NNS_MATRIX*
 */
// ==========================================================================
void gmGmkTruckLightEfctMain(OBS_OBJECT_WORK *obj_work)
{
#if 1
//	NNS_MATRIX		*mtx = (NNS_MATRIX*)obj_work->user_work;
	GMS_GMK_TRUCK_WORK	*truck_work;

//	MTM_ASSERT(mtx);

	if (obj_work->parent_obj == NULL) {
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

	truck_work = (GMS_GMK_TRUCK_WORK*)obj_work->parent_obj;
	obj_work->dir.z = (u16)(obj_work->parent_obj->dir.z + truck_work->slope_z_dir);

	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}

//	obj_work->pos.x = FXM_FLOAT_TO_FX32(NNM_MTX(*mtx, 0, 3));
//	obj_work->pos.y = FXM_FLOAT_TO_FX32(-NNM_MTX(*mtx, 1, 3));
//	obj_work->pos.z = FXM_FLOAT_TO_FX32(NNM_MTX(*mtx, 2, 3));

//	// 汎用処理
//	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);

#else
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

	GmComEfctSetDispOffsetF((GMS_EFFECT_3DES_WORK*)obj_work,
				vec.x, vec.y, vec.z + 32.f);

	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
#endif
}

// ==========================================================================
// gmGmkTruckLightEfctDispFunc
/*!
 *	ギミック トロッコ ライトエフェクト 描画処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_work	: NNS_MATRIX*
 *		座標の更新が描画処理内で行われているが、親オブジェクト参照による
 *		位置変更なので、問題無しとする
 */
// ==========================================================================
void gmGmkTruckLightEfctDispFunc(OBS_OBJECT_WORK *obj_work)
{
	NNS_MATRIX		*mtx = (NNS_MATRIX*)obj_work->user_work;

	MTM_ASSERT(mtx);

	if (obj_work->parent_obj == NULL) {
		return;
	}

	obj_work->pos.x = FXM_FLOAT_TO_FX32(NNM_MTX(*mtx, 0, 3));
	obj_work->pos.y = FXM_FLOAT_TO_FX32(-NNM_MTX(*mtx, 1, 3));
	obj_work->pos.z = FXM_FLOAT_TO_FX32(NNM_MTX(*mtx, 2, 3));


	// 標準描画処理
	ObjDrawActionSummary(obj_work);
}

// ==========================================================================
// エフェクト
// ==========================================================================
// ==========================================================================
// gmGmkTruckCreateSparkEfct
/*!
 *	ギミック トロッコ 火花エフェクト生成
 *
 *	@param	truck_work	[in]	トロッコワーク
 *	@param	type		[in]	エフェクトタイプ GME_EFCT_Z03_IDX_SPARK or GME_EFCT_Z03_IDX_SPARK_S
 */
// ==========================================================================
void gmGmkTruckCreateSparkEfct(GMS_GMK_TRUCK_WORK *truck_work, s32 efct_type)
{
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)

	MTM_ASSERT(efct_type == GME_EFCT_Z03_IDX_SPARK || efct_type == GME_EFCT_Z03_IDX_SPARK_S);

	// 火花エフェクト生成
	if (!truck_work->efct_f_spark) {
		truck_work->efct_f_spark = GmEfctZoneEsCreate((OBS_OBJECT_WORK*)truck_work,
										 GSD_MAIN_ZONE_TYPE_3, efct_type);
		//GmComEfctAddDispOffsetF(sting_work->efct_r_jet, -11.f, -9.f, 0);
		truck_work->efct_f_spark->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_FIX_NODISP | OBD_OBJECT_NOCLIP;
		truck_work->efct_f_spark->efct_com.obj_work.user_work = (u32)&truck_work->tire_pos_f;
		truck_work->efct_f_spark->efct_com.obj_work.user_timer = efct_type;
		truck_work->efct_f_spark->efct_com.obj_work.user_flag = 0;		// 前輪
		// メイン処理差し替え
		truck_work->efct_f_spark->efct_com.obj_work.ppFunc	= gmGmkTruckSparkEfctMain;
		truck_work->efct_f_spark->efct_com.obj_work.ppOut	= gmGmkTruckSparkEfctDispFunc;
		// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	}
	if (!truck_work->efct_b_spark) {
		truck_work->efct_b_spark = GmEfctZoneEsCreate((OBS_OBJECT_WORK*)truck_work,
										 GSD_MAIN_ZONE_TYPE_3, efct_type);
		//GmComEfctAddDispOffsetF(sting_work->efct_r_jet, -11.f, -9.f, 0);
		truck_work->efct_b_spark->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_FIX_NODISP | OBD_OBJECT_NOCLIP;
		truck_work->efct_b_spark->efct_com.obj_work.user_work = (u32)&truck_work->tire_pos_b;
		truck_work->efct_b_spark->efct_com.obj_work.user_timer = efct_type;
		truck_work->efct_b_spark->efct_com.obj_work.user_flag = 1;		// 後輪
		// メイン処理差し替え
		truck_work->efct_b_spark->efct_com.obj_work.ppFunc	= gmGmkTruckSparkEfctMain;
		truck_work->efct_b_spark->efct_com.obj_work.ppOut	= gmGmkTruckSparkEfctDispFunc;
		// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
}

#if 0
// ==========================================================================
// gmGmkTruckClearSparkEfct
/*!
 *	ギミック トロッコ 火花エフェクト破棄
 *
 *	@param sting_work	[in] スティンガーワーク
 */
// ==========================================================================
void gmGmkTruckClearSparkEfct(GMS_ENE_STING_WORK *sting_work)
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
}
#endif

// ==========================================================================
// gmGmkTruckSparkEfctMain
/*!
 *	ギミック トロッコ 火花エフェクト
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_work	: GMS_GMK_TRUCK_WORK*
 *		user_timer	: エフェクトタイプ保存 GME_EFCT_Z03_IDX_SPARK_S or GME_EFCT_Z03_IDX_SPARK
 *		user_flag	: 0:前輪   1:後輪
 */
// ==========================================================================
void gmGmkTruckSparkEfctMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_TRUCK_WORK	*truck_work = (GMS_GMK_TRUCK_WORK*)obj_work->parent_obj;
	OBS_OBJECT_WORK		*move_target_obj;
//	NNS_MATRIX			*mtx = (NNS_MATRIX*)obj_work->user_work;
//	VecFx32				vec;
//	NNS_VECTOR			ofst_vec;
//	u16					dir, ofst_dir;
	u32					prev_disp_flag;

	MTM_ASSERT(truck_work);
//	MTM_ASSERT(mtx);

	if (obj_work->parent_obj == NULL) {
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

	move_target_obj = (OBS_OBJECT_WORK*)truck_work->target_player;
	if (move_target_obj == NULL) {
		move_target_obj = (OBS_OBJECT_WORK*)truck_work;
	}

	// 前の表示フラグ取得
	prev_disp_flag = obj_work->disp_flag;

	if (!(move_target_obj->move_flag & (OBD_MOVE_UNDER))) {
		// 接地していないときは描画OFF
		obj_work->disp_flag |= OBD_DISP_NODISP;
		return;
	}
	obj_work->disp_flag &= ~OBD_DISP_NODISP;


	// 速度で切り替え
#if !_IPHONE
	if (obj_work->user_timer == GME_EFCT_Z03_IDX_SPARK) {
	// エフェクト大
		if (MTM_MATH_ABS(move_target_obj->spd_m) < GMD_GMK_TRUCK_SPARK_EFCT_BIG_MIN_SPD) {
			if (prev_disp_flag & OBD_DISP_NODISP) {
				// 前フレームで非表示だった場合は即時削除
				obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
			}
			else {
				// 終了
				ObjDrawKillAction3DES(obj_work);

				// 終了処理に任せる
				obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEnd;
			}

			if (obj_work->user_flag == 0) {
				// 前輪
				truck_work->efct_f_spark = NULL;
			}
			else {
				// 後輪
				truck_work->efct_b_spark = NULL;
			}

			// 小タイプエフェクトを作る
			gmGmkTruckCreateSparkEfct(truck_work, GME_EFCT_Z03_IDX_SPARK_S);
		}
	}
	else
#endif // !_IPHONE
	{
	// エフェクト小
#if _IPHONE
		if (MTM_MATH_ABS(move_target_obj->spd_m) < GMD_GMK_TRUCK_SPARK_EFCT_SMALL_MIN_SPD) {
#else
		if (MTM_MATH_ABS(move_target_obj->spd_m) >= GMD_GMK_TRUCK_SPARK_EFCT_BIG_MIN_SPD ||
					MTM_MATH_ABS(move_target_obj->spd_m) < GMD_GMK_TRUCK_SPARK_EFCT_SMALL_MIN_SPD) {
#endif 
			if (prev_disp_flag & OBD_DISP_NODISP) {
				// 前フレームで非表示だった場合は即時削除
				obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
			}
			else {
				// 終了
				ObjDrawKillAction3DES(obj_work);

				// 終了処理に任せる
				obj_work->ppFunc = GmEffectDefaultMainFuncDeleteAtEnd;
			}

			if (obj_work->user_flag == 0) {
				// 前輪
				truck_work->efct_f_spark = NULL;
			}
			else {
				// 後輪
				truck_work->efct_b_spark = NULL;
			}
#if !_IPHONE
			if (MTM_MATH_ABS(move_target_obj->spd_m) >= GMD_GMK_TRUCK_SPARK_EFCT_BIG_MIN_SPD) {
				// 大タイプエフェクトを作る
				gmGmkTruckCreateSparkEfct(truck_work, GME_EFCT_Z03_IDX_SPARK);
			}
#endif // !_IPHONE
		}
	}


#if 0
	// 表示位置設定
	vec.x = FXM_FLOAT_TO_FX32(NNM_MTX(*mtx, 0, 3));
	vec.y = FXM_FLOAT_TO_FX32(-NNM_MTX(*mtx, 1, 3));
	vec.z = FXM_FLOAT_TO_FX32(NNM_MTX(*mtx, 2, 3));

	if (move_target_obj->spd_m >= 0) {
		// 前方移動
		obj_work->disp_flag &= ~OBD_DISP_HFLIP;
		//obj_work->dir.z = obj_work->parent_obj->dir.z;
		//obj_work->dir.z = test_dir;
		dir		= (u16)(0x2000 - obj_work->parent_obj->dir.z);
		ofst_dir= (u16)(-obj_work->parent_obj->dir.z - 0x0800);
	}
	else {
		// 後方移動
		obj_work->disp_flag |= OBD_DISP_HFLIP;
		//obj_work->dir.z = (u16)(0x10000-obj_work->parent_obj->dir.z);
		//obj_work->dir.z = (u16)-test_dir;
		dir		= (u16)(0x2000 + obj_work->parent_obj->dir.z);
		ofst_dir= (u16)(obj_work->parent_obj->dir.z + 0x0800);
	}
	obj_work->pos = vec;

	dir += test_dir;


	ofst_vec.x = nnSin(ofst_dir) * GMD_GMK_TRUCK_EFCT_SPRAK_OFST_DIST;
	ofst_vec.y = nnCos(ofst_dir) * GMD_GMK_TRUCK_EFCT_SPRAK_OFST_DIST;
	ofst_vec.z = 0.0f;
	GmComEfctSetDispOffsetF((GMS_EFFECT_3DES_WORK*)obj_work,
					ofst_vec.x, ofst_vec.y, ofst_vec.z);

	GmComEfctSetDispRotation((GMS_EFFECT_3DES_WORK*)obj_work, 0, 0, dir);
	//GmComEfctSetDispRotation((GMS_EFFECT_3DES_WORK*)obj_work, 0, 0, (u16)(test_dir + 0x2000));

	//obj_work->dir.z = (u16)-obj_work->parent_obj->dir.z;
#endif

	// 汎用処理
	//GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
	GmEffectDefaultMainFuncDeleteAtEnd(obj_work);	// Zはこちらで設定する
}

// ==========================================================================
// gmGmkTruckSparkEfctDispFunc
/*!
 *	ギミック トロッコ 火花エフェクト 描画処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_work	: GMS_GMK_TRUCK_WORK*
 *		座標の更新が描画処理内で行われているが、親オブジェクト参照による
 *		位置変更なので、問題無しとする
 */
// ==========================================================================
void gmGmkTruckSparkEfctDispFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_TRUCK_WORK	*truck_work = (GMS_GMK_TRUCK_WORK*)obj_work->parent_obj;
	OBS_OBJECT_WORK		*move_target_obj;
	NNS_MATRIX			*mtx = (NNS_MATRIX*)obj_work->user_work;
	VecFx32				vec;
	NNS_VECTOR			ofst_vec;
	u16					dir, ofst_dir;

	MTM_ASSERT(truck_work);
	MTM_ASSERT(mtx);

	if (obj_work->parent_obj == NULL) {
		return;
	}

//	if (obj_work->disp_flag & OBD_DISP_NODISP) {
//		return;
//	}

	move_target_obj = (OBS_OBJECT_WORK*)truck_work->target_player;
	if (move_target_obj == NULL) {
		move_target_obj = (OBS_OBJECT_WORK*)truck_work;
	}

	// 表示位置設定
	vec.x = FXM_FLOAT_TO_FX32(NNM_MTX(*mtx, 0, 3));
	vec.y = FXM_FLOAT_TO_FX32(-NNM_MTX(*mtx, 1, 3));
	vec.z = FXM_FLOAT_TO_FX32(NNM_MTX(*mtx, 2, 3));

	if (move_target_obj->spd_m >= 0) {
		// 前方移動
		obj_work->disp_flag &= ~OBD_DISP_HFLIP;
		dir		= (u16)(0x2000 - obj_work->parent_obj->dir.z);
		ofst_dir= (u16)(-obj_work->parent_obj->dir.z - 0x0800);
	}
	else {
		// 後方移動
		obj_work->disp_flag |= OBD_DISP_HFLIP;
		dir		= (u16)(0x2000 + obj_work->parent_obj->dir.z);
		ofst_dir= (u16)(obj_work->parent_obj->dir.z + 0x0800);
	}
	obj_work->pos = vec;

	// オフセット位置設定
	ofst_vec.x = nnSin(ofst_dir) * GMD_GMK_TRUCK_EFCT_SPRAK_OFST_DIST;
	ofst_vec.y = nnCos(ofst_dir) * GMD_GMK_TRUCK_EFCT_SPRAK_OFST_DIST;
	ofst_vec.z = 0.0f;
	GmComEfctSetDispOffsetF((GMS_EFFECT_3DES_WORK*)obj_work,
					ofst_vec.x, ofst_vec.y, ofst_vec.z);

	// 回転量設定
	GmComEfctSetDispRotation((GMS_EFFECT_3DES_WORK*)obj_work, 0, 0, dir);

	// 標準描画処理
	ObjDrawActionSummary(obj_work);
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
