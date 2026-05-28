// ==========================================================================
/*!
  @file gmMain.h
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmMain.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_MAIN_H_
#define GM_MAIN_H_

//----- Include Files -------------------------------------------------------
#ifndef _DS
#include "typedef.h"
#include "fx.h"
#include "mt.h"
#include "mi.h"
#endif

#include "objObject.h"
#include "gsMainSys.h"
#include "gmTask.h"
#include "gmObjDef.h"
#include "gmMainDat.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
/* デバック設定 */
#if defined (MTD_DEBUG)
#define GMD_DEBUG_DEBUGPAUSE_LEVEL	(10)	//!< デバック用ポーズレベル

#define GMD_DEBUG_DUMMY_MAP_TEX		(0)


#if _IPHONE || _PC
	// ◆環境整備までの暫定カット
//#define GMD_DEBUG_NO_CREATE_ENEMY		(1)		 // エネミー生成なし
//#define GMD_DEBUG_NO_CREATE_RING		(1)		 // リング生成なし
//#define GMD_DEBUG_NO_CREATE_GIMMICK		(1)		 // ギミック生成なし
//#define GMD_DEBUG_NO_CREATE_EFFECT				// エフェクト生成なし
//#define GMD_DEBUG_NO_CREATE_COCKPIT				// コックピット関連生成なし
	
#define GMD_DEBUG_AVOID_OBJECT_ERROR  // カット内容を反映させた際におこる様々な問題をこの定義で一括回避
#endif // _IPHONE || _PC

#endif //defined (MTD_DEBUG)



/* カメラスケール設定 */
//#define GMD_CAMERA_SCALE	(0.078125f * 2.19089f * GMD_OBJ_DRAW_SCALE)//2.1908902300206644538278791312032
#if !_IPHONE
	#define GMD_CAMERA_SCALE	(0.2997259375f)
#else //!_IPHONE
	#ifdef _MG_IPAD	
		//sss -- ipad camera paraMS
		#define _IPAD_SCALE_COEFF (480.f / 1024.f)
		#define GMD_CAMERA_SCALE    (0.674383359375f * _IPAD_SCALE_COEFF)
		//#define GMD_CAMERA_SCALE (0.50578751953125f * _IPAD_SCALE_COEFF)
		#define GMD_CAMERA_UP_SCALE_ADD  (0.01f * _IPAD_SCALE_COEFF)
		#define GMD_CAMERA_UP_SCALE_MAX  (0.3371916796875f * _IPAD_SCALE_COEFF)
	#else
		#define GMD_CAMERA_SCALE    (0.674383359375f)
			//#define GMD_CAMERA_SCALE (0.50578751953125f)
		#define GMD_CAMERA_UP_SCALE_ADD  (0.01f)
		#define GMD_CAMERA_UP_SCALE_MAX  (0.3371916796875f)
	#endif
#endif //!_IPHONE
	
/* オブジェクトシステム初期化用画面サイズ */
#define GMD_OBJ_LCD_X		((s16)(GMD_CAMERA_SCALE * GSD_DISP_WIDTH))
#define GMD_OBJ_LCD_Y		((s16)(GMD_CAMERA_SCALE * GSD_DISP_HEIGHT))

/* オブジェクトシステムイベント生成範囲 */
#if !_IPHONE
#define GMD_OBJ_CLIP_LCD_X	(383)
#define GMD_OBJ_CLIP_LCD_Y	(215)
#endif	// #if !_IPHONE

/* オブジェクト マップブロック 単位 */
#define GMD_MAP_BLOCK_LOCAL_SIZE	(20.f)	//!< マップブロック1ブロック分のローカルデータサイズ
#define GMD_MAP_BLOCK_SIZE			(64)	//!< マップブロック1ブロックのサイズ

/* マップ配置設定 */
#define GMD_MAP_ADJUST_POS_Z_FINAL_M		(-64)	//!< Finalステージ M面から後ろのオブジェクトZ位置補正値


