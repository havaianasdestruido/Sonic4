// ===========================================================================
/*!
	@file	dmStfrlMdlCtrlMdlCtrl.cpp
	@brief	デモ・スタッフロール画面モデル操作モジュール

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmStaffRollMdlCtrl.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
	スタッフロール作成メモ
	
	１、スタッフロールではAoActionを使用せず、通常プリミティブ描画に
		テクスチャを貼って表示する処理にする。
	
	２、Wii容量削減のため、データを軽くするために書き文字ではなく、
		プログラムで文字列データ(ASCII)を見て、テクスチャから文字を
  		切り出して文字列を表示するやり方にする。
  		(aoYsdFileモジュールを使用)
	
	３、スクリーンショットのデータは一枚ずつデータとして持ち、
  		初めに3枚分読み込んだあとは、一枚表示が終るごとに、
  		次の一枚を読み込むような裏読み方式にて行う。
	
	４、仕様書ではEND画面とスタッフロール画面とで２つあるように記述しているが、
  		処理としては全てこのモジュール内で行うようにする。
	
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "objObject.h"
#include "objObjectLoad.h"

#include "gmMain.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmObj.h"
#include "gmPlayer.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmComEfct.h"
#include "gmEffectBoss.h"
#include "gmBossCommon.h"
#include "gmBoss1.h"
#include "dmStaffRollMdlCtrl.h"

#include "akMath.h"
#include "izFade.h"
#include "aoWinSys.h"

// データヘッダ
#include "common/model/SON_MDL.hmb"
#include "common/model/SON_MTN.hmb"

#if _IPHONE
#include "model/RING_MAT.HMB"
#endif //_IPHONE

// モデルデータヘッダ
#include "../file/common/arc/BOSS03.hmb"
#include "../file/common/model/BOSS03_MDL.hmb"
#include "../file/common/model/BOSS03_BODY_MTN.hmb"
#include "../file/common/model/BOSS03_EGG_MTN.hmb"

// ----- Macros ------------------------------------------------（マクロ定義）

// 表示関連
#define DMD_STFRL_BOSS_BODY_NODE_IDX_EGG_CONNECT	(11)	//!< エッグマン接続ノード
#define DMD_STFRL_BOSS_BODY_NODE_IDX_BODY_POSTURE	(2)		//!< 本体姿勢

// 演出関連
#define DMD_STFRL_FIRST_DISP_WAIT_TIME		(240.f)
#define DMD_STFRL_EFCT_FADE_IN_TIME			(32.f)
#define DMD_STFRL_EFCT_FADE_OUT_TIME		(32.f)
#define DMD_STFRL_EFCT_FADE_WAIT_TIME		(32.f)

#define DMD_STFRL_EFCT_QSTN_COLOR_INIT		(255)
#define DMD_STFRL_EFCT_QSTN_ALPHA_INIT		(0)
#define DMD_STFRL_EFCT_QSTN_ALPHA_MAX		(255)
#define DMD_STFRL_EFCT_QSTN_FADE_SPD		(8)

#define DMD_STFRL_DISP_LAST_LIST_PAGE		(17)

#define DMD_STFRL_LOAD_LOOP_TIME			(12.0f)
#define DMD_STFRL_LOADED_WAIT_TIME			(60.0f)

#define DMD_STFRL_SONIC_MOVE_INIT_SPD		(50.f)
#define DMD_STFRL_SONIC_MAIN_INIT_POS		(48.f)
#define DMD_STFRL_SONIC_OTHER_INIT_POS		(0.f)
#define DMD_STFRL_SONIC_MOVE_SPD			(12.f)
#define DMD_STFRL_SONIC_MOVE_ACCEL			(0.8f)
#define DMD_STFRL_SONIC_FW_MTN_TIME			(300)
#define DMD_STFRL_SONIC_FADE_START_TIME		(60)
#define DMD_STFRL_SONIC_FADE_END_TIME		(120)

#define DMD_STFRL_BOSS_MOVE_DOWN_SPD			(1.0 * FX32_ONE)
#define DMD_STFRL_BOSS_MOVE_UP_SPD				(-0.6 * FX32_ONE)
#define DMD_STFRL_BOSS_NODISP_HEIGHT_POS_Y		(-180 * FX32_ONE)
#define DMD_STFRL_BOSS_DISP_HEIGHT_POS_Y		(-16 * FX32_ONE)
#define DMD_STFRL_BOSS_COMP_DISP_HEIGHT_POS_Y	(-40 * FX32_ONE)
#define DMD_STFRL_BOSS_COMP_EFCT_START_TIME		(60)
#define DMD_STFRL_BOSS_NOCOMP_EFCT_START_TIME	(120)
#define DMD_STFRL_BOSS_EGG_LAUGH_TIME			(180)
#define DMD_STFRL_BOSS_M_SONIC_EFCT_TIME		(100)
#define DMD_STFRL_BOSS_INIT_POS_X				(0 * FX32_ONE)
#define DMD_STFRL_BOSS_INIT_POS_Y				(-16 * FX32_ONE)
#define DMD_STFRL_BOSS_INIT_POS_Z				(-20 * FX32_ONE)
#define DMD_STFRL_BOSS_INIT_DISP_DIR			(300)

#define DMD_STFRL_ONE_AROUND_DIR			(0x10000)
//#define DMD_STFRL_RING_EFCT_DISP_NUM		(8)
#if !_IPHONE
#define DMD_STFRL_RING_ROTATE_SPD			(0x200)
#else //!_IPHONE
#define DMD_STFRL_RING_ROTATE_SPD			(0x000)
#endif //!_IPHONE
#define DMD_STFRL_RING_DISP_TIME			(10)
#define DMD_STFRL_RING_NODISP_TIME			(60)



// フラグ関連
#define DMD_STFRL_SONIC_FLAG_EFCT_END			(1 << 0)		//!< 

#define DMD_STFRL_BODY_FLAG_COMPLETE_EFCT		(1 << 0)		//!< 
#define DMD_STFRL_BODY_FLAG_MOVE_DOWN_START		(1 << 1)		//!< 
#define DMD_STFRL_BODY_FLAG_MOVE_UP_START		(1 << 2)		//!< 
#define DMD_STFRL_BODY_FLAG_COMP_EFCT_END		(1 << 3)		//!< 
#define DMD_STFRL_BODY_FLAG_M_SONIC_EFCT_START	(1 << 4)		//!< 

#define DMS_STFRL_FLAG_CHNG_MTN_EGG_LAUGH_REQ	(1 << 21)		//!< エッグマン笑いモーション切り替えフラグ
#define DMS_STFRL_EGG_FLAG_CHNG_MTN_EGG_LAUGH	(1 << 0)		//!< エッグマン笑いモーション切り替えフラグ

#define DMD_STFRL_RING_FLAG_SPLASH_EFCT_START	(1 << 0)


// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）
#if _WII
/// Wii用プレイヤーマテリアルコールバックパラメータ
typedef struct tag_DMS_STFRL_MAT_CALLBACK_PARAM {
	u32		draw_id;
} DMS_STFRL_MAT_CALLBACK_PARAM;
#endif	// #if _WII

// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// ソニック関連
static void dmStfrlMdlCtrlSonicProcWaitSetup(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlSonicProcWaitChngDash2(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlSonicProcWaitMtnEnd(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlSonicProcWaitFadeEnd(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlSonicProcIdle(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlSonicDrawFunc(OBS_OBJECT_WORK *obj_work);

#if _WII
static void dmStfrlSetLodAction(DMS_STFRL_SONIC_WORK *sonic_work, s32 act_id);
static void dmStfrlSetLodActionFrame(DMS_STFRL_SONIC_WORK *sonic_work, fx32 frame);
static void dmStfrlMotionCallback(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param);
static NNE_BOOL dmStfrlMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif

// ボス関連
static void dmStfrlMdlCtrlBoss1BodyExit(MTS_TASK_TCB *tcb);
static void dmStfrlMdlCtrlBodyProcWaitSetup(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlBodyProcBodyMain(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlEggProcWaitSetup(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlEggProcMain(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlEggProcMainIdle(OBS_OBJECT_WORK *obj_work);

static void dmStfrlMdlCtrlBodyProcBodyCompStartWait(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlBodyProcBodyCompInitStart(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlBodyProcBodyCompMoveDown(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlBodyProcBodyCompLaughWait(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlBodyProcBodyCompMoveUpWait(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlBodyProcBodyCompEndWaitIdle(OBS_OBJECT_WORK *obj_work);

// リング関連
static void dmStfrlMdlCtrlRingProcInitSetup(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlRingProcDispIdle(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlRingProcNoDispIdle(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlRingProcStartWait(OBS_OBJECT_WORK *obj_work);
static void dmStfrlMdlCtrlRingDrawFunc(OBS_OBJECT_WORK *obj_work);

static void dmStfrlMdlCtrlCreateRingEfct(fx32 pos_x, fx32 pos_y);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）

// リング表示開始位置テーブル
const static fx32 dm_stfrl_ring_disp_pos_tbl[15][2] = {
	{-128 * FX32_ONE,   4 * FX32_ONE},
	{  20 * FX32_ONE,  26 * FX32_ONE},
	{ -48 * FX32_ONE, -18 * FX32_ONE},
	{   8 * FX32_ONE, -62 * FX32_ONE},
	{ -48 * FX32_ONE, -40 * FX32_ONE},
	{-108 * FX32_ONE,   4 * FX32_ONE},
	{ -40 * FX32_ONE,  26 * FX32_ONE},
	{ -18 * FX32_ONE, -18 * FX32_ONE},
	{   0 * FX32_ONE, -62 * FX32_ONE},
	{-128 * FX32_ONE, -40 * FX32_ONE},
	{ -60 * FX32_ONE,   4 * FX32_ONE},
	{   8 * FX32_ONE,  26 * FX32_ONE},
	{  20 * FX32_ONE, -18 * FX32_ONE},
	{ -68 * FX32_ONE, -62 * FX32_ONE},
	{ -60 * FX32_ONE, -40 * FX32_ONE},
};



// リングエフェクト表示開始位置オフセットテーブル
const static fx32 dm_stfrl_ring_efct_disp_offset_tbl[15][2] = {
	{  0 * FX32_ONE,   0 * FX32_ONE},
	{ 16 * FX32_ONE,  16 * FX32_ONE},
	{-16 * FX32_ONE,  16 * FX32_ONE},
	{  0 * FX32_ONE,   0 * FX32_ONE},
	{ 24 * FX32_ONE, -16 * FX32_ONE},
	{  8 * FX32_ONE,  24 * FX32_ONE},
	{  0 * FX32_ONE,   0 * FX32_ONE},
	{  6 * FX32_ONE, -18 * FX32_ONE},
	{ 17 * FX32_ONE,  14 * FX32_ONE},
	{  0 * FX32_ONE,   0 * FX32_ONE},
	{ 24 * FX32_ONE, -16 * FX32_ONE},
	{  8 * FX32_ONE,  24 * FX32_ONE},
	{  0 * FX32_ONE,   0 * FX32_ONE},
	{  6 * FX32_ONE, -18 * FX32_ONE},
	{ 17 * FX32_ONE,  14 * FX32_ONE},
};



static OBS_ACTION3D_NN_WORK *dm_stfrl_sonic_obj_3d_list	= NULL;
static OBS_ACTION3D_NN_WORK *dm_stfrl_boss1_obj_3d_list	= NULL;
static OBS_ACTION3D_NN_WORK	*dm_stfrl_ring_obj_3d = NULL;		//!< リング描画用オブジェクト		

// ----- Global Functions ----------------------（グローバル関数の定義：外部）




// ------------------------------- SONIC関連 ------------------------------------
// =======================================================================
// DmStfrlMdlCtrlSonicBuild
/*!
   スタッフロール用ソニック データ構築
 */
