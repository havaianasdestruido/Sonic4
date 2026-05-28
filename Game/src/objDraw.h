// ==========================================================================
/*!
	@file objDraw.h
	@brief オブジェクト 描画

	@author mana
	@author modifier Ishizaki
				Copyright(c) 2007 Dimps

  $Id: objDraw.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */
/*!
  @page  obj_draw obj オブジェクト
 
    
  @sa objObject.c objObject.h objDraw.c objDraw.h
 */
#ifndef OBJ_DRAW_H_
#define OBJ_DRAW_H_

//----- Include Files -------------------------------------------------------
#include "objObject.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#if (OBD_USE_ACTION3D_NN)
#define OBD_DRAW_CMD_STATE_3DNN			(0)				//!< 描画コマンドステータス 3DNN
#define OBD_DRAW_CMD_STATE_PRE_MAPFAR	(1)				//!< 描画コマンドステータス 遠景用前処理
#define OBD_DRAW_CMD_STATE_MAPFAR		(2)				//!< 描画コマンドステータス 遠景用
#define OBD_DRAW_CMD_STATE_POST_MAPFAR	(3)				//!< 描画コマンドステータス 遠景用後処理
#define OBD_DRAW_CMD_STATE_WATER		(4)				//!< 描画コマンドステータス 水面
#define OBD_DRAW_CMD_STATE_WATER_BACK	(5)				//!< 描画コマンドステータス 奥の滝
#define OBD_DRAW_CMD_STATE_NEAR_MAP		(7)				//!< 描画コマンドステータス 超近景
#define OBD_DRAW_CMD_STATE_POST_WATER	(8)				//!< 描画コマンドステータス 水面後処理
#define OBD_DRAW_CMD_STATE_PRE_WATER	(9)				//!< 描画コマンドステータス 水面前処理
#define OBD_DRAW_CMD_STATE_3DFIX		(10)			//!< 描画コマンドステータス 3D描画FIX表示
#define OBD_DRAW_CMD_STATE_MAPMID		(11)			//!< 描画コマンドステータス 中景(z3-1, z3-3)
#define OBD_DRAW_CMD_STATE_WATER_MAPMID	(12)			//!< 描画コマンドステータス 中景前水

#if OBD_USE_ACTION2D_AMA
#define OBD_DRAW_CMD_STATE_2DAMA		(6)				//!< 描画ステータス 2D AMA
#endif	// #if OBD_USE_ACTION2D_AMA

#define OBD_DRAW_CMD_STATE_GMSG_WIN		(13)
#define OBD_DRAW_CMD_STATE_GMSG_MSG		(14)


#define OBD_DRAW_CMD_STATE_INVALID		(0xFFFFFFFF)	//!< 描画コマンドステータス 無効

#if _IPHONE
#define OBD_DRAW_CMD_STATE_3DNN_PRE		(15)			//!< 描画コマンドステータス 3DNN 装飾とか
#define OBD_DRAW_CMD_STATE_3DNN_POST	(16)			//!< 描画コマンドステータス 3DNN エフェクト系
#define OBD_DRAW_CMD_STATE_3DNN_WS		(17)			//!< 描画コマンドステータス 3DNN WaterSliderしぶき

#define OBD_DRAW_SET_NN_CMD_STATE_TBL_MSG_START	(16)	//!< 描画コマンドステータス設定テーブルメッセージ位置

#define OBD_DRAW_CMD_STATE_MAX			(18)				//!< 描画コマンドステータス 実行最大数
#else
#define OBD_DRAW_SET_NN_CMD_STATE_TBL_MSG_START	(13)	//!< 描画コマンドステータス設定テーブルメッセージ位置

#define OBD_DRAW_CMD_STATE_MAX			(15)				//!< 描画コマンドステータス 実行最大数
#endif // _IPHONE

/// 
enum {
	OBD_DRAW_USER_COMMAND_3DNN_MODEL	= 0,		//!< 3DNN モデル描画
	OBD_DRAW_USER_COMMAND_3DNN_MODEL_MATMTN,		//!< 3DNN モデル描画 マテリアルモーションつき
	OBD_DRAW_USER_COMMAND_3DNN_MOTION,				//!< 3DNN モーションTRS描画
	OBD_DRAW_USER_COMMAND_3DNN_MOTION_MATMTN,		//!< 3DNN モーションTRS描画 マテリアルモーションつき
	OBD_DRAW_USER_COMMAND_3DNN_SET_CAMERA,			//!< 3DNN カメラ設定
	OBD_DRAW_USER_COMMAND_3DNN_USER_FUNC,			//!< 3DNN ユーザー処理呼び出し
	OBD_DRAW_USER_COMMAND_3DNN_DRAW_MOTION,			//!< 3DNN モーション描画
	OBD_DRAW_USER_COMMAND_3DNN_DRAW_MOTION_MATMTN,	//!< 3DNN モーション描画 マテリアルモーションつき

	OBD_DRAW_USER_COMMAND_3DNN_MAX
};

enum {
	OBD_DRAW_USER_COMMAND_SORT_3DNN_MODEL	= 0,	//!< 3DNN モデル描画
	OBD_DRAW_USER_COMMAND_SORT_3DNN_MATMTN,

	OBD_DRAW_USER_COMMAND_SORT_3DNN_MAX
};


/// 描画スレッドユーザー処理
typedef void (*OBF_DRAW_USER_DT_FUNC)(void*);

/// objDraw3DNNModel_DT 描画スレッドユーザー処理パラメーター構造体
typedef struct tag_OBS_DRAW_PARAM_3DNN_USER_FUNC {
	OBF_DRAW_USER_DT_FUNC	func;					//!< ユーザー処理
	void					*param;					//!< ユーザー処理に受け渡すパラメーター
} OBS_DRAW_PARAM_3DNN_USER_FUNC;


/// 描画時ユーザー処理(描画スレッドでの呼び出し)
typedef void (*OBF_DRAW_USER_FUNC)(void*);

/// マテリアルコールバック型
typedef NNE_BOOL (*OBF_MATERIAL_CB)(NNS_DRAWCALLBACK_VAL *, void *);

/// 
// ================================================================
// OBF_DRAW_3DNN_MPLT_CB_FUNC
/*!
  マトリックスパレットCB関数（描画スレッドでの呼び出し）

  @param	mtx_plt			[io]	マトリックスパレット（計算済み）
  @param	object			[in]	NNオブジェクト
  @param	mplt_cb_param	[io]	パラメータ（amDrawMallocDataBuffer()で取得した領域を使用すること！）
 */
