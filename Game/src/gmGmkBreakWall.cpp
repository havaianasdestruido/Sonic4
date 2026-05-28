// ===========================================================================
/*!
	@file	gmGmkBreakWall.cpp
	@brief	ギミック 破壊可能な壁

	@author	ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkBreakWall.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

/* note
 *		横破壊型の壁は、そのグラフィックの意匠よりかなり大きめに被破壊判定矩形を有します。
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
 *		縦破壊型の床や天井も同じアルゴリズムで、
 *		迫るソニックにあわせて矩形を縮め、矩形幅が意匠に見合ったところで破壊が成立します。
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
#include "gmPadVib.h"

#include "gmSound.h"

#include "gmGmkBreakWall.h"

// データヘッダ
#include "common/model/gmk_b_wall1_mdl.hmb"
#include "common/model/GMK_B_WALL2_MDL.HMB"
#include "common/model/GMK_B_WALL3_MDL.HMB"
#include "common/model/GMK_B_WALL4_MDL.HMB"




// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static fx32 gmk_bwall_effect_y;	//	初期化不要(ランダムの代わりだから)

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_breakwall_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
// -------------

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// -------------

// ----- Definitions -------------------------------------------（定数の宣言）
// ---------------------------------------------------------------------------
// オブジェクトタイプ
typedef enum tag_GME_GMK_TYPE{

	GME_GMK_TYPE_BREAK_WALL = 0,	//
	GME_GMK_TYPE_BREAK_WALL_H,		// 横タイプ壁 20091006 Dimps Ishizaki
	GME_GMK_TYPE_BREAK_FLOOR,		//

	GME_GMK_TYPE_MAX

}GME_GMK_TYPE;
#define	GMD_GMK_TYPE_WALL		(0x00)
#define	GMD_GMK_TYPE_FLOOR		(0x01)
#define	GMM_GMK_TYPE_CHECK(gmk)			((gmk<GME_GMK_TYPE_BREAK_FLOOR)?GMD_GMK_TYPE_WALL:GMD_GMK_TYPE_FLOOR)
#define	GMM_GMK_TYPE_IS_WALL(gmk)		(GMM_GMK_TYPE_CHECK(gmk) == GMD_GMK_TYPE_WALL)
#define	GMM_GMK_TYPE_IS_VECT(gmk)		(gmk == GME_GMK_TYPE_BREAK_WALL ? TRUE : FALSE) // 091006 Dimps Ishizaki
typedef enum tag_GME_GMK_PARTS_TYPE{

	GME_GMK_WALL_TYPE_L1 = 0,
	GME_GMK_WALL_TYPE_L2,
	GME_GMK_WALL_TYPE_R1,
	GME_GMK_WALL_TYPE_R2,
	GME_GMK_WALL_TYPE_C1,
	GME_GMK_WALL_TYPE_C2,
	GME_GMK_WALL_TYPE_FLOOR,
	GME_GMK_WALL_TYPE_C1_H,	// 091006 Dimps Ishizaki

	GME_GMK_WALL_TYPE_MAX

}GME_GMK_WALL_TYPE;



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
//	壁タイプ　当たり矩形拡大マージン
#define		GMD_GMK_BWALL_H_RECT_MARGIN		(32)
#define		GMD_GMK_BWALL_H_RECT_W			(32)

#define		GMD_GMK_BFLOOR_V_RECT_MARGIN	(32)
#define		GMD_GMK_BFLOOR_V_RECT_H			(32)
const static s16 tbl_gm_gmk_bwall_col_rect[GME_GMK_TYPE_MAX][GME_GMK_RECT_DATA_MAX] = {

	//	破壊可能壁１
	{
		//	土地あたり矩形
		  32,	//	中心位置からの幅
		  64,	//	中心位置からの高さ
		 -16,	//	中心位置から地形あたりの発生する場所までのオフセット
		 -64,	//	中心位置から地形あたりの発生する場所までのオフセット

		//	破壊あたり矩形
		 -(GMD_GMK_BWALL_H_RECT_W/2)-GMD_GMK_BWALL_H_RECT_MARGIN,    //	左	//	土地矩形より一回り大きくとっておく
		 -64,                                                      //	上	//	速度が速いと先に土地矩形にヒットしソニックが止まってしまう。
		 +(GMD_GMK_BWALL_H_RECT_W/2)+GMD_GMK_BWALL_H_RECT_MARGIN,    //	右	//	ヒット時にソニックの速度と座標を確認し、
		   0                                                       //	下	//	届かないようであればヒットを無効にする。
	},

	// 091006 Dimps Ishizaki
	//	破壊可能壁１ 水平
	{
		//	土地あたり矩形
		  64,	//	中心位置からの幅
		  32,	//	中心位置からの高さ
		 -64,	//	中心位置から地形あたりの発生する場所までのオフセット
		 -16,	//	中心位置から地形あたりの発生する場所までのオフセット

		//	破壊あたり矩形
		 -64,                                                          // 左 // 土地矩形より一回り大きくとっておく
		 -(GMD_GMK_BFLOOR_V_RECT_H/2)-GMD_GMK_BFLOOR_V_RECT_MARGIN,    // 上 // 速度が速いと先に土地矩形にヒットしソニックが止まってしまう。
		 +0,                                                           // 右 // ヒット時にソニックの速度と座標を確認し、
		 +(GMD_GMK_BFLOOR_V_RECT_H/2)+GMD_GMK_BFLOOR_V_RECT_MARGIN,    // 下 //	届かないようであればヒットを無効にする。
	},

	//	破壊可能床１
	{
		//	土地あたり矩形
		  64,	//	中心位置からの幅
		  32,	//	中心位置からの高さ
		 -32,	//	中心位置から地形あたりの発生する場所までのオフセット
		 -16,	//	中心位置から地形あたりの発生する場所までのオフセット

		//	破壊あたり矩形
		 -32,                                                          // 左 // 土地矩形より一回り大きくとっておく
		 -(GMD_GMK_BFLOOR_V_RECT_H/2)-GMD_GMK_BFLOOR_V_RECT_MARGIN,    // 上 // 速度が速いと先に土地矩形にヒットしソニックが止まってしまう。
		 +32,                                                          // 右 // ヒット時にソニックの速度と座標を確認し、
		 +(GMD_GMK_BFLOOR_V_RECT_H/2)+GMD_GMK_BFLOOR_V_RECT_MARGIN,    // 下 //	届かないようであればヒットを無効にする。
	},

};
#define		GMD_GMK_BWALL_H_BREAK_SPD	(g_gm_player_parameter[GSD_MAIN_PLAYER_1P].spd_max_spin/4)
		//	スピンダッシュ破壊型オブジェクトの最低必要速度 1/4
// ---------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	モデルテーブル
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const u16 tbl_breakwall_mdl[GSD_MAIN_ZONE_TYPE_MAX][GME_GMK_WALL_TYPE_MAX] =
{
	//	ゾーン１
	{
		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_L1_V_ZNO,
		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_L2_V_ZNO,
		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_R1_V_ZNO,
		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_R2_V_ZNO,
		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_V_ZNO,
		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_F_V_ZNO,
		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_H_ZNO,// 20090731 dimps Ishizaki
		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_V_ZNO,// 20091006 dimps Ishizaki
	},
	//	ゾーン２
	{
		IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_V_ZNO,
		IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_V_ZNO,
		IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_V_ZNO,
		IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_V_ZNO,
		IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_V_ZNO,
		IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_V_ZNO,
		IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_H_ZNO,
		IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_V_ZNO,// 20091006 dimps Ishizaki
	},
	//	ゾーン３
	{
		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_ZNO,
		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_ZNO,
		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_ZNO,
		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_ZNO,
		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_ZNO,
		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_ZNO,
		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_ZNO,
		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_ZNO,// 20091006 dimps Ishizaki
	},

	//	ゾーン４
	{
		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_V_ZNO,
		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_V_ZNO,
		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_V_ZNO,
		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_V_ZNO,
		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_V_ZNO,
		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_V_ZNO,
		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_H_ZNO,
		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_V_ZNO,
	},

};
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------





// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef struct tag_GMS_GMK_BWALL_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	GME_GMK_TYPE		obj_type;		//!< 壁か床かオブジェクトタイプ
	GME_GMK_WALL_TYPE	wall_type;		//!< 壁か床かのうちでもさらにどの部品か

	BOOL				hitpass;		//!< 当たり判定の実行確認
	s16					hitcheck;		//!< 当たり判定の実行段階
	u16					broketype;		//!< 破壊不可能な行動を指定

	u16					vect;			//!< 向き


}GMS_GMK_BWALL_WORK;
#define		GMD_GMK_BWALL_HIT_LEFT		(1)
#define		GMD_GMK_BWALL_HIT_RIGHT		(2)
#define		GMD_GMK_BFLOOR_HIT_TOP		(3)
#define		GMD_GMK_BFLOOR_HIT_BOTTOM	(4)

#define		GMD_GMK_BWALL_HARD_SPIN_D	(1<<0)
#define		GMD_GMK_BWALL_HARD_SPIN_J	(1<<1)
#define		GMD_GMK_BWALL_HARD_DASH		(1<<2)

#define		GMD_GMK_BFLOOR_HARD_CANNON	(1<<0)

#define		COMWORK		gmk_work.ene_com
#define		OBJWORK		COMWORK.obj_work




// ===========================================================================
// gmGmkBreakWall*
/*!
	ギミック 破壊可能壁床

	@note
		ソニックが乗っかると崩れだす足場です。
		足場の左に乗るか、右に乗るかで崩れるパターンが変わります。
 */
