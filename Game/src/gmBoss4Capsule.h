// =======================================================================
/*!
  @file	gmBoss4.h
  @brief ボス4 Capsule

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Capsule.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_4_CAPSULE_H_
#define GM_BOSS_4_CAPSULE_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/
#include "gmBoss4Effect.h"
#include "gmBOss4Chibi.h"
#include "gmBoss4Util.h"

/*------ Macros --------------------------------------------------------*/
#if _IPHONE
#define		GMD_BOSS4_CAP_MAX						(6)								// カプセル数
#define		GMD_BOSS4_CAP_ROTATE_SPD				GMM_BOSS4_PAL_SPEED(6.0f)		// 回転スピード
#else
#define		GMD_BOSS4_CAP_MAX						(9)								// カプセル数
#define		GMD_BOSS4_CAP_ROTATE_SPD				GMM_BOSS4_PAL_SPEED(8.5f)		// 回転スピード
#endif // _IPHONE

#define		GMD_BOSS4_CAP_ZOOM_SMALL				(65.0f)							// 最小円
#define		GMD_BOSS4_CAP_ZOOM_BIG					(100.0f)							// 最大円

#define		GMD_BOSS4_CAP_ZOOM_TIME					GMM_BOSS4_PAL_TIME(4.5f * 60)	// 間隔
#define		GMD_BOSS4_CAP_ZOOM_TIME_RAND			GMM_BOSS4_PAL_TIME(60)			// 間隔誤差

#define		GMD_BOSS4_CPA_ROTATE_X_DEG				(60.0f)							// X軸回転角度限界
#define		GMD_BOSS4_CPA_ROTATE_Z_DEG				(45.0f)							// Z軸回転角度限界
#define		GMD_BOSS4_CPA_ROTATE_X_SPD				(0.5f)							// X軸回転スピード
#define		GMD_BOSS4_CPA_ROTATE_Z_SPD				(1.0f)							// Z軸回転スピード

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ボス4 カプセルアクションID列挙型
typedef enum
{
	GME_BOSS4_CAP_ACT_ID_NORMAL	= 0,
	
	GME_BOSS4_CAP_ACT_ID_MAX
} GME_BOSS4_CAP_ACT_ID;

//! ボス4 カプセルタイプID列挙型
typedef enum
{
	GME_BOSS4_CAP_TYPE_ID_NORMAL	= 0,
	GME_BOSS4_CAP_TYPE_ID_BOUND,
	
	GME_BOSS4_CAP_TYPE_ID_MAX
} GME_BOSS4_CAP_TYPE_ID;

typedef struct tag_GMS_BOSS4_CAP_WORK	GMS_BOSS4_CAP_WORK;


//! ボス4カプセルワーク
struct tag_GMS_BOSS4_CAP_WORK
{
	GMS_ENEMY_3D_WORK			ene_3d;
	
	GME_BOSS4_CAP_TYPE_ID		type;
	Uint32						flag;

	GMS_BOSS4_1SHOT_TIMER		timer;
	GME_BOSS4_CAP_ACT_ID		egg_act_id;
	Sint32						cap_no;		// カプセルナンバーこれにより位置が決まる

	GMS_BOSS4_EFF_BOMB_WORK		bomb;		// 爆発用
	Sint32						wait;		// タスクウエイト用

	GMS_BOSS4_FLICKER_WORK		flk_work;	//!< ダメージ点滅ワーク

	GME_BOSS4_CHIBI_TYPE_ID		chibi_type;	// 第２段階でちびエッグマンを吐き出すタイプ
	
	void	(*proc_update)(GMS_BOSS4_CAP_WORK*);	//!< 更新処理関数
};

/*------ External Declarations -----------------------------------------*/

// =======================================================================
// GmBoss4Build
/*!
  ボス１ データ構築
 */
// =======================================================================
extern void GmBoss4CapsuleBuild(void);

// =======================================================================
// GmBoss4Flush
/*!
  ボス１ データ片付け
 */
// =======================================================================
extern void GmBoss4CapsuleFlush(void);


// =======================================================================
// GmBoss4CapsuleInit1st
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4CapsuleInit1st(GMS_EVE_RECORD_EVENT *eve_rec,	
									fx32 pos_x, fx32 pos_y, u8 type);


// =======================================================================
// GmBoss4CapsuleInit2nd
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4CapsuleInit2nd(GMS_EVE_RECORD_EVENT *eve_rec,	
									fx32 pos_x, fx32 pos_y, u8 type);

// =======================================================================
//	全部のカプセルを回す
/*!
  カプセル　回転

  @param inv	[in]	回転スピード
*/
// =======================================================================
extern	void GmBoss4CapsuleUpdateRol( float spd );


// =======================================================================
// gmBoss4CapsuleInvincible
/*!
  カプセル　無敵設定
  
  @param inv	[in]	無敵時間
  @param atk	[in]	当たり自体をなくすかどうか [TRUE = 当たりあり(標準)] 
 */
// =======================================================================
void	GmBoss4CapsuleSetInvincible( Sint32 inv, BOOL hit = TRUE);


// =======================================================================
// gmBoss4CapsuleGetCount
/*!
  カプセル　現在存在している数
  
 */
// =======================================================================
Sint32	GmBoss4CapsuleGetCount();


// =======================================================================
// gmBoss4CapsuleClear
/*!
  カプセル　現在存在している数を初期化(先頭で行う処理を入れる)
  
 */
// =======================================================================
void	GmBoss4CapsuleClear();


// =======================================================================
// gmBoss4CapsuleExplosion
/*!
  全カプセルを爆破する
  
 */
// =======================================================================
void	GmBoss4CapsuleExplosion();


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

#endif /* GM_BOSS_4_CAPSULE_H_ */
