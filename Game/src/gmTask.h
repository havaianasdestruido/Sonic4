// ================================================================
/*!
  @file	gmTask.h
  @brief HOG タスク設定

  @author Ishizaki
                Copyright(c) 2009 Dimps

  $Id: gmTask.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */

#ifndef GM_TASK_H_
#define GM_TASK_H_



//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------
// =====================================================================
// タスクポーズレベル
// =====================================================================
#define GMD_TASK_PAUSELEVEL_DEF		(0)			//!< ポーズレベルディフォルト
#define GMD_TASK_GAME_PAUSE_LEVEL	(2)			//!< ゲームポーズレベル
#define GMD_TASK_NO_GAME_PAUSE		(3)			//!< ゲームポーズ無効

#define GMD_TASK_PAUSE_LEVEL_OBJSYS	(GMD_TASK_PAUSELEVEL_DEF)	//!< オブジェクトシステム
#define GMD_TASK_PAUSE_LEVEL_CAMERA	(GMD_TASK_PAUSELEVEL_DEF)	//!< カメラ

#define GMD_TASK_PAUSE_LEVEL_EVTMGR	(GMD_TASK_PAUSELEVEL_DEF)	//!< イベントマネージャー

#define GMD_TASK_PAUSE_LEVEL_DEMO	(GMD_TASK_GAME_PAUSE_LEVEL)	//!< ポーズメニューからのデモ起動中
// =====================================================================
// タスクプライオリティ
// =====================================================================
#define GMD_TASK_PRIO_OBJSYS		(MTD_TASK_PRIORITY_TAIL - 4)	//!< オブジェクトシステム


#define GMD_TASK_PRIO_INIT			( 0x0010 )	//!< 初期化(終了処理その他)処理優先
#define GMD_TASK_PRIO_MAIN_PRE		( 0x1000 )	//!< 前処理優先
#define GMD_TASK_PRIO_CAMERA		( 0x2000 )	//!< カメラ
#define GMD_TASK_PRIO_MAIN_POST		( 0x8000 )	//!< 後処理優先
//#define GMD_TASK_PRIO_PLAYDEMO_MGR	( 0x0FFF )	//!< プレイデモマネージャー
//#define GMD_TASK_PRIO_CONT_RING_MGR	( 0x0FFF )	//!< リング対戦マネージャー
//#define GMD_TASK_PRIO_REPLAY_MGR	( 0x0FFF )	//!< タイムアタックリプレイマネージャー
//#define GMD_TASK_PRIO_KEYREC_PLAY	( GMD_TASK_PRIO_PLAYER-1 )	//!< キーレコード再生時 (プレイヤー直前で再生)
//#define GMD_TASK_PRIO_KEYREC_REC	( GMD_TASK_PRIO_PLAYER+1 )	//!< キーレコード記録時 (プレイヤー直後で記録)
#define GMD_TASK_PRIO_DATA_LOAD		( 0x0800 )	//!< データ読み込み


#define GMD_TASK_PRIO_MAP			( 0x3000 )	//!< マップマネージャ
#define GMD_TASK_PRIO_MAPFAR		( 0x3100 )	//!< 遠景マネージャ
//#define GMD_TASK_PRIO_MAP_V			( 0xF200 )	//!< マップV中処理
#define GMD_TASK_PRIO_EVTMGR		( 0x1090 )	//!< イベントマネージャ
#define GMD_TASK_PRIO_PLAYER		( 0x1100 )	//!< プレイヤー
#define GMD_TASK_PRIO_ENEMY			( 0x1500 )	//!< エネミー
#define GMD_TASK_PRIO_GIMMICK		( 0x1800 )	//!< ギミック
#define GMD_TASK_PRIO_GIMMICK_R		( GMD_TASK_PRIO_PLAYER - 10 )	//!< 乗れるギミック
#define GMD_TASK_PRIO_EFFECT		( 0x1a00 )	//!< エフェクト
#define GMD_TASK_PRIO_EFFECT_MAX	( 0x1b00 )	//!< エフェクト最大プライオリティ（エフェクト描画の順序付けのため、複数の優先度を使用します）
#define GMD_TASK_PRIO_EFFECT_SERVER	( 0x5000 )	//!< エフェクトサーバー（エフェクトの削除を行います。）
#define GMD_TASK_PRIO_TRAIL_SYS		( 0x2100 )	//!< TRAILシステム (描画を含むのでカメラより後にする)
#define GMD_TASK_PRIO_DECORATION	( 0x1500 )	//!< 装飾
#define GMD_TASK_PRIO_RING			( 0x1E00 )	//!< リング
#define GMD_TASK_PRIO_WATER_SURFACE	( GMD_TASK_PRIO_CAMERA + 10 )	//!< 水面（カメラの後）
#define GMD_TASK_PRIO_FIX			( 0x4800 )	//!< FIX
#define GMD_TASK_PRIO_SCORE			( 0x4800 )	//!< スコア表示
#define GMD_TASK_PRIO_CLEARDEMO		( 0x4810 )	//!< クリアデモ
#define GMD_TASK_PRIO_STARTDEMO		( 0x4810 )	//!< スタートデモ
#define GMD_TASK_PRIO_GAMEOVER		( 0x4820 )	//!< ゲーム／タイムオーバー
#define GMD_TASK_PRIO_START_MSG		( 0x4850 )	//!< ゲーム開始時メッセージ

