// =======================================================================
/*!
  @file	gmEffectCmn.cpp
  @brief 共通エフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmEffectCmn.cpp 20 2011-04-22 12:46:46Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*
  << REMINDER >>
  	共通エフェクトのアーカイブは下記の並びでデータが格納されている必要があります。
  
   AMBレイアウト
  ┌──────────┬idx ┐
  │AME_00              │ 0  │
  │AME_01              │ 1  │
  │AME_02              │ 2  │
  │AME_03              │ 3  │
  │AME_04              │ 4  │
  │...                 │... │
  │AME_N-1             │n-1 │
  ├──────────┼──┤
  │テクスチャAMB ｘ１  │ n  │
  ├──────────┼──┤
  │OBJECT_00           │n+1 │ ※	（テクスチャAMBインデックス + 1） - OBJECT_00インデックス
  │OBJECT_01           │n+2 │	でOBJECT_00起点のインデックスを取得
  │OBJECT_02           │n+3 │
  │...                 │    │
  │MDL_AMBTEX_00       │    │
  │MDL_AMBTEX_01       │    │
  │...                 │ ...│
  └──────────┴──┘
 */


/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "objObject.h"
#include "gmMain.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"

/*------ Macros --------------------------------------------------------*/
#define GMD_EFCT_CMN_BUILD_REG_ALLOWANCE_NUM		(64)	//!< 登録可能コマンド数にこの数だけ空きができたらロード開始
#define GMD_EFCT_CMN_FLUSH_REG_ALLOWANCE_NUM		(16)	//!< 登録可能コマンド数にこの数だけ空きができたらフラッシュ開始

//############ 無敵エフェクト関連 #############################################
#define GMD_EFCT_CMN_INVINCIBLE_MAIN_ANGLE_RATE			(AKM_DEGtoA16(10))	//!< メインパーツの回転速度
#define GMD_EFCT_CMN_INVINCIBLE_SUB_ANGLE_RATE			(AKM_DEGtoA16(10))	//!< サブパーツの回転速度
#define GMD_EFCT_CMN_INVINCIBLE_SUB_DIST_TO_SPD_FACTOR		((fx32)(FX32_ONE * 0.05f))	//!< プレイヤーとの距離にこの係数を掛けて基本速度とする
#define GMD_EFCT_CMN_INVINCIBLE_SUB_PLYSPD_TO_SPD_FACTOR	((fx32)(FX32_ONE * 0.25f))	//!< プレイヤーの速度にこの係数を掛けて基本速度に加算する

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
typedef enum
{
	GME_EFCT_CMN_PROCESS_STATE_NOP	= 0,		//!< 処理外
	GME_EFCT_CMN_PROCESS_STATE_WAIT_START,		//!< ロード開始待ち
	GME_EFCT_CMN_PROCESS_STATE_WAIT_COMPLETE,	//!< ロード完了待ち
	
	GME_EFCT_CMN_PROCESS_STATE_MAX
} GME_EFCT_CMN_PROCESS_STATE;


typedef struct tag_GMS_EFCT_CMN_CREATE_PARAM
{
	GMS_EFFECT_CREATE_PARAM	create_param;	//!< 生成パラメータ
	Sint32	mdl_ambtex_idx;		//!< テクスチャAMBのアーカイブAMB内のインデックス（モデル用のAMBTEXが別に用意されている場合も個々の変更で対応可能）
} GMS_EFCT_CMN_CREATE_PARAM;
/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
static inline Sint32 gmEfctCmnGetAmeDworkNo(Sint32 ame_idx);
static inline Sint32 gmEfctCmnGetModelDworkNo(Sint32 ame_idx);
static inline Sint32 gmEfctCmnGetObjectDworkNo(Sint32 ame_idx);
static inline Sint32 gmEfctCmnGetMdlAmbtexDworkNo(Sint32 ame_idx);
static inline Sint32 gmEfctCmnGetMdlTexlistDworkNo(Sint32 ame_idx);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
static GME_EFCT_CMN_PROCESS_STATE gm_efct_cmn_proc_state	= GME_EFCT_CMN_PROCESS_STATE_NOP;	//!< ビルド・フラッシュ処理ステート
static Sint32 gm_efct_cmn_tex_reg_id	= -1;	//!< テクスチャ登録コマンドID
static Sint32 *gm_efct_cmn_model_reg_id_list	= NULL;	//!< モデル登録コマンドIDリスト
static Sint32 gm_efct_cmn_model_reg_num	= 0;	//!< モデル登録コマンドID数
static Sint32 *gm_efct_cmn_mdl_tex_reg_id_list	= NULL;	//!< モデル用テクスチャ登録コマンドID

