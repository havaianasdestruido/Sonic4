// ==========================================================================
/*!
  @file gmGmkSwWall.cpp
  @brief ギミック スイッチ壁

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGmkSwWall.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmComEfct.h"
#include "gmGmkSwitch.h"
#include "gmSound.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

#include "gmGmkSwWall.h"

// データヘッダ
#include "common/model/GMK_SW_WALL3_MDL.HMB"
#include "common/model/GMK_SW_WALL4_MDL.HMB"

#include "common/model/GMK_SW_WALL3_MAT.HMB"

#if _IPHONE
// commonと構成が変わっている場合に使用する定義
#include "iPhone/model/GMK_SW_WALL3_TVX.HMB"
#endif // _IPHONE



//----- Definitions ---------------------------------------------------------

#define GMD_GMK_SW_WALL_TEST_TVX       (1 & _IPHONE)

/* eve_rec->left */
// 管理ID
/* eve_rec->top */
/* eve_rec->width */
/* eve_rec->height */
/* eve_rec->flag */
#define GMD_GMK_SWWALL_SW_CLOSE		(0x0001)	//!< スイッチで閉じる

enum {
	GMD_GMK_SW_WALL_OPT_MDL_Z3_STONE	= 0,
	GMD_GMK_SW_WALL_OPT_MDL_Z3_GLARE,
	GMD_GMK_SW_WALL_OPT_MDL_Z3_MAX,

	GMD_GMK_SW_WALL_OPT_MDL_Z4_GEAR		= 0,
	GMD_GMK_SW_WALL_OPT_MDL_Z4_GEAR_BASE,
	GMD_GMK_SW_WALL_OPT_MDL_Z4_MAX
};
#if GMD_GMK_SW_WALL_OPT_MDL_Z3_MAX >= GMD_GMK_SW_WALL_OPT_MDL_Z4_MAX
	#define GMD_GMK_SW_WALL_OPT_MDL_MAX		GMD_GMK_SW_WALL_OPT_MDL_Z3_MAX
#else
	#define GMD_GMK_SW_WALL_OPT_MDL_MAX		GMD_GMK_SW_WALL_OPT_MDL_Z4_MAX
#endif


#define GMD_GMK_SWWALL_COL_DIR_BUF		(4*32)	//!< 角度バッファサイズ(最大サイズ)

/// スイッチ壁ワーク
typedef struct tag_GMS_GMK_SWWALL_WORK {
	GMS_ENEMY_3D_WORK		gmk_work;
	OBS_ACTION3D_NN_WORK	obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_MAX];		// ZONE4スイッチ壁装飾ギア, 宝石, etc...

	u32						id;				// 対応するスイッチID

	fx32					wall_size;		// 壁サイズ
	fx32					wall_draw_size;	// 壁描画サイズ

	u16						wall_type;		// 壁タイプ	// GMD_GMK_SWWALL_TYPE
	u16						gear_dir;		// 装飾ギア回転量
	u16						gear_base_dir;	// 装飾ギアベース回転量

	VecFx32					gear_pos;		// 装飾ギア表示位置
	VecFx32					gearbase_pos;	// 装飾ギアベース表示位置

	fx32					wall_spd;		// 壁移動速度

//	OBS_DATA_WORK			*col_data_work;	// 地形データワーク
	u8						col_dir_buf[GMD_GMK_SWWALL_COL_DIR_BUF];

	GSS_SND_SE_HANDLE		*h_snd;			//!< 壁移動時SE用

} GMS_GMK_SWWALL_WORK;

enum {
	GMD_GMK_SWWALL_TYPE_RIGHT	= 0,
	GMD_GMK_SWWALL_TYPE_LEFT,
	GMD_GMK_SWWALL_TYPE_BOTTOM,
	GMD_GMK_SWWALL_TYPE_TOP,

	GMD_GMK_SWWALL_TYPE_MAX
};

/// 地形角度データ
enum {
	GMD_GMK_SWWALL_COL_DIR_H4	= 0,
	GMD_GMK_SWWALL_COL_DIR_H8,
	GMD_GMK_SWWALL_COL_DIR_V4,
	GMD_GMK_SWWALL_COL_DIR_V8,

	GMD_GMK_SWWALL_COL_DIR_MAX
};

// enemy_flag
#define GMD_GMK_ENEMY_FLAG_GEAR_OPT			(0x0001)	//!< ギアオプション表示あり
#define GMD_GMK_ENEMY_FLAG_STONE_OPT		(0x0002)	//!< 石オプション表示あり
#define GMD_GMK_ENEMY_FLAG_GEAR_BASE_HFLIP	(0x0004)	//!< ギアベースをHフリップ表示

// 壁サイズ
#define GMD_GMK_SWWALL_BLOCK_SIZE		(32)		//!< 壁ブロックサイズ
#define GMD_GMK_SWWALL_Z3_WALL_SIZE		(32*4)		//1< ZONE3通常壁サイズ
#define GMD_GMK_SWWALL_Z3_L_WALL_SIZE	(32*8)		//1< ZONE3ロング壁サイズ
#define GMD_GMK_SWWALL_Z4_WALL_SIZE		(32*3)		//1< ZONE4通常壁サイズ

// 地形設定
#define GMD_GMK_SWWALL_COL_Z3_THICK		(32)		//!< ZONE3タイプ壁厚み
#define GMD_GMK_SWWALL_COL_Z4_THICK		(24)		//!< ZONE4タイプ壁厚み
#define GMD_GMK_SWWALL_COL_OFST_X		(-16)		//!< X地形固定タイプのオフセットX
#define GMD_GMK_SWWALL_COL_OFST_Y		(-16)		//!< Y地形固定タイプのオフセットY

// 移動速度
#define GMD_GMK_SWWALL_DEF_SPD			(4)			//!< 壁標準移動速度

