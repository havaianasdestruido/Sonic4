// =======================================================================
/*!
  @file	gmBoss5Egg.cpp
  @brief ボスファイナル エッグマン

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5Egg.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmBossCommon.h"
#include "gmBoss5.h"
#include "gmBoss5Rocket.h"
#include "gmBoss5Turret.h"
#include "gmBoss5Egg.h"
#include "gmBoss5Efct.h"

#include "gmPlySeq.h"

// データヘッダ
#include "../file/common/arc/BOSS05.hmb"
#include "../file/common/model/BOSS05_MDL.hmb"
#include "../file/common/model/BOSS05_EGG_MTN.hmb"

/*------ Macros --------------------------------------------------------*/
//############ エッグマン #####################################################
/* 定義値 */
#define GMD_BOSS5_EGG_HIDE_HIGHT			((fx32)(FX32_ONE * 64))		//!< 地面に対してこの高さ分ずらすと地中に隠れる

// 走り処理関連
#define GMD_BOSS5_EGG_ESCAPE_RUN_DISTANCE_FASTEST	((fx32)(FX32_ONE * 64))	//!< この値までプレイヤーとの距離が縮まったときプレイヤーと同じ速度になる
#define GMD_BOSS5_EGG_ESCAPE_RUN_DISTANCE_SLOWEST	((fx32)(FX32_ONE * 96))	//!< この値以上プレイヤーと離れていたとき最も遅い速度が適用される
#define GMD_BOSS5_EGG_ESCAPE_RUN_SLOWEST_SPD		((fx32)(FX32_ONE * 4))	//!< 既定距離以上プレイヤーと離れているとこの速度が適用される
#define GMD_BOSS5_EGG_ESCAPE_RUN_VIEWOUT_OFST_LEFT	(-16)
#define GMD_BOSS5_EGG_ESCAPE_RUN_VIEWOUT_OFST_RIGHT	(16)

// ジャンプ関連
#define GMD_BOSS5_EGG_JUMP_START_OFST_POS_X	((fx32)(FX32_ONE * -224))	//!< 本体からこのオフセットずれた位置に到達したらジャンプ移動を開始する
#define GMD_BOSS5_EGG_JUMP_RUNUP_SPD_X		((fx32)(FX32_ONE * 8))		//!< 
#define GMD_BOSS5_EGG_JUMP_INIT_SPD_X		((fx32)(FX32_ONE * 1))		//!< ジャンプ時水平方向初速度
#define GMD_BOSS5_EGG_JUMP_INIT_SPD_Y		((fx32)(FX32_ONE * -6))		//!< ジャンプ時垂直方向初速度

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
//############ ボスFINALエッグマン ############################################
/* インターフェース関数 */
/* 補助関数 */
static void gmBoss5EggInitEscapeRun(GMS_BOSS5_EGG_WORK *egg_work);
static void gmBoss5EggUpdateEscapeRun(GMS_BOSS5_EGG_WORK *egg_work);
static void gmBoss5EggInitJump(GMS_BOSS5_EGG_WORK *egg_work, fx32 dest_pos_x);
static BOOL gmBoss5EggUpdateJump(GMS_BOSS5_EGG_WORK *egg_work);
static void gmBoss5EggGetBodyNodePos(const GMS_BOSS5_EGG_WORK *egg_work, VecFx32 *pos_out);

