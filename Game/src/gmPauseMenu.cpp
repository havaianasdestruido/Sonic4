// ===========================================================================
/*!
	@file	gmPauseMenu.cpp
	@brief	アクト中ポーズメニューUIモジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#if !_IPHONE

#include "gmPauseMenu.h"
#include "ao.h"

#include "gmSound.h"
#include "objDraw.h"

#include "gs.h"
#include "gsMainSys.h"
#include "gsBackup.hpp"

#include "ace/g_pause.hma"
#include "ace/g_pause_l.hma"
#include "ace/g_pause_p.hma"
#include "ace/g_pause_p_l.hma"


// ----- Macros ------------------------------------------------（マクロ定義）

// 定数
#define GMD_PMENU_EFCT_IN_TIME		(24)	//!< 入り演出時間
#define GMD_PMENU_EFCT_OUT_TIME		(24)	//!< 退出演出時間
#define GMD_PMENU_EFCT_WIN_TIME		(8)		//!< ウインドウ演出時間
#define GMD_PMENU_EFCT_DECIDE_TIME	(12)	//!< 決定演出時間

#define GMD_PMENU_SCREEN_W			(960)	//!< スクリーン横サイズ
#define GMD_PMENU_SCREEN_H			(720)	//!< スクリーン縦サイズ

#if !_WII
#define GMD_PMENU_WIN_W				(430)	//!< ウインドウ横サイズ
#define GMD_PMENU_WIN_H				(196)	//!< ウインドウ縦サイズ
#else
#define GMD_PMENU_WIN_W				(645)	//!< ウインドウ横サイズ
#define GMD_PMENU_WIN_H				(280)	//!< ウインドウ縦サイズ
#endif // !_WII

#define GMD_PMENU_USE_DSTATE		(1)		//!< ステート描画使用
#define GMD_PMENU_DRAW_STATE		(4)		//!< 描画ステート

#if _PS3 || _WII
//! パッド切断対応有効(無効にする場合はコメントアウト)
#define GMD_PMENU_DC_ENABLE			(1)
#endif // _PS3 || _WII

#if !_WII
#define GMD_PMENU_DCMSG_W			(430)	//!< パッド切断メッセージ横サイズ
#define GMD_PMENU_DCMSG_H			(128)	//!< パッド切断メッセージ縦サイズ
#else
#define GMD_PMENU_DCMSG_W			(645)	//!< パッド切断メッセージ横サイズ
#define GMD_PMENU_DCMSG_H			(192)	//!< パッド切断メッセージ縦サイズ
#endif // !_WII

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）

// ===========================================================================
//	enum GME_PMENU_STATE
// ---------------------------------------------------------------------------
//!	ポーズメニュー状態列挙
// ===========================================================================
typedef enum tag_GME_PMENU_STATE {
	GMD_PMENU_STATE_LOADING		= 0,	//!< ファイル読み込み中
	GMD_PMENU_STATE_LOADED,				//!< ファイル読み込み済み
	GMD_PMENU_STATE_BUILDING,			//!< 構築中
	GMD_PMENU_STATE_BUILDED,			//!< 構築完了
	GMD_PMENU_STATE_EXECUTE,			//!< 実行中
	GMD_PMENU_STATE_FLUSHING,			//!< 解放中
	GMD_PMENU_STATE_RELEASED,			//!< ファイル解放済み

	GMD_PMENU_STATE_NUM,				//!< 状態数
	GMD_PMENU_STATE_NONE,				//!< 無効コード
} GME_PMENU_STATE;

// ===========================================================================
//	enum GME_PMENU_ACT
// ---------------------------------------------------------------------------
//!	アクション列挙
// ===========================================================================
typedef enum tag_GME_PMENU_ACT {
	ACT_TITLE_BASE			= 0,	//!< タイトル土台

#if _PC || _XBOX
	ACT_BTN_BASE1,					//!< ボタン土台
	ACT_BTN_BASE1_S,				//!< ボタン土台(選択)
	ACT_BTN_BASE2,					//!< ボタン土台
	ACT_BTN_BASE2_S,				//!< ボタン土台(選択)
	ACT_BTN_BASE3,					//!< ボタン土台
	ACT_BTN_BASE3_S,				//!< ボタン土台(選択)
	ACT_BTN_BASE4,					//!< ボタン土台
	ACT_BTN_BASE4_S,				//!< ボタン土台(選択)
	ACT_BTN_BASE5,					//!< ボタン土台
	ACT_BTN_BASE5_S,				//!< ボタン土台(選択)
	ACT_BTN_BASE6,					//!< ボタン土台
	ACT_BTN_BASE6_S,				//!< ボタン土台(選択)
#elif _PS3
	ACT_BTN_BASE1,					//!< ボタン土台
	ACT_BTN_BASE1_S,				//!< ボタン土台(選択)
	ACT_BTN_BASE2,					//!< ボタン土台
	ACT_BTN_BASE2_S,				//!< ボタン土台(選択)
	ACT_BTN_BASE3,					//!< ボタン土台
	ACT_BTN_BASE3_S,				//!< ボタン土台(選択)
#else
	ACT_BTN_BASE1,					//!< ボタン土台
	ACT_BTN_BASE1_S,				//!< ボタン土台(選択)
	ACT_BTN_BASE2,					//!< ボタン土台
	ACT_BTN_BASE2_S,				//!< ボタン土台(選択)
#endif
	ACT_BTN_BACK,					//!< ボタン背景

	ACT_L_TITLE,					//!< タイトル
	ACT_L_BTN_BACK,					//!< 戻るボタン
	ACT_L_DLG_YES,					//!< ダイアログ選択「はい」
	ACT_L_DLG_YES_S,				//!< ダイアログ選択「はい」(選択)
	ACT_L_DLG_NO,					//!< ダイアログ選択「いいえ」
	ACT_L_DLG_NO_S,					//!< ダイアログ選択「いいえ」(選択)
	ACT_L_MSG_RETRY,				//!< リトライ確認メッセージ
	ACT_L_MSG_WOLRD,				//!< ワールド戻り確認メッセージ
	ACT_L_MSG_MMENU,				//!< メインメニュー戻り確認メッセージ

	ACT_P_BACK_MARK_O,				//!< 戻るボタンマークO
	ACT_P_BACK_MARK_X,				//!< 戻るボタンマークX

	ACT_PL_BTN_RETRY,				//!< リトライボタン
	ACT_PL_BTN_RETRY_S,				//!< リトライボタン(選択)
#if !_WII
	ACT_PL_BTN_OPTION,				//!< オプションボタン
	ACT_PL_BTN_OPTION_S,			//!< オプションボタン(選択)
#endif // !_WII
#if _PC || _XBOX
	ACT_PL_BTN_RANK,				//!< ランキングボタン
	ACT_PL_BTN_RANK_S,				//!< ランキングボタン(選択)
	ACT_PL_BTN_TROPHY,				//!< 実績ボタン
	ACT_PL_BTN_TROPHY_S,			//!< 実績ボタン(選択)
	ACT_PL_BTN_RESUME,				//!< ゲームに戻るボタン
	ACT_PL_BTN_RESUME_S,			//!< ゲームに戻るボタン(選択)
#endif // _PC || _XBOX
	ACT_PL_BTN_WORLD,				//!< ワールドマップへ戻るボタン
	ACT_PL_BTN_WORLD_S,				//!< ワールドマップへ戻るボタン(選択)

	ACT_NUM,						//!< アクション数
	ACT_NONE,						//!< 無効コード

	ACT_CMN_S = ACT_TITLE_BASE,		//!< 共通開始位置
	ACT_LNG_S = ACT_L_TITLE,		//!< 言語別開始位置
	ACT_PLF_S = ACT_P_BACK_MARK_O,	//!< プラットフォーム別開始位置
	ACT_PLNG_S = ACT_PL_BTN_RETRY,	//!< プラットフォーム＆言語別開始位置
} GME_PMENU_ACT;

// ===========================================================================
//	struct GME_PMENU_SE
// ---------------------------------------------------------------------------
//!	ポーズメニューSE列挙
// ===========================================================================
typedef enum tag_GME_PMENU_SE {
	GMD_PMENU_SE_CURSOR		= 0,	//!< カーソル移動
	GMD_PMENU_SE_DECIDE,			//!< 決定
	GMD_PMENU_SE_CANCEL,			//!< キャンセル
	GMD_PMENU_SE_START,				//!< 起動

	GMD_PMENU_SE_NUM,				//!< SE数
	GMD_PMENU_SE_NONE,				//!< 無効コード
} GME_PMENU_SE;

// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct GMS_PMENU_GLOBAL
// ---------------------------------------------------------------------------
//!	ポーズメニュー用グローバルワーク
// ===========================================================================
typedef struct tag_GMS_PMENU_GLOBAL {
	GME_PMENU_STATE		state;		//!< 状態
	AMS_TCB*			tcb;		//!< タスクTCBポインタ
	GME_PMENU_RESULT	result;		//!< 結果

	// テクスチャ
	AOS_TEXTURE		tex_cmn;		//!< 共通テクスチャ
	AOS_TEXTURE		tex_lng;		//!< 言語別テクスチャ
	AOS_TEXTURE		tex_plf;		//!< プラットフォーム別テクスチャ
	AOS_TEXTURE		tex_plng;		//!< プラットフォーム＆言語別テクスチャ
	AOS_TEXTURE		tex_win;		//!< ウインドウテクスチャ
#if defined(GMD_PMENU_DC_ENABLE)
	AOS_TEXTURE		tex_dcmsg;		//!< パッド切断メッセージテクスチャ
#endif // defined(GMD_PMENU_DC_ENABLE)

	// ファイル
	void*			file_cmn_ama;	//!< 共通AMAファイル
	void*			file_cmn_amb;	//!< 共通AMBファイル
	void*			file_lng_ama;	//!< 言語別AMAファイル
	void*			file_lng_amb;	//!< 言語別AMBファイル
	void*			file_plf_ama;	//!< プラットフォーム別AMAファイル
	void*			file_plf_amb;	//!< プラットフォーム別AMBファイル
	void*			file_plng_ama;	//!< プラットフォーム＆言語別AMAファイル
	void*			file_plng_amb;	//!< プラットフォーム＆言語別AMBファイル
	void*			file_win_amb;	//!< ウインドウAMBファイル
#if defined(GMD_PMENU_DC_ENABLE)
	void*			file_dcmsg_amb;	//!< パッド切断メッセージAMBファイル
#endif // defined(GMD_PMENU_DC_ENABLE)
	AMS_FS*			fs;				//!< ファイル読み込みFS

	GSS_SND_SE_HANDLE*	se_handle;	//!< SE再生用ハンドル
} GMS_PMENU_GLOBAL;

// ===========================================================================
//	struct GMS_PMENU_WORK
// ---------------------------------------------------------------------------
//!	ポーズメニューワーク
// ===========================================================================
typedef struct tag_GMS_PMENU_WORK {
	u32					select;			//!< 選択番号
	u32					select_sub;		//!< サブ選択番号
	GME_PMENU_RESULT	result;			//!< 結果
	AOS_ACTION*			act[ACT_NUM];	//!< アクション配列
} GMS_PMENU_WORK;

// ===========================================================================
//	struct GMS_PMENU_DRAW_WORK
// ---------------------------------------------------------------------------
//!	ポーズメニュー描画タスクワーク
// ===========================================================================
typedef struct tag_GMS_PMENU_DRAW_WORK {
	NNS_TEXLIST*		tex_win;		//!< ウインドウテクスチャ
	NNS_TEXLIST*		tex_msg;		//!< メッセージテクスチャ
	s32					ivalue;			//!< 整数値
	f32					fvalue;			//!< 浮動少数値
} GMS_PMENU_DRAW_WORK;

// ===========================================================================
//	struct GMS_PMENU_TASK_WORK
// ---------------------------------------------------------------------------
//!	ポーズメニュータスクワーク
// ===========================================================================
typedef struct tag_GMS_PMENU_TASK_WORK {
	GMS_PMENU_WORK*		work;			//!< ワーク
	u32					proc_count;		//!< プロシージャカウンタ
	void (*proc)(GMS_PMENU_WORK*);		//!< プロシージャ
#if defined(GMD_PMENU_DC_ENABLE)
	u32					dc_count;		//!< パッド切断カウンタ
	u32					cproc_count;	//!< パッド切断プロシージャカウンタ
	void (*cproc)(GMS_PMENU_WORK*);		//!< パッド切断プロシージャ
#endif // defined(GMD_PMENU_DC_ENABLE)
} GMS_PMENU_TASK_WORK;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// タスク
static void gmPmTask00(AMS_TCB* tcb);
static void gmPmTask01(AMS_TCB* tcb);
static void gmPmTaskDestructor(AMS_TCB* tcb);
static u32 gmPmGetProcCount(void);
static void gmPmSetProc(void (*proc)(GMS_PMENU_WORK*));
#if defined(GMD_PMENU_DC_ENABLE)
static u32& gmPmGetDcCount(void);
static u32 gmPmGetCProcCount(void);
static void gmPmSetCProc(void (*proc)(GMS_PMENU_WORK*));
#endif // defined(GMD_PMENU_DC_ENABLE)

// プロシージャ
static void gmPmProcBegin(GMS_PMENU_WORK* work);
static void gmPmProcEfctIn(GMS_PMENU_WORK* work);
static void gmPmProcSelect(GMS_PMENU_WORK* work);
static void gmPmProcDecide(GMS_PMENU_WORK* work);
static void gmPmProcEfctOut(GMS_PMENU_WORK* work);
static void gmPmProcRetryIn(GMS_PMENU_WORK* work);
static void gmPmProcRetrySelect(GMS_PMENU_WORK* work);
static void gmPmProcRetryDecide(GMS_PMENU_WORK* work);
static void gmPmProcRetryOut(GMS_PMENU_WORK* work);
static void gmPmProcBackIn(GMS_PMENU_WORK* work);
static void gmPmProcBackSelect(GMS_PMENU_WORK* work);
static void gmPmProcBackDecide(GMS_PMENU_WORK* work);
static void gmPmProcBackOut(GMS_PMENU_WORK* work);
static void gmPmProcMainMenuIn(GMS_PMENU_WORK* work);
static void gmPmProcMainMenuSelect(GMS_PMENU_WORK* work);
static void gmPmProcMainMenuDecide(GMS_PMENU_WORK* work);
static void gmPmProcMainMenuOut(GMS_PMENU_WORK* work);
static void gmPmProcEnd(GMS_PMENU_WORK* work);

// 描画
static void gmPmDrawEfctIn(GMS_PMENU_WORK* work, f32 rate);
static void gmPmDrawSelect(GMS_PMENU_WORK* work, u32 select);
static f32 gmPmDrawGetSelShowPos(u32 i);
#if GMD_PMENU_USE_DSTATE
static void gmPmDrawWindowEfctPrev(void* param);
#endif // GMD_PMENU_USE_DSTATE
static void gmPmDrawWindowEfct(GMS_PMENU_WORK* work, f32 rate);
static void gmPmDrawWindowMessage(
	GMS_PMENU_WORK* work, u32 act_no, u32 select);

#if defined(GMD_PMENU_DC_ENABLE)
// パッド切断プロシージャ
static void gmPmCProcEfctIn(GMS_PMENU_WORK* work);
static void gmPmCProcMessage(GMS_PMENU_WORK* work);
static void gmPmCProcEfctOut(GMS_PMENU_WORK* work);

// パッド切断用描画
static void gmPmCDrawBlack(GMS_PMENU_WORK* work, u8 alpha);
static void gmPmCDrawEfctIn(GMS_PMENU_WORK* work, f32 rate);
static void gmPmCDrawMessage(GMS_PMENU_WORK* work);
static void gmPmCDrawTaskBlack(AMS_TCB* tcb);
static void gmPmCDrawTaskEfctIn(AMS_TCB* tcb);
static void gmPmCDrawTaskMessage(AMS_TCB* tcb);
static void gmPmCDrawPre(void);
static void gmPmCDrawPost(void);
#endif // defined(GMD_PMENU_DC_ENABLE)

// ユーティリティ
static void gmPmUtilSetTexture(u32 act_no);

// サウンド
static void gmPmSndPlaySe(GME_PMENU_SE se);

// パッド入力
static u16 gmPmPadStand(void);
static u16 gmPmPadMStand(void);
static u16 gmPmPadMRepeat(void);
static BOOL gmPmPadIsDisable(void);

// 前処理 & 後処理
static void gmPmLoadTaskWait(AMS_TCB* tcb);
static void gmPmBuildTaskWait(AMS_TCB* tcb);
static void gmPmFlushTaskWait(AMS_TCB* tcb);

// 便利
static GMS_PMENU_GLOBAL* gmPmGetGlbWork(void);
static AMS_FS* gmPmReadRequest(const char* name);
static BOOL gmPmGlbIsRetryEnable(void);
static BOOL gmPmGlbIsBackEnable(void);
static BOOL gmPmGlbIsSpecialStage(void);

#if defined(MTD_DEBUG)
// デバッグ
static void gmPmDgbEvTaskWaitStart(AMS_TCB* tcb);
static void gmPmDbgEvTaskWaitLoad(AMS_TCB* tcb);
static void gmPmDbgEvTaskWaitBuild(AMS_TCB* tcb);
static void gmPmDbgEvTaskWaitExecute(AMS_TCB* tcb);
static void gmPmDbgEvTaskExecute(AMS_TCB* tcb);
static void gmPmDbgEvTaskWaitFlush(AMS_TCB* tcb);
static void gmPmDbgEvTaskPre(AMS_TCB* tcb);
static void gmPmDbgEvTaskPost(AMS_TCB* tcb);
static void gmPmDbgEvTaskDraw(AMS_TCB* tcb);
#endif // defined(MTD_DEBUG)

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! ファイルディレクトリパス
// ===========================================================================
static const char* g_gm_pm_file_dir_path = GSS_BASE_PATH "G_COM/MENU/";

// ===========================================================================
//! 共通AMAファイル名
// ===========================================================================
static const char* g_gm_pm_file_cmn_ama_name = "G_PAUSE.AMA";

// ===========================================================================
//! 共通AMBファイル名
// ===========================================================================
static const char* g_gm_pm_file_cmn_amb_name = "G_PAUSE.AMB";

// ===========================================================================
//! プラットフォーム別AMAファイル名
// ===========================================================================
static const char* g_gm_pm_file_plf_ama_name = "G_PAUSE_P.AMA";

// ===========================================================================
//! プラットフォーム別AMBファイル名
// ===========================================================================
static const char* g_gm_pm_file_plf_amb_name = "G_PAUSE_P.AMB";

// ===========================================================================
//! 言語別AMAファイル名
// ===========================================================================
static const char* g_gm_pm_file_lng_ama_name = "G_PAUSE_L.AMA";

// ===========================================================================
//! 言語別AMBファイル名配列
// ===========================================================================
static const char* g_gm_pm_file_lng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	"G_PAUSE_JP.AMB",
	"G_PAUSE_US.AMB",
	"G_PAUSE_FR.AMB",
	"G_PAUSE_IT.AMB",
	"G_PAUSE_GE.AMB",
	"G_PAUSE_SP.AMB",
};

// ===========================================================================
//! プラットフォーム＆言語別AMAファイル名
// ===========================================================================
static const char* g_gm_pm_file_plng_ama_name = "G_PAUSE_P_L.AMA";

// ===========================================================================
//! プラットフォーム＆言語別AMBファイル名配列
// ===========================================================================
static const char* g_gm_pm_file_plng_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	"G_PAUSE_P_JP.AMB",
	"G_PAUSE_P_US.AMB",
	"G_PAUSE_P_FR.AMB",
	"G_PAUSE_P_IT.AMB",
	"G_PAUSE_P_GE.AMB",
	"G_PAUSE_P_SP.AMB",
};

// ===========================================================================
//! ウインドウAMBファイル名
// ===========================================================================
static const char* g_gm_pm_file_win_amb_name = "G_PAUSE_WIN.AMB";

#if defined(GMD_PMENU_DC_ENABLE)
// ===========================================================================
//! パッド切断メッセージAMBファイル名配列
// ===========================================================================
static const char* g_gm_pm_file_dc_msg_amb_name_tbl[GSD_LANGUAGE_NUM] = {
	"G_PAUSE_DC_MSG_JP.AMB",
	"G_PAUSE_DC_MSG_US.AMB",
	"G_PAUSE_DC_MSG_FR.AMB",
	"G_PAUSE_DC_MSG_IT.AMB",
	"G_PAUSE_DC_MSG_GE.AMB",
	"G_PAUSE_DC_MSG_SP.AMB",
};
#endif // defined(GMD_PMENU_DC_ENABLE)

// ===========================================================================
//! アクションID配列
// ===========================================================================
static const u32 g_gm_pm_act_id_tbl[ACT_NUM] = {
	IDA_G_PAUSE_ACT_TITLE_BASE,
#if _PC || _XBOX
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
#elif _PS3
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
#else
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
	IDA_G_PAUSE_ACT_BTN_BASE,
	IDA_G_PAUSE_ACT_BTN_BASE_S,
#endif
	IDA_G_PAUSE_ACT_BACK,

	IDA_G_PAUSE_L_ACT_TITLE_PAUSE,
	IDA_G_PAUSE_L_ACT_BTN_BACK,
	IDA_G_PAUSE_L_ACT_DLG_SEL_YES,
	IDA_G_PAUSE_L_ACT_DLG_SEL_YES_S,
	IDA_G_PAUSE_L_ACT_DLG_SEL_NO,
	IDA_G_PAUSE_L_ACT_DLG_SEL_NO_S,
	IDA_G_PAUSE_L_ACT_MSG_RETRY,
	IDA_G_PAUSE_L_ACT_MSG_WORLD,
	IDA_G_PAUSE_L_ACT_MSG_MAINMENU,

#if _PC || _XBOX
	IDA_G_PAUSE_P_ACT_MARK_BACK,
	IDA_G_PAUSE_P_ACT_MARK_BACK,
#else
	IDA_G_PAUSE_P_ACT_MARK_BACK_O,
	IDA_G_PAUSE_P_ACT_MARK_BACK_X,
#endif // _PC || _XBOX

	IDA_G_PAUSE_P_L_ACT_BTN_RETRY,
	IDA_G_PAUSE_P_L_ACT_BTN_RETRY_S,
#if !_WII
	IDA_G_PAUSE_P_L_ACT_BTN_OPTION,
	IDA_G_PAUSE_P_L_ACT_BTN_OPTION_S,
#endif // !_WII
#if _PC || _XBOX
	IDA_G_PAUSE_P_L_ACT_BTN_RANK,
	IDA_G_PAUSE_P_L_ACT_BTN_RANK_S,
	IDA_G_PAUSE_P_L_ACT_BTN_TROPHY,
	IDA_G_PAUSE_P_L_ACT_BTN_TROPHY_S,
	IDA_G_PAUSE_P_L_ACT_BTN_RESUME,
	IDA_G_PAUSE_P_L_ACT_BTN_RESUME_S,
#endif // _PC || _XBOX
	IDA_G_PAUSE_P_L_ACT_BTN_WORLD,
	IDA_G_PAUSE_P_L_ACT_BTN_WORLD_S,
};

// ===========================================================================
//	f32 g_gm_pmenu_bb_width_tbl[]
// ---------------------------------------------------------------------------
//!	戻るボタン+文字の幅配列
// ===========================================================================
static const f32 g_gm_pmenu_bb_width_tbl[GSD_LANGUAGE_NUM] = {
	74.0f, 90.0f, 106.0f, 114.0f, 108.0f, 92.0f
};

// ===========================================================================
//	GMS_PMENU_GLOBAL g_gm_pmenu_work
// ---------------------------------------------------------------------------
//!	グローバルワーク
// ===========================================================================
static GMS_PMENU_GLOBAL g_gm_pmenu_work = {
	GMD_PMENU_STATE_RELEASED, NULL, GME_PMENU_RESULT_NONE,
	{ 0 }, { 0 }, { 0 }, { 0 }, { 0 },
#if defined(GMD_PMENU_DC_ENABLE)
	{ 0 },
#endif // defined(GMD_PMENU_DC_ENABLE)
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
#if defined(GMD_PMENU_DC_ENABLE)
	NULL,
#endif // defined(GMD_PMENU_DC_ENABLE)
	NULL,
	NULL,
};

#if defined(MTD_DEBUG)
// ===========================================================================
//	BOOL g_gm_pmenu_debug_se_disable
// ---------------------------------------------------------------------------
//!	デバッグ用SE無効フラグ
// ===========================================================================
static BOOL g_gm_pmenu_debug_se_disable = FALSE;
#endif // defined(MTD_DEBUG)

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ***************************************************************************
// リソース管理
// ***************************************************************************
// ===========================================================================
//	GmPauseMenuLoadStart
/*!
	リソースファイル読み込み開始

	@note
	ポーズメニューに必要なファイルの読み込みを開始します。\n
	この関数は即時復帰となります。\n
	完了判定はGmPauseMenuLoadIsFinished関数で行うようにして下さい。\n
	既に読み込みを開始している場合に呼び出すと、アサートし何も行いません。\n
	GmPauseMenuLoadIsFinished関数がTRUEを返す状態で呼び出すと、
	アサートし何も行いません。\n
*/
// ===========================================================================
void GmPauseMenuLoadStart(void)
{
#if defined(MTD_DEBUG)
	g_gm_pmenu_debug_se_disable = FALSE;
#endif // defined(MTD_DEBUG)

	// ワーク取得
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();

	// 読み込み済み判定
	if (GmPauseMenuLoadIsFinished()) {
		amAssert(0);
		return;
	}

	// 読み込み中判定
	if (work->state == GMD_PMENU_STATE_LOADING) {
		amAssert(0);
		return;
	}

	// チェック
	amAssert(work->state == GMD_PMENU_STATE_RELEASED);
	amAssert(work->tcb == NULL);
	amAssert(work->file_cmn_ama == NULL);
	amAssert(work->file_cmn_amb == NULL);
	amAssert(work->file_lng_ama == NULL);
	amAssert(work->file_lng_amb == NULL);
	amAssert(work->file_plf_ama == NULL);
	amAssert(work->file_plf_amb == NULL);
	amAssert(work->file_plng_ama == NULL);
	amAssert(work->file_plng_amb == NULL);
	amAssert(work->file_win_amb == NULL);
#if defined(GMD_PMENU_DC_ENABLE)
	amAssert(work->file_dcmsg_amb == NULL);
#endif // defined(GMD_PMENU_DC_ENABLE)
	amAssert(work->fs == NULL);

	// 初期化
	work->file_cmn_ama = NULL;
	work->file_cmn_amb = NULL;
	work->file_lng_ama = NULL;
	work->file_lng_amb = NULL;
	work->file_plf_ama = NULL;
	work->file_plf_amb = NULL;
	work->file_plng_ama = NULL;
	work->file_plng_amb = NULL;
	work->file_win_amb = NULL;
#if defined(GMD_PMENU_DC_ENABLE)
	work->file_dcmsg_amb = NULL;
#endif // defined(GMD_PMENU_DC_ENABLE)
	work->fs = NULL;
	work->se_handle = NULL;

	// 状態設定
	work->state = GMD_PMENU_STATE_LOADING;

	// ファイル読み込み開始
	work->fs = gmPmReadRequest(g_gm_pm_file_cmn_ama_name);

	// ファイル読み込み待ちタスク作成
	work->tcb = amTaskMake(
		gmPmLoadTaskWait, NULL, 0, 0, 0, "gmPauseMenu::Load");

	// ファイル読み込み待ちタスク開始
	amTaskStart(work->tcb);
}

