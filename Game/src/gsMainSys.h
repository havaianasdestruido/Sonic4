// ================================================================
/*!
  @file gsMainSys.h
  @brief メインシステム

  @author Ishizaki
                Copyright(c) 2009 Dimps

  $Id: gsMainSys.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 *
 */

#ifndef GS_MAIN_SYS_H_
#define GS_MAIN_SYS_H_



//----- Include Files -------------------------------------------------------
#include "mt.h"
#include "gsBackup.hpp"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
// インライン３用DEFINE
//#define HOG_INLINE3_ROM

// αROM用定義
#if defined(_DLC)
#define HOG_ALPHA_ROM
#endif //defined(_DLC)

#if defined (MTD_DEBUG)

#define	GSD_DEBUG_LAUNCHER				(1)					//!< ランチャー

#if defined (HOG_ALPHA_ROM) || _IPHONE
#define	GSD_DEBUG_DEMO_SELECT_TOP		(1)					//!< GSD_DEBUG_DEMO_SELECT 無効時、デバックメニューのみ有効
#else
#define	GSD_DEBUG_DEMO_SELECT			(1)					//!< デモセレクト
//#define	GSD_DEBUG_DEMO_SELECT_TOP		(1)					//!< GSD_DEBUG_DEMO_SELECT 無効時、デバックメニューのみ有効
#endif

#if _PC | _XBOX
//#define	GSD_DEBUG_MODEL_BUILD				//!< デバック用モデルビルド
#endif	// #if _PC | _XBOX
#else	// #if defined (MTD_DEBUG)
#endif	// #if defined (MTD_DEBUG)




// ==========================================================================
// ゲーム中基本定義
// ==========================================================================
/* プレイヤー標準残機数 */
#define GSD_MAINSYS_PLAYER_REST_DEF	(3)

/* プレイヤー残機数最大値 */
#define GSD_MAINSYS_PLAYER_REST_MAX	(1000)	// (残機1で表示0にしているので、表示を999とするためMAXを1000にする)

// ウインドウ開閉演出フレーム
#define GSD_MAINSYS_WIN_EFCT_FRAME	(8)

