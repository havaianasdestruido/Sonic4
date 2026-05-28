// ==========================================================================
/*!
  @file gmCamera.cpp
  @brief 

  @author 
				Copyright(c) 2009 Dimps

  $Id: gmCamera.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "objCamera.h"
#include "gmCamera.h"

#include "gmPlayer.h"
#include "gmPlayerDat.h"
#include "gmPlySpec.h"
#include "gmMapFar.h"
#include "gmMap.h"
#include "gmMain.h"
#include "gmGameDat.h"
//----- Definitions ---------------------------------------------------------
#define GMD_CAM_FLAG_LOOKUP		0x00000001	// 見上げ
#define GMD_CAM_FLAG_LOOKDOWN	0x00000002	// 見下ろし
#define GMD_CAM_LOOKUP_TIME		(90)		// 見上げ開始までの時間
#define GMD_CAM_LOOKUP_SCR_SPD	(0x0000002)	// 見上げスクロール移動量
#define GMD_CAM_LOOKUP_SCR_LMT_UP (0x0000050)	// 見上げスクロール制限値
#define GMD_CAM_LOOKUP_SCR_LMT_DW (-0x0000060)	// 見上げスクロール制限値

#if _IPHONE
#define GMD_CAMERA_TRUCK_ROLL_SPEED_ACCEL    (0x0d00) // カメラ回転速度 加速力アップ
#define GMD_CAMERA_TRUCK_ROLL_SPEED_MAX      (0x1200) // カメラ回転速度 MAX
#define GMD_CAMERA_TRUCK_ROLL_SPEED_SLOW_MAX (0x02a0) // カメラ回転速度 SLOW MAX
#else
#define GMD_CAMERA_TRUCK_ROLL_SPEED_MAX      (0x1000) // カメラ回転速度 MAX
#define GMD_CAMERA_TRUCK_ROLL_SPEED_SLOW_MAX (0x0300) // カメラ回転速度 SLOW MAX
#endif // _IPHONE

typedef struct tag_GMS_CAMERA_WORK {
	// 見上げ/見下ろし管理
	s32 	flag;			///< 状態フラグ
	s32 	timer;			///< キー入力継続時間
	s32 	offset;			///< カメラオフセット量
	// 描画スケール管理
	float	scale_now;		///< 表示スケール
	float	scale_target;	///< 目標スケール
	float	scale_spd;		///< スケール変更速度
} GMS_CAMERA_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
#if GMD_MAP_FAR_TEST
static void gmCameraFuncMapFar(OBS_CAMERA *obj_camera);
#endif //GMD_MAP_FAR_TEST
static void gmCameraFuncAddMap(OBS_CAMERA *obj_camera);
static Float gmCameraVibCheck(Float vib);
static void gmCameraFuncWater(OBS_CAMERA *obj_camera);
static void gmCameraLookupCheck(OBS_CAMERA *obj_camera);
static void gmCameraScaleChange(OBS_CAMERA *obj_camera);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
// 画面振動
static NNS_VECTOR gm_camera_vibration;
static GMS_CAMERA_WORK gm_camera_work;

static 		 NNS_VECTOR	gm_camera_option_allow_pos;
static const NNS_VECTOR gm_camera_common_allow_pos = {15.f, 50.f, 0.f};
static const NNS_VECTOR	gm_camera_splstg_allow_pos = {0.f, 0.f, 0.f};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmCameraInit
/*!
 *	カメラ初期化
 */
// ==========================================================================
void GmCameraInit(void)
{
	OBS_CAMERA	*camera;
	// 仮
	NNS_VECTOR	cam_pos = {177.f, -1580.f, 50.f};

	// カメラ初期化
	ObjCameraInit(GME_CAMERA_NO_MAIN, &cam_pos, GMD_TASK_GROUP_OBJSYS, GMD_TASK_PAUSE_LEVEL_CAMERA, GMD_TASK_PRIO_CAMERA);
	ObjCamera3dInit(GME_CAMERA_NO_MAIN);
	g_obj.glb_camera_id = GME_CAMERA_NO_MAIN;
	g_obj.glb_camera_type = NNE_PROJECTION_TYPE_ORTHO;	// 正射影
	GmCameraDelayReset();
	GmCameraAllowReset();
	ObjCameraSetUserFunc(GME_CAMERA_NO_MAIN, GmCameraFunc);	// ユーザー処理

	camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	//camera->scale = 0.078125f * GMD_OBJ_DRAW_SCALE * 2.4f;
	camera->scale = GMD_CAMERA_SCALE;
	camera->ofst.z = 1000.0f;
	//camera->scale = 2.0f;	// ◆

	// 振動パラメータ
	gm_camera_vibration.x = 0;
	gm_camera_vibration.y = 0;
	gm_camera_vibration.z = 0;

	// 個別ワーク
	gm_camera_work.flag = 0;
	gm_camera_work.timer = 0;
	gm_camera_work.offset = 0;
	gm_camera_work.scale_now	= 1.0f;
	gm_camera_work.scale_target	= 1.0f;
	gm_camera_work.scale_spd	= 0.1f;
	
#if GMD_MAP_FAR_TEST
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	if ( zone_type == GSD_MAIN_ZONE_TYPE_1
		    || zone_type == GSD_MAIN_ZONE_TYPE_2
			|| zone_type == GSD_MAIN_ZONE_TYPE_3
			|| zone_type == GSD_MAIN_ZONE_TYPE_FINAL
			|| zone_type == GSD_MAIN_ZONE_TYPE_SS
	){
		//遠景（仮設定）
/*
#ifdef _MG_IPAD
		NNS_VECTOR	cam_pos_far = {0.0f, 0.0f, 30.0f};		
#else*/
		NNS_VECTOR	cam_pos_far = {0.0f, 0.0f, 60.0f};		
/*		
#endif
 */
		ObjCameraInit( GME_CAMERA_NO_FAR, &cam_pos_far, GMD_TASK_GROUP_OBJSYS, GMD_TASK_PAUSE_LEVEL_CAMERA, GMD_TASK_PRIO_CAMERA );
		ObjCamera3dInit( GME_CAMERA_NO_FAR );
		ObjCameraSetUserFunc( GME_CAMERA_NO_FAR, gmCameraFuncMapFar );	// ユーザー処理
		camera = ObjCameraGet(GME_CAMERA_NO_FAR);
		camera->command_state = OBD_DRAW_CMD_STATE_PRE_MAPFAR;

		if ( GSD_MAIN_ZONE_TYPE_3 == zone_type )
		{
			//camera->fovy = NNM_DEGtoA32(25.0f);
			camera->fovy = NNM_DEGtoA32(40.0f);
		}
		else
		{
			camera->fovy = NNM_DEGtoA32(40.0f);
		}

		camera->znear = 0.1f;
		camera->zfar = 32768.0f;
	}
	
#endif	//GMD_MAP_FAR_TEST

	// 超近景・スクロールタイプ中景用カメラ
#if 1
	{
		s32			i;
		OBS_CAMERA	*camera;
		NNS_VECTOR	cam_pos = {177.f, -1580.f, 50.f};
		//NNS_VECTOR	allow_pos_snear = {15.f, 50.f, 0.f};
		//NNS_VECTOR	play_pos_snear = {50.f, 50.f, 0.f};
		s32	camera_id_tbl[GMD_CAMERA_ADDMAP_NUM] = {GME_CAMERA_NO_SNEAR, GME_CAMERA_NO_MID1,
													GME_CAMERA_NO_MID2, GME_CAMERA_NO_MID3};
		s32 check_data_no[GMD_CAMERA_ADDMAP_NUM] = {GMD_GAMEDAT_MAPSET_ADD_LOCAL_N_MP, GMD_GAMEDAT_MAPSET_ADD_LOCAL_M1_MP,
													GMD_GAMEDAT_MAPSET_ADD_LOCAL_M2_MP, GMD_GAMEDAT_MAPSET_ADD_LOCAL_M3_MP};
		u32 command_state_tbl[GMD_CAMERA_ADDMAP_NUM] = {OBD_DRAW_CMD_STATE_NEAR_MAP, OBD_DRAW_CMD_STATE_3DNN,
														OBD_DRAW_CMD_STATE_3DNN, OBD_DRAW_CMD_STATE_3DNN};
														// コマンドステートは カメラ設定時に指定するので標準設定でよい

		for (i = 0; i < GMD_CAMERA_ADDMAP_NUM; i++) {
			if (g_gm_gamedat_map_set_add[check_data_no[i]]) {
				// データがセットされている時はカメラを初期化

				// カメラ初期化
				ObjCameraInit(camera_id_tbl[i], &cam_pos, GMD_TASK_GROUP_OBJSYS, GMD_TASK_PAUSE_LEVEL_CAMERA, GMD_TASK_PRIO_CAMERA);
				ObjCamera3dInit(camera_id_tbl[i]);
				//ObjCameraAllowSet(0, &allow_pos_snear);			// スクロール開始遊び
				//ObjCameraPlaySet(0, &play_pos_snear);			// スクロール遅延遊び
				ObjCameraSetUserFunc(camera_id_tbl[i], gmCameraFuncAddMap);	// ユーザー処理
			//	ObjCameraLimitSet(0, );
			//	ObjCameraDispOffsetSet(0, &ofs_pos);
			//	ObjCameraTargetObjSet(0, &g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work);

				camera = ObjCameraGet(camera_id_tbl[i]);
				camera->scale = GMD_CAMERA_SCALE;
				camera->ofst.z = 1000.0f;

				camera->command_state = command_state_tbl[i];
			}
		}
	}
#else
	{
		OBS_CAMERA	*camera;
		NNS_VECTOR	cam_pos_snear = {177.f, -1580.f, 50.f};
		//NNS_VECTOR	allow_pos_snear = {15.f, 50.f, 0.f};
		//NNS_VECTOR	play_pos_snear = {50.f, 50.f, 0.f};

		// カメラ初期化
		ObjCameraInit(GME_CAMERA_NO_SNEAR, &cam_pos_snear, GMD_TASK_GROUP_OBJSYS, GMD_TASK_PAUSE_LEVEL_CAMERA, GMD_TASK_PRIO_CAMERA);
		ObjCamera3dInit(GME_CAMERA_NO_SNEAR);
		//ObjCameraAllowSet(0, &allow_pos_snear);			// スクロール開始遊び
		//ObjCameraPlaySet(0, &play_pos_snear);			// スクロール遅延遊び
		ObjCameraSetUserFunc(GME_CAMERA_NO_SNEAR, gmCameraFuncAddMap);	// ユーザー処理
	//	ObjCameraLimitSet(0, );
	//	ObjCameraDispOffsetSet(0, &ofs_pos);
	//	ObjCameraTargetObjSet(0, &g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work);

		camera = ObjCameraGet(GME_CAMERA_NO_SNEAR);
		camera->scale = GMD_CAMERA_SCALE;
		camera->ofst.z = 1000.0f;
	}
#endif


	//水面カメラ
	NNS_VECTOR	cam_pos_water = {cam_pos.x, cam_pos.y, cam_pos.z};	
	ObjCameraInit(
		GME_CAMERA_NO_WATER,
		&cam_pos_water, 
		GMD_TASK_GROUP_OBJSYS, 
		GMD_TASK_PAUSE_LEVEL_CAMERA, 
		GMD_TASK_PRIO_CAMERA);
	ObjCamera3dInit(GME_CAMERA_NO_WATER);
	ObjCameraSetUserFunc( GME_CAMERA_NO_WATER, gmCameraFuncWater );	// ユーザー処理
	camera = ObjCameraGet(GME_CAMERA_NO_WATER);
	camera->command_state = OBD_DRAW_CMD_STATE_PRE_WATER;
	camera->scale = GMD_CAMERA_SCALE;
	camera->ofst.z = 1000.0f;

}

