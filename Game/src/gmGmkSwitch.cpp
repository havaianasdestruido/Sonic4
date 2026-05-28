// ==========================================================================
/*!
  @file gmGmkSwitch.cpp
  @brief ギミック スイッチ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGmkSwitch.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmComEfct.h"
#include "gmSound.h"
#include "gmPadVib.h"

#include "gmGmkSwitch.h"

// データヘッダ
#include "common/model/GMK_SWITCH3_MDL.HMB"
#include "common/model/GMK_SWITCH4_MDL.HMB"
#include "common/model/GMK_SWITCH3_MAT.HMB"

//----- Definitions ---------------------------------------------------------
/* eve_rec->left */
// 管理ID
/* eve_rec->top */
// フレーム
/* eve_rec->width */
// 秒
/* eve_rec->height */
/* eve_rec->flag */
//#define GMD_GMK_SW_		(0x0000)

/// スイッチワーク
typedef struct tag_GMS_GMK_SW_WORK {
	GMS_ENEMY_3D_WORK		gmk_work;
	OBS_ACTION3D_NN_WORK	obj_3d_base;	//スイッチベース分描画用

	s32						top_pos_y;		// スイッチ上部位置

	u32						id;				// スイッチID
	fx32					time;			// 再度OFFになるまでの時間



} GMS_GMK_SW_WORK;

// enemy_flag
#define GMD_GMK_SWITCH_ENEMY_FLAG_RIDER	(0x00000001)	//!< 乗っている人がいるフラグ(矩形使用時)

/// スイッチ状態ワーク
typedef struct tag_GMS_GMK_SW_STATE_WORK {
	BOOL	sw;				//!< スイッチ状態
	fx32	time;			//!< OFFになるまでの時間 初期状態0で無効
	BOOL	gear;			//!< スイッチギアタイプ
	fx32	per;			//!< 歯車スイッチ 展開率 (OFF)FX32_ONE ～ 0(ON)
//	fx32	spd;			//!< 歯車スイッチ 速度
//	fx32	spd_frame;		//!< 歯車スイッチ 経過時間
} GMS_GMK_SW_STATE_WORK;

// 地形矩形設定
#define GMD_GMK_SW_COL_RECT_WIDTH	(8*4)
#define GMD_GMK_SW_COL_RECT_HEIGHT	(8*3)
#define GMD_GMK_SW_COL_RECT_OFST_X	(-GMD_GMK_SW_COL_RECT_WIDTH/2)
//#define GMD_GMK_SW_COL_RECT_OFST_Y	(-GMD_GMK_SW_COL_RECT_HEIGHT + 2)

// BODY矩形
#define GMD_GMK_SW_BODY_RECT_LEFT	(-16)
#define GMD_GMK_SW_BODY_RECT_TOP	(-16-4)
#define GMD_GMK_SW_BODY_RECT_RIGHT	(16)
#define GMD_GMK_SW_BODY_RECT_BOTTOM	(-0-4)

// スイッチ上部オフセット
#define GMD_GMK_SW_TOP_OFF_POS_OFST_Y	(-16+2)		//!< OFFの時のスイッチオフセット
#define GMD_GMK_SW_TOP_ON_POS_OFST_Y	(-10)		//!< ONの時のスイッチオフセット

#define GMD_GMK_SW_TOP_ON_SPD			(2)			//!< スイッチが押し込まれる速度
#define GMD_GMK_SW_TOP_OFF_SPD			(-2)		//!< スイッチがあがる速度

// スイッチOFF待機時間補正値
#define GMD_GMK_SW_TIME_MIN				(3 * FX32_ONE)	//!< 待機時間がこの値より小さい場合に補正

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSwOffInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwOffMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwOnInit(OBS_OBJECT_WORK *obj_work, BOOL now_on);
static void gmGmkSwOnMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwitchDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static void gmGmkSwDispFunc(OBS_OBJECT_WORK *obj_work);
static void gmGmkSwSetCol(OBS_COLLISION_WORK *col_work, s32 top_pos_y);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_switch_obj_3d_list = NULL;

/// スイッチ状態ワーク
static GMS_GMK_SW_STATE_WORK gm_gmk_switch_state[GMD_GMK_SW_MAX] = {{0}};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSwitchBuildType3
/*!
 *	ギミック スイッチ データ構築 ZONE3タイプ
 */
