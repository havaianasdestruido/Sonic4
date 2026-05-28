// =======================================================================
/*!
  @file	gmEffectZone.cpp
  @brief ゾーン専用エフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmEffectZone.cpp 20 2011-04-22 12:46:46Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "objObject.h"
#include "gmMain.h"
#include "gmEffect.h"

#include "gmEffectZone.h"

/*------ Macros --------------------------------------------------------*/
#define GMD_EFCT_ZONE_BUILD_REG_ALLOWANCE_NUM		(48)	//!< 登録可能コマンド数にこの数だけ空きができたらロード開始
#define GMD_EFCT_ZONE_FLUSH_REG_ALLOWANCE_NUM		(8)		//!< 登録可能コマンド数にこの数だけ空きができたらフラッシュ開始

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! ビルド・フラッシュ処理ステート
typedef enum
{
	GME_EFCT_ZONE_PROCESS_STATE_NOP	= 0,		//!< 処理外
	GME_EFCT_ZONE_PROCESS_STATE_WAIT_START,		//!< ロード・フラッシュ開始待ち
	GME_EFCT_ZONE_PROCESS_STATE_WAIT_COMPLETE,	//!< ロード・フラッシュ完了待ち
	
	GME_EFCT_ZONE_PROCESS_STATE_MAX
} GME_EFCT_ZONE_PROCESS_STATE;

//! ゾーン専用エフェクト生成パラメータ構造体
typedef struct tag_GMS_EFCT_ZONE_CREATE_PARAM
{
	GMS_EFFECT_CREATE_PARAM	create_param;	//!< 生成パラメータ
	Sint32	model_dwork_no;	//!< モデルデータ データワーク番号
	Sint32	mdl_ambtex_idx;		//!< テクスチャAMBのアーカイブAMB内のインデックス（モデル用のAMBTEXが別に用意されている場合も個々の変更で対応可能）
} GMS_EFCT_ZONE_CREATE_PARAM;

//! ゾーン専用エフェクト生成情報
typedef struct tag_GMS_EFCT_ZONE_CREATE_INFO
{
	const GMS_EFCT_ZONE_CREATE_PARAM	*zone_create_param;	//!< ゾーン専用EF生成パラメータテーブル
	Sint32								num;				//!< テーブルの項目数
} GMS_EFCT_ZONE_CREATE_INFO;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
static inline Sint32 gmEfctZoneGetAmeDworkNo(GSE_MAIN_ZONE_TYPE zone_no, Sint32 ame_amb_idx);
static inline Sint32 gmEfctZoneGetObjectDworkNo(Sint32 model_dwork_no);
static inline Sint32 gmEfctZoneGetMdlTexlistDworkNo(Sint32 model_dwork_no);
static inline Sint32 gmEfctZoneGetMdlAmbtexDworkNo(Sint32 model_dwork_no);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
static GME_EFCT_ZONE_PROCESS_STATE gm_efct_zone_proc_state	= GME_EFCT_ZONE_PROCESS_STATE_NOP;	//!< ビルド・フラッシュ処理ステート
static Sint32 gm_efct_zone_tex_reg_id	= -1;	//!< テクスチャ登録コマンドID
static Sint32 *gm_efct_zone_model_reg_id_list	= NULL;	//!< モデル登録コマンドIDリスト
static Sint32 gm_efct_zone_model_reg_num	= 0;	//!< モデル登録コマンドID数
static Sint32 *gm_efct_zone_mdl_tex_reg_id_list	= NULL;	//!< モデル用テクスチャ登録コマンドID
static GSE_MAIN_ZONE_TYPE gm_efct_zone_target_zone_no	= GSD_MAIN_ZONE_TYPE_NONE;	//!< ビルド・フラッシュするゾーンの番号

#include "gmEffectZoneTbl.inc"


/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmEfctCmnBuildDataInit
/*!
  ゾーン専用エフェクトデータ構築 初期化
 */
// =======================================================================
void GmEfctZoneBuildDataInit(GSE_MAIN_ZONE_TYPE zone_no)
{
	MTM_ASSERT(gm_efct_zone_target_zone_no == GSD_MAIN_ZONE_TYPE_NONE);
	
	gm_efct_zone_proc_state	= GME_EFCT_ZONE_PROCESS_STATE_WAIT_START;
	
	// ゾーン番号格納
	gm_efct_zone_target_zone_no	= zone_no;
}

