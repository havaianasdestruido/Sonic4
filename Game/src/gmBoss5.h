// =======================================================================
/*!
  @file	gmBoss5.h
  @brief ボスファイナル

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_5_H_
#define GM_BOSS_5_H_


#include "gsSound.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmBossCommon.h"

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/
//############ 切り替えマクロ #################################################
#define GMD_BOSS5_BOOL_USE_OBSOLETE_FUNCTION		(0)		//!< 0なら参照されていない関数を無効化

//############ 共通 ###########################################################
/* 定義値 */
#define GMD_BOSS5_DEFAULT_BLEND_SPD			((Float)0.125f)		//!< ボス５のデフォルトモーションブレンド速度

#define GMD_BOSS5_LIFE							(33)			//!< ライフ値
#define GMD_BOSS5_STRONG_MODE_THRESHOLD_LIFE	(17)			//!< ライフ値がこの値「以下」になったら凶暴モードへ移行

#define GMD_BOSS5_DEFAULT_POS_Z				(GMD_OBJ_DEFAULT_POS_Z_C)	//!< デフォルトZ位置
#define GMD_BOSS5_BG_FARSIDE_POS_Z			(GMD_OBJ_DEFAULT_POS_Z_B_BACK - (fx32)(FX32_ONE * 96))	//!< 近景の向こう側に隠れる位置（B面位置に加えて、さらに本体が十分に隠れる分だけ奥側に配置）

#define GMD_BOSS5_VIB_PHASE_NUM				(40)				//!< 汎用振動処理フェーズ数

//############ 管理 ###########################################################
/* フラグ GMS_BOSS5_MGR_WORK::flag */
#define GMD_BOSS5_MGR_FLAG_SETUP_END		(1 << 0)		//!< セットアップ完了フラグ
#define GMD_BOSS5_MGR_FLAG_CLEAR_BOSS		(1 << 1)		//!< ボスを消去する
#define GMD_BOSS5_MGR_FLAG_REQUEST_STRONG_MODE	(1 << 2)	//!< 凶暴モード移行リクエスト
#define GMD_BOSS5_MGR_FLAG_STRONG_MODE		(1 << 3)		//!< 凶暴モード有効中フラグ
#define GMD_BOSS5_MGR_FLAG_CAMERA_OFST_SAVED	(1 << 4)	//!< カメラオフセット保存済みフラグ
#define GMD_BOSS5_MGR_FLAG_ALARM_FADE_ACTIVE	(1 << 5)	//!< 警告フェード有効中フラグ（このフラグが立っていたら警告フェードオブジェクトを生成しない, オフにすると警告フェード消去）
// ↓シーケンス同期関連
#define GMD_BOSS5_MGR_FLAG_CAMERA_SLIDE_NEEDED	(1 << 19)	//!< Egg -> Body 4:3画面用カメラスライド要求フラグ
#define GMD_BOSS5_MGR_FLAG_CAMERA_SLIDE_STARTED	(1 << 20)	//!< Body -> Body 4:3画面用カメラスライド開始済みフラグ
#define GMD_BOSS5_MGR_FLAG_BODY_STOOD_BY	(1 << 21)		//!< Body -> Mgr 本体配置完了フラグ
#define GMD_BOSS5_MGR_FLAG_PLY_PASSED_TRG	(1 << 22)		//!< Trg -> Mgr プレイヤーがトリガギミック通過した
#define GMD_BOSS5_MGR_FLAG_CTPLT_STORE_NEEDED	(1 << 23)	//!< Body -> Ctplt カタパルト収納要求フラグ
#define GMD_BOSS5_MGR_FLAG_CTPLT_IS_STORED	(1 << 24)		//!< Ctplt -> Body カタパルト収納完了フラグ
#define GMD_BOSS5_MGR_FLAG_BODY_DEMO_IS_FINISHED	(1 << 25)	//!< Body -> Mgr,Ctplt 本体のデモ処理完了フラグ
#define GMD_BOSS5_MGR_FLAG_LAND_NEEDED		(1 << 26)		//!< Body -> Mgr 足場要求フラグ
#define GMD_BOSS5_MGR_FLAG_BODY_DEFEAT		(1 << 27)		//!< Body -> Mgr 本体撃破フラグ（立ったら、終了デモを開始する）
#define GMD_BOSS5_MGR_FLAG_CHASING_EXPL_NEEDED	(1 << 28)	//!< Body -> Mgr プレイヤー追いかけ爆発要求フラグ（立ったら、追いかけ爆発生成を更新する）
#define GMD_BOSS5_MGR_FLAG_LAND_SHAKE_NEEDED	(1 << 29)	//!< Body -> Land 足場振動要求フラグ
#define GMD_BOSS5_MGR_FLAG_LAND_BREAK_NEEDED	(1 << 30)	//!< Body -> Land 足場崩壊要求フラグ
#define GMD_BOSS5_MGR_FLAG_LAST_FADE_END		(1 << 31)	//!< LastFadeOut -> Mgr フェードアウト完了フラグ

