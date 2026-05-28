// ===========================================================================
/*!
	@file	gmGmkBreakObj.cpp
	@brief	ギミック 破壊可能なオブジェ

	@author	ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkBreakObj.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

/* note
 *		横破壊型のオブジェは、そのグラフィックの意匠よりかなり大きめに被破壊判定矩形を有します。
 *		被破壊判定矩形は、接触を検知するとコリジョンを無効にし、
 *		評価対象矩形との重なった部分だけ小さくなります。
 *		被破壊判定矩形は、接触検知と縮小を繰り返し、その幅が意匠に見合ったところで破壊成立となります。
 *		プレイヤーが矩形範囲内から範囲外へ向かって移動するような動きをとった場合は、
 *		最初の判定によって縮められた範囲から外れることにより、接触は無効となり、
 *		オブジェクトはコリジョン情報と矩形情報を初期化します。
 *		範囲に接触してから移動が急に止まった場合も、縮めた矩形に届かない計算になりますので
 *		理論上、停止した段階で接触は無効となりオブジェクトはコリジョン情報と矩形情報を初期化します。
 *		ただし停止の条件に関しては未評価であり、小数部の計算誤差で停止した状態で
 *		毎フレーム接触した状態を繰り返す恐れがありますが、
 *		どの破壊壁も破壊には所定の速度が必要なため、停止状態での不具合は発生しないと考えられます。
 *
 *		基本的に破壊壁シリーズと同じアルゴリズムで評価対象が[上下][左右]の２点になります。
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
#include "gmPlayerDat.h"
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmEffectZone.h"
#include "gmPlyScoreDef.h"

#include "gmSound.h"

#include "gmGmkBreakObj.h"

// データヘッダ
#include "common/model/gmk_b_obj_1_mdl.hmb"
#include "common/model/GMK_B_OBJ_2_MDL.HMB"
#include "common/model/GMK_B_OBJ_3_MDL.HMB"




// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）


// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_breakobj_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
// -------------

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// -------------

// ----- Definitions -------------------------------------------（定数の宣言）
// ---------------------------------------------------------------------------
// オブジェクトタイプ
//	ゾーンごとにモデルがあるので　＝ゾーン定義「GSE_MAIN_ZONE_TYPE」とする

// 地形矩形テーブル
typedef enum tag_GME_GMK_BOBJ_RECT_DATA{

	GME_GMK_RECT_DATA_COL_WIDTH	= 0,	// 幅
	GME_GMK_RECT_DATA_COL_HEIGHT,		// 高さ
	GME_GMK_RECT_DATA_COL_OFST_X,		// 中心位置からＸ方向のオフセット
	GME_GMK_RECT_DATA_COL_OFST_Y,		// 中心位置からＹ方向のオフセット

	GME_GMK_RECT_DATA_DEF_LEFT,			// 当たり判定矩形　左側
	GME_GMK_RECT_DATA_DEF_TOP,			// 当たり判定矩形　
	GME_GMK_RECT_DATA_DEF_RIGHT,		// 当たり判定矩形　
	GME_GMK_RECT_DATA_DEF_BOTTOM,		// 当たり判定矩形　中心位置からＹ方向のオフセット

	GME_GMK_RECT_DATA_MAX

} GME_GMK_BOBJ_RECT_DATA;
// ---------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	土地当たり矩形大きさ定義
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	岩タイプ
	//		コリジョン矩形
#define		GMD_GMK_BOBJ1_RECT_WIDTH		(36)
#define		GMD_GMK_BOBJ1_RECT_HEIGHT		(23)

#define		GMD_GMK_BOBJ2_RECT_WIDTH		(36)
#define		GMD_GMK_BOBJ2_RECT_HEIGHT		(25)

#define		GMD_GMK_BOBJ3_RECT_WIDTH		(36)
#define		GMD_GMK_BOBJ3_RECT_HEIGHT		(38)

	//		↓当たり矩形拡大マージン
#define		GMD_GMK_BOBJ_RECT_MARGIN		(32)

	//		↓破壊当たり矩形マージン
#define		GMD_GMK_BOBJ1_BREAKRECT_MARGIN_H	(4)		//	左右
#define		GMD_GMK_BOBJ1_BREAKRECT_MARGIN_V	(4)		//	上のみ
/*!　※先にコリジョンに接触しないようにコリジョン矩形より上の数値分大きく
 * 　　被破壊レクトを設定します。
 * 　　また、コリジョン矩形と被破壊矩形は
 * 　　破壊オブジェに隠されたスプリングより先に接触するように調整してあります。
 * 　　スプリングなどの大きさが変更になった場合は先に接触してしまわないよう
 * 　　上記の設定を調整してください。
 */

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const static s16 tbl_gm_gmk_bobj_col_rect[GSD_MAIN_ZONE_TYPE_MAX][GME_GMK_RECT_DATA_MAX] = {

	//	破壊可能オブジェ　ZONE1岩
	{
		//	土地あたり矩形
		  GMD_GMK_BOBJ1_RECT_WIDTH,		//	中心位置からの幅
		  GMD_GMK_BOBJ1_RECT_HEIGHT,	//	中心位置からの高さ
		 -(GMD_GMK_BOBJ1_RECT_WIDTH/2),	//	中心位置から地形あたりの発生する場所までのオフセット
		 -(GMD_GMK_BOBJ1_RECT_HEIGHT),	//	中心位置から地形あたりの発生する場所までのオフセット

		//	破壊あたり矩形
		 -(GMD_GMK_BOBJ1_RECT_WIDTH/2)-GMD_GMK_BOBJ_RECT_MARGIN,    // 左
		 -(GMD_GMK_BOBJ1_RECT_HEIGHT) -GMD_GMK_BOBJ_RECT_MARGIN,    // 上
		 +(GMD_GMK_BOBJ1_RECT_WIDTH/2)+GMD_GMK_BOBJ_RECT_MARGIN,    // 右
		   0                                                         // 下(接地するようなので０にしておく)
	},
	//	破壊可能オブジェ　ZONE2キラキラ岩
	{
		//	土地あたり矩形
		  GMD_GMK_BOBJ2_RECT_WIDTH,		//	中心位置からの幅
		  GMD_GMK_BOBJ2_RECT_HEIGHT,	//	中心位置からの高さ
		 -(GMD_GMK_BOBJ2_RECT_WIDTH/2),	//	中心位置から地形あたりの発生する場所までのオフセット
		 -(GMD_GMK_BOBJ2_RECT_HEIGHT),	//	中心位置から地形あたりの発生する場所までのオフセット

		//	破壊あたり矩形
		 -(GMD_GMK_BOBJ2_RECT_WIDTH/2)-GMD_GMK_BOBJ_RECT_MARGIN,    // 左
		 -(GMD_GMK_BOBJ2_RECT_HEIGHT) -GMD_GMK_BOBJ_RECT_MARGIN,    // 上
		 +(GMD_GMK_BOBJ2_RECT_WIDTH/2)+GMD_GMK_BOBJ_RECT_MARGIN,    // 右
		   0                                                         // 下(接地するようなので０にしておく)
	},

	//	破壊可能オブジェ　ZONE3 岩
	{
		//	土地あたり矩形
		  GMD_GMK_BOBJ3_RECT_WIDTH,		//	中心位置からの幅
		  GMD_GMK_BOBJ3_RECT_HEIGHT,	//	中心位置からの高さ
		 -(GMD_GMK_BOBJ3_RECT_WIDTH/2),	//	中心位置から地形あたりの発生する場所までのオフセット
		 -(GMD_GMK_BOBJ3_RECT_HEIGHT),	//	中心位置から地形あたりの発生する場所までのオフセット

		//	破壊あたり矩形
		 -(GMD_GMK_BOBJ3_RECT_WIDTH/2)-GMD_GMK_BOBJ_RECT_MARGIN,    // 左
		 -(GMD_GMK_BOBJ3_RECT_HEIGHT) -GMD_GMK_BOBJ_RECT_MARGIN,    // 上
		 +(GMD_GMK_BOBJ3_RECT_WIDTH/2)+GMD_GMK_BOBJ_RECT_MARGIN,    // 右
		   0                                                         // 下(接地するようなので０にしておく)
	},


};
// ---------------------------------------------------------------------------
const static u16 tbl_gm_gmk_bobj_act_id[GSD_MAIN_ZONE_TYPE_MAX] =
{
	IDB_GMK_B_OBJ_1_MDL_GMK_B_OBJ_1_ZNO,
	IDB_GMK_B_OBJ_2_MDL_GMK_B_OBJ_2_ZNO,
	IDB_GMK_B_OBJ_3_MDL_GMK_B_OBJ_3_ZNO,
	IDB_GMK_B_OBJ_1_MDL_GMK_B_OBJ_1_ZNO,
	IDB_GMK_B_OBJ_1_MDL_GMK_B_OBJ_1_ZNO,
};
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------





// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef struct tag_GMS_GMK_BOBJ_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	GSE_MAIN_ZONE_TYPE	zone_type;		//!< ゾーン判定

	s16					breakrect_left;		//!< コリジョンの左側
	s16					breakrect_right;		//!< コリジョンの右側
	s16					breakrect_top;		//!< コリジョンの上側

	BOOL				hitpass;		//!< 当たり判定の実行確認
	s16					hitcheck;		//!< 当たり判定の実行段階
	u16					broketype;		//!< 破壊不可能な行動を指定

	u16					vect;			//!< 向き


}GMS_GMK_BOBJ_WORK;
#define		GMD_GMK_BOBJ_HIT_LEFT		(1)
#define		GMD_GMK_BOBJ_HIT_RIGHT		(2)
#define		GMD_GMK_BFLOOR_HIT_TOP		(3)
#define		GMD_GMK_BFLOOR_HIT_BOTTOM	(4)

#define		GMD_GMK_BOBJ_HARD_SPIN_D	(1<<0)
#define		GMD_GMK_BOBJ_HARD_SPIN_J	(1<<1)
#define		GMD_GMK_BOBJ_HARD_DASH		(1<<2)

#define		COMWORK		gmk_work.ene_com
#define		OBJWORK		COMWORK.obj_work




