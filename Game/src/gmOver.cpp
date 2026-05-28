// =======================================================================
/*!
  @file	gmOver.cpp
  @brief ゲームオーバー・タイムオーバー演出

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmOver.cpp 20 2011-04-22 12:46:46Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "gsEnvironment.h"
#include "gmMain.h"
#include "gmGameDat.h"
#include "gmCockpit.h"
#include "izFade.h"

#include "gmOver.h"

// データヘッダ
#include "common/arc/CPIT_MAIN.HMB"
#include "common/ace/G_OVER.HMA"
#include "common/ace/G_OVER_FR.HMA"
#include "common/ace/G_OVER_GE.HMA"
#include "common/ace/G_OVER_IT.HMA"
#include "common/ace/G_OVER_JP.HMA"
#include "common/ace/G_OVER_SP.HMA"
#include "common/ace/G_OVER_US.HMA"

//mpp-----------------------------------------
#include "mppCheckPointStorage.h"

/*------ Macros --------------------------------------------------------*/
//############ 共通 ###########################################################
#define GMD_OVER_PART_FADEOUT_POS_Z			((fx32)(FX32_ONE * -16))	//!< フェードアウトパーツ座標Z

//############ 管理 ###########################################################
#define GMD_OVER_MGR_START_WAIT_TIME		(30)		//!< 演出開始待ち時間


// ゲームオーバー
#define GMD_OVER_MGR_GAMEOVER_LOOP_DURATION		(480)	//!< ゲームオーバー表示停滞時間

// タイムオーバー

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! パーツタイプ列挙型
typedef enum
{
	GME_OVER_PART_TYPE_STRING	= 0,	//!< ゲーム・タイムオーバー文字
	GME_OVER_PART_TYPE_FADEOUT,			//!< フェードアウト
	
	GME_OVER_PART_TYPE_MAX,
	GME_OVER_PART_TYPE_NONE	= -1
} GME_OVER_PART_TYPE;

//! テクスチャアーカイブインデックス
typedef enum
{
	GME_OVER_TEX_IDX_COMMON	= 0,
	GME_OVER_TEX_IDX_STRING,
	
	GME_OVER_TEX_IDX_MAX
} GME_OVER_TEX_IDX;

//! AMAデータインデックス
typedef enum
{
	GME_OVER_AMA_IDX_COMMON	= 0,
	GME_OVER_AMA_IDX_STRING,
	
	GME_OVER_AMA_IDX_MAX
} GME_OVER_AMA_IDX;

//! 文字パーツ サブパーツインデックス列挙型
typedef enum
{
	GME_OVER_STRING_SUBPART_GOV_GAME	= 0,	//!< ゲームオーバーのGAME
	GME_OVER_STRING_SUBPART_GOV_OVER,			//!< ゲームオーバーのOVER
	GME_OVER_STRING_SUBPART_TOV_TIME,			//!< タイムオーバーのTIME
	GME_OVER_STRING_SUBPART_TOV_OVER,			//!< タイムオーバーのOVER
	
	GME_OVER_STRING_SUBPART_MAX
} GME_OVER_STRING_SUBPART;

//! フェードアウトパーツ サブパーツインデックス列挙型
typedef enum
{
	GME_OVER_FADEOUT_SUBPART_GOV_FOUT	= 0,	//!< ゲームオーバーのフェードアウト
	GME_OVER_FADEOUT_SUBPART_TOV_FOUT,			//!< タイムオーバーのフェードアウト
	
	GME_OVER_FADEOUT_SUBPART_MAX
} GME_OVER_FADEOUT_SUBPART;


typedef struct tag_GMS_OVER_MGR_WORK	GMS_OVER_MGR_WORK;

//! 管理ワーク
struct tag_GMS_OVER_MGR_WORK
{
	//! 更新処理関数
	void	(*proc_update)(GMS_OVER_MGR_WORK*);
	
	//! 表示関連処理関数（更新処理がストップしてても行いたい処理）
	void	(*proc_disp)(GMS_OVER_MGR_WORK*);
	
	Uint32		wait_timer;
	