/* 処理関数 */
/* 制御処理 */
static void gmBoss5EggMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
// エッグマンシーケンス
static void gmBoss5EggProcInit(GMS_BOSS5_EGG_WORK *egg_work);
static void gmBoss5EggProcUpdateStandby(GMS_BOSS5_EGG_WORK *egg_work);
static void gmBoss5EggProcUpdateRun(GMS_BOSS5_EGG_WORK *egg_work);
static void gmBoss5EggProcUpdateStartJump(GMS_BOSS5_EGG_WORK *egg_work);
static void gmBoss5EggProcUpdateJump(GMS_BOSS5_EGG_WORK *egg_work);
static void gmBoss5EggProcUpdateFall(GMS_BOSS5_EGG_WORK *egg_work);
static void gmBoss5EggProcUpdateAnger(GMS_BOSS5_EGG_WORK *egg_work);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmBoss5EggInit
/*!
  ボスFINAL エッグマン初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss5EggInit(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_3D_WORK	*ene_3d;
	GMS_BOSS5_EGG_WORK	*egg_work;
	
	// オブジェクト生成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS5_EGG_WORK),
										"BOSS5_EGG");
	
	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	egg_work	= (GMS_BOSS5_EGG_WORK*)obj_work;
	
	// Z位置設定
	obj_work->pos.z	= GMD_BOSS5_BG_FARSIDE_POS_Z;
	
	// 地形当たり設定
	ObjObjectFieldRectSet(obj_work,
						  -16, -16, 16, 0);
	
	// エッグマンモデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 &GmBoss5GetObject3dList()[IDB_BOSS05_MDL_EGGMAN_ZNO],
								 &ene_3d->obj_3d);
	
	// エッグマンモーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,
								  TRUE,
								  ObjDataGet(GMD_DWORK_NO_BOSS_05_EGG_MTN),
								  NULL,
								  0,
								  NULL);
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= GMD_BOSS5_DEFAULT_BLEND_SPD;
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	&= ~OBD_DISP_NODIRFLIP;	// HFLIPを反映させる
	obj_work->disp_flag	&= ~OBD_DISP_HFLIP;	// 右向き
	obj_work->move_flag	|= OBD_MOVE_FALL | OBD_MOVE_NOCOL_W;
	obj_work->move_flag	&= ~OBD_MOVE_NOCOL;
	
	// ホーミングアタックの対象からはずす
	egg_work->ene_3d.ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5EggMain;
	
	// シーケンス初期化
	gmBoss5EggProcInit(egg_work);
	
#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}


// =======================================================================
// GmBoss5EggCreate
/*!
  エッグマン生成
  
  @param body_work	[io]	本体ワーク
  @param pos_x		[in]	生成座標X
  @param pos_y		[in]	生成座標Y
  
  @return エッグマンワーク
  
  @note
  エッグマンを生成する際はこの関数を呼び出してください。
 */
// =======================================================================
GMS_BOSS5_EGG_WORK* GmBoss5EggCreate(GMS_BOSS5_BODY_WORK *body_work, fx32 pos_x, fx32 pos_y)
{
	OBS_OBJECT_WORK *obj_egg = GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS5_EGG,
														 pos_x, pos_y,
														 0,//flag
														 0,0,0,0,
														 0);
	obj_egg->parent_obj	= GMM_BS_OBJ(body_work);
	
	return (GMS_BOSS5_EGG_WORK*)obj_egg;
}

/*------ Static Functions ----------------------------------------------*/
// ============================================================================
// インターフェース関数
// ============================================================================

// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss5EggInitEscapeRun
/*!
  逃亡走り処理 初期化
  
  @param egg_work	[io]	エッグマンワーク
 */
// =======================================================================
void gmBoss5EggInitEscapeRun(GMS_BOSS5_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	// 速度クリア
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 初期位置設定（プレイヤーより1画面分右に配置）
	obj_work->pos.x	= GmBsCmnGetPlayerObj()->pos.x + (fx32)((Sint32)OBD_OBJ_CLIP_LCD_X << FX32_SHIFT);
}


// =======================================================================
// gmBoss5EggUpdateEscapeRun
/*!
  逃亡走り処理 更新
  
  @param egg_work	[io]	エッグマンワーク
 */
// =======================================================================
void gmBoss5EggUpdateEscapeRun(GMS_BOSS5_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	BOOL	is_viewout;
	
	// 画面外チェック
	is_viewout	= ObjViewOutCheck(obj_work->pos.x, obj_work->pos.y,
								  0,
								  GMD_BOSS5_EGG_ESCAPE_RUN_VIEWOUT_OFST_LEFT,
								  0,
								  GMD_BOSS5_EGG_ESCAPE_RUN_VIEWOUT_OFST_RIGHT,
								  0);
	
	// 画面内ならエッグマンの速度を調整
	if (!is_viewout) {
		fx32	distance	= obj_work->pos.x - GmBsCmnGetPlayerObj()->pos.x;
		fx32	ratio	= FX_Div(GMD_BOSS5_EGG_ESCAPE_RUN_DISTANCE_SLOWEST - distance,
								 GMD_BOSS5_EGG_ESCAPE_RUN_DISTANCE_SLOWEST - GMD_BOSS5_EGG_ESCAPE_RUN_DISTANCE_FASTEST);
		
		ratio	= MTM_MATH_CLIP(ratio, 0, FX32_ONE);
		
		// DISTANCE_SLOWEST からどれだけ DISTANCE_FASTEST に近いかに応じて
		// 既定速度～プレイヤー速度の間で線形補間
		// （i.e. DISTANCE_SLOWESTに近いほど既定速度に近くなり、
		//        DISTANCE_FASTESTに近いほどプレイヤー速度に近づく）
		obj_work->spd.x	=
			FX_Mul(ratio, GmBsCmnGetPlayerObj()->spd_m) +
				FX_Mul(FX32_ONE - ratio, GMD_BOSS5_EGG_ESCAPE_RUN_SLOWEST_SPD);
	}
}