// ===========================================================================
// gmGmkBreakObj*
/*!
	ギミック 破壊可能オブジェ

	@note
		ソニックが乗っかると崩れだす足場です。
		足場の左に乗るか、右に乗るかで崩れるパターンが変わります。
 */
// ---------------------------------------------------------------------------
static void gmGmkBreakObjStay(OBS_OBJECT_WORK *obj_work);

static void gmGmkBreakObjHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);

static void gmGmkBreakObj_CreateParts(OBS_OBJECT_WORK *parent_obj, GSE_MAIN_ZONE_TYPE type, u16 vect);
// ===========================================================================


// ---------------------------------------------------------------------------
// gmGmkBreakObjStart
// gmGmkBreakObjStay
/*!
	ギミック 破壊可能壁サブ初期化
	ギミック 破壊可能壁待機

	@note
		ソニックがぶつかるまで待機
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBreakObjStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BOBJ_WORK *pwork = (GMS_GMK_BOBJ_WORK*)obj_work;
	OBS_RECT_WORK *rect_work;

	//	土地あたり矩形餅オブジェ設定
	//	普段では通り抜けられないように
	pwork->COMWORK.col_work.obj_col.obj       = obj_work;
	// 地形矩形設定
	pwork->COMWORK.col_work.obj_col.width  = (u16)tbl_gm_gmk_bobj_col_rect[pwork->zone_type][GME_GMK_RECT_DATA_COL_WIDTH ];	//	中心位置からの幅
	pwork->COMWORK.col_work.obj_col.height = (u16)tbl_gm_gmk_bobj_col_rect[pwork->zone_type][GME_GMK_RECT_DATA_COL_HEIGHT];	//	中心位置からの高さ
	pwork->COMWORK.col_work.obj_col.ofst_x = tbl_gm_gmk_bobj_col_rect[pwork->zone_type][GME_GMK_RECT_DATA_COL_OFST_X];	//	中心位置から地形あたりの発生する場所までのオフセット
	pwork->COMWORK.col_work.obj_col.ofst_y = tbl_gm_gmk_bobj_col_rect[pwork->zone_type][GME_GMK_RECT_DATA_COL_OFST_Y];	//	中心位置から地形あたりの発生する場所までのオフセット
	pwork->COMWORK.col_work.obj_col.dir = (u16)(0x0000);

	pwork->breakrect_left  = (s16)(pwork->COMWORK.col_work.obj_col.ofst_x -GMD_GMK_BOBJ1_BREAKRECT_MARGIN_H);
	pwork->breakrect_right = (s16)(pwork->breakrect_left + pwork->COMWORK.col_work.obj_col.width +GMD_GMK_BOBJ1_BREAKRECT_MARGIN_H);
	pwork->breakrect_top   = (s16)(pwork->COMWORK.col_work.obj_col.ofst_y -GMD_GMK_BOBJ1_BREAKRECT_MARGIN_V);

	// 矩形設定
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	// 対プレイヤー
	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF];
	rect_work->ppDef = gmGmkBreakObjHit;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 被破壊矩形設定
	ObjRectWorkSet(rect_work,
	               tbl_gm_gmk_bobj_col_rect[pwork->zone_type][GME_GMK_RECT_DATA_DEF_LEFT],
	               tbl_gm_gmk_bobj_col_rect[pwork->zone_type][GME_GMK_RECT_DATA_DEF_TOP],
	               tbl_gm_gmk_bobj_col_rect[pwork->zone_type][GME_GMK_RECT_DATA_DEF_RIGHT],
	               tbl_gm_gmk_bobj_col_rect[pwork->zone_type][GME_GMK_RECT_DATA_DEF_BOTTOM]);
//	rect_work->flag |= OBD_RECT_OUT;

	//	ホーミングチェック
	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = NULL;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 被破壊矩形設定
	ObjRectWorkSet(rect_work,
	               pwork->breakrect_left, 
	               pwork->breakrect_top, 
	               pwork->breakrect_right, 
	               0 );

	// 破壊可能壁固有のメンバ初期化
	pwork->hitpass = FALSE;
	pwork->hitcheck = 0;

	obj_work->ppFunc = gmGmkBreakObjStay;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBreakObjStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BOBJ_WORK *pwork = (GMS_GMK_BOBJ_WORK*)obj_work;

	if( pwork->hitpass == FALSE )
	{
		if (pwork->hitcheck != 0){
			gmGmkBreakObjStart(obj_work);
		}
	}
	pwork->hitpass = FALSE;
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkBreakObjHit
/*!
	ギミック 破壊可能壁 プレイヤー接触

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const u16 tbl_gmk_breakobj_effect[GSD_MAIN_ZONE_TYPE_MAX] =
{
	GME_EFCT_Z01_IDX_OBJECT_Z1,		// GSD_MAIN_ZONE_TYPE_1
	GME_EFCT_Z02_IDX_OBJECT_Z2,		// GSD_MAIN_ZONE_TYPE_2
	GME_EFCT_Z03_IDX_OBJECT_Z3,		// GSD_MAIN_ZONE_TYPE_3
	GME_EFCT_Z01_IDX_OBJECT_Z1,		// GSD_MAIN_ZONE_TYPE_4
	GME_EFCT_Z01_IDX_OBJECT_Z1,		// GSD_MAIN_ZONE_TYPE_FINAL
	GME_EFCT_Z01_IDX_OBJECT_Z1,		// GSD_MAIN_ZONE_TYPE_SS
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBreakObjHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	//	一応プレイヤーが当たったかどうかを確認
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] )
	{
		GMS_GMK_BOBJ_WORK *pwork = (GMS_GMK_BOBJ_WORK*)obj_work;
		OBS_RECT_WORK *rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF];

		if (ply_work->act_state == GME_PLY_ACT_STATE_JUMP_SPIN ||
		        ply_work->act_state == GME_PLY_ACT_STATE_HOMING ||
		        ply_work->player_flag & GMD_PLF_TRUCK_RIDE)
		{
			s16	l,r,t,b;	// Left,Right,Top,Bottom
			u16 vect;
			l = r = t = b = 0;
			//	矩形再設定
			vect = 0x0000;
			if( obj_work->pos.x >= ply_work->obj_work.pos.x )
			{
				//	Sonic→|岩|
				//	岩の左側を補修
				l = (s16)((obj_work->pos.x>>FX32_SHIFT)+rect_work->rect.left
				           - match_rect->rect.right);
				l = (s16)((ply_work->obj_work.pos.x>>FX32_SHIFT)-l);
				pwork->hitcheck = 1;
			}
			else
			{
				//	|岩|←Sonic
				//	岩の右側を補修
				r = (s16)((obj_work->pos.x>>FX32_SHIFT)+rect_work->rect.right
				           - match_rect->rect.left  -(ply_work->obj_work.pos.x>>FX32_SHIFT));
				pwork->hitcheck = 1;
				vect = 0x8000;
			}
			if( obj_work->pos.y >= ply_work->obj_work.pos.y )
			{
				//	Sonic
				//	  ↓
				//	 |岩|
				//	岩の上側を補修
					t = (s16)((obj_work->pos.y>>FX32_SHIFT)+rect_work->rect.top
					           - match_rect->rect.bottom);
					t = (s16)((ply_work->obj_work.pos.y>>FX32_SHIFT)-t);
				pwork->hitcheck = 1;
			}

			if( rect_work->rect.right > pwork->breakrect_right )
			{
				rect_work->rect.right -= r;	//!< 右側を縮める
				if( rect_work->rect.right < pwork->breakrect_right )
					rect_work->rect.right = pwork->breakrect_right;
			}
			if( rect_work->rect.left < pwork->breakrect_left )
			{
				rect_work->rect.left  += l;	//!< 左側を縮める
				if( rect_work->rect.left > pwork->breakrect_left )
					rect_work->rect.left = pwork->breakrect_left;
			}
			if( rect_work->rect.top < pwork->breakrect_top )
			{
				rect_work->rect.top   += t;	//!< 上側を縮める
				if( rect_work->rect.top > pwork->breakrect_top )
					rect_work->rect.top = pwork->breakrect_top;
			}
			pwork->hitpass = TRUE;

			s16	h,v;
			h = (s16)((ply_work->obj_work.spd.x)>>FX32_SHIFT);
			v = (s16)((ply_work->obj_work.spd.y)>>FX32_SHIFT);

			/*	［矩形の右側から速度を足した(向かってくるなら－値)結果が破壊矩形より小さい　か
				　矩形の左側へ速度を足した(向かってくるなら＋値)結果が破壊矩形より大きい］で
				　矩形の上部へ速度を足した(向かってくるなら＋値)結果が破壊矩形より大きい　なら */
			if ( ((rect_work->rect.right + h <= pwork->breakrect_right) ||
			    	(rect_work->rect.left + h >= pwork->breakrect_left ))&&
			    		rect_work->rect.top + v >= pwork->breakrect_top   )
			{
				u16 model = tbl_gmk_breakobj_effect[pwork->zone_type];
				switch( pwork->zone_type )
				{
					case GSD_MAIN_ZONE_TYPE_1:
						//	夕焼けあり
						if( g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3 )
							model = GME_EFCT_Z01_IDX_OBJECT_Z1_A3;
						break;

					case GSD_MAIN_ZONE_TYPE_3:
						//	水中あり
						if( g_gm_main_system.water_level != 0xffff &&
						       g_gm_main_system.water_level < ((obj_work->pos.y>>FX32_SHIFT)+pwork->breakrect_top) )
							model = GME_EFCT_Z03_IDX_OBJECT_W_Z3;
						break;

					case GSD_MAIN_ZONE_TYPE_2:
					case GSD_MAIN_ZONE_TYPE_4:
					case GSD_MAIN_ZONE_TYPE_FINAL:
					case GSD_MAIN_ZONE_TYPE_SS:
					default:
						break;
				}
				OBS_OBJECT_WORK *effobj_work = 
						(OBS_OBJECT_WORK*)GmEfctZoneEsCreate(NULL, pwork->zone_type, model );
				effobj_work->pos.x = obj_work->pos.x;
				effobj_work->pos.y = obj_work->pos.y;
				effobj_work->pos.z = obj_work->pos.z;

				gmGmkBreakObj_CreateParts(obj_work, pwork->zone_type, vect);
				pwork->COMWORK.col_work.obj_col.obj = NULL;
				GmEnemyDefaultDefFunc( mine_rect, match_rect);
		//		pwork->hitcheck = -pwork->hitcheck;	//	到達
				// SE
				GmSoundPlaySE("BreakOBJ");
				// スコア加算
				GmPlayerAddScore(ply_work, GMD_PLY_SCORE_BREAK_OBJ, obj_work->pos.x, obj_work->pos.y);
				return;
			}
		}
	}
	// ヒットしなかった事にする
	//ObjRectFuncNoHit(mine_rect, match_rect);	// 20090918 Dimps Ishizaki
}
// ---------------------------------------------------------------------------
// ===========================================================================



// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// gmGmkBreakObjParts
/*!
	ギミック 破壊可能オブジェ　破壊演出パーツ

	@note
		ソニックがぶつかると砕け散る破片たち。

 */
// ---------------------------------------------------------------------------
typedef struct tag_GMS_GMK_BOBJ_PARTS
{
	GMS_EFFECT_3DNN_WORK	eff_work;

	s16						falltimer;

}GMS_GMK_BOBJ_PARTS;
// ---------------------------------------------------------------------------
static void gmGmkBreakObjParts_Main(OBS_OBJECT_WORK *obj_work);
// ---------------------------------------------------------------------------
static void gmGmkBreakObjParts_Main(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BOBJ_PARTS *pwork = (GMS_GMK_BOBJ_PARTS*)obj_work;

	obj_work->dir.z += 0x200;

	pwork->falltimer -= 1;
	if( pwork->falltimer <= 0 )
	{
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
	}
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// gmGmkBreakObj_CreateParts
/*!
	ギミック 破壊可能オブジェ　破壊演出パーツ生成

	@note
		ソニックがぶつかると砕け散る破片の生成

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef enum tag_GME_GMK_BOBJ_PARTS_PARAMS
{
	GME_GMK_BOBJ_PARTS_ACT_ID = 0,
	GME_GMK_BOBJ_PARTS_OFF_X,
	GME_GMK_BOBJ_PARTS_OFF_Y,
	GME_GMK_BOBJ_PARTS_VECT,
	GME_GMK_BOBJ_PARTS_SPD,

	GME_GMK_BOBJ_PARTS_MAX

}GME_GMK_BOBJ_PARTS_PARAMS;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const u16 tbl_gm_gmk_bobj1_parts[][GME_GMK_BOBJ_PARTS_MAX] =
{
	{ IDB_GMK_B_OBJ_1_MDL_GMK_B_OBJ_1_P1_ZNO, (u16)-16,  (u16)+16, (0xd000),(u16)(3*FX16_ONE) },
	{ IDB_GMK_B_OBJ_1_MDL_GMK_B_OBJ_1_P1_ZNO, (u16)  0,  (u16)+20, (0xc000),(u16)(3*FX16_ONE) },
	{ IDB_GMK_B_OBJ_1_MDL_GMK_B_OBJ_1_P1_ZNO, (u16)+16,  (u16)+16, (0xb000),(u16)(3*FX16_ONE) },
	{ IDB_GMK_B_OBJ_1_MDL_GMK_B_OBJ_1_P2_ZNO, (u16)-12,  (u16)+24, (0xd000),(u16)(4*FX16_ONE) },
	{ IDB_GMK_B_OBJ_1_MDL_GMK_B_OBJ_1_P2_ZNO, (u16)  0,  (u16)+28, (0xc000),(u16)(4*FX16_ONE) },
	{ IDB_GMK_B_OBJ_1_MDL_GMK_B_OBJ_1_P2_ZNO, (u16)+12,  (u16)+24, (0xb000),(u16)(4*FX16_ONE) },
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const u16 tbl_gm_gmk_bobj2_parts[][GME_GMK_BOBJ_PARTS_MAX] =
{
	{ IDB_GMK_B_OBJ_2_MDL_GMK_B_OBJ_2_P1_ZNO, (u16)-16,  (u16)+16, (0xd000),(u16)(3*FX16_ONE) },
	{ IDB_GMK_B_OBJ_2_MDL_GMK_B_OBJ_2_P2_ZNO, (u16)  0,  (u16)+20, (0xc000),(u16)(3*FX16_ONE) },
	{ IDB_GMK_B_OBJ_2_MDL_GMK_B_OBJ_2_P1_ZNO, (u16)+16,  (u16)+16, (0xb000),(u16)(3*FX16_ONE) },
	{ IDB_GMK_B_OBJ_2_MDL_GMK_B_OBJ_2_P2_ZNO, (u16)-12,  (u16)+24, (0xd000),(u16)(4*FX16_ONE) },
	{ IDB_GMK_B_OBJ_2_MDL_GMK_B_OBJ_2_P1_ZNO, (u16)  0,  (u16)+28, (0xc000),(u16)(4*FX16_ONE) },
	{ IDB_GMK_B_OBJ_2_MDL_GMK_B_OBJ_2_P2_ZNO, (u16)+12,  (u16)+24, (0xb000),(u16)(4*FX16_ONE) },
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const u16 tbl_gm_gmk_bobj3_parts[][GME_GMK_BOBJ_PARTS_MAX] =
{
	{ IDB_GMK_B_OBJ_3_MDL_GMK_B_OBJ_3_P3_ZNO, (u16)-16,  (u16)+16, (0xd000),(u16)(3*FX16_ONE) },
	{ IDB_GMK_B_OBJ_3_MDL_GMK_B_OBJ_3_P2_ZNO, (u16)  0,  (u16)+20, (0xc000),(u16)(3*FX16_ONE) },
	{ IDB_GMK_B_OBJ_3_MDL_GMK_B_OBJ_3_P3_ZNO, (u16)+16,  (u16)+16, (0xb000),(u16)(3*FX16_ONE) },
	{ IDB_GMK_B_OBJ_3_MDL_GMK_B_OBJ_3_P2_ZNO, (u16)-12,  (u16)+24, (0xd000),(u16)(4*FX16_ONE) },
	{ IDB_GMK_B_OBJ_3_MDL_GMK_B_OBJ_3_P1_ZNO, (u16)  0,  (u16)+28, (0xc000),(u16)(4*FX16_ONE) },
	{ IDB_GMK_B_OBJ_3_MDL_GMK_B_OBJ_3_P2_ZNO, (u16)+12,  (u16)+24, (0xb000),(u16)(4*FX16_ONE) },
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static struct tag_GMS_GMK_BOBJ_PARTS_PARAM_TABLE
{
	u16	*params;
	u16	num;

} GMS_GMK_BOBJ_PARTS_PARAM_TABLE[] =
{
	//	GSD_MAIN_ZONE_TYPE_1
		{ (u16*)&tbl_gm_gmk_bobj1_parts[0][0], ((sizeof(tbl_gm_gmk_bobj1_parts)/GME_GMK_BOBJ_PARTS_MAX)/sizeof(u16)) },
	//	GSD_MAIN_ZONE_TYPE_2
		{ (u16*)&tbl_gm_gmk_bobj2_parts[0][0], ((sizeof(tbl_gm_gmk_bobj2_parts)/GME_GMK_BOBJ_PARTS_MAX)/sizeof(u16)) },
	//	GSD_MAIN_ZONE_TYPE_3
		{ (u16*)&tbl_gm_gmk_bobj3_parts[0][0], ((sizeof(tbl_gm_gmk_bobj3_parts)/GME_GMK_BOBJ_PARTS_MAX)/sizeof(u16)) },
};
// ---------------------------------------------------------------------------
static void gmGmkBreakObj_CreateParts(OBS_OBJECT_WORK *parent_obj, GSE_MAIN_ZONE_TYPE type, u16 vect)
{
	/*
	 *	破壊されたオブジェの破片
	 *
	 *	
	 *	
	 *	
	 *	
	 *	
	 *	
	 *	
	 *	
	 *	
	 */

	u16	*ppara;
	int	i;
	ppara = GMS_GMK_BOBJ_PARTS_PARAM_TABLE[type].params;
	for (i= 0; i < GMS_GMK_BOBJ_PARTS_PARAM_TABLE[type].num; i++ )
	{
		GMS_GMK_BOBJ_PARTS *pwork =
			(GMS_GMK_BOBJ_PARTS*)GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_BOBJ_PARTS),
			                                             NULL,
			                                             0,
			                                             "BreakObj_Parts");
		OBS_OBJECT_WORK *obj_work = (OBS_OBJECT_WORK*)pwork;

		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_breakobj_obj_3d_list[ppara[GME_GMK_BOBJ_PARTS_ACT_ID]],
		                             &pwork->eff_work.obj_3d);