	GMS_COCKPIT_2D_WORK	*string_sub_parts[GME_OVER_STRING_SUBPART_MAX];
	GMS_COCKPIT_2D_WORK	*fadeout_sub_parts[GME_OVER_FADEOUT_SUBPART_MAX];
};

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
//############ 共通 ###########################################################
static inline BOOL gmOverIsSkipKeyOn(void);
static inline void gmOverSetActionHide(GMS_COCKPIT_2D_WORK *cpit_2d);
static inline void gmOverSetActionPlay(GMS_COCKPIT_2D_WORK *cpit_2d);
static inline void gmOverSetActionPause(GMS_COCKPIT_2D_WORK *cpit_2d);

//############ 管理 ###########################################################
static void gmOverDest(MTS_TASK_TCB *tcb);
static void gmOverMain(MTS_TASK_TCB *tcb);
// ゲームオーバー更新シーケンス
static void gmOverProcUpdateGOInit(GMS_OVER_MGR_WORK *mgr_work);
static void gmOverProcUpdateGOWaitStart(GMS_OVER_MGR_WORK *mgr_work);
static void gmOverProcUpdateGOLoop(GMS_OVER_MGR_WORK *mgr_work);
static void gmOverProcUpdateGOWaitFadeEnd(GMS_OVER_MGR_WORK *mgr_work);
static void gmOverProcUpdateGOWaitFinalizeFade(GMS_OVER_MGR_WORK *mgr_work);
// タイムオーバー更新シーケンス
static void gmOverProcUpdateTOInit(GMS_OVER_MGR_WORK *mgr_work);
static void gmOverProcUpdateTOWaitStart(GMS_OVER_MGR_WORK *mgr_work);
static void gmOverProcUpdateTOWaitFadeEnd(GMS_OVER_MGR_WORK *mgr_work);
static void gmOverProcUpdateTOWaitFinalizeFade(GMS_OVER_MGR_WORK *mgr_work);
// 表示関連シーケンス
static void gmOverProcDispLoop(GMS_OVER_MGR_WORK *mgr_work);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

//! タスクTCB
static MTS_TASK_TCB* gm_over_tcb	= NULL;

//! テクスチャAMB参照ポインタリスト
static void *gm_over_texamb_list[GME_OVER_TEX_IDX_MAX]	= {0};
//! AOテクスチャ構造体リスト
static AOS_TEXTURE gm_over_textures[GME_OVER_TEX_IDX_MAX]	= {{0}};

//! テクスチャアーカイブ AMBインデックステーブル
const static Sint32 gm_over_tex_amb_idx_tbl[GSD_LANGUAGE_NUM][GME_OVER_TEX_IDX_MAX]	= {
	// JP
	{
		IDB_CPIT_MAIN_G_OVER_AMB,
		IDB_CPIT_MAIN_G_OVER_JP_AMB,
	},
	// US
	{
		IDB_CPIT_MAIN_G_OVER_AMB,
		IDB_CPIT_MAIN_G_OVER_US_AMB,
	},
	// FR
	{
		IDB_CPIT_MAIN_G_OVER_AMB,
		IDB_CPIT_MAIN_G_OVER_FR_AMB,
	},
	// IT
	{
		IDB_CPIT_MAIN_G_OVER_AMB,
		IDB_CPIT_MAIN_G_OVER_IT_AMB,
	},
	// GE
	{
		IDB_CPIT_MAIN_G_OVER_AMB,
		IDB_CPIT_MAIN_G_OVER_GE_AMB,
	},
	// SP
	{
		IDB_CPIT_MAIN_G_OVER_AMB,
		IDB_CPIT_MAIN_G_OVER_SP_AMB,
	},
};

