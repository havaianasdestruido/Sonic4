// =======================================================================
/*!
  @file	gmBoss5Land.cpp
  @brief ボスファイナル 足場

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5Land.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "gmMain.h"
#include "gmEventTbl.h"
#include "gmEnemy.h"
#include "gmBoss5.h"
#include "gmBoss5Rocket.h"
#include "gmBoss5Turret.h"
#include "gmBOss5Egg.h"
#include "gmBoss5Efct.h"

#include "gmBoss5Land.h"

// データヘッダ
#include "../file/common/arc/BOSS05.hmb"
#include "../file/common/model/BOSS05_MDL.hmb"

/*------ Macros --------------------------------------------------------*/
//############ 足場 ###########################################################
/* フラグ */
#define GMD_BOSS5_LAND_FLAG_SHAKE_ACTIVE	(1 << 0)	//!< 足場振動有効フラグ
#define GMD_BOSS5_LAND_FLAG_BREAK_ACTIVE	(1 << 1)	//!< 足場崩壊有効フラグ

//############ 足場構成パーツ #################################################
/* 定義値 */
#define GMD_BOSS5_LAND_PLACE_PTN_LEN		(3)		//!< 足場パーツ配置パターンの、1パターンの構成パーツ数

#define GMD_BOSS5_LAND_LDPART_SPIN_ROT_AXIS_NUM	(2)					//!< パーツを回転させる軸の数
#define GMD_BOSS5_LAND_LDPART_SPIN_ROT_SPD_DEG	(AKM_DEGtoA32(1))	//!< パーツ落下回転時の回転速度（1軸分）

//! 足場パーツのサイズ（整数値）
#define GMD_BOSS5_LAND_LDPART_WIDTH_INT		(64)
#define GMD_BOSS5_LAND_LDPART_HEIGHT_INT	(64)
//! 足場パーツのサイズ（固定小数値）
#define GMD_BOSS5_LAND_LDPART_WIDTH_FX		((fx32)(FX32_ONE * GMD_BOSS5_LAND_LDPART_WIDTH_INT))
#define GMD_BOSS5_LAND_LDPART_HEIGHT_FX		((fx32)(FX32_ONE * GMD_BOSS5_LAND_LDPART_HEIGHT_INT))

//! 足場パーツ中心のオフセット（期待した中心位置から実際の中心位置への差分）
#define GMD_BOSS5_LAND_LDPART_CENTER_OFST_X_FX	((fx32)(FX32_ONE * -3))
#define GMD_BOSS5_LAND_LDPART_CENTER_OFST_Y_FX	((fx32)(FX32_ONE * 6))

// 落下関係
#define GMD_BOSS5_LAND_LDPART_FALL_Z_SPD_MAX	((fx32)(FX32_ONE * 1.5f))	//!< 落下時のZ方向初速 最大絶対速度
#define GMD_BOSS5_LAND_LDPART_FALL_XY_SPD_FL	(5.f)						//!< 落下時のXY方向初速度
#define GMD_BOSS5_LAND_LDPART_FALL_XY_DIR_RANGE_DEG	((Sint32)60)			//!< 落下時の上方向拡散範囲角度

// 振動関係
#define GMD_BOSS5_LAND_LDPART_VIB_PHASE_NUM	(40)					//!< 振動処理フェーズ数
#define GMD_BOSS5_LAND_LDPART_VIB_AMPLITUDE	((fx32)(FX32_ONE * 2))	//!< 振動振幅


/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! 足場構成パーツタイプ列挙型
typedef enum
{
	GME_BOSS5_LAND_TYPE_01,
	GME_BOSS5_LAND_TYPE_02,
	GME_BOSS5_LAND_TYPE_03,
	
	GME_BOSS5_LAND_TYPE_MAX
} GME_BOSS5_LAND_TYPE;

//! 足場配置情報構造体
typedef struct tag_GMS_BOSS5_LAND_PLACEMENT_INFO
{
	fx32	pos_x;
	fx32	pos_y;
	Sint32	part_num;
} GMS_BOSS5_LAND_PLACEMENT_INFO;


