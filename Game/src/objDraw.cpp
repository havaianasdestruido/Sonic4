// ==========================================================================
/*!
	@file objDraw.c
	@brief オブジェクト

	@author mana
	@author modifier Ishizaki
				Copyright(c) 2007 Dimps

  $Id: objDraw.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 *
 *
 */


/*
 *	Note :
 *
 *	Target PC Xbox360 PS3 Wii
 *
 *
 *
 */


//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objDraw.h"
#include "objObjectLoad.h"
#include "objObject.h"
#include "gsMainSys.h"

#include "gmMain.h"

//----- Definitions ---------------------------------------------------------
#define OBD_CAMERA_2DSPRITE_FOV			( 0x3f00 )			///< OBD_DISP_3D_SPRITEを使って表示する時のFOV値

#define OBD_DRAW_MODEL_ALL				(1 & _IPHONE)		//!< モデルを一括描画する。

#if (OBD_USE_ACTION3D_NN)

/// objDraw3DNNModel_DT(ObjDraw3DNNModel) 描画パラメーター構造体
typedef struct tag_OBS_DRAW_PARAM_3DNN_MODEL {
	AMS_PARAM_DRAW_OBJECT	param;					//!< モデル描画パラメーター
	NNS_MATRIX				mtx;					//!< 

	// 追加
	AMS_DRAWSTATE			draw_state;				//!< 描画ステータス 実体
	AMS_DRAWSTATE			*state;					//!< 描画ステータス 参照

	// その他
	void					(*user_func)(void*);	//!< ユーザー処理関数
	void					*user_param;			//!< ユーザーパラメーター NULL可

	//NNE_BOOL				(*material_cb_func)(NNS_DRAWCALLBACK_VAL*, void*);	//!< マテリアルコールバック
	OBF_MATERIAL_CB			material_cb_func;	//!< マテリアルコールバック
	void					*material_cb_param;		//!< マテリアルコールバックパラメータ

	u32						use_light_flag;			//!< 使用ライトフラグ

#if _PS3 | _XBOX | _PC
	NNS_RGB					toon_rim_param;			//!< トゥーンリムライト	
	float					toon_camouflage;		//!< トゥーン用迷彩
#endif

#if _WII
	NNS_VECTOR				toon_light;				//!< トゥーンライト
#endif

} OBS_DRAW_PARAM_3DNN_MODEL;

/// objDraw3DNNMotion_DT(ObjDraw3DNNMotion) 描画パラメーター構造体
typedef struct tag_OBS_DRAW_PARAM_3DNN_MOTION {
	AMS_PARAM_DRAW_MOTION_TRS	param;
	NNS_MATRIX					mtx;

	// 追加
	AMS_DRAWSTATE				draw_state;				//!< 描画ステータス 実体
	AMS_DRAWSTATE				*state;					//!< 描画ステータス 参照

	// その他
	void						(*user_func)(void*);	//!< ユーザー処理関数
	void						*user_param;			//!< ユーザーパラメーター NULL可
	
	OBF_DRAW_3DNN_MPLT_CB_FUNC	mplt_cb_func;			//!< マトリックスパレットCB関数
	void						*mplt_cb_param;			//!< マトリックスパレットCBパラメータ

	//NNE_BOOL					(*material_cb_func)(NNS_DRAWCALLBACK_VAL*, void*);	//!< マテリアルコールバック
	OBF_MATERIAL_CB				material_cb_func;		//!< マテリアルコールバック
	void						*material_cb_param;		//!< マテリアルコールバックパラメータ

	u32							use_light_flag;			//!< 使用ライトフラグ

#if _PS3 | _XBOX | _PC
	NNS_RGB						toon_rim_param;			//!< トゥーンリムライト	
	float						toon_camouflage;		//!< トゥーン用迷彩
#endif

#if _WII
	NNS_VECTOR					toon_light;				//!< トゥーンライト
#endif

	//	NNS_TRS					trslist 実体 末端配置
} OBS_DRAW_PARAM_3DNN_MOTION;


/// objDraw3DNNDrawMotion_DT(ObjDraw3DNNDrawMotion) 描画パラメーター構造体
typedef struct tag_OBS_DRAW_PARAM_3DNN_DRAW_MOTION {
	AMS_PARAM_DRAW_MOTION		param;
	NNS_MATRIX					mtx;

	// 追加
	AMS_DRAWSTATE				draw_state;				//!< 描画ステータス 実体
	AMS_DRAWSTATE				*state;					//!< 描画ステータス 参照

	// その他
	void						(*user_func)(void*);	//!< ユーザー処理関数
	void						*user_param;			//!< ユーザーパラメーター NULL可
	
	OBF_DRAW_3DNN_MPLT_CB_FUNC	mplt_cb_func;			//!< マトリックスパレットCB関数
	void						*mplt_cb_param;			//!< マトリックスパレットCBパラメータ

	//NNE_BOOL					(*material_cb_func)(NNS_DRAWCALLBACK_VAL*, void*);	//!< マテリアルコールバック
	OBF_MATERIAL_CB				material_cb_func;		//!< マテリアルコールバック
	void						*material_cb_param;		//!< マテリアルコールバックパラメータ

	u32							use_light_flag;			//!< 使用ライトフラグ

#if _PS3 | _XBOX | _PC
	NNS_RGB						toon_rim_param;			//!< トゥーンリムライト
	float						toon_camouflage;		//!< トゥーン用迷彩
#endif

#if _WII
	NNS_VECTOR					toon_light;				//!< トゥーンライト
#endif

	//	NNS_TRS					trslist 実体 末端配置
} OBS_DRAW_PARAM_3DNN_DRAW_MOTION;


/// objDraw3DNNSortModel_DT 描画パラメーター構造体
typedef struct tag_OBS_DRAW_PARAM_3DNN_SORT_MODEL {
	AMS_COMMAND_HEADER			cmd_header;				//!< コマンドヘッダ
	AMS_PARAM_SORT_DRAW_OBJECT	param;					//!< モデル描画パラメーター
	AMS_DRAWSTATE				state;

	// その他
	void						(*user_func)(void*);	//!< ユーザー処理関数
	void						*user_param;			//!< ユーザーパラメーター NULL可

	//NNE_BOOL					(*material_cb_func)(NNS_DRAWCALLBACK_VAL*, void*);	//!< マテリアルコールバック
	OBF_MATERIAL_CB				material_cb_func;		//!< マテリアルコールバック
	void						*material_cb_param;		//!< マテリアルコールバックパラメータ

	u32							use_light_flag;			//!< 使用ライトフラグ

#if _PS3 | _XBOX | _PC
	NNS_RGB						toon_rim_param;			//!< トゥーンリムライト	
	float						toon_camouflage;		//!< トゥーン用迷彩
#endif

#if _WII
	NNS_VECTOR					toon_light;				//!< トゥーンライト
#endif

	//	NNS_MATRIX			plt_mtx		実体 末端配置
	//	NNF_NODESTATUS		nstat		実体 末端配置
} OBS_DRAW_PARAM_3DNN_SORT_MODEL;

/// objDraw3DNNSetCamera_DT 描画パラメーター構造体
typedef struct tag_OBS_DRAW_PARAM_3DNN_SET_CAMERA {
	// 3D用
	NNE_PROJECTION_TYPE		proj_type;		//!< 射影タイプ

	NNS_MATRIX44			prj_mtx;		//!< 射影マトリクス
	NNS_MATRIX				view_mtx;		//!< ビューマトリクス
} OBS_DRAW_PARAM_3DNN_SET_CAMERA;

#if _IPHONE
/// ObjDraw3DNNDrawPrimitive_DT 描画パラメータ構造体
typedef struct tag_OBS_DRAW_PARAM_3DNN_DRAW_PRIMITIVE {
	AMS_PARAM_DRAW_PRIMITIVE	dat;	//!<	プリミティブ
	NNS_MATRIX					mtx;	//!<	変換マトリクス
	NNE_PRIM_LIGHT				light;	//!<	ライト設定
	NNE_PRIM_CULL				cull;	//!<	カリング設定
} OBS_DRAW_PARAM_3DNN_DRAW_PRIMITIVE;
#endif //_IPHONE

#endif // #if (OBD_USE_ACTION3D_NN)


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------
#if defined(_IPHONE)
#if !defined(HOG_CFR_TOUCH_SAMPLING)
static NNS_MATRIXSTACK	&_am_draw_stack = _am_default_stack;
#else //!defined(HOG_CFR_TOUCH_SAMPLING)
extern NNS_MATRIXSTACK	_am_draw_stack;
#endif //!defined(HOG_CFR_TOUCH_SAMPLING)
#else //defined(_IPHONE)
extern NNS_MATRIXSTACK	_am_game_stack;
static NNS_MATRIXSTACK	&_am_draw_stack = _am_game_stack;
#endif //defined(_IPHONE)


//----- Static Declarations -------------------------------------------------
static void objDrawStart_DT(AMS_TCB *tcb);

static void objDraw3DNNSetCamera(OBS_CAMERA *obj_camera, NNE_PROJECTION_TYPE proj_type, u32 command_state);

static void objDraw3DNNModelCommandFunc(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
static void objDraw3DNNModel_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
static void objDraw3DNNMotion_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
static void objDraw3DNNSetCamera_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
static void objDraw3DNNUserFunc_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
static void objDraw3DNNDrawMotion_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);

#if _IPHONE
static void objDraw3DNNDrawPrimitive_DT(void *param);
#endif //_IPHONE

static void objDraw3DNNModelCommandSortFunc(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);
static void objDraw3DNNSortModel_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);

static void objDraw3DNNSetMaterialCallback(OBF_MATERIAL_CB cb_func, void *cb_param);
static NNE_BOOL objDraw3DNNMaterialCallback(NNS_DRAWCALLBACK_VAL *draw_cb_val);

static void objDrawSetDrawLight(u32 use_light_flag);
static void objDrawSetDefaultLight(void);

static void objDraw3DESEffectServerMain(MTS_TASK_TCB *tcb);

static void objDraw3DESMatrixPush_UserFunc(void *param);
static void objDraw3DESMatrixPop_UserFunc(void *param);

#if (OBD_USE_ACTION2D_AMA)
static void objDraw2DAMAPre_DT(void *param);
#endif // #if (OBD_USE_ACTION2D_AMA)
//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
#if (OBD_USE_ACTION3D_NN)
/// amDraw 登録 ユーザー描画コマンド処理関数テーブル
void (*obj_draw_user_command_func_tbl[OBD_DRAW_USER_COMMAND_3DNN_MAX])(AMS_COMMAND_HEADER *, NNF_DRAWOBJ) = {
	objDraw3DNNModel_DT,		// 0 : オブジェクト描画
	objDraw3DNNModel_DT,		// 1 : オブジェクト描画 マテリアルモーションつき
	objDraw3DNNMotion_DT,		// 2 : モーション描画(TRS付き)
	objDraw3DNNMotion_DT,		// 3 : モーション描画(TRS付き) マテリアルモーションつき
	objDraw3DNNSetCamera_DT,	// 4 : カメラ設定
	objDraw3DNNUserFunc_DT,		// 5 : ユーザー処理
	objDraw3DNNDrawMotion_DT,	// 6 : モーション描画
	objDraw3DNNDrawMotion_DT,	// 6 : モーション描画 マテリアルモーションつき
};

/// amDraw 登録 ユーザー描画コマンド処理関数テーブル (ソート)
void (*obj_draw_user_command_sort_func_tbl[OBD_DRAW_USER_COMMAND_SORT_3DNN_MAX])(AMS_COMMAND_HEADER *, NNF_DRAWOBJ) = {
	objDraw3DNNSortModel_DT,	// 0 : オブジェクト描画
	objDraw3DNNSortModel_DT,	// 0 : オブジェクト描画 マテリアルモーションつき
};

//static u8 obj_draw_3dnn_setting_task_work[8] = {0};

/// NN 実行コマンドステートテーブル
u32 obj_draw_3dnn_command_state_tbl[OBD_DRAW_CMD_STATE_MAX] = {
	OBD_DRAW_CMD_STATE_3DNN,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
	OBD_DRAW_CMD_STATE_INVALID,
};

/// コマンド 実行時 amDrawEndScene(); を呼び出すかの設定
BOOL obj_draw_3dnn_command_state_exe_end_scene_tbl[OBD_DRAW_CMD_STATE_MAX] = {
	TRUE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
	FALSE,
};

/// マテリアルコールバック関数
static OBF_MATERIAL_CB	obj_draw_material_cb_func = NULL;
/// マテリアルコールバックパラメータ
static void *obj_draw_material_cb_param = NULL;
#endif

#if (OBD_USE_ACTION3D_ES)
static MTS_TASK_TCB *obj_draw_effect_server_tcb	= NULL;
#endif /* OBD_USE_ACTION3D_ES */

#if _WII
static NNS_MATRIX	obj_draw_unit_matrix = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
#if !_WII
	0.0f, 0.0f, 0.0f, 1.0f,
#endif
};
#endif
//----- Global Functions ----------------------------------------------------
// ==========================================================================
// 描画初期化
// ==========================================================================
// ==========================================================================
// ObjDrawInit
/*!
 *	描画システム初期化
 */
// ==========================================================================
void ObjDrawInit(void)
{
#if OBD_USE_ACTION3D_NN
	s32		i;

	// ユーザー描画処理関数テーブル登録
	amDrawSetDrawCommandFunc(objDraw3DNNModelCommandFunc, objDraw3DNNModelCommandSortFunc);

	// コマンドステートテーブル初期化
	ObjDrawClearNNCommandStateTbl();

	// 描画時設定ステータス 初期化用データ取得
#if 1
	{
		Sint32			slot;
		NNS_MATRIX		*mtx;

		g_obj_draw_3dnn_draw_state.drawflag			= 0;
		g_obj_draw_3dnn_draw_state.diffuse.mode		= NNE_MATCTRLMODE_MODULATE;
		g_obj_draw_3dnn_draw_state.diffuse.r		= 1.0f;
		g_obj_draw_3dnn_draw_state.diffuse.g		= 1.0f;
		g_obj_draw_3dnn_draw_state.diffuse.b		= 1.0f;
		g_obj_draw_3dnn_draw_state.ambient.mode		= NNE_MATCTRLMODE_MODULATE;
		g_obj_draw_3dnn_draw_state.ambient.r		= 1.0f;
		g_obj_draw_3dnn_draw_state.ambient.g		= 1.0f;
		g_obj_draw_3dnn_draw_state.ambient.b		= 1.0f;
		g_obj_draw_3dnn_draw_state.alpha.mode		= NNE_MATCTRLMODE_MODULATE;
		g_obj_draw_3dnn_draw_state.alpha.alpha		= 1.0f;
		g_obj_draw_3dnn_draw_state.specular.mode	= NNE_MATCTRLMODE_MODULATE;
		g_obj_draw_3dnn_draw_state.specular.r		= 1.0f;
		g_obj_draw_3dnn_draw_state.specular.g		= 1.0f;
		g_obj_draw_3dnn_draw_state.specular.b		= 1.0f;
		g_obj_draw_3dnn_draw_state.blend.mode		= NNE_MATCTRL_BLEND_ALPHA;
		g_obj_draw_3dnn_draw_state.envmap.texsrc	= NNE_MATCTRL_TEXCOORDSRC_NORMAL;
		g_obj_draw_3dnn_draw_state.zmode.compare	= NNE_TRUE;
		g_obj_draw_3dnn_draw_state.zmode.func		= AMD_ZFUNC_DEFAULT;
		g_obj_draw_3dnn_draw_state.zmode.update		= NNE_TRUE;
		mtx		= &g_obj_draw_3dnn_draw_state.envmap.texmtx;
		nnMakeUnitMatrix(mtx);
		nnTranslateMatrix(mtx, mtx, 0.5f, 0.5f, 0.0f);
		nnScaleMatrix(mtx, mtx, 0.5f, 0.5f, 0.0f);
		for (slot = 0; slot < 4; slot++) {
			g_obj_draw_3dnn_draw_state.texoffset[slot].mode	= NNE_MATCTRLMODE_ADD;
			g_obj_draw_3dnn_draw_state.texoffset[slot].u	= 0.0f;
			g_obj_draw_3dnn_draw_state.texoffset[slot].v	= 0.0f;
		}
	}
#else
	amDrawPushState();
	amDrawInitState();
	amDrawGetState(&g_obj_draw_3dnn_draw_state);
	amDrawPopState();
#endif

	// ライト初期化
	{
		OBS_LIGHT	*light;
		NNS_RGBA	light_col = {1.0f, 1.0f, 1.0f, 1.0f,};
		NNS_VECTOR	light_vec = {-1.0f, -1.0f, -1.0f};

		// 標準使用ライト
		g_obj.def_user_light_flag = OBD_LIGHT_USE_FLAG_0;

		// アンビエントカラー
		g_obj.ambient_color.r = 0.8f;
		g_obj.ambient_color.g = 0.8f;
		g_obj.ambient_color.b = 0.8f;

		// パラレルライトで初期化
		nnNormalizeVector(&light_vec, &light_vec);
		for (i = 0, light = &g_obj.light[0]; i < NNE_LIGHT_MAX; i++) {
			ObjDrawSetParallelLight((NNE_LIGHT)i, &light_col, 1.f, &light_vec);
		}

#if _WII
		// Wii用トゥーンライト
		g_obj.toon_light_vec = light_vec;
#endif
	}

#if _PS3 | _XBOX | _PC
	// リムライト設定
	g_obj.toon_rim_param.r = 1.0f;
	g_obj.toon_rim_param.g = 1.0f;
	g_obj.toon_rim_param.b = 1.0f;
	// 迷彩設定
	g_obj.toon_camouflage = 0.0f;
#endif
	
#endif // #if OBD_USE_ACTION3D_NN

#if OBD_USE_ACTION2D_AMA
	// 2D AMA のステート描画開始
	AoActSysSetDrawStateEnable(TRUE); 
	AoActSysSetDrawState(OBD_DRAW_CMD_STATE_2DAMA);
	// 2D AMA の描画タスク優先を設定
	AoActSysSetDrawTaskPrio(0x2000);

#endif
}

// ==========================================================================
// ObjDrawESEffectSystemInit
/*!
 *	ESエフェクトシステム初期化
 *	
 *	@param	pause_level	[in]	エフェクトサーバータスクのポーズレベル
 *	@param	task_prio	[in]	エフェクトサーバータスクのタスクプライオリティ
 *	@param	group		[in]	エフェクトサーバータスクのグループ
 */
// ==========================================================================
void ObjDrawESEffectSystemInit(u16 pause_level, u32 task_prio, u32 group)
{
#if OBD_USE_ACTION3D_ES
	// エフェクトシステムの初期化
	amEffectSystemInit();
	
	// エフェクトサーバー起動
	obj_draw_effect_server_tcb	=
		MTM_TASK_MAKE_TCB(objDraw3DESEffectServerMain,
						  NULL,
						  0,	// flag
						  pause_level,
						  task_prio,
						  (s32)group,
						  0,	// worksize
						  "ES_EFFECT_SERVER");
#endif // #if OBD_USE_ACTION3D_ES
}

// ==========================================================================
// ObjDrawESEffectSystemIsActive
/*!
 *	ESエフェクトシステムが動作しているかチェック
 *	
 *	@retval	TRUE	動作中
 *	@retval	FALSE	停止中（未初期化）
 */
