// =======================================================================
/*!
  @file	gmBoss5Turret.cpp
  @brief ボスファイナル 砲塔

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5Turret.cpp 2 2011-04-11 05:21:26Z thamada $
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

// データヘッダ
#include "../file/common/arc/BOSS05.hmb"
#include "../file/common/model/BOSS05_MDL.hmb"
#include "../file/common/model/BOSS05_BODY_MTN.hmb"

/*------ Macros --------------------------------------------------------*/
//############ 砲塔 ###########################################################
/* フラグ */
/* 定義値 */
#define GMD_BOSS5_TURRET_TILT_NEAR_ANGLE		(AKM_DEGtoA32(45))	//!< 砲塔の2D全方位回転時に、手前に少し傾ける角度
#define GMD_BOSS5_TURRET_FACE_PLY_SPD_DEG		(2.f)				//!< プレイヤーを向くときの砲塔回転最大角速度
#define GMD_BOSS5_TURRET_FACE_TIME				(10)				//!< 出現処理完了直後から発射開始までの間にプレイヤーの位置を追いかける時間

#define GMD_BOSS5_TURRET_SLIDE_LENGTH_MAX		(16.f)				//!< 銃座ノードからのY方向オフセット位置最大値
#define GMD_BOSS5_TURRET_SLIDE_RAISE_SPD_F		((Float)2.f)		//!< 砲塔スライド上昇速度
#define GMD_BOSS5_TURRET_SLIDE_LOWER_SPD_F		((Float)2.f)		//!< 砲塔スライド下降速度

#define GMD_BOSS5_TURRET_COVER_SLIDE_DEG_MAX	(-135.f)			//!< カバーの最大開放角度
#define GMD_BOSS5_TURRET_COVER_SLIDE_SCALE_MAX	(0.8f)				//!< カバー最大開放時のスケール
#define GMD_BOSS5_TURRET_COVER_SLIDE_OPEN_RATIO_SPD_F	(0.07f)		//!< カバーオープンのスライド度合い増加速度
#define GMD_BOSS5_TURRET_COVER_SLIDE_CLOSE_RATIO_SPD_F	(0.07f)		//!< カバーオープンのスライド度合い減少速度

#define GMD_BOSS5_TURRET_SLIDE_POLE_DISP_OFST_Y	(-12.5f)			//!< ポールの、砲塔に対する表示オフセット座標

// バルカン発射関連
#define GMD_BOSS5_TURRET_VULCAN_SHOT_INTERVAL	(40)				//!< バルカン連射時の個々の弾の発射間隔
#define GMD_BOSS5_TURRET_VULCAN_BULLET_SPD		((fx32)(FX32_ONE * 2))	//!< バルカン弾速度
#define GMD_BOSS5_TURRET_VULCAN_FIRE_OFST_FORWARD	((fx32)(FX32_ONE * 16.f))	//!< 発射エフェクトオフセット位置（砲塔中心から砲口方向へのオフセット）
#define GMD_BOSS5_TURRET_VULCAN_FIRE_OFST_Z			((fx32)(FX32_ONE * 32.f))	//!< 砲塔等より手前に表示させるための、カメラ側へのオフセット（マップ座標系）
#define GMD_BOSS5_TURRET_VULCAN_BULLET_OFST_FORWARD	((fx32)(FX32_ONE * 16.f))	//!< 弾エフェクトオフセット位置（砲塔中心から砲口方向へのオフセット）
#define GMD_BOSS5_TURRET_VULCAN_BULLET_OFST_Z		((fx32)(FX32_ONE * 32.f))	//!< 砲塔等より手前に表示させるための、カメラ側へのオフセット（マップ座標系）

//############ 動作シーケンス #################################################
// 砲塔
#define GMD_BOSS5_TURRET_SEQ_LIFE_LEVEL_NUM		(5)					//!< ライフレベル数

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! 砲塔 シーケンス バルカン発射情報構造体
typedef struct tag_GMS_BOSS5_TURRET_SEQ_VUL_SHOT_INFO
{
	Sint32	life_threshold;	//!< 残りライフ(最後の一撃分含む)がこの値「以下」なら該当
	Uint32	wait_time;		//!< 次の連射までの待ち時間
	Sint32	shot_num;		//!< 連射数
} GMS_BOSS5_TURRET_SEQ_VUL_SHOT_INFO;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
//############ ボスFINAL砲塔 ##################################################
/* インターフェース関数 */
/* 補助関数 */
static void gmBoss5TurretGetDispRotatedOfstPos(GMS_BOSS5_TURRET_WORK *trt_work, const VecFx32 *src_ofst_pos,
											   VecFx32 *dest_ofst_pos);
