// ===========================================================================
/*!
	@file	gmGmkPressWall.cpp
	@brief	ギミック 迫る壁＠ゾーン３と４

	@author	ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkPressWall.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
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
#include "gmPlySeq.h"
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmEffectZone.h"
#include "gmSound.h"
#include "gmPadVib.h"

#include "gmGmkPressWall.h"

// データヘッダ
#include "common/model/gmk_presswall_mdl.hmb"

#if _IPHONE
#include "iphone/model/gmk_presswall4_mdl.hmb"
#include "iphone/model/gmk_presswall4_mtn.hmb"
#include "iphone/model/gmk_presswall4_mat.hmb"
#else
#include "common/model/gmk_presswall4_mdl.hmb"
#include "common/model/gmk_presswall4_mtn.hmb"
#include "common/model/gmk_presswall4_mat.hmb"
#endif // _IPHONE



// ----- Struct Definitions --------------------------------------（型の宣言）
// -----

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -----

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// -----

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -----

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_presswall_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
#define	OBJWORK		COMWORK.obj_work
// -----

// ----- Macro Functions -----------------------------------（処理マクロ定義）
#define	chgf(f)		ppFunc = (f)
// -----

// ----- Definitions -------------------------------------------（定数の宣言）
#define	GMD_GMK_PRESSWALL_WIDTH			(320*FX32_ONE)		//	モデル横サイズ
#define	GMD_GMK_PRESSWALL_HEIGHT		(192*FX32_ONE)		//	モデル縦サイズ

#define	GMD_GMK_PRESSWALL_COL_OFST_X	(-64*3)
#define	GMD_GMK_PRESSWALL_COL_WIDTH		(64*3)
#define	GMD_GMK_PRESSWALL_COL_HEIGHT	(256)	//	画面の高さ？

#if _IPHONE
#define GMD_GMK_PRESSWALL4_MAT_GLARE		(IDB_GMK_PRESSWALL4_MAT_GMK_P_WALL_G_4_INV)
#define GMD_GMK_PRESSWALL4_MAT_ROLL			(IDB_GMK_PRESSWALL4_MAT_GMK_P_WALL_4_INV)

#define GMD_GMK_PRESSWALL4_MDL				(IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_4_INO)
#define GMD_GMK_PRESSWALL4_MDL_A			(IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_A_4_INO)
#define GMD_GMK_PRESSWALL4_MDL_B			(IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_B_4_INO)
#define GMD_GMK_PRESSWALL4_MDL_G			(IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_G_4_INO)
#define GMD_GMK_PRESSWALL4_MDL_LIGHT		(-1) // ダミー定義
#else
#define GMD_GMK_PRESSWALL4_MAT_GLARE		(IDB_GMK_PRESSWALL4_MAT_GMK_P_WALL_G_4_ZNV)
#define GMD_GMK_PRESSWALL4_MAT_ROLL			(-1) // ダミー定義

#define GMD_GMK_PRESSWALL4_MDL				(IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_4_ZNO)
#define GMD_GMK_PRESSWALL4_MDL_A			(IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_A_4_ZNO)
#define GMD_GMK_PRESSWALL4_MDL_B			(IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_B_4_ZNO)
#define GMD_GMK_PRESSWALL4_MDL_G			(IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_G_4_ZNO)
#define GMD_GMK_PRESSWALL4_MDL_LIGHT		(IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_LIGHT_4_ZNO)
};
#endif // _IPHONE




// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分
// ----- Struct Definitions --------------------------------------（型の宣言）
//! 迫る壁＠ゾーン３と４
typedef struct tag_GMS_GMK_PWALL_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。

	fx32	master_posy;				//!< 表示のために

	fx32	wall_speed;					//!<
	s16		wall_vibration;				//!< 上下振動
	s16		wall_effect_build_timer;	//!< 上下振動

	fx32	wall_height;				//!< 移動Ｙ位置固定型なら０以外

	fx32	wall_brake;					//!< ゾーン４用停止速度
	fx32	wall_timer;					//!< 汎用

	BOOL	ply_death;
	BOOL	stop_wall;

	OBS_OBJECT_WORK		*efct_obj;

	GSS_SND_SE_HANDLE*	se_handle;

#if _IPHONE
	u32		mat_timer;					//!< マテリアルモーション設定
	u32		mat_timer_line;				//!< マテリアルモーション更新タイミング閾値
#endif // _IPHONE
}GMS_GMK_PWALL_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
static GMS_GMK_PWALL_WORK *pwall;
// ---------------------------------------------------------------------------



// ===========================================================================
// gmGmkPressWall*
/*!
	ギミック 迫る壁＠ゾーン３と４

	@note

 */
// ---------------------------------------------------------------------------
static void gmGmkPressWallStay(OBS_OBJECT_WORK *obj_work);
static void gmGmkPressWallForce(OBS_OBJECT_WORK *obj_work);

static void gmGmkPressWallCreateRail(OBS_OBJECT_WORK *parent_obj,fx32 height, fx32 pos_y);
static void gmGmkPressWallCreateParts(OBS_OBJECT_WORK *parent_obj,fx32 pos_y,fx32 height);

static void gmGmkPressWallZ4Hit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);

static void gmGmkPressWallExit(MTS_TASK_TCB *tcb);
static void gmGmkPressWallSeStop(OBS_OBJECT_WORK *obj_work);
// ===========================================================================