// ==========================================================================
// GmCameraExit
/*!
 *	カメラ終了
 */
// ==========================================================================
void GmCameraExit(void)
{
}
// ==========================================================================
// GmCameraPosSet
/*!
 *	カメラ指定座標セット
 *
 *	@param	pos_x		[in]	指定Ｘ座標
 *	@param	pos_y		[in]	指定Ｙ座標
 *	@param	pos_z		[in]	指定Ｚ座標
 */
// ==========================================================================
void GmCameraPosSet(fx32 pos_x, fx32 pos_y, fx32 pos_z)
{
	OBS_CAMERA	*obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);

#if 1
	float	lcd_width = (float)(GSD_DISP_WIDTH/2) * obj_camera->scale;
	float	lcd_height= (float)(GSD_DISP_HEIGHT/2) * obj_camera->scale;
	float	bottom	= g_gm_main_system.map_fcol.bottom - (float)(GSD_DISP_HEIGHT/2) * obj_camera->scale;
#else
	float	lcd_width = (float)(AMD_SCREEN_2D_WIDTH/2) * obj_camera->scale;
	float	lcd_height= (float)(AMD_SCREEN_2D_HEIGHT/2) * obj_camera->scale;
	float	bottom	= g_gm_main_system.map_fcol.bottom - (float)(AMD_SCREEN_2D_HEIGHT/2) * obj_camera->scale;
#endif

    obj_camera->pos.x = FXM_FX32_TO_FLOAT(pos_x);
	if (obj_camera->pos.x < lcd_width) {
	    obj_camera->pos.x = lcd_width;
	}
    obj_camera->pos.y = -FXM_FX32_TO_FLOAT(pos_y);
	if (obj_camera->pos.y > -lcd_height) {
	    obj_camera->pos.y = -lcd_height;
	}
	if (obj_camera->pos.y > bottom) {
	    obj_camera->pos.y = bottom;
	}

    obj_camera->pos.z = FXM_FX32_TO_FLOAT(pos_z) + 50.0f;

	obj_camera->disp_pos.x = obj_camera->pos.x;// - lcd_width;
	obj_camera->disp_pos.y = obj_camera->pos.y;// - lcd_height;
	obj_camera->disp_pos.z = obj_camera->pos.z;
}
// ==========================================================================
// GmCameraVibrationSet
/*!
 *	カメラ画面振動セット
 *
 *	@param	obj_camera	[in]	オブジェクトカメラ
 */
// ==========================================================================
void GmCameraVibrationSet(fx32 vib_x, fx32 vib_y, fx32 vib_z)
{
	gm_camera_vibration.x = FXM_FX32_TO_FLOAT(vib_x);
	gm_camera_vibration.y = FXM_FX32_TO_FLOAT(vib_y);
	gm_camera_vibration.z = FXM_FX32_TO_FLOAT(vib_z);
}

// ==========================================================================
// GmCameraAllowSet
/*!
 *	スクロール開始遊びセット
 *
 *	@param	alw_x		[in]	開始遊び X
 *	@param	alw_y		[in]	開始遊び Y
 *	@param	alw_z		[in]	開始遊び Z
 */
// ==========================================================================
void GmCameraAllowSet(Float alw_x, Float alw_y, Float alw_z)
{
	gm_camera_option_allow_pos.x = alw_x;
	gm_camera_option_allow_pos.y = alw_y;
	gm_camera_option_allow_pos.z = alw_z;
	
	ObjCameraAllowSet(GME_CAMERA_NO_MAIN, &gm_camera_option_allow_pos);		// スクロール開始遊び
}

// ==========================================================================
// GmCameraAllowReset
/*!
 *	スクロール開始遊びリセット
 */
// ==========================================================================
void GmCameraAllowReset(void)
{
	if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// 通常ステージ
		ObjCameraAllowSet(GME_CAMERA_NO_MAIN, &gm_camera_common_allow_pos);	// スクロール開始遊び
	} else {
		// スペシャルステージ
		ObjCameraAllowSet(GME_CAMERA_NO_MAIN, &gm_camera_splstg_allow_pos);	// スクロール開始遊び
	}
}

// ==========================================================================
// GmCameraDelaySet
/*!
 *	スクロール遅延遊びセット
 *
 *	@param	dly_x		[in]	遅延遊び X
 *	@param	dly_y		[in]	遅延遊び Y
 *	@param	dly_z		[in]	遅延遊び Z
 */
// ==========================================================================
void GmCameraDelaySet(Float dly_x, Float dly_y, Float dly_z)
{
	NNS_VECTOR	play_pos;

	play_pos.x = dly_x;
	play_pos.y = dly_y;
	play_pos.z = dly_z;
	
	ObjCameraPlaySet(GME_CAMERA_NO_MAIN, &play_pos);						// スクロール遅延遊び
}

// ==========================================================================
// GmCameraDelayReset
/*!
 *	スクロール遅延遊びリセット
 */
// ==========================================================================
void GmCameraDelayReset(void)
{
	NNS_VECTOR	play_pos = {50.f, 50.f, 0.f};
	NNS_VECTOR	spl_pos = {0.f, 0.f, 0.f};

	if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// 通常ステージ
		ObjCameraPlaySet(GME_CAMERA_NO_MAIN, &play_pos);					// スクロール遅延遊び
	} else {
		// スペシャルステージ
		ObjCameraPlaySet(GME_CAMERA_NO_MAIN, &spl_pos);						// スクロール遅延遊び
	}
}


// ==========================================================================
// GmCameraScaleSet
/*!
 *	表示スケールの変更（拡縮）
 *
 *	@param	scale_target	[in]	目標スケール(1.0f で基準サイズに戻る)
 *	@param	scale_spd		[in]	スケール変更速度
 *
 *	@note
 *		簡易処理のため大きな縮小率をセットするとマップ描画範囲や
 *		イベント生成範囲にて不具合が生じるため、設定数値に注意
 */
// ==========================================================================
void GmCameraScaleSet(float scale_target, float scale_spd)
{
	gm_camera_work.scale_target	= scale_target;
	gm_camera_work.scale_spd	= scale_spd;
}