// ---------------------------------------------------------------------------
static void gmGmkBreakWallStay(OBS_OBJECT_WORK *obj_work);

static void gmGmkBreakWallHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);

static void gmGmkBreakWall_CreateParts(OBS_OBJECT_WORK *parent_obj, GME_GMK_WALL_TYPE type, GME_GMK_TYPE obj_type/*091006 Dimps Ishizaki*/, u16 vect);
// ===========================================================================


// ---------------------------------------------------------------------------
// gmGmkBreakWallStart
// gmGmkBreakWallStay
/*!
	ギミック 破壊可能壁サブ初期化
	ギミック 破壊可能壁待機

	@note
		ソニックがぶつかるまで待機
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBreakWallStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BWALL_WORK *pwork = (GMS_GMK_BWALL_WORK*)obj_work;
	OBS_RECT_WORK *rect_work;

	//	土地あたり矩形餅オブジェ設定
	//	普段では通り抜けられないように
	pwork->COMWORK.col_work.obj_col.obj       = obj_work;
	// 地形矩形設定
	pwork->COMWORK.col_work.obj_col.width  = (u16)tbl_gm_gmk_bwall_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_COL_WIDTH ];	//	中心位置からの幅
	pwork->COMWORK.col_work.obj_col.height = (u16)tbl_gm_gmk_bwall_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_COL_HEIGHT];	//	中心位置からの高さ
	pwork->COMWORK.col_work.obj_col.ofst_x = tbl_gm_gmk_bwall_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_COL_OFST_X];	//	中心位置から地形あたりの発生する場所までのオフセット
	pwork->COMWORK.col_work.obj_col.ofst_y = tbl_gm_gmk_bwall_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_COL_OFST_Y];	//	中心位置から地形あたりの発生する場所までのオフセット

	pwork->COMWORK.col_work.obj_col.dir = (u16)(0x0000);

	// 矩形設定
	// 対プレイヤー
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = gmGmkBreakWallHit;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 被破壊矩形設定
	ObjRectWorkSet(rect_work,
	               tbl_gm_gmk_bwall_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_LEFT],
	               tbl_gm_gmk_bwall_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_TOP],
	               tbl_gm_gmk_bwall_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_RIGHT],
	               tbl_gm_gmk_bwall_col_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_BOTTOM]);
//	rect_work->flag |= OBD_RECT_OUT;

	// 破壊可能壁固有のメンバ初期化
	pwork->hitpass = FALSE;
	pwork->hitcheck = 0;

	obj_work->ppFunc = gmGmkBreakWallStay;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBreakWallStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BWALL_WORK *pwork = (GMS_GMK_BWALL_WORK*)obj_work;

	//	当たっていた状態から当たらない状態へ変移

	if (pwork->hitcheck < 0){

		u16 vect = (u16)((pwork->hitcheck&0x01)? 0x0000:0x8000);
		OBS_OBJECT_WORK *effobj_work = NULL;
		//	破壊成立
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST|OBD_OBJECT_NOHIT;
		// SE
		GmSoundPlaySE("BreakWall");
		// 振動
		GMM_PAD_VIB_SMALL();

		//	破片ビルド
		gmGmkBreakWall_CreateParts(obj_work, pwork->wall_type, pwork->obj_type, vect );

		//	エフェクトビルド
		if( gmk_bwall_effect_y > 48*FX32_ONE )
		{
			while( gmk_bwall_effect_y > 16*FX32_ONE )
				gmk_bwall_effect_y -= 13*FX32_ONE;
		}
#if _IPHONE
		fx32 obj_z = obj_work->pos.z;
#endif // _IPHONE
		// 20090731 dimps Ishizaki
		// エフェクトをゾーン別に生成
		switch (g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id]) {
		case GSD_MAIN_ZONE_TYPE_1:
			effobj_work = 
				(OBS_OBJECT_WORK*)GmEfctZoneEsCreate(NULL, GSD_MAIN_ZONE_TYPE_1, GME_EFCT_Z01_IDX_WALL_Z1);	
			break;
		case GSD_MAIN_ZONE_TYPE_2:
			effobj_work = 
				(OBS_OBJECT_WORK*)GmEfctZoneEsCreate(NULL, GSD_MAIN_ZONE_TYPE_2, GME_EFCT_Z02_IDX_WALL_Z2);	
			break;
		case GSD_MAIN_ZONE_TYPE_3:
			effobj_work = 
				(OBS_OBJECT_WORK*)GmEfctZoneEsCreate(NULL, GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_WALL_Z3);	
#if _IPHONE
				if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
					obj_z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;
					effobj_work->obj_3des->command_state = OBD_DRAW_CMD_STATE_3DNN_PRE;
				}
#endif // _IPHONE
			break;
		case GSD_MAIN_ZONE_TYPE_4:
			effobj_work = 
				(OBS_OBJECT_WORK*)GmEfctZoneEsCreate(NULL, GSD_MAIN_ZONE_TYPE_4, GME_EFCT_Z04_IDX_WALL_Z4);	
			break;
		case GSD_MAIN_ZONE_TYPE_FINAL:
		default:
			break;
		}
		if (effobj_work) {
			effobj_work->pos.x = obj_work->pos.x;
			effobj_work->pos.y = obj_work->pos.y-gmk_bwall_effect_y;
#if _IPHONE
			effobj_work->pos.z = obj_z;
#else
			effobj_work->pos.z = obj_work->pos.z;
#endif // _IPHONE
			gmk_bwall_effect_y += 31*FX32_ONE;
			effobj_work->dir.z = vect;
			effobj_work->disp_flag &= ~OBD_DISP_NODIR;
		}
/*
		effobj_work->spd.x = (vect==0x0000)? +8*FX32_ONE:-8*FX32_ONE;
		effobj_work->spd_add.x = -(effobj_work->spd.x/32);
		effobj_work->spd.y = -4*FX32_ONE;
		effobj_work->spd_add.y = (4*FX32_ONE/32);
		effobj_work->move_flag &= ~OBD_MOVE_NOMOVE;
*/
		return;
	}

	if( pwork->hitpass == FALSE )
	{
		if (pwork->hitcheck != 0){
			gmGmkBreakWallStart(obj_work);
		}
	}
	pwork->hitpass = FALSE;
}



// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// gmGmkBreakWallHit
/*!
	ギミック 破壊可能壁 プレイヤー接触

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBreakWallHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	//	ゾーンに応じて破壊に至るかどうかの判定を変えます
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	//	一応プレイヤーが当たったかどうかを確認
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] )
	{
		GMS_GMK_BWALL_WORK *pwork = (GMS_GMK_BWALL_WORK*)obj_work;
		OBS_RECT_WORK *rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];

		//	縦横確認
		switch( GMM_GMK_TYPE_CHECK(pwork->obj_type) )
		{
			case GMD_GMK_TYPE_WALL:
			//	壁タイプ
				if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {	// 091006 Dimps Ishizaki
					// トロッコ時
					// 一定速度以下で壊れない
					if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < ply_work->spd3 &&
							MTM_MATH_ABS(ply_work->obj_work.spd.x) < ply_work->spd3) {
						break;
					}
				}
				else if( ply_work->act_state == GME_PLY_ACT_STATE_SPIN_DASH ||
				    ply_work->act_state == GME_PLY_ACT_STATE_SPIN_ACC ||
				    ply_work->act_state == GME_PLY_ACT_STATE_SPIN ||
					ply_work->act_state == GME_PLY_ACT_STATE_SPIN_SMALL )
				{
					if( pwork->broketype & GMD_GMK_BWALL_HARD_SPIN_D )	//	スピンダッシュで壊れない
						break;

					if( ply_work->act_state != GME_PLY_ACT_STATE_SPIN &&
						ply_work->act_state != GME_PLY_ACT_STATE_SPIN_SMALL)
					{
						//	ソニックがスピンダッシュ(まだ発射前)なら破壊準備でコリジョン消去
						pwork->COMWORK.col_work.obj_col.obj = NULL;
						pwork->hitcheck = 1;
						pwork->hitpass = TRUE;
					}
					else 
					//else if( ply_work->act_state == GME_PLY_ACT_STATE_SPIN )
					{
						//	ソニックがスピンダッシュなら破壊準備
						//	しかしながら速度が規定以下に落ちていた場合は破壊はできない
						if( MTM_MATH_ABS(ply_work->obj_work.spd_m) < GMD_GMK_BWALL_H_BREAK_SPD )
							break;
					}
				}
				else if( ply_work->act_state == GME_PLY_ACT_STATE_JUMP_SPIN )
				{
					if( pwork->broketype & GMD_GMK_BWALL_HARD_SPIN_J )	//	ダッシュでは壊れない
						break;
				}
				else if( ply_work->act_state == GME_PLY_ACT_STATE_DASH_2 ||
				         ply_work->act_state == GME_PLY_ACT_STATE_DASH_2 ||
				         ply_work->act_state == GME_PLY_ACT_STATE_DASH_1 )
				{
					if( pwork->broketype & GMD_GMK_BWALL_HARD_DASH )	//	ダッシュでは壊れない
						break;
				}
				else
					break;

				//	矩形再設定 091006 Dimps Ishizaki
				if (GMM_GMK_TYPE_IS_VECT(pwork->obj_type)) {
					// 縦向き
					if( obj_work->pos.x >= ply_work->obj_work.pos.x )
					{
						//	Sonic→|壁|
						//	壁の左側を補修
						s16 l = (s16)((obj_work->pos.x>>FX32_SHIFT)+rect_work->rect.left
								- match_rect->rect.right);
						l = (s16)((ply_work->obj_work.pos.x>>FX32_SHIFT)-l);
						rect_work->rect.left  += l;		//!< 左側を縮める
						pwork->hitcheck = GMD_GMK_BWALL_HIT_LEFT;
					}
					else
					{
						//	|壁|←Sonic
						//	壁の右側を補修
						s16 r = (s16)((obj_work->pos.x>>FX32_SHIFT)+rect_work->rect.right
								- match_rect->rect.left  -(ply_work->obj_work.pos.x>>FX32_SHIFT));
						rect_work->rect.right -= r;		//!< 右側を縮める
						pwork->hitcheck = GMD_GMK_BWALL_HIT_RIGHT;
					}
					pwork->COMWORK.col_work.obj_col.obj = NULL;
					pwork->hitpass = TRUE;

					if( rect_work->rect.left  >= -GMD_GMK_BWALL_H_RECT_W/2 ||
						rect_work->rect.right <=  GMD_GMK_BWALL_H_RECT_W/2 ){
						//	絵の幅に達したか？
						pwork->hitcheck = (s16)(-pwork->hitcheck);	//	到達
						return;
					}
				}
				else {
					// 横向き
					if( obj_work->pos.y >= ply_work->obj_work.pos.y )
					{
						//	Sonic
						//	  ↓
						//	 |床|
						//	床の上側を補修
						s16 top = (s16)((obj_work->pos.y>>FX32_SHIFT)+rect_work->rect.top
										 - match_rect->rect.bottom);
						top = (s16)((ply_work->obj_work.pos.y>>FX32_SHIFT)-top);
						rect_work->rect.top += top;		//!< 上側を縮める
						pwork->hitcheck = GMD_GMK_BFLOOR_HIT_TOP;
					}
					else
					{
						//	 |天|
						//	  ↑
						//	Sonic
						//	壁の下側を補修
						s16 btm = (s16)((obj_work->pos.y>>FX32_SHIFT)+rect_work->rect.bottom
										- match_rect->rect.top);
						btm -= (s16)(ply_work->obj_work.pos.y>>FX32_SHIFT);
						rect_work->rect.bottom -= btm;		//!< 右側を縮める
						pwork->hitcheck = GMD_GMK_BFLOOR_HIT_BOTTOM;
					}
					pwork->COMWORK.col_work.obj_col.obj = NULL;
					pwork->hitpass = TRUE;

					if( rect_work->rect.top  >= -GMD_GMK_BFLOOR_V_RECT_H/2 ||
						rect_work->rect.bottom <=  GMD_GMK_BFLOOR_V_RECT_H/2 ){
						//	絵の幅に達したか？
						pwork->hitcheck = (s16)(-pwork->hitcheck);	//	到達
						return;
					}
				}
				break;

			case GMD_GMK_TYPE_FLOOR:
			//	床タイプ
				if( ((pwork->broketype & GMD_GMK_BFLOOR_HARD_CANNON) && ply_work->act_state != GME_PLY_ACT_STATE_GMK_CANNON_SHOOT)||
				    ((pwork->broketype & GMD_GMK_BFLOOR_HARD_CANNON) && ply_work->act_state == GME_PLY_ACT_STATE_GMK_CANNON_SHOOT && ply_work->obj_work.spd.y > 0) )
					break;	//	キャノン破壊タイプなのにキャノン状態でなかったらアウト
							//	キャノン破壊タイプでキャノン状態なんだけど落下中だったらアウト

				if( (ply_work->act_state != GME_PLY_ACT_STATE_JUMP_SPIN && !(pwork->broketype & GMD_GMK_BFLOOR_HARD_CANNON))||
				      (ply_work->obj_work.pos.y <= obj_work->pos.y && ply_work->obj_work.spd.y <= 0) ||
				      (ply_work->obj_work.pos.y >= obj_work->pos.y && ply_work->obj_work.spd.y >= 0) )
					break;	//	スピンジャンプでなく上から下降中でもなく下から上昇中でもなければアウト

				//	ソニックがスピンジャンプで接近中なら破壊準備
				//	矩形再設定
				if( obj_work->pos.y >= ply_work->obj_work.pos.y )
				{
					//	Sonic
					//	  ↓
					//	 |床|
					//	床の上側を補修
					s16 top = (s16)((obj_work->pos.y>>FX32_SHIFT)+rect_work->rect.top
					                 - match_rect->rect.bottom);
					top = (s16)((ply_work->obj_work.pos.y>>FX32_SHIFT)-top);
					rect_work->rect.top += top;		//!< 上側を縮める
					pwork->hitcheck = GMD_GMK_BFLOOR_HIT_TOP;
				}
				else
				{
					//	 |天|
					//	  ↑
					//	Sonic
					//	壁の下側を補修
					s16 btm = (s16)((obj_work->pos.y>>FX32_SHIFT)+rect_work->rect.bottom
					                - match_rect->rect.top);
					btm -= (s16)(ply_work->obj_work.pos.y>>FX32_SHIFT);
					rect_work->rect.bottom -= btm;		//!< 右側を縮める
					pwork->hitcheck = GMD_GMK_BFLOOR_HIT_BOTTOM;
				}
				pwork->COMWORK.col_work.obj_col.obj = NULL;
				pwork->hitpass = TRUE;

				if( rect_work->rect.top  >= -GMD_GMK_BFLOOR_V_RECT_H/2 ||
				    rect_work->rect.bottom <=  GMD_GMK_BFLOOR_V_RECT_H/2 ){
					//	絵の幅に達したか？
					pwork->hitcheck = (s16)(-pwork->hitcheck);	//	到達
					return;
				}
				break;
		}
	}
	// ヒットしなかった事にする
	//ObjRectFuncNoHit(mine_rect, match_rect);	// 20090918 Dimps Ishizaki
}
// ---------------------------------------------------------------------------
// ===========================================================================



// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// gmGmkBreakWallParts
/*!
	ギミック 破壊可能壁　破壊演出パーツ

	@note
		ソニックがぶつかると砕け散る破片たち。

 */