// ================================================================
typedef void (*OBF_DRAW_3DNN_MPLT_CB_FUNC)(NNS_MATRIX *mtx_plt, const NNS_OBJECT *object, void *mplt_cb_param);

/// 
// ================================================================
// OBF_DRAW_3DNN_MOTION_CB_FUNC
/*!
  モーションCB関数（メインスレッドでの呼び出し）

  @param	motion			[in]	モーションデータ（計算済み）
  @param	object			[in]	NNオブジェクト
  @param	mtn_cb_param	[io]	パラメータ
 */
// ================================================================
typedef void (*OBF_DRAW_3DNN_MOTION_CB_FUNC)(const AMS_MOTION *motion, const NNS_OBJECT *object, void *mtn_cb_param);

#endif	// #if (OBD_USE_ACTION3D_NN)

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// 描画初期化
// ==========================================================================
// ==========================================================================
// ObjDrawInit
/*!
 *	描画システム初期化
 */
// ==========================================================================
extern void ObjDrawInit(void);

// ==========================================================================
// ObjDrawESEffectSystemInit
/*!
 *	ESエフェクトシステム初期化
 *	
 *	@param	pause_level	[in]	エフェクトサーバータスクのポーズレベル
 *	@param	task_prio	[in]	エフェクトサーバータスクのタスクプライオリティ
 *	@param	group		[in]	エフェクトサーバータスクのグループ
 */
// ==========================================================================
extern void ObjDrawESEffectSystemInit(u16 pause_level, u32 task_prio, u32 group);

// ==========================================================================
// ObjDrawESEffectSystemIsActive
/*!
 *	ESエフェクトシステムが動作しているかチェック
 *	
 *	@retval	TRUE	動作中
 *	@retval	FALSE	停止中（未初期化）
 */
// ==========================================================================
extern BOOL ObjDrawESEffectSystemIsActive(void);

// ==========================================================================
// 描画終了
// ==========================================================================
// ==========================================================================
// ObjDrawExit
/*!
 *	描画システム終了
 */
// ==========================================================================
extern void ObjDrawExit(void);

// ==========================================================================
// ObjDrawESEffectSystemExit
/*!
 *	ESエフェクトシステム終了
 *	
 *	@note
 *		エフェクトサーバーを終了します。
 */
// ==========================================================================
extern void ObjDrawESEffectSystemExit(void);

// ==========================================================================
// オブジェクト各種設定
// ==========================================================================
// ==========================================================================
// ObjDrawPrioritySet
/*!
 *	アクション設定関数
 *
 *	@param obj_work	[in]	ゲームオブジェクトワークポインタ
 *	@param prio		[in]	設定する描画優先度 0 最手前 ～ 31最奥
 */
// ==========================================================================
extern void ObjDrawPrioritySet(OBS_OBJECT_WORK *obj_work, u32 prio);

#if (OBD_USE_ACTION2D | OBD_USE_ACTION3D_SS | OBD_USE_ACTION3D_POLY | OBD_USE_ACTION3D_SMA)
// ================================================================
// ObjDrawActionPrioritySet
/*!
  アクション設定関数

  @param pAct   [in] アクションポインタ
  @param usPrio [in] 設定する描画優先度 0 最手前 ～ 31最奥

 */
// ================================================================
extern void ObjDrawActionPrioritySet( MTS_ACTION *pAct, u32 usPrio );
#endif	// #if (OBD_USE_ACTION2D | OBD_USE_ACTION3D_SS | OBD_USE_ACTION3D_POLY | OBD_USE_ACTION3D_SMA)

#if defined _DS
// ==========================================================================
// ObjDrawBgPrioritySet
/*!
 *	アクション設定関数
 *
 *	@param obj_work	[in]	ゲームオブジェクトワークポインタ
 *	@param prio		[in]	設定する描画BG優先度 0 最手前 ～ 3最奥
 */
// ==========================================================================
extern void ObjDrawBgPrioritySet(OBS_OBJECT_WORK *obj_work, u32 prio);
#endif // #if defined _DS

#if (OBD_USE_ACTION2D)
// ================================================================
// ObjDrawActionBgPrioritySet
/*!
  アクション設定関数

  @param pAct   [in] アクションポインタ
  @param usPrio [in] 設定する描画BG優先度 0 最手前 ～ 3最奥

 */
// ================================================================
extern void ObjDrawActionBgPrioritySet( MTS_ACTION *pAct, u32 usPrio );
#endif

// ================================================================
// ObjDrawObjectActionSet
/*!
  アクション設定関数

  @param pObj   [in] オブジェクトワークポインタ
  @param usID [in] 設定するアクションID

 */
// ================================================================
extern void ObjDrawObjectActionSet( OBS_OBJECT_WORK *pObj, s32 usID );

#if OBD_USE_ACTION3D_NN
// ================================================================
// ObjDrawObjectActionSet3DNN
/*!
  アクション設定関数 3D NN

	@param	obj_work	[in] オブジェクトワークポインタ
	@param	id			[in] 設定するアクションID
	@param	mbuf_id		[in] 3Dモーション バッファID
 */
// ================================================================
extern void ObjDrawObjectActionSet3DNN(OBS_OBJECT_WORK *obj_work, s32 id, s32 mbuf_id);

// ================================================================
// ObjDrawAction3dActionSet3DNN
/*!
  アクション設定関数 3D NN

	@param	obj_3d		[in] 3Dオブジェクト描画ワークポインタ
	@param	id			[in] 設定するアクションID
	@param	mbuf_id		[in] 3Dモーション バッファID
 */
// ================================================================
extern void ObjDrawAction3dActionSet3DNN(OBS_ACTION3D_NN_WORK *obj_3d, s32 id, s32 mbuf_id);

// ================================================================
// ObjDrawObjectActionSet3DNNBlend
/*!
  アクション設定関数 3D NN モーションブレンドあり

	@param	obj_work	[in] オブジェクトワークポインタ
	@param	id			[in] 設定するアクションID

	@note
		モーションバッファ 0 のアクションを 1に移動し、
		新規設定のアクションをモーションバッファ 0に設定します。
		モーションブレンド速度は blend_spd に設定しておく必要があります。
 */
// ================================================================
extern void ObjDrawObjectActionSet3DNNBlend(OBS_OBJECT_WORK *obj_work, s32 id);

// ================================================================
// ObjDrawAction3dActionSet3DNNBlend
/*!
  アクション設定関数 3D NN モーションブレンドあり

	@param	obj_3d		[in] 3Dオブジェクト描画ワークポインタ
	@param	id			[in] 設定するアクションID

	@note
		モーションバッファ 0 のアクションを 1に移動し、
		新規設定のアクションをモーションバッファ 0に設定します。
		モーションブレンド速度は blend_spd に設定しておく必要があります。
 */