// 歯車描画オフセット(横向き右に出るタイプ)
#define GMD_GMK_SWWALL_GEAR_OPT_OFST_X		(-48+12.8)
#define GMD_GMK_SWWALL_GEAR_OPT_OFST_Y		(16.8)
#define GMD_GMK_SWWALL_GEARBASE_OPT_OFST_X	(-48)
#define GMD_GMK_SWWALL_GEARBASE_OPT_OFST_Y	(12)

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSwWallDest(MTS_TASK_TCB *tcb);
static void gmGmkSwWallFwInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwWallFwMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwWallOpenInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwWallOpenMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwWallCloseInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwWallCloseMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwWallDispFunc(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwWallSetCol(OBS_COLLISION_WORK *col_work, fx32 size, fx32 draw_size, u16 wall_type);
static void gmGmkSwWallSetColDir(u8 *buf, s32 width, s32 height, BOOL wall);
static void gmGmkSwWallCheckColOff(GMS_GMK_SWWALL_WORK *swwall_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_sw_wall_obj_3d_list = NULL;

#if GMD_GMK_SW_WALL_TEST_TVX
static AMS_AMB_HEADER* g_gm_gmk_sw_wall3_obj_tvx_list = NULL;
#endif // GMD_GMK_SW_WALL_TEST_TVX
//static OBS_DATA_WORK gm_gmk_sw_wall_col_dir[GMD_GMK_SWWALL_COL_DIR_MAX] = {NULL};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSwWallBuild
/*!
 *	ギミック スイッチ壁 データ構築
 */
// ==========================================================================
void GmGmkSwWallBuild(void)
{
	gm_gmk_sw_wall_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SW_WALL_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SW_WALL_TEX),
								0/*draw_flag*/);
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
	GSE_MAIN_STAGE_ID stage_id = GsGetMainSysInfo()->stage_id;
	if (GSD_MAIN_STAGE_ID_3_1 <= stage_id && stage_id <= GSD_MAIN_STAGE_ID_3_BOSS) {
		tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_SW_WALL_TVX );
		amBindConv((Uint8*)tvx);
		g_gm_gmk_sw_wall33_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
	}
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
}

// ==========================================================================
// GmGmkSwWallFlush
/*!
 *	ギミック スイッチ壁 データ片付け
 */
// ==========================================================================
void GmGmkSwWallFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SW_WALL_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_sw_wall_obj_3d_list, amb->file_num);
	
#if GMD_GMK_BOSS3_PILLAR_TEST_TVX
	g_gm_gmk_sw_wall33_obj_tvx_list = NULL; // 特に問題ないので常にNULL
#endif // GMD_GMK_BOSS3_PILLAR_TEST_TVX
}

// ==========================================================================
// GmGmkSwWallInit
/*!
 *	ギミック スイッチ壁 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *			
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSwWallInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_SWWALL_WORK	*swwall_work;
//	OBS_RECT_WORK		*rect_work;
	OBS_COLLISION_WORK	*col_work;
	u16					wall_type = 0;
	u16					wall_thick;

	UNREFERENCED_PARAMETER(type);

	// オブジェクト生成
	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_SWWALL_WORK), "GMK_SW_WALL");

	gmk_work	= (GMS_ENEMY_3D_WORK*)obj_work;
	swwall_work	= (GMS_GMK_SWWALL_WORK*)obj_work;

	// モデル初期化
	// ベース
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_sw_wall_obj_3d_list[IDB_GMK_SW_WALL3_MDL_GMK_SW_WALL_3_ZNO],
					&gmk_work->obj_3d);

	if (GMD_EVENT_ID_GMK_SW_WALL_Z4_HR <= eve_rec->id &&
				eve_rec->id <= GMD_EVENT_ID_GMK_SW_WALL_Z4_VT) {
		// Zone4
		// 装飾ギア(モーションはないので開放不要)
		ObjCopyAction3dNNModel(&gm_gmk_sw_wall_obj_3d_list[IDB_GMK_SW_WALL4_MDL_GMK_SW_WALL_GEAR_ZNO],
					&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z4_GEAR]);
		// 装飾ギアベース(モーションはないので開放不要)
		ObjCopyAction3dNNModel(&gm_gmk_sw_wall_obj_3d_list[IDB_GMK_SW_WALL4_MDL_GMK_SW_WALL_GEAR_BASE_ZNO],
					&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z4_GEAR_BASE]);
		swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z4_GEAR_BASE].drawflag |= NND_DRAWOBJ_DOUBLESIDE;

		gmk_work->ene_com.enemy_flag |= GMD_GMK_ENEMY_FLAG_GEAR_OPT;	// 歯車オプションあり
	}
	else {
		// Zone3
#if !_IPHONE
		// 石
		ObjCopyAction3dNNModel(&gm_gmk_sw_wall_obj_3d_list[IDB_GMK_SW_WALL3_MDL_GMK_SW_WALL_S_3_ZNO],
					&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z3_STONE]);
		// グレア
		ObjCopyAction3dNNModel(&gm_gmk_sw_wall_obj_3d_list[IDB_GMK_SW_WALL3_MDL_GMK_SW_WALL_G_3_ZNO],
					&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z3_GLARE]);
		ObjAction3dNNMaterialMotionLoad(&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z3_GLARE], 0/*reg_file_id*/,
									ObjDataGet(GMD_DWORK_NO_GMK_SW_WALL_MAT), NULL/*filename*/,
									0/*index*/, NULL/*archive*/,
									1/*motion_num*/, 1/*mmotion_num*/);

		gmk_work->ene_com.enemy_flag |= GMD_GMK_ENEMY_FLAG_STONE_OPT;	// 石オプションあり