/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
//############ 共通 ###########################################################
static BOOL gmBoss5LandGetPlacementInfo(GMS_BOSS5_LAND_PLACEMENT_INFO *place_info);

//############ 足場 ###########################################################
/* 補助関数 */
static void gmBoss5LandSetObjCollisionRect(GMS_BOSS5_LAND_WORK *land_work, Sint32 part_num);
static void gmBoss5LandDisableObjCollision(GMS_BOSS5_LAND_WORK *land_work);
/* 処理関数 */
/* 制御処理 */
static void gmBoss5LandMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
static void gmBoss5LandProcInit(GMS_BOSS5_LAND_WORK *land_work);
static void gmBoss5LandProcUpdateIdle(GMS_BOSS5_LAND_WORK *land_work);
static void gmBoss5LandProcUpdateShake(GMS_BOSS5_LAND_WORK *land_work);

//############ 足場構成パーツ #################################################
static GMS_BOSS5_LDPART_WORK* gmBoss5LandCreateLdPart(GMS_BOSS5_LAND_WORK *land_work,
													  GME_BOSS5_LAND_TYPE land_type,
													  Sint32 part_index);
/* 補助関数 */
static void gmBoss5LdPartInitSpin(GMS_BOSS5_LDPART_WORK *ldpart_work);
static void gmBoss5LdPartUpdateSpin(GMS_BOSS5_LDPART_WORK *ldpart_work);
static void gmBoss5LdPartInitFall(GMS_BOSS5_LDPART_WORK *ldpart_work);
static void gmBoss5LdPartInitVib(GMS_BOSS5_LDPART_WORK *ldpart_work);
static void gmBoss5LdPartUpdateVib(GMS_BOSS5_LDPART_WORK *ldpart_work);
static void gmBoss5LdPartClearVib(GMS_BOSS5_LDPART_WORK *ldpart_work);
/* 処理関数 */
/* 制御処理 */
static void gmBoss5LdPartMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
static void gmBoss5LdPartProcInit(GMS_BOSS5_LDPART_WORK *ldpart_work);
static void gmBoss5LdPartProcUpdateIdle(GMS_BOSS5_LDPART_WORK *ldpart_work);
static void gmBoss5LdPartProcUpdateShake(GMS_BOSS5_LDPART_WORK *ldpart_work);
static void gmBoss5LdPartProcUpdateFall(GMS_BOSS5_LDPART_WORK *ldpart_work);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! 足場構成パーツモデルAMBインデックステーブル
const static Sint32 gm_boss5_land_mdl_amb_idx_tbl[GME_BOSS5_LAND_TYPE_MAX]	= {
	IDB_BOSS05_MDL_GMK_FINAL_LAND_01_ZNO,
	IDB_BOSS05_MDL_GMK_FINAL_LAND_02_ZNO,
	IDB_BOSS05_MDL_GMK_FINAL_LAND_03_ZNO,
};

//! 足場構成パーツマテリアルモーションデータテーブル
const static Sint32 gm_boss5_land_mat_mtn_data_tbl[GME_BOSS5_LAND_TYPE_MAX]	= {
	IDB_BOSS05_BOSS05_LAND01_MTN_AMB,
	IDB_BOSS05_BOSS05_LAND02_MTN_AMB,
	IDB_BOSS05_BOSS05_LAND03_MTN_AMB,
};
//! 足場構成パーツマテリアルモーションデータワーク番号テーブル
const static Sint32 gm_boss5_land_mat_mtn_dwork_no_tbl[GME_BOSS5_LAND_TYPE_MAX]	= {
	GMD_DWORK_NO_BOSS_05_LAND01_MAT,
	GMD_DWORK_NO_BOSS_05_LAND02_MAT,
	GMD_DWORK_NO_BOSS_05_LAND03_MAT,
};

//! 足場構成パーツ配置パターンテーブル
const static GME_BOSS5_LAND_TYPE gm_boss5_land_place_pattern_tbl[GMD_BOSS5_LAND_PLACE_PTN_LEN]	= {
	GME_BOSS5_LAND_TYPE_01,
	GME_BOSS5_LAND_TYPE_02,
	GME_BOSS5_LAND_TYPE_03,
};