// =======================================================================
void DmStfrlMdlCtrlSonicBuild(void)
{
	// ソニックモデル
	dm_stfrl_sonic_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_MDL].pData,
								(AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_TEX].pData,
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);

#if _WII
	// LODデータビルド
//	GmPlyLodCnvAddr(g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_LOD].pData);
#endif

}


// =======================================================================
// DmStfrlMdlCtrlSonicFlush
/*!
   スタッフロール用ソニック データ解放
 */
// =======================================================================
void DmStfrlMdlCtrlSonicFlush(void)
{
	AMS_AMB_HEADER	*amb;
	
	// ソニックオブジェクト開放
	amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_MDL].pData;
	GmGameDBuildRegFlushModel(dm_stfrl_sonic_obj_3d_list, amb->file_num);
	
	dm_stfrl_sonic_obj_3d_list = NULL;
}



// ==========================================================================
// DmStfrlMdlCtrlSetSonicObj
/*!
	スタッフロール用のソニックOBJ設定処理
 */
// ==========================================================================
DMS_STFRL_SONIC_WORK *DmStfrlMdlCtrlSetSonicObj(void)
{
	DMS_STFRL_SONIC_WORK *sonic_work = NULL;
	OBS_OBJECT_WORK	*obj_work = NULL;
	OBS_ACTION3D_NN_WORK *obj_3d = NULL;
	
#if defined(MTD_DEBUG)  // デバッグ版
    OS_TPrintf( "● StfrlMdlCtrlSonicInit ●\n" );
#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版
	
	// ボスオブジェクトタスク作成
	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(0x6000
										   , 0
										   , GMD_TASK_PAUSELEVEL_DEF
										   , GMD_TASK_PAUSELEVEL_DEF
										   , sizeof(DMS_STFRL_SONIC_WORK)
										   , "STAFFROLL_SONIC"
										   );
	
	// 本体ワーク保存
	sonic_work = (DMS_STFRL_SONIC_WORK *)obj_work;
	
	// 終了処理差し替え
//	mtTaskChangeTcbDestructor(obj_work->tcb, dmStfrlMdlCtrlBoss1BodyExit);
	
	
	// 本体モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 dm_stfrl_sonic_obj_3d_list,
								 obj_work->obj_3d);
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= 0.0625;//0.125f;
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	obj_3d = sonic_work->obj_work.obj_3d;
#if _WII
	// Wiiは専用マテリアルコールバック設定
	obj_3d->material_cb_func = dmStfrlMaterialCallback;
	// モーションコールバック設定
	obj_3d->mtn_cb_func = dmStfrlMotionCallback;
	obj_3d->mtn_cb_param = sonic_work;
	
	// ロッドモーションデータ設定
	sonic_work->hand_lod_header = (GMS_PLY_LOD_MTN_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_LOD].pData;
