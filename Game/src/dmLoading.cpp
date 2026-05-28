// ===========================================================================
/*!
	@file	dmLoading.cpp
	@brief	デモ・ローディング画面

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmLoading.cpp 20 2011-04-22 12:46:46Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#include "dmLoading.h"
#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "izFade.h"
#include "aoWinSys.h"

// h
#include "common/ace/D_LOAD.HMA"

// ----- Macros ------------------------------------------------（マクロ定義）

#define DMD_LOADING_TASK_PAUSELEVEL			(0x7fff)
#define DMD_LOADING_TASK_PRIO_MAIN			(0x2000)
#define DMD_LOADING_TASK_GROUP_MAIN			(10)

#define DMD_LOADING_FILE_PATH_NUM_MAX		(60)

#define DMD_LOADING_CMN_DATA_FILENAME		(GSS_BASE_PATH"DEMO/CMN/D_CMN_WIN.AMB")
#define DMD_LOADING_DATA_FILENAME			(GSS_BASE_PATH"DEMO/OPTION/D_OPTION_USER.AMB")

#define DMD_LOADING_SIZE_WIDTH				(960.0f)
#define DMD_LOADING_SIZE_HEIGHT				(720.0f)
#define DMD_LOADING_SIZE_HALF_WIDTH			(480.0f)
#define DMD_LOADING_SIZE_HALF_HEIGHT		(360.0f)


// プライオリティ設定
#define DMD_LOADING_DRAW_PRIO_CHAR			(0x3000)
#define DMD_LOADING_DRAW_PRIO_BG			(0x2000)
#define DMD_LOADING_DRAW_PRIO_FIX			(0x2800)

// 表示
#define DMD_LOADING_DRAW_STATE_ID			(10)

// フェード関連
#define DMD_LOADING_FADEIN_TIME				(32.0f)
#define DMD_LOADING_FADEOUT_TIME			(32.0f)

// 演出関連
#define DMD_LOADING_LOAD_LOOP_TIME			(12.0f)
#define DMD_LOADING_LOADED_WAIT_TIME		(32.0f)

#define DMD_LOADING_SONIC_MOVE_INIT_SPD		(50.f)
#define DMD_LOADING_SONIC_MAIN_INIT_POS		(0.f)
#define DMD_LOADING_SONIC_OTHER_INIT_POS	(0.f)
#define DMD_LOADING_SONIC_MOVE_SPD			(12.f)
#define DMD_LOADING_SONIC_MOVE_ACCEL		(0.8f)

// フラグ関連
#define DMD_LOADING_FLAG_EXIT				(1 << 0)		//!< 終了フラグ
#define DMD_LOADING_FLAG_CANCEL				(1 << 1)		//!< キャンセル
#define DMD_LOADING_FLAG_DECIDE				(1 << 2)		//!< 決定フラグ



// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）
//!< ファイル種別
typedef enum tag_DME_LOADING_DATA_TYPE
{
	DME_LOADING_DATA_TYPE_CMN_DATA = 0,		//!< 共通データ
//	DME_LOADING_DATA_TYPE_LANG_DATA,		//!< 言語別データ
	
	DME_LOADING_DATA_TYPE_MAX,
	DME_LOADING_DATA_TYPE_NONE
} DME_LOADING_DATA_TYPE;


//! アクションテーブル(ノード含む)
typedef enum tag_DME_LOADING_ACT
{
	// 言語共通
	ACT_BG_WHITE,
	ACT_BG_BOTTOM,
	ACT_OBI,
	ACT_RUN,
//	ACT_STOP,
	ACT_PERIOD1,
	ACT_PERIOD2,
	ACT_PERIOD3,

	// 言語別
	ACT_TEXT_LOAD,
	
	ACT_NUM,
	
	ACT_NONE
} DME_LOADING_ACT;


typedef struct tag_DMS_LOADING_MAIN_WORK	DMS_LOADING_MAIN_WORK;

//! メインタスクワーク
struct tag_DMS_LOADING_MAIN_WORK {

	AMS_FS			*arc_amb;							//!< アーカイブAMBファイル
	void			*ama[DME_LOADING_DATA_TYPE_MAX];	//!< AMAファイル
	void			*amb[DME_LOADING_DATA_TYPE_MAX];	//!< AMBファイル
	
	AOS_TEXTURE		tex[DME_LOADING_DATA_TYPE_MAX];		//!< テクスチャ

	// メニュー用アクション
	AOS_ACTION 		*act[ACT_NUM];

	void (*proc_update)(DMS_LOADING_MAIN_WORK *);		//!< メニュー用プロシージャ
	void (*proc_draw)(DMS_LOADING_MAIN_WORK *);			//!< 描画用プロシージャ

	float timer;											//!< 汎用タイマー
	u32	flag;											//!< 汎用フラグ
	float efct_timer;

	float sonic_set_frame;								//!< ソニックの表示フレーム
	float sonic_pos_x;									//!< ソニックの表示位置X
	float sonic_move_spd;								//!< ソニックの移動速度
	
	BOOL is_maingame_load;								//!< メインゲームのロード時
	BOOL is_play_maingame;
	u32 draw_state;
	
	s32 lang_id;
};


//! 管理構造体
typedef struct tag_DMS_LOADING_MGR {
	MTS_TASK_TCB *tcb;	//!< TCB
} DMS_LOADING_MGR;



// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void dmLoadingInit(void);
static void dmLoadingProcMain(MTS_TASK_TCB *tcb);
static void dmLoadingDest(MTS_TASK_TCB *tcb);

// 初期化設定関連
static void dmLoadingProcInit(DMS_LOADING_MAIN_WORK *main_work);
static void dmLoadingProcCreateAct(DMS_LOADING_MAIN_WORK *main_work);

// メニュー用プロシージャ
static void dmLoadingProcStopDraw(DMS_LOADING_MAIN_WORK *main_work);

static void dmLoadingProcFadeIn(DMS_LOADING_MAIN_WORK *main_work);
static void dmLoadingProcNowLoading(DMS_LOADING_MAIN_WORK *main_work);
static void dmLoadingProcAlreadyLoaded(DMS_LOADING_MAIN_WORK *main_work);
static void dmLoadingProcFadeOut(DMS_LOADING_MAIN_WORK *main_work);

// 描画関連処理
static void dmLoadingProcActDraw(DMS_LOADING_MAIN_WORK *main_work);
static void dmLoadingCommonDraw(DMS_LOADING_MAIN_WORK *main_work);
static void dmLoadingTaskDraw(AMS_TCB* tcb);

// 演出関連設定処理


static void dmLoadingSetInitData(DMS_LOADING_MAIN_WORK *main_work);

static s32 dmLoadingIsTexLoad(void);
static s32 dmLoadingIsTexRelease(void);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）

// アクションIDテーブル(初期状態)
const static u32 g_dm_act_id_tbl[ACT_NUM] = {
	// 言語共通
	IDA_D_LOAD_ACT_BG_WHITE,
	IDA_D_LOAD_ACT_BG_BOTTOM,		// 仮
	IDA_D_LOAD_ACT_OBI,
	IDA_D_LOAD_ACT_RUN,
//	IDA_D_LOAD_ACT_STOP,
	IDA_D_LOAD_ACT_PERIOD1,
	IDA_D_LOAD_ACT_PERIOD2,
	IDA_D_LOAD_ACT_PERIOD3,

	// 言語別
	IDA_D_LOAD_ACT_TEXT_LOAD,
};


//管理情報
static DMS_LOADING_MGR *dm_loading_mgr_p = NULL;

static void *dm_loading_arc_amb;

static void *dm_loading_ama[DME_LOADING_DATA_TYPE_MAX];
static void *dm_loading_amb[DME_LOADING_DATA_TYPE_MAX];
static AOS_TEXTURE dm_loading_tex[DME_LOADING_DATA_TYPE_MAX];

static BOOL dm_loading_check_load_comp = FALSE;
static u32 dm_loading_draw_state = 0;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ==========================================================================
// DmLoadingBuild
/*!
 *	ファイル選択データ構築
  	(ファイルの読込みは呼び出し側で行い、引数でポインタを渡してこちらでデータ構築)
 */
