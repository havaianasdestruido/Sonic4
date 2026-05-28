// =======================================================================
/*!
  @file	gmBoss4.h
  @brief ボス4

  @author Kosuke Yurita
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_4_H_
#define GM_BOSS_4_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/
#include "gmEnemy.h"
#include "gmBoss4Util.h"

//----------------------------------------------------------------------
// データヘッダ
//----------------------------------------------------------------------
#include "../file/common/arc/BOSS04.hmb"
#include "../file/common/model/BOSS04_MDL.hmb"
#include "../file/common/model/BOSS04_BODY_MTN.hmb"
#include "../file/common/model/BOSS04_EGG_MTN.hmb"
#include "../file/common/model/BOSS04_CAPSULE_MTN.hmb"

/*------ Macros --------------------------------------------------------*/
//############ ゲーム全般関係 #################################################
//------------------------------------------------------
/* 暫定処理切り替え */
//------------------------------------------------------

//#define		GMD_BOSS4_TEMPORARY							//!< 定義していると、暫定処理を使う

//------------------------------------------------------
/* ファイル関連 */
//------------------------------------------------------
#define		GMD_BOSS4_ARC	(g_gm_gamedat_enemy_arc)

//------------------------------------------------------
/* 機能切り替えマクロ */
//------------------------------------------------------
#define		GMD_BOSS4_AKTNML_MOVE_USE_PARTIAL_CURVE		//!< 定義していると、通常攻撃左右移動の際に円の部分曲線で補間する

//------------------------------------------------------
//	ボスラッシュ用マクロ
//	ボスラッシュを自動的に判別して内容を変更します。
//------------------------------------------------------
#define		GMM_BOSS4_STAGE( s4, sf )			(GmBoss4CheckBossRush() ? (sf) : (s4) )

//############ 高速スクロール #################################################
// TODO ボスラッシュ用調節( 左がSTAGE4, 右がFINAL )
// 調整用
#define		GMD_BOSS4_SCROLL_INIT_X				GMM_BOSS4_STAGE( 2500, 11000 ) 	//<! 強制スクロール開始ポイント

#define		GMD_BOSS4_SCROLL_START_X			GMM_BOSS4_STAGE( 2500, 11000 )	//<! 強制スクロール初期
#define		GMD_BOSS4_SCROLL_END_X				GMM_BOSS4_STAGE( 3972, 12536 )	//<! 強制スクロール後ろ

#define		GMD_BOSS4_SCROLL_OUT_X				GMM_BOSS4_STAGE( 2500, 11000 )	//<! 強制スクロールボス消滅時にワープ場所

#define		GMD_BOSS4_SCROLL_SPD_MAX			GMM_BOSS4_PAL_SPEED(8.0f)		//<! 強制スクロール最高速

#define		GMD_BOSS4_SCROLL_SPD_START			GMM_BOSS4_PAL_SPEED(4.0f)		//<! 強制スクロール初期スピード
#define		GMD_BOSS4_SCROLL_SPD_ADD			GMM_BOSS4_PAL_SPEED(0.1f)		//<! 強制スクロール加速値
#define		GMD_BOSS4_SCROLL_SPD_SUB			GMM_BOSS4_PAL_SPEED(0.2f)		//<! 強制スクロール減速値

#define		GMD_BOSS4_SCROLL_SPD_BOSS			GMM_BOSS4_PAL_SPEED(5.0f)		//<! 初期ボスの移動スピード(GMD_BOSS4_SCROLL_SPD_MAXより遅くなくてはならない)
																				//<! スクロールと同時にこのスピードでボスは動き続ける

#define		GMD_BOSS4_SCROLL_SPD_BOSS_BROKEN	GMM_BOSS4_PAL_SPEED(7.0f)		//<! 撃破後のスピード(普通GMD_BOSS4_SCROLL_SPD_MAXより遅い)

#define		GMD_BOSS4_SCROLL_RIGHT_LIMIT_X		(50)							//<! ボス機の右端からの標準位置

#define		GMD_BOSS4_SCROLL_HOMING_LENGTH		(140.0f)						//<! スクロール時ボス機にホーミングできる距離