// =======================================================================
// GmEfctZoneBuildDataLoopInit
/*!
  ゾーン専用エフェクトデータ構築 開始
  
  @note
  実際にロードコマンドを発行します。
 */
// =======================================================================
void GmEfctZoneBuildDataLoopInit(void)
{
	OBS_DATA_WORK	*arc_dwork;
	void			*eff_zone_arc;
	OBS_DATA_WORK	*ambtex_dwork;
	void			*texlistbuf;
	const GMS_EFCT_ZONE_CREATE_INFO	*create_info	= &gm_efct_zone_create_info[gm_efct_zone_target_zone_no];
	Sint32			model_reg_cnt	= 0;
	
	MTM_ASSERT(gm_efct_zone_tex_reg_id == -1);
	MTM_ASSERT(gm_efct_zone_model_reg_id_list == NULL);
	MTM_ASSERT(gm_efct_zone_model_reg_num == 0);
	MTM_ASSERT(gm_efct_zone_mdl_tex_reg_id_list == NULL);
	
	MTM_ASSERT(gm_efct_zone_target_zone_no != GSD_MAIN_ZONE_TYPE_NONE);
	
	MTM_ASSERT(gm_efct_zone_proc_state == GME_EFCT_ZONE_PROCESS_STATE_WAIT_START);
	
	// ゾーン専用エフェクトアーカイブ取得
	arc_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_ZONE_ARC);
	MTM_ASSERT(arc_dwork->num > 0);
	eff_zone_arc	= ObjDataGetInc(arc_dwork);
	
	// ゾーン別に使用モデル数を取得するのは手間なので、ゾーン専用エフェクトのモデルデータワーク数合計を使用
	gm_efct_zone_model_reg_num	= GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_END - GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_START;
	
	// モデル登録コマンドIDリスト・モデル用テクスチャ登録コマンドIDリスト確保
	if (gm_efct_zone_model_reg_num > 0) {
		gm_efct_zone_model_reg_id_list	= (Sint32*)amMemAlloc(sizeof(Sint32) * gm_efct_zone_model_reg_num);
		gm_efct_zone_mdl_tex_reg_id_list	= (Sint32*)amMemAlloc(sizeof(Sint32) * gm_efct_zone_model_reg_num);
		// -1 で埋める
		memset(gm_efct_zone_model_reg_id_list, -1, sizeof(Sint32) * gm_efct_zone_model_reg_num);
		memset(gm_efct_zone_mdl_tex_reg_id_list, -1, sizeof(Sint32) * gm_efct_zone_model_reg_num);
	}
	
	// 共通エフェクトテクスチャAMB取得
	ambtex_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_ZONE_AMBTEX);
	ObjDataLoadAmbIndex(ambtex_dwork,
						gm_efct_zone_texamb_index_tbl[gm_efct_zone_target_zone_no],
						eff_zone_arc);
	
	// テクスチャVRAMロード開始
	gm_efct_zone_tex_reg_id	=
		ObjAction3dESTextureLoadToDwork(ObjDataGet(GMD_DWORK_NO_EFFECT_ZONE_TEXLIST),
										ambtex_dwork->pData,
										&texlistbuf);
	
	
	// モデル毎処理
	for (Sint32 i = 0; i < create_info->num; ++i) {
		Sint32 model_amb_index	= create_info->zone_create_param[i].create_param.model_idx;
		Sint32 model_dwork_no	= create_info->zone_create_param[i].model_dwork_no;
		Sint32 object_dwork_no	= gmEfctZoneGetObjectDworkNo(model_dwork_no);
		Sint32 mdl_ambtex_amb_index	= create_info->zone_create_param[i].mdl_ambtex_idx;
		Sint32 mdl_ambtex_dwork_no	= gmEfctZoneGetMdlAmbtexDworkNo(model_dwork_no);
		Sint32 mdl_texlist_dwork_no	= gmEfctZoneGetMdlTexlistDworkNo(model_dwork_no);
		
		if (model_amb_index != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
			
			MTM_ASSERT(model_reg_cnt < gm_efct_zone_model_reg_num);
			MTM_ASSERT(-1 != mdl_ambtex_amb_index);
			MTM_ASSERT(model_dwork_no < GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_END);
			MTM_ASSERT(object_dwork_no < GMD_DWORK_NO_EFFECT_ZONE_OBJECT_END);
			MTM_ASSERT(mdl_ambtex_dwork_no < GMD_DWORK_NO_EFFECT_ZONE_MDL_AMBTEX_END);
			MTM_ASSERT(mdl_texlist_dwork_no < GMD_DWORK_NO_EFFECT_ZONE_MDL_TEXLIST_END);
			
			// ゾーン専用エフェクトモデル用テクスチャAMB取得
			ambtex_dwork	= ObjDataGet(mdl_ambtex_dwork_no);
			ObjDataLoadAmbIndex(ambtex_dwork,
								mdl_ambtex_amb_index,
								eff_zone_arc);
			
			// テクスチャVRAMロード開始
			gm_efct_zone_mdl_tex_reg_id_list[model_reg_cnt]	=
				ObjAction3dESTextureLoadToDwork(ObjDataGet(mdl_texlist_dwork_no),
												ambtex_dwork->pData,
												&texlistbuf);
			
			// ゾーン専用エフェクトモデルデータ取得
			ObjDataLoadAmbIndex(ObjDataGet(model_dwork_no),
								model_amb_index,
								eff_zone_arc);
			
			// モデルVRAMロード開始
			gm_efct_zone_model_reg_id_list[model_reg_cnt]	=
				ObjAction3dESModelLoadToDwork(ObjDataGet(object_dwork_no),
											  ObjDataGet(model_dwork_no)->pData,
											  0);
			
			// モデル登録カウント
			model_reg_cnt++;
		}
	}
}