// ==========================================================================
void DmLoadingBuild(AMS_FS *arc_amb)
{
	int i = 0;

	// 管理情報初期化
	dm_loading_mgr_p = (DMS_LOADING_MGR *)amMemAlloc(sizeof(DMS_LOADING_MGR));
	
	for (i = 0; i < DME_LOADING_DATA_TYPE_MAX; i++) {
		amZeroMemory(&dm_loading_tex[i], sizeof(AOS_TEXTURE));
	}
	
	dm_loading_arc_amb = arc_amb->buf;
	
	// AMBファイルロード
	for (i = 0; i < DME_LOADING_DATA_TYPE_MAX; i++) {
		amBindConv((u8 *)arc_amb->buf);
		
		dm_loading_ama[i] = amBindGet((AMS_AMB_HEADER*)arc_amb->buf
									  , 0
									  );
		
		dm_loading_amb[i] = amBindGet((AMS_AMB_HEADER*)arc_amb->buf
									  , 1
									  );
	}
	
	// アドレス変換
	for (i = 0; i < DME_LOADING_DATA_TYPE_MAX; i++) {
		amConvertAddress(dm_loading_ama[i]);
		amConvertAddress(dm_loading_amb[i]);
	}
	
	// テクスチャ構築
	for (i = 0; i < DME_LOADING_DATA_TYPE_MAX; i++) {
		// テクスチャ構築開始
		AoTexBuild(&dm_loading_tex[i], dm_loading_amb[i]);
		AoTexLoad(&dm_loading_tex[i]);
	}
}