#endif // !_IPHONE
		// サウンドハンドル取得
		swwall_work->h_snd = GsSoundAllocSeHandle();
	}

	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkSwWallDest);

	// 優先設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_BACK;

	// 描画処理設定
	obj_work->ppOut = gmGmkSwWallDispFunc;

	/* スイッチID取得 */
	MTM_ASSERT((u8)eve_rec->left < GMD_GMK_SW_MAX);
	swwall_work->id = (u32)MTM_MATH_CLIP(eve_rec->left, 0, GMD_GMK_SW_MAX);

	/* 壁サイズ取得 */
	if (GMD_EVENT_ID_GMK_SW_WALL_Z3_HR <= eve_rec->id &&
				eve_rec->id <= GMD_EVENT_ID_GMK_SW_WALL_Z3_VT) {
		// ZONE3通常
		swwall_work->wall_size = GMD_GMK_SWWALL_Z3_WALL_SIZE*FX32_ONE;
		wall_type = (u16)(eve_rec->id - GMD_EVENT_ID_GMK_SW_WALL_Z3_HR);
		wall_thick = GMD_GMK_SWWALL_COL_Z3_THICK;
	}
	else if (GMD_EVENT_ID_GMK_SW_WALL_Z3_HR_L8 <= eve_rec->id &&
				eve_rec->id <= GMD_EVENT_ID_GMK_SW_WALL_Z3_VT_L8) {
		// ZONE3ロング
		swwall_work->wall_size = GMD_GMK_SWWALL_Z3_L_WALL_SIZE*FX32_ONE;
		wall_type = (u16)(eve_rec->id - GMD_EVENT_ID_GMK_SW_WALL_Z3_HR_L8);
		wall_thick = GMD_GMK_SWWALL_COL_Z3_THICK;
	}
	else {
		// ZONE4通常
		swwall_work->wall_size = GMD_GMK_SWWALL_Z4_WALL_SIZE*FX32_ONE;
		wall_type = (u16)(eve_rec->id - GMD_EVENT_ID_GMK_SW_WALL_Z4_HR);
		MTM_ASSERT(wall_type < GMD_GMK_SWWALL_TYPE_MAX);
		wall_thick = GMD_GMK_SWWALL_COL_Z4_THICK;

		// 装飾ギア設定
		swwall_work->gear_pos = obj_work->pos;
		swwall_work->gearbase_pos = obj_work->pos;
		switch (eve_rec->id) {
		default:
		case GMD_EVENT_ID_GMK_SW_WALL_Z4_HR:
			swwall_work->gear_pos.x += (fx32)(GMD_GMK_SWWALL_GEAR_OPT_OFST_X*FX32_ONE);
			swwall_work->gear_pos.y += (fx32)(GMD_GMK_SWWALL_GEAR_OPT_OFST_Y*FX32_ONE);
			swwall_work->gearbase_pos.x += (fx32)(GMD_GMK_SWWALL_GEARBASE_OPT_OFST_X*FX32_ONE);
			swwall_work->gearbase_pos.y += (fx32)(GMD_GMK_SWWALL_GEARBASE_OPT_OFST_Y*FX32_ONE);
			// 歯車をかみ合うようにずらす
			swwall_work->gear_base_dir = (u16)(0x10000 * -15 / 360);
			break;
		case GMD_EVENT_ID_GMK_SW_WALL_Z4_HL:
			swwall_work->gear_pos.x -= (fx32)(GMD_GMK_SWWALL_GEAR_OPT_OFST_X*FX32_ONE);
			swwall_work->gear_pos.y += (fx32)(GMD_GMK_SWWALL_GEAR_OPT_OFST_Y*FX32_ONE);
			swwall_work->gearbase_pos.x -= (fx32)(GMD_GMK_SWWALL_GEARBASE_OPT_OFST_X*FX32_ONE);
			swwall_work->gearbase_pos.y += (fx32)(GMD_GMK_SWWALL_GEARBASE_OPT_OFST_Y*FX32_ONE);
			// 歯車をかみ合うようにずらす
			swwall_work->gear_base_dir = (u16)(0x10000 * 15 / 360);
			// 歯車ベースをHフリップ表示する
			gmk_work->ene_com.enemy_flag |= GMD_GMK_ENEMY_FLAG_GEAR_BASE_HFLIP;

			break;
		case GMD_EVENT_ID_GMK_SW_WALL_Z4_VB:
			swwall_work->gear_pos.x += (fx32)(GMD_GMK_SWWALL_GEAR_OPT_OFST_Y*FX32_ONE);
			swwall_work->gear_pos.y += (fx32)(GMD_GMK_SWWALL_GEAR_OPT_OFST_X*FX32_ONE);
			swwall_work->gearbase_pos.x += (fx32)(GMD_GMK_SWWALL_GEARBASE_OPT_OFST_Y*FX32_ONE);
			swwall_work->gearbase_pos.y += (fx32)(GMD_GMK_SWWALL_GEARBASE_OPT_OFST_X*FX32_ONE);
			// 歯車をかみ合うようにずらす
			swwall_work->gear_base_dir = (u16)(0x10000 * (60/2+15) / 360);
			// 歯車ベースをHフリップ表示する
			gmk_work->ene_com.enemy_flag |= GMD_GMK_ENEMY_FLAG_GEAR_BASE_HFLIP;

			// 壁の回転量も設定
			obj_work->dir.z = 0xC000;
			break;
		case GMD_EVENT_ID_GMK_SW_WALL_Z4_VT:
			swwall_work->gear_pos.x += (fx32)(GMD_GMK_SWWALL_GEAR_OPT_OFST_Y*FX32_ONE);
			swwall_work->gear_pos.y -= (fx32)(GMD_GMK_SWWALL_GEAR_OPT_OFST_X*FX32_ONE);
			swwall_work->gearbase_pos.x += (fx32)(GMD_GMK_SWWALL_GEARBASE_OPT_OFST_Y*FX32_ONE);
			swwall_work->gearbase_pos.y -= (fx32)(GMD_GMK_SWWALL_GEARBASE_OPT_OFST_X*FX32_ONE);
			// 歯車をかみ合うようにずらす
			swwall_work->gear_base_dir = (u16)(0x10000 * (60/2-15) / 360);

			// 壁の回転量も設定
			obj_work->dir.z = 0xC000;
			break;
		}

	}
	// 基点NO保存
	swwall_work->wall_type = wall_type;

	// スイッチ状態チェック
	if (GmGmkSwitchTypeIsGear(swwall_work->id) &&
			!((GmGmkSwitchGetPer(swwall_work->id) == 0 && GmGmkSwitchIsOn(swwall_work->id)) ||
			(GmGmkSwitchGetPer(swwall_work->id) == FX32_ONE && !GmGmkSwitchIsOn(swwall_work->id)))) {
		fx32	per = GmGmkSwitchGetPer(swwall_work->id);
		// 歯車タイプで途中状態
		if (GmGmkSwitchIsOn(swwall_work->id)) {
			if (eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) {
				per = FX32_ONE - per;
			}
		}
		else {
			if (!(eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE)) {
				per = FX32_ONE - per;
			}
		}
		swwall_work->wall_draw_size = FX_Mul(swwall_work->wall_size, per);
	}
	else {
		// 通常タイプ
		if ((GmGmkSwitchIsOn(swwall_work->id) && (eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE)) ||
				(!GmGmkSwitchIsOn(swwall_work->id) && !(eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE))) {
			// 閉まっている
			swwall_work->wall_draw_size = swwall_work->wall_size;
		}

	}
	// ◆途中状態の時チェック

	/* 地形設定 */
	col_work = &gmk_work->ene_com.col_work;
	col_work->obj_col.obj		= obj_work;
	col_work->obj_col.dir_data	= swwall_work->col_dir_buf;		// 角度バッファ設定
	col_work->obj_col.diff_data	= (s8*)g_gm_default_col;		// 基本地形情報
	col_work->obj_col.flag		|= OBD_COLOBJ_NODIR_PARENT |
							OBD_COLOBJ_NOFREE_DIR_DATA | OBD_COLOBJ_NOFREE_DIFF_DATA;
	if (wall_type <= GMD_GMK_SWWALL_TYPE_LEFT) {
		// 横向き壁
		col_work->obj_col.height	= wall_thick;				// 地形サイズ設定(ドット)
		col_work->obj_col.ofst_y	= GMD_GMK_SWWALL_COL_OFST_Y;
	}
	else {
		// 縦向き壁
		col_work->obj_col.width		= wall_thick;				// 地形サイズ設定(ドット)
		col_work->obj_col.ofst_x	= GMD_GMK_SWWALL_COL_OFST_X;
	}
	gmGmkSwWallSetCol(col_work, swwall_work->wall_size, swwall_work->wall_draw_size, wall_type);

	/* フラグ設定 */
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形チェックなし
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	gmk_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOPRESSDIE;	// プレイヤーを圧死させない

	// シーケンス設定
	if (swwall_work->wall_draw_size == 0 ||
				swwall_work->wall_draw_size == swwall_work->wall_size) {
		gmGmkSwWallFwInit(obj_work);
	}
	else {
		if ((GmGmkSwitchIsOn(swwall_work->id) && (eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE)) ||
				(!GmGmkSwitchIsOn(swwall_work->id) && !(eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE))) {
			// 閉まりかけ
			gmGmkSwWallCloseInit(obj_work);
		}
		else {
			// 開きかけ
			gmGmkSwWallOpenInit(obj_work);
		}
	}

	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSwWallDest
/*!
 *	ギミック スイッチ壁 デストラクタ
 *
 *	@param tcb	[in] TCB
 */
// ==========================================================================
void gmGmkSwWallDest(MTS_TASK_TCB *tcb)
{
	GMS_GMK_SWWALL_WORK	*swwall_work;

	swwall_work = (GMS_GMK_SWWALL_WORK*)mtTaskGetTcbWork(tcb);

	if (swwall_work->gmk_work.ene_com.enemy_flag & GMD_GMK_ENEMY_FLAG_STONE_OPT) {
		// グレアモーション解放
		ObjAction3dNNMotionRelease(&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z3_GLARE]);
	}

	// サウンドハンドル解放
	if (swwall_work->h_snd) {
		GmSoundStopSE(swwall_work->h_snd);
		GsSoundFreeSeHandle(swwall_work->h_snd);
		swwall_work->h_snd = NULL;
	}

	// 標準終了処理
	GmEnemyDefaultExit(tcb);
}

// ==========================================================================
// gmGmkSwWallFwInit
/*!
 *	ギミック スイッチ壁 FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwWallFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SWWALL_WORK		*swwall_work = (GMS_GMK_SWWALL_WORK*)obj_work;

	// クリッピングON
	obj_work->flag &= ~OBD_OBJECT_NOCLIP;

	// メイン処理設定
	obj_work->ppFunc = gmGmkSwWallFwMain;

	// 地形有効
	obj_work->col_work->obj_col.obj = obj_work;

	// SE停止
	if (swwall_work->h_snd) {
		GmSoundStopSE(swwall_work->h_snd);
	}
}

// ==========================================================================
// gmGmkSwWallFwMain
/*!
 *	ギミック スイッチ壁 FW処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwWallFwMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SWWALL_WORK		*swwall_work = (GMS_GMK_SWWALL_WORK*)obj_work;
	GMS_EVE_RECORD_EVENT	*eve_rec = swwall_work->gmk_work.ene_com.eve_rec;

	// スイッチ状態チェック
	if (swwall_work->wall_draw_size) {
		if ((!(eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) && GmGmkSwitchIsOn(swwall_work->id)) ||
					((eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) && !GmGmkSwitchIsOn(swwall_work->id))) {
			// 壁オープン
			gmGmkSwWallOpenInit(obj_work);
		}
	}
	else {
		if (((eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) && GmGmkSwitchIsOn(swwall_work->id)) ||
					(!(eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) && !GmGmkSwitchIsOn(swwall_work->id))) {
			// 壁クローズ
			gmGmkSwWallCloseInit(obj_work);
		}
	}

	// 地形OFFチェック
	gmGmkSwWallCheckColOff(swwall_work);
}

// ==========================================================================
// gmGmkSwWallOpenInit
/*!
 *	ギミック スイッチ壁 オープン初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwWallOpenInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SWWALL_WORK		*swwall_work = (GMS_GMK_SWWALL_WORK*)obj_work;

	// 壁速度取得
	if (GmGmkSwitchTypeIsGear(swwall_work->id)) {
		// 歯車スイッチ
		swwall_work->wall_spd = 0;
	}
	else {
		// 通常スイッチ
		swwall_work->wall_spd = GMD_GMK_SWWALL_DEF_SPD*FX32_ONE;
	}

	// クリッピングOFF
	obj_work->flag |= OBD_OBJECT_NOCLIP;

	// メイン処理設定
	obj_work->ppFunc = gmGmkSwWallOpenMain;

	// 地形有効
	obj_work->col_work->obj_col.obj = obj_work;

	// SE
	if (swwall_work->h_snd) {
		GmSoundStopSE(swwall_work->h_snd);
		GmSoundPlaySE("Boss3_01", swwall_work->h_snd);
	}
}

// ==========================================================================
// gmGmkSwWallOpenMain
/*!
 *	ギミック スイッチ壁 オープン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwWallOpenMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SWWALL_WORK		*swwall_work = (GMS_GMK_SWWALL_WORK*)obj_work;
	fx32					per;

	// スイッチ逆転チェック
	if (((swwall_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) &&
				GmGmkSwitchIsOn(swwall_work->id)) ||
			(!(swwall_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) &&
				!GmGmkSwitchIsOn(swwall_work->id))) {
		// CLOSEへ
		gmGmkSwWallCloseInit(obj_work);
	}

	// 壁移動
	if (swwall_work->wall_spd) {
		swwall_work->wall_draw_size -= swwall_work->wall_spd;
	}
	else {
		// 展開量を常にチェック
		per = GmGmkSwitchGetPer(swwall_work->id);
		if (swwall_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) {
			per = FX32_ONE - per;
		}
		swwall_work->wall_draw_size = FX_Mul(swwall_work->wall_size, per);
	}
	if (swwall_work->wall_draw_size <= 0) {
		// オープン終了
		swwall_work->wall_draw_size = 0;

		// FWへ
		gmGmkSwWallFwInit(obj_work);
	}

	// 歯車回転
	swwall_work->gear_dir = (u16)(swwall_work->wall_draw_size / (GMD_GMK_SWWALL_BLOCK_SIZE*2) * 16);
	if (swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_HL ||
			swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_VB) {
		swwall_work->gear_dir = (u16)-swwall_work->gear_dir;
	}
#if 0
	if (swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_HL ||
			swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_VT) {
		swwall_work->gear_dir += (u16)(0x10000 * (60/2) / 360);	// 歯車半分の回転量をずらす
	}
	if (swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_HL ||
			swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_VB) {
		swwall_work->gear_dir = (u16)-swwall_work->gear_dir;
	}
#endif

	// 地形設定
	gmGmkSwWallSetCol(&swwall_work->gmk_work.ene_com.col_work,
			swwall_work->wall_size, swwall_work->wall_draw_size,
			swwall_work->wall_type);
}


// ==========================================================================
// gmGmkSwWallCloseInit
/*!
 *	ギミック スイッチ壁 クローズ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwWallCloseInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SWWALL_WORK		*swwall_work = (GMS_GMK_SWWALL_WORK*)obj_work;

	// 壁速度取得
	if (GmGmkSwitchTypeIsGear(swwall_work->id)) {
		// 歯車スイッチ
		swwall_work->wall_spd = 0;
	}
	else {
		// 通常スイッチ
		swwall_work->wall_spd = GMD_GMK_SWWALL_DEF_SPD*FX32_ONE;
	}

	// クリッピングOFF
	obj_work->flag |= OBD_OBJECT_NOCLIP;

	// メイン処理設定
	obj_work->ppFunc = gmGmkSwWallCloseMain;

	// 地形有効
	obj_work->col_work->obj_col.obj = obj_work;

	// SE
	if (swwall_work->h_snd) {
		GmSoundStopSE(swwall_work->h_snd);
		GmSoundPlaySE("Boss3_01", swwall_work->h_snd);
	}
}

// ==========================================================================
// gmGmkSwWallCloseMain
/*!
 *	ギミック スイッチ壁 クローズ処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwWallCloseMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SWWALL_WORK		*swwall_work = (GMS_GMK_SWWALL_WORK*)obj_work;
	fx32					per;

	// スイッチ逆転チェック
	if (((swwall_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) &&
				!GmGmkSwitchIsOn(swwall_work->id)) ||
			(!(swwall_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) &&
				GmGmkSwitchIsOn(swwall_work->id))) {
		// OPENへ
		gmGmkSwWallOpenInit(obj_work);
	}

	// 壁移動
	if (swwall_work->wall_spd) {
		swwall_work->wall_draw_size += swwall_work->wall_spd;
	}
	else {
		// 展開量を常にチェック
		per = GmGmkSwitchGetPer(swwall_work->id);
		if (swwall_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_SWWALL_SW_CLOSE) {
			per = FX32_ONE - per;
		}
		swwall_work->wall_draw_size = FX_Mul(swwall_work->wall_size, per);
	}
	if (swwall_work->wall_draw_size >= swwall_work->wall_size) {
		// クローズ終了
		swwall_work->wall_draw_size = swwall_work->wall_size;

		// FWへ
		gmGmkSwWallFwInit(obj_work);
	}

	// 歯車回転
	swwall_work->gear_dir = (u16)(swwall_work->wall_draw_size / (GMD_GMK_SWWALL_BLOCK_SIZE*2) * 16);
	if (swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_HL ||
			swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_VB) {
		swwall_work->gear_dir = (u16)-swwall_work->gear_dir;
	}
#if 0
	if (swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_HL ||
			swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_VT) {
		swwall_work->gear_dir += (u16)(0x10000 * (60/2) / 360);	// 歯車半分の回転量をずらす
	}
	if (swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_HL ||
			swwall_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z4_VB) {
		swwall_work->gear_dir = (u16)-swwall_work->gear_dir;
	}
#endif

	// 地形設定
	gmGmkSwWallSetCol(&swwall_work->gmk_work.ene_com.col_work,
			swwall_work->wall_size, swwall_work->wall_draw_size,
			swwall_work->wall_type);

	// 地形OFFチェック
	gmGmkSwWallCheckColOff(swwall_work);
}


// ==========================================================================
// gmGmkSwWallDispFunc
/*!
 *	ギミック スイッチ壁 描画処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwWallDispFunc(OBS_OBJECT_WORK *obj_work)
{
	s32						draw_num;
	GMS_GMK_SWWALL_WORK		*swwall_work = (GMS_GMK_SWWALL_WORK*)obj_work;
	OBS_COLLISION_WORK		*col_work;
	VecFx32					pos, scale;
	VecU16					dir;
	fx32					pos_add_x, pos_add_y;
	u32						disp_flag, glare_disp_flag;

	// 壁描画数取得
	draw_num = (swwall_work->wall_draw_size + (GMD_GMK_SWWALL_BLOCK_SIZE*FX32_ONE-1)) / (GMD_GMK_SWWALL_BLOCK_SIZE*FX32_ONE);

	// 描画基点取得
	col_work = &swwall_work->gmk_work.ene_com.col_work;
	pos = obj_work->pos;
	pos_add_x = pos_add_y = 0;
	if (swwall_work->wall_type <= GMD_GMK_SWWALL_TYPE_LEFT) {
		// 横向き
		if (swwall_work->wall_type == GMD_GMK_SWWALL_TYPE_RIGHT) {
			pos.x += (col_work->obj_col.width + col_work->obj_col.ofst_x - GMD_GMK_SWWALL_BLOCK_SIZE/2) << FX32_SHIFT;
			pos_add_x = -GMD_GMK_SWWALL_BLOCK_SIZE*FX32_ONE;	// 左へ描画
		}
		else {
			pos.x += (col_work->obj_col.ofst_x + GMD_GMK_SWWALL_BLOCK_SIZE/2) << FX32_SHIFT;
			pos_add_x = GMD_GMK_SWWALL_BLOCK_SIZE*FX32_ONE;		// 右へ描画
		}
	}
	else {
		// 縦向き
		if (swwall_work->wall_type == GMD_GMK_SWWALL_TYPE_BOTTOM) {
			pos.y += (col_work->obj_col.height + col_work->obj_col.ofst_y - GMD_GMK_SWWALL_BLOCK_SIZE/2) << FX32_SHIFT;
			pos_add_y = -GMD_GMK_SWWALL_BLOCK_SIZE*FX32_ONE;	// 上へ描画
		}
		else {
			pos.y += (col_work->obj_col.ofst_y + GMD_GMK_SWWALL_BLOCK_SIZE/2) << FX32_SHIFT;
			pos_add_y = GMD_GMK_SWWALL_BLOCK_SIZE*FX32_ONE;		// 下へ描画
		}
	}

	// disp_flag 取得
	disp_flag = obj_work->disp_flag;

#if GMD_GMK_SW_WALL_TEST_TVX
	void* model_tvx = NULL;
	if (g_gm_gmk_sw_wall3_obj_tvx_list) {
		model_tvx = amBindGet(g_gm_gmk_sw_wall3_obj_tvx_list, 0);
	}
#endif // GMD_GMK_SW_WALL_TEST_TVX

	// 本体描画
	if (swwall_work->gmk_work.ene_com.enemy_flag & GMD_GMK_ENEMY_FLAG_STONE_OPT) {
#if !_IPHONE
		glare_disp_flag = disp_flag | OBD_DISP_REPEAT;
		
		if (!ObjObjectPauseCheck(0)) {
			// マテリアルモーションアップデート
			ObjDrawAction3DNNMaterialUpdate(&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z3_GLARE],
				&glare_disp_flag);
		}
		// アップデートOFF
		glare_disp_flag |= OBD_DISP_NOUPDATE;
#endif // !_IPHONE
		for (;draw_num > 0; draw_num--, pos.x += pos_add_x, pos.y += pos_add_y) {
#if GMD_GMK_SW_WALL_TEST_TVX
			if (model_tvx) {
				// Z3 SW WALL
				GmTvxSetModel(model_tvx, obj_work->obj_3d->texlist, &pos, &obj_work->scale, 0, 0);
			}
			else {
				// Z4歯車付き SW WALL
				ObjDrawAction3DNN(obj_work->obj_3d, &pos, &obj_work->dir, &obj_work->scale, &disp_flag);
			}
#else
			ObjDrawAction3DNN(obj_work->obj_3d, &pos, &obj_work->dir, &obj_work->scale, &disp_flag);
#endif // GMD_GMK_SW_WALL_TEST_TVX
#if !_IPHONE
			// 石
			ObjDrawAction3DNN(&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z3_STONE],
					&pos, &obj_work->dir, &obj_work->scale, &disp_flag);
			// グレア
			ObjDrawAction3DNN(&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z3_GLARE],
					&pos, &obj_work->dir, &obj_work->scale, &glare_disp_flag);
#endif // !_IPHONE
		}
	}
	else {
		for (;draw_num > 0; draw_num--, pos.x += pos_add_x, pos.y += pos_add_y) {
			ObjDrawAction3DNN(obj_work->obj_3d, &pos, &obj_work->dir, &obj_work->scale, &disp_flag);
#if !_IPHONE
			if (swwall_work->gmk_work.ene_com.enemy_flag & GMD_GMK_ENEMY_FLAG_STONE_OPT) {
				// 石
				ObjDrawAction3DNN(&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z3_STONE],
						&pos, &obj_work->dir, &obj_work->scale, &disp_flag);
				// グレア
				ObjDrawAction3DNN(&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z3_GLARE],
						&pos, &obj_work->dir, &obj_work->scale, &glare_disp_flag);
			}
#endif // !_IPHONE
		}
	}

	// オプションモデル描画
#if 1
	if (swwall_work->gmk_work.ene_com.enemy_flag & GMD_GMK_ENEMY_FLAG_GEAR_OPT) {
		// 歯車
		dir.x = dir.y = 0;
		dir.z = (u16)(swwall_work->gear_dir + swwall_work->gear_base_dir);

		ObjDrawAction3DNN(&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z4_GEAR],
				&swwall_work->gear_pos, &dir, &obj_work->scale, &disp_flag);

		// 歯車ベース
		scale.x = scale.y = scale.z = FX32_ONE;
		if (swwall_work->gmk_work.ene_com.enemy_flag & GMD_GMK_ENEMY_FLAG_GEAR_BASE_HFLIP) {
			scale.x = -scale.x;
		}
		ObjDrawAction3DNN(&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z4_GEAR_BASE],
				&swwall_work->gearbase_pos, &obj_work->dir, &scale, &disp_flag);
	}
#else
	if (swwall_work->gmk_work.ene_com.enemy_flag & GMD_GMK_ENEMY_FLAG_GEAR_OPT) {
		dir.x = dir.y = 0;
		dir.z = swwall_work->gear_dir;

		ObjDrawAction3DNN(&swwall_work->obj_3d_opt[GMD_GMK_SW_WALL_OPT_MDL_Z4_GEAR],
				&swwall_work->gear_pos, &dir, &obj_work->scale, &disp_flag);
	}
#endif
}


// ==========================================================================
// gmGmkSwWallSetCol
/*!
 *	地形再設定
 *
 *	@param	col_work	[in]	コリジョンワーク
 *	@param	size		[in]	壁サイズ
 *	@param	draw_size	[in]	描画サイズ
 *	@param	wall_type	[in]	壁タイプ
 */