// ===========================================================================
//	GmPauseMenuLoadIsFinished
/*!
	リソースファイル読み込み完了判定

	@return 真：完了済み　偽：それ以外
	@note
	GmPauseMenuLoadStart関数で開始したファイル読み込みが
	完了したかどうかを判定します。\n
	GmPauseMenuLoadStart関数を呼び出す前と、
	GmPauseMenuRelease関数を呼び出した後は、は常にFALSEを返します。\n
*/
// ===========================================================================
BOOL GmPauseMenuLoadIsFinished(void)
{
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();
	if ((work->state == GMD_PMENU_STATE_RELEASED) ||
		(work->state == GMD_PMENU_STATE_LOADING))
	{
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	GmPauseMenuBuildStart
/*!
	リソース構築開始

	@note
	ポーズメニューに必要なリソースの構築を開始します。\n
	この関数は即時復帰となります。\n
	完了判定はGmPauseMenuBuildIsFinished関数で行うようにして下さい。\n
	GmPauseMenuLoadIsFinished関数がFALSEを返す状態では
	呼び出すことはできません。\n
	(呼び出した場合はアサートし何も行いません。)\n
	既に構築を開始している場合に呼び出すと、アサートし何も行いません。\n
	GmPauseMenuBuildIsFinished関数がTRUEを返す状態で呼び出すと、
	アサートし何も行いません。\n
*/
// ===========================================================================
void GmPauseMenuBuildStart(void)
{
	// ワーク取得
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();

	// ファイル未読み込み判定
	if (!GmPauseMenuLoadIsFinished()) {
		amAssert(0);
		return;
	}

	// 構築済み判定
	if (GmPauseMenuBuildIsFinished()) {
		amAssert(0);
		return;
	}

	// 構築中判定
	if (work->state == GMD_PMENU_STATE_BUILDING) {
		amAssert(0);
		return;
	}

	// チェック
	amAssert(work->state == GMD_PMENU_STATE_LOADED);
	amAssert(work->tcb == NULL);
	amAssert(work->file_cmn_ama != NULL);
	amAssert(work->file_cmn_amb != NULL);
	amAssert(work->file_lng_ama != NULL);
	amAssert(work->file_lng_amb != NULL);
	amAssert(work->file_plf_ama != NULL);
	amAssert(work->file_plf_amb != NULL);
	amAssert(work->file_plng_ama != NULL);
	amAssert(work->file_plng_amb != NULL);
	amAssert(work->file_win_amb != NULL);
#if defined(GMD_PMENU_DC_ENABLE)
	amAssert(work->file_dcmsg_amb != NULL);
#endif // defined(GMD_PMENU_DC_ENABLE)
	amAssert(work->fs == NULL);

	// 状態設定
	work->state = GMD_PMENU_STATE_BUILDING;

	// 構築開始
	AoTexBuild(&work->tex_cmn, work->file_cmn_amb);
	AoTexLoad(&work->tex_cmn);
	AoTexBuild(&work->tex_lng, work->file_lng_amb);
	AoTexLoad(&work->tex_lng);
	AoTexBuild(&work->tex_plf, work->file_plf_amb);
	AoTexLoad(&work->tex_plf);
	AoTexBuild(&work->tex_plng, work->file_plng_amb);
	AoTexLoad(&work->tex_plng);
	AoTexBuild(&work->tex_win, work->file_win_amb);
	AoTexLoad(&work->tex_win);
#if defined(GMD_PMENU_DC_ENABLE)
	AoTexBuild(&work->tex_dcmsg, work->file_dcmsg_amb);
	AoTexLoad(&work->tex_dcmsg);
#endif // defined(GMD_PMENU_DC_ENABLE)

	// 構築待ちタスク作成
	work->tcb = amTaskMake(
		gmPmBuildTaskWait, NULL, 0, 0, 0, "gmPauseMenu::Build");

	// 構築待ちタスク開始
	amTaskStart(work->tcb);
}

// ===========================================================================
//	GmPauseMenuBuildIsFinished
/*!
	リソース構築完了判定

	@return 真：完了済み　偽：それ以外
	@note
	GmPauseMenuBuildStart関数で開始した構築処理が
	完了したかどうかを判定します。\n
	GmPauseMenuBuildStart関数を呼び出す前と、
	GmPauseMenuFlushStart関数を呼び出した後は、は常にFALSEを返します。\n
*/
// ===========================================================================
BOOL GmPauseMenuBuildIsFinished(void)
{
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();
	if ((work->state == GMD_PMENU_STATE_BUILDED) ||
		(work->state == GMD_PMENU_STATE_EXECUTE))
	{
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	GmPauseMenuFlushStart
/*!
	リソース解放開始

	@note
	ポーズメニューに必要なリソースの解放を開始します。\n
	この関数は即時復帰となります。\n
	完了判定はGmPauseMenuFlushIsFinished関数で行うようにして下さい。\n
	GmPauseMenuBuildIsFinished関数がFALSEを返す状態では
	呼び出すことはできません。\n
	(呼び出した場合はアサートし何も行いません。)\n
	既に解放を開始している場合に呼び出すと、アサートし何も行いません。\n
	GmPauseMenuFlushIsFinished関数がTRUEを返す状態で呼び出すと、
	アサートし何も行いません。\n
*/
// ===========================================================================
void GmPauseMenuFlushStart(void)
{
	// ワーク取得
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();

	// 未構築判定
	if (!GmPauseMenuBuildIsFinished()) {
		amAssert(0);
		return;
	}

	// 実行中判定
	if (!GmPauseMenuIsFinished()) {
		amAssert(0);
		// 強制キャンセルし続行
		GmPauseMenuCancel();
	}

	// 解放済み判定
	if (GmPauseMenuFlushIsFinished()) {
		amAssert(0);
		return;
	}

	// 解放中判定
	if (work->state == GMD_PMENU_STATE_FLUSHING) {
		amAssert(0);
		return;
	}

	// チェック
	amAssert(work->state == GMD_PMENU_STATE_BUILDED);
	amAssert(work->tcb == NULL);
	amAssert(work->file_cmn_ama != NULL);
	amAssert(work->file_cmn_amb != NULL);
	amAssert(work->file_lng_ama != NULL);
	amAssert(work->file_lng_amb != NULL);
	amAssert(work->file_plf_ama != NULL);
	amAssert(work->file_plf_amb != NULL);
	amAssert(work->file_plng_ama != NULL);
	amAssert(work->file_plng_amb != NULL);
	amAssert(work->file_win_amb != NULL);
#if defined(GMD_PMENU_DC_ENABLE)
	amAssert(work->file_dcmsg_amb != NULL);
#endif // defined(GMD_PMENU_DC_ENABLE)
	amAssert(work->fs == NULL);

	// 状態設定
	work->state = GMD_PMENU_STATE_FLUSHING;

	// 解放開始
	AoTexRelease(&work->tex_cmn);
	AoTexRelease(&work->tex_lng);
	AoTexRelease(&work->tex_plf);
	AoTexRelease(&work->tex_plng);
	AoTexRelease(&work->tex_win);
#if defined(GMD_PMENU_DC_ENABLE)
	AoTexRelease(&work->tex_dcmsg);
#endif // defined(GMD_PMENU_DC_ENABLE)

	// 解放待ちタスク作成
	work->tcb = amTaskMake(
		gmPmFlushTaskWait, NULL, 0, 0, 0, "gmPauseMenu::Flush");

	// 構築待ちタスク開始
	amTaskStart(work->tcb);
}

// ===========================================================================
//	GmPauseMenuFlushIsFinished
/*!
	リソース解放完了判定

	@return 真：完了済み　偽：それ以外
	@note
	GmPauseMenuFlushStart関数で開始した解放処理が
	完了したかどうかを判定します。\n
	GmPauseMenuFlushStart関数を呼び出す前と、
	GmPauseMenuBuildStart関数を呼び出した後は、は常にFALSEを返します。\n
*/
// ===========================================================================
BOOL GmPauseMenuFlushIsFinished(void)
{
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();
	if ((work->state == GMD_PMENU_STATE_BUILDING) ||
		(work->state == GMD_PMENU_STATE_BUILDED) ||
		(work->state == GMD_PMENU_STATE_EXECUTE) ||
		(work->state == GMD_PMENU_STATE_FLUSHING))
	{
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	GmPauseMenuRelease
/*!
	リソースファイル解放

	@note
	ポーズメニューに必要なファイルの解放を行います。\n
	この関数は完了復帰となります。\n
	GmPauseMenuFlushIsFinished関数がFALSEを返す状態では
	呼び出すことはできません。\n
	(呼び出した場合はアサートし何も行いません。)\n
	既に解放済みの場合は何も行いません。\n
*/
// ===========================================================================
void GmPauseMenuRelease(void)
{
	// ワーク取得
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();

	// チェック
	if (!GmPauseMenuFlushIsFinished()) {
		amAssert(0);
		return;
	}

	// ファイル解放
	if (work->file_cmn_ama) {
		amMemFree(work->file_cmn_ama);
		work->file_cmn_ama = NULL;
	}
	if (work->file_cmn_amb) {
		amMemFree(work->file_cmn_amb);
		work->file_cmn_amb = NULL;
	}
	if (work->file_lng_ama) {
		amMemFree(work->file_lng_ama);
		work->file_lng_ama = NULL;
	}
	if (work->file_lng_amb) {
		amMemFree(work->file_lng_amb);
		work->file_lng_amb = NULL;
	}
	if (work->file_plf_ama) {
		amMemFree(work->file_plf_ama);
		work->file_plf_ama = NULL;
	}
	if (work->file_plf_amb) {
		amMemFree(work->file_plf_amb);
		work->file_plf_amb = NULL;
	}
	if (work->file_plng_ama) {
		amMemFree(work->file_plng_ama);
		work->file_plng_ama = NULL;
	}
	if (work->file_plng_amb) {
		amMemFree(work->file_plng_amb);
		work->file_plng_amb = NULL;
	}
	if (work->file_win_amb) {
		amMemFree(work->file_win_amb);
		work->file_win_amb = NULL;
	}
#if defined(GMD_PMENU_DC_ENABLE)
	if (work->file_dcmsg_amb) {
		amMemFree(work->file_dcmsg_amb);
		work->file_dcmsg_amb = NULL;
	}
#endif // defined(GMD_PMENU_DC_ENABLE)

	// 初期状態に設定
	work->state = GMD_PMENU_STATE_RELEASED;
	work->tcb = NULL;
	work->fs = NULL;
}


// ***************************************************************************
// 実行
// ***************************************************************************
// ===========================================================================
//	GmPauseMenuStart
/*!
	ポーズメニュー開始

	@param prio		[in] メインタスク優先度
	@note
	ポーズメニューを開始します。\n
	以降、GmPauseMenuIsFinished関数がTRUEを返すまで、
	内部でポーズメニュー処理を進めます。\n
*/
// ===========================================================================
void GmPauseMenuStart(u32 prio)
{
	// ワーク取得
	GMS_PMENU_GLOBAL* gwork = gmPmGetGlbWork();

	// チェック
	if (!GmPauseMenuBuildIsFinished()) {
		amAssert(0);
		return;
	}
	if (!GmPauseMenuIsFinished()) {
		amAssert(0);
		return;
	}
	amAssert(gwork->state == GMD_PMENU_STATE_BUILDED);
	amAssert(gwork->tcb == NULL);

	// 状態設定
	gwork->state = GMD_PMENU_STATE_EXECUTE;

	// 結果初期化
	gwork->result = GME_PMENU_RESULT_NONE;

	// SEハンドル確保
	gwork->se_handle = GsSoundAllocSeHandle();

	// タスク作成
	gwork->tcb = amTaskMake(
		gmPmTask00, gmPmTaskDestructor, prio, 0, 0, "gmPauseMenu::Execute");

	// タスクワーク初期化
	GMS_PMENU_TASK_WORK* twork =
		(GMS_PMENU_TASK_WORK*)amTaskGetWork(gwork->tcb);
	twork->work = (GMS_PMENU_WORK*)amMemAlloc(sizeof(GMS_PMENU_WORK));
	twork->proc_count = 0;
	twork->proc = gmPmProcBegin;
#if defined(GMD_PMENU_DC_ENABLE)
	twork->dc_count = 0;
	twork->cproc_count = 0;
	twork->cproc = NULL;
#endif // defined(GMD_PMENU_DC_ENABLE)

	// ワーク初期化
	GMS_PMENU_WORK* work = twork->work;
	amZeroMemory(work, sizeof(GMS_PMENU_WORK));
	work->result = GME_PMENU_RESULT_NONE;

	// タスク開始
	amTaskStart(gwork->tcb);

	// SE再生
	gmPmSndPlaySe(GMD_PMENU_SE_START);
}

// ===========================================================================
//	GmPauseMenuCancel
/*!
	ポーズメニューキャンセル

	@note
	GmPauseMenuStart関数で開始したポーズメニュー処理を強制的に終了させます。\n
	メニューがどのような状態であっても、即時に終了するので、
	リセット時などを除き、通常は使用しないで下さい。\n
	この関数を呼び出した場合、GmPauseMenuGetResult関数で取得できる結果は、
	必ずGME_PMENU_RESULT_CANCELとなります。\n
	この関数を呼び出した直後から、
	GmPauseMenuIsFinished関数がTRUEを返すようになります。\n
*/
// ===========================================================================
void GmPauseMenuCancel(void)
{
	// ワーク取得
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();

	// ポーズメニュー中判定
	if (GmPauseMenuIsFinished()) {
		return;
	}

	// 強制終了
	amTaskDelete(work->tcb);
	work->tcb = NULL;
	work->result = GME_PMENU_RESULT_CANCEL;
}

// ===========================================================================
//	GmPauseMenuIsFinished
/*!
	ポーズメニュー完了判定

	@note
	GmPauseMenuStart関数で開始したポーズメニュー処理が
	完了したかを判定します。\n
	GmPauseMenuStart関数呼び出し前はTRUEを返します。\n
*/
// ===========================================================================
BOOL GmPauseMenuIsFinished(void)
{
	// ワーク取得
	GMS_PMENU_GLOBAL* gwork = gmPmGetGlbWork();

	// 判定
	if (gwork->state == GMD_PMENU_STATE_EXECUTE) {
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//	GmPauseMenuGetResult
/*!
	ポーズメニュー結果取得

	@note
	GmPauseMenuStart関数で開始したポーズメニューにおいて、
	ユーザが何を選択したかを返します。\n
	この関数はGmPauseMenuIsFinished関数がTRUEを返す状態で呼び出して下さい。\n
	それ以外の状態で呼び出すとアサートしGME_PMENU_RESULT_NONEを返します。\n
	GmPauseMenuStart関数を呼び出す前に呼び出した場合は
	GME_PMENU_RESULT_NONEを返します。\n
*/
// ===========================================================================
GME_PMENU_RESULT GmPauseMenuGetResult(void)
{
	return gmPmGetGlbWork()->result;
}


#if defined(MTD_DEBUG)

// ***************************************************************************
// デバッグ
// ***************************************************************************
// ===========================================================================
//! ポーズメニュー確認用イベント開始
// ===========================================================================
void GmPauseMenuDebugEventStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

#if GMD_PMENU_USE_DSTATE
	// アクション設定
	AoActSysSetDrawStateEnable(TRUE);
	AoActSysSetDrawState(GMD_PMENU_DRAW_STATE);
#else
	AoActSysSetDrawStateEnable(FALSE);
#endif // GMD_PMENU_USE_DSTATE

	// タスク作成
	AMS_TCB* tcb = amTaskMake(
		gmPmDgbEvTaskWaitStart, NULL, 0x1000, 0, 0, "gmPauseMenu::DebugEvent");

	// タスク開始
	amTaskStart(tcb);
}

#endif // defined(MTD_DEBUG)

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// タスク
// ***************************************************************************
// ===========================================================================
//! タスクプロシージャ00
// ===========================================================================
void gmPmTask00(AMS_TCB* tcb)
{
	GMS_PMENU_TASK_WORK* twork = (GMS_PMENU_TASK_WORK*)amTaskGetWork(tcb);

	// 終了判定
	if (twork->proc == NULL) {
		// 終了へ遷移
		amTaskSetProcedure(tcb, gmPmTask01);
		return;
	}

	// 通常プロシージャ呼び出し
	if (twork->proc) {
		// プロシージャ退避
		void (*prev_proc)(GMS_PMENU_WORK*);
		prev_proc = twork->proc;

		// プロシージャ実行
		twork->proc(twork->work);

		// カウント更新
		if (twork->proc_count < ((u32)-1)) {
			twork->proc_count += 1;
		}

		// プロシージャ切り替え判定
		if (twork->proc != prev_proc) {
			twork->proc_count = 0;
		}
	}

#if defined(GMD_PMENU_DC_ENABLE)
	// パッド切断判定
	// 通常プロシージャが有る場合だけ判定
	if (twork->proc) {

		// 既存の切断プロシージャがない場合だけ判定
		if (twork->cproc == NULL) {

			// キャンセル状態でない場合だけ判定
			if (twork->work->result != GME_PMENU_RESULT_CANCEL) {
				if (!AoPadIsConnected()) {

					// 切断プロシージャ開始
					twork->cproc = gmPmCProcEfctIn;
					twork->cproc_count = 0;
				}
			}
		}
	}

	// 切断プロシージャ呼び出し
	if (twork->cproc) {
		// プロシージャ退避
		void (*prev_proc)(GMS_PMENU_WORK*);
		prev_proc = twork->cproc;

		// プロシージャ実行
		twork->cproc(twork->work);

		// カウント更新
		if (twork->cproc_count < ((u32)-1)) {
			twork->cproc_count += 1;
		}

		// プロシージャ切り替え判定
		if (twork->cproc != prev_proc) {
			twork->cproc_count = 0;
		}
	}
#endif // defined(GMD_PMENU_DC_ENABLE)
}

// ===========================================================================
//! タスクプロシージャ01
// ===========================================================================
void gmPmTask01(AMS_TCB* tcb)
{
	amTaskDelete(tcb);
}

// ===========================================================================
//! タスクデストラクタ
// ===========================================================================
void gmPmTaskDestructor(AMS_TCB* tcb)
{
	GMS_PMENU_GLOBAL* gwork = gmPmGetGlbWork();
	GMS_PMENU_TASK_WORK* twork = (GMS_PMENU_TASK_WORK*)amTaskGetWork(tcb);
	amAssert(tcb == gwork->tcb);

	// SEハンドル解放
	if (gwork->se_handle) {
		GsSoundFreeSeHandle(gwork->se_handle);
		gwork->se_handle = NULL;
	}

	// ワーク解放
	if (twork->work) {
		// アクション解放
		for (u32 i = 0; i < ACT_NUM; ++i) {
			if (twork->work->act[i]) {
				AoActDelete(twork->work->act[i]);
				twork->work->act[i] = NULL;
			}
		}
		amMemFree(twork->work);
		twork->work = NULL;
	}

	// 状態変更
	gwork->state = GMD_PMENU_STATE_BUILDED;
	gwork->tcb = NULL;
}

// ===========================================================================
//! プロシージャカウンタ取得
// ===========================================================================
u32 gmPmGetProcCount(void)
{
	return ((GMS_PMENU_TASK_WORK*)
		amTaskGetWork(gmPmGetGlbWork()->tcb))->proc_count;
}

// ===========================================================================
//! プロシージャ変更
// ===========================================================================
void gmPmSetProc(void (*proc)(GMS_PMENU_WORK*))
{
	((GMS_PMENU_TASK_WORK*)amTaskGetWork(gmPmGetGlbWork()->tcb))->proc = proc;
}

#if defined(GMD_PMENU_DC_ENABLE)
// ===========================================================================
//! 切断カウンタ取得
// ===========================================================================
u32& gmPmGetDcCount(void)
{
	return ((GMS_PMENU_TASK_WORK*)
		amTaskGetWork(gmPmGetGlbWork()->tcb))->dc_count;
}

// ===========================================================================
//! 切断プロシージャカウンタ取得
// ===========================================================================
u32 gmPmGetCProcCount(void)
{
	return ((GMS_PMENU_TASK_WORK*)
		amTaskGetWork(gmPmGetGlbWork()->tcb))->cproc_count;
}

// ===========================================================================
//! 切断プロシージャ変更
// ===========================================================================
void gmPmSetCProc(void (*proc)(GMS_PMENU_WORK*))
{
	((GMS_PMENU_TASK_WORK*)amTaskGetWork(gmPmGetGlbWork()->tcb))->cproc = proc;
}
#endif // defined(GMD_PMENU_DC_ENABLE)


// ***************************************************************************
// プロシージャ
// ***************************************************************************
// ===========================================================================
//! 開始
// ===========================================================================
void gmPmProcBegin(GMS_PMENU_WORK* work)
{
	// グローバルワーク取得
	GMS_PMENU_GLOBAL* gwork = gmPmGetGlbWork();

	// アクション構築
	for (u32 i = 0; i < ACT_NUM; ++i) {
		// ファイル選別
		const void* ama;
		AOS_TEXTURE* tex;
		if (i >= ACT_PLNG_S) {
			ama = gwork->file_plng_ama;
			tex = &gwork->tex_plng;
		}
		else if (i >= ACT_PLF_S) {
			ama = gwork->file_plf_ama;
			tex = &gwork->tex_plf;
		}
		else if (i >= ACT_LNG_S) {
			ama = gwork->file_lng_ama;
			tex = &gwork->tex_lng;
		}
		else {
			ama = gwork->file_cmn_ama;
			tex = &gwork->tex_cmn;
		}
		// 構築
		AoActSetTexture(AoTexGetTexList(tex));
		u32 act_id = g_gm_pm_act_id_tbl[i];
		if (!gmPmGlbIsBackEnable()) {
			if (i == ACT_PL_BTN_WORLD) {
				act_id = IDA_G_PAUSE_P_L_ACT_BTN_MMENU;
			}
			else if (i == ACT_PL_BTN_WORLD_S) {
				act_id = IDA_G_PAUSE_P_L_ACT_BTN_MMENU_S;
			}
		}
		if (!gmPmGlbIsSpecialStage()) {
			if (i == ACT_L_MSG_RETRY) {
				act_id = IDA_G_PAUSE_L_ACT_MSG_RETRY2;
			}
		}
		work->act[i] = AoActCreate(ama, act_id);
	}

	// 入り演出へ遷移
	gmPmSetProc(gmPmProcEfctIn);
}

// ===========================================================================
//! 入り演出
// ===========================================================================
void gmPmProcEfctIn(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// 入り演出描画
	gmPmDrawEfctIn(work, (f32)count / (f32)GMD_PMENU_EFCT_IN_TIME);

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_IN_TIME) {

		// 次へ遷移
		gmPmSetProc(gmPmProcSelect);
	}
}

// ===========================================================================
//! 選択
// ===========================================================================
void gmPmProcSelect(GMS_PMENU_WORK* work)
{
	// 選択項目数
#if _PC || _XBOX
	const u32 sel_num = 6;
#elif _WII
	const u32 sel_num = 2;
#else
	const u32 sel_num = 3;
#endif // _PC || _XBOX

	// 選択切り替え
	BOOL is_select = FALSE;
	if (gmPmPadMRepeat() & GSD_KEY_UP) {
		if (work->select > 0) {
			work->select -= 1;
			is_select = TRUE;
		}
		else if (gmPmPadMStand() & GSD_KEY_UP) {
			work->select = (u32)(sel_num - 1);
			is_select = TRUE;
		}
	}
	if (gmPmPadMRepeat() & GSD_KEY_DOWN) {
		if (work->select < (u32)(sel_num - 1)) {
			work->select += 1;
			is_select = TRUE;
		}
		else if (gmPmPadMStand() & GSD_KEY_DOWN) {
			work->select = 0;
			is_select = TRUE;
		}
	}
	if (is_select) {
		gmPmSndPlaySe(GMD_PMENU_SE_CURSOR);
	}

	// 描画
	gmPmDrawSelect(work, work->select);

	// 決定判定
	if (gmPmPadStand() & GSD_KEY_DECIDE) {

		// 選択項目から結果を取得
#if _PC || _XBOX
		switch (work->select) {
		case 0:
			work->result = GME_PMENU_RESULT_CANCEL;
			break;
		case 1:
			if (gmPmGlbIsRetryEnable()) {
				work->result = GME_PMENU_RESULT_RETRY;
			}
			break;
		case 2:
			work->result = GME_PMENU_RESULT_OPTION;
			break;
		case 3:
			work->result = GME_PMENU_RESULT_RANKING;
			break;
		case 4:
			work->result = GME_PMENU_RESULT_TROPHY;
			break;
		case 5:
			if (gmPmGlbIsBackEnable()) {
				work->result = GME_PMENU_RESULT_BACK;
			}
			else {
				work->result = GME_PMENU_RESULT_MAINMENU;
			}
			break;
		default:
			amAssert(0);
			work->result = GME_PMENU_RESULT_CANCEL;
			break;
		}
#elif _WII
		switch (work->select) {
		case 0:
			if (gmPmGlbIsRetryEnable()) {
				work->result = GME_PMENU_RESULT_RETRY;
			}
			break;
		case 1:
			if (gmPmGlbIsBackEnable()) {
				work->result = GME_PMENU_RESULT_BACK;
			}
			else {
				work->result = GME_PMENU_RESULT_MAINMENU;
			}
			break;
		default:
			amAssert(0);
			work->result = GME_PMENU_RESULT_CANCEL;
			break;
		}
#else
		switch (work->select) {
		case 0:
			if (gmPmGlbIsRetryEnable()) {
				work->result = GME_PMENU_RESULT_RETRY;
			}
			break;
		case 1:
			work->result = GME_PMENU_RESULT_OPTION;
			break;
		case 2:
			if (gmPmGlbIsBackEnable()) {
				work->result = GME_PMENU_RESULT_BACK;
			}
			else {
				work->result = GME_PMENU_RESULT_MAINMENU;
			}
			break;
		default:
			amAssert(0);
			work->result = GME_PMENU_RESULT_CANCEL;
			break;
		}
#endif // _PC || _XBOX

		if ((u32)work->result < GME_PMENU_RESULT_NUM) {
			gmPmSetProc(gmPmProcDecide);
			gmPmSndPlaySe(GMD_PMENU_SE_DECIDE);
		}
	}
	// キャンセル判定
	else if (gmPmPadStand() & (GSD_KEY_CANCEL | KEY_START)) {
		work->result = GME_PMENU_RESULT_CANCEL;
		gmPmSetProc(gmPmProcEfctOut);

		gmPmSndPlaySe(GMD_PMENU_SE_CANCEL);
	}
}

// ===========================================================================
//! 決定演出
// ===========================================================================
void gmPmProcDecide(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// 演出描画
	if ((count % 8) < 4) {
		gmPmDrawSelect(work, 6);
	}
	else {
		gmPmDrawSelect(work, work->select);
	}

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_DECIDE_TIME) {
		gmPmSetProc(gmPmProcEfctOut);
	}
}

// ===========================================================================
//! 退出演出
// ===========================================================================
void gmPmProcEfctOut(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// 入り演出描画
	gmPmDrawEfctIn(work, 1.0f - ((f32)count / (f32)GMD_PMENU_EFCT_OUT_TIME));

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_OUT_TIME) {

		// リトライなら確認メッセージ表示
		if (work->result == GME_PMENU_RESULT_RETRY) {
			gmPmSetProc(gmPmProcRetryIn);
		}

		// ワードドマップへ戻るなら確認メッセージ表示
		else if (work->result == GME_PMENU_RESULT_BACK) {
			gmPmSetProc(gmPmProcBackIn);
		}

		// メインメニューへ戻るなら確認メッセージ表示
		else if (work->result == GME_PMENU_RESULT_MAINMENU) {
			gmPmSetProc(gmPmProcMainMenuIn);
		}

		// 終わる
		else {
			gmPmSetProc(gmPmProcEnd);
		}
	}
}

// ===========================================================================
//! リトライ確認入り演出
// ===========================================================================
void gmPmProcRetryIn(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// ウインドウ演出描画
	gmPmDrawWindowEfct(work, (f32)count / (f32)GMD_PMENU_EFCT_WIN_TIME);

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_WIN_TIME) {

		// 次へ遷移
		work->select_sub = 1;
		gmPmSetProc(gmPmProcRetrySelect);
	}
}

// ===========================================================================
//! リトライ確認選択
// ===========================================================================
void gmPmProcRetrySelect(GMS_PMENU_WORK* work)
{
	// 選択切り替え
	BOOL is_select = FALSE;
	if (gmPmPadMStand() & GSD_KEY_LEFT) {
		work->select_sub = 0;
		is_select = TRUE;
	}
	if (gmPmPadMStand() & GSD_KEY_RIGHT) {
		work->select_sub = 1;
		is_select = TRUE;
	}
	if (is_select) {
		gmPmSndPlaySe(GMD_PMENU_SE_CURSOR);
	}

	// ウインドウ描画
	gmPmDrawWindowMessage(work, ACT_L_MSG_RETRY, work->select_sub);

	// 決定判定
	if (gmPmPadStand() & GSD_KEY_DECIDE) {
		if (work->select_sub != 0) {
			work->result = GME_PMENU_RESULT_NONE;
		}
		// 次へ遷移
		gmPmSetProc(gmPmProcRetryDecide);

		gmPmSndPlaySe(GMD_PMENU_SE_DECIDE);
	}
	// キャンセル判定
	else if ((gmPmPadStand() & GSD_KEY_CANCEL) || gmPmPadIsDisable()) {
		work->result = GME_PMENU_RESULT_NONE;
		// 次へ遷移
		gmPmSetProc(gmPmProcRetryOut);

		gmPmSndPlaySe(GMD_PMENU_SE_CANCEL);
	}
}

// ===========================================================================
//! リトライ確認決定演出
// ===========================================================================
void gmPmProcRetryDecide(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// ウインドウ描画
	if ((count % 8) < 4) {
		gmPmDrawWindowMessage(work, ACT_L_MSG_RETRY, 2);
	}
	else {
		gmPmDrawWindowMessage(work, ACT_L_MSG_RETRY, work->select_sub);
	}

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_DECIDE_TIME) {
		gmPmSetProc(gmPmProcRetryOut);
	}
}

// ===========================================================================
//! リトライ確認退出演出
// ===========================================================================
void gmPmProcRetryOut(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// ウインドウ演出描画
	gmPmDrawWindowEfct(
		work, 1.0f - ((f32)count / (f32)GMD_PMENU_EFCT_WIN_TIME));

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_WIN_TIME) {

		// 結果なしならもう一度表示
		if (work->result >= GME_PMENU_RESULT_NUM) {
			gmPmSetProc(gmPmProcEfctIn);
		}

		// 終了
		else {
			gmPmSetProc(gmPmProcEnd);
		}
	}
}

// ===========================================================================
//! ワールドマップへ戻る確認入り演出
// ===========================================================================
void gmPmProcBackIn(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// ウインドウ演出描画
	gmPmDrawWindowEfct(work, (f32)count / (f32)GMD_PMENU_EFCT_WIN_TIME);

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_WIN_TIME) {

		// 次へ遷移
		work->select_sub = 1;
		gmPmSetProc(gmPmProcBackSelect);
	}
}