// ==========================================================================
// DmLoadingBuildCheck
/*!
 *	ファイル選択データ構築 終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmLoadingBuildCheck(void)
{
	// テクスチャ構築チェック
	if (dmLoadingIsTexLoad()) {
		// フラグ扱いでON
		return (TRUE);
	}

	return (FALSE);
}


// ==========================================================================
// DmLoadingFlush
/*!
 *	ファイル選択データフラッシュ
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
void DmLoadingFlush(void)
{
	// テクスチャ解放
	for (int i = 0; i < DME_LOADING_DATA_TYPE_MAX; i++) {
		AoTexRelease(&dm_loading_tex[i]);
	}
}


// ==========================================================================
// DmLoadingFlushCheck
/*!
 *	ファイル選択データフラッシュ終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmLoadingFlushCheck(void)
{
	// テクスチャ解放
	if (dmLoadingIsTexRelease()) {
		
		if (dm_loading_arc_amb) {
			amMemFree(dm_loading_arc_amb);
			dm_loading_arc_amb = NULL;
		}
		
		if (dm_loading_mgr_p) {
			amMemFree(dm_loading_mgr_p);
			dm_loading_mgr_p = NULL;
		}
		
		return (TRUE);
	}
	
	return (FALSE);
}


// ==========================================================================
// DmLoadingStart
/*!
	ローディング画面開始処理
 */
// ==========================================================================
void DmLoadingStart(void)
{
	dmLoadingInit();
}



// ==========================================================================
// DmLoadingIsExit
/*!
	ローディング画面の終了確認処理
 */
// ==========================================================================
BOOL DmLoadingIsExit(void)
{
	if (dm_loading_mgr_p != NULL) {
		if (dm_loading_mgr_p->tcb == NULL) {
		
			return TRUE;
		}
	}
	else {
		return TRUE;
	}

	return FALSE;
}


// ==========================================================================
// DmLoadingExit
/*!
	ローディング画面の終了処理
 */
// ==========================================================================
void DmLoadingExit(void)
{
	// タスククリア
	if (dm_loading_mgr_p->tcb) {
		mtTaskClearTcb(dm_loading_mgr_p->tcb);
		
		dm_loading_mgr_p->tcb = NULL;
	}
}


// ==========================================================================
// DmLoadingSetLoadComplete
/*!
	ローディング完了設定
 */
// ==========================================================================
void DmLoadingSetLoadComplete(void)
{
	// ロード完了設定
	dm_loading_check_load_comp = TRUE;
}