/* オブジェクト描画スケール */
#define GMD_OBJ_DRAW_SCALE		(3.2f)									//!< 描画スケール float (64/20)
#define GMD_OBJ_DRAW_SCALE_FX	((fx32)(FX32_ONE*GMD_OBJ_DRAW_SCALE))	//!< 描画スケール fx

/* 画面スクロール最大移動速度 */
#define GMD_MAIN_SCR_SPD_MAX	(16)

/* タイム最大値 */
#ifdef MPPDEBUG_INFINITE_LIFE	
	#define GMD_MAIN_TIME_MAX			(2*60*60)		//sss //cheat: infinite time
#else
	#define GMD_MAIN_TIME_MAX			(9*60*60+59*60+59)		//!< 最大タイム
#endif

/* ゲームオーバー演出開始待機 */
#define GMD_MAIN_GOVER_SWAIT_TIME	(60*2)	//!< ゲームオーバーデモ開始待機時間

/* ライト明度 */
#define	GMD_LIGHT_COMN_INTENSITY		(1.0f)	// 通常の基本ライト明度
#define GMD_LIGHT_CMN_DARK_INTENSITY	(0.8f)	// Zone4-3暗い時のライト明度(基本)
#define GMD_LIGHT_MAP_DARK_INTENSITY	(0.4f)	// Zone4-3暗い時のライト明度(マップ用)
#define GMD_LIGHT_EX_MAP_DARK_INTENSITY	(0.7f)	// Zone4-3暗い時のライト明度(マップEX用)
#define GMD_LIGHT_PLY_DARK_INTENSITY	(1.0f)	// Zone4-3暗い時のライト明度(ソニック用)
#define GMD_LIGHT_GEAR_DARK_INTENSITY	(0.9f)	// Zone4-3暗い時のライト明度(歯車用)
#if _IPHONE
#define GMD_LIGHT_PLY_CMN_INTENSITY		(1.5f)	// 通常のライト明度(ソニック用)
#else
#define GMD_LIGHT_PLY_CMN_INTENSITY		(1.0f)	// 通常のライト明度(ソニック用)
#endif // _IPHONE

#if _IPHONE
typedef enum tag_GME_MAIN_CAMSCALE_STATE {
	GMD_MAIN_CAMSCALE_STATE_NON = 0, //!< なし
	GMD_MAIN_CAMSCALE_STATE_ZOOM, //!< ズーム中
	GMD_MAIN_CAMSCALE_STATE_UP, //!< ズーム完了
	
	GMD_MAIN_CAMSCALE_STATE_MAX
} GME_MAIN_CAMSCALE_STATE;
#endif // _IPHONE

