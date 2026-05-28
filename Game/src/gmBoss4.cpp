// =======================================================================
/*!
  @file	gmBoss4.cpp
  @brief ボス4

  @author K.Yurita(SafariGames)
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "gs.h"
#include "gmMain.h"
#include "gmGameDBuild.h"
#include "gmGamedat.h"
#include "gmEventTbl.h"
#include "gmCamera.h"
#include "gmSound.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectBoss.h"
#include "gmBossCommon.h"
#include "gmDeco.h"

#include "gmBoss4.h"
#include "gmBoss4Body.h"
#include "gmBoss4Capsule.h"
#include "gmBoss4Eggman.h"
#include "gmBoss4Effect.h"
#include "gmBoss4Chibi.h"
#include "gmBoss4Util.h"

#include "gmMap.h"				//<死んだときにカメラを初期化するため

#include "gmPlySeq.h"

#include "gmGmkCamScrLim.h"

#include "gmPauseMenu.h"

#if	defined(_WII) && !defined(_PS3)
#pragma warning off (10178)			// 型宣言のプロトタイプのワーニングをOFFにする
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
									// ただしstatic宣言はつけれないため、問題がある場合は直接修正する
#endif	//_WII

//----------------------------------------------------------------------
// データヘッダ
//	TODO : 最終的にBOSS4のデータに変更
//----------------------------------------------------------------------
#include "../file/common/arc/BOSS04.hmb"
#include "../file/common/model/BOSS04_MDL.hmb"
#include "../file/common/model/BOSS04_BODY_MTN.hmb"
#include "../file/common/model/BOSS04_EGG_MTN.hmb"
#include "../file/common/model/BOSS04_CAPSULE_MTN.hmb"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/
// =======================================================================
// GMM_BOSS4_MGR
/*!
  ボス1本体ワークから管理ワークを取り出す
  
  @param work	[in]	ボス１本体ワーク
  
  @return 管理ワーク(GMS_BOSS4_MGR_WORK)
 */
// =======================================================================
#define GMM_BOSS4_MGR(work)	((work)->mgr_work)


/*------ Definitions ---------------------------------------------------*/
#if defined (MTD_DEBUG)
#define GMM_BOSS4_CREATE_WORK(eve_rec, pos_x, pos_y, work_size, name)	\
	(GmEnemyCreateWork(eve_rec, pos_x, pos_y, work_size, GMD_TASK_PRIO_GIMMICK_R, name))
#else //(MTD_DEBUG)
#define GMM_BOSS4_CREATE_WORK(eve_rec, pos_x, pos_y, work_size, name)	\
	(GmEnemyCreateWork(eve_rec, pos_x, pos_y, work_size, GMD_TASK_PRIO_GIMMICK_R))
#endif //(MTD_DEBUG)

// スクロール状態
enum	GME_BOSS4_SCROLL_TYPE{
	GME_BOSS4_SCROLL_TYPE_NORMAL	= 0,	// 通常
	GME_BOSS4_SCROLL_TYPE_ADDSPEED,			// スピード加速 
	GME_BOSS4_SCROLL_TYPE_SUBSPEED,			// スピード減速 

	GME_BOSS4_SCROLL_TYPE_MAX
};

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
//############ ボス１管理 #####################################################
/* 制御処理 */
static void gmBoss4MgrWaitLoad(OBS_OBJECT_WORK *obj_work);
static void gmBoss4MgrMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss4MgrWaitRelease(OBS_OBJECT_WORK *obj_work);

//static void gmBoss4ScrollFunc(MTS_TASK_TCB *none);
//static void gmBoss4ScrollExit(MTS_TASK_TCB *none);

void GmBoss4ScrollOff();

#if defined(MTD_DEBUG)
//############ デバッグ ###############################################
#endif /* defined(MTD_DEBUG) */


