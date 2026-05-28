// ===========================================================================
/*!
	@file	gmClearDemo.cpp
	@brief	デモ・クリアデモ画面

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: gmClearDemo.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#include "gmClearDemo.h"
#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "gmMain.h"
#include "gmObj.h"
#include "gmMap.h"
#include "gmPlayer.h"
#include "gmPlySeq.h"
#include "gmGameDat.h"
#include "gmFix.h"
#include "gmRing.h"
#include "izFade.h"
#include "gmTask.h"
#include "gsSound.h"
#include "gmSound.h"
#include "gmCamera.h"
#include "gmGmkCamScrLim.h"
#include "gmDecoGlare.h"
#include "akUtil.h"
#include "gmPadVib.h"

#include "gsBackup.hpp"
#include "gsBackupStage.hpp"

#include "gmGameDat.h"
#include "gmMainDat.h"
#include "objObject.h"
#include "gmCockpit.h"
#include "hgTrophy.h"

#if _IPHONE
	#include "erTrgAoAction.hpp"
#endif //_IPHONE

// データヘッダ
#include "common/arc/CPIT_MAIN.HMB"
#if !_IPHONE
#include "common/ace/G_RSLT.HMA"
#include "common/ace/G_RSLT_JP.HMA"
#else //!_IPHONE
#include "ace/G_RSLT.HMA"
#include "ace/G_RSLT_JP.HMA"
#endif //!_IPHONE
//#include "common/ace/G_RSLT_US.HMA"

//mpp---------------------
#include "mppUtil.h"
#include "mppAchievementSupport.h"


// ----- Macros ------------------------------------------------（マクロ定義）

#define GMD_CLRDM_FILE_PATH_NUM_MAX		(60)

#define GMD_CLRDM_SCORE_UPDOWN_NUM		(100)
#define GMD_CLRDM_1UP_SCORE				(10000)

#define GMD_CLRDM_NORMAL_IDLE_TIME		(120)
#define GMD_CLRDM_1UP_IDLE_TIME			(270)

#define GMD_CLRDM_PREV_SCORE_IDLE_TIME	(150)
#define GMD_CLRDM_PREV_TIME_IDLE_TIME	(300)

#define GMD_CLRDM_WAIT_SONIC_DISP_TIME	(60)
#define GMD_CLRDM_SONIC_DISP_TIME		(30)
#define GMD_CLRDM_SONIC_NODISP_TIME		(10)

#define DMD_CLRDM_TIME_SCORE_MAX		(80000)
#define DMD_CLRDM_RING_SCORE_MAX		(99900)
#define DMD_CLRDM_TOTAL_SCORE_MAX		(999999990)

#define DMD_CLRDM_INIT_SCORE_NUM		(1000000000)
#define DMD_CLRDM_INIT_RECORD_TIME_NUM	(36000)

#define DMD_CLRDM_EMER_LIGHT_DIST		(74)

#if _IPHONE
#define DMD_CLROM_TEXT_SCALE_X			((fx32)(1.0f * FX32_ONE))						//<文字拡大量
#define DMD_CLROM_TEXT_SCALE_Y			((fx32)(1.0f * DMD_CLROM_TEXT_SCALE_X))			//<文字拡大量
#define DMD_CLROM_SMALL_TEXT_SCALE_X	((fx32)(1.0f * FX32_ONE))						//<小さい文字拡大量
#define DMD_CLROM_SMALL_TEXT_SCALE_Y	((fx32)(1.0f * DMD_CLROM_SMALL_TEXT_SCALE_X))	//<小さい文字拡大量
#endif //_IPHONE
// フェード関連
#define GMD_CLRDM_FADEIN_TIME			(32.0f)
#define GMD_CLRDM_FADEOUT_TIME			(32.0f)

#define GMD_CLRDM_STAGE_SOUND_FADEOUT_TIME	(64)

// フラグ関連
#define GMD_CLRDM_FLAG_EXIT				(1 << 0)		//!< 終了フラグ
#define GMD_CLRDM_FLAG_CANCEL			(1 << 1)		//!< キャンセル
#define GMD_CLRDM_FLAG_SKIP				(1 << 2)		//!< 決定フラグ
#define GMD_CLRDM_FLAG_SCORE_CALC_END	(1 << 3)		//!< スコア算出終了フラグ
#define GMD_CLRDM_FLAG_GET_1UP			(1 << 4)		//!< 1UPかどうか


// 以下は描画関係のみ別のフラグに分けるかも
#define GMD_CLRDM_FLAG_DISP_LINE		(1 << 5)		//!< ライン
#define GMD_CLRDM_FLAG_DISP_SONIC_ICON	(1 << 6)		//!< ソニックアイコン
#define GMD_CLRDM_FLAG_DISP_TIME_SCORE	(1 << 7)		//!< タイムスコア
#define GMD_CLRDM_FLAG_DISP_RING_SCORE	(1 << 8)		//!< リングスコア
#define GMD_CLRDM_FLAG_DISP_TOTAL_SCORE	(1 << 9)		//!< トータルスコア
#define GMD_CLRDM_FLAG_DISP_CLEAR_TIME	(1 << 10)		//!< クリアタイム
#define GMD_CLRDM_FLAG_DISP_TIME_SONIC	(1 << 11)		//!< タイムアタック時のソニックアイコン

#define GMD_CLRDM_FLAG_IS_NEW_RECORD	(1 << 12)		//!< タイムアタック時にNEWRECORDか
#define GMD_CLRDM_FLAG_RETRY_GAME		(1 << 13)		//!< ゲームリトライ決定フラグ

#define GMD_CLRDM_FLAG_BACK_CLEAR_ACT	(1 << 14)		//!< ゲームリトライ決定フラグ

#define GMD_CLRDM_FLAG_CHAOS_EMERALD_GET (1 << 15)		//!< カオスエメラルドを取得したかどうか

#define GMD_CLRDM_FLAG_DISP_SONIC_ICON2	(1 << 16)		//!< ソニックアイコン(2個目)

// 描画フラグ


// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）

typedef enum tag_GME_CLRDM_DATA_TYPE
{
	GME_CLRDM_DATA_TYPE_CMN_DATA = 0,		//!< 共通データ
	GME_CLRDM_DATA_TYPE_LANG_DATA,			//!< 言語別データ
	
	GME_CLRDM_DATA_TYPE_MAX,
	GME_CLRDM_DATA_TYPE_NONE
} GME_CLRDM_DATA_TYPE;


typedef enum tag_GME_CLRDM_SCORE_TYPE
{
	GME_CLRDM_SCORE_TYPE_CALC_DATA = 0,		//!< 計算用スコア
	GME_CLRDM_SCORE_TYPE_DISP_DATA,			//!< 表示用スコア
	
	GME_CLRDM_SCORE_TYPE_MAX,
	GME_CLRDM_SCORE_TYPE_NONE
} GME_CLRDM_SCORE_TYPE;


typedef struct tag_GMS_CLRDM_MAIN_WORK	GMS_CLRDM_MAIN_WORK;

//! メインタスクワーク
struct tag_GMS_CLRDM_MAIN_WORK {
	
	AMS_FS *		ama_fs[GME_CLRDM_DATA_TYPE_MAX];	//!< AMAファイル管理
	AMS_FS *		amb_fs[GME_CLRDM_DATA_TYPE_MAX];	//!< AMBファイル管理
	void *			ama[GME_CLRDM_DATA_TYPE_MAX];		//!< AMAファイル
	void *			amb[GME_CLRDM_DATA_TYPE_MAX];		//!< AMBファイル
	
	AOS_TEXTURE		tex[GME_CLRDM_DATA_TYPE_MAX];		//!< テクスチャ

	GMS_COCKPIT_2D_WORK	*tex_up_act[5];				//!< テキストアクション
	GMS_COCKPIT_2D_WORK	*tex_time_act;				//!< タイムスコアテキスト
	GMS_COCKPIT_2D_WORK	*tex_ring_act;				//!< リングスコアテキスト
	GMS_COCKPIT_2D_WORK	*tex_total_act;				//!< トータルスコアテキスト
	GMS_COCKPIT_2D_WORK	*time_num_act[5];			//!< タイムスコア数字
	GMS_COCKPIT_2D_WORK	*ring_num_act[5];			//!< リングスコア数字
	GMS_COCKPIT_2D_WORK	*total_num_act[9];			//!< トータルスコア数字
	GMS_COCKPIT_2D_WORK	*line_act[3];				//!< ライン
	GMS_COCKPIT_2D_WORK	*sonic_icon_act;			//!< ソニックアイコン
	GMS_COCKPIT_2D_WORK	*sonic_icon_act2;			//!< ソニックアイコン
	
	// タイムアタック用
	GMS_COCKPIT_2D_WORK	*tex_big_time_act;			//!< タイムアタックテキスト
	GMS_COCKPIT_2D_WORK	*record_time_num_act[7];	//!< レコードタイム数字
	GMS_COCKPIT_2D_WORK	*time_sonic_icon_act;		//!< タイムアタック用ソニックアイコン
	GMS_COCKPIT_2D_WORK	*tex_new_record_act;		//!< NEW RECORDS テキスト
	
	GMS_COCKPIT_2D_WORK	*tex_retry_act;				//!< リトライテキスト
	GMS_COCKPIT_2D_WORK	*tex_back_slct_act;			//!< ACT選択戻るテキスト
	GMS_COCKPIT_2D_WORK	*bg_retry;					//!< リトライ背景
#if _IPHONE
	er::CTrgAoAction	trg_retry;					//<リトライトリガ
	GMS_COCKPIT_2D_WORK	*btn_retry[3];				//<リトライ
	er::CTrgAoAction	trg_back;					//<戻るトリガ
	GMS_COCKPIT_2D_WORK	*btn_back[3];				//<戻る
#endif //_IPHONE
	
	// スペステ用
	GMS_COCKPIT_2D_WORK	*tex_spst_up_act[3];		//!< スペステ用テキストアクション
	GMS_COCKPIT_2D_WORK	*spst_num_act[7];			//!< スペステ用ステージ番号アクション
	GMS_COCKPIT_2D_WORK	*icon_emer_up_act[7];		//!< 上段カオスエメラルドアクション
	GMS_COCKPIT_2D_WORK	*icon_emer_down_act[7];		//!< 下段カオスエメラルドアクション
	GMS_COCKPIT_2D_WORK	*icon_emer_light_act;		//!< カオスエメラルド光アクション
	GMS_COCKPIT_2D_WORK	*tex_extend_act;			//!< EXTENDテキスト
	// ソニックアイコンは通常時のものを使いまわす
	

	void (*proc_input)(GMS_CLRDM_MAIN_WORK *);		//!< 入力処理関数
	void (*proc_update)(GMS_CLRDM_MAIN_WORK *);		//!< 表示処理関数
	void (*proc_calc_score)(GMS_CLRDM_MAIN_WORK *);	//!< スコア計算処理関数

	// スコアアタック用
	u32 time_score[GME_CLRDM_SCORE_TYPE_MAX];		//!< タイムスコア保存用変数
	u32 ring_score[GME_CLRDM_SCORE_TYPE_MAX];		//!< リングスコア保存用変数
	u32 total_score[GME_CLRDM_SCORE_TYPE_MAX];		//!< トータルスコア保存用変数
	
	// タイムアタック用
	u32 clear_time;
	u16 time_min;
	u16 time_sec;
	u16 time_msec;

	float timer;										//!< 汎用タイマー
	float flash_timer;								//!< 点滅用タイマー
	u32	flag;										//!< 汎用フラグ
	s32 idle_time;									//!< アイドル時間
	s32 count;										//!< 1フレーム辺りのカウント数(60FPS以外になった場合の補正)
	GSE_GAME_MODE	game_mode;						//!< ゲームモード(ノーマルモードかタイムアタックか)

	// スペステ用
	BOOL is_clear_spe_stg;
	BOOL is_full_eme;
	BOOL is_get_eme;
	s32 has_eme_num;
	s32 get_eme_no;
	BOOL is_first_spe_clear;
	
	s32 next_evt;									//!< 次のイベント
	u32 cur_retry_slct;								//!< 

	u16 stage_id;									//!< ステージID
	u16 prev_spe_stage_id;							//!<
	
	BOOL nodisp_check;
};


//! クリアデモ管理
typedef struct tag_GMS_CLRDM_MGR {
	MTS_TASK_TCB *tcb;	//!< TCB
} GMS_CLRDM_MGR;


// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void gmClearDemoInit(void);
static void gmClearDemoRetryInit(void);
static void gmClearDemoProcMain(MTS_TASK_TCB *tcb);
static void gmClearDemoDest(MTS_TASK_TCB *tcb);

static void gmClearDemoSetMainUpdateProc(GMS_CLRDM_MAIN_WORK *main_work);

static void gmClearDemoProcMoveEfct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcPrevCalcScore(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcCalcScore(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcWaitDispSonic(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcDispIdle(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcFadeOut(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcFinish(GMS_CLRDM_MAIN_WORK *main_work);

static BOOL gmClearDemoIsTexLoad();
static BOOL gmClearDemoIsTexRelease();

inline static s16 gmClearDemoGetRingNum(void);
inline static u32 gmClearDemoGetScore(void);
inline static u32 gmClearDemoGetGameTime(void);
inline static u32 gmClearDemoGetChallengeNum(void);


static void gmClearDemoCreateObjActScore(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoCreateObjActTime(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoCreateObjSpeScore(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoCreateObjSpeTime(GMS_CLRDM_MAIN_WORK *main_work);

static void gmClearDemoCreateObjAct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoCreateObjNormalTimeAtk(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoCreateObjSpecialScoreAtk(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoCreateObjSpecialTimeAtk(GMS_CLRDM_MAIN_WORK *main_work);


static void gmClearDemoCreateObjActForStage(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoCreateObjSpeActForStage(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetSortBufAct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoUpdateAct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetRetryInitAct(GMS_CLRDM_MAIN_WORK *main_work);

static void gmClearDemoSetPlayGameScore(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetSpecialStageScore(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetClearTimeRecord(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetSaveScoreData(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetSpecialStageClearInfo(GMS_CLRDM_MAIN_WORK *main_work);

static void gmClearDemoSetSortBufScore(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetSortBufScoreAct(GMS_COCKPIT_2D_WORK *score_act[], u32 score, u32 digits);
static void gmClearDemoSetTimeAtkSortBufAct(GMS_CLRDM_MAIN_WORK *main_work);

static void gmClearDemoSetCalcScore(GMS_CLRDM_MAIN_WORK *main_work);
//static void gmClearDemoSetCalcScoreForTime(GMS_CLRDM_MAIN_WORK *main_work);
//static void gmClearDemoSetCalcScoreForRing(GMS_CLRDM_MAIN_WORK *main_work);

static void gmClearDemoSetInitDispAct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetScoreData(GMS_CLRDM_MAIN_WORK *main_work);
static u32 gmClearDemoGetTimeScore(s32 clear_time);
static u32 gmClearDemoGetTimeSpeScore(s32 clear_time);
static void gmClearDemoSetDispScore(GMS_COCKPIT_2D_WORK *score_act[], u32 score, u32 digits);
static void gmClearDemoSetFlashSonic(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoTimeFlushEffect(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetSortBufTimeAct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetRetryInput(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetRetrySortBufAct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoSetRetryDispInfo(GMS_CLRDM_MAIN_WORK *main_work);

static void gmClearDemoProcTimeMoveEfct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcTimeWaitTimeEfct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcTimeMoveNewRecord(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcTimeDispEffect(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcRetryStart(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcChangeRetryOut(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcChangeRetryIn(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcWaitSelectRetry(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcWaitRetrySonicRunEfct(GMS_CLRDM_MAIN_WORK *main_work);

static void gmClearDemoProcSpeScoreMoveEfct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeScorePrevCalcScore(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeScoreCalcScore(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeScoreWaitDispSonic(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeScoreDispIdle(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeScoreFadeOut(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeScoreFinish(GMS_CLRDM_MAIN_WORK *main_work);


static void gmClearDemoProcSpeTimeMoveEfct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeTimeTimeMoveNewRecord(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeTimeWaitTimeEfct(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeTimeDispEffect(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeTimeChangeRetryOut(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeTimeChangeRetryIn(GMS_CLRDM_MAIN_WORK *main_work);
static void gmClearDemoProcSpeTimeWaitSelectRetry(GMS_CLRDM_MAIN_WORK *main_work);


#if _IPHONE
static void gmClearDemoSetBgColorBlack(AMS_TCB *tcb = NULL);
#endif //_IPHONE


// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ステージテキストアクションAMBテーブル
const s32 dm_clrdm_stage_text_amb_id[GSD_MAIN_STAGE_ID_MAX] = {
	GME_CLRDM_DATA_TYPE_CMN_DATA,			// ZONE1
	GME_CLRDM_DATA_TYPE_CMN_DATA,
	GME_CLRDM_DATA_TYPE_CMN_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_CMN_DATA,			// ZONE2
	GME_CLRDM_DATA_TYPE_CMN_DATA,
	GME_CLRDM_DATA_TYPE_CMN_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_CMN_DATA,			// ZONE3
	GME_CLRDM_DATA_TYPE_CMN_DATA,
	GME_CLRDM_DATA_TYPE_CMN_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_CMN_DATA,			// ZONE4
	GME_CLRDM_DATA_TYPE_CMN_DATA,
	GME_CLRDM_DATA_TYPE_CMN_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,			// ZONE_FINAL
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,			// SPECIAL STAGE
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
	GME_CLRDM_DATA_TYPE_LANG_DATA,
};

// ステージテキストアクションIDテーブル
const u16 dm_clrdm_stage_text_act_id[GSD_MAIN_STAGE_ID_MAX] = {
	IDA_G_RSLT_JP_ACT_TEX_ACT,			// ZONE1
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_BOS,
	IDA_G_RSLT_JP_ACT_TEX_ACT,			// ZONE2
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_BOS,
	IDA_G_RSLT_JP_ACT_TEX_ACT,			// ZONE3
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_BOS,
	IDA_G_RSLT_JP_ACT_TEX_ACT,			// ZONE4
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_BOS,
	IDA_G_RSLT_JP_ACT_TEX_FINA,			// ZONE_FINAL
	IDA_G_RSLT_JP_ACT_TEX_FINA,
	IDA_G_RSLT_JP_ACT_TEX_FINA,
	IDA_G_RSLT_JP_ACT_TEX_FINA,
	IDA_G_RSLT_JP_ACT_TEX_FINA,
	IDA_G_RSLT_JP_ACT_TEX_ACT,			// SPECIAL STAGE
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_ACT,
	IDA_G_RSLT_JP_ACT_TEX_ACT,
};

// ステージIDアクションIDテーブル
const u16 dm_clrdm_stage_num_act_id[GSD_MAIN_STAGE_ID_MAX] = {
	IDA_G_RSLT_ACT_TEX_1,			// ZONE1
	IDA_G_RSLT_ACT_TEX_2,
	IDA_G_RSLT_ACT_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_TEX_1,			// ZONE2
	IDA_G_RSLT_ACT_TEX_2,
	IDA_G_RSLT_ACT_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_TEX_1,			// ZONE3
	IDA_G_RSLT_ACT_TEX_2,
	IDA_G_RSLT_ACT_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_TEX_1,			// ZONE4
	IDA_G_RSLT_ACT_TEX_2,
	IDA_G_RSLT_ACT_TEX_3,
	0xffff,
	0xffff,							// ZONE_FINAL
	0xffff,
	0xffff,
	0xffff,
	0xffff,
	IDA_G_RSLT_ACT_TEX_1,			// SPECIAL STAGE
	IDA_G_RSLT_ACT_TEX_2,
	IDA_G_RSLT_ACT_TEX_3,
	IDA_G_RSLT_ACT_TEX_3,
	IDA_G_RSLT_ACT_TEX_3,
	IDA_G_RSLT_ACT_TEX_3,
	IDA_G_RSLT_ACT_TEX_3,
};


// ステージIDアクションIDテーブル(ドイツ用)
const u16 dm_clrdm_stage_ge_num_act_id[GSD_MAIN_STAGE_ID_MAX] = {
	IDA_G_RSLT_ACT_GE_TEX_1,			// ZONE1
	IDA_G_RSLT_ACT_GE_TEX_2,
	IDA_G_RSLT_ACT_GE_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_GE_TEX_1,			// ZONE2
	IDA_G_RSLT_ACT_GE_TEX_2,
	IDA_G_RSLT_ACT_GE_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_GE_TEX_1,			// ZONE3
	IDA_G_RSLT_ACT_GE_TEX_2,
	IDA_G_RSLT_ACT_GE_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_GE_TEX_1,			// ZONE4
	IDA_G_RSLT_ACT_GE_TEX_2,
	IDA_G_RSLT_ACT_GE_TEX_3,
	0xffff,
	0xffff,							// ZONE_FINAL
	0xffff,
	0xffff,
	0xffff,
	0xffff,
	IDA_G_RSLT_ACT_GE_TEX_1,			// SPECIAL STAGE
	IDA_G_RSLT_ACT_GE_TEX_2,
	IDA_G_RSLT_ACT_GE_TEX_3,
	IDA_G_RSLT_ACT_GE_TEX_3,
	IDA_G_RSLT_ACT_GE_TEX_3,
	IDA_G_RSLT_ACT_GE_TEX_3,
	IDA_G_RSLT_ACT_GE_TEX_3,
};



// ステージIDアクションIDテーブル(フランス用)
const u16 dm_clrdm_stage_fr_num_act_id[GSD_MAIN_STAGE_ID_MAX] = {
	IDA_G_RSLT_ACT_FR_TEX_1,			// ZONE1
	IDA_G_RSLT_ACT_FR_TEX_2,
	IDA_G_RSLT_ACT_FR_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_FR_TEX_1,			// ZONE2
	IDA_G_RSLT_ACT_FR_TEX_2,
	IDA_G_RSLT_ACT_FR_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_FR_TEX_1,			// ZONE3
	IDA_G_RSLT_ACT_FR_TEX_2,
	IDA_G_RSLT_ACT_FR_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_FR_TEX_1,			// ZONE4
	IDA_G_RSLT_ACT_FR_TEX_2,
	IDA_G_RSLT_ACT_FR_TEX_3,
	0xffff,
	0xffff,							// ZONE_FINAL
	0xffff,
	0xffff,
	0xffff,
	0xffff,
	IDA_G_RSLT_ACT_FR_TEX_1,			// SPECIAL STAGE
	IDA_G_RSLT_ACT_FR_TEX_2,
	IDA_G_RSLT_ACT_FR_TEX_3,
	IDA_G_RSLT_ACT_FR_TEX_3,
	IDA_G_RSLT_ACT_FR_TEX_3,
	IDA_G_RSLT_ACT_FR_TEX_3,
	IDA_G_RSLT_ACT_FR_TEX_3,
};



// ステージIDアクションIDテーブル(スペイン用)
const u16 dm_clrdm_stage_sp_num_act_id[GSD_MAIN_STAGE_ID_MAX] = {
	IDA_G_RSLT_ACT_SP_TEX_1,			// ZONE1
	IDA_G_RSLT_ACT_SP_TEX_2,
	IDA_G_RSLT_ACT_SP_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_SP_TEX_1,			// ZONE2
	IDA_G_RSLT_ACT_SP_TEX_2,
	IDA_G_RSLT_ACT_SP_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_SP_TEX_1,			// ZONE3
	IDA_G_RSLT_ACT_SP_TEX_2,
	IDA_G_RSLT_ACT_SP_TEX_3,
	0xffff,
	IDA_G_RSLT_ACT_SP_TEX_1,			// ZONE4
	IDA_G_RSLT_ACT_SP_TEX_2,
	IDA_G_RSLT_ACT_SP_TEX_3,
	0xffff,
	0xffff,							// ZONE_FINAL
	0xffff,
	0xffff,
	0xffff,
	0xffff,
	IDA_G_RSLT_ACT_SP_TEX_1,			// SPECIAL STAGE
	IDA_G_RSLT_ACT_SP_TEX_2,
	IDA_G_RSLT_ACT_SP_TEX_3,
	IDA_G_RSLT_ACT_SP_TEX_3,
	IDA_G_RSLT_ACT_SP_TEX_3,
	IDA_G_RSLT_ACT_SP_TEX_3,
	IDA_G_RSLT_ACT_SP_TEX_3,
};



// ステージIDテキストアクションIDテーブル
const u16 dm_clrdm_stage_tex_act_id[GSD_MAIN_STAGE_ID_MAX] = {
	IDA_G_RSLT_JP_ACT_TEX_MADE,			// ZONE1
	IDA_G_RSLT_JP_ACT_TEX_MADE,
	IDA_G_RSLT_JP_ACT_TEX_MADE,
	IDA_G_RSLT_JP_ACT_TEX_DEF,
	IDA_G_RSLT_JP_ACT_TEX_MADE,			// ZONE2
	IDA_G_RSLT_JP_ACT_TEX_MADE,
	IDA_G_RSLT_JP_ACT_TEX_MADE,
	IDA_G_RSLT_JP_ACT_TEX_DEF,
	IDA_G_RSLT_JP_ACT_TEX_MADE,			// ZONE3
	IDA_G_RSLT_JP_ACT_TEX_MADE,
	IDA_G_RSLT_JP_ACT_TEX_MADE,
	IDA_G_RSLT_JP_ACT_TEX_DEF,
	IDA_G_RSLT_JP_ACT_TEX_MADE,			// ZONE4
	IDA_G_RSLT_JP_ACT_TEX_MADE,
	IDA_G_RSLT_JP_ACT_TEX_MADE,
	IDA_G_RSLT_JP_ACT_TEX_DEF,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,		// ZONE_FINAL
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,			// SPECIAL STAGE
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
};


// ステージIDテキストアクションIDテーブル
const u16 dm_clrdm_stage_ge_tex_act_id[GSD_MAIN_STAGE_ID_MAX] = {
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,			// ZONE1
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_DEF,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,			// ZONE2
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_DEF,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,			// ZONE3
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_DEF,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,			// ZONE4
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_DEF,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,		// ZONE_FINAL
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,			// SPECIAL STAGE
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
	IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR,
};


// 
static const s32 g_gm_clear_demo_data_ama_id[GSD_LANGUAGE_NUM] = {
	IDB_CPIT_MAIN_G_RSLT_JP_AMA,	//日本語
	IDB_CPIT_MAIN_G_RSLT_US_AMA,	//英語
	IDB_CPIT_MAIN_G_RSLT_FR_AMA,	//フランス語
	IDB_CPIT_MAIN_G_RSLT_IT_AMA,	//イタリア語
	IDB_CPIT_MAIN_G_RSLT_GE_AMA,	//ドイツ語
	IDB_CPIT_MAIN_G_RSLT_SP_AMA,	//スペイン語
};

// 
static const s32 g_gm_clear_demo_data_amb_id[GSD_LANGUAGE_NUM] = {
	IDB_CPIT_MAIN_G_RSLT_JP_AMB,	//日本語
	IDB_CPIT_MAIN_G_RSLT_US_AMB,	//英語
	IDB_CPIT_MAIN_G_RSLT_FR_AMB,	//フランス語
	IDB_CPIT_MAIN_G_RSLT_IT_AMB,	//イタリア語
	IDB_CPIT_MAIN_G_RSLT_GE_AMB,	//ドイツ語
	IDB_CPIT_MAIN_G_RSLT_SP_AMB,	//スペイン語
};

// タイムスコアのクリアタイムテーブル(通常演出時)
const s32 dm_clrdm_clear_time_sec_tbl[GSD_MAIN_STAGE_ID_MAX][9] = {
	{60 * 1 +  0, 60 * 1 + 10, 60 * 1 + 20, 60 * 1 + 40, 60 * 2 +  0, 60 * 2 + 30, 60 * 3 +  0, 60 * 3 + 40, 60 * 4 + 20},	// 1-1
	{60 * 1 + 10, 60 * 1 + 20, 60 * 1 + 30, 60 * 1 + 50, 60 * 2 + 10, 60 * 2 + 40, 60 * 3 + 10, 60 * 3 + 50, 60 * 4 + 30},	// 1-2
	{60 * 1 + 20, 60 * 1 + 30, 60 * 1 + 40, 60 * 2 +  0, 60 * 2 + 20, 60 * 2 + 50, 60 * 3 + 20, 60 * 4 +  0, 60 * 4 + 40},	// 1-3
	{60 * 0 + 40, 60 * 0 + 45, 60 * 0 + 50, 60 * 1 +  0, 60 * 1 + 10, 60 * 1 + 30, 60 * 1 + 50, 60 * 2 + 20, 60 * 2 + 50},	// 1-4
	{60 * 1 + 20, 60 * 1 + 30, 60 * 1 + 40, 60 * 2 +  0, 60 * 2 + 20, 60 * 2 + 50, 60 * 3 + 20, 60 * 4 +  0, 60 * 4 + 40},	// 2-1
	{60 * 2 + 50, 60 * 3 +  0, 60 * 3 + 10, 60 * 3 + 30, 60 * 3 + 50, 60 * 4 + 20, 60 * 4 + 50, 60 * 5 + 30, 60 * 6 + 10},	// 2-2
	{60 * 2 + 30, 60 * 2 + 40, 60 * 2 + 50, 60 * 3 + 10, 60 * 3 + 30, 60 * 4 +  0, 60 * 4 + 30, 60 * 5 + 10, 60 * 5 + 50},	// 2-3
	{60 * 1 +  0, 60 * 1 +  5, 60 * 1 + 10, 60 * 1 + 20, 60 * 1 + 30, 60 * 1 + 50, 60 * 2 + 10, 60 * 2 + 40, 60 * 3 + 10},	// 2-4
	{60 * 1 + 40, 60 * 1 + 50, 60 * 2 +  0, 60 * 2 + 20, 60 * 2 + 40, 60 * 3 + 10, 60 * 3 + 40, 60 * 4 + 20, 60 * 5 +  0},	// 3-1
	{60 * 2 + 50, 60 * 3 +  0, 60 * 3 + 10, 60 * 3 + 30, 60 * 3 + 50, 60 * 4 + 20, 60 * 4 + 50, 60 * 5 + 30, 60 * 6 + 10},	// 3-2
	{60 * 2 +  0, 60 * 2 + 10, 60 * 2 + 20, 60 * 2 + 40, 60 * 3 +  0, 60 * 3 + 30, 60 * 4 +  0, 60 * 4 + 40, 60 * 5 + 20},	// 3-3
	{60 * 1 + 40, 60 * 1 + 45, 60 * 1 + 50, 60 * 2 +  0, 60 * 2 + 10, 60 * 2 + 30, 60 * 2 + 50, 60 * 3 + 20, 60 * 3 + 50},	// 3-4
	{60 * 1 + 40, 60 * 1 + 50, 60 * 2 +  0, 60 * 2 + 20, 60 * 2 + 40, 60 * 3 + 10, 60 * 3 + 40, 60 * 4 + 20, 60 * 5 +  0},	// 4-1
	{60 * 3 +  0, 60 * 3 + 10, 60 * 3 + 20, 60 * 3 + 40, 60 * 4 +  0, 60 * 4 + 30, 60 * 5 +  0, 60 * 5 + 40, 60 * 6 + 20},	// 4-2
	{60 * 2 + 10, 60 * 2 + 20, 60 * 2 + 30, 60 * 2 + 50, 60 * 3 + 10, 60 * 3 + 40, 60 * 4 + 10, 60 * 4 + 50, 60 * 5 + 30},	// 4-3
	{60 * 1 + 20, 60 * 1 + 25, 60 * 1 + 30, 60 * 1 + 40, 60 * 1 + 50, 60 * 2 + 10, 60 * 2 + 30, 60 * 3 +  0, 60 * 3 + 30},	// 4-4
	{60 * 6 +  0, 60 * 6 +  5, 60 * 6 + 10, 60 * 6 + 20, 60 * 6 + 30, 60 * 6 + 50, 60 * 7 + 10, 60 * 7 + 40, 60 * 8 + 10},	// F-1
	{60 * 6 +  0, 60 * 6 +  5, 60 * 6 + 10, 60 * 6 + 20, 60 * 6 + 30, 60 * 6 + 50, 60 * 7 + 10, 60 * 7 + 40, 60 * 8 + 10},	// F-2
	{60 * 6 +  0, 60 * 6 +  5, 60 * 6 + 10, 60 * 6 + 20, 60 * 6 + 30, 60 * 6 + 50, 60 * 7 + 10, 60 * 7 + 40, 60 * 8 + 10},	// F-3
	{60 * 6 +  0, 60 * 6 +  5, 60 * 6 + 10, 60 * 6 + 20, 60 * 6 + 30, 60 * 6 + 50, 60 * 7 + 10, 60 * 7 + 40, 60 * 8 + 10},	// F-4
	{60 * 6 +  0, 60 * 6 +  5, 60 * 6 + 10, 60 * 6 + 20, 60 * 6 + 30, 60 * 6 + 50, 60 * 7 + 10, 60 * 7 + 40, 60 * 8 + 10},	// F-5
	{60 * 0 + 30, 60 * 0 + 50, 60 * 1, 60 * 1 + 30, 60 * 2, 60 * 3, 60 * 4, 60 * 5, 60 * 6},	// S-1(未使用)
	{60 * 0 + 30, 60 * 0 + 50, 60 * 1, 60 * 1 + 30, 60 * 2, 60 * 3, 60 * 4, 60 * 5, 60 * 6},	// S-2(未使用)
	{60 * 0 + 30, 60 * 0 + 50, 60 * 1, 60 * 1 + 30, 60 * 2, 60 * 3, 60 * 4, 60 * 5, 60 * 6},	// S-3(未使用)
	{60 * 0 + 30, 60 * 0 + 50, 60 * 1, 60 * 1 + 30, 60 * 2, 60 * 3, 60 * 4, 60 * 5, 60 * 6},	// S-4(未使用)
	{60 * 0 + 30, 60 * 0 + 50, 60 * 1, 60 * 1 + 30, 60 * 2, 60 * 3, 60 * 4, 60 * 5, 60 * 6},	// S-5(未使用)
	{60 * 0 + 30, 60 * 0 + 50, 60 * 1, 60 * 1 + 30, 60 * 2, 60 * 3, 60 * 4, 60 * 5, 60 * 6},	// S-6(未使用)
	{60 * 0 + 30, 60 * 0 + 50, 60 * 1, 60 * 1 + 30, 60 * 2, 60 * 3, 60 * 4, 60 * 5, 60 * 6},	// S-7(未使用)
	{60 * 0 + 30, 60 * 0 + 50, 60 * 1, 60 * 1 + 30, 60 * 2, 60 * 3, 60 * 4, 60 * 5, 60 * 6},	// ENDING(未使用)
};


// タイムスコアのスコアテーブル(通常演出時)
const u32 dm_clrdm_clear_time_score_tbl[9] = {
	80000,			// 
	50000,			// 
	10000,			// 
	5000,			// 
	4000,			// 
	3000,			// 
	2000,			// 
	1000,			// 
	500,			// 
};


// テキストアクションテーブル(通常演出時)
const u16 dm_clrdm_act_id_tbl[5] = {
	0,				// 
	1,				// 
	2,				// 
	3,				// 
	0,				// 
};


const s32 dm_clrdm_spe_stg_get_act_no[GSD_MAIN_STAGE_ID_MAX] = {
	1,		// 1-1
	2,		// 1-2
	3,		// 1-3
	0,		// 1-4
	4,		// 2-1
	5,		// 2-2
	6,		// 2-3
	0,		// 2-4
	7,		// 3-1
	8,		// 3-2
	9,		// 3-3
	0,		// 3-4
	10,		// 4-1
	11,		// 4-2
	12,		// 4-3
	0,		// 4-4
	0,		// F-1
	0,		// F-2
	0,		// F-3
	0,		// F-4
	0,		// F-5
	0,		// S-1
	0,		// S-2
	0,		// S-3
	0,		// S-4
	0,		// S-5
	0,		// S-6
	0,		// S-7
};


const u16 dm_clrdm_spe_stg_get_prev_act_no[13] = {
	0,		//<未所持
	0,	//<ゾーン1アクト1
	1,	//<ゾーン1アクト2
	2,	//<ゾーン1アクト3
	4,	//<ゾーン2アクト1
	5,	//<ゾーン2アクト2
	6,	//<ゾーン2アクト3
	8,	//<ゾーン3アクト1
	9,	//<ゾーン3アクト2
	10,	//<ゾーン3アクト3
	12,	//<ゾーン4アクト1
	13,	//<ゾーン4アクト2
	14,	//<ゾーン4アクト3
};



//管理情報
static GMS_CLRDM_MGR gm_clrdm_mgr;
static GMS_CLRDM_MGR *gm_clrdm_mgr_p = NULL;

static void *gm_clrdm_amb[GME_CLRDM_DATA_TYPE_MAX] = {NULL, NULL};
static AOS_TEXTURE gm_clrdm_tex[GME_CLRDM_DATA_TYPE_MAX];

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ==========================================================================
// GmClearDemoBuild
/*!
 *	クリアデモデータ構築
 */