#endif
	
	
	// 本体モーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,
								  TRUE,
								  &g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_MOTION],
								  NULL,
								  0,
								  NULL);
	
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	|= OBD_DISP_HFLIP | OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;
	obj_work->disp_flag &= ~OBD_DISP_NODISP;
	
	obj_work->disp_flag |= OBD_DISP_3D_PARALLEL | OBD_DISP_USERMTX_RIGHT | OBD_DISP_DRAWSTATE;
	
	// アルファ制御あり
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_ALPHA;
	
	obj_work->pos.x = 0;
	obj_work->pos.y = 24 * FX32_ONE;
	obj_work->pos.z = -3 * FX32_ONE;
	
	
	// 表示角度
	obj_work->dir.y = AKM_DEGtoA16(90);
	
	// 初期状態は表示にする
	obj_work->obj_3d->draw_state.alpha.alpha = 1.f;
	sonic_work->alpha = 1.f;
	
	// 初期状態のモーション設定
	ObjDrawObjectActionSet(obj_work, IDB_SON_MTN_SON_GOAL_02_ZNM);
	
#if _WII
	// 手ロッドアクション設定
	dmStfrlSetLodAction(sonic_work, IDB_SON_MTN_SON_GOAL_02_ZNM);
#endif
	
	obj_work->ppOut		= dmStfrlMdlCtrlSonicDrawFunc;
	
	// メイン処理設定
	obj_work->ppFunc	= dmStfrlMdlCtrlSonicProcWaitSetup;
	
	return sonic_work;
}



#if _WII
// ==========================================================================
// dmStfrlSetLodAction
/*!
 *	ロッドアクション設定
 *
 *	@param	sonic_work	[in]	プレイヤーワーク
 *	@param	act_id		[in]	アクションID
 */
// ==========================================================================
void dmStfrlSetLodAction(DMS_STFRL_SONIC_WORK *sonic_work, s32 act_id)
{
	MTM_ASSERT(sonic_work);
	MTM_ASSERT(act_id < sonic_work->hand_lod_header->mtn_num);

	sonic_work->hand_lod_mtn = sonic_work->hand_lod_header->mtn_data + act_id;
	sonic_work->hand_lod_pat = sonic_work->hand_lod_mtn->pat_data;
	sonic_work->hand_lod_pat_no = 0;
}


// ==========================================================================
// dmStfrlSetLodActionFrame
/*!
 *	ロッドアクションフレーム設定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	frame		[in]	設定フレーム
 */
// ==========================================================================
void dmStfrlSetLodActionFrame(DMS_STFRL_SONIC_WORK *sonic_work, fx32 frame)
{
	u32				i;
	GMS_PLY_LOD_PAT	*pat_data;

	if (sonic_work->hand_lod_pat->start_frame <= frame) {
		pat_data = sonic_work->hand_lod_pat;
		i		 = sonic_work->hand_lod_pat_no;
	}
	else {
		pat_data = sonic_work->hand_lod_mtn->pat_data;
		i		 = 0;
	}

	for (; i < sonic_work->hand_lod_mtn->pat_num; i++, pat_data++) {
		if (((s32)pat_data->start_frame << FX32_SHIFT) > frame) {
			break;
		}
	}

	MTM_ASSERT(i != 0);

	if (i != 0) {
		i--;
		pat_data--;
	}

	// パターンデータ設定
	sonic_work->hand_lod_pat		= pat_data;
	sonic_work->hand_lod_pat_no	= i;
}


// ==========================================================================
// dmStfrlMotionCallback
/*!
 *	プレイヤー モーションコールバック
 *
 *	@param	motion	[in]	モーション
 *	@param	object	[in]	オブジェクト
 *	@param	param	[in]	パラメータ	(ply_work)
 */
// ==========================================================================
void dmStfrlMotionCallback(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param)
{
	DMS_STFRL_SONIC_WORK			*sonic_work = (DMS_STFRL_SONIC_WORK*)param;
	OBS_ACTION3D_NN_WORK			*obj_3d = sonic_work->obj_work.obj_3d;
	DMS_STFRL_MAT_CALLBACK_PARAM	*ply_mat_cb_param;

	UNREFERENCED_PARAMETER(motion);
	UNREFERENCED_PARAMETER(object);

	// 手ロッドモーションUpdate
	dmStfrlSetLodActionFrame(sonic_work, FXM_FLOAT_TO_FX32(obj_3d->frame[0]));

	// マテリアルコールバック設定
	ply_mat_cb_param = (DMS_STFRL_MAT_CALLBACK_PARAM *)amDrawMallocDataBuffer(sizeof(DMS_STFRL_MAT_CALLBACK_PARAM));
	ply_mat_cb_param->draw_id = sonic_work->hand_lod_pat->user_data;
	obj_3d->material_cb_param = ply_mat_cb_param;
}

// ==========================================================================
// dmStfrlMaterialCallback
/*!
 *	Wii用 プレイヤーマテリアルコールバック
 *
 *	@param val				[in]	NNS_DRAWCALLBACK_VAL構造体へのポインタ
 *	@param param			[in]	ユーザーパラメータ(GMS_PLAYER_MAT_CALLBACK_PARAM)
 *
 *	@return		NNE_BOOL
 */
// ==========================================================================
#if 1
NNE_BOOL dmStfrlMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	u32								user_data;
	DMS_STFRL_MAT_CALLBACK_PARAM	*user_param;

	if (param) {
		user_param = (DMS_STFRL_MAT_CALLBACK_PARAM*)param;

		// ユーザーデータ取得
		user_data = ObjDraw3DNNGetMaterialUserData(val);

		if (!(!user_data ||
				((user_data & GMD_PLY_LOD_TYPE_R_MASK) ==
					(user_param->draw_id & GMD_PLY_LOD_TYPE_R_MASK)) ||
				((user_data & GMD_PLY_LOD_TYPE_L_MASK) ==
					(user_param->draw_id & GMD_PLY_LOD_TYPE_L_MASK)))) {
			return (NNE_FALSE);
		}
	}

	// Toon汎用処理
	return (ObjDrawToonMaterialCallback(val, param));
}
#endif

#endif // #if _WII



// ------------------------------- BOSS関連 ------------------------------------

// =======================================================================
// DmStfrlMdlCtrlBoss1Build
/*!
   スタッフロール用ボス１ データ構築
 */
// =======================================================================
void DmStfrlMdlCtrlBoss1Build(void)
{
	void	*mdl_amb;
	void	*tex_amb;
	mdl_amb	= ObjDataLoadAmbIndex(NULL, IDB_BOSS03_BOSS03_MDL_AMB, g_gm_gamedat_enemy_arc);
	tex_amb	= ObjDataLoadAmbIndex(NULL, IDB_BOSS03_BOSS03_TEX_AMB, g_gm_gamedat_enemy_arc);
	
	// モデル構築
	dm_stfrl_boss1_obj_3d_list	=
		GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)mdl_amb
								  , (AMS_AMB_HEADER*)tex_amb
								  , NND_DRAWOBJ_SHADER_USER_PROFILE_TOON
								  );
	
	// モーション
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_03_BODY_MTN),
						IDB_BOSS03_BOSS03_BODY_MTN_AMB, g_gm_gamedat_enemy_arc);
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_03_EGG_MTN),
						IDB_BOSS03_BOSS03_EGG_MTN_AMB, g_gm_gamedat_enemy_arc);
	
	
}


// =======================================================================
// DmStfrlMdlCtrlBoss1Flush
/*!
   スタッフロール用ボス１ データ片付け
 */