/****************************************************************************/
// イベントデータ
/*
	必要となるイベントデータを定義する。
	ここで定義したイベントはイベントデータにも同時に登録する。(gsMainSys.c _gs_evt_data)
	かならず1番目(0)には空データを設定する。
*/
/****************************************************************************/
/// イベントID
typedef enum _usre_evt_id {
	GSD_EVT_ID_NOP			=	0,		// 予約
	GSD_EVT_ID_SYS_INIT,				// システム初期化
	GSD_EVT_ID_LOGO_SEGA,				// セガロゴ
	GSD_EVT_ID_TITLE,					// タイトル
	GSD_EVT_ID_MAINMENU,				// メインメニュー
	GSD_EVT_ID_MAP,						// 全体マップ
	GSD_EVT_ID_MAINGAME,				// メインゲーム
	GSD_EVT_ID_RANKING,					// ランキング
	GSD_EVT_ID_OPTION,					// オプション
	GSD_EVT_ID_ENDING,					// エンディング
	GSD_EVT_ID_STAFFROLL,				// スタッフロール
	GSD_EVT_ID_SPSTAGE_BRANCH,			// スペステ分岐
#if _PC || _XBOX || _PS3 || _IPHONE
	GSD_EVT_ID_BUYSCREEN,				// 製品版購入画面
#endif // _PC || _XBOX || _PS3 || _IPHONE
	GSD_EVT_ID_LOGO_SONIC,				// ソニックチームロゴ
#if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR)) || _XBOX || _PC
	GSD_EVT_ID_LOGO_ESRB,				// ESRBロゴ(レーティング)
#endif
#if _PS3
	GSD_EVT_ID_SAVE_ATTENTION,			// 起動直後オートセーブ注意表示
#endif // _PS3
#if _WII || _PC
	GSD_EVT_ID_WII_EXP_OPE,				// Wii 横持ち促し画面
#endif
#if (defined(HOG_RGN_JP) && _PS3) || _PC
	GSD_EVT_ID_PS3_KIDO,				// PS3起動直後画面
#endif
#if _IPHONE
	GSD_EVT_ID_MOVIE,					// ムービー
#endif // _IPHONE

#if defined (MTD_DEBUG)
	/* デバック */

#if defined (GSD_DEBUG_DEMO_SELECT) || defined (GSD_DEBUG_DEMO_SELECT_TOP)
	GSD_EVT_ID_DEBUG_DEMO,				// デモデバッグ
#if !_IPHONE
	GSD_EVT_ID_DEBUG_PAUSE_MENU,		// ポーズメニューテスト
#endif //!_IPHONE
	GSD_EVT_ID_DEBUG_LANG_CHANGE,		// 言語切り替えメニュー
#endif

#if defined GSD_DEBUG_MODEL_BUILD
	GSD_EVT_ID_DEBUG_MODEL_BUILD,		// デバック用モデルビルド
#endif

#if !_IPHONE
	GSD_EVT_ID_AOTESTSYSMSG,			// aoSysMsgテスト
	GSD_EVT_ID_AOTESTSYSMSG2,			// aoSysMsgテスト
	GSD_EVT_ID_AOTESTSAVE,				// セーブテスト
	GSD_EVT_ID_AOTESTTROPHY,			// トロフィーテスト
	GSD_EVT_ID_AOTESTNET,				// 通信テスト
	GSD_EVT_ID_AOTESTWAIT,				// 待ちテスト
	GSD_EVT_ID_AOTESTPRESENCE,			// プレゼンステスト
	GSD_EVT_ID_AOTESTEXIT,				// ゲーム終了テスト
	GSD_EVT_ID_AOTESTAVATARAWARD,		// アバターアワードテスト
	GSD_EVT_ID_AOTESTSAFEFRAME,			// セーフフレーム表示
	GSD_EVT_ID_AOTESTWIIFATAL,			// WiiFatalエラーテスト
	GSD_EVT_ID_GSTRIALTEST,				// 体験版テスト
	GSD_EVT_ID_GSSTRAPIMAGETEST,		// ストラップ画面テスト
#endif	// #if !_IPHONE
	GSD_EVT_ID_DBGBUYSCREEN,			// 製品版購入画面デバッグ用イベント
#if _PS3
	GSD_EVT_ID_SAVE_ATTENTION_DEBUG,	// 起動直後オートセーブ注意テスト表示
#endif // _PS3

#endif

	GSD_EVT_ID_NUM
} GSE_EVT_ID;


/****************************************************************************/
// メインシステム
/****************************************************************************/
/// ゲームモード game_mode
typedef enum {
	GSD_GAME_MODE_STORY		= 0,	//!< ストーリーモード
	GSD_GAME_MODE_TIME_ATTACK,		//!< タイムアタックモード
//	GSD_GAME_MODE_SPECIAL_STAGE,	//!< スペステモード
	GSD_GAME_MODE_ENDING,			//!< エンディングモード

	GSD_GAME_MODE_MAX
} GSE_GAME_MODE;

/// 難易度 level
typedef enum tag_GSE_GAME_LEVEL_TYPE {
	GSD_GAME_LEVEL_EASY		= 0,	//!< やさしい
	GSD_GAME_LEVEL_NORMAL,			//!< 普通
	GSD_GAME_LEVEL_HARD,			//!< むずかしい

	GSD_GAME_LEVEL_MAX
} GSE_GAME_LEVEL_TYPE;

/// キャラクターID
typedef enum tag_GSE_CHAR_ID {
	GSD_CHAR_ID_SONIC	= 0,			//!< ソニック
	GSD_CHAR_ID_S_SONIC,				//!< スーパーソニック
	GSD_CHAR_ID_SP_SONIC,				//!< スペステソニック
	GSD_CHAR_ID_PN_SONIC,				//!< ピンボールソニック
	GSD_CHAR_ID_PN_S_SONIC,				//!< ピンボールスーパーソニック
	GSD_CHAR_ID_TR_SONIC,				//!< トロッコソニック
	GSD_CHAR_ID_TR_S_SONIC,				//!< トロッコスーパーソニック

	GSD_CHAR_ID_MAX,

	GSD_CHAR_ID_INVALID	= -1	//!< 無効値
} GSE_CHAR_ID;

/// プレイヤー数
typedef enum tag_GSE_MAIN_PLAYER {
	GSD_MAIN_PLAYER_1P		= 0,

	GSD_MAIN_PLAYER_MAX				//!< プレイヤー数最大数
} GSE_MAIN_PLAYER;