// ==========================================================================
BOOL ObjDrawESEffectSystemIsActive(void)
{
	if (obj_draw_effect_server_tcb != NULL) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// ==========================================================================
// 描画終了
// ==========================================================================
// ==========================================================================
// ObjDrawExit
/*!
 *	描画システム終了
 */
// ==========================================================================
void ObjDrawExit(void)
{
	// マテリアルコールバック設定クリア(念の為)
	nnSetMaterialCallback(NULL);
	
#if _WII
	GXSetTevDirect(GX_TEVSTAGE0);
#endif

#if OBD_USE_ACTION2D_AMA
	// 2D AMA のステート描画を終了
	AoActSysSetDrawStateEnable(FALSE); 
#endif
}

// ==========================================================================
// ObjDrawESEffectSystemExit
/*!
 *	ESエフェクトシステム終了
 *	
 *	@note
 *		エフェクトサーバーを終了します。
 */
// ==========================================================================
void ObjDrawESEffectSystemExit(void)
{
#if OBD_USE_ACTION3D_ES
	MTM_ASSERT(obj_draw_effect_server_tcb);
	// エフェクトサーバー停止
	mtTaskClearTcb(obj_draw_effect_server_tcb);
	obj_draw_effect_server_tcb	= NULL;
#endif // #if OBD_USE_ACTION3D_ES
}

// ==========================================================================
// オブジェクト各種設定
// ==========================================================================
// ==========================================================================
// ObjDrawPrioritySet
/*!
 *	アクション設定関数
 *
 *	@param obj_work	[in]	ゲームオブジェクトワークポインタ
 *	@param prio		[in]	設定する描画優先度 0 最手前 ～ 31最奥
 */
// ==========================================================================
void ObjDrawPrioritySet(OBS_OBJECT_WORK *obj_work, u32 prio)
{
	UNREFERENCED_PARAMETER(obj_work);
	UNREFERENCED_PARAMETER(prio);

#if OBD_USE_ACTION2D
	if (obj_work->obj_2d) {
        ObjDrawActionPrioritySet(&obj_work->obj_2d->act_spr.act, prio);
	}
#endif	// #if OBD_USE_ACTION2D
#if OBD_USE_ACTION3D_SS
	if (obj_work->obj_3dss) {
        ObjDrawActionPrioritySet(&obj_work->obj_3dss->act_ss.act, prio);
	}
#endif // #if OBD_USE_ACTION3D_SS
#if OBD_USE_ACTION3D_POLY
	if (obj_work->obj_3dpoly) {
		MTM_ASSERT(prio <= 31);
		obj_work->obj_3dpoly->act_poly.prio = (u8)prio;
	}
#endif // #if OBD_USE_ACTION3D_POLY
#if OBD_USE_ACTION3D_SMA
	if (obj_work->obj_3dsma) {
        mtSmaSetPriority(obj_work->obj_3dsma->act_sma, (u8)prio);
	}
#endif // #if OBD_USE_ACTION3D_SMA
}


#if (OBD_USE_ACTION2D | OBD_USE_ACTION3D_SS | OBD_USE_ACTION3D_POLY | OBD_USE_ACTION3D_SMA)
// ================================================================
// ObjDrawActionPrioritySet
/*!
  アクション設定関数

  @param pAct   [in] アクションポインタ
  @param usPrio [in] 設定する描画優先度 0 最手前 ～ 31最奥

 */
// ================================================================
void ObjDrawActionPrioritySet( MTS_ACTION *pAct, u32 usPrio )
{
    MTM_ASSERT( usPrio <= 31 );

    pAct->sort_prio  = (u8)usPrio;
}
#endif	// #if (OBD_USE_ACTION2D | OBD_USE_ACTION3D_SS | OBD_USE_ACTION3D_POLY | OBD_USE_ACTION3D_SMA)

#if defined _DS
// ==========================================================================
// ObjDrawBgPrioritySet
/*!
 *	アクション設定関数
 *
 *	@param obj_work	[in]	ゲームオブジェクトワークポインタ
 *	@param prio		[in]	設定する描画BG優先度 0 最手前 ～ 3最奥
 */
// ==========================================================================
void ObjDrawBgPrioritySet(OBS_OBJECT_WORK *obj_work, u32 prio)
{
	if (obj_work->obj_2d) {
		ObjDrawActionBgPrioritySet(&obj_work->obj_2d->act_spr.act, prio);
	}
//#if OBD_USE_ACTION3D_SS
//	if (obj_work->obj_3dss) {
//		ObjDrawActionBgPrioritySet(&obj_work->obj_3dss->act_ss.act, prio);
//	}
//#endif // #if OBD_USE_ACTION3D_SS
}
#endif // #if defined _DS

#if (OBD_USE_ACTION2D)
// ================================================================
// ObjDrawActionBgPrioritySet
/*!
  アクション設定関数

  @param pAct   [in] アクションポインタ
  @param usPrio [in] 設定する描画BG優先度 0 最手前 ～ 3最奥

 */
// ================================================================
void ObjDrawActionBgPrioritySet( MTS_ACTION *pAct, u32 usPrio )
{
    MTM_ASSERT( usPrio <= 3 );

    pAct->obj_prio  = (u8)usPrio;
}
#endif // #if (OBD_USE_ACTION2D | OBD_USE_ACTION3D_SS | OBD_USE_ACTION3D_POLY | OBD_USE_ACTION3D_SMA)


// ================================================================
// ObjDrawObjectActionSet
/*!
  アクション設定関数

  @param obj_work   [in] オブジェクトワークポインタ
  @param id			[in] 設定するアクションID

	@note
		NNタイプ3Dオブジェクトの場合は、通常時はモーションバッファIDが0固定になります。\n
		(モーションブレンド時は0, 1交互)
		モーションバッファID指定を行う場合は ObjDrawObjectActionSet3DNN を使用して下さい

 */
// ================================================================
void ObjDrawObjectActionSet( OBS_OBJECT_WORK *obj_work, s32 id )
{
#if (OBD_USE_ACTION3D_SPR | OBD_USE_ACTION3D_SS | OBD_USE_ACTION2D)
	s32				i;
	u32				flag;
	OBS_RECT_WORK	*rect_work;
#endif	// #if (OBD_USE_ACTION3D_SPR | OBD_USE_ACTION3D_SS | OBD_USE_ACTION2D)

    obj_work->disp_flag &= ~OBD_DISP_END;
    obj_work->disp_flag &= ~OBD_DISP_REPEAT;

	// 3D
#if OBD_USE_ACTION3D_NN
	if (obj_work->obj_3d && obj_work->obj_3d->motion) {
		obj_work->obj_3d->act_id[0/*mbuf_id*/] = id;
		amMotionSet(obj_work->obj_3d->motion, 0/*mbuf_id*/, id);
		obj_work->obj_3d->frame[0] = 0.f;
	}
#endif	// #if OBD_USE_ACTION3D_NN

	// 3D
#if OBD_USE_ACTION3D_NNS
    if ( pObj->obj_3d && pObj->obj_3d->anime[MTE_ACT3D_NNS_ANIM_CA] ){
        pObj->obj_3d->act_id = id;
        ObjObjectAction3dSet(pObj, MTE_ACT3D_NNS_ANIM_CA, id);
    }
#endif	// #if OBD_USE_ACTION3D_NNS

	// 3DSPR
#if OBD_USE_ACTION3D_SPR
    if( pObj->obj_3dspr && pObj->obj_3dspr->bac ) {
		MTS_ACTION3D_SPRITE	*act_3dspr = &pObj->obj_3dspr->act_3dspr;

        mtActResetStruct( &act_3dspr->act, id );

        // アクション変更で矩形削除
		if (pObj->rect_num && pObj->rect_work) {
	        for (i = 0, rect_work = pObj->rect_work; i < pObj->rect_num; i++, rect_work++) {
				if (rect_work->flag & OBD_RECT_NOAUTO_ENABLEOFF) {
					continue;
				}
	            // 当たり削除
				rect_work->flag &= ~OBD_RECT_ENABLE;
	        }

			// 1パターン目に矩形情報があれば転送できるようにする
			flag = act_3dspr->act.flag;
			act_3dspr->act.flag |= MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_PLT;
			mtAct3dUpdateSprite(act_3dspr, (MTF_ACT_CMD_CB)pObj->ppActCall, (u32)pObj);
			act_3dspr->act.flag = flag;
			mtActResetStruct( &act_3dspr->act, id );
		}
	}
#endif	// #if OBD_USE_ACTION3D_SPR

	// 2D AMA
#if OBD_USE_ACTION2D_AMA
		// 設定なし
#endif	// #if OBD_USE_ACTION2D_AMA

	// 2D
#if OBD_USE_ACTION2D
    if( pObj->obj_2d && pObj->obj_2d->act_spr.act.bac_addr ) {
    	MTS_ACTION_DS	*act_ds = &pObj->obj_2d->act_spr;

        mtActResetStructDS( act_ds, id );

        // アクション変更で矩形削除
		if (pObj->rect_num && pObj->rect_work) {
	        for (i = 0, rect_work = pObj->rect_work; i < pObj->rect_num; i++, rect_work++) {
				if (rect_work->flag & OBD_RECT_NOAUTO_ENABLEOFF) {
					continue;
				}
	            // 当たり削除
				rect_work->flag &= ~OBD_RECT_ENABLE;
	        }

			// 1パターン目に矩形情報があれば転送できるようにする
			flag = act_ds->act.flag;
			act_ds->act.flag |= MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_PLT;
			mtActUpdateDS(act_ds, (MTF_ACT_CMD_CB)pObj->ppActCall, (u32)pObj);
			act_ds->act.flag = flag;
			mtActResetStructDS( act_ds, id );
		}
    }
#endif	// #if OBD_USE_ACTION2D

	// SS
#if OBD_USE_ACTION3D_SS
	if (pObj->obj_3dss && pObj->obj_3dss->bac) {
		MTS_ACTION_SS	*act_ss = &pObj->obj_3dss->act_ss;

		mtActResetStruct( &act_ss->act, id );	// mtActResetStructSS(act_ss, id);

        // アクション変更で矩形削除
		if (pObj->rect_num && pObj->rect_work) {
	        for (i = 0, rect_work = pObj->rect_work; i < pObj->rect_num; i++, rect_work++) {
				if (rect_work->flag & OBD_RECT_NOAUTO_ENABLEOFF) {
					continue;
				}
	            // 当たり削除
				rect_work->flag &= ~OBD_RECT_ENABLE;
	        }

			// 1パターン目に矩形情報があれば転送できるようにする
			flag = act_ss->act.flag;
			act_ss->act.flag |= MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_PLT;
			mtActUpdateSS(act_ss, (MTF_ACT_CMD_CB)pObj->ppActCall, (u32)pObj);
			act_ss->act.flag = flag;
			mtActResetStruct( &act_ss->act, id );
		}
	}
#endif // #if OBD_USE_ACTION3D_SS


#if OBD_USE_ACTION3D_POLY
	if (pObj->obj_3dpoly && pObj->obj_3dpoly->plm) {
		IZS_PLA_ACTION	*act_poly = &pObj->obj_3dpoly->act_poly;

		IzPolyActResetStruct(act_poly, id);
	}
#endif // #if OBD_USE_ACTION3D_POLY

	// SMA
#if OBD_USE_ACTION3D_SMA
	if (pObj->obj_3dsma && pObj->obj_3dsma->smm) {
        mtSmaSetMotion(pObj->obj_3dsma->act_sma, id);
		mtSmaSetMotionFrame(pObj->obj_3dsma->act_sma, 0);
	//	pObj->obj_3dsma->frame = 0;
	//	pObj->obj_3dsma->frame_max = mtSmaGetMotionFrames(pObj->obj_3dsma->act_sma, id);

        // ◆矩形情報設定も行うか?
	}
#endif // #if OBD_USE_ACTION3D_SMA
}

#if (OBD_USE_ACTION3D_NN)
// ================================================================
// ObjDrawObjectActionSet3DNN
/*!
  アクション設定関数 3D NN

	@param	obj_work	[in] オブジェクトワークポインタ
	@param	id			[in] 設定するアクションID
	@param	mbuf_id		[in] 3Dモーション バッファID
 */
// ================================================================
void ObjDrawObjectActionSet3DNN(OBS_OBJECT_WORK *obj_work, s32 id, s32 mbuf_id)
{
	//s32				i;
	//u32				flag;
	//OBS_RECT_WORK	*rect_work;

	MTM_ASSERT((u32)mbuf_id < OBD_ACTION3D_NN_MTN_BUF_NUM);

	obj_work->disp_flag &= ~OBD_DISP_END;
	obj_work->disp_flag &= ~OBD_DISP_REPEAT;

	// 3D
#if 1
	ObjDrawAction3dActionSet3DNN(obj_work->obj_3d, id, mbuf_id);
#else
	if (obj_work->obj_3d && obj_work->obj_3d->motion) {
		obj_work->obj_3d->act_id[mbuf_id] = id;
		amMotionSet(obj_work->obj_3d->motion, mbuf_id, id);
		obj_work->obj_3d->frame[mbuf_id] = 0.f;
	}
#endif

	// 矩形設定◆
}

// ================================================================
// ObjDrawAction3dActionSet3DNN
/*!
  アクション設定関数 3D NN

	@param	obj_3d		[in] 3Dオブジェクト描画ワークポインタ
	@param	id			[in] 設定するアクションID
	@param	mbuf_id		[in] 3Dモーション バッファID
 */
// ================================================================
void ObjDrawAction3dActionSet3DNN(OBS_ACTION3D_NN_WORK *obj_3d, s32 id, s32 mbuf_id)
{
	MTM_ASSERT(obj_3d);
	MTM_ASSERT(obj_3d->motion);

	if ((u32)mbuf_id >= OBD_ACTION3D_NN_MTN_BUF_NUM) {
		MTM_ASSERT((u32)mbuf_id < OBD_ACTION3D_NN_MTN_BUF_NUM);
		return;
	}

	obj_3d->act_id[mbuf_id] = id;
	amMotionSet(obj_3d->motion, mbuf_id, id);
	obj_3d->frame[mbuf_id] = 0.f;
}

// ================================================================
// ObjDrawObjectActionSet3DNNBlend
/*!
  アクション設定関数 3D NN モーションブレンドあり

	@param	obj_work	[in] オブジェクトワークポインタ
	@param	id			[in] 設定するアクションID

	@note
		モーションバッファ 0 のアクションを 1に移動し、
		新規設定のアクションをモーションバッファ 0に設定します。
		モーションブレンド速度は blend_spd に設定しておく必要があります。
 */
// ================================================================
void ObjDrawObjectActionSet3DNNBlend(OBS_OBJECT_WORK *obj_work, s32 id)
{
#if 1
	obj_work->disp_flag &= ~OBD_DISP_END;
	obj_work->disp_flag &= ~OBD_DISP_REPEAT;

	ObjDrawAction3dActionSet3DNNBlend(obj_work->obj_3d, id);
#else
	OBS_ACTION3D_NN_WORK	*obj_3d;

	obj_work->disp_flag &= ~OBD_DISP_END;
	obj_work->disp_flag &= ~OBD_DISP_REPEAT;

	if (obj_work->obj_3d && obj_work->obj_3d->motion) {
		obj_3d = obj_work->obj_3d;

		// アクション移動
		ObjDrawObjectActionSet3DNN(obj_work, obj_3d->act_id[0], 1);
		// フレーム移動
		obj_3d->frame[1] = obj_3d->frame[0];

		// 新規アクション設定
		ObjDrawObjectActionSet3DNN(obj_work, id, 0);

		// ブレンド率初期化
		obj_3d->marge = 1.0f;

		// ブレンド開始
		obj_3d->flag |= OBD_ACTFLAG_3D_NN_BLEND;
	}
#endif

	// 矩形設定◆
}

// ================================================================
// ObjDrawAction3dActionSet3DNNBlend
/*!
  アクション設定関数 3D NN モーションブレンドあり

	@param	obj_3d		[in] 3Dオブジェクト描画ワークポインタ
	@param	id			[in] 設定するアクションID

	@note
		モーションバッファ 0 のアクションを 1に移動し、
		新規設定のアクションをモーションバッファ 0に設定します。
		モーションブレンド速度は blend_spd に設定しておく必要があります。
 */
// ================================================================
void ObjDrawAction3dActionSet3DNNBlend(OBS_ACTION3D_NN_WORK *obj_3d, s32 id)
{
	MTM_ASSERT(obj_3d);
	MTM_ASSERT(obj_3d->motion);

	// アクション移動
	ObjDrawAction3dActionSet3DNN(obj_3d, obj_3d->act_id[0], 1);
	// フレーム移動
	obj_3d->frame[1] = obj_3d->frame[0];

	// 新規アクション設定
	ObjDrawAction3dActionSet3DNN(obj_3d, id, 0);

	// ブレンド率初期化
	obj_3d->marge = 1.0f;

	// ブレンド開始
	obj_3d->flag |= OBD_ACTFLAG_3D_NN_BLEND;
}

// ================================================================
// ObjDrawObjectActionSet3DNNMaterial
/*!
  アクション設定関数 3D NN マテリアルモーション

	@param	obj_work	[in] オブジェクトワークポインタ
	@param	id			[in] 設定するアクションID
 */
// ================================================================
void ObjDrawObjectActionSet3DNNMaterial(OBS_OBJECT_WORK *obj_work, s32 id)
{
	obj_work->disp_flag &= ~OBD_DISP_END;
	obj_work->disp_flag &= ~OBD_DISP_REPEAT;

	// 3D
#if 1
	ObjDrawAction3dActionSet3DNNMaterial(obj_work->obj_3d, id);
#else
	if (obj_work->obj_3d && obj_work->obj_3d->motion) {
		obj_work->obj_3d->mat_act_id = id;
		amMotionMaterialSet(obj_work->obj_3d->motion, id);
		obj_work->obj_3d->mat_frame = 0.f;
	}
#endif
}

// ================================================================
// ObjDrawAction3dActionSet3DNNMaterial
/*!
  アクション設定関数 3D NN マテリアルモーション

	@param	obj_3d		[in] 3Dオブジェクト描画ワークポインタ
	@param	id			[in] 設定するアクションID
 */
// ================================================================
void ObjDrawAction3dActionSet3DNNMaterial(OBS_ACTION3D_NN_WORK *obj_3d, s32 id)
{
	MTM_ASSERT(obj_3d);
	MTM_ASSERT(obj_3d->motion);

	obj_3d->mat_act_id = id;
	amMotionMaterialSet(obj_3d->motion, id);
	obj_3d->mat_frame = 0.f;
}

#endif // #if OBD_USE_ACTION3D_NN



// ==========================================================================
// ObjDrawActionGet
/*!
 *	アクション番号取得関数
 *
 *	@param obj_work	[in]	ゲームオブジェクトワークポインタ
 *
 *	@return  設定されているアクションID

	@note
		NNタイプ3Dオブジェクトの場合は、モーションバッファIDが0固定になります。\n
		モーションバッファID指定を行う場合は ObjDrawActionGet3DNN を使用して下さい
 */
// ==========================================================================
s32 ObjDrawActionGet(OBS_OBJECT_WORK *obj_work)
{
#if OBD_USE_ACTION3D_NN
	if (obj_work->obj_3d && obj_work->obj_3d->motion) {
		return (obj_work->obj_3d->act_id[0/*mbuf_id*/]);
	}

#if OBD_USE_ACTION2D_AMA
    if( obj_work->obj_2d )
        return (s32)obj_work->obj_2d->act_id;
#endif	// #if OBD_USE_ACTION2D_AMA
#endif

#if OBD_USE_ACTION3D_NNS
    if ( obj_work->obj_3d )
        return obj_work->obj_3d->act_id;
#endif

#if OBD_USE_ACTION3D_SPR
    if( obj_work->obj_3dspr )
        return obj_work->obj_3dspr->act_3dspr.act.act_id;
#endif

#if OBD_USE_ACTION2D
    if( obj_work->obj_2d )
        return obj_work->obj_2d->act_spr.act.act_id;
#endif

#if OBD_USE_ACTION3D_SS
    if( obj_work->obj_3dss )
        return obj_work->obj_3dss->act_ss.act.act_id;
#endif // #if OBD_USE_ACTION3D_SS

#if OBD_USE_ACTION3D_POLY
    if( obj_work->obj_3dpoly )
        return obj_work->obj_3dpoly->act_poly.act_id;
#endif // #if OBD_USE_ACTION3D_POLY

#if OBD_USE_ACTION3D_SMA
	if( obj_work->obj_3dsma )
        return obj_work->obj_3dsma->act_sma->mtn_id;
#endif // #if OBD_USE_ACTION3D_SMA

    return 0;
}

#if OBD_USE_ACTION3D_NN
// ==========================================================================
// ObjDrawActionGet3DNN
/*!
 *	アクション番号取得関数
 *
 *	@param obj_work	[in]	ゲームオブジェクトワークポインタ
 *	@param mbuf_id	[in]	3Dモーション使用モーションバッファID
 *
 *	@return  設定されているアクションID
 */
// ==========================================================================
s32 ObjDrawActionGet3DNN(OBS_OBJECT_WORK *obj_work, s32 mbuf_id)
{
	if (mbuf_id >= OBD_ACTION3D_NN_MTN_BUF_NUM) {
		MTM_ASSERT((u32)mbuf_id < OBD_ACTION3D_NN_MTN_BUF_NUM);
		return (0);
	}

	if (obj_work->obj_3d && obj_work->obj_3d->motion) {
		return (obj_work->obj_3d->act_id[mbuf_id]);
	}

	return (0);
}
#endif


// ==========================================================================
// アクション 解凍・転送分離
// ==========================================================================

// ==========================================================================
// 標準関数
// ==========================================================================
// ==========================================================================
// ObjDrawActionCallBack
/*!
 *	アクションコールバック関数
 *
 *	@param	cmd			[io]	コマンドデータ
 *	@param	act			[io]	アクションデータ
 *	@param	user_data	[in]	ユーザーデータ
 *
 *	@note
 *		ppActCallに設定する標準関数です。\n
 *		必要に応じてppActCallに登録する関数を作成して下さい。
 */
// ==========================================================================
#if defined _DS
void ObjDrawActionCallBack(const MTS_ACT_COMMAND *cmd, MTS_ACTION *act, u32 user_data)
{

#if 1
    // 矩形データ時
    switch( cmd->cmd_id ){
    case MTE_ACT_COMMAND_RECT:
		{
			OBS_RECT_WORK				*rect_work;
			OBS_OBJECT_WORK				*obj_work = (OBS_OBJECT_WORK*)user_data;
			const MTS_ACT_COMMAND_RECT	*cmd_rect = (const MTS_ACT_COMMAND_RECT *)cmd;

			if (cmd_rect->no < OBD_OBJ_RECT_MAX) {
				rect_work = obj_work->rect_work + cmd_rect->no;

				if ( (cmd_rect->rect[0] == cmd_rect->rect[2]) && (cmd_rect->rect[1] == cmd_rect->rect[3])) {
					// 空
					ObjRectWorkSet(rect_work, 0, 0, 0, 0);
					rect_work->flag &= ~OBD_RECT_ENABLE;
					break;
				}
				// 元が空矩形の時、
				if ( ! ( rect_work->flag & OBD_RECT_ENABLE ) ) {
					// HITフラグを落とす
					rect_work->flag &= ~OBD_RECT_HIT;
				}
				ObjRectWorkSet( rect_work, cmd_rect->rect[0], cmd_rect->rect[1], cmd_rect->rect[2], cmd_rect->rect[3]);
			}
		}
		break;
	}
#endif
#if 0
    // オブジェクトに設定してあるコールバックを呼び出す
    if ( pWork->ppActCall )
        pWork->ppActCall( cmd, act, user_data );
#endif
}
#endif //  #if defined _DS

// ==========================================================================
// ObjDrawActionSummary
/*!
 *	パーツ付き3d+2dのオブジェクト表示まとめ
 *
 *	@param	pWork	[in] オブジェクトワークポインタ
 *
 *	@note
 *		ppOutに設定する標準関数です。\n
 *		必要に応じてppOutに登録する関数を作成して下さい。
 */
// ==========================================================================
void ObjDrawActionSummary(OBS_OBJECT_WORK *obj_work)
{
#if OBD_USE_ACTION2D
	u32					disp_flag;
#endif

#if OBD_USE_ACTION3D_NN
	if (obj_work->obj_3d && ObjAction3dNNModelLoadCheck(obj_work->obj_3d)) {
		ObjDrawObjectAction3DNN(obj_work, obj_work->obj_3d);
	}
	
#if OBD_USE_ACTION3D_ES
	// EffectaStudioエフェクト描画
	if (obj_work->obj_3des && ObjAction3dESEffectLoadCheck(obj_work->obj_3des)) {
		ObjDrawObjectAction3DES(obj_work, obj_work->obj_3des);
	}
#endif /* OBD_USE_ACTION3D_ES */

	// 2D AMA
#if OBD_USE_ACTION2D_AMA
	if (obj_work->obj_2d && ObjAction2dAMALoadCheck(obj_work->obj_2d)) {
		ObjDrawObjectAction2DAMA(obj_work, obj_work->obj_2d);
	}
#endif	// #if OBD_USE_ACTION2D_AMA

#endif


	// 設定されている3Dタイプに合わせた描画を行う（一応平行可能）
#if OBD_USE_ACTION3D_NNS
	if (obj_work->obj_3d && obj_work->obj_3d->model) {
		ObjDrawObjectAction3DDS(obj_work, &obj_work->obj_3d->act_3d.a3d, NULL);
	}
#endif


#if OBD_USE_ACTION3D_1M1S
	if (obj_work->obj_s3d && obj_work->obj_s3d->model) {
		ObjDrawObjectAction3DDS(obj_work, &obj_work->obj_s3d->act_s3d.a3d, NULL);
	}
#endif


#if OBD_USE_ACTION3D_SPR
	if (obj_work->obj_3dspr && obj_work->obj_3dspr->bac) {
		ObjDrawObjectAction3DDS(obj_work, &obj_work->obj_3dspr->act_3dspr.a3d, obj_work->obj_3dspr->act_uncomp);
	}
#endif


#if OBD_USE_ACTION2D
	if (obj_work->obj_2d && obj_work->obj_2d->act_spr.act.bac_addr) {
		// 矩形用2Dアクションかチェック
		if (obj_work->obj_3d || obj_work->obj_s3d || obj_work->obj_3dspr) {
            disp_flag = obj_work->disp_flag;
		}

		ObjDrawObjectAction(obj_work, &obj_work->obj_2d->act_spr);

		// 表示フラグ復帰
		if (obj_work->obj_3d || obj_work->obj_s3d || obj_work->obj_3dspr) {
			obj_work->disp_flag = disp_flag;
		}
    }
#endif


#if OBD_USE_ACTION3D_SS
	if (obj_work->obj_3dss && obj_work->obj_3dss->bac) {
		OBS_ACTION_SP_SETTING_WORK	*sp_setting = NULL;

#if OBD_USE_TEX_VRAM_UNCOMP
		if (obj_work->obj_3dss->act_uncomp) {
			sp_setting = (OBS_ACTION_SP_SETTING_WORK*)obj_work->obj_3dss->act_uncomp;
		}
#endif
#if OBD_USE_TEX_VRAM_DB
		if (obj_work->obj_3dss->act_texdb) {
			sp_setting = (OBS_ACTION_SP_SETTING_WORK*)obj_work->obj_3dss->act_texdb;
		}
#endif

		ObjDrawObjectActionSS(obj_work, &obj_work->obj_3dss->act_ss, sp_setting);
	}
#endif


#if OBD_USE_ACTION3D_POLY
	if (obj_work->obj_3dpoly && obj_work->obj_3dpoly->plm) {
		ObjDrawObjectActionPoly(obj_work, &obj_work->obj_3dpoly->act_poly, NULL);
	}
#endif


#if OBD_USE_ACTION3D_SMA
	if (obj_work->obj_3dsma && obj_work->obj_3dsma->smm) {
		ObjDrawObjectAction3DSma(obj_work, obj_work->obj_3dsma->act_sma, obj_work->obj_3dsma->act_uncomp);
	}
#endif
}

// ==========================================================================
// 描画
// ==========================================================================

#if OBD_USE_ACTION2D
// ==========================================================================
// ObjDrawObjectAction
/*!
 *	オブジェクトアクション
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param act_spr		[in]	アクションポインタ
 */
// ==========================================================================
void ObjDrawObjectAction(OBS_OBJECT_WORK *obj_work, MTS_ACTION_DS *act_spr)
{
	VecFx32	pos;
	VecU16	dir;

    // セット
	pos = obj_work->pos;
	dir = obj_work->dir;

	// オフセットを足す
	pos.x += obj_work->ofst.x;
	pos.y += obj_work->ofst.y;

	// 逆重力対応
	if (obj_work->dir_fall) {
		dir.z += obj_work->dir_fall;
	}

	// 3DオブジェクトならA側の処理は不要
	if (obj_work->obj_3d || obj_work->obj_s3d || obj_work->obj_3dspr) {
		act_spr->flag |= MTD_ACT_DS_FLAG_DISABLE_GE_A;
	}

	ObjDrawAction(act_spr, &pos, &dir, &obj_work->scale, &obj_work->disp_flag, (MTF_ACT_CMD_CB)obj_work->ppActCall, (u32)obj_work);
}

// ==========================================================================
// ObjDrawAction
/*!
 *	オブジェクトアクション
 *
 *	@param pAct				[io]	アクションポインタ 
 *	@param vPos				[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir				[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale			[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag		[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pActCall			[in]	アクションコールバック （ NULL可 ）
 *	@param ulActCallAddr	[in]	アクションコールバック引数アドレス （ NULL可 ）
 */
// ==========================================================================
void ObjDrawAction( MTS_ACTION_DS * pAct, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, MTF_ACT_CMD_CB pActCall, u32 ulActCallAddr )
{
    s32 lX = 0,lY = 0;
    s32 sSpdWork;
    u32 ulDispFlag = 0;
    u8  uc3D = 0;
    fx32 fScaleX, fScaleY;
    u16 usDir = 0;
    u16 i;
    

    if ( pDispFlag ){
        ulDispFlag = *pDispFlag;
    
        // チェックフラグを落とす
        pAct->act.flag &= ~( MTD_ACT_FLAG_REPEAT | MTD_ACT_FLAG_FLIP_H | MTD_ACT_FLAG_FLIP_V);
        *pDispFlag &= ~(OBD_DISP_END);

        // フラグ設定
        if ( ulDispFlag & OBD_DISP_REPEAT )
            pAct->act.flag |= MTD_ACT_FLAG_REPEAT;
        if ( ulDispFlag & OBD_DISP_HFLIP )
            pAct->act.flag |= MTD_ACT_FLAG_FLIP_H;
        if ( ulDispFlag & OBD_DISP_VFLIP )
            pAct->act.flag |= MTD_ACT_FLAG_FLIP_V;
    }

    if ( vDir && !(ulDispFlag & OBD_DISP_NODIR)) {
        usDir = vDir->z;
	}

    for ( i = 0; i < 2; ++i ){
        // カメラ位置設定
        if ( (g_obj.flag & OBD_OBJ_CAMERA) && !( ulDispFlag & OBD_DISP_NOMAP ) ){
            lX = g_obj.camera[i][0] >> FX32_SHIFT;
            lY = g_obj.camera[i][1] >> FX32_SHIFT;
        }
        if ( !(ulDispFlag & OBD_DISP_NOPOS) ){
            // 位置設定
            if ( vPos ){
                pAct->pos[i][0] = (s16)( (vPos->x >> FX32_SHIFT) - lX );
                pAct->pos[i][1] = (s16)( (vPos->y >> FX32_SHIFT) - lY );
                if ( (g_obj.flag & OBD_OBJ_BELT) &&
                		!(ulDispFlag & OBD_DISP_NOBELT)) {
                    pAct->pos[i][1] += FX_Mul(vPos->z, g_obj.depth) >> FX32_SHIFT;
				}
            }
            if ( !( ulDispFlag & OBD_DISP_NOOFST ) ){
                pAct->pos[i][0] += g_obj.offset[MTD_X];
                pAct->pos[i][1] += g_obj.offset[MTD_Y];
            }
        }
    }
    
    
    // アニメ速度をGlobal速度にあわす
    sSpdWork = pAct->act.speed;
    pAct->act.speed = FX_Mul(pAct->act.speed, g_obj.speed);
    
    // アクションを進めない（再転送時のみ転送処理）単独使用を可能にするためMTD_ACT_FLAG_STOPは用いない
	// OBD_DISP_NOUPDATEでも1パターン目だけは転送する対応
    if ( ulDispFlag & OBD_DISP_STOP ||
			((ulDispFlag & OBD_DISP_NOUPDATE) && 0 >= pAct->act.timer)) {
        pAct->act.speed = 0;
	}
    
    // アクション更新
    if ( !( ulDispFlag & OBD_DISP_NOUPDATE ) ||
			((ulDispFlag & OBD_DISP_NOUPDATE) && 0 >= pAct->act.timer)) {
        mtActUpdateDS( pAct, pActCall, ulActCallAddr );
    }

    // 速度戻す
    pAct->act.speed = sSpdWork;

    // 拡大率設定
#if 1
	{
		fScaleX = fScaleY = FX32_ONE;

		if (vScale && !(ulDispFlag & OBD_DISP_NOSCALE)) {
			fScaleX = vScale->x;
			fScaleY = vScale->y;
		}
		if (!(ulDispFlag & OBD_DISP_NODRAWSCALE)) {
			fScaleX = FX_Mul(fScaleX, g_obj.draw_scale.x);
			fScaleY = FX_Mul(fScaleY, g_obj.draw_scale.y);
		}
		if (!(ulDispFlag & OBD_DISP_NOGLBSCALE)) {
			fScaleX = FX_Mul(fScaleX, g_obj.glb_scale.x);
			fScaleY = FX_Mul(fScaleY, g_obj.glb_scale.y);

		    // 拡大時位置計算
		    if (  g_obj.glb_scale.x != 0x1000 ){
		        pAct->pos[0][0] = (s16)((s32)g_obj.cam_scale_center[0][MTD_X] + FX_Mul((pAct->pos[0][0] - g_obj.cam_scale_center[0][MTD_X]), g_obj.glb_scale.x));
		        pAct->pos[1][0] = (s16)((s32)g_obj.cam_scale_center[1][MTD_X] + FX_Mul((pAct->pos[1][0] - g_obj.cam_scale_center[1][MTD_X]), g_obj.glb_scale.x));
		    }
		    if (  g_obj.glb_scale.y != 0x1000 ){
		        pAct->pos[0][1] = (s16)((s32)g_obj.cam_scale_center[0][MTD_Y] + FX_Mul((pAct->pos[0][1] - g_obj.cam_scale_center[0][MTD_Y]), g_obj.glb_scale.y));
		        pAct->pos[1][1] = (s16)((s32)g_obj.cam_scale_center[1][MTD_Y] + FX_Mul((pAct->pos[1][1] - g_obj.cam_scale_center[1][MTD_Y]), g_obj.glb_scale.y));
		    }
		}
	}
#else
	if (!(ulDispFlag & OBD_DISP_NOGLBSCALE)) {
	    fScaleX = g_obj.scale.x;
	    fScaleY = g_obj.scale.y;

	    if ( vScale && !(ulDispFlag & OBD_DISP_NOSCALE)){
	        fScaleX = FX_Mul( fScaleX, vScale->x );
	        fScaleY = FX_Mul( fScaleY, vScale->y );
	    }
	    // 拡大時位置計算
	    if (  g_obj.glb_scale.x != 0x1000 ){
	        pAct->pos[0][0] = (s16)((s32)g_obj.cam_scale_center[0][MTD_X] + FX_Mul((pAct->pos[0][0] - g_obj.cam_scale_center[0][MTD_X]), g_obj.glb_scale.x));
	        pAct->pos[1][0] = (s16)((s32)g_obj.cam_scale_center[1][MTD_X] + FX_Mul((pAct->pos[1][0] - g_obj.cam_scale_center[1][MTD_X]), g_obj.glb_scale.x));
	    }
	    if (  g_obj.glb_scale.y != 0x1000 ){
	        pAct->pos[0][1] = (s16)((s32)g_obj.cam_scale_center[0][MTD_Y] + FX_Mul((pAct->pos[0][1] - g_obj.cam_scale_center[0][MTD_Y]), g_obj.glb_scale.y));
	        pAct->pos[1][1] = (s16)((s32)g_obj.cam_scale_center[1][MTD_Y] + FX_Mul((pAct->pos[1][1] - g_obj.cam_scale_center[1][MTD_Y]), g_obj.glb_scale.y));
	    }
	}
	else {
	// グローバルスケールを使用しない
		fScaleX = FX32_ONE;
		fScaleY = FX32_ONE;
		if (vScale && !(ulDispFlag & OBD_DISP_NOSCALE)) {
			fScaleX = FX_Mul(fScaleX, vScale->x);
			fScaleY = FX_Mul(fScaleY, vScale->y);
		}
	}
#endif

// 表示登録
    if ( !( ulDispFlag & OBD_DISP_NODISP ) ){
        if ( usDir ||
             !(fScaleX == FX32_ONE && fScaleY == FX32_ONE)){
            mtActDrawAffineDS( pAct, fScaleX, fScaleY, usDir );
        }else{
            mtActDrawDS( pAct );
        }
    }

    if (pDispFlag && !(ulDispFlag & OBD_DISP_RECTONLY) ){
        // 終了フラグセット
        if ( pAct->act.flag & MTD_ACT_FLAG_END )
            *pDispFlag |= OBD_DISP_END;
    }
}
#endif // #if OBD_USE_ACTION2D


#if (OBD_USE_ACTION3D_NNS || OBD_USE_ACTION3D_1M1S || OBD_USE_ACTION3D_SPR)
// ==========================================================================
// ObjDrawObjectAction3DDS
/*!
 *	オブジェクトアクション 3D
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param act_3d		[in]	アクションポインタ
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
void ObjDrawObjectAction3DDS(OBS_OBJECT_WORK *obj_work, MTS_ACTION3D *act_3d, OBS_ACTION_UNCOMP_WORK *act_uncomp)
{
	VecFx32	pos;
	VecU16	dir;

	// セット
	pos = obj_work->pos;
	dir = obj_work->dir;

	// オフセットを足す
	pos.x += obj_work->ofst.x;
	pos.y += obj_work->ofst.y;
	pos.z += obj_work->ofst.z;

	// 逆重力対応
	if (obj_work->dir_fall) {
		dir.z += obj_work->dir_fall;
	}

	// 3DオブジェクトならA側の処理は不要
#if 1	// ◆ObjDrawObjectAction内で判定しているのでここの処理は不要では?
	if (obj_work->obj_3d || obj_work->obj_s3d) {
		if (obj_work->obj_2d) {
			obj_work->obj_2d->act_spr.flag |= MTD_ACT_DS_FLAG_DISABLE_GE_A;
		}
	}
#else
    if ( pWork->obj_3d || pWork->obj_s3d )
        pAct->flag |= MTD_ACT_DS_FLAG_DISABLE_GE_A;
#endif

	ObjDrawAction3DDS(act_3d, &pos, &dir, &obj_work->scale, &obj_work->disp_flag, obj_work->obj_s3d, (MTF_ACT_CMD_CB)obj_work->ppActCall, (u32)obj_work, act_uncomp);
}

// ==========================================================================
// ObjDrawAction3DDS
/*!
 *	オブジェクトアクション
 *
 *	@param pAct			[in]	アクション3Dポインタ
 *	@param vPos			[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir			[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale		[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pSimpleList	[in]	オブジェクト3D1m1s表示ワークポインタ
 *	@param pActCall		[in]	アクションコールバック （ NULL可 ）
 *	@param ulActCallAddr[in]	アクションコールバック引数アドレス （ NULL可 ）
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
void ObjDrawAction3DDS(MTS_ACTION3D * pAct, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, OBS_ACTION3D_SIMPLE_WORK *pSimpleList, MTF_ACT_CMD_CB pActCall, u32 ulActCallAddr, OBS_ACTION_UNCOMP_WORK *act_uncomp)
{
    u16 usDir = 0;
    u32 i;
    s32 sSpdWork;
    fx32 fSpdWork;
    fx16 fSpdWorkBlend;
    
    MTS_ACTION3D_NNS * pActN = NULL;
    MTS_ACTION3D_SPRITE * pActS = NULL;
    MTS_ACTION * pAct2d = NULL;
    
    MtxFx44 org_prj; // プロジェクション保持ワーク
    VecFx32 vScaleWork; // 拡大率保持ワーク

    VecFx32	pos;
	VecU16	dir;

    u32 ulDispFlag = 0;
    
    vScaleWork = pAct->scale;

	// pos取得
	if (vPos) {
		pos = *vPos;
	}
	else {
		pos.x = pos.y = pos.z = 0;
	}

	// dir取得
	if (vDir) {
		dir = *vDir;
	}
	else {
		dir.x = dir.y = dir.z = 0;
	}

    // NNS3Dの場合ポインタを設定
    if ( pAct->type == MTE_ACT3D_STRUCT_NNS ){
        pActN = (MTS_ACTION3D_NNS *)pAct;
    }

    // 3DSpriteの場合ポインタを設定
    if ( pAct->type == MTE_ACT3D_STRUCT_SPRITE ){
        pActS = (MTS_ACTION3D_SPRITE *)pAct;
        pAct2d = &pActS->act;
    }

    if ( pDispFlag )
        ulDispFlag = *pDispFlag;

    // pAct->flag &= ~( MTD_ACT3D_FLAG_MASTER_STOP );

    
    if ( pAct2d ){
        pAct2d->flag &= ~( MTD_ACT_FLAG_REPEAT | MTD_ACT_FLAG_FLIP_H | MTD_ACT_FLAG_FLIP_V);
        // スプライト用フラグ設定
        if ( ulDispFlag & OBD_DISP_REPEAT )
            pAct2d->flag |= MTD_ACT_FLAG_REPEAT;
        if ( (ulDispFlag & OBD_DISP_HFLIP ) )
            pAct2d->flag |= MTD_ACT_FLAG_FLIP_H;
        if ( ulDispFlag & OBD_DISP_VFLIP )
            pAct2d->flag |= MTD_ACT_FLAG_FLIP_V;
    }else{
        // 2D的なフリップ設定
        if ( ulDispFlag & OBD_DISP_HFLIP )
            usDir = (MTD_MATH_MAX_ANGLE>> 1) + (MTD_MATH_MAX_ANGLE >> 2);
        else
            usDir = MTD_MATH_MAX_ANGLE >> 2;
    }
    
    if ( pActN ){
        // NNSフラグ設定
        for ( i = 0; i < MTE_ACT3D_NNS_ANIM_MAX; ++i )
            pActN->aflag[i] &= ~( MTD_ACT3D_NNS_AFLAG_REPEAT | MTD_ACT3D_NNS_AFLAG_STOP );
        if ( ulDispFlag & OBD_DISP_REPEAT ){
            for ( i = 0; i < MTE_ACT3D_NNS_ANIM_MAX; ++i )
                pActN->aflag[i] |= MTD_ACT3D_NNS_AFLAG_REPEAT;
        }
    }

    // END落とし
    ulDispFlag &= ~(OBD_DISP_END);

    // ライトを設定する
    if( OBD_DISP_3D_LOCK_LIGHT & ulDispFlag )
    {
        if ( ulDispFlag & OBD_DISP_HFLIP )
        {
            // ライトをオブジェクトに向ける
            for( i = 0; i < g_obj.light_num; ++i )
                NNS_G3dGlbLightVector( (GXLightId)i, g_obj.light_dir[i].x, g_obj.light_dir[i].y, g_obj.light_dir[i].z );
        }
        else
        {
            // ライトをオブジェクトに向ける
            for( i = 0; i < g_obj.light_num; ++i )
                NNS_G3dGlbLightVector( (GXLightId)i, (fx16)-g_obj.light_dir[i].x, g_obj.light_dir[i].y, g_obj.light_dir[i].z );
        }
    }
    else
    {
        // そのままの向きに設定する
        for( i = 0; i < g_obj.light_num; ++i )
            NNS_G3dGlbLightVector( (GXLightId)i, g_obj.light_dir[i].x, g_obj.light_dir[i].y, g_obj.light_dir[i].z );
    }

    // 位置設定
    if ( !(ulDispFlag & OBD_DISP_NOPOS) ){
        s32 lX = 0,lY = 0;

        if ( g_obj.flag & OBD_OBJ_CAMERA && !( ulDispFlag & OBD_DISP_NOMAP ) ){
        	if (g_obj.pp3dCam) {
        		// カメラ取得関数実行
        		g_obj.pp3dCam(&lX, &lY);
        	}
        	else {
        		// メイン2Dカメラから取得
				lX = g_obj.camera[0][0];
				lY = g_obj.camera[0][1];
        	}
        }

        /*
        s16     lx = 0, ly = 0;
        
        if ( g_obj.flag & OBD_OBJ_CAMERA && !( ulDispFlag & OBD_DISP_NOMAP ) ){
            NL_MapCnvWorldToDispPos(
                NL_MapGetCameraMain(),
                lPosX, lPosY,
                &lx, &ly );
        }else{
            lx = (s16)(lPosX >> 8);
            ly = (s16)(lPosY >> 8);
        }
         */
        pAct->trans.x =  (pos.x - lX);
        pAct->trans.y = -(pos.y - lY);
        pAct->trans.z =  (pos.z);

        if (!( ulDispFlag & OBD_DISP_NOOFST )){
            pAct->trans.x += g_obj.offset[0] << FX32_SHIFT;
            pAct->trans.y += g_obj.offset[1] << FX32_SHIFT;
        }

        if ( (g_obj.flag & OBD_OBJ_BELT) &&
        		!(ulDispFlag & OBD_DISP_NOBELT)){
            pAct->trans.z = pos.z << 1;
            pAct->trans.y -= (fx32)FX_Mul(pos.z, g_obj.depth);
        }
        // 全体拡大時の位置調整
#if 1
        if ( g_obj.glb_scale.x != 0x1000 ){
            pAct->trans.x = ((s32)g_obj.cam_scale_center[0][MTD_X] << FX32_SHIFT) + FX_Mul((pAct->trans.x - ((s32)g_obj.cam_scale_center[0][MTD_X] << FX32_SHIFT)), g_obj.glb_scale.x);
        }
        if ( g_obj.glb_scale.y != 0x1000 ) {
            pAct->trans.y = -(((s32)g_obj.cam_scale_center[0][MTD_Y] << FX32_SHIFT) + FX_Mul(( -pAct->trans.y - ((s32)g_obj.cam_scale_center[0][MTD_Y] << FX32_SHIFT) ), g_obj.glb_scale.y));
		}
#else
        if ( g_obj.scale.x != 0x1000 ){
            pAct->trans.x = ((OBD_LCD_X / 2) << FX32_SHIFT) + FX_Mul((pAct->trans.x - ((OBD_LCD_X / 2) << FX32_SHIFT)), g_obj.scale.x);
        }
        if ( g_obj.scale.y != 0x1000 ) {
            pAct->trans.y = -(((OBD_LCD_Y / 2) << FX32_SHIFT) + FX_Mul(( -pAct->trans.y - ((OBD_LCD_Y / 2) << FX32_SHIFT) ), g_obj.scale.y));
		}
#endif        
    }

    // 向き回転行列セット
    if ( !(ulDispFlag & OBD_DISP_NODIR) && !(ulDispFlag & OBD_DISP_NOPOS) ){
        MtxFx33     mRotZ;
        
        MTX_Identity33( &pAct->rot );
        
        // Ｙ軸回転（左右フリップ）
        MTX_RotY33( &mRotZ, mtMathSin( usDir ), mtMathCos( usDir ) );
        MTX_Concat33( &pAct->rot, &mRotZ, &pAct->rot );

        // Ｘ軸回転
        MTX_RotX33( &mRotZ, mtMathSin( (u16)-(dir.x)), mtMathCos( (u16)-(dir.x) ) );
        MTX_Concat33( &pAct->rot, &mRotZ, &pAct->rot );
        
        // Ｙ軸回転
        MTX_RotY33( &mRotZ, mtMathSin( (u16)-(dir.y)), mtMathCos( (u16)-(dir.y) ) );
        MTX_Concat33(&pAct->rot, &mRotZ, &pAct->rot );
        
        // Ｚ軸回転（傾き）
        MTX_RotZ33( &mRotZ, mtMathSin( (u16)-(dir.z) ), mtMathCos( (u16)-(dir.z ) ) );
        
        MTX_Concat33( &pAct->rot, &mRotZ, &pAct->rot );

    }
    // スケール設定
    {
        fx32 fScaleX = FX32_ONE, fScaleY = FX32_ONE, fScaleZ = FX32_ONE;
        
        if ( vScale ){
            fScaleX = FX_Mul( g_obj.scale.x, vScale->x );
            fScaleY = FX_Mul( g_obj.scale.y, vScale->y );
            fScaleZ = FX_Mul( g_obj.scale.z, vScale->z );
            //fScaleZ = vScale->z;
        }
        VEC_Set( &pAct->scale,
                 FX_Mul( pAct->scale.x, fScaleX),
                 FX_Mul( pAct->scale.y, fScaleY),
                 FX_Mul( pAct->scale.z, fScaleZ) );
    }
    
    // オブジェクトがカメラの表面を向くように設定する
    if( ulDispFlag & OBD_DISP_3D_PARALLEL )
    {
        //  カメラが+Z（手前）から-Z（奥）をまっすぐに見つめて（XY平面に投影する）
        //  オブジェクトがZ=0の時に丁度画面と座標が一致するようにカメラ側で制御しており
        //  また下記図の範囲が表示されるようになっていることが前提です。
        // Y↑         256  X
        //  ├─────┬→
        //  │  ここが  │
        //  │ カメラの │
        //  │ 表示範囲 │
        //  ├─────┘
        //  │-192
        //  ここでは射影行列の『左右非対称の透視射影』を使ってオブジェクトがどこにいても
        //  オブジェクトの中心に焦点がくるように調整しています。
        const MTS_UTIL_CAMERA_PERSP     *cam_persp  = &g_obj.camera3d->persp;
        fx32    nh, nw, oh, ow, rate;

        // カメラのパース情報から近接クリッピング面の幅と高さを求める
        nh  = FX_Mul( FX_Div( mtMathSin( cam_persp->fov ), mtMathCos( cam_persp->fov ) ), cam_persp->near );
        nw  = FX_Mul( nh, cam_persp->aspect );

        // オブジェクトの移動量を近接クリッピング面に投影したときの量で求める
        // 視点とNear面間の距離：視点とオブジェクト間の距離＝Near面での移動量：オブジェクト移動量
        // ∴Near面での移動量＝視点とNear面間の距離÷視点とオブジェクト間の距離×オブジェクト移動量
        rate    = FX_Div( cam_persp->near, g_obj.camera3d->pos.z );
        oh  = FX_Mul( rate, ( ( FX32_ONE * OBD_LCD_Y ) / 2 ) - (-pAct->trans.y) );
        ow  = FX_Mul( rate, ( ( FX32_ONE * OBD_LCD_X ) / 2 ) - (+pAct->trans.x) );

        // 最終的に画面に描画される位置はカメラのパースペクティブ側で調整するため
        // オブジェクトの本来の座標は画面の中心に固定する
        pAct->trans.x =   (fx32)( OBD_LCD_X / 2 ) << FX32_SHIFT;
        pAct->trans.y =  -(fx32)( OBD_LCD_Y / 2 ) << FX32_SHIFT;

        // 近接クリップ面の位置をオフセット分だけずらして射影する
        org_prj = *NNS_G3dGlbGetProjectionMtx();
        NNS_G3dGlbFrustum(
            +nh-oh,
            -nh-oh,
            -nw+ow,
            +nw+ow,
            cam_persp->near,
            cam_persp->far );
    }
    // 3Dスプライトを2Dスプライトのように表示
    else if ( ulDispFlag & OBD_DISP_3D_SPRITE ){
        MTS_UTIL_CAMERA_LOOKAT  camera;
        camera = *g_obj.camera3d;
        // カメラ位置更新
        camera.at.x     =  FX32_ONE * GX_LCD_SIZE_X / 2;
        camera.at.y     = -FX32_ONE * GX_LCD_SIZE_Y / 2;
        camera.at.z     = 0;
        camera.pos.x =  FX32_ONE * GX_LCD_SIZE_X / 2;
        camera.pos.y = -FX32_ONE * GX_LCD_SIZE_Y / 2;
        camera.persp.fov = OBD_CAMERA_2DSPRITE_FOV;
        // Ｘ、Ｙ座標がディスプレイの座標と１対１に対応するようにカメラの距離を設定する
        camera.pos.z = ( GX_LCD_SIZE_Y / 2 ) * FX_Div(
            mtMathCos( camera.persp.fov ),
            mtMathSin( camera.persp.fov ) );
        camera.up.x     = 0;
        camera.up.y     = FX32_ONE;
        camera.up.z     = 0;
        // カメラ関係を設定する
        mtUtilSetCameraG3dGlb( &camera );

    }
    
    // アニメ速度をGlobal速度にあわす
    if ( pActN ){
        fSpdWork = pActN->master_anim_spd;
        pActN->master_anim_spd = FX_Mul( pActN->master_anim_spd, g_obj.speed );
        fSpdWorkBlend = pActN->master_blend_spd;
        pActN->master_blend_spd = (fx16)FX_Mul( pActN->master_blend_spd, g_obj.speed );

    }
    if ( pAct2d ){
        sSpdWork = pAct2d->speed;
        pAct2d->speed = FX_Mul(pAct2d->speed, g_obj.speed);
    }    
    // アニメ停止チェック
	if (pActN) {
		if (ulDispFlag & OBD_DISP_STOP) {
			pActN->master_anim_spd = 0;
		}
	}
	if (pAct2d) {
		if (ulDispFlag & OBD_DISP_STOP ||
			((ulDispFlag & OBD_DISP_NOUPDATE) && 0 >= pAct2d->timer)) {// OBD_DISP_NOUPDATEでも1パターン目だけは転送する対応
			pAct2d->speed = 0;
		}
	}