#define		GMD_BOSS4_SCROLL_SONIC_SPD			(-0.5f)							//<! ソニックスピード(じっとしていると下がっていくように設定)
#define		GMD_BOSS4_SCROLL_SONIC_SPD_LOW		(+0.6f)							//<! ボスを倒した後画面右にいる場合のスピード
#define		GMD_BOSS4_SCROLL_SONIC_SPD_HIGH		(+1.4f)							//<! ボスを倒した後画面左にいる場合のスピード
																				//<! 1.0fで中間スピードになります

#define		GMD_BOSS4_SCROLL_COIN_ADDSPD_FIRST	(1.0f)							//<! 飛び散るコインの初期の足しこみスピード
#define		GMD_BOSS4_SCROLL_COIN_ADDSPD		(-0.1f)							//<! 飛び散るコインの毎フレーム足しこまれるスピード

#define		GMD_BOSS4_SCROLL_LIMIT_OFFSET_X			(48)							//<! プレイヤーが壁に張り付かないように(仮対処)
//############ 共通 ###########################################################
/* 定義値 */
#define		GMD_BOSS4_LIFE						GMM_BOSS4_STAGE( 8, 4 )			//!< ライフ値
#define		GMD_BOSS4_EXTRA_ATK_THRESHOLD_LIFE	GMM_BOSS4_STAGE( 3, 4 )			//!< このライフ値以下になったら追加攻撃開始
//#define	GMD_BOSS4_DEFAULT_POS_Z				(GMD_OBJ_DEFAULT_POS_Z_C)		//!< デフォルトZ位置
#define		GMD_BOSS4_DEFAULT_POS_Z				(GMD_OBJ_DEFAULT_POS_Z_C_BACK)	//!< デフォルトZ位置

#define		GMD_BOSS4_GROUND_POS_Y				((fx32)(FX32_ONE * 314))		//!< 地面の高さ

#define		GMD_BOSS4_DEFAULT_BLEND_SPD			((Float)0.125f)					//!< ボス4のデフォルトモーションブレンド速度

//############ 管理 ###########################################################
/* フラグ GMS_BOSS4_MGR_WORK::flag */
#define		GMD_BOSS4_MGR_FLAG_LOAD_END			(1 << 0)						//!< ロード完了フラグ
#define		GMD_BOSS4_MGR_FLAG_CLEAR_BOSS		(1 << 1)						//!< ボスを消去する

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
// =======================================================================
// GMM_BOSS4_AREA_***
/*!
  スクロール可能範囲矩形の座標を取得
  
  @return スクロール可能範囲矩形座標（マップ座標系, 固定小数）
  
  @note
	第一段階のスクロールロックされた際の上下左右端の座標を得るのに使用します。
 */
// =======================================================================
#define		GMM_BOSS4_AREA_LEFT()				((fx32)(g_gm_main_system.map_fcol.left << FX32_SHIFT))
#define		GMM_BOSS4_AREA_TOP()				((fx32)(g_gm_main_system.map_fcol.top << FX32_SHIFT))
#define		GMM_BOSS4_AREA_RIGHT()				((fx32)(g_gm_main_system.map_fcol.right << FX32_SHIFT))
#define		GMM_BOSS4_AREA_BOTTOM()				((fx32)(g_gm_main_system.map_fcol.bottom << FX32_SHIFT))
// 画面中心
#define		GMM_BOSS4_AREA_CENTER_X()			(GMM_BOSS4_AREA_LEFT() + ((GMM_BOSS4_AREA_RIGHT() - GMM_BOSS4_AREA_LEFT()) / 2))
#define		GMM_BOSS4_AREA_CENTER_Y()			(GMM_BOSS4_AREA_TOP() + ((GMM_BOSS4_AREA_BOTTOM() - GMM_BOSS4_AREA_TOP()) / 2))


// ===========================================================================
// 宣言 struct GMS_BOSS4_BODY_WORK
// ===========================================================================
typedef	struct tag_GMS_BOSS4_BODY_WORK	GMS_BOSS4_BODY_WORK;

