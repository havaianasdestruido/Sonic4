// =======================================================================
/*!
  @file	gmBoss4.h
  @brief ボス1

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Util.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_4_UTIL_H_
#define GM_BOSS_4_UTIL_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/
#include "fx.h"
#include "gmRing.h"
#include "gmBossCommon.h"

/*------ Macros --------------------------------------------------------*/
#define		GMM_BOSS4_AREA_LEFT()				((fx32)(g_gm_main_system.map_fcol.left << FX32_SHIFT))
#define		GMM_BOSS4_AREA_TOP()				((fx32)(g_gm_main_system.map_fcol.top << FX32_SHIFT))
#define		GMM_BOSS4_AREA_RIGHT()				((fx32)(g_gm_main_system.map_fcol.right << FX32_SHIFT))
#define		GMM_BOSS4_AREA_BOTTOM()				((fx32)(g_gm_main_system.map_fcol.bottom << FX32_SHIFT))
// 画面中心
#define		GMM_BOSS4_AREA_CENTER_X()			(GMM_BOSS4_AREA_LEFT() + ((GMM_BOSS4_AREA_RIGHT() - GMM_BOSS4_AREA_LEFT()) / 2))
#define		GMM_BOSS4_AREA_CENTER_Y()			(GMM_BOSS4_AREA_TOP() + ((GMM_BOSS4_AREA_BOTTOM() - GMM_BOSS4_AREA_TOP()) / 2))


#define		GMD_BOSS4_RIGHTWARD_ANGLE			(AKM_DEGtoA16(60.f))		//!< 右向き時の角度（正面方向基準）
#define		GMD_BOSS4_LEFTWARD_ANGLE			(AKM_DEGtoA16(300.f))		//!< 左向き時の角度（正面方向基準）

#define		GMD_BOSS4_PLYATK_HORM_AFTER_SPD_X	(5.0f)						//!< ホーミング攻撃したあとの跳ね返りスピード(X)
#define		GMD_BOSS4_PLYATK_HORM_AFTER_SPD_Y	(4.0f)						//!< ホーミング攻撃したあとの跳ね返りスピード(Y)

#define		GMD_BOSS4_PLYATK_NORM_AFTER_SPD_X	(0.6f)						//!< ホーミング攻撃したあとの跳ね返り係数(X)
#define		GMD_BOSS4_PLYATK_NORM_AFTER_SPD_Y	(1.2f)						//!< ホーミング攻撃したあとの跳ね返り係数(Y)

#define		GMD_BOSS4_SNM_NO_MAX				(32)						//!< ノード番号の最大値

#define		GMD_BOSS4_STAGE4BOSS_ID				(15)						//!< ステージ4のBOSSのID
/*------ Macro Functions -----------------------------------------------*/
//------------------------------------------------------
// PAL用マクロ
//	関数そのものを関数そのものを変更する方法もありますが、
//  数値を読み取る場合にその数が飛ぶ可能性があるため
//	念のためこちらのマクロで処理します
//------------------------------------------------------
#define		GMM_BOSS4_PAL_SPEED(spd)			(GmBoss4UtilCheckPAL() ? (spd * (60.0f/50.0f)) : spd)
#define		GMM_BOSS4_PAL_TIME(time)			(GmBoss4UtilCheckPAL() ? (Sint32)(time * (50.0f/60.0f)) : (Sint32)time )
#define		GMM_BOSS4_PAL_ZOOM(zoom)			(GmBoss4UtilCheckPAL() ? (zoom>=1.0f ? zoom * (60.0f/50.0f) : zoom * (50.0f/60.0f)) : zoom )

/*------ Definitions ---------------------------------------------------*/
extern	const	NNS_RGB		gm_boss4_color_white;

enum GME_BOSS4_DIR{
	GME_BOSS4_DIR_RIGHT = 0,
	GME_BOSS4_DIR_LEFT,

	GME_BOSS4_DIR_MAX,
};

