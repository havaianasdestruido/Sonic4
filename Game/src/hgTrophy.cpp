// =======================================================================
/*!
  @file	hgTrophy.cpp
  @brief HOG実績・トロフィー

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: hgTrophy.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"

#include "syEvtSys.h"
#include "gsTrophy.h"
#include "gsTrial.h"

#include "gmGameDat.h"
#include "gmMain.h"
#include "gmEventTbl.h"
#include "gmPlayer.h"
#include "gmEnemy.h"

#include "AkUtil.h"

#include "hgTrophy.h"

/*------ Macros --------------------------------------------------------*/

#define HGD_TROPHY_1000_ENE_KILL_THRESHOLD		(1000)	//!< エネミー1000体撃破閾値（文字通り1000）
#define HGD_TROPHY_ENE_KILL_COUNT_LIMIT			(HGD_TROPHY_1000_ENE_KILL_THRESHOLD)	//!< エネミー撃破カウント最大値
#define HGD_TROPHY_INC_PLY_DMG_COUNT_LIMIT		(1)		//!< プレイヤーダメージ回数カウント最大値（現状、1回までしかカウントしない）
#define HGD_TROPHY_FINAL_CLEAR_COUNT_LIMIT		(2)		//!< ファイナルクリア回数カウント最大数

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! トロフィータイプ
typedef enum
{
	HGE_TROPHY_TYPE_NORMAL	= 0,
	HGE_TROPHY_TYPE_AVATAR,
	
	HGE_TROPHY_TYPE_MAX
} HGE_TROPHY_TYPE;

//! トロフィーID列挙型（※データで定義してある並びと対応させること）
typedef enum
{
	HGE_TROPHY_ID_STAGE11_CLEAR	= 0,
	HGE_TROPHY_ID_DEFEAT_BOSS,
	HGE_TROPHY_ID_FIRST_CHAOS_EMERALD,
	HGE_TROPHY_ID_1000_ENE_KILL,
	HGE_TROPHY_ID_SSONIC_IN_ALL_ACT,
	HGE_TROPHY_ID_REACH_END,
	HGE_TROPHY_ID_UPLOAD_ALL_RECORDS,
	HGE_TROPHY_ID_STAGES1_ALL_RINGS,
	HGE_TROPHY_ID_99_CHALLENGE,
	HGE_TROPHY_ID_ALL_CHAOS_EMERALDS,
	HGE_TROPHY_ID_STAGE11_CLEAR_IN_1MIN,
	HGE_TROPHY_ID_STAGEF_CLEAR_NO_DAMAGE,
	
	HGE_TROPHY_ID_MAX
} HGE_TROPHY_ID;

//! トロフィーID（アバターアワード）列挙型
typedef enum
{
	HGE_TROPHY_AVATAR_ID_STAGE_ENDING_ALL_RINGS	= 0,
	HGE_TROPHY_AVATAR_ID_STAGEF_CLEAR_ALL_EMERALDS,
	
	HGE_TROPHY_AVATAR_ID_MAX
} HGE_TROPHY_AVATAR_ID;

//! 解除条件判定関数
typedef BOOL (*HGF_TROPHY_ACQUIRE_CHECK_FUNC)(void);

//! 判定情報構造体
typedef struct tag_HGS_TROPHY_CHECK_INFO
{
	HGE_TROPHY_TYPE					trophy_type;
	Uint32							trophy_id;
	HGF_TROPHY_ACQUIRE_CHECK_FUNC	acquire_check_func;
} HGS_TROPHY_CHECK_INFO;

//! 判定タイミング情報構造体
typedef struct tag_HGS_TROPHY_CHECK_TIMING_INFO
{
	const HGS_TROPHY_CHECK_INFO	*check_info_tbl;
	Sint32	num;
} HGS_TROPHY_CHECK_TIMING_INFO;




/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/

static BOOL hgTrophyIsPlayDemo(void);
static inline Uint32 hgTrophyGetClearTime(void);

#if _PS3 || _XBOX	// PS3,Xbox360のみ使用
static void hgTrophyTryAcquisitionNormal(const HGS_TROPHY_CHECK_INFO *check_info);
static void hgTrophyTryAcquisitionAvatar(const HGS_TROPHY_CHECK_INFO *check_info);
#endif /* _PS3 || _XBOX */

