// =======================================================================
/*!
  @file	gmEffect.cpp
  @brief エフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmEffect.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "objObject.h"
#include "gmTask.h"
#include "gmObjDef.h"
#include "gmMain.h"

#include "gmEffect.h"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
static void gmEffectDefaultRecFunc(OBS_OBJECT_WORK *obj_work);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

/*------ Global Functions ----------------------------------------------*/

// ==========================================================================
// GmEffectInit
/*!
 *	エフェクト関連 初期化
 *
 *	@note
 *		エフェクト関連の初期化を一括して行います
 */
// =========================================================================
void GmEffectInit(void)
{
	;
}


// ==========================================================================
// GmEffectExit
/*!
 *	エフェクト関連 終了処理
 *
 *	@note
 *		エフェクト関連の終了処理を一括して行います
 */
// ==========================================================================
void GmEffectExit(void)
{
	;
}

// ==========================================================================
// エフェクトワーク
// ==========================================================================
// ==========================================================================
// GmEffectCreateWork
/*!
 *	エフェクトワークの作成・初期化
 *
 *	@param	work_size	[in]	取得するTCBワークサイズ
 *	@param	parent_obj	[in]	親オブジェクト(NULL可)
 *	@param	sort_prio	[in]	エフェクトソート用のプライオリティ
 *	@param	name		[in]	デバッグ用TCB名(NULL可)
 *
 *	@return	取得したOBS_OBJECT_WORKワークアドレス
 *
 *	@note
 *		指定サイズのワークを持つオブジェクト管理TCBを作成し\n
 *		OBS_OBJECT_WORKの基本設定を行います。\n
 *		work_sizeは sizeof(OBS_OBJECT_WORK) 以上の値を設定して下さい。\n
 *		親オブジェクトが存在する場合は、初期座標を親オブジェクトの座標に設定します。\n
 *		矩形登録無し クリップ無し 当たり無し で設定します
 */
// ==========================================================================
#if defined(MTD_DEBUG)
OBS_OBJECT_WORK* GmEffectCreateWork(Uint32 work_size, OBS_OBJECT_WORK *parent_obj, u16 sort_prio, const char *name)
#else
OBS_OBJECT_WORK* GmEffectCreateWork(Uint32 work_size, OBS_OBJECT_WORK *parent_obj, u16 sort_prio)
#endif /* defined(MTD_DEBUG) */
{
	OBS_OBJECT_WORK	*obj_work;
	
	if (work_size < sizeof(GMS_EFFECT_COM_WORK)) {
#if defined(MTD_DEBUG)
		amAssert(!"gmEffect::GmEffectCreateWork() Error! work_size too small\n");
#endif /* defined(MTD_DEBUG) */
		work_size	= sizeof(GMS_EFFECT_COM_WORK);
	}
	
	// オブジェクト取得
	MTM_ASSERT(GMD_TASK_PRIO_EFFECT < GMD_TASK_PRIO_EFFECT_MAX);
	MTM_ASSERT((GMD_TASK_PRIO_EFFECT + sort_prio) < GMD_TASK_PRIO_EFFECT_MAX);
#if defined(MTD_DEBUG)
	obj_work	= OBM_OBJECT_TASK_DETAIL_INIT((u16)(GMD_TASK_PRIO_EFFECT + sort_prio),
											  GMD_TASK_GROUP_EFFECT,
											  GMD_TASK_PAUSELEVEL_DEF,
											  GMD_OBJ_OBJPAUSELEVEL_DEF,
											  work_size, name);
#else
	obj_work	= OBM_OBJECT_TASK_DETAIL_INIT((u16)(GMD_TASK_PRIO_EFFECT + sort_prio),
											  GMD_TASK_GROUP_EFFECT,
											  GMD_TASK_PAUSELEVEL_DEF,
											  GMD_OBJ_OBJPAUSELEVEL_DEF,
											  work_size, NULL);
#endif /* defined(MTD_DEBUG) */
	
	if (obj_work == NULL) {
		MTM_ASSERT(!"gmEffect::GmEffectCreateWork() object create error! \n");
		return NULL;
	}
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, GmEffectDefaultExit);
	
	// オブジェクトタイプ
	obj_work->obj_type	= GMD_OBJTYPE_EFFECT;
	
	// 標準関数設定
	obj_work->ppOut		= ObjDrawActionSummary;
	obj_work->ppOutSub	= NULL;
	obj_work->ppIn		= NULL;
	obj_work->ppMove	= ObjObjectMove;
	obj_work->ppActCall	= NULL;
	obj_work->ppRec		= gmEffectDefaultRecFunc;
	obj_work->ppLast	= NULL;
	//obj_work->ppViewCheck	= NULL;	// OBM_OBJECT_TASK_DETAIL_INITで設定済み
	MTM_ASSERT(obj_work->ppViewCheck);
	
	// 落下ステータス設定
	obj_work->spd_fall		= GMD_OBJ_DEF_FALL_SPD;		// 落下加速度
	obj_work->spd_fall_max	= GMD_OBJ_DEF_FALL_SPDMA;	// 落下最大速度
	
	// 親設定
	if (parent_obj) {
		obj_work->parent_obj	= parent_obj;
		obj_work->pos.x	= parent_obj->pos.x;
		obj_work->pos.y	= parent_obj->pos.y;
		obj_work->pos.z	= parent_obj->pos.z;
	}
	
	// フラグ設定
	obj_work->disp_flag	|= OBD_DISP_NODIR;							// 回転無し
	obj_work->flag		|= OBD_OBJECT_NOHIT | OBD_OBJECT_NOCLIP;	// 矩形登録無し クリッピング無し
	obj_work->move_flag	|= OBD_MOVE_NOCOL;							// 当たり関連無し
	// Enemyに倣ってB面（問題あるなら修正）
	obj_work->flag	|= OBD_OBJECT_B;
	
	return obj_work;
}