//############ 本体 ###########################################################
/* フラグ GMS_BOSS5_BODY_WORK::flag */
#define GMD_BOSS5_BODY_FLAG_INVINCIBLE		(1 << 0)			//!< 無敵状態（当たりはあるが、ライフは減らない）
#define GMD_BOSS5_BODY_FLAG_TARGET_ON		(1 << 1)			//!< ターゲットカーソル表示中
#define GMD_BOSS5_BODY_FLAG_HIDE_ARM_L		(1 << 2)			//!< 左腕非表示
#define GMD_BOSS5_BODY_FLAG_HIDE_ARM_R		(1 << 3)			//!< 右腕非表示
#define GMD_BOSS5_BODY_FLAG_USE_RKT_OFST	(1 << 4)			//!< ロケット配置オフセットマトリクス使用フラグ
#define GMD_BOSS5_BODY_FLAG_NO_HIT_TIME_COUNTING	(1 << 5)	//!< 食らい無効タイマカウント中
#define GMD_BOSS5_BODY_FLAG_IS_POKING		(1 << 6)			//!< 腕突き出し処理中フラグ
#define GMD_BOSS5_BODY_FLAG_POKE_TRG_TIME_LMT_COUNTING	(1 << 7)	//!< 腕突き出し発動期限タイマカウント中
#define GMD_BOSS5_BODY_FLAG_RKT_CNCT_ATK_ACTIVE	(1 << 8)		//!< 本体接続型ロケットの攻撃当たりを有効化
#define GMD_BOSS5_BODY_FLAG_HOLD_VULCAN		(1 << 9)			//!< バルカン発射禁止
#define GMD_BOSS5_BODY_FLAG_TURRET_ACTIVE	(1 << 10)			//!< バルカン有効（生成済み）フラグ
#define GMD_BOSS5_BODY_FLAG_LEAKAGE_ACTIVE	(1 << 11)			//!< 漏電エフェクト有効中フラグ（オフにするとエフェクトが消える。直接書き換え禁止）
#define GMD_BOSS5_BODY_FLAG_LEAKAGE_NEEDED	(1 << 12)			//!< 漏電エフェクト必要状態フラグ（オンにすると漏電エフェクトが生成され、オフにすると消去される）
#define GMD_BOSS5_BODY_FLAG_DENY_HOMING_ATK	(1 << 13)			//!< このフラグが立っているとホーミングの対象外になる
#define GMD_BOSS5_BODY_FLAG_JET_ACTIVE		(1 << 14)			//!< 噴射エフェクト有効中フラグ（オフにするとエフェクトが消える。直接書き換え禁止）
#define GMD_BOSS5_BODY_FLAG_JETSMOKE_ACTIVE	(1 << 15)			//!< 噴射スモークエフェクト有効中フラグ（オフにするとエフェクトが消える。直接書き換え禁止）
#define GMD_BOSS5_BODY_FLAG_FINAL_MODE		(1 << 16)			//!< 最後の一撃モードフラグ
#define GMD_BOSS5_BODY_FLAG_CRASH_SUCCESS	(1 << 17)			//!< 地球割り成功フラグ
#define GMD_BOSS5_BODY_FLAG_RKT_SCATTER_NEEDED	(1 << 18)		//!< ロケットパンチ飛散要求（オンにするとロケットパンチパーツが飛散動作を開始する）
#define GMD_BOSS5_BODY_FLAG_SCT_FALL_ACTIVE	(1 << 19)			//!< パーツ飛散処理時 本体落下有効中フラグ
#define GMD_BOSS5_BODY_FLAG_PRELIM_LEAKAGE_ACTIVE	(1 << 20)	//!< 予備漏電エフェクト有孔虫フラグ（オフにするとエフェクトが消える。直接書き換え禁止）
#define GMD_BOSS5_BODY_FLAG_LAST_BIG_EXPL_CREATED	(1 << 21)	//!< パーツ飛散後、本体落下着地時の大爆発エフェクト生成済みフラグ
// シグナル系
#define GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_DEFEAT		(1 << 22)	//!< body -> body 撃破演出開始通知
#define GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_POKE			(1 << 23)	//!< body -> body 腕突き出し攻撃要求通知
#define GMD_BOSS5_BODY_FLAG_SIGNAL_E2B_EGG_GOT_IN	(1 << 24)	//!< egg -> body エッグマン搭乗完了通知
#define GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_R	(1 << 25)	//!< rocket -> body 右ロケット帰還完了通知
#define GMD_BOSS5_BODY_FLAG_SIGNAL_R2B_RETURNED_L	(1 << 26)	//!< rocket -> body 左ロケット帰還完了通知
#define GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_LAUNCH_R		(1 << 27)	//!< body -> rocket 右ロケット発射要求通知
#define GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_LAUNCH_L		(1 << 28)	//!< body -> rocket 左ロケット発射要求通知
#define GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_RETURN_R		(1 << 29)	//!< body -> rocket 右ロケット帰還要求通知
#define GMD_BOSS5_BODY_FLAG_SIGNAL_B2R_RETURN_L		(1 << 30)	//!< body -> rocket 左ロケット帰還要求通知
#define GMD_BOSS5_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE	(1 << 31)	//!< body -> body ダメージ通知フラグ

/* 定義値 */
#define GMD_BOSS5_BODY_PLY_SEARCH_HIST_NUM			(11)

// 地球割り関連
#define GMD_BOSS5_BODY_CRASH_POINT_A_POS_OFST_X	((fx32)(FX32_ONE * -256))	//!< 地球割り・落下目標A地点のオフセット座標（スクロールロックエリアの水平方向中心基準）
#define GMD_BOSS5_BODY_CRASH_POINT_B_POS_OFST_X	((fx32)(FX32_ONE * 0))		//!< 地球割り・落下目標B地点のオフセット座標（スクロールロックエリアの水平方向中心基準）
#define GMD_BOSS5_BODY_CRASH_POINT_C_POS_OFST_X	((fx32)(FX32_ONE * 256))	//!< 地球割り・落下目標C地点のオフセット座標（スクロールロックエリアの水平方向中心基準）

//############ パーツ飛散 #####################################################
/* 定義値 */
#define GMD_BOSS5_SCT_SPIN_AXIS_NUM				(2)			//!< 飛散パーツを回転させる軸の数
#define GMD_BOSS5_SCT_PART_FLY_DELETE_TIME		(180)		//!< 消去タイマ時間
#define GMD_BOSS5_SCT_SPIN_SPD_ANGLE			(AKM_DEGtoA32(3.f))		//!< 飛散パーツ１軸分の回転速度