// ==========================================================================
// GmCameraSetClipCamera
/*!
 *	クリッピング, イベント生成範囲基点 を設定
 *
 *	@param	obj_camera	[in]	対象カメラ
 *
 *	@note
 *		disp_pos の設定が済んだ後に呼び出してください。\n
 *		基本的に、ObjObjectCameraSet とセットで呼び出します
 */
// ==========================================================================
void GmCameraSetClipCamera(OBS_CAMERA *obj_camera)
{
#if _IPHONE
	UNREFERENCED_PARAMETER(obj_camera);
	ObjObjectClipCameraSet(g_obj.camera[0][MTD_X], g_obj.camera[0][MTD_Y]);
#else
	float	left	= (float)(g_gm_main_system.map_fcol.left   + GMD_OBJ_CLIP_LCD_X/2);
	float	right	= (float)(g_gm_main_system.map_fcol.right  - GMD_OBJ_CLIP_LCD_X/2);
	float	top		= (float)(g_gm_main_system.map_fcol.top    + GMD_OBJ_CLIP_LCD_Y/2);
	float	bottom	= (float)(g_gm_main_system.map_fcol.bottom - GMD_OBJ_CLIP_LCD_Y/2);

	float		camera_x, camera_y;

	if (obj_camera->disp_pos.x <= left) {
		camera_x = left;
	}
	else if (obj_camera->disp_pos.x >= right) {
		camera_x = right;
	}
	else {
		camera_x = obj_camera->disp_pos.x;
	}
	if (-obj_camera->disp_pos.y <= top) {
		camera_y = top;
	}
	else if (-obj_camera->disp_pos.y >= bottom) {
		camera_y = bottom;
	}
	else {
		camera_y = -obj_camera->disp_pos.y;
	}


	ObjObjectClipCameraSet(FXM_FLOAT_TO_FX32(camera_x - (float)(OBD_OBJ_CLIP_LCD_X/2)),
						FXM_FLOAT_TO_FX32(camera_y - (float)(OBD_OBJ_CLIP_LCD_Y/2)));
#endif
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// GmCameraFunc
/*!
 *	メインカメラ
 *	@param	obj_camera	[in]	オブジェクトカメラ
 */
// ==========================================================================
void GmCameraFunc(OBS_CAMERA *obj_camera)
{
    NNS_VECTOR pos;
	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	pos.x = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.x);
    pos.z = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.z);
	if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// 通常はカメラＹを少しオフセット
	    pos.y = FXM_FX32_TO_FLOAT(-ply_work->obj_work.pos.y + GMD_SCR_PLY_Y_OFFS);
	} else{
		// スペステはオフセット量を少し抑える（スペステ終了時画面回転時の中心ずれ違和感を抑えるため）
	    pos.y = FXM_FX32_TO_FLOAT(-ply_work->obj_work.pos.y + GMD_SCR_PLY_Y_OFFS/3);
	}

    // 目的値を記録
	if ((ply_work->player_flag & GMD_PLF_DIE) &&
				!(ply_work->player_flag & GMD_PLF_TATK_RETRY)) {
		// プレイヤー死亡時はカメラ不動(リトライ演出中を除く)
	    pos.x = obj_camera->work.x;
	    pos.y = obj_camera->work.y;
	    pos.z = obj_camera->work.z;
	} else {
		// 通常時は更新
	    obj_camera->work.x = pos.x;
	    obj_camera->work.y = pos.y;
	    obj_camera->work.z = pos.z;
    }

    obj_camera->prev_pos.x = obj_camera->pos.x;
    obj_camera->prev_pos.y = obj_camera->pos.y;
    obj_camera->prev_pos.z = obj_camera->pos.z;

    if ( obj_camera->flag & OBD_CAMERA_FIX ){
        if ( obj_camera->target_obj ){
            obj_camera->pos.x = pos.x;
            obj_camera->pos.y = pos.y;
            obj_camera->pos.z = pos.z;
        }

        return;
    }

	// スケール変更チェック＆実行
	gmCameraScaleChange(obj_camera);


    if ( obj_camera->flag & OBD_CAMERA_SHIFT ){
        // シフトで各軸移動
        obj_camera->pos.x = ObjShiftSetF( obj_camera->pos.x, pos.x, obj_camera->shift, obj_camera->spd_max.x, obj_camera->spd_add.x );
        obj_camera->pos.y = ObjShiftSetF( obj_camera->pos.y, pos.x, obj_camera->shift, obj_camera->spd_max.y, obj_camera->spd_add.y );
        obj_camera->pos.z = ObjShiftSetF( obj_camera->pos.z, pos.x, obj_camera->shift, obj_camera->spd_max.z, obj_camera->spd_add.z );
    }else{
		// スクロール開始範囲セット
		obj_camera->allow.x = obj_camera->allow_limit.x;
		obj_camera->allow.z = obj_camera->allow_limit.z;
		if (ply_work->obj_work.move_flag & OBD_MOVE_JUMP) {	// 接地で判定する場合は、OBD_MOVE_UNDER
			// 空中
//			obj_camera->allow.y = MTM_MATH_MIN(obj_camera->allow.y + GMD_SCR_ALLOW_SPD, obj_camera->allow_limit.y);
			obj_camera->allow.y = obj_camera->allow_limit.y;
		} else {
			//　地上
			obj_camera->allow.y = MTM_MATH_MAX(obj_camera->allow.y - GMD_SCR_ALLOW_SPD, 0);
		}
		
		// スクロール開始範囲チェック(スクロールトリガ)
		// X
		if (pos.x < (obj_camera->pos.x - obj_camera->allow.x)) {
			pos.x += obj_camera->allow.x;
		} else if (pos.x > (obj_camera->pos.x + obj_camera->allow.x)) {
			pos.x -= obj_camera->allow.x;
		} else {
			pos.x = obj_camera->pos.x;
		}
		// Y
		if (pos.y < (obj_camera->pos.y - obj_camera->allow.y)) {
			pos.y += obj_camera->allow.y;
		} else if (pos.y > (obj_camera->pos.y + obj_camera->allow.y)) {
			pos.y -= obj_camera->allow.y;
		} else {
			pos.y = obj_camera->pos.y;
		}
        // 移動速度設定、加減速
        if ( pos.x != obj_camera->pos.x ){
            obj_camera->spd.x = ObjSpdUpSetF( obj_camera->spd.x, obj_camera->spd_add.x, obj_camera->spd_max.x);
        }else{
            obj_camera->spd.x = ObjSpdDownSetF( obj_camera->spd.x, obj_camera->spd_add.x);
        }
        if ( pos.y != obj_camera->pos.y ){
            obj_camera->spd.y = ObjSpdUpSetF( obj_camera->spd.y, obj_camera->spd_add.y, obj_camera->spd_max.y);
        }else{
            obj_camera->spd.y = ObjSpdDownSetF( obj_camera->spd.y, obj_camera->spd_add.y);
        }
        if ( pos.z != obj_camera->pos.z ){
            obj_camera->spd.z = ObjSpdUpSetF( obj_camera->spd.z, obj_camera->spd_add.z, obj_camera->spd_max.z);
        }else{
            obj_camera->spd.z = ObjSpdDownSetF( obj_camera->spd.z, obj_camera->spd_add.z);
        }
        // 各軸移動
        if ( pos.x > obj_camera->pos.x ){
            obj_camera->pos.x += obj_camera->spd.x;
            if ( obj_camera->pos.x > pos.x )
                obj_camera->pos.x = pos.x;
        }else{
            obj_camera->pos.x -= obj_camera->spd.x;
            if ( obj_camera->pos.x < pos.x )
                obj_camera->pos.x = pos.x;
        }
        if ( pos.y > obj_camera->pos.y ){
            obj_camera->pos.y += obj_camera->spd.y;
            if ( obj_camera->pos.y > pos.y )
                obj_camera->pos.y = pos.y;
        }else{
            obj_camera->pos.y -= obj_camera->spd.y;
            if ( obj_camera->pos.y < pos.y )
                obj_camera->pos.y = pos.y;
        }
        if ( pos.z > obj_camera->pos.z ){
            obj_camera->pos.z += obj_camera->spd.z;
            if ( obj_camera->pos.z > pos.z )
                obj_camera->pos.z = pos.z;
        }else{
            obj_camera->pos.z -= obj_camera->spd.z;
            if ( obj_camera->pos.z < pos.z )
                obj_camera->pos.z = pos.z;
        }
    }
#if 0
    // 離れオーバーチェック
    if ( MTM_MATH_ABS(pos.x - obj_camera->pos.x) > obj_camera->play_ofst_max.x ){
        if ( pos.x > obj_camera->pos.x ){
            obj_camera->pos.x = pos.x - obj_camera->play_ofst_max.x;
        }else{
            obj_camera->pos.x = pos.x + obj_camera->play_ofst_max.x;
        }

    }
    if ( MTM_MATH_ABS(pos.y - obj_camera->pos.y) > obj_camera->play_ofst_max.y ){
        if ( pos.y > obj_camera->pos.y ){
            obj_camera->pos.y = pos.y - obj_camera->play_ofst_max.y;
        }else{
            obj_camera->pos.y = pos.y + obj_camera->play_ofst_max.y;
        }

    }