/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! ボス全体アクションIDテーブル
const static GMS_BOSS4_PART_ACT_INFO gm_boss4_act_id_tbl[GME_BOSS4_ACT_ID_MAX][GME_BOSS4_PART_IDX_MAX]	= {
	// ACT_ID											IS_MAINTAIN		IS_REPEAT	MTN_SPD	IS_BLEND	BLEND_SPD
	// スタート時の降りてくるときに使用
	// GME_BOSS4_ACT_ID_APP_FALL
	{
		{IDB_BOSS04_BODY_MTN_B04_1_ATT01_01B_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_1_ATT01_01E_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// 未使用
	// GME_BOSS4_ACT_ID_APP_END	（※GENESIS版準拠の挙動になったので現在未使用）
	{
		{IDB_BOSS04_BODY_MTN_B04_1_ATT01_01B_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_1_ATT01_01E_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// 通常移動(一番最初の動き)
	//GME_BOSS4_ACT_ID_PRE_ATK_NML_MOVE
	{
		{IDB_BOSS04_BODY_MTN_B04_1_ATT01_01B_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_1_ATT01_01E_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// 通常移動
	// GME_BOSS4_ACT_ID_ATK_NML_MOVE
	{
		{IDB_BOSS04_BODY_MTN_B04_1_ATT01_01B_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_1_ATT01_01E_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},

	// 第２形態瞬間変化(Final用)
	// GME_BOSS4_ACT_ID_START_2
	{
		{IDB_BOSS04_BODY_MTN_B04_2_STA_01B_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		1.0f/*GMD_BOSS4_DEFAULT_BLEND_SPD*/,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_2_STA_01E_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		1.0f/*GMD_BOSS4_DEFAULT_BLEND_SPD*/,	FALSE},		// EGG
	},

	// 通常移動(第２形態)
	// GME_BOSS4_ACT_ID_ATK_NML_MOVE2
	{
		{IDB_BOSS04_BODY_MTN_B04_2_ATT01_01B_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		1.0f/*GMD_BOSS4_DEFAULT_BLEND_SPD*/,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_2_ATT01_01E_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		1.0f/*GMD_BOSS4_DEFAULT_BLEND_SPD*/,	FALSE},		// EGG
	},

	// TODO : ↓不要？
	// GME_BOSS4_ACT_ID_DAMAGE_NML
	{
		{IDB_BOSS04_BODY_MTN_B04_DMG01_01B_ZNM,			TRUE,			FALSE,		1.f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_DMG01_01E_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS4_ACT_ID_ESCAPE
	{
		{IDB_BOSS04_BODY_MTN_B04_DMG02_01B_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_DMG02_01E_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},

	// GME_BOSS4_ACT_ID_ANGRY_L1
	{
		{IDB_BOSS04_BODY_MTN_B04_1_ANG_01B_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_1_ANG_01E_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	// GME_BOSS4_ACT_ID_ANGRY_L2
	{
		{IDB_BOSS04_BODY_MTN_B04_1_ANG_02B_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_1_ANG_02E_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	// GME_BOSS4_ACT_ID_ANGRY_L3
	{
		{IDB_BOSS04_BODY_MTN_B04_1_ANG_03B_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS04_EGG_MTN_B04_1_ANG_03E_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
};


// 構築済みモデル(obj_3d)格納リスト
static	OBS_ACTION3D_NN_WORK *gm_boss4_obj_3d_list			= NULL;

// ひとつしか必要ないため、安全のためグローバルにもつ
static	GMS_BOSS4_MGR_WORK*		gm_boss4_mgr_work			= NULL;

// 強制スクロールフラグ
static	GME_BOSS4_SCROLL_TYPE	gm_boss4_n_scroll			= GME_BOSS4_SCROLL_TYPE_NORMAL;	//!< スクロールモード
static	Sint32					gm_boss4_n_offset_x			= 0;							//!< 今回のズレ
static	Float					gm_boss4_f_scroll_spd		= 0.0f;							//!< 現在のスピード
static	Float					gm_boss4_f_scroll_spd_max	= GMD_BOSS4_SCROLL_SPD_MAX;		//!< 最高速
static	Sint32					gm_boss4_n_scroll_pt_x		= 0;							//!< 現在のスクロール位置
static	Sint32					gm_boss4_n_scroll_start		= GMD_BOSS4_SCROLL_START_X;		//!< スクロール範囲
static	Sint32					gm_boss4_n_scroll_end		= GMD_BOSS4_SCROLL_END_X;		//!< スクロール範囲
static	BOOL					gm_boss4_b_warpout			= FALSE;
static	BOOL					gm_boss4_is_2nd				= FALSE;
/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// GmBoss4SetLife()
/*!
  ボス4 ライフ設定
 */
// =======================================================================
void GmBoss4SetLife( Sint32 life )
{
	MTM_ASSERT( gm_boss4_mgr_work );

	gm_boss4_mgr_work->life = life;
}

// =======================================================================
// GmBoss4SetLife()
/*!
  ボス4 ライフ取得
 */
// =======================================================================
Sint32 GmBoss4GetLife()
{
	MTM_ASSERT( gm_boss4_mgr_work );

	if (gm_boss4_mgr_work == NULL){
		return 0;
	}

	return gm_boss4_mgr_work->life;	
}

// =======================================================================
// GmBoss4SetLife()
/*!
  ボス4 ボス本体の取得
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4GetBodyWork()
{
	MTM_ASSERT( gm_boss4_mgr_work );

	if (gm_boss4_mgr_work == NULL){
		return NULL;
	}

	return (OBS_OBJECT_WORK*)gm_boss4_mgr_work->body_work;	
}

// =======================================================================
// GmBoss4GetObj3D()
/*!
  ボス4 obj_3dモデル(OBS_ACTION3D_NN_WORK)の取得
 */
// =======================================================================
OBS_ACTION3D_NN_WORK*	GmBoss4GetObj3D(Sint32 n)
{

#ifdef	_DEBUG
	AMS_AMB_HEADER	*mdl_amb;
	mdl_amb	= (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(NULL, IDB_BOSS04_BOSS04_MDL_AMB, GMD_BOSS4_ARC);

	MTM_ASSERT( mdl_amb!=NULL ); 
	MTM_ASSERT( mdl_amb->file_num > n );
#endif

	return &gm_boss4_obj_3d_list[n];
}

// =======================================================================
// GmBoss4GetActInfo()
/*!
  ボス4パーツアクション情報の取得
 */
// =======================================================================
const GMS_BOSS4_PART_ACT_INFO*	GmBoss4GetActInfo(Sint32 act, Sint32 parts)
{
	return &gm_boss4_act_id_tbl[act][parts];
}


// =======================================================================
// GmBoss4Build
/*!
  ボス4 データ構築
 */
// =======================================================================
void GmBoss4Build(void)
{
	void	*mdl_amb;
	void	*tex_amb;
	mdl_amb	= ObjDataLoadAmbIndex(NULL, IDB_BOSS04_BOSS04_MDL_AMB, GMD_BOSS4_ARC);
	tex_amb	= ObjDataLoadAmbIndex(NULL, IDB_BOSS04_BOSS04_TEX_AMB, GMD_BOSS4_ARC);
	
	// モデル構築
	gm_boss4_obj_3d_list	=
		GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)mdl_amb, (AMS_AMB_HEADER*)tex_amb,
								  NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);

	GmBoss4BodyBuild();
	GmBoss4EggmanBuild();
	GmBoss4CapsuleBuild();
	GmBoss4ChibiBuild();
	GmBoss4EffectBuild();
}


// =======================================================================
// GmBoss4Flush
/*!
  ボス１ データ片付け
 */
// =======================================================================
void GmBoss4Flush(void)
{
	AMS_AMB_HEADER	*mdl_amb;

	GmBoss4EffectFlush();
	GmBoss4ChibiFlush();
	GmBoss4CapsuleFlush();
	GmBoss4EggmanFlush();
	GmBoss4BodyFlush();

	// モデル解放
	mdl_amb	= (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(NULL, IDB_BOSS04_BOSS04_MDL_AMB, GMD_BOSS4_ARC);
	
	GmGameDBuildRegFlushModel(gm_boss4_obj_3d_list, mdl_amb->file_num);
	
	gm_boss4_obj_3d_list	= NULL;
	gm_boss4_mgr_work		= NULL;
}


// =======================================================================
// GmBoss4IsBuildeded()
/*!
  ボス4 データ構築終了チェック
 */
// =======================================================================
BOOL GmBoss4IsBuilded(void)
{
	MTM_ASSERT( gm_boss4_mgr_work );

	return (BOOL)(gm_boss4_mgr_work->flag & GMD_BOSS4_MGR_FLAG_LOAD_END);
}


// =======================================================================
// GmBoss4Init
/*!
  ボス１（管理）初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4Init(GMS_EVE_RECORD_EVENT *eve_rec,
							 fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);

	OBS_OBJECT_WORK	*obj_mgr;
	GMS_BOSS4_MGR_WORK	*mgr_work;

	// オブジェクト作成
	obj_mgr		= GMM_BOSS4_CREATE_WORK(	eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS4_MGR_WORK),
										"Boss4_MGR");
	
	mgr_work	= (GMS_BOSS4_MGR_WORK*)obj_mgr;

	gm_boss4_mgr_work		= mgr_work;

	// ワーク設定
	obj_mgr->flag		|= OBD_OBJECT_NOCLIP;
	obj_mgr->disp_flag	|= OBD_DISP_NODISP;
	obj_mgr->move_flag	|= (OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);
	
	// ライフ設定
	mgr_work->life		= GMD_BOSS4_LIFE;

	// パーツ発生用に確保
	obj_mgr->pos.x		= pos_x;
	obj_mgr->pos.y		= pos_y;
#if 0
	// 各パーツ生成
	{
		OBS_OBJECT_WORK	*obj_body;
		OBS_OBJECT_WORK	*obj_egg;
		GMS_BOSS4_BODY_WORK	*body_work;
		
		// ボス機本体
		obj_body	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS4_BODY,
												pos_x, pos_y,
												0,//flag
												0,0,0,0,
												0);

		// エッグマン
		obj_egg		= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS4_EGG,
												pos_x, pos_y,
												0,//flag
												0,0,0,0,
												0);

		// 回るカプセル
		OBS_OBJECT_WORK	*obj_cap;

		// カプセル連中初期化
		GmBoss4CapsuleClear();

		for(int i=0;i<GMD_BOSS4_CAP_MAX;i++){

			// GmBoss4CapsuleInit1st()の呼び出し
			obj_cap	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS4_CAP_1,
													pos_x, pos_y,
													0,//flag
													0,0,0,0,
													0);

			// 親は「ボス機」にしておく
			obj_cap->parent_obj = obj_body;
		}

		body_work	= (GMS_BOSS4_BODY_WORK*)obj_body;
		
		// 本体を管理の子に設定
		mgr_work->body_work		= body_work;
		
		// 本体に管理ワークの参照設定
		body_work->mgr_work		= mgr_work;
		
		// 各パーツの親設定
		obj_body->parent_obj	= obj_mgr;
		obj_egg->parent_obj		= obj_body;
		
		gm_boss4_mgr_work		= mgr_work;
		
		// 各パーツへの参照を設定
		body_work->parts_objs[GME_BOSS4_PART_IDX_BODY]	= obj_body;
		body_work->parts_objs[GME_BOSS4_PART_IDX_EGG ]	= obj_egg;
	}
#endif	
	// 処理関数設定
	obj_mgr->ppFunc	= gmBoss4MgrWaitLoad;

	/*
	//! 念のためスクロールロック解除
	GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_RIGHT|GMD_GMK_SCR_LMT_RELEASE_LEFT|GMD_GMK_SCR_LMT_RELEASE_BOTTOM|GMD_GMK_SCR_LMT_RELEASE_TOP);
	//! カメラターゲット

	// 位置設定
	GmMapBuildColData();
	*/


	// 完全に強制スクロールをOFF
	GmBoss4ScrollOff();

	// 第一形態
	gm_boss4_is_2nd = FALSE;

	// 中景のテスト		動きを見るために仮でここに入れてます。
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_FINAL_1) {	// Ishizaki Dimps 20091209
		// ループマップに対応させる
		GmMapSetAddMapScrlScaleMagX(GME_MAP_ADD_MAP_MID1, 1); 
		GmMapSetAddMapScrlScaleMagX(GME_MAP_ADD_MAP_MID2, 1); 
		GmMapSetAddMapScrlScaleMagX(GME_MAP_ADD_MAP_MID3, 1); 
		GmMapSetAddMapXLoop();
		GmMapEnableAddMapUserScrlX();
	}

	gm_boss4_n_scroll_start		= GMD_BOSS4_SCROLL_START_X;		//!< スクロール範囲
	gm_boss4_n_scroll_end		= GMD_BOSS4_SCROLL_END_X;		//!< スクロール範囲

	return obj_mgr;
}



// =======================================================================
// GmBoss4GetScrollOffset()
/*!
  強制スクロールのスクロール量を取得
  マップ戻す量なども一緒に入っている
 */
// =======================================================================
fx32 GmBoss4GetScrollOffset()
{
	// 強制スクロール量を取得
	return FX_F32_TO_FX32( gm_boss4_n_offset_x );
}


/*------ Static Functions ----------------------------------------------*/

// ############################################################################
// 共通
// ############################################################################
// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss4SetPartTextureBurnt
/*!
  パーツのテクスチャを黒こげタイプにする
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  スロット0のテクスチャオフセットをu+0.5しています。
 */
// =======================================================================
void gmBoss4SetPartTextureBurnt(OBS_OBJECT_WORK *obj_work, BOOL burn)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(obj_work->disp_flag & OBD_DISP_DRAWSTATE);
	
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_TEXOFFSET;	// 20090924 Dimps Ishizaki
	obj_work->obj_3d->draw_state.texoffset[0].mode	= NNE_MATCTRLMODE_ADD;

	if (burn){
		obj_work->obj_3d->draw_state.texoffset[0].u	= 0.5f;
	}else{
		obj_work->obj_3d->draw_state.texoffset[0].u	= 0.0f;
	}
}


// =======================================================================
// gmBoss4IsScrollLocked
/*!
  スクロールロック済み判定
  
  @retval TRUE	スクロールロックされている
  @retval FALSE	スクロールロックされていない
 */
// =======================================================================
BOOL gmBoss4IsScrollLockBusy(void)
{
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SCR_LIMIT_BUSY) {
		return TRUE;
	}
	
	return FALSE;

#if 0
	OBS_OBJECT_WORK	*obj_work;
	
	// TODO : とりあえず自前で実装。
	//        将来的にスクロールロックされているかチェックする手立てを用意してもらう
	
	
	obj_work	= ObjObjectSearchRegistObject(NULL, GMD_OBJTYPE_GIMMICK);
	while (obj_work) {
		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
		if (ene_com->eve_rec->id == GMD_EVENT_ID_SCR_LIMIT_SET) {
			return TRUE;
		}
		obj_work	= ObjObjectSearchRegistObject(obj_work, GMD_OBJTYPE_GIMMICK);
	}
	
	return FALSE;
#endif
}



// ############################################################################
// ボス1 管理
// ############################################################################

// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss4MgrWaitLoad
/*!
  本体 ロード完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4MgrWaitLoad(OBS_OBJECT_WORK *obj_work)
{

	GMS_BOSS4_MGR_WORK	*mgr_work	= (GMS_BOSS4_MGR_WORK*)obj_work;
//	GMS_BOSS4_BODY_WORK	*body_work	= mgr_work->body_work;

	BOOL	is_load_end	= FALSE;

	fx32	pos_x	= obj_work->pos.x;
	fx32	pos_y	= obj_work->pos.y;

	if (GmBsCmnIsFinalZoneType(obj_work)) {
//		if (1 /* 仮  FINAL ZONE 用にデータロード完了判定を行う予定 (TODO ZONE1ボスを確認する)*/) {
		// ボスラッシュ時データロード待ち
		if (GmMainDatLoadBossBattleLoadCheck(GMD_GAMEDAT_LOAD_BOSS_TYPE_4)) {
			is_load_end	= TRUE;
		}
	}
	else {
		// 通常ステージの場合はロード完了待ちを行わない
		is_load_end	= TRUE;
	}

	// 各パーツ生成
	if (is_load_end)
	{
		OBS_OBJECT_WORK	*obj_body;
		OBS_OBJECT_WORK	*obj_egg;
		GMS_BOSS4_BODY_WORK	*body_work;
		
		// ボス機本体
		obj_body	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS4_BODY,
												pos_x, pos_y,
												0,//flag
												0,0,0,0,
												0);

		GmBoss4IncObjCreateCount();			// オブジェクトカウントアップ


		// エッグマン
		obj_egg		= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS4_EGG,
												pos_x, pos_y,
												0,//flag
												0,0,0,0,
												0);

		GmBoss4IncObjCreateCount();			// オブジェクトカウントアップ

		// 回るカプセル
		OBS_OBJECT_WORK	*obj_cap;

		// カプセル連中初期化
		GmBoss4CapsuleClear();

		for(int i=0;i<GMD_BOSS4_CAP_MAX;i++){

			// GmBoss4CapsuleInit1st()の呼び出し
			obj_cap	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS4_CAP_1,
													pos_x, pos_y,
													0,//flag
													0,0,0,0,
													0);

			// 親は「ボス機」にしておく
			obj_cap->parent_obj = obj_body;

			GmBoss4IncObjCreateCount();			// オブジェクトカウントアップ
		}

		body_work	= (GMS_BOSS4_BODY_WORK*)obj_body;
		
		// 本体を管理の子に設定
		mgr_work->body_work		= body_work;
		
		// 本体に管理ワークの参照設定
		body_work->mgr_work		= mgr_work;
		
		// 各パーツの親設定
		obj_body->parent_obj	= obj_work;
		obj_egg->parent_obj		= obj_body;
		
//		gm_boss4_mgr_work		= mgr_work;
		
		// 各パーツへの参照を設定
		body_work->parts_objs[GME_BOSS4_PART_IDX_BODY]	= obj_body;
		body_work->parts_objs[GME_BOSS4_PART_IDX_EGG ]	= obj_egg;
	}
/*
	// 本体パーツの処理で、全てのパーツのアクションをまとめて設定しないといけないので
	// パーツが一通りそろっているかチェック
	for (Sint32 i = 0; i < GME_BOSS4_PART_IDX_MAX; ++i) {
		if (body_work->parts_objs[i] == NULL) {
			result	= FALSE;
		}
	}
*/	
	if (is_load_end) {
		mgr_work->flag	|= GMD_BOSS4_MGR_FLAG_LOAD_END;
		obj_work->ppFunc	= gmBoss4MgrMain;
	}
}

// =======================================================================
// gmBoss1MgrWaitRelease
/*!
  管理 解放待ち処理
 */
// =======================================================================
void gmBoss4MgrWaitRelease(OBS_OBJECT_WORK *obj_work)
{
//	GMS_BOSS4_MGR_WORK	*mgr_work	= (GMS_BOSS4_MGR_WORK*)obj_work;
	
	// REMINDER :
	// 各パーツやエフェクトのオブジェクトの生成時に生成数をカウント ＆
	// それらのデストラクタに生成数デクリメント処理を仕込んでおく
	// この関数内で生成数が0になるのを待って、0になったら全て消去されたと判定する
	
	if (GmBoss4IsAllCreatedObjDeleted()) {
		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
		
		// ファイナルゾーンでは自分も消去
		ene_com->enemy_flag	|= GMD_ENEMY_FLAG_DIE;	// 復活しないようにする
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
		
		// データ開放処理開始
		GmGameDatReleaseBossBattleStart(GMD_GAMEDAT_LOAD_BOSS_TYPE_4);
/*		
		// スクロールロック解除（ファイナルゾーンでは左以外全部解除）
		GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_TOP |
								GMD_GMK_SCR_LMT_RELEASE_RIGHT |
								GMD_GMK_SCR_LMT_RELEASE_BOTTOM);
*/
	}
}

// =======================================================================
// gmBoss4MgrMain
/*!
  管理 メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4MgrMain(OBS_OBJECT_WORK *obj_work)
{
	// カメラに合わせてロックをかけ、カメラを一気に動かす仕組み
	OBS_CAMERA*	obj_cam = ObjCameraGet(GME_CAMERA_NO_MAIN);
	// カメラがない場合は終わる
	if (obj_cam==NULL)
		return;

	GMS_BOSS4_MGR_WORK	*mgr_work	= (GMS_BOSS4_MGR_WORK*)obj_work;
	
	if (mgr_work->flag & GMD_BOSS4_MGR_FLAG_CLEAR_BOSS) {
		if (mgr_work->body_work) {
			GMM_BS_OBJ(mgr_work->body_work)->flag	|= OBD_OBJECT_TASKCLEAR_REQUEST;
			mgr_work->body_work	= NULL;
		}
		// FinalZone用に追加
		if (GmBsCmnIsFinalZoneType(obj_work)) {
			// ファイナルゾーンではデータ解放を行う
			obj_work->ppFunc	= gmBoss4MgrWaitRelease;
/*
			GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
			// ファイナルゾーンでは自分も消去
			ene_com->enemy_flag	|= GMD_ENEMY_FLAG_DIE;	// 復活しないようにする
			obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
*/
		}
	}
	// カプセルを回す
	GmBoss4CapsuleUpdateRol( GMD_BOSS4_CAP_ROTATE_SPD );
/*
	amPrintf( 10, 25, "CAPSULE: %d", GmBoss4CapsuleGetCount() );
	amPrintf( 10, 26, "HP     : %d", GmBoss4GetLife() );

	amPrintf( 10, 27, "WIDTH  : %d", OBD_LCD_X );
	amPrintf( 10, 28, "HEIGHT : %d", OBD_LCD_Y );

	if (AoPadStand() & KEY_L1) {
		if (g_obj.flag & OBD_OBJ_RECT_D){
			g_obj.flag &= ~OBD_OBJ_RECT_D;
		}else{
			g_obj.flag |= OBD_OBJ_RECT_D;
		}
	}
*/
	obj_cam->flag &= ~OBD_CAMERA_FIX;

	{
		// 中景
		static float xold	= 0;
		float xmove = obj_cam->disp_pos.x - obj_cam->prev_disp_pos.x;
		if (xmove<-(GMD_BOSS4_SCROLL_SPD_MAX+8.0f)){
			xmove = xold;
		}
		if (xmove>(GMD_BOSS4_SCROLL_SPD_MAX+8.0f)){
			xmove = xold;
		}
		if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_FINAL_1) {	// Ishizaki Dimps 20091209
			GmMapSetAddMapUserScrlXAddSize( xmove );
		}
		xold = xmove;
	}

	OBS_OBJECT_WORK* ply = (OBS_OBJECT_WORK*)GmBsCmnGetPlayerObj();
	GMS_PLAYER_WORK* ply_work = (GMS_PLAYER_WORK*)ply;
/*
#ifdef	_DEBUG
	// Rボタンでコインが減らないようにしておく
	if (AoPadDirect() & KEY_R1) {
		if (ply_work->ring_num<=0){
			ply_work->ring_num=10;
		}
		amPrintf( 10, 31, "**!!**!! COIN UP MODE **!!**!!" );
	}
#endif	//_DEBUG
*/
//#if 0
	// 強制スクロール
	if ( gm_boss4_n_scroll || gm_boss4_f_scroll_spd > 0){

		Sint32	_left	= g_gm_main_system.map_fcol.left	/*obj_cam->pos.x - 120*/;
		Sint32	_right	= g_gm_main_system.map_fcol.right	/*obj_cam->pos.x + 120*/;


		NNS_VECTOR	vec = { FX_FX32_TO_F32( GmBoss4GetScrollOffset() ), 0.0f, 0.0f };
		if (vec.x < 0.f) {
			// ワープした時のみ
			amTrailEFOffsetPos(AMTRE_HANDLE_ACCELL, &vec ); 
		}
		//-----------------------------------------
		// ソニック
		//-----------------------------------------

		// ソニックは動かないので、オフセット分だけ足してスクロールスピードを引く
		// これでマップ移動に対応できる
		ply->pos.x += GmBoss4GetScrollOffset();

		// 死亡中
		Sint32	add = (Sint32)gm_boss4_f_scroll_spd * FX32_ONE;

		if (ply_work->player_flag & GMD_PLF_DIE){
			// 死亡中はスクロール依存させる
			add = 0;
		}
		if (ply_work->seq_state==GME_PLY_SEQ_STATE_JUMP){
			if (ply_work->obj_work.spd.x < FX_F32_TO_FX32(2.0f)){
				add /=4;
			}
		}
		if (ply_work->seq_state==GME_PLY_SEQ_STATE_HOMING_REF){
			if (ply_work->obj_work.spd.x < FX_F32_TO_FX32(3.0f)){
				add /=4;
			}
		}
		ply->pos.x -= add;

		// リミッターチェック
		Sint32 px = (ply->pos.x / FX32_ONE);
		// 現在リミッターに引っかかると動作しなくなるため、
		// リミッターに引っかからないようにする
		if (_left+GMD_BOSS4_SCROLL_LIMIT_OFFSET_X > px ){
			ply->pos.x = (_left+GMD_BOSS4_SCROLL_LIMIT_OFFSET_X) * FX32_ONE;
		}
		if (_right < px ){
			ply->pos.x = (_right -2) * FX32_ONE;
		}

		//amPrintf( 10, 30, "(X,Y)  : %d, %d", ply->pos.x / FX32_ONE, ply->pos.y / FX32_ONE );
/*
		// ソニックのオートラン設定
		BOOL	gm_boss4_b_scroll = FALSE;
		if (gm_boss4_n_scroll==1 || gm_boss4_n_scroll==2){
			gm_boss4_b_scroll = TRUE;
		}
		GmPlayerSetAutoRun( (GMS_PLAYER_WORK*)ply, (fx32)((gm_boss4_f_scroll_spd-0.1f) * FX32_ONE), gm_boss4_b_scroll );
*/
		//-----------------------------------------
		// ボスのリミッターチェック
		//-----------------------------------------
		GMS_BOSS4_BODY_WORK*	body_work	= mgr_work->body_work;
		OBS_OBJECT_WORK*		body_obj	= (OBS_OBJECT_WORK*)body_work;

		if (body_obj!=NULL){
			body_obj->pos.x += GmBoss4GetScrollOffset();
			body_obj->pos.x -= FX_F32_TO_FX32( gm_boss4_f_scroll_spd );

			if (gm_boss4_n_scroll == GME_BOSS4_SCROLL_TYPE_ADDSPEED ){
				// 遠くから現れるために
				body_obj->pos.x += FX_F32_TO_FX32( GMD_BOSS4_SCROLL_SPD_BOSS );
			}else{
				// ボス撃破後(ゆっくり左に流れる)
				body_obj->pos.x += FX_F32_TO_FX32( GMD_BOSS4_SCROLL_SPD_BOSS_BROKEN );
			}

			Sint32 bx = (Sint32)FX_FX32_TO_F32( body_obj->pos.x );
			if (_left > bx ){
				body_obj->pos.x = FX_F32_TO_FX32( _left );
			}

			// ボス撃破前はこの位置より下がらない
			if ( gm_boss4_n_scroll == GME_BOSS4_SCROLL_TYPE_ADDSPEED ){
				bx = (Sint32)FX_FX32_TO_F32( body_obj->pos.x );
				if (_right - GMD_BOSS4_SCROLL_RIGHT_LIMIT_X > bx ){
					body_obj->pos.x = FX_F32_TO_FX32( _right - GMD_BOSS4_SCROLL_RIGHT_LIMIT_X );
				}
			}
		}
		//-----------------------------------------
		// ボスへのホーミング距離をここで変えてしまう(TEST)
		//-----------------------------------------
		if (body_obj!=NULL){
			GMS_ENEMY_3D_WORK*	ene_3d	= (GMS_ENEMY_3D_WORK*)body_work;

			fx32 len = (fx32)(abs(body_obj->pos.x - ply->pos.x));
			if (len > FX_F32_TO_FX32( GMD_BOSS4_SCROLL_HOMING_LENGTH )){
				ene_3d->ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
			}else{
				ene_3d->ene_com.enemy_flag	&= ~GMD_ENEMY_FLAG_NOHOMING;
			}
			//amPrintf( 10, 29, "LEN : %3.2f", FX_FX32_TO_F32(len) );
		}
		
	}
//#endif
}

// ==========================================================================
// GmCameraFunc
/*!
 *	メインカメラ
 *	@param	obj_camera	[in]	オブジェクトカメラ
 */
// ==========================================================================
void gmCameraForceScrollFunc(OBS_CAMERA *obj_cam)
{
	/*
		強制スクロール注意点
			以下の方法で強制スクロールを行う
			優先順位が非常に重要になります。

			タスク処理は、通常のオブジェクトを配置してから、
			カメラが最後に配置され、それにより映像が出るというところに注意する。
			つまりこのタスクはオブジェ群より跡に処理されることになる
		
		1.カメラのポジジョンを強制スクロールポジションにする
		2.強制スクロールポジションを移動させ、その移動量を他のオブジェクトのために残しておく
		3.画面リミッターを強制スクロールポジションにあわせ作成する。

		この順序で処理することにより、オブジェがおかしくならない状態でワープさせることができる
	*/

	GMS_PLAYER_WORK* ply_work = (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
/*
	Sint32 y = 15;
	amPrintf(2, y, "CAM X : %f", obj_cam->pos.x);		y++;
	amPrintf(2, y, "CAM Y : %f", obj_cam->pos.y);		y++;
	amPrintf(2, y, "CAM Z : %f", obj_cam->pos.z);		y++;
	amPrintf(2, y, "TAR X : %f", obj_cam->target_pos.x);	y++;
	amPrintf(2, y, "TAR Y : %f", obj_cam->target_pos.y);	y++;
	amPrintf(2, y, "TAR Z : %f", obj_cam->target_pos.z);	y++;
*/
	if (g_gm_main_system.game_flag	& GMD_GAME_FLAG_PAUSE_DEMO)
		return;

#ifdef	MTD_DEBUG
	if (g_obj.pause_level >=0){
		if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_DEBUG_PAUSE)
			return;
	}
#endif	//(MTD_DEBUG)

	// 初期段階の場合は、カメラの値を取り込む
	if (gm_boss4_f_scroll_spd<=-100.0f && gm_boss4_n_scroll_pt_x==0){
		if ((Float)(obj_cam->pos.x != obj_cam->prev_pos.x)) {
			gm_boss4_f_scroll_spd = (Float)(obj_cam->pos.x - obj_cam->prev_pos.x);//FX_FX32_TO_F32( play->spd.x );
		} else {
			gm_boss4_f_scroll_spd = FXM_FX32_TO_FLOAT(ply_work->obj_work.move.x);
		}
		gm_boss4_n_scroll_pt_x	= (Sint32)( obj_cam->pos.x + gm_boss4_f_scroll_spd );// + FX_FX32_TO_F32( play->spd.x );//(Sint32)obj_cam->target_pos.x;
		
	}


	// 強制スクロール加速
	if (gm_boss4_n_scroll!=0){
		gm_boss4_f_scroll_spd	+= GMD_BOSS4_SCROLL_SPD_ADD;
	}else{
		// ボス撃破後スクロールOFFのときスピードダウン
		gm_boss4_f_scroll_spd	-= GMD_BOSS4_SCROLL_SPD_SUB;
	}

	// スクロールスピードの範囲を超えないように
	if (gm_boss4_f_scroll_spd > gm_boss4_f_scroll_spd_max){
		gm_boss4_f_scroll_spd = gm_boss4_f_scroll_spd_max;
	}
	if (gm_boss4_f_scroll_spd < 0){
		gm_boss4_f_scroll_spd = 0;
	}

	if (gm_boss4_n_scroll==0 && gm_boss4_f_scroll_spd==0){
		// カメラを通常状態に戻す
		g_gm_main_system.map_fcol.left = 0;
		g_gm_main_system.map_fcol.right = g_gm_main_system.map_fcol.map_block_num_x*64;

		ObjCameraSetUserFunc(GME_CAMERA_NO_MAIN, GmCameraFunc );	// ユーザー処理

		// スクロールロック解除（ファイナルゾーンでは左以外全部解除）
		GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_TOP |
								GMD_GMK_SCR_LMT_RELEASE_RIGHT |
								GMD_GMK_SCR_LMT_RELEASE_BOTTOM);

		GmBoss4UtilPlayerStop( false );

		return;
	}

    NNS_VECTOR pos;

	//GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	pos.x = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.x);
    pos.y = FXM_FX32_TO_FLOAT(-ply_work->obj_work.pos.y + GMD_SCR_PLY_Y_OFFS);
    pos.z = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.z);

    // 目的値を記録
    obj_cam->work.x = pos.x;
    obj_cam->work.y = pos.y;
    obj_cam->work.z = pos.z;

//	obj_cam->prev_pos.x = obj_cam->pos.x;
//	obj_cam->prev_pos.y = obj_cam->pos.y;
//	obj_cam->prev_pos.z = obj_cam->pos.z;

	//
/*
	gm_boss4_n_scroll_pt_x += (Sint32)gm_boss4_f_scroll_spd;
	gm_boss4_n_offset_x = (Sint32)gm_boss4_f_scroll_spd;
	if (gm_boss4_n_scroll_pt_x> gm_boss4_n_scroll_end ){
		gm_boss4_n_offset_x = -(gm_boss4_n_scroll_end - gm_boss4_n_scroll_start) + gm_boss4_f_scroll_spd;
		gm_boss4_n_scroll_pt_x -= ( gm_boss4_n_scroll_end - gm_boss4_n_scroll_start);
	}
*/
	if (gm_boss4_n_scroll == GME_BOSS4_SCROLL_TYPE_NORMAL){
		GmBoss4UtilPlayerStop( false );

		// カメラをソニック(進み先)に徐々にあわせる

		Sint32 ptx = (Sint32)FX_FX32_TO_F32( ply_work->obj_work.pos.x + ply_work->obj_work.spd.x );
/*
		if (ptx > gm_boss4_n_scroll_pt_x + 16){
			gm_boss4_n_scroll_pt_x	+=16;
		}else if (ptx < gm_boss4_n_scroll_pt_x - 16){
			gm_boss4_n_scroll_pt_x	-=16;
		}else{
			gm_boss4_n_scroll_pt_x = ptx;
		}
*/
		if (ptx < (gm_boss4_n_scroll_pt_x - obj_cam->allow.x)) {
			ptx += (Sint32)obj_cam->allow.x;
		} else if (ptx > (gm_boss4_n_scroll_pt_x + obj_cam->allow.x)) {
			ptx -= (Sint32)obj_cam->allow.x;
		} else {
			ptx = gm_boss4_n_scroll_pt_x;
		}
		gm_boss4_n_scroll_pt_x = ptx;

	}


	BOOL	scr_loop_ret = FALSE;
	if (obj_cam->pos.x > (Float)gm_boss4_n_scroll_pt_x) {
		// スクロールがループした
		scr_loop_ret = TRUE;
	}
	
	static float ofst = 0;

	obj_cam->prev_pos.x			=	obj_cam->pos.x;
	obj_cam->pos.x				=	(Float)gm_boss4_n_scroll_pt_x;

//	obj_cam->prev_disp_pos.x	=	obj_cam->disp_pos.x;
	obj_cam->disp_pos.x			=	(Float)gm_boss4_n_scroll_pt_x;

	obj_cam->target_pos.x		=	gm_boss4_n_scroll_pt_x + ofst;


	// オブジェクトカメラ設定
	ObjObjectCameraSet(	FXM_FLOAT_TO_FX32( obj_cam->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_cam->disp_pos.y - (float)(OBD_LCD_Y/2)),
						FXM_FLOAT_TO_FX32( obj_cam->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_cam->disp_pos.y - (float)(OBD_LCD_Y/2)));
	
	// クリッピングカメラ設定
	GmCameraSetClipCamera(obj_cam);

	if (  (scr_loop_ret)
		&&(g_gs_main_sys_info.stage_id >= GSD_MAIN_STAGE_ID_FINAL_1) ) {

		//強制スクロール中
		if ( gm_boss4_n_scroll==GME_BOSS4_SCROLL_TYPE_ADDSPEED 
			|| gm_boss4_n_scroll==GME_BOSS4_SCROLL_TYPE_SUBSPEED
		){
			//装飾ループ用の情報を設定
			GmDecoSetLoopState();
		}
		
		// ファイナルゾーンのループ先オブジェクト(シャッター)を生成
		GmEveMgrCreateEventLcd( GMD_EVE_SEARCH_PROC_FLAG_NONE );

		//装飾ループ用の情報をクリア
		GmDecoClearLoopState();
	}

	// ダメージコイン処理
	GMS_RING_WORK*	ring_work = NULL;

	GmBoss4UtilIterateDamageRingInit();
	
	static Sint32 _damage_cnt = 0;
	if (_damage_cnt==0){
		ring_work = GmBoss4UtilIterateDamageRingGet();
		if ( ring_work!=NULL ){
			// しょっぱなのダメージ!
			_damage_cnt++;
			GmBoss4UtilIterateDamageRingInit();
			while ( (ring_work = GmBoss4UtilIterateDamageRingGet()) != NULL ){
				ring_work->pos.x += GmBoss4GetScrollOffset();
				// 少しだけこちらに流す
				ring_work->spd_x += FX_F32_TO_FX32( GMD_BOSS4_SCROLL_COIN_ADDSPD_FIRST );
			}
		}
	}else{
		ring_work = GmBoss4UtilIterateDamageRingGet();
		if ( ring_work != NULL ){
		}else{
			_damage_cnt = 0;
		}
	}
	

	GmBoss4UtilIterateDamageRingInit();
	while ( (ring_work = GmBoss4UtilIterateDamageRingGet()) != NULL ){
		ring_work->pos.x += GmBoss4GetScrollOffset();
		// 少しだけこちらに流す
		ring_work->spd_x += FX_F32_TO_FX32( GMD_BOSS4_SCROLL_COIN_ADDSPD );
	}

	// ソニックの位置の割合によって画面をずらす(VGA用)
	OBS_OBJECT_WORK* play = (OBS_OBJECT_WORK*)GmBsCmnGetPlayerObj();
#if 1	// AMD_SCREEN_2D_WIDTHからGSD_DIPS_WIDTHに変更することでワイド、非ワイドを対応したため、念のため
		// 大きさに合わせて対応。 ただし画面確認は、1280,960のみしか行っていない

#ifdef	MTD_DEBUG
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		amPrintf( 10, 27, "(A,D)  : %2.1f, %2.1f", GSD_DISP_WIDTH, AMD_SCREEN_2D_WIDTH );
		amPrintf( 10, 28, "(P-C)  : %2.1f", (play->pos.x/FX32_ONE) - obj_cam->pos.x );
		amPrintf( 10, 29, "(SA)   : %2.1f", (AMD_SCREEN_2D_WIDTH/GSD_DISP_WIDTH - 1.0f) /30 );
		amPrintf( 10, 30, "(OFS)  : %2.1f", (AMD_SCREEN_2D_WIDTH/GSD_DISP_WIDTH - 1.0f)* ((play->pos.x/FX32_ONE) - obj_cam->pos.x) / 30);
	}
#endif
	ofst = (AMD_SCREEN_2D_WIDTH/GSD_DISP_WIDTH - 1.0f)* ((play->pos.x/FX32_ONE) - obj_cam->pos.x) / 30;

#else	//
	// VGAの時はカメラをスクロールするようにする
	// ソニックがWIDEの画面外にいるときはなにもしない
	if (play->pos.x > FX_F32_TO_FX32( g_gm_main_system.map_fcol.left )){

		// TODO 正式にチェックする
		if (OBD_LCD_X < 300){
			ofst = (((play->pos.x/FX32_ONE) - obj_cam->pos.x) / 86/*(OBD_LCD_X/4)*/ );
		}

	}
#endif 

	// ソニックのオートラン設定
	BOOL	gm_boss4_b_scroll = FALSE;
	if (gm_boss4_n_scroll==GME_BOSS4_SCROLL_TYPE_ADDSPEED || 
		gm_boss4_n_scroll==GME_BOSS4_SCROLL_TYPE_SUBSPEED){
		gm_boss4_b_scroll = TRUE;
	}
	else{
		//装飾設定
		GmDecoEndLoop();
	}

	Float	xmove =	gm_boss4_f_scroll_spd;
	if (xmove<-100){
		xmove += (GMD_BOSS4_SCROLL_END_X - GMD_BOSS4_SCROLL_START_X);
	}

	// ソニックを操作不可にする
	if (gm_boss4_n_scroll == GME_BOSS4_SCROLL_TYPE_SUBSPEED){

		// ソニックが通常走りモーションになっている
		if (ply_work->seq_state == GME_PLY_SEQ_STATE_WALK) {
			// ソニック動作させない
			GmBoss4UtilPlayerStop( true );
		}

		// 画面の真ん中に寄せる
		if (play->pos.x > FX_F32_TO_FX32( obj_cam->pos.x )){
			GmPlayerSetAutoRun( (GMS_PLAYER_WORK*)play,
								(fx32)((xmove+GMD_BOSS4_SCROLL_SONIC_SPD_LOW) * FX32_ONE),
								gm_boss4_b_scroll );
		}else{
			GmPlayerSetAutoRun( (GMS_PLAYER_WORK*)play,
								(fx32)((xmove+GMD_BOSS4_SCROLL_SONIC_SPD_HIGH) * FX32_ONE),
								gm_boss4_b_scroll );
		}

	}else{

		// じっとしてたら下がっていくように設定
		GmPlayerSetAutoRun( (GMS_PLAYER_WORK*)play, (fx32)((xmove + GMD_BOSS4_SCROLL_SONIC_SPD) * FX32_ONE), gm_boss4_b_scroll );
	}

	gm_boss4_n_scroll_pt_x += (Sint32)gm_boss4_f_scroll_spd;
	gm_boss4_n_offset_x = (Sint32)gm_boss4_f_scroll_spd;
	if (gm_boss4_b_warpout){
		gm_boss4_b_warpout=FALSE;
		Sint32 len = GMD_BOSS4_SCROLL_OUT_X - gm_boss4_n_scroll_pt_x;
		gm_boss4_n_scroll_pt_x = GMD_BOSS4_SCROLL_OUT_X;
		gm_boss4_n_offset_x = len;
	}
	if (gm_boss4_n_scroll_pt_x> gm_boss4_n_scroll_end ){
		gm_boss4_n_offset_x = -(gm_boss4_n_scroll_end - gm_boss4_n_scroll_start) + (Sint32)gm_boss4_f_scroll_spd;
		gm_boss4_n_scroll_pt_x -= ( gm_boss4_n_scroll_end - gm_boss4_n_scroll_start);
	}


	// 左右固定 WIDE画面端で左右を固定する
	// ボスの移動範囲はWIDEで対応するため、AMD_SCREEN_2D_WIDTHをそのまま使用する
	g_gm_main_system.map_fcol.left	=	gm_boss4_n_scroll_pt_x - (Sint32)((float)(AMD_SCREEN_2D_WIDTH/2) * GMD_CAMERA_SCALE)-GMD_BOSS4_SCROLL_LIMIT_OFFSET_X;//180;
	g_gm_main_system.map_fcol.right	=	gm_boss4_n_scroll_pt_x + (Sint32)((float)(AMD_SCREEN_2D_WIDTH/2) * GMD_CAMERA_SCALE);//180;

}

