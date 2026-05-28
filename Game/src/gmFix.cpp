// =======================================================================
/*!
  @file	gmFix.cpp
  @brief FIX

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmFix.cpp 20 2011-04-22 12:46:46Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "gsEnvironment.h"
#include "gmMain.h"
#include "gmPlayer.h"
#include "gmGameDat.h"
#include "gmCockpit.h"
#include "akMath.h"
#include "akUtil.h"
#include "gmSound.h"
#if _IPHONE
#include "gmPause.h"
#include "gmPadVirtualPad.hpp"
#endif //_IPHONE

#include "gmFix.h"

// データヘッダ
#include "common/arc/CPIT_MAIN.HMB"
#if !_IPHONE
#include "common/ace/G_FIX.HMA"
#else //!_IPHONE
#include "iPhone/ace/G_FIX.HMA"
#endif //!_IPHONE
#include "common/ace/SPST.HMA"
#include "common/ace/SPST_FR.HMA"
#include "common/ace/SPST_GE.HMA"
#include "common/ace/SPST_IT.HMA"
#include "common/ace/SPST_JP.HMA"
#include "common/ace/SPST_SP.HMA"
#include "common/ace/SPST_US.HMA"

/*------ Macros --------------------------------------------------------*/
//############ FIX管理 ########################################################
/* フラグ */
#define GMD_FIX_MGR_FLAG_CLEAR				(1 << 0)	//!< タスククリアフラグ
#define GMD_FIX_MGR_FLAG_HIDE				(1 << 1)	//!< 非表示フラグ
#define GMD_FIX_MGR_FLAG_TIMER_COUNTDOWN	(1 << 2)	//!< タイマがカウントダウンタイプ（タイムアウト直前判定の閾値が変わります）

#if _IPHONE
#define GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_SUPER_SONIC	(1 << 9)	//!< バーチャルパッド(スーバーソニック)非表示フラグ
#define GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_PAUSE		(1 <<10)	//!< バーチャルパッド(ポーズ)非表示フラグ
#define GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_ACTION		(1 <<11)	//!< バーチャルパッド(アクション)非表示フラグ
#define GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_MOVE_PAD		(1 <<12)	//!< バーチャルパッド(十時キー)非表示フラグ
#endif //_IPHONE

/* リクエストフラグ GMS_FIX_MGR_WORK::req_flag */
#define GMD_FIX_MGR_REQUEST_FLAG_TIMER_FLASH_ACT	(1 << 0)	//!< タイマフラッシュ動作リクエスト

/* 定義値 */

//############ FIXパーツ共通 ##################################################
/* フラグ */
#define GMD_FIX_PART_FLAG_ACTIVE	(1 << 0)		//!< パーツ有効フラグ
#define GMD_FIX_PART_FLAG_NODISP	(1 << 1)		//!< パーツ非表示フラグ
#define GMD_FIX_PART_FLAG_BLINK		(1 << 2)		//!< 点滅有効フラグ（点滅自体はパーツ各自で実装）
#define GMD_FIX_PART_FLAG_BLINK_OFF	(1 << 3)		//!< 「暗」状態フラグ

/* 定義値 */

//############ リングカウント パーツ ##########################################
/* フラグ */
/* 定義値 */
#define GMD_FIX_PART_RINGCOUNT_DIGIT_NUM			(3)		//!< リングカウントの桁数
#define GMD_FIX_PART_RINGCOUNT_DIGIT_BLINK_ON_TIME	(10)	//!< 数字点滅「明」状態時間
#define GMD_FIX_PART_RINGCOUNT_DIGIT_BLINK_OFF_TIME	(10)	//!< 数字点滅「暗」状態時間

//############ スコアパーツ ###################################################
/* フラグ */
/* 定義値 */
#define GMD_FIX_PART_SCORE_DIGIT_NUM				(9)		//!< スコアの桁数

#define GMD_FIX_PART_SCORE_S22_TA_NARROWSCREEN_ADJUST_OFST_X	((fx32)(FX32_ONE * 64))	//!< Stage2-2でタイムアタックモードかつ画面アスペクト比4:3の時にこの値だけX位置をずらす

//############ タイマパーツ ###################################################
/* フラグ */
#define GMD_FIX_PART_TIMER_FLAG_RED_BY_TEX_FRAME	(1 << 0)	//!< オンの場合は、フレーム設定で赤数字・文字に設定
#define GMD_FIX_PART_TIMER_FLAG_FLASH_ACT_ACTIVE	(1 << 1)	//!< フラッシュ動作中フラグ
/* 定義値 */
#define GMD_FIX_PART_TIMER_UNIT_NUM					(3)		//!< 分・秒などで分けられる単位の数（分・秒・ミリ秒の単位を使用ならば→3）
#define GMD_FIX_PART_TIMER_MIN_DIGIT_NUM			(1)		//!< 分 表示桁数
#define GMD_FIX_PART_TIMER_SEC_DIGIT_NUM			(2)		//!< 秒 表示桁数
#define GMD_FIX_PART_TIMER_MSEC_DIGIT_NUM			(2)		//!< ミリ秒 表示桁数
#define GMD_FIX_PART_TIMER_ALL_DIGIT_NUM			(GMD_FIX_PART_TIMER_MIN_DIGIT_NUM \
													 + GMD_FIX_PART_TIMER_SEC_DIGIT_NUM \
													 + GMD_FIX_PART_TIMER_MSEC_DIGIT_NUM)
#define GMD_FIX_PART_TIMER_DECO_CHAR_NUM			(2)		//!< 装飾文字（プライムなど）の数

#define GMD_FIX_PART_TIMER_DIGIT_RED_FRAME_OFST		(10.f)	//!< 通常カラーの数字アクションにこのフレームを加算すると赤バージョンになる
#define GMD_FIX_PART_TIMER_DECO_CHAR_RED_FRAME_OFST	(1.f)	//!< 通常カラーの装飾文字アクションにこのフレームを加算すると赤バージョンになる

#define GMD_FIX_PART_TIMER_DIGIT_BLINK_ON_TIME		(10)	//!< 数字点滅「明」状態時間
#define GMD_FIX_PART_TIMER_DIGIT_BLINK_OFF_TIME		(10)	//!< 数字点滅「暗」状態時間
#define GMD_FIX_PART_TIMER_DIGIT_BLINK_REMAIN_SEC	(20)	//!< 残り何秒で点滅開始か

#define GMD_FIX_PART_TIMER_FLASH_ACT_SCALE_MAX		((fx32)(FX32_ONE * 1.5f))	//!< フラッシュ動作時のスケールデフォルト値
#define GMD_FIX_PART_TIMER_FLASH_ACT_SCALE_DEF		((fx32)(FX32_ONE * 1))	//!< フラッシュ動作時のスケール最大値

//############ チャレンジ数パーツ #############################################
/* フラグ */
/* 定義値 */
#define GMD_FIX_PART_CHALLENGE_DIGIT_NUM			(3)		//!< チャレンジ数の桁数

#if _IPHONE
//############ バーチャルパッドパーツ ##########################################
/* フラグ */
/* 定義値 */
#endif //_IPHONE


/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! FIXパーツタイプ列挙型
typedef enum
{
	GME_FIX_PART_TYPE_RINGCOUNT	= 0,	//!< リング数
	GME_FIX_PART_TYPE_SCORE,			//!< スコア
	GME_FIX_PART_TYPE_TIMER,			//!< タイマ
	GME_FIX_PART_TYPE_CHALLENGE,		//!< チャレンジ数
#if _IPHONE
	GME_FIX_PART_TYPE_VIRTUAL_PAD,		//!< バーチャルパッド
#endif //_IPHONE
	
	GME_FIX_PART_TYPE_MAX,
	GME_FIX_PART_TYPE_NONE	= -1
} GME_FIX_PART_TYPE;

//! テクスチャアーカイブインデックス
typedef enum
{
	GME_FIX_TEX_IDX_FIX	= 0,
	GME_FIX_TEX_IDX_SSFIX_CMN,
	GME_FIX_TEX_IDX_SSFIX_LANG,
	
	GME_FIX_TEX_IDX_MAX
} GME_FIX_TEX_IDX;

//! AMAデータインデックス
typedef enum
{
	GME_FIX_AMA_IDX_COMMON	= 0,	//!< 共通データ
	GME_FIX_AMA_IDX_SSFIX_CMN,		//!< スペステ用共通データ
	GME_FIX_AMA_IDX_SSFIX_LANG,		//!< スペステ用言語別データ
	
	GME_FIX_AMA_IDX_MAX
} GME_FIX_AMA_IDX;

#if _IPHONE
//! FIXパーツ配置
typedef enum
{
	GME_FIX_PLAN_TILT			= 0,	//<傾斜操作
	GME_FIX_PLAN_FLICK,					//<バーチャルパッド(フリック)
	GME_FIX_PLAN_VIRTUAL_PAD,			//<バーチャルパッド(通常)
	
	GME_FIX_PLAN_MAX
} GME_FIX_PLAN;
#endif //_IPHONE

//! リングパーツ サブパーツインデックス列挙型
typedef enum
{
	GME_FIX_RING_SUBPART_ICON	= 0,
	GME_FIX_RING_SUBPART_DIGIT_100,
	GME_FIX_RING_SUBPART_DIGIT_10,
	GME_FIX_RING_SUBPART_DIGIT_1,
	
	GME_FIX_RING_SUBPART_MAX
} GME_FIX_RING_SUBPART;

//! スコアパーツ サブパーツインデックス列挙型
typedef enum
{
	GME_FIX_SCORE_SUBPART_TAB = 0,
	GME_FIX_SCORE_SUBPART_NUM_1,	// 左の桁からの順番
	GME_FIX_SCORE_SUBPART_NUM_2,
	GME_FIX_SCORE_SUBPART_NUM_3,
	GME_FIX_SCORE_SUBPART_NUM_4,
	GME_FIX_SCORE_SUBPART_NUM_5,
	GME_FIX_SCORE_SUBPART_NUM_6,
	GME_FIX_SCORE_SUBPART_NUM_7,
	GME_FIX_SCORE_SUBPART_NUM_8,
	GME_FIX_SCORE_SUBPART_NUM_9,
	
	GME_FIX_SCORE_SUBPART_MAX
} GME_FIX_SCORE_SUBPART;

//! タイマパーツ サブパーツインデックス列挙型
typedef enum
{
	GME_FIX_TIMER_SUBPART_ICON	= 0,
	GME_FIX_TIMER_SUBPART_NUM_10000,
	GME_FIX_TIMER_SUBPART_NUM_AP1,
	GME_FIX_TIMER_SUBPART_NUM_1000,
	GME_FIX_TIMER_SUBPART_NUM_100,
	GME_FIX_TIMER_SUBPART_NUM_AP2,
	GME_FIX_TIMER_SUBPART_NUM_10,
	GME_FIX_TIMER_SUBPART_NUM_1,
	
	GME_FIX_TIMER_SUBPART_MAX
} GME_FIX_TIMER_SUBPART;

//! チャレンジ数パーツ サブパーツインデックス列挙型
typedef enum
{
	GME_FIX_CHALLENGE_SUBPART_SONIC	= 0,
	GME_FIX_CHALLENGE_SUBPART_STRING,	// SONICの文字
	GME_FIX_CHALLENGE_SUBPART_NUM_100,
	GME_FIX_CHALLENGE_SUBPART_NUM_10,
	GME_FIX_CHALLENGE_SUBPART_NUM_1,
	
	GME_FIX_CHALLENGE_SUBPART_MAX
} GME_FIX_CHALLENGE_SUBPART;

#if _IPHONE
//! バーチャルパッドパーツ サブパーツインデックス列挙型
typedef enum
{
	GME_FIX_VIRTUAL_PAD_SUBPART_SSONIC = 0,	//スーパーソニック化ボタン
	GME_FIX_VIRTUAL_PAD_SUBPART_PAUSE,		//ポーズ
	GME_FIX_VIRTUAL_PAD_SUBPART_ACTION,		//アクション(ジャンプ)
	GME_FIX_VIRTUAL_PAD_SUBPART_MOVE,		//移動キー
	
	GME_FIX_VIRTUAL_PAD_SUBPART_MAX,
} GME_FIX_VIRTUAL_PAD_SUBPART;
#endif //_IPHONE


typedef struct tag_GMS_FIX_MGR_WORK	GMS_FIX_MGR_WORK;
typedef struct tag_GMS_FIX_PART_WORK	GMS_FIX_PART_WORK;

typedef struct tag_GMS_FIX_PART_RINGCOUNT	GMS_FIX_PART_RINGCOUNT;
typedef struct tag_GMS_FIX_PART_SCORE		GMS_FIX_PART_SCORE;
typedef struct tag_GMS_FIX_PART_TIMER		GMS_FIX_PART_TIMER;
typedef struct tag_GMS_FIX_PART_CHALLENGE	GMS_FIX_PART_CHALLENGE;
#if _IPHONE
typedef struct tag_GMS_FIX_PART_VIRTUAL_PAD	GMS_FIX_PART_VIRTUAL_PAD;
#endif //_IPHONE


//! FIX共通パーツワーク構造体
struct tag_GMS_FIX_PART_WORK
{
	GMS_FIX_MGR_WORK	*parent_mgr;
	
	GME_FIX_PART_TYPE	part_type;
	
	Uint32	flag;
	
	Uint32	blink_timer;	//!< 点滅タイマ
	Uint32	blink_on_time;	//!< 点滅 「明」時間
	Uint32	blink_off_time;	//!< 点滅 「暗」時間
	
	//! 更新処理関数（内部的な情報更新）
	void	(*proc_update)(GMS_FIX_PART_WORK*);
	//! 表示関連処理関数（描画処理ではなく、表示に関わるパラメタ設定など、
	//                    内部情報更新がストップしてても行いたい処理）
	void	(*proc_disp)(GMS_FIX_PART_WORK*);
};

//! リンクカウントパーツワーク
struct tag_GMS_FIX_PART_RINGCOUNT
{
	GMS_FIX_PART_WORK	part_work;
	
	GMS_COCKPIT_2D_WORK	*sub_parts[GME_FIX_RING_SUBPART_MAX];
	
	// 各桁の数字
	Sint32	digit_list[GMD_FIX_PART_RINGCOUNT_DIGIT_NUM];
};

//! スコアパーツワーク
struct tag_GMS_FIX_PART_SCORE
{
	GMS_FIX_PART_WORK	part_work;
	
	GMS_COCKPIT_2D_WORK	*sub_parts[GME_FIX_SCORE_SUBPART_MAX];
	
	Sint32 digit_list[GMD_FIX_PART_SCORE_DIGIT_NUM];
};

//! タイマパーツワーク
struct tag_GMS_FIX_PART_TIMER
{
	GMS_FIX_PART_WORK	part_work;
	
	GMS_COCKPIT_2D_WORK	*sub_parts[GME_FIX_TIMER_SUBPART_MAX];
	
	Uint32	flag;
	
	Sint32	digit_list[GMD_FIX_PART_TIMER_ALL_DIGIT_NUM];
	