// =======================================================================
// GmEffectDefaultExit
/*!
  エフェクト解放処理
  
  @param tcb	[in]	TCB
 */
// =======================================================================
void GmEffectDefaultExit(MTS_TASK_TCB *tcb)
{
	// オブジェクト標準解放
	ObjObjectExit(tcb);
}


// =======================================================================
// GmEffect3dESCreateByParam
/*!
  エフェクト3DES パラメータ指定生成
  
  @param create_param	[in]	生成パラメータ構造体
  @param parent_obj		[io]	親オブジェクト
  @param arc			[in]	エフェクトアーカイブ（NULL不可）
  @param ame_dwork		[in]	AMEデータDW（読み込み先）
  @param ambtex_dwork	[in]	テクスチャAMBデータDW（読み込み先）
  @param texlist_dwork	[in]	共有テクスチャリストDW（読み込み済み）
  @param model_dwork	[in]	モデルデータDW（使用しない場合はNULL指定）（読み込み先）
  @param object_dwork	[in]	共有オブジェクト（使用しない場合はNULL指定）（読み込み済み）
  
  @return エフェクト3DESワーク
 
  @note
  パラメータ情報構造体のデータに基づいてエフェクト生成を行います。
  使用するテクスチャリスト、オブジェクトが転送済みであることが前提です。
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* GmEffect3dESCreateByParam(const GMS_EFFECT_CREATE_PARAM *create_param,
												OBS_OBJECT_WORK *parent_obj,
												void *arc,
												OBS_DATA_WORK *ame_dwork,
												OBS_DATA_WORK *ambtex_dwork,
												OBS_DATA_WORK *texlist_dwork,
												OBS_DATA_WORK *model_dwork,
												OBS_DATA_WORK *object_dwork,
												Uint32 work_size/*=sizeof(GMS_EFFECT_3DES_WORK)*/)
{
	OBS_OBJECT_WORK	*obj_work;
	GMS_EFFECT_3DES_WORK	*eff_3des;
	
	MTM_ASSERT(arc);
	MTM_ASSERT(ame_dwork);
	MTM_ASSERT(ambtex_dwork);
	MTM_ASSERT(texlist_dwork);
	
	// エフェクト生成
	obj_work	= GMM_EFFECT_CREATE_WORK(work_size,
										 parent_obj,
										 0,
										 "EF_3DES_CREATE_BY_PARAM");
	
	eff_3des	= (GMS_EFFECT_3DES_WORK*)obj_work;
	
	
	// ESエフェクトデータロード
	ObjObjectAction3dESEffectLoad(obj_work,
								  &eff_3des->obj_3des,
								  ame_dwork,
								  NULL,//filename
								  create_param->ame_idx,
								  arc);
	
	// ESテクスチャロード
	ObjObjectAction3dESTextureLoad(obj_work,
								   obj_work->obj_3des,
								   ambtex_dwork,
								   NULL,//filename,
								   0,//amb_index
								   NULL,//archive
								   FALSE);	// 転送しない
	// ロード済みESテクスチャセット
	ObjObjectAction3dESTextureSetByDwork(obj_work,
										 texlist_dwork);
	
	if (NULL != model_dwork && create_param->model_idx != GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE) {
		// ESモデルロード
		ObjObjectAction3dESModelLoad(obj_work,
									 obj_work->obj_3des,
									 model_dwork,
									 NULL,//filename
									 0,//amb_index,
									 NULL,//archive
									 0,//draw_flag
									 FALSE);	// 転送しない
		
		if (NULL != object_dwork) {
			// ロード済みESモデルセット
			ObjObjectAction3dESModelSetByDwork(obj_work, object_dwork);
		}
	}
	
	
	// 基本設定
	GmEffect3DESSetupBase(eff_3des,
						  create_param->pos_type,
						  create_param->init_flag);
	
	// 表示オフセット設定
	GmEffect3DESSetDispOffset(eff_3des,
							  create_param->disp_ofst.x,
							  create_param->disp_ofst.y,
							  create_param->disp_ofst.z);
	
	// 表示回転設定
	GmEffect3DESSetDispRotation(eff_3des,
								create_param->disp_rot.x,
								create_param->disp_rot.y,
								create_param->disp_rot.z);
	
	// スケール設定
	GmEffect3DESSetScale(eff_3des, create_param->scale);
	
	// メイン処理関数セット
	obj_work->ppFunc	= create_param->main_func;
	
	return eff_3des;
}

