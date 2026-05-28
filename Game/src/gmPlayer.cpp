// ==========================================================================
/*!
  @file gmPlayer.cpp
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlayer.cpp 22 2011-04-25 02:28:55Z thamada $
  $Date:: 2011-04-25 11:28:55 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "gmSound.h"
#include "objObject.h"
#include "gmRing.h"
#include "gmEnemy.h"
#include "gmEventTbl.h"
#include "gmPlyEfct.h"
#include "gmComEfct.h"
#include "gmGameDBuild.h"
#include "gmCamera.h"
#include "gmObj.h"
#include "gmScore.h"
#include "gmPlyScoreDef.h"
#include "gmSound.h"

#include "gmPlayerDat.h"
#include "gmPlySeq.h"
#include "gmPlySeqGmk.h"
#include "gmPlayer.h"
#include "gmPlySpec.h"
#include "gmEnding.h"

#include "hgTrophy.h"

#include "gmGameDat.h"	// スペステ チェック(検索用:_SS/SPL)

#if _IPHONE
#include "gmPadPolarHandle.hpp"
#include "gmPadVirtualPad.hpp"
#endif // _IPHONE

// データヘッダ
#include "common/model/SON_MDL.hmb"
#include "common/model/SSON_MDL.hmb"

#include "mppAchievementSupport.h"

//----- Definitions ---------------------------------------------------------
#define GMD_PLY_USE_ROT_Z_TRUCK_MOVE	(1)		//!< トロッコ移動に傾きを使用する

#if 1
#define GMD_PLD_TRUCK_SLOPE_FLY_SPD			(0x4000)//(0x7000)					//!< 坂道飛び出しチェックを行う速度
#define GMD_PLD_TRUCK_SLOPE_FLY_DIR			(0x1000)					//!< 坂道飛び出しチェックを行う角度
#define GMD_PLD_TRUCK_SLOPE_FLY_SPD_PER		((fx32)(FX32_ONE*1))//((fx32)(FX32_ONE*0.7))		//!< 坂道飛び出し速度係数
#else
static fx32 truck_slope_fly_spd = 0x7000;
static u16 truck_slope_fly_dir = 0x1000;
static float track_slopefly_spd_m_per = 0.7f;
#define GMD_PLD_TRUCK_SLOPE_FLY_SPD			(truck_slope_fly_spd)						//!< 坂道飛び出しチェックを行う速度
#define GMD_PLD_TRUCK_SLOPE_FLY_DIR			(truck_slope_fly_dir)						//!< 坂道飛び出しチェックを行う角度
#define GMD_PLD_TRUCK_SLOPE_FLY_SPD_PER		((fx32)(FX32_ONE*track_slopefly_spd_m_per))	//!< 坂道飛び出し速度係数
#endif


#if _WII
/// Wii用プレイヤーマテリアルコールバックパラメータ
typedef struct tag_GMS_PLAYER_MAT_CALLBACK_PARAM {
	u32		draw_id;
} GMS_PLAYER_MAT_CALLBACK_PARAM;


#endif	// #if _WII

#if _IPHONE
/// タッチ領域設定
typedef enum tag_GME_PLAYER_TOUCH_RECT {
	GME_PLAYER_TOUCH_RECT_F, //!< 前方領域タッチ
	GME_PLAYER_TOUCH_RECT_B, //!< 後方領域タッチ
	
	GME_PLAYER_TOUCH_RECT_NUM //!< 総数
} GME_PLAYER_TOUCH_RECT;

#endif // _IPHONE

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
// ================================================================
// GMM_PLAYER_USE_TRICK_GMK
/*!
	トリック可能ギミック使用

  @param ply_work [in] プレイヤーワークポインタ

  @note
	trick_countにギミック使用カウントをセットし、ギミックのトリック使用カウンタを加算します。\n
	トリックギミックが設定されていない場合はなにもしません
 */
// ================================================================
#define GMM_PLAYER_USE_TRICK_GMK(ply_work)	\
{ \
	if ((ply_work)->trick_gmk_eve_rec) { \
		(ply_work)->trick_count = ply_work->trick_gmk_eve_rec->byte_param[0]; \
		if ((ply_work)->trick_gmk_eve_rec->byte_param[0] < 0xff ) { \
            ++(ply_work)->trick_gmk_eve_rec->byte_param[0]; \
		} \
		(ply_work)->trick_gmk_eve_rec = NULL; \
	} \
}

#if _IPHONE
// ================================================================
// GMM_PLAYER_IS_TOUCH_SUPER_SONIC_REGION
/*!
 スーパーソニック領域タッチ判定
 
 @param x [in] タッチ座標X
 @param y [in] タッチ座標Y
 
 @note
 指定された領域を触ったか否かを判定します。
 */
// ================================================================
#define GMM_PLAYER_IS_TOUCH_SUPER_SONIC_REGION(x, y)	(((x) > 390 && (x) < 475) && ((y) > 5 && (y) < 85))
#endif // _IPHONE

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
// データ読み込み
//static void gmPlayerLoadDest(MTS_TASK_TCB *tcb);
//static void gmPlayerLoadWait(MTS_TASK_TCB *tcb);

// 
static BOOL gmPlayerObjRelease(OBS_OBJECT_WORK *obj_work);
static BOOL gmPlayerObjReleaseWait(OBS_OBJECT_WORK *obj_work);
static void gmPlayerExit(MTS_TASK_TCB *tcb);
static void gmPlayerMain(OBS_OBJECT_WORK *obj_work);

static void gmPlayerDispFunc(OBS_OBJECT_WORK *obj_work);
static void gmPlayerDefaultInFunc(OBS_OBJECT_WORK *obj_work);
static void gmPlayerSplStgInFunc(OBS_OBJECT_WORK *obj_work);
static void gmPlayerRectTruckFunc(OBS_OBJECT_WORK *obj_work);
static void gmPlayerDefaultLastFunc(OBS_OBJECT_WORK *obj_work);
static void gmPlayerTruckCollisionFunc(OBS_OBJECT_WORK *obj_work);

static void gmPlayerAtkFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect);
static void gmPlayerDefFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect);

// プレイヤーステータス設定
static void gmPlayerPushSet(GMS_PLAYER_WORK *ply_work);
static void gmPlayerEarthTouch(GMS_PLAYER_WORK *ply_work);
//static void gmPlayerCheckTruckEarthTouch(GMS_PLAYER_WORK *ply_work);
//static void gmPlyTruckObjCollision(OBS_OBJECT_WORK *obj_work);
static void gmPlayerWaterCheck(GMS_PLAYER_WORK *ply_work);
static void gmPlayerTimeOverCheck(GMS_PLAYER_WORK *ply_work);
static void gmPlayerFallDownCheck(GMS_PLAYER_WORK *ply_work);
static void gmPlayerGetHomingTarget(GMS_PLAYER_WORK *ply_work);
static void gmPlayerSuperSonicCheck(GMS_PLAYER_WORK *ply_work);
static void gmPlayerSuperSonicToSonic(GMS_PLAYER_WORK *ply_work);
static void gmPlayerPressureCheck(GMS_PLAYER_WORK *ply_work);

// キー関連
static void gmPlayerKeyGet(GMS_PLAYER_WORK *ply_work);
static u16 gmPlayerRemapKey(GMS_PLAYER_WORK *ply_work, u16 key, Angle32 key_rot_z);
#if _IPHONE
// iPhone関数
#include "dbgPadEmu.hpp"

static Angle32 gmPlayerKeyGetRotZ(GMS_PLAYER_WORK *ply_work);
static u16 gmPlayerRemapKeyIPhone(GMS_PLAYER_WORK *ply_work);
static u16 gmPlayerRemapKeyIPhoneZone32SS(GMS_PLAYER_WORK *ply_work);
static BOOL gmPlayerIsInputDPadJumpKey(GMS_PLAYER_WORK *ply_work, s32 ignore_input);
static BOOL gmPlayerIsInputDPadSSonicKey(GMS_PLAYER_WORK *ply_work, s32 ignore_input);
#endif

// カメラ関連
static void gmPlayerCameraOffset(GMS_PLAYER_WORK *ply_work);

#if _WII
// Wii用プレイヤー手ロッド切り替え
static void gmPlayerSetLodAction(GMS_PLAYER_WORK *ply_work, s32 act_id);
static void gmPlayerSetLodActionFrame(GMS_PLAYER_WORK *ply_work, fx32 frame);
static void gmGmkPlayerMotionCallback(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param);
static NNE_BOOL gmPlayerMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif

// トロッコ用
static void gmGmkPlayerMotionCallbackTruck(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param);


//----- Global Variables ----------------------------------------------------
/// プレイヤーデータ格納用データワーク
OBS_DATA_WORK		g_gm_player_data_work[GSD_MAIN_PLAYER_MAX][GMD_PLAYER_DATA_MAX] = {{{0}}};

static OBS_ACTION3D_NN_WORK *g_gm_ply_son_obj_3d_list = NULL;				//!< ソニックモデル
static OBS_ACTION3D_NN_WORK *g_gm_ply_sson_obj_3d_list = NULL;				//!< スーパーソニックモデル

//----- Local Variables -----------------------------------------------------
// 自分プレイヤー座標保持用(ゴースト記録等に使用)
static s32 gm_pos_x = 0;	//[1:31:0]
static s32 gm_pos_y = 0;	//[1:31:0]
static s32 gm_pos_z = 0;	//[1:31:0]
// static GMS_GHOST_DATA _nl_ghost[GMD_GHOST_TIME] = {0}; // 6byte x 2700 = 16k 
// static GMS_GHOST_DATA _nl_ghost_save[GMD_GHOST_TIME] = {0}; // 6byte x 2700 = 16k 
// static s16 _nl_ghost_check = 0; // GHOST記録ありなしチェック
//static MTS_TASK_TCB * gm_player_bgm_tcb = NULL; // 特殊BGM管理タスク

//static MTS_TASK_TCB	*gm_player_data_load_tcb = NULL;	//!< データ読み込みTCB


/// キーマップ対応キーリスト
static u32	gm_key_map_key_list[GMD_PLAYER_KEY_MAP_MAX] = {
				KEY_L_UP, KEY_L_DOWN, KEY_L_LEFT, KEY_L_RIGHT,
				KEY_R_DOWN, KEY_R_RIGHT, KEY_R_LEFT, KEY_R_UP
};


/// プレイヤー使用モデルデータリスト
static OBS_ACTION3D_NN_WORK **gm_ply_obj_3d_list_tbl[GSD_CHAR_ID_MAX][GMD_PLY_MODEL_SET_MAX] = {
	{&g_gm_ply_son_obj_3d_list, &g_gm_ply_sson_obj_3d_list},		// ソニック
	{&g_gm_ply_son_obj_3d_list, &g_gm_ply_sson_obj_3d_list},		// スーパーソニック
	{&g_gm_ply_son_obj_3d_list, &g_gm_ply_sson_obj_3d_list},		// スペステソニック
	{&g_gm_ply_son_obj_3d_list, &g_gm_ply_sson_obj_3d_list},		// ピンボールソニック
	{&g_gm_ply_son_obj_3d_list, &g_gm_ply_sson_obj_3d_list},		// ピンボールスーパーソニック
	{&g_gm_ply_son_obj_3d_list, &g_gm_ply_sson_obj_3d_list},		// トロッコソニック
	{&g_gm_ply_son_obj_3d_list, &g_gm_ply_sson_obj_3d_list},		// トロッコスーパーソニック
};

#ifdef _IPHONE
//// プレイヤータッチ矩形
static OBS_RECT_WORK gm_ply_touch_rect[GME_PLAYER_TOUCH_RECT_NUM] = {{0}};


//// コントロールタイプ
static GME_PLAYER_CONTROL_TYPE gm_player_control_type = GME_PLAYER_CONTROL_TYPE_TILT;

//// Aボタン入力矩形
const static u16 gm_player_push_jump_key_rect[GME_PLAYER_CONTROL_TYPE_NUM][MTD_RECT] = {
	{400, 228, 472, 300}, // Aタイプ
	{400, 228, 472, 300}, // Bタイプ(下配置に変更)
	{  0,   0,   0,   0}, // 傾斜タイプ(ダミー)
};

//// SSonicボタン入力矩形
const static u16 gm_player_push_ssonic_key_rect[GME_PLAYER_CONTROL_TYPE_NUM][MTD_RECT] = {
	{320, 228, 392, 300}, // Aタイプ
	{320, 228, 392, 300}, // Bタイプ(下配置に変更)
	{400,   5, 472,  85}, // 傾斜タイプ
};
#endif // _IPHONE

/// コンボスコアテーブル
const static s32 gm_ply_score_combo_tbl[GMD_PLY_SCORE_COMBO_MAX] = {
	GMD_PLY_SCORE_COMBO_1,
	GMD_PLY_SCORE_COMBO_2,
	GMD_PLY_SCORE_COMBO_3,
	GMD_PLY_SCORE_COMBO_4,
	GMD_PLY_SCORE_COMBO_5,
};

/// コンボスコア振動レベルテーブル
const static GME_SCORE_VIB_LEVEL gm_ply_score_combo_vib_level_tbl[GMD_PLY_SCORE_COMBO_MAX] = {
	GME_SCORE_VIB_LEVEL_0,
	GME_SCORE_VIB_LEVEL_0,
	GME_SCORE_VIB_LEVEL_1,
	GME_SCORE_VIB_LEVEL_2,
	GME_SCORE_VIB_LEVEL_3,
};

/// コンボスコアスケールテーブル
const static fx32 gm_ply_score_combo_scale_tbl[GMD_PLY_SCORE_COMBO_MAX] = {
	(fx32)(FX32_ONE*1.0),
	(fx32)(FX32_ONE*1.0),
	(fx32)(FX32_ONE*1.2),
	(fx32)(FX32_ONE*1.5),
	(fx32)(FX32_ONE*2.0),
};

/* デバッグ */
#if defined (MTD_DEBUG)
/// シーケンス名
const char *g_gm_player_seq_name_tbl[GME_PLY_SEQ_STATE_MAX] = {
	"FW",					//!< SEQ : FW
	"WALK",					//!< SEQ : 歩き
	"TURN",					//!< SEQ : 振り向き
	"LOOKUP_ST",			//!< SEQ : 見上げ 開始
	"LOOKUP_M",				//!< SEQ : 見上げ 中
	"LOOKUP_END",			//!< SEQ : 見上げ 終了
	"SQUAT_ST",				//!< SEQ : しゃがみ 開始
	"SQUAT_M",				//!< SEQ : しゃがみ 中
	"SQUAT_END",			//!< SEQ : しゃがみ 終了
	"BRAKE",				//!< SEQ : ブレーキ
	"SPIN",					//!< SEQ : スピン
	"SPIN_DASHACC",			//!< SEQ : スピンダッシュ加速
	"SPIN_DASH",			//!< SEQ : スピンダッシュ
	"STAGGER_F",			//!< SEQ : よろける 前
	"STAGGER_B",			//!< SEQ : よろける 後ろ
	"STAGGER_D",			//!< SEQ : よろける 危険
	"FALL",					//!< SEQ : 落下
	"JUMP",					//!< SEQ : ジャンプ
	"WALL PUSH",			//!< SEQ : 壁押し
	"HOMING",				//!< SEQ : ホーミング
	"HOMING REF",			//!< SEQ : ホーミング 跳ね返り
	"JUMP_DASH",			//!< SEQ : ジャンプダッシュ
	"DAMAGE",				//!< SEQ : ダメージ
	"DEATH",				//!< SEQ : 死亡
	"TRANS SUPER",			//!< SEQ : スーパーソニック化
	"BOSS CLEAR",			//!< SEQ : ボスクリア
	"BOSS5 DEMO",			//!< SEQ : ボスFINAL演出FW
	"T RETRY FW",			//!< SEQ : タイムアタックリトライ FW
	"T RETRY ACC",			//!< SEQ : タイムアタックリトライ その場加速

	"SPRING JUMP",			//!< SEQ : スプリングジャンプ
	"ROCK RIDE START",		//!< SEQ : 岩乗り開始
	"ROCK RIDE",			//!< SEQ : 岩乗り
	"PULLEY HANG",			//!< SEQ : 滑車掴まり
	"BREATH",				//!< SEQ : 息継ぎ
	"DASH PANEL",			//!< SEQ : ダッシュパネル
	"TARZAN ROPE",			//!< SEQ : ターザンロープ
	"WATER_SLIDER",			//!< SEQ : ウォータースライダー
	"S_PIPE_SPIN",			//!< SEQ : Ｓ字パイプ
	"CORKSCREW",			//!< SEQ : コークスクリュー
	"DEMO FW",				//!< SEQ : デモ用フットワーク

	"STOPPER",				//!< SEQ : ストッパー＠ピンボール

	"CANNON",				//!< SEQ : 大砲
	"CANNON_SHOOT",			//!< SEQ : 大砲発射

	"UPBUMPER",				//!< SEQ : アップバンパー

	"SEESAW",				//!< SEQ : シーソー

	"PINBALL",				//!< SEQ : ピンボール
	"PINBALL AIR",			//!< SEQ : ピンボール（空中）
	"FLIPPER",				//!< SEQ : フリッパー
	"SPRING CTPLT HOLD",	//!< SEQ : スプリングカタパルト
	"SPRING CTPLT UP",		//!< SEQ : スプリングカタパルト↑
	"SPRING CTPLT LR",		//!< SEQ : スプリングカタパルト↑以外
	"FORCE SPIN",			//!< SEQ : 強制スピンモード
	"FORCE SPIN DEC",		//!< SEQ : 強制スピン 減速タイプ
	"FORCE SPIN FALL",		//!< SEQ : 強制スピン 落下

	"MOVE GEAR",			//!< SEQ : 移動歯車
	"DRAIN_TANK",			//!< SEQ : 排液装置
	"DRAIN_TANK_FALL",		//!< SEQ : 排液装置（落下）

	"STEAM PIPE",			//!< SEQ : スチームパイプ

	"POP STEAM JUMP",		//!< SEQ : ポップスチーム
	"SPL_RING_IN",			//!< SEQ : スペステリングＩＮ
	"BOSS2_CATCH",			//!< SEQ : ボス2掴み
	"F-BOSS_LANDING",		//!< SEQ : ボスFINAL地球割り着地振動
	"ENDING1",				//!< SEQ : エンディング 演出１
	"ENDING2",				//!< SEQ : エンディング 演出２
	"TRUCK_DANGER",			//!< SEQ : トロッコ危険状態
	"TRUCK_DANGER_RET",		//!< SEQ : トロッコ危険復帰
	"SPIN FALL",			//!< SEQ : スピンで落下
};
#endif

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// GmPlayerBuild
/*!
 *	プレイヤーデータ構築
 */
// ==========================================================================
void GmPlayerBuild(void)
{
#if 1
	// ゲームとは別途呼ぶ可能性があるので、個別に初期化

	s32						i;
	AMS_AMB_HEADER			*mdl_amb, *tex_amb;
	OBS_ACTION3D_NN_WORK	*obj_3d;
	
	// ソニックモデル
	mdl_amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_MDL].pData;
	tex_amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_TEX].pData;
	g_gm_ply_son_obj_3d_list = (OBS_ACTION3D_NN_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_NN_WORK) * mdl_amb->file_num);
	MI_CpuClear8(g_gm_ply_son_obj_3d_list, sizeof(OBS_ACTION3D_NN_WORK) * mdl_amb->file_num);

	obj_3d = g_gm_ply_son_obj_3d_list;
	for (i = 0; i < mdl_amb->file_num; i++, obj_3d++) {
		ObjAction3dNNModelLoad(obj_3d,
				NULL/*data_work*/, NULL/*file_name*/,
				i/*index*/, mdl_amb,
				NULL/*filename_tex*/, tex_amb,
				NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*NNF_DRAWOBJ drawflag*/);
	}

	// スーパーソニックモデル
	mdl_amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SSON_MDL].pData;
	tex_amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SSON_TEX].pData;
	g_gm_ply_sson_obj_3d_list = (OBS_ACTION3D_NN_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_NN_WORK) * mdl_amb->file_num);
	MI_CpuClear8(g_gm_ply_sson_obj_3d_list, sizeof(OBS_ACTION3D_NN_WORK) * mdl_amb->file_num);

	obj_3d = g_gm_ply_sson_obj_3d_list;
	for (i = 0; i < mdl_amb->file_num; i++, obj_3d++) {
		ObjAction3dNNModelLoad(obj_3d,
				NULL/*data_work*/, NULL/*file_name*/,
				i/*index*/, mdl_amb,
				NULL/*filename_tex*/, tex_amb,
				NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*NNF_DRAWOBJ drawflag*/);
	}

#if _WII
	// LODデータビルド
	GmPlyLodCnvAddr(g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_LOD].pData);
#endif

#else
	// ソニックモデル
	g_gm_ply_son_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_MODEL].pData,
								(AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_TEX].pData,
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
	// スーパーソニックモデル
	g_gm_ply_sson_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SSON_MODEL].pData,
								(AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SSON_TEX].pData,
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
#endif
}

// ==========================================================================
// GmPlayerFlush
/*!
 *	プレイヤーデータ開放
 */
// ==========================================================================
void GmPlayerFlush(void)
{
#if 1
	// ゲームとは別途呼ぶ可能性があるので、個別に開放
	s32						i;
	AMS_AMB_HEADER			*amb;
	OBS_ACTION3D_NN_WORK	*obj_3d;

	// オブジェクト開放
	// ソニック
	amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_MDL].pData;
	obj_3d = g_gm_ply_son_obj_3d_list;

	for (i = 0; i < amb->file_num; i++, obj_3d++) {
		ObjAction3dNNModelRelease(obj_3d);
	}

	// スーパーソニック
	amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SSON_MDL].pData;
	obj_3d = g_gm_ply_sson_obj_3d_list;

	for (i = 0; i < amb->file_num; i++, obj_3d++) {
		ObjAction3dNNModelRelease(obj_3d);
	}

#else
	AMS_AMB_HEADER	*amb;

	// オブジェクト開放
	// ソニック
	amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_MODEL].pData;
	GmGameDBuildRegFlushModel(g_gm_ply_son_obj_3d_list, amb->file_num);
	// スーパーソニック
	amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SSON_MODEL].pData;
	GmGameDBuildRegFlushModel(g_gm_ply_sson_obj_3d_list, amb->file_num);

	g_gm_ply_son_obj_3d_list = NULL;
	g_gm_ply_sson_obj_3d_list = NULL;
#endif
}

// ==========================================================================
// GmPlayerBuildCheck
/*!
 *	プレイヤーデータ構築 終了チェック
 */
// ==========================================================================
BOOL GmPlayerBuildCheck(void)
{
	s32						i;
	AMS_AMB_HEADER			*amb;
	OBS_ACTION3D_NN_WORK	*obj_3d;

	// ソニック
	amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_MDL].pData;
	obj_3d = g_gm_ply_son_obj_3d_list;
	for (i = 0; i < amb->file_num; i++, obj_3d++) {
		if (ObjAction3dNNModelLoadCheck(obj_3d) == FALSE) {
			return (FALSE);
		}
	}

	// スーパーソニック
	amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SSON_MDL].pData;
	obj_3d = g_gm_ply_sson_obj_3d_list;
	for (i = 0; i < amb->file_num; i++, obj_3d++) {
		if (ObjAction3dNNModelLoadCheck(obj_3d) == FALSE) {
			return (FALSE);
		}
	}

	return (TRUE);
}

// ==========================================================================
// GmPlayerFlushCheck
/*!
 *	プレイヤーデータ 片付け 終了チェック
 */
// ==========================================================================
BOOL GmPlayerFlushCheck(void)
{
	s32						i;
	AMS_AMB_HEADER			*amb;
	OBS_ACTION3D_NN_WORK	*obj_3d;

	// ソニック
	if (g_gm_ply_son_obj_3d_list) {
		amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SON_MDL].pData;
		obj_3d = g_gm_ply_son_obj_3d_list;
		for (i = 0; i < amb->file_num; i++, obj_3d++) {
			if (ObjAction3dNNModelReleaseCheck(obj_3d) == FALSE) {
				return (FALSE);
			}
		}

		amMemFree(g_gm_ply_son_obj_3d_list);
		g_gm_ply_son_obj_3d_list = NULL;
	}

	// スーパーソニック
	if (g_gm_ply_sson_obj_3d_list) {
		amb = (AMS_AMB_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_SSON_MDL].pData;
		obj_3d = g_gm_ply_sson_obj_3d_list;
		for (i = 0; i < amb->file_num; i++, obj_3d++) {
			if (ObjAction3dNNModelReleaseCheck(obj_3d) == FALSE) {
				return (FALSE);
			}
		}

		amMemFree(g_gm_ply_sson_obj_3d_list);
		g_gm_ply_sson_obj_3d_list = NULL;
	}

	return (TRUE);
}


// ==========================================================================
// データリリース
// ==========================================================================
// ==========================================================================
// GmPlayerRelease
/*!
 *	プレイヤー解放
 */
// ==========================================================================
void GmPlayerRelease(void)
{
	s32		i, player_num;

	for (player_num = 0; player_num < GSD_MAIN_PLAYER_MAX; player_num++) {
		for (i = 0; i < GMD_PLAYER_DATA_MAX; i++) {
			ObjDataRelease(&g_gm_player_data_work[player_num][i]);
		}
	}

#if defined (MTD_DEBUG)
	for (player_num = 0; player_num < GSD_MAIN_PLAYER_MAX; player_num++) {
		for (i = 0; i < GMD_PLAYER_DATA_MAX; i++) {
			if (g_gm_player_data_work[player_num][i].num) {
				MTM_ASSERT(!"gmPlayer.cpp::GmPlayerRelease() player data no release\n");
			}
		}
	}
#endif
}


// ================================================================
// プレイヤー生成
// ================================================================
// ================================================================
// GmPlayerInit
/*!
 *	プレイヤーオブジェクト初期化関数
 *
 *	@param	char_id		[in]	プレイヤーキャラクタータイプ GSE_CHAR_ID_****
 *	@param	ctrl_id		[in]	コントローラーID
 *	@param	player_id	[in]	プレイヤーID
 *	@param	camera_id	[in]	カメラID
 *
 *	@return	プレイヤーオブジェクトワーク
 */
// ================================================================
GMS_PLAYER_WORK* GmPlayerInit(GSE_CHAR_ID char_id, u16 ctrl_id, u16 player_id, u16 camera_id)
{
	s32					i;
	OBS_OBJECT_WORK		*obj_work;
	GMS_PLAYER_WORK		*ply_work;

	const u16		atk_flag[GMD_PLAYER_RECT_NUM] = {0,															// くらい
											GMD_OBJ_RECT_ATK_FLAG_NORMALATK | GMD_OBJ_RECT_ATK_FLAG_EFCTATK,	// 攻撃
											GMD_OBJ_RECT_ATK_FLAG_BODYATK};			// 体
	// 防御フラグ
	const u16		def_flag[GMD_PLAYER_RECT_NUM] = {GMD_OBJ_RECT_DEF_FLAG_WEAK_NORMALATK,	// くらい
											GMD_OBJ_RECT_DEF_FLAG_NORMALATK,				// 攻撃
											GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK};			// 体

	if (char_id >= GSD_CHAR_ID_MAX) {
		MTM_ASSERT((u32)char_id < GSD_CHAR_ID_MAX);
		char_id = (GSE_CHAR_ID)0;
	}
    //MTM_ASSERT(ctrl_id < AMD_PAD_PORT_MAX);	コントローラーIDは自動取得 環境によって最大数が違うのでここではチェックしない

#if defined(MTD_DEBUG)  // デバッグ版
    OS_TPrintf( "● PlayerInit ●\n" );
#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版

	/* プレイヤーオブジェクトタスク作成 */
	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(GMD_TASK_PRIO_PLAYER, GMD_TASK_GROUP_PLAYER, GMD_TASK_PAUSELEVEL_DEF/*pause_level*/,
					GMD_TASK_PAUSELEVEL_DEF/*obj_pause_level*/, sizeof(GMS_PLAYER_WORK), "PLAYER OBJ");

	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmPlayerExit);

	// 開放待機処理設定
	obj_work->ppUserRelease		= gmPlayerObjRelease;
	obj_work->ppUserReleaseWait	= gmPlayerObjReleaseWait;

	/* ワーク初期化 */
    ply_work = (GMS_PLAYER_WORK*)obj_work;

	/* プレイヤーワーク設定 */
    // プレイヤーオブジェクトID設定(自分 : 0, 対戦相手・ゴースト等 : 1～)
	ply_work->player_id = (u8)player_id;
	ply_work->camera_no = (u8)camera_id;

    // コントローラID設定
	ply_work->ctrl_id = (u8)ctrl_id;

    // プレイヤーキャラクタータイプ設定
    ply_work->char_id = (u8)char_id;

	// アクションステートクリア
	ply_work->act_state		= GME_PLY_ACT_STATE_INVALID;
	ply_work->prev_act_state= GME_PLY_ACT_STATE_INVALID;

	// 演出用MTX初期化
	nnMakeUnitMatrix(&ply_work->ex_obj_mtx_r);

	/* 描画設定 */
#if defined(MTD_DEBUG)  // デバッグ版
	OS_TPrintf( "PlayerModelSetup\n" );
#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版

//	g_obj.drawflag |= NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND;
//	g_obj.load_drawflag |= NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND;

//	g_obj.drawflag |= (NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT);
//	g_obj.drawflag |= (NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND);
//	g_obj.drawflag |= NND_DRAWOBJ_MATCTRL_TEXOFFSET;

	/* モデル設定 */
	GmPlayerInitModel(ply_work);		// char_id に従って、モデルとモーションをセットする

	/* キーマップ設定 */
//#if !_WII
#if 1
    ply_work->key_map[GMD_PLAYER_KEY_MAP_UP]      = KEY_L_UP;	// 標準設定
    ply_work->key_map[GMD_PLAYER_KEY_MAP_DOWN]    = KEY_L_DOWN;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_LEFT]    = KEY_L_LEFT;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_RIGHT]   = KEY_L_RIGHT;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_A]       = KEY_R_DOWN;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_B]       = KEY_R_RIGHT;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_X]       = KEY_R_LEFT;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_Y]       = KEY_R_UP;
#else
    ply_work->key_map[GMD_PLAYER_KEY_MAP_UP]      = KEY_L_LEFT;	// 標準設定
    ply_work->key_map[GMD_PLAYER_KEY_MAP_DOWN]    = KEY_L_RIGHT;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_LEFT]    = KEY_L_DOWN;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_RIGHT]   = KEY_L_UP;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_A]       = KEY_R_DOWN;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_B]       = KEY_R_RIGHT;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_X]       = KEY_R_LEFT;
    ply_work->key_map[GMD_PLAYER_KEY_MAP_Y]       = KEY_R_UP;
#endif

    /* オブジェクトワーク初期設定 */
	obj_work->obj_type	= GMD_OBJTYPE_PLAYER;
	obj_work->flag |= OBD_OBJECT_NOCLIP;
	obj_work->flag |= OBD_OBJECT_B;

	// スケール初期化

	// 処理関数登録
	obj_work->ppOut		= gmPlayerDispFunc;
	if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// 通常
		obj_work->ppIn		= gmPlayerDefaultInFunc;
	} else {
		// SpecialStage
		obj_work->ppIn		= gmPlayerSplStgInFunc;
	}
//	obj_work->ppActCall	= gmPlayerActionCallBack;
//    ply_work->obj_work.ppRec = gmPlayerRectSet;
	obj_work->ppLast	= gmPlayerDefaultLastFunc;
	obj_work->ppMove	= GmPlySeqMoveFunc;
	//obj_work->ppRec		= ;
	obj_work->disp_flag |= OBD_DISP_3D_LOCK_LIGHT;   // 絶えずライトを正面に向ける

	// 描画オフセット設定
	//ply_work->disp_ofst_y = GMD_PL_DISP_OFST_Y;

	/* プレイヤーシーケンスステータス設定 */
	GmPlySeqSetSeqState(ply_work);

    /* 状態初期化 */
    GmPlayerStateInit(ply_work);

#if 0
    /* プレイヤーアクション関数設定 */
    ply_work->ppFw		= GmPlayerFwInit;          // フットワーク 必須
    ply_work->ppWalk	= GmPlayerWalkInit;        // 歩き
    ply_work->ppSquat	= gmPlayerSquatInit;       // しゃがみ
    ply_work->ppJump	= gmPlayerJumpInit;        // ジャンプ
    if ( usCharacter == GSD_PLAYER_BLAZE ) {
        ply_work->ppJumpAtk = gmPlayerBlazeHoverInit; // ホバー
	}
    else {
        ply_work->ppJumpAtk = gmPlayerSonicHomingInit; // ホーミング＆ジャンプダッシュ
	}
#endif

	/* 地形あたり矩形設定 */
	//ObjObjectFieldRectSet(obj_work, -8/*left*/, -32/*top*/, 8/*right*/, 0/*bottom*/);
	ObjObjectFieldRectSet(obj_work, GMD_PL_REC_L, GMD_PL_REC_T, GMD_PL_REC_R, GMD_PL_REC_B);

	/* 矩形 */
	// 矩形設定
	ObjObjectGetRectBuf(obj_work, ply_work->rect_work, GMD_PLAYER_RECT_NUM);	// 自動登録バッファに設定
	for (i = 0; i < GMD_PLAYER_RECT_NUM; i++) {
		ObjRectGroupSet(&ply_work->rect_work[i], GMD_OBJ_RECT_GROUP_PLAYER, GMD_OBJ_RECT_TARGET_GROUPFLAG_ENEMY);
		ObjRectAtkSet(&ply_work->rect_work[i], atk_flag[i], GMD_OBJ_RECT_ATK_POWER_DEFAULT);
		ObjRectDefSet(&ply_work->rect_work[i], def_flag[i], GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		ply_work->rect_work[i].parent_obj = obj_work;
		ply_work->rect_work[i].flag &= ~OBD_RECT_ENABLE;	// はじめは無効

		// 奥行きを仮設定
		ply_work->rect_work[i].rect.back = -16;
		ply_work->rect_work[i].rect.front = 16;
	}
	ply_work->rect_work[GMD_PLAYER_RECT_DEF].ppDef = gmPlayerDefFunc;	// 防御処理
	ply_work->rect_work[GMD_PLAYER_RECT_ATK].ppHit = gmPlayerAtkFunc;	// 攻撃処理
	ply_work->rect_work[GMD_PLAYER_RECT_DEF].flag |= OBD_RECT_NODAMAGE;
	ply_work->rect_work[GMD_PLAYER_RECT_ATK].flag |= OBD_RECT_GROUP;
	ply_work->rect_work[GMD_PLAYER_RECT_BODY].flag |= OBD_RECT_GROUP | OBD_RECT_NODAMAGE | OBD_RECT_NOHIT_UP;
	// ◆仮矩形
	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// スペステ用矩形サイズ設定
		ObjObjectFieldRectSet(obj_work, -7 , -8 , 7, 10);
		ObjRectWorkZSet(&ply_work->rect_work[GMD_PLAYER_RECT_BODY],
							-11, -11, -500,
							 11,  11,  500);
		ObjRectWorkZSet(&ply_work->rect_work[GMD_PLAYER_RECT_DEF],
							-12, -12, -500,
							 12,  12,  500);
		ObjRectWorkZSet(&ply_work->rect_work[GMD_PLAYER_RECT_ATK],
							-13, -13, -500,
							 13,  13,  500);
	} else {
		// 通常矩形サイズ設定
		ObjRectWorkZSet(&ply_work->rect_work[GMD_PLAYER_RECT_BODY],
							-8, -32 + GMD_PL_REC_B, -500,
							8, 0 + GMD_PL_REC_B, 500);
		ObjRectWorkZSet(&ply_work->rect_work[GMD_PLAYER_RECT_DEF],
							-8, -32 + GMD_PL_REC_B, -500,
							8, 0 + GMD_PL_REC_B, 500);
		ObjRectWorkZSet(&ply_work->rect_work[GMD_PLAYER_RECT_ATK],
							-16, -32 + GMD_PL_REC_B, -500,
							16, 0 + GMD_PL_REC_B, 500);
	}
	ply_work->rect_work[GMD_PLAYER_RECT_ATK].flag &= ~OBD_RECT_ENABLE;	// はじめは無効
	