#include "gmEffectCmnTbl.inc"

/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GmEfctCmnBuildDataInit
/*!
  共通エフェクトデータ構築 初期化
 */
// =======================================================================
void GmEfctCmnBuildDataInit(void)
{
	gm_efct_cmn_proc_state	= GME_EFCT_CMN_PROCESS_STATE_WAIT_START;
}

// =======================================================================
// GmEfctCmnBuildDataLoopInit
/*!
  共通エフェクトデータ構築 開始
  
  @note
  実際にロードコマンドを発行します。
 */
// =======================================================================
void GmEfctCmnBuildDataLoopInit(void)
{
	OBS_DATA_WORK	*arc_dwork;
	void			*eff_cmn_arc;
	OBS_DATA_WORK	*ambtex_dwork;
	void			*texlistbuf;
	Sint32			model_reg_cnt	= 0;
	
	MTM_ASSERT(gm_efct_cmn_tex_reg_id == -1);
	MTM_ASSERT(gm_efct_cmn_model_reg_id_list == NULL);
	MTM_ASSERT(gm_efct_cmn_model_reg_num == 0);
	MTM_ASSERT(gm_efct_cmn_mdl_tex_reg_id_list == NULL);
	
	MTM_ASSERT(gm_efct_cmn_proc_state == GME_EFCT_CMN_PROCESS_STATE_WAIT_START);
	
	// 共通エフェクトアーカイブ取得
	arc_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_ARC);
	MTM_ASSERT(arc_dwork->num > 0);
	eff_cmn_arc	= ObjDataGetInc(arc_dwork);
	
	// モデルの数だけを取得するのは手間なので、共通エフェクトのモデルデータワーク数を使用（＝AMEの数）
	gm_efct_cmn_model_reg_num	= GMD_DWORK_NO_EFFECT_CMN_MODELDAT_END - GMD_DWORK_NO_EFFECT_CMN_MODELDAT_START;
	
	// モデル登録コマンドIDリスト確保
	if (gm_efct_cmn_model_reg_num > 0) {
		gm_efct_cmn_model_reg_id_list	= (Sint32*)amMemAlloc(sizeof(Sint32) * gm_efct_cmn_model_reg_num);
		gm_efct_cmn_mdl_tex_reg_id_list	= (Sint32*)amMemAlloc(sizeof(Sint32) * gm_efct_cmn_model_reg_num);
		// -1 で埋める
		memset(gm_efct_cmn_model_reg_id_list, -1, sizeof(Sint32) * gm_efct_cmn_model_reg_num);
		memset(gm_efct_cmn_mdl_tex_reg_id_list, -1, sizeof(Sint32) * gm_efct_cmn_model_reg_num);
	}
	
	// 共通エフェクトテクスチャAMB取得
	ambtex_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_AMBTEX);
	ObjDataLoadAmbIndex(ambtex_dwork,
						IDB_EFF_CMN_EFF_CMN_TEX_AMB,
						eff_cmn_arc);
	
	// テクスチャVRAMロード開始
	gm_efct_cmn_tex_reg_id	=
		ObjAction3dESTextureLoadToDwork(ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_TEXLIST),
										ambtex_dwork->pData,
										&texlistbuf);
	
	// モデル毎処理
	for (Sint32 i = 0; i < GME_EFCT_CMN_IDX_MAX; ++i) {
		const GMS_EFCT_CMN_CREATE_PARAM	*ccr_param	= &gm_efct_cmn_create_param_tbl[i];
		Sint32 model_amb_index	= ccr_param->create_param.model_idx;
		Sint32 model_dwork_no	= gmEfctCmnGetModelDworkNo(i);
		Sint32 object_dwork_no	= gmEfctCmnGetObjectDworkNo(i);
		Sint32 mdl_ambtex_amb_index	= ccr_param->mdl_ambtex_idx;
		Sint32 mdl_ambtex_dwork_no	= gmEfctCmnGetMdlAmbtexDworkNo(i);
		Sint32 mdl_texlist_dwork_no	= gmEfctCmnGetMdlTexlistDworkNo(i);
		
		if (model_amb_index != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
			
			MTM_ASSERT(model_reg_cnt < gm_efct_cmn_model_reg_num);
			MTM_ASSERT(-1 != mdl_ambtex_amb_index);
			MTM_ASSERT(model_dwork_no < GMD_DWORK_NO_EFFECT_CMN_MODELDAT_END);
			MTM_ASSERT(object_dwork_no < GMD_DWORK_NO_EFFECT_CMN_OBJECT_END);
			MTM_ASSERT(mdl_ambtex_dwork_no < GMD_DWORK_NO_EFFECT_CMN_MDL_AMBTEX_END);
			MTM_ASSERT(mdl_texlist_dwork_no < GMD_DWORK_NO_EFFECT_CMN_MDL_TEXLIST_END);
			
			// 共通エフェクトモデル用テクスチャAMB取得
			ambtex_dwork	= ObjDataGet(mdl_ambtex_dwork_no);
			ObjDataLoadAmbIndex(ambtex_dwork,
								mdl_ambtex_amb_index,
								eff_cmn_arc);
			
			// テクスチャVRAMロード開始
			gm_efct_cmn_mdl_tex_reg_id_list[model_reg_cnt] =
				ObjAction3dESTextureLoadToDwork(ObjDataGet(mdl_texlist_dwork_no),
												ambtex_dwork->pData,
												&texlistbuf);
			
			// 共通エフェクトモデルデータ取得
			ObjDataLoadAmbIndex(ObjDataGet(model_dwork_no),
								model_amb_index,
								eff_cmn_arc);
			
			// モデルVRAMロード開始
			gm_efct_cmn_model_reg_id_list[model_reg_cnt]	=
				ObjAction3dESModelLoadToDwork(ObjDataGet(object_dwork_no),
											  ObjDataGet(model_dwork_no)->pData,
											  0);
			
			// モデル登録カウント
			model_reg_cnt++;
		}
	}
}

