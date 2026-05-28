// ==========================================================================
/*!
  @file gmGmkCamScrLim.cpp
  @brief ギミック カメラ スクロール範囲制限

  @author K.Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkCamScrLim.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ==========================================================================
/*
 * $Log: $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmEnemy.h"
#include "gmMain.h"
#include "gmTask.h"
#include "gmPlySeq.h"
#include "gmEventMgr.h"
#include "gmEventTbl.h"
#include "gmGmkCamScrLim.h"

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
struct tag_GMS_GMK_CAM_SCR_LIMIT_WORK;

/// 領域設定ワーク
typedef struct tag_GMS_GMK_CAM_SCR_LIMIT_SETTING {
	u32		flag;
	s32		limit_rect[MTD_RECT];								// 制限矩形
	void	(*func)(struct tag_GMS_GMK_CAM_SCR_LIMIT_WORK*);	// 実行中関数
//	s8		player_target;										// ターゲットプレイヤー
} GMS_GMK_CAM_SCR_LIMIT_SETTING;

/// カメラ スクロール範囲制限 ワーク
typedef struct tag_GMS_GMK_CAM_SCR_LIMIT_WORK {
	GMS_ENEMY_COM_WORK				gmk_work;

	GMS_GMK_CAM_SCR_LIMIT_SETTING	limit_setting;

} GMS_GMK_CAM_SCR_LIMIT_WORK;

/*
// GMS_GMK_CAM_SCR_LIMIT_SETTING : flag
#define GMD_GMK_CAM_SCR_LIMIT_FLAG_LOCK_LEFT	(0x00000001)		//!< 左 制限中
#define GMD_GMK_CAM_SCR_LIMIT_FLAG_LOCK_TOP		(0x00000002)		//!< 上 制限中
#define GMD_GMK_CAM_SCR_LIMIT_FLAG_LOCK_RIGHT	(0x00000004)		//!< 右 制限中
#define GMD_GMK_CAM_SCR_LIMIT_FLAG_LOCK_BOTTOM	(0x00000008)		//!< 下 制限中

// GMS_EVE_RECORD_EVENT : flag
#define GMD_GMK_CAM_SCR_LIMIT_EVE_FLAG_LOCK_LEFT		(0x0001)	//!< 左端を制限
#define GMD_GMK_CAM_SCR_LIMIT_EVE_FLAG_LOCK_TOP			(0x0002)	//!< 上端を制限
#define GMD_GMK_CAM_SCR_LIMIT_EVE_FLAG_LOCK_RIGHT		(0x0004)	//!< 右端を制限
#define GMD_GMK_CAM_SCR_LIMIT_EVE_FLAG_LOCK_BOTTOM		(0x0008)	//!< 下端を制限
#define GMD_GMK_CAM_SCR_LIMIT_EVE_FLAG_SCR_A			(0x0010)	//!< プレイヤーA面時有効
#define GMD_GMK_CAM_SCR_LIMIT_EVE_FLAG_SCR_B			(0x0020)	//!< プレイヤーB面時有効
*/


