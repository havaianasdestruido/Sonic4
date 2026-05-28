// =======================================================================
/*!
	@file	gmGmkCannon.c
	@brief	ギミック 大砲＠ゾーン２

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkCannon.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmPlySeq.h"
#include "gmPlySpec.h"
#include "gmPlayer.h"
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmSound.h"
#include "gmPadVib.h"

#include "gmEffectCmn.h"

#include "gmGmkCannon.h"

// データヘッダ
#include "common/model/gmk_cannon_mdl.hmb"



// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void gmGmkCannonFieldColOn(OBS_OBJECT_WORK *obj_work);
static void gmGmkCannonFieldColOff(OBS_OBJECT_WORK *obj_work);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_cannon_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
#define	OBJWORK		COMWORK.obj_work
#define	chgf(f)		ppFunc = (f)
// -------------

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// -------------

// ----- Definitions -------------------------------------------（定数の宣言）
// 調整項目
#define	GMD_GMK_CANNON_PLAYER_SHOOT_SPD		GMD_PL_DEF_MAX_SPD


// ---------------------------------------------------------------------------
// あたり判定矩形テーブル
typedef enum tag_GME_GMK_RECT_OBJ{

	GME_GMK_RECT_OBJ_CANNON = 0,
	GME_GMK_RECT_OBJ_CANNON_COL,
	GME_GMK_RECT_OBJ_BASE_COL,

	GME_GMK_RECT_OBJ_MAX

} GME_GMK_RECT_OBJ;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef enum tag_GME_GMK_RECT_DATA{

	GME_GMK_RECT_DATA_LEFT = 0,			// 当たり判定矩形　左側
	GME_GMK_RECT_DATA_TOP,				// 当たり判定矩形　
	GME_GMK_RECT_DATA_RIGHT,			// 当たり判定矩形　
	GME_GMK_RECT_DATA_BOTTOM,			// 当たり判定矩形　中心位置からＹ方向のオフセット

	GME_GMK_RECT_DATA_MAX

} GME_GMK_RECT_DATA;
// ---------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	当たり矩形大きさ定義
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	GME_GMK_CANNON_STARTUP_RECT_X		(-12)
#define	GME_GMK_CANNON_STARTUP_RECT_W		( 24)
#define	GME_GMK_CANNON_STARTUP_RECT_Y		(-38)
#define	GME_GMK_CANNON_STARTUP_RECT_H		( 32)

#define	GME_GMK_CANNON_COL_BODY_RECT_X		(-12)	//(-15)
#define	GME_GMK_CANNON_COL_BODY_RECT_W		(24)	//( 30)	// diff_data使用のため8dotごと
#define	GME_GMK_CANNON_COL_BODY_RECT_Y		(-30)	//(-30)
#define	GME_GMK_CANNON_COL_BODY_RECT_H		(56)	//( 58)	// diff_data使用のため8dotごと

#define	GME_GMK_CANNON_COL_BASE_RECT_X		(-40)
#define	GME_GMK_CANNON_COL_BASE_RECT_W		( 80)
#define	GME_GMK_CANNON_COL_BASE_RECT_Y		(-56)
#define	GME_GMK_CANNON_COL_BASE_RECT_H		( 56)
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// 角度定義
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define		GMD_GMK_CANNON_ANGLE_PARTS		(90/15)
#define		GMD_GMK_CANNON_ANGLE_1NOTCH		(0x0aaa)
#if _IPHONE
#define		GMD_GMK_CANNON_ANGLE_1NOTCH_CHK	(0x0aaa) // 補正が必要なら設定する
#endif // _IPHONE
#define		GMD_GMK_CANNON_ANGLE_RIGHT		(GMD_GMK_CANNON_ANGLE_1NOTCH*6)
#define		GMD_GMK_CANNON_ANGLE_LEFT		(-(GMD_GMK_CANNON_ANGLE_1NOTCH*6))
#define		GMD_GMK_CANNON_ANGLE_ROT_SPD	(GMD_GMK_CANNON_ANGLE_1NOTCH/8 + 1)

#define		GMD_GMK_CANNON_ANGLE_270		(GMD_GMK_CANNON_ANGLE_0-GMD_GMK_CANNON_ANGLE_1NOTCH*6)
#define		GMD_GMK_CANNON_ANGLE_285		(GMD_GMK_CANNON_ANGLE_0-GMD_GMK_CANNON_ANGLE_1NOTCH*5)
#define		GMD_GMK_CANNON_ANGLE_300		(GMD_GMK_CANNON_ANGLE_0-GMD_GMK_CANNON_ANGLE_1NOTCH*4)
#define		GMD_GMK_CANNON_ANGLE_315		(GMD_GMK_CANNON_ANGLE_0-GMD_GMK_CANNON_ANGLE_1NOTCH*3)
#define		GMD_GMK_CANNON_ANGLE_330		(GMD_GMK_CANNON_ANGLE_0-GMD_GMK_CANNON_ANGLE_1NOTCH*2)
#define		GMD_GMK_CANNON_ANGLE_345		(GMD_GMK_CANNON_ANGLE_0-GMD_GMK_CANNON_ANGLE_1NOTCH*1)
#define		GMD_GMK_CANNON_ANGLE_0			(GMD_GMK_CANNON_ANGLE_1NOTCH*0)
#define		GMD_GMK_CANNON_ANGLE_15			(GMD_GMK_CANNON_ANGLE_1NOTCH*1)
#define		GMD_GMK_CANNON_ANGLE_30			(GMD_GMK_CANNON_ANGLE_1NOTCH*2)
#define		GMD_GMK_CANNON_ANGLE_45			(GMD_GMK_CANNON_ANGLE_1NOTCH*3)
#define		GMD_GMK_CANNON_ANGLE_60			(GMD_GMK_CANNON_ANGLE_1NOTCH*4)
#define		GMD_GMK_CANNON_ANGLE_75			(GMD_GMK_CANNON_ANGLE_1NOTCH*5)
#define		GMD_GMK_CANNON_ANGLE_90			(GMD_GMK_CANNON_ANGLE_1NOTCH*6)
// ---------------------------------------------------------------------------



// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef struct tag_GMS_GMK_CANNON_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。

	GMS_PLAYER_WORK		*ply_work;		//!< 玉装填！
	BOOL				hitpass;		//!< 1/60前の当たり判定
	s16					shoot_after;	//!< 射出後のウェイト

	s16					angle_set;		//!< 向かう角度fx32
	s16					angle_now;		//!fx32型のアングル

	s32					cannon_power;	//! 吹き上げ力


}GMS_GMK_CANNON_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// ---------------------------------------------------------------------------



// ===========================================================================
// gmGmkCannon*
/*!
	ギミック 大砲＠ゾーン２

	@note

 */
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkCannonFieldColOn
//	ギミック大砲 地形コリジョンON
// ---------------------------------------------------------------------------
void gmGmkCannonFieldColOn(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_CANNON_WORK *pwork = (GMS_GMK_CANNON_WORK*)obj_work;
	//	土地あたり矩形設定
	pwork->COMWORK.col_work.obj_col.obj       = obj_work;
	// 地形矩形設定
	pwork->COMWORK.col_work.obj_col.diff_data	= (s8*)g_gm_default_col;
	pwork->COMWORK.col_work.obj_col.width  = GME_GMK_CANNON_COL_BODY_RECT_W;
	pwork->COMWORK.col_work.obj_col.height = GME_GMK_CANNON_COL_BODY_RECT_H;
	pwork->COMWORK.col_work.obj_col.ofst_x = GME_GMK_CANNON_COL_BODY_RECT_X;
	pwork->COMWORK.col_work.obj_col.ofst_y = GME_GMK_CANNON_COL_BODY_RECT_Y;
	pwork->COMWORK.col_work.obj_col.flag  |= (OBD_COLOBJ_NOFREE_DIFF_DATA		// diff_dataを開放しない
											| OBD_COLOBJ_NODIR					// 自dir を使用しない
											| OBD_COLOBJ_NODIR_PARENT);			// 親dir を使用しない
}

