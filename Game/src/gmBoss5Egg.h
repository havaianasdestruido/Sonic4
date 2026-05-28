// =======================================================================
/*!
  @file	gmBoss5Egg.h
  @brief ボスファイナル エッグマン

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5Egg.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_5_EGG_H_
#define GM_BOSS_5_EGG_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/
//############ エッグマン #####################################################
/* フラグ */
#define GMD_BOSS5_EGG_FLAG_SWEAT_ACTIVE		(1 << 0)	//!< 汗エフェクト有効中フラグ（オフにするとエフェクトが消える。直接書き換え禁止）

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ボス５エッグマンワーク
typedef struct tag_GMS_BOSS5_EGG_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	void (*proc_update)(struct tag_GMS_BOSS5_EGG_WORK*);
	
	Uint32		flag;
	Uint32		wait_timer;
	
	fx32		jump_dest_pos_x;
} GMS_BOSS5_EGG_WORK;

/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GmBoss5EggInit
/*!
  ボスFINAL エッグマン初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss5EggInit(GMS_EVE_RECORD_EVENT *eve_rec,
									   fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss5EggCreate
/*!
  エッグマン生成
  
  @param body_work	[io]	本体ワーク
  @param pos_x		[in]	生成座標X
  @param pos_y		[in]	生成座標Y
  
  @return エッグマンワーク
  
  @note
  エッグマンを生成する際はこの関数を呼び出してください。
 */
// =======================================================================
extern GMS_BOSS5_EGG_WORK* GmBoss5EggCreate(GMS_BOSS5_BODY_WORK *body_work, fx32 pos_x, fx32 pos_y);

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

#endif /* GM_BOSS5_EGG_H_ */