// ==========================================================================
void GmClearDemoBuild(void)
{
	int i = 0;
	s32 lang = (s32)GsEnvGetLanguage();
	
	if (lang >= 1) {
		lang = 2 * lang;
	}
	else {
		lang = 0;
	}

	// 管理情報初期化
	amZeroMemory(&gm_clrdm_mgr, sizeof(GMS_CLRDM_MGR));
	gm_clrdm_mgr_p = &gm_clrdm_mgr;
	
	for (i = 0; i < GME_CLRDM_DATA_TYPE_MAX; i++) {
		amZeroMemory(&gm_clrdm_tex[i], sizeof(AOS_TEXTURE));
	}

	// AMBファイルロード
	gm_clrdm_amb[0] = ObjDataLoadAmbIndex(NULL
										, IDB_CPIT_MAIN_G_RSLT_AMB
										, g_gm_gamedat_cockpit_main_arc
										);
	
	gm_clrdm_amb[1] = ObjDataLoadAmbIndex(NULL
										, g_gm_clear_demo_data_amb_id[GsEnvGetLanguage()]
//										, IDB_CPIT_MAIN_G_RSLT_JP_AMB + lang
										, g_gm_gamedat_cockpit_main_arc
										);
	
	// アドレス変換
	for (i = 0; i < GME_CLRDM_DATA_TYPE_MAX; i++) {
		amConvertAddress(gm_clrdm_amb[i]);
	}
	
	// テクスチャ構築
	for (i = 0; i < GME_CLRDM_DATA_TYPE_MAX; i++) {
		// テクスチャ構築開始
		AoTexBuild(&gm_clrdm_tex[i], gm_clrdm_amb[i]);
		AoTexLoad(&gm_clrdm_tex[i]);
	}
}

// ==========================================================================
// GmClearDemoBuildCheck
/*!
 *	クリアデモデータ構築 終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL GmClearDemoBuildCheck(void)
{
	// テクスチャ構築チェック
	if (gmClearDemoIsTexLoad()) {
		// フラグ扱いでON
		return (TRUE);
	}

	return (FALSE);
}


// ==========================================================================
// GmClearDemoFlush
/*!
 *	クリアデモデータフラッシュ
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
void GmClearDemoFlush(void)
{
	// テクスチャ解放
	for (int i = 0; i < GME_CLRDM_DATA_TYPE_MAX; i++) {
		AoTexRelease(&gm_clrdm_tex[i]);
	}
}

// ==========================================================================
// GmClearDemoFlushCheck
/*!
 *	クリアデモデータフラッシュ終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL GmClearDemoFlushCheck(void)
{
	// テクスチャ解放
	if (gmClearDemoIsTexRelease()) {
#if !defined HOG_INLINE3_ROM
//		IzFadeExit();
#endif
		
		return (TRUE);
	}
	
	return (FALSE);
}

// ==========================================================================
// GmClearDemoStart
/*!
	クリアデモ画面開始処理
 */
// ==========================================================================
void GmClearDemoStart(void)
{
	gmClearDemoInit();
}


// ==========================================================================
// GmClearDemoExit
/*!
	クリアデモ画面強制終了処理
 */
// ==========================================================================
void GmClearDemoExit(void)
{
	// タスククリア
	if (gm_clrdm_mgr_p->tcb) {
		mtTaskClearTcb(gm_clrdm_mgr_p->tcb);

		gm_clrdm_mgr_p->tcb = NULL;
	}

	// クリアデモ終了フラグON(システムに設定)
//	g_gm_main_system.game_flag |= GMD_GAME_FLAG_RESULT_END;
}



// ==========================================================================
// GmClearDemoIsExit
/*!
	クリアデモ画面終了チェック処理
 */
// ==========================================================================
BOOL GmClearDemoIsExit(void)
{
	if (!gm_clrdm_mgr_p->tcb) {
		return TRUE;
	}

	return FALSE;
}



// ==========================================================================
// GmClearDemoRetryStart
/*!
	リトライ画面開始処理
 */
// ==========================================================================
void GmClearDemoRetryStart(void)
{
	// リトライ画面のみを表示する要の初期化処理
	gmClearDemoRetryInit();
}


// ----- Static Functions --------------------（スタティック関数の定義：局所）
// ==========================================================================
// gmClearDemoInit
/*!
	クリアデモ画面初期化処理
 */
