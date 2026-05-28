// =======================================================================
/*!
  @file	gmBoss4.h
  @brief ボス1

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Body.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_4_BODY_H_
#define GM_BOSS_4_BODY_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/
#include "gmBoss4.h"
#include "gmBoss4Effect.h"
#include "gmBoss4Util.h"

/*------ Macros --------------------------------------------------------*/
//############ ノード番号 #####################################################
// 本体
#if _IPHONE
#define		GMD_BOSS4_BODY_NODE_IDX_EGG_CONNECT			( 2)	//(11)		//!< エッグマン接続ノード
#define		GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE		( 2)	//(2)		//!< 本体姿勢
#define		GMD_BOSS4_BODY_NODE_IDX_BODY_BOOSTER_L		( 5)	//(2)		//!< ブースターL
#define		GMD_BOSS4_BODY_NODE_IDX_BODY_BOOSTER_R		( 8)	//(2)		//!< ブースターR
#define		GMD_BOSS4_BODY_NODE_IDX_BODY_LIGHT_L		( 9)	//(2)		//!< ライトL
#define		GMD_BOSS4_BODY_NODE_IDX_BODY_LIGHT_R		(10)	//(2)		//!< ライトR
#else
#define		GMD_BOSS4_BODY_NODE_IDX_EGG_CONNECT			(2)	//(11)		//!< エッグマン接続ノード
#define		GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE		(2)	//(2)		//!< 本体姿勢
#define		GMD_BOSS4_BODY_NODE_IDX_BODY_BOOSTER_L		(5)	//(2)		//!< ブースターL
#define		GMD_BOSS4_BODY_NODE_IDX_BODY_BOOSTER_R		(10)//(2)		//!< ブースターR
#define		GMD_BOSS4_BODY_NODE_IDX_BODY_LIGHT_L		(7)	//(2)		//!< ライトL
#define		GMD_BOSS4_BODY_NODE_IDX_BODY_LIGHT_R		(6)	//(2)		//!< ライトR
#endif // _IPHONE

#define		GMD_BOSS4_BODY_NODE_SNM_NUM					(6)				//!< SNM登録ノード数
#define		GMD_BOSS4_BODY_NODE_CNM_NUM					(1)				//!< CNM登録ノード数

/* 定義値 */
//############ 本体 ###########################################################
/* フラグ	GMS_BOSS4_BODY_WORK::flag */
#define		GMD_BOSS4_BODY_FLAG_CHAIN_DEPEND			(1 << 0)		//!< 鎖独立動作フラグ
#define		GMD_BOSS4_BODY_FLAG_INDP_ACT_FORBIDDEN		(1 << 1)		//!< エッグマン独立アクション禁止フラグ
#define		GMD_BOSS4_BODY_FLAG_INVINCIBLE				(1 << 2)		//!< 無敵状態（当たりはあるが、ライフは減らない）
#define		GMD_BOSS4_BODY_FLAG_CHAIN_DEAD				(1 << 3)		//!< 死亡フラグ（ライフが無い状態）
#define		GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE			(1 << 4)		//!< アフターバーナー有効フラグ
#define		GMD_BOSS4_BODY_FLAG_CHAIN_NODISP			(1 << 5)		//!< 鎖非表示
#define		GMD_BOSS4_BODY_FLAG_CHAIN_NOHIT				(1 << 6)		//!< 鎖攻撃矩形オフ
#define		GMD_BOSS4_BODY_FLAG_MANUAL_CHAIN_MOTION		(1 << 7)		//!< 手動鎖モーション（OBD_DISP_STOPのオフへの変更を抑制する）
#define		GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_EX		(1 << 8)		//!< アフターバーナー(第２段階)有効フラグ
#define		GMD_BOSS4_BODY_FLAG_BOSSLIGHT_ACTIVE		(1 << 9)		//!< ボスライト有効フラグ
#define		GMD_BOSS4_BODY_FLAG_ABURNER_ACTIVE_DWN		(1 << 10)		//!< 下バーナー有効フラグ

