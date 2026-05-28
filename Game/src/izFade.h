// ==========================================================================
/*!
  @file izFade.h
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: izFade.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef IZ_FADE_H_
#define IZ_FADE_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
/// フェードタイプ カラー・IN OUT
typedef enum tag_IZE_FADE_TYPE {
	IZE_FADE_TYPE_BLACK_FADEIN	= 0,
	IZE_FADE_TYPE_BLACK_FADEOUT,
	IZE_FADE_TYPE_WHITE_FADEIN,
	IZE_FADE_TYPE_WHITE_FADEOUT,

	IZE_FADE_TYPE_MAX
} IZE_FADE_TYPE;

/// フェード設定タイプ
typedef enum tag_IZE_FADE_SET_TYPE {
	IZE_FADE_SET_TYPE_NORMAL	= 0,	//!< 通常設定 (前のフェードがある場合でも再設定)
	IZE_FADE_SET_TYPE_TAKEOEVER,		//!< 継続設定 (前のフェードがある場合は、そのカラーからフェード)

	IZE_FADE_SET_TYPE_MAX
} IZE_FADE_SET_TYPE;


#define IZD_FADE_TASK_PAUSE_LEVEL_DEF	(0)			//!< 標準ポーズレベル
#define IZD_FADE_TASK_GROUP_DEF			(0)			//!< 標準タスクグループ

#define IZD_FADE_DT_PRIO_DEF			(0xEFFF)	//!< 描画タスク優先
#define IZD_FADE_DRAW_STATE_DEF			(18)		//!< 標準描画ステート(objDrawやgmMainを見て影響の出ない番号を使う)




#define IZD_FADE_SURFACE_SET_NUM	(2)		//!< プリミティブサーフェイス数
#define IZD_FADE_VTX_NUM			(4)		//!< 頂点数
/// フェードワーク
typedef struct tag_IZS_FADE_WORK {
	AMS_PARAM_DRAW_PRIMITIVE	prim_param;
	NNS_PRIM2D_PC				vtx[IZD_FADE_SURFACE_SET_NUM][IZD_FADE_VTX_NUM];
	NNS_MATRIX					mtx;	// いらないようだが念の為設定しておく	

	NNS_RGBA		start_col;	// フェード開始カラー
	NNS_RGBA		end_col;	// フェード終了カラー
	NNS_RGBA		now_col;	// 現在のカラー

	float			time;		// フェード時間
	float			count;		// カウンタ
	float			speed;		// 速度

	u32				flag;

	u32				draw_state;
	u16				dt_prio;

	u16				vtx_no;		// 現在使用しているvtx no

} IZS_FADE_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// IzFadeInitEasy
/*!
 *	フェード初期化
 *
 *	@param	fade_set_type	[in]	フェードセットタイプ IZE_FADE_SET_TYPE
 *	@param	fade_type		[in]	フェードタイプ IZE_FADE_TYPE		
 *	@param	time			[in]	フェード時間 (フレーム)
 *	@param	draw_start		[in]	描画開始処理の有無 (default TRUE)
 *
 *	@note
 *		設定されたカラーのプリミティブで画面を覆い、全画面フェードを行います。
 */
// ==========================================================================
extern void IzFadeInitEasy(IZE_FADE_SET_TYPE fade_set_type, IZE_FADE_TYPE fade_type, float time, BOOL draw_start=TRUE);

// ==========================================================================
// IzFadeInitEasyTask
/*!
 *	フェード初期化
 *
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
 *
 *	@note
 *		設定されたカラーのプリミティブで画面を覆い、全画面フェードを行います。
 */
// ==========================================================================
extern void IzFadeInitEasyTask(IZE_FADE_SET_TYPE fade_set_type,
				u8 start_col_r, u8 start_col_g, u8 start_col_b, u8 start_col_a,
				u8 end_col_r, u8 end_col_g, u8 end_col_b, u8 end_col_a,
				float time, BOOL draw_start=TRUE);

// ==========================================================================
// IzFadeInitEasyColor
/*!
 *	フェード初期化
 *
 *	@param	group			[in]	フェード処理タスクグループ
 *	@param	pause_level		[in]	フェード処理ポーズレベル
 *	@param	dt_prio			[in]	描画処理プライオリティ
 *	@param	draw_state		[in]	描画ステート
 *	@param	fade_set_type	[in]	フェードセットタイプ IZE_FADE_SET_TYPE
 *	@param	fade_type		[in]	フェードタイプ IZE_FADE_TYPE
 *	@param	time			[in]	フェード時間 (フレーム)
 *	@param	draw_start		[in]	描画開始処理の有無 (default TRUE)
 *
 *	@note
 *		設定されたカラーのプリミティブで画面を覆い、全画面フェードを行います。
 */
