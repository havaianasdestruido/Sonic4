// ==========================================================================
/*!
	@file objTblWork.h
	@brief オブジェクト 描画

	@author mana
	@author modifier Ishizaki
				Copyright(c) 2007 Dimps

  $Id: objTblWork.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */
/*!
  @page  obj_tbl_work obj オブジェクト
 
    
  @sa objObject.c objObject.h objTblWork.c objTblWork.h
 */
#ifndef OBJ_TBL_WORK_H_
#define OBJ_TBL_WORK_H_


//----- Include Files -------------------------------------------------------
#include "objObject.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
/// オブジェクトテーブル データタイプ
typedef enum tag_DRE_TBL_WORK_TYPE {
	OBD_TBLWORK_TYPE_ACT		= 0,		///< オブジェクトテーブルアクション
	OBD_TBLWORK_TYPE_MOVE,					///< オブジェクトテーブルムーブ
	OBD_TBLWORK_TYPE_SCALE,					///< オブジェクトテーブルスケール
	OBD_TBLWORK_TYPE_DIR,					///< オブジェクトテーブルディレクション

	OBD_TBLWORK_TYPE_MAX					///< オブジェクトテーブル データタイプ最大数
} DRE_TBL_WORK_TYPE;

/// オブジェクトテーブルアクション（ファイルにするのが望ましい）
typedef struct tag_OBS_ACT_TBL {
	u16	act_id;			///< アクションID
	u8	time;			///< 時間
	u8	flag;			///< フラグ
} OBS_ACT_TBL;

// OBS_ACT_TBL:flag
#define OBD_TBLWORK_ACTFLAG_REPEAT_POINT	( 1 << 0 )	///< テーブルリピートの始点
#define OBD_TBLWORK_ACTFLAG_REPEAT			( 1 << 1 )	///< 指定アクションをリピートする（テーブルリピートとは別）


/// オブジェクトテーブルムーブ（ファイルにするのが望ましい）
typedef struct tag_OBS_MOVE_TBL {
	VecFx32	spd;		///< 速度
	VecFx32	spd_add;	///< 加速値
	u8		time;		///< 時間
	u8		flag;		///< フラグ
} OBS_MOVE_TBL;

// OBS_MOVE_TBL:flag
#define OBD_TBLWORK_MOVE_REPEAT_POINT		( 1 << 0 )	///< テーブルリピートの始点


/// オブジェクトテーブルスケール（ファイルにするのが望ましい）
typedef struct tag_OBS_SCALE_TBL {
	VecFx32	scale;		///< スケール
	u8		time;		///< 時間
	u8		flag;		///< フラグ
} OBS_SCALE_TBL;

// OBS_SCALE_TBL:flag
#define OBD_TBLWORK_SCALE_REPEAT_POINT		( 1 << 0 ) ///< テーブルリピートの始点


/// オブジェクトテーブルディレクション（ファイルにするのが望ましい）
typedef struct tag_OBS_DIR_TBL {
	VecU16	dir;		///< 角度
	u8		time;		///< 時間
	u8		flag;		///< フラグ
} OBS_DIR_TBL;

// OBS_DIR_TBL:flag
#define OBD_TBLWORK_DIR_REPEAT_POINT		( 1 << 0 ) ///< テーブルリピートの始点


/// オブジェクトテーブルワーク オブジェテーブルを使って動作させる時、オブジェクト側にこのワークを用意する。
typedef struct tag_OBS_TBL_WORK {
	s16				key_timer[OBD_TBLWORK_TYPE_MAX];		///< テーブル進行度
	s16				move_timer[OBD_TBLWORK_TYPE_MAX];		///< タイマー進行度
	u32				flag;									///< 動作フラグ

	VecFx32			spd;			///< 親オフセット時の速度保持ワーク
	VecFx32			scale;			///< 速度のスケーリング
	VecU16			dir;			///< 速度の回転

	OBS_ACT_TBL		*act_tbl;		///< アクションテーブル先頭ポインタ
	OBS_MOVE_TBL	*move_tbl;		///< 移動テーブル先頭ポインタ
	OBS_SCALE_TBL	*scale_tbl;		///< スケールテーブル先頭ポインタ
	OBS_DIR_TBL		*dir_tbl;		///< 角度テーブル先頭ポインタ

	OBS_DATA_WORK	*data_work[OBD_TBLWORK_TYPE_MAX];	///< アクションテーブル データポインタ ファイルの場合

	s16				repeat_point[OBD_TBLWORK_TYPE_MAX];			///< リピート先

} OBS_TBL_WORK;

