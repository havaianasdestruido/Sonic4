// =======================================================================
/*!
	@file	gmGmkStopper.c
	@brief	ギミック ストッパー＠ゾーン２ピンボール

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkStopper.cpp 2 2011-04-11 05:21:26Z thamada $
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
//camera test
#include "gmCamera.h"

#include "gmGmkSlot.h"
#include "gmGmkStopper.h"

// データヘッダ
#include "common/model/gmk_stopper_mdl.hmb"
#include "common/model/gmk_stopper_mat.hmb"



// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）

// ==========================================================================
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void gmGmkStopperReset(OBS_OBJECT_WORK *obj_work);
static void gmGmkStopperLockWait(OBS_OBJECT_WORK *obj_work);
static void gmGmkStopperExit(MTS_TASK_TCB *tcb);


// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_stopper_obj_3d_list = NULL;

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

	GME_GMK_TYPE_STOPPER_NORM = 0,	// ノーマルボーナス
	GME_GMK_TYPE_STOPPER_SLOT,		// スロットスターター

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
#define		GMD_GMK_STOPPER_V_MARGIN		( 4)
#define		GMD_GMK_STOPPER_V_HIT_MARGIN	( 4)
const static s16 tbl_gm_gmk_piston_col_rect[GME_GMK_RECT_DATA_MAX] = {

	//	ストッパー＠ゾーン２ピンボール　岩

		//	土地あたり矩形
		  32,		//	中心位置からの幅
		  32,		//	中心位置からの高さ
		 -16,		//	中心位置から地形あたりの発生する場所までのオフセット
		 -16,		//	中心位置から地形あたりの発生する場所までのオフセット

#if 0	// ■■■■ヒット処理(yamatani ver)■■■■
		//	拘束あたり矩形
		 -16,	    // 左
		 -16-GMD_GMK_STOPPER_V_MARGIN,	    // 上
		 +16,	    // 右
		 +16+GMD_GMK_STOPPER_V_MARGIN	    // 下
		//	拘束あたりの上側か下側に接触したときに、
		//	上側なら→Ｙ速度が＋(下移動)で、左右が完全範囲内ならＩＮ
		//	下側なら→Ｙ速度が－(上移動)で、左右が完全範囲内ならＩＮ
		//	それ以外は
		//	上側なら→Ｙ速度が＋(下移動)で、Ｙ速度反転
		//	下側なら→Ｙ速度が－(上移動)で、Ｙ速度反転
		//	します。
#else	// ■■■■ヒット処理(kuramoto ver)■■■■
		//	拘束あたり矩形
		 (-15   ),	    // 左
		 (-16 -1),	    // 上
		 (+15   ),	    // 右
		 (+16   )	    // 下
#endif	// ■■■■ヒット処理■■■■
};
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------





// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef struct tag_GMS_GMK_STOPPER_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	GME_GMK_TYPE		obj_type;		//!< 上向きか下向きかオブジェクトタイプ

	GMS_PLAYER_WORK		*ply_work;		//!< プレイヤーインで値有効
	s16					player_pass_timer;

	int					call_slot_id;

	GSS_SND_SE_HANDLE*	se_handle;		//!< SEハンドル

}GMS_GMK_STOPPER_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// ---------------------------------------------------------------------------



// ===========================================================================
// gmGmkStopper*
/*!
	ギミック ストッパー＠ゾーン２ピンボール

	@note
		ソニックの位置を判定し、入れない場合は土地当たりを有効に、
		スタート位置に入った場合はソニックをコントロールする必要があります。

		とりあえず存在と当たり判定と土地当たりを作成。
 */
