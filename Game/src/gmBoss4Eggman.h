// =======================================================================
/*!
  @file	gmBoss4.h
  @brief ボス1

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Eggman.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_4_EGGMAN_H_
#define GM_BOSS_4_EGGMAN_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/
//############ ノード番号 #####################################################
#define GMD_BOSS4_EGG_NODE_IDX_HAND_RIGHT	(9)	//(2)		//!< 右手
#define GMD_BOSS4_EGG_NODE_IDX_HAND_LEFT	(6)	//(2)		//!< 左手

#define GMD_BOSS4_EGG_NODE_SNM_NUM			(4)		//!< SNM登録ノード数

//############ エッグマン #####################################################
/* フラグ */
#define GMD_BOSS4_EGG_FLAG_INDP_ACT_SET		(1 << 0)	//!< 独立アクション設定中フラグ
#define GMD_BOSS4_EGG_FLAG_SWEAT_ACTIVE		(1 << 1)	//!< 汗エフェクト有効中フラグ

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ボス1 エッグマン独立アクションID列挙型
//! これを使用するときはgm_boss4_egg_act_id_tbl[]にテーブルを追加してください
typedef enum
{
	GME_BOSS4_EGG_ACT_ID_LAUGH_L	= 0,
	GME_BOSS4_EGG_ACT_ID_LAUGH_R,
	GME_BOSS4_EGG_ACT_ID_DAMAGE,
	GME_BOSS4_EGG_ACT_ID_THROW,
	GME_BOSS4_EGG_ACT_ID_THROW_LEFT,
	
	GME_BOSS4_EGG_ACT_ID_MAX
} GME_BOSS4_EGG_ACT_ID;

typedef struct tag_GMS_BOSS4_EGG_WORK	GMS_BOSS4_EGG_WORK;


//! ボス１エッグマンワーク
struct tag_GMS_BOSS4_EGG_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	Uint32					flag;
	
	GME_BOSS4_EGG_ACT_ID	egg_act_id;

	// 反転しなくてはならないため
	GMS_BOSS4_DIRECTION		dir_work;

	// ノード閲覧システム
	GMS_BOSS4_NODE_MATRIX	node_work;

	void	(*proc_update)(GMS_BOSS4_EGG_WORK*);	//!< 更新処理関数
};

/*------ External Declarations -----------------------------------------*/

// =======================================================================
// GmBoss4Build
/*!
  ボス１ データ構築
 */
// =======================================================================
extern void GmBoss4EggmanBuild(void);

// =======================================================================
// GmBoss4Flush
/*!
  ボス１ データ片付け
 */
// =======================================================================
extern void GmBoss4EggmanFlush(void);


// =======================================================================
// GmBoss4EggInit
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4EggInit(GMS_EVE_RECORD_EVENT *eve_rec,
									   fx32 pos_x, fx32 pos_y, u8 type);


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

#endif /* GM_BOSS_4_EGGMAN_H_ */