/// ステージID
typedef enum tag_GSE_MAIN_STAGE_ID {
	GSD_MAIN_STAGE_ID_1_1,			//!< ZONE1-1
	GSD_MAIN_STAGE_ID_1_2,			//!< ZONE1-2
	GSD_MAIN_STAGE_ID_1_3,			//!< ZONE1-3
	GSD_MAIN_STAGE_ID_1_BOSS,		//!< ZONE1-BOSS
	GSD_MAIN_STAGE_ID_2_1,			//!< ZONE2-1
	GSD_MAIN_STAGE_ID_2_2,			//!< ZONE2-2
	GSD_MAIN_STAGE_ID_2_3,			//!< ZONE2-3
	GSD_MAIN_STAGE_ID_2_BOSS,		//!< ZONE2-BOSS
	GSD_MAIN_STAGE_ID_3_1,			//!< ZONE3-1
	GSD_MAIN_STAGE_ID_3_2,			//!< ZONE3-2
	GSD_MAIN_STAGE_ID_3_3,			//!< ZONE3-3
	GSD_MAIN_STAGE_ID_3_BOSS,		//!< ZONE3-BOSS
	GSD_MAIN_STAGE_ID_4_1,			//!< ZONE4-1
	GSD_MAIN_STAGE_ID_4_2,			//!< ZONE4-2
	GSD_MAIN_STAGE_ID_4_3,			//!< ZONE4-3
	GSD_MAIN_STAGE_ID_4_BOSS,		//!< ZONE4-BOSS
	GSD_MAIN_STAGE_ID_FINAL_1,		//!< ZONEFinal-1  final stage
	GSD_MAIN_STAGE_ID_FINAL_2,		//!< ZONEFinal-2  (//sss - unused on iPhone)
	GSD_MAIN_STAGE_ID_FINAL_3,		//!< ZONEFinal-3  (//sss - unused on iPhone)
	GSD_MAIN_STAGE_ID_FINAL_4,		//!< ZONEFinal-4  (//sss - unused on iPhone)
	GSD_MAIN_STAGE_ID_FINAL_5,		//!< ZONEFinal-5  (//sss - unused on iPhone)

	GSD_MAIN_STAGE_ID_SS1,			//!< SpeclalStage1
	GSD_MAIN_STAGE_ID_SS2,			//!< SpeclalStage2
	GSD_MAIN_STAGE_ID_SS3,			//!< SpeclalStage3
	GSD_MAIN_STAGE_ID_SS4,			//!< SpeclalStage4
	GSD_MAIN_STAGE_ID_SS5,			//!< SpeclalStage5
	GSD_MAIN_STAGE_ID_SS6,			//!< SpeclalStage6
	GSD_MAIN_STAGE_ID_SS7,			//!< SpeclalStage7


	GSD_MAIN_STAGE_ID_ENDING,		//!< エンディングステージ

	GSD_MAIN_STAGE_ID_MAX

} GSE_MAIN_STAGE_ID;

/// エリアID
//typedef enum tag_GSE_MAIN_STAGEAREA_ID {
//	GSD_MAIN_STAGEAREA_ID_1	= 0,
//	GSD_MAIN_STAGEAREA_ID_2,
//	GSD_MAIN_STAGEAREA_ID_3,
//
//	GSD_MAIN_STAGEAREA_MAX
//
//} GSE_MAIN_STAGEAREA_ID;

/// ステージタイプ
typedef enum tag_GME_MAIN_STAGE_TYPE {
	GSD_MAIN_STAGE_TYPE_ACT	= 0,		//!< 通常ACTステージ
	GSD_MAIN_STAGE_TYPE_BOSS,			//!< ボスステージ
	GSD_MAIN_STAGE_TYPE_SS,				//!< スペシャルステージ

	GSD_MAIN_STAGE_TYPE_MAX

} GSE_MAIN_STAGE_TYPE;

/// ステージゾーンタイプ
typedef enum tag_GSE_MAIN_ZONE_TYPE {
	GSD_MAIN_ZONE_TYPE_1		= 0,
	GSD_MAIN_ZONE_TYPE_2,
	GSD_MAIN_ZONE_TYPE_3,
	GSD_MAIN_ZONE_TYPE_4,
	GSD_MAIN_ZONE_TYPE_FINAL,
	GSD_MAIN_ZONE_TYPE_SS,

	GSD_MAIN_ZONE_TYPE_MAX,
	GSD_MAIN_ZONE_TYPE_NONE	= -1
} GSE_MAIN_ZONE_TYPE;