typedef struct tag_GMS_MAIN_SYSTEM {
	u32				game_flag;

	MTS_TASK_TCB	*pre_tcb;			//!< 前処理TCB
	MTS_TASK_TCB	*post_tcb;			//!< 後処理TCB

	u32				game_time;			//!< ゲームタイマー
	u32				sync_time;			//!< 同期用タイマー(ギミック等で使用)

	/* プレイヤー情報 */
	struct tag_GMS_PLAYER_WORK	*ply_work[GSD_MAIN_PLAYER_MAX];	//!< 現在存在するプレイヤータイプオブジェクトワーク

	u32				marker_pri;			//!< 開始位置(チェック)ポイントの優先度
	u32				time_save;			//!< 中間ポイントタイム
	fx32			resume_pos_x;		//!< 開始位置X(再開位置) チェックポイントを通ると更新されます
	fx32			resume_pos_y;		//!< 開始位置Y(再開位置)

	u32				player_rest_num[GSD_MAIN_PLAYER_MAX];	//!< プレイヤー残機数

	/* マップ情報 */
	OBS_DIFF_COLLISION		map_fcol;		// 差分地形当たりデータアドレステーブル
	s32					map_size[MTD_XY];	// マップサイズ (GmMapBuildColDataで設定)

	u16	water_level;	//!< 水面の高さ(0xFFFFで無効)

	u16				pseudofall_dir;			//!< 擬似重力角度
	//Angle32			pseudofall_cam_ofst;	//!< 擬似重力カメラ オフセット値

	/* 死亡演出用 */
	fx32			die_event_wait_time;	//!< 死亡後遷移待機タイマー

	/* ライト */
	// 標準ライト
	NNS_VECTOR		def_light_vec;
	NNS_RGBA		def_light_col;
	// プレイヤーライト
	NNS_VECTOR		ply_light_vec;			//!< スーパーソニックの後の復帰など
	NNS_RGBA		ply_light_col;
	
	/* 実績用 */
	u32				ply_dmg_count;			//!< プレイヤーダメージ回数カウント

	/* Finalボスステージ用 */
	s32				boss_load_no;			//!< 現在ロードされているボスNO -1で未ロード
#if _IPHONE
	Angle32			polar_diff;				//!< 前フレームのパッド傾き
	Angle32			polar_now;				//!< 今フレームのパッド傾き
	
	GME_MAIN_CAMSCALE_STATE	camscale_state; //!< プレイヤー待機によるカメラアップ状態 iPhone専用
	float camera_scale; //!< カメラアップ
#endif // _IPHONE

#if defined (MTD_DEBUG)
	u32				debug_flag;
	s32				debug_save_pause_level;
	s32				debug_mode;				//!< デバック機能切り替え 現在のモード
	//s32				debug_ply_die_time_cnt;		// プレイヤーが死んだ時の時間をカウント(暫定)

	u32				debug_force_clear_cnt;		//!< 強制クリア用キーカウンタ
	u32				debug_pause_time_flag_save;	//!< タイムカウントフラグ退避
	s32				debug_check_mode;			//!< デバッグチェックモード
#endif

#if 0
	u32	score_trick;		//!< トリックスコア
	u32	score_speed;		//!< スピードスコア
	u32	score_speed_num;	//!< スピードスコア取得回数
	u32	score_ring;			//!< リングスコア
	u32 clear_state_flag;	//!< クリア後参照フラグ
	// ゲーム終了後外部参照ありデータ ここまで

	u8	player_num;			//!< 残機数

	u8	marker_pri;			//!< 開始位置(チェック)ポイントの優先度
	u32	time_save;			//!< 中間ポイントタイム
	fx32	resume_pos_x;	//!< 開始位置(再開位置)
	fx32	resume_pos_y;

	u16	resume_water_level;	//!< 水面の高さ(0で無効)

	// キー
	MTS_PAD_KEY_STATUS	key_status;				//!< メインプレイヤーキー取得用

	/* 通信関連 */
	// WiFi
	u16	net_frame_cnt[GSD_PLAYER_MAX];			//!< 通信パケット管理用フレームカウンタ
												// [0]    : 自分
												// [1] ～ : 対戦相手(受信パケットから設定)
	u16	net_use_packet_frame[GSD_PLAYER_MAX];	//!< パケット情報取得時の管理用フレーム
												// [0](自分)は使用しない

	u16 goal_packet_wait_timer;					//!< ゴール時 相手ゴールパケット待機タイマ
	u16 goal_packet_send_again_timer;			//!< ゴール時 ゴールパケット再送タイマ
	u16 goal_fade_packet_send_again_timer;		//!< ゴール時 フェードイベントパケット再送タイマ

	GMS_MAIN_GOAL_PACKET	goal_packet[GSD_PLAYER_MAX];	//!< ゴールパケット情報 相手分は受信した情報を格納

	void					*ene_packet;		//!< 敵パケット格納バッファ
	u32						ene_packet_num;		//!< 敵パケット格納バッファ使用数

	s32	ranking_cnt[GSD_PLAYER_MAX];			//!< レース順位チェック用
	u32	ranking_cnt_max[GSD_PLAYER_MAX];		//!< レース順位チェック用

	fx32	race_c_point_x;						//!< レースステージチェックポイントX位置

	/* ミッション関連 */
	u32				mission_time;				//!< ミッション用 制限時間保存
	s32				mission_rand;				//!< ミッション用 ランダム値 (開始時に設定)
	s16				mission_target_max;			//!< ミッション用 ステージ中最大フラッグ数 or 最大アイテム設置数 or リング目標数
	s16				mission_flag_no;			//!< ミッション用 フラッグ取得NO
	s16				mission_enemy_cnt;			//!< ミッション用 倒した敵カウンタ
	s16				mission_trick_cnt;			//!< ミッション用 トリックカウンタ
#endif
} GMS_MAIN_SYSTEM;