// ================================================================
extern void ObjDrawAction3dActionSet3DNNBlend(OBS_ACTION3D_NN_WORK *obj_3d, s32 id);

// ================================================================
// ObjDrawObjectActionSet3DNNMaterial
/*!
  アクション設定関数 3D NN マテリアルモーション

	@param	obj_work	[in] オブジェクトワークポインタ
	@param	id			[in] 設定するアクションID
 */
// ================================================================
extern void ObjDrawObjectActionSet3DNNMaterial(OBS_OBJECT_WORK *obj_work, s32 id);

// ================================================================
// ObjDrawAction3dActionSet3DNNMaterial
/*!
  アクション設定関数 3D NN マテリアルモーション

	@param	obj_3d		[in] 3Dオブジェクト描画ワークポインタ
	@param	id			[in] 設定するアクションID
 */
// ================================================================
extern void ObjDrawAction3dActionSet3DNNMaterial(OBS_ACTION3D_NN_WORK *obj_3d, s32 id);

#endif

// ==========================================================================
// ObjDrawActionGet
/*!
 *	アクション番号取得関数
 *
 *	@param obj_work	[in]	ゲームオブジェクトワークポインタ
 *
 *	@return  設定されているアクションID
 */
// ==========================================================================
extern s32 ObjDrawActionGet(OBS_OBJECT_WORK *obj_work);

#if OBD_USE_ACTION3D_NN
// ==========================================================================
// ObjDrawActionGet3DNN
/*!
 *	アクション番号取得関数
 *
 *	@param obj_work	[in]	ゲームオブジェクトワークポインタ
 *	@param mbuf_id	[in]	3Dモーション使用モーションバッファID
 *
 *	@return  設定されているアクションID
 */
// ==========================================================================
extern s32 ObjDrawActionGet3DNN(OBS_OBJECT_WORK *obj_work, s32 mbuf_id);
#endif

// ==========================================================================
// アクション 解凍・転送分離
// ==========================================================================
#if OBD_USE_ACTION2D
// ==========================================================================
// ObjDrawSetActionActUncomp
/*!
 *	アクション キャラクター 解凍・転送 分離用設定
 *
 *	@param obj_2d		[in]	アクションオブジェクトワークポインタ
 *	@param act_uncomp	[in]	解凍・転送分離ワーク
 *	@param cha_size		[in]	キャラクタサイズ(OBD_AUTO_CHARSIZE 可)
 *
 *	@note
 *		解凍領域を自動取得する場合は、\n
 *		この関数を実行する前にアクションをロードしておく必要があります\n
 *		設定したアクションは、アクションコールバック内で ObjDrawTransActUncomp を呼び出してください
 */
// ==========================================================================
extern void ObjDrawSetActionActUncomp(OBS_ACTION2D_WORK *obj_2d, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 cha_size);
#endif

#if OBD_USE_ACTION3D_SPR
// ==========================================================================
// ObjDrawSet3dSpriteActUncomp
/*!
 *	3Dスプライト キャラクター 解凍・転送 分離用設定
 *
 *	@param obj_work		[in]	ゲームオブジェクトワークポインタ
 *	@param act_uncomp	[in]	解凍・転送分離ワーク
 *	@param tex_size		[in]	テクスチャサイズ(OBD_AUTO_CHARSIZE 可)
 *
 *	@return  設定されているアクションID
 *
 *	@note
 *		解凍領域を自動取得する場合は、\n
 *		この関数を実行する前にアクションをロードしておく必要があります\n
 *		設定したアクションは、アクションコールバック内で ObjDrawTransActUncomp を呼び出してください
 */
// ==========================================================================
extern void ObjDrawSet3dSpriteActUncomp(OBS_ACTION3D_SPRITE_WORK *obj_3dspr, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 tex_size);
#endif

#if OBD_USE_ACTION3D_SS
// ==========================================================================
// ObjDrawSetSoftwareSpriteActUncomp
/*!
 *	ソフトウェアスプライト キャラクター 解凍・転送 分離用設定
 *
 *	@param obj_3dss		[in]	ソフトウェアスプライトオブジェクトワークポインタ
 *	@param act_uncomp	[in]	解凍・転送分離ワーク
 *	@param tex_size		[in]	テクスチャサイズ(OBD_AUTO_CHARSIZE 可)
 *
 *	@note
 *		解凍領域を自動取得する場合は、\n
 *		この関数を実行する前にアクションをロードしておく必要があります\n
 *		設定したアクションは、アクションコールバック内で ObjDrawTransActUncomp を呼び出してください
 */
// ==========================================================================
extern void ObjDrawSetSoftwareSpriteActUncomp(OBS_ACTION3D_SS_WORK *obj_3dss, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 tex_size);
#endif // #if OBD_USE_ACTION3D_SS

#if OBD_USE_ACTION3D_SMA
// ==========================================================================
// ObjDrawSet3DSmaActUncomp
/*!
 *	3D SMA キャラクター 解凍・転送 分離用設定
 *
 *	@param obj_3dsma	[in]	3D SMAオブジェクトワークポインタ
 *	@param act_uncomp	[in]	解凍・転送分離ワーク
 *	@param tex_size		[in]	テクスチャサイズ(OBD_AUTO_CHARSIZE 可)
 *
 *	@note
 *		解凍領域を自動取得する場合は、\n
 *		この関数を実行する前にアクションをロードしておく必要があります\n
 *		設定したアクションは、アクションコールバック内で ObjDrawTransActUncomp を呼び出してください
 */
// ==========================================================================
extern void ObjDrawSet3DSmaActUncomp(OBS_ACTION3D_SMA_WORK *obj_3dsma, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 tex_size);
#endif // #if OBD_USE_ACTION3D_SMA

#if OBD_USE_ACTION2D
// ==========================================================================
// ObjDrawTransActUncomp
/*!
 *	解凍済みアクションデータの転送
 *
 *	@param	cmd			[in]	コマンドデータ
 *	@param	act			[io]	アクションデータ
 *	@param	user_data	[in]	解凍・転送分離ワーク
 *
 *	@note
 *		アクション解凍・転送分離を行う場合に、アクションコールバックから呼び出してください。
 */
// ==========================================================================
extern void ObjDrawTransActUncomp(const MTS_ACT_COMMAND *cmd, MTS_ACTION *act, OBS_ACTION_UNCOMP_WORK *act_uncomp);
#endif

