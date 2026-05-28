// ==========================================================================
/*!
  @file gmPlySeqGmk.h
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlySeqGmk.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_PLY_SEQ_GMK_H_
#define GM_PLY_SEQ_GMK_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
/* トロッコ 危険発生 */
#define GMD_PLY_SEQ_GMK_TRUCK_DANGER_VIB_PLAYER	(1)	//!< トロッコ危険発生 プレイヤー振動あり
#define GMD_PLY_SEQ_GMK_TRUCK_DANGER_VIB_TRUCK	(1)	//!< トロッコ危険発生 トロッコ振動あり

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// ギミックシーケンス初期化処理
// ==========================================================================
// ==========================================================================
// スプリングジャンプ GME_PLY_SEQ_STATE_GMK_SPRINGJUMP
// ==========================================================================
// ==========================================================================
// GmPlySeqGmkInitSpringJump
/*!
 *	ジャンプ 初期化
 *
 *	@param	ply_work			[in]	プレイヤーワーク
 *	@param	spd_x				[in]	速度X
 *	@param	spd_y				[in]	速度Y
 *	@param	spd_clear			[in]	以前の速度をクリア
 *	@param	no_jump_move_time	[in]	ジャンプ後の左右移動不可時間設定
 *	@param	fall_dir			[in]	トロッコ時の重力方向変更設定 -1で無効
 *	@param	t_cam_slow			[in]	トロッコ時接地までカメラ回転速度をゆっくりに
 */
// ==========================================================================
extern void GmPlySeqInitSpringJump(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y,
									BOOL spd_clear, fx32 no_jump_move_time, s32 fall_dir, BOOL t_cam_slow);

// ==========================================================================
// 岩乗り GME_PLY_SEQ_STATE_GMK_ROCK_RIDE
// ==========================================================================
// ==========================================================================
// GmPlySeqInitRockRide
/*!
 *	岩乗り 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	com_work	[in]	ギミックオブジェクトワーク GMS_ENEMY_COM_WORK
 */
// ==========================================================================
extern void GmPlySeqInitRockRideStart(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *com_work);
extern void GmPlySeqInitRockRide(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *com_work);

// ==========================================================================
// 滑車掴まり GME_PLY_SEQ_STATE_GMK_PULLEY
// ==========================================================================
// ==========================================================================
// GmPlySeqInitPulley
/*!
 *	滑車掴まり 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	com_work	[in]	ギミックオブジェクトワーク GMS_ENEMY_COM_WORK
 */
// ==========================================================================
extern void GmPlySeqInitPulley(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *com_work);

// ==========================================================================
// 息継ぎ GME_PLY_SEQ_STATE_GMK_BREATHING
// ==========================================================================
// ==========================================================================
// GmPlySeqInitBreathing
/*!
 *	息継ぎ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
extern void GmPlySeqInitBreathing(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// ダッシュパネル GME_PLY_SEQ_STATE_GMK_DASHPANEL
// ==========================================================================
// ==========================================================================
// GmPlySeqInitDashPanel
/*!
 *	ダッシュパネル 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	速度X
 *	@param	spd_y		[in]	速度Y
 */
// ==========================================================================
typedef enum tag_GME_PLYGMK_DASHPANEL_TYPE {
	GME_PLYGMK_DASHPANEL_RIGHT	= 0,		//!< 右タイプ
	GME_PLYGMK_DASHPANEL_LEFT,				//!< 左タイプ
	GME_PLYGMK_DASHPANEL_V_RIGHT,			//!< 縦右壁タイプ
	GME_PLYGMK_DASHPANEL_V_LEFT,			//!< 縦左壁タイプ

	GME_PLYGMK_DASHPANEL_MAX
} GME_PLYGMK_DASHPANEL_TYPE;
extern void GmPlySeqInitDashPanel(GMS_PLAYER_WORK *ply_work, GME_PLYGMK_DASHPANEL_TYPE type);

// ==========================================================================
// ターザンロープ GME_PLY_SEQ_STATE_GMK_TARZAN_ROPE
// ==========================================================================
// ==========================================================================
// GmPlySeqInitTarzanRope
/*!
 *	息継ぎ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	com_work	[in]	ギミックオブジェクトワーク GMS_ENEMY_COM_WORK
 */
// ==========================================================================
extern void GmPlySeqInitTarzanRope(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *com_work);

// ==========================================================================
// ウォータースライダー GME_PLY_SEQ_STATE_GMK_WATER_SLIDER
// ==========================================================================
// ==========================================================================
// GmPlySeqInitWaterSlider
/*!
 *	ウォータースライダー 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	com_work	[in]	ギミックオブジェクトワーク GMS_ENEMY_COM_WORK
 */