#endif
    if ( MTM_MATH_ABS(pos.z - obj_camera->pos.z) > obj_camera->play_ofst_max.z ){
        if ( pos.z > obj_camera->pos.z ){
            obj_camera->pos.z = pos.z - obj_camera->play_ofst_max.z;
        }else{
            obj_camera->pos.z = pos.z + obj_camera->play_ofst_max.z;
        }
    }
    // 範囲オーバーチェック
//	objCameraPosLimitCheck(obj_camera, &obj_camera->pos );

	// 振動チェック
	obj_camera->disp_ofst.x = gm_camera_vibration.x / 16;
	obj_camera->disp_ofst.y = gm_camera_vibration.y / 16;
	obj_camera->disp_ofst.z = gm_camera_vibration.z / 16;
	gm_camera_vibration.x = gmCameraVibCheck(gm_camera_vibration.x);
	gm_camera_vibration.y = gmCameraVibCheck(gm_camera_vibration.y);
	gm_camera_vibration.z = gmCameraVibCheck(gm_camera_vibration.z);
	
	obj_camera->pos.z = 50.0f;

#if !_IPHONE
	// コンソール版
	obj_camera->disp_pos.x = obj_camera->pos.x + obj_camera->ofst.x;
	obj_camera->disp_pos.y = obj_camera->pos.y + obj_camera->ofst.y;
	obj_camera->disp_pos.z = obj_camera->pos.z + obj_camera->ofst.z;
#else
	// iPhone版：iPhone版は、画面拡大時のカメラオフセットに対しオフセット量を補正する
	{
		NNS_VECTOR	ofst;
		ofst.x = obj_camera->ofst.x;
		ofst.y = obj_camera->ofst.y;
		ofst.z = obj_camera->ofst.z;
		if (obj_camera->scale < GMD_CAMERA_SCALE) {
			// スケール値によりオフセット量を調整
			Float rate = obj_camera->scale - GMD_CAMERA_UP_SCALE_MAX;
			rate /= GMD_CAMERA_SCALE - GMD_CAMERA_UP_SCALE_MAX;
			ofst.x *= rate;
			ofst.y *= rate;
		}
		obj_camera->disp_pos.x = obj_camera->pos.x + ofst.x;
		obj_camera->disp_pos.y = obj_camera->pos.y + ofst.y;
		obj_camera->disp_pos.z = obj_camera->pos.z + ofst.z;
	}
#endif
	gmCameraLookupCheck(obj_camera);								// 見上げ/見下ろし
	
	// 表示限界リミットチェック
	{
#if 1
		float	left	= g_gm_main_system.map_fcol.left   + (float)(GSD_DISP_WIDTH/2) * obj_camera->scale;
		float	right	= g_gm_main_system.map_fcol.right  - (float)(GSD_DISP_WIDTH/2) * obj_camera->scale;
		float	top		= g_gm_main_system.map_fcol.top    + (float)(GSD_DISP_HEIGHT/2) * obj_camera->scale;
		float	bottom	= g_gm_main_system.map_fcol.bottom - (float)(GSD_DISP_HEIGHT/2) * obj_camera->scale;
#else
		float	left	= g_gm_main_system.map_fcol.left   + (float)(AMD_SCREEN_2D_WIDTH/2) * obj_camera->scale;
		float	right	= g_gm_main_system.map_fcol.right  - (float)(AMD_SCREEN_2D_WIDTH/2) * obj_camera->scale;
		float	top		= g_gm_main_system.map_fcol.top    + (float)(AMD_SCREEN_2D_HEIGHT/2) * obj_camera->scale;
		float	bottom	= g_gm_main_system.map_fcol.bottom - (float)(AMD_SCREEN_2D_HEIGHT/2) * obj_camera->scale;
#endif
		// scale変更時の左右端チェック
#if 1
		// 左端チェック
		if (obj_camera->disp_pos.x < left) {
			obj_camera->disp_pos.x = left;

			if (obj_camera->disp_pos.x > right) {
				obj_camera->disp_pos.x = (right + left) /2;
			}
		}
		// 右端チェック
		else
		if (obj_camera->disp_pos.x > right) {
			obj_camera->disp_pos.x = right;

			if (obj_camera->disp_pos.x < left) {
				obj_camera->disp_pos.x = (right + left) /2;
			}
		}
#else
		// 左端チェック
		if (obj_camera->disp_pos.x < left) {
			obj_camera->disp_pos.x = left;
		}
		// 右端チェック
		if (obj_camera->disp_pos.x > right) {
			obj_camera->disp_pos.x = right;
		}
#endif
		// 上端チェック
		if (obj_camera->disp_pos.y > -top) {
			obj_camera->disp_pos.y = -top;
		}
		// 下端チェック
		if (obj_camera->disp_pos.y < -bottom) {
			obj_camera->disp_pos.y = -bottom;
		}
	}

	obj_camera->target_pos = obj_camera->disp_pos;
	obj_camera->target_pos.z -= 50.0f;

	// スペステ回転時は何も回転補正しない
	if (GSM_MAIN_STAGE_IS_SPSTAGE()) {
	} else
#if 1
// ループカメラテスト開始その１
	if (  (obj_camera->flag & OBD_CAMERA_ROT_EX)
		&&(ply_work->obj_work.move_flag & OBD_MOVE_UNDER) ){
		// ループカメラ用回転制御
		// 接地時のみ
		s16	ply_roll;
		s16	cam_roll;
		s16	add_roll;
		ply_roll = (s16)(0-(s16)ply_work->obj_work.dir.z);
		cam_roll = (s16)obj_camera->roll;
		if (  ((u16)ply_roll > 0x4000)
			&&((u16)ply_roll < 0xc000) ) {
			add_roll = (s16)((u16)((u32)((u16)(ply_roll) + (u16)(cam_roll)) /2));
		} else {
			add_roll = (s16)((ply_roll + cam_roll) /2);
		}
		obj_camera->roll = (Angle32)add_roll;
		obj_camera->flag &= ~OBD_CAMERA_ROT_EX;
	}
#if 0	// 傾斜操作によるカメラ回転制御：ここから
	else
// ループカメラテスト終了その１

	if (!(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC)) {
		// 傾斜操作による画面回転
		Angle32	roll;
#if 0	// ほぼコントローラ角度直値
		roll = (0-AoPadRotZ())/4;
		roll = (roll + obj_camera->roll) /2;
#elif 0	// コントローラＺ回転角度履歴からの平均値
		u16		i;
		obj_camera->roll_ptr = ++obj_camera->roll_ptr & 0x0f;
		obj_camera->roll_hist[obj_camera->roll_ptr] = ply_work->key_rot_z/4;

		roll = 0;
		for (i = 0; i < 16;i++) {
			roll += obj_camera->roll_hist[i];
		}
		roll /= 16;
#elif 0	// コントローラＺ回転角度累乗値履歴からの平均値
		u16		i;
		float	mul;
		obj_camera->roll_ptr = ++obj_camera->roll_ptr & 0x0f;

		roll = ply_work->key_rot_z/4;
		mul = float(roll);
		mul = MTM_MATH_ABS(mul / (GMD_SCR_ROLL_MAX/2));
		roll = fx32(roll * mul);
		obj_camera->roll_hist[obj_camera->roll_ptr] = roll;

		roll = 0;
		for (i = 0; i < 16;i++) {
			roll += obj_camera->roll_hist[i];
		}
		roll /= 16;
#elif 1	// コントローラＺ回転角度累乗値履歴からの平均値(コントローラ傾斜リミットをプレイヤー傾斜にあわせる)
		u16		i;
		float	fMul,fRol;
		obj_camera->roll_ptr = ++obj_camera->roll_ptr & 0x0f;

		roll = MTM_MATH_CLIP(ply_work->key_rot_z, -GMD_PL_DEF_ROLL_MAX, GMD_PL_DEF_ROLL_MAX);
		fRol = (float)(roll * GMD_SCR_ROLL_MAX / GMD_PL_DEF_ROLL_MAX);
		fMul = fRol;
		fMul = MTM_MATH_ABS(fMul / GMD_SCR_ROLL_MAX);
		roll = fx32(fRol * fMul);
		obj_camera->roll_hist[obj_camera->roll_ptr] = roll;

		roll = 0;
		for (i = 0; i < 16;i++) {
			roll += obj_camera->roll_hist[i];
		}
		roll /= 16;
#else	// プレイヤー速度から回転角を求める
		u16		i;
//		fx32	spd = ply_work->obj_work.spd_m;
//		roll = FXM_FX32_TO_FLOAT(spd);
	//	roll = FXM_FX32_TO_FLOAT(ply_work->obj_work.spd_m);
		roll = Angle32(ply_work->obj_work.spd_m) / 12;

		obj_camera->roll_ptr = ++obj_camera->roll_ptr & 0x07;
		obj_camera->roll_hist[obj_camera->roll_ptr] = roll;

		roll = 0;
		for (i = 0; i < 8;i++) {
			roll += obj_camera->roll_hist[i];
		}
		roll /= 8;
#endif	// コントロールタイプ
		obj_camera->roll = MTM_MATH_CLIP(roll, -GMD_SCR_ROLL_MAX, GMD_SCR_ROLL_MAX);
	}