//    if (( ulDispFlag & OBD_DISP_STOP )){
//        if ( pActN )
//            pActN->master_anim_spd = 0;
//        if ( pAct2d )
//            pAct2d->speed = 0;
//    }

    // アニメーションを進める
    if (!( ulDispFlag & OBD_DISP_NOUPDATE ) ||
    		(pAct2d && (ulDispFlag & OBD_DISP_NOUPDATE) && 0 >= pAct2d->timer)) {
        if ( pActN && pActN->ro.recJntAnm ) {
            pActN->ro.flag |= NNS_G3D_RENDEROBJ_FLAG_RECORD;
		}
        if ( pActS ) {
			u32	flag_work;
			if (act_uncomp) {
				// 解凍・転送分離設定
				pAct2d->cha_vram = MTE_CHA_VRAM_ADDRESS;
				pAct2d->cha_addr = (u32)act_uncomp->cha_uncomp;

				// アクションフラグ退避
				flag_work = pAct2d->flag & (MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_REQ_CHA);
				pAct2d->flag &= ~MTD_ACT_FLAG_NO_CHA;		// 即時転送に変更
				pAct2d->flag |= MTD_ACT_FLAG_NO_REQ_CHA;
			}

			// アクション更新
            mtAct3dUpdateSprite(pActS, pActCall, ulActCallAddr);

			if (act_uncomp) {
				// 設定復帰
				pAct2d->cha_vram = act_uncomp->cha_vram;
				pAct2d->cha_addr = act_uncomp->cha_addr;
				pAct2d->flag = (pAct2d->flag & ~(MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_REQ_CHA)) | flag_work;
			}
		}
        else
            // アクション更新
            mtAct3dUpdateAll(pAct);
    }else{
        if ( pActN )
            pActN->ro.flag &= ~NNS_G3D_RENDEROBJ_FLAG_RECORD;

    }
    // 速度戻す
    if ( pAct2d )
        pAct2d->speed = sSpdWork;
    if ( pActN ){
        pActN->master_anim_spd = fSpdWork;
        pActN->master_blend_spd = fSpdWorkBlend;
    }
    // 非表示チェック
    if (!( ulDispFlag & OBD_DISP_NODISP )){

        // 1SHAPEデータ
        if ( pSimpleList ){
            // 他パーツ描画ループ
            MTS_ACTION3D * pActTemp;
            OBS_ACTION3D_SIMPLE_WORK * pSimpleTemp = pSimpleList;
            
            for ( ;; ){
                if ( pSimpleTemp ){
                    pActTemp = &pSimpleTemp->act_s3d.a3d;
                    if ( !(pSimpleTemp->flag & OBD_ACTFLAG_3D_NODISP) ){
                        // 座標、回転行列などコピー
                        VEC_Set( &pActTemp->scale, pAct->scale.x, pAct->scale.y, pAct->scale.z);
                        VEC_Set( &pActTemp->trans, pAct->trans.x, pAct->trans.y, pAct->trans.z);
                        if ( !(pSimpleTemp->flag & OBD_ACTFLAG_3D_ROT_NOCOPY ))
                            MTX_Copy33( &pAct->rot, &pActTemp->rot );
                        mtAct3dDrawAll(pActTemp);
                    }
                } else {
                    break;
                }
                pSimpleTemp = pSimpleTemp->next;
            }
        }else{
            mtAct3dDrawAll(pAct);
        }
    }
    // 変更した射影行列を元に戻す
    if( ulDispFlag & OBD_DISP_3D_PARALLEL )
        *(MtxFx44*)NNS_G3dGlbGetProjectionMtx() = org_prj;
    // 変更したカメラを元に戻す
    if ( ulDispFlag & OBD_DISP_3D_SPRITE )
        mtUtilSetCameraG3dGlb( g_obj.camera3d );

    // アニメ終了フラグ管理
    if ( pActN ){
        if ( pActN->aflag[MTE_ACT3D_NNS_ANIM_CA] & MTD_ACT3D_NNS_AFLAG_BLEND_END ){
            ulDispFlag &= ~OBD_DISP_3D_BLEND;
        }
        // 終了フラグセット
        if ( pActN->aflag[MTE_ACT3D_NNS_ANIM_CA] & MTD_ACT3D_NNS_AFLAG_END ){
            ulDispFlag &= ~OBD_DISP_3D_BLEND;
            ulDispFlag |= OBD_DISP_END;
        }
    }
    if ( pAct2d ){
        if (pAct2d->flag & MTD_ACT_FLAG_END )
            ulDispFlag |= OBD_DISP_END;
    }
    if ( vScale )
        pAct->scale = vScaleWork;
    if ( pDispFlag )
        *pDispFlag = ulDispFlag;

}
#endif // #if (OBD_USE_ACTION3D_NNS || OBD_USE_ACTION3D_1M1S || OBD_USE_ACTION3D_SPR)


#if OBD_USE_ACTION3D_SS
// ==========================================================================
// ObjDrawObjectActionSS
/*!
 *	オブジェクトソフトウェアスプライトアクション
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param act_spr		[in]	アクションポインタ
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
//void ObjDrawObjectActionSS(OBS_OBJECT_WORK *obj_work, MTS_ACTION_SS *act_ss, OBS_ACTION_UNCOMP_WORK *act_uncomp)
void ObjDrawObjectActionSS(OBS_OBJECT_WORK *obj_work, MTS_ACTION_SS *act_ss, OBS_ACTION_SP_SETTING_WORK *sp_setting)
{
	VecFx32	pos;
	VecU16	dir;

    // セット
	pos = obj_work->pos;
	dir = obj_work->dir;

	// オフセットを足す
	pos.x += obj_work->ofst.x;
	pos.y += obj_work->ofst.y;

	// 逆重力対応
	if (obj_work->dir_fall) {
		dir.z += obj_work->dir_fall;
	}

	// 3DオブジェクトならA側の処理は不要
//	if (obj_work->obj_3d || obj_work->obj_s3d || obj_work->obj_3dspr) {
//		act_spr->flag |= MTD_ACT_DS_FLAG_DISABLE_GE_A;
//	}

	ObjDrawActionSS(act_ss, &pos, &dir, &obj_work->scale, &obj_work->disp_flag, (MTF_ACT_CMD_CB)obj_work->ppActCall, (u32)obj_work, sp_setting);
}

// ==========================================================================
// ObjDrawActionSS
/*!
 *	オブジェクトアクション
 *
 *	@param pActSS			[io]	アクションポインタ 
 *	@param vPos				[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir				[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale			[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag		[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pActCall			[in]	アクションコールバック （ NULL可 ）
 *	@param ulActCallAddr	[in]	アクションコールバック引数アドレス （ NULL可 ）
 *	@param act_uncomp		[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
void ObjDrawActionSS( MTS_ACTION_SS * pActSS, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, MTF_ACT_CMD_CB pActCall, u32 ulActCallAddr, OBS_ACTION_SP_SETTING_WORK *sp_setting)
{
	s32				lX = 0, lY = 0;
	s32				sSpdWork;
	u32				ulDispFlag = 0;
	u8				uc3D = 0;
	fx32			fScaleX, fScaleY;
	u16				usDir = 0;

	if (pDispFlag) {
		ulDispFlag = *pDispFlag;

		// チェックフラグを落とす
		pActSS->act.flag &= ~( MTD_ACT_FLAG_REPEAT | MTD_ACT_FLAG_FLIP_H | MTD_ACT_FLAG_FLIP_V);
		*pDispFlag &= ~(OBD_DISP_END);

		// フラグ設定
		if ( ulDispFlag & OBD_DISP_REPEAT )
			pActSS->act.flag |= MTD_ACT_FLAG_REPEAT;
		if ( ulDispFlag & OBD_DISP_HFLIP )
			pActSS->act.flag |= MTD_ACT_FLAG_FLIP_H;
		if ( ulDispFlag & OBD_DISP_VFLIP )
			pActSS->act.flag |= MTD_ACT_FLAG_FLIP_V;
	}

	if (vDir && !(ulDispFlag & OBD_DISP_NODIR)) {
		usDir = vDir->z;
	}

	// カメラ位置設定
	if ((g_obj.flag & OBD_OBJ_CAMERA) && !(ulDispFlag & OBD_DISP_NOMAP)) {
		if (g_obj.pp3dCam) {
			// カメラ取得関数実行
			g_obj.pp3dCam(&lX, &lY);
		}
		else {
			// メイン2Dカメラから取得
			lX = g_obj.camera[MTE_GE2_A][MTD_X];
			lY = g_obj.camera[MTE_GE2_A][MTD_Y];
		}

	//	lX >>= FX32_SHIFT;
	//	lY >>= FX32_SHIFT;
	}
	if (!(ulDispFlag & OBD_DISP_NOPOS)) {
		// 位置設定
		if (vPos) {
			pActSS->act.pos[MTD_X] = (s16)((vPos->x - lX) >> FX32_SHIFT);
			pActSS->act.pos[MTD_Y] = (s16)((vPos->y - lY) >> FX32_SHIFT);

			// pActSS->act.pos[MTD_X] = (s16)( (vPos->x >> FX32_SHIFT) - lX );
			// pActSS->act.pos[MTD_Y] = (s16)( (vPos->y >> FX32_SHIFT) - lY );

			if ((g_obj.flag & OBD_OBJ_BELT) &&
        			!(ulDispFlag & OBD_DISP_NOBELT)) {
				pActSS->act.pos[MTD_Y] += FX_Mul(vPos->z, g_obj.depth) >> FX32_SHIFT;
			}
		}
		if (!(ulDispFlag & OBD_DISP_NOOFST)) {
			pActSS->act.pos[MTD_X] += g_obj.offset[MTD_X];
			pActSS->act.pos[MTD_Y] += g_obj.offset[MTD_Y];
		}
	}

	// アニメ速度をGlobal速度にあわす
	sSpdWork = pActSS->act.speed;
	pActSS->act.speed = FX_Mul(pActSS->act.speed, g_obj.speed);

	// アクションを進めない（再転送時のみ転送処理）単独使用を可能にするためMTD_ACT_FLAG_STOPは用いない
	// OBD_DISP_NOUPDATEでも1パターン目だけは転送する対応
	if (ulDispFlag & OBD_DISP_STOP ||
			((ulDispFlag & OBD_DISP_NOUPDATE) && 0 >= pActSS->act.timer)) {
		pActSS->act.speed = 0;
	}

	// アクション更新
	if (!(ulDispFlag & OBD_DISP_NOUPDATE) ||
			((ulDispFlag & OBD_DISP_NOUPDATE) && 0 >= pActSS->act.timer)) {	// OBD_DISP_NOUPDATEでも1パターン目だけは転送する対応
		u32	flag_work;
		OBS_ACTION_UNCOMP_WORK		*act_uncomp = NULL;
#if OBD_USE_TEX_VRAM_DB
		OBS_ACTION_TEXVRAM_DB_WORK	*act_texdb = NULL;
#endif // #if OBD_USE_TEX_VRAM_DB

		if (sp_setting) {
			switch (sp_setting->sp_setting_type) {
			case OBE_OBJ_ACTION_SP_SETTING_TYPE_UNCOMP:
				act_uncomp = (OBS_ACTION_UNCOMP_WORK*)sp_setting;
				break;
#if OBD_USE_TEX_VRAM_DB
			case OBE_OBJ_ACTION_SP_SETTING_TYPE_TEXVRAM_DB:
				act_texdb = (OBS_ACTION_TEXVRAM_DB_WORK*)sp_setting;
				break;
#endif // #if OBD_USE_TEX_VRAM_DB
			}
		}


		if (act_uncomp) {
			// 解凍・転送分離設定
			pActSS->act.cha_vram = MTE_CHA_VRAM_ADDRESS;
			pActSS->act.cha_addr = (u32)act_uncomp->cha_uncomp;

			// アクションフラグ退避
			flag_work = pActSS->act.flag & (MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_REQ_CHA);
			pActSS->act.flag &= ~MTD_ACT_FLAG_NO_CHA;		// 即時転送に変更
			pActSS->act.flag |= MTD_ACT_FLAG_NO_REQ_CHA;
		}
#if OBD_USE_TEX_VRAM_DB
		else if (act_texdb) {
			// 現在の転送アドレス設定
			pActSS->act.cha_vram = MTE_CHA_VRAM_ADDRESS;
			pActSS->act.cha_addr = g_obj.db_tex_slot_at_lcdc[g_obj.db_tex_vram_flip ^ 0x01][act_texdb->slot_no] +
											act_texdb->ofst_addr;
			// アクションフラグ退避
			flag_work = pActSS->act.flag & (MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_REQ_CHA);
			pActSS->act.flag &= ~MTD_ACT_FLAG_NO_CHA;		// 即時転送に変更
			pActSS->act.flag |= MTD_ACT_FLAG_NO_REQ_CHA;

			// 転送済みフラグOFF
			act_texdb->trans_flag = FALSE;
		}
#endif // #if OBD_USE_TEX_VRAM_DB

		// アクション更新
		mtActUpdateSS(pActSS, pActCall, ulActCallAddr);

		if (act_uncomp) {
			// 設定復帰
			pActSS->act.cha_vram = act_uncomp->cha_vram;
			pActSS->act.cha_addr = act_uncomp->cha_addr;
			pActSS->act.flag = (pActSS->act.flag & ~(MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_REQ_CHA)) | flag_work;
		}
#if OBD_USE_TEX_VRAM_DB
		else if (act_texdb) {
			if (!act_texdb->trans_flag) {
				// 今フレームは転送されていないので 転送する
				mtActRestoreSS(pActSS);
			}

			pActSS->act.cha_vram = act_texdb->cha_vram;
			pActSS->act.cha_addr = act_texdb->cha_addr;
			pActSS->act.flag = (pActSS->act.flag & ~(MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_REQ_CHA)) | flag_work;
		}
#endif // #if OBD_USE_TEX_VRAM_DB
	}

	// 速度戻す
	pActSS->act.speed = sSpdWork;

	// 拡大率設定
#if 1
	{
		fScaleX = fScaleY = FX32_ONE;

		if (vScale && !(ulDispFlag & OBD_DISP_NOSCALE)) {
			fScaleX = vScale->x;
			fScaleY = vScale->y;
		}
		if (!(ulDispFlag & OBD_DISP_NODRAWSCALE)) {
			fScaleX = FX_Mul(fScaleX, g_obj.draw_scale.x);
			fScaleY = FX_Mul(fScaleY, g_obj.draw_scale.y);
		}
		if (!(ulDispFlag & OBD_DISP_NOGLBSCALE)) {
			fScaleX = FX_Mul(fScaleX, g_obj.glb_scale.x);
			fScaleY = FX_Mul(fScaleY, g_obj.glb_scale.y);

			// 拡大時位置計算
			if (g_obj.glb_scale.x != 0x1000) {
				pActSS->act.pos[MTD_X] = (s16)((s32)g_obj.cam_scale_center[0][MTD_X] + FX_Mul((pActSS->act.pos[MTD_X] - g_obj.cam_scale_center[0][MTD_X]), g_obj.glb_scale.x));
			}
			if (g_obj.glb_scale.y != 0x1000) {
				pActSS->act.pos[MTD_Y] = (s16)((s32)g_obj.cam_scale_center[0][MTD_Y] + FX_Mul((pActSS->act.pos[MTD_Y] - g_obj.cam_scale_center[0][MTD_Y]), g_obj.glb_scale.y));
			}
		}
	}
#else
	if (!(ulDispFlag & OBD_DISP_NOGLBSCALE)) {
		fScaleX = g_obj.scale.x;
		fScaleY = g_obj.scale.y;

		if (vScale && !(ulDispFlag & OBD_DISP_NOSCALE)) {
			fScaleX = FX_Mul(fScaleX, vScale->x);
			fScaleY = FX_Mul(fScaleY, vScale->y);
		}
		// 拡大時位置計算
		if (g_obj.glb_scale.x != 0x1000) {
			pActSS->act.pos[MTD_X] = (s16)((s32)g_obj.cam_scale_center[0][MTD_X] + FX_Mul((pActSS->act.pos[MTD_X] - g_obj.cam_scale_center[0][MTD_X]), g_obj.glb_scale.x));
		}
		if (g_obj.glb_scale.y != 0x1000) {
			pActSS->act.pos[MTD_Y] = (s16)((s32)g_obj.cam_scale_center[0][MTD_Y] + FX_Mul((pActSS->act.pos[MTD_Y] - g_obj.cam_scale_center[0][MTD_Y]), g_obj.glb_scale.y));
		}
	}
	else {
	// グローバルスケールを使用しない
		fScaleX = FX32_ONE;
		fScaleY = FX32_ONE;
		if (vScale && !(ulDispFlag & OBD_DISP_NOSCALE)) {
			fScaleX = FX_Mul(fScaleX, vScale->x);
			fScaleY = FX_Mul(fScaleY, vScale->y);
		}
	}
#endif

	// 表示
	if (!(ulDispFlag & OBD_DISP_NODISP)) {
		mtActSetCameraSS(48*FX32_ONE);		// 描画のゆがみ改善の為 FX32_ONE >> 48*FX32_ONEに変更
											// あふれる場合は修正すること
		NNS_G3dGeMtxMode(GX_MTXMODE_POSITION);
		NNS_G3dGeIdentity();
		if (usDir ||
				(fScaleX != FX32_ONE || fScaleY != FX32_ONE)) {
			mtActDrawAffineSS(pActSS, fScaleX, fScaleY, usDir);
		}
		else {
			mtActDrawSS( pActSS );
		}
	}

//    if ( !(ulDispFlag & OBD_DISP_RECTONLY) ){
        // 終了フラグセット
	if (pDispFlag &&
			(pActSS->act.flag & MTD_ACT_FLAG_END)) {
		*pDispFlag |= OBD_DISP_END;
	}
//    }
}
#endif // #if OBD_USE_ACTION3D_SS


#if OBD_USE_ACTION3D_POLY
// ==========================================================================
// ObjDrawObjectActionPoly
/*!
 *	オブジェクトポリゴンアクションアクション
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param act_spr		[in]	アクションポインタ
 *	@param sp_setting	[in]	テクスチャ解凍・転送分離用ワーク(NULL可) 現在無効
 */
// ==========================================================================
void ObjDrawObjectActionPoly(OBS_OBJECT_WORK *obj_work, IZS_PLA_ACTION *act_poly, OBS_ACTION_SP_SETTING_WORK *sp_setting)
{
	VecFx32	pos;
	VecU16	dir;

    // セット
	pos = obj_work->pos;
	dir = obj_work->dir;

	// オフセットを足す
	pos.x += obj_work->ofst.x;
	pos.y += obj_work->ofst.y;

	// 逆重力対応
	if (obj_work->dir_fall) {
		dir.z += obj_work->dir_fall;
	}

	// 3DオブジェクトならA側の処理は不要
//	if (obj_work->obj_3d || obj_work->obj_s3d || obj_work->obj_3dspr) {
//		act_spr->flag |= MTD_ACT_DS_FLAG_DISABLE_GE_A;
//	}

	ObjDrawActionPoly(act_poly, &pos, &dir, &obj_work->scale, &obj_work->disp_flag, (MTF_ACT_CMD_CB)obj_work->ppActCall, (u32)obj_work, sp_setting);
}

// ==========================================================================
// ObjDrawActionPoly
/*!
 *	オブジェクトアクション
 *
 *	@param pActSS			[io]	アクションポインタ 
 *	@param vPos				[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir				[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale			[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag		[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pActCall			[in]	アクションコールバック （ NULL可 ）現在無効
 *	@param ulActCallAddr	[in]	アクションコールバック引数アドレス （ NULL可 ）現在無効
 *	@param act_uncomp		[in]	テクスチャ解凍・転送分離用ワーク(NULL可) 現在無効
 */
// ==========================================================================
void ObjDrawActionPoly(IZS_PLA_ACTION *act_poly, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, MTF_ACT_CMD_CB pActCall, u32 ulActCallAddr, OBS_ACTION_SP_SETTING_WORK *sp_setting)
{
	s32				lX = 0, lY = 0;
	s32				sSpdWork;
	u32				ulDispFlag = 0;
	u8				uc3D = 0;
	fx32			fScaleX, fScaleY;
	u16				usDir = 0;

	if (pDispFlag) {
		ulDispFlag = *pDispFlag;

		// チェックフラグを落とす
		act_poly->flag &= ~( IZD_PLA_ACTION_FLAG_REPEAT | IZD_PLA_ACTION_FLAG_FLIP_H | IZD_PLA_ACTION_FLAG_FLIP_V);
		*pDispFlag &= ~(OBD_DISP_END);

		// フラグ設定
		if ( ulDispFlag & OBD_DISP_REPEAT )
			act_poly->flag |= IZD_PLA_ACTION_FLAG_REPEAT;
		if ( ulDispFlag & OBD_DISP_HFLIP )
			act_poly->flag |= IZD_PLA_ACTION_FLAG_FLIP_H;
		if ( ulDispFlag & OBD_DISP_VFLIP )
			act_poly->flag |= IZD_PLA_ACTION_FLAG_FLIP_V;
	}

	// 角度設定
	if (vDir && !(ulDispFlag & OBD_DISP_NODIR)) {
		usDir = vDir->z;
	}
	act_poly->dir = usDir;

	// カメラ位置設定
	if ((g_obj.flag & OBD_OBJ_CAMERA) && !(ulDispFlag & OBD_DISP_NOMAP)) {
		if (g_obj.pp3dCam) {
			// カメラ取得関数実行
			g_obj.pp3dCam(&lX, &lY);
		}
		else {
			// メイン2Dカメラから取得
			lX = g_obj.camera[MTE_GE2_A][MTD_X];
			lY = g_obj.camera[MTE_GE2_A][MTD_Y];
		}

	//	lX >>= FX32_SHIFT;
	//	lY >>= FX32_SHIFT;
	}
	if (!(ulDispFlag & OBD_DISP_NOPOS)) {
		// 位置設定
		if (vPos) {
			act_poly->pos[MTD_X] = (vPos->x - lX);
			act_poly->pos[MTD_Y] = (vPos->y - lY);

			// pActSS->act.pos[MTD_X] = (s16)( (vPos->x >> FX32_SHIFT) - lX );
			// pActSS->act.pos[MTD_Y] = (s16)( (vPos->y >> FX32_SHIFT) - lY );

			if ((g_obj.flag & OBD_OBJ_BELT) &&
        			!(ulDispFlag & OBD_DISP_NOBELT)) {
				act_poly->pos[MTD_Y] += FX_Mul(vPos->z, g_obj.depth);
			}
		}
		if (!(ulDispFlag & OBD_DISP_NOOFST)) {
			act_poly->pos[MTD_X] += g_obj.offset[MTD_X] << FX32_SHIFT;
			act_poly->pos[MTD_Y] += g_obj.offset[MTD_Y] << FX32_SHIFT;
		}
	}

	// アニメ速度をGlobal速度にあわす
	sSpdWork = act_poly->speed;
	act_poly->speed = FX_Mul(act_poly->speed, g_obj.speed);

	// アクションを進めない（再転送時のみ転送処理）単独使用を可能にするためMTD_ACT_FLAG_STOPは用いない
	// OBD_DISP_NOUPDATEでも1パターン目だけは転送する対応
	if (ulDispFlag & OBD_DISP_STOP ||
			((ulDispFlag & OBD_DISP_NOUPDATE) && 0 >= act_poly->frame)) {
		act_poly->speed = 0;
	}

	// アクション更新
	if (!(ulDispFlag & OBD_DISP_NOUPDATE) ||
			((ulDispFlag & OBD_DISP_NOUPDATE) && 0 >= act_poly->frame)) {	// OBD_DISP_NOUPDATEでも1パターン目だけは転送する対応
#if 0
		u32	flag_work;
		OBS_ACTION_UNCOMP_WORK		*act_uncomp = NULL;
#if OBD_USE_TEX_VRAM_DB
		OBS_ACTION_TEXVRAM_DB_WORK	*act_texdb = NULL;
#endif // #if OBD_USE_TEX_VRAM_DB

		if (sp_setting) {
			switch (sp_setting->sp_setting_type) {
			case OBE_OBJ_ACTION_SP_SETTING_TYPE_UNCOMP:
				act_uncomp = (OBS_ACTION_UNCOMP_WORK*)sp_setting;
				break;
#if OBD_USE_TEX_VRAM_DB
			case OBE_OBJ_ACTION_SP_SETTING_TYPE_TEXVRAM_DB:
				act_texdb = (OBS_ACTION_TEXVRAM_DB_WORK*)sp_setting;
				break;
#endif // #if OBD_USE_TEX_VRAM_DB
			}
		}


		if (act_uncomp) {
			// 解凍・転送分離設定
			pActSS->act.cha_vram = MTE_CHA_VRAM_ADDRESS;
			pActSS->act.cha_addr = (u32)act_uncomp->cha_uncomp;

			// アクションフラグ退避
			flag_work = pActSS->act.flag & (MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_REQ_CHA);
			pActSS->act.flag &= ~MTD_ACT_FLAG_NO_CHA;		// 即時転送に変更
			pActSS->act.flag |= MTD_ACT_FLAG_NO_REQ_CHA;
		}
#if OBD_USE_TEX_VRAM_DB
		else if (act_texdb) {
			// 現在の転送アドレス設定
			pActSS->act.cha_vram = MTE_CHA_VRAM_ADDRESS;
			pActSS->act.cha_addr = g_obj.db_tex_slot_at_lcdc[g_obj.db_tex_vram_flip ^ 0x01][act_texdb->slot_no] +
											act_texdb->ofst_addr;
			// アクションフラグ退避
			flag_work = pActSS->act.flag & (MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_REQ_CHA);
			pActSS->act.flag &= ~MTD_ACT_FLAG_NO_CHA;		// 即時転送に変更
			pActSS->act.flag |= MTD_ACT_FLAG_NO_REQ_CHA;

			// 転送済みフラグOFF
			act_texdb->trans_flag = FALSE;
		}
#endif // #if OBD_USE_TEX_VRAM_DB
#endif // #if 0

		// アクション更新
		IzPolyActUpdate(act_poly);

#if 0
		if (act_uncomp) {
			// 設定復帰
			pActSS->act.cha_vram = act_uncomp->cha_vram;
			pActSS->act.cha_addr = act_uncomp->cha_addr;
			pActSS->act.flag = (pActSS->act.flag & ~(MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_REQ_CHA)) | flag_work;
		}
#if OBD_USE_TEX_VRAM_DB
		else if (act_texdb) {
			if (!act_texdb->trans_flag) {
				// 今フレームは転送されていないので 転送する
				mtActRestoreSS(pActSS);
			}

			pActSS->act.cha_vram = act_texdb->cha_vram;
			pActSS->act.cha_addr = act_texdb->cha_addr;
			pActSS->act.flag = (pActSS->act.flag & ~(MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_REQ_CHA)) | flag_work;
		}
#endif // #if OBD_USE_TEX_VRAM_DB
#endif // #if 0
	}

	// 速度戻す
	act_poly->speed = sSpdWork;

	// 拡大率設定
#if 1
	{
		fScaleX = fScaleY = FX32_ONE;

		if (vScale && !(ulDispFlag & OBD_DISP_NOSCALE)) {
			fScaleX = vScale->x;
			fScaleY = vScale->y;
		}
		if (!(ulDispFlag & OBD_DISP_NODRAWSCALE)) {
			fScaleX = FX_Mul(fScaleX, g_obj.draw_scale.x);
			fScaleY = FX_Mul(fScaleY, g_obj.draw_scale.y);
		}
		if (!(ulDispFlag & OBD_DISP_NOGLBSCALE)) {
			fScaleX = FX_Mul(fScaleX, g_obj.glb_scale.x);
			fScaleY = FX_Mul(fScaleY, g_obj.glb_scale.y);

		    // 拡大時位置計算
			if (g_obj.glb_scale.x != 0x1000) {
				//act_poly->pos[MTD_X] = (s16)((s32)g_obj.cam_scale_center[0][MTD_X] + FX_Mul((act_poly->pos[MTD_X] - g_obj.cam_scale_center[0][MTD_X]), g_obj.glb_scale.x));
				act_poly->pos[MTD_X] = (g_obj.cam_scale_center[0][MTD_X] << FX32_SHIFT) +
							FX_Mul((act_poly->pos[MTD_X] - (g_obj.cam_scale_center[0][MTD_X] << FX32_SHIFT)), g_obj.glb_scale.x);
			}
			if (g_obj.glb_scale.y != 0x1000) {
				//act_poly->pos[MTD_Y] = (s16)((s32)g_obj.cam_scale_center[0][MTD_Y] + FX_Mul((act_poly->pos[MTD_Y] - g_obj.cam_scale_center[0][MTD_Y]), g_obj.glb_scale.y));
				act_poly->pos[MTD_Y] = (g_obj.cam_scale_center[0][MTD_Y] << FX32_SHIFT) +
							FX_Mul((act_poly->pos[MTD_Y] - (g_obj.cam_scale_center[0][MTD_Y] << FX32_SHIFT)), g_obj.glb_scale.y);
			}
		}
		act_poly->scale[MTD_X] = fScaleX;
		act_poly->scale[MTD_Y] = fScaleY;
	}
#else
	if (!(ulDispFlag & OBD_DISP_NOGLBSCALE)) {
		fScaleX = g_obj.scale.x;
		fScaleY = g_obj.scale.y;

		if (vScale && !(ulDispFlag & OBD_DISP_NOSCALE)) {
			fScaleX = FX_Mul(fScaleX, vScale->x);
			fScaleY = FX_Mul(fScaleY, vScale->y);
		}
		// 拡大時位置計算
		if (g_obj.glb_scale.x != 0x1000) {
			//act_poly->pos[MTD_X] = (s16)((s32)g_obj.cam_scale_center[0][MTD_X] + FX_Mul((act_poly->pos[MTD_X] - g_obj.cam_scale_center[0][MTD_X]), g_obj.glb_scale.x));
			act_poly->pos[MTD_X] = (g_obj.cam_scale_center[0][MTD_X] << FX32_SHIFT) +
						FX_Mul((act_poly->pos[MTD_X] - (g_obj.cam_scale_center[0][MTD_X] << FX32_SHIFT)), g_obj.glb_scale.x);
		}
		if (g_obj.glb_scale.y != 0x1000) {
			//act_poly->pos[MTD_Y] = (s16)((s32)g_obj.cam_scale_center[0][MTD_Y] + FX_Mul((act_poly->pos[MTD_Y] - g_obj.cam_scale_center[0][MTD_Y]), g_obj.glb_scale.y));
			act_poly->pos[MTD_Y] = (g_obj.cam_scale_center[0][MTD_Y] << FX32_SHIFT) +
						FX_Mul((act_poly->pos[MTD_Y] - (g_obj.cam_scale_center[0][MTD_Y] << FX32_SHIFT)), g_obj.glb_scale.y);
		}
	}
	else {
	// グローバルスケールを使用しない
		fScaleX = FX32_ONE;
		fScaleY = FX32_ONE;
		if (vScale && !(ulDispFlag & OBD_DISP_NOSCALE)) {
			fScaleX = FX_Mul(fScaleX, vScale->x);
			fScaleY = FX_Mul(fScaleY, vScale->y);
		}
	}
	act_poly->scale[MTD_X] = fScaleX;
	act_poly->scale[MTD_Y] = fScaleY;
#endif

	// 表示
	if (!(ulDispFlag & OBD_DISP_NODISP)) {
		IzPolyActSetCamera(48*FX32_ONE);
		NNS_G3dGeMtxMode(GX_MTXMODE_POSITION);
		NNS_G3dGeIdentity();

		IzPolyActDraw(act_poly);
	}

//    if ( !(ulDispFlag & OBD_DISP_RECTONLY) ){
        // 終了フラグセット
	if (pDispFlag &&
			(act_poly->flag & IZD_PLA_ACTION_FLAG_END)) {
		*pDispFlag |= OBD_DISP_END;
	}
//    }
}

#endif // #if OBD_USE_ACTION3D_POLY


#if OBD_USE_ACTION3D_SMA
// ==========================================================================
// ObjDrawObjectAction3DSma
/*!
 *	オブジェクト3D SMAアクション
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param act_sma	[in]	アクションポインタ
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
void ObjDrawObjectAction3DSma(OBS_OBJECT_WORK *obj_work, MTS_SMA *act_sma, OBS_ACTION_UNCOMP_WORK *act_uncomp)
{
	VecFx32	pos;
	VecU16	dir;

    // セット
	pos = obj_work->pos;
	dir = obj_work->dir;

	// オフセットを足す
	pos.x += obj_work->ofst.x;
	pos.y += obj_work->ofst.y;

	// 逆重力対応
	if (obj_work->dir_fall) {
		dir.z += obj_work->dir_fall;
	}

	// 3DオブジェクトならA側の処理は不要
//	if (obj_work->obj_3d || obj_work->obj_s3d || obj_work->obj_3dspr) {
//		act_spr->flag |= MTD_ACT_DS_FLAG_DISABLE_GE_A;
//	}

	ObjDrawAction3DSma(act_sma, &pos, &dir, &obj_work->scale, &obj_work->disp_flag, (OBF_SMA_CMD_CB)obj_work->ppActCall, (u32)obj_work, act_uncomp);
}

// ==========================================================================
// ObjDrawAction3DSma
/*!
 *	オブジェクト3D SMAアクション
 *
 *	@param act_sma		[io]	アクションポインタ 
 *	@param vPos				[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir				[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale			[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag		[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pActCall			[in]	アクションコールバック （ NULL可 ）
 *	@param ulActCallAddr	[in]	アクションコールバック引数アドレス （ NULL可 ）
 *	@param act_uncomp		[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 */
// ==========================================================================
void ObjDrawAction3DSma(MTS_SMA *act_sma, VecFx32* vPos, VecU16* vDir, VecFx32* vScale, u32* pDispFlag, OBF_SMA_CMD_CB pActCall, u32 ulActCallAddr, OBS_ACTION_UNCOMP_WORK *act_uncomp)
{
	fx32			cam_x = 0, cam_y = 0;
	fx32			spd_work;
	u32				ulDispFlag = 0;
	u8				uc3D = 0;
	fx32			fScaleX, fScaleY;
//	u16				usDir = 0;
	s32				flip_h = 1, flip_v = 1;	// フリップスケール
//	MtxFx44			org_prj;				// プロジェクション保持ワーク

	// 角度設定
	if (!(ulDispFlag & OBD_DISP_NODIR) && vDir) {
		//usDir = vDir->z;
		act_sma->angle = vDir->z;
	}
	else {
		act_sma->angle = 0;
	}

	// 表示フラグ取得
	// フリップ設定
	if (pDispFlag) {
		ulDispFlag = *pDispFlag;

		// チェックフラグを落とす
	//	pActSS->act.flag &= ~( MTD_ACT_FLAG_REPEAT | MTD_ACT_FLAG_FLIP_H | MTD_ACT_FLAG_FLIP_V);
		*pDispFlag &= ~(OBD_DISP_END);

		// フラグ設定
	//	if ( ulDispFlag & OBD_DISP_REPEAT ) {
	//		pActSS->act.flag |= MTD_ACT_FLAG_REPEAT;
	//	}
		if ( ulDispFlag & OBD_DISP_HFLIP ) {
			flip_h = -1;
		}
		if ( ulDispFlag & OBD_DISP_VFLIP ) {
			flip_v = -1;
		}
	}

	// カメラ位置設定
	if ((g_obj.flag & OBD_OBJ_CAMERA) && !(ulDispFlag & OBD_DISP_NOMAP)) {
		if (g_obj.pp3dCam) {
			// カメラ取得関数実行
			g_obj.pp3dCam(&cam_x, &cam_y);
		}
		else {
			// メイン2Dカメラから取得
			cam_x = g_obj.camera[MTE_GE2_A][MTD_X];
			cam_y = g_obj.camera[MTE_GE2_A][MTD_Y];
		}
	}

	// 座標設定
	if (!(ulDispFlag & OBD_DISP_NOPOS)) {
		// 位置設定
		if (vPos) {
			act_sma->pos[MTD_X] = vPos->x - cam_x;
			act_sma->pos[MTD_Y] = vPos->y - cam_y;

			if ((g_obj.flag & OBD_OBJ_BELT) &&
        			!(ulDispFlag & OBD_DISP_NOBELT)) {
				act_sma->pos[MTD_Y] += FX_Mul(vPos->z, g_obj.depth);
			}
		}
		if (!(ulDispFlag & OBD_DISP_NOOFST)) {
			act_sma->pos[MTD_X] += g_obj.offset[MTD_X] << FX32_SHIFT;
			act_sma->pos[MTD_Y] += g_obj.offset[MTD_Y] << FX32_SHIFT;
		}
	}

	// 拡大率設定
	// フリップも反映
	fScaleX = g_obj.scale.x * flip_h;
	fScaleY = g_obj.scale.y * flip_v;
	if (vScale) {
		fScaleX = FX_Mul(fScaleX, vScale->x);
		fScaleY = FX_Mul(fScaleY, vScale->y);
	}
	act_sma->scale[MTD_X] = fScaleX;
	act_sma->scale[MTD_Y] = fScaleY;

	// 拡大時位置計算
	if (g_obj.glb_scale.x != 0x1000) {
		fx32	scale_center = (s32)g_obj.cam_scale_center[0][MTD_X] << FX32_SHIFT;
		act_sma->pos[MTD_X] = scale_center + FX_Mul(act_sma->pos[MTD_X] - scale_center, g_obj.glb_scale.x);
				//(s16)((s32)g_obj.cam_scale_center[0][MTD_X] + FX_Mul((pActSS->act.pos[MTD_X] - g_obj.cam_scale_center[0][MTD_X]), g_obj.scale.x));
	}
	if (g_obj.glb_scale.y != 0x1000) {
		fx32	scale_center = (s32)g_obj.cam_scale_center[0][MTD_Y] << FX32_SHIFT;
		act_sma->pos[MTD_Y] = scale_center + FX_Mul(act_sma->pos[MTD_Y] - scale_center, g_obj.glb_scale.y);
				//(s16)((s32)g_obj.cam_scale_center[0][MTD_Y] + FX_Mul((pActSS->act.pos[MTD_Y] - g_obj.cam_scale_center[0][MTD_Y]), g_obj.scale.y));
	}

	// アニメ速度をGlobal速度にあわす
	spd_work = act_sma->frame_speed;
	act_sma->frame_speed = FX_Mul(act_sma->frame_speed, g_obj.speed);

	// アクションを進めない（再転送時のみ転送処理）
	if (ulDispFlag & (OBD_DISP_STOP | OBD_DISP_NOUPDATE)) {
		act_sma->frame_speed = 0;
	}

	// アクション更新
//	if (!(ulDispFlag & OBD_DISP_NOUPDATE)) {	// OBD_DISP_NOUPDATEの時も、スピード0で更新する
	{
		fx32	mtn_frame;

		mtSmaUpdateFrame(act_sma);
		mtn_frame = mtSmaGetMotionFrames(act_sma, act_sma->mtn_id) << FX32_SHIFT;
		if (act_sma->frame >= mtn_frame) {

			if (ulDispFlag & OBD_DISP_REPEAT) {
				// リピート
				act_sma->frame -= mtn_frame;
			}
			else {
				//モーション終了フラグ設定
				if (pDispFlag) {
					*pDispFlag |= OBD_DISP_END;
				}
				act_sma->frame = mtn_frame;
			}
		}

		// モーション情報計算
		mtSmaUpdateMotion(act_sma);
		mtSmaUpdateMotionGlobal(act_sma);

		// テクスチャ転送リクエスト
		if (act_uncomp) {
#if 1
			// ◆とりあえず通常転送
			mtSmaTransTexture(act_sma);
#else
			BOOL	b_trans;

			// 解凍・転送分離設定
			//act_sma->cha_vram = MTE_CHA_VRAM_ADDRESS;
			act_sma->tex_vram_addr = (u32)act_uncomp->cha_uncomp;

			// 即時転送
			b_trans = objDrawActionSmaTransTexture(act_sma, MTE_CHA_VRAM_ADDRESS);

			// 設定復帰
			//act_sma->cha_vram = act_uncomp->cha_vram;
			act_sma->tex_vram_addr = act_uncomp->cha_addr;

			if (b_trans) {
				// 転送リクエスト発行
				mtChaRequestTransAddr(act_uncomp->cha_uncomp, act_uncomp->cha_size,
						act_uncomp->cha_vram, act_uncomp->cha_addr);
			}
#endif // !if 0
		}
		else {
			// 通常
			mtSmaTransTexture(act_sma);
		}

		// ユーザー情報取得
		//mtSmaAtrCalc(act_sma);

		// あたり情報取得
		mtSmaClsCalc(act_sma);

		// コールバック
		if (pActCall) {
			OBS_SMA_COMMAND	cmd = {0};
			u16	i, data_num;

			// 矩形がある時はコールバックを呼ぶ
			data_num = mtSmaClsGetCollisionNum(act_sma);
			for (i = 0; i < data_num; i++) {
				MTS_SMA_CLS		*sma_cls = mtSmaClsGetCollision(act_sma, i);
				cmd.cmd_id = OBE_SMA_COMMAND_RECT;
				cmd.addr = (u32)sma_cls;

				pActCall(&cmd, act_sma, ulActCallAddr);
			}

#if 0
			// パラメーターがある時はコールバックを呼ぶ
			data_num = mtSmaAtrGetAttributeNum(act_sma);
			for (i = 0; i < data_num; i++) {
				MTS_SMA_ATR		*sma_attr = mtSmaAtrGetAttribute(act_sma, i);
				cmd.cmd_id = OBS_SMA_COMMAND_VALUE;
				cmd.addr = (u32)sma_attr;

				pActCall(&cmd, act_sma, ulActCallAddr);
			}
#endif
		}
	}

	// 速度戻す
	act_sma->frame_speed = spd_work;

	// 表示
	if (!(ulDispFlag & OBD_DISP_NODISP)) {
		// 描画
		//mtSmaSetCamera();
	//	org_prj = *NNS_G3dGlbGetProjectionMtx();

		mtSmaSetCamera();
		NNS_G3dGeMtxMode(GX_MTXMODE_POSITION);
		NNS_G3dGeIdentity();
		mtSmaDraw(act_sma);

	//	*(MtxFx44*)NNS_G3dGlbGetProjectionMtx() = org_prj;
	}
}

