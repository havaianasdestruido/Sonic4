// =======================================================================
/*!
  @file	gmEffectBoss.cpp
  @brief ボスエフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmEffectBoss.cpp 2 2011-04-11 05:21:26Z thamada $
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

#include "gmEffectBoss.h"

/*------ Macros --------------------------------------------------------*/
#define GMD_EFCT_BOSS_SINGLE_BUILD_NUM_MAX	(32)		//!< 単体ビルド可能最大数（＝ボス専用エフェクトロード可能最大数）

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
typedef struct tag_GMS_EFCT_BOSS_CMN_CREATE_PARAM
{
	GMS_EFFECT_CREATE_PARAM	create_param;	//!< 生成パラメータ
	Sint32	mdl_ambtex_idx;		//!< テクスチャAMBのアーカイブAMB内のインデックス（モデル用のAMBTEXが別に用意されている場合も個々の変更で対応可能）
} GMS_EFCT_BOSS_CMN_CREATE_PARAM;

//! ボスエフェクト構築ワーク（各ボス専用エフェクト用）
typedef struct tag_GMS_EFCT_BOSS_SINGLE_BUILD_WORK
{
	Sint32	tex_reg_id;
	Sint32	model_reg_id;
	OBS_DATA_WORK	*ambtex_dwork;
	OBS_DATA_WORK	*texlist_dwork;
	OBS_DATA_WORK	*model_dwork;
	OBS_DATA_WORK	*object_dwork;
} GMS_EFCT_BOSS_SINGLE_BUILD_WORK;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
static inline Sint32 gmEfctBossCmnGetAmeDworkNo(Sint32 ame_idx);
static inline Sint32 gmEfctBossCmnGetModelDworkNo(Sint32 ame_idx);
static inline Sint32 gmEfctBossCmnGetObjectDworkNo(Sint32 ame_idx);
static inline Sint32 gmEfctBossCmnGetMdlAmbtexDworkNo(Sint32 ame_idx);
static inline Sint32 gmEfctBossCmnGetMdlTexlistDworkNo(Sint32 ame_idx);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
static Sint32 gm_efct_boss_cmn_tex_reg_id	= -1;	//!< テクスチャ登録コマンドID
static Sint32 *gm_efct_boss_cmn_model_reg_id_list	= NULL;	//!< モデル登録コマンドIDリスト
static Sint32 gm_efct_boss_cmn_model_reg_num	= 0;	//!< モデル登録コマンドID数
static Sint32 *gm_efct_boss_cmn_mdl_tex_reg_id_list	= NULL;	//!< モデル用テクスチャ登録コマンドID

static Sint32 gm_efct_boss_single_reg_num	= 0;	//!< エフェクト単体登録数
static GMS_EFCT_BOSS_SINGLE_BUILD_WORK gm_efct_boss_single_build_list[GMD_EFCT_BOSS_SINGLE_BUILD_NUM_MAX]	= {{0}};	//!< エフェクト単体ビルドワークリスト

#include "gmEffectBossTbl.inc"

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmEfctBosssCmnBuildDataInit
/*!
  ボス共通エフェクトデータ構築 開始
 */