// =======================================================================
// gmBoss4ScrollInit
/*!
  強制スクロールイベント ( イベント設定用 )
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4ScrollInit(GMS_EVE_RECORD_EVENT *eve_rec,
									fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	UNREFERENCED_PARAMETER(pos_x);
	UNREFERENCED_PARAMETER(pos_y);
	UNREFERENCED_PARAMETER(eve_rec);

	gm_boss4_n_scroll		= GME_BOSS4_SCROLL_TYPE_ADDSPEED;
	gm_boss4_f_scroll_spd	= GMD_BOSS4_SCROLL_SPD_START;

	// 初期段階であることをカメラに伝える
	gm_boss4_f_scroll_spd = -100.0f;	//FX_FX32_TO_F32( play->spd.x );
	gm_boss4_n_scroll_pt_x		= 0;	//obj_cam->pos.x + FX_FX32_TO_F32( play->spd.x );//(Sint32)obj_cam->target_pos.x;

	// メインカメラの処理を変更する
	ObjCameraSetUserFunc(GME_CAMERA_NO_MAIN, gmCameraForceScrollFunc );	// ユーザー処理

	gm_boss4_is_2nd = TRUE;

	return NULL;
}


// =======================================================================
// gmBoss4ScrollNext
/*!
  強制スクロールイベントを次に移す
  
 */