#endif	// #if OBD_USE_ACTION3D_SMA



#if (OBD_USE_ACTION3D_NN)
// ==========================================================================
// ObjDrawClearNNCommandStateTbl
/*!
 *	オブジェクト NN 描画コマンドステートテーブル初期化
 *
 *	@note
 *		描画スレッドで実行されるコマンドステートを初期化します。\n
 *		OBD_DRAW_CMD_STATE_3DNN以外のコマンドが破棄されます。\n
 *		コマンドステートを再設定する際前に呼び出してください。\n
 */
// ==========================================================================
void ObjDrawClearNNCommandStateTbl(void)
{
	for (int i = 0; i < OBD_DRAW_CMD_STATE_MAX; i++) {
		obj_draw_3dnn_command_state_tbl[i] = OBD_DRAW_CMD_STATE_INVALID;
		obj_draw_3dnn_command_state_exe_end_scene_tbl[i] = FALSE;
	}
	obj_draw_3dnn_command_state_tbl[0] = OBD_DRAW_CMD_STATE_3DNN;
	obj_draw_3dnn_command_state_exe_end_scene_tbl[0] = TRUE;
}

// ==========================================================================
// ObjDrawSetNNCommandStateTbl
/*!
 *	オブジェクト NN 描画コマンドステート設定
 *
 *	@param tbl_no			[in]	設定するテーブルNO 0 ～ OBD_DRAW_CMD_STATE_MAX-1
 *	@param command_state	[in]	コマンドステート
 *	@param end_scene		[in]	amDrawEndScene 実行設定
 *
 *	@note
 *		描画スレッドで実行されるコマンドステートを設定します。\n
 *		テーブルNOの若い順から実行されます。
 */
// ==========================================================================
void ObjDrawSetNNCommandStateTbl(u32 tbl_no, u32 command_state, BOOL end_scene)
{
	MTM_ASSERT(tbl_no < OBD_DRAW_CMD_STATE_MAX);
	obj_draw_3dnn_command_state_tbl[tbl_no] = command_state;
	obj_draw_3dnn_command_state_exe_end_scene_tbl[tbl_no] = end_scene;
}

// ==========================================================================
// ObjDrawNNStart
/*!
 *	オブジェクトアクション 描画開始
 *
 *	@param pAct			[in]	アクション3Dポインタ
 *	@param vPos			[in]	オブジェクト座標 （ NULL可 ）
 *	@param vDir			[in]	オブジェクト角度 （ NULL可 ）
 *	@param vScale		[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param pDispFlag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *	@param pSimpleList	[in]	オブジェクト3D1m1s表示ワークポインタ
 *	@param pActCall		[in]	アクションコールバック （ NULL可 ）
 *	@param ulActCallAddr[in]	アクションコールバック引数アドレス （ NULL可 ）
 *
 *	@note
 *		描画スレッドに登録されたコマンドを開始します。
 */
// ==========================================================================
void ObjDrawNNStart(void)
{
	// 描画タスク作成
	amDrawMakeTask(objDrawStart_DT, 0x1000, (void*)NULL/*8byteデータバッファアドレス*/);
											// カメラデータとか渡すかも

	// amDrawMallocDataBuffer(sizeof(OBS_DRAW_PARAM_3DNN_USER_FUNC) + param_size);
}


// ==========================================================================
// ObjDraw3DNNSetCamera
/*!
 *	オブジェクトアクション 3D NN カメラ設定
 *
 *	@param	camera_id	[in]	オブジェクトカメラID -1で仮カメラ
 *	@param	proj_type	[in]	射影タイプ NNE_PROJECTION_TYPE
 *	@param	task_prio	[in]	カメラ設定タスクプライオリティ
 *
 *	@note
 *		カメラを設定します
 */
// ==========================================================================
void ObjDraw3DNNSetCamera(s32 camera_id, NNE_PROJECTION_TYPE proj_type)
{
#if 1
	OBS_CAMERA			*obj_camera = NULL;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	if (camera_id >= 0) {
		obj_camera = ObjCameraGet(camera_id);
		MTM_ASSERT(obj_camera);
	}

	objDraw3DNNSetCamera(obj_camera, proj_type, obj_camera->command_state);

#else
/*sss
	OBS_DRAW_PARAM_3DNN_SET_CAMERA	*camera_param;
	OBS_CAMERA						*obj_camera;
	u32								command_state;

	// バッファ取得
	camera_param = (OBS_DRAW_PARAM_3DNN_SET_CAMERA *)amDrawMallocDataBuffer(sizeof(OBS_DRAW_PARAM_3DNN_SET_CAMERA));

	if (camera_id >= 0) {
		obj_camera = ObjCameraGet(camera_id);
		MTM_ASSERT(obj_camera);
	}

	// 射影タイプ保存
	camera_param->proj_type = proj_type;

	if (obj_camera) {
		// コマンドステート取得
		command_state = obj_camera->command_state;

		// 射影行列コピー
		switch (proj_type) {
		default:
			MTM_ASSERT(0);
		case NNE_PROJECTION_TYPE_PERSPECTIVE:
		// 透視射影
			memcpy(camera_param->prj_mtx, obj_camera->prj_pers_mtx, sizeof(NNS_MATRIX44));
			break;

		case NNE_PROJECTION_TYPE_ORTHO:
		// 正射影
			memcpy(camera_param->prj_mtx, obj_camera->prj_ortho_mtx, sizeof(NNS_MATRIX44));
			break;
		}

		// ワールドビューマトリクスコピー
		memcpy(camera_param->view_mtx, obj_camera->view_mtx, sizeof(NNS_MATRIX));
	}
	else {
		// 仮カメラ
		NNS_MATRIX			view_mtx;
		NNS_MATRIX44		proj_mtx;
		NNS_CAMERAPTR		camera_ptr;
		NNS_CAMERA_TARGET_ROLL	camera;
		NNS_VECTOR		target_pos = {
			0.0f, 0.0f, 0.0f,
		};

		// プログラムで生成するカメラ初期設定
		camera_ptr.fType	= NND_CAMERATYPE_TARGET_ROLL;
		camera_ptr.pCamera	= &camera;
		camera.Target		= target_pos;
		camera.Position		= camera.Target;
		camera.Position.z	+= 50.0f;
		camera.Position.x	+= 0.0f;
		camera.Roll			= NNM_DEGtoA32(0.0f);	// カメラロール
		camera.Fovy			= NNM_DEGtoA32(45.0f);	// カメラ視野角
		camera.Aspect		= AMD_SCREEN_ASPECT;	// イメージアスペクト
		camera.ZNear		= 1.0f; 				// カメラ近接面
		camera.ZFar			= 60000.f; 				// カメラ遠方面

		// プロジェクション設定
		switch (proj_type) {
		default:
			MTM_ASSERT(0);
		case NNE_PROJECTION_TYPE_PERSPECTIVE:
		// 透視射影
			nnMakePerspectiveMatrix(&proj_mtx,
					camera.Fovy, camera.Aspect, camera.ZNear, camera.ZFar);
			memcpy(camera_param->prj_mtx, &proj_mtx, sizeof(NNS_MATRIX44));
			break;

		case NNE_PROJECTION_TYPE_ORTHO:
		// 正射影
			{
				float	scale = 0.078125f;	// ワイド時に横5ブロック分がこれぐらい
				float	dx, dy;
				//dy		= AMD_SCREEN_2D_HEIGHT * scale * 0.5f;
				dy		= g_obj.disp_height * scale * 0.5f;
				dx		= dy * camera.Aspect;
				nnMakeOrthoMatrix(&proj_mtx,
						-dx, dx, -dy, dy, camera.ZNear, camera.ZFar);
				memcpy(camera_param->prj_mtx, &proj_mtx, sizeof(NNS_MATRIX44));
			}
			break;
		}

		// カメラからワールドビューマトリクスを求める
		nnMakeTargetRollCameraViewMatrix(&view_mtx, &camera);

		// ワールドビューマトリクスコピー
		memcpy(camera_param->view_mtx, &view_mtx, sizeof(NNS_MATRIX));
	}

	// エフェクト用にワールドビューマトリクスを設定
	amEffectSetWorldViewMatrix(&camera_param->view_mtx);

	// 描画コマンド発行
	amDrawRegistCommand(command_state, OBD_DRAW_USER_COMMAND_3DNN_SET_CAMERA, camera_param);
 */
#endif
}

// ==========================================================================
// ObjDraw3DNNSetCameraEx
/*!
 *	オブジェクトアクション 3D NN カメラ設定
 *
 *	@param	camera_id		[in]	オブジェクトカメラID -1で仮カメラ
 *	@param	proj_type		[in]	射影タイプ NNE_PROJECTION_TYPE
 *	@param	command_state	[in]	コマンドステート
 *
 *	@note
 *		カメラを設定します\n
 *		コマンドステートをユーザー指定します。
 */
// ==========================================================================
void ObjDraw3DNNSetCameraEx(s32 camera_id, NNE_PROJECTION_TYPE proj_type, u32 command_state)
{
	OBS_CAMERA			*obj_camera = NULL;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	if (camera_id >= 0) {
		obj_camera = ObjCameraGet(camera_id);
		MTM_ASSERT(obj_camera);
	}

	objDraw3DNNSetCamera(obj_camera, proj_type, command_state);
}

// ==========================================================================
// ObjDraw3DNNUserFunc
/*!
 *	オブジェクトアクション 3D NN ユーザー処理登録
 *
 *	@param	user_func		[in]	登録するユーザー処理
 *	@param	param			[in]	受け渡すパラメータバッファ
 *	@param	param_size		[in]	受け渡すパラメータサイズ
 *	@param	command_state	[in]	描画コマンドステート
 *
 *	@note
 *		描画スレッド中のユーザー処理を登録します。
 */
// ==========================================================================
void ObjDraw3DNNUserFunc(OBF_DRAW_USER_DT_FUNC user_func, void *param, s32 param_size, u32 command_state)
{
	OBS_DRAW_PARAM_3DNN_USER_FUNC	*user_param;

	MTM_ASSERT(param_size >= 0);
	MTM_ASSERT(user_func);
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	// バッファ取得
	user_param = (OBS_DRAW_PARAM_3DNN_USER_FUNC *)amDrawMallocDataBuffer((s32)(sizeof(OBS_DRAW_PARAM_3DNN_USER_FUNC) + param_size));
	user_param->func = user_func;
	if (param && param_size) {
		// パラメーターコピー
		user_param->param = (void*)(user_param + 1);
		memcpy(user_param->param, param, (size_t)param_size);
	}
	else {
		user_param->param = NULL;
	}

	// 描画コマンド発行
	amDrawRegistCommand(command_state, OBD_DRAW_USER_COMMAND_3DNN_USER_FUNC, user_param);
}


// ==========================================================================
// ObjDrawObjectAction3DNN
/*!
 *	オブジェクトアクション 3D NN
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param obj_3d		[in]	表示ワークポインタ
 *	@param act_uncomp	[in]	テクスチャ解凍・転送分離用ワーク(NULL可)
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 *		描画は、描画スレッドに必要情報を渡して行います。
 */
// ==========================================================================
void ObjDrawObjectAction3DNN(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_NN_WORK *obj_3d)
{
	VecFx32	pos;
	VecU16	dir;

	// セット
	pos = obj_work->pos;
	dir = obj_work->dir;

	// オフセットを足す
	pos.x += obj_work->ofst.x;
	pos.y += obj_work->ofst.y;
	pos.z += obj_work->ofst.z;

	// 逆重力対応
	if (obj_work->dir_fall) {
		dir.z += obj_work->dir_fall;
	}

#if 0
	// 3DオブジェクトならA側の処理は不要
#if 1	// ◆ObjDrawObjectAction内で判定しているのでここの処理は不要では?
	if (obj_work->obj_3d || obj_work->obj_s3d) {
		if (obj_work->obj_2d) {
			obj_work->obj_2d->act_spr.flag |= MTD_ACT_DS_FLAG_DISABLE_GE_A;
		}
	}
#else
    if ( pWork->obj_3d || pWork->obj_s3d )
        pAct->flag |= MTD_ACT_DS_FLAG_DISABLE_GE_A;
#endif
#endif

	ObjDrawAction3DNN(obj_3d, &pos, &dir, &obj_work->scale, &obj_work->disp_flag);
}

// ==========================================================================
// ObjDrawAction3DNN
/*!
 *	オブジェクトアクション 3D NN
 *
 *	@param obj_3d		[in]	3DNN オブジェクトワーク
 *	@param pos			[in]	オブジェクト座標 （ NULL可 ）
 *	@param dir			[in]	オブジェクト角度 （ NULL可 ）
 *	@param scale		[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param p_disp_flag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 */
// ==========================================================================
void ObjDrawAction3DNN(OBS_ACTION3D_NN_WORK *obj_3d, VecFx32 *pos, VecU16 *dir, VecFx32 *scale, u32 *p_disp_flag)
{
	//s32			i;
	//float		spd[2] = {0.0f};
	u32			disp_flag = 0;
	NNS_MATRIX	obj_mtx;
	VecFx32		scale_m = {FX32_ONE, FX32_ONE, FX32_ONE};

	if (p_disp_flag) {
		if (!(*p_disp_flag & OBD_DISP_STOP)) {
			// 停止中はENDフラグを落とさない
			*p_disp_flag &= ~(OBD_DISP_END | OBD_DISP_MAT_END);
		}

		disp_flag = *p_disp_flag;
	}

#if 01
	// 
	// TEST!
	// 
	if (obj_3d && obj_3d->object) {
		BOOL use_flag = FALSE;
#if OBD_OBJECT_USE_NOEXIST
		disp_flag &= ~OBD_DISP_NOEXIST_ENABLE;
#endif // OBD_OBJECT_USE_NOEXIST
		// 移動値が無い、またはユーザー行列を持つなら処理を行わない
		if (pos && !(disp_flag & (OBD_DISP_NOPOS | OBD_DISP_USERMTX | OBD_DISP_USERMTX_RIGHT | OBD_DISP_NOCLIP))) {
			OBS_CAMERA* obj_camera = ObjCameraGet(g_obj.glb_camera_id);
			
			// 画面が横に回転している都合上、計算に使う値も270度回転する
			float left   = obj_camera->disp_pos.x + obj_camera->bottom; // 大きい値で計算
			float right  = obj_camera->disp_pos.x + obj_camera->top; // 大きい値で計算
			//float top    = -(obj_camera->disp_pos.y + obj_camera->right);
			//float bottom = -(obj_camera->disp_pos.y + obj_camera->left);
			float top    = -(obj_camera->disp_pos.y + obj_camera->top);
			float bottom = -(obj_camera->disp_pos.y + obj_camera->bottom);
			
			float set_scale = 1.0f;
			if (!(disp_flag & OBD_DISP_NOSCALE)) {
				fx32 base_scale = scale->x;
				fx32 chk_scale = scale->y;
				if (base_scale < chk_scale) {
					base_scale = chk_scale;
				}
				chk_scale = scale->z;
				if (base_scale < chk_scale) {
					base_scale = chk_scale;
				}
				set_scale = FX_FX32_TO_F32(base_scale);
			}
			set_scale *= GMD_OBJ_DRAW_SCALE;
			
			// やや広めに判定するための x1.2
			float obj_left   = FX_FX32_TO_F32(pos->x) - (obj_3d->object->Radius * 1.2f * set_scale);
			float obj_right  = FX_FX32_TO_F32(pos->x) + (obj_3d->object->Radius * 1.2f * set_scale);
			float obj_top    = FX_FX32_TO_F32(pos->y) - (obj_3d->object->Radius * 1.2f * set_scale);
			float obj_bottom = FX_FX32_TO_F32(pos->y) + (obj_3d->object->Radius * 1.2f * set_scale);

			// 画面外に行ってそうなら描画しないフラグを立てる
			if (left > obj_right)  use_flag = TRUE;
			if (right < obj_left)  use_flag = TRUE;
			if (top > obj_bottom)  use_flag = TRUE;
			if (bottom < obj_top)  use_flag = TRUE;
		}
		if (use_flag) {
#if OBD_OBJECT_USE_NOEXIST
			disp_flag |= OBD_DISP_NODISP | OBD_DISP_NOEXIST_ENABLE;
#else
			disp_flag |= OBD_DISP_NODISP;
#endif // OBD_OBJECT_USE_NOEXIST
		}
	}
#if OBD_OBJECT_USE_NOEXIST
	if (disp_flag & OBD_DISP_NOEXIST) {
		if (p_disp_flag) {
			*p_disp_flag &= ~OBD_DISP_NOEXIST_ENABLE;
			*p_disp_flag |= disp_flag & (OBD_DISP_END | OBD_DISP_MAT_END | OBD_DISP_NOEXIST_ENABLE);
		}
		return;
	}
#endif // OBD_OBJECT_USE_NOEXIST
#if 0
	// 見えないならモーション系を更新だけして終了
	if (disp_flag  & OBD_DISP_NODISP) {
		if (obj_3d->motion && obj_3d->motion->mmobject) {
			ObjDrawAction3DNNMaterialUpdate(obj_3d, &disp_flag);
		}
		if (obj_3d->motion && obj_3d->motion->mtnbuf[0]) {
			ObjDrawAction3DNNMotionUpdate(obj_3d, &disp_flag);
			// モーションコールバック
			if (obj_3d->mtn_cb_func) {
				obj_3d->mtn_cb_func(obj_3d->motion, obj_3d->object, obj_3d->mtn_cb_param);
			}
		}
		obj_3d->user_param = NULL;
		obj_3d->mplt_cb_param	= NULL;
		obj_3d->material_cb_param = NULL;
		
		if (p_disp_flag) {
			*p_disp_flag |= disp_flag & (OBD_DISP_END | OBD_DISP_MAT_END);
		}
		return;
	}
#endif // 0
	// 
	// KOKOMADE!
	// 
#endif
	nnMakeUnitMatrix(&obj_mtx);

	// 移動値
	if (pos && !(disp_flag & OBD_DISP_NOPOS)) {
		VecFx32	pos_work = *pos;
		if (!(disp_flag & OBD_DISP_3D_COORDINATE)) {
			// 2D座標で位置指定
			pos_work.y = -pos_work.y;
		}

		// 拡大時位置計算
		if (!(disp_flag & OBD_DISP_NOGLBSCALE)) {
			if (g_obj.glb_scale.x != 0x1000) {
				pos_work.x = FX_Mul(pos_work.x, g_obj.glb_scale.x);
			}
			if (g_obj.glb_scale.y != 0x1000) {
				pos_work.y = FX_Mul(pos_work.y, g_obj.glb_scale.y);
			}
			if (g_obj.glb_scale.z != 0x1000) {
				pos_work.z = FX_Mul(pos_work.z, g_obj.glb_scale.z);
			}
		}

		nnTranslateMatrix(&obj_mtx, &obj_mtx,
						FX_FX32_TO_F32(pos_work.x), FX_FX32_TO_F32(pos_work.y), FX_FX32_TO_F32(pos_work.z));
	}

	// 回転
	if (dir && !(disp_flag & OBD_DISP_NODIR)) {
		if (!(disp_flag & OBD_DISP_3D_COORDINATE)) {
			nnRotateXYZMatrix(&obj_mtx, &obj_mtx, -dir->x, dir->y, -dir->z);
		}
		else {
			nnRotateXYZMatrix(&obj_mtx, &obj_mtx, dir->x, dir->y, dir->z);
		}
	}
	// 左右フリップ(回転)
	if (!(disp_flag & OBD_DISP_NODIRFLIP)) {
		if (!(disp_flag & OBD_DISP_DIR2DFLIP)) {
			if (disp_flag & OBD_DISP_HFLIP) {
				nnRotateYMatrix(&obj_mtx, &obj_mtx, 0xC000);
			}
			else {
				nnRotateYMatrix(&obj_mtx, &obj_mtx, 0x4000);
			}
		}
		else {
			if (disp_flag & OBD_DISP_VFLIP) {
				nnRotateXMatrix(&obj_mtx, &obj_mtx, 0x8000);
			}
			if (disp_flag & OBD_DISP_HFLIP) {
				nnRotateYMatrix(&obj_mtx, &obj_mtx, 0x8000);
			}
		//	else {
		//		nnRotateYMatrix(&obj_mtx, &obj_mtx, 0x0000);
		//	}
		}
	}
	// スケール
#if 1
	if (scale && !(disp_flag & OBD_DISP_NOSCALE)) {
		scale_m = *scale;
	}
	if (!(disp_flag & OBD_DISP_NODRAWSCALE)) {
		scale_m.x = FX_Mul(scale_m.x, g_obj.draw_scale.x);
		scale_m.y = FX_Mul(scale_m.y, g_obj.draw_scale.y);
		scale_m.z = FX_Mul(scale_m.z, g_obj.draw_scale.z);
	}
	if (!(disp_flag & OBD_DISP_NOGLBSCALE)) {
		scale_m.x = FX_Mul(scale_m.x, g_obj.glb_scale.x);
		scale_m.y = FX_Mul(scale_m.y, g_obj.glb_scale.y);
		scale_m.z = FX_Mul(scale_m.z, g_obj.glb_scale.z);

		// 拡大時位置計算
		// trans部で反映すみ
	}
	nnScaleMatrix(&obj_mtx, &obj_mtx,
					FX_FX32_TO_F32(scale_m.x), FX_FX32_TO_F32(scale_m.y), FX_FX32_TO_F32(scale_m.z));
#else
	if (scale && !(disp_flag & OBD_DISP_NOSCALE)) {
		nnScaleMatrix(&obj_mtx, &obj_mtx,
						FX_FX32_TO_F32(scale->x), FX_FX32_TO_F32(scale->y), FX_FX32_TO_F32(scale->z));
	}
#endif

	// ユーザー行列
	if (disp_flag & OBD_DISP_USERMTX) {
		nnMultiplyMatrix(&obj_mtx, &obj_3d->user_obj_mtx, &obj_mtx);
	}

	// ユーザー行列（右から乗算）
	if (disp_flag & OBD_DISP_USERMTX_RIGHT) {
		nnMultiplyMatrix(&obj_mtx, &obj_mtx, &obj_3d->user_obj_mtx_r);
	}

	//current_mtx = amMatrixGetCurrent();
	//nnMultiplyMatrix(&obj_mtx, &obj_mtx, current_mtx);
	amMatrixPush(&obj_mtx);

	// マテリアルモーションアップデート
	if (obj_3d->motion && obj_3d->motion->mmobject) {
		ObjDrawAction3DNNMaterialUpdate(obj_3d, &disp_flag);
	}

	if (obj_3d->motion && obj_3d->motion->mtnbuf[0]) {

		// モーションアップデート
		ObjDrawAction3DNNMotionUpdate(obj_3d, &disp_flag);

		// 描画発行
	//	amMotionDraw(OBD_DRAW_CMD_STATE_3DNN, motion, object, texlist,
	//						TEST_DRAW_OBJFLAG);

		// モーションコールバック
		if (obj_3d->mtn_cb_func) {
			obj_3d->mtn_cb_func(obj_3d->motion, obj_3d->object, obj_3d->mtn_cb_param);
		}

		if (!(disp_flag & OBD_DISP_NODISP)) {
			AMS_DRAWSTATE	*draw_state = NULL;
			if (disp_flag & OBD_DISP_DRAWSTATE) {
				draw_state = &obj_3d->draw_state;
			}

#if 1
			if (obj_3d->marge == 0.f || obj_3d->marge == 1.f) {
				NNS_MOTION	*motion;
				float		frame;
				s32			act_id;

				if (obj_3d->marge == 0.f) {
					act_id = obj_3d->act_id[0];
					frame = obj_3d->frame[0];
				}
				else {
					act_id = obj_3d->act_id[1];
					frame = obj_3d->frame[1];
				}
				motion	= obj_3d->motion->mtnbuf[act_id & 0xFFFF];
				frame	+= amMotionGetStartFrame(obj_3d->motion, act_id);

				if (obj_3d->motion && obj_3d->motion->mmobject) {
					//ObjDraw3DNNDrawMotionMaterialMotion(motion, frame, obj_3d->object, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
					//			obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
					//			obj_3d->mplt_cb_func, obj_3d->mplt_cb_param,
					//			obj_3d->material_cb_func, obj_3d->material_cb_param,
//#if _PS3 | _XBOX | _PC
//								draw_state, obj_3d->use_light_flag, obj_3d->toon_rim_param);
//#else
//								draw_state, obj_3d->use_light_flag);
//#endif
					// 暫定
					ObjDraw3DNNMotionMaterialMotion(obj_3d->motion, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
								obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
								obj_3d->mplt_cb_func, obj_3d->mplt_cb_param,
								obj_3d->material_cb_func, obj_3d->material_cb_param,
#if _PS3 | _XBOX | _PC
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_rim_param, obj_3d->toon_camouflage);
#elif _WII
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_light);
#else
								draw_state, obj_3d->use_light_flag);
#endif
				}
				else {
					ObjDraw3DNNDrawMotion(motion, frame, obj_3d->object, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
								obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
								obj_3d->mplt_cb_func, obj_3d->mplt_cb_param,
								obj_3d->material_cb_func, obj_3d->material_cb_param,
#if _PS3 | _XBOX | _PC
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_rim_param, obj_3d->toon_camouflage);
#elif _WII
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_light);
#else
								draw_state, obj_3d->use_light_flag);
#endif
				}
			}
			else {
				if (obj_3d->motion && obj_3d->motion->mmobject) {
					ObjDraw3DNNMotionMaterialMotion(obj_3d->motion, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
								obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
								obj_3d->mplt_cb_func, obj_3d->mplt_cb_param,
								obj_3d->material_cb_func, obj_3d->material_cb_param,
#if _PS3 | _XBOX | _PC
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_rim_param, obj_3d->toon_camouflage);
#elif _WII
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_light);
#else
								draw_state, obj_3d->use_light_flag);
#endif
				}
				else {
					ObjDraw3DNNMotion(obj_3d->motion, obj_3d->motion->object, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
								obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
								obj_3d->mplt_cb_func, obj_3d->mplt_cb_param,
								obj_3d->material_cb_func, obj_3d->material_cb_param,
#if _PS3 | _XBOX | _PC
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_rim_param, obj_3d->toon_camouflage);
#elif _WII
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_light);
#else
								draw_state, obj_3d->use_light_flag);
#endif
				}
			}


#else
			if (obj_3d->motion && obj_3d->motion->mmobject) {
				if (obj_3d->marge == 0.f || obj_3d->marge == 1.f) {
					NNS_MOTION	*motion;
					float		frame;
					s32			act_id;

					if (obj_3d->marge == 0.f) {
						act_id = obj_3d->act_id[0];
						frame = obj_3d->frame[0];
					}
					else {
						act_id = obj_3d->act_id[1];
						frame = obj_3d->frame[1];
					}
					motion	= obj_3d->motion->mtnbuf[act_id & 0xFFFF];
					frame	+= amMotionGetStartFrame(obj_3d->motion, act_id);

					ObjDraw3DNNDrawMotionMaterialMotion(motion, frame, obj_3d->object, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
								obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
								obj_3d->mplt_cb_func, obj_3d->mplt_cb_param,
								obj_3d->material_cb_func, obj_3d->material_cb_param,
#if _PS3 | _XBOX | _PC
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_rim_param, &obj_3d->toon_camouflage);
#elif _WII
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_light);
#else
								draw_state, obj_3d->use_light_flag);
#endif
				}
				else {
					ObjDraw3DNNMotionMaterialMotion(obj_3d->motion, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
								obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
								obj_3d->mplt_cb_func, obj_3d->mplt_cb_param,
								obj_3d->material_cb_func, obj_3d->material_cb_param,
#if _PS3 | _XBOX | _PC
								draw_state, obj_3d->use_light_flag, obj_3d->toon_rim_param, obj_3d->toon_camouflage);
#elif _WII
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_light);
#else
								draw_state, obj_3d->use_light_flag);
#endif
				}
			}
			else {
				if (obj_3d->marge == 0.f || obj_3d->marge == 1.f) {
					NNS_MOTION	*motion;
					float		frame;
					s32			act_id;

					if (obj_3d->marge == 0.f) {
						act_id = obj_3d->act_id[0];
						frame = obj_3d->frame[0];
					}
					else {
						act_id = obj_3d->act_id[1];
						frame = obj_3d->frame[1];
					}
					motion	= obj_3d->motion->mtnbuf[act_id & 0xFFFF];
					frame	+= amMotionGetStartFrame(obj_3d->motion, act_id);

					ObjDraw3DNNDrawMotion(motion, frame, obj_3d->object, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
								obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
								obj_3d->mplt_cb_func, obj_3d->mplt_cb_param,
								obj_3d->material_cb_func, obj_3d->material_cb_param,
#if _PS3 | _XBOX | _PC
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_rim_param, &obj_3d->toon_camouflage);
#elif _WII
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_light);
#else
								draw_state, obj_3d->use_light_flag);
#endif
				}
				else {
					ObjDraw3DNNMotion(obj_3d->motion, obj_3d->object, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
								obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
								obj_3d->mplt_cb_func, obj_3d->mplt_cb_param,
								obj_3d->material_cb_func, obj_3d->material_cb_param,
#if _PS3 | _XBOX | _PC
								draw_state, obj_3d->use_light_flag, obj_3d->toon_rim_param, obj_3d->toon_camouflage);
#elif _WII
								draw_state, obj_3d->use_light_flag, &obj_3d->toon_light);
#else
								draw_state, obj_3d->use_light_flag);
#endif
				}
			}
#endif
		}

		obj_3d->user_param = NULL;
		obj_3d->mplt_cb_param	= NULL;
		obj_3d->material_cb_param = NULL;
	}
	else {

		// 描画発行
	//	amDrawObject(OBD_DRAW_CMD_STATE_3DNN, object, texlist,
	//					TEST_DRAW_OBJFLAG);

		if (!(disp_flag & OBD_DISP_NODISP)) {
			AMS_DRAWSTATE	*draw_state = NULL;
			if (disp_flag & OBD_DISP_DRAWSTATE) {
				draw_state = &obj_3d->draw_state;
			}

			if (obj_3d->motion && obj_3d->motion->mmobject) {
				//ObjDraw3DNNModelMaterialMotion(obj_3d->object, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
				//			obj_3d->user_func, obj_3d->user_param, obj_3d->command_state, draw_state, obj_3d->use_light_flag);
				// ◆暫定
				ObjDraw3DNNMotionMaterialMotion(obj_3d->motion, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
							obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
							NULL, NULL,
							obj_3d->material_cb_func, obj_3d->material_cb_param,
#if _PS3 | _XBOX | _PC
							draw_state, obj_3d->use_light_flag, &obj_3d->toon_rim_param, obj_3d->toon_camouflage);
#elif _WII
							draw_state, obj_3d->use_light_flag, &obj_3d->toon_light);
#else
							draw_state, obj_3d->use_light_flag);
#endif
			}
			else {
				ObjDraw3DNNModel(obj_3d->object, obj_3d->texlist, obj_3d->drawflag, obj_3d->sub_obj_type,
							obj_3d->user_func, obj_3d->user_param, obj_3d->command_state,
							obj_3d->material_cb_func, obj_3d->material_cb_param,
#if _PS3 | _XBOX | _PC
							draw_state, obj_3d->use_light_flag, &obj_3d->toon_rim_param, obj_3d->toon_camouflage);
#elif _WII
							draw_state, obj_3d->use_light_flag, &obj_3d->toon_light);
#else
							draw_state, obj_3d->use_light_flag);
#endif
			}
		}
		obj_3d->user_param = NULL;
		obj_3d->mplt_cb_param	= NULL;
		obj_3d->material_cb_param = NULL;
	}

	amMatrixPop();

	if (p_disp_flag) {
#if OBD_OBJECT_USE_NOEXIST
		*p_disp_flag &= ~OBD_DISP_NOEXIST_ENABLE;
		*p_disp_flag |= disp_flag & (OBD_DISP_END | OBD_DISP_MAT_END | OBD_DISP_NOEXIST_ENABLE);
#else
		*p_disp_flag |= disp_flag & (OBD_DISP_END | OBD_DISP_MAT_END);
#endif // OBD_OBJECT_USE_NOEXIST;
	}
}

// ==========================================================================
// ObjDrawAction3DNNMotionUpdate
/*!
 *	オブジェクトアクション 3D NN モーション更新
 *
 *	@param obj_3d		[in]	3DNN オブジェクトワーク
 *	@param p_disp_flag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 */
// ==========================================================================
void ObjDrawAction3DNNMotionUpdate(OBS_ACTION3D_NN_WORK *obj_3d, u32 *p_disp_flag)
{
	s32		i;
	u32		disp_flag = 0;
	float	spd[2] = {0.0f, 0.0f};

	MTM_ASSERT(obj_3d->motion && obj_3d->motion->mtnbuf[0]);

	if (p_disp_flag) {
		disp_flag = *p_disp_flag;
	}

	if (!(disp_flag & OBD_DISP_NOUPDATE)) {
		if (!(disp_flag & OBD_DISP_STOP)) {
			// モーションブレンド
			if (obj_3d->flag & OBD_ACTFLAG_3D_NN_BLEND) {
				obj_3d->marge -= obj_3d->blend_spd;

				if (obj_3d->marge <= 0.0f) {
					// ブレンド終了
					obj_3d->marge = 0.0f;
					obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_BLEND;
				}
			}

			// 再生速度取得
			if (!(disp_flag & (OBD_DISP_STOP | OBD_DISP_NOUPDATE))) {
				spd[0] = obj_3d->speed[0] * FX_FX32_TO_F32(g_obj.speed);
				spd[1] = obj_3d->speed[1] * FX_FX32_TO_F32(g_obj.speed);
			}

			// アニメーションをすすめる
			if (obj_3d->marge < 1.0f || obj_3d->flag & (OBD_ACTFLAG_3D_NN_BG_ANM | OBD_ACTFLAG_3D_NN_BLEND)) {
				obj_3d->frame[0] += spd[0];
			}
			if (!(obj_3d->flag & OBD_ACTFLAG_3D_NN_BLEND) &&	// モーションブレンド中は進めない
					(obj_3d->marge > 0.0f || obj_3d->flag & (OBD_ACTFLAG_3D_NN_BG_ANM | OBD_ACTFLAG_3D_NN_BLEND))) {
				obj_3d->frame[1] += spd[1];
			}

			// ループチェック
			if (disp_flag & OBD_DISP_REPEAT) {
				float	frame_max;
				for (i = 0; i < OBD_ACTION3D_NN_MTN_BUF_NUM; i++) {
					frame_max = amMotionGetEndFrame(obj_3d->motion, obj_3d->act_id[i]) -
										amMotionGetStartFrame(obj_3d->motion, obj_3d->act_id[i]);

					while (obj_3d->frame[i] >= frame_max) {
						obj_3d->frame[i] = obj_3d->frame[i] - frame_max;
						if (i == 0) {
							disp_flag |= OBD_DISP_END;
						}
					}
				}
			}
			else {
				float	frame_max;
				for (i = 0; i < OBD_ACTION3D_NN_MTN_BUF_NUM; i++) {
					frame_max = amMotionGetEndFrame(obj_3d->motion, obj_3d->act_id[i]) -
										amMotionGetStartFrame(obj_3d->motion, obj_3d->act_id[i]);

					if (obj_3d->frame[i] >= frame_max - 1.f) {
						obj_3d->frame[i] = frame_max - 1.f;
						if (i == 0) {
							disp_flag |= OBD_DISP_END;
						}
					}
				}
			}
		} // !(disp_flag & OBD_DISP_STOP)

		// フレーム設定
		if (obj_3d->marge < 1.0f || obj_3d->flag & (OBD_ACTFLAG_3D_NN_BG_ANM | OBD_ACTFLAG_3D_NN_BLEND)) {
			amMotionSetFrame(obj_3d->motion, 0, obj_3d->frame[0] + amMotionGetStartFrame(obj_3d->motion, obj_3d->act_id[0]));
		}
		if (obj_3d->marge > 0.0f || obj_3d->flag & (OBD_ACTFLAG_3D_NN_BG_ANM | OBD_ACTFLAG_3D_NN_BLEND)) {
			amMotionSetFrame(obj_3d->motion, 1, obj_3d->frame[1] + amMotionGetStartFrame(obj_3d->motion, obj_3d->act_id[1]));
		}

		// モーション計算
		amMotionGet(obj_3d->motion, obj_3d->marge, obj_3d->per);
	} // !(disp_flag & OBD_DISP_NOUPDATE)

	if (p_disp_flag) {
		*p_disp_flag |= disp_flag & OBD_DISP_END;
	}
}

