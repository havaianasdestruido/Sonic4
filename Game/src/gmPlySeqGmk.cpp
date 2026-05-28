// ==========================================================================
/*!
  @file gmPlySeqGmk.cpp
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlySeqGmk.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date::						   $
 */
// ==========================================================================
/*
 * Memo
 *
 *	ギミックシーケンス
 *
 *	登録流れ
 *	1.gmPlySeqDat.h
 *		GME_PLY_SEQ_STATE にシーケンスステートを登録する。
 *		ギミックは GME_PLY_SEQ_STATE_GMK_START より後ろに登録。
 *
 *	2.gmPlySeqDat.cpp
 *		g_gm_ply_seq_state_data_tbl にシーケンス設定データを登録。
 *
 *	3.gmPlySeq.cpp
 *		g_gm_ply_seq_init_tbl_*** にシーケンス初期化処理を登録する。
 *		後述の初期化方法1の時は普通に登録、初期化方法2の時は
 *		NULLを入れておく
 *
 *
 *	・初期化
 *		方法1
 *		g_gm_ply_seq_init_tblに初期化関数を登録し、
 *		シーケンス変更呼び出し処理から
 *		GmPlySeqChangeSequence を呼び出してシーケンスチェンジ
 *
 *		方法2
 *		シーケンス変更呼び出し処理から
 *		GmPlySeqChangeSequenceState でシーケンスステートのみを変更し、
 *		変更呼び出し処理内で各種設定を行う
 *		GmPlySeqChangeSequenceState は、各種初期化を行う前に実行する
 *		
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmObj.h"
#include "gmPlayer.h"
#include "gmEnemy.h"
#include "gmSound.h"
#include "gmPlyEfct.h"
#include "gmCamera.h"
#include "gmPadVib.h"

#include "gmPlySeq.h"
#include "gmPlySpec.h"
#include "gmPlySeqGmk.h"

#include "gmMain.h"

#include "gmGmkScrew.h"
#include "gmGmkWaterSlider.h"
#include "gmGmkTruck.h"
#include "gmEnding.h"

//----- Definitions ---------------------------------------------------------
// ==========================================================================
// 各種ギミック設定
// ==========================================================================
/* ジャンプ系ギミック */
#define GMD_PLYGMK_SPRING_SIDE_NOSPD_TIME	( 64*FX32_ONE )	//!< 横スプリング時の減速無し時間
//#define GMD_PLYGMK_JUMP_NOSPD_TIME			( 48 )	//!< ジャンプ台時の減速無し時間
//#define GMD_PLYGMK_CIRCLE_NOSPD_TIME		( 30 )	//!< ダッシュサークル時の減速無し時間

//大岩（傾斜）ギミック
#define GMD_GMK_ROCK_RIDE_FRAME_WAIT_JUMP	( 5 )					//ジャンプ待ちフレーム数
#define GMD_GMK_ROCK_RIDE_OFFSET_PLAYER		( 0x00038000L)			//岩乗り時プレイヤ描画オフセット
#define GMD_GMK_ROCK_RIDE_START_JUMP_X		( 0x00003600L)			//開始演出ジャンプ値
#define GMD_GMK_ROCK_RIDE_START_JUMP_Y		(-0x00006000L)			//開始演出ジャンプ値
//#define GMD_GMK_ROCK_RIDE_SPEED_ADD			( 0x00000100L)			//スピード追加量
#define GMD_GMK_ROCK_RIDE_SPEED_ADD			( 0x00000180L)			//スピード追加量
#define GMD_GMK_ROCK_RIDE_SPEED_RANGE		( 0x0000f000L)			//スピード変化範囲
#define GMD_GMK_ROCK_RIDE_SPEED_MID			(-0x00008000L)			//スピード中速
//#define GMD_GMK_ROCK_RIDE_SPEED_LIMIT		( 0x00003000L)			//岩から弾かれるスピード
#define GMD_GMK_ROCK_RIDE_SPEED_LIMIT		( 0x00003C00L)			//岩から弾かれるスピード
#define GMD_GMK_ROCK_RIDE_SPEED_PINCH		( 0x00000b00L)			//おっとっとモーションになるスピード
#define GMD_GMK_ROCK_RIDE_JUMP_X			( 0x00004000L)			//岩から弾かれるジャンプ値
#define GMD_GMK_ROCK_RIDE_JUMP_Y			( 0x00003000L)			//岩から弾かれるジャンプ値
#define GMD_GMK_ROCK_RIDE_KEY_ANGLE_LIMIT	(NNM_DEGtoA32(90.0f))	//コントローラ角度制限
#define GMD_GMK_ROCK_RIDE_OFFSET_CAMERA_Y	( -48 )					//カメラオフセット値


/* ダッシュパネル */
#define GMD_PLYGMK_DASHPANEL_SPD			(0xD800)//(GMD_PL_DEF_MAX_SPD)	//!< ダッシュパネル速度
#define GMD_PLYGMK_DASHPANEL_TIME			(60)					//!< ダッシュパネルダッシュ時間
#define GMD_PLYGMK_DASHPANEL_NOSPDDOWN_TIME	(12*FX32_ONE)			//!< ダッシュパネル減速なし時間

/* コークスクリュー設定 */
// 標準スクリュー ループ部分
#define GMD_PLYGMK_SCREW_LENGTH				( 0x01759d0 )	// 1ループの距離 1.19.12
#define GMD_PLYGMK_SCREW_WIDTH				(  288 )		// 1ループの幅(ドット)
#define GMD_PLYGMK_SCREW_HEIGHT				( 76 / 2 )		// 半径

// 滑車ギミック
#define GMD_PLYGMK_PULLEY_OFS_Y				(40*FX32_ONE)	// 掴まり時Ｙオフセット

//ピンボール
#define GMD_PLYGMK_PINBALL_TIME					(60)			//ピンボールダッシュ時間

// 移動歯車
#define GMD_PLYGMK_MOVE_GEAR_CAM_OFST_X		(0)				//!< 移動歯車 カメラオフセット X
#define GMD_PLYGMK_MOVE_GEAR_CAM_OFST_Y		(-48)			//!< 移動歯車 カメラオフセット Y

/* スチームパイプ */
#define GMD_PLYGMK_STEAMPIPEOUT_TIME		(60)					//!< スチームパイプスピンダッシュ時間

/* 強制スピン 減速タイプ */
#define GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX	(0x2000)		//!< 強制スピン減速タイプ時最大速度
#define GMD_PLYGMK_FORCE_SPIN_DEC_SPDDEC	(0x0800)		//!< 強制スピン減速タイプ現速度

/* トロッコ 危険発生 */
#define GMD_PLY_SEQ_GMK_TRUCK_DANGER_DIR_FRAME				(17)//(40)		//!< 危険発生角度変更モーションフレーム
#define GMD_PLY_SEQ_GMK_TRUCK_DANGER_RET_DIR_FRAME			(14)//(33)		//!< 危険回避角度変更モーションフレーム
#define GMD_PLY_SEQ_GMK_TRUCK_DANGER_RET_DIR_START_FRAME	(18)//(27)		//!< 危険回避角度変更開始モーションフレーム

#define GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_X	(-0.f)
#define GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_Y	(-5.f)//(-7.f)
#define GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_Z	(-9.f)//(-8.f)
//static float trans_x_1 = -0.f;//1.25f;//-19.2f/GMD_OBJ_DRAW_SCALE;
//static float trans_y_1 = -7.f;//8.f;///GMD_OBJ_DRAW_SCALE;
//static float trans_z_1 = -8.f;//6.0f;///GMD_OBJ_DRAW_SCALE;

#define GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_DIR	(0x1800)	//!< 傾き角度
#define GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_SPD	(0x0020)	//!< 傾き速度



//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

static void gmPlySeqGmkMainGimmickRockRidePush(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainGimmickRockRideStartWait(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainGimmickRockRideStartJump(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainGimmickRockRideStartFall(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainGimmickRockRide(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainGimmickRockRideStop(GMS_PLAYER_WORK *ply_work);

static void gmPlySeqGmkMainGimmickBreathing(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainDashPanel(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainWaterSlider(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainSpipe(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkScrewMain(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMoveScrew(GMS_PLAYER_WORK *ply_work, fx32 screw_length, s16 screw_width, s16 screw_height);
static void gmPlySeqGmkCannonWait(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkStopperMove(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkStopperWait(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkStopperEnd(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkSeesaw(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainPinball(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainPinballAir(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainPinballCtplt(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainPinballCtpltHold(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainFlipper(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainForceSpin(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainForceSpinDec(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainForceSpinFall(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainDrainTank(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainMoveGear(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMoveGearMove(GMS_PLAYER_WORK *ply_work, BOOL spd_up_type);
static void gmPlySeqGmkMoveGearAnimeSpeedSetWalk(GMS_PLAYER_WORK *ply_work, fx32 spd_set);
static void gmPlySeqGmkMainSteamPipe(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainBoss5Quake(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainEndingFrontSide(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainEndingFinish(GMS_PLAYER_WORK *ply_work);
static u32 gmPlayerCheckTruckAirFoot(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainTruckDanger(GMS_PLAYER_WORK *ply_work);
static void gmPlySeqGmkMainTruckDangerRet(GMS_PLAYER_WORK *ply_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
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
void GmPlySeqInitSpringJump(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y,
									BOOL spd_clear, fx32 no_jump_move_time, s32 fall_dir, BOOL t_cam_slow)
{
	BOOL	b_act_set = TRUE;

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_SPRINGJUMP);

	// 以前の速度クリアチェック
	if (spd_clear) {
		ply_work->obj_work.spd.x = ply_work->obj_work.spd.y = 0;
		ply_work->obj_work.spd_add.x = ply_work->obj_work.spd_add.y = 0;
		ply_work->obj_work.spd_m = 0;
	}

	// トロッコ時設定
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
#ifndef GMD_MAIN_USE_BODY_ROTATE
		if (fall_dir != -1) {
			//// 接地まで擬似ジャンプ重力固定
			Angle32	dir_dist;
			//ply_work->gmk_flag |= GMD_PLGF_GMK_JUMP_PSEUDOFALL_DIR_FIX;
			// GmPlySeqGmkInitGmkJumpの後に設定

			// 重力方向変更
			// ジャンプ時重力方向変更
			ply_work->jump_pseudofall_dir = (u16)fall_dir;
			// 擬似重力方向変更
			dir_dist = fall_dir - ply_work->ply_pseudofall_dir;
			if ((u16)(MTM_MATH_ABS(dir_dist)) > 0x8000) {
				if (dir_dist < 0) {
					ply_work->ply_pseudofall_dir += 0x10000 + dir_dist;
				}
				else {
					ply_work->ply_pseudofall_dir += dir_dist - 0x10000;
				}
				//ply_work->ply_pseudofall_dir += (s16)((u16)(0x10000 - (fall_dir - ply_work->ply_pseudofall_dir)));
			}
			else { 
				ply_work->ply_pseudofall_dir = fall_dir;
			}
			g_gm_main_system.pseudofall_dir = (u16)ply_work->ply_pseudofall_dir;
		}
#endif // !GMD_MAIN_USE_BODY_ROTATE

		// 攻撃設定
		GmPlayerSetAtk(ply_work);

		// アクション自動設定OFF
		b_act_set = FALSE;

		// アクション変更
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_FALL);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

	// ギミックジャンプ設定
	GmPlySeqGmkInitGmkJump(ply_work, spd_x, spd_y, b_act_set);

#if GMD_PLY_USE_CAM_ROT_TRUCK_JUMP
	if ((ply_work->player_flag & GMD_PLF_TRUCK_RIDE) &&
				fall_dir != -1) {
		// 接地処理がすんでから設定(GmPlySeqGmkInitGmkJump 内でLandingあり)
		// ジャンプ中ユーザー入力によるカメラ回転なしに
		ply_work->gmk_flag2 |= GMD_PLGF2_TRUCK_JUMP_NO_CAM_ROT;
	}
#endif

	// トロッコの時は強制的にフリップを解除
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;

		// カメラ回転速度設定
		if (t_cam_slow) {
			// 接地までカメラ回転をゆっくりに
			ply_work->gmk_flag2 |= GMD_PLGF2_TRUCK_CAM_ROT_SLOW;
		}
	}

	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE && fall_dir != -1) {
		// 接地まで擬似ジャンプ重力固定
		ply_work->gmk_flag |= GMD_PLGF_GMK_JUMP_PSEUDOFALL_DIR_FIX;
	}

	// ジャンプ中移動不可設定
	if (no_jump_move_time > 0) {
		GmPlySeqSetNoJumpMoveTime(ply_work, no_jump_move_time);
	}

	// SE
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		GmSoundPlaySE("Lorry5");
	}
	else {
		GmSoundPlaySE("Spring");
	}

	// コントローラー振動
	GMM_PAD_VIB_SMALL();
}

// ==========================================================================
// 岩乗り開始 GME_PLY_SEQ_STATE_GMK_ROCK_RIDE_START
// ==========================================================================
// ==========================================================================
// GmPlySeqInitRockRideStart
/*!
 *	岩乗り開始 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	com_work	[in]	ギミックオブジェクトワーク GMS_ENEMY_COM_WORK
 */
// ==========================================================================
void GmPlySeqInitRockRideStart(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *com_work)
{
	if (ply_work->gmk_obj == (OBS_OBJECT_WORK*)com_work) {
		return;
	}

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_ROCK_RIDE_START);

	// プレイヤー ギミック初期化
    GmPlayerStateGimmickInit(ply_work);

	// ギミックオブジェクト登録
	ply_work->gmk_obj = &com_work->obj_work;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainGimmickRockRidePush;

	//---------------------------------------
	//プレイヤ初期化
	//---------------------------------------
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	amAssert( player_obj_work );
	OBS_OBJECT_WORK* gimmick_obj_work = ply_work->gmk_obj;
	amAssert( gimmick_obj_work );

	//モーション変更
	GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_PUSH1 );

	//とまる
	player_obj_work->spd_m = 0;
	player_obj_work->spd.x = 0;
	player_obj_work->spd.y = 0;
	player_obj_work->spd.z = 0;
	player_obj_work->spd_add.x = 0;
	player_obj_work->spd_add.y = 0;
	player_obj_work->spd_add.z = 0;

	//向き
	if ( player_obj_work->pos.x < gimmick_obj_work->pos.x ){
		player_obj_work->disp_flag &= ~OBD_DISP_HFLIP;
	}
	else{
		player_obj_work->disp_flag |= OBD_DISP_HFLIP;
	}
}


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
void GmPlySeqInitRockRide(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *com_work)
{
	OBS_OBJECT_WORK* gimmick_obj_work = ply_work->gmk_obj;
	if (gimmick_obj_work == (OBS_OBJECT_WORK*)com_work) {
		return;
	}
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_ROCK_RIDE);

	// ギミック接着設定
	GmPlySeqGmkInitGimmickDependInit(ply_work, &com_work->obj_work, 0, 0, 0);

	// ギミックオブジェクト登録
	ply_work->gmk_obj = &com_work->obj_work;

	// フラグ類の設定は後で		
	com_work->target_dp_dist = GMD_GMK_ROCK_RIDE_OFFSET_PLAYER;
	ply_work->player_flag |= GMD_PLF_USER3 | GMD_PLF_USER4;
	ply_work->obj_work.move_flag |= OBD_MOVE_NOCOL;

	//ワーク
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	amAssert( player_obj_work );
	gimmick_obj_work = ply_work->gmk_obj;
	
	//下側からあたった場合は終了
	if ( player_obj_work->pos.y > gimmick_obj_work->pos.y ){
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL);
		ply_work->seq_func = gmPlySeqGmkMainGimmickRockRideStop;
	}
	//上側からあたった場合乗る
	else{
		ply_work->seq_func = gmPlySeqGmkMainGimmickRockRide;
		
		//カメラ
		GmPlayerCameraOffsetSet( ply_work, 0, GMD_GMK_ROCK_RIDE_OFFSET_CAMERA_Y );
		GmCameraAllowSet(10.0f, 30.f, 0.0f);
	}

	//---------------------------------------
	//プレイヤ初期化
	//---------------------------------------
	//モーション変更
	GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_GMK_BALL_01 );
	player_obj_work->disp_flag |= OBD_DISP_REPEAT;

	//ジャンプ用設定
	ply_work->gmk_flag |= GMD_PLGF_GMK_AIR_JUMP;
	
	//スピード
	fx32 pos_per = FX_Div( gimmick_obj_work->pos.x - player_obj_work->pos.x, GMD_GMK_ROCK_RIDE_OFFSET_PLAYER);
	player_obj_work->spd_m = FX_Mul(pos_per, GMD_GMK_ROCK_RIDE_SPEED_LIMIT) + gimmick_obj_work->spd_m;

	//向き
	if ( gimmick_obj_work->spd_m > 0 ){
		player_obj_work->disp_flag &= ~OBD_DISP_HFLIP;
	}
	else{
		player_obj_work->disp_flag |= OBD_DISP_HFLIP;
	}
}

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
void GmPlySeqInitPulley(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *com_work)
{
	if (ply_work->gmk_obj == (OBS_OBJECT_WORK*)com_work) {
		return;
	}
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_PULLEY);

	com_work->obj_work.spd.x = ply_work->obj_work.spd.x;
	if(!(ply_work->obj_work.move_flag & OBD_MOVE_JUMP)) {
		com_work->obj_work.spd.x = FX_Mul((ply_work->obj_work.spd_m ), mtMathCos( ply_work->obj_work.dir.z ));
	}
	com_work->obj_work.move_flag &= ~OBD_MOVE_DIR;

	// ギミック接着設定
	GmPlySeqGmkInitGimmickDependInit(ply_work, &com_work->obj_work, 0, GMD_PLYGMK_PULLEY_OFS_Y, 0);
	// フラグ類の設定は後で
#if 1
	com_work->target_dp_pos.x = 0;
	com_work->target_dp_pos.y = GMD_PLYGMK_PULLEY_OFS_Y;
	com_work->target_dp_pos.z = 0;
//	ply_work->player_flag |= (GMD_PLF_USER2);
//	ply_work->player_flag |= (GMD_PLF_USER2 | GMD_PLF_USER4);
	ply_work->player_flag |= (GMD_PLF_USER3 | GMD_PLF_USER4);
	com_work->target_dp_dist = -GMD_PLYGMK_PULLEY_OFS_Y;

#else
	com_work->target_dp_dist  = -32*FX32_ONE;
	com_work->target_dp_dir.z = 0;
	ply_work->player_flag |= (GMD_PLF_USER3 | GMD_PLF_USER4);
#endif
	ply_work->obj_work.move_flag |= OBD_MOVE_NOCOL;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_JUMP | OBD_MOVE_UNDER);
	ply_work->gmk_flag |= GMD_PLGF_GMK_AIR_JUMP;

	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_HANG_ST);
	ply_work->obj_work.pos = com_work->obj_work.pos;
	ply_work->obj_work.pos.y += GMD_PLYGMK_PULLEY_OFS_Y;
}

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
void GmPlySeqInitBreathing(GMS_PLAYER_WORK *ply_work)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_BREATHING);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	//プレイヤワーク設定
	ply_work->seq_func = gmPlySeqGmkMainGimmickBreathing;

	//プレイヤオブジェクトワーク設定
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	amAssert( player_obj_work );
	player_obj_work->spd_m = 0;
	player_obj_work->spd.x = 0;
	player_obj_work->spd.y = 0;
	player_obj_work->spd_add.x = 0;
	player_obj_work->spd_add.y = 0;

	//モーション変更
	if ( player_obj_work->move_flag & OBD_MOVE_UNDER ){
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_BREATH);
	}
	else{
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_BREATH_J);
	}

	//効果音
	GmSoundPlaySE("Breathe");

}

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
void GmPlySeqInitDashPanel(GMS_PLAYER_WORK *ply_work, GME_PLYGMK_DASHPANEL_TYPE type)
{
	//fx32	spd_x, spd_y;

	MTM_ASSERT(ply_work->obj_work.move_flag & OBD_MOVE_UNDER);	// 接地状態の必要あり
	MTM_ASSERT((u32)type < GME_PLYGMK_DASHPANEL_MAX);

#if 1
	const fx32 spd_tbl[GME_PLYGMK_DASHPANEL_MAX][MTD_XY] = {
		{GMD_PLYGMK_DASHPANEL_SPD, 0},	// 右
		{-GMD_PLYGMK_DASHPANEL_SPD, 0},	// 左
		{0, -GMD_PLYGMK_DASHPANEL_SPD},	// 縦右壁
		{0, -GMD_PLYGMK_DASHPANEL_SPD},	// 縦左壁
	};
#endif

	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_DASHPANEL);

	// アクション変更
	if (!(ply_work->player_flag & GMD_PLF_TRUCK_RIDE)) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN_SMALL);
	}
	else {
		if (type == GME_PLYGMK_DASHPANEL_LEFT || type == GME_PLYGMK_DASHPANEL_V_LEFT) {
			ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_L;
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_RUN_L);
		}
		else {
			ply_work->gmk_flag &= GMD_PLGF_GMK_TRUCK_L;
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_RUN);
		}
	}
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// ダッシュ時間
	ply_work->obj_work.user_timer = GMD_PLYGMK_DASHPANEL_TIME;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainDashPanel;

	// 速度設定
#if 0
	spd_x = spd_y = 0;
	if (type == GME_PLYGMK_DASHPANEL_RIGHT) {
		spd_x = ply_work->spd_max;
	}
	else if (type == GME_PLYGMK_DASHPANEL_LEFT) {
		spd_x = -ply_work->spd_max;
	}
	else {
		spd_y = -ply_work->spd_max;
	}
	GmPlySeqGmkSpdSet(ply_work, spd_x, spd_y);
#else
	if (!(ply_work->player_flag & GMD_PLF_TRUCK_RIDE)) {
		// 通常
		GmPlySeqGmkSpdSet(ply_work, spd_tbl[type][MTD_X], spd_tbl[type][MTD_Y]);
	}
	else {
		// トロッコ時
		GmPlySeqGmkTruckSpdSet(ply_work, spd_tbl[type][MTD_X], spd_tbl[type][MTD_Y]);
	}
#endif

	ply_work->no_spddown_timer = GMD_PLYGMK_DASHPANEL_NOSPDDOWN_TIME;

	// MAX speed SET @ ダッシュパネル→コークスクリュー時に急減速する対策
	ply_work->spd_work_max = ply_work->obj_work.spd_m;
	

	// 攻撃設定
	GmPlayerSetAtk(ply_work);

    // SE
	GmSoundPlaySE("Spin");

	if (!(ply_work->player_flag & GMD_PLF_TRUCK_RIDE)) {
		// エフェクト ブラー
		GmPlyEfctCreateSpinDashBlur(ply_work, 1/*小*/);
		GmPlyEfctCreateSpinDashCircleBlur(ply_work);

		// 軌跡エフェクト
		GmPlyEfctCreateTrail(ply_work, GME_PLY_EFCT_TRAIL_TYPE_SPINDASH);
	}

	// コントローラー振動
	GMM_PAD_VIB_SMALL();
}

// ==========================================================================
// ターザンロープ GME_PLY_SEQ_STATE_GMK_TARZAN_ROPE
// ==========================================================================
// ==========================================================================
// GmPlySeqInitTarzanRope
/*!
 *	ターザンロープ 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	com_work	[in]	ギミックオブジェクトワーク GMS_ENEMY_COM_WORK
 */
// ==========================================================================
void GmPlySeqInitTarzanRope(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *com_work)
{
	if (ply_work->gmk_obj == (OBS_OBJECT_WORK*)com_work) {
		return;
	}
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_TARZAN_ROPE);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	// ギミックオブジェクト登録
	ply_work->gmk_obj = &com_work->obj_work;

	//プレイヤワーク設定
	ply_work->seq_func = NULL;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_UNDER | OBD_MOVE_FALL | OBD_MOVE_DIR);

	//モーション変更
	GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_GMK_ROPE );
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	player_obj_work->disp_flag |= OBD_DISP_REPEAT;
	ply_work->gmk_flag |= GMD_PLGF_GMK_AIR_JUMP;
}

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
void GmPlySeqInitWaterSlider(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *com_work)
{
	if (ply_work->gmk_obj == (OBS_OBJECT_WORK*)com_work) {
		return;
	}

	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_WATER_SLIDER);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work); 

	// ギミックオブジェクト登録
	ply_work->gmk_obj = &com_work->obj_work;

	//プレイヤワーク設定
	ply_work->seq_func = gmPlySeqGmkMainWaterSlider;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_JUMP);

	//モーション変更
	GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_GMK_SLIDE );
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	player_obj_work->disp_flag |= OBD_DISP_REPEAT;

	//エフェクト作成
	GmGmkWaterSliderCreateEffect();
}

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
void GmPlySeqInitSpipe(GMS_PLAYER_WORK *ply_work)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_SPIPE);

	if (  (ply_work->act_state != GME_PLY_ACT_STATE_SPIN)
		&&(ply_work->act_state != GME_PLY_ACT_STATE_SPIN_SMALL) ) {
		// SE
		GmSoundPlaySE("Spin");
	}

	// アクション変更
	if (ply_work->act_state != GME_PLY_ACT_STATE_SPIN_SMALL) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN_SMALL);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}
	ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	// メイン処理設定
	ply_work->seq_func = gmPlySeqGmkMainSpipe;


	// エフェクト ブラー
	GmPlyEfctCreateSpinDashBlur(ply_work, 1/*小*/);
	GmPlyEfctCreateSpinDashCircleBlur(ply_work);
}

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
u16 GmPlySeqScrewCheck( GMS_PLAYER_WORK *ply_work )
{
    if ( ply_work->seq_func == gmPlySeqGmkScrewMain )
        return TRUE;
    return FALSE;
}
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
void GmPlySeqInitScrew(GMS_PLAYER_WORK *ply_work, GMS_ENEMY_COM_WORK *gmk_work, fx32 pos_x, fx32 pos_y, u16 flag)
{
	if (GmPlySeqScrewCheck(ply_work)) {
		// すでにコークスクリュー中
		return;
	}
	
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_SCREW);

	// アクション変更
	GmPlayerWalkActionSet(ply_work);

	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOMOVE;	// 移動をこちらで制御
	ply_work->gmk_flag |= GMD_PLGF_GMK_AIR_JUMP
						  | GMD_PLGF_GMK_NO_MAXDASH;

	// ギミックオブジェクト登録
	ply_work->gmk_obj = &gmk_work->obj_work;

	// ギミック情報保持
	ply_work->gmk_work0 = pos_x;
	ply_work->gmk_work1 = pos_y;