// =======================================================================
void DmStfrlMdlCtrlBoss1Flush(void)
{
	AMS_AMB_HEADER	*mdl_amb;
	
	// モーション
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_03_EGG_MTN));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_03_BODY_MTN));
	
	// モデル解放
	mdl_amb	= (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(NULL, IDB_BOSS03_BOSS03_MDL_AMB, g_gm_gamedat_enemy_arc);
	
	GmGameDBuildRegFlushModel(dm_stfrl_boss1_obj_3d_list, mdl_amb->file_num);
	
	dm_stfrl_boss1_obj_3d_list = NULL;
}




// ==========================================================================
// DmStfrlMdlCtrlSetBodyObj
/*!
	スタッフロール用のボスOBJ設定処理
 */
// ==========================================================================
DMS_STFRL_BOSS_BODY_WORK *DmStfrlMdlCtrlSetBodyObj(void)
{
	DMS_STFRL_BOSS_BODY_WORK *body_work = NULL;
	OBS_OBJECT_WORK	*obj_work = NULL;
	
#if defined(MTD_DEBUG)  // デバッグ版
    OS_TPrintf( "● StfrlMdlCtrlBossBodyInit ●\n" );
#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版
	
	// ボスオブジェクトタスク作成
	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(0x6000
										   , 0
										   , GMD_TASK_PAUSELEVEL_DEF
										   , GMD_TASK_PAUSELEVEL_DEF
										   , sizeof(DMS_STFRL_BOSS_BODY_WORK)
										   , "BOSS1_BODY"
										   );
	
	// 本体ワーク保存
	body_work = (DMS_STFRL_BOSS_BODY_WORK *)obj_work;
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, dmStfrlMdlCtrlBoss1BodyExit);
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	|= OBD_DISP_HFLIP | OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;
	obj_work->disp_flag &= ~OBD_DISP_NODISP;
	
	
	// 本体モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 &dm_stfrl_boss1_obj_3d_list[IDB_BOSS03_MDL_B03_BODY_ZNO],
								 obj_work->obj_3d);
	
	// 本体モーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,
								  TRUE,
								  ObjDataGet(GMD_DWORK_NO_BOSS_03_BODY_MTN),
								  NULL,
								  0,
								  NULL);
	
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= 0.125f;// TODO : 仮
	
	obj_work->ppOut		= ObjDrawActionSummary;
	
	// メイン処理設定
	obj_work->ppFunc	= dmStfrlMdlCtrlBodyProcWaitSetup;
	
	return body_work;
}



// =======================================================================
// DmStfrlMdlCtrlSetEggObj
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
DMS_STFRL_BOSS_EGG_WORK	*DmStfrlMdlCtrlSetEggObj(OBS_OBJECT_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work;
	DMS_STFRL_BOSS_EGG_WORK	*egg_work;
	
#if defined(MTD_DEBUG)  // デバッグ版
    OS_TPrintf( "● StfrlMdlCtrlBossEggmanInit ●\n" );
#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版
	
	// エッグマンオブジェクトタスク作成
	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(0x6000
										   , 0
										   , GMD_TASK_PAUSELEVEL_DEF
										   , GMD_TASK_PAUSELEVEL_DEF
										   , sizeof(DMS_STFRL_BOSS_EGG_WORK)
										   , "BOSS1_EGG"
										   );
	
	// エッグワーク保存
	egg_work = (DMS_STFRL_BOSS_EGG_WORK	*)obj_work;
	
	// 親設定
	obj_work->parent_obj = body_work;
	
	// エッグマンの表示位置は本体のノードに付随するため、設定なし
	
	// 地形当たり無し
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	
	ObjObjectCopyAction3dNNModel(obj_work
								 , &dm_stfrl_boss1_obj_3d_list[IDB_BOSS03_MDL_EGGMAN_ZNO]
								 , obj_work->obj_3d
								 );
	
	// エッグマンモーションロード
	ObjObjectAction3dNNMotionLoad(obj_work
								  , 0
								  , TRUE
								  , ObjDataGet(GMD_DWORK_NO_BOSS_03_EGG_MTN)
								  , NULL
								  , 0
								  , NULL
								  );
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ユーザ描画ステートを使用
	obj_work->disp_flag	|= OBD_DISP_DRAWSTATE;
	
	obj_work->ppOut		= ObjDrawActionSummary;
	
	// メイン処理設定
	obj_work->ppFunc	= dmStfrlMdlCtrlEggProcWaitSetup;
	
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	|= OBD_DISP_REPEAT;
	obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;
	
	return egg_work;
}



// ---------------------------- リング関連 ---------------------------------
// ==========================================================================
// データ構築
// ==========================================================================
// ==========================================================================
// DmStfrlMdlCtrlRingBuild
/*!
 *	リングデータ構築
 */
// ==========================================================================
void DmStfrlMdlCtrlRingBuild(void)
{
	MTM_ASSERT(dm_stfrl_ring_obj_3d == NULL);

	// リング描画用オブジェクト取得
	dm_stfrl_ring_obj_3d =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER *)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STFRL_RING_MODEL)
									  , (AMS_AMB_HEADER *)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STFRL_RING_TEX)
									  , 0
									  );
}

// ==========================================================================
// DmStfrlMdlCtrlRingFlush
/*!
 *	リングデータフラッシュ
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
void DmStfrlMdlCtrlRingFlush(void)
{
	// データフラッシュ
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STFRL_RING_MODEL);

	GmGameDBuildRegFlushModel(dm_stfrl_ring_obj_3d, amb->file_num);
	
	dm_stfrl_ring_obj_3d = NULL;
}




// ==========================================================================
// DmStfrlMdlCtrlSetRingObj
/*!
	スタッフロール用のリングOBJ設定処理
 */
// ==========================================================================
DMS_STFRL_RING_WORK *DmStfrlMdlCtrlSetRingObj(s32 delay_time, u32 type)
{
	DMS_STFRL_RING_WORK *ring_work = NULL;
	OBS_OBJECT_WORK	*obj_work = NULL;
	
#if defined(MTD_DEBUG)  // デバッグ版
    OS_TPrintf( "● StfrlMdlCtrlRingInit ●\n" );
#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版
	
	// リングオブジェクトタスク作成
	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(0x6000
										   , 0
										   , GMD_TASK_PAUSELEVEL_DEF
										   , GMD_TASK_PAUSELEVEL_DEF
										   , sizeof(DMS_STFRL_RING_WORK)
										   , "RING_OBJ"
										   );
	
	// 本体ワーク保存
	ring_work = (DMS_STFRL_RING_WORK *)obj_work;
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	|= OBD_DISP_HFLIP | OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;
	obj_work->disp_flag &= ~OBD_DISP_NODISP;
	
	// α値を使用したいのでユーザ描画ステートを使用
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;
	
	// 本体モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 dm_stfrl_ring_obj_3d,
								 obj_work->obj_3d);
#if _IPHONE
	ObjObjectAction3dNNMaterialMotionLoad(obj_work, 0, NULL, NULL, IDB_RING_MAT_RING_INV
									, (void*)ObjDataGet(GMD_DWORK_NO_RING_MAT)->pData);