// ---------------------------------------------------------------------------
// gmGmkPressWall_ppOut
/*!
	ギミック 迫る壁＠ゾーン３ 描画

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWall_ppOut(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALL_WORK *pwork = (GMS_GMK_PWALL_WORK*)obj_work;

	fx32 height;

	obj_work->pos.y  = pwork->master_posy;
	obj_work->pos.y += obj_work->user_timer;	//	振動
	if( pwork->wall_height == 0 )
	{
		//	全領域描画型
		while( obj_work->pos.y+GMD_GMK_PRESSWALL_HEIGHT < g_obj.camera[0][MTD_Y] )
		{
			obj_work->pos.y += GMD_GMK_PRESSWALL_HEIGHT;
		}
		while( obj_work->pos.y > g_obj.camera[0][MTD_Y] )
		{
			obj_work->pos.y -= GMD_GMK_PRESSWALL_HEIGHT;
		}
		height = obj_work->pos.y - g_obj.camera[0][MTD_Y];
		while( height < 256*FX32_ONE )
		{
			ObjDrawActionSummary(obj_work);
			obj_work->pos.y += GMD_GMK_PRESSWALL_HEIGHT;
			height += GMD_GMK_PRESSWALL_HEIGHT;
		}
	}
	else
	{
		//	縦位置固定スライド型
		height = 0;
		while( height < pwork->wall_height )
		{
			ObjDrawActionSummary(obj_work);
			obj_work->pos.y += GMD_GMK_PRESSWALL_HEIGHT;
			height += GMD_GMK_PRESSWALL_HEIGHT;
		}
	}

	obj_work->pos.y = g_obj.camera[0][MTD_Y];
}
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkPressWallStay
/*!
	ギミック 迫る壁＠ゾーン３と４ 待機

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallStay(OBS_OBJECT_WORK *obj_work)
{
	//	横スクロールが自分の位置に達したらスタート
	if (  (g_obj.camera[0][MTD_X] >= obj_work->pos.x)
		||(  (g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id] == GSD_MAIN_ZONE_TYPE_3)	// Zone3で
		   &&(obj_work->user_flag) ) )															// リスタート用壁だった場合も開始
	{
		GMS_GMK_PWALL_WORK *pwork = (GMS_GMK_PWALL_WORK*)obj_work;
		//	土地あたり矩形餅オブジェ設定
		//	普段では通り抜けられないように
		pwork->COMWORK.col_work.obj_col.obj       = obj_work;	//	まだ当たるように
		// 地形矩形設定
		pwork->COMWORK.col_work.obj_col.diff_data	= (s8*)g_gm_default_col;
		pwork->COMWORK.col_work.obj_col.width  = GMD_GMK_PRESSWALL_COL_WIDTH;	//	中心位置からの幅
		pwork->COMWORK.col_work.obj_col.ofst_x = GMD_GMK_PRESSWALL_COL_OFST_X;
		pwork->COMWORK.col_work.obj_col.height = GMD_GMK_PRESSWALL_COL_HEIGHT;	// 固定
		pwork->COMWORK.col_work.obj_col.ofst_y = 0;								// 固定
		pwork->COMWORK.col_work.obj_col.flag  |= (OBD_COLOBJ_NOFREE_DIFF_DATA		// diff_dataを開放しない
												| OBD_COLOBJ_NODIR					// 自dir を使用しない
												| OBD_COLOBJ_NODIR_PARENT);			// 親dir を使用しない

//		pwork->COMWORK.col_work.obj_col.dir = (u16)(0x0000);
//		pwork->COMWORK.col_work.obj_col.flag |= OBD_COLOBJ_NODIR_PARENT;
		pwork->COMWORK.col_work.obj_col.attr &= ~OBD_COL_DATA_ATTR_THROUGH;

		obj_work->disp_flag &= ~OBD_DISP_NODISP;	//	表示開始

		if( g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id] == GSD_MAIN_ZONE_TYPE_3 )
		{
			if( pwork->wall_height > 0 )
				gmGmkPressWallCreateRail( obj_work, pwork->wall_height, pwork->master_posy);
		}
		if( g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id] == GSD_MAIN_ZONE_TYPE_4 )
		{
			gmGmkPressWallCreateParts( obj_work, pwork->master_posy,pwork->wall_height);
			//	攻撃矩形設定
			// 対プレイヤー
			OBS_RECT_WORK *rect_work;
			pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;
			pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
			pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_ENABLE;
			rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK];
		//	ObjRectAtkSet(rect_work, GMD_OBJ_RECT_ATK_FLAG_NORMALATK, GMD_OBJ_RECT_ATK_POWER_DEFAULT);
		//	ObjRectDefSet(rect_work, 0/*flag*/, 0/*power*/);
			ObjRectWorkZSet(rect_work,
			               -16,  0,  -32, 0, 192, +32 );
			rect_work->flag |= OBD_RECT_ENABLE;
			rect_work->flag |= OBD_RECT_OUT;			// 連続判定防止用
			obj_work->flag &= ~OBD_OBJECT_NOHIT;			//	まだ
			rect_work->ppHit = gmGmkPressWallZ4Hit;
		}

		if (  (g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id] == GSD_MAIN_ZONE_TYPE_3)	// Zone3で
			&&(obj_work->user_flag) ) {															// リスタート用壁だった場合は
			// リスタート壁に使用したフラグ消去
			obj_work->user_flag = NULL;
		} else {
			// 一般起動タイプは動き出し時の処理
			// コントローラー振動
			GMM_PAD_VIB_MID_TIME(60);											// 60 Frame

			// SE handle取得＆コール
			pwork->se_handle = GsSoundAllocSeHandle();
#if _IPHONE
			// 環境音的な使い方なので強制再生
			GmSoundPlaySEForce("MovingWall", pwork->se_handle);
#else
			GmSoundPlaySE("MovingWall", pwork->se_handle);
#endif // _IPHONE
		}

		pwork->efct_obj = NULL;
		obj_work->chgf(gmGmkPressWallForce);
		gmGmkPressWallForce(obj_work);

	}
}
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkPressWallForce
/*!
	ギミック 迫る壁　迫ってくる

	@note
		
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const fx32 wall_vib[] =
{
	1*FX32_ONE,	//0
	0*FX32_ONE,	//1
	2*FX32_ONE,	//2
	1*FX32_ONE,	//3
	0*FX32_ONE,	//4
	2*FX32_ONE,	//5
	1*FX32_ONE,	//6
	0*FX32_ONE,	//7

	 2*FX32_ONE,	//8
	-2*FX32_ONE,	//9
	 0*FX32_ONE,	//A
	 4*FX32_ONE,	//B
	 0*FX32_ONE,	//C
	-4*FX32_ONE,	//D
	 0*FX32_ONE,	//E
	 4*FX32_ONE,	//F
	 0*FX32_ONE,	//10
	-2*FX32_ONE,	//11
	 2*FX32_ONE,	//12
	 0*FX32_ONE,	//13
};
#define	GMD_GMK_PRESSWALL_VIB_TABLE_NUM		(0x14)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallForce_100(OBS_OBJECT_WORK *obj_work);		//	停止
static void gmGmkPressWallForce_200(OBS_OBJECT_WORK *obj_work);		//	停止
static void gmGmkPressWallForceZ4_Hit(OBS_OBJECT_WORK *obj_work);	//	一時停止
static void gmGmkPressWallForceZ4_Stop(OBS_OBJECT_WORK *obj_work);	//	一時停止
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallForce(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALL_WORK *pwork = (GMS_GMK_PWALL_WORK*)obj_work;
	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if( ply_work->player_flag & GMD_PLF_DIE )
	{
		//	プレイヤーデスのため停止させる
		pwork->ply_death = TRUE;
		pwork->wall_speed = 0;
		// SE停止
		gmGmkPressWallSeStop(obj_work);
	}

	if( g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id] == GSD_MAIN_ZONE_TYPE_3 )
	{
		if( pwork->wall_speed == 0 || pwork->ply_death )
		{
#if 0
			int i;
			for( i =0; i < 4; i++ )
			{
				OBS_OBJECT_WORK *effobj_work = 
					(OBS_OBJECT_WORK*)GmEfctZoneEsCreate(NULL, GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_WALL_M_Z3);
				effobj_work->pos.x = obj_work->pos.x - ((64*i)+(mtMathRand()&0x1f))*FX32_ONE;
				effobj_work->pos.y = g_obj.camera[0][MTD_Y] - ((8*i)+(mtMathRand()&0x1f))*FX32_ONE;
				effobj_work->pos.z = obj_work->pos.z;
				effobj_work->spd.x = pwork->wall_speed;
			}
#endif // 0
			pwork->wall_vibration &= 0x03;
			pwork->wall_vibration += 3;
			pwork->wall_brake = pwork->wall_speed;
			obj_work->chgf(gmGmkPressWallForce_100);
			gmGmkPressWallForce_100(obj_work);
			return;
		}

		obj_work->pos.x += pwork->wall_speed;
		obj_work->user_timer = (fx32)wall_vib[pwork->wall_vibration&0x7];
		pwork->wall_vibration++;

		if( pwork->wall_effect_build_timer == 0 )
		{
			OBS_OBJECT_WORK *effobj_work = 
				(OBS_OBJECT_WORK*)GmEfctZoneEsCreate(NULL, GSD_MAIN_ZONE_TYPE_3, GME_EFCT_Z03_IDX_WALL_M_Z3);
			effobj_work->pos.x = obj_work->pos.x;
			effobj_work->pos.y = g_obj.camera[0][MTD_Y]/* + ((mtMathRand()&0x1f)*FX32_ONE)*/;
			effobj_work->pos.z = obj_work->pos.z;
			effobj_work->spd.x = pwork->wall_speed;

			pwork->wall_effect_build_timer = (s16)((mtMathRand()&0x3f)+90);
		}
		pwork->wall_effect_build_timer -= 1;
		obj_work->pos.y = g_obj.camera[0][MTD_Y];
	}
	else
	{
		//	ゾーン４
		obj_work->pos.x += pwork->wall_speed;

		if( pwork->ply_death || pwork->wall_speed == 0 )
		{
			if( pwork->efct_obj )
			{
				//	エフェクト停止
				ObjDrawKillAction3DES(pwork->efct_obj);
				pwork->efct_obj = NULL;
				pwork->wall_effect_build_timer = 0;
			}
			obj_work->chgf(gmGmkPressWallForceZ4_Stop);
		}
		else if( obj_work->user_flag & 0x01 || pwork->ply_death )
		{
			//	ヒット判定
			pwork->wall_brake = pwork->wall_speed;
			obj_work->user_flag &= ~0x01;
			obj_work->chgf(gmGmkPressWallForceZ4_Hit);

			if( pwork->efct_obj )
			{
				//	エフェクト停止
				ObjDrawKillAction3DES(pwork->efct_obj);
				pwork->efct_obj = NULL;
				pwork->wall_effect_build_timer = 0;
			}
		}
		else
		{
#if 0
			if( pwork->wall_effect_build_timer == 0 )
			{
				//	移動継続ならエフェクトを生成

				OBS_OBJECT_WORK *effobj_work = 
					(OBS_OBJECT_WORK*)GmEfctZoneEsCreate(obj_work, GSD_MAIN_ZONE_TYPE_4, GME_EFCT_Z04_IDX_WALL_M_Z4);
				effobj_work->flag &= ~OBD_OBJECT_PARENT_FIX;			// 親付随なし
				effobj_work->disp_flag &= ~OBD_DISP_REPEAT;
				effobj_work->pos.y = g_obj.camera[0][MTD_Y] + (GMD_OBJ_LCD_Y/2)*FX32_ONE;

				if( pwork->efct_obj )
					ObjDrawKillAction3DES(pwork->efct_obj);

				pwork->efct_obj = effobj_work;
				pwork->wall_effect_build_timer = 45;
			}
			if( pwork->efct_obj )
				pwork->efct_obj->pos.x = obj_work->pos.x;
#endif // 0
			pwork->wall_effect_build_timer -= 1;
		}
	}

	if( pwork != pwall )
	{
		//	新しい壁に次を譲る
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		pwork->COMWORK.enemy_flag |= GMD_ENEMY_FLAG_DIE;

		// SE停止
		gmGmkPressWallSeStop(obj_work);
	}

}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallForce_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALL_WORK *pwork = (GMS_GMK_PWALL_WORK*)obj_work;

	pwork->wall_brake -= (pwork->wall_speed/GMD_GMK_PRESSWALL_VIB_TABLE_NUM);
	obj_work->pos.x += pwork->wall_brake;
	if( pwork->wall_vibration < GMD_GMK_PRESSWALL_VIB_TABLE_NUM )
	{
		obj_work->user_timer = (fx32)wall_vib[pwork->wall_vibration];
		pwork->wall_vibration++;
	}
	else
	{
		obj_work->chgf(gmGmkPressWallForce_200);	//	処理なし
		// SE停止
		gmGmkPressWallSeStop(obj_work);
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallForce_200(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALL_WORK *pwork = (GMS_GMK_PWALL_WORK*)obj_work;
	if( pwork != pwall )
	{
		//	新しい壁に次を譲る
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		pwork->COMWORK.enemy_flag |= GMD_ENEMY_FLAG_DIE;
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallForceZ4_Hit_100(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
#define		GMD_GMK_PRESSWALLZ4HIT_WAIT_TIME	(64)
static void gmGmkPressWallForceZ4_Hit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALL_WORK *pwork = (GMS_GMK_PWALL_WORK*)obj_work;
	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if( ply_work->player_flag & GMD_PLF_DIE )
	{
		//	プレイヤーデスのため停止させる
		pwork->ply_death = TRUE;
		pwork->wall_speed = 0;
		// SE停止
		gmGmkPressWallSeStop(obj_work);
	}

	pwork->wall_brake -= (pwork->wall_speed/GMD_GMK_PRESSWALLZ4HIT_WAIT_TIME);
	if( pwork->wall_brake <= 0 || pwork->wall_speed == 0 )
	{
		pwork->wall_brake = 0;
		if( pwork->ply_death == FALSE && pwork->wall_speed != 0 )
			obj_work->chgf(gmGmkPressWallForceZ4_Hit_100);
		else
			obj_work->chgf(gmGmkPressWallForceZ4_Stop);
	}
	if( pwork->gmk_work.obj_3d.speed[0] > 0 )
	{
		pwork->gmk_work.obj_3d.speed[0] -= 1.0f/GMD_GMK_PRESSWALLZ4HIT_WAIT_TIME;
		pwork->gmk_work.obj_3d.speed[1] -= 1.0f/GMD_GMK_PRESSWALLZ4HIT_WAIT_TIME;
#if _IPHONE
		pwork->mat_timer++;
		if (pwork->mat_timer > pwork->mat_timer_line) {
			// 更新
			obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
			pwork->mat_timer_line = pwork->mat_timer;
			pwork->mat_timer = 0;
		}
		else {
			// 停止しておく
			obj_work->disp_flag |= OBD_DISP_NOUPDATE;
		}
#endif // _IPHONE
	}
	obj_work->pos.x += pwork->wall_brake;
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPressWallForceZ4_Hit_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALL_WORK *pwork = (GMS_GMK_PWALL_WORK*)obj_work;

	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if( ply_work->player_flag & GMD_PLF_DIE )
	{
		//	プレイヤーデスのため停止させる
		pwork->ply_death = TRUE;
		pwork->wall_speed = 0;
		obj_work->chgf(gmGmkPressWallForceZ4_Stop);
		return;
	}

	pwork->wall_brake += (pwork->wall_speed/GMD_GMK_PRESSWALLZ4HIT_WAIT_TIME);
	if( pwork->wall_brake >= pwork->wall_speed )
	{
		pwork->wall_brake = pwork->wall_speed;

		pwork->gmk_work.obj_3d.speed[0] = 1.0f;
		pwork->gmk_work.obj_3d.speed[1] = 1.0f;
#if _IPHONE
		pwork->mat_timer = 0;
		pwork->mat_timer_line = 0;
		obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
#endif // _IPHONE

		obj_work->flag &= ~OBD_OBJECT_NOHIT;							// 矩形あたり有効
		pwork->ply_death = FALSE;
		obj_work->chgf(gmGmkPressWallForce);
	}
	if( pwork->gmk_work.obj_3d.speed[0] < 1.0f )
	{
		pwork->gmk_work.obj_3d.speed[0] += 1.0f/GMD_GMK_PRESSWALLZ4HIT_WAIT_TIME;
		pwork->gmk_work.obj_3d.speed[1] += 1.0f/GMD_GMK_PRESSWALLZ4HIT_WAIT_TIME;
#if _IPHONE
		pwork->mat_timer++;
		if (pwork->mat_timer > pwork->mat_timer_line) {
			// 停止して更新
			obj_work->disp_flag |= OBD_DISP_NOUPDATE;
			pwork->mat_timer_line = pwork->mat_timer;
			pwork->mat_timer = 0;
		}
		else {
			// アニメ
			obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
		}
#endif // _IPHONE
	}
	obj_work->pos.x += pwork->wall_brake;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallForceZ4_Stop(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALL_WORK *pwork = (GMS_GMK_PWALL_WORK*)obj_work;
	if( pwork->gmk_work.obj_3d.speed[0] > 0 )
	{
		pwork->gmk_work.obj_3d.speed[0] -= 1.0f/GMD_GMK_PRESSWALLZ4HIT_WAIT_TIME;
		pwork->gmk_work.obj_3d.speed[1] -= 1.0f/GMD_GMK_PRESSWALLZ4HIT_WAIT_TIME;
#if _IPHONE
		pwork->mat_timer++;
		if (pwork->mat_timer > pwork->mat_timer_line) {
			// 更新
			obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
			pwork->mat_timer_line = pwork->mat_timer;
			pwork->mat_timer = 0;
		}
		else {
			// 停止しておく
			obj_work->disp_flag |= OBD_DISP_NOUPDATE;
		}
#endif // _IPHONE
		if( pwork->gmk_work.obj_3d.speed[0] <= 0 )
		{
			pwork->gmk_work.obj_3d.speed[0] = 0.0f;
			pwork->gmk_work.obj_3d.speed[1] = 0.0f;
#if _IPHONE
			// 停止
			obj_work->disp_flag |= OBD_DISP_NOUPDATE;
#endif // _IPHONE
			// SE停止
			gmGmkPressWallSeStop(obj_work);
		}
	}
}
// ---------------------------------------------------------------------------





// ---------------------------------------------------------------------------
//	gmGmkPointMarkerHit
/*!
 *	迫る壁ゾーン４あたり判定
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallZ4Hit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;

	obj_work->flag |= OBD_OBJECT_NOHIT;							// 矩形あたり無効
	obj_work->user_flag |= 0x01;								//	アタリ告知
	GmEnemyDefaultAtkFunc( mine_rect, match_rect);
}
// ---------------------------------------------------------------------------




// ---------------------------------------------------------------------------
//	gmGmkPressWallSeStop
/*!
 *	迫る壁ループSE停止
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallSeStop(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALL_WORK *pwork = (GMS_GMK_PWALL_WORK*)obj_work;

	if (pwork->se_handle) {
		GsSoundStopSeHandle(pwork->se_handle);
		GsSoundFreeSeHandle(pwork->se_handle);
		pwork->se_handle = NULL;
	}
}
// ---------------------------------------------------------------------------




// ---------------------------------------------------------------------------
// gmGmkPressWallStart
/*!
	ギミック 迫る壁　サブ初期化

	@note
		
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALL_WORK *pwork = (GMS_GMK_PWALL_WORK*)obj_work;

	pwall = pwork;
	/*
		// チェック用矩形
		OBS_RECT_WORK *rect_work;
		pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;
		rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
		rect_work->ppDef = NULL;
		rect_work->ppHit = NULL;
		// 矩形設定
		ObjRectWorkSet( rect_work,
		                GMD_GMK_PRESSWALL_COL_OFST_X,
		                0,
		                0,
		                GMD_GMK_PRESSWALL_COL_HEIGHT);
		obj_work->flag &= ~OBD_OBJECT_NOHIT;							// 矩形あたり有効
	*/

	//	消えない！
	obj_work->flag |= OBD_OBJECT_NOCLIP;

	// 固有のメンバ初期化
	pwork->wall_vibration = 0;
	pwork->wall_effect_build_timer = 0;
	pwork->master_posy = obj_work->pos.y;

	pwall->stop_wall = FALSE;

	obj_work->disp_flag |= OBD_DISP_NODISP;	//	まだ表示なし
	obj_work->chgf(gmGmkPressWallStay);
}
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
// gmGmkPressWallStopperMain
/*!
	ギミック 迫る壁　停止

	@note
		
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallStopperMain(OBS_OBJECT_WORK *obj_work)
{
	if( pwall == NULL )
	{
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		return;
	}

	if( obj_work->user_flag )
	{
		//	配置の範囲内チェックを行う
		if( obj_work->pos.y < g_obj.camera[0][MTD_Y]-128*FX32_ONE || 
		    obj_work->pos.y > g_obj.camera[0][MTD_Y]+(256+128)*FX32_ONE )
			return;
	}

	if( pwall->OBJWORK.pos.x > obj_work->pos.x )
	{
		GMS_ENEMY_COM_WORK *ene_com = (GMS_ENEMY_COM_WORK *)obj_work;
		pwall->OBJWORK.pos.x = obj_work->pos.x;
		pwall->wall_speed = 0;

		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		ene_com->enemy_flag |= GMD_ENEMY_FLAG_DIE;
	}

	if( obj_work->user_work != (u32)pwall )
	{
		GMS_ENEMY_COM_WORK *ene_com = (GMS_ENEMY_COM_WORK *)obj_work;
		//	新しい壁が現れたので役目を終える
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		ene_com->enemy_flag |= GMD_ENEMY_FLAG_DIE;
	}
}
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
// gmGmkPressWallStopperStart
/*!
	ギミック 迫る壁　停止　サブ初期化

	@note
		
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallStopperStart(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_work = (u32)pwall;

	obj_work->disp_flag |= OBD_DISP_NODISP;	//	表示なし
	obj_work->move_flag |= OBD_MOVE_NOMOVE|OBD_MOVE_NOCOL|OBD_MOVE_NOCOLOBJ;			// 移動無し 地形あたり無し

	//	消えない！
	obj_work->flag |= OBD_OBJECT_NOCLIP;

	obj_work->chgf(gmGmkPressWallStopperMain);
}
// --------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ===========================================================================





// ===========================================================================
// --------------------------------------------------------------------------
// gmGmkPressWallControler
/*!
	ギミック 迫る壁　速度コントロール

	@note

 */
// --------------------------------------------------------------------------
typedef struct tag_GMS_GMK_PWALLCTRL_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	union
	{
		fx32	line_top;
		fx32	line_left;
	};
	union
	{
		fx32	line_bottom;
		fx32	line_right;
	};
	GMS_PLAYER_WORK	*ply_work;
	fx32	last_ply_x;					//!<
	fx32	last_ply_y;					//!<

}GMS_GMK_PWALLCTRL_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// gmGmkPressWallControler
/*!
	ギミック 迫る壁　速度コントロール

	@note
		
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallControler(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALLCTRL_WORK *pwork = (GMS_GMK_PWALLCTRL_WORK*)obj_work;
	GMS_PLAYER_WORK *ply_work = pwork->ply_work;

	if( pwall )
	{
		if( (obj_work->user_flag&0x01) == 0 )
		{
			//	縦通過判定　
		}
		else
		{
			//	横通過判定　
			if( obj_work->pos.x >  pwork->last_ply_x && obj_work->pos.x <= ply_work->obj_work.pos.x )
			{
				if( !(obj_work->user_flag&0x02)||
					  (ply_work->obj_work.pos.y >= pwork->line_top && ply_work->obj_work.pos.y <= pwork->line_bottom) )
				{
					if( obj_work->user_flag&0x04 )
					{
						//	ワープ
						if( pwall->OBJWORK.pos.x <= (g_obj.camera[0][MTD_X] - 8*FX32_ONE) )
							pwall->OBJWORK.pos.x = (g_obj.camera[0][MTD_X] - 8*FX32_ONE);
					}
					pwall->wall_speed = (fx32)obj_work->user_timer;
					if( pwall->wall_speed == 0 )
					{
						pwall->stop_wall = TRUE;
					}
					obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
					pwork->COMWORK.enemy_flag |= GMD_ENEMY_FLAG_DIE;
					return;
				}
			}
		}
	}
	pwork->last_ply_x = ply_work->obj_work.pos.x;
	pwork->last_ply_y = ply_work->obj_work.pos.y;
}
// --------------------------------------------------------------------------



// --------------------------------------------------------------------------
// gmGmkPressWallControlerStart
/*!
	ギミック 迫る壁　速度コントロール　サブ初期化

	@note
		
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPressWallControlerStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PWALLCTRL_WORK *pwork = (GMS_GMK_PWALLCTRL_WORK*)obj_work;
	GMS_PLAYER_WORK *ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	pwork->ply_work = ply_work;
	pwork->last_ply_x = ply_work->obj_work.pos.x;
	pwork->last_ply_y = ply_work->obj_work.pos.y;

	obj_work->disp_flag |= OBD_DISP_NODISP;	//	表示なし
	obj_work->move_flag |= OBD_MOVE_NOMOVE|OBD_MOVE_NOCOL|OBD_MOVE_NOCOLOBJ;			// 移動無し 地形あたり無し

	obj_work->chgf(gmGmkPressWallControler);
}
// --------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ===========================================================================



// ===========================================================================
// gmGmkPressWallCreate*
/*!
	ギミック 迫る壁＠ゾーン３と４ 装飾品生成
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// --------------------------------------------------------------------------
// レールや部品
typedef struct tag_GMS_GMK_PRESSWALL_PARTS
{
	GMS_EFFECT_3DNN_WORK	eff_work;
	fx32					ofst_y;
	fx32					master_posy;

}GMS_GMK_PRESSWALL_PARTS;
// ---------------------------------------------------------------------------
static void gmGmkPressWallParts(OBS_OBJECT_WORK *obj_work)
{
	obj_work->pos.x = obj_work->parent_obj->pos.x;
}
// ---------------------------------------------------------------------------
static void gmGmkPressWallRail(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PRESSWALL_PARTS *pwork = (GMS_GMK_PRESSWALL_PARTS*)obj_work;
	GMS_GMK_PWALL_WORK *parent_work = (GMS_GMK_PWALL_WORK*)obj_work->parent_obj;


	obj_work->pos.x = obj_work->parent_obj->pos.x;

	obj_work->pos.y  = parent_work->master_posy;
	obj_work->pos.y += obj_work->parent_obj->user_timer;	//	振動
	obj_work->pos.y += pwork->ofst_y;
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
static void gmGmkPressWallCreateRail(OBS_OBJECT_WORK *parent_obj,fx32 height,fx32 pos_y)
{
	OBS_OBJECT_WORK *obj_work;
	GMS_EFFECT_3DNN_WORK *eff_work;

	{
		//	上側レール
		obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_PRESSWALL_PARTS),
			                                             parent_obj,
			                                             0,
			                                             "PresswallRail-Top");
		eff_work = (GMS_EFFECT_3DNN_WORK*)obj_work;
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_presswall_obj_3d_list[IDB_GMK_PRESSWALL_MDL_GMK_P_RAIL_TOP_3_ZNO],
		                             &eff_work->obj_3d);
		// フラグ
		obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;			// 親付随なし
 		obj_work->pos.y = pos_y;
 		obj_work->pos.z = (fx32)(parent_obj->pos.z+1*FX32_ONE);

		obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
		obj_work->disp_flag |= OBD_DISP_NODIR;
		obj_work->disp_flag |= OBD_DISP_DRAWSTATE;

		((GMS_GMK_PRESSWALL_PARTS*)obj_work)->ofst_y = (fx32)(-2 * FX32_ONE);				// 座標補正
		obj_work->chgf(gmGmkPressWallRail);
	}
	{
		//	下側レール
		obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_PRESSWALL_PARTS),
			                                             parent_obj,
			                                             0,
			                                             "PresswallRail-Botom");
		eff_work = (GMS_EFFECT_3DNN_WORK*)obj_work;
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_presswall_obj_3d_list[IDB_GMK_PRESSWALL_MDL_GMK_P_RAIL_BTM_3_ZNO],
		                             &eff_work->obj_3d);
		// フラグ
		obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;			// 親付随なし
 		obj_work->pos.y = (fx32)(pos_y+height);
 		obj_work->pos.z = (fx32)(parent_obj->pos.z+1*FX32_ONE);

		obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
		obj_work->disp_flag |= OBD_DISP_NODIR;
		obj_work->disp_flag |= OBD_DISP_DRAWSTATE;

		((GMS_GMK_PRESSWALL_PARTS*)obj_work)->ofst_y = (fx32)(height - (16 * FX32_ONE));	// 座標補正
		obj_work->chgf(gmGmkPressWallRail);
	}
}
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
static void gmGmkPressWallZ4Parts_ppOut(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PRESSWALL_PARTS *pwork = (GMS_GMK_PRESSWALL_PARTS*)obj_work;

	fx32 height;

	obj_work->pos.y  = pwork->master_posy;
	//	全領域描画型
	while( obj_work->pos.y+GMD_GMK_PRESSWALL_HEIGHT < g_obj.camera[0][MTD_Y] )
	{
		obj_work->pos.y += GMD_GMK_PRESSWALL_HEIGHT;
	}
	while( obj_work->pos.y > g_obj.camera[0][MTD_Y] )
	{
		obj_work->pos.y -= GMD_GMK_PRESSWALL_HEIGHT;
	}
	height = obj_work->pos.y - g_obj.camera[0][MTD_Y];
	while( height < 256*FX32_ONE )
	{
		ObjDrawActionSummary(obj_work);
		obj_work->pos.y += GMD_GMK_PRESSWALL_HEIGHT;
		height += GMD_GMK_PRESSWALL_HEIGHT;
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if _IPHONE
#define GMK_Z4PRESSWALL_MDL_TBL_NUM  (3)
#else
#define GMK_Z4PRESSWALL_MDL_TBL_NUM  (4)
#endif 
static u32 tbl_gmk_z4PressWall_model[] =
{
#if _IPHONE
	IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_A_4_INO,
	IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_B_4_INO,
	IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_G_4_INO,
#else
	IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_A_4_ZNO,
	IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_B_4_ZNO,
	IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_LIGHT_4_ZNO,
	IDB_GMK_PRESSWALL4_MDL_GMK_P_WALL_G_4_ZNO,
#endif // _IPHONE
};
static fx32 tbl_gmk_z4PressWall_ofst_z[] =
{
	0*FX32_ONE,
	0*FX32_ONE,
#if !_IPHONE
	16*FX32_ONE,
#endif // !_IPHONE
	128*FX32_ONE,
};
static void gmGmkPressWallCreateParts(OBS_OBJECT_WORK *parent_obj,fx32 pos_y,fx32 height)
{
	OBS_OBJECT_WORK *obj_work;
	GMS_EFFECT_3DNN_WORK *eff_work;

	int i;
	for( i = 0; i < GMK_Z4PRESSWALL_MDL_TBL_NUM; i++ )
	{
		obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_PRESSWALL_PARTS),
			                                             parent_obj,
			                                             0,
			                                             "PresswallZ4Parts");

		eff_work = (GMS_EFFECT_3DNN_WORK*)obj_work;
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_presswall_obj_3d_list[tbl_gmk_z4PressWall_model[i]],
		                             &eff_work->obj_3d);
		// フラグ
		obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;			// 親付随なし
		obj_work->pos.y = pos_y;
		obj_work->pos.z = (fx32)(parent_obj->pos.z + tbl_gmk_z4PressWall_ofst_z[i]);

		obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
		obj_work->disp_flag |= OBD_DISP_NODIR;
		obj_work->disp_flag |= OBD_DISP_DRAWSTATE;
		obj_work->chgf(gmGmkPressWallParts);	//	処理なし

		GMS_GMK_PRESSWALL_PARTS *pwork = (GMS_GMK_PRESSWALL_PARTS*)obj_work;
		pwork->master_posy = pos_y;
		// 描画変更など
		if( height == 0 )
		{
			obj_work->ppOut = gmGmkPressWallZ4Parts_ppOut;
		}
	}
	//	最後の１つ＝G_4
	ObjAction3dNNMaterialMotionLoad( obj_work->obj_3d,
	                                 0,				//reg_file_id
	                                 NULL,			//data_work
	                                 NULL,			//mtn_data_path
	                                 GMD_GMK_PRESSWALL4_MAT_GLARE,
	                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_PRESSWALL_MAT)->pData );
	ObjDrawObjectActionSet3DNNMaterial( obj_work, GMD_GMK_PRESSWALL4_MAT_GLARE );

	obj_work->obj_3d->mat_speed = 1.0f;
	obj_work->disp_flag |= OBD_DISP_REPEAT;
}
// ===========================================================================


// ==========================================================================
// gmGmkPressWallExit
/*!
 *	ギミック 迫る壁終了処理
 *
 *	@param tcb		[in] tcbワーク
 */
// ==========================================================================
static void gmGmkPressWallExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK*	obj_work  = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	
	// 共通ポインタをクリア
	if (pwall == (GMS_GMK_PWALL_WORK*)obj_work) {
		pwall = NULL;
	}

	// SE停止
	gmGmkPressWallSeStop(obj_work);
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}

// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ===========================================================================
// GmGmkPressWallInit
/*!
	ギミック 迫る壁＠ゾーン３と４ 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// ===========================================================================
OBS_OBJECT_WORK* GmGmkPressWallInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_PWALL_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_PWALL_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_PWALL_WORK), "Gmk_PressWall");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	// モデル初期化
	//	ゾーン３用
	if( g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id] == GSD_MAIN_ZONE_TYPE_3 )
	{
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_presswall_obj_3d_list[IDB_GMK_PRESSWALL_MDL_GMK_P_WALL_3_ZNO],
		                             &gmk_work->obj_3d);
		// 描画変更
		obj_work->ppOut = gmGmkPressWall_ppOut;

		if( eve_rec->height == 0 )
		{
			pwork->wall_height = 0;
			obj_work->pos.y -= 128*FX32_ONE;
		}
		else
		{
			pwork->wall_height = (fx32)(eve_rec->height*64*FX32_ONE);
			obj_work->pos.y -= pwork->wall_height;
//			obj_work->pos.y -= (fx32)(pwork->wall_height + (/*16*/8<<FX32_SHIFT));	// 座標補正テスト
		}
		// 優先設定
		obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_N_BACK - FX32_ONE;
	}
	else
	{
	//	GSD_MAIN_ZONE_TYPE_4,

		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_presswall_obj_3d_list[GMD_GMK_PRESSWALL4_MDL],
		                             &gmk_work->obj_3d);
		// 描画変更
		obj_work->ppOut = gmGmkPressWall_ppOut;