// ==========================================================================
// 標準関数
// ==========================================================================
#if defined _DS
// ==========================================================================
// ObjDrawActionCallBack
/*!
 *	アクションコールバック関数
 *
 *	@param	cmd			[io]	コマンドデータ
 *	@param	act			[io]	アクションデータ
 *	@param	user_data	[in]	ユーザーデータ
 *
 *	@note
 *		ppActCallに設定する標準関数です。\n
 *		必要に応じてppActCallに登録する関数を作成して下さい。
 */
// ==========================================================================
extern void ObjDrawActionCallBack(const MTS_ACT_COMMAND *cmd, MTS_ACTION *act, u32 user_data);
#endif

// ================================================================
// ObjDrawActionSummary
/*!
	パーツ付き3d+2dのオブジェクト表示まとめ

	@param	pWork	[in] オブジェクトワークポインタ

	@note
		ppOutに設定する標準関数です。\n
		必要に応じてppOutに登録する関数を作成して下さい。
 */
// ================================================================
extern void ObjDrawActionSummary(OBS_OBJECT_WORK *pWork);

// ================================================================
// 描画
// ================================================================
#if OBD_USE_ACTION2D
// ==========================================================================
// ObjDrawObjectAction
/*!
 *	オブジェクトアクション
 *
 *	@param obj_work	[in]	ゲームオブジェワークポインタ
 *	@param act_spr	[in]	アクションポインタ
 */
// ==========================================================================
extern void ObjDrawObjectAction(OBS_OBJECT_WORK *obj_work, MTS_ACTION_DS *act_spr);
#endif

#if OBD_USE_ACTION2D
// ================================================================
// ObjDrawAction
/*!
  オブジェクトアクション

  @param pAct      [io] アクションポインタ 
  @param vPos      [in] オブジェクト座標 （ NULL可 ）
  @param vDir      [in] オブジェクト角度 （ NULL可 ）
  @param vScale    [in] オブジェクト拡大率 （ NULL可 ）
  @param pDispFlag [io] 表示フラグポインタ（ NULLの場合、標準扱い ）
  @param pActCall      [in] アクションコールバック （ NULL可 ）
  @param ulActCallAddr [in] アクションコールバック引数アドレス （ NULL可 ）
 */
// ================================================================
extern void ObjDrawAction( MTS_ACTION_DS * pAct, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, MTF_ACT_CMD_CB pActCall, u32 ulActCallAddr );
#endif

#if (OBD_USE_ACTION3D_NNS || OBD_USE_ACTION3D_1M1S || OBD_USE_ACTION3D_SPR)
// ==========================================================================
// ObjDrawObjectAction3D
/*!
 *	オブジェクトアクション 3D
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param act_3d		[in]	アクションポインタ
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
extern void ObjDrawObjectAction3D(OBS_OBJECT_WORK *obj_work, MTS_ACTION3D *act_3d, OBS_ACTION_UNCOMP_WORK *act_uncomp);
#endif

#if (OBD_USE_ACTION3D_NNS || OBD_USE_ACTION3D_1M1S || OBD_USE_ACTION3D_SPR)
// ================================================================
// ObjDrawAction3D
/*!
  オブジェクトアクション
 *	@param pAct			[in]	アクション3Dポインタ
 *	@param vPos			[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir			[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale		[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pSimpleList	[in]	オブジェクト3D1m1s表示ワークポインタ
 *	@param pActCall		[in]	アクションコールバック （ NULL可 ）
 *	@param ulActCallAddr[in]	アクションコールバック引数アドレス （ NULL可 ）
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ================================================================
extern void ObjDrawAction3D( MTS_ACTION3D * pAct, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, OBS_ACTION3D_SIMPLE_WORK *pSimpleList, MTF_ACT_CMD_CB pActCall, u32 ulActCallAddr, OBS_ACTION_UNCOMP_WORK *act_uncomp);
#endif

#if OBD_USE_ACTION3D_SS
// ==========================================================================
// ObjDrawObjectActionSS
/*!
 *	オブジェクトソフトウェアスプライトアクション
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param act_spr		[in]	アクションポインタ
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
//extern void ObjDrawObjectActionSS(OBS_OBJECT_WORK *obj_work, MTS_ACTION_SS *act_ss, OBS_ACTION_UNCOMP_WORK *act_uncomp);
extern void ObjDrawObjectActionSS(OBS_OBJECT_WORK *obj_work, MTS_ACTION_SS *act_ss, OBS_ACTION_SP_SETTING_WORK *sp_setting);

// ==========================================================================
// ObjDrawActionSS
/*!
 *	オブジェクトアクション
 *
 *	@param pActSS			[io]	アクションポインタ 
 *	@param vPos				[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir				[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale			[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag		[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pActCall			[in]	アクションコールバック （ NULL可 ）
 *	@param ulActCallAddr	[in]	アクションコールバック引数アドレス （ NULL可 ）
 *	@param act_uncomp		[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
//extern void ObjDrawActionSS( MTS_ACTION_SS * pActSS, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, MTF_ACT_CMD_CB pActCall, u32 ulActCallAddr, OBS_ACTION_UNCOMP_WORK *act_uncomp);
extern void ObjDrawActionSS( MTS_ACTION_SS * pActSS, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, MTF_ACT_CMD_CB pActCall, u32 ulActCallAddr, OBS_ACTION_SP_SETTING_WORK *sp_setting);
#endif // #if OBD_USE_ACTION3D_SS

#if OBD_USE_ACTION3D_POLY
// ==========================================================================
// ObjDrawObjectActionPoly
/*!
 *	オブジェクトポリゴンアクションアクション
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param act_spr		[in]	アクションポインタ
 *	@param sp_setting	[in]	テクスチャ解凍・転送分離用ワーク(NULL可) 現在無効
 */
// ==========================================================================
extern void ObjDrawObjectActionPoly(OBS_OBJECT_WORK *obj_work, IZS_PLA_ACTION *act_poly, OBS_ACTION_SP_SETTING_WORK *sp_setting);

// ==========================================================================
// ObjDrawActionPoly
/*!
 *	オブジェクトアクション
 *
 *	@param pActSS			[io]	アクションポインタ 
 *	@param vPos				[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir				[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale			[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag		[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pActCall			[in]	アクションコールバック （ NULL可 ）現在無効
 *	@param ulActCallAddr	[in]	アクションコールバック引数アドレス （ NULL可 ）現在無効
 *	@param act_uncomp		[in]	テクスチャ解凍・転送分離用ワーク(NULL可) 現在無効
 */
// ==========================================================================
extern void ObjDrawActionPoly(IZS_PLA_ACTION *act_poly, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, MTF_ACT_CMD_CB pActCall, u32 ulActCallAddr, OBS_ACTION_SP_SETTING_WORK *sp_setting);