//	ply_work->gmk_work2 = (s32)(screw_type >> GMD_GMK_SCREW_TYPE_SHIFT);		// スクリュータイプ保存
//	ply_work->gmk_work3 = angle_type;											// 角度タイプ保存
//	ply_work->obj_work.user_work = (u32)(screw_type & GMD_GMK_SCREW_EVE_REC_MASK);	// スクリューフラグ保存
	ply_work->obj_work.user_work = (u32)flag;									// フラグ保存
	ply_work->obj_work.user_timer = 0;

    // オーバー分の既に移動した距離を足す
	if ( ply_work->obj_work.user_work & GMD_GMK_SCREW_EVE_FLAG_LEFT ) {
		if ( ply_work->gmk_work0 > ply_work->obj_work.pos.x ) {
			ply_work->obj_work.user_timer = ply_work->gmk_work0 - ply_work->obj_work.pos.x;
		}
	}
	else {
		if ( ply_work->gmk_work0 < ply_work->obj_work.pos.x ) {
			ply_work->obj_work.user_timer = ply_work->obj_work.pos.x - ply_work->gmk_work0;
		}
	}

    ply_work->gmk_work1 -= ply_work->obj_work.field_rect[OBD_BOTTOM] << FX32_SHIFT;

	// 調整
//	if ( ply_work->gmk_work2 == GMD_GMK_SCREW_TYPE_VERTICAL ) {
//		if ( !( ply_work->obj_work.user_work & GMD_GMK_SCREW_EVE_FLAG_UP )) {
//			ply_work->gmk_work1 -= 0x04000;
//		}
//	} else {
//		ply_work->obj_work.user_work |= GMD_GMK_SCREW_EVE_FLAG_UP;
//		//ply_work->gmk_work1 += 0x0200;
//	}

	// メイン処理設定
	ply_work->seq_func = gmPlySeqGmkScrewMain;
    ply_work->timer = 16;										// 地面チェック無視時間
}

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
void GmPlySeqInitDemoFw( GMS_PLAYER_WORK *ply_work )
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_DEMO_FW);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	//プレイヤワーク設定
	ply_work->seq_func = NULL;

	//モーション変更
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_FW);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}
	else {
		GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_FW );
		OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
		player_obj_work->disp_flag |= OBD_DISP_REPEAT;
	}
}

// ==========================================================================
// 大砲 GME_PLY_SEQ_STATE_GMK_CANNON
// ==========================================================================
// ==========================================================================
// GmPlySeqInitCannon
/*!
 *	大砲　装填
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitCannon(GMS_PLAYER_WORK *ply_work,GMS_ENEMY_COM_WORK *gmk_work)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_CANNON);

	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	ply_work->obj_work.move_flag |= OBD_MOVE_NOCOLOBJ/*|OBD_MOVE_FALL*/;	//	地形あたりをはずす。落下はデフォ？

	ply_work->obj_work.pos.x = gmk_work->obj_work.pos.x;
	ply_work->obj_work.spd_m = 0;
	ply_work->obj_work.spd.x = 0;
	ply_work->obj_work.spd.x = 0;
//	ply_work->obj_work.spd.y = 0;
	if( ply_work->obj_work.spd_add.y <= 0 )
		ply_work->obj_work.spd_add.y = GMD_OBJ_DEF_FALL_SPD;

	// メイン処理設定
	ply_work->seq_func = gmPlySeqGmkCannonWait;
	ply_work->gmk_obj = &gmk_work->obj_work;

	// バリア, 無敵エフェクト, スーパーソニックエフェクト表示OFF	// 20091126 Dimps Ishizaki
	ply_work->gmk_flag2 |= GMD_PLGF2_BARRIER_DISP_OFF | GMD_PLGF2_INVINCIBLE_DISP_OFF | GMD_PLGF2_SUPEREFCT_DISP_OFF;
	GmPlayerSetDefInvincible(ply_work);					// 無敵セット
	ply_work->invincible_timer = 0;						// ダメージ後の無敵解消対策
}

// ==========================================================================
// GmPlySeqInitCannonShoot
/*!
 *	大砲　発射
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitCannonShoot(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_CANNON_SHOOT);
	GmPlySeqGmkInitGmkJump( ply_work, spd_x, spd_y);

	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_CANNON_SHOOT);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	GmPlayerSetDefNormal(ply_work);						// 無敵を通常化
	
	// 攻撃設定
	GmPlayerSetAtk(ply_work);

	// エフェクト ブラー
	GmPlyEfctCreateSpinJumpBlur(ply_work);
}

// ==========================================================================
// ストッパー GME_PLY_SEQ_STATE_GMK_STOPPER
// ==========================================================================
// ==========================================================================
// GmPlySeqInitStopper
/*!
 *	ストッパー　確保
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitStopper(GMS_PLAYER_WORK *ply_work,GMS_ENEMY_COM_WORK *gmk_work)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_STOPPER);

	// アクション変更
	if (ply_work->act_state != GME_PLY_ACT_STATE_SPIN) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
	}
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
//	ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP;
	
	ply_work->obj_work.spd_m = 0;
	ply_work->obj_work.spd.x = 0;
	ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_FALL;	//		落ちない
	ply_work->obj_work.flag |= OBD_OBJECT_NOHIT;	// ヒットなし
	// メイン処理設定
	ply_work->seq_func = gmPlySeqGmkStopperMove;
	ply_work->gmk_obj = &gmk_work->obj_work;
}

// ==========================================================================
// GmPlySeqInitStopperEnd
/*!
 *	ストッパー　開放
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitStopperEnd(GMS_PLAYER_WORK *ply_work)
{
	//	スピン状態は保持
//	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
//	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	ply_work->obj_work.move_flag |= ( OBD_MOVE_FALL		//		戻す
									 |OBD_MOVE_JUMP);
	// メイン処理設定
	ply_work->seq_func = gmPlySeqGmkStopperEnd;		// 操作無効な状態を暫し維持するため専用シーケンス
}

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
void GmPlySeqGmkInitUpBumper(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_UPBUMPER);

	// ギミックジャンプ設定
	GmPlySeqGmkInitGmkJump(ply_work, spd_x, spd_y);

	// SE
	GmSoundPlaySE("Spring");
}

// ==========================================================================
// シーソー GME_PLY_SEQ_STATE_GMK_SEESAW
// ==========================================================================
// ==========================================================================
// GmPlySeqGmkInitSeesaw
/*!
 *	シーソー　プレイヤー確保
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqGmkInitSeesaw(GMS_PLAYER_WORK *ply_work,GMS_ENEMY_COM_WORK *gmk_work)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_SEESAW);

	// アクション変更
	if (ply_work->act_state != GME_PLY_ACT_STATE_SPIN_SMALL) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN_SMALL);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}
	ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_FALL;	//		落ちない

	ply_work->obj_work.spd_m = 0;
	ply_work->obj_work.spd.x = 0;
	ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.dir.z = 0;
	
	// メイン処理設定
	ply_work->seq_func = gmPlySeqGmkSeesaw;
	ply_work->gmk_obj = &gmk_work->obj_work;

	// エフェクト ブラー
	GmPlyEfctCreateSpinDashBlur(ply_work, 1/*大*/);
	GmPlyEfctCreateSpinDashCircleBlur(ply_work);
}

// ================================================================
//	GmPlySeqGmkInitSeesawEnd
/*!
  シーソー開放関数

  @note
    	ストッパーのロックが終了しておっこちるところです。
 */
// ================================================================
void GmPlySeqGmkInitSeesawEnd(GMS_PLAYER_WORK *ply_work, fx32 spdx, fx32 spdy)
{
	// シーケンスステート設定
	GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL);
	GmPlySeqGmkInitGmkJump( ply_work, spdx, spdy, FALSE );
	ply_work->no_spddown_timer = 0;									// シーソー→ポイントマーカーの位置にてソニックが滑るので、no_spddown_timer をゼロにしてみる
	ply_work->gmk_obj = NULL;
	//	スピン状態を保持
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

	// 攻撃設定
	GmPlayerSetAtk(ply_work);
}

// ================================================================
//	GmPlySeqGmkInitSpinFall
/*!
  スピン状態で落下

  @note
    強制スピン空中状態から落下への遷移ですが、
    他の状態からスピン落下遷移に兼用しても問題なさそう
 */
// ================================================================
void GmPlySeqGmkInitSpinFall(GMS_PLAYER_WORK *ply_work, fx32 spdx, fx32 spdy)
{
	// シーケンスステート設定
	ply_work->gmk_obj = NULL;
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_SPIN_FALL);
	GmPlySeqInitFallState(ply_work);

	GmPlySeqGmkInitGmkJump( ply_work, spdx, spdy, FALSE );
	ply_work->no_spddown_timer = 0;
	// アクション設定(スピン状態を保持)
	if (ply_work->act_state != GME_PLY_ACT_STATE_SPIN) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}
	// 攻撃設定
	GmPlayerSetAtk(ply_work);
	// エフェクト生成
	GmPlyEfctCreateSpinJumpBlur(ply_work);
}
//----------------------------------------------------------------


// ==========================================================================
// ピンボール GME_PLY_SEQ_STATE_GMK_PINBALL
// ==========================================================================
// ==========================================================================
// GmPlySeqInitPinball
/*!
 *	ピンボール 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	速度X
 *	@param	spd_y		[in]	速度Y
 *	@param	no_spddown_timer		[in]	スピードダウンしない時間
 */
// ==========================================================================
void GmPlySeqInitPinball(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y, s32 no_spddown_timer )
{
	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_PINBALL);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	// アクション変更
	if ( ply_work->act_state != GME_PLY_ACT_STATE_JUMP_SPIN ){
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		//ブラーエフェクト
		GmPlyEfctCreateSpinJumpBlur(ply_work);
	}
    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;

	//速度
	GmPlySeqGmkSpdSet(ply_work, spd_x, spd_y);
	ply_work->obj_work.spd_add.x = 0;
	ply_work->obj_work.spd_add.y = 0;
	ply_work->obj_work.spd_add.z = 0;

	//時間
	ply_work->obj_work.user_timer = GMD_PLYGMK_PINBALL_TIME;
	ply_work->no_spddown_timer = no_spddown_timer*FX32_ONE;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainPinball;

	// 攻撃設定
	GmPlayerSetAtk(ply_work);

    // SE
	GmSoundPlaySE("Spin");
}

// ==========================================================================
// ピンボール（空中） GME_PLY_SEQ_STATE_GMK_PINBALL_AIR
// ==========================================================================
// ==========================================================================
// GmPlySeqInitPinballAir
/*!
 *	ピンボール 初期化
 *
 *	@param	ply_work				[in]	プレイヤーワーク
 *	@param	spd_x					[in]	速度X
 *	@param	spd_y					[in]	速度Y
 *	@param	no_move_time			[in]	操作できない時間
 *	@param	flag_no_recover_homing	[in]	ホーミング回復しないフラグ
 *	@param	no_spddown_timer		[in]	スピードダウンしない時間
 */
// ==========================================================================
void GmPlySeqInitPinballAir(
							GMS_PLAYER_WORK *ply_work, 
							fx32 spd_x, 
							fx32 spd_y, 
							s32 no_move_time,	// = 5
							BOOL flag_no_recover_homing,	// = FALSE
							s32 no_spddown_timer	// = 0
							)
{
	//攻撃状況を保存
	u32 flag_attack = 0;
	if ( ply_work->rect_work[GMD_PLAYER_RECT_ATK].flag & OBD_RECT_ENABLE ){
		flag_attack = 1;
	}
	
	//ホーミング使用状況を保存
	u32 flag_no_homing = 0;
	if ( ply_work->player_flag & GMD_PLF_NOHOMING ){
		flag_no_homing = 1;
	}

	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_PINBALL_AIR);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	//ホーミング使用済み、回復しないフラグの場合
	if ( flag_no_recover_homing & flag_no_homing ){
		ply_work->player_flag |= GMD_PLF_NOHOMING;
	}

	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;
	ply_work->obj_work.move_flag |= OBD_MOVE_FALL;
	ply_work->obj_work.flag &= ~OBD_OBJECT_NOHIT;

	// 移動不可をＯＮに
	ply_work->player_flag |= GMD_PLF_NOJUMPMOVE;

	ply_work->obj_work.spd_fall = FX_Mul( ply_work->obj_work.spd_fall, FX_F32_TO_FX32(1.1f) ); 

	//移動
	ply_work->obj_work.dir.y = 0;
	GmPlySeqGmkSpdSet(ply_work, spd_x, spd_y);
	ply_work->obj_work.spd_add.x = 0;
	ply_work->obj_work.spd_add.y = 0;
	ply_work->obj_work.spd_add.z = 0;
	ply_work->obj_work.spd_m = 0;

	//リピート情報保存
	BOOL flag_repeat = FALSE;
	if ( ply_work->obj_work.disp_flag & OBD_DISP_REPEAT ){
		flag_repeat = TRUE;
	}

	// アクション変更
	GME_PLY_ACT_STATE act_state = ply_work->act_state;
	switch (act_state){
		case GME_PLY_ACT_STATE_JUMP_FALL_TURN:
			act_state = GME_PLY_ACT_STATE_JUMP_FALL;
			flag_repeat = TRUE;
			break;
		case GME_PLY_ACT_STATE_JUMP_FALL_R_TURN:
			act_state = GME_PLY_ACT_STATE_JUMP_FALL_R;
			flag_repeat = TRUE;
			break;
		case GME_PLY_ACT_STATE_FW:
		case GME_PLY_ACT_STATE_FW_EX:
		case GME_PLY_ACT_STATE_TURN:
		case GME_PLY_ACT_STATE_TURN_RUN:
		case GME_PLY_ACT_STATE_TURN_BRAKE:
		case GME_PLY_ACT_STATE_WALK:
		case GME_PLY_ACT_STATE_RUN:
		case GME_PLY_ACT_STATE_DASH_1:
		case GME_PLY_ACT_STATE_DASH_2:
		case GME_PLY_ACT_STATE_BRAKE1:
		case GME_PLY_ACT_STATE_BRAKE2:
		case GME_PLY_ACT_STATE_BRAKE3:
			act_state = GME_PLY_ACT_STATE_JUMP_FALL;
			flag_repeat = TRUE;
			break;
		default:
			break;
	}

	GmPlayerActionChange(ply_work, act_state);

	//リピート情報復帰
	if ( flag_repeat ){
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

	//時間
	ply_work->no_spddown_timer = no_spddown_timer*FX32_ONE;
	ply_work->obj_work.user_timer = no_move_time;
	ply_work->obj_work.user_flag = 1;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainPinballAir;

	// 攻撃設定
	if ( flag_attack ){
		GmPlayerSetAtk(ply_work);
	}

    // 壁張り付き対応
	if (ply_work->gmk_flag & GMD_PLGF_GMK_WALL) {
		ply_work->obj_work.spd.z = ply_work->obj_work.spd.y;
		ply_work->obj_work.spd.y = 0;
		if ( ply_work->obj_work.pos.z < 0 ) {
			ply_work->obj_work.spd.z = -ply_work->obj_work.spd.z;
		}
    }
}


// ==========================================================================
// フリッパー GME_PLY_SEQ_STATE_GMK_PINBALL
// ==========================================================================
// ==========================================================================
// GmPlySeqInitFlipper
/*!
 *	フリッパー 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	速度X
 *	@param	spd_y		[in]	速度Y
 */
// ==========================================================================
void GmPlySeqInitFlipper(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y, GMS_ENEMY_COM_WORK *com_work )
{
	if (ply_work->gmk_obj == (OBS_OBJECT_WORK*)com_work) {
		return;
	}

	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_FLIPPER);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	// ギミックオブジェクト登録
	ply_work->gmk_obj = &com_work->obj_work;

	// アクション変更
	if ( ply_work->act_state != GME_PLY_ACT_STATE_JUMP_SPIN ){
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		//ブラーエフェクト
		GmPlyEfctCreateSpinJumpBlur(ply_work);
	}

	//移動設定
	ply_work->obj_work.spd.x = spd_x;
	ply_work->obj_work.spd.y = spd_y;
	ply_work->obj_work.spd.z = 0;
	ply_work->obj_work.spd_add.x = 0;
	ply_work->obj_work.spd_add.y = 0;
	ply_work->obj_work.spd_add.z = 0;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_FALL | OBD_MOVE_UNDER);
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_NOCOL;
	ply_work->obj_work.spd_m = 0;