#if _IPHONE
	ObjRectWorkZSet(&gm_ply_touch_rect[GME_PLAYER_TOUCH_RECT_F], -16, -64 + GMD_PL_REC_B, -500,
					64, 24 + GMD_PL_REC_B, 500);
	ObjRectWorkZSet(&gm_ply_touch_rect[GME_PLAYER_TOUCH_RECT_B], -64, -64 + GMD_PL_REC_B, -500,
					-16, 24 + GMD_PL_REC_B, 500);
	
	ply_work->calc_accel.x = 0.0f;
	ply_work->calc_accel.y = 0.0f;
	ply_work->calc_accel.z = 0.0f;
	
	ply_work->control_type = GME_PLAYER_CONTROL_TYPE_TILT;
	ply_work->jump_rect = (u16*)gm_player_push_jump_key_rect[GME_PLAYER_CONTROL_TYPE_TILT]; // 本当はいらない
	ply_work->ssonic_rect = (u16*)gm_player_push_ssonic_key_rect[GME_PLAYER_CONTROL_TYPE_TILT];
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
		ply_work->control_type = GME_PLAYER_CONTROL_TYPE_A;
		ply_work->jump_rect = (u16*)gm_player_push_jump_key_rect[GME_PLAYER_CONTROL_TYPE_A];
		ply_work->ssonic_rect = (u16*)gm_player_push_ssonic_key_rect[GME_PLAYER_CONTROL_TYPE_A];
	}
	// SS用
	ply_work->accel_counter = 0;
	ply_work->dir_vec_add = 0;
	// SE用
	ply_work->spin_se_timer = 0;
	ply_work->spin_back_se_timer = 0;
#if GMD_PLY_SAFE_TOUCH_SPIN
	ply_work->safe_timer = 0;
	ply_work->safe_jump_timer = 0;
	ply_work->safe_spin_timer = 0;
#endif // GMD_PLY_SAFE_TOUCH_SPIN
#endif // _IPHONE


	/* 矩形初期登録 */
#if 0
	if (!(main_info->game_flag & GSD_GAME_FLAG_WM && ply_work->player_id)) {
		// 自プレイヤー
		// 体矩形
		ObjObjectRectSet(&ply_work->obj_work, &ply_work->rect_work[0], 0, TRUE);	// 矩形自動登録

		// 攻撃矩形
		ObjObjectRectSet(&ply_work->obj_work, &ply_work->rect_work[1], 1, TRUE);	// 矩形自動登録
	}
	else {
		// 攻撃矩形
		ObjObjectRectSet(&ply_work->obj_work, &ply_work->rect_work[1], 1, TRUE);	// 矩形自動登録

		// 通信対戦相手の体押し合い用矩形設定
		ObjObjectRectSet(&ply_work->obj_work, &ply_work->rect_work[2], 2, TRUE);	// 矩形自動登録

		// オブジェクト地形圧死OFF
		ply_work->player_flag |= GMD_PLF_NOCOLDIE;
		ply_work->obj_work.move_flag |= OBD_MOVE_NOCOLOBJ;
	}
	/* 標準くらい処理設定 */
	ply_work->rect_work[0].ppDef = GmPlayerDamageReactionInit;
#endif



#if 0

    /* 座標や状態など初期化 */
    if ( (main_info->game_flag & GSD_GAME_FLAG_SUSPEND) && !ply_work->player_id ) {
    /* 情報復帰 */
		// 中断後再開時 自分キャラのみ
#if defined(MTD_DEBUG)  // デバッグ版
        OS_TPrintf( "PlayerSuspend\n" );
#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版

        // スペステから移行時は自キャラの各状態を設定する
        // リング
        ply_work->ring_num			= main_info->suspend_ply_ring_num;
        ply_work->ring_stage_num	= main_info->suspend_ply_ring_stg_num;

        // タイム
        g_gm_main_system.time = main_info->suspend_time;

        // バリア
		switch (main_info->suspend_barrier) {
		case 1:
            ply_work->player_flag |= GMD_PLF_BARRIER;
			GmEffectInitPlayerBarrier(ply_work);
            break;
		case 2:
            ply_work->player_flag |= GMD_PLF_MAGNET;
			GmEffectInitPlayerMagnetBarrier(ply_work);
            break;
		}
        // テンション
        ply_work->tension = main_info->suspend_tension;

        // 座標
        ply_work->obj_work.pos.x = main_info->suspend_ply_pos_x;
        ply_work->obj_work.pos.y = main_info->suspend_ply_pos_y;

        // ギミック生成を行う
        ply_work->gmk_flag |= GMD_PLGF_GMK_CREATE;
        
        // 一定時間無敵
        ply_work->invincible_timer = ply_work->time_damage;
        ply_work->rect_work[0].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
        //ply_work->rect_work[0].usDefFlag |= OBD_HIT_POWER_5;

#if 0	// ◆
        // 暗い時なら全体照明を元にもどす
        if ( _nl_game.sSSLight ){
            NL_MapEachStageBldOff( (1 << MTE_GE2_A) | (1 << MTE_GE2_B),  8);
            ply_work->gmk_flag |= GMD_PLGF_GMK_LIGHT_OFF;
        }
#endif	// 0
    }
	else {
	/* 通常初期化 */
		// 初期座標設定
        if (!( g_gm_main_system.resume_pos_x || g_gm_main_system.resume_pos_y )) {
            g_gm_main_system.resume_pos_x = 40 << FX32_SHIFT;
            g_gm_main_system.resume_pos_y = 120 << FX32_SHIFT;
        }
        ply_work->obj_work.pos.x = g_gm_main_system.resume_pos_x;
        ply_work->obj_work.pos.y = g_gm_main_system.resume_pos_y;

		// 下方向HITにしておく
		ply_work->obj_work.move_flag |= OBD_MOVE_UNDER;

		// 初期テンション値
        if ( !GmMainIsBossStage() ) {
            ply_work->tension = (s16)( 100 << GMD_PLAYER_TENSION_SHIFT );
		}

        // 即逆重力の時は向きを反転
        if ( GmReverseCheck( ply_work->obj_work.pos.x, ply_work->obj_work.pos.y ) ) {
            ply_work->obj_work.disp_flag ^= OBD_DISP_HFLIP;
        }

    }
#endif

	/* 通信対戦時追加初期化 */
//	if (main_info->game_flag & GSD_GAME_FLAG_WM) {
//		ply_work->invincible_timer = GMD_PL_WM_START_INVINCIBLE_TIME;	// 初期無敵時間
//	}

	// 自分プレイヤーは座標保持
	if (!ply_work->player_id) {
        gm_pos_x = (ply_work->obj_work.pos.x >> FX32_SHIFT);
        gm_pos_y = (ply_work->obj_work.pos.y >> FX32_SHIFT);
        gm_pos_z = (ply_work->obj_work.pos.z >> FX32_SHIFT);
	}

	/* メイン処理設定 */
	obj_work->ppFunc = gmPlayerMain;

	/* Fwシーケンスへ */
	GmPlySeqChangeFw(ply_work);

#if 0
	/* ステージタイプ別設定 */
    if ( GmMainIsBossStage() ) {	// ボスステージ
		// 通常3D, ライトステージ固定
		ply_work->obj_work.disp_flag &= ~( OBD_DISP_3D_PARALLEL | OBD_DISP_3D_LOCK_LIGHT );
	}

    /* 通信時設定 */
    if (main_info->game_flag & GSD_GAME_FLAG_WM) {
		// 初期リング1
        ply_work->ring_num = 1;
    }

	/* 対戦相手・ゴースト設定 */
	if (ply_work->player_id) {	// 対戦相手プレイヤー
		// アイコン初期化
		GmEffectInitContestPlayerIcon(ply_work);
	}
#endif

	// ◆初期位置仮
	//ply_work->obj_work.pos.x = 128*FX32_ONE;
	//ply_work->obj_work.pos.y = 432*FX32_ONE;
	ply_work->obj_work.pos.x = g_gm_main_system.resume_pos_x;
	ply_work->obj_work.pos.y = g_gm_main_system.resume_pos_y;

	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_2_2) {
		// ピンボール化
		GmPlayerSetPinballSonic(ply_work);
	} else
	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// スペステピンボール
		GmPlayerSetSplStgSonic(ply_work);
	}
	

	return (ply_work);
}

// ================================================================
// GmPlayerResetInit
/*!
  プレイヤー状態初期化（死亡時など

  @param ply_work   [in] 対象プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerResetInit(GMS_PLAYER_WORK *ply_work)
{
//	GSS_MAIN_SYS_INFO	*main_info = GsGetMainSysInfo();

	// キー入力不可を落としておく
	ply_work->player_flag &= ~GMD_PLF_NOKEY;

#if 0
    // 対戦の時はストックチェックや、ゲームオーバー移行、イベント復活を行わない
    if ( main_info->game_mode != GSD_GAME_MODE_CONTEST && !(ply_work->gmk_flag & GMD_PLGF_GMK_WARP) ) {	// 対戦, ワープ処理時は除く
		// プレイヤー死亡処理終了
		g_gm_main_system.flag |= GMD_MAIN_FLAG_PLAYER_DIE;
		g_gm_main_system.clear_state_flag |= GMD_MAIN_CLEAR_STATE_FLAG_PLAYER_DIE;
		return;
    }
	else {
        // マップ関連再初期化
//        GmMapInitNetRestart();
        ply_work->gmk_flag |= GMD_PLGF_GMK_CREATE;

	    // マップ表示
//	    g_gm_map.camera[ply_work->camera_no].flag |= GMD_MAP_CAM_FLAG_DISP_A;
//	    g_gm_map.camera[ply_work->camera_no].flag |= GMD_MAP_CAM_FLAG_DISP_B;

	    // プレイヤー位置リセット
	    ply_work->obj_work.pos.x = g_gm_main_system.resume_pos_x;
	    ply_work->obj_work.pos.y = g_gm_main_system.resume_pos_y;
    }

	/* 状況別設定 */
    if ( !( ply_work->gmk_flag & GMD_PLGF_GMK_WARP) ) {
	// ワープの時はやらない
        ply_work->slow_timer = 0;
        ply_work->obj_work.dir_fall  = 0;
        ply_work->confusion_timer = 0;

		// キーマップ初期化
        ply_work->key_map[GMD_PLAYER_KEY_MAP_UP]      = KEY_L_UP;	// 標準設定◆
        ply_work->key_map[GMD_PLAYER_KEY_MAP_DOWN]    = KEY_L_DOWN;
        ply_work->key_map[GMD_PLAYER_KEY_MAP_LEFT]    = KEY_L_LEFT;
        ply_work->key_map[GMD_PLAYER_KEY_MAP_RIGHT]   = KEY_L_RIGHT;

        // フラグ初期化
        ply_work->player_flag  &= ~(GMD_PLF_BARRIER | GMD_PLF_MAGNET);
        ply_work->obj_work.flag   |= OBD_OBJECT_B;

        // タイマ初期化
        ply_work->nitro_timer = 0;
        ply_work->multi_nohit_timer = 0;
        ply_work->genocide_timer = 0;
        ply_work->invincible_timer = 0;
        ply_work->disapprove_item_catch_timer = 0;
        ply_work->hs_trick_timer = 0;

        // テクスチャパレット初期化
        //ObjPaletteTexVary( &ply_work->tex_plt, 0,0,0);

        // リング初期化
        ply_work->ring_num = 0;

        if ((main_info->game_flag & GSD_GAME_FLAG_WM) ) {
		// 通信対戦時
            ply_work->ring_num = 1;
			ply_work->invincible_timer = GMD_PL_WM_RESTART_INVINCIBLE_TIME;
		}
    }
	else {
	// ワープの時だけ行う
        ply_work->gmk_flag &= ~GMD_PLGF_GMK_CREATE;		// ワープの時は後で生成

        // 座標ワープ
        ply_work->obj_work.pos.x = ply_work->warp_pos_x;
        ply_work->obj_work.pos.y = ply_work->warp_pos_y;
        ply_work->obj_work.pos.z = 0;

        ply_work->warp_pos_x = 0;
        ply_work->warp_pos_y = 0;
        // 一定時間無敵
        ply_work->invincible_timer = ply_work->time_damage;
        ply_work->disapprove_item_catch_timer = ply_work->invincible_timer;
    }
#endif

    // ベルトスクロール解除
    g_obj.flag &= ~OBD_OBJ_BELT;
    // オブジェクトglobal設定初期化
    g_obj.scroll[MTD_X] =
    	g_obj.scroll[MTD_Y] = 0x0000;

	// フラグ初期化
    ply_work->player_flag &= ~(GMD_PLF_COLDIE | GMD_PLF_NOCOLDIE | /*GMD_PLF_NITRO | GMD_PLF_BOOST | */GMD_PLF_NOKEY);
    ply_work->gmk_flag &= ~(GMD_PLGF_GMK_NEON_A |
							GMD_PLGF_GMK_MAP_LIMIT_LCD_X | GMD_PLGF_GMK_MAP_LIMIT_LCD_Y |
							GMD_PLGF_MAP_LIMIT_LCD_X | GMD_PLGF_MAP_LIMIT_LCD_X |
							GMD_PLGF_BELT | GMD_PLGF_GMK_HOLD_POS_Z |
							//GMD_PLGF_GMK_WALLED |
							//GMD_PLGF_GMK_LIGHT_OFF | GMD_PLGF_GMK_LIGHT_OFF_CHK | GMD_PLGF_GMK_WALL | GMD_PLGF_GMK_WALL_END | 
							GMD_PLGF_CAMERA_GMK_X | GMD_PLGF_CAMERA_GMK_X_FIX |
							GMD_PLGF_CAMERA_GMK_Y | GMD_PLGF_CAMERA_GMK_Y_FIX |
							GMD_PLGF_CAMERA_CENTER_OFST);

//	if (!GmMainIsCreateDHB()) {
//	   ply_work->gmk_flag &= ~GMD_PLGF_DH_BOARD;
//	}

	// プレイヤー中心へ移動完了◆
//    GmMapCamExternOpExitX((MTE_GE2_TYPE)ply_work->camera_no);
//    GmMapCamExternOpExitY((MTE_GE2_TYPE)ply_work->camera_no);

    // タイマ等初期化
    //ply_work->lFrontWallPosY = 0;
    //ply_work->lFrontWallPosZ = 0;
//    ply_work->tension_combo_timer = 0;
//    ply_work->tension_combo = 0;

    // グラインド初期化
    ply_work->graind_id			= 0;
    ply_work->graind_prev_ride	= 0;
    ply_work->gmk_flag &= ~GMD_PLGF_GRAIND_HITCHECK;

	// スピードプールクリア
    ply_work->spd_pool = 0;

	// スケール初期化
	ply_work->obj_work.scale.x =
	    ply_work->obj_work.scale.y =
	    ply_work->obj_work.scale.z = FX32_ONE;

	// プレイヤーステート初期化
    GmPlayerStateInit(ply_work);
    // ギミックステータスクリア
    GmPlayerStateGimmickInit(ply_work);
}

// ================================================================
// GmPlayerInitModel
/*!
 *	プレイヤーオブジェクト モデルセット
 *
 *	@param	ply_work		[in]	プレイヤーワーク
 *
 *	@note
 *		char_idに従い、モデルデータとモーションデータを設定します。
 */
// ================================================================
void GmPlayerInitModel(GMS_PLAYER_WORK *ply_work)
{
	s32						i, j;
	OBS_ACTION3D_NN_WORK	*obj_3d_list;
	OBS_OBJECT_WORK			*obj_work;
	OBS_ACTION3D_NN_WORK	*obj_3d;

	MTM_ASSERT(ply_work->obj_3d_work[0].motion == NULL);

	obj_work	= &ply_work->obj_work;

	// モデル初期化
	obj_3d		= &ply_work->obj_3d_work[0];
	for (i = 0; i < GMD_PLY_MODEL_SET_MAX; i++) {
		obj_3d_list = *gm_ply_obj_3d_list_tbl[ply_work->char_id][i];
		for (j = 0; j < GMD_PLY_MODEL_TYPE_MAX; j++, obj_3d++) {
			// 3Dアクション初期化
			ObjCopyAction3dNNModel(&obj_3d_list[j], obj_3d);

			// モーションブレンド速度設定
			obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD;

			// トゥーン設定
			ObjDrawSetToon(obj_3d);
#if _WII
			// Wiiは専用マテリアルコールバック設定
			obj_3d->material_cb_func = gmPlayerMaterialCallback;
			// モーションコールバック設定
			obj_3d->mtn_cb_func = gmGmkPlayerMotionCallback;
			obj_3d->mtn_cb_param = ply_work;
			// ロッドモーションデータ設定
			ply_work->hand_lod_header = (GMS_PLY_LOD_MTN_HEADER*)g_gm_player_data_work[GSD_MAIN_PLAYER_1P][GMD_PLAYER_DATA_LOD].pData;

			if (i == GMD_PLY_MODEL_SET_SUPER) {
				// スーパーソニック用トゥーンライト
				obj_3d->toon_light.x = 0.0f;
				obj_3d->toon_light.y = -0.28f;
				obj_3d->toon_light.z = -1.11f;
			}
#endif

			// 専用ライト設定
			obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
			obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
		}
	}

	// 3D描画ワークを設定
	obj_work->obj_3d = &ply_work->obj_3d_work[0];
	// モデル解放無しフラグ設定
	obj_work->flag |= OBD_OBJECT_NORELEASE_3D;


	// モーションロード
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									&g_gm_player_data_work[ply_work->player_id][GMD_PLAYER_DATA_MOTION], NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/,
									GMD_PLAYER_MTN_NUM/*motion_num*/, AMD_MOTION_MATERIAL_DEFAULT_MAX);

	obj_work->disp_flag |= OBD_DISP_3D_PARALLEL | OBD_DISP_USERMTX_RIGHT;

	// モーションワークコピー
	for (i = 1; i < GMD_PLY_MODEL_TYPE_MAX*GMD_PLY_MODEL_SET_MAX; i++) {
		ply_work->obj_3d_work[i].motion = ply_work->obj_3d_work[0].motion;
	}

	// 初期使用モデル設定
	GmPlayerSetModel(ply_work, GMD_PLY_MODEL_SET_NORMAL);
}

// ================================================================
// GmPlayerSetModel
/*!
 *	プレイヤーオブジェクト モデルセット
 *
 *	@param	ply_work		[in]	プレイヤーワーク
 *	@param	model_set		[in]	設定するモデルセット
 */
// ================================================================
void GmPlayerSetModel(GMS_PLAYER_WORK *ply_work, GME_PLY_MODEL_SET model_set)
{
#if 1
	GME_PLY_ACT_STATE	act_state;

	MTM_ASSERT((u32)model_set < GMD_PLY_MODEL_SET_MAX);

	// 使用モデル設定
	ply_work->obj_3d[GMD_PLY_MODEL_TYPE_NORMAL] =
			&ply_work->obj_3d_work[model_set*GMD_PLY_MODEL_TYPE_MAX + GMD_PLY_MODEL_TYPE_NORMAL];
	ply_work->obj_3d[GMD_PLY_MODEL_TYPE_SPIN] =
			&ply_work->obj_3d_work[model_set*GMD_PLY_MODEL_TYPE_MAX + GMD_PLY_MODEL_TYPE_SPIN];
	ply_work->obj_3d[GMD_PLY_MODEL_TYPE_IPHONE] =
			&ply_work->obj_3d_work[model_set*GMD_PLY_MODEL_TYPE_MAX + GMD_PLY_MODEL_TYPE_IPHONE];
	ply_work->obj_3d[GMD_PLY_MODEL_TYPE_SPINJUMP] =
			&ply_work->obj_3d_work[model_set*GMD_PLY_MODEL_TYPE_MAX + GMD_PLY_MODEL_TYPE_SPINJUMP];

	// 描画モデル設定
	act_state = ply_work->act_state;
	if (act_state == GME_PLY_ACT_STATE_INVALID) {
		act_state = (GME_PLY_ACT_STATE)0;
	}
	ply_work->obj_work.obj_3d =
		ply_work->obj_3d[*(g_gm_player_model_tbl[ply_work->char_id] + act_state)];
#else
	s32						i;
	OBS_ACTION3D_NN_WORK	*obj_3d_list;
	OBS_OBJECT_WORK			*obj_work;

	obj_work = &ply_work->obj_work;

	if (ply_work->obj_3d[0].motion) {
		// モーションを開放
		// 使用しているモーションワークは1つのみ(その他はコピー)
		ObjAction3dNNMotionRelease(&ply_work->obj_3d[0]);
		ply_work->obj_3d[0].motion = NULL;
		ply_work->obj_3d[1].motion = NULL;
	}

	// モデル
	obj_3d_list = *gm_ply_obj_3d_list_tbl[ply_work->char_id];
	for (i = 0; i < GMD_PLY_MODEL_TYPE_MAX; i++) {
		// 3Dアクション初期化
		ObjCopyAction3dNNModel(&obj_3d_list[i], &ply_work->obj_3d[i]);

		// モーションブレンド速度設定
		ply_work->obj_3d[i].blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD;
	}

	// 3D描画ワークを設定
	obj_work->obj_3d = &ply_work->obj_3d[0];
	// モデル解放無しフラグ設定
	obj_work->flag |= OBD_OBJECT_NORELEASE_3D;

	// トゥーン設定
	ObjDrawSetToon(&ply_work->obj_3d[0]);
	ObjDrawSetToon(&ply_work->obj_3d[1]);


	// モーションロード
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									&g_gm_player_data_work[ply_work->player_id][GMD_PLAYER_DATA_MOTION], NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/,
									128/*motion_num*/, AMD_MOTION_MATERIAL_DEFAULT_MAX);

	obj_work->disp_flag |= OBD_DISP_3D_PARALLEL | OBD_DISP_USERMTX_RIGHT;

	// モーションワークコピー
	ply_work->obj_3d[1].motion = ply_work->obj_3d[0].motion;
#endif
}

// =====================================================================
// プレイヤーステータス パラメータ設定
// =====================================================================
// ================================================================
// GmPlayerStateInit
/*!
  プレイヤー状態初期化（ダメージ時など

  @param ply_work   [in] 対象プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerStateInit(GMS_PLAYER_WORK *ply_work)
{

#if 0	// ◆水ステージ
    if ( _nl_global.zone == 1 ){
        // 水中チェック
        if ( g_gm_map.camera[ply_work->camera_no].water_level ){
            if ( (ply_work->obj_work.pos.y >> FX32_SHIFT) > g_gm_map.camera[ply_work->camera_no].water_level ){
                ply_work->player_flag |= GMD_PLF_WATER;
            }
            else{
                ply_work->player_flag &= ~GMD_PLF_WATER;
            }
        }else{
            ply_work->player_flag &= ~GMD_PLF_WATER;
        }
    }
#endif	// 0

    /* プレイヤーアクション関数設定 */
//    ply_work->ppFw		= GmPlayerFwInit;          // フットワーク 必須
//    ply_work->ppWalk	= GmPlayerWalkInit;        // 歩き
//    ply_work->ppSquat	= gmPlayerSquatInit;       // しゃがみ
//    ply_work->ppJump	= gmPlayerJumpInit;        // ジャンプ
//	ply_work->ppJumpAtk = gmPlayerSonicHomingInit; // ホーミング＆ジャンプダッシュ

	/* シーケンステーブル再設定 */
	//ply_work->seq_init_tbl = &g_gm_ply_seq_init_tbl[ply_work->char_id][0];
	ply_work->seq_init_tbl = &(g_gm_ply_seq_init_tbl_list[ply_work->char_id])[0];

//    // 関数設定初期化 (変える場合がある分だけ)
//	//ply_work->obj_work.ppOut		= gmPlayerDispFunc;
//	ply_work->obj_work.ppIn			= gmPlayerDefaultInFunc;
//	ply_work->obj_work.ppActCall	= gmPlayerActionCallBack;
//    //ply_work->obj_work.ppLast		= gmPlayerDefaultLastFunc;
//	//ply_work->obj_work.ppRec = gmPlayerRectSet;



#if 0
    /* 矩形設定 */
	// 地形あたり
    ply_work->obj_work.field_rect[OBD_LEFT   ] = GMD_PL_REC_L -1;
    ply_work->obj_work.field_rect[OBD_RIGHT  ] = GMD_PL_REC_R +1;
    ply_work->obj_work.field_rect[OBD_TOP    ] = GMD_PL_REC_T;
    ply_work->obj_work.field_rect[OBD_BOTTOM ] = GMD_PL_REC_B;

	// 体矩形
//	ObjObjectRectSet(&ply_work->obj_work, &ply_work->rect_work[0], 0, TRUE);	// 矩形自動登録
//	ply_work->rect_work[0].ppDef		= GmPlayerDamageReactionInit;	// GmPlayerInitでじかに設定 GmPlayerStateGimmickInitでクリア
    ply_work->rect_work[0].user_flag	= GMD_OBJ_RECT_GROUP_PLAYER_SET;
	ply_work->rect_work[0].flag			&= ~(OBD_RECT_HIT | OBD_RECT_DAMAGE);
	ply_work->rect_work[0].flag			|= OBD_RECT_GROUP;
    if ( !ply_work->invincible_timer ) {
    	ObjRectAtkSet(&ply_work->rect_work[0], GMD_OBJ_RECT_HIT_FLAG_BODY, GMD_OBJ_RECT_HIT_POWER_DEFAULT);
    	ObjRectDefSet(&ply_work->rect_work[0], 0, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	}

	if (!(GsGetMainSysInfo()->game_flag & GSD_GAME_FLAG_WM && ply_work->player_id)) {
	// 通常
		// 攻撃矩形
		//ply_work->rect_work[1].ppDef		= NULL;
		ply_work->rect_work[1].user_flag	= GMD_OBJ_RECT_GROUP_PLAYER_SET;
	    ply_work->rect_work[1].flag			&= ~(OBD_RECT_ATK_FUNC | OBD_RECT_HIT | OBD_RECT_DAMAGE);
	    ply_work->rect_work[1].flag			|= OBD_RECT_GROUP;
		ObjRectAtkSet(&ply_work->rect_work[1], 0, 0);	// 攻撃クリア
	//	ObjRectDefSet(&ply_work->rect_work[1], 0, 0);
		ObjRectDefSet(&ply_work->rect_work[1], GMD_OBJ_RECT_DEF_FLAG_INVINCIBLE, GMD_OBJ_RECT_DEF_POWER_INVINCIBLE);
	}
	else {
	// 通信対戦
		// 攻撃矩形
		//ply_work->rect_work[1].ppDef		= NULL;
		ply_work->rect_work[1].user_flag	= GMD_OBJ_RECT_GROUP_ENEMY_SET;
	    ply_work->rect_work[1].flag			&= ~(OBD_RECT_ATK_FUNC | OBD_RECT_HIT | OBD_RECT_DAMAGE);
	    ply_work->rect_work[1].flag			|= OBD_RECT_GROUP;
		ObjRectAtkSet(&ply_work->rect_work[1], 0, 0);	// 攻撃クリア
		//ObjRectDefSet(&ply_work->rect_work[1], 0, 0);
		ObjRectDefSet(&ply_work->rect_work[1], GMD_OBJ_RECT_DEF_FLAG_INVINCIBLE, GMD_OBJ_RECT_DEF_POWER_INVINCIBLE);

#if 0
		// 通信対戦相手の体押し合い用矩形設定
		ply_work->rect_work[2].ppDef		= gmPlayerMultipleDefInit;
        ply_work->rect_work[2].user_flag	= GMD_OBJ_RECT_GROUP_ENEMY;	// くらいのみ
        ply_work->rect_work[2].flag			|= (OBD_RECT_GROUP | OBD_RECT_NODAMAGE | OBD_RECT_NOHIT_UP);
        ply_work->rect_work[2].parent_obj	= &ply_work->obj_work;
    	ObjRectAtkSet(&ply_work->rect_work[2], 0, 0);
    	ObjRectDefSet(&ply_work->rect_work[2], ((u16)~GMD_OBJ_RECT_DEF_FLAG_BODY), GMD_OBJ_RECT_DEF_POWER_DEFAULT);	// 体あたりのみチェック
        //ObjRectWorkSet(&ply_work->rect_work[2], -48,-32, 48, 32);
        ObjRectWorkSet(&ply_work->rect_work[2], -16,-16, 16, 16);
#endif
	}
#endif

    /* プレイヤーパラメータ初期化 */
    GmPlayerSpdParameterSet(ply_work);

    // オブジェクト用数値初期化
    ply_work->obj_work.dir.x = 0;
    ply_work->obj_work.dir.y = 0;
    ply_work->obj_work.dir.z = 0;
    ply_work->obj_work.spd_m = 0;
    ply_work->obj_work.spd.x = 0;
    ply_work->obj_work.spd.y = 0;
    ply_work->obj_work.spd.z = 0;
//    ply_work->blaze_timer = 0;

    if ( !(ply_work->gmk_flag & GMD_PLGF_BELT) ) {
        ply_work->obj_work.pos.z = 0;
	}
    ply_work->obj_work.ride_obj = NULL;

    // プレイヤー用数値初期化
	if ( !(ply_work->gmk_flag & (GMD_PLGF_GMK_MAP_LIMIT_LCD_X | GMD_PLGF_GMK_MAP_LIMIT_LCD_Y )) ) {
		ply_work->gmk_obj = NULL;
		ply_work->gmk_camera_ofst_x = 0;
		ply_work->gmk_camera_ofst_y = 0;
		ply_work->gmk_camera_gmk_center_ofst_x = 0;
		ply_work->gmk_camera_gmk_center_ofst_y = 0;
		ply_work->gmk_flag &= ~(GMD_PLGF_CAMERA_GMK_X | GMD_PLGF_CAMERA_GMK_Y | GMD_PLGF_BOOST_CHK_ON_GMK |
							GMD_PLGF_CAMERA_GMK_X_FIX | GMD_PLGF_CAMERA_GMK_Y_FIX | GMD_PLGF_CAMERA_CENTER_OFST);
    }
    ply_work->gmk_work0 = 0;
    ply_work->gmk_work1 = 0;
    ply_work->gmk_work2 = 0;
    ply_work->gmk_work3 = 0;
//    ply_work->boost_out_timer = 0;
//    ply_work->nitro_ban_timer = 0;
    //ply_work->trick_combo = 0;
    ply_work->spd_work_max = 0;
    //ply_work->hover_timer = GMD_PL_BLAZE_HOVER_TIME;

    // ジャンプ開始位置クリア
    ply_work->camera_jump_pos_y = 0;

    if ( ply_work->graind_id & GMD_PLG_GRAIND_RIDE ) {
        ply_work->obj_work.flag |= OBD_OBJECT_B;
	}

    // フラグ設定
	ply_work->gmk_flag &= ~(GMD_PLGF_TOUCH | GMD_PLGF_TOUCH_FORCE_DIR | GMD_PLGF_TOUCH_FLIP | GMD_PLGF_TOUCH_DISP_FLIP |/*GMD_PLGF_GMK_NONITROEFFECT | GMD_PLGF_GMK_TABLE_COL | */GMD_PLGF_GMK_WARP | GMD_PLGF_GMK_HOLD_POS_Z);

    ply_work->player_flag &= ~(GMD_PLF_STATE_INIT_CLEAR_MASK);
    ply_work->obj_work.flag  &= ~OBD_OBJECT_NOHIT;
    ply_work->obj_work.flag  |= OBD_OBJECT_NOCLIP;
    ply_work->obj_work.disp_flag &= ~(OBD_DISP_NODISP | OBD_DISP_NODIR | OBD_DISP_STOP);
//    if (!GmMainIsBossStage()) {
//        ply_work->obj_work.disp_flag |= OBD_DISP_3D_PARALLEL;
//	}
    ply_work->obj_work.move_flag &= ~(OBD_MOVE_COL_MASK | OBD_MOVE_NOCOL | OBD_MOVE_NOCOLOBJ | OBD_MOVE_NOCOLFIELD | OBD_MOVE_JUMP |
							OBD_MOVE_NOMOVE | OBD_MOVE_NOSPDM | OBD_MOVE_NOSPD | OBD_MOVE_NOFLOW | OBD_MOVE_NOCLR_SPDXY);
    ply_work->obj_work.move_flag |= OBD_MOVE_FALL | OBD_MOVE_DIR | OBD_MOVE_SLOPE | OBD_MOVE_SLOPE_STICK | OBD_MOVE_LIMIT_OUT | OBD_MOVE_UNDERALL | OBD_MOVE_TOP_DIFF;

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコ時
		ply_work->obj_work.move_flag &= ~OBD_MOVE_SLOPE_STICK;
	} else if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		//スペステ時
		ply_work->obj_work.move_flag &= ~OBD_MOVE_SLOPE_STICK;
		ply_work->obj_work.move_flag |= OBD_MOVE_NOCLR_SPDXY | OBD_MOVE_NOCOLFIELD;
	}

 
	// タイマークリア
	ply_work->no_jump_move_timer = 0;


	// モーションブレンディング速度設定
    if ( ply_work->obj_work.obj_3d ) {
		ply_work->obj_work.obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD;
	}

#if defined(MTD_DEBUG)
    ply_work->debug_flag &= ~GMD_PLFD_SETMODE;
    ply_work->debug_flag &= ~GMD_PLFD_MOVEMODE;
    // ply_work->debug_flag &= ~GMD_PLFD_DECOSET;

//    // 表示オブジェ消去
//    gmPlayerDebugSetDispClear(ply_work);
//
//    if ( !EfFadeEndCheck() ) {
//        // フェードイン
//        EfFadeInit( EFD_FADE_BLACK_IN, EFD_FADE_SPD );
//    }
#endif

//    // グラインドSE停止
//    NNS_SndPlayerStopSeq( &ply_work->h_snd_graind, 0 );
//
//    // 歓声SE停止
//    NNS_SndPlayerStopSeq( &ply_work->h_snd_gallery, 32 );
//    NNS_SndHandleReleaseSeq( &ply_work->h_snd_gallery );
}

 
// ================================================================
// GmPlayerSetCardinalPoints
/*!
  プレイヤー基点座標設定

  @param pos_x		[in] 基点座標X(fx32)
  @param pos_y		[in] 基点座標Y(fx32)
  @param pos_z		[in] 基点座標Z(fx32)

  @note
    自プレイヤーの基点座標を設定します \n
    ゴースト表示時はゴースト側のプレイヤーオブジェクトの座標も設定します。
 */
// ================================================================
#if 0
void GmPlayerSetCardinalPoints(fx32 pos_x, fx32 pos_y, fx32 pos_z)
{
	gm_pos_x = (pos_x >> FX32_SHIFT);
	gm_pos_y = (pos_y >> FX32_SHIFT);
	gm_pos_z = (pos_z >> FX32_SHIFT);

	if (GsGetMainSysInfo()->game_flag & GSD_GAME_FLAG_GHOST &&
			g_gm_main_ply_obj_list[1]) {
		// ゴーストの座標を設定する
		g_gm_main_ply_obj_list[1]->obj.pos.x = pos_x;
		g_gm_main_ply_obj_list[1]->obj.pos.y = pos_y;
		g_gm_main_ply_obj_list[1]->obj.pos.z = pos_z;
	}
}
#endif

// ================================================================
// GmPlayerStateGimmickInit
/*!
  プレイヤー状態ギミック初期化（ギミックHIT時

  @param ply_work		[in] 対象プレイヤーワークポインタ

  @note
    ギミック中にギミックにヒットして中断される事もあるので\n
    ギミックで使用されるフラグ設定を元に戻す処理はここに入れる\n
 */