// ===========================================================================
//! ワールドマップへ戻る確認選択
// ===========================================================================
void gmPmProcBackSelect(GMS_PMENU_WORK* work)
{
	// 選択切り替え
	BOOL is_select = FALSE;
	if (gmPmPadMStand() & GSD_KEY_LEFT) {
		work->select_sub = 0;
		is_select = TRUE;
	}
	if (gmPmPadMStand() & GSD_KEY_RIGHT) {
		work->select_sub = 1;
		is_select = TRUE;
	}
	if (is_select) {
		gmPmSndPlaySe(GMD_PMENU_SE_CURSOR);
	}

	// ウインドウ描画
	gmPmDrawWindowMessage(work, ACT_L_MSG_WOLRD, work->select_sub);

	// 決定判定
	if (gmPmPadStand() & GSD_KEY_DECIDE) {
		if (work->select_sub != 0) {
			work->result = GME_PMENU_RESULT_NONE;
		}
		// 次へ遷移
		gmPmSetProc(gmPmProcBackDecide);

		gmPmSndPlaySe(GMD_PMENU_SE_DECIDE);
	}
	// キャンセル判定
	else if ((gmPmPadStand() & GSD_KEY_CANCEL) || gmPmPadIsDisable()) {
		work->result = GME_PMENU_RESULT_NONE;
		// 次へ遷移
		gmPmSetProc(gmPmProcBackOut);

		gmPmSndPlaySe(GMD_PMENU_SE_CANCEL);
	}
}