// ---------------------------------------------------------------------------
// gmGmkCannonFieldColOff
//	ギミック大砲 地形コリジョンOFF
// ---------------------------------------------------------------------------
void gmGmkCannonFieldColOff(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_CANNON_WORK *pwork = (GMS_GMK_CANNON_WORK*)obj_work;
	pwork->COMWORK.col_work.obj_col.obj       = NULL;
}

// ---------------------------------------------------------------------------
// gmGmkCannon_*
/*!
	ギミック 大砲＠ゾーン２

	@note
		大砲オブジェの共通関数
		OBS_OBJECT_WORK を引数としません
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkCannon_CannonTurn(GMS_GMK_CANNON_WORK *pwork)
{
	//	砲台回転
	if( pwork->angle_set > pwork->angle_now )
	{
		//	右回転 dir + 方向
		pwork->angle_now += GMD_GMK_CANNON_ANGLE_ROT_SPD;
		if( pwork->angle_now > pwork->angle_set  )
			pwork->angle_now = pwork->angle_set;
	}
	else
	{
		//	左回転 dir - 方向
		pwork->angle_now -= GMD_GMK_CANNON_ANGLE_ROT_SPD;
		if( pwork->angle_now < pwork->angle_set )
			pwork->angle_now = pwork->angle_set;
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
static void gmGmkCannonStay(OBS_OBJECT_WORK *obj_work);
static void gmGmkCannonReady(OBS_OBJECT_WORK *obj_work);
static void gmGmkCannonShoot(OBS_OBJECT_WORK *obj_work);
static void gmGmkCannonHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkCannonStart
/*!
	ギミック 大砲＠ゾーン２

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkCannonStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_CANNON_WORK *pwork = (GMS_GMK_CANNON_WORK*)obj_work;
	OBS_RECT_WORK *rect_work;

	// 矩形設定

	//	土地あたり矩形設定
	gmGmkCannonFieldColOn(obj_work);

	// 対プレイヤー
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = gmGmkCannonHit;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 被破壊矩形設定
	ObjRectWorkSet(rect_work,
	               GME_GMK_CANNON_STARTUP_RECT_X,
	               GME_GMK_CANNON_STARTUP_RECT_Y,
	               GME_GMK_CANNON_STARTUP_RECT_X+GME_GMK_CANNON_STARTUP_RECT_W,
	               GME_GMK_CANNON_STARTUP_RECT_Y+GME_GMK_CANNON_STARTUP_RECT_H );
	obj_work->flag &= ~OBD_OBJECT_NOHIT;

	pwork->ply_work = NULL;		//	プレイヤー装填完了ワーク
	pwork->angle_set = GMD_GMK_CANNON_ANGLE_0;
	pwork->angle_now = GMD_GMK_CANNON_ANGLE_0;
	pwork->COMWORK.enemy_flag &= ~GMD_ENEMY_FLAG_NOHOMING;	// ホーミング対象に復帰

	obj_work->chgf(gmGmkCannonStay);
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkCannonStay
/*!
	ギミック 大砲＠ゾーン２

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkCannonStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_CANNON_WORK *pwork = (GMS_GMK_CANNON_WORK*)obj_work;
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	// ソニックがホーミング中は大砲の地形コリジョンなし
	if (pwork->COMWORK.col_work.obj_col.obj) {
		if (ply_work->act_state == GME_PLY_ACT_STATE_HOMING) {
			gmGmkCannonFieldColOff(obj_work);
		}
	} else {
		if (ply_work->act_state != GME_PLY_ACT_STATE_HOMING) {
			gmGmkCannonFieldColOn(obj_work);
		}
	}

	// 矩形にhitしたら発射準備に移る
	if( pwork->ply_work )
	{
		OBS_RECT_WORK *rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
		rect_work->flag &= ~OBD_RECT_ENABLE;						// body collision disable

		if (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_CANNON) {
			// ソニックが大砲以外のステートに変更されたら大砲を準備状態に戻す
			gmGmkCannonStart(obj_work);
			return;
		}

		if( obj_work->pos.y <= pwork->ply_work->obj_work.pos.y ) {
			obj_work->chgf(gmGmkCannonReady);
		}
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkCannonReady
/*!
	ギミック 大砲＠ゾーン２

	@note
		コントローラーで角度制御
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	GMD_GMK_CANNON_ANGLE_TBL_SIZE	sizeof(tbl_cannon_angle)/sizeof(u16)
static s16 gmGmkCannon_GetAngle(u16 key)
{
	if( key & PAD_KEY_RIGHT )
		return  GMD_GMK_CANNON_ANGLE_1NOTCH;

	if( key & PAD_KEY_LEFT )
		return -GMD_GMK_CANNON_ANGLE_1NOTCH;

	return 0;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkCannonReady(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_CANNON_WORK *pwork = (GMS_GMK_CANNON_WORK*)obj_work;
	s16	angle_chk1 = pwork->angle_set;
	s16	angle_chk2 = pwork->angle_now;
	if( pwork->angle_set == pwork->angle_now ) {
		// 回転入力更新チェック
#if _IPHONE
		// 操作方法により処理を変更
		if (!(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC)) {
			Angle32 angle_set   = (Angle32)(_am_iphone_accel_data.sensor.x * (float)0x4000) * 3;
			if (angle_set > 0x8000) {
				angle_set = 0x8000;
			}
			else if (angle_set < -0x8000)  {
				angle_set = -0x8000;
			}
			angle_set /= 2;
			// 補正
			// 次を超えないと増やさない
			if (angle_set >= pwork->angle_now + GMD_GMK_CANNON_ANGLE_1NOTCH_CHK) {
				angle_set = pwork->angle_now + GMD_GMK_CANNON_ANGLE_1NOTCH;
			}
			else if (pwork->angle_now == GMD_GMK_CANNON_ANGLE_75 && angle_set >= GMD_GMK_CANNON_ANGLE_90) {
				angle_set = GMD_GMK_CANNON_ANGLE_90;
			}
			else if (angle_set <= pwork->angle_now - GMD_GMK_CANNON_ANGLE_1NOTCH_CHK) {
				angle_set = pwork->angle_now - GMD_GMK_CANNON_ANGLE_1NOTCH;
			}
			else if (pwork->angle_now == GMD_GMK_CANNON_ANGLE_285 && angle_set <= GMD_GMK_CANNON_ANGLE_270) {
				angle_set = GMD_GMK_CANNON_ANGLE_270;
			}
			else {
				angle_set = pwork->angle_now;
			}
			pwork->angle_set = (s16)angle_set;
		} else
#endif // _IPHONE
		{
			pwork->angle_set += gmGmkCannon_GetAngle((u16)(pwork->ply_work->key_on));
			if( pwork->angle_set > GMD_GMK_CANNON_ANGLE_90 && (u16)pwork->angle_set < (u16)GMD_GMK_CANNON_ANGLE_270 )
				pwork->angle_set = GMD_GMK_CANNON_ANGLE_90;
			if( pwork->angle_set < GMD_GMK_CANNON_ANGLE_270 && (u16)pwork->angle_set > (u16)GMD_GMK_CANNON_ANGLE_90 )
				pwork->angle_set = GMD_GMK_CANNON_ANGLE_270;
		}

		// 回転SE(1ノッチ毎に1コール)
		if (angle_chk1 != pwork->angle_set)
			GmSoundPlaySE("Cannon1");
	}

	if( pwork->angle_set != pwork->angle_now )
	{
		// 大砲実回転チェック
		gmGmkCannon_CannonTurn(pwork);
		obj_work->dir.z = (u16)pwork->angle_now;
	}
	
	// 回転SE(回転し始める時に１コール)
	if (  (pwork->angle_set == pwork->angle_now)			// 指定角度に一致して
		&&(angle_chk2 == pwork->angle_now)					// 静止するまで発射不可
		&&(GmPlayerKeyCheckJumpKeyPush(pwork->ply_work)) )	// 発射入力
	{
		// 発射！
		//	エフェクト生成
		GMS_EFFECT_3DES_WORK *pefct = 
		GmEfctCmnEsCreate( &pwork->ply_work->obj_work, GME_EFCT_CMN_IDX_CANON);
		pefct->efct_com.obj_work.dir.z = obj_work->dir.z;
		pefct->efct_com.obj_work.pos.x += (fx32)(mtMathSin(obj_work->dir.z)*32);
		pefct->efct_com.obj_work.pos.y -= (fx32)(mtMathCos(obj_work->dir.z)*32);
		//	位相が1/4ずれるのでSin←→Cosです。

		GmPlySeqInitCannonShoot(pwork->ply_work,
		                       ((fx32)mtMathCos(obj_work->dir.z-0x4000)*pwork->cannon_power),
                               ((fx32)mtMathSin(obj_work->dir.z-0x4000)*pwork->cannon_power));

		gmGmkCannonFieldColOff(obj_work);						// 地形コリジョンOFF
		pwork->COMWORK.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;	// ホーミング対象から除外
		pwork->shoot_after = 0;
		obj_work->chgf(gmGmkCannonShoot);
		gmGmkCannonShoot(obj_work);
		// SE
		GmSoundPlaySE("Cannon2");
		// 振動
		GMM_PAD_VIB_SMALL();
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkCannonShoot
/*!
	ギミック 大砲＠ゾーン２

	@note
		発射
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkCannonShootEnd(OBS_OBJECT_WORK *obj_work);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkCannonShoot(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_CANNON_WORK *pwork = (GMS_GMK_CANNON_WORK*)obj_work;

	pwork->shoot_after++;
	if( pwork->shoot_after == 16 ) {
		// 地形コリジョン復帰
		gmGmkCannonFieldColOn(obj_work);					// 地形コリジョンON
		if (pwork->angle_now == GMD_GMK_CANNON_ANGLE_0) {
			// 現在アングルが真上なら即次装填
			pwork->ply_work = NULL;
			gmGmkCannonStart(obj_work);
			return;
		}
	}
	if( pwork->shoot_after > 32 ) {
		// 回転元に戻す
		pwork->ply_work = NULL;
		pwork->angle_set = GMD_GMK_CANNON_ANGLE_0;
		obj_work->chgf(gmGmkCannonShootEnd);
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkCannonShootEnd(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_CANNON_WORK *pwork = (GMS_GMK_CANNON_WORK*)obj_work;

	gmGmkCannon_CannonTurn(pwork);
	obj_work->dir.z = (u16)pwork->angle_now;
	if( pwork->angle_now == pwork->angle_set )
		gmGmkCannonStart(obj_work);
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkCannonHit
/*!
	ギミック キャノン プレイヤー接触

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkCannonHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	GMS_GMK_CANNON_WORK *pwork = (GMS_GMK_CANNON_WORK*)obj_work;
	//	一応プレイヤーが当たったかどうかを確認
	pwork->hitpass = NULL;
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] )
	{
		if( pwork->ply_work != ply_work	)	//	未装填
		{
			//	上チェック
			s16 t;
			t = (s16)((obj_work->pos.y>>FX32_SHIFT)+mine_rect->rect.top);
			
			if (  (  (ply_work->obj_work.pos.y>>FX32_SHIFT < t)			// 通常はコリジョンヒット且つ上側
				   &&( ply_work->obj_work.move.y >= 0 ) )				// 　＋プレイヤー下降中のみ
				||(ply_work->act_state == GME_PLY_ACT_STATE_HOMING) )	// ホーミングならコリジョンヒットで基準満たしたとみなす
			{
				// 移動速度が速ければ数回に分けてチェック の準備
				s16	width, pmove;
				s16	cnt = 1;
				width = (s16)(MTM_MATH_ABS(mine_rect->rect.left - match_rect->rect.left)
							+ MTM_MATH_ABS(mine_rect->rect.right - match_rect->rect.right));
				pmove = (s16)(MTM_MATH_ABS(ply_work->obj_work.move.x >> FX32_SHIFT));
				if (pmove) {
					cnt = (s16)(pmove / width + 1);
				}
				if (ply_work->obj_work.move.x < 0) {
					width = (s16)(0 - width);
				}

				//	左右チェック
				s16	l,r;	// Left,Right
				s16 px;
				l = (s16)((obj_work->pos.x>>FX32_SHIFT) + mine_rect->rect.left
				                                        - match_rect->rect.left);	//	大砲の左側を取得
				r = (s16)((obj_work->pos.x>>FX32_SHIFT) + mine_rect->rect.right
				                                        - match_rect->rect.right);	//	大砲の右側を取得
				px = (s16)(ply_work->obj_work.pos.x>>FX32_SHIFT);

				// 移動速度が速ければ数回に分けてチェック の準備
				for (;cnt;cnt--) {
				//	左右完全範囲チェック
					if( px >= l && px <= r )
					{
						pwork->ply_work = ply_work;
						GmPlySeqInitCannon(ply_work,(GMS_ENEMY_COM_WORK*)obj_work);
						// SE
						GmSoundPlaySE("Cannon3");
						break;
					}
					px = (s16)(px + width);
				}
			}
		}
		//	プレイヤー装填中は処理なし
		pwork->hitpass = TRUE;	//	1/60前の当たり判定
	}
	// ヒットしなかった事にする
	mine_rect->flag &= ~(OBD_RECT_DAMAGE | OBD_RECT_FRAMEHIT | OBD_RECT_FRAMEOUT);

}
// ---------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================





// ==========================================================================
// gmGmkCannonBase*
/*!
	ギミック 大砲＠ゾーン２

	@note
 */