//-----------------------------------------------------
// 方向指定する時の構造体
//-----------------------------------------------------
typedef struct tag_GMS_BOSS4_DIRECTION
{
	// 実際にどちらを向いているか
	GME_BOSS4_DIR			direction;

	Angle16					cur_angle;		//!< 現在の向き
	Angle16					orig_angle;		//!< 振り向き開始時角度
	Angle32					turn_angle;		//!< 振り向き時オフセット角度
	Angle32					turn_amount;	//!< 現在の角度から最終的に何度回転させるか
	Angle32					turn_spd;		//!< 回転角速度
	Angle32					turn_gen_var;	//!< 緩やかターンの速度カーブ角度変数
	Angle32					turn_gen_factor;	//!< 緩やかターンの速度カーブ決定値（コサイン角度）

} GMS_BOSS4_DIRECTION;

//! ダメージ点滅ワーク
typedef struct tag_GMS_BOSS4_FLICKER_WORK
{
	BOOL	is_active;		//!< アクティブフラグ
	Uint32	cycles;			//!< 周期数
	Uint32	interval_timer;	//!< インターバルタイマ
	Angle32	cur_angle;		//!< 現在の角度
	Angle32	add_timer;		//!< 足しこむ角度
	Float	radius;			//!< モデル半径
	Uint32	interval_flk;	//!< インターバルタイマ(点滅間隔用)
	NNS_RGB	color;			//!< カラー
	Uint32	reserved[3];
} GMS_BOSS4_FLICKER_WORK;

typedef	struct	tag_GMS_BOSS4_MOVE
{
	VecFx32		pos;

	VecFx32		start;
	VecFx32		end;
	Sint32		now_count;
	Sint32		max_count;
	Sint32		type;		//!< 0 = 等間隔, 1= サインカーブ間隔
							// sin0 - sin180の間で進む
} GMS_BOSS4_MOVE;

typedef	struct	tag_GMS_BOSS4_NODE_MATRIX
{
	char					_id[8];			//!< 規定の文字が入る(開放処理用)
	Sint32					initCount;
	Sint32					useCount;
	GMS_BS_CMN_BMCB_MGR		mtn_mgr;		//!< モーションCB管理ワーク
	GMS_BS_CMN_SNM_WORK		snm_work;		//!< SNMワーク
	Sint32					work[GMD_BOSS4_SNM_NO_MAX];
	OBS_OBJECT_WORK*		obj_work;
} GMS_BOSS4_NODE_MATRIX;


//! 1ショットタイマワーク
typedef struct tag_GMS_BOSS4_1SHOT_TIMER
{
	Uint32	timer;
	BOOL	is_active;	//! 有効フラグ
} GMS_BOSS4_1SHOT_TIMER;


// タイマーの値がある場合、あたらなくなるタイマー
typedef struct tag_GMS_BOSS4_NOHIT_TIMER
{
	Uint32					timer;
	GMS_ENEMY_COM_WORK*		ene_com;

} GMS_BOSS4_NOHIT_TIMER;

// =======================================================================
//	GmBoss4UtilInit1ShotTimer()
/*!
	１ショットタイマー初期化
  
	@param	one_shot_timer	[io]	タイマーワーク
	@param	frame			[io]	フレーム数

 */
// =======================================================================
void GmBoss4UtilInit1ShotTimer(	GMS_BOSS4_1SHOT_TIMER* one_shot_timer, Uint32 frame);

// =======================================================================
//	GmBoss4UtilInit1ShotTimer()
/*!
	１ショットタイマー更新
  
	@param	one_shot_timer	[io]	タイマーワーク

 */
// =======================================================================
BOOL GmBoss4UtilUpdate1ShotTimer(GMS_BOSS4_1SHOT_TIMER *one_shot_timer);

// =======================================================================
//	GmBoss4UtilInitNodeMatrix()
/*!
	ノード参照システム簡易化
  
	@param	obj_work	[io]	オブジェクトワーク
	@param	node_work	[io]	ノードワーク
	@param	max_node	[in]	使用できるNodeMatrixの数

 */