// ==========================================================================
extern void IzFadeInitEasyColor(s32 group, u16 pause_level, u16 dt_prio, u32 draw_state, IZE_FADE_SET_TYPE fade_set_type,
				IZE_FADE_TYPE fade_type, float time, BOOL draw_start=TRUE);

// ==========================================================================
// IzFadeInit
/*!
 *	フェード初期化
 *
 *	@param	group			[in]	フェード処理タスクグループ
 *	@param	pause_level		[in]	フェード処理ポーズレベル
 *	@param	dt_prio			[in]	描画処理プライオリティ
 *	@param	draw_state		[in]	描画ステート
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
 *
 *	@note
 *		設定されたカラーのプリミティブで画面を覆い、全画面フェードを行います。
 *		draw_startがFALSEの場合は、描画の登録を行うのみで、描画の開始を行いません。
 *		FALSEを設定する場合は、必ずユーザーが描画の開始(amDrawExecCommand)を
 *		行ってください。
 *		また、この設定のときは、ブレンディング設定などを復旧しないので、
 *		必要な場合はユーザーが復旧して下さい。
 */
// ==========================================================================
extern void IzFadeInit(s32 group, u16 pause_level, u16 dt_prio, u32 draw_state, IZE_FADE_SET_TYPE fade_set_type,
				u8 start_col_r, u8 start_col_g, u8 start_col_b, u8 start_col_a,
				u8 end_col_r, u8 end_col_g, u8 end_col_b, u8 end_col_a,
				float time, BOOL draw_start=TRUE);

// ==========================================================================
// IzFadeExit
/*!
 *	フェード終了
 *
 *	@note
 *		IzFadeInitで生成したフェード処理は、自分では終了しませんので、\n
 *		不要になった時点で終了して下さい。
 */
// ==========================================================================
extern void IzFadeExit(void);

// ==========================================================================
// IzFadeIsExe
/*!
 *	フェード実行中チェック
 *
 *	@reuturn	TRUE : 実行中
 *
 *	@note
 *		フェード処理が生成されているかどうかで判定します。\n
 *		フェード処理が終了しているかどうかは IzFadeIsEnd で確認して下さい。
 */
// ==========================================================================
extern BOOL IzFadeIsExe(void);

// ==========================================================================
// IzFadeIsEnd
/*!
 *	フェード終了チェック
 *
 *	@reuturn	TRUE : 終了 or フェード実行中でない
 */
// ==========================================================================
extern BOOL IzFadeIsEnd(void);

// ==========================================================================
// IzFadeRestoreDrawSetting
/*!
 *	フェードで使用した描画設定を通常設定に復旧する
 *
 *	@note
 *		フェード初期化時にdraw_startをFALSEにした場合、フェードで使用した
 *		描画設定が復旧されません。
 *		ユーザーは必要に応じて、自分で復旧して下さい。
 *		ユーザー描画コマンド内で、IzFadeRestoreDrawSetting を呼び出すと
 *		標準状態に復旧します。
 */
// ==========================================================================
extern void IzFadeRestoreDrawSetting(void);

// ==========================================================================
// IzFadeSetStopUpdate1Frame
/*!
 *	1フレーム更新停止設定
 *
 *	@param	fade_work	[in]	フェードワーク NULLの場合は標準フェードに設定
 */
// ==========================================================================
extern void IzFadeSetStopUpdate1Frame(IZS_FADE_WORK *fade_work);

// ==========================================================================
// IzFadeSetWork
/*!
 *	フェードワーク初期化
 *
 *	@param	fade_work		[in]	フェードワーク
 *	@param	dt_prio			[in]	描画処理プライオリティ
 *	@param	draw_state		[in]	描画ステート
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
extern void IzFadeSetWork(IZS_FADE_WORK *fade_work, u16 dt_prio, u32 draw_state, IZE_FADE_SET_TYPE fade_set_type,
				u8 start_col_r, u8 start_col_g, u8 start_col_b, u8 start_col_a,
				u8 end_col_r, u8 end_col_g, u8 end_col_b, u8 end_col_a,
				float time, BOOL draw_start/*=TRUE*/, BOOL conti_state/*=FALSE*/);

// ==========================================================================
// IzFadeUpdate
/*!
 *	フェード更新
 *
 *	@param	fade_work	[in]	フェードワーク
 */
// ==========================================================================
extern void IzFadeUpdate(IZS_FADE_WORK *fade_work);

// ==========================================================================
// IzFadeDraw
/*!
 *	フェード描画
 *
 *	@param	fade_work	[in]	フェードワーク
 */
// ==========================================================================
extern void IzFadeDraw(IZS_FADE_WORK *fade_work);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // IZ_FADE_H_

//----- Include Files -------------------------------------------------------