//! 足場構成パーツ振動テーブル
const static fx32 gm_boss5_land_vib_tbl[GMD_BOSS5_LAND_LDPART_VIB_PHASE_NUM][MTD_XY]	= {
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * -1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * -1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * -1.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * -0.7f),	(fx32)(FX32_ONE * 0.7f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * 0.7f),	(fx32)(FX32_ONE * -0.7f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * -1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * -1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * -1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 1.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
	{	(fx32)(FX32_ONE * 0.0f),	(fx32)(FX32_ONE * 0.0f),	},
};

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmBoss5LandInit
/*!
  ボスFINAL 足場初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss5LandInit(GMS_EVE_RECORD_EVENT *eve_rec,
								 fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	UNREFERENCED_PARAMETER(pos_x);
	UNREFERENCED_PARAMETER(pos_y);
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_3d;
	GMS_BOSS5_LAND_WORK	*land_work;
	GMS_BOSS5_LAND_PLACEMENT_INFO	place_info;
	
	// 配置情報を取得
	if (FALSE == gmBoss5LandGetPlacementInfo(&place_info)) {
		return NULL;
	}
	
	// オブジェクトワーク生成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										place_info.pos_x,
										place_info.pos_y,
										sizeof(GMS_BOSS5_LAND_WORK),
										"BOSS5_LAND");
	// MEMO : オブジェクトタイプはイベントIDで判別して設定される
	
	gmk_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	land_work	= (GMS_BOSS5_LAND_WORK*)obj_work;
	
	// B面の位置に表示させる
	obj_work->pos.z	= GMD_OBJ_DEFAULT_POS_Z_B;
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->flag	&= ~OBD_OBJECT_PARENT_FIX;
	obj_work->disp_flag	&= ~OBD_DISP_NODISP;
	obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;
	obj_work->move_flag	|= (OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE);
	obj_work->move_flag	&= ~(OBD_MOVE_FALL);
	
	// パーツ生成
	for (Sint32 i = 0; i < place_info.part_num; ++i) {
		Uint32	ptn_idx	= i % GMD_BOSS5_LAND_PLACE_PTN_LEN;
		
		gmBoss5LandCreateLdPart(land_work,
								gm_boss5_land_place_pattern_tbl[ptn_idx],
								i);
	}
	
	// オブジェクト地形矩形設定
	gmBoss5LandSetObjCollisionRect(land_work, place_info.part_num);
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5LandMain;
	
	// シーケンス初期化
	gmBoss5LandProcInit(land_work);
	
	return obj_work;
}

// =======================================================================
// GmBoss5LandCreate
/*!
  ボスFINAL足場生成
  
  @param mgr_work	[io]	管理ワーク
  
  @return 足場ワーク(GMS_BOSS5_LAND_WORK)
  
  @note
  足場を生成する際はこの関数を呼び出してください。
 */
// =======================================================================
GMS_BOSS5_LAND_WORK* GmBoss5LandCreate(GMS_BOSS5_MGR_WORK *mgr_work)
{
	OBS_OBJECT_WORK	*obj_mgr	= GMM_BS_OBJ(mgr_work);
	OBS_OBJECT_WORK	*obj_land;
	
	obj_land	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS5_LAND,
											obj_mgr->pos.x,	// 一応、管理オブジェクトの座標を指定
											obj_mgr->pos.y,	// 一応、管理オブジェクトの座標を指定
											0,//flag
											0, 0, 0, 0, //left top width height
											0);//type
	
	if (NULL == obj_land) {
		return NULL;
	}
	
	// 管理ワークへの参照設定
	GMS_BOSS5_LAND_WORK	*land_work	= (GMS_BOSS5_LAND_WORK*)obj_land;
	land_work->mgr_work	= mgr_work;
	
	return (GMS_BOSS5_LAND_WORK*)obj_land;
}

// =======================================================================
// GmBoss5LandSetLight
/*!
  ボスFINAL足場用ライト設定
 */
// =======================================================================
void GmBoss5LandSetLight(void)
{
	NNS_RGBA	light_color	= {
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	NNS_VECTOR	light_vec;
	
	light_vec.x = 0.f;
	light_vec.y = -0.2f;
	light_vec.z = -1.f;
	
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_1, &light_color, 1.0f, &light_vec);
}


