// =======================================================================
/*!
	@file	dmStaffRollMdlCtrl.h
	@brief	デモ・スタッフロール画面モデル操作モジュール

	@author Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmStaffRollMdlCtrl.h 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *
 *
 */

#ifndef DM_STFRL_MDL_CTRL_H_
#define DM_STFRL_MDL_CTRL_H_



// ----- Include Files ---------------------------------------（インクルード）

#include "objObject.h"
#include "gmBossCommon.h"

#if _WII
#include "gmPlyLod.h"
#endif

// ----- Macros ------------------------------------------------（マクロ定義）

#define DMD_STFRL_RING_EFCT_DISP_NUM		(6)

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

//! ソニックワーク
typedef struct tag_DMS_STFRL_SONIC_WORK
{
	OBS_OBJECT_WORK		obj_work;		//!< オブジェクトワーク
	
#if _WII
	GMS_PLY_LOD_MTN_HEADER	*hand_lod_header;	//!< Wii用手ロッド切り替えデータヘッダ
	GMS_PLY_LOD_MTN	*hand_lod_mtn;				//!< Wii用手ロッド切り替えモーションデータ
	GMS_PLY_LOD_PAT	*hand_lod_pat;				//!< Wii用手ロッド切り替えパターンデータ
	u32				hand_lod_pat_no;			//!< Wii用手ロッド切り替えパターンNO
	u32		hand_mat_user_data;					//!< Wii用手ロッド切り替え設定

#endif

	s16		timer;				//!< リング生存タイマー
	u16		flag;				//!< リング状態フラグ
	float	alpha;				//!< リング１まとまり分の透過度(0.f ～ 1.f)
	float	alpha_spd;			//!< 透過度切り替え速度
	
} DMS_STFRL_SONIC_WORK;


//! ボス本体ワーク
typedef struct tag_DMS_STFRL_BOSS_BODY_WORK
{
	OBS_OBJECT_WORK		obj_work;		//!< オブジェクトワーク
	
	GMS_BS_CMN_BMCB_MGR	bmcb_mgr;		//!< ボスモーションCB管理ワーク
	GMS_BS_CMN_SNM_WORK	snm_work;		//!< SNMワーク
	
	Sint32	egg_snm_reg_id;
	Sint32	body_snm_reg_id;			//!< 本体（アフターバーナーや煙などをくっつけるため）
	
	// モーションSTATEなど
	u32 flag;							//!<
	s32 timer;
	
} DMS_STFRL_BOSS_BODY_WORK;


//! ボスエッグマンワーク
typedef struct tag_DMS_STFRL_BOSS_EGG_WORK
{
	OBS_OBJECT_WORK		obj_work;		//!< オブジェクトワーク
	
	// モーションSTATEなど
	u32 flag;							//!<
	s32 timer;							//!< 
	
} DMS_STFRL_BOSS_EGG_WORK;


//! リングワーク
typedef struct tag_DMS_STFRL_RING_WORK {
	
	OBS_OBJECT_WORK		obj_work;		//!< オブジェクトワーク

	VecFx32	start_pos;								//!< リング座標
	VecFx32	pos[DMD_STFRL_RING_EFCT_DISP_NUM];		//!< リング座標
	VecFx32	scale;				//!< リング拡大率
	fx32	spd_x[DMD_STFRL_RING_EFCT_DISP_NUM];	//!< リング移動速度 X
	fx32	spd_y[DMD_STFRL_RING_EFCT_DISP_NUM];	//!< リング移動速度 Y
	
	void (*proc_efct)(OBS_OBJECT_WORK *);		//!< エフェクト用プロシージャ
	
	s32		efct_start_time;	//!< 演出開始時間
	s32		timer;				//!< リング生存タイマー
	s32		efct_timer;			//!< エフェクト用タイマー
	u32		flag;				//!< リング状態フラグ
	float	alpha;				//!< リング１まとまり分の透過度(0.f ～ 1.f)
	float	alpha_spd;			//!< 透過度切り替え速度]
	
	s32		disp_ring_pos_no;	//!< 
	s32		disp_efct_pos_no;	//!< 
	
} DMS_STFRL_RING_WORK;


// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）

// =======================================================================
// DmStfrlMdlCtrlSonicBuild
/*!
   スタッフロール用ソニック データ構築
 */
// =======================================================================
extern void DmStfrlMdlCtrlSonicBuild(void);

// =======================================================================
// DmStfrlMdlCtrlSonicFlush
/*!
   スタッフロール用ソニック データ解放
 */
// =======================================================================
extern void DmStfrlMdlCtrlSonicFlush(void);


// ==========================================================================
// DmStfrlMdlCtrlSetSonicObj
/*!
	スタッフロール用のソニックOBJ設定処理
 */
// ==========================================================================
extern DMS_STFRL_SONIC_WORK *DmStfrlMdlCtrlSetSonicObj(void);


// =======================================================================
// DmStfrlMdlCtrlBoss1Build
/*!
   スタッフロール用ボス１ データ構築
 */
// =======================================================================
extern void DmStfrlMdlCtrlBoss1Build(void);


// =======================================================================
// DmStfrlMdlCtrlBoss1Flush
/*!
   スタッフロール用ボス１ データ片付け
 */
// =======================================================================
extern void DmStfrlMdlCtrlBoss1Flush(void);


// ==========================================================================
// DmStfrlMdlCtrlSetBodyObj
/*!
	スタッフロール用のボスOBJ設定処理
 */
// ==========================================================================
extern DMS_STFRL_BOSS_BODY_WORK *DmStfrlMdlCtrlSetBodyObj(void);


// =======================================================================
// DmStfrlMdlCtrlSetEggObj
/*!
  ボス１ エッグマン 初期化
  
  @return	オブジェクトワーク
 */
// =======================================================================
extern DMS_STFRL_BOSS_EGG_WORK *DmStfrlMdlCtrlSetEggObj(OBS_OBJECT_WORK *body_work);


// ==========================================================================
// DmStfrlMdlCtrlRingBuild
/*!
 *	リングデータ構築
 */
// ==========================================================================
extern void DmStfrlMdlCtrlRingBuild(void);

// ==========================================================================
// DmStfrlMdlCtrlRingFlush
/*!
 *	リングデータフラッシュ
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
extern void DmStfrlMdlCtrlRingFlush(void);

// ==========================================================================
// DmStfrlMdlCtrlSetRingObj
/*!
	スタッフロール用のリングOBJ設定処理
 */
// ==========================================================================
extern DMS_STFRL_RING_WORK *DmStfrlMdlCtrlSetRingObj(s32 delay_time, u32 type);


// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ===========================================================================
//	function
/*!
	説明

	@param param0	[in] 入力引数0説明
	@param param1	[out] 出力ポインタ引数1説明
	@param param2	[io] 入出力ポインタ引数2説明
	@return 返値説明
	@note 補足説明
*/
// ===========================================================================

#endif // DM_STFRL_MDL_CTRL_H_

// ***************************************************************************
// ラベル
// ***************************************************************************