#define GMD_BOSS5_SCT_ARM_FLY_DELAY_LEFT		(10)		//!< 左腕飛散開始遅延時間（本体側のパーツとロケットパンチの飛散タイミングを合わせるために定義）
#define GMD_BOSS5_SCT_ARM_FLY_DELAY_RIGHT		(30)		//!< 右腕飛散開始遅延時間（本体側のパーツとロケットパンチの飛散タイミングを合わせるために定義）
#define GMD_BOSS5_SCT_LEG_FLY_DELAY_LEFT		(80)		//!< 左脚飛散開始遅延時間（脚パーツの飛散タイミングと本体の落下タイミングを合わせるために定義）
#define GMD_BOSS5_SCT_LEG_FLY_DELAY_RIGHT		(90)		//!< 右脚飛散開始遅延時間（脚パーツの飛散タイミングと本体の落下タイミングを合わせるために定義）

/*------ Macro Functions -----------------------------------------------*/

// =======================================================================
// GMM_BOSS5_AREA_***
/*!
  スクロール可能範囲矩形の座標を取得
  
  @return スクロール可能範囲矩形座標（マップ座標系, 固定小数）
  
  @note
  スクロールロックされた際の上下左右端の座標を得るのに使用します。
 */
// =======================================================================
#define GMM_BOSS5_AREA_LEFT()	((fx32)(g_gm_main_system.map_fcol.left << FX32_SHIFT))
#define GMM_BOSS5_AREA_TOP()	((fx32)(g_gm_main_system.map_fcol.top << FX32_SHIFT))
#define GMM_BOSS5_AREA_RIGHT()	((fx32)(g_gm_main_system.map_fcol.right << FX32_SHIFT))
#define GMM_BOSS5_AREA_BOTTOM()	((fx32)(g_gm_main_system.map_fcol.bottom << FX32_SHIFT))
// 画面中心
#define GMM_BOSS5_AREA_CENTER_X()	(GMM_BOSS5_AREA_LEFT() + ((GMM_BOSS5_AREA_RIGHT() - GMM_BOSS5_AREA_LEFT()) / 2))
#define GMM_BOSS5_AREA_CENTER_Y()	(GMM_BOSS5_AREA_TOP() + ((GMM_BOSS5_AREA_BOTTOM() - GMM_BOSS5_AREA_TOP()) / 2))

/*------ Definitions ---------------------------------------------------*/

//! ボスFINAL 本体 ステート列挙型
typedef enum
{
	GME_BOSS5_BODY_STATE_NOP	= 0,
	GME_BOSS5_BODY_STATE_START,
	
	GME_BOSS5_BODY_STATE_MOVE_NML,
	GME_BOSS5_BODY_STATE_MOVE_FAST_FWD,
	GME_BOSS5_BODY_STATE_MOVE_FAST_BWD,	// MOVE_FAST_FWDステート中のMOVE_FAST_CRASHサブシーケンスからのみ遷移
	
	GME_BOSS5_BODY_STATE_STOMP_NML,
	GME_BOSS5_BODY_STATE_STOMP_STR,
	GME_BOSS5_BODY_STATE_CRASH,	// 地球割り
	
	GME_BOSS5_BODY_STATE_RPUNCH_NML,
	GME_BOSS5_BODY_STATE_RPUNCH_STR,
	
	GME_BOSS5_BODY_STATE_BERSERK,	// 凶暴化演出
	
	GME_BOSS5_BODY_STATE_DEFEAT,	// 撃破演出
	
	GME_BOSS5_BODY_STATE_MAX
} GME_BOSS5_BODY_STATE;

//! ボスFINAL 本体 サブシーケンス列挙型
typedef enum
{
	GME_BOSS5_BODY_SUB_SEQ_MOVE_FAST_FWD_CRASH	= 0,	// 高速前方移動時 壁衝突
	GME_BOSS5_BODY_SUB_SEQ_MOVE_FAST_BWD_CRASH,			// 高速後方移動時 壁衝突
	GME_BOSS5_BODY_SUB_SEQ_RPUNCH_STR_DMG,				// 強ロケットパンチ時よろけ
	
	GME_BOSS5_BODY_SUB_SEQ_MAX,
	GME_BOSS5_BODY_SUB_SEQ_NONE	= -1
} GME_BOSS5_BODY_SUB_SEQ;

//! ボスFINAL 戦略ステート
typedef enum
{
	GME_BOSS5_STRAT_STATE_NONE	= 0,
	
	GME_BOSS5_STRAT_STATE_START,
	GME_BOSS5_STRAT_STATE_NML_MOVE_A,	// 最初
	GME_BOSS5_STRAT_STATE_NML_MOVE_B,	// 確率A後
	GME_BOSS5_STRAT_STATE_NML_MOVE_C,	// 確率B後
	GME_BOSS5_STRAT_STATE_NML_STOMP_A,	// 通常初回前後移動後
	GME_BOSS5_STRAT_STATE_NML_STOMP_B,	// 確率A後
	GME_BOSS5_STRAT_STATE_NML_RPUNCH,	// 確率A後
	
	GME_BOSS5_STRAT_STATE_BERSERK,		// 凶暴化演出
	GME_BOSS5_STRAT_STATE_STR_MOVE_A,	// 最初
	GME_BOSS5_STRAT_STATE_STR_MOVE_B,	// 確率A後
	GME_BOSS5_STRAT_STATE_STR_MOVE_C,	// 確率B後
	GME_BOSS5_STRAT_STATE_STR_STOMP_A,	// 凶暴初回前後移動後
	GME_BOSS5_STRAT_STATE_STR_STOMP_B,	// 確率A後
	GME_BOSS5_STRAT_STATE_STR_RPUNCH_A,	// 確率A後
	GME_BOSS5_STRAT_STATE_STR_RPUNCH_B,	// 確率C後
	
	GME_BOSS5_STRAT_STATE_CRASH,		// 地球割り
	
	GME_BOSS5_STRAT_STATE_MAX
} GME_BOSS5_STRAT_STATE;