// ===========================================================================
//	enum GME_BOSS4_ACT_ID
// ---------------------------------------------------------------------------
//! ボス4 アクションID列挙型
// ===========================================================================
typedef enum
{
	GME_BOSS4_ACT_ID_APP_FALL	= 0,	//!< スタート時の降下
	GME_BOSS4_ACT_ID_APP_END,			//!< スタート時の降下後の余韻
	
	GME_BOSS4_ACT_ID_PRE_ATK_NML_MOVE,	//!< 通常攻撃 開始移動
	GME_BOSS4_ACT_ID_ATK_NML_MOVE,		//!< 通常攻撃 移動中	(標準動作)
	
	GME_BOSS4_ACT_ID_START_2,			//!< 第２形態 瞬間変化	(標準動作)
	GME_BOSS4_ACT_ID_ATK_NML_MOVE2,		//!< 通常攻撃 移動中	(標準動作)

	GME_BOSS4_ACT_ID_DAMAGE_NML,		//!< 通常ダメージ
	
	GME_BOSS4_ACT_ID_ESCAPE,			//!< 逃亡

	GME_BOSS4_ACT_ID_ANGRY_L1,			//!< 怒り演出(その1)
	GME_BOSS4_ACT_ID_ANGRY_L2,			//!< 怒り演出(その2)
	GME_BOSS4_ACT_ID_ANGRY_L3,			//!< 怒り演出(その3)

	GME_BOSS4_ACT_ID_MAX

} GME_BOSS4_ACT_ID;

// ===========================================================================
//	enum GME_BOSS4_PART_IDX
// ---------------------------------------------------------------------------
//! ボス4 パーツインデックス列挙型
// ===========================================================================
typedef enum
{
	GME_BOSS4_PART_IDX_BODY	= 0,	// ボスマシン
	GME_BOSS4_PART_IDX_EGG,			// エッグマン
	GME_BOSS4_PART_IDX_MAX

} GME_BOSS4_PART_IDX;


// ===========================================================================
//	struct GME_BOSS4_PART_ACT_INFO
// ---------------------------------------------------------------------------
//! ボス4パーツアクション情報構造体
// ===========================================================================
typedef struct tag_GMS_BOSS4_PART_ACT_INFO
{
	Uint16		act_id;					//!< モーション番号（AMBインデックス）
	Uint8		is_maintain;			//!< 前のアクション継続
	Uint8		is_repeat;				//!< リピート
	Float		mtn_spd;				//!< モーション再生速度
	BOOL		is_blend;				//!< ブレンド有無
	Float		blend_spd;				//!< ブレンド速度
	BOOL		is_merge_manual;		//!< マニュアルマージ有無

} GMS_BOSS4_PART_ACT_INFO;



// ===========================================================================
//	struct GMS_BOSS4_MTN_SUSPEND_WORK
// ---------------------------------------------------------------------------
//! モーション再生停滞ワーク
// ===========================================================================
typedef struct tag_GMS_BOSS4_MTN_SUSPEND_WORK
{
	BOOL	is_suspended;
	Uint32	suspend_timer;
} GMS_BOSS4_MTN_SUSPEND_WORK;



// ===========================================================================
//	struct GMS_BOSS4_MGR_WORK
// ---------------------------------------------------------------------------
//! ボス4管理ワーク
// ===========================================================================
struct tag_GMS_BOSS4_MGR_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	Sint32				life;	// ボスライフ
	
	Uint32				flag;
	
	GMS_BOSS4_BODY_WORK	*body_work;

	Sint32				obj_create_cnt;	//!< オブジェクト生成カウント　11/6
};

typedef struct tag_GMS_BOSS4_MGR_WORK	GMS_BOSS4_MGR_WORK;


/*------ External Declarations -----------------------------------------*/
//############ 共通 ###########################################################
/* 補助関数 */

// =======================================================================
// gmBoss4SetPartTextureBurnt
/*!
  パーツのテクスチャを黒こげタイプにする
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  スロット0のテクスチャオフセットをu+0.5しています。
 */
