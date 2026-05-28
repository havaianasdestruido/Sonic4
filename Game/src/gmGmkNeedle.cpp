// ===========================================================================
/*!
	@file	gmGmkNeedle.cpp
	@brief	ギミック トゲ

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: gmGmkNeedle.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmPlayer.h"
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmSound.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

#include "gmGmkNeedle.h"

// データヘッダ
#include "common/model/gmk_needle_mdl.hmb"

#if _IPHONE
// commonと構成が変わっている場合に使用する定義
#include "iPhone/model/GMK_NEEDLE_TVX.HMB"
#endif // _IPHONE


// ----- Macros ------------------------------------------------（マクロ定義）
// イベントデータのフラグ関連
// #define GMD_GMK_NEEDLE_

#define GMD_GMK_NEEDLE_TEST_TVX       (1 & _IPHONE)

// その他汎用マクロ
#define GMD_GMK_NEEDLE_NUM			(5)				//!< 一塊分のトゲの本数
#define GMD_GMK_COL_ROTATE_DIR		(0x4000)		//!< コリジョン用回転角度
#define GMD_GMK_COL_ACT_ROTATE_DIR	(0x8000)		//!< コリジョン用回転角度(出入りトゲ用)
#define GMD_GMK_DRAW_ROTATE_DIR		(0xc000)		//!< 描画用回転角度

// 出入りトゲ用マクロ
#define GMD_GMK_ACT_NDL_SCALING_TIME	(6)		//!< トゲが出入りする際のスケーリングにかかる時間

#if 0
#define GMD_GMK_ACT_NDL_WAITING_TIME	(60 * 2)		//!< トゲが出たまま、または入ったままの状態の時間
#else
#define GMD_GMK_ACT_NDL_WAITING_TIME	(60 * 1)		//!< トゲが出たまま、または入ったままの状態の時間
#endif

// 出入りトゲ用変動拡大率
#define GMD_GMK_ACT_NDL_SCALING_RATE	(FX32_ONE / GMD_GMK_ACT_NDL_SCALING_TIME)		//!< 

#define GMD_GMK_NEEDLE_TRUCK_ATK_RECT_ADJUST	(16)	//!< トロッコ用攻撃矩形補正値

#define GMD_GMK_ACT_NDL_COL_SCALE_EFCT	(1 << 0)
#define GMD_GMK_ACT_NDL_COL_SCALE_UP	(1 << 1)
#define GMD_GMK_ACT_NDL_COL_SCALE_DOWN	(1 << 2)

// ----- Macro Functions -----------------------------------（処理マクロ定義）

// ----- Definitions -------------------------------------------（定数の宣言）

typedef enum tag_GMD_GMK_NDL_TYPE {
	GMD_GMK_NDL_TYPE_UP	= 0,		//!< 上向き通常
	GMD_GMK_NDL_TYPE_LEFT,			//!< 左向き通常
	GMD_GMK_NDL_TYPE_DOWN,			//!< 下向き通常
	GMD_GMK_NDL_TYPE_RIGHT,			//!< 右向き通常
	GMD_GMK_NDL_TYPE_ACT_UP,		//!< 上向き通常
	GMD_GMK_NDL_TYPE_ACT_DOWN,		//!< 下向き通常

	GMD_GMK_NDL_TYPE_MAX
} GMD_GMK_NDL_TYPE;

typedef enum tag_GMD_GMK_NDL_INOUT_STATE {
	GMD_GMK_NDL_INOUT_STATE_OUT	= 0,	//!< 針が出る場合
	GMD_GMK_NDL_INOUT_STATE_IN,			//!< 針が入る場合

	GMD_GMK_NDL_INOUT_STATE_MAX
} GMD_GMK_NDL_INOUT_STATE;

typedef enum tag_GMD_GMK_NDL_COL_DATA {
	GMD_GMK_NDL_COL_DATA_WIDTH	= 0,		//!< 上向き通常
	GMD_GMK_NDL_COL_DATA_HEIGHT,			//!< 左向き通常
	GMD_GMK_NDL_COL_DATA_OFST_X,			//!< 下向き通常
	GMD_GMK_NDL_COL_DATA_OFST_Y,			//!< 右向き通常

	GMD_GMK_NDL_COL_DATA_MAX
} GMD_GMK_NDL_COL_DATA;


//! トゲワーク
typedef struct tag_GMS_GMK_NEEDLE_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< エネミー・ギミック共通ワーク
	
	// 他の表示のみのローカルイベントをエフェクト扱いにするかも
	s32							timer;			//!< タイマー
	u32							state;			//!< トゲの表示状態(出入り用)
	s32							scale_timer;	//!< 
	u32							scale_flag;		//!< 
	u16							needle_type;	//!< トゲの種類(方向・出入り)
	u16							is_first_disp;	//!< 
	
#if GMD_GMK_NEEDLE_TEST_TVX
	u32							color;			//!< カラー
#endif // GMD_GMK_NEEDLE_TEST_TVX
} GMS_GMK_NEEDLE_WORK;

// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
//static void gmGmkNeedleExit(OBS_OBJECT_WORK *obj_work);
static void gmGmkNeedleFwInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkNeedleFwMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkNeedleStandFwMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkNeedleDrawFunc(OBS_OBJECT_WORK *obj_work);

#if !GMD_GMK_NEEDLE_TEST_TVX
static void gmGmkBackNeedleDrawFunc(OBS_OBJECT_WORK *obj_work);
static void gmGmkStandNeedleDrawFunc(OBS_OBJECT_WORK *obj_work);
#endif // !GMD_GMK_NEEDLE_TEST_TVX

static u16 GmGmkNeedleGetType(u16 type);

//static void gmGmkNeedleStandExit(MTS_TASK_TCB *tcb);
//static void gmGmkNeedleBackExit(MTS_TASK_TCB *tcb);

static void gmGmkNeedleBackFwMain(OBS_OBJECT_WORK *obj_work);

static void gmGmkActNeedleFwInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkActNeedleFwMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkActNeedleScalingInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkActNeedleScalingMain(OBS_OBJECT_WORK *obj_work);

static void gmGmkActNeedleRectWaitInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkActNeedleRectWaitMain(OBS_OBJECT_WORK *obj_work);

static void gmGmkActNeedleSetScaleColRect(OBS_OBJECT_WORK *obj_work);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）
// 通常トゲタイプ
const static u32 gm_gmk_ndl_type_tbl[GMD_GMK_NDL_TYPE_MAX] = {
	GMD_GMK_NDL_TYPE_UP,			//!< 上向き通常
	GMD_GMK_NDL_TYPE_LEFT,			//!< 左向き通常
	GMD_GMK_NDL_TYPE_DOWN,			//!< 下向き通常
	GMD_GMK_NDL_TYPE_RIGHT,			//!< 右向き通常
	GMD_GMK_NDL_TYPE_ACT_UP,		//!< 上向き出入り
	GMD_GMK_NDL_TYPE_ACT_DOWN,		//!< 下向き出入り
};

// 攻撃矩形テーブル
const static s32 gm_gmk_atk_rect_tbl[GMD_GMK_NDL_TYPE_MAX][MTD_RECT] = {
	{-8, -33, 15, -8},			//!< 上向き通常
	{-37, -8, -8, 4},			//!< 左向き通常
	{-12, 32, 12, 8},			//!< 下向き通常
	{8, -6, 37, 4},				//!< 右向き通常
	
	{-8, -33, 15, -8},			//!< 上向き出入り
//	{-15, -36, 15, -2},			//!< 上向き出入り
	{-14, 2, 14, 32},			//!< 下向き出入り
};

// 地形矩形テーブル
const static s32 gm_gmk_col_rect_tbl[GMD_GMK_NDL_TYPE_MAX][GMD_GMK_NDL_COL_DATA_MAX] = {
	{24, 30, -8, -32},			//!< 上向き通常
	{40, 30, -36, -16},			//!< 左向き通常
	{32, 32, -16, 0},			//!< 下向き通常
	{40, 28, -4, -16},			//!< 右向き通常

	{24, 32, -8, -32},			//!< 上向き出入り
//	{32, 32, -16, -24},			//!< 上向き出入り
	{32, 32, -16, 0},			//!< 下向き出入り
};

// トゲ表示位置オフセットテーブル(上向き)
const static fx32 gm_gmk_disp_ofst_tbl_u[GMD_GMK_NEEDLE_NUM][MTD_XY] = {
	{0, 0},						//!< 中央
	{-10 * FX32_ONE, 0},		//!< 手前左
	{10 * FX32_ONE, 0},			//!< 手前右
	{-5 * FX32_ONE, 0},			//!< 奥左
	{5 * FX32_ONE, 0},			//!< 奥右
};
// トゲ表示位置オフセットテーブル(左向き)
const static fx32 gm_gmk_disp_ofst_tbl_l[GMD_GMK_NEEDLE_NUM][MTD_XY] = {
	{0, 0},						//!< 中央
	{0, 10 * FX32_ONE},			//!< 手前左
	{0, -10 * FX32_ONE},		//!< 手前右
	{0, 5 * FX32_ONE},			//!< 奥左
	{0, -5 * FX32_ONE},			//!< 奥右
};
// トゲ表示位置オフセットテーブル(下向き)
const static fx32 gm_gmk_disp_ofst_tbl_d[GMD_GMK_NEEDLE_NUM][MTD_XY] = {
	{0, 0},						//!< 中央
	{-10 * FX32_ONE, 0},		//!< 手前左
	{10 * FX32_ONE, 0},			//!< 手前右
	{-5 * FX32_ONE, 0},			//!< 奥左
	{5 * FX32_ONE, 0},			//!< 奥右
};
// トゲ表示位置オフセットテーブル(右向き)
const static fx32 gm_gmk_disp_ofst_tbl_r[GMD_GMK_NEEDLE_NUM][MTD_XY] = {
	{0, 0},						//!< 中央
	{0, 10 * FX32_ONE},			//!< 手前左
	{0, -10 * FX32_ONE},		//!< 手前右
	{0, 5 * FX32_ONE},			//!< 奥左
	{0, -5 * FX32_ONE},			//!< 奥右
};

static OBS_ACTION3D_NN_WORK *gm_gmk_needle_obj_3d_list = NULL;
#if GMD_GMK_NEEDLE_TEST_TVX
static AMS_AMB_HEADER* gm_gmk_needle_obj_tvx_list = NULL;
#endif // GMD_GMK_NEEDLE_TEST_TVX

// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkNeedleBuild
/*!
	ギミック トゲ データ構築
 */