// ==========================================================================
void gmClearDemoInit(void)
{
	MTS_TASK_TCB		*tcb;
	GMS_CLRDM_MAIN_WORK	*main_work;
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();

	// メインタスク作成
	tcb = MTM_TASK_MAKE_TCB(gmClearDemoProcMain
							, gmClearDemoDest
							, 0
							, GMD_TASK_PAUSELEVEL_DEF
							, GMD_TASK_PRIO_CLEARDEMO
							, GMD_TASK_GROUP_CLEARDEMO
							, sizeof(GMS_CLRDM_MAIN_WORK)
							, "CLRDM_MAIN"
							);
	
	// TCBをグローバルに保存
	gm_clrdm_mgr_p->tcb = tcb;
	
	// ワーク初期化
	main_work = (GMS_CLRDM_MAIN_WORK *)mtTaskGetTcbWork(tcb);
	
	// 初期化処理があればここに記述
	for (int i = 0; i < GME_CLRDM_DATA_TYPE_MAX; i++) {
		main_work->tex[i] = gm_clrdm_tex[i];
	}

#if _IPHONE	//当たり判定構築
	{	//リトライ
		er::CTrgAoAction &trg = main_work->trg_retry;
		new(&trg) er::CTrgAoAction();
	}
	{	//戻る
		er::CTrgAoAction &trg = main_work->trg_back;
		new(&trg) er::CTrgAoAction();
	}
#endif //_IPHONE	//当たり判定構築

	// カウンタ設定		// 現状、仮でフレームレート60とする
	main_work->count = 60 / 60;

	// ステージ番号取得		※gsMainSysから取得する
	main_work->stage_id = gs_main->stage_id;

	// ここでモード取得(ノーマルモードかタイムアタックか)
	main_work->game_mode = gs_main->game_mode;

	// そのステージで取得したスコアを設定(ノーマル・タイムアタック・スペステの３つでシーケンスを切り替える)
	if (main_work->stage_id <= GSD_MAIN_STAGE_ID_FINAL_5) {
		main_work->is_clear_spe_stg = FALSE;
		
		gmClearDemoSetPlayGameScore(main_work);
	}
	// スペステ用スコアセット
	else {
		main_work->is_clear_spe_stg = TRUE;
		
		gmClearDemoSetSpecialStageScore(main_work);
		
		// スペステのクリアデータ取得
		gmClearDemoSetSpecialStageClearInfo(main_work);
	}
	
	// セーブ領域の構造体にデータを反映(セーブそのものは行わない)
	gmClearDemoSetSaveScoreData(main_work);

	// オブジェクト2Dアクション構築
	if (main_work->stage_id >= GSD_MAIN_STAGE_ID_SS1
		&& main_work->stage_id < GSD_MAIN_STAGE_ID_MAX) {
		if (main_work->game_mode == GSD_GAME_MODE_STORY) {
			// スペステスコアアタック
			gmClearDemoCreateObjSpeScore(main_work);
			gmClearDemoCreateObjSpecialScoreAtk(main_work);
			
			// 初期非表示設定
			gmClearDemoSetInitDispAct(main_work);
		}
		else {
			// スペステタイムアタック
			gmClearDemoCreateObjSpeTime(main_work);
			gmClearDemoCreateObjSpecialTimeAtk(main_work);
		}
	}
	
	else {
		if (main_work->game_mode == GSD_GAME_MODE_STORY) {
			// 通常ステージスコアアタック
			gmClearDemoCreateObjActScore(main_work);
			gmClearDemoCreateObjAct(main_work);
			
			// 初期非表示設定
			gmClearDemoSetInitDispAct(main_work);
		}
		else {
			// 通常ステージタイムアタック
			gmClearDemoCreateObjActTime(main_work);
			gmClearDemoCreateObjNormalTimeAtk(main_work);
		}
	}
	
	// リザルト用じんぐる再生の暫定処理
	if (gs_main->stage_id >= GSD_MAIN_STAGE_ID_FINAL_1
		&& gs_main->stage_id <= GSD_MAIN_STAGE_ID_FINAL_5) {
		GmSoundPlayClearFinal();
	}
	else {
		GmSoundPlayClear();
	}
	
	// プロシージャ設定(ノーマル・タイムアタック・スペステの３つでシーケンスを切り替える)
	gmClearDemoSetMainUpdateProc(main_work);
}



// ==========================================================================
// gmClearDemoRetryInit
/*!
	クリアデモ画面リトライのみ用初期化処理
 */
// ==========================================================================
void gmClearDemoRetryInit(void)
{
	MTS_TASK_TCB		*tcb;
	GMS_CLRDM_MAIN_WORK	*main_work;
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();

	// メインタスク作成
	tcb = MTM_TASK_MAKE_TCB(gmClearDemoProcMain
							, gmClearDemoDest
							, 0
							, GMD_TASK_PAUSELEVEL_DEF
							, GMD_TASK_PRIO_CLEARDEMO
							, GMD_TASK_GROUP_CLEARDEMO
							, sizeof(GMS_CLRDM_MAIN_WORK)
							, "CLRDM_MAIN"
							);
	
	// TCBをグローバルに保存
	gm_clrdm_mgr_p->tcb = tcb;
	
	// ワーク初期化
	main_work = (GMS_CLRDM_MAIN_WORK *)mtTaskGetTcbWork(tcb);
	
	// 初期化処理があればここに記述
	for (int i = 0; i < GME_CLRDM_DATA_TYPE_MAX; i++) {
		main_work->tex[i] = gm_clrdm_tex[i];
	}

#if _IPHONE	//当たり判定構築
	{	//リトライ
		er::CTrgAoAction &trg = main_work->trg_retry;
		new(&trg) er::CTrgAoAction();
	}
	{	//戻る
		er::CTrgAoAction &trg = main_work->trg_back;
		new(&trg) er::CTrgAoAction();
	}
#endif //_IPHONE	//当たり判定構築

	// カウンタ設定		// 現状、仮でフレームレート60とする
	main_work->count = 60 / 60;

	// ステージ番号取得		※gsMainSysから取得する
	main_work->stage_id = gs_main->stage_id;
	
	if (main_work->stage_id >= GSD_MAIN_STAGE_ID_SS1) {
		main_work->is_clear_spe_stg = TRUE;
	}
	else {
		main_work->is_clear_spe_stg = FALSE;
	}

	// ここでモード取得(タイムアタック以外はASSERT)
	main_work->game_mode = gs_main->game_mode;
	MTM_ASSERT(main_work->game_mode == GSD_GAME_MODE_TIME_ATTACK);

	// オブジェクト生成
	gmClearDemoCreateObjActTime(main_work);
	
	// オブジェクト2Dアクション構築
	// 通常ステージタイムアタック
	gmClearDemoCreateObjNormalTimeAtk(main_work);

	// 初期非表示設定
//	gmClearDemoSetInitDispAct(main_work);
	gmClearDemoSetRetryInitAct(main_work);

	// プロシージャ設定
	main_work->proc_update = gmClearDemoProcRetryStart;
}



// ==========================================================================
// gmClearDemoSetMainUpdateProc
/*!
	更新プロシージャ分岐設定処理
 */
// ==========================================================================
void gmClearDemoSetMainUpdateProc(GMS_CLRDM_MAIN_WORK *main_work)
{
	// スペステのリザルト時の処理
	if (main_work->stage_id >= GSD_MAIN_STAGE_ID_SS1) {
		// スコアアタック時
		if (main_work->game_mode == GSD_GAME_MODE_STORY) {
			main_work->proc_update = gmClearDemoProcSpeScoreMoveEfct;
		}
		
		// タイムアタック時
		else {
			main_work->proc_update = gmClearDemoProcSpeTimeMoveEfct;
			gmClearDemoSetClearTimeRecord(main_work);
			gmClearDemoSetSortBufTimeAct(main_work);
			
			// NEW_RECORDテキストのスピードは0にする
			main_work->tex_new_record_act->obj_2d.speed = 0.0f;
		}
	}
	// 通常ACTのリザルト
	else {
		// スコアアタック時
		if (main_work->game_mode == GSD_GAME_MODE_STORY) {
			main_work->proc_update = gmClearDemoProcMoveEfct;
		}
		
		// タイムアタック時
		else {
			main_work->proc_update = gmClearDemoProcTimeMoveEfct;
			gmClearDemoSetClearTimeRecord(main_work);
			gmClearDemoSetSortBufTimeAct(main_work);
			
			// NEW_RECORDテキストのスピードは0にする
			main_work->tex_new_record_act->obj_2d.speed = 0.0f;
		}
	}
}



// ==========================================================================
// gmClearDemoSetPlayGameScore
/*!
	ゲーム中で取得したスコアの設定処理
 */
// ==========================================================================
void gmClearDemoSetPlayGameScore(GMS_CLRDM_MAIN_WORK *main_work)
{
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();
	
	// ゲーム中のスコアを加算
	gs_main->clear_score = gmClearDemoGetScore();
	
	// タイムスコアのタイムから得点を計算
	gs_main->clear_time = (s32)gmClearDemoGetGameTime();
	
	// 取得リング数から得点を計算
	gs_main->clear_ring = gmClearDemoGetRingNum();
	
	// メインシステムからタイムスコアを取得
	main_work->time_score[GME_CLRDM_SCORE_TYPE_CALC_DATA] = 0;
	main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
		= gmClearDemoGetTimeScore(gs_main->clear_time);
	
	// リングスコアを取得
	main_work->ring_score[GME_CLRDM_SCORE_TYPE_CALC_DATA] = 0;
	main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] = gs_main->clear_ring * 100;
	
	if (main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] > DMD_CLRDM_RING_SCORE_MAX) {
		main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] = DMD_CLRDM_RING_SCORE_MAX;
	}
	
	// 現在までのトータルスコアを取得
	main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] = gs_main->clear_score;
	
	// 計算結果側の変数にタイムスコアとリングスコア分を足した値を設定
	main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA]
		= main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			+ main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			+ main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA];
	
	if (main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA] > DMD_CLRDM_TOTAL_SCORE_MAX) {
		main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA] = DMD_CLRDM_TOTAL_SCORE_MAX;
	}
}



// ==========================================================================
// gmClearDemoSetSaveScoreData
/*!
	ゲームクリア時のタイム設定処理
 */
// ==========================================================================
void gmClearDemoSetSaveScoreData(GMS_CLRDM_MAIN_WORK *main_work)
{
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();
	u32 tmp_stage_id = 0;
	u32 save_hi_score = 0;
	bool sonic_type = 0;
	
	
	// システムバックアップインスタンス作成
	gs::backup::SSystem &sys_data = gs::backup::SSystem::CreateInstance();
	
	
	if (main_work->stage_id >= GSD_MAIN_STAGE_ID_SS1) {
		// スペシャルステージデータインスタンス作成
		gs::backup::SSpecial &spe_data
			= gs::backup::SSpecial::CreateInstance();
		
		tmp_stage_id = (u32)(main_work->stage_id - (GSD_MAIN_STAGE_ID_SS1 - gs::backup::ESpecialStage::Stage1));
		
		if (GMM_MAIN_GOAL_AS_SUPER_SONIC()) {
			// スペステでスーパーソニックは使用不可のため、アサート
			MTM_ASSERT(0);
		}
		
		if (gs_main->game_mode == GSD_GAME_MODE_STORY) {
			if (main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA] >= 10) {
				save_hi_score = (u32)(main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA]);// / 10);
			}
			else {
				save_hi_score = 0;
			}
			
			// 2回目以降のゴールの場合、カオスエメラルドが1upなので残機を加算
			if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_CHAOSGET
				&& main_work->is_first_spe_clear == FALSE) {
				// 実際に一機アップするのはこのタイミング
			    GmPlayerStockGet(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P], 1);
			}
			
			if (spe_data[tmp_stage_id].GetHighScore() < save_hi_score
				|| spe_data[tmp_stage_id].GetHighScore() == DMD_CLRDM_INIT_SCORE_NUM) {
				// セーブするのはカオスエメラルドをとったときだけ
				if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_CHAOSGET) {
					// スペシャルステージのハイスコア取得
					spe_data[tmp_stage_id].SetHighScore(save_hi_score);
					// アップロードチェック設定
					spe_data[tmp_stage_id].SetHighScoreUploaded(false);
					
					// カオスエメラルド取得フラグON
					main_work->flag |= GMD_CLRDM_FLAG_CHAOS_EMERALD_GET;
				}
			}
		}
		
		else if (gs_main->game_mode != GSD_GAME_MODE_TIME_ATTACK) {
			// スコアアタック・タイムアタックでもないとき、ASSERT
			MTM_ASSERT(0);
		}
		
		else {
			// 2回目以降のゴールの場合、カオスエメラルドが1upなので残機を加算
			if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_CHAOSGET
				&& main_work->is_first_spe_clear == FALSE) {
				// タイムアタックでゴールしたら必ず一機アップ
			    GmPlayerStockGet(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P], 1);
			}
			
			if (spe_data[tmp_stage_id].GetFastTime() < (u32)gs_main->clear_time
				|| spe_data[tmp_stage_id].GetFastTime() == DMD_CLRDM_INIT_RECORD_TIME_NUM) {
				// セーブするのはカオスエメラルドをとったときだけ
				if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_CHAOSGET) {
					// スペシャルステージのレコード取得
					spe_data[tmp_stage_id].SetFastTime((u32)gs_main->clear_time);
					// アップロードチェック設定
					spe_data[tmp_stage_id].SetFastTimeUploaded(false);
					
					// レコードタイム更新フラグON
					main_work->flag |= GMD_CLRDM_FLAG_IS_NEW_RECORD;
				}
			}
		}
	}

	else {
		// 通常ステージデータインスタンス作成
		gs::backup::SStage &data
			= gs::backup::SStage::CreateInstance();
		
		tmp_stage_id = (u32)(main_work->stage_id);
		
		// FINALステージ補正
		if (tmp_stage_id > GSD_MAIN_STAGE_ID_FINAL_1
			&& tmp_stage_id <= GSD_MAIN_STAGE_ID_FINAL_5) {
			tmp_stage_id = GSD_MAIN_STAGE_ID_FINAL_1;
		}
		
#if _WII
		// 最終クリアACTをセーブ		// ※クリアしたときのみ？
		sys_data.SetLastClearAct((gs::backup::EStage::Type)tmp_stage_id);
#endif
		
		if (GMM_MAIN_USE_SUPER_SONIC()) {
			sonic_type = true;
		}
		else {
			sonic_type = false;
		}
		
		// スーパーソニックでゴールパネルを通過した場合(ゲームモードの判別なし)
		if (GMM_MAIN_GOAL_AS_SUPER_SONIC()) {
			// スーパーソニックでゴールパネルを通過したセーブデータを書き込み
			data[tmp_stage_id].SetUseSuperSonicOnce(true);
		}
		
		
		if (gs_main->game_mode == GSD_GAME_MODE_STORY) {
			if (main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA] >= 10) {
				save_hi_score = (u32)(main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA]);// / 10);
			}
			else {
				save_hi_score = 0;
			}
			
			// スコアアタックのみ初プレイかどうかのチェックあり
			if (data[tmp_stage_id].GetHighScore(false) == DMD_CLRDM_INIT_SCORE_NUM) {
				g_gs_main_sys_info.is_first_play = TRUE;
			}
			else {
				g_gs_main_sys_info.is_first_play = FALSE;
			}
			
			
			if (data[tmp_stage_id].GetHighScore(sonic_type) < save_hi_score
				|| data[tmp_stage_id].GetHighScore(sonic_type) == DMD_CLRDM_INIT_SCORE_NUM) {
				// 通常ステージのハイスコア取得
				data[tmp_stage_id].SetHighScore(save_hi_score, sonic_type);
				// アップロードチェック設定
				data[tmp_stage_id].SetHighScoreUploaded(sonic_type, false);
			}
			
			// スペステリング突入した場合
			if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPECIAL_STAGE) {
				// スペステリングに入ったステージIDを保存
				g_gs_main_sys_info.prev_stage_id = g_gs_main_sys_info.stage_id;
			}
			else {
				g_gs_main_sys_info.prev_stage_id = 0xffff;
			}
		}
		
		else if (gs_main->game_mode != GSD_GAME_MODE_TIME_ATTACK) {
			// スコアアタック・タイムアタックでもないとき、ASSERT
			MTM_ASSERT(0);
		}
		
		else {
			if (data[tmp_stage_id].GetFastTime(sonic_type) > (u32)gs_main->clear_time
				|| data[tmp_stage_id].GetFastTime(sonic_type) == DMD_CLRDM_INIT_RECORD_TIME_NUM) {
				
				if (sonic_type == true) {
					if (data[tmp_stage_id].GetFastTime(false) > (u32)gs_main->clear_time
						|| data[tmp_stage_id].GetFastTime(false) == DMD_CLRDM_INIT_RECORD_TIME_NUM) {
						// レコードタイム更新フラグON
						main_work->flag |= GMD_CLRDM_FLAG_IS_NEW_RECORD;
					}
				}
				else {
					if (data[tmp_stage_id].GetFastTime(true) > (u32)gs_main->clear_time
						|| data[tmp_stage_id].GetFastTime(true) == DMD_CLRDM_INIT_RECORD_TIME_NUM) {
						// レコードタイム更新フラグON
						main_work->flag |= GMD_CLRDM_FLAG_IS_NEW_RECORD;
					}
				}
				
				// 通常ステージのレコード取得
				data[tmp_stage_id].SetFastTime((u32)gs_main->clear_time, sonic_type);
				// アップロードチェック設定
				data[tmp_stage_id].SetFastTimeUploaded(sonic_type, false);
				
			}
		}
	}
	
	// 累計敵討伐数設定
	sys_data.SetKilled(gs_main->ene_kill_count);
	
	// 残機数設定
	sys_data.SetPlayerStock(gs_main->rest_player_num);
	
	// ファイナルステージの場合
	if (gs_main->stage_id >= GSD_MAIN_STAGE_ID_FINAL_1
		&& gs_main->stage_id <= GSD_MAIN_STAGE_ID_FINAL_5) {
		// ゲームクリアカウント加算
		HgTrophyIncFinalClearCount();
		
		sys_data.SetClearCount(gs_main->final_clear_count);
	}
}



// ==========================================================================
// gmClearDemoSetClearTimeRecord
/*!
	ゲームクリア時のタイム設定処理
 */
// ==========================================================================
void gmClearDemoSetClearTimeRecord(GMS_CLRDM_MAIN_WORK *main_work)
{
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();
	OBS_OBJECT_WORK		*obj_work = NULL;
	
	// タイム取得
	main_work->clear_time = (u32)gs_main->clear_time;
	
	// セーブデータから既存のタイムレコードを取得
	
	
	// 既存のタイムレコードより今回クリアした際のタイムの方が速い場合、NEW_RECORDフラグON
//	if () {
//		;
//	}
//	
//	
	// クリア時の状態がスーパーソニックかどうか
	
	
	// 取得したクリアタイムを分・秒・ミリ秒に変換
	AkUtilFrame60ToTime(main_work->clear_time
						, &main_work->time_min
						, &main_work->time_sec
						, &main_work->time_msec
						);
	
	main_work->record_time_num_act[0]->obj_2d.speed = 0.0f;
	main_work->record_time_num_act[2]->obj_2d.speed = 0.0f;
	main_work->record_time_num_act[3]->obj_2d.speed = 0.0f;
	main_work->record_time_num_act[5]->obj_2d.speed = 0.0f;
	main_work->record_time_num_act[6]->obj_2d.speed = 0.0f;
	main_work->record_time_num_act[1]->obj_2d.speed = 0.0f;
	main_work->record_time_num_act[4]->obj_2d.speed = 0.0f;
	
	
	// タイム表示設定フラグON
	for (int i = 0; i < 7; i++) {
		obj_work = (OBS_OBJECT_WORK *)main_work->record_time_num_act[i];
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
	}
	
	// NEW_RECORDフラグONならば新記録をセーブ		※リザルト終了時？
	
}



// ==========================================================================
// gmClearDemoSetSpecialStageScore
/*!
	スペステクリア時のスコア設定処理
 */
// ==========================================================================
void gmClearDemoSetSpecialStageScore(GMS_CLRDM_MAIN_WORK *main_work)
{
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();
	
	// ゲーム中のスコアを加算
	gs_main->clear_score = gmClearDemoGetScore();
	
	// タイムスコアのタイムから得点を計算
	gs_main->clear_time = (s32)gmClearDemoGetGameTime();
	
	// 取得リング数から得点を計算
	gs_main->clear_ring = gmClearDemoGetRingNum();
	
	// メインシステムからタイムスコアを取得
	main_work->time_score[GME_CLRDM_SCORE_TYPE_CALC_DATA] = 0;
	main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
		= gmClearDemoGetTimeSpeScore(gs_main->clear_time);		// 仮
//	main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] = 24000;		// 仮
	
	// リングスコアを取得
	main_work->ring_score[GME_CLRDM_SCORE_TYPE_CALC_DATA] = 0;
	main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] = gs_main->clear_ring * 100;	// 仮