#endif // #if OBD_USE_ACTION3D_POLY

#if OBD_USE_ACTION3D_SMA
// ==========================================================================
// ObjDrawObjectAction3DSma
/*!
 *	オブジェクト3D SMAアクション
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param act_3dsma	[in]	アクションポインタ
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
extern void ObjDrawObjectAction3DSma(OBS_OBJECT_WORK *obj_work, MTS_SMA *act_3dsma, OBS_ACTION_UNCOMP_WORK *act_uncomp);

// ==========================================================================
// ObjDrawAction3DSma
/*!
 *	オブジェクト3D SMAアクション
 *
 *	@param act_3dsma		[io]	アクションポインタ 
 *	@param vPos				[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir				[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale			[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag		[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pActCall			[in]	アクションコールバック （ NULL可 ）
 *	@param ulActCallAddr	[in]	アクションコールバック引数アドレス （ NULL可 ）
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
extern void ObjDrawAction3DSma(MTS_SMA *act_3dsma, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, OBF_SMA_CMD_CB pActCall, u32 ulActCallAddr, OBS_ACTION_UNCOMP_WORK *act_uncomp);
#endif // #if OBD_USE_ACTION3D_SMA


#if (OBD_USE_ACTION3D_NN)
// ==========================================================================
// ObjDrawClearNNCommandStateTbl
/*!
 *	オブジェクト NN 描画コマンドステートテーブル初期化
 *
 *	@note
 *		描画スレッドで実行されるコマンドステートを初期化します。\n
 *		OBD_DRAW_CMD_STATE_3DNN以外のコマンドが破棄されます。\n
 *		コマンドステートを再設定する際前に呼び出してください。\n
 */
// ==========================================================================
extern void ObjDrawClearNNCommandStateTbl(void);

// ==========================================================================
// ObjDrawSetNNCommandStateTbl
/*!
 *	オブジェクト NN 描画コマンドステート設定
 *
 *	@param tbl_no			[in]	設定するテーブルNO 0 ～ OBD_DRAW_CMD_STATE_MAX-1
 *	@param command_state	[in]	コマンドステート
 *	@param end_scene		[in]	amDrawEndScene 実行設定
 *
 *	@note
 *		描画スレッドで実行されるコマンドステートを設定します。\n
 *		テーブルNOの若い順から実行されます。
 */
// ==========================================================================
extern void ObjDrawSetNNCommandStateTbl(u32 tbl_no, u32 command_state, BOOL end_scene = TRUE);

// ==========================================================================
// ObjDrawNNStart
/*!
 *	オブジェクトアクション 描画開始
 *
 *	@param pAct			[in]	アクション3Dポインタ
 *	@param vPos			[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir			[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale		[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pSimpleList	[in]	オブジェクト3D1m1s表示ワークポインタ
 *	@param pActCall		[in]	アクションコールバック （ NULL可 ）
 *	@param ulActCallAddr[in]	アクションコールバック引数アドレス （ NULL可 ）
 *
 *	@note
 *		描画スレッドに登録されたコマンドを開始します。
 */
// ==========================================================================
extern void ObjDrawNNStart(void);

// ==========================================================================
// ObjDraw3DNNSetCamera
/*!
 *	オブジェクトアクション 3D NN カメラ設定
 *
 *	@param	camera_id	[in]	オブジェクトカメラID -1で仮カメラ
 *	@param	proj_type	[in]	射影タイプ NNE_PROJECTION_TYPE
 *	@param	task_prio	[in]	カメラ設定タスクプライオリティ
 *
 *	@note
 *		カメラを設定します
 */
// ==========================================================================
extern void ObjDraw3DNNSetCamera(s32 camera_id, NNE_PROJECTION_TYPE proj_type);

// ==========================================================================
// ObjDraw3DNNSetCameraEx
/*!
 *	オブジェクトアクション 3D NN カメラ設定
 *
 *	@param	camera_id		[in]	オブジェクトカメラID -1で仮カメラ
 *	@param	proj_type		[in]	射影タイプ NNE_PROJECTION_TYPE
 *	@param	command_state	[in]	コマンドステート
 *
 *	@note
 *		カメラを設定します\n
 *		コマンドステートをユーザー指定します。
 */
// ==========================================================================
extern void ObjDraw3DNNSetCameraEx(s32 camera_id, NNE_PROJECTION_TYPE proj_type, u32 command_state);

// ==========================================================================
// ObjDraw3DNNUserFunc
/*!
 *	オブジェクトアクション 3D NN ユーザー処理登録
 *
 *	@param	user_func		[in]	登録するユーザー処理
 *	@param	param			[in]	受け渡すパラメータバッファ
 *	@param	param_size		[in]	受け渡すパラメータサイズ
 *	@param	command_state	[in]	描画コマンドステート
 *
 *	@note
 *		描画スレッド中のユーザー処理を登録します。
 */
// ==========================================================================
extern void ObjDraw3DNNUserFunc(OBF_DRAW_USER_DT_FUNC user_func, void *param, s32 param_size, u32 command_state);

// ==========================================================================
// ObjDrawObjectAction3DNN
/*!
 *	オブジェクトアクション 3D NN
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param obj_3d		[in]	表示ワークポインタ
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 *		描画は、描画スレッドに必要情報を渡して行います。
 */
// ==========================================================================
extern void ObjDrawObjectAction3DNN(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_NN_WORK *obj_3d);

// ==========================================================================
// ObjDrawAction3DNN
/*!
 *	オブジェクトアクション 3D NN
 *
 *	@param pAct			[in]	アクション3Dポインタ
 *	@param vPos			[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir			[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale		[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 */
// ==========================================================================
extern void ObjDrawAction3DNN(OBS_ACTION3D_NN_WORK *obj_3d, VecFx32 *pos, VecU16 *dir, VecFx32 *scale, u32 *p_disp_flag);

// ==========================================================================
// ObjDrawAction3DNNMotionUpdate
/*!
 *	オブジェクトアクション 3D NN モーション更新
 *
 *	@param obj_3d		[in]	3DNN オブジェクトワーク
 *	@param p_disp_flag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 */
// ==========================================================================
extern void ObjDrawAction3DNNMotionUpdate(OBS_ACTION3D_NN_WORK *obj_3d, u32 *p_disp_flag);

// ==========================================================================
// ObjDrawAction3DNNMaterialUpdate
/*!
 *	オブジェクトアクション 3D NN マテリアルモーション更新
 *
 *	@param obj_3d		[in]	3DNN オブジェクトワーク
 *	@param p_disp_flag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 */