// ==========================================================================
extern void GmPlySeqInitWaterSlider(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *com_work);

// ==========================================================================
// Ｓ字パイプ GME_PLY_SEQ_STATE_GMK_S_PIPE
// ==========================================================================
// ==========================================================================
// GmPlySeqInitSpipe
/*!
 *	Ｓ字パイプ（スピン状態）
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
extern void GmPlySeqInitSpipe(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// コークスクリュー GME_PLY_SEQ_STATE_GMK_SCREW
// ==========================================================================
// ==========================================================================
// GmPlySeqScrewCheck
/*!
  スクリュー中かチェック
  @return TRUE スクリュー中、 FALSE スクリュー中ではない
 */
// ==========================================================================
extern u16 GmPlySeqScrewCheck( GMS_PLAYER_WORK *ply_work );

// ==========================================================================
// GmPlySeqInitScrew
/*!
 *	コークスクリュー
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	gmk_work	[in]	ギミックオブジェクトワーク GMS_ENEMY_COM_WORK
 *	@param	pos_x		[in]	ギミックX座標
 *	@param	pos_y		[in]	ギミックY座標
 *	@param	flag		[in]	イベントフラグ
 */
// ==========================================================================
extern void GmPlySeqInitScrew(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *gmk_work, fx32 pos_x, fx32 pos_y, u16 flag);

// ==========================================================================
// デモ用フットワーク GME_PLY_SEQ_STATE_GMK_DEMO_FW
// ==========================================================================
// ==========================================================================
// GmPlySeqInitDemoFw
/*!
 *	デモ用フットワーク 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
extern void GmPlySeqInitDemoFw( GMS_PLAYER_WORK *ply_work );

// ==========================================================================
// 大砲 GME_PLY_SEQ_STATE_GMK_CANNON
// ==========================================================================
// ==========================================================================
// GmPlySeqInitCannon
/*!
	大砲に吸い込まれる
 */
// ==========================================================================
extern void GmPlySeqInitCannon(GMS_PLAYER_WORK *ply_work,GMS_ENEMY_COM_WORK *gmk_work);

// ==========================================================================
// 大砲 GME_PLY_SEQ_STATE_GMK_CANNON_SHOOT
// ==========================================================================
// ==========================================================================
// GmPlySeqInitCannon
/*!
	大砲に吸い込まれる
 */
// ==========================================================================
extern void GmPlySeqInitCannonShoot(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y);


// ==========================================================================
// GmPlySeqInitCannon
/*!
 *	ストッパー　確保
 *
 */
// ==========================================================================
extern void GmPlySeqInitStopper(GMS_PLAYER_WORK *ply_work,GMS_ENEMY_COM_WORK *gmk_work);


// ==========================================================================
// GmPlySeqInitStopperEnd
/*!
 *	ストッパー　開放
 *
 */
// ==========================================================================
extern void GmPlySeqInitStopperEnd(GMS_PLAYER_WORK *ply_work);


// ==========================================================================
// GmPlySeqGmkInitUpBumper
/*!
 *	上昇バンパー ジャンプ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	速度X
 *	@param	spd_y		[in]	速度Y
 */
// ==========================================================================
extern void GmPlySeqGmkInitUpBumper(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y);


// ==========================================================================
// GmPlySeqGmkInitSeesaw
/*!
 *	シーソー　プレイヤー確保
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
extern void GmPlySeqGmkInitSeesaw(GMS_PLAYER_WORK *ply_work,GMS_ENEMY_COM_WORK *gmk_work);


// ================================================================
// gmPlySeqGmkSeesawEnd
/*!
  シーソー開放関数

  @note
    	ストッパーのロックが終了しておっこちるところです。
 */
// ================================================================
extern void GmPlySeqGmkInitSeesawEnd(GMS_PLAYER_WORK *ply_work, fx32 spdx, fx32 spdy);


// ================================================================
//	GmPlySeqGmkInitSpinFall
/*!
  スピン状態で落下

  @note
    強制スピン空中状態から落下への遷移ですが、
    他の状態からスピン落下遷移に兼用しても問題なさそう
 */
// ================================================================
extern void GmPlySeqGmkInitSpinFall(GMS_PLAYER_WORK *ply_work, fx32 spdx, fx32 spdy);

// ==========================================================================
// GmPlySeqInitPinball
/*!
 *	ピンボール初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	速度X
 *	@param	spd_y		[in]	速度Y
 *	@param	no_spddown_timer		[in]	スピードダウンしない時間
 */