// ==========================================================================
void GmGmkNeedleBuild(void)
{
	gm_gmk_needle_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER *)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_NEEDLE_MODEL)
									  , (AMS_AMB_HEADER *)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_NEEDLE_TEX)
									  , 0
									  );
#if GMD_GMK_NEEDLE_TEST_TVX
	void* tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_NEEDLE_TVX );
	amBindConv((Uint8*)tvx);
	gm_gmk_needle_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
#endif // GMD_GMK_NEEDLE_TEST_TVX
}


// ==========================================================================
// GmGmkNeedleFlush
/*!
	ギミック トゲ データ片付け
 */
// ==========================================================================
void GmGmkNeedleFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_NEEDLE_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_needle_obj_3d_list, amb->file_num);
	gm_gmk_needle_obj_3d_list = NULL;
#if GMD_GMK_NEEDLE_TEST_TVX
	gm_gmk_needle_obj_tvx_list = NULL;
#endif // GMD_GMK_NEEDLE_TEST_TVX
}


// ===========================================================================
// GmGmkNeedleInit
/*!
	ギミック トゲ 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note
		user_timer : FWアクションID
		user_work  : 実行アクションID
 */
// ===========================================================================
OBS_OBJECT_WORK* GmGmkNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_RECT_WORK		*rect_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;

	OBS_OBJECT_WORK		*back_obj = NULL;
	OBS_OBJECT_WORK		*stand_obj = NULL;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_NEEDLE_WORK), "GMK_NEEDLE_MAIN");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;

	// トゲタイプ取得
	ndl_work->needle_type = GmGmkNeedleGetType(eve_rec->id);

	ObjObjectCopyAction3dNNModel(obj_work
								 , &gm_gmk_needle_obj_3d_list[IDB_GMK_NEEDLE_MDL_GMK_NEEDLE_F_ZNO]
								 , &gmk_work->obj_3d
								 );
#if !GMD_GMK_NEEDLE_TEST_TVX
	// ローカルイベント作成(後ろ側のトゲとスタンド用)
	back_obj = GmEventMgrLocalEventBirth(
					GMD_EVENT_ID_NEEDLE_BACK,
					obj_work->pos.x,
					obj_work->pos.y,
					gmk_work->ene_com.eve_rec->flag,
					gmk_work->ene_com.eve_rec->left,
					gmk_work->ene_com.eve_rec->top,
					gmk_work->ene_com.eve_rec->width,
					gmk_work->ene_com.eve_rec->height,
					(u8)ndl_work->needle_type
					);

	stand_obj = GmEventMgrLocalEventBirth(
					GMD_EVENT_ID_NEEDLE_STAND,
					obj_work->pos.x,
					obj_work->pos.y,
					gmk_work->ene_com.eve_rec->flag,
					gmk_work->ene_com.eve_rec->left,
					gmk_work->ene_com.eve_rec->top,
					gmk_work->ene_com.eve_rec->width,
					gmk_work->ene_com.eve_rec->height,
					(u8)ndl_work->needle_type
					);

	// 後ろ側トゲとスタンドにはクリップ無効設定になっているので、親と同時に消えるように親を設定
	back_obj->parent_obj = obj_work;
	stand_obj->parent_obj = obj_work;