// ================================================================
void GmPlayerStateGimmickInit(GMS_PLAYER_WORK *ply_work)
{
    // プレイヤー用数値初期化
    ply_work->gmk_obj = NULL;
    ply_work->gmk_camera_ofst_x = 0;
    ply_work->gmk_camera_ofst_y = 0;
	ply_work->gmk_camera_gmk_center_ofst_x = 0;
	ply_work->gmk_camera_gmk_center_ofst_y = 0;
    ply_work->gmk_work0 = 0;
    ply_work->gmk_work1 = 0;
    ply_work->gmk_work2 = 0;
    ply_work->gmk_work3 = 0;
//    ply_work->blaze_timer = 0;

    ply_work->obj_work.dir.x = 0;
    ply_work->obj_work.dir.y = 0;

	// コンボカウンタクリア
	ply_work->score_combo_cnt = 0;

	// カメラ中心点クリア
	GmPlayerCameraOffsetSet(ply_work, 0, 0);
	// カメラスクロール遊び設定クリア
	GmCameraAllowReset();

    // フラグ設定
    if ( ply_work->graind_id & GMD_PLG_GRAIND_RIDE ){
        ply_work->obj_work.flag |= OBD_OBJECT_B;
        ply_work->graind_id = 0;
    }

#if 0
	// 矩形設定
	ply_work->rect_work[0].ppDef = GmPlayerDamageReactionInit;	// くらい処理初期化
	ply_work->rect_work[0].flag &= ~(OBD_RECT_HIT | OBD_RECT_DAMAGE);	// HIT復帰
#endif

	ply_work->player_flag &= ~(GMD_PLF_STATE_GIMMICK_INIT_CLEAR_MASK);
	ply_work->gmk_flag &= ~GMD_PLGF_STATE_GIMMICK_INIT_CLEAR_MASK;
	ply_work->gmk_flag2 &= ~GMD_PLGF2_STATE_GIMMICK_INIT_CLEAR_MASK;
    ply_work->obj_work.disp_flag &= ~(OBD_DISP_NODIR | OBD_DISP_NODISP);
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_MOVE_NOCOLOBJ | OBD_MOVE_NOCOLFIELD | OBD_MOVE_NOSPDM | OBD_MOVE_NOSPD | OBD_MOVE_NOFLOW | OBD_MOVE_NOCLR_SPDXY);
	ply_work->obj_work.move_flag |= OBD_MOVE_FALL | OBD_MOVE_DIR | OBD_MOVE_SLOPE_STICK | OBD_MOVE_SLOPE | OBD_MOVE_UNDERALL | OBD_MOVE_TOP_DIFF;

	ply_work->no_jump_move_timer = 0;

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコ時
		ply_work->obj_work.move_flag &= ~OBD_MOVE_SLOPE_STICK;
	} else if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		//スペステ時
		ply_work->obj_work.move_flag &= ~OBD_MOVE_SLOPE_STICK;
		ply_work->obj_work.move_flag |= OBD_MOVE_NOCLR_SPDXY | OBD_MOVE_NOCOLFIELD;

	}

//    if( !GmMainIsBossStage() ) {
//        ply_work->obj_work.disp_flag |= OBD_DISP_3D_PARALLEL;
//	}

	// グラインドSE ストップ
//    NNS_SndPlayerStopSeq( &ply_work->h_snd_graind, 0 );

#if defined(MTD_DEBUG)
    ply_work->debug_flag &= ~GMD_PLFD_SETMODE;
    ply_work->debug_flag &= ~GMD_PLFD_MOVEMODE;
    // ply_work->debug_flag &= ~GMD_PLFD_DECOSET;

    // 表示オブジェ消去
//    gmPlayerDebugSetDispClear(ply_work);
#endif
}

// ================================================================
// GmPlayerSpdParameterSet
/*!
  プレイヤーの速度などの基本パラメーターをセットする

  @param ply_work   [in] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerSpdParameterSet(GMS_PLAYER_WORK *ply_work)
{
    /* プレイヤー用数値設定 */
    ply_work->spd_add       = g_gm_player_parameter[ply_work->char_id].spd_add;		// 通常 加速値

 //   if ( GmMainIsBossStage() ) {
 //   // ボスステージは最大速度を半分にする
 //       ply_work->spd_max = g_gm_player_parameter[ply_work->char_id].spd_max >> 1;	// 通常 最大速度値
//	}
//    else {
        ply_work->spd_max = g_gm_player_parameter[ply_work->char_id].spd_max;		// 通常 最大速度値
//	}
	// 速度段階設定
    ply_work->spd1 = (fx32)(ply_work->spd_max * 0.15);
    ply_work->spd2 = (fx32)(ply_work->spd_max * 0.30);
    ply_work->spd3 = (fx32)(ply_work->spd_max * 0.50);
    ply_work->spd4 = (fx32)(ply_work->spd_max * 0.70); 
    ply_work->spd5 = (fx32)(ply_work->spd_max * 0.90);

    ply_work->spd_dec			= g_gm_player_parameter[ply_work->char_id].spd_dec;				// 通常放置 減速値
    ply_work->spd_spin			= g_gm_player_parameter[ply_work->char_id].spd_spin;			// スピン 初速値
    ply_work->spd_add_spin		= g_gm_player_parameter[ply_work->char_id].spd_add_spin;		// スピン 加速値
    ply_work->spd_max_spin		= g_gm_player_parameter[ply_work->char_id].spd_max_spin;		// スピン 最大速度値
    ply_work->spd_dec_spin		= g_gm_player_parameter[ply_work->char_id].spd_dec_spin;		// スピン放置 減速値
//    ply_work->spd_max_boost		= g_gm_player_parameter[ply_work->char_id].spd_max_boost;		// ニトロ 加速値
//    ply_work->spd_add_nitro		= g_gm_player_parameter[ply_work->char_id].spd_add_nitro;		// ニトロ 加速値
//    ply_work->spd_max_nitro		= g_gm_player_parameter[ply_work->char_id].spd_max_nitro;		// ニトロ 最大速度値
//    ply_work->spd_dec_nitro		= g_gm_player_parameter[ply_work->char_id].spd_dec_nitro;		// ニトロ放置 減速値
//    ply_work->spd_chk_nitro		= g_gm_player_parameter[ply_work->char_id].spd_chk_nitro;		// ニトロダウン（解除） 速度値
    ply_work->spd_max_add_slope	= g_gm_player_parameter[ply_work->char_id].spd_max_add_slope;	// 坂道時の最大速度アップ値
    ply_work->spd_jump			= g_gm_player_parameter[ply_work->char_id].spd_jump;			// ジャンプ 初速値

    ply_work->time_air		= (fx32)(g_gm_player_parameter[ply_work->char_id].time_air << FX32_SHIFT);				// 空気の持ち時間
    ply_work->time_damage	= (fx32)(g_gm_player_parameter[ply_work->char_id].time_damage << FX32_SHIFT);				// ダメージ後の無敵時間
    ply_work->fall_timer	= (fx32)(g_gm_player_parameter[ply_work->char_id].fall_wait_time << FX32_SHIFT);		// 一定角度以上接地時の落下開始までの待ち時間

    ply_work->spd_jump_add	= g_gm_player_parameter[ply_work->char_id].spd_jump_add;	// ジャンプ 横方向 加速値
	ply_work->spd_jump_max	= g_gm_player_parameter[ply_work->char_id].spd_jump_max;	// ジャンプ 横方向 最大速度値
	ply_work->spd_jump_dec	= g_gm_player_parameter[ply_work->char_id].spd_jump_dec;	// ジャンプ 横方向 最大速度値

	// ピンボールスピン用速度
	ply_work->spd_add_spin_pinball				= g_gm_player_parameter[ply_work->char_id].spd_add_spin_pinball;
	ply_work->spd_max_spin_pinball				= g_gm_player_parameter[ply_work->char_id].spd_max_spin_pinball;
	ply_work->spd_dec_spin_pinball				= g_gm_player_parameter[ply_work->char_id].spd_dec_spin_pinball;
	ply_work->spd_max_add_slope_spin_pinball	= g_gm_player_parameter[ply_work->char_id].spd_max_add_slope_spin_pinball;


	if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// 通常
		if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
			ply_work->obj_work.dir_slope = GMD_PL_KEI_DIR_TRUCK;	// トロッコ
		}
		else {
			ply_work->obj_work.dir_slope = GMD_PL_KEI_DIR;			// 通常
		}
    } else {
		// スペステ
	    ply_work->obj_work.dir_slope	= 1;
	}
    ply_work->obj_work.spd_slope		= g_gm_player_parameter[ply_work->char_id].spd_slope;
    ply_work->obj_work.spd_slope_max	= g_gm_player_parameter[ply_work->char_id].spd_slope_max;
    ply_work->obj_work.spd_fall		= g_gm_player_parameter[ply_work->char_id].spd_fall;
    ply_work->obj_work.spd_fall_max	= g_gm_player_parameter[ply_work->char_id].spd_fall_max;
    ply_work->obj_work.push_max		= g_gm_player_parameter[ply_work->char_id].push_max;

	// 水中時設定
    if ( ply_work->player_flag & GMD_PLF_WATER ) {
        GMD_PLAYER_WATERJUMP_SET(ply_work->spd_jump);
        GMD_PLAYER_WATER_SET(ply_work->obj_work.spd_fall);
    }

	// ハイスピード対応
	if (ply_work->hi_speed_timer) {

		/* プレイヤー用数値設定 */
		ply_work->spd_add <<= 1;		// 通常 加速値
		if (ply_work->spd_add > GMD_PL_DEF_MAX_SPD) {
			ply_work->spd_add = GMD_PL_DEF_MAX_SPD;
		}

		ply_work->spd_max <<= 1;		// 通常 最大速度値
		if (ply_work->spd_max > GMD_PL_DEF_MAX_SPD) {
			ply_work->spd_max = GMD_PL_DEF_MAX_SPD;
		}

		// 速度段階設定
		//ply_work->spd1 <<= 1;
		//ply_work->spd2 <<= 1;
		//ply_work->spd3 <<= 1;
		//ply_work->spd4 <<= 1;
		//ply_work->spd5 <<= 1;

		ply_work->spd_dec <<= 1;			// 通常放置 減速値
		ply_work->spd_spin <<= 1;			// スピン 初速値
		ply_work->spd_add_spin <<= 1;		// スピン 加速値
		ply_work->spd_max_spin <<= 1;		// スピン 最大速度値
		if (ply_work->spd_max_spin > GMD_PL_DEF_MAX_SPD) {
			ply_work->spd_max_spin = GMD_PL_DEF_MAX_SPD;
		}
		ply_work->spd_dec_spin <<= 1;		// スピン放置 減速値
		ply_work->spd_max_add_slope <<= 1;	// 坂道時の最大速度アップ値
		//ply_work->spd_jump <<= 1;			// ジャンプ 初速値

		//ply_work->time_air <<= 1;			// 空気の持ち時間
		//ply_work->time_damage <<= 1;		// ダメージ後の無敵時間
		//ply_work->fall_timer <<= 1;			// 一定角度以上接地時の落下開始までの待ち時間

		ply_work->spd_jump_add <<= 1;		// ジャンプ 横方向 加速値
		ply_work->spd_jump_max <<= 1;		// ジャンプ 横方向 最大速度値
		if (ply_work->spd_jump_max > GMD_PL_DEF_MAX_SPD) {
			ply_work->spd_jump_max = GMD_PL_DEF_MAX_SPD;
		}
		ply_work->spd_jump_dec <<= 1;		// ジャンプ 横方向 最大速度値

		//ply_work->obj_work.dir_slope <<= 1;
		//ply_work->obj_work.spd_slope <<= 1;
		//ply_work->obj_work.spd_slope_max <<= 1;
		//ply_work->obj_work.spd_fall <<= 1;
		//ply_work->obj_work.spd_fall_max <<= 1;
		//ply_work->obj_work.push_max <<= 1;
	}


#if 0
	// 速度ダウン中
	if ( ply_work->speed_curse ) {
		// 呪われ中
		ply_work->spd_add >>= ply_work->speed_curse;
		ply_work->spd_max >>= ply_work->speed_curse;

		ply_work->spd_spin >>= ply_work->speed_curse;
		ply_work->spd_add_spin >>= ply_work->speed_curse;
		ply_work->spd_max_spin >>= ply_work->speed_curse;
		ply_work->spd_max_add_slope >>= ply_work->speed_curse;
		ply_work->spd_jump >>= ply_work->speed_curse;

		ply_work->obj_work.spd_slope >>= ply_work->speed_curse;
		ply_work->obj_work.spd_slope_max >>= ply_work->speed_curse;
		ply_work->obj_work.push_max >>= ply_work->speed_curse;
	}
#endif
	// ◆暫定
	//ply_work->rect_work[GMD_PLAYER_RECT_ATK].flag &= ~OBD_RECT_ENABLE;	// 無効化
}

// ================================================================
// GmPlayerSpdParameterSetWater
/*!
  パラメータ設定 水中関連設定のみ

  @param	ply_work	[in] プレイヤーワークポインタ
  @param	water		[in] TRUE : 水中設定  FALSE : 陸上設定
 */
// ================================================================
void GmPlayerSpdParameterSetWater(GMS_PLAYER_WORK *ply_work, BOOL water)
{
    ply_work->spd_jump				= g_gm_player_parameter[ply_work->char_id].spd_jump;			// ジャンプ 初速値
    ply_work->obj_work.spd_fall		= g_gm_player_parameter[ply_work->char_id].spd_fall;
	// 水中時設定
	if (water) {
		GMD_PLAYER_WATERJUMP_SET(ply_work->spd_jump);
		GMD_PLAYER_WATER_SET(ply_work->obj_work.spd_fall);
	}
}

// ================================================================
// GmPlayerSetAtk
/*!
  攻撃設定

  @param ply_work   [in] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerSetAtk(GMS_PLAYER_WORK *ply_work)
{
	ply_work->rect_work[GMD_PLAYER_RECT_ATK].flag |= OBD_RECT_ENABLE;	// 有効化
	ObjRectHitAgain(&ply_work->rect_work[GMD_PLAYER_RECT_ATK]);
}

// ================================================================
// GmPlayerSetDefInvincible
/*!
  くらい無敵設定

  @param ply_work   [in] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerSetDefInvincible(GMS_PLAYER_WORK *ply_work)
{
	// くらい設定OFF
	ply_work->rect_work[GMD_PLAYER_RECT_DEF].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
}

// ================================================================
// GmPlayerSetDefNormal
/*!
  くらい通常設定

  @param ply_work   [in] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerSetDefNormal(GMS_PLAYER_WORK *ply_work)
{
	// くらい設定OFF
	ply_work->rect_work[GMD_PLAYER_RECT_DEF].def_power = GMD_OBJ_RECT_DEF_POWER_DEFAULT;
}

// ================================================================
// GmPlayerBreathingSet
/*!
  息継ぎステータス設定

  @param ply_work   [in] プレイヤーワークポインタ

  @note
	プレイヤーが息継ぎをした時に窒息状態を元に戻します

 */
// ================================================================
void GmPlayerBreathingSet(GMS_PLAYER_WORK *ply_work)
{
	ply_work->water_timer = 0;

	// 危険BGMを戻す◆
}

// ================================================================
// GmPlayerSetMarkerPoint
/*!
  中間ポイント設定

  @param ply_work   [in] プレイヤーワークポインタ
  @param pos_x		[in] 再開位置 X
  @param pos_y		[in] 再開位置 Y

 */
// ================================================================
void GmPlayerSetMarkerPoint(GMS_PLAYER_WORK *ply_work, fx32 pos_x, fx32 pos_y)
{
	// 中間ポイントタイム
	g_gm_main_system.time_save = g_gm_main_system.game_time;

	// 再開座標設定
	g_gm_main_system.resume_pos_x = pos_x;
	g_gm_main_system.resume_pos_y = pos_y - (ply_work->obj_work.field_rect[MTD_BOTTOM] << FX32_SHIFT);
}

// ================================================================
// GmPlayerSetSuperSonic
/*!
	スーパーソニック設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
void GmPlayerSetSuperSonic(GMS_PLAYER_WORK *ply_work)
{
	// ステート初期化
	GmPlayerStateInit(ply_work);

	// トロッコ時 Z座標補正
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		ply_work->obj_work.pos.z = GMD_PL_TRUCK_TRANS_SUPER_POS_Z;
		ply_work->gmk_flag |= GMD_PLGF_GMK_HOLD_POS_Z;
	}

	// キャラクターID変更
	if (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) {
		// ピンボールスーパーソニック
		ply_work->char_id = GSD_CHAR_ID_PN_S_SONIC;
	}
	else if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコスーパーソニック
		ply_work->char_id = GSD_CHAR_ID_TR_S_SONIC;
	}
	else {
		// 通常スーパーソニック
		ply_work->char_id = GSD_CHAR_ID_S_SONIC;
	}

	// フラグ設定
	ply_work->player_flag |= GMD_PLF_SUPER_SONIC;

	// 無敵化

	/* モデル設定 */
	GmPlayerSetModel(ply_work, GMD_PLY_MODEL_SET_SUPER);

	/* プレイヤーシーケンスステータス設定 */
	GmPlySeqSetSeqState(ply_work);

	/* スピードパラメータ設定 */
	GmPlayerSpdParameterSet(ply_work);

	// フラグ設定
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_COL_MASK | OBD_MOVE_FALL);
	ply_work->obj_work.flag |= OBD_OBJECT_NOHIT;	// HIT OFF

	// エフェクト スーパーソニックオーラ
	GmPlyEfctCreateSuperAuraDeco(ply_work);
	GmPlyEfctCreateSuperAuraBase(ply_work);

	// リング減少タイマーを演出中だけMAXにしておく
	ply_work->super_sonic_ring_timer = 0x7FFFFFFF;

	// ライト演出用ステータスクリア
	ply_work->light_rate	= 0.0f;
	ply_work->light_anm_flag= 0;

	// ゲーム中スーパーソニック使用フラグ追加
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_USE_SUPER_SONIC;

	// SE
	GmSoundPlaySE("Transform");

	// BGM変更
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_ENDING) {
		GmSoundPlayJingleInvincible();
	}
}

// ================================================================
// GmPlayerSetEndSuperSonic
/*!
	スーパーソニック終了設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
void GmPlayerSetEndSuperSonic(GMS_PLAYER_WORK *ply_work)
{
	// キャラクターID変更
	if (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) {
		// ピンボールソニック
		ply_work->char_id = GSD_CHAR_ID_PN_SONIC;
	}
	else if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコソニック
		ply_work->char_id = GSD_CHAR_ID_TR_SONIC;
	}
	else {
		// 通常ソニック
		ply_work->char_id = GSD_CHAR_ID_SONIC;
	}

	// フラグ設定
	ply_work->player_flag &= ~GMD_PLF_SUPER_SONIC;

	// 無敵解除

	/* モデル設定 */
	GmPlayerSetModel(ply_work, GMD_PLY_MODEL_SET_NORMAL);

	/* プレイヤーシーケンスステータス設定 */
	GmPlySeqSetSeqState(ply_work);

	/* スピードパラメータ設定 */
	GmPlayerSpdParameterSet(ply_work);

	// エフェクト スーパーソニック終了オーラ
	GmPlyEfctCreateSuperEnd(ply_work);

	// ライト演出クリア
	GmPlayerSetDefLight();
	GmPlayerSetDefRimParam(ply_work);
}

// ================================================================
// GmPlayerSetSplStgSonic
/*!
	スペステソニック設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
void GmPlayerSetSplStgSonic(GMS_PLAYER_WORK *ply_work)
{
	// フラグ設定
	ply_work->obj_work.move_flag |= OBD_MOVE_SLOPE
									| OBD_MOVE_NOMOVE			// 起動時は停止状態
									| OBD_MOVE_NOCOL;			// 起動時はコリジョンなし
																// フェード完了で解除する
	ply_work->obj_work.move_flag &= ~OBD_MOVE_SLOPE_STICK;

	/* プレイヤーシーケンスステータス設定 */
	GmPlySeqSetSeqState(ply_work);

	/* スピードパラメータ設定 */
	GmPlayerSpdParameterSet(ply_work);

	// アクション初期設定(シーケンス中で基本的には切り替えなくなる為)
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

}

// ================================================================
// GmPlayerSetPinballSonic
/*!
	ピンボールソニック設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
void GmPlayerSetPinballSonic(GMS_PLAYER_WORK *ply_work)
{
	// キャラクターID変更
	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		// スーパーピンボールソニック
		ply_work->char_id = GSD_CHAR_ID_PN_S_SONIC;
	}
	else {
		// 通常ピンボールソニック
		ply_work->char_id = GSD_CHAR_ID_PN_SONIC;
	}

	// フラグ設定
	ply_work->player_flag |= GMD_PLF_PINBALL_SONIC;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_SLOPE_STICK;	// 坂道接着OFF

	// 描画オフセット設定
	//ply_work->disp_ofst_y = GMD_PL_DISP_OFST_Y_PINBALL;

	/* プレイヤーシーケンスステータス設定 */
	GmPlySeqSetSeqState(ply_work);

	/* スピードパラメータ設定 */
	GmPlayerSpdParameterSet(ply_work);

	// アクション初期設定(シーケンス中で基本的には切り替えなくなる為)
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
}

// ================================================================
// GmPlayerSetEndPinballSonic
/*!
	ピンボールソニック終了設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
void GmPlayerSetEndPinballSonic(GMS_PLAYER_WORK *ply_work)
{
	// キャラクターID変更
	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		// スーパーソニック
		ply_work->char_id = GSD_CHAR_ID_S_SONIC;
	}
	else {
		// 通常ソニック
		ply_work->char_id = GSD_CHAR_ID_SONIC;
	}

	// フラグ設定
	ply_work->player_flag &= ~GMD_PLF_PINBALL_SONIC;
	ply_work->obj_work.move_flag |= OBD_MOVE_SLOPE_STICK;	// 坂道接着ON

	// 描画オフセット設定
	//ply_work->disp_ofst_y = GMD_PL_DISP_OFST_Y;

	/* プレイヤーシーケンスステータス設定 */
	GmPlySeqSetSeqState(ply_work);

	/* スピードパラメータ設定 */
	GmPlayerSpdParameterSet(ply_work);
}

// ================================================================
// GmPlayerSetTruckRide
/*!
	トロッコライド設定

	@param	ply_work		[in]	プレイヤーワークポインタ
	@param	truck_obj		[in]	トロッコオブジェクト
	@param	field_left		[in]	地面あたり設定左
	@param	field_top		[in]	地面あたり設定上
	@param	field_right		[in]	地面あたり設定右
	@param	field_bottom	[in]	地面あたり設定下
 */
// ================================================================
void GmPlayerSetTruckRide(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *truck_obj, s16 field_left, s16 field_top, s16 field_right, s16 field_bottom)
{
	s32						i;
	OBS_CAMERA				*camera;
	OBS_OBJECT_WORK			*obj_work;
	BOOL					b_demo = FALSE;
	OBS_ACTION3D_NN_WORK	*obj_3d;

	obj_work = (OBS_OBJECT_WORK*)ply_work;

	// キャラクターID変更
	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		// トロッコスーパーソニック
		ply_work->char_id = GSD_CHAR_ID_TR_S_SONIC;
	}
	else {
		// 通常トロッコソニック
		ply_work->char_id = GSD_CHAR_ID_TR_SONIC;
	}

	// フラグ設定
	ply_work->player_flag |= GMD_PLF_TRUCK_RIDE;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_SLOPE_STICK;	// 坂道接着OFF
	ply_work->gmk_flag2 &= ~GMD_PLGF2_TRUCK_SLOPEFLY_DEC;	// 坂道ジャンプ減速OFF

	// トロッコオブジェクト保存
	ply_work->truck_obj = truck_obj;

	// 矩形登録前処理設定
	ply_work->obj_work.ppRec = gmPlayerRectTruckFunc;

	// 地形判定処理設定
	ply_work->obj_work.ppCol = gmPlayerTruckCollisionFunc;

	// モーションコールバック設定
	obj_3d = &ply_work->obj_3d_work[0];
	for (i = 0; i < GMD_PLY_MODEL_TYPE_MAX*GMD_PLY_MODEL_SET_MAX; i++, obj_3d++) {
		obj_3d->mtn_cb_func = gmGmkPlayerMotionCallbackTruck;
		obj_3d->mtn_cb_param= ply_work;
	}
	nnMakeUnitMatrix(&ply_work->truck_mtx_ply_mtn_pos);

	/* プレイヤーシーケンスステータス設定 */
	GmPlySeqSetSeqState(ply_work);

	/* スピードパラメータ設定 */
	GmPlayerSpdParameterSet(ply_work);

	/* 地形あたり矩形設定 */
	ObjObjectFieldRectSet(obj_work, field_left, field_top, field_right, field_bottom);
	// 補正値調整
	obj_work->field_ajst_w_db_f = 3;//2;
	obj_work->field_ajst_w_db_b = 4;
	obj_work->field_ajst_w_dl_f = 3;
	obj_work->field_ajst_w_dl_b = 4;
	obj_work->field_ajst_w_dt_f = 3;//2;
	obj_work->field_ajst_w_dt_b = 4;
	obj_work->field_ajst_w_dr_f = 3;
	obj_work->field_ajst_w_dr_b = 4;

	obj_work->field_ajst_h_db_r = 3;
	obj_work->field_ajst_h_db_l = 3;
	obj_work->field_ajst_h_dl_r = 3;
	obj_work->field_ajst_h_dl_l = 3;
	obj_work->field_ajst_h_dt_r = 3;
	obj_work->field_ajst_h_dt_l = 3;
	obj_work->field_ajst_h_dr_r = 3;
	obj_work->field_ajst_h_dr_l = 3;
	
	/* 矩形サイズ調整◆要モーション対応 */
	ObjRectWorkSet(&ply_work->rect_work[GMD_PLAYER_RECT_BODY],
						-8, (s16)(-32 + field_bottom),
						8, (s16)(field_bottom));
	ObjRectWorkSet(&ply_work->rect_work[GMD_PLAYER_RECT_DEF],
						-8, (s16)(-48 + field_bottom),
						8, (s16)(-16 + field_bottom));
	ObjRectWorkSet(&ply_work->rect_work[GMD_PLAYER_RECT_ATK],
						//-8, (s16)(-16 + field_bottom),
						//8, (s16)(field_bottom));
						-16, (s16)(-48 + field_bottom),
						16, (s16)(-16 + field_bottom));//
						//-16, (s16)(-48 + field_bottom),
						//16, (s16)(-16 + field_bottom));
	ply_work->rect_work[GMD_PLAYER_RECT_ATK].flag &= ~OBD_RECT_ENABLE;	// はじめは無効

	// アクション初期設定(シーケンス中で基本的には切り替えなくなる為)
	//GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
	//ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

	/* カメラ処理変更 */
	camera = ObjCameraGet(g_obj.glb_camera_id);
	camera->user_func = GmCameraTruckFunc;

	/* プレイヤーをFWへ */
	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
		// 右向きへ強制変換
		GmPlayerSetReverse(ply_work);
	}
	if (ply_work->seq_state == GME_PLY_SEQ_STATE_GMK_DEMO_FW) {
		b_demo = TRUE;
	}
	GmPlySeqChangeFw(ply_work);
	if (b_demo) {
		// デモシーケンスに切り替えなおしておく
		GmPlySeqInitDemoFw(ply_work);
	}
}

// ================================================================
// GmPlayerSetEndTruckRide
/*!
	トロッコライド終了設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
void GmPlayerSetEndTruckRide(GMS_PLAYER_WORK *ply_work)
{
	s32						i;
	OBS_CAMERA				*camera;
	OBS_OBJECT_WORK			*obj_work;
	OBS_ACTION3D_NN_WORK	*obj_3d;

	obj_work = (OBS_OBJECT_WORK*)ply_work;

	// キャラクターID変更
	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		// スーパーソニック
		ply_work->char_id = GSD_CHAR_ID_S_SONIC;
	}
	else {
		// 通常ソニック
		ply_work->char_id = GSD_CHAR_ID_SONIC;
	}

	// フラグ設定
	ply_work->player_flag &= ~GMD_PLF_TRUCK_RIDE;
	ply_work->obj_work.move_flag |= OBD_MOVE_SLOPE_STICK;	// 坂道接着ON

	// 矩形登録前処理クリア
	ply_work->obj_work.ppRec = NULL;

	// 地形判定処理クリア
	ply_work->obj_work.ppCol = NULL;

	// モーションコールバック設定クリア・復旧
	obj_3d = &ply_work->obj_3d_work[0];
	for (i = 0; i < GMD_PLY_MODEL_TYPE_MAX*GMD_PLY_MODEL_SET_MAX; i++, obj_3d++) {
#if _WII
		// 復旧
		obj_3d->mtn_cb_func = gmGmkPlayerMotionCallback;
#else
		// クリア
		obj_3d->mtn_cb_func	= NULL;
		obj_3d->mtn_cb_param= NULL;
#endif
	}

	/* プレイヤーシーケンスステータス設定 */
	GmPlySeqSetSeqState(ply_work);

	/* スピードパラメータ設定 */
	GmPlayerSpdParameterSet(ply_work);

	/* 地形あたり矩形復旧 */
	ObjObjectFieldRectSet(&ply_work->obj_work, GMD_PL_REC_L, GMD_PL_REC_T, GMD_PL_REC_R, GMD_PL_REC_B);
	// 補正値復帰
	obj_work->field_ajst_w_db_f = 2;
	obj_work->field_ajst_w_db_b = 4;
	obj_work->field_ajst_w_dl_f = 2;
	obj_work->field_ajst_w_dl_b = 4;
	obj_work->field_ajst_w_dt_f = 2;
	obj_work->field_ajst_w_dt_b = 4;
	obj_work->field_ajst_w_dr_f = 2;
	obj_work->field_ajst_w_dr_b = 4;

	obj_work->field_ajst_h_db_r = 1;
	obj_work->field_ajst_h_db_l = 1;
	obj_work->field_ajst_h_dl_r = 1;
	obj_work->field_ajst_h_dl_l = 1;
	obj_work->field_ajst_h_dt_r = 1;
	obj_work->field_ajst_h_dt_l = 1;
	obj_work->field_ajst_h_dr_r = 2;
	obj_work->field_ajst_h_dr_l = 2;

	/* 矩形サイズ復旧◆要モーション対応 */
	ObjRectWorkZSet(&ply_work->rect_work[GMD_PLAYER_RECT_BODY],
						-8, -32 + GMD_PL_REC_B, -500,
						8, 0 + GMD_PL_REC_B, 500);
	ObjRectWorkZSet(&ply_work->rect_work[GMD_PLAYER_RECT_DEF],
						-8, -32 + GMD_PL_REC_B, -500,
						8, 0 + GMD_PL_REC_B, 500);
	ObjRectWorkZSet(&ply_work->rect_work[GMD_PLAYER_RECT_ATK],
						-16, -32 + GMD_PL_REC_B, -500,
						16, 0 + GMD_PL_REC_B, 500);
	ply_work->rect_work[GMD_PLAYER_RECT_ATK].flag &= ~OBD_RECT_ENABLE;	// はじめは無効

	/* 重力クリア */
	ply_work->obj_work.dir_fall = 0;
	g_gm_main_system.pseudofall_dir = 0;

	/* カメラ処理復旧 */
	camera = ObjCameraGet(g_obj.glb_camera_id);
	camera->user_func = GmCameraFunc;
}

// ================================================================
// GmPlayerSetGoalState
/*!
	ゴール時プレイヤー設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
void GmPlayerSetGoalState(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->player_flag & GMD_PLF_PINBALL_SONIC) {
		// ピンボールから戻す
		GmPlayerSetEndPinballSonic(ply_work);
	}

	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		// スーパーソニックからソニックへ戻す
		gmPlayerSuperSonicToSonic(ply_work);
	}
	GmPlayerSetDefInvincible(ply_work);					// ダメージ後の無敵をリセットその１
	ply_work->invincible_timer = 0;						// ダメージ後の無敵をリセットその２
	ply_work->genocide_timer = 0;						// 無敵アイテム効果リセット

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコの時BODY矩形サイズを調整する(スペステリングのため)
		ObjRectWorkSet(&ply_work->rect_work[GMD_PLAYER_RECT_BODY],
							0, -37,
							16, -5);
	}
}

// ================================================================
// GmPlayerSetAutoRun
/*!
	ソニック オートラン

	@param	ply_work		[in]	プレイヤーワークポインタ
	@param	scroll_spd_x	[in]	画面スクロール速度
	@param	enable			[in]	TRUE : 有効化  FALSE : 無効化
 */
// ================================================================
void GmPlayerSetAutoRun(GMS_PLAYER_WORK *ply_work, fx32 scroll_spd_x, BOOL enable)
{
	if (enable) {
		// オートラン設定
		ply_work->player_flag |= GMD_PLF_AUTO_RUN;
		ply_work->scroll_spd_x = scroll_spd_x;

		// オートランチェック用コード
		//g_gm_main_system.map_fcol.right = g_gm_main_system.map_fcol.left + 64*6;
	}
	else {
		// オートラン終了
		ply_work->player_flag &= ~GMD_PLF_AUTO_RUN;
	}

}

// =====================================================================
// プレイヤー リング テンション ストック管理
// =====================================================================
// リング
// ================================================================
// GmPlayerRingGet
/*!
  リング増加関数
 
  @param ply_work  [io] プレイヤーワークポインタ
  @param add_ring    [in] 増加リング数

  @note
		ダメージリング取得時の累計リング減算は外で行う事
 */
// ================================================================
void GmPlayerRingGet( GMS_PLAYER_WORK *ply_work, s16 add_ring )
{
    s16 ring_prev = ply_work->ring_num;
    s16 i;
    // リング追加
    ply_work->ring_num += add_ring;
    ply_work->ring_num = (s16)MTM_MATH_CLIP( ply_work->ring_num, 0, GMD_PLAYER_RING_NUM_MAX );
    
    // 累計リング追加
    ply_work->ring_stage_num += add_ring;
    ply_work->ring_stage_num = (s16)MTM_MATH_CLIP( ply_work->ring_stage_num, 0, GMD_PLAYER_RING_STAGE_NUM_MAX );
    
    // SE
	GmRingGetSE();

    // タイムアタックは1UPチェックをしない
    if ( g_gs_main_sys_info.game_mode == GSD_GAME_MODE_TIME_ATTACK) {
		return;
	}

	if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
	    // 通常ステージは100枚毎チェック
		if (!(ply_work->player_flag & GMD_PLF_SUPER_SONIC) &&
				g_gs_main_sys_info.game_mode != GSD_GAME_MODE_TIME_ATTACK) {
			for ( i = 100; i <= 900; i += 100 ) {
				if ( ring_prev < i && ply_work->ring_num >= i ) {
					// 1UP
					GmPlayerStockGet( ply_work, 1 );
					// ジングル
					GmSoundPlayJingle1UP(TRUE);
				//	// SE
				//	GmSoundPlaySE("Special_1up");
				}
			}
		}
	} else {
		// スペシャルステージは50枚(1回だけ)
		if (  (ring_prev < 50)
			&&(ply_work->ring_num >= 50) ) {
			// 1UP
			GmPlayerStockGet( ply_work, 1 );
			// ジングル
			GmSoundPlayJingle1UP(TRUE);
		}
	}
}

// ================================================================
// GmPlayerRingDec
/*!
  リング減少関数
 
  @param ply_work	 [io] プレイヤーワークポインタ
  @param dec_ring    [in] 減少リング数

  @note
		ダメージリングを振りまかずにリングを減らす場合に使用
 */
// ================================================================
void GmPlayerRingDec( GMS_PLAYER_WORK *ply_work, s16 dec_ring )
{
//	s16 ring_prev = ply_work->ring_num;
//	s16 i;

    // リング追加
    ply_work->ring_num -= dec_ring;
    ply_work->ring_num = (s16)MTM_MATH_CLIP( ply_work->ring_num, 0, GMD_PLAYER_RING_NUM_MAX );
    
    // 累計リング追加
    ply_work->ring_stage_num -= dec_ring;
    ply_work->ring_stage_num = (s16)MTM_MATH_CLIP( ply_work->ring_stage_num, 0, GMD_PLAYER_RING_STAGE_NUM_MAX );

}

// プレイヤーストック
// ================================================================
// GmPlayerStockGet
/*!
  プレイヤー人数増加関数
 
  @param ply_work  [io] プレイヤーワークポインタ
  @param add_stock   [in] 増加人数

 */