// =======================================================================
void GmBoss4ScrollNext()
{
	if (gm_boss4_n_scroll==GME_BOSS4_SCROLL_TYPE_ADDSPEED){
		gm_boss4_n_scroll = GME_BOSS4_SCROLL_TYPE_SUBSPEED;
	}else if (gm_boss4_n_scroll == GME_BOSS4_SCROLL_TYPE_SUBSPEED){
		gm_boss4_n_scroll = GME_BOSS4_SCROLL_TYPE_NORMAL;
	}
	gm_boss4_n_offset_x = 0;
}


// =======================================================================
// gmBoss4ScrollOff()
/*!
  強制スクロールイベント 終了(ボス撃破)
  
 */
// =======================================================================
void GmBoss4ScrollOff()
{
	gm_boss4_n_scroll		= GME_BOSS4_SCROLL_TYPE_NORMAL;
	gm_boss4_n_offset_x		= 0;
	gm_boss4_f_scroll_spd	= 0;
	gm_boss4_b_warpout		= FALSE;
}

// =======================================================================
// gmBoss4ScrrollOut()
/*!
  強制スクロールイベント 移動(ボス撃破の白フラッシュで使用する)
  
 */
// =======================================================================
void GmBoss4ScrollOut()
{
	gm_boss4_b_warpout		= TRUE;
}

