// ==========================================================================
/*!
  @file gmPlySeqDat.h
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlySeqDat.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_PLY_SEQ_DAT_H_
#define GM_PLY_SEQ_DAT_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
// ==========================================================================
// シーケンスタイプ
// ==========================================================================
/// シーケンスタイプ
typedef enum tag_GME_PLY_SEQ_STATE {
	// 通常シーケンス
	GME_PLY_SEQ_STATE_FW	= 0,		//!< SEQ : FW
	GME_PLY_SEQ_STATE_WALK,				//!< SEQ : 歩き
	GME_PLY_SEQ_STATE_TURN,				//!< SEQ : 振り向き
	GME_PLY_SEQ_STATE_LOOKUP_ST,		//!< SEQ : 見上げ 開始
	GME_PLY_SEQ_STATE_LOOKUP_M,			//!< SEQ : 見上げ 中
	GME_PLY_SEQ_STATE_LOOKUP_END,		//!< SEQ : 見上げ 終了
	GME_PLY_SEQ_STATE_SQUAT_ST,			//!< SEQ : しゃがみ 開始
	GME_PLY_SEQ_STATE_SQUAT_M,			//!< SEQ : しゃがみ 中
	GME_PLY_SEQ_STATE_SQUAT_END,		//!< SEQ : しゃがみ 終了
	GME_PLY_SEQ_STATE_BRAKE,			//!< SEQ : ブレーキ
	GME_PLY_SEQ_STATE_SPIN,				//!< SEQ : スピン

	GME_PLY_SEQ_STATE_SPIN_DASHACC,		//!< SEQ : スピンダッシュ加速
	GME_PLY_SEQ_STATE_SPIN_DASH,		//!< SEQ : スピンダッシュ

	GME_PLY_SEQ_STATE_STAGGER_F,		//!< SEQ : よろける 前		// STAGGER類範囲指定あり
	GME_PLY_SEQ_STATE_STAGGER_B,		//!< SEQ : よろける 後ろ
	GME_PLY_SEQ_STATE_STAGGER_D,		//!< SEQ : よろける 危険
	GME_PLY_SEQ_STATE_FALL,				//!< SEQ : 落下
	GME_PLY_SEQ_STATE_JUMP,				//!< SEQ : ジャンプ

	GME_PLY_SEQ_STATE_WALLPUSH,			//!< SEQ : 壁押し
	GME_PLY_SEQ_STATE_HOMING,			//!< SEQ : ホーミング
	GME_PLY_SEQ_STATE_HOMING_REF,		//!< SEQ : ホーミングアタック跳ね返り
	GME_PLY_SEQ_STATE_JUMPDASH,			//!< SEQ : ジャンプダッシュ

	GME_PLY_SEQ_STATE_DAMAGE,			//!< SEQ : ダメージ
	GME_PLY_SEQ_STATE_DEATH,			//!< SEQ : 死亡

	GME_PLY_SEQ_STATE_TRANS_SUPER,		//!< SEQ : スーパーソニック化

	GME_PLY_SEQ_STATE_BOSS_GOAL,		//!< SEQ : ボスゴール(カプセル開放後)

	GME_PLY_SEQ_STATE_BOSS5_DEMO,		//!< SEQ : ボスFINAL演出FW

	GME_PLY_SEQ_STATE_T_RETRY_FW,		//!< SEQ : タイムアタックリトライ FW
	GME_PLY_SEQ_STATE_T_RETRY_ACC,		//!< SEQ : タイムアタックリトライ その場加速

	GME_PLY_SEQ_STATE_NORMAL_NUM,
	GME_PLY_SEQ_STATE_NORMAL_END = GME_PLY_SEQ_STATE_NORMAL_NUM,		//!< 通常シーケンス終了
	
	// 以下ギミック関連
	GME_PLY_SEQ_STATE_GMK_START = GME_PLY_SEQ_STATE_NORMAL_END,
	GME_PLY_SEQ_STATE_GMK_SPRINGJUMP = GME_PLY_SEQ_STATE_GMK_START,		//!< SEQ : スプリングジャンプ
	GME_PLY_SEQ_STATE_GMK_ROCK_RIDE_START,	//!< SEQ : 岩乗り開始
	GME_PLY_SEQ_STATE_GMK_ROCK_RIDE,		//!< SEQ : 岩乗り
	GME_PLY_SEQ_STATE_GMK_PULLEY,			//!< SEQ : 滑車掴まり
	GME_PLY_SEQ_STATE_GMK_BREATHING,		//!< SEQ : 息継ぎ
	GME_PLY_SEQ_STATE_GMK_DASHPANEL,		//!< SEQ : ダッシュパネル
	GME_PLY_SEQ_STATE_GMK_TARZAN_ROPE,		//!< SEQ : ターザンロープ
	GME_PLY_SEQ_STATE_GMK_WATER_SLIDER,		//!< SEQ : ウォータースライダー
	GME_PLY_SEQ_STATE_GMK_SPIPE,			//!< SEQ : Ｓ字パイプ
	GME_PLY_SEQ_STATE_GMK_SCREW,			//!< SEQ : コークスクリュー
	GME_PLY_SEQ_STATE_GMK_DEMO_FW,			//!< SEQ : デモ用フットワーク

	GME_PLY_SEQ_STATE_GMK_STOPPER,			//!< SEQ : ストッパー＠ピンボール

	GME_PLY_SEQ_STATE_GMK_CANNON,			//!< SEQ : 大砲
	GME_PLY_SEQ_STATE_GMK_CANNON_SHOOT,		//!< SEQ : 大砲発射

	GME_PLY_SEQ_STATE_GMK_UPBUMPER,			//!< SEQ : アップバンパー

	GME_PLY_SEQ_STATE_GMK_SEESAW,			//!< SEQ : シーソー

	GME_PLY_SEQ_STATE_GMK_PINBALL,			//!< SEQ : ピンボール
	GME_PLY_SEQ_STATE_GMK_PINBALL_AIR,		//!< SEQ : ピンボール（空中）	
	GME_PLY_SEQ_STATE_GMK_FLIPPER,			//!< SEQ : フリッパー	
	GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_HOLD,	//!< SEQ : スプリングカタパルト確保
	GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_UP,	//!< SEQ : スプリングカタパルト↑
	GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_LR,	//!< SEQ : スプリングカタパルト↑以外

	GME_PLY_SEQ_STATE_GMK_FORCESPIN,		//!< SEQ : 強制スピンモード
	GME_PLY_SEQ_STATE_GMK_FORCESPIN_DEC,	//!< SEQ : 強制スピンモード減速タイプ
	GME_PLY_SEQ_STATE_GMK_FORCESPIN_FALL,	//!< SEQ : 強制スピンモード落下

	GME_PLY_SEQ_STATE_GMK_MOVE_GEAR,		//!< SEQ : 移動歯車
	
	GME_PLY_SEQ_STATE_GMK_DRAIN_TANK,		//!< SEQ : 排液装置
	GME_PLY_SEQ_STATE_GMK_DRAIN_TANK_FALL,	//!< SEQ : 排液装置（落下）

	GME_PLY_SEQ_STATE_GMK_STEAMPIPE,		//!< SEQ : スチームパイプ

	GME_PLY_SEQ_STATE_GMK_POP_STEAM,		//!< SEQ : ポップスチーム
	GME_PLY_SEQ_STATE_GMK_SPL_IN,			//!< SEQ : スペステリングＩＮ
	GME_PLY_SEQ_STATE_GMK_BOSS2_CATCH,		//!< SEQ : ボス2掴み
	
	GME_PLY_SEQ_STATE_GMK_BOSS5_QUAKE,		//!< SEQ : ボスFINAL地球割り着地振動

	GME_PLY_SEQ_STATE_GMK_ENDING_DEMO1,		//!< SEQ : エンディング 演出１
	GME_PLY_SEQ_STATE_GMK_ENDING_DEMO2,		//!< SEQ : エンディング 演出２

	GME_PLY_SEQ_STATE_GMK_TRUCK_DANGER,		//!< SEQ : トロッコ 危険状態
	GME_PLY_SEQ_STATE_GMK_TRUCK_DANGER_RET,	//!< SEQ : トロッコ 危険回避

	GME_PLY_SEQ_STATE_GMK_SPIN_FALL,		//!< SEQ : スピン状態で落下

	GME_PLY_SEQ_STATE_MAX
} GME_PLY_SEQ_STATE;


// ==========================================================================
// シーケンス遷移データ
// ==========================================================================
typedef struct tag_GMS_PLY_SEQ_STATE_DATA {
	u32	check_attr;							//!< 各種チェック属性
	u32	accept_attr;						//!< 移行可能属性
//	u32	cansel_attr;						//!< キャンセル可能属性
} GMS_PLY_SEQ_STATE_DATA;


// GMS_PLY_SEQ_STATE_DATA : check_attr チェック属性
#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_TURN					(0x00000001)	//!< 振り向きチェックを行う
#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_DIRECT_TURN			(0x00000002)	//!< 歩き中振り向き判定を行う
#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_WALK_END				(0x00000004)	//!< 歩き終了判定
#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_FALL					(0x00000008)	//!< 落下
#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_END_LOOKUP				(0x00000010)	//!< 見上げ終了
#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SQUAT				(0x00000020)	//!< しゃがみ終了
//#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_END_BRAKE				(0x00000040)	//!< ブレーキ終了
#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPIN				(0x00000080)	//!< スピン終了
//#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPINACC			(0x00000100)	//!< スピン加速終了
//#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_END_SPINDASH			(0x00000200)	//!< スピンダッシュ終了
#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_END_PUSH				(0x00000400)	//!< 押し終了
//#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_END_HOMING				(0x00000800)	//!< ホーミング終了

#define	GMD_PLY_SEQ_STATE_CHECK_ATTR_STAGGER				(0x00008000)	//!< よろけ判定
// モーション速度設定処理
#define GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_NOSET		(0x00400000)	//!< モーション速度設定を設定しない
#define GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK			(0x00800000)	//!< 歩きタイプモーション速度設定

// 移動処理タイプ
#define GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_PINBALL		(0x04000000)	//!< ピンボール用スピン時移動
#define GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN_NODEC		(0x08000000)	//!< スピン減速無し移動
#define GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP_DIR			(0x10000000)	//!< ジャンプ中角度変化
#define GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_SPIN				(0x20000000)	//!< スピン時移動
#define GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_JUMP				(0x40000000)	//!< ジャンプ時移動
#define GMD_PLY_SEQ_STATE_CHECK_ATTR_MOVE_WALK				(0x80000000)	//!< 歩き移動

// GMS_PLY_SEQ_STATE_DATA : accept_attr 移行可能属性
//#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_STAND					(0x00000001)	//!< フットワーク戻り受け付け
#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALK					(0x00000002)	//!< 歩き受け付け
#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_BRAKE					(0x00000004)	//!< ブレーキ受付
#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMP					(0x00000008)	//!< ジャンプ受付
#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_HOMING				(0x00000010)	//!< ホーミング受付

#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_LOOKUP				(0x00000040)	//!< 見上げ受付
#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT					(0x00000080)	//!< しゃがみ受付
#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPIN					(0x00000100)	//!< スピン受付
#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SPINACC				(0x00000200)	//!< スピンダッシュ加速受付
#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_WALLPUSH				(0x00000400)	//!< 壁押し受付
#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_PINBALL_SPINACC		(0x00000800)	//!< ピンボール時 スピンダッシュ加速受付
#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_SQUAT_SPIN			(0x00001000)	//!< しゃがみ時スピン即時受け付け

//#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_JUMPDASH				(0x00000020)	//!< 空中ダッシュ受付
//#define	GMD_PLY_SEQ_STATE_ACCEPT_ATTR_THROUGH				(0x00010000)	//!< すり抜け落下受付



//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------
extern const GMS_PLY_SEQ_STATE_DATA g_gm_ply_seq_state_data_tbl[GSD_CHAR_ID_MAX][GME_PLY_SEQ_STATE_MAX];	//!< シーケンス設定データ

//----- External Declarations -----------------------------------------------

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _PT_H_

//----- Include Files -------------------------------------------------------