//	main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] = 4800;		// 仮
	
	if (main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] > DMD_CLRDM_RING_SCORE_MAX) {
		main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] = DMD_CLRDM_RING_SCORE_MAX;
	}
	
	// 現在までのトータルスコアを取得
	main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] = gs_main->clear_score;		// 仮
//	main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] = 84200;		// 仮
	
	// 計算結果側の変数にタイムスコアとリングスコア分を足した値を設定
	main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA]
		= main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			+ main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			+ main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA];
	
	if (main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA] > DMD_CLRDM_TOTAL_SCORE_MAX) {
		main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA] = DMD_CLRDM_TOTAL_SCORE_MAX;
	}
}



// ==========================================================================
// gmClearDemoSetSpecialStageClearInfo
/*!
	スペステクリア時のクリア情報設定処理
 */
// ==========================================================================
void gmClearDemoSetSpecialStageClearInfo(GMS_CLRDM_MAIN_WORK *main_work)
{
	u32 tmp_stage_id = 0;
	u16 set_stage_id = 0;
	
	if (g_gs_main_sys_info.prev_stage_id != 0xffff) {
		set_stage_id = g_gs_main_sys_info.prev_stage_id;
	}
	else {
		set_stage_id = 0xffff;
	}
	
	// スペシャルステージデータインスタンス作成
	gs::backup::SSpecial &spe_data
		= gs::backup::SSpecial::CreateInstance();
	
	tmp_stage_id = (u32)(main_work->stage_id - (GSD_MAIN_STAGE_ID_SS1 - gs::backup::ESpecialStage::Stage1));
	
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD) {
		main_work->is_full_eme = TRUE;
	}
	
	// プレイしたスペステでカオスエメラルドを取得したとき
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_CHAOSGET) {
		
		////success
		main_work->is_get_eme = TRUE;
		
		if (spe_data[tmp_stage_id].IsGetEmerald() == false
			&& set_stage_id != 0xffff) {
			spe_data[tmp_stage_id].SetEmeraldStage((gs::backup::SSpecialSolo::EEmeraldStage::Type)dm_clrdm_spe_stg_get_act_no[set_stage_id]);
			
			main_work->is_first_spe_clear = TRUE;
		}
		
		main_work->get_eme_no = (s32)tmp_stage_id;
		
		// S-7でエメラルドを取得した＝全て取得になるので、フラグ設定
		if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_SS7) {
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD;
			///mppAchievementSupport::get()->event_ChaosEmeraldCollected(7);
		}
		
		// 取得済みのエメラルド数を設定
		main_work->has_eme_num = (s32)(tmp_stage_id + 1);
		
		// 取得済みのエメラルド数を設定
		for (int i = 7; i > 0; i--) {
			if (spe_data[(u32)(i - 1)].IsGetEmerald()) {
				main_work->has_eme_num = i;
				
				break;
			}
		}
		mppAchievementSupport::get()->event_ChaosEmeraldCollected();
	}	
	else {
		////fail
		main_work->is_get_eme = FALSE;
		main_work->get_eme_no = -1;
		
		// 取得済みのエメラルド数を設定
		for (int i = 7; i > 0; i--) {
			if (spe_data[(u32)(i - 1)].IsGetEmerald()) {
				main_work->has_eme_num = i;
				
				break;
			}
		}

	}
	

}



// ==========================================================================
// gmClearDemoGetTimeScore
/*!
	クリア時のタイムからスコアを取得する処理
 */
// ==========================================================================
u32 gmClearDemoGetTimeScore(s32 clear_time)
{
	GSS_MAIN_SYS_INFO	*gs_main = GsGetMainSysInfo();
	u32 result = 0;
	u16 check_time_min = 0;
	u16 check_time_sec = 0;
	u16 check_time_msec = 0;
	int i = 0;
	s32 cmp_time_sec = 0;
	
	
	AkUtilFrame60ToTime((u32)clear_time
						, &check_time_min
						, &check_time_sec
						, &check_time_msec
						);
	
	// 秒単位の値を保存(ミリ秒は無視)
	cmp_time_sec = check_time_min * 60 + check_time_sec;

	for (i = 0; i < 9; i++) {
		if (cmp_time_sec < dm_clrdm_clear_time_sec_tbl[gs_main->stage_id][i]) {
			result = dm_clrdm_clear_time_score_tbl[i];

			break;
		}
	}
	
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_TIMEOVER) {
		result = 0;
	}
	
	return result;
}



// ==========================================================================
// gmClearDemoGetTimeSpeScore
/*!
	クリア時のタイムからスコアを取得する処理
 */
// ==========================================================================
u32 gmClearDemoGetTimeSpeScore(s32 clear_time)
{
	u32 result = 0;
	u16 check_time_min = 0;
	u16 check_time_sec = 0;
	u16 check_time_msec = 0;
	s32 cmp_time_sec = 0;
	
	
	AkUtilFrame60ToTime((u32)clear_time
						, &check_time_min
						, &check_time_sec
						, &check_time_msec
						);
	
	// 秒単位の値を保存(ミリ秒は無視)
	cmp_time_sec = check_time_min * 60 + check_time_sec;
	
	// スペステスコアは残り時間(秒単位) * 100点
	result = (u32)(cmp_time_sec * 100);
	
	// スペステ!ブロックに衝突した場合、強制に0点
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_FAILED) {
		result = 0;
	}
	
	// タイムアタックの場合、強制に0点
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_TIMEOVER) {
		result = 0;
	}
	
	return result;
}



// ==========================================================================
// gmClearDemoSetInitDispAct
/*!
	各アクションの初期非表示設定処理
 */
// ==========================================================================
void gmClearDemoSetInitDispAct(GMS_CLRDM_MAIN_WORK *main_work)
{
	OBS_OBJECT_WORK		*obj_work;
	int i = 0;

	
	for (i = 0; i < 5; i++) {
		obj_work = (OBS_OBJECT_WORK *)main_work->time_num_act[i];
		obj_work->disp_flag |= OBD_DISP_NODISP;
		
		obj_work = (OBS_OBJECT_WORK *)main_work->ring_num_act[i];
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	
	for (i = 0; i < 9; i++) {
		obj_work = (OBS_OBJECT_WORK *)main_work->total_num_act[i];
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}

	for (i = 0; i < 3; i++) {
		obj_work = (OBS_OBJECT_WORK *)main_work->line_act[i];
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	
	obj_work = (OBS_OBJECT_WORK *)main_work->sonic_icon_act;
	obj_work->disp_flag |= OBD_DISP_NODISP;
	
	obj_work = (OBS_OBJECT_WORK *)main_work->sonic_icon_act2;
	obj_work->disp_flag |= OBD_DISP_NODISP;
}



// ==========================================================================
// gmClearDemoProcMain
/*!
	クリアデモ画面メインプロシージャ処理
 */
// ==========================================================================
void gmClearDemoProcMain(MTS_TASK_TCB *tcb)
{
	GMS_CLRDM_MAIN_WORK	*main_work;

	// ワーク取得
	main_work = (GMS_CLRDM_MAIN_WORK *)mtTaskGetTcbWork(tcb);

	// 終了処理
	if (main_work->flag & GMD_CLRDM_FLAG_EXIT) {
		// タスククリア
		if (gm_clrdm_mgr_p->tcb) {
			mtTaskClearTcb(gm_clrdm_mgr_p->tcb);
			gm_clrdm_mgr_p->tcb = NULL;
		}
		
		// スペステリング突入した場合
		if (main_work->stage_id < GSD_MAIN_STAGE_ID_SS1) {
			if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPECIAL_STAGE) {
				// スペステリングに入ったステージIDを保存
				g_gs_main_sys_info.prev_stage_id = g_gs_main_sys_info.stage_id;
			}
			else {
				g_gs_main_sys_info.prev_stage_id = 0xffff;
			}
		}
		
		// ここで通常ACT→スペステ時に失敗した場合、ステージIDを変更
		if (main_work->flag & GMD_CLRDM_FLAG_BACK_CLEAR_ACT) {
			
//			g_gs_main_sys_info.stage_id = main_work->prev_spe_stage_id;
			
			main_work->flag &= ~GMD_CLRDM_FLAG_BACK_CLEAR_ACT;
		}
		
		// クリアデモ(リトライ時も)終了フラグON(システムに設定)
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_RESULT_END;
		
		if (main_work->flag & GMD_CLRDM_FLAG_RETRY_GAME) {
			g_gm_main_system.game_flag |= GMD_GAME_FLAG_GAMESYS_RESTART;
			
			main_work->flag &= ~GMD_CLRDM_FLAG_RETRY_GAME;
		}
		
		return;
	}

	// キャンセルフラグチェック

	// 更新プロシージャ
	if (main_work->proc_update) {
		main_work->proc_update(main_work);
	}
}


// ==========================================================================
// gmClearDemoDest
/*!
	クリアデモ画面終了処理
 */
// ==========================================================================
void gmClearDemoDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
	
}


// ==========================================================================
// 通常ステージ(スペステ以外)のノーマルモード時の演出シーケンス
// ==========================================================================
// ==========================================================================
// gmClearDemoProcMoveEfct
/*!
	テキスト移動演出中処理
 */
// ==========================================================================
void gmClearDemoProcMoveEfct(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 最後の移動アクションが終端になったら
	if (AoActIsEndTrs(main_work->tex_total_act->obj_2d.act)) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcPrevCalcScore;
		
		// ライン表示設定フラグON
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_LINE);
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_TOTAL_SCORE);
	}
	
	// 演出終了チェック
	if (AoActIsEndTrs(main_work->tex_time_act->obj_2d.act)) {
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_TIME_SCORE);
	}
	
	if (AoActIsEndTrs(main_work->tex_ring_act->obj_2d.act)) {
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_RING_SCORE);
	}

	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);

	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);
}



// ==========================================================================
// gmClearDemoProcPrevCalcScore
/*!
	スコア加減算演出前の待ち処理
 */
// ==========================================================================
void gmClearDemoProcPrevCalcScore(GMS_CLRDM_MAIN_WORK *main_work)
{
	// タイマー更新
	main_work->timer += main_work->count;
	
	// スコア計算までの規定時間がたったら
	if (main_work->timer > GMD_CLRDM_PREV_SCORE_IDLE_TIME) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcCalcScore;

		// スコア計算処理関数設定
		main_work->proc_calc_score = gmClearDemoSetCalcScore;

		// タイマー初期化
		main_work->timer = 0;
		
//		return;
	}

	// SKIPフラグ処理
#if !_IPHONE
	if ((AoPadStand() & GSD_KEY_DECIDE)
		|| (AoPadStand() & GSD_KEY_CANCEL)
		|| (AoPadStand() & KEY_START)) {
#else //!_IPHONE
	if (amTpIsTouchPush(0)) {
#endif //!_IPHONE
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_SKIP);
	}

	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);

	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);

}



// ==========================================================================
// gmClearDemoProcCalcScore
/*!
	スコア加減算演出中処理
 */
// ==========================================================================
void gmClearDemoProcCalcScore(GMS_CLRDM_MAIN_WORK *main_work)
{
	
	// タイマー更新
	main_work->timer += main_work->count;
	
	// スコア算出演出が終わったら、またはスキップフラグONならば
	if (main_work->flag & GMD_CLRDM_FLAG_SKIP
		|| main_work->flag & GMD_CLRDM_FLAG_SCORE_CALC_END) {
		// 表示待ちプロシージャへ
		main_work->proc_update = gmClearDemoProcWaitDispSonic;
		
		// 表示用スコアに算出結果のスコアを代入
		main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			= main_work->time_score[GME_CLRDM_SCORE_TYPE_CALC_DATA];
		
		main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			= main_work->ring_score[GME_CLRDM_SCORE_TYPE_CALC_DATA];
		
		main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			= main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA];
		
		// 
		main_work->proc_calc_score = NULL;
		
		amFlagOff(main_work->flag, GMD_CLRDM_FLAG_SKIP);
		amFlagOff(main_work->flag, GMD_CLRDM_FLAG_SCORE_CALC_END);

		// 1UPの場合
		if (main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] >= GMD_CLRDM_1UP_SCORE) {
			amFlagOn(main_work->flag, GMD_CLRDM_FLAG_GET_1UP);
			main_work->idle_time = GMD_CLRDM_1UP_IDLE_TIME;
		}
		else {
			main_work->idle_time = GMD_CLRDM_NORMAL_IDLE_TIME;
		}
		
		// 実績設定
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_CLEAR_DEMO);

		main_work->timer = 0;
//		return;
	}

	// SKIPフラグ処理
#if !_IPHONE
	if ((AoPadStand() & GSD_KEY_DECIDE)
		|| (AoPadStand() & GSD_KEY_CANCEL)
		|| (AoPadStand() & KEY_START)) {
#else //!_IPHONE
	if (amTpIsTouchPush(0)) {
#endif //!_IPHONE
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_SKIP);
	}

	// スコア加減処理
	if (main_work->proc_calc_score
		&& main_work->timer > 1) {
		main_work->proc_calc_score(main_work);

		main_work->timer = 0;
	}

	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);

	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);

	// スコアカウントサウンド処理
	if (!main_work->proc_calc_score) {
		GmSoundPlaySE("Result2");
	}
	else if (!main_work->timer) {
		GmSoundPlaySE("Result1");
	}
	
}

	
// ==========================================================================
// gmClearDemoProcWaitDispSonic
/*!
	演出終了後のソニックアイコン表示待ち処理
 */
// ==========================================================================
	
static bool mpp_needToflushAchievementsToGlobalNet_afterGeneralLevelEnd = false;
	
void gmClearDemoProcWaitDispSonic(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 表示待ち時間が過ぎたらフェードアウトへ
	if (main_work->timer >= GMD_CLRDM_WAIT_SONIC_DISP_TIME) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcDispIdle;
		main_work->timer = 0;
		
		mpp_needToflushAchievementsToGlobalNet_afterGeneralLevelEnd = true;
		
		if (main_work->flag & GMD_CLRDM_FLAG_GET_1UP) {
			GmSoundPlayJingle(GME_SOUND_JINGLE_IDX_1UP);
			
			// 実際に一機アップするのはこのタイミング
	        GmPlayerStockGet(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P], 1);
		}
		
		return;
	}
	
	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);

	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);

	// タイマー更新
	main_work->timer += main_work->count;

}



// ==========================================================================
// gmClearDemoProcDispIdle
/*!
	演出終了後のアイドル中処理(1up時の表示・BGM再生処理も行う)
 */
// ==========================================================================
void gmClearDemoProcDispIdle(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 表示待ち時間が過ぎたらフェードアウトへ
	if (main_work->timer >= main_work->idle_time) {
		
		if(mpp_needToflushAchievementsToGlobalNet_afterGeneralLevelEnd == true) {
			mpp_needToflushAchievementsToGlobalNet_afterGeneralLevelEnd = false;
			mpp_checkAchievementsForSuccessLevelEnd(); //general level end
			mpp_flushAchievementsToGlobalNet(true); //general level end
			//＼＼mppUtil::sendScore(g_gs_main_sys_info.game_mode,g_gs_main_sys_info.stage_id,main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA],main_work->time_min,main_work->time_sec,main_work->time_msec);
		}
		
		
		if(mppUtil::isAchievementAlertShown()) //sss
			return;

		// 終了処理へ
		main_work->proc_update = gmClearDemoProcFadeOut;
//		main_work->timer = 0;
		
		// フェードアウト開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , GMD_CLRDM_FADEOUT_TIME
					   );
		
//		return;
	}
	
	// ソニックアイコン点滅処理
	if (main_work->flag & GMD_CLRDM_FLAG_GET_1UP) {
		gmClearDemoSetFlashSonic(main_work);
	}

	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);

	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);

	// タイマー更新
	main_work->timer += main_work->count;

}



// ==========================================================================
// gmClearDemoProcFadeOut
/*!
	フェードアウト中処理
 */
// ==========================================================================
void gmClearDemoProcFadeOut(GMS_CLRDM_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
//		IzFadeExit();
		
		// 遷移先なし
		main_work->proc_update = gmClearDemoProcFinish;
		
		main_work->timer = 0;
		
		return;
	}
	
	// ソニックアイコン点滅処理
	if (main_work->flag & GMD_CLRDM_FLAG_GET_1UP) {
		gmClearDemoSetFlashSonic(main_work);
	}
	
	if (main_work->game_mode == GSD_GAME_MODE_STORY) {
		// ソートバッファ登録
		gmClearDemoSetSortBufAct(main_work);
		
		// スコアデータ更新
		gmClearDemoSetScoreData(main_work);
		
		// アクション更新
		gmClearDemoUpdateAct(main_work);
	}
	
	// タイマー更新
	main_work->timer += main_work->count;
}



// ==========================================================================
// gmClearDemoProcFinish
/*!
	終了設定処理
 */
// ==========================================================================
void gmClearDemoProcFinish(GMS_CLRDM_MAIN_WORK *main_work)
{
	main_work->proc_update = NULL;

#if _IPHONE	//当たり判定解放
	{	//リトライ
		er::CTrgAoAction &trg = main_work->trg_retry;
		trg.Release();
		trg.~CTrgAoAction();
	}
	{	//戻る
		er::CTrgAoAction &trg = main_work->trg_back;
		trg.Release();
		trg.~CTrgAoAction();
	}
#endif //_IPHONE	//当たり判定解放
	
	// タスク終了フラグON
	amFlagOn(main_work->flag, GMD_CLRDM_FLAG_EXIT);
}

// ==========================================================================
// 通常ステージ(スペステ以外)のタイムアタックモード時の演出シーケンス
// ==========================================================================
// ==========================================================================
// gmClearDemoProcTimeMoveEfct
/*!
	テキスト移動演出中処理
 */
// ==========================================================================
void gmClearDemoProcTimeMoveEfct(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 最後の大TIMEアクションが終端になったら
	if (AoActIsEndTrs(main_work->tex_big_time_act->obj_2d.act)) {
		// ここでNEW RECORDかどうかを判定して分岐させる
		main_work->proc_update = gmClearDemoProcTimeWaitTimeEfct;
		
		for (int i = 0; i < 7; i++) {
			ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->record_time_num_act[i]
											   , &main_work->record_time_num_act[i]->obj_2d
											   , NULL
											   , NULL
											   , IDB_CPIT_MAIN_G_RSLT_AMA
											   , GmGameDatGetCockpitData()
											   , AoTexGetTexList(&main_work->tex[0])
											   , (u32)(IDA_G_RSLT_ACT_TA_TIME1 + i)
											   , FALSE
											   );
#if _IPHONE
			((OBS_OBJECT_WORK *)main_work->record_time_num_act[i])->scale.x = DMD_CLROM_TEXT_SCALE_X;
			((OBS_OBJECT_WORK *)main_work->record_time_num_act[i])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
		}
		
		gmClearDemoSetClearTimeRecord(main_work);
		gmClearDemoSetSortBufTimeAct(main_work);
		
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_CLEAR_TIME);
		
		// ソニックアイコン表示フラグON
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_TIME_SONIC);
	}

	// ソートバッファ登録
	gmClearDemoSetTimeAtkSortBufAct(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);
}



// ==========================================================================
// gmClearDemoProcTimeWaitTimeEfct
/*!
	NEW RECORDテキスト待ち演出中処理
 */
// ==========================================================================
void gmClearDemoProcTimeWaitTimeEfct(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 最後の大TIMEアクションが終端になったら
	if (main_work->timer > 60.f) {
		// ここでNEW RECORDかどうかを判定して分岐させる
		if (main_work->flag & GMD_CLRDM_FLAG_IS_NEW_RECORD) {
			main_work->proc_update = gmClearDemoProcTimeMoveNewRecord;
			
			// NEWRECORDテキストのスピードを戻す
			main_work->tex_new_record_act->obj_2d.speed = 1.0f;
			
			main_work->idle_time = 180;
		}
		else {
			main_work->proc_update = gmClearDemoProcTimeDispEffect;
			
			main_work->idle_time = 120;
		}
		
		// 実績設定
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_CLEAR_DEMO);
		
		main_work->timer = 0.f;
	}

	// ソートバッファ登録
	gmClearDemoSetTimeAtkSortBufAct(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);
	
	// 
	if (main_work->timer > 120.f) {
		return;
	}
	else {
		// タイマー更新
		main_work->timer += main_work->count;
	}
}



// ==========================================================================
// gmClearDemoProcTimeMoveNewRecord
/*!
	NEW RECORDテキスト移動演出中処理
 */
