// =======================================================================
/*!
	@file	gmGmkSpear.c
	@brief	ギミック 槍＠ゾーン３とゾーン４も

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkSpear.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmPlayer.h"
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"

#include "gmSound.h"

#include "gmGmkSpear.h"

// データヘッダ
#include "common/model/gmk_spear_mdl.hmb"



// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_spear_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
#define	OBJWORK		COMWORK.obj_work
#define	chgf(f)		ppFunc = (f)
// -------------

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// -------------

// ----- Definitions -------------------------------------------（定数の宣言）
// 調整項目
#define	GMD_GMK_SPEAR_DEFAULT_STROKE		((fx32)48.0*FX32_ONE)	//	伸びる長さ標準
#define	GMD_GMK_SPEAR_DEFAULT_STROKE_WAIT	(120)					//	伸び待ち時間標準
#define	GMD_GMK_SPEAR_DEFAULT_SHRINK_WAIT	(120)					//	縮み待ち時間標準
#define	GMD_GMK_SPEAR_STROKE_SPEED			((fx32)8.0*FX32_ONE)	//	伸びる速さ標準
#define	GMD_GMK_SPEAR_BASE_HEIGHT			((fx32)4.0*FX32_ONE)	//	ベースの高さ
#define	GMD_GMK_SPEAR_ROD_HEIGHT			((fx32)5.0*FX32_ONE)	//	棒の長さ



// ---------------------------------------------------------------------------
// オブジェクトタイプ
typedef enum tag_GME_GMK_TYPE{

	GME_GMK_TYPE_SPEAR_U = 0,	// 上伸び
	GME_GMK_TYPE_SPEAR_D,		// 下伸び
	GME_GMK_TYPE_SPEAR_L,		// 左伸び
	GME_GMK_TYPE_SPEAR_R,		// 右伸び

	GME_GMK_TYPE_MAX

}GME_GMK_TYPE;
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// あたり判定矩形テーブル
typedef enum tag_GME_GMK_BWALL_RECT_DATA{

	GME_GMK_RECT_DATA_LEFT = 0,		// 当たり判定矩形　左側
	GME_GMK_RECT_DATA_TOP,			// 当たり判定矩形　
	GME_GMK_RECT_DATA_RIGHT,		// 当たり判定矩形　
	GME_GMK_RECT_DATA_BOTTOM,		// 当たり判定矩形　中心位置からＹ方向のオフセット

	GME_GMK_RECT_DATA_MAX

} GME_GMK_RECT_DATA;
// ---------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	当たり矩形大きさ定義
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define		GMD_GMK_SPEAR_RECT_W		( 8)
#define		GMD_GMK_SPEAR_RECT_H		(16)

#define		GMD_GMK_SPEAR_RECT_X1		(-(GMD_GMK_SPEAR_RECT_W/2))
#define		GMD_GMK_SPEAR_RECT_Y1		(  0)
#define		GMD_GMK_SPEAR_RECT_X2		(GMD_GMK_SPEAR_RECT_X1+GMD_GMK_SPEAR_RECT_W)
#define		GMD_GMK_SPEAR_RECT_Y2		(GMD_GMK_SPEAR_RECT_Y1-GMD_GMK_SPEAR_RECT_H)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const static s16 tbl_gm_gmk_spear_rect[GME_GMK_TYPE_MAX][GME_GMK_RECT_DATA_MAX] = {

	//	槍＠ゾーン３とゾーン４も
	{
		//	土地あたり矩形＠上伸び
		//	X1:Y1
		//       ■
		//	       X2:Y2
		  GMD_GMK_SPEAR_RECT_X1,	//	左
		  GMD_GMK_SPEAR_RECT_Y1,	//	上
		  GMD_GMK_SPEAR_RECT_X2,	//	右
		  GMD_GMK_SPEAR_RECT_Y2,	//	下
	},
	{
		//	土地あたり矩形＠下伸び
		//	X1:Y2
		//       ■
		//	       X2:Y1
		  GMD_GMK_SPEAR_RECT_X1,	//	左
		  GMD_GMK_SPEAR_RECT_Y1,	//	上
		  GMD_GMK_SPEAR_RECT_X2,	//	右
		  GMD_GMK_SPEAR_RECT_Y2,	//	下
	},
	{
		//	土地あたり矩形＠左伸び
		//	Y1:X1
		//       ■
		//	       Y2:X2
		  GMD_GMK_SPEAR_RECT_Y1,	//	左
		  GMD_GMK_SPEAR_RECT_X1,	//	上
		  GMD_GMK_SPEAR_RECT_Y2,	//	右
		  GMD_GMK_SPEAR_RECT_X2,	//	下
	},
	{
		//	土地あたり矩形＠右伸び
		//	Y1:X2
		//       ■
		//	       Y2:X1
		  GMD_GMK_SPEAR_RECT_Y2,	//	左
		  GMD_GMK_SPEAR_RECT_X2,	//	上
		  GMD_GMK_SPEAR_RECT_Y1,	//	右
		  GMD_GMK_SPEAR_RECT_X1,	//	下
	}
};
// ---------------------------------------------------------------------------





// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef struct tag_GMS_GMK_SPEAR_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	GME_GMK_TYPE		obj_type;		//!< 向きオブジェクトタイプ
	u16					vect;			//!< 上向き=0xc000から

	fx32				stroke_spd;				//!< 移動速度
	fx32				timer_dec;				//!< 計算用
	fx32				timer_set_move;			//!< 移動幅
	s16					timer_set_wait_upper;	//!< Hyeneで設定した上死点(伸びきり)待機時間
	s16					timer_set_wait_lower;	//!< Hyeneで設定した下死点(縮みきり)待機時間


}GMS_GMK_SPEAR_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// ---------------------------------------------------------------------------



// ===========================================================================
// gmGmkSpear*
/*!
	ギミック 槍＠ゾーン３とゾーン４も

	@note

 */
