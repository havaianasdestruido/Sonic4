// ===========================================================================
/*!
	@file	gmGmkPointMarker.cpp
	@brief	ギミック ポイントマーカー

	@author	ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkPointMarker.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmEffectCmn.h"

#include "gmSound.h"

#include "gmGmkPointMarker.h"

// データヘッダ
#include "common/model/gmk_p_marker_mdl.hmb"
#include "common/model/gmk_p_marker_mtn.hmb"
#include "common/model/gmk_p_marker_mat.hmb"

#include "mppCheckPointStorage.h"



// ----- Struct Definitions --------------------------------------（型の宣言）
//! ポイントマーカー

typedef struct tag_GMS_GMK_PMARKER_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。

	fx32 markerdist;
	fx32 markerdistlast;
	int hitcounter;

	u16	marker_prty;

}GMS_GMK_PMARKER_WORK;
#define		OBJ_3D		gmk_work.obj_3d
#define		COMWORK		gmk_work.ene_com
#define		OBJWORK		COMWORK.obj_work
#define		chgFunc(func)	ppFunc = func

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）


// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_pmarker_obj_3d_list = NULL;


// ----- Macros ------------------------------------------------（マクロ定義）


// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）
// ---------------------------------------------------------------------------
// 地形矩形テーブル
typedef enum tag_GME_GMK_COL_DATA{
	GME_GMK_COL_DATA_WIDTH	= 0,	// 幅
	GME_GMK_COL_DATA_HEIGHT,		// 高さ
	GME_GMK_COL_DATA_OFST_X,		// 中心位置からＸ方向のオフセット
	GME_GMK_COL_DATA_OFST_Y,		// 中心位置からＹ方向のオフセット

	GME_GMK_COL_DATA_MAX

} GME_GMK_COL_DATA;
// ---------------------------------------------------------------------------
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	当たり矩形大きさ定義
#define		GMD_GMK_PMARKER_RECT_LEFT		(-16)
#define		GMD_GMK_PMARKER_RECT_RIGHT		(+16)
#define		GMD_GMK_PMARKER_RECT_TOP		(-64)
#define		GMD_GMK_PMARKER_RECT_BOTTOM		(  0)
#define		GMD_GMK_PMARKER_EFFECTIVE_RANGE	(  4)

//	土地当たり矩形大きさ定義
// ---------------------------------------------------------------------------



// ==========================================================================
// ==========================================================================
// ==========================================================================
// gmGmkPointMarker*
/*!
 *	ギミック ポイントマーカー
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ---------------------------------------------------------------------------
static void gmGmkPointMarkerStart(OBS_OBJECT_WORK *obj_work);
static void gmGmkPointMarkerStay(OBS_OBJECT_WORK *obj_work);
static void gmGmkPointMarkerHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
//	gmGmkPointMarkerHit
/*!
 *	ポイントマーカーあたり判定
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPointMarkerHit(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_GMK_PMARKER_WORK *pwork = (GMS_GMK_PMARKER_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	//	通過したかどうかを判定してみる
	#define	inrange	(GMD_GMK_PMARKER_EFFECTIVE_RANGE*FX32_ONE)
	//	ソニックの進行方向と触れた位置等で確認
	pwork->markerdist = pwork->OBJWORK.pos.x - ply_work->obj_work.pos.x;
	if (    (pwork->markerdist <= +inrange && pwork->markerdist >= -inrange)||	//	接触有効
	        (pwork->markerdist <  +inrange && pwork->markerdistlast >= +inrange) || // 高速通過
	        (pwork->markerdist >  -inrange && pwork->markerdistlast <= -inrange))   // 高速通過
	{
		//	再確認
		if( g_gm_main_system.marker_pri < pwork->marker_prty )
		{
			GmPlayerSetMarkerPoint( ply_work, pwork->OBJWORK.pos.x, pwork->OBJWORK.pos.y); 
			g_gm_main_system.marker_pri = pwork->marker_prty;

			pwork->marker_prty = 0;
		    pwork->hitcounter = 2;
			
			mppCheckPointStorage::saveState(mppCheckPointStorage::SAVE_AFTER_CHECKPOINT);
			
			
		}
		pwork->OBJWORK.flag |= OBD_OBJECT_NOHIT;							// 矩形あたり無し◆
		return;
	}
	// ヒットしなかった事にする
	mine_rect->flag &= ~(OBD_RECT_DAMAGE | OBD_RECT_FRAMEHIT | OBD_RECT_FRAMEOUT);
}
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
//	gmGmkPointMarkerStay
/*!
 *	ポイントマーカー待機
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPointMarkerStay_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkPointMarkerStay_200(OBS_OBJECT_WORK *obj_work);
static void gmGmkPointMarkerStay_300(OBS_OBJECT_WORK *obj_work);
static void gmGmkPointMarkerStay_400(OBS_OBJECT_WORK *obj_work);
//static void gmGmkPointMarkerStay_DB_000(OBS_OBJECT_WORK *obj_work);
#define	gmGmkPointMarkerStay_DB_000		(NULL)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPointMarkerStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PMARKER_WORK *pwork = (GMS_GMK_PMARKER_WORK*)obj_work;

	ObjDrawObjectActionSet(obj_work, IDB_GMK_P_MARKER_MTN_GMK_P_MARKER_ZNM);
	pwork->markerdist = 0;	//	当たり判定を確認
    pwork->hitcounter = 0;
	obj_work->chgFunc(gmGmkPointMarkerStay_100);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPointMarkerStay_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PMARKER_WORK *pwork = (GMS_GMK_PMARKER_WORK*)obj_work;

	pwork->markerdistlast = pwork->markerdist;
	pwork->markerdist = 0;	//	当たり判定を確認
	if( pwork->hitcounter > 0 )
	{
		// SE
		GmSoundPlaySE("Marker");
		gmGmkPointMarkerStay_200(obj_work);
	}
	else if( g_gm_main_system.marker_pri >= pwork->marker_prty )
	{
		pwork->marker_prty = 0;
		gmGmkPointMarkerStay_400(obj_work);
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPointMarkerStay_210(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPointMarkerStay_200(OBS_OBJECT_WORK *obj_work)
{
	ObjDrawObjectActionSet(obj_work, IDB_GMK_P_MARKER_MTN_GMK_P_MARKER_TURN_ZNM);
	obj_work->chgFunc(gmGmkPointMarkerStay_210);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPointMarkerStay_210(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PMARKER_WORK *pwork = (GMS_GMK_PMARKER_WORK*)obj_work;

	if (obj_work->disp_flag & OBD_DISP_END) {
		pwork->hitcounter -= 1;
		if (pwork->hitcounter == 0) {
			ObjDrawObjectActionSet(obj_work, IDB_GMK_P_MARKER_MTN_GMK_P_MARKER_ZNM);
			obj_work->chgFunc(gmGmkPointMarkerStay_300);
		} else {
			gmGmkPointMarkerStay_200(obj_work);
		}
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPointMarkerStay_300(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END)
	{
		gmGmkPointMarkerStay_400(obj_work);
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPointMarkerStay_400(OBS_OBJECT_WORK *obj_work)
{
	GMS_EFFECT_3DES_WORK *efct_3des = GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_POINT);
	GmEffect3DESAddDispOffset(efct_3des, 0.0f, 34.0f, 0.0f);
	// 優先設定
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
		efct_3des->efct_com.obj_work.pos.z = obj_work->pos.z+10*FX32_ONE;// トロッコライト対応
	}
	else {
		efct_3des->efct_com.obj_work.pos.z = obj_work->pos.z+16*FX32_ONE;
	}

	ObjAction3dNNMaterialMotionLoad( obj_work->obj_3d,
	                                 0,				//reg_file_id
	                                 NULL,			//data_work
	                                 NULL,			//mtn_data_path
	                                 IDB_GMK_P_MARKER_MAT_GMK_P_MARKER_TURN_ZNV,
	                                 (void*)ObjDataGet(GMD_DWORK_NO_GMK_P_MARKER_MAT)->pData );
	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_GMK_P_MARKER_MAT_GMK_P_MARKER_TURN_ZNV );

	obj_work->obj_3d->mat_speed = 1.0f;
	obj_work->disp_flag |= OBD_DISP_REPEAT;
	obj_work->chgFunc(NULL);	//	処理なし
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifndef gmGmkPointMarkerStay_DB_000
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPointMarkerStay_DB_010(OBS_OBJECT_WORK *obj_work);
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPointMarkerStay_DB_000(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PMARKER_WORK *pwork = (GMS_GMK_PMARKER_WORK*)obj_work;
	pwork->hitcounter+= 1;
	if( pwork->hitcounter > 60 )
	{
		ObjDrawObjectActionSet(obj_work, IDB_GMK_P_MARKER_MTN_GMK_P_MARKER_TURN_ZNM);
		obj_work->chgFunc(gmGmkPointMarkerStay_DB_010);
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkPointMarkerStay_DB_010(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PMARKER_WORK *pwork = (GMS_GMK_PMARKER_WORK*)obj_work;
	if (obj_work->disp_flag & OBD_DISP_END) {
		pwork->hitcounter = 0;
		ObjDrawObjectActionSet(obj_work, IDB_GMK_P_MARKER_MTN_GMK_P_MARKER_ZNM);
		gmGmkPointMarkerStay(obj_work);
		pwork->OBJWORK.flag &= ~OBD_OBJECT_NOHIT;	// 矩形あたり復活
	}
}
#endif
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//	gmGmkPointMarkerStart
/*!
 *	ポイントマーカー初期設定
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkPointMarkerStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_PMARKER_WORK *pwork = (GMS_GMK_PMARKER_WORK*)obj_work;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_pmarker_obj_3d_list[IDB_GMK_P_MARKER_MDL_GMK_P_MARKER_ZNO],
	                             &pwork->OBJ_3D);
	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work,
	                              0/*reg_file_id*/,
	                              FALSE/*marge*/,
	                              ObjDataGet(GMD_DWORK_NO_GMK_P_MARKER_MTN),
	                              NULL/*mtn_data_path*/,
	                              0/*index*/,
	                              NULL/*archive*/);
	ObjDrawObjectActionSet(obj_work, IDB_GMK_P_MARKER_MTN_GMK_P_MARKER_ZNM);

	if( g_gm_main_system.marker_pri < pwork->marker_prty )
	{
		// 矩形設定
		OBS_RECT_WORK *rect_work;
		// 対プレイヤー
		pwork->COMWORK.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
		pwork->COMWORK.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
		rect_work = &pwork->COMWORK.rect_work[GMD_ENEMY_RECT_BODY];
		rect_work->ppDef = gmGmkPointMarkerHit;
		rect_work->ppHit = NULL;
		ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
		ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		// 被破壊矩形設定
		ObjRectWorkSet(rect_work,
		               		GMD_GMK_PMARKER_RECT_LEFT,
		               		GMD_GMK_PMARKER_RECT_TOP,
		               		GMD_GMK_PMARKER_RECT_RIGHT,
		               		GMD_GMK_PMARKER_RECT_BOTTOM);
	}
	else
	{
		pwork->marker_prty = 0;
	}

	gmGmkPointMarkerStay(obj_work);
}
// ==========================================================================