#if _PS3 || _XBOX	// PS3,Xbox360のみ使用
/* 判定関数 */
static BOOL hgTrophyCheckAcquireStage11Clear(void);
static BOOL hgTrophyCheckAcquireDefeatBoss(void);
static BOOL hgTrophyCheckAcquireFirstChaosEmerald(void);
static BOOL hgTrophyCheckAcquire1000EnemyKill(void);
static BOOL hgTrophyCheckAcquireSsonicInAllAct(void);
static BOOL hgTrophyCheckAcquireReachEnd(void);
static BOOL hgTrophyCheckAcquireUploadAllRecords(void);
static BOOL hgTrophyCheckAcquireStageS1AllRings(void);
static BOOL hgTrophyCheckAcquire99Challenge(void);
static BOOL hgTrophyCheckAcquireAllChaosEmeralds(void);
static BOOL hgTrophyCheckAcquireStage11ClearIn1Min(void);
static BOOL hgTrophyCheckAcquireStageFClearNoDamage(void);
// アバターアワード
static BOOL hgTrophyCheckAcquireStageEndingAllRings(void);
static BOOL hgTrophyCheckAcquireStageFClearAllEmeralds(void);
#endif /* _PS3 || _XBOX */

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

#if _PS3 || _XBOX	// PS3,Xbox360のみ使用


/* 判定情報テーブル定義 */
//! クリアデモ タイミング 判定情報テーブル
const static HGS_TROPHY_CHECK_INFO hg_trophy_check_info_tbl_clear_demo[]	= {
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_STAGE11_CLEAR,			hgTrophyCheckAcquireStage11Clear,	},
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_FIRST_CHAOS_EMERALD,		hgTrophyCheckAcquireFirstChaosEmerald,	},
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_STAGES1_ALL_RINGS,		hgTrophyCheckAcquireStageS1AllRings,	},
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_ALL_CHAOS_EMERALDS,		hgTrophyCheckAcquireAllChaosEmeralds,	},
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_STAGE11_CLEAR_IN_1MIN,	hgTrophyCheckAcquireStage11ClearIn1Min,	},
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_STAGEF_CLEAR_NO_DAMAGE,	hgTrophyCheckAcquireStageFClearNoDamage,	},
	{ HGE_TROPHY_TYPE_AVATAR,	HGE_TROPHY_AVATAR_ID_STAGEF_CLEAR_ALL_EMERALDS,	hgTrophyCheckAcquireStageFClearAllEmeralds,	},
};
const static HGS_TROPHY_CHECK_INFO hg_trophy_check_info_tbl_goal_in[]	= {
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_SSONIC_IN_ALL_ACT,		hgTrophyCheckAcquireSsonicInAllAct,	},
};
//! ボス撃破タイミング 判定情報テーブル
const static HGS_TROPHY_CHECK_INFO hg_trophy_check_info_tbl_defeat_boss[]	= {
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_DEFEAT_BOSS,			hgTrophyCheckAcquireDefeatBoss,	},
};
//! エネミー撃破数増加時
const static HGS_TROPHY_CHECK_INFO hg_trophy_check_info_tbl_inc_ene_kill_count[]	= {
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_1000_ENE_KILL,	hgTrophyCheckAcquire1000EnemyKill,	},
};
//! スタッフロール終了時
const static HGS_TROPHY_CHECK_INFO hg_trophy_check_info_tbl_end_credits_finished[]	= {
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_REACH_END,	hgTrophyCheckAcquireReachEnd,	},
};
const static HGS_TROPHY_CHECK_INFO hg_trophy_check_info_tbl_upload_record[]	= {
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_UPLOAD_ALL_RECORDS,	hgTrophyCheckAcquireUploadAllRecords,	},
};
//! チャレンジ数増加タイミング 判定情報テーブル
const static HGS_TROPHY_CHECK_INFO hg_trophy_check_info_tbl_inc_challenge[]	= {
	{ HGE_TROPHY_TYPE_NORMAL,	HGE_TROPHY_ID_99_CHALLENGE,	hgTrophyCheckAcquire99Challenge,	},
};
//! エンディングステージクリア時 判定情報テーブル
const static HGS_TROPHY_CHECK_INFO hg_trophy_check_info_tbl_clear_ending_stage[]	= {
	{ HGE_TROPHY_TYPE_AVATAR,	HGE_TROPHY_AVATAR_ID_STAGE_ENDING_ALL_RINGS, hgTrophyCheckAcquireStageEndingAllRings,	},
};

