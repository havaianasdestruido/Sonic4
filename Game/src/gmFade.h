// ==========================================================================
/*!
  @file gmFade.h
  @brief フェードオブジェクト

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmFade.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 *	ppOutを使用します。
 *	ppFuncはユーザーが使用できます。
 *
 *	使い方
 *      ・オブジェクト生成
 *          GmFadeCreateFadeObj
 *                │
 *      ・フェード設定
 *          GmFadeSetFade
 *        ┌──→┤
 *        │・フェード設定
 *        │    GmFadeSetFade
 *        └───┤
 *		・フェード終了
 *			OBJ_OBJECT_TASK_CLEAR
 *
 *
 */


#ifndef GM_FADE_H_
#define GM_FADE_H_


//----- Include Files -------------------------------------------------------
#include "izFade.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
/// フェードオブジェクトワーク
typedef struct tag_GMS_FADE_OBJ_WORK {
	OBS_OBJECT_WORK	obj_work;
	IZS_FADE_WORK	fade_work;
} GMS_FADE_OBJ_WORK;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// IzFadeSetWork
/*!
 *	フェードワーク初期化
 *
 *	@param	prio			[in]	フェード処理タスク優先
 *	@param	group			[in]	フェード処理タスクグループ
 *	@param	pause_level		[in]	フェード処理ポーズレベル
 *	@param	work_size		[in]	ワークサイズ sizeof(GMS_FADE_OBJ_WORK) 以上
 *	@param	dt_prio			[in]	描画処理プライオリティ
 *	@param	draw_state		[in]	描画ステート
 *
 *	@note
 *		
 */
// ==========================================================================
extern GMS_FADE_OBJ_WORK* GmFadeCreateFadeObj(u16 prio, u8 group, u8 pause_level, u32 work_size, u16 dt_prio, u32 draw_state);

// ==========================================================================
// GmFadeSetFade
/*!
 *	フェードワーク設定
 *
 *	@param	fade_obj		[in]	フェードオブジェクト
 *	@param	fade_set_type	[in]	フェードセットタイプ IZE_FADE_SET_TYPE
 *	@param	start_col_r		[in]	開始カラー値 RED
 *	@param	start_col_g		[in]	開始カラー値 GREEN	
 *	@param	start_col_b		[in]	開始カラー値 BLUE	
 *	@param	start_col_a		[in]	開始カラー値 ALPHA	
 *	@param	end_col_r		[in]	終了カラー値 RED	
 *	@param	end_col_g		[in]	終了カラー値 GREEN			
 *	@param	end_col_b		[in]	終了カラー値 BLUE	
 *	@param	end_col_a		[in]	終了カラー値 RED			
 *	@param	time			[in]	フェード時間 (フレーム)
 *	@param	draw_start		[in]	描画開始処理の有無 (default TRUE)
 *	@param	conti_state		[in]	fade_workの設定の引継ぎあり(default FALSE)
 *
 *	@note
 *		IZS_FADE_WORKを設定します。\n
 *		conti_state==FALSEの場合は、完全初期化、\n
 *		conti_state==TRUEの場合は、以前のステータスを引き継ぎます。\n
 *		初めての設定の場合は、必ずFALSEを設定してください。
 */
// ==========================================================================
inline void GmFadeSetFade(GMS_FADE_OBJ_WORK *fade_obj, IZE_FADE_SET_TYPE fade_set_type,
				u8 start_col_r, u8 start_col_g, u8 start_col_b, u8 start_col_a,
				u8 end_col_r, u8 end_col_g, u8 end_col_b, u8 end_col_a,
				float time, BOOL draw_start, BOOL conti_state)
{
	IZS_FADE_WORK		*fade_work;

	MTM_ASSERT(fade_obj);
	fade_work = &fade_obj->fade_work;

	IzFadeSetWork(fade_work, fade_work->dt_prio, fade_work->draw_state, fade_set_type,
				start_col_r, start_col_g, start_col_b, start_col_a,
				end_col_r, end_col_g, end_col_b, end_col_a,
				time, draw_start, conti_state);
}

// ==========================================================================
// GmFadeIsEnd
/*!
 *	フェード終了チェック
 *
 *	@reuturn	TRUE : 終了 or フェード実行中でない
 */
// ==========================================================================
extern BOOL GmFadeIsEnd(GMS_FADE_OBJ_WORK *fade_obj);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _PT_H_

//----- Include Files -------------------------------------------------------