// =======================================================================
// GmEfctCmnBuildDataLoop
/*!
  共通エフェクトデータ構築 ループ
  
  @retval TRUE	構築完了
  @retval FALSE	構築中
 */
// =======================================================================
BOOL GmEfctCmnBuildDataLoop(void)
{
	BOOL	result	= TRUE;
	
	// NOPステートの場合（ビルド完了後など）は何もしない
	if (gm_efct_cmn_proc_state == GME_EFCT_CMN_PROCESS_STATE_NOP) {
		return TRUE;
	}
	
	if (gm_efct_cmn_proc_state == GME_EFCT_CMN_PROCESS_STATE_WAIT_START) {
		// 余裕が出来るまで待つ
		if (GsMainSysGetDisplayListRegistNum() < AMD_REGISTLIST_NUM - GMD_EFCT_CMN_BUILD_REG_ALLOWANCE_NUM) {
			GmEfctCmnBuildDataLoopInit();
			gm_efct_cmn_proc_state	= GME_EFCT_CMN_PROCESS_STATE_WAIT_COMPLETE;
		}
		
		return FALSE;
	}
	
	// テクスチャロード完了チェック
	if (gm_efct_cmn_tex_reg_id != -1) {
		if (amDrawIsRegistComplete(gm_efct_cmn_tex_reg_id)) {
			gm_efct_cmn_tex_reg_id	= -1;
		}
		else {
			result	= FALSE;
		}
	}
	
	// モデルロード完了チェック
	for (Sint32 i = 0; i < gm_efct_cmn_model_reg_num; ++i) {
		
		// モデル用テクスチャ
		if (gm_efct_cmn_mdl_tex_reg_id_list[i] != -1) {
			if (amDrawIsRegistComplete(gm_efct_cmn_mdl_tex_reg_id_list[i])) {
				gm_efct_cmn_mdl_tex_reg_id_list[i]	= -1;
			}
			else {
				result	= FALSE;
			}
		}
		
		// モデル自体
		if (gm_efct_cmn_model_reg_id_list[i] != -1) {
			if (amDrawIsRegistComplete(gm_efct_cmn_model_reg_id_list[i])) {
				gm_efct_cmn_model_reg_id_list[i]	= -1;
			}
			else {
				result	= FALSE;
			}
		}
	}
	
	if (result) {
		// 完了時処理
		gm_efct_cmn_proc_state	= GME_EFCT_CMN_PROCESS_STATE_NOP;
	}
	
	return result;
}