/*------ Static Functions ----------------------------------------------*/

// ############################################################################
// 共通
// ############################################################################

// =======================================================================
// gmBoss5LandGetPlacementInfo
/*!
  足場配置情報取得
  
  @param place_info	[out]	情報格納先
  
  @retval TRUE	情報取得成功
  @retval FALSE	情報取得失敗（アサートに失敗します）
  
  @note
  足場配置情報ギミックをサーチして、格納されている情報を参照します。
 */
// =======================================================================
BOOL gmBoss5LandGetPlacementInfo(GMS_BOSS5_LAND_PLACEMENT_INFO *place_info)
{
	OBS_OBJECT_WORK	*obj_work;
	GMS_EVE_RECORD_EVENT	*eve_rec;
	MTM_ASSERT(place_info);
	
	// 足場配置情報ギミックを参照して配置場所やパーツの数を取得
	
	obj_work	= ObjObjectSearchRegistObject(NULL, GMD_OBJTYPE_GIMMICK);
	while (obj_work) {
		
		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
		if (ene_com->eve_rec) {
			if (ene_com->eve_rec->id == GMD_EVENT_ID_GMK_BOSS5_LAND_PLACE) {
				eve_rec	= ene_com->eve_rec;
				break;
			}
		}
		
		obj_work	= ObjObjectSearchRegistObject(obj_work, GMD_OBJTYPE_GIMMICK);
	}
	
	// 必ず配置されていることが前提
	if (obj_work == NULL) {
		MTM_ASSERT(!"gmBoss5Land.cpp::gmBoss5LandGetPlacementInfo() Error! no land placement gmk found.\n");
		return FALSE;
	}
	
	// 配置座標設定
	place_info->pos_x	= obj_work->pos.x;
	place_info->pos_y	= obj_work->pos.y;
	
	{
		Sint32	range_width;	// 配置情報ギミック中心から矩形右端までの幅
		range_width	= (Sint32)(eve_rec->left + eve_rec->width);
		
		// 値は8ドット単位で収められているので1ドット単位に変換
		range_width	<<= 3;
		
		// 指定範囲に収まるパーツ数を算出
		place_info->part_num	= range_width / GMD_BOSS5_LAND_LDPART_WIDTH_INT;
	}
	
	return TRUE;
}


// ############################################################################
// 足場
// ############################################################################

// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss5LandSetObjCollisionRect
/*!
  足場 オブジェクト地形矩形設定
  
  @param land_work	[io]	足場ワーク
  @param part_num	[in]	足場構成パーツ数
 */
// =======================================================================
void gmBoss5LandSetObjCollisionRect(GMS_BOSS5_LAND_WORK *land_work, Sint32 part_num)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(land_work);
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)land_work;
	
	MTM_ASSERT(ene_com);
	
	ene_com->col_work.obj_col.obj	= GMM_BS_OBJ(land_work);
	
	ene_com->col_work.obj_col.width		= (Uint16)(part_num * GMD_BOSS5_LAND_LDPART_WIDTH_INT);
	ene_com->col_work.obj_col.height	= GMD_BOSS5_LAND_LDPART_HEIGHT_INT;
	ene_com->col_work.obj_col.ofst_x	= 0;
	ene_com->col_work.obj_col.ofst_y	= 0;
	
	// 画面外クリッピングチェックで矩形判定がオフにならないように画面外オフセットを設定
	// （※OBD_OBJECT_NOCLIPがオンになっていても必要）
	obj_work->view_out_ofst_plus[OBD_LEFT]	= (Sint16)(ene_com->col_work.obj_col.ofst_x - (Sint16)ene_com->col_work.obj_col.width);
	obj_work->view_out_ofst_plus[OBD_RIGHT]	= (Sint16)(ene_com->col_work.obj_col.ofst_x + (Sint16)ene_com->col_work.obj_col.width);
}

// =======================================================================
// gmBoss5LandDisableObjCollision
/*!
  足場 オブジェクト地形無効化
  
  @param land_work	[io]	足場ワーク
 */