#if _IPHONE
	ply_work->obj_work.dir.z = 0; // めり込み回避
#endif // _IPHONE

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainFlipper;

	// 攻撃設定
	//GmPlayerSetAtk(ply_work);

    // SE
	GmSoundPlaySE("Spin");

	//フリップ
	if ( ply_work->obj_work.spd.x > 0 ){
		ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
	}
	else if ( ply_work->obj_work.spd.x < 0 ){
		ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
	}
}


// ==========================================================================
// 強制スピン GME_PLY_SEQ_STATE_GMK_FORCESPIN
// ==========================================================================
// ================================================================
// GmPlySeqGmkInitForceSpin
/*!
  強制スピン

  @note
		  強制スピン
 *		user_flag  : 強制スピンタイプ OBD_OBJECT_USER_0 : 減速タイプ
 */
// ================================================================
void GmPlySeqGmkInitForceSpin(GMS_PLAYER_WORK *ply_work)
{
	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_FORCESPIN);
	GmPlayerStateGimmickInit(ply_work);
	// スピン継続状態でなければSEコール
	if (  (ply_work->act_state != GME_PLY_ACT_STATE_SPIN)
		&&(ply_work->act_state != GME_PLY_ACT_STATE_SPIN_SMALL) ) {
	    // SE
		GmSoundPlaySE("Spin");
	}
	// アクション変更
	if (ply_work->act_state != GME_PLY_ACT_STATE_SPIN) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		// エフェクト ブラー
		GmPlyEfctCreateSpinDashBlur(ply_work, 0/*大*/);
	}

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainForceSpin;
	ply_work->obj_work.user_timer = ply_work->obj_work.spd_m;

	// スピンタイプ設定
	ply_work->obj_work.user_flag = 0;

	// 速度設定
//	GmPlySeqGmkSpdSet(ply_work,
//	                           FX_Mul(ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z)),
//	                           FX_Mul(ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z)) );
	// 攻撃設定
	GmPlayerSetAtk(ply_work);

	ply_work->obj_work.move_flag |= (OBD_MOVE_UNDER | OBD_MOVE_FALL | OBD_MOVE_DIR);
	ply_work->gmk_obj = NULL;

}

// ==========================================================================
// 強制スピン減速タイプ GME_PLY_SEQ_STATE_GMK_FORCESPIN_DEC
// ==========================================================================
// ================================================================
// GmPlySeqGmkInitForceSpinDec
/*!
  強制スピン 減速タイプ

  @note
		  強制スピン
 *		user_flag  : 強制スピンタイプ OBD_OBJECT_USER_0 : 減速タイプ
 */
// ================================================================
void GmPlySeqGmkInitForceSpinDec(GMS_PLAYER_WORK *ply_work)
{
	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_FORCESPIN_DEC);
	GmPlayerStateGimmickInit(ply_work);
	// スピン継続状態でなければSEコール
	if (  (ply_work->act_state != GME_PLY_ACT_STATE_SPIN)
		&&(ply_work->act_state != GME_PLY_ACT_STATE_SPIN_SMALL) ) {
	    // SE
		GmSoundPlaySE("Spin");
	}
	if (ply_work->act_state != GME_PLY_ACT_STATE_SPIN) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		// エフェクト ブラー
		GmPlyEfctCreateSpinDashBlur(ply_work, 0/*大*/);
	}

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainForceSpinDec;
	ply_work->obj_work.user_timer = ply_work->obj_work.spd_m;

	// スピンタイプ設定
	ply_work->obj_work.user_flag = OBD_OBJECT_USER_0;

	// 攻撃設定
	GmPlayerSetAtk(ply_work);

	// フラグ設定
	ply_work->obj_work.move_flag |= (OBD_MOVE_UNDER | OBD_MOVE_FALL | OBD_MOVE_DIR);
	ply_work->gmk_obj = NULL;
}

// ==========================================================================
// 強制スピン落下 GME_PLY_SEQ_STATE_GMK_FORCESPIN_FALL
// ==========================================================================
// ================================================================
// GmPlySeqGmkInitForceSpinFall
/*!
  強制スピン 落下

  @note
		  強制スピン 落下
 *		user_flag  : 強制スピンタイプ OBD_OBJECT_USER_0 : 減速タイプ
 */
// ================================================================
void GmPlySeqGmkInitForceSpinFall(GMS_PLAYER_WORK *ply_work)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_FORCESPIN_FALL);

	// gmPlySeqGmkMainForceSpin, gmPlySeqGmkMainForceSpinDecからしか遷移しないので
	// アクションを切り替えない

	// フラグ設定
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_FALL;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainForceSpinFall;

	// 速度設定
	ply_work->obj_work.spd.x = FX_Mul(ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z));
	ply_work->obj_work.spd.y = FX_Mul(ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z));
	if (ply_work->obj_work.user_flag & OBD_OBJECT_USER_0) {
		// 減速タイプの落下は下に落ちるようにする
		if (MTM_MATH_ABS(ply_work->obj_work.spd.x) > MTM_MATH_ABS(ply_work->obj_work.spd.y)) {
			ply_work->obj_work.spd.y = ply_work->obj_work.spd.x >> 1;
			if (ply_work->obj_work.spd.y < 0) {
				ply_work->obj_work.spd.y = -ply_work->obj_work.spd.y;
			}
			ply_work->obj_work.spd.x >>= 1;
		}
	}
	// 速度設定
//	GmPlySeqGmkSpdSet(ply_work,
//	                           FX_Mul(ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z)),
//	                           FX_Mul(ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z)) );
}
//----------------------------------------------------------------


// ==========================================================================
// ピンボール（カタパルトロック） GME_PLY_SEQ_STATE_GMK_PINBALL_HOLD
// ==========================================================================
// ==========================================================================
// GmPlySeqInitPinballCtpltHold
/*!
 *	スプリングカタパルト　プレイヤー確保
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitPinballCtpltHold(GMS_PLAYER_WORK *ply_work,GMS_ENEMY_COM_WORK *gmk_work)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_HOLD);

	// アクション変更
//	if (ply_work->act_state != GME_PLY_ACT_STATE_SPIN) {
	if (  (ply_work->prev_seq_state != GME_PLY_SEQ_STATE_GMK_FORCESPIN)
		&&(ply_work->prev_seq_state != GME_PLY_SEQ_STATE_GMK_FORCESPIN_DEC) ) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}
	ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_FALL;	//		落ちない

	ply_work->obj_work.spd_m = 0;
	ply_work->obj_work.spd.x = 0;
	ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.dir.z = 0;

	// エフェクト ブラー
	GmPlyEfctCreateSpinDashBlur(ply_work, 0/*大*/);

	// メイン処理設定
	ply_work->seq_func = gmPlySeqGmkMainPinballCtpltHold;
	ply_work->gmk_obj = &gmk_work->obj_work;
}

// ==========================================================================
// ピンボール（カタパルト発射） GME_PLY_SEQ_STATE_GMK_PINBALL_AIR
// ==========================================================================
// ==========================================================================
// GmPlySeqInitPinballCtplt
/*!
 *	ピンボール 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	速度X
 *	@param	spd_y		[in]	速度Y
 */
// ==========================================================================
void GmPlySeqInitPinballCtplt(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y)
{
	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, 
		(!spd_x)? GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_UP:GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_LR);

	// アクション変更
//	if (ply_work->act_state != GME_PLY_ACT_STATE_SPIN) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
//	}

	// 速度設定
	if( spd_x )
	{
	    ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;
		// メイン処理設定
    	ply_work->seq_func = gmPlySeqGmkMainPinballCtplt;

		// 向きセット
		if (spd_x > 0) {
			ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
		} else {
			ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
		}
    }
    else
    {
		ply_work->obj_work.move_flag |= OBD_MOVE_JUMP|OBD_MOVE_FALL;
		GmPlySeqGmkSpdSet(ply_work, spd_x, spd_y);
	    ply_work->obj_work.spd_m = 0;
	    ply_work->obj_work.dir.z = 0;
    	ply_work->seq_func = gmPlySeqGmkMainPinballAir;
	}

	// 攻撃設定
	ply_work->obj_work.flag &= ~OBD_OBJECT_NOHIT;
	GmPlayerSetAtk(ply_work);

	// ダッシュ時間
	ply_work->no_spddown_timer = 600*FX32_ONE;

	// SE
	GmSoundPlaySE("Catapult");
	GmPlyEfctCreateSpinJumpBlur(ply_work);

}


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
void GmPlySeqInitMoveGear(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *gmk_obj, BOOL cam_adjust)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_MOVE_GEAR);

	// ギミックステート初期化
	//GmPlayerStateGimmickInit(ply_work);

	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// 角度クリア
	ply_work->obj_work.dir.z = 0;

	// ギミック接着
	GmPlySeqGmkInitGimmickDependInit(ply_work, gmk_obj, 0/*ofst_x*/, 0/*ofst_y*/, 0/*ofst_z*/);
	ply_work->player_flag |= GMD_PLF_USER2 |			// 位置設定 オフセット設定 target_dp_pos
							GMD_PLF_WALK_SMK_EFCT_OFF;	// 歩き煙エフェクトOFF
	ply_work->obj_work.user_flag = OBD_OBJECT_USER_0;	// ギミックvib_timer反映
	ply_work->obj_work.move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_UNDER;		// 地形チェックOFF 常に地面接触状態に

	// アクション設定
	if (ply_work->obj_work.spd_m != 0) {
		// 歩きアクション設定
		GmPlayerWalkActionSet(ply_work);
	}
	else if (ply_work->act_state != GME_PLY_ACT_STATE_FW) {
		// FW
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_FW);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

	if (cam_adjust) {
		// カメラ注視点変更
		GmPlayerCameraOffsetSet(ply_work, GMD_PLYGMK_MOVE_GEAR_CAM_OFST_X, GMD_PLYGMK_MOVE_GEAR_CAM_OFST_Y);
		// カメラ注視点固定
		GmCameraAllowSet(0.f, 0.f, 0.f);
	}

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainMoveGear;
}


// ==========================================================================
// スチームパイプ GME_PLY_SEQ_STATE_GMK_STEAMPIPE
// ==========================================================================
// ================================================================
// GmPlySeqInitSteamPipeIn
/*!
 *  スチームパイプへようこそ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *  @note
 *		user_timer : 演出カウンタ
 */
// ================================================================
void GmPlySeqInitSteamPipeIn(GMS_PLAYER_WORK *ply_work)
{
	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_STEAMPIPE);
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);

	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

	//	移動設定
	ply_work->obj_work.move_flag |= OBD_MOVE_NOCOL;	// 土地あたり無効にしておく
	ply_work->obj_work.move_flag &= ~OBD_MOVE_FALL;
	ply_work->obj_work.spd.x = 0;
	ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.spd_m = 0;
	//	パイプにあわせる
//	ply_work->obj_work.pos.y -= 4*FX32_ONE;

	// 移動中無敵設定
	GmPlayerSetDefInvincible(ply_work);
	ply_work->invincible_timer = 0;						// ダメージ後の無敵解消対策

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainSteamPipe;

	ply_work->gmk_obj = NULL;

	// モーション速度加速演出タイマ
	ply_work->obj_work.user_timer = 0;

	// 発光エフェクト生成
	GmPlyEfctCreateSteamPipe(ply_work);
	// エフェクト ブラー
	GmPlyEfctCreateSpinDashBlur(ply_work, 0/*大*/);
}
// ==========================================================================
// GME_PLY_SEQ_STATE_GMK_STEAMPIPE
// ==========================================================================
// ==========================================================================
// GmPlySeqInitSteamPipeOut
/*!
 *  スチームパイプ排出
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_x		[in]	射出速度
 *
 *  @note
 *		速度はobj_work->spd.xへ設定してお願いします。
 */
// ==========================================================================
void GmPlySeqInitSteamPipeOut(GMS_PLAYER_WORK *ply_work,fx32 spd_x)
{
	//	無視させたフラグの復活
	ply_work->obj_work.move_flag &= ~OBD_MOVE_NOCOL;	// 土地あたり復活
	ply_work->obj_work.move_flag |=  OBD_MOVE_FALL;		// 落下処理復活

	// 一旦着地した扱いにする
//	GmPlySeqLandingSet(ply_work, 0);

	// 無敵解除
	GmPlayerSetDefNormal(ply_work);

	// ダッシュ時間
	ply_work->obj_work.user_timer = GMD_PLYGMK_STEAMPIPEOUT_TIME;

	// 速度設定
	GmPlySeqGmkInitGmkJump(ply_work, spd_x, 0);			// 排出後はジャンプに処理を託す(seq_funcもセットされる)
//	GmPlySeqGmkSpdSet(ply_work, spd_x, 0);
//	ply_work->obj_work.pos.y -= 4*FX32_ONE;
	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);

	// 攻撃設定
	GmPlayerSetAtk(ply_work);

    // SE
//	GmSoundPlaySE("Spin");
}

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
void GmPlySeqGmkInitPopSteamJump(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y, fx32 no_jump_move_time)
{
	// シーケンスステート設定
	if (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_POP_STEAM) {
		GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_POP_STEAM);
	}

	//	速度情報のクリア
	ply_work->obj_work.spd.x = ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.spd_add.x = ply_work->obj_work.spd_add.y = 0;
	ply_work->obj_work.spd_m = 0;

	// ギミックジャンプ設定
	GmPlySeqGmkInitGmkJump(ply_work, spd_x, spd_y);

	// ジャンプ中移動不可設定 // 20091126 Dimps Ishizaki 移動不可時間追加
	if (no_jump_move_time > 0) {
		GmPlySeqSetNoJumpMoveTime(ply_work, no_jump_move_time);
	}

	// 移動不可をＯＮに
	//ply_work->player_flag |= GMD_PLF_NOJUMPMOVE;
	// SE
	//GmSoundPlaySE("");
}



// ==========================================================================
// 排液装置 GME_PLY_SEQ_STATE_GMK_DRAIN_TANK
// ==========================================================================
// ==========================================================================
// GmPlySeqInitDrainTank
/*!
 *	排液装置 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitDrainTank(GMS_PLAYER_WORK *ply_work)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_DRAIN_TANK);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);

	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_UNDER | OBD_MOVE_FALL);

	ply_work->obj_work.spd.x = 0;
	ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.spd.z = 0;
	ply_work->obj_work.spd_add.x = 0;
	ply_work->obj_work.spd_add.y = 0;
	ply_work->obj_work.spd_add.z = 0;

	// メイン処理設定
    ply_work->seq_func = NULL;
}

// ==========================================================================
// 排液装置 GME_PLY_SEQ_STATE_GMK_DRAIN_TANK_FALL
// ==========================================================================
// ==========================================================================
// GmPlySeqInitDrainTankFall
/*!
 *	排液装置（落下） 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqInitDrainTankFall(GMS_PLAYER_WORK *ply_work)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_DRAIN_TANK_FALL);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	// アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_FALL;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

	ply_work->obj_work.spd_add.x = 0;
	ply_work->obj_work.spd_add.y = 0;
	ply_work->obj_work.spd_add.z = 0;

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainDrainTank;
}

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
void GmPlySeqInitSplIn(GMS_PLAYER_WORK *ply_work, VecFx32 pos)
{

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_SPL_IN);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	// アクション変更
	if ( ply_work->act_state != GME_PLY_ACT_STATE_SPIN ){
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		//ブラーエフェクト
	//	GmPlyEfctCreateSpinJumpBlur(ply_work);
	}

	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_UNDER | OBD_MOVE_FALL);

	ply_work->obj_work.pos.x = pos.x;
	ply_work->obj_work.pos.y = pos.y;

	// メイン処理設定
    ply_work->seq_func = NULL;

	// スペシャルステージ突入フラグ
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_SPECIAL_STAGE;
}

// ==========================================================================
// ボス2掴み GME_PLY_SEQ_STATE_GMK_BOSS2_CATCH
// ==========================================================================
// ==========================================================================
// GmPlySeqGmkInitBoss2Catch
/*!
 *	ボス2掴み 初期化
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void GmPlySeqGmkInitBoss2Catch(GMS_PLAYER_WORK *ply_work)
{
	//シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_BOSS2_CATCH);

	//アクション変更
	if ( ply_work->act_state != GME_PLY_ACT_STATE_JUMP_SPIN ){
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_UNDER | OBD_MOVE_FALL);

	//メイン処理設定
    ply_work->seq_func = NULL;
}

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
void GmPlySeqGmkInitBoss5Quake(GMS_PLAYER_WORK *ply_work, Sint32 no_move_time)
{
	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_BOSS5_QUAKE);
	
	// アクション変更
	if (ply_work->act_state != GME_PLY_ACT_STATE_STAGGER_B) {
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_STAGGER_B);
		ply_work->obj_work.disp_flag	|= OBD_DISP_REPEAT;
	}
	
	// 速度クリア
	ply_work->obj_work.spd.x = ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.spd_add.x = ply_work->obj_work.spd_add.y = 0;
	ply_work->obj_work.spd_m = 0;
	
	ply_work->obj_work.move_flag	|= OBD_MOVE_NOSPDM | OBD_MOVE_NOMOVE;
	
	// 操作停止時間
	ply_work->obj_work.user_timer	= no_move_time;
	
	// メイン処理設定
	ply_work->seq_func	= gmPlySeqGmkMainBoss5Quake;
}

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
void GmPlySeqGmkInitEndingDemo1(GMS_PLAYER_WORK *ply_work)
{
	//シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_ENDING_DEMO1);

	//アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_ENDING_FR1);

	//メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainEndingFrontSide;

	ply_work->obj_work.spd_m = 0;
	ply_work->obj_work.spd.x = 0;
	ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.spd_add.x = 0;
	ply_work->obj_work.spd_add.y = 0;
}

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
void GmPlySeqGmkInitEndingDemo2(GMS_PLAYER_WORK *ply_work, BOOL type2)
{
	//シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_ENDING_DEMO2);

	//アクション変更
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_SPIN);
	ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

	//メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainEndingFinish;

	ply_work->obj_work.spd.y = -0x2800;
	ply_work->obj_work.spd_add.y = GMD_OBJ_DEF_FALL_SPD/4;
	ply_work->obj_work.dir.y = 0x4000;
	ply_work->obj_work.user_work = 0;										// スケール拡大加速度に使用
	ply_work->obj_work.user_flag = 0;										// NormalSonic Type2判定に使用
	if (type2) {
		ply_work->obj_work.user_flag = 1;
	}
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_NOCOL;
}

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
 *		gmk_work0	角度速度
 *		gmk_work1	現在の角度
 *		gmk_work2	カウンタ
 *		gmk_work3	トロッコ角度
 *		player_flag : GMD_PLF_USER1	トロッコ復旧開始告知
 *		player_flag : GMD_PLF_USER2	トロッコ傾き演出終了
 *		player_flag : GMD_PLF_USER3	トロッコ右壁接着(前輪浮き)
 *		user_timer	: がたん演出速度
 */
// ==========================================================================
void GmPlySeqGmkInitTruckDanger(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *gmk_obj)
{
	s32	rot_dir;
	u32	air_foot;

	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_TRUCK_DANGER);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	// シーケンスを抜けた時にギミックステートをクリアできるようにセットしておく
	ply_work->gmk_obj = gmk_obj;

	// 演出中無敵設定
	GmPlayerSetDefInvincible(ply_work);
	ply_work->invincible_timer = 0;		// ダメージ後の無敵解消対策(ダメージによる矩形復帰を防ぐ)

	// プレイヤーフラグクリア
	ply_work->player_flag &= ~GMD_PLF_USER_MASK;

	// 左向きフラグクリア
	ply_work->gmk_flag &= ~GMD_PLGF_GMK_TRUCK_L;

	// 演出用MTX反映開始
	ply_work->gmk_flag |= GMD_PLGF_GMK_EXMTX_R;
	// 初期化
	nnMakeUnitMatrix(&ply_work->ex_obj_mtx_r);

	// 現在の角度を取得
	//rot_dir = (0x8000/*裏返る分*/ - ply_work->obj_work.dir.z);
	rot_dir = (0x8000/*裏返る分*/ - ply_work->obj_work.dir.z)
						+ (s16)(g_gm_main_system.pseudofall_dir - ply_work->obj_work.dir_fall);