// =======================================================================
// GmEfctZoneBuildDataLoop
/*!
  ゾーン専用エフェクトデータ構築 ループ
  
  @retval TRUE	構築完了
  @retval FALSE	構築中
 */
// =======================================================================
BOOL GmEfctZoneBuildDataLoop(void)
{
	BOOL	result	= TRUE;
	
	if (gm_efct_zone_target_zone_no == GSD_MAIN_ZONE_TYPE_NONE) {
		return result;
	}
	
	// NOPステートの場合（ビルド完了後など）は何もしない
	if (gm_efct_zone_proc_state == GME_EFCT_ZONE_PROCESS_STATE_NOP) {
		return TRUE;
	}
	
	if (gm_efct_zone_proc_state == GME_EFCT_ZONE_PROCESS_STATE_WAIT_START) {
		// 余裕が出来るまで待つ
		if (GsMainSysGetDisplayListRegistNum() < AMD_REGISTLIST_NUM - GMD_EFCT_ZONE_BUILD_REG_ALLOWANCE_NUM) {
			GmEfctZoneBuildDataLoopInit();
			gm_efct_zone_proc_state	= GME_EFCT_ZONE_PROCESS_STATE_WAIT_COMPLETE;
		}
		
		return FALSE;
	}
	
	// テクスチャロード完了チェック
	if (gm_efct_zone_tex_reg_id != -1) {
		if (amDrawIsRegistComplete(gm_efct_zone_tex_reg_id)) {
			gm_efct_zone_tex_reg_id	= -1;
		}
		else {
			result	= FALSE;
		}
	}
	
	// モデルロード完了チェック
	for (Sint32 i = 0; i < gm_efct_zone_model_reg_num; ++i) {
		
		// モデル用テクスチャ
		if (gm_efct_zone_mdl_tex_reg_id_list[i] != -1) {
			if (amDrawIsRegistComplete(gm_efct_zone_mdl_tex_reg_id_list[i])) {
				gm_efct_zone_mdl_tex_reg_id_list[i]	= -1;
			}
			else {
				result	= FALSE;
			}
		}
		
		// モデル自体
		if (gm_efct_zone_model_reg_id_list[i] != -1) {
			if (amDrawIsRegistComplete(gm_efct_zone_model_reg_id_list[i])) {
				gm_efct_zone_model_reg_id_list[i]	= -1;
			}
			else {
				result	= FALSE;
			}
		}
	}
	
	if (result) {
		// 対象ゾーン番号クリア
		gm_efct_zone_target_zone_no	= GSD_MAIN_ZONE_TYPE_NONE;
		
		// ロードステートクリア
		gm_efct_zone_proc_state	= GME_EFCT_ZONE_PROCESS_STATE_NOP;
	}
	else {
		MTM_ASSERT(gm_efct_zone_target_zone_no != GSD_MAIN_ZONE_TYPE_NONE);
	}
	
	return result;
}