// game_flag
//#define GMD_GAME_FLAG_FINISH_DATALOAD	(0x00000001)	//!< データ読み込み終了
#define GMD_GAME_FLAG_GAMESYS_RESTART	(0x00000002)	//!< ゲームシステムリスタート
#define GMD_GAME_FLAG_CLEAR				(0x00000004)	//!< ステージクリアー
#define GMD_GAME_FLAG_RESULT_START		(0x00000008)	//!< リザルト演出開始
#define GMD_GAME_FLAG_RESULT_END		(0x00000010)	//!< リザルト演出終了
#define GMD_GAME_FLAG_GAMEOVER_START	(0x00000020)	//!< ゲームオーバー演出開始
#define GMD_GAME_FLAG_PAUSE_DEMO		(0x00000040)	//!< ポーズ演出中
#define GMD_GAME_FLAG_PAUSE_IS_DECIDED	(0x00000080)	//!< ポーズメニュー 決定された
#define GMD_GAME_FLAG_GAMEOVER_END		(0x00000100)	//!< ゲームオーバー演出終了
#define GMD_GAME_FLAG_TIMEOVER			(0x00000200)	//!< タイムオーバー死亡
#define GMD_GAME_FLAG_COUNT_GAME_TIME	(0x00000400)	//!< ゲームタイマーカウントアップ
#define GMD_GAME_FLAG_COUNT_SYNC_TIME	(0x00000800)	//!< 同期タイマーカウントアップ
#define GMD_GAME_FLAG_START_DEMO		(0x00001000)	//!< スタートデモ中
#define GMD_GAME_FLAG_WATER_LEVEL_EFCT_OFF	(0x00002000)	//!< water_levelが有効な値でも水面エフェクトを出さない
#define GMD_GAME_FLAG_SPECIAL_STAGE		(0x00004000)	//!< スペステリング突入
#define GMD_GAME_FLAG_SCR_LIMIT_BUSY	(0x00008000)	//!< スクロール制限機能動作中
#define GMD_GAME_FLAG_SPL_CHAOSGET		(0x00010000)	//!< スペステ：カオスエメラルドゲット（クリア）
#define GMD_GAME_FLAG_SPL_FAILED		(0x00020000)	//!< スペステ：失敗（ゴールパネル接触）
#define GMD_GAME_FLAG_SPL_TIMEOVER		(0x00040000)	//!< スペステ：タイムオーバー
#define GMD_GAME_FLAG_USE_SUPER_SONIC	(0x00080000)	//!< ゲーム中でスーパーソニック発動
#define GMD_GAME_FLAG_GOAL_IN			(0x00100000)	//!< ゴールした瞬間からＯＮ
#define GMD_GAME_FLAG_FINAL_DATA_LOAD	(0x00200000)	//!< ファイナルステージでデータロード発生中
#define GMD_GAME_FLAG_FINAL_DATA_RELEASE	(0x00400000)	//!< ファイナルステージでデータリリース発生中
#define GMD_GAME_FLAG_ENDING			(0x00800000)	//!< エンディング中
#define GMD_GAME_FLAG_START_MSG			(0x01000000)	//!< ゲーム開始時メッセージ表示中
#define GMD_GAME_FLAG_GOAL_AS_S_SONIC	(0x02000000)	//!< スーパーソニックの姿でゴールした
#define GMD_GAME_FLAG_RESUMED_FROM_MARKER	(0x04000000)	//!< ポイントマーカーから再開した