#if _IPHONE
		ObjAction3dNNMaterialMotionLoad( obj_work->obj_3d,
		                                 0,				//reg_file_id
		                                 NULL,			//data_work
		                                 NULL,			//mtn_data_path
		                                 GMD_GMK_PRESSWALL4_MAT_ROLL,
		                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_PRESSWALL_MAT)->pData );
		ObjDrawObjectActionSet3DNNMaterial( obj_work, 0 );
#else
		// モーション初期化
		ObjObjectAction3dNNMotionLoad(obj_work,
		                              0/*reg_file_id*/,
		                              FALSE/*marge*/,
		                              ObjDataGet(GMD_DWORK_NO_GMK_PRESSWALL_MTN),
		                              NULL/*mtn_data_path*/,
		                              0/*index*/,
		                              NULL/*archive*/);
		ObjDrawObjectActionSet(obj_work, IDB_GMK_PRESSWALL4_MTN_GMK_P_WALL_4_ZNM);
#endif // _IPHONE
		obj_work->disp_flag |= OBD_DISP_REPEAT;

		if( eve_rec->height == 0 )
		{
			pwork->wall_height = 0;
			obj_work->pos.y -= 192*FX32_ONE;
		}
		else
		{
			pwork->wall_height = (fx32)(eve_rec->height*192*FX32_ONE);
			obj_work->pos.y -= pwork->wall_height;
		}
		// 優先設定
		obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_N - FX32_ONE;
	}

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE|OBD_MOVE_NOCOL;			// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag |= OBD_OBJECT_NOHIT;								// 矩形あたり無し◆

	//	パラメーター
	if( eve_rec->width )
	{
		pwork->wall_speed = (eve_rec->width*FX32_ONE)/10;
	}
	else
	{
		pwork->wall_speed = (fx32)(1.0*FX32_ONE);
	}

	// SE handle 開放のために、destructorは専用処理に置き換える
	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkPressWallExit);
	pwork->se_handle = NULL;
	
	gmGmkPressWallStart(obj_work);

	return obj_work;
}
// ===========================================================================