// =======================================================================
// GmEfctZoneFlushDataInit
/*!
  ゾーン専用エフェクトデータ後片付け 初期化
 */
// =======================================================================
void GmEfctZoneFlushDataInit(GSE_MAIN_ZONE_TYPE zone_no)
{
	MTM_ASSERT(gm_efct_zone_target_zone_no == GSD_MAIN_ZONE_TYPE_NONE);
	
	gm_efct_zone_proc_state	= GME_EFCT_ZONE_PROCESS_STATE_WAIT_START;
	
	// ゾーン番号格納
	gm_efct_zone_target_zone_no	= zone_no;
}

// =======================================================================
// GmEfctZoneFlushDataLoopInit
/*!
  ゾーン専用エフェクトデータ後片付け 開始
  
  @note
  実際に解放コマンドを発行します。
 */
// =======================================================================
void GmEfctZoneFlushDataLoopInit(void)
{
	OBS_DATA_WORK	*arc_dwork;
	OBS_DATA_WORK	*ambtex_dwork;
	OBS_DATA_WORK	*model_dwork;
	const GMS_EFCT_ZONE_CREATE_INFO	*create_info	= &gm_efct_zone_create_info[gm_efct_zone_target_zone_no];
	Sint32			model_reg_cnt	= 0;
	
	MTM_ASSERT(gm_efct_zone_target_zone_no != GSD_MAIN_ZONE_TYPE_NONE);
	
	MTM_ASSERT(gm_efct_zone_proc_state == GME_EFCT_ZONE_PROCESS_STATE_WAIT_START);
	
	// ゾーンの全てのエフェクト種類数分ループして解放
	for (Sint32 i = 0; i < create_info->num; ++i) {
		const GMS_EFCT_ZONE_CREATE_PARAM	*zcr_param	= &create_info->zone_create_param[i];
		Sint32 model_dwork_no	= zcr_param->model_dwork_no;
		Sint32 object_dwork_no	= gmEfctZoneGetObjectDworkNo(model_dwork_no);
		MTM_ASSERT(model_dwork_no < GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_END);
		MTM_ASSERT(object_dwork_no < GMD_DWORK_NO_EFFECT_ZONE_OBJECT_END);
		
		if (zcr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
			
			MTM_ASSERT(model_reg_cnt < gm_efct_zone_model_reg_num);
			
			// モデルVRAM解放開始
			gm_efct_zone_model_reg_id_list[model_reg_cnt]	=
				ObjAction3dESModelReleaseDwork(ObjDataGet(object_dwork_no));
			
			// ゾーン専用エフェクトモデルデータ解放（実際には参照カウントデクリメントのみ）
			model_dwork	= ObjDataGet(model_dwork_no);
			ObjDataRelease(model_dwork);
			
			// モデル用テクスチャVRAM解放開始
			gm_efct_zone_mdl_tex_reg_id_list[model_reg_cnt]	=
				ObjAction3dESTextureReleaseDwork(ObjDataGet(gmEfctZoneGetMdlTexlistDworkNo(model_dwork_no)));
			
			// モデル用テクスチャAMB解放（実際には参照カウントデクリメントのみ）
			ObjDataRelease(ObjDataGet(gmEfctZoneGetMdlAmbtexDworkNo(model_dwork_no)));
			
			// モデル登録カウント
			model_reg_cnt++;
		}
	}
	
	
	// テクスチャVRAM解放開始
	gm_efct_zone_tex_reg_id	=
		ObjAction3dESTextureReleaseDwork(ObjDataGet(GMD_DWORK_NO_EFFECT_ZONE_TEXLIST));
	
	// ゾーン専用エフェクトテクスチャAMB解放（実際には参照カウントデクリメントのみ）
	ambtex_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_ZONE_AMBTEX);
	ObjDataRelease(ambtex_dwork);
	
	// ゾーン専用エフェクトアーカイブ解放（実際には参照カウントデクリメントのみ）
	arc_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_ZONE_ARC);
	ObjDataRelease(arc_dwork);
}

// =======================================================================
// GmEfctZoneFlushDataLoop
/*!
  ゾーン専用エフェクトデータ後片付け ループ
  
  @retval TRUE	後片付け完了
  @retval FALSE	後片付け中
 */