#if _IPHONE
#define GMD_GAME_FLAG_BGM_PLAY_ENABLE	(0x08000000)	//!< BGM再生可能設定
#define GMD_GAME_FLAG_BGM_PLAY_WAIT		(0x10000000)	//!< BGM再生可能設定
#define GMD_GAME_FLAG_SUSPENDED_PAUSE	(0x20000000)	//!< サスペンド後のポーズを実行
#endif // _IPHONE

/* 各種マスク */
/// ポーズ発動待機マスク
#define GMD_GAME_FLAG_PAUSE_WAIT_MASK	(GMD_GAME_FLAG_PAUSE_DEMO \
										 | GMD_GAME_FLAG_PAUSE_IS_DECIDED \
										 | GMD_GAME_FLAG_START_DEMO \
										 | GMD_GAME_FLAG_RESULT_START \
										 | GMD_GAME_FLAG_GAMEOVER_START \
										 | GMD_GAME_FLAG_GOAL_IN \
										 | GMD_GAME_FLAG_SPL_CHAOSGET \
										 | GMD_GAME_FLAG_SPL_FAILED \
										 | GMD_GAME_FLAG_SPL_TIMEOVER \
										 | GMD_GAME_FLAG_FINAL_DATA_LOAD \
										 | GMD_GAME_FLAG_FINAL_DATA_RELEASE \
										 | GMD_GAME_FLAG_ENDING \
										 | GMD_GAME_FLAG_START_MSG)

/// ゲーム開始時(リスタート時)クリアフラグマスク
#define GMD_GAME_FLAG_START_CLEAR_MASK	(GMD_GAME_FLAG_GAMESYS_RESTART | \
										GMD_GAME_FLAG_CLEAR | \
										GMD_GAME_FLAG_RESULT_START | \
										GMD_GAME_FLAG_RESULT_END | \
										GMD_GAME_FLAG_GAMEOVER_START | \
										GMD_GAME_FLAG_PAUSE_DEMO | \
										GMD_GAME_FLAG_PAUSE_IS_DECIDED | \
										GMD_GAME_FLAG_GAMEOVER_END | \
										GMD_GAME_FLAG_TIMEOVER | \
										GMD_GAME_FLAG_USE_SUPER_SONIC | \
										GMD_GAME_FLAG_FINAL_DATA_LOAD/*念のため*/ | \
										GMD_GAME_FLAG_FINAL_DATA_RELEASE/*念のため*/ | \
										GMD_GAME_FLAG_GOAL_AS_S_SONIC | \
										GMD_GAME_FLAG_RESUMED_FROM_MARKER | \
										GMD_GAME_FLAG_WATER_LEVEL_EFCT_OFF)


#if defined (MTD_DEBUG)
// debug_flag
#define GMD_GAME_DEBUG_FLAG_DEBUG_PAUSE	(0x00000001)	//!< デバックポーズ中
#define GMD_GAME_DEBUG_FLAG_PLY_POS_16	(0x00000002)	//!< プレイヤー座標を16進表示
#define GMD_GAME_DEBUG_FLAG_PLY_POS_10F	(0x00000004)	//!< プレイヤー座標を10進少数あり表示
#define GMD_GAME_DEBUG_FLAG_PLY_NO_DISP	(0x00000008)	//!< プレイヤー非表示
#define GMD_GAME_DEBUG_FLAG_MAP_NO_DISP	(0x00000010)	//!< マップ非表示
#define GMD_GAME_DEBUG_FLAG_OBJ_NO_DISP	(0x00000020)	//!< オブジェクト非表示
#if _IPHONE
#define GMD_GAME_DEBUG_FLAG_UP_SCREEN	(0x00000040)	//!< 画面強制アップ設定
#endif // _IPHONE