#endif // !GMD_GMK_NEEDLE_TEST_TVX
	// ppOutの関数を自前の描画設定関数に切り替え(一つのイベントに対して複数個の描画を行うため)
	obj_work->ppOut = gmGmkNeedleDrawFunc;

	gmk_work->ene_com.col_work.obj_col.obj			= obj_work;

	// 地形矩形設定
	gmk_work->ene_com.col_work.obj_col.width		= gm_gmk_col_rect_tbl[ndl_work->needle_type][GMD_GMK_NDL_COL_DATA_WIDTH];
	gmk_work->ene_com.col_work.obj_col.height		= gm_gmk_col_rect_tbl[ndl_work->needle_type][GMD_GMK_NDL_COL_DATA_HEIGHT];
	gmk_work->ene_com.col_work.obj_col.ofst_x		= gm_gmk_col_rect_tbl[ndl_work->needle_type][GMD_GMK_NDL_COL_DATA_OFST_X];
	gmk_work->ene_com.col_work.obj_col.ofst_y		= gm_gmk_col_rect_tbl[ndl_work->needle_type][GMD_GMK_NDL_COL_DATA_OFST_Y];

	gmk_work->ene_com.col_work.obj_col.dir = (u16)(GMD_GMK_COL_ROTATE_DIR * (ndl_work->needle_type - GMD_GMK_NDL_TYPE_UP));
	
	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE;

	// 矩形設定(攻撃)
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];

	// 仮矩形
	ObjRectWorkZSet(rect_work
					, gm_gmk_atk_rect_tbl[ndl_work->needle_type][MTD_LEFT]
					, gm_gmk_atk_rect_tbl[ndl_work->needle_type][MTD_TOP]
					, -500
					, gm_gmk_atk_rect_tbl[ndl_work->needle_type][MTD_RIGHT]
					, gm_gmk_atk_rect_tbl[ndl_work->needle_type][MTD_BOTTOM]
					, 500
					);
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
		// トロッコステージ対応
		if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_LEFT) {
			rect_work->rect.left -= GMD_GMK_NEEDLE_TRUCK_ATK_RECT_ADJUST;
		}
		else if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_RIGHT) {
			rect_work->rect.right += GMD_GMK_NEEDLE_TRUCK_ATK_RECT_ADJUST;
		}

	}
	// 矩形判定ON
	rect_work->flag |= OBD_RECT_ENABLE;
	rect_work->flag |= OBD_RECT_OUT;			// 連続判定防止用


	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		ObjDrawObjectActionSet(obj_work, IDB_GMK_NEEDLE_MDL_GMK_NEEDLE_F_ZNO);
		obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;		// 
		obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_2;		// 
	}


	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_MOVE_UNDER;	// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
//	obj_work->flag |= OBD_OBJECT_NOHIT;							// 矩形あたり無し(チェック用)

	ndl_work->state = GMD_GMK_NDL_INOUT_STATE_OUT;
	
	// 通常時初期化処理
	gmGmkNeedleFwInit(obj_work);
	
#if OBD_OBJECT_USE_NOEXIST
	obj_work->flag |= OBD_OBJECT_NOEXIST_ENABLE;
#endif // OBD_OBJECT_USE_NOEXIST
	
	// 終了処理差し替え(ワークにOBJデータを持った際に変更する)
//	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkNeedleExit);
#if GMD_GMK_NEEDLE_TEST_TVX
	ndl_work->color = 0xffffffff;
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3 || g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS) {
		ndl_work->color = 0xffa0a0ff;
	}
	else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		ndl_work->color = 0xa0a0a0ff;
	}
#endif // GMD_GMK_NEEDLE_TEST_TVX
	return (obj_work);
}



// ===========================================================================
// GmGmkActNeedleInit
/*!
	ギミック 出入りトゲ 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note
 */
// ===========================================================================
OBS_OBJECT_WORK* GmGmkActNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_RECT_WORK		*rect_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;

	OBS_OBJECT_WORK		*back_obj = NULL;
	OBS_OBJECT_WORK		*stand_obj = NULL;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_NEEDLE_WORK), "GMK_NEEDLE_ACT_MAIN");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;

	// トゲタイプ取得
	ndl_work->needle_type = GmGmkNeedleGetType(eve_rec->id);

	ObjObjectCopyAction3dNNModel(obj_work
								 , &gm_gmk_needle_obj_3d_list[IDB_GMK_NEEDLE_MDL_GMK_NEEDLE_F_ZNO]
								 , &gmk_work->obj_3d
								 );

	
#if !GMD_GMK_NEEDLE_TEST_TVX
	// ローカルイベント作成(後ろ側のトゲとスタンド用)
	back_obj = GmEventMgrLocalEventBirth(
					GMD_EVENT_ID_NEEDLE_BACK,
					obj_work->pos.x,
					obj_work->pos.y,
					gmk_work->ene_com.eve_rec->flag,
					gmk_work->ene_com.eve_rec->left,
					gmk_work->ene_com.eve_rec->top,
					gmk_work->ene_com.eve_rec->width,
					gmk_work->ene_com.eve_rec->height,
					(u8)ndl_work->needle_type
					);

	stand_obj = GmEventMgrLocalEventBirth(
					GMD_EVENT_ID_NEEDLE_STAND,
					obj_work->pos.x,
					obj_work->pos.y,
					gmk_work->ene_com.eve_rec->flag,
					gmk_work->ene_com.eve_rec->left,
					gmk_work->ene_com.eve_rec->top,
					gmk_work->ene_com.eve_rec->width,
					gmk_work->ene_com.eve_rec->height,
					(u8)ndl_work->needle_type
					);

	// 後ろ側トゲとスタンドにはクリップ無効設定になっているので、親と同時に消えるように親を設定
	back_obj->parent_obj = obj_work;
	stand_obj->parent_obj = obj_work;