#if 1
		// かくつきが無くなる・・・
		((NNS_MATERIAL_GLES11_DESC*)(obj_work->obj_3d->object->pMatPtrList->pMaterial))->fFlag = NND_MATFLAG_DISABLE_LIGHTING;
#endif // 0
		obj_work->pos.x = parent_obj->pos.x + (fx32)((s16)ppara[GME_GMK_BOBJ_PARTS_OFF_X]*FX32_ONE);
		obj_work->pos.y = parent_obj->pos.y - (fx32)((s16)ppara[GME_GMK_BOBJ_PARTS_OFF_Y]*FX32_ONE);
		obj_work->pos.z = parent_obj->pos.z + 1;	//	少し手前に

		vect = ppara[GME_GMK_BOBJ_PARTS_VECT];		//	飛んでいく方向のベクトル
		fx32 spd = -((fx32)ppara[GME_GMK_BOBJ_PARTS_SPD]);	//	飛んでいくスピード

		obj_work->spd.x = +((mtMathCos(vect)*spd)>>FX32_SHIFT);
		obj_work->spd.y = -((mtMathSin(vect)*spd)>>FX32_SHIFT);
		obj_work->spd_add.y = 0x0400;	//	仮

		obj_work->dir.x = 0;
		obj_work->dir.y = 0;
		obj_work->dir.z = vect;
		// フラグ
		obj_work->move_flag |= OBD_MOVE_NOCOL;						// 地形あたり無し
		obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
		obj_work->disp_flag &= ~OBD_DISP_NODIR;
		obj_work->flag |= OBD_OBJECT_NOHIT;							// 矩形あたり無し◆

		pwork->falltimer = 60;

		ppara += GME_GMK_BOBJ_PARTS_MAX;
		obj_work->ppFunc = gmGmkBreakObjParts_Main;
	}
}
// ===========================================================================




// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ===========================================================================
// GmGmkBreakObjInit
/*!
	ギミック 破壊可能オブジェ 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// ===========================================================================
//	破壊オブジェ　生成に関する定義

// ---------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkBreakObjInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_BOBJ_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;


	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_BOBJ_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_BOBJ_WORK), "GMK_BREAK_LAND_MAIN");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_breakobj_obj_3d_list[IDB_GMK_B_OBJ_1_MDL_GMK_B_OBJ_1_ZNO],
	                             &gmk_work->obj_3d);
	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE|OBD_MOVE_NOCOL;			// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// ライト設定
#if _WII
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_1 ||
			g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_2) {
		gmk_work->obj_3d.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
		gmk_work->obj_3d.use_light_flag |= OBD_LIGHT_USE_FLAG_5;
	}
#endif

	//	ゾーン区別
	pwork->zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	// あたり矩形などの設定と処理のスタート
	gmGmkBreakObjStart(obj_work);
	return obj_work;
}
// ===========================================================================



// ===========================================================================
// GmGmkBreakObjBuild
/*!
	ギミック 破壊可能オブジェ データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkBreakObjBuild(void)
{
	gm_gmk_breakobj_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_B_OBJ_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_B_OBJ_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkBreakLandFlush
/*!
	ギミック 破壊可能オブジェ データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkBreakObjFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_B_OBJ_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_breakobj_obj_3d_list, amb->file_num);
}
// ===========================================================================
