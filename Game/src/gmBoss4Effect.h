// =======================================================================
/*!
  @file	gmBoss4.h
  @brief ボス4 Effect

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Effect.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_BOSS_4_EFFECT_H_
#define GM_BOSS_4_EFFECT_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/
#include "gmEffect.h"
#include "gmBossCommon.h"
#include "gmBoss4Util.h"

/*------ Macros --------------------------------------------------------*/
//############ 爆発エフェクト #################################################
/* フラグ */
/* 定義値 */
#define GMD_BOSS4_EFF_BOMB_OFST_Z					((fx32)(FX32_ONE * 32))	//!< 爆発エフェクト表示座標の親からのオフセット座標Z（ボスより手前に表示させるため一律にずらす）

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! 爆発エフェクト タイプ列挙型
typedef enum
{
	GME_BOSS4_EFF_BOMB_TYPE_SMALL	= 0,	// 小
	GME_BOSS4_EFF_BOMB_TYPE_CAPSULE,
	GME_BOSS4_EFF_BOMB_TYPE_CHIBI_01,
	GME_BOSS4_EFF_BOMB_TYPE_CHIBI_02,
	GME_BOSS4_EFF_BOMB_TYPE_CHIBI_03,
	GME_BOSS4_EFF_BOMB_TYPE_PARTS,
	
	GME_BOSS4_EFF_BOMB_TYPE_MAX
} GME_BOSS4_EFF_BOMB_TYPE;

//! 爆発エフェクトワーク
typedef struct tag_GMS_BOSS4_EFF_BOMB_WORK
{
	OBS_OBJECT_WORK	*parent_obj;
	GME_BOSS4_EFF_BOMB_TYPE	bomb_type;
	Uint32	interval_timer;
	Uint32	interval_min;
	Uint32	interval_max;
	fx32	pos[MTD_XY];
	fx32	area[MTD_WH];
#if _IPHONE
	Sint32	interval_timer_sound;
#endif // _IPHONE
} GMS_BOSS4_EFF_BOMB_WORK;

//! 衝撃波エフェクトワーク
typedef struct tag_GMS_BOSS4_EFF_SHOCKWAVE_WORK
{
	GMS_EFFECT_3DES_WORK	eff_3des;
	
	Uint32					atk_rect_timer;	//!< 攻撃矩形有効時間タイマ
} GMS_BOSS4_EFF_SHOCKWAVE_WORK;

//! 衝撃波エフェクトサブパーツワーク
typedef struct tag_GMS_BOSS4_EFF_SHOCKWAVE_SUB_WORK
{
	GMS_EFFECT_3DES_WORK	eff_3des;
} GMS_BOSS4_EFF_SHOCKWAVE_SUB_WORK;

//! パーツ飛散エフェクト パーツ操作NDCワーク
typedef struct tag_GMS_BOSS4_EFF_SCT_PART_NDC_WORK
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	ncd_obj;
	
	AMS_QUAT	spin_quat;	//!< 回転差分クォータニオン
	BOOL		is_ironball;	//!< 鉄球タイプフラグ（TRUEだと回転が遅く設定される）
} GMS_BOSS4_EFF_SCT_PART_NDC_WORK;

//! 衝撃波エフェクトワーク
typedef struct tag_GMS_BOSS4_EFF_COMMON_WORK
{
	GMS_EFFECT_3DES_WORK	eff_3des;
	
	// リンク用のデータ
	GMS_BOSS4_NODE_MATRIX*	node_work;
	Sint32					link;
	NNS_VECTOR				ofs;			//!< リンクオフセット
	NNS_VECTOR				rot;			//!< リンクローテーション
	Uint32*					lookflag;		//!< 監視フラグ
	Uint32					lookmask;		//!< 監視パターン
} GMS_BOSS4_EFF_COMMON_WORK;


/*------ External Declarations -----------------------------------------*/

// =======================================================================
// GmBoss4Build
/*!
  ボス１ データ構築
 */
// =======================================================================
extern void GmBoss4EffectBuild(void);

// =======================================================================
// GmBoss4Flush
/*!
  ボス１ データ片付け
 */
// =======================================================================
extern void GmBoss4EffectFlush(void);


