// ==========================================================================
/*!
  @file gmGameDat.h
  @brief ゲームデータ管理

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGameDat.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_GAME_DAT_H_
#define GM_GAME_DAT_H_


//----- Include Files -------------------------------------------------------
#include "gsMainSys.h"
#include "gmMainDat.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#define GMD_GAMEDAT_NOLOAD_NOUSE_DATA	(1 & _IPHONE) // 不要データロードしない

#if _IPHONE
#define GMD_GAMEDAT_NO_CREATE_MAP_MOTION (1) // MAP motion none
#else
//#define GMD_GAMEDAT_NO_CREATE_MAP_MOTION (1) // MAP motion none
#endif _IPHONE
	
/// ロード処理タイプ
typedef enum tag_GME_GAMEDAT_LOAD_PROC {
	GMD_GAMEDAT_LOAD_PROC_NORMAL    = 0,        //!< 通常ロード
	GMD_GAMEDAT_LOAD_PROC_PRE_LOAD,				//!< 先行読み込み(データ読み込み後 後処理せずに待機)

	GMD_GAMEDAT_LOAD_PROC_MAX
} GME_GAMEDAT_LOAD_PROC;

/// データ読み込み状況
typedef enum tag_GME_GAMEDAT_LOAD_PROGRESS {
	GMD_GAMEDAT_LOAD_PROGRESS_NOLOAD	= 0,	//!< 読み込みを開始していない
	GMD_GAMEDAT_LOAD_PROGRESS_LOADING,			//!< 読み込み中
	GMD_GAMEDAT_LOAD_PROGRESS_LOADFINISH,		//!< データ読み込みまで終了
	GMD_GAMEDAT_LOAD_PROGRESS_COMPLETE,			//!< 読み込み処理全終了

	GMD_GAMEDAT_LOAD_PROGRESS_MAX
} GME_GAMEDAT_LOAD_PROGRESS;


/// 固定MAPデータ(読み込み順)
enum {
	GMD_GAMEDAT_MAP_MAP_SET	= 0,	//!< マップセットAMB
	GMD_GAMEDAT_MAP_MODEL,			//!< マップモデルAMB
	GMD_GAMEDAT_MAP_TEX,			//!< マップテクスチャAMB
	GMD_GAMEDAT_MAP_ATTR,			//!< マップアトリビュートAMB
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	GMD_GAMEDAT_MAP_MTN,			//!< マップモーションAMB
	GMD_GAMEDAT_MAP_MMTN,			//!< マップマテリアルモーションAMB
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	GMD_GAMEDAT_MAP_MAX
};

/// マップセットデータタイプ
typedef enum tag_GME_GAMEDAT_MAPSET {
	GMD_GAMEDAT_MAPSET_A_MP	= 0,	//!< マップA配置
	GMD_GAMEDAT_MAPSET_B_MP,		//!< マップB配置
	GMD_GAMEDAT_MAPSET_A_MD,		//!< マップAオフセット配置
	GMD_GAMEDAT_MAPSET_B_MD,		//!< マップBオフセット配置
	GMD_GAMEDAT_MAPSET_ATTR_A_MP,	//!< マップA属性配置
	GMD_GAMEDAT_MAPSET_ATTR_B_MP,	//!< マップB属性配置
	GMD_GAMEDAT_MAPSET_EV,			//!< イベントデータ
	GMD_GAMEDAT_MAPSET_DC,			//!< 装飾データ
	GMD_GAMEDAT_MAPSET_RG,			//!< リングデータ

	GMD_GAMEDAT_MAPSET_MAX,			//!< 通常マップデータ最大値

	// 以下追加面 gmMap.h の定義も増やす
	GMD_GAMEDAT_MAPSET_ADD_START = GMD_GAMEDAT_MAPSET_MAX,
	GMD_GAMEDAT_MAPSET_ADD_N_MP = GMD_GAMEDAT_MAPSET_ADD_START,	//!< 超近景マップ配置
	GMD_GAMEDAT_MAPSET_ADD_N_MD,	//!< 超近景マップオフセット配置
	GMD_GAMEDAT_MAPSET_ADD_M_MP,	//!< 中景マップ配置
	GMD_GAMEDAT_MAPSET_ADD_M_MD,	//!< 中景マップオフセット配置
	GMD_GAMEDAT_MAPSET_ADD_M1_MP,	//!< 中景マップスクロールあり1 配置
	GMD_GAMEDAT_MAPSET_ADD_M1_MD,	//!< 中景マップスクロールあり1 オフセット配置
	GMD_GAMEDAT_MAPSET_ADD_M2_MP,	//!< 中景マップスクロールあり2 配置
	GMD_GAMEDAT_MAPSET_ADD_M2_MD,	//!< 中景マップスクロールあり2 オフセット配置
	GMD_GAMEDAT_MAPSET_ADD_M3_MP,	//!< 中景マップスクロールあり3 配置
	GMD_GAMEDAT_MAPSET_ADD_M3_MD,	//!< 中景マップスクロールあり3 オフセット配置

	GMD_GAMEDAT_MAPSET_ADD_MAX,		//!< 追加マップデータ最大値

	GMD_GAMEDAT_MAPSET_ADD_LOCAL_N_MP = 0,	//!< 超近景マップ配置 ローカルNO
	GMD_GAMEDAT_MAPSET_ADD_LOCAL_N_MD,		//!< 超近景マップオフセット配置 ローカルNO
	GMD_GAMEDAT_MAPSET_ADD_LOCAL_M_MP,		//!< 中景マップ配置 ローカルNO
	GMD_GAMEDAT_MAPSET_ADD_LOCAL_M_MD,		//!< 中景マップオフセット配置 ローカルNO
	GMD_GAMEDAT_MAPSET_ADD_LOCAL_M1_MP,		//!< 中景マップスクロールあり1 配置 ローカルNO
	GMD_GAMEDAT_MAPSET_ADD_LOCAL_M1_MD,		//!< 中景マップスクロールあり1 オフセット配置 ローカルNO
	GMD_GAMEDAT_MAPSET_ADD_LOCAL_M2_MP,		//!< 中景マップスクロールあり2 配置 ローカルNO
	GMD_GAMEDAT_MAPSET_ADD_LOCAL_M2_MD,		//!< 中景マップスクロールあり2 オフセット配置 ローカルNO
	GMD_GAMEDAT_MAPSET_ADD_LOCAL_M3_MP,		//!< 中景マップスクロールあり3 配置 ローカルNO
	GMD_GAMEDAT_MAPSET_ADD_LOCAL_M3_MD,		//!< 中景マップスクロールあり3 オフセット配置 ローカルNO

	GMD_GAMEDAT_MAPSET_ADD_LOCAL_MAX

} GME_GAMEDAT_MAPSET;

/// マップアトリビュートセットデータタイプ
typedef enum tag_GME_GAMEDAT_ATTRSET {
	GMD_GAMEDAT_ATTRSET_AT	= 0,	//!< 属性データ
	GMD_GAMEDAT_ATTRSET_DF,			//!< 差分データ
	GMD_GAMEDAT_ATTRSET_DI,			//!< 角度データ

	GMD_GAMEDAT_ATTRSET_MAX
} GME_GAMEDAT_ATTRSET;


/// ボス連戦用データタイプ
typedef enum tag_GME_GAMEDAT_LOAD_BOSS_TYPE {
	GMD_GAMEDAT_LOAD_BOSS_TYPE_1	= 0,	//!< ボス1
	GMD_GAMEDAT_LOAD_BOSS_TYPE_2,			//!< ボス2
	GMD_GAMEDAT_LOAD_BOSS_TYPE_3,			//!< ボス3
	GMD_GAMEDAT_LOAD_BOSS_TYPE_4,			//!< ボス4
	GMD_GAMEDAT_LOAD_BOSS_TYPE_F,			//!< ボスF

	GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX
} GME_GAMEDAT_LOAD_BOSS_TYPE;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------
/// ステージタイプ設定
extern const GSE_MAIN_STAGE_TYPE g_gm_gamedat_stage_type_tbl[GSD_MAIN_STAGE_ID_MAX];

/// ステージゾーンタイプ設定
extern const GSE_MAIN_ZONE_TYPE g_gm_gamedat_zone_type_tbl[GSD_MAIN_STAGE_ID_MAX];


/* データ読み込みワーク */
extern void	*g_gm_gamedat_map[GMD_GAMEDAT_MAP_MAX];						//!< MAPデータ
extern void	*g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_MAX];				//!< MAPセットデータ
extern void	*g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_MAX];//!< 追加MAPセットデータ
extern void	*g_gm_gamedat_map_attr_set[GMD_GAMEDAT_ATTRSET_MAX];		//!< MAPアトリビュートセットデータ