// =======================================================================
void gmBoss5LandDisableObjCollision(GMS_BOSS5_LAND_WORK *land_work)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)land_work;
	
	MTM_ASSERT(ene_com);
	
	ene_com->col_work.obj_col.obj	= NULL;
}

// ============================================================================
// 処理関数
// ============================================================================

// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss5LandMain
/*!
  足場 メイン処理
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5LandMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_LAND_WORK	*land_work	= (GMS_BOSS5_LAND_WORK*)obj_work;
	
	// 更新処理
	if (land_work->proc_update) {
		land_work->proc_update(land_work);
	}
}

// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss5LandProc****
/*!
  足場シーケンス
  
  @param land_work	[io]	足場ワーク
 */
// =======================================================================
// 足場シーケンス初期化
void gmBoss5LandProcInit(GMS_BOSS5_LAND_WORK *land_work)
{
	land_work->proc_update	= gmBoss5LandProcUpdateIdle;
}

// 足場シーケンス更新 停滞処理
void gmBoss5LandProcUpdateIdle(GMS_BOSS5_LAND_WORK *land_work)
{
	// 足場振動要求待ち
	if (land_work->mgr_work->flag & GMD_BOSS5_MGR_FLAG_LAND_SHAKE_NEEDED) {
		// 振動シーケンス有効化
		land_work->flag	|= GMD_BOSS5_LAND_FLAG_SHAKE_ACTIVE;
		
		land_work->proc_update	= gmBoss5LandProcUpdateShake;
	}
}

// 足場シーケンス更新 振動処理
void gmBoss5LandProcUpdateShake(GMS_BOSS5_LAND_WORK *land_work)
{
	// 足場崩壊要求待ち
	if (land_work->mgr_work->flag & GMD_BOSS5_MGR_FLAG_LAND_BREAK_NEEDED) {
		
		// 崩壊シーケンス有効化
		land_work->flag	|= GMD_BOSS5_LAND_FLAG_BREAK_ACTIVE;
		
		// オブジェクト地形を無効化
		gmBoss5LandDisableObjCollision(land_work);
		
		land_work->proc_update	= NULL;
	}
}


// ############################################################################
// 足場構成パーツ
// ############################################################################

// =======================================================================
// gmBoss5LandCreateLdPart
/*!
  足場構成パーツを生成
  
  @param land_work	[io]	足場ワーク
  @param land_type	[in]	足場タイプ（デザイン形状）
  @parma part_index	[in]	構成パーツ番号
  
  @return 足場パーツワーク
 
  @note
  地形やヒットの当たり判定が無いエフェクトとして作成します。
  足場を構成するパーツの一部として動作しますが、  親によって外部から操作されるのでは無く、
  親に格納されている情報を参照して自分自身を操作します。
 */