// =======================================================================
void GmEfctBossCmnBuildDataInit(void)
{
	OBS_DATA_WORK	*arc_dwork;
	void			*eff_bscmn_arc;
	OBS_DATA_WORK	*ambtex_dwork;
	void			*texlistbuf;
	Sint32			model_reg_cnt	= 0;
	
	MTM_ASSERT(gm_efct_boss_cmn_tex_reg_id == -1);
	MTM_ASSERT(gm_efct_boss_cmn_model_reg_id_list == NULL);
	MTM_ASSERT(gm_efct_boss_cmn_model_reg_num == 0);
	MTM_ASSERT(gm_efct_boss_cmn_mdl_tex_reg_id_list == NULL);
	
	// 共通エフェクトアーカイブ取得
	arc_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC);
	MTM_ASSERT(arc_dwork->num > 0);
	eff_bscmn_arc	= ObjDataGetInc(arc_dwork);
	
	// モデルの数だけを取得するのは手間なので、共通エフェクトのモデルデータワーク数を使用（＝AMEの数）
	gm_efct_boss_cmn_model_reg_num	= GMD_DWORK_NO_EFFECT_BOSS_CMN_MODELDAT_END - GMD_DWORK_NO_EFFECT_BOSS_CMN_MODELDAT_START;
	
	// モデル登録コマンドIDリスト確保
	if (gm_efct_boss_cmn_model_reg_num > 0) {
		gm_efct_boss_cmn_model_reg_id_list	= (Sint32*)amMemAlloc(sizeof(Sint32) * gm_efct_boss_cmn_model_reg_num);
		gm_efct_boss_cmn_mdl_tex_reg_id_list	= (Sint32*)amMemAlloc(sizeof(Sint32) * gm_efct_boss_cmn_model_reg_num);
		// -1 で埋める
		memset(gm_efct_boss_cmn_model_reg_id_list, -1, sizeof(Sint32) * gm_efct_boss_cmn_model_reg_num);
		memset(gm_efct_boss_cmn_mdl_tex_reg_id_list, -1, sizeof(Sint32) * gm_efct_boss_cmn_model_reg_num);
	}
	
	// 共通エフェクトテクスチャAMB取得
	ambtex_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_BOSS_CMN_AMBTEX);
	ObjDataLoadAmbIndex(ambtex_dwork,
						IDB_EFF_BS_CMN_EFF_BS_TEX_AMB,
						eff_bscmn_arc);
	
	// テクスチャVRAMロード開始
	gm_efct_boss_cmn_tex_reg_id	=
		ObjAction3dESTextureLoadToDwork(ObjDataGet(GMD_DWORK_NO_EFFECT_BOSS_CMN_TEXLIST),
										ambtex_dwork->pData,
										&texlistbuf);
	
	// モデル毎処理
	for (Sint32 i = 0; i < GME_EFCT_BOSS_CMN_IDX_MAX; ++i) {
		const GMS_EFCT_BOSS_CMN_CREATE_PARAM	*bcr_param	= &gm_efct_boss_cmn_create_param_tbl[i];
		Sint32 model_amb_index	= bcr_param->create_param.model_idx;
		Sint32 model_dwork_no	= gmEfctBossCmnGetModelDworkNo(i);
		Sint32 object_dwork_no	= gmEfctBossCmnGetObjectDworkNo(i);
		Sint32 mdl_ambtex_amb_index	= bcr_param->mdl_ambtex_idx;
		Sint32 mdl_ambtex_dwork_no	= gmEfctBossCmnGetMdlAmbtexDworkNo(i);
		Sint32 mdl_texlist_dwork_no	= gmEfctBossCmnGetMdlTexlistDworkNo(i);
		
		if (model_amb_index != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
			
			MTM_ASSERT(model_reg_cnt < gm_efct_boss_cmn_model_reg_num);
			MTM_ASSERT(-1 != mdl_ambtex_amb_index);
			MTM_ASSERT(model_dwork_no < GMD_DWORK_NO_EFFECT_BOSS_CMN_MODELDAT_END);
			MTM_ASSERT(object_dwork_no < GMD_DWORK_NO_EFFECT_BOSS_CMN_OBJECT_END);
			MTM_ASSERT(mdl_ambtex_dwork_no < GMD_DWORK_NO_EFFECT_BOSS_CMN_MDL_AMBTEX_END);
			MTM_ASSERT(mdl_texlist_dwork_no < GMD_DWORK_NO_EFFECT_BOSS_CMN_MDL_TEXLIST_END);
			
			// 共通エフェクトモデル用テクスチャAMB取得
			ambtex_dwork	= ObjDataGet(mdl_ambtex_dwork_no);
			ObjDataLoadAmbIndex(ambtex_dwork,
								mdl_ambtex_amb_index,
								eff_bscmn_arc);
			
			// テクスチャVRAMロード開始
			gm_efct_boss_cmn_mdl_tex_reg_id_list[model_reg_cnt] =
				ObjAction3dESTextureLoadToDwork(ObjDataGet(mdl_texlist_dwork_no),
												ambtex_dwork->pData,
												&texlistbuf);
			
			// 共通エフェクトモデルデータ取得
			ObjDataLoadAmbIndex(ObjDataGet(model_dwork_no),
								model_amb_index,
								eff_bscmn_arc);
			
			// モデルVRAMロード開始
			gm_efct_boss_cmn_model_reg_id_list[model_reg_cnt]	=
				ObjAction3dESModelLoadToDwork(ObjDataGet(object_dwork_no),
											  ObjDataGet(model_dwork_no)->pData,
											  0);
			
			// モデル登録カウント
			model_reg_cnt++;
		}
	}
}