// =======================================================================
BOOL GmEfctZoneFlushDataLoop(void)
{
	BOOL	result	= TRUE;
	const GMS_EFCT_ZONE_CREATE_INFO	*create_info;
	Sint32	model_reg_cnt	= 0;
	
	// NOPステートの場合（フラッシュ完了後など）は何もしない
	if (gm_efct_zone_proc_state == GME_EFCT_ZONE_PROCESS_STATE_NOP) {
		return TRUE;
	}
	
	if (gm_efct_zone_proc_state == GME_EFCT_ZONE_PROCESS_STATE_WAIT_START) {
		// 余裕が出来るまで待つ
		if (GsMainSysGetDisplayListRegistNum() < AMD_REGISTLIST_NUM - GMD_EFCT_ZONE_FLUSH_REG_ALLOWANCE_NUM) {
			GmEfctZoneFlushDataLoopInit();
			gm_efct_zone_proc_state	= GME_EFCT_ZONE_PROCESS_STATE_WAIT_COMPLETE;
		}
		
		return FALSE;
	}
	
	// 生成情報取得
	create_info	= &gm_efct_zone_create_info[gm_efct_zone_target_zone_no];
	
	if (gm_efct_zone_model_reg_num != 0) {	// 無駄にループしないようにする
		
		// モデル解放チェック
		for (Sint32 i = 0; i < create_info->num; ++i) {
			const GMS_EFCT_ZONE_CREATE_PARAM	*zcr_param	= &create_info->zone_create_param[i];
			Sint32 model_dwork_no	= zcr_param->model_dwork_no;
			Sint32 object_dwork_no	= gmEfctZoneGetObjectDworkNo(model_dwork_no);
			
			if (zcr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
				
				if (gm_efct_zone_model_reg_id_list[model_reg_cnt] != -1) {
					
					MTM_ASSERT(object_dwork_no < GMD_DWORK_NO_EFFECT_ZONE_OBJECT_END);
					
					if (ObjAction3dESModelReleaseDworkCheck(ObjDataGet(object_dwork_no),
															gm_efct_zone_model_reg_id_list[model_reg_cnt])) {
						gm_efct_zone_model_reg_id_list[model_reg_cnt]	= -1;
					}
					else {
						result	= FALSE;
					}
				}
				
				// モデル用テクスチャ
				if (gm_efct_zone_mdl_tex_reg_id_list[model_reg_cnt] != -1) {
					if (ObjAction3dESTextureReleaseDworkCheck(ObjDataGet(gmEfctZoneGetMdlTexlistDworkNo(model_dwork_no)),
															  gm_efct_zone_mdl_tex_reg_id_list[model_reg_cnt])) {
						gm_efct_zone_mdl_tex_reg_id_list[model_reg_cnt]	= -1;
					}
					else {
						result	= FALSE;
					}
				}
				
				// モデル登録カウント
				model_reg_cnt++;
			}
		}
	}
	else {
		MTM_ASSERT(NULL == gm_efct_zone_mdl_tex_reg_id_list);
		MTM_ASSERT(NULL == gm_efct_zone_model_reg_id_list);
		MTM_ASSERT(GSD_MAIN_ZONE_TYPE_NONE == gm_efct_zone_target_zone_no);
	}
	
	
	// テクスチャ解放チェック
	if (gm_efct_zone_tex_reg_id != -1) {
		if (ObjAction3dESTextureReleaseDworkCheck(ObjDataGet(GMD_DWORK_NO_EFFECT_ZONE_TEXLIST),
												  gm_efct_zone_tex_reg_id)) {
			gm_efct_zone_tex_reg_id	= -1;
		}
		else {
			result	= FALSE;
		}
	}
	
	if (result) {
		
		if (gm_efct_zone_mdl_tex_reg_id_list) {
			amMemFree(gm_efct_zone_mdl_tex_reg_id_list);
			gm_efct_zone_mdl_tex_reg_id_list	= NULL;
			// reg_numはmodel_reg_id_listと共用
		}
		
		if (gm_efct_zone_model_reg_id_list) {
			amMemFree(gm_efct_zone_model_reg_id_list);
			gm_efct_zone_model_reg_id_list	= NULL;
			gm_efct_zone_model_reg_num	= 0;
		}
		
		
		// 対象ゾーン番号クリア
		gm_efct_zone_target_zone_no	= GSD_MAIN_ZONE_TYPE_NONE;
		
		// ロードステートクリア
		gm_efct_zone_proc_state	= GME_EFCT_ZONE_PROCESS_STATE_NOP;
	}
	else {
		MTM_ASSERT(gm_efct_zone_target_zone_no != GSD_MAIN_ZONE_TYPE_NONE);
	}
	
	return result;
}

