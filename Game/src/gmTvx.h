// ==========================================================================
/*!
	@file gmTvx.h
	@brief ゲーム TVX描画

	@author Satoshi Akitomi
				Copyright(c) 2010 Dimps

  $Id: gmTvx.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */
/*!
  @page  gm_tvx gm TVX描画
 
 更なる最適化の為に別の実装で描画を行っても問題ありません。
 */
#ifndef GM_TVX_H_
#define GM_TVX_H_

//----- Include Files -------------------------------------------------------
#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#define GMD_TVX_DISP_ROTATE			(1 <<  0)	//!<	TVX描画 回転使用
#define GMD_TVX_DISP_SCALE			(1 <<  1)	//!<	TVX描画 拡大使用
#define GMD_TVX_DISP_LIGHT_DISABLE	(1 <<  2)	//!<	TVX描画 疑似ライト未使用
#define GMD_TVX_DISP_BLEND			(1 <<  3)	//!<	TVX描画 ブレンド使用

/// TVX描画 拡張設定ワーク
typedef struct tag_GMS_TVX_EX_WORK {
	NNE_PRIM_TEXWRAP    u_wrap;		//!< TEXTURE WRAP U
	NNE_PRIM_TEXWRAP    v_wrap;		//!< TEXTURE WRAP V
	NNS_TEXCOORD        coord;		//!< TEXTURE UV スクロール値
	NNS_RGBA8888		color;		//!< 頂点色マスク(GMD_TVX_DISP_LIGHT_DISABLE時のみ使用)
}GMS_TVX_EX_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// TVX初期化
// ==========================================================================
// ==========================================================================
// GmTvxInit
/*!
 *	TVX システム ビルド
 */
// ==========================================================================
extern void GmTvxBuild(void);

// ==========================================================================
// GmTvxInit(void)
/*!
 *	TVX システム初期化
 */
// ==========================================================================
extern void GmTvxInit(void);

// 描画終了
// ==========================================================================
// ==========================================================================
// GmTvxExit
/*!
 *	TVX システム終了
 */
// ==========================================================================
extern void GmTvxExit(void);

// ==========================================================================
// GmTvxFlush
/*!
 *	TVX システム フラッシュ
 */
// ==========================================================================
extern void GmTvxFlush(void);

// ==========================================================================
// オブジェクト各種設定
// ==========================================================================
// ==========================================================================
// GmTvxSetModel
/*!
 *	TVXモデル設定
 *
 *	@param model_tvx	[in]	TVXモデルデータ
 *	@param model_tex	[in]	テクスチャリスト
 *	@param pos			[in]	描画座標
 *	@param scale		[in]	拡大値(負の値で反転にも使用可能)
 *	@param flag			[in]	TVX描画フラグ
 *	@param rotate_z		[in]	Z回転値(0x0000～0xffff)
 *
 *	@note 拡張情報が必要な場合はGmTvxSetModelExを使ってください。
 *
 */
// ==========================================================================
extern void GmTvxSetModel(void* model_tvx, NNS_TEXLIST* model_tex,
						VecFx32* pos, VecFx32* scale, u32 flag, Angle16 rotate_z);

// ==========================================================================
// GmTvxSetModelEx
/*!
 *	TVXモデル設定 拡張設定付き
 *
 *	@param model_tvx	[in]	TVXモデルデータ
 *	@param model_tex	[in]	テクスチャリスト
 *	@param pos			[in]	描画座標
 *	@param scale		[in]	拡大値(負の値で反転にも使用可能)
 *	@param flag			[in]	TVX描画フラグ
 *	@param rotate_z		[in]	Z回転値(0x0000～0xffff)
 *	@param ex_work		[in]	TVX描画 拡張情報
 *
 *	@note 拡張情報が必要ない場合はGmTvxSetModelを使ってください。
 *
 */
// ==========================================================================
void GmTvxSetModelEx(void* model_tvx, NNS_TEXLIST* model_tex,
						VecFx32* pos, VecFx32* scale, u32 flag, Angle16 rotate_z,
						GMS_TVX_EX_WORK* ex_work);

// ==========================================================================
// GmTvxExecuteDraw
/*!
 *	TVX 一斉描画
 *
 *	@note GmTvxSetModelで設定したモデルデータについて一斉描画
 *		描画コマンドは OBD_DRAW_CMD_STATE_3DNN
 *		一度実行した後は設定したモデルは初期化される。
 */
// ==========================================================================
extern void GmTvxExecuteDraw(void);


#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_TVX_H_