// ==========================================================================
void gmGmkSwWallSetCol(OBS_COLLISION_WORK *col_work, fx32 size, fx32 draw_size, u16 wall_type)
{
#if 1
	/* 地形設定 */
	if (wall_type <= GMD_GMK_SWWALL_TYPE_LEFT) {
		// 横向き
		col_work->obj_col.width	= (u16)(((draw_size >> FX32_SHIFT) + 7) & 0xFFFFFFF8);	// キャラ単位サイズに変換
		if (wall_type == GMD_GMK_SWWALL_TYPE_RIGHT) {
			col_work->obj_col.ofst_x = (s16)((-size >> (FX32_SHIFT + 1)) -
												(col_work->obj_col.width - (draw_size>>FX32_SHIFT)));
									//  -size/FX32_ONE/2;
		}
		else {
			col_work->obj_col.ofst_x = (s16)(((size>>1) - draw_size) >> FX32_SHIFT) ;
									// (size/2 - draw_size)/FX32_ONE;
		}

		// 地形角度設定
		gmGmkSwWallSetColDir(col_work->obj_col.dir_data,
						(col_work->obj_col.width + 7) >> 3/* /8 */,
						(col_work->obj_col.height + 7) >> 3/* /8 */,
						FALSE);
	}
	else {
		// 縦向き
		col_work->obj_col.height= (u16)(((draw_size >> FX32_SHIFT) + 7) & 0xFFFFFFF8);	// キャラ単位サイズに変換
		if (wall_type == GMD_GMK_SWWALL_TYPE_BOTTOM) {
			col_work->obj_col.ofst_y = (s16)((-size >> (FX32_SHIFT+1)) -
												(col_work->obj_col.height - (draw_size>>FX32_SHIFT)));
									// -size/2/FX32_ONE;
		}
		else {
			col_work->obj_col.ofst_y = (s16)(((size>>1) - draw_size) >> FX32_SHIFT);
									// (size/2 - draw_size)/FX32_ONE;
		}

		// 地形角度設定
		gmGmkSwWallSetColDir(col_work->obj_col.dir_data,
						(col_work->obj_col.width + 7) >> 3/* /8 */,
						(col_work->obj_col.height + 7) >> 3/* /8 */,
						TRUE);
	}
#else
	/* 地形設定 */
	if (wall_type <= GMD_GMK_SWWALL_TYPE_LEFT) {
		// 横向き
		col_work->obj_col.width	= (u16)(draw_size >> FX32_SHIFT);
		if (wall_type == GMD_GMK_SWWALL_TYPE_RIGHT) {
			col_work->obj_col.ofst_x = (s16)(-size >> (FX32_SHIFT + 1));
									//  -size/FX32_ONE/2;
		}
		else {
			col_work->obj_col.ofst_x = (s16)(((size>>1) - draw_size) >> FX32_SHIFT);
									// (size/2 - draw_size)/FX32_ONE;
		}

		// 地形角度設定
		gmGmkSwWallSetColDir(col_work->obj_col.dir_data,
						(col_work->obj_col.width + 7) >> 3/* /8 */,
						(col_work->obj_col.height + 7) >> 3/* /8 */,
						FALSE);
	}
	else {
		// 縦向き
		col_work->obj_col.height= (u16)(draw_size >> FX32_SHIFT);
		if (wall_type == GMD_GMK_SWWALL_TYPE_BOTTOM) {
			col_work->obj_col.ofst_y = (s16)(-size >> (FX32_SHIFT+1));
									// -size/2/FX32_ONE;
		}
		else {
			col_work->obj_col.ofst_y = (s16)(((size>>1) - draw_size) >> FX32_SHIFT);
									// (size/2 - draw_size)/FX32_ONE;
		}

		// 地形角度設定
		gmGmkSwWallSetColDir(col_work->obj_col.dir_data,
						(col_work->obj_col.width + 7) >> 3/* /8 */,
						(col_work->obj_col.height + 7) >> 3/* /8 */,
						TRUE);
	}
#endif
}



