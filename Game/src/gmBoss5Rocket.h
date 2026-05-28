// =======================================================================
/*!
  @file	gmBoss5Rocket.h
  @brief ボスファイナル ロケットパンチ
  
  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5Rocket.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_5_ROCKET_H_
#define GM_BOSS_5_ROCKET_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/
//############ ロケット #######################################################
/* フラグ */
#define GMD_BOSS5_RKT_FLAG_HIT_DONE			(1 << 0)				//!< プレイヤーにヒット済みフラグ
#define GMD_BOSS5_RKT_FLAG_GROUND_STUCK		(1 << 1)				//!< 地面突き刺さり状態フラグ
#define GMD_BOSS5_RKT_FLAG_LEAKAGE_ACTIVE	(1 << 2)				//!< ロケット漏電エフェクト有効中フラグ（オフにするとエフェクトが消える。直接書き換え禁止）
#define GMD_BOSS5_RKT_FLAG_LEAKAGE_NEEDED	(1 << 3)				//!< ロケット漏電エフェクト必要状態フラグ（オンにすると漏電エフェクトが生成され、オフにすると消去される）
#define GMD_BOSS5_RKT_FLAG_JET_NML_ACTIVE	(1 << 4)				//!< ロケット噴射エフェクト有効中フラグ（オフにするとエフェクトが消える。直接書き換え禁止）
#define GMD_BOSS5_RKT_FLAG_JET_REV_ACTIVE	(1 << 5)				//!< ロケット逆噴射エフェクト有効中フラグ（オフにするとエフェクトが消える。直接書き換え禁止）
#define GMD_BOSS5_RKT_FLAG_SMOKE_ACTIVE		(1 << 6)				//!< ロケット黒煙エフェクト有効中フラグ（オフにするとえふぇくおtが消える。直接書き換え禁止）
#define GMD_BOSS5_RKT_FLAG_IS_DOWN			(1 << 7)				//!< 倒れ状態フラグ
#define GMD_BOSS5_RKT_FLAG_DOWN_VIB_DONE	(1 << 8)				//!< 倒れ状態になったときの振動処理済みフラグ
/* 定義値 */
#define GMD_BOSS5_RKT_PLY_SEARCH_HIST_NUM	(21)					//!< サーチ履歴記録可能数

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

//! ボス５ ロケットタイプ列挙型
typedef enum
{
	GME_BOSS5_RKT_TYPE_LEFT	= 0,
	GME_BOSS5_RKT_TYPE_RIGHT,
	
	GME_BOSS5_RKT_TYPE_MAX
} GME_BOSS5_RKT_TYPE;

