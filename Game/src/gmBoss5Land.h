// =======================================================================
/*!
  @file	gmBoss5Land.h
  @brief ボスファイナル 足場

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5Land.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS5_LAND_H_
#define GM_BOSS5_LAND_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/
#include "gmEffect.h"
#include "gmEnemy.h"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ボス５足場構成パーツワーク（エフェクト扱い）
typedef struct tag_GMS_BOSS5_LDPART_WORK
{
	GMS_EFFECT_3DNN_WORK	efct_3d;
	
	OBS_COLLISION_WORK		col_work;
	
	void (*proc_update)(struct tag_GMS_BOSS5_LDPART_WORK*);
	
	Sint32	vib_cnt;		//! 振動制御カウンタ
	fx32	vib_ofst[MTD_XY];			//!< 振動オフセット
	fx32	pivot_parent_ofst[MTD_XY];	//!< 基準親オフセット
	
	NNS_QUATERNION	rot_diff_quat;	//!< フレーム毎の差分回転クォータニオン
	NNS_QUATERNION	cur_rot_quat;		//!< 現在の回転姿勢
	
	Sint32	part_index;	//!< 構成パーツ番号
	Uint32	wait_timer;
	Uint32	brk_glass_cnt;	//!< 割れガラスエフェクト生成カウント
} GMS_BOSS5_LDPART_WORK;

//! ボス５足場ワーク（ギミック扱い）
typedef struct tag_GMS_BOSS5_LAND_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	void (*proc_update)(struct tag_GMS_BOSS5_LAND_WORK*);
	
	GMS_BOSS5_MGR_WORK	*mgr_work;
	
	Uint32		flag;
	Uint32		wait_timer;
} GMS_BOSS5_LAND_WORK;


/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GmBoss5LandInit
/*!
  ボスFINAL 足場初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
  
  @note
  ローカル生成イベントです。
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss5LandInit(GMS_EVE_RECORD_EVENT *eve_rec,
										fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss5LandCreate
/*!
  ボスFINAL足場生成
  
  @param mgr_work	[io]	管理ワーク
  
  @return 足場ワーク(GMS_BOSS5_LAND_WORK)
  
  @note
  足場を生成する際はこの関数を呼び出してください。
 */
// =======================================================================
extern GMS_BOSS5_LAND_WORK* GmBoss5LandCreate(GMS_BOSS5_MGR_WORK *mgr_work);


// =======================================================================
// GmBoss5LandSetLight
/*!
  ボスFINAL足場用ライト設定
 */
// =======================================================================
extern void GmBoss5LandSetLight(void);


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

#endif /* GM_BOSS5_LAND_H_ */