#endif // !GMD_GMK_NEEDLE_TEST_TVX
	// ppOutの関数を自前の描画設定関数に切り替え(一つのイベントに対して複数個の描画を行うため)
	obj_work->ppOut = gmGmkNeedleDrawFunc;

	gmk_work->ene_com.col_work.obj_col.obj			= obj_work;

	// 地形矩形設定
	gmk_work->ene_com.col_work.obj_col.width		= gm_gmk_col_rect_tbl[ndl_work->needle_type][GMD_GMK_NDL_COL_DATA_WIDTH];
	gmk_work->ene_com.col_work.obj_col.height		= gm_gmk_col_rect_tbl[ndl_work->needle_type][GMD_GMK_NDL_COL_DATA_HEIGHT];
	gmk_work->ene_com.col_work.obj_col.ofst_x		= gm_gmk_col_rect_tbl[ndl_work->needle_type][GMD_GMK_NDL_COL_DATA_OFST_X];
	gmk_work->ene_com.col_work.obj_col.ofst_y		= gm_gmk_col_rect_tbl[ndl_work->needle_type][GMD_GMK_NDL_COL_DATA_OFST_Y];

	gmk_work->ene_com.col_work.obj_col.dir = (u16)(GMD_GMK_COL_ACT_ROTATE_DIR * (ndl_work->needle_type - GMD_GMK_NDL_TYPE_ACT_UP));
	
	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE;

	// 矩形設定(攻撃)
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];

	// 仮矩形
	ObjRectWorkZSet(rect_work
					, gm_gmk_atk_rect_tbl[ndl_work->needle_type][MTD_LEFT]
					, gm_gmk_atk_rect_tbl[ndl_work->needle_type][MTD_TOP]
					, -500
					, gm_gmk_atk_rect_tbl[ndl_work->needle_type][MTD_RIGHT]
					, gm_gmk_atk_rect_tbl[ndl_work->needle_type][MTD_BOTTOM]
					, 500
					);
	// 矩形判定ON
	rect_work->flag |= OBD_RECT_ENABLE;
	rect_work->flag |= OBD_RECT_OUT;			// 連続判定防止用

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_MOVE_UNDER;	// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	gmk_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOPRESSDIE;

#if 0
	// 初期状態は消えている状態からスタート
	obj_work->scale.y = 0;
	amFlagOn(obj_work->flag, OBD_OBJECT_NOHIT);
	amFlagOn(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);

	ndl_work->state = GMD_GMK_NDL_INOUT_STATE_IN;
	
	// 通常時初期化処理
	gmGmkActNeedleScalingInit(obj_work);

#else
	// 初期状態は出ている状態からスタート
	obj_work->scale.y = FX32_ONE;
	amFlagOn(obj_work->flag, OBD_OBJECT_NOHIT);
//	amFlagOff(obj_work->flag, OBD_OBJECT_NOHIT);
	amFlagOff(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);

	ndl_work->state = GMD_GMK_NDL_INOUT_STATE_OUT;
	ndl_work->is_first_disp = 1;
	ndl_work->timer = -30;

	// 通常時初期化処理
	obj_work->ppFunc = gmGmkActNeedleFwMain;
//	gmGmkNeedleFwInit(obj_work);

#endif
	
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		ObjDrawObjectActionSet(obj_work, IDB_GMK_NEEDLE_MDL_GMK_NEEDLE_F_ZNO);
		obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;		// 
		obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_2;		// 
	}

	// 終了処理差し替え(ワークにOBJデータを持った際に変更する)
//	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkNeedleExit);
	
#if GMD_GMK_NEEDLE_TEST_TVX
	ndl_work->color = 0xffffffff;
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3 || g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS) {
		ndl_work->color = 0xffa0a0ff;
	}
	else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		ndl_work->color = 0xa0a0a0ff;
	}
#endif // GMD_GMK_NEEDLE_TEST_TVX
	return (obj_work);
}


// ===========================================================================
// GmGmkBackNeedleInit
/*!
	ギミック トゲ(後ろ) 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note
 */
// ===========================================================================
OBS_OBJECT_WORK* GmGmkBackNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
#if GMD_GMK_NEEDLE_TEST_TVX
	UNREFERENCED_PARAMETER(eve_rec);
	UNREFERENCED_PARAMETER(pos_x);
	UNREFERENCED_PARAMETER(pos_y);
	UNREFERENCED_PARAMETER(type);
	
	return NULL;
#else
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_NEEDLE_WORK), "GMK_NEEDLE_BACK");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
	
	// トゲタイプ取得
	ndl_work->needle_type = type;

	ObjObjectCopyAction3dNNModel(obj_work
								 , &gm_gmk_needle_obj_3d_list[IDB_GMK_NEEDLE_MDL_GMK_NEEDLE_B_ZNO]
								 , &gmk_work->obj_3d
								 );

	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - 3 * FX32_ONE;

	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	
	// ppOutの関数を自前の描画設定関数に切り替え(一つのイベントに対して複数個の描画を行うため)
	obj_work->ppOut = gmGmkBackNeedleDrawFunc;
	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_MOVE_UNDER;	// 移動無し 地形あたり無し
	obj_work->flag |= OBD_OBJECT_NOHIT;							// 矩形あたり無し◆

	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		ObjDrawObjectActionSet(obj_work, IDB_GMK_NEEDLE_MDL_GMK_NEEDLE_B_ZNO);
		obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;		// 
		obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_2;		// 
	}
	

	// メイン処理
	obj_work->ppFunc = gmGmkNeedleBackFwMain;
	
	// 終了処理差し替え(デバッグ用)
//	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkNeedleBackExit);
	
	return (obj_work);
#endif // GMD_GMK_NEEDLE_TEST_TVX
}


// ===========================================================================
// GmGmkStandNeedleInit
/*!
	ギミック トゲ台座 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note
 */
// ===========================================================================
OBS_OBJECT_WORK* GmGmkStandNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
#if GMD_GMK_NEEDLE_TEST_TVX
	UNREFERENCED_PARAMETER(eve_rec);
	UNREFERENCED_PARAMETER(pos_x);
	UNREFERENCED_PARAMETER(pos_y);
	UNREFERENCED_PARAMETER(type);
	
	return NULL;
#else
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	
	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_NEEDLE_WORK), "GMK_NEEDLE_STAND");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
	
	// トゲタイプ取得
	ndl_work->needle_type = type;

	ObjObjectCopyAction3dNNModel(obj_work
								 , &gm_gmk_needle_obj_3d_list[IDB_GMK_NEEDLE_MDL_GMK_NEEDLE_STAND_ZNO]
								 , &gmk_work->obj_3d
								 );


	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - 1;
	
	// ppOutの関数を自前の描画設定関数に切り替え(一つのイベントに対して複数個の描画を行うため)
	obj_work->ppOut = gmGmkStandNeedleDrawFunc;
	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_MOVE_UNDER;	// 移動無し 地形あたり無し
	obj_work->flag |= OBD_OBJECT_NOHIT;							// 矩形あたり無し◆

	
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		ObjDrawObjectActionSet(obj_work, IDB_GMK_NEEDLE_MDL_GMK_NEEDLE_STAND_ZNO);
		obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;		// 通常ライトOFF
		obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_2;		// 専用ライトOFF
	}

	// メイン処理
	obj_work->ppFunc = gmGmkNeedleStandFwMain;
	
	// 終了処理差し替え(デバッグ用)