#endif //_IPHONE
	
	
	obj_work->disp_flag	|= OBD_DISP_HFLIP | OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;
	obj_work->disp_flag &= ~OBD_DISP_NODISP;
	
	obj_work->disp_flag |= OBD_DISP_3D_PARALLEL | OBD_DISP_USERMTX_RIGHT | OBD_DISP_DRAWSTATE;
	
	// アルファ制御あり
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_ALPHA;
	
	// 初期状態は非表示にする
	obj_work->obj_3d->draw_state.alpha.alpha = 0.f;
	
	obj_work->ppOut		= dmStfrlMdlCtrlRingDrawFunc;
	
	// メイン処理設定
	obj_work->ppFunc	= dmStfrlMdlCtrlRingProcStartWait;
	
	ring_work->disp_ring_pos_no = (s32)type;
	
	// 引数の表示位置を取得・保存
	ring_work->start_pos.x = dm_stfrl_ring_disp_pos_tbl[ring_work->disp_ring_pos_no][0];
	ring_work->start_pos.y = dm_stfrl_ring_disp_pos_tbl[ring_work->disp_ring_pos_no][1];
	ring_work->start_pos.z = -3 * FX32_ONE;
	
	// 発射待ち時間
	ring_work->efct_start_time = delay_time;
	
	ring_work->disp_efct_pos_no = (s32)type;
	
	return ring_work;
}







// ------------------------- ソニック関連 --------------------------------

// =======================================================================
// dmStfrlMdlCtrlSonicProcWaitSetup
/*!
  ソニック 生成完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlSonicProcWaitSetup(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_SONIC_WORK *sonic_work	= (DMS_STFRL_SONIC_WORK *)obj_work;
	
	sonic_work->timer++;
	
	// モーションFWのループ設定
	if (obj_work->disp_flag & OBD_DISP_END) {
		
		if (sonic_work->timer > DMD_STFRL_SONIC_FW_MTN_TIME) {
			// ゴール時のモーションを設定
			ObjDrawObjectActionSet3DNNBlend(obj_work
											, IDB_SON_MTN_SON_WALK_ZNM
											);
			
#if _WII
			// 手ロッドアクション設定
			dmStfrlSetLodAction(sonic_work, IDB_SON_MTN_SON_WALK_ZNM);
#endif
			// メイン処理設定
			obj_work->ppFunc	= dmStfrlMdlCtrlSonicProcWaitChngDash2;
			
			sonic_work->timer = 0;
		}
		
		else {
			// ゴール時のモーションを設定
			ObjDrawObjectActionSet3DNNBlend(obj_work
											, IDB_SON_MTN_SON_GOAL_02_ZNM
											);
			
#if _WII
			// 手ロッドアクション設定
			dmStfrlSetLodAction(sonic_work, IDB_SON_MTN_SON_GOAL_02_ZNM);
#endif
		}
	}
}



// =======================================================================
// dmStfrlMdlCtrlSonicProcWaitChngDash2
/*!
  ソニック 生成完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlSonicProcWaitChngDash2(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_SONIC_WORK *sonic_work	= (DMS_STFRL_SONIC_WORK *)obj_work;
	GMS_EFFECT_3DES_WORK	*efct_work;
	
	sonic_work->timer++;
	
	// モーションFWのループ設定
	if (obj_work->disp_flag & OBD_DISP_END) {
		
		if (sonic_work->timer > 30) {
			// ブレンド速度設定
			obj_work->obj_3d->blend_spd	= 0.125f;// TODO : 仮
			
			// ゴール時のモーションを設定
			ObjDrawObjectActionSet3DNNBlend(obj_work
											, IDB_SON_MTN_SON_DASH2_ZNM
											);
			
#if _WII
			// 手ロッドアクション設定
			dmStfrlSetLodAction(sonic_work, IDB_SON_MTN_SON_DASH2_ZNM);
#endif
			// 右
			efct_work = GmEfctCmnEsCreate(obj_work, GME_EFCT_CMN_IDX_ROLLDASH_R);
			// 位置調整
			GmComEfctSetDispOffsetF(efct_work, -1.5f, 0.f, 9.f);
#if _IPHONE
			efct_work->obj_3des.ecb->drawObjState = OBD_DRAW_CMD_STATE_3DNN; // 描画コマンドを通常へ
#endif // _IPHONE
			
			// 汎用処理
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
			
			// メイン処理設定
			obj_work->ppFunc	= dmStfrlMdlCtrlSonicProcWaitMtnEnd;
			
			sonic_work->timer = 0;
		}
		
		else {
			// ゴール時のモーションを設定
			ObjDrawObjectActionSet3DNNBlend(obj_work
											, IDB_SON_MTN_SON_WALK_ZNM
											);
			
#if _WII
			// 手ロッドアクション設定
			dmStfrlSetLodAction(sonic_work, IDB_SON_MTN_SON_WALK_ZNM);
#endif
		}
	}
}



// =======================================================================
// dmStfrlMdlCtrlSonicProcWaitMtnEnd
/*!
  ソニック モーション終了待ち処理
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlSonicProcWaitMtnEnd(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_SONIC_WORK *sonic_work	= (DMS_STFRL_SONIC_WORK *)obj_work;
	
	sonic_work->timer++;
	
	if (obj_work->spd_m <= 0x64cc - 0x200) {
		obj_work->spd_m += 0x200;
	}
	
	//
	if (obj_work->disp_flag & OBD_DISP_END) {
		
		// ゴール時のモーションを再設定
		ObjDrawObjectActionSet3DNNBlend(obj_work
										, IDB_SON_MTN_SON_DASH2_ZNM
										);
		
#if _WII
		// 手ロッドアクション設定
		dmStfrlSetLodAction(sonic_work, IDB_SON_MTN_SON_DASH2_ZNM);
#endif
		// 汎用処理
		GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
		
		if (sonic_work->timer > DMD_STFRL_SONIC_FADE_START_TIME) {
			// メイン処理設定(走りぬけモーションへ)
			obj_work->ppFunc	= dmStfrlMdlCtrlSonicProcWaitFadeEnd;
			
			// ダッシュモーションを再設定
			ObjDrawObjectActionSet3DNNBlend(obj_work
											, IDB_SON_MTN_SON_DASH2_ZNM
											);
			
#if _WII
			// 手ロッドアクション設定
			dmStfrlSetLodAction(sonic_work, IDB_SON_MTN_SON_DASH2_ZNM);
#endif
			// 汎用処理
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
			
			sonic_work->timer = DMD_STFRL_SONIC_FADE_END_TIME;
			
//			sonic_work->alpha_spd = (float)(1.f / DMD_STFRL_SONIC_FADE_END_TIME);
		}
	}
}



// =======================================================================
// dmStfrlMdlCtrlSonicProcWaitFadeEnd
/*!
  ソニック モーション終了待ち処理
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlSonicProcWaitFadeEnd(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_SONIC_WORK *sonic_work	= (DMS_STFRL_SONIC_WORK *)obj_work;
	
	sonic_work->timer--;
	
	obj_work->pos.x += 0x12000;
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		if (sonic_work->timer <= 0) {
			// 非表示になっていくプロシージャに切り替え
			obj_work->ppFunc	= dmStfrlMdlCtrlSonicProcIdle;
			
			sonic_work->timer = 0;
			
			sonic_work->flag |= DMD_STFRL_SONIC_FLAG_EFCT_END;
			
			obj_work->disp_flag |= OBD_DISP_NODISP;
		}
		
		else {
			// ダッシュモーションを再設定
			ObjDrawObjectActionSet3DNNBlend(obj_work
											, IDB_SON_MTN_SON_DASH2_ZNM
											);
			
#if _WII
			// 手ロッドアクション設定
			dmStfrlSetLodAction(sonic_work, IDB_SON_MTN_SON_DASH2_ZNM);
#endif
			
			// 汎用処理
			GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
		}
	}
	
	
	if (sonic_work->alpha <= 0.f) {
//		sonic_work->alpha = 0.f;
	}
	else {
//		sonic_work->alpha = sonic_work->alpha_spd * sonic_work->timer;
	}
}



// =======================================================================
// dmStfrlMdlCtrlSonicProcIdle
/*!
  ソニック 待機処理(事実上、処理なし)
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlSonicProcIdle(OBS_OBJECT_WORK *obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);
}



// ==========================================================================
// dmStfrlMdlCtrlSonicDrawFunc
/*!
 *	スタッフロール用ソニック描画関数
 */