// ==========================================================================
// ObjDrawAction3DNNMaterialUpdate
/*!
 *	オブジェクトアクション 3D NN マテリアルモーション更新
 *
 *	@param obj_3d		[in]	3DNN オブジェクトワーク
 *	@param p_disp_flag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 */
// ==========================================================================
void ObjDrawAction3DNNMaterialUpdate(OBS_ACTION3D_NN_WORK *obj_3d, u32 *p_disp_flag)
{
	u32		disp_flag = 0;
	float	spd = 0.0f;

	MTM_ASSERT(obj_3d->motion && obj_3d->motion->mmobject);

	if (p_disp_flag) {
		disp_flag = *p_disp_flag;
	}

	if (!(disp_flag & OBD_DISP_NOUPDATE)) {
		if (!(disp_flag & OBD_DISP_STOP)) {
			// 再生速度取得
			if (!(disp_flag & (OBD_DISP_STOP | OBD_DISP_NOUPDATE))) {
				spd = obj_3d->mat_speed * FX_FX32_TO_F32(g_obj.speed);
			}

			// アニメーションをすすめる
			obj_3d->mat_frame += spd;

			// ループチェック
			if (disp_flag & OBD_DISP_REPEAT) {
				float	frame_max;
				frame_max = amMotionMaterialGetEndFrame(obj_3d->motion, obj_3d->mat_act_id) -
									amMotionMaterialGetStartFrame(obj_3d->motion, obj_3d->mat_act_id);

				while (obj_3d->mat_frame >= frame_max) {
					obj_3d->mat_frame = obj_3d->mat_frame - frame_max;
					disp_flag |= OBD_DISP_MAT_END;
				}
			}
			else {
				float	frame_max;
				frame_max = amMotionMaterialGetEndFrame(obj_3d->motion, obj_3d->mat_act_id) -
									amMotionMaterialGetStartFrame(obj_3d->motion, obj_3d->mat_act_id);
				if (obj_3d->mat_frame >= frame_max - 1.f) {
					obj_3d->mat_frame = frame_max - 1.f;
					disp_flag |= OBD_DISP_MAT_END;
				}
			}
		} // !(disp_flag & OBD_DISP_STOP)

		// フレーム設定
		amMotionMaterialSetFrame(obj_3d->motion, obj_3d->mat_frame + amMotionMaterialGetStartFrame(obj_3d->motion, obj_3d->mat_act_id));

		// モーション計算
		amMotionMaterialCalc(obj_3d->motion);
	} // !(disp_flag & OBD_DISP_NOUPDATE)

	if (p_disp_flag) {
		*p_disp_flag |= disp_flag & OBD_DISP_MAT_END;
	}
}

// ==========================================================================
// ObjDraw3DNNModel
/*!
 *	オブジェクトアクション 描画
 *
 *	@param object				[in]	オブジェクト
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。\n
 *		user_param は、ObjDraw3DNNModel呼び出し前にamDrawMallocDataBufferで取得して下さい。
 *		amDrawMallocDataBufferでのバッファ取得は、描画毎に再取得が必要です。
 */
// ==========================================================================
void ObjDraw3DNNModel(NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param,
				u32 command_state,
				OBF_MATERIAL_CB material_cb_func/*=NULL*/, void *material_cb_param/*=NULL*/,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_RGB *toon_rim_param/*=NULL*/, float toon_camouflage/*=0.0f*/)
#elif _WII
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_VECTOR *toon_light/*=NULL*/)
#else
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/)
#endif
{
	// ◆ amDrawObjectのコピー改変
	OBS_DRAW_PARAM_3DNN_MODEL	*draw_param;
	AMS_PARAM_DRAW_OBJECT		*param;
	NNS_MATRIX					*mtx;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}

	draw_param = (OBS_DRAW_PARAM_3DNN_MODEL *)amDrawMallocDataBuffer(sizeof(OBS_DRAW_PARAM_3DNN_MODEL));

	mtx = &draw_param->mtx;
	nnCopyMatrix(mtx, amMatrixGetCurrent());		// ◆とりあえずカレントマトリクス

	param =&draw_param->param;
	param->object		= object;
	param->mtx			= mtx;
	param->sub_obj_type	= sub_obj_type;
	param->flag			= drawflag;
	param->texlist		= texlist;
	//param->material_func	= func;
	param->material_func= NULL;		// objDrawのコールバックを使用
	param->scaleZ		= 1.0f;

	// 追加
	draw_param->state = NULL;
	if (draw_state) {
		draw_param->state = &draw_param->draw_state;
		MI_CpuCopy8(draw_state, &draw_param->draw_state, sizeof(AMS_DRAWSTATE));
	}

	// ライト
	draw_param->use_light_flag = use_light_flag;

#if _PS3 | _XBOX | _PC
	// トゥーンリムライト
	if (toon_rim_param) {
		draw_param->toon_rim_param = *toon_rim_param;
	}
	else {
		draw_param->toon_rim_param = g_obj.toon_rim_param;
	}
	// 迷彩
	draw_param->toon_camouflage = toon_camouflage;
#elif _WII
	// トゥーンライト
	if (toon_light) {
		draw_param->toon_light = *toon_light;
	}
	else {
		draw_param->toon_light = g_obj.toon_light_vec;
	}
#endif

	// ユーザー処理
	draw_param->user_func	= user_func;
	draw_param->user_param	= user_param;

	// マテリアルコールバック
	draw_param->material_cb_func	= material_cb_func;
	draw_param->material_cb_param	= material_cb_param;

	// 描画コマンド発行
	amDrawRegistCommand(command_state, OBD_DRAW_USER_COMMAND_3DNN_MODEL, param);
}

// ==========================================================================
// ObjDraw3DNNModelMaterialMotion
/*!
 *	オブジェクトアクション マテリアルモーションつき 描画
 *
 *	@param object				[in]	オブジェクト
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。\n
 *		user_param は、ObjDraw3DNNModel呼び出し前にamDrawMallocDataBufferで取得して下さい。
 *		amDrawMallocDataBufferでのバッファ取得は、描画毎に再取得が必要です。
 */
// ==========================================================================
void ObjDraw3DNNModelMaterialMotion(NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param,
				u32 command_state,
				OBF_MATERIAL_CB material_cb_func/*=NULL*/, void *material_cb_param/*=NULL*/,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_RGB *toon_rim_param/*=NULL*/, float toon_camouflage/*=0.0f*/)
#elif _WII
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_VECTOR *toon_light/*=NULL*/)
#else
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/)
#endif
{
	// ◆ amDrawObjectMaterialMotionのコピー改変
	OBS_DRAW_PARAM_3DNN_MODEL	*draw_param;
	AMS_PARAM_DRAW_OBJECT		*param;
	NNS_MATRIX					*mtx;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	amThreadCheckSafe(0, "ObjDraw3DNNModelMaterialMotion");

	draw_param = (OBS_DRAW_PARAM_3DNN_MODEL *)amDrawMallocDataBuffer(sizeof(OBS_DRAW_PARAM_3DNN_MODEL));

	mtx = &draw_param->mtx;
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param =&draw_param->param;
	param->object		= object;
	param->mtx			= mtx;
	param->sub_obj_type	= sub_obj_type;
	param->flag			= drawflag;
	param->texlist		= texlist;
	//param->material_func	= func;
	param->material_func= NULL;		// objDrawのコールバックを使用
	param->scaleZ		= 1.0f;

	// 追加
	draw_param->state = NULL;
	if (draw_state) {
		draw_param->state = &draw_param->draw_state;
		MI_CpuCopy8(draw_state, &draw_param->draw_state, sizeof(AMS_DRAWSTATE));
	}

	// ライト
	draw_param->use_light_flag = use_light_flag;

#if _PS3 | _XBOX | _PC
	// トゥーンリムライト
	if (toon_rim_param) {
		draw_param->toon_rim_param = *toon_rim_param;
	}
	else {
		draw_param->toon_rim_param = g_obj.toon_rim_param;
	}
	// 迷彩
	draw_param->toon_camouflage = toon_camouflage;
#elif _WII
	// トゥーンライト
	if (toon_light) {
		draw_param->toon_light = *toon_light;
	}
	else {
		draw_param->toon_light = g_obj.toon_light_vec;
	}
#endif

	// ユーザー処理
	draw_param->user_func	= user_func;
	draw_param->user_param	= user_param;
	// マテリアルコールバック
	draw_param->material_cb_func	= material_cb_func;
	draw_param->material_cb_param	= material_cb_param;

	// 描画コマンド発行
	amDrawRegistCommand(command_state, OBD_DRAW_USER_COMMAND_3DNN_MODEL_MATMTN, param);
}


// ==========================================================================
// ObjDraw3DNNMotion
/*!
 *	オブジェクト3Dモーション 描画
 *
 *	@param motion				[in]	モーション
 *	@param object				[in]	オブジェクト
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param mplt_cb_func			[in]	マトリックスパレットCB関数
 *	@param mplt_cb_param		[in]	マトリックスパレットCBパラメータ(amDrawMallocDataBuffer)
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。
 */
// ==========================================================================
void ObjDraw3DNNMotion(AMS_MOTION *motion, NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param, u32 command_state,
				OBF_DRAW_3DNN_MPLT_CB_FUNC mplt_cb_func/*=NULL*/,
				void *mplt_cb_param/*=NULL*/,
				OBF_MATERIAL_CB material_cb_func/*=NULL*/, void *material_cb_param/*=NULL*/,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_RGB *toon_rim_param/*=NULL*/, float toon_camouflage/*=0.0f*/)
#elif _WII
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_VECTOR *toon_light/*=NULL*/)
#else
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/)
#endif
{
	// ◆ amMotionDrawのコピー改変
	OBS_DRAW_PARAM_3DNN_MOTION	*draw_param;
	AMS_PARAM_DRAW_MOTION_TRS	*param;
	NNS_MATRIX					*mtx;
	s32							num;
	s32							motion_id;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	num		= motion->node_num;

	draw_param = (OBS_DRAW_PARAM_3DNN_MOTION *)amDrawMallocDataBuffer((s32)(sizeof(OBS_DRAW_PARAM_3DNN_MOTION)
										+ sizeof(NNS_TRS) * num));

	mtx = &draw_param->mtx;
	nnCopyMatrix(mtx, amMatrixGetCurrent());		// ◆とりあえずカレントマトリクス

	param =&draw_param->param;
	param->object		= object;
	param->mtx			= mtx;
	param->sub_obj_type	= sub_obj_type;
	param->flag			= drawflag;
	param->texlist		= texlist;
	param->trslist	= (NNS_TRS *)(draw_param + 1);
	//param->material_func	= func;
	param->material_func= NULL;		// objDrawのコールバックを使用
	memcpy(param->trslist, motion->data, sizeof(NNS_TRS) * num);

	motion_id			= motion->mbuf[0].motion_id;
	param->motion		= motion->mtnfile[motion_id >> 16].motion[motion_id & 0xffff];
	param->frame		= motion->mbuf[0].frame;

//	param->mmotion	= NULL;
//	param->mframe	= 0.f;

	// 追加
	draw_param->state = NULL;
	if (draw_state) {
		draw_param->state = &draw_param->draw_state;
		MI_CpuCopy8(draw_state, &draw_param->draw_state, sizeof(AMS_DRAWSTATE));
	}

	// ライト
	draw_param->use_light_flag = use_light_flag;

#if _PS3 | _XBOX | _PC
	// トゥーンリムライト
	if (toon_rim_param) {
		draw_param->toon_rim_param = *toon_rim_param;
	}
	else {
		draw_param->toon_rim_param = g_obj.toon_rim_param;
	}
	// 迷彩
	draw_param->toon_camouflage = toon_camouflage;
#elif _WII
	// トゥーンライト
	if (toon_light) {
		draw_param->toon_light = *toon_light;
	}
	else {
		draw_param->toon_light = g_obj.toon_light_vec;
	}
#endif

	// ユーザー処理
	draw_param->user_func	= user_func;
	draw_param->user_param	= user_param;
	
	// マトリックスパレットコールバック
	draw_param->mplt_cb_func	= mplt_cb_func;
	draw_param->mplt_cb_param	= mplt_cb_param;

	// マテリアルコールバック
	draw_param->material_cb_func	= material_cb_func;
	draw_param->material_cb_param	= material_cb_param;

	// 描画コマンド発行
	amDrawRegistCommand(command_state, OBD_DRAW_USER_COMMAND_3DNN_MOTION, param);
}

// ==========================================================================
// ObjDraw3DNNMotionMaterialMotion
/*!
 *	オブジェクト3Dモーション マテリアルモーションつき 描画
 *
 *	@param motion				[in]	モーション
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param mplt_cb_func			[in]	マトリックスパレットCB関数
 *	@param mplt_cb_param		[in]	マトリックスパレットCBパラメータ(amDrawMallocDataBuffer)
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。
 */
// ==========================================================================
void ObjDraw3DNNMotionMaterialMotion(AMS_MOTION *motion, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param, u32 command_state,
				OBF_DRAW_3DNN_MPLT_CB_FUNC mplt_cb_func/*=NULL*/,
				void *mplt_cb_param/*=NULL*/,
				OBF_MATERIAL_CB material_cb_func/*=NULL*/, void *material_cb_param/*=NULL*/,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_RGB *toon_rim_param/*=NULL*/, float toon_camouflage/*=0.0f*/)
#elif _WII
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_VECTOR *toon_light/*=NULL*/)
#else
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/)
#endif
{
	// ◆ amMotionMaterialDrawのコピー改変
	OBS_DRAW_PARAM_3DNN_MOTION	*draw_param;
	AMS_PARAM_DRAW_MOTION_TRS	*param;
	NNS_MATRIX					*mtx;
	s32							num;
	s32							motion_id;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	if (motion->mmobject == NULL) {
		ObjDraw3DNNMotion(motion, motion->object, texlist, drawflag, sub_obj_type,
				user_func, user_param, command_state,
				mplt_cb_func, mplt_cb_param,
				material_cb_func, material_cb_param,
#if _PS3 | _XBOX | _PC
				draw_state, use_light_flag, toon_rim_param, toon_camouflage);
#elif _WII
				draw_state, use_light_flag, toon_light);
#else
				draw_state, use_light_flag);
#endif
		return;
	}

	num		= motion->node_num;

	draw_param = (OBS_DRAW_PARAM_3DNN_MOTION *)amDrawMallocDataBuffer((s32)(sizeof(OBS_DRAW_PARAM_3DNN_MOTION)
										+ sizeof(NNS_TRS) * num + motion->mmobj_size));

	mtx = &draw_param->mtx;
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param =&draw_param->param;
	param->mtx			= mtx;
	param->sub_obj_type	= sub_obj_type;
	param->flag			= drawflag;
	param->texlist		= texlist;
	param->trslist		= (NNS_TRS *)(draw_param + 1);
	//param->material_func	= func;
	param->material_func= NULL;		// objDrawのコールバックを使用
	memcpy(param->trslist, motion->data, sizeof(NNS_TRS) * num);

//	param->object	= (NNS_OBJECT *)(param->trslist + num);
//	memcpy(param->object, motion->mmobject, motion->mmobj_size);
	param->object	= motion->object;
	param->mmotion	= motion->mmtn[motion->mmotion_id];
	param->mframe	= motion->mmotion_frame;

	motion_id			= motion->mbuf[0].motion_id;
	if (motion->mtnfile[motion_id >> 16].file != NULL) {
		param->motion		= motion->mtnfile[motion_id >> 16].motion[motion_id & 0xffff];
		param->frame		= motion->mbuf[0].frame;
	} else {
		param->motion		= NULL;
		param->frame		= 0.0f;
	}

	// 追加
	draw_param->state = NULL;
	if (draw_state) {
		draw_param->state = &draw_param->draw_state;
		MI_CpuCopy8(draw_state, &draw_param->draw_state, sizeof(AMS_DRAWSTATE));
	}

	// ライト
	draw_param->use_light_flag = use_light_flag;

#if _PS3 | _XBOX | _PC
	// トゥーンリムライト
	if (toon_rim_param) {
		draw_param->toon_rim_param = *toon_rim_param;
	}
	else {
		draw_param->toon_rim_param = g_obj.toon_rim_param;
	}
	// 迷彩
	draw_param->toon_camouflage = toon_camouflage;
#elif _WII
	// トゥーンライト
	if (toon_light) {
		draw_param->toon_light = *toon_light;
	}
	else {
		draw_param->toon_light = g_obj.toon_light_vec;
	}
#endif

	// ユーザー処理
	draw_param->user_func	= user_func;
	draw_param->user_param	= user_param;
	
	// マトリックスパレットコールバック
	draw_param->mplt_cb_func	= mplt_cb_func;
	draw_param->mplt_cb_param	= mplt_cb_param;

	// マテリアルコールバック
	draw_param->material_cb_func	= material_cb_func;
	draw_param->material_cb_param	= material_cb_param;

	// 描画コマンド発行
	amDrawRegistCommand(command_state, OBD_DRAW_USER_COMMAND_3DNN_MOTION_MATMTN, param);
}

// ==========================================================================
// ObjDraw3DNNMotion
/*!
 *	オブジェクト3Dモーション 描画
 *
 *	@param motion				[in]	モーション NNS_MOTION
 *	@param frame				[in]	モーションフレーム
 *	@param object				[in]	オブジェクト
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param mplt_cb_func			[in]	マトリックスパレットCB関数
 *	@param mplt_cb_param		[in]	マトリックスパレットCBパラメータ(amDrawMallocDataBuffer)
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。
 */
// ==========================================================================
void ObjDraw3DNNDrawMotion(NNS_MOTION *motion, float frame, NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param, u32 command_state,
				OBF_DRAW_3DNN_MPLT_CB_FUNC mplt_cb_func/*=NULL*/,
				void *mplt_cb_param/*=NULL*/,
				OBF_MATERIAL_CB material_cb_func/*=NULL*/, void *material_cb_param/*=NULL*/,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_RGB *toon_rim_param/*=NULL*/, float toon_camouflage/*=0.0f*/)
#elif _WII
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_VECTOR *toon_light/*=NULL*/)
#else
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/)
#endif
{
	// ◆ amDrawMotionのコピー改変

	OBS_DRAW_PARAM_3DNN_DRAW_MOTION	*draw_param;
	AMS_PARAM_DRAW_MOTION			*param;
	NNS_MATRIX						*mtx;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	amThreadCheckSafe(0, "ObjDraw3DNNDrawMotion");

	draw_param	= (OBS_DRAW_PARAM_3DNN_DRAW_MOTION *)amDrawMallocDataBuffer(sizeof(OBS_DRAW_PARAM_3DNN_DRAW_MOTION));

	mtx = &draw_param->mtx;
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param =&draw_param->param;
	param->object	= object;
	param->mtx		= mtx;
	param->sub_obj_type	= sub_obj_type;
	param->flag		= drawflag;
	param->texlist	= texlist;
	param->motion	= motion;
	param->frame	= frame;
	//param->material_func	= func;
	param->material_func= NULL;		// objDrawのコールバックを使用

	// 追加
	draw_param->state = NULL;
	if (draw_state) {
		draw_param->state = &draw_param->draw_state;
		MI_CpuCopy8(draw_state, &draw_param->draw_state, sizeof(AMS_DRAWSTATE));
	}

	// ライト
	draw_param->use_light_flag = use_light_flag;

#if _PS3 | _XBOX | _PC
	// トゥーンリムライト
	if (toon_rim_param) {
		draw_param->toon_rim_param = *toon_rim_param;
	}
	else {
		draw_param->toon_rim_param = g_obj.toon_rim_param;
	}
	// 迷彩
	draw_param->toon_camouflage = toon_camouflage;
#elif _WII
	// トゥーンライト
	if (toon_light) {
		draw_param->toon_light = *toon_light;
	}
	else {
		draw_param->toon_light = g_obj.toon_light_vec;
	}
#endif

	// ユーザー処理
	draw_param->user_func	= user_func;
	draw_param->user_param	= user_param;
	
	// マトリックスパレットコールバック
	draw_param->mplt_cb_func	= mplt_cb_func;
	draw_param->mplt_cb_param	= mplt_cb_param;

	// マテリアルコールバック
	draw_param->material_cb_func	= material_cb_func;
	draw_param->material_cb_param	= material_cb_param;

	// 描画コマンド発行
	amDrawRegistCommand(command_state, OBD_DRAW_USER_COMMAND_3DNN_DRAW_MOTION, param);
}

// ==========================================================================
// ObjDraw3DNNDrawMotionMaterialMotion
/*!
 *	オブジェクト3Dモーション マテリアルモーションつき 描画
 *
 *	@param motion				[in]	モーション NNS_MOTION
 *	@param frame				[in]	モーションフレーム
 *	@param object				[in]	オブジェクト
 *	@param texlist				[in]	テクスチャリスト
 *	@param drawflag				[in]	オブジェクト描画フラグ
 *	@param sub_obj_type			[in]	サブオブジェクトタイプ
 *	@param user_func			[in]	ユーザー処理関数
 *	@param user_param			[in]	ユーザーパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param command_state		[in]	描画コマンドステート
 *	@param mplt_cb_func			[in]	マトリックスパレットCB関数
 *	@param mplt_cb_param		[in]	マトリックスパレットCBパラメータ(amDrawMallocDataBuffer)
 *	@param material_cb_func		[in]	マテリアルコールバック関数
 *	@param material_cb_param	[in]	マテリアルコールバックパラメーターバッファアドレス(amDrawMallocDataBuffer)
 *	@param draw_state			[in]	描画時設定ステータス
 *	@param toon_rim_param		[in]	リムライト設定
 *	@param toon_camouflage		[in]	迷彩設定
 *
 *	@note
 *		描画スレッドに描画処理を登録します。
 */
// ==========================================================================
void ObjDraw3DNNDrawMotionMaterialMotion(NNS_MOTION *motion, float frame, NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNF_SUBOBJTYPE sub_obj_type,
				OBF_DRAW_USER_FUNC user_func, void *user_param, u32 command_state,
				OBF_DRAW_3DNN_MPLT_CB_FUNC mplt_cb_func/*=NULL*/,
				void *mplt_cb_param/*=NULL*/,
				OBF_MATERIAL_CB material_cb_func/*=NULL*/, void *material_cb_param/*=NULL*/,
#if _PS3 | _XBOX | _PC
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_RGB *toon_rim_param/*=NULL*/, float toon_camouflage/*=0.0f*/)
#elif _WII
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/,
				NNS_VECTOR *toon_light/*=NULL*/)
#else
				AMS_DRAWSTATE *draw_state/*=NULL*/, u32 use_light_flag/*=OBD_LIGHT_USE_FLAG_0*/)
#endif
{
	// ◆ amDrawMotionMaterialMotionのコピー改変
	OBS_DRAW_PARAM_3DNN_DRAW_MOTION	*draw_param;
	AMS_PARAM_DRAW_MOTION			*param;
	NNS_MATRIX						*mtx;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	amThreadCheckSafe(0, "ObjDraw3DNNDrawMotionaterialMotion");

	draw_param	= (OBS_DRAW_PARAM_3DNN_DRAW_MOTION *)amDrawMallocDataBuffer(sizeof(OBS_DRAW_PARAM_3DNN_DRAW_MOTION));

	mtx = &draw_param->mtx;
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param =&draw_param->param;
	param->object	= (NNS_OBJECT *)object;
	param->mtx		= mtx;
	param->sub_obj_type	= sub_obj_type;
	param->flag		= drawflag;
	param->texlist	= texlist;
	param->motion	= motion;
	param->frame	= frame;
	//param->material_func	= func;
	param->material_func= NULL;		// objDrawのコールバックを使用

	// 追加
	draw_param->state = NULL;
	if (draw_state) {
		draw_param->state = &draw_param->draw_state;
		MI_CpuCopy8(draw_state, &draw_param->draw_state, sizeof(AMS_DRAWSTATE));
	}

	// ライト
	draw_param->use_light_flag = use_light_flag;

#if _PS3 | _XBOX | _PC
	// トゥーンリムライト
	if (toon_rim_param) {
		draw_param->toon_rim_param = *toon_rim_param;
	}
	else {
		draw_param->toon_rim_param = g_obj.toon_rim_param;
	}
	// 迷彩
	draw_param->toon_camouflage = toon_camouflage;
#elif _WII
	// トゥーンライト
	if (toon_light) {
		draw_param->toon_light = *toon_light;
	}
	else {
		draw_param->toon_light = g_obj.toon_light_vec;
	}
#endif

	// ユーザー処理
	draw_param->user_func	= user_func;
	draw_param->user_param	= user_param;
	
	// マトリックスパレットコールバック
	draw_param->mplt_cb_func	= mplt_cb_func;
	draw_param->mplt_cb_param	= mplt_cb_param;

	// マテリアルコールバック
	draw_param->material_cb_func	= material_cb_func;
	draw_param->material_cb_param	= material_cb_param;

	amDrawRegistCommand(command_state, OBD_DRAW_USER_COMMAND_3DNN_DRAW_MOTION_MATMTN, param);
}

#if _IPHONE
// ==========================================================================
// ObjDraw3DNNDrawPrimitive
/*!
 *	プリミティブオブジェクト  描画
 *
 *	@param prim		[in]	プリミティブ		AMS_PARAM_DRAW_PRIMITIVE
 *	@param command	[in]	描画コマンド		
 *  @param light	[in]	ライト使用フラグ	NNE_PRIM_LIGHT
 *	@param cull		[in]	カリングフラグ		NNE_PRIM_CULL
 *
 *	@note
 *		描画スレッドにプリミティブ描画処理を登録します。
 */
