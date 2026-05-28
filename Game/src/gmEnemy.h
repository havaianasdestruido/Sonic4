// ================================================================
/*!
  @file gmEnemy.h
  @brief エネミーオブジェクト

  @author mana
  @author modifier Ishizaki
                Copyright(c) 2009 Dimps
  $Id: gmEnemy.h 2 2011-04-11 05:21:26Z thamada $
 */
// ================================================================
/*
 * memo
 *
 *
 */

#ifndef GM_ENEMY_H_
#define GM_ENEMY_H_

#include "objObject.h"
#include "gmEventMgr.h"
#include "gmPlayer.h"
#include "gmTask.h"


#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ----------------------------------------------------
#if defined(AMD_DEBUG)
	//#define GMD_ENEMY_TEST (1) // エネミー描画テスト
#endif //defined(AMD_DEBUG)

#if 0
// =============================================================================
// ネット対応
// =============================================================================
/// 敵情報パケット 構造体
typedef struct tag_GMS_ENEMY_PACKET {
	u8	state;			//!< 状態
	u8	player;		//!< 現在未使用 参加プレイヤー数が3人以上の時に仕様予定
	u16	eve_ofst;	//!< チェック用アドレス差分
} GMS_ENEMY_PACKET;

/// 敵情報パケット 状態 GMS_ENEMY_PACKET:state
typedef enum {
	GMD_ENEMY_STATE_NULL  = 0,
	GMD_ENEMY_STATE_DAMAGE,		//!< ダメージ
	GMD_ENEMY_STATE_DIE,		//!< 死亡
	GMD_ENEMY_STATE_HIT1,		//!< 一つ目の矩形に当たった（必要な物のみこのパケットを送る
	GMD_ENEMY_STATE_HIT2,		//!< 二つ目の矩形に当たった
	GMD_ENEMY_STATE_HIT3,		//!< 三つ目の矩形に当たった
	GMD_ENEMY_STATE_HIT_SP,		//!< 特殊矩形に当たった
	
	GMD_ENEMY_STATE_ITEM_SLOW,	//!< スローアイテム
	GMD_ENEMY_STATE_ITEM_CON,	//!< 混乱アイテム
	GMD_ENEMY_STATE_ITEM_TEN,	//!< テンションアイテム
	GMD_ENEMY_STATE_ITEM_WARP,	//!< ワープアイテム

	GMD_ENEMY_STATE_CHECKED,	//!< パケットチェック済み(システム管理用)

	GMD_ENEMY_STATE_MAX
} GME_ENEMY_PACKET_STATE;
#endif

// =============================================================================
// 敵・ギミックオブジェクト
// =============================================================================
/// 矩形設定
enum {
	GMD_ENEMY_RECT_DEF	= 0,		//!< くらい矩形
	GMD_ENEMY_RECT_ATK,				//!< 攻撃矩形
	GMD_ENEMY_RECT_BODY,			//!< 体あたり(存在判定)

	// 以降必要に応じて追加
	// 追加した場合は GmEnemyCreateWork の atk_flag, def_flagを調整する事

	GMD_ENEMY_RECT_NUM				//!< 保持矩形数
};

/// 敵・ギミックオブジェクト ベース構造体
typedef struct tag_GMS_ENEMY_COM_WORK {
	OBS_OBJECT_WORK			obj_work;
	OBS_RECT_WORK			rect_work[GMD_ENEMY_RECT_NUM];
//	OBS_COLLISION_OBJ		tCol;
	OBS_COLLISION_WORK		col_work;			//!< オブジェクト地形ワーク
	//NNSSndHandle*			h_snd_se;			//!< 汎用SEハンドル

	GMS_EVE_RECORD_EVENT	*eve_rec;			//!< レコードポインタ
	u8						eve_x;				//!< 生成スキップ情報待避バッファ(pEve->pos_xを待避 pEve->pos_xが-1で次回生成スキップ)
	u8						vit;				//!< 体力
	fx32					born_pos_x;			//!< 初期生成位置
	fx32					born_pos_y;

	fx32					invincible_timer;	//!< 無敵タイマー

	u32						enemy_flag;			//!< 下位16bit 汎用フラグ  上位16bit システム固定(敵用)フラグ
	u16						act_state;			//!< アクション状態

//	u16						usScore;
//	s32						lWork1;
//	s32						lWork2;

	OBS_OBJECT_WORK			*target_obj;		//!< 目標オブジェクト（基本的にプレイヤーポインタ）

	// 対象接着設定
	VecU16					target_dp_dir;		//!< 対象接着角度
	VecFx32					target_dp_pos;		//!< 対象接着位置
	fx32					target_dp_dist;		//!< 対象接着距離

	// Net対応
	//GMS_ENEMY_PACKET		packet;				//!< パケットデータ

} GMS_ENEMY_COM_WORK;
// 敵・ギミック毎に必要な変数は、GMS_ENEMY_WORKをベースに個別に構造体を作成する事
// (例)
// typedef struct tag_GMS_ENEMY_EGG_WORK {
//		GMS_ENEMY_WORK	enemy;
//		u32				work;
//} GMS_ENEMY_EGG_WORK;
//