// OBS_TBL_WORK:flag
#define OBD_TBLWORK_FLAG_ACT_END			( 1 << 0)	///< アクションテーブルを再生し終えた
#define OBD_TBLWORK_FLAG_MOVE_END			( 1 << 1)	///< 移動テーブルを再生し終えた
#define OBD_TBLWORK_FLAG_SCALE_END			( 1 << 2)	///< スケールテーブルを再生し終えた
#define OBD_TBLWORK_FLAG_DIR_END			( 1 << 3)	///< 角度テーブルを再生し終えた

#define OBD_TBLWORK_FLAG_SPD_VFLIP			( 1 << 4)	///< オブジェクトの上下向きに合わせて移動方向を変化させる
#define OBD_TBLWORK_FLAG_SPD_HFLIP			( 1 << 5)	///< オブジェクトの左右向きに合わせて移動方向を変化させる
#define OBD_TBLWORK_FLAG_REPEAT				( 1 << 6)	///< 繰り返し再生する。
#define OBD_TBLWORK_FLAG_SPD_DIR			( 1 << 7)	///< 移動方向へ角度をを向ける（ 登録した #OBS_DIR_TBLは無視される）未使用
//#define OBD_TBLWORK_FLAG_NO_FLIP			( 1 << 4)	///< フリップ設定を無視する

#define OBD_TBLWORK_FLAG_NO_DISP			( 1 << 8)	///< 表示、非表示設定を無視する 未使用
#define OBD_TBLWORK_FLAG_MOVE_PARENT		( 1 << 9)	///< ムーブテーブルをオブジェクト速度へ設定せず、親からのオフセットを加算するようにする
#define OBD_TBLWORK_FLAG_FLIP_PARRENT_FIX	( 1 <<10)	///< 親の向きに合わせて自動でVFLIP、HFLIPする

#define OBD_TBLWORK_FLAG_NO_ACT				( 1 <<24)	///< 登録したアクションテーブルを無視する
#define OBD_TBLWORK_FLAG_NO_MOVE			( 1 <<25)	///< 登録した移動テーブルを無視する
#define OBD_TBLWORK_FLAG_NO_SCALE			( 1 <<26)	///< 登録したスケールテーブルを無視する
#define OBD_TBLWORK_FLAG_NO_DIR				( 1 <<27)	///< 登録した角度テーブルを無視する

// #define OBD_OBJTBL_ACT_FILE    ( 1 <<28) ///< アクションテーブルはファイル（削除時開放）
// #define OBD_OBJTBL_MOVE_FILE   ( 1 <<29) ///< 移動テーブルはファイル（削除時開放）
// #define OBD_OBJTBL_SCALE_FILE  ( 1 <<30) ///< スケールテーブルはファイル（削除時開放）
// #define OBD_OBJTBL_DIR_FILE    ( 1 <<31) ///< 角度テーブルはファイル（削除時開放）


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// ObjObjectTblWork
/*!
 *	オブジェクト テーブルワーク 動作再生
 *
 *	@param obj_work	[io]	オブジェクトワークポインタ
 *
 *	@note
 *		テーブルに設定した動きをオブジェクトに行わせる
 */
// ==========================================================================
extern void ObjObjectTblWork(OBS_OBJECT_WORK *obj_work);

// ==========================================================================
// ObjTblWork
/*!
 *	テーブルワーク  動作再生
 *
 *	@param obj_work	[io]	オブジェクトワークポインタ
 *	@param obj_tbl	[in]	テーブルワークポインタ
 *
 *	@note
 *		テーブルに設定した動きをオブジェクトに行わせる
 */
// ==========================================================================
extern void ObjTblWork(OBS_OBJECT_WORK *obj_work, OBS_TBL_WORK *obj_tbl);

// ==========================================================================
// ObjObjectTblWorkReset
/*!
 *	オブジェクト テーブルワーク 初期化
 *
 *	@param obj_tbl	[in]	テーブルワークポインタ
 *
 *	@note
 *		オブジェクトの速度なども初期化する
 */
// ==========================================================================
extern void ObjObjectTblWorkReset(OBS_OBJECT_WORK *obj_work);

// ==========================================================================
// ObjTblWorkReset
/*!
 *	テーブルワーク初期化
 *
 *	@param obj_tbl	[in]	テーブルワークポインタ
 */
// ==========================================================================
extern void ObjTblWorkReset(OBS_TBL_WORK *obj_tbl);

// ==========================================================================
// ObjTblWorkRelease
/*!
 *	オブジェクト テーブルワークファイル開放
 *
 *	@param obj_tbl	[in]	テーブルワークポインタ
 *
 *	@note
 *		ファイル読み込みしていた場合、各データを開放する。
 */