//	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkNeedleStandExit);
	
	return (obj_work);
#endif // GMD_GMK_NEEDLE_TEST_TVX
}



// ----- Static Functions --------------------（スタティック関数の定義：局所）
// ==========================================================================
// gmGmkNeedleExit
/*!
 *	ギミック トゲ 終了処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
/*
void gmGmkNeedleExit(OBS_OBJECT_WORK *obj_work)		// 現在未使用
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
}
*/


// ==========================================================================
// gmGmkNeedleBackExit
/*!
 *	ギミック トゲ 終了処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
/*
void gmGmkNeedleBackExit(MTS_TASK_TCB *tcb)		// 現在未使用
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	u32 test1 = 0;
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;

	test1 = obj_work->rect_num;
}
*/

// ==========================================================================
// gmGmkNeedleStandExit
/*!
 *	ギミック トゲ 終了処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
/*
void gmGmkNeedleStandExit(MTS_TASK_TCB *tcb)		// 現在未使用
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	u32 test2 = 0;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;

	test2 = obj_work->rect_num;
}
*/

// ==========================================================================
// gmGmkNeedleFwInit
/*!
 *	ギミック トゲ FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkNeedleFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;

	// メイン処理
	obj_work->ppFunc = gmGmkNeedleFwMain;
}


// ==========================================================================
// gmGmkNeedleFwMain
/*!
 *	ギミック トゲ FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkNeedleFwMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	OBS_RECT_WORK		*rect_work;
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];

	// 上向き針に乗っているプレイヤーにダメージ（特殊）
	if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_UP) {
		OBS_OBJECT_WORK	*rider_obj = gmk_work->ene_com.col_work.obj_col.rider_obj;

		// オブジェクトが自分に乗っていプレイヤーであれば
		if (rider_obj && rider_obj->ride_obj == (OBS_OBJECT_WORK*)gmk_work) {
			if (rider_obj->obj_type == GMD_OBJTYPE_PLAYER) {
				// ダメージON
				rect_work->flag |= OBD_RECT_ENABLE;
			}
		}
		else {
			rect_work->flag &= ~OBD_RECT_ENABLE;
		}
	}
	
}



// ==========================================================================
// gmGmkActNeedleFwInit
/*!
 *	ギミック 出入りトゲ FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkActNeedleFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;

	// トゲが出ている状態ならば
	if (ndl_work->state == GMD_GMK_NDL_INOUT_STATE_OUT) {
		// 矩形判定あり
//		amFlagOff(obj_work->flag, OBD_OBJECT_NOHIT);
		amFlagOff(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);
	}
	else {
		amFlagOn(obj_work->flag, OBD_OBJECT_NOHIT);
//		amFlagOn(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);
	}
	
	// メイン処理
	obj_work->ppFunc = gmGmkActNeedleFwMain;
}


// ==========================================================================
// gmGmkActNeedleFwMain
/*!
 *	ギミック トゲ FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkActNeedleFwMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	OBS_RECT_WORK		*rect_work;
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];

	// 上向き針に乗っているプレイヤーにダメージ（特殊）
	if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_ACT_UP) {
		OBS_OBJECT_WORK	*rider_obj = gmk_work->ene_com.col_work.obj_col.rider_obj;

		// オブジェクトが自分に乗っているプレイヤーであれば
		if (rider_obj && rider_obj->ride_obj == (OBS_OBJECT_WORK*)gmk_work) {
			if (rider_obj->obj_type == GMD_OBJTYPE_PLAYER) {
				// ダメージON
				amFlagOff(obj_work->flag, OBD_OBJECT_NOHIT);
			}
		}
		else {
			amFlagOn(obj_work->flag, OBD_OBJECT_NOHIT);
		}
	}
	
//	if (ndl_work->timer >= GMD_GMK_ACT_NDL_WAITING_TIME - 1) {
//		amFlagOff(obj_work->flag, OBD_OBJECT_NOHIT);
//		amFlagOn(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);
//	}
	
	if (ndl_work->timer >= GMD_GMK_ACT_NDL_WAITING_TIME) {
		ndl_work->timer = 0;
		gmGmkActNeedleScalingInit(obj_work);
	}
	else {
		// タイマー更新
		ndl_work->timer++;
	}
	
	if (ndl_work->scale_flag & GMD_GMK_ACT_NDL_COL_SCALE_EFCT) {
		gmGmkActNeedleSetScaleColRect(obj_work);
	}
}




// ==========================================================================
// gmGmkActNeedleScalingInit
/*!
 *	ギミック トゲ スケーリング中 初期化処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkActNeedleScalingInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;

	// トゲが出ていない状態ならば
	if (ndl_work->state == GMD_GMK_NDL_INOUT_STATE_IN) {
		// 矩形判定なし
		amFlagOn(obj_work->flag, OBD_OBJECT_NOHIT);
		amFlagOn(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);
		ndl_work->scale_flag |= GMD_GMK_ACT_NDL_COL_SCALE_EFCT;
		ndl_work->scale_flag |= GMD_GMK_ACT_NDL_COL_SCALE_DOWN;
	}
	else {
		if (!ndl_work->is_first_disp) {
			obj_work->scale.y = 0x0100;
			ndl_work->scale_flag |= GMD_GMK_ACT_NDL_COL_SCALE_EFCT;
			ndl_work->scale_flag |= GMD_GMK_ACT_NDL_COL_SCALE_UP;
		}
		else {
			ndl_work->scale_flag |= GMD_GMK_ACT_NDL_COL_SCALE_EFCT;
			ndl_work->scale_flag |= GMD_GMK_ACT_NDL_COL_SCALE_DOWN;
		}
	}
	

	if (ndl_work->is_first_disp) {
		amFlagOn(obj_work->flag, OBD_OBJECT_NOHIT);
		amFlagOn(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);

		ndl_work->state = GMD_GMK_NDL_INOUT_STATE_IN;
		ndl_work->is_first_disp = 0;
	}
	
	amFlagOff(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);
	
	if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_ACT_UP) {
		OBS_OBJECT_WORK	*rider_obj = gmk_work->ene_com.col_work.obj_col.rider_obj;

		// オブジェクトが自分に乗っているプレイヤーであれば
		if (rider_obj && rider_obj->ride_obj == (OBS_OBJECT_WORK*)gmk_work) {
			if (rider_obj->obj_type == GMD_OBJTYPE_PLAYER) {
				// ダメージON
				amFlagOff(obj_work->flag, OBD_OBJECT_NOHIT);
			}
		}
		else {
			amFlagOn(obj_work->flag, OBD_OBJECT_NOHIT);
		}
	}
	
	// メイン処理
	obj_work->ppFunc = gmGmkActNeedleScalingMain;
}



// ==========================================================================
// gmGmkActNeedleScalingMain
/*!
 *	ギミック トゲ スケーリング中 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkActNeedleScalingMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
	
	// スケーリングタイマーチェック
	if (ndl_work->timer >= GMD_GMK_ACT_NDL_SCALING_TIME) {
		ndl_work->timer = 0;
		
		gmGmkActNeedleRectWaitInit(obj_work);
//		amFlagOff(obj_work->flag, OBD_OBJECT_NOHIT);
		
		return;
	}
	else {
		amFlagOn(obj_work->flag, OBD_OBJECT_NOHIT);
//		amFlagOn(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);
	}

	// スケーリング処理(針の出入り)
	if (ndl_work->state == GMD_GMK_NDL_INOUT_STATE_OUT) {
		obj_work->scale.y = (fx32)(obj_work->scale.y + GMD_GMK_ACT_NDL_SCALING_RATE);
	}
	else {
		obj_work->scale.y = (fx32)(obj_work->scale.y - GMD_GMK_ACT_NDL_SCALING_RATE);
	}
	
	// スケール値が範囲外になった場合、CLIP
	if (obj_work->scale.y > FX32_ONE) {
		obj_work->scale.y = FX32_ONE;
	}
	else if (obj_work->scale.y <= 0) {
		obj_work->scale.y = 0;
	}
	amFlagOff(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);
	
	obj_work->scale.y = MTM_MATH_CLIP(obj_work->scale.y, 0, FX32_ONE);
	
	if (ndl_work->scale_flag & GMD_GMK_ACT_NDL_COL_SCALE_EFCT) {
		gmGmkActNeedleSetScaleColRect(obj_work);
	}
	
//	gmk_work->ene_com.col_work.obj_col.height = (u16)(32 * obj_work->scale.y >> FX32_SHIFT);
//	gmk_work->ene_com.col_work.obj_col.ofst_y = (s16)(-1 * gmk_work->ene_com.col_work.obj_col.height);
	
	// 上向き針に乗っているプレイヤーにダメージ（特殊）
	if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_ACT_UP) {
		OBS_OBJECT_WORK	*rider_obj = gmk_work->ene_com.col_work.obj_col.rider_obj;

		// オブジェクトが自分に乗っているプレイヤーであれば
		if (rider_obj && rider_obj->ride_obj == (OBS_OBJECT_WORK*)gmk_work) {
			if (rider_obj->obj_type == GMD_OBJTYPE_PLAYER) {
				// ダメージON
				amFlagOff(obj_work->flag, OBD_OBJECT_NOHIT);
				
//				if (ndl_work->state == GMD_GMK_NDL_INOUT_STATE_OUT) {
//					rider_obj->flow.y = -32 / 6 * FX32_ONE;
//				}
//				else {
//					rider_obj->flow.y = 32 / 6 * FX32_ONE;
//				}
			}
		}
		else {
			amFlagOn(obj_work->flag, OBD_OBJECT_NOHIT);
		}
	}
	
	// タイマー更新
	ndl_work->timer++;
}


// ==========================================================================
// gmGmkActNeedleRectWaitInit
/*!
 *	ギミック 出入りトゲ コリジョン矩形発生初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkActNeedleRectWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;

	// メイン処理
	obj_work->ppFunc = gmGmkActNeedleRectWaitMain;
}


// ==========================================================================
// gmGmkActNeedleRectWaitMain
/*!
 *	ギミック トゲ コリジョン矩形発生処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkActNeedleRectWaitMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	OBS_RECT_WORK		*rect_work;
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];

	amFlagOff(gmk_work->ene_com.col_work.obj_col.flag, OBD_COLOBJ_NOHIT);
	gmGmkActNeedleFwInit(obj_work);
	ndl_work->state ^= GMD_GMK_NDL_INOUT_STATE_IN;

	if (ndl_work->state == GMD_GMK_NDL_INOUT_STATE_IN) {
		// 出入りトゲSE再生
		GmSoundPlaySE("Spine");
	}
	
	if (ndl_work->scale_flag & GMD_GMK_ACT_NDL_COL_SCALE_EFCT) {
		gmGmkActNeedleSetScaleColRect(obj_work);
	}
	
/*
	
	// 上向き針に乗っているプレイヤーにダメージ（特殊）
	if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_UP
		|| ndl_work->needle_type == GMD_GMK_NDL_TYPE_ACT_UP) {
		OBS_OBJECT_WORK	*rider_obj = gmk_work->ene_com.col_work.obj_col.rider_obj;

		// オブジェクトが自分に乗っていプレイヤーであれば
		if (rider_obj && rider_obj->ride_obj == (OBS_OBJECT_WORK*)gmk_work) {
			if (rider_obj->obj_type == GMD_OBJTYPE_PLAYER) {
				// ダメージON
				amFlagOff(obj_work->flag, OBD_OBJECT_NOHIT);
//				rect_work->flag |= OBD_RECT_ENABLE;
			}
		}
		else {
			amFlagOn(obj_work->flag, OBD_OBJECT_NOHIT);
//			rect_work->flag &= ~OBD_RECT_ENABLE;
		}
	}
*/	
}