//! ボス５ロケットワーク
typedef struct tag_GMS_BOSS5_ROCKET_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	void (*proc_update)(struct tag_GMS_BOSS5_ROCKET_WORK*);
	
	Uint32				flag;
	
	GME_BOSS5_RKT_TYPE	rkt_type;		//!< ロケットタイプ
	
	Uint32				wait_timer;		//!< 汎用待機タイマ
	
	Uint32				hit_count;		//!< ヒット数カウント
	
	Uint32				no_hit_timer;	//!< ヒット無効タイマ
	
	Uint32				wfall_atk_toggle_timer;	//!< 弱体落下時の攻撃当たりオン・オフタイマ
	
	Angle32				move_dir;		//!< 移動方向
	fx32				acc;			//!< 加速度（スカラ値）
	fx32				max_spd;		//!< 最高速度
	VecFx32				launch_pos;		//!< 発射開始位置
	VecFx32				dest_pos;		//!< 目標位置（折り返し位置）
	
	VecFx32				rvs_acc;		//!< 逆噴射加速ベクトル
	
	Angle32				rot_spd;		//!< 回転角速度（吹っ飛びや跳ね返り時に回転アニメーションさせるために使用）
	
	Angle32				stuck_dir;		//!< 突き刺さり時角度保存
	Float				stuck_lean_ratio;	//!< 突き刺さり時傾き変化補間係数
	Float				hit_vib_amp_deg;	//!< ヒット振動振幅角度
	Angle32				hit_vib_sin_angle;	//!< ヒット振動波形用角度パラメータ
	
	Angle32				pivot_fall_angle;	//!< 落下時の基本角度
	Angle32				wobble_sin_param_angle;	//!< 落下時のふらつき用サイン波のパラメータ角度
	
	NNS_QUATERNION		sct_cur_quat;	//!< 飛散用 現在の姿勢
	NNS_QUATERNION		sct_spin_quat;	//!< 差分回転クォータニオン
	
	Sint32				arm_snm_id;		//!< 腕を接続するノード（本体側）のSNM登録ID
	
	GMS_BS_CMN_BMCB_MGR	bmcb_mgr;		//!< ボスモーションCB管理ワーク
	GMS_BS_CMN_SNM_WORK	snm_work;		//!< SNMワーク
	//Sint32			***_snm_reg_id;
	Sint32				drill_snm_reg_id;	//!< ドリルSNM登録ID
	
	VecFx32				pivot_prev_pos;	//!< ノード追随・矩形追随 相対配置用の基準座標（前フレーム）
	
	NNS_QUATERNION		stuck_lerp_src_quat;	//!< 本体腕ノード追随の、回転無視→回転反映への移行補間 初期姿勢
	Float				stuck_lerp_ratio;		//!< 本体腕ノード追随の、回転反映移行補間の進捗度合い
	Float				stuck_lerp_ratio_spd;	//!< 本体腕ノード追随の、回転反映移行補間の進捗速度
	
	GMS_BS_CMN_DELAY_SEARCH_WORK	dsearch_work;	//!< 遅延サーチワーク
	VecFx32				search_hist_buf[GMD_BOSS5_RKT_PLY_SEARCH_HIST_NUM];	//!< 履歴記録バッファ
	Sint32				ply_search_delay;	//!< プレイヤーサーチ遅延フレーム
} GMS_BOSS5_ROCKET_WORK;

/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GmBoss5RocketInit
/*!
  ボスFINAL ロケット（ロケットパンチの弾）初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ（0: 通常時, 1: 凶暴時）
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss5RocketInit(GMS_EVE_RECORD_EVENT *eve_rec,
										  fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss5RocketLaunchNormal
/*!
  通常ロケット発射
  
  @param body_work		[io]	本体ワーク
  @param rkt_type		[in]	ロケットタイプ(GME_BOSS5_RKT_TYPE_XXX)
  
  @return ロケットワーク
  
  @note
  ボス通常時版のロケットパンチを発射します。
 */
// =======================================================================
extern GMS_BOSS5_ROCKET_WORK* GmBoss5RocketLaunchNormal(GMS_BOSS5_BODY_WORK *body_work,
														GME_BOSS5_RKT_TYPE rkt_type);

// =======================================================================
// gmBoss5RocketLaunchStrong
/*!
  強化ロケット発射
  
  @param body_work		[io]	本体ワーク
  @param rkt_type		[in]	ロケットタイプ(GME_BOSS5_RKT_TYPE_XXX)
  
  @return ロケットワーク
  
  @note
  ボス凶暴時版のロケットパンチを発射します。
 */
// =======================================================================
extern GMS_BOSS5_ROCKET_WORK* GmBoss5RocketLaunchStrong(GMS_BOSS5_BODY_WORK *body_work,
														GME_BOSS5_RKT_TYPE rkt_type);

// =======================================================================
// GmBoss5RocketSpawnConnected
/*!
  本体接続ロケット 生成
  
  @param body_work	[io]	本体ワーク
  @param rkt_type	[in]	ロケットタイプ(GME_BOSS5_RKT_TYPE_XXX)
  
  @return ロケットワーク
 
  @note
  ボス本体に接続している状態のロケットパンチを生成します。
 */
// =======================================================================
extern GMS_BOSS5_ROCKET_WORK* GmBoss5RocketSpawnConnected(GMS_BOSS5_BODY_WORK *body_work,
														  GME_BOSS5_RKT_TYPE rkt_type);

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

#endif /* GM_BOSS_5_ROCKET_H_ */
