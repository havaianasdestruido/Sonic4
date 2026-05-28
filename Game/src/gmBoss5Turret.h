// =======================================================================
/*!
  @file	gmBoss5Turret.h
  @brief ボスファイナル 砲塔

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss5Turret.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_5_TURRET_H_
#define GM_BOSS_5_TURRET_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/
/* 定義値 */
#define GMD_BOSS5_TURRET_START_LIFE_THRESHOLD	(8)		//!< ライフ値がこの値以下になったら砲塔を開始する

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! 砲塔スライドタイプ列挙型
typedef enum
{
	GME_BOSS5_TURRET_SLIDE_TYPE_RAISE	= 0,
	GME_BOSS5_TURRET_SLIDE_TYPE_LOWER,
	
	GME_BOSS5_TURRET_SLIDE_TYPE_MAX
} GME_BOSS5_TURRET_SLIDE_TYPE;

//! カバースライドタイプ列挙型
typedef enum
{
	GME_BOSS5_TURRET_COVER_SLIDE_TYPE_OPEN	= 0,
	GME_BOSS5_TURRET_COVER_SLIDE_TYPE_CLOSE,
	
	GME_BOSS5_TURRET_COVER_SLIDE_TYPE_MAX
} GME_BOSS5_TURRET_COVER_SLIDE_TYPE;


//! ボス５砲塔ワーク
typedef struct tag_GMS_BOSS5_TURRET_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	void (*proc_update)(struct tag_GMS_BOSS5_TURRET_WORK*);
	
	Uint32			flag;
	
	Uint32			wait_timer;		//!< 汎用待機タイマ
	
	Angle32			fire_dir_z;		//!< 発射方向（マップ座標系・Z軸周り角度）
	AMS_QUAT		disp_quat;		//!< 表示回転
	
	GME_BOSS5_TURRET_SLIDE_TYPE trt_slide_type;	//!< 砲塔のスライドタイプ
	Float			trt_slide_length;	//!< 頭内部からスライドする長さ（=0.fで完全に収納された状態）
	GME_BOSS5_TURRET_COVER_SLIDE_TYPE cvr_slide_type;	//!< カバーのスライドタイプ
	Float			cvr_slide_ratio;	//!< スライドの進捗度合い（=0.fで完全に閉じた状態、=1.fで開いた状態）
	
	Sint32			vul_shot_remain;	//!< バルカン連射残り数
	Sint32			vul_burst_timer;	//!< バルカン連射時 個々の弾の発射間隔タイマ
	Sint32			vul_shot_angle;		//!< 発射方向
	VecFx32			vul_fire_pos;		//!< 発射エフェクト生成位置
	VecFx32			vul_bullet_pos;		//!< 弾エフェクト生成位置
} GMS_BOSS5_TURRET_WORK;

/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GmBoss5TurretInit
/*!
  ボスFINAL 砲塔初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss5TurretInit(GMS_EVE_RECORD_EVENT *eve_rec,
										  fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss5TurretStartUp
/*!
  砲塔起動
  
  @param body_work	[io]	本体ワーク
  
  @return 砲塔ワーク
 */
// =======================================================================
extern GMS_BOSS5_TURRET_WORK* GmBoss5TurretStartUp(GMS_BOSS5_BODY_WORK *body_work);


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

#endif /* GM_BOSS_5_TURRET_H_ */