// ================================================================
void GmPlayerStockGet( GMS_PLAYER_WORK *ply_work, s16 add_stock )
{
    // タイムアタックは1UPしない
    if (g_gs_main_sys_info.game_mode == GSD_GAME_MODE_TIME_ATTACK &&
			!(GSD_MAIN_STAGE_ID_SS1 <= g_gs_main_sys_info.stage_id &&
				g_gs_main_sys_info.stage_id <= GSD_MAIN_STAGE_ID_SS7)) {
		return;
	}

// ◆残機格納先は変更
	g_gm_main_system.player_rest_num[ply_work->player_id] += add_stock;
	g_gm_main_system.player_rest_num[ply_work->player_id] =
		MTM_MATH_CLIP(g_gm_main_system.player_rest_num[ply_work->player_id], 0, GSD_MAINSYS_PLAYER_REST_MAX);
//	g_gm_main_system.player_num += add_stock;
//	g_gm_main_system.player_num = (u8)MTM_MATH_CLIP(g_gm_main_system.player_num, 0, GMD_MAIN_PLAYER_STOCK_MAX);

    //GmSoundPlayBgm( NULL, SEQ_1up );
	
	//sss[142] mppAchievementSupport::get()->event_CheckLifeCount();
	
	// トロフィー・実績獲得判定
	HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_INC_CHALLENGE);
}

// スコア
// ================================================================
// GmPlayerComboScore
/*!
 *	コンボスコア加算
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	score		[in]	加算スコア
 *	@param	pos_x		[in]	表示位置X
 *	@param	pos_y		[in]	表示位置Y
 */
// ================================================================
void GmPlayerAddScore(GMS_PLAYER_WORK *ply_work, s32 score, fx32 pos_x, fx32 pos_y)
{
	MTM_ASSERT(score > 0);

	// スコア加算
	ply_work->score += score;

	// 表示生成
	GmScoreCreateScore(score, pos_x, pos_y, FX32_ONE, GME_SCORE_VIB_LEVEL_0);
}

// ================================================================
// GmPlayerAddScoreNoDisp
/*!
 *	スコア加算 表示無し
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	score		[in]	加算スコア
 */
// ================================================================
void GmPlayerAddScoreNoDisp(GMS_PLAYER_WORK *ply_work, s32 score)
{
	MTM_ASSERT(score > 0);

	// スコア加算
	ply_work->score += score;
}

// ================================================================
// GmPlayerComboScore
/*!
 *	コンボスコア加算
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	pos_x		[in]	表示位置X
 *	@param	pos_y		[in]	表示位置Y
 */
// ================================================================
void GmPlayerComboScore(GMS_PLAYER_WORK *ply_work, fx32 pos_x, fx32 pos_y)
{
	s32	score;
	u32 combo_level;

	if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
		// 接地中はコンボにならない
		ply_work->score_combo_cnt = 0;
	}
	else {
		ply_work->score_combo_cnt++;
		if (ply_work->score_combo_cnt > 9999) {
			ply_work->score_combo_cnt = 9999;	// 念のためチェック
		}
	}

	if (ply_work->score_combo_cnt == 0) {
		combo_level = 0;
	}
	else if (ply_work->score_combo_cnt - 1 >= GMD_PLY_SCORE_COMBO_MAX) {
		combo_level = GMD_PLY_SCORE_COMBO_MAX - 1;
	}
	else {
		combo_level = ply_work->score_combo_cnt - 1;
	}

	score = gm_ply_score_combo_tbl[combo_level];

	// スコア加算
	ply_work->score += score;

	GmScoreCreateScore(score, pos_x, pos_y,
			gm_ply_score_combo_scale_tbl[combo_level], gm_ply_score_combo_vib_level_tbl[combo_level]);
}

// =====================================================================
// アイテム取得
// =====================================================================
// =====================================================================
// GmPlayerItemHiSpeedSet
/*!
	アイテム ハイスピード取得
 
	@param ply_work  [io] プレイヤーワークポインタ

 */
// =====================================================================
void GmPlayerItemHiSpeedSet(GMS_PLAYER_WORK *ply_work)
{
	MTM_ASSERT(ply_work->obj_work.obj_type == GMD_OBJTYPE_PLAYER);

	ply_work->hi_speed_timer = GMD_PL_ITEM_HISPEED_TIME * FX32_ONE;

	// 速度再設定
	GmPlayerSpdParameterSet(ply_work);

	// 早足BGM開始
	GmSoundChangeSpeedupBGM();
#if 0
	if (!(pPlayer->player_flag & GMD_PLF_NO_ITEMSLOW)) {
	    // スロウエフェクト表示
		GmEffectInitPlayerVSItemEffect(pPlayer, GMD_EFFECT_VS_ITEM_EFFECT_TYPE_SLOW);
	    // スロウ設定
	    pPlayer->slow_timer = GMD_PLAYER_ITEM_SLOW_TIME;
	}
#endif
}

// ================================================================
// GmPlayerItemInvincibleSet
/*!
	アイテム 無敵取得
 
	@param ply_work  [io] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerItemInvincibleSet(GMS_PLAYER_WORK *ply_work)
{
	MTM_ASSERT(ply_work->obj_work.obj_type == GMD_OBJTYPE_PLAYER);
	// SE

	// BGM変更
	GmSoundPlayJingleInvincible();

	// 無敵エフェクト
	if (!ply_work->genocide_timer) {
		GmPlyEfctCreateInvincible(ply_work);
	}

	// 無敵タイマーセット
	ply_work->genocide_timer = GMD_PL_ITEM_GENOCIDE_TIME * FX32_ONE;
#if 0
    // SE
    GmSoundPlayArcSe( NULL, Game_SE, SE_Muteki );
    // BGM設定
    gmBgmChangeInit( pPlayer, SEQ_muteki );
    pPlayer->water_timer = 0;
    // 無敵タイマーセット
    pPlayer->genocide_timer = 999 * FX32_ONE;
	GmEffectInitPlayerNoEnemy(pPlayer);
#endif
}

// ================================================================
// GmPlayerItemRing10Set
/*!
	アイテム リング10取得
 
	@param ply_work  [io] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerItemRing10Set(GMS_PLAYER_WORK *ply_work)
{
	MTM_ASSERT(ply_work->obj_work.obj_type == GMD_OBJTYPE_PLAYER);

	// リング取得
	GmPlayerRingGet(ply_work, 10);
}

// ================================================================
// GmPlayerItemBarrierSet
/*!
	アイテム バリア取得
 
	@param ply_work  [io] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerItemBarrierSet(GMS_PLAYER_WORK *ply_work)
{
	MTM_ASSERT(ply_work->obj_work.obj_type == GMD_OBJTYPE_PLAYER);

	// バリア取得チェック
	if ( !(ply_work->player_flag & (GMD_PLF_BARRIER/* | GMD_PLF_MAGNET*/)) ){
		// SE
		//GmSoundPlayArcSe( NULL, Game_SE, SE_Barrier );

		// バリアエフェクト
		GmPlyEfctCreateBarrier(ply_work);

		// SE
		GmSoundPlaySE("Barrier");
	} 
	ply_work->player_flag |= GMD_PLF_BARRIER;
}

// ================================================================
// GmPlayerItem1UPSet
/*!
	アイテム 1UP取得
 
	@param ply_work  [io] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerItem1UPSet(GMS_PLAYER_WORK *ply_work)
{
	MTM_ASSERT(ply_work->obj_work.obj_type == GMD_OBJTYPE_PLAYER);

	// プレイヤーストック加算
	GmPlayerStockGet(ply_work, 1);

	// ジングル
	GmSoundPlayJingle1UP(TRUE);
#if 0
    // バリアエフェクト
    if ( !(pPlayer->player_flag & (GMD_PLF_BARRIER | GMD_PLF_MAGNET)) ){
        // SE
        GmSoundPlayArcSe( NULL, Game_SE, SE_Barrier );

		// バリアエフェクト
		GmEffectInitPlayerBarrier(pPlayer);
    } 
    pPlayer->player_flag |= GMD_PLF_BARRIER;
#endif
}



#if 0
// ================================================================
// GmPlayerBarrierMagnetSet
/*!
  磁力バリア取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerBarrierMagnetSet( GMS_PLAYER_WORK *pPlayer )
{
#if 0
    if ( !(pPlayer->player_flag & GMD_PLF_MAGNET)){
        // SE
        GmSoundPlayArcSe( NULL, Game_SE, SE_Barrier );

		// 磁石バリアエフェクト
		GmEffectInitPlayerMagnetBarrier(pPlayer);
    }
    pPlayer->player_flag |= GMD_PLF_BARRIER | GMD_PLF_MAGNET;
#endif
}

// ================================================================
// GmPlayerHyperSpeedTrickSet
/*!
  高速トリック取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerHyperSpeedTrickSet( GMS_PLAYER_WORK *pPlayer )
{
#if 0
	if (!pPlayer->hs_trick_timer) {
		// モーション速度設定
		pPlayer->obj.obj_3d->act_3d.master_anim_spd = (pPlayer->obj.obj_3d->act_3d.master_anim_spd << 1) +
									(pPlayer->obj.obj_3d->act_3d.master_anim_spd >> 1);
		pPlayer->obj.obj_2d->act_spr.act.speed = pPlayer->obj.obj_3d->act_3d.master_anim_spd;

		if (g_gm_player_action3d_opt_ca_tbl[pPlayer->char_no]) {	// オプションモデルモーションテーブルが存在する場合
			pPlayer->obj_3d_opt.act_3d.master_anim_spd	= pPlayer->obj.obj_3d->act_3d.master_anim_spd;
		}
	}

    // 高速トリックタイマーセット
	pPlayer->hs_trick_timer = GMD_PLAYER_HS_TRICK_TIME;
#endif
}

// ================================================================
// GmPlayerConfusionSet
/*!
  混乱取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerConfusionSet( GMS_PLAYER_WORK *pPlayer )
{
#if 0
    // 混乱エフェクト表示
	GmEffectInitPlayerVSItemEffect(pPlayer, GMD_EFFECT_VS_ITEM_EFFECT_TYPE_CONFUSION);
    // 混乱設定
    pPlayer->confusion_timer = GMD_PLAYER_ITEM_CONFUSION_TIME;
#endif
}
// ================================================================
// GmPlayerTensionDownSet
/*!
  テンションダウン取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerTensionDownSet( GMS_PLAYER_WORK *pPlayer )
{
#if 0
    // 自分の時
    if ( !pPlayer->player_id ) {
		GmSoundPlayArcSe( &pPlayer->h_snd_gallery, Game_SE, SE_Trick_miss );
		//GmSoundGameSeLoopInit(Game_SE, SE_Trick_miss, 128);
		//GmSoundGameSeSpInit(Game_SE, SE_Trick_miss, GMD_SOUND_SE_SP_FLAG_LOOP, NULL, 128, 0);
	}

    // エフェクト表示
	GmEffectInitPlayerVSItemEffect(pPlayer, GMD_EFFECT_VS_ITEM_EFFECT_TYPE_TENSION_DOWN);
    // テンション減少
    GmPlayerTensionGet( pPlayer,-GMD_PLAYER_TENSION_MAX );
	GmFixSetTensionAct((s16)(pPlayer->tension >> GMD_PLAYER_TENSION_SHIFT), GMD_FIX_TENSION_DEC_ANIM_VIBRATION);
#endif
}

// ================================================================
// GmPlayerWarpSet
/*!
  ワープ取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
void GmPlayerWarpSet( GMS_PLAYER_WORK *pPlayer )
{
#if 0
    if ( pPlayer->player_flag & GMD_PLF_GOAL )
        return;

    // 設定面変化
    if ( g_gm_main_ply_obj_list[1]->gmk_flag & GMD_PLGF_GMK_B)
        pPlayer->obj.flag |= OBD_OBJECT_B;
    else
        pPlayer->obj.flag &= ~OBD_OBJECT_B;
    // ワープ設定
    pPlayer->warp_pos_x = g_gm_main_ply_obj_list[1]->obj.pos.x;
    pPlayer->warp_pos_y = g_gm_main_ply_obj_list[1]->obj.pos.y;

    // ワープ初期化
    GmPlayerWarpInit(pPlayer);
#endif
}
#endif

// =====================================================================
// プレイヤーアクション設定
// =====================================================================
// ================================================================
// GmPlayerActionChange
/*!
  アクションチェンジ

  @param ply_work   [in] プレイヤーワークポインタ
  @param act_state	[in] 設定する状態インデクス

 */
// ================================================================
void GmPlayerActionChange(GMS_PLAYER_WORK *ply_work, GME_PLY_ACT_STATE act_state)
{
	s32	act_id;

	ply_work->prev_act_state	= ply_work->act_state;
	ply_work->act_state			= act_state;

	// モデル設定
	MTM_ASSERT(*(g_gm_player_model_tbl[ply_work->char_id] + act_state) < GMD_PLY_MODEL_TYPE_MAX);
	ply_work->obj_work.obj_3d =
		ply_work->obj_3d[*(g_gm_player_model_tbl[ply_work->char_id] + act_state)];
	// モーション保持モデルの差し替え
	ply_work->obj_work.obj_3d->motion->object = ply_work->obj_work.obj_3d->object;


	// モーションワークコピー
	//ply_work->obj_3d[1].motion = ply_work->obj_3d[0].motion;

	// アクション設定
	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
		// 左向き
		act_id = *(g_gm_player_motion_left_tbl[ply_work->char_id] + act_state);
	}
	else {
		// 右向き
		act_id = *(g_gm_player_motion_right_tbl[ply_work->char_id] + act_state);
	}

	if (ply_work->prev_act_state != GME_PLY_ACT_STATE_INVALID &&
			*(g_gm_player_model_tbl[ply_work->char_id] + act_state) ==
					*(g_gm_player_model_tbl[ply_work->char_id] + ply_work->prev_act_state) &&
					//g_gm_player_mtn_blend_setting[ply_work->prev_act_state] &&
					//g_gm_player_mtn_blend_setting[act_state]) {
					*(g_gm_player_mtn_blend_setting_tbl[ply_work->char_id] + ply_work->prev_act_state) &&
					*(g_gm_player_mtn_blend_setting_tbl[ply_work->char_id] + act_state)) {
		// ブレンドあり
#if 0
		ObjDrawObjectActionSet3DNN(&ply_work->obj_work,
					act_id,
					0/*mbuf_id*/);
#else
		ObjDrawObjectActionSet3DNNBlend(&ply_work->obj_work, act_id);
#endif
		// ブレンド速度設定◆暫定
		if (act_state == GME_PLY_ACT_STATE_SPIN ||
				act_state == GME_PLY_ACT_STATE_SPIN_SMALL) {
			ply_work->obj_work.obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD / 2;
		}
		else if (GME_PLY_ACT_STATE_WALK <= ply_work->prev_act_state && ply_work->prev_act_state < GME_PLY_ACT_STATE_DASH_2 &&
				(act_state == GME_PLY_ACT_STATE_JUMP_FALL || act_state == GME_PLY_ACT_STATE_JUMP_FALL_R)) {
			ply_work->obj_work.obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD / 2;
		}
		else if (ply_work->prev_act_state == GME_PLY_ACT_STATE_FW && act_state == GME_PLY_ACT_STATE_WALK) {
			// ブレーキターンからのFW>Walkが目立つ為
			ply_work->obj_work.obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD / 2;
		}
		else if (ply_work->prev_seq_state == GME_PLY_SEQ_STATE_HOMING_REF) {
			ply_work->obj_work.obj_3d->blend_spd = 1.f / 12.f;
		}
		else {
			ply_work->obj_work.obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD;
		}
	}
	else {
		// 通常設定
		ObjDrawObjectActionSet3DNN(&ply_work->obj_work,
					act_id,
					0/*mbuf_id*/);
	}

#if _WII
	// 手ロッドアクション設定
	gmPlayerSetLodAction(ply_work, act_id);
#endif



#if 0
    s16 state_prev = ply_work->act_state;
	u8	char_no = ply_work->char_id;
	fx32	anim_spd;

	if (ply_work->hs_trick_timer) {
		// 高速トリック時
		anim_spd = (fx32)(FX32_ONE * GMD_PLAYER_HS_TRICK_ANIME_SPD_MAG);
	}
	else {
		anim_spd = FX32_ONE;
	}

    ply_work->act_state = act_state;

    // フラグ落とし
    ply_work->obj_work.disp_flag &= ~(OBD_DISP_END | OBD_DISP_REPEAT);
    //ply_work->obj_work.disp_flag &= ~OBD_DISP_REPEAT;

    // 当たりフラグ落とし
    ply_work->rect_work[0].flag &= ~OBD_RECT_ENABLE;
    ply_work->rect_work[1].flag &= ~OBD_RECT_ENABLE;

    // メインモデル
    if ( ply_work->obj_work.obj_3d ) {
        // モデルチェンジチェック
        if ( *(g_gm_player_action3d_md_tbl[char_no] + state_prev) != *(g_gm_player_action3d_md_tbl[char_no] + act_state)){
            void * pTempMemJnt,*pTempMemMat;
            pTempMemJnt = ply_work->obj_3d.act_3d.ro.recJntAnm;
            pTempMemMat = ply_work->obj_3d.act_3d.ro.recMatAnm;
            // モデルセットアップ
            ObjObjectAction3dModelSet( &ply_work->obj, *(g_gm_player_action3d_md_tbl[char_no] + act_state));
            ply_work->obj_work.disp_flag &= ~OBD_DISP_3D_BLEND;
            ply_work->obj_3d.act_3d.ro.recJntAnm = pTempMemJnt;
            ply_work->obj_3d.act_3d.ro.recMatAnm = pTempMemMat;
            ply_work->player_flag |= GMD_PLF_AFTERIMAGE_UPDATE;
        }

		// ブレンディング設定
		if ( !ply_work->player_id && (ply_work->obj_work.disp_flag & OBD_DISP_3D_BLEND) ) {
			ply_work->obj_work.obj_3d->act_3d.aflag[MTE_ACT3D_NNS_ANIM_CA] |= MTD_ACT3D_NNS_AFLAG_BLEND;
            // ply_work->obj_work.obj_3d->act_3d.aflag[MTE_ACT3D_NNS_ANIM_CA] &= ~MTD_ACT3D_NNS_AFLAG_BLEND_END;
		}
		else {
			ply_work->obj_work.obj_3d->act_3d.aflag[MTE_ACT3D_NNS_ANIM_CA] &= ~MTD_ACT3D_NNS_AFLAG_BLEND;
		}

		// CAモーション
		if (!(GME_PLY_ACT_STATE_GMK_DHB_START <= act_state && act_state <= GME_PLY_ACT_STATE_GMK_DHB_END-1/*最後のモーションは通常モーション使用*/)) {
			// 通常	
			ObjObjectAction3dSet(&ply_work->obj, MTE_ACT3D_NNS_ANIM_CA, *(g_gm_player_action3d_ca_tbl[char_no] + act_state) );
		}
		else {
			// 滑降ボードモーション
			mtAct3dSetAnimNNS(&ply_work->obj_work.obj_3d->act_3d, MTE_ACT3D_NNS_ANIM_CA,
                          ply_work->opt_anime, *(g_gm_player_action3d_ca_tbl[char_no] + act_state), NULL);
		}
#if GMD_PLY_DAT_IVA_ENABLE
        // VAモーション
		if (*(g_gm_player_action3d_va_tbl[char_no] + act_state) &&
				(*(g_gm_player_action3d_va_tbl[char_no] + act_state) != *(g_gm_player_action3d_va_tbl[char_no] + state_prev)) ) {
			ObjObjectAction3dSet(&ply_work->obj, MTE_ACT3D_NNS_ANIM_VA, *(g_gm_player_action3d_va_tbl[char_no] + act_state));
		}
#endif	// #if GMD_PLY_DAT_IVA_ENABLE
        ply_work->obj_work.obj_3d->act_3d.master_anim_spd = anim_spd;
        ply_work->obj_work.disp_flag &= ~OBD_DISP_3D_BLEND;
    }

	// オプションモデル
	if (g_gm_player_action3d_opt_ca_tbl[char_no]) {	// オプションモデルモーションテーブルが存在する場合
		if (*(g_gm_player_action3d_opt_ca_tbl[char_no] + act_state) != GMD_PLY_DAT_MTN_NO_NULL) {
			ply_work->player_flag |= GMD_PLF_OPT_MDL_ENABLE;			// オプションモデル有効

			// アクションチェンジ
			if (!(GME_PLY_ACT_STATE_GMK_DHB_START <= act_state && act_state <= GME_PLY_ACT_STATE_GMK_DHB_END-1/*最後のモーションは通常モーション使用*/)) {
				// 通常
				mtAct3dSetAnimNNS(&ply_work->obj_3d_opt.act_3d, MTE_ACT3D_NNS_ANIM_CA,
						ply_work->obj_3d_opt.anime[MTE_ACT3D_NNS_ANIM_CA], *(g_gm_player_action3d_opt_ca_tbl[char_no] + act_state), NULL);
			}
			else {
				// 滑降ボードモーション
				mtAct3dSetAnimNNS(&ply_work->obj_3d_opt.act_3d, MTE_ACT3D_NNS_ANIM_CA,
						ply_work->opt_anime, *(g_gm_player_action3d_opt_ca_tbl[char_no] + act_state), NULL);
			}


			// ブレンディング設定
			if ( !ply_work->player_id && (ply_work->obj_work.disp_flag & OBD_DISP_3D_BLEND) ) {
				ply_work->obj_3d_opt.act_3d.aflag[MTE_ACT3D_NNS_ANIM_CA] |= MTD_ACT3D_NNS_AFLAG_BLEND;
			}
			else {
				ply_work->obj_3d_opt.act_3d.aflag[MTE_ACT3D_NNS_ANIM_CA] &= ~MTD_ACT3D_NNS_AFLAG_BLEND;
			}

			ply_work->obj_3d_opt.act_3d.master_anim_spd = anim_spd;
		}
		else {
			ply_work->player_flag &= ~GMD_PLF_OPT_MDL_ENABLE;			// オプションモデル無効
		}
	}

    // 2D矩形設定
    {
		MTS_ACTION_DS	*act_ds = &ply_work->obj_work.obj_2d->act_spr;
	    act_ds->act.speed = anim_spd;
	    mtActResetStructDS(act_ds, (u16)*(g_gm_player_action_tbl[char_no] + act_state));
    }
#endif
}


// ================================================================
// GmPlayerSaveResetAction
/*!
  アクション再設定用情報保存

  @param ply_work		[in] プレイヤーワークポインタ
  @param reset_act_work	[in] アクションリセット用情報ワーク

  @note
	アクション再設定用情報を保存します。

 */
// ================================================================
void GmPlayerSaveResetAction(GMS_PLAYER_WORK *ply_work, GMS_PLAYER_RESET_ACT_WORK *reset_act_work)
{
	MTM_ASSERT(ply_work);
	MTM_ASSERT(reset_act_work);

	// 情報保存
	reset_act_work->frame[0] = ply_work->obj_work.obj_3d->frame[0];
	reset_act_work->frame[1] = ply_work->obj_work.obj_3d->frame[1];
	reset_act_work->blend_spd	= ply_work->obj_work.obj_3d->blend_spd;
	reset_act_work->marge		= ply_work->obj_work.obj_3d->marge;
	reset_act_work->obj_3d_flag = ply_work->obj_work.obj_3d->flag;
}

// ================================================================
// GmPlayerResetAction
/*!
  アクション再設定

  @param ply_work   [in] プレイヤーワークポインタ

  @note
	現在の設定でアクションを再設定します。

 */
// ================================================================
void GmPlayerResetAction(GMS_PLAYER_WORK *ply_work, GMS_PLAYER_RESET_ACT_WORK *reset_act_work)
{
#if 1
	s32					i;
	GME_PLY_ACT_STATE	act_state[2];
	u32					disp_flag;
	float				max_frame[2];

	MTM_ASSERT(ply_work);
	MTM_ASSERT(reset_act_work);


	// 現在のアクションを設定しなおす
	act_state[0] = ply_work->act_state;
	act_state[1] = ply_work->prev_act_state;
	disp_flag	= ply_work->obj_work.disp_flag;

	GmPlayerActionChange(ply_work, act_state[1]);
	GmPlayerActionChange(ply_work, act_state[0]);
	ply_work->obj_work.obj_3d->frame[0] = reset_act_work->frame[0];
	ply_work->obj_work.obj_3d->frame[1] = reset_act_work->frame[1];
	ply_work->obj_work.obj_3d->blend_spd	= reset_act_work->blend_spd;
	ply_work->obj_work.obj_3d->marge		= reset_act_work->marge;
	ply_work->obj_work.obj_3d->flag			&= ~OBD_ACTFLAG_3D_NN_BLEND;
	ply_work->obj_work.obj_3d->flag			|= reset_act_work->obj_3d_flag & OBD_ACTFLAG_3D_NN_BLEND;
	ply_work->obj_work.disp_flag			|= disp_flag & (OBD_DISP_REPEAT | OBD_DISP_END);

	for (i = 0; i < 2; i++) {
		max_frame[i] = amMotionGetEndFrame(ply_work->obj_work.obj_3d->motion, ply_work->obj_work.obj_3d->act_id[i]) -
							amMotionGetStartFrame(ply_work->obj_work.obj_3d->motion, ply_work->obj_work.obj_3d->act_id[i]);

		if (ply_work->obj_work.obj_3d->frame[i] >= max_frame[i]) {
			ply_work->obj_work.obj_3d->frame[i] = 0.f;
		}
	}




#else
	float				frame[2], blend_spd, marge;
	GME_PLY_ACT_STATE	act_state[2];
	u32					disp_flag, obj_3d_flag;

	// 現在のアクションを設定しなおす
	act_state[0] = ply_work->act_state;
	act_state[1] = ply_work->prev_act_state;
	frame[0] = ply_work->obj_work.obj_3d->frame[0];
	frame[1] = ply_work->obj_work.obj_3d->frame[1];
	blend_spd	= ply_work->obj_work.obj_3d->blend_spd;
	marge		= ply_work->obj_work.obj_3d->marge;
	disp_flag	= ply_work->obj_work.disp_flag;
	obj_3d_flag = ply_work->obj_work.obj_3d->flag;

	GmPlayerActionChange(ply_work, act_state[1]);
	GmPlayerActionChange(ply_work, act_state[0]);
	ply_work->obj_work.obj_3d->frame[0] = frame[0];
	ply_work->obj_work.obj_3d->frame[1] = frame[1];
	ply_work->obj_work.obj_3d->blend_spd	= blend_spd;
	ply_work->obj_work.obj_3d->marge		= marge;
	ply_work->obj_work.obj_3d->flag			&= ~OBD_ACTFLAG_3D_NN_BLEND;
	ply_work->obj_work.obj_3d->flag			|= obj_3d_flag & OBD_ACTFLAG_3D_NN_BLEND;
	ply_work->obj_work.disp_flag			|= disp_flag & (OBD_DISP_REPEAT | OBD_DISP_END);
#endif
}


// ==========================================================================
// GmPlayerWalkActionSet
/*!
 *	速度に合わせて歩きアクションを設定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlayerWalkActionSet(GMS_PLAYER_WORK *ply_work)
{
	fx32				spd = MTM_MATH_ABS(ply_work->obj_work.spd_m);
	GME_PLY_ACT_STATE	act_state = GME_PLY_ACT_STATE_WALK;
	BOOL				b_max_dash = FALSE;
	s16					dir_z = (s16)ply_work->obj_work.dir.z;

	// 最大ダッシュ可能状態チェック
	if ((!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && dir_z > GMD_PL_MAXDASH_DIR) ||
			((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && dir_z < -GMD_PL_MAXDASH_DIR) &&
			!(ply_work->gmk_flag & GMD_PLGF_GMK_NO_MAXDASH)) {
		b_max_dash = TRUE;
		// MAXダッシュ有効時間更新
		ply_work->maxdash_timer = GMD_PL_MAXDASH_TIME*FX32_ONE;
	}

	if ( spd < ply_work->spd1 ) {
		act_state = GME_PLY_ACT_STATE_WALK;
	}
	else if ( spd < ply_work->spd2 ) {
		act_state = GME_PLY_ACT_STATE_RUN;

		if (!(ply_work->player_flag & GMD_PLF_WALK_SMK_EFCT_OFF)) {
			// エフェクト 砂煙
			GmPlyEfctCreateRunDust(ply_work);
		}
	}
	else if ( spd < ply_work->spd3 ) {
		act_state = GME_PLY_ACT_STATE_DASH_1;

		if (!(ply_work->player_flag & GMD_PLF_WALK_SMK_EFCT_OFF)) {
			// エフェクト ダッシュ1 砂煙
			GmPlyEfctCreateDash1Dust(ply_work);
		}
	}
	else if ( spd < ply_work->spd4 && (b_max_dash || ply_work->maxdash_timer)) {
		act_state = GME_PLY_ACT_STATE_DASH_2;
		// エフェクト 回転足
		GmPlyEfctCreateRollDash(ply_work);

		if (!(ply_work->player_flag & GMD_PLF_WALK_SMK_EFCT_OFF)) {
			// エフェクト ダッシュ2 砂煙
			GmPlyEfctCreateDash2Dust(ply_work);
		}
		// エフェクト ダッシュ2 衝撃波
		GmPlyEfctCreateDash2Impact(ply_work);
		// エフェクト スーパーソニックオーラ
		GmPlyEfctCreateSuperAuraDash(ply_work);
	}
	else{
		// DASH2になれなかった
		act_state = GME_PLY_ACT_STATE_DASH_1;
		// エフェクト ダッシュ1 砂煙
		if (!(ply_work->player_flag & GMD_PLF_WALK_SMK_EFCT_OFF)) {
			GmPlyEfctCreateDash1Dust(ply_work);
		}
	}

	GmPlayerActionChange(ply_work, act_state);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;


//	if (ply_work->act_state == GME_PLY_ACT_STATE_DASH_2 &&
//			b_max_dash) {
//		// MAXダッシュ有効時間更新
//		ply_work->maxdash_timer = GMD_PL_MAXDASH_TIME*FX32_ONE;
//	}
}

// ================================================================
// GmPlayerWalkActionCheck
/*!
  速度に合わせて歩きアクションを変化させていく
 */
// ================================================================
void GmPlayerWalkActionCheck( GMS_PLAYER_WORK *ply_work )
{
	BOOL	b_max_dash = FALSE;
	s16		dir_z = (s16)ply_work->obj_work.dir.z;
    fx32	fSpd = MTM_MATH_ABS(ply_work->obj_work.spd_m);

    // 速度に合わせたアクション設定

	// ◆スーパーソニックの時は変更しない？

	// 最大ダッシュ可能状態チェック
	if (((!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && dir_z > GMD_PL_MAXDASH_DIR) ||
			((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && dir_z < -GMD_PL_MAXDASH_DIR)) &&
			!(ply_work->gmk_flag & GMD_PLGF_GMK_NO_MAXDASH)) {
		b_max_dash = TRUE;
		// MAXダッシュ有効時間更新
		ply_work->maxdash_timer = GMD_PL_MAXDASH_TIME*FX32_ONE;
	}

    // 歩き以外のモーション時
    if ( ply_work->act_state < GME_PLY_ACT_STATE_WALK || ply_work->act_state > GME_PLY_ACT_STATE_DASH_2) {
        GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_WALK );
	}

	if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
		// 速度1の場合でアニメ終了時、速度をチェックしてアニメ変更
		if ( ply_work->act_state == GME_PLY_ACT_STATE_WALK ) {
			if ( fSpd >= ply_work->spd2 ) {
				GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_RUN );

				if (!(ply_work->player_flag & GMD_PLF_WALK_SMK_EFCT_OFF)) {
					// エフェクト 砂煙
					GmPlyEfctCreateRunDust(ply_work);
				}
			}
		}
		// 速度2の場合でアニメ終了時、速度をチェックしてアニメ変更
		else if ( ply_work->act_state == GME_PLY_ACT_STATE_RUN ) {
			if ( fSpd >= ply_work->spd3 ) {
				GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_DASH_1 );

				if (!(ply_work->player_flag & GMD_PLF_WALK_SMK_EFCT_OFF)) {
					// エフェクト ダッシュ1 砂煙
					GmPlyEfctCreateDash1Dust(ply_work);
				}
			}
			else if (fSpd < ply_work->spd2) {
				// 速度ダウン時
				GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_WALK );
			}
		}
		// 速度3の場合でアニメ終了時、速度をチェックしてアニメ変更
		else if ( ply_work->act_state == GME_PLY_ACT_STATE_DASH_1 ) {
			if ( fSpd >= ply_work->spd_max && (b_max_dash || ply_work->maxdash_timer)) {
				GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_DASH_2 );
				// エフェクト 回転足
				GmPlyEfctCreateRollDash(ply_work);

				if (!(ply_work->player_flag & GMD_PLF_WALK_SMK_EFCT_OFF)) {
					// エフェクト ダッシュ2 砂煙
					GmPlyEfctCreateDash2Dust(ply_work);
				}
				// エフェクト ダッシュ2 衝撃波
				GmPlyEfctCreateDash2Impact(ply_work);
			}
			else if (fSpd < ply_work->spd3) {
				// 速度ダウン時
				GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_RUN );

				if (!(ply_work->player_flag & GMD_PLF_WALK_SMK_EFCT_OFF)) {
					// エフェクト 砂煙
					GmPlyEfctCreateRunDust(ply_work);
				}
			}
		}
		// 速度4の場合でアニメ終了時、速度をチェックしてアニメ変更
		else if ( ply_work->act_state == GME_PLY_ACT_STATE_DASH_2 ) {
			if ( fSpd < ply_work->spd_max || !(b_max_dash || ply_work->maxdash_timer)) {
				// 速度ダウン時, 強制的にアクションOFF時
				GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_DASH_1 );

				if (!(ply_work->player_flag & GMD_PLF_WALK_SMK_EFCT_OFF)) {
					// エフェクト ダッシュ1 砂煙
					GmPlyEfctCreateDash1Dust(ply_work);
				}
			}
		}
	}

//	if (ply_work->act_state == GME_PLY_ACT_STATE_DASH_2 &&
//			b_max_dash) {
//		// MAXダッシュ有効時間更新
//		ply_work->maxdash_timer = GMD_PL_MAXDASH_TIME*FX32_ONE;
//	}

    ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
}



#if 0
// ================================================================
// GmPlayerSetActionFrame
/*!
  プレイヤーアクションフレーム設定

	@param	ply_work	[in]	プレイヤーワークポインタ
	@param	frame		[in]	設定フレーム

	@note
		frame < 0 で最終フレームに設定
 */
// ================================================================
void GmPlayerSetActionFrame(GMS_PLAYER_WORK *ply_work, fx32 frame)
{
#if 0
	if (frame < 0 || frame >= NNS_G3dAnmObjGetNumFrame(ply_work->obj_work.obj_3d->act_3d.aobj[0])) {
		frame = NNS_G3dAnmObjGetNumFrame(ply_work->obj_work.obj_3d->act_3d.aobj[0]) - 1;
	}
	ply_work->obj_work.obj_3d->act_3d.aobj[0]->frame = frame;

	// オプションモデル
	if (ply_work->player_flag & GMD_PLF_OPT_MDL_ENABLE) {
		ply_work->obj_3d_opt.act_3d.aobj[0]->frame = frame;
	}
#endif
}
#endif

// =====================================================================
// プレイヤー状態 アクションステータス設定
// =====================================================================
// ================================================================
// GmPlayerAnimeSpeedSetWalk
/*!
  アニメーションスピード設定 歩き系

  @param ply_work   [io] プレイヤーポインタ
  @param spd_set	[in] アニメ速度の基準となる数値
    
 */
// ================================================================
void GmPlayerAnimeSpeedSetWalk(GMS_PLAYER_WORK *ply_work, fx32 spd_set )
{
	fx32	spd;

	spd = (fx32)MTM_MATH_ABS((spd_set >> 3) + (spd_set >> 2));

	// スーパーソニックのときは調整

	if (spd <= 1 * FX32_ONE) {
		spd = 1 * FX32_ONE;
	}
	if (spd >= 8 * FX32_ONE) {
		spd = 8 * FX32_ONE;
	}

#if 0
	if (ply_work->hs_trick_timer) {
		// 高速トリック時
		// GMD_PLAYER_HS_TRICK_ANIME_SPD_MAG
		fSpd = (fSpd << 1) + (fSpd >> 1);
	}
#endif

	if (ply_work->act_state == GME_PLY_ACT_STATE_DASH_2) {
		// 最大ダッシュの時は等速
		spd = FX32_ONE;
	}
	else if ((ply_work->act_state == GME_PLY_ACT_STATE_SPIN ||
				ply_work->act_state == GME_PLY_ACT_STATE_SPIN_SMALL) &&
				(ply_work->obj_work.obj_3d->flag & OBD_ACTFLAG_3D_NN_BLEND) &&
				spd > FX32_ONE) {
		// スピン補間中は等速
		spd = FX32_ONE;
	}

	// 速度設定
	// 矩形情報

	// メインモデル
	if (ply_work->obj_work.obj_3d) {
		ply_work->obj_work.obj_3d->speed[0] = FXM_FX32_TO_FLOAT(spd);
		ply_work->obj_work.obj_3d->speed[1] = FXM_FX32_TO_FLOAT(spd);
	}
}