	Float	digit_frame_ofst;		//!< 数字アクションフレームオフセット（赤数字用にオフセットするために使用）
	Float	deco_char_frame_ofst;	//!< 装飾文字アクションフレームオフセット（赤文字用にオフセットするために使用）
	
	Uint16 cur_sec;	//! 現在の「秒」の値（時間切れ前の秒の切り替えタイミングを得るのに使用）
	Uint16 reserved[1];
	
	Float	fade_ratio;
	Float	scale_ratio;
	Uint32	flash_act_phase;
};

//! チャレンジ数パーツワーク
struct tag_GMS_FIX_PART_CHALLENGE
{
	GMS_FIX_PART_WORK	part_work;
	
	GMS_COCKPIT_2D_WORK	*sub_parts[GME_FIX_CHALLENGE_SUBPART_MAX];
	
	Sint32 digit_list[GMD_FIX_PART_CHALLENGE_DIGIT_NUM];
};

#if _IPHONE
//! バーチャルパッドパーツワーク
struct tag_GMS_FIX_PART_VIRTUAL_PAD
{
	GMS_FIX_PART_WORK	part_work;
	
	GMS_COCKPIT_2D_WORK	*sub_parts[GME_FIX_VIRTUAL_PAD_SUBPART_MAX];
	float				pause_icon_frame[2];
};
#endif //_IPHONE


//! FIX管理ワーク構造体
struct tag_GMS_FIX_MGR_WORK
{
	Uint32	flag;
	Uint32	req_flag;	//!< 動作リクエストフラグ
	
	void	(*proc_update)(GMS_FIX_MGR_WORK*);	//!< 更新処理関数
	
	GMS_FIX_PART_WORK	*part_work[GME_FIX_PART_TYPE_MAX];
	
	GMS_FIX_PART_RINGCOUNT		part_ringcount;
	GMS_FIX_PART_SCORE			part_score;
	GMS_FIX_PART_TIMER			part_timer;
	GMS_FIX_PART_CHALLENGE		part_challenge;
#if _IPHONE
	GMS_FIX_PART_VIRTUAL_PAD	part_virtual_pad;
#endif //_IPHONE
};


// =======================================================================
// GMF_FIX_PART_INIT_FUNC
/*!
  FIXパーツ初期化関数型
 */
// =======================================================================
typedef void (*GMF_FIX_PART_INIT_FUNC)(GMS_FIX_MGR_WORK*);


/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
//############ 共通 ###########################################################
static void gmFixSubpartOutFunc(OBS_OBJECT_WORK *obj_work);
static void gmFixSetFrameStatic(OBS_OBJECT_WORK *obj_work, Float frame);
static void gmFixUpdatePart(GMS_FIX_PART_WORK *part_work);
static BOOL gmFixCheckClear(const GMS_FIX_PART_WORK *part_work);

inline static BOOL gmFixIsSpecialStage(void);
inline static BOOL gmFixIsTimeAttack(void);
inline static BOOL gmFixIsStage22(void);
inline static BOOL gmFixIsStageTruck(void);

#if _IPHONE
inline static GME_FIX_PLAN gmFixGetPlan(void);
#endif //_IPHONE
inline static Sint16 gmFixGetRingNum(void);
inline static Uint32 gmFixGetScore(void);
inline static Uint32 gmFixGetGameTime(void);
inline static Uint32 gmFixGetChallengeNum(void);

static void gmFixInitBlink(GMS_FIX_PART_WORK *part_work, Uint32 on_time, Uint32 off_time);
static BOOL gmFixUpdateBlink(GMS_FIX_PART_WORK *part_work);

//############ 管理 ###########################################################
static void gmFixDest(MTS_TASK_TCB *tcb);
static void gmFixProcMain(MTS_TASK_TCB *tcb);
static void gmFixRegisterPart(GMS_FIX_MGR_WORK *mgr_work, GMS_FIX_PART_WORK *part_work, GME_FIX_PART_TYPE part_type);
static void gmFixUnregisterPart(GMS_FIX_PART_WORK *part_work);
static BOOL gmFixProcessRequest(GMS_FIX_MGR_WORK *mgr_work, Uint32 req_flag_bit);

//############ リングカウント #################################################
static void gmFixRingCountPartInit(GMS_FIX_MGR_WORK* mgr_work);
static void gmFixRingCountPartProcUpdateMain(GMS_FIX_PART_WORK *part_work);
static void gmFixRingCountPartProcDispMain(GMS_FIX_PART_WORK *part_work);
static void gmFixRingCountPartUpdateDigitList(GMS_FIX_PART_RINGCOUNT *part_ringcount);
static void gmFixRingCountPartUpdateActionDigitsType(GMS_FIX_PART_RINGCOUNT *part_ringcount);
static void gmFixRingCountPartSetDispDigits(GMS_FIX_PART_RINGCOUNT *part_ringcount, BOOL enable);

//############ スコア #########################################################
static void gmFixScorePartInit(GMS_FIX_MGR_WORK* mgr_work);
static void gmFixScorePartProcUpdateMain(GMS_FIX_PART_WORK *part_work);
static void gmFixScorePartProcDispMain(GMS_FIX_PART_WORK *part_work);
static void gmFixScorePartUpdateDigitList(GMS_FIX_PART_SCORE *part_score);
static void gmFixScorePartUpdateActionDigitsType(GMS_FIX_PART_SCORE *part_score);

//############ タイマ #########################################################
static void gmFixTimerPartInit(GMS_FIX_MGR_WORK* mgr_work);
static void gmFixTimerSSPartInit(GMS_FIX_MGR_WORK *mgr_work);
static void gmFixTimerPartProcUpdateMain(GMS_FIX_PART_WORK *part_work);
static void gmFixTimerPartProcDispMain(GMS_FIX_PART_WORK *part_work);
static void gmFixTimerPartUpdateDigitList(GMS_FIX_PART_TIMER *part_timer);
static void gmFixTimerPartUpdateActionDigitsType(GMS_FIX_PART_TIMER *part_timer);
static void gmFixTimerPartSetDispDigits(GMS_FIX_PART_TIMER *part_timer, BOOL enable);
static void gmFixTimerPartSetDigitsRed(GMS_FIX_PART_TIMER *part_timer, BOOL enable);
static void gmFixTimerPartSetColorRedDigits(GMS_FIX_PART_TIMER *part_timer, BOOL enable);
static void gmFixTimerPartSetTexRedDigits(GMS_FIX_PART_TIMER *part_timer, BOOL enable);
static BOOL gmFixTimerPartIsTimeRunningOut(const GMS_FIX_MGR_WORK *mgr_work);
static void gmFixTimerPartInitFlashAction(GMS_FIX_PART_TIMER *part_timer);
static void gmFixTimerPartUpdateFlashAction(GMS_FIX_PART_TIMER *part_timer);

//############ チャレンジ数 ###################################################
static void gmFixChallengePartInit(GMS_FIX_MGR_WORK* mgr_work);
static void gmFixChallengePartProcUpdateMain(GMS_FIX_PART_WORK *part_work);
static void gmFixChallengePartProcDispMain(GMS_FIX_PART_WORK *part_work);
static void gmFixChallengePartUpdateDigitList(GMS_FIX_PART_CHALLENGE *part_challenge);
static void gmFixChallengePartUpdateActionDigitsType(GMS_FIX_PART_CHALLENGE *part_challenge);

#if _IPHONE
//############ バーチャルパッド数 #############################################
template <Uint32 NFlag> class gmFixVirtualPadOutClass;
static void gmFixVirtualPadPartInit(GMS_FIX_MGR_WORK* mgr_work);
static void gmFixVirtualPadPartProcUpdateMain(GMS_FIX_PART_WORK *part_work);
static void gmFixVirtualPadPartProcDispMain(GMS_FIX_PART_WORK *part_work);
static bool gmFixVirtualPadPartIsDispSuperSonicIcon(GMS_FIX_PART_VIRTUAL_PAD *part_virtual_pad);
static bool gmFixVirtualPadPartIsDispPauseIcon(GMS_FIX_PART_VIRTUAL_PAD *part_virtual_pad);
static bool gmFixVirtualPadPartIsOnPauseIcon(GMS_FIX_PART_VIRTUAL_PAD *part_virtual_pad);
static float gmFixVirtualPadPartGetMovePadFrame(GMS_FIX_PART_VIRTUAL_PAD *);
static bool gmFixVirtualPadPartIsOnActionIcon(GMS_FIX_PART_VIRTUAL_PAD *);

#endif //_IPHONE


/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! FIX管理タスクTCB
static MTS_TASK_TCB* gm_fix_tcb	= NULL;

//! テクスチャAMB参照ポインタリスト
static void *gm_fix_texamb_list[GME_FIX_TEX_IDX_MAX]	= {0};
//! AOテクスチャ構造体リスト
static AOS_TEXTURE gm_fix_textures[GME_FIX_TEX_IDX_MAX]	= {{0}};

//! テクスチャアーカイブ AMBインデックステーブル
const static Sint32 gm_fix_tex_amb_idx_tbl[GSD_LANGUAGE_NUM][GME_FIX_TEX_IDX_MAX]	= {
	// JP
	{
		IDB_CPIT_MAIN_G_FIX_AMB,
		IDB_CPIT_MAIN_SPST_AMB,
		IDB_CPIT_MAIN_SPST_JP_AMB,
	},
	// US
	{
		IDB_CPIT_MAIN_G_FIX_AMB,
		IDB_CPIT_MAIN_SPST_AMB,
		IDB_CPIT_MAIN_SPST_US_AMB,
	},
	// FR
	{
		IDB_CPIT_MAIN_G_FIX_AMB,
		IDB_CPIT_MAIN_SPST_AMB,
		IDB_CPIT_MAIN_SPST_FR_AMB,
	},
	// IT
	{
		IDB_CPIT_MAIN_G_FIX_AMB,
		IDB_CPIT_MAIN_SPST_AMB,
		IDB_CPIT_MAIN_SPST_IT_AMB,
	},
	// GE
	{
		IDB_CPIT_MAIN_G_FIX_AMB,
		IDB_CPIT_MAIN_SPST_AMB,
		IDB_CPIT_MAIN_SPST_GE_AMB,
	},
	// SP
	{
		IDB_CPIT_MAIN_G_FIX_AMB,
		IDB_CPIT_MAIN_SPST_AMB,
		IDB_CPIT_MAIN_SPST_SP_AMB,
	},
};

//! AMAデータ AMBインデックステーブル
const static Sint32 gm_fix_ama_amb_idx_tbl[GSD_LANGUAGE_NUM][GME_FIX_AMA_IDX_MAX]	= {
	// JP
	{
		IDB_CPIT_MAIN_G_FIX_AMA,
		IDB_CPIT_MAIN_SPST_AMA,
		IDB_CPIT_MAIN_SPST_JP_AMA,
	},
	// US
	{
		IDB_CPIT_MAIN_G_FIX_AMA,
		IDB_CPIT_MAIN_SPST_AMA,
		IDB_CPIT_MAIN_SPST_US_AMA,
	},
	// FR
	{
		IDB_CPIT_MAIN_G_FIX_AMA,
		IDB_CPIT_MAIN_SPST_AMA,
		IDB_CPIT_MAIN_SPST_FR_AMA,
	},
	// IT
	{
		IDB_CPIT_MAIN_G_FIX_AMA,
		IDB_CPIT_MAIN_SPST_AMA,
		IDB_CPIT_MAIN_SPST_IT_AMA,
	},
	// GE
	{
		IDB_CPIT_MAIN_G_FIX_AMA,
		IDB_CPIT_MAIN_SPST_AMA,
		IDB_CPIT_MAIN_SPST_GE_AMA,
	},
	// SP
	{
		IDB_CPIT_MAIN_G_FIX_AMA,
		IDB_CPIT_MAIN_SPST_AMA,
		IDB_CPIT_MAIN_SPST_SP_AMA,
	},
};

//! FIXパーツ初期化関数テーブル
const static GMF_FIX_PART_INIT_FUNC gm_fix_part_init_func_tbl[GME_FIX_PART_TYPE_MAX]	= {
	gmFixRingCountPartInit,
	gmFixScorePartInit,
	gmFixTimerPartInit,
	gmFixChallengePartInit,
#if _IPHONE
	gmFixVirtualPadPartInit,
#endif //_IPHONE
};

//! タイムアタックモード用パーツ初期化関数テーブル
const static GMF_FIX_PART_INIT_FUNC gm_fix_ta_part_init_func_tbl[GME_FIX_PART_TYPE_MAX]	= {
	gmFixRingCountPartInit,
	NULL,
	gmFixTimerPartInit,
	NULL,
#if _IPHONE
	gmFixVirtualPadPartInit,
#endif //_IPHONE
};

//! タイムアタックモード Stage2-2専用 パーツ初期化関数テーブル
const static GMF_FIX_PART_INIT_FUNC gm_fix_ta_s22_part_init_func_tbl[GME_FIX_PART_TYPE_MAX]	= {
	gmFixRingCountPartInit,
	gmFixScorePartInit,
	gmFixTimerPartInit,
	NULL,
#if _IPHONE
	gmFixVirtualPadPartInit,
#endif //_IPHONE
};

//! スペステ用パーツ初期化関数テーブル
const static GMF_FIX_PART_INIT_FUNC gm_fix_ss_part_init_func_tbl[GME_FIX_PART_TYPE_MAX]	= {
	NULL,
	NULL,
	gmFixTimerSSPartInit,
	NULL,
#if _IPHONE
	gmFixVirtualPadPartInit,
#endif //_IPHONE
};

//############ アクションIDテーブル ###########################################
//! リングカウントパーツ 全アクションIDテーブル
const static Sint32 gm_fix_ringcount_act_id_tbl[GME_FIX_RING_SUBPART_MAX]	= {
	IDA_G_FIX_ACT_ICON_RING,
	IDA_G_FIX_ACT_NUM_A_100,
	IDA_G_FIX_ACT_NUM_A_10,
	IDA_G_FIX_ACT_NUM_A_1,
};


//! スコアパーツ 全アクションIDテーブル
const static Sint32 gm_fix_score_act_id_tbl[GME_FIX_SCORE_SUBPART_MAX]	= {
	IDA_G_FIX_ACT_TAB_SCORE,
	IDA_G_FIX_ACT_NUM_B_1,
	IDA_G_FIX_ACT_NUM_B_2,
	IDA_G_FIX_ACT_NUM_B_3,
	IDA_G_FIX_ACT_NUM_B_4,
	IDA_G_FIX_ACT_NUM_B_5,
	IDA_G_FIX_ACT_NUM_B_6,
	IDA_G_FIX_ACT_NUM_B_7,
	IDA_G_FIX_ACT_NUM_B_8,
	IDA_G_FIX_ACT_NUM_B_9,
};