// ===========================================================================
//! ワールドマップへ戻る確認決定演出
// ===========================================================================
void gmPmProcBackDecide(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// ウインドウ描画
	if ((count % 8) < 4) {
		gmPmDrawWindowMessage(work, ACT_L_MSG_WOLRD, 2);
	}
	else {
		gmPmDrawWindowMessage(work, ACT_L_MSG_WOLRD, work->select_sub);
	}

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_DECIDE_TIME) {
		gmPmSetProc(gmPmProcBackOut);
	}
}

// ===========================================================================
//! ワールドマップへ戻る確認退出演出
// ===========================================================================
void gmPmProcBackOut(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// ウインドウ演出描画
	gmPmDrawWindowEfct(
		work, 1.0f - ((f32)count / (f32)GMD_PMENU_EFCT_WIN_TIME));

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_WIN_TIME) {

		// 結果なしならもう一度表示
		if (work->result >= GME_PMENU_RESULT_NUM) {
			gmPmSetProc(gmPmProcEfctIn);
		}

		// 終了
		else {
			gmPmSetProc(gmPmProcEnd);
		}
	}
}

// ===========================================================================
//! メインメニューへ戻る確認入り演出
// ===========================================================================
void gmPmProcMainMenuIn(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// ウインドウ演出描画
	gmPmDrawWindowEfct(work, (f32)count / (f32)GMD_PMENU_EFCT_WIN_TIME);

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_WIN_TIME) {

		// 次へ遷移
		work->select_sub = 1;
		gmPmSetProc(gmPmProcMainMenuSelect);
	}
}