// ================================================================
// GmPlayerSpdSet
/*!
  プレイヤーの速度を設定する

  @param ply_work  [in] プレイヤーワークポインタ
  @param spd_x [in] ジャンプ速度X
  @param spd_y [in] 

  @note
 */
// ================================================================
void GmPlayerSpdSet(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y)
{
    ply_work->no_spddown_timer = 128*FX32_ONE;
    
    if ( spd_x < 0 ){
        ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
    }
    else{
        ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
    }
    
    // 速度設定
    if ( ply_work->obj_work.move_flag & OBD_MOVE_JUMP ){
        if ( ( (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd.x > spd_x) ||
             (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd.x < spd_x) )
            ply_work->obj_work.spd.x = spd_x;
        if ( MTM_MATH_ABS(ply_work->obj_work.spd.y) < MTM_MATH_ABS(spd_y) )
            ply_work->obj_work.spd.y = spd_y;
    }
    else{
        switch( (((ply_work->obj_work.dir.z + 0x2000) & 0xc000) >> 6)){
        case 0:
            // 左が進行方向
        case 2:
            // 右が進行方向
            // 左右が進行方向
            if ( ( (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m > spd_x) ||
                 (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m < spd_x) )
                ply_work->obj_work.spd_m = spd_x;
            if ( MTM_MATH_ABS(ply_work->obj_work.spd.y) < MTM_MATH_ABS(spd_y) ){
                ply_work->obj_work.spd.y = spd_y;
                if ( ply_work->obj_work.spd.y < 0 )
                    ply_work->obj_work.move_flag |= OBD_MOVE_JUMP;
            }
            break;
        case 1:
            // 下が進行方向
            if ( ( (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m > spd_y) ||
                 (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m < spd_y) ){
                ply_work->obj_work.spd_m = spd_y;
            }
            
            if ( MTM_MATH_ABS(ply_work->obj_work.spd.x) < MTM_MATH_ABS(spd_x) )
                ply_work->obj_work.spd.x = spd_x;
            break;
        case 3:
            // 上が進行方向
            if ( ( (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m > -spd_y) ||
                 (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m < -spd_y) ){
                ply_work->obj_work.spd_m = -spd_y;
            }
            
            if ( MTM_MATH_ABS(ply_work->obj_work.spd.x) < MTM_MATH_ABS(spd_x) )
                ply_work->obj_work.spd.x = spd_x;
            break;
        }
        
    }
    
    // ブースト
   // GmPlayerBoostInit(ply_work);
}

// ==========================================================================
// GmPlayerSetReverse
/*!
 *	オブジェクト反転
 *
 *	@param	lact_work	[in]	リンクアクション管理ワーク
 *
 *	@note
 *		キーコマンドの再設定も行います
 */
// ==========================================================================
void GmPlayerSetReverse(GMS_PLAYER_WORK *ply_work)
{
	// プログラムターンOFF
	ply_work->player_flag &= ~GMD_PLF_PGM_TURN_MASK;
	ply_work->pgm_turn_dir = 0;
	ply_work->pgm_turn_spd = 0;

	ply_work->obj_work.disp_flag ^= OBD_DISP_HFLIP;

	// モーションに左右設定がある場合は切り替える。
	// フレーム保持を行うこと◆
	// モーションブレンドはカット
	if (*(g_gm_player_motion_left_tbl[ply_work->char_id] + ply_work->act_state) !=
				*(g_gm_player_motion_right_tbl[ply_work->char_id] + ply_work->act_state)) {
		float	frame[2];
		u32		disp_flag = ply_work->obj_work.disp_flag & (OBD_DISP_END | OBD_DISP_REPEAT);

		frame[0] = ply_work->obj_work.obj_3d->frame[0];

		GmPlayerActionChange(ply_work, ply_work->act_state);

		// フレーム復帰
		ply_work->obj_work.obj_3d->frame[0] = frame[0];

		// ブレンドカット
		ply_work->obj_work.obj_3d->marge = 0.f;
		ply_work->obj_work.obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_BLEND;

		// disp_flag復帰
		ply_work->obj_work.disp_flag |= disp_flag;
	}

	// その他の設定
	// キー反転とか必要？
}

// ==========================================================================
// GmPlayerSetReverseOnlyState
/*!
 *	オブジェクト反転 ステータスのみ
 *
 *	@param	lact_work	[in]	リンクアクション管理ワーク
 *
 *	@note
 *		後でアクションが変更されるものとし、ステータスのみ変換します。\n
 *		キーコマンドの再設定も行います
 */
// ==========================================================================
void GmPlayerSetReverseOnlyState(GMS_PLAYER_WORK *ply_work)
{
	// プログラムターンOFF
	ply_work->player_flag &= ~GMD_PLF_PGM_TURN_MASK;
	ply_work->pgm_turn_dir = 0;
	ply_work->pgm_turn_spd = 0;

	ply_work->obj_work.disp_flag ^= OBD_DISP_HFLIP;

}

// ================================================================
// キー入力関係
// ================================================================
// ================================================================
// GmPlayerKeyCheckWalkLeft
/*!
  プレイヤーキーチェック 歩き左
  
  @param    player  [in] 対象プレイヤー
  
  @return   TRUE : 左歩き入力あり
 */
// ================================================================
BOOL GmPlayerKeyCheckWalkLeft(GMS_PLAYER_WORK *ply_work)
{
#if GMD_PLY_USE_ROT_Z_TRUCK_MOVE
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		if (ply_work->key_on & PAD_KEY_LEFT ||
				ply_work->key_rot_z < 0) {
			return (TRUE);
		}
	}
	else if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
		if (ply_work->key_on & PAD_KEY_LEFT) {
			return (TRUE);
		}
	}
	else {
		if (ply_work->key_on & PAD_KEY_LEFT ||
				ply_work->key_walk_rot_z < 0) {
			return (TRUE);
		}
	}
#else
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
		if (ply_work->key_on & PAD_KEY_LEFT) {
			return (TRUE);
		}
	}
	else {
		if (ply_work->key_on & PAD_KEY_LEFT ||
				ply_work->key_walk_rot_z < 0) {
			return (TRUE);
		}
	}
#endif

	return (FALSE);
}

// ================================================================
// GmPlayerKeyCheckWalkRight
/*!
  プレイヤーキーチェック 歩き右
  
  @param    player  [in] 対象プレイヤー
  
  @return   TRUE : 左歩き入力あり
 */
// ================================================================
BOOL GmPlayerKeyCheckWalkRight(GMS_PLAYER_WORK *ply_work)
{
#if GMD_PLY_USE_ROT_Z_TRUCK_MOVE
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		if (ply_work->key_on & PAD_KEY_RIGHT ||
				ply_work->key_rot_z > 0) {
			return (TRUE);
		}
	}
	else if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
		if (ply_work->key_on & PAD_KEY_RIGHT) {
			return (TRUE);
		}
	}
	else {
		if (ply_work->key_on & PAD_KEY_RIGHT ||
				ply_work->key_walk_rot_z > 0) {
			return (TRUE);
		}
	}
#else
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
		if (ply_work->key_on & PAD_KEY_RIGHT) {
			return (TRUE);
		}
	}
	else {
		if (ply_work->key_on & PAD_KEY_RIGHT ||
				ply_work->key_walk_rot_z > 0) {
			return (TRUE);
		}
	}
#endif

	return (FALSE);
}

// ================================================================
// GmPlayerKeyCheckJumpKeyOn
/*!
  プレイヤーキーチェック ジャンプ キーON
  
  @param    player  [in] 対象プレイヤー
  
  @return   TRUE : ジャンプ入力あり
 */
// ================================================================
BOOL GmPlayerKeyCheckJumpKeyOn(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->key_on & PAD_BUTTON_JUMP) {
		return (TRUE);
	}

	return (FALSE);
}

// ================================================================
// GmPlayerKeyCheckJumpKeyPush
/*!
  プレイヤーキーチェック ジャンプ キーPUSH
  
  @param    player  [in] 対象プレイヤー
  
  @return   TRUE : ジャンプ入力あり
 */
// ================================================================
BOOL GmPlayerKeyCheckJumpKeyPush(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->key_push & PAD_BUTTON_JUMP) {
		return (TRUE);
	}

	return (FALSE);
}

// ================================================================
// GmPlayerKeyGetGimmickRotZ
/*!
  プレイヤーキー取得 ギミック用 コントローラー回転量取得
  
  @param    player  [in] 対象プレイヤー
  
  @return   ROT Z
 */
// ================================================================
Angle32 GmPlayerKeyGetGimmickRotZ(GMS_PLAYER_WORK *ply_work)
{
	Angle32	rot_z;

	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
	// クラシック
		rot_z = ply_work->key_walk_rot_z;
	}
	else {
	// ノーマル
		rot_z = ply_work->key_rot_z;
	}

	return (rot_z);
}

// ================================================================
// GmPlayerKeyCheckTransformKeyPush
/*!
  プレイヤーキーチェック スーパーソニック変身 キーPUSH
  
  @param    player  [in] 対象プレイヤー
  
  @return   TRUE : 変身キー入力あり
 */
// ================================================================
BOOL GmPlayerKeyCheckTransformKeyPush(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->key_push & PAD_BUTTON_TRANSFORM) {
		return (TRUE);
	}

	return (FALSE);
}

// ==========================================================================
// プレイヤーライト
// ==========================================================================
// プレイヤー使用ライトはNNE_LIGHT_6
// ==========================================================================
// GmPlayerSetLight
/*!
 *	プレイヤーライト設定
 *  
 *	@param	light_vec	[in]	ライト方向ベクトル
 *	@param	light_col	[in]	ライトカラー
 */
// ==========================================================================
void GmPlayerSetLight(NNS_VECTOR *light_vec, NNS_RGBA *light_col)
{
	NNS_VECTOR	vec_temp;

	nnNormalizeVector(&vec_temp, light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_6, light_col, 1.f, &vec_temp);
}

// ==========================================================================
// GmPlayerSetDefRimParam
/*!
 *	プレイヤー標準リムライトパラメータ設定
 *  
 *	@param	ply_work		[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlayerSetDefRimParam(GMS_PLAYER_WORK *ply_work)
{
#if _PS3 | _XBOX | _PC
	s32						i;
	OBS_ACTION3D_NN_WORK	*obj_3d;

	MTM_ASSERT(ply_work);

	obj_3d = ply_work->obj_3d_work;
	for (i = 0; i < GMD_PLY_MODEL_TYPE_MAX*GMD_PLY_MODEL_SET_MAX; i++, obj_3d++) {
		obj_3d->toon_rim_param = g_obj.toon_rim_param;
	}
#else
	UNREFERENCED_PARAMETER(ply_work);
#endif
}

// ==========================================================================
// GmPlayerSetRimParam
/*!
 *	プレイヤーリムライトパラメータ設定
 *  
 *	@param	ply_work		[in]	プレイヤーワーク
 *	@param	toon_rim_param	[in]	リムライトパラメータ
 */
// ==========================================================================
void GmPlayerSetRimParam(GMS_PLAYER_WORK *ply_work, NNS_RGB *toon_rim_param)
{
#if _PS3 | _XBOX | _PC
	s32						i;
	OBS_ACTION3D_NN_WORK	*obj_3d;

	MTM_ASSERT(ply_work);
	MTM_ASSERT(toon_rim_param);

	obj_3d = ply_work->obj_3d_work;
	for (i = 0; i < GMD_PLY_MODEL_TYPE_MAX*GMD_PLY_MODEL_SET_MAX; i++, obj_3d++) {
		obj_3d->toon_rim_param = *toon_rim_param;
	}
#else
	UNREFERENCED_PARAMETER(ply_work);
	UNREFERENCED_PARAMETER(toon_rim_param);
#endif
}

// ==========================================================================
// Utility
// ==========================================================================
// ==========================================================================
// GmPlayerCheckGimmickEnable
/*!
 *	ギミックオブジェクトが有効かチェック
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@return	TRUE : 有効
 */
// ==========================================================================
BOOL GmPlayerCheckGimmickEnable(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->gmk_obj && (ply_work->gmk_obj->obj_type == GMD_OBJTYPE_GIMMICK)) {
		GMS_ENEMY_COM_WORK *ene_com = (GMS_ENEMY_COM_WORK*)ply_work->gmk_obj;
		if (ene_com->target_obj == (OBS_OBJECT_WORK*)ply_work) {
			return (TRUE);
		}
	}
	return (FALSE);
}

// ==========================================================================
// GmPlayerIsTransformSuperSonic
/*!
 *	スーパーソニックになれるかチェック
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@return	TRUE : 変身できる
 */
// ==========================================================================
BOOL GmPlayerIsTransformSuperSonic(GMS_PLAYER_WORK *ply_work)
{
	//////if(!false)return TRUE;//sss//test
	// 変身できない状態チェック
	if ((g_gm_main_system.game_flag & (GMD_GAME_FLAG_GOAL_IN)) ||
			(ply_work->player_flag & (GMD_PLF_DIE | GMD_PLF_ACT_GOAL))) {
		return (FALSE);
	}

#if defined (MTD_DEBUG)
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_SUPER_SONIC_FREE) {
		if (!GSM_MAIN_STAGE_IS_SPSTAGE() &&					// スペステモードでなく											// リングが50以上で
				!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {							// スーパーソニックでない
			return (TRUE);
		}
		return (FALSE);
	}
	else {
#endif

	if (!GSM_MAIN_STAGE_IS_SPSTAGE() &&					// スペステモードでなく
			(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD) &&	// カオスエメラルドが7つあり
			ply_work->ring_num >= 50 &&													// リングが50以上で
			!(ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {							// スーパーソニックでない
		return (TRUE);
	}
	return (FALSE);


#if defined (MTD_DEBUG)
	}
#endif
}

// ==========================================================================
// GmPlayerCameraOffsetSet
/*!
 *	演出用カメラ中心点オフセットをセット
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	ofs_x		[in]	オフセットＸ
 *	@param	ofs_y		[in]	オフセットＹ
 */
// ==========================================================================
void GmPlayerCameraOffsetSet(GMS_PLAYER_WORK *ply_work, s16 ofs_x, s16 ofs_y)
{
	ply_work->gmk_camera_center_ofst_x = ofs_x;
	ply_work->gmk_camera_center_ofst_y = ofs_y;
}

#if _IPHONE
// ==========================================================================
// GmPlayerIsStateWait
/*!
 *	現在待機中か否かのチェック
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@return	TRUE : 待機中
 */
// ==========================================================================
BOOL GmPlayerIsStateWait(GMS_PLAYER_WORK *ply_work)
{
	BOOL flag = FALSE;
	
	if (ply_work->act_state >= GME_PLY_ACT_STATE_WAIT_0_1 && ply_work->act_state <= GME_PLY_ACT_STATE_WAIT_2_2) {
		flag = TRUE;
	}
	
	return flag;
}
#endif // _IPHONE

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmPlayerObjRelease
/*!
 *	プレイヤーオブジェクト開放処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@return	TRUE : 待機処理あり
 */
// ==========================================================================
BOOL gmPlayerObjRelease(OBS_OBJECT_WORK *obj_work)
{
//	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)obj_work;
	//s32	i;

	// オブジェクト開放
//	for (i = 0; i < GMD_PLY_MODEL_TYPE_MAX; i++) {
//		ObjAction3dNNModelRelease(&ply_work->obj_3d[i]);
//	}

	// 3D描画ワークをクリア
	obj_work->obj_3d = NULL;

	// 待機あり
	return (TRUE);
}

// ==========================================================================
// gmPlayerObjReleaseWait
/*!
 *	プレイヤーオブジェクト開放待機処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@return	TRUE : 待機中
 */
// ==========================================================================
BOOL gmPlayerObjReleaseWait(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)obj_work;
	s32	i;

//	// オブジェクト開放待機
//	for (i = 0; i < GMD_PLY_MODEL_TYPE_MAX; i++) {
//		if (ObjAction3dNNModelReleaseCheck(&ply_work->obj_3d[i]) == FALSE) {
//			// 待機中
//			return (TRUE);
//		}
//	}

	// モーションデータ開放
	// 使用しているモーションワークは1つのみ(その他はコピー)
	ObjAction3dNNMotionRelease(&ply_work->obj_3d_work[0]);
	for (i = 1; i < GMD_PLY_MODEL_TYPE_MAX*GMD_PLY_MODEL_SET_MAX; i++) {
		ply_work->obj_3d_work[i].motion = NULL;
	}

	// 待機終了
	return (FALSE);
}

// ==========================================================================
// gmPlayerExit
/*!
 *	プレイヤー終了処理
 *
 *	@param	tcb	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlayerExit(MTS_TASK_TCB *tcb)
{
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)mtTaskGetTcbWork(tcb);

	// ワーク保存アドレスクリア
	g_gm_main_system.ply_work[ply_work->player_id] = NULL;

	// オブジェクト標準解放
	ObjObjectExit(tcb);
}

// ==========================================================================
// gmPlayerMain
/*!
 *	プレイヤーメイン処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmPlayerMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)obj_work;

//	// 汎用処理
//	ObjObjectMain(obj_work);
#if _IPHONE
	// SE更新
	if (ply_work->spin_se_timer > 0) {
		ply_work->spin_se_timer--;
	}
	if (ply_work->spin_back_se_timer > 0) {
		ply_work->spin_back_se_timer--;
	}
#endif // _IPHONE
	// 各種チェック

	// シーケンス処理
	GmPlySeqMain(ply_work);
//	if (ply_work->seq_func) {
//		ply_work->seq_func(ply_work);
//	}

#if 0
	/* ソニックライト設定 */
	// スーパーソニックのとき、リムライトアニメーション
	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		NNS_RGBA light_col = {
			1.0f, 1.0f, 1.0f, 1.0f,
		};

		NNS_VECTOR normalLightdir0 = {0.0f, 0.0f, -1.0f};
		NNS_RGB normalRimParam0 = {0.0f, 1.0f, 1.0f}; // 明
		NNS_RGB normalRimParam1 = {1.0f, 1.0f, 1.0f}; // 暗

		NNS_VECTOR rimLight_dir0 = {0.0f, 0.0f, 1.0f};
		NNS_RGB RimParam0 = {6.5f, 1.0f, 1.0f}; // 明
		NNS_RGB RimParam1 = {1.0f, 1.0f, 1.0f}; // 暗 

		NNS_RGB rimParam;
		float nper = 1.0f - ply_work->light_rate;

		// 普通のアニメーション
		if ( ply_work->light_anm_flag == 0 || ply_work->light_anm_flag == 1 )
		{
			ObjDrawSetParallelLight(NNE_LIGHT_6, &light_col, 1.f, &normalLightdir0);
			rimParam.r = normalRimParam0.r * nper + normalRimParam1.r * ply_work->light_rate;
			rimParam.g = normalRimParam0.g * nper + normalRimParam1.g * ply_work->light_rate;
			rimParam.b = normalRimParam0.b * nper + normalRimParam1.b * ply_work->light_rate;
			GmPlayerSetRimParam( ply_work, &rimParam);
		}
		// 逆光表現
		else
		{
			ObjDrawSetParallelLight(NNE_LIGHT_6, &light_col, 1.f, &rimLight_dir0);
			rimParam.r = RimParam0.r * nper + RimParam1.r * ply_work->light_rate;
			rimParam.g = RimParam0.g * nper + RimParam1.g * ply_work->light_rate;
			rimParam.b = RimParam0.b * nper + RimParam1.b * ply_work->light_rate;
			GmPlayerSetRimParam( ply_work, &rimParam);
		}

		// 現在は、普通⇔逆光が出るようにしています
		if ( ply_work->light_anm_flag == 0)
		{
			ply_work->light_rate += 0.01f;
			if ( ply_work->light_rate > 1.0f )
			{
				ply_work->light_rate = 1.0f;
				ply_work->light_anm_flag = 1;
				return;
			}
		}
		else if ( ply_work->light_anm_flag == 1)
		{
			ply_work->light_rate -= 0.01f;
			if ( ply_work->light_rate < 0.0f )
			{
				ply_work->light_rate = 0.0f;
				ply_work->light_anm_flag = 2;
				return;
			}
		}
		// 逆光
		else if ( ply_work->light_anm_flag == 2)
		{
			ply_work->light_rate += 0.01f;
			if ( ply_work->light_rate > 1.0f )
			{
				ply_work->light_rate = 1.0f;
				ply_work->light_anm_flag = 3;
				return;
			}
		}
		// 逆光
		else if ( ply_work->light_anm_flag == 3)
		{
			ply_work->light_rate -= 0.01f;
			if ( ply_work->light_rate < 0.0f )
			{
				ply_work->light_rate = 0.0f;
				ply_work->light_anm_flag = 0;
				return;
			}
		}
	}
#endif // 0

#if defined (MTD_DEBUG)
#if !defined (HOG_ALPHA_ROM)
#if 0	// gmMainに引越し
	// ステータス表示
	{
		s32	y = 3;

		if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_PLY_POS_16) {
			// 16進表示
			amPrintf(2, y, "POS X : %x", obj_work->pos.x);
			y++;
			amPrintf(2, y, "POS Y : %x", obj_work->pos.y);
			y++;
			amPrintf(2, y, "POS Z : %x", obj_work->pos.z);
			y++;
		}
		else if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_PLY_POS_10F) {
			// 10進小数点あり表示
			amPrintf(2, y, "POS X : %0.8f", FXM_FX32_TO_FLOAT(obj_work->pos.x));
			y++;
			amPrintf(2, y, "POS Y : %0.8f", FXM_FX32_TO_FLOAT(obj_work->pos.y));
			y++;
			amPrintf(2, y, "POS Z : %0.8f", FXM_FX32_TO_FLOAT(obj_work->pos.z));
			y++;
		}
		else {
			// 10進表示
			amPrintf(2, y, "POS X : %d", (obj_work->pos.x >> FX32_SHIFT));
			y++;
			amPrintf(2, y, "POS Y : %d", (obj_work->pos.y >> FX32_SHIFT));
			y++;
			amPrintf(2, y, "POS Z : %d", (obj_work->pos.z >> FX32_SHIFT));
			y++;
		}
		amPrintf(2, y, "SPD_M : %x", obj_work->spd_m);
		y++;
		amPrintf(2, y, "SPD_X : %x", obj_work->spd.x);
		y++;
		amPrintf(2, y, "SPD_Y : %x", obj_work->spd.y);
		y++;
		amPrintf(2, y, "DIR_Z : %x", obj_work->dir.z);
		y++;
		amPrintf(2, y, "RING  : %x", ply_work->ring_num);
		y++;

		amPrintf(2, y, "SEQ : %s", g_gm_player_seq_name_tbl[ply_work->seq_state]);
		y++;

		if (obj_work->move_flag & OBD_MOVE_UNDER) {
			amPrintf(2, y, "UNDER : ON");
		}
		else {
			amPrintf(2, y, "UNDER : OFF");
		}
		y++;
	}
#endif
#endif // #if !defined (HOG_ALPHA_ROM)
#endif
}


// ==========================================================================
// 登録処理関数
// ==========================================================================
// ==========================================================================
// gmPlayerDispFunc
/*!
 *	
 *	@param	obj_work	[in]	オブジェクトワーク
 *	
 *	ppOut 標準登録関数 プレイヤー描画用関数
 */
// ==========================================================================
void gmPlayerDispFunc(OBS_OBJECT_WORK *obj_work)
{
	OBS_ACTION3D_NN_WORK	*obj_3d;
	GMS_PLAYER_WORK			*ply_work;
	u16						save_dir = 0;
	float					disp_ofst_y, disp_ofst_x;

	ply_work = (GMS_PLAYER_WORK*)obj_work;

	// 行列計算
	obj_3d = obj_work->obj_3d;
	nnMakeUnitMatrix(&obj_3d->user_obj_mtx_r);

	// 演出用MTX反映
	if (ply_work->gmk_flag & GMD_PLGF_GMK_EXMTX_R) {
		nnMultiplyMatrix(&obj_3d->user_obj_mtx_r, &obj_3d->user_obj_mtx_r, &ply_work->ex_obj_mtx_r);
	}

	// 描画オフセット取得
	disp_ofst_x = 0.f;
	disp_ofst_y = (float)GMD_PL_DISP_OFST_Y;
	if ((ply_work->player_flag & GMD_PLF_PINBALL_SONIC) &&
			!(GME_PLY_ACT_STATE_SPIN <= ply_work->act_state &&
			ply_work->act_state <= GME_PLY_ACT_STATE_SPIN_DASH) || 
		(GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) ) {
		disp_ofst_y = (float)GMD_PL_DISP_OFST_Y_PINBALL;
	}
	else if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE &&
			!(ply_work->gmk_flag2 & GMD_PLGF2_TRUCK_FALL_DEATH)) {	// 落下死亡は除く
		// トロッコ中
		disp_ofst_x = (float)GMD_PL_DISP_OFST_X_TRUCK;
		disp_ofst_y = (float)GMD_PL_DISP_OFST_Y_TRUCK;
	}

	// ソニック地形分下げる その他補正
	nnTranslateMatrix(&obj_3d->user_obj_mtx_r, &obj_3d->user_obj_mtx_r,
			0,		// Z
			//((float)-(GMD_PL_REC_B+2/*調整値*/) / FXM_FX32_TO_FLOAT(g_obj.draw_scale.y)),
			(disp_ofst_y / FXM_FX32_TO_FLOAT(g_obj.draw_scale.y)),
			(disp_ofst_x / FXM_FX32_TO_FLOAT(g_obj.draw_scale.x)));		// X

	if (ply_work->player_flag & GMD_PLF_PGM_TURN_MASK) {
		// ターン回転設定
		save_dir = ply_work->obj_work.dir.y;
		ply_work->obj_work.dir.y += ply_work->pgm_turn_dir;
	}

	// ◆重力方向変換時の角度調整対応？
#if defined (MTD_DEBUG)
	u32 debug_temp_disp_flag = 0;
	if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_PLY_NO_DISP) {
		debug_temp_disp_flag = ply_work->obj_work.disp_flag;
		ply_work->obj_work.disp_flag |= OBD_DISP_NODISP;
	}
#endif

	ObjDrawActionSummary(obj_work);

#if defined (MTD_DEBUG)
	if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_PLY_NO_DISP) {
		ply_work->obj_work.disp_flag &= ~OBD_DISP_NODISP;
		ply_work->obj_work.disp_flag |= debug_temp_disp_flag & OBD_DISP_NODISP;
	}
#endif

	if (ply_work->player_flag & GMD_PLF_PGM_TURN_MASK) {
		// Y回転復帰
		ply_work->obj_work.dir.y = save_dir;
	}
}

// ==========================================================================
// gmPlayerDefaultInFunc
/*!
 *	
 *	@param	obj_work	[in]	オブジェクトワーク
 *	
 *	ppIn 標準登録関数 プレイヤーー毎処理
 */
// ==========================================================================
void gmPlayerDefaultInFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	ply_work = (GMS_PLAYER_WORK*)obj_work;

	// キー取得
	gmPlayerKeyGet(ply_work);

	// 水中チェック
	gmPlayerWaterCheck(ply_work);

	// タイムオーバーチェック
	gmPlayerTimeOverCheck(ply_work);

	// 落下死亡チェック
	gmPlayerFallDownCheck(ply_work);

	// 圧死チェック
	gmPlayerPressureCheck(ply_work);

	// ホーミングターゲットセット
	gmPlayerGetHomingTarget(ply_work);

	// スーパーソニックチェック
	gmPlayerSuperSonicCheck(ply_work);

	// グラインドチェック

	// 押し状態設定
	gmPlayerPushSet(ply_work);

	// 接地チェック
	gmPlayerEarthTouch(ply_work);

	// トロッコ中 前フレーム角度取得
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		ply_work->truck_prev_dir		= ply_work->obj_work.dir.z;
		ply_work->truck_prev_dir_fall	= ply_work->obj_work.dir_fall;
	}

    // プレイヤー定例処理
	if (ply_work->gmk_obj) {
		if (ply_work->gmk_obj->flag & OBD_OBJECT_TASKCLEAR) {
			ply_work->gmk_obj = NULL;
		}
	}

    // ダメージ後無敵処理
	if (ply_work->invincible_timer) {
		ply_work->invincible_timer = ObjTimeCountDown(ply_work->invincible_timer);

		if (ply_work->invincible_timer & (0x04 * FX32_ONE)) {
			ply_work->obj_work.disp_flag |= OBD_DISP_NODISP;
		}
		else {
			ply_work->obj_work.disp_flag &= ~OBD_DISP_NODISP;
		}

		if (!ply_work->invincible_timer) {
			// 無敵終了
			ply_work->obj_work.disp_flag &= ~OBD_DISP_NODISP;
			GmPlayerSetDefNormal(ply_work);
			//if (!(ply_work->player_flag & GMD_PLF_GOAL)) {
			//	ply_work->rect_work[0].def_power = GMD_OBJ_RECT_DEF_POWER_DEFAULT;
			//}
		}
	}

	/* アイテム関連 */
    // アイテム取れないタイマー
	if (ply_work->disapprove_item_catch_timer) {
		ply_work->disapprove_item_catch_timer = ObjTimeCountDown(ply_work->disapprove_item_catch_timer);
	}

#if 1
	// 無敵設定
	{
		OBS_RECT_WORK	*rect_work;

		if (ply_work->genocide_timer) {
			ply_work->genocide_timer = ObjTimeCountDown(ply_work->genocide_timer);
		}

		if (ply_work->genocide_timer || (ply_work->player_flag & GMD_PLF_SUPER_SONIC)) {
			ply_work->water_timer = 0; // 無敵中は息クリア

			// 体矩形を+攻撃矩形化
			rect_work = &ply_work->rect_work[GMD_PLAYER_RECT_BODY];
			//rect_work->def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
			//rect_work->hit_power = GMD_OBJ_RECT_HIT_POWER_ULTIMATE;
			rect_work->hit_flag |= GMD_OBJ_RECT_ATK_FLAG_NORMALATK;
			//ply_work->rect_work[0].usDefFlag |= OBD_HIT_POWER_5;
			//ply_work->rect_work[0].hit_flag |= OBD_HIT_POWER_5|OBD_HIT_NORMAL;
			rect_work->hit_power = GMD_OBJ_RECT_ATK_POWER_INVINCIBLE;

			// 防御矩形を無敵化
			rect_work = &ply_work->rect_work[GMD_PLAYER_RECT_DEF];
			rect_work->def_flag = GMD_OBJ_RECT_DEF_FLAG_NOHIT;
		}
		else if (ply_work->rect_work[GMD_PLAYER_RECT_BODY].hit_flag & GMD_OBJ_RECT_ATK_FLAG_NORMALATK) {
			// 無敵中でなく、無敵設定が残っている場合はクリア
			ply_work->obj_work.disp_flag &= ~OBD_DISP_NODISP;

			//if (!(ply_work->player_flag & GMD_PLF_GOAL)) {
			//	ply_work->rect_work[0].def_power = GMD_OBJ_RECT_DEF_POWER_DEFAULT;
			//}
			// 体矩形を復旧
			rect_work = &ply_work->rect_work[GMD_PLAYER_RECT_BODY];
			//rect_work->hit_power = GMD_OBJ_RECT_ATK_POWER_DEFAULT;
			rect_work->hit_flag &= ~GMD_OBJ_RECT_ATK_FLAG_NORMALATK;
			rect_work->hit_power = GMD_OBJ_RECT_ATK_POWER_DEFAULT;

			// 防御矩形を復旧
			rect_work = &ply_work->rect_work[GMD_PLAYER_RECT_DEF];
			rect_work->def_flag = GMD_OBJ_RECT_DEF_FLAG_WEAK_NORMALATK;

			// 無敵ジングル停止
			GmSoundStopJingleInvincible();
		}
	}

#else
	// アイテム無敵処理
	if ( ply_work->genocide_timer) {
		OBS_RECT_WORK	*rect_work;

		ply_work->water_timer = 0; // 無敵中は息クリア

		ply_work->genocide_timer = ObjTimeCountDown(ply_work->genocide_timer);

		// 体矩形を+攻撃矩形化
		rect_work = &ply_work->rect_work[GMD_PLAYER_RECT_BODY];
		//rect_work->def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
		//rect_work->hit_power = GMD_OBJ_RECT_HIT_POWER_ULTIMATE;
		rect_work->hit_flag |= GMD_OBJ_RECT_ATK_FLAG_NORMALATK;
        //ply_work->rect_work[0].usDefFlag |= OBD_HIT_POWER_5;
        //ply_work->rect_work[0].hit_flag |= OBD_HIT_POWER_5|OBD_HIT_NORMAL;
        // ゴールフラグが立っていなければ
		if(!(ply_work->player_flag & GMD_PLF_GOAL)) {
		//	// 点滅
		//	if ( !(g_gm_game_info.time & 0xe) && ( g_gm_game_info.flag & GMD_MAIN_FLAG_COUNT_TIME) ) {
		//		ObjPaletteTexVary( &ply_work->tex_plt, 31,31,31);
		//	}
          //  // ニトロMAX中は元に戻さず
          //  else if ( !ply_work->nitro_timer ) {
          //      ObjPaletteTexVary( &ply_work->tex_plt, 0,0,0);
		//	}
        }        
        // 無敵終了
		if (!ply_work->genocide_timer) {
			ply_work->obj_work.disp_flag &= ~OBD_DISP_NODISP;

			//if (!(ply_work->player_flag & GMD_PLF_GOAL)) {
			//	ply_work->rect_work[0].def_power = GMD_OBJ_RECT_DEF_POWER_DEFAULT;
			//}
			// 体矩形を復旧
			rect_work = &ply_work->rect_work[GMD_PLAYER_RECT_BODY];

			rect_work->hit_power = GMD_OBJ_RECT_ATK_POWER_DEFAULT;
			rect_work->hit_flag &= ~GMD_OBJ_RECT_ATK_FLAG_NORMALATK;
			//ObjPaletteTexVary( &ply_work->tex_plt, 0,0,0);
        }
    }
#endif

	// アイテム ハイスピード
	if (ply_work->hi_speed_timer) {
		ply_work->hi_speed_timer = ObjTimeCountDown(ply_work->hi_speed_timer);

		if (!ply_work->hi_speed_timer) {
			// パラメータを戻す
			GmPlayerSpdParameterSet(ply_work);
		}
	}

	// ホーミング有効化タイマーカウントダウン
	if (ply_work->homing_timer) {
		ply_work->homing_timer = ObjTimeCountDown(ply_work->homing_timer);
	}

	/* トロッコ用処理 */
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		u16			now_dir;

		// 接地不可フラグ移行
		ply_work->obj_work.sys_flag &= ~OBD_SYSF_NOLANDING_PREVMASK;
		if (ply_work->obj_work.sys_flag & OBD_SYSF_NOLANDING_UNDER) {
			ply_work->obj_work.sys_flag |= OBD_SYSF_NOLANDING_UNDERPREV;
		}
		if (ply_work->obj_work.sys_flag & OBD_SYSF_NOLANDING_LEFT) {
			ply_work->obj_work.sys_flag |= OBD_SYSF_NOLANDING_LEFTPREV;
		}
		if (ply_work->obj_work.sys_flag & OBD_SYSF_NOLANDING_TOP) {
			ply_work->obj_work.sys_flag |= OBD_SYSF_NOLANDING_TOPPREV;
		}
		if (ply_work->obj_work.sys_flag & OBD_SYSF_NOLANDING_RIGHT) {
			ply_work->obj_work.sys_flag |= OBD_SYSF_NOLANDING_RIGHTPREV;
		}
		ply_work->obj_work.sys_flag &= ~OBD_SYSF_NOLANDING_MASK;

		// R地形範囲フラグ移行
		ply_work->gmk_flag2 &= ~GMD_PLGF2_TRUCK_R_AREA_PREV;
		if (ply_work->gmk_flag2 & GMD_PLGF2_TRUCK_R_AREA) {
			ply_work->gmk_flag2 |= GMD_PLGF2_TRUCK_R_AREA_PREV;
		}
		ply_work->gmk_flag2 &= ~GMD_PLGF2_TRUCK_R_AREA;

		/* ジャンプ中重力方向設定チェック */
		if (ply_work->jump_pseudofall_eve_id_set == 0) {
			// 今回セットしたものがない場合はwaitをカレントへ
			ply_work->jump_pseudofall_eve_id_cur = ply_work->jump_pseudofall_eve_id_wait;
		}
		ply_work->jump_pseudofall_eve_id_set = 0;
		ply_work->jump_pseudofall_eve_id_wait = 0;


		/* 擬似重力設定 */