// ===========================================================================
// GmGmkPressWallStopInit
/*!
	ギミック 迫る壁＠ゾーン３と４ 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// ===========================================================================
OBS_OBJECT_WORK* GmGmkPressWallStopInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	if (  (g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id] == GSD_MAIN_ZONE_TYPE_3)
		&&(pwall == NULL ) ) {
		// リスタート時用の壁を生成
		obj_work = GmGmkPressWallInit(eve_rec ,pos_x, pos_y, type);	// event param はStopInitのものを利用
		obj_work->user_flag = TRUE;
	}

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "Gmk_PressWallStopper");
	obj_work->user_flag = FALSE;
	if( eve_rec->flag & 0x01 )
	{
		obj_work->user_flag = TRUE;
	}
	gmGmkPressWallStopperStart(obj_work);

	return obj_work;
}
// ===========================================================================



// ===========================================================================
// GmGmkPressWallControlInit
/*!
	ギミック 迫る壁＠ゾーン３と４ 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// ===========================================================================
OBS_OBJECT_WORK* GmGmkPressWallControlerInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_PWALLCTRL_WORK	*pwork;
	OBS_RECT_WORK *rect_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_PWALLCTRL_WORK), "Gmk_PressWallControler");

	MTM_ASSERT( eve_rec->left == 0 || eve_rec->top == 0 );
	MTM_ASSERT( eve_rec->left != 0 || eve_rec->top != 0 );

	pwork = (GMS_GMK_PWALLCTRL_WORK*)obj_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = NULL;
	rect_work->ppHit = NULL;
	rect_work->flag &= ~OBD_RECT_ENABLE;
	if( eve_rec->left != 0 )
	{
		//	横軸で設定
		// 矩形設定
		ObjRectWorkSet( rect_work,
		                (s16)(eve_rec->left*2),
		                0,
		                (s16)(eve_rec->width*2),
		                1);
		obj_work->user_flag = 0;
		obj_work->user_timer = eve_rec->height*((2*FX32_ONE)/10);
	}
	else
	{
		//	縦軸で設定
		rect_work->ppDef = NULL;
		rect_work->ppHit = NULL;
		// 矩形設定
		ObjRectWorkSet( rect_work,
		                0,
		                (s16)(eve_rec->top*2),
		                1,
		                (s16)(eve_rec->height*2));

		pwork->line_top = (eve_rec->top*2)*FX32_ONE + obj_work->pos.y;
		pwork->line_bottom = (eve_rec->height*2)*FX32_ONE + obj_work->pos.y;

		obj_work->user_flag = 1;
		obj_work->user_timer = eve_rec->width*((2*FX32_ONE)/10);
	}
	obj_work->flag &= ~OBD_OBJECT_NOHIT;							// 矩形あたりは無効

	if( eve_rec->flag & 0x01 )
		obj_work->user_flag |= 2;
	if( eve_rec->flag & 0x02 )
		obj_work->user_flag |= 4;

	gmGmkPressWallControlerStart(obj_work);

	return obj_work;
}
// ===========================================================================




// ===========================================================================
// GmGmkPressWallBuild
/*!
	ギミック 迫る壁＠ゾーン３と４ データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkPressWallBuild(void)
{
	gm_gmk_presswall_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PRESSWALL_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PRESSWALL_TEX),
								0/*draw_flag*/);

	pwall = NULL;
}
// ===========================================================================


// ===========================================================================
// GmGmkPressWallFlush
/*!
	ギミック 迫る壁＠ゾーン３と４ データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkPressWallFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_PRESSWALL_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_presswall_obj_3d_list, amb->file_num);
}
// ===========================================================================
