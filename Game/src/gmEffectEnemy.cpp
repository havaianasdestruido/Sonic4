// =======================================================================
/*!
  @file	gmEffectEnemy.cpp
  @brief エネミー専用エフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmEffectEnemy.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmGameDat.h"
#include "gmEffect.h"

#include "gmEffectEnemy.h"

// データヘッダ
#include "../file/common/arc/EFF_E002.HMB"
#include "../file/common/arc/EFF_E004.HMB"
#include "../file/common/arc/EFF_E005.HMB"
#include "../file/common/arc/EFF_E006.HMB"
#include "../file/common/arc/EFF_E007.HMB"
#include "../file/common/arc/EFF_E010.HMB"
#include "../file/common/arc/EFF_E013.HMB"
#include "../file/common/arc/EFF_E014.HMB"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/
#define GMM_EFCT_ENE_STAGE_FLAG(stage_no)	(((Uint32)1 << stage_no))	// ステージ1 → stage_no=0

//! アーカイブのデータワーク番号から各データワーク番号取得
//  ARC→AMBTEX→TEXLIST および、
//  AME→MODEL→OBJECT の順でデータ番号が連番になっていること！
#define GMM_EFCT_ENE_ARC_DW_NO(p_cr_param)		((Sint32)(p_cr_param->arc_dwork_no + 0))
#define GMM_EFCT_ENE_AMBTEX_DW_NO(p_cr_param)	((Sint32)(p_cr_param->ambtex_dwork_no + 0))
#define GMM_EFCT_ENE_TEXLIST_DW_NO(p_cr_param)	((Sint32)(p_cr_param->ambtex_dwork_no + 1))
#define GMM_EFCT_ENE_AME_DW_NO(p_cr_param)		((Sint32)(p_cr_param->ame_dwork_no + 0))
#define GMM_EFCT_ENE_MODEL_DW_NO(p_cr_param)	((Sint32)(p_cr_param->ame_dwork_no + 1))
#define GMM_EFCT_ENE_OBJECT_DW_NO(p_cr_param)	((Sint32)(p_cr_param->ame_dwork_no + 2))
#define GMM_EFCT_ENE_MDL_AMBTEX_DW_NO(p_cr_param)	((Sint32)(p_cr_param->ame_dwork_no + 3))
#define GMM_EFCT_ENE_MDL_TEXLIST_DW_NO(p_cr_param)	((Sint32)(p_cr_param->ame_dwork_no + 4))


/*------ Definitions ---------------------------------------------------*/
//! エネミー専用エフェクト生成パラメータ構造体
typedef struct tag_GMS_EFCT_ENE_CREATE_PARAM
{
	GMS_EFFECT_CREATE_PARAM	create_param;	//!< 生成パラメータ
	Sint32	arc_dwork_no;		//!< アーカイブ データワーク番号
	
	// 共有するものはAMBTEXからの連番として処理
	Sint32	ambtex_dwork_no;	//!< テクスチャAMB データワーク番号
	//Sint32	texlist_dwork_no;	//!< テクスチャリスト データワーク番号
	
	// 共有しないものはAMEからの連番として処理
	Sint32	ame_dwork_no;		//!< AMEデータ データワーク番号
	//Sint32	model_dwork_no;		//!< モデルデータ データワーク番号
	//Sint32	object_dwork_no;	//!< NNオブジェクト データワーク番号
	Sint32	ambtex_idx;			//!< テクスチャAMBのアーカイブAMB内のインデックス（モデル用のAMBTEXが別に用意されている場合も個々の変更で対応可能）
	Uint32	stage_flag;			//!< 出現ステージフラグ
} GMS_EFCT_ENE_CREATE_PARAM;


/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
static Sint32 gm_efct_ene_tex_reg_id_list[GME_EFCT_ENE_IDX_MAX]	= {-1};	//!< テクスチャ登録コマンドID
static Sint32 gm_efct_ene_model_reg_id_list[GME_EFCT_ENE_IDX_MAX]	= {-1};	//!< モデル登録コマンドIDリスト
static GSE_MAIN_ZONE_TYPE gm_efct_ene_target_zone_no	= GSD_MAIN_ZONE_TYPE_NONE;	//!< ビルド・フラッシュするゾーンの番号

#include "gmEffectEnemyTbl.inc"

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmEfctEneBuildDataInit
/*!
  エネミー専用エフェクトデータ構築 開始
 */
