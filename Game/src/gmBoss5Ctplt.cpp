// =======================================================================
/*!
  @file	gmBoss5Ctplt.cpp
  @brief ボスファイナル カタパルト
  
  @author Keisuke Tanaka
 				Copyright(c) 2008 Dimps
  $Id: gmBoss5Ctplt.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "gmMain.h"
#include "gmEventTbl.h"
#include "gmEnemy.h"
#include "gmBoss5.h"

#include "gmBoss5Ctplt.h"

// データヘッダ
#include "../file/common/arc/BOSS05.hmb"
#include "../file/common/model/BOSS05_MDL.hmb"

/*------ Macros --------------------------------------------------------*/
//############ カタパルト #####################################################
/* 定義値 */
#define GMD_BOSS5_CTPLT_BG_FARSIDE_POS_Z		(GMD_OBJ_DEFAULT_POS_Z_B - (fx32)(FX32_ONE * 1))	//!< 近景の向こう側にかろうじて隠れる位置
#define GMD_BOSS5_CTPLT_MOVE_DOWN_ACC			((fx32)(FX32_ONE * 0.05f))		//!< 収納時下降加速度
#define GMD_BOSS5_CTPLT_MOVE_DOWN_HIDE_HEIGHT	((fx32)(FX32_ONE * 32))			//!< 地面の位置からこの高さだけ下の座標を下回ったら隠れたとみなす

// オブジェクト地形矩形サイズ
#define GMD_BOSS5_CTPLT_OBJ_COL_RECT_WIDTH_INT	(40)
#define GMD_BOSS5_CTPLT_OBJ_COL_RECT_HEIGHT_INT	(256)
#define GMD_BOSS5_CTPLT_OBJ_COL_RECT_OFST_X_INT	(-86)
#define GMD_BOSS5_CTPLT_OBJ_COL_RECT_OFST_Y_INT	(-256)

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ボス５カタパルト（ギミック扱い）
typedef struct tag_GMS_BOSS5_CTPLT_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	void	(*proc_update)(struct tag_GMS_BOSS5_CTPLT_WORK*);
} GMS_BOSS5_CTPLT_WORK;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
//############ カタパルト #####################################################
/* 補助関数 */
static void gmBoss5CtpltSetObjCollisionRect(GMS_BOSS5_CTPLT_WORK *ctplt_work);
/* 処理関数 */
/* 制御関数 */
static void gmBoss5CtpltMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
static void gmBoss5CtpltProcInit(GMS_BOSS5_CTPLT_WORK *ctplt_work);
static void gmBoss5CtpltProcIdle(GMS_BOSS5_CTPLT_WORK *ctplt_work);
static void gmBoss5CtpltProcMoveDown(GMS_BOSS5_CTPLT_WORK *ctplt_work);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmBoss5CtpltInit
/*!
  ボスFINAL カタパルト初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss5CtpltInit(GMS_EVE_RECORD_EVENT *eve_rec,
								  fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	GMS_ENEMY_3D_WORK	*gmk_3d;
	GMS_BOSS5_CTPLT_WORK	*ctplt_work;
	OBS_OBJECT_WORK	*obj_work;
	
	// オブジェクト生成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,	// 座標は後で改めて設定
										sizeof(GMS_BOSS5_CTPLT_WORK),
										"BOSS5_CTPLT");
	
	gmk_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	ctplt_work	= (GMS_BOSS5_CTPLT_WORK*)obj_work;
	
	// カタパルトモデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 &GmBoss5GetObject3dList()[IDB_BOSS05_MDL_GMK_DEGG_CTPLT_ZNO],
								 &gmk_3d->obj_3d);
	
	// カタパルトマテリアルモーションロード
	ObjObjectAction3dNNMaterialMotionLoad(obj_work,
										  0,
										  ObjDataGet(GMD_DWORK_NO_BOSS_05_CTPLT_MAT),
										  NULL,
										  IDB_BOSS05_BOSS05_CTPLT_MTN_AMB,
										  NULL);
	
	// ワーク設定
	obj_work->flag	&= ~OBD_OBJECT_PARENT_FIX;
	obj_work->flag	|= (OBD_OBJECT_NOHIT | OBD_OBJECT_NOCLIP);
	obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	obj_work->move_flag	&= ~OBD_MOVE_FALL;
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5CtpltMain;
	
	// シーケンス初期化
	gmBoss5CtpltProcInit(ctplt_work);
	
	return obj_work;
}

// =======================================================================
// GmBoss5CtpltCreate
/*!
  ボスFINALカタパルト生成
  
  @param body_work	[io]	本体ワーク
  
  @note
  カタパルトを生成する際はこの関数を呼び出してください。
 */
