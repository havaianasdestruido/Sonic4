// =======================================================================
/*!
	@file	gmGmkSeesaw.c
	@brief	ギミック シーソー＠ゾーン４工場

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkSeesaw.cpp 2 2011-04-11 05:21:26Z thamada $
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

#include "gmGmkSeesaw.h"

// データヘッダ
#include "common/model/gmk_seesaw_mdl.hmb"



// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_seesaw_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
#define	OBJWORK		COMWORK.obj_work
#define	chgf(f)		ppFunc = (f)
// -------------

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// -------------

// ----- Definitions -------------------------------------------（定数の宣言）
#define		GMD_GMK_SEESAW_SONIC_OFFSET_Y		(14)
#define		GMD_GMK_SEESAW_TABLE_OFFSET_Y		(15)




#define		GMD_GMK_SEESAW_TILT_MAX			(0x1200)
//	※この値はHyenaのイベント画像角度に連動しています。
//	　変更の場合はイベント情報も確認・変更のこと。

#define		GMD_GMK_SEESAW_TILT_DELTA_MAX	(0x0100)


// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// オブジェクトタイプ
typedef enum tag_GME_GMK_TYPE{

	GME_GMK_TYPE_SEESAW_0 = 0,	// 水平
	GME_GMK_TYPE_SEESAW_30,		// 右傾き
	GME_GMK_TYPE_SEESAW_330,	// 左傾き

	GME_GMK_TYPE_MAX

}GME_GMK_TYPE;
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// あたり判定矩形テーブル
typedef enum tag_GME_GMK_BWALL_RECT_DATA{

	GME_GMK_RECT_DATA_COL_WIDTH	= 0,	// 幅
	GME_GMK_RECT_DATA_COL_HEIGHT,		// 高さ
	GME_GMK_RECT_DATA_COL_OFST_X,		// 中心位置からＸ方向のオフセット
	GME_GMK_RECT_DATA_COL_OFST_Y,		// 中心位置からＹ方向のオフセット

	GME_GMK_RECT_DATA_DEF_LEFT = 0,		// 当たり判定矩形　左側
	GME_GMK_RECT_DATA_DEF_TOP,			// 当たり判定矩形　
	GME_GMK_RECT_DATA_DEF_RIGHT,		// 当たり判定矩形　
	GME_GMK_RECT_DATA_DEF_BOTTOM,		// 当たり判定矩形　中心位置からＹ方向のオフセット

	GME_GMK_RECT_DATA_MAX

} GME_GMK_RECT_DATA;
// ---------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	当たり矩形大きさ定義
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define		GMD_GMK_SEESAW_RECT_X		(0)
#define		GMD_GMK_SEESAW_RECT_Y		(-8)
#define		GMD_GMK_SEESAW_RECT_W		(96+16)
#define		GMD_GMK_SEESAW_RECT_H		( 8)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const static s16 tbl_gm_gmk_seesaw_rect[GME_GMK_TYPE_MAX][GME_GMK_RECT_DATA_MAX] = {

	//	シーソー＠ゾーン４工場
	{
		//	テスト土地あたり矩形
		   96,		//	幅
		   32,		//	高さ
		  -48,		//	位置
		  -32,		//	位置
	},
};
// ---------------------------------------------------------------------------



// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef struct tag_GMS_GMK_SEESAW_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	u16		seesaw_id;

	s16		initial_tilt;
	s16		tilt;
	s16		tilt_d;
	s16		tilt_acc;
	s16		tilt_timer;
	s16		tilt_se_timer;

	fx32	hold_x,hold_y;
	fx64	player_distance;
	fx32	player_speed;

	GMS_PLAYER_WORK *ply_work;

}GMS_GMK_SEESAW_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//	連動管理
#define	GMD_GMK_SEESAW_MAX		(16)
static s16 seesaw_tilt[GMD_GMK_SEESAW_MAX];
static s16 seesaw_alive[GMD_GMK_SEESAW_MAX];
static GMS_GMK_SEESAW_WORK *control_right;	//	権利持ち
static u16	lock_seesaw_id;		//	ロック中のシーソーＩＤ
// ---------------------------------------------------------------------------



// ===========================================================================
// gmGmkSeesaw*
/*!
	ギミック シーソー＠ゾーン４工場

	@note
		ソニックの位置を判定し、入れない場合は土地当たりを有効に、
		スタート位置に入った場合はソニックをコントロールする必要があります。

		とりあえず存在と当たり判定と土地当たりを作成。
 */