// =======================================================================
void GmEfctEneBuildDataInit(GSE_MAIN_ZONE_TYPE zone_no)
{
	OBS_DATA_WORK	*arc_dwork;
	void			*eff_ene_arc;
	OBS_DATA_WORK	*ambtex_dwork;
	OBS_DATA_WORK	*texlist_dwork;
	void		*texlistbuf;
	
	MTM_ASSERT(gm_efct_ene_target_zone_no == GSD_MAIN_ZONE_TYPE_NONE);
	
	MTM_ASSERT(gm_efct_ene_tex_reg_id_list[0] == -1);
	MTM_ASSERT(gm_efct_ene_model_reg_id_list[0] == -1);
	
	// 初回は先頭しか-1が入ってないので全体を-1でクリア
	memset(gm_efct_ene_tex_reg_id_list, -1, sizeof(Sint32) * GME_EFCT_ENE_IDX_MAX);
	memset(gm_efct_ene_model_reg_id_list, -1, sizeof(Sint32) * GME_EFCT_ENE_IDX_MAX);
	
	// ゾーン番号格納
	gm_efct_ene_target_zone_no	= zone_no;
	
	// 全エネミー専用エフェクトの数分ループして必要なもののみビルド
	for (Sint32 i = 0; i < GME_EFCT_ENE_IDX_MAX; ++i) {
		const GMS_EFCT_ENE_CREATE_PARAM *ene_cr_param	= &gm_efct_ene_create_param_tbl[i];
		Sint32	arc_dw_no	= ene_cr_param->arc_dwork_no;
		
		MTM_ASSERT(gm_efct_ene_tex_reg_id_list[i] == -1);
		MTM_ASSERT(gm_efct_ene_model_reg_id_list[i] == -1);
		
		// 当該ステージに出現しないエネミーのエフェクトはビルドしない
		if (!(ene_cr_param->stage_flag & GMM_EFCT_ENE_STAGE_FLAG(zone_no))) {
			continue;
		}
		
		
		// エネミー専用エフェクトアーカイブ取得
		arc_dwork	= ObjDataGet(arc_dw_no);
		MTM_ASSERT(arc_dwork->num > 0);
		eff_ene_arc	= ObjDataGetInc(arc_dwork);
		
		
		
		// モデルVRAMロード
		{
			Sint32 model_amb_index	= ene_cr_param->create_param.model_idx;
			Sint32 model_dw_no	= GMM_EFCT_ENE_MODEL_DW_NO(ene_cr_param);
			Sint32 object_dw_no	= GMM_EFCT_ENE_OBJECT_DW_NO(ene_cr_param);
			
			if (model_amb_index != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
				MTM_ASSERT(model_dw_no < GMD_DWORK_NO_EFFECT_ENE_END);
				MTM_ASSERT(object_dw_no < GMD_DWORK_NO_EFFECT_ENE_END);
				
				// エネミー専用エフェクトモデルデータ取得
				ObjDataLoadAmbIndex(ObjDataGet(model_dw_no),
									model_amb_index,
									eff_ene_arc);
				
				// モデルVRAMロード開始
				gm_efct_ene_model_reg_id_list[i]	=
					ObjAction3dESModelLoadToDwork(ObjDataGet(object_dw_no),
												  ObjDataGet(model_dw_no)->pData,
												  0);
				
				ambtex_dwork	= ObjDataGet(GMM_EFCT_ENE_MDL_AMBTEX_DW_NO(ene_cr_param));
				texlist_dwork	= ObjDataGet(GMM_EFCT_ENE_MDL_TEXLIST_DW_NO(ene_cr_param));
			}
			else {
				
				ambtex_dwork	= ObjDataGet(GMM_EFCT_ENE_AMBTEX_DW_NO(ene_cr_param));
				texlist_dwork	= ObjDataGet(GMM_EFCT_ENE_TEXLIST_DW_NO(ene_cr_param));
			}
		}
		
		// REMINDER : AMBTEXのインデックスは生成パラメータの ambtex_idx を常に使用するが、
		//            テクスチャで使用するデータワークはオブジェクト使用の有無で異なるので注意
		
		// テクスチャAMB取得
		ObjDataLoadAmbIndex(ambtex_dwork,
							ene_cr_param->ambtex_idx,
							eff_ene_arc);
		
		// テクスチャVRAMロード開始
		gm_efct_ene_tex_reg_id_list[i]	=
			ObjAction3dESTextureLoadToDwork(texlist_dwork,
											ambtex_dwork->pData,
											&texlistbuf);
	}
}

// =======================================================================
// GmEfctEneBuildDataLoop
/*!
  エネミー専用エフェクトデータ構築 ループ
  
  @retval TRUE	構築完了
  @retval FALSE	構築中
 */