// =======================================================================
GMS_BOSS5_LDPART_WORK* gmBoss5LandCreateLdPart(GMS_BOSS5_LAND_WORK *land_work,
											   GME_BOSS5_LAND_TYPE land_type,
											   Sint32 part_index)
{
	GMS_EFFECT_3DNN_WORK	*efct_3d;
	GMS_BOSS5_LDPART_WORK	*ldpart_work;
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(land_work);
	OBS_OBJECT_WORK	*obj_work;
	
	MTM_ASSERT(land_type < GME_BOSS5_LAND_TYPE_MAX);
	
	// オブジェクト生成
	obj_work	= (OBS_OBJECT_WORK*)GMM_EFFECT_CREATE_WORK(sizeof(GMS_BOSS5_LDPART_WORK),
														   parent_obj,
														   0,
														   "BOSS5_LAND_PART");
	
	efct_3d	= (GMS_EFFECT_3DNN_WORK*)obj_work;
	ldpart_work	= (GMS_BOSS5_LDPART_WORK*)obj_work;
	
	// パーツ番号保存
	ldpart_work->part_index	= part_index;
	
	// 足場構成パーツモデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 &GmBoss5GetObject3dList()[gm_boss5_land_mdl_amb_idx_tbl[land_type]],
								 &efct_3d->obj_3d);
	// 足場はトゥーン描画しない
	obj_work->obj_3d->drawflag	&= ~NND_DRAWOBJ_SHADER_USER_PROFILE_TOON;
	
	// 足場マテリアルモーションロード
	ObjObjectAction3dNNMaterialMotionLoad(obj_work,
										  0,
										  ObjDataGet(gm_boss5_land_mat_mtn_dwork_no_tbl[land_type]),
										  NULL,
										  gm_boss5_land_mat_mtn_data_tbl[land_type],
										  NULL);
	
	// Wii向けトゥーン設定不要
	//ObjDrawObjectSetToon
	
	// 足場用ライトを使用
	// MEMO :
	//   データ側でライトを無視する設定に変更となったが、
	//   ライトの影響を受ける部分もまだあり、この状態でデータも調整されているので一応残しておく。
	//   問題があったら修正。
	obj_work->obj_3d->use_light_flag	&= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag	|= OBD_LIGHT_USE_FLAG_1;
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_PARENT_FIX;	// 最初は親追随
	obj_work->flag	|= (OBD_OBJECT_NOHIT | OBD_OBJECT_NOCLIP);
	obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	obj_work->move_flag	&= ~OBD_MOVE_FALL;
	
	// 座標設定
	obj_work->parent_ofst.x	= (GMD_BOSS5_LAND_LDPART_WIDTH_FX / 2) + part_index * GMD_BOSS5_LAND_LDPART_WIDTH_FX + GMD_BOSS5_LAND_LDPART_CENTER_OFST_X_FX;
	obj_work->parent_ofst.y	= (GMD_BOSS5_LAND_LDPART_HEIGHT_FX / 2) + GMD_BOSS5_LAND_LDPART_CENTER_OFST_Y_FX;
	obj_work->parent_ofst.z	= 0;
	
	// 基準親オフセット設定
	ldpart_work->pivot_parent_ofst[MTD_X]	= obj_work->parent_ofst.x;
	ldpart_work->pivot_parent_ofst[MTD_Y]	= obj_work->parent_ofst.y;
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5LdPartMain;
	
	// シーケンス初期化
	gmBoss5LdPartProcInit(ldpart_work);
	
	return ldpart_work;
}

// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss5LdPartInitSpin
/*!
  落下中回転初期化
  
  @param ldpart_work	[io]	足場構成パーツワーク
  
  @note
  既定範囲内のランダムな軸・角速度で回転する処理を行います。
 */
// =======================================================================
void gmBoss5LdPartInitSpin(GMS_BOSS5_LDPART_WORK *ldpart_work)
{
	nnMakeUnitQuaternion(&ldpart_work->cur_rot_quat);
	nnMakeUnitQuaternion(&ldpart_work->rot_diff_quat);
	
	for (Sint32 i = 0; i < GMD_BOSS5_LAND_LDPART_SPIN_ROT_AXIS_NUM; ++i) {
		NNS_VECTOR	spin_axis;
		Float	rand_z;
		Angle16	rand_angle;
		NNS_QUATERNION	diff_rot;
		
		// ランダムな回転軸を取得
		rand_z	= FX_FX32_TO_F32(AkMathRandFx()) * 2.f - 1.f;	// -1.f ～ 1.f
		rand_z	= MTM_MATH_CLIP(rand_z, -1.f, 1.f);
		rand_angle	= AKM_DEGtoA16(360.f * FX_FX32_TO_F32(AkMathRandFx()));	// 0～360degのランダム角度
		AkMathGetRandomUnitVector(&spin_axis, rand_z, rand_angle);
		
		// 差分回転を乗算
		nnMakeRotateAxisQuaternion(&diff_rot, spin_axis.x, spin_axis.y, spin_axis.z,
								   GMD_BOSS5_LAND_LDPART_SPIN_ROT_SPD_DEG);
		nnMultiplyQuaternion(&ldpart_work->rot_diff_quat, &diff_rot, &ldpart_work->rot_diff_quat);
	}
}

// =======================================================================
// gmBoss5LdPartUpdateSpin
/*!
  落下中回転更新
  
  @param ldpart_work	[io]	足場構成パーツワーク
 */