/// 敵・ギミックオブジェクト 3Dモデル使用 構造体
typedef struct tag_GMS_ENEMY_3D_WORK {
	GMS_ENEMY_COM_WORK			ene_com;
	OBS_ACTION3D_NN_WORK		obj_3d;
} GMS_ENEMY_3D_WORK;

// GMS_ENEMY_WORK:enemy_flag
// 下位16bitは使用場所で定義する事
#define GMD_ENEMY_FLAG_USER1		( 1 << 0 ) // 汎用使用フラグ1
#define GMD_ENEMY_FLAG_USER2		( 1 << 1 ) // 汎用使用フラグ2
#define GMD_ENEMY_FLAG_USER3		( 1 << 2 ) // 汎用使用フラグ3
#define GMD_ENEMY_FLAG_USER4		( 1 << 3 ) // 汎用使用フラグ4

// 上位16bit システム固定フラグ
#define GMD_ENEMY_FLAG_NOPRESSDIE	( 1 << 14 )			//!< プレイヤーを圧死させない
#define GMD_ENEMY_FLAG_NOHOMING		( 1 << 15 )			//!< ホーミング対象からはずす(obj_type == ENEMY|GIMMICKの時)
#define GMD_ENEMY_FLAG_DIE			( 1 << 16 )			//!< 死亡フラグ
#define GMD_ENEMY_FLAG_NOANIMAL		( 1 << 17 )			//!< 動物なしフラグ
#define GMD_ENEMY_FLAG_ROOM			( 1 << 18 )			//!< ルーム用処理軽減タイプ

//----- Macros ---------------------------------------------------------
// ==========================================================================
// GMM_ENEMY_CREATE_WORK
/*!
 *	エネミー(ギミック)ワークの作成・初期化
 *
 *	@param	eve_rec		[in]	イベントレコード
 *	@param	pos_x		[in]	初期生成位置X
 *	@param	pos_y		[in]	初期生成位置Y
 *	@param	work_size	[in]	取得するTCBワークサイズ
 *	@param	name		[in]	デバッグ用TCB名(NULL可)
 *
 *	@return	取得したOBS_OBJECT_WORKワークアドレス
 *
 *	@note
 *		指定サイズのワークを持つオブジェクト管理TCBを作成し\n
 *		OBS_OBJECT_WORKの基本設定を行います。\n
 *		work_sizeは sizeof(GMS_ENEMY_COM_WORK) 以上の値を設定して下さい。
 */
// ==========================================================================
#if defined (MTD_DEBUG)
#define GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, work_size, name)	(GmEnemyCreateWork(eve_rec, pos_x, pos_y, work_size, GMD_TASK_PRIO_ENEMY, name))
#else
#define GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, work_size, name)	(GmEnemyCreateWork(eve_rec, pos_x, pos_y, work_size, GMD_TASK_PRIO_ENEMY))
#endif

// ==========================================================================
// GMM_ENEMY_CREATE_RIDE_WORK
/*!
 *	ライドタイプ エネミー(ギミック)ワークの作成・初期化
 *
 *	@param	eve_rec		[in]	イベントレコード
 *	@param	pos_x		[in]	初期生成位置X
 *	@param	pos_y		[in]	初期生成位置Y
 *	@param	work_size	[in]	取得するTCBワークサイズ
 *	@param	name		[in]	デバッグ用TCB名(NULL可)
 *
 *	@return	取得したOBS_OBJECT_WORKワークアドレス
 *
 *	@note
 *		プレイヤーが乗るタイプのオブジェクトを生成します(処理優先が上)\n
 *		指定サイズのワークを持つオブジェクト管理TCBを作成し\n
 *		OBS_OBJECT_WORKの基本設定を行います。\n
 *		work_sizeは sizeof(GMS_ENEMY_COM_WORK) 以上の値を設定して下さい。
 */
// ==========================================================================
#if defined (MTD_DEBUG)
#define GMM_ENEMY_CREATE_RIDE_WORK(eve_rec, pos_x, pos_y, work_size, name)	(GmEnemyCreateWork(eve_rec, pos_x, pos_y, work_size, GMD_TASK_PRIO_GIMMICK_R, name))
#else
#define GMM_ENEMY_CREATE_RIDE_WORK(eve_rec, pos_x, pos_y, work_size, name)	(GmEnemyCreateWork(eve_rec, pos_x, pos_y, work_size, GMD_TASK_PRIO_GIMMICK_R))
#endif