// =======================================================================
BOOL GmEfctEneBuildDataLoop(void)
{
	BOOL	result	= TRUE;
	
	if (gm_efct_ene_target_zone_no == GSD_MAIN_ZONE_TYPE_NONE) {
		return result;
	}
	
	for (Sint32 i = 0; i < GME_EFCT_ENE_IDX_MAX; ++i) {
		const GMS_EFCT_ENE_CREATE_PARAM *ene_cr_param	= &gm_efct_ene_create_param_tbl[i];
		
		// 当該ステージに出現しないエネミーのエフェクトはスキップ
		if (!(ene_cr_param->stage_flag & GMM_EFCT_ENE_STAGE_FLAG(gm_efct_ene_target_zone_no))) {
			continue;
		}
		
		// テクスチャロード完了チェック
		if (gm_efct_ene_tex_reg_id_list[i] != -1) {
			if (amDrawIsRegistComplete(gm_efct_ene_tex_reg_id_list[i])) {
				gm_efct_ene_tex_reg_id_list[i]	= -1;
			}
			else {
				result	= FALSE;
			}
		}
		
		// モデルロード完了チェック
		if (gm_efct_ene_model_reg_id_list[i] != -1) {
			
			if (amDrawIsRegistComplete(gm_efct_ene_model_reg_id_list[i])) {
				gm_efct_ene_model_reg_id_list[i]	= -1;
			}
			else {
				result	= FALSE;
			}
		}
	}
	
	if (result) {
		// 対象ゾーン番号クリア
		gm_efct_ene_target_zone_no	= GSD_MAIN_ZONE_TYPE_NONE;
	}
	else {
		MTM_ASSERT(gm_efct_ene_target_zone_no != GSD_MAIN_ZONE_TYPE_NONE);
	}
	
	return result;
}

// =======================================================================
// GmEfctEneFlushDataInit
/*!
  エネミー専用エフェクトデータ後片付け 開始
 */
// =======================================================================
void GmEfctEneFlushDataInit(GSE_MAIN_ZONE_TYPE zone_no)
{
	OBS_DATA_WORK	*ambtex_dwork;
	OBS_DATA_WORK	*texlist_dwork;
	
	MTM_ASSERT(gm_efct_ene_target_zone_no == GSD_MAIN_ZONE_TYPE_NONE);
	
	// ゾーン番号格納
	gm_efct_ene_target_zone_no	= zone_no;
	
	
	// 全エネミー専用エフェクトの数分ループして解放
	for (Sint32 i = 0; i < GME_EFCT_ENE_IDX_MAX; ++i) {
		const GMS_EFCT_ENE_CREATE_PARAM *ene_cr_param	= &gm_efct_ene_create_param_tbl[i];
		
		// 当該ステージに出現しないエネミーのエフェクトはスキップ
		if (!(ene_cr_param->stage_flag & GMM_EFCT_ENE_STAGE_FLAG(zone_no))) {
			continue;
		}
		
		if (ene_cr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
			// モデルVRAM解放開始
			gm_efct_ene_model_reg_id_list[i]	=
				ObjAction3dESModelReleaseDwork(ObjDataGet(GMM_EFCT_ENE_OBJECT_DW_NO(ene_cr_param)));
			
			// ゾーン専用モデルデータ解放（実際には参照カウントデクリメントのみ）
			ObjDataRelease(ObjDataGet(GMM_EFCT_ENE_MODEL_DW_NO(ene_cr_param)));
			
			ambtex_dwork	= ObjDataGet(GMM_EFCT_ENE_MDL_AMBTEX_DW_NO(ene_cr_param));
			texlist_dwork	= ObjDataGet(GMM_EFCT_ENE_MDL_TEXLIST_DW_NO(ene_cr_param));
		}
		else {
			
			ambtex_dwork	= ObjDataGet(GMM_EFCT_ENE_AMBTEX_DW_NO(ene_cr_param));
			texlist_dwork	= ObjDataGet(GMM_EFCT_ENE_TEXLIST_DW_NO(ene_cr_param));
		}
		
		
		// テクスチャVRAM解放開始
		gm_efct_ene_tex_reg_id_list[i]	=
			ObjAction3dESTextureReleaseDwork(texlist_dwork);
		
		// ゾーン専用エフェクトテクスチャAMB解放（実際には参照カウントデクリメントのみ）
		ObjDataRelease(ambtex_dwork);
		
		// ゾーン専用エフェクトアーカイブ解放（実際には参照カウントデクリメントのみ）
		ObjDataRelease(ObjDataGet(GMM_EFCT_ENE_ARC_DW_NO(ene_cr_param)));
	}
}

// =======================================================================
// GmEfctEneFlushDataLoop
/*!
  エネミー専用エフェクトデータ後片付け ループ
  
  @retval TRUE	後片付け完了
  @retval FALSE	後片付け中
 */