// =======================================================================
// GmBoss4Is2ndStage()
/*!
	第２形態状態なのかをチェックします
 */
// =======================================================================
BOOL	GmBoss4Is2ndStage(){
	return gm_boss4_is_2nd;
}

// =======================================================================
// GmBoss4CheckBossRush()
/*!
	現在のステージからステージファイナルの
	ボスラッシュかどうかをチェックします
  
 */
// =======================================================================
BOOL	GmBoss4CheckBossRush()
{
	if (gm_boss4_mgr_work==NULL)
		return FALSE;

//	MTM_ASSERT( gm_boss4_mgr_work );

	return GmBsCmnIsFinalZoneType( (OBS_OBJECT_WORK*)gm_boss4_mgr_work );
/*
#if 1
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();

	if (gs_main->stage_id != GMD_BOSS4_STAGE4BOSS_ID){
		return TRUE;
	}
	return FALSE;
#else
	return TRUE;
#endif
*/
}

// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// GmBoss4IncObjCreateCount
/*!
  オブジェクト生成カウント 増加
  
  @param mgr_work	[io]	管理ワーク
  
  @note
  ボス専用データアーカイブを参照するオブジェクトを生成する際に呼び出してください。
 */
// =======================================================================
void GmBoss4IncObjCreateCount()
{
	MTM_ASSERT( gm_boss4_mgr_work!=NULL );
	MTM_ASSERT( gm_boss4_mgr_work->obj_create_cnt >= 0 );
	
	gm_boss4_mgr_work->obj_create_cnt	+= 1;
}