// ==========================================================================
void GmGmkSwitchBuildTypeZone3(void)
{
	gm_gmk_switch_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SWITCH_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SWITCH_TEX),
								0/*draw_flag*/);
	// スイッチステータスクリア
	MI_CpuClear8(gm_gmk_switch_state, sizeof(gm_gmk_switch_state));
}

// ==========================================================================
// GmGmkSwitchBuildType4
/*!
 *	ギミック スイッチ データ構築 ZONE4タイプ
 */
// ==========================================================================
void GmGmkSwitchBuildTypeZone4(void)
{
#if 1
	gm_gmk_switch_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SWITCH_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SWITCH_TEX),
								0/*draw_flag*/);
#else
	AMS_AMB_HEADER	*amb;
	void			*txb;

	// TXB取得
	amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SWITCH_TEX);
	amBindConv((u8*)amb);		// 中のデータはコンバートしない

	txb = amBindGet(amb, amb->file_num - 1/*一番最後がTXB*/);

	gm_gmk_switch_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SWITCH_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SWITCH_TEX),
								0/*draw_flag*/,
								txb);
#endif
	// スイッチステータスクリア
	MI_CpuClear8(gm_gmk_switch_state, sizeof(gm_gmk_switch_state));
}

// ==========================================================================
// GmGmkSwitchReBuild
/*!
 *	ギミック スイッチ 再構築
 */
// ==========================================================================
void GmGmkSwitchReBuild(void)
{
	// スイッチステータスクリア
	MI_CpuClear8(gm_gmk_switch_state, sizeof(gm_gmk_switch_state));
}

// ==========================================================================
// GmGmkSwitchFlush
/*!
 *	ギミック スイッチ データ片付け
 */
// ==========================================================================
void GmGmkSwitchFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SWITCH_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_switch_obj_3d_list, amb->file_num);
}


// ==========================================================================
// GmGmkSwitchInit
/*!
 *	ギミック スイッチ 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *			
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSwitchInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_SW_WORK		*switch_work;
	OBS_RECT_WORK		*rect_work;
	OBS_COLLISION_WORK	*col_work;

	UNREFERENCED_PARAMETER(type);

	// オブジェクト生成
	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_SW_WORK), "GMK_SWITCH");

	gmk_work	= (GMS_ENEMY_3D_WORK*)obj_work;
	switch_work	= (GMS_GMK_SW_WORK*)obj_work;

	// モデル初期化
	// トップ
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_switch_obj_3d_list[IDB_GMK_SWITCH3_MDL_GMK_SWITCH_TOP_ZNO],
					&gmk_work->obj_3d);

	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_3) {
		// マテリアルモーション
		ObjAction3dNNMaterialMotionLoad(&gmk_work->obj_3d, 0/*reg_file_id*/,
									ObjDataGet(GMD_DWORK_NO_GMK_SWITCH_MAT), NULL/*filename*/,
									0/*index*/, NULL/*archive*/,
									1/*motion_num*/, 1/*mmotion_num*/);
		ObjDrawAction3dActionSet3DNNMaterial(&gmk_work->obj_3d, IDB_GMK_SWITCH3_MAT_GMK_SWITCH_TOP_ZNV);
		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}

	// ベース(モーションはないので開放不要)
	ObjCopyAction3dNNModel(&gm_gmk_switch_obj_3d_list[IDB_GMK_SWITCH3_MDL_GMK_SWITCH_ZNO], &switch_work->obj_3d_base);

	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK - 32*FX32_ONE;

	// 描画処理設定
	obj_work->ppOut = gmGmkSwDispFunc;


	/* 矩形設定 */
#if 0
	// BODY
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work,
						GMD_GMK_SW_BODY_RECT_LEFT, GMD_GMK_SW_BODY_RECT_TOP,
						GMD_GMK_SW_BODY_RECT_RIGHT, GMD_GMK_SW_BODY_RECT_BOTTOM);
	rect_work->flag &= ~OBD_RECT_ENABLE;