// =======================================================================
// GmEfctZoneEsCreate
/*!
  ゾーン専用エフェクト（ESタイプ）生成
  
  @param parent_obj		[io]	親オブジェクト（NULL可）
  @param zone_no		[in]	ゾーン番号
  @param efct_zone_idx	[in]	エフェクトインデックス(GME_EFCT_Z**_IDX_**)
  
  @return エフェクト3DESワーク
  
  @note
  ゾーン毎のインデックス範囲を超えたefct_zone_idxを指定するとアサートします。
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* GmEfctZoneEsCreate(OBS_OBJECT_WORK *parent_obj,
										 GSE_MAIN_ZONE_TYPE zone_no, Sint32 efct_zone_idx)
{
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	GMS_EFFECT_3DES_WORK	*eff_3des;
	const GMS_EFCT_ZONE_CREATE_INFO	*create_info	= &gm_efct_zone_create_info[zone_no];
	const GMS_EFCT_ZONE_CREATE_PARAM	*zcr_param;
	OBS_DATA_WORK	*model_data_work;
	OBS_DATA_WORK	*object_data_work;
	OBS_DATA_WORK	*ambtex_data_work;
	OBS_DATA_WORK	*texlist_data_work;
	
	MTM_ASSERT(create_info != NULL);
	MTM_ASSERT(efct_zone_idx < create_info->num);
	
	// 指定インデックスの生成情報取得
	zcr_param	= &create_info->zone_create_param[efct_zone_idx];
	
	if (zcr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
		Sint32 model_dwork_no	= zcr_param->model_dwork_no;
		Sint32 object_dwork_no	= gmEfctZoneGetObjectDworkNo(model_dwork_no);
		
		// モデルデータを格納するデータワーク取得
		model_data_work	= ObjDataGet(model_dwork_no);
		
		// オブジェクトを格納するデータワークを取得
		object_data_work	= ObjDataGet(object_dwork_no);
		
		// テクスチャAMBを格納するデータワーク取得
		ambtex_data_work	= ObjDataGet(gmEfctZoneGetMdlAmbtexDworkNo(model_dwork_no));
		
		// テクスチャリストを格納するデータワーク取得
		texlist_data_work	= ObjDataGet(gmEfctZoneGetMdlTexlistDworkNo(model_dwork_no));
	}
	else {
		model_data_work	= NULL;
		object_data_work	= NULL;
		ambtex_data_work	= ObjDataGet(GMD_DWORK_NO_EFFECT_ZONE_AMBTEX);
		texlist_data_work	= ObjDataGet(GMD_DWORK_NO_EFFECT_ZONE_TEXLIST);
	}
	
	eff_3des	= GmEffect3dESCreateByParam(&zcr_param->create_param,
											parent_obj,
											ObjDataGet(GMD_DWORK_NO_EFFECT_ZONE_ARC)->pData,
											ObjDataGet(
												gmEfctZoneGetAmeDworkNo(zone_no,
																		zcr_param->create_param.ame_idx)),
											ambtex_data_work,
											texlist_data_work,
											model_data_work,
											object_data_work);
	
	return eff_3des;
#else
	return GmEffect3dESCreateDummy(parent_obj);
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
}


/*------ Static Functions ----------------------------------------------*/
// =======================================================================
// gmEfctZoneGetAmeDworkNo
/*!
  ゾーン専用エフェクト 指定エフェクトのAME用データワーク番号取得
  
  @param zone_no		[in]	ゾーン番号
  @param ame_amb_idx	[in]	AMEのAMB内でのインデックス番号
  
  @return AME用データワークの番号
 */
