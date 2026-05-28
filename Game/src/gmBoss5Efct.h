// =======================================================================
/*!
  @file	gmBoss5Efct.h
  @brief ボスファイナル エフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5Efct.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_5_EFCT_H_
#define GM_BOSS_5_EFCT_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

// =======================================================================
// GmBoss5EfctBuild
/*!
  ボス5 エフェクト構築
  
  @note
  GmBoss5Build()から呼び出してください。
 */
// =======================================================================
extern void GmBoss5EfctBuild(void);

// =======================================================================
// GmBoss5EfctFlush
/*!
  ボス5 エフェクト片付け
  
  @note
  GmBoss5Flush()から呼び出してください。
 */
// =======================================================================
extern void GmBoss5EfctFlush(void);

// =======================================================================
// GmBoss5EfctTryStartLeakage
/*!
  漏電エフェクト開始
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
extern void GmBoss5EfctTryStartLeakage(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctEndLeakage
/*!
  漏電エフェクト終了
  
  @param body_work	[io]	本体ワーク
  @param no_vanish	[in]	消失エフェクト無効フラグ
  							(TRUE: 消失EF生成しない, FALSE: 消失EF生成する)
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
extern void GmBoss5EfctEndLeakage(GMS_BOSS5_BODY_WORK *body_work, BOOL no_vanish=FALSE);

// =======================================================================
// GmBoss5EfctStartPrelimLeakage
/*!
  予備漏電エフェクト開始
  
  @param body_work	[io]	本体ワーク
  
  @note
  漏電エフェクト前の予備動作エフェクト
 */