// =======================================================================
void GmBoss4UtilInitNodeMatrix( GMS_BOSS4_NODE_MATRIX* node_work, OBS_OBJECT_WORK* obj_work, Sint32 max_node = 8 );

// =======================================================================
//	GmBoss4UtilExitNodeMatrix()
/*!
	ノード参照システム終了
  
	@param	obj_work	[io]	オブジェクトワーク
	@param	node_work	[io]	ノードワーク
	@param	max_node	[in]	使用できるNodeMatrixの数

 */
// =======================================================================
void GmBoss4UtilExitNodeMatrix( GMS_BOSS4_NODE_MATRIX* node_work );

// =======================================================================
//	GmBoss4UtilInitNodeMatrix()
/*!
	ノードを参照 (モデルのノードIDをそのまま指定する)
  
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
const NNS_MATRIX* GmBoss4UtilGetNodeMatrix( GMS_BOSS4_NODE_MATRIX* node_work, Sint32 node_id );


// =======================================================================
//	GmBoss4UtilSetNodeMatrixNN()
/*!
	ノードにくっつける
  
	@param	obj_work	[io]	くっつけるオブジェ
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
void	GmBoss4UtilSetNodeMatrixNN( OBS_OBJECT_WORK* obj_work,
								   GMS_BOSS4_NODE_MATRIX* node_work, Sint32 node_id );

// =======================================================================
//	GmBoss4UtilSetNodeMatrixES()
/*!
	ノードにくっつける
  
	@param	obj_work	[io]	くっつけるオブジェ
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
void	GmBoss4UtilSetNodeMatrixES( OBS_OBJECT_WORK* obj_work,
								   GMS_BOSS4_NODE_MATRIX* node_work, Sint32 node_id );


// =======================================================================
//	GmBoss4UtilSetMatrixNN()
/*!
	マトリクスにくっつける
  
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
void	GmBoss4UtilSetMatrixNN( OBS_OBJECT_WORK* obj_work, NNS_MATRIX*	w_mtx );

// =======================================================================
//	GmBoss4UtilSetMatrixES()
/*!
	マトリクスにくっつける
  
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
void	GmBoss4UtilSetMatrixES( OBS_OBJECT_WORK* obj_work, NNS_MATRIX*	w_mtx );


// =======================================================================
// GmBoss4UtilPlayerStop()
/*!
  ソニックをストップさせる
  
  @param b			[in]	ストップ

 */
// =======================================================================
void GmBoss4UtilPlayerStop(BOOL b);


// =======================================================================
// GmBoss4UtilTimerStop()
/*!
  ゲームタイマーをストップさせる
  
  @param b			[in]	ストップ

 */
// =======================================================================
extern void GmBoss4UtilTimerStop( BOOL b );

// =======================================================================
// GmBoss4UtilInitMove()
/*!
  移動設定
  
  @param _work			[io]	MOVEワーク
  @param start			[in]	スタート位置
  @param end			[in]	エンド位置
  @param count			[in]	フレーム数
  @param type			[in]	0=等間隔/ 1=サインカーブ間隔

  @note
	スタート位置からエンド位置まで進むようにワークを定義する
 */
// =======================================================================
extern void GmBoss4UtilInitMove(GMS_BOSS4_MOVE* _work, VecFx32* start, VecFx32* end, Sint32 count, Sint32 type = 1);

// =======================================================================
// GmBoss4UtilUpdateMove()
/*!
  移動
  
  @param _work		[io]	MOVEワーク
  @praam pos		[out]	ポジション(NULLだとかきこまない)

  @note
	スタート位置からエンド位置まで進む
	移動完了時にTRUEを返す
	posには自分でワークを用意する必要がある
 */
// =======================================================================
extern BOOL GmBoss4UtilUpdateMove(GMS_BOSS4_MOVE* _work, VecFx32* pos = NULL );


// =======================================================================
// GmBoss4UtilUpdateMovePositon()
/*!
  移動要素をオブジェクトに反映させる
  
  @param move_work		[io]	MOVEワーク
  @param obj_work		[io]	OBJワーク

  @note
	毎フレーム呼ぶ必要がある
 */