extern void	*g_gm_gamedat_enemy_arc;								//!< エネミーデータアーカイブ

extern void	*g_gm_gamedat_ring[GMD_DWORK_NO_RING_END - GMD_DWORK_NO_RING_START];		//!< リングデータ
extern void	*g_gm_gamedat_gimmick[GMD_DWORK_NO_GMK_END - GMD_DWORK_NO_GMK_START];		//!< ギミックデータ
extern void	*g_gm_gamedat_enemy[GMD_DWORK_NO_ENEMY_END - GMD_DWORK_NO_ENEMY_START];		//!< エネミーデータ
extern void	*g_gm_gamedat_effect[GMD_DWORK_NO_EFFECT_ARC_END - GMD_DWORK_NO_EFFECT_ARC_START];	//!< エフェクトデータ

extern void	*g_gm_gamedat_cockpit_main_arc;						//!< コックピットデータアーカイブ

extern GSE_MAIN_STAGE_ID g_gm_gamedat_bossbattle_stage_id_tbl[GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX];
//----- External Declarations -----------------------------------------------
// ==========================================================================
// GmGameDatLoadInit
/*!
 *	ゲームデータロード 初期化
 *
 *	@param	stage_id		[in]	ロードステージタイプ
 *	@param	area_id			[in]	ロードエリアタイプ
 *	@param	char_id_list	[in]	ロードキャラクタタイプバッファ
 *
 *	@note
 *		char_id_list は GSD_MAIN_PLAYER_MAX長のバッファである事\n
 *		未使用時は -1を格納
 */