// ==========================================================================
void gmClearDemoProcTimeMoveNewRecord(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 最後の大TIMEアクションが終端になったら
	if (AoActIsEndTrs(main_work->tex_new_record_act->obj_2d.act)) {
		// NEWRECORD取得なのでタイム部分を光らせる演出へ遷移
		main_work->proc_update = gmClearDemoProcTimeDispEffect;
		
		// NEWレコード更新ジングル再生
		GmSoundPlayJingle(GME_SOUND_JINGLE_IDX_NEW_RECORD);
	}

	// ソートバッファ登録
	gmClearDemoSetTimeAtkSortBufAct(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);
}



// ==========================================================================
// gmClearDemoProcTimeDispEffect
/*!
	レコードタイム表示演出処理
 */
// ==========================================================================
void gmClearDemoProcTimeDispEffect(GMS_CLRDM_MAIN_WORK *main_work)
{
	// タイマー更新
	main_work->timer += main_work->count;

#if !_IPHONE
	if (AoPadStand() & GSD_KEY_DECIDE) {
#else //!_IPHONE
	if (amTpIsTouchPush(0)) {
#endif //!_IPHONE	
		main_work->flag |= GMD_CLRDM_FLAG_SKIP;
	}
	
	// スコア計算までの規定時間がたったら
	if (main_work->timer > main_work->idle_time
		|| main_work->flag & GMD_CLRDM_FLAG_SKIP) {
		// 終了処理へ
		//＼＼mpp_checkAchievementsForSuccessLevelEnd(); //time attack end
		main_work->proc_update = gmClearDemoProcChangeRetryOut;
		
		// フェードアウト
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , 32.f
					   );

		// タイマー初期化
		main_work->timer = 0;
		
		main_work->flag &= ~GMD_CLRDM_FLAG_SKIP;
		
//		return;
	}
	
	// タイム部の閃光演出
	gmClearDemoTimeFlushEffect(main_work);

	// ソートバッファ登録
	gmClearDemoSetTimeAtkSortBufAct(main_work);
}



// ==========================================================================
// gmClearDemoProcRetryStart
/*!
	リトライ開始プロシージャ
 */
// ==========================================================================
void gmClearDemoProcRetryStart(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 終了処理へ
	main_work->proc_update = gmClearDemoProcChangeRetryOut;

	// フェードアウト
	IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
				   , IZE_FADE_TYPE_BLACK_FADEOUT
				   , 32.f
				   );

	// タイマー初期化
	main_work->timer = 0;
	
}



// ==========================================================================
// gmClearDemoProcChangeRetryOut
/*!
	リトライ画面切り替えフェードアウト中処理
 */
// ==========================================================================
void gmClearDemoProcChangeRetryOut(GMS_CLRDM_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
				
		
		if (main_work->nodisp_check) {
						
			if(main_work->time_min>0 || main_work->time_sec>0) {//sss[102]
				mpp_checkAchievementsForSuccessLevelEnd(); //time attack end
				mpp_flushAchievementsToGlobalNet(true); //time attack end		
				mppUtil::sendScore(g_gs_main_sys_info.game_mode,g_gs_main_sys_info.stage_id,main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA],main_work->time_min,main_work->time_sec,main_work->time_msec);	
			}			
			
			// フェードイン開始
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEIN
						   , 32.f
						   );
			
			main_work->proc_update = gmClearDemoProcChangeRetryIn;
			
			main_work->nodisp_check	= FALSE;
			
			return;
		}
		else {
			// リトライ用表示設定
			gmClearDemoSetRetryDispInfo(main_work);
			
			gmClearDemoSetRetrySortBufAct(main_work);
			
			main_work->nodisp_check	= TRUE;
		}
	}
	
	// ソートバッファ登録
	gmClearDemoSetTimeAtkSortBufAct(main_work);
}



// ==========================================================================
// gmClearDemoProcChangeRetryIn
/*!
	リトライ画面切り替えフェードイン中処理
 */
// ==========================================================================
void gmClearDemoProcChangeRetryIn(GMS_CLRDM_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		
		if(mppUtil::isAchievementAlertShown()) //sss
			return;		
		
		main_work->proc_update = gmClearDemoProcWaitSelectRetry;
	}
}



// ==========================================================================
// gmClearDemoProcWaitSelectRetry
/*!
	演出終了後のソニックアイコン表示待ち処理
 */
// ==========================================================================
void gmClearDemoProcWaitSelectRetry(GMS_CLRDM_MAIN_WORK *main_work)
{
#if _IPHONE
	gmClearDemoSetBgColorBlack();
#endif //_IPHONE

	// 入力処理
	gmClearDemoSetRetryInput(main_work);
	
	// リトライ決定時
	if (main_work->flag & GMD_CLRDM_FLAG_SKIP) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcWaitRetrySonicRunEfct;
		
		// ソニック走りぬけ演出開始
		GmPlySeqChangeTRetryAcc(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]);
		
		return;
	}
	
	
	if (main_work->flag & GMD_CLRDM_FLAG_CANCEL) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcFadeOut;
		
		// フェードアウト開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , GMD_CLRDM_FADEOUT_TIME
					   );
		
		return;
	}
}



// ==========================================================================
// gmClearDemoProcWaitRetrySonicRunEfct
/*!
	ソニック走りぬけ演出終了待ち処理
 */
// ==========================================================================
void gmClearDemoProcWaitRetrySonicRunEfct(GMS_CLRDM_MAIN_WORK *main_work)
{
#if _IPHONE
	gmClearDemoSetBgColorBlack();
#endif //_IPHONE

	// ソニックが画面外へ抜けたとき、フェードアウト開始
	if (ObjObjectViewOutCheck(&g_gm_main_system.ply_work[0]->obj_work)) {
		
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcFadeOut;
		
		main_work->flag |= GMD_CLRDM_FLAG_RETRY_GAME;
		
		// フェードアウト開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , GMD_CLRDM_FADEOUT_TIME
					   );
	}
}



// ==========================================================================
// gmClearDemoSetRetryInput
/*!
	演出終了後のソニックアイコン表示待ち処理
 */
// ==========================================================================
void gmClearDemoSetRetryInput(GMS_CLRDM_MAIN_WORK *main_work)
{
#if !_IPHONE
	// キャンセル処理
	if (AoPadStand() & GSD_KEY_CANCEL) {
		main_work->flag |= GMD_CLRDM_FLAG_CANCEL;
		
		GmSoundPlaySE("Cancel");
		
		return;
	}
	
	// ステージ決定処理
	if (AoPadStand() & GSD_KEY_DECIDE) {
		if (main_work->cur_retry_slct == 0) {
			main_work->flag |= GMD_CLRDM_FLAG_SKIP;
		}
		else {
			main_work->flag |= GMD_CLRDM_FLAG_CANCEL;
		}
		
		GmSoundPlaySE("Ok");
		
		return;
	}

	if (AoPadMStand() & GSD_KEY_UP) {
		if (main_work->cur_retry_slct != 0) {
			GmSoundPlaySE("Cursol");
		}
		
		main_work->cur_retry_slct = 0;
		
		main_work->tex_retry_act->obj_2d.frame = 1.0f;
		main_work->tex_back_slct_act->obj_2d.frame = 0.0f;
		
		return;
	}
		
	else if (AoPadMStand() & GSD_KEY_DOWN) {
		if (main_work->cur_retry_slct != 1) {
			GmSoundPlaySE("Cursol");
		}
		
		main_work->cur_retry_slct = 1;
		
		main_work->tex_retry_act->obj_2d.frame = 0.0f;
		main_work->tex_back_slct_act->obj_2d.frame = 1.0f;
		
		return;
	}
#else //!_IPHONE
	{	//リトライ
		er::CTrgAoAction &trg = main_work->trg_retry;
		trg.Update();
		if (trg.GetState(0)[er::CTrgState::EState::Up] && trg.GetState(0)[er::CTrgState::EState::Prev]) {
			//選択
			main_work->flag |= GMD_CLRDM_FLAG_SKIP;
			for (int i = 0, max = arrayof(main_work->btn_retry); i < max; ++i) {
				main_work->btn_retry[i]->obj_2d.frame = 2.0f;
				main_work->btn_retry[i]->obj_2d.speed = 1.0f;
			}
			GmSoundPlaySE("Ok");
		} else if (trg.GetState(0)[er::CTrgState::EState::On]) {
			//タップ中
			for (int i = 0, max = arrayof(main_work->btn_retry); i < max; ++i) {
				main_work->btn_retry[i]->obj_2d.frame = 1.0f;
			}
		} else if (trg.GetState(0)[er::CTrgState::EState::Out]) {
			//タップキャンセル
			for (int i = 0, max = arrayof(main_work->btn_retry); i < max; ++i) {
				main_work->btn_retry[i]->obj_2d.frame = 0.0f;
			}
		}
	}
	{	//戻る
		er::CTrgAoAction &trg = main_work->trg_back;
		trg.Update();
		if (trg.GetState(0)[er::CTrgState::EState::Up] && trg.GetState(0)[er::CTrgState::EState::Prev]) {
			//選択
			main_work->flag |= GMD_CLRDM_FLAG_CANCEL;
			for (int i = 0, max = arrayof(main_work->btn_back); i < max; ++i) {
				main_work->btn_back[i]->obj_2d.frame = 2.0f;
				main_work->btn_back[i]->obj_2d.speed = 1.0f;
			}
			GmSoundPlaySE("Ok");
		} else if (trg.GetState(0)[er::CTrgState::EState::Stand]) {
			//タップ中
			for (int i = 0, max = arrayof(main_work->btn_back); i < max; ++i) {
				main_work->btn_back[i]->obj_2d.frame = 1.0f;
			}
		} else if (trg.GetState(0)[er::CTrgState::EState::Out]) {
			//タップキャンセル
			for (int i = 0, max = arrayof(main_work->btn_back); i < max; ++i) {
				main_work->btn_back[i]->obj_2d.frame = 0.0f;
			}
		}
	}
#endif //!_IPHONE	
}



// ==========================================================================
// gmClearDemoSetRetryDispInfo
/*!
	リトライ用表示設定処理
 */
// ==========================================================================
void gmClearDemoSetRetryDispInfo(GMS_CLRDM_MAIN_WORK *main_work)
{
	OBS_OBJECT_WORK		*obj_work = NULL;
	OBS_CAMERA			*camera = NULL;
	GMS_PLAYER_WORK		*ply_work = NULL;
	
	// リトライフラグON
	g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag |= GMD_PLF_TATK_RETRY;
	
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	
	// リトライ画面では無音にするため、サウンド停止
	GmSoundStopStageBGM(GMD_CLRDM_STAGE_SOUND_FADEOUT_TIME);
	GmSoundStopJingle(GMD_CLRDM_STAGE_SOUND_FADEOUT_TIME);
	GmSoundStopBGMJingle(GMD_CLRDM_STAGE_SOUND_FADEOUT_TIME);
	GsSoundStopSe();
	
	// マップの非表示
	GmMapSetDisp(FALSE);
	
	// FIXの非表示
	GmFixSetDisp(FALSE);
	
	// 背景初期化設定(オブジェクトを全て非表示)
	GmObjSetAllObjectNoDisp();
	
	// リング非表示対応(リングは他のOBJと別扱いのため)
	GmRingGetWork()->flag |= GMD_RING_SYS_FLAG_NODISP;
	
	// 水しぶきエフェクト無効フラグON
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_WATER_LEVEL_EFCT_OFF;
	g_gm_main_system.water_level = (u16)-1;
	
	// タイムカウントフラグOFF(タイムカウントさせない)
	g_gm_main_system.game_flag &= ~(GMD_GAME_FLAG_COUNT_GAME_TIME
									| GMD_GAME_FLAG_COUNT_SYNC_TIME);
	
	// プレイヤーの表示を復活
	ply_work->obj_work.flag &= ~OBD_OBJECT_NOFUNC;
	ply_work->obj_work.disp_flag &= ~OBD_DISP_NODISP;
	
	// リトライ移行時のカメラ情報取得
	camera = ObjCameraGet(g_obj.glb_camera_id);
	
	// カメラのユーザー処理をデフォルト状態に戻す	※4ボスでのカメラスクロール対応
	ObjCameraSetUserFunc(GME_CAMERA_NO_MAIN, GmCameraFunc);
	
	// カメラのスケール値を通常時に戻す
	GmCameraScaleSet(1.0f, 1.0f);

	// カメラ回転角度をリセット
	camera->roll = 0;
	
	ply_work->gmk_flag &= ~GMD_PLGF_GMK_EXMTX_R;
	
	// ソニックの表示位置をカメラの位置(中心)に設定
	ply_work->obj_work.pos.x
		= FXM_FLOAT_TO_FX32(camera->pos.x);
	ply_work->obj_work.pos.y
		= FXM_FLOAT_TO_FX32(camera->pos.y * -1);
	
	// スクロールロックON(イベント非使用版)
	{	// スクロール制限セット
		GMS_EVE_RECORD_EVENT eve_rec;
		eve_rec.flag	= GMD_GMK_SCR_LMT_EVE_FLAG_ALL;
		eve_rec.left	= -192/2;
		eve_rec.top		= -170/2;
		eve_rec.width	= 384/2;
		eve_rec.height	= 224/2;
		GmCamScrLimitSetDirect(&eve_rec
							, FXM_FLOAT_TO_FX32(camera->pos.x)
							, FXM_FLOAT_TO_FX32(camera->pos.y * -1));
	}
	
	// ソニックのリトライ用FWシーケンス設定
	GmPlySeqChangeTRetryFw(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]);
	
	// プレイヤー以外の全オブジェクトタイプの処理をNOFUNCに設定
	GmObjSetObjectNoFunc(1 << GMD_OBJTYPE_ENEMY);
	GmObjSetObjectNoFunc(1 << GMD_OBJTYPE_GIMMICK);
	GmObjSetObjectNoFunc(1 << GMD_OBJTYPE_DECORATION);
	GmObjSetObjectNoFunc(1 << GMD_OBJTYPE_EFFECT);
	GmObjSetObjectNoFunc(1 << GMD_OBJTYPE_COCKPIT);
	GmObjSetObjectNoFunc(1 << GMD_OBJTYPE_FADE);
	GmObjSetObjectNoFunc(1 << GMD_OBJTYPE_MAPFAR);
	
	// 振動処理OFF設定 
	GMM_PAD_VIB_STOP();  
	
	// リトライ選択項目アクションの表示
	obj_work = (OBS_OBJECT_WORK *)main_work->tex_retry_act;
	obj_work->disp_flag &= ~OBD_DISP_NODISP;
	
	obj_work = (OBS_OBJECT_WORK *)main_work->tex_back_slct_act;
	obj_work->disp_flag &= ~OBD_DISP_NODISP;
	
	obj_work = (OBS_OBJECT_WORK *)main_work->bg_retry;
	obj_work->disp_flag &= ~OBD_DISP_NODISP;

#if _IPHONE
	//クリアタイムを非表示化
	amFlagOff(main_work->flag, GMD_CLRDM_FLAG_DISP_CLEAR_TIME);
	main_work->flag &= ~GMD_CLRDM_FLAG_DISP_CLEAR_TIME;
	for (int i = 0; i < arrayof(main_work->record_time_num_act); i++) {
		obj_work = (OBS_OBJECT_WORK *)main_work->record_time_num_act[i];
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}

	{	//リトライ
		for (int i = 0, max = arrayof(main_work->btn_retry); i < max; ++i) {
			obj_work = (OBS_OBJECT_WORK *)main_work->btn_retry[i];
			obj_work->disp_flag &= ~OBD_DISP_NODISP;
		}
	}
	{	//戻る
		for (int i = 0, max = arrayof(main_work->btn_back); i < max; ++i) {
			obj_work = (OBS_OBJECT_WORK *)main_work->btn_back[i];
			obj_work->disp_flag &= ~OBD_DISP_NODISP;
		}
	}
#endif //_IPHONE
	
	
	// コマンドSTATE変更(2D面の後ろに3Dモデルを置くように変更)
#if _IPHONE
	ObjDrawClearNNCommandStateTbl();
#endif //_IPHONE
	ObjDrawSetNNCommandStateTbl( 0, OBD_DRAW_CMD_STATE_PRE_MAPFAR, FALSE );
	ObjDrawSetNNCommandStateTbl( 1, OBD_DRAW_CMD_STATE_MAPFAR, FALSE );
	ObjDrawSetNNCommandStateTbl( 2, OBD_DRAW_CMD_STATE_POST_MAPFAR, TRUE );
	ObjDrawSetNNCommandStateTbl( 3, OBD_DRAW_CMD_STATE_WATER_BACK, TRUE );
	ObjDrawSetNNCommandStateTbl( 4, OBD_DRAW_CMD_STATE_MAPMID, TRUE );			// 中景
	ObjDrawSetNNCommandStateTbl( 5, OBD_DRAW_CMD_STATE_WATER_MAPMID, TRUE );	// 中景前水
	ObjDrawSetNNCommandStateTbl( 6, OBD_DRAW_CMD_STATE_PRE_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl( 7, OBD_DRAW_CMD_STATE_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl( 8, OBD_DRAW_CMD_STATE_POST_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl( 9, OBD_DRAW_CMD_STATE_NEAR_MAP, TRUE );
	ObjDrawSetNNCommandStateTbl(10, OBD_DRAW_CMD_STATE_3DFIX, TRUE );
	ObjDrawSetNNCommandStateTbl(11, OBD_DRAW_CMD_STATE_2DAMA, TRUE );
	ObjDrawSetNNCommandStateTbl(12, OBD_DRAW_CMD_STATE_3DNN, TRUE );
	
}



// ==========================================================================
// スペシャルステージのスコアアタックモード時の演出シーケンス
// ==========================================================================

// ==========================================================================
// gmClearDemoProcSpeScoreMoveEfct
/*!
	テキスト移動演出中処理
 */
// ==========================================================================
void gmClearDemoProcSpeScoreMoveEfct(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 最後の移動アクションが終端になったら
	if (AoActIsEndTrs(main_work->tex_total_act->obj_2d.act)) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcSpeScorePrevCalcScore;

		// ライン表示設定フラグON
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_LINE);
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_TOTAL_SCORE);
	}
	
	// 演出終了チェック
	if (AoActIsEndTrs(main_work->tex_time_act->obj_2d.act)) {
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_TIME_SCORE);
	}
	
	if (AoActIsEndTrs(main_work->tex_ring_act->obj_2d.act)) {
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_RING_SCORE);
	}

	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);

	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);
}



// ==========================================================================
// gmClearDemoProcSpeScorePrevCalcScore
/*!
	スコア加減算演出前の待ち処理
 */
// ==========================================================================
void gmClearDemoProcSpeScorePrevCalcScore(GMS_CLRDM_MAIN_WORK *main_work)
{
	// タイマー更新
	main_work->timer += main_work->count;
	
	// スコア計算までの規定時間がたったら
	if (main_work->timer > GMD_CLRDM_PREV_SCORE_IDLE_TIME) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcSpeScoreCalcScore;

		// スコア計算処理関数設定
		main_work->proc_calc_score = gmClearDemoSetCalcScore;

		// タイマー初期化
		main_work->timer = 0;
		
//		return;
	}

	// SKIPフラグ処理
#if !_IPHONE
	if ((AoPadStand() & GSD_KEY_DECIDE)
		|| (AoPadStand() & GSD_KEY_CANCEL)
		|| (AoPadStand() & KEY_START)) {
#else
	if (amTpIsTouchPush(0)) {
#endif
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_SKIP);
	}

	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);

	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);

}



// ==========================================================================
// gmClearDemoProcSpeScoreCalcScore
/*!
	スコア加減算演出中処理
 */
// ==========================================================================
void gmClearDemoProcSpeScoreCalcScore(GMS_CLRDM_MAIN_WORK *main_work)
{
	
	// タイマー更新
	main_work->timer += main_work->count;
	
	// スコア算出演出が終わったら、またはスキップフラグONならば
	if (main_work->flag & GMD_CLRDM_FLAG_SKIP
		|| main_work->flag & GMD_CLRDM_FLAG_SCORE_CALC_END) {
		// 表示待ちプロシージャへ
		main_work->proc_update = gmClearDemoProcSpeScoreWaitDispSonic;
		
		// 表示用スコアに算出結果のスコアを代入
		main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			= main_work->time_score[GME_CLRDM_SCORE_TYPE_CALC_DATA];
		
		main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			= main_work->ring_score[GME_CLRDM_SCORE_TYPE_CALC_DATA];
		
		main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			= main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA];
		
		// 
		main_work->proc_calc_score = NULL;
		
		amFlagOff(main_work->flag, GMD_CLRDM_FLAG_SKIP);
		amFlagOff(main_work->flag, GMD_CLRDM_FLAG_SCORE_CALC_END);

		// 1UPの場合
		if (main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] >= GMD_CLRDM_1UP_SCORE) {
			amFlagOn(main_work->flag, GMD_CLRDM_FLAG_GET_1UP);
			main_work->idle_time = GMD_CLRDM_1UP_IDLE_TIME;
		}
		else {
			main_work->idle_time = GMD_CLRDM_NORMAL_IDLE_TIME;
		}
		
		// 実績設定
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_CLEAR_DEMO);
		
		main_work->timer = 0;
