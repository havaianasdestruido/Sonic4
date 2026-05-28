// =======================================================================
/*!
	@file	gmGmkUpBumper.c
	@brief	ギミック 登るバンパー＠ゾーン４工場

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkUpBumper.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmPadVib.h"

#include "gmGmkUpBumper.h"

// データヘッダ
#include "common/model/gmk_up_bmpr_mdl.hmb"



// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static fx32	player_spd_x;
static fx32	player_spd_y;
static s16	player_spd_keep_timer;
#define		GMD_GMK_UPBUMPER_PLAYERSPD_KEEP_TIME	(60)

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_upbumper_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
#define	OBJWORK		COMWORK.obj_work
#define	chgf(f)		ppFunc = (f)
// -------------

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// -------------

// ----- Definitions -------------------------------------------（定数の宣言）
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// オブジェクトタイプ
typedef enum tag_GME_GMK_TYPE{

	GME_GMK_TYPE_UPBUMPER_L = 0,	// 左張り付き
	GME_GMK_TYPE_UPBUMPER_R,		// 右張り付き

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
#define		GMD_GMK_UPBUMPER_RECT_X		(0)
#define		GMD_GMK_UPBUMPER_RECT_Y		(-8)
#define		GMD_GMK_UPBUMPER_RECT_W		( 8)
#define		GMD_GMK_UPBUMPER_RECT_H		( 8)


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const static s16 tbl_gm_gmk_upbumper_rect[GME_GMK_TYPE_MAX][GME_GMK_RECT_DATA_MAX] = {

	//	登るバンパー＠ゾーン４工場
	{
		//	土地あたり矩形＠左はりつき
		  GMD_GMK_UPBUMPER_RECT_X,		//	左
		  GMD_GMK_UPBUMPER_RECT_Y,		//	上
		  GMD_GMK_UPBUMPER_RECT_X+GMD_GMK_UPBUMPER_RECT_W,	//	右
		  GMD_GMK_UPBUMPER_RECT_Y+GMD_GMK_UPBUMPER_RECT_H,	//	下
	},
	{
		//	土地あたり矩形＠右はりつき
		  -GMD_GMK_UPBUMPER_RECT_W,		//	左
		   GMD_GMK_UPBUMPER_RECT_Y,		//	上
		   GMD_GMK_UPBUMPER_RECT_X,		//	右
		   GMD_GMK_UPBUMPER_RECT_Y+GMD_GMK_UPBUMPER_RECT_H,	//	下
	}
};
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//	はね返りデータ定義
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef struct tag_GMS_GMK_UPBUMPER_REBOUND_DATA{

	GME_PLY_ACT_STATE	act_state;

	fx32				spd_x;
	fx32				spd_y;

}GMS_GMK_UPBUMPER_REBOUND_DATA;
static const GMS_GMK_UPBUMPER_REBOUND_DATA tbl_upbmper_rebound_data[] =
{
	{
		GME_PLY_ACT_STATE_JUMP_SPIN,
		 (fx32)(4.0*FX32_ONE),	//	横
		-(fx32)(4.0*FX32_ONE),	//	上昇
	},
	{
		GME_PLY_ACT_STATE_JUMP_FALL,
		 (fx32)(8.0*FX32_ONE),	//	一定
		-(fx32)(8.0*FX32_ONE),	//	一定
	},
};
#define	GMD_GMK_UPBUMPER_REBOUND_DATA_NUM	(sizeof(tbl_upbmper_rebound_data)/sizeof(GMS_GMK_UPBUMPER_REBOUND_DATA))
// ---------------------------------------------------------------------------





// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef struct tag_GMS_GMK_UPBUMPER_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	GME_GMK_TYPE		obj_type;		//!< 上向きか下向きかオブジェクトタイプ

	s16					player_spd_keep_timer_mine;
										//!< プレイヤーにスピードを与えると値が入る
										//   値がある間デクリメント。０で速度情報を消す。
}GMS_GMK_UPBUMPER_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// ---------------------------------------------------------------------------



// ===========================================================================
// gmGmkUpBumper*
/*!
	ギミック 登るバンパー＠ゾーン４工場

	@note
		ソニックの位置を判定し、入れない場合は土地当たりを有効に、
		スタート位置に入った場合はソニックをコントロールする必要があります。

		とりあえず存在と当たり判定と土地当たりを作成。
 */