#endif	// 傾斜操作によるカメラ回転制御：ここまで
// ループカメラテスト開始その２
	else {
		// 徐々に角度を戻す
		obj_camera->roll = obj_camera->roll * 9 / 10;
		obj_camera->flag &= ~OBD_CAMERA_ROT_EX;
	}
// ループカメラテスト終了その２
#endif

	// オブジェクトカメラ設定
	ObjObjectCameraSet(FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)),
						FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)));
	
	// クリッピングカメラ設定
	GmCameraSetClipCamera(obj_camera);

#if defined(MTD_DEBUG)

#if !defined (HOG_ALPHA_ROM) && !defined (HOG_PRESENT_ROM_IPHONE)
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		s32	y = 18;

		amPrintf(2, y, "CAM X : %f", obj_camera->pos.x);		y++;
		amPrintf(2, y, "CAM Y : %f", obj_camera->pos.y);		y++;
		amPrintf(2, y, "CAM Z : %f", obj_camera->pos.z);		y++;
		amPrintf(2, y, "TAR X : %f", obj_camera->target_pos.x);	y++;
		amPrintf(2, y, "TAR Y : %f", obj_camera->target_pos.y);	y++;
		amPrintf(2, y, "TAR Z : %f", obj_camera->target_pos.z);	y++;
#if _PS3
		amPrintf(2, y, "ROT X : %d", AoPadRotX());	y++;
		amPrintf(2, y, "ROT Z : %d", AoPadRotZ());	y++;

		amPrintf(2, y, "AXS X : %d", AoPadAxisX());	y++;
		amPrintf(2, y, "AXS Y : %d", AoPadAxisY());	y++;
		amPrintf(2, y, "AXS Z : %d", AoPadAxisZ());	y++;
		amPrintf(2, y, "AXS G : %d", AoPadAxisG());	y++;

#elif _PC | _XBOX
		amPrintf(2, y, "RadLT : %d", AoPadTriggerL());	y++;
		amPrintf(2, y, "PadRT : %d", AoPadTriggerR());	y++;
#elif _WII
		amPrintf(2, y, "ROT X : %d", AoPadRotX());	y++;

		amPrintf(2, y, "AXS X : %f", AoPadAxisX());	y++;
		amPrintf(2, y, "AXS Y : %f", AoPadAxisY());	y++;
		amPrintf(2, y, "AXS Z : %f", AoPadAxisZ());	y++;
#endif	// PF毎の表示
	}
#endif //!defined (HOG_ALPHA_ROM) && !defined (HOG_PRESENT_ROM_IPHONE)

#endif //defined(MTD_DEBUG)
}

// ==========================================================================
// gmCameraFuncMapFar
/*!
 *	遠景用カメラ
 *
 *	@param	obj_camera	[in]	オブジェクトカメラ
 */
// ==========================================================================
void gmCameraFuncMapFar(OBS_CAMERA *obj_camera)
{
	
	//プレイヤカメラの座標から遠景カメラの座標を決定
	OBS_CAMERA* player_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	obj_camera->pos = GmMapFarGetCameraPos( &player_camera->disp_pos );
	obj_camera->target_pos = GmMapFarGetCameraTarget( &obj_camera->pos );
	obj_camera->disp_pos = obj_camera->pos;

	//ロール
	if (!GSM_MAIN_STAGE_IS_SPSTAGE()) {
		// 一般Zone
		obj_camera->roll = player_camera->roll;
	} else {
		// スペステは回転しない
	}

#if defined(MTD_DEBUG)

#if !defined (HOG_ALPHA_ROM) && !defined (HOG_PRESENT_ROM_IPHONE)
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		int y = 30;

		amPrintf(2, y, "FCAM X : %f", obj_camera->pos.x);		y++;
		amPrintf(2, y, "FCAM Y : %f", obj_camera->pos.y);		y++;
		amPrintf(2, y, "FCAM Z : %f", obj_camera->pos.z);		y++;
		amPrintf(2, y, "FTAR X : %f", obj_camera->target_pos.x);	y++;
		amPrintf(2, y, "FTAR Y : %f", obj_camera->target_pos.y);	y++;
		amPrintf(2, y, "FTAR Z : %f", obj_camera->target_pos.z);	y++;
		amPrintf(2, y, "FOVY   : %f", NNM_A32toDEG(player_camera->fovy));	y++;
	}
#endif //!defined (HOG_ALPHA_ROM) && !defined (HOG_PRESENT_ROM_IPHONE)
#endif // defined(MTD_DEBUG)

}


// ==========================================================================
// gmCameraFuncAddMap
/*!
 *	超近景用カメラ
 *
 *	@param	obj_camera	[in]	オブジェクトカメラ
 */
// ==========================================================================
void gmCameraFuncAddMap(OBS_CAMERA *obj_camera)
{
#if 1
	// プレイヤカメラの座標から近景カメラの座標を決定
	OBS_CAMERA	*main_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
//	NNS_VECTOR	vec_temp;

	obj_camera->prev_pos		= obj_camera->pos;
	obj_camera->prev_disp_pos	= obj_camera->disp_pos;

	// カメラコピー
	obj_camera->disp_pos		= main_camera->disp_pos;
	//obj_camera->prev_disp_pos	= main_camera->prev_disp_pos;
	obj_camera->pos				= main_camera->pos;
	//obj_camera->prev_pos		= main_camera->prev_pos;
	obj_camera->ofst			= main_camera->ofst;
	obj_camera->disp_ofst		= main_camera->disp_ofst;
	obj_camera->target_ofst		= main_camera->target_ofst;
	obj_camera->play_ofst_max	= main_camera->play_ofst_max;
//	obj_camera->allow		= main_camera->allow;
//	obj_camera->allow_limit	= main_camera->allow_limit;
//	obj_camera->target_obj	= main_camera->target_obj;
//	obj_camera->target_pos	= main_camera->target_pos;
	obj_camera->camup_obj	= main_camera->camup_obj;
	obj_camera->camup_pos	= main_camera->camup_pos;
//	obj_camera->spd			= main_camera->spd;
//	obj_camera->spd_add		= main_camera->spd_add;
//	obj_camera->spd_max		= main_camera->spd_max;
	obj_camera->roll		= main_camera->roll;
//	obj_camera->roll_hist	= main_camera->roll_hist;
//	obj_camera->roll_ptr	= main_camera->roll_ptr;
//	obj_camera->shift		= main_camera->shift;
//	obj_camera->index		= main_camera->index;
//	obj_camera->work		= main_camera->work;
//	obj_camera->command_state	= main_camera->command_state;
//	obj_camera->limit			= main_camera->limit;
//	obj_camera->camera_type		= main_camera->camera_type;
//	obj_camera->prj_pers_mtx	= main_camera->prj_pers_mtx;
//	obj_camera->prj_ortho_mtx	= main_camera->prj_ortho_mtx;
//	obj_camera->view_mtx		= main_camera->view_mtx;
//	obj_camera->fovy		= main_camera->fovy;
	obj_camera->up_vec		= main_camera->up_vec;
	obj_camera->scale		= main_camera->scale;
	obj_camera->left		= main_camera->left;
	obj_camera->right		= main_camera->right;
	obj_camera->bottom		= main_camera->bottom;
	obj_camera->top			= main_camera->top;
	obj_camera->znear		= main_camera->znear;
	obj_camera->zfar		= main_camera->zfar;
	obj_camera->aspect		= main_camera->aspect;


	// 座標補正
	//GmMapGetAddMapCameraPos(&main_camera->pos, &obj_camera->pos, obj_camera->camera_id);
	//GmMapGetAddMapCameraPos(&main_camera->disp_pos, &obj_camera->disp_pos, obj_camera->camera_id);
	//GmMapGetAddMapCameraPos(&main_camera->target_pos, &obj_camera->target_pos, obj_camera->camera_id);
	GmMapGetAddMapCameraPos(&main_camera->disp_pos, &main_camera->target_pos,
				&obj_camera->disp_pos, &obj_camera->target_pos, obj_camera->camera_id);

	//ロール
	//obj_camera->roll = main_camera->roll;
#if 0
	// テスト
	GmMapSetAddMapXLoop();

	GmMapEnableAddMapUserScrlX();
	GmMapSetAddMapScrlScaleMagX(GME_MAP_ADD_MAP_NEAR, 4);
	GmMapSetAddMapUserScrlXAddSize(1.f);
#endif


#else
	// プレイヤカメラの座標から近景カメラの座標を決定
	OBS_CAMERA	*main_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	NNS_VECTOR	vec_temp;

	// 座標補正
	GmMapGetAddMapCameraPos(&main_camera->disp_pos, &obj_camera->disp_pos);
	obj_camera->pos	= obj_camera->disp_pos;
	nnSubtractVector(&vec_temp, &main_camera->target_pos, &main_camera->disp_pos); 
	nnAddVector(&main_camera->target_pos, &vec_temp, &obj_camera->pos);

	//ロール
	obj_camera->roll = main_camera->roll;
#endif
}