// =======================================================================
// GmEfctCmnFlushDataInit
/*!
  共通エフェクトデータ後片付け 初期化
 */
// =======================================================================
void GmEfctCmnFlushDataInit(void)
{
	gm_efct_cmn_proc_state	= GME_EFCT_CMN_PROCESS_STATE_WAIT_START;
}

// =======================================================================
// GmEfctCmnFlushDataLoopInit
/*!
  共通エフェクトデータ後片付け 開始
  
  @note
  実際に解放コマンドを発行します。
 */
// =======================================================================
void GmEfctCmnFlushDataLoopInit(void)
{
	OBS_DATA_WORK	*arc_dwork;
	OBS_DATA_WORK	*ambtex_dwork;
	OBS_DATA_WORK	*model_dwork;
	Sint32			model_reg_cnt	= 0;
	
	MTM_ASSERT(gm_efct_cmn_proc_state == GME_EFCT_CMN_PROCESS_STATE_WAIT_START);
	MTM_ASSERT(gm_efct_cmn_tex_reg_id == -1);
	
	
	// 全てのエフェクト種類数分ループして解放
	for (Sint32 i = 0; i < GME_EFCT_CMN_IDX_MAX; ++i) {
		const GMS_EFCT_CMN_CREATE_PARAM	*ccr_param	= &gm_efct_cmn_create_param_tbl[i];
		Sint32	model_dwork_no	= gmEfctCmnGetModelDworkNo(i);
		Sint32	object_dwork_no	= gmEfctCmnGetObjectDworkNo(i);
		
		MTM_ASSERT(model_dwork_no < GMD_DWORK_NO_EFFECT_CMN_MODELDAT_END);
		MTM_ASSERT(object_dwork_no < GMD_DWORK_NO_EFFECT_CMN_OBJECT_END);
		
		if (ccr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
			// モデルVRAM解放開始
			gm_efct_cmn_model_reg_id_list[model_reg_cnt]	=
				ObjAction3dESModelReleaseDwork(ObjDataGet(object_dwork_no));
			
			// 共通エフェクトモデルデータ解放（実際には参照カウントデクリメントのみ）
			model_dwork	= ObjDataGet(model_dwork_no);
			ObjDataRelease(model_dwork);
			
			// モデル用テクスチャVRAM解放開始
			gm_efct_cmn_mdl_tex_reg_id_list[model_reg_cnt]	=
				ObjAction3dESTextureReleaseDwork(ObjDataGet(gmEfctCmnGetMdlTexlistDworkNo(i)));
			
			// モデル用テクスチャAMB解放（実際には参照カウントデクリメントのみ）
			ObjDataRelease(ObjDataGet(gmEfctCmnGetMdlAmbtexDworkNo(i)));
			
			// モデル登録カウント
			model_reg_cnt++;
		}
	}
	
	
	// テクスチャVRAM解放開始
	gm_efct_cmn_tex_reg_id	=
		ObjAction3dESTextureReleaseDwork(ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_TEXLIST));
	
	// 共通エフェクトテクスチャAMB解放（実際には参照カウントデクリメントのみ）
	ambtex_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_AMBTEX);
	ObjDataRelease(ambtex_dwork);
	
	// 共通エフェクトアーカイブ解放（実際には参照カウントデクリメントのみ）
	arc_dwork	= ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_ARC);
	ObjDataRelease(arc_dwork);
}