static void gmBoss5TurretGetVulcanFirePos(GMS_BOSS5_TURRET_WORK *trt_work, VecFx32 *out_pos);
static void gmBoss5TurretGetVulcanBulletPos(GMS_BOSS5_TURRET_WORK *trt_work, VecFx32 *out_pos);
static void gmBoss5TurretInitDispRot(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretUpdateDispRot(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretUpdateDirFollowingPos(GMS_BOSS5_TURRET_WORK *trt_work, const VecFx32 *targ_pos, Float deg);
static void gmBoss5TurretSetRoundFaceRot(GMS_BOSS5_TURRET_WORK *trt_work, Angle32 dir_z_angle, Angle32 tilt_near_angle);
static void gmBoss5TurretUpdateDirFacePly(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretInitVulcanBurstShot(GMS_BOSS5_TURRET_WORK *trt_work, Sint32 shot_num);
static BOOL gmBoss5TurretUpdateVulcanBurstShot(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretClearVulcanBurstShot(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretInitPartsPose(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretEndPartsPose(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretInitSlideTurret(GMS_BOSS5_TURRET_WORK *trt_work,
										 GME_BOSS5_TURRET_SLIDE_TYPE slide_type);
static BOOL gmBoss5TurretUpdateSlideTurret(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretInitSlideCover(GMS_BOSS5_TURRET_WORK *trt_work,
										GME_BOSS5_TURRET_COVER_SLIDE_TYPE slide_type);
static BOOL gmBoss5TurretUpdateSlideCover(GMS_BOSS5_TURRET_WORK *trt_work);
/* 処理関数 */
/* 制御処理 */
static void gmBoss5TurretMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
// 砲塔シーケンス
static void gmBoss5TurretProcInit(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretProcUpdateStandby(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretProcUpdateOpen(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretProcUpdateAppear(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretProcUpdateFace(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretProcUpdateFire(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretProcUpdateDisappear(GMS_BOSS5_TURRET_WORK *trt_work);
static void gmBoss5TurretProcUpdateClose(GMS_BOSS5_TURRET_WORK *trt_work);

//############ 動作シーケンス #################################################
static Uint32 gmBoss5TurretSeqGetVulcanWaitTime(GMS_BOSS5_TURRET_WORK *trt_work);
static Sint32 gmBoss5TurretSeqGetVulcanShotNum(GMS_BOSS5_TURRET_WORK *trt_work);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! 砲塔 シーケンス バルカン発射情報テーブル
const static GMS_BOSS5_TURRET_SEQ_VUL_SHOT_INFO gm_boss5_trt_seq_vul_shot_info_tbl[GMD_BOSS5_TURRET_SEQ_LIFE_LEVEL_NUM]	= {
	//	threshold								wait_time	shot_num
	{	1,										0,			0,	},
	{	2,										5*60,		5,	},
	{	4,										5*60,		3,	},
	{	GMD_BOSS5_TURRET_START_LIFE_THRESHOLD,	8*60,		3,	},
	{	GMD_BOSS5_LIFE,							0,			0,	},
};

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmBoss5TurretInit
/*!
  ボスFINAL 砲塔初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss5TurretInit(GMS_EVE_RECORD_EVENT *eve_rec,
								  fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_3D_WORK	*ene_3d;
	GMS_BOSS5_TURRET_WORK	*trt_work;
	
	// オブジェクト生成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS5_TURRET_WORK),
										"BOSS5_TRT");
	
	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	trt_work	= (GMS_BOSS5_TURRET_WORK*)obj_work;
	
	// 地形当たり無し
	
	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 &(GmBoss5GetObject3dList()[IDB_BOSS05_MDL_B05_BALKAN_ZNO]),
								 &ene_3d->obj_3d);
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= GMD_BOSS5_DEFAULT_BLEND_SPD;	// 念のため設定しておく
	
	// ワーク設定
	obj_work->flag	|= (OBD_OBJECT_NOCLIP | OBD_OBJECT_NOHIT);
	obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	
	// ホーミングの対象外
	ene_3d->ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// 表示回転初期化
	gmBoss5TurretInitDispRot(trt_work);
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss5TurretMain;
	
	// シーケンス初期化
	gmBoss5TurretProcInit(trt_work);
	
#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}


// =======================================================================
// GmBoss5TurretStartUp
/*!
  砲塔起動
  
  @param body_work	[io]	本体ワーク
  
  @return 砲塔ワーク
 */
// =======================================================================
GMS_BOSS5_TURRET_WORK* GmBoss5TurretStartUp(GMS_BOSS5_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_body	= GMM_BS_OBJ(body_work);
	OBS_OBJECT_WORK	*obj_trt;
	
	// 生成
	obj_trt	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS5_TURRET,
										obj_body->pos.x, obj_body->pos.y,
										0,//flag
										0,0,0,0,
										0);
	
	// 親設定
	obj_trt->parent_obj	= obj_body;
	
	return (GMS_BOSS5_TURRET_WORK*)obj_trt;
}


/*------ Static Functions ----------------------------------------------*/
// ============================================================================
// インターフェース関数
// ============================================================================



// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss5TurretGetDispRotatedOfstPos
/*!
  オフセット座標の表示回転適用後オフセット座標取得
  
  @param trt_work		[io]	砲塔ワーク
  @param src_ofst_pos	[in]	基本姿勢基準のオフセット（マップ座標系）
  @param dest_ofst_pos	[out]	結果オフセット座標格納先（マップ座標系）
  
  @note
  砲塔の基本姿勢時の砲塔中心から砲塔先端へのオフセット座標から、
  砲塔の表示回転を適用した後の砲塔先端のオフセット座標を取得します。
 */
// =======================================================================
void gmBoss5TurretGetDispRotatedOfstPos(GMS_BOSS5_TURRET_WORK *trt_work, const VecFx32 *src_ofst_pos,
										VecFx32 *dest_ofst_pos)
{
	NNS_VECTOR	ofst_pos_f;
	NNS_MATRIX	rot_mtx;
	
	// 描画座標系でオフセットを取得
	amVectorSet(&ofst_pos_f,
				FX_FX32_TO_F32(src_ofst_pos->x),
				FX_FX32_TO_F32(-src_ofst_pos->y),
				FX_FX32_TO_F32(src_ofst_pos->z));
	
	// 回転マトリクス取得
	nnMakeQuaternionMatrix(&rot_mtx, &trt_work->disp_quat);
	
	// 回転適用
	nnTransformVector(&ofst_pos_f, &rot_mtx, &ofst_pos_f);
	
	// マップ座標系に戻す
	VEC_Set(dest_ofst_pos,
			FX_F32_TO_FX32(ofst_pos_f.x),
			FX_F32_TO_FX32(-ofst_pos_f.y),
			FX_F32_TO_FX32(ofst_pos_f.z));
}

// =======================================================================
// gmBoss5TurretGetVulcanFirePos
/*!
  バルカン発射エフェクト生成座標取得
  
  @param trt_work	[io]	砲塔ワーク
  @param out_pos	[out]	結果座標（マップ座標系）
  
  @note
  砲塔の姿勢から、バルカン発射エフェクトを生成する位置を取得します。
  Z位置については砲塔の位置に対して常に固定になります。
  GMS_BOSS5_TURRET_WORK::disp_quatが確定した後に呼び出してください。
 */
// =======================================================================
void gmBoss5TurretGetVulcanFirePos(GMS_BOSS5_TURRET_WORK *trt_work, VecFx32 *out_pos)
{
	const static VecFx32 ofst_pos	= {0, 0, GMD_BOSS5_TURRET_VULCAN_FIRE_OFST_FORWARD};
	VecFx32	rotated_ofst_pos;
	OBS_OBJECT_WORK	*obj_trt	= GMM_BS_OBJ(trt_work);
	
	MTM_ASSERT(trt_work);
	MTM_ASSERT(out_pos);
	
	// 砲塔回転後のオフセットを取得
	gmBoss5TurretGetDispRotatedOfstPos(trt_work, &ofst_pos, &rotated_ofst_pos);
	
	// マップ座標系に変換
	VEC_Set(out_pos,
			obj_trt->pos.x + rotated_ofst_pos.x,
			obj_trt->pos.y + rotated_ofst_pos.y,
			obj_trt->pos.z + GMD_BOSS5_TURRET_VULCAN_FIRE_OFST_Z);
}

// =======================================================================
// gmBoss5TurretGetVulcanBulletPos
/*!
  バルカン弾エフェクト生成座標取得
  
  @param trt_work	[io]	砲塔ワーク
  @param out_pos	[out]	結果座標（マップ座標系）
  
  @note
  砲塔の姿勢から、バルカン弾エフェクトを生成する位置を取得します。
  Z位置については砲塔の位置に対して常に固定になります。
  GMS_BOSS5_TURRET_WORK::disp_quatが確定した後に呼び出してください。
 */
// =======================================================================
void gmBoss5TurretGetVulcanBulletPos(GMS_BOSS5_TURRET_WORK *trt_work, VecFx32 *out_pos)
{
	const static VecFx32 ofst_pos	= {0, 0, GMD_BOSS5_TURRET_VULCAN_BULLET_OFST_FORWARD};
	VecFx32	rotated_ofst_pos;
	OBS_OBJECT_WORK	*obj_trt	= GMM_BS_OBJ(trt_work);
	
	MTM_ASSERT(trt_work);
	MTM_ASSERT(out_pos);
	
	// 砲塔回転後のオフセットを取得
	gmBoss5TurretGetDispRotatedOfstPos(trt_work, &ofst_pos, &rotated_ofst_pos);
	
	// マップ座標系に変換
	VEC_Set(out_pos,
			obj_trt->pos.x + rotated_ofst_pos.x,
			obj_trt->pos.y + rotated_ofst_pos.y,
			obj_trt->pos.z + GMD_BOSS5_TURRET_VULCAN_BULLET_OFST_Z);
}

// =======================================================================
// gmBoss5TurretInitDispRot
/*!
  表示回転初期化
  
  @param trt_work	[io]	砲塔ワーク
 */
// =======================================================================
void gmBoss5TurretInitDispRot(GMS_BOSS5_TURRET_WORK *trt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(trt_work);
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	
	// 初期化
	obj_work->disp_flag	&= ~OBD_DISP_USERMTX_RIGHT;
	nnMakeUnitQuaternion(&trt_work->disp_quat);
	nnMakeUnitMatrix(&obj_work->obj_3d->user_obj_mtx_r);
}

// =======================================================================
// gmBoss5TurretUpdateDispRot
/*!
  表示回転更新
  
  @param trt_work	[io]	砲塔ワーク
  
  @note
  表示用の回転行列をオブジェクトに設定します。
 */
// =======================================================================
void gmBoss5TurretUpdateDispRot(GMS_BOSS5_TURRET_WORK *trt_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(trt_work);
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	
	// ユーザマトリクスR設定
	obj_work->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
	nnMakeQuaternionMatrix(&obj_work->obj_3d->user_obj_mtx_r,
						   &trt_work->disp_quat);
}

// =======================================================================
// gmBoss5TurretUpdateDirFollowingPos
/*!
  指定座標の方向に発射角度を更新
  
  @param trt_work	[io]	砲塔ワーク
  @param targ_pos	[in]	目標座標
  @param deg		[in]	最大回転速度（degree値）
  
  @note
  OBS_OBJECT_WORK::dirは更新しません。
 */
// =======================================================================
void gmBoss5TurretUpdateDirFollowingPos(GMS_BOSS5_TURRET_WORK *trt_work, const VecFx32 *targ_pos, Float deg)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(trt_work);
	Angle32	angle;
	Angle32	diff_angle;
	Angle32	add_angle;
	
	MTM_ASSERT(deg >= 0.f);
	
	angle	= MTD_MATH_ANGLE_MASK & nnArcTan2(FX_FX32_TO_F32(targ_pos->y - obj_work->pos.y),
											  FX_FX32_TO_F32(targ_pos->x - obj_work->pos.x));
	
	// 差分角度を取得
	diff_angle	= MTD_MATH_ANGLE_MASK & (angle - (Angle32)trt_work->fire_dir_z);
	
	// 回転方向を決定
	if (diff_angle >= AKM_DEGtoA32(180)) {
		diff_angle	= -(AKM_DEGtoA32(360) - diff_angle);
		add_angle	= AKM_DEGtoA32(-deg);
	}
	else {
		add_angle	= AKM_DEGtoA32(deg);
	}
	
	// 指定速度未満の角速度ならば差分角度をそのまま適用する
	if (MTM_MATH_ABS(diff_angle) <= MTM_MATH_ABS(add_angle)) {
		add_angle	= diff_angle;
	}
	
	trt_work->fire_dir_z	= (Angle16)(MTD_MATH_ANGLE_MASK & ((Angle32)trt_work->fire_dir_z + add_angle));
}


// =======================================================================
// gmBoss5TurretSetRoundFaceRot
/*!
  2D全方位回転用に回転情報を設定
  
  @param trt_work			[io]	砲塔ワーク
  @param dir_z_deg			[in]	Z回転角度（マップ座標系）
  @param tilt_near_angle	[in]	手前傾き角度(+値で手前側に向く)
  
  @note
  ここで設定した値が反映されるには、
  gmBoss5TurretUpdateDispRot()が呼ばれている必要があります。
 */
// =======================================================================
void gmBoss5TurretSetRoundFaceRot(GMS_BOSS5_TURRET_WORK *trt_work, Angle32 dir_z_angle, Angle32 tilt_near_angle)
{
	Angle32	fire_angle;
	AMS_QUAT	fire_rot_quat;
	
	// 発射方向を描画座標系で取得
	fire_angle	= MTD_MATH_ANGLE_MASK & -(Angle32)dir_z_angle;
	
	// 基準姿勢（右向き）
	// Y+90だとZ回転したときにシンメトリーでなくなるので、X+90→Z+90で右に向ける。
	nnMakeRotateXZYQuaternion(&trt_work->disp_quat,
							  AKM_DEGtoA32(90),		// 下向ける(X回転)
							  -tilt_near_angle,		// 少し手前に傾ける(Y回転)
							  AKM_DEGtoA32(90));	// +Xを基準にする(Z回転)
	
	// 発射方向回転のクォータニオン
	nnMakeRotateXYZQuaternion(&fire_rot_quat,
							  0, 0, fire_angle);
	
	// 発射方向回転適用
	nnMultiplyQuaternion(&trt_work->disp_quat, &fire_rot_quat, &trt_work->disp_quat);
}

// =======================================================================
// gmBoss5TurretUpdateDirFacePly
/*!
  プレイヤーの方向を向く 更新
  
  @param trt_work	[io]	砲塔ワーク
 */
// =======================================================================
void gmBoss5TurretUpdateDirFacePly(GMS_BOSS5_TURRET_WORK *trt_work)
{
	OBS_OBJECT_WORK	*obj_ply	= GmBsCmnGetPlayerObj();
	Angle32	tilt_angle;
	
	// プレイヤーの方向に向くように発射角を更新
	gmBoss5TurretUpdateDirFollowingPos(trt_work, &obj_ply->pos,
									   GMD_BOSS5_TURRET_FACE_PLY_SPD_DEG);
	
#if 1
	// 下方向を向くほど手前に傾く(deg=90で傾き最大)
	// 完全に水平方向(deg=0,180)の時は手前への傾き無し
	
	MTM_ASSERT(GMD_BOSS5_TURRET_TILT_NEAR_ANGLE >= 0);
	{
		Float	sin_val;
		
		sin_val	= nnSin(trt_work->fire_dir_z);
		if (sin_val < 0) {
			// 上向き(deg=180～360)の時は傾き無し
			sin_val	= 0;
		}
		
		// 傾き角取得
		tilt_angle	= (Angle32)(sin_val * (MTD_MATH_ANGLE_MASK & GMD_BOSS5_TURRET_TILT_NEAR_ANGLE));
	}
	
	// 2D全方位角設定
	gmBoss5TurretSetRoundFaceRot(trt_work,
								 trt_work->fire_dir_z,
								 tilt_angle);

#else
	// 手前への傾きが常に同じ角度
	
	// 2D全方位角設定
	gmBoss5TurretSetRoundFaceRot(trt_work,
								 trt_work->fire_dir_z,
								 GMD_BOSS5_TURRET_TILT_NEAR_ANGLE);
#endif
}

// =======================================================================
// gmBoss5TurretInitVulcanBurstShot
/*!
  バルカン連射処理初期化
  
  @param trt_work	[io]	砲塔ワーク
  @param shot_num	[io]	連射数
  
  @note
  バルカンを指定回数連続で発射します。
  発射間隔は既定値を使用しています。
 */
// =======================================================================
void gmBoss5TurretInitVulcanBurstShot(GMS_BOSS5_TURRET_WORK *trt_work, Sint32 shot_num)
{
	// 連射数設定
	trt_work->vul_shot_remain	= shot_num;
	trt_work->vul_burst_timer	= GMD_BOSS5_TURRET_VULCAN_SHOT_INTERVAL;
	
	// 角度設定
	trt_work->vul_shot_angle	= trt_work->fire_dir_z;
	
	// 発射エフェクト生成位置設定
	gmBoss5TurretGetVulcanFirePos(trt_work, &trt_work->vul_fire_pos);
	
	// 弾エフェクト生成位置設定
	gmBoss5TurretGetVulcanBulletPos(trt_work, &trt_work->vul_bullet_pos);
}

// =======================================================================
// gmBoss5TurretUpdateVulcanBurstShot
/*!
  バルカン連射処理更新
  
  @param trt_work	[io]	砲塔ワーク
  
  @retval TRUE	連射完了
  @retval FALSE	連射中
 */
// =======================================================================
BOOL gmBoss5TurretUpdateVulcanBurstShot(GMS_BOSS5_TURRET_WORK *trt_work)
{
	// 指定回数発射する
	if (trt_work->vul_shot_remain) {
		
		// 既定間隔で発射
		if (trt_work->vul_burst_timer) {
			trt_work->vul_burst_timer--;
		}
		else {
			
			// 発射エフェクト生成位置設定
			gmBoss5TurretGetVulcanFirePos(trt_work, &trt_work->vul_fire_pos);
			
			// 弾エフェクト生成位置設定
			gmBoss5TurretGetVulcanBulletPos(trt_work, &trt_work->vul_bullet_pos);
			
			// 発射エフェクト
			GmBoss5EfctCreateVulcanFire(trt_work,
										&trt_work->vul_fire_pos,
										trt_work->vul_shot_angle);
			// 弾エフェクト
			GmBoss5EfctCreateVulcanBullet(trt_work,
										  &trt_work->vul_bullet_pos,
										  trt_work->vul_shot_angle,
										  GMD_BOSS5_TURRET_VULCAN_BULLET_SPD);
			
			trt_work->vul_shot_remain--;
			trt_work->vul_burst_timer	= GMD_BOSS5_TURRET_VULCAN_SHOT_INTERVAL;
		}
	}
	else {
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss5TurretClearVulcanBurstShot
/*!
  バルカン連射処理クリア
  
  @param trt_work	[io]	砲塔ワーク
 */
// =======================================================================
void gmBoss5TurretClearVulcanBurstShot(GMS_BOSS5_TURRET_WORK *trt_work)
{
	// パラメータクリア
	trt_work->vul_shot_remain	= 0;
	trt_work->vul_burst_timer	= 0;
}

// =======================================================================
// gmBoss5TurretInitPartsPose
/*!
  本体側操作パーツ姿勢操作初期化
  
  @param trt_work	[io]	砲塔ワーク
  
  @note
  ポール・カバーの姿勢操作が可能となるように（本体側の）CNM有効化などの初期化処理を行います。
 */
// =======================================================================
void gmBoss5TurretInitPartsPose(GMS_BOSS5_TURRET_WORK *trt_work)
{
	NNS_MATRIX	unit_mtx;
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(trt_work)->parent_obj;
	const Sint32	cnm_reg_id_tbl[]	= {
		parent_body->pole_cnm_reg_id,
		parent_body->cover_cnm_reg_id,
	};
	const Sint32	elem_num	= sizeof(cnm_reg_id_tbl) / sizeof(cnm_reg_id_tbl[0]);
														  
	nnMakeUnitMatrix(&unit_mtx);
	
	for (Sint32 i = 0; i < elem_num; ++i) {
		// CNM 左乗算モードに変更
		GmBsCmnChangeCNMModeNode(&parent_body->cnm_mgr_work,
								 cnm_reg_id_tbl[i],
								 GME_BS_CMN_CNM_MODE_MULT_LEFT);
		// CNM ノードローカル座標系でマトリクス設定
		GmBsCmnEnableCNMLocalCoordinate(&parent_body->cnm_mgr_work,
										cnm_reg_id_tbl[i],
										TRUE);
		// CNM 適用有効化
		GmBsCmnEnableCNMMtxNode(&parent_body->cnm_mgr_work,
								cnm_reg_id_tbl[i],
								TRUE);
		// 設定マトリクス初期化
		GmBsCmnSetCNMMtx(&parent_body->cnm_mgr_work,
						 &unit_mtx,
						 cnm_reg_id_tbl[i]);
	}
}

// =======================================================================
// gmBoss5TurretEndPartsPose
/*!
  本体側操作パーツ姿勢操作終了
  
  @param trt_work	[io]	砲塔ワーク
  
  @note
  姿勢操作対象パーツのCNMの初期化などを行い、ポール・カバー姿勢操作を終了します。
  使用しない場合は必ずこの関数を呼んで終了してください。
 */
// =======================================================================
void gmBoss5TurretEndPartsPose(GMS_BOSS5_TURRET_WORK *trt_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(trt_work)->parent_obj;
	
	const Sint32	cnm_reg_id_tbl[]	= {
		parent_body->pole_cnm_reg_id,
		parent_body->cover_cnm_reg_id,
	};
	const Sint32	elem_num	= sizeof(cnm_reg_id_tbl) / sizeof(cnm_reg_id_tbl[0]);
	
	for (Sint32 i = 0; i < elem_num; ++i) {
		// CNM適用無効化
		GmBsCmnEnableCNMMtxNode(&parent_body->cnm_mgr_work,
								cnm_reg_id_tbl[i],
								FALSE);
	}
}

// =======================================================================
// gmBoss5TurretInitSlideTurret
/*!
  砲塔スライド処理 初期化
  
  @param trt_work	[io]	砲塔ワーク
  @param slide_type	[in]	スライドタイプ
  
  @note
  砲塔を支えるポールも動かします。
 */
// =======================================================================
void gmBoss5TurretInitSlideTurret(GMS_BOSS5_TURRET_WORK *trt_work,
								  GME_BOSS5_TURRET_SLIDE_TYPE slide_type)
{
	// スライドタイプ設定
	trt_work->trt_slide_type	= slide_type;
	
	if (slide_type == GME_BOSS5_TURRET_SLIDE_TYPE_RAISE) {
		trt_work->trt_slide_length	= 0;
	}
	else {
		MTM_ASSERT(slide_type == GME_BOSS5_TURRET_SLIDE_TYPE_LOWER);
		trt_work->trt_slide_length	= GMD_BOSS5_TURRET_SLIDE_LENGTH_MAX;
	}
}

// =======================================================================
// gmBoss5TurretUpdateSlideTurret
/*!
  砲塔スライド処理 更新
  
  @param trt_work	[io]	砲塔ワーク
  
  @retval TRUE	スライド終了
  @retval FALSE	スライド中
 */
// =======================================================================
BOOL gmBoss5TurretUpdateSlideTurret(GMS_BOSS5_TURRET_WORK *trt_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(trt_work)->parent_obj;
	BOOL	result;
	
	if (trt_work->trt_slide_type == GME_BOSS5_TURRET_SLIDE_TYPE_RAISE) {
		// 上昇スライド
		if (trt_work->trt_slide_length < GMD_BOSS5_TURRET_SLIDE_LENGTH_MAX) {
			trt_work->trt_slide_length	+= GMD_BOSS5_TURRET_SLIDE_RAISE_SPD_F;
			result	= FALSE;
		}
		else {
			trt_work->trt_slide_length	= GMD_BOSS5_TURRET_SLIDE_LENGTH_MAX;
			result	= TRUE;
		}
	}
	else {
		// 下降スライド
		MTM_ASSERT(trt_work->trt_slide_type == GME_BOSS5_TURRET_SLIDE_TYPE_LOWER);
		if (trt_work->trt_slide_length > 0) {
			trt_work->trt_slide_length	-= GMD_BOSS5_TURRET_SLIDE_LOWER_SPD_F;
			result	= FALSE;
		}
		else {
			trt_work->trt_slide_length	= 0;
			result	= TRUE;
		}
	}
	
	
	// スライド位置をポールに反映
	{
		NNS_MATRIX	slide_mtx;
		nnMakeTranslateMatrix(&slide_mtx,
							  0,
							  trt_work->trt_slide_length + GMD_BOSS5_TURRET_SLIDE_POLE_DISP_OFST_Y,
							  0);
		GmBsCmnSetCNMMtx(&parent_body->cnm_mgr_work,
						 &slide_mtx,
						 parent_body->pole_cnm_reg_id);
	}
	
	return result;
}

// =======================================================================
// gmBoss5TurretInitSlideCover
/*!
  カバースライド処理 初期化
  
  @param trt_work	[io]	砲塔ワーク
  
  @note
  本体頭部にある砲塔のカバーをスライド開閉します。
 */
// =======================================================================
void gmBoss5TurretInitSlideCover(GMS_BOSS5_TURRET_WORK *trt_work,
								 GME_BOSS5_TURRET_COVER_SLIDE_TYPE slide_type)
{
	// スライドタイプ設定
	trt_work->cvr_slide_type	= slide_type;
	
	if (slide_type == GME_BOSS5_TURRET_COVER_SLIDE_TYPE_OPEN) {
		trt_work->cvr_slide_ratio	= 0.f;
	}
	else {
		MTM_ASSERT(slide_type == GME_BOSS5_TURRET_COVER_SLIDE_TYPE_CLOSE);
		
		trt_work->cvr_slide_ratio	= 1.f;
	}
}

// =======================================================================
// gmBoss5TurretUpdateSlideCover
/*!
  カバースライド処理 更新
  
  @param trt_work	[io]	砲塔ワーク
  
  @retval TRUE	スライド終了
  @retval FALSE	スライド中
 */
// =======================================================================
BOOL gmBoss5TurretUpdateSlideCover(GMS_BOSS5_TURRET_WORK *trt_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(trt_work)->parent_obj;
	BOOL	result;
	
	if (trt_work->cvr_slide_type == GME_BOSS5_TURRET_COVER_SLIDE_TYPE_OPEN) {
		if (trt_work->cvr_slide_ratio < 1.f) {
			trt_work->cvr_slide_ratio	+= GMD_BOSS5_TURRET_COVER_SLIDE_OPEN_RATIO_SPD_F;
			result	= FALSE;
		}
		else {
			trt_work->cvr_slide_ratio	= 1.f;
			result	= TRUE;
		}
	}
	else {
		MTM_ASSERT(trt_work->cvr_slide_type == GME_BOSS5_TURRET_COVER_SLIDE_TYPE_CLOSE);
		if (trt_work->cvr_slide_ratio > 0.f) {
			trt_work->cvr_slide_ratio	-= GMD_BOSS5_TURRET_COVER_SLIDE_CLOSE_RATIO_SPD_F;
			result	= FALSE;
		}
		else {
			trt_work->cvr_slide_ratio	= 0.f;
			result	= TRUE;
		}
	}
	
	// 回転をカバーに反映
	{
		NNS_MATRIX	slide_mtx;
		Float	scale	= 1.f + trt_work->cvr_slide_ratio * (GMD_BOSS5_TURRET_COVER_SLIDE_SCALE_MAX - 1.f);
		nnMakeRotateXMatrix(&slide_mtx,
						AKM_DEGtoA32(trt_work->cvr_slide_ratio * GMD_BOSS5_TURRET_COVER_SLIDE_DEG_MAX));
		nnScaleMatrix(&slide_mtx, &slide_mtx, scale, scale, scale);
		GmBsCmnSetCNMMtx(&parent_body->cnm_mgr_work,
						 &slide_mtx,
						 parent_body->cover_cnm_reg_id);
	}
	
	return result;
}

// ============================================================================
// 処理関数
// ============================================================================



// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss5TurretMain
/*!
  砲塔 メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss5TurretMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS5_TURRET_WORK	*trt_work	= (GMS_BOSS5_TURRET_WORK*)obj_work;
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)obj_work->parent_obj;
	NNS_MATRIX	trt_ofst;
	
	// 更新処理
	if (trt_work->proc_update) {
		trt_work->proc_update(trt_work);
	}
	
	// オフセットマトリクス作成
	nnMakeTranslateMatrix(&trt_ofst, 0, trt_work->trt_slide_length, 0);
	
	// 銃座ノード追随
	GmBsCmnUpdateObject3DNNStuckWithNodeRelative(obj_work,
												 &parent_body->snm_work,
												 parent_body->pole_snm_reg_id,
												 FALSE,
												 &obj_work->parent_obj->pos,
												 &parent_body->pivot_prev_pos,
												 &trt_ofst);
	
	// 回転を反映
	gmBoss5TurretUpdateDispRot(trt_work);
}


// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss5TurretProc****
/*!
  砲塔シーケンス
  
  @param trt_work	[io]	砲塔ワーク
 */
// =======================================================================
// 砲塔シーケンス初期化
void gmBoss5TurretProcInit(GMS_BOSS5_TURRET_WORK *trt_work)
{
	trt_work->proc_update	= gmBoss5TurretProcUpdateStandby;
}

// 砲塔シーケンス更新 待機処理
void gmBoss5TurretProcUpdateStandby(GMS_BOSS5_TURRET_WORK *trt_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(trt_work)->parent_obj;
	
	// バルカン禁止中、もしくはもう発射しないときは何もしない
	if (parent_body->flag & GMD_BOSS5_BODY_FLAG_HOLD_VULCAN ||
		gmBoss5TurretSeqGetVulcanShotNum(trt_work) <= 0) {
		return;
	}
	
	// 次の発射までの待機
	if (trt_work->wait_timer) {
		trt_work->wait_timer--;
	}
	else {
		
		// パーツ姿勢操作開始
		gmBoss5TurretInitPartsPose(trt_work);
		
		// カバーオープン処理初期化
		gmBoss5TurretInitSlideCover(trt_work,
									GME_BOSS5_TURRET_COVER_SLIDE_TYPE_OPEN);
		
		// 初期角度をプレイヤーの方向に設定する
		gmBoss5TurretUpdateDirFollowingPos(trt_work, &GmBsCmnGetPlayerObj()->pos,
										   360.f);
		
		trt_work->proc_update	= gmBoss5TurretProcUpdateOpen;
	}
}

// 砲塔シーケンス更新 カバーオープン処理
void gmBoss5TurretProcUpdateOpen(GMS_BOSS5_TURRET_WORK *trt_work)
{
	if (gmBoss5TurretUpdateSlideCover(trt_work)) {
		// スライド上昇処理初期化
		gmBoss5TurretInitSlideTurret(trt_work, GME_BOSS5_TURRET_SLIDE_TYPE_RAISE);
		
		trt_work->proc_update	= gmBoss5TurretProcUpdateAppear;
	}
}

// 砲塔シーケンス更新 出現処理
void gmBoss5TurretProcUpdateAppear(GMS_BOSS5_TURRET_WORK *trt_work)
{
	// プレイヤーの方向を向く（gmBoss5TurretInitVulcanBurstShot()より前に呼び出すこと）
	gmBoss5TurretUpdateDirFacePly(trt_work);
	
	if (gmBoss5TurretUpdateSlideTurret(trt_work)) {
		
		trt_work->wait_timer	= GMD_BOSS5_TURRET_FACE_TIME;
		
		trt_work->proc_update	= gmBoss5TurretProcUpdateFace;
	}
}

// 砲塔シーケンス更新 プレイヤーの方向を向く処理
void gmBoss5TurretProcUpdateFace(GMS_BOSS5_TURRET_WORK *trt_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(trt_work)->parent_obj;
	
	// プレイヤーの方向を向く（gmBoss5TurretInitVulcanBurstShot()より前に呼び出すこと）
	gmBoss5TurretUpdateDirFacePly(trt_work);
	
	// バルカン禁止フラグが立っていたら直ちに終了
	if (parent_body->flag & GMD_BOSS5_BODY_FLAG_HOLD_VULCAN) {
		
		trt_work->wait_timer	= 0;	// タイマクリア
		trt_work->proc_update	= gmBoss5TurretProcUpdateDisappear;
		return;
	}
	
	// 既定時間プレイヤーの方向を追いかける
	if (trt_work->wait_timer) {
		trt_work->wait_timer--;
	}
	else {
		
		// バルカン発射初期化
		gmBoss5TurretInitVulcanBurstShot(trt_work,
										 gmBoss5TurretSeqGetVulcanShotNum(trt_work));
		
		trt_work->proc_update	= gmBoss5TurretProcUpdateFire;
	}
}

// 砲塔シーケンス更新 発射処理
void gmBoss5TurretProcUpdateFire(GMS_BOSS5_TURRET_WORK *trt_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(trt_work)->parent_obj;
	
	// バルカンを発射し終えるか、禁止フラグが立つまで発射更新
	if (gmBoss5TurretUpdateVulcanBurstShot(trt_work) ||
		parent_body->flag & GMD_BOSS5_BODY_FLAG_HOLD_VULCAN) {
		
		// バルカン発射終了
		gmBoss5TurretClearVulcanBurstShot(trt_work);
		
		
		// スライド下降処理初期化
		gmBoss5TurretInitSlideTurret(trt_work, GME_BOSS5_TURRET_SLIDE_TYPE_LOWER);
		
		trt_work->proc_update	= gmBoss5TurretProcUpdateDisappear;
	}
}


// 砲塔シーケンス更新 収納処理
void gmBoss5TurretProcUpdateDisappear(GMS_BOSS5_TURRET_WORK *trt_work)
{
	if (gmBoss5TurretUpdateSlideTurret(trt_work)) {
		
		// 待機時間設定
		trt_work->wait_timer	= gmBoss5TurretSeqGetVulcanWaitTime(trt_work);
		
		// カバークローズ処理初期化
		gmBoss5TurretInitSlideCover(trt_work,
									GME_BOSS5_TURRET_COVER_SLIDE_TYPE_CLOSE);
		
		trt_work->proc_update	= gmBoss5TurretProcUpdateClose;
	}
}

// 砲塔シーケンス カバークローズ処理
void gmBoss5TurretProcUpdateClose(GMS_BOSS5_TURRET_WORK *trt_work)
{
	if (gmBoss5TurretUpdateSlideCover(trt_work)) {
		
		// パーツ姿勢操作終了
		gmBoss5TurretEndPartsPose(trt_work);
		
		trt_work->proc_update	= gmBoss5TurretProcUpdateStandby;
	}
}


// ############################################################################
// ボスFINAL 砲塔 動作シーケンス
// ############################################################################
// =======================================================================
// gmBoss5TurretSeqGetVulcanWaitTime
/*!
  バルカン発射待ち時間取得
  
  @param trt_work	[io]	砲塔ワーク
  
  @return 発射待ち時間
 
  @note
  次の連射までの待ち時間を取得します。
 */
// =======================================================================
Uint32 gmBoss5TurretSeqGetVulcanWaitTime(GMS_BOSS5_TURRET_WORK *trt_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(trt_work)->parent_obj;
	Sint32	life	= parent_body->mgr_work->life;
	const GMS_BOSS5_TURRET_SEQ_VUL_SHOT_INFO	*vshot_info	= NULL;
	
	for (Sint32 i = 0; i < GMD_BOSS5_TURRET_SEQ_LIFE_LEVEL_NUM; ++i) {
		if (life <= gm_boss5_trt_seq_vul_shot_info_tbl[i].life_threshold) {
			vshot_info	= &gm_boss5_trt_seq_vul_shot_info_tbl[i];
			break;
		}
	}
	
	if (vshot_info == NULL) {
		MTM_ASSERT(FALSE);
		return 0;
	}
	
	return vshot_info->wait_time;
}

// =======================================================================
// gmBoss5TurretSeqGetVulcanShotNum
/*!
  バルカン連射数取得
  
  @param trt_work	[io]	砲塔ワーク
  
  @return 連射数
 
  @note
  一回の連射で何発発射するかを取得します。
 */
// =======================================================================
Sint32 gmBoss5TurretSeqGetVulcanShotNum(GMS_BOSS5_TURRET_WORK *trt_work)
{
	GMS_BOSS5_BODY_WORK	*parent_body	= (GMS_BOSS5_BODY_WORK*)GMM_BS_OBJ(trt_work)->parent_obj;
	Sint32	life	= parent_body->mgr_work->life;
	const GMS_BOSS5_TURRET_SEQ_VUL_SHOT_INFO	*vshot_info	= NULL;
	
	for (Sint32 i = 0; i < GMD_BOSS5_TURRET_SEQ_LIFE_LEVEL_NUM; ++i) {
		if (life <= gm_boss5_trt_seq_vul_shot_info_tbl[i].life_threshold) {
			vshot_info	= &gm_boss5_trt_seq_vul_shot_info_tbl[i];
			break;
		}
	}
	
	if (vshot_info == NULL) {
		MTM_ASSERT(FALSE);
		return 0;
	}
	
	return vshot_info->shot_num;
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