// ----- Static Functions --------------------（スタティック関数の定義：局所）
// ==========================================================================
// dmLoadingInit
/*!
	ローディング画面初期化処理
 */
// ==========================================================================
void dmLoadingInit(void)
{
	DMS_LOADING_MAIN_WORK	*main_work;
	
	// メインタスク作成
	dm_loading_mgr_p->tcb = MTM_TASK_MAKE_TCB(dmLoadingProcMain
											   , dmLoadingDest
											   , 0
											   , DMD_LOADING_TASK_PAUSELEVEL
											   , DMD_LOADING_TASK_PRIO_MAIN
											   , DMD_LOADING_TASK_GROUP_MAIN
											   , sizeof(DMS_LOADING_MAIN_WORK)
											   , "LOADING_MAIN"
											   );
	
	// ワーク初期化
	main_work = (DMS_LOADING_MAIN_WORK *)mtTaskGetTcbWork(dm_loading_mgr_p->tcb);
	
	main_work->draw_state = (u32)AoActSysGetDrawStateEnable();

	AoActSysSetDrawStateEnable((s32)main_work->draw_state);
	
	if (main_work->draw_state) {
		dm_loading_draw_state = AoActSysGetDrawState();
	}

	// 初期化処理があればここに記述
	dmLoadingSetInitData(main_work);

	// プロシージャ設定
	main_work->proc_update = dmLoadingProcInit;
}



// ==========================================================================
// dmLoadingSetInitData
/*!
	セーブデータにある名前データの設定処理
 */
// ==========================================================================
void dmLoadingSetInitData(DMS_LOADING_MAIN_WORK *main_work)
{
	s16 tmp_cur_evt = 0;
	
	// 初期化関連
	dm_loading_check_load_comp = FALSE;

	// 現在ロードしているイベントがメインゲームかどうかの設定
	tmp_cur_evt = SyGetEvtInfo()->cur_evt_id;
	
	if (tmp_cur_evt == GSD_EVT_ID_MAINGAME
		|| tmp_cur_evt == GSD_EVT_ID_ENDING
		|| tmp_cur_evt == GSD_EVT_ID_SPSTAGE_BRANCH) {
		main_work->is_maingame_load = TRUE;
		main_work->sonic_pos_x = DMD_LOADING_SONIC_MAIN_INIT_POS;
	}
	else {
		main_work->is_maingame_load = FALSE;
		main_work->sonic_pos_x = DMD_LOADING_SONIC_OTHER_INIT_POS;
	}
	
	// 国別ID取得
	main_work->lang_id = (s32)GsEnvGetLanguage();
}



// ==========================================================================
// dmLoadingProcMain
/*!
	ローディング画面メインプロシージャ処理
 */
// ==========================================================================
void dmLoadingProcMain(MTS_TASK_TCB *tcb)
{
	DMS_LOADING_MAIN_WORK	*main_work;

	// ワーク取得
	main_work = (DMS_LOADING_MAIN_WORK *)mtTaskGetTcbWork(tcb);

	// 終了処理
	if (main_work->flag & DMD_LOADING_FLAG_EXIT) {
		// タスククリア
		DmLoadingExit();
		
		// イベント遷移用設定
		
	}

	// 更新処理用プロシージャ
	if (main_work->proc_update) {
		main_work->proc_update(main_work);
	}

	// 描画設定プロシージャ
	if (main_work->proc_draw) {
		main_work->proc_draw(main_work);
	}

}


// ==========================================================================
// dmLoadingDest
/*!
	ローディング画面終了処理
 */
// ==========================================================================
void dmLoadingDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
}



// ==========================================================================
// dmLoadingProcInit
/*!
	テクスチャ構築待ち処理
 */
// ==========================================================================
void dmLoadingProcInit(DMS_LOADING_MAIN_WORK *main_work)
{
	// 次へ遷移
	main_work->proc_update = dmLoadingProcCreateAct;
}


// ==========================================================================
// dmLoadingProcCreateAct
/*!
	アクション生成処理

  	※cur_fileを設定する際は必ずcrsr_idxとcur_vrtcl_fileを設定して
  	それらの和を設定すること。
 */
