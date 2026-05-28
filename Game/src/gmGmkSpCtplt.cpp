// =======================================================================
/*!
	@file	gmGmkSpCtplt.c
	@brief	ギミック スプリングカタパ＠ゾーン２

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkSpCtplt.cpp 2 2011-04-11 05:21:26Z thamada $
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

#include "gmSound.h"

#include "gmGmkSpCtplt.h"

// データヘッダ
#include "common/model/GMK_SP_CTPLT_MDL.HMB"
#include "common/model/GMK_SP_CTPLT_MTN.HMB"
#include "common/model/GMK_SP_CTPLT_MAT.HMB"



// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void gmGmkSpCtpltSeStop(OBS_OBJECT_WORK *obj_work);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_spctplt_obj_3d_list = NULL;

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

	GME_GMK_TYPE_SPCTPLT_0 = 0,	// 水平
	GME_GMK_TYPE_SPCTPLT_45,	// 右射出
	GME_GMK_TYPE_SPCTPLT_315,	// 左射出

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
#define		GMD_GMK_SPCTPLT_0_RECT_W		(4)
#define		GMD_GMK_SPCTPLT_0_RECT_H		(16)
#define		GMD_GMK_SPCTPLT_0_RECT_X		(-GMD_GMK_SPCTPLT_0_RECT_W/2)
#define		GMD_GMK_SPCTPLT_0_RECT_Y		(-64)

#define		GMD_GMK_SPCTPLT_45_RECT_W		(16)
#define		GMD_GMK_SPCTPLT_45_RECT_H		(16)
#define		GMD_GMK_SPCTPLT_45_RECT_X		(+45-GMD_GMK_SPCTPLT_315_RECT_W)
#define		GMD_GMK_SPCTPLT_45_RECT_Y		(-45)

#define		GMD_GMK_SPCTPLT_315_RECT_W		(16)
#define		GMD_GMK_SPCTPLT_315_RECT_H		(16)
#define		GMD_GMK_SPCTPLT_315_RECT_X		(-45)
#define		GMD_GMK_SPCTPLT_315_RECT_Y		(-45)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const static s16 tbl_gm_gmk_spctplt_rect[GME_GMK_TYPE_MAX][GME_GMK_RECT_DATA_MAX] = {

	//	スプリングカタパ＠ゾーン２
	{
		//	テスト土地あたり矩形
		  GMD_GMK_SPCTPLT_0_RECT_X,		//	左
		  GMD_GMK_SPCTPLT_0_RECT_Y,		//	上
		  GMD_GMK_SPCTPLT_0_RECT_X+GMD_GMK_SPCTPLT_0_RECT_W,		//	右
		  GMD_GMK_SPCTPLT_0_RECT_Y+GMD_GMK_SPCTPLT_0_RECT_H,		//	下
	},
	//	スプリングカタパ＠ゾーン２
	{
		//	テスト土地あたり矩形
		  GMD_GMK_SPCTPLT_45_RECT_X,		//	左
		  GMD_GMK_SPCTPLT_45_RECT_Y,		//	上
		  GMD_GMK_SPCTPLT_45_RECT_X+GMD_GMK_SPCTPLT_45_RECT_W,		//	右
		  GMD_GMK_SPCTPLT_45_RECT_Y+GMD_GMK_SPCTPLT_45_RECT_H,		//	下
	},
	//	スプリングカタパ＠ゾーン２
	{
		//	テスト土地あたり矩形
		  GMD_GMK_SPCTPLT_315_RECT_X,		//	左
		  GMD_GMK_SPCTPLT_315_RECT_Y,		//	上
		  GMD_GMK_SPCTPLT_315_RECT_X+GMD_GMK_SPCTPLT_315_RECT_W,		//	右
		  GMD_GMK_SPCTPLT_315_RECT_Y+GMD_GMK_SPCTPLT_315_RECT_H,		//	下
	},
};
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// スプリング設定
#define		GMD_GMK_SPCTPLT_HEIGHT		(64+14)		//	カタパルト全長＋ソニックへそ
#define		GMD_GMK_SPCTPLT_SHRINK		(22+14)		//	カタパルト全長＋ソニックへそ
#define		GMD_GMK_SPCTPLT_LENGTH		(GMD_GMK_SPCTPLT_HEIGHT-GMD_GMK_SPCTPLT_SHRINK)
// ---------------------------------------------------------------------------



// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef struct tag_GMS_GMK_SPCTPLT_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	u16		ctplt_id;
	u16		ctplt_tilt;
	int		ctplt_return_timer;

	fx32	ctplt_height;

	GMS_PLAYER_WORK *ply_work;

	GSS_SND_SE_HANDLE*	se_handle;

}GMS_GMK_SPCTPLT_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// ---------------------------------------------------------------------------


// ===========================================================================
// gmGmkSpCtplt*
/*!
	ギミック スプリングカタパ＠ゾーン２

	@note
		ソニックの位置を判定し、入れない場合は土地当たりを有効に、
		スタート位置に入った場合はソニックをコントロールする必要があります。

		とりあえず存在と当たり判定と土地当たりを作成。
 */