// ==========================================================================
void dmStfrlMdlCtrlSonicDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_SONIC_WORK *sonic_work	= (DMS_STFRL_SONIC_WORK *)obj_work;
	
	obj_work->obj_3d->draw_state.alpha.alpha = sonic_work->alpha;
	
	// 表示用の設定が済んだので表示させる
	ObjDrawActionSummary(obj_work);
}




// ------------------------------- BOSS関連 ------------------------------------

// =======================================================================
// dmStfrlMdlCtrlBoss1BodyExit
/*!
  本体終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void dmStfrlMdlCtrlBoss1BodyExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	DMS_STFRL_BOSS_BODY_WORK	*body_work	= (DMS_STFRL_BOSS_BODY_WORK*)obj_work;
	
	// ボスモーションコールバックシステム解除・クリア
	GmBsCmnClearBossMotionCBSystem(obj_work);
	
	// ノードマトリクス取得関連解放
	GmBsCmnDeleteSNMWork(&body_work->snm_work);
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}



// =======================================================================
// dmStfrlMdlCtrlBodyProcWaitSetup
/*!
  本体 生成完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlBodyProcWaitSetup(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_BOSS_BODY_WORK *body_work	= (DMS_STFRL_BOSS_BODY_WORK *)obj_work;
	
	// BMCBシステム初期化
	GmBsCmnInitBossMotionCBSystem(obj_work,
								  &body_work->bmcb_mgr);
	
	// ノードマトリクス取得初期化
	GmBsCmnCreateSNMWork(&body_work->snm_work
						 , obj_work->obj_3d->object
						 , 1
						 );
	
	// モーションコールバックを実行リストに追加
	GmBsCmnAppendBossMotionCallback(&body_work->bmcb_mgr
									, &body_work->snm_work.bmcb_link
									);
	
	
	// ノードマトリクス取得ノード追加
	body_work->egg_snm_reg_id	= GmBsCmnRegisterSNMNode(&body_work->snm_work
														 , DMD_STFRL_BOSS_BODY_NODE_IDX_EGG_CONNECT
														 );
	
	
	if (body_work->flag & DMD_STFRL_BODY_FLAG_COMPLETE_EFCT) {
		body_work->timer = 0;
		obj_work->ppFunc	= dmStfrlMdlCtrlBodyProcBodyCompInitStart;
	}
	else {
		body_work->timer = DMD_STFRL_BOSS_NOCOMP_EFCT_START_TIME;
		obj_work->ppFunc	= dmStfrlMdlCtrlBodyProcBodyMain;
	}
}



// =======================================================================
// dmStfrlMdlCtrlBodyProcBodyMain
/*!
  本体 メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlBodyProcBodyMain(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_BOSS_BODY_WORK *body_work	= (DMS_STFRL_BOSS_BODY_WORK *)obj_work;
	
	// ステート遷移が発生していないときだけ更新処理を行う
	
	// 初期位置設定
	obj_work->pos.x = DMD_STFRL_BOSS_INIT_POS_X;
	obj_work->pos.y = DMD_STFRL_BOSS_INIT_POS_Y;
	
	// Z位置設定
	obj_work->pos.z	= DMD_STFRL_BOSS_INIT_POS_Z;
	
	// 表示角度
	obj_work->dir.y = (u16)AKM_DEGtoA16(DMD_STFRL_BOSS_INIT_DISP_DIR);
	
	
	if (body_work->timer != 0) {
		// タイマー更新
		body_work->timer--;
	}
	else {
		body_work->flag |= DMS_STFRL_FLAG_CHNG_MTN_EGG_LAUGH_REQ;
	}
	
}



// =======================================================================
// dmStfrlMdlCtrlBodyProcBodyCompInitStart
/*!
  本体 演出開始初期化関数(コンプリート版)
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlBodyProcBodyCompInitStart(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_BOSS_BODY_WORK *body_work	= (DMS_STFRL_BOSS_BODY_WORK *)obj_work;
	
	// ステート遷移が発生していないときだけ更新処理を行う
	
	// 初期位置設定
	obj_work->pos.x = 0;
	obj_work->pos.y = DMD_STFRL_BOSS_NODISP_HEIGHT_POS_Y;
	
	// Z位置設定
	obj_work->pos.z	= DMD_STFRL_BOSS_INIT_POS_Z;
	
	// 表示角度
	obj_work->dir.y = (u16)AKM_DEGtoA16(DMD_STFRL_BOSS_INIT_DISP_DIR);
	
	// 移動開始フラグONならば
	if (body_work->flag & DMD_STFRL_BODY_FLAG_MOVE_DOWN_START) {
		
		// 下降演出プロシージャへ
		obj_work->ppFunc	= dmStfrlMdlCtrlBodyProcBodyCompStartWait;
		
		// フラグOFF
		body_work->flag &= ~DMD_STFRL_BODY_FLAG_MOVE_DOWN_START;
	}
}



// =======================================================================
// dmStfrlMdlCtrlBodyProcBodyCompStartWait
/*!
  本体 演出開始待ち関数(コンプリート版)
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlBodyProcBodyCompStartWait(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_BOSS_BODY_WORK *body_work	= (DMS_STFRL_BOSS_BODY_WORK *)obj_work;
	
	// 移動開始待ち用のタイマー更新
	body_work->timer++;
	
	// 移動開始フラグONならば
	if (body_work->timer > DMD_STFRL_BOSS_COMP_EFCT_START_TIME) {
		
		// 下降演出プロシージャへ
		obj_work->ppFunc	= dmStfrlMdlCtrlBodyProcBodyCompMoveDown;
		
		// タイマー初期化
		body_work->timer = 0;
	}
}




// =======================================================================
// dmStfrlMdlCtrlBodyProcBodyCompMoveDown
/*!
  本体 下降演出中処理関数(コンプリート版)
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlBodyProcBodyCompMoveDown(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_BOSS_BODY_WORK *body_work	= (DMS_STFRL_BOSS_BODY_WORK *)obj_work;
	
	// 座標更新処理
	obj_work->pos.y = (fx32)(obj_work->pos.y + DMD_STFRL_BOSS_MOVE_DOWN_SPD);
	
	// 座標再設定処理
	
	
	// 下降先まで辿り着いたら
	if (obj_work->pos.y >= DMD_STFRL_BOSS_COMP_DISP_HEIGHT_POS_Y) {
		// 表示位置再設定(保険)
		obj_work->pos.y = DMD_STFRL_BOSS_COMP_DISP_HEIGHT_POS_Y;
		
		// エッグマン笑い演出開始フラグON
		body_work->flag |= DMS_STFRL_FLAG_CHNG_MTN_EGG_LAUGH_REQ;
		
		// 笑い演出中プロシージャへ切り替え
		obj_work->ppFunc	= dmStfrlMdlCtrlBodyProcBodyCompLaughWait;
	}
	
}



// =======================================================================
// dmStfrlMdlCtrlBodyProcBodyCompLaughWait
/*!
  本体 エッグマン笑い演出終了待ち中処理関数(コンプリート版)
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlBodyProcBodyCompLaughWait(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_BOSS_BODY_WORK *body_work	= (DMS_STFRL_BOSS_BODY_WORK *)obj_work;
	
	
	body_work->timer++;
	
	// 演出終了フラグONならば
	if (body_work->timer >= DMD_STFRL_BOSS_EGG_LAUGH_TIME) {
//	if (body_work->flag & DMD_STFRL_BODY_FLAG_MOVE_UP_START) {
		// タイマー初期化
		body_work->timer = 0;
		
		// 上昇演出へ
		obj_work->ppFunc	= dmStfrlMdlCtrlBodyProcBodyCompMoveUpWait;
		
		// フラグOFF
		body_work->flag &= ~DMD_STFRL_BODY_FLAG_MOVE_UP_START;
	}
	
}



// =======================================================================
// dmStfrlMdlCtrlBodyProcBodyCompMoveUpWait
/*!
  本体 上昇演出終了待ち中処理関数(コンプリート版)
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlBodyProcBodyCompMoveUpWait(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_BOSS_BODY_WORK *body_work	= (DMS_STFRL_BOSS_BODY_WORK *)obj_work;
	
	// 座標更新処理(上昇)
	obj_work->pos.y = (fx32)(obj_work->pos.y + DMD_STFRL_BOSS_MOVE_UP_SPD);
	
	// 座標再設定処理
	if (body_work->timer > DMD_STFRL_BOSS_M_SONIC_EFCT_TIME) {
		// メタルソニック演出開始フラグON
		body_work->flag |= DMD_STFRL_BODY_FLAG_M_SONIC_EFCT_START;
	}
	else {
		body_work->timer++;
	}
	
	// 上昇先まで辿り着いたら
	if (obj_work->pos.y <= DMD_STFRL_BOSS_NODISP_HEIGHT_POS_Y) {
		// 表示位置再設定(保険)
		obj_work->pos.y = DMD_STFRL_BOSS_NODISP_HEIGHT_POS_Y;
		
		// END演出全終了フラグON
		body_work->flag |= DMD_STFRL_BODY_FLAG_COMP_EFCT_END;
		
		// END画面終了までIDLEに入る
		obj_work->ppFunc	= dmStfrlMdlCtrlBodyProcBodyCompEndWaitIdle;
		
		// タイマー初期化
		body_work->timer = 0;
	}
}



// =======================================================================
// dmStfrlMdlCtrlBodyProcBodyCompEndWaitIdle
/*!
  END演出終了待ち中処理関数(コンプリート版)
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlBodyProcBodyCompEndWaitIdle(OBS_OBJECT_WORK *obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);
	
	// 処理なし
}



// =======================================================================
// dmStfrlMdlCtrlEggProcWaitSetup
/*!
  エッグマン メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlEggProcWaitSetup(OBS_OBJECT_WORK *obj_work)
{
//	DMS_STFRL_BOSS_BODY_WORK *body_work = (DMS_STFRL_BOSS_BODY_WORK *)obj_work->parent_obj;
//	DMS_STFRL_BOSS_EGG_WORK	*egg_work = (DMS_STFRL_BOSS_EGG_WORK *)obj_work;
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= 0.025f;
	
	obj_work->ppFunc	= dmStfrlMdlCtrlEggProcMain;
}



// =======================================================================
// dmStfrlMdlCtrlEggProcMain
/*!
  エッグマン メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlEggProcMain(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_BOSS_BODY_WORK *body_work = (DMS_STFRL_BOSS_BODY_WORK *)obj_work->parent_obj;
	DMS_STFRL_BOSS_EGG_WORK	*egg_work = (DMS_STFRL_BOSS_EGG_WORK *)obj_work;
	
	// エッグマン設置ノードにくっつける
	GmBsCmnUpdateObject3DNNStuckWithNode(obj_work,
										 &body_work->snm_work,
										 body_work->egg_snm_reg_id,
										 TRUE);
	
	// 笑いシーケンス切り替え処理
	if (body_work->flag & DMS_STFRL_FLAG_CHNG_MTN_EGG_LAUGH_REQ) {
		
		if (!(egg_work->flag & DMS_STFRL_EGG_FLAG_CHNG_MTN_EGG_LAUGH)) {
			ObjDrawObjectActionSet3DNNBlend(obj_work
											, IDB_BOSS03_EGG_MTN_B03_1_STA_01E_ZNM
											);
			
			obj_work->obj_3d->frame[0] = 0.f;
			
			// ブレンド速度設定
			obj_work->obj_3d->blend_spd	= 0.025f;
			
			egg_work->flag |= DMS_STFRL_EGG_FLAG_CHNG_MTN_EGG_LAUGH;
			
			obj_work->ppFunc = dmStfrlMdlCtrlEggProcMainIdle;
		}
		
	}
}


// =======================================================================
// dmStfrlMdlCtrlEggProcMainIdle
/*!
  エッグマン メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void dmStfrlMdlCtrlEggProcMainIdle(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_BOSS_BODY_WORK *body_work = (DMS_STFRL_BOSS_BODY_WORK *)obj_work->parent_obj;
//	DMS_STFRL_BOSS_EGG_WORK	*egg_work = (DMS_STFRL_BOSS_EGG_WORK *)obj_work;
	
	// エッグマン設置ノードにくっつける
	GmBsCmnUpdateObject3DNNStuckWithNode(obj_work,
										 &body_work->snm_work,
										 body_work->egg_snm_reg_id,
										 TRUE);
	
	// 笑いシーケンス切り替え処理
	if (obj_work->disp_flag & OBD_DISP_END) {
//	if (obj_work->obj_3d->frame[0] >= 160.f) {
		
		ObjDrawObjectActionSet3DNNBlend(obj_work
										, IDB_BOSS03_EGG_MTN_B03_1_STA_01E_ZNM
										);
		
		obj_work->obj_3d->frame[0] = 0.f;
		
		// ブレンド速度設定
		obj_work->obj_3d->blend_spd	= 0.125f;
	}
	
	
}


// ---------------------------- リング関連 ---------------------------------

// ==========================================================================
// dmStfrlMdlCtrlRingProcStartWait
/*!
 *	リング演出開始待ちプロシージャ
 */