// =======================================================================
// GmEfctCmnFlushDataLoop
/*!
  共通エフェクトデータ後片付け ループ
  
  @retval TRUE	後片付け完了
  @retval FALSE	後片付け中
 */
// =======================================================================
BOOL GmEfctCmnFlushDataLoop(void)
{
	BOOL	result	= TRUE;
	Sint32	model_reg_cnt	= 0;
	
	// NOPステートの場合（フラッシュ完了後など）は何もしない
	if (gm_efct_cmn_proc_state == GME_EFCT_CMN_PROCESS_STATE_NOP) {
		return TRUE;
	}
	
	if (gm_efct_cmn_proc_state == GME_EFCT_CMN_PROCESS_STATE_WAIT_START) {
		// 余裕が出来るまで待つ
		if (GsMainSysGetDisplayListRegistNum() < AMD_REGISTLIST_NUM - GMD_EFCT_CMN_FLUSH_REG_ALLOWANCE_NUM) {
			GmEfctCmnFlushDataLoopInit();
			gm_efct_cmn_proc_state	= GME_EFCT_CMN_PROCESS_STATE_WAIT_COMPLETE;
		}
		
		return FALSE;
	}
	
	if (gm_efct_cmn_model_reg_num != 0) {	// 無駄にループしないようにする
		// モデル解放チェック
		for (Sint32 i = 0; i < GME_EFCT_CMN_IDX_MAX; ++i) {
			const GMS_EFCT_CMN_CREATE_PARAM	*ccr_param	= &gm_efct_cmn_create_param_tbl[i];
			
			if (ccr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
				
				if (gm_efct_cmn_model_reg_id_list[model_reg_cnt] != -1) {
					
					Sint32	dwork_no	= gmEfctCmnGetObjectDworkNo(i);
					MTM_ASSERT(dwork_no < GMD_DWORK_NO_EFFECT_CMN_OBJECT_END);
					
					if (ObjAction3dESModelReleaseDworkCheck(ObjDataGet(dwork_no),
															gm_efct_cmn_model_reg_id_list[model_reg_cnt])) {
						gm_efct_cmn_model_reg_id_list[model_reg_cnt]	= -1;
					}
					else {
						result	= FALSE;
					}
				}
				
				// モデル用テクスチャ
				if (gm_efct_cmn_mdl_tex_reg_id_list[model_reg_cnt] != -1) {
					if (ObjAction3dESTextureReleaseDworkCheck(ObjDataGet(gmEfctCmnGetMdlTexlistDworkNo(i)),
															  gm_efct_cmn_mdl_tex_reg_id_list[model_reg_cnt])) {
						gm_efct_cmn_mdl_tex_reg_id_list[model_reg_cnt]	= -1;
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
		MTM_ASSERT(NULL == gm_efct_cmn_mdl_tex_reg_id_list);
		MTM_ASSERT(NULL == gm_efct_cmn_model_reg_id_list);
	}
	
	// テクスチャ解放チェック
	if (gm_efct_cmn_tex_reg_id != -1) {
		if (ObjAction3dESTextureReleaseDworkCheck(ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_TEXLIST),
												  gm_efct_cmn_tex_reg_id)) {
			gm_efct_cmn_tex_reg_id	= -1;
		}
		else {
			result	= FALSE;
		}
	}
	
	if (result) {
		
		if (gm_efct_cmn_mdl_tex_reg_id_list) {
			amMemFree(gm_efct_cmn_mdl_tex_reg_id_list);
			gm_efct_cmn_mdl_tex_reg_id_list	= NULL;
			// reg_num は model_reg_id_list と共用
		}
		
		if (gm_efct_cmn_model_reg_id_list) {
			amMemFree(gm_efct_cmn_model_reg_id_list);
			gm_efct_cmn_model_reg_id_list	= NULL;
			gm_efct_cmn_model_reg_num	= 0;
		}
		
		gm_efct_cmn_proc_state	= GME_EFCT_CMN_PROCESS_STATE_NOP;
	}
	
	return result;
}

// =======================================================================
// GmEfctCmnEsCreate
/*!
  共通エフェクト（ESタイプ）生成
  
  @param parent_obj		[in]	親オブジェクト（NULL可）
  @param efct_cmn_idx	[in]	共通エフェクトインデックス
  
  @return エフェクト3DESワーク
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* GmEfctCmnEsCreate(OBS_OBJECT_WORK *parent_obj, GME_EFCT_CMN_IDX efct_cmn_idx)
{
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	GMS_EFFECT_3DES_WORK	*eff_3des;
	const GMS_EFCT_CMN_CREATE_PARAM	*ccr_param;
	OBS_DATA_WORK	*model_dwork;
	OBS_DATA_WORK	*object_dwork;
	OBS_DATA_WORK	*ambtex_data_work;
	OBS_DATA_WORK	*texlist_data_work;
	
	// 指定インデックスの生成情報取得
	ccr_param	= &gm_efct_cmn_create_param_tbl[efct_cmn_idx];
	
	if (ccr_param->create_param.model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
		Sint32	ame_index	= ccr_param->create_param.ame_idx;
		
		// モデルデータを格納するデータワーク取得
		model_dwork	= ObjDataGet(gmEfctCmnGetModelDworkNo(ame_index));
		
		// オブジェクト（VRAM関連付け）を格納するデータワークを取得
		// アーカイブAMB内で、テクスチャAMBの次からオブジェクトが順に格納されている前提
		object_dwork	= ObjDataGet(gmEfctCmnGetObjectDworkNo(ame_index));
		
		// テクスチャAMBを格納するデータワーク取得
		ambtex_data_work	= ObjDataGet(gmEfctCmnGetMdlAmbtexDworkNo(ame_index));
		
		// テクスチャリストを格納するデータワーク取得
		texlist_data_work	= ObjDataGet(gmEfctCmnGetMdlTexlistDworkNo(ame_index));
	}
	else {
		model_dwork	= NULL;
		object_dwork	= NULL;
		ambtex_data_work	= ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_AMBTEX);
		texlist_data_work	= ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_TEXLIST);
	}
	
	eff_3des	= GmEffect3dESCreateByParam(&ccr_param->create_param,
											parent_obj,
											ObjDataGet(GMD_DWORK_NO_EFFECT_CMN_ARC)->pData,
											ObjDataGet(gmEfctCmnGetAmeDworkNo(ccr_param->create_param.ame_idx)),
											ambtex_data_work,
											texlist_data_work,
											model_dwork,
											object_dwork);
	
	return eff_3des;
#else
	return GmEffect3dESCreateDummy(parent_obj);
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
}

// =======================================================================
// GmEfctCmnUpdateInvincibleMainPart
/*!
  無敵エフェクト メインパーツ 更新
  
  @param efct_3des	[io]	3DESエフェクトワーク
  
  @note
  無敵エフェクトの、プレイヤー本体側パーツの更新処理を行います。
  終了チェック等は行いません。毎フレーム呼び出してください。
 */
// =======================================================================
void GmEfctCmnUpdateInvincibleMainPart(GMS_EFFECT_3DES_WORK *efct_3des)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)efct_3des;
	
	// 回転
	obj_work->dir.z += GMD_EFCT_CMN_INVINCIBLE_MAIN_ANGLE_RATE;
}

// =======================================================================
// GmEfctCmnUpdateInvincibleSubPart
/*!
  無敵エフェクト サブパーツ 更新
  
  @param efct_3des	[io]	3DESエフェクトワーク
  @param ply_obj	[in]	プレイヤーオブジェクト
  
  @note
  無敵エフェクトの、プレイヤーに遅れて付いてくるパーツの更新処理を行います。
  終了チェック等は行いません。毎フレーム呼び出してください。
 */
// =======================================================================
void GmEfctCmnUpdateInvincibleSubPart(GMS_EFFECT_3DES_WORK *efct_3des,
									  const OBS_OBJECT_WORK *ply_obj)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)efct_3des;
	
	// 回転
	obj_work->dir.z += GMD_EFCT_CMN_INVINCIBLE_SUB_ANGLE_RATE;
	
	// プレイヤーとの距離から基本速度を算出
	obj_work->spd.x	= FX_Mul(ply_obj->pos.x - obj_work->pos.x,
							 GMD_EFCT_CMN_INVINCIBLE_SUB_DIST_TO_SPD_FACTOR);
	obj_work->spd.y	= FX_Mul(ply_obj->pos.y - obj_work->pos.y,
							 GMD_EFCT_CMN_INVINCIBLE_SUB_DIST_TO_SPD_FACTOR);
	
	// プレイヤーとの相対速度が開いていく変化を緩やかにするため、
	// プレイヤー速度の既定割合の値を基本速度に加える
	// （基本速度とプレイヤーの速度が同じ方向の時のみ）
	if ((obj_work->spd.x > 0 && ply_obj->move.x > 0) ||
		(obj_work->spd.x < 0 && ply_obj->move.x < 0)) {
		obj_work->spd.x	+= FX_Mul(ply_obj->move.x,
								  GMD_EFCT_CMN_INVINCIBLE_SUB_PLYSPD_TO_SPD_FACTOR);
	}
	if ((obj_work->spd.y > 0 && ply_obj->move.y > 0) ||
		(obj_work->spd.y < 0 && ply_obj->move.y < 0)) {
		obj_work->spd.y	+= FX_Mul(ply_obj->move.y,
								  GMD_EFCT_CMN_INVINCIBLE_SUB_PLYSPD_TO_SPD_FACTOR);
	}
}