// =======================================================================
void gmBoss5LdPartUpdateSpin(GMS_BOSS5_LDPART_WORK *ldpart_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(ldpart_work);
	
	// 差分回転を適用
	nnMultiplyQuaternion(&ldpart_work->cur_rot_quat,
						 &ldpart_work->rot_diff_quat,
						 &ldpart_work->cur_rot_quat);
	
	// 現在の回転姿勢をユーザマトリクスにセット
	nnMakeQuaternionMatrix(&obj_work->obj_3d->user_obj_mtx_r,
						   &ldpart_work->cur_rot_quat);
	obj_work->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
}

// =======================================================================
// gmBoss5LdPartInitFall
/*!
  落下処理初期化
  
  @param ldpart_work	[io]	足場構成パーツワーク
 */
// =======================================================================
void gmBoss5LdPartInitFall(GMS_BOSS5_LDPART_WORK *ldpart_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(ldpart_work);
	
	Sint32	rand_deg	= ((Sint32)mtMathRand() % GMD_BOSS5_LAND_LDPART_FALL_XY_DIR_RANGE_DEG);
	Angle32	rand_angle	= AKM_DEGtoA32(rand_deg + (270 - (GMD_BOSS5_LAND_LDPART_FALL_XY_DIR_RANGE_DEG / 2)));
	fx32	rand_z		= FX_Mul(AkMathRandFx(), GMD_BOSS5_LAND_LDPART_FALL_Z_SPD_MAX * 2) - GMD_BOSS5_LAND_LDPART_FALL_Z_SPD_MAX;
	
	obj_work->spd.y	= (fx32)(FX32_ONE * GMD_BOSS5_LAND_LDPART_FALL_XY_SPD_FL * nnSin(rand_angle));
	obj_work->spd.x	= (fx32)(FX32_ONE * GMD_BOSS5_LAND_LDPART_FALL_XY_SPD_FL * nnCos(rand_angle));
	obj_work->spd.z	= rand_z;
	
	// 落下有効
	obj_work->flag	&= ~OBD_OBJECT_PARENT_FIX;
	obj_work->move_flag	|= OBD_MOVE_FALL;
}

// =======================================================================
// gmBoss5LdPartInitVib
/*!
  振動処理初期化
  
  @param ldpart_work	[io]	構成パーツワーク
 */
// =======================================================================
void gmBoss5LdPartInitVib(GMS_BOSS5_LDPART_WORK *ldpart_work)
{
	ldpart_work->vib_cnt	= mtMathRand() % GMD_BOSS5_LAND_LDPART_VIB_PHASE_NUM;
	
	ldpart_work->vib_ofst[MTD_X]	=
		ldpart_work->vib_ofst[MTD_Y]	= 0;
}

// =======================================================================
// gmBoss5LdPartUpdateVib
/*!
  振動処理更新
  
  @param ldpart_work	[io]	構成パーツワーク
 */
// =======================================================================
void gmBoss5LdPartUpdateVib(GMS_BOSS5_LDPART_WORK *ldpart_work)
{
	MTM_ASSERT(ldpart_work->vib_cnt < GMD_BOSS5_LAND_LDPART_VIB_PHASE_NUM);
	
	// オフセットに反映
	ldpart_work->vib_ofst[MTD_X]	= FX_Mul(gm_boss5_land_vib_tbl[ldpart_work->vib_cnt][MTD_X], GMD_BOSS5_LAND_LDPART_VIB_AMPLITUDE);
	ldpart_work->vib_ofst[MTD_Y]	= FX_Mul(gm_boss5_land_vib_tbl[ldpart_work->vib_cnt][MTD_Y], GMD_BOSS5_LAND_LDPART_VIB_AMPLITUDE);
	
	// フェーズ更新
	ldpart_work->vib_cnt++;
	if (ldpart_work->vib_cnt >= GMD_BOSS5_LAND_LDPART_VIB_PHASE_NUM) {
		ldpart_work->vib_cnt	= 0;
	}
}

// =======================================================================
// gmBoss5LdPartClearVib
/*!
  振動処理クリア
  
  @param ldpart_work	[io]	構成パーツワーク
  
  @note
  振動が不要になった時点で呼び出してください。
 */