// ===========================================================================
//! メインメニューへ戻る確認選択
// ===========================================================================
void gmPmProcMainMenuSelect(GMS_PMENU_WORK* work)
{
	// 選択切り替え
	BOOL is_select = FALSE;
	if (gmPmPadMStand() & GSD_KEY_LEFT) {
		work->select_sub = 0;
		is_select = TRUE;
	}
	if (gmPmPadMStand() & GSD_KEY_RIGHT) {
		work->select_sub = 1;
		is_select = TRUE;
	}
	if (is_select) {
		gmPmSndPlaySe(GMD_PMENU_SE_CURSOR);
	}

	// ウインドウ描画
	gmPmDrawWindowMessage(work, ACT_L_MSG_MMENU, work->select_sub);

	// 決定判定
	if (gmPmPadStand() & GSD_KEY_DECIDE) {
		if (work->select_sub != 0) {
			work->result = GME_PMENU_RESULT_NONE;
		}
		// 次へ遷移
		gmPmSetProc(gmPmProcMainMenuDecide);

		gmPmSndPlaySe(GMD_PMENU_SE_DECIDE);
	}
	// キャンセル判定
	else if ((gmPmPadStand() & GSD_KEY_CANCEL) || gmPmPadIsDisable()) {
		work->result = GME_PMENU_RESULT_NONE;
		// 次へ遷移
		gmPmSetProc(gmPmProcMainMenuOut);

		gmPmSndPlaySe(GMD_PMENU_SE_CANCEL);
	}
}

// ===========================================================================
//! メインメニューへ戻る確認決定演出
// ===========================================================================
void gmPmProcMainMenuDecide(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// ウインドウ描画
	if ((count % 8) < 4) {
		gmPmDrawWindowMessage(work, ACT_L_MSG_MMENU, 2);
	}
	else {
		gmPmDrawWindowMessage(work, ACT_L_MSG_MMENU, work->select_sub);
	}

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_DECIDE_TIME) {
		gmPmSetProc(gmPmProcMainMenuOut);
	}
}

// ===========================================================================
//! メインメニューへ戻る確認退出演出
// ===========================================================================
void gmPmProcMainMenuOut(GMS_PMENU_WORK* work)
{
	// カウント取得
	u32 count = gmPmGetProcCount();

	// ウインドウ演出描画
	gmPmDrawWindowEfct(
		work, 1.0f - ((f32)count / (f32)GMD_PMENU_EFCT_WIN_TIME));

	// 演出終了判定
	if (count >= GMD_PMENU_EFCT_WIN_TIME) {

		// 結果なしならもう一度表示
		if (work->result >= GME_PMENU_RESULT_NUM) {
			gmPmSetProc(gmPmProcEfctIn);
		}

		// 終了
		else {
			gmPmSetProc(gmPmProcEnd);
		}
	}
}

// ===========================================================================
//! 終了
// ===========================================================================
void gmPmProcEnd(GMS_PMENU_WORK* work)
{
	// グローバルワーク取得
	GMS_PMENU_GLOBAL* gwork = gmPmGetGlbWork();

	// 結果格納
	gwork->result = work->result;

	// 終了
	gmPmSetProc(NULL);
}


