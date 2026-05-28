// =======================================================================
/*!
  @file	gmGmkBoss5LandPlace.cpp
  @brief ボスFINAL足場配置基準

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmGmkBoss5LandPlace.cpp 2 2011-04-11 05:21:26Z thamada $
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

#include "gmGmkBoss5LandPlace.h"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! 足場配置基準ギミックワーク
typedef struct tag_GMS_GMK_BOSS5_LAND_PLACE_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
} GMS_GMK_BOSS5_LAND_PLACE_WORK;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmGmkBoss5LandPlaceInit
/*!
  ボスFINAL 足場配置基準 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmGmkBoss5LandPlaceInit(GMS_EVE_RECORD_EVENT *eve_rec,
										 fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	
	// オブジェクトワーク生成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x,
										pos_y,
										sizeof(GMS_GMK_BOSS5_LAND_PLACE_WORK),
										"BOSS5_LAND_PLACE");
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	&= ~OBD_DISP_NODISP;
	obj_work->move_flag	|= (OBD_MOVE_NOCOL | OBD_MOVE_NOMOVE);
	obj_work->move_flag	&= ~(OBD_MOVE_FALL);
	
	return obj_work;
}


/*------ Static Functions ----------------------------------------------*/
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