//	if (rot_dir > 0) {
//		rot_dir = (u16)rot_dir;
//	}

	// 現在の角度を設定
	ply_work->gmk_work1 = 0;

	// カウンタ設定
	ply_work->gmk_work2 = GMD_PLY_SEQ_GMK_TRUCK_DANGER_DIR_FRAME * FX32_ONE;

	// トロッコ角度クリア
	ply_work->gmk_work3 = 0;

	// がたん回数カウンタクリア
	ply_work->obj_work.user_work = 0;

	// 地形両端浮き具合チェック
	air_foot = gmPlayerCheckTruckAirFoot(ply_work);

	//if (ply_work->obj_work.dir.z <= 0x6000) {
	if (ply_work->obj_work.dir.z <= 0x8000) {
		
		if (air_foot & 0x01) {
		// 前が接地している
			// トロッコ 後輪はずれる
			rot_dir -= GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_DIR;	// トロッコが傾く分引いておく

			// 速度設定
			ply_work->obj_work.user_timer = 0x400;
		}
		else {
			// そのままソニックが転げ落ちるタイプ
			ply_work->player_flag |= GMD_PLF_USER2;	// トロッコがたん演出を終了済みに
			// アクション設定
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_DANGER);
		}
	}
	else {
		if (air_foot & 0x02) {
		// 後ろが接地している
			// トロッコ 前輪はずれる
			rot_dir += GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_DIR;	// トロッコが傾く分足しておく
			ply_work->player_flag |= GMD_PLF_USER3;	// 前輪浮きタイプ

			// 速度設定
			ply_work->obj_work.user_timer = -0x400;
		}
		else {
			// そのままソニックが転げ落ちるタイプ
			ply_work->player_flag |= GMD_PLF_USER2;	// トロッコがたん演出を終了済みに
			// アクション設定
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_DANGER);
		}
	}


	// 角度速度を設定
	ply_work->gmk_work0 = (u16)(rot_dir / GMD_PLY_SEQ_GMK_TRUCK_DANGER_DIR_FRAME);


	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainTruckDanger;

	// SE
	GmSoundPlaySE("Lorry2");
}

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
void GmPlySeqGmkInitTruckDangerRet(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *gmk_obj)
{
	s32	rot_dir;
	s32	gmk_work3_temp;
	u32	player_flag_temp;

	gmk_work3_temp		= ply_work->gmk_work3;	// 保存
	player_flag_temp	= ply_work->player_flag & (GMD_PLF_USER_MASK & ~GMD_PLF_USER2/*不要フラグクリア*/);

	// シーケンスステート設定
	GmPlySeqChangeSequenceState(ply_work, GME_PLY_SEQ_STATE_GMK_TRUCK_DANGER);

	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	// プレイヤーフラグ復帰
	ply_work->player_flag |= player_flag_temp;

	// シーケンスを抜けた時にギミックステートをクリアできるようにセットしておく
	ply_work->gmk_obj = gmk_obj;

	// アクション設定
	GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_DANGER_RE);

	// 演出用MTX反映開始
	ply_work->gmk_flag |= GMD_PLGF_GMK_EXMTX_R;
	// 現在の角度を取得
	rot_dir = (0x8000/*裏返る分*/ - ply_work->obj_work.dir.z - gmk_work3_temp
							+ (s16)(g_gm_main_system.pseudofall_dir - ply_work->obj_work.dir_fall));
	if (rot_dir > 0) {
		rot_dir = (u16)rot_dir;
	}

	// 角度速度を設定
	ply_work->gmk_work0 = (-rot_dir / GMD_PLY_SEQ_GMK_TRUCK_DANGER_RET_DIR_FRAME);

	// 現在の角度を設定
	ply_work->gmk_work1 = rot_dir;

	// カウンタ設定
	ply_work->gmk_work2 = 0;

	// トロッコ角度を復旧
	ply_work->gmk_work3 = gmk_work3_temp;

	// 振動解除
#if GMD_PLY_SEQ_GMK_TRUCK_DANGER_VIB_PLAYER
	ply_work->obj_work.vib_timer = 0;
#else
#if GMD_PLY_SEQ_GMK_TRUCK_DANGER_VIB_TRUCK
	ply_work->truck_obj->vib_timer = 0;
#endif
#endif

	// メイン処理設定
    ply_work->seq_func = gmPlySeqGmkMainTruckDangerRet;
}



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
void GmPlySeqGmkInitGmkJump(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y, BOOL set_act/*= TRUE*/)
{
	// 死亡時はジャンプしない
	if ( ply_work->player_flag & GMD_PLF_DIE ) {
		return;
	}
	// ギミックステート初期化
	GmPlayerStateGimmickInit(ply_work);

	// 空中の速度状態に設定する
	if ( ply_work->obj_work.move_flag & OBD_MOVE_UNDER ) {
		ply_work->obj_work.spd.x = ply_work->obj_work.spd_m;
	}
	// 一旦着地した扱いにする
	GmPlySeqLandingSet(ply_work, 0);

	// 速度重力変換
	if (ply_work->player_flag & GMD_PLF_TRUCK_RIDE) {
		ObjObjectSpdDirFall(&spd_x, &spd_y, (u16)(-ply_work->jump_pseudofall_dir));
	}
	else {
		ObjObjectSpdDirFall(&spd_x, &spd_y, ply_work->obj_work.dir_fall);
	}

	if (!( ply_work->obj_work.move_flag & OBD_MOVE_JUMP)) {
		// ジャンプ開始時のY座標を記憶
		ply_work->camera_jump_pos_y = ply_work->obj_work.pos.y;
	}

	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM;
	ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;

	//ply_work->obj_work.ppFunc = GmPlayerJumpMain;

	// 速度設定
	if ( spd_x ) {
		if (set_act) {
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_GF01);
		}

		ply_work->obj_work.spd.x = spd_x;
		// 向き設定
		if ( ply_work->obj_work.spd.x < 0 ){
			if ( ply_work->obj_work.spd_m > 0 ) {
				ply_work->obj_work.spd_m = 0;
			}
			ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
		}
		else {
			if ( ply_work->obj_work.spd_m < 0 ) {
				ply_work->obj_work.spd_m = 0;
			}
			ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
		}

		// 減速度ダウン待機タイマー
		ply_work->no_spddown_timer = GMD_PLYGMK_SPRING_SIDE_NOSPD_TIME;
	}
	else{
		if (set_act) {
			// アクション変更
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_JUMP_G01);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
		ply_work->obj_work.spd.x = FX_Mul(ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z));
	}

	if (spd_y) {
		ply_work->obj_work.spd.y = spd_y;
	}
	else {
		ply_work->obj_work.spd.y = FX_Mul(ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z));
	}

	// 攻撃矩形クリア
	//ObjRectAtkSet(&ply_work->rect_work[1], 0, 0);

	ply_work->obj_work.user_timer	= 0;
	ply_work->obj_work.user_work	= 0;
	ply_work->timer					= 0;

	// ジャンプ設定
	GmPlySeqSetJumpState(ply_work, 0,
			GMD_PLY_SEQ_SETJUMPSTATE_IGNORE_JUMPBTN | GMD_PLY_SEQ_SETJUMPSTATE_GMK_JUMP);

	// トリックコンボクリア
	//ply_work->trick_combo		= 0;

	// 水中設定
	if (ply_work->player_flag & GMD_PLF_WATER) {
		GMD_PLAYER_WATERJUMP_SET(ply_work->obj_work.spd.x);
		GMD_PLAYER_WATERJUMP_SET(ply_work->obj_work.spd.y);
		// 泡表示
		//GmEffectInitPlayerBubble(ply_work, 0, 0, g_gm_map.camera[ply_work->camera_no].water_level);
	}
}

// ==========================================================================
// GmPlySeqGmkInitGimmickDependInit
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
 *			GMD_ENEMY_FLAG_USER1	: 終了フラグ 単純終了 \n
 *			GMD_ENEMY_FLAG_USER2	: 終了フラグ XY速度 速度反映 ギミックspd.x spd.y \n
 *			GMD_ENEMY_FLAG_USER3	: 終了フラグ X移動量 速度反映 ギミックspd_m \n
 *			GMD_ENEMY_FLAG_USER4	: 終了フラグ XY移動量 速度反映 プレイヤーmove
 */
// ==========================================================================
void GmPlySeqGmkInitGimmickDependInit(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *gmk_obj, fx32 ofst_x, fx32 ofst_y, fx32 ofst_z)
{
	if (ply_work->gmk_obj == gmk_obj) {
		return;
	}

	/* 落下速度戻す */
	GmPlayerSpdParameterSet(ply_work);

	/* プレイヤー ギミック初期化 */
    GmPlayerStateGimmickInit(ply_work);

	// ギミックオブジェクト登録
    ply_work->gmk_obj = gmk_obj;

	/* 動作設定 */
	ply_work->obj_work.move_flag |= OBD_MOVE_JUMP | OBD_MOVE_NOSPDM | OBD_MOVE_NOMOVE;
	ply_work->obj_work.move_flag &= ~(OBD_MOVE_FALL | OBD_MOVE_DIR);

	// 描画設定
	//ply_work->obj_work.disp_flag &= ~OBD_DISP_3D_PARALLEL;

	// user_flagクリア
	ply_work->obj_work.user_flag = 0;

	// プレイヤー汎用フラグOFF
	ply_work->player_flag &= ~GMD_PLF_USER_MASK;

	// プレイヤー設定
	//ply_work->player_flag |= GMD_PLF_NITRO_NOSTART;			// ニトロ開始不可

	// ギミック設定
	//ply_work->gmk_flag |= GMD_PLGF_CAMERA_GMK_Y;				// Y軸ギミック注視 ◆必要ならばこれも設定できるように
	//ply_work->gmk_camera_ofst_y = -58;

    // 速度クリア
	ply_work->obj_work.spd.x = 0;
	ply_work->obj_work.spd.y = 0;
	ply_work->obj_work.spd_m = 0;

    // 角度設定
	//ply_work->obj_work.dir.z = gmk_obj->dir.z;

	// ユーザーワーククリア
	ply_work->obj_work.user_work = 0;
	ply_work->obj_work.user_timer = 0;

	// つかまり位置オフセット保存
	ply_work->gmk_work0 = ofst_x;
	ply_work->gmk_work1 = ofst_y;
	ply_work->gmk_work2 = ofst_z;

	// 座標設定

	// メイン処理設定
    ply_work->seq_func = GmPlySeqGmkMainGimmickDepend;

}

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
void GmPlySeqGmkMainGimmickDepend(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK		*ply_obj = (OBS_OBJECT_WORK*)ply_work, *gmk_obj;
	GMS_ENEMY_COM_WORK	*gmk_work;
	BOOL				b_continue = TRUE;

	gmk_obj = ply_work->gmk_obj;
	if (gmk_obj) {
		gmk_work = (GMS_ENEMY_COM_WORK*)ply_work->gmk_obj;

#if 0
		/* 終了チェック */
		if (gmk_obj->user_flag & OBD_OBJECT_USER_0) {
			b_continue = FALSE;
			ply_obj->spd.x = gmk_obj->spd.x;
			ply_obj->spd.y = gmk_obj->spd.y;
			ply_work->gmk_obj = NULL;
		}
#endif
		/* 終了チェック */
		if (gmk_work->enemy_flag &
				(GMD_ENEMY_FLAG_USER1 | GMD_ENEMY_FLAG_USER2 | GMD_ENEMY_FLAG_USER3 | GMD_ENEMY_FLAG_USER4)) {
			b_continue = FALSE;
			ply_work->gmk_obj = NULL;
			if (gmk_work->enemy_flag & GMD_ENEMY_FLAG_USER2) {
				// XY速度 速度反映
				ply_obj->spd.x = gmk_obj->spd.x;
				ply_obj->spd.y = gmk_obj->spd.y;
			}
			else if (gmk_work->enemy_flag & GMD_ENEMY_FLAG_USER3) {
				// M速度 X速度反映
				ply_obj->spd.x = gmk_obj->spd_m;
			}
			else if (gmk_work->enemy_flag & GMD_ENEMY_FLAG_USER4) {
				// XY移動量 速度反映
				ply_obj->spd.x = ply_obj->move.x;
				ply_obj->spd.y = ply_obj->move.y;
			}
		}
		else {
			// 前座標退避
			ply_obj->prev_pos.x = ply_obj->pos.x;
			ply_obj->prev_pos.y = ply_obj->pos.y;
			ply_obj->prev_pos.z = ply_obj->pos.z;

			/* 座標算出 */
			if (ply_work->player_flag & GMD_PLF_USER1) {
				// 座標直設定
				ply_obj->pos = gmk_obj->pos;
			}
			else if (ply_work->player_flag & GMD_PLF_USER2) {
				// オフセット設定
				ply_obj->pos.x = gmk_obj->pos.x + gmk_work->target_dp_pos.x;
				ply_obj->pos.y = gmk_obj->pos.y + gmk_work->target_dp_pos.y;
				ply_obj->pos.z = gmk_obj->pos.z + gmk_work->target_dp_pos.z;
			}
			else if (ply_work->player_flag & GMD_PLF_USER3) {
				// 距離 + 角度
				NNS_MATRIX	rot_mtx;
				NNS_VECTOR	vec = {0.f, -1.f, 0.f};

				nnMakeUnitMatrix(&rot_mtx);
				nnRotateXYZMatrix(&rot_mtx, &rot_mtx, 
						-gmk_work->target_dp_dir.x,
						gmk_work->target_dp_dir.y,
						gmk_work->target_dp_dir.z);

				nnTransformVector(&vec, &rot_mtx, &vec);
				nnScaleVector(&vec, &vec, FXM_FX32_TO_FLOAT(gmk_work->target_dp_dist)); 

				ply_obj->pos.x = gmk_obj->pos.x + FXM_FLOAT_TO_FX32(vec.x);
				ply_obj->pos.y = gmk_obj->pos.y + FXM_FLOAT_TO_FX32(vec.y);
				ply_obj->pos.z = gmk_obj->pos.z + FXM_FLOAT_TO_FX32(vec.z);
			}

			/* 回転反映 */
			if (ply_work->player_flag & GMD_PLF_USER4) {
				ply_obj->dir = gmk_work->target_dp_dir;
			}

			// 移動量保存
			ply_obj->move.x = ply_obj->pos.x - ply_obj->prev_pos.x;
			ply_obj->move.y = ply_obj->pos.y - ply_obj->prev_pos.y;
			ply_obj->move.z = ply_obj->pos.z - ply_obj->prev_pos.z;

			// vib_timer反映
			if (ply_obj->user_flag & OBD_OBJECT_USER_0 &&
					gmk_obj->vib_timer) {
				ply_obj->vib_timer = gmk_obj->vib_timer + FX32_ONE;
			}

			// flowクリア
			if (ply_obj->move_flag & OBD_MOVE_NOMOVE) {
				ply_obj->flow.x = ply_obj->flow.y = ply_obj->flow.z = 0;
			}
		}
	}

	/* 終了処理 */
	if (!ply_work->gmk_obj) {
		// ギミック終了
		//ply_work->gmk_obj = NULL;

		// ギミックステート初期化
		GmPlayerStateGimmickInit(ply_work);

#if 0
		/* 動作設定 */
		ply_obj->move_flag &= ~(OBD_MOVE_NOSPDM | OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);
		ply_obj->move_flag |= OBD_MOVE_FALL | OBD_MOVE_DIR;

		// 描画設定
		//ply_obj->disp_flag |= OBD_DISP_3D_PARALLEL;

		// プレイヤー設定
		ply_work->player_flag &= ~(GMD_PLF_NITRO_NOSTART | GMD_PLF_NOCAMERA_OFST);		// ニトロ開始可能 カメラ移動演出OFF終了

		// ギミックフラグ設定
		//ply_work->gmk_flag &= ~(GMD_PLGF_GMK_NONITROEFFECT | GMD_PLGF_CAMERA_GMK_X | GMD_PLGF_CAMERA_GMK_Y);

		// ギミックカメラ固定オフセット値クリア
		ply_work->gmk_camera_ofst_x = ply_work->gmk_camera_ofst_y = 0;

		// 角度クリア
		ply_obj->dir.x = ply_obj->dir.y = ply_obj->dir.z = 0;

		// SE停止
		//NNS_SndPlayerStopSeq(&ply_work->h_snd_se, 0);
#endif
	}
}

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
void GmPlySeqGmkSpdSet(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y)
{
	if (spd_x < 0) {
		ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
	}
	else if (spd_x > 0) {
		ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
	}
	
	// 速度設定
	if (ply_work->obj_work.move_flag & OBD_MOVE_JUMP) {
		// ジャンプ状態
		if ( ( (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd.x > spd_x) ||
			(!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd.x < spd_x) ) {
			ply_work->obj_work.spd.x = spd_x;
		}
		if ( MTM_MATH_ABS(ply_work->obj_work.spd.y) < MTM_MATH_ABS(spd_y) ) {
			ply_work->obj_work.spd.y = spd_y;
		}
	}
	else {
		switch ( (((ply_work->obj_work.dir.z + 0x2000) & 0xc000) >> 14)) {
		case 0:
			// 左が進行方向
		case 2:
			// 右が進行方向
			// 左右が進行方向
			if ( ( (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m > spd_x) ||
				(!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m < spd_x) ) {
				ply_work->obj_work.spd_m = spd_x;
			}
			if ( MTM_MATH_ABS(ply_work->obj_work.spd.y) < MTM_MATH_ABS(spd_y) ) {
				ply_work->obj_work.spd.y = spd_y;
				if ( ply_work->obj_work.spd.y < 0 ) {
					ply_work->obj_work.move_flag |= OBD_MOVE_JUMP;
				}
			}
			break;
		case 1:
			// 下が進行方向
#if 1
			if ( ( (spd_y > 0) && ply_work->obj_work.spd_m < spd_y) ||
				 ( (spd_y < 0) && ply_work->obj_work.spd_m > spd_y) ) {
				ply_work->obj_work.spd_m = spd_y;
			}

			if (ply_work->obj_work.spd_m > 0) {
				ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
			}
			else {
				ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
			}
#else
			if ( ( (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m > spd_y) ||
				 (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m < spd_y) ) {
				ply_work->obj_work.spd_m = spd_y;
			}
#endif

			if ( MTM_MATH_ABS(ply_work->obj_work.spd.x) < MTM_MATH_ABS(spd_x) ) {
				ply_work->obj_work.spd.x = spd_x;
			}
			break;
		case 3:
			// 上が進行方向
#if 1
			if ( ( (spd_y > 0) && ply_work->obj_work.spd_m > -spd_y) ||
				 ( (spd_y < 0) && ply_work->obj_work.spd_m < -spd_y) ) {
				ply_work->obj_work.spd_m = -spd_y;
			}

			if (ply_work->obj_work.spd_m > 0) {
				ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
			}
			else {
				ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
			}
#else
			if ( ( (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m > -spd_y) ||
				 (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m < -spd_y) ) {
				ply_work->obj_work.spd_m = -spd_y;
			}
#endif
			
			if ( MTM_MATH_ABS(ply_work->obj_work.spd.x) < MTM_MATH_ABS(spd_x) ) {
				ply_work->obj_work.spd.x = spd_x;
			}
			break;
		}
		
	}
}


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
void GmPlySeqGmkTruckSpdSet(GMS_PLAYER_WORK *ply_work, fx32 spd_x, fx32 spd_y)
{
	if (spd_x < 0) {
		ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_L;
	}
	else if (spd_x > 0) {
		ply_work->gmk_flag &= ~GMD_PLGF_GMK_TRUCK_L;
	}
	
	// 速度設定
	if (ply_work->obj_work.move_flag & OBD_MOVE_JUMP) {
		// ジャンプ状態
		if ( ( (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd.x > spd_x) ||
			(!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd.x < spd_x) ) {
			ply_work->obj_work.spd.x = spd_x;
		}
		if ( MTM_MATH_ABS(ply_work->obj_work.spd.y) < MTM_MATH_ABS(spd_y) ) {
			ply_work->obj_work.spd.y = spd_y;
		}
	}
	else {
		u16	dir, dir_temp;

		dir = (u16)(ply_work->obj_work.dir.z + ply_work->obj_work.dir_fall);// - ply_work->jump_pseudofall_dir);
		dir_temp =  (u16)(((dir + 0x2000) & 0xc000));
		dir_temp =  (u16)(((dir + 0x2000) & 0xc000) >> 14);

	//	dir = (u16)(ply_work->obj_work.dir.z + (ply_work->obj_work.dir_fall - g_gm_main_system.pseudofall_dir));
	//	dir_temp =  (((dir + 0x2000) & 0xc000));
	//	dir_temp =  (((dir + 0x2000) & 0xc000) >> 14);

		switch ( (((dir + 0x2000) & 0xc000) >> 14)) {
		case 0:
			// 右が進行方向
		case 2:
			// 左が進行方向
			// 左右が進行方向
			if ( ( (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_L) && ply_work->obj_work.spd_m > spd_x) ||
				(!(ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_L) && ply_work->obj_work.spd_m < spd_x) ) {
				ply_work->obj_work.spd_m = spd_x;
			}
			if ( MTM_MATH_ABS(ply_work->obj_work.spd.y) < MTM_MATH_ABS(spd_y) ) {
				ply_work->obj_work.spd.y = spd_y;
				if ( ply_work->obj_work.spd.y < 0 ) {
					ply_work->obj_work.move_flag |= OBD_MOVE_JUMP;
				}
			}
			break;

		case 1:
			// 下が進行方向
			if ( ( (spd_y > 0) && ply_work->obj_work.spd_m < spd_y) ||
				 ( (spd_y < 0) && ply_work->obj_work.spd_m > spd_y) ) {
				ply_work->obj_work.spd_m = spd_y;
			}

			if (ply_work->obj_work.spd_m > 0) {
				ply_work->gmk_flag &= ~GMD_PLGF_GMK_TRUCK_L;
			}
			else {
				ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_L;
			}
			
			if ( MTM_MATH_ABS(ply_work->obj_work.spd.x) < MTM_MATH_ABS(spd_x) ) {
				ply_work->obj_work.spd.x = spd_x;
			}
			break;
		case 3:
			// 上が進行方向
			if ( ( (spd_y > 0) && ply_work->obj_work.spd_m > -spd_y) ||
				 ( (spd_y < 0) && ply_work->obj_work.spd_m < -spd_y) ) {
				ply_work->obj_work.spd_m = -spd_y;
			}

			if (ply_work->obj_work.spd_m > 0) {
				ply_work->gmk_flag &= ~GMD_PLGF_GMK_TRUCK_L;
			}
			else {
				ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_L;
			}

			if ( MTM_MATH_ABS(ply_work->obj_work.spd.x) < MTM_MATH_ABS(spd_x) ) {
				ply_work->obj_work.spd.x = spd_x;
			}
			break;
		}
		
	}
}

//----- Local Functions -----------------------------------------------------


// ==========================================================================
// 岩乗り開始 GME_PLY_SEQ_STATE_GMK_ROCK_RIDE_START
// ==========================================================================

// ==========================================================================
// gmPlySeqGmkMainGimmickRockRidePush
/*!
 *	岩乗り開始 押す
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqGmkMainGimmickRockRidePush(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	amAssert( player_obj_work );
	player_obj_work->obj_3d->speed[0] -= 0.02f;
	if ( player_obj_work->obj_3d->speed[0] <= 0.5f ){
		player_obj_work->obj_3d->speed[0] = 0.5f;
	}

	//プッシュモーションが終わったらジャンプ
	if ( player_obj_work->disp_flag & OBD_DISP_END ){

		player_obj_work->user_timer = GMD_GMK_ROCK_RIDE_FRAME_WAIT_JUMP;
		// メイン処理設定
		ply_work->seq_func = gmPlySeqGmkMainGimmickRockRideStartWait;
		player_obj_work->obj_3d->speed[0] = 1.0f;
	}
}

// ==========================================================================
// gmPlySeqGmkMainGimmickRockRideStartWait
/*!
 *	岩乗り開始 ジャンプ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqGmkMainGimmickRockRideStartWait(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	amAssert( player_obj_work );

	--player_obj_work->user_timer;
	if ( player_obj_work->user_timer > 0 ){
		return;
	}
	player_obj_work->user_timer = 0;
	ply_work->seq_func = gmPlySeqGmkMainGimmickRockRideStartJump;

}

// ==========================================================================
// gmPlySeqGmkMainGimmickRockRideStartJump
/*!
 *	岩乗り開始 ジャンプ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqGmkMainGimmickRockRideStartJump(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	amAssert( player_obj_work );

	//待機モーションが終わったらジャンプ(当たり判定があるまで操作不能)
	if ( player_obj_work->disp_flag & OBD_DISP_END ){
		const OBS_OBJECT_WORK* gimmick_obj_work = ply_work->gmk_obj;
		amAssert( gimmick_obj_work );

		fx32 jump_x = GMD_GMK_ROCK_RIDE_START_JUMP_X;
		if ( gimmick_obj_work->pos.x < player_obj_work->pos.x ){
			jump_x = -jump_x;
		}
		GmPlySeqGmkInitGmkJump( ply_work, jump_x, GMD_GMK_ROCK_RIDE_START_JUMP_Y );
		ply_work->seq_func = gmPlySeqGmkMainGimmickRockRideStartFall;
	}
}

// ==========================================================================
// gmPlySeqGmkMainGimmickRockRideStartFall
/*!
 *	岩乗り ジャンプ落下
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqGmkMainGimmickRockRideStartFall(GMS_PLAYER_WORK *ply_work)
{
	//着地したらFWへ
	if ( ply_work->obj_work.move_flag & OBD_MOVE_UNDER ){
		GmPlySeqLandingSet(ply_work, 0);
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return ;
	}
}

// ==========================================================================
// 岩乗り GME_PLY_SEQ_STATE_GMK_ROCK_RIDE
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainGimmickRockRide
/*!
 *	岩乗り 
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqGmkMainGimmickRockRide(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	amAssert( player_obj_work );
	OBS_OBJECT_WORK* gimmick_obj_work = ply_work->gmk_obj;
	amAssert( gimmick_obj_work );

	//---------------------------------
	//とまる
	//---------------------------------
	if ( gimmick_obj_work->spd_m == 0 ){
		ply_work->seq_func = gmPlySeqGmkMainGimmickRockRideStop;

		//カメラ戻す
		GmPlayerCameraOffsetSet(ply_work, 0, 0);
		GmCameraAllowReset();
		return ;
	}

	//---------------------------------
	//傾きから加速度算出
	//---------------------------------
	Angle32 key_rot_z = GmPlayerKeyGetGimmickRotZ( ply_work );
	s32 angle = (s32)(-key_rot_z);
	float angle_per = (float)angle / (float)GMD_GMK_ROCK_RIDE_KEY_ANGLE_LIMIT;
	fx32 max_speed = FX_Mul(GMD_GMK_ROCK_RIDE_SPEED_RANGE, FX_F32_TO_FX32(angle_per) );
	if ( gimmick_obj_work->spd_m > 0 ){
		max_speed += GMD_GMK_ROCK_RIDE_SPEED_MID;
	}
	else{
		max_speed -= GMD_GMK_ROCK_RIDE_SPEED_MID;
	}

	fx32 add_speed = max_speed - player_obj_work->spd_m;
	if ( add_speed > 0 ){
		player_obj_work->spd_m += GMD_GMK_ROCK_RIDE_SPEED_ADD;
	}
	else if (add_speed < 0){
		player_obj_work->spd_m -= GMD_GMK_ROCK_RIDE_SPEED_ADD;
	}
	
	//---------------------------------
	//角度制限
	//---------------------------------
	//速度差から角度を算出
	fx32 roll = gimmick_obj_work->spd_m - player_obj_work->spd_m;
	s32	roll_val = MTM_MATH_ABS(roll);

	//弾かれる
//	printf("key_rot_z %d / roll %d \n", key_rot_z, roll_val);
	if ( roll_val >= GMD_GMK_ROCK_RIDE_SPEED_LIMIT ){
//		printf("out !roll %d \n", roll_val);
		fx32 jump_x = GMD_GMK_ROCK_RIDE_JUMP_X;
		if ( roll < 0 ){
			jump_x = -jump_x;
		}
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL);
		GmPlySeqGmkInitGmkJump( ply_work, jump_x, GMD_GMK_ROCK_RIDE_JUMP_Y );

		//カメラ戻す
		GmPlayerCameraOffsetSet(ply_work, 0, 0);
		GmCameraAllowReset();
		return ;
	}
	//おっとっとモーション変更
	else if (roll_val >= GMD_GMK_ROCK_RIDE_SPEED_PINCH){
		if ( ply_work->act_state != GME_PLY_ACT_STATE_GMK_BALL_02 ){
			GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_GMK_BALL_02 );
			player_obj_work->disp_flag |= OBD_DISP_REPEAT;
		}
	}
	//通常モーション
	else {
		if ( ply_work->act_state != GME_PLY_ACT_STATE_GMK_BALL_01 ){
			GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_GMK_BALL_01 );
			player_obj_work->disp_flag |= OBD_DISP_REPEAT;
		}
	}

	//---------------------------------
	//接着
	//---------------------------------
	//傾き
	GMS_ENEMY_COM_WORK* com_work = (GMS_ENEMY_COM_WORK*)gimmick_obj_work;
	//com_work->target_dp_dir.z = (u16)(roll * 0.8f);
	com_work->target_dp_dir.z = (u16)(roll * 4 / 5);

	//接着
	GmPlySeqGmkMainGimmickDepend( ply_work );
}

// ==========================================================================
// fmPlySeqGmkMainGimmickRockRideStop
/*!
 *	岩乗り 停止
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqGmkMainGimmickRockRideStop(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	amAssert( player_obj_work );

	GmPlySeqGmkInitGmkJump( ply_work, player_obj_work->spd.x, player_obj_work->spd.y );
}




// ==========================================================================
// 息継ぎ GME_PLY_SEQ_STATE_GMK_BREATHING
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainGimmickBreathing
/*!
 *	息継ぎ 停止
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqGmkMainGimmickBreathing(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	amAssert( player_obj_work );

	// 着地設定
	GmPlySeqLandingSet(ply_work, 0);

	//モーション終わったらFWに
	if ( player_obj_work->disp_flag & OBD_DISP_END ){
		// FWへ
		if ( player_obj_work->move_flag & OBD_MOVE_UNDER ){	
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		}
		else{
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL);
		}
	}
}


// ==========================================================================
// ダッシュパネル GME_PLY_SEQ_STATE_GMK_DASHPANEL
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainDashPanel
/*!
 *	ダッシュパネル
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer : ダッシュ時間
 */
// ==========================================================================
void gmPlySeqGmkMainDashPanel(GMS_PLAYER_WORK *ply_work)
{
	ply_work->obj_work.user_timer--;
	if (ply_work->obj_work.user_timer <= 0 ||
			!ply_work->obj_work.spd_m) {
		// 通常走りに
		// 速度設定を戻す
		GmPlayerSpdParameterSet(ply_work);
		// FWへ (速度が正常にのっていればWalkになる)
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}
}

// ==========================================================================
// ウォータースライダー GME_PLY_SEQ_STATE_GMK_WATER_SLIDER
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainWaterSlider
/*!
 *	ウォータースライダー
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 */
// ==========================================================================
void gmPlySeqGmkMainWaterSlider(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK* player_obj_work = &ply_work->obj_work;
	amAssert( player_obj_work );

	//離れた
	if ( !(player_obj_work->move_flag & OBD_MOVE_UNDER)	//設置していない 
	){
		//表示オフセット
		nnMakeUnitMatrix( &ply_work->ex_obj_mtx_r );

		//拡張マトリクス
		ply_work->gmk_flag &= ~GMD_PLGF_GMK_EXMTX_R;

		// FWへ
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return ;
	}
	//ジャンプ
	else if ( GmPlayerKeyCheckJumpKeyPush(ply_work) ){

		//表示オフセット
		nnMakeUnitMatrix( &ply_work->ex_obj_mtx_r );

		//拡張マトリクス
		ply_work->gmk_flag &= ~GMD_PLGF_GMK_EXMTX_R;

		//スピンジャンプ
		//player_obj_work->dir.z = 0;
		player_obj_work->spd_m /= 2;
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_JUMP);
	}
}

// ==========================================================================
// Ｓ字パイプ GME_PLY_SEQ_STATE_GMK_SPIPE
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainSpipe
/*!
 *	Ｓ字パイプ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer : ダッシュ時間
 */
// ==========================================================================
void gmPlySeqGmkMainSpipe(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->gmk_flag & GMD_PLGF_GMK_S_PIPE) {
		// Ｓ字パイプスピン継続
	    if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
			// Ｓ字パイプ中なのに地面から放れてしまった
				// 飛び出したソニックを地面方向に押し付け再接地させてみる
			fx32	offs = 0;
			offs = FX_Mul(0x0000a000/*(ply_work->obj_work.spd_m )*/, mtMathCos( ply_work->obj_work.dir.z - (s32)(MTD_MATH_MAX_ANGLE/4)));
			ply_work->obj_work.pos.x -= offs;// *2;
			offs = FX_Mul(0x0000a000/*(ply_work->obj_work.spd_m )*/, mtMathSin( ply_work->obj_work.dir.z - (s32)(MTD_MATH_MAX_ANGLE/4)));
			ply_work->obj_work.pos.y -= offs;// *2;
			ply_work->obj_work.spd.x = 0;
			ply_work->obj_work.spd.y = 0;
		}
	    ply_work->obj_work.move_flag &= ~(OBD_MOVE_JUMP | OBD_MOVE_NOSPDM);
		if (!ply_work->obj_work.spd_m) {
			// 速度０なら再加速
			ply_work->obj_work.spd_m = 0x2000;
			// SE
			GmSoundPlaySE("Spin");
		}
	} else {
		// 通常スピンに
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN);
//	    ply_work->seq_func = gmPlySeqSpinMain;
//		// シーケンスステート置き換え
//		ply_work->prev_seq_state = GME_PLY_SEQ_STATE_SPIN;
//		ply_work->seq_state = seq_state;

	}
	ply_work->gmk_flag &= ~GMD_PLGF_GMK_S_PIPE;
}

// ================================================================
// gmPlySeqGmkScrewMain
/*!
  スクリューメイン関数

  @note
    gmk_work0	ギミックX座標\n
    gmk_work1	ギミックY座標\n
    gmk_work2	スクリュータイプ\n
    gmk_work3	角度タイプ\n
	user_work	スクリューフラグ\n
	user_timer	移動距離\n
	timer		地面チェック無視時間\n
    
 */
// ================================================================
void gmPlySeqGmkScrewMain(GMS_PLAYER_WORK *ply_work)
{
	// 歩きアクション変更チェック
	GmPlayerWalkActionCheck(ply_work);

	const GMS_PLY_SEQ_STATE_DATA	*seq_state_data;
	seq_state_data	= ply_work->seq_state_data_tbl;
	// 落下チェック
	if ( /*ply_work->gmk_work2 != GMD_GMK_SCREW_TYPE_GRAIND && */MTM_MATH_ABS(ply_work->obj_work.spd_m ) < ply_work->spd2) {


	    // 足元が浮いていたら落下する
//		if ( GmPlayerFallCheck(ply_work) ) {
		if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
            ply_work->obj_work.dir.x =
	            ply_work->obj_work.dir.y =
				ply_work->obj_work.dir.z = 0;
			ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;	// 移動制御を戻す
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL);
            return;
		}
	}

	// ジャンプチェック