// =======================================================================
extern void GmBoss4UtilUpdateMovePosition(GMS_BOSS4_MOVE* _work, OBS_OBJECT_WORK* obj_work );


// =======================================================================
// GmBoss4UtilIsDirectionPositiveFromCurrent
/*!
  最短回転が正回転方向か判定
  
  @param body_work		[io]	本体ワーク
  @param target_angle	[in]	目標方向
  
  @retval TRUE	現在の角度→指定角度が正回転方向
  @retval FALSE 現在の角度→指定角度が負回転方向
  
  @note
  現在の角度から指定角度への最短回転が正回転方向か判定します。
  (e.g. 現在の角度が130degで指定角が90degの場合は最短回転は負方向。
        現在の角度が270degだった場合は最短回転は正方向。)
 */
// =======================================================================
extern BOOL GmBoss4UtilIsDirectionPositiveFromCurrent(GMS_BOSS4_DIRECTION*	_work, Angle16 target_angle);

// =======================================================================
// GmBoss4UtilUpdateDirection
/*!
  ボス１ 向き更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  ボスの向き情報をオブジェクトの角度に反映します。
  毎フレーム呼んでください。
 */
// =======================================================================
#if _IPHONE
extern void GmBoss4UtilUpdateDirection(GMS_BOSS4_DIRECTION* _work, OBS_OBJECT_WORK* obj_work, BOOL flag = FALSE);
#else
extern void GmBoss4UtilUpdateDirection(GMS_BOSS4_DIRECTION* _work, OBS_OBJECT_WORK* obj_work );
#endif // _IPHONE

// =======================================================================
// GmBoss4UtilSetDirectionNormal
/*!
  標準の角度（真横より正面寄り）に設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  真横よりも少し正面寄りに向くように角度設定を行います。
  GME_BOSS4_DIRで向きは設定されます
 */
// =======================================================================
extern void GmBoss4UtilSetDirectionNormal( GMS_BOSS4_DIRECTION* _work );

// =======================================================================
// GmBoss4UtilSetDirection
/*!
  任意角度設定
  
  @param body_work	[io]	本体ワーク
  @param deg		[in]	角度
  
  @note
  指定の値に角度を設定します。オブジェクトへの反映は行われません。
 */
// =======================================================================
extern void GmBoss4UtilSetDirection(GMS_BOSS4_DIRECTION* _work, Angle16 deg);

// =======================================================================
// gmBoss4BodyInitTurn
/*!
  振り向き回転処理を初期化
 
  @param body_work		[io]	本体ワーク
  @param turn_amount	[in]	振り向き回転量
  @param turn_spd		[in]	回転角速度
  
  @note
  turn_amountには現在の角度から差分でどれだけ回転させるか指定します。
  時計回りはマイナス値、反時計回りはプラス値を指定します。
  Angle32で扱える角度の範囲に注意してください。
 */
// =======================================================================
extern void GmBoss4UtilInitTurn(GMS_BOSS4_DIRECTION* _work,
						 Angle32 turn_amount, Angle32 turn_spd);

// =======================================================================
// gmBoss4BodyUpdateTurn
/*!
  振り向き回転処理更新
  
  @param body_work	[io]	本体ワーク
  @param spd_rate	[in]	速度係数（デフォルト1.f）
  
  @retval	TRUE	振り向き回転完了
  @retval	FALSE	振り向き回転中
  
  @note
  実際に回転を実施します。deg_addはgmBoss4BodyInitTurn()で指定した回転角と
  同じ符号になるようにしてください。
 */
// =======================================================================
extern BOOL GmBoss4UtilUpdateTurn(GMS_BOSS4_DIRECTION* _work, Float spd_rate = 1.0f);