// ==========================================================================
extern void ObjDrawAction3DNNMaterialUpdate(OBS_ACTION3D_NN_WORK *obj_3d, u32 *p_disp_flag);

// ==========================================================================
// ObjDraw3DNNModel
/*!
 *	オブジェクトアクション 描画
 *
 *	@param object				[in]	オブジェクト
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。\n
 *		user_param は、ObjDraw3DNNModel呼び出し前にamDrawMallocDataBufferで取得して下さい。
 *		amDrawMallocDataBufferでのバッファ取得は、描画毎に再取得が必要です。
 */
// ==========================================================================
extern void ObjDraw3DNNModel(NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param,
				u32 command_state,
				OBF_MATERIAL_CB material_cb_func=NULL, void *material_cb_param=NULL,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_RGB *toon_rim_param=NULL, float toon_camouflage=0.0f);
#elif _WII
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_VECTOR *toon_light=NULL);
#else
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0);
#endif

// ==========================================================================
// ObjDraw3DNNModelMaterialMotion
/*!
 *	オブジェクトアクション マテリアルモーションつき 描画
 *
 *	@param object				[in]	オブジェクト
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。\n
 *		user_param は、ObjDraw3DNNModel呼び出し前にamDrawMallocDataBufferで取得して下さい。
 *		amDrawMallocDataBufferでのバッファ取得は、描画毎に再取得が必要です。
 */
// ==========================================================================
extern void ObjDraw3DNNModelMaterialMotion(NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param, u32 command_state,
				OBF_MATERIAL_CB material_cb_func=NULL, void *material_cb_param=NULL,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_RGB *toon_rim_param=NULL, float toon_camouflage=0.0f);
#elif _WII
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_VECTOR *toon_light=NULL);
#else
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0);
#endif

// ==========================================================================
// ObjDraw3DNNMotion
/*!
 *	オブジェクト3Dモーション 描画
 *
 *	@param motion				[in]	モーション
 *	@param object				[in]	オブジェクト
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param mplt_cb_func			[in]	マトリックスパレットCB関数
 *	@param mplt_cb_param		[in]	マトリックスパレットCBパラメータ(amDrawMallocDataBuffer)
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。
 */
// ==========================================================================
extern void ObjDraw3DNNMotion(AMS_MOTION *motion, NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param, u32 command_state,
				OBF_DRAW_3DNN_MPLT_CB_FUNC mplt_cb_func=NULL,
				void *mplt_cb_param=NULL,
				OBF_MATERIAL_CB material_cb_func=NULL, void *material_cb_param=NULL,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_RGB *toon_rim_param=NULL, float toon_camouflage=0.0f);
#elif _WII
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_VECTOR *toon_light=NULL);
#else
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0);
#endif

// ==========================================================================
// ObjDraw3DNNMotionMaterialMotion
/*!
 *	オブジェクト3Dモーション マテリアルモーションつき 描画
 *
 *	@param motion				[in]	モーション
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param mplt_cb_func			[in]	マトリックスパレットCB関数
 *	@param mplt_cb_param		[in]	マトリックスパレットCBパラメータ(amDrawMallocDataBuffer)
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。
 */
// ==========================================================================
extern void ObjDraw3DNNMotionMaterialMotion(AMS_MOTION *motion, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param, u32 command_state,
				OBF_DRAW_3DNN_MPLT_CB_FUNC mplt_cb_func/*=NULL*/,
				void *mplt_cb_param/*=NULL*/,
				OBF_MATERIAL_CB material_cb_func=NULL, void *material_cb_param=NULL,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_RGB *toon_rim_param=NULL, float toon_camouflage=0.0f);
#elif _WII
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_VECTOR *toon_light=NULL);
#else
				AMS_DRAWSTATE *draw_state=NULL, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0);
#endif

// ==========================================================================
// ObjDraw3DNNDrawMotion
/*!
 *	オブジェクト3Dモーション 描画
 *
 *	@param motion				[in]	モーション NNS_MOTION
 *	@param frame				[in]	モーションフレーム
 *	@param object				[in]	オブジェクト
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param mplt_cb_func			[in]	マトリックスパレットCB関数
 *	@param mplt_cb_param		[in]	マトリックスパレットCBパラメータ(amDrawMallocDataBuffer)
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。
 */
// ==========================================================================
extern void ObjDraw3DNNDrawMotion(NNS_MOTION *motion, float frame, NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param, u32 command_state,
				OBF_DRAW_3DNN_MPLT_CB_FUNC mplt_cb_func/*=NULL*/,
				void *mplt_cb_param/*=NULL*/,
				OBF_MATERIAL_CB material_cb_func/*=NULL*/, void *material_cb_param/*=NULL*/,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_RGB *toon_rim_param=NULL, float toon_camouflage=0.0f);
#elif _WII
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_VECTOR *toon_light=NULL);
#else
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0);
#endif

// ==========================================================================
// ObjDraw3DNNDrawMotionMaterialMotion
/*!
 *	オブジェクト3Dモーション マテリアルモーションつき 描画
 *
 *	@param motion				[in]	モーション NNS_MOTION
 *	@param frame				[in]	モーションフレーム
 *	@param object				[in]	オブジェクト
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param mplt_cb_func			[in]	マトリックスパレットCB関数
 *	@param mplt_cb_param		[in]	マトリックスパレットCBパラメータ(amDrawMallocDataBuffer)
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。
 */
// ==========================================================================
extern void ObjDraw3DNNDrawMotionMaterialMotion(NNS_MOTION *motion, float frame, NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param, u32 command_state,
				OBF_DRAW_3DNN_MPLT_CB_FUNC mplt_cb_func/*=NULL*/,
				void *mplt_cb_param/*=NULL*/,
				OBF_MATERIAL_CB material_cb_func/*=NULL*/, void *material_cb_param/*=NULL*/,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_RGB *toon_rim_param=NULL, float toon_camouflage=0.0f);
#elif _WII
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0,
				NNS_VECTOR *toon_light=NULL);
#else
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag=OBD_LIGHT_USE_FLAG_0);
#endif

#if _IPHONE
// ==========================================================================
// ObjDraw3DNNDrawPrimitive
/*!
 *	プリミティブオブジェクト  描画
 *
 *	@param prim		[in]	プリミティブ		AMS_PARAM_DRAW_PRIMITIVE
 *	@param command	[in]	描画コマンド		
 *  @param light	[in]	ライト使用フラグ	NNE_PRIM_LIGHT
 *	@param cull		[in]	カリングフラグ		NNE_PRIM_CULL
 *
 *	@note
 *		描画スレッドにプリミティブ描画処理を登録します。
 */
