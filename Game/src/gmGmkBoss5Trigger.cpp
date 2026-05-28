// =======================================================================
/*!
  @file	gmGmkBoss5Trigger.cpp
  @brief ボスFINAL発動トリガ

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmGmkBoss5Trigger.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "gmMain.h"
#include "gmEventTbl.h"
#include "gmEnemy.h"
#include "gmBoss5.h"

#include "gmGmkBoss5Trigger.h"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ボスFINAL発動トリガギミックワーク
typedef struct tag_GMS_GMK_BOSS5_TRIGGER_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
} GMS_GMK_BOSS5_TRIGGER_WORK;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
void gmGmkBoss5TriggerMain(OBS_OBJECT_WORK *obj_work);
BOOL gmGmkBoss5TriggerTryAnnounce(void);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// GmGmkBoss5TriggerInit
/*!
  ボスFINAL 発動トリガ 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmGmkBoss5TriggerInit(GMS_EVE_RECORD_EVENT *eve_rec,
									   fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	
	// オブジェクトワーク生成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x,
										pos_y,
										sizeof(GMS_GMK_BOSS5_TRIGGER_WORK),
										"BOSS5_TRIGGER");
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	&= ~OBD_DISP_NODISP;
	obj_work->move_flag	|= (OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE);
	obj_work->move_flag	&= ~(OBD_MOVE_FALL);
	
	obj_work->ppFunc	= gmGmkBoss5TriggerMain;
	
	return obj_work;
}


/*------ Static Functions ----------------------------------------------*/
// =======================================================================
// gmGmkBoss5TriggerMain
/*!
  ボスFINAL発動トリガ メイン処理
  
  @note
  プレイヤーが自分の座標を通過したか監視します。
  通過した場合はボスFINALに通知します。
 */
// =======================================================================
void gmGmkBoss5TriggerMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_OBJECT_WORK	*ply_obj	= (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	
	if (ply_obj) {
		if (ply_obj->pos.x >= obj_work->pos.x) {
			if (gmGmkBoss5TriggerTryAnnounce()) {
				obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
			}
		}
	}
}

// =======================================================================
// gmGmkBoss5TriggerTryAnnounce
/*!
  トリガ通過通知試行
  
  @retval TRUE	通知成功
  @retval FALSE	未通知
  
  @note
  ボスFINALの管理オブジェクトをサーチして、
  見つかった管理オブジェクトに、プレイヤーがトリガを通過したことを知らせます。
 */
// =======================================================================
BOOL gmGmkBoss5TriggerTryAnnounce(void)
{
	OBS_OBJECT_WORK	*obj_work;
	
	// ボスFINALの管理オブジェクトをサーチ
	obj_work	= ObjObjectSearchRegistObject(NULL, GMD_OBJTYPE_ENEMY);
	while (obj_work) {
		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
		if (ene_com->eve_rec) {
			if (ene_com->eve_rec->id == GMD_EVENT_ID_ENE_BOSS_FINAL) {
				break;
			}
		}
	}
	
	// 見つからなかった場合はなにもしない
	if (obj_work == NULL) {
		return FALSE;
	}
	
	// トリガ通過をボスFINALに知らせる
	{
		GMS_BOSS5_MGR_WORK	*mgr_work	= (GMS_BOSS5_MGR_WORK*)obj_work;
		GmBoss5MgrAnnouncePassedTrigger(mgr_work);
	}
	
	return TRUE;
}

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
