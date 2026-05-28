// ==========================================================================
/*!
  @file gmGmkStart.cpp
  @brief スタート位置設定ギミック

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGmkStart.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date::						   $
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmEnemy.h"
#include "gmMain.h"
#include "gmTask.h"
#include "gmCamera.h"

#include "gmGmkStart.h"

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ================================================================
// GmGmkFlagChangeInit
/*!
  フラグ変更系ギミック初期化関数

  @param eve_rec	[io] レコードポインタ
  @param pos_x		[in] 出現座標
  @param pos_y		[in] 
  @param type		[in] 処理内容タイプ 通常は0
 */
// ================================================================
OBS_OBJECT_WORK* GmGmkStartInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_CAMERA	*obj_camera;

	UNREFERENCED_PARAMETER(type);

	// リスタート, スタートデモ無しチェック
	if (!(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_RESTART)) {	// プレイデモの時も設定しない◆
		// プレイヤー開始位置設定
		g_gm_main_system.resume_pos_x = pos_x;
		g_gm_main_system.resume_pos_y = pos_y - 1*FX32_ONE;
	}
#if defined (HOG_PRESENT_ROM_IPHONE)
	// next act
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_NEXTACT) {
		// プレイヤー開始位置設定
		g_gm_main_system.resume_pos_x = pos_x;
		g_gm_main_system.resume_pos_y = pos_y - 1*FX32_ONE;
		g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_NEXTACT;
	}
#endif // HOG_PRESENT_ROM_IPHONE
	// スキップフラグ書き込み
	eve_rec->pos_x = GMD_EVE_RECORD_CMD_SKIP;

	GmCameraPosSet(g_gm_main_system.resume_pos_x, g_gm_main_system.resume_pos_y, 0);								// カメラ位置セット

	obj_camera = ObjCameraGet(g_obj.glb_camera_id);
	// オブジェクトカメラ設定
	ObjObjectCameraSet(FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)),
						FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)));
	// クリッピングカメラ設定
	GmCameraSetClipCamera(obj_camera);

	return (NULL);
}

//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