// ==========================================================================
extern void ObjDraw3DNNDrawPrimitive(AMS_PARAM_DRAW_PRIMITIVE *prim, u32 command = OBD_DRAW_CMD_STATE_3DNN, NNE_PRIM_LIGHT light = NNE_PRIM_LIGHT_DISABLE, NNE_PRIM_CULL cull = NNE_PRIM_CULL_NONE);
#endif //_IPHONE

// ==========================================================================
// マテリアルコールバック
// ==========================================================================
// ==========================================================================
// ObjDraw3DNNGetMaterialUserData
/*!
 *	オブジェクト3D NN マテリアルユーザーデータ取得
 *
 *	@param val		[in]	ドローコールバック変数
 *
 *	@return		ユーザーデータ
 */
// ==========================================================================
extern u32 ObjDraw3DNNGetMaterialUserData(NNS_DRAWCALLBACK_VAL *val);

// ==========================================================================
// Wii用 トゥーン設定
// ==========================================================================
// ==========================================================================
// ObjDrawObjectSetToon
/*!
 *	Wii用 トゥーンマテリアルコールバック設定
 *
 *	@param obj_3d			[in]	OBS_ACTION3D_NN_WORKワーク
 *
 *	@note
 *		Wii用トゥーンマテリアルコールバックを設定します。\n
 *		その他のターゲットでは空関数になります。\n
 *		material_cb_func, material_cb_param に設定が行われていても\n
 *		上書きしますので気をつけてください。
 */
// ==========================================================================
extern void ObjDrawSetToon(OBS_ACTION3D_NN_WORK *obj_3d);

// ==========================================================================
// ObjDrawObjectSetToon
/*!
 *	Wii用 トゥーンマテリアルコールバック設定
 *
 *	@param obj_work			[in]	オブジェクトワーク
 *
 *	@note
 *		Wii用トゥーンマテリアルコールバックを設定します。\n
 *		その他のターゲットでは空関数になります。\n
 *		material_cb_func, material_cb_param に設定が行われていても\n
 *		上書きしますので気をつけてください。
 */
// ==========================================================================
#define ObjDrawObjectSetToon(obj_work)	ObjDrawSetToon((obj_work)->obj_3d);

// ==========================================================================
// ObjDrawToonMaterialCallback
/*!
 *	Wii用 トゥーンマテリアルコールバック
 *
 *	@param val				[in]	NNS_DRAWCALLBACK_VAL構造体へのポインタ
 *	@param param			[in]	ユーザーパラメータ(使用しない)
 *
 *	@return		NNE_BOOL
 *
 *	@note
 *		Wii用 トゥーンマテリアルコールバックです。\n
 *		他のターゲットでは空関数になります。
 */
// ==========================================================================
extern NNE_BOOL ObjDrawToonMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);

#endif // #if (OBD_USE_ACTION3D_NN)

#if (OBD_USE_ACTION3D_ES)
// ==========================================================================
// ObjDrawObjectAction3DES
/*!
 *	オブジェクトアクション 3D ES
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param obj_3des		[in]	表示ワークポインタ
 *
 *	@note
 *		アニメーション更新はメインスレッドで行います。
 *		描画は、描画スレッドに必要情報を渡して行います。
 */
// ==========================================================================
extern void ObjDrawObjectAction3DES(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_ES_WORK *obj_3des);

// ==========================================================================
// ObjDrawAction3DES
/*!
 *	オブジェクトアクション 3D ES
 *
 *	@param obj_3des		[in]	3DES オブジェクトワーク
 *	@param pos			[in]	オブジェクト座標 （ NULL可 ）
 *	@param dir			[in]	オブジェクト角度 （ NULL可 ）
 *	@param scale		[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param p_disp_flag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 */
// ==========================================================================
extern void ObjDrawAction3DES(OBS_ACTION3D_ES_WORK *obj_3des, VecFx32 *pos, VecU16 *dir, VecFx32 *scale, u32 *p_disp_flag);

// ==========================================================================
// ObjDraw3DESEffect
/*!
 *	オブジェクトアクション 3D ES 描画
 *
 *	@param ecb				[in]	ECB（エフェクトコントロールブロック）
 *	@param texlist			[in]	テクスチャリスト
 *	@param command_state	[in]	コマンドステート
 *
 *	@note
 *		描画スレッドに描画処理を登録します。\n
 */
// ==========================================================================
extern void ObjDraw3DESEffect(AMS_AME_ECB *ecb, NNS_TEXLIST *texlist, Uint32 command_state);

// ==========================================================================
// ObjDraw3DESMatrixPush
/*!
 *	オブジェクトアクション 3D ES マトリックスプッシュ
 *
 *	@param mtx				[in]	プッシュするマトリックス
 *	@param command_state	[in]	コマンドステート
 *
 *	@note
 *		ESエフェクト描画用にマトリックスをプッシュし、\n
 *		ビュー行列を加味して3Dプリミティブ描画用マトリクスを設定します。
 */
// ==========================================================================
extern void ObjDraw3DESMatrixPush(NNS_MATRIX *mtx, Uint32 command_state);

// ==========================================================================
// ObjDraw3DESMatrixPop
/*!
 *	オブジェクトアクション 3D ES マトリックスポップ
 *
 *	@param command_state	[in]	コマンドステート
 *
 *	@note
 *		ESエフェクト描画用にマトリックスをポップします。\n
 */
// ==========================================================================
extern void ObjDraw3DESMatrixPop(Uint32 command_state);

// ==========================================================================
// ObjDrawKillAction3DES
/*!
 *	オブジェクトアクション 3D ES 終了
 *
 *	@param obj_work		[in]	3DESワーク
 *
 *	@note
 *		ESエフェクトを終了します。
 *		生成済みのパーティクルを即時に更新停止したり消去することはありません。
 *		寿命が無限に設定されているエフェクトを自然に終わらせたい時等に使用してください。
 */
// ==========================================================================
extern void ObjDrawKillAction3DES(OBS_OBJECT_WORK *obj_work);

// =======================================================================
// ObjDraw3DESSetCamera
/*!
  オブジェクトアクション 3D ES カメラ設定
  
  @param obj_3des	[in]	オブジェクト3DESワーク
  @param obj_mtx	[in]	エフェクト用のワールド変換行列
  
  @note
  エフェクト用にカメラ設定を行います。
  内部でObjDraw3DNNSetCamera()を呼んでいます。
  obj_mtxの値を参照して、エフェクトソート用のカメラを適切な座標に設定しています。
 */