// ---------------------------------------------------------------------------
static void gmGmkSpCtpltStay(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpCtplt_PlayerHold(OBS_OBJECT_WORK *obj_work);

static void gmGmkSpCtplt_PlayerHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSpCtpltStart
/*!
	ギミック スプリングカタパ　サブ初期化

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpCtpltStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPCTPLT_WORK *pwork = (GMS_GMK_SPCTPLT_WORK*)obj_work;
	OBS_RECT_WORK *rect_work;

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work,
	                              0/*reg_file_id*/,
	                              FALSE/*marge*/,
	                              ObjDataGet(GMD_DWORK_NO_GMK_SPCTPLT_MTN),
	                              NULL/*mtn_data_path*/,
	                              0/*index*/,
	                              NULL/*archive*/);
	ObjDrawObjectActionSet(obj_work, IDB_GMK_SP_CTPLT_MTN_GMK_SP_CTPLT_FW_ZNM);

	ObjObjectAction3dNNMaterialMotionLoad( obj_work,
	                                       0,				//reg_file_id
	                                       ObjDataGet(GMD_DWORK_NO_GMK_SPCTPLT_MAT),	//data_work
	                                       NULL,			//filename
	                                       0,				//index
	                                       NULL );			//archive
	obj_work->obj_3d->mat_speed = 1.0f;

	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_SP_CTPLT_MAT_GMK_SP_CTPLT_GROW_ZNV );
	obj_work->disp_flag &= ~OBD_DISP_REPEAT;

	// チェック用矩形
	// 対プレイヤー
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;
	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = NULL;
	rect_work->ppHit = gmGmkSpCtplt_PlayerHit;
	// 矩形設定
	ObjRectWorkSet( rect_work,
	                tbl_gm_gmk_spctplt_rect[pwork->ctplt_id][GME_GMK_RECT_DATA_DEF_LEFT],
	                tbl_gm_gmk_spctplt_rect[pwork->ctplt_id][GME_GMK_RECT_DATA_DEF_TOP],
	                tbl_gm_gmk_spctplt_rect[pwork->ctplt_id][GME_GMK_RECT_DATA_DEF_RIGHT],
	                tbl_gm_gmk_spctplt_rect[pwork->ctplt_id][GME_GMK_RECT_DATA_DEF_BOTTOM] );
	obj_work->flag &= ~OBD_OBJECT_NOHIT;							// 矩形あたり有効

	obj_work->dir.z = pwork->ctplt_tilt;

	pwork->ply_work = NULL;
	pwork->ctplt_height = GMD_GMK_SPCTPLT_HEIGHT*FX32_ONE;
	obj_work->chgf(gmGmkSpCtpltStay);
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSpCtpltStay
/*!
	ギミック スプリングカタパ

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpCtpltStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPCTPLT_WORK *pwork = (GMS_GMK_SPCTPLT_WORK*)obj_work;
	GMS_PLAYER_WORK *ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	if( pwork->ply_work == ply_work )
	{
		obj_work->chgf(gmGmkSpCtplt_PlayerHold);
		gmGmkSpCtplt_PlayerHold(obj_work);
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSpCtplt_PlayerHold
/*!
	ギミック スプリングカタパ

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpCtplt_PlayerHold_100(OBS_OBJECT_WORK *obj_work);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpCtplt_PlayerHold(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPCTPLT_WORK *pwork = (GMS_GMK_SPCTPLT_WORK*)obj_work;
	GMS_PLAYER_WORK *ply_work = pwork->ply_work;

	fx32 ofst_x,ofst_y;

	if (  (ply_work->player_flag & GMD_PLF_DIE)
		||(g_gm_main_system.game_flag & (GMD_GAME_FLAG_TIMEOVER				// タイムオーバー死亡
										| GMD_GAME_FLAG_SPL_TIMEOVER) ) ) {	// スペステ：タイムオーバー
		// 死亡時は一切処理しない
		return;
	}
	if (g_gm_main_system.game_flag & ( GMD_GAME_FLAG_START_DEMO			// スタートデモ中
									 | GMD_GAME_FLAG_START_MSG ) ) {	// ゲーム開始時メッセージ表示
		// 移動不可状態では操作系見ない
	}
	else if( ply_work->key_release & PAD_BUTTON_JUMP )
	{
		fx32 spdx,spdy;

		spdx = 0;
		spdy = (GMD_GMK_SPCTPLT_HEIGHT*FX32_ONE-pwork->ctplt_height)/2;
		ply_work->obj_work.spd_m = spdy;
		if( pwork->ctplt_tilt != 0x0000 )
		{
			//	45°
			spdx = spdy = 
				(fx32)((2896.3093*spdy)/4096);
			if( pwork->ctplt_tilt == 0xe000 )
				spdx = -spdx;		//315°
		}
		else
		{
			if( spdy < 2*FX32_ONE )
			{
				spdy = 2*FX32_ONE;
			}
			else if( spdy > 9*FX32_ONE )
			{
				spdy = 9*FX32_ONE+((9*spdy)/(GMD_GMK_SPCTPLT_LENGTH/2));
			}
		}
		ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_SP_CTPLT_MAT_GMK_SP_CTPLT_GROW_ZNV );
		obj_work->disp_flag &= ~OBD_DISP_REPEAT;
		ObjDrawObjectActionSet(obj_work, IDB_GMK_SP_CTPLT_MTN_GMK_SP_CTPLT_RUN_ZNM);
		pwork->ctplt_height = GMD_GMK_SPCTPLT_HEIGHT*FX32_ONE;

		// プレイヤーをジャンプ
		ply_work->obj_work.dir.z = (u16)(pwork->ctplt_tilt + 0xc000);
		GmPlySeqInitPinballCtplt( ply_work, spdx, -spdy );
		// 振動
		GMM_PAD_VIB_SMALL();

		obj_work->chgf(gmGmkSpCtplt_PlayerHold_100);
		pwork->ctplt_return_timer = 4;
		pwork->ply_work = NULL;

		// SE停止
		gmGmkSpCtpltSeStop(obj_work);
	}
	else if( ply_work->key_on & PAD_BUTTON_JUMP )
	{
		if( pwork->ctplt_height == GMD_GMK_SPCTPLT_HEIGHT*FX32_ONE )
		{
			// 溜め始め
			ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_SP_CTPLT_MAT_GMK_SP_CTPLT_SHRINK_ZNV );
			obj_work->disp_flag &= ~OBD_DISP_REPEAT;
			ObjDrawObjectActionSet(obj_work, IDB_GMK_SP_CTPLT_MTN_GMK_SP_CTPLT_SHRINK_ZNM);
			// SE handle取得＆コール
			pwork->se_handle = GsSoundAllocSeHandle();
			GmSoundPlaySE("Catapult1", pwork->se_handle);	// SEコール
		}
		if( pwork->ctplt_height > GMD_GMK_SPCTPLT_SHRINK*FX32_ONE )
		{
			pwork->ctplt_height -= ((GMD_GMK_SPCTPLT_LENGTH*FX32_ONE)/57);	//縮むまでのフレーム数+誤差吸収
			if( pwork->ctplt_height < GMD_GMK_SPCTPLT_SHRINK*FX32_ONE )
				pwork->ctplt_height = GMD_GMK_SPCTPLT_SHRINK*FX32_ONE;
		}
	}

	if( pwork->ctplt_tilt == 0x0000 )
	{
		ofst_x = 0;
		ofst_y = -pwork->ctplt_height;
	}
	else
	{
		//	45°
		ofst_x = ofst_y = 
			-(fx32)((2896.3093*pwork->ctplt_height)/4096);
		if( pwork->ctplt_tilt == 0x2000 )
			ofst_x = -ofst_x;	//315°
	}
	ply_work->obj_work.pos.x = obj_work->pos.x + ofst_x;
	ply_work->obj_work.pos.y = obj_work->pos.y + ofst_y;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpCtplt_PlayerHold_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPCTPLT_WORK *pwork = (GMS_GMK_SPCTPLT_WORK*)obj_work;

	pwork->ctplt_return_timer -= 1;
	if( pwork->ctplt_return_timer <= 0 )
	{
		obj_work->flag &= ~OBD_OBJECT_NOHIT;					// 射出まで当たり無効
		obj_work->chgf(gmGmkSpCtpltStay);
		gmGmkSpCtpltStay(obj_work);
	}
}
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkSpCtplt_PlayerHit
/*!
	ギミック スプリングカタパルト プレイヤー接触

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpCtplt_PlayerHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	if (  (ply_work->player_flag & GMD_PLF_DIE)								// 死亡
		||(ply_work->obj_work.flag & OBD_OBJECT_NOHIT)						// ヒット判定無し
		||(g_gm_main_system.game_flag & (GMD_GAME_FLAG_TIMEOVER				// タイムオーバー死亡
										| GMD_GAME_FLAG_SPL_TIMEOVER) ) ) {	// スペステ：タイムオーバー
		return;
	}

	//	一応プレイヤーが当たったかどうかを確認
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] )
	{
		GMS_GMK_SPCTPLT_WORK *pwork = (GMS_GMK_SPCTPLT_WORK*)obj_work;
//		GmPlySeqGmkInitSeesaw(ply_work, &pwork->COMWORK);	//	仮
		GmPlySeqInitPinballCtpltHold(ply_work, &pwork->COMWORK);

		ply_work->obj_work.flag |= OBD_OBJECT_NOHIT;
		obj_work->flag |= OBD_OBJECT_NOHIT;					// 射出まで当たり無効

		pwork->ply_work = ply_work;		//	発動
	}
}
// ---------------------------------------------------------------------------





// ---------------------------------------------------------------------------
//	gmGmkSpCtpltSeStop
/*!
 *	スプリングカタパルトループSE停止
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSpCtpltSeStop(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SPCTPLT_WORK *pwork = (GMS_GMK_SPCTPLT_WORK*)obj_work;

	if (pwork->se_handle) {
		GsSoundStopSeHandle(pwork->se_handle);
		GsSoundFreeSeHandle(pwork->se_handle);
		pwork->se_handle = NULL;
	}
}
// ---------------------------------------------------------------------------
// gmGmkSpCtpltExit
/*!
 *	ギミック スプリングカタパルト終了処理
 *
 *	@param tcb		[in] tcbワーク
 */