//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkCamScrLimitMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkCamScrLimitReleaseMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkCamScrLimitSetting(OBS_OBJECT_WORK *obj_work);
static void gmGmkCamScrLimitRelease(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkCamScrLimitRelease
/*!
 *	ギミック カメラ スクロール範囲制限 解除 初期化関数
 *	（マップイベント以外からの呼び出し用）
 *
 *	@param	flag	[in]	スクロール制限解除方向
 *
 *	@note
 *		flag	: スクロール制限を解除したい方向をflagにセット \n
 *			左	:	GMD_GMK_SCR_LMT_RELEASE_LEFT	 \n
 *			上	:	GMD_GMK_SCR_LMT_RELEASE_TOP		 \n
 *			右	:	GMD_GMK_SCR_LMT_RELEASE_RIGHT	 \n
 *			下	:	GMD_GMK_SCR_LMT_RELEASE_BOTTOM	 \n
 *			全て:	GMD_GMK_SCR_LMT_RELEASE_ALL
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkCamScrLimitRelease(u8 flag)
{
	OBS_OBJECT_WORK	*obj_work;
	obj_work = GmEventMgrLocalEventBirth(
				GMD_EVENT_ID_NOSET_SCR_LIMIT_REL,	// u16 id, 
				0,									// fx32 pos_x, 
				0,									// fx32 pos_y, 
				(u16)(flag & GMD_GMK_SCR_LMT_RELEASE_ALL),// u16 flag, 
				0,									// s8 left, 
				0,									// s8 top, 
				0,									// u8 width, 
				0,									// u8 height, 
				0);									// u8 type);
//	obj_work->user_flag = (u32)(flag & GMD_GMK_SCR_LMT_RELEASE_ALL);
	return (obj_work);

}

// ==========================================================================
// GmGmkCamScrLimitSet
/*!
 *	ギミック カメラ スクロール範囲制限 初期化関数
 *	（マップイベント以外からの呼び出し用）
 *
 *	@param	eve_rec	[inout]	レコードポインタ
 *	@param	pos_x	[in]	出現座標
 *	@param	pos_y	[in]
 *
 *	@note
 *		eve_rec に flag, left, top, width, heigh
 *		pos_x , pos_y それぞれに必用な情報をセットしてコールしてください。
 *
 *		pos_x/y(画面中心) から left, top, width, height で
 *		セットした距離でスクロールを制限します。
 *
 *		※ツールの数値入力制約都合から制限距離(left, top, width, height)
 *		　は値が２倍されて使用されることに注意。
 *
 *		flag	: スクロール制限を設置したい方向をflagにセット \n
 *			左	:	GMD_GMK_SCR_LMT_RELEASE_LEFT	 \n
 *			上	:	GMD_GMK_SCR_LMT_RELEASE_TOP		 \n
 *			右	:	GMD_GMK_SCR_LMT_RELEASE_RIGHT	 \n
 *			下	:	GMD_GMK_SCR_LMT_RELEASE_BOTTOM	 \n
 *			全て:	GMD_GMK_SCR_LMT_RELEASE_ALL
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkCamScrLimitSet(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y)
{
	OBS_OBJECT_WORK	*obj_work;
	obj_work = GmEventMgrLocalEventBirth(
				GMD_EVENT_ID_NOSET_SCR_LIMIT_SET,	// u16 id, 
				pos_x,								// fx32 pos_x, 
				pos_y,								// fx32 pos_y, 
				eve_rec->flag,						// u16 flag, 
				eve_rec->left,						// s8 left, 
				eve_rec->top,						// s8 top, 
				eve_rec->width,						// u8 width, 
				eve_rec->height,					// u8 height, 
				0);									// u8 type);
	return (obj_work);
}



// ==========================================================================
// GmGmkCamScrLimitInit
/*!
 *	ギミック カメラ スクロール範囲制限設置 初期化関数
 *
 *	@param	eve_rec	[inout]	レコードポインタ
 *	@param	pos_x	[in]	出現座標
 *	@param	pos_y	[in]
 *	@param	type	[in]	処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkCamScrLimitInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_COM_WORK			*gmk_work;
	OBS_OBJECT_WORK				*obj_work;
	GMS_GMK_CAM_SCR_LIMIT_WORK	*limit_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_CAM_SCR_LIMIT_WORK), "GMK_CAM_SCRLMT");
	limit_work = (GMS_GMK_CAM_SCR_LIMIT_WORK*)obj_work;
	gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;

	// 再生成しない
	gmk_work->enemy_flag |= GMD_ENEMY_FLAG_DIE;
	obj_work->user_flag = eve_rec->flag;
	
	// 個別設定
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_DISP_NODISP;
	obj_work->flag |= OBD_OBJECT_NOCLIP;						// 画面外でも生存

	/*** 範囲制限設定 ***/
	{
		GMS_GMK_CAM_SCR_LIMIT_SETTING	*limit_setting	= &limit_work->limit_setting;

		// 制限範囲設定
		limit_setting->limit_rect[MTD_LEFT] 	= (obj_work->pos.x >> FX32_SHIFT) + (s32)(eve_rec->left) * 2;
		limit_setting->limit_rect[MTD_RIGHT]	= (obj_work->pos.x >> FX32_SHIFT) + (s32)(eve_rec->left) * 2 + (s32)(eve_rec->width) * 2;
		limit_setting->limit_rect[MTD_TOP]		= (obj_work->pos.y >> FX32_SHIFT) + (s32)(eve_rec->top) * 2;
		limit_setting->limit_rect[MTD_BOTTOM]	= (obj_work->pos.y >> FX32_SHIFT) + (s32)(eve_rec->top) * 2 + (s32)(eve_rec->height) * 2;
	}

	/*** メイン処理設定 ***/
	if (eve_rec->id == GMD_EVENT_ID_NOSET_SCR_LIMIT_SET) {
		// プログラム呼び出しの場合は即時発動
		obj_work->ppFunc = gmGmkCamScrLimitSetting;
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_SCR_LIMIT_BUSY;			// スクロール制限制御開始
	} else {
		// セット配置の場合はソニックがギミックより右側に来たら発動
		obj_work->ppFunc = gmGmkCamScrLimitMain;
	}
	return (obj_work);
}