//! 判定情報テーブル
const static HGS_TROPHY_CHECK_TIMING_INFO hg_trophy_check_timing_info_tbl[HGE_TROPHY_CHECK_TIMING_MAX]	= {
	{	hg_trophy_check_info_tbl_clear_demo,	sizeof(hg_trophy_check_info_tbl_clear_demo) / sizeof(hg_trophy_check_info_tbl_clear_demo[0])	},
	{	hg_trophy_check_info_tbl_goal_in,	sizeof(hg_trophy_check_info_tbl_goal_in) / sizeof(hg_trophy_check_info_tbl_goal_in[0])	},
	{	hg_trophy_check_info_tbl_defeat_boss,	sizeof(hg_trophy_check_info_tbl_defeat_boss) / sizeof(hg_trophy_check_info_tbl_defeat_boss[0])	},
	{	hg_trophy_check_info_tbl_inc_ene_kill_count, sizeof(hg_trophy_check_info_tbl_inc_ene_kill_count) / sizeof(hg_trophy_check_info_tbl_inc_ene_kill_count[0])	},
	{	hg_trophy_check_info_tbl_end_credits_finished, sizeof(hg_trophy_check_info_tbl_end_credits_finished) / sizeof(hg_trophy_check_info_tbl_end_credits_finished[0])	},
	{	hg_trophy_check_info_tbl_upload_record,	sizeof(hg_trophy_check_info_tbl_upload_record) / sizeof(hg_trophy_check_info_tbl_upload_record[0])	},
	{	hg_trophy_check_info_tbl_inc_challenge,	sizeof(hg_trophy_check_info_tbl_inc_challenge) / sizeof(hg_trophy_check_info_tbl_inc_challenge[0])	},
	{	hg_trophy_check_info_tbl_clear_ending_stage, sizeof(hg_trophy_check_info_tbl_clear_ending_stage) / sizeof(hg_trophy_check_info_tbl_clear_ending_stage[0])	},
};


#endif /* _PS3 || _XBOX */


/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// HgTrophyTryAcquisition
/*!
  トロフィー獲得判定＆更新
  
  @param timing	[in]	判定タイミング種別(HGE_TROPHY_CHECK_TIMING_XXX)
  
  @note
  指定タイミング種別に対応した解除判定・更新処理を行います。
 */
// =======================================================================
void HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING timing)
{
#if _PS3 || _XBOX	// PS3,Xbox360のみ使用
	
	const HGS_TROPHY_CHECK_TIMING_INFO	*check_timing_info	= &hg_trophy_check_timing_info_tbl[timing];
	
	// 体験版では何もしない
	if (GsTrialIsTrial()) {
		return;
	}
	
	// プレイデモの時は何もしない
	if (hgTrophyIsPlayDemo()) {
		return;
	}
	
	// 指定タイミングの各条件を処理
	for (Sint32 i = 0; i < check_timing_info->num; ++i) {
		const HGS_TROPHY_CHECK_INFO	*check_info	= &check_timing_info->check_info_tbl[i];
		
		switch (check_info->trophy_type) {
		case HGE_TROPHY_TYPE_NORMAL:
			hgTrophyTryAcquisitionNormal(check_info);
			break;
			
		case HGE_TROPHY_TYPE_AVATAR:
			hgTrophyTryAcquisitionAvatar(check_info);
			break;
			
		default:
			MTM_ASSERT(FALSE);
		}
	}
#else
	UNREFERENCED_PARAMETER(timing);
#endif /* _PS3 || _XBOX */
}


// =======================================================================
// HgTrophyIncEnemyKillCount
/*!
  エネミー撃破数増加
  
  @param ene_obj	[in]	エネミーオブジェクト
  
  @note
  エネミーを撃破したときに呼び出してください。
  対象となるエネミーオブジェクトの時のみカウントされます。
 */