// =======================================================================
inline Sint32 gmEfctZoneGetAmeDworkNo(GSE_MAIN_ZONE_TYPE zone_no, Sint32 ame_amb_idx)
{
	Sint32 zone_dwork_start_no	= GMD_DWORK_NO_EFFECT_ZONE_AME_00;
	for (Sint32 i = 0; i < zone_no; ++i) {
		zone_dwork_start_no	+= gm_efct_zone_texamb_index_tbl[i];
	}
	
	return (zone_dwork_start_no + ame_amb_idx);
}

// =======================================================================
// gmEfctZoneGetObjectDworkNo
/*!
  ゾーン専用エフェクト オブジェクトのデータワークの番号取得
  
  @param model_dwork_no	[in]	モデルデータのデータワーク番号
  
  @return オブジェクトのデータワーク番号
  
  @note
  モデル用データワーク番号から、対応するオブジェクト用データワーク番号を取得します。
 */
// =======================================================================
inline Sint32 gmEfctZoneGetObjectDworkNo(Sint32 model_dwork_no)
{
	Sint32	object_dwork_no;
	object_dwork_no	= GMD_DWORK_NO_EFFECT_ZONE_OBJECT_START + (model_dwork_no - GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_START);
	
	MTM_ASSERT(object_dwork_no < GMD_DWORK_NO_EFFECT_ZONE_OBJECT_END);
	
	return object_dwork_no;
}

// =======================================================================
// gmEfctZoneGetMdlAmbtexDworkNo
/*!
  ゾーン専用エフェクト モデル用テクスチャAMBのデータワーク番号取得
  
  @param model_dwork_no	[in]	モデルデータのデータワーク番号
  
  @return モデル用テクスチャAMBのデータワーク番号
 
  @note
  モデルのデータワーク番号から、対応するモデル用テクスチャAMBのデータワーク番号を取得します。
 */
// =======================================================================
inline Sint32 gmEfctZoneGetMdlAmbtexDworkNo(Sint32 model_dwork_no)
{
	Sint32	mdl_ambtex_dwork_no;
	mdl_ambtex_dwork_no	= GMD_DWORK_NO_EFFECT_ZONE_MDL_AMBTEX_START + (model_dwork_no - GMD_DWORK_NO_EFFECT_ZONE_MDL_AMBTEX_START);
	
	MTM_ASSERT(mdl_ambtex_dwork_no < GMD_DWORK_NO_EFFECT_ZONE_MDL_AMBTEX_END);
	
	return mdl_ambtex_dwork_no;
}

// =======================================================================
// gmEfctZoneGetMdlTexlistDworkNo
/*!
  ゾーン専用エフェクト モデル用テクスチャリストのデータワーク番号取得
 
  @param model_dwork_no	[in]	モデルデータのデータワーク番号
 
  @return モデル用テクスチャリストのデータワーク番号
 
  @note
  モデルのデータワーク番号から、対応するモデル用テクスチャリストのデータワーク番号を取得します。
 */
// =======================================================================
inline Sint32 gmEfctZoneGetMdlTexlistDworkNo(Sint32 model_dwork_no)
{
	Sint32 mdl_texlist_dwork_no;
	mdl_texlist_dwork_no = GMD_DWORK_NO_EFFECT_ZONE_MDL_TEXLIST_START + (model_dwork_no - GMD_DWORK_NO_EFFECT_ZONE_MDL_TEXLIST_START);
	
	MTM_ASSERT(mdl_texlist_dwork_no < GMD_DWORK_NO_EFFECT_ZONE_MDL_TEXLIST_END);
	
	return mdl_texlist_dwork_no;
}


// =======================================================================
// GmEfctZoneStaticVarInit
/*!
 static変数の初期化
 */
// =======================================================================
void GmEfctZoneStaticVarInit(void)
{
	gm_efct_zone_proc_state = GME_EFCT_ZONE_PROCESS_STATE_NOP;	//!< ビルド・フラッシュ処理ステート
	gm_efct_zone_tex_reg_id = -1;								//!< テクスチャ登録コマンドID
	gm_efct_zone_model_reg_id_list = NULL;						//!< モデル登録コマンドIDリスト
	gm_efct_zone_model_reg_num = 0;								//!< モデル登録コマンドID数
	gm_efct_zone_mdl_tex_reg_id_list = NULL;					//!< モデル用テクスチャ登録コマンドID
	gm_efct_zone_target_zone_no = GSD_MAIN_ZONE_TYPE_NONE;		//!< ビルド・フラッシュするゾーンの番号
}

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