#if !GMD_GMK_NEEDLE_TEST_TVX
// ==========================================================================
// gmGmkNeedleBackFwMain
/*!
 *	ギミック トゲスタンド FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkNeedleBackFwMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;

	if (obj_work->parent_obj) {
		obj_work->scale.y = obj_work->parent_obj->scale.y;
	}
}



// ==========================================================================
// gmGmkNeedleStandFwMain
/*!
 *	ギミック トゲスタンド FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkNeedleStandFwMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
}
#endif // !GMD_GMK_NEEDLE_TEST_TVX



// ==========================================================================
// GmGmkNeedleGetType
/*!
 *	ギミック トゲタイプ取得処理
 *
 *	@param type	[in] トゲのタイプ
 */
// ==========================================================================
static u16 GmGmkNeedleGetType(u16 type)
{
	u16 tmp_type = 0;
	u16 result = 0;

	// 通常トゲならば
	if (type < GMD_EVENT_ID_ACT_NEEDLE_U) {
		tmp_type = (u16)(type - GMD_EVENT_ID_NEEDLE_U);

		result = gm_gmk_ndl_type_tbl[tmp_type];
	}

	// 出入りトゲならば
	else {
		tmp_type = (u16)(type - GMD_EVENT_ID_ACT_NEEDLE_U);
		
		tmp_type = (u16)(GMD_GMK_NDL_TYPE_ACT_UP + tmp_type);
		
		result = gm_gmk_ndl_type_tbl[tmp_type];
	}
	
	return result;
}


