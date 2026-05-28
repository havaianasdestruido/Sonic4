// =======================================================================
/*!
	@file	gmGmkPiston.c
	@brief	ギミック ピストン＠ゾーン４工場

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkPiston.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmEffectCmn.h"
#include "gmSound.h"

#include "gmGmkPiston.h"

// データヘッダ
#include "common/model/gmk_piston_mdl.hmb"



// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）


// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_piston_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
#define	OBJWORK		COMWORK.obj_work
#define	chgf(f)		ppFunc = (f)
// -------------

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// -------------

// ----- Definitions -------------------------------------------（定数の宣言）
// ---------------------------------------------------------------------------
// 調整項目
#define	GMD_GMK_PISTON_STROKE_SPEED		((fx32)4.0*FX32_ONE)
#define	GMD_GMK_PISTON_STROKE			((fx32)128.0*FX32_ONE)

#define	GMD_GMK_PISTON_OFF_Z			(64*FX32_ONE)


// ---------------------------------------------------------------------------
// オブジェクトタイプ
typedef enum tag_GME_GMK_TYPE{

	GME_GMK_TYPE_PISTON_UP = 0,	// 上向き
	GME_GMK_TYPE_PISTON_DOWN,	// 下向き

	GME_GMK_TYPE_MAX

}GME_GMK_TYPE;

// 地形矩形テーブル
typedef enum tag_GME_GMK_BWALL_RECT_DATA{

	GME_GMK_RECT_DATA_COL_WIDTH	= 0,	// 幅
	GME_GMK_RECT_DATA_COL_HEIGHT,		// 高さ
	GME_GMK_RECT_DATA_COL_OFST_X,		// 中心位置からＸ方向のオフセット
	GME_GMK_RECT_DATA_COL_OFST_Y,		// 中心位置からＹ方向のオフセット

	GME_GMK_RECT_DATA_DEF_LEFT,			// 当たり判定矩形　左側
	GME_GMK_RECT_DATA_DEF_TOP,			// 当たり判定矩形　
	GME_GMK_RECT_DATA_DEF_RIGHT,		// 当たり判定矩形　
	GME_GMK_RECT_DATA_DEF_BOTTOM,		// 当たり判定矩形　中心位置からＹ方向のオフセット

	GME_GMK_RECT_DATA_MAX

} GME_GMK_BWALL_RECT_DATA;
// ---------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	土地当たり矩形大きさ定義
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	岩タイプ
	//		コリジョン矩形
#define		GMD_GMK_BOBJ1_RECT_WIDTH		(56)
#define		GMD_GMK_BOBJ1_RECT_HEIGHT		(32)
	//		当たり矩形拡大マージン
#define		GMD_GMK_BOBJ1_RECT_MARGIN		(32)


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const static s16 tbl_gm_gmk_piston_col_rect[GME_GMK_TYPE_MAX][GME_GMK_RECT_DATA_MAX] = {

	//	ピストン＠ゾーン４工場
	{
		//	土地あたり矩形
		  GMD_GMK_BOBJ1_RECT_WIDTH,		//	中心位置からの幅
		  GMD_GMK_BOBJ1_RECT_HEIGHT,	//	中心位置からの高さ
		 -(GMD_GMK_BOBJ1_RECT_WIDTH/2),	//	中心位置から地形あたりの発生する場所までのオフセット
		                         0,		//	中心位置から地形あたりの発生する場所までのオフセット

		//	破壊あたり矩形
		 -(GMD_GMK_BOBJ1_RECT_WIDTH/2),    // 左
		     GMD_GMK_BOBJ1_RECT_HEIGHT,    // 上
		 +(GMD_GMK_BOBJ1_RECT_WIDTH/2),    // 右
		                            0      // 下(接地するようなので０にしておく)
	},
	{
		//	土地あたり矩形
		  GMD_GMK_BOBJ1_RECT_WIDTH,		//	中心位置からの幅
		  GMD_GMK_BOBJ1_RECT_HEIGHT,	//	中心位置からの高さ
		 -(GMD_GMK_BOBJ1_RECT_WIDTH/2),	//	中心位置から地形あたりの発生する場所までのオフセット
		  -GMD_GMK_BOBJ1_RECT_HEIGHT,	//	中心位置から地形あたりの発生する場所までのオフセット

		//	破壊あたり矩形
		 -(GMD_GMK_BOBJ1_RECT_WIDTH/2),    // 左
		                             0,    // 上
		 +(GMD_GMK_BOBJ1_RECT_WIDTH/2),    // 右
		    GMD_GMK_BOBJ1_RECT_HEIGHT      // 下(接地するようなので０にしておく)
	},
};
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------





// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef struct tag_GMS_GMK_PISTON_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	GME_GMK_TYPE		obj_type;		//!< 上向きか下向きかオブジェクトタイプ
	u16					piston_vect;

	fx32				stroke_spd;				//!< 移動速度
	fx32				timer_dec;				//!< 計算用
	fx32				timer_set_move;			//!< 移動時間　移動幅÷速度
	s32					timer_set_wait_upper;	//!< Hyeneで設定した上死点(伸びきり)待機時間
	s32					timer_set_wait_lower;	//!< Hyeneで設定した下死点(縮みきり)待機時間

	BOOL				efct_di;

}GMS_GMK_PISTON_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// ---------------------------------------------------------------------------



// ===========================================================================
// gmGmkPiston*
/*!
	ギミック ピストン＠ゾーン４工場

	@note
		画面を上下するギミックです。
		世界的に２ストロークは排除へと向かっているのが非常に残念です。
 */
