// ===========================================================================
/*!
	@file	gmCockpit.h
	@brief	コックピット

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: gmCockpit.h 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

/* 重複インクルード回避手法 */

#ifndef GM_COCKPIT_H_
#define GM_COCKPIT_H_

#if	defined(__cplusplus)
extern "C" {
#endif


// ----- Include Files ---------------------------------------（インクルード）

// ----- Macros ------------------------------------------------（マクロ定義）
/*
// 3DES初期化設定フラグ
#define GMD_COCKPIT_3DES_FLAG_NOFLIP				(1 << 0)	//!< フリップ反映しない
#define GMD_COCKPIT_3DES_FLAG_STICKPARENT		(1 << 1)	//!< 親に付随
#define GMD_COCKPIT_3DES_FLAG_SCALE_BY_MTX		(1 << 2)	//!< 行列でスケーリング（モデル使用エフェクトなどで使用。TYPE_EMTでは正常に反映されません）
#define GMD_COCKPIT_3DES_FLAG_ENABLE_DIR			(1 << 4)	//!< オブジェクトのdirで回転する
*/
/*! TYPE_EMT系の場合にデータのRotationを適用する（フラグオフの場合はデータ側の回転は無視されます。
  フラグオンでもプログラム側で設定したクォータニオンの左側にデータ側の回転が乗算されてしまう為、
  ES上での姿勢そのままで表示する場合など、プログラム側でエミッターの回転を制御しない場合での使用を推奨します。）
 */

//#define GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT	(1 << 5)

/* 定義値 */
//#define GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE		(-1)		//!< 生成パラメータモデルインデックス 不使用指定マーク

// ----- Macro Functions -----------------------------------（処理マクロ定義）

#if defined(MTD_DEBUG)
#define GMM_COCKPIT_CREATE_WORK(work_size, parent_obj, sort_prio, name) (GmCockpitCreateWork(work_size, parent_obj, sort_prio, name))
#else
#define GMM_COCKPIT_CREATE_WORK(work_size, parent_obj, sort_prio, name) (GmCockpitCreateWork(work_size, parent_obj, sort_prio))
#endif /* defined(MTD_DEBUG) */

// ----- Definitions -------------------------------------------（定数の宣言）
//! コックピットワーク共通部
typedef struct tag_GMS_COCKPIT_COM_WORK
{
	OBS_OBJECT_WORK				obj_work;	//!< オブジェクトワーク
	
} GMS_COCKPIT_COM_WORK;

//! 2D コックピットワーク
typedef struct tag_GMS_COCKPIT_2D_WORK
{
	GMS_COCKPIT_COM_WORK		cpit_com;
	
	OBS_ACTION2D_AMA_WORK		obj_2d;		//!< 2Dオブジェクト
} GMS_COCKPIT_2D_WORK;

//! 3DNN コックピットワーク
typedef struct tag_GMS_COCKPIT_3DNN_WORK
{
	GMS_COCKPIT_COM_WORK		cpit_com;
	
	OBS_ACTION3D_NN_WORK		obj_3d;		//!< 3DNNオブジェクト
} GMS_COCKPIT_3DNN_WORK;



// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ==========================================================================
// 初期化 終了処理
// ==========================================================================
// ==========================================================================
// GmCockpitInit
/*!
 *	コックピット関連 初期化
 *
 *	@note
 *		コックピット関連の初期化を一括して行います
 */
// =========================================================================
extern void GmCockpitInit(void);

// ==========================================================================
// GmCockpitExit
/*!
 *	コックピット関連 終了処理
 *
 *	@note
 *		コックピット関連の終了処理を一括して行います
 */
// ==========================================================================
extern void GmCockpitExit(void);

// ==========================================================================
// コックピットワーク
// ==========================================================================
// ==========================================================================
// GmCockpitCreateWork
/*!
 *	コックピットワークの作成・初期化
 *
 *	@param	work_size	[in]	取得するTCBワークサイズ
 *	@param	parent_obj	[in]	親オブジェクト(NULL可)
 *	@param	sort_prio	[in]	エフェクトソート用のプライオリティ
 *	@param	name		[in]	デバッグ用TCB名(NULL可)
 *
 *	@return	取得したOBS_OBJECT_WORKワークアドレス
 *
 *	@note
 *		指定サイズのワークを持つオブジェクト管理TCBを作成し\n
 *		OBS_OBJECT_WORKの基本設定を行います。\n
 *		work_sizeは sizeof(OBS_OBJECT_WORK) 以上の値を設定して下さい。\n
 *		親オブジェクトが存在する場合は、初期座標を親オブジェクトの座標に設定します。\n
 *		矩形登録無し クリップ無し 当たり無し で設定します
 */
// ==========================================================================
#if defined(MTD_DEBUG)
extern OBS_OBJECT_WORK* GmCockpitCreateWork(Uint32 work_size, OBS_OBJECT_WORK *parent_obj, u16 sort_prio, const char *name);
#else
extern OBS_OBJECT_WORK* GmCockpitCreateWork(Uint32 work_size, OBS_OBJECT_WORK *parent_obj, u16 sort_prio);
#endif /* defined(MTD_DEBUG) */



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

#endif /* GM_COCKPIT_H_ */