#define GMD_TASK_PRIO_DATALOAD_WAIT	( 0x1000 )	//!< データロード待機処理
//#define GMD_TASK_PRIO_OBJECT		( 0x2000 )	//!< その他オブジェクト
#define GMD_TASK_PRIO_PAUSE			( 0x7100 )	//!< ポーズタスク
#define GMD_TASK_PRIO_PAD_VIB		( 0x1080 )	//!< パッド振動(各種オブジェクトより早く)

//#define GMD_TASK_PRIO_CAMERA3D		( 0x10A0 )//( 0x3F00 )	//!< 3Dカメラ（プレイヤーの位置が確定した後に処理すること）
//#define GMD_TASK_PRIO_MAP_POST		( 0x4F10 )	//!< マップ末尾処理（HDMA、パレット転送リクエスト等）
#define GMD_TASK_PRIO_SOUND			( 0x7fff )	//!< サウンド
// カメラマン（カメラタスクより優先を高くすること）
//#define GMD_TASK_PRIO_CAMERAMAN		( GMD_TASK_PRIO_CAMERA3D - 1 )
// 開始演出（カメラ関係を操作しているためカメラよりも優先を高くすること）
//#define GMD_TASK_PRIO_STDM			( GMD_TASK_PRIO_CAMERAMAN - 1 )
// 終了演出
//#define GMD_TASK_PRIO_CLDM			(GMD_TASK_PRIO_STDM)
// チュートリアル
//#define GMD_TASK_PRIO_TUTORIAL_MGR	( 0x10F0 )	//!< チュートリアルマネージャー

// =====================================================================
// タスクグループタイプ
// =====================================================================
//  この中から１つだけ選んで使用すること
//  またこのタイプを増やした場合はゲーム終了処理でも対応すること
// グループは消去順に並べておく事
enum {
	GMD_TASK_GROUP_NO_START	= 1,

	GMD_TASK_GROUP_NO_PLAYER	= GMD_TASK_GROUP_NO_START,	//!< プレイヤー関連
	GMD_TASK_GROUP_NO_ENEMY,							//!< 敵, ギミック関連
	GMD_TASK_GROUP_NO_DECO,								//!< 消えてもゲームに支障を及ぼさない処理
	GMD_TASK_GROUP_NO_OBJSYS,							//!< オブジェクトシステム(データ管理上 オブジェクトをすべて削除してから)
	GMD_TASK_GROUP_NO_GAMESYS,							//!< ゲームシステム関連

	GMD_TASK_GROUP_NO_END								//!< 通常範囲外(ゲーム終了時に終了してしまわないものを含む)
};

// =====================================================================
// タスクグループ
// =====================================================================
#define GMD_TASK_GROUP_OBJSYS		( GMD_TASK_GROUP_NO_OBJSYS )	//!< オブジェクトシステム

#define GMD_TASK_GROUP_DATA_LOAD	( GMD_TASK_GROUP_NO_GAMESYS )	//!< データ読み込み