//	if (GmPlayerKeyCheckJumpKeyPush(ply_work)) {
///		if ( ply_work->ppJump ) {
//			if ( ply_work->gmk_work2 == GMD_GMK_SCREW_TYPE_GRAIND ) {
//				ply_work->obj_work.flag |= OBD_OBJECT_B;
//			}
//			ply_work->obj_work.dir.x =
//				ply_work->obj_work.dir.y =
//				ply_work->obj_work.dir.z = 0;
//			ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;	// 移動制御を戻す
//			ply_work->ppJump(ply_work);
//			return;
///		}
//	}

	// 着地チェック
    if ( ply_work->timer ) {
		--ply_work->timer;
	}
	else {
//		if ( ply_work->obj_work.user_work & GMD_GMK_SCREW_EVE_FLAG_UP ) {
//			ply_work->obj_work.move_flag &= ~OBD_MOVE_JUMP;
//		}

		// 着地
		if ( ply_work->obj_work.move_flag & (OBD_MOVE_UNDER | OBD_MOVE_BACK | OBD_MOVE_FRONT) ) {
            ply_work->obj_work.dir.x =
	            ply_work->obj_work.dir.y =
				ply_work->obj_work.dir.z = 0;
			ply_work->obj_work.move_flag &= ~OBD_MOVE_NOMOVE;	// 移動制御を戻す

//			if (ply_work->gmk_work2 == GMD_GMK_SCREW_TYPE_VERTICAL &&
//					!((ply_work->obj_work.user_work >> GMD_GMK_SCREW_LOOP_NUM_SHIFT) & 0x01)) {
//				// 縦スクリュー 半ループ偶数の時は移動量・向き反転
//				ply_work->obj_work.spd_m = -ply_work->obj_work.spd_m;
//				ply_work->obj_work.disp_flag ^= OBD_DISP_HFLIP;
//			}
			// 空中の速度状態に設定する
			ply_work->obj_work.spd.x = ply_work->obj_work.spd_m;

			// 着地設定
			GmPlySeqLandingSet(ply_work, 0);
			GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
//			if ( ply_work->gmk_work2 == GMD_GMK_SCREW_TYPE_GRAIND ) {
//				GmPlayerGraindInit(ply_work);
//			}
//			else {
//				ply_work->ppFw(ply_work);
//			}
			return;
		}
    }

//	if ( ply_work->gmk_work2 != GMD_GMK_SCREW_TYPE_GRAIND) {
//
//		if (!(ply_work->gmk_flag & GMD_PLGF_DH_BOARD)) {	// 滑降ボードでない
//			// アニメーション速度設定
//			GmPlayerAnimeSpeedSet(ply_work, ply_work->obj_work.spd_m );
//
//			if ( ply_work->act_state != GMD_PLY_STATE_SCREW1 ) {
//				// アクション設定
//				GmPlayerWalkActionCheck(ply_work);
//			}
//		}
//
//		// 移動速度設定
//		GmPlayerMoveWalk(ply_work);
		GmPlySeqMoveWalk(ply_work);
//	}
	// アニメーション速度設定
//	if (seq_state_data[ply_work->seq_state].check_attr &							gasdgas
//			GMD_PLY_SEQ_STATE_CHECK_ATTR_MTN_SPEED_WALK) {							gasdga
//		// 歩きタイプ速度設定														gsdgasga
//		GmPlayerAnimeSpeedSetWalk(ply_work, ply_work->obj_work.spd_m);				gsdgasd
//	}																				gragara

//	switch ( ply_work->gmk_work2 ) {
//	default:
//	case GMD_GMK_SCREW_TYPE_NORMAL:
		// 自前移動処理
		gmPlySeqGmkMoveScrew(ply_work, GMD_PLYGMK_SCREW_LENGTH, GMD_PLYGMK_SCREW_WIDTH, GMD_PLYGMK_SCREW_HEIGHT);
//		break;
//
//	case GMD_GMK_SCREW_TYPE_VERTICAL:
//		// 自前移動処理
//		if (ply_work->gmk_work3 == 0) {
//			gmPlayerGmkMoveScrewV(ply_work,
//	                           GMD_PLYGMK_V1_SCREW1_LENGTH, GMD_PLYGMK_V1_SCREW1_WIDTH, GMD_PLYGMK_V1_SCREW1_HEIGHT,
//	                           GMD_PLYGMK_V1_SCREW2_LENGTH, GMD_PLYGMK_V1_SCREW2_WIDTH, GMD_PLYGMK_V1_SCREW2_HEIGHT);
//		}
//		else {
//			gmPlayerGmkMoveScrewV(ply_work,
//	                           GMD_PLYGMK_V2_SCREW1_LENGTH, GMD_PLYGMK_V2_SCREW1_WIDTH, GMD_PLYGMK_V2_SCREW1_HEIGHT,
//	                           GMD_PLYGMK_V2_SCREW2_LENGTH, GMD_PLYGMK_V2_SCREW2_WIDTH, GMD_PLYGMK_V2_SCREW2_HEIGHT);
//		}
//		break;
//
//	case GMD_GMK_SCREW_TYPE_GRAIND:
//		// 自前移動処理
//		gmPlayerGmkMoveScrew(ply_work, GMD_PLYGMK_SCREWG_LENGTH, GMD_PLYGMK_SCREWG_WIDTH, GMD_PLYGMK_SCREWG_HEIGHT);
//		break;
  //  }

    
    // 前作定型ワープスクリュー処理
    // 1ループの距離をMTD_MATH_MAX_ANGLEに射影 (1ループは144ドット
    //ulLength = (u16)((ulLength ) * MTD_MATH_MAX_ANGLE / ( 144 ));
    //ulLength &= MTD_MATH_ANGLE_MASK;
    //lCos = mtMathCos( (u16)ulLength );
    //ply_work->obj_work.ucDirY = (u8)((ulLength + (MTD_MATH_ANGLE_MASK >> 2) & MTD_MATH_ANGLE_MASK) >> 9);
    //ply_work->obj_work.lPosX = ply_work->gmk_work0 + (64 << 8) - ( (lCos * 64) >> 4 );
    
    // usLength = (u16)((usLength + 0x04) * 1024 / (288 ));
    // usLength &= 0x03FF;
    // lCos = mtMathCos((u16)(usLength << 8));
    // ply_work->obj_work.lPosX = ply_work->gmk_work0 + (35 << 8) - ((lCos * (38 - ply_work->obj_work.rField[OBD_BOTTOM])) >> 4);
}

// ================================================================
// gmPlySeqGmkMoveScrew
/*!
  スクリュー横 移動関数、角度設定

  @parama ply_work			[io] プレイヤーワーク
  @parama screw_length	[in] １ループの距離
  @parama screw_width	[in] １ループの高さ
  @parama screw_height	[in] １ループ部の半径
    
  @note
    gmk_work0	ギミックX座標\n
    gmk_work1	ギミックY座標\n
    gmk_work2	スクリュータイプ\n
	user_work	スクリューフラグ\n
	user_timer	移動距離\n
	timer		地面チェック無視時間\n

 */