// ==========================================================================
extern void GmGameDatLoadInit(GME_GAMEDAT_LOAD_PROC proc_type, u16 stage_id, s16 *char_id_list);

// ==========================================================================
// GmGameDatLoadPost
/*!
 *	ゲームデータロード 後処理
 *
 *	@note
 *		GMD_GAMEDAT_LOAD_PROC_PRE_LOAD 設定でデータ読み込みをした場合の、
 *		後処理一括実行
 */
// ==========================================================================
extern void GmGameDatLoadPost(void);

// ==========================================================================
// GmGameDatLoadCheck
/*!
 *	ゲームデータロード 読み込みチェック
 *
 *	@return		読み込み状況 GME_GAMEDAT_LOAD_PROGRESS
 */
// ==========================================================================
extern GME_GAMEDAT_LOAD_PROGRESS GmGameDatLoadCheck(void);

// ==========================================================================
// GmGameDatLoadExit
/*!
 *	ゲームデータロード 処理終了
 */
// ==========================================================================
extern void GmGameDatLoadExit(void);

// ==========================================================================
// データリリース
// ==========================================================================
// ==========================================================================
// GmGameDatRelease
/*!
 *	ゲームデータリリース
 *
 *	@note
 *		読み込んだデータを解放したりしています。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。
 */
// ==========================================================================
extern void GmGameDatRelease(void);

// ==========================================================================
// GmGameDatReleaseStandard
/*!
 *	ゲームデータリリース 標準データ
 *
 *	@note
 *		読み込んだデータを解放したりしています。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。
 */
// ==========================================================================
extern void GmGameDatReleaseStandard(void);

// ==========================================================================
// GmGameDatReleaseArea
/*!
 *	ゲームデータリリース エリアデータ
 *
 *	@note
 *		読み込んだデータを解放したりしています。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。
 */
// ==========================================================================
extern void GmGameDatReleaseArea(void);

// ==========================================================================
// GmGameDatReleaseCheck
/*!
 *	ゲームデータリリース 開放チェック
 *
 *	@return		TRUE : 開放処理終了
 */
// ==========================================================================
extern BOOL GmGameDatReleaseCheck(void);

// ==========================================================================
// データ取得
// ==========================================================================
// ==========================================================================
// GmGameDatGetEnemyData
/*!
 *	エネミーデータ取得
 *
 *	@param	data_no	[in]	GMD_DWORK_NO_ENEMY_****
 *
 *	@return		データアドレス
 */
// ==========================================================================
extern void* GmGameDatGetEnemyData(s32 data_no);

// ==========================================================================
// GmGameDatGetGimmickData
/*!
 *	ギミックデータ取得
 *
 *	@param	data_no	[in]	GMD_DWORK_NO_GMK_****
 *
 *	@return		データアドレス
 */
// ==========================================================================
extern void* GmGameDatGetGimmickData(s32 data_no);

// ==========================================================================
// GmGameDatGetCockpitData
/*!
 *	コックピットデータ取得
 *
 *	@param	data_no	[in]	GMD_DWORK_NO_GMK_****
 *
 *	@return		データアドレス
 */
// ==========================================================================
inline void* GmGameDatGetCockpitData(void)
{
	MTM_ASSERT(g_gm_gamedat_cockpit_main_arc);
	return g_gm_gamedat_cockpit_main_arc;
}

// ==========================================================================
// ボス連戦用
// ==========================================================================
// ==========================================================================
// GmGameDatLoadBoosBattleInit
/*!
 *	ゲームデータロード 初期化 ボス連戦用
 *
 *	@param	boss_type		[in]	ロードボスタイプ
 *
 *	@note
 *		GmGameDatLoadCheck でロードチェック
 *		GmGameDatLoadExit でロード終了
 */
// ==========================================================================
extern void GmGameDatLoadBoosBattleInit(GME_GAMEDAT_LOAD_BOSS_TYPE boss_type);

// ==========================================================================
// GmGameDatBoosBattleRelease
/*!
 *	ゲームデータリリース ボス連戦用
 *
 *	@note
 *		読み込んだデータを解放したりしています。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。
 */
// ==========================================================================
extern void GmGameDatBoosBattleRelease(GME_GAMEDAT_LOAD_BOSS_TYPE boss_type);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GAME_DAT_H_

//----- Include Files -------------------------------------------------------