// =======================================================================
void	gmBoss4SetPartTextureBurnt(OBS_OBJECT_WORK *obj_work, BOOL burn = true);

// =======================================================================
// gmBoss4IsScrollLockBusy
/*!
  スクロールロック発生と終了を監視
  
  @retval TRUE	スクロールロックされている
  @retval FALSE	スクロールロックされていない
 */
// =======================================================================
BOOL	gmBoss4IsScrollLockBusy(void);


// =======================================================================
// GmBoss4SetLife()
/*!
  ボス4 ボス本体の取得
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4GetBodyWork();

// =======================================================================
// GmBoss4SetLife()
/*!
  ボス4 ライフ設定

  @param life		[in]	ライフ
 */
// =======================================================================
extern void GmBoss4SetLife( Sint32 life );

// =======================================================================
// GmBoss4GetLife()
/*!
  ボス4 ライフ取得
 */
// =======================================================================
extern Sint32 GmBoss4GetLife();


// =======================================================================
// GmBoss4GetObj3D()
/*!
  ボス4 obj_3dモデル(OBS_ACTION3D_NN_WORK)の取得
 */
// =======================================================================
OBS_ACTION3D_NN_WORK*	GmBoss4GetObj3D(Sint32 n);


// =======================================================================
//	アクションの取得
// =======================================================================
const GMS_BOSS4_PART_ACT_INFO*	GmBoss4GetActInfo(Sint32 act, Sint32 parts);


// =======================================================================
// GmBoss4Build
/*!
  ボス4 データ構築
 */
// =======================================================================
extern void GmBoss4Build(void);


// =======================================================================
// GmBoss4Flush
/*!
  ボス１ データ片付け
 */
// =======================================================================
extern void GmBoss4Flush(void);


// =======================================================================
// GmBoss4IsBuildeded()
/*!
  ボス4 データ構築終了チェック
 */
// =======================================================================
extern BOOL GmBoss4IsBuilded(void);


// =======================================================================
// GmBoss4Init
/*!
  ボス１（管理）初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4Init(GMS_EVE_RECORD_EVENT *eve_rec,
									fx32 pos_x, fx32 pos_y, u8 type);


// =======================================================================
// gmBoss4ScrollInit
/*!
  強制スクロールイベント開始 ( イベント設定用 )
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
 */
// =======================================================================
extern OBS_OBJECT_WORK* GmBoss4ScrollInit(GMS_EVE_RECORD_EVENT *eve_rec,
									fx32 pos_x, fx32 pos_y, u8 type);


// =======================================================================
// gmBoss4ScrollOff
/*!
  強制スクロールイベント 終了
  
 */
// =======================================================================
extern	void	GmBoss4ScrollOff();


// =======================================================================
// gmBoss4ScrrollOut()
/*!
  強制スクロールイベント 移動(ボス撃破の白フラッシュで使用する)
  
 */
// =======================================================================
void GmBoss4ScrollOut();


// =======================================================================
// gmBoss4ScrollNext
/*!
  強制スクロールイベント移行
  
 */
// =======================================================================
extern	void	GmBoss4ScrollNext();


// =======================================================================
// GmBoss4GetScrollOffset()
/*!
  強制スクロールのスクロール量を取得
  マップ戻す量なども一緒に入っている
 */
// =======================================================================
extern	fx32	GmBoss4GetScrollOffset();

// =======================================================================
// GmBoss4Is2ndStage()
/*!
	第２形態状態なのかをチェックします
 */
// =======================================================================
extern	BOOL	GmBoss4Is2ndStage();

// =======================================================================
// GmBoss4CheckBossRush()
/*!
	現在のステージからステージファイナルの
	ボスラッシュかどうかをチェックします
  
 */
// =======================================================================
BOOL	GmBoss4CheckBossRush();


// =======================================================================
// GmBoss4CheckBossRus
/*!
	ボスラッシュ用開放用
  
 */
// =======================================================================
void GmBoss4IncObjCreateCount();
void GmBoss4DecObjCreateCount();
BOOL GmBoss4IsAllCreatedObjDeleted();

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

#endif /* GM_BOSS_4_H_ */