// ================================================================
// ギミック用
// ================================================================
// ================================================================
// GMM_GMK_CHECK_VALID_GMK
/*!
  ギミック用 ギミック有効性チェック

  @param gmk_work [in] エネミーワークポインタ GMS_ENEMY_WORK

  @note
	GMS_ENEMY_WORK:target_objにプレイヤー、GMS_PLAYER_WORK:gmk_objに時ギミックが\n
	設定するギミック稼動中に ギミックが有効であるかチェックします
 */
// ================================================================
#define GMM_GMK_CHECK_VALID_GMK(gmk_work)	((gmk_work)->target_obj && \
											((GMS_PLAYER_WORK*)(gmk_work)->target_obj)->gmk_obj == (OBS_OBJECT_WORK*)(gmk_work) && \
											!(((GMS_PLAYER_WORK*)(gmk_work)->target_obj)->player_flag & GMD_PLF_DIE))


// ================================================================
// GMM_GMK_GET_PLAYER_KEY_PUSH
/*!
  ギミック用 プレイヤーキー PUSH 取得

  @param gmk_work [in] エネミーワークポインタ GMS_ENEMY_WORK

  @note
	GMS_ENEMY_WORK:target_obj にプレイヤー保持中にプレイヤーのキーを取得します。\n
	使用まえに GMM_GMK_CHECK_VALID_GMK でギミック有効性をチェックして下さい
 */
// ================================================================
#define GMM_GMK_GET_PLAYER_KEY_PUSH(gmk_work)	(((GMS_PLAYER_WORK*)(gmk_work)->target_obj)->key_push)

// ================================================================
// GMM_GMK_GET_PLAYER_KEY_PUSH
/*!
  ギミック用 プレイヤーキー ON 取得

  @param gmk_work [in] エネミーワークポインタ GMS_ENEMY_WORK

  @note
	GMS_ENEMY_WORK:target_obj にプレイヤー保持中にプレイヤーのキーを取得します。\n
	使用まえに GMM_GMK_CHECK_VALID_GMK でギミック有効性をチェックして下さい
 */
// ================================================================
#define GMM_GMK_GET_PLAYER_KEY_ON(gmk_work)		(((GMS_PLAYER_WORK*)(gmk_work)->target_obj)->key_on)


//----- Macros Functions -----------------------------------------------



//----- External Variables ---------------------------------------------
//extern void * _nl_ene_archive; // エネミー＆ギミックアーカイブポインタ
extern u8 NNM_ALIGN_VC(4) NNM_ALIGN_CW(4) g_gm_default_col[];		// 基本地形情報 //_nl_default_col[];
extern u8 NNM_ALIGN_VC(4) NNM_ALIGN_CW(4) g_gm_through_colattr[];	// 地形属性情報バッファ //_nl_through_colattr[];



//----- External Declarations ------------------------------------------
// ==========================================================================
// GmEnemyCreateWork
/*!
 *	エネミーワークの作成・初期化
 *
 *	@param	eve_rec		[in]	イベントレコード
 *	@param	pos_x		[in]	初期生成位置X
 *	@param	pos_y		[in]	初期生成位置Y
 *	@param	work_size	[in]	取得するTCBワークサイズ
 *	@param	prio		[in]	タスク優先
 *	@param	name		[in]	デバッグ用TCB名(NULL可)
 *
 *	@return	取得したOBS_OBJECT_WORKワークアドレス
 *
 *	@note
 *		指定サイズのワークを持つオブジェクト管理TCBを作成し\n
 *		OBS_OBJECT_WORKの基本設定を行います。\n
 *		work_sizeは sizeof(GMS_ENEMY_COM_WORK) 以上の値を設定して下さい。\n
 *		当たり無し で設定します
 */
// ==========================================================================
#if defined (MTD_DEBUG)
extern OBS_OBJECT_WORK* GmEnemyCreateWork(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u32 work_size, u16 prio, const char *name);
#else
extern OBS_OBJECT_WORK* GmEnemyCreateWork(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u32 work_size, u16 prio);
#endif

// ================================================================
// GmEnemyDefaultExit
/*!
  エネミー解放処理
    
  @param pTcb [in] タスクポインタ
 */
// ================================================================
extern void GmEnemyDefaultExit(MTS_TASK_TCB *pTcb);

// ================================================================
// GmEnemyActionSet
/*!
  アクション設定関数

	@param ene_com	[in] オブジェクトワークポインタ
	@param id			[in] 設定するアクションID

 */