// ==========================================================================
// gmCameraFuncWater
/*!
 *	水面用カメラ
 *
 *	@param	obj_camera	[in]	オブジェクトカメラ
 */
// ==========================================================================
void gmCameraFuncWater(OBS_CAMERA *obj_camera)
{
	//メインカメラの座標を設定
	OBS_CAMERA* main_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	nnCopyVector( &obj_camera->pos, &main_camera->pos);
	nnCopyVector( &obj_camera->target_pos, &main_camera->target_pos);
	nnCopyVector( &obj_camera->disp_pos, &main_camera->disp_pos);

	//ロール
	//obj_camera->roll = main_camera->roll;
}

// ==========================================================================
// GmCameraTruckFunc
/*!
 *	メインカメラ トロッコ用
 *	@param	obj_camera	[in]	オブジェクトカメラ
 */
// ==========================================================================
void GmCameraTruckFunc(OBS_CAMERA *obj_camera)
{
    NNS_VECTOR pos;
	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if ((ply_work->player_flag & GMD_PLF_DIE) &&
				!(ply_work->player_flag & GMD_PLF_TATK_RETRY)) {
		// プレイヤー死亡時はカメラ不動(リトライ演出中を除く)
		return;
	}
/*
    if ( obj_camera->target_obj ){
//		pos.x = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.x - ((OBD_LCD_X>>1) << FX32_SHIFT)) + obj_camera->target_ofst.x;
//		pos.y = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.y - ((OBD_LCD_Y>>1) << FX32_SHIFT)) + obj_camera->target_ofst.y;
//		pos.z = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.z) + obj_camera->target_ofst.z;
		pos.x = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.x) + obj_camera->target_ofst.x;
		pos.y = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.y - (((OBD_LCD_Y<<1)+200) << FX32_SHIFT)) + obj_camera->target_ofst.y;
		pos.z = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.z) + obj_camera->target_ofst.z;
    }else{
        pos.x = obj_camera->target_pos.x - FXM_FX32_TO_FLOAT(((OBD_LCD_X>>1) << FX32_SHIFT)) + obj_camera->target_ofst.x;
        pos.y = obj_camera->target_pos.y - FXM_FX32_TO_FLOAT(((OBD_LCD_Y>>1) << FX32_SHIFT)) + obj_camera->target_ofst.y;
        pos.z = obj_camera->target_pos.z + obj_camera->target_ofst.z;
    }
*/
	pos.x = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.x);
    pos.y = FXM_FX32_TO_FLOAT(-ply_work->obj_work.pos.y + GMD_SCR_PLY_Y_OFFS);
    pos.z = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.z);

    // 目的値を記録
    obj_camera->work.x = pos.x;
    obj_camera->work.y = pos.y;
    obj_camera->work.z = pos.z;

    obj_camera->prev_pos.x = obj_camera->pos.x;
    obj_camera->prev_pos.y = obj_camera->pos.y;
    obj_camera->prev_pos.z = obj_camera->pos.z;

    if ( obj_camera->flag & OBD_CAMERA_FIX ){
        if ( obj_camera->target_obj ){
            obj_camera->pos.x = pos.x;
            obj_camera->pos.y = pos.y;
            obj_camera->pos.z = pos.z;
        }

        return;
    }

    if ( obj_camera->flag & OBD_CAMERA_SHIFT ){
        // シフトで各軸移動
        obj_camera->pos.x = ObjShiftSetF( obj_camera->pos.x, pos.x, obj_camera->shift, obj_camera->spd_max.x, obj_camera->spd_add.x );
        obj_camera->pos.y = ObjShiftSetF( obj_camera->pos.y, pos.x, obj_camera->shift, obj_camera->spd_max.y, obj_camera->spd_add.y );
        obj_camera->pos.z = ObjShiftSetF( obj_camera->pos.z, pos.x, obj_camera->shift, obj_camera->spd_max.z, obj_camera->spd_add.z );
    }else{
		// スクロール開始範囲セット
		obj_camera->allow.x = obj_camera->allow_limit.x;
		obj_camera->allow.z = obj_camera->allow_limit.z;
		if (ply_work->obj_work.move_flag & OBD_MOVE_JUMP) {	// 接地で判定する場合は、OBD_MOVE_UNDER
			// 空中
//			obj_camera->allow.y = MTM_MATH_MIN(obj_camera->allow.y + GMD_SCR_ALLOW_SPD, obj_camera->allow_limit.y);
			obj_camera->allow.y = obj_camera->allow_limit.y;
		} else {
			//　地上
			obj_camera->allow.y = MTM_MATH_MAX(obj_camera->allow.y - GMD_SCR_ALLOW_SPD, 0);
		}
		
		// スクロール開始範囲チェック(スクロールトリガ)
		// X
		if (pos.x < (obj_camera->pos.x - obj_camera->allow.x)) {
			pos.x += obj_camera->allow.x;
		} else if (pos.x > (obj_camera->pos.x + obj_camera->allow.x)) {
			pos.x -= obj_camera->allow.x;
		} else {
			pos.x = obj_camera->pos.x;
		}
		// Y
		if (pos.y < (obj_camera->pos.y - obj_camera->allow.y)) {
			pos.y += obj_camera->allow.y;
		} else if (pos.y > (obj_camera->pos.y + obj_camera->allow.y)) {
			pos.y -= obj_camera->allow.y;
		} else {
			pos.y = obj_camera->pos.y;
		}
        // 移動速度設定、加減速
        if ( pos.x != obj_camera->pos.x ){
            obj_camera->spd.x = ObjSpdUpSetF( obj_camera->spd.x, obj_camera->spd_add.x, obj_camera->spd_max.x);
        }else{
            obj_camera->spd.x = ObjSpdDownSetF( obj_camera->spd.x, obj_camera->spd_add.x);
        }
        if ( pos.y != obj_camera->pos.y ){
            obj_camera->spd.y = ObjSpdUpSetF( obj_camera->spd.y, obj_camera->spd_add.y, obj_camera->spd_max.y);
        }else{
            obj_camera->spd.y = ObjSpdDownSetF( obj_camera->spd.y, obj_camera->spd_add.y);
        }
        if ( pos.z != obj_camera->pos.z ){
            obj_camera->spd.z = ObjSpdUpSetF( obj_camera->spd.z, obj_camera->spd_add.z, obj_camera->spd_max.z);
        }else{
            obj_camera->spd.z = ObjSpdDownSetF( obj_camera->spd.z, obj_camera->spd_add.z);
        }
        // 各軸移動
        if ( pos.x > obj_camera->pos.x ){
            obj_camera->pos.x += obj_camera->spd.x;
            if ( obj_camera->pos.x > pos.x )
                obj_camera->pos.x = pos.x;
        }else{
            obj_camera->pos.x -= obj_camera->spd.x;
            if ( obj_camera->pos.x < pos.x )
                obj_camera->pos.x = pos.x;
        }
        if ( pos.y > obj_camera->pos.y ){
            obj_camera->pos.y += obj_camera->spd.y;
            if ( obj_camera->pos.y > pos.y )
                obj_camera->pos.y = pos.y;
        }else{
            obj_camera->pos.y -= obj_camera->spd.y;
            if ( obj_camera->pos.y < pos.y )
                obj_camera->pos.y = pos.y;
        }
        if ( pos.z > obj_camera->pos.z ){
            obj_camera->pos.z += obj_camera->spd.z;
            if ( obj_camera->pos.z > pos.z )
                obj_camera->pos.z = pos.z;
        }else{
            obj_camera->pos.z -= obj_camera->spd.z;
            if ( obj_camera->pos.z < pos.z )
                obj_camera->pos.z = pos.z;
        }
    }
#if 0
    // 離れオーバーチェック
    if ( MTM_MATH_ABS(pos.x - obj_camera->pos.x) > obj_camera->play_ofst_max.x ){
        if ( pos.x > obj_camera->pos.x ){
            obj_camera->pos.x = pos.x - obj_camera->play_ofst_max.x;
        }else{
            obj_camera->pos.x = pos.x + obj_camera->play_ofst_max.x;
        }

    }
    if ( MTM_MATH_ABS(pos.y - obj_camera->pos.y) > obj_camera->play_ofst_max.y ){
        if ( pos.y > obj_camera->pos.y ){
            obj_camera->pos.y = pos.y - obj_camera->play_ofst_max.y;
        }else{
            obj_camera->pos.y = pos.y + obj_camera->play_ofst_max.y;
        }

    }
#endif
    if ( MTM_MATH_ABS(pos.z - obj_camera->pos.z) > obj_camera->play_ofst_max.z ){
        if ( pos.z > obj_camera->pos.z ){
            obj_camera->pos.z = pos.z - obj_camera->play_ofst_max.z;
        }else{
            obj_camera->pos.z = pos.z + obj_camera->play_ofst_max.z;
        }
    }
    // 範囲オーバーチェック