// ---------------------------------------------------------------------------
static void gmGmkSeesawStay(OBS_OBJECT_WORK *obj_work);
static void gmGmkSeesaw_PlayerHold(OBS_OBJECT_WORK *obj_work);
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSeesawExitTCB
/*!
	ギミック シーソー　開放

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSeesawExitTCB(MTS_TASK_TCB *tcb)
{
	GMS_GMK_SEESAW_WORK	*pwork;

	pwork = (GMS_GMK_SEESAW_WORK*)mtTaskGetTcbWork(tcb);
	seesaw_alive[pwork->seesaw_id] -= 1;

	// プレイヤー保持したタスクが消滅する時は共通ワークを消してから
	if (control_right == pwork) {
		control_right  = NULL;
		lock_seesaw_id = NULL;
	}
	// オブジェクト標準解放
	GmEnemyDefaultExit(tcb);
}
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkSeesawStart
/*!
	ギミック シーソー　サブ初期化

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSeesawStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SEESAW_WORK *pwork = (GMS_GMK_SEESAW_WORK*)obj_work;
	OBS_RECT_WORK *rect_work;

	// チェック用矩形
	// 対プレイヤー
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = NULL;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 被破壊矩形設定
	ObjRectWorkSet(rect_work, -48, -24, 48, 0 );

	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF];
	rect_work->flag |= OBD_RECT_ENABLE;
	rect_work->ppDef = NULL;
	rect_work->ppHit = NULL;
//	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
//	ObjRectDefSet(rect_work, 0, 0);
	// 被破壊矩形設定
	ObjRectWorkSet(rect_work, -2, -2, 2, 2 );

	obj_work->flag &= ~OBD_OBJECT_NOHIT;							// 矩形あたり無し◆

	pwork->initial_tilt = pwork->tilt;
	pwork->tilt_d = 0;
	pwork->tilt_se_timer = 0;
	if( seesaw_alive[pwork->seesaw_id] == 0 )
		seesaw_tilt[pwork->seesaw_id] = pwork->tilt;
	else
		pwork->tilt = seesaw_tilt[pwork->seesaw_id];

	obj_work->dir.z = (u16)pwork->tilt;
	seesaw_alive[pwork->seesaw_id] += 1;
	// 終了処理差し替え 上記のカウントデクリメントを行う
	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkSeesawExitTCB);

	obj_work->chgf(gmGmkSeesawStay);
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSeesawStay
/*!
	ギミック シーソー

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSeesawStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SEESAW_WORK *pwork = (GMS_GMK_SEESAW_WORK*)obj_work;
	GMS_PLAYER_WORK *ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	//	角度修正
	if( lock_seesaw_id == pwork->seesaw_id )
	{
		pwork->tilt = seesaw_tilt[pwork->seesaw_id];
		obj_work->dir.z = (u16)pwork->tilt;
		pwork->tilt_timer = 60;
	}
	else if( pwork->tilt != pwork->initial_tilt )
	{
		if( pwork->tilt_timer <= 0 )
		{
			if( pwork->tilt > pwork->initial_tilt )
			{
				pwork->tilt -= GMD_GMK_SEESAW_TILT_DELTA_MAX;
				if( pwork->tilt < pwork->initial_tilt )
					 pwork->tilt = pwork->initial_tilt;
			}
			else if( pwork->tilt < pwork->initial_tilt )
			{
				pwork->tilt += GMD_GMK_SEESAW_TILT_DELTA_MAX;
				if( pwork->tilt > pwork->initial_tilt )
					 pwork->tilt = pwork->initial_tilt;
			}
		}
		else
			pwork->tilt_timer -= 1;
	}
	obj_work->dir.z = (u16)pwork->tilt;
	if( pwork->tilt == 0 )
	{
		pwork->tilt = 0;
	}

	// 拘束判定除外条件チェック
	if (  (ply_work->player_flag & GMD_PLF_DIE)					// 死亡中
		||(ply_work->obj_work.flag & OBD_OBJECT_NOHIT)			// ヒット判定無し
		||(  (ply_work->obj_work.move_flag & OBD_MOVE_JUMP)		// ジャンプ中で
		   &&(ply_work->obj_work.spd.y < 0) ) ) {				//   且つ上昇中
		return;
	}
	
	//	プレイヤーの位置と自分の幅を確認
	fx32 offset_x = mtMathSin((u16)pwork->tilt)*(GMD_GMK_SEESAW_TABLE_OFFSET_Y+GMD_GMK_SEESAW_SONIC_OFFSET_Y);	//	90度位相がずれる
	fx32 offset_y = mtMathCos((u16)pwork->tilt)*(GMD_GMK_SEESAW_TABLE_OFFSET_Y+GMD_GMK_SEESAW_SONIC_OFFSET_Y);	//	90度位相がずれる
	fx32 w = (mtMathCos((u16)pwork->tilt)*GMD_GMK_SEESAW_RECT_W)/2;
	fx32 h = MTM_MATH_ABS((mtMathSin((u16)pwork->tilt)*GMD_GMK_SEESAW_RECT_W)/2);

	fx32 ply_px = ply_work->obj_work.pos.x - obj_work->pos.x;
	if( ply_px >= -(w+32*FX32_ONE-offset_x) && ply_px <= (w+32*FX32_ONE+offset_x) )
	{
		fx64 pa,pb;
		fx64 ga,gb;
		fx32 x,y;

		//	プレイヤー軸線
		if( ply_work->obj_work.move.x == 0 )
		{
			if( ply_work->obj_work.move.y == 0 )
				return;		//	動いていない場合は評価しない

			//	縦軸のみ移動
			pa = ply_work->obj_work.pos.x;
			pb = 0;
		}
		else if( ply_work->obj_work.move.y == 0 )
		{
			//	横軸のみ移動
			pa = 0;
			pb = ply_work->obj_work.pos.y;
		}
		else
		{
			//	軸線判定
			pa = (fx64)(ply_work->obj_work.move.y << FX32_SHIFT);	// 1/60前の移動Ｘ
			pa /= ply_work->obj_work.move.x;							// 1/60前の移動Ｙ
			pb = pa*ply_work->obj_work.pos.x;
			pb >>= FX32_SHIFT;
			pb = ply_work->obj_work.pos.y - pb;
			//	Y = paX+ pb;
		}

		//	ギミック軸線
		ga = mtMathCos((u16)pwork->tilt);	// ga = 0は角度的にありえないので省く
		gb = mtMathSin((u16)pwork->tilt);	// 0sin = 0
		gb <<= FX32_SHIFT;
		ga = gb/ga;
		gb = ga*(obj_work->pos.x+offset_x);
		gb >>= FX32_SHIFT;
		gb = (obj_work->pos.y-offset_y)-gb;
		//	Y = gaX+ gb;
		//	水平の線は ga = 0 && gb = y

		if( pa != 0 && pa == ga )
		{
			//	同一式の場合、プレイヤーのＸ位置からギミック線上のＹ位置のみ求める
			x = ply_work->obj_work.pos.x;
			y = (fx32)(((ga*x)>>FX32_SHIFT) + gb);
		}
		else if( pa != 0 && pb != 0 )
		{
			//	Y = paX+pb
			//	Y = gaX+gb
			//	paX+pb = gaX+gb;
			//	paX-gaX = gb-pb;
			//	X(pa-ga)= gb-pb;
			//	X = (gb-pb)/(pa-ga)
			//	Y = paX+pb
			x = (fx32)(((gb-pb)<<FX32_SHIFT)/(pa-ga));
			y = (fx32)(((ga*x)>>FX32_SHIFT) + gb);	//	ギミック戦場の接点
		}
		else if( pa == 0 )
		{
			//	横軸のみ移動
			//	y = ga.x +gb;
			//	ga.x = y-gb
			//	x = (y-gb)/ga;
			y = (fx32)pb;
			if( ga != 0 )
			{
				x = (fx32)(((y-gb)<<FX32_SHIFT)/ga);
			}
			else
			{
				x = ply_work->obj_work.pos.x;
			}
		}
		else if( pb == 0 )
		{
			//	縦軸のみ移動
			//	X = player_x;
			//	Y = player_x*ga + gb;
			x = (fx32)pa;
			y = (fx32)(((ga*pa)>>FX32_SHIFT) + gb);
		}
		//	x,y が交点

#if 1
		#define		_ol	(obj_work->pos.x-(w-offset_x))
		#define		_or	(obj_work->pos.x+(w+offset_x))
		#define		_ot	(obj_work->pos.y-(h+offset_y)-2)
		#define		_ob	(obj_work->pos.y+(h-offset_y)+2)
#else
		//	矩形表示で視覚的にデバッグしやすく
		fx32		_ol = -(w-offset_x);
		fx32		_or = +(w+offset_x);
		fx32		_ot = -(h+offset_y)-2;
		fx32		_ob = +(h-offset_y)+2;

		OBS_RECT_WORK *rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
		ObjRectWorkSet(rect_work, (s16)(_ol>>FX32_SHIFT), (s16)(_ot>>FX32_SHIFT), (s16)(_or>>FX32_SHIFT), (s16)(_ob>>FX32_SHIFT) );

		rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF];
		ObjRectWorkSet(rect_work,
		                          (s16)(((x-obj_work->pos.x)>>FX32_SHIFT)-1),
		                          (s16)(((y-obj_work->pos.y)>>FX32_SHIFT)-1),
		                          (s16)(((x-obj_work->pos.x)>>FX32_SHIFT)+1),
		                          (s16)(((y-obj_work->pos.y)>>FX32_SHIFT)+1));

		_ol += obj_work->pos.x;
		_or += obj_work->pos.x;
		_ot += obj_work->pos.y;
		_ob += obj_work->pos.y;
#endif
		//	交点がシーソーの範囲内かチェックする
		if( _ol <= x && x <= _or && _ob >= y && y >= _ot )
		{
			//	移動前と移動後の位置が交点を通過したかチェックする
			fx32	pxB,pyB;	//	
			fx32	pxS,pyS;	//	
			pxB = ply_work->obj_work.pos.x - ply_work->obj_work.move.x;
			pyB = ply_work->obj_work.pos.y - ply_work->obj_work.move.y;
			pxS = ply_work->obj_work.pos.x;
			pyS = ply_work->obj_work.pos.y;
			if( pxB < pxS )
		 		MTM_MATH_SWAP( pxB, pxS);
			if( pyB < pyS )
		 		MTM_MATH_SWAP( pyB, pyS);

			pyB += 0x500;//0x600;	//	誤差吸収
			pyS -= 0x500;//0x600;	//	移動結果に幅をもたせる
			if( pxS <= x && x <= pxB && pyB >= y && y >= pyS )
			{
				//	成立準備
				pwork->ply_work = ply_work;
				pwork->hold_x = x-offset_x;
				pwork->hold_y = y;
				if( control_right == NULL )
					gmGmkSeesaw_PlayerHold(obj_work);
				else
					obj_work->chgf(gmGmkSeesaw_PlayerHold);
			}
		}
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSeesaw_PlayerHold
/*!
	ギミック シーソー

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSeesaw_PlayerHold_100(OBS_OBJECT_WORK *obj_work);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSeesaw_PlayerHold(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SEESAW_WORK *pwork = (GMS_GMK_SEESAW_WORK*)obj_work;
	GMS_PLAYER_WORK *ply_work = pwork->ply_work;

	if( control_right != NULL )
	{
		//	優先はソニックが進む側にある
		if( (control_right->OBJWORK.pos.x >= obj_work->pos.x  && ply_work->obj_work.spd.x >= 0) ||
			//	保持者が右にいて、ソニックが右向きか
		    (control_right->OBJWORK.pos.x <= obj_work->pos.x  && ply_work->obj_work.spd.x <= 0) )
			//	保持者が左にいて、ソニックが左向きなら
		{
			//	優先は現保持者にある
			obj_work->chgf(gmGmkSeesawStay);
			gmGmkSeesawStay(obj_work);
			return;
		}
		//	ソニックは自分に向かっている
		pwork->player_speed = control_right->player_speed;	//	継続
	}
	else
	{
		pwork->player_speed = ply_work->obj_work.move.x/2;	//	少しスピードダウン
	}

	GmPlySeqGmkInitSeesaw(ply_work,&pwork->COMWORK);
//	ply_work->obj_work.pos.x = pwork->hold_x;
//	ply_work->obj_work.pos.y = pwork->hold_y;

	fx64 dist = (pwork->hold_x-obj_work->pos.x)<<FX32_SHIFT;
	dist /= mtMathCos(pwork->tilt);
	pwork->player_distance = dist;

	control_right = pwork;	//	権利保有
	obj_work->chgf(gmGmkSeesaw_PlayerHold_100);
	gmGmkSeesaw_PlayerHold_100(obj_work);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSeesaw_PlayerHold_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SEESAW_WORK *pwork = (GMS_GMK_SEESAW_WORK*)obj_work;
	GMS_PLAYER_WORK *ply_work = pwork->ply_work;

	lock_seesaw_id = NULL;
 	if (  (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_SEESAW)
 		||(control_right != pwork)
		||(ply_work->obj_work.move_flag & OBD_MOVE_NOCOL)			// デバッグ移動によりNOCOLフラグが立てられた場合の対処
 	   ) {
		//	何らかの理由でシーソーから外れた
		pwork->tilt_d = 0;
		pwork->tilt_se_timer = 0;
		if( control_right == pwork )
			control_right = NULL;
		obj_work->chgf(gmGmkSeesawStay);
		gmGmkSeesawStay(obj_work);
		return;
	}

	lock_seesaw_id = pwork->seesaw_id;
	pwork->tilt_timer = 60;

	control_right = pwork;	//	権利保有中
	//	角度調整
	s32	rot_d = (s32)GmPlayerKeyGetGimmickRotZ(pwork->ply_work);
	if( rot_d > GMD_GMK_SEESAW_TILT_DELTA_MAX )
		rot_d = GMD_GMK_SEESAW_TILT_DELTA_MAX;
	if( rot_d < -GMD_GMK_SEESAW_TILT_DELTA_MAX )
		rot_d = -GMD_GMK_SEESAW_TILT_DELTA_MAX;
	pwork->tilt_d = (s16)rot_d;

//	pwork->tilt_d += rot_d;
//	if( pwork->tilt_d > 0x0100 )
//		pwork->tilt_d = 0x0100;
//	if( pwork->tilt_d < -0x0100 )
//		pwork->tilt_d = -0x0100;

	if( pwork->tilt_d != 0 )
	{
		pwork->tilt += pwork->tilt_d;
		if( pwork->tilt_se_timer == 0 )
		{
			// SE
			GmSoundPlaySE("Seesaw");
			pwork->tilt_se_timer = 8;
		}
		pwork->tilt_se_timer -= 1;

		if( pwork->tilt >= GMD_GMK_SEESAW_TILT_MAX )
		{
			pwork->tilt = GMD_GMK_SEESAW_TILT_MAX;
			pwork->tilt_d = 0;
		}
		if( pwork->tilt <= -GMD_GMK_SEESAW_TILT_MAX )
		{
			pwork->tilt = -GMD_GMK_SEESAW_TILT_MAX;
			pwork->tilt_d = 0;
		}
	}
	obj_work->dir.z = (u16)pwork->tilt;
	seesaw_tilt[pwork->seesaw_id] = (s16)obj_work->dir.z;

	//	プレイヤーがカベにあたっている場合
	if( ply_work->obj_work.move_flag & OBD_MOVE_FRONT )
	{
		//	移動が阻害された！
		fx32 offset_x = mtMathSin((u16)pwork->tilt)*(GMD_GMK_SEESAW_TABLE_OFFSET_Y+GMD_GMK_SEESAW_SONIC_OFFSET_Y);	//	90度位相がずれる
		offset_x = (obj_work->pos.x+offset_x);
		offset_x = ply_work->obj_work.pos.x - offset_x;

		fx64 dist = offset_x<<FX32_SHIFT;
		dist /= mtMathCos(pwork->tilt);
		pwork->player_distance = dist;
		if( !(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && pwork->player_speed > 0 ||
		     (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && pwork->player_speed < 0 ) {
			pwork->player_speed = 0;
		}
	}
	//	プレイヤーをここで動かす
	fx64 lastdistance = pwork->player_distance;		// 1/60前の移動量保持
	pwork->player_distance += pwork->player_speed;
	lastdistance = pwork->player_distance-lastdistance;	// 1/60前の移動量
	pwork->player_speed += (0x100*mtMathSin((u16)pwork->tilt))>>FX32_SHIFT;

	fx32	gmkx,gmky;
	gmkx = mtMathSin((u16)pwork->tilt)*(GMD_GMK_SEESAW_TABLE_OFFSET_Y+GMD_GMK_SEESAW_SONIC_OFFSET_Y);	//	90度位相がずれる
	gmkx = obj_work->pos.x + gmkx;
	gmky = mtMathCos((u16)pwork->tilt)*(GMD_GMK_SEESAW_TABLE_OFFSET_Y+GMD_GMK_SEESAW_SONIC_OFFSET_Y);	//	90度位相がずれる
	gmky = obj_work->pos.y - gmky;

	fx32	plyx,plyy;
	plyx = (fx32)((pwork->player_distance*mtMathCos((u16)pwork->tilt))>>FX32_SHIFT);
	plyx = gmkx + plyx;
	plyy = (fx32)((pwork->player_distance*mtMathSin((u16)pwork->tilt))>>FX32_SHIFT);
	plyy = gmky + plyy;

	if( pwork->player_speed < 0 && !(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) )
		ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
	else if( pwork->player_speed > 0 && ply_work->obj_work.disp_flag & OBD_DISP_HFLIP )
		ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;

	if( pwork->player_distance <= (GMD_GMK_SEESAW_RECT_W+1)*FX32_ONE/2 && pwork->player_distance >= -(GMD_GMK_SEESAW_RECT_W+1)*FX32_ONE/2 )
	{
		// シーソー上に存在
		ply_work->obj_work.spd.x = plyx-ply_work->obj_work.pos.x;
		ply_work->obj_work.spd.y = plyy-ply_work->obj_work.pos.y;
//		ply_work->obj_work.pos.x = plyx;
//		ply_work->obj_work.pos.y = plyy;
	}
	else
	{
		// シーソー上からこぼれ落ちた
		fx32 spdx = (fx32)((lastdistance*mtMathCos((u16)pwork->tilt))>>FX32_SHIFT);
		fx32 spdy = (fx32)((lastdistance*mtMathSin((u16)pwork->tilt))>>FX32_SHIFT);
		if (MTM_MATH_ABS(spdx) < 0x100) {
			// 最低速度Xをセットしてみる
			if (spdx < 0) {
				spdx = (fx32)(-0x400);
			} else {
				spdx = (fx32)(0x400);
			}
		}
		GmPlySeqGmkInitSeesawEnd( ply_work, spdx, spdy);
		pwork->tilt_d = 0;
		control_right = NULL;
		lock_seesaw_id = NULL;
		obj_work->chgf(gmGmkSeesawStay);
	}
}
// ---------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================



// ==========================================================================
// gmGmkSeesaw*
/*!
	ギミック シーソー＠ゾーン４

	@note
 */