#if 0
// =======================================================================
// gmBoss4EffShockwaveInit
/*!
  衝撃波 初期化
  
  @param body_work	[io]	本体ワーク
  
  @return 3desエフェクトワーク
  
  @note
  真ん中パーツと、左右それぞれの衝撃波パーツで構成されます。
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* gmBoss4EffShockwaveInit(/*GMS_BOSS4_CHAIN_WORK*/ void* chain_work);

// =======================================================================
// gmBoss4EffScatterInit
/*!
  パーツ飛散エフェクト 初期化
  
  @param chain_work	[io]	鎖ワーク
  
  @note
  ノード操作オブジェクトを生成します。
 */
// =======================================================================
void gmBoss4EffScatterInit(/*GMS_BOSS4_CHAIN_WORK*/ void* chain_work);
#endif

// =======================================================================
// gmBoss4EffDamageInit
/*!
  ダメージエフェクト初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  破片の出るエフェクトです。
 */
// =======================================================================
void gmBoss4EffDamageInit(/*GMS_BOSS4_BODY_WORK*/ void* body_work);


// ############################################################################
// 爆発エフェクト
// ############################################################################
// =======================================================================
// GmBoss4EffBombInitCreate
/*!
  爆発エフェクト 生成処理初期化
 
  @param bomb_work		[io]	本体ワーク
  @param bomb_type		[in]	爆発タイプ
  @param pos_x			[in]	生成範囲中心座標X
  @param pos_y			[in]	生成範囲中心座標Y
  @param width			[in]	生成範囲幅
  @param height			[in]	生成範囲高さ
  @param interval_min	[in]	生成間隔最短時間
  @param interval_max	[in]	生成間隔最長時間
 */
// =======================================================================
void GmBoss4EffBombInitCreate(GMS_BOSS4_EFF_BOMB_WORK *bomb_work,
							  GME_BOSS4_EFF_BOMB_TYPE bomb_type,
							  OBS_OBJECT_WORK *parent_obj,
							  fx32 pos_x, fx32 pos_y, fx32 width, fx32 height,
							  Uint32 interval_min, Uint32 interval_max);

// =======================================================================
// gmBoss4EffBombUpdateCreate
/*!
  爆発エフェクト 生成処理更新
  
  @param bomb_work	[io]	爆発ワーク
  
  @note
  初期化時に指定されたパラメータで爆発エフェクトを生成します。
  生成期間中は毎フレーム呼び出してください。
 */
// =======================================================================
void GmBoss4EffBombUpdateCreate(GMS_BOSS4_EFF_BOMB_WORK *bomb_work);


// =======================================================================
// GmBoss4EffCommonInit
/*!
  エフェクト共通出力 初期化
  
  @param id			[in]	表示エフェクトID
  @param pos		[in]	表示ポジション(リンクの場合はオフセット)
  @param parent_obj	[in]	親オブジェクト
  @param type		[in]	エフェクトタイプ
  @param flag		[in]	エフェクトフラグ
  @param mtx		[in]	ノードワーク
  @param link		[in]	リンクノード番号
  @param rot		[in]	回転
  @param ctrl_flag	[in]	消去を行うためのフラグ
  @param mask		[in]	消去するためのBITパターン
  
  @return 3desエフェクトワーク
  
  @note
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* GmBoss4EffCommonInit(Sint32 id, VecFx32* pos, OBS_OBJECT_WORK* parent_obj=NULL,
										   GME_EFFECT_3DES_POS_TYPE type=GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
										   Uint32 flag=GMD_EFFECT_3DES_FLAG_NOFLIP,
										   GMS_BOSS4_NODE_MATRIX* mtx=NULL,
										   Sint32 link=-1, 
										   VecFx32* rot=NULL,
										   Uint32* ctrl_flag=NULL, Uint32 mask=0);

// =======================================================================
// GmBoss4EffChangeType
/*!
  エフェクトタイプの変更
  
  @param efct_work	[io]	エフェクトワーク
  @param type		[in]	エフェクトタイプ
  @param flag		[in]	エフェクトフラグ
  
  @note
	初期化した後、表示する前にタイプを変更する。
	自動的に高速スクロールに対応する
 */
// =======================================================================
void GmBoss4EffChangeType( GMS_EFFECT_3DES_WORK* efct_work, GME_EFFECT_3DES_POS_TYPE type, Uint32 init_flag );


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

#endif /* GM_BOSS_4_EFFECT_H_ */