#define GMD_TASK_GROUP_PLAYER		( GMD_TASK_GROUP_NO_PLAYER )	//!< プレイヤー
#define GMD_TASK_GROUP_ENEMY		( GMD_TASK_GROUP_NO_ENEMY )		//!< エネミー
#define GMD_TASK_GROUP_BOSS			( GMD_TASK_GROUP_NO_ENEMY )		//!< ボス
#define GMD_TASK_GROUP_GIMMICK		( GMD_TASK_GROUP_NO_ENEMY )		//!< ギミック
#define GMD_TASK_GROUP_DECO_SYS		( GMD_TASK_GROUP_NO_GAMESYS )	//!< 装飾管理システム
#define GMD_TASK_GROUP_EFFECT		( GMD_TASK_GROUP_NO_DECO )		//!< エフェクト
#define GMD_TASK_GROUP_EFFECT_SERVER	(GMD_TASK_GROUP_NO_GAMESYS)	//!< エフェクトサーバー
#define GMD_TASK_GROUP_TRAIL_SYS	( GMD_TASK_GROUP_NO_DECO )		//!< TRAILシステム
#define GMD_TASK_GROUP_RING_SYS		( GMD_TASK_GROUP_NO_GAMESYS )	//!< リングシステム
//#define GMD_TASK_GROUP_OBJECT		( GMD_TASK_GROUP_NO_ENEMY )	//!< その他オブジェクト
#define GMD_TASK_GROUP_FIX			( GMD_TASK_GROUP_NO_GAMESYS )	//!< FIX
#define GMD_TASK_GROUP_SCORE		( GMD_TASK_GROUP_NO_GAMESYS )	//!< スコア表示
#define GMD_TASK_GROUP_CLEARDEMO	( GMD_TASK_GROUP_NO_GAMESYS )	//!< CLEARDEMO
#define GMD_TASK_GROUP_STARTDEMO	( GMD_TASK_GROUP_NO_GAMESYS )	//!< STARTDEMO
#define GMD_TASK_GROUP_GAMEOVER		( GMD_TASK_GROUP_NO_GAMESYS )	//!< ゲーム／タイムオーバー
#define GMD_TASK_GROUP_PAUSE		( GMD_TASK_GROUP_NO_END )		//!< ポーズ	ゲーム終了時に一緒に終了してしまわない為に通常範囲外で設定
//#define GMD_TASK_GROUP_GAMEMAIN1	( GMD_TASK_GROUP_NO_GAMESYS )	//!< ゲームメイン先頭処理
//#define GMD_TASK_GROUP_GAMEMAIN2	( GMD_TASK_GROUP_NO_GAMESYS )	//!< ゲームメイン末尾処理
#define GMD_TASK_GROUP_MAP			( GMD_TASK_GROUP_NO_GAMESYS )	//!< マップマネージャ
#define GMD_TASK_GROUP_MAPFAR		( GMD_TASK_GROUP_NO_GAMESYS )	//!< マップマネージャ
#define GMD_TASK_GROUP_EVTMGR		( GMD_TASK_GROUP_NO_GAMESYS )	//!< イベントマネージャ
#define GMD_TASK_GROUP_START_MSG	( GMD_TASK_GROUP_NO_GAMESYS )	//!< ゲーム開始時メッセージ

#define GMD_TASK_GROUP_MAIN_PRE		( GMD_TASK_GROUP_NO_GAMESYS )	//!< ゲーム前処理
#define GMD_TASK_GROUP_MAIN_POST	( GMD_TASK_GROUP_NO_GAMESYS )	//!< ゲーム後処理
#define GMD_TASK_GROUP_OBJ_SYS		( GMD_TASK_GROUP_NO_OBJSYS )	//!< オブジェクトシステム
#define GMD_TASK_GROUP_SOUND		( GMD_TASK_GROUP_NO_GAMESYS )	//!< サウンド
#define GMD_TASK_GROUP_PAD_VIB		( GMD_TASK_GROUP_NO_GAMESYS)	//!< パッド振動
//#define GMD_TASK_GROUP_PLAYDEMO		( GMD_TASK_GROUP_NO_GAMESYS )	//!< プレイデモ
//#define GMD_TASK_GROUP_CONT_RING	( GMD_TASK_GROUP_NO_GAMESYS )	//!< リング対戦マネージャー
//#define GMD_TASK_GROUP_REPLAY_MGR	( GMD_TASK_GROUP_NO_GAMESYS )	//!< タイムアタックリプレイマネージャー



#if 0
// アクション優先
#define GMD_ACT_PRIO_SYSYTEM			(  1 )	//!< システム関連(最優先表示項目)
#define GMD_ACT_PRIO_TRICK_FIX			(  5 )	//!< トリック演出
#define GMD_ACT_PRIO_EFFECT_FIX			(  6 )	//!< エフェクトFIX
#define GMD_ACT_PRIO_RING				( 11 )	//!< リング
#define GMD_ACT_PRIO_EFFECT				( 12 )	//!< エフェクト
#define GMD_ACT_PRIO_PLAYER				( 13 )	//!< プレイヤー
#define GMD_ACT_PRIO_EFFECT_BACK		( 14 )	//!< エフェクト プレイヤー後ろ
//#define GMD_ACT_PRIO_DECORATION_FRONT	(  9 )	//!< 手前(A面前)装飾(5 ～ 7 を使用)
#define GMD_ACT_PRIO_DECORATION			( 25 )	//!< 背景(B面前)装飾(21 ～ 23 を使用)
#define GMD_ACT_PRIO_ENEMY				( 23 )	//!< 敵
#define GMD_ACT_PRIO_GIMMICK			( 23 )	//!< ギミック
#define GMD_ACT_PRIO_ITEM				( 24 )	//!< アイテム
#endif


// デバック用タスクカラー
#define GMD_DEBUG_TASK_COLOR_MAIN_PRE	(GX_RGB(16, 16, 16))	//!< メインPRE処理カラー
#define GMD_DEBUG_TASK_COLOR_MAIN_POST	(GX_RGB(16, 16, 16))	//!< メインPOST処理カラー


//----- External Declarations ------------------------------------------

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_TASK_H_