//! ボス５パーツインデックス列挙型
typedef enum
{
	GME_BOSS5_PART_IDX_BODY	= 0,
	
	GME_BOSS5_PART_IDX_MAX
} GME_BOSS5_PART_IDX;

//! ボス５ アクションID列挙型
typedef enum
{
	GME_BOSS5_ACT_ID_START	= 0,			//!< 開始（地面埋まり～せり上がり）
	GME_BOSS5_ACT_ID_FW,					//!< フットワーク（汎用）
	GME_BOSS5_ACT_ID_MOVE_NML,				//!< 通常歩き
	GME_BOSS5_ACT_ID_MOVE_NML_ABORT,		//!< 通常歩き 中断
	GME_BOSS5_ACT_ID_MOVE_FAST_FWD_PREP,	//!< 前進走り 準備
	GME_BOSS5_ACT_ID_MOVE_FAST_FWD_R_JUMP,	//!< 前進走り 右ジャンプ
	GME_BOSS5_ACT_ID_MOVE_FAST_FWD_R_AIR,	//!< 前進走り 右滞空
	GME_BOSS5_ACT_ID_MOVE_FAST_FWD_R_LAND,	//!< 前進走り 右着地
	GME_BOSS5_ACT_ID_MOVE_FAST_FWD_L_JUMP,	//!< 前進走り 左ジャンプ
	GME_BOSS5_ACT_ID_MOVE_FAST_FWD_L_AIR,	//!< 前進走り 左滞空
	GME_BOSS5_ACT_ID_MOVE_FAST_FWD_L_LAND,	//!< 前進走り 左着地
	GME_BOSS5_ACT_ID_MOVE_FAST_BWD_PREP,	//!< 後退走り 準備
	GME_BOSS5_ACT_ID_MOVE_FAST_BWD_R_JUMP,	//!< 後退走り 右ジャンプ
	GME_BOSS5_ACT_ID_MOVE_FAST_BWD_R_AIR,	//!< 後退走り 右滞空
	GME_BOSS5_ACT_ID_MOVE_FAST_BWD_R_LAND,	//!< 後退走り 右着地
	GME_BOSS5_ACT_ID_MOVE_FAST_BWD_L_JUMP,	//!< 後退走り 左ジャンプ
	GME_BOSS5_ACT_ID_MOVE_FAST_BWD_L_AIR,	//!< 後退走り 左滞空
	GME_BOSS5_ACT_ID_MOVE_FAST_BWD_L_LAND,	//!< 後退走り 左着地
	GME_BOSS5_ACT_ID_MOVE_FAST_FWD_CRASH,	//!< 前進走り 壁衝突
	GME_BOSS5_ACT_ID_MOVE_FAST_BWD_CRASH,	//!< 後退走り 壁衝突
	GME_BOSS5_ACT_ID_MOVE_FAST_RECOVER,		//!< 走り共通 復帰
	GME_BOSS5_ACT_ID_STOMP_IGNITE,			//!< エンジン点火
	GME_BOSS5_ACT_ID_STOMP_HOVER,			//!< ちょっと浮く
	GME_BOSS5_ACT_ID_STOMP_FLYUP,			//!< 上昇
	GME_BOSS5_ACT_ID_STOMP_FALL,			//!< 落下
	GME_BOSS5_ACT_ID_STOMP_LAND,			//!< 着地
	GME_BOSS5_ACT_ID_CRASH_PREP,			//!< 地球割り 準備
	GME_BOSS5_ACT_ID_CRASH_HOVER,			//!< 地球割り ジャンプ開始
	GME_BOSS5_ACT_ID_CRASH_FLYUP,			//!< 地球割り 上昇
	GME_BOSS5_ACT_ID_CRASH_FALL,			//!< 地球割り 落下
	GME_BOSS5_ACT_ID_CRASH_LAND,			//!< 地球割り 着地・突き
	GME_BOSS5_ACT_ID_CRASH_SINK,			//!< 地球割り 沈下
	GME_BOSS5_ACT_ID_RPUNCH_PREP,			//!< ロケットパンチ 準備
	GME_BOSS5_ACT_ID_RPUNCH_LAUNCH_FIRST,	//!< ロケットパンチ 一発目
	GME_BOSS5_ACT_ID_RPUNCH_LAUNCH_SECOND,	//!< ロケットパンチ 二発目
	GME_BOSS5_ACT_ID_RPUNCH_RETURN,			//!< ロケットパンチ 戻り
	GME_BOSS5_ACT_ID_RPC_STR_DMG_START,		//!< 強ロケットパンチダメージ 開始
	GME_BOSS5_ACT_ID_RPC_STR_DMG_LOOP,		//!< 強ロケットパンチダメージ ループ
	GME_BOSS5_ACT_ID_RPC_STR_DMG_RECOVER,	//!< 強ロケットパンチダメージ 復帰
	GME_BOSS5_ACT_ID_BERSERK_BREAKDOWN,		//!< 凶暴化演出 機能停止
	GME_BOSS5_ACT_ID_BERSERK_SHAKE,			//!< 凶暴化演出 振動動作（ガクガク）
	GME_BOSS5_ACT_ID_BERSERK_ROAR_PREP,		//!< 凶暴化演出 咆哮準備
	GME_BOSS5_ACT_ID_BERSERK_ROAR_START,	//!< 凶暴化演出 咆哮開始
	GME_BOSS5_ACT_ID_BERSERK_ROAR_LOOP,		//!< 凶暴化演出 咆哮ループ
	GME_BOSS5_ACT_ID_BERSERK_STAMP,			//!< 凶暴化演出 踏み込み
	GME_BOSS5_ACT_ID_BERSERK_KICKUP,		//!< 凶暴化演出 蹴り上げ
	
	GME_BOSS5_ACT_ID_MAX
} GME_BOSS5_ACT_ID;