// =======================================================================
// GmBoss4DecObjCreateCount
/*!
  オブジェクト生成カウント 減少
  
  @param mgr_work	[io]	管理ワーク
  
  @note
  ボス専用データアーカイブを参照するオブジェクトのデストラクト時に呼び出してください。
 */
// =======================================================================
void GmBoss4DecObjCreateCount()
{
	MTM_ASSERT( gm_boss4_mgr_work!=NULL );
	MTM_ASSERT( gm_boss4_mgr_work->obj_create_cnt > 0 );

	gm_boss4_mgr_work->obj_create_cnt	-= 1;
}

// =======================================================================
// GmBoss4IsAllCreatedObjDeleted
/*!
  生成カウントされたオブジェクトが全て消去されたか判定
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
BOOL GmBoss4IsAllCreatedObjDeleted()
{
	MTM_ASSERT( gm_boss4_mgr_work!=NULL );
	MTM_ASSERT( gm_boss4_mgr_work->obj_create_cnt >= 0 );
	
	if (gm_boss4_mgr_work->obj_create_cnt <= 0) {
		return TRUE;
	}
	
	return FALSE;
}


#if 0
// =======================================================================
// gmBoss4ScrollInit
/*!
  強制スクロールイベント ( イベント設定用 )
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4ScrollInit(GMS_EVE_RECORD_EVENT *eve_rec,
									fx32 pos_x, fx32 pos_y, u8 type)
{
	// すでに強制スクロール中
	if (pScrollTask!=NULL){
		return NULL;
	}

	OBS_CAMERA*	obj_cam = ObjCameraGet(GME_CAMERA_NO_MAIN);
	gm_boss4_n_scroll_pt_x = obj_cam->pos.x;

	// 強制スクロール管理タスクを生む (優先をカメラより前に)
	pScrollTask = MTM_TASK_MAKE_TCB( gmBoss4ScrollFunc, gmBoss4ScrollExit, 0, GMD_TASK_PAUSELEVEL_DEF, /*GMD_TASK_PRIO_MAIN_PRE*/0x6800,
	                              GMD_TASK_GROUP_NO_ENEMY, 0, "Scroll" );

	return NULL;
}