// =======================================================================
BOOL GmEfctEneFlushDataLoop(void)
{
	BOOL	result	= TRUE;
	OBS_DATA_WORK	*texlist_dwork;
	
	if (gm_efct_ene_target_zone_no == GSD_MAIN_ZONE_TYPE_NONE) {
		return result;
	}
	
	for (Sint32 i = 0; i < GME_EFCT_ENE_IDX_MAX; ++i) {
		const GMS_EFCT_ENE_CREATE_PARAM *ene_cr_param	= &gm_efct_ene_create_param_tbl[i];
		
		
		
		// 当該ステージに出現しないエネミーのエフェクトはスキップ
		if (!(ene_cr_param->stage_flag & GMM_EFCT_ENE_STAGE_FLAG(gm_efct_ene_target_zone_no))) {
			continue;
		}
		
		// モデル使用有無に応じて適切なテクスチャリストのデータワークを取得
		if (ene_cr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
			texlist_dwork	= ObjDataGet(GMM_EFCT_ENE_MDL_TEXLIST_DW_NO(ene_cr_param));
		}
		else {
			texlist_dwork	= ObjDataGet(GMM_EFCT_ENE_TEXLIST_DW_NO(ene_cr_param));
		}
		
		// モデル解放チェック
		if (gm_efct_ene_model_reg_id_list[i] != -1) {
			
			if (ObjAction3dESModelReleaseDworkCheck(ObjDataGet(GMM_EFCT_ENE_OBJECT_DW_NO(ene_cr_param)),
													gm_efct_ene_model_reg_id_list[i])) {
				gm_efct_ene_model_reg_id_list[i]	= -1;
			}
			else {
				result	= FALSE;
			}
			
		}
		
		// テクスチャ解放チェック
		if (gm_efct_ene_tex_reg_id_list[i] != -1) {
			if (ObjAction3dESTextureReleaseDworkCheck(texlist_dwork,
													  gm_efct_ene_tex_reg_id_list[i])) {
				gm_efct_ene_tex_reg_id_list[i]	= -1;
			}
			else {
				result	= FALSE;
			}
		}
	}
	
	if (result) {
		// 対象ゾーン番号クリア
		gm_efct_ene_target_zone_no	= GSD_MAIN_ZONE_TYPE_NONE;
	}
	else {
		MTM_ASSERT(gm_efct_ene_target_zone_no != GSD_MAIN_ZONE_TYPE_NONE);
	}
	
	return result;
}



// =======================================================================
// GmEfctEneEsCreate
/*!
  エネミー専用エフェクト（ESタイプ）生成
  
  @param parent_obj		[io]	親オブジェクト（NULL可）
  @param ene_type		[in]	対象エネミーの番号
  @param efct_ene_idx	[in]	エフェクトインデックス
  
  
  @return エフェクト3DESワーク
 
  @note
  指定したエフェクトが現在のゾーンで呼び出せないものの場合はアサートします。
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* GmEfctEneEsCreate(OBS_OBJECT_WORK *parent_obj,
										Sint32 efct_ene_idx)
{
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	GMS_EFFECT_3DES_WORK	*eff_3des;
	const GMS_EFCT_ENE_CREATE_PARAM	*ecr_param;
	OBS_DATA_WORK	*model_data_work;
	OBS_DATA_WORK	*object_data_work;
	
	// 指定インデックスの生成情報取得
	ecr_param	= &gm_efct_ene_create_param_tbl[efct_ene_idx];
	
	// ステージチェック
	MTM_ASSERT(ecr_param->stage_flag & GMM_EFCT_ENE_STAGE_FLAG(g_gm_gamedat_zone_type_tbl[GsGetMainSysInfo()->stage_id]));
	
	if (ecr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
		
		// モデルデータを格納するデータワーク取得
		model_data_work	= ObjDataGet(GMM_EFCT_ENE_MODEL_DW_NO(ecr_param));
		
		// オブジェクトを格納するデータワークを取得
		object_data_work	= ObjDataGet(GMM_EFCT_ENE_OBJECT_DW_NO(ecr_param));
	}
	else {
		model_data_work	= NULL;
		object_data_work	= NULL;
	}
	
	eff_3des	= GmEffect3dESCreateByParam(&ecr_param->create_param,
											parent_obj,
											ObjDataGet(GMM_EFCT_ENE_ARC_DW_NO(ecr_param))->pData,
											ObjDataGet(GMM_EFCT_ENE_AME_DW_NO(ecr_param)),
											ObjDataGet(GMM_EFCT_ENE_AMBTEX_DW_NO(ecr_param)),
											ObjDataGet(GMM_EFCT_ENE_TEXLIST_DW_NO(ecr_param)),
											model_data_work,
											object_data_work);
	
	return eff_3des;
#else
	return GmEffect3dESCreateDummy(parent_obj);
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
}


/*------ Static Functions ----------------------------------------------*/
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