// ---------------------------------------------------------------------------
static void gmGmkSpearStay(OBS_OBJECT_WORK *obj_work);		//	最小時待ち
static void gmGmkSpearStroke(OBS_OBJECT_WORK *obj_work);	//	伸びる
static void gmGmkSpearWait(OBS_OBJECT_WORK *obj_work);		//	最大時待ち
static void gmGmkSpearShrink(OBS_OBJECT_WORK *obj_work);	//	縮む
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSpearSyncTimeGet
/*!
	ギミック 槍　同期時間取得
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
u32 gmGmkSpearSyncTimeGet(GMS_GMK_SPEAR_WORK *pwork)
{
	u32 sync_timer = g_gm_main_system.sync_time;
	u32 spear_time;
	u32 move_time;

	move_time = (u32)((pwork->timer_set_move+(pwork->stroke_spd-1))/pwork->stroke_spd);
	spear_time  = move_time*2;
	spear_time += pwork->timer_set_wait_upper;
	spear_time += pwork->timer_set_wait_lower;
	sync_timer %= spear_time;

	return sync_timer;
}
// ---------------------------------------------------------------------------
// gmGmkSpearStart
/*!
	ギミック ギミック 槍＠ゾーン３とゾーン４　サブ初期化

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpearStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPEAR_WORK *pwork = (GMS_GMK_SPEAR_WORK*)obj_work;
	OBS_RECT_WORK *rect_work;

	// 矩形設定
	// 対プレイヤー
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_ENABLE;
	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK];
//	ObjRectAtkSet(rect_work, GMD_OBJ_RECT_ATK_FLAG_NORMALATK, GMD_OBJ_RECT_ATK_POWER_DEFAULT);
//	ObjRectDefSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectWorkZSet(rect_work,
	               tbl_gm_gmk_spear_rect[pwork->obj_type][GME_GMK_RECT_DATA_LEFT],
	               tbl_gm_gmk_spear_rect[pwork->obj_type][GME_GMK_RECT_DATA_TOP],
	               -500,
	               tbl_gm_gmk_spear_rect[pwork->obj_type][GME_GMK_RECT_DATA_RIGHT],
	               tbl_gm_gmk_spear_rect[pwork->obj_type][GME_GMK_RECT_DATA_BOTTOM],
	               +500);
	rect_work->flag |= OBD_RECT_ENABLE;
	rect_work->flag |= OBD_RECT_OUT;			// 連続判定防止用
	obj_work->flag &= ~OBD_OBJECT_NOHIT;


	//	同期をとります
	{
		u32 sync_timer = g_gm_main_system.sync_time;
		u32 spear_time;
		u32 move_time;

		move_time = (u32)((pwork->timer_set_move+(pwork->stroke_spd-1))/pwork->stroke_spd);
		spear_time  = move_time*2;
		spear_time += pwork->timer_set_wait_upper;
		spear_time += pwork->timer_set_wait_lower;

		if( sync_timer <= (u32)pwork->timer_dec )
		{
			pwork->timer_dec -= (sync_timer-1);
			pwork->timer_dec += pwork->timer_set_wait_lower;
			gmGmkSpearStay(obj_work);	//下死点
			return;
		}
		sync_timer -= pwork->timer_dec;


		sync_timer %= spear_time;
		//	下死点待機時間中判定
		if( sync_timer <= (u32)pwork->timer_set_wait_lower )
		{
			pwork->timer_dec = (fx32)(pwork->timer_set_wait_lower-(sync_timer-1));
			gmGmkSpearStay(obj_work);	//下死点
			return;
		}
		sync_timer -= (u32)pwork->timer_set_wait_lower;


		//	ストローク速度を取得
		fx32 spdx = mtMathCos(pwork->vect);
		fx32 spdy = mtMathSin(pwork->vect);
		     spdx = (spdx * pwork->stroke_spd)>>FX32_SHIFT;
		     spdy = (spdy * pwork->stroke_spd)>>FX32_SHIFT;

		//	伸びストローク時間中判定
		if( sync_timer < move_time )
		{
			//	かっちりあわせる
			pwork->timer_dec = pwork->timer_set_move;
			while( sync_timer > 1 )
			{
				pwork->timer_dec -= pwork->stroke_spd;
				obj_work->pos.x += spdx;
				obj_work->pos.y += spdy;
				sync_timer -= 1;
			}
			obj_work->spd.x = spdx;
			obj_work->spd.y = spdy;
			gmGmkSpearStroke(obj_work);
			return;
		}
		sync_timer -= move_time;


		//	上死点位置まで移動
		fx32 movex = (mtMathCos(pwork->vect)*pwork->timer_set_move)>>FX32_SHIFT;
		fx32 movey = (mtMathSin(pwork->vect)*pwork->timer_set_move)>>FX32_SHIFT;
		obj_work->pos.x += movex;
		obj_work->pos.y += movey;
		//	上死点待機時間内判定
		if( sync_timer <= (u32)pwork->timer_set_wait_upper )
		{
			pwork->timer_dec = (fx32)(pwork->timer_set_wait_upper-(sync_timer-1));
			gmGmkSpearWait(obj_work);	//上死点
			return;
		}
		sync_timer -= pwork->timer_set_wait_upper;


//		MTM_ASSERT( sync_timer <= move_time );
		//	縮みストローク時間中判定
		pwork->timer_dec = pwork->timer_set_move;
		while( sync_timer > 1 )
		{
		//	かっちりあわせる
			pwork->timer_dec -= pwork->stroke_spd;
			obj_work->pos.x -= spdx;
			obj_work->pos.y -= spdy;
			sync_timer -= 1;
		}
		obj_work->spd.x = -spdx;
		obj_work->spd.y = -spdy;
		gmGmkSpearShrink(obj_work);
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSpearStay
/*!
	ギミック ギミック 槍＠ゾーン３とゾーン４

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpearStay_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpearStay_200(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearStay(OBS_OBJECT_WORK *obj_work)
{
	obj_work->chgf(gmGmkSpearStay_100);
	gmGmkSpearStay_100(obj_work);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearStay_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPEAR_WORK *pwork = (GMS_GMK_SPEAR_WORK*)obj_work;
	//	初期待ち
	pwork->timer_dec -= 1;
	if( pwork->timer_dec <= 0 )
	{
		obj_work->chgf(gmGmkSpearStay_200);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearStay_200(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPEAR_WORK *pwork = (GMS_GMK_SPEAR_WORK*)obj_work;
	//	伸びる～伸び待ち
	//	初期設定
	obj_work->spd.x = mtMathCos(pwork->vect);
	obj_work->spd.y = mtMathSin(pwork->vect);
	obj_work->spd.x = (obj_work->spd.x * pwork->stroke_spd)>>FX32_SHIFT;
	obj_work->spd.y = (obj_work->spd.y * pwork->stroke_spd)>>FX32_SHIFT;

	pwork->timer_dec = pwork->timer_set_move;
	// SE
	GmSoundPlaySE("Spear");
	gmGmkSpearStroke(obj_work);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpearStroke_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpearStroke_200(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearStroke(OBS_OBJECT_WORK *obj_work)
{
	//	１ストローク先行計算
	obj_work->pos.x += obj_work->spd.x;
	obj_work->pos.y += obj_work->spd.y;
	//	伸びる
	obj_work->chgf(gmGmkSpearStroke_100);
	gmGmkSpearStroke_100(obj_work);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearStroke_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPEAR_WORK *pwork = (GMS_GMK_SPEAR_WORK*)obj_work;
	pwork->timer_dec -= pwork->stroke_spd;
	if( pwork->timer_dec <= 0 )
	{
		if( pwork->timer_dec < 0 )
		{
			obj_work->spd.x = mtMathCos(pwork->vect);
			obj_work->spd.y = mtMathSin(pwork->vect);
			obj_work->spd.x = (obj_work->spd.x * pwork->timer_dec)>>FX32_SHIFT;
			obj_work->spd.y = (obj_work->spd.y * pwork->timer_dec)>>FX32_SHIFT;
		}
		else
		{
			obj_work->spd.x = 0;
			obj_work->spd.y = 0;
		}
		obj_work->chgf(gmGmkSpearStroke_200);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearStroke_200(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPEAR_WORK *pwork = (GMS_GMK_SPEAR_WORK*)obj_work;
	obj_work->spd.x = 0;
	obj_work->spd.y = 0;
#if 0
	pwork->timer_dec = pwork->timer_set_wait_upper;
#else
	// ポーズごとにsync_timerがずれる為、同期あわせる
	u32	move_time = (u32)((pwork->timer_set_move+(pwork->stroke_spd-1))/pwork->stroke_spd);
	u32	sync_timer = gmGmkSpearSyncTimeGet(pwork);
	sync_timer -= (u32)pwork->timer_set_wait_lower;
	sync_timer -= move_time;

	//	上死点待機時間内判定
	if (sync_timer <= (u32)pwork->timer_set_wait_upper) { 
		pwork->timer_dec = (fx32)(pwork->timer_set_wait_upper-(sync_timer-1));
	} else {
		pwork->timer_dec = 0;
	}
#endif
	gmGmkSpearWait(obj_work);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpearWait_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpearWait_200(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearWait(OBS_OBJECT_WORK *obj_work)
{
	//	最大時待ち
	obj_work->chgf(gmGmkSpearWait_100);
	gmGmkSpearWait_100(obj_work);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearWait_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPEAR_WORK *pwork = (GMS_GMK_SPEAR_WORK*)obj_work;
	pwork->timer_dec -= 1;
	if( pwork->timer_dec <= 0 )
	{
		obj_work->chgf(gmGmkSpearWait_200);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearWait_200(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPEAR_WORK *pwork = (GMS_GMK_SPEAR_WORK*)obj_work;
	obj_work->spd.x = -(mtMathCos(pwork->vect)*pwork->stroke_spd)>>FX32_SHIFT;
	obj_work->spd.y = -(mtMathSin(pwork->vect)*pwork->stroke_spd)>>FX32_SHIFT;

	pwork->timer_dec = pwork->timer_set_move;
	gmGmkSpearShrink(obj_work);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpearShrink_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpearShrink_200(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearShrink(OBS_OBJECT_WORK *obj_work)	//	縮む
{
	//	１ストローク先行計算
	obj_work->pos.x += obj_work->spd.x;
	obj_work->pos.y += obj_work->spd.y;
	obj_work->chgf(gmGmkSpearShrink_100);
	gmGmkSpearShrink_100(obj_work);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearShrink_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPEAR_WORK *pwork = (GMS_GMK_SPEAR_WORK*)obj_work;
	pwork->timer_dec -= pwork->stroke_spd;
	if( pwork->timer_dec <= 0 )
	{
		if( pwork->timer_dec < 0 )
		{
			obj_work->spd.x = mtMathCos(pwork->vect);
			obj_work->spd.y = mtMathSin(pwork->vect);
			obj_work->spd.x = -((obj_work->spd.x * pwork->timer_dec)>>FX32_SHIFT);
			obj_work->spd.y = -((obj_work->spd.y * pwork->timer_dec)>>FX32_SHIFT);
		}
		else
		{
			obj_work->spd.x = 0;
			obj_work->spd.y = 0;
		}
		obj_work->chgf(gmGmkSpearShrink_200);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSpearShrink_200(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPEAR_WORK *pwork = (GMS_GMK_SPEAR_WORK*)obj_work;
	obj_work->spd.x = 0;
	obj_work->spd.y = 0;
#if 0
	pwork->timer_dec = (fx32)(pwork->timer_set_wait_lower);
#else
	// ポーズごとにsync_timerがずれる為、同期あわせる
	u32	sync_timer = gmGmkSpearSyncTimeGet(pwork);
	if (sync_timer <= (u32)pwork->timer_set_wait_lower) {
		pwork->timer_dec = (fx32)(pwork->timer_set_wait_lower-(sync_timer-1));
	} else {
		pwork->timer_dec = 0;
	}
#endif
	gmGmkSpearStay(obj_work);
}
// ---------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================





// ==========================================================================
// gmGmkSpearRod*
/*!
	ギミック ギミック 槍＠ゾーン３とゾーン４

	@note
 */