#if 0
/// セーブエラータイプ
typedef enum
{
	GSE_ERROR_SAVE_TYPE_READ_ERROR	= 0,	//!< 読み込みエラー
	GSE_ERROR_SAVE_TYPE_WRITE_ERROR,		//!< 書き込みエラー
	GSE_ERROR_SAVE_TYPE_FILE_DESTROY,		//!< ファイル破壊エラー
	GSE_ERROR_SAVE_TYPE_FILE_NOTFOUND,		//!< ファイルが見つからなかった
	
	GSE_ERROR_SAVE_TYPE_MAX
} GSE_ERROR_SAVE_TYPE;
#endif


/// メインシステム情報
typedef struct tag_GSS_MAIN_SYS_INFO {
	u32					main_flag;

	u32					game_flag;

#if _IPHONE
	u32					sys_flag;
#endif // _IPHONE

	/* システム情報 */
	float				sys_disp_width;			//!< 表示解像度
	float				sys_disp_height;		//!< 表示解像度

	/* ゲーム設定情報 */
	GSE_GAME_LEVEL_TYPE	level;					//!< 難易度
	GSE_GAME_MODE		game_mode;				//!< 現在のゲームモード ゲーム初期化を行う前に設定しておく事

	/* ゲーム進行情報 */
	u16					stage_id;				//!< 現在のステージID
//	u16					area_id;				//!< 現在のエリアID

	GSE_CHAR_ID			char_id[GSD_MAIN_PLAYER_MAX];	//!< 使用キャラクターID GSE_CHAR_ID -1で無効

	/* クリア時取得データ */
	u32					clear_ring;				//!< クリア時のリング数
	u32					clear_score;			//!< クリア時のスコア
	s32					clear_time;				//!< クリア時のタイム

	/* プレイヤー情報 */
	u32					rest_player_num;		//!< 残機数

	/* サウンド情報 */
	Float				se_volume;				//!< SEのボリューム値
	Float				bgm_volume;				//!< BGMのボリューム値

	/* トロフィー・実績用情報 */
	u32					ene_kill_count;			//!< エネミー撃破カウント
	u32					final_clear_count;		//!< ファイナルゾーンクリアカウント

	/* その他演出用情報 */
	/* エラー関連 */
	/* セーブデータ展開データ */
	GSS_BACKUP			backup;
	GSS_BACKUP			cmp_backup;
	u32					is_save_run;				//!< セーブを行っていいかどうか(0の場合、常にセーブをカットする)
	/* その他一括管理するべきデータ等 */
	u16					prev_stage_id;				//!< 暫定仮でここに配置
	BOOL				is_spe_clear;				//!< 暫定仮で配置
	BOOL				is_first_play;				//!< プレイしたステージが初回かどうか
	
	/* デバック用 */
#if defined(MTD_DEBUG)  // デバッグ版
	u32					debug_rest_player_num;		//!< 暫定残機数格納


    u32 debug_flag; 
	u32	debug_bl_msg_type;	//!< バックアップローダメッセージタイプ（表示確認用）

	fx32				debug_key_save_player_pos_x;	//!< キーセーブ用 プレイヤー開始位置保存
	fx32				debug_key_save_player_pos_y;
	BOOL				debug_key_save_player_hflip;	//!< キーセーブ用 プレイヤー反転状態
	u32					debug_key_save_play_timer;		//!< キーセーブ用 タイマ
#endif

} GSS_MAIN_SYS_INFO;

// GSS_MAIN_SYS_INFO : game_flag
#define GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC		(0x00000001)		//!< 入力タイプクラシック
#define GSD_MAINSYS_GAME_FLAG_CLEAR				(0x00000002)		//!< 直前にプレイしたゲームをクリア
#define GSD_MAINSYS_GAME_FLAG_RESTART			(0x00000004)		//!< リスタートあり
#define GSD_MAINSYS_GAME_FLAG_TIMEOVER			(0x00000008)		//!< タイムオーバーあり
#define GSD_MAINSYS_GAME_FLAG_DATALOADEND		(0x00000010)		//!< データロード済み
#define GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD	(0x00000020)		//!< カオスエメラルド 7つ保持
#define GSD_MAINSYS_GAME_FLAG_PAD_VIB_ENABLE	(0x00000040)		//!< パッド振動有効
#define GSD_MAINSYS_GAME_FLAG_SPECIAL_STAGE		(0x00000080)		//!< スペシァルステージ
#define GSD_MAINSYS_GAME_FLAG_TIME_RESET_AT_MARKER	(0x00000100)	//!< ポイントマーカーを通過した状態でタイマがリセットされた
#define GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK		(0x00000200)	//!< 入力タイプフリック