// ***************************************************************************
// 描画
// ***************************************************************************
// ===========================================================================
//! 入り演出描画
// ===========================================================================
void gmPmDrawEfctIn(GMS_PMENU_WORK* work, f32 rate)
{
	u32 act_id;
	AOS_ACTION* act;

	// アクションID
#if _PC || _XBOX
	const u32 act_id_num = 6;
	const u32 act_id_tbl[act_id_num] = {
		ACT_PL_BTN_RESUME,
		ACT_PL_BTN_RETRY,
		ACT_PL_BTN_OPTION,
		ACT_PL_BTN_RANK,
		ACT_PL_BTN_TROPHY,
		ACT_PL_BTN_WORLD,
	};
#elif _WII
	const u32 act_id_num = 2;
	const u32 act_id_tbl[2] = {
		ACT_PL_BTN_RETRY,
		ACT_PL_BTN_WORLD,
	};
#else
	const u32 act_id_num = 3;
	const u32 act_id_tbl[3] = {
		ACT_PL_BTN_RETRY,
		ACT_PL_BTN_OPTION,
		ACT_PL_BTN_WORLD,
	};
#endif // _PC || _XBOX

	// アキュムレート退避
	AoActAcmPush();

	// ボタン以外のアルファ用カラー作成
	AOS_ACT_COL col;
	col.r = col.g = col.b = 255;
	if (rate < 0.5f) {
		col.a = 0;
	}
	else {
		col.a = (u8)(255 * ((rate - 0.5f) * 2.0f));
	}

	// 背景描画
	if (col.a > 0) {
		AoActAcmInit();
		AoActAcmApplyColor(col);
#if _PC || _XBOX
	AoActAcmApplyTrans(
		(f32)GMD_PMENU_SCREEN_W * 0.5f,
		(f32)GMD_PMENU_SCREEN_H * 0.5f + 32.0f,
		0.0f);
#else
	AoActAcmApplyTrans(
		(f32)GMD_PMENU_SCREEN_W * 0.5f,
		(f32)GMD_PMENU_SCREEN_H * 0.5f,
		0.0f);
#endif // _PC || _XBOX
		act_id = ACT_BTN_BACK;
		act = work->act[act_id];
		gmPmUtilSetTexture(act_id);
		AoActUpdate(act);
		AoActSortRegAction(act);
	}

	// ボタン描画
	f32 first_pos = -1.0f;
	f32 last_pos = -1.0f;
	for (u32 i = 0; i < act_id_num; ++i) {
		if (act_id_tbl[i] >= ACT_NUM) {
			continue;
		}

		// 初動レート
		f32 mrate = (0.5f / (f32)act_id_num) * (f32)i;
		if (mrate > rate) {
			continue;
		}

		// 実レート
		f32 rrate = (rate - mrate) * 2.0f;
		if (rrate > 1.0f) {
			rrate = 1.0f;
		}
		rrate = 1.0f - rrate;
		rrate = 1.0f - (rrate * rrate);

		// 表示位置算出
		f32 pos = gmPmDrawGetSelShowPos(i);
		if (first_pos < 0.0f) {
			first_pos = pos;
		}
		if (rrate < 1.0f) {
			pos += (f32)GMD_PMENU_SCREEN_H - ((f32)GMD_PMENU_SCREEN_H * rrate);
		}
		if (i == (u32)(act_id_num - 1)) {
			last_pos = pos;
		}

		// アキュムレート設定
		AoActAcmInit();
		AoActAcmApplyTrans((f32)GMD_PMENU_SCREEN_W * 0.5f, pos - 16.0f, 0.0f);

		// 土台描画
		act_id = (u32)(ACT_BTN_BASE1 + (i * 2));
		act = work->act[act_id];
		gmPmUtilSetTexture(act_id);
		AoActUpdate(act);
		AoActSortRegAction(act);

		// 描画
		AoActAcmApplyTrans(0.0f, 8.0f, 0.0f);
		act_id = act_id_tbl[i];
		act = work->act[act_id];
		gmPmUtilSetTexture(act_id);
		AoActUpdate(act);
		AoActSortRegAction(act);
	}

	if (col.a > 0) {
		// タイトル土台描画
		AoActAcmInit();
		AoActAcmApplyColor(col);
		AoActAcmApplyTrans(
			(f32)GMD_PMENU_SCREEN_W * 0.5f, first_pos - 64.0f, 0.0f);
		act = work->act[ACT_TITLE_BASE];
		gmPmUtilSetTexture(ACT_TITLE_BASE);
		AoActUpdate(act);
		AoActSortRegAction(act);

		// タイトル描画
		AoActAcmApplyTrans(0.0f, -16.0f, 0.0f);
		act = work->act[ACT_L_TITLE];
		gmPmUtilSetTexture(ACT_L_TITLE);
		AoActUpdate(act);
		AoActSortRegAction(act);

		// 戻るボタンX位置算出
		f32 bb_pos_x = ((f32)GMD_PMENU_SCREEN_W * 0.5f) -
			(g_gm_pmenu_bb_width_tbl[GsEnvGetLanguage()] / 2.0f) + 16.0f;

		// 戻るボタン描画
		AoActAcmInit();
		AoActAcmApplyColor(col);
#if _XBOX || _PC
		AoActAcmApplyTrans(bb_pos_x, last_pos + 48.0f, 0.0f);
#else
		AoActAcmApplyTrans(bb_pos_x, last_pos + 64.0f, 0.0f);
#endif // _XBOX || _PC
		if (GeEnvGetDecideKey() == GSD_DECIDE_KEY_O) {
			act_id = ACT_P_BACK_MARK_X;
		}
		else {
			act_id = ACT_P_BACK_MARK_O;
		}
		gmPmUtilSetTexture(act_id);
		act = work->act[act_id];
		AoActUpdate(act);
		AoActSortRegAction(act);

		// 戻る描画
		AoActAcmApplyTrans(16.0f, -16.0f, 0.0f);
		act_id = ACT_L_BTN_BACK;
		gmPmUtilSetTexture(act_id);
		act = work->act[act_id];
		AoActUpdate(act);
		AoActSortRegAction(act);
	}

	// アキュムレート復帰
	AoActAcmPop();
}

// ===========================================================================
//! 選択描画
// ===========================================================================
void gmPmDrawSelect(GMS_PMENU_WORK* work, u32 select)
{
	u32 act_id;
	AOS_ACTION* act;

	// アクションID
#if _PC || _XBOX
	const u32 act_id_num = 6;
	const u32 act_id_tbl[act_id_num] = {
		ACT_PL_BTN_RESUME,
		ACT_PL_BTN_RETRY,
		ACT_PL_BTN_OPTION,
		ACT_PL_BTN_RANK,
		ACT_PL_BTN_TROPHY,
		ACT_PL_BTN_WORLD,
	};
#elif _WII
	const u32 act_id_num = 2;
	const u32 act_id_tbl[2] = {
		ACT_PL_BTN_RETRY,
		ACT_PL_BTN_WORLD,
	};
#else
	const u32 act_id_num = 3;
	const u32 act_id_tbl[3] = {
		ACT_PL_BTN_RETRY,
		ACT_PL_BTN_OPTION,
		ACT_PL_BTN_WORLD,
	};
#endif // _PC || _XBOX

	// アキュムレート退避
	AoActAcmPush();

	// 背景描画
	AoActAcmInit();
#if _PC || _XBOX
	AoActAcmApplyTrans(
		(f32)GMD_PMENU_SCREEN_W * 0.5f,
		(f32)GMD_PMENU_SCREEN_H * 0.5f + 32.0f,
		0.0f);
#else
	AoActAcmApplyTrans(
		(f32)GMD_PMENU_SCREEN_W * 0.5f,
		(f32)GMD_PMENU_SCREEN_H * 0.5f,
		0.0f);
#endif // _PC || _XBOX
	act_id = ACT_BTN_BACK;
	act = work->act[act_id];
	gmPmUtilSetTexture(act_id);
	AoActUpdate(act);
	AoActSortRegAction(act);

	// ボタン描画
	f32 first_pos = -1.0f;
	f32 last_pos = -1.0f;
	for (u32 i = 0; i < act_id_num; ++i) {
		if (act_id_tbl[i] >= ACT_NUM) {
			continue;
		}

		// 表示位置算出
		f32 pos = gmPmDrawGetSelShowPos(i);
		if (first_pos < 0.0f) {
			first_pos = pos;
		}
		last_pos = pos;

		// アキュムレート設定
		AoActAcmInit();
		AoActAcmApplyTrans((f32)GMD_PMENU_SCREEN_W * 0.5f, pos - 16.0f, 0.0f);

		// 土台描画
		if (i == select) {
			act_id = (u32)(ACT_BTN_BASE1 + (i * 2) + 1);
		}
		else {
			act_id = (u32)(ACT_BTN_BASE1 + (i * 2));
		}
		act = work->act[act_id];
		gmPmUtilSetTexture(act_id);
		AoActUpdate(act);
		AoActSortRegAction(act);

		// 描画
		AoActAcmApplyTrans(0.0f, 8.0f, 0.0f);
		act_id = act_id_tbl[i];
		if (i == select) {
			act_id += 1;
		}
		act = work->act[act_id];
		gmPmUtilSetTexture(act_id);
		AoActUpdate(act);
		AoActSortRegAction(act);
	}

	// タイトル土台描画
	AoActAcmInit();
	AoActAcmApplyTrans(
		(f32)GMD_PMENU_SCREEN_W * 0.5f, first_pos - 64.0f, 0.0f);
	act = work->act[ACT_TITLE_BASE];
	gmPmUtilSetTexture(ACT_TITLE_BASE);
	AoActUpdate(act);
	AoActSortRegAction(act);

	// タイトル描画
	AoActAcmApplyTrans(0.0f, -16.0f, 0.0f);
	act = work->act[ACT_L_TITLE];
	gmPmUtilSetTexture(ACT_L_TITLE);
	AoActUpdate(act);
	AoActSortRegAction(act);

	// 戻るボタンX位置算出
	f32 bb_pos_x = ((f32)GMD_PMENU_SCREEN_W * 0.5f) -
		(g_gm_pmenu_bb_width_tbl[GsEnvGetLanguage()] / 2.0f) + 16.0f;

	// 戻るボタン描画
	AoActAcmInit();
#if _XBOX || _PC
	AoActAcmApplyTrans(bb_pos_x, last_pos + 48.0f, 0.0f);
#else
	AoActAcmApplyTrans(bb_pos_x, last_pos + 64.0f, 0.0f);
#endif // _XBOX || _PC
	if (GeEnvGetDecideKey() == GSD_DECIDE_KEY_O) {
		act_id = ACT_P_BACK_MARK_X;
	}
	else {
		act_id = ACT_P_BACK_MARK_O;
	}
	gmPmUtilSetTexture(act_id);
	act = work->act[act_id];
	AoActUpdate(act);
	AoActSortRegAction(act);

	// 戻る描画
	AoActAcmApplyTrans(16.0f, -16.0f, 0.0f);
	act_id = ACT_L_BTN_BACK;
	gmPmUtilSetTexture(act_id);
	act = work->act[act_id];
	AoActUpdate(act);
	AoActSortRegAction(act);

	// アキュムレート復帰
	AoActAcmPop();
}

// ===========================================================================
//! 選択項目表示位置取得
// ===========================================================================
f32 gmPmDrawGetSelShowPos(u32 i)
{
#if _PC || _XBOX
	return (f32)(
		((s32)(GMD_PMENU_SCREEN_H / 2) - (s32)(64 * 2 - 32)) + (s32)(51 * i));
#elif _WII
	return (f32)(((s32)(GMD_PMENU_SCREEN_H / 2) - 32) + (s32)(64 * i));
#else
	return (f32)(
		((s32)(GMD_PMENU_SCREEN_H / 2) - (s32)(64 * 1)) + (s32)(64 * i));
#endif
}

#if GMD_PMENU_USE_DSTATE
// ===========================================================================
//! ウインドウ描画前処理
// ===========================================================================
void gmPmDrawWindowEfctPrev(void* param)
{
	UNREFERENCED_PARAMETER(param);
	AoActDrawPre();
}
#endif // GMD_PMENU_USE_DSTATE

// ===========================================================================
//! ウインドウ描画
// ===========================================================================
void gmPmDrawWindowEfct(GMS_PMENU_WORK* work, f32 rate)
{
	UNREFERENCED_PARAMETER(work);

	// グローバルワーク取得
	GMS_PMENU_GLOBAL* gwork = gmPmGetGlbWork();

	// ウインドウ形状算出
	f32 x = (f32)GMD_PMENU_SCREEN_W * 0.5f;
	f32 y = (f32)GMD_PMENU_SCREEN_H * 0.5f;
	f32 w = (f32)GMD_PMENU_WIN_W * rate;
	f32 h = (f32)GMD_PMENU_WIN_H * rate;

#if GMD_PMENU_USE_DSTATE

	// 前処理
	ObjDraw3DNNUserFunc(
		gmPmDrawWindowEfctPrev, NULL, 0, OBD_DRAW_CMD_STATE_2DAMA);

	AoWinSysDrawState(
		AOD_WIN_TYPE_A, AoTexGetTexList(&gwork->tex_win), 0,
		x, y, w, h, AoActSysGetDrawState());

#else

	AoWinSysDrawTask(
		AOD_WIN_TYPE_A, AoTexGetTexList(&gwork->tex_win), 0,
		x, y, w, h, (u16)(AoActSysGetDrawTaskPrio() - 1));

#endif // GMD_PMENU_USE_DSTATE
}

// ===========================================================================
//! ウインドウメッセージ描画
// ===========================================================================
void gmPmDrawWindowMessage(GMS_PMENU_WORK* work, u32 act_no, u32 select)
{
	// ウインドウ描画
	gmPmDrawWindowEfct(work, 1.0f);

	// アキュムレート退避
	AoActAcmPush();

	u32 act_id;
	AOS_ACTION* act;
	const f32 win_l = (f32)((GMD_PMENU_SCREEN_W - GMD_PMENU_WIN_W) / 2);
	const f32 win_t = (f32)((GMD_PMENU_SCREEN_H - GMD_PMENU_WIN_H) / 2);
	const f32 win_r = win_l + (f32)GMD_PMENU_WIN_W;
	const f32 center_x = (f32)GMD_PMENU_SCREEN_W * 0.5f;
	const f32 win_rt = win_t - 28.0f;

	// 戻るボタン描画
	AoActAcmInit();
	AoActAcmApplyTrans(
		win_r - g_gm_pmenu_bb_width_tbl[GsEnvGetLanguage()] + 16.0f,
		win_rt + 26.0f, 0.0f);
	if (GeEnvGetDecideKey() == GSD_DECIDE_KEY_O) {
		act_id = ACT_P_BACK_MARK_X;
	}
	else {
		act_id = ACT_P_BACK_MARK_O;
	}
	gmPmUtilSetTexture(act_id);
	act = work->act[act_id];
	AoActUpdate(act);
	AoActSortRegAction(act);

	// 戻る描画
	AoActAcmApplyTrans(16.0f, -16.0f, 0.0f);
	act_id = ACT_L_BTN_BACK;
	gmPmUtilSetTexture(act_id);
	act = work->act[act_id];
	AoActUpdate(act);
	AoActSortRegAction(act);

	// メッセージ描画
	AoActAcmInit();
#if _WII
	AoActAcmApplyScale(1.5f, 1.5f);
#endif // _WII
	AoActAcmApplyTrans(center_x, win_t + 26.0f, 0.0f);
	gmPmUtilSetTexture(act_no);
	act = work->act[act_no];
	AoActUpdate(act);
	AoActSortRegAction(act);

	// はい
	AoActAcmInit();
#if _WII
	AoActAcmApplyScale(1.5f, 1.5f);
	AoActAcmApplyTrans(
		center_x - 90.0f, win_t + 26.0f + 196.0f + 10.0f, 0.0f);
#else
	AoActAcmApplyTrans(
		center_x - 60.0f, win_t + 26.0f + 128.0f + 10.0f, 0.0f);
#endif // _WII
	if (select == 0) {
		act_id = ACT_L_DLG_YES_S;
	}
	else {
		act_id = ACT_L_DLG_YES;
	}
	gmPmUtilSetTexture(act_id);
	act = work->act[act_id];
	AoActUpdate(act);
	AoActSortRegAction(act);

	// いいえ
	AoActAcmInit();
#if _WII
	AoActAcmApplyScale(1.5f, 1.5f);
	AoActAcmApplyTrans(
		center_x + 90.0f, win_t + 26.0f + 196.0f + 10.0f, 0.0f);
#else
	AoActAcmApplyTrans(
		center_x + 60.0f, win_t + 26.0f + 128.0f + 10.0f, 0.0f);
#endif // _WII
	if (select == 1) {
		act_id = ACT_L_DLG_NO_S;
	}
	else {
		act_id = ACT_L_DLG_NO;
	}
	gmPmUtilSetTexture(act_id);
	act = work->act[act_id];
	AoActUpdate(act);
	AoActSortRegAction(act);

	// アキュムレート復帰
	AoActAcmPop();
}