// ================================================================
void gmPlySeqGmkMoveScrew(GMS_PLAYER_WORK *ply_work,
                               fx32 screw_length, s16 screw_width, s16 screw_height)
{
	fx32	lCos;
    fx32	pos;
	fx32	percent;	// 0x1000 = 100%
	u16		angle_per;
	s8		loop_num;

#if 0
    // 距離計算、デバッガで確認
    {
        fx32 test;
        fx32 hen1,hen2;
        hen1 = (fx32)( screw_width ); // 横辺
        hen2 = (fx32)( (( screw_height /* 半径 */) * 2) * 3.14); // 縦辺
        hen1 = (hen1 * hen1) << FX32_SHIFT;
        hen2 = (hen2 * hen2) << FX32_SHIFT;
        test = mtMathSqrt( hen1 + hen2 );
        screw_length = (s32)(test >> 4); // 0x021e52 // ループの長さ

    }
#endif

    // 自前移動
	ply_work->obj_work.user_timer += MTM_MATH_ABS(ply_work->obj_work.spd_m);

    // 位置を設定
	pos = ply_work->obj_work.user_timer;

#if 0
    // 半ループをいくつ過ぎたか
    loop_num = (s8)((MTM_MATH_ABS(pos) + (screw_length >>2)) / (screw_length>>1));

    // 終了繋ぎチェック
    if ( loop_num >= ply_work->obj_work.user_work >> GMD_GMK_SCREW_LOOP_NUM_SHIFT ){
    }
#endif

	// 1ループあたりの距離の何割まで進んでいるか計算
	loop_num = (s8)(pos / screw_length);
	pos = pos - ( screw_length * loop_num );
    percent = ((pos << 8) / screw_length) << 4;	// FX_Div(pos, screw_length);
	angle_per = (u16)((percent << 4) & 0xFFFF);

	// 割合に合わせて角度を設定する
	ply_work->obj_work.dir.x = angle_per;
	if ( ply_work->obj_work.user_work & GMD_GMK_SCREW_EVE_FLAG_LEFT ) {
		ply_work->obj_work.dir.x = (u16)-ply_work->obj_work.dir.x;
	}

	if (ply_work->obj_work.dir.x < 0x4000) {
		ply_work->obj_work.dir.z = ply_work->obj_work.dir.x;
	}
    else if ( ply_work->obj_work.dir.x < 0x8000 ) {
		ply_work->obj_work.dir.z = (u16)(0x04000 - (ply_work->obj_work.dir.x - 0x4000));
    }
	else if ( ply_work->obj_work.dir.x < 0xc000 ) {
		ply_work->obj_work.dir.z = (u16)(ply_work->obj_work.dir.x - 0x8000);
    }
	else {
		ply_work->obj_work.dir.z = (u16)(0x10000 - ply_work->obj_work.dir.x);
	}
	ply_work->obj_work.dir.z >>= 1;

#if 1
    if ( ply_work->obj_work.dir.x < 0x8000 ) {
		ply_work->obj_work.dir.z = (u16)-ply_work->obj_work.dir.z;
	}
#else
    if ( ply_work->obj_work.dir.x >= 0x8000 ) {
		ply_work->obj_work.dir.z = (u16)-ply_work->obj_work.dir.z;
	}
	ply_work->obj_work.dir.z = (u16)-ply_work->obj_work.dir.z;
#endif

//	if ( ply_work->gmk_work2 == GMD_GMK_SCREW_TYPE_GRAIND ) {	// グラインドタイプは回転が逆
		// 両方グラインドタイプにあわせる
		ply_work->obj_work.dir.x = (u16)-ply_work->obj_work.dir.x;
//	}

    lCos = mtMathCos(angle_per);

	screw_height -= ply_work->obj_work.field_rect[OBD_BOTTOM];
//	if ( ply_work->obj_work.user_work & GMD_GMK_SCREW_EVE_FLAG_UP) {
		screw_height = (s16)-screw_height;	// ◆縦じゃない限り必ずここを通る？
//	}

    // 座標設定
    ply_work->obj_work.prev_pos.x = ply_work->obj_work.pos.x;		// 座標保存
    ply_work->obj_work.prev_pos.y = ply_work->obj_work.pos.y;

    /*if ( ply_work->obj_work.user_work & GMD_GMK_SCREW_EVE_FLAG_LEFT )
        ply_work->obj_work.lPosX = ply_work->gmk_work0 - (screw_width << 8) + ( (lCos * screw_width) >> 4 );
    else
        ply_work->obj_work.lPosX = ply_work->gmk_work0 + (screw_width << 8) - ( (lCos * screw_width) >> 4 );
    ply_work->obj_work.lPosY = ply_work->gmk_work1 + ((loop_num * screw_height) << 8) + (screw_height * percent);
     */
	if ( ply_work->obj_work.user_work & GMD_GMK_SCREW_EVE_FLAG_LEFT ) {
		ply_work->obj_work.pos.x = ply_work->gmk_work0 - ((loop_num * screw_width) << FX32_SHIFT) - (screw_width * percent);
	}
	else {
		ply_work->obj_work.pos.x = ply_work->gmk_work0 + ((loop_num * screw_width) << FX32_SHIFT) + (screw_width * percent);
	}
	ply_work->obj_work.pos.y = ply_work->gmk_work1 + (screw_height << FX32_SHIFT) - (lCos * screw_height);

	// 予定移動値設定
	ply_work->obj_work.move.x = ply_work->obj_work.pos.x - ply_work->obj_work.prev_pos.x;
	ply_work->obj_work.move.y = ply_work->obj_work.pos.y - ply_work->obj_work.prev_pos.y;
}



// ================================================================
// gmPlySeqGmkCannonWait
/*!
  大砲メイン関数

  @note
    
 */
// ================================================================
void gmPlySeqGmkCannonWait(GMS_PLAYER_WORK *ply_work)
{
	if( ply_work->obj_work.pos.y >= ply_work->gmk_obj->pos.y )
	{
		ply_work->obj_work.pos.y = ply_work->gmk_obj->pos.y;
		ply_work->obj_work.spd.y = 0;
		ply_work->obj_work.spd_add.y = 0;
		ply_work->obj_work.spd_add.y = 0;
		ply_work->obj_work.spd_fall  = 0;
		ply_work->seq_func = NULL;	//	処理なし
		ply_work->obj_work.move_flag &= ~OBD_MOVE_FALL;	//	
	}
}
//----------------------------------------------------------------


// ================================================================
// gmPlySeqGmkStopperMove
/*!
  ストッパーメイン関数

  @note
    ストッパー定位置まで位置補正
 */
// ================================================================
void gmPlySeqGmkStopperMove(GMS_PLAYER_WORK *ply_work)
{
	fx32	pos_diff;
	// Xpos補正
	ply_work->obj_work.pos.x = (ply_work->obj_work.pos.x + ply_work->gmk_obj->pos.x) / 2;
	pos_diff = MTM_MATH_ABS(ply_work->obj_work.pos.x - ply_work->gmk_obj->pos.x);
	if (pos_diff < (FX32_ONE/4)) {
		// ストッパーとの差分が小さければ同じ位置とする
		ply_work->obj_work.pos.x = ply_work->gmk_obj->pos.x;
	}

	// Ypos補正
	if (ply_work->obj_work.pos.y > ply_work->gmk_obj->pos.y) {
		// ソニックが下
		ply_work->obj_work.pos.y -= FX32_ONE * 8;
		if (ply_work->obj_work.pos.y < ply_work->gmk_obj->pos.y) {
			ply_work->obj_work.pos.y = ply_work->gmk_obj->pos.y;
		}
	} else {
		// ソニックが上
		ply_work->obj_work.pos.y += FX32_ONE * 8;
		if (ply_work->obj_work.pos.y > ply_work->gmk_obj->pos.y) {
			ply_work->obj_work.pos.y = ply_work->gmk_obj->pos.y;
		}
	}

	if (  (ply_work->obj_work.pos.x == ply_work->gmk_obj->pos.x)
		&&(ply_work->obj_work.pos.y == ply_work->gmk_obj->pos.y) ) {
		// 位置補正完了
		ply_work->seq_func = gmPlySeqGmkStopperWait;
	}
}
//----------------------------------------------------------------


// ================================================================
// gmPlySeqGmkStopperWait
/*!
  ストッパーメイン関数

  @note
    ストッパー演出終了まで待機
 */
// ================================================================
void gmPlySeqGmkStopperWait(GMS_PLAYER_WORK *ply_work)
{
	UNREFERENCED_PARAMETER(ply_work);
}
//----------------------------------------------------------------


// ================================================================
// gmPlySeqGmkStopperEnd
/*!
  ストッパーメイン関数

  @note
    	ストッパーのロックが終了しておっこちるところです。
 */
// ================================================================
void gmPlySeqGmkStopperEnd(GMS_PLAYER_WORK *ply_work)
{
	BOOL slot_end = FALSE;
	if (!ply_work->gmk_obj) {
		slot_end = TRUE;
	} else if (ply_work->gmk_obj->user_timer < (ply_work->obj_work.pos.y>>FX32_SHIFT) ) {
		slot_end = TRUE;
	}
	if (slot_end) {
		fx32	spd_y = ply_work->obj_work.spd.y;
		// シーケンスステート設定
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FALL);
		GmPlySeqGmkInitGmkJump(ply_work, 0, spd_y, FALSE);
		ply_work->gmk_obj = NULL;
		//	スピン状態を保持
		if (ply_work->act_state != GME_PLY_ACT_STATE_SPIN) {
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_SPIN);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
		ply_work->obj_work.flag &= ~OBD_OBJECT_NOHIT;	// ヒット復帰

	}
}
//----------------------------------------------------------------



// ================================================================
// gmPlySeqGmkSeesaw
/*!
  シーソーメイン処理関数

  @note
    
 */
// ================================================================
void gmPlySeqGmkSeesaw(GMS_PLAYER_WORK *ply_work)
{
	UNREFERENCED_PARAMETER(ply_work);
}
//----------------------------------------------------------------



// ================================================================
// gmPlySeqGmkMainPinball
/*!
  ピンボールメイン処理関数

  @note
    
 */
// ================================================================
void gmPlySeqGmkMainPinball(GMS_PLAYER_WORK *ply_work)
{
	ply_work->obj_work.user_timer--;
	
	//指定時間が来たらFWへ
	if ( ply_work->obj_work.user_timer <= 0 
		|| ply_work->obj_work.spd_m == 0 )
	{
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return ;
	}

	//空中に
	if ( !(ply_work->obj_work.move_flag & OBD_MOVE_UNDER) ){
		fx32 speed_x = FX_Mul(ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z));
		fx32 speed_y = FX_Mul(ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z));
		GmPlySeqInitPinballAir(ply_work, speed_x, speed_y );
		return ;
	}
}

// ================================================================
// gmPlySeqGmkMainPinballAir
/*!
  ピンボール（空中）メイン処理関数

  @note
    
 */
// ================================================================
void gmPlySeqGmkMainPinballAir(GMS_PLAYER_WORK *ply_work)
{
	if ( ply_work->obj_work.user_timer > 0 ) {
		ply_work->obj_work.user_timer--;
	}
	
	//指定時間が来たら移動受付
	if ( ply_work->obj_work.user_timer <= 0 && ply_work->obj_work.user_flag ){
		ply_work->player_flag &= ~GMD_PLF_NOJUMPMOVE;
	}

	//着地したらFWへ
	if ( ply_work->obj_work.move_flag & OBD_MOVE_UNDER ){
		ply_work->no_spddown_timer = 0;
		GmPlySeqLandingSet(ply_work, 0);
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return ;
	}
}

// ================================================================
// gmPlySeqGmkMainPinballCtpltHold
/*!
  シーソーメイン処理関数

  @note
    
 */
// ================================================================
void gmPlySeqGmkMainPinballCtpltHold(GMS_PLAYER_WORK *ply_work)
{
	UNREFERENCED_PARAMETER(ply_work);
}
// ================================================================
// gmPlySeqGmkMainPinballCtplt
/*!
  ピンボールメイン処理関数

  @note
    
 */
// ================================================================
void gmPlySeqGmkMainPinballCtplt(GMS_PLAYER_WORK *ply_work)
{
	//空中に
	if ( !(ply_work->obj_work.move_flag & OBD_MOVE_UNDER) ){
		fx32 speed_x = FX_Mul(ply_work->obj_work.spd_m, mtMathCos(ply_work->obj_work.dir.z));
		fx32 speed_y = FX_Mul(ply_work->obj_work.spd_m, mtMathSin(ply_work->obj_work.dir.z));
		GmPlySeqInitPinballAir(ply_work, speed_x, speed_y );
		return ;
	}
}


// ================================================================
// gmPlySeqGmkMainFlipper
/*!
  フリッパーメイン処理関数

  @note
    
 */
// ================================================================
void gmPlySeqGmkMainFlipper(GMS_PLAYER_WORK *ply_work)
{
	if ( MTM_MATH_ABS(ply_work->gmk_obj->pos.x - ply_work->obj_work.pos.x) > 54*FX32_ONE 
			|| MTM_MATH_ABS(ply_work->gmk_obj->pos.y - ply_work->obj_work.pos.y) > 32*FX32_ONE  
	){
		//プレイヤシーケンス変更
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		ply_work->obj_work.move_flag |= OBD_MOVE_FALL;
		ply_work->obj_work.move_flag &= ~(OBD_MOVE_NOSPDM | OBD_MOVE_NOCOL);
	}
}

// ==========================================================================
// 強制スピン GME_PLY_SEQ_STATE_GMK_FORCESPIN
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainForceSpin
/*!
 *	強制スピン
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer : 移動量
 *		user_flag  : 強制スピンタイプ OBD_OBJECT_USER_0 : 減速タイプ
 */
// ==========================================================================
void gmPlySeqGmkMainForceSpin(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->obj_work.spd_m == 0) {
		// 停止したら強制的に進む
		if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
			ply_work->obj_work.spd_m = (fx32)(-GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX);
		} else {
			ply_work->obj_work.spd_m = (fx32)(GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX);
		}
	    // SE
		GmSoundPlaySE("Spin");
	}
	if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
		// 強制スピン 落下へ
		// シーケンス変更
		GmPlySeqGmkInitForceSpinFall(ply_work);
	}
}

// ==========================================================================
// 強制スピン 減速タイプ GME_PLY_SEQ_STATE_GMK_FORCESPIN_DEC
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainForceSpinDec
/*!
 *	強制スピン 減速タイプ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer : 移動量
 *		user_flag  : 強制スピンタイプ OBD_OBJECT_USER_0 : 減速タイプ
 */
// ==========================================================================
void gmPlySeqGmkMainForceSpinDec(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->obj_work.spd_m == 0) {
		// 停止したら強制的に進む
		if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
			ply_work->obj_work.spd_m = (fx32)(-GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX);
		} else {
			ply_work->obj_work.spd_m = (fx32)(GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX);
		}
	    // SE
		GmSoundPlaySE("Spin");
	}

	// 一定以上の速度がある場合は減速する
	if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
		// 左向き
		if (ply_work->obj_work.spd_m < -GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX) {
			ply_work->obj_work.spd_m = ObjSpdDownSet(ply_work->obj_work.spd_m, -GMD_PLYGMK_FORCE_SPIN_DEC_SPDDEC);
// 最低速度維持しようとすると、進退窮まる場合があるので停止するまで減速させる@09/12/23 kuramoto
//			if (ply_work->obj_work.spd_m > -GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX) {
//				ply_work->obj_work.spd_m = -GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX;
//			}
		}
	}
	else {
		// 右向き
		if (ply_work->obj_work.spd_m > GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX) {
			ply_work->obj_work.spd_m = ObjSpdDownSet(ply_work->obj_work.spd_m, GMD_PLYGMK_FORCE_SPIN_DEC_SPDDEC);
// 最低速度維持しようとすると、進退窮まる場合があるので停止するまで減速させる@09/12/23 kuramoto
//			if (ply_work->obj_work.spd_m < GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX) {
//				ply_work->obj_work.spd_m = GMD_PLYGMK_FORCE_SPIN_DEC_SPDMAX;
//			}
		}
	}

	if (!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER)) {
		// 強制スピン 落下へ
		// シーケンス変更
		GmPlySeqGmkInitForceSpinFall(ply_work);
		ply_work->obj_work.spd_m = 0;		// 減速タイプ時はspd_mをクリア
	}
}

// ==========================================================================
// 強制スピン GME_PLY_SEQ_STATE_GMK_FORCESPIN_FALL
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainForceSpinFall
/*!
 *	強制スピン落下
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer : 移動量
 *		user_flag  : 強制スピンタイプ OBD_OBJECT_USER_0 : 減速タイプ
 *					 強制スピン, 強制スピン減速タイプ内で設定
 */
// ==========================================================================
void gmPlySeqGmkMainForceSpinFall(GMS_PLAYER_WORK *ply_work)
{
//	if (ply_work->obj_work.spd_m == 0) {
//		ply_work->obj_work.spd_m = ply_work->obj_work.user_timer;
//	}
	if (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) {
		// 強制スピンへ
		// シーケンス変更
		if (ply_work->obj_work.user_flag & OBD_OBJECT_USER_0) {
			// 減速タイプ
			GmPlySeqGmkInitForceSpinDec(ply_work);
		}
		else {
			// 通常タイプ
			GmPlySeqGmkInitForceSpin(ply_work);
		}
	}
}

// ==========================================================================
// 移動歯車 GME_PLY_SEQ_STATE_GMK_MOVE_GEAR
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainMoveGear
/*!
 *	移動歯車
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		ギミックのuser_flag OBD_OBJECT_USER_0 が有効の時は、
 *		移動量設定をギミックに任せます。
 *		また、ユーザーのジャンプを禁止します。
 */
// ==========================================================================
void gmPlySeqGmkMainMoveGear(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK	*gmk_obj;
	BOOL anime_spd_ctrl = TRUE;
	
	// ギミックチェック
	gmk_obj = ply_work->gmk_obj;
	if (gmk_obj == NULL) {
		// ギミックがなくなった
		GmPlySeqChangeFw(ply_work);
		return;
	}

	// ジャンプチェック
	if (!(gmk_obj->user_flag & OBD_OBJECT_USER_0) &&
			GmPlayerKeyCheckJumpKeyPush(ply_work)) {
		// 速度クリア
		ply_work->obj_work.spd_m = 0;
		ply_work->obj_work.spd.x = ply_work->obj_work.spd.y = 0;
		// ジャンプへ移行
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_JUMP);
		return;
	}

	// 常に地面接触状態に
	ply_work->obj_work.move_flag |= OBD_MOVE_UNDER;

	if (gmk_obj->user_flag & OBD_OBJECT_USER_1) {
	// スイッチ終点状態
		// 速度クリア
		ply_work->obj_work.spd_m = 0;
		ply_work->obj_work.spd.x = ply_work->obj_work.spd.y = 0;
	}