// ---------------------------------------------------------------------------
typedef struct tag_GMS_GMK_BWALL_PARTS
{
	GMS_EFFECT_3DNN_WORK	eff_work;

	s16						falltimer;
	u16						vect;

}GMS_GMK_BWALL_PARTS;
// ---------------------------------------------------------------------------
static void gmGmkBreakLandParts_Main(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BWALL_PARTS *pwork = (GMS_GMK_BWALL_PARTS*)obj_work;

	if( pwork->vect > 0x8000 )
	{
		obj_work->dir.x += 0x400;
		obj_work->dir.y += 0x300;
	}
	else
	{
		obj_work->dir.x -= 0x400;
		obj_work->dir.y -= 0x300;
	}

	pwork->falltimer -= 1;
	if( pwork->falltimer <= 0 )
	{
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
	}
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// gmGmkBreakWall_CreateParts
/*!
	ギミック 破壊可能壁　破壊演出パーツ生成

	@note
		ソニックがぶつかると砕け散る破片の生成

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef enum tag_GME_GMK_BWALL_PARTS_PARAMS
{
	GME_GMK_BWALL_PARTS_ACT_ID = 0,
	GME_GMK_BWALL_PARTS_OFF_X,
	GME_GMK_BWALL_PARTS_OFF_Y,
	GME_GMK_BWALL_PARTS_VECT,
	GME_GMK_BWALL_PARTS_VECT_Z,
	GME_GMK_BWALL_PARTS_SPEED,

	GME_GMK_BWALL_PARTS_MAX

}GME_GMK_BWALL_PARTS_PARAMS;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define		GMD_GMK_B_WALL1_PARTS_LARGE		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_V_P01_ZNO
#define		GMD_GMK_B_WALL1_PARTS_SMALL1		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_V_P02_ZNO
#define		GMD_GMK_B_WALL1_PARTS_SMALL2		IDB_GMK_B_WALL1_MDL_GMK_B_WALL1_V_P03_ZNO
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static const u16 tbl_gm_gmk_bwall1_1_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
	//	モデル名                  // Ｘオフセット
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16) -10, (u16)-50, (0xf100), (0xf800), 6 },//0
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16)  -8, (u16)-38, (0xfc00), (0xf800), 7 },//2
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16)  -8, (u16)-28, (0x0400), (0xf800), 8 },//4
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16)  +8, (u16)-14, (0x0c00), (0xf800), 5 },//6

	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16) -10, (u16)-54, (0xd000), (0x0800), 7 },//0	// ■□　■□
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16)  -8, (u16)-42, (0xd800), (0x0800), 6 },//2	// ■□　■□
	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16)  -8, (u16)-24, (0x1800), (0x0800), 7 },//4	// ■□　■□
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16)  +8, (u16)-10, (0x2000), (0x0800), 6 },//6	// ■□　■□

	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16)   0, (u16)-54, (0xb800), (0xf800), 8 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16)   0, (u16)-38, (0xb000), (0xf800), 8 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16)   0, (u16)-30, (0x5000), (0xf800), 8 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16)   0, (u16)-14, (0x4800), (0xf800), 8 },
};
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static const u16 tbl_gm_gmk_bwall1_2_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
	//	モデル名                  // Ｘオフセット
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16)  +8, (u16)-52, (0xe400), (0xf800), 5 },//1	// □０　□１↑56
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16) +10, (u16)-36, (0xf000), (0xf800), 8 },//3	// □２　□３↑40
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16) +10, (u16)-30, (0x1000), (0xf800), 7 },//5	// □４　□５↑24
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16) -10, (u16)-12, (0x2400), (0xf800), 6 },//7	// □６　□７↑ 8

	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16)  +8, (u16)-56, (0xe000), (0x0800), 5 },//1	// □０　□１↑56
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16) +10, (u16)-40, (0xf000), (0x0800), 5 },//3	// □２　□３↑40
	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16) +10, (u16)-26, (0x1000), (0x0800), 5 },//5	// □４　□５↑24
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16) -10, (u16)- 8, (0x3000), (0x0800), 5 },//7	// □６　□７↑ 8

	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16)   0, (u16)-54, (0xb000), (0xf800), 8 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16)   0, (u16)-38, (0xa800), (0xf800), 8 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16)   0, (u16)-30, (0x5800), (0xf800), 8 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16)   0, (u16)-14, (0x5000), (0xf800), 8 },
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const u16 tbl_gm_gmk_bfloor1_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
//	 ■□ ■□  ■□ ■□  -8
//	 □■ □■  □■ □■    
//	          ┼             
//	 ■□ ■□  ■□ ■□    
//	 □■ □■  □■ □■  +8
//    -24  -8     +8  +24
	//	上からヒット基準のデータ設定
	//								         x,        y,  xvect     zvect   speed
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16) +24, (u16)+ 4, (0x4100), (0x0e00), 4 },
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16)  +8, (u16)- 2, (0x4080), (0x0600), 5 },
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16)  -8, (u16)+ 2, (0x3f80), (0x7200), 6 },
	{ GMD_GMK_B_WALL1_PARTS_LARGE,   (u16) -24, (u16)- 4, (0x3f00), (0x7a00), 5 },

	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16)  +4, (u16) -8, (0x4800), (0x1000), 6 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16) +16, (u16)-12, (0x4a00), (0x0800), 7 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16) +24, (u16)+10, (0x4c00), (0x0400), 5 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16) +20, (u16)+16, (0x4e00), (0x0C00), 7 },

	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16)  -4, (u16) -8, (0x5800), (0x7000), 5 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16) -16, (u16)-12, (0x5600), (0x7800), 6 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL1,  (u16) -24, (u16)+10, (0x5400), (0x7c00), 7 },
	{ GMD_GMK_B_WALL1_PARTS_SMALL2,  (u16) -20, (u16)+16, (0x5200), (0x7400), 5 },
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define		GMD_GMK_B_WALL2_PARTS_S		IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_P00_ZNO
#define		GMD_GMK_B_WALL2_PARTS_L		IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_P01_ZNO
#define		GMD_GMK_B_WALL2_PARTS_L2	IDB_GMK_B_WALL2_MDL_GMK_B_WALL2_P02_ZNO
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static const u16 tbl_gm_gmk_bwall2_1_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
	//	モデル名                  // Ｘオフセット
	{ GMD_GMK_B_WALL2_PARTS_S,  (u16)   0, (u16)-50, (0xf100), (0xf800), 6 },//0
//	{ GMD_GMK_B_WALL2_PARTS_S,  (u16)  -8, (u16)-38, (0xfc00), (0xf800), 7 },//2
	{ GMD_GMK_B_WALL2_PARTS_S,  (u16)  +8, (u16)-28, (0x0400), (0xf800), 8 },//4

	{ GMD_GMK_B_WALL2_PARTS_L,  (u16)  +8, (u16)-14, (0x0c00), (0xf800), 5 },//6
	{ GMD_GMK_B_WALL2_PARTS_L,  (u16) -10, (u16)-54, (0xd000), (0x0800), 7 },//0	// ■□　■□

	{ GMD_GMK_B_WALL2_PARTS_L2, (u16)   0, (u16)-42, (0xd800), (0x0800), 6 },//2	// ■□　■□
//	{ GMD_GMK_B_WALL2_PARTS_L2, (u16)  -8, (u16)-24, (0x1800), (0x0800), 7 },//4	// ■□　■□
	{ GMD_GMK_B_WALL2_PARTS_L2, (u16)  +8, (u16)-10, (0x2000), (0x0800), 6 },//6	// ■□　■□
};
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static const u16 tbl_gm_gmk_bwall2_2_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
	//	モデル名                  // Ｘオフセット
	{ GMD_GMK_B_WALL2_PARTS_S,  (u16)  -8, (u16)-52, (0xe400), (0xf800), 5 },//1	// □０　□１↑56
//	{ GMD_GMK_B_WALL2_PARTS_S,  (u16) +10, (u16)-36, (0xf000), (0xf800), 8 },//3	// □２　□３↑40
	{ GMD_GMK_B_WALL2_PARTS_S,  (u16) +10, (u16)-30, (0x1000), (0xf800), 7 },//5	// □４　□５↑24

	{ GMD_GMK_B_WALL2_PARTS_L,  (u16) -10, (u16)-12, (0x2400), (0xf800), 6 },//7	// □６　□７↑ 8
	{ GMD_GMK_B_WALL2_PARTS_L,  (u16)  +8, (u16)-56, (0xe000), (0x0800), 5 },//1	// □０　□１↑56

	{ GMD_GMK_B_WALL2_PARTS_L2, (u16) +10, (u16)-40, (0xf000), (0x0800), 5 },//3	// □２　□３↑40
//	{ GMD_GMK_B_WALL2_PARTS_L2, (u16) +10, (u16)-26, (0x1000), (0x0800), 5 },//5	// □４　□５↑24
	{ GMD_GMK_B_WALL2_PARTS_L2, (u16) -10, (u16)- 8, (0x3000), (0x0800), 5 },//7	// □６　□７↑ 8
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const u16 tbl_gm_gmk_bfloor2_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
//	 ■□ ■□  ■□ ■□  -8
//	 □■ □■  □■ □■    
//	          ┼             
//	 ■□ ■□  ■□ ■□    
//	 □■ □■  □■ □■  +8
//    -24  -8     +8  +24
	//	上からヒット基準のデータ設定
	//								         x,        y,  xvect     zvect   speed
	{ GMD_GMK_B_WALL2_PARTS_S,  (u16)  +8, (u16)-52, (0xe400), (0xf800), 5 },//1	// □０　□１↑56
//	{ GMD_GMK_B_WALL2_PARTS_S,  (u16) +10, (u16)-36, (0xf000), (0xf800), 8 },//3	// □２　□３↑40
	{ GMD_GMK_B_WALL2_PARTS_S,  (u16) +10, (u16)-30, (0x1000), (0xf800), 7 },//5	// □４　□５↑24

	{ GMD_GMK_B_WALL2_PARTS_L,  (u16) -10, (u16)-12, (0x2400), (0xf800), 6 },//7	// □６　□７↑ 8
	{ GMD_GMK_B_WALL2_PARTS_L,  (u16)  +8, (u16)-56, (0xe000), (0x0800), 5 },//1	// □０　□１↑56

	{ GMD_GMK_B_WALL2_PARTS_L2, (u16) +10, (u16)-40, (0xf000), (0x0800), 5 },//3	// □２　□３↑40
//	{ GMD_GMK_B_WALL2_PARTS_L2, (u16) +10, (u16)-26, (0x1000), (0x0800), 5 },//5	// □４　□５↑24
	{ GMD_GMK_B_WALL2_PARTS_L2, (u16) -10, (u16)- 8, (0x3000), (0x0800), 5 },//7	// □６　□７↑ 8
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define		GMD_GMK_B_WALL3_PARTS_1		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_P1_ZNO
#define		GMD_GMK_B_WALL3_PARTS_2		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_P2_ZNO
#define		GMD_GMK_B_WALL3_PARTS_3		IDB_GMK_B_WALL3_MDL_GMK_B_WALL_3_P3_ZNO
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static const u16 tbl_gm_gmk_bwall3_1_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
	//	モデル名                  // Ｘオフセット
	{ GMD_GMK_B_WALL3_PARTS_1,   (u16) -10, (u16)-50, (0xf100), (0xf800), 6 },//0
	{ GMD_GMK_B_WALL3_PARTS_1,   (u16)  -8, (u16)-38, (0xfc00), (0xf800), 7 },//2
	{ GMD_GMK_B_WALL3_PARTS_1,   (u16)  -8, (u16)-28, (0x0400), (0xf800), 8 },//4
	{ GMD_GMK_B_WALL3_PARTS_1,   (u16)  +8, (u16)-14, (0x0c00), (0xf800), 5 },//6

	{ GMD_GMK_B_WALL3_PARTS_2,  (u16) -10, (u16)-54, (0xd000), (0x0800), 7 },//0	// ■□　■□
	{ GMD_GMK_B_WALL3_PARTS_3,  (u16)  -8, (u16)-42, (0xd800), (0x0800), 6 },//2	// ■□　■□
	{ GMD_GMK_B_WALL3_PARTS_2,  (u16)  -8, (u16)-24, (0x1800), (0x0800), 7 },//4	// ■□　■□
	{ GMD_GMK_B_WALL3_PARTS_3,  (u16)  +8, (u16)-10, (0x2000), (0x0800), 6 },//6	// ■□　■□

	{ GMD_GMK_B_WALL3_PARTS_2,  (u16)   0, (u16)-54, (0xb800), (0xf800), 8 },
	{ GMD_GMK_B_WALL3_PARTS_3,  (u16)   0, (u16)-38, (0xb000), (0xf800), 8 },
	{ GMD_GMK_B_WALL3_PARTS_2,  (u16)   0, (u16)-30, (0x5000), (0xf800), 8 },
	{ GMD_GMK_B_WALL3_PARTS_3,  (u16)   0, (u16)-14, (0x4800), (0xf800), 8 },
};
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static const u16 tbl_gm_gmk_bwall3_2_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
	//	モデル名                  // Ｘオフセット
	{ GMD_GMK_B_WALL3_PARTS_1,  (u16)  +8, (u16)-52, (0xe400), (0xf800), 5 },//1	// □０　□１↑56
	{ GMD_GMK_B_WALL3_PARTS_1,  (u16) +10, (u16)-36, (0xf000), (0xf800), 8 },//3	// □２　□３↑40
	{ GMD_GMK_B_WALL3_PARTS_1,  (u16) +10, (u16)-30, (0x1000), (0xf800), 7 },//5	// □４　□５↑24
	{ GMD_GMK_B_WALL3_PARTS_1,  (u16) -10, (u16)-12, (0x2400), (0xf800), 6 },//7	// □６　□７↑ 8

	{ GMD_GMK_B_WALL3_PARTS_2,  (u16)  +8, (u16)-56, (0xe000), (0x0800), 5 },//1	// □０　□１↑56
	{ GMD_GMK_B_WALL3_PARTS_3,  (u16) +10, (u16)-40, (0xf000), (0x0800), 5 },//3	// □２　□３↑40
	{ GMD_GMK_B_WALL3_PARTS_2,  (u16) +10, (u16)-26, (0x1000), (0x0800), 5 },//5	// □４　□５↑24
	{ GMD_GMK_B_WALL3_PARTS_3,  (u16) -10, (u16)- 8, (0x3000), (0x0800), 5 },//7	// □６　□７↑ 8

	{ GMD_GMK_B_WALL3_PARTS_2,  (u16)   0, (u16)-54, (0xb000), (0xf800), 8 },
	{ GMD_GMK_B_WALL3_PARTS_3,  (u16)   0, (u16)-38, (0xa800), (0xf800), 8 },
	{ GMD_GMK_B_WALL3_PARTS_2,  (u16)   0, (u16)-30, (0x5800), (0xf800), 8 },
	{ GMD_GMK_B_WALL3_PARTS_3,  (u16)   0, (u16)-14, (0x5000), (0xf800), 8 },
};
// 091006 Dimps Ishizaki
static const u16 tbl_gm_gmk_bwall3_1_h_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
	//								     x,        y,  xvect     zvect   speed
	{ GMD_GMK_B_WALL3_PARTS_1,   (u16) -52, (u16)- 8, (0x4100), (0x0e00), 5 },
	{ GMD_GMK_B_WALL3_PARTS_1,   (u16) -36, (u16)-10, (0x4080), (0x0600), 8 },
	{ GMD_GMK_B_WALL3_PARTS_1,   (u16) -30, (u16)-10, (0x3f80), (0x7200), 7 },
	{ GMD_GMK_B_WALL3_PARTS_1,   (u16) -12, (u16)+10, (0x3f00), (0x7a00), 6 },

	{ GMD_GMK_B_WALL3_PARTS_2,   (u16) -56, (u16) -8, (0x4800), (0x1000), 5 },
	{ GMD_GMK_B_WALL3_PARTS_3,   (u16) -40, (u16)-10, (0x4a00), (0x0800), 5 },
	{ GMD_GMK_B_WALL3_PARTS_2,   (u16) -26, (u16)-10, (0x4c00), (0x0400), 5 },
	{ GMD_GMK_B_WALL3_PARTS_3,   (u16) - 8, (u16)+10, (0x4e00), (0x0C00), 5 },

	{ GMD_GMK_B_WALL3_PARTS_2,   (u16) -54, (u16)  0, (0x5800), (0x7000), 8 },
	{ GMD_GMK_B_WALL3_PARTS_3,   (u16) -38, (u16)  0, (0x5600), (0x7800), 8 },
	{ GMD_GMK_B_WALL3_PARTS_2,   (u16) -30, (u16)  0, (0x5400), (0x7c00), 8 },
	{ GMD_GMK_B_WALL3_PARTS_3,   (u16) -14, (u16)  0, (0x5200), (0x7400), 8 },
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define		GMD_GMK_B_WALL4_PARTS_1		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_P01_ZNO
#define		GMD_GMK_B_WALL4_PARTS_2		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_P02_ZNO
#define		GMD_GMK_B_WALL4_PARTS_3		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_P03_ZNO
#define		GMD_GMK_B_WALL4_PARTS_4		IDB_GMK_B_WALL4_MDL_GMK_B_WALL4_P04_ZNO
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static const u16 tbl_gm_gmk_bwall4_1_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
	//	モデル名                  // Ｘオフセット
	{ GMD_GMK_B_WALL4_PARTS_1,  (u16)   0, (u16)-50, (0xf100), (0xf800), 6 },//0

	{ GMD_GMK_B_WALL4_PARTS_2,  (u16) -12, (u16)-54, (0xfc00), (0xf800), 7 },//2
	{ GMD_GMK_B_WALL4_PARTS_2,  (u16)  +8, (u16)-12, (0x0400), (0xf800), 8 },//4

	{ GMD_GMK_B_WALL4_PARTS_3,  (u16)  +8, (u16)-12, (0x0c00), (0xf800), 5 },//6
	{ GMD_GMK_B_WALL4_PARTS_3,  (u16) -10, (u16)-16, (0xd000), (0x0800), 7 },//0	// ■□　■□

	{ GMD_GMK_B_WALL4_PARTS_4,  (u16)   0, (u16)-42, (0xd800), (0x0800), 6 },//2	// ■□　■□
	{ GMD_GMK_B_WALL4_PARTS_4,  (u16)  -8, (u16)-24, (0x1800), (0x0800), 7 },//4	// ■□　■□
	{ GMD_GMK_B_WALL4_PARTS_4,  (u16)  +8, (u16)-10, (0x2000), (0x0800), 6 },//6	// ■□　■□
};
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static const u16 tbl_gm_gmk_bwall4_2_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
	//	モデル名                  // Ｘオフセット
	{ GMD_GMK_B_WALL4_PARTS_1,  (u16)   0, (u16)-50, (0xc200), (0xf800), 5 },//1	// □０　□１↑56
                                
	{ GMD_GMK_B_WALL4_PARTS_2,  (u16) -12, (u16)-54, (0xc100), (0xf800), 8 },//3	// □２　□３↑40
	{ GMD_GMK_B_WALL4_PARTS_2,  (u16)  +8, (u16)-12, (0x3e00), (0xf800), 7 },//5	// □４　□５↑24
                                
	{ GMD_GMK_B_WALL4_PARTS_3,  (u16)  +8, (u16)-12, (0x2400), (0xf800), 6 },//7	// □６　□７↑ 8
	{ GMD_GMK_B_WALL4_PARTS_3,  (u16) -10, (u16)-16, (0xe000), (0x0800), 5 },//1	// □０　□１↑56
                                
	{ GMD_GMK_B_WALL4_PARTS_4,  (u16)   0, (u16)-42, (0xf000), (0x0800), 5 },//3	// □２　□３↑40
	{ GMD_GMK_B_WALL4_PARTS_4,  (u16)  -8, (u16)-24, (0x1000), (0x0800), 5 },//5	// □４　□５↑24
	{ GMD_GMK_B_WALL4_PARTS_4,  (u16)  +8, (u16)-10, (0x3000), (0x0800), 5 },//7	// □６　□７↑ 8
};
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static const u16 tbl_gm_gmk_bwall4_h_parts[][GME_GMK_BWALL_PARTS_MAX] =
{
	//	モデル名                  // Ｘオフセット
	{ GMD_GMK_B_WALL4_PARTS_1,  (u16)  +8, (u16)  0, (0x4100), (0x0e00), 5 },//1	// □０　□１↑56

	{ GMD_GMK_B_WALL4_PARTS_2,  (u16) -12, (u16)  0, (0x3f80), (0x7200), 8 },//3	// □２　□３↑40
	{ GMD_GMK_B_WALL4_PARTS_2,  (u16)  +8, (u16)-12, (0x3f00), (0x7a00), 7 },//5	// □４　□５↑24

	{ GMD_GMK_B_WALL4_PARTS_3,  (u16) -24, (u16) -8, (0x4800), (0x1000), 6 },//7	// □６　□７↑ 8
	{ GMD_GMK_B_WALL4_PARTS_3,  (u16) +28, (u16)-10, (0x4a00), (0x0800), 5 },//1	// □０　□１↑56

	{ GMD_GMK_B_WALL4_PARTS_4,  (u16)   0, (u16)  0, (0x4e00), (0x0C00), 5 },//3	// □２　□３↑40
	{ GMD_GMK_B_WALL4_PARTS_4,  (u16)  -8, (u16)-12, (0x5800), (0x7000), 5 },//5	// □４　□５↑24
	{ GMD_GMK_B_WALL4_PARTS_4,  (u16)  +8, (u16)- 4, (0x5600), (0x7800), 5 },//7	// □６　□７↑ 8
};                                                   
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef struct tag_GMS_GMK_BWALL_PARTS_PARAM_TABLE
{
	u16	*params;
	u16	num;

}GMS_GMK_BWALL_PARTS_PARAM_TABLE;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static GMS_GMK_BWALL_PARTS_PARAM_TABLE tbl_gmk_bwall_parts_z1[GME_GMK_WALL_TYPE_MAX] =
{
	{ (u16*)&tbl_gm_gmk_bwall1_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall1_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall1_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall1_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall1_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall1_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall1_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall1_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall1_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall1_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall1_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall1_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bfloor1_parts[0][0],   ((sizeof(tbl_gm_gmk_bfloor1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bfloor1_parts[0][0],   ((sizeof(tbl_gm_gmk_bfloor1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },// 20091006 dimps Ishizaki
};
static GMS_GMK_BWALL_PARTS_PARAM_TABLE tbl_gmk_bwall_parts_z2[GME_GMK_WALL_TYPE_MAX] =
{
	{ (u16*)&tbl_gm_gmk_bwall2_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall2_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall2_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall2_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall2_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall2_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall2_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall2_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall2_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall2_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall2_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall2_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bfloor2_parts[0][0],  ((sizeof(tbl_gm_gmk_bfloor2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bfloor2_parts[0][0],  ((sizeof(tbl_gm_gmk_bfloor2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },// 20091006 dimps Ishizaki
};
static GMS_GMK_BWALL_PARTS_PARAM_TABLE tbl_gmk_bwall_parts_z3[GME_GMK_WALL_TYPE_MAX] =
{
	{ (u16*)&tbl_gm_gmk_bwall3_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall3_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall3_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall3_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall3_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall3_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall3_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall3_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall3_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall3_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall3_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall3_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)NULL,                             ((sizeof(tbl_gm_gmk_bwall3_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall3_1_h_parts[0][0], ((sizeof(tbl_gm_gmk_bwall3_1_h_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },// 20091006 dimps Ishizaki
};
static GMS_GMK_BWALL_PARTS_PARAM_TABLE tbl_gmk_bwall_parts_z4[GME_GMK_WALL_TYPE_MAX] =
{
	{ (u16*)&tbl_gm_gmk_bwall4_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall4_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall4_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall4_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall4_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall4_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall4_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall4_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall4_1_parts[0][0], ((sizeof(tbl_gm_gmk_bwall4_1_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall4_2_parts[0][0], ((sizeof(tbl_gm_gmk_bwall4_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)&tbl_gm_gmk_bwall4_h_parts[0][0], ((sizeof(tbl_gm_gmk_bwall4_h_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
	{ (u16*)NULL,                             ((sizeof(tbl_gm_gmk_bwall4_2_parts)/GME_GMK_BWALL_PARTS_MAX)/sizeof(u16)) },
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static GMS_GMK_BWALL_PARTS_PARAM_TABLE *tbl_gmk_bwall_parts[GSD_MAIN_ZONE_TYPE_MAX-1] =
{
	tbl_gmk_bwall_parts_z1,		//	GSD_MAIN_ZONE_TYPE_1
	tbl_gmk_bwall_parts_z2,		//	GSD_MAIN_ZONE_TYPE_2
	tbl_gmk_bwall_parts_z3,		//	GSD_MAIN_ZONE_TYPE_3
	tbl_gmk_bwall_parts_z4,		//	GSD_MAIN_ZONE_TYPE_4
};
// ---------------------------------------------------------------------------
static void gmGmkBreakWall_CreateParts(OBS_OBJECT_WORK *parent_obj, GME_GMK_WALL_TYPE type, GME_GMK_TYPE obj_type, u16 vect)
{
	/*
	 *	破壊された壁の破片
	 *
	 *	ゾーン１壁(2*4)   大大   56
	 *                     小12  48
	 *	　　　　　　　　　 大    40
	 *                     小12  32
	 *	　　　　　　　　　大大   24
	 *                     小12  16
	 *	　　　　　  HIT→  大    _8
	 *	                          0
	 */
	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	//	ゾーン区別
	u16 zone = (u16)g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];

	u16	*ppara;
	int	i;
	u16 rvect = (u16)(mtMathRand()%0x2000);
	ppara = tbl_gmk_bwall_parts[zone][type].params;
	for (i= 0; i < tbl_gmk_bwall_parts[zone][type].num; i++ )
	{
		GMS_GMK_BWALL_PARTS *pwork =
			(GMS_GMK_BWALL_PARTS*)GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_BWALL_PARTS),
			                                             NULL,
			                                             0,
			                                             "BreakWall_Parts");
		OBS_OBJECT_WORK *obj_work = (OBS_OBJECT_WORK*)pwork;

		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_breakwall_obj_3d_list[ppara[GME_GMK_BWALL_PARTS_ACT_ID]],
		                             &pwork->eff_work.obj_3d);