// ==========================================================================
void dmLoadingProcCreateAct(DMS_LOADING_MAIN_WORK *main_work)
{
	// ファイル選別
	const void *ama;
	AOS_TEXTURE *tex;
	
	// アクション構築
	for (u32 i = 0; i < ACT_NUM; ++i) {
		ama = dm_loading_ama[DME_LOADING_DATA_TYPE_CMN_DATA];
		tex = &dm_loading_tex[DME_LOADING_DATA_TYPE_CMN_DATA];
		
		// 構築
		AoActSetTexture(AoTexGetTexList(tex));
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
	}
	
	// イベント遷移
	main_work->proc_update = dmLoadingProcFadeIn;
	
	// テクスチャセットまで出来たので描画プロシージャを設定
	main_work->proc_draw = dmLoadingProcActDraw;
	
	// フェードイン開始
	if (main_work->is_maingame_load) {
#if 0
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEIN
					   , DMD_LOADING_FADEIN_TIME
					   );
#else
		IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
							, 0x7fff
							, IZD_FADE_DT_PRIO_DEF
							, IZD_FADE_DRAW_STATE_DEF
							, IZE_FADE_SET_TYPE_TAKEOEVER
							, IZE_FADE_TYPE_BLACK_FADEIN
							, DMD_LOADING_FADEIN_TIME
							, TRUE
							);
#endif
	}
	else {
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEIN
					   , DMD_LOADING_FADEIN_TIME
					   );
	}
}



// ==========================================================================
// dmLoadingProcFadeIn
/*!
	ローディング時のフェードイン中処理
 */
// ==========================================================================
void dmLoadingProcFadeIn(DMS_LOADING_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
		IzFadeExit();
		
		main_work->proc_update = dmLoadingProcNowLoading;
	}
}



// ==========================================================================
// dmLoadingProcNowLoading
/*!
	ローディング時のローディング中処理
 */
// ==========================================================================
void dmLoadingProcNowLoading(DMS_LOADING_MAIN_WORK *main_work)
{
	// ロードが完了したかどうかをチェック
	if (dm_loading_check_load_comp
		&& main_work->timer > 60.f) {
		main_work->proc_update = dmLoadingProcAlreadyLoaded;
		main_work->timer = 0.f;
		
		// フェード処理開始
		if (main_work->is_maingame_load) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_WHITE_FADEOUT
								, DMD_LOADING_FADEOUT_TIME
								, TRUE
								);
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_WHITE_FADEOUT
						   , DMD_LOADING_FADEOUT_TIME
						   );
		}
	}
	
	if (main_work->sonic_set_frame >= DMD_LOADING_LOAD_LOOP_TIME) {
		main_work->sonic_set_frame = 0.f;
	}
	
	main_work->sonic_set_frame++;
	
	main_work->timer++;
}



// ==========================================================================
// dmLoadingProcAlreadyLoaded
/*!
	ローディング終了中処理
	(ソニックが走り抜けていく演出中)
 */
// ==========================================================================
void dmLoadingProcAlreadyLoaded(DMS_LOADING_MAIN_WORK *main_work)
{
	if (main_work->timer > DMD_LOADING_LOADED_WAIT_TIME) {
		main_work->proc_update = dmLoadingProcFadeOut;
		
		main_work->timer = 0.f;
	}
	
	if (main_work->sonic_set_frame >= DMD_LOADING_LOAD_LOOP_TIME) {
		main_work->sonic_set_frame = 0.f;
	}
	
	// ソニックの移動分座標加算
	main_work->sonic_pos_x += DMD_LOADING_SONIC_MOVE_INIT_SPD;
	
	main_work->sonic_set_frame++;
	main_work->timer++;
}



// ==========================================================================
// dmLoadingProcFadeOut
/*!
	フェードアウト中処理
 */
// ==========================================================================
void dmLoadingProcFadeOut(DMS_LOADING_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
//		IzFadeExit();
		
		main_work->proc_update = dmLoadingProcStopDraw;
		main_work->proc_draw = NULL;
	}
}



// ==========================================================================
// dmLoadingProcStopDraw
/*!
	描画停止処理
 */