//! AMAデータ AMBインデックステーブル
const static Sint32 gm_over_ama_amb_idx_tbl[GSD_LANGUAGE_NUM][GME_OVER_AMA_IDX_MAX]	= {
	// JP
	{
		IDB_CPIT_MAIN_G_OVER_AMA,
		IDB_CPIT_MAIN_G_OVER_JP_AMA,
		
	},
	// US
	{
		IDB_CPIT_MAIN_G_OVER_AMA,
		IDB_CPIT_MAIN_G_OVER_US_AMA,
	},
	// FR
	{
		IDB_CPIT_MAIN_G_OVER_AMA,
		IDB_CPIT_MAIN_G_OVER_FR_AMA,
	},
	// IT
	{
		IDB_CPIT_MAIN_G_OVER_AMA,
		IDB_CPIT_MAIN_G_OVER_IT_AMA,
	},
	// GE
	{
		IDB_CPIT_MAIN_G_OVER_AMA,
		IDB_CPIT_MAIN_G_OVER_GE_AMA,
	},
	// SP
	{
		IDB_CPIT_MAIN_G_OVER_AMA,
		IDB_CPIT_MAIN_G_OVER_SP_AMA,
	},
};


//############ アクションIDテーブル ###########################################

//! 文字パーツ 全アクションIDテーブル
const static Uint32 gm_over_string_act_id_tbl[GSD_LANGUAGE_NUM][GME_OVER_STRING_SUBPART_MAX]	= {
	// JP
	{
		IDA_G_OVER_JP_ACT_G_TEX_GAME,
		IDA_G_OVER_JP_ACT_G_TEX_OVER,
		IDA_G_OVER_JP_ACT_T_TEX_TIME,
		IDA_G_OVER_JP_ACT_T_TEX_OVER,
	},
	// US
	{
		IDA_G_OVER_US_ACT_G_TEX_GAME,
		IDA_G_OVER_US_ACT_G_TEX_OVER,
		IDA_G_OVER_US_ACT_T_TEX_TIME,
		IDA_G_OVER_US_ACT_T_TEX_OVER,
	},
	// FR
	{
		IDA_G_OVER_FR_ACT_G_TEX_GAME,
		IDA_G_OVER_FR_ACT_G_TEX_OVER,
		IDA_G_OVER_FR_ACT_T_TEX_TIME,
		IDA_G_OVER_FR_ACT_T_TEX_OVER,
	},
	// IT
	{
		IDA_G_OVER_IT_ACT_G_TEX_GAME,
		IDA_G_OVER_IT_ACT_G_TEX_OVER,
		IDA_G_OVER_IT_ACT_T_TEX_TIME,
		IDA_G_OVER_IT_ACT_T_TEX_OVER,
	},
	// GE
	{
		IDA_G_OVER_GE_ACT_G_TEX_GAME,
		IDA_G_OVER_GE_ACT_G_TEX_OVER,
		IDA_G_OVER_GE_ACT_T_TEX_TIME,
		IDA_G_OVER_GE_ACT_T_TEX_OVER,
	},
	// SP
	{
		IDA_G_OVER_SP_ACT_G_TEX_GAME,
		IDA_G_OVER_SP_ACT_G_TEX_OVER,
		IDA_G_OVER_SP_ACT_T_TEX_TIME,
		IDA_G_OVER_SP_ACT_T_TEX_OVER,
	},
};

//! フェードアウトパーツ 全アクションIDテーブル
const static Uint32 gm_over_fadeout_act_id_tbl[GME_OVER_FADEOUT_SUBPART_MAX]	= {
	IDA_G_OVER_ACT_G_FADEOUT,
	IDA_G_OVER_ACT_T_FADEOUT,
};


/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmOverBuildDataInit
/*!
  テクスチャビルド開始
 */
// =======================================================================
void GmOverBuildDataInit(void)
{
	amZeroMemory(gm_over_textures, sizeof(AOS_TEXTURE) * GME_OVER_TEX_IDX_MAX);
	
	for (Sint32 i = 0; i < GME_OVER_TEX_IDX_MAX; ++i) {
		MTM_ASSERT(gm_over_texamb_list[i] == NULL);
		
		gm_over_texamb_list[i]	=
			ObjDataLoadAmbIndex(NULL,
								gm_over_tex_amb_idx_tbl[GsEnvGetLanguage()][i],
								GmGameDatGetCockpitData());
		AoTexBuild(&gm_over_textures[i], gm_over_texamb_list[i]);
		AoTexLoad(&gm_over_textures[i]);
	}
}