// ==========================================================================
// GmGmkPointMarkerInit
/*!
 *	ギミック ポイントマーカー 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkPointMarkerInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_PMARKER_WORK *pwork;
	OBS_OBJECT_WORK *obj_work;

	UNREFERENCED_PARAMETER(type);

	if (g_gs_main_sys_info.game_mode == GSD_GAME_MODE_TIME_ATTACK) {
		// タイムアタックの時は生成しない
		eve_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;	// 次からスキップ
		return (NULL);
	}

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_PMARKER_WORK), "GMK_POINT_MARKER");
	pwork = (GMS_GMK_PMARKER_WORK*)obj_work;

	obj_work->pos.y += (1*FX32_ONE);		//	微調整
	// 優先設定
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;					// トロッコライト対応
	}
	else {
		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - (16*FX32_ONE);		//	奥に移動させる
	}

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE|OBD_MOVE_NOCOL;			// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	//	ポイントマーカー優先
	pwork->marker_prty = eve_rec->left;

	gmGmkPointMarkerStart(obj_work);

	return obj_work;
}
// ===========================================================================


// ===========================================================================
// GmGmkPointMarkerBuild
/*!
	ギミック ポイントマーカー データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkPointMarkerBuild(void)
{
	gm_gmk_pmarker_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_P_MARKER_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_P_MARKER_TEX),
								0/*draw_flag*/);
}
// ===========================================================================


// ===========================================================================
// GmGmkPointMarkerFlush
/*!
	ギミック ポイントマーカー データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkPointMarkerFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_P_MARKER_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_pmarker_obj_3d_list, amb->file_num);
}
// ===========================================================================