// ==========================================================================
extern void GmPlySeqInitPinball(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y, s32 no_spddown_timer );

// ==========================================================================
// GmPlySeqInitPinballAir
/*!
 *	ピンボール（空中） 初期化
 *
 *	@param	ply_work				[in]	プレイヤーワーク
 *	@param	spd_x					[in]	速度X
 *	@param	spd_y					[in]	速度Y
 *	@param	no_move_time			[in]	操作できない時間
 *	@param	flag_no_recover_homing	[in]	ホーミング回復しないフラグ
 *	@param	no_spddown_timer		[in]	スピードダウンしない時間
 */
// ==========================================================================
extern void GmPlySeqInitPinballAir(
								   GMS_PLAYER_WORK *ply_work, 
								   fx32 spd_x, 
								   fx32 spd_y, 
								   s32 no_move_time = 5,
								   BOOL flag_no_recover_homing = FALSE,
								   s32 no_spddown_timer = 0);

// ==========================================================================
// GmPlySeqInitFlipper
/*!
 *	フリッパー初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	速度X
 *	@param	spd_y		[in]	速度Y
 */
// ==========================================================================
extern void GmPlySeqInitFlipper(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y, GMS_ENEMY_COM_WORK *com_work );

// ================================================================
// GmPlySeqGmkInitForceSpin
/*!
  強制スピン

  @note
		  強制スピン
 */