#define GMD_DEBUG_FLAG_NODISP_FAR	(0x00010000)	//!< 遠景非表示
#define GMD_DEBUG_FLAG_NODISP_FIX	(0x00020000)	//!< 敵非表示

enum {
	GMD_GAME_DEBUG_CHENGE_MODE_DEBUG_RECT	= 0,		//!< デバック矩形表示切替モード
	GMD_GAME_DEBUG_CHENGE_MODE_PLY_POS_TYPE,			//!< プレイヤー座標表示切替モード

	GMD_GAME_DEBUG_CHENGE_MODE_MAX
};

// デバッグチェックモード
typedef enum {
	GMD_MAIN_DEBUG_CHECK_MODE_NONE	= 0,
	GMD_MAIN_DEBUG_CHECK_MODE_BOSS_F_FINISH,	//!< 最後の一撃確認用
	
	GMD_MAIN_DEBUG_CHECK_MODE_MAX
} GME_MAIN_DEBUG_CHECK_MODE;
#endif

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
// ==========================================================================
// GMM_MAIN_STAGE_IS_BOSS
/*!
 *	ボスステージチェック
 *
 *	@return	TRUE : ボスステージ
 */
// ==========================================================================
#define GMM_MAIN_STAGE_IS_BOSS()	(g_gm_gamedat_stage_type_tbl[g_gs_main_sys_info.stage_id] == \
											GSD_MAIN_STAGE_TYPE_BOSS ? TRUE : FALSE)

// ==========================================================================
// GMM_MAIN_STAGE_IS_SS
/*!
 *	スペシャルステージチェック
 *
 *	@return	TRUE : スペシャルステージ
 */
// ==========================================================================
#define GMM_MAIN_STAGE_IS_SS()		(g_gm_gamedat_stage_type_tbl[g_gs_main_sys_info.stage_id] == \
											GSD_MAIN_STAGE_TYPE_SS ? TRUE : FALSE)

// ==========================================================================
// GMM_MAIN_STAGE_IS_ENDING
/*!
 *	エンディングステージチェック
 *
 *	@return	TRUE : エンディングステージ
 */
// ==========================================================================
#define GMM_MAIN_STAGE_IS_ENDING()		(g_gs_main_sys_info.game_mode == \
											GSD_GAME_MODE_ENDING ? TRUE : FALSE)

// ==========================================================================
// GMM_MAIN_GET_ZONE_TYPE
/*!
 *	ゾーンタイプ取得
 *
 *	@return	ゾーンタイプ GSE_MAIN_ZONE_TYPE
 */
// ==========================================================================
#define GMM_MAIN_GET_ZONE_TYPE()		(g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id])

// ==========================================================================
// GMM_MAIN_USE_SUPER_SONIC
/*!
 *	スーパーソニック使用状況
 *
 *	@return	TRUE : 現在のステージで使用した
 */
// ==========================================================================
#define GMM_MAIN_USE_SUPER_SONIC()		((g_gm_main_system.game_flag & GMD_GAME_FLAG_USE_SUPER_SONIC) ? TRUE : FALSE)

// ==========================================================================
// GMM_MAIN_GOAL_AS_SUPER_SONIC
/*!
 *	スーパーソニックの姿でゴールしたかチェック
 *
 *	@return	TRUE : 現在のステージでスーパーソニックの姿でゴールした。
 */
// ==========================================================================
#define GMM_MAIN_GOAL_AS_SUPER_SONIC()	((g_gm_main_system.game_flag & GMD_GAME_FLAG_GOAL_AS_S_SONIC) ? TRUE : FALSE)

//----- External Variables --------------------------------------------------
/// ゲームメインシステムワーク
extern GMS_MAIN_SYSTEM	g_gm_main_system;

//----- External Declarations -----------------------------------------------
// ==========================================================================
// 初期化・終了処理
// ==========================================================================
// ==========================================================================
// GmMainGSInit
/*!
 *	ゲーム 開始時 GS初期化
 */
// ==========================================================================
extern void GmMainGSInit(void);