#if defined(GMD_DEBUG_NO_CREATE_EFFECT)
// =======================================================================
// GmEffect3dESCreateDummy
/*!
  ダミーエフェクト生成
  
  @param parent_obj	[io]	親オブジェクト
  
  @return エフェクト3DESワーク
  
  @note
  最初からOBD_DISP_ENDフラグが立っている空オブジェクトを生成します。
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* GmEffect3dESCreateDummy(OBS_OBJECT_WORK *parent_obj)
{
	OBS_OBJECT_WORK	*obj_work;
	GMS_EFFECT_3DES_WORK	*efct_3des;
	
	// エフェクト生成
	obj_work	= GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DES_WORK),
										 parent_obj,
										 0,
										 "EF_3DES_DUMMY");
	obj_work->disp_flag	|= OBD_DISP_END;
	
	efct_3des	= (GMS_EFFECT_3DES_WORK*)obj_work;
	
	obj_work->obj_3des	= &efct_3des->obj_3des;
	
	obj_work->ppOut	= NULL;
	
	obj_work->ppFunc	= GmEffectDefaultMainFuncDeleteAtEnd;
	
	return efct_3des;
}
#endif /* defined(GMD_DEBUG_NO_CREATE_EFFECT) */

// =======================================================================
// GmEffectRectInit
/*!
  エフェクト矩形初期化
  
  @param	efct_com			[in]	エフェクトワーク
  @param	atk_flag_tbl		[in]	攻撃設定フラグテーブル
  @param	def_flag_tbl		[in]	防御設定フラグテーブル
  @@param	my_group			[in]	矩形グループ設定
  @param	target_group_flag	[in]	対象グループフラグ設定
  
  @note
  	atk_flag_tbl と def_flag_tbl は GME_EFFECT_RECT_NUM分繋がる配列として渡してください
 */