// ==========================================================================
// GmGmkCamScrLimitReleaseInit
/*!
 *	ギミック カメラ スクロール範囲制限 解除 初期化関数
 *
 *	@param	eve_rec	[inout]	レコードポインタ
 *	@param	pos_x	[in]	出現座標
 *	@param	pos_y	[in]
 *	@param	type	[in]	処理内容タイプ 通常は0
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkCamScrLimitReleaseInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_COM_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_COM_WORK), "GMK_SCRLMT_RELEASE");
//	obj_work = GMM_ENEMY_CREATE_WORK(NULL/*eve_rec*/, NULL/*pos_x*/, NULL/*pos_y*/,
//										sizeof(GMS_ENEMY_COM_WORK), "GMK_SCRLMT_RELEASE");

	gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;

	obj_work->user_flag = eve_rec->flag;
	obj_work->user_timer = 0;									// スクロール解除経過時間

	// 個別設定
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_DISP_NODISP;
	obj_work->flag |= OBD_OBJECT_NOCLIP;						// 画面外でも生存

	/*** メイン処理設定 ***/
	if (eve_rec->id == GMD_EVENT_ID_NOSET_SCR_LIMIT_REL) {
		// プログラム呼び出しの場合は即時発動
		obj_work->ppFunc = gmGmkCamScrLimitRelease;
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_SCR_LIMIT_BUSY;			// スクロール制限制御開始
	} else {
		// セット配置の場合はソニックがギミックより右側に来たら発動
		obj_work->ppFunc = gmGmkCamScrLimitReleaseMain;
	}
	obj_work->user_work = 3;	// 制限値変更速度(widh/height共用)

	return (obj_work);
}


// ==========================================================================
// GmCamScrLimitSetDirect
/*!
 * スクロール制限即時セット
 *	@param	eve_rec	[inout]	レコードポインタ
 *	@param	pos_x	[in]	出現座標
 *	@param	pos_y	[in]
 *
 *	@note
 *		eve_rec に flag, left, top, width, heigh
 *		pos_x , pos_y それぞれに必用な情報をセットしてコールしてください。
 *
 *		pos_x/y(画面中心) から left, top, width, height で
 *		セットした距離でスクロールを制限します。
 *
 *		※ツールの数値入力制約都合から制限距離(left, top, width, height)
 *		　は値が２倍されて使用されることに注意。
 *
 *		flag	: スクロール制限を設置したい方向をflagにセット \n
 *			左	:	GMD_GMK_SCR_LMT_RELEASE_LEFT	 \n
 *			上	:	GMD_GMK_SCR_LMT_RELEASE_TOP		 \n
 *			右	:	GMD_GMK_SCR_LMT_RELEASE_RIGHT	 \n
 *			下	:	GMD_GMK_SCR_LMT_RELEASE_BOTTOM	 \n
 *			全て:	GMD_GMK_SCR_LMT_RELEASE_ALL
 */
// ==========================================================================
void GmCamScrLimitSetDirect(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y)
{
	if (eve_rec->flag & GMD_GMK_SCR_LMT_RELEASE_LEFT) {
		g_gm_main_system.map_fcol.left 	= (pos_x >> FX32_SHIFT) + (s32)(eve_rec->left) * 2;
	}
	if (eve_rec->flag & GMD_GMK_SCR_LMT_RELEASE_RIGHT) {
		g_gm_main_system.map_fcol.right	= (pos_x >> FX32_SHIFT) + (s32)(eve_rec->left) * 2 + (s32)(eve_rec->width) * 2;
	}
	if (eve_rec->flag & GMD_GMK_SCR_LMT_RELEASE_TOP) {
		g_gm_main_system.map_fcol.top	= (pos_y >> FX32_SHIFT) + (s32)(eve_rec->top) * 2;
	}
	if (eve_rec->flag & GMD_GMK_SCR_LMT_RELEASE_BOTTOM) {
		g_gm_main_system.map_fcol.bottom = (pos_y >> FX32_SHIFT) + (s32)(eve_rec->top) * 2 + (s32)(eve_rec->height) * 2;
	}
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkCamScrLimitMain
/*!
 *	ギミック スクロール制限 通常メイン処理
 *
 *	@param	obj_work	[in]	エネミーワーク
 */
// ==========================================================================
void gmGmkCamScrLimitMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work	= g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	
	if (obj_work->pos.x <= ply_work->obj_work.pos.x) {
		obj_work->ppFunc = gmGmkCamScrLimitSetting;
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_SCR_LIMIT_BUSY;			// スクロール制限制御開始
	}
}
// ==========================================================================
// gmGmkCamScrLimitReleaseMain
/*!
 *	ギミック スクロール制限解除 通常メイン処理
 *
 *	@param	obj_work	[in]	エネミーワーク
 */