// =======================================================================
void HgTrophyIncEnemyKillCount(const OBS_OBJECT_WORK *ene_obj)
{
	GSS_MAIN_SYS_INFO	*main_sys_info	= GsGetMainSysInfo();
	GMS_ENEMY_COM_WORK	*ene_com;
	
	// プレイデモの時は何もしない
	if (hgTrophyIsPlayDemo()) {
		return;
	}
	
	// エネミーワーク取得
	if (ene_obj->obj_type == GMD_OBJTYPE_ENEMY) {
		ene_com	= (GMS_ENEMY_COM_WORK*)ene_obj;
	}
	else {
		return;
	}
	
	// イベントレコードが無い場合は何もしない
	if (ene_com->eve_rec == NULL) {
		return;
	}
	
	// 対象外のIDの場合は何もしない
	// （「warning:comparison is always true」対策のため、
	//   「>=」を 「==」と「>」に分解して比較しています）
	if (!((ene_com->eve_rec->id == GMD_EVENT_ID_ENEMY_START ||
		   ene_com->eve_rec->id > GMD_EVENT_ID_ENEMY_START) &&
		  ene_com->eve_rec->id < GMD_EVENT_ID_ENE_ZAKO_END)) {
		return;
	}
	
	// カウントアップ
	if (main_sys_info->ene_kill_count < HGD_TROPHY_ENE_KILL_COUNT_LIMIT) {
		main_sys_info->ene_kill_count++;
	}
	else {
		main_sys_info->ene_kill_count	= HGD_TROPHY_ENE_KILL_COUNT_LIMIT;
	}
	
	// トロフィー・実績獲得判定
	HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_INC_ENE_KILL_COUNT);
}

// =======================================================================
// HgTrophyIncPlayerDamageCount
/*!
  プレイヤーダメージ回数増加
  
  @param ply_work	[io]	プレイヤーワーク
  
  @note
  プレイヤーがダメージを受けた際に呼び出してください。
 */
// =======================================================================
void HgTrophyIncPlayerDamageCount(const GMS_PLAYER_WORK *ply_work)
{
	UNREFERENCED_PARAMETER(ply_work);
	
	// プレイデモの時は何もしない
	if (hgTrophyIsPlayDemo()) {
		return;
	}
	
	// カウントアップ
	if (g_gm_main_system.ply_dmg_count < HGD_TROPHY_INC_PLY_DMG_COUNT_LIMIT) {
		// 現状、1回までしかカウントしない
		g_gm_main_system.ply_dmg_count++;
	}
	else {
		g_gm_main_system.ply_dmg_count	= HGD_TROPHY_INC_PLY_DMG_COUNT_LIMIT;
	}
}

// =======================================================================
// HgTrophyIncFinalClearCount
/*!
  ファイナルゾーンクリア回数増加
  
  @note
  ファイナルゾーンクリア時に呼び出してください。
 */
// =======================================================================
void HgTrophyIncFinalClearCount(void)
{
	GSS_MAIN_SYS_INFO	*main_sys_info	= GsGetMainSysInfo();
	
	
	// カウントアップ
	if (main_sys_info->final_clear_count < HGD_TROPHY_FINAL_CLEAR_COUNT_LIMIT) {
		main_sys_info->final_clear_count++;
	}
	else {
		main_sys_info->final_clear_count	= HGD_TROPHY_FINAL_CLEAR_COUNT_LIMIT;
	}
}

/*------ Static Functions ----------------------------------------------*/
// =======================================================================
// hgTrophyIsPlayDemo
/*!
  プレイデモ中か判定
  
  @retval TURE	プレイデモ中
  @retval FALSE	プレイデモ中ではない
 */
// =======================================================================
BOOL hgTrophyIsPlayDemo(void)
{
	// TODO : プレイデモ判定対応（必要なら）
	return FALSE;
}

// =======================================================================
// hgTrophyGetClearTime
/*!
  ゲームタイマ値取得
  
  @return 現在のゲームタイマ値
 */
// =======================================================================
inline Uint32 hgTrophyGetClearTime(void)
{
	if (GsGetMainSysInfo()->clear_time >= GMD_MAIN_TIME_MAX) {
		return GMD_MAIN_TIME_MAX;
	}
	return GsGetMainSysInfo()->clear_time;
}

#if _PS3 || _XBOX	// PS3,Xbox360のみ使用
// =======================================================================
// hgTrophyTryAcquisitionNormal
/*!
  通常トロフィー・実績獲得試行
  
  @param check_info	[in]	判定情報
 */
// =======================================================================
void hgTrophyTryAcquisitionNormal(const HGS_TROPHY_CHECK_INFO *check_info)
{
	// 獲得済みの場合はスキップ
	if (GsTrophyIsAcquired(check_info->trophy_id)) {
		return;
	}
	
	if (check_info->acquire_check_func()) {
		// 獲得条件を満たしていたら、獲得試行
		GsTrophyUpdateAcquisition(check_info->trophy_id);
	}
}