// =======================================================================
void GmEffectRectInit(GMS_EFFECT_COM_WORK *efct_com,
					  const Uint16 *atk_flag_tbl, const Uint16 *def_flag_tbl,
					  Uint8 my_group, Uint8 target_group_flag)
{
	OBS_OBJECT_WORK				*obj_work;
	
	obj_work = (OBS_OBJECT_WORK*)efct_com;
	
	// 矩形設定
	ObjObjectGetRectBuf(obj_work, efct_com->rect_work, GME_EFFECT_RECT_NUM);	// 自動登録バッファに設定
	for (Sint32 i = 0; i < GME_EFFECT_RECT_NUM; i++) {
		ObjRectGroupSet(&efct_com->rect_work[i], my_group, target_group_flag);
		ObjRectAtkSet(&efct_com->rect_work[i], atk_flag_tbl[i], GMD_OBJ_RECT_ATK_POWER_DEFAULT);
		ObjRectDefSet(&efct_com->rect_work[i], def_flag_tbl[i], GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		efct_com->rect_work[i].parent_obj = obj_work;
		efct_com->rect_work[i].flag &= ~OBD_RECT_ENABLE;	// はじめは無効
	}
	efct_com->rect_work[GME_EFFECT_RECT_DEF].ppDef = GmEffectDefaultDefFunc;	// 防御処理
	efct_com->rect_work[GME_EFFECT_RECT_ATK].ppHit = GmEffectDefaultAtkFunc;	// 攻撃処理
}

// =======================================================================
// GmEnemyDefaultDefFunc
/*!
  エフェクトダメージ食らい処理
  
  @param my_rect	[io]	自分矩形ワークポインタ
  @param your_rect	[io]	相手矩形ワークポインタ
  
  @note
   ppDefへ登録
 */
// =======================================================================
void GmEffectDefaultDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	UNREFERENCED_PARAMETER(my_rect);
	UNREFERENCED_PARAMETER(your_rect);
}

// =======================================================================
// GmEnemyDefaultAtkFunc
/*!
  エフェクト攻撃HIT処理
 
  @param my_rect	[io]	自分矩形ワークポインタ
  @param your_rect	[io]	相手矩形ワークポインタ
  
  @note
   ppHitへ登録
 */
// =======================================================================
void GmEffectDefaultAtkFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	UNREFERENCED_PARAMETER(my_rect);
	UNREFERENCED_PARAMETER(your_rect);
}


// ==========================================================================
// エフェクト設定
// ==========================================================================
// =======================================================================
// GmEffect3DESSetupBase
/*!
  エフェクト 3D ES 基本設定
  
  @param efct_3des	[io]	3DESエフェクトワーク
  @param pos_type	[in]	配置タイプ(GME_EFFECT_3DES_POS_TYPE_XXX)
  @param init_flag	[in]	初期化設定フラグ(GMD_EFFECT_3DES_FLAG_XXX)
  
  @note
  ESの回転・平行移動の正常な反映は、フラグ設定に強く依存しているため、
  この関数によって初期設定を行うことを推奨します。
  OBS_OBJECT_WORK::obj_3des が設定されている時のみ有効です。
  設定されていない(=NULL)場合はアサートします。
  OBS_OBJECT_WORK や OBS_ACTION3D_ES_WORK の設定が上書きされますので、
  さらにフラグ設定などを行う場合は、この関数呼び出しの後に行ってください。
  デフォルトで、OBD_DISP_ENDフラグが立ったときに自分を消去する処理関数が設定されます。
  （必要であれば自前の処理関数に上書きしても問題ありません。）
 */