// ==========================================================================
// GmMainGSRetryInit
/*!
 *	ゲーム 開始時 リトライ時GS初期化
 */
// ==========================================================================
extern void GmMainGSRetryInit(void);

// ==========================================================================
// GmMainInit
/*!
 *	ゲーム初期化
 */
// ==========================================================================
extern void GmMainInit(void *arg);

// ==========================================================================
// GmMainEnd
/*!
 *	ゲーム 終了
 *
 *	@note
 *		ゲーム処理の終了のみ行う
 */
// ==========================================================================
extern void GmMainEnd(void);

// ==========================================================================
// GmMainExit
/*!
 *	ゲーム メイン終了処理
 */
// ==========================================================================
extern void GmMainExit(void);

// ==========================================================================
// GmMainRestartExit
/*!
 *	ゲーム リスタート時メイン終了処理
 */
// ==========================================================================
extern void GmMainRestartExit(void);

// ==========================================================================
// GmMainExitForStaffroll
/*!
 *	スタッフロールにて使用する メイン終了処理
  	エンディング終了時にデータのみ残したままとなっているため、
  	データ解放処理のみを行う関数を用意
 */
// ==========================================================================
extern void GmMainExitForStaffroll(void);

// ==========================================================================
// タイマ関連
// ==========================================================================
// ==========================================================================
// GmMainCheckExeTimerCount
/*!
 *	タイマーカウント実行チェック
 *
 *	@return	TRUE : カウント実行
 *
 *	@note
 *		各種状況をチェックしてメインタイマーカウントアップを行うかチェックします
 */
// ==========================================================================
extern BOOL GmMainCheckExeTimerCount(void);


#if _IPHONE
// ==========================================================================
// 描画関連
// ==========================================================================
// ==========================================================================
// GmMainIsDrawEnable
/*!
 *	描画実行推奨チェック
 *
 *	@return	TRUE : 描画すべき
 *
 *	@note
 *		描画しないフレームを判定してくれます。
 */
// ==========================================================================
extern BOOL GmMainIsDrawEnable(void);

// ==========================================================================
// GmMainGetDrawMotionSpeed
/*!
 *	モーション実行推奨チェック
 *
 *	@return	掛け合わせるべきモーション速度
 *
 *	@note
 *		スキップした分必要になるモーション速度を返します。。
 */
// ==========================================================================
extern float GmMainGetDrawMotionSpeed(void);

// ==========================================================================
// GmMainGetObjectRotation
/*!
 *	オブジェクト回転情報作成
 *
 *	@return	現在のステージにて回転させるべきオブジェクトの角度
 *
 *	@note
 *		iPhone版では<BR>
 *			・カメラが回転する<BR>
 *			・カメラ固定で本体の傾きにゲームが追随する<BR>
 *		の2パターンがあるため、<BR>
 *		それに対応した角度を返します。
 */
// ==========================================================================
extern u16 GmMainGetObjectRotation(void);

// ==========================================================================
// GmMainGetLightColor
/*!
 *	オブジェクトに設定する疑似ライト情報入手
 *
 *	@return	今のオブジェクトに設定すべき色(0xRRGGBBAA)
 *
 *	@note
 *		疑似ライトカラーとして頂点に設定してください。
 */
// ==========================================================================
extern u32 GmMainGetLightColor(void);

#endif // _IPHONE


// ==========================================================================
// ボス専用データロード
// ==========================================================================
// ==========================================================================
// GmMainDatLoadBossBattleStart
/*!
 *	ゲームデータロード 開始 ボス連戦用
 *
 *	@param	boss_type		[in]	ロードボスタイプ GME_GAMEDAT_LOAD_BOSS_TYPE
 *
 *	@note
 *		GmGameDatLoadCheck でロードチェック
 *		GmGameDatLoadExit でロード終了
 *		ビルドまで行います
 */
// ==========================================================================
extern void GmMainDatLoadBossBattleStart(s32 boss_type);