// =======================================================================
// gmBoss5EggInitJump
/*!
  ジャンプ処理初期化
  
  @param egg_work	[io]	エッグマンワーク
  @param dest_pos_x	[in]	目標地点X座標
  
  @note
  座標を直接制御してジャンプ移動（放物線運動）を行います。
 */
// =======================================================================
void gmBoss5EggInitJump(GMS_BOSS5_EGG_WORK *egg_work, fx32 dest_pos_x)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	// 加速度クリア
	VEC_Set(&obj_work->spd_add, 0, 0, 0);
	
	// 初速度設定
	obj_work->spd.x	= GMD_BOSS5_EGG_JUMP_INIT_SPD_X;
	obj_work->spd.y	= GMD_BOSS5_EGG_JUMP_INIT_SPD_Y;
	
	// 垂直初速度・目標地点までの距離から、地面着地と同時に目標地点に到達するような重力加速度を求める
	obj_work->spd_add.y	= -2 * FX_Mul(FX_Div(obj_work->spd.x, dest_pos_x - obj_work->pos.x),
									  obj_work->spd.y);
	
	// ジャンプ目標水平位置
	egg_work->jump_dest_pos_x	= dest_pos_x;
	
	// フラグ設定
	obj_work->move_flag	|=  (OBD_MOVE_NOCOL | OBD_MOVE_JUMP);
	obj_work->move_flag	&= ~OBD_MOVE_FALL;
}

// =======================================================================
// gmBoss5EggUpdateJump
/*!
  ジャンプ処理更新
  
  @param egg_work	[io]	エッグマンワーク
  
  @retval TRUE	目標地点到達
  @retval FALSE	目標地点未到達
 */
// =======================================================================
BOOL gmBoss5EggUpdateJump(GMS_BOSS5_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	if (obj_work->pos.x >= egg_work->jump_dest_pos_x) {
		// 目標水平位置に到達したら水平移動停止
		obj_work->spd.x	= 0;
	}
	
	// 地中に隠れるのを待つ
	if (obj_work->pos.y > parent_body->ground_v_pos + GMD_BOSS5_EGG_HIDE_HIGHT) {
		// 移動停止
		GmBsCmnSetObjSpdZero(obj_work);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss5EggGetBodyNodePos
/*!
  本体の胴体ノード座標取得
  
  @param egg_work	[in]	エッグマンワーク
  @param pos_out	[out]	座標格納先
 */
// =======================================================================
void gmBoss5EggGetBodyNodePos(const GMS_BOSS5_EGG_WORK *egg_work, VecFx32 *pos_out)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	const NNS_MATRIX	*w_mtx;
	
	MTM_ASSERT(parent_body);
	MTM_ASSERT(pos_out);
	
	w_mtx	= GmBsCmnGetSNMMtx(&parent_body->snm_work,
							   parent_body->body_snm_reg_id);
	
	VEC_Set(pos_out,
			FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3)),
			-FX_F32_TO_FX32(NNM_MTX(*w_mtx, 1, 3)),
			FX_F32_TO_FX32(NNM_MTX(*w_mtx, 2, 3)));
}

// ============================================================================
// 処理関数
// ============================================================================

// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss5EggMain
/*!
  エッグマン メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5EggMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_EGG_WORK	*egg_work	= (GMS_BOSS5_EGG_WORK*)obj_work;
	
	// 更新処理
	if (egg_work->proc_update) {
		egg_work->proc_update(egg_work);
	}
}


// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss5EggProc****
/*!
  エッグマンシーケンス
  
  @param egg_work	[io]	エッグマンワーク
 */
// =======================================================================
// エッグマンシーケンス初期化
void gmBoss5EggProcInit(GMS_BOSS5_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	// 走りアクション設定
	GmBsCmnSetAction(obj_work, IDB_BOSS05_EGG_MTN_B05_0_ATT01_01E_ZNM,
					 TRUE, FALSE);
	
	// 汗エフェクト開始
	GmBoss5EfctStartEggSweat(egg_work);
	
	// 処理関数設定
	egg_work->proc_update	= gmBoss5EggProcUpdateStandby;
}