// =======================================================================
void GmEffect3DESSetupBase(GMS_EFFECT_3DES_WORK *efct_3des,
						   GME_EFFECT_3DES_POS_TYPE pos_type, Uint32 init_flag)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)efct_3des;
	OBS_ACTION3D_ES_WORK	*obj_3des;
	
	MTM_ASSERT(obj_work);
	
	obj_3des	= obj_work->obj_3des;
	MTM_ASSERT(obj_3des);
	
	// 初期化時パラメータを保存
	efct_3des->saved_pos_type	= pos_type;
	efct_3des->saved_init_flag	= init_flag;
	
	/* 配置タイプを反映 */
	
	switch (pos_type) {
	case GME_EFFECT_3DES_POS_TYPE_MTX:
		obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_POSITION_EMITTER;
		obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_PARTICLE_DEPEND;
		break;
	case GME_EFFECT_3DES_POS_TYPE_EMT:
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_POSITION_EMITTER;
		obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_PARTICLE_DEPEND;
		break;
	case GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND:
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_POSITION_EMITTER;
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_PARTICLE_DEPEND;
		break;
	default:
		MTM_ASSERT(!"gmEffect.cpp::GmEffect3DESSetupBase() Error! Invalid pos_type.\n");
		break;
	}
	
	/* 初期化フラグ反映 */
	
	// フリップ有効・無効設定
	if (init_flag & GMD_EFFECT_3DES_FLAG_NOFLIP) {
		obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;
	}
	else {
		obj_work->disp_flag	&= ~OBD_DISP_NODIRFLIP;
	}
	
	// 親付随設定
	if (init_flag & GMD_EFFECT_3DES_FLAG_STICKPARENT) {
		MTM_ASSERT(obj_work->parent_obj);
		obj_work->flag	|= OBD_OBJECT_PARENT_FIX;
	}
	else {
		obj_work->flag	&= ~OBD_OBJECT_PARENT_FIX;
	}
	
	// スケール方法設定
	if (init_flag & GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX) {
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_SCALE_BY_MTX;
	}
	else {
		obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_SCALE_BY_MTX;
	}
	
	if (init_flag & GMD_EFFECT_3DES_FLAG_ENABLE_DIR) {
		obj_work->disp_flag	&= ~OBD_DISP_NODIR;
	}
	else {
		obj_work->disp_flag	|= OBD_DISP_NODIR;
	}
	
	// ESデータ側のRotation反映設定
	if (init_flag & GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT) {
		MTM_ASSERT(OBD_ACTFLAG_3D_ES_POSITION_EMITTER & obj_3des->flag);
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_EMT_USE_DATA_ROT;
	}
	else {
		obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_EMT_USE_DATA_ROT;
	}
	
	// 親のNODISP反映設定
	if (init_flag & GMD_EFFECT_3DES_FLAG_COPY_NODISP) {
		obj_work->flag	&= ~OBD_OBJECT_PARENT_FIX_NODISP;
	}
	else {
		obj_work->flag	|= OBD_OBJECT_PARENT_FIX_NODISP;
	}
	
	
	// デフォルト処理関数設定
	MTM_ASSERT(obj_work->ppFunc == NULL);
	obj_work->ppFunc	= GmEffectDefaultMainFuncDeleteAtEnd;	// ENDフラグが立ったら消去
}

// =======================================================================
// GmEffect3DESChangeBase
/*!
  エフェクト 3D ES 基本設定 変更
  
  @param efct_3des	[io]	3DESエフェクトワーク
  @param pos_type	[in]	配置タイプ(GME_EFFECT_3DES_POS_TYPE_XXX)
  @param init_flag	[in]	初期化設定フラグ(GMD_EFFECT_3DES_FLAG_XXX)
  
  @note
  処理関数を変更せずに基本設定を上書き変更します。
 */
// =======================================================================
void GmEffect3DESChangeBase(GMS_EFFECT_3DES_WORK *efct_3des,
							GME_EFFECT_3DES_POS_TYPE pos_type, Uint32 init_flag)
{
	OBS_OBJECT_WORK	*obj_work	= &efct_3des->efct_com.obj_work;
	void (*proc)(OBS_OBJECT_WORK*)	= obj_work->ppFunc;
	
	obj_work->ppFunc	= NULL;
	
	GmEffect3DESSetupBase(efct_3des, pos_type, init_flag);
	
	obj_work->ppFunc	= proc;
}


// =======================================================================
// GmEffect3DESSetDispOffset
/*!
  エフェクト 3DES 表示オフセット設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param ofst_x		[in]	表示オフセットX
  @param ofst_y		[in]	表示オフセットY
  @param ofst_z		[in]	表示オフセットZ
  
  @note
  表示座標オフセットを設定します。
  この関数で設定した値はオブジェクト自体の座標には影響しません。
  右手系（Y上）の座標系で、表示回転オフセットのみが適用された状態からのオフセットとなります。
 */