#if defined(GMD_PMENU_DC_ENABLE)
#define GMD_PM_CEFCT_TIME			(8)		//!< 切断プロシージャ演出時間
#define GMD_PM_CEFCT_BLACK_ALPHA	(127)	//!< 切断プロシージャ黒アルファ値
// ***************************************************************************
// パッド切断プロシージャ
// ***************************************************************************
// ===========================================================================
//! 入り演出
// ===========================================================================
void gmPmCProcEfctIn(GMS_PMENU_WORK* work)
{
	// カウンタ取得
	u32 count = gmPmGetCProcCount();

	// 黒描画
	gmPmCDrawBlack(
		work, (u8)((GMD_PM_CEFCT_BLACK_ALPHA * count) / GMD_PM_CEFCT_TIME));

	// 演出描画
	gmPmCDrawEfctIn(work, (f32)count / (f32)GMD_PM_CEFCT_TIME);

	// 演出終了判定
	if (count >= GMD_PM_CEFCT_TIME) {
		gmPmSetCProc(gmPmCProcMessage);
	}
}

// ===========================================================================
//! メッセージ表示
// ===========================================================================
void gmPmCProcMessage(GMS_PMENU_WORK* work)
{
	// カウンタ取得
	u32 count = gmPmGetCProcCount();

	// 切断カウント初期化
	if (count == 0) {
		gmPmGetDcCount() = 0;
	}

	// 黒描画
	gmPmCDrawBlack(work, GMD_PM_CEFCT_BLACK_ALPHA);

	// 演出描画
	gmPmCDrawEfctIn(work, 1.0f);

	// メッセージ描画
	gmPmCDrawMessage(work);

	// パッドが接続されてから最低1秒経過していれば終了
	if (AoPadIsConnected()) {
		gmPmGetDcCount() += 1;
		if (gmPmGetDcCount() >= 60) {
			gmPmSetCProc(gmPmCProcEfctOut);
		}
	}
	else {
		gmPmGetDcCount() = 0;
	}
}

// ===========================================================================
//! 退出演出
// ===========================================================================
void gmPmCProcEfctOut(GMS_PMENU_WORK* work)
{
	// カウンタ取得
	u32 count = gmPmGetCProcCount();
	u32 rcount = (u32)(GMD_PM_CEFCT_TIME - count);

	// 黒描画
	gmPmCDrawBlack(
		work, (u8)((GMD_PM_CEFCT_BLACK_ALPHA * rcount) / GMD_PM_CEFCT_TIME));

	// 演出描画
	gmPmCDrawEfctIn(work, (f32)rcount / (f32)GMD_PM_CEFCT_TIME);

	// 演出終了判定
	if (count >= GMD_PM_CEFCT_TIME) {
		gmPmSetCProc(NULL);
	}
}


// ***************************************************************************
// パッド切断用描画
// ***************************************************************************
// ===========================================================================
//! 画面全体黒描画
// ===========================================================================
void gmPmCDrawBlack(GMS_PMENU_WORK* work, u8 alpha)
{
	UNREFERENCED_PARAMETER(work);
	GMS_PMENU_DRAW_WORK* dwork = (GMS_PMENU_DRAW_WORK*)
		amDrawMallocDataBuffer(sizeof(GMS_PMENU_DRAW_WORK));
	dwork->tex_win = AoTexGetTexList(&gmPmGetGlbWork()->tex_win);
	dwork->tex_msg = AoTexGetTexList(&gmPmGetGlbWork()->tex_dcmsg);
	dwork->ivalue = (s32)alpha;
	dwork->fvalue = 0.0f;
	amDrawMakeTask(gmPmCDrawTaskBlack, 0xfffd, (u32)dwork);
}

// ===========================================================================
//! 入り演出描画
// ===========================================================================
void gmPmCDrawEfctIn(GMS_PMENU_WORK* work, f32 rate)
{
	UNREFERENCED_PARAMETER(work);
	GMS_PMENU_DRAW_WORK* dwork = (GMS_PMENU_DRAW_WORK*)
		amDrawMallocDataBuffer(sizeof(GMS_PMENU_DRAW_WORK));
	dwork->tex_win = AoTexGetTexList(&gmPmGetGlbWork()->tex_win);
	dwork->tex_msg = AoTexGetTexList(&gmPmGetGlbWork()->tex_dcmsg);
	dwork->ivalue = 0;
	dwork->fvalue = rate;
	amDrawMakeTask(gmPmCDrawTaskEfctIn, 0xfffe, (u32)dwork);
}

// ===========================================================================
//! メッセージ描画
// ===========================================================================
void gmPmCDrawMessage(GMS_PMENU_WORK* work)
{
	UNREFERENCED_PARAMETER(work);
	GMS_PMENU_DRAW_WORK* dwork = (GMS_PMENU_DRAW_WORK*)
		amDrawMallocDataBuffer(sizeof(GMS_PMENU_DRAW_WORK));
	dwork->tex_win = AoTexGetTexList(&gmPmGetGlbWork()->tex_win);
	dwork->tex_msg = AoTexGetTexList(&gmPmGetGlbWork()->tex_dcmsg);
	dwork->ivalue = 0;
	dwork->fvalue = 0.0f;
	amDrawMakeTask(gmPmCDrawTaskMessage, 0xffff, (u32)dwork);
}

// ===========================================================================
//! 画面全体黒描画タスク
// ===========================================================================
void gmPmCDrawTaskBlack(AMS_TCB* tcb)
{
	// ワーク取得
	GMS_PMENU_DRAW_WORK* work = *((GMS_PMENU_DRAW_WORK**)amTaskGetWork(tcb));

	// パラメータ取得
	u8 alpha = (u8)work->ivalue;

	// 描画前処理
	gmPmCDrawPre();

	// テクスチャ設定
	nnSetPrimitiveTexNum(NULL, -1);

	// 頂点データ作成
	NNS_PRIM3D_PC v[6];
	v[0].Col = (u32)alpha;
	v[1].Col = v[2].Col = v[5].Col = v[0].Col;
	v[0].Pos.x = v[1].Pos.x = 0.0f;
	v[2].Pos.x = v[5].Pos.x = (f32)AOD_ACT_SCREEN_WIDTH;
	v[0].Pos.y = v[2].Pos.y = 0.0f;
	v[1].Pos.y = v[5].Pos.y = (f32)AOD_ACT_SCREEN_HEIGHT;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[5].Pos.z = -2.0f;
	v[3] = v[1];
	v[4] = v[2];

	// ワイド補正
	AoActDrawCorWide(v, 6, AOD_ACT_CORW_NONE);

	// プリミティブ描画
	nnBeginDrawPrimitive3D(
		NNE_PRIM3D_FMT_PC,
		NNE_PRIM_ALPHABLEND_ON,
		NNE_PRIM_LIGHT_DISABLE,
		NNE_PRIM_CULL_NONE);
	nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_LIST, v, 6);
	nnEndDrawPrimitive3D();

	// 描画後処理
	gmPmCDrawPost();
}

// ===========================================================================
//! 入り演出描画タスク
// ===========================================================================
void gmPmCDrawTaskEfctIn(AMS_TCB* tcb)
{
	// ワーク取得
	GMS_PMENU_DRAW_WORK* work = *((GMS_PMENU_DRAW_WORK**)amTaskGetWork(tcb));

	// パラメータ取得
	NNS_TEXLIST* tex = work->tex_win;
	f32 rate = work->fvalue;

	// ウインドウ描画
	const f32 win_x = (f32)AOD_ACT_SCREEN_WIDTH * 0.5f;
	const f32 win_y = (f32)AOD_ACT_SCREEN_HEIGHT * 0.5f;
	const f32 win_w = GMD_PMENU_DCMSG_W;
	const f32 win_h = GMD_PMENU_DCMSG_H;
	AoWinSysDraw(
		AOD_WIN_TYPE_A, tex, 0, win_x, win_y, win_w * rate, win_h * rate);
}

// ===========================================================================
//! メッセージ描画タスク
// ===========================================================================
void gmPmCDrawTaskMessage(AMS_TCB* tcb)
{
	// ワーク取得
	GMS_PMENU_DRAW_WORK* work = *((GMS_PMENU_DRAW_WORK**)amTaskGetWork(tcb));

	// 描画前処理
	gmPmCDrawPre();

	// テクスチャ設定
	nnSetPrimitiveTexNum(work->tex_msg, 0);
	nnSetPrimitiveTexState(
		NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV,
		NNE_PRIM_TEXWRAP_CLAMP, NNE_PRIM_TEXWRAP_CLAMP);

	// プリミティブ描画
	nnBeginDrawPrimitive3D(
		NNE_PRIM3D_FMT_PCT,
		NNE_PRIM_ALPHABLEND_ON,
		NNE_PRIM_LIGHT_DISABLE,
		NNE_PRIM_CULL_NONE);

	NNS_PRIM3D_PCT v[6];
	v[0].Col = v[1].Col = v[2].Col = v[5].Col = 0xffffffff;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[5].Pos.z = -2.0f;
	v[0].Pos.x = v[1].Pos.x =
		(f32)(AOD_ACT_SCREEN_WIDTH - GMD_PMENU_DCMSG_W) * 0.5f;
	v[2].Pos.x = v[5].Pos.x = v[0].Pos.x + (f32)GMD_PMENU_DCMSG_W;
	v[0].Pos.y = v[2].Pos.y =
		(f32)(AOD_ACT_SCREEN_HEIGHT - GMD_PMENU_DCMSG_H) * 0.5f;
	v[1].Pos.y = v[5].Pos.y = v[0].Pos.y + (f32)GMD_PMENU_DCMSG_H;
	v[0].Tex.u = v[1].Tex.u = 0.0f;
	v[2].Tex.u = v[5].Tex.u = 1.0f;
	v[0].Tex.v = v[2].Tex.v = 0.0f;
	v[1].Tex.v = v[5].Tex.v = 1.0f;
	v[3] = v[1];
	v[4] = v[2];
	AoActDrawCorWide(v, 6, AOD_ACT_CORW_CENTER);
	nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_LIST, v, 6);

	nnEndDrawPrimitive3D();

	// 描画後処理
	gmPmCDrawPost();
}

// ===========================================================================
//! 描画前処理
// ===========================================================================
void gmPmCDrawPre(void)
{
	// 描画ステート初期化
	amDrawPushState();
	amDrawInitState();

	// 前処理
	AoActDrawPre();

	// Z更新無効 & Zテスト無効
#if _PS3
	nnSetPrimitive3DAlphaTestPS3(NNE_FALSE);
	nnSetPrimitive3DDepthMaskPS3(NNE_FALSE);
	nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_ALWAYS);