// --------------------------------------------------------------------------
typedef struct tag_GMS_GMK_SPEARPARTS_WORK
{
	GMS_EFFECT_3DNN_WORK eff_work;
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。

	GME_GMK_TYPE		obj_type;
	fx32				fulcrum;
	fx32				*connect;

}GMS_GMK_SPEARPARTS_WORK;
#define	OBJ_3D		eff_work.obj_3d
#define	COMWORK		eff_work.efct_com
// ---------------------------------------------------------------------------
static void gmGmkSpearRod(OBS_OBJECT_WORK *obj_work);
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
// gmGmkSpearRod*
/*!
	ギミック ギミック 槍＠ゾーン３とゾーン４

	@note
		槍の先端にくっついて伸び縮みします。
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpearRod(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPEARPARTS_WORK *pwork = (GMS_GMK_SPEARPARTS_WORK*)obj_work;
	fx32 stroke;
	stroke = MTM_MATH_ABS(*pwork->connect - pwork->fulcrum);
	stroke /= (GMD_GMK_SPEAR_ROD_HEIGHT>>FX32_SHIFT);
	obj_work->scale.y = stroke;
}
// --------------------------------------------------------------------------




// ---------------------------------------------------------------------------
// gmGmkSpear_CreateParts
/*!
	ギミック 槍　部品生成

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpear_CreateParts(GMS_GMK_SPEAR_WORK *pwork)
{
	GMS_GMK_SPEARPARTS_WORK *parts;
	OBS_OBJECT_WORK *parent_obj;
	OBS_OBJECT_WORK *obj_work;

	parent_obj = (OBS_OBJECT_WORK*)pwork;
	//	土台
	{
		obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_SPEARPARTS_WORK),
		                                  NULL,
		                                  0,
		                                 "Gmk_SpearBase");
		parts =(GMS_GMK_SPEARPARTS_WORK*)obj_work;
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_spear_obj_3d_list[IDB_GMK_SPEAR_MDL_GMK_SPEAR_BASE_ZNO],
		                             &parts->eff_work.obj_3d);

		obj_work->parent_obj = parent_obj;

		obj_work->pos.x = parent_obj->pos.x;
		obj_work->pos.y = parent_obj->pos.y;
		obj_work->pos.z = parent_obj->pos.z;
		switch (pwork->obj_type)
		{
			case GME_GMK_TYPE_SPEAR_U:
				obj_work->pos.y += GMD_GMK_SPEAR_BASE_HEIGHT;
				break;
			case GME_GMK_TYPE_SPEAR_D:
				obj_work->pos.y -= GMD_GMK_SPEAR_BASE_HEIGHT;
				break;
			case GME_GMK_TYPE_SPEAR_L:
				obj_work->pos.x += GMD_GMK_SPEAR_BASE_HEIGHT;
				break;
			case GME_GMK_TYPE_SPEAR_R:
				obj_work->pos.x -= GMD_GMK_SPEAR_BASE_HEIGHT;
				break;
			case GME_GMK_TYPE_MAX:
				break;
		}
		obj_work->dir.z = parent_obj->dir.z;
		// フラグ
		obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;		// 佇む
		obj_work->move_flag |= OBD_MOVE_NOCOL;			// 移動無し 地形あたりチェック無し
		obj_work->disp_flag &= ~OBD_DISP_NODIR;
		obj_work->flag |= OBD_OBJECT_NOHIT;				// 矩形あたり無し◆
		obj_work->chgf(NULL);		//	ジョブなし

	}
	//	槍身
	{
		obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_SPEARPARTS_WORK),
		                                  NULL,
		                                  0,
		                                 "Gmk_SpearRod");
		parts =(GMS_GMK_SPEARPARTS_WORK*)obj_work;
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_spear_obj_3d_list[IDB_GMK_SPEAR_MDL_GMK_SPEAR_BAR_ZNO],
		                             &parts->eff_work.obj_3d);

		obj_work->parent_obj = parent_obj;
		obj_work->parent_ofst.x = 0;
		obj_work->parent_ofst.y = 0;
		obj_work->parent_ofst.z = -1;
		obj_work->dir.z = parent_obj->dir.z;
		// フラグ
		obj_work->flag |= OBD_OBJECT_PARENT_FIX;						// くっつく
		obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
		obj_work->disp_flag &= ~OBD_DISP_NODIR;
		obj_work->flag |= OBD_OBJECT_NOHIT;								// 矩形あたり無し◆

		switch (pwork->obj_type)
		{
			case GME_GMK_TYPE_SPEAR_U:
				parts->connect = &parent_obj->pos.y;
				break;
			case GME_GMK_TYPE_SPEAR_D:
				parts->connect = &parent_obj->pos.y;
				break;
			case GME_GMK_TYPE_SPEAR_L:
				parts->connect = &parent_obj->pos.x;
				break;
			case GME_GMK_TYPE_SPEAR_R:
				parts->connect = &parent_obj->pos.x;
				break;
			case GME_GMK_TYPE_MAX:
				break;
		}
		parts->obj_type = pwork->obj_type;
		parts->fulcrum = *parts->connect;
		obj_work->chgf(gmGmkSpearRod);		//	伸び縮み
	}
}
// --------------------------------------------------------------------------
// ==========================================================================





// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkSpear?Init
/*!
 *	ギミック 槍＠ゾーン３とゾーン４も 初期化関数
 *	GmGmkSpearUInit 上向き
 *	GmGmkSpearDInit 下向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
static OBS_OBJECT_WORK* gmGmkSpearInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_SPEAR_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_SPEAR_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_SPEAR_WORK), "Gmk_Spear");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_spear_obj_3d_list[IDB_GMK_SPEAR_MDL_GMK_SPEAR_ZNO],
	                             &gmk_work->obj_3d);
	// 優先設定
	obj_work->pos.z = GMD_OBJ_ENEMY_POS_Z;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し

	//	標準のデータ
	pwork->timer_set_move = GMD_GMK_SPEAR_DEFAULT_STROKE;
	pwork->stroke_spd = GMD_GMK_SPEAR_STROKE_SPEED;
	pwork->timer_set_wait_upper = GMD_GMK_SPEAR_DEFAULT_STROKE_WAIT;
	pwork->timer_set_wait_lower = GMD_GMK_SPEAR_DEFAULT_SHRINK_WAIT;

	//	設定の反映
	pwork->timer_dec = eve_rec->height;				//	初期ウェイト時間
	if( eve_rec->flag & 0x01F )
	{
		fx32 add_spd;
		if( !(eve_rec->flag & 0x010) )
			add_spd = (fx32)((eve_rec->flag&0x0f)<<(FX32_SHIFT-2));
		else
		{
			add_spd = (fx32)((-(eve_rec->flag&0x0f))<<(FX32_SHIFT-2));
			if( add_spd == 0 )
				add_spd = -4*FX32_ONE;
		}
		pwork->stroke_spd += add_spd;
	}
	return obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSpearUInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	上向きに伸びる
	GMS_GMK_SPEAR_WORK	*pwork;

	pwork = (GMS_GMK_SPEAR_WORK*)gmGmkSpearInit(eve_rec, pos_x, pos_y, type);
	pwork->obj_type = GME_GMK_TYPE_SPEAR_U;
	pwork->vect = 0xc000;
	pwork->gmk_work.ene_com.obj_work.dir.z = 0x0000;

	//	設定の反映
	if( eve_rec->left > 0 )
		pwork->timer_set_wait_upper = eve_rec->left;	//	Hyenaで設定する伸び状態待ち時間
	if( eve_rec->width > 0 )
		pwork->timer_set_wait_lower = eve_rec->width;	//	Hyenaで設定する縮み状態待ち時間
	if( eve_rec->top < 0 )
		pwork->timer_set_move = -(fx32)(eve_rec->top<<FX32_SHIFT);

	//	土台と刀身の作成
	gmGmkSpear_CreateParts(pwork);

	pwork->gmk_work.ene_com.obj_work.chgf(gmGmkSpearStart);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSpearDInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	下向きに伸びる
	GMS_GMK_SPEAR_WORK	*pwork;

	pwork = (GMS_GMK_SPEAR_WORK*)gmGmkSpearInit(eve_rec, pos_x, pos_y, type);
	pwork->obj_type = GME_GMK_TYPE_SPEAR_D;
	pwork->gmk_work.ene_com.obj_work.dir.z = 0x8000;
	pwork->vect = 0x4000;

	//	設定の反映
	if( eve_rec->left > 0 )
		pwork->timer_set_wait_upper = eve_rec->left;	//	Hyenaで設定する伸び状態待ち時間
	if( eve_rec->width > 0 )
		pwork->timer_set_wait_lower = eve_rec->width;	//	
	if( eve_rec->top > 0 )
		pwork->timer_set_move = (fx32)(eve_rec->top<<FX32_SHIFT);

	//	土台と刀身の作成
	gmGmkSpear_CreateParts(pwork);

	pwork->gmk_work.ene_com.obj_work.chgf(gmGmkSpearStart);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSpearLInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	左向きに伸びる
	GMS_GMK_SPEAR_WORK	*pwork;

	pwork = (GMS_GMK_SPEAR_WORK*)gmGmkSpearInit(eve_rec, pos_x, pos_y, type);
	pwork->obj_type = GME_GMK_TYPE_SPEAR_L;
	pwork->gmk_work.ene_com.obj_work.dir.z = 0xc000;
	pwork->vect = 0x8000;

	//	設定の反映
	if( eve_rec->top > 0 )
		pwork->timer_set_wait_upper = eve_rec->top;	//	Hyenaで設定する伸び状態待ち時間
	if( eve_rec->width > 0 )
		pwork->timer_set_wait_lower = eve_rec->width;	//	
	if( eve_rec->left < 0 )
		pwork->timer_set_move = -(fx32)(eve_rec->left<<FX32_SHIFT);

	//	土台と刀身の作成
	gmGmkSpear_CreateParts(pwork);

	pwork->gmk_work.ene_com.obj_work.chgf(gmGmkSpearStart);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSpearRInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	//	右向きに伸びる
	GMS_GMK_SPEAR_WORK	*pwork;

	pwork = (GMS_GMK_SPEAR_WORK*)gmGmkSpearInit(eve_rec, pos_x, pos_y, type);
	pwork->obj_type = GME_GMK_TYPE_SPEAR_R;
	pwork->gmk_work.ene_com.obj_work.dir.z = 0x4000;
	pwork->vect = 0x0000;

	//	設定の反映
	if( eve_rec->top > 0 )
		pwork->timer_set_wait_upper = eve_rec->top;	//	Hyenaで設定する伸び状態待ち時間
	if( eve_rec->width > 0 )
		pwork->timer_set_wait_lower = eve_rec->width;	//	
	if( eve_rec->left > 0 )
		pwork->timer_set_move = (fx32)(eve_rec->left<<FX32_SHIFT);

	//	土台と刀身の作成
	gmGmkSpear_CreateParts(pwork);

	pwork->gmk_work.ene_com.obj_work.chgf(gmGmkSpearStart);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================



// ===========================================================================
// GmGmkSpearBuild
/*!
	ギミック 槍＠ゾーン３とゾーン４も データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkSpearBuild(void)
{
	gm_gmk_spear_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPEAR_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPEAR_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkSpearFlush
/*!
	ギミック 槍＠ゾーン３とゾーン４も データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkSpearFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPEAR_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_spear_obj_3d_list, amb->file_num);
}
// ===========================================================================