// ==========================================================================
// GmMainDatLoadBossBattleLoadCheck
/*!
 *	ゲームデータロード 開始 ボス連戦用
 *
 *	@param	boss_type	[in]	チェックするボスタイプ 引数無しで何のデータでもOKに
 *
 *	@return TRUE : 終了
 */
// ==========================================================================
extern BOOL GmMainDatLoadBossBattleLoadCheck(s32 boss_type=-1);

// ==========================================================================
// GmMainDatLoadBossBattleLoadNowCheck
/*!
 *	ゲームデータロード中 チェック ボス連戦用
 *
 *	@return TRUE : 終了
 */
// ==========================================================================
extern BOOL GmMainDatLoadBossBattleLoadNowCheck(void);

// ==========================================================================
// GmGameDatLoadBossBattleExit
/*!
 *	ゲームデータロード 終了 ボス連戦用
 */
// ==========================================================================
extern void GmGameDatLoadBossBattleExit(void);

// ==========================================================================
// ボス連戦用 データリリース
// ==========================================================================
// ==========================================================================
// GmGameDatLoadBoosBattleStart
/*!
 *	ゲームデータリリース 開始 ボス連戦用
 *
 *	@param	boss_type		[in]	ロードボスタイプ GME_GAMEDAT_LOAD_BOSS_TYPE
 *
 *	@note
 *		Flushまで行います
 */
// ==========================================================================
extern void GmGameDatReleaseBossBattleStart(s32 boss_type);

// ==========================================================================
// GmMainDatReleaseBossBattleReleaseCheck
/*!
 *	ゲームデータリリース チェック ボス連戦用
 *
 *	@return TRUE : 終了
 */
// ==========================================================================
extern BOOL GmMainDatReleaseBossBattleReleaseCheck(void);

// ==========================================================================
// GmMainDatReleaseBossBattleReleaseNowCheck
/*!
 *	ゲームデータリリース中 チェック ボス連戦用
 *
 *	@return TRUE : 終了
 */
// ==========================================================================
extern BOOL GmMainDatReleaseBossBattleReleaseNowCheck(void);

// ==========================================================================
// GmGameDatReleaseBossBattleExit
/*!
 *	ゲームデータリリース 終了 ボス連戦用
 */
// ==========================================================================
extern void GmGameDatReleaseBossBattleExit(void);


// ==========================================================================
// Utility
// ==========================================================================
// ==========================================================================
// GmMainIsWaterLevel
/*!
 *	水面有効チェック
 *
 *	@return	TRUE : 水面有効
 */
// ==========================================================================
inline BOOL GmMainIsWaterLevel(void)
{
	if (g_gm_main_system.water_level == (u16)-1) {
		return (FALSE);
	}
	return (TRUE);
}
	
#if _IPHONE
// ==========================================================================
// GmMainKeyCheckPauseKeyOn
/*!
 *	ポーズキー On
 *
 *	@return	-1:入力なし　0～4:入力された番号
 *
 *	@note
 *		タッチの判定を返します。現在は領域を限定して判定しています。
 */
// ==========================================================================
Sint32 GmMainKeyCheckPauseKeyOn(void);

// ==========================================================================
// GmMainKeyCheckPauseKeyPush
/*!
 *	ポーズキー Push
 *
 *	@return	-1:入力なし　0～4:入力された番号
 *
 *	@note
 *		タッチの判定を返します。現在は領域を限定して判定しています。
 */
// ==========================================================================
Sint32 GmMainKeyCheckPauseKeyPush(void);


// ==========================================================================
// GmMainClearSuspendedPause
/*!
 *	サスペンド後のポーズフラグを消去
 *
 */
// ==========================================================================
inline void GmMainClearSuspendedPause(void)
{
	g_gs_main_sys_info.game_flag &= ~GMD_GAME_FLAG_SUSPENDED_PAUSE;
}

#endif // _IPHONE

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_MAIN_H_

//----- Include Files -------------------------------------------------------