// ================================================================
extern void GmEnemyActionSet(GMS_ENEMY_COM_WORK *ene_com, u16 id);

// ================================================================
// GmEnemyDefaultDefFunc
/*!
  エネミーダメージ喰らい処理

	@param	mine_rect		[in]	自分矩形ワークポインタ
	@param	match_rect		[in]	相手矩形ワークポインタ

  @note
	ppDefへ登録
 */
// ================================================================
extern void GmEnemyDefaultDefFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect);

// ==========================================================================
// GmEnemyDefaultAtkFunc
/*!
 *	エネミー攻撃HIT処理
 *
 *	@param	mine_rect		[in]	自分矩形ワークポインタ
 *	@param	match_rect		[in]	相手矩形ワークポインタ
 *
 *	@note
 *		ppHitへ登録
 */
// ==========================================================================
extern void GmEnemyDefaultAtkFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect);

// ==========================================================================
// 登録関数
// ==========================================================================
// ==========================================================================
// GmEnemyDefaultMoveFunc
/*!
 *	エネミー移動処理
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@ntoe
 *		ppMove登録
 */
// ==========================================================================
extern void GmEnemyDefaultMoveFunc(OBS_OBJECT_WORK *obj_work);

// ================================================================
// GmEnemyDefaultInFunc
/*!
  エネミー特殊毎処理（内容未定 DBの例ではヒットストップカウンタや無敵カウンタなど）

  @note
	ppIn登録
 */
// ================================================================
extern void GmEnemyDefaultInFunc(OBS_OBJECT_WORK *obj_work);

#if 0
// ================================================================
// ネット対応
// ================================================================
// ================================================================
// GmEnemyDefFuncSetPacketState
/*!
  エネミーダメージ喰らい処理 パケットステータス指定タイプ

  @param	match_rect	[in]	相手矩形ワーク
  @param	mine_rect	[in]	自分矩形ワーク
  @param	state		[in]	ステート GMD_ENEMY_STATE_****

  @return	GME_ENEMY_DEF_FUNC_PS_STATE

  @note
	ニトロHIT以外は、stateで指定したステータスでパケット送信します。\n
	HIT処理内から呼び出して下さい。\n
	送信ステータスがGMD_ENEMY_STATE_DIEの場合はgmEnemyDefaultDefFuncを使用して下さい。\n
	ニトロHIT以外は敵死亡ステータス, HIT OFF を行いません。\n
	体力対応はありません。
 */
// ================================================================
typedef enum tag_GME_ENEMY_DEF_FUNC_PS_STATE {
	GMD_ENEMY_DEF_FUNC_PS_STATE_NOHIT	= 0,		//!< HIT無し
	GMD_ENEMY_DEF_FUNC_PS_STATE_NORMAL,				//!< 通常HIT
	GMD_ENEMY_DEF_FUNC_PS_STATE_NITRO,				//!< ニトロHIT

	GMD_ENEMY_DEF_FUNC_PS_STATE_MAX
} GME_ENEMY_DEF_FUNC_PS_STATE;
GME_ENEMY_DEF_FUNC_PS_STATE GmEnemyDefFuncSetPacketState(OBS_RECT_WORK *match_rect, OBS_RECT_WORK *mine_rect, u8 state);

// ================================================================
// GmEnemyPacketPreAllCheck
/*!
  エネミーパケットチェック 前処理

	@note
		アイテム取得に対するステータス変化処理はここで行い\n
		その他のパケットは個々のオブジェクトの処理内で行う\n
		(イベントに対してパケット受信フラグを設定する)
 */
// ================================================================
void GmEnemyPacketPreAllCheck();

// ================================================================
// GmEnemyPacketPostAllCheck
/*!
  エネミーパケットチェック

	@note
		最後にチェックされていないパケットをチェックする(非連動画面対応)
 */
// ================================================================
void GmEnemyPacketPostAllCheck(void);

// ================================================================
// GmEnemyPacketSet
/*!
  敵パケット送信関数
 
  @param pWork   [in] エネミーワーク
  @param pPlayer [in] プレイヤーワーク
  @param usID    [in] パケットID
 
 */
// ================================================================
void GmEnemyPacketSet( GMS_ENEMY_WORK *pWork, GMS_PLAYER_WORK * pPlayer, u16 usID );
#endif

// ================================================================
// Util
// ================================================================

extern void mppEnemyList_clearAll();
extern void mppEnemyList_add(GMS_EVE_RECORD_EVENT *eve_rec);
extern bool mppEnemyList_remove(GMS_EVE_RECORD_EVENT *eve_rec);
extern int mppEnemyList_getCount();	
extern GMS_EVE_RECORD_EVENT* mppEnemyList_getRec(int index);
	

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_ENEMY_H_