// ================================================================
extern void GmPlySeqGmkInitForceSpin(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlySeqGmkInitForceSpinDec
/*!
  強制スピン 減速タイプ

  @note
		  強制スピン
 *		user_flag  : 強制スピンタイプ OBD_OBJECT_USER_0 : 減速タイプ
 */
// ================================================================
extern void GmPlySeqGmkInitForceSpinDec(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlySeqGmkInitForceSpinFall
/*!
  強制スピン 落下

  @note
		  強制スピン 落下
 */
// ================================================================
extern void GmPlySeqGmkInitForceSpinFall(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// GmPlySeqInitPinballCtpltHold
/*!
 *	シーソー　プレイヤー確保
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitPinballCtpltHold(GMS_PLAYER_WORK *ply_work,GMS_ENEMY_COM_WORK *gmk_work);

// ================================================================
// GmPlySeqInitPinballCtplt
/*!
  ピンボールカタパルト射出

  @note
 */
// ================================================================
extern void GmPlySeqInitPinballCtplt(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y);

// ==========================================================================
// 移動歯車 GME_PLY_SEQ_STATE_GMK_MOVE_GEAR
// ==========================================================================
// ==========================================================================
// GmPlySeqInitMoveGear
/*!
 *	移動歯車 初期化
 *
 *	@param	ply_work			[in]	プレイヤーワーク
 *	@param	gmk_obj				[in]	ギミックオブジェクト
 *	@param	cam_adjust			[in]	カメラ補正の有無
 *
 *	@note
 */
// ==========================================================================
extern void GmPlySeqInitMoveGear(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *gmk_obj, BOOL cam_adjust);

// ==========================================================================
// 移動歯車 戻り回転時 GME_PLY_SEQ_STATE_GMK_MOVE_GEAR_RET
// ==========================================================================
// ==========================================================================
// GmPlySeqInitMoveGearRet
/*!
 *	移動歯車 戻り回転時 初期化
 *
 *	@param	ply_work			[in]	プレイヤーワーク
 *	@param	gmk_obj				[in]	ギミックオブジェクト
 *
 *	@note
 */
// ==========================================================================
extern void GmPlySeqInitMoveGearRet(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *gmk_obj);

// ==========================================================================
// GmPlySeqInitDrainTank
/*!
 *	排液装置 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
extern void GmPlySeqInitDrainTank(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// GmPlySeqInitDrainTankFall
/*!
 *	排液装置（落下） 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
extern void GmPlySeqInitDrainTankFall(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// スチームパイプ GME_PLY_SEQ_STATE_GMK_STEAMPIPE
// ==========================================================================
// ================================================================
// GmPlySeqInitSteamPipeIn
/*!
  スチームパイプへようこそ

  @note
 */
// ================================================================
extern void GmPlySeqInitSteamPipeIn(GMS_PLAYER_WORK *ply_work);
// ==========================================================================
// GmPlySeqInitSteamPipeOut
/*!
 *  スチームパイプへまたのおこしを
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	射出速度
 *
 *  @note
 *		速度はobj_work->spd.xへ設定してお願いします。
 */
// ==========================================================================
extern void GmPlySeqInitSteamPipeOut(GMS_PLAYER_WORK *ply_work,fx32 spd_x);

// ==========================================================================
// スプリングジャンプ GME_PLY_SEQ_STATE_GMK_POP_STEAM
// ==========================================================================
// ==========================================================================
// GmPlySeqGmkInitPopSteamJump
/*!
 *	ジャンプ 初期化
 *
 *	@param	ply_work			[in]	プレイヤーワーク
 *	@param	spd_x				[in]	速度X
 *	@param	spd_y				[in]	速度Y
 *	@param	no_jump_move_time	[in]	ジャンプ後の左右移動不可時間設定
 */
// ==========================================================================
extern void GmPlySeqGmkInitPopSteamJump(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y, fx32 no_jump_move_time);


// ==========================================================================
// スペステリングＩＮ GME_PLY_SEQ_STATE_GMK_SPL_IN
// ==========================================================================
// ==========================================================================
// GmPlySeqInitSplIn
/*!
 *	スペステリングＩＮ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
extern void GmPlySeqInitSplIn(GMS_PLAYER_WORK *ply_work, VecFx32 pos);

// ==========================================================================
// GmPlySeqGmkInitBoss2Catch
/*!
 *	ボス2掴み 初期化
 *
 *	@param	ply_work			[in]	プレイヤーワーク
 */
// ==========================================================================
extern void GmPlySeqGmkInitBoss2Catch(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// ボスFINAL地球割り着地振動 GME_PLY_SEQ_STATE_GMK_BOSS5_QUAKE
// ==========================================================================
// ==========================================================================
// GmPlySeqGmkInitBoss5Quake
/*!
 *	ボスFINAL地球割り着地振動 初期化
 *
 *	@param	ply_work		[io]	プレイヤーワーク
 *	@param	no_move_time	[in]	操作停止時間
 */
// ==========================================================================
extern void GmPlySeqGmkInitBoss5Quake(GMS_PLAYER_WORK *ply_work, Sint32 no_move_time);

// ==========================================================================
// Ending演出 GME_PLY_SEQ_STATE_GMK_ENDING_DEMO1
// ==========================================================================
// ==========================================================================
// GmPlySeqGmkInitEndingDemo1
/*!
 *	エンディング演出１
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
extern void GmPlySeqGmkInitEndingDemo1(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// Ending演出 GME_PLY_SEQ_STATE_GMK_ENDING_DEMO2
// ==========================================================================
// ==========================================================================
// GmPlySeqGmkInitEndingDemo2
/*!
 *	エンディング演出２
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	type		[in]	NormalSonic Type2 の場合 TRUE
 */
// ==========================================================================
extern void GmPlySeqGmkInitEndingDemo2(GMS_PLAYER_WORK *ply_work, BOOL type2);

// ==========================================================================
// トロッコ GME_PLY_SEQ_STATE_GMK_TRUCK_DANGER
// ==========================================================================
// ==========================================================================
// GmPlySeqGmkInitTruckDanger
/*!
 *	トロッコ危険発生 初期化
 *
 *	@param	ply_work			[in]	プレイヤーワーク
 *	@param	gmk_obj				[in]	ギミックオブジェクト
 *	@param	cam_adjust			[in]	カメラ補正の有無
 *
 *	@note
 *		通常状態はメインシーケンスで行う特殊タイプです
 */
// ==========================================================================
extern void GmPlySeqGmkInitTruckDanger(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *gmk_obj);

// ==========================================================================
// トロッコ GME_PLY_SEQ_STATE_GMK_TRUCK_DANGER_RET
// ==========================================================================
// ==========================================================================
// GmPlySeqGmkInitTruckDangerRet
/*!
 *	トロッコ危険回避 初期化
 *
 *	@param	ply_work			[in]	プレイヤーワーク
 *	@param	gmk_obj				[in]	ギミックオブジェクト
 *	@param	cam_adjust			[in]	カメラ補正の有無
 *
 *	@note
 *		通常状態はメインシーケンスで行う特殊タイプです
 */
// ==========================================================================
extern void GmPlySeqGmkInitTruckDangerRet(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *gmk_obj);

// ==========================================================================
// Utility
// ==========================================================================
// ==========================================================================
// GmPlySeqGmkInitGmkJump
/*!
 *	ギミックジャンプ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	速度X
 *	@param	spd_y		[in]	速度Y
 *	@param	set_act		[in]	ギミックジャンプアクション設定 TRUE で自動設定
 */
// ==========================================================================
extern void GmPlySeqGmkInitGmkJump(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y, BOOL set_act = TRUE);

// ==========================================================================
// gmPlySeqGmkInitGimmickDepend
/*!
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	gmk_obj		[in]	ギミックオブジェクトワーク
 *	@param	ofst_x		[in]	つかまり位置オフセットX
 *	@param	ofst_y		[in]	つかまり位置オフセットY
 *	@param	ofst_z		[in]	つかまり位置オフセットZ
 *
 *	@note
 *		アクションは呼び出し元で設定して下さい。\n
 *		gmk_obj使用、終了時にNULLにする事\n
 *		呼び出す前にtarget_dp_***を正しく設定しておいてください。\n
 *		gmk_work0 : オフセットX \n
 *		gmk_work1 : オフセットY \n
 *		gmk_work2 : オフセットZ \n
 *		target_dp_dir		: 角度反映 \n
 *		target_dp_pos		: 座標直接反映 \n
 *		target_dp_dist		: 距離反映 \n
 *		ply_work:player_flag \n
 *			GMD_PLF_USER1			: 位置設定 座標直設定 target_dp_pos\n
 *			GMD_PLF_USER2			: 位置設定 オフセット設定 target_dp_pos\n
 *			GMD_PLF_USER3			: 位置設定 距離 + 角度設定 target_dp_dist + target_dp_dir\n
 *			GMD_PLF_USER4			: 角度 プレイヤー角度あり target_dp_dir\n
 *		ply_obj:user_flag \n
 *			OBD_OBJECT_USER_0		: ギミックvib_timer反映\n
 *		gmk_obj:enemy_flag \n
 *			GMD_ENEMY_FLAG_USER0	: 終了フラグ 単純終了 \n
 *			GMD_ENEMY_FLAG_USER1	: 終了フラグ XY速度 速度反映 ギミックspd.x spd.y \n
 *			GMD_ENEMY_FLAG_USER2	: 終了フラグ X移動量 速度反映 ギミックspd_m \n
 *			GMD_ENEMY_FLAG_USER3	: 終了フラグ XY移動量 速度反映 プレイヤーmove
 */
// ==========================================================================
extern void GmPlySeqGmkInitGimmickDependInit(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *gmk_obj, fx32 ofst_x, fx32 ofst_y, fx32 ofst_z);

// ==========================================================================
// GmPlySeqGmkMainGimmickDepend
/*!
 *	ギミック接着処理
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@return	TRUE : シーケンス続行中
 *
 *	@note
 *		ply_work:player_flag \n
 *			GMD_PLF_USER1			: 位置設定 座標直設定 target_dp_pos\n
 *			GMD_PLF_USER2			: 位置設定 オフセット設定 target_dp_pos\n
 *			GMD_PLF_USER3			: 位置設定 距離 + 角度設定 target_dp_dist + target_dp_dir\n
 *			GMD_PLF_USER4			: 角度 プレイヤー角度あり target_dp_dir\n
 *		ply_obj:user_flag \n
 *			OBD_OBJECT_USER_0		: ギミックvib_timer反映\n
 *		gmk_obj:enemy_flag \n
 *			GMD_ENEMY_FLAG_USER1	: 終了フラグ 単純終了 \n
 *			GMD_ENEMY_FLAG_USER2	: 終了フラグ XY速度 速度反映 ギミックspd.x spd.y \n
 *			GMD_ENEMY_FLAG_USER3	: 終了フラグ X移動量 速度反映 ギミックspd_m \n
 *			GMD_ENEMY_FLAG_USER4	: 終了フラグ XY移動量 速度反映 プレイヤーmove
 */
// ==========================================================================
extern void GmPlySeqGmkMainGimmickDepend(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// GmPlySeqGmkSpdSet
/*!
 *	プレイヤーの速度を設定する
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	ジャンプ速度X
 *	@param	spd_y		[in]	ジャンプ速度Y
 *
 *	@note
 *		プレイヤーの角度 設定速度によって\n
 *		速度設定を行います
 */
// ==========================================================================
extern void GmPlySeqGmkSpdSet(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y);

// ==========================================================================
// GmPlySeqGmkTruckSpdSet
/*!
 *	プレイヤーの速度を設定する ダッシュパネルトロッコの時用
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	ジャンプ速度X
 *	@param	spd_y		[in]	ジャンプ速度Y
 *
 *	@note
 *		プレイヤーの角度 設定速度によって\n
 *		速度設定を行います
 */
// ==========================================================================
extern void GmPlySeqGmkTruckSpdSet(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _PT_H_

//----- Include Files -------------------------------------------------------