#if GMD_PLY_USE_CAM_ROT_TRUCK_JUMP
		if ((!(ply_work->gmk_flag2 & GMD_PLGF2_TRUCK_JUMP_NO_CAM_ROT) ||
					(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) &&
				ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_DEMO_FW) {
#else
		if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER &&
				ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_DEMO_FW) {
#endif
#ifdef GMD_MAIN_USE_BODY_ROTATE
			ply_work->ply_pseudofall_dir = -ply_work->key_rot_z;
#else
			Angle32	fall_rot = 0, rot_dist, rot_spd;


#if GMD_PLY_USE_ROT_Z_TRUCK_MOVE
#if _PC || _XBOX// || _IPHONE
			fall_rot = -ply_work->key_rot_z >> 2;
#elif _IPHONE // _IPHONE TRUCK
			fall_rot = -ply_work->key_rot_z;
#else
			// PS3 WII
			if (ply_work->key_rot_z > 0x2000) {
				fall_rot = -0x4000/2;
			}
			else if (ply_work->key_rot_z < -0x2000) {
				fall_rot = 0x4000/2;
			}
			else {
				fall_rot = -ply_work->key_rot_z;
			}
#endif
#else
#if _PC || _XBOX || _IPHONE
			fall_rot = -ply_work->key_rot_z >> 2;	// デバックチェック及びiPhone用
#endif
#endif

#if _PC || _XBOX
#if GMD_PLY_USE_ROT_Z_TRUCK_MOVE
			if (fall_rot > 0) {
				if (fall_rot < -ply_work->key_walk_rot_z >> 2) {
					fall_rot = -ply_work->key_walk_rot_z >> 2;
				}
			}
			else if (fall_rot < 0) {
				if (fall_rot > -ply_work->key_walk_rot_z >> 2) {
					fall_rot = -ply_work->key_walk_rot_z >> 2;
				}
			}
			else {
				fall_rot = -ply_work->key_walk_rot_z >> 2;
			}
#else
			if (!fall_rot) {
				fall_rot = -ply_work->key_walk_rot_z >> 2;
			}
#endif
#endif

			// TRUCK:20100414
// _IPHONE TRUCK
#if 1
			// 回転量補正
#if _IPHONE
#define GMD_PLAY_TRUCK_ROTATE_MAX  (0x1400)
#else
#define GMD_PLAY_TRUCK_ROTATE_MAX  (0x200)
#endif // _IPHONE
#if _IPHONE
#define GMD_PLAYER_TRUCK_ROTATE_BUF_MAX (0xC000)
			if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK) {
				static Angle32 fall_rot_buf = 0;
				
				// TRUCK:20100426
				fall_rot = fall_rot * 38 / 10;
				
				// 蓄えを放出
				if ((fall_rot_buf > 0 && fall_rot >= 0) || (fall_rot_buf <  0 && fall_rot <= 0)) {
					fall_rot += fall_rot_buf;
				}
				fall_rot_buf = 0;
				
				// 現在地が既定値ごえなら蓄えに戻す
				if (fall_rot > GMD_PLAY_TRUCK_ROTATE_MAX) {
					fall_rot_buf += fall_rot - GMD_PLAY_TRUCK_ROTATE_MAX;
					if (fall_rot_buf > GMD_PLAYER_TRUCK_ROTATE_BUF_MAX) {
						fall_rot_buf = GMD_PLAYER_TRUCK_ROTATE_BUF_MAX;
					}
					fall_rot = GMD_PLAY_TRUCK_ROTATE_MAX;
				}
				else if (fall_rot < -GMD_PLAY_TRUCK_ROTATE_MAX)  {
					fall_rot_buf += fall_rot + GMD_PLAY_TRUCK_ROTATE_MAX;
					if (fall_rot_buf < -GMD_PLAYER_TRUCK_ROTATE_BUF_MAX) {
						fall_rot_buf = -GMD_PLAYER_TRUCK_ROTATE_BUF_MAX;
					}
					fall_rot = -GMD_PLAY_TRUCK_ROTATE_MAX;
				}
			}
			else 
#endif // _IPHONE
			{
#if _IPHONE
				fall_rot /= 24;
#else
				s32 div_temp;
				s32	ply_spd_m_abs = MTM_MATH_ABS(ply_work->obj_work.spd_m);

				div_temp = (s32)(0x20000 / (1.f + (float)ply_spd_m_abs / (float)0x8000));

				if (fall_rot > 0) {
					fall_rot = (fall_rot * (fall_rot >> 1)) / div_temp;
				}
				else {
					fall_rot = -(fall_rot * (fall_rot >> 1)) / div_temp;
				}
#endif // _IPHONE
			}
#else
			if (fall_rot > 0) {
				fall_rot = (fall_rot * (fall_rot >> 1)) / 0x20000;
			}
			else {
				fall_rot = -(fall_rot * (fall_rot >> 1)) / 0x20000;
			}
#endif
			// 現在の角度取得
			now_dir = (u16)(ply_work->obj_work.dir.z + (ply_work->obj_work.dir_fall - g_gm_main_system.pseudofall_dir));
			
#define GMD_PLAYER_NOW_DIR_MAX (0xeaab)
#define GMD_PLAYER_NOW_DIR_MIN (0x1554)
// TRUCK:20100416
			if (now_dir > GMD_PLAYER_NOW_DIR_MIN && now_dir < GMD_PLAYER_NOW_DIR_MAX) {
				if (now_dir > 0x8000 && fall_rot > 0) {
					fall_rot = 0;
				//	ply_work->ply_pseudofall_dir -= (GMD_PLAYER_NOW_DIR_MAX - (s32)now_dir);
				//	printf("MAX:%x \n\n", ply_work->ply_pseudofall_dir);
				}
				else if (now_dir <= 0x8000 && fall_rot <= 0) {
					fall_rot = 0;
				//	ply_work->ply_pseudofall_dir += ((s32)now_dir - GMD_PLAYER_NOW_DIR_MIN);
				//	printf("MIN:%x \n\n", ply_work->ply_pseudofall_dir);
				}
			}
			
			// 踏ん張り中は 落下する方向へはまわさない
			if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_STICK) {
				if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_DANGER) {
#if !_IPHONE // _IPHONE TRUCK
#if GMD_PLY_USE_CAM_ROT_TRUCK_JUMP
					if ((0x8000 > ply_work->truck_stick_prev_dir && ply_work->truck_stick_prev_dir > 0x4000 && fall_rot < 0) ||
							(0x8000 < ply_work->truck_stick_prev_dir && ply_work->truck_stick_prev_dir < 0xC000 && fall_rot > 0)) {
						fall_rot = 0;	// 角度クリア
					}
#else
					// 元
					if ((0x8000 > now_dir && now_dir > 0x4000 && fall_rot < 0) ||
							(0x8000 < now_dir && now_dir < 0xC000 && fall_rot > 0)) {
						fall_rot = 0;	// 角度クリア
					}
#endif
#endif // !_IPHONE
				}
				else {
					// 危険演出開始時は動かない
					fall_rot = 0;	// 角度クリア
				}
			}

			// ゴール演出中 は水平に戻す
#if _IPHONE
			if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SPECIAL_STAGE ||
				ply_work->player_flag & GMD_PLF_ACT_GOAL) {
				// スペステリングin or ゴールシーケンス中
#else
			if (g_gm_main_system.game_flag & GMD_GAME_FLAG_GOAL_IN ||
					ply_work->player_flag & GMD_PLF_GOAL) {
#endif // _IPHONE
				if ((s16)now_dir < 0) {
					fall_rot = (s16)now_dir;
				}
				else if ((s16)now_dir > 0) {
					fall_rot = (s16)now_dir;
				}
				else {
					fall_rot = 0;
				}
			}

			// 回転距離取得
			rot_dist = fall_rot;
			if (rot_dist > 0x8000) {
				// 近い方に回す
				rot_dist -= 0x10000;
			}
			else if (rot_dist < -0x8000) {
				// 近い方に回す
				rot_dist += 0x10000;
			}

			rot_spd = rot_dist;
				
			if (MTM_MATH_ABS(rot_spd) > GMD_PLAY_TRUCK_ROTATE_MAX/*最大回転量*/) {
				if (rot_spd >= 0) {
					rot_spd = GMD_PLAY_TRUCK_ROTATE_MAX;
				}
				else {
					rot_spd = -GMD_PLAY_TRUCK_ROTATE_MAX;
				}
			}

			ply_work->ply_pseudofall_dir += rot_spd;
#endif // GMD_MAIN_USE_BODY_ROTATE
			g_gm_main_system.pseudofall_dir = (u16)ply_work->ply_pseudofall_dir;
		}


		/* 重力方向設定 */
		// 前重力保存
		ply_work->prev_dir_fall = obj_work->dir_fall;

		// 重力設定(90度ずつ変更)
		if (obj_work->move_flag & OBD_MOVE_UNDER) {
			// 地上
			obj_work->dir_fall = (u16)((g_gm_main_system.pseudofall_dir + 0x2000) & 0xC000);
		}
		else {
			// 空中
			obj_work->dir_fall = (u16)((ply_work->jump_pseudofall_dir + 0x2000) & 0xC000);
		}

		if (ply_work->prev_dir_fall != obj_work->dir_fall) {
			// 角度が切り替わった時に補正(重力方向が0度になるため)
			ply_work->obj_work.dir.z -= obj_work->dir_fall - ply_work->prev_dir_fall;
		}

// TRUCK:20100416
#if 01
		if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
			now_dir = (u16)(ply_work->obj_work.dir.z + (ply_work->obj_work.dir_fall - g_gm_main_system.pseudofall_dir));
			if (now_dir > GMD_PLAYER_NOW_DIR_MIN && now_dir < GMD_PLAYER_NOW_DIR_MAX) {
				if (now_dir > 0x8000) {
					ply_work->ply_pseudofall_dir -= (GMD_PLAYER_NOW_DIR_MAX - (s32)now_dir);
				}
				else if (now_dir <= 0x8000) {
					ply_work->ply_pseudofall_dir += ((s32)now_dir - GMD_PLAYER_NOW_DIR_MIN);
				}
#if 0
				g_gm_main_system.pseudofall_dir = (u16)ply_work->ply_pseudofall_dir;
				// 重力設定(90度ずつ変更)
				if (obj_work->move_flag & OBD_MOVE_UNDER) {
					// 地上
					obj_work->dir_fall = (u16)((g_gm_main_system.pseudofall_dir + 0x2000) & 0xC000);
				}
				else {
					// 空中
					obj_work->dir_fall = (u16)((ply_work->jump_pseudofall_dir + 0x2000) & 0xC000);
				}
				
				if (ply_work->prev_dir_fall != obj_work->dir_fall) {
					// 角度が切り替わった時に補正(重力方向が0度になるため)
					ply_work->obj_work.dir.z -= obj_work->dir_fall - ply_work->prev_dir_fall;
				}
#endif // 0
			}
		}
#endif // 01
			
			
			
		// 踏ん張りチェック
		// 一定角度で移動停止
		now_dir = (u16)(ply_work->obj_work.dir.z + (ply_work->obj_work.dir_fall - g_gm_main_system.pseudofall_dir));

#if GMD_PLY_USE_CAM_ROT_TRUCK_JUMP
		if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
			if (!(ply_work->gmk_flag2 & (GMD_PLGF2_TRUCK_R_AREA | GMD_PLGF2_TRUCK_R_AREA_PREV)) &&
					(GMD_PL_TRUCK_DANGER_DIR < now_dir && now_dir < GMD_PL_TRUCK_DANGER_DIR_REV)) {
				if (ply_work->truck_prev_dir == obj_work->dir.z) {
					
					ply_work->obj_work.spd_m = 0;

					if (!(ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_STICK)) {
						// 踏ん張り開始
						ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_STICK;

						// 危険演出へ移行
						GmPlySeqGmkInitTruckDanger(ply_work, ply_work->truck_obj);

						// 落下待ち時間再設定
						GmPlayerSpdParameterSet(ply_work);
					}
				}
			}
			else if (!(ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_STICK)) {
				// 踏ん張り状態にならない角度の時保存
				ply_work->truck_stick_prev_dir = now_dir;
			}
		}
#else
		// 元
		if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER &&
				!(ply_work->gmk_flag2 & (GMD_PLGF2_TRUCK_R_AREA | GMD_PLGF2_TRUCK_R_AREA_PREV)) &&
				ply_work->truck_prev_dir == obj_work->dir.z) {
			if (GMD_PL_TRUCK_DANGER_DIR < now_dir && now_dir < GMD_PL_TRUCK_DANGER_DIR_REV) {
				
				ply_work->obj_work.spd_m = 0;

				if (!(ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_STICK)) {
					// 踏ん張り開始
					ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_STICK;

					// 危険演出へ移行
					GmPlySeqGmkInitTruckDanger(ply_work, ply_work->truck_obj);

					// 落下待ち時間再設定
					GmPlayerSpdParameterSet(ply_work);
				}
			}
		}
#endif
		// 踏ん張り復帰チェックは gmPlySeqGmkMainTruckDanger が行う
		if ((ply_work->gmk_flag & (GMD_PLGF_GMK_TRUCK_STICK | GMD_PLGF_GMK_TRUCK_DANGER)) ==
				(GMD_PLGF_GMK_TRUCK_STICK | GMD_PLGF_GMK_TRUCK_DANGER)) {
			// 踏ん張り終了チェック
			if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER) ||
					!(GMD_PL_TRUCK_DANGER_DIR < now_dir && now_dir < GMD_PL_TRUCK_DANGER_DIR_REV)) {
				// 踏ん張り終了
			//	ply_work->gmk_flag &= ~(GMD_PLGF_GMK_TRUCK_STICK | GMD_PLGF_GMK_TRUCK_DANGER);
			//	ply_work->obj_work.vib_timer = 0;
				// 危険演出(gmPlySeqGmkMainTruckDanger)に終了告知
				ply_work->player_flag |= GMD_PLF_USER1;
				// 落下待ち時間再設定
				GmPlayerSpdParameterSet(ply_work);

				// 危険終了演出へ移行
			}
		}
		if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_DANGER_RET) {
			// 踏ん張り終了
			ply_work->gmk_flag &= ~(GMD_PLGF_GMK_TRUCK_STICK | GMD_PLGF_GMK_TRUCK_DANGER | GMD_PLGF_GMK_TRUCK_DANGER_RET);
			ply_work->obj_work.vib_timer = 0;
			// くらい復帰
			GmPlayerSetDefNormal(ply_work);
			// 落下待ち時間再設定
			GmPlayerSpdParameterSet(ply_work);
		}
	}
	/* トロッコ用処理 ここまで */
}

// ==========================================================================
// gmPlayerSplStgInFunc
/*!
 *	
 *	@param	obj_work	[in]	オブジェクトワーク
 *	
 *	ppIn SpecialStage登録関数 プレイヤーー毎処理
 */
// ==========================================================================
void gmPlayerSplStgInFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work;

	ply_work = (GMS_PLAYER_WORK*)obj_work;

	// キー取得
	gmPlayerKeyGet(ply_work);

	// タイムオーバーチェック
	gmPlayerTimeOverCheck(ply_work);

	// 押し状態設定
//	gmPlayerPushSet(ply_work);

	// 接地チェック
	gmPlayerEarthTouch(ply_work);

	// スペシャルステージ用処理
	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {

		/* 擬似重力設定 */
#ifdef GMD_MAIN_USE_BODY_ROTATE
		g_gm_main_system.pseudofall_dir = (u16)-ply_work->key_rot_z; // 本体の傾きをそのまま使用
#else
		OBS_CAMERA	*camera;
		camera = ObjCameraGet(g_obj.glb_camera_id);
		g_gm_main_system.pseudofall_dir = (u16)-camera->roll;
#endif // GMD_MAIN_USE_BODY_ROTATE
		
		// 前重力保存
		ply_work->prev_dir_fall2 = ply_work->prev_dir_fall;
		ply_work->prev_dir_fall = obj_work->dir_fall;

		// 重力設定(90度ずつ変更)
		obj_work->dir_fall = (u16)((g_gm_main_system.pseudofall_dir + 0x2000) & 0xC000);

		ply_work->jump_pseudofall_dir = g_gm_main_system.pseudofall_dir;	// ジャンプ中角度セット
//		if (ply_work->prev_dir_fall != obj_work->dir_fall) {
//			// 角度が切り替わった時に補正(重力方向が0度になるため)
//			ply_work->obj_work.dir.z -= obj_work->dir_fall - ply_work->prev_dir_fall;
//		}
/*
		// 基礎回転量変更チェック
		if (g_gm_main_system.pseudofall_cam_ofst != (u16)(0x10000 - obj_work->dir_fall)) {
			// プレイヤーが重力方向にそった地面にいる時に一定時間以上たったら基礎値をまわす
			if ((obj_work->move_flag & OBD_MOVE_UNDER) &&
						((u16)(obj_work->dir.z + 0x2000) >> 14) == 0) {
				ply_work->dir_fall_fix_timer = ObjTimeCountUp(ply_work->dir_fall_fix_timer);

				// 基礎回転量変更
				if (ply_work->dir_fall_fix_timer >= FX32_ONE * 8) {
					g_gm_main_system.pseudofall_cam_ofst = (u16)(0x10000 - obj_work->dir_fall);
				}
			}
			else {
				// 固定時間クリア
				ply_work->dir_fall_fix_timer = 0;
			}
		}
		else {
			// 固定時間クリア
			ply_work->dir_fall_fix_timer = 0;
		}

		if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_STICK) {
			if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER) ||
					!(0x4000 < ply_work->obj_work.dir.z &&
						ply_work->obj_work.dir.z < 0xC000)) {
				// 踏ん張り終了
				ply_work->gmk_flag &= ~GMD_PLGF_GMK_TRUCK_STICK;
				ply_work->obj_work.vib_timer = 0;
			}
		}
*/
	}

    // プレイヤー定例処理
	if (ply_work->gmk_obj) {
		if (ply_work->gmk_obj->flag & OBD_OBJECT_TASKCLEAR) {
			ply_work->gmk_obj = NULL;
		}
	}
}


// ==========================================================================
// gmPlayerRectTruckFunc
/*!
 *	トロッコ用 矩形登録前処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *	
 *	ppRec 標準登録関数 プレイヤーー毎処理
 */
// ==========================================================================
void gmPlayerRectTruckFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK *ply_work = (GMS_PLAYER_WORK*)obj_work;

	// 移動中トロッコひき潰し対応
	if (((obj_work->move_flag & OBD_MOVE_UNDER && MTM_MATH_ABS(obj_work->spd_m) > 0x0100) ||
			!(obj_work->move_flag & OBD_MOVE_UNDER)) &&
			!(ply_work->player_flag & GMD_PLF_DIE) &&
			!(ply_work->seq_state == GME_PLY_SEQ_STATE_DAMAGE)) {
		// 攻撃設定
		GmPlayerSetAtk(ply_work);
	}
	else {
		// 攻撃無効化
		ply_work->rect_work[GMD_PLAYER_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	}
}

// ==========================================================================
// gmPlayerDefaultLastFunc
/*!
 *	
 *	@param	obj_work	[in]	オブジェクトワーク
 *	
 *	ppLast 標準登録関数 プレイヤーー毎処理
 */
// ==========================================================================
void gmPlayerDefaultLastFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)obj_work;
	// ◆後で
	
    // カメラずらし設定
    gmPlayerCameraOffset(ply_work);

	// 壁HIT時に速度クリアを行わないフラグを解除
	if (!ply_work->gmk_obj || (ply_work->gmk_flag & GMD_PLGF_BOOST_CHK_ON_GMK)) {
		//if(!(ply_work->player_flag & GMD_PLF_NITRO_START)) {
		obj_work->move_flag &= ~OBD_MOVE_NOSPD;
		//}
	}
	// オートラン設定
	if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {
		if ((obj_work->pos.x >> FX32_SHIFT) >
				g_gm_main_system.map_fcol.left + 128) {	// 左端では速度クリアあり
			// 速度クリアなし
			obj_work->move_flag |= OBD_MOVE_NOSPD;
		}
	}

#if 0
	// トロッコ時接地チェック
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		// 一定角度で移動停止
		if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
			if (0x4000 < ply_work->obj_work.dir.z &&
					ply_work->obj_work.dir.z < 0xC000) {
				ply_work->obj_work.spd_m = 0;
			}
		}
	}
#endif
}


// ==========================================================================
// gmPlayerTruckCollisionFunc
/*!
 *	トロッコ用地形チェック処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *	
 *	ppLast 拡張登録関数 プレイヤートロッコ用地形チェック処理
 */
// ==========================================================================
void gmPlayerTruckCollisionFunc(OBS_OBJECT_WORK *obj_work)
{
	BOOL	b_check = FALSE;
	u32		prev_move_flag = 0;
	fx32	prev_spd_m = 0;
	VecFx32	prev_spd = {0};
	VecFx32	prev_pos = {0};
	VecFx32	prev_move = {0};
	u16		prev_dir = 0, prev_dir_fall = 0;

	if ((obj_work->move_flag & OBD_MOVE_UNDER) &&
			!(obj_work->move_flag & OBD_MOVE_JUMP) &&
			MTM_MATH_ABS(obj_work->spd_m) >= GMD_PLD_TRUCK_SLOPE_FLY_SPD) {
		// 飛び出しチェックあり
		b_check = TRUE;

		// 現在の情報を保存
		prev_move_flag	= obj_work->move_flag;
		prev_spd_m		= obj_work->spd_m;
		prev_spd		= obj_work->spd;
		prev_pos		= obj_work->pos;
		prev_move		= obj_work->move;
		prev_dir		= obj_work->dir.z;
		prev_dir_fall	= obj_work->dir_fall;
	}

	// 通常地形処理
	if (g_obj.ppCollision) {
		g_obj.ppCollision(obj_work);
	}

	// 飛び出しチェックあり状態で地面背一致がある場合 飛び出しチェック
	if (b_check && (obj_work->move_flag & OBD_MOVE_UNDER)) {
		// 前の角度と一定以上変更があった場合に飛び出しチェック
		u16	dir;

		dir = (u16)(obj_work->dir.z - prev_dir);
		if (prev_spd_m > 0) {
			if (!(GMD_PLD_TRUCK_SLOPE_FLY_DIR <= dir &&
					dir <= 0x4000)) {
				// 範囲外
				return;
			}
		}
		else {
			if (!((0x10000 - GMD_PLD_TRUCK_SLOPE_FLY_DIR) >= dir &&
					dir >= 0x8000)) {
				// 範囲外
				return;
			}
		}

		// 飛び出し

#if 0
		// 前の速度を前の速度分解
		dir = (u16)(prev_dir + prev_dir_fall - g_gm_main_system.pseudofall_dir);
		if (prev_spd_m > 0) {	// 移動方向で若干補正
			dir = (u16)(dir - 0x0200);
		}
		else {
			dir = (u16)(dir + 0x0200);
		}
		obj_work->spd.x = FX_Mul(prev_spd_m, mtMathCos(dir));
		obj_work->spd.y = FX_Mul(prev_spd_m, mtMathSin(dir));
#endif

		// 各種ステータス復帰
		obj_work->move_flag = prev_move_flag;
		obj_work->spd_m		= FX_Mul(prev_spd_m, GMD_PLD_TRUCK_SLOPE_FLY_SPD_PER);
		obj_work->spd		= prev_spd;
		obj_work->pos		= prev_pos;
		obj_work->move		= prev_move;
		obj_work->dir.z		= prev_dir;
		obj_work->dir_fall	= prev_dir_fall;

		// UNDER OFF
		obj_work->move_flag = prev_move_flag & ~OBD_MOVE_UNDER;

		// 坂道飛び出し時空中減速
		((GMS_PLAYER_WORK*)obj_work)->gmk_flag2 |= GMD_PLGF2_TRUCK_SLOPEFLY_DEC;

		// 通常地形処理
	//	obj_work->move_flag |= OBD_MOVE_JUMP;	// ジャンプ状態でチェック
	//	if (g_obj.ppCollision) {
	//		g_obj.ppCollision(obj_work);
	//	}
	//	obj_work->move_flag &= ~OBD_MOVE_JUMP;
	}

	if ((obj_work->move_flag & (OBD_MOVE_UNDER | OBD_MOVE_UNDERPREV)) ==
											(OBD_MOVE_UNDER | OBD_MOVE_UNDERPREV)) {	// ジャンプ後着地などは即時角度設定を行う
		// 重力方向が変わっている場合は以前の角度を修正する
		if (obj_work->dir_fall != ((GMS_PLAYER_WORK*)obj_work)->truck_prev_dir_fall) {
			((GMS_PLAYER_WORK*)obj_work)->truck_prev_dir = (u16)(((GMS_PLAYER_WORK*)obj_work)->truck_prev_dir +
								((s32)((GMS_PLAYER_WORK*)obj_work)->truck_prev_dir_fall) - (s32)obj_work->dir_fall);
			((GMS_PLAYER_WORK*)obj_work)->truck_prev_dir_fall = obj_work->dir_fall;
		}
		
		// 一定角度以上一気に角度が変わっている場合は補間する
		if (MTM_MATH_ABS((s32)((GMS_PLAYER_WORK*)obj_work)->truck_prev_dir - (s32)obj_work->dir.z) > 0x0400) {
			obj_work->dir.z = ObjRoopMove16(((GMS_PLAYER_WORK*)obj_work)->truck_prev_dir, obj_work->dir.z, 0x0400);
		}
	}
}

// ==========================================================================
// 矩形処理
// ==========================================================================
// ==========================================================================
// gmPlayerAtkFunc
/*!
 *	プレイヤー攻撃HIT処理
 *
 *	@param	mine_rect		[in]	自分矩形ワークポインタ
 *	@param	match_rect		[in]	相手矩形ワークポインタ
 *
 *	@note
 *		ppHitへ登録
 */
// ==========================================================================
void gmPlayerAtkFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect)
{
	UNREFERENCED_PARAMETER(mine_rect);
	UNREFERENCED_PARAMETER(match_rect);
}

// ==========================================================================
// gmPlayerDefFunc
/*!
 *	プレイヤーくらいHIT処理
 *
 *	@param	mine_rect		[in]	自分矩形ワークポインタ
 *	@param	match_rect		[in]	相手矩形ワークポインタ
 *
 *	@note
 *		ppDefへ登録
 */
// ==========================================================================
void gmPlayerDefFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect)
{
	GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK *)mine_rect->parent_obj;
	fx32			spd;
	u32				gmk_flag;

	// 実績用
	// プレイヤーダメージ回数カウント
	HgTrophyIncPlayerDamageCount(ply_work);

	/* 速度取得 */
	if ( ply_work->obj_work.move_flag & OBD_MOVE_NOSPDM ) {
		spd = ply_work->obj_work.spd.x;
	}
	else {
		spd = ply_work->obj_work.spd_m;
	}

	if (match_rect->parent_obj->obj_type == GMD_OBJTYPE_GIMMICK) {
		GMS_ENEMY_COM_WORK	*ene_com = (GMS_ENEMY_COM_WORK*)match_rect->parent_obj;

		if ((GMD_EVENT_ID_NEEDLE_U <= ene_com->eve_rec->id && ene_com->eve_rec->id <= GMD_EVENT_ID_NEEDLE_R) ||
				ene_com->eve_rec->id == GMD_EVENT_ID_ACT_NEEDLE_U ||
					ene_com->eve_rec->id == GMD_EVENT_ID_ACT_NEEDLE_D) {
			// 針ダメージ
			GmSoundPlaySE("Damage2");
		}
	}

	// SE
	//GmSoundPlaySE("Damage1");
	//GmSoundPlaySE("Damage2");		針◆
	//GmSoundPlaySE("Damage3");		水
//	// 針ダメージの時、針の音を鳴らす
//	if ( pDamage->hit_flag & GMD_OBJ_RECT_HIT_FLAG_NEEDLE ) {
//		GmSoundPlayArcSe( &ply_work->h_snd_se, Game_SE, SE_Toge );
//	}

//	/* テンション設定 */
//	if ( !(ply_work->player_flag & GMD_PLF_BARRIER ) ) {
//	// テンション減少
//		GmPlayerTensionGet( ply_work, -GMD_PLAYER_TRICK_DAMAGE );
//		GmFixSetTensionAct((s16)(ply_work->tension >> GMD_PLAYER_TENSION_SHIFT), GMD_FIX_TENSION_DEC_ANIM_VIBRATION);
//	}

	/* ダメージ設定 */
#if defined(MTD_DEBUG)  // デバッグ版
//	if (!( g_gs_main_sys_info.debug_flag & GSD_DEBUG_NODAMAGE )){
#endif
	//	if (g_gs_main_sys_info.stage_id != GMD_MAIN_STAGE_ID_1_TUTORIAL) {	// チュートリアル時は死なない
			// 一撃死攻撃フラグチェック◆後で復旧
		//	if ( match_rect->hit_flag & GMD_OBJ_RECT_HIT_FLAG_DEATH ) {
		//		// リングばら撒き
		//		if ( ply_work->ring_num ){
		//			// SE
		//			GmSoundPlayArcSe( &ply_work->h_snd_se, Game_SE, SE_Ringlost );
		//			GmRingDamageSet(ply_work);
		//		}
		//		// 死亡
		//		GmPlayerDieInit(ply_work);
		//		return;
		//	}

			if ( !(ply_work->player_flag & GMD_PLF_BARRIER ) ){
				// リングが無ければ死亡
				if ( !ply_work->ring_num ){
					// 死亡シーケンスへ
					GmPlySeqChangeDeath(ply_work);
					return;
				}
			}
	//	}
#if defined(MTD_DEBUG)  // デバッグ版
//	}
#endif

	/* プレイヤーステータス初期化 */
	gmk_flag = ply_work->gmk_flag;			// 退避
	GmPlayerStateInit(ply_work);
	// ダメージを受けてもテーブルcolはそのまま
	//ply_work->gmk_flag |= gmk_flag & GMD_PLGF_GMK_TABLE_COL;

	if ( !(ply_work->player_flag & GMD_PLF_BARRIER ) ){
		// SE
		if (ply_work->ring_num) {
			GmSoundPlaySE("Ring2");
		}

		// リングばら撒き
	//	if (!(g_gs_main_sys_info.game_flag & GSD_GAME_FLAG_WM)) {
			// 通常時
			GmRingDamageSet(ply_work);
	//	}
	//	else {
	//		// 通信時
	//		GmRingDamageSetNum(ply_work, GMD_PL_WM_DAMAGE_RING_NUM);
	//		ply_work->player_flag |= GMD_PLF_N_DMG;
	//	}

		// エフェクト 敵がヒット
		GmComEfctCreateHitEnemy(&ply_work->obj_work,
						(mine_rect->rect.left+mine_rect->rect.right)*FX32_ONE/2,
						(mine_rect->rect.top+mine_rect->rect.bottom)*FX32_ONE/2);
	}

	// バリア解除
	if (ply_work->player_flag & (GMD_PLF_BARRIER | GMD_PLF_MAGNET)) {
		// バリア解除サウンド
		GmSoundPlaySE("Damage1");
//		GmSoundPlaySE("Ex_Barrier");
	}
	ply_work->player_flag &= ~(GMD_PLF_BARRIER | GMD_PLF_MAGNET);

	// 画面振動
	//EfQuake( EFD_QUAKE_S_MIDDLE );

	// 無敵設定
	ply_work->invincible_timer = ply_work->time_damage;
	//◆後でply_work->rect_work[0].def_power = GMD_OBJ_RECT_DEF_POWER_INVINCIBLE;
	//ply_work->rect_work[0].usDefFlag |= OBD_HIT_POWER_5;

	/* 演出動作設定 */
	// ダメージシーケンスへ
	GmPlySeqChangeDamage(ply_work);
#if 0
	// 通常
	/* アクション設定 */
	if ( MTM_MATH_ABS(fSpd) >= ply_work->spd3 ) {
		// 前転がり
		GmPlayerActionChange( ply_work, GMD_PLY_STATE_DAMAGE_F );
	}
	else {
		GmPlayerActionChange( ply_work, GMD_PLY_STATE_DAMAGE );
	}

	/* 速度設定 */
	if ( MTM_MATH_ABS(fSpd) < GMD_PL_DAMAGE_CHECK_SPD ){
		fSpd = 0;
	}
	// 跳ねる
	ply_work->obj_work.spd.x = GMD_PL_DAMAGE_JUMP_X;
	ply_work->obj_work.spd.y = GMD_PL_DAMAGE_JUMP_Y;
	ply_work->obj_work.spd_m = 0;

	// 方向設定
	if (ply_work->act_state == GMD_PLY_STATE_DAMAGE_F) {
		// 前転がり時方向そのまま
		ply_work->obj_work.spd.x = fSpd >> 1;
	}
	else {
		if (fSpd > 0) {
			ply_work->obj_work.spd.x = -ply_work->obj_work.spd.x;		// 移動反転
		}
		else if (fSpd == 0 && !(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP)) {
			ply_work->obj_work.spd.x = -ply_work->obj_work.spd.x;		// 移動反転
		}
	}

	// ジャンプ設定
	// ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

	// メイン処理設定
	ply_work->obj_work.ppFunc = gmPlayerDamageMain;
#endif

//	// SE
//	switch ( ply_work->char_no ) {
//	default:
//		// Voice
//		GmSoundPlayArcSe( &ply_work->h_snd_se, Game_SE, SE_Owa );
//		break;
//	case GSD_PLAYER_BLAZE:
//		// Voice
//		GmSoundPlayArcSe( &ply_work->h_snd_se, Game_SE, SE_Ups );
//		break;
//	}
}

#if 0	// ◆必要になったら対応
// ==========================================================================
// gmPlayerActionCallBack
/*!
 *	
 *	@param	pCmd		[in]	コマンドワーク
 *	@param	pAct		[in]	アクションワーク
 *	@param	ulObjWork	[in]	パラメータ
 *	
 *	ppOut 標準登録関数 プレイヤーアクションコールバック関数
 */
// ==========================================================================
void gmPlayerActionCallBack( const MTS_ACT_COMMAND* pCmd, MTS_ACTION* pAct, u32 ulObjWork)
{
}
#endif

// =====================================================================
// ステータス関連
// =====================================================================
// =====================================================================
// gmPlayerPushSet
/*!
 *	プレイヤー押し設定
 *
 *	@param	ply_work	[io] プレイヤーワークポインタ
 */
// =====================================================================
void gmPlayerPushSet(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->seq_state == GME_PLY_SEQ_STATE_WALLPUSH &&
			ply_work->act_state == GME_PLY_ACT_STATE_PUSH2) {
		// 押し状態に変更
		ply_work->obj_work.move_flag |= OBD_MOVE_PUSH;
		//// 押し速度再設定
		//ply_work->obj_work.push_max = g_gm_player_parameter[ply_work->char_id].push_max;
	}
	else {
		// 押し状態でない
		ply_work->obj_work.move_flag &= ~OBD_MOVE_PUSH;
	}
#if 0
    // 押しアクションorニトロ時
    if ( ply_work->act_state == GMD_PLY_STATE_WALL2 || ply_work->player_flag & GMD_PLF_NITRO ){
        ply_work->obj_work.move_flag |= OBD_MOVE_PUSH;
        // ニトロの時は押し速度も速く
        if ( ply_work->player_flag & GMD_PLF_NITRO)
            ply_work->obj_work.push_max = ply_work->spd_max_nitro >> 1;
        else
            ply_work->obj_work.push_max = g_gm_player_parameter[ply_work->char_no].push_max;

    }else{
        // 押し状態ではない
        ply_work->obj_work.push_max = g_gm_player_parameter[ply_work->char_no].push_max;
        ply_work->obj_work.move_flag &= ~OBD_MOVE_PUSH;
    }
#endif
}