// --------------------------------------------------------------------------
typedef struct tag_GMS_GMK_SEESAWPARTS_WORK
{
	GMS_EFFECT_3DNN_WORK eff_work;
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。

}GMS_GMK_SEESAWPARTS_WORK;
#define	OBJ_3D		eff_work.obj_3d
#define	COMWORK		eff_work.efct_com
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// gmGmkSeesaw_CreateParts
/*!
	ギミック シーソー　部品生成

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSeesaw_CreateParts(GMS_GMK_SEESAW_WORK *pwork)
{
	GMS_GMK_SEESAWPARTS_WORK *parts;
	OBS_OBJECT_WORK *parent_obj;
	OBS_OBJECT_WORK *obj_work;

	parent_obj = (OBS_OBJECT_WORK*)pwork;
	//	土台
	{
		obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_SEESAWPARTS_WORK),
		                                  NULL,
		                                  0,
		                                 "Gmk_SeesawParts");
		parts =(GMS_GMK_SEESAWPARTS_WORK*)obj_work;
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_seesaw_obj_3d_list[IDB_GMK_SEESAW_MDL_SEESAW_B_ZNO],
		                             &parts->eff_work.obj_3d);

		obj_work->parent_obj = parent_obj;
		obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;		// 佇む
		obj_work->pos.x = parent_obj->pos.x;
		obj_work->pos.y = parent_obj->pos.y;
		obj_work->pos.z = parent_obj->pos.z+1*FX32_ONE;
		// フラグ
		obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
		obj_work->disp_flag |= OBD_DISP_NODIR;
		obj_work->flag |= OBD_OBJECT_NOHIT;								// 矩形あたり無し◆
		obj_work->chgf(NULL);		//	ジョブなし
	}
}
// --------------------------------------------------------------------------
// ==========================================================================






// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkSeesaw?Init
/*!
 *	ギミック シーソー＠ゾーン４工場 初期化関数
 *	GmGmkSeesawUInit 上向き
 *	GmGmkSeesawDInit 下向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
static GMS_GMK_SEESAW_WORK* gmGmkSeesawInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_SEESAW_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_SEESAW_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_SEESAW_WORK), "Gmk_Seesaw");
	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;
	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_seesaw_obj_3d_list[IDB_GMK_SEESAW_MDL_SEESAW_A_ZNO],
	                             &gmk_work->obj_3d);
	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	//	シーソーＩＤ
	pwork->seesaw_id = eve_rec->left;
	MTM_ASSERT(pwork->seesaw_id < GMD_GMK_SEESAW_MAX);

	//	部品
	gmGmkSeesaw_CreateParts(pwork);

	return pwork;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSeesaw0Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_SEESAW_WORK	*pwork;

	pwork = (GMS_GMK_SEESAW_WORK*)gmGmkSeesawInit(eve_rec, pos_x, pos_y, type);
	pwork->tilt = 0x0000;
	gmGmkSeesawStart(&pwork->gmk_work.ene_com.obj_work);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSeesaw30Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_SEESAW_WORK	*pwork;

	pwork = (GMS_GMK_SEESAW_WORK*)gmGmkSeesawInit(eve_rec, pos_x, pos_y, type);
	pwork->tilt = GMD_GMK_SEESAW_TILT_MAX;
	gmGmkSeesawStart(&pwork->gmk_work.ene_com.obj_work);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSeesaw330Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_SEESAW_WORK	*pwork;

	pwork = (GMS_GMK_SEESAW_WORK*)gmGmkSeesawInit(eve_rec, pos_x, pos_y, type);
	pwork->tilt = -GMD_GMK_SEESAW_TILT_MAX;
	gmGmkSeesawStart(&pwork->gmk_work.ene_com.obj_work);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================



// ===========================================================================
// GmGmkSeesawBuild
/*!
	ギミック シーソー＠ゾーン４工場 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkSeesawBuild(void)
{
	gm_gmk_seesaw_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SEESAW_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SEESAW_TEX),
								0/*draw_flag*/);
	for( int i = 0; i < 16; i++ )
	{
		seesaw_alive[i] = 0;
	}
	control_right = NULL;
}
// ===========================================================================


// ===========================================================================
// GmGmkSeesawFlush
/*!
	ギミック シーソー＠ゾーン４工場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkSeesawFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SEESAW_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_seesaw_obj_3d_list, amb->file_num);
}
// ===========================================================================