// ---------------------------------------------------------------------------
static void gmGmkStopperStay(OBS_OBJECT_WORK *obj_work);
static void gmGmkStopperHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkStopperStart
/*!
	ギミック ストッパー　サブ初期化

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;
	OBS_RECT_WORK *rect_work;

	//	土地あたり矩形餅オブジェ設定
	//	普段では通り抜けられないように
	pwork->COMWORK.col_work.obj_col.obj       = obj_work;

	// 地形矩形設定
	pwork->COMWORK.col_work.obj_col.width  = (u16)(tbl_gm_gmk_piston_col_rect[GME_GMK_RECT_DATA_COL_WIDTH]);	//	中心位置からの幅
	pwork->COMWORK.col_work.obj_col.height = (u16)(tbl_gm_gmk_piston_col_rect[GME_GMK_RECT_DATA_COL_HEIGHT]);	//	中心位置からの高さ
	pwork->COMWORK.col_work.obj_col.ofst_x = tbl_gm_gmk_piston_col_rect[GME_GMK_RECT_DATA_COL_OFST_X];	//	中心位置から地形あたりの発生する場所までのオフセット
	pwork->COMWORK.col_work.obj_col.ofst_y = tbl_gm_gmk_piston_col_rect[GME_GMK_RECT_DATA_COL_OFST_Y];	//	中心位置から地形あたりの発生する場所までのオフセット
	pwork->COMWORK.col_work.obj_col.dir = (u16)(0x0000);

	// 矩形設定
	// 対プレイヤー
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
	rect_work->ppDef = gmGmkStopperHit;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 被破壊矩形設定
	ObjRectWorkSet(rect_work,
	               tbl_gm_gmk_piston_col_rect[GME_GMK_RECT_DATA_DEF_LEFT],
	               tbl_gm_gmk_piston_col_rect[GME_GMK_RECT_DATA_DEF_TOP],
	               tbl_gm_gmk_piston_col_rect[GME_GMK_RECT_DATA_DEF_RIGHT],
	               tbl_gm_gmk_piston_col_rect[GME_GMK_RECT_DATA_DEF_BOTTOM]);
	obj_work->flag &= ~OBD_OBJECT_NOHIT;								// 矩形あたりあり◆

	// SE_handle初期化
	pwork->se_handle = NULL;
	// SE handle 開放のために、destructorは専用処理に置き換える
	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkStopperExit);

	obj_work->chgf(gmGmkStopperStay);
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkStopperStay
/*!
	ギミック ストッパー

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperStay_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkStopperStay_Norm(OBS_OBJECT_WORK *obj_work);
static void gmGmkStopperStay_Slot(OBS_OBJECT_WORK *obj_work);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;

	if( pwork->ply_work != NULL )
	{
		if( pwork->call_slot_id == -1 )
		{
			gmGmkStopperStay_Norm(obj_work);
		}
		else
		{
			gmGmkStopperStay_Slot(obj_work);
		}
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperStay_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;

	if( obj_work->user_timer < (pwork->ply_work->obj_work.pos.y>>FX32_SHIFT) )
	{
		//	プレイヤーが落下して範囲を離れた
		gmGmkStopperReset(obj_work);
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperStay_Norm_100(OBS_OBJECT_WORK *obj_work);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperStay_Norm(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;
	//	通過
	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_STOPPER_MAT_GMK_STOPPER_IN_ZNV );
	obj_work->disp_flag &= ~OBD_DISP_STOP;
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	pwork->player_pass_timer = 8*16+15;	//	ボーナスタイム
	obj_work->chgf(gmGmkStopperStay_Norm_100);

	//cameraオフセット量限界値をセット(Yのみ/Xは基準値のまま)
	GmCameraAllowSet(15.f, 32.f, 0.f);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperStay_Norm_110(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkStopperStay_Norm_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;

	// プレイヤー捕捉状況を確認
	if (  (pwork->ply_work != g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P])
		||(pwork->ply_work->gmk_obj != obj_work)
		||(pwork->ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_STOPPER) ) {
		// プレイヤー捕捉状態に異変あり
		gmGmkStopperReset(obj_work);
		return;
	}

	//camera制御(スクロール上限はGmCameraAllowSetで抑える)
	OBS_CAMERA	*obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	GmCameraPosSet( FXM_FLOAT_TO_FX32(obj_camera->pos.x),
					FXM_FLOAT_TO_FX32(-obj_camera->pos.y) + FX32_ONE * 4,
					FXM_FLOAT_TO_FX32(obj_camera->pos.z));

	pwork->player_pass_timer -= 1;
	if( pwork->player_pass_timer <= 0 )
	{
		obj_work->disp_flag &= ~OBD_DISP_REPEAT;
//		obj_work->chgf(gmGmkStopperStay_Norm_110);
		gmGmkStopperStay_Norm_110(obj_work);
	}
	else if( !(pwork->player_pass_timer%16) )
	{
		//	スコア足す　１０００点
		MTM_ASSERT(pwork->ply_work);
		GmPlayerAddScore( pwork->ply_work, 1000, pwork->ply_work->obj_work.pos.x, pwork->ply_work->obj_work.pos.y );

		// 得点加算 SE
		GmSoundPlaySE("Casino3", pwork->se_handle);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkStopperStay_Norm_110(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;
	GMS_PLAYER_WORK *ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

//	if (obj_work->disp_flag & OBD_DISP_MAT_END)
	{
		pwork->player_pass_timer = 0;
		pwork->OBJ_3D.mat_frame = 0;											// アニメーションを初期に戻す
		obj_work->disp_flag |= OBD_DISP_STOP;
		obj_work->chgf(gmGmkStopperStay_100);
		MTM_ASSERT(pwork->ply_work);

		if (ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_STOPPER) {
			// ソニックを落下させる
			GmPlySeqInitStopperEnd(pwork->ply_work);
		}
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperStay_Slot_100(OBS_OBJECT_WORK *obj_work);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperStay_Slot(OBS_OBJECT_WORK *obj_work)
{
	//	通過
	ObjDrawObjectActionSet3DNNMaterial( obj_work, 0/*IDB_GMK_STOPPER_MAT_GMK_STOPPER_2_IN_ZNV*/ );
	obj_work->disp_flag &= ~OBD_DISP_STOP;
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	obj_work->chgf(gmGmkStopperStay_Slot_100);

	//cameraオフセット量限界値をセット(Yのみ/Xは基準値のまま)
	GmCameraAllowSet(15.f, 56.f, 0.f);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperStay_Slot_110(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkStopperStay_Slot_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;

	// プレイヤー捕捉状況を確認
	if (  (pwork->ply_work != g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P])
		||(pwork->ply_work->gmk_obj != obj_work)
		||(pwork->ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_STOPPER) ) {
		// プレイヤー捕捉状態に異変あり
		gmGmkStopperReset(obj_work);
		return;
	}

	//camera制御(スクロール上限はGmCameraAllowSetで抑える)
	OBS_CAMERA	*obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	GmCameraPosSet( FXM_FLOAT_TO_FX32(obj_camera->pos.x),
					FXM_FLOAT_TO_FX32(-obj_camera->pos.y) + FX32_ONE * 4,
					FXM_FLOAT_TO_FX32(obj_camera->pos.z));
	
	if( GmGmkSlotIsStatus(pwork->call_slot_id) )
	{
		obj_work->disp_flag &= ~OBD_DISP_REPEAT;
		obj_work->chgf(gmGmkStopperStay_Slot_110);
		gmGmkStopperStay_Slot_110(obj_work);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkStopperStay_Slot_110(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;
	GMS_PLAYER_WORK *ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	pwork->OBJ_3D.mat_frame = 0;											// アニメーションを初期に戻す
	obj_work->disp_flag |= OBD_DISP_STOP;
	obj_work->chgf(gmGmkStopperStay_100);
	MTM_ASSERT(pwork->ply_work);

	if (ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_STOPPER) {
		// ソニックを落下させる
		GmPlySeqInitStopperEnd(pwork->ply_work);
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkStopperReset
/*!
	ギミック ストッパー 自身を待機状態に状態変更

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void gmGmkStopperReset(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;

	pwork->COMWORK.col_work.obj_col.obj = obj_work;	// 土地当たり復活
	obj_work->flag &= ~OBD_OBJECT_NOHIT;			// 矩形あたり復活

	pwork->player_pass_timer = 0;
	pwork->ply_work = NULL;

	pwork->OBJ_3D.mat_frame = 0;											// アニメーションを初期に戻す
	obj_work->disp_flag |= OBD_DISP_STOP;

	obj_work->chgf(gmGmkStopperStay);

	// SE Handle開放
	if (pwork->se_handle) {
		GmSoundStopSE(pwork->se_handle); 
		GsSoundFreeSeHandle(pwork->se_handle);
		pwork->se_handle = NULL;
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkStopperLockWait
/*!
	ギミック ストッパー プレイヤー捕捉後定位置までの待機

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void gmGmkStopperLockWait(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;
	GMS_PLAYER_WORK	*ply_work;

	// プレイヤー捕捉状況を確認
	if (  (pwork->ply_work != g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P])
		||(pwork->ply_work->gmk_obj != obj_work)
		||(pwork->ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_STOPPER) ) {
		// プレイヤー捕捉状態に異変あり
		gmGmkStopperReset(obj_work);
		return;
	}

	ply_work = (GMS_PLAYER_WORK*)pwork->ply_work;
	if (  (obj_work->pos.x == ply_work->obj_work.pos.x)
		&&(obj_work->pos.y == ply_work->obj_work.pos.y) ) {
		// ストッパーとソニックの座標が一致したら得点処理へ移行
		if( pwork->call_slot_id >= 0 ) {
			// スロット用ストッパー
			if( GmGmkSlotStartRequest(pwork->call_slot_id,ply_work) == FALSE ) {// ■スロット スタート！
				MTM_ASSERT(0);	// スロット状態に異常あり
			}
			obj_work->chgf(gmGmkStopperStay_Slot);
			// ストッパーIN SE
			GmSoundPlaySE("Casino3");	// SE
		} else {
			// 単体ストッパー
			obj_work->chgf(gmGmkStopperStay_Norm);
			// ストッパーIN SE
			if (pwork->se_handle) {
				// もしもhandle保持しっ放しならば事前に開放
				GmSoundStopSE(pwork->se_handle); 
				GsSoundFreeSeHandle(pwork->se_handle);
			}
			pwork->se_handle = GsSoundAllocSeHandle(); 
			pwork->se_handle->flag |= GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR; // クリアしないフラグ 

			GmSoundPlaySE("Casino3", pwork->se_handle);
		}
	}
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// gmGmkStopperHit
/*!
	ギミック ストッパー プレイヤー接触

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkStopperHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
#if 0	// ■■■■ヒット処理(yamatani ver)■■■■
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	//	一応プレイヤーが当たったかどうかを確認
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] )
	{
		//	上下チェック
		s16 tb = 0;
		if (obj_work->pos.y > ply_work->obj_work.pos.y && ply_work->obj_work.spd.y >= 0)
		{
			//	上側で落下中
			//	上側チェック　プレイヤーを上(<)に見た場合の値
			tb = (s16)((obj_work->pos.y>>FX32_SHIFT)+mine_rect->rect.top
			           - match_rect->rect.bottom);
			tb  = (s16)((ply_work->obj_work.pos.y>>FX32_SHIFT) - tb);
			tb += (s16)((ply_work->obj_work.move.y>>FX32_SHIFT));
		}
		else if (obj_work->pos.y < ply_work->obj_work.pos.y && ply_work->obj_work.spd.y < 0)
		{
			//	下側チェック　プレイヤーを下(>)に見た場合の値
			tb = (s16)((obj_work->pos.y>>FX32_SHIFT)+mine_rect->rect.bottom
			           - match_rect->rect.top);
			tb  = (s16)((ply_work->obj_work.pos.y>>FX32_SHIFT) - tb);
			tb += (s16)((ply_work->obj_work.move.y>>FX32_SHIFT));
		}
		if( tb >= (GMD_GMK_STOPPER_V_MARGIN-GMD_GMK_STOPPER_V_HIT_MARGIN)||
			    tb <= -(GMD_GMK_STOPPER_V_MARGIN-GMD_GMK_STOPPER_V_HIT_MARGIN) )
		{
			//	左右チェック
			s16	l,r;	// Left,Right
			s16 px;
			l = (s16)((obj_work->pos.x>>FX32_SHIFT) + mine_rect->rect.left);		//	ストッパーの左側を取得
			r = (s16)((obj_work->pos.x>>FX32_SHIFT) + mine_rect->rect.right);	//	ストッパーの右側を取得
			px = (s16)(ply_work->obj_work.pos.x>>FX32_SHIFT);
			//	左右完全範囲チェック
			//	矩形再チェック
			if( px > (l - match_rect->rect.left) && px < (r - match_rect->rect.right))
			{
				ply_work->obj_work.pos.x = obj_work->pos.x;
				ply_work->obj_work.spd.x = 0;

				GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;
				pwork->COMWORK.col_work.obj_col.obj = NULL;
				//	上下完全範囲チェック
				s16	t,b;	// Top,Bottom
				s16 py;
				t = (s16)((obj_work->pos.y>>FX32_SHIFT) + mine_rect->rect.top);		//	ストッパーの上側を取得
				b = (s16)((obj_work->pos.y>>FX32_SHIFT) + mine_rect->rect.bottom);	//	ストッパーの上側を取得
				py = (s16)(ply_work->obj_work.pos.y>>FX32_SHIFT);
				if( py > (t - match_rect->rect.top) && py < (b - match_rect->rect.bottom) )
				{
					ply_work->obj_work.pos.y = obj_work->pos.y;
					if( pwork->call_slot_id >= 0 )
					{
						if( GmGmkSlotStartRequest(pwork->call_slot_id,ply_work) == FALSE )	// ■スロット スタート！
						{
							//	進行停止の保険。ここにくること自体おかしい。
							ObjRectFuncNoHit(mine_rect, match_rect);	// ヒットしなかった事にする
							return;
						}
					}
					obj_work->flag |= OBD_OBJECT_NOHIT;		// 矩形あたり抜く
					GmPlySeqInitStopper( ply_work, &pwork->COMWORK );						// ■ストッパー ロック
					pwork->ply_work = ply_work;
					obj_work->user_timer = (s32)(b-match_rect->rect.top);	//	開放ライン
				}
			}
			else if( px > (l - match_rect->rect.right) && 
				     px < (r - match_rect->rect.left) )
			{
				if( tb > 0 && ply_work->obj_work.spd.y > 0 )
				{
					ply_work->obj_work.spd.y = -4*FX32_ONE;					//	跳ね返す
					if( ply_work->obj_work.disp_flag & OBD_DISP_HFLIP )
					{
						if( ply_work->obj_work.spd.x >= -3*FX32_ONE )
							ply_work->obj_work.spd.x = -3*FX32_ONE;					//	跳ね返す
					}
					else
					{
						if( ply_work->obj_work.spd.x <= 3*FX32_ONE )
							ply_work->obj_work.spd.x =  3*FX32_ONE;					//	跳ね返す
					}
				}
//					ply_work->obj_work.spd.y = -(ply_work->obj_work.spd.y);	//	跳ね返す
				//	落下時は土地あたりに頭を打つから放置
			}
		}
	}
	// ヒットしなかった事にする
	ObjRectFuncNoHit(mine_rect, match_rect);
#else	// ■■■■ヒット処理(kuramoto ver)■■■■
	OBS_OBJECT_WORK *obj_work = mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;

	BOOL slot_lock = FALSE;
	
	//	プレイヤー以外は無視
	if( ply_work == g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P] )
	{
		if (obj_work->pos.y > ply_work->obj_work.pos.y) {
			// プレイヤーが上
			if (ply_work->obj_work.spd.y >= 0) {
				// 落下中なら許可
				slot_lock = TRUE;
			}
		} else {
			// プレイヤーが下
			if (ply_work->obj_work.spd.y <= 0) {
				// 上昇中なら許可
				slot_lock = TRUE;
			}
		}
		if (slot_lock) {
			// スロットロックを許可
			GmPlySeqInitStopper( ply_work, &pwork->COMWORK );						// ■ストッパー ロック

			pwork->COMWORK.col_work.obj_col.obj = NULL;	// 地形コリジョンOFF
			obj_work->flag |= OBD_OBJECT_NOHIT;			// 矩形あたりOFF
			pwork->ply_work = ply_work;
			obj_work->user_timer = (s32)((obj_work->pos.y >> FX32_SHIFT)
										+ pwork->COMWORK.col_work.obj_col.height
										+ pwork->COMWORK.col_work.obj_col.ofst_y
										- ply_work->rect_work[GMD_PLAYER_RECT_DEF].rect.top);
			obj_work->chgf(gmGmkStopperLockWait);
		} else {
			// 当たらなかった事にする
			mine_rect->flag &= ~(OBD_RECT_DAMAGE | OBD_RECT_FRAMEHIT | OBD_RECT_FRAMEOUT);
		}
	}
#endif	// ■■■■ヒット処理■■■■

}
// ---------------------------------------------------------------------------
// ==========================================================================
// gmGmkStopperExit
/*!
 *	ギミック ストッパー 終了処理
 *
 *	@param tcb		[in] tcbワーク
 */
// ==========================================================================
void gmGmkStopperExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK*	obj_work  = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_GMK_STOPPER_WORK *pwork = (GMS_GMK_STOPPER_WORK*)obj_work;
	
	if (pwork->se_handle) {
		GsSoundStopSeHandle(pwork->se_handle);
		GsSoundFreeSeHandle(pwork->se_handle);
		pwork->se_handle = NULL;
	}
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}
// ---------------------------------------------------------------------------


#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================





// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkStopper?Init
/*!
 *	ギミック ストッパー＠ゾーン２ピンボール 初期化関数
 *	GmGmkStopperUInit 上向き
 *	GmGmkStopperDInit 下向き
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkStopperNormInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_STOPPER_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_STOPPER_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_STOPPER_WORK), "Gmk_StopperRod");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_stopper_obj_3d_list[IDB_GMK_STOPPER_MDL_GMK_STOPPER_ZNO],
	                             &gmk_work->obj_3d);
	// 優先設定
//	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE;
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_BACK;

	//モーション
	ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
	                                 0,				//reg_file_id
	                                 NULL,			//data_work
	                                 NULL,			//mtn_data_path
	                                 IDB_GMK_STOPPER_MAT_GMK_STOPPER_IN_ZNV,
	                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_STOPPER_MAT)->pData );
	gmk_work->obj_3d.mat_speed = 1.0f;
	obj_work->disp_flag |= OBD_DISP_STOP | OBD_DISP_REPEAT;
	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	pwork->call_slot_id = -1;	//	非スロットスターター
	gmGmkStopperStart(obj_work);

	return obj_work;
}
// ==========================================================================
OBS_OBJECT_WORK* GmGmkStopperSlotInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_STOPPER_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_STOPPER_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_STOPPER_WORK), "Gmk_StopperRod");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_stopper_obj_3d_list[IDB_GMK_STOPPER_MDL_GMK_STOPPER_2_ZNO],
	                             &gmk_work->obj_3d);
	// 優先設定
//	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - FX32_ONE;
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_BACK;

	//モーション
	ObjAction3dNNMaterialMotionLoad( &gmk_work->obj_3d,
	                                 0,				//reg_file_id
	                                 NULL,			//data_work
	                                 NULL,			//mtn_data_path
	                                 IDB_GMK_STOPPER_MAT_GMK_STOPPER_2_IN_ZNV,
	                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_STOPPER_MAT)->pData );
	gmk_work->obj_3d.mat_speed = 1.0f;
	obj_work->disp_flag |= OBD_DISP_STOP | OBD_DISP_REPEAT;
	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	pwork->call_slot_id = eve_rec->left;	//	スロットスターター
	gmGmkStopperStart(obj_work);

	return obj_work;
}
// ==========================================================================



// ===========================================================================
// GmGmkStopperBuild
/*!
	ギミック ストッパー＠ゾーン２ピンボール データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkStopperBuild(void)
{
	gm_gmk_stopper_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STOPPER_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STOPPER_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkStopperFlush
/*!
	ギミック ストッパー＠ゾーン２ピンボール データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkStopperFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STOPPER_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_stopper_obj_3d_list, amb->file_num);
}
// ===========================================================================