#if _WII
		// ライト設定
		if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_1 ||
				g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_2) {
			pwork->eff_work.obj_3d.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
			pwork->eff_work.obj_3d.use_light_flag |= OBD_LIGHT_USE_FLAG_5;
		}
#endif
		u16 spd = ppara[GME_GMK_BWALL_PARTS_SPEED];
		u16 xvect = (u16)(ppara[GME_GMK_BWALL_PARTS_VECT]);			//	飛んでいく方向のベクトル縦(y)と横(x+z)
		u16 zvect = (u16)(ppara[GME_GMK_BWALL_PARTS_VECT_Z]+rvect/2);	//	飛んでいく方向のベクトル横(xとz)

		//if( GMM_GMK_TYPE_CHECK(obj_type) == GMD_GMK_TYPE_WALL )
		if( GMM_GMK_TYPE_IS_VECT(obj_type) )	// 091006 Dimps Ishizaki
		{
			if( xvect >= 0x8000 )
				xvect += rvect;
			else
				xvect -= rvect;

			if( !vect )
			{
				obj_work->pos.x = parent_obj->pos.x + (fx32)((s16)ppara[GME_GMK_BWALL_PARTS_OFF_X]*FX32_ONE);
				obj_work->dir.z = 0;
			}
			else
			{
				obj_work->pos.x = parent_obj->pos.x - (fx32)((s16)ppara[GME_GMK_BWALL_PARTS_OFF_X]*FX32_ONE);
				obj_work->dir.z = 0x8000;
				xvect = (u16)(0x8000-xvect);
			}
			pwork->vect = xvect;
			obj_work->pos.y = parent_obj->pos.y + (fx32)((s16)ppara[GME_GMK_BWALL_PARTS_OFF_Y]*FX32_ONE);

			/*
			GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

			fx32 spd;
			if( GMM_GMK_TYPE_IS_WALL(type) )
				spd = MTM_MATH_ABS(ply_work->obj_work.move.x);
			else
				spd = MTM_MATH_ABS(ply_work->obj_work.move.y)*4;

			if( spd >= 6*FX32_ONE ){
				//	速度が６以上なら８に纏める
				spd = 8*FX32_ONE;
			}
			else if( spd >= 5*FX32_ONE ){
				//	速度が６未満５以上なら６に
				spd = 6*FX32_ONE;
			//	spd = (spd-(5*FX32_ONE))*2+6*FX32_ONE;
			}
			else if( spd >= 4*FX32_ONE ){
				//	速度が５未満４以上なら５に
				spd = 4*FX32_ONE;
			//	spd = (spd-(4*FX32_ONE))*2+4*FX32_ONE;
			}
			else if( spd >= 3*FX32_ONE ){
				//	速度が４未満３以上なら４未満～３に
			//	spd = (spd-(3*FX32_ONE))*2+3*FX32_ONE;
			}
			else
				spd = 2*FX32_ONE;

			obj_work->spd.x = +((mtMathCos(xvect)*spd)>>FX32_SHIFT);
			obj_work->spd.y = +((mtMathSin(xvect)*spd)>>FX32_SHIFT);
			obj_work->spd_add.y = 0x100;	//	仮
			*/

			u16 zvect = (u16)(ppara[GME_GMK_BWALL_PARTS_VECT_Z]+rvect/2);	//	飛んでいく方向のベクトル
			fx32 xzspd      = ((mtMathCos(xvect)*spd));
			obj_work->spd.y = ((mtMathSin(xvect)*spd));
			obj_work->spd.x = ((mtMathCos(zvect)*xzspd)>>FX32_SHIFT);
			obj_work->spd.z = -((mtMathSin(zvect)*MTM_MATH_ABS(xzspd))>>FX32_SHIFT);
			obj_work->pos.z = parent_obj->pos.z+(mtMathSin(zvect)* 8);	//	少し手前に
			obj_work->spd.x += (ply_work->obj_work.move.x >> 1);
		}
		else
		{
			if( xvect >= 0xc000 )
				xvect += rvect;
			else
				xvect -= rvect;

			obj_work->pos.x = parent_obj->pos.x + (fx32)((s16)ppara[GME_GMK_BWALL_PARTS_OFF_X]*FX32_ONE);
			if( !vect )
			{
				obj_work->pos.y = parent_obj->pos.y + (fx32)((s16)ppara[GME_GMK_BWALL_PARTS_OFF_Y]*FX32_ONE);
				obj_work->dir.z = 0;
			}
			else
			{
				obj_work->pos.y = parent_obj->pos.y - (fx32)((s16)ppara[GME_GMK_BWALL_PARTS_OFF_Y]*FX32_ONE);
				obj_work->dir.z = 0x8000;
				xvect = (u16)(0x10000-xvect);
			}
			pwork->vect = xvect;

			fx32 xzspd      = ((mtMathCos(xvect)*spd));
			obj_work->spd.y = ((mtMathSin(xvect)*spd));
			obj_work->spd.x = ((mtMathCos(zvect)*xzspd)>>FX32_SHIFT);
			obj_work->spd.z = -((mtMathSin(zvect)*MTM_MATH_ABS(xzspd))>>FX32_SHIFT);
			obj_work->pos.z = parent_obj->pos.z+(mtMathSin(zvect)* 8);	//	少し手前に
		}
		obj_work->spd_add.y = 0x180;	//	仮

		obj_work->dir.x = 0;
		obj_work->dir.z = 0;
		// フラグ
		obj_work->move_flag |= OBD_MOVE_NOCOL;						// 地形あたり無し
		obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
		obj_work->disp_flag &= ~OBD_DISP_NODIR;
		obj_work->flag |= OBD_OBJECT_NOHIT;							// 矩形あたり無し◆

		if( obj_work->spd.y < 0 )
			pwork->falltimer = 90;
		else
			pwork->falltimer = 120;

		ppara += GME_GMK_BWALL_PARTS_MAX;
		obj_work->ppFunc = gmGmkBreakLandParts_Main;