//! Zone22用スコアパーツ 全アクションIDテーブル
#if !_IPHONE
const static Sint32 gm_fix_score_stage22_act_id_tbl[GME_FIX_SCORE_SUBPART_MAX]	= {
	IDA_G_FIX_ACT_ICON_SCORE,
	IDA_G_FIX_ACT_TA_NUM_D_9,
	IDA_G_FIX_ACT_TA_NUM_D_8,
	IDA_G_FIX_ACT_TA_NUM_D_7,
	IDA_G_FIX_ACT_TA_NUM_D_6,
	IDA_G_FIX_ACT_TA_NUM_D_5,
	IDA_G_FIX_ACT_TA_NUM_D_4,
	IDA_G_FIX_ACT_TA_NUM_D_3,
	IDA_G_FIX_ACT_TA_NUM_D_2,
	IDA_G_FIX_ACT_TA_NUM_D_1,
};
#else // !_IPHONE
const static Sint32 gm_fix_score_stage22_act_id_tbl[GME_FIX_PLAN_MAX][GME_FIX_SCORE_SUBPART_MAX]	= {
	//GME_FIX_PLAN_TILT
	{-1,
	IDA_G_FIX_ACT_TA_NUM_D_9,		IDA_G_FIX_ACT_TA_NUM_D_8,		IDA_G_FIX_ACT_TA_NUM_D_7,
	IDA_G_FIX_ACT_TA_NUM_D_6,		IDA_G_FIX_ACT_TA_NUM_D_5,		IDA_G_FIX_ACT_TA_NUM_D_4,
	IDA_G_FIX_ACT_TA_NUM_D_3,		IDA_G_FIX_ACT_TA_NUM_D_2,		IDA_G_FIX_ACT_TA_NUM_D_1},
	//GME_FIX_PLAN_FLICK
	{-1,
	IDA_G_FIX_ACT_DA_TA_NUM_D_9,	IDA_G_FIX_ACT_DA_TA_NUM_D_8,	IDA_G_FIX_ACT_DA_TA_NUM_D_7,
	IDA_G_FIX_ACT_DA_TA_NUM_D_6,	IDA_G_FIX_ACT_DA_TA_NUM_D_5,	IDA_G_FIX_ACT_DA_TA_NUM_D_4,
	IDA_G_FIX_ACT_DA_TA_NUM_D_3,	IDA_G_FIX_ACT_DA_TA_NUM_D_2,	IDA_G_FIX_ACT_DA_TA_NUM_D_1},
	//GME_FIX_PLAN_VIRTUAL_PAD
	{-1,
	IDA_G_FIX_ACT_DA_TA_NUM_D_9,	IDA_G_FIX_ACT_DA_TA_NUM_D_8,	IDA_G_FIX_ACT_DA_TA_NUM_D_7,
	IDA_G_FIX_ACT_DA_TA_NUM_D_6,	IDA_G_FIX_ACT_DA_TA_NUM_D_5,	IDA_G_FIX_ACT_DA_TA_NUM_D_4,
	IDA_G_FIX_ACT_DA_TA_NUM_D_3,	IDA_G_FIX_ACT_DA_TA_NUM_D_2,	IDA_G_FIX_ACT_DA_TA_NUM_D_1},
};
#endif // !_IPHONE

//! タイマパーツ 全アクションIDテーブル
const static Sint32 gm_fix_timer_act_id_tbl[GME_FIX_TIMER_SUBPART_MAX]	= {
	IDA_G_FIX_ACT_ICON_TIME,
	IDA_G_FIX_ACT_NUM_C_10000,
	IDA_G_FIX_ACT_NUM_C_A,
	IDA_G_FIX_ACT_NUM_C_1000,
	IDA_G_FIX_ACT_NUM_C_100,
	IDA_G_FIX_ACT_NUM_C_B,
	IDA_G_FIX_ACT_NUM_C_10,
	IDA_G_FIX_ACT_NUM_C_1,
};

//! タイムアタック用タイマパーツ 全アクションIDテーブル
const static Sint32 gm_fix_timer_timeattack_act_id_tbl[GME_FIX_TIMER_SUBPART_MAX]	= {
	IDA_G_FIX_ACT_ICON_TIMEATTACK,
	IDA_G_FIX_ACT_TA_NUM_C_10000,
	IDA_G_FIX_ACT_TA_NUM_C_A,
	IDA_G_FIX_ACT_TA_NUM_C_1000,
	IDA_G_FIX_ACT_TA_NUM_C_100,
	IDA_G_FIX_ACT_TA_NUM_C_B,
	IDA_G_FIX_ACT_TA_NUM_C_10,
	IDA_G_FIX_ACT_TA_NUM_C_1,
};

//! SS用タイマパーツ 全アクションIDテーブル
const static Sint32 gm_fix_timer_ss_act_id_tbl[GSD_LANGUAGE_NUM][GME_FIX_TIMER_SUBPART_MAX]	= {
	// JP
	{
		IDA_SPST_JP_ACT_TEX_TIME,	// SPST_JPのAMA
		IDA_SPST_ACT_1,
		IDA_SPST_ACT_2,
		IDA_SPST_ACT_3,
		IDA_SPST_ACT_4,
		IDA_SPST_ACT_5,
		IDA_SPST_ACT_6,
		IDA_SPST_ACT_7,
	},
	// US
	{
		IDA_SPST_US_ACT_TEX_TIME,	// SPST_USのAMA
		IDA_SPST_ACT_1,
		IDA_SPST_ACT_2,
		IDA_SPST_ACT_3,
		IDA_SPST_ACT_4,
		IDA_SPST_ACT_5,
		IDA_SPST_ACT_6,
		IDA_SPST_ACT_7,
	},
	// FR
	{
		IDA_SPST_FR_ACT_TEX_TIME,	// SPST_FRのAMA
		IDA_SPST_ACT_1,
		IDA_SPST_ACT_2,
		IDA_SPST_ACT_3,
		IDA_SPST_ACT_4,
		IDA_SPST_ACT_5,
		IDA_SPST_ACT_6,
		IDA_SPST_ACT_7,
	},
	// IT
	{
		IDA_SPST_IT_ACT_TEX_TIME,	// SPST_ITのAMA
		IDA_SPST_ACT_1,
		IDA_SPST_ACT_2,
		IDA_SPST_ACT_3,
		IDA_SPST_ACT_4,
		IDA_SPST_ACT_5,
		IDA_SPST_ACT_6,
		IDA_SPST_ACT_7,
	},
	// GE
	{
		IDA_SPST_GE_ACT_TEX_TIME,	// SPST_GEのAMA
		IDA_SPST_ACT_1,
		IDA_SPST_ACT_2,
		IDA_SPST_ACT_3,
		IDA_SPST_ACT_4,
		IDA_SPST_ACT_5,
		IDA_SPST_ACT_6,
		IDA_SPST_ACT_7,
	},
	// SP
	{
		IDA_SPST_SP_ACT_TEX_TIME,	// SPST_SPのAMA
		IDA_SPST_ACT_1,
		IDA_SPST_ACT_2,
		IDA_SPST_ACT_3,
		IDA_SPST_ACT_4,
		IDA_SPST_ACT_5,
		IDA_SPST_ACT_6,
		IDA_SPST_ACT_7,
	},
};

//! チャレンジ数パーツ 全アクションIDテーブル
#if !_IPHONE
const static Sint32 gm_fix_challenge_act_id_tbl[GME_FIX_CHALLENGE_SUBPART_MAX]	= {
	IDA_G_FIX_ACT_ICON_SONIC,
	IDA_G_FIX_ACT_TEX_SONIC,
	IDA_G_FIX_ACT_NUM_D_100,
	IDA_G_FIX_ACT_NUM_D_10,
	IDA_G_FIX_ACT_NUM_D_1,
};
#else //!_IPHONE
const static Sint32 gm_fix_challenge_act_id_tbl[GME_FIX_PLAN_MAX][GME_FIX_CHALLENGE_SUBPART_MAX]	= {
	//GME_FIX_PLAN_TILT
	{IDA_G_FIX_ACT_ICON_SONIC,		IDA_G_FIX_ACT_TEX_SONIC,
	IDA_G_FIX_ACT_NUM_D_100,		IDA_G_FIX_ACT_NUM_D_10,		IDA_G_FIX_ACT_NUM_D_1},
	//GME_FIX_PLAN_FLICK
	{IDA_G_FIX_ACT_DA_ICON_SONIC,	IDA_G_FIX_ACT_DA_TEX_SONIC,
	IDA_G_FIX_ACT_DA_NUM_D_100,		IDA_G_FIX_ACT_DA_NUM_D_10,	IDA_G_FIX_ACT_DA_NUM_D_1},
	//GME_FIX_PLAN_VIRTUAL_PAD
	{IDA_G_FIX_ACT_DA_ICON_SONIC,	IDA_G_FIX_ACT_DA_TEX_SONIC,
	IDA_G_FIX_ACT_DA_NUM_D_100,		IDA_G_FIX_ACT_DA_NUM_D_10,	IDA_G_FIX_ACT_DA_NUM_D_1},
};
#endif //!_IPHONE

#if _IPHONE
//! バーチャルパッドパーツ 全アクションIDテーブル
const static Sint32 gm_fix_virtual_pad_act_id_tbl[GME_FIX_PLAN_MAX][GME_FIX_VIRTUAL_PAD_SUBPART_MAX]	= {
	//GME_FIX_PLAN_TILT
	{IDA_G_FIX_ACT_ICON_SUPER01,	IDA_G_FIX_ACT_START,	-1,					-1},
	//GME_FIX_PLAN_FLICK
	{IDA_G_FIX_ACT_DA_ICON_SUPER01,	IDA_G_FIX_ACT_START,	IDA_G_FIX_ACT_JUMP,	-1},
	//GME_FIX_PLAN_VIRTUAL_PAD
	{IDA_G_FIX_ACT_DA_ICON_SUPER01,	IDA_G_FIX_ACT_START,	IDA_G_FIX_ACT_JUMP,	IDA_G_FIX_ACT_CROSS},
};
#endif //_IPHONE


//############ パーツ共通 #####################################################
//! 数字タイプ(0-9)に対応するフレーム数テーブル（リングカウント以外）
const static Float gm_fix_part_common_digit_type_frame_tbl[10]	= {
	0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f,
};


//############ リングカウントパーツ関連 #######################################
//! リングカウントパーツ 数字 サブパーツインデックステーブル
const static GME_FIX_RING_SUBPART gm_fix_part_ring_count_digit_subpart_idx_tbl[GMD_FIX_PART_RINGCOUNT_DIGIT_NUM]	= {
	GME_FIX_RING_SUBPART_DIGIT_1,
	GME_FIX_RING_SUBPART_DIGIT_10,
	GME_FIX_RING_SUBPART_DIGIT_100,
};

//! 数字タイプ(0-9,赤0)に対応するフレーム数テーブル
const static Float gm_fix_part_ring_count_digit_type_frame_tbl[11]	= {
	0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f
};

//############ スコアパーツ関連 ###############################################
//! スコアパーツ 数字サブパーツインデックステーブル
const static GME_FIX_SCORE_SUBPART gm_fix_part_score_digit_subpart_idx_tbl[GMD_FIX_PART_SCORE_DIGIT_NUM]	= {
	GME_FIX_SCORE_SUBPART_NUM_9,
	GME_FIX_SCORE_SUBPART_NUM_8,
	GME_FIX_SCORE_SUBPART_NUM_7,
	GME_FIX_SCORE_SUBPART_NUM_6,
	GME_FIX_SCORE_SUBPART_NUM_5,
	GME_FIX_SCORE_SUBPART_NUM_4,
	GME_FIX_SCORE_SUBPART_NUM_3,
	GME_FIX_SCORE_SUBPART_NUM_2,
	GME_FIX_SCORE_SUBPART_NUM_1,
};

//############ タイマパーツ関連 ###############################################
//! タイマパーツ 数字 サブパーツインデックステーブル
const static GME_FIX_TIMER_SUBPART gm_fix_part_timer_digit_subpart_idx_tbl[GMD_FIX_PART_TIMER_ALL_DIGIT_NUM]	= {
	GME_FIX_TIMER_SUBPART_NUM_1,
	GME_FIX_TIMER_SUBPART_NUM_10,
	GME_FIX_TIMER_SUBPART_NUM_100,
	GME_FIX_TIMER_SUBPART_NUM_1000,
	GME_FIX_TIMER_SUBPART_NUM_10000,
};

//! タイマパーツ 装飾文字（プライムなど） サブパーツインデックステーブル
const static GME_FIX_TIMER_SUBPART gm_fix_part_timer_deco_char_subpart_idx_tbl[GMD_FIX_PART_TIMER_DECO_CHAR_NUM]	= {
	GME_FIX_TIMER_SUBPART_NUM_AP1,
	GME_FIX_TIMER_SUBPART_NUM_AP2,
};

//############ チャレンジ数パーツ関連 #######################################
//! チャレンジ数パーツ 数字サブパーツインデックステーブル
const static GME_FIX_CHALLENGE_SUBPART gm_fix_part_challenge_digit_subpart_idx_tbl[GMD_FIX_PART_CHALLENGE_DIGIT_NUM]	= {
	GME_FIX_CHALLENGE_SUBPART_NUM_1,
	GME_FIX_CHALLENGE_SUBPART_NUM_10,
	GME_FIX_CHALLENGE_SUBPART_NUM_100,
};


/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmFixBuildDataInit
/*!
  テクスチャビルド開始
 */
// =======================================================================
void GmFixBuildDataInit(void)
{
	amZeroMemory(gm_fix_textures, sizeof(AOS_TEXTURE) * GME_FIX_TEX_IDX_MAX);
	
	for (Sint32 i = 0; i < GME_FIX_TEX_IDX_MAX; ++i) {
		MTM_ASSERT(gm_fix_texamb_list[i] == NULL);
		
		gm_fix_texamb_list[i]	=
			ObjDataLoadAmbIndex(NULL,
								gm_fix_tex_amb_idx_tbl[GsEnvGetLanguage()][i],
								GmGameDatGetCockpitData());
		AoTexBuild(&gm_fix_textures[i], gm_fix_texamb_list[i]);
		AoTexLoad(&gm_fix_textures[i]);
	}
}

// =======================================================================
// GmFixBuildDataLoop
/*!
  テクスチャビルド完了待ち
  
  @retval TRUE	ビルド完了
  @retval FALSE	ビルド中
 */
// =======================================================================
BOOL GmFixBuildDataLoop(void)
{
	BOOL	b_loaded	= TRUE;
	
	for (Sint32 i = 0; i < GME_FIX_TEX_IDX_MAX; ++i) {
		if (!AoTexIsLoaded(&gm_fix_textures[i])) {
			b_loaded	= FALSE;
		}
	}
	
	return b_loaded;
}

// =======================================================================
// GmFixFlushDataInit
/*!
  テクスチャフラッシュ開始
 */
// =======================================================================
void GmFixFlushDataInit(void)
{
	// テクスチャ解放開始
	for (Sint32 i = 0; i < GME_FIX_TEX_IDX_MAX; ++i) {
		AoTexRelease(&gm_fix_textures[i]);
	}
}

// =======================================================================
// GmFixFlushDataLoop
/*!
  テクスチャフラッシュ完了待ち
  
  @retval TRUE	フラッシュ完了
  @retval FALSE	フラッシュ中
 */