// =======================================================================
// hgTrophyTryAcquisitionAvatar
/*!
  アバターアワード獲得試行
  
  @param check_info	[in]	判定情報
 */
// =======================================================================
void hgTrophyTryAcquisitionAvatar(const HGS_TROPHY_CHECK_INFO *check_info)
{
#if _XBOX
	// 獲得済みの場合はスキップ
	if (GsTrophyAvatarIsAcquired(check_info->trophy_id)) {
		return;
	}
	
	if (check_info->acquire_check_func()) {
		// 獲得条件を満たしていたら、獲得試行
		GsTrophyAvatarUpdateAcquisition(check_info->trophy_id);
	}
#else
	UNREFERENCED_PARAMETER(check_info);
#endif /* _XBOX */
}
#endif /* _PS3 || _XBOX */


#if _PS3 || _XBOX	// PS3,Xbox360のみ使用

// ============================================================================
// 判定関数
// ============================================================================
// =======================================================================
// hgTrophyCheckAcquireStage11Clear
/*!
  トロフィー獲得判定 ステージ1-1クリア
 */
// =======================================================================
BOOL hgTrophyCheckAcquireStage11Clear(void)
{
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	// クリアデモ中に呼び出される前提
	
	if (GsGetMainSysInfo()->stage_id == GSD_MAIN_STAGE_ID_1_1) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// hgTrophyCheckAcquireDefeatBoss
/*!
  トロフィー獲得判定 ボス撃破
 */
// =======================================================================
BOOL hgTrophyCheckAcquireDefeatBoss(void)
{
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	// ボス撃破のタイミングは呼び出し側で判定してもらう
	
	if (GMM_MAIN_STAGE_IS_BOSS()) {
		return TRUE;
	}
	else {
		MTM_ASSERT("hgTrophy.cpp::hgTrophyCheckAcquireDefeatBoss() Error! Not supposed to be called in this stage\n");
		return FALSE;
	}
}

// =======================================================================
// hgTrophyCheckAcquireFirstChaosEmerald
/*!
  トロフィー獲得判定 一つ目のカオスエメラルド取得
 */
// =======================================================================
BOOL hgTrophyCheckAcquireFirstChaosEmerald(void)
{
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_CHAOSGET) {
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// hgTrophyCheckAcquire1000EnemyKill
/*!
  トロフィー獲得判定 エネミー1000体撃破
 */
// =======================================================================
BOOL hgTrophyCheckAcquire1000EnemyKill(void)
{
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	if (GsGetMainSysInfo()->ene_kill_count >= HGD_TROPHY_1000_ENE_KILL_THRESHOLD) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// hgTrophyCheckAcquireSsonicInAllAct
/*!
  トロフィー獲得判定 全てのACTでスーパーソニックでクリア
  
  @note
  クリアの瞬間にスーパーソニックの姿でなくてはならない。
 */
// =======================================================================
BOOL hgTrophyCheckAcquireSsonicInAllAct(void)
{
	BOOL	result	= TRUE;
	
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	// 今クリアしたステージが「ACTステージではない場合」や
	// 「エンディングステージの場合」、「スパソニでゴールしていない」場合は対象外なので何もしない
	if (GSD_MAIN_STAGE_TYPE_ACT != g_gm_gamedat_stage_type_tbl[g_gs_main_sys_info.stage_id] ||
		g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_ENDING ||
		!GMM_MAIN_GOAL_AS_SUPER_SONIC()) {
		return FALSE;
	}
	
	// 全ACTについてスパソニクリアかどうか判定
	for (Uint32 stage_id = 0; stage_id < GSD_MAIN_STAGE_ID_MAX; ++stage_id) {
		
		if (stage_id == GSD_MAIN_STAGE_ID_ENDING) {
			// エンディングステージは対象外
			continue;
		}
		
		// ACTステージかチェック
		if (GSD_MAIN_STAGE_TYPE_ACT == g_gm_gamedat_stage_type_tbl[stage_id]) {
			
			MTM_ASSERT(stage_id < GSD_MAIN_STAGE_ID_FINAL_1);	// 念のためチェック
			
			if (stage_id == g_gs_main_sys_info.stage_id) {
				// 現在のステージについてはまだ「スパソニでゴール」の記録が行われていないので、
				// 今現在の情報から自前で判定
				if (!GMM_MAIN_GOAL_AS_SUPER_SONIC()) {
					result	= FALSE;
				}
			}
			else {
				if (!GsMainSysIsStageGoalAsSuperSonic(stage_id)) {
					result	= FALSE;
				}
			}
		}
	}
	
	return result;
}

// =======================================================================
// hgTrophyCheckAcquireReachEnd
/*!
  トロフィー獲得判定 最後の敵を倒してエンディングの最後に到達
  
  @note
  スタッフロール終了後のタイミングで解除。
  エンディングステージ後のスタッフロールの場合の時のみ呼ぶ。
 */
// =======================================================================
BOOL hgTrophyCheckAcquireReachEnd(void)
{
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_STAFFROLL);
	
	// 呼び出し元で判定して呼んでもらうので必ずTRUEを返す。
	return TRUE;
}

// =======================================================================
// hgTrophyCheckAcquireUploadAllRecords
/*!
  トロフィー獲得判定 全ての記録をアップロード
  
  @note
  (スコア+タイム) * 24ステージ = 48項目 を全てアップロードしたかチェック
 */
// =======================================================================
BOOL hgTrophyCheckAcquireUploadAllRecords(void)
{
	BOOL result	= TRUE;
	
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_RANKING ||
			   SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	for (Uint32 stage_id = 0; stage_id < GSD_MAIN_STAGE_ID_MAX; ++stage_id) {
		
		if (stage_id == GSD_MAIN_STAGE_ID_ENDING) {
			// エンディングステージは対象外
			continue;
		}
		
		// MEMO : FINAL-2以降のFINALステージはFINAL-1として扱われる
		if (!(GsMainSysIsStageScoreUploadOnce(stage_id) &&
			  GsMainSysIsStageTimeUploadOnce(stage_id))) {
			result	= FALSE;
		}
	}
	
	return result;
}

// =======================================================================
// hgTrophyCheckAcquireStageS1AllRings
/*!
  トロフィー獲得判定 スペシャルステージ１のリングを全て取得＆クリア
  
  @note
  カオスエメラルド(1UP)を取得するか、「!」に接触したらクリア扱い
 */
// =======================================================================
BOOL hgTrophyCheckAcquireStageS1AllRings(void)
{
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	if (GsGetMainSysInfo()->stage_id == GSD_MAIN_STAGE_ID_SS1) {
		
		if ((g_gm_main_system.game_flag & (GMD_GAME_FLAG_SPL_CHAOSGET | GMD_GAME_FLAG_SPL_FAILED)) &&
			GsGetMainSysInfo()->clear_ring >= GmEventMgrGetRingNum()) {
			MTM_ASSERT(GsGetMainSysInfo()->clear_ring == GmEventMgrGetRingNum());
			return TRUE;
		}
	}
	
	return FALSE;
}

// =======================================================================
// hgTrophyCheckAcquire99Challenge
/*!
  トロフィー獲得判定 チャレンジ数99以上
 */
// =======================================================================
BOOL hgTrophyCheckAcquire99Challenge(void)
{
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	if (g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] >= 100) {
		// ゲーム中の残チャレンジ数表示が99になったときに真
		// （＝実際のチャレンジ数が100になったとき）
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// hgTrophyCheckAcquireAllChaosEmeralds
/*!
  トロフィー獲得判定 全てのカオスエメラルド取得
 */
// =======================================================================
BOOL hgTrophyCheckAcquireAllChaosEmeralds(void)
{
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	// 7つ取得済みかをクリア毎に必ず判定するのではなく、
	// 今回のクリアでエメラルドを獲得したとき（既・未取得関わらず）のみ判定する
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPL_CHAOSGET) {
		if (GsGetMainSysInfo()->game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD) {
			return TRUE;
		}
	}
	
	return FALSE;
}

// =======================================================================
// hgTrophyCheckAcquireStage11ClearIn1Min
/*!
  トロフィー獲得判定 ステージ1-1を1分以内にクリア
  
  @note
  タイムアタックモード以外では、タイムオーバーの場合は対象外となります。
  タイムアタックモードの場合は死亡したら必ずスタート地点に戻るため、
  タイムオーバーになっても対象外にはなりません。
 */
// =======================================================================
BOOL hgTrophyCheckAcquireStage11ClearIn1Min(void)
{
	GSS_MAIN_SYS_INFO	*main_sys_info	= GsGetMainSysInfo();
	
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	if (main_sys_info->stage_id == GSD_MAIN_STAGE_ID_1_1) {
		
		// タイムアタックモード「以外」では、
		// チェックポイントから再開したときにタイマがリセットされた場合は対象外。
		// まだチェックポイントを通過していない状態でタイムオーバー（タイマがリセット）になる分には対象「内」となる
		if ((main_sys_info->game_mode != GSD_GAME_MODE_TIME_ATTACK)) {
			if (main_sys_info->game_flag & GSD_MAINSYS_GAME_FLAG_TIME_RESET_AT_MARKER) {
				return FALSE;
			}
		}
#if defined(MTD_DEBUG)
		else {
			// タイムアタックの場合は必ずスタート地点に戻るので
			// タイムオーバーかどうかはチェックしない
			MTM_ASSERT(!(g_gm_main_system.game_flag & GMD_GAME_FLAG_RESUMED_FROM_MARKER));
			
			// 何もしない
		}
#endif /* defined(MTD_DEBUG) */
		
		Uint16	min;
		Uint16	sec;
		Uint16	msec;
		AkUtilFrame60ToTime(hgTrophyGetClearTime(), &min, &sec, &msec);
		
		if (min == 0 || (min == 1 && sec == 0 && msec == 0) ) {
			return TRUE;
		}
	}
	
	return FALSE;
}

// =======================================================================
// hgTrophyCheckAcquireStageFClearNoDamage
/*!
  トロフィー獲得判定 ステージFINALをノーダメージでクリア
  
  @note
  （チェックポイントではなく）ステージの最初の位置開始して
  一度もダメージを受けずにクリアしたかチェックします。
  チェックポイントから再開した場合は、その後ノーダメージでクリアしたとしても
  条件を満たしたとはみなされません。
 */
// =======================================================================
BOOL hgTrophyCheckAcquireStageFClearNoDamage(void)
{
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	if (GsGetMainSysInfo()->stage_id == GSD_MAIN_STAGE_ID_FINAL_1) {
		if (g_gm_main_system.ply_dmg_count == 0 &&
			!(g_gm_main_system.game_flag & GMD_GAME_FLAG_RESUMED_FROM_MARKER)) {
			// g_gm_main_system.marker_priの値はチェックポイントを通過した時点で変わってしまうので、
			// 開始時点で記録された「チェックポイントからの再開の有無」フラグを参照する
			return TRUE;
		}
	}
	
	return FALSE;
}

// =======================================================================
// hgTrophyCheckAcquireStageEndingAllRings
/*!
  アバターアワード獲得判定 エンディングステージで全てのリングを獲得
  
  @note
  ファイナルゾーンの場合、カオスエメラルドを全て取得済みで、
  かつ、今回のクリアも含めてファイナルゾーンクリア回数が2回以上かチェックします。
 */
// =======================================================================
BOOL hgTrophyCheckAcquireStageEndingAllRings(void)
{
#if _XBOX
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_ENDING);
	
	if (GsGetMainSysInfo()->stage_id == GSD_MAIN_STAGE_ID_ENDING) {
		if (GsGetMainSysInfo()->clear_ring >= GmEventMgrGetRingNum()) {
			MTM_ASSERT(GsGetMainSysInfo()->clear_ring == GmEventMgrGetRingNum());
			return TRUE;
		}
	}
	
	return FALSE;
#else
	return FALSE;
#endif /* _XBOX */
}

// =======================================================================
// hgTrophyCheckAcquireStageEndingAllRings
/*!
  アバターアワード獲得判定 全てのカオスエメラルドを集めた後にファイナルステージをクリア
 */
// =======================================================================
BOOL hgTrophyCheckAcquireStageFClearAllEmeralds(void)
{
#if _XBOX
	MTM_ASSERT(SyGetEvtInfo()->cur_evt_id == GSD_EVT_ID_MAINGAME);
	
	GSS_MAIN_SYS_INFO	*main_sys_info	= GsGetMainSysInfo();
	
	if (main_sys_info->stage_id == GSD_MAIN_STAGE_ID_FINAL_1) {
		if ((GsGetMainSysInfo()->game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD) &&
			main_sys_info->final_clear_count >= 2) {
			return TRUE;
		}
	}
	
	return FALSE;
#else
	return FALSE;
#endif /* _XBOX */
}

#endif /* _PS3 || _XBOX */


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