//! ボス５移動 フェーズタイプ
typedef enum
{
	GME_BOSS5_BODY_MOVE_PHASE_TYPE_NONE	= 0,	//!< なし
	GME_BOSS5_BODY_MOVE_PHASE_TYPE_LEFT,		//!< 左足接地移動
	GME_BOSS5_BODY_MOVE_PHASE_TYPE_RIGHT,		//!< 右足接地移動
	
	GME_BOSS5_BODY_MOVE_PHASE_TYPE_MAX
} GME_BOSS5_BODY_MOVE_PHASE_TYPE;

//! ボス５走行移動タイプ
typedef enum
{
	GME_BOSS5_BODY_RUN_TYPE_LEFT	= 0,	//!< 左足が前
	GME_BOSS5_BODY_RUN_TYPE_RIGHT,			//!< 右足が前
	
	GME_BOSS5_BODY_RUN_TYPE_MAX
} GME_BOSS5_BODY_RUN_TYPE;

//! ボス５腕タイプ列挙型
typedef enum
{
	GME_BOSS5_ARM_TYPE_LEFT	= 0,
	GME_BOSS5_ARM_TYPE_RIGHT,
	
	GME_BOSS5_ARM_TYPE_MAX
} GME_BOSS5_ARM_TYPE;

//! ボス５腕パーツインデックス列挙型
typedef enum
{
	GME_BOSS5_ARMPART_IDX_SHOULDER	= 0,
	GME_BOSS5_ARMPART_IDX_ELBOW,
	GME_BOSS5_ARMPART_IDX_FOREARM,
	
	GME_BOSS5_ARMPART_IDX_MAX
} GME_BOSS5_ARMPART_IDX;

//! ボス５脚タイプ列挙型
typedef enum
{
	GME_BOSS5_LEG_TYPE_LEFT	= 0,
	GME_BOSS5_LEG_TYPE_RIGHT,
	
	GME_BOSS5_LEG_TYPE_MAX
} GME_BOSS5_LEG_TYPE;

//! ボス５噴射口タイプ列挙型
typedef enum
{
	GME_BOSS5_NOZZLE_TYPE_LEFT	= 0,
	GME_BOSS5_NOZZLE_TYPE_RIGHT,
	
	GME_BOSS5_NOZZLE_TYPE_MAX
} GME_BOSS5_NOZZLE_TYPE;

//! ボス５ 矩形配置ポイント列挙型
typedef enum
{
	GME_BOSS5_BODY_RECTPOINT_LEFT_FOOT	= 0,
	GME_BOSS5_BODY_RECTPOINT_RIGHT_FOOT,
	
	GME_BOSS5_BODY_RECTPOINT_SUB_NUM,	//!< 独自管理分の矩形の数
	
	// これ以降はGMS_ENEMY_COM_WORK分の矩形
	
	GME_BOSS5_BODY_RECTPOINT_BODY	= GME_BOSS5_BODY_RECTPOINT_SUB_NUM,
	
	GME_BOSS5_BODY_RECTPOINT_MAX
} GME_BOSS5_BODY_RECTPOINT;

//! ボス５ 飛散対象パーツインデックス列挙型
typedef enum
{
	GME_BOSS5_SCT_PART_IDX_SHOULDER_L	= 0,
	GME_BOSS5_SCT_PART_IDX_SHOULDER_R,
	GME_BOSS5_SCT_PART_IDX_GROIN_L,		//!< 脚の付け根L
	GME_BOSS5_SCT_PART_IDX_GROIN_R,		//!< 脚の付け根R
	GME_BOSS5_SCT_PART_IDX_ELBOW_L,
	GME_BOSS5_SCT_PART_IDX_ELBOW_R,
	GME_BOSS5_SCT_PART_IDX_FOREARM_L,
	GME_BOSS5_SCT_PART_IDX_FOREARM_R,
	GME_BOSS5_SCT_PART_IDX_THIGH_L,		//!< 太ももL
	GME_BOSS5_SCT_PART_IDX_THIGH_R,		//!< 太ももR
	GME_BOSS5_SCT_PART_IDX_KNEE_L,
	GME_BOSS5_SCT_PART_IDX_KNEE_R,
	GME_BOSS5_SCT_PART_IDX_KNEEPAD_L,
	GME_BOSS5_SCT_PART_IDX_KNEEPAD_R,
	GME_BOSS5_SCT_PART_IDX_FOOTCOVER_L,
	GME_BOSS5_SCT_PART_IDX_FOOTCOVER_R,
	GME_BOSS5_SCT_PART_IDX_FOOT_L,
	GME_BOSS5_SCT_PART_IDX_FOOT_R,
	GME_BOSS5_SCT_PART_IDX_TOE_L,
	GME_BOSS5_SCT_PART_IDX_TOE_R,
	GME_BOSS5_SCT_PART_IDX_HEEL_L,
	GME_BOSS5_SCT_PART_IDX_HEEL_R,
	
	GME_BOSS5_SCT_PART_IDX_MAX
} GME_BOSS5_SCT_PART_IDX;


//! 爆発 タイプ列挙型
typedef enum
{
	GME_BOSS5_EXPL_TYPE_SMALL	= 0,		// 小爆発
	GME_BOSS5_EXPL_TYPE_SMALL_WITH_FRAGS,	// 破片付き小爆発
	GME_BOSS5_EXPL_TYPE_BIG,				// 大爆発
	
	GME_BOSS5_EXPL_TYPE_MAX
} GME_BOSS5_EXPL_TYPE;