#if defined HOG_PRESENT_ROM_IPHONE
#define GSD_MAINSYS_GAME_FLAG_NEXTACT			(0x00100000)		//!< 次ACTへ
#endif // HOG_PRESENT_ROM_IPHONE

#define GSD_MAINSYS_GAME_FLAG_STARTINIT_MASK	(GSD_MAINSYS_GAME_FLAG_CLEAR | \
											GSD_MAINSYS_GAME_FLAG_RESTART | \
											GSD_MAINSYS_GAME_FLAG_DATALOADEND | \
											GSD_MAINSYS_GAME_FLAG_TIMEOVER | \
											GSD_MAINSYS_GAME_FLAG_TIME_RESET_AT_MARKER)	//!< ゲーム開始時のクリアマスク


#define GSD_MAINSYS_GAME_FLAG_RETRY_MASK	(GSD_MAINSYS_GAME_FLAG_CLEAR | \
											GSD_MAINSYS_GAME_FLAG_RESTART | \
											GSD_MAINSYS_GAME_FLAG_TIMEOVER | \
											GSD_MAINSYS_GAME_FLAG_TIME_RESET_AT_MARKER)	//!< ゲームリトライ開始時のクリアマスク



/* データ参照用定義 */
#define GSD_DISP_WIDTH		(g_gs_main_sys_info.sys_disp_width)		//!< 表示解像度X
#define GSD_DISP_HEIGHT		(g_gs_main_sys_info.sys_disp_height)	//!< 表示解像度Y

#if _IPHONE
// GSS_MAIN_SYS_INFO : sys_flag
#define GSD_MAINSYS_SYS_FLAG_SUSPEND		(0x00000001)		//!< サスペンドが発生した
#endif // _IPHONE

/* デバック設定 */
#if defined (MTD_DEBUG)
// GSS_MAIN_SYS_INFO::debug_flag
#define GSD_DEBUG_RECT_H				(1 <<  0)	//!< 当たり判定矩形表示
//#define GSD_DEBUG_RECT_F				(1 << 1)	//!< 地形判定矩形表示
#define GSD_DEBUG_BAR					(1 <<  2)	//!< 処理バー表示
#define GSD_DEBUG_DATA					(1 <<  3)	//!< データ表示

//#define GSD_DEBUG_NODAMAGE				(1 << 4)	//!< ダメージを受けてもしなない
#define GSD_DEBUG_STEP_ON				(1 <<  5)	//!< ステップ停止を有効にする
#define GSD_DEBUG_SLOW					(1 <<  6)	//!< ゲームを擬似スローで実行

#define GSD_DEBUG_PAUSE					(1 <<  9)	//!< デバッグポーズ有効
#define GSD_DEBUG_KEY_PLAY				(1 << 10)	//!< キープレイデバック中
#define GSD_DEBUG_KEY_SAVE				(1 << 11)	//!< キーセーブデバック中

#define GSD_DEBUG_SUPER_SONIC_FREE		(1 << 12)	//!< スーパーソニック自由起動
#define GSD_DEBUG_DEBUG_DISP			(1 << 13)	//!< デバック表示ON
#define GSD_DEBUG_FPS_DISP				(1 << 16)	//!< FPS表示ON

#define GSD_DEBUG_ZONE_FINAL_5_CHECK	(1 << 14)	//!< FINAL-5(ファイナルボス)チェック
#define GSD_DEBUG_ZONE_FINAL_5_CHECK_DONE	(1 << 15)
#define GSD_DEBUG_ZONE_FINAL_CHECK_MASK	(GSD_DEBUG_ZONE_FINAL_5_CHECK | \
										 GSD_DEBUG_ZONE_FINAL_5_CHECK_DONE)
#endif // #if defined (MTD_DEBUG)

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
// ==========================================================================
// GSM_MAIN_STAGE_IS_SPSTAGE
/*!
 *	スペステステージチェック
 *
 *	@return	TRUE : スペステステージ
 */