// =======================================================================
// GmEfctBossCmnBuildDataLoop
/*!
  ボス共通エフェクトデータ構築 ループ
  
  @retval TRUE	構築完了
  @retval FALSE	構築中
 */
// =======================================================================
BOOL GmEfctBossCmnBuildDataLoop(void)
{
	BOOL	result	= TRUE;
	
	// テクスチャロード完了チェック
	if (gm_efct_boss_cmn_tex_reg_id != -1) {
		if (amDrawIsRegistComplete(gm_efct_boss_cmn_tex_reg_id)) {
			gm_efct_boss_cmn_tex_reg_id	= -1;
		}
		else {
			result	= FALSE;
		}
	}
	
	// モデルロード完了チェック
	for (Sint32 i = 0; i < gm_efct_boss_cmn_model_reg_num; ++i) {
		
		// モデル用テクスチャ
		if (gm_efct_boss_cmn_mdl_tex_reg_id_list[i] != -1) {
			if (amDrawIsRegistComplete(gm_efct_boss_cmn_mdl_tex_reg_id_list[i])) {
				gm_efct_boss_cmn_mdl_tex_reg_id_list[i]	= -1;
			}
			else {
				result	= FALSE;
			}
		}
		
		// モデル自体
		if (gm_efct_boss_cmn_model_reg_id_list[i] != -1) {
			if (amDrawIsRegistComplete(gm_efct_boss_cmn_model_reg_id_list[i])) {
				gm_efct_boss_cmn_model_reg_id_list[i]	= -1;
			}
			else {
				result	= FALSE;
			}
		}
	}
	
	return result;
}

// =======================================================================
// GmEfctBossCmnFlushDataInit
/*!
  ボス共通エフェクトデータ後片付け 開始
 */
// =======================================================================
void GmEfctBossCmnFlushDataInit(void)
{
	OBS_DATA_WORK	*arc_dwork;
	OBS_DATA_WORK	*ambtex_dwork;
	OBS_DATA_WORK	*model_dwork;
	Sint32			model_reg_cnt	= 0;
	
	MTM_ASSERT(gm_efct_boss_cmn_tex_reg_id == -1);
	
	
	// 全てのエフェクト種類数分ループして解放
	for (Sint32 i = 0; i < GME_EFCT_BOSS_CMN_IDX_MAX; ++i) {
		const GMS_EFCT_BOSS_CMN_CREATE_PARAM	*bcr_param	= &gm_efct_boss_cmn_create_param_tbl[i];
		Sint32	model_dwork_no	= gmEfctBossCmnGetModelDworkNo(i);
		Sint32	object_dwork_no	= gmEfctBossCmnGetObjectDworkNo(i);
		
		MTM_ASSERT(model_dwork_no < GMD_DWORK_NO_EFFECT_BOSS_CMN_MODELDAT_END);
		MTM_ASSERT(object_dwork_no < GMD_DWORK_NO_EFFECT_BOSS_CMN_OBJECT_END);
		
		if (bcr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
			// モデルVRAM解放開始
			gm_efct_boss_cmn_model_reg_id_list[model_reg_cnt]	=
				ObjAction3dESModelReleaseDwork(ObjDataGet(object_dwork_no));
			
			// 共通エフェクトモデルデータ解放（実際には参照カウントデクリメントのみ）
			model_dwork	= ObjDataGet(model_dwork_no);
			ObjDataRelease(model_dwork);
			
			// モデル用テクスチャVRAM解放開始
			gm_efct_boss_cmn_mdl_tex_reg_id_list[model_reg_cnt]	=
				ObjAction3dESTextureReleaseDwork(ObjDataGet(gmEfctBossCmnGetMdlTexlistDworkNo(i)));
			
			// モデル用テクスチャAMB解放（実際には参照カウントデクリメントのみ）
			ObjDataRelease(ObjDataGet(gmEfctBossCmnGetMdlAmbtexDworkNo(i)));
			
			// モデル登録カウント
			model_reg_cnt++;
		}
	}
	
	
	// テクスチャVRAM解放開始
	gm_efct_boss_cmn_tex_reg_id	=
		ObjAction3dESTextureReleaseDwork(ObjDataGet(GMD_DWORK_NO_EFFECT_BOSS_CMN_TEXLIST));
	
	// 共通エフェクトテクスチャAMB解放（実際には参照カウントデクリメントのみ）
	ambtex_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_BOSS_CMN_AMBTEX);
	ObjDataRelease(ambtex_dwork);
	
	// 共通エフェクトアーカイブ解放（実際には参照カウントデクリメントのみ）
	arc_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC);
	ObjDataRelease(arc_dwork);
}