// ==========================================================================
void dmLoadingProcStopDraw(DMS_LOADING_MAIN_WORK *main_work)
{
	for (int i = 0; i < ACT_NUM; i++) {
		if (main_work->act[i]) {
			AoActDelete(main_work->act[i]);
			main_work->act[i] = NULL;
		}
	}
	
	main_work->proc_update = NULL;
	main_work->flag |= DMD_LOADING_FLAG_EXIT;
}




// ==========================================================================
// dmLoadingProcActDraw
/*!
	描画設定プロシージャ処理
 */
// ==========================================================================
void dmLoadingProcActDraw(DMS_LOADING_MAIN_WORK *main_work)
{
	// 共通描画処理は描画時は常に設定
	dmLoadingCommonDraw(main_work);
	
	// 描画タスク生成
	if (main_work->draw_state) {
		amDrawMakeTask(dmLoadingTaskDraw, (u16)0x8000, (u32)0);
	}
}



// ==========================================================================
// dmLoadingTaskDraw
/*!
	ローディング画面の描画タスク
 */
// ==========================================================================
void dmLoadingTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(dm_loading_draw_state);
	amDrawEndScene();
}



// ==========================================================================
// dmLoadingCommonDraw
/*!
	共通描画設定処理
 */
// ==========================================================================
void dmLoadingCommonDraw(DMS_LOADING_MAIN_WORK *main_work)
{
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_LOADING_DRAW_PRIO_BG);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_loading_tex[0]));

	// アクション登録
	for (int i = ACT_BG_WHITE; i < ACT_NUM; i++) {
		AoActSortRegAction(main_work->act[i]);
	}
	
	// ソニックアイコンフレーム設定
	AoActSetFrame(main_work->act[ACT_RUN], main_work->sonic_set_frame);
	
	// ローディング文字のローカライズ用フレーム設定
	AoActSetFrame(main_work->act[ACT_TEXT_LOAD], (f32)main_work->lang_id);
	
	// アクション更新
	AoActSetTexture(AoTexGetTexList(&dm_loading_tex[0]));

	for (int i = ACT_BG_WHITE; i <= ACT_OBI; i++) {
		AoActUpdate(main_work->act[i], 0.0f);
	}
	
	
	AoActAcmPush();
	
	AoActAcmInit();
	AoActAcmApplyTrans(main_work->sonic_pos_x
					   , 0.f
					   , 0
					   );
	
	AoActUpdate(main_work->act[ACT_RUN], 0.0f);
	
	AoActAcmPop();
	
	
	for (int i = ACT_PERIOD1; i <= ACT_PERIOD3; i++) {
		AoActUpdate(main_work->act[i], 1.0f);
	}
	
	AoActUpdate(main_work->act[ACT_TEXT_LOAD], 0.0f);

	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
}



// ==========================================================================
// dmLoadingIsTexLoad
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
s32 dmLoadingIsTexLoad(void)
{
	
	for (int i = 0; i < DME_LOADING_DATA_TYPE_MAX; i++) {
		if (!AoTexIsLoaded(&dm_loading_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}

	return 1;
}


// ==========================================================================
// dmLoadingIsTexRelease
/*!
	テクスチャ解放完了チェック処理
 */
// ==========================================================================
s32 dmLoadingIsTexRelease(void)
{
	
	for (int i = 0; i < DME_LOADING_DATA_TYPE_MAX; i++) {
		if (!AoTexIsReleased(&dm_loading_tex[i])) {
			// フラグ扱いでON
			return 0;
		}
	}

	return 1;
}


// ==========================================================================
// DmLoadingStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void DmLoadingStaticVarInit(void)
{
	dm_loading_mgr_p = NULL;
	
	dm_loading_arc_amb = NULL;
	
	memset(dm_loading_ama, 0, sizeof(dm_loading_ama));
	memset(dm_loading_amb, 0, sizeof(dm_loading_amb));
	memset(dm_loading_tex, 0, sizeof(dm_loading_tex));
	
	dm_loading_check_load_comp = FALSE;
	dm_loading_draw_state = 0;
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