// =======================================================================
// gmBoss4BodyInitTurnGently
/*!
  緩やか振り向き回転 初期化
 
  @param body_work		[io]	本体ワーク
  @param dest_angle		[in]	目標角度
  @param frame			[in]	回転にかけるフレーム数
  @param is_positive	[in]	正方向回転フラグ(TRUE : 正回転, FALSE : 負回転)
 */
// =======================================================================
extern void GmBoss4UtilInitTurnGently(GMS_BOSS4_DIRECTION* _work, Angle16 dest_angle,
							   Sint32 frame, BOOL is_positive);

// =======================================================================
// gmBoss4BodyUpdateTurnGently
/*!
  緩やか振り向き回転 更新
 
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
extern BOOL GmBoss4UtilUpdateTurnGently(GMS_BOSS4_DIRECTION* _work);




// =======================================================================
// GmBoss4UtilLookAtPlayer
/*!
  緩やか振り向き回転で、勝手にプレイヤーの向きを向く。
 
  @param _work		[io]	方向ワーク
  @param obj_work	[io]	OBJワーク
  @param time		[in]	方向転換時間
 */
// =======================================================================
BOOL GmBoss4UtilLookAtPlayer(GMS_BOSS4_DIRECTION* _work, OBS_OBJECT_WORK* obj_work, Sint32 time);

// =======================================================================
// GmBoss4UtilLookAtPlayerCheckDirection
/*!
  緩やか振り向き回転で、勝手にプレイヤーの向きを向く。
  (Updateで使用できるようにしたバージョン。ただ初期化が絶対に必要)
 
  @param _work		[io]	方向ワーク
  @param obj_work	[io]	OBJワーク
  @param time		[in]	方向転換時間
 */
// =======================================================================
BOOL GmBoss4UtilLookAtPlayerCheckDirection(GMS_BOSS4_DIRECTION* _work, OBS_OBJECT_WORK* obj_work, Sint32 time);


// =======================================================================
// GmBoss4UtilLookAtCenter
/*!
  緩やか振り向き回転で、勝手に中央を向く。
 
  @param _work		[io]	方向ワーク
  @param obj_work	[io]	OBJワーク
  @param time		[in]	方向転換時間
 */
// =======================================================================
BOOL GmBoss4UtilLookAtCenter(GMS_BOSS4_DIRECTION* _work, OBS_OBJECT_WORK* obj_work, Sint32 time);


// =======================================================================
// GmBoss4UtilLookAtPlayer
/*!
  緩やか振り向き回転で、勝手その方向を向く
  GmBoss4UtilLookAt***()を先に実行しておく必要がある
 
  @param _work		[io]	方向ワーク
  @param obj_work	[io]	OBJワーク
  @param time		[in]	方向転換時間
 */
// =======================================================================
BOOL GmBoss4UtilLookAt(GMS_BOSS4_DIRECTION* _work );


// =======================================================================
// GmBoss4UtilInitFlicker
/*!
  点滅初期化
 
  @param obj_work	[io]	オブジェクトワーク（obj_3dが設定されている必要があります）
  @param flk_work	[io]	ダメージ点滅ワーク
  @param times		[in]	点滅回数
  @param start		[in]	点滅までの時間
  @param start		[in]	点滅のスピードおよび間隔

  
  @note
  前の点滅状態を引き継がずに強制的に初期化する場合はclean_init=TRUEを設定してください。
 */
// =======================================================================
void GmBoss4UtilInitFlicker( OBS_OBJECT_WORK *obj_work,
										GMS_BOSS4_FLICKER_WORK *flk_work,
										Sint32		times = 3,		// 点滅回数
										Sint32		start = 0,		// 始まるまでの時間
										Sint32		spd = 4,		// 点滅スピード
										Sint32		interval = 0,	// 点滅間隔
										const NNS_RGB*	rgb = &gm_boss4_color_white
										);

// =======================================================================
// GmBsCmnUpdateObject3DNNDamageFlicker
/*!
  ダメージ点滅更新
  
  @param obj_work	[io]	オブジェクトワーク（obj_3dが設定されている必要があります）
  @param flk_work	[io]	ダメージ点滅ワーク
  
  @retval	TRUE	更新終了
  @retval	FALSE	更新中
  
  @note
  毎フレーム呼んでください。
  TRUEを待たずに終了する場合はGmBoss4UtilEndFlicker()を呼んで
  終了処理を行ってください。
 */