// =====================================================================
// gmPlayerEarthTouch
/*!
 *	どんな角度でも接地した方向に接地させる
 *
 *	@param	ply_work	[io] プレイヤーワークポインタ
 */
// =====================================================================
void gmPlayerEarthTouch(GMS_PLAYER_WORK *ply_work)
{
	// フラグチェック
	if ( ply_work->gmk_flag & GMD_PLGF_TOUCH ){
		u16 dir_z = 0;

		// 壁接地でくっつく
		if ( ply_work->obj_work.move_flag & OBD_MOVE_COL_MASK ) {
			// 角度設定
			if ( ply_work->obj_work.move_flag & OBD_MOVE_UNDER ) {
				GmPlySeqLandingSet(ply_work, 0x0000);
			}
			else if ( ply_work->obj_work.move_flag & OBD_MOVE_OVER ) {
				if ( ply_work->obj_work.disp_flag & OBD_DISP_HFLIP ) {
					GmPlySeqLandingSet(ply_work, 0x6000);
				}
				else {
					GmPlySeqLandingSet(ply_work, 0xa000);
				}
				// 頭の位置の角度取得
				// 空ならそのまま

				{
					// 接地角度を ply_work->obj_work.dir.z にセット
					OBS_COL_CHK_DATA	col_data;

					col_data.pos_x	= ply_work->obj_work.pos.x >> FX32_SHIFT;
					col_data.pos_y	= (ply_work->obj_work.pos.y >> FX32_SHIFT) + ply_work->obj_work.field_rect[OBD_TOP] - 4;
					col_data.flag	= (u16)(ply_work->obj_work.flag & OBD_OBJECT_B);
					col_data.vec	= OBD_COL_UP;
					col_data.dir	= &dir_z;
					col_data.attr	= NULL;
					dir_z = ply_work->obj_work.dir.z;
					ObjDiffCollisionFast(&col_data);
					ply_work->obj_work.dir.z = dir_z;
				}
			}
			else {
				if ( ply_work->obj_work.move_flag & OBD_MOVE_FRONT ) {
					if ( ply_work->obj_work.disp_flag & OBD_DISP_HFLIP ) {
						GmPlySeqLandingSet(ply_work, 0x4000);
					}
					else {
						GmPlySeqLandingSet(ply_work, 0xc000);
					}
				}
				else if ( ply_work->obj_work.move_flag & OBD_MOVE_BACK ) {
					if ( ply_work->obj_work.disp_flag & OBD_DISP_HFLIP ) {
						GmPlySeqLandingSet(ply_work, 0xc000);
					}
					else {
						GmPlySeqLandingSet(ply_work, 0x4000);
					}
				}
			}

			// 向き変更
			if (ply_work->gmk_flag & GMD_PLGF_TOUCH_FORCE_DIR) {
				// 接地方向強制する
				if (ply_work->obj_work.dir.z < 0x8000) {
					ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
					ply_work->obj_work.spd_m = -MTM_MATH_ABS(ply_work->obj_work.spd_m);
				} else {
					ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
					ply_work->obj_work.spd_m = MTM_MATH_ABS(ply_work->obj_work.spd_m);
				}
			} else {
				// フラグにより接地方向を変更するかチェック
				if (ply_work->gmk_flag & GMD_PLGF_TOUCH_DISP_FLIP) {
					ply_work->obj_work.disp_flag ^= OBD_DISP_HFLIP;
				}
				if (ply_work->gmk_flag &  GMD_PLGF_TOUCH_FLIP) { 
					ply_work->obj_work.disp_flag ^= OBD_DISP_HFLIP;
					ply_work->obj_work.spd_m = -ply_work->obj_work.spd_m;
				}
			}
			// 接地状態終了
			ply_work->obj_work.move_flag |= OBD_MOVE_UNDER;
			ply_work->gmk_flag &= ~(GMD_PLGF_TOUCH | GMD_PLGF_TOUCH_FORCE_DIR | GMD_PLGF_TOUCH_FLIP | GMD_PLGF_TOUCH_DISP_FLIP);
			GmPlySeqChangeFw(ply_work);
		}
	}
}

// =====================================================================
// gmPlayerWaterCheck
/*!
 *	水中チェック
 *
 *	@param	ply_work	[io] プレイヤーワークポインタ
 */
// =====================================================================
void gmPlayerWaterCheck(GMS_PLAYER_WORK *ply_work)
{
	fx32	rest_time;
//	// スタートデモ中は判定しない◆
//	if ( !(g_gm_game_info.flag & GMD_MAIN_FLAG_COUNT_TIME) ) {
//		return;
//	}
	if (GmMainIsWaterLevel()) {
		// 胸まで水中等は後で◆
		if ((ply_work->obj_work.pos.y >> FX32_SHIFT) - GMD_PLAYER_WATER_CHECK_OFST >= g_gm_main_system.water_level) {
			BOOL	b_face_up = FALSE;	// 顔が水面の上
			// 水中
			if (!(ply_work->player_flag & GMD_PLF_WATER)) {
				// 水中に入った瞬間
				if (!(g_gm_main_system.game_flag & GMD_GAME_FLAG_WATER_LEVEL_EFCT_OFF)) {
					// エフェクト 水しぶき
					GmPlyEfctCreateSpray(ply_work);
					// エフェクト 泡
					GmPlyEfctCreateBubble(ply_work);
					// SE
					GmSoundPlaySE("Spray");
				}
				// BGMチェンジ？

				// 水中速度設定
				GmPlayerSpdParameterSetWater(ply_work, TRUE);
			}
			ply_work->player_flag |= GMD_PLF_WATER;

			// 水中息切れチェックカウントアップ
			if (!(ply_work->player_flag & (GMD_PLF_DIE | GMD_PLF_GOAL))) {
				// 首より上が水面の上ならセーフ
				if ((ply_work->obj_work.pos.y >> FX32_SHIFT)-GMD_PLAYER_WATER_FACE_UP_OFST >=
						g_gm_main_system.water_level) {
					ply_work->water_timer = ObjTimeCountUp(ply_work->water_timer);
				}
				else {
					ply_work->water_timer = 0;

					// 半水中走り水しぶき
					GmPlyEfctCreateRunSpray(ply_work);	// エフェクトの方で生成判定を行う

					b_face_up = TRUE;
				}
			}

			if (!(ply_work->player_flag & (GMD_PLF_DIE | GMD_PLF_GOAL))) {
				// 水中時泡エフェクト
				if (!((ply_work->water_timer >> FX32_SHIFT) % 50) &&	// ランダムにする？◆
						ply_work->water_timer < ply_work->time_air) {
				//if (g_gm_main_system.game_flag & GMD_GAME_FLAG_COUNT_GAME_TIME) {
				//	if (!(g_gm_main_system.game_time & 0x1f) && !(mtMathRand() & 0x003)) {
						// エフェクト 泡
						GmPlyEfctCreateBubble(ply_work);
				//	}
				}

				// 水中一定間隔警告SE
				if (!b_face_up && !((ply_work->water_timer >> FX32_SHIFT) % GMD_PL_WATER_ATTENSION_SE_INT)) {

					if (!(ply_work->gmk_flag & GMD_PLGF_WATER_ALERT)) {
						// SE
						GmSoundPlaySE("Attention");
					}

					ply_work->gmk_flag |= GMD_PLGF_WATER_ALERT;
				}
				else {
					ply_work->gmk_flag &= ~GMD_PLGF_WATER_ALERT;
				}

				// 息切れカウンタ
				rest_time = ply_work->time_air - ply_work->water_timer;
				if (rest_time >= GMD_PLAYER_WATER_COUNT_TIME_OFST) {
					if (rest_time - GMD_PLAYER_WATER_COUNT_TIME_OFST <= 10*60*FX32_ONE &&
							!(((rest_time - GMD_PLAYER_WATER_COUNT_TIME_OFST) >> FX32_SHIFT) % (2*60))) {
						u32		no;

						no = (u32)(((rest_time - GMD_PLAYER_WATER_COUNT_TIME_OFST) >> FX32_SHIFT) / (2*60));
						no = MTM_MATH_CLIP(no, 0, 5);

						// エフェクト
						GmPlyEfctWaterCount(ply_work, no);
					}
				}
				// 溺れジングル
				if (rest_time == ((10*60+30)*FX32_ONE+GMD_PLAYER_WATER_COUNT_TIME_OFST)) {
					// 警告ジングル開始
					GmSoundPlayJingleObore();
				}
				else if (rest_time > (10*60+30)*FX32_ONE+GMD_PLAYER_WATER_COUNT_TIME_OFST) {
					// 溺れジングル停止
					GmSoundStopJingleObore();
				}
				// 息切れ死亡チェック
				if (ply_work->water_timer > ply_work->time_air) {
					// 息切れ死亡
					GmPlySeqChangeDeath(ply_work);
					ply_work->obj_work.spd.y = 0;

					// 息切れ死亡エフェクト
					GmPlyEfctWaterDeath(ply_work);
					return;
				}
			} // if (!(ply_work->player_flag & (GMD_PLF_DIE | GMD_PLF_GOAL))
		}
		else {
			if (ply_work->player_flag & GMD_PLF_WATER) {
				// さっきまで水中だった
				if (!(g_gm_main_system.game_flag & GMD_GAME_FLAG_WATER_LEVEL_EFCT_OFF)) {
					// エフェクト 水しぶき
					GmPlyEfctCreateSpray(ply_work);
					// SE
					GmSoundPlaySE("Spray");
				}
				// BGMチェンジ
				// 溺れジングル停止
				GmSoundStopJingleObore();

				// 陸上速度設定
				GmPlayerSpdParameterSetWater(ply_work, FALSE);
			}
			ply_work->player_flag &= ~GMD_PLF_WATER;
			ply_work->water_timer = 0;

			// 速度再設定
			ply_work->obj_work.spd_fall		= g_gm_player_parameter[ply_work->char_id].spd_fall;
			ply_work->obj_work.spd_fall_max	= g_gm_player_parameter[ply_work->char_id].spd_fall_max;

			return;
		}
	}
	else {
		// 水中で無い
		if (ply_work->player_flag & GMD_PLF_WATER) {
			// いきなり無効になった場合はエフェクトなどは出さない
			// 速度再設定
			ply_work->obj_work.spd_fall		= g_gm_player_parameter[ply_work->char_id].spd_fall;
			ply_work->obj_work.spd_fall_max	= g_gm_player_parameter[ply_work->char_id].spd_fall_max;
		}
		ply_work->player_flag &= ~GMD_PLF_WATER;
		ply_work->water_timer = 0;

		// 各種演出用フラグクリア
		ply_work->gmk_flag &= ~GMD_PLGF_WATER_ALERT;

		return;
    }
}

// =====================================================================
// gmPlayerTimeOverCheck
/*!
 *	タイムオーバーチェック
 *
 *	@param	ply_work	[io] プレイヤーワークポインタ
 */
// =====================================================================
void gmPlayerTimeOverCheck(GMS_PLAYER_WORK *ply_work)
{
		
	if (g_gm_main_system.game_flag & (GMD_GAME_FLAG_CLEAR				// ステージクリアー
									| GMD_GAME_FLAG_RESULT_START		// リザルト演出開始
									| GMD_GAME_FLAG_RESULT_END			// リザルト演出終了
									| GMD_GAME_FLAG_GAMEOVER_START		// ゲームオーバー演出開始
									| GMD_GAME_FLAG_TIMEOVER			// タイムオーバー死亡
									| GMD_GAME_FLAG_SPECIAL_STAGE		// スペステリング突入
									| GMD_GAME_FLAG_SPL_TIMEOVER		// スペステ：タイムオーバー
									| GMD_GAME_FLAG_GOAL_IN ) ) {		// ゴールした瞬間からＯＮ
		return;
	}

	if (ply_work->player_flag & GMD_PLF_TATK_RETRY){						// リトライ画面中
		return;
	}

	if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// ◆タイムアウト死亡ありの時
		if (!(ply_work->player_flag & GMD_PLF_DIE) &&
				g_gm_main_system.game_time >= GMD_MAIN_TIME_MAX) {
			// タイムオーバー死亡
			GmPlySeqChangeDeath(ply_work);

			g_gm_main_system.game_flag |= GMD_GAME_FLAG_TIMEOVER;

			// 保存時間をクリア
			///sss[TIMEOVER]	
			g_gm_main_system.time_save = 0;

			// タイムオーバー発生を保存
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_TIMEOVER;
		}
	} else {
		// スペステタイムアウトチェックはこちら
		if ((s32)(g_gm_main_system.game_time) <= 0) {

			// タイムオーバーを通知
			g_gm_main_system.game_flag |= GMD_GAME_FLAG_SPL_TIMEOVER;

			// 保存時間をクリア
			///sss[TIMEOVER]	
			g_gm_main_system.time_save = 0;

			// タイムオーバー発生を保存
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_TIMEOVER;

			// プレイヤー拘束
			ply_work->obj_work.move_flag |= (  OBD_MOVE_NOMOVE			// 移動停止
											 | OBD_MOVE_NOCOL );		// コリジョン無し
		}
	}
}

// =====================================================================
// gmPlayerFallDownCheck
/*!
 *	落下死亡チェック
 *
 *	@param	ply_work	[io] プレイヤーワークポインタ
 */
// =====================================================================
void gmPlayerFallDownCheck(GMS_PLAYER_WORK *ply_work)
{
	if (!(ply_work->player_flag & GMD_PLF_DIE)) {
		if (((g_gm_main_system.map_size[MTD_Y] - 16) << FX32_SHIFT) <=
				ply_work->obj_work.pos.y) {
			// 最下層落下死亡
			GmPlySeqChangeDeath(ply_work);
		}
	}
}

// =====================================================================
// gmPlayerPressureCheck
/*!
 *	プレイヤー押し潰されチェック
 *
 *	@param	ply_work	[io] プレイヤーワークポインタ
 */
// =====================================================================
void gmPlayerPressureCheck(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->player_flag & GMD_PLF_COLDIE && !ply_work->obj_work.touch_obj) {
		// 接触オブジェがなければ左右の埋まりチェックを行う
		if ((ply_work->obj_work.move_flag & OBD_MOVE_FRONT && ply_work->obj_work.move_flag & OBD_MOVE_BACK)) {
			GmPlySeqChangeDeath(ply_work);
		}
	}
    
	// 接触オブジェクト地形があって
	if (ply_work->obj_work.touch_obj && !(ply_work->player_flag & GMD_PLF_NOCOLDIE)) {

		// 圧死可能オブジェクトチェック
		if (ply_work->obj_work.touch_obj->obj_type != GMD_OBJTYPE_GIMMICK ||
				(ply_work->obj_work.touch_obj->obj_type == GMD_OBJTYPE_GIMMICK &&
					!(((GMS_ENEMY_COM_WORK*)ply_work->obj_work.touch_obj)->enemy_flag & GMD_ENEMY_FLAG_NOPRESSDIE))) {
			
			if (!ply_work->obj_work.ride_obj && ply_work->obj_work.move_flag & OBD_MOVE_UNDER &&
					ply_work->obj_work.move_flag & OBD_MOVE_OVER &&
					ply_work->obj_work.touch_obj->move.y <= 0) {
				// 乗っているオブジェクトがなくて上下が壁に接触している時は
				// 上の辺りがオブジェクトなので 接触オブジェクトが上に移動していたらあたらない
			}
			else {
				// 上下、もしくは左右が壁にHITしている時
				if ((ply_work->obj_work.move_flag & OBD_MOVE_UNDER && ply_work->obj_work.move_flag & OBD_MOVE_OVER) ||
					(ply_work->obj_work.move_flag & OBD_MOVE_FRONT && ply_work->obj_work.move_flag & OBD_MOVE_BACK)) {
					// 死亡
					//if ( ++ply_work->pressure_timer > 2 )
					GmPlySeqChangeDeath(ply_work);
				}
			}
		}
	}

	// 圧死タイマークリア
	//ply_work->pressure_timer = 0;
}


// =====================================================================
// gmPlayerSuperSonicCheck
/*!
 *	スーパーソニックチェック
 *
 *	@param	ply_work	[io] プレイヤーワークポインタ
 */
// =====================================================================
void gmPlayerSuperSonicCheck(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
		// リングチェック
		ply_work->super_sonic_ring_timer = ObjTimeCountDown(ply_work->super_sonic_ring_timer);
		if (!ply_work->super_sonic_ring_timer) {
			// リングを一つ減らす
			ply_work->ring_num--;
			if (ply_work->ring_num <= 0) {

				ply_work->ring_num = 0;

				gmPlayerSuperSonicToSonic(ply_work);
			}
			else {
				ply_work->super_sonic_ring_timer = GMD_PL_SUPER_SONIC_RING_DEC_INT * FX32_ONE;
			}
		}
	}
}


// =====================================================================
// gmPlayerSuperSonicToSonic
/*!
 *	スーパーソニックからソニックへアクション変更
 *
 *	@param	ply_work	[io] プレイヤーワークポインタ
 */
// =====================================================================
void gmPlayerSuperSonicToSonic(GMS_PLAYER_WORK *ply_work)
{
	GMS_PLAYER_RESET_ACT_WORK	reset_act_work;
	// アクション再設定情報取得
	GmPlayerSaveResetAction(ply_work, &reset_act_work);

	// スーパーソニック終了
	GmPlayerSetEndSuperSonic(ply_work);

	// 現在のアクションを設定しなおす
	if ((!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER) ||
				(ply_work->obj_work.move_flag & OBD_MOVE_JUMP)) &&
			ply_work->act_state == GME_PLY_ACT_STATE_DASH_1 ||
			ply_work->act_state == GME_PLY_ACT_STATE_DASH_2) {
		// 落下アクションにきりかえる
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_FALL_R);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}
	else {
		GmPlayerResetAction(ply_work, &reset_act_work);
	}
}
// =====================================================================
// gmPlayerGetHomingTarget
/*!
 *	ホーミングターゲット取得
 *
 *	@param	ply_work	[io] プレイヤーワークポインタ
 */
// =====================================================================
fx32	test_dist = GMD_PL_HOMING_DIST;
void gmPlayerGetHomingTarget(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK		*search_obj, *target_obj;
	GMS_ENEMY_COM_WORK	*ene_com;
	float				ply_pos_x, ply_pos_y;
	float				target_pos_x, target_pos_y;
	float				dist_x, dist_y, dist_now;
	float				dist_max = FXM_FX32_TO_FLOAT(GMD_PL_HOMING_DIST) * FXM_FX32_TO_FLOAT(GMD_PL_HOMING_DIST);
	Angle32				angle_s, angle_e;
	Angle32				target_angle;
	OBS_RECT_WORK		*rect_work;

	float				dist_per = GMD_PL_HOMING_DIST_PER;//((float)((float)GMD_PL_HOMING_DIST/(float)GMD_PL_HOMING_DIST_UNDER));

	if (ply_work->homing_boost_timer) {
		//dist_max = FXM_FX32_TO_FLOAT(test_dist) * FXM_FX32_TO_FLOAT(test_dist);
		dist_per = ((float)((float)GMD_PL_HOMING_DIST/(float)GMD_PL_HOMING_DIST));

		ply_work->homing_boost_timer = ObjTimeCountDown(ply_work->homing_boost_timer);
	}


	if (ply_work->enemy_obj) {
		// 死亡チェック
		if (ply_work->enemy_obj->flag & OBD_OBJECT_TASKCLEAR) {
			ply_work->enemy_obj = NULL;
		}
	}
//	if ( ply_work->player_flag & GMD_PLF_NOHOMING ){
//		return;
//	}
//
//	if ( GmMainIsBossStage() )
//		return;

	rect_work = &ply_work->rect_work[GMD_PLAYER_RECT_BODY];
	ply_pos_x = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.x);
	ply_pos_y = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.y + ((rect_work->rect.top + rect_work->rect.bottom)>>1));

	// サーチ範囲角度取得
	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
		angle_s = 0x8000 - GMD_PL_HOMING_DIR_S;
		angle_e = 0x8000 - GMD_PL_HOMING_DIR_E;
	}
	else {
		angle_s = GMD_PL_HOMING_DIR_S;
		angle_e = GMD_PL_HOMING_DIR_E;
	}
	if (angle_e < angle_s) {
		MTM_MATH_SWAP(angle_s, angle_e);
	}

	search_obj = ObjObjectSearchRegistObject(NULL, 0xFFFF);
	target_obj = NULL;

	for ( ; search_obj; search_obj = ObjObjectSearchRegistObject(search_obj, 0xFFFF)) {

		ene_com = (GMS_ENEMY_COM_WORK*)search_obj;

		if (search_obj->disp_flag & OBD_DISP_NODISP) {
			continue;
		}

		// サーチ対象
		// エネミー, スプリング, アイテムボックス, 破壊オブジェ, ターザンロープ
		// 滑車, 大砲, スイッチ
		if (search_obj->obj_type == GMD_OBJTYPE_GIMMICK) {

			if (ene_com->enemy_flag & GMD_ENEMY_FLAG_NOHOMING) {
			// ホーミング対象からはずすギミック
				continue;
			}
			if (!(GMD_EVENT_ID_GMK_ITEM_HISPEED <= ene_com->eve_rec->id &&
						ene_com->eve_rec->id <= GMD_EVENT_ID_GMK_ITEM_1UP &&
						!ene_com->eve_rec->byte_param[1]/*非破壊分*/) &&			// アイテムボックス
					!(GMD_EVENT_ID_GMK_SPRING_U <= ene_com->eve_rec->id &&
						ene_com->eve_rec->id <= GMD_EVENT_ID_GMK_SPRING_LUG) &&	
					!(GMD_EVENT_ID_GMK_SPRING_RUG_A <= ene_com->eve_rec->id &&
						ene_com->eve_rec->id <= GMD_EVENT_ID_GMK_SPRING_LUG_A) &&	// スプリング
					!(GMD_EVENT_ID_BREAKOBJ == ene_com->eve_rec->id)&&				// 破壊オブジェ			
					!(GMD_EVENT_ID_TARZAN_ROPE <= ene_com->eve_rec->id &&
						ene_com->eve_rec->id <= GMD_EVENT_ID_TARZAN_ROPE_R) &&		// ターザンロープ
					!(GMD_EVENT_ID_CAPSULE == ene_com->eve_rec->id) &&				// カプセル
					!(GMD_EVENT_ID_PULLEY_BASE == ene_com->eve_rec->id) &&			// 滑車
					!(GMD_EVENT_ID_CANNON == ene_com->eve_rec->id) &&				// 大砲
					!(GMD_EVENT_ID_GMK_SWITCH == ene_com->eve_rec->id)) {				// スイッチ
				continue;
			}
		}
		else if (search_obj->obj_type != GMD_OBJTYPE_ENEMY) {
			continue;
		}
		else if (ene_com->enemy_flag & GMD_ENEMY_FLAG_NOHOMING) {
			// ホーミング対象からはずす敵
			continue;
		}
		// 未登録分
		// スイッチ

		rect_work = &ene_com->rect_work[GMD_ENEMY_RECT_BODY];
		target_pos_x = FXM_FX32_TO_FLOAT(search_obj->pos.x);
		target_pos_y = FXM_FX32_TO_FLOAT(search_obj->pos.y + ((rect_work->rect.top + rect_work->rect.bottom)>>1));

		dist_x = target_pos_x - ply_pos_x;
		dist_y = target_pos_y - ply_pos_y;

		// サーチ範囲角度チェック
		target_angle = nnArcTan2(dist_y, dist_x);	// 逆回転で判定
		if (target_angle < angle_s ||
				target_angle > angle_e) {
			continue;
		}

		// サーチ範囲距離チェック
#if 1
#if 1
		// 楕円チェック
		// 縦の長さを基点として変換
		dist_y = dist_y * dist_per;
		dist_now = dist_x * dist_x + dist_y * dist_y;
		if (dist_now < dist_max) {
			dist_max = dist_now;
			target_obj = search_obj;
		}
#else
		// 楕円チェック
		// 縦の長さを基点として変換
		dist_y = dist_y * GMD_PL_HOMING_DIST_PER;
		dist_now = dist_x * dist_x + dist_y * dist_y;
		if (dist_now < dist_max) {
			dist_max = dist_now;
			target_obj = search_obj;
		}
#endif
#else
		// 円チェック
		dist_now = dist_x * dist_x + dist_y * dist_y;
		if (dist_now < dist_max) {
			dist_max = dist_now;
			target_obj = search_obj;
			//OS_Printf("search dir : %d\n", target_angle);
		}
#endif
	}

	// ターゲット設定
	ply_work->enemy_obj = target_obj;
	if (ply_work->cursol_enemy_obj != ply_work->enemy_obj ||
			!GmPlySeqCheckAcceptHoming(ply_work)) {
		// カーソル生成対象エネミークリア
		ply_work->cursol_enemy_obj = NULL;
	}
}

// =====================================================================
// キー関連
// =====================================================================
// ================================================================
// gmPlayerKeyGet
/*!
  プレイヤーキー関数
 */
// ================================================================
void gmPlayerKeyGet(GMS_PLAYER_WORK *ply_work)
{
#if 0
	// キーステート更新
	if (ply_work->player_id == 0) {
		if (!(g_gm_main_system.flag & GMD_MAIN_FLAG_KEY_PLAY)) {
			// 通常
			mtPadUpdateKey(&g_gm_main_system.key_status, PAD_Read());
		}
		else {
			// キープレイ用
		//	mtPadUpdateKey(&ply_work->key_status, g_gm_main_system.key_play_buf);
		}
	}
#endif

	// キー無視チェック
	if (ply_work->no_key_timer || (ply_work->player_flag & GMD_PLF_NOKEY)) {
		// キー入力無効タイマー
		ply_work->no_key_timer = ObjTimeCountDown(ply_work->no_key_timer);
		ply_work->key_on     = 0;
		ply_work->key_push   = 0;
		ply_work->key_repeat = 0;
		ply_work->key_release = 0;
#if _IPHONE
		ply_work->key_rot_z   = 0;
		ply_work->key_walk_rot_z = 0;
#endif // _IPHONE
		return;
	}
	if (!(ply_work->player_flag & GMD_PLF_NOKEY)) {
		if (ply_work->player_id == 0) {
			u16	change, key_on;
			s32	i;
			Angle32	rot_z = 0;
#if !_WII
			u16	cursol_key = 0;
#endif

			// 傾斜値取得
#if _PC | _XBOX
			if ( (AoPadTriggerL() > (s32)(0x400 * 0.007782))
			   ||(AoPadTriggerR() > (s32)(0x400 * 0.007782)) ) {
				rot_z = (s32)nnRoundOff((float)(AoPadTriggerR() / 0.007782))
					   -(s32)nnRoundOff((float)(AoPadTriggerL() / 0.007782));
			}
#elif _PS3
			if ( (AoPadRotZ() > 0x400)
			   ||(AoPadRotZ() <-0x400) ) {
				rot_z = -AoPadRotZ();
			}
#elif _WII
			if ( (AoPadRotX() > 0x400)
			   ||(AoPadRotX() <-0x400) ) {
				rot_z = AoPadRotX();
			}
#elif _IPHONE
			rot_z = gmPlayerKeyGetRotZ(ply_work);
#if TARGET_IPHONE_SIMULATOR
			{
				static Angle32 s_rotate = 0;
				u16 direct = dbg::CPadEmu::CreateInstance().GetPadStand();
				if (KEY_L_RIGHT & direct) {
					s_rotate += 0x4000;
					if (0x4000 < s_rotate) {
						s_rotate = 0x4000;
					}
				} else if (KEY_L_LEFT & direct) {
					s_rotate -= 0x4000;
					if (s_rotate < -0x4000) {
						s_rotate = -0x4000;
					}
				}
				rot_z = s_rotate;
			}
#endif //TARGET_IPHONE_SIMULATOR
#else
#endif

			// 傾斜角度保存
			ply_work->prev_key_rot_z = ply_work->key_rot_z;
			ply_work->key_rot_z = rot_z;

			// キーリマップ
#if !_IPHONE
#if _WII
			key_on					= gmPlayerRemapKey(ply_work, AoPadDirect(), rot_z);
#else
			cursol_key = AoPadADirect() & (KEY_L_RIGHT|KEY_L_LEFT|KEY_L_UP|KEY_L_DOWN);
			if (!cursol_key) {
				cursol_key = AoPadDirect() & (KEY_L_RIGHT|KEY_L_LEFT|KEY_L_UP|KEY_L_DOWN);
			}
			key_on					= gmPlayerRemapKey(ply_work,
												(u16)(cursol_key |
												(AoPadDirect() & (KEY_R_RIGHT|KEY_R_LEFT|KEY_R_UP|KEY_R_DOWN))), rot_z);
#endif
#else // !_IPHONE
#if defined(MTD_DEBUG)
			key_on					= gmPlayerRemapKey(ply_work, dbg::CPadEmu::CreateInstance().GetPadDirect(), rot_z);
#else
			key_on					= gmPlayerRemapKey(ply_work, 0, rot_z);
#endif //defined(MTD_DEBUG)
#endif // !_IPHONE
			change					= (u16)(ply_work->key_on ^ key_on);
			ply_work->key_push		= (u16)(change & key_on);
			ply_work->key_release	= (u16)(change & ~key_on);
			ply_work->key_on		= key_on;

			// リピート
			ply_work->key_repeat	= 0;
			for (i = 0; i < GMD_PLAYER_KEY_MAP_MAX; i++) {
				if (!(ply_work->key_on & gm_key_map_key_list[i])) {
					ply_work->key_repeat_timer[i] = AMD_PAD_REPEAT_START;
				}
				else if (--ply_work->key_repeat_timer[i] == 0) {
					ply_work->key_repeat |= ply_work->key_on & gm_key_map_key_list[i];
					ply_work->key_repeat_timer[i] = AMD_PAD_REPEAT_SPEED;
				}
			}


			// 歩き角度取得
			ply_work->key_walk_rot_z = 0;
			if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
				// クラシック
#if _PS3
				if ( (AoPadAnalogLX() > 0x1000)
						||(AoPadAnalogLX() <-0x1000) ) {
					ply_work->key_walk_rot_z = AoPadAnalogLX();
				}

				// アナログキーチェック
				if (!ply_work->key_walk_rot_z) {
					if (ply_work->key_on & KEY_L_RIGHT) {
						ply_work->key_walk_rot_z = 0x7FFF;
					}
					else if (ply_work->key_on & KEY_L_LEFT) {
						ply_work->key_walk_rot_z = -0x7FFF;
					}
				}
#elif _XBOX | _PC
				if ( (AoPadAnalogLX() > 0x1000)
						||(AoPadAnalogLX() <-0x1000) ) {
					ply_work->key_walk_rot_z = AoPadAnalogLX();
				}

				// アナログキーチェック
				if (!ply_work->key_walk_rot_z) {
					if (ply_work->key_on & KEY_L_RIGHT) {
						ply_work->key_walk_rot_z = 0x7FFF;
					}
					else if (ply_work->key_on & KEY_L_LEFT) {
						ply_work->key_walk_rot_z = -0x7FFF;
					}
				}
#elif _WII
				if (ply_work->key_on & KEY_L_RIGHT) {
					ply_work->key_walk_rot_z = 0x7FFF;
				}
				else if (ply_work->key_on & KEY_L_LEFT) {
					ply_work->key_walk_rot_z = -0x7FFF;
				}
#else
				if (ply_work->key_on & KEY_L_RIGHT) {
					ply_work->key_walk_rot_z = 0x7FFF;
				}
				else if (ply_work->key_on & KEY_L_LEFT) {
					ply_work->key_walk_rot_z = -0x7FFF;
				}
#endif
			}
			else {
				// ノーマル
				// 歩き用角度取得
				ply_work->key_walk_rot_z = rot_z;
			}
#if 0
			if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
				// クラシック
#if _PS3
				if ( (AoPadRotZ() > 0x400)
				   ||(AoPadRotZ() <-0x400) ) {
					ply_work->key_rot_z = -AoPadRotZ();
				}
#elif _XBOX | _PC
				ply_work->key_rot_z = 0;
				if ( (AoPadAnalogLX() > 0x1000)
				   ||(AoPadAnalogLX() <-0x1000) ) {
					ply_work->key_rot_z = AoPadAnalogLX();
				}
				// アナログキーチェック
				if (!ply_work->key_rot_z) {
					if (ply_work->key_on & KEY_L_RIGHT) {
						ply_work->key_rot_z = 0x7FFF;
					}
					else if (ply_work->key_on & KEY_L_LEFT) {
						ply_work->key_rot_z = -0x7FFF;
					}
				}
#else// _WII
				ply_work->key_rot_z = 0;
				if (ply_work->key_on & KEY_L_RIGHT) {
					ply_work->key_rot_z = 0x7FFF;
				}
				else if (ply_work->key_on & KEY_L_LEFT) {
					ply_work->key_rot_z = -0x7FFF;
				}
#endif
			}
			else {
				// ノーマル
				ply_work->key_rot_z = 0;
#if _PS3
				if ( (AoPadRotZ() > 0x400)
				   ||(AoPadRotZ() <-0x400) ) {
					ply_work->key_rot_z = -AoPadRotZ();
				}
#elif _XBOX | _PC
				if ( (AoPadAnalogLX() > 0x1000)
				   ||(AoPadAnalogLX() <-0x1000) ) {
					ply_work->key_rot_z = AoPadAnalogLX();
				}

				// アナログキーチェック
				if (!ply_work->key_rot_z) {
					if (ply_work->key_on & KEY_L_RIGHT) {
						ply_work->key_rot_z = 0x7FFF;
					}
					else if (ply_work->key_on & KEY_L_LEFT) {
						ply_work->key_rot_z = -0x7FFF;
					}
				}
#elif _WII
				if ( (AoPadRotX() > 0x400)
				   ||(AoPadRotX() <-0x400) ) {
					ply_work->key_rot_z = AoPadRotX();
				}
#else
				if (ply_work->key_on & KEY_L_RIGHT) {
					ply_work->key_rot_z = 0x7FFF;
				}
				else if (ply_work->key_on & KEY_L_LEFT) {
					ply_work->key_rot_z = -0x7FFF;
				}
#endif
			}
#endif

		}
	}
	if (GMM_MAIN_STAGE_IS_ENDING()) {
		// エンディング中ならデモ用にキー変更
		GmEndingPlyKeyCustom(ply_work);
	}
}

// ================================================================
// gmPlayerRemapKey
/*!
  プレイヤーのキーデータをマッピングする
  
  @param    player  [in] 対象プレイヤー
  @param    key     [in] 入力キー
  
  @return   マッピング後のキー
  
  @note
  この関数を通すと設定されたマッピングテーブルを元にキーが入れ替わります。
 */
// ================================================================
u16 gmPlayerRemapKey(GMS_PLAYER_WORK *ply_work, u16 key, Angle32 key_rot_z)
{
    u16     new_key = (u16)(key & ~(KEY_L_RIGHT|KEY_L_LEFT|KEY_L_UP|KEY_L_DOWN
										|KEY_R_RIGHT|KEY_R_LEFT|KEY_R_DOWN|KEY_R_RIGHT));

#if _IPHONE
	// TRUCK:20100414
	if (GSM_MAIN_STAGE_IS_SPSTAGE() || (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2)) {
		// 3-2 or SS
		if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK) {
			// フリックなら
			// 傾きシステムからフォーカス中タッチを入手
			gm::CPadPolarHandle &polar = gm::CPadPolarHandle::CreateInstance();
			s32 index = polar.GetFocusTpIndex();
			
			if (gmPlayerIsInputDPadJumpKey(ply_work, index)) {
				// ジャンプ
				new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_A];
			}
			if (gmPlayerIsInputDPadSSonicKey(ply_work, index)) {
				// 変身
				new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_X];
			}
		}
		else {
			// 傾斜操作ならいつもどおり
			new_key |= gmPlayerRemapKeyIPhoneZone32SS(ply_work);
		}
	}
	else 
#endif // _IPHONE
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
		// クラシックモード
#if _IPHONE
		gm::CPadVirtualPad &virtual_pad = gm::CPadVirtualPad::CreateInstance();
		u16 pad = virtual_pad.GetValue();
		if( KEY_L_RIGHT & pad ) {
			new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_RIGHT];
		}
		else if( KEY_L_LEFT & pad ) {
			new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_LEFT];
		}
		else if( KEY_L_UP & pad ) {
			new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_UP];
		}
		else if( KEY_L_DOWN & pad ) {
			new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_DOWN];
		}
		if (gmPlayerIsInputDPadJumpKey(ply_work, -1)) {
			// ジャンプ
			new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_A];
		}
		if (gmPlayerIsInputDPadSSonicKey(ply_work, -1)) {
			// 変身
			new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_X];
		}