//		return;
	}

	// SKIPフラグ処理
#if !_IPHONE
	if ((AoPadStand() & GSD_KEY_DECIDE)
		|| (AoPadStand() & GSD_KEY_CANCEL)
		|| (AoPadStand() & KEY_START)) {
#else
	if (amTpIsTouchPush(0)) {
#endif
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_SKIP);
	}

	// スコア加減処理
	if (main_work->proc_calc_score
		&& main_work->timer > 1) {
		main_work->proc_calc_score(main_work);

		main_work->timer = 0;
	}

	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);

	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);

	// スコアカウントサウンド処理
	if (!main_work->proc_calc_score) {
		GmSoundPlaySE("Result2");
	}
	else if (!main_work->timer) {
		GmSoundPlaySE("Result1");
	}
	
}


// ==========================================================================
// gmClearDemoProcSpeScoreWaitDispSonic
/*!
	演出終了後のソニックアイコン表示待ち処理
 */
// ==========================================================================
void gmClearDemoProcSpeScoreWaitDispSonic(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 表示待ち時間が過ぎたらフェードアウトへ
	if (main_work->timer >= GMD_CLRDM_WAIT_SONIC_DISP_TIME) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcSpeScoreDispIdle;
		main_work->timer = 0;

		if (main_work->flag & GMD_CLRDM_FLAG_GET_1UP) {
			GmSoundPlayJingle(GME_SOUND_JINGLE_IDX_1UP);
			
	        GmPlayerStockGet(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P], 1);
		}
		
		return;
	}
	
	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);

	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);

	// タイマー更新
	main_work->timer += main_work->count;

}

static int inv_min, inv_sec, inv_msec; //sss - inversed time result

static void mpp_inverseTimeForSpecialStage(int min, int sec, int msec)//substr from 1:30
{
	const int MAX_TIME = 90*100; //1:30
	int inv_time_ms = MAX_TIME - ((min*60+sec)*100+msec);
	if(inv_time_ms<0) inv_time_ms=0;
	if(inv_time_ms>MAX_TIME) inv_time_ms=MAX_TIME;
	
	inv_msec = (inv_time_ms)%100;
	inv_sec = (inv_time_ms/100)%60;
	inv_min = (inv_time_ms/(100*60));
	
}

// ==========================================================================
// gmClearDemoProcSpeScoreDispIdle
/*!
	演出終了後のアイドル中処理(1up時の表示・BGM再生処理も行う)
 */
// ==========================================================================
void gmClearDemoProcSpeScoreDispIdle(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 表示待ち時間が過ぎたらフェードアウトへ
	if (main_work->timer >= main_work->idle_time) {
		
		mpp_checkAchievementsForSuccessLevelEnd(); //sss
		mpp_flushAchievementsToGlobalNet(true); //end of special stages (General)
		//was: mppUtil::sendScore(g_gs_main_sys_info.game_mode,g_gs_main_sys_info.stage_id,main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA],main_work->time_min,main_work->time_sec,main_work->time_msec);		
		//＼＼mpp_inverseTimeForSpecialStage(main_work->time_min,main_work->time_sec,main_work->time_msec);
		//＼＼mppUtil::sendScore(g_gs_main_sys_info.game_mode,g_gs_main_sys_info.stage_id,main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA], inv_min, inv_sec, inv_msec);		
		
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcSpeScoreFadeOut;
//		main_work->timer = 0;
		
		// フェードアウト開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_WHITE_FADEOUT
					   , GMD_CLRDM_FADEOUT_TIME
					   );
		
//		return;
	}
	
	// ソニックアイコン点滅処理
	if (main_work->flag & GMD_CLRDM_FLAG_GET_1UP) {
		gmClearDemoSetFlashSonic(main_work);
	}
	
	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);
	
	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);
	
	// アクション更新
	gmClearDemoUpdateAct(main_work);
	
	// タイマー更新
	main_work->timer += main_work->count;
}



// ==========================================================================
// gmClearDemoProcSpeScoreFadeOut
/*!
	フェードアウト中処理
 */
// ==========================================================================
void gmClearDemoProcSpeScoreFadeOut(GMS_CLRDM_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		
		if(mppUtil::isAchievementAlertShown()) //sss
			return;
		// フェード終了
//		IzFadeExit();

		// 遷移先なし
		main_work->proc_update = gmClearDemoProcSpeScoreFinish;

		main_work->timer = 0;
		return;
	}
	
	// ソニックアイコン点滅処理
	if (main_work->flag & GMD_CLRDM_FLAG_GET_1UP) {
		gmClearDemoSetFlashSonic(main_work);
	}

	// ソートバッファ登録
	gmClearDemoSetSortBufAct(main_work);

	// スコアデータ更新
	gmClearDemoSetScoreData(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);
	
	// タイマー更新
	main_work->timer += main_work->count;
}



// ==========================================================================
// gmClearDemoProcSpeScoreFinish
/*!
	終了設定処理
 */
// ==========================================================================
void gmClearDemoProcSpeScoreFinish(GMS_CLRDM_MAIN_WORK *main_work)
{
	main_work->proc_update = NULL;
	
	// タスク終了フラグON
	amFlagOn(main_work->flag, GMD_CLRDM_FLAG_EXIT);
}




// ==========================================================================
// スペステステージのタイムアタックモード時の演出シーケンス
// ==========================================================================
// ==========================================================================
// gmClearDemoProcSpeTimeMoveEfct
/*!
	テキスト移動演出中処理
 */
// ==========================================================================
void gmClearDemoProcSpeTimeMoveEfct(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 最後の大TIMEアクションが終端になったら
	if (AoActIsEndTrs(main_work->tex_big_time_act->obj_2d.act)) {
		// ここでNEW RECORDかどうかを判定して分岐させる
		main_work->proc_update = gmClearDemoProcSpeTimeWaitTimeEfct;
		
		for (int i = 0; i < 7; i++) {
			ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->record_time_num_act[i]
											   , &main_work->record_time_num_act[i]->obj_2d
											   , NULL
											   , NULL
											   , IDB_CPIT_MAIN_G_RSLT_AMA
											   , GmGameDatGetCockpitData()
											   , AoTexGetTexList(&main_work->tex[0])
											   , (u32)(IDA_G_RSLT_ACT_TA_TIME1 + i)
											   , FALSE
											   );
#if _IPHONE
			((OBS_OBJECT_WORK *)main_work->record_time_num_act[i])->scale.x = DMD_CLROM_TEXT_SCALE_X;
			((OBS_OBJECT_WORK *)main_work->record_time_num_act[i])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
		}
		
		gmClearDemoSetClearTimeRecord(main_work);
		gmClearDemoSetSortBufTimeAct(main_work);
		
		// タイム表示設定フラグON
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_CLEAR_TIME);
		
		// ソニックアイコン表示フラグON
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_TIME_SONIC);
	}

	// ソートバッファ登録
	gmClearDemoSetTimeAtkSortBufAct(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);
}



// ==========================================================================
// gmClearDemoProcSpeTimeWaitTimeEfct
/*!
	NEW RECORDテキスト待ち演出中処理
 */
// ==========================================================================
void gmClearDemoProcSpeTimeWaitTimeEfct(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 最後の大TIMEアクションが終端になったら
	if (main_work->timer > 60.f) {
		// ここでNEW RECORDかどうかを判定して分岐させる
		if (main_work->flag & GMD_CLRDM_FLAG_IS_NEW_RECORD) {
			main_work->proc_update = gmClearDemoProcSpeTimeTimeMoveNewRecord;
			
			// NEWRECORDテキストのスピードを戻す
			main_work->tex_new_record_act->obj_2d.speed = 1.0f;
			
			main_work->idle_time = 180;
		}
		else {
			main_work->proc_update = gmClearDemoProcSpeTimeDispEffect;
			
			main_work->idle_time = 120;
		}
		
		// 実績設定
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_CLEAR_DEMO);
		
		main_work->timer = 0.f;
	}

	// ソートバッファ登録
	gmClearDemoSetTimeAtkSortBufAct(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);
	
	// 
	if (main_work->timer > 120.f) {
		return;
	}
	else {
		// タイマー更新
		main_work->timer += main_work->count;
	}
}



// ==========================================================================
// gmClearDemoProcSpeTimeTimeMoveNewRecord
/*!
	NEW RECORDテキスト移動演出中処理
 */
// ==========================================================================
void gmClearDemoProcSpeTimeTimeMoveNewRecord(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 最後の大TIMEアクションが終端になったら
	if (AoActIsEndTrs(main_work->tex_new_record_act->obj_2d.act)) {
		// NEWRECORD取得なのでタイム部分を光らせる演出へ遷移
		main_work->proc_update = gmClearDemoProcSpeTimeDispEffect;
		
		// NEWレコード更新ジングル再生
		GmSoundPlayJingle(GME_SOUND_JINGLE_IDX_NEW_RECORD);
	}

	// ソートバッファ登録
	gmClearDemoSetTimeAtkSortBufAct(main_work);

	// アクション更新
	gmClearDemoUpdateAct(main_work);
}



// ==========================================================================
// gmClearDemoProcSpeTimeDispEffect
/*!
	レコードタイム表示演出処理
 */
// ==========================================================================
void gmClearDemoProcSpeTimeDispEffect(GMS_CLRDM_MAIN_WORK *main_work)
{
	// タイマー更新
	main_work->timer += main_work->count;
	
	if (AoPadStand() & GSD_KEY_DECIDE) {
		main_work->flag |= GMD_CLRDM_FLAG_SKIP;
	}
	
	// スコア計算までの規定時間がたったら
	if (main_work->timer > main_work->idle_time
		|| main_work->flag & GMD_CLRDM_FLAG_SKIP) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcSpeTimeChangeRetryOut;
		if(main_work->time_min>0 || main_work->time_sec>0) {
			mpp_checkAchievementsForSuccessLevelEnd();
			mpp_flushAchievementsToGlobalNet(true);//end of special stage Time Attack
			//was: mppUtil::sendScore(g_gs_main_sys_info.game_mode,g_gs_main_sys_info.stage_id,main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA],main_work->time_min,main_work->time_sec,main_work->time_msec);
			mpp_inverseTimeForSpecialStage(main_work->time_min,main_work->time_sec,main_work->time_msec);	
			mppUtil::sendScore(g_gs_main_sys_info.game_mode,g_gs_main_sys_info.stage_id,main_work->total_score[GME_CLRDM_SCORE_TYPE_CALC_DATA], inv_min, inv_sec, inv_msec);
		}

		// フェードアウト
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , 32.f
					   );

		// タイマー初期化
		main_work->timer = 0;
		
		main_work->flag &= ~GMD_CLRDM_FLAG_SKIP;
	}
	
	// タイム部の閃光演出
	gmClearDemoTimeFlushEffect(main_work);

	// ソートバッファ登録
	gmClearDemoSetTimeAtkSortBufAct(main_work);
}



// ==========================================================================
// gmClearDemoProcSpeTimeChangeRetryOut
/*!
	リトライ画面切り替えフェードアウト中処理
 */
// ==========================================================================
void gmClearDemoProcSpeTimeChangeRetryOut(GMS_CLRDM_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcSpeTimeChangeRetryIn;
		
		// フェードアウト
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEIN
					   , 32.f
					   );
		
		// リトライ用表示設定
		gmClearDemoSetRetryDispInfo(main_work);
		
		gmClearDemoSetRetrySortBufAct(main_work);
		
		// タイマー初期化
		main_work->timer = 0;
		
		return;
	}
	
	// ソートバッファ登録
	gmClearDemoSetTimeAtkSortBufAct(main_work);
}



// ==========================================================================
// gmClearDemoProcSpeTimeChangeRetryIn
/*!
	リトライ画面切り替えフェードイン中処理
 */
// ==========================================================================
void gmClearDemoProcSpeTimeChangeRetryIn(GMS_CLRDM_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		
		main_work->proc_update = gmClearDemoProcSpeTimeWaitSelectRetry;
	}
}



// ==========================================================================
// gmClearDemoProcSpeTimeWaitSelectRetry
/*!
	演出終了後のソニックアイコン表示待ち処理
 */
// ==========================================================================
void gmClearDemoProcSpeTimeWaitSelectRetry(GMS_CLRDM_MAIN_WORK *main_work)
{
#if _IPHONE
	gmClearDemoSetBgColorBlack();
#endif //_IPHONE

	// 入力処理
	gmClearDemoSetRetryInput(main_work);
	
	// リトライ決定時
	if (main_work->flag & GMD_CLRDM_FLAG_SKIP) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcWaitRetrySonicRunEfct;
		
		// ソニック走りぬけ演出開始
		GmPlySeqChangeTRetryAcc(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]);
		
		return;
	}
	
	if (main_work->flag & GMD_CLRDM_FLAG_CANCEL) {
		// 終了処理へ
		main_work->proc_update = gmClearDemoProcFadeOut;
		
		// フェードアウト開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEOUT
					   , GMD_CLRDM_FADEOUT_TIME
					   );
		
		return;
	}
}



// ==========================================================================
// gmClearDemoCreateObjActScore
/*!
	クリアデモで使用するオブジェクトアクションの構築処理
 */
// ==========================================================================
void gmClearDemoCreateObjActScore(GMS_CLRDM_MAIN_WORK *main_work)
{
	int i = 0;
	OBS_OBJECT_WORK		*obj_work;

	// 移動クリアテキスト(ステージ別分を除く)
	for (i = 0; i < 5; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_UPTEXT"
										   );

		main_work->tex_up_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
		
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_TIMENUM"
										   );

		main_work->time_num_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
		
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_RINGNUM"
										   );

		main_work->ring_num_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
		
	}
	
	for (i = 0; i < 9; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_TOTALNUM"
										   );

		main_work->total_num_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	for (i = 0; i < 3; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_LINE"
										   );

		main_work->line_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	// タイムスコアテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_TIMETEXT"
									   );

	main_work->tex_time_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// リングスコアテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_RINGTEXT"
									   );

	main_work->tex_ring_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// トータルスコアテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_TOTALTEXT"
									   );

	main_work->tex_total_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// ソニックアイコン
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_SONICICON"
									   );

	main_work->sonic_icon_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_SONICICON2"
									   );

	main_work->sonic_icon_act2 = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
}



// ==========================================================================
// gmClearDemoCreateObjActTime
/*!
	クリアデモで使用するオブジェクトアクションの構築処理
 */
// ==========================================================================
void gmClearDemoCreateObjActTime(GMS_CLRDM_MAIN_WORK *main_work)
{
	int i = 0;
	OBS_OBJECT_WORK		*obj_work;
	
	// ここからタイムアタック関連
	// 移動クリアテキスト(ステージ別分を除く)
	for (i = 0; i < 5; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_UPTEXT"
										   );

		main_work->tex_up_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	// タイムアタックのTIMEテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_TIMEATK_TEXT"
									   );

	main_work->tex_big_time_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// タイムアタックの時間数字
	for (i = 0; i < 7; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_TIME_DIGIT_NUM"
										   );

		main_work->record_time_num_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	// タイムアタックのソニックアイコン
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_TIMEATK_SONIC"
									   );

	main_work->time_sonic_icon_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// タイムアタックのNEW_RECORDテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_NEWRECORD_TEXT"
									   );

	main_work->tex_new_record_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// リトライ背景
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_BG_RETRY"
									   );

	main_work->bg_retry = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// リトライテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_RETRY_TEXT"
									   );

	main_work->tex_retry_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// ACT選択に戻るテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_ACT_BACK_TEXT"
									   );

	main_work->tex_back_slct_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;

#if _IPHONE
	//リトライ台紙
	for (int i = 0, max = arrayof(main_work->btn_retry); i < max; ++i) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_RETRY_BTN"
										   );

		main_work->btn_retry[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	//ACT選択に戻る台紙
	for (int i = 0, max = arrayof(main_work->btn_back); i < max; ++i) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_ACT_BACK_BTN"
										   );

		main_work->btn_back[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
#endif //_IPHONE
}



// ==========================================================================
// gmClearDemoCreateObjSpeScore
/*!
	クリアデモで使用するオブジェクトアクションの構築処理
 */
// ==========================================================================
void gmClearDemoCreateObjSpeScore(GMS_CLRDM_MAIN_WORK *main_work)
{
	int i = 0;
	OBS_OBJECT_WORK		*obj_work;
	
	// ここからはスペステ用
	
	// スペステ用上段テキストアクション
	for (i = 0; i < 3; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_UP_SPST_TEXT"
										   );

		main_work->tex_spst_up_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	// タイムアタックの時間数字
	for (i = 0; i < 7; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_SPE_STAGE_NO"
										   );

		main_work->spst_num_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
		
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_EMER_UP_ICON"
										   );

		main_work->icon_emer_up_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
		
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_EMER_DOWN_ICON"
										   );

		main_work->icon_emer_down_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	// カオスエメラルド光演出用
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_LIGHT_EMER"
									   );

	main_work->icon_emer_light_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// EXTENDテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_EXTEND_TEXT"
									   );

	main_work->tex_extend_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// 移動クリアテキスト(ステージ別分を除く)
	for (i = 0; i < 5; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_TIMENUM"
										   );

		main_work->time_num_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
		
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_RINGNUM"
										   );

		main_work->ring_num_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
		
	}
	
	for (i = 0; i < 9; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_TOTALNUM"
										   );

		main_work->total_num_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	for (i = 0; i < 3; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_LINE"
										   );

		main_work->line_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	// タイムスコアテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_TIMETEXT"
									   );

	main_work->tex_time_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// リングスコアテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_RINGTEXT"
									   );

	main_work->tex_ring_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// トータルスコアテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_TOTALTEXT"
									   );

	main_work->tex_total_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// ソニックアイコン
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_SONICICON"
									   );

	main_work->sonic_icon_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_SONICICON2"
									   );

	main_work->sonic_icon_act2 = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
}



// ==========================================================================
// gmClearDemoCreateObjSpeTime
/*!
	クリアデモで使用するオブジェクトアクションの構築処理
 */
// ==========================================================================
void gmClearDemoCreateObjSpeTime(GMS_CLRDM_MAIN_WORK *main_work)
{
	int i = 0;
	OBS_OBJECT_WORK		*obj_work;
	
	// ここからタイムアタック関連
	// スペステ用上段テキストアクション
	for (i = 0; i < 3; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_UP_SPST_TEXT"
										   );

		main_work->tex_spst_up_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	// タイムアタックの時間数字
	for (i = 0; i < 7; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_SPE_STAGE_NO"
										   );

		main_work->spst_num_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
		
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_EMER_UP_ICON"
										   );

		main_work->icon_emer_up_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
		
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_EMER_DOWN_ICON"
										   );

		main_work->icon_emer_down_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	// カオスエメラルド光演出用
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_LIGHT_EMER"
									   );

	main_work->icon_emer_light_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	
	// タイムアタックのTIMEテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_TIMEATK_TEXT"
									   );

	main_work->tex_big_time_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// タイムアタックの時間数字
	for (i = 0; i < 7; i++) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_TIME_DIGIT_NUM"
										   );

		main_work->record_time_num_act[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	
	// タイムアタックのソニックアイコン
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_TIMEATK_SONIC"
									   );

	main_work->time_sonic_icon_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// タイムアタックのNEW_RECORDテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_NEWRECORD_TEXT"
									   );

	main_work->tex_new_record_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// リトライ背景
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_BG_RETRY"
									   );

	main_work->bg_retry = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// リトライテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_RETRY_TEXT"
									   );

	main_work->tex_retry_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;
	
	// ACT選択に戻るテキスト
	obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
									   , NULL
									   , 0
									   , "CLRDM_ACT_BACK_TEXT"
									   );

	main_work->tex_back_slct_act = (GMS_COCKPIT_2D_WORK *)obj_work;
	obj_work = NULL;

#if _IPHONE
	//リトライ台紙
	for (int i = 0, max = arrayof(main_work->btn_retry); i < max; ++i) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_RETRY_BTN"
										   );

		main_work->btn_retry[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
	//ACT選択に戻る台紙
	for (int i = 0, max = arrayof(main_work->btn_back); i < max; ++i) {
		obj_work = GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK)
										   , NULL
										   , 0
										   , "CLRDM_ACT_BACK_BTN"
										   );

		main_work->btn_back[i] = (GMS_COCKPIT_2D_WORK *)obj_work;
		obj_work = NULL;
	}