// =======================================================================
// GmEfctBossCmnFlushDataLoop
/*!
  ボス共通エフェクトデータ後片付け ループ
  
  @retval TRUE	後片付け完了
  @retval FALSE	後片付け中
 */
// =======================================================================
BOOL GmEfctBossCmnFlushDataLoop(void)
{
	BOOL	result	= TRUE;
	Sint32	model_reg_cnt	= 0;
	
	if (gm_efct_boss_cmn_model_reg_num != 0) {	// 無駄にループしないようにする
		
		// モデル解放チェック
		for (Sint32 i = 0; i < GME_EFCT_BOSS_CMN_IDX_MAX; ++i) {
			const GMS_EFCT_BOSS_CMN_CREATE_PARAM	*bcr_param	= &gm_efct_boss_cmn_create_param_tbl[i];
			
			if (bcr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
				
				if (gm_efct_boss_cmn_model_reg_id_list[model_reg_cnt] != -1) {
					
					Sint32	dwork_no	= gmEfctBossCmnGetObjectDworkNo(i);
					MTM_ASSERT(dwork_no < GMD_DWORK_NO_EFFECT_BOSS_CMN_OBJECT_END);
					
					if (ObjAction3dESModelReleaseDworkCheck(ObjDataGet(dwork_no),
															gm_efct_boss_cmn_model_reg_id_list[model_reg_cnt])) {
						gm_efct_boss_cmn_model_reg_id_list[model_reg_cnt]	= -1;
					}
					else {
						result	= FALSE;
					}
				}
				
				// モデル用テクスチャ
				if (gm_efct_boss_cmn_mdl_tex_reg_id_list[model_reg_cnt] != -1) {
					if (ObjAction3dESTextureReleaseDworkCheck(ObjDataGet(gmEfctBossCmnGetMdlTexlistDworkNo(i)),
															  gm_efct_boss_cmn_mdl_tex_reg_id_list[model_reg_cnt])) {
						gm_efct_boss_cmn_mdl_tex_reg_id_list[model_reg_cnt]	= -1;
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
		MTM_ASSERT(NULL == gm_efct_boss_cmn_mdl_tex_reg_id_list);
		MTM_ASSERT(NULL == gm_efct_boss_cmn_model_reg_id_list);
	}
	
	
	// テクスチャ解放チェック
	if (gm_efct_boss_cmn_tex_reg_id != -1) {
		if (ObjAction3dESTextureReleaseDworkCheck(ObjDataGet(GMD_DWORK_NO_EFFECT_BOSS_CMN_TEXLIST),
												  gm_efct_boss_cmn_tex_reg_id)) {
			gm_efct_boss_cmn_tex_reg_id	= -1;
		}
		else {
			result	= FALSE;
		}
	}
	
	if (result) {
		
		if (gm_efct_boss_cmn_mdl_tex_reg_id_list) {
			amMemFree(gm_efct_boss_cmn_mdl_tex_reg_id_list);
			gm_efct_boss_cmn_mdl_tex_reg_id_list	= NULL;
			// reg_num は model_reg_id_list と共用
		}
		
		if (gm_efct_boss_cmn_model_reg_id_list) {
			amMemFree(gm_efct_boss_cmn_model_reg_id_list);
			gm_efct_boss_cmn_model_reg_id_list	= NULL;
			gm_efct_boss_cmn_model_reg_num	= 0;
		}
	}
	
	return result;
}


// =======================================================================
// GmEfctBossBuildSingleDataInit
/*!
  ボスエフェクトデータ単体構築 初期化
 */
// =======================================================================
void GmEfctBossBuildSingleDataInit(void)
{
	gm_efct_boss_single_reg_num	= 0;
}

// =======================================================================
// GmEfctBossBuildSingleDataReg
/*!
  ボスエフェクトデータ単体構築 開始登録
  
  @param tex_index		[in]	テクスチャAMBのAMBインデックス
  @param ambtex_dwork	[io]	テクスチャAMB格納先データワーク
  @param texlist_dwork	[io]	テクスチャリスト格納先データワーク
  @param model_index	[in]	モデルデータのAMBインデックス
  @param model_dwork	[io]	モデルデータ格納先データワーク(NULL可)
  @param object_dwork	[io]	NNオブジェクト格納先データワーク(NULL可)
  @param arc			[io]	テクスチャAMB,モデルデータが格納されているAMB
 */
// =======================================================================
void GmEfctBossBuildSingleDataReg(Sint32 tex_index,
								  OBS_DATA_WORK *ambtex_dwork,
								  OBS_DATA_WORK *texlist_dwork,
								  Sint32 model_index,
								  OBS_DATA_WORK *model_dwork,
								  OBS_DATA_WORK *object_dwork,
								  void *arc)
{
	void			*texlistbuf;
	
	GMS_EFCT_BOSS_SINGLE_BUILD_WORK	*build_work;
	
	MTM_ASSERT(gm_efct_boss_single_reg_num < GMD_EFCT_BOSS_SINGLE_BUILD_NUM_MAX);
	
	// ビルドワーク取得
	build_work	= &gm_efct_boss_single_build_list[gm_efct_boss_single_reg_num];
	gm_efct_boss_single_reg_num++;
	
	// テクスチャAMB取得
	ObjDataLoadAmbIndex(ambtex_dwork,
						tex_index,
						arc);
						
	build_work->tex_reg_id	=
		ObjAction3dESTextureLoadToDwork(texlist_dwork,
										ambtex_dwork->pData,
										&texlistbuf);
	// データワークを参照しておく
	build_work->ambtex_dwork	= ambtex_dwork;
	build_work->texlist_dwork	= texlist_dwork;
	
	if (model_dwork) {
		
		// モデルデータ取得
		ObjDataLoadAmbIndex(model_dwork,
							model_index,
							arc);
		
		// モデルVRAMロード開始
		build_work->model_reg_id	=
			ObjAction3dESModelLoadToDwork(object_dwork,
										  model_dwork->pData,
										  0);
		// データワークを参照しておく
		build_work->model_dwork	= model_dwork;
		build_work->object_dwork	= object_dwork;
	}
	else {
		build_work->model_reg_id	= -1;
		build_work->model_dwork	= NULL;
		build_work->object_dwork	= NULL;
	}
}


// =======================================================================
// GmEfctBossBuildSingleDataReg
/*!
  ボスエフェクトデータ単体構築 ループ
  
  @retval TRUE	構築完了
  @retval FALSE	構築中
 */
// =======================================================================
BOOL GmEfctBossBuildSingleDataLoop(void)
{
	BOOL	result	= TRUE;
	
	for (Sint32 i = 0; i < gm_efct_boss_single_reg_num; ++i) {
		GMS_EFCT_BOSS_SINGLE_BUILD_WORK	*build_work	= &gm_efct_boss_single_build_list[i];
		
		// テクスチャロード完了チェック
		if (build_work->tex_reg_id != -1) {
			if (amDrawIsRegistComplete(build_work->tex_reg_id)) {
				build_work->tex_reg_id	= -1;
			}
			else {
				result	= FALSE;
			}
		}
		
		// モデルロード完了チェック
		if (build_work->model_reg_id != -1) {
			if (amDrawIsRegistComplete(build_work->model_reg_id)) {
				build_work->model_reg_id	= -1;
			}
			else {
				result	= FALSE;
			}
		}
	}
	
	return result;
}

// =======================================================================
// GmEfctBossFlushSingleDataInit
/*!
  ボスエフェクト単体構築データ 解放初期化・開始
 */
// =======================================================================
void GmEfctBossFlushSingleDataInit(void)
{
	GMS_EFCT_BOSS_SINGLE_BUILD_WORK	*build_work;
	
	MTM_ASSERT(gm_efct_boss_single_reg_num <= GMD_EFCT_BOSS_SINGLE_BUILD_NUM_MAX);
	
	// ビルドワーク取得
	for (Sint32 i = 0; i < gm_efct_boss_single_reg_num; ++i) {
		build_work	= &gm_efct_boss_single_build_list[i];
		
		if (build_work->object_dwork != NULL) {
			// モデルVRAM解放開始
			build_work->model_reg_id	=
				ObjAction3dESModelReleaseDwork(build_work->object_dwork);
			
			// モデルデータ解放（実際には参照カウントデクリメントのみ）
			MTM_ASSERT(build_work->model_dwork);
			ObjDataRelease(build_work->model_dwork);
			build_work->model_dwork	= NULL;
		}
		
		
		// テクスチャVRAM解放開始
		build_work->tex_reg_id	=
			ObjAction3dESTextureReleaseDwork(build_work->texlist_dwork);
		
		// テクスチャAMB解放（実際には参照カウントデクリメントのみ）
		MTM_ASSERT(build_work->ambtex_dwork);
		ObjDataRelease(build_work->ambtex_dwork);
		build_work->ambtex_dwork	= NULL;
	}
}

// =======================================================================
// GmEfctBossFlushSingleDataLoop
/*!
  ボスエフェクト単体構築データ 解放 ループ
  
  @retval TRUE	解放済み
  @retval FALSE	解放中
 */
// =======================================================================
BOOL GmEfctBossFlushSingleDataLoop(void)
{
	GMS_EFCT_BOSS_SINGLE_BUILD_WORK	*build_work;
	BOOL	result	= TRUE;
	
	for (Sint32 i = 0; i < gm_efct_boss_single_reg_num; ++i) {
		build_work	= &gm_efct_boss_single_build_list[i];
		
		// モデル解放チェック
		if (build_work->model_reg_id != -1) {
			if (ObjAction3dESModelReleaseDworkCheck(build_work->object_dwork,
													build_work->model_reg_id)) {
				build_work->model_reg_id	= -1;
				build_work->object_dwork	= NULL;
			}
			else {
				result	= FALSE;
			}
		}
		
		// テクスチャ解放チェック
		if (build_work->tex_reg_id != -1) {
			if (ObjAction3dESTextureReleaseDworkCheck(build_work->texlist_dwork,
													  build_work->tex_reg_id)) {
				build_work->tex_reg_id	= -1;
				build_work->texlist_dwork	= NULL;
			}
			else {
				result	= FALSE;
			}
		}
	}
	
	if (result) {
		gm_efct_boss_single_reg_num	= 0;
	}
	
	return result;
}



// =======================================================================
// GmEfctBossCmnEsCreate
/*!
  ボス共通エフェクト（ESタイプ）生成
  
  @param parent_obj		[in]	親オブジェクト（NULL可）
  @param efct_bscmn_idx	[in]	ボス共通エフェクトインデックス
  
  @return エフェクト3DESワーク
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* GmEfctBossCmnEsCreate(OBS_OBJECT_WORK *parent_obj, GME_EFCT_BOSS_CMN_IDX efct_bscmn_idx)
{
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	GMS_EFFECT_3DES_WORK	*eff_3des;
	const GMS_EFCT_BOSS_CMN_CREATE_PARAM	*bcr_param;
	OBS_DATA_WORK	*model_dwork;
	OBS_DATA_WORK	*object_dwork;
	OBS_DATA_WORK	*ambtex_data_work;
	OBS_DATA_WORK	*texlist_data_work;
	
	// 指定インデックスの生成情報取得
	bcr_param	= &gm_efct_boss_cmn_create_param_tbl[efct_bscmn_idx];
	
	if (bcr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
		Sint32	ame_index	= bcr_param->create_param.ame_idx;
		
		// モデルデータを格納するデータワーク取得
		model_dwork	= ObjDataGet(gmEfctBossCmnGetModelDworkNo(ame_index));
		
		// オブジェクト（VRAM関連付け）を格納するデータワークを取得
		// アーカイブAMB内で、テクスチャAMBの次からオブジェクトが順に格納されている前提
		object_dwork	= ObjDataGet(gmEfctBossCmnGetObjectDworkNo(ame_index));
		
		// テクスチャAMBを格納するデータワーク取得
		ambtex_data_work	= ObjDataGet(gmEfctBossCmnGetMdlAmbtexDworkNo(ame_index));
		
		// テクスチャリストを格納するデータワーク取得
		texlist_data_work	= ObjDataGet(gmEfctBossCmnGetMdlTexlistDworkNo(ame_index));
	}
	else {
		model_dwork	= NULL;
		object_dwork	= NULL;
		ambtex_data_work	= ObjDataGet(GMD_DWORK_NO_EFFECT_BOSS_CMN_AMBTEX);
		texlist_data_work	= ObjDataGet(GMD_DWORK_NO_EFFECT_BOSS_CMN_TEXLIST);
	}
	
	eff_3des	= GmEffect3dESCreateByParam(&bcr_param->create_param,
											parent_obj,
											ObjDataGet(GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC)->pData,
											ObjDataGet(gmEfctBossCmnGetAmeDworkNo(bcr_param->create_param.ame_idx)),
											ambtex_data_work,
											texlist_data_work,
											model_dwork,
											object_dwork);
	
	return eff_3des;
#else
	return GmEffect3dESCreateDummy(parent_obj);
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
}


/*------ Static Functions ----------------------------------------------*/

// =======================================================================
// gmEfctBossCmnGet***DworkNo
/*!
  データワーク番号取得
 
  @param ame_idx	[in]	AMEのアーカイブAMB内のインデックス
  
  @return データワーク番号
 */
// =======================================================================
// AMEデータワークインデックス取得
inline Sint32 gmEfctBossCmnGetAmeDworkNo(Sint32 ame_idx)
{
	return GMD_DWORK_NO_EFFECT_BOSS_CMN_AME_START + ame_idx;
}
// モデルデータワークインデックス取得
inline Sint32 gmEfctBossCmnGetModelDworkNo(Sint32 ame_idx)
{
	return GMD_DWORK_NO_EFFECT_BOSS_CMN_MODELDAT_START + ame_idx;
}
// オブジェクトデータワークインデックス取得
inline Sint32 gmEfctBossCmnGetObjectDworkNo(Sint32 ame_idx)
{
	return GMD_DWORK_NO_EFFECT_BOSS_CMN_OBJECT_START + ame_idx;
}
// モデル用テクスチャAMBデータワークインデックス取得
inline Sint32 gmEfctBossCmnGetMdlAmbtexDworkNo(Sint32 ame_idx)
{
	return GMD_DWORK_NO_EFFECT_BOSS_CMN_MDL_AMBTEX_START + ame_idx;
}
// モデル用テクスチャリストデータワークインデックス取得
inline Sint32 gmEfctBossCmnGetMdlTexlistDworkNo(Sint32 ame_idx)
{
	return GMD_DWORK_NO_EFFECT_BOSS_CMN_MDL_TEXLIST_START + ame_idx;
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