#endif


	/* 地形設定 */
	col_work = &gmk_work->ene_com.col_work;
	col_work->obj_col.obj		= obj_work;
	col_work->obj_col.width		= GMD_GMK_SW_COL_RECT_WIDTH;			// 地形サイズ設定(ドット)
	col_work->obj_col.height	= GMD_GMK_SW_COL_RECT_HEIGHT;
	col_work->obj_col.ofst_x	= GMD_GMK_SW_COL_RECT_OFST_X;
	col_work->obj_col.ofst_y	= GMD_GMK_SW_TOP_OFF_POS_OFST_Y;		// 初期位置 OFF位置設定

	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
		// トロッコステージは引っかからないようにする
		//col_work->obj_col.ofst_y	= -1;
		col_work->obj_col.obj = NULL;

		// 矩形で設定
		// BODY
		rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
		rect_work->ppDef = gmGmkSwitchDefFunc;
		rect_work->ppHit = NULL;
		ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
		ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		ObjRectWorkSet(rect_work,
							GMD_GMK_SW_BODY_RECT_LEFT, GMD_GMK_SW_BODY_RECT_TOP,
							GMD_GMK_SW_BODY_RECT_RIGHT, GMD_GMK_SW_BODY_RECT_BOTTOM);
		rect_work->flag |= OBD_RECT_NODAMAGE | OBD_RECT_ENABLE;
	}

	/* フラグ設定 */
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形チェックなし
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	gmk_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOPRESSDIE;	// 圧死させない

	/* 終了処理差し替え */
	//mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkSwDest);

	/* スイッチID取得 */
	MTM_ASSERT((u8)eve_rec->left < GMD_GMK_SW_MAX);
	switch_work->id = (u32)MTM_MATH_CLIP(eve_rec->left, 0, GMD_GMK_SW_MAX);

	/* スイッチ復帰待機時間取得 */
	switch_work->time = eve_rec->width*60*FX32_ONE + eve_rec->top*FX32_ONE;
	if (switch_work->time && switch_work->time < GMD_GMK_SW_TIME_MIN) {
		// スイッチが下がる時にプレイヤーが間に合わない可能性があるので少し増やす
		switch_work->time = GMD_GMK_SW_TIME_MIN;
	}

	// シーケンス設定
	if (gm_gmk_switch_state[switch_work->id].sw) {
		// スイッチ上部初期位置
		switch_work->top_pos_y = GMD_GMK_SW_TOP_ON_POS_OFST_Y;
		// ONシーケンスへ
		gmGmkSwOnInit(obj_work, FALSE);
	}
	else {
		// スイッチ上部初期位置
		switch_work->top_pos_y = GMD_GMK_SW_TOP_OFF_POS_OFST_Y;
		// OFFシーケンスへ
		gmGmkSwOffInit(obj_work);
	}

	// 地形設定
	gmGmkSwSetCol(&switch_work->gmk_work.ene_com.col_work, switch_work->top_pos_y);

	return (obj_work);
}

// ==========================================================================
// GmGmkSwitchIsOn
/*!
 *	ギミック スイッチ 状態チェック
 *
 *	@param sw_id	[io] スイッチID
 *
 *	@retuen		TRUE : スイッチON
 */
// ==========================================================================
BOOL GmGmkSwitchIsOn(u32 sw_id)
{
	return (gm_gmk_switch_state[sw_id].sw);
}

// ==========================================================================
// GmGmkSwitchTypeIsGear
/*!
 *	ギミック スイッチ 歯車タイプチェック
 *
 *	@param sw_id	[io] スイッチID
 *
 *	@retuen		TRUE : 歯車スイッチ
 */
// ==========================================================================
BOOL GmGmkSwitchTypeIsGear(u32 sw_id)
{
	return (gm_gmk_switch_state[sw_id].gear);
}

// ==========================================================================
// GmGmkSwitchSetOnGearSwitch
/*!
 *	ギミック スイッチ 歯車スイッチ ON設定
 *
 *	@param sw_id	[io] スイッチID
 *	@param per		[io] 展開率 (0 ～ FX32_ONE)
 */
// ==========================================================================
void GmGmkSwitchSetOnGearSwitch(u32 sw_id, fx32 per)
{
	gm_gmk_switch_state[sw_id].sw	= TRUE;
	gm_gmk_switch_state[sw_id].gear	= TRUE;
	gm_gmk_switch_state[sw_id].per	= per;
}

// ==========================================================================
// GmGmkSwitchSetOffGearSwitch
/*!
 *	ギミック スイッチ 歯車スイッチ OFF設定
 *
 *	@param sw_id	[io] スイッチID
 *	@param per		[io] 展開率 (0 ～ FX32_ONE)
 */
// ==========================================================================
void GmGmkSwitchSetOffGearSwitch(u32 sw_id, fx32 per)
{
	gm_gmk_switch_state[sw_id].sw	= FALSE;
	gm_gmk_switch_state[sw_id].gear	= TRUE;
	gm_gmk_switch_state[sw_id].per	= per;
}