//	objCameraPosLimitCheck(obj_camera, &obj_camera->pos );

	// 振動チェック
	obj_camera->disp_ofst.x = gm_camera_vibration.x / 16;
	obj_camera->disp_ofst.y = gm_camera_vibration.y / 16;
	obj_camera->disp_ofst.z = gm_camera_vibration.z / 16;
	gm_camera_vibration.x = gmCameraVibCheck(gm_camera_vibration.x);
	gm_camera_vibration.y = gmCameraVibCheck(gm_camera_vibration.y);
	gm_camera_vibration.z = gmCameraVibCheck(gm_camera_vibration.z);
	
	obj_camera->pos.z = 50.0f;
#if !_IPHONE
	// コンソール版
	obj_camera->disp_pos.x = obj_camera->pos.x + obj_camera->ofst.x;
	obj_camera->disp_pos.y = obj_camera->pos.y + obj_camera->ofst.y;
	obj_camera->disp_pos.z = obj_camera->pos.z + obj_camera->ofst.z;
#else
	// iPhone版：iPhone版は、画面拡大時のカメラオフセットに対しオフセット量を補正する
	{
		NNS_VECTOR	ofst;
		ofst.x = obj_camera->ofst.x;
		ofst.y = obj_camera->ofst.y;
		ofst.z = obj_camera->ofst.z;
		if (obj_camera->scale < GMD_CAMERA_SCALE) {
			// スケール値によりオフセット量を調整
			Float rate = obj_camera->scale - GMD_CAMERA_UP_SCALE_MAX;
			rate /= GMD_CAMERA_SCALE - GMD_CAMERA_UP_SCALE_MAX;
			ofst.x *= rate;
			ofst.y *= rate;
		}
		obj_camera->disp_pos.x = obj_camera->pos.x + ofst.x;
		obj_camera->disp_pos.y = obj_camera->pos.y + ofst.y;
		obj_camera->disp_pos.z = obj_camera->pos.z + ofst.z;
	}
#endif
	gmCameraLookupCheck(obj_camera);								// 見上げ/見下ろし

	// 表示限界リミットチェック
	{
#if 1
		float	left	= g_gm_main_system.map_fcol.left   + (float)(GSD_DISP_WIDTH/2) * obj_camera->scale;
		float	right	= g_gm_main_system.map_fcol.right  - (float)(GSD_DISP_WIDTH/2) * obj_camera->scale;
		float	top		= g_gm_main_system.map_fcol.top    + (float)(GSD_DISP_HEIGHT/2) * obj_camera->scale;
		float	bottom	= g_gm_main_system.map_fcol.bottom - (float)(GSD_DISP_HEIGHT/2) * obj_camera->scale;
#else
		float	left	= g_gm_main_system.map_fcol.left   + (float)(AMD_SCREEN_2D_WIDTH/2) * obj_camera->scale;
		float	right	= g_gm_main_system.map_fcol.right  - (float)(AMD_SCREEN_2D_WIDTH/2) * obj_camera->scale;
		float	top		= g_gm_main_system.map_fcol.top    + (float)(AMD_SCREEN_2D_HEIGHT/2) * obj_camera->scale;
		float	bottom	= g_gm_main_system.map_fcol.bottom - (float)(AMD_SCREEN_2D_HEIGHT/2) * obj_camera->scale;
#endif
		
		// 左端チェック
		if (obj_camera->disp_pos.x < left) {
			obj_camera->disp_pos.x = left;
		}
		// 右端チェック
		if (obj_camera->disp_pos.x > right) {
			obj_camera->disp_pos.x = right;
		}
		// 上端チェック
		if (obj_camera->disp_pos.y > -top) {
			obj_camera->disp_pos.y = -top;
		}
		// 下端チェック
		if (obj_camera->disp_pos.y < -bottom) {
			obj_camera->disp_pos.y = -bottom;
		}
	}

	obj_camera->target_pos = obj_camera->disp_pos;
	obj_camera->target_pos.z -= 50.0f;

#if 0
// ループカメラテスト開始その１
	if (  (obj_camera->flag & OBD_CAMERA_ROT_EX)
		&&(ply_work->obj_work.move_flag & OBD_MOVE_UNDER) ){
		// ループカメラ用回転制御
		// 接地時のみ
		s16	ply_roll;
		s16	cam_roll;
		s16	add_roll;
		ply_roll = (0-(s16)ply_work->obj_work.dir.z);
		cam_roll = (s16)obj_camera->roll;
		if (  ((u16)ply_roll > 0x4000)
			&&((u16)ply_roll < 0xc000) ) {
			add_roll = (u16)((u32)((u16)(ply_roll) + (u16)(cam_roll)) /2);
		} else {
			add_roll = (ply_roll + cam_roll) /2;
		}
		obj_camera->roll = (Angle32)add_roll;
		obj_camera->flag &= ~OBD_CAMERA_ROT_EX;
	}
	else
// ループカメラテスト終了その１
#endif // #if 0



	// 傾斜カメラ
	// 重力方向に変更
	{
		Angle32	rot_dist, rot_spd;
		Angle32 rot_max_spd = GMD_CAMERA_TRUCK_ROLL_SPEED_MAX;

		if (ply_work->gmk_flag2 & GMD_PLGF2_TRUCK_CAM_ROT_SLOW) {
			// カメラ最大回転速度をゆっくりに
			rot_max_spd = GMD_CAMERA_TRUCK_ROLL_SPEED_SLOW_MAX;
		}

		rot_dist = -ply_work->ply_pseudofall_dir - obj_camera->roll;

		if (rot_dist > 0x8000) {
			// 近い方に回す
			rot_dist -= 0x10000;
		}
		else if (rot_dist < -0x8000) {
			// 近い方に回す
			rot_dist += 0x10000;
		}

		if (MTM_MATH_ABS(rot_dist) < 0x0080/* 最小回転量 */) {
			rot_spd = rot_dist;
		}
		else {
			rot_spd = rot_dist >> 4;
#if _IPHONE
			if (MTM_MATH_ABS(rot_spd) > GMD_CAMERA_TRUCK_ROLL_SPEED_ACCEL/*最大回転量*/) {
				rot_spd += rot_spd - GMD_CAMERA_TRUCK_ROLL_SPEED_ACCEL; // 一定以上の角度は倍値対応
			}
#endif // _IPHONE
			if (MTM_MATH_ABS(rot_spd) > rot_max_spd/*最大回転量*/) {
				if (rot_spd >= 0) {
					rot_spd = rot_max_spd;
				}
				else {
					rot_spd = -rot_max_spd;
				}
			}
			else if (MTM_MATH_ABS(rot_spd) < 0x0080/* 最小回転量 */) {
				if (rot_spd >= 0) {
					rot_spd = 0x0080;
				}
				else {
					rot_spd = -0x0080;
				}
			}
		}

#ifndef GMD_MAIN_USE_BODY_ROTATE
		obj_camera->roll += rot_spd;
#endif // GMD_MAIN_USE_BODY_ROTATE

		// 1回転を超えた場合は戻しておく
		if (obj_camera->roll > 0x10000) {
			obj_camera->roll -= 0x10000;
			ply_work->ply_pseudofall_dir += 0x10000;
		}
		else if (obj_camera->roll < -0x10000) {
			obj_camera->roll += 0x10000;
			ply_work->ply_pseudofall_dir -= 0x10000;
		}
	}