// =======================================================================
// GmOverBuildDataLoop
/*!
  テクスチャビルド完了待ち
  
  @retval TRUE	ビルド完了
  @retval FALSE	ビルド中
 */
// =======================================================================
BOOL GmOverBuildDataLoop(void)
{
	BOOL	b_loaded	= TRUE;
	
	for (Sint32 i = 0; i < GME_OVER_TEX_IDX_MAX; ++i) {
		if (!AoTexIsLoaded(&gm_over_textures[i])) {
			b_loaded	= FALSE;
		}
	}
	
	return b_loaded;
}

// =======================================================================
// GmOverFlushDataInit
/*!
  テクスチャフラッシュ開始
 */
// =======================================================================
void GmOverFlushDataInit(void)
{
	// テクスチャ解放開始
	for (Sint32 i = 0; i < GME_OVER_TEX_IDX_MAX; ++i) {
		AoTexRelease(&gm_over_textures[i]);
	}
}

// =======================================================================
// GmOverFlushDataLoop
/*!
  テクスチャフラッシュ完了待ち
  
  @retval TRUE	フラッシュ完了
  @retval FALSE	フラッシュ中
 */
// =======================================================================
BOOL GmOverFlushDataLoop(void)
{
	BOOL	b_flushed	= TRUE;
	
	// 解放完了待ち
	for (Sint32 i = 0; i < GME_OVER_TEX_IDX_MAX; ++i) {
		
		if (gm_over_texamb_list[i] == NULL) {
			continue;
		}
		
		if (!AoTexIsReleased(&gm_over_textures[i])) {
			b_flushed	= FALSE;
		}
		else {
			gm_over_texamb_list[i]	= NULL;
			amZeroMemory(&gm_over_textures[i], sizeof(AOS_TEXTURE));
		}
	}
	
	return b_flushed;
}

// =======================================================================
// GmOverStart
/*!
  ゲーム／タイムオーバー開始
  
  @param type	[in]	デモタイプ(GME_OVER_TYPE_XXX)
 */