#if 0	// ■■■■終点挙動(ishizaki ver)■■■■
	// アクション設定
	if (ply_work->act_state != GME_PLY_ACT_STATE_TURN &&
		((GmPlayerKeyCheckWalkLeft(ply_work) && !(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m <= 0) ||
			(GmPlayerKeyCheckWalkRight(ply_work) && (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m >= 0) ||
			((gmk_obj->user_flag & OBD_OBJECT_USER_0) &&
				((!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m <= 0) ||
				((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m >= 0))) ) ) {
		// ターン
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_TURN);
		// プログラムターン有り
		GmPlySeqSetProgramTurnFwTurn(ply_work);
	}
	else if (ply_work->act_state == GME_PLY_ACT_STATE_TURN) {
		// ターン終了待機中
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// 反転
			GmPlayerSetReverseOnlyState(ply_work);
			// FW
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_FW);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
	}
	else if ((gmk_obj->user_flag & OBD_OBJECT_USER_1) &&
				((GmPlayerKeyCheckWalkLeft(ply_work) && (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP)) ||
				(GmPlayerKeyCheckWalkRight(ply_work) && !(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP))) ) {
		// 終点状態で向いている方向に進もうとしている時
#if GMD_GMK_GEAR_STAGGER_BACK
		if (ply_work->act_state != GME_PLY_ACT_STATE_STAGGER_B) {
			// おっとっとアクション設定
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_STAGGER_B);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
#else
		if (ply_work->act_state != GME_PLY_ACT_STATE_STAGGER_F) {
			// おっとっとアクション設定
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_STAGGER_F);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
#endif
#else	// ■■■■終点挙動(kuramoto ver)■■■■
	// アクション設定
	if (  (ply_work->act_state != GME_PLY_ACT_STATE_TURN)																						// ターン中でなくて
		&&(  (  (!(gmk_obj->user_flag & OBD_OBJECT_USER_1))																						//   スイッチ終端状況でない時に
			  &&(  (GmPlayerKeyCheckWalkLeft(ply_work) && !(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m <= 0) 	//     右から左を向こうとしているか？
			     ||(GmPlayerKeyCheckWalkRight(ply_work) && (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m >= 0) ) )//     左から右を向こうとしているか？
		   ||(	((gmk_obj->user_flag & OBD_OBJECT_USER_0)																						//   強制移動中に
			  &&(  (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m <= 0)											//     左を向くべき状況か？
				 ||(( ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && ply_work->obj_work.spd_m >= 0) ) ) ) 									//     右を向くべき状況か？
		   ||(  (gmk_obj->user_flag & OBD_OBJECT_USER_1)																						//   スイッチ終端状況の時に
			  &&(gmk_obj->user_work == 7)																										//   振り向き可能な歯車状況で
			  &&(  (GmPlayerKeyCheckWalkLeft(ply_work) && !(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && gmk_obj->user_timer <= 0)			//     右から左を向こうとしているか？
				 ||(GmPlayerKeyCheckWalkRight(ply_work) && (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) && gmk_obj->user_timer >= 0) ) )		//     左から右を向こうとしているか？
		  ) ) {
		// ターン
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_TURN);
		// プログラムターン有り
		GmPlySeqSetProgramTurnFwTurn(ply_work);
	}
	else if (ply_work->act_state == GME_PLY_ACT_STATE_TURN) {
		// ターン終了待機中
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// 反転
			GmPlayerSetReverseOnlyState(ply_work);
			// FW
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_FW);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
	}
	else if ((gmk_obj->user_flag & OBD_OBJECT_USER_1) &&
				((GmPlayerKeyCheckWalkLeft(ply_work) && (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP)) ||
				(GmPlayerKeyCheckWalkRight(ply_work) && !(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP))) ||
				(gmk_obj->user_timer != 0)) {
		// 終点状態で向いている方向に進もうとしている時

//----test
//		if (  (gmk_obj->user_work == 0)
//			||(gmk_obj->user_work == 4) ) {
		if (  (ply_work->ring_num == 0)
			&&(  (gmk_obj->user_work == 0)
			   ||(gmk_obj->user_work == 4) ) ) {
			if (ply_work->act_state != GME_PLY_ACT_STATE_STAGGER_F) {
//			if (ply_work->act_state != GME_PLY_ACT_STATE_STAGGER_D) {
			// おっとっとアクション設定
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_STAGGER_F);
//				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_STAGGER_D);
				ply_work->obj_work.obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD / 4;
				ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
			}
			ply_work->obj_work.obj_3d->speed[0] = 0.5f;//0.4f;
			ply_work->obj_work.obj_3d->speed[1] = 0.5f;//0.4f;
			anime_spd_ctrl = FALSE;
		} else 
//----test
		if (  (  (  (!(gmk_obj->user_flag & OBD_OBJECT_USER_3))			// 横移動タイプで
			      &&(GmPlayerKeyCheckWalkRight(ply_work) ) )			//   右に移動しようとしたとき
			   ||(  (gmk_obj->user_flag & OBD_OBJECT_USER_3)			// 縦移動タイプで
			    &&(GmPlayerKeyCheckWalkLeft(ply_work) ) ) )				//   左に移動しようとしたとき
			&&(gmk_obj->user_work == 7)									// 揺り戻し後で
			&&(gmk_obj->user_timer == 0) ) {							// 歯車回転速度ゼロの時
			// 歯車回転の逆向きに歩く時はロック解除し歩きへ
			GmPlySeqChangeFw(ply_work);
			return;
		} else
		if (  (gmk_obj->user_work == 1)			// user_work = gear_work->move_stagger_stepのコピー
		   ||(gmk_obj->user_work == 2) ) {		// :プレイヤーに状況伝えるため、利便性の高いuser_workを利用
			// 逆回転
			if (ply_work->act_state != GME_PLY_ACT_STATE_GMK_BALL_01) {
				GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_GMK_BALL_01 );
				ply_work->obj_work.obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD / 4;
				ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
			}
		} else {
			// 順回転
//			GmPlayerWalkActionCheck(ply_work);
			if (ply_work->act_state != GME_PLY_ACT_STATE_RUN) {
				GmPlayerActionChange( ply_work, GME_PLY_ACT_STATE_RUN );
//				ply_work->obj_work.obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD / 4;
				ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
			}
		}
//		gmPlySeqGmkMoveGearAnimeSpeedSetWalk(ply_work, gmk_obj->user_timer);
		if (ply_work->act_state != GME_PLY_ACT_STATE_STAGGER_F)	//test
//		if (ply_work->act_state != GME_PLY_ACT_STATE_STAGGER_D)	//test
		{
			// アニメーション速度を独自にセット
			anime_spd_ctrl = FALSE;
			fx32	spd, spd_set;
//			spd_set = gmk_obj->user_timer;
			spd_set = gmk_obj->user_timer * 3;
			spd = (fx32)MTM_MATH_ABS((spd_set >> 3) + (spd_set >> 2));

			if (spd <= (fx32)(0.25 * FX32_ONE)) {
				spd = (fx32)(0.25 * FX32_ONE);
			}
			if (spd >= 8 * FX32_ONE) {
				spd = 8 * FX32_ONE;
			}
			ply_work->obj_work.obj_3d->speed[0] = FXM_FX32_TO_FLOAT(spd);
			ply_work->obj_work.obj_3d->speed[1] = FXM_FX32_TO_FLOAT(spd);
		}
#endif	// ■■■■終点挙動■■■■
	}
	else if (ply_work->obj_work.spd_m != 0) {
		// 歩きアクション設定
		GmPlayerWalkActionCheck(ply_work);
	}
	else if (ply_work->act_state != GME_PLY_ACT_STATE_FW) {
		// FW
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_FW);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}

	// 速度値の移動のみ行う
	if (!(gmk_obj->user_flag & (OBD_OBJECT_USER_0 | OBD_OBJECT_USER_1))) {
		if (gmk_obj->user_flag & OBD_OBJECT_USER_2) {
			// 速いタイプ
			gmPlySeqGmkMoveGearMove(ply_work, TRUE);
		}
		else {
			// 遅いタイプ
			gmPlySeqGmkMoveGearMove(ply_work, FALSE);
		}
	}

	// 歩きタイプ速度設定
	if (anime_spd_ctrl)
		gmPlySeqGmkMoveGearAnimeSpeedSetWalk(ply_work, ply_work->obj_work.spd_m);

	// 接着処理
	GmPlySeqGmkMainGimmickDepend(ply_work);
}

// ==========================================================================
// gmPlySeqGmkMoveGearMove
/*!
 *	移動歯車 プレイヤー移動設定関数
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	spd_up_type	[in]	スピードアップタイプ
 */
// ==========================================================================
void gmPlySeqGmkMoveGearMove(GMS_PLAYER_WORK *ply_work, BOOL spd_up_type)
{
	fx32		spd_add;// = 0;
	fx32		spd_max;// = 0;
	fx32		spd_dec;// = 0;

	if (!spd_up_type) {
		// 遅いタイプ
		if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x0400) {
			spd_add = ply_work->spd_add >> 3;
			spd_dec = ply_work->spd_dec;
		}
		else if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x0800) {
			spd_add = ply_work->spd_add >> 2;
			spd_dec = (ply_work->spd_dec >> 1) + (ply_work->spd_dec >> 2);
		}
		else if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x0C00) {
			spd_add = ply_work->spd_add >> 1;
			spd_dec = ply_work->spd_dec >> 1;
		}
		else if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x1000) {
			spd_add = (ply_work->spd_add >> 1) + (ply_work->spd_add >> 2);
			spd_dec = ply_work->spd_dec >> 2;
		}
		else {
			spd_add = ply_work->spd_add;
			spd_dec = ply_work->spd_dec >> 3;
		}
	}
	else {
		// 速度UPタイプ
		if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x0C00) {
			spd_add = ply_work->spd_add >> 1;
			spd_dec = ply_work->spd_dec;
		}
		else if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x1000) {
			spd_add = (ply_work->spd_add >> 1) + (ply_work->spd_add >> 2);
			spd_dec = (ply_work->spd_dec >> 1) + (ply_work->spd_dec >> 2);
		}
		else {
			spd_add = ply_work->spd_add;
			spd_dec = ply_work->spd_dec >> 1;
		}
	}
#if 0
	if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x0400) {
		spd_add = ply_work->spd_add >> 2;	// 加速度調整
		spd_dec = ply_work->spd_dec >> 2;
	}
	else if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x0800) {
		spd_add = ply_work->spd_add >> 1;	// 加速度調整
		spd_dec = ply_work->spd_dec >> 1;
	}
	else if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x0C00) {
		spd_add = ply_work->spd_add >> 1;	// 加速度調整
		spd_dec = ply_work->spd_dec >> 1;
	}
	else if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x1000) {
		spd_add = (ply_work->spd_add >> 1) + (ply_work->spd_add >> 2);	// 加速度調整
		spd_dec = (ply_work->spd_dec >> 1) + (ply_work->spd_dec >> 2);
	}

	if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 0x1200) {
		spd_add = ply_work->spd_add >> 1;	// 加速度調整
		spd_dec = ply_work->spd_dec >> 1;
	}
	else {
		spd_add = ply_work->spd_add;
		spd_dec = ply_work->spd_dec;
	}
#endif
	spd_max = (ply_work->spd_max >> 1) + (ply_work->spd_max >> 2);

	// 傾斜角に合わせて最高速度設定
	if (GmPlayerKeyCheckWalkRight(ply_work) ||
			GmPlayerKeyCheckWalkLeft(ply_work)) {		// 0になった時点で通常のMAX速度でチェックする
		s32	roll = MTM_MATH_ABS(ply_work->key_walk_rot_z);

		if (roll > GMD_PL_DEF_ROLL_MAX) {
			roll = GMD_PL_DEF_ROLL_MAX;
		}
		spd_max = spd_max * roll / GMD_PL_DEF_ROLL_MAX;
	}
	if (spd_max < ply_work->prev_walk_roll_spd_max) {
		// 減速は緩やかに
		spd_max = ply_work->prev_walk_roll_spd_max - spd_dec;
		if (spd_max < 0) {
			spd_max = 0;
		}
	}
	ply_work->prev_walk_roll_spd_max = spd_max;

    // 角度に合わせて最大速度調整
	if (ply_work->obj_work.dir.z) {
		fx32 spd = FX_Mul(ply_work->spd_max_add_slope, mtMathSin(ply_work->obj_work.dir.z));
		if (spd > 0) {
			spd_max += spd;
		}
	}
    
	if (ply_work->no_spddown_timer) {
		spd_dec = 0;
	}
    // 速度に合わせて加速度修正
	else {
		fx32 percent = 0;
		if (MTM_MATH_ABS(ply_work->obj_work.spd_m) <= ply_work->spd1) {
			spd_add = spd_add * 5 / 8;
		}
		else if (MTM_MATH_ABS(ply_work->obj_work.spd_m) <= ply_work->spd2) {
			spd_add >>= 1;
		}
		//else if (MTM_MATH_ABS(ply_work->obj_work.spd_m) <= ply_work->spd3) {
		//	//spd_add = spd_add;
		//}
		else if (MTM_MATH_ABS(ply_work->obj_work.spd_m) > ply_work->spd3) {
			if (spd_max - ply_work->spd3) {
				percent = FX_Div(MTM_MATH_ABS(ply_work->obj_work.spd_m) - ply_work->spd3, spd_max - ply_work->spd3);
				if (percent > FX32_ONE/*0x0100*/) {
					percent = FX32_ONE;
				}
			}
			else {
				percent = FX32_ONE;
			}
			// 倍率調整
			percent = (percent * 0x00f80) >> FX32_SHIFT;

			spd_add = spd_add - FX_Mul(spd_add, percent);
		}
	}

	// 水中対応
//	if (ply_work->player_flag & GMD_PLF_WATER) {
//		GMD_PLAYER_WATER_SET(spd_add);
//		GMD_PLAYER_WATER_SET(spd_dec);
//	}

	// 坂道瞬間最大速度アップ
	if (ply_work->spd_work_max >= spd_max &&
			MTM_MATH_ABS(ply_work->obj_work.spd_m) >= spd_max) {
		if (ply_work->spd_work_max > ply_work->obj_work.spd_m) {
			ply_work->spd_work_max = MTM_MATH_ABS(ply_work->obj_work.spd_m);
		}
		spd_max = ply_work->spd_work_max;
	}

	// オートラン最高速度補正
	if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {
		if (GmPlayerKeyCheckWalkRight(ply_work) &&
				(spd_max > ply_work->scroll_spd_x + GMD_PL_AUTO_RUN_MAX_SPD_OFST)) {
			// 右移動中は最高速度減少
			spd_max = ply_work->scroll_spd_x + GMD_PL_AUTO_RUN_MAX_SPD_OFST;
		}
	}

	// 加速
	{
	// 通常
		if (GmPlayerKeyCheckWalkLeft(ply_work) | GmPlayerKeyCheckWalkRight(ply_work)) {
			if (GmPlayerKeyCheckWalkRight(ply_work)) {
			// 反転はシーケンス変更部で
				if ( ply_work->obj_work.spd_m < 0 ) {
					ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, spd_dec);
				}
				ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m, spd_add, spd_max);
			}
			else {
			// 反転はシーケンス変更部で
				if ( ply_work->obj_work.spd_m > 0 ) {
					ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, spd_dec);
				}
				ply_work->obj_work.spd_m = ObjSpdUpSet( ply_work->obj_work.spd_m,-spd_add, spd_max);
			}
		}
		else {
			ply_work->spd_pool = 0;
			ply_work->obj_work.spd.x = MTM_MATH_CLIP( ply_work->obj_work.spd.x, -spd_max, spd_max );
			ply_work->obj_work.spd_m = MTM_MATH_CLIP( ply_work->obj_work.spd_m, -spd_max, spd_max );
			if ( (((ply_work->obj_work.dir.z + 0x2000) & 0xff00) <= ( 0x2000 << 1)) ) {
				// 減速
				if (ply_work->player_flag & GMD_PLF_NOBRAKE) {
					// このフレームは減速できません
					ply_work->player_flag &= ~GMD_PLF_NOBRAKE;
					return;
				}
				if (ply_work->player_flag & GMD_PLF_AUTO_RUN) {
					// オートラン時減速
					fx32	min_spd;

					if (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) &&
							ply_work->seq_state == GME_PLY_SEQ_STATE_WALK) {
						min_spd = ply_work->scroll_spd_x + GMD_PL_AUTO_RUN_FREE_DEC_MIN_SPD_OFST;
						if (min_spd < 0) {
							min_spd = 0;
						}

						ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, spd_dec);
						if (ply_work->obj_work.spd_m < min_spd) {
							// 右向き走りでキーを離した減速では一定速度以下にはならない
							ply_work->obj_work.spd_m = min_spd;
						}
					}
					else {
						// 通常減速
						ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, spd_dec);
					}
				}
				else {
					// 通常減速
					ply_work->obj_work.spd_m = ObjSpdDownSet( ply_work->obj_work.spd_m, spd_dec);
				}
	        }
	    }
    }
}

// ==========================================================================
// gmPlySeqGmkMoveGearAnimeSpeedSetWalk
/*!
 *	移動歯車 アニメーション速度設定
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqGmkMoveGearAnimeSpeedSetWalk(GMS_PLAYER_WORK *ply_work, fx32 spd_set)
{
	fx32	spd;

	if (GME_PLY_ACT_STATE_WALK <= ply_work->act_state &&
				ply_work->act_state <= GME_PLY_ACT_STATE_DASH_1) {
		spd = (fx32)MTM_MATH_ABS((spd_set >> 3) + (spd_set >> 2));

		if (spd <= (fx32)(0.5 * FX32_ONE)) {
			spd = (fx32)(0.5 * FX32_ONE);
		}
		if (spd >= 8 * FX32_ONE) {
			spd = 8 * FX32_ONE;
		}
	}
	else {
		// 最大ダッシュの時, 歩きモーション以外は等速
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

// ==========================================================================
// スチームパイプ GME_PLY_SEQ_STATE_GMK_STEAMPIPE
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainSteamPipe
/*!
 *	スチームパイプ処理
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer : 移動量
 */
// ==========================================================================
void gmPlySeqGmkMainSteamPipe(GMS_PLAYER_WORK *ply_work)
{
	UNREFERENCED_PARAMETER(ply_work);

	if (ply_work->obj_work.user_timer < 60*FX32_ONE) {
		float	per;
		ply_work->obj_work.user_timer = ObjTimeCountUp(ply_work->obj_work.user_timer);
		per = FXM_FX32_TO_FLOAT(ply_work->obj_work.user_timer) / 60.f * 2;

		// モーション速度アップ
		ply_work->obj_work.obj_3d->speed[0] = 1.f + per;
		ply_work->obj_work.obj_3d->speed[1] = 1.f + per;
	}
}

// ==========================================================================
// 排液装置 GME_PLY_SEQ_STATE_GMK_DRAIN_TANK
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainDrainTank
/*!
 *	排液装置
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer : 移動量
 */
// ==========================================================================
void gmPlySeqGmkMainDrainTank(GMS_PLAYER_WORK *ply_work)
{
	//着地したらFWへ
	if ( ply_work->obj_work.move_flag & OBD_MOVE_UNDER ){
		GmPlySeqLandingSet(ply_work, 0);
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
		return ;
	}

	//加速度調整
	if ( ply_work->obj_work.spd_add.x > 0 ){
		if ( ply_work->obj_work.spd.x >= 0 ){
			ply_work->obj_work.spd_add.x = 0;
			ply_work->obj_work.spd.x = 0;
		}
	}
	else if ( ply_work->obj_work.spd_add.x < 0 ){
		if ( ply_work->obj_work.spd.x <= 0 ){
			ply_work->obj_work.spd_add.x = 0;
			ply_work->obj_work.spd.x = 0;
		}
	}
}

// ==========================================================================
// gmPlySeqGmkMainBoss5Quake
/*!
 *	ボスFINAL 地球割り着地振動
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		user_timer : 移動量
 */
// ==========================================================================
void gmPlySeqGmkMainBoss5Quake(GMS_PLAYER_WORK *ply_work)
{
	if (ply_work->obj_work.user_timer > 0) {
		ply_work->obj_work.user_timer--;
	}
	else {
		// タイマ完了したらFWへ
		
		GmPlySeqLandingSet(ply_work, 0);
		
		ply_work->obj_work.move_flag	&= ~OBD_MOVE_NOMOVE;
		
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_FW);
	}
}

// ==========================================================================
// エンディング演出１ GME_PLY_SEQ_STATE_GMK_ENDING_DEMO1
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainEndingFrontSide
/*!
 *	エンディング演出 手前を見る
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqGmkMainEndingFrontSide(GMS_PLAYER_WORK *ply_work)
{
	if (  (ply_work->act_state == GME_PLY_ACT_STATE_GMK_ENDING_FR1)
		&&(ply_work->obj_work.disp_flag & OBD_DISP_END) ) {
		// 手前を見る(ループ)アクションセット
		GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_ENDING_FR2);
		ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
	}
}
// ==========================================================================
// エンディング演出２ GME_PLY_SEQ_STATE_GMK_ENDING_DEMO2
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainEndingFinish
/*!
 *	エンディング演出 フィニッシュ
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 */
// ==========================================================================
void gmPlySeqGmkMainEndingFinish(GMS_PLAYER_WORK *ply_work)
{
	// 拡大制御
	if (  (ply_work->act_state != GME_PLY_ACT_STATE_GMK_ENDING_FN12)
		&&(ply_work->act_state != GME_PLY_ACT_STATE_GMK_ENDING_FN22)
		&&(ply_work->act_state != GME_PLY_ACT_STATE_GMK_ENDING_FNS2) ) {
#if 0	// 拡大速度等速
		ply_work->obj_work.scale.x += 0x00c0;
		ply_work->obj_work.scale.y += 0x00c0;
		ply_work->obj_work.scale.z += 0x00c0;
#else	// 拡大速度加速
		ply_work->obj_work.user_work += 0x04;
		ply_work->obj_work.scale.x += ply_work->obj_work.user_work;
		ply_work->obj_work.scale.y += ply_work->obj_work.user_work;
		ply_work->obj_work.scale.z += ply_work->obj_work.user_work;
#endif
		ply_work->obj_work.pos.z += 0x0400;	// 徐々に優先を上げる
	}

	// アクション変更
	if (ply_work->act_state == GME_PLY_ACT_STATE_JUMP_SPIN) {
		// ジャンプ中
		if (ply_work->obj_work.spd.y > -0x1000) {
			// フィニッシュ(移行)ポーズ１セット
			if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
				// スーパーソニック
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_ENDING_FNS1);
			} else {
				// 通常ソニック
				if (ply_work->obj_work.user_flag) {
					// type2
					GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_ENDING_FN21);
				} else {
					// type1
					GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_ENDING_FN11);
				}
			}
			return;
		}
	}

	if (  (  (ply_work->act_state == GME_PLY_ACT_STATE_GMK_ENDING_FN11)
		   ||(ply_work->act_state == GME_PLY_ACT_STATE_GMK_ENDING_FN21)
		   ||(ply_work->act_state == GME_PLY_ACT_STATE_GMK_ENDING_FNS1) )
		&&(ply_work->obj_work.disp_flag & OBD_DISP_END) ) {
		// フィニッシュ(キメ)アクションセット
		if (ply_work->player_flag & GMD_PLF_SUPER_SONIC) {
			// スーパーソニック
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_ENDING_FNS2);
		} else {
			// 通常ソニック
			if (ply_work->obj_work.user_flag) {
				// type2
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_ENDING_FN22);
			} else {
				// type1
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_ENDING_FN12);
			}
		}
		ply_work->obj_work.move_flag |= OBD_MOVE_NOMOVE;
#if _WII | _IPHONE
		ply_work->obj_work.disp_flag |= OBD_DISP_NODISP;					// Wii(とiPhone)は一枚絵に差し替え
#endif//_WII | _IPHONE
		GmEndingTrophySet();
	}

}

// ==========================================================================
// トロッコ GME_PLY_SEQ_STATE_GMK_TRUCK_DANGER
// ==========================================================================
// ==========================================================================
// gmPlayerCheckTruckAirFoot
/*!
 *	トロッコ足浮きチェック
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@return		0x01 : トロッコ前方接地あり  0x02 : トロッコ後方接地あり
 */