// ==========================================================================
void dmStfrlMdlCtrlRingProcStartWait(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_RING_WORK *ring_work = NULL;
	
	ring_work = (DMS_STFRL_RING_WORK *)obj_work;
	
	// 演出開始フラグONならば
	if (ring_work->flag & DMD_STFRL_RING_FLAG_SPLASH_EFCT_START) {
		ring_work->timer++;
		
		if (ring_work->timer >= ring_work->efct_start_time) {
			// 演出開始プロシージャへ
			obj_work->ppFunc = dmStfrlMdlCtrlRingProcInitSetup;
			
			
			dmStfrlMdlCtrlCreateRingEfct(ring_work->start_pos.x
										 	+ dm_stfrl_ring_efct_disp_offset_tbl[ring_work->disp_efct_pos_no][0]
									  , ring_work->start_pos.y
										 	+ dm_stfrl_ring_efct_disp_offset_tbl[ring_work->disp_efct_pos_no][1]
									  );
			
			dmStfrlMdlCtrlCreateRingEfct(ring_work->start_pos.x
										 	+ dm_stfrl_ring_efct_disp_offset_tbl[ring_work->disp_efct_pos_no+1][0]
									  , ring_work->start_pos.y
										 	+ dm_stfrl_ring_efct_disp_offset_tbl[ring_work->disp_efct_pos_no+1][1]
									  );
			
			dmStfrlMdlCtrlCreateRingEfct(ring_work->start_pos.x
										 	+ dm_stfrl_ring_efct_disp_offset_tbl[ring_work->disp_efct_pos_no+2][0]
									  , ring_work->start_pos.y
										 	+ dm_stfrl_ring_efct_disp_offset_tbl[ring_work->disp_efct_pos_no+2][1]
									  );
			
			ring_work->timer = 0;
			
			// フラグOFF
			ring_work->flag &= ~DMD_STFRL_RING_FLAG_SPLASH_EFCT_START;
		}
	}
}