// エッグマンシーケンス更新 待機処理
void gmBoss5EggProcUpdateStandby(GMS_BOSS5_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	GMS_BOSS5_MGR_WORK	*mgr_work	= ((GMS_BOSS5_BODY_WORK*)obj_work->parent_obj)->mgr_work;
	
	if (mgr_work->flag & GMD_BOSS5_MGR_FLAG_PLY_PASSED_TRG) {
		// 逃亡走り処理初期化
		gmBoss5EggInitEscapeRun(egg_work);
		
		egg_work->proc_update	= gmBoss5EggProcUpdateRun;
	}
}

// エッグマンシーケンス更新 走り処理
void gmBoss5EggProcUpdateRun(GMS_BOSS5_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	VecFx32	body_pos;
	
	// 本体胴体座標取得
	gmBoss5EggGetBodyNodePos(egg_work, &body_pos);
	
	// 逃亡走り処理更新
	gmBoss5EggUpdateEscapeRun(egg_work);
	
	if (obj_work->pos.x >=  body_pos.x + GMD_BOSS5_EGG_JUMP_START_OFST_POS_X) {
		
		// ジャンプ助走モーション時の速度
		obj_work->spd.x	= GMD_BOSS5_EGG_JUMP_RUNUP_SPD_X;
		
		// ジャンプ開始アクション設定
		GmBsCmnSetAction(obj_work, IDB_BOSS05_EGG_MTN_B05_0_ATT02_01E_ZNM,
						 FALSE, TRUE);
		
		egg_work->proc_update	= gmBoss5EggProcUpdateStartJump;
	}
}

// エッグマンシーケンス更新 ジャンプ開始処理
void gmBoss5EggProcUpdateStartJump(GMS_BOSS5_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		GMS_BOSS5_MGR_WORK	*mgr_work	= ((GMS_BOSS5_BODY_WORK*)obj_work->parent_obj)->mgr_work;
		VecFx32	head_pos;
		
		// 4:3画面用カメラスライド要求
		mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_CAMERA_SLIDE_NEEDED;
		
		// 汗エフェクト終了
		GmBoss5EfctEndEggSweat(egg_work);
		
		// 本体胴体座標取得
		gmBoss5EggGetBodyNodePos(egg_work, &head_pos);
		
		// ジャンプ移動開始
		gmBoss5EggInitJump(egg_work, head_pos.x);
		
		// ジャンプ中アクション設定
		GmBsCmnSetAction(obj_work, IDB_BOSS05_EGG_MTN_B05_0_ATT02_02E_ZNM,
						 FALSE, FALSE);
		
		egg_work->proc_update	= gmBoss5EggProcUpdateJump;
	}
}

// エッグマンシーケンス更新 ジャンプ中処理
void gmBoss5EggProcUpdateJump(GMS_BOSS5_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	// ジャンプ更新
	gmBoss5EggUpdateJump(egg_work);
	
	// 下降に転じたら下降アクションに設定
	if (obj_work->spd.y > 0) {
		
		// 下降アクション設定
		GmBsCmnSetAction(obj_work, IDB_BOSS05_EGG_MTN_B05_0_ATT02_03E_ZNM,
						 FALSE, FALSE);
		
		egg_work->proc_update	= gmBoss5EggProcUpdateFall;
	
	}
}

// エッグマンシーケンス更新 下降処理
void gmBoss5EggProcUpdateFall(GMS_BOSS5_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	// 乗り込み完了待ち
	if (gmBoss5EggUpdateJump(egg_work)) {
		VecFx32	head_pos;
		
		// 本体胴体座標取得
		gmBoss5EggGetBodyNodePos(egg_work, &head_pos);
		
		// 位置設定
		obj_work->pos	= head_pos;
		
		// 怒りアクション設定
		GmBsCmnSetAction(obj_work, IDB_BOSS05_EGG_MTN_B05_0_ATT03_01E_ZNM,
						 FALSE, FALSE);
		
		// 左を向く
		obj_work->disp_flag	|= OBD_DISP_HFLIP;
		
		egg_work->proc_update	= gmBoss5EggProcUpdateAnger;
	}
}

// エッグマンシーケンス更新 怒り処理
void gmBoss5EggProcUpdateAnger(GMS_BOSS5_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		
		// 搭乗完了通知
		parent_body->flag	|= GMD_BOSS5_BODY_FLAG_SIGNAL_E2B_EGG_GOT_IN;
		
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