/*------ Static Functions ----------------------------------------------*/

// =======================================================================
// gmEfctCmnGet***DworkNo
/*!
  データワーク番号取得
 
  @param ame_idx	[in]	AMEのアーカイブAMB内のインデックス
  
  @return データワーク番号
 */
// =======================================================================
// AMEデータワークインデックス取得
inline Sint32 gmEfctCmnGetAmeDworkNo(Sint32 ame_idx)
{
	return GMD_DWORK_NO_EFFECT_CMN_AME_START + ame_idx;
}
// モデルデータワークインデックス取得
inline Sint32 gmEfctCmnGetModelDworkNo(Sint32 ame_idx)
{
	return GMD_DWORK_NO_EFFECT_CMN_MODELDAT_START + ame_idx;
}
// オブジェクトデータワークインデックス取得
inline Sint32 gmEfctCmnGetObjectDworkNo(Sint32 ame_idx)
{
	return GMD_DWORK_NO_EFFECT_CMN_OBJECT_START + ame_idx;
}
// モデル用テクスチャAMBデータワークインデックス取得
inline Sint32 gmEfctCmnGetMdlAmbtexDworkNo(Sint32 ame_idx)
{
	return GMD_DWORK_NO_EFFECT_CMN_MDL_AMBTEX_START + ame_idx;
}
// モデル用テクスチャリストデータワークインデックス取得
inline Sint32 gmEfctCmnGetMdlTexlistDworkNo(Sint32 ame_idx)
{
	return GMD_DWORK_NO_EFFECT_CMN_MDL_TEXLIST_START + ame_idx;
}

// =======================================================================
// GmEfctCmnStaticVarInit
/*!
 static変数の初期化
 */
// =======================================================================
void GmEfctCmnStaticVarInit(void)
{
	gm_efct_cmn_proc_state = GME_EFCT_CMN_PROCESS_STATE_NOP;	//!< ビルド・フラッシュ処理ステート
	gm_efct_cmn_tex_reg_id = -1;			//!< テクスチャ登録コマンドID
	gm_efct_cmn_model_reg_id_list = NULL;	//!< モデル登録コマンドIDリスト
	gm_efct_cmn_model_reg_num = 0;			//!< モデル登録コマンドID数
	gm_efct_cmn_mdl_tex_reg_id_list = NULL;	//!< モデル用テクスチャ登録コマンドID
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