// ==========================================================================
// dmStfrlMdlCtrlRingProcInitSetup
/*!
 *	リング初期化設定プロシージャ
 */
// ==========================================================================
void dmStfrlMdlCtrlRingProcInitSetup(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_RING_WORK *ring_work = NULL;
	u16 tmp_next_dir = 0;
	u16 tmp_start_dir = 0;
	
	ring_work = (DMS_STFRL_RING_WORK *)obj_work;
	
	// 初期状態は非表示にする
	obj_work->obj_3d->draw_state.alpha.alpha = 0.f;
	
	// 次のリングとの角度距離を求める
	tmp_next_dir = DMD_STFRL_ONE_AROUND_DIR / DMD_STFRL_RING_EFCT_DISP_NUM;
	
	// +X方向を0度としたところから一つ目の発射角度を求める
	tmp_start_dir = 0;//tmp_next_dir / 2;
	
	for (int i = 0; i < DMD_STFRL_RING_EFCT_DISP_NUM; i++) {
		// 初期表示位置設定
		ring_work->pos[i].x = ring_work->start_pos.x;
		ring_work->pos[i].y = ring_work->start_pos.y;
		ring_work->pos[i].z = -3;
		
		// 演出移動速度設定
		ring_work->spd_x[i] = mtMathSin((u16)(tmp_start_dir + i * tmp_next_dir));
		ring_work->spd_y[i] = mtMathCos((u16)(tmp_start_dir + i * tmp_next_dir));
		
		ring_work->spd_y[i] += 0x200;
//		ring_work->spd_x[i] /= 0x2;
//		ring_work->spd_y[i] /= 0x2;
	}
	
	// 非表示から表示状態へ切り替わる際の速度を設定
	ring_work->alpha_spd = 1.f / DMD_STFRL_RING_DISP_TIME;
	
	// メイン処理設定
	obj_work->ppFunc	= dmStfrlMdlCtrlRingProcDispIdle;
}



// ==========================================================================
// dmStfrlMdlCtrlRingProcDispIdle
/*!
 *	リング初期化設定プロシージャ
 */
// ==========================================================================
void dmStfrlMdlCtrlRingProcDispIdle(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_RING_WORK *ring_work = NULL;
	
	
	ring_work = (DMS_STFRL_RING_WORK *)obj_work;
	
	for (int i = 0; i < DMD_STFRL_RING_EFCT_DISP_NUM; i++) {
//		ring_work->spd_x[i] -= 0x20;
		ring_work->spd_y[i] += 0x40;
	}
	
	
	ring_work->timer++;
	
	if (ring_work->timer >= DMD_STFRL_RING_DISP_TIME) {
		
		// 非表示になっていくプロシージャに切り替え
		obj_work->ppFunc	= dmStfrlMdlCtrlRingProcNoDispIdle;
		
		ring_work->timer = DMD_STFRL_RING_NODISP_TIME;
		
		ring_work->alpha_spd = 1.f / DMD_STFRL_RING_NODISP_TIME;
	}
	
	if (ring_work->alpha >= 1.f) {
		ring_work->alpha = 1.f;
	}
	else {
		ring_work->alpha = ring_work->alpha_spd * ring_work->timer;
	}
	
	
}



// ==========================================================================
// dmStfrlMdlCtrlRingProcNoDispIdle
/*!
 *	リング初期化設定プロシージャ
 */
// ==========================================================================
void dmStfrlMdlCtrlRingProcNoDispIdle(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_RING_WORK *ring_work = NULL;
	
	ring_work = (DMS_STFRL_RING_WORK *)obj_work;
	
	for (int i = 0; i < DMD_STFRL_RING_EFCT_DISP_NUM; i++) {
//		ring_work->spd_x[i] -= 0x20;
		ring_work->spd_y[i] += 0x40;
	}
	
	
	ring_work->timer--;
	
	if (ring_work->timer <= 0) {
		
		// 非表示になっていくプロシージャに切り替え
		obj_work->ppFunc	= dmStfrlMdlCtrlRingProcStartWait;
		
		ring_work->timer = 0;
		
		ring_work->disp_ring_pos_no++;
		
		if (ring_work->disp_ring_pos_no > 12) {
			ring_work->disp_ring_pos_no = 0;
		}
		
		ring_work->disp_efct_pos_no++;
		
		if (ring_work->disp_efct_pos_no > 12) {
			ring_work->disp_efct_pos_no = 0;
		}
		
		ring_work->start_pos.x = dm_stfrl_ring_disp_pos_tbl[ring_work->disp_ring_pos_no][0];
		ring_work->start_pos.y = dm_stfrl_ring_disp_pos_tbl[ring_work->disp_ring_pos_no][1];
	}
	
	
	if (ring_work->alpha <= 0.f) {
		ring_work->alpha = 0.f;
	}
	else {
		ring_work->alpha = ring_work->alpha_spd * ring_work->timer;
	}
}



// ==========================================================================
// dmStfrlMdlCtrlRingDrawFunc
/*!
 *	3Dリング描画関数
 */
// ==========================================================================
void dmStfrlMdlCtrlRingDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	DMS_STFRL_RING_WORK *ring_work = NULL;
	
	ring_work = (DMS_STFRL_RING_WORK *)obj_work;
	
	obj_work->dir.y += DMD_STFRL_RING_ROTATE_SPD;
	
	obj_work->obj_3d->draw_state.alpha.alpha = ring_work->alpha;
	
	
	for (int i = 0; i < DMD_STFRL_RING_EFCT_DISP_NUM; i++) {
		
		// 各リングの移動方向へ移動
		ring_work->pos[i].x += ring_work->spd_x[i];
		ring_work->pos[i].y += ring_work->spd_y[i];
		
		// 移動後の表示位置を表示用OBJに設定
		obj_work->pos.x = ring_work->pos[i].x;
		obj_work->pos.y = ring_work->pos[i].y;
		obj_work->pos.z = ring_work->pos[i].z;
		if (0 == i) {
			obj_work->disp_flag &= ~OBD_DISP_STOP;
		} else {
			obj_work->disp_flag |= OBD_DISP_STOP;
		}
		
		// 表示用の設定が済んだので表示させる
		ObjDrawActionSummary(obj_work);
	}
}


// ==========================================================================
// dmStfrlMdlCtrlCreateRingEfct
/*!
 *	リングエフェクト作成
 */
// ==========================================================================
void dmStfrlMdlCtrlCreateRingEfct(fx32 pos_x, fx32 pos_y)
{
	GMS_EFFECT_3DES_WORK	*efct_work;

	efct_work = GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_RING);

	// 位置調整
	efct_work->efct_com.obj_work.pos.x = pos_x;
	efct_work->efct_com.obj_work.pos.y = pos_y;
	efct_work->efct_com.obj_work.pos.z = -3;
}






// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