#if _IPHONE
		// ライト設定
		obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
		obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_1;
#endif // _IPHONE
	}
}
// ===========================================================================




// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ===========================================================================
// GmGmkBreakWallInit
/*!
	ギミック 破壊可能壁 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// ===========================================================================
//	破壊壁　生成に関する定義
#define GMD_GMK_BWALL_EVE_FLAG_NO_BREAK		(0x0007)	//!< 破壊不可
// ---------------------------------------------------------------------------
static OBS_OBJECT_WORK* gmGmkBreakWallInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type, GME_GMK_WALL_TYPE wall)
{
	GMS_GMK_BWALL_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_BWALL_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_BWALL_WORK), "GMK_BREAK_LAND_MAIN");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	u16 model = tbl_breakwall_mdl[g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id]][wall];
	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_breakwall_obj_3d_list[model],
	                             &gmk_work->obj_3d);
	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE|OBD_MOVE_NOCOL;			// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	gmk_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOPRESSDIE;		// 圧死なし 091006 Dimps Ishizaki

	// 破壊属性
	pwork->broketype = (u16)(eve_rec->flag & GMD_GMK_BWALL_EVE_FLAG_NO_BREAK);

	// ギミックタイプ 091006 Dimps Ishizaki
	if (eve_rec->id == GMD_EVENT_ID_BREAKWALL1_C_H) {
		pwork->obj_type = GME_GMK_TYPE_BREAK_WALL_H;
	}
	else {
		pwork->obj_type = GME_GMK_TYPE_BREAK_WALL;
	}
	pwork->wall_type = wall;

#if _WII
	// ライト設定
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_1 ||
			g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_2) {
		gmk_work->obj_3d.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
		gmk_work->obj_3d.use_light_flag |= OBD_LIGHT_USE_FLAG_5;
	}
#endif
#if _IPHONE
	// ライト設定
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3 ||
		g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS) {
		gmk_work->obj_3d.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
		gmk_work->obj_3d.use_light_flag |= OBD_LIGHT_USE_FLAG_1;
	}
#endif // _IPHONE
	return obj_work;
}
// ---------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkBreakWall_L1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_BWALL_WORK	*pwork;
	pwork = (GMS_GMK_BWALL_WORK*)gmGmkBreakWallInit(eve_rec, pos_x, pos_y, type, GME_GMK_WALL_TYPE_L1);
	// あたり矩形などの設定と処理のスタート
	gmGmkBreakWallStart((OBS_OBJECT_WORK*)pwork);

	return (OBS_OBJECT_WORK*)pwork;
}
// ---------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkBreakWall_L2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_BWALL_WORK	*pwork;
	pwork = (GMS_GMK_BWALL_WORK*)gmGmkBreakWallInit(eve_rec, pos_x, pos_y, type, GME_GMK_WALL_TYPE_L2);
	// あたり矩形などの設定と処理のスタート
	gmGmkBreakWallStart((OBS_OBJECT_WORK*)pwork);

	return (OBS_OBJECT_WORK*)pwork;
}
// ---------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkBreakWall_R1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_BWALL_WORK	*pwork;
	pwork = (GMS_GMK_BWALL_WORK*)gmGmkBreakWallInit(eve_rec, pos_x, pos_y, type, GME_GMK_WALL_TYPE_R1);
	// あたり矩形などの設定と処理のスタート
	gmGmkBreakWallStart((OBS_OBJECT_WORK*)pwork);

	return (OBS_OBJECT_WORK*)pwork;
}
// ---------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkBreakWall_R2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_BWALL_WORK	*pwork;
	pwork = (GMS_GMK_BWALL_WORK*)gmGmkBreakWallInit(eve_rec, pos_x, pos_y, type, GME_GMK_WALL_TYPE_R2);
	// あたり矩形などの設定と処理のスタート
	gmGmkBreakWallStart((OBS_OBJECT_WORK*)pwork);

	return (OBS_OBJECT_WORK*)pwork;
}
// ---------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkBreakWall_C1Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_BWALL_WORK	*pwork;
	pwork = (GMS_GMK_BWALL_WORK*)gmGmkBreakWallInit(eve_rec, pos_x, pos_y, type, GME_GMK_WALL_TYPE_C1);
	// あたり矩形などの設定と処理のスタート
	gmGmkBreakWallStart((OBS_OBJECT_WORK*)pwork);
	pwork->gmk_work.ene_com.obj_work.disp_flag |= OBD_DISP_NODIRFLIP;

	return (OBS_OBJECT_WORK*)pwork;
}
// ---------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkBreakWall_C2Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_BWALL_WORK	*pwork;
	pwork = (GMS_GMK_BWALL_WORK*)gmGmkBreakWallInit(eve_rec, pos_x, pos_y, type, GME_GMK_WALL_TYPE_C2);
	// あたり矩形などの設定と処理のスタート
	gmGmkBreakWallStart((OBS_OBJECT_WORK*)pwork);
	pwork->gmk_work.ene_com.obj_work.disp_flag |= OBD_DISP_NODIRFLIP;
	pwork->gmk_work.ene_com.obj_work.obj_3d->drawflag |= NND_DRAWOBJ_DOUBLESIDE;

	return (OBS_OBJECT_WORK*)pwork;
}
// ---------------------------------------------------------------------------
// 091006 Dimps Ishizaki
OBS_OBJECT_WORK* GmGmkBreakWall_C1_H_Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_BWALL_WORK	*pwork;
	pwork = (GMS_GMK_BWALL_WORK*)gmGmkBreakWallInit(eve_rec, pos_x, pos_y, type, GME_GMK_WALL_TYPE_C1_H);
	// あたり矩形などの設定と処理のスタート
	gmGmkBreakWallStart((OBS_OBJECT_WORK*)pwork);
	pwork->gmk_work.ene_com.obj_work.disp_flag |= OBD_DISP_NODIRFLIP;
	pwork->gmk_work.ene_com.obj_work.disp_flag &= ~OBD_DISP_NODIR;


	pwork->gmk_work.ene_com.obj_work.dir.z = 0xC000;
	pwork->gmk_work.ene_com.col_work.obj_col.flag |= OBD_COLOBJ_NODIR_PARENT;

	return (OBS_OBJECT_WORK*)pwork;
}
// ===========================================================================


// ===========================================================================
// GmGmkBreakFloorInit
/*!
	ギミック 破壊可能床＆天井 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// ===========================================================================
//	破壊床　生成に関する定義
// ---------------------------------------------------------------------------
// 破壊属性
#define GMD_GMK_BFLOOR_EVE_FLAG_NO_BREAK		(0x0001)	//!< 大砲でのみ破壊
// その他設定
#define GMD_GMK_BFLOOR_EVE_FLAG_Z_ADJUST		(0x0002)	//!< 表示Z位置補正
// ---------------------------------------------------------------------------
OBS_OBJECT_WORK* GmGmkBreakFloorInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_BWALL_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;


	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_BWALL_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_BWALL_WORK), "GMK_BREAK_LAND_MAIN");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	u16 model = tbl_breakwall_mdl[g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id]][GME_GMK_WALL_TYPE_FLOOR];
	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_breakwall_obj_3d_list[model],
	                             &gmk_work->obj_3d);
	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// 表示位置補正
	if (eve_rec->flag & GMD_GMK_BFLOOR_EVE_FLAG_Z_ADJUST) {
		obj_work->pos.z -= FX32_ONE;
	}

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE|OBD_MOVE_NOCOL;			// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	pwork->broketype = (u16)(eve_rec->flag & GMD_GMK_BFLOOR_EVE_FLAG_NO_BREAK);
	
#if _IPHONE
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3 ||
		g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS) {
		// ライト設定
		gmk_work->obj_3d.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
		gmk_work->obj_3d.use_light_flag |= OBD_LIGHT_USE_FLAG_1;
	}
#endif // _IPHONE
	// あたり矩形などの設定と処理のスタート
	pwork->obj_type = GME_GMK_TYPE_BREAK_FLOOR;
	pwork->wall_type = GME_GMK_WALL_TYPE_FLOOR;
	gmGmkBreakWallStart(obj_work);
	return obj_work;
}
// ===========================================================================



// ===========================================================================
// GmGmkBreakWallBuild
/*!
	ギミック 破壊可能壁 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkBreakWallBuild(void)
{
	gm_gmk_breakwall_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_B_WALL_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_B_WALL_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkBreakLandFlush
/*!
	ギミック 破壊可能壁床 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkBreakWallFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_B_WALL_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_breakwall_obj_3d_list, amb->file_num);
}
// ===========================================================================