// =======================================================================
BOOL GmFixFlushDataLoop(void)
{
	BOOL	b_flushed	= TRUE;
	
	// 解放完了待ち
	for (Sint32 i = 0; i < GME_FIX_TEX_IDX_MAX; ++i) {
		
		if (gm_fix_texamb_list[i] == NULL) {
			continue;
		}
		
		if (!AoTexIsReleased(&gm_fix_textures[i])) {
			b_flushed	= FALSE;
		}
		else {
			gm_fix_texamb_list[i]	= NULL;
			amZeroMemory(&gm_fix_textures[i], sizeof(AOS_TEXTURE));
		}
	}
	
	return b_flushed;
}

static float mpp_ss_button_timer = 0.f; //super sonic button

// =======================================================================
// GmFixInit
/*!
  FIX初期化
 */
// =======================================================================
void GmFixInit(void)
{
	mpp_ss_button_timer = 0.f;	
	GMS_FIX_MGR_WORK	*mgr_work;
	const GMF_FIX_PART_INIT_FUNC	*init_func_tbl;
	
	// タスク生成
	gm_fix_tcb	= MTM_TASK_MAKE_TCB(gmFixProcMain,
									gmFixDest,
									0,//flag
									GMD_TASK_PAUSELEVEL_DEF,
									GMD_TASK_PRIO_FIX,
									GMD_TASK_GROUP_FIX,
									sizeof(GMS_FIX_MGR_WORK),
									"GM_FIX_MGR");
	
	// ワーク初期化
	mgr_work	= (GMS_FIX_MGR_WORK*)mtTaskGetTcbWork(gm_fix_tcb);
	amZeroMemory(mgr_work, sizeof(GMS_FIX_MGR_WORK));
	
	if (gmFixIsSpecialStage()) {
		// スペステ用初期化
		init_func_tbl	= &gm_fix_ss_part_init_func_tbl[0];
		
		// タイマはカウントダウン
		mgr_work->flag	|= GMD_FIX_MGR_FLAG_TIMER_COUNTDOWN;
	}
	else {
		// 通常ステージ用初期化
		if (gmFixIsTimeAttack()) {
			// タイムアタック用初期化
			if (gmFixIsStage22()) {
				// Zone2-Act2のみ専用の構成
				init_func_tbl	= &gm_fix_ta_s22_part_init_func_tbl[0];
			}
			else {
				init_func_tbl	= &gm_fix_ta_part_init_func_tbl[0];
			}
		}
		else {
			init_func_tbl	= &gm_fix_part_init_func_tbl[0];
		}
	}
	
	// パーツ生成
	for (Sint32 i = 0; i < GME_FIX_PART_TYPE_MAX; ++i) {
		if (init_func_tbl[i]) {
			init_func_tbl[i](mgr_work);
		}
	}
	
	// 処理関数設定
	mgr_work->proc_update	= NULL;	// 今のところ特になし
}

// =======================================================================
// GmFixExit
/*!
  FIX終了
 */
// =======================================================================
void GmFixExit(void)
{
	if (gm_fix_tcb) {
		mtTaskClearTcb(gm_fix_tcb);
		gm_fix_tcb	= NULL;
	}
	
	mpp_ss_button_timer = 0.f;	
}

// =======================================================================
// GmFixSetDisp
/*!
  表示設定
  
  @param enable		[in]	有効フラグ（TRUE:表示, FALSE:非表示）
  
  @note
  FALSE設定することで表示関連処理を無効にします。
 */
// =======================================================================
void GmFixSetDisp(BOOL enable)
{
#if !_IPHONE
	if (gm_fix_tcb) {
		GMS_FIX_MGR_WORK *mgr_work	= (GMS_FIX_MGR_WORK*)mtTaskGetTcbWork(gm_fix_tcb);
		
		if (mgr_work) {
			if (enable) {
				mgr_work->flag	&= ‾GMD_FIX_MGR_FLAG_HIDE;
			}
			else {
				mgr_work->flag	|= GMD_FIX_MGR_FLAG_HIDE;
			}
		}
	}
#else //!_IPHONE
	GmFixSetDispEx(enable, enable, enable, enable, enable);
#endif //!_IPHONE
}

#if _IPHONE
// =======================================================================
// GmFixSetDispEx
/*!
  拡張表示設定
  
  @param enable			[in]	有効フラグ（TRUE:表示, FALSE:非表示）
  @param enable_ss		[in]	有効フラグ（TRUE:表示, FALSE:非表示）バーチャルパッド(スーバーソニック)
  @param enable_pause	[in]	有効フラグ（TRUE:表示, FALSE:非表示）バーチャルパッド(ポーズ)
  @param enable_action	[in]	有効フラグ（TRUE:表示, FALSE:非表示）バーチャルパッド(アクション)
  @param enable_move	[in]	有効フラグ（TRUE:表示, FALSE:非表示）バーチャルパッド(十時キー)

  @note
  FALSE設定することで表示関連処理を無効にします。
 */
// =======================================================================
void GmFixSetDispEx(BOOL enable, BOOL enable_ss, BOOL enable_pause
						, BOOL enable_action, BOOL enable_move)
{
	if (gm_fix_tcb) {
		GMS_FIX_MGR_WORK *mgr_work	= (GMS_FIX_MGR_WORK*)mtTaskGetTcbWork(gm_fix_tcb);
		if (mgr_work) {
			Sint32 flag_true = ((enable)? GMD_FIX_MGR_FLAG_HIDE: 0)
							| ((enable_ss)? GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_SUPER_SONIC: 0)
							| ((enable_pause)? GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_PAUSE: 0)
							| ((enable_action)? GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_ACTION: 0)
							| ((enable_move)? GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_MOVE_PAD: 0);
			Sint32 flag_false = ((enable)? 0: GMD_FIX_MGR_FLAG_HIDE)
							| ((enable_ss)? 0: GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_SUPER_SONIC)
							| ((enable_pause)? 0: GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_PAUSE)
							| ((enable_action)? 0: GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_ACTION)
							| ((enable_move)? 0: GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_MOVE_PAD);
		
			mgr_work->flag	&= ~flag_true;
			mgr_work->flag	|= flag_false;
		}
	}
}
#endif //_IPHONE

// =======================================================================
// GmFixIsDisp
/*!
  表示中判定
  
  @retval TRUE	表示中
  @retval FALSE	非表示中
 */