// ==========================================================================
u32 gmPlayerCheckTruckAirFoot(GMS_PLAYER_WORK *ply_work)
{
	OBS_OBJECT_WORK		*obj_work;
	OBS_COL_CHK_DATA	col_data;
	s32					diff_1, diff_2;
	u32					ret_val = 0;
	fx32				ofst_x, ofst_y;
	fx32				base_pos_x = 0, base_pos_y = 0;
	u16					now_dir, dir_type;
					
	if (ply_work->obj_work.ride_obj) {
		// オブジェクト地形に乗っている時はよろけチェックを行わない
		return (ret_val);
	}

	obj_work = (OBS_OBJECT_WORK*)ply_work;
	now_dir = (u16)(obj_work->dir.z + obj_work->dir_fall);

	// 基本情報設定
	col_data.flag	= (u16)(obj_work->flag & OBD_OBJECT_B);
	col_data.vec	= OBD_COL_DOWN;
	col_data.dir	= NULL;
	col_data.attr	= NULL;

	dir_type = (u16)(((obj_work->dir.z + 0x2000) & 0xc000) >> 14);
	dir_type += (u16)(((obj_work->dir_fall + 0x2000) & 0xc000) >> 14);
	dir_type &= 0x3;
	switch (dir_type) {
	case 0:
		// 下方向が進行方向
		col_data.vec = OBD_COL_DOWN;
		break;
	case 1:
		// 左方向が進行方向
		col_data.vec = OBD_COL_LEFT;
		break;
	case 2:
		// 上方向が進行方向
		col_data.vec = OBD_COL_UP;
		break;
	case 3:
		// 右方向が進行方向
		col_data.vec = OBD_COL_RIGHT;
		break;
	}

	// 基点からチェック座標取得
	if (now_dir & 0x3FFF) {
		NNS_VECTOR	point_a, point_b, center, point_h;
		NNS_VECTOR	line_ab, line_ac;
		float		l_scale;
		// 接触点取得
		switch ((now_dir & 0xC000) >> 14) {
		case 0:	// 左下
			base_pos_x = obj_work->field_rect[OBD_LEFT] - 2;
			base_pos_y = obj_work->field_rect[OBD_BOTTOM] + 2;
			break;
		case 1:	// 左上
			base_pos_x = obj_work->field_rect[OBD_LEFT] - 2;
			base_pos_y = obj_work->field_rect[OBD_TOP] - 2;
			break;
		case 2:	// 右上
			base_pos_x = obj_work->field_rect[OBD_RIGHT] + 2;
			base_pos_y = obj_work->field_rect[OBD_TOP] - 2;
			break;
		case 3:	// 右下
			base_pos_x = obj_work->field_rect[OBD_RIGHT] + 2;
			base_pos_y = obj_work->field_rect[OBD_BOTTOM] + 2;
			break;
		}
		// 接触点から本来のチェック座標を取得する
		// 接触点から一定の距離にある点を角度をかけて中心からの垂線を下ろした点を取る
		point_a.x = (float)base_pos_x;
		point_a.y = (float)-base_pos_y;
		point_a.z = 0;

		point_b.x = base_pos_x + 10.f * nnCos(-now_dir);
		point_b.y = -base_pos_y + 10.f * nnSin(-now_dir);
		point_b.z = 0;

		center.x = center.y = center.z = 0;

		line_ab.x = point_b.x - point_a.x;
		line_ab.y = point_b.y - point_a.y;
		line_ab.z = point_b.z - point_a.z;
		line_ac.x = center.x - point_a.x;
		line_ac.y = center.y - point_a.y;
		line_ac.z = center.z - point_a.z;

		l_scale = nnDotProductVector(&line_ab, &line_ac) / nnDotProductVector(&line_ab, &line_ab);

		point_h.x = point_a.x + line_ab.x * l_scale;
		point_h.y = point_a.y + line_ab.y * l_scale;
		point_h.z = point_a.z + line_ab.z * l_scale;

		base_pos_x = FXM_FLOAT_TO_FX32(point_h.x);
		base_pos_y = FXM_FLOAT_TO_FX32(-point_h.y);
	}
	else {
		switch ((now_dir & 0xC000) >> 14) {
		case 0:	// 下
			base_pos_x = 0;
			base_pos_y = obj_work->field_rect[OBD_BOTTOM] << FX32_SHIFT;
			break;
		case 1:	// 左
			base_pos_x = -obj_work->field_rect[OBD_BOTTOM] << FX32_SHIFT;
			base_pos_y = 0;
			break;
		case 2:	// 上
			base_pos_x = 0;
			base_pos_y = -obj_work->field_rect[OBD_BOTTOM] << FX32_SHIFT;
			break;
		case 3:	// 右
			base_pos_x = obj_work->field_rect[OBD_BOTTOM] << FX32_SHIFT;
			base_pos_y = 0;
			break;
		}
	}

	// 車輪の位置をチェックする
	// 前輪
	ofst_x = FXM_FLOAT_TO_FX32(obj_work->field_rect[OBD_RIGHT] * nnCos(now_dir));
	ofst_y = FXM_FLOAT_TO_FX32(obj_work->field_rect[OBD_RIGHT] * nnSin(now_dir));
	col_data.pos_x	= (base_pos_x + ofst_x + obj_work->pos.x) >> FX32_SHIFT;
	col_data.pos_y	= (base_pos_y + ofst_y + obj_work->pos.y) >> FX32_SHIFT;


	diff_1 = ObjDiffCollision(&col_data);
	if (diff_1 <= 2) {
		ret_val |= 0x01;
	}

	// 後輪
	ofst_x = FXM_FLOAT_TO_FX32(obj_work->field_rect[OBD_LEFT] * nnCos(now_dir));
	ofst_y = FXM_FLOAT_TO_FX32(obj_work->field_rect[OBD_LEFT] * nnSin(now_dir));
	col_data.pos_x	= (base_pos_x + ofst_x + obj_work->pos.x) >> FX32_SHIFT;
	col_data.pos_y	= (base_pos_y + ofst_y + obj_work->pos.y) >> FX32_SHIFT;


	diff_2 = ObjDiffCollision(&col_data);
	if (diff_2 <= 2) {
		ret_val |= 0x02;
	}

	return (ret_val);
}

// ==========================================================================
// gmPlySeqGmkMainTruckDanger
/*!
 *	トロッコ危険状態
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		gmk_work0	角度速度
 *		gmk_work1	現在の角度
 *		gmk_work2	カウンタ
 *		gmk_work3	トロッコ角度
 *		player_flag : GMD_PLF_USER1	トロッコ復旧開始告知
 *		player_flag : GMD_PLF_USER2	トロッコ傾き演出終了
 *		player_flag : GMD_PLF_USER3	トロッコ右壁接着(前輪浮き)
 *		user_timer	: がたん演出速度
 *		user_work	: がたん回数カウンタ
 */
// ==========================================================================
void gmPlySeqGmkMainTruckDanger(GMS_PLAYER_WORK *ply_work)
{
	NNS_MATRIX	mat;
	float		truck_c_x, truck_c_y, truck_c_z;

	if (!(ply_work->player_flag & GMD_PLF_USER2)) {
		// トロッコ がたん演出
#if 0
		if (MTM_MATH_ABS(ply_work->gmk_work3) < GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_DIR) {
			if (!(ply_work->player_flag & GMD_PLF_USER3)) {
				// 後輪浮き
				ply_work->gmk_work3 += 0x400;
			}
			else {
				// 前輪浮き
				ply_work->gmk_work3 -= 0x400;
			}
		}
		else {
			// アクション設定
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_DANGER);
			// がたん演出終了
			ply_work->player_flag |= GMD_PLF_USER2;
		}
#else
		if (MTM_MATH_ABS(ply_work->gmk_work3) < GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_DIR) {
			ply_work->gmk_work3 += ply_work->obj_work.user_timer;

			if (!(ply_work->player_flag & GMD_PLF_USER3)) {
				// 後輪浮き
				if (ply_work->gmk_work3 > GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_DIR) {
					ply_work->gmk_work3 = GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_DIR;
				}
				ply_work->obj_work.user_timer = ObjSpdUpSet(ply_work->obj_work.user_timer, GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_SPD, 0x400);
			}
			else {
				// 前輪浮き
				if (ply_work->gmk_work3 < -GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_DIR) {
					ply_work->gmk_work3 = -GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_DIR;
				}
				ply_work->obj_work.user_timer = ObjSpdUpSet(ply_work->obj_work.user_timer, -GMD_PLF_SEQ_GMK_TRUCK_DANGER_SLANT_SPD, 0x400);
			}
		}
		else {
			// 一度目のがたんでアクションを切り替える
			if (ply_work->act_state != GME_PLY_ACT_STATE_GMK_TRUCK_DANGER &&
					ply_work->act_state != GME_PLY_ACT_STATE_GMK_TRUCK_DANGER_ST) {
				// アクション設定
				GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_DANGER);
			}

			// 一定数跳ね返ったあとがたん演出終了
			if (ply_work->obj_work.user_work < 3) {
				ply_work->obj_work.user_work++;
				ply_work->obj_work.user_timer = -ply_work->obj_work.user_timer >> 1;
				if (ply_work->gmk_work3 < 0) {
					ply_work->gmk_work3 += 1;
				}
				else {
					ply_work->gmk_work3 -= 1;
				}
			}
			else {
				// がたん演出終了
				ply_work->player_flag |= GMD_PLF_USER2;
			}
		}
#endif
	}

	if (ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_DANGER) {
		// 角度変更時間設定
		ply_work->gmk_work2 = ObjTimeCountDown(ply_work->gmk_work2);
		// 現在の角度設定
		if (ply_work->gmk_work2) {
			ply_work->gmk_work1 = (u16)(ply_work->gmk_work1 + ply_work->gmk_work0);
		}
		if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
			// アクション設定
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_DANGER_ST);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;

			// 危険状態告知
			ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_DANGER;

			// ブルブル
#if GMD_PLY_SEQ_GMK_TRUCK_DANGER_VIB_PLAYER
			ply_work->obj_work.vib_timer = ply_work->fall_timer;
#else
#if GMD_PLY_SEQ_GMK_TRUCK_DANGER_VIB_TRUCK
			ply_work->truck_obj->vib_timer = ply_work->fall_timer;
#endif
#endif
		}
	}
	else if (ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_DANGER) {
		// 現在の角度によって調整
		//ply_work->gmk_work1 = (u16)(ply_work->obj_work.dir.z + ply_work->obj_work.dir_fall + 0x8000/*裏返る分*/);
		ply_work->gmk_work1 = (u16)(0x8000/*裏返る分*/ - ply_work->obj_work.dir.z - ply_work->gmk_work3
										+ (s16)(g_gm_main_system.pseudofall_dir - ply_work->obj_work.dir_fall));

		// 踏ん張り終了チェック
		if (ply_work->player_flag & GMD_PLF_USER1) {
			// 踏ん張り終了

			// 危険回避へ
			GmPlySeqGmkInitTruckDangerRet(ply_work, ply_work->truck_obj);
		}
	}

	// がたん演出 アクション切り替え終了でぶら下がり終了告知
	if (!(ply_work->gmk_flag & GMD_PLGF_GMK_TRUCK_DANGER) &&
			ply_work->act_state == GME_PLY_ACT_STATE_GMK_TRUCK_DANGER_ST &&
			ply_work->player_flag & GMD_PLF_USER2) {
		// 危険状態告知
		ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_DANGER;
	}

	// 演出用MTX反映
	nnMakeUnitMatrix(&ply_work->ex_obj_mtx_r);
	nnTranslateMatrix(&ply_work->ex_obj_mtx_r, 
				&ply_work->ex_obj_mtx_r,
				-GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_X,
				-GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_Y,
				-GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_Z); 
				//-19.2f/GMD_OBJ_DRAW_SCALE, -25.6f/GMD_OBJ_DRAW_SCALE, 4.0f/GMD_OBJ_DRAW_SCALE); 
	nnRotateXMatrix(&ply_work->ex_obj_mtx_r, &ply_work->ex_obj_mtx_r, ply_work->gmk_work1);
	nnTranslateMatrix(&ply_work->ex_obj_mtx_r, 
				&ply_work->ex_obj_mtx_r,
				GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_X,
				GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_Y,
				GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_Z); 
				//19.2f/GMD_OBJ_DRAW_SCALE, 25.6f/GMD_OBJ_DRAW_SCALE, -4.0f/GMD_OBJ_DRAW_SCALE);
	
	// トロッコがたん演出
#if 1
	if (!(ply_work->player_flag & GMD_PLF_USER3)) {
		// 後輪浮き
		truck_c_x = GMD_GMK_TRUCK_FW_CENTER_X;
		truck_c_y = GMD_GMK_TRUCK_FW_CENTER_Y;
		truck_c_z = GMD_GMK_TRUCK_FW_CENTER_Z;
	}
	else {
		// 前輪浮き
		truck_c_x = GMD_GMK_TRUCK_BW_CENTER_X;
		truck_c_y = GMD_GMK_TRUCK_BW_CENTER_Y;
		truck_c_z = GMD_GMK_TRUCK_BW_CENTER_Z;
	}
	nnMakeUnitMatrix(&mat);
	nnTranslateMatrix(&mat, &mat, 
				-truck_c_x, -truck_c_y, -truck_c_z); 
	nnRotateXMatrix(&mat, &mat, ply_work->gmk_work3);
	nnRotateYMatrix(&mat, &mat, MTM_MATH_ABS(ply_work->gmk_work3) >> 2);
	nnRotateZMatrix(&mat, &mat, MTM_MATH_ABS(ply_work->gmk_work3) >> 2);
	nnTranslateMatrix(&mat, &mat, 
				truck_c_x, truck_c_y, truck_c_z); 
	nnMultiplyMatrix(&ply_work->ex_obj_mtx_r, &mat, &ply_work->ex_obj_mtx_r); 
#else
	nnMakeUnitMatrix(&mat);
	nnTranslateMatrix(&mat, &mat, 
				-GMD_GMK_TRUCK_FW_CENTER_X, -GMD_GMK_TRUCK_FW_CENTER_Y, -GMD_GMK_TRUCK_FW_CENTER_Z); 
	nnRotateXMatrix(&mat, &mat, ply_work->gmk_work3);
//	nnRotateYMatrix(&mat, &mat, test_rot_y);
//	nnRotateZMatrix(&mat, &mat, test_rot_z);
	nnTranslateMatrix(&mat, &mat, 
				GMD_GMK_TRUCK_FW_CENTER_X, GMD_GMK_TRUCK_FW_CENTER_Y, GMD_GMK_TRUCK_FW_CENTER_Z); 
	nnMultiplyMatrix(&ply_work->ex_obj_mtx_r, &mat, &ply_work->ex_obj_mtx_r); 
#endif
}

// ==========================================================================
// トロッコ GME_PLY_SEQ_STATE_GMK_TRUCK_DANGER_RET
// ==========================================================================
// ==========================================================================
// gmPlySeqGmkMainTruckDangerRet
/*!
 *	トロッコ危険復帰
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@note
 *		gmk_work0	角度速度
 *		gmk_work1	現在の角度
 *		gmk_work2	カウンタ
 *		gmk_work3	トロッコ角度
 *		player_flag : GMD_PLF_USER1	トロッコ復旧開始告知
 *		player_flag : GMD_PLF_USER2	トロッコ傾き演出開始
 *		player_flag : GMD_PLF_USER3	トロッコ右壁接着(前輪浮き)
 */
// ==========================================================================
void gmPlySeqGmkMainTruckDangerRet(GMS_PLAYER_WORK *ply_work)
{
	NNS_MATRIX	mat;
	float		truck_c_x, truck_c_y, truck_c_z;

	ply_work->gmk_work2 = ObjTimeCountUp(ply_work->gmk_work2);

	if (GMD_PLY_SEQ_GMK_TRUCK_DANGER_RET_DIR_START_FRAME*FX32_ONE <= ply_work->gmk_work2 &&
			 ply_work->gmk_work2 <=
				(GMD_PLY_SEQ_GMK_TRUCK_DANGER_RET_DIR_START_FRAME+GMD_PLY_SEQ_GMK_TRUCK_DANGER_RET_DIR_FRAME)*FX32_ONE) {
		// 回転
#if 1
		ply_work->gmk_work1 = ply_work->gmk_work1 + ply_work->gmk_work0;

		if (ply_work->gmk_work0 < 0) {
			if (ply_work->gmk_work1 < 0) {
				ply_work->gmk_work1 = 0;
			}
		}
		else {
			if (ply_work->gmk_work1 > 0) {
				ply_work->gmk_work1 = 0;
			}
		}
#else
		ply_work->gmk_work1 = (u16)(ply_work->gmk_work1 + ply_work->gmk_work0);
		if (ply_work->gmk_work1 < 0) {
			ply_work->gmk_work1 = 0;
		}
#endif
	}
	if (ply_work->player_flag & GMD_PLF_USER2) {
		// トロッコ がたん演出戻し
#if 1
		ply_work->gmk_work3 = ObjSpdDownSet(ply_work->gmk_work3, 0x400);
		if (MTM_MATH_ABS(ply_work->gmk_work3) == 0) {
#else
		ply_work->gmk_work3 -= 0x400;
		if (ply_work->gmk_work3 <= 0) {
#endif
			ply_work->gmk_work3 = 0;

			// 復帰終了
			ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_DANGER_RET;	// プレイヤー標準処理が復帰するように促す

			// くらい復帰
			//GmPlayerSetDefNormal(ply_work);
			// プレイヤーの踏ん張り復帰で戻す

			// FWへ
			GmPlySeqChangeFw(ply_work);
			return;
		}
	}
	else if (ply_work->obj_work.disp_flag & OBD_DISP_END) {
		ply_work->gmk_work1 = 0;
		if (ply_work->gmk_work3) {
			// トロッコ角度がある場合は戻す
			ply_work->player_flag |= GMD_PLF_USER2;

			// アクションを先にFWへ
			ply_work->gmk_flag &= ~GMD_PLGF_GMK_TRUCK_L;
			GmPlayerActionChange(ply_work, GME_PLY_ACT_STATE_GMK_TRUCK_FW);
			ply_work->obj_work.disp_flag |= OBD_DISP_REPEAT;
		}
		else {
			// 復帰終了
			ply_work->gmk_flag |= GMD_PLGF_GMK_TRUCK_DANGER_RET;	// プレイヤー標準処理が復帰するように促す

			// くらい復帰
			//GmPlayerSetDefNormal(ply_work);
			// プレイヤーの踏ん張り復帰で戻す

			// FWへ
			GmPlySeqChangeFw(ply_work);
			return;
		}
	}

	// 演出用MTX反映
	nnMakeUnitMatrix(&ply_work->ex_obj_mtx_r);
	nnTranslateMatrix(&ply_work->ex_obj_mtx_r, 
				&ply_work->ex_obj_mtx_r, 
				-GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_X,
				-GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_Y,
				-GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_Z); 
				//-19.2f/GMD_OBJ_DRAW_SCALE, -25.6f/GMD_OBJ_DRAW_SCALE, 4.0f/GMD_OBJ_DRAW_SCALE); 
	nnRotateXMatrix(&ply_work->ex_obj_mtx_r, &ply_work->ex_obj_mtx_r, (u16)ply_work->gmk_work1);
	nnTranslateMatrix(&ply_work->ex_obj_mtx_r, 
				&ply_work->ex_obj_mtx_r, 
				GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_X,
				GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_Y,
				GMD_PLF_SEQ_GMK_TRUCK_DANGER_ROT_CENTER_Z); 
				//19.2f/GMD_OBJ_DRAW_SCALE, 25.6f/GMD_OBJ_DRAW_SCALE, -4.0f/GMD_OBJ_DRAW_SCALE); 
	
	// トロッコがたん演出
#if 1
	if (!(ply_work->player_flag & GMD_PLF_USER3)) {
		// 後輪浮き
		truck_c_x = GMD_GMK_TRUCK_FW_CENTER_X;
		truck_c_y = GMD_GMK_TRUCK_FW_CENTER_Y;
		truck_c_z = GMD_GMK_TRUCK_FW_CENTER_Z;
	}
	else {
		// 前輪浮き
		truck_c_x = GMD_GMK_TRUCK_BW_CENTER_X;
		truck_c_y = GMD_GMK_TRUCK_BW_CENTER_Y;
		truck_c_z = GMD_GMK_TRUCK_BW_CENTER_Z;
	}
	nnMakeUnitMatrix(&mat);
	nnTranslateMatrix(&mat, &mat, -truck_c_x, -truck_c_y, -truck_c_z); 
	nnRotateXMatrix(&mat, &mat, ply_work->gmk_work3);
//	nnRotateYMatrix(&mat, &mat, test_rot_y);
	nnRotateZMatrix(&mat, &mat, MTM_MATH_ABS(ply_work->gmk_work3) >> 2);
	nnTranslateMatrix(&mat, &mat, truck_c_x, truck_c_y, truck_c_z); 
	nnMultiplyMatrix(&ply_work->ex_obj_mtx_r, &mat, &ply_work->ex_obj_mtx_r);
#else
	nnMakeUnitMatrix(&mat);
	nnTranslateMatrix(&mat, &mat, 
				-GMD_GMK_TRUCK_FW_CENTER_X, -GMD_GMK_TRUCK_FW_CENTER_Y, -GMD_GMK_TRUCK_FW_CENTER_Z); 
	nnRotateXMatrix(&mat, &mat, ply_work->gmk_work3);
//	nnRotateYMatrix(&mat, &mat, test_rot_y);
//	nnRotateZMatrix(&mat, &mat, test_rot_z);
	nnTranslateMatrix(&mat, &mat, 
				GMD_GMK_TRUCK_FW_CENTER_X, GMD_GMK_TRUCK_FW_CENTER_Y, GMD_GMK_TRUCK_FW_CENTER_Z); 
	nnMultiplyMatrix(&ply_work->ex_obj_mtx_r, &mat, &ply_work->ex_obj_mtx_r);
#endif
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