// =======================================================================
extern void GmBoss5EfctStartPrelimLeakage(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctEndPrelimLeakage
/*!
  予備漏電エフェクト終了
  
  @param body_work	[io]	本体ワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
extern void GmBoss5EfctEndPrelimLeakage(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctCreateWalkStepSmoke
/*!
  歩き足接地時 煙エフェクト 生成
  
  @param body_work	[io]	本体ワーク
  @param leg_type	[in]	脚タイプ
 */
// =======================================================================
extern void GmBoss5EfctCreateWalkStepSmoke(GMS_BOSS5_BODY_WORK *body_work,
										   GME_BOSS5_LEG_TYPE leg_type);

// =======================================================================
// GmBoss5EfctCreateRunStepSmoke
/*!
  走り足接地時 煙エフェクト 生成
  
  @param body_work	[io]	本体ワーク
  @param leg_type	[in]	脚タイプ
 */
// =======================================================================
extern void GmBoss5EfctCreateRunStepSmoke(GMS_BOSS5_BODY_WORK *body_work,
										  GME_BOSS5_LEG_TYPE leg_type);

// =======================================================================
// GmBoss5EfctCreateBerserkStampSmoke
/*!
  凶暴化演出時 踏み込み煙エフェクト 生成
  
  @param body_work		[io]	本体ワーク
  @param leg_type		[in]	脚タイプ
  @param spawn_delay	[in]	エフェクト発生までの遅延時間
 */
// =======================================================================
extern void GmBoss5EfctCreateBerserkStampSmoke(GMS_BOSS5_BODY_WORK *body_work,
											   GME_BOSS5_LEG_TYPE leg_type,
											   Uint32 spawn_delay);

// =======================================================================
// GmBoss5EfctCreateCrashLandingSmoke
/*!
  地球割り着地時 煙エフェクト 生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
extern void GmBoss5EfctCreateCrashLandingSmoke(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctCreateBreakingGlass
/*!
  割れガラスエフェクト 生成
  
  @param parent_obj	[io]	親オブジェクトワーク
 */
// =======================================================================
extern void GmBoss5EfctCreateBreakingGlass(OBS_OBJECT_WORK *parent_obj);

// =======================================================================
// GmBoss5EfctStartJet
/*!
  本体噴射エフェクト開始
  
  @param body_work	[io]	本体ワーク

  @note
  GmBoss5EfctEndJet()で停止してください。
 */
// =======================================================================
extern void GmBoss5EfctStartJet(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctEndJet
/*!
  本体噴射エフェクト終了
  
  @param body_work	[io]	本体ワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
extern void GmBoss5EfctEndJet(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctStartJetSmoke
/*!
  本体噴射スモークエフェクト開始
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
extern void GmBoss5EfctStartJetSmoke(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctEndJetSmoke
/*!
  本体噴射スモークエフェクト開始
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
extern void GmBoss5EfctEndJetSmoke(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctTryStartRocketLeakage
/*!
  ロケット漏電エフェクト開始
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
extern void GmBoss5EfctTryStartRocketLeakage(GMS_BOSS5_ROCKET_WORK *rkt_work);

// =======================================================================
// GmBoss5EfctEndRocketLeakage
/*!
  ロケット漏電エフェクト終了
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
extern void GmBoss5EfctEndRocketLeakage(GMS_BOSS5_ROCKET_WORK *rkt_work);

// =======================================================================
// GmBoss5EfctCreateRocketLaunch
/*!
  ロケット発射エフェクト
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
extern void GmBoss5EfctCreateRocketLaunch(GMS_BOSS5_ROCKET_WORK *rkt_work);

// =======================================================================
// GmBoss5EfctCreateRocketDock
/*!
  ロケットドッキングエフェクト
  
  @param body_work	[io]	本体ワーク
  @param rkt_type	[in]	ロケットタイプ
  
  @note
  本体オブジェクトが親として設定されます。
 */
// =======================================================================
extern void GmBoss5EfctCreateRocketDock(GMS_BOSS5_BODY_WORK *body_work, GME_BOSS5_RKT_TYPE rkt_type);

// =======================================================================
// GmBoss5EfctStartRocketJet
/*!
  ロケット噴射エフェクト開始
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
extern void GmBoss5EfctStartRocketJet(GMS_BOSS5_ROCKET_WORK *rkt_work);

// =======================================================================
// GmBoss5EfctEndRocketJet
/*!
  ロケット噴射エフェクト終了
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
extern void GmBoss5EfctEndRocketJet(GMS_BOSS5_ROCKET_WORK *rkt_work);

// =======================================================================
// GmBoss5EfctStartRocketJetReverse
/*!
  ロケット逆噴射エフェクト開始
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
extern void GmBoss5EfctStartRocketJetReverse(GMS_BOSS5_ROCKET_WORK *rkt_work);

// =======================================================================
// GmBoss5EfctEndRocketJetReverse
/*!
  ロケット逆噴射エフェクト終了
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
extern void GmBoss5EfctEndRocketJetReverse(GMS_BOSS5_ROCKET_WORK *rkt_work);

// =======================================================================
// GmBoss5EfctCreateRocketLandingShockwave
/*!
  ロケット着地衝撃波エフェクト生成
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
extern void GmBoss5EfctCreateRocketLandingShockwave(GMS_BOSS5_ROCKET_WORK *rkt_work);

// =======================================================================
// GmBoss5EfctCreateLandingShockwave
/*!
  着地衝撃波生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
extern void GmBoss5EfctCreateLandingShockwave(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctCreateStrikeShockwave
/*!
  地面突き衝撃波
  
  @param body_work		[io]	本体ワーク
  @param spawn_delay	[in]	エフェクト発生までの遅延時間
 */
// =======================================================================
extern void GmBoss5EfctCreateStrikeShockwave(GMS_BOSS5_BODY_WORK *body_work, Uint32 spawn_delay);

// =======================================================================
// GmBoss5EfctTargetCursorInit
/*!
  ターゲットカーソルエフェクト初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
extern void GmBoss5EfctTargetCursorInit(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctCrashCursorInit
/*!
  地球割り用ターゲットカーソル初期化
  
  @param body_work		[io]	本体ワーク
  @param pos_x			[in]	停止目標X位置
  @param duration_time	[in]	継続時間
 */
// =======================================================================
extern void GmBoss5EfctCrashCursorInit(GMS_BOSS5_BODY_WORK *body_work,
									   fx32 pos_x, Uint32 duration_time);

// =======================================================================
// GmBoss5EfctCreateVulcanFire
/*!
  バルカン発射エフェクト初期化
  
  @param trt_work	[io]	砲塔ワーク
  @param pos		[in]	発射位置
  @param angle		[in]	発射方向
 */
// =======================================================================
extern void GmBoss5EfctCreateVulcanFire(GMS_BOSS5_TURRET_WORK *trt_work,
										const VecFx32 *pos, Angle32 angle);

// =======================================================================
// GmBoss5EfctCreateVulcanBullet
/*!
  バルカン弾エフェクト初期化
  
  @param trt_work	[io]	砲塔ワーク
  @param pos		[in]	発射位置
  @param angle		[in]	発射方向
  @param spd		[in]	移動速度
 */
// =======================================================================
extern void GmBoss5EfctCreateVulcanBullet(GMS_BOSS5_TURRET_WORK *trt_work,
										  const VecFx32 *pos, Angle32 angle, fx32 spd);

// =======================================================================
// GmBoss5EfctCreateSmallExplosion
/*!
  小爆発生成
  
  @param pos_x	[in]	生成座標X
  @param pos_y	[in]	生成座標Y
  @param pos_z	[in]	生成座標Z
 */
// =======================================================================
extern void GmBoss5EfctCreateSmallExplosion(fx32 pos_x, fx32 pos_y, fx32 pos_z);

// =======================================================================
// GmBoss5EfctCreateBigExplosion
/*!
  大爆発生成
  
  @param pos_x	[in]	生成座標X
  @param pos_y	[in]	生成座標Y
  @param pos_z	[in]	生成座標Z
 */
// =======================================================================
extern void GmBoss5EfctCreateBigExplosion(fx32 pos_x, fx32 pos_y, fx32 pos_z);

// =======================================================================
// GmBoss5EfctCreateFragments
/*!
  破片エフェクト生成
  
  @param pos_x	[in]	生成座標X
  @param pos_y	[in]	生成座標Y
  @param pos_z	[in]	生成座標Z
 */
// =======================================================================
extern void GmBoss5EfctCreateFragments(fx32 pos_x, fx32 pos_y, fx32 pos_z);

// =======================================================================
// GmBoss5EfctCreateDamage
/*!
  ダメージエフェクト 生成
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
extern void GmBoss5EfctCreateDamage(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctStartRocketSmoke
/*!
  ロケット黒煙エフェクト 開始
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
extern void GmBoss5EfctStartRocketSmoke(GMS_BOSS5_ROCKET_WORK *rkt_work);

// =======================================================================
// GmBoss5EfctEndRocketSmoke
/*!
  ロケット黒煙エフェクト 終了
  
  @param rkt_work	[io]	ロケットワーク
  
  @note
  消去を開始します。直ちには消えません。
 */
// =======================================================================
extern void GmBoss5EfctEndRocketSmoke(GMS_BOSS5_ROCKET_WORK *rkt_work);

// =======================================================================
// GmBoss5EfctBreakdownSmokesInit
/*!
  機能停止黒煙エフェクト
  
  @param body_work		[io]	本体ワーク
  @param duration_time	[in]	継続時間
  
  @note
  duration_time経過後に自動的に消えます。
 */
// =======================================================================
extern void GmBoss5EfctBreakdownSmokesInit(GMS_BOSS5_BODY_WORK *body_work,
										  Uint32 duration_time);

// =======================================================================
// GmBoss5EfctBodySmallSmokesInit
/*!
  本体 小さい黒煙エフェクト 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
extern void GmBoss5EfctBodySmallSmokesInit(GMS_BOSS5_BODY_WORK *body_work);

// =======================================================================
// GmBoss5EfctBerserkSteamInit
/*!
  凶暴スチームエフェクト
  
  @param body_work	[io]	本体ワーク
 */
// =================================================================;======
extern void GmBoss5EfctBerserkSteamInit(GMS_BOSS5_BODY_WORK *body_work, Uint32 count);

// =======================================================================
// GmBoss5EfctStartEggSweat
/*!
  エッグマン 汗エフェクト開始
  
  @param egg_work	[io]	エッグマンワーク
 */
// =======================================================================
extern void GmBoss5EfctStartEggSweat(GMS_BOSS5_EGG_WORK *egg_work);

// =======================================================================
// GmBoss5EfctEndEggSweat
/*!
  エッグマン 汗エフェクト終了
  
  @param egg_work	[io]	エッグマンワーク
 */
// =======================================================================
extern void GmBoss5EfctEndEggSweat(GMS_BOSS5_EGG_WORK *egg_work);

// =======================================================================
// GmBoss5EfctCreateRocketRollSpark
/*!
  ロケット 回転火花エフェクト生成
  
  @param rkt_work	[io]	ロケットワーク
 */
// =======================================================================
extern void GmBoss5EfctCreateRocketRollSpark(GMS_BOSS5_ROCKET_WORK *rkt_work);

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

#endif /* GM_BOSS_5_EFCT_H_ */