#define		GMD_BOSS4_BODY_FLAG_UP_AVOID				(1 << 11)		//!< 攻撃を避けるために上に上がる
#define		GMD_BOSS4_BODY_FLAG_DOWN_AVOID				(1 << 12)		//!< 避けた後したに降りているとき

// シグナル系（例：B2C=bodyが立ててchainがチェックするフラグ。F=eFfect)
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_BOSSLIGHT	(1 << 19)		//!< ボス機のライト

#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER_EX	(1 << 20)		//!< アフターバーナー生成通知フラグ(第２段階用)
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_THROW	(1 << 21)		//!< 投げる
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_THROW_L	(1 << 22)		//!< 投げる(左)

#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_ESCAPE	(1 << 23)		//!< 逃亡通知
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_BURNT	(1 << 24)		//!< 黒こげテクスチャへの変更通知
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2F_ABURNER		(1 << 25)		//!< アフターバーナー生成通知フラグ
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2C_SCATTER		(1 << 26)		//!< パーツ飛散通知フラグ
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2C_EFF_SW		(1 << 27)		//!< エフェクト発生通知フラグ
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_C2E_ATK_HIT		(1 << 28)		//!< ヒット通知フラグ
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_DAMAGE	(1 << 29)		//!< body->eggダメージ通知フラグ
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE	(1 << 30)		//!< body->bodyダメージ通知フラグ
#define		GMD_BOSS4_BODY_FLAG_SIGNAL_B2B_DEFEAT		(1 << 31)		//!< 死亡演出開始通知フラグ

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
// アフターバーナーのタイプ
typedef enum
{
	GME_BOSS4_BODY_ABURNER_TYPE_NONE	= 0,
	GME_BOSS4_BODY_ABURNER_TYPE_NORMAL,									//!< アフターバーナー通常タイプ
	GME_BOSS4_BODY_ABURNER_TYPE_EX,										//!< アフターバーナー第２段階
	
	GME_BOSS4_BODY_ABURNER_TYPE_MAX
} GME_BOSS4_BODY_ABURNER_TYPE;

	
//! ボス4 本体 ステート列挙型
typedef enum
{
	GME_BOSS4_BODY_STATE_NOP	= 0,									//!< 何もしない
	GME_BOSS4_BODY_STATE_START,											//!< 開始
	GME_BOSS4_BODY_STATE_PRE_ATK_NML,									//!< 通常攻撃開始処理
	GME_BOSS4_BODY_STATE_ATK_NML,										//!< 通常攻撃

	GME_BOSS4_BODY_STATE_1ST_END,										//!< 第1形態終了
	GME_BOSS4_BODY_STATE_2ND,											//!< 第2形態開始

	GME_BOSS4_BODY_STATE_DAMAGE_NML,									//!< 通常ダメージ
	GME_BOSS4_BODY_STATE_DEFEAT,										//!< 撃破
	GME_BOSS4_BODY_STATE_ESCAPE,										//!< 逃亡
	
	GME_BOSS4_BODY_STATE_MAX
} GME_BOSS4_BODY_STATE;


typedef struct tag_GMS_BOSS4_BODY_WORK	GMS_BOSS4_BODY_WORK;

//===============================================================================
//! ボス4本体ワーク
//===============================================================================
struct tag_GMS_BOSS4_BODY_WORK
{
	GMS_ENEMY_3D_WORK		ene_3d;
	
	GME_BOSS4_BODY_STATE	state;										//!< ステート
	GME_BOSS4_BODY_STATE	prev_state;									//!< 前のステート
	
	GMS_BOSS4_MGR_WORK		*mgr_work;									//!< 管理ワーク
	
	void	(*proc_update)(GMS_BOSS4_BODY_WORK*);						//!< 更新処理関数
	
	Uint32	flag;
	
	GME_BOSS4_ACT_ID		whole_act_id;								//!< ボス１全体アクションID
	