// =======================================================================
void GmOverStart(GME_OVER_TYPE type)
{
	{{//qqq
		OS_TPrintf("try to clear saved state after GAME_OVER or TIME_OVER...\n");			
		mppCheckPointStorage::removeState();
	}}	
	
	GMS_OVER_MGR_WORK	*mgr_work;
	
#if defined(GMD_DEBUG_NO_CREATE_COCKPIT)
	UNREFERENCED_PARAMETER(mgr_work);
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_GAMEOVER_END;
	return;
#endif /* defined(GMD_DEBUG_NO_CREATE_COCKPIT) */
	
	// タスク生成
	gm_over_tcb	= MTM_TASK_MAKE_TCB(gmOverMain,
									gmOverDest,
									0,//flag
									GMD_TASK_PAUSELEVEL_DEF,
									GMD_TASK_PRIO_GAMEOVER,
									GMD_TASK_GROUP_GAMEOVER,
									sizeof(GMS_OVER_MGR_WORK),
									"GM_OVER_MGR");
	
	// ワーク初期化
	mgr_work	= (GMS_OVER_MGR_WORK*)mtTaskGetTcbWork(gm_over_tcb);
	amZeroMemory(mgr_work, sizeof(GMS_OVER_MGR_WORK));
	
	// 文字サブパーツ生成
	for (Sint32 i = 0; i < GME_OVER_STRING_SUBPART_MAX; ++i) {
		OBS_OBJECT_WORK	*obj_work;
		GMS_COCKPIT_2D_WORK	*cpit_2d;
		
		obj_work	= GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK),
											  NULL,
											  0,
											  "GAME_OVER");
		cpit_2d	= (GMS_COCKPIT_2D_WORK*)obj_work;
		
		// AMAロード
		ObjObjectAction2dAMALoadSetTexlist(obj_work,
										   &cpit_2d->obj_2d,
										   NULL,//data_work
										   NULL,//filename
										   gm_over_ama_amb_idx_tbl[GsEnvGetLanguage()][GME_OVER_AMA_IDX_STRING],
										   GmGameDatGetCockpitData(),
										   AoTexGetTexList(&gm_over_textures[GME_OVER_TEX_IDX_STRING]),
										   gm_over_string_act_id_tbl[GsEnvGetLanguage()][i],
										   FALSE);
		
		// 管理ワークに設定
		mgr_work->string_sub_parts[i]	= cpit_2d;
		
		// 最初は表示しない
		gmOverSetActionHide(cpit_2d);
	}
	
	// フェードアウトサブパーツ生成
	for (Sint32 j = 0; j < GME_OVER_FADEOUT_SUBPART_MAX; ++j) {
		OBS_OBJECT_WORK	*obj_work;
		GMS_COCKPIT_2D_WORK	*cpit_2d;
		
		obj_work	= GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK),
											  NULL,
											  0,
											  "GAME_OVER");
		cpit_2d	= (GMS_COCKPIT_2D_WORK*)obj_work;
		
		// AMAロード
		ObjObjectAction2dAMALoadSetTexlist(obj_work,
										   &cpit_2d->obj_2d,
										   NULL,//data_work
										   NULL,//filename
										   gm_over_ama_amb_idx_tbl[GsEnvGetLanguage()][GME_OVER_AMA_IDX_COMMON],
										   GmGameDatGetCockpitData(),
										   AoTexGetTexList(&gm_over_textures[GME_OVER_TEX_IDX_COMMON]),
										   gm_over_fadeout_act_id_tbl[j],
										   FALSE);
		
		// 管理ワークに設定
		mgr_work->fadeout_sub_parts[j]	= cpit_2d;
		
		// 手前に表示
		obj_work->pos.z	= GMD_OVER_PART_FADEOUT_POS_Z;
		
		// リピートしない
		obj_work->disp_flag	&= ~OBD_DISP_REPEAT;
		
		// 最初は表示しない
		gmOverSetActionHide(cpit_2d);
	}
	
	
	// シーケンス初期化
	switch (type) {
	case GME_OVER_TYPE_GAMEOVER:
		gmOverProcUpdateGOInit(mgr_work);
		break;
	case GME_OVER_TYPE_TIMEOVER:
		gmOverProcUpdateTOInit(mgr_work);
		break;
#if defined(MTD_DEBUG)
	default:
		MTM_ASSERT(0);
#endif /* defined(MTD_DEBUG) */
	}
	
	// 表示関連処理設定
	mgr_work->proc_disp	= gmOverProcDispLoop;
}

// =======================================================================
// GmOverExit
/*!
  ゲーム／タイムオーバー終了
 */
// =======================================================================
void GmOverExit(void)
{
	if (gm_over_tcb) {
		mtTaskClearTcb(gm_over_tcb);
		gm_over_tcb	= NULL;
	}
}




/*------ Static Functions ----------------------------------------------*/
// ############################################################################
// 共通
// ############################################################################
// =======================================================================
// gmOverIsSkipKeyOn
/*!
  スキップするキーが押されたかチェック
  
  @retval TRUE	スキップキー押された
  @retval FALSE	スキップキー押されてない
 */
// =======================================================================
inline BOOL gmOverIsSkipKeyOn(void)
{
	// キーが押されたらスキップ
	if (AoPadDirect() & (KEY_R_UP | KEY_R_DOWN | KEY_R_RIGHT | KEY_R_LEFT)) {
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmOverSetActionHide
/*!
  アクションを停止・非表示状態にする
  
  @param cpit_2d	[io]	コックピットワーク
 */
// =======================================================================
inline void gmOverSetActionHide(GMS_COCKPIT_2D_WORK *cpit_2d)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)cpit_2d;
	
	obj_work->disp_flag	|= OBD_DISP_NODISP | OBD_DISP_NOUPDATE;
}

// =======================================================================
// gmOverSetActionPlay
/*!
  アクションを再生状態にする
  
  @param cpit_2d	[io]	コックピットワーク
 */