// ==========================================================================
extern void ObjTblWorkRelease(OBS_TBL_WORK *obj_tbl);

// ==========================================================================
// ObjTblWorkActSet
/*!
 *	オブジェクト テーブルワーク オブジェクトテーブルアクションセット
 *
 *	@param obj_tbl	[in]	テーブルワークポインタ
 *	@param act_tbl	[in]	オブジェクトテーブルアクションポインタ
 *
 *	@note
 *		メモリ上にデータがある場合は、直接指定
 */
// ==========================================================================
extern void ObjTblWorkActSet(OBS_TBL_WORK *obj_tbl, OBS_ACT_TBL *act_tbl);

// ==========================================================================
// ObjTblWorkActLoad
/*!
 *	オブジェクト テーブルワーク オブジェクトテーブルアクションロード
 *
 *	@param obj_tbl		[in]	テーブルワークポインタ
 *	@param data_work	[in]	データ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
 *	@param arc_buf		[in]	アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)
 */
// ==========================================================================
extern void ObjTblWorkActLoad(OBS_TBL_WORK *obj_tbl, const char *path, OBS_DATA_WORK *data_work, void *arc_buf);

// ==========================================================================
// ObjTblWorkMoveSet
/*!
 *	オブジェクト テーブルワーク オブジェクトテーブルムーブセット
 *
 *	@param obj_tbl	[in]	テーブルワークポインタ
 *	@param move_tbl	[in]	オブジェクトテーブルムーブポインタ
 *
 *	@note
 *		メモリ上にデータがある場合は、直接指定
 */
// ==========================================================================
extern void ObjTblWorkMoveSet(OBS_TBL_WORK *obj_tbl, OBS_MOVE_TBL *move_tbl);

// ==========================================================================
// ObjTblWorkMoveLoad
/*!
 *	オブジェクト テーブルワーク オブジェクトテーブルムーブロード
 *
 *	@param obj_tbl		[in]	テーブルワークポインタ
 *	@param data_work	[in]	データ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
 *	@param arc_buf		[in]	アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)
 */
// ==========================================================================
extern void ObjTblWorkMoveLoad(OBS_TBL_WORK *obj_tbl, const char *path, OBS_DATA_WORK *data_work, void *arc_buf);

// ==========================================================================
// ObjTblWorkScaleSet
/*!
 *	オブジェクト テーブルワーク オブジェクトテーブルスケールセット
 *
 *	@param obj_tbl		[in]	テーブルワークポインタ
 *	@param scale_tbl	[in]	オブジェクトテーブルスケールポインタ
 *
 *	@note
 *		メモリ上にデータがある場合は、直接指定
 */
// ==========================================================================
extern void ObjTblWorkScaleSet(OBS_TBL_WORK *obj_tbl, OBS_SCALE_TBL *scale_tbl);

// ==========================================================================
// ObjTblWorkScaleLoad
/*!
 *	オブジェクト テーブルワーク オブジェクトテーブルスケールロード
 *
 *	@param obj_tbl		[in]	テーブルワークポインタ
 *	@param data_work	[in]	データ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
 *	@param arc_buf		[in]	アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)
 */
// ==========================================================================
extern void ObjTblWorkScaleLoad(OBS_TBL_WORK *obj_tbl, const char *path, OBS_DATA_WORK *data_work, void *arc_buf);

// ==========================================================================
// ObjTblWorkDirSet
/*!
 *	オブジェクト テーブルワーク関数 オブジェクトテーブルディレクションセット
 *
 *	@param obj_tbl	[in]	テーブルワークポインタ
 *	@param dir_tbl	[in]	オブジェクトテーブル角度ポインタ
 *
 *	@note
 *		メモリ上にデータがある場合は、直接指定
 */
// ==========================================================================
extern void ObjTblWorkDirSet(OBS_TBL_WORK *obj_tbl, OBS_DIR_TBL *dir_tbl);

// ==========================================================================
// ObjTblWorkDirLoad
/*!
 *	オブジェクト テーブルワーク関数 オブジェクトテーブルディレクションロード
 *
 *	@param obj_tbl		[in]	テーブルワークポインタ
 *	@param data_work	[in]	データ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
 *	@param arc_buf		[in]	アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)
 */
// ==========================================================================
extern void ObjTblWorkDirLoad(OBS_TBL_WORK *obj_tbl, const char *path, OBS_DATA_WORK *data_work, void *arc_buf);


#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // OBJ_TBL_WORK_H_