//!< ボス５ 警告レベル列挙型
typedef enum
{
	GME_BOSS5_ALARM_LEVEL_STRONG	= 0,	// 凶暴モード時
	GME_BOSS5_ALARM_LEVEL_CRASH,			// 地球割り時
	
	GME_BOSS5_ALARM_LEVEL_MAX
} GME_BOSS5_ALARM_LEVEL;

typedef struct tag_GMS_BOSS5_MGR_WORK	GMS_BOSS5_MGR_WORK;
typedef struct tag_GMS_BOSS5_BODY_WORK	GMS_BOSS5_BODY_WORK;
typedef struct tag_GMS_BOSS5_CORE_WORK	GMS_BOSS5_CORE_WORK;

//! 1ショットタイマワーク
typedef struct tag_GMS_BOSS5_1SHOT_TIMER
{
	Uint32	timer;
	BOOL	is_active;	//!< 有効フラグ
} GMS_BOSS5_1SHOT_TIMER;

//! 接地移動ワーク
typedef struct tag_GMS_BOSS5_GRD_MOVE_WORK
{
	fx32	cur_diff_x;		//!< 基準点と参照点の差 現在値
	fx32	prev_diff_x;	//!< 基準点と参照点の差 前回値
	Sint32	ref_snm_reg_id;		//!< 参照点ノードのSNM登録ID
	BOOL	is_first_updated;	//!< 初回更新フラグ（初回更新を座標に反映させないためのフラグ）
} GMS_BOSS5_GRD_MOVE_WORK;

//! 腕アニメーションワーク
typedef struct tag_GMS_BOSS5_ARM_ANIM_WORK
{
	BOOL	is_anim;
	Uint32	anim_wait_timer;
	Float	cur_rate;
	Float	rate_add;
	NNS_QUATERNION	start_quat[GME_BOSS5_ARMPART_IDX_MAX];
	NNS_QUATERNION	end_quat[GME_BOSS5_ARMPART_IDX_MAX];
} GMS_BOSS5_ARM_ANIM_WORK;

//! 爆発ワーク(eXPLoDe)
typedef struct tag_GMS_BOSS5_EXPL_WORK
{
	OBS_OBJECT_WORK	*parent_obj;
	GME_BOSS5_EXPL_TYPE	expl_type;
	Uint32	interval_timer;
	Uint32	interval_min;
	Uint32	interval_max;
	Float	se_frequency;
	Float	se_freq_cnt;
	fx32	ofst_pos[MTD_XY];
	fx32	area[MTD_WH];
} GMS_BOSS5_EXPL_WORK;

//! ボス５管理ワーク
struct tag_GMS_BOSS5_MGR_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	Sint32				life;	// ボスライフ
	
	void	(*proc_update)(GMS_BOSS5_MGR_WORK*);	//!< 更新処理関数
	
	Uint32				flag;
	
	Uint32				wait_timer;		//!< 汎用待機タイマ
	
	fx32				ply_demo_run_dest_x;	//!< 演出走り目標座標X
	
	Sint16				save_camera_offset[MTD_XY];	//!< カメラオフセット保存
	
	GME_BOSS5_ALARM_LEVEL	alarm_level;
	
	GMS_BOSS5_EXPL_WORK	small_expl_work;		//!< 小爆発処理ワーク
	GMS_BOSS5_EXPL_WORK	big_expl_work;			//!< 大爆発処理ワーク
	
	GMS_BOSS5_BODY_WORK		*body_work;
};