void GmBoss4ScrollOff()
{
	if (pScrollTask!=NULL){
		mtTaskClearTcb( pScrollTask );
		pScrollTask = NULL;
	}
}


void gmBoss4ScrollExit(MTS_TASK_TCB *none)
{
	pScrollTask = NULL;
}


void gmBoss4ScrollFunc(MTS_TASK_TCB *none)
{
	// カメラに合わせてロックをかけ、カメラを一気に動かす仕組み
	OBS_CAMERA*	obj_cam = ObjCameraGet(GME_CAMERA_NO_MAIN);

	// カメラがない場合は終わる
	if (obj_cam==NULL)
		return;

	obj_cam->flag |= OBD_CAMERA_FIX;

	gm_boss4_n_scroll_pt_x += gm_boss4_n_scroll_spd;
	gm_boss4_n_offset_x = gm_boss4_n_scroll_spd;
	if (gm_boss4_n_scroll_pt_x> gm_boss4_n_scroll_end ){
		gm_boss4_n_offset_x = -(gm_boss4_n_scroll_end - gm_boss4_n_scroll_start);
		gm_boss4_n_scroll_pt_x -= ( gm_boss4_n_scroll_end - gm_boss4_n_scroll_start);
	}
//		static float cx = 192;
//		static float cy = -276;
//		static float cz = 50;

	obj_cam->prev_pos.x = obj_cam->pos.x;
	obj_cam->pos.x = gm_boss4_n_scroll_pt_x;
//		obj_cam->pos.y = cy;
//		obj_cam->pos.z = cz;

	obj_cam->prev_disp_pos.x = obj_cam->disp_pos.x;
	obj_cam->disp_pos.x = gm_boss4_n_scroll_pt_x;
//		obj_cam->disp_pos.y = cy;
//		obj_cam->disp_pos.z = cz + 1000;

	obj_cam->target_pos.x = gm_boss4_n_scroll_pt_x;

	// 左右固定
	float	left	= g_gm_main_system.map_fcol.left   + (float)(AMD_SCREEN_2D_WIDTH/2) * obj_cam->scale;
	float	right	= g_gm_main_system.map_fcol.right  - (float)(AMD_SCREEN_2D_WIDTH/2) * obj_cam->scale;
	g_gm_main_system.map_fcol.left = obj_cam->pos.x - 180;
	g_gm_main_system.map_fcol.right = obj_cam->pos.x + 180;

	// ソニック(ここで行わないとガタつくので)
	OBS_OBJECT_WORK* ply = (OBS_OBJECT_WORK*)GmBsCmnGetPlayerObj();

	// ソニックは動かないので、オフセット分だけ足してスクロールスピードを引く
	// これでマップ移動に対応できる
	ply->pos.x += GmBoss4GetScrollOffset();// * FX32_ONE;
	ply->pos.x -= gm_boss4_n_scroll_spd * FX32_ONE;

	// リミッターチェック
	Sint32 px = (ply->pos.x / FX32_ONE);
	if (g_gm_main_system.map_fcol.left > px ){
		ply->pos.x = g_gm_main_system.map_fcol.left * FX32_ONE;
	}
	if (g_gm_main_system.map_fcol.right < px ){
//			ply->pos.x = (( px - (gm_boss4_n_scroll_end-180+8)) + g_gm_main_system.map_fcol.left) * FX32_ONE;
		ply->pos.x = g_gm_main_system.map_fcol.right * FX32_ONE;
	}
	amPrintf( 10, 27, "(X,Y)  : %d, %d", ply->pos.x / FX32_ONE, ply->pos.y / FX32_ONE );
}
#endif




// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================
