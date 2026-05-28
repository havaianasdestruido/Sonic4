// =======================================================================
/*!
	@file	gmGmkPopSteam.c
	@brief	ギミック ポップスチーム＠ゾーン４工場

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkPopSteam.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmPlySpec.h"
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmEffectCmn.h"
#include "gmPadVib.h"

#include "gmSound.h"

#include "gmGmkPopSteam.h"

// データヘッダ
#include "common/model/GMK_P_STEAM_MDL.HMB"



#define		GME_EFCT_CMN_IDX_STEAM_L_LOOP GME_EFCT_CMN_IDX_STEAM_L
#define		GME_EFCT_CMN_IDX_STEAM_M_LOOP GME_EFCT_CMN_IDX_STEAM_M
#define		GME_EFCT_CMN_IDX_STEAM_S_LOOP GME_EFCT_CMN_IDX_STEAM_S



// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）


// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_popsteam_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
#define	OBJWORK		COMWORK.obj_work
#define	chgf(f)		ppFunc = (f)
// -------------

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// -------------

// ----- Definitions -------------------------------------------（定数の宣言）
// ---------------------------------------------------------------------------
#define	GMD_POP_STEAM_MAIN_PRIO_Z	(8*FX32_ONE)
#define	GMD_POP_STEAM_STEAM_PRIO_Z	(1*FX32_ONE)
#define	GMD_POP_STEAM_STEAM_EFFECT_PRIO_Z	(16*FX32_ONE)


#define GMD_POP_STEAM_USE_DIR_MODEL	(1)	//向き別にモデルを変更する


// ---------------------------------------------------------------------------
// オブジェクトタイプ
typedef enum tag_GME_GMK_TYPE{

	GME_GMK_TYPE_POPSTEAM_U = 0,	// ↑射出
	GME_GMK_TYPE_POPSTEAM_R,		// →射出
	GME_GMK_TYPE_POPSTEAM_D,		// ↓射出
	GME_GMK_TYPE_POPSTEAM_L			// ←射出
}GME_GMK_TYPE;
#define GME_GMK_TYPE_MAX (GME_GMK_TYPE_POPSTEAM_L+1)
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// あたり判定矩形テーブル
typedef enum tag_GME_GMK_BWALL_RECT_DATA{

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
#define		GMD_GMK_POPSTEAM_RECT_FWD		(96)	//	前方・吹き飛ばす方向　値は標準。スチームの大きさで変える必要があります。
#define		GMD_GMK_POPSTEAM_RECT_BACK		( 0)	//	後方・射出口
#define		GMD_GMK_POPSTEAM_RECT_SIDE		( 8)	//	横、プラスマイナス違いで同値
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const static s16 tbl_gm_gmk_upbumper_rect[GME_GMK_TYPE_MAX][GME_GMK_RECT_DATA_MAX] = {

	//	ポップスチーム＠ゾーン４工場
	{
		//	吹き上げ当たり矩形　上向き
		  -GMD_GMK_POPSTEAM_RECT_SIDE,	//	左
		  -GMD_GMK_POPSTEAM_RECT_FWD,	//	上
		  +GMD_GMK_POPSTEAM_RECT_SIDE,	//	右
		   GMD_GMK_POPSTEAM_RECT_BACK,	//	下
	},
	{
		//	吹き上げ当たり矩形　右向き
		   GMD_GMK_POPSTEAM_RECT_BACK,	//	左
		  -GMD_GMK_POPSTEAM_RECT_SIDE,	//	上
		   GMD_GMK_POPSTEAM_RECT_FWD,	//	右
		  +GMD_GMK_POPSTEAM_RECT_SIDE,	//	下
	},
	{
		//	吹き上げ当たり矩形　下向き
		  -GMD_GMK_POPSTEAM_RECT_SIDE,	//	左
		  -GMD_GMK_POPSTEAM_RECT_BACK,	//	上
		  +GMD_GMK_POPSTEAM_RECT_SIDE,	//	右
		   GMD_GMK_POPSTEAM_RECT_FWD,	//	下
	},
	{
		//	吹き上げ当たり矩形　左向き
		  -GMD_GMK_POPSTEAM_RECT_FWD,	//	左
		  -GMD_GMK_POPSTEAM_RECT_SIDE,	//	上
		   GMD_GMK_POPSTEAM_RECT_BACK,	//	右
		  +GMD_GMK_POPSTEAM_RECT_SIDE,	//	下
	},
};
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//	当たり矩形大きさ定義
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	GMD_GMK_P_STEAM_INTTIME		(60)
// ※　インターバル型のポップスチームが蒸気を吐く時間
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// エフェクトスチームのアクション名定義
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static u16 tbl_popsteam_effct[2][3] =
{
	{
		 GME_EFCT_CMN_IDX_STEAM_S,
		 GME_EFCT_CMN_IDX_STEAM_M,
		 GME_EFCT_CMN_IDX_STEAM_L,
	},
	{
		GME_EFCT_CMN_IDX_STEAM_S_LOOP,
		GME_EFCT_CMN_IDX_STEAM_M_LOOP,
		GME_EFCT_CMN_IDX_STEAM_L_LOOP,
	},
};
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// タイプ別モデルID定義
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if	GMD_POP_STEAM_USE_DIR_MODEL
static const s32 tbl_popsteam_model_id[GME_GMK_TYPE_MAX][2] = {
	{IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_ZNO, IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_05_ZNO},
	{IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_02_ZNO, IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_06_ZNO},
	{IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_03_ZNO, IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_07_ZNO},
	{IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_04_ZNO, IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_08_ZNO},
};
static const s32 tbl_popsteam_pipe_model_id[GME_GMK_TYPE_MAX] = {
	IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_PIPE_01_ZNO,
	IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_PIPE_02_ZNO,
	IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_PIPE_03_ZNO,
	IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_PIPE_04_ZNO,
};
#endif	//GMD_POP_STEAM_USE_DIR_MODEL
// ---------------------------------------------------------------------------



// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef struct tag_GMS_GMK_P_STEAM_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	GME_GMK_TYPE		obj_type;		//!< 上向きか下向きかオブジェクトタイプ

	s16					timer;			//!< 汎用タイマー

	u16					steamvect;		//!< スチームの向き　上が0xC000
	s16					steamsize;		//!< スチームの大きさ
	s16					steamwait;		//!< スチームの噴出待ち時間

	fx32				steampower;		//!< スチームの威力

	u16					status;			//!< 当たったらＯＮに
	#define				GMD_GMK_PSTEAM_STAT_HIT		(0x0001)	// プレイヤーがHIT矩形内に居ればON
	#define				GMD_GMK_PSTEAM_STAT_TRUE	(0x0002)	// プレイヤーが継続HITしていればON

	GMS_PLAYER_WORK		*ply_work;

	OBS_OBJECT_WORK		*opt_timer;
	OBS_OBJECT_WORK		*opt_steam;
	OBS_OBJECT_WORK		*opt_steam_int[3];

	fx32				pos_x,pos_y;


}GMS_GMK_P_STEAM_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// ---------------------------------------------------------------------------



// ===========================================================================
// gmGmkPopSteam*
/*!
	ギミック ポップスチーム＠ゾーン４工場

	@note

 */