// =======================================================================
void GmEffect3DESSetDispOffset(GMS_EFFECT_3DES_WORK *efct_3des,
							   Float ofst_x, Float ofst_y, Float ofst_z)
{
	OBS_ACTION3D_ES_WORK	*obj_3des;
	
	MTM_ASSERT(efct_3des);
	
	obj_3des	= ((OBS_OBJECT_WORK*)efct_3des)->obj_3des;
	
	amVectorSet(&obj_3des->disp_ofst, ofst_x, ofst_y, ofst_z);
}

// =======================================================================
// GmEffect3DESAddDispOffset
/*!
  エフェクト 3DES 表示オフセット加算設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param ofst_add_x	[in]	表示オフセット加算値X
  @param ofst_add_y	[in]	表示オフセット加算値Y
  @param ofst_add_z	[in]	表示オフセット加算値Z
  
  @note
  既に設定されている表示座標オフセットに対して加算した値を設定します。
  この関数で設定した値はオブジェクト自体の座標には影響しません。
  右手系（Y上）の座標系で、表示回転オフセットのみが適用された状態からのオフセットとなります。
 */
// =======================================================================
void GmEffect3DESAddDispOffset(GMS_EFFECT_3DES_WORK *efct_3des,
							   Float ofst_add_x, Float ofst_add_y, Float ofst_add_z)
{
	OBS_ACTION3D_ES_WORK	*obj_3des;
	
	MTM_ASSERT(efct_3des);
	
	obj_3des	= ((OBS_OBJECT_WORK*)efct_3des)->obj_3des;
	
	amVectorAdd(&obj_3des->disp_ofst, ofst_add_x, ofst_add_y, ofst_add_z);
}

// =======================================================================
// GmEffect3DESSetDispOffsetCircleX
/*!
  X軸周りの円上に表示オフセットを設定
 
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param radius		[in]	半径
  @param angle		[in]	角度
  
  @note
  X軸周りの半径"radius"の円上（YZ平面上）に表示オフセットを設定します。
 */
// =======================================================================
void GmEffect3DESSetDispOffsetCircleX(GMS_EFFECT_3DES_WORK *efct_3des,
									  Float radius, Angle16 angle)
{
	GmEffect3DESSetDispOffset(efct_3des,
							  0,
							  radius * nnSin(angle),
							  radius * nnCos(angle));
}

// =======================================================================
// GmEffect3DESSetDispRotation
/*!
  エフェクト 3DES 表示回転設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param rot_x		[in]	表示回転X
  @param rot_y		[in]	表示回転Y
  @param rot_z		[in]	表示回転Z
  
  @note
  表示回転オフセットを設定します。
  この関数で設定した値はオブジェクト自体の座標や角度に影響しません。
  右手系（Y上）の座標系で、各種座標変換が適用されていない状態からの回転オフセットとなります。
 */
// =======================================================================
void GmEffect3DESSetDispRotation(GMS_EFFECT_3DES_WORK *efct_3des,
								 Angle16 rot_x, Angle16 rot_y, Angle16 rot_z)
{
	OBS_ACTION3D_ES_WORK	*obj_3des;
	
	MTM_ASSERT(efct_3des);
	
	obj_3des	= ((OBS_OBJECT_WORK*)efct_3des)->obj_3des;
	
	obj_3des->disp_rot.x	= (Uint16)rot_x;
	obj_3des->disp_rot.y	= (Uint16)rot_y;
	obj_3des->disp_rot.z	= (Uint16)rot_z;
}

// =======================================================================
// GmEffect3DESAddDispRotation
/*!
  エフェクト 3DES 表示回転加算設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param rot_add_x	[in]	表示回転加算値X
  @param rot_add_y	[in]	表示回転加算値Y
  @param rot_add_z	[in]	表示回転加算値Z
  
  @note
  既に設定されている表示回転オフセットに対して加算した値を設定します。
  この関数で設定した値はオブジェクト自体の座標や角度に影響しません。
  右手系（Y上）の座標系で、各種座標変換が適用されていない状態からの回転オフセットとなります。
 */