//! ボス５本体ワーク
struct tag_GMS_BOSS5_BODY_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	GME_BOSS5_BODY_STATE	state;		//!< ステート
	GME_BOSS5_BODY_STATE	prev_state;	//!< 前のステート
	
	GME_BOSS5_BODY_SUB_SEQ	sub_seq;	//!< サブシーケンス
	
	GME_BOSS5_STRAT_STATE	strat_state;	//!< 戦略ステート
	
	GMS_BOSS5_MGR_WORK		*mgr_work;	//!< 管理ワーク
	
	void	(*proc_update)(GMS_BOSS5_BODY_WORK*);	//!< 更新処理関数
	
	Uint32					flag;
	
	GME_BOSS5_ACT_ID		whole_act_id;	//!< ボス５全体アクションID
	
	Uint32					wait_timer;		//!< 汎用待機タイマ
	Uint32					no_hit_timer;	//!< 喰らい無効タイマ
	Uint32					fast_move_timer;	//!< 高速移動継続タイマ
	Uint32					poke_trg_limit_timer;	//!< 腕突き出し発動期限タイマ
	
	fx32					ground_v_pos;	//!< 地面垂直位置
	
	fx32					crash_pos_ofst_x;	//!< 地球割り落下目標地点オフセットX座標（スクロールロックエリアの水平方向中心基準）
	
	//! HIT無効タイマ終了後の食らいフラグ状態リクエストフラグ
	//  (1 << GME_BOSS5_BODY_RECTPOINT_XXX)がオンだと矩形当たり有効化・オフだと矩形当たり無効化されます
	Uint32					def_rect_req_flag;
	
	OBS_RECT_WORK			sub_rect_work[GME_BOSS5_BODY_RECTPOINT_SUB_NUM][GMD_ENEMY_RECT_NUM];	//!< 独自管理の当たり矩形
	
	GMS_BS_CMN_BMCB_MGR		bmcb_mgr;		//!< ボスモーションCB管理ワーク
	GMS_BS_CMN_SNM_WORK		snm_work;		//!< SNMワーク
	//Sint32				***_snm_reg_id;
	Sint32					body_snm_reg_id;	//!< 胴体SNM登録ID
	Sint32					lfoot_snm_reg_id;	//!< 左足SNM登録ID
	Sint32					rfoot_snm_reg_id;	//!< 右足SNM登録ID
	Sint32					armpt_snm_reg_ids[GME_BOSS5_ARM_TYPE_MAX][GME_BOSS5_ARMPART_IDX_MAX];
	Sint32					leg_snm_reg_ids[GME_BOSS5_LEG_TYPE_MAX];	//!< 足SNM登録ID
	Sint32					pole_snm_reg_id;	//!< バルカン銃座（ポール）SNM登録ID
	Sint32					groin_snm_reg_ids[GME_BOSS5_LEG_TYPE_MAX];	//!< 脚の付け根SNM登録ID
	Sint32					nozzle_snm_reg_ids[GME_BOSS5_NOZZLE_TYPE_MAX];	//!< 噴射口SNM登録ID
	
	VecFx32					pivot_prev_pos;	//!< ノード追随・矩形追随 相対配置用の基準座標（前フレーム）
	
	GMS_BS_CMN_CNM_MGR_WORK	cnm_mgr_work;	//!< CNM管理ワーク
	//Sint32				***_cnm_reg_id;
	//! 腕CNM登録ID
	Sint32					arm_cnm_reg_id[GME_BOSS5_ARM_TYPE_MAX][GME_BOSS5_ARMPART_IDX_MAX];
	//! バルカン銃座（ポール）CNM登録ID
	Sint32					pole_cnm_reg_id;
	//! 砲塔用カバーCNM登録ID
	Sint32					cover_cnm_reg_id;
	//! 首CNM登録ID
	Sint32					neck_cnm_reg_id;
	//! 頭CNM登録ID
	Sint32					head_cnm_reg_id;	// 蝶番のあたりにあるノード
	//! 飛散用CNM登録ID
	Sint32					scatter_cnm_reg_ids[GME_BOSS5_SCT_PART_IDX_MAX];
	
	//! ロケット配置オフセットマトリクス（CNMによる腕操作に合わせてロケットを動かすのに使用）
	NNS_MATRIX				rkt_ofst_mtx[GME_BOSS5_ARM_TYPE_MAX];
	//! 腕の各パーツ回転クォータニオン
	NNS_QUATERNION			arm_part_rot_quat[GME_BOSS5_ARM_TYPE_MAX][GME_BOSS5_ARMPART_IDX_MAX];
	
	GMS_BOSS5_ARM_ANIM_WORK	arm_anim_work;	//!< 腕アニメーションワーク
	Sint32					arm_poke_anim_phase;	//!< 腕突き出しアニメーションフェーズ
	
	NNS_QUATERNION			cnpy_close_init_quat;	//!< キャノピークローズ初期姿勢
	NNS_QUATERNION			cnpy_close_dest_quat;	//!< キャノピークローズ目標姿勢
	Float					cnpy_close_ratio;		//!< キャノピークローズ進捗度合い(0.f～1.f)
	Float					cnpy_close_ratio_spd;	//!< キャノピークローズ進捗度合い進行速度
	
	GMS_BS_CMN_DMG_FLICKER_WORK	flk_work;	//!< ダメージ点滅ワーク
	
	fx32					foot_ofst_record_src[GME_BOSS5_LEG_TYPE_MAX];	//!< 足ノードのモデル中心からのオフセット記録（元モーション用）
	fx32					foot_ofst_record_dest[GME_BOSS5_LEG_TYPE_MAX];	//!< 足ノードのモデル中心からのオフセット記録（目標モーション用）
	BOOL					adj_hgap_is_active;	//!< モーションブレンドズレ補正 有効フラグ
	Sint32					adj_hgap_act_id;	//!< モーションブレンドズレ補正 対象アクションID
	GME_BOSS5_LEG_TYPE		adj_hgap_leg_type;	//!< モーションブレンドズレ補正 基準脚タイプ
	
	VecFx32					grdmv_pivot_pos;	//!< 接地移動基準座標
	GMS_BOSS5_GRD_MOVE_WORK		grdmv_work;		//!< 接地移動ワーク
	
	GME_BOSS5_BODY_MOVE_PHASE_TYPE	cur_move_phase_type;	//! 現在の移動フェーズタイプ
	BOOL					is_move_reverse;			//!< 後方移動フラグ
	
	Sint32					walk_end_monitor_phase_cnt;	//!< 歩行終了監視 足接地タイミングフェーズカウント
	BOOL					is_player_behind;			//!< プレイヤーが後方にいたことを記録するフラグ
	
	Sint32					cur_walk_grnd_phase_cnt;	//!< 歩行足接地タイミングフェーズカウント
	GME_BOSS5_BODY_RUN_TYPE	run_grnd_runtype;			//!< 走行足接地時エフェクト処理 対象走行タイプ
	Uint32					run_grnd_delay_timer;		//!< 走行足接地時エフェクト処理 発生遅延タイマ
	Uint32					run_grnd_spawn_remain;		//!< 走行足接地時エフェクト処理 残り発生回数
	
	GME_BOSS5_BODY_RUN_TYPE	cur_run_type;		//!< 現在の走行タイプ
	
	GMS_BOSS5_1SHOT_TIMER	se_timer;		//!< SE用1ショットタイマ
	Uint32					se_cnt;			//!< SE再生カウント（何回も再生するときなどに利用）
	GSS_SND_SE_HANDLE		*se_hnd_leakage;	//!< 本体漏電用SEハンドル
	
	GMS_BOSS5_1SHOT_TIMER	targ_se_timer;	//!< ターゲットSE用1ショットタイマ
	Float					targ_se_cur_interval;	//!< ターゲットSE再生現在のインターバル
	
	GMS_BS_CMN_DELAY_SEARCH_WORK	dsearch_work;	//!< 遅延サーチワーク
	VecFx32					search_hist_buf[GMD_BOSS5_BODY_PLY_SEARCH_HIST_NUM];	//!< 履歴記録バッファ
	Sint32					ply_search_delay;	//!< プレイヤーサーチ遅延フレーム
	
	Angle32					turn_src_dir;	//!< 方向転換元角度
	Angle32					turn_tgt_ofst_dir;	//!< 方向転換目標角度
	Float					turn_ratio;		//!< 方向転換進捗度合い(0.f ～ 1.f)
	
	Float					bsk_shake_acc_ratio;	//!< 凶暴化演出時の振動モーション加速進捗
	Float					bsk_shake_acc_ratio_spd;	//!< 凶暴化演出時の振動モーション加速進捗速度
	Float					bsk_shake_init_spd;		//!< 凶暴化演出時の初期モーション速度
	
	Uint32					crash_strike_vib_delay_timer;	//!< 地面突き本体振動開始遅延タイマ
	Sint32					crash_strike_vib_phase;	//!< 地面突き本体振動フェーズカウント
	Float					crash_strike_vib_ratio;	//!< 進捗度合い
	
	Uint32					start_rise_vib_int_timer;	//!< 開始せり上がり時振動インターバルタイマ
	
	Uint32					sct_land_vib_timer;	//!< パーツ飛散後着地時の振動タイマ
	
	GMS_BOSS5_EXPL_WORK		expl_work;	//!< 爆発処理ワーク
	
	//! 構成パーツのオブジェクト（自分自身も含む）
	OBS_OBJECT_WORK	*parts_objs[GME_BOSS5_PART_IDX_MAX];
	
	//! 中心オブジェクト
	OBS_OBJECT_WORK	*part_obj_core;
};