// ---------------------------------------------------------------------------
static void gmGmkUpBumperStay(OBS_OBJECT_WORK *obj_work);
static void gmGmkUpBumperHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkUpBumperStart
/*!
	ギミック 登るバンパー　サブ初期化

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkUpBumperStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_UPBUMPER_WORK *pwork = (GMS_GMK_UPBUMPER_WORK*)obj_work;
	OBS_RECT_WORK *rect_work;

	// 矩形設定
	// 対プレイヤー
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = gmGmkUpBumperHit;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 被破壊矩形設定
	ObjRectWorkSet(rect_work,
	               tbl_gm_gmk_upbumper_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_LEFT],
	               tbl_gm_gmk_upbumper_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_TOP],
	               tbl_gm_gmk_upbumper_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_RIGHT],
	               tbl_gm_gmk_upbumper_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_BOTTOM]);
	obj_work->flag &= ~OBD_OBJECT_NOHIT;								// 矩形あたり無し◆

	obj_work->chgf(gmGmkUpBumperStay);
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkUpBumperStay
/*!
	ギミック 登るバンパー

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkUpBumperStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_UPBUMPER_WORK *pwork = (GMS_GMK_UPBUMPER_WORK*)obj_work;

	if( pwork->player_spd_keep_timer_mine > 0 )
	{
		if( pwork->player_spd_keep_timer_mine > player_spd_keep_timer )
		{
			//	ほかのバンパーに更新されてなければ
			pwork->player_spd_keep_timer_mine = player_spd_keep_timer;
			player_spd_keep_timer -= 1;
			if( player_spd_keep_timer <= 0 )
			{
				pwork->player_spd_keep_timer_mine = 0;
				player_spd_keep_timer = 0;
				player_spd_x = player_spd_y = 0;
			}
		}
		else
		{
			pwork->player_spd_keep_timer_mine = 0;
		}
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkUpBumperHit
/*!
	ギミック 登るバンパー プレイヤー接触

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkUpBumperHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_GMK_UPBUMPER_WORK *pwork = (GMS_GMK_UPBUMPER_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	//	一応プレイヤーが当たったかどうかを確認
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] )
	{
		fx32 spd_x;
		fx32 spd_y;
		if( player_spd_keep_timer <= 0 )
		{
			u32	i;

			spd_x = 0;
			for (i = 0; i < GMD_GMK_UPBUMPER_REBOUND_DATA_NUM; i++)
			{
				if( ply_work->act_state == tbl_upbmper_rebound_data[i].act_state )
				{
					spd_x = tbl_upbmper_rebound_data[i].spd_x;
					spd_y = tbl_upbmper_rebound_data[i].spd_y;

					player_spd_x = spd_x;
					player_spd_y = spd_y;
					player_spd_keep_timer = GMD_GMK_UPBUMPER_PLAYERSPD_KEEP_TIME;
					pwork->player_spd_keep_timer_mine = (s16)(player_spd_keep_timer+1);
					break;
				}
			}
			if (spd_x == 0)
			{
				spd_x = MTM_MATH_ABS(ply_work->obj_work.spd.x);
				spd_x += spd_x>>3;	//	1.125
				if( spd_x > 8*FX32_ONE )
					spd_x = 8*FX32_ONE;
				if( spd_x < 4*FX32_ONE )
					spd_x = 4*FX32_ONE;
				spd_y = -4*FX32_ONE;
			}
		}
		else
		{
			spd_x = player_spd_x;
			spd_y = player_spd_y;
			player_spd_keep_timer = GMD_GMK_UPBUMPER_PLAYERSPD_KEEP_TIME;
			pwork->player_spd_keep_timer_mine = (s16)(player_spd_keep_timer+1);
		}

		if( pwork->obj_type == GME_GMK_TYPE_UPBUMPER_R )
			spd_x = -spd_x;
		// プレイヤーをジャンプ
		GmPlySeqGmkInitUpBumper(ply_work, spd_x, spd_y);
		// 振動
		GMM_PAD_VIB_SMALL();
	}
	// 一応ヒットしなかった事にする
	mine_rect->flag &= ~(OBD_RECT_DAMAGE | OBD_RECT_FRAMEHIT | OBD_RECT_FRAMEOUT);
}
// ---------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================





// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkUpBumper?Init
/*!
 *	ギミック 登るバンパー＠ゾーン４工場 初期化関数
 *	GmGmkUpBumperUInit 上向き
 *	GmGmkUpBumperDInit 下向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
static OBS_OBJECT_WORK* gmGmkUpBumperInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_UPBUMPER_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_UPBUMPER_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_UPBUMPER_WORK), "Gmk_UpBumper");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_upbumper_obj_3d_list[IDB_GMK_UP_BMPR_MDL_GMK_UP_BMPR_ZNO],
	                             &gmk_work->obj_3d);
	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	return obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkUpBumperLInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_UPBUMPER_WORK	*pwork;

	pwork = (GMS_GMK_UPBUMPER_WORK*)gmGmkUpBumperInit(eve_rec, pos_x, pos_y, type);
	pwork->obj_type = GME_GMK_TYPE_UPBUMPER_L;
	gmGmkUpBumperStart(&pwork->gmk_work.ene_com.obj_work);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkUpBumperRInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_UPBUMPER_WORK	*pwork;
	OBS_OBJECT_WORK		*obj_work;

	obj_work = gmGmkUpBumperInit(eve_rec, pos_x, pos_y, type);
	pwork = (GMS_GMK_UPBUMPER_WORK*)obj_work;

//	obj_work->disp_flag |= OBD_DISP_HFLIP;
//	obj_work->disp_flag &= ~OBD_DISP_NODIRFLIP;
//	pwork->gmk_work.ene_com.obj_work.disp_flag |= OBD_DISP_HFLIP;
	obj_work->disp_flag &= ~OBD_DISP_NODIRFLIP;
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_DOUBLESIDE;
	obj_work->dir.y = 0x4000;

	pwork->obj_type = GME_GMK_TYPE_UPBUMPER_R;
	gmGmkUpBumperStart(&pwork->gmk_work.ene_com.obj_work);

	return obj_work;
}
// ==========================================================================



// ===========================================================================
// GmGmkUpBumperBuild
/*!
	ギミック 登るバンパー＠ゾーン４工場 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkUpBumperBuild(void)
{
	gm_gmk_upbumper_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_UPBUMPER_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_UPBUMPER_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkUpBumperFlush
/*!
	ギミック 登るバンパー＠ゾーン４工場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkUpBumperFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_UPBUMPER_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_upbumper_obj_3d_list, amb->file_num);
}
// ===========================================================================