// =======================================================================
void GmBoss5CtpltCreate(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_body	= GMM_BS_OBJ(body_work);
	OBS_OBJECT_WORK	*obj_ctplt;
	
	obj_ctplt	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS5_CTPLT,
											obj_body->pos.x,	// 一応、本体の座標を指定
											obj_body->pos.y,	// 一応、本体の座標を指定
											0,//flag
											0, 0, 0, 0,	// left top width height
											0);//type
	
	// 親設定
	obj_ctplt->parent_obj	= obj_body;
	
	// 座標設定
	obj_ctplt->pos.x	= obj_body->pos.x;
	obj_ctplt->pos.y	= body_work->ground_v_pos;
	obj_ctplt->pos.z	= GMD_BOSS5_CTPLT_BG_FARSIDE_POS_Z;
}


/*------ Static Functions ----------------------------------------------*/
// ############################################################################
// カタパルト
// ############################################################################
// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss5CtpltSetObjCollisionRect
/*!
  カタパルト オブジェクト地形矩形設定
  
  @param ctplt_work	[io]	カタパルトワーク
 */
// =======================================================================
void gmBoss5CtpltSetObjCollisionRect(GMS_BOSS5_CTPLT_WORK *ctplt_work)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)ctplt_work;
	
	MTM_ASSERT(ene_com);
	
	ene_com->col_work.obj_col.obj	= GMM_BS_OBJ(ctplt_work);
	
	ene_com->col_work.obj_col.width		= GMD_BOSS5_CTPLT_OBJ_COL_RECT_WIDTH_INT;
	ene_com->col_work.obj_col.height	= GMD_BOSS5_CTPLT_OBJ_COL_RECT_HEIGHT_INT;
	ene_com->col_work.obj_col.ofst_x	= GMD_BOSS5_CTPLT_OBJ_COL_RECT_OFST_X_INT;
	ene_com->col_work.obj_col.ofst_y	= GMD_BOSS5_CTPLT_OBJ_COL_RECT_OFST_Y_INT;
}

// ============================================================================
// 処理関数
// ============================================================================

// ============================================================================
// 制御関数
// ============================================================================
// =======================================================================
// gmBoss5CtpltMain
/*!
  カタパルト メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5CtpltMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_CTPLT_WORK	*ctplt_work	= (GMS_BOSS5_CTPLT_WORK*)obj_work;
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_MGR_WORK	*mgr_work	= parent_body->mgr_work;
	
	// 本体側のデモ処理完了したら、
	// カタパルトがなくなるまで（＝本体が本来の座標に表示されるようになるまで）
	// 見えない壁を設定しておく
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_BODY_DEMO_IS_FINISHED) {
		gmBoss5CtpltSetObjCollisionRect(ctplt_work);
	}
	
	// 更新処理
	if (ctplt_work->proc_update) {
		ctplt_work->proc_update(ctplt_work);
	}
}

// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss5CtpltProc****
/*!
  カタパルト シーケンス
  
  @param ctplt_work	[io]	カタパルトワーク
 */
// =======================================================================
// カタパルトシーケンス初期化
void gmBoss5CtpltProcInit(GMS_BOSS5_CTPLT_WORK *ctplt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(ctplt_work);
	
	// マテリアルモーション開始
	ObjDrawObjectActionSet3DNNMaterial(obj_work, 0);
	obj_work->disp_flag	|= OBD_DISP_REPEAT;
	
	ctplt_work->proc_update	= gmBoss5CtpltProcIdle;
}

// カタパルトシーケンス更新 停滞処理
void gmBoss5CtpltProcIdle(GMS_BOSS5_CTPLT_WORK *ctplt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(ctplt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_MGR_WORK	*mgr_work	= parent_body->mgr_work;
	
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_CTPLT_STORE_NEEDED) {
		
		// 下降加速度設定
		obj_work->spd_add.y	= GMD_BOSS5_CTPLT_MOVE_DOWN_ACC;
		
		ctplt_work->proc_update	= gmBoss5CtpltProcMoveDown;
	}
}

// カタパルトシーケンス更新 下降処理
void gmBoss5CtpltProcMoveDown(GMS_BOSS5_CTPLT_WORK *ctplt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(ctplt_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS5_MGR_WORK	*mgr_work	= parent_body->mgr_work;
	
	// 収納待ち
	if (obj_work->pos.y > parent_body->ground_v_pos + GMD_BOSS5_CTPLT_MOVE_DOWN_HIDE_HEIGHT) {
		
		// カタパルト収納完了通知
		mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_CTPLT_IS_STORED;
		
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
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