#elif _WII
	nnSetPrimitive3DAlphaCompareGC(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	nnSetPrimitive3DZModeGC(GX_FALSE, GX_ALWAYS, GX_FALSE);
#else
#error
#endif

	// ブレンド設定
#if _PS3
	nnSetPrimitive3DBlendPS3(
		NND_BLENDFUNC_PS3_SRC_ALPHA,
		NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA,
		NND_BLENDOP_PS3_FUNC_ADD);
#elif _WII
	nnSetPrimitive3DBlendModeGC(
		GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
#endif

	// フェード設定
	amDrawSetFog(0);
}

// ===========================================================================
//! 描画後処理
// ===========================================================================
void gmPmCDrawPost(void)
{
	// 描画ステート復帰
	amDrawPopState();
}
#endif // defined(GMD_PMENU_DC_ENABLE)


// ***************************************************************************
// ユーティリティ
// ***************************************************************************
// ===========================================================================
//! 指定アクションのテクスチャ設定
// ===========================================================================
void gmPmUtilSetTexture(u32 act_no)
{
	// グローバルワーク取得
	GMS_PMENU_GLOBAL* gwork = gmPmGetGlbWork();

	// テクスチャファイル選別
	AOS_TEXTURE* tex;
	if (act_no >= ACT_PLNG_S) {
		tex = &gwork->tex_plng;
	}
	else if (act_no >= ACT_PLF_S) {
		tex = &gwork->tex_plf;
	}
	else if (act_no >= ACT_LNG_S) {
		tex = &gwork->tex_lng;
	}
	else {
		tex = &gwork->tex_cmn;
	}

	// テクスチャ設定
	AoActSetTexture(AoTexGetTexList(tex));
}


// ***************************************************************************
// サウンド
// ***************************************************************************
// ===========================================================================
//! SE再生
// ===========================================================================
void gmPmSndPlaySe(GME_PMENU_SE se)
{
	amAssert((u32)se < GMD_PMENU_SE_NUM);

	// グローバルワーク取得
	GMS_PMENU_GLOBAL* gwork = gmPmGetGlbWork();

	// システムUI表示時は起動SEを再生しない
	if ((se == GMD_PMENU_SE_START) && AoSysIsShowPlatformUI()) {
		return;
	}

	char* se_name_tbl[GMD_PMENU_SE_NUM] = {
		"Cursol",		// カーソル移動
		"Ok",			// 決定
		"Cancel",		// キャンセル
		"Pause",		// ポーズメニュー起動時
	};

#if defined(MTD_DEBUG)
	if (!g_gm_pmenu_debug_se_disable) {
		GmSoundPlaySE(se_name_tbl[se], gwork->se_handle);
	}
#else
	GmSoundPlaySE(se_name_tbl[se], gwork->se_handle);
#endif // defined(MTD_DEBUG)
}


// ***************************************************************************
// パッド入力
// ***************************************************************************
// ===========================================================================
//! AoPadStandラッパ
// ===========================================================================
u16 gmPmPadStand(void)
{
#if defined(GMD_PMENU_DC_ENABLE)
	if (gmPmPadIsDisable()) {
		return 0;
	}
#endif // defined(GMD_PMENU_DC_ENABLE)
	return AoPadStand();
}

// ===========================================================================
//! AoPadMStandラッパ
// ===========================================================================
u16 gmPmPadMStand(void)
{
#if defined(GMD_PMENU_DC_ENABLE)
	if (gmPmPadIsDisable()) {
		return 0;
	}
#endif // defined(GMD_PMENU_DC_ENABLE)
	return AoPadMStand();
}

// ===========================================================================
//! AoPadMRepeatラッパ
// ===========================================================================
u16 gmPmPadMRepeat(void)
{
#if defined(GMD_PMENU_DC_ENABLE)
	if (gmPmPadIsDisable()) {
		return 0;
	}
#endif // defined(GMD_PMENU_DC_ENABLE)
	return AoPadMRepeat();
}

// ===========================================================================
//! 切断メッセージ表示によるパッド無効判定
// ===========================================================================
BOOL gmPmPadIsDisable(void)
{
#if defined(GMD_PMENU_DC_ENABLE)
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();
	if (work->tcb) {
		if (((GMS_PMENU_TASK_WORK*)amTaskGetWork(work->tcb))->cproc) {
			return TRUE;
		}
	}
#endif // defined(GMD_PMENU_DC_ENABLE)
	return FALSE;
}


// ***************************************************************************
// 前処理 & 後処理
// ***************************************************************************
// ===========================================================================
//! ファイル読み込み待ち
// ===========================================================================
void gmPmLoadTaskWait(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// ワーク取得
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();
	amAssert(work->state == GMD_PMENU_STATE_LOADING);
	amAssert(work->fs != NULL);

	// ファイル読み込み完了待ち
	if (amFsIsComplete(work->fs)) {

		// ファイルバッファ取得
		void* buf = work->fs->buf;
		work->fs->buf = NULL;
		amFsClearRequest(work->fs);
		work->fs = NULL;

		// アドレス変換
		amConvertAddress(buf);

		// 次ファイルリクエスト発行
		if (work->file_cmn_ama == NULL) {
			work->file_cmn_ama = buf;
			work->fs = gmPmReadRequest(g_gm_pm_file_cmn_amb_name);
		}
		else if (work->file_cmn_amb == NULL) {
			work->file_cmn_amb = buf;
			work->fs = gmPmReadRequest(g_gm_pm_file_lng_ama_name);
		}
		else if (work->file_lng_ama == NULL) {
			work->file_lng_ama = buf;
			work->fs = gmPmReadRequest(
				g_gm_pm_file_lng_amb_name_tbl[GsEnvGetLanguage()]);
		}
		else if (work->file_lng_amb == NULL) {
			work->file_lng_amb = buf;
			work->fs = gmPmReadRequest(g_gm_pm_file_plf_ama_name);
		}
		else if (work->file_plf_ama == NULL) {
			work->file_plf_ama = buf;
			work->fs = gmPmReadRequest(g_gm_pm_file_plf_amb_name);
		}
		else if (work->file_plf_amb == NULL) {
			work->file_plf_amb = buf;
			work->fs = gmPmReadRequest(g_gm_pm_file_plng_ama_name);
		}
		else if (work->file_plng_ama == NULL) {
			work->file_plng_ama = buf;
			work->fs = gmPmReadRequest(
				g_gm_pm_file_plng_amb_name_tbl[GsEnvGetLanguage()]);
		}
		else if (work->file_plng_amb == NULL) {
			work->file_plng_amb = buf;
			work->fs = gmPmReadRequest(g_gm_pm_file_win_amb_name);
		}
		else if (work->file_win_amb == NULL) {
			work->file_win_amb = buf;

#if defined(GMD_PMENU_DC_ENABLE)
			work->fs = gmPmReadRequest(
				g_gm_pm_file_dc_msg_amb_name_tbl[GsEnvGetLanguage()]);
		}
		else if (work->file_dcmsg_amb == NULL) {
			work->file_dcmsg_amb = buf;

#endif // defined(GMD_PMENU_DC_ENABLE)

			// 終了
			amTaskDelete(tcb);
			work->state = GMD_PMENU_STATE_LOADED;
			work->tcb = NULL;
		}
	}
}

// ===========================================================================
//! 構築待ち
// ===========================================================================
void gmPmBuildTaskWait(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// ワーク取得
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();
	amAssert(work->state == GMD_PMENU_STATE_BUILDING);

	// 構築完了判定
	if (AoTexIsLoaded(&work->tex_cmn) &&
		AoTexIsLoaded(&work->tex_lng) &&
		AoTexIsLoaded(&work->tex_plf) &&
		AoTexIsLoaded(&work->tex_plng) &&
		AoTexIsLoaded(&work->tex_win)
#if defined(GMD_PMENU_DC_ENABLE)
		&& AoTexIsLoaded(&work->tex_dcmsg)
#endif // defined(GMD_PMENU_DC_ENABLE)
		)
	{
		// 終了
		amTaskDelete(tcb);
		work->state = GMD_PMENU_STATE_BUILDED;
		work->tcb = NULL;
	}
}

// ===========================================================================
//! 解放待ち
// ===========================================================================
void gmPmFlushTaskWait(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// ワーク取得
	GMS_PMENU_GLOBAL* work = gmPmGetGlbWork();
	amAssert(work->state == GMD_PMENU_STATE_FLUSHING);

	// 解放完了判定
	if (AoTexIsReleased(&work->tex_cmn) &&
		AoTexIsReleased(&work->tex_lng) &&
		AoTexIsReleased(&work->tex_plf) &&
		AoTexIsReleased(&work->tex_plng) &&
		AoTexIsReleased(&work->tex_win)
#if defined(GMD_PMENU_DC_ENABLE)
		&& AoTexIsReleased(&work->tex_dcmsg)
#endif // defined(GMD_PMENU_DC_ENABLE)
		)
	{
		// 終了
		amTaskDelete(tcb);
		work->state = GMD_PMENU_STATE_LOADED;
		work->tcb = NULL;
	}
}


// ***************************************************************************
// 便利
// ***************************************************************************
// ===========================================================================
//! グローバルワーク取得
// ===========================================================================
GMS_PMENU_GLOBAL* gmPmGetGlbWork(void)
{
	return &g_gm_pmenu_work;
}

// ===========================================================================
//! ファイル読み込みリクエスト発行
// ===========================================================================
AMS_FS* gmPmReadRequest(const char* name)
{
	char path[256];
	sprintf(path, "%s%s", g_gm_pm_file_dir_path, name);
	return amFsReadBackground(path);
}

// ===========================================================================
//! リトライ有効判定
// ===========================================================================
BOOL gmPmGlbIsRetryEnable(void)
{
	return TRUE;
}

// ===========================================================================
//! ステセレ戻る有効判定
// ===========================================================================
BOOL gmPmGlbIsBackEnable(void)
{
	// 1-1がクリア済みなら有効
	if (GsMainSysIsStageClear((s32)GSD_MAIN_STAGE_ID_1_1)) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! スペシャルステージ判定
// ===========================================================================
BOOL gmPmGlbIsSpecialStage(void)
{
	u32 id = (u32)(GsGetMainSysInfo()->stage_id);
	if ((id >= (u32)GSD_MAIN_STAGE_ID_SS1) &&
		(id <= (u32)GSD_MAIN_STAGE_ID_SS7))
	{
		return TRUE;
	}
	return FALSE;
}


#if defined(MTD_DEBUG)

// ***************************************************************************
// デバッグ
// ***************************************************************************
// ===========================================================================
//! 開始待ち
// ===========================================================================
void gmPmDgbEvTaskWaitStart(AMS_TCB* tcb)
{
	amPrint(4, 4, "PLEASE PUSH KEY TO START.");
	amPrintf(4, 5, "%c:START", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 終了判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		amTaskDelete(tcb);
		SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
		SyChangeNextEvt();

		AoActSysSetDrawStateEnable();
		AoActSysSetDrawState();
		return;
	}

	// 開始判定
	else if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		GmPauseMenuLoadStart();
		g_gm_pmenu_debug_se_disable = TRUE;
		amTaskSetProcedure(tcb, gmPmDbgEvTaskWaitLoad);
	}
}

// ===========================================================================
//! ファイル読み込み待ち
// ===========================================================================
void gmPmDbgEvTaskWaitLoad(AMS_TCB* tcb)
{
	amPrint(4, 4, "NOW LOADING...");
	if (GmPauseMenuLoadIsFinished()) {
		GmPauseMenuBuildStart();
		amTaskSetProcedure(tcb, gmPmDbgEvTaskWaitBuild);
	}
}

// ===========================================================================
//! 構築待ち
// ===========================================================================
void gmPmDbgEvTaskWaitBuild(AMS_TCB* tcb)
{
	amPrint(4, 4, "NOW BUILDING...");
	if (GmPauseMenuBuildIsFinished()) {
		amTaskSetProcedure(tcb, gmPmDbgEvTaskWaitExecute);
	}
}

// ===========================================================================
//! 実行待ち
// ===========================================================================
void gmPmDbgEvTaskWaitExecute(AMS_TCB* tcb)
{
	amPrint(4, 4, "PLEASE PUSH KEY TO EXECUTE.");
	amPrintf(4, 5, "%c:EXECUTE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:BACK", GsEnvDebugGetCancelKeyChar());
	amPrintf(4, 8, "PREV RESULT : %d", GmPauseMenuGetResult());

	// ワーク取得
	AMS_TCB** tcb_tbl = (AMS_TCB**)amTaskGetWork(tcb);

	// 解放判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		GmPauseMenuFlushStart();
		amTaskSetProcedure(tcb, gmPmDbgEvTaskWaitFlush);
		return;
	}

	// 実行判定
	else if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		tcb_tbl[0] = amTaskMake(gmPmDbgEvTaskPre, 0, 0x0000, 0, 0, "");
		tcb_tbl[1] = amTaskMake(gmPmDbgEvTaskPost, 0, 0xffff, 0, 0, "");
		GmPauseMenuStart(0x2000);
		amTaskSetProcedure(tcb, gmPmDbgEvTaskExecute);
	}
}

// ===========================================================================
//! 実行中
// ===========================================================================
void gmPmDbgEvTaskExecute(AMS_TCB* tcb)
{
	amPrint(4, 4, "EXECUTE.");

	// ワーク取得
	AMS_TCB** tcb_tbl = (AMS_TCB**)amTaskGetWork(tcb);

	// キャンセル判定
	if (AoPadSomeoneStand(KEY_R_UP) >= 0) {
		GmPauseMenuCancel();
	}

	// 終了判定
	if (GmPauseMenuIsFinished()) {
		amTaskDelete(tcb_tbl[0]);
		amTaskDelete(tcb_tbl[1]);
		amTaskSetProcedure(tcb, gmPmDbgEvTaskWaitExecute);
	}
}

// ===========================================================================
//! 解放待ち
// ===========================================================================
void gmPmDbgEvTaskWaitFlush(AMS_TCB* tcb)
{
	amPrint(4, 4, "NOW FLUSHING...");
	if (GmPauseMenuFlushIsFinished()) {
		GmPauseMenuRelease();
		amTaskSetProcedure(tcb, gmPmDgbEvTaskWaitStart);
	}
}

// ===========================================================================
//! 前処理
// ===========================================================================
void gmPmDbgEvTaskPre(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// アクションソートバッファクリア
	AoActSortUnregAll();

	// アクションアキュムレートクリア
	AoActAcmInit();
}

// ===========================================================================
//! 後処理
// ===========================================================================
void gmPmDbgEvTaskPost(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// アクションソート実行
	AoActSortExecute();

	// アクション描画
	AoActSortDraw();

	// アクションソートバッファクリア
	AoActSortUnregAll();

	// 描画タスク生成
	amDrawMakeTask(gmPmDbgEvTaskDraw, (u16)0x8000, (u32)0);
}

// ===========================================================================
//! 描画タスク
// ===========================================================================
void gmPmDbgEvTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(GMD_PMENU_DRAW_STATE);
	amDrawEndScene();
}

#endif // defined(MTD_DEBUG)

// ===========================================================================
//	function
/*!
	説明

	@param param0	[in] 入力引数0説明
	@param param1	[out] 出力ポインタ引数1説明
	@param param2	[io] 入出力ポインタ引数2説明
	@return 返値説明
	@note 補足説明
*/
// ===========================================================================

// ===========================================================================
//	int variable
// ---------------------------------------------------------------------------
//!	変数説明
// ===========================================================================

	// =======================================================================
	//	function
	/*!
		説明

		@param param0	[in] 入力引数0説明
		@param param1	[out] 出力ポインタ引数1説明
		@param param2	[io] 入出力ポインタ引数2説明
		@return 返値説明
		@note 補足説明
	*/
	// =======================================================================

// ***************************************************************************
// ラベル
// ***************************************************************************
#endif //!_IPHONE