// ---------------------------------------------------------------------------
static void gmGmkPistonStay(OBS_OBJECT_WORK *obj_work);		//縮んだ状態で待ち
static void gmGmkPistonStroke(OBS_OBJECT_WORK *obj_work);		//伸び
static void gmGmkPistonTopDeadWait(OBS_OBJECT_WORK *obj_work);	//上死点
static void gmGmkPistonShrink(OBS_OBJECT_WORK *obj_work);		//縮み

static void gmGmkPistonRod_Create(OBS_OBJECT_WORK *parent_obj);
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkPistonSyncTimeGet
/*!
	ギミック ピストン　同期時間取得
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
u32 gmGmkPistonSyncTimeGet(GMS_GMK_PISTON_WORK *pwork)
{
	u32 sync_timer = g_gm_main_system.sync_time;
	u32 piston_time;
	u32 move_time;

	move_time = (u32)((pwork->timer_set_move+(pwork->stroke_spd-1))/pwork->stroke_spd);
	piston_time  = move_time*2;
	piston_time += pwork->timer_set_wait_upper;
	piston_time += pwork->timer_set_wait_lower;
	sync_timer %= piston_time;

	return sync_timer;
}

// ---------------------------------------------------------------------------
// gmGmkPistonStart
/*!
	ギミック ピストン　サブ初期化

	@note
		
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPistonStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
//	OBS_RECT_WORK *rect_work;

	//	土地あたり矩形餅オブジェ設定
	//	普段では通り抜けられないように
	pwork->COMWORK.col_work.obj_col.obj       = obj_work;
	// 地形矩形設定
	pwork->COMWORK.col_work.obj_col.width  = (u16)(tbl_gm_gmk_piston_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_COL_WIDTH]);	//	中心位置からの幅
	pwork->COMWORK.col_work.obj_col.height = (u16)(tbl_gm_gmk_piston_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_COL_HEIGHT]);	//	中心位置からの高さ
	pwork->COMWORK.col_work.obj_col.ofst_x = tbl_gm_gmk_piston_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_COL_OFST_X];	//	中心位置から地形あたりの発生する場所までのオフセット
	pwork->COMWORK.col_work.obj_col.ofst_y = tbl_gm_gmk_piston_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_COL_OFST_Y];	//	中心位置から地形あたりの発生する場所までのオフセット
	pwork->COMWORK.col_work.obj_col.dir = (u16)(0x0000);

// diff_deta入れてみる	■■■■■■■■kuramoto■■■■■■■■
	pwork->COMWORK.col_work.obj_col.diff_data = (s8*)g_gm_default_col;	
	pwork->COMWORK.col_work.obj_col.flag	 |= OBD_COLOBJ_NOFREE_DIFF_DATA		// diff_dataを開放しない
											  | OBD_COLOBJ_NODIR_PARENT;		// 親の角度無視
// diff_deta入れてみる	■■■■■■■■kuramoto■■■■■■■■
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;



	// 矩形設定
	// 対プレイヤー
// 不要矩形情報セット処理削除	■■■■■■■■kuramoto■■■■■■■■
//	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
//	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
//	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
//	rect_work->ppDef = NULL;
//	rect_work->ppHit = NULL;
//	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
//	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
//	// 被破壊矩形設定
//	ObjRectWorkSet(rect_work,
//	               tbl_gm_gmk_piston_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_LEFT],
//	               tbl_gm_gmk_piston_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_TOP],
//	               tbl_gm_gmk_piston_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_RIGHT],
//	               tbl_gm_gmk_piston_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_BOTTOM]);
//	obj_work->flag &= ~OBD_OBJECT_NOHIT;								// 矩形あたり無し◆
// 不要矩形情報セット処理削除	■■■■■■■■kuramoto■■■■■■■■

	gmGmkPistonRod_Create(obj_work);

	// ピストン固有のメンバ初期化

	//	同期をとります
	{
		u32 move_time;
		u32 sync_timer;

		move_time = (u32)((pwork->timer_set_move+(pwork->stroke_spd-1))/pwork->stroke_spd);
		sync_timer = gmGmkPistonSyncTimeGet(pwork);

		//	下死点待機時間中判定
		if( sync_timer <= (u32)pwork->timer_set_wait_lower )
		{
			pwork->timer_dec = (fx32)(pwork->timer_set_wait_lower-(sync_timer-1));
			gmGmkPistonStay(obj_work);	//下死点
			return;
		}
		sync_timer -= (u32)pwork->timer_set_wait_lower;


		//	ストローク速度を取得
		fx32 spdy = (pwork->piston_vect==0x0000)?	// 下向き？
                      pwork->stroke_spd:/* +方向=下*/
                     -pwork->stroke_spd;/* -方向=上*/


		//	伸びストローク時間中判定
		if( sync_timer < move_time )
		{
			//	かっちりあわせる
			pwork->timer_dec = pwork->timer_set_move;
			while( sync_timer > 1 )
			{
				pwork->timer_dec -= pwork->stroke_spd;
				obj_work->pos.y += spdy;
				sync_timer -= 1;
			}
			gmGmkPistonStroke(obj_work);
			return;
		}
		sync_timer -= move_time;


		//	上死点位置まで移動
		obj_work->pos.y += (pwork->piston_vect==0x0000)?	// 下向き？
		                      pwork->timer_set_move:/* +方向=下*/
		                     -pwork->timer_set_move;/* -方向=上*/


		//	上死点待機時間内判定
		if( sync_timer <= (u32)pwork->timer_set_wait_upper )
		{
			pwork->timer_dec = (fx32)(pwork->timer_set_wait_upper-(sync_timer-1));
			gmGmkPistonTopDeadWait(obj_work);	//上死点
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
			obj_work->pos.y -= spdy;
			sync_timer -= 1;
		}
		gmGmkPistonShrink(obj_work);
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkPistonStay
/*!
	ギミック ピストン運動

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPistonStay_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkPistonStay_200(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonStay(OBS_OBJECT_WORK *obj_work)
{
	obj_work->spd.y = 0;
	obj_work->chgf(gmGmkPistonStay_100);
	gmGmkPistonStay_100(obj_work);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonStay_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;

	//	生成後の初期待ち時間
	pwork->timer_dec -= 1;
	if( pwork->timer_dec <= 0 )
	{
		obj_work->chgf(gmGmkPistonStay_200);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonStay_200(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
	//	伸び設定
	pwork->timer_dec = pwork->timer_set_move;
	obj_work->chgf(gmGmkPistonStroke);
	gmGmkPistonStroke(obj_work);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPistonStroke_100(OBS_OBJECT_WORK *obj_work);	//	伸び中
static void gmGmkPistonStroke_200(OBS_OBJECT_WORK *obj_work);	//
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonStroke(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
	//	伸び開始
	obj_work->spd.y = (pwork->piston_vect==0x0000)?
	                    pwork->stroke_spd:/* +方向=下*/
	                   -pwork->stroke_spd;/* -方向=上*/

	//	gmGmkPistonStroke_100をこの割り込みで通過するので要先行計算
	obj_work->pos.y += obj_work->spd.y;
	obj_work->chgf(gmGmkPistonStroke_100);
	gmGmkPistonStroke_100(obj_work);

	GmSoundPlaySE("Piston1");	// SE
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonStroke_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
	//	伸び中
	pwork->timer_dec -= pwork->stroke_spd;
	if( pwork->timer_dec <= 0 )
	{
		obj_work->spd.y = 0;
		if( pwork->timer_dec < 0 )
		{
			obj_work->spd.y = pwork->timer_dec;
			if (pwork->piston_vect==0x8000)
				obj_work->spd.y = -obj_work->spd.y;
		}
//		pwork->timer_dec = (fx32)(pwork->timer_set_wait_upper);
//		gmGmkPistonTopDeadWait(obj_work);
		obj_work->chgf(gmGmkPistonStroke_200);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
void gmGmkPistonStroke_200(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
#if 0
	pwork->timer_dec = (fx32)(pwork->timer_set_wait_upper);
#else
	// ポーズごとにsync_timerがずれる為、同期あわせる
	u32	move_time = (u32)((pwork->timer_set_move+(pwork->stroke_spd-1))/pwork->stroke_spd);
	u32	sync_timer = gmGmkPistonSyncTimeGet(pwork);
	sync_timer -= (u32)pwork->timer_set_wait_lower;
	sync_timer -= move_time;

	//	上死点待機時間内判定
	if (sync_timer <= (u32)pwork->timer_set_wait_upper) { 
		pwork->timer_dec = (fx32)(pwork->timer_set_wait_upper-(sync_timer-1));
	} else {
		pwork->timer_dec = 0;
	}
#endif

	gmGmkPistonTopDeadWait(obj_work);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPistonTopDeadWait_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkPistonTopDeadWait_200(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonTopDeadWait(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
	if( !pwork->efct_di )
	{
		//	上死点
		OBS_OBJECT_WORK *eff_obj =
				(OBS_OBJECT_WORK*)GmEfctCmnEsCreate( NULL, GME_EFCT_CMN_IDX_PISTON );
		eff_obj->pos.x = obj_work->pos.x;
		eff_obj->pos.y = obj_work->pos.y;
		eff_obj->pos.z = obj_work->pos.z + /*1*/16*FX32_ONE;
		eff_obj->dir.z = obj_work->dir.z;
		GmSoundPlaySE("Piston2");	// SE
	}

	obj_work->spd.y = 0;
	obj_work->chgf(gmGmkPistonTopDeadWait_100);
	gmGmkPistonTopDeadWait_100(obj_work);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonTopDeadWait_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
	//	伸び待ち中
	pwork->timer_dec -= 1;
	if( pwork->timer_dec <= 0 )
	{
		obj_work->chgf(gmGmkPistonTopDeadWait_200);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonTopDeadWait_200(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
	//	縮み設定
	pwork->timer_dec = pwork->timer_set_move;
	gmGmkPistonShrink(obj_work);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPistonShrink_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkPistonShrink_200(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonShrink(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
	//	縮み開始
	obj_work->spd.y = (pwork->piston_vect==0x0000)?
	                   -pwork->stroke_spd:/* -方向=上*/
	                   +pwork->stroke_spd;/* +方向=下*/

	//	gmGmkPistonShrink_100をこの割り込みで通過するので要先行計算
	obj_work->pos.y += obj_work->spd.y;
	obj_work->chgf(gmGmkPistonShrink_100);
	gmGmkPistonShrink_100(obj_work);

	GmSoundPlaySE("Piston1");	// SE
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonShrink_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
	//	縮み中
	pwork->timer_dec -= pwork->stroke_spd;
	if( pwork->timer_dec <= 0 )
	{
		obj_work->spd.y = 0;
		if( pwork->timer_dec < 0 )
		{
			obj_work->spd.y = pwork->timer_dec;
			if (pwork->piston_vect!=0x8000)
				obj_work->spd.y = -obj_work->spd.y;
		}
		obj_work->chgf(gmGmkPistonShrink_200);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPistonShrink_200(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;
#if 0
	pwork->timer_dec = (fx32)(pwork->timer_set_wait_lower);
#else
	// ポーズごとにsync_timerがずれる為、同期あわせる
	u32	sync_timer = gmGmkPistonSyncTimeGet(pwork);
	if (sync_timer <= (u32)pwork->timer_set_wait_lower) {
		pwork->timer_dec = (fx32)(pwork->timer_set_wait_lower-(sync_timer-1));
	} else {
		pwork->timer_dec = 0;
	}
#endif
	gmGmkPistonStay(obj_work);
}
// ---------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================


// ==========================================================================
// gmGmkPistonRod*
/*!
	ギミック ピストンロッド＠ゾーン４工場

	@note
		ピストンを上下させるものとピストンをつなぐシャフトです。
		コネクティングロッド(コンロッド)という言い方もあります。
 */
// --------------------------------------------------------------------------
typedef struct tag_GMS_GMK_PISTONROD_WORK
{
	GMS_EFFECT_3DNN_WORK eff_work;
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	GME_GMK_TYPE		obj_type;		//!< 壁か床かオブジェクトタイプ

	fx32				fulcrum;

}GMS_GMK_PISTONROD_WORK;
#define	OBJ_3D		eff_work.obj_3d
#define	COMWORK		eff_work.efct_com
// ---------------------------------------------------------------------------
static void gmGmkPistonRodStay(OBS_OBJECT_WORK *obj_work);
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// gmGmkPistonRod*
/*!
	ギミック ピストンロッド＠ゾーン４工場

	@note
		ピストンを上下させるものとピストンをつなぐシャフトです。
		コネクティングロッド(コンロッド)という言い方もあります。
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPistonRodStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PISTONROD_WORK *pwork = (GMS_GMK_PISTONROD_WORK*)obj_work;

	fx32 stroke;
	stroke = MTM_MATH_ABS(obj_work->parent_obj->pos.y - pwork->fulcrum);

	stroke /= 8;
	obj_work->scale.y = stroke;
}
// --------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkPiston_CreateRod
/*!
	ギミック ピストンロッド生成

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPistonRod_Create(OBS_OBJECT_WORK *parent_obj)
{
	GMS_GMK_PISTONROD_WORK *pwork =
		(GMS_GMK_PISTONROD_WORK*)GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_PISTONROD_WORK),
			                                             NULL,
			                                             0,
			                                             "Gmk_PistonRod");
	OBS_OBJECT_WORK *obj_work = (OBS_OBJECT_WORK*)pwork;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_piston_obj_3d_list[IDB_GMK_PISTON_MDL_GMK_PISTON_POLE_ZNO],
	                             &pwork->eff_work.obj_3d);

	obj_work->parent_obj = parent_obj;
	obj_work->parent_ofst.x = 0;
	obj_work->parent_ofst.y = 16*FX32_ONE;
	obj_work->parent_ofst.z = - GMD_OBJ_GIMMICK_POS_Z_BACK + GMD_OBJ_DEFAULT_POS_Z_B_BACK;

	obj_work->dir.z = (u16)(parent_obj->dir.z^0x8000);
	if( obj_work->dir.z == 0x0000 )
		obj_work->parent_ofst.y = -obj_work->parent_ofst.y;

	// フラグ
	obj_work->flag |= OBD_OBJECT_PARENT_FIX;						// 親付随
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->disp_flag &= ~OBD_DISP_NODIR;
	obj_work->flag |= OBD_OBJECT_NOHIT;								// 矩形あたり無し◆

	obj_work->chgf(gmGmkPistonRodStay);

	pwork->fulcrum = parent_obj->pos.y+obj_work->parent_ofst.y;
}
// --------------------------------------------------------------------------
// ==========================================================================






// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkPiston?Init
/*!
 *	ギミック ピストン＠ゾーン４工場 初期化関数
 *	GmGmkPistonUInit 上向き
 *	GmGmkPistonDInit 下向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
static OBS_OBJECT_WORK* gmGmkPistonInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_PISTON_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;


	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_PISTON_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_PISTON_WORK), "Gmk_PistonRod");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_piston_obj_3d_list[IDB_GMK_PISTON_MDL_GMK_PISTON_TOP_ZNO],
	                             &gmk_work->obj_3d);
	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->disp_flag &= ~OBD_DISP_NODIR;
	obj_work->flag |= OBD_OBJECT_NOHIT;								// 矩形あたり無し◆

	//	標準のデータ
	pwork->stroke_spd  = GMD_GMK_PISTON_STROKE_SPEED;		//	標準の速度
	pwork->timer_set_move = GMD_GMK_PISTON_STROKE;			//	標準の移動量

	//	設定の反映
//	pwork->timer_dec = eve_rec->width;						//	Hyenaで設定する初期待機フレーム数
	if( eve_rec->flag & 0x01F )
	{
		fx32 add_spd;
		if( (eve_rec->flag&0x1f) <= 0x010 )
			add_spd = (fx32)((eve_rec->flag&0x1f)<<(FX32_SHIFT-2));
		else
		{
			add_spd = (fx32)((-(eve_rec->flag&0x0f))<<(FX32_SHIFT-2));
		}
		pwork->stroke_spd += add_spd;
	}
	pwork->efct_di = !(eve_rec->flag&0x80)? FALSE:TRUE;

	pwork->timer_set_wait_upper = eve_rec->left*2;
	pwork->timer_set_wait_lower = eve_rec->height*2;

	return obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPistonUpInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK	*obj_work = gmGmkPistonInit( eve_rec ,pos_x, pos_y, type);
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;

	pwork->obj_type = GME_GMK_TYPE_PISTON_UP;	// 上向き
	pwork->piston_vect = 0x8000;
	obj_work->dir.z = 0x0000;
	pwork->gmk_work.ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NODIR_PARENT;

	//	移動幅
	if( eve_rec->top < 0 )
		pwork->timer_set_move = (-eve_rec->top*2)<<FX32_SHIFT;
	else if( eve_rec->top > 0 )
		pwork->timer_set_move = (eve_rec->top*2)<<FX32_SHIFT;

	// あたり矩形などの設定と処理のスタート
	obj_work->chgf(gmGmkPistonStart);
	return obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPistonDownInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK	*obj_work = gmGmkPistonInit( eve_rec ,pos_x, pos_y, type);
	GMS_GMK_PISTON_WORK *pwork = (GMS_GMK_PISTON_WORK*)obj_work;

	pwork->obj_type = GME_GMK_TYPE_PISTON_DOWN;	// 下向き
	pwork->piston_vect = 0x0000;
	obj_work->dir.z = 0x8000;
	pwork->gmk_work.ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NODIR_PARENT;

	//	移動幅
	if( eve_rec->top > 0 )
		pwork->timer_set_move = (eve_rec->top*2)<<FX32_SHIFT;
	else if( eve_rec->top < 0 )
		pwork->timer_set_move = (-eve_rec->top*2)<<FX32_SHIFT;

	// あたり矩形などの設定と処理のスタート
	obj_work->chgf(gmGmkPistonStart);
	return obj_work;
}
// ==========================================================================



// ===========================================================================
// GmGmkPistonBuild
/*!
	ギミック ピストン＠ゾーン４工場 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkPistonBuild(void)
{
	gm_gmk_piston_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PISTON_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PISTON_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkPistonFlush
/*!
	ギミック ピストン＠ゾーン４工場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkPistonFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PISTON_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_piston_obj_3d_list, amb->file_num);
}
// ===========================================================================