// ==========================================================================
void gmGmkCamScrLimitReleaseMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK	*ply_work	= g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	
	if (obj_work->pos.x <= ply_work->obj_work.pos.x) {
		obj_work->ppFunc = gmGmkCamScrLimitRelease;
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_SCR_LIMIT_BUSY;			// スクロール制限制御開始
	}
}
// ==========================================================================
// gmGmkCamScrLimitSetting
/*!
 *	ギミック スクロール制限 通常メイン処理
 *
 *	@param	obj_work	[in]	エネミーワーク
 */
// ==========================================================================
void gmGmkCamScrLimitSetting(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_COM_WORK			*gmk_work	= (GMS_ENEMY_COM_WORK*)obj_work;
//	GMS_PLAYER_WORK				*ply_work	= g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	GMS_GMK_CAM_SCR_LIMIT_WORK	*limit_work	= (GMS_GMK_CAM_SCR_LIMIT_WORK*)gmk_work;
	OBS_CAMERA					*obj_camera = ObjCameraGet(0);
	s32	cam_pos_x = FXM_FLOAT_TO_FX32(obj_camera->pos.x) >> FX32_SHIFT;
	s32	cam_pos_y = -FXM_FLOAT_TO_FX32(obj_camera->pos.y) >> FX32_SHIFT;
	s32 spd_w = 1;	// 制限値変更速度 width
	s32 spd_h = 1;	// 制限値変更速度 height
#if 1
	s32	lcd_width = FXM_FLOAT_TO_FX32((float)(AMD_SCREEN_2D_WIDTH/2) * obj_camera->scale) >> FX32_SHIFT;
	s32	lcd_height= FXM_FLOAT_TO_FX32((float)(AMD_SCREEN_2D_HEIGHT/2) * obj_camera->scale) >> FX32_SHIFT;
#else
	s32	lcd_width = FXM_FLOAT_TO_FX32((float)(OBD_LCD_X/2) * obj_camera->scale) >> FX32_SHIFT;
	s32	lcd_height= FXM_FLOAT_TO_FX32((float)(OBD_LCD_Y/2) * obj_camera->scale) >> FX32_SHIFT;
#endif
	u8	flag = TRUE;

	if (obj_work->user_flag & GMD_GMK_SCR_LMT_EVE_FLAG_LEFT) {
		// 左端リミット制限
		if (g_gm_main_system.map_fcol.left  != limit_work->limit_setting.limit_rect[MTD_LEFT]) {
			// 左端が制限値と一致していない
			s32 scr_left = cam_pos_x - lcd_width;
			if (scr_left > limit_work->limit_setting.limit_rect[MTD_LEFT]) {
				// 制限値が画面外なら即時リミット反映
				g_gm_main_system.map_fcol.left   = limit_work->limit_setting.limit_rect[MTD_LEFT];
			} else {
				// 制限値が画面内なら調整
				if (scr_left > g_gm_main_system.map_fcol.left) {
					// 現在のリミット左端が画面外なら、カメラ左端即時反映＋リミット位置に近づける
					g_gm_main_system.map_fcol.left   = scr_left + spd_w;

				} else {
					// 現在のリミット左端が画面内なら、リミット位置に近づける
					g_gm_main_system.map_fcol.left += spd_w;
				}
				if (g_gm_main_system.map_fcol.left > limit_work->limit_setting.limit_rect[MTD_LEFT]) {
					// リミット左端が制限値を超えてしまったら補正
					g_gm_main_system.map_fcol.left = limit_work->limit_setting.limit_rect[MTD_LEFT];
				}
			}
			flag = FALSE;
		}
	}
	if (obj_work->user_flag & GMD_GMK_SCR_LMT_EVE_FLAG_RIGHT) {
		// 右端リミット制限
		if (g_gm_main_system.map_fcol.right  != limit_work->limit_setting.limit_rect[MTD_RIGHT]) {
			// 右端が制限値と一致していない
			s32 scr_right = cam_pos_x + lcd_width;
			if (scr_right < limit_work->limit_setting.limit_rect[MTD_RIGHT]) {
				// 制限値が画面外なら即時リミット反映
				g_gm_main_system.map_fcol.right   = limit_work->limit_setting.limit_rect[MTD_RIGHT];
			} else {
				// 制限値が画面内なら調整
				if (scr_right < g_gm_main_system.map_fcol.right) {
					// 現在のリミット右端が画面外なら、カメラ右端即時反映＋リミット位置に近づける
					g_gm_main_system.map_fcol.right   = scr_right - spd_w ;

				} else {
					// 現在のリミット右端が画面内なら、リミット位置に近づける
					g_gm_main_system.map_fcol.right -= spd_w;
				}
				if (g_gm_main_system.map_fcol.right < limit_work->limit_setting.limit_rect[MTD_RIGHT]) {
					// リミット右端が制限値を超えてしまったら補正
					g_gm_main_system.map_fcol.right = limit_work->limit_setting.limit_rect[MTD_RIGHT];
				}
			}
			flag = FALSE;
		}
	}
	if (obj_work->user_flag & GMD_GMK_SCR_LMT_EVE_FLAG_TOP) {
		// 上端リミット制限
		if (g_gm_main_system.map_fcol.top  != limit_work->limit_setting.limit_rect[MTD_TOP]) {
			// 上端が制限値と一致していない
			s32 scr_top = cam_pos_y - lcd_height;
			if (scr_top > limit_work->limit_setting.limit_rect[MTD_TOP]) {
				// 制限値が画面外なら即時リミット反映
				g_gm_main_system.map_fcol.top   = limit_work->limit_setting.limit_rect[MTD_TOP];
			} else {
				// 制限値が画面内なら調整
				if (scr_top > g_gm_main_system.map_fcol.top) {
					// 現在のリミット上端が画面外なら、カメラ上端即時反映＋リミット位置に近づける
					g_gm_main_system.map_fcol.top   = scr_top + spd_h;

				} else {
					// 現在のリミット右端が画面内なら、リミット位置に近づける
					g_gm_main_system.map_fcol.top += spd_h;
				}
				if (g_gm_main_system.map_fcol.top > limit_work->limit_setting.limit_rect[MTD_TOP]) {
					// リミット上端が制限値を超えてしまったら補正
					g_gm_main_system.map_fcol.top = limit_work->limit_setting.limit_rect[MTD_TOP];
				}
			}
			flag = FALSE;
		}
	}
	if (obj_work->user_flag & GMD_GMK_SCR_LMT_EVE_FLAG_BOTTOM) {
		// 下端リミット制限
		if (g_gm_main_system.map_fcol.bottom != limit_work->limit_setting.limit_rect[MTD_BOTTOM]) {
			// 下端が制限値と一致していない
			s32 scr_bottom = cam_pos_y + lcd_height;
			if (scr_bottom < limit_work->limit_setting.limit_rect[MTD_BOTTOM]) {
				// 制限値が画面外なら即時リミット反映
				g_gm_main_system.map_fcol.bottom = limit_work->limit_setting.limit_rect[MTD_BOTTOM];
			} else {
				// 制限値が画面内なら調整
				if (scr_bottom < g_gm_main_system.map_fcol.bottom) {
					// 現在のリミット下端が画面外なら、カメラ下端即時反映＋リミット位置に近づける
					g_gm_main_system.map_fcol.bottom = scr_bottom - spd_h;

				} else {
					// 現在のリミット下端が画面内なら、リミット位置に近づける
					g_gm_main_system.map_fcol.bottom -= spd_h;
				}
				if (g_gm_main_system.map_fcol.bottom < limit_work->limit_setting.limit_rect[MTD_BOTTOM]) {
					// リミット下端が制限値を超えてしまったら補正
					g_gm_main_system.map_fcol.bottom = limit_work->limit_setting.limit_rect[MTD_BOTTOM];
				}
			}
			flag = FALSE;
		}
	}

	if (flag) {
		// 全てのセット条件を満たすとタスク消去
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_SCR_LIMIT_BUSY;		// スクロール制限制御終了
	}
}