// ==========================================================================
// gmGmkActNeedleSetScaleColRect
/*!
 *	ギミック トゲ コリジョン矩形発生処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkActNeedleSetScaleColRect(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	s32 tmp_height = 0;
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
	
	ndl_work->scale_timer++;
	
	if (ndl_work->scale_flag & GMD_GMK_ACT_NDL_COL_SCALE_DOWN) {
		tmp_height = (s32)(gmk_work->ene_com.col_work.obj_col.height - 3);
		
		if (tmp_height < 0) {
			tmp_height = 0;
			ndl_work->scale_timer = 0;
			ndl_work->scale_flag &= ~GMD_GMK_ACT_NDL_COL_SCALE_EFCT;
			ndl_work->scale_flag &= ~GMD_GMK_ACT_NDL_COL_SCALE_DOWN;
		}
		
		gmk_work->ene_com.col_work.obj_col.height = (u16)tmp_height;
		gmk_work->ene_com.col_work.obj_col.ofst_y = (s16)(-1 * gmk_work->ene_com.col_work.obj_col.height);
	}
	else if (ndl_work->scale_flag & GMD_GMK_ACT_NDL_COL_SCALE_UP) {
		tmp_height = (s32)(gmk_work->ene_com.col_work.obj_col.height + 4);
		
		if (tmp_height > 32) {
			tmp_height = 32;
			ndl_work->scale_timer = 0;
			ndl_work->scale_flag &= ~GMD_GMK_ACT_NDL_COL_SCALE_EFCT;
			ndl_work->scale_flag &= ~GMD_GMK_ACT_NDL_COL_SCALE_UP;
		}
		
		gmk_work->ene_com.col_work.obj_col.height = (u16)tmp_height;
		gmk_work->ene_com.col_work.obj_col.ofst_y = (s16)(-1 * gmk_work->ene_com.col_work.obj_col.height);
	}
}



// ==========================================================================
// gmGmkNeedleDrawFunc
/*!
 *	ギミック トゲ 描画設定処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkNeedleDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
	
#if GMD_GMK_NEEDLE_TEST_TVX
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	
	void *tvx_needle, *tvx_stand;
	VecFx32 pos;
	VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
	Angle16 dir_z = 0x0000;
	NNS_TEXLIST* texlist = obj_work->obj_3d->texlist;
	GMS_TVX_EX_WORK work;
	
	// 方向設定(向きに対して回転させる)
	dir_z = (Angle16)(-GMD_GMK_DRAW_ROTATE_DIR * (ndl_work->needle_type - GMD_GMK_NDL_TYPE_UP));
	if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_ACT_DOWN) {
		dir_z = (Angle16)(-0x8000);
	}
	obj_work->dir.z = -dir_z; // 当たり判定が不正なのをただす
	
	// extend 
	work.u_wrap = NNE_PRIM_TEXWRAP_CLAMP;
	work.v_wrap = NNE_PRIM_TEXWRAP_CLAMP;
	work.coord.u = 0.0f;
	work.coord.v = 0.0f;
	work.color = ndl_work->color;
	
	// 設定された本数分、描画する
	tvx_needle = amBindGet(gm_gmk_needle_obj_tvx_list, IDB_MODEL_GMK_NEEDLE_F_TVX);
	tvx_stand  = amBindGet(gm_gmk_needle_obj_tvx_list, IDB_MODEL_GMK_NEEDLE_STAND_TVX);
	for (int i = 0; i < 5; i++) {
		pos.z = obj_work->pos.z;
		if (i >= 3) {
			pos.z -= (2 * FX32_ONE);
		}
		// ここでタイプ別毎の表示位置設定
		switch (ndl_work->needle_type) {
		case 0:
		case 4:
			pos.x = obj_work->pos.x + gm_gmk_disp_ofst_tbl_u[i][MTD_X];
			pos.y = obj_work->pos.y + gm_gmk_disp_ofst_tbl_u[i][MTD_Y];
			break;
			
		case 1:
		case 5:
			pos.x = obj_work->pos.x + gm_gmk_disp_ofst_tbl_l[i][MTD_X];
			pos.y = obj_work->pos.y + gm_gmk_disp_ofst_tbl_l[i][MTD_Y];
			break;
			
		case 2:
			pos.x = obj_work->pos.x + gm_gmk_disp_ofst_tbl_d[i][MTD_X];
			pos.y = obj_work->pos.y + gm_gmk_disp_ofst_tbl_d[i][MTD_Y];
			break;
			
		case 3:
			pos.x = obj_work->pos.x + gm_gmk_disp_ofst_tbl_r[i][MTD_X];
			pos.y = obj_work->pos.y + gm_gmk_disp_ofst_tbl_r[i][MTD_Y];
			break;
			
		default:
			break;
		}
		GmTvxSetModelEx(tvx_needle, texlist, &pos, &obj_work->scale, GMD_TVX_DISP_ROTATE | GMD_TVX_DISP_SCALE | GMD_TVX_DISP_LIGHT_DISABLE, dir_z, &work);
		GmTvxSetModel(tvx_stand,  texlist, &pos, &scale, GMD_TVX_DISP_ROTATE | GMD_TVX_DISP_SCALE, dir_z);
	}
#else
	fx32 tmp_save_ofst[MTD_XY];

	// 一つ目の表示位置を保存
	tmp_save_ofst[MTD_X] = obj_work->ofst.x;
	tmp_save_ofst[MTD_Y] = obj_work->ofst.y;
	
	// 設定された本数分、描画する(前列の長いトゲ)
	for (int i = 0; i < 3; i++) {
		// ここでタイプ別毎の表示位置オフセットの値設定
		switch (ndl_work->needle_type) {
		case 0:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_u[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_u[i][MTD_Y];
			
			break;
		case 1:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_l[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_l[i][MTD_Y];
			
			break;
		case 2:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_d[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_d[i][MTD_Y];
			
			break;
		case 3:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_r[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_r[i][MTD_Y];
			
			break;
		case 4:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_u[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_u[i][MTD_Y];
			
			break;
		case 5:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_d[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_d[i][MTD_Y];
			
			break;
		default:
			break;
		}
		
		// 方向設定(向きに対して回転させる)
		if (ndl_work->needle_type < 4) {
			obj_work->dir.z = (u16)(GMD_GMK_DRAW_ROTATE_DIR * (ndl_work->needle_type - GMD_GMK_NDL_TYPE_UP));
		}
		else if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_ACT_DOWN) {
			obj_work->dir.z = (u16)(0x8000);
		}
		
		ObjDrawActionSummary(obj_work);
	}
	
	// 一つ目の表示位置に戻す
	obj_work->ofst.x	= tmp_save_ofst[MTD_X];
	obj_work->ofst.y	= tmp_save_ofst[MTD_Y];
#endif // GMD_GMK_NEEDLE_TEST_TVX
}


#if !GMD_GMK_NEEDLE_TEST_TVX
// ==========================================================================
// gmGmkBackNeedleDrawFunc
/*!
 *	ギミック トゲ 描画設定処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkBackNeedleDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	
	fx32 tmp_save_ofst[MTD_XY];
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
	
	// 一つ目の表示位置を保存
	tmp_save_ofst[MTD_X] = obj_work->ofst.x;
	tmp_save_ofst[MTD_Y] = obj_work->ofst.y;
	
	// 設定された本数分、描画する(後列の短いトゲ)
	for (int i = 3; i < 5; i++) {
		switch (ndl_work->needle_type) {
		case 0:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_u[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_u[i][MTD_Y];
			
			break;
		case 1:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_l[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_l[i][MTD_Y];
			
			break;
		case 2:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_d[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_d[i][MTD_Y];
			
			break;
		case 3:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_r[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_r[i][MTD_Y];
			
			break;
		case 4:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_u[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_u[i][MTD_Y];
			
			break;
		case 5:
			obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_d[i][MTD_X];
			obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_d[i][MTD_Y];
			
			break;
		default:
			break;
		}
		
		// 方向設定(向きに対して回転させる)
		if (ndl_work->needle_type < 4) {
			obj_work->dir.z = (u16)(GMD_GMK_DRAW_ROTATE_DIR * (ndl_work->needle_type - GMD_GMK_NDL_TYPE_UP));
		}
		else if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_ACT_DOWN) {
			obj_work->dir.z = (u16)(0x8000);
		}
		
		ObjDrawActionSummary(obj_work);
	}
	
	// 一つ目の表示位置に戻す
	obj_work->ofst.x	= tmp_save_ofst[MTD_X];
	obj_work->ofst.y	= tmp_save_ofst[MTD_Y];
}



// ==========================================================================
// gmGmkBackNeedleDrawFunc
/*!
 *	ギミック トゲ 描画設定処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
static void gmGmkStandNeedleDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_NEEDLE_WORK	*ndl_work;
	
	fx32 tmp_save_ofst[MTD_XY];
	
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ndl_work = (GMS_GMK_NEEDLE_WORK*)gmk_work;
	
	// 一つ目の表示位置を保存
	tmp_save_ofst[MTD_X] = obj_work->ofst.x;
	tmp_save_ofst[MTD_Y] = obj_work->ofst.y;
	
	// 設定された本数分、描画する(スタンドは前列後列共に共通なので５つ分)
	for (int i = 0; i < 5; i++) {
		switch (ndl_work->needle_type) {
		case 0:
			if (i == 3 || i == 4) {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_u[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_u[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE * 2;
			}
			else {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_u[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_u[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z;
			}
			
			break;
		case 1:
			if (i == 3 || i == 4) {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_l[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_l[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + FX32_ONE;
			}
			else {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_l[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_l[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z;
			}
			
			break;
		case 2:
			if (i == 3 || i == 4) {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_d[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_d[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + FX32_ONE;
			}
			else {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_d[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_d[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z;
			}
			
			break;
		case 3:
			if (i == 3 || i == 4) {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_r[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_r[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + FX32_ONE;
			}
			else {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_r[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_r[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z;
			}
			
			break;
		case 4:
			if (i == 3 || i == 4) {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_u[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_u[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE * 2;
			}
			else {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_u[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_u[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z;
			}
			
			break;
		case 5:
			if (i == 3 || i == 4) {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_d[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_d[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK + FX32_ONE;
			}
			else {
				obj_work->ofst.x	= gm_gmk_disp_ofst_tbl_d[i][MTD_X];
				obj_work->ofst.y	= gm_gmk_disp_ofst_tbl_d[i][MTD_Y];
				obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z;
			}
			
			break;
		default:
			break;
		}
		
		// 方向設定(向きに対して回転させる)
		if (ndl_work->needle_type < 4) {
			obj_work->dir.z = (u16)(GMD_GMK_DRAW_ROTATE_DIR * (ndl_work->needle_type - GMD_GMK_NDL_TYPE_UP));
		}
		else if (ndl_work->needle_type == GMD_GMK_NDL_TYPE_ACT_DOWN) {
			obj_work->dir.z = (u16)(0x8000);
		}
		
		ObjDrawActionSummary(obj_work);
	}
	
	// 一つ目の表示位置に戻す
	obj_work->ofst.x	= tmp_save_ofst[MTD_X];
	obj_work->ofst.y	= tmp_save_ofst[MTD_Y];
}
#endif // !GMD_GMK_NEEDLE_TEST_TVX


// ==========================================================================
// GmGmkNeedleSetLight
/*!
 *	ギミック 針 ライト設定		※4-3以外はデフォルト設定になっています(デバッグライトも無効です)
 */