#endif //_IPHONE
}



// ==========================================================================
// gmClearDemoCreateObjAct
/*!
	クリアデモで使用するオブジェクトアクションの構築処理
 */
// ==========================================================================
void gmClearDemoCreateObjAct(GMS_CLRDM_MAIN_WORK *main_work)
{
	int i = 0;

	// 移動クリアテキスト(ステージ別分を除く)
	// ステージ別アクション設定
	gmClearDemoCreateObjActForStage(main_work);
	
	// タイムスコア数字、リングスコア数字
	for (i = 0; i < 5; i++) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->time_num_act[i]
										   , &main_work->time_num_act[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , (u32)(IDA_G_RSLT_ACT_SCORE_TIME_1 + i)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->time_num_act[i])->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->time_num_act[i])->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->ring_num_act[i]
										   , &main_work->ring_num_act[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , (u32)(IDA_G_RSLT_ACT_SCORE_RING_1 + i)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->ring_num_act[i])->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->ring_num_act[i])->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	for (i = 0; i < 9; i++) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->total_num_act[i]
										   , &main_work->total_num_act[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , (u32)(IDA_G_RSLT_ACT_SCORE_TOTAL_1 + i)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->total_num_act[i])->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->total_num_act[i])->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	for (i = 0; i < 3; i++) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->line_act[i]
										   , &main_work->line_act[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , (u32)(IDA_G_RSLT_ACT_LINE_1 + i)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->line_act[i])->scale.x = DMD_CLROM_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->line_act[i])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	// タイムスコアテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_time_act
									   , &main_work->tex_time_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_TEX_TIME
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_time_act)->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_time_act)->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	
	// リングスコアテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_ring_act
									   , &main_work->tex_ring_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_TEX_RING
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_ring_act)->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_ring_act)->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	
	// トータルスコアテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_total_act
									   , &main_work->tex_total_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_TEX_TOTAL
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_total_act)->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_total_act)->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	
	// ソニックアイコン
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->sonic_icon_act
									   , &main_work->sonic_icon_act->obj_2d
									   , NULL
									   , NULL
									   , IDB_CPIT_MAIN_G_RSLT_AMA
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[0])
									   , IDA_G_RSLT_ACT_ICON_SONIC
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->sonic_icon_act)->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->sonic_icon_act)->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
	
	
}



// ==========================================================================
// gmClearDemoCreateObjNormalTimeAtk
/*!
	クリアデモで使用するオブジェクトアクションの構築処理
 */
// ==========================================================================
void gmClearDemoCreateObjNormalTimeAtk(GMS_CLRDM_MAIN_WORK *main_work)
{
	// タイムアタック用
	// ステージ別アクション設定
	gmClearDemoCreateObjActForStage(main_work);
	
	
	// タイムアタックテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_big_time_act
									   , &main_work->tex_big_time_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_TA_TEX_TIME
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_big_time_act)->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_big_time_act)->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
#if 0	
	for (int i = 0; i < 7; i++) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->record_time_num_act[i]
										   , &main_work->record_time_num_act[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , (u32)(IDA_G_RSLT_ACT_TA_TIME1 + i)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->record_time_num_act[i])->scale.x = DMD_CLROM_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->record_time_num_act[i])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	}
#endif
	
	// タイムアタック用ソニックアイコン
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->time_sonic_icon_act
									   , &main_work->time_sonic_icon_act->obj_2d
									   , NULL
									   , NULL
									   , IDB_CPIT_MAIN_G_RSLT_AMA
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[0])
									   , IDA_G_RSLT_ACT_TA_ICON_SONIC
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->time_sonic_icon_act)->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->time_sonic_icon_act)->scale.y = DMD_CLROM_TEXT_SCALE_Y;
	// ここでソニックの状態別で表示を変更する
	main_work->time_sonic_icon_act->obj_2d.frame = ((GMM_MAIN_USE_SUPER_SONIC()? 1.0f: 0.0f));
	main_work->time_sonic_icon_act->obj_2d.speed = 0.0f;
#endif //_IPHONE
	
	// NEW RECORDS テキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_new_record_act
									   , &main_work->tex_new_record_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_TA_TEX_NEW
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_new_record_act)->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_new_record_act)->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
}



// ==========================================================================
// gmClearDemoCreateObjSpecialScoreAtk
/*!
	クリアデモで使用するオブジェクトアクションの構築処理
 */
// ==========================================================================
void gmClearDemoCreateObjSpecialScoreAtk(GMS_CLRDM_MAIN_WORK *main_work)
{
	int i = 0;
	OBS_OBJECT_WORK *obj_work = NULL;
	
	
	// ここからはスペステ用
	
	// スペステ用テキストアクション
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[0]
									   , &main_work->tex_spst_up_act[0]->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , (u32)(IDA_G_RSLT_JP_ACT_TEX_SPST)
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[0])->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[0])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
	if (main_work->is_get_eme
		&& main_work->is_first_spe_clear) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[1]
										   , &main_work->tex_spst_up_act[1]->obj_2d
										   , NULL
										   , NULL
										   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[1])
										   , (u32)(IDA_G_RSLT_JP_ACT_TEX_SONIC2)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[1])->scale.x = DMD_CLROM_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[1])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[2]
									   , &main_work->tex_spst_up_act[2]->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , (u32)(IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR)
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[2])->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[2])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
	
	gmClearDemoCreateObjSpeActForStage(main_work);
	
	// ステージ番号
	for (i = 0; i < main_work->has_eme_num; i++) {
		if (main_work->is_get_eme
			&& main_work->is_first_spe_clear) {
			ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->icon_emer_down_act[i]
											   , &main_work->icon_emer_down_act[i]->obj_2d
											   , NULL
											   , NULL
											   , IDB_CPIT_MAIN_G_RSLT_AMA
											   , GmGameDatGetCockpitData()
											   , AoTexGetTexList(&main_work->tex[0])
											   , (u32)(IDA_G_RSLT_ACT_ICON_EME1_1 + i)
											   , FALSE
											   );
#if _IPHONE
			((OBS_OBJECT_WORK *)main_work->icon_emer_down_act[i])->scale.x = DMD_CLROM_TEXT_SCALE_X;
			((OBS_OBJECT_WORK *)main_work->icon_emer_down_act[i])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
		}
		else {
			ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->icon_emer_up_act[i]
											   , &main_work->icon_emer_up_act[i]->obj_2d
											   , NULL
											   , NULL
											   , IDB_CPIT_MAIN_G_RSLT_AMA
											   , GmGameDatGetCockpitData()
											   , AoTexGetTexList(&main_work->tex[0])
											   , (u32)(IDA_G_RSLT_ACT_ICON_EME2_1 + i)
											   , FALSE
											   );
#if _IPHONE
			((OBS_OBJECT_WORK *)main_work->icon_emer_up_act[i])->scale.x = DMD_CLROM_TEXT_SCALE_X;
			((OBS_OBJECT_WORK *)main_work->icon_emer_up_act[i])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
		}
	}
	
	
	// タイムスコア数字、リングスコア数字
	for (i = 0; i < 5; i++) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->time_num_act[i]
										   , &main_work->time_num_act[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , (u32)(IDA_G_RSLT_ACT_SP_SCORE_TIME_1 + i)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->time_num_act[i])->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->time_num_act[i])->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->ring_num_act[i]
										   , &main_work->ring_num_act[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , (u32)(IDA_G_RSLT_ACT_SP_SCORE_RING_1 + i)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->ring_num_act[i])->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->ring_num_act[i])->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	for (i = 0; i < 9; i++) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->total_num_act[i]
										   , &main_work->total_num_act[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , (u32)(IDA_G_RSLT_ACT_SP_SCORE_TOTAL_1 + i)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->total_num_act[i])->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->total_num_act[i])->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	for (i = 0; i < 3; i++) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->line_act[i]
										   , &main_work->line_act[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , (u32)(IDA_G_RSLT_ACT_SP_LINE_1 + i)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->line_act[i])->scale.x = DMD_CLROM_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->line_act[i])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	// タイムスコアテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_time_act
									   , &main_work->tex_time_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_SP_TEX_TIME
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_time_act)->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_time_act)->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	
	// リングスコアテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_ring_act
									   , &main_work->tex_ring_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_SP_TEX_RING
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_ring_act)->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_ring_act)->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	
	// トータルスコアテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_total_act
									   , &main_work->tex_total_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_SP_TEX_TOTAL
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_total_act)->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_total_act)->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	
	// ソニックアイコン
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->sonic_icon_act
									   , &main_work->sonic_icon_act->obj_2d
									   , NULL
									   , NULL
									   , IDB_CPIT_MAIN_G_RSLT_AMA
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[0])
									   , IDA_G_RSLT_ACT_SP_ICON_SONIC1
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->sonic_icon_act)->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->sonic_icon_act)->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
	// ソニックアイコン2
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->sonic_icon_act2
									   , &main_work->sonic_icon_act2->obj_2d
									   , NULL
									   , NULL
									   , IDB_CPIT_MAIN_G_RSLT_AMA
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[0])
									   , IDA_G_RSLT_ACT_SP_ICON_SONIC2
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->sonic_icon_act2)->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->sonic_icon_act2)->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
	
	// カオスエメラルド光演出用
	if (0) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->icon_emer_light_act
										   , &main_work->icon_emer_light_act->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , IDA_G_RSLT_ACT_ICON_EME02
										   , FALSE
										   );
		
		// 表示ずれ用設定
		obj_work = (OBS_OBJECT_WORK *)main_work->icon_emer_light_act;
		obj_work->pos.x = (DMD_CLRDM_EMER_LIGHT_DIST * (main_work->stage_id - GSD_MAIN_STAGE_ID_SS1)) * FX32_ONE;
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->icon_emer_light_act)->scale.x = DMD_CLROM_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->icon_emer_light_act)->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	// EXTENDテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_extend_act
									   , &main_work->tex_extend_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_TEX_EXTE
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_extend_act)->scale.x = DMD_CLROM_SMALL_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_extend_act)->scale.y = DMD_CLROM_SMALL_TEXT_SCALE_Y;
#endif //_IPHONE
	
	
	
}



// ==========================================================================
// gmClearDemoCreateObjSpecialTimeAtk
/*!
	クリアデモで使用するオブジェクトアクションの構築処理
 */
// ==========================================================================
void gmClearDemoCreateObjSpecialTimeAtk(GMS_CLRDM_MAIN_WORK *main_work)
{
	int i = 0;
	
	// ここからはスペステ用
	
	// スペステ用テキストアクション
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[0]
									   , &main_work->tex_spst_up_act[0]->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , (u32)(IDA_G_RSLT_JP_ACT_TEX_SPST)
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[0])->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[0])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[2]
									   , &main_work->tex_spst_up_act[2]->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , (u32)(IDA_G_RSLT_JP_ACT_TEX_SP_CLEAR)
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[2])->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_spst_up_act[2])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
	
	gmClearDemoCreateObjSpeActForStage(main_work);
	
	// ステージ番号
	for (i = 0; i < main_work->has_eme_num; i++) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->icon_emer_up_act[i]
										   , &main_work->icon_emer_up_act[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , (u32)(IDA_G_RSLT_ACT_ICON_EME2_1 + i)
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->icon_emer_up_act[i])->scale.x = DMD_CLROM_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->icon_emer_up_act[i])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	// タイムアタックテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_big_time_act
									   , &main_work->tex_big_time_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_TA_TEX_TIME
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_big_time_act)->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_big_time_act)->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
	// タイムアタック用ソニックアイコン
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->time_sonic_icon_act
									   , &main_work->time_sonic_icon_act->obj_2d
									   , NULL
									   , NULL
									   , IDB_CPIT_MAIN_G_RSLT_AMA
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[0])
									   , IDA_G_RSLT_ACT_TA_ICON_SONIC
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->time_sonic_icon_act)->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->time_sonic_icon_act)->scale.y = DMD_CLROM_TEXT_SCALE_Y;
	// ここでソニックの状態別で表示を変更する
	main_work->time_sonic_icon_act->obj_2d.frame = ((GMM_MAIN_USE_SUPER_SONIC()? 1.0f: 0.0f));
	main_work->time_sonic_icon_act->obj_2d.speed = 0.0f;
#endif //_IPHONE
	
	// NEW RECORDS テキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_new_record_act
									   , &main_work->tex_new_record_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_TA_TEX_NEW
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_new_record_act)->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_new_record_act)->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
}



// ==========================================================================
// gmClearDemoCreateObjActForStage
/*!
	ステージ別の2Dアクション生成処理
 */
// ==========================================================================
void gmClearDemoCreateObjActForStage(GMS_CLRDM_MAIN_WORK *main_work)
{
	GSE_LANGUAGE lang_type = GSD_LANGUAGE_JP;
	u16 tex_clr_id = 0;
	s32 tmp_ama_id = 0;
	u16 lang_id = 0;
	u16 num_act_id = 0;
	
	lang_type = GsEnvGetLanguage();
	
	// イタリア語のとき
	if (lang_type == GSD_LANGUAGE_IT) {
		num_act_id = IDA_G_RSLT_ACT_IT_SONIC;
	}
	// スペイン語のとき
	else if (lang_type == GSD_LANGUAGE_SP) {
		num_act_id = IDA_G_RSLT_ACT_SP_SONIC;
	}
	// ドイツ語以外のとき
	else {
		num_act_id = IDA_G_RSLT_ACT_TEX_SONIC;
	}
	
	// ソニックテキスト
	if (lang_type != GSD_LANGUAGE_GE) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_up_act[0]
										   , &main_work->tex_up_act[0]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , num_act_id
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->tex_up_act[0])->scale.x = DMD_CLROM_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->tex_up_act[0])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	// ステージ別のクリアテキストID取得
	if (lang_type != GSD_LANGUAGE_GE) {
		tex_clr_id = dm_clrdm_stage_tex_act_id[main_work->stage_id];
	}
	else {
		tex_clr_id = dm_clrdm_stage_ge_tex_act_id[main_work->stage_id];
	}
	
	// ステージ別のクリアテキスト設定
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_up_act[1]
									   , &main_work->tex_up_act[1]->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , tex_clr_id
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_up_act[1])->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_up_act[1])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
	if (dm_clrdm_stage_text_amb_id[main_work->stage_id] == 0) {
		lang_id = 0;
		tmp_ama_id = IDB_CPIT_MAIN_G_RSLT_AMA;
	}
	else {
		lang_id = 1;
		tmp_ama_id = g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()];
	}
	
	// THROUGHテキスト設定
	if (lang_id == 0
		&& (lang_type == GSD_LANGUAGE_JP || lang_type == GSD_LANGUAGE_US)) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_up_act[2]
										   , &main_work->tex_up_act[2]->obj_2d
										   , NULL
										   , NULL
										   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[1])
										   , IDA_G_RSLT_JP_ACT_TEX_THRO
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->tex_up_act[2])->scale.x = DMD_CLROM_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->tex_up_act[2])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	}
	
	
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_up_act[3]
									   , &main_work->tex_up_act[3]->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , dm_clrdm_stage_text_act_id[main_work->stage_id]
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->tex_up_act[3])->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->tex_up_act[3])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	
	// ドイツ語のとき
	if (lang_type == GSD_LANGUAGE_GE) {
		num_act_id = dm_clrdm_stage_ge_num_act_id[main_work->stage_id];
	}
	// フランス語のとき
	else if (lang_type == GSD_LANGUAGE_FR) {
		num_act_id = dm_clrdm_stage_fr_num_act_id[main_work->stage_id];
	}
	// スペイン語のとき
	else if (lang_type == GSD_LANGUAGE_SP) {
		num_act_id = dm_clrdm_stage_sp_num_act_id[main_work->stage_id];
	}
	// イタリア語のとき
	else if (lang_type == GSD_LANGUAGE_IT) {
		num_act_id = dm_clrdm_stage_sp_num_act_id[main_work->stage_id];
	}
	// ドイツ語以外のとき
	else {
		num_act_id = dm_clrdm_stage_num_act_id[main_work->stage_id];
	}
	
	if (num_act_id != 0xffff) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_up_act[4]
										   , &main_work->tex_up_act[4]->obj_2d
										   , NULL
										   , NULL
										   , tmp_ama_id
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[lang_id])
										   , num_act_id
										   , FALSE
										   );
#if _IPHONE
		((OBS_OBJECT_WORK *)main_work->tex_up_act[4])->scale.x = DMD_CLROM_TEXT_SCALE_X;
		((OBS_OBJECT_WORK *)main_work->tex_up_act[4])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
	}
}



// ==========================================================================
// gmClearDemoCreateObjSpeActForStage
/*!
	スペシャルステージ別の2Dアクション生成処理
 */
// ==========================================================================
void gmClearDemoCreateObjSpeActForStage(GMS_CLRDM_MAIN_WORK *main_work)
{
	GSE_LANGUAGE lang_type = GSD_LANGUAGE_JP;
	s32 tmp_disp_id = 0;
	s32 tmp_spe_act_id = 0;
	
	lang_type = GsEnvGetLanguage();
	
	tmp_disp_id = (s32)(main_work->stage_id - GSD_MAIN_STAGE_ID_SS1);
	
	if (lang_type != GSD_LANGUAGE_IT) {
		tmp_spe_act_id = IDA_G_RSLT_ACT_TA_NUM1;
	}
	else {
		tmp_spe_act_id = IDA_G_RSLT_ACT_IT_NUM1;
	}
	
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->spst_num_act[tmp_disp_id]
									   , &main_work->spst_num_act[tmp_disp_id]->obj_2d
									   , NULL
									   , NULL
									   , IDB_CPIT_MAIN_G_RSLT_AMA
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[0])
									   , (u32)(tmp_spe_act_id + tmp_disp_id)
									   , FALSE
									   );
#if _IPHONE
	((OBS_OBJECT_WORK *)main_work->spst_num_act[tmp_disp_id])->scale.x = DMD_CLROM_TEXT_SCALE_X;
	((OBS_OBJECT_WORK *)main_work->spst_num_act[tmp_disp_id])->scale.y = DMD_CLROM_TEXT_SCALE_Y;
#endif //_IPHONE
}



// ==========================================================================
// gmClearDemoSetSortBufAct
/*!
	アクションのソートバッファ設定処理
 */