// ==========================================================================
// gmGmkCamScrLimitRelease
/*!
 *	ギミック スクロール制限 解除 通常メイン処理
 *
 *	@param	obj_work	[in]	エネミーワーク
 */
// ==========================================================================
void gmGmkCamScrLimitRelease(OBS_OBJECT_WORK *obj_work)
{
//	GMS_ENEMY_COM_WORK		*gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;
	OBS_CAMERA				*obj_camera = ObjCameraGet(0);
	s32	cam_pos_x = FXM_FLOAT_TO_FX32(obj_camera->pos.x) >> FX32_SHIFT;
	s32	cam_pos_y = -FXM_FLOAT_TO_FX32(obj_camera->pos.y) >> FX32_SHIFT;
	s32	lcd_width = FXM_FLOAT_TO_FX32((float)(AMD_SCREEN_2D_WIDTH/2) * obj_camera->scale) >> FX32_SHIFT;
	s32	lcd_height= FXM_FLOAT_TO_FX32((float)(AMD_SCREEN_2D_HEIGHT/2) * obj_camera->scale) >> FX32_SHIFT;
	s32 spd_w = (s32)obj_work->user_work;//3;	// 制限値変更速度 width
	s32 spd_h = (s32)obj_work->user_work;//3;	// 制限値変更速度 height
	u8	flag = TRUE;
	
	if (obj_work->user_flag & GMD_GMK_SCR_LMT_RELEASE_LEFT) {
		if (g_gm_main_system.map_fcol.left) {
			if (cam_pos_x - lcd_width > g_gm_main_system.map_fcol.left) {
				g_gm_main_system.map_fcol.left = 0;
			} else {
				g_gm_main_system.map_fcol.left -= spd_w;
				if (g_gm_main_system.map_fcol.left < 0) {
					g_gm_main_system.map_fcol.left = 0;
				}
				flag = FALSE;
			}
		}
	}

	if (obj_work->user_flag & GMD_GMK_SCR_LMT_RELEASE_RIGHT) {
		if (g_gm_main_system.map_fcol.right < g_gm_main_system.map_fcol.map_block_num_x*64) {
			if (cam_pos_x + lcd_width < g_gm_main_system.map_fcol.right) {
				g_gm_main_system.map_fcol.right = g_gm_main_system.map_fcol.map_block_num_x*64;
			} else {
				g_gm_main_system.map_fcol.right += spd_w;
				if (g_gm_main_system.map_fcol.right > g_gm_main_system.map_fcol.map_block_num_x*64) {
					g_gm_main_system.map_fcol.right = g_gm_main_system.map_fcol.map_block_num_x*64;
				}
				flag = FALSE;
			}
		}
	}

	if (obj_work->user_flag & GMD_GMK_SCR_LMT_RELEASE_TOP) {
		if (g_gm_main_system.map_fcol.top) {
			if (cam_pos_y - lcd_height > g_gm_main_system.map_fcol.top) {
				g_gm_main_system.map_fcol.top = 0;
			} else {
				g_gm_main_system.map_fcol.top -= spd_h;
				if (g_gm_main_system.map_fcol.top < 0) {
					g_gm_main_system.map_fcol.top = 0;
				}
				flag = FALSE;
			}
		}
	}

	if (obj_work->user_flag & GMD_GMK_SCR_LMT_RELEASE_BOTTOM) {
		if (g_gm_main_system.map_fcol.bottom < g_gm_main_system.map_fcol.map_block_num_y*64) {
			if (cam_pos_y + lcd_height < g_gm_main_system.map_fcol.bottom) {
				g_gm_main_system.map_fcol.bottom = g_gm_main_system.map_fcol.map_block_num_y*64;
			} else {
				g_gm_main_system.map_fcol.bottom += spd_h;
				if (g_gm_main_system.map_fcol.bottom > g_gm_main_system.map_fcol.map_block_num_y*64) {
					g_gm_main_system.map_fcol.bottom = g_gm_main_system.map_fcol.map_block_num_y*64;
				}
				flag = FALSE;
			}
		}
	}

	if (  (flag)							// 全方向の解除が完了
		&&(obj_work->user_timer) ) {		// 解除開始後１フレーム以上経過しないと解除しない＠BOSS2 シャッター対策
		
		// 全ての解除条件を満たすとタスク消去
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_SCR_LIMIT_BUSY;		// スクロール制限制御終了
	}
	obj_work->user_timer++;													// 解除時間加算
}
// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