// ==========================================================================
void ObjDraw3DNNDrawPrimitive(AMS_PARAM_DRAW_PRIMITIVE *prim, u32 command/* = OBD_DRAW_CMD_STATE_3DNN*/,NNE_PRIM_LIGHT light/* = NNE_PRIM_LIGHT_DISABLE*/, NNE_PRIM_CULL cull/* = NNE_PRIM_CULL_NONE*/)
{
	OBS_DRAW_PARAM_3DNN_DRAW_PRIMITIVE param;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	// 設定をコピー
	amCopyMemory(&param.dat, prim, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	nnCopyMatrix(&param.mtx, amMatrixGetCurrent());
	param.light = light;
	param.cull  = cull;

	// 登録
	ObjDraw3DNNUserFunc(objDraw3DNNDrawPrimitive_DT, &param, sizeof(OBS_DRAW_PARAM_3DNN_DRAW_PRIMITIVE), command);	
}
#endif //_IPHONE

// ==========================================================================
// マテリアルコールバック
// ==========================================================================
// ==========================================================================
// ObjDraw3DNNGetMaterialUserData
/*!
 *	オブジェクト3D NN マテリアルユーザーデータ取得
 *
 *	@param val		[in]	ドローコールバック変数
 *
 *	@return		ユーザーデータ
 */
// ==========================================================================
u32 ObjDraw3DNNGetMaterialUserData(NNS_DRAWCALLBACK_VAL *val)
{
	u32	user_data = 0;

#if _PC
	switch (val->pMaterial->fType) {
	case NND_MATTYPE_MATRIEALDESC:
		user_data = ((NNS_MATERIAL_DESC*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_STDSHADERDESC:
		user_data = ((NNS_MATERIAL_STDSHADER_DESC*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_USERPROFILE:
		user_data = ((NNS_MATERIAL_STDSHADER_DESC_USER_PROFILE*)val->pMaterial->pMaterial)->User;
		break;

	default:
		user_data = 0;
		break;
	}

#elif _PS3
	switch (val->pMaterial->fType) {
	case NND_MATTYPE_PS3_STDSHADERDESC:
		user_data = ((NNS_MATERIAL_STDSHADER_DESC*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_PS3_USERPROFILE:
		user_data = ((NNS_MATERIAL_STDSHADER_DESC_USER_PROFILE*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_PS3_MATRIEALDESC:
		MTM_ASSERT(!"objDraw.cpp::ObjDraw3DNNGetMaterialUserData() Error NND_MATTYPE_PS3_MATRIEALDESC old state\n");
	default:
		user_data = 0;
		break;
	}

#elif _XBOX
	switch (val->pMaterial->fType) {
	case NND_MATTYPE_MATRIEALDESC:
		user_data = ((NNS_MATERIAL_DESC*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_STDSHADERDESC:
		user_data = ((NNS_MATERIAL_STDSHADER_DESC*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_USERPROFILE:
		user_data = ((NNS_MATERIAL_STDSHADER_DESC_USER_PROFILE*)val->pMaterial->pMaterial)->User;
		break;

	default:
		user_data = 0;
		break;
	}

#elif _WII
	switch (val->pMaterial->fType) {
	case NND_MATTYPE_NOTEXTURE:
		user_data = ((NNS_MATERIAL_NOTEXTURE*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_TEXTURE:
		user_data = ((NNS_MATERIAL_TEXTURE*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_TEXTURE2:
		user_data = ((NNS_MATERIAL_TEXTURE2*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_TEXTURE3:
		user_data = ((NNS_MATERIAL_TEXTURE3*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_TEXTURE4:
		user_data = ((NNS_MATERIAL_TEXTURE4*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_TEXTURE5:
		user_data = ((NNS_MATERIAL_TEXTURE5*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_TEXTURE6:
		user_data = ((NNS_MATERIAL_TEXTURE6*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_TEXTURE7:
		user_data = ((NNS_MATERIAL_TEXTURE7*)val->pMaterial->pMaterial)->User;
		break;

	case NND_MATTYPE_TEXTURE8:
		//user_data = ((NNS_MATERIAL_TEXTURE8*)val->pMaterial->pMaterial)->User;
		MTM_ASSERT(!"objDraw.cpp::ObjDraw3DNNGetMaterialUserData() Error NND_MATTYPE_TEXTURE8 no struct\n");
		// no break
	default:
		user_data = 0;
		break;
	}
	
#elif _IPHONE
	switch (val->pMaterial->fType) {
		case NND_MATTYPE_GLES11_MATRIEALDESC:
			user_data = ((NNS_MATERIAL_GLES11_DESC*)val->pMaterial->pMaterial)->User;
			break;
			
		case NND_MATTYPE_GL_STDSHADERDESC:
			MTM_ASSERT(!"objDraw.cpp::ObjDraw3DNNGetMaterialUserData() Error NND_MATTYPE_GL_STDSHADERDESC no support\n");
			break;
			
		case NND_MATTYPE_GL_USERPROFILE:
			MTM_ASSERT(!"objDraw.cpp::ObjDraw3DNNGetMaterialUserData() Error NND_MATTYPE_GL_USERPROFILE no support\n");
			break;
			
		case NND_MATTYPE_GL_MATRIEALDESC:
			MTM_ASSERT(!"objDraw.cpp::ObjDraw3DNNGetMaterialUserData() Error NND_MATTYPE_GL_MATRIEALDESC old state\n");
		default:
			user_data = 0;
			break;
	}
	
#endif

	return (user_data);
}

// ==========================================================================
// Wii用 トゥーン設定
// ==========================================================================
// ==========================================================================
// ObjDrawObjectSetToon
/*!
 *	Wii用 トゥーンマテリアルコールバック設定
 *
 *	@param obj_3d			[in]	OBS_ACTION3D_NN_WORKワーク
 *
 *	@note
 *		Wii用トゥーンマテリアルコールバックを設定します。^n
 *		その他のターゲットでは空関数になります。\n
 *		material_cb_func, material_cb_param に設定が行われていても\n
 *		上書きしますので気をつけてください。
 */
// ==========================================================================
void ObjDrawSetToon(OBS_ACTION3D_NN_WORK *obj_3d)
{
#if _WII
	if (obj_3d) {
#if defined (MTD_DEBUG)
		if (obj_3d->material_cb_func || obj_3d->material_cb_param) {
			OS_Printf("Warning! objDraw.cpp::ObjDrawSetToon() cb_func or cb_param already been set\n");
		}
#endif
		obj_3d->material_cb_func = ObjDrawToonMaterialCallback;
		obj_3d->material_cb_param= NULL;
	}
#if defined (MTD_DEBUG)
	else {
		MTM_ASSERT(!"objDraw.cpp::ObjDrawSetToon() obj_3d not initialized\n");
	}
#endif
#else
	UNREFERENCED_PARAMETER(obj_3d);
#endif
}

// ==========================================================================
// ObjDrawToonMaterialCallback
/*!
 *	Wii用 トゥーンマテリアルコールバック
 *
 *	@param val				[in]	NNS_DRAWCALLBACK_VAL構造体へのポインタ
 *	@param param			[in]	ユーザーパラメータ(使用しない)
 *
 *	@return		NNE_BOOL
 *
 *	@note
 *		Wii用 トゥーンマテリアルコールバックです。\n
 *		他のターゲットでは nnPutMaterialCore を呼び出します。
 */
// ==========================================================================
NNE_BOOL ObjDrawToonMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	UNREFERENCED_PARAMETER(param);
#if _WII
	return (amDrawToonMaterial(val));
#else
	return (nnPutMaterialCore(val));
#endif
}

#endif // #if (OBD_USE_ACTION3D_NN)



#if (OBD_USE_ACTION3D_ES)
// ==========================================================================
// ObjDrawObjectAction3DES
/*!
 *	オブジェクトアクション 3D ES
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param obj_3des		[in]	表示ワークポインタ
 *
 *	@note
 *		アニメーション更新はメインスレッドで行います。
 *		描画は、描画スレッドに必要情報を渡して行います。
 */
// ==========================================================================
void ObjDrawObjectAction3DES(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_ES_WORK *obj_3des)
{
	VecFx32	pos;
	VecU16	dir;
	
	// セット
	pos	= obj_work->pos;
	dir	= obj_work->dir;
	
	// オフセットを足す
	pos.x	+= obj_work->ofst.x;
	pos.y	+= obj_work->ofst.y;
	pos.z	+= obj_work->ofst.z;
	
	// 逆重力対応
	if (obj_work->dir_fall) {
		dir.z	+= obj_work->dir_fall;
	}
	
	// 描画
	ObjDrawAction3DES(obj_3des, &pos, &dir,
					  &obj_work->scale,
					  &obj_work->disp_flag);
}


// ==========================================================================
// ObjDrawAction3DES
/*!
 *	オブジェクトアクション 3D ES
 *
 *	@param obj_3des		[in]	3DES オブジェクトワーク
 *	@param pos			[in]	オブジェクト座標 （ NULL可 ）
 *	@param dir			[in]	オブジェクト角度 （ NULL可 ）
 *	@param scale		[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param p_disp_flag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *
 *	@note
 *		モーション計算は、メインスレッドで行います。
 *		リピートフラグ(OBD_DISP_REPEAT)はサポートしないため、
 *		データ側で設定するか、手動で再生成してください。
 *		スケールはX,Y,Z,個別に設定することはできません。
 *		scale.xに設定された値が共通のスケール値として使用されます。
 */
// ==========================================================================
void ObjDrawAction3DES(OBS_ACTION3D_ES_WORK *obj_3des, VecFx32 *pos, VecU16 *dir, VecFx32 *scale, u32 *p_disp_flag)
{
	u32	disp_flag	= 0;
	float	save_speed;
	AMS_QUAT	quat;
	AMS_VECTOR	vec;
	NNS_MATRIX	obj_mtx;
	
	AMS_QUAT	q_disp;
	AMS_QUAT	q_flip;
	AMS_QUAT	q_obj;
	
	AMS_VECTOR	vec_disp;
	AMS_VECTOR	vec_pos;
	
	AMS_VECTOR	vec_scale;
	
	MTM_ASSERT(obj_3des);
	MTM_ASSERT(obj_3des->eff);
	
	/*
	 * 座標変換イメージ
	 *   [pos][q_obj][q_flip][scale(mtx)][disp_ofst][disp_rot][scale(rate)]v
	 *
	 *	※[XXX]は行列, vはローカル座標
	 *	disp_rot	: 表示回転
	 *	disp_ofst	: 表示オフセット
	 *	scale(rate)	: スケール（エミッタのスケールレートを用いた場合）
	 *	scale(mtx)	: スケール（スケール行列を用いた場合）
	 *	q_flip		: フリップ回転
	 *	q_obj		: オブジェクト回転(dir値)
	 *	pos			: オブジェクト座標(pos値)
	 *    ※scale(rate)とscale(mtx)は二者択一
	 */
	
	if (obj_3des->ecb == NULL) {
		return;
	}
	
	// クォータニオン初期化
	amQuatInit(&quat);
	// ベクタ初期化
	amVectorInit(&vec);
	
	// 行列初期化（座標変換による配置のときのみ使用）
	nnMakeUnitMatrix(&obj_mtx);
	
	if (p_disp_flag) {
		disp_flag = *p_disp_flag;

#if 1
		if (!(*p_disp_flag & OBD_DISP_STOP)) {
			// 停止中はENDフラグを落とさない
			*p_disp_flag	&= ~OBD_DISP_END;
		}
#else
		*p_disp_flag	&= ~OBD_DISP_END;
#endif
		
	}
	
	
	// 表示角度
	amQuatEulerToQuatXYZ(&q_disp,
						 (Angle32)obj_3des->disp_rot.x,
						 (Angle32)obj_3des->disp_rot.y,
						 (Angle32)obj_3des->disp_rot.z);
	
	
	// 左右フリップ（回転）
	if (!(disp_flag & OBD_DISP_NODIRFLIP)) {
		
		if (disp_flag & OBD_DISP_HFLIP) {
			
			if (obj_3des->flag & OBD_ACTFLAG_3D_ES_Z_AXIS_FLIP) {	// Z軸回転を利用
				// -Y方向→-X方向
				amQuatEulerToQuatXYZ(&q_flip, NNM_DEGtoA32(.0f), NNM_DEGtoA32(.0f), NNM_DEGtoA32(270.0f));
			}
			else {	// Y軸回転を利用
				// +Z方向→-X方向
				amQuatEulerToQuatXYZ(&q_flip, NNM_DEGtoA32(.0f), NNM_DEGtoA32(270.0f), NNM_DEGtoA32(.0f));
			}
		}
		else {
			if (obj_3des->flag & OBD_ACTFLAG_3D_ES_Z_AXIS_FLIP) {	// Z軸回転を利用
				// -Y方向→+X方向
				amQuatEulerToQuatXYZ(&q_flip, NNM_DEGtoA32(.0f), NNM_DEGtoA32(.0f), NNM_DEGtoA32(90.0f));
			}
			else {	// Y軸回転を利用
				// +Z方向→+X方向
				amQuatEulerToQuatXYZ(&q_flip, NNM_DEGtoA32(.0f), NNM_DEGtoA32(90.0f), NNM_DEGtoA32(.0f));
			}
		}
	}
	else {
		amQuatInit(&q_flip);
	}
	
	// オブジェクト回転
	if (dir && !(disp_flag & OBD_DISP_NODIR)) {
		
		if (!(disp_flag & OBD_DISP_3D_COORDINATE)) {
			amQuatEulerToQuatXYZ(&q_obj,
								 (Angle32)-dir->x, (Angle32)dir->y, (Angle32)-dir->z);
		}
		else {
			amQuatEulerToQuatXYZ(&q_obj,
								 (Angle32)dir->x, (Angle32)dir->y, (Angle32)dir->z);
		}
		
		// ユーザ角度クォータニオン
		if (obj_3des->flag & OBD_ACTFLAG_3D_ES_USER_DIR_QUAT) {
			// DIR前にユーザ指定のクォータニオンで回しておく
			nnMultiplyQuaternion(&q_obj, &q_obj, &obj_3des->user_dir_quat);	// 右から乗算
		}
	}
	else {
		amQuatInit(&q_obj);
	}
	
	
	// 表示オフセット
	amVectorSet(&vec_disp,
				obj_3des->disp_ofst.x,
				obj_3des->disp_ofst.y,
				obj_3des->disp_ofst.z);
	
	
	// オブジェクト平行移動座標
	if (pos) {
		VecFx32	pos_work	= *pos;
		if (!(disp_flag & OBD_DISP_3D_COORDINATE)) {
			pos_work.y	= -pos_work.y;
		}
		
		amVectorSet(&vec_pos,
					FX_FX32_TO_F32(pos_work.x),
					FX_FX32_TO_F32(pos_work.y),
					FX_FX32_TO_F32(pos_work.z));
	}
	else {
		amVectorInit(&vec_pos);
	}
	
	// スケール設定
	amVectorOne(&vec_scale);
	if (scale && !(disp_flag & OBD_DISP_NOSCALE)) {
		if (obj_3des->flag & OBD_ACTFLAG_3D_ES_SCALE_BY_MTX) {
			amVectorSet(&vec_scale,
						FX_FX32_TO_F32(scale->x),
						FX_FX32_TO_F32(scale->y),
						FX_FX32_TO_F32(scale->z));
			amEffectSetSizeRate(obj_3des->ecb, 1.f);
		}
		else {
			// スケール
			amEffectSetSizeRate(obj_3des->ecb, FX_FX32_TO_F32(scale->x));
		}
	}
	
	// =============== 配置設定 ====================
	if (obj_3des->flag & OBD_ACTFLAG_3D_ES_POSITION_EMITTER) {
		// エミッター自体の回転・平行移動による配置
		
		// 表示オフセット加算
		amVectorAdd(&vec, &vec_disp, &vec);
		
		// 表示回転以外のクォータニオンを合成
		amQuatMulti(&quat, &q_flip, &quat);
		amQuatMulti(&quat, &q_obj, &quat);
		
		// オフセットに回転を反映（ベクトルの型に注意）
		{
			NNS_MATRIX	mtx_dir_flip;	// dir と flipによる回転マトリクス
			// 表示回転以外をオフセットに適用
			nnMakeQuaternionMatrix(&mtx_dir_flip, &quat);
			AMS_VECTOR3	vec3	= {vec.x, vec.y, vec.z};	// PS3 Warning回避のため、一旦正しい型の変数にコピー
			nnTransformVector(&vec3, &mtx_dir_flip, &vec3);
			vec.x	= vec3.x;
			vec.y	= vec3.y;
			vec.z	= vec3.z;
		}
		
		
		// 表示回転クォータニオン合成
		amQuatMulti(&quat, &quat, &q_disp);	// 「右」から乗算
		
		// エミッタ回転設定
		if (obj_3des->flag & OBD_ACTFLAG_3D_ES_EMT_USE_DATA_ROT) {
			// データのrotationを適用する
			// （データ側の回転はここで設定したものより後に掛かるので注意。
			// 回転せずにそのまま出す場合以外は使用は「非」推奨）
			amEffectSetRotate(obj_3des->ecb, &quat, TRUE);
		}
		else {
			// データのrotationを上書き
			amEffectSetRotate(obj_3des->ecb, &quat);
		}
		
		// スケール設定
		{
			NNS_MATRIX	scale_mtx;
			nnMakeScaleMatrix(&scale_mtx, vec_scale.x, vec_scale.y, vec_scale.z);
			// スケール行列を左から乗算
			nnMultiplyMatrix(&obj_mtx, &scale_mtx, &obj_mtx);
		}
		
		// エミッタ平行移動設定
		if (obj_3des->flag & OBD_ACTFLAG_3D_ES_PARTICLE_DEPEND) {
			// マトリックスでオブジェクト平行移動を反映
			NNS_MATRIX	trans_mtx;
			nnMakeTranslateMatrix(&trans_mtx, vec_pos.x, vec_pos.y, vec_pos.z);
			nnMultiplyMatrix(&obj_mtx, &trans_mtx, &obj_mtx);
		}
		else {
			// オブジェクト平行移動座標加算
			amVectorAdd(&vec, &vec_pos, &vec);
		}
		
		amEffectSetTranslate(obj_3des->ecb, &vec);
	}
	else {
		// 行列による座標変換を用いた配置
		NNS_MATRIX	mtx_disp_ofst;
		NNS_MATRIX	mtx_disp_rot;
		
		// 表示回転以外のクォータニオンを合成
		amQuatMulti(&quat, &q_flip, &quat);
		amQuatMulti(&quat, &q_obj, &quat);
		
		// 表示回転・オフセット「以外」のマトリクスを生成
		nnMakeQuaternionMatrix(&obj_mtx, &quat);
		NNS_VECTOR	vec3_pos	= {vec_pos.x, vec_pos.y, vec_pos.z};	// PS3 Warning回避のため、一旦正しい型の変数にコピー
		nnCopyVectorMatrixTranslation(&obj_mtx,
									  &vec3_pos);	// 平行移動を左から掛けるのと等価
		
		// スケール反映
		{
			NNS_MATRIX	scale_mtx;
			nnMakeScaleMatrix(&scale_mtx, vec_scale.x, vec_scale.y, vec_scale.z);
			// スケール行列を「右」から乗算
			nnMultiplyMatrix(&obj_mtx, &obj_mtx, &scale_mtx);
		}
		
		// 表示オフセット平行移動を反映
		nnMakeTranslateMatrix(&mtx_disp_ofst,
							  vec_disp.x, vec_disp.y, vec_disp.z);
		nnMultiplyMatrix(&obj_mtx, &obj_mtx, &mtx_disp_ofst);
		
		// 表示回転を反映
		nnMakeQuaternionMatrix(&mtx_disp_rot, &q_disp);
		nnMultiplyMatrix(&obj_mtx, &obj_mtx, &mtx_disp_rot);
	}
	
	// カメラ設定
	ObjDraw3DESSetCamera(obj_3des, &obj_mtx);
	
	// エフェクトのワールド変換行列をプッシュ（描画コマンド）
	ObjDraw3DESMatrixPush(&obj_mtx, obj_3des->command_state);
	
	/* 更新・描画 */
	
	// 速度退避
	save_speed	= amEffectGetUnitFrame();
	
	if (!(disp_flag & OBD_DISP_NOUPDATE)) {
		if (disp_flag & OBD_DISP_STOP) {
			amEffectSetUnitTime(.0f, 60);
		}
		else {
			amEffectSetUnitTime(obj_3des->speed * FX_FX32_TO_F32(g_obj.speed), 60);
		}
		
		// 更新
		amEffectUpdate(obj_3des->ecb);
		if (amEffectIsDelete(obj_3des->ecb)) {
#if 0
			/* obj_3des->ecb の値をクリアしないこと！ */
			
			// リピートチェック
			if (disp_flag & OBD_DISP_REPEAT) {
				// リピート時の再生成
				amEffectDelete(obj_3des->ecb);
				obj_3des->ecb	= amEffectCreate((AMS_AME_HEADER*)obj_3des->eff);// TODO : 仮
			}
			else {
				if (p_disp_flag) {
					*p_disp_flag	|= OBD_DISP_END;
				}
			}
#else
			// リピートはサポートしない
			obj_3des->ecb	= NULL;
			if (p_disp_flag) {
				*p_disp_flag	|= OBD_DISP_END;
			}
#endif
		}
	}
	
	// 速度戻す
	amEffectSetUnitTime(save_speed, 60);
	
	// 描画
	if (!(disp_flag & OBD_DISP_NODISP)) {
		if (obj_3des->ecb) {
			ObjDraw3DESEffect(obj_3des->ecb, obj_3des->texlist, obj_3des->command_state);
			
			// 複製描画
			if (obj_3des->flag & OBD_ACTFLAG_3D_ES_DUPLICATE_DRAW) {
				NNS_MATRIX	dup_draw_mtx;
				// 複製分のワールドマトリクスを作成
				nnMakeTranslateMatrix(&dup_draw_mtx,
									  obj_3des->dup_draw_ofst.x,
									  obj_3des->dup_draw_ofst.y,
									  obj_3des->dup_draw_ofst.z);
				nnMultiplyMatrix(&dup_draw_mtx, &dup_draw_mtx, &obj_mtx);
				ObjDraw3DESMatrixPop(obj_3des->command_state);					// 通常描画のマトリクスをポップ
				ObjDraw3DESSetCamera(obj_3des, &dup_draw_mtx);	// 複製描画用カメラ設定
				ObjDraw3DESMatrixPush(&dup_draw_mtx, obj_3des->command_state);	// 複製描画のマトリクスをプッシュ
				ObjDraw3DESEffect(obj_3des->ecb, obj_3des->texlist, obj_3des->command_state);	// 描画
			}
		}
	}
	
	// エフェクトのワールド変換行列をポップ（描画コマンド）
	ObjDraw3DESMatrixPop(obj_3des->command_state);
}


// ==========================================================================
// ObjDraw3DESEffect
/*!
 *	オブジェクトアクション 3D ES 描画
 *
 *	@param ecb				[in]	ECB（エフェクトコントロールブロック）
 *	@param texlist			[in]	テクスチャリスト
 *	@param command_state	[in]	コマンドステート
 *
 *	@note
 *		描画スレッドに描画処理を登録します。\n
 */
// ==========================================================================
void ObjDraw3DESEffect(AMS_AME_ECB *ecb, NNS_TEXLIST *texlist, Uint32 command_state)
{
	// デフォルトでは3DNNと一緒に描画
	amEffectDraw(ecb, texlist, command_state);
}


// ==========================================================================
// ObjDraw3DESMatrixPush
/*!
 *	オブジェクトアクション 3D ES マトリックスプッシュ
 *
 *	@param mtx				[in]	プッシュするマトリックス
 *	@param command_state	[in]	コマンドステート
 *
 *	@note
 *		ESエフェクト描画用にマトリックスをプッシュし、\n
 *		ビュー行列を加味して3Dプリミティブ描画用マトリクスを設定します。
 */
// ==========================================================================
void ObjDraw3DESMatrixPush(NNS_MATRIX *mtx, Uint32 command_state)
{
#if 1
	ObjDraw3DNNUserFunc(objDraw3DESMatrixPush_UserFunc,
						mtx, sizeof(NNS_MATRIX), command_state);
#else
	amMatrixPush(&obj_mtx);
#endif
}

// ==========================================================================
// ObjDraw3DESMatrixPop
/*!
 *	オブジェクトアクション 3D ES マトリックスポップ
 *
 *	@param command_state	[in]	コマンドステート
 *
 *	@note
 *		ESエフェクト描画用にマトリックスをポップします。\n
 */
// ==========================================================================
void ObjDraw3DESMatrixPop(Uint32 command_state)
{
#if 1
	ObjDraw3DNNUserFunc(objDraw3DESMatrixPop_UserFunc,
						NULL, 0, command_state);
#else
	amMatrixPop();
#endif
}


// ==========================================================================
// ObjDrawKillAction3DES
/*!
 *	オブジェクトアクション 3D ES 終了
 *
 *	@param obj_work		[in]	3DESワーク
 *
 *	@note
 *		ESエフェクトを終了します。
 *		生成済みのパーティクルを即時に更新停止したり消去することはありません。
 *		寿命が無限に設定されているエフェクトを自然に終わらせたい時等に使用してください。
 */
// ==========================================================================
void ObjDrawKillAction3DES(OBS_OBJECT_WORK *obj_work)
{
	MTM_ASSERT(obj_work->obj_3des);
	
	if (obj_work->obj_3des->ecb) {
		amEffectKill(obj_work->obj_3des->ecb);
	}
}

// =======================================================================
// ObjDraw3DESSetCamera
/*!
  オブジェクトアクション 3D ES カメラ設定
  
  @param obj_3des	[in]	オブジェクト3DESワーク
  @param obj_mtx	[in]	エフェクト用のワールド変換行列
  
  @note
  エフェクト用にカメラ設定を行います。
  内部でObjDraw3DNNSetCamera()を呼んでいます。
  obj_mtxの値を参照して、エフェクトソート用のカメラを適切な座標に設定しています。
 */
// =======================================================================
void ObjDraw3DESSetCamera(OBS_ACTION3D_ES_WORK *obj_3des, const NNS_MATRIX *obj_mtx)
{
	NNS_VECTOR	sort_cam_pos;
	NNS_VECTOR	cam_ofst;
	
	MTM_ASSERT(obj_3des);
	MTM_ASSERT(obj_mtx);
	
	// 3DNNのカメラ設定
	ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, g_obj.glb_camera_type, obj_3des->command_state);
	
	// カメラ座標を取得
	ObjCameraDispPosGet(g_obj.glb_camera_id, &sort_cam_pos);
	
	/*
	  エフェクトの回転・平行移動にマトリクスを用いた場合、
	  マトリクスによる座標変換がAMS_PARAM_DRAW_PRIMITIVE::sortZ の値の計算に
	  反映されないため、カメラのほうを動かすことで（無理矢理）正しい深度が得られるようにする。
	*/
	
	// カメラとエフェクトとの本来の位置関係になるように
	// ソート用カメラの位置を調整する
	amVectorSet(&cam_ofst,
				-NNM_MTX(*obj_mtx, 0, 3),
				-NNM_MTX(*obj_mtx, 1, 3),
				-NNM_MTX(*obj_mtx, 2, 3));
	nnAddVector(&sort_cam_pos, &cam_ofst, &sort_cam_pos);
	
	// ソート用カメラ座標設定
	amEffectSetCameraPos(&sort_cam_pos);
}
#endif // #if (OBD_USE_ACTION3D_ES)



#if OBD_USE_ACTION2D_AMA
// ==========================================================================
// ObjDrawObjectAction2DAMA
/*!
 *	オブジェクトアクション 2D AMA
 *
 *	@param obj_work		[in]	ゲームオブジェワークポインタ
 *	@param obj_2d		[in]	表示ワークポインタ
 *
 *	@note
 *		実際の描画はObjDrawAction2DAMADrawStartの実行で始まります。
 */
// ==========================================================================
void ObjDrawObjectAction2DAMA(OBS_OBJECT_WORK *obj_work, OBS_ACTION2D_AMA_WORK *obj_2d)
{
	VecFx32	pos;
	VecU16	dir;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	// セット
	pos = obj_work->pos;
	dir = obj_work->dir;

	// オフセットを足す
	pos.x += obj_work->ofst.x;
	pos.y += obj_work->ofst.y;
	pos.z += obj_work->ofst.z;


	ObjDrawAction2DAMA(obj_2d, &pos, &obj_work->dir, &obj_work->scale, &obj_work->disp_flag);
}

// ==========================================================================
// ObjDrawAction2DAMA
/*!
 *	オブジェクトアクション 2D AMA
 *
 *	@param obj_2d		[in]	2DAMA オブジェクトワーク
 *	@param pos			[in]	オブジェクト座標 （ NULL可 ）
 *	@param dir			[in]	オブジェクト角度 （ NULL可 ）
 *	@param scale		[in]	オブジェクト拡大率 （ NULL可 ）
 *	@param p_disp_flag	[io]	表示フラグポインタ（ NULLの場合、標準扱い ）
 *
 *	@note
 *		実際の描画はObjDrawAction2DAMADrawStartの実行で始まります。
 */
// ==========================================================================
void ObjDrawAction2DAMA(OBS_ACTION2D_AMA_WORK *obj_2d, VecFx32 *pos, VecU16 *dir, VecFx32 *scale, u32 *p_disp_flag)
{
	AOS_ACT_ACM	acm;
	u32			disp_flag = 0;
	
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	if (p_disp_flag) {
		if (!(*p_disp_flag & OBD_DISP_STOP)) {
			// 停止中はENDフラグを落とさない
			*p_disp_flag &= ~(OBD_DISP_END);
		}

		disp_flag = *p_disp_flag;
	}

	// テクスチャセット
	AoActSetTexture(obj_2d->texlist);

	AoActAcmInit(&acm);

	// 移動値
	if (pos && !(disp_flag & OBD_DISP_NOPOS)) {
		acm.trans_x = FXM_FX32_TO_FLOAT(pos->x);
		acm.trans_y = FXM_FX32_TO_FLOAT(pos->y);
		acm.trans_z = FXM_FX32_TO_FLOAT(pos->z);

		// 現状、FIX類の表示しか行わないため、
		// OBD_DISP_NOGLBSCALE などのチェックなしでスケールを反映させない
	}

	// 回転
	if (dir && !(disp_flag & OBD_DISP_NODIR)) {
		acm.rotate = NNM_A32toDEG(dir->z);
	}

	// スケール
	if (scale && !(disp_flag & OBD_DISP_NOSCALE)) {
		// 現状、FIX類の表示しか行わないため、
		// OBD_DISP_NOGLBSCALE などのチェックなしでスケールを反映させない
		acm.scale_x = FXM_FX32_TO_FLOAT(scale->x);
		acm.scale_y = FXM_FX32_TO_FLOAT(scale->y);
	}

	// カラー
	acm.color = obj_2d->color;

	// フェード
	acm.fade = obj_2d->fade;

	// アキュムレート設定
	AoActAcmPush(&acm);

	// アップデート
	if (!(disp_flag & OBD_DISP_NOUPDATE)) {
		float	spd;

		spd = obj_2d->speed * FX_FX32_TO_F32(g_obj.speed) * GmMainGetDrawMotionSpeed();

		if (obj_2d->act->frame == obj_2d->frame) {
			AoActUpdate(obj_2d->act, spd);
			obj_2d->frame = obj_2d->act->frame;
		}
		else {
			// フレームが外部で設定されている
			AoActSetFrame(obj_2d->act, obj_2d->frame);
			AoActUpdate(obj_2d->act, 0);
		}

		// ループチェック
		if (disp_flag & OBD_DISP_REPEAT) {
			if (AoActIsEnd(obj_2d->act)) {
				obj_2d->frame = 0;
				disp_flag |= OBD_DISP_END;
			}
		}
		else {
			if (AoActIsEnd(obj_2d->act)) {
				disp_flag |= OBD_DISP_END;
			}
		}

	}

	// 描画
	if (!(disp_flag & OBD_DISP_NODISP)) {
		AoActSortRegAction(obj_2d->act);
	}

	AoActAcmPop(1);

	if (p_disp_flag) {
		*p_disp_flag |= disp_flag & OBD_DISP_END;
	}
}

// ==========================================================================
// ObjDrawAction2DAMADrawStart
/*!
 *	オブジェクトアクション 2D AMA 描画開始
 *
 *	@note
 *		ObjDrawAction2DAMA等で登録された描画命令をソートして描画します。
 */
// ==========================================================================
void ObjDrawAction2DAMADrawStart(void)
{
	// test
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	// 前処理登録
	ObjDraw3DNNUserFunc(objDraw2DAMAPre_DT,
						NULL, 0, OBD_DRAW_CMD_STATE_2DAMA);

	// アクションのソート
#if !_IPHONE
	AoActSortExecute();
#else //!_IPHONE
	AoActSortExecuteFix();
#endif //!_IPHONE

	// 描画
	AoActSortDraw();

	// 登録解除
	AoActSortUnregAll();
}

#endif // #if OBD_USE_ACTION2D_AMA


// ==========================================================================
// ライト設定
// ==========================================================================
#if OBD_USE_ACTION3D_NN
// ==========================================================================
// ObjDrawSetParallelLight
/*!
 *	パラレルライト 設定
 *
 *	@param light_no		[in]	ライトNO
 *	@param col			[in]	カラー設定
 *	@param intensity	[in]	輝度
 *	@param vec			[in]	ライトベクトル設定
 *
 *	@note
 *		light_noのライトをパラレルライトで設定します。\n
 *		ライトのON・OFF、反映は別途行ってください。
 */
// ==========================================================================
void ObjDrawSetParallelLight(NNE_LIGHT light_no, NNS_RGBA *col, float intensity, NNS_VECTOR *vec)
{
	NNS_LIGHT_PARALLEL	*light;

	MTM_ASSERT((u32)light_no < NNE_LIGHT_MAX);
	MTM_ASSERT(col);
	MTM_ASSERT(vec);

	light = &g_obj.light[light_no].parallel;

	nnSetUpParallelLight(light, col, intensity, vec);
	g_obj.light[light_no].light_type = NND_LIGHTTYPE_PARALLEL;
	//nnSetLight(light_no, light, NND_LIGHTTYPE_PARALLEL);
}

// ==========================================================================
// ObjDrawSetPointLight
/*!
 *	ポイントライト 設定
 *
 *	@param light_no		[in]	ライトNO
 *	@param col			[in]	カラー設定
 *	@param intensity	[in]	輝度
 *	@param pos			[in]	ライト位置設定
 *	@param falloffstart	[in]	距離減衰開始
 *	@param falloffend	[in]	距離減衰終了
 *
 *	@note
 *		light_noのライトをポイントライトで設定します。\n
 *		ライトのON・OFF、反映は別途行ってください。
 */
// ==========================================================================
void ObjDrawSetPointLight(NNE_LIGHT light_no, NNS_RGBA *col, float intensity, NNS_VECTOR *pos, float falloffstart, float falloffend)
{
	NNS_LIGHT_POINT	*light;

	MTM_ASSERT((u32)light_no < NNE_LIGHT_MAX);
	MTM_ASSERT(col);
	MTM_ASSERT(pos);

	light = &g_obj.light[light_no].point;

	nnSetUpPointLight(light, col, intensity, pos, falloffstart, falloffend);
	g_obj.light[light_no].light_type = NND_LIGHTTYPE_POINT;
	//nnSetLight(light_no, light, NND_LIGHTTYPE_POINT);
}

// ==========================================================================
// ObjDrawSetTargetSpotLight
/*!
 *	ターゲットスポットライト 設定
 *
 *	@param light_no		[in]	ライトNO
 *	@param col			[in]	カラー設定
 *	@param intensity	[in]	輝度
 *	@param pos			[in]	ライト位置設定
 *	@param target		[in]	ターゲット位置
 *	@param innerangle	[in]	減衰開始角度
 *	@param outerangle	[in]	減衰終了角度
 *	@param falloffstart	[in]	距離減衰開始
 *	@param falloffend	[in]	距離減衰終了
 *
 *	@note
 *		light_noのローテーションスポットライトで設定します。\n
 *		ライトのON・OFF、反映は別途行ってください。
 */
// ==========================================================================
void ObjDrawSetTargetSpotLight(NNE_LIGHT light_no, NNS_RGBA *col, float intensity, NNS_VECTOR *pos, NNS_VECTOR *target,
							Angle32 innerangle, Angle32 outerangle, float falloffstart, float falloffend)
{
	NNS_LIGHT_TARGET_SPOT	*light;

	MTM_ASSERT((u32)light_no < NNE_LIGHT_MAX);
	MTM_ASSERT(col);
	MTM_ASSERT(pos);
	MTM_ASSERT(target);

	light = &g_obj.light[light_no].target_spot;

	nnSetUpTargetSpotLight(light, col, intensity, pos, target, innerangle, outerangle, falloffstart, falloffend);
	g_obj.light[light_no].light_type = NND_LIGHTTYPE_TARGET_SPOT;
	//nnSetLight(light_no, light, NND_LIGHTTYPE_TARGET_SPOT);
}

// ==========================================================================
// ObjDrawSetRotationSpotLight
/*!
 *	ローテーションスポットライト 設定
 *
 *	@param light_no		[in]	ライトNO
 *	@param col			[in]	カラー設定
 *	@param intensity	[in]	輝度
 *	@param pos			[in]	ライト位置設定
 *	@param rottype		[in]	ローテーションタイプ NNE_ROTATETYPE
 *	@param rotation		[in]	回転設定設定
 *	@param innerangle	[in]	減衰開始角度
 *	@param outerangle	[in]	減衰終了角度
 *	@param falloffstart	[in]	距離減衰開始
 *	@param falloffend	[in]	距離減衰終了
 *
 *	@note
 *		light_noのローテーションスポットライトで設定します。\n
 *		ライトのON・OFF、反映は別途行ってください。
 */
// ==========================================================================
void ObjDrawSetRotationSpotLight(NNE_LIGHT light_no, NNS_RGBA *col, float intensity, NNS_VECTOR *pos,
							NNE_ROTATETYPE rottype, NNS_ROTATE_A32 *rotation,
							Angle32 innerangle, Angle32 outerangle, float falloffstart, float falloffend)
{
	NNS_LIGHT_ROTATION_SPOT	*light;

	MTM_ASSERT((u32)light_no < NNE_LIGHT_MAX);
	MTM_ASSERT(col);
	MTM_ASSERT(pos);
	MTM_ASSERT(rotation);

	light = &g_obj.light[light_no].rotation_spot;

	nnSetUpRotationSpotLight(light, col, intensity, pos, rottype, rotation,
									innerangle, outerangle, falloffstart, falloffend);
	g_obj.light[light_no].light_type = NND_LIGHTTYPE_ROTATION_SPOT;
	//nnSetLight(light_no, light, NND_LIGHTTYPE_ROTATION_SPOT);
}

#if _WII
// ==========================================================================
// ObjDrawSetSpecularGCLight
/*!
 *	スペキュラーGCライト 設定
 *
 *	@param light_no		[in]	ライトNO
 *	@param col			[in]	カラー設定
 *	@param intensity	[in]	輝度
 *	@param pos			[in]	ライト位置設定
 *	@param rottype		[in]	ローテーションタイプ NNE_ROTATETYPE
 *	@param rotation		[in]	回転設定設定
 *	@param innerangle	[in]	減衰開始角度
 *	@param outerangle	[in]	減衰終了角度
 *	@param falloffstart	[in]	距離減衰開始
 *	@param falloffend	[in]	距離減衰終了
 *
 *	@note
 *		light_noのローテーションスポットライトで設定します。\n
 *		ライトのON・OFF、反映は別途行ってください。
 */
// ==========================================================================
void ObjDrawSetSpecularGCLight(NNE_LIGHT light_no, NNS_RGBA *col, NNS_VECTOR *dir)
{
	NNS_LIGHT_SPECULAR_GC	*light;

	MTM_ASSERT((u32)light_no < NNE_LIGHT_MAX);
	MTM_ASSERT(col);
	MTM_ASSERT(dir);

	light = &g_obj.light[light_no].specular_gc;

	nnSetUpSpecularLightGC(light, col, dir);

	g_obj.light[light_no].light_type = NND_LIGHTTYPE_SPECULAR_GC;
}
#endif // #if _WII


#endif // #if OBD_USE_ACTION3D_NN

//----- Local Functions -----------------------------------------------------
#if (OBD_USE_ACTION3D_NN)
// ==========================================================================
// objDrawStart_DT
/*!
 *	オブジェクトアクション (描画スレッド処理)
 *
 *	@param tcb			[in]	TCB
 *
 *	@note
 *		描画スレッドに登録されるTCBです。
 */
// ==========================================================================
void objDrawStart_DT(AMS_TCB *tcb)
{
	s32	i;

	UNREFERENCED_PARAMETER(tcb);

#if 1
	// ライト設定

	//   全てのライトの初期化
	//nnInitLight();		// draw_proc 開始時に実行済み

	// 環境光
	nnSetAmbientColor(g_obj.ambient_color.r, g_obj.ambient_color.g, g_obj.ambient_color.b);

	// ライト設定
	for (i = 0; i < NNE_LIGHT_MAX; i++) {
		if (g_obj.def_user_light_flag & (0x00000001 << i)) {
			nnSetLight((NNE_LIGHT)i, &g_obj.light[i].light_param, g_obj.light[i].light_type);
			nnSetLightSwitch((NNE_LIGHT)i, NNE_ON);
		}
		else {
			nnSetLightSwitch((NNE_LIGHT)i, NNE_OFF);
		}
	}

	// ライト反映
	nnPutLightSettings();

#if _WII
	// Wii用トゥーンライト
	_am_draw_toonDir = g_obj.toon_light_vec;
#endif

#else
	NNS_LIGHT_PARALLEL	light;
	NNS_RGBA			light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	static float			light_intensity = 1.0f;
	static NNS_VECTOR		light_dir = {
		-1.0f, -1.0f, -1.0f,
	};
	NNS_VECTOR		light_dir_work;

	// ◆ライトは仮
	// ライトの初期設定
	//   全てのライトの初期化
	//nnInitLight();		// draw_proc 開始時に実行済み
	//   環境光 RGB
	nnSetAmbientColor(0.8f, 0.8f, 0.8f);

	//   ライト０初期設定
	nnNormalizeVector(&light_dir_work, &light_dir);
	nnSetUpParallelLight(&light, &light_col, light_intensity, &light_dir_work);
	nnSetLight(NNE_LIGHT_0, &light, NND_LIGHTTYPE_PARALLEL);	// パラレルライト
	nnSetLightSwitch(NNE_LIGHT_0, NNE_ON);

	// Zone1-3のみ夕焼け処理【暫定処理】
	if (g_gs_main_sys_info.stage_id == 	GSD_MAIN_STAGE_ID_1_3) {
		nnSetAmbientColor(1.f, 0.0f, 0.0f);
	}

	nnPutLightSettings();

#if _WII
	// Wii用トゥーンライト
	_am_draw_toonDir = light_dir_work;
#endif
#endif	// #if 1

	// 3DNN描画
	for (i = 0; i < OBD_DRAW_CMD_STATE_MAX; i++) {
		if (obj_draw_3dnn_command_state_tbl[i] == OBD_DRAW_CMD_STATE_INVALID) {
			continue;
		}
		
#if defined(AMD_DEBUG)
		bool is_nodisp = false;
		if (g_gm_main_system.debug_flag & GMD_DEBUG_FLAG_NODISP_FAR) {
			switch (obj_draw_3dnn_command_state_tbl[i]) {
			case OBD_DRAW_CMD_STATE_PRE_MAPFAR:
			case OBD_DRAW_CMD_STATE_MAPFAR:
			case OBD_DRAW_CMD_STATE_POST_MAPFAR:
				is_nodisp = true;
				break;
			default:
				break;
			}
		}
		if (g_gm_main_system.debug_flag & GMD_DEBUG_FLAG_NODISP_FIX) {
			switch (obj_draw_3dnn_command_state_tbl[i]) {
			case OBD_DRAW_CMD_STATE_2DAMA:
				is_nodisp = true;
				break;
			default:
				break;
			}
		}
		if (!is_nodisp) //下に続ける
#endif //defined(AMD_DEBUG)
		amDrawExecCommand(obj_draw_3dnn_command_state_tbl[i], g_obj.drawflag);

		if (obj_draw_3dnn_command_state_exe_end_scene_tbl[i]) {
			// シーン描画終了(半透明描画開始)
			amDrawEndScene();
		}
	}
}

// ==========================================================================
// objDraw3DNNSetCamera
/*!
 *	オブジェクトアクション 3D NN カメラ設定
 *
 *	@param	camera_id	[in]	オブジェクトカメラID -1で仮カメラ
 *	@param	proj_type	[in]	射影タイプ NNE_PROJECTION_TYPE
 *	@param	task_prio	[in]	カメラ設定タスクプライオリティ
 *
 *	@note
 *		カメラを設定します
 */
// ==========================================================================
void objDraw3DNNSetCamera(OBS_CAMERA *obj_camera, NNE_PROJECTION_TYPE proj_type, u32 command_state)
{
	OBS_DRAW_PARAM_3DNN_SET_CAMERA	*camera_param;

	// バッファ取得
	camera_param = (OBS_DRAW_PARAM_3DNN_SET_CAMERA *)amDrawMallocDataBuffer(sizeof(OBS_DRAW_PARAM_3DNN_SET_CAMERA));

	// 射影タイプ保存
	camera_param->proj_type = proj_type;

	if (obj_camera) {
		// 射影行列コピー
		switch (proj_type) {
		default:
			MTM_ASSERT(0);
		// no break;
		case NNE_PROJECTION_TYPE_PERSPECTIVE:
		// 透視射影
			memcpy(camera_param->prj_mtx, obj_camera->prj_pers_mtx, sizeof(NNS_MATRIX44));
			break;

		case NNE_PROJECTION_TYPE_ORTHO:
		// 正射影
			memcpy(camera_param->prj_mtx, obj_camera->prj_ortho_mtx, sizeof(NNS_MATRIX44));
			break;
		}

		// ワールドビューマトリクスコピー
		memcpy(camera_param->view_mtx, obj_camera->view_mtx, sizeof(NNS_MATRIX));
	}
	else {
		// 仮カメラ
		NNS_MATRIX			view_mtx;
		NNS_MATRIX44		proj_mtx;
		NNS_CAMERAPTR		camera_ptr;
		NNS_CAMERA_TARGET_ROLL	camera;
		NNS_VECTOR		target_pos = {
			0.0f, 0.0f, 0.0f,
		};

		// プログラムで生成するカメラ初期設定
		camera_ptr.fType	= NND_CAMERATYPE_TARGET_ROLL;
		camera_ptr.pCamera	= &camera;
		camera.Target		= target_pos;
		camera.Position		= camera.Target;
		camera.Position.z	+= 50.0f;
		camera.Position.x	+= 0.0f;
		camera.Roll			= NNM_DEGtoA32(0.0f);	// カメラロール
		camera.Fovy			= NNM_DEGtoA32(45.0f);	// カメラ視野角
		camera.Aspect		= AMD_SCREEN_ASPECT;	// イメージアスペクト
		camera.ZNear		= 1.0f; 				// カメラ近接面
		camera.ZFar			= 60000.f; 				// カメラ遠方面

		// プロジェクション設定
		switch (proj_type) {
		default:
			MTM_ASSERT(0);
		// no break;
		case NNE_PROJECTION_TYPE_PERSPECTIVE:
		// 透視射影
			nnMakePerspectiveMatrix(&proj_mtx,
					camera.Fovy, camera.Aspect, camera.ZNear, camera.ZFar);
			memcpy(camera_param->prj_mtx, &proj_mtx, sizeof(NNS_MATRIX44));
			break;

		case NNE_PROJECTION_TYPE_ORTHO:
		// 正射影
			{
				float	scale = 0.078125f;	// ワイド時に横5ブロック分がこれぐらい
				float	dx, dy;
				//dy		= AMD_SCREEN_2D_HEIGHT * scale * 0.5f;
				dy		= g_obj.disp_height * scale * 0.5f;
				dx		= dy * camera.Aspect;
				nnMakeOrthoMatrix(&proj_mtx,
						-dx, dx, -dy, dy, camera.ZNear, camera.ZFar);
				memcpy(camera_param->prj_mtx, &proj_mtx, sizeof(NNS_MATRIX44));
			}
			break;
		}

		// カメラからワールドビューマトリクスを求める
		nnMakeTargetRollCameraViewMatrix(&view_mtx, &camera);

		// ワールドビューマトリクスコピー
		memcpy(camera_param->view_mtx, &view_mtx, sizeof(NNS_MATRIX));
	}

	// エフェクト用にワールドビューマトリクスを設定
	amEffectSetWorldViewMatrix(&camera_param->view_mtx);

	// 描画コマンド発行
	amDrawRegistCommand(command_state, OBD_DRAW_USER_COMMAND_3DNN_SET_CAMERA, camera_param);
}

// ==========================================================================
// 描画コマンド
// ==========================================================================
// ==========================================================================
// objDraw3DNNModelCommandFunc
/*!
 *	オブジェクト3Dモデル コマンド処理関数
 *
 *	@param command		[in]	コマンドワーク
 *	@param drawflag		[in]	描画フラグ NNF_DRAWOBJ
 */
// ==========================================================================
void objDraw3DNNModelCommandFunc(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	MTM_ASSERT(command);

	obj_draw_user_command_func_tbl[command->command_id](command, drawflag);
}

// ==========================================================================
// objDraw3DNNModel_DT
/*!
 *	オブジェクト3Dモデル 描画 (描画スレッド処理)
 *
 *	@param command		[in]	コマンドワーク
 *	@param drawflag		[in]	描画フラグ NNF_DRAWOBJ
 */
// ==========================================================================
void objDraw3DNNModel_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	// ◆ _amDrawObjectのコピー 改変

	OBS_DRAW_PARAM_3DNN_MODEL		*draw_param;
	AMS_PARAM_DRAW_OBJECT			*param;
	NNS_MATRIX						base_mtx, *plt_mtx;
	NNF_NODESTATUS					*nstat;
	s32								num;

	OBS_DRAW_PARAM_3DNN_SORT_MODEL	*sort_draw_param;
	AMS_COMMAND_HEADER				*command_sort;
	AMS_PARAM_SORT_DRAW_OBJECT		*param_sort;
	AMS_DRAWSTATE					*draw_state;


	amMatrixPush();

	draw_param	= (OBS_DRAW_PARAM_3DNN_MODEL *)command->param;

	// ライト設定
	if (g_obj.def_user_light_flag ^ draw_param->use_light_flag) {
		// 標準ライトと違いがある
		objDrawSetDrawLight(draw_param->use_light_flag);
	}

	// ユーザー処理
	if (draw_param->user_func) {
		draw_param->user_func(draw_param->user_param);
	}

	// 不透明描画
	param		= &draw_param->param;
	num			= param->object->nNode;

	sort_draw_param = (OBS_DRAW_PARAM_3DNN_SORT_MODEL*)amDrawMallocWorkBuffer((s32)(
							sizeof(OBS_DRAW_PARAM_3DNN_SORT_MODEL) +
							//(sizeof(NNF_NODESTATUS) + sizeof(NNS_MATRIX)) * num);
							sizeof(NNS_MATRIX) * num +
							sizeof(NNF_NODESTATUS) * ((num + 3) & ~3)));
	// ◆不要かも
	amZeroMemory(sort_draw_param,
							sizeof(OBS_DRAW_PARAM_3DNN_SORT_MODEL) +
							//(sizeof(NNF_NODESTATUS) + sizeof(NNS_MATRIX)) * num);
							sizeof(NNS_MATRIX) * num +
							sizeof(NNF_NODESTATUS) * ((num + 3) & ~3));

	plt_mtx	= (NNS_MATRIX *)(sort_draw_param + 1);
	nstat	= (NNF_NODESTATUS *)(plt_mtx + num);

	if (param->mtx != NULL) {
		nnMultiplyMatrix(&base_mtx, amMatrixGetCurrent(), param->mtx);
		nnMultiplyMatrix(&base_mtx, &_am_draw_world_view_matrix, &base_mtx);
		//nnMultiplyMatrix(&base_mtx, amDrawGetWorldViewMatrix(), &base_mtx);
	}
	else {
		nnMultiplyMatrix(&base_mtx,
				&_am_draw_world_view_matrix, amMatrixGetCurrent());
	}
	nnSetUpNodeStatusList(nstat, num, NND_NODESTATUS_NONE);
#if AMD_USE_DRAW_THREAD
	nnCalcMatrixPalette(plt_mtx, nstat, param->object, &base_mtx,
			&_am_draw_stack, NND_SETNODESTATUS_CLIP_HIDE);
#else
	nnCalcMatrixPalette(plt_mtx, nstat, param->object, &base_mtx,
			&_am_default_stack, NND_SETNODESTATUS_CLIP_HIDE);
#endif

#if 0
	nnTranslateMatrix(&base_mtx, &base_mtx, param->object->Center.x,
			param->object->Center.y, param->object->Center.z);
#endif

#if _WII
	NNS_VTXLISTPTR	*vtx_list;
	nnCalcPliableVerticesGC(&vtx_list, param->object, plt_mtx);
	nnSetPliableVertexBufferGC(vtx_list, &obj_draw_unit_matrix);
#endif

	if (param->texlist != NULL)
		nnSetTextureList(param->texlist);

	// 追加 描画ステータス設定
	if (draw_param->state) {
		amDrawPushState();
		amDrawSetState(draw_param->state);
	}

	// マテリアルコールバック設定
	if (draw_param->material_cb_func) {
		objDraw3DNNSetMaterialCallback(draw_param->material_cb_func, draw_param->material_cb_param);
	}

	// トゥーンリムライト設定
#if _PS3 | _XBOX | _PC
	if ((param->flag | drawflag | amDrawGetState()->drawflag) & NND_DRAWOBJ_SHADER_USER_PROFILE_TOON) {
#if _PC | _XBOX
		nnSetUserUniformDXG20(0,
			draw_param->toon_rim_param.r, draw_param->toon_rim_param.g, draw_param->toon_rim_param.b,
			draw_param->toon_camouflage);
#elif _PS3
		nnSetUserUniformPS3(0,
			draw_param->toon_rim_param.r, draw_param->toon_rim_param.g, draw_param->toon_rim_param.b,
			draw_param->toon_camouflage);
#endif
	}
#endif

	// Wii用トゥーンライト設定
#if _WII
	_am_draw_toonDir = draw_param->toon_light;
#endif

	if (command->command_id == OBD_DRAW_USER_COMMAND_3DNN_MODEL) {
		nnDrawObject(
				param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if OBD_DRAW_MODEL_ALL
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | amDrawGetState()->drawflag);
	//	nnDrawObject(param->object, plt_mtx, nstat,
	//			param->sub_obj_type |
	//				NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE |
	//				NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
	//			param->flag | drawflag);
	} else {
		nnDrawMaterialMotionObject(
				(NNS_MATMOTOBJ *)param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if OBD_DRAW_MODEL_ALL
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | amDrawGetState()->drawflag);
	}

	// マテリアルコールバック設定クリア
	if (draw_param->material_cb_func) {
		objDraw3DNNSetMaterialCallback(NULL, NULL);
#if _WII
		GXSetTevDirect(GX_TEVSTAGE0);
#endif
	}

	// 追加 描画ステータス復帰
	if (draw_param->state) {
		amDrawPopState();
	}

	// ライト復帰
	if (g_obj.def_user_light_flag ^ draw_param->use_light_flag) {
		// 標準ライトと違いがある
		objDrawSetDefaultLight();
	}

	// 半透明描画登録
#if !OBD_DRAW_MODEL_ALL
#if NND_NEW_OBJECT_FORMAT
	if (param->object->fType & NND_OBJTYPE_TRANSPARENT) {
#else
	if (1) {
#endif
		//command_sort	= (AMS_COMMAND_HEADER *)(nstat + ((num + 3) & ~3));
		//param_sort		= (AMS_PARAM_SORT_DRAW_OBJECT *)(command_sort + 1);
		//draw_state		= (AMS_DRAWSTATE *)(param_sort + 1);
		command_sort	= &sort_draw_param->cmd_header;
		param_sort		= &sort_draw_param->param;
		draw_state		= &sort_draw_param->state;
		//command_sort	= (AMS_COMMAND_HEADER*)sort_draw_param;//&sort_draw_param->cmd_header;
		//param_sort		= &sort_draw_param->param;

		if (draw_param->state) {
			// 追加 不透明描画時のステートを引き継ぎ
			MI_CpuCopy8(draw_param->state, draw_state, sizeof(AMS_DRAWSTATE));
		}
		else {
			amDrawGetState(draw_state);
		}

		param_sort->drawflag	= drawflag;
		param_sort->draw_object	= param;
		param_sort->mtx			= plt_mtx;
		param_sort->nstat_list	= nstat;
		param_sort->draw_state	= draw_state;
#if _WII
		param_sort->vtx_list	= vtx_list;
#endif

		if (command->command_id == OBD_DRAW_USER_COMMAND_3DNN_MODEL) {
			command_sort->command_id	= OBD_DRAW_USER_COMMAND_SORT_3DNN_MODEL;
		}
		else {
			command_sort->command_id	= OBD_DRAW_USER_COMMAND_SORT_3DNN_MATMTN;
		}
		command_sort->param			= param_sort;
		command_sort->state			= 0;

		// ライト
		sort_draw_param->use_light_flag = draw_param->use_light_flag;

#if _PS3 | _XBOX | _PC
		// トゥーンリムライト
		sort_draw_param->toon_rim_param = draw_param->toon_rim_param;
#endif
#if _WII
		// Wii用トゥーンライト設定
		sort_draw_param->toon_light = draw_param->toon_light;
#endif

		// ユーザー処理
		sort_draw_param->user_func	= draw_param->user_func;
		sort_draw_param->user_param	= draw_param->user_param;

		// マテリアルコールバック
		sort_draw_param->material_cb_func	= draw_param->material_cb_func;
		sort_draw_param->material_cb_param	= draw_param->material_cb_param;
		
		amDrawAddSort(command_sort,
			(Sint32)((param->object->Radius * param->scaleZ - NNM_MTX(base_mtx, 2, 3))
				* 100.0f));
	}
#endif // #if !OBD_DRAW_MODEL_ALL

	amMatrixPop();
}

// ==========================================================================
// objDraw3DNNMotion_DT
/*!
 *	オブジェクト3Dモーション 描画 (描画スレッド処理)
 *
 *	@param command		[in]	コマンドワーク
 *	@param drawflag		[in]	描画フラグ NNF_DRAWOBJ
 */
// ==========================================================================
void objDraw3DNNMotion_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	// ◆ _amDrawMotionTRSのコピー 改変
	OBS_DRAW_PARAM_3DNN_MOTION		*draw_param;
	AMS_PARAM_DRAW_MOTION_TRS		*param;
	NNS_MATRIX						base_mtx, *plt_mtx;
	NNF_NODESTATUS					*nstat;
	Sint32							node_num, plt_num;

	OBS_DRAW_PARAM_3DNN_SORT_MODEL	*sort_draw_param;
	AMS_COMMAND_HEADER				*command_sort;
	AMS_PARAM_SORT_DRAW_OBJECT		*param_sort;
	AMS_DRAWSTATE					*draw_state;


	amMatrixPush();

	draw_param	= (OBS_DRAW_PARAM_3DNN_MOTION *)command->param;

	// ライト設定
	if (g_obj.def_user_light_flag ^ draw_param->use_light_flag) {
		// 標準ライトと違いがある
		objDrawSetDrawLight(draw_param->use_light_flag);
	}

	// ユーザー処理
	if (draw_param->user_func) {
		draw_param->user_func(draw_param->user_param);
	}

	// 不透明描画
	param		= &draw_param->param;
	node_num	= param->object->nNode;
	plt_num		= param->object->nMtxPal;

	if ((command->command_id == OBD_DRAW_USER_COMMAND_3DNN_MOTION_MATMTN)
			&& (param->mmotion != NULL)) {
		NNS_MATMOTOBJ	*mmobject;
		mmobject	= (NNS_MATMOTOBJ *)(param->trslist + node_num);
		nnInitMaterialMotionObject(mmobject,
				param->object, param->mmotion);
		nnCalcMaterialMotion(mmobject,
				param->object, param->mmotion, param->mframe);
		param->object	= (NNS_OBJECT *)mmobject;
	}

	sort_draw_param = (OBS_DRAW_PARAM_3DNN_SORT_MODEL*)amDrawMallocWorkBuffer((s32)(
							sizeof(OBS_DRAW_PARAM_3DNN_SORT_MODEL) +
							sizeof(NNS_MATRIX) * plt_num +
							sizeof(NNF_NODESTATUS) * node_num));
	amZeroMemory(sort_draw_param,
							sizeof(OBS_DRAW_PARAM_3DNN_SORT_MODEL) +
							sizeof(NNS_MATRIX) * plt_num +
							sizeof(NNF_NODESTATUS) * node_num);

	plt_mtx	= (NNS_MATRIX *)(sort_draw_param + 1);
	nstat	= (NNF_NODESTATUS *)(plt_mtx + plt_num);

	if (param->mtx != NULL) {
		nnMultiplyMatrix(&base_mtx, amMatrixGetCurrent(), param->mtx);
		nnMultiplyMatrix(&base_mtx, &_am_draw_world_view_matrix, &base_mtx);
		//nnMultiplyMatrix(&base_mtx, amDrawGetWorldViewMatrix(), &base_mtx);
	} else {
		nnMultiplyMatrix(&base_mtx,
				&_am_draw_world_view_matrix, amMatrixGetCurrent());
	}
	nnSetUpNodeStatusList(nstat, node_num, NND_NODESTATUS_NONE);
#if AMD_USE_DRAW_THREAD
	nnCalcMatrixPaletteTRSList(plt_mtx, nstat, param->object,
			param->trslist, &base_mtx, &_am_draw_stack,
			NND_SETNODESTATUS_CLIP_HIDE);
#else
	nnCalcMatrixPaletteTRSList(plt_mtx, nstat, param->object,
			param->trslist, &base_mtx, &_am_default_stack,
			NND_SETNODESTATUS_CLIP_HIDE);
#endif

	if (param->motion != NULL)
		nnCalcNodeHideMotion(nstat, param->motion, param->frame);

	// マトリックスパレットコールバック
	if (draw_param->mplt_cb_func) {
		draw_param->mplt_cb_func(plt_mtx, param->object, draw_param->mplt_cb_param);
	}

#if _WII
	NNS_VTXLISTPTR	*vtx_list;
	nnCalcPliableVerticesGC(&vtx_list, param->object, plt_mtx);
	nnSetPliableVertexBufferGC(vtx_list, &obj_draw_unit_matrix);
#endif

	if (param->texlist != NULL)
		nnSetTextureList(param->texlist);

	// 追加 描画ステータス設定
	if (draw_param->state) {
		amDrawPushState();
		amDrawSetState(draw_param->state);
	}

	// マテリアルコールバック設定
	if (draw_param->material_cb_func) {
		objDraw3DNNSetMaterialCallback(draw_param->material_cb_func, draw_param->material_cb_param);
	}

	// トゥーンリムライト設定
#if _PS3 | _XBOX | _PC
	if ((param->flag | drawflag | amDrawGetState()->drawflag) & NND_DRAWOBJ_SHADER_USER_PROFILE_TOON) {
#if _PC | _XBOX
		nnSetUserUniformDXG20(0,
			draw_param->toon_rim_param.r, draw_param->toon_rim_param.g, draw_param->toon_rim_param.b,
			draw_param->toon_camouflage);
#elif _PS3
		nnSetUserUniformPS3(0,
			draw_param->toon_rim_param.r, draw_param->toon_rim_param.g, draw_param->toon_rim_param.b,
			draw_param->toon_camouflage);
#endif
	}
#endif

	// Wii用トゥーンライト設定
#if _WII
	_am_draw_toonDir = draw_param->toon_light;
#endif

	if (command->command_id == OBD_DRAW_USER_COMMAND_3DNN_MOTION) {
		nnDrawObject(
				param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if OBD_DRAW_MODEL_ALL
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | amDrawGetState()->drawflag);
	} else {
		nnDrawMaterialMotionObject(
				(NNS_MATMOTOBJ *)param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if OBD_DRAW_MODEL_ALL
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | amDrawGetState()->drawflag);
	}

	// マテリアルコールバック設定クリア
	if (draw_param->material_cb_func) {
		objDraw3DNNSetMaterialCallback(NULL, NULL);
#if _WII
		GXSetTevDirect(GX_TEVSTAGE0);
#endif
	}

	// 追加 描画ステータス復帰
	if (draw_param->state) {
		amDrawPopState();
	}

	// ライト復帰
	if (g_obj.def_user_light_flag ^ draw_param->use_light_flag) {
		// 標準ライトと違いがある
		objDrawSetDefaultLight();
	}

	// 半透明描画登録
#if !OBD_DRAW_MODEL_ALL
#if NND_NEW_OBJECT_FORMAT
	if (param->object->fType & NND_OBJTYPE_TRANSPARENT) {
#else
	if (1) {
#endif
		//command_sort	= (AMS_COMMAND_HEADER*)sort_draw_param;//&sort_draw_param->cmd_header;
		//param_sort		= &sort_draw_param->param;
		//command_sort	= (AMS_COMMAND_HEADER *)(nstat + num);
		//param_sort		= (AMS_PARAM_SORT_DRAW_OBJECT *)(command_sort + 1);
		//draw_state		= (AMS_DRAWSTATE *)(param_sort + 1);
		command_sort	= &sort_draw_param->cmd_header;
		param_sort		= &sort_draw_param->param;
		draw_state		= &sort_draw_param->state;

		if (draw_param->state) {
			// 追加 不透明描画時のステートを引き継ぎ
			MI_CpuCopy8(draw_param->state, draw_state, sizeof(AMS_DRAWSTATE));
		}
		else {
			amDrawGetState(draw_state);
		}

		//param_sort->drawflag	= g_obj.drawflag;	// ソート処理にdrawflagが引き継がれない為
		param_sort->drawflag	= drawflag;
		param_sort->draw_object	= (AMS_PARAM_DRAW_OBJECT *)param;
		param_sort->mtx			= plt_mtx;
		param_sort->nstat_list	= nstat;
		param_sort->draw_state	= draw_state;
#if _WII
		param_sort->vtx_list	= vtx_list;
#endif
		if (command->command_id == AMD_COMMAND_DRAW_OBJECT) {
			command_sort->command_id	= OBD_DRAW_USER_COMMAND_SORT_3DNN_MODEL;
		}
		else {
			command_sort->command_id	= OBD_DRAW_USER_COMMAND_SORT_3DNN_MATMTN;
		}
		command_sort->param			= param_sort;

		// ライト
		sort_draw_param->use_light_flag = draw_param->use_light_flag;

#if _PS3 | _XBOX | _PC
		// トゥーンリムライト
		sort_draw_param->toon_rim_param = draw_param->toon_rim_param;
		// 迷彩
		sort_draw_param->toon_camouflage = draw_param->toon_camouflage;
#endif

#if _WII
		// Wii用トゥーンライト設定
		sort_draw_param->toon_light = draw_param->toon_light;
#endif

		// ユーザー処理
		sort_draw_param->user_func	= draw_param->user_func;
		sort_draw_param->user_param	= draw_param->user_param;

		// マテリアルコールバック
		sort_draw_param->material_cb_func	= draw_param->material_cb_func;
		sort_draw_param->material_cb_param	= draw_param->material_cb_param;


		amDrawAddSort(command_sort,
				(Sint32)((param->object->Radius - NNM_MTX(base_mtx, 2, 3))
				* 100.0f));
		//amDrawAddSort(command_sort,
		//	(Sint32)((param->object->Radius * param->scaleZ - NNM_MTX(base_mtx, 2, 3))
		//		* 100.0f));
	}
#endif // #if !OBD_DRAW_MODEL_ALL

	amMatrixPop();
}

#if _IPHONE
// ==========================================================================
// objDraw3DNNDrawPrimitive_DT
/*!
 *	@param	param	[in]	描画パラメータ(OBS_DRAW_PARAM_3DNN_DRAW_PRIMITIVEが登録済)
 *
 */
// ==========================================================================
static void objDraw3DNNDrawPrimitive_DT(void * param) {
	// amDraw3DPrimitive系からの処理流用
	OBS_DRAW_PARAM_3DNN_DRAW_PRIMITIVE	*dt_work = (OBS_DRAW_PARAM_3DNN_DRAW_PRIMITIVE*)param;
	AMS_PARAM_DRAW_PRIMITIVE* prim = &dt_work->dat;
	
	// 表示
	NNS_MATRIX						base_mtx;
	
	amMatrixPush();
	nnMultiplyMatrix(&base_mtx, amMatrixGetCurrent(), &dt_work->mtx);
	nnMultiplyMatrix(&base_mtx, &_am_draw_world_view_matrix, &base_mtx);
	nnSetPrimitive3DMatrix(&base_mtx);
	
	nnSetPrimitiveTexNum( prim->texlist, prim->texId );
	nnSetPrimitiveTexState( NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV, prim->uwrap, prim->vwrap );
	
	// αブレンドやＺテストなどを設定
	// αテスト
	if ( prim->aTest )
	{
#if _PC | _XBOX 
		// 暫定的に設定（アルファ比較式やアルファ参照値は要検討）
		nnSetPrimitive3DAlphaTestDXG20(NNE_TRUE);
		nnSetPrimitive3DAlphaFuncDXG20(NNE_CMPFUNC_GREATER, 0x80);
#elif _PS3
		nnSetPrimitive3DAlphaFuncPS3(NND_CMPFUNC_PS3_GREATER, 0.5f);
#elif _WII
		nnSetPrimitive3DAlphaCompareGC( GX_GREATER, 0x80, GX_AOP_AND, GX_GREATER, 0x80 );
#elif _IPHONE
		nnSetPrimitive3DAlphaFuncGL(NND_CMPFUNC_GL_GREATER, 0.5f);
#endif
	}
	// αテストしない
	else
	{
#if _PC | _XBOX
		nnSetPrimitive3DAlphaTestDXG20(NNE_FALSE);
#elif _PS3
		nnSetPrimitive3DAlphaFuncPS3(NND_CMPFUNC_PS3_ALWAYS, 0.5f);
#elif _WII
		nnSetPrimitive3DAlphaCompareGC( GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0 );
#elif _IPHONE
		nnSetPrimitive3DAlphaFuncGL(NND_CMPFUNC_GL_ALWAYS, 0.5f);
#endif
	}
	
	// Zマスク（Ｚバッファを更新しない）
	if( prim->zMask )
	{
#if _PC | _XBOX
		nnSetPrimitive3DDepthMaskDXG20(NNE_FALSE);
#elif _PS3
		nnSetPrimitive3DDepthMaskPS3(NNE_FALSE);
#elif _WII
		zmask = GX_FALSE;
#elif _IPHONE
		nnSetPrimitive3DDepthMaskGL(NNE_FALSE);
#endif
	}
	else
	{
#if _PC | _XBOX
		nnSetPrimitive3DDepthMaskDXG20(NNE_TRUE);
#elif _PS3
		nnSetPrimitive3DDepthMaskPS3(NNE_TRUE);
#elif _WII
		zmask = GX_TRUE;
#elif _IPHONE
		nnSetPrimitive3DDepthMaskGL(NNE_TRUE);
#endif
	}
	
	// Zテストする
	if( prim->zTest )
	{
#if _PC | _XBOX
		nnSetPrimitive3DDepthTestDXG20(NNE_TRUE);
		nnSetPrimitive3DDepthFuncDXG20(NNE_CMPFUNC_LESSEQUAL);
#elif _PS3
		nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_LEQUAL);
#elif _WII
		ztest = GX_TRUE;
		zfunc = GX_LEQUAL;
#elif _IPHONE
		nnSetPrimitive3DDepthFuncGL(NND_CMPFUNC_GL_LEQUAL);
#endif
	}
	// Zテストしない
	else
	{
#if _PC | _XBOX
		nnSetPrimitive3DDepthTestDXG20(NNE_FALSE);
#elif _PS3
		nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_ALWAYS);
#elif _WII
		ztest = GX_FALSE;
		zfunc = GX_NEVER;
#elif _IPHONE
		nnSetPrimitive3DDepthFuncGL(NND_CMPFUNC_GL_ALWAYS);
#endif
	}
	
#if _WII
	nnSetPrimitive3DZModeGC(ztest, zfunc, zmask);
#endif
	
	// αブレンド
	if ( prim->ablend )
	{
#if _PC | _XBOX
		nnSetPrimitive3DBlendDXG20( prim->bldSrc, prim->bldDst, prim->bldMode );
#elif _PS3
		nnSetPrimitive3DBlendPS3( prim->bldSrc, prim->bldDst, prim->bldMode);
#elif _WII
		nnSetPrimitive3DBlendModeGC( prim->bldMode, prim->bldSrc, prim->bldDst, GX_LO_NOOP );
#elif _IPHONE
		if ( prim->bldMode == NND_BLENDOP_GL_FUNC_ADD )
		{
			switch ( prim->bldDst )
			{
				case NND_BLENDFUNC_GL_ONE: // 加算
					nnSetPrimitiveBlend( NNE_PRIM_BLEND_ADD );
					break;
					
				case NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA: // 乗算
					nnSetPrimitiveBlend( NNE_PRIM_BLEND_BLEND );
					break;
					
				default:
					nnSetPrimitiveBlend( NNE_PRIM_BLEND_BLEND );
					break;
			}
		}
#endif
	}
	else
	{
#if _WII
		nnSetPrimitiveBlend( NNE_PRIM_BLEND_BLEND ); // これを呼んでいないと加算半透明が設定される
#endif	
	}
	
	nnBeginDrawPrimitive3D( prim->format3D, prim->ablend, dt_work->light, dt_work->cull);
	
	switch ( prim->format3D )
	{
		case NNE_PRIM3D_FMT_PCT:
			nnDrawPrimitive3D( prim->type, prim->vtxPCT3D, prim->count );
			break;
			
		case NNE_PRIM3D_FMT_PC:
			nnDrawPrimitive3D( prim->type, prim->vtxPC3D, prim->count );
			break;
			
		default:
			amAssert(0);
			break;
	}
	
	nnEndDrawPrimitive3D();
	
	amMatrixPop();
}
#endif //_IPHONE
	
// ==========================================================================
// objDraw3DNNSetCamera_DT
/*!
 *	オブジェクトアクション 3D NN カメラ設定 (描画スレッド処理)
 *
 *	@param command		[in]	コマンドワーク
 *	@param drawflag		[in]	描画フラグ NNF_DRAWOBJ
 *
 *	@note
 *		カメラを設定します
 */
// ==========================================================================
void objDraw3DNNSetCamera_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	OBS_DRAW_PARAM_3DNN_SET_CAMERA	*camera_param;

	UNREFERENCED_PARAMETER(drawflag);

	camera_param	= (OBS_DRAW_PARAM_3DNN_SET_CAMERA *)command->param;

	
	if (camera_param->proj_type == NNE_PROJECTION_TYPE_ORTHO) {
		// 正射影
		//nnSetProjection(&camera_param->prj_mtx, NNE_PROJECTION_TYPE_ORTHO);
		amDrawSetProjection(&camera_param->prj_mtx, NNE_PROJECTION_TYPE_ORTHO);
	}
	else {
		// 透視射影
		//nnSetProjection(&camera_param->prj_mtx, NNE_PROJECTION_TYPE_PERSPECTIVE);
		amDrawSetProjection(&camera_param->prj_mtx, NNE_PROJECTION_TYPE_PERSPECTIVE);
	}

	// ワールドビューマトリクスの設定
	amDrawSetWorldViewMatrix(&camera_param->view_mtx);

	// ライト用ビューマトリクス
	nnSetLightMatrix(&camera_param->view_mtx);
	nnPutLightSettings();		// 設定転送
}

// ==========================================================================
// objDraw3DNNUserFunc_DT
/*!
 *	オブジェクトアクション 3D NN ユーザー処理呼び出し (描画スレッド処理)
 *
 *	@param command		[in]	コマンドワーク
 *	@param drawflag		[in]	描画フラグ NNF_DRAWOBJ
 */
// ==========================================================================
void objDraw3DNNUserFunc_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	OBS_DRAW_PARAM_3DNN_USER_FUNC	*user_param;

	UNREFERENCED_PARAMETER(drawflag);

	user_param	= (OBS_DRAW_PARAM_3DNN_USER_FUNC *)command->param;

	MTM_ASSERT(user_param->func);

	// ユーザー処理呼び出し
	user_param->func(user_param->param);
}

// ==========================================================================
// objDraw3DNNDrawMotion_DT
/*!
 *	オブジェクト3Dモーション 描画 (描画スレッド処理)
 *
 *	@param command		[in]	コマンドワーク
 *	@param drawflag		[in]	描画フラグ NNF_DRAWOBJ
 */
// ==========================================================================
void objDraw3DNNDrawMotion_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	// ◆ _amDrawMotionのコピー 改変
	OBS_DRAW_PARAM_3DNN_DRAW_MOTION		*draw_param;
	AMS_PARAM_DRAW_MOTION				*param;
	NNS_MATRIX							base_mtx, *plt_mtx;
	NNF_NODESTATUS						*nstat;
	Sint32								num;

	OBS_DRAW_PARAM_3DNN_SORT_MODEL		*sort_draw_param;
	AMS_COMMAND_HEADER					*command_sort;
	AMS_PARAM_SORT_DRAW_OBJECT			*param_sort;
	AMS_DRAWSTATE						*draw_state;


	amMatrixPush();

	draw_param	= (OBS_DRAW_PARAM_3DNN_DRAW_MOTION *)command->param;

	// ライト設定
	if (g_obj.def_user_light_flag ^ draw_param->use_light_flag) {
		// 標準ライトと違いがある
		objDrawSetDrawLight(draw_param->use_light_flag);
	}

	// ユーザー処理
	if (draw_param->user_func) {
		draw_param->user_func(draw_param->user_param);
	}

	// 不透明描画
	param	= &draw_param->param;
	num		= param->object->nNode;

	sort_draw_param = (OBS_DRAW_PARAM_3DNN_SORT_MODEL*)amDrawMallocWorkBuffer((s32)(
							sizeof(OBS_DRAW_PARAM_3DNN_SORT_MODEL) +
							(sizeof(NNF_NODESTATUS) + sizeof(NNS_MATRIX)) * num));

	amZeroMemory(sort_draw_param,
							sizeof(OBS_DRAW_PARAM_3DNN_SORT_MODEL) +
							(sizeof(NNF_NODESTATUS) + sizeof(NNS_MATRIX)) * num);

	plt_mtx	= (NNS_MATRIX *)(sort_draw_param + 1);
	nstat	= (NNF_NODESTATUS *)(plt_mtx + num);

	if (param->mtx != NULL) {
		nnMultiplyMatrix(&base_mtx, amMatrixGetCurrent(), param->mtx);
		nnMultiplyMatrix(&base_mtx, &_am_draw_world_view_matrix, &base_mtx);
	} else {
		nnMultiplyMatrix(&base_mtx,
				&_am_draw_world_view_matrix, amMatrixGetCurrent());
	}
	nnSetUpNodeStatusList(nstat, num, NND_NODESTATUS_NONE);
#if AMD_USE_DRAW_THREAD
	nnCalcMatrixPaletteMotion(plt_mtx, nstat, param->object,
			param->motion, param->frame, &base_mtx, &_am_draw_stack,
			NND_SETNODESTATUS_CLIP_HIDE);
#else
	nnCalcMatrixPaletteMotion(plt_mtx, nstat, param->object,
			param->motion, param->frame, &base_mtx, &_am_default_stack,
			NND_SETNODESTATUS_CLIP_HIDE);
#endif

	nnCalcNodeHideMotion(nstat, param->motion, param->frame);

	// マトリックスパレットコールバック
	if (draw_param->mplt_cb_func) {
		draw_param->mplt_cb_func(plt_mtx, param->object, draw_param->mplt_cb_param);
	}

#if _WII
	NNS_VTXLISTPTR	*vtx_list;
	nnCalcPliableVerticesGC(&vtx_list, param->object, plt_mtx);
	nnSetPliableVertexBufferGC(vtx_list, &obj_draw_unit_matrix);
#endif

	if (param->texlist != NULL)
		nnSetTextureList(param->texlist);

	// 追加 描画ステータス設定
	if (draw_param->state) {
		amDrawPushState();
		amDrawSetState(draw_param->state);
	}

	// マテリアルコールバック設定
	if (draw_param->material_cb_func) {
		objDraw3DNNSetMaterialCallback(draw_param->material_cb_func, draw_param->material_cb_param);
	}

	// トゥーンリムライト設定
#if _PS3 | _XBOX | _PC
	if ((param->flag | drawflag | amDrawGetState()->drawflag) & NND_DRAWOBJ_SHADER_USER_PROFILE_TOON) {
#if _PC | _XBOX
		nnSetUserUniformDXG20(0,
			draw_param->toon_rim_param.r, draw_param->toon_rim_param.g, draw_param->toon_rim_param.b,
			draw_param->toon_camouflage);
#elif _PS3
		nnSetUserUniformPS3(0,
			draw_param->toon_rim_param.r, draw_param->toon_rim_param.g, draw_param->toon_rim_param.b,
			draw_param->toon_camouflage);
#endif
	}
#endif

	// Wii用トゥーンライト設定
#if _WII
	_am_draw_toonDir = draw_param->toon_light;
#endif

	if (command->command_id == OBD_DRAW_USER_COMMAND_3DNN_DRAW_MOTION) {
		nnDrawObject(
				param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if OBD_DRAW_MODEL_ALL
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | amDrawGetState()->drawflag);
	} else {
		nnDrawMaterialMotionObject(
				(NNS_MATMOTOBJ *)param->object, plt_mtx, nstat,
				param->sub_obj_type
					| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
#if OBD_DRAW_MODEL_ALL
					| NND_SUBOBJTYPE_TRANSPARENCY_ALL,
#else
					| NND_SUBOBJTYPE_OPAQUE | NND_SUBOBJTYPE_PUNCHTHROUGH,
#endif
				param->flag | drawflag | amDrawGetState()->drawflag);
	}

	// マテリアルコールバック設定クリア
	if (draw_param->material_cb_func) {
		objDraw3DNNSetMaterialCallback(NULL, NULL);
#if _WII
		GXSetTevDirect(GX_TEVSTAGE0);
#endif
	}

	// 追加 描画ステータス復帰
	if (draw_param->state) {
		amDrawPopState();
	}

	// ライト復帰
	if (g_obj.def_user_light_flag ^ draw_param->use_light_flag) {
		// 標準ライトと違いがある
		objDrawSetDefaultLight();
	}

	// 半透明描画登録
#if !OBD_DRAW_MODEL_ALL
#if NND_NEW_OBJECT_FORMAT
	if (param->object->fType & NND_OBJTYPE_TRANSPARENT) {
#else
	if (1) {
#endif
		//AMS_COMMAND_HEADER			*command_sort;
		//AMS_PARAM_SORT_DRAW_OBJECT	*param_sort;
		//AMS_DRAWSTATE				*draw_state;

		command_sort	= &sort_draw_param->cmd_header;
		param_sort		= &sort_draw_param->param;
		draw_state		= &sort_draw_param->state;

		if (draw_param->state) {
			// 追加 不透明描画時のステートを引き継ぎ
			MI_CpuCopy8(draw_param->state, draw_state, sizeof(AMS_DRAWSTATE));
		}
		else {
			amDrawGetState(draw_state);
		}

		param_sort->drawflag	= drawflag;
		param_sort->draw_object	= (AMS_PARAM_DRAW_OBJECT *)param;
		param_sort->mtx			= plt_mtx;
		param_sort->nstat_list	= nstat;
		param_sort->draw_state	= draw_state;
#if _WII
		param_sort->vtx_list	= vtx_list;
#endif
		if (command->command_id == OBD_DRAW_USER_COMMAND_3DNN_DRAW_MOTION) {
			command_sort->command_id	= OBD_DRAW_USER_COMMAND_SORT_3DNN_MODEL;
		}
		else {
			command_sort->command_id	= OBD_DRAW_USER_COMMAND_SORT_3DNN_MATMTN;
		}
		command_sort->param			= param_sort;

		// ライト
		sort_draw_param->use_light_flag = draw_param->use_light_flag;

#if _PS3 | _XBOX | _PC
		// トゥーンリムライト
		sort_draw_param->toon_rim_param = draw_param->toon_rim_param;
		// 迷彩
		sort_draw_param->toon_camouflage = draw_param->toon_camouflage;
#endif

#if _WII
		// Wii用トゥーンライト設定
		sort_draw_param->toon_light = draw_param->toon_light;
#endif

		// ユーザー処理
		sort_draw_param->user_func	= draw_param->user_func;
		sort_draw_param->user_param	= draw_param->user_param;

		// マテリアルコールバック
		sort_draw_param->material_cb_func	= draw_param->material_cb_func;
		sort_draw_param->material_cb_param	= draw_param->material_cb_param;

		amDrawAddSort(command_sort,
				(Sint32)((param->object->Radius - NNM_MTX(base_mtx, 2, 3))
				* 100.0f));
		//amDrawAddSort(command_sort,
		//	(Sint32)((param->object->Radius * param->scaleZ - NNM_MTX(base_mtx, 2, 3))
		//		* 100.0f));
	}
#endif // #if !OBD_DRAW_MODEL_ALL

	amMatrixPop();
}

// ==========================================================================
// 描画コマンド (ソート)
// ==========================================================================
// ==========================================================================
// objDraw3DNNModelCommandSortFunc
/*!
*	オブジェクト3Dモデル コマンド(ソート)処理関数
 *
 *	@param command		[in]	コマンドワーク
 *	@param drawflag		[in]	描画フラグ NNF_DRAWOBJ
 */
// ==========================================================================
void objDraw3DNNModelCommandSortFunc(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	MTM_ASSERT(command);

	obj_draw_user_command_sort_func_tbl[command->command_id](command, drawflag);
}

// ==========================================================================
// objDraw3DNNSortModel_DT
/*!
 *	オブジェクト3D ソート 描画 (描画スレッド処理)
 *
 *	@param command		[in]	コマンドワーク
 *	@param drawflag		[in]	描画フラグ NNF_DRAWOBJ
 */
// ==========================================================================
void objDraw3DNNSortModel_DT(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag)
{
	// ◆ _amDrawSortObjectのコピー 改変
	OBS_DRAW_PARAM_3DNN_SORT_MODEL	*sort_draw_param;
	AMS_PARAM_SORT_DRAW_OBJECT		*param_sort;
	AMS_PARAM_DRAW_OBJECT			*param;

	UNREFERENCED_PARAMETER(drawflag);

	amMatrixPush();

	sort_draw_param	= (OBS_DRAW_PARAM_3DNN_SORT_MODEL *)command;//->param;

	// ライト設定
	if (g_obj.def_user_light_flag ^ sort_draw_param->use_light_flag) {
		// 標準ライトと違いがある
		objDrawSetDrawLight(sort_draw_param->use_light_flag);
	}

	// ユーザー処理
	if (sort_draw_param->user_func) {
		sort_draw_param->user_func(sort_draw_param->user_param);
	}

	// 半透明描画
	param_sort		= &sort_draw_param->param;
	param			= param_sort->draw_object;

#if _WII
	nnSetPliableVertexBufferGC(param_sort->vtx_list, &obj_draw_unit_matrix);
#endif

	if (param->texlist != NULL)
		nnSetTextureList(param->texlist);

	// 描画ステータス設定
	if (param_sort->draw_state != NULL) {
		amDrawPushState();
		amDrawSetState(param_sort->draw_state);
	}

	// マテリアルコールバック設定
	objDraw3DNNSetMaterialCallback(sort_draw_param->material_cb_func, sort_draw_param->material_cb_param);

	// トゥーンリムライト設定
#if _PS3 | _XBOX | _PC
	if ((param->flag | drawflag | amDrawGetState()->drawflag) & NND_DRAWOBJ_SHADER_USER_PROFILE_TOON) {
#if _PC | _XBOX
		nnSetUserUniformDXG20(0,
			sort_draw_param->toon_rim_param.r, sort_draw_param->toon_rim_param.g, sort_draw_param->toon_rim_param.b,
			sort_draw_param->toon_camouflage);
#elif _PS3
		nnSetUserUniformPS3(0,
			sort_draw_param->toon_rim_param.r, sort_draw_param->toon_rim_param.g, sort_draw_param->toon_rim_param.b,
			sort_draw_param->toon_camouflage);
#endif
	}
#endif

	// Wii用トゥーンライト設定
#if _WII
	_am_draw_toonDir = sort_draw_param->toon_light;
#endif

	if (command->command_id == OBD_DRAW_USER_COMMAND_SORT_3DNN_MODEL) {
		nnDrawObject(
				param->object, param_sort->mtx, param_sort->nstat_list,
				param->sub_obj_type
						| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
						| NND_SUBOBJTYPE_TRANSPARENT,
				param->flag | param_sort->drawflag | amDrawGetState()->drawflag);
	} else {
		nnDrawMaterialMotionObject(
				(NNS_MATMOTOBJ *)param->object, param_sort->mtx, param_sort->nstat_list,
				param->sub_obj_type
						| NND_SUBOBJTYPE_RIGID | NND_SUBOBJTYPE_PLIABLE
						| NND_SUBOBJTYPE_TRANSPARENT,
				param->flag | param_sort->drawflag | amDrawGetState()->drawflag);
	}

	// マテリアルコールバック設定クリア
	objDraw3DNNSetMaterialCallback(NULL, NULL);

#if _WII
	nnSetPliableVertexBufferGC(NULL, &obj_draw_unit_matrix);
#endif

	// 追加 描画ステータス復帰
	if (param_sort->draw_state) {
		amDrawPopState();
	}

	// ライト復帰
	if (g_obj.def_user_light_flag ^ sort_draw_param->use_light_flag) {
		// 標準ライトと違いがある
		objDrawSetDefaultLight();
	}

	amMatrixPop();
}


// ==========================================================================
// マテリアルコールバック
// ==========================================================================
// ==========================================================================
// objDraw3DNNMaterialCallback
/*!
 *	オブジェクト3D NN マテリアルコールバック
 *
 *	@param tcb		[in]	TCB
 */
// ==========================================================================
void objDraw3DNNSetMaterialCallback(OBF_MATERIAL_CB cb_func, void *cb_param)
{
	obj_draw_material_cb_func	= cb_func;
	obj_draw_material_cb_param	= cb_param;
	if (cb_func) {
		// コールバック設定
		nnSetMaterialCallback(objDraw3DNNMaterialCallback);
		if (nnGetMaterialCallback() != objDraw3DNNMaterialCallback) {
			MTM_ASSERT(0);
		}
	}
	else {
		// コールバッククリア
		nnSetMaterialCallback(NULL);
		
#if _WII
		GXSetTevDirect(GX_TEVSTAGE0);
#endif
	}
}

// ==========================================================================
// objDraw3DNNMaterialCallback
/*!
 *	オブジェクト3D NN マテリアルコールバック
 *
 *	@param tcb		[in]	TCB
 */
// ==========================================================================
NNE_BOOL objDraw3DNNMaterialCallback(NNS_DRAWCALLBACK_VAL *draw_cb_val)
{
	if (obj_draw_material_cb_func) {
		// ユーザーコールバックへ
		return (obj_draw_material_cb_func(draw_cb_val, obj_draw_material_cb_param));
	}
	return (nnPutMaterialCore(draw_cb_val));
}

// ==========================================================================
// ライト設定
// ==========================================================================
// ==========================================================================
// objDrawSetDrawLight
/*!
 *	オブジェクト3D NN ライト設定
 *
 *	@param use_light_flag		[in]	使用ライトフラグ
 *
 *	@note
 *		描画時のライト設定を行います。\n
 *		drawスレッド処理です \n
 *		use_light_flagの設定がない場合は、すべてOFFになります。
 */
// ==========================================================================
void objDrawSetDrawLight(u32 use_light_flag)
{
	s32	i;

	for (i = 0; i < NNE_LIGHT_MAX; i++) {
		if (use_light_flag & (1 << i)) {
			nnSetLight((NNE_LIGHT)i, &g_obj.light[i].light_param, g_obj.light[i].light_type);
			nnSetLightSwitch((NNE_LIGHT)i, NNE_ON);
		}
		else {
			nnSetLightSwitch((NNE_LIGHT)i, NNE_OFF);
		}
	}

	// ライト反映
	nnPutLightSettings();
}

// ==========================================================================
// objDrawSetDefaultLight
/*!
 *	オブジェクト3D NN 標準ライト設定
 *
 *	@note
 *		標準ライト設定を行います。\n
 *		drawスレッド処理です
 */
// ==========================================================================
void objDrawSetDefaultLight(void)
{
	s32	i;

	for (i = 0; i < NNE_LIGHT_MAX; i++) {
		if (g_obj.def_user_light_flag & (1 << i)) {
			nnSetLight((NNE_LIGHT)i, &g_obj.light[i].light_param, g_obj.light[i].light_type);
			nnSetLightSwitch((NNE_LIGHT)i, NNE_ON);
		}
		else {
			nnSetLightSwitch((NNE_LIGHT)i, NNE_OFF);
		}
	}

	// ライト反映
	nnPutLightSettings();
}

#endif // #if (OBD_USE_ACTION3D_NN)



#if (OBD_USE_ACTION3D_ES)
// ==========================================================================
// objDraw3DESEffectServerMain
/*!
 *	オブジェクト3D ES エフェクトサーバー メイン関数（メインスレッド）
 *
 *	@param tcb		[in]	TCB
 */
// ==========================================================================
void objDraw3DESEffectServerMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

    amEffectExecute();
}

// ==========================================================================
// objDraw3DESMatrixPush_UserFunc
/*!
 *	オブジェクト3D ES マトリックスプッシュ ユーザ関数（描画スレッド）
 *
 *	@param param		[in]	プッシュするマトリックス(NNS_MATRIX)
 */
// ==========================================================================
void objDraw3DESMatrixPush_UserFunc(void *param)
{
	NNS_MATRIX	mtx;
	NNS_MATRIX	*cur_mtx;
	NNS_MATRIX	param_mtx;
	
	amMatrixPush();
	
	cur_mtx	= amMatrixGetCurrent();
	
	// paramをNNS_MATRIXにキャストして直接参照してMultiplyMatrixの引数にすると
	// 結果が上手く得られないため、一旦別の変数にコピーする。
	memcpy(&param_mtx, param, sizeof(NNS_MATRIX));
	
	// 描画スレッド側のカレントマトリクスにワールドマトリクスを積んでおく
	// （amDrawPrimitive3D()で参照されるため。
	//  ビューマトリクスは積まない）
	nnMultiplyMatrix(cur_mtx, cur_mtx, &param_mtx);
	
	nnMultiplyMatrix(&mtx, amDrawGetWorldViewMatrix(), cur_mtx);
	
	// 3Dプリミティブ描画用マトリックスをセット（ビューマトリクス込み）
	nnSetPrimitive3DMatrix(&mtx);
}

// ==========================================================================
// objDraw3DESMatrixPop_UserFunc
/*!
 *	オブジェクト3D ES マトリックスポップ ユーザ関数（描画スレッド）
 */
// ==========================================================================
void objDraw3DESMatrixPop_UserFunc(void *param)
{
	UNREFERENCED_PARAMETER(param);
	amMatrixPop();
}
#endif // #if (OBD_USE_ACTION3D_ES)


#if (OBD_USE_ACTION2D_AMA)
// ==========================================================================
// objDraw2DAMAPre_DT
/*!
 *	オブジェクトアクション 2D AMA 前処理
 */
// ==========================================================================
void objDraw2DAMAPre_DT(void *param)
{
	UNREFERENCED_PARAMETER(param);

	AoActDrawPre();

	// 描画呼び出しは ObjDrawNNStart で行う
	//amDrawExecCommand(OBD_DRAW_CMD_STATE_2DAMA);
	//amDrawEndScene();
}

#endif	// #if (OBD_USE_ACTION2D_AMA)


// ================================================================
// objActDivisionDraw
/*!
  分割アクションを描画する
  
  @param    act     [in] 対象アクション
  @param    pOffst  [in] 座標テーブル XYXYXY… 1:15

  @note
    pOffstのテーブル数に注意してください\n
    実際の絵のサイズではなく、スプライトのサイズ分使用します\n
    足りなかった場合、フローした箇所のメモリを参照します（表示位置がおかしくなります）\n
    OPEでは正しく動作しません\n
 */
// ================================================================
/*static void objActDivisionDraw( MTS_ACTION *act, s16 *pOffst )
{
    MTS_GE2_CONTEXT     *ge = _mt_ge2_table[act->ge_type];
    const MTS_ACT_BAC_CONVERSION_PATTERN  *cnv_ptn  =
        (const MTS_ACT_BAC_CONVERSION_PATTERN*)( (u32)mtActGetConversionSectionFromBac( act->bac_addr ) + act->cnv_ofst );
    s32     center_pos[MTD_XY];     // 表示中心座標
    s32     size[MTD_RECT];         // ローカル座標系でのパターン全体サイズ
    u32     cha_name;               // 開始キャラクタネーム
    u16     attrib0_flag    = 0;    // アトリビュート0のフラグ
    u16     attrib1_flag    = 0;    // アトリビュート1のフラグ
    u8     oam_max_size_x      = 0;    // 最大横サイズ
    u8     oam_max_size_y      = 0;    // 最大縦サイズ

#if defined(MTD_DEBUG)
    // スプライトで扱えるキャラクタフォーマットかチェックする
    switch( mtActGetActionSectionFromBac( act->bac_addr )->info[act->act_id].cha_format )
    {
    case MTE_CHA_FORMAT_16:
    case MTE_CHA_FORMAT_256:
    case MTE_CHA_FORMAT_DIRECT:
        break;
    default:
        MTM_ASSERT( !"mtAction::mtActDraw() Error! Invalid character format!\n" );
        return;
    }
#endif  // #endif of #if defined(MTD_DEBUG)

    // 表示するオブジェクトが無ければ何もせずに終了する
    if( 0 == cnv_ptn->num_obj )
        return ;

    // 表示中心座標を求める
    center_pos[MTD_X]   = act->pos[MTD_X];
    center_pos[MTD_Y]   = act->pos[MTD_Y];
    size[MTD_LEFT]      = (s32)cnv_ptn->size[MTD_LEFT];
    size[MTD_TOP]       = (s32)cnv_ptn->size[MTD_TOP];
    size[MTD_RIGHT]     = (s32)cnv_ptn->size[MTD_RIGHT];
    size[MTD_BOTTOM]    = (s32)cnv_ptn->size[MTD_BOTTOM];

    // 必要なOAMの数を計算
    oam_max_size_x =(u8)(((size[MTD_RIGHT] - size[MTD_LEFT]) / (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ) );
    oam_max_size_y =(u8)(((size[MTD_BOTTOM] - size[MTD_TOP]) >> 3) );

    if ( !oam_max_size_x )
        oam_max_size_x = 1;
    if ( !oam_max_size_y )
        oam_max_size_y = 1;
    
    if( MTD_ACT_FLAG_ENABLE_OFFSET & act->flag )
    {
        center_pos[MTD_X]   += ge->spr_offset[MTD_X];
        center_pos[MTD_Y]   += ge->spr_offset[MTD_Y];
    }

    // フリップを設定する
    if( MTD_ACT_FLAG_FLIP_H & act->flag )
    {
        attrib1_flag        |= MTD_OBJ_ATTR1_FLIP_H;
        size[MTD_LEFT]      = -(s32)cnv_ptn->size[MTD_RIGHT];
        size[MTD_RIGHT]     = -(s32)cnv_ptn->size[MTD_LEFT];
    }
    if( MTD_ACT_FLAG_FLIP_V & act->flag )
    {
        attrib1_flag        |= MTD_OBJ_ATTR1_FLIP_V;
        size[MTD_TOP]       = -(s32)cnv_ptn->size[MTD_BOTTOM];
        size[MTD_BOTTOM]    = -(s32)cnv_ptn->size[MTD_TOP];
    }

    // モザイクを設定する
    if( MTD_ACT_FLAG_MOSAIC & act->flag )
        attrib0_flag    |= MTD_OBJ_ATTR0_MOSAIC;

    // OBJ-VRAMアドレスから基準キャラクタネームを求める
    if( MTD_OBJ_ATTR0_MODE_BMP_OBJ == ( MTD_OBJ_ATTR0_MODE_BMP_OBJ & cnv_ptn->obj_info[0].attribute[0] ) )
        cha_name    = MTM_ACT_BMP_CHA_NAME_FROM_OFST(
                        act->cha_addr - _mt_obj_vram_addr[act->ge_type], ge );
    else
        cha_name    = MTM_ACT_CHA_NAME_FROM_OFST(
                        act->cha_addr - _mt_obj_vram_addr[act->ge_type], ge );

    // 各オブジェ情報からOAMソートを作成する
    {
        const MTS_ACT_BAC_CONVERSION_OBJ    *obj_info   = cnv_ptn->obj_info;
        const u16   *obj_size;          // オブジェサイズ
        s32         obj_pos[MTD_XY];    // オブジェ座標
        MTS_SPR_OAM_SORT    *oam_sort;
        u32                 dest_name_shift;    // キャラクタネームシフト回数
        u32                 i,j,k,l = 0;
        u8 cha_name_add = 0;
        u8     oam_size_x      = 0;    // 横サイズ
        u8     oam_size_y      = 0;    // 縦サイズ
        
        // キャラクタフォーマットに対応したキャラクタネームシフト回数を求める
        if( MTD_OBJ_ATTR0_MODE_BMP_OBJ == ( MTD_OBJ_ATTR0_MODE_BMP_OBJ & cnv_ptn->obj_info[0].attribute[0] ) )
            dest_name_shift = _mt_ge2_table[act->ge_type]->vram_obj_bmp_unit_shift;
        else
            dest_name_shift = _mt_ge2_table[act->ge_type]->vram_obj_unit_shift;

        for( k = 0; k < cnv_ptn->num_obj; ++k, ++obj_info ) {
            // オブジェサイズを求める
            obj_size    =
                _mt_act_obj_size[ MTM_ACT_TBL_INDEX_FROM_SHAPE_SIZE( obj_info->attribute[0], obj_info->attribute[1] ) ];

            // サイズから繰り返し数を取得
            oam_size_x =(u8)((obj_size[MTD_WIDTH ] / (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ) );
            oam_size_y =(u8)((obj_size[MTD_HEIGHT] >> 3) );

            if ( !oam_size_x )
                oam_size_x = 1;
            if ( !oam_size_y )
                oam_size_y = 1;
            cha_name_add = 0;

            // 現表示物座標からグローバル座標カウンタを設定する 
            l = 0;

            
            // このスプライトを分割して登録する
            for( j = 0; j < oam_size_y; ++j ){
                
                l = (u8)((( ( (s32)obj_info->attribute[1] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) >> 3) >> _mt_ge2_table[act->ge_type]->vram_obj_unit_shift);
                l += (u8)(((( ( (s32)obj_info->attribute[0] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) >> 3) + j) * oam_max_size_x);
                
                for( i = 0; i < oam_size_x; ++i ){
                    
                    // フリップ時を考慮しつつオブジェの座標を求める
                    if( MTD_OBJ_ATTR1_FLIP_H & attrib1_flag )
                        obj_pos[MTD_X]  = (s32)(center_pos[MTD_X] + pOffst[l * 2 + MTD_X] + cnv_ptn->center[MTD_X] - ( ( (s32)obj_info->attribute[1] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) - ( i * ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift ) ) - (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift));
                    else
                        obj_pos[MTD_X]  = (s32)(center_pos[MTD_X] + pOffst[l * 2 + MTD_X] - cnv_ptn->center[MTD_X] + ( ( (s32)obj_info->attribute[1] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) + ( i * ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ));
                    if( MTD_OBJ_ATTR1_FLIP_V & attrib1_flag )
                        obj_pos[MTD_Y]  = (s32)(center_pos[MTD_Y] + pOffst[l * 2 + MTD_Y] + cnv_ptn->center[MTD_Y] - ( ( (s32)obj_info->attribute[0] << ( 32 - 8 ) ) >> ( 32 - 8 ) ) - ( j * 8 ) - 8);
                    else
                        obj_pos[MTD_Y]  = (s32)(center_pos[MTD_Y] + pOffst[l * 2 + MTD_Y] - cnv_ptn->center[MTD_Y] + ( ( (s32)obj_info->attribute[0] << ( 32 - 8 ) ) >> ( 32 - 8 ) ) + ( j * 8 ));

                    // オブジェ1つ1つをクリッピングチェックする
                    if( MTD_ACT_FLAG_ENABLE_CLIP & act->flag ) {
                        if(
                            0 >= obj_pos[MTD_X] + ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift )
                            || OBD_LCD_X <= obj_pos[MTD_X]
                            || 0 >= obj_pos[MTD_Y] + 8
                            || OBD_LCD_Y <= obj_pos[MTD_Y] )
                            {
                                // 画面外にあるため描画する必要なし
                                ++cha_name_add;
                                ++l;
                                continue;
                            }
                    }

                    // OAMソートに登録する
                    oam_sort    = mtSprGetOamSort( act->ge_type, act->priority );

                    oam_sort->attribute[0]  = (u16)( 1 << MTD_OBJ_ATTR0_SHAPE_SHIFT| ( obj_pos[MTD_Y] & MTD_OBJ_ATTR0_Y_MASK ) );
                    oam_sort->attribute[0]  ^= attrib0_flag;

                    oam_sort->attribute[1]  = (u16)( ( obj_pos[MTD_X] & MTD_OBJ_ATTR1_X_MASK ) );
                    oam_sort->attribute[1]  ^= attrib1_flag;

                    oam_sort->attribute[2]  = (u16)(
                        ( ( (( ( obj_info->attribute[2] & MTD_OBJ_ATTR2_NAME_MASK ) + ( 1 << dest_name_shift ) - 1 ) >> dest_name_shift)+ cha_name + cha_name_add ) & MTD_OBJ_ATTR2_NAME_MASK )
                        | ( ( ( act->flag & MTD_ACT_FLAG_BG_NO_MASK ) >> MTD_ACT_FLAG_BG_NO_SHIFT ) << MTD_OBJ_ATTR2_PRIORITY_BG_SHIFT )
                        | ( ( ( act->plt_ofst_no << MTD_OBJ_ATTR2_PALETTE_SHIFT ) ) & MTD_OBJ_ATTR2_PALETTE_MASK ) );
                    ++cha_name_add;
                    ++l;
                }
            }
        }
    }
}*/

// ================================================================
// objActDivisionDrawAffine
/*!
  分割アクションを拡縮描画する
  
  @param    act     [in] 対象アクション
  @param    pOffst  [in] 座標テーブル XYXYXY… 1:15
  @param    scale_x [in] Ｘ方向のスケール
  @param    scale_y [in] Ｙ方向のスケール

  @note
    pOffstのテーブル数に注意してください\n
    実際の絵のサイズではなく、スプライトのサイズ分使用します\n
    足りなかった場合、フローした箇所のメモリを参照します（表示位置がおかしくなります）\n
    OPEでは正しく動作しません\n
    回転は使用できません\n
 */
// ================================================================
/*static void objActDivisionDrawAffine( MTS_ACTION *act, s16 *pOffst, fx32 scale_x, fx32 scale_y )
{
    MTS_GE2_CONTEXT     *ge = _mt_ge2_table[act->ge_type];
    const MTS_ACT_BAC_CONVERSION_PATTERN  *cnv_ptn  =
        (const MTS_ACT_BAC_CONVERSION_PATTERN*)( (u32)mtActGetConversionSectionFromBac( act->bac_addr ) + act->cnv_ofst );
    MtxFx22 affine_flip[4];         // フリップを考慮したアフィン変換
    MtxFx22 affine_inv_flip[4];     // フリップを考慮したアフィン変換

    s32     center_pos[MTD_XY];     // 表示中心座標
    s32     size[MTD_RECT];         // ローカル座標系でのパターン全体サイズ
    u32     cha_name;               // 開始キャラクタネーム
    u16     attrib0_flag    = 0;    // アトリビュート0のフラグ
    u16     attrib1_flag    = 0;    // アトリビュート1のフラグ
    u8     oam_max_size_x      = 0;    // 最大横サイズ
    u8     oam_max_size_y      = 0;    // 最大縦サイズ

#if defined(MTD_DEBUG)
    // スプライトで扱えるキャラクタフォーマットかチェックする
    switch( mtActGetActionSectionFromBac( act->bac_addr )->info[act->act_id].cha_format )
    {
    case MTE_CHA_FORMAT_16:
    case MTE_CHA_FORMAT_256:
    case MTE_CHA_FORMAT_DIRECT:
        break;
    default:
        MTM_ASSERT( !"mtAction::mtActDraw() Error! Invalid character format!\n" );
        return;
    }
#endif  // #endif of #if defined(MTD_DEBUG)

    // 表示するオブジェクトが無ければ何もせずに終了する
    if( 0 == cnv_ptn->num_obj )
        return ;

    // スケールがゼロに近い場合は描画しない
    if( MTD_MATH_AFFINE_SCALE_MIN > MTM_MATH_ABS( scale_x ) || MTD_MATH_AFFINE_SCALE_MIN > MTM_MATH_ABS( scale_y ) )
        return ;

    // アフィンパラメータが不足している場合は通常描画に切り替える
    // 最高でも通常、H、V、HVフリップの４パターンが必要になる
    {
        u16     need_ap_max = 4 > cnv_ptn->num_obj ? cnv_ptn->num_obj  : (u16)4;

        if( ge->spr_affine_base + ge->spr_affine_num + need_ap_max >= 32 )
        {
#if defined(MTD_DEBUG)
            if( MTD_DEBUG_ASSERT_AFFINE_OVER & _mt_debug_flag )
            {
                MTM_ASSERT( !"mtAction::mtActDrawAffine() Error! Affine parameter limit!\n" );
            }
#endif  // #endif of #if defined(MTD_DEBUG)
            mtActDraw( act );
            return;
        }
    }

    // 全パターンのアフィンを作成する
    //  この行列はアフィン行列ではなくAGBの回転拡縮パラメータのためスケールを逆数で指定する
    {
        fx32    scale_x_inv = FX_Inv( scale_x );
        fx32    scale_y_inv = FX_Inv( scale_y );

        MTX_Identity22( &affine_flip[0] );

        // Hフリップ
        MTX_ScaleApply22( &affine_flip[0], &affine_inv_flip[1], -scale_x_inv, scale_y_inv );
        MTX_ScaleApply22( &affine_flip[0], &affine_flip[1],     -scale_x,     scale_y );

        // Vフリップ
        MTX_ScaleApply22( &affine_flip[0], &affine_inv_flip[2], scale_x_inv, -scale_y_inv );
        MTX_ScaleApply22( &affine_flip[0], &affine_flip[2],     scale_x,     -scale_y );

        // HVフリップ
        MTX_ScaleApply22( &affine_flip[0], &affine_inv_flip[3], -scale_x_inv, -scale_y_inv );
        MTX_ScaleApply22( &affine_flip[0], &affine_flip[3],     -scale_x,     -scale_y );

        // フリップなし
        MTX_ScaleApply22( &affine_flip[0], &affine_inv_flip[0], scale_x_inv, scale_y_inv );
        MTX_ScaleApply22( &affine_flip[0], &affine_flip[0],     scale_x,     scale_y );
    }


    // 表示中心座標を求める
    center_pos[MTD_X]   = act->pos[MTD_X];
    center_pos[MTD_Y]   = act->pos[MTD_Y];
    size[MTD_LEFT]      = (s32)cnv_ptn->size[MTD_LEFT];
    size[MTD_TOP]       = (s32)cnv_ptn->size[MTD_TOP];
    size[MTD_RIGHT]     = (s32)cnv_ptn->size[MTD_RIGHT];
    size[MTD_BOTTOM]    = (s32)cnv_ptn->size[MTD_BOTTOM];

    // 必要なOAMの数を計算
    oam_max_size_x =(u8)(((size[MTD_RIGHT] - size[MTD_LEFT]) / (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ) );
    oam_max_size_y =(u8)(((size[MTD_BOTTOM] - size[MTD_TOP]) >> 3) );

    if ( !oam_max_size_x )
        oam_max_size_x = 1;
    if ( !oam_max_size_y )
        oam_max_size_y = 1;
    
    if( MTD_ACT_FLAG_ENABLE_OFFSET & act->flag )
    {
        center_pos[MTD_X]   += ge->spr_offset[MTD_X];
        center_pos[MTD_Y]   += ge->spr_offset[MTD_Y];
    }

    // フリップを設定する
    if( MTD_ACT_FLAG_FLIP_H & act->flag )
    {
        attrib1_flag        |= MTD_OBJ_ATTR1_FLIP_H;
        size[MTD_LEFT]      = -(s32)cnv_ptn->size[MTD_RIGHT];
        size[MTD_RIGHT]     = -(s32)cnv_ptn->size[MTD_LEFT];
    }
    if( MTD_ACT_FLAG_FLIP_V & act->flag )
    {
        attrib1_flag        |= MTD_OBJ_ATTR1_FLIP_V;
        size[MTD_TOP]       = -(s32)cnv_ptn->size[MTD_BOTTOM];
        size[MTD_BOTTOM]    = -(s32)cnv_ptn->size[MTD_TOP];
    }

    // モザイクを設定する
    if( MTD_ACT_FLAG_MOSAIC & act->flag )
        attrib0_flag    |= MTD_OBJ_ATTR0_MOSAIC;

    // 倍角を設定する
    if( MTD_ACT_FLAG_BAIKAKU & act->flag )
        attrib0_flag    |= MTD_OBJ_ATTR0_BAIKAKU;

    // OBJ-VRAMアドレスから基準キャラクタネームを求める
    if( MTD_OBJ_ATTR0_MODE_BMP_OBJ == ( MTD_OBJ_ATTR0_MODE_BMP_OBJ & cnv_ptn->obj_info[0].attribute[0] ) )
        cha_name    = MTM_ACT_BMP_CHA_NAME_FROM_OFST(
                        act->cha_addr - _mt_obj_vram_addr[act->ge_type], ge );
    else
        cha_name    = MTM_ACT_CHA_NAME_FROM_OFST(
                        act->cha_addr - _mt_obj_vram_addr[act->ge_type], ge );

    // 各オブジェ情報からOAMソートを作成する
    {
        const MTS_ACT_BAC_CONVERSION_OBJ    *obj_info   = cnv_ptn->obj_info;
        const u16   *obj_size;          // オブジェサイズ
        s32         obj_pos[MTD_XY];    // オブジェ座標
        fx32        local_pos[MTD_XY];  // スプライト回転時のローカル座標
        u16         use_affine_param[4] = { (u16)-1, (u16)-1, (u16)-1, (u16)-1 };
        MTS_SPR_OAM_SORT    *oam_sort;
        u32                 dest_name_shift;    // キャラクタネームシフト回数
        u32                 i,j,k,l = 0;
        u32                 affine_index;
        u8 cha_name_add = 0;
        u8     oam_size_x      = 0;    // 横サイズ
        u8     oam_size_y      = 0;    // 縦サイズ
        
        // キャラクタフォーマットに対応したキャラクタネームシフト回数を求める
        if( MTD_OBJ_ATTR0_MODE_BMP_OBJ == ( MTD_OBJ_ATTR0_MODE_BMP_OBJ & cnv_ptn->obj_info[0].attribute[0] ) )
            dest_name_shift = _mt_ge2_table[act->ge_type]->vram_obj_bmp_unit_shift;
        else
            dest_name_shift = _mt_ge2_table[act->ge_type]->vram_obj_unit_shift;

        for( k = 0; k < cnv_ptn->num_obj; ++k, ++obj_info ) {
            // オブジェサイズを求める
            obj_size    =
                _mt_act_obj_size[ MTM_ACT_TBL_INDEX_FROM_SHAPE_SIZE( obj_info->attribute[0], obj_info->attribute[1] ) ];

            // サイズから繰り返し数を取得
            oam_size_x =(u8)((obj_size[MTD_WIDTH ] / (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ) );
            oam_size_y =(u8)((obj_size[MTD_HEIGHT] >> 3) );

            if ( !oam_size_x )
                oam_size_x = 1;
            if ( !oam_size_y )
                oam_size_y = 1;
            cha_name_add = 0;

            // 現表示物座標からグローバル座標カウンタを設定する 
            l = 0;

            
            // このスプライトを分割して登録する
            for( j = 0; j < oam_size_y; ++j ){
                
                l = (u8)((( ( (s32)obj_info->attribute[1] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) >> 3) >> _mt_ge2_table[act->ge_type]->vram_obj_unit_shift);
                l += (u8)(((( ( (s32)obj_info->attribute[0] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) >> 3) + j) * oam_max_size_x);
                
                // 使用するアフィンのインデックスを求める
                affine_index    = (u32)( ( ( attrib1_flag ^ obj_info->attribute[1] ) & ( MTD_OBJ_ATTR1_FLIP_H | MTD_OBJ_ATTR1_FLIP_V ) )
                                         >> MTD_OBJ_ATTR1_FLIP_SHIFT );
                for( i = 0; i < oam_size_x; ++i ){
                    
                    local_pos[MTD_X]    = (fx32)( ( ( (fx32)obj_info->attribute[1] << ( 32 - 9 ) ) >> ( 32 - 9 ) )    // アトリビュートの座標を符号拡張
                                            - cnv_ptn->center[MTD_X] + pOffst[l * 2 + MTD_X]
                                            + ( i * ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) )
                                            + ( (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) >> 1) )  << FX32_DEC_SIZE;
                    
                    local_pos[MTD_Y]    = (fx32)( ( ( (fx32)obj_info->attribute[0] << ( 32 - 8 ) ) >> ( 32 - 8 ) )    // アトリビュートの座標を符号拡張
                                            - cnv_ptn->center[MTD_Y] + pOffst[l * 2 + MTD_Y]
                                            + ( j * 8 )
                                            + ( 8 >> 1 ))   << FX32_DEC_SIZE;
                        
                    obj_pos[MTD_X]  = center_pos[MTD_X]     // 表示座標
                        - ( (fx32)( ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ) >> 1 )    // 半スプライトサイズ分ずらしたので元に戻している
                            + (( FX_Mul( affine_flip[affine_index]._00, local_pos[MTD_X] ) + FX_Mul( affine_flip[affine_index]._10, local_pos[MTD_Y] ))>> FX32_DEC_SIZE );    // ローカル座標を回転拡縮している
                    obj_pos[MTD_Y]  = center_pos[MTD_Y]     // 表示座標
                        - ( (fx32)( 8 ) >> 1 )    // 半スプライトサイズ分ずらしたので元に戻している
                            + (( FX_Mul( affine_flip[affine_index]._01, local_pos[MTD_X] ) + FX_Mul( affine_flip[affine_index]._11, local_pos[MTD_Y] ) )>> FX32_DEC_SIZE );    // ローカル座標を回転拡縮している

                    // 倍角している場合は座標を補正する
                    if( MTD_OBJ_ATTR0_BAIKAKU & attrib0_flag ) {
                        obj_pos[MTD_X]  -= (fx32)obj_size[MTD_X] << ( FX32_DEC_SIZE - 1 );
                        obj_pos[MTD_Y]  -= (fx32)obj_size[MTD_Y] << ( FX32_DEC_SIZE - 1 );
                        *(u32*)obj_size <<= 1;
                    }
                    // オブジェ1つ1つをクリッピングチェックする
                    if( MTD_ACT_FLAG_ENABLE_CLIP & act->flag ) {
                        if(
                            0 >= obj_pos[MTD_X] + ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift )
                            || OBD_LCD_X <= obj_pos[MTD_X]
                            || 0 >= obj_pos[MTD_Y] + 8
                            || OBD_LCD_Y <= obj_pos[MTD_Y] )
                            {
                                // 画面外にあるため描画する必要なし
                                ++cha_name_add;
                                ++l;
                                continue;
                            }
                    }
                    // まだ設定していないアフィンであればパラメータを設定する
                    if( 0 > (s16)use_affine_param[affine_index] ) {

                        use_affine_param[affine_index]  =
                            mtSprGetAffine( act->ge_type, &affine_inv_flip[affine_index] );

                        if( 0 <= (s16)use_affine_param[affine_index] )
                            {
                                G2_SetOBJAffine(
                                    (GXOamAffine*)&ge->spr_oam_buffer[use_affine_param[affine_index] << 2],
                                    &affine_inv_flip[affine_index] );
                            }
                        else
                            {
                                // アフィンパラメータが足りなかったため描画をあきらめる
                                ++cha_name;continue;
                            }
                    }

                    // OAMソートに登録する
                    oam_sort    = mtSprGetOamSort( act->ge_type, act->priority );

                    oam_sort->attribute[0]  = (u16)( 1 << MTD_OBJ_ATTR0_SHAPE_SHIFT| ( obj_pos[MTD_Y] & MTD_OBJ_ATTR0_Y_MASK ) | MTD_OBJ_ATTR0_ZOOM);
                    oam_sort->attribute[0]  ^= attrib0_flag;

                    oam_sort->attribute[1]  = (u16)( ( obj_pos[MTD_X] & MTD_OBJ_ATTR1_X_MASK ) );
                    oam_sort->attribute[1]  = (u16)( ( oam_sort->attribute[1] & ~MTD_OBJ_ATTR1_ZOOM_PARAM )
                                                     | ( use_affine_param[affine_index] << MTD_OBJ_ATTR1_ZOOM_PARAM_SHIFT ) );

                    oam_sort->attribute[2]  = (u16)(
                        ( ( (( ( obj_info->attribute[2] & MTD_OBJ_ATTR2_NAME_MASK ) + ( 1 << dest_name_shift ) - 1 ) >> dest_name_shift)+ cha_name + cha_name_add ) & MTD_OBJ_ATTR2_NAME_MASK )
                        | ( ( ( act->flag & MTD_ACT_FLAG_BG_NO_MASK ) >> MTD_ACT_FLAG_BG_NO_SHIFT ) << MTD_OBJ_ATTR2_PRIORITY_BG_SHIFT )
                        | ( ( ( act->plt_ofst_no << MTD_OBJ_ATTR2_PALETTE_SHIFT ) ) & MTD_OBJ_ATTR2_PALETTE_MASK ) );
                    ++cha_name_add;
                    ++l;
                }
            }
        }
    }
}*/
// ================================================================
// test_func
/*!
  テスト関数
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return   返値説明
 
  @note
  補足説明
 */
// ================================================================