// =======================================================================
extern void ObjDraw3DESSetCamera(OBS_ACTION3D_ES_WORK *obj_3des, const NNS_MATRIX *obj_mtx);
#endif // #if (OBD_USE_ACTION3D_ES)

#if OBD_USE_ACTION2D_AMA
// ==========================================================================
// ObjDrawObjectAction2DAMA
/*!
 *	オブジェクトアクション 2D AMA
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param obj_2d		[in]	表示ワークポインタ
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 *		描画は、描画スレッドに必要情報を渡して行います。
 */
// ==========================================================================
extern void ObjDrawObjectAction2DAMA(OBS_OBJECT_WORK *obj_work, OBS_ACTION2D_AMA_WORK *obj_2d);

// ==========================================================================
// ObjDrawAction2DAMA
/*!
 *	オブジェクトアクション 2D AMA
 *
 *	@param obj_2d		[in]	2DAMA オブジェクトワーク
 *	@param pos			[in]	オブジェクト座標 （ NULL可 ）
 *	@param dir			[in]	オブジェクト角度 （ NULL可 ）
 *	@param scale		[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param p_disp_flag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 */
// ==========================================================================
extern void ObjDrawAction2DAMA(OBS_ACTION2D_AMA_WORK *obj_2d, VecFx32 *pos, VecU16 *dir, VecFx32 *scale, u32 *p_disp_flag);

// ==========================================================================
// ObjDrawAction2DAMADrawStart
/*!
 *	オブジェクトアクション 2D AMA 描画開始
 *
 *	@note
 *		ObjDrawAction2DAMA等で登録された描画命令をソートして描画します。
 */
// ==========================================================================
extern void ObjDrawAction2DAMADrawStart(void);

#endif // #if OBD_USE_ACTION2D_AMA

// ==========================================================================
// ライト設定
// ==========================================================================
#if OBD_USE_ACTION3D_NN
// ==========================================================================
// ObjDrawSetParallelLight
/*!
 *	パラレルライト 設定
 *
 *	@param light_no		[in]	ライトNO
 *	@param col			[in]	カラー設定
 *	@param intensity	[in]	輝度
 *	@param vec			[in]	ライトベクトル設定
 *
 *	@note
 *		light_noのライトをパラレルライトで設定します。\n
 *		ライトのON・OFF、反映は別途行ってください。
 */
// ==========================================================================
extern void ObjDrawSetParallelLight(NNE_LIGHT light_no, NNS_RGBA *col, float intensity, NNS_VECTOR *vec);

// ==========================================================================
// ObjDrawSetPointLight
/*!
 *	ポイントライト 設定
 *
 *	@param light_no		[in]	ライトNO
 *	@param col			[in]	カラー設定
 *	@param intensity	[in]	輝度
 *	@param pos			[in]	ライト位置設定
 *	@param falloffstart	[in]	距離減衰開始
 *	@param falloffend	[in]	距離減衰終了
 *
 *	@note
 *		light_noのライトをポイントライトで設定します。\n
 *		ライトのON・OFF、反映は別途行ってください。
 */
// ==========================================================================
extern void ObjDrawSetPointLight(NNE_LIGHT light_no, NNS_RGBA *col, float intensity, NNS_VECTOR *pos, float falloffstart, float falloffend);

// ==========================================================================
// ObjDrawSetTargetSpotLight
/*!
 *	ターゲットスポットライト 設定
 *
 *	@param light_no		[in]	ライトNO
 *	@param col			[in]	カラー設定
 *	@param intensity	[in]	輝度
 *	@param pos			[in]	ライト位置設定
 *	@param target		[in]	ターゲット位置
 *	@param innerangle	[in]	減衰開始角度
 *	@param outerangle	[in]	減衰終了角度
 *	@param falloffstart	[in]	距離減衰開始
 *	@param falloffend	[in]	距離減衰終了
 *
 *	@note
 *		light_noのローテーションスポットライトで設定します。\n
 *		ライトのON・OFF、反映は別途行ってください。
 */
// ==========================================================================
extern void ObjDrawSetTargetSpotLight(NNE_LIGHT light_no, NNS_RGBA *col, float intensity, NNS_VECTOR *pos, NNS_VECTOR *target,
							Angle32 innerangle, Angle32 outerangle, float falloffstart, float falloffend);

// ==========================================================================
// ObjDrawSetRotationSpotLight
/*!
 *	ローテーションスポットライト 設定
 *
 *	@param light_no		[in]	ライトNO
 *	@param col			[in]	カラー設定
 *	@param intensity	[in]	輝度
 *	@param pos			[in]	ライト位置設定
 *	@param rottype		[in]	ローテーションタイプ NNE_ROTATETYPE
 *	@param rotation		[in]	回転設定設定
 *	@param innerangle	[in]	減衰開始角度
 *	@param outerangle	[in]	減衰終了角度
 *	@param falloffstart	[in]	距離減衰開始
 *	@param falloffend	[in]	距離減衰終了
 *
 *	@note
 *		light_noのローテーションスポットライトで設定します。\n
 *		ライトのON・OFF、反映は別途行ってください。
 */
// ==========================================================================
extern void ObjDrawSetRotationSpotLight(NNE_LIGHT light_no, NNS_RGBA *col, float intensity, NNS_VECTOR *pos,
							NNE_ROTATETYPE rottype, NNS_ROTATE_A32 *rotation,
							Angle32 innerangle, Angle32 outerangle, float falloffstart, float falloffend);

#if _WII
// ==========================================================================
// ObjDrawSetSpecularGCLight
/*!
 *	スペキュラーGCライト 設定
 *
 *	@param light_no		[in]	ライトNO
 *	@param col			[in]	カラー設定
 *	@param intensity	[in]	輝度
 *	@param pos			[in]	ライト位置設定
 *	@param rottype		[in]	ローテーションタイプ NNE_ROTATETYPE
 *	@param rotation		[in]	回転設定設定
 *	@param innerangle	[in]	減衰開始角度
 *	@param outerangle	[in]	減衰終了角度
 *	@param falloffstart	[in]	距離減衰開始
 *	@param falloffend	[in]	距離減衰終了
 *
 *	@note
 *		light_noのローテーションスポットライトで設定します。\n
 *		ライトのON・OFF、反映は別途行ってください。
 */
// ==========================================================================
extern void ObjDrawSetSpecularGCLight(NNE_LIGHT light_no, NNS_RGBA *col, NNS_VECTOR *dir);
#endif // #if _WII

#endif // #if OBD_USE_ACTION3D_NN

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // OBJ_DRAW_H_