// ---------------------------------------------------------------------------
static void gmGmkSpCtpltExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK*	obj_work  = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	
	// SE停止
	gmGmkSpCtpltSeStop(obj_work);
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}

// ---------------------------------------------------------------------------
// gmGmkSpCtpltInit
/*!
 *	ギミック スプリングカタパ＠ゾーン２ 共通初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ---------------------------------------------------------------------------
static OBS_OBJECT_WORK* gmGmkSpCtpltInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_SPCTPLT_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_SPCTPLT_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_SPCTPLT_WORK), "Gmk_Seesaw");
	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;
	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_spctplt_obj_3d_list[IDB_GMK_SP_CTPLT_MDL_GMK_SP_CTPLT_FW_ZNO],
	                             &gmk_work->obj_3d);

	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// SE handle 開放のために、destructorは専用処理に置き換える
	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkSpCtpltExit);
	pwork->se_handle = NULL;

	return obj_work;
}
// ---------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================



// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkSpCtplt?Init
/*!
 *	ギミック スプリングカタパ＠ゾーン２ 初期化関数
 *	GmGmkSpCtplt0Init   上向き
 *	GmGmkSpCtplt45Init  右向き
 *	GmGmkSpCtplt315Init 左向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSpCtplt0Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_SPCTPLT_WORK	*pwork;

	pwork = (GMS_GMK_SPCTPLT_WORK*)gmGmkSpCtpltInit(eve_rec, pos_x, pos_y, type);
	pwork->ctplt_tilt = 0x0000;
	pwork->ctplt_id = GME_GMK_TYPE_SPCTPLT_0;	// 垂直

	gmGmkSpCtpltStart(&pwork->gmk_work.ene_com.obj_work);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSpCtplt45Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_SPCTPLT_WORK	*pwork;

	pwork = (GMS_GMK_SPCTPLT_WORK*)gmGmkSpCtpltInit(eve_rec, pos_x, pos_y, type);
	pwork->ctplt_tilt = 0x2000;
	pwork->ctplt_id = GME_GMK_TYPE_SPCTPLT_45;	// 右射出

	gmGmkSpCtpltStart(&pwork->gmk_work.ene_com.obj_work);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSpCtplt315Init(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_SPCTPLT_WORK	*pwork;

	pwork = (GMS_GMK_SPCTPLT_WORK*)gmGmkSpCtpltInit(eve_rec, pos_x, pos_y, type);
	pwork->ctplt_tilt = 0xe000;
	pwork->ctplt_id = GME_GMK_TYPE_SPCTPLT_315;	// 左射出

	gmGmkSpCtpltStart(&pwork->gmk_work.ene_com.obj_work);

	return &pwork->gmk_work.ene_com.obj_work;
}
// ==========================================================================



// ===========================================================================
// GmGmkSpCtpltBuild
/*!
	ギミック スプリングカタパ＠ゾーン２ データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkSpCtpltBuild(void)
{
	gm_gmk_spctplt_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPCTPLT_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPCTPLT_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkSpCtpltFlush
/*!
	ギミック スプリングカタパ＠ゾーン２ データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkSpCtpltFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPCTPLT_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_spctplt_obj_3d_list, amb->file_num);
}
// ===========================================================================