// =======================================================================
inline void gmOverSetActionPlay(GMS_COCKPIT_2D_WORK *cpit_2d)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)cpit_2d;
	
	obj_work->disp_flag	&= ~(OBD_DISP_NODISP | OBD_DISP_NOUPDATE);
}

// =======================================================================
// gmOverSetActionPause
/*!
  アクションを一時停止状態にする
  
  @param cpit_2d	[io]	コックピットワーク
 */
// =======================================================================
inline void gmOverSetActionPause(GMS_COCKPIT_2D_WORK *cpit_2d)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)cpit_2d;
	
	obj_work->disp_flag	&= ~OBD_DISP_NODISP;
	obj_work->disp_flag	|= OBD_DISP_NOUPDATE;
}

// ############################################################################
// 管理
// ############################################################################
// =======================================================================
// gmOverDest
/*!
  管理タスク 終了処理関数
 */
// =======================================================================
void gmOverDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
}

// =======================================================================
// gmOverMain
/*!
  管理タスク メイン処理関数
 */
// =======================================================================
void gmOverMain(MTS_TASK_TCB *tcb)
{
	GMS_OVER_MGR_WORK	*mgr_work	= (GMS_OVER_MGR_WORK*)mtTaskGetTcbWork(tcb);
	
	// 更新処理
	if (mgr_work->proc_update) {
		mgr_work->proc_update(mgr_work);
	}
	
	// 表示関連処理
	if (mgr_work->proc_disp) {
		mgr_work->proc_disp(mgr_work);
	}
}


// ============================================================================
// 更新シーケンス
// ============================================================================
// =======================================================================
// gmOverProcUpdateGO****
/*!
  ゲームオーバー演出シーケンス
 */
// =======================================================================
// ゲームオーバー更新シーケンス初期化
void gmOverProcUpdateGOInit(GMS_OVER_MGR_WORK *mgr_work)
{
	mgr_work->wait_timer	= GMD_OVER_MGR_START_WAIT_TIME;
	
	// 処理関数設定
	mgr_work->proc_update	= gmOverProcUpdateGOWaitStart;
}

// ゲームオーバー更新 開始待ち
void gmOverProcUpdateGOWaitStart(GMS_OVER_MGR_WORK *mgr_work)
{
	// 既定時間待機
	if (mgr_work->wait_timer) {
		mgr_work->wait_timer--;
	}
	else {
		// 文字パーツ演出開始
		gmOverSetActionPlay(mgr_work->string_sub_parts[GME_OVER_STRING_SUBPART_GOV_GAME]);
		gmOverSetActionPlay(mgr_work->string_sub_parts[GME_OVER_STRING_SUBPART_GOV_OVER]);
		
		// ループ表示時間
		mgr_work->wait_timer	= GMD_OVER_MGR_GAMEOVER_LOOP_DURATION;
		
		// 処理関数設定
		mgr_work->proc_update	= gmOverProcUpdateGOLoop;
	}
}

// ゲームオーバー更新 表示中停滞
void gmOverProcUpdateGOLoop(GMS_OVER_MGR_WORK *mgr_work)
{
	// キーが押されたらスキップ
	if (gmOverIsSkipKeyOn()) {
		mgr_work->wait_timer	= 0;
	}
	
	// 既定時間表示し続ける
	if (mgr_work->wait_timer) {
		mgr_work->wait_timer--;
	}
	else {
		// フェード開始
		gmOverSetActionPlay(mgr_work->fadeout_sub_parts[GME_OVER_FADEOUT_SUBPART_GOV_FOUT]);
		
		// 処理関数設定
		mgr_work->proc_update	= gmOverProcUpdateGOWaitFadeEnd;
	}
}

// ゲームオーバー更新 フェードアウト終了待ち
void gmOverProcUpdateGOWaitFadeEnd(GMS_OVER_MGR_WORK *mgr_work)
{
	OBS_OBJECT_WORK	*subpart_obj	= (OBS_OBJECT_WORK*)mgr_work->fadeout_sub_parts[GME_OVER_FADEOUT_SUBPART_GOV_FOUT];
	
	if (subpart_obj->disp_flag & OBD_DISP_END) {
		
		// イベント遷移時にクリアカラーが見えないように直ちに黒く塗りつぶす
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL,
					   IZE_FADE_TYPE_BLACK_FADEOUT,
					   1);
		
		mgr_work->proc_update	= gmOverProcUpdateGOWaitFinalizeFade;
	}
}