	GME_BOSS4_ACT_ID		egg_revert_mtn_id;
//	Uint16					egg_revert_mtn_id;							//!< エッグマン独立アクションからの戻り先モーション
	Uint16					reserved[1];
	
	Uint32					wait_timer;									//!< 汎用待機タイマ
	Uint32					wait_timer2;								//!< 汎用待機タイマ

	GMS_BOSS4_NODE_MATRIX	node_work;									//!< ノード管理

	GMS_BS_CMN_CNM_MGR_WORK	cnm_mgr_work;								//!< CNM管理ワーク
	Sint32					chaintop_cnm_reg_id;						//!< 鎖の付け根マトリクス操作
	
	GMS_BS_CMN_DMG_FLICKER_WORK	flk_work;								//!< ダメージ点滅ワーク
	
	GMS_BOSS4_1SHOT_TIMER	se_timer;									//!< SE用1ショットタイマ
	Uint32					se_cnt;										//!< SE再生カウント（何回も再生するときなどに利用）
	
	Sint32					move_time;									//!< 通常攻撃移動 所要時間
	Sint32					move_cnt;									//!< 通常攻撃移動 フレームカウント

	GMS_BOSS4_MOVE			move_work;									//!< 簡易移動

	GMS_BOSS4_DIRECTION		dir;										//!< 方向管理ワーク

	fx32					drift_pivot_x;								//!< ドリフト移動 軸座標X
	Angle32					drift_angle;								//!< ドリフト移動 サイン波角度
	Angle32					drift_ang_spd;								//!< ドリフト移動 サイン波角速度
	Sint32					drift_timer;								//!< ドリフト移動 タイマ（所要時間厳守のため必要）
	
	fx32					atk_nml_alt;								//!< 通常攻撃の高度
	
	VecFx32					bash_targ_pos;								//!< 叩きつけ突進目標位置
	VecFx32					bash_ret_pos;								//!< 叩きつけ戻り目標位置
	VecFx32					bash_orig_pos;								//!< 叩きつけ各種移動開始元位置
	Angle32					bash_homing_deg;							//!< 叩きつけ戻り移動時速度カーブ角度
	
	Sint32					damage_timer;								//! ダメージ中

	GMS_BOSS4_EFF_BOMB_WORK	bomb_work;									//!< 爆発処理ワーク
	GMS_BOSS4_EFF_BOMB_WORK	bomb_work2;									//!< 爆発処理ワーク(破片)
	
	//! 構成パーツのオブジェクト（自分自身も含む）
	OBS_OBJECT_WORK	*parts_objs[GME_BOSS4_PART_IDX_MAX];
	
	//! 各構成パーツの再生停滞タイマ
	GMS_BOSS4_MTN_SUSPEND_WORK	mtn_suspend[GME_BOSS4_PART_IDX_MAX];
	
	GMS_BOSS4_NOHIT_TIMER	nohit_work;
	
	GMS_CMN_FLASH_SCR_WORK	flash_work;									//!< 画面フラッシュ用

	Sint32					avoid_timer;								//!< 上で避けている時間
	fx32					avoid_yspd;									//!< 上昇スピード

#if defined(GMD_BOSS4_TEMPORARY)
	Uint32					no_hit_timer;
#endif /* defined(GMD_BOSS4_TEMPORARY) */
};

/*------ External Declarations -----------------------------------------*/

// =======================================================================
// GmBoss4Build
/*!
  ボス4 データ構築
 */
// =======================================================================
extern void GmBoss4BodyBuild(void);

// =======================================================================
// GmBoss4Flush
/*!
  ボス4 データ片付け
 */
// =======================================================================
extern void GmBoss4BodyFlush(void);

// =======================================================================
// GmBoss4BodyInit
/*!
  ボス１本体初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4BodyInit(GMS_EVE_RECORD_EVENT *eve_rec,
										fx32 pos_x, fx32 pos_y, u8 type);



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

#endif /* GM_BOSS_4_BODY_H_ */