// =======================================================================
BOOL GmBoss4UtilUpdateFlicker( OBS_OBJECT_WORK *obj_work,
										  GMS_BOSS4_FLICKER_WORK *flk_work );

// =======================================================================
// GmBoss4UtilEndFlicker
/*!
  ダメージ点滅終了
  
  @param obj_work	[io]	オブジェクトワーク（obj_3dが設定されている必要があります）
  @param flk_work	[io]	ダメージ点滅ワーク
  
  @note
  ダメージ点滅を終了してパラメータをクリアします。
  更新が終了していない状態で呼び出すこともできますが、
  即時的に表示が切り替わることに留意してください。
 */
// =======================================================================
void GmBoss4UtilEndFlicker( OBS_OBJECT_WORK *obj_work,
									   GMS_BOSS4_FLICKER_WORK *flk_work);




// =======================================================================
// GmBoss4UtilRotateVecFx32
/*!
  XY方向ベクトルをZ軸回転で角度分回した結果を返す
  
  @param f		[io]	方向ベクトル
  @param angle	[in]	角度
  
 */
// =======================================================================
void GmBoss4UtilRotateVecFx32( VecFx32* f, Angle32 angle );


// =======================================================================
// GmBoss4UtilIterateDamageRingInit
/*!
  ダメージリングを列挙開始
  
 */
// =======================================================================
void GmBoss4UtilIterateDamageRingInit();


// =======================================================================
// GmBoss4UtilIterateDamageRingGet
/*!
  ダメージリングを取得

  @return	ダメージリング構造体(なければNULL)
 */
// =======================================================================
GMS_RING_WORK* GmBoss4UtilIterateDamageRingGet();



// =======================================================================
// GmBoss4UtilSetPlayerReaction
/*!
//	第一段階の
//	カプセルを攻撃した後のBOSS4共通リアクションの設定
  
  @param player		[io]	プレイヤーワーク
  @param enemy		[io]	カプセルワーク
 */
// =======================================================================
void	GmBoss4UtilSetPlayerAttackReaction( OBS_OBJECT_WORK* player, OBS_OBJECT_WORK* enemy );

#if 0
// =======================================================================
// GmBoss4UtilCheckBossRush()
/*!
	現在のステージからステージファイナルの
	ボスラッシュかどうかをチェックします
  
 */
// =======================================================================
BOOL	GmBoss4UtilCheckBossRush();
#endif // 0

// =======================================================================
// GmBoss4UtilInitNoHitTimer()
/*!
	時間内、当たりをなくすよう設定を行います。
  
  @param timer_work	[io]	タイマーワーク(Sint32)
  @param ene_com	[io]	GMS_ENEMY_COM_WORK
  @param timer		[in]	カウンター値

 */
// =======================================================================
void	GmBoss4UtilInitNoHitTimer( GMS_BOSS4_NOHIT_TIMER* work, GMS_ENEMY_COM_WORK* ene_com, Sint32 time);


// =======================================================================
// GmBoss4UtilUpdateNoHitTimer()
/*!
	時間の更新を行います。
	時間内、当たりをなくします。
  
 */
// =======================================================================
BOOL	GmBoss4UtilUpdateNoHitTimer( GMS_BOSS4_NOHIT_TIMER* work );


// =======================================================================
// GmBoss4UtilCheckPAL()
/*!
	PAL(50fps)かをチェックする
 */
// =======================================================================
extern	BOOL	GmBoss4UtilCheckPAL();


// =======================================================================
// GmBoss4UtilIsScrollLocking()
/*!
	画面がロックされているかチェック
 */
// =======================================================================
BOOL	GmBoss4UtilIsScrollLocking();


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

#endif /* GM_BOSS_4_UTIL_H_ */