// ==========================================================================
#define GSM_MAIN_STAGE_IS_SPSTAGE()	(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_SPECIAL_STAGE ? \
											TRUE : FALSE)

// ==========================================================================
// GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY
/*!
 *	スペステステージチェック(リトライ中は除く)
 *
 *	@return	TRUE : スペステステージ
 */
// ==========================================================================
#define GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()													\
	( (  (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_SPECIAL_STAGE)					\
	   &&(!(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_TATK_RETRY)))	\
	 ? TRUE : FALSE)

//----- External Variables --------------------------------------------------
extern GSS_MAIN_SYS_INFO	g_gs_main_sys_info;				//!< メインシステム情報


//----- External Declarations -----------------------------------------------
// ==========================================================================
// GsInitUser
/*!
 *	システム初期化ユーザー関数
 */
// ==========================================================================
extern void GsInitUser(void);

// ==========================================================================
// GsExitUser
/*!
 *	システム終了処理ユーザー関数
 */
// ==========================================================================
extern void GsExitUser(void);

// ==========================================================================
// GsGetMainSysInfo
/*!
 *	メインシステム情報取得
 *
 *	@return	メインシステム情報構造体アドレス
 */
// ==========================================================================
inline GSS_MAIN_SYS_INFO* GsGetMainSysInfo(void)
{
	return (&g_gs_main_sys_info);
}

// ==========================================================================
// GsMainSysIsStageClear
/*!
 *	引数に指定したステージがクリア済みかどうかを返す関数
 *
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
extern BOOL GsMainSysIsStageClear(s32 stage_id);

// ==========================================================================
// GsMainSysIsStageSonicClear
/*!
 *	引数に指定したステージが
	ソニックを使用してクリア済みかどうかを返す関数
 *
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
extern BOOL GsMainSysIsStageSonicClear(s32 stage_id);

// ==========================================================================
// GsMainSysIsStageSuperSonicClear
/*!
 *	引数に指定したステージがスーパーソニックにて
 *	クリア済みかどうかを返す関数
 *	
 *	※引数のステージIDはZONE1～4とZONE FINALのステージIDを指定してください。
 *	それ以外のステージIDだとアサートになります。
 *	
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
extern BOOL GsMainSysIsStageSuperSonicClear(s32 stage_id);

// ==========================================================================
// GsMainSysIsStageGoalAsSuperSonic
/*!
 *	引数に指定したステージがスーパーソニックにて
 *	クリア済みかどうかを返す関数
 *	
 *	※引数のステージIDはZONE1～4とZONE FINALのステージIDを指定してください。
 *	それ以外のステージIDだとアサートになります。
 *	
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
extern BOOL GsMainSysIsStageGoalAsSuperSonic(s32 stage_id);

// ==========================================================================
// GsMainSysIsStageScoreUploadOnce
/*!
 *	引数に指定したステージがハイスコアデータを一度でも
 *	アップロードしたかどうかを取得する関数
 *	
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
extern BOOL GsMainSysIsStageScoreUploadOnce(s32 stage_id);

// ==========================================================================
// GsMainSysIsStageTimeUploadOnce
/*!
 *	引数に指定したステージがタイムレコードデータを一度でも
 *	アップロードしたかどうかを取得する関数
 *	
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
extern BOOL GsMainSysIsStageTimeUploadOnce(s32 stage_id);


// ==========================================================================
// GsMainSysIsSpecialStageClearedAct
/*!
 *	引数に指定したステージで既に一度スペステを
 *	クリアしたACTかどうかを返す関数
 *	
 *	@return	TRUE : 引数のステージではスペステをクリア済み
 *			FALSE: 引数のステージではスペステを未クリア
 */
// ==========================================================================
extern BOOL GsMainSysIsSpecialStageClearedAct(s32 stage_id);


// ==========================================================================
// システム初期化
// ==========================================================================
// ==========================================================================
// GsMainSysSystemInitEvent
/*!
 *	システム初期化用イベント
 */
// ==========================================================================
extern void GsMainSysSystemInitEvent(void *arg);

#if _IPHONE
// ==========================================================================
// GsMainSysSetSleepFlag
/*!
 *	ハード的にスリープに入るべきか否かを設定します。
 *	標準は入る設定になっています。
 *	
 *	@param	flag	[in]	TRUE:スリープに入る	FALSE:入らない
 */