// ==========================================================================
// GmGmkSwitchGetPer
/*!
 *	ギミック スイッチ 歯車スイッチ 展開度取得
 *
 *	@param sw_id	[io] スイッチID
 *
 *	@return 展開度
 */
// ==========================================================================
fx32 GmGmkSwitchGetPer(u32 sw_id)
{
#if 1
	return (gm_gmk_switch_state[sw_id].per);
#else
	if (gm_gmk_switch_state[sw_id].sw) {
		return (gm_gmk_switch_state[sw_id].spd);
	}
	return (0);
#endif
}

//----- Local Functions -----------------------------------------------------
#if 0
// ==========================================================================
// gmGmkSwFwInit
/*!
 *	ギミック スイッチ FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwDest(MTS_TASK_TCB *tcb)
{
	GMS_ENEMY_COM_WORK	*ene_com;

	ene_com = (GMS_ENEMY_COM_WORK*)mtTaskGetTcbWork(tcb);



	// 汎用終了処理
	ObjObjectExit(tcb);
}
#endif



// ==========================================================================
// gmGmkSwOffInit
/*!
 *	ギミック スイッチ OFF状態初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwOffInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SW_WORK	*switch_work = (GMS_GMK_SW_WORK*)obj_work;

	// スイッチOFF設定
	gm_gmk_switch_state[switch_work->id].sw		= FALSE;
	gm_gmk_switch_state[switch_work->id].time	= 0;

	// クリッピングOFF
	obj_work->flag &= ~OBD_OBJECT_NOCLIP;

	// スイッチ上部位置
	if (switch_work->top_pos_y < GMD_GMK_SW_TOP_OFF_POS_OFST_Y) {
		// 念のため
		switch_work->top_pos_y = GMD_GMK_SW_TOP_OFF_POS_OFST_Y;
	}

	obj_work->ppFunc = gmGmkSwOffMain;
}

// ==========================================================================
// gmGmkSwOffMain
/*!
 *	ギミック スイッチ OFF状態メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwOffMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SW_WORK	*switch_work = (GMS_GMK_SW_WORK*)obj_work;

	// スイッチがあがっていない場合は演出
	if (switch_work->top_pos_y > GMD_GMK_SW_TOP_OFF_POS_OFST_Y) {
		switch_work->top_pos_y += GMD_GMK_SW_TOP_OFF_SPD;
		if (switch_work->top_pos_y < GMD_GMK_SW_TOP_OFF_POS_OFST_Y) {
			switch_work->top_pos_y = GMD_GMK_SW_TOP_OFF_POS_OFST_Y;
		}

		// 地形再設定
		gmGmkSwSetCol(&switch_work->gmk_work.ene_com.col_work, switch_work->top_pos_y);
	}

	if ((switch_work->gmk_work.ene_com.col_work.obj_col.rider_obj &&
				switch_work->gmk_work.ene_com.col_work.obj_col.rider_obj->obj_type == GMD_OBJTYPE_PLAYER) ||
			(switch_work->gmk_work.ene_com.enemy_flag & GMD_GMK_SWITCH_ENEMY_FLAG_RIDER)) {
		// スイッチON
		gmGmkSwOnInit(obj_work, TRUE);
	}

	// 乗っているフラグOFF
	switch_work->gmk_work.ene_com.enemy_flag &= ~GMD_GMK_SWITCH_ENEMY_FLAG_RIDER;
}

// ==========================================================================
// gmGmkSwOnInit
/*!
 *	ギミック スイッチ ON状態初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 *	@param now_on	[in] 押した時TRUE (以前からONだった場合FALSE)
 */
// ==========================================================================
void gmGmkSwOnInit(OBS_OBJECT_WORK *obj_work, BOOL now_on)
{
	GMS_GMK_SW_WORK	*switch_work = (GMS_GMK_SW_WORK*)obj_work;

	// スイッチON設定
	gm_gmk_switch_state[switch_work->id].sw		= TRUE;
	gm_gmk_switch_state[switch_work->id].time	= switch_work->time;

	// 時間設定ありのものはクリッピング無し
	if (switch_work->time) {
		obj_work->flag |= OBD_OBJECT_NOCLIP;
	}

	// スイッチ上部位置
	if (switch_work->top_pos_y > GMD_GMK_SW_TOP_ON_POS_OFST_Y) {
		// 念のため
		switch_work->top_pos_y = GMD_GMK_SW_TOP_ON_POS_OFST_Y;
	}
	
	if (now_on) {
	    // SE
		GmSoundPlaySE("Switch");

		// 振動
		GMM_PAD_VIB_SMALL();

		// エフェクト
		GmComEfctCreateSpring(obj_work, 0/*ofst_x*/, -8*FX32_ONE/*ofst_y*/, -obj_work->pos.z);
	}

	obj_work->ppFunc = gmGmkSwOnMain;
}