// --------------------------------------------------------------------------
typedef struct tag_GMS_GMK_CANNONPARTS_WORK
{
	GMS_EFFECT_3DNN_WORK eff_work;
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。


}GMS_GMK_CANNONPARTS_WORK;
#define	OBJ_3D		eff_work.obj_3d
#define	COMWORK		eff_work.efct_com
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkCannon_CreateParts
/*!
	ギミック 大砲　部品生成

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkCannon_CreateParts(GMS_GMK_CANNON_WORK *pwork)
{
	GMS_GMK_CANNONPARTS_WORK *parts;
	OBS_OBJECT_WORK *parent_obj;
	OBS_OBJECT_WORK *obj_work;

	parent_obj = (OBS_OBJECT_WORK*)pwork;
	//	土台
	{
		obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_CANNONPARTS_WORK),
		                                  NULL,
		                                  0,
		                                 "Gmk_CannonBase");
		parts =(GMS_GMK_CANNONPARTS_WORK*)obj_work;
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_cannon_obj_3d_list[IDB_GMK_CANNON_MDL_GMK_CANNON_BASE_ZNO],
		                             &parts->eff_work.obj_3d);

		obj_work->parent_obj = parent_obj;

		obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;						// 佇む
		obj_work->pos.x = parent_obj->pos.x;
		obj_work->pos.y = parent_obj->pos.y+30*FX32_ONE;
#if _IPHONE
		obj_work->pos.z = parent_obj->pos.z+30*FX32_ONE;
#else
		obj_work->pos.z = parent_obj->pos.z;
#endif // _IPHONE
		obj_work->dir.y = parent_obj->dir.y;
		// フラグ
		obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
		obj_work->disp_flag &= ~OBD_DISP_NODIR;
		obj_work->flag |= OBD_OBJECT_NOHIT;								// 矩形あたり無し◆
		obj_work->chgf(NULL);		//	ジョブなし
	}
}
// --------------------------------------------------------------------------
// ==========================================================================





// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkCannonInit
/*!
 *	ギミック 大砲＠ゾーン２ 初期化関数
 *	GmGmkCannonInit
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkCannonInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_CANNON_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_CANNON_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_CANNON_WORK), "Gmk_Cannon");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_cannon_obj_3d_list[IDB_GMK_CANNON_MDL_GMK_CANNON_ZNO],
	                             &gmk_work->obj_3d);
	// 優先設定
//	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE;
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_FRONT;

	// 位置ズレ修正
	obj_work->pos.y -= 4 * FX32_ONE + FX32_ONE / 2;

	obj_work->dir.y = 0x8000;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し

	//	パラメーター設定
	if( eve_rec->width != 0 )
		pwork->cannon_power = eve_rec->width;
	else
		pwork->cannon_power = GMD_GMK_CANNON_PLAYER_SHOOT_SPD;

	//	土台の作成
	gmGmkCannon_CreateParts(pwork);
	//	開始
	gmGmkCannonStart(obj_work);

	return obj_work;
}
// ==========================================================================



// ===========================================================================
// GmGmkCannonBuild
/*!
	ギミック 大砲＠ゾーン２ データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkCannonBuild(void)
{
	gm_gmk_cannon_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_CANNON_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_CANNON_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkCannonFlush
/*!
	ギミック 大砲＠ゾーン２ データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkCannonFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_CANNON_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_cannon_obj_3d_list, amb->file_num);
}
// ===========================================================================