#else
		if( KEY_L_RIGHT & key ) {
			new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_RIGHT];
		}
		if( KEY_L_LEFT & key ) {
			new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_LEFT];
		}
		if( KEY_L_UP & key ) {
			new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_UP];
		}
		if( KEY_L_DOWN & key ) {
			new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_DOWN];
		}
#endif // _IPHONE
	}
	else {
		// ノーマルモード
		if (key_rot_z > 0x400) {
			new_key |= KEY_L_RIGHT;
		}
		else if (key_rot_z < -0x400) {
			new_key |= KEY_L_LEFT;
		}
#if _IPHONE
		// ジャンプ/スピンなどの制御関係
		new_key |= gmPlayerRemapKeyIPhone(ply_work);
#endif //_IPHONE
	}

    if( KEY_R_DOWN & key ) {
        new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_A];
	}
    if( KEY_R_RIGHT & key ) {
        new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_B];
	}
    if( KEY_R_LEFT & key ) {
        new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_X];
	}
    if( KEY_R_UP & key ) {
        new_key |= ply_work->key_map[GMD_PLAYER_KEY_MAP_Y];
	}

    return new_key;
}


#if _IPHONE
// ================================================================
// gmPlayerKeyGetRotZ
/*!
 Z回転の値を作成する(iPhone)
 
 @param    ply_work  [in] 対象プレイヤーワーク
 
 @return   回転値
 
 @note
 ステージによって作成するZ回転の値の計算式を変更します。
 */
// ================================================================
static Angle32 gmPlayerKeyGetRotZ(GMS_PLAYER_WORK *ply_work)
{
	Angle32 rot_z = 0;
	
	ply_work->is_nudge = FALSE; // 揺らしはなしで初期化
	
#ifdef GMD_MAIN_USE_BODY_ROTATE
	GSE_MAIN_STAGE_ID stage_id = (GSE_MAIN_STAGE_ID)g_gs_main_sys_info.stage_id;
	// not 3-2 or SS
	if (!GSM_MAIN_STAGE_IS_SPSTAGE() && !(stage_id == GSD_MAIN_STAGE_ID_3_2)) {
		rot_z = nnArcTan2(_am_iphone_accel_data.sensor.x , -_am_iphone_accel_data.sensor.z);
		if (rot_z < 0x800 && rot_z > -0x800) {
			rot_z = 0;
		}
		else {
			rot_z *= 4; // base:3
			if (rot_z > 0x8000) {
				rot_z = 0x8000;
			}
			else if (rot_z < -0x8000) {
				rot_z = -0x8000;
			}
		}
	}
	else {
		rot_z = _am_iphone_accel_data.rot_z;
		// 回転補正
		if (rot_z >  0x8000) {
			rot_z -= 0x8000;
		}
		if (rot_z < -0x8000) {
			rot_z += 0x8000;
		}
	}
#endif // GMD_MAIN_USE_BODY_ROTATE

#if 01
	GSE_MAIN_STAGE_ID stage_id = (GSE_MAIN_STAGE_ID)g_gs_main_sys_info.stage_id;
	
	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// 揺らし計算
		NNS_VECTOR* hard_accel = &(_am_iphone_accel_data.core);
		NNS_VECTOR* ply_accel  = &(ply_work->calc_accel);
		
		// 加速度を分解して判定する
		ply_accel->x = hard_accel->x * 0.1f + ply_accel->x * (1.0f - 0.1f);
		ply_accel->y = hard_accel->y * 0.1f + ply_accel->y * (1.0f - 0.1f);
		ply_accel->y = hard_accel->y * 0.1f + ply_accel->y * (1.0f - 0.1f);
		
		float x = hard_accel->x - ply_accel->x;
		float y = hard_accel->y - ply_accel->y;
		float z = hard_accel->z - ply_accel->z;
		
		float length = nnSqrt(x*x + y*y + z*z);
		
		if (length >= 2.0f) {
			ply_work->is_nudge = TRUE;
		}
	}
	
#if 01
	if (GSM_MAIN_STAGE_IS_SPSTAGE() && (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK)) {
		// SS
		//rot_z = g_gm_main_system.polar_now;
		rot_z = g_gm_main_system.polar_diff;
	}
	else 
#endif // 01
#if 01
	// TRUCK:20100414
	if (stage_id == GSD_MAIN_STAGE_ID_3_2 && (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK)) {
		// Zone3-2
		rot_z = g_gm_main_system.polar_diff;
	}
	else 
#endif // 01
	{
		// other
		//rot_z = nnArcTan2(_am_iphone_accel_data.sensor.x , (fabsf(_am_iphone_accel_data.sensor.z) + fabsf(_am_iphone_accel_data.sensor.y)));
		rot_z = (Angle32)(_am_iphone_accel_data.sensor.x * (float)0x4000);
		if (rot_z < 0x800 && rot_z > -0x800) {
			rot_z = 0;
		}
		else {
			rot_z *= 3;
			if (rot_z > 0x8000) {
				rot_z = 0x8000;
			}
			else if (rot_z < -0x8000) {
				rot_z = -0x8000;
			}
		}
	}
#else
	{
		rot_z = nnArcTan2(_am_iphone_accel_data.sensor.x , -_am_iphone_accel_data.sensor.z);
		if (rot_z < 0x800 && rot_z > -0x800) {
			rot_z = 0;
		}
		else {
			rot_z *= 3;
			if (rot_z > 0x8000) {
				rot_z = 0x8000;
			}
			else if (rot_z < -0x8000) {
				rot_z = -0x8000;
			}
		}
	}
#endif // 01
	
	return rot_z;
}



#if GMD_PLY_SAFE_TOUCH_SPIN
#define GMD_PLY_SAFE_TOUCH_TIMER  (25)
	
#define GMD_PLY_SAFE_SPIN_TIMER_TURNCHECK (3)
#define GMD_PLY_SAFE_SPIN_TIMER_TURNWAIT  (2)
#define GMD_PLY_SAFE_SPIN_TIMER_PULLWAIT  (1)
#endif // GMD_PLY_SAFE_TOUCH_SPIN

// ================================================================
// gmPlayerRemapKeyIPhone
/*!
 タッチの入力状態からキーをマッピングする
 
 @param    ply_work  [in] 対象プレイヤー
 
 @return   マッピング後のキー
 
 @note
 ステージ、プレイヤーパラメータにより設定されるキーは変更されます。
 */
// ================================================================
static u16 gmPlayerRemapKeyIPhone(GMS_PLAYER_WORK* ply_work)
{
	u16 new_key = 0;
#if defined (MTD_DEBUG)
	if (AoPadDirect() & KEY_SELECT) {
		return new_key;
	}
	if (AoPadDirect() & KEY_R2) {
		return new_key;
	}
#endif // MTD_DEBUG
	// ポーズ判定
	if (GmMainKeyCheckPauseKeyPush() != -1) {
		return new_key;
	}
	GME_PLY_SEQ_STATE seq_state = ply_work->seq_state;
#if GMD_PLY_SAFE_TOUCH_SPIN
	if (ply_work->safe_timer > 0) {
		ply_work->safe_timer--;
		if (amTpIsTouchPull(0)) {
			ply_work->safe_timer = 0;
			ply_work->safe_jump_timer = 10;
		}
		else if (ply_work->safe_timer == 0) {
			ply_work->safe_spin_timer = GMD_PLY_SAFE_SPIN_TIMER_TURNCHECK;
		}
	}
	else if (ply_work->safe_jump_timer > 0) {
		ply_work->safe_jump_timer--;
		new_key |= KEY_R_DOWN;
	}
	else if (ply_work->safe_spin_timer) {
		u16 turn_key = 0;
		u32 disp_flag = (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP);
		if (amTpIsTouchPull(0)) {
			// 発動
			ply_work->safe_spin_timer = 0;
		}
		switch(ply_work->safe_spin_timer) {
			case GMD_PLY_SAFE_SPIN_TIMER_TURNCHECK:
				if (ObjTouchCheck(&ply_work->obj_work, &gm_ply_touch_rect[GME_PLAYER_TOUCH_RECT_B])) {
					if (disp_flag) {
						turn_key = KEY_L_RIGHT; // 強制右入力
					}
					else {
						turn_key = KEY_L_LEFT; // 強制左入力
					}
					ply_work->safe_spin_timer = GMD_PLY_SAFE_SPIN_TIMER_TURNWAIT;
				}
				else {
					turn_key = KEY_L_DOWN;
					ply_work->safe_spin_timer = GMD_PLY_SAFE_SPIN_TIMER_PULLWAIT;
				}
				new_key |= turn_key;
				break;
				
			case GMD_PLY_SAFE_SPIN_TIMER_TURNWAIT:
				if (seq_state != GME_PLY_SEQ_STATE_TURN) {
					ply_work->safe_spin_timer = GMD_PLY_SAFE_SPIN_TIMER_PULLWAIT;
				}
				new_key |= KEY_L_DOWN;
				break;
				
			case GMD_PLY_SAFE_SPIN_TIMER_PULLWAIT:
				new_key |= KEY_L_DOWN;
				if (!amTpIsTouchPull(0)) {
					// しゃがみ
					if (seq_state == GME_PLY_SEQ_STATE_SQUAT_ST
						|| seq_state == GME_PLY_SEQ_STATE_SQUAT_M
						|| seq_state == GME_PLY_SEQ_STATE_SQUAT_END) {
						new_key |= KEY_R_DOWN;
					}
				}
				break;
				
			default:
				break;
		}
	}
	else 
#endif // GMD_PLY_SAFE_TOUCH_SPIN
	{
		BOOL touch_on   = amTpIsTouchOn(0);
		BOOL touch_push = amTpIsTouchPush(0);
#if 1
		// スーパーソニックキー判定
		if (GmPlayerIsTransformSuperSonic(ply_work)) {
			// 入力あり
			if (touch_on) {
				// 座標判定
				if (GMM_PLAYER_IS_TOUCH_SUPER_SONIC_REGION(_am_tp_touch[0].on[AMD_X], _am_tp_touch[0].on[AMD_Y])) {
					new_key |= PAD_BUTTON_TRANSFORM;
				}
			}
		}
#endif // 1
		if (!(new_key & PAD_BUTTON_TRANSFORM)) {
			// ソニックタッチによるスピン用入力作成
			u16 turn_key = 0;
			u32 disp_flag = (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP);
			u16 touch = ObjTouchCheck(&ply_work->obj_work, &gm_ply_touch_rect[GME_PLAYER_TOUCH_RECT_F]);
			if (!touch) {
				if (ObjTouchCheck(&ply_work->obj_work, &gm_ply_touch_rect[GME_PLAYER_TOUCH_RECT_B])) {
					touch = 1;
					if (disp_flag) {
						turn_key = KEY_L_RIGHT; // 強制右入力
					}
					else {
						turn_key = KEY_L_LEFT; // 強制左入力
					}
				}
			}
			
			// タッチ判定による処理
			if (touch) {
#if GMD_PLY_SAFE_TOUCH_SPIN
				// セーフ領域
				if (_am_tp_touch[0].on[AMD_X] < 80 || _am_tp_touch[0].on[AMD_X] > 400) {
					ply_work->safe_timer = GMD_PLY_SAFE_TOUCH_TIMER;
				}
				else 
#endif // GMD_PLY_SAFE_TOUCH_SPIN
				// turnより
				if (seq_state == GME_PLY_SEQ_STATE_TURN) {
					if (touch_push | touch_on) {
	//					ply_work->spin_state = GMD_PLY_SPIN_STATE_TURN;
						new_key |= KEY_L_DOWN;
					}
				}
				// spin待機
				else if (seq_state == GME_PLY_SEQ_STATE_SPIN_DASH
					|| seq_state == GME_PLY_SEQ_STATE_SPIN_DASHACC
					) {
					if (touch_push | touch_on) {
						new_key |= KEY_L_DOWN;// | KEY_R_DOWN;
					}
				}
				// たったまま/brake から
				else if (seq_state == GME_PLY_SEQ_STATE_FW
					|| seq_state == GME_PLY_SEQ_STATE_BRAKE
					) {
					// ターン優先
					if (turn_key) {
						new_key |= turn_key;
					}
					else {
						if (touch_push || touch_on) {
	//						ply_work->spin_state = GMD_PLY_SPIN_STATE_NORMAL;
							new_key |= KEY_L_DOWN;
						}
					}
				}
				// しゃがみ
				else if (seq_state == GME_PLY_SEQ_STATE_SQUAT_ST
					|| seq_state == GME_PLY_SEQ_STATE_SQUAT_M
					|| seq_state == GME_PLY_SEQ_STATE_SQUAT_END) {
					if (touch_push || touch_on) {
	//					ply_work->spin_state = GMD_PLY_SPIN_STATE_NORMAL;
						new_key |= KEY_L_DOWN | KEY_R_DOWN;
					}
				}
				// walk(fast)より
				else if (seq_state == GME_PLY_SEQ_STATE_WALK || ply_work->spin_state == GMD_PLY_SPIN_STATE_WALK) {
					if (touch_push || touch_on) {
						ply_work->spin_state = GMD_PLY_SPIN_STATE_WALK;
						new_key |= KEY_L_DOWN;
					}
				}
				// other
				else {
					if (touch_push || touch_on) {
						ply_work->spin_state = GMD_PLY_SPIN_STATE_NON;
						new_key |= KEY_R_DOWN;
					}
				}
			}
			// 通常タッチ
			else {
				ply_work->spin_state = GMD_PLY_SPIN_STATE_NON;
				if (touch_push || touch_on) {
					new_key |= KEY_R_DOWN;
				}
			}
		}
	}
	
	return new_key;
}
	
// ================================================================
// gmPlayerRemapKeyIPhoneZone32SS
/*!
	タッチの入力状態からキーをマッピングする Zone3-2 SS only
	
	@param    ply_work  [in] 対象プレイヤー
	
	@return   マッピング後のキー
	
	@note
	ステージ、プレイヤーパラメータにより設定されるキーは変更されます。
 */
// ================================================================
static u16 gmPlayerRemapKeyIPhoneZone32SS(GMS_PLAYER_WORK* ply_work)
{
	u16 new_key = 0;
	
	// ポーズ判定
	if (GmMainKeyCheckPauseKeyPush() != -1) {
		return new_key;
	}
	
	// 入力チェック
	for (int i = 0; i < AMD_TP_TOUCH_POS_MAX; i++) {
		BOOL touch_on     = amTpIsTouchOn(i);
		BOOL touch_push   = amTpIsTouchPush(i);
		
#if 1
		// スーパーソニックキー判定
		if (GmPlayerIsTransformSuperSonic(ply_work)) {
			// 入力あり
			if (touch_on) {
				// 座標判定
				if (GMM_PLAYER_IS_TOUCH_SUPER_SONIC_REGION(_am_tp_touch[i].on[AMD_X], _am_tp_touch[i].on[AMD_Y])) {
					new_key |= PAD_BUTTON_TRANSFORM;
				}
			}
		}
#endif // 1
		if (!(new_key & PAD_BUTTON_TRANSFORM)) {
			if (touch_push || touch_on) {
				new_key |= KEY_R_DOWN;
			}
		}
		if (new_key) {
			break;
		}
	}
	
	return new_key;
}
	

// ==========================================================================
// gmPlayerIsInputDPadJumpKey
/*!
 *	プレイヤーがDPadでのジャンプするキーを入力されたかどうか
 *
 *	@param    player  [in] 対象プレイヤー
 *	@param    ignore_input [in] 無視する入力(他入力競合対策)
 *	
 *	@return	TRUE : 入力された
 *
 *	@note	操作モードと座標から入力判定を返すだけの処理になる。
 *
 */
// ==========================================================================
BOOL gmPlayerIsInputDPadJumpKey(GMS_PLAYER_WORK *ply_work, s32 ignore_key)
{
	GME_PLAYER_CONTROL_TYPE type = ply_work->control_type;
	
	if (type == GME_PLAYER_CONTROL_TYPE_TILT) {
		// 傾斜操作なので終了
		return FALSE;
	}
	
	if (!ply_work->jump_rect) {
#ifdef MTD_DEBUG
		MTM_ASSERT(0);
		return FALSE;
#endif // MTD_DEBUG
	}
	
	BOOL flag = FALSE;
	// 判定
	for (int i = 0; i < AMD_TP_TOUCH_POS_MAX; i++) {
		if (i == ignore_key && !amTpIsTouchPush(i)) {
			// 無視キー対策
			// SS用に押した瞬間は無視を働かせないようにする
			continue;
		}
		if (amTpIsTouchOn(i)) {
			s16 x = _am_tp_touch[i].on[AMD_X];
			s16 y = _am_tp_touch[i].on[AMD_Y];
			if (ply_work->jump_rect[MTD_LEFT] <= x && x <= ply_work->jump_rect[MTD_RIGHT]
				&& ply_work->jump_rect[MTD_TOP] <= y && y <= ply_work->jump_rect[MTD_BOTTOM]
				) {
				flag = TRUE;
				break;
			}
		}
	}
	
	return flag;
}

// ==========================================================================
// gmPlayerIsInputDPadSSonicKey
/*!
 *	プレイヤーがDPadでのスーパーソニックキーを入力されたかどうか
 *
 *	@param    player  [in] 対象プレイヤー
 *	@param    ignore_input [in] 無視する入力(他入力競合対策)
 *	
 *	@return	TRUE : 入力された
 *
 *	@note	操作モードと座標から入力判定を返すだけの処理になる。
 *
 */
// ==========================================================================
BOOL gmPlayerIsInputDPadSSonicKey(GMS_PLAYER_WORK *ply_work, s32 ignore_key)
{
	GME_PLAYER_CONTROL_TYPE type = ply_work->control_type;
	
	if (type == GME_PLAYER_CONTROL_TYPE_TILT) {
		// 傾斜操作なので終了
		return FALSE;
	}
	
	if (!ply_work->ssonic_rect) {
#ifdef MTD_DEBUG
		MTM_ASSERT(0);
		return FALSE;
#endif // MTD_DEBUG
	}
	
	BOOL flag = FALSE;
	// 判定
	for (int i = 0; i < AMD_TP_TOUCH_POS_MAX; i++) {
		if (i == ignore_key && !amTpIsTouchPush(i)) {
			// 無視キー対策
			// SS用に押した瞬間は無視を働かせないようにする
			continue;
		}
		if (amTpIsTouchOn(i)) {
			s16 x = _am_tp_touch[i].on[AMD_X];
			s16 y = _am_tp_touch[i].on[AMD_Y];
			if (ply_work->ssonic_rect[MTD_LEFT] <= x && x <= ply_work->ssonic_rect[MTD_RIGHT]
				&& ply_work->ssonic_rect[MTD_TOP] <= y && y <= ply_work->ssonic_rect[MTD_BOTTOM]
				) {
				flag = TRUE;
				break;
			}
		}
	}
	
	return flag;
}

#endif


// =====================================================================
// カメラ
// =====================================================================
// ================================================================
// gmPlayerCameraOffset
/*!
  プレイヤーカメラズレ設定
  @param ply_work [io] プレイヤーワークポインタ
 */
// ================================================================
void gmPlayerCameraOffset(GMS_PLAYER_WORK *ply_work)
{
//	fx32 fTempX,fTempY;
    s16 sOfstX,sOfstY;
    u8 ucShift = 4;		// カメラ移動速度設定用シフト値

    if ( ply_work->player_id ) {
		// 自プレイヤー以外は処理を行わない
        return;
	}

	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		// スペステではプレイヤー側でカメラシフト制御しない
		return;
	}

	if (!ply_work->gmk_obj) {
	// ギミックが存在しない
		// ギミック用オフセット設定クリア
		ply_work->gmk_flag &= ~GMD_PLGF_CAMERA_CENTER_OFST;
		ply_work->gmk_camera_gmk_center_ofst_x = 0;
		ply_work->gmk_camera_gmk_center_ofst_y = 0;
	}

	sOfstX = 0;	//OBD_LCD_X/2;
	sOfstY = 0;	//OBD_LCD_Y/2;
#if 0
    else if (ply_work->gmk_flag & GMD_PLGF_CAMERA_CENTER_OFST) {
    	sOfstX += ply_work->gmk_camera_gmk_center_ofst_x;
    	sOfstY += ply_work->gmk_camera_gmk_center_ofst_y;
    }
#endif

#if 0//プレイヤー移動に対するカメラ遅延forSRA(DS)
	    // カメラ停止タイマーチェック
	    if ( ply_work->camera_stop_timer ) {
	        --ply_work->camera_stop_timer;
	        // プレイヤーの移動量分オフセット設定する
	        fTempX = ply_work->camera_ofst_tag_x;
	        fTempY = ply_work->camera_ofst_tag_y;
	#if 0
	        fTempX += ply_work->obj.move.x;
	        fTempY += ply_work->obj.move.y;

	        ucShift = 6;
	#else
	        fTempX += ply_work->obj_work.move.x << 4;
	        fTempY += ply_work->obj_work.move.y << 4;// ◆

	        ucShift = 2;
	#endif
	    }
		else {
	        fx32 fFlowX = 0,fFlowY = 0;
	        if ( ply_work->obj_work.ride_obj ) {
	            // 乗り物の移動分は無視する（気持ち悪かったので）
	            fFlowX = ply_work->obj_work.ride_obj->move.x;
	            fFlowY = ply_work->obj_work.ride_obj->move.y;
	        }
	        // 目的地設定
	#if 0
	        fTempX = ((ply_work->obj.prev_pos.x - (ply_work->obj.pos.x - fFlowX)));
	        fTempY = ((ply_work->obj.prev_pos.y - (ply_work->obj.pos.y - fFlowY)) >> 1);
	#else
	        fTempX = ((ply_work->obj_work.prev_pos.x - (ply_work->obj_work.pos.x - fFlowX)) << 4);
	        fTempY = ((ply_work->obj_work.prev_pos.y - (ply_work->obj_work.pos.y - fFlowY)) << 3);
	#endif

	        // 一定値以下は０
	        if ( MTM_MATH_ABS(fTempX) < GMD_PL_CAMERA_OFST_MINI_X ) {
	            fTempX = 0;
	        }
	        if ( MTM_MATH_ABS(fTempY) < GMD_PL_CAMERA_OFST_MINI_Y ) {
	            fTempY = 0;
	        }
	    }
	    if (ply_work->camera_stop_timer) {
	        // フローチェック
	        if ( fTempX > GMD_PL_CAMERA_OFST_MAX ) {
				 fTempX = GMD_PL_CAMERA_OFST_MAX;
			}
	        else if ( fTempX < -GMD_PL_CAMERA_OFST_MAX) {
				fTempX = -GMD_PL_CAMERA_OFST_MAX;
			}
	        if ( fTempY > GMD_PL_CAMERA_OFST_Y1_MAX ) {
				fTempY = GMD_PL_CAMERA_OFST_Y1_MAX;
			}
	        else if ( fTempY < GMD_PL_CAMERA_OFST_Y2_MAX) {
				fTempY = GMD_PL_CAMERA_OFST_Y2_MAX;
			}
	    }
		else {
	        // フローチェック
	        if ( fTempX > GMD_PL_CAMERA_OFST_MAX>>1 ) {
				fTempX = GMD_PL_CAMERA_OFST_MAX>>1;
			}
	        else if ( fTempX < -GMD_PL_CAMERA_OFST_MAX>>1 ) {
				fTempX = -GMD_PL_CAMERA_OFST_MAX>>1;
			}
	        if ( fTempY > GMD_PL_CAMERA_OFST_Y1_MAX>>1 ) {
				fTempY = GMD_PL_CAMERA_OFST_Y1_MAX>>1;
			}
	        else if ( fTempY < GMD_PL_CAMERA_OFST_Y2_MAX>>1 ) {
				fTempY = GMD_PL_CAMERA_OFST_Y2_MAX>>1;
			}
	    }
	    ply_work->camera_ofst_tag_x = fTempX;
	    ply_work->camera_ofst_tag_y = fTempY;
#endif//プレイヤー移動に対するカメラ遅延forSRA(DS)

    if ( ply_work->player_flag & GMD_PLF_NOCAMERA_OFST ) {
        // カメラずらしを無し設定へ移行
        ply_work->camera_ofst_x -= ply_work->camera_ofst_x >> 2;
        ply_work->camera_ofst_y -= ply_work->camera_ofst_y >> 2;
    }
	else {
#if 1
		if (ply_work->gmk_flag & GMD_PLGF_CAMERA_CENTER_OFST) {
	        // 移動
	        ply_work->camera_ofst_x += (ply_work->camera_ofst_tag_x - ply_work->camera_ofst_x +
										((ply_work->gmk_camera_center_ofst_x + ply_work->gmk_camera_gmk_center_ofst_x) << FX32_SHIFT)) >> ucShift;
	        ply_work->camera_ofst_y += (ply_work->camera_ofst_tag_y - ply_work->camera_ofst_y +
	        							((ply_work->gmk_camera_center_ofst_y + ply_work->gmk_camera_gmk_center_ofst_y) << FX32_SHIFT)) >> ucShift;
		}
		else {
	        // 移動
	        ply_work->camera_ofst_x += (ply_work->camera_ofst_tag_x - ply_work->camera_ofst_x + (ply_work->gmk_camera_center_ofst_x << FX32_SHIFT)) >> ucShift;
	        ply_work->camera_ofst_y += (ply_work->camera_ofst_tag_y - ply_work->camera_ofst_y + (ply_work->gmk_camera_center_ofst_y << FX32_SHIFT)) >> ucShift;
		}
#else
        // 移動
        ply_work->camera_ofst_x += (ply_work->camera_ofst_tag_x - ply_work->camera_ofst_x) >> ucShift;
        ply_work->camera_ofst_y += (ply_work->camera_ofst_tag_y - ply_work->camera_ofst_y) >> ucShift;
#endif

#if 0
        if ( ply_work->camera_jump_pos_y ){
            fTempY = (ply_work->obj.pos.y ) - ply_work->camera_jump_pos_y;
            if      ( fTempY > GMD_PL_CAMERA_OFST_Y1_MAX ) fTempY = GMD_PL_CAMERA_OFST_Y1_MAX;
            else if ( fTempY < GMD_PL_CAMERA_OFST_Y2_MAX ) fTempY = GMD_PL_CAMERA_OFST_Y2_MAX;
            ply_work->camera_ofst_y = fTempY;
        }
#endif
    }
    // 設定
//    GmMapSetPlayerLcdPos( (sOfstX << FX32_SHIFT) + ply_work->camera_ofst_x, (sOfstY << FX32_SHIFT) + ply_work->camera_ofst_y);
/*

	g_gm_map.camera[ge].player_lcd_pos_spd[MTD_X] = (sOfstX << FX32_SHIFT) + ply_work->camera_ofst_x;
	g_gm_map.camera[ge].player_lcd_pos_spd[MTD_Y] = (sOfstY << FX32_SHIFT) + ply_work->camera_ofst_y;

	// カメラプレイヤー固定中は、外部からの設定無視
	if (g_gm_map.camera[ge].flag & (GMD_MAP_CAM_FLAG_LOCK_PLAYER
								| GMD_MAP_CAM_FLAG_LOCKING_PLAYER))
		return;

	// 値を設定
	g_gm_map.camera[ge].player_lcd_pos[MTD_X] = g_gm_map.camera[ge].player_lcd_pos_spd[MTD_X];
	g_gm_map.camera[ge].player_lcd_pos[MTD_Y] = g_gm_map.camera[ge].player_lcd_pos_spd[MTD_Y];

*/
	// プレイヤーからの速度設定専用
	// 設定速度保存
	{
		OBS_CAMERA	*obj_camera;
		obj_camera = ObjCameraGet(0);
		obj_camera->ofst.x = FXM_FX32_TO_FLOAT((sOfstX << FX32_SHIFT) + ply_work->camera_ofst_x);
		obj_camera->ofst.y = FXM_FX32_TO_FLOAT((sOfstY << FX32_SHIFT) + ply_work->camera_ofst_y);
	}
}

// ==========================================================================
// 手 ロッド切り替え
// ==========================================================================
#if _WII
// ==========================================================================
// gmPlayerSetLodAction
/*!
 *	ロッドアクション設定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	act_id		[in]	アクションID
 */
// ==========================================================================
void gmPlayerSetLodAction(GMS_PLAYER_WORK *ply_work, s32 act_id)
{
	MTM_ASSERT(ply_work);
	MTM_ASSERT(act_id < ply_work->hand_lod_header->mtn_num);

	ply_work->hand_lod_mtn = ply_work->hand_lod_header->mtn_data + act_id;
	ply_work->hand_lod_pat = ply_work->hand_lod_mtn->pat_data;
	ply_work->hand_lod_pat_no = 0;
}


// ==========================================================================
// gmPlayerSetLodActionFrame
/*!
 *	ロッドアクションフレーム設定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	frame		[in]	設定フレーム
 */
// ==========================================================================
void gmPlayerSetLodActionFrame(GMS_PLAYER_WORK *ply_work, fx32 frame)
{
	u32				i;
	GMS_PLY_LOD_PAT	*pat_data;

	if (ply_work->hand_lod_pat->start_frame <= frame) {
		pat_data = ply_work->hand_lod_pat;
		i		 = ply_work->hand_lod_pat_no;
	}
	else {
		pat_data = ply_work->hand_lod_mtn->pat_data;
		i		 = 0;
	}

	for (; i < ply_work->hand_lod_mtn->pat_num; i++, pat_data++) {
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
	ply_work->hand_lod_pat		= pat_data;
	ply_work->hand_lod_pat_no	= i;
}


// ==========================================================================
// gmGmkPlayerMotionCallback
/*!
 *	プレイヤー モーションコールバック
 *
 *	@param	motion	[in]	モーション
 *	@param	object	[in]	オブジェクト
 *	@param	param	[in]	パラメータ	(ply_work)
 */
// ==========================================================================
void gmGmkPlayerMotionCallback(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param)
{
	GMS_PLAYER_WORK					*ply_work = (GMS_PLAYER_WORK*)param;
	OBS_ACTION3D_NN_WORK			*obj_3d = ply_work->obj_work.obj_3d;
	GMS_PLAYER_MAT_CALLBACK_PARAM	*ply_mat_cb_param;

	UNREFERENCED_PARAMETER(motion);
	UNREFERENCED_PARAMETER(object);

	// 手ロッドモーションUpdate
	gmPlayerSetLodActionFrame(ply_work, FXM_FLOAT_TO_FX32(obj_3d->frame[0]));

	// マテリアルコールバック設定
	ply_mat_cb_param = (GMS_PLAYER_MAT_CALLBACK_PARAM *)amDrawMallocDataBuffer(sizeof(GMS_PLAYER_MAT_CALLBACK_PARAM));
	ply_mat_cb_param->draw_id = ply_work->hand_lod_pat->user_data;
	obj_3d->material_cb_param = ply_mat_cb_param;
}

// ==========================================================================
// gmPlayerMaterialCallback
/*!
 *	Wii用 プレイヤーマテリアルコールバック
 *
 *	@param val				[in]	NNS_DRAWCALLBACK_VAL構造体へのポインタ
 *	@param param			[in]	ユーザーパラメータ(GMS_PLAYER_MAT_CALLBACK_PARAM)
 *
 *	@return		NNE_BOOL
 */
// ==========================================================================
NNE_BOOL gmPlayerMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	u32								user_data;
	GMS_PLAYER_MAT_CALLBACK_PARAM	*user_param;

	if (param) {
		user_param = (GMS_PLAYER_MAT_CALLBACK_PARAM*)param;

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


#endif // #if _WII


// ==========================================================================
// トロッコ用
// ==========================================================================
// ==========================================================================
// gmGmkPlayerMotionCallback
/*!
 *	プレイヤー モーションコールバック
 *
 *	@param	motion	[in]	モーション
 *	@param	object	[in]	オブジェクト
 *	@param	param	[in]	パラメータ	(ply_work)
 */
// ==========================================================================
#define GMD_PLAYER_NODE_ID_TRUCK_CENTER		(2)
void gmGmkPlayerMotionCallbackTruck(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param)
{
	/* Node 2 : Hips */
	NNS_MATRIX			node_mtx, base_mtx;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)param;

	// ベースマトリクス取得
	nnMakeUnitMatrix(&base_mtx);
	nnMultiplyMatrix(&base_mtx, &base_mtx, amMatrixGetCurrent());

	// 階層マトリクスを求める
	// truck_mtx_ply_mtn_pos
	nnCalcNodeMatrixTRSList(&node_mtx, object, 
			GMD_PLAYER_NODE_ID_TRUCK_CENTER, 
			motion->data, &base_mtx);
	MI_CpuCopy8(&node_mtx, &ply_work->truck_mtx_ply_mtn_pos, sizeof(NNS_MATRIX));

#if _WII
	// Wii用コールバック
	gmGmkPlayerMotionCallback(motion, object, param);
#endif
}


// ==========================================================================
// GmPlayerStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void GmPlayerStaticVarInit(void)
{
	/// プレイヤーデータ格納用データワーク
	memset(g_gm_player_data_work, 0, sizeof(g_gm_player_data_work));
	
	g_gm_ply_son_obj_3d_list = NULL;				//!< ソニックモデル
	g_gm_ply_sson_obj_3d_list = NULL;				//!< スーパーソニックモデル
	
	//----- Local Variables -----------------------------------------------------
	// 自分プレイヤー座標保持用(ゴースト記録等に使用)
	gm_pos_x = 0;	//[1:31:0]
	gm_pos_y = 0;	//[1:31:0]
	gm_pos_z = 0;	//[1:31:0]
	//memset(_nl_ghost, 0, sizeof(_nl_ghost)); // 6byte x 2700 = 16k 
	//memset(_nl_ghost_save, 0, sizeof(_nl_ghost_save)); // 6byte x 2700 = 16k 
	//_nl_ghost_check = 0; // GHOST記録ありなしチェック
	//gm_player_bgm_tcb = NULL; // 特殊BGM管理タスク
	
	//gm_player_data_load_tcb = NULL;	//!< データ読み込みTCB
	
	/// キーマップ対応キーリスト
	static const u32 sc_gm_key_map_key_list[GMD_PLAYER_KEY_MAP_MAX] = {
		KEY_L_UP, KEY_L_DOWN, KEY_L_LEFT, KEY_L_RIGHT,
		KEY_R_DOWN, KEY_R_RIGHT, KEY_R_LEFT, KEY_R_UP
	};
	memcpy(gm_key_map_key_list, sc_gm_key_map_key_list, sizeof(gm_key_map_key_list));
	
	/// プレイヤー使用モデルデータリスト
	static const OBS_ACTION3D_NN_WORK **sc_gm_ply_obj_3d_list_tbl[GSD_CHAR_ID_MAX][GMD_PLY_MODEL_SET_MAX] = {
		{	(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_son_obj_3d_list,
			(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_sson_obj_3d_list},		// ソニック
		{	(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_son_obj_3d_list,
			(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_sson_obj_3d_list},		// スーパーソニック
		{	(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_son_obj_3d_list,
			(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_sson_obj_3d_list},		// スペステソニック
		{	(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_son_obj_3d_list,
			(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_sson_obj_3d_list},		// ピンボールソニック
		{	(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_son_obj_3d_list,
			(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_sson_obj_3d_list},		// ピンボールスーパーソニック
		{	(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_son_obj_3d_list,
			(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_sson_obj_3d_list},		// トロッコソニック
		{	(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_son_obj_3d_list,
			(const OBS_ACTION3D_NN_WORK **)&g_gm_ply_sson_obj_3d_list},		// トロッコスーパーソニック
	};
	memcpy(gm_ply_obj_3d_list_tbl, sc_gm_ply_obj_3d_list_tbl, sizeof(gm_ply_obj_3d_list_tbl));
	
#ifdef _IPHONE
	//// プレイヤータッチ矩形
	memset(gm_ply_touch_rect, 0, sizeof(gm_ply_touch_rect));
	
	//// コントロールタイプ
	gm_player_control_type = GME_PLAYER_CONTROL_TYPE_TILT;
#endif // _IPHONE
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