// ---------------------------------------------------------------------------
static void gmGmkPopSteamStay(OBS_OBJECT_WORK *obj_work);
static void gmGmkPopSteamHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkBeltPopSteam_ppOut
/*!
	ギミック ポップスチーム　＠ゾーン４工場 描画

	@note
		噴出前に少しの間揺らします
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static s8 tbl_psteam_viv[][MTD_XY] =
{
	{  0,  0 },		//
	{ +1,  0 },		//
	{  0,  0 },		//
	{ -1,  0 },		//
	{  0,  0 },		//
	{  0, +1 },		//
	{  0,  0 },		//
	{ +1, +1 },		//
	{  0,  0 },		//
	{ -1, +1 },		//
};
#define	GMD_GMK_PSTEAM_VIB_TBL_MAX	(sizeof(tbl_psteam_viv)/MTD_XY);
#if 0
static void gmGmkBeltPopSteam_ppOut(OBS_OBJECT_WORK *obj_work)
{
	if( !obj_work->user_flag )
	{
		ObjDrawActionSummary(obj_work);
		return;
	}

	fx32 px,py;
	px = obj_work->pos.x;
	py = obj_work->pos.y;

	obj_work->user_timer += 1;
	obj_work->user_timer %= GMD_GMK_PSTEAM_VIB_TBL_MAX;

	obj_work->pos.x += (tbl_psteam_viv[obj_work->user_timer][MTD_X]<<FX32_SHIFT);
	obj_work->pos.y += (tbl_psteam_viv[obj_work->user_timer][MTD_Y]<<FX32_SHIFT);
	obj_work->user_flag = FALSE;

	ObjDrawActionSummary(obj_work);
	obj_work->pos.x = px;
	obj_work->pos.y = py;
}
// ---------------------------------------------------------------------------
#endif

#if GMD_POP_STEAM_USE_DIR_MODEL
// ---------------------------------------------------------------------------
// gmGmkBeltPopSteam_ppOutUseDirModel
/*!
	ギミック ポップスチーム　＠ゾーン４工場 描画

	@note
		向き別にモデルを変更した場合の描画
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBeltPopSteam_ppOutUseDirModel(OBS_OBJECT_WORK *obj_work)
{
	u16 dir_z = obj_work->dir.z;
	obj_work->dir.z = 0;
	ObjDrawActionSummary(obj_work);
	obj_work->dir.z = dir_z;
}
#endif	//GMD_POP_STEAM_USE_DIR_MODEL

// ---------------------------------------------------------------------------
// gmGmkPopSteam*
/*!
	ギミック ポップスチーム

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void _gmGmkPopSteam(GMS_GMK_P_STEAM_WORK *pwork)
{
	GMS_PLAYER_WORK *ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if (pwork->status & GMD_GMK_PSTEAM_STAT_HIT) {
		if (ply_work->seq_state == GME_PLY_SEQ_STATE_TRANS_SUPER) {
			// スーパーソニック変身中ならヒット却下
			pwork->ply_work = NULL;
			pwork->status &= ~(GMD_GMK_PSTEAM_STAT_HIT | GMD_GMK_PSTEAM_STAT_TRUE);				// HIT情報OFF
			return;
		}

		// 矩形内にソニックあり
		// プレイヤーをジャンプ
		fx32 spd_x, spd_y;
		if( pwork->steamvect & 0x4000 )	//	上下？
		{
			//	上下！
			spd_x = 0;
			spd_y = pwork->steampower;
			if( pwork->steamvect & 0x8000 )	//	上？
				spd_y = -spd_y;
		}
		else
		{
			//	左右！
			spd_y = 0;
			spd_x = pwork->steampower;
			if( pwork->steamvect & 0x8000 )	//	左？
				spd_x = -spd_x;
		}
		GmPlySeqGmkInitPopSteamJump(ply_work, spd_x, spd_y, (s32)pwork->gmk_work.ene_com.eve_rec->top << (1/*HyenaOption!*/ + FX32_SHIFT));
															// 20091126 Dimps Ishizaki 移動不可時間追加
		if (!(pwork->status & GMD_GMK_PSTEAM_STAT_TRUE)) {
			// SE
			GmSoundPlaySE("Steam");
			// 振動
			GMM_PAD_VIB_SMALL();
		}
		pwork->status |= GMD_GMK_PSTEAM_STAT_TRUE;
	} else {
		// 矩形内にソニックなし
		pwork->status &= ~GMD_GMK_PSTEAM_STAT_TRUE;
	}
	pwork->status &= ~GMD_GMK_PSTEAM_STAT_HIT;				// HIT情報は毎回OFF
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPopSteamStay_100(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPopSteamStay(OBS_OBJECT_WORK *obj_work)
{
	obj_work->chgf(gmGmkPopSteamStay_100);
	gmGmkPopSteamStay_100(obj_work);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPopSteamStay_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_P_STEAM_WORK *pwork = (GMS_GMK_P_STEAM_WORK*)obj_work;
	//	常に噴出
	_gmGmkPopSteam(pwork);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPopSteamWait(OBS_OBJECT_WORK *obj_work);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPopSteamInterval_100(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPopSteamInterval(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_P_STEAM_WORK *pwork = (GMS_GMK_P_STEAM_WORK*)obj_work;

	obj_work->flag &= ~OBD_OBJECT_NOHIT;		// 矩形あたり復活

	pwork->opt_steam =
			(OBS_OBJECT_WORK*)GmEfctCmnEsCreate( obj_work, (GME_EFCT_CMN_IDX)tbl_popsteam_effct[0][pwork->steamsize] );
	pwork->opt_steam->dir.z = obj_work->dir.z;

	obj_work->chgf(gmGmkPopSteamInterval_100);
	gmGmkPopSteamInterval_100(obj_work);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPopSteamInterval_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_P_STEAM_WORK *pwork = (GMS_GMK_P_STEAM_WORK*)obj_work;
	//	噴出状態
	pwork->timer -= (pwork->steamwait/GMD_GMK_P_STEAM_INTTIME);
	if( pwork->timer <= 0 )
	{
		if( pwork->opt_steam )
		{
			ObjDrawKillAction3DES(pwork->opt_steam);
			pwork->opt_steam = NULL;
		}
		pwork->timer = 0;
		gmGmkPopSteamWait(obj_work);
		return;
	}
	_gmGmkPopSteam(pwork);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPopSteamWait_100(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPopSteamWait(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_P_STEAM_WORK *pwork = (GMS_GMK_P_STEAM_WORK*)obj_work;

	int i;
	for( i = 0; i < 3; i++ )
	{
		pwork->opt_steam_int[i] = NULL;
	}
	obj_work->flag |= OBD_OBJECT_NOHIT;		// 矩形あたり退避
	obj_work->chgf(gmGmkPopSteamWait_100);
	gmGmkPopSteamWait_100(obj_work);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static s8 tbl_steam_efct_ofst[4][3][MTD_XY] =
{
	{ { -16,+16 },{   0,+16 }, { +16,+16 } }, 	// ↑
	{ { -16,-16 },{ -16,  0 }, { -16,+16 } },	// →
	{ { +16,-16 },{   0,-16 }, { -16,-16 } },	// ↓
	{ { +16,+16 },{ +16,  0 }, { +16,-16 } },	// ←
};
static void gmGmkPopSteamWait_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_P_STEAM_WORK *pwork = (GMS_GMK_P_STEAM_WORK*)obj_work;
	pwork->timer += 1;
	if( pwork->timer >= pwork->steamwait )
	{
		if( pwork->opt_steam_int[0] != NULL )
		{
			int i;
			for( i = 0; i < 3; i++ )
			{
				ObjDrawKillAction3DES(pwork->opt_steam_int[i]);
				pwork->opt_steam_int[i] = NULL;
			}
		}
		obj_work->user_timer = 0;
		obj_work->pos.x = pwork->pos_x;
		obj_work->pos.y = pwork->pos_y;
		gmGmkPopSteamInterval(obj_work);
		return;
	}
	else if( pwork->timer >= (pwork->steamwait*3/4) )
	{
#if 0
		if( pwork->opt_steam_int[0] == NULL )
		{
			int i;
			for( i = 0; i < 3; i++ )
			{
				pwork->opt_steam_int[i] =
				(OBS_OBJECT_WORK*)GmEfctCmnEsCreate( obj_work, GME_EFCT_CMN_IDX_STEAM );

				pwork->opt_steam_int[i]->dir.z = (u16)(obj_work->dir.z - 0x2000+(i*0x2000));
				pwork->opt_steam_int[i]->pos.x = (fx32)(obj_work->pos.x+(tbl_steam_efct_ofst[pwork->obj_type][i][MTD_X]*FX32_ONE));
				pwork->opt_steam_int[i]->pos.y = (fx32)(obj_work->pos.y+(tbl_steam_efct_ofst[pwork->obj_type][i][MTD_Y]*FX32_ONE));
				pwork->opt_steam_int[i]->pos.z = (fx32)(obj_work->pos.z+GMD_POP_STEAM_STEAM_EFFECT_PRIO_Z);
//				GmEffect3DESAddDispOffset( (GMS_EFFECT_3DES_WORK*)pwork->opt_steam_int[i],
//				                            (float),
//				                            (float),
//				                            +10.0f );
			}
		}
#endif // 0
		obj_work->pos.x = (fx32)(pwork->pos_x+(tbl_psteam_viv[obj_work->user_timer][MTD_X]<<FX32_SHIFT));
		obj_work->pos.y = (fx32)(pwork->pos_y+(tbl_psteam_viv[obj_work->user_timer][MTD_Y]<<FX32_SHIFT));
//		obj_work->ofst.z = 16*FX32_ONE;
		obj_work->user_timer += 1;
		obj_work->user_timer %= GMD_GMK_PSTEAM_VIB_TBL_MAX;
	}
	_gmGmkPopSteam(pwork);
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkPopSteamStart
/*!
	ギミック ポップスチーム　サブ初期化

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void create_steampipe(OBS_OBJECT_WORK *parent_obj, GME_GMK_TYPE obj_type);
static OBS_OBJECT_WORK* create_steamtimer(OBS_OBJECT_WORK *parent_obj, GME_GMK_TYPE obj_type);

static void gmGmkPopSteamStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_P_STEAM_WORK *pwork = (GMS_GMK_P_STEAM_WORK*)obj_work;
	OBS_RECT_WORK *rect_work;

	// 矩形設定
	// 対プレイヤー
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = gmGmkPopSteamHit;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 被破壊矩形設定
	ObjRectWorkSet(rect_work,
	               tbl_gm_gmk_upbumper_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_LEFT],
	               tbl_gm_gmk_upbumper_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_TOP],
	               tbl_gm_gmk_upbumper_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_RIGHT],
	               tbl_gm_gmk_upbumper_rect[pwork->obj_type][GME_GMK_RECT_DATA_DEF_BOTTOM]);
	rect_work->flag &= ~OBD_RECT_OUT;			// 要連続判定
	obj_work->flag &= ~OBD_OBJECT_NOHIT;								// 矩形あたり無し◆

	s16	*rect = &rect_work->rect.top;	// 初期値はdummy
	switch(pwork->obj_type)
	{
		case GME_GMK_TYPE_POPSTEAM_U:		// ↑射出
			rect = &rect_work->rect.top;
			obj_work->dir.z = 0x0000;
			pwork->steamvect = 0xc000;
			break;

		case GME_GMK_TYPE_POPSTEAM_R:		// →射出
			rect = &rect_work->rect.right;
			obj_work->dir.z = 0x4000;
			pwork->steamvect = 0x0000;
			break;

		case GME_GMK_TYPE_POPSTEAM_D:		// ↓射出
			rect = &rect_work->rect.bottom;
			obj_work->dir.z = 0x8000;
			pwork->steamvect = 0x4000;
			break;

		case GME_GMK_TYPE_POPSTEAM_L:		// ←射出
			rect = &rect_work->rect.left;
			obj_work->dir.z = 0xc000;
			pwork->steamvect = 0x8000;
			break;
	}

	if( pwork->steamsize == 2 )
	{
		//	高い
		*rect /= 96;
		*rect *= 128;
	}
	else if( pwork->steamsize == 0 )
	{
		//	低い
		*rect /= 96;
		*rect *= 64;
	}

	pwork->timer = 0;
	pwork->status = 0;
	pwork->opt_steam = NULL;

	pwork->pos_x = obj_work->pos.x;
	pwork->pos_y = obj_work->pos.y;

	//	同期
	if( pwork->steamwait > 0 )
	{
		u32 sync_timer = g_gm_main_system.sync_time;

		sync_timer %= (pwork->steamwait+GMD_GMK_P_STEAM_INTTIME);
		if( sync_timer < GMD_GMK_P_STEAM_INTTIME )
		{
			pwork->timer = (s16)((sync_timer*pwork->steamwait)/GMD_GMK_P_STEAM_INTTIME);
			gmGmkPopSteamInterval(obj_work);
		}
		else
		{
			pwork->timer = (s16)(pwork->steamwait-(sync_timer-GMD_GMK_P_STEAM_INTTIME));
			gmGmkPopSteamWait(obj_work);
		}

		// 描画変更
//		obj_work->ppOut = gmGmkBeltPopSteam_ppOut;

		//	オプションビルド
		//	パイプ
		create_steampipe(obj_work, pwork->obj_type);
		pwork->opt_timer = create_steamtimer(obj_work, pwork->obj_type);
	}
	else
	{
		OBS_OBJECT_WORK *effobj_work = 
			(OBS_OBJECT_WORK*)GmEfctCmnEsCreate( obj_work, (GME_EFCT_CMN_IDX)tbl_popsteam_effct[1][pwork->steamsize] );
		effobj_work->dir.z = obj_work->dir.z;
		effobj_work->pos.z -= GMD_POP_STEAM_STEAM_PRIO_Z;
		gmGmkPopSteamStay(obj_work);
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkPopSteamHit
/*!
	ギミック ポップスチーム プレイヤー接触

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPopSteamHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_GMK_P_STEAM_WORK *pwork = (GMS_GMK_P_STEAM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	//	一応プレイヤーが当たったかどうかを確認
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] )
	{
		pwork->ply_work = ply_work;
		pwork->status |= GMD_GMK_PSTEAM_STAT_HIT;							// HITした事をメイン処理に通知
	}
	// 一応ヒットしなかった事にする
	mine_rect->flag &= ~(OBD_RECT_DAMAGE | OBD_RECT_FRAMEHIT | OBD_RECT_FRAMEOUT);
}
// ---------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================



// ==========================================================================
// ---------------------------------------------------------------------------
typedef struct tag_GMS_GMK_POPSTEAMPARTS_WORK
{
	GMS_EFFECT_3DNN_WORK eff_work;
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	fx32	z_off;

}GMS_GMK_POPSTEAMPARTS_WORK;
#define	OBJ_3D		eff_work.obj_3d
#define	COMWORK		eff_work.ene_com
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
static fx32 tbl_popsteam_pipe_off[][MTD_XY] =
{
	{ 0,             +96*FX32_ONE },	// GME_GMK_TYPE_POPSTEAM_U
	{ -96*FX32_ONE, 0             },	// GME_GMK_TYPE_POPSTEAM_R
	{ 0,             -96*FX32_ONE },	// GME_GMK_TYPE_POPSTEAM_D
	{ +96*FX32_ONE, 0             },	// GME_GMK_TYPE_POPSTEAM_L
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void create_steampipe(OBS_OBJECT_WORK *parent_obj,GME_GMK_TYPE obj_type)
{
	GMS_GMK_POPSTEAMPARTS_WORK *pwork;
	OBS_OBJECT_WORK *obj_work;
	obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_POPSTEAMPARTS_WORK),
	                                    NULL,
	                                    0,
	                                   "Gmk_PopSteamPipe");

	pwork = (GMS_GMK_POPSTEAMPARTS_WORK*)obj_work;
	// モデル初期化
#if	GMD_POP_STEAM_USE_DIR_MODEL
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_popsteam_obj_3d_list[tbl_popsteam_pipe_model_id[obj_type]],
	                             &pwork->eff_work.obj_3d);
	obj_work->ppOut = gmGmkBeltPopSteam_ppOutUseDirModel;
#else
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_popsteam_obj_3d_list[IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_R_ZNO],
	                             &pwork->eff_work.obj_3d);
	obj_work->ppOut
#endif	//GMD_POP_STEAM_USE_DIR_MODEL

	obj_work->parent_obj = parent_obj;

	obj_work->pos.x = parent_obj->pos.x + tbl_popsteam_pipe_off[obj_type][MTD_X];
	obj_work->pos.y = parent_obj->pos.y + tbl_popsteam_pipe_off[obj_type][MTD_Y];
	obj_work->pos.z = parent_obj->pos.z - 16*FX32_ONE;
	obj_work->dir.z = parent_obj->dir.z;
	// フラグ
	obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;		// 佇む
	obj_work->move_flag |= OBD_MOVE_NOCOL;			// 移動無し 地形あたりチェック無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->disp_flag &= ~OBD_DISP_NODIR;
	obj_work->chgf(NULL);		//	ジョブなし
}
// ---------------------------------------------------------------------------





// ---------------------------------------------------------------------------
// gmGmkPopSteamTimer
/*!
	ギミック ポップスチーム タイマー

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPopSteamTimer(OBS_OBJECT_WORK *obj_work)
{
//	GMS_GMK_POPSTEAMPARTS_WORK *pwork = (GMS_GMK_POPSTEAMPARTS_WORK*)obj_work;
	GMS_GMK_P_STEAM_WORK *parent = (GMS_GMK_P_STEAM_WORK*)obj_work->parent_obj;

	u32 wait_time = 0x10000;
	fx32 offz = (fx32)(1 * FX32_ONE);

	wait_time *= parent->timer;
	wait_time /= parent->steamwait;

//	offz = (fx32)(obj_work->dir.z);	// 回転角度をＺオフセットの基準にする事意味がないようなのでコメントアウト@kuramoto/09.12.03
	obj_work->dir.z = obj_work->parent_obj->dir.z;
	obj_work->dir.z += (u16)wait_time;
	offz += (fx32)wait_time;

	obj_work->parent_ofst.z = (fx32)(offz>>3);

	obj_work->ofst.x = obj_work->parent_obj->ofst.x;
	obj_work->ofst.y = obj_work->parent_obj->ofst.x;

}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
static fx32 tbl_popsteam_timer_off[][MTD_XY] =
{
	{ 0,             +41*FX32_ONE },	// GME_GMK_TYPE_POPSTEAM_U
	{ -41*FX32_ONE, 0             },	// GME_GMK_TYPE_POPSTEAM_R
	{ 0,             -41*FX32_ONE },	// GME_GMK_TYPE_POPSTEAM_D
	{ +41*FX32_ONE, 0             },	// GME_GMK_TYPE_POPSTEAM_L
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static OBS_OBJECT_WORK* create_steamtimer(OBS_OBJECT_WORK *parent_obj,GME_GMK_TYPE obj_type)
{
	GMS_GMK_POPSTEAMPARTS_WORK *pwork;
	OBS_OBJECT_WORK *obj_work;
	obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_POPSTEAMPARTS_WORK),
	                                    NULL,
	                                    0,
	                                   "Gmk_PopSteamTimer");

	pwork = (GMS_GMK_POPSTEAMPARTS_WORK*)obj_work;
	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_popsteam_obj_3d_list[IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_RED_ZNO],
	                             &pwork->eff_work.obj_3d);

	obj_work->parent_obj = parent_obj;

	obj_work->parent_ofst.x = tbl_popsteam_timer_off[obj_type][MTD_X];
	obj_work->parent_ofst.y = tbl_popsteam_timer_off[obj_type][MTD_Y];
	obj_work->parent_ofst.z = 0;
	obj_work->dir.z = parent_obj->dir.z;
	obj_work->flag |= OBD_OBJECT_PARENT_FIX;		// 吸着

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;			// 移動無し 地形あたりチェック無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->disp_flag &= ~OBD_DISP_NODIR;
	obj_work->chgf(gmGmkPopSteamTimer);

	return obj_work;
}
// ---------------------------------------------------------------------------

// ==========================================================================



// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkPopSteam?Init
/*!
 *	ギミック ポップスチーム＠ゾーン４工場 初期化関数
 *	GmGmkPopSteamUInit 上向き
 *	GmGmkPopSteamDInit 下向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
static OBS_OBJECT_WORK* gmGmkPopSteamInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type, GME_GMK_TYPE obj_type )
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_P_STEAM_WORK), "Gmk_PopSteam");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// モデル初期化
#if	GMD_POP_STEAM_USE_DIR_MODEL
	s32 id = tbl_popsteam_model_id[obj_type][0];
	if( eve_rec->height == 0 )
		id = tbl_popsteam_model_id[obj_type][1];

	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_popsteam_obj_3d_list[id],
	                             &gmk_work->obj_3d);
	obj_work->ppOut = gmGmkBeltPopSteam_ppOutUseDirModel;
#else
	u32 id = IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_ZNO;
	if( eve_rec->height == 0 )
		id = IDB_GMK_P_STEAM_MDL_GMK_P_STEAM_05_ZNO;

	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_popsteam_obj_3d_list[id],
	                             &gmk_work->obj_3d);
#endif	//GMD_POP_STEAM_USE_DIR_MODEL
	// 優先設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT - GMD_POP_STEAM_MAIN_PRIO_Z;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	//	ローカル設定
	GMS_GMK_P_STEAM_WORK *pwork = (GMS_GMK_P_STEAM_WORK*)obj_work;

	if( (eve_rec->flag & 0x03) == 0x02 )
	{
		//	高い
		pwork->steamsize = 2;
	}
	else if( (eve_rec->flag & 0x03) == 0x01 )
	{
		//	低い
		pwork->steamsize = 0;
	}
	else
		pwork->steamsize = 1;

	pwork->steampower = eve_rec->width*2;	//HyenaOption!
	pwork->steampower <<= FX32_SHIFT;
	if( pwork->steampower == 0 )
		pwork->steampower = GMD_PL_DEF_MAX_SPD;

	pwork->steamwait = (s16)(eve_rec->height*2);	//HyenaOption!
	pwork->obj_type = obj_type;

	return obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPopSteamUInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK *obj_work;

	obj_work = gmGmkPopSteamInit(eve_rec, pos_x, pos_y, type, GME_GMK_TYPE_POPSTEAM_U);
	obj_work->chgf(gmGmkPopSteamStart);

	return obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPopSteamRInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK *obj_work;

	obj_work = gmGmkPopSteamInit(eve_rec, pos_x, pos_y, type, GME_GMK_TYPE_POPSTEAM_R);
	obj_work->dir.z = 0x4000;
	obj_work->chgf(gmGmkPopSteamStart);

	return obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPopSteamDInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK *obj_work;

	obj_work = gmGmkPopSteamInit(eve_rec, pos_x, pos_y, type, GME_GMK_TYPE_POPSTEAM_D);
	obj_work->dir.z = 0x8000;
	obj_work->chgf(gmGmkPopSteamStart);

	return obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPopSteamLInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK *obj_work;

	obj_work = gmGmkPopSteamInit(eve_rec, pos_x, pos_y, type, GME_GMK_TYPE_POPSTEAM_L);
	obj_work->dir.z = 0xc000;
	obj_work->chgf(gmGmkPopSteamStart);

	return obj_work;
}
// ==========================================================================



// ===========================================================================
// GmGmkPopSteamBuild
/*!
	ギミック ポップスチーム＠ゾーン４工場 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkPopSteamBuild(void)
{
	gm_gmk_popsteam_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_POPSTEAM_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_POPSTEAM_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkPopSteamFlush
/*!
	ギミック ポップスチーム＠ゾーン４工場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkPopSteamFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_POPSTEAM_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_popsteam_obj_3d_list, amb->file_num);
}
// ===========================================================================