// =======================================================================
void gmBoss5LdPartClearVib(GMS_BOSS5_LDPART_WORK *ldpart_work)
{
	ldpart_work->vib_cnt	= 0;
	
	ldpart_work->vib_ofst[MTD_X]	=
		ldpart_work->vib_ofst[MTD_Y]	= 0;
}

// ============================================================================
// 処理関数
// ============================================================================


// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss5LdPartMain
/*!
  足場 メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5LdPartMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_LDPART_WORK	*ldpart_work	= (GMS_BOSS5_LDPART_WORK*)obj_work;
	
	// 座標更新
	obj_work->parent_ofst.x	= ldpart_work->pivot_parent_ofst[MTD_X] + ldpart_work->vib_ofst[MTD_X];
	obj_work->parent_ofst.y	= ldpart_work->pivot_parent_ofst[MTD_Y] + ldpart_work->vib_ofst[MTD_Y];
	
	if (ldpart_work->proc_update) {
		ldpart_work->proc_update(ldpart_work);
	}
}


// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss5LdPartProc****
/*!
  足場構成パーツシーケンス
  
  @param ldpart_work	[io]	足場構成パーツワーク
 */
// =======================================================================
// 足場構成パーツシーケンス初期化
void gmBoss5LdPartProcInit(GMS_BOSS5_LDPART_WORK *ldpart_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(ldpart_work);
	
	// マテリアルモーション開始
	ObjDrawObjectActionSet3DNNMaterial(obj_work, 0);
	obj_work->disp_flag	|= OBD_DISP_REPEAT;
	
	ldpart_work->proc_update	= gmBoss5LdPartProcUpdateIdle;
}

// 足場構成パーツシーケンス更新 停滞処理
void gmBoss5LdPartProcUpdateIdle(GMS_BOSS5_LDPART_WORK *ldpart_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(ldpart_work);
	GMS_BOSS5_LAND_WORK	*parent_land	= (GMS_BOSS5_LAND_WORK*)obj_work->parent_obj;
	
	if (parent_land->flag & GMD_BOSS5_LAND_FLAG_SHAKE_ACTIVE) {
		
		// 振動初期化
		gmBoss5LdPartInitVib(ldpart_work);
		
		ldpart_work->proc_update	= gmBoss5LdPartProcUpdateShake;
	}
}

// 足場構成パーツシーケンス更新 振動処理
void gmBoss5LdPartProcUpdateShake(GMS_BOSS5_LDPART_WORK *ldpart_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(ldpart_work);
	GMS_BOSS5_LAND_WORK	*parent_land	= (GMS_BOSS5_LAND_WORK*)obj_work->parent_obj;
	
	// 振動更新
	gmBoss5LdPartUpdateVib(ldpart_work);
	
	// 崩壊処理有効化待ち
	if (parent_land->flag & GMD_BOSS5_LAND_FLAG_BREAK_ACTIVE) {
		
		// 振動クリア
		gmBoss5LdPartClearVib(ldpart_work);
		
		// 回転処理初期化
		gmBoss5LdPartInitSpin(ldpart_work);
		
		// 落下処理初期化（更新処理無し）
		gmBoss5LdPartInitFall(ldpart_work);
		
		// エフェクト生成待ちタイマ設定
		// 瞬間不可軽減のため、パーツ番号奇数と偶数で1フレームずらして生成
		ldpart_work->wait_timer	= (Uint32)(ldpart_work->part_index & 0x01);
		
		ldpart_work->proc_update	= gmBoss5LdPartProcUpdateFall;
	}
}

// 足場構成パーツシーケンス更新 落下処理
void gmBoss5LdPartProcUpdateFall(GMS_BOSS5_LDPART_WORK *ldpart_work)
{
	if (ldpart_work->wait_timer) {
		ldpart_work->wait_timer--;
	}
	else {
		// 1つだけ生成
		if (ldpart_work->brk_glass_cnt == 0) {
			GmBoss5EfctCreateBreakingGlass(GMM_BS_OBJ(ldpart_work));
			ldpart_work->brk_glass_cnt++;
		}
	}
	// 回転処理更新
	gmBoss5LdPartUpdateSpin(ldpart_work);
}


// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================