#if 0
	// 傾斜操作によるカメラ回転制御：ここから
	//if (!(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC)) {
	{
		// 傾斜操作による画面回転
		Angle32	roll;
#if 0	// ほぼコントローラ角度直値
		roll = (0-AoPadRotZ())/4;
		roll = (roll + obj_camera->roll) /2;
#elif 0	// コントローラＺ回転角度履歴からの平均値
		u16		i;
		obj_camera->roll_ptr = ++obj_camera->roll_ptr & 0x0f;
		obj_camera->roll_hist[obj_camera->roll_ptr] = ply_work->key_rot_z/4;

		roll = 0;
		for (i = 0; i < 16;i++) {
			roll += obj_camera->roll_hist[i];
		}
		roll /= 16;
#elif 0	// コントローラＺ回転角度累乗値履歴からの平均値
		u16		i;
		float	mul;
		obj_camera->roll_ptr = ++obj_camera->roll_ptr & 0x0f;

		roll = ply_work->key_rot_z/4;
		mul = float(roll);
		mul = MTM_MATH_ABS(mul / (GMD_SCR_ROLL_MAX/2));
		roll = fx32(roll * mul);
		obj_camera->roll_hist[obj_camera->roll_ptr] = roll;

		roll = 0;
		for (i = 0; i < 16;i++) {
			roll += obj_camera->roll_hist[i];
		}
		roll /= 16;
#elif 1	// コントローラＺ回転角度累乗値履歴からの平均値(コントローラ傾斜リミットをプレイヤー傾斜にあわせる)
		u16		i;
		float	fMul,fRol;
		obj_camera->roll_ptr = ++obj_camera->roll_ptr & 0x0f;

		roll = MTM_MATH_CLIP(ply_work->key_rot_z, -GMD_PL_DEF_ROLL_MAX, GMD_PL_DEF_ROLL_MAX);
		fRol = (float)(roll * GMD_SCR_ROLL_MAX / GMD_PL_DEF_ROLL_MAX);
		fMul = fRol;
		fMul = MTM_MATH_ABS(fMul / GMD_SCR_ROLL_MAX);
		roll = fx32(fRol * fMul);
		obj_camera->roll_hist[obj_camera->roll_ptr] = roll;

		roll = 0;
		for (i = 0; i < 16;i++) {
			roll += obj_camera->roll_hist[i];
		}
		roll /= 16;
#else	// プレイヤー速度から回転角を求める
		u16		i;
//		fx32	spd = ply_work->obj_work.spd_m;
//		roll = FXM_FX32_TO_FLOAT(spd);
	//	roll = FXM_FX32_TO_FLOAT(ply_work->obj_work.spd_m);
		roll = Angle32(ply_work->obj_work.spd_m) / 12;

		obj_camera->roll_ptr = ++obj_camera->roll_ptr & 0x07;
		obj_camera->roll_hist[obj_camera->roll_ptr] = roll;

		roll = 0;
		for (i = 0; i < 8;i++) {
			roll += obj_camera->roll_hist[i];
		}
		roll /= 8;
#endif	// コントロールタイプ
		obj_camera->roll = MTM_MATH_CLIP(roll, -GMD_SCR_ROLL_MAX, GMD_SCR_ROLL_MAX);
	}
	// 傾斜操作によるカメラ回転制御：ここまで
#endif


#if 0
// ループカメラテスト開始その２
	else {

		obj_camera->roll = obj_camera->roll * 9 / 10;
		obj_camera->flag &= ~OBD_CAMERA_ROT_EX;
	}
// ループカメラテスト終了その２
#endif

	// オブジェクトカメラ設定
	ObjObjectCameraSet(FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)),
						FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)));
	
	// クリッピングカメラ設定
	GmCameraSetClipCamera(obj_camera);


#if defined(MTD_DEBUG)

#if 1	// ステータス表示：ここから
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		s32	y = 18;

		amPrintf(2, y, "CAM X : %f", obj_camera->pos.x);		y++;
		amPrintf(2, y, "CAM Y : %f", obj_camera->pos.y);		y++;
		amPrintf(2, y, "CAM Z : %f", obj_camera->pos.z);		y++;
		amPrintf(2, y, "TAR X : %f", obj_camera->target_pos.x);	y++;
		amPrintf(2, y, "TAR Y : %f", obj_camera->target_pos.y);	y++;
		amPrintf(2, y, "TAR Z : %f", obj_camera->target_pos.z);	y++;
#if _PS3
		amPrintf(2, y, "ROT X : %d", AoPadRotX());	y++;
		amPrintf(2, y, "ROT Z : %d", AoPadRotZ());	y++;
#elif _PC | _XBOX
		amPrintf(2, y, "RadLT : %d", AoPadTriggerL());	y++;
		amPrintf(2, y, "PadRT : %d", AoPadTriggerR());	y++;
#elif _WII
		amPrintf(2, y, "ROT X : %d", AoPadRotX());	y++;
#endif	// PF毎の表示
		amPrintf(2, y, "CAM ROT : %f", obj_camera->roll);	y++;
		
	}
#endif	// ステータス表示：ここまで

#endif // defined(MTD_DEBUG)

}


// ==========================================================================
// gmCameraVibCheck
/*!
 *	振動量チェック
 *
 *	@param	vib		[in]	振動量変数
 */
// ==========================================================================
Float gmCameraVibCheck(Float vib)
{
	if (vib) {
		if (vib > 0) {
			vib = 0 - (vib - 1);
			if (vib > 0) {
				vib = 0;
			}
		} else {
			vib = 0 - (vib + 1);
			if (vib < 0) {
				vib = 0;
			}
		}
//		vib = 0-(vib / 2);
	}
	return (vib);
}

// ==========================================================================
// gmCameraLookupCheck
/*!
 *	見上げ/見下ろしチェック
 *
 *	@param	obj_camera	[in]	オブジェクトカメラ
 */
// ==========================================================================
void gmCameraLookupCheck(OBS_CAMERA *obj_camera)
{
	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	s32	now_flag = 0;

	// 見上げ/見下ろしチェック
	if (GSM_MAIN_STAGE_IS_SPSTAGE())
		return;

	// 見上げ
	if (  (ply_work->seq_state == GME_PLY_SEQ_STATE_LOOKUP_M)
		&&(ply_work->key_on & PAD_KEY_UP) ) {

		now_flag = GMD_CAM_FLAG_LOOKUP;
	}

	// 見下ろし
	if (  (ply_work->seq_state == GME_PLY_SEQ_STATE_SQUAT_M)
		&&(ply_work->key_on & PAD_KEY_DOWN) ) {

		now_flag = GMD_CAM_FLAG_LOOKDOWN;
	}

	if (  (gm_camera_work.flag & now_flag)
		&&(gm_camera_work.timer > GMD_CAM_LOOKUP_TIME) ) {
		// スクロール中
		if (gm_camera_work.flag & GMD_CAM_FLAG_LOOKUP) {
			gm_camera_work.offset += GMD_CAM_LOOKUP_SCR_SPD;
		} else {
			gm_camera_work.offset -= GMD_CAM_LOOKUP_SCR_SPD;
		}
		s32 gaze = (s32)(ply_work->camera_ofst_y >> FX32_SHIFT);	// カメラ注視点変更を考慮(カメラセンター変更)
		gm_camera_work.offset = 
			MTM_MATH_CLIP(gm_camera_work.offset, GMD_CAM_LOOKUP_SCR_LMT_DW - gaze, GMD_CAM_LOOKUP_SCR_LMT_UP - gaze);
	} else {
		if (gm_camera_work.flag & now_flag) {
			// 入力継続
			gm_camera_work.timer++;
		} else if (now_flag) {
			// 新規入力
			gm_camera_work.flag = now_flag;
			gm_camera_work.timer = 1;
		} else {
			// 入力なし
			gm_camera_work.flag = 0;
			gm_camera_work.timer = 0;
		}

		// スクロールを戻す
		if (gm_camera_work.offset > 0) {
			gm_camera_work.offset -= GMD_CAM_LOOKUP_SCR_SPD;
			if (gm_camera_work.offset < 0) {
				gm_camera_work.offset = 0;
			}
		} else if (gm_camera_work.offset < 0) {
			gm_camera_work.offset += GMD_CAM_LOOKUP_SCR_SPD;
			if (gm_camera_work.offset > 0) {
				gm_camera_work.offset = 0;
			}
		}
	
	}

	// オフセット加算
	u16 roll = (u16)((obj_camera->roll + 0x2000) >> 14);
	switch (roll) {
		case 0:	// 回転なし
			obj_camera->disp_pos.y += gm_camera_work.offset;
			break;
		case 1:	// 左が天井
			obj_camera->disp_pos.x -= gm_camera_work.offset;
			break;
		case 2:	// 下が天井
			obj_camera->disp_pos.y -= gm_camera_work.offset;
			break;
		case 3:	// 右が天井
			obj_camera->disp_pos.x += gm_camera_work.offset;
			break;
	}
}



// ==========================================================================
// gmCameraScaleChange
/*!
 *	スケール変更チェック＆実行
 *
 *	@param	obj_camera	[in]	オブジェクトカメラ
 */
// ==========================================================================
void gmCameraScaleChange(OBS_CAMERA *obj_camera)
{
#if 0	// デバッグ操作
	if (AoPadStand() & KEY_R1) {
		// ＲＢ PUSHで拡縮率切り替え
		if (AoPadTriggerR() > (s32)(0x400 * 0.007782)) {
			// ＲＴ押していたら縮小
			GmCameraScaleSet(0.72f, 0.01f);
		} else {
			// ＲＴ押してなかったら元通り
			GmCameraScaleSet(1.0f, 0.01f);
		}
	}
#endif


	if (gm_camera_work.scale_now == gm_camera_work.scale_target) {
		return;
	}

	if (gm_camera_work.scale_now < gm_camera_work.scale_target) {
		// 拡大
		gm_camera_work.scale_now += gm_camera_work.scale_spd;
		if (gm_camera_work.scale_now > gm_camera_work.scale_target) {
			// 目標値に到達
			gm_camera_work.scale_now = gm_camera_work.scale_target;
		}
	} else	if (gm_camera_work.scale_now > gm_camera_work.scale_target) {
		// 縮小
		gm_camera_work.scale_now -= gm_camera_work.scale_spd;
		if (gm_camera_work.scale_now < gm_camera_work.scale_target) {
			// 目標値に到達
			gm_camera_work.scale_now = gm_camera_work.scale_target;
		}
	}

	obj_camera->scale = GMD_CAMERA_SCALE / gm_camera_work.scale_now;
}
// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