// ==========================================================================
// gmGmkSwWallSetColDir
/*!
 *	地形角度再設定
 *
 *	@param	buf			[in]	角度格納バッファ(width*height以上)
 *	@param	width		[in]	壁サイズ(キャラ)
 *	@param	height		[in]	壁サイズ(キャラ)
 *	@param	wall		[in]	タイプ wall:壁(縦) FALSE:床(横)
 */
// ==========================================================================
void gmGmkSwWallSetColDir(u8 *buf, s32 width, s32 height, BOOL wall)
{
	s32				i, j;
	s32				width_half, height_half;

	u8				*col_dir_p1, *col_dir_p2;	

	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_3_2) {
		// 3-2以外は設定不要
		return;
	}

	MTM_ASSERT(buf);

	if (width <= 0 || height <= 0) {
		//MTM_ASSERT(!"gmGmkSwWall.cpp::gmGmkSwWallSetColDir() size zero\n");
		return;
	}

	// 半サイズ取得
	width_half = width >> 1;
	height_half = height >> 1;

	if (wall) {
	// 縦壁
		col_dir_p1 = buf;
		for (i = 0; i < height; i++, col_dir_p1 += width) {
			// 左半分
			col_dir_p2 = col_dir_p1;
			for (j = 0; j < width_half; j++, col_dir_p2++) {
				*col_dir_p2 = 0xC0;
			}
			// 右半分
			for (; j < width; j++, col_dir_p2++) {
				*col_dir_p2 = 0x40;
			}
		}
		// 上下調整(両端を除いて上下角度を入れる)
		// 上
		col_dir_p1 = buf + 1;
		for (i = 1; i < width - 1; i++, col_dir_p1++) {
			*col_dir_p1 = 0x00;
		}
		// 下
		if (height > 1) {
			col_dir_p1 = buf + (height - 1)*width + 1;
			for (i = 1; i < width - 1; i++, col_dir_p1++) {
				*col_dir_p1 = 0x80;
			}
		}
	}
	else {
	// 横床
		col_dir_p1 = buf;
		// 上半分
		for (i = 0; i < height_half; i++) {
			for (j = 0; j < width; j++, col_dir_p1++) {
				*col_dir_p1 = 0x00;
			}
		}
		// 下半分
		for (; i < height; i++) {
			for (j = 0; j < width; j++, col_dir_p1++) {
				*col_dir_p1 = 0x80;
			}
		}
		// 左右調整(上下端を除いて左右角度を入れる)
		// 左
		col_dir_p1 = buf + width/* *1 */;
		for (i = 1; i < height - 1; i++, col_dir_p1+=width) {
			*col_dir_p1 = 0xC0;
		}
		// 右
		if (width > 1) {
			col_dir_p1 = buf + width/* *1 */ + (width-1);
			for (i = 1; i < height - 1; i++, col_dir_p1+=width) {
				*col_dir_p1 = 0x40;
			}
		}
	}
}