// ==========================================================================
extern void GsMainSysSetSleepFlag(BOOL flag);

// ==========================================================================
// GsMainSysIsSuspendedSystem
/*!
 *	ハード的にサスペンドさせられているかどうかをチェックする
 *	
 *	@return	TRUE : サスペンドしている
 *			FALSE: サスペンドしてない
 */
// ==========================================================================
extern BOOL GsMainSysIsSuspendedSystem(void);

// ==========================================================================
// GsMainSysSetSuspendedFlag
/*!
 *	ハード的なサスペンドのフラグを設定する
 *	ゲーム側からは呼ばないようにしてください。
 *	
 *	@param flag [in] TRUE:前フレームまでサスペンドした / FALSE:してない
 *	
 */
// ==========================================================================
extern void GsMainSysSetSuspendedFlag(BOOL flag);

// ==========================================================================
// GsMainSysGetSuspendedFlag
/*!
 *	前フレームでサスペンドしていたか否かを取得
 *	
 *	@return TRUE:サスペンドした履歴があった
 *			FALSE:無い
 *	
 */
// ==========================================================================
extern BOOL GsMainSysGetSuspendedFlag(void);

// ==========================================================================
// GsMainSysSetAccelFlag
/*!
 *	ハード的に加速度センサーを使うか否かを設定します。
 *	標準は無効になっています。
 *	
 *	@param	flag	[in]	TRUE:使用	FALSE:使用しない
 */
// ==========================================================================
extern void GsMainSysSetAccelFlag(BOOL flag);
#endif // _IPHONE

// ==========================================================================
// Utility
// ==========================================================================
// ==========================================================================
// GsMainSysInfoInit
/*!
 *	メインシステム情報の初期化
 *
 *	@return	メインシステム情報構造体アドレス
 */
// ==========================================================================
extern void GsMainSysInfoInit(GSS_MAIN_SYS_INFO *gs_main);

// ==========================================================================
// 描画システム
// ==========================================================================
// ==========================================================================
// GsMainSysGetDisplayListRegistNum
/*!
 *	ディスプレイリストマネージャー 登録コマンド数取得
 *
 *	@return	登録コマンド数
 */
// ==========================================================================
inline s32 GsMainSysGetDisplayListRegistNum(void)
{
	return (_am_displaylist_manager.regist_num + _am_displaylist_manager.reg_write_num);
}

// ==========================================================================
// シェーダー
// ==========================================================================
// ==========================================================================
// GsMainSysLoadShader
/*!
 *	メインシステム情報の初期化
 *
 *	@return	メインシステム情報構造体アドレス
 */
// ==========================================================================
extern void GsMainSysLoadShader(char *file_path);

// ==========================================================================
// GsMainSysCheckLoadShaderFinished
/*!
 *	シェーダー初期化終了チェック
 *
 *	@return	TRUE : 終了
 */
// ==========================================================================
extern BOOL GsMainSysCheckLoadShaderFinished(void);

// ==========================================================================
// ゲーム関連
// ==========================================================================
// ==========================================================================
// GsGetGameLevel
/*!
 *	ゲームレベル取得
 *
 *	@return	ゲームレベル
 */
// ==========================================================================
inline GSE_GAME_LEVEL_TYPE GsGetGameLevel(void)
{
	return (g_gs_main_sys_info.level);
}

// ==========================================================================
// GsGetGameLevel
/*!
 *	ゲームレベル設定
 *
 *	@param	level	[in]	ゲームレベル
 */
// ==========================================================================
inline void GsSetGameLevel(GSE_GAME_LEVEL_TYPE level)
{
	MTM_ASSERT((u32)level < GSD_GAME_LEVEL_MAX);

	g_gs_main_sys_info.level = level;
}

// ===========================================================================
//! ゲーム終了要求発行
// ===========================================================================
inline void GsReqExit(void)
{
#if _PC
	amWinMainLoopQuit();
#elif _XBOX
	amXboxReqExit();
#elif _PS3
	amPs3ReqExit();
#elif _WII
	amWiiReqExit();
#endif
}

#if	defined(__cplusplus)
} /* extern "C" */
#endif


#endif // GS_MAIN_SYS_H_

//----- Include Files -------------------------------------------------------