// ゲームオーバー更新 遷移用暗転終了待ち
void gmOverProcUpdateGOWaitFinalizeFade(GMS_OVER_MGR_WORK *mgr_work)
{
	if (IzFadeIsEnd()) {
		
		// 終了処理
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_GAMEOVER_END;
		
		mgr_work->proc_update	= NULL;
	}
}

// =======================================================================
// gmOverProcUpdateTO****
/*!
  タイムオーバー演出シーケンス
 */
// =======================================================================
// タイムオーバー更新シーケンス初期化
void gmOverProcUpdateTOInit(GMS_OVER_MGR_WORK *mgr_work)
{
	mgr_work->wait_timer	= GMD_OVER_MGR_START_WAIT_TIME;
	
	// 処理関数設定
	mgr_work->proc_update	= gmOverProcUpdateTOWaitStart;
}

// タイムオーバー更新 開始待ち
void gmOverProcUpdateTOWaitStart(GMS_OVER_MGR_WORK *mgr_work)
{
	if (mgr_work->wait_timer) {
		mgr_work->wait_timer--;
	}
	else {
		// 文字パーツ演出開始
		gmOverSetActionPlay(mgr_work->string_sub_parts[GME_OVER_STRING_SUBPART_TOV_TIME]);
		gmOverSetActionPlay(mgr_work->string_sub_parts[GME_OVER_STRING_SUBPART_TOV_OVER]);
		
		// フェード開始
		gmOverSetActionPlay(mgr_work->fadeout_sub_parts[GME_OVER_FADEOUT_SUBPART_TOV_FOUT]);
		
		// （フェードアクションに実際の尺が入ってるので、自前で表示停滞待機しない）
		
		// 処理関数設定
		mgr_work->proc_update	= gmOverProcUpdateTOWaitFadeEnd;
	}
}

// タイムオーバー更新 フェードアウト終了待ち
void gmOverProcUpdateTOWaitFadeEnd(GMS_OVER_MGR_WORK *mgr_work)
{
	OBS_OBJECT_WORK *subpart_obj	= (OBS_OBJECT_WORK*)mgr_work->fadeout_sub_parts[GME_OVER_FADEOUT_SUBPART_TOV_FOUT];
	
	if (subpart_obj->disp_flag & OBD_DISP_END) {
		
		// イベント遷移時にクリアカラーが見えないように直ちに黒く塗りつぶす
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL,
					   IZE_FADE_TYPE_BLACK_FADEOUT,
					   1);
		
		mgr_work->proc_update	= gmOverProcUpdateTOWaitFinalizeFade;
	}
}

// タイムオーバー更新 遷移用暗転終了待ち
void gmOverProcUpdateTOWaitFinalizeFade(GMS_OVER_MGR_WORK *mgr_work)
{
	if (IzFadeIsEnd()) {
		
		// 終了処理
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_GAMEOVER_END;
		
		mgr_work->proc_update	= NULL;
	}
}

// ============================================================================
// 表示関連シーケンス
// ============================================================================
// =======================================================================
// gmOverProcDispLoop
/*!
  共通表示関連シーケンス
 */
// =======================================================================
void gmOverProcDispLoop(GMS_OVER_MGR_WORK *mgr_work)
{
	UNREFERENCED_PARAMETER(mgr_work);
}


// =======================================================================
// GmOverStaticVarInit
/*!
 static変数の初期化
 */
// =======================================================================
void GmOverStaticVarInit(void)
{
	//! タスクTCB
	gm_over_tcb = NULL;
	
	//! テクスチャAMB参照ポインタリスト
	memset(gm_over_texamb_list, 0, sizeof(gm_over_texamb_list));
	//! AOテクスチャ構造体リスト
	memset(gm_over_textures, 0, sizeof(gm_over_textures));
}

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