// ==========================================================================
// gmGmkSwOnMain
/*!
 *	ギミック スイッチ ON状態メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwOnMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SW_WORK	*switch_work = (GMS_GMK_SW_WORK*)obj_work;

	// スイッチが押し込まれていない場合は演出
	if (switch_work->top_pos_y < GMD_GMK_SW_TOP_ON_POS_OFST_Y) {
		switch_work->top_pos_y += GMD_GMK_SW_TOP_ON_SPD;
		if (switch_work->top_pos_y > GMD_GMK_SW_TOP_ON_POS_OFST_Y) {
			switch_work->top_pos_y = GMD_GMK_SW_TOP_ON_POS_OFST_Y;
		}

		// 地形再設定
		gmGmkSwSetCol(&switch_work->gmk_work.ene_com.col_work, switch_work->top_pos_y);
	}


	// 乗っているフラグOFF
	switch_work->gmk_work.ene_com.enemy_flag &= ~GMD_GMK_SWITCH_ENEMY_FLAG_RIDER;


	if ((switch_work->gmk_work.ene_com.col_work.obj_col.rider_obj &&
				switch_work->gmk_work.ene_com.col_work.obj_col.rider_obj->obj_type == GMD_OBJTYPE_PLAYER) ||
			(switch_work->gmk_work.ene_com.enemy_flag & GMD_GMK_SWITCH_ENEMY_FLAG_RIDER)) {
		// 有効時間更新
		gm_gmk_switch_state[switch_work->id].time = switch_work->time;
	}
	else if (gm_gmk_switch_state[switch_work->id].time) {
		gm_gmk_switch_state[switch_work->id].time =
				ObjTimeCountDown(gm_gmk_switch_state[switch_work->id].time);

		if (!gm_gmk_switch_state[switch_work->id].time) {
			// スイッチ終了
			gmGmkSwOffInit(obj_work);
		}
	}

	// 乗っているフラグOFF
	switch_work->gmk_work.ene_com.enemy_flag &= ~GMD_GMK_SWITCH_ENEMY_FLAG_RIDER;
}


// ==========================================================================
// gmGmkSwitchDefFunc
/*!
 *	ギミック スイッチ 矩形 くらい処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 *		ppDefに登録\n
 *		3-2専用発動矩形処理
 */
// ==========================================================================
void gmGmkSwitchDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_ENEMY_COM_WORK	*com_work = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	if (com_work == NULL) {
		return;
	}
	if (ply_work == NULL || ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		return;
	}

	// 乗っている人がいるフラグON
	com_work->enemy_flag |= GMD_GMK_SWITCH_ENEMY_FLAG_RIDER;
}


// ==========================================================================
// gmGmkSwDispFunc
/*!
 *	ギミック スイッチ 描画処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSwDispFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SW_WORK		*switch_work = (GMS_GMK_SW_WORK*)obj_work;
	VecFx32				pos;
	u32					disp_flag;

	// disp_flag 取得
	disp_flag = obj_work->disp_flag;

	// トップ描画
	pos = obj_work->pos;
	pos.y += switch_work->top_pos_y << FX32_SHIFT;
	ObjDrawAction3DNN(obj_work->obj_3d, &pos, &obj_work->dir, &obj_work->scale, &obj_work->disp_flag);

	// 本体描画
	ObjDrawAction3DNN(&switch_work->obj_3d_base, &obj_work->pos, &obj_work->dir, &obj_work->scale, &disp_flag);
}



// ==========================================================================
// gmGmkSwSetCol
/*!
 *	地形再設定
 *
 *	@param	col_work	[in]	こりジョンワーク
 *	@param	top_pos_y	[in]	スイッチトップ位置
 */
// ==========================================================================
void gmGmkSwSetCol(OBS_COLLISION_WORK *col_work, s32 top_pos_y)
{
	/* 地形設定 */
	//col_work->obj_col.height	= GMD_GMK_SW_COL_RECT_HEIGHT;
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
		// トロッコステージは引っかからないようにする
		//col_work->obj_col.ofst_y	= -1;
	}
	else {
		col_work->obj_col.ofst_y	= (s16)top_pos_y;
	}
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