// ==========================================================================
// gmGmkSwWallCheckColOff
/*!
 *	地形OFFチェック
 *
 *	@param	swwall_work	[in]	SW壁ワーク
 */
// ==========================================================================
void gmGmkSwWallCheckColOff(GMS_GMK_SWWALL_WORK *swwall_work)
{
	OBS_OBJECT_WORK			*obj_work;
	GMS_EVE_RECORD_EVENT	*eve_rec;
	OBS_OBJECT_WORK			*ply_obj;

	// Zone3-2 のみ
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_3_2) {
		return;
	}

	obj_work	= (OBS_OBJECT_WORK*)swwall_work;
	eve_rec		= swwall_work->gmk_work.ene_com.eve_rec;
	if (eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z3_HR ||
			eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z3_HL ||
			eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z3_HR_L8 ||
			eve_rec->id == GMD_EVENT_ID_GMK_SW_WALL_Z3_HL_L8) {
		// 横壁にプレイヤーが挟まりそうな時は下に落とす
		ply_obj = (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

		s32	ply_pos_x = ply_obj->pos.x >> FX32_SHIFT;
		s32	ply_pos_y = ply_obj->pos.y >> FX32_SHIFT;

		s32	obj_pos_x = obj_work->pos.x >> FX32_SHIFT;
		s32	obj_pos_y = obj_work->pos.y >> FX32_SHIFT;
		s32 wall_size = swwall_work->wall_size >> FX32_SHIFT;

		if (((obj_pos_x - (wall_size >> 1)) < (ply_pos_x + ply_obj->field_rect[MTD_RIGHT])) &&
				((obj_pos_x + (wall_size >> 1)) > (ply_pos_x + ply_obj->field_rect[MTD_LEFT])) &&
			(obj_pos_y + obj_work->col_work->obj_col.ofst_y + 8/*調整値*/ <= 
					(ply_pos_y + ply_obj->field_rect[MTD_BOTTOM])) &&
				(obj_pos_y + obj_work->col_work->obj_col.ofst_y + obj_work->col_work->obj_col.height >
					(ply_pos_y + ply_obj->field_rect[MTD_TOP]))) {
			// 地形無効
			obj_work->col_work->obj_col.obj = NULL;
		}
		else {
			// 地形有効
			obj_work->col_work->obj_col.obj = obj_work;
		}

	}
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