// =======================================================================
void GmEffect3DESAddDispRotation(GMS_EFFECT_3DES_WORK *efct_3des,
								 Angle16 rot_add_x, Angle16 rot_add_y, Angle16 rot_add_z)
{
	OBS_ACTION3D_ES_WORK	*obj_3des;
	
	MTM_ASSERT(efct_3des);
	
	obj_3des	= ((OBS_OBJECT_WORK*)efct_3des)->obj_3des;
	
	obj_3des->disp_rot.x	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & ((Angle32)obj_3des->disp_rot.x + (Angle32)rot_add_x));
	obj_3des->disp_rot.y	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & ((Angle32)obj_3des->disp_rot.y + (Angle32)rot_add_y));
	obj_3des->disp_rot.z	= (Uint16)(Angle16)(MTD_MATH_ANGLE_MASK & ((Angle32)obj_3des->disp_rot.z + (Angle32)rot_add_z));
}

// =======================================================================
// GmEffect3DESSetDuplicateDraw
/*!
  複製描画設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param ofst_x		[in]	通常描画位置からのオフセットX（描画座標系）
  @param ofst_y		[in]	通常描画位置からのオフセットY（描画座標系）
  @param ofst_z		[in]	通常描画位置からのオフセットZ（描画座標系）
  
  @note
  通常の描画に加えて、指定したオフセット位置にもう一度描画を行うように設定します。
  設定をクリア・解除するにはGmEffect3DESClearDuplicateDraw()を呼び出してください。
 */
// =======================================================================
void GmEffect3DESSetDuplicateDraw(GMS_EFFECT_3DES_WORK *efct_3des,
								  Float ofst_x, Float ofst_y, Float ofst_z)
{
	OBS_ACTION3D_ES_WORK	*obj_3des	= ((OBS_OBJECT_WORK*)efct_3des)->obj_3des;
	
	MTM_ASSERT(obj_3des);
	
	amVectorSet(&obj_3des->dup_draw_ofst, ofst_x, ofst_y, ofst_z);
	obj_3des->flag	|= OBD_ACTFLAG_3D_ES_DUPLICATE_DRAW;
}

// =======================================================================
// GmEffect3DESClearDuplicateDraw
/*!
  複製描画解除
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  
  @note
  複製描画を解除し、設定値をクリアします。
 */
// =======================================================================
void GmEffect3DESClearDuplicateDraw(GMS_EFFECT_3DES_WORK *efct_3des)
{
	OBS_ACTION3D_ES_WORK	*obj_3des	= ((OBS_OBJECT_WORK*)efct_3des)->obj_3des;
	
	MTM_ASSERT(obj_3des);
	
	amVectorInit(&obj_3des->dup_draw_ofst);
	obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_DUPLICATE_DRAW;
}

// =======================================================================
// GmEffectDefaultMainFuncDeleteAtEnd
/*!
  エフェクト メイン処理関数 アニメーション終了時削除
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  OBD_DISP_ENDフラグが立った時に自分をクリアします。
 */
// =======================================================================
void GmEffectDefaultMainFuncDeleteAtEnd(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
/*!
  エフェクト メイン処理関数 アニメーション終了時削除（親Z角度コピー）
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  親のdir.zをコピーします。
  OBD_DISP_ENDフラグが立った時に自分をクリアします。
 */
// =======================================================================
void GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
	
	if (obj_work->parent_obj) {
		obj_work->dir.z	= obj_work->parent_obj->dir.z;
	}
}

/*------ Static Functions ----------------------------------------------*/
// ==========================================================================
// 汎用処理
// ==========================================================================
// =======================================================================
// gmEffectDefaultRecFunc
/*!
  エフェクト矩形登録
  
  @param obj_work	[in]	オブジェクトワーク
 */
// =======================================================================
void gmEffectDefaultRecFunc(OBS_OBJECT_WORK *obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);
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