// ==========================================================================
void GmGmkNeedleSetLight(void)
{
	NNS_RGBA	light_col = {1.0f, 1.0f, 1.0f, 1.0f};
	NNS_VECTOR	light_vec;
	float	intensity;
	
	// ライト設定	※※※(とりあえず仮で標準ライト設定)→決まり次第、設定修正
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) { // ここは4-3だけ特殊設定
//	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_4) {	// 念のため、ここでもZONE4の判定を入れておく
#if _WII
		// ZONE4
		light_vec.x = -0.1f;
		light_vec.y = 0.1f;
		light_vec.z = -1.0f;
#else
		// ZONE4
		light_vec.x = -0.1f;
		light_vec.y = -0.09f;
		light_vec.z = -0.93f;
#endif
	}
	else {
		light_vec.x = -1.0f;
		light_vec.y = -1.0f;
		light_vec.z = -1.0f;
	}
	light_col.r = 1.0f;
	light_col.g = 1.0f;
	light_col.b = 1.0f;
	
	nnNormalizeVector(&light_vec, &light_vec);
	
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		// Zone4-3のみ暗く
		intensity = GMD_LIGHT_CMN_DARK_INTENSITY;
	} else {
		// 通常
		intensity = GMD_LIGHT_COMN_INTENSITY;
	}
	
	ObjDrawSetParallelLight(NNE_LIGHT_2, &light_col, intensity, &light_vec);
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================