//! ボス５ 中心オブジェクトワーク
struct tag_GMS_BOSS5_CORE_WORK
{
	GMS_ENEMY_COM_WORK	ene_com;
	
	void	(*proc_update)(GMS_BOSS5_CORE_WORK*);	//!< 更新処理関数
};


/*------ External Declarations -----------------------------------------*/
// 構築済みモデル(obj_3d)格納リスト
extern OBS_ACTION3D_NN_WORK *g_gm_boss5_obj_3d_list;


// =======================================================================
// GmBoss5Build
/*!
  ボスFINAL データ構築
 */
// =======================================================================
extern void GmBoss5Build(void);

// =======================================================================
// GmBoss5Flush
/*!
  ボスFINAL データ片付け
 */
// =======================================================================
extern void GmBoss5Flush(void);

// =======================================================================
// GmBoss5Init
/*!
  ボスFINAL（管理）初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss5Init(GMS_EVE_RECORD_EVENT *eve_rec,
									fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss5BodyInit
/*!
  ボスFINAL本体初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss5BodyInit(GMS_EVE_RECORD_EVENT *eve_rec,
										fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss5CoreInit
/*!
  ボスFINAL 中心オブジェクト初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
  
  @note
  常に本体の胴体部中心を追随するオブジェクトです。
  本体の当たり矩形を胴体部位置に設定する処理を行ったり、
  胴体部中心の座標を取得するためなどに使用されます。
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss5CoreInit(GMS_EVE_RECORD_EVENT *eve_rec,
										fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss5GetObject3dList
/*!
  構築済みモデル(obj_3d)格納リスト取得
  
  @return 構築済みモデル格納リスト
 */
// =======================================================================
extern OBS_ACTION3D_NN_WORK* GmBoss5GetObject3dList(void);

// =======================================================================
// GmBoss5BodyGetPlySearchPos
/*!
  プレイヤーサーチ 座標取得
  
  @param body_work	[io]	本体ワーク
  @param pos		[out]	座標格納先
 */
// =======================================================================
extern void GmBoss5BodyGetPlySearchPos(const GMS_BOSS5_BODY_WORK *body_work, VecFx32 *pos);

// =======================================================================
// GmBoss5ScatterSetFlyParam
/*!
  パーツ飛散エフェクト パーツ飛散パラメータ設定
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  主に移動関連のパラメータを設定します。
 */
// =======================================================================
extern void GmBoss5ScatterSetFlyParam(OBS_OBJECT_WORK *obj_work);

// =======================================================================
// GmBoss5MgrAnnouncePassedTrigger
/*!
  ボスFINAL発動トリガ通過を管理に伝える
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
inline void GmBoss5MgrAnnouncePassedTrigger(GMS_BOSS5_MGR_WORK *mgr_work)
{
	mgr_work->flag	|= GMD_BOSS5_MGR_FLAG_PLY_PASSED_TRG;
}

// =======================================================================
// GmBoss5Init1ShotTimer
/*!
  1ショットタイマ 初期化
  
  @param one_shot_timer	[io]	1ショットタイマワーク
 */
// =======================================================================
extern void GmBoss5Init1ShotTimer(GMS_BOSS5_1SHOT_TIMER *one_shot_timer, Uint32 frame);

// =======================================================================
// GmBoss5Update1ShotTimer
/*!
  1ショットタイマ 更新
  
  @param one_shot_timer	[io]	1ショットタイマワーク
  
  @retval TRUE	指定フレーム到達
  @retval FALSE	指定フレームに満たないor超過
  
  @note
  既定フレーム経過したら一度だけTRUEを返すタイマの初期化
 */
// =======================================================================
extern BOOL GmBoss5Update1ShotTimer(GMS_BOSS5_1SHOT_TIMER *one_shot_timer);

// =======================================================================
// GmBoss5UpdateVib
/*!
  汎用振動処理 更新
  
  @param phase_cnt	[in]	現在のフェーズ
  @param scale		[in]	スケール
  @param pos_x		[out]	X座標格納先
  @param pos_y		[out]	Y座標格納先
  
  @return 次のフェーズ
 
  @note
  phase_cntがフェーズの最大値を超えていた場合は回り込んだ値が使用されます。
  戻り値も回り込んだ値が返されます。
 */
// =======================================================================
extern Sint32 GmBoss5UpdateVib(Sint32 phase_cnt, fx32 scale, fx32 *pos_x, fx32 *pos_y);

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

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* GM_BOSS_5_H_ */
