// =======================================================================
/*!
  @file	gmBoss4.h
  @brief ボス4 Capsule

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Chibi.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_4_CHIBI_H_
#define GM_BOSS_4_CHIBI_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/
#include "gmBoss4Effect.h"
#include "gmBoss4Util.h"

/*------ Macros --------------------------------------------------------*/
#define	GME_BOSS4_LIFE_H					GMM_BOSS4_STAGE( 3, 4 )				//! 第2形態でライフがこれ以上の場合、残りライフ3の攻撃を行う( B004ゾーン4ボス.doc )
#define	GME_BOSS4_LIFE_L					GMM_BOSS4_STAGE( 1, 2 )				//! 第2形態でライフがこれ以下の場合、残りライフ1の攻撃を行う( B004ゾーン4ボス.doc )

//############ ノード番号 #####################################################
#define GMD_BOSS4_CHIBI_NODE_IDX_BASE	(0)	//(2)		//!

#define GMD_BOSS4_CHIBI_NODE_SNM_NUM			(1)		//!< SNM登録ノード数

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ボス4 カプセルアクションID列挙型
typedef enum
{
	GME_BOSS4_CHIBI_ACT_ID_NORMAL	= 0,
	
	GME_BOSS4_CHIBI_ACT_ID_MAX
} GME_BOSS4_CHIBI_ACT_ID;

//! ボス4 カプセルタイプID列挙型
typedef enum
{
	GME_BOSS4_CHIBI_TYPE_ID_NORMAL	= 0,
	GME_BOSS4_CHIBI_TYPE_ID_BOUND,
	GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD,
	GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG,
	GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON,
	
	GME_BOSS4_CHIBI_TYPE_ID_MAX
} GME_BOSS4_CHIBI_TYPE_ID;

typedef struct tag_GMS_BOSS4_CHIBI_WORK	GMS_BOSS4_CHIBI_WORK;


//! ボス4ちびエッグマンワーク
struct tag_GMS_BOSS4_CHIBI_WORK
{
	GMS_ENEMY_3D_WORK			ene_3d;
	
	GME_BOSS4_CHIBI_TYPE_ID		type;
	Uint32						flag;

	GMS_BOSS4_1SHOT_TIMER		timer;
	GME_BOSS4_CHIBI_ACT_ID		act_id;
	Sint32						cap_no;		// カプセルナンバーこれにより位置が決まる

	GMS_BOSS4_EFF_BOMB_WORK		bomb;		// 爆発用
	Sint32						wait;		// タスクウエイト用(PAL対応は自力で)

	GMS_BOSS4_FLICKER_WORK		flk_work;	//!< のこり時間点滅ワーク
	Sint32						count;		// 点滅周期用タイム

	GMS_BOSS4_DIRECTION			dir;

	Sint32						bound;		//!< バウンド中
	fx32						bnd_xspd;	//!< バウンドスピード確保用

	GMS_EFFECT_3DES_WORK*		boost;		//!< ブースター

	// ノード閲覧システム
	GMS_BOSS4_NODE_MATRIX		node_work;

	void	(*proc_update)(GMS_BOSS4_CHIBI_WORK*);	//!< 更新処理関数
};

/*------ External Declarations -----------------------------------------*/

// =======================================================================
// gmBoss4ChibiGetThrowType
/*!
	投げる方法を返す

  @param my_rect	[in]	type　(念のため攻撃タイプを入れれるようにしておく)
*/
// =======================================================================
Sint32 gmBoss4ChibiGetThrowType( GME_BOSS4_CHIBI_TYPE_ID type = GME_BOSS4_CHIBI_TYPE_ID_BOUND );

// =======================================================================
// gmBoss4ChibiGetAttackType
/*!
	攻撃タイプを返す

  @param my_rect	[in]	ライフ
*/
// =======================================================================
GME_BOSS4_CHIBI_TYPE_ID gmBoss4ChibiGetAttackType(int life);

// =======================================================================
// GmBoss4Build
/*!
  ボス4 データ構築
 */
// =======================================================================
extern void GmBoss4ChibiBuild(void);

// =======================================================================
// GmBoss4Flush
/*!
  ボス4 データ片付け
 */
// =======================================================================
extern void GmBoss4ChibiFlush(void);


// =======================================================================
// GmBoss4ChibiInit1st
/*!
  ボス4 エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4ChibiInit1st(GMS_EVE_RECORD_EVENT *eve_rec,	
									fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss4ChibiInit2nd
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4ChibiInit2nd(GMS_EVE_RECORD_EVENT *eve_rec,	
									fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss4ChibiInit2ndSpeed
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4ChibiInit2ndSpeed(GMS_EVE_RECORD_EVENT *eve_rec,	
									fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss4ChibiInit2ndBig
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4ChibiInit2ndBig(GMS_EVE_RECORD_EVENT *eve_rec,	
									fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
// GmBoss4CapsuleInitIron
/*!
  ボス１ エッグマン 初期化(第２段階 鉄球)
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4ChibiInit2ndIron(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type);


// =======================================================================
// gmBoss4ChibiInvincible
/*!
  ちびエッグマン　無敵設定
  
  @param inv	[in]	無敵時間
 */
// =======================================================================
extern void	GmBoss4ChibiSetInvincible( BOOL inv );


// =======================================================================
// GmBoss4ChibiExplosion
/*!
  全ちびエッグマンを爆破する
  
 */
// =======================================================================
void	GmBoss4ChibiExplosion();



// =======================================================================
// gmBoss4ChibiBoosterCreate
/*!
	ちびエッグマンのブースターを表示
  
 */
// =======================================================================
void gmBoss4ChibiBoosterCreate( GMS_BOSS4_CHIBI_WORK* chibi );

// =======================================================================
// gmBoss4ChibiBoosterDelete
/*!
	ちびエッグマンのブースターを消す
  
 */
// =======================================================================
void gmBoss4ChibiBoosterDelete( GMS_BOSS4_CHIBI_WORK* chibi );

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

#endif /* GM_BOSS_4_CHIBI_H_ */