// =======================================================================
BOOL GmFixIsDisp(void)
{
	if (gm_fix_tcb) {
		GMS_FIX_MGR_WORK *mgr_work	= (GMS_FIX_MGR_WORK*)mtTaskGetTcbWork(gm_fix_tcb);
		
		if (mgr_work) {
			if (!(mgr_work->flag & GMD_FIX_MGR_FLAG_HIDE)) {
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

// =======================================================================
// GmFixRequestTimerFlash
/*!
  タイマフラッシュ動作開始リクエスト
  
  @note
  FIXが起動していないときは何もしません。
 */
// =======================================================================
void GmFixRequestTimerFlash(void)
{
	if (gm_fix_tcb) {
		GMS_FIX_MGR_WORK *mgr_work	= (GMS_FIX_MGR_WORK*)mtTaskGetTcbWork(gm_fix_tcb);
		
		if (mgr_work) {
			mgr_work->req_flag	|= GMD_FIX_MGR_REQUEST_FLAG_TIMER_FLASH_ACT;
		}
	}
}


/*------ Static Functions ----------------------------------------------*/

// ############################################################################
// 共通
// ############################################################################
// =======================================================================
// gmFixSubpartOutFunc
/*!
  サブパーツ用描画処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmFixSubpartOutFunc(OBS_OBJECT_WORK *obj_work)
{
	if (gm_fix_tcb == NULL) {
		return;
	}
	
	GMS_FIX_MGR_WORK *mgr_work	= (GMS_FIX_MGR_WORK*)mtTaskGetTcbWork(gm_fix_tcb);
	
	MTM_ASSERT(mgr_work);
	
	if (!(mgr_work->flag & GMD_FIX_MGR_FLAG_HIDE)) {
		ObjDrawActionSummary(obj_work);
	}
}

// =======================================================================
// gmFixSetFrameStatic
/*!
  フレーム設定（モーションなし）
  
  @param obj_work	[io]	オブジェクトワーク
  @param frame		[in]	設定するフレーム
 
  @note
  フレーム設定＋モーション速度停止を行います。
 */
// =======================================================================
void gmFixSetFrameStatic(OBS_OBJECT_WORK *obj_work, Float frame)
{
	obj_work->obj_2d->frame	= frame;
	obj_work->obj_2d->speed	= 0;
}


// =======================================================================
// gmFixUpdatePart
/*!
  パーツ 更新
 
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixUpdatePart(GMS_FIX_PART_WORK *part_work)
{
	MTM_ASSERT(part_work);
	MTM_ASSERT(part_work->parent_mgr);
	
	// 死亡チェック
	if (!(part_work->flag & GMD_FIX_PART_FLAG_ACTIVE)) {
		return;
	}
	
	// 消去チェック
	if (gmFixCheckClear(part_work)) {
		gmFixUnregisterPart(part_work); // 関数内で GMD_FIX_PART_FLAG_ACTIVE のオフ設定も行われます
		return;
	}
	
	// 更新処理
	if (part_work->proc_update) {
		part_work->proc_update(part_work);
	}
	
	// 表示関連処理
	if (part_work->proc_disp) {
		if (!(part_work->flag & GMD_FIX_PART_FLAG_NODISP)) {
			part_work->proc_disp(part_work);
		}
	}
}

// =======================================================================
// gmFixCheckClear
/*!
  パーツが消去対象条件を満たしているか判定
  
  @param part_work	[in]	パーツワーク
 
  @retval TRUE	消去条件満たしている
  @retval FALSE 消去条件満たしていない
 */
// =======================================================================
BOOL gmFixCheckClear(const GMS_FIX_PART_WORK *part_work)
{
	if (gm_fix_tcb == NULL) {
		return TRUE;
	}
	
	if (part_work->parent_mgr == NULL) {
		return TRUE;
	}
	
	if (part_work->parent_mgr->flag & GMD_FIX_MGR_FLAG_CLEAR) {
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmFixIsSpecialStage
/*!
  スペステ判定
  
  @retval TRUE	スペステ
  @retval FALSE	スペステではない
 
  @note
  現在のステージがスペシャルステージか判定します。
 */
// =======================================================================
inline BOOL gmFixIsSpecialStage(void)
{
	switch (GsGetMainSysInfo()->stage_id) {
	case GSD_MAIN_STAGE_ID_SS1:
	case GSD_MAIN_STAGE_ID_SS2:
	case GSD_MAIN_STAGE_ID_SS3:
	case GSD_MAIN_STAGE_ID_SS4:
	case GSD_MAIN_STAGE_ID_SS5:
	case GSD_MAIN_STAGE_ID_SS6:
	case GSD_MAIN_STAGE_ID_SS7:
		return TRUE;
		
	default:
		return FALSE;
	}
}

// =======================================================================
// gmFixIsTimeAttack
/*!
  タイムアタック判定
  
  @retval TRUE	タイムアタックモード
  @retval FALSE	タイムアタックモードではない
 
  @note
  ゲームモードがタイムアタックかどうか判定します。
 */
// =======================================================================
inline BOOL gmFixIsTimeAttack(void)
{
	if (GsGetMainSysInfo()->game_mode == GSD_GAME_MODE_TIME_ATTACK) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmFixIsStage22
/*!
  ステージ2-2判定
  
  @retval TRUE	ステージ2-2
  @retval FALSE	ステージ2-2ではない
 
  @note
  現在のステージがZone2-Act2か判定します。
 */
// =======================================================================
inline BOOL gmFixIsStage22(void)
{
	switch (GsGetMainSysInfo()->stage_id) {
	case GSD_MAIN_STAGE_ID_2_2:
		return TRUE;
		
	default:
		return FALSE;
	}
}

// =======================================================================
// gmFixIsStageTruck
/*!
  トロッコ判定
  
  @retval TRUE	トロッコステージ
  @retval FALSE	トロッコステージではない
 
  @note
  現在のステージがトロッコ(Zone3-Act2)か判定します。
 */
// =======================================================================
inline BOOL gmFixIsStageTruck(void)
{
	switch (GsGetMainSysInfo()->stage_id) {
	case GSD_MAIN_STAGE_ID_3_2:
		return TRUE;
		
	default:
		return FALSE;
	}
}

#if _IPHONE
// =======================================================================
// gmFixGetPlan
/*!
  FIX表示プランの取得
  
  @return FIX表示
  */
// =======================================================================
inline GME_FIX_PLAN gmFixGetPlan(void)
{
	GME_FIX_PLAN result = GME_FIX_PLAN_TILT;

	GSS_MAIN_SYS_INFO *main_sys_info = GsGetMainSysInfo();
	if (gmFixIsStageTruck()) {
		//トロッコステージなら
		if (GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK & main_sys_info->game_flag) {
			result = GME_FIX_PLAN_FLICK;
		}
	} else if (gmFixIsSpecialStage()) {
		//スペステなら
		if (GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK & main_sys_info->game_flag) {
			result = GME_FIX_PLAN_FLICK;
		}
	} else {
		//通常ステージ
		if (GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC & main_sys_info->game_flag) {
			result = GME_FIX_PLAN_VIRTUAL_PAD;
		}
	}
	
	return result;
}
#endif //_IPHONE

// =======================================================================
// gmFixGetRingNum
/*!
  リング数取得
  
  @return 現在のリング数
  */
// =======================================================================
inline Sint16 gmFixGetRingNum(void)
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
// gmFixGetScore
/*!
  スコア取得
  
  @return 現在のスコア
 */
// =======================================================================
inline Uint32 gmFixGetScore(void)
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
// gmFixGetGameTime
/*!
  ゲームタイマ値取得
  
  @return 現在のゲームタイマ値
 */
// =======================================================================
inline Uint32 gmFixGetGameTime(void)
{
	if (g_gm_main_system.game_time >= GMD_MAIN_TIME_MAX) {
		return GMD_MAIN_TIME_MAX;
	}
	return g_gm_main_system.game_time;
}

// =======================================================================
// gmFixGetChallengeNum
/*!
  チャレンジ数取得
  
  @return 現在のチャレンジ数
  
  @note
  ゲームオーバーになるまでに後何回死亡できるかを返します。
  （0ならば、次死んだらゲームオーバー）
 */
// =======================================================================
inline Uint32 gmFixGetChallengeNum(void)
{
	Uint32 challenge_num	= 0;
	
	// 0を下回らないようにする
	if (g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] > 0) {
		// 適切な値を取得
		challenge_num	= g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] - 1;
	}
	
	// クリッピング
	challenge_num	= MTM_MATH_CLIP(challenge_num, 0, GSD_MAINSYS_PLAYER_REST_MAX - 1);
	
	return challenge_num;
}

// =======================================================================
// gmFixInitBlink
/*!
  点滅処理 初期化
  
  @param part_work	[io]	パーツワーク
  @param on_time	[in]	点滅の「明」時間
  @parma off_time	[in]	点滅の「暗」時間
  
  @note
  gmFixUpdateBlink()で点滅を更新してください。
 */
// =======================================================================
void gmFixInitBlink(GMS_FIX_PART_WORK *part_work, Uint32 on_time, Uint32 off_time)
{
	part_work->blink_timer	= on_time + off_time;
	part_work->blink_on_time	= on_time;
	part_work->blink_off_time	= off_time;
}

// =======================================================================
// gmFixUpdateBlink
/*!
  点滅処理 更新
  
  @param part_work	[io]	パーツワーク
  
  @retval	TRUE	「明」状態
  @retval	FALSE	「暗」状態
  
  @note
  表示・非表示のフラグ設定などは行いません。
  TRUEを返す間は表示状態にし、FALSEを返す間は非表示状態に設定してください。
  最初は「明」状態から開始します。
 */
// =======================================================================
BOOL gmFixUpdateBlink(GMS_FIX_PART_WORK *part_work)
{
	BOOL	result	= FALSE;
	
	// タイマ更新
	if (part_work->blink_timer) {
		part_work->blink_timer--;
	}
	
	// 状態判定
	if (part_work->blink_timer >= part_work->blink_off_time) {
		// 「明」状態
		result	= TRUE;
	}
	else {
		// 「暗」状態
		result	= FALSE;
	}
	
	if (part_work->blink_timer == 0) {
		// 1ループ終了・タイマ再設定
		part_work->blink_timer	= part_work->blink_on_time + part_work->blink_off_time;
	}
	
	return result;
}

// ############################################################################
// 管理
// ############################################################################
// =======================================================================
// gmFixDest
/*!
  管理タスク 終了処理関数
 */
// =======================================================================
void gmFixDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
}

// =======================================================================
// gmFixProcMain
/*!
  管理タスク メイン処理関数
 */
// =======================================================================
void gmFixProcMain(MTS_TASK_TCB *tcb)
{
	GMS_FIX_MGR_WORK	*mgr_work	= (GMS_FIX_MGR_WORK*)mtTaskGetTcbWork(tcb);
	
	// 管理更新処理
	if (mgr_work->proc_update) {
		mgr_work->proc_update(mgr_work);
	}
	
	// 全パーツ更新
	for (Sint32 i = 0; i < GME_FIX_PART_TYPE_MAX; ++i) {
		if (mgr_work->part_work[i]) {
			gmFixUpdatePart(mgr_work->part_work[i]);
		}
	}
}

// =======================================================================
// gmFixRegisterPart
/*!
  管理ワークにパーツを登録
  
  @param mgr_work	[io]	管理ワーク
  @param part_work	[io]	パーツワーク
  @param part_type	[in]	パーツタイプ
 */
// =======================================================================
void gmFixRegisterPart(GMS_FIX_MGR_WORK *mgr_work, GMS_FIX_PART_WORK *part_work, GME_FIX_PART_TYPE part_type)
{
	MTM_ASSERT(mgr_work);
	MTM_ASSERT(part_work);
	
	MTM_ASSERT(part_work->parent_mgr == NULL);
	
	// パーツワーク設定
	mgr_work->part_work[part_type]	= part_work;
	
	// パーツタイプ設定
	part_work->part_type	= part_type;
	
	// 親設定
	part_work->parent_mgr	= mgr_work;
	
	// 有効化
	part_work->flag	|= GMD_FIX_PART_FLAG_ACTIVE;
}

// =======================================================================
// gmFixUnregisterPart
/*!
  管理ワークからパーツを登録解除
  
  @param mgr_work	[io]	管理ワーク
  @param part_work	[io]	パーツワーク
  
  @note
  パーツの所属管理ワークが設定されていない場合は
  管理ワークからの参照削除は行われません。
 */
// =======================================================================
void gmFixUnregisterPart(GMS_FIX_PART_WORK *part_work)
{
	GMS_FIX_MGR_WORK *mgr_work	= part_work->parent_mgr;
	
	if (mgr_work) {
		// パーツワーク設定解除
		mgr_work->part_work[part_work->part_type]	= NULL;
	}
	
	// パーツタイプクリア
	part_work->part_type	= GME_FIX_PART_TYPE_NONE;
	
	// 親解除
	part_work->parent_mgr	= NULL;
	
	// 無効化
	part_work->flag	&= ~GMD_FIX_PART_FLAG_ACTIVE;
}

// =======================================================================
// gmFixProcessRequest
/*!
  リクエスト処理
  
  @param mgr_work		[io]	管理ワーク
  @param req_flag_bit	[in]	リクエストフラグビット（1bit）
  
  @note
  req_flag_bitで複数のビットが立っていた場合はアサートに失敗します。
 */
// =======================================================================
BOOL gmFixProcessRequest(GMS_FIX_MGR_WORK *mgr_work, Uint32 req_flag_bit)
{
	MTM_ASSERT(1 == AkMathCountBitPopulation(req_flag_bit));
	
	if (mgr_work->req_flag & req_flag_bit) {
		mgr_work->req_flag	&= ~req_flag_bit;
		return TRUE;
	}
	else {
		return FALSE;
	}
}


// ############################################################################
// リングカウント
// ############################################################################
// =======================================================================
// gmFixRingCountPartInit
/*!
  FIX リングカウントパーツ初期化
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
void gmFixRingCountPartInit(GMS_FIX_MGR_WORK* mgr_work)
{
	OBS_OBJECT_WORK	*obj_work;
	GMS_COCKPIT_2D_WORK	*cpit_2d;
	GMS_FIX_PART_WORK	*part_work;
	
	part_work	= (GMS_FIX_PART_WORK*)&mgr_work->part_ringcount;
	
	// パーツ登録
	gmFixRegisterPart(mgr_work, part_work, GME_FIX_PART_TYPE_RINGCOUNT);
	
	// 更新処理関数設定
	part_work->proc_update	= gmFixRingCountPartProcUpdateMain;
	
	// 表示関連関数設定
	part_work->proc_disp	= gmFixRingCountPartProcDispMain;
	
	// サブパーツ生成
	for (Sint32 i = 0; i < GME_FIX_RING_SUBPART_MAX; ++i) {
		obj_work	= GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK),
											  NULL,
											  0,
											  "FIX_RING");
		
		cpit_2d	= (GMS_COCKPIT_2D_WORK*)obj_work;
		
		// 描画処理関数差し替え
		obj_work->ppOut	= gmFixSubpartOutFunc;
		
		ObjObjectAction2dAMALoadSetTexlist(obj_work,
										   &cpit_2d->obj_2d,
										   NULL,//data_work
										   NULL,//filename
										   gm_fix_ama_amb_idx_tbl[GsEnvGetLanguage()][GME_FIX_AMA_IDX_COMMON],
										   GmGameDatGetCockpitData(),
										   AoTexGetTexList(&gm_fix_textures[GME_FIX_TEX_IDX_FIX]),
										   (Uint32)gm_fix_ringcount_act_id_tbl[i],
										   FALSE);
		
		// フレームクリア
		gmFixSetFrameStatic(obj_work, 0);
		
		// ワークセット
		((GMS_FIX_PART_RINGCOUNT*)part_work)->sub_parts[i]	= (GMS_COCKPIT_2D_WORK*)obj_work;
	
	}
}


// =======================================================================
// gmFixRingCountPartProcUpdateMain
/*!
  リングカウントパーツ メイン更新処理関数
  
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixRingCountPartProcUpdateMain(GMS_FIX_PART_WORK *part_work)
{
	GMS_FIX_PART_RINGCOUNT	*part_ringcount	= (GMS_FIX_PART_RINGCOUNT*)part_work;
	
	// 数字情報更新
	gmFixRingCountPartUpdateDigitList(part_ringcount);
	
	// 点滅開始判定
	if (gmFixGetRingNum() == 0) {
		if (!(part_work->flag & GMD_FIX_PART_FLAG_BLINK)) {
			
			part_work->flag	|= GMD_FIX_PART_FLAG_BLINK;
			
			gmFixInitBlink(part_work,
						   GMD_FIX_PART_RINGCOUNT_DIGIT_BLINK_ON_TIME,
						   GMD_FIX_PART_RINGCOUNT_DIGIT_BLINK_OFF_TIME);
		}
	}
	else {
		part_work->flag	&= ~GMD_FIX_PART_FLAG_BLINK;
	}
}

// =======================================================================
// gmFixRingCountPartProcDispMain
/*!
  リングカウントパーツ メイン表示関連処理関数
  
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixRingCountPartProcDispMain(GMS_FIX_PART_WORK *part_work)
{
	GMS_FIX_PART_RINGCOUNT	*part_ringcount	= (GMS_FIX_PART_RINGCOUNT*)part_work;
	
	// 点滅処理
	if (part_work->flag & GMD_FIX_PART_FLAG_BLINK) {
		if (gmFixUpdateBlink(part_work)) {
			part_work->flag	&= ~GMD_FIX_PART_FLAG_BLINK_OFF;
		}
		else {
			part_work->flag	|= GMD_FIX_PART_FLAG_BLINK_OFF;
		}
	}
	else {
		part_work->flag	&= ~GMD_FIX_PART_FLAG_BLINK_OFF;
	}
	
	// 点滅の状態を反映
	if (part_work->flag & GMD_FIX_PART_FLAG_BLINK_OFF) {
		gmFixRingCountPartSetDispDigits(part_ringcount, FALSE);
	}
	else {
		gmFixRingCountPartSetDispDigits(part_ringcount, TRUE);
	}
	
	// 表示に反映
	gmFixRingCountPartUpdateActionDigitsType(part_ringcount);
}

// =======================================================================
// gmFixRingCountPartUpdateDigitList
/*!
  現在のリングカウント値を各桁の数字情報に反映する
  
  @param part_ringcount	[io]	リングカウントパーツ
  
  @note
  現在のリングカウント値を取得し、リングカウントパーツワークの各桁の数字情報に反映します。
  gmFixRingCountPartUpdateActionDigitsType() を呼ぶことで、更新内容が反映されます。
 */
// =======================================================================
void gmFixRingCountPartUpdateDigitList(GMS_FIX_PART_RINGCOUNT *part_ringcount)
{
	Sint32	ring_count	= (Sint32)gmFixGetRingNum();	// リング数取得
	
	// 各桁の数字に分解
	AkUtilNumValueToDigits(ring_count, part_ringcount->digit_list, 
						   GMD_FIX_PART_RINGCOUNT_DIGIT_NUM);
	
	if (ring_count == 0) {
		for (Sint32 i = 0; i < GMD_FIX_PART_RINGCOUNT_DIGIT_NUM; ++i) {
			part_ringcount->digit_list[i]	= 10;	// 赤数字0
		}
	}
}

// =======================================================================
// gmFixRingCountPartUpdateActionDigitsType
/*!
  リング数表示の各桁アクションの数字タイプを設定する
 
  @param part_ringcount	[io]	リングカウントパーツ
 */
// =======================================================================
void gmFixRingCountPartUpdateActionDigitsType(GMS_FIX_PART_RINGCOUNT *part_ringcount)
{
	// 各数字アクションのフレームをセット
	for (Sint32 i = 0; i < GMD_FIX_PART_RINGCOUNT_DIGIT_NUM; ++i) {
		GMS_COCKPIT_2D_WORK	*cpit_2d	= part_ringcount->sub_parts[gm_fix_part_ring_count_digit_subpart_idx_tbl[i]];
		gmFixSetFrameStatic((OBS_OBJECT_WORK*)cpit_2d,
							gm_fix_part_ring_count_digit_type_frame_tbl[part_ringcount->digit_list[i]]);
	}
}

// =======================================================================
// gmFixRingCountPartSetDispDigits
/*!
  リング数表示の数字サブパーツの表示・非表示を設定
  
  @param part_ringcount	[io]	リングカウントパーツ
  @param enable			[in]	有効フラグ
 */
// =======================================================================
void gmFixRingCountPartSetDispDigits(GMS_FIX_PART_RINGCOUNT *part_ringcount, BOOL enable)
{
	for (Sint32 i = 0; i < GME_FIX_RING_SUBPART_MAX; ++i) {
		if (i == GME_FIX_RING_SUBPART_ICON) {
			continue;
		}
		
		if (enable) {
			((OBS_OBJECT_WORK*)part_ringcount->sub_parts[i])->disp_flag	&= ~OBD_DISP_NODISP;
		}
		else {
			((OBS_OBJECT_WORK*)part_ringcount->sub_parts[i])->disp_flag	|= OBD_DISP_NODISP;
		}
	}
}

// ############################################################################
// スコア
// ############################################################################
// =======================================================================
// gmFixScorePartInit
/*!
  FIX スコアパーツ初期化
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
void gmFixScorePartInit(GMS_FIX_MGR_WORK* mgr_work)
{
	OBS_OBJECT_WORK	*obj_work;
	GMS_COCKPIT_2D_WORK	*cpit_2d;
	GMS_FIX_PART_WORK	*part_work;
	const static Sint32 *act_id_tbl;
	fx32	pos_ofst_x	= 0;
	
	
	part_work	= (GMS_FIX_PART_WORK*)&mgr_work->part_score;
	
	// パーツ登録
	gmFixRegisterPart(mgr_work, part_work, GME_FIX_PART_TYPE_SCORE);
	
	// 更新処理関数設定
	part_work->proc_update	= gmFixScorePartProcUpdateMain;
	
	// 表示関連処理関数設定
	part_work->proc_disp	= gmFixScorePartProcDispMain;
	
	// アクションIDテーブル取得
	if (gmFixIsStage22()) {
#if !_IPHONE
		act_id_tbl	= gm_fix_score_stage22_act_id_tbl;
#else //!_IPHONE
		act_id_tbl	= gm_fix_score_stage22_act_id_tbl[gmFixGetPlan()];
#endif //!_IPHONE
		
#if !_IPHONE
		// スコアアタックで画面比率が4:3の時は少し右にずらす
		if (!gmFixIsTimeAttack() && !_am_draw_video.wide_screen) {
			pos_ofst_x	= GMD_FIX_PART_SCORE_S22_TA_NARROWSCREEN_ADJUST_OFST_X;
		}
#endif //!_IPHONE
	}
	else {
		act_id_tbl	= gm_fix_score_act_id_tbl;
		pos_ofst_x	= 0;
	}
	
	// サブパーツ生成
	for (Sint32 i = 0; i < GME_FIX_SCORE_SUBPART_MAX; ++i) {
#if _IPHONE
		if (act_id_tbl[i] < 0) {
			((GMS_FIX_PART_SCORE*)part_work)->sub_parts[i] = NULL;
			continue;
		}
#endif //_IPHONE
		obj_work	= GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK),
											  NULL,
											  0,
											  "FIX_SCORE");
		
		cpit_2d	= (GMS_COCKPIT_2D_WORK*)obj_work;
		
		// 描画処理関数差し替え
		obj_work->ppOut	= gmFixSubpartOutFunc;
		
		ObjObjectAction2dAMALoadSetTexlist(obj_work,
										   &cpit_2d->obj_2d,
										   NULL,//data_work
										   NULL,//filename
										   gm_fix_ama_amb_idx_tbl[GsEnvGetLanguage()][GME_FIX_AMA_IDX_COMMON],
										   GmGameDatGetCockpitData(),
										   AoTexGetTexList(&gm_fix_textures[GME_FIX_TEX_IDX_FIX]),
										   (Uint32)act_id_tbl[i],
										   FALSE);
		
		// フレームクリア
		gmFixSetFrameStatic(obj_work, 0);
		
		// 水平位置調整
		obj_work->pos.x	+= pos_ofst_x;
		
		// ワークセット
		((GMS_FIX_PART_SCORE*)part_work)->sub_parts[i]	= (GMS_COCKPIT_2D_WORK*)obj_work;
	}
}

// =======================================================================
// gmFixScorePartProcUpdateMain
/*!
  スコアパーツ メイン更新処理関数
  
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixScorePartProcUpdateMain(GMS_FIX_PART_WORK *part_work)
{
	GMS_FIX_PART_SCORE	*part_score	= (GMS_FIX_PART_SCORE*)part_work;
	
	// 数字情報更新
	gmFixScorePartUpdateDigitList(part_score);
}

// =======================================================================
// gmFixScorePartProcDispMain
/*!
  スコアパーツ メイン表示関連処理関数
  
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixScorePartProcDispMain(GMS_FIX_PART_WORK *part_work)
{
	GMS_FIX_PART_SCORE	*part_score	= (GMS_FIX_PART_SCORE*)part_work;
	
	// 表示に反映
	gmFixScorePartUpdateActionDigitsType(part_score);
}

// =======================================================================
// gmFixScorePartUpdateDigitList
/*!
  現在のスコア値を各桁の数字情報に反映する
  
  @param part_score	[io]	スコアパーツ
  
  @note
  現在のスコア値を取得し、スコアパーツワークの各桁の数字情報に反映します。
  gmFixScorePartUpdateActionDigitsType() を呼ぶことで、更新内容が反映されます。
 */
// =======================================================================
void gmFixScorePartUpdateDigitList(GMS_FIX_PART_SCORE *part_score)
{
	Sint32	score	= (Sint32)gmFixGetScore();	// スコア取得
	
	// 各桁の数字に分解
	AkUtilNumValueToDigits(score, part_score->digit_list, 
						   GMD_FIX_PART_SCORE_DIGIT_NUM);
}

// =======================================================================
// gmFixScorePartUpdateActionDigitsType
/*!
  スコア表示の各桁アクションの数字タイプを設定する
 
  @param part_score	[io]	スコアパーツ
 */
// =======================================================================
void gmFixScorePartUpdateActionDigitsType(GMS_FIX_PART_SCORE *part_score)
{
	// 各数字アクションのフレームをセット
	for (Sint32 i = 0; i < GMD_FIX_PART_SCORE_DIGIT_NUM; ++i) {
		GMS_COCKPIT_2D_WORK	*cpit_2d	= part_score->sub_parts[gm_fix_part_score_digit_subpart_idx_tbl[i]];
		gmFixSetFrameStatic((OBS_OBJECT_WORK*)cpit_2d,
							gm_fix_part_common_digit_type_frame_tbl[part_score->digit_list[i]]);
	}
}



// ############################################################################
// タイマ
// ############################################################################
// =======================================================================
// gmFixTimerPartInit
/*!
  FIX タイマパーツ初期化
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
void gmFixTimerPartInit(GMS_FIX_MGR_WORK* mgr_work)
{
	OBS_OBJECT_WORK	*obj_work;
	GMS_COCKPIT_2D_WORK	*cpit_2d;
	GMS_FIX_PART_WORK	*part_work;
	GMS_FIX_PART_TIMER	*part_timer;
	const static Sint32 *act_id_tbl;
	
	
	part_work	= (GMS_FIX_PART_WORK*)&mgr_work->part_timer;
	part_timer	= &mgr_work->part_timer;
	
	// パーツ登録
	gmFixRegisterPart(mgr_work, part_work, GME_FIX_PART_TYPE_TIMER);
	
	// 更新処理関数設定
	part_work->proc_update	= gmFixTimerPartProcUpdateMain;
	
	// 表示関連処理関数設定
	part_work->proc_disp	= gmFixTimerPartProcDispMain;
	
	// アクションIDテーブル取得
	if (gmFixIsTimeAttack()) {
		act_id_tbl	= gm_fix_timer_timeattack_act_id_tbl;
		
		// アクションのフレームをオフセットして赤表示に変えるようにする
		part_timer->flag	|= GMD_FIX_PART_TIMER_FLAG_RED_BY_TEX_FRAME;
	}
	else {
		act_id_tbl	= gm_fix_timer_act_id_tbl;
	}
	
	// サブパーツ生成
	for (Sint32 i = 0; i < GME_FIX_TIMER_SUBPART_MAX; ++i) {
		obj_work	= GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK),
											  NULL,
											  0,
											  "FIX_TIMER");
		
		cpit_2d	= (GMS_COCKPIT_2D_WORK*)obj_work;
		
		// 描画処理関数差し替え
		obj_work->ppOut	= gmFixSubpartOutFunc;
		
		ObjObjectAction2dAMALoadSetTexlist(obj_work,
										   &cpit_2d->obj_2d,
										   NULL,//data_work
										   NULL,//filename
										   gm_fix_ama_amb_idx_tbl[GsEnvGetLanguage()][GME_FIX_AMA_IDX_COMMON],
										   GmGameDatGetCockpitData(),
										   AoTexGetTexList(&gm_fix_textures[GME_FIX_TEX_IDX_FIX]),
										   (Uint32)act_id_tbl[i],
										   FALSE);
		
		// フレームクリア
		gmFixSetFrameStatic(obj_work, 0);

#if _IPHONE
		// タイムアタック時には水平位置調整
		if (gmFixIsTimeAttack()) {
			obj_work->pos.x	+= FX_F32_TO_FX32(-98.0f);
		}
#endif //_IPHONE
		
		// ワークセット
		((GMS_FIX_PART_TIMER*)part_work)->sub_parts[i]	= (GMS_COCKPIT_2D_WORK*)obj_work;
	}
}

// =======================================================================
// gmFixTimerSSPartInit
/*!
  FIX SS用 タイマパーツ初期化
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
void gmFixTimerSSPartInit(GMS_FIX_MGR_WORK *mgr_work)
{
	OBS_OBJECT_WORK	*obj_work;
	GMS_COCKPIT_2D_WORK	*cpit_2d;
	GMS_FIX_PART_WORK	*part_work;
	GMS_FIX_PART_TIMER	*part_timer;
	
	
	part_work	= (GMS_FIX_PART_WORK*)&mgr_work->part_timer;
	part_timer	= &mgr_work->part_timer;
	
	// パーツ登録
	gmFixRegisterPart(mgr_work, part_work, GME_FIX_PART_TYPE_TIMER);
	
	// 更新処理関数設定
	part_work->proc_update	= gmFixTimerPartProcUpdateMain;
	
	// 表示関連処理関数設定
	part_work->proc_disp	= gmFixTimerPartProcDispMain;
	
	// アクションのフレームをオフセットして赤表示に変えるようにする
	part_timer->flag	|= GMD_FIX_PART_TIMER_FLAG_RED_BY_TEX_FRAME;
	
	// サブパーツ生成
	for (Sint32 i = 0; i < GME_FIX_TIMER_SUBPART_MAX; ++i) {
		GME_FIX_AMA_IDX	ama_idx;
		GME_FIX_TEX_IDX	tex_arc_idx;
		
		obj_work	= GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK),
											  NULL,
											  0,
											  "FIX_TIMER_SS");
		
		cpit_2d	= (GMS_COCKPIT_2D_WORK*)obj_work;
		
		// 各サブパーツに対応するAMAインデックス・テクスチャアーカイブインデックスを得る
		if (i == GME_FIX_TIMER_SUBPART_ICON) {
			ama_idx	= GME_FIX_AMA_IDX_SSFIX_LANG;
			tex_arc_idx	= GME_FIX_TEX_IDX_SSFIX_LANG;
		}
		else {
			ama_idx	= GME_FIX_AMA_IDX_SSFIX_CMN;
			tex_arc_idx	= GME_FIX_TEX_IDX_SSFIX_CMN;
		}
		
		// 描画処理関数差し替え
		obj_work->ppOut	= gmFixSubpartOutFunc;
		
		ObjObjectAction2dAMALoadSetTexlist(obj_work,
										   &cpit_2d->obj_2d,
										   NULL,//data_work
										   NULL,//filename
										   gm_fix_ama_amb_idx_tbl[GsEnvGetLanguage()][ama_idx],
										   GmGameDatGetCockpitData(),
										   AoTexGetTexList(&gm_fix_textures[tex_arc_idx]),
										   (Uint32)gm_fix_timer_ss_act_id_tbl[GsEnvGetLanguage()][i],
										   FALSE);
		// フレームクリア
		gmFixSetFrameStatic(obj_work, 0);
		
		// ワークセット
		((GMS_FIX_PART_TIMER*)part_work)->sub_parts[i]	= (GMS_COCKPIT_2D_WORK*)obj_work;
	}
}

// =======================================================================
// gmFixTimerPartProcUpdateMain
/*!
  タイマパーツ メイン更新処理関数
  
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixTimerPartProcUpdateMain(GMS_FIX_PART_WORK *part_work)
{
	GMS_FIX_PART_TIMER	*part_timer	= (GMS_FIX_PART_TIMER*)part_work;
	
	// 数字情報更新
	gmFixTimerPartUpdateDigitList(part_timer);
	
	// フラッシュ動作
	if (gmFixProcessRequest(part_work->parent_mgr, GMD_FIX_MGR_REQUEST_FLAG_TIMER_FLASH_ACT)) {
		gmFixTimerPartInitFlashAction(part_timer);
	}
	else {
		gmFixTimerPartUpdateFlashAction(part_timer);
	}
	
	// 点滅開始判定
	if (gmFixTimerPartIsTimeRunningOut(part_work->parent_mgr)) {
		
		Uint16	sec;
		// SE再生用に秒の値を取得
		AkUtilFrame60ToTime(gmFixGetGameTime(), NULL, &sec, NULL);
		
		if (!(part_work->flag & GMD_FIX_PART_FLAG_BLINK)) {
			
			part_work->flag	|= GMD_FIX_PART_FLAG_BLINK;
			
			gmFixInitBlink(part_work,
						   GMD_FIX_PART_TIMER_DIGIT_BLINK_ON_TIME,
						   GMD_FIX_PART_TIMER_DIGIT_BLINK_OFF_TIME);
			
			// 初回にも必ずSEが鳴るようにする
			part_timer->cur_sec	= (Uint16)(sec - 1);
		}
		
		// 秒が経過するたびにSE再生
		if (part_timer->cur_sec != sec) {
			part_timer->cur_sec	= sec;
			GmSoundPlaySE("Countdown");
		}
	}
	else {
		part_work->flag	&= ~GMD_FIX_PART_FLAG_BLINK;
	}
}

// =======================================================================
// gmFixTimerPartProcDispMain
/*!
  タイマパーツ メイン表示関連処理関数
  
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixTimerPartProcDispMain(GMS_FIX_PART_WORK *part_work)
{
	GMS_FIX_PART_TIMER	*part_timer	= (GMS_FIX_PART_TIMER*)part_work;
	
	// 点滅処理
	if (part_work->flag & GMD_FIX_PART_FLAG_BLINK) {
		
		// 点滅中は赤に色づけ
		gmFixTimerPartSetDigitsRed(part_timer, TRUE);
		
		if (gmFixUpdateBlink(part_work)) {
			part_work->flag	&= ~GMD_FIX_PART_FLAG_BLINK_OFF;
		}
		else {
			part_work->flag	|= GMD_FIX_PART_FLAG_BLINK_OFF;
		}
	}
	else {
		
		// 点滅していないときはは色づけしない
		gmFixTimerPartSetDigitsRed(part_timer, FALSE);
		
		part_work->flag	&= ~GMD_FIX_PART_FLAG_BLINK_OFF;
	}
	
	// 点滅の状態を反映
	if (part_work->flag & GMD_FIX_PART_FLAG_BLINK_OFF) {
		gmFixTimerPartSetDispDigits(part_timer, FALSE);
	}
	else {
		gmFixTimerPartSetDispDigits(part_timer, TRUE);
	}
	
	// 表示に反映
	gmFixTimerPartUpdateActionDigitsType(part_timer);
}

// =======================================================================
// gmFixTimerPartUpdateDigitList
/*!
  現在のゲームタイマ値を各桁の数字情報に反映する
 
  @param part_timer	[io]	タイマパーツ
  
  @note
  現在のゲームタイマ値を取得し、タイマパーツワークの各桁の数字情報に反映します。
  gmFixTimerPartUpdateActionDigitsType() を呼ぶことで、更新内容が表示に反映されます。
 */
// =======================================================================
void gmFixTimerPartUpdateDigitList(GMS_FIX_PART_TIMER *part_timer)
{
	Uint16 val[GMD_FIX_PART_TIMER_UNIT_NUM];
	const static Sint32 digit_num_list[GMD_FIX_PART_TIMER_UNIT_NUM] = {
		GMD_FIX_PART_TIMER_MSEC_DIGIT_NUM,
		GMD_FIX_PART_TIMER_SEC_DIGIT_NUM,
		GMD_FIX_PART_TIMER_MIN_DIGIT_NUM, };
	Sint32	index;
	
	// フレーム数を時間に変換
	AkUtilFrame60ToTime(gmFixGetGameTime(), &val[2], &val[1], &val[0]);
	
	// 分秒ミリ秒を各桁の数字に分解
	index	= 0;
	for (Sint32 i = 0; i < GMD_FIX_PART_TIMER_UNIT_NUM; ++i) {
		AkUtilNumValueToDigits(val[i], &part_timer->digit_list[index], digit_num_list[i]);
		index	+= digit_num_list[i];
	}
}


// =======================================================================
// gmFixTimerPartUpdateActionDigitsType
/*!
  タイマ表示の各桁アクションの数字タイプを設定する
  
  @param part_timer	[io]	タイマパーツ
 */
// =======================================================================
void gmFixTimerPartUpdateActionDigitsType(GMS_FIX_PART_TIMER *part_timer)
{
	// 各数字アクションのフレームをセット
	for (Sint32 i = 0; i < GMD_FIX_PART_TIMER_ALL_DIGIT_NUM; ++i) {
		GMS_COCKPIT_2D_WORK *cpit_2d	= part_timer->sub_parts[gm_fix_part_timer_digit_subpart_idx_tbl[i]];
		gmFixSetFrameStatic((OBS_OBJECT_WORK*)cpit_2d,
							gm_fix_part_common_digit_type_frame_tbl[part_timer->digit_list[i]] + part_timer->digit_frame_ofst);
	}
	
	// プライム（',"）のアクションのフレームをセット
	for (Sint32 i = 0; i < GMD_FIX_PART_TIMER_DECO_CHAR_NUM; ++i) {
		GMS_COCKPIT_2D_WORK	*cpit_2d	= part_timer->sub_parts[gm_fix_part_timer_deco_char_subpart_idx_tbl[i]];
		
		gmFixSetFrameStatic((OBS_OBJECT_WORK*)cpit_2d,
							0 + part_timer->deco_char_frame_ofst);
	}
}

// =======================================================================
// gmFixTimerPartSetDispDigits
/*!
  タイマ表示の数字サブパーツの表示・非表示を設定
  
  @param part_timer		[io]	タイマパーツ
  @param enable			[in]	有効フラグ
 */
// =======================================================================
void gmFixTimerPartSetDispDigits(GMS_FIX_PART_TIMER *part_timer, BOOL enable)
{
	for (Sint32 i = 0; i < GME_FIX_TIMER_SUBPART_MAX; ++i) {
		if (i == GME_FIX_TIMER_SUBPART_ICON) {
			continue;
		}
		
		if (enable) {
			((OBS_OBJECT_WORK*)part_timer->sub_parts[i])->disp_flag	&= ~OBD_DISP_NODISP;
		}
		else {
			((OBS_OBJECT_WORK*)part_timer->sub_parts[i])->disp_flag	|= OBD_DISP_NODISP;
		}
	}
}

// =======================================================================
// gmFixTimerPartSetDigitsRed
/*!
  タイマ表示の数字サブパーツを赤表示に設定
 
  @param part_timer	[io]	タイマパーツ
  @param enable		[in]	有効フラグ
  
  @note
  赤表示にする適切な方法を判定して、その方法で設定を行います。
 */
// =======================================================================
void gmFixTimerPartSetDigitsRed(GMS_FIX_PART_TIMER *part_timer, BOOL enable)
{
	if (part_timer->flag & GMD_FIX_PART_TIMER_FLAG_RED_BY_TEX_FRAME) {
		gmFixTimerPartSetTexRedDigits(part_timer, enable);
	}
	else {
		gmFixTimerPartSetColorRedDigits(part_timer, enable);
	}
}

// =======================================================================
// gmFixTimerPartSetColorRedDigits
/*!
  タイマ表示の数字サブパーツの色付け（赤）を設定
  
  @param part_timer	[io]	タイマパーツ
  @param enable		[in]	有効フラグ
  
  @note
  フェードカラーで色づけを行います。
 */
// =======================================================================
void gmFixTimerPartSetColorRedDigits(GMS_FIX_PART_TIMER *part_timer, BOOL enable)
{
	const static Uint32	color_red	= 0xff0000ff;
	const static Uint32	color_default	= 0xffffffff;
	
	for (Sint32 i = 0; i < GME_FIX_TIMER_SUBPART_MAX; ++i) {
		OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)part_timer->sub_parts[i];
		if (i == GME_FIX_TIMER_SUBPART_ICON) {
			continue;
		}
		
		if (enable) {
			obj_work->obj_2d->color.c	= color_red;
		}
		else {
			obj_work->obj_2d->color.c	= color_default;
		}
	}
}

// =======================================================================
// gmFixTimerPartSetTexRedDigits
/*!
  タイマ表示の数字サブパーツの色替えテクスチャ（赤）を設定
  
  @param part_timer	[io]	タイマパーツ
  @param enable		[in]	有効フラグ
  
  @note
  色違いのテクスチャに設定されるようにフレームを設定します。
 */
// =======================================================================
void gmFixTimerPartSetTexRedDigits(GMS_FIX_PART_TIMER *part_timer, BOOL enable)
{
	if (enable) {
		part_timer->digit_frame_ofst	= GMD_FIX_PART_TIMER_DIGIT_RED_FRAME_OFST;
		part_timer->deco_char_frame_ofst	= GMD_FIX_PART_TIMER_DECO_CHAR_RED_FRAME_OFST;
	}
	else {
		part_timer->digit_frame_ofst	= 0;
		part_timer->deco_char_frame_ofst	= 0;
	}
}

// =======================================================================
// gmFixTimerPartIsTimeRunningOut
/*!
  残り時間わずかか判定
  
  @param mgr_work	[in]	管理ワーク
  
  @retval TRUE	既定残り時間を経過
  @retval FALSE	既定残り時間に到達していない
 
  @note
  タイムオーバーまで既定時間を切っているか判定します。
 */
// =======================================================================
BOOL gmFixTimerPartIsTimeRunningOut(const GMS_FIX_MGR_WORK *mgr_work)
{
	Uint16 min;
	Uint16 sec;
	
	MTM_ASSERT(mgr_work);
	
	// フレーム数を時間に変換
	AkUtilFrame60ToTime(gmFixGetGameTime(), &min, &sec, NULL);
	
	// タイムアウトまでの残り秒数が既定秒数以下かチェック
	if (mgr_work->flag & GMD_FIX_MGR_FLAG_TIMER_COUNTDOWN) {
		// カウントダウンの場合
		if (min <= 0 &&
			sec < GMD_FIX_PART_TIMER_DIGIT_BLINK_REMAIN_SEC) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else {
		// カウントアップの場合
		if (min >= 9 &&
			sec + GMD_FIX_PART_TIMER_DIGIT_BLINK_REMAIN_SEC >= 60) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
}

// =======================================================================
// gmFixTimerPartInitFlashAction
/*!
  フラッシュ動作 初期化
  
  @param part_timer	[io]	タイマパーツ
 */
// =======================================================================
void gmFixTimerPartInitFlashAction(GMS_FIX_PART_TIMER *part_timer)
{
	part_timer->flag	|= GMD_FIX_PART_TIMER_FLAG_FLASH_ACT_ACTIVE;
	
	part_timer->fade_ratio	= 0.f;
	part_timer->scale_ratio	= 0.f;
	part_timer->flash_act_phase	= 0;
}

// =======================================================================
// gmFixTimerPartUpdateFlashAction
/*!
  フラッシュ動作 更新
  
  @param param0 [in] 入力引数0説明
 */
// =======================================================================
void gmFixTimerPartUpdateFlashAction(GMS_FIX_PART_TIMER *part_timer)
{
	BOOL	is_phase_end	= FALSE;
	
	// 有効でないときは何もしない
	if (!(part_timer->flag & GMD_FIX_PART_TIMER_FLAG_FLASH_ACT_ACTIVE)) {
		return;
	}
	
	// フェーズ処理
	switch (part_timer->flash_act_phase) {
	case 0:
		
		// スケール遅め、フェードを早めに進める
		part_timer->fade_ratio	+= 0.5f;
		part_timer->scale_ratio	+= 0.25f;
		
		part_timer->fade_ratio	= MTM_MATH_CLIP(part_timer->fade_ratio, 0.f, 1.f);
		part_timer->scale_ratio	= MTM_MATH_CLIP(part_timer->fade_ratio, 0.f, 1.f);
		
		// スケールが終わったタイミングで終了
		MTM_ASSERT(part_timer->fade_ratio >= part_timer->scale_ratio);	// フェードが先行
		if (part_timer->scale_ratio >= 1.f) {
			part_timer->flash_act_phase++;
		}
		break;
		
	case 1:
		
		// スケールは早め、フェードはゆっくり戻す
		part_timer->fade_ratio	-= 0.02f;
		part_timer->scale_ratio	-= 0.05f;
		
		part_timer->fade_ratio	= MTM_MATH_CLIP(part_timer->fade_ratio, 0.f, 1.f);
		part_timer->scale_ratio	= MTM_MATH_CLIP(part_timer->scale_ratio, 0.f, 1.f);
		
		// フェードが終わったタイミングで終了
		MTM_ASSERT(part_timer->fade_ratio >= part_timer->scale_ratio);	// スケールが先行
		if (part_timer->fade_ratio <= 0.f) {
			part_timer->flash_act_phase++;
		}
		break;
		
	default:
		is_phase_end	= TRUE;
	}
	
	
	const static Uint32 flash_subparts_tbl[]	= {
		GME_FIX_TIMER_SUBPART_NUM_10000,
		GME_FIX_TIMER_SUBPART_NUM_AP1,
		GME_FIX_TIMER_SUBPART_NUM_1000,
		GME_FIX_TIMER_SUBPART_NUM_100,
		GME_FIX_TIMER_SUBPART_NUM_AP2,
		GME_FIX_TIMER_SUBPART_NUM_10,
		GME_FIX_TIMER_SUBPART_NUM_1,
	};
	
	Uint32	elem_num	= sizeof(flash_subparts_tbl) / sizeof(flash_subparts_tbl[0]);
	
	
	if (!is_phase_end) {
		// 更新中
		
		
		
		fx32	cur_scale;
		Uint8	cur_alpha;
		
		// scale_ratioの度合いにより、スケール値をSCALE_DEF 〜 SCALE_MAXで変化させる
		cur_scale	=
			GMD_FIX_PART_TIMER_FLASH_ACT_SCALE_DEF +
				(fx32)(part_timer->scale_ratio * (GMD_FIX_PART_TIMER_FLASH_ACT_SCALE_MAX -
												  GMD_FIX_PART_TIMER_FLASH_ACT_SCALE_DEF));
		
		// fade_ratioの度合いにより、α値を0x00〜0xffで変化させる
		cur_alpha	= (Uint8)MTM_MATH_CLIP((0xff * part_timer->fade_ratio), 0, 0xff);
		
		// 各サブパーツに反映
		for (Uint32 i = 0; i < elem_num; ++i) {
			OBS_OBJECT_WORK	*obj_sub	= (OBS_OBJECT_WORK*)part_timer->sub_parts[flash_subparts_tbl[i]];
			
			obj_sub->scale.x	=
				obj_sub->scale.y	= cur_scale;
			
			// 目標の色を白に設定
			obj_sub->obj_2d->fade.b	=
				obj_sub->obj_2d->fade.g	=
					obj_sub->obj_2d->fade.r	= 0xff;
			
			obj_sub->obj_2d->fade.a	= cur_alpha;
		}
	}
	else {
		// 終了時
		
		// フラッシュ動作パラメータクリア
		part_timer->fade_ratio	= 0.f;
		part_timer->scale_ratio	= 0.f;
		part_timer->flash_act_phase	= 0;
		
		// オブジェクトの各種パラメータを初期化
		for (Uint32 i = 0; i < elem_num; ++i) {
			OBS_OBJECT_WORK	*obj_sub	= (OBS_OBJECT_WORK*)part_timer->sub_parts[flash_subparts_tbl[i]];
			
			obj_sub->scale.x	=
				obj_sub->scale.y	= GMD_FIX_PART_TIMER_FLASH_ACT_SCALE_DEF;
			
			obj_sub->obj_2d->fade.a	=
				obj_sub->obj_2d->fade.b	=
					obj_sub->obj_2d->fade.g	=
						obj_sub->obj_2d->fade.r	= 0x00;
		}
		
		// フラッシュ動作中でない
		part_timer->flag	&= ~GMD_FIX_PART_TIMER_FLAG_FLASH_ACT_ACTIVE;
	}
}

// ############################################################################
// チャレンジ数
// ############################################################################
// =======================================================================
// gmFixChallengePartInit
/*!
  FIX チャレンジ数パーツ初期化
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
void gmFixChallengePartInit(GMS_FIX_MGR_WORK* mgr_work)
{
	OBS_OBJECT_WORK	*obj_work;
	GMS_COCKPIT_2D_WORK	*cpit_2d;
	GMS_FIX_PART_WORK	*part_work;
	
	
	part_work	= (GMS_FIX_PART_WORK*)&mgr_work->part_challenge;
	
	// パーツ登録
	gmFixRegisterPart(mgr_work, part_work, GME_FIX_PART_TYPE_CHALLENGE);
	
	// 更新処理関数設定
	part_work->proc_update	= gmFixChallengePartProcUpdateMain;
	
	// 表示関連処理関数設定
	part_work->proc_disp	= gmFixChallengePartProcDispMain;
	
	// サブパーツ生成
	for (Sint32 i = 0; i < GME_FIX_CHALLENGE_SUBPART_MAX; ++i) {
		obj_work	= GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK),
											  NULL,
											  0,
											  "FIX_CHALLENGE");
		
		cpit_2d	= (GMS_COCKPIT_2D_WORK*)obj_work;
		
		// 描画処理関数差し替え
		obj_work->ppOut	= gmFixSubpartOutFunc;
		
		ObjObjectAction2dAMALoadSetTexlist(obj_work,
										   &cpit_2d->obj_2d,
										   NULL,//data_work
										   NULL,//filename
										   gm_fix_ama_amb_idx_tbl[GsEnvGetLanguage()][GME_FIX_AMA_IDX_COMMON],
										   GmGameDatGetCockpitData(),
										   AoTexGetTexList(&gm_fix_textures[GME_FIX_TEX_IDX_FIX]),
#if !_IPHONE
										   (Uint32)gm_fix_challenge_act_id_tbl[i],
#else //!_IPHONE
										   (Uint32)gm_fix_challenge_act_id_tbl[gmFixGetPlan()][i],
#endif //!_IPHONE
										   FALSE);
		
		// フレームクリア
		gmFixSetFrameStatic(obj_work, 0);
		
		// ワークセット
		((GMS_FIX_PART_CHALLENGE*)part_work)->sub_parts[i]	= (GMS_COCKPIT_2D_WORK*)obj_work;
	}
}


// =======================================================================
// gmFixChallengePartProcUpdateMain
/*!
  チャレンジ数パーツ メイン処理関数
  
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixChallengePartProcUpdateMain(GMS_FIX_PART_WORK *part_work)
{
	GMS_FIX_PART_CHALLENGE	*part_challenge	= (GMS_FIX_PART_CHALLENGE*)part_work;
	Float	sonic_icon_frame	= 0.f;
	
	// ソニックの状態に応じてアイコンを設定
	if (g_gm_main_system.ply_work[0]) {
		if (g_gm_main_system.ply_work[0]->player_flag & GMD_PLF_SUPER_SONIC) {
			sonic_icon_frame	= 1.f;
		}
		else {
			sonic_icon_frame	= 0.f;
		}
	}
	gmFixSetFrameStatic((OBS_OBJECT_WORK*)part_challenge->sub_parts[GME_FIX_CHALLENGE_SUBPART_SONIC],
						sonic_icon_frame);
	
	// 数字情報更新
	gmFixChallengePartUpdateDigitList(part_challenge);
}

// =======================================================================
// gmFixChallengePartProcDispMain
/*!
  チャレンジ数パーツパーツ メイン表示関連処理関数
  
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixChallengePartProcDispMain(GMS_FIX_PART_WORK *part_work)
{
	GMS_FIX_PART_CHALLENGE	*part_challenge	= (GMS_FIX_PART_CHALLENGE*)part_work;
	
	// 表示に反映
	gmFixChallengePartUpdateActionDigitsType(part_challenge);
}


// =======================================================================
// gmFixChallengePartUpdateDigitList
/*!
  現在のチャレンジ数を各桁の数字情報に反映する
  
  @param part_challenge	[io]	チャレンジ数パーツワーク
  
  @note
  現在のチャレンジ数を取得し、チャレンジ数パーツワークの各桁の数字情報に反映します。
  gmFixChallengePartUpdateActionDigitsType() を呼ぶことで、更新内容が反映されます。
 */
// =======================================================================
void gmFixChallengePartUpdateDigitList(GMS_FIX_PART_CHALLENGE *part_challenge)
{
	Sint32	challenge_num	= (Sint32)gmFixGetChallengeNum();	// チャレンジ数取得
	
	// 各桁の数字に分解
	AkUtilNumValueToDigits(challenge_num, part_challenge->digit_list, 
						   GMD_FIX_PART_CHALLENGE_DIGIT_NUM);
}

// =======================================================================
// gmFixChallengePartUpdateActionDigitsType
/*!
  チャレンジ数表示の各桁アクションの数字タイプを設定する
 
  @param part_challenge	[io]	スコアパーツ
 */
// =======================================================================
void gmFixChallengePartUpdateActionDigitsType(GMS_FIX_PART_CHALLENGE *part_challenge)
{
	// 各数字アクションのフレームをセット
	for (Sint32 i = 0; i < GMD_FIX_PART_CHALLENGE_DIGIT_NUM; ++i) {
		GMS_COCKPIT_2D_WORK	*cpit_2d	= part_challenge->sub_parts[gm_fix_part_challenge_digit_subpart_idx_tbl[i]];
		gmFixSetFrameStatic((OBS_OBJECT_WORK*)cpit_2d,
							gm_fix_part_common_digit_type_frame_tbl[part_challenge->digit_list[i]]);
	}
}


#if _IPHONE
// ############################################################################
// バーチャルパッド
// ############################################################################
// =======================================================================
// gmFixVirtualPadOutClass<NFlag>::OutFunc
/*!
  サブパーツ(バーチャルパッド)用描画処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
template <Uint32 NFlag>
class gmFixVirtualPadOutClass {
public:
	static void OutFunc(OBS_OBJECT_WORK *obj_work) {
		if (gm_fix_tcb) {
			GMS_FIX_MGR_WORK *mgr_work = (GMS_FIX_MGR_WORK *)mtTaskGetTcbWork(gm_fix_tcb);
			MTM_ASSERT(mgr_work);

			if (!(NFlag & mgr_work->flag)) {
				ObjDrawActionSummary(obj_work);
			}
		}
	}
};

// =======================================================================
// gmFixVirtualPadPartInit
/*!
  FIX バーチャルパッドパーツ初期化
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
void gmFixVirtualPadPartInit(GMS_FIX_MGR_WORK* mgr_work)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_COCKPIT_2D_WORK	*cpit_2d;
	GMS_FIX_PART_WORK	*part_work;
	
	// 描画処理関数テーブルの作成
	typedef void (*TOutFunc)(struct _OBS_OBJECT_WORK*);
	const TOutFunc c_out_func_table[GME_FIX_VIRTUAL_PAD_SUBPART_MAX] = {
		gmFixVirtualPadOutClass<GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_SUPER_SONIC>::OutFunc,	//<GME_FIX_VIRTUAL_PAD_SUBPART_SSONIC
		gmFixVirtualPadOutClass<GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_PAUSE>::OutFunc,			//<GME_FIX_VIRTUAL_PAD_SUBPART_PAUSE
		gmFixVirtualPadOutClass<GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_ACTION>::OutFunc,		//<GME_FIX_VIRTUAL_PAD_SUBPART_ACTION
		gmFixVirtualPadOutClass<GMD_FIX_MGR_FLAG_HIDE_VIRTUAL_PAD_PART_MOVE_PAD>::OutFunc,		//<GME_FIX_VIRTUAL_PAD_SUBPART_MOVE
	};
	
	part_work	= (GMS_FIX_PART_WORK*)&mgr_work->part_virtual_pad;
	
	// パーツ登録
	gmFixRegisterPart(mgr_work, part_work, GME_FIX_PART_TYPE_VIRTUAL_PAD);
	
	// 更新処理関数設定
	part_work->proc_update	= gmFixVirtualPadPartProcUpdateMain;
	
	// 表示関連処理関数設定
	part_work->proc_disp	= gmFixVirtualPadPartProcDispMain;
	
	// サブパーツ生成
	for (Sint32 i = 0, max = GME_FIX_VIRTUAL_PAD_SUBPART_MAX; i < max; ++i) {
		if (gm_fix_virtual_pad_act_id_tbl[gmFixGetPlan()][i] < 0) {
			((GMS_FIX_PART_VIRTUAL_PAD*)part_work)->sub_parts[i] = NULL;
			continue;
		}
		obj_work	= GMM_COCKPIT_CREATE_WORK(sizeof(GMS_COCKPIT_2D_WORK),
											  NULL,
											  0,
											  "FIX_VIRTUAL_PAD");
		
		cpit_2d	= (GMS_COCKPIT_2D_WORK*)obj_work;
		
		// 描画処理関数差し替え
		obj_work->ppOut	= c_out_func_table[i];
		
		ObjObjectAction2dAMALoadSetTexlist(obj_work,
										   &cpit_2d->obj_2d,
										   NULL,//data_work
										   NULL,//filename
										   gm_fix_ama_amb_idx_tbl[GsEnvGetLanguage()][GME_FIX_AMA_IDX_COMMON],
										   GmGameDatGetCockpitData(),
										   AoTexGetTexList(&gm_fix_textures[GME_FIX_TEX_IDX_FIX]),
										   (Uint32)gm_fix_virtual_pad_act_id_tbl[gmFixGetPlan()][i],
										   FALSE);

		//表示状態で初期化
		amFlagOff(obj_work->disp_flag, OBD_DISP_NODISP);
		
		// フレームクリア
		gmFixSetFrameStatic(obj_work, 0);
		
		//ポーズメニュー位置調整
		if (GME_FIX_VIRTUAL_PAD_SUBPART_PAUSE == i) {
			if (gmFixIsSpecialStage()) {
				//スペステでは右端に移動
				obj_work->pos.x	+= FX_F32_TO_FX32(400.0f);
			} else if (gmFixIsTimeAttack()) {
				//傾斜操作のタイムアタックではSSアイコンに左隣に移動
				obj_work->pos.x	+= FX_F32_TO_FX32(200.0f);
			}
		}

		// ワークセット
		((GMS_FIX_PART_VIRTUAL_PAD*)part_work)->sub_parts[i]	= (GMS_COCKPIT_2D_WORK*)obj_work;
	}
	
	//ポーズアイコンの各国対応
	switch (GsEnvGetLanguage()) {
	case GSD_LANGUAGE_IT:	//!< イタリア語
	case GSD_LANGUAGE_SP:	//!< スペイン語
		//ポーラー
		((GMS_FIX_PART_VIRTUAL_PAD*)part_work)->pause_icon_frame[0] = 2.0f;
		((GMS_FIX_PART_VIRTUAL_PAD*)part_work)->pause_icon_frame[1] = 3.0f;
		break;
	default:
		//ポーズ
		((GMS_FIX_PART_VIRTUAL_PAD*)part_work)->pause_icon_frame[0] = 0.0f;
		((GMS_FIX_PART_VIRTUAL_PAD*)part_work)->pause_icon_frame[1] = 1.0f;
		break;
	}
	
	amFlagOff(part_work->flag, GMD_FIX_PART_FLAG_NODISP);
}


// =======================================================================
// gmFixVirtualPadPartProcUpdateMain
/*!
  バーチャルパッドパーツ メイン処理関数
  
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixVirtualPadPartProcUpdateMain(GMS_FIX_PART_WORK *part_work)
{
	GMS_FIX_PART_VIRTUAL_PAD *part_virtual_pad = (GMS_FIX_PART_VIRTUAL_PAD*)part_work;

	// スーパーソニックボタン
	{
		OBS_OBJECT_WORK *obj_work = (OBS_OBJECT_WORK*)part_virtual_pad->sub_parts[GME_FIX_VIRTUAL_PAD_SUBPART_SSONIC];
		if (gmFixVirtualPadPartIsDispSuperSonicIcon(part_virtual_pad)) {
			amFlagOff(obj_work ->disp_flag, OBD_DISP_NODISP);
		} else {
			amFlagOn(obj_work ->disp_flag, OBD_DISP_NODISP);
		}
	}

	// ポーズボタン
	{
		OBS_OBJECT_WORK *obj_work = (OBS_OBJECT_WORK *)part_virtual_pad->sub_parts[GME_FIX_VIRTUAL_PAD_SUBPART_PAUSE];
		if (gmFixVirtualPadPartIsDispPauseIcon(part_virtual_pad)) {
			amFlagOff(obj_work->disp_flag, OBD_DISP_NODISP);
			if (gmFixVirtualPadPartIsOnPauseIcon(part_virtual_pad)) {
				gmFixSetFrameStatic(obj_work, part_virtual_pad->pause_icon_frame[1]);
			} else {
				gmFixSetFrameStatic(obj_work, part_virtual_pad->pause_icon_frame[0]);
			}
		} else {
			amFlagOn(obj_work->disp_flag, OBD_DISP_NODISP);
		}
	}

	//バーチャルパッド
	switch (gmFixGetPlan()) {
	case GME_FIX_PLAN_VIRTUAL_PAD:
		// 移動(十字)キー
		{
			OBS_OBJECT_WORK *obj_work = (OBS_OBJECT_WORK *)part_virtual_pad->sub_parts[GME_FIX_VIRTUAL_PAD_SUBPART_MOVE];
			float frame = gmFixVirtualPadPartGetMovePadFrame(part_virtual_pad);
			gmFixSetFrameStatic(obj_work, frame);
		}
		//nobreak;
	case GME_FIX_PLAN_FLICK:
		// アクション(ジャンプ)ボタン
		{
			OBS_OBJECT_WORK *obj_work = (OBS_OBJECT_WORK *)part_virtual_pad->sub_parts[GME_FIX_VIRTUAL_PAD_SUBPART_ACTION];
			if (gmFixVirtualPadPartIsOnActionIcon(part_virtual_pad)) {
				gmFixSetFrameStatic(obj_work, 1.0f);
			} else {
				gmFixSetFrameStatic(obj_work, 0.0f);
			}
		}
		break;
	case GME_FIX_PLAN_TILT:
	default:
		break;
	}
}

// =======================================================================
// gmFixVirtualPadPartProcDispMain
/*!
  バーチャルパッドパーツパーツ メイン表示関連処理関数
  
  @param part_work	[io]	パーツワーク
 */
// =======================================================================
void gmFixVirtualPadPartProcDispMain(GMS_FIX_PART_WORK *)
{
}

// =======================================================================
// gmFixVirtualPadPartIsDispSuperSonicIcon
/*!
  スーパーソニック変身アイコンを表示するか
  
  @param part_virtual_pad	[io]	バーチャルパッドパーツワーク
  
  @retval	true	表示する
  @retval	false	表示しない
 */

#define SS_BUTTON_NODISP_TIME (26.f)

// =======================================================================
bool gmFixVirtualPadPartIsDispSuperSonicIcon(GMS_FIX_PART_VIRTUAL_PAD *)
{
	mpp_ss_button_timer+=0.1f;//sss[24] farsh
	return GmPlayerIsTransformSuperSonic(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]) && (mpp_ss_button_timer>SS_BUTTON_NODISP_TIME);	
}

// =======================================================================
// gmFixVirtualPadPartIsDispPauseIcon
/*!
  ポーズアイコンを表示するか
  
  @param part_virtual_pad	[io]	バーチャルパッドパーツワーク
  
  @retval	true	表示する
  @retval	false	表示しない
 */
// =======================================================================
bool gmFixVirtualPadPartIsDispPauseIcon(GMS_FIX_PART_VIRTUAL_PAD *)
{
	return true;
}

// =======================================================================
// gmFixVirtualPadPartIsOnPauseIcon
/*!
  ポーズアイコンを押下状態にするか
  
  @param part_virtual_pad	[io]	バーチャルパッドパーツワーク
  
  @retval	true	押下状態にする
  @retval	false	押下状態にしない
 */
// =======================================================================
bool gmFixVirtualPadPartIsOnPauseIcon(GMS_FIX_PART_VIRTUAL_PAD *)
{
	bool result = false;
	if (GmPauseCheckExecutable() || (g_gm_main_system.game_flag & (GMD_GAME_FLAG_PAUSE_DEMO  | GMD_GAME_FLAG_PAUSE_IS_DECIDED))) {
		//ポーズデモ以外のポーズ不可なら
		if (0 <= GmMainKeyCheckPauseKeyOn()) {
			result = true;
		}
	}
	return result;
}

// =======================================================================
// gmFixVirtualPadPartGetMovePadFrame
/*!
  移動(十字)キーの表示フレームの取得
  
  @param part_virtual_pad	[io]	バーチャルパッドパーツワーク
  
  @return 移動(十字)キーに於いて表示するフレーム
 */
// =======================================================================
float gmFixVirtualPadPartGetMovePadFrame(GMS_FIX_PART_VIRTUAL_PAD *)
{
	const gm::CPadVirtualPad &virtual_pad = gm::CPadVirtualPad::CreateInstance();
	u16 pad = virtual_pad.GetValue();
	struct SKeyToFrame {
		AME_KEYMASK	key;
		float		frame;
	};
	const SKeyToFrame c_key_to_frame_table[] = {	{KEY_L_RIGHT	, 4.0f}
												,	{KEY_L_LEFT		, 3.0f}
												,	{KEY_L_DOWN		, 2.0f}
												,	{KEY_L_UP		, 1.0f}
												}; //先着優先
	float result = 0.0f;
	for (const SKeyToFrame *k2f = c_key_to_frame_table, *k2f_end = c_key_to_frame_table + arrayof(c_key_to_frame_table); k2f != k2f_end; ++k2f) {
		if (k2f->key & pad) {
			result = k2f->frame;
			break;
		}
	}
	return result;
}

// =======================================================================
// gmFixVirtualPadPartIsOnActionIcon
/*!
  アクションアイコンを押下状態にするか
  
  @param part_virtual_pad	[io]	バーチャルパッドパーツワーク
  
  @retval	true	押下状態にする
  @retval	false	押下状態にしない
 */
// =======================================================================
bool gmFixVirtualPadPartIsOnActionIcon(GMS_FIX_PART_VIRTUAL_PAD *)
{
	return GmPlayerKeyCheckJumpKeyOn(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]);
}
#endif //_IPHONE


// =======================================================================
// GmFixStaticVarInit
/*!
 static変数の初期化
 */
// =======================================================================
void GmFixStaticVarInit(void)
{
	//! FIX管理タスクTCB
	gm_fix_tcb	= NULL;
	
	//! テクスチャAMB参照ポインタリスト
	memset(gm_fix_texamb_list, 0, sizeof(gm_fix_texamb_list));
	//! AOテクスチャ構造体リスト
	memset(gm_fix_textures, 0, sizeof(gm_fix_textures));
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