// ==========================================================================
void gmClearDemoSetSortBufAct(GMS_CLRDM_MAIN_WORK *main_work)
{
	OBS_OBJECT_WORK		*obj_work;
	int i = 0;

	// 表示フラグがONのときのみ表示
	if (!(main_work->flag & GMD_CLRDM_FLAG_DISP_LINE)) {
		for (i = 0; i < 3; i++) {
			obj_work = (OBS_OBJECT_WORK *)main_work->line_act[i];
			obj_work->disp_flag |= OBD_DISP_NODISP;
		}
	}
	else {
		for (i = 0; i < 3; i++) {
			obj_work = (OBS_OBJECT_WORK *)main_work->line_act[i];
			obj_work->disp_flag &= ~OBD_DISP_NODISP;
		}
	}
	
	if (!(main_work->flag & GMD_CLRDM_FLAG_DISP_SONIC_ICON)) {
		obj_work = (OBS_OBJECT_WORK *)main_work->sonic_icon_act;
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	else {
		obj_work = (OBS_OBJECT_WORK *)main_work->sonic_icon_act;
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
	}

	gmClearDemoSetSortBufScore(main_work);
}



// ==========================================================================
// gmClearDemoSetTimeAtkSortBufAct
/*!
	タイムアタック時のアクションのソートバッファ設定処理
 */
// ==========================================================================
void gmClearDemoSetTimeAtkSortBufAct(GMS_CLRDM_MAIN_WORK *main_work)
{
	OBS_OBJECT_WORK		*obj_work;
	int i = 0;

	// 表示フラグがONのときのみ表示
	if (!(main_work->flag & GMD_CLRDM_FLAG_DISP_CLEAR_TIME)) {
		// タイム表示設定フラグON
//		for (i = 0; i < 7; i++) {
//			obj_work = (OBS_OBJECT_WORK *)main_work->record_time_num_act[i];
//			obj_work->disp_flag |= OBD_DISP_NODISP;
//		}
	}
	else {
		// タイム表示設定フラグON
		for (i = 0; i < 7; i++) {
			obj_work = (OBS_OBJECT_WORK *)main_work->record_time_num_act[i];
			obj_work->disp_flag &= ~OBD_DISP_NODISP;
		}
	}
	
	// スペステでカオスエメラルドが揃っていない場合、光演出表示用フラグ
#if !_IPHONE	
	// ここでソニックの状態別で表示を変更する
	if (GMM_MAIN_USE_SUPER_SONIC()) {
		main_work->time_sonic_icon_act->obj_2d.frame = 1.0f;
	}
	else {
		main_work->time_sonic_icon_act->obj_2d.frame = 0.0f;
	}
	
	main_work->time_sonic_icon_act->obj_2d.speed = 0.0f;		// 暫定
#endif //!_IPHONE
	gmClearDemoSetSortBufScore(main_work);
}



// ==========================================================================
// gmClearDemoSetRetryInitAct
/*!
	タイムアタック時のアクションのソートバッファ設定処理
 */
// ==========================================================================
void gmClearDemoSetRetryInitAct(GMS_CLRDM_MAIN_WORK *main_work)
{
	OBS_OBJECT_WORK		*obj_work;
	int i = 0;

	// 表示フラグがONのときのみ表示
	for (i = 0; i < 5; i++) {
		obj_work = (OBS_OBJECT_WORK *)main_work->tex_up_act[i];
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	
	for (i = 0; i < 7; i++) {
		obj_work = (OBS_OBJECT_WORK *)main_work->record_time_num_act[i];
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	
	obj_work = (OBS_OBJECT_WORK *)main_work->tex_big_time_act;
	obj_work->disp_flag |= OBD_DISP_NODISP;
	obj_work = (OBS_OBJECT_WORK *)main_work->time_sonic_icon_act;
	obj_work->disp_flag |= OBD_DISP_NODISP;
	obj_work = (OBS_OBJECT_WORK *)main_work->tex_new_record_act;
	obj_work->disp_flag |= OBD_DISP_NODISP;
	
#if !_IPHONE
	main_work->tex_retry_act->obj_2d.frame = 1.0f;
#else //!_IPHONE
	main_work->tex_retry_act->obj_2d.frame = 0.0f;
#endif //!_IPHONE
	main_work->tex_retry_act->obj_2d.speed = 0.0f;		// 暫定
	
	main_work->tex_back_slct_act->obj_2d.frame = 0.0f;
	main_work->tex_back_slct_act->obj_2d.speed = 0.0f;		// 暫定

#if _IPHONE
	for (int i = 0, max = arrayof(main_work->btn_retry); i < max; ++i) {
		main_work->btn_retry[i]->obj_2d.frame = 0.0f;
		main_work->btn_retry[i]->obj_2d.speed = 0.0f;
	}

	for (int i = 0, max = arrayof(main_work->btn_back); i < max; ++i) {
		main_work->btn_back[i]->obj_2d.frame = 0.0f;
		main_work->btn_back[i]->obj_2d.speed = 0.0f;
	}
#endif //_IPHONE
}



// ==========================================================================
// gmClearDemoSetRetrySortBufAct
/*!
	タイムアタック時のアクションのソートバッファ設定処理
 */
// ==========================================================================
void gmClearDemoSetRetrySortBufAct(GMS_CLRDM_MAIN_WORK *main_work)
{
	OBS_OBJECT_WORK		*obj_work;
	int i = 0;

	// 表示フラグがONのときのみ表示
	if (main_work->is_clear_spe_stg) {
		for (i = 0; i < 3; i++) {
			obj_work = (OBS_OBJECT_WORK *)main_work->tex_spst_up_act[i];
			obj_work->disp_flag |= OBD_DISP_NODISP;
		}
	}
	else {
		for (i = 0; i < 5; i++) {
			obj_work = (OBS_OBJECT_WORK *)main_work->tex_up_act[i];
			obj_work->disp_flag |= OBD_DISP_NODISP;
		}
	}
	
	for (i = 0; i < 7; i++) {
		obj_work = (OBS_OBJECT_WORK *)main_work->record_time_num_act[i];
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
	
	obj_work = (OBS_OBJECT_WORK *)main_work->tex_big_time_act;
	obj_work->disp_flag |= OBD_DISP_NODISP;
	obj_work = (OBS_OBJECT_WORK *)main_work->time_sonic_icon_act;
	obj_work->disp_flag |= OBD_DISP_NODISP;
	obj_work = (OBS_OBJECT_WORK *)main_work->tex_new_record_act;
	obj_work->disp_flag |= OBD_DISP_NODISP;
	
	
	// リトライ背景
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->bg_retry
									   , &main_work->bg_retry->obj_2d
									   , NULL
									   , NULL
									   , IDB_CPIT_MAIN_G_RSLT_AMA
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[0])
									   , IDA_G_RSLT_ACT_RETRY_BG
									   , FALSE
									   );

#if _IPHONE
	//リトライ台紙
	for (int i = 0, max = arrayof(main_work->btn_retry); i < max; ++i) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->btn_retry[i]
										   , &main_work->btn_retry[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , IDA_G_RSLT_ACT_BTN01_L + i
										   , FALSE
										   );
	}

	// ACT選択へ戻る台紙
	for (int i = 0, max = arrayof(main_work->btn_back); i < max; ++i) {
		ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->btn_back[i]
										   , &main_work->btn_back[i]->obj_2d
										   , NULL
										   , NULL
										   , IDB_CPIT_MAIN_G_RSLT_AMA
										   , GmGameDatGetCockpitData()
										   , AoTexGetTexList(&main_work->tex[0])
										   , IDA_G_RSLT_ACT_BTN02_L + i
										   , FALSE
										   );
	}
#endif //_IPHONE
	
	// リトライテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_retry_act
									   , &main_work->tex_retry_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_RE_TEX_RETRY
									   , FALSE
									   );
	
	// ACT選択へ戻るテキスト
	ObjObjectAction2dAMALoadSetTexlist((OBS_OBJECT_WORK *)main_work->tex_back_slct_act
									   , &main_work->tex_back_slct_act->obj_2d
									   , NULL
									   , NULL
									   , g_gm_clear_demo_data_ama_id[GsEnvGetLanguage()]
									   , GmGameDatGetCockpitData()
									   , AoTexGetTexList(&main_work->tex[1])
									   , IDA_G_RSLT_JP_ACT_RE_TEX_ACT
									   , FALSE
									   );
	
	
#if !_IPHONE
	main_work->tex_retry_act->obj_2d.frame = 1.0f;
#else //!_IPHONE
	main_work->tex_retry_act->obj_2d.frame = 0.0f;
#endif //!_IPHONE
	main_work->tex_retry_act->obj_2d.speed = 0.0f;		// 暫定
	
	main_work->tex_back_slct_act->obj_2d.frame = 0.0f;
	main_work->tex_back_slct_act->obj_2d.speed = 0.0f;		// 暫定

#if _IPHONE //ボタン台紙
	for (int i = 0, max = arrayof(main_work->btn_retry); i < max; ++i) {
		main_work->btn_retry[i]->obj_2d.frame = 0.0f;
		main_work->btn_retry[i]->obj_2d.speed = 0.0f;
	}

	for (int i = 0, max = arrayof(main_work->btn_back); i < max; ++i) {
		main_work->btn_back[i]->obj_2d.frame = 0.0f;
		main_work->btn_back[i]->obj_2d.speed = 0.0f;
	}
#endif //_IPHONE //ボタン台紙

#if _IPHONE	//当たり判定構築
	{	//リトライ
		er::CTrgAoAction &trg = main_work->trg_retry;
		AOS_ACTION *act = main_work->btn_retry[1]->obj_2d.act;
		trg.Create(act);
	}
	{	//戻る
		er::CTrgAoAction &trg = main_work->trg_back;
		AOS_ACTION *act = main_work->btn_back[1]->obj_2d.act;
		trg.Create(act);
	}
#endif //_IPHONE	//当たり判定構築
}



// ==========================================================================
// gmClearDemoSetSortBufScore
/*!
	アクションのソートバッファ設定処理
 */
// ==========================================================================
void gmClearDemoSetSortBufScore(GMS_CLRDM_MAIN_WORK *main_work)
{
	// タイムスコアの桁数アクション登録
	if (main_work->flag & GMD_CLRDM_FLAG_DISP_TIME_SCORE) {
		gmClearDemoSetSortBufScoreAct(main_work->time_num_act
									  , main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
									  , 5);
	}

	// リングスコアの桁数アクション登録
	if (main_work->flag & GMD_CLRDM_FLAG_DISP_RING_SCORE) {
		gmClearDemoSetSortBufScoreAct(main_work->ring_num_act
									  , main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
									  , 5);
	}

	// トータルスコアの桁数アクション登録
	if (main_work->flag & GMD_CLRDM_FLAG_DISP_TOTAL_SCORE) {
		gmClearDemoSetSortBufScoreAct(main_work->total_num_act
									  , main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
									  , 9);
	}

}



// ==========================================================================
// gmClearDemoSetSortBufScoreAct
/*!
	アクションのソートバッファ設定処理
 */
// ==========================================================================
void gmClearDemoSetSortBufScoreAct(GMS_COCKPIT_2D_WORK *score_act[], u32 score, u32 digits)
{
	OBS_OBJECT_WORK		*obj_work;
	int i = 0;
	int j = 0;
	int tmp_digit = 1;
	
	// スコアの値を見て、何桁までをソートバッファに登録するかを設定する
	if (score < 10) {
		
		for (i = 0; i < (s32)(digits - 1); i++) {
			obj_work = (OBS_OBJECT_WORK *)score_act[i];
			obj_work->disp_flag |= OBD_DISP_NODISP;
		}

		obj_work = (OBS_OBJECT_WORK *)score_act[digits - 1];
		obj_work->disp_flag &= ~OBD_DISP_NODISP;
	}

	else if (score >= (u32)10) {
		for (i = 0; i < (s32)digits; i++) {
			
			for (j = 0; j < (s32)(digits - i - 1); j++) {
				tmp_digit = tmp_digit * 10;
			}

			if (score < (u32)tmp_digit) {
				
				obj_work = (OBS_OBJECT_WORK *)score_act[i];
				obj_work->disp_flag |= OBD_DISP_NODISP;
			}
			else {
				obj_work = (OBS_OBJECT_WORK *)score_act[i];
				obj_work->disp_flag &= ~OBD_DISP_NODISP;
			}

			tmp_digit = 1;
		}
	}

	else {
		
	}
}



// ==========================================================================
// gmClearDemoSetSortBufTimeAct
/*!
	アクションのソートバッファ設定処理
 */
// ==========================================================================
void gmClearDemoSetSortBufTimeAct(GMS_CLRDM_MAIN_WORK *main_work)
{
	int tmp_sec_10 = 0;
	int tmp_sec_1 = 0;
	int tmp_msec_10 = 0;
	int tmp_msec_1 = 0;
	
	if (main_work->time_sec >= 10) {
		tmp_sec_10 = main_work->time_sec / 10;
	}
	else {
		tmp_sec_10 = 0;
	}
	
	if (main_work->time_msec >= 10) {
		tmp_msec_10 = main_work->time_msec / 10;
	}
	else {
		tmp_msec_10 = 0;
	}
	
	tmp_sec_1 = main_work->time_sec % 10;
	tmp_msec_1 = main_work->time_msec % 10;
	
	// 分
	main_work->record_time_num_act[0]->obj_2d.frame = (float)main_work->time_min;
	main_work->record_time_num_act[2]->obj_2d.frame = (float)tmp_sec_10;
	main_work->record_time_num_act[3]->obj_2d.frame = (float)tmp_sec_1;
	main_work->record_time_num_act[5]->obj_2d.frame = (float)tmp_msec_10;
	main_work->record_time_num_act[6]->obj_2d.frame = (float)tmp_msec_1;
	
	main_work->record_time_num_act[1]->obj_2d.frame = 0.0f;
	main_work->record_time_num_act[4]->obj_2d.frame = 0.0f;
	
}



// ==========================================================================
// gmClearDemoUpdateAct
/*!
	アクション更新処理
 */
// ==========================================================================
void gmClearDemoUpdateAct(GMS_CLRDM_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
}



// ==========================================================================
// gmClearDemoTimeFlushEffect
/*!
	時間表示部の閃光演出処理
 */
// ==========================================================================
void gmClearDemoTimeFlushEffect(GMS_CLRDM_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
}



// ==========================================================================
// gmClearDemoSetCalcScore
/*!
	スコア計算設定処理(タイムスコア・リングスコアを一度に加減算する処理)
 */
// ==========================================================================
void gmClearDemoSetCalcScore(GMS_CLRDM_MAIN_WORK *main_work)
{
	// リングスコア側が0以下になったら
	if (main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] <= 0
		&& main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] <= 0) {
		main_work->proc_calc_score = NULL;
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_SCORE_CALC_END);

		return;
	}

	// タイムスコア加減算処理
	if (main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] > 0) {
		main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			 -= GMD_CLRDM_SCORE_UPDOWN_NUM;

		main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			 += GMD_CLRDM_SCORE_UPDOWN_NUM;
	}
	
	// リングスコア加減算処理
	if (main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] > 0) {
		main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			 -= GMD_CLRDM_SCORE_UPDOWN_NUM;

		main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
			 += GMD_CLRDM_SCORE_UPDOWN_NUM;
	}
}



// ==========================================================================
// gmClearDemoSetCalcScoreForTime
/*!
	タイムスコア計算設定処理(スコアを加減算する処理)
 */
// ==========================================================================
#if 0
void gmClearDemoSetCalcScoreForTime(GMS_CLRDM_MAIN_WORK *main_work)
{
	// タイムスコア側が0以下になったら
	if (main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] <= 0) {
		main_work->proc_calc_score = gmClearDemoSetCalcScoreForRing;

		return;
	}

	// スコア加減算処理
	main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
		 -= GMD_CLRDM_SCORE_UPDOWN_NUM;

	main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
		 += GMD_CLRDM_SCORE_UPDOWN_NUM;
}
#endif

// ==========================================================================
// gmClearDemoSetCalcScoreForRing
/*!
	リングスコア計算設定処理(スコアを加減算する処理)
 */
// ==========================================================================
#if 0
void gmClearDemoSetCalcScoreForRing(GMS_CLRDM_MAIN_WORK *main_work)
{
	// リングスコア側が0以下になったら
	if (main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA] <= 0) {
		main_work->proc_calc_score = NULL;
		amFlagOn(main_work->flag, GMD_CLRDM_FLAG_SCORE_CALC_END);

		return;
	}

	// スコア加減算処理
	main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
		 -= GMD_CLRDM_SCORE_UPDOWN_NUM;

	main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
		 += GMD_CLRDM_SCORE_UPDOWN_NUM;
}
#endif


// ==========================================================================
// gmClearDemoSetScoreData
/*!
	スコアの値に沿ったアクション設定処理
 */
// ==========================================================================
void gmClearDemoSetScoreData(GMS_CLRDM_MAIN_WORK *main_work)
{
	
	// タイムスコアの桁数アクション登録7
	gmClearDemoSetDispScore(main_work->time_num_act
						  , main_work->time_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
						  , 5);

	// リングスコアの桁数アクション登録
	gmClearDemoSetDispScore(main_work->ring_num_act
						  , main_work->ring_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
						  , 5);

	// トータルスコアの桁数アクション登録
	gmClearDemoSetDispScore(main_work->total_num_act
						  , main_work->total_score[GME_CLRDM_SCORE_TYPE_DISP_DATA]
						  , 9);

}



// ==========================================================================
// gmClearDemoSetDispScore
/*!
	スコア表示判定設定処理
 */
// ==========================================================================
void gmClearDemoSetDispScore(GMS_COCKPIT_2D_WORK *score_act[], u32 score, u32 digits)
{
	int i = 0;
	int j = 0;
	f32 disp = 0.0f;
	int tmp_digit = 1;

	
	// 下２桁は常に0
	for (i = 0; i < 1; i++) {
		score_act[digits - 1 - i]->obj_2d.frame = 0.0f;
		score_act[digits - 1 - i]->obj_2d.speed = 0.0f;		// 暫定
	}
	
	// 3桁以上は商により値を求める
	for (i = 1; i < (s32)digits; i++) {
		for (j = 0; j < i; j++) {
			tmp_digit = tmp_digit * 10;
		}

		// 指定桁数よりも多い場合のみ、算出
		if (score >= (u32)tmp_digit) {
			disp = (f32)(score / tmp_digit);

			disp = (f32)((int)disp % 10);
			score_act[digits - 1 - i]->obj_2d.frame = disp;
			score_act[digits - 1 - i]->obj_2d.speed = 0.0f;		// 暫定
		}
		else {
			score_act[digits - 1 - i]->obj_2d.frame = 0.0f;
			score_act[digits - 1 - i]->obj_2d.speed = 0.0f;		// 暫定
		}

		tmp_digit = 1;
	}
	
}



// ==========================================================================
// gmClearDemoSetFlashSonic
/*!
	ソニックアイコンの点滅設定処理
 */
// ==========================================================================
void gmClearDemoSetFlashSonic(GMS_CLRDM_MAIN_WORK *main_work)
{
	// 表示時
	if (main_work->flag & GMD_CLRDM_FLAG_DISP_SONIC_ICON) {
		if (main_work->flash_timer >= GMD_CLRDM_SONIC_DISP_TIME) {
			amFlagOff(main_work->flag, GMD_CLRDM_FLAG_DISP_SONIC_ICON);
			main_work->flash_timer = 0;
		}
	}
	
	// 非表示時
	else {
		if (main_work->flash_timer >= GMD_CLRDM_SONIC_NODISP_TIME) {
			amFlagOn(main_work->flag, GMD_CLRDM_FLAG_DISP_SONIC_ICON);
			main_work->flash_timer = 0;
		}
	}
	
	main_work->flash_timer += main_work->count;
}



// ==========================================================================
// gmClearDemoIsTexLoad
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
BOOL gmClearDemoIsTexLoad()
{
	s32 result = 0;

	for (int i = 0; i < GME_CLRDM_DATA_TYPE_MAX; i++) {
		if (AoTexIsLoaded(&gm_clrdm_tex[i])) {
			// フラグ扱いでON
			result |= 1 << i;
		}
	}

	if (result == 3) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


// ==========================================================================
// gmClearDemoIsTexRelease
/*!
	テクスチャ解放完了チェック処理
 */
// ==========================================================================
BOOL gmClearDemoIsTexRelease()
{
	s32 result = 0;

	for (int i = 0; i < GME_CLRDM_DATA_TYPE_MAX; i++) {
		if (AoTexIsReleased(&gm_clrdm_tex[i])) {
			// フラグ扱いでON
			result |= 1 << i;
		}
	}

	if (result == 3) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}



// =======================================================================
// gmClearDemoGetRingNum
/*!
  リング数取得
  
  @return 現在のリング数
  */
// =======================================================================
inline s16 gmClearDemoGetRingNum(void)
{
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]) {
		return g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->ring_num;
	}
	else {
		// プレイヤー生成されてないときは0を返す
		return 0;
	}
}

// =======================================================================
// gmClearDemoGetScore
/*!
  スコア取得
  
  @return 現在のスコア
 */
// =======================================================================
inline u32 gmClearDemoGetScore(void)
{
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]) {
		return g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->score;
	}
	else {
		// プレイヤー生成されてないときは0を返す
		return 0;
	}
}

// =======================================================================
// gmClearDemoGetGameTime
/*!
  ゲームタイマ値取得
  
  @return 現在のゲームタイマ値
 */
// =======================================================================
inline u32 gmClearDemoGetGameTime(void)
{
#if 0
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_TIMEOVER) {
		if (g_gs_main_sys_info.stage_id >= GSD_MAIN_STAGE_ID_SS1) {
			return 0;
		}
		else {
			return GMD_MAIN_TIME_MAX;
		}
	}
#endif
	// スペステプレイ時でカオスエメラルドを取っていない場合
	if (g_gs_main_sys_info.stage_id >= GSD_MAIN_STAGE_ID_SS1
		&& !(g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_CHAOSGET)) {
		// レコードタイムを0に固定
		return 0;
	}
	
	// 9分59秒を超える場合、9分59秒に設定
	if (g_gm_main_system.game_time >= GMD_MAIN_TIME_MAX) {
		return GMD_MAIN_TIME_MAX;
	}
	
	return g_gm_main_system.game_time;
}

// =======================================================================
// gmClearDemoGetChallengeNum
/*!
  チャレンジ数取得
  
  @return 現在のチャレンジ数
 */
// =======================================================================
inline u32 gmClearDemoGetChallengeNum(void)
{
	return g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P];
}


#if _IPHONE
// =======================================================================
// gmClearDemoSetBgColorBlack
/*!
  BGカラーを黒に変更
  
  @note
	インゲーム側がBGカラーを設定し続けているので、実行した1fのみ有効
 */
// =======================================================================
void gmClearDemoSetBgColorBlack(AMS_TCB *tcb) {
	if (tcb) {
		//描画タスクなら
		NNS_RGBA_U8 color = {0x00, 0x00, 0x00, 0xFF};
		amDrawSetBGColor(&color);
	} else {
		//描画タスクじゃないなら
		amDrawMakeTask(gmClearDemoSetBgColorBlack, 0xFF00);
	}
}
#endif //_IPHONE


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
