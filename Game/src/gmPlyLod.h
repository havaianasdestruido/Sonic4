// ==========================================================================
/*!
  @file gmPlyLod.h
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlyLod.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_PLY_LOD_H_
#define GM_PLY_LOD_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#if _WII


/* WII用ハンドロッドモーション */
/// プレイヤーロッドモーションパターンデータ
typedef struct tag_GMS_PLY_LOD_PAT {
	s16		start_frame;	//!< 開始フレーム
	u16		user_data;		//!< 指定ユーザーデータ
} GMS_PLY_LOD_PAT;

/// プレイヤーロッドモーションデータ
typedef struct tag_GMS_PLY_LOD_MTN {
	u32				pat_num;			//!< パターン数
	GMS_PLY_LOD_PAT	*pat_data;			//!< パターンデータ
} GMS_PLY_LOD_MTN;

/// プレイヤーロッドモーションヘッダ
typedef struct tag_GMS_PLY_LOD_MTN_HEADER {
	char			id[4];
	u32				mtn_num;		//!< モーション数
	GMS_PLY_LOD_MTN	*mtn_data;		//!< モーションデータ
} GMS_PLY_LOD_MTN_HEADER;


#define GMD_PLY_LOD_TYPE_R_GU			(256)		//!< グー
#define GMD_PLY_LOD_TYPE_R_PAR			(512)		//!< パー
#define GMD_PLY_LOD_TYPE_R_FOREFINGER	(768)		//!< 人差し指
#define GMD_PLY_LOD_TYPE_R_THUMB_UP		(1024)		//!< サムアップ

#define GMD_PLY_LOD_TYPE_L_GU			(256*16)	//!< グー
#define GMD_PLY_LOD_TYPE_L_PAR			(512*16)	//!< パー
#define GMD_PLY_LOD_TYPE_L_FOREFINGER	(768*16)	//!< 人差し指
#define GMD_PLY_LOD_TYPE_L_THUMB_UP		(1024*16)	//!< サムアップ

#define GMD_PLY_LOD_TYPE_R_MASK			(0x0F00)	//!< ユーザーデータマスク 右手
#define GMD_PLY_LOD_TYPE_L_MASK			(0xF000)	//!< ユーザーデータマスク 右手


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// GmPlyLodCnvAddr
/*!
 *	ロッドアクション アドレスコンバート
 *
 *	@param	data		[in]	ロッドモーションデータ
 */
// ==========================================================================
extern void GmPlyLodCnvAddr(void *data);

// ==========================================================================
// GmPlyLodGetLodActionPatData
/*!
 *	ロッドアクション プレイヤーハンド パターンデータ取得
 *
 *	@param	act_id		[in]	アクションID
 *	@param	frame		[in]	設定フレーム
 *
 *	@note
 *		データ参照のみの為 ゲーム外からの参照可能
 */
// ==========================================================================
extern GMS_PLY_LOD_PAT* GmPlyLodGetLodActionPatData(GMS_PLY_LOD_MTN_HEADER *header, s32 act_id, fx32 frame);
#endif

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_PLY_LOD_H_

//----- Include Files -------------------------------------------------------
