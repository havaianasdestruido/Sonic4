// ==========================================================================
/*!
	@file gmPrimitive.c
	@brief ゲーム TVX描画

	@author Satoshi Akitomi
				Copyright(c) 2010 Dimps

  $Id: gmTvx.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
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
 *	Target iPhone
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
#include "gmTvx.h"

//----- Definitions ---------------------------------------------------------

// プリミティブ描画情報
#define GMD_TVX_DRAW_WORK_NUM		( 16)	//!<	描画ワーク個数(最終的には減る予定)
#define GMD_TVX_DRAW_STACK_NUM		(256)	//!<	描画スタック個数(増える？)

typedef struct tag_GMS_TVX_DRAW_STACK {
	AOS_TVX_VERTEX* vtx;		//!< 頂点情報
	VecFx32         pos;		//!< 位置情報
	VecFx32         scale;		//!< スケール情報
	u32             disp_flag;	//!< 描画フラグ
	u32             vtx_num;	//!< 頂点数
	Angle32         rotate_z;	//!< Z回転
	NNS_TEXCOORD    coord;		//!< UV スクロール値
	NNS_RGBA8888    color;		//!< 頂点カラーマスク
} GMS_TVX_DRAW_STACK;

typedef struct tag_GMS_TVX_DRAW_WORK {
	NNS_TEXLIST*       tex;				//!< テクスチャリスト
	s32                tex_id;			//!< 使用テクスチャID
	u32                all_vtx_num;		//!< 総頂点数
	u32                stack_num;		//!< 登録TVXスタック数
	NNE_PRIM_TEXWRAP   u_wrap;			//!< TEXTURE WRAP U
	NNE_PRIM_TEXWRAP   v_wrap;			//!< TEXTURE WRAP V
	GMS_TVX_DRAW_STACK stack[GMD_TVX_DRAW_STACK_NUM]; //!< TVXスタック
} GMS_TVX_DRAW_WORK;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static GMS_TVX_DRAW_WORK* gm_tvx_draw_work = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// TVX初期化
// ==========================================================================
// ==========================================================================
// GmTvxBuild
/*!
 *	TVX システム ビルド
 */
// ==========================================================================
void GmTvxBuild(void)
{
	MTM_ASSERT(!gm_tvx_draw_work);
	
	u32 size = (u32)(sizeof(GMS_TVX_DRAW_WORK) * GMD_TVX_DRAW_WORK_NUM);
	gm_tvx_draw_work = (GMS_TVX_DRAW_WORK*)amMemAlloc(size);
	
	GmTvxInit();
}

// ==========================================================================
// GmTvxInit
/*!
 *	TVX システム初期化
 */
// ==========================================================================
void GmTvxInit(void)
{
	GMS_TVX_DRAW_WORK* work = gm_tvx_draw_work;
	
	MTM_ASSERT(work);
	
	u32 size = (u32)(sizeof(GMS_TVX_DRAW_WORK) * GMD_TVX_DRAW_WORK_NUM);
	//	初期化
	amZeroMemory(work, size);
	//	テクスチャIDのみ別初期化
	for (int i = 0; i < GMD_TVX_DRAW_WORK_NUM; i ++) {
		work[i].tex_id = -1;
	}
}

// ==========================================================================
// TVX終了
// ==========================================================================
// ==========================================================================
// GmTvxExit
/*!
 *	TVX システム終了
 */
// ==========================================================================
void GmTvxExit(void)
{
	GmTvxInit();
}

// ==========================================================================
// GmTvxFlush
/*!
 *	TVX システム フラッシュ
 */
// ==========================================================================
void GmTvxFlush(void)
{
	GMS_TVX_DRAW_WORK* work = gm_tvx_draw_work;
	
	MTM_ASSERT(work);
	
	//	後片付け
	amMemFree(work);
	gm_tvx_draw_work = NULL;
}

// ==========================================================================
// オブジェクト各種設定
// ==========================================================================
// ==========================================================================
// GmTvxSetModel
/*!
 *	TVXモデル設定
 *
 *	@param model_tvx	[in]	TVXモデルデータ
 *	@param model_tex	[in]	テクスチャリスト
 *	@param pos			[in]	描画座標
 *	@param scale		[in]	拡大値(負の値で反転にも使用可能)
 *	@param flag			[in]	TVX描画フラグ
 *	@param rotate_z		[in]	Z回転値(0x0000～0xffff)
 *
 *	@note 拡張情報が必要な場合はGmTvxSetModelExを使ってください。
 *
 */
// ==========================================================================
void GmTvxSetModel(void* model_tvx, NNS_TEXLIST* model_tex,
						VecFx32* pos, VecFx32* scale, u32 flag, Angle16 rotate_z)
{
	GMS_TVX_EX_WORK ex_work;
	
	// 通常設定
	ex_work.u_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	ex_work.v_wrap  = NNE_PRIM_TEXWRAP_CLAMP;
	ex_work.coord.u = 0.0f;
	ex_work.coord.v = 0.0f;
	ex_work.color   = 0xffffffff; // フルカラー設定
	
	// GmTvxSetModelExにそのまま送る
	GmTvxSetModelEx(model_tvx, model_tex, pos, scale, flag, rotate_z, &ex_work);
}

// ==========================================================================
// GmTvxSetModelEx
/*!
 *	TVXモデル設定 拡張設定付き
 *
 *	@param model_tvx	[in]	TVXモデルデータ
 *	@param model_tex	[in]	テクスチャリスト
 *	@param pos			[in]	描画座標
 *	@param scale		[in]	拡大値(負の値で反転にも使用可能)
 *	@param flag			[in]	TVX描画フラグ
 *	@param rotate_z		[in]	Z回転値(0x0000～0xffff)
 *	@param ex_work		[in]	TVX描画 拡張情報
 *
 *	@note 拡張情報が必要ない場合はGmTvxSetModelを使ってください。
 *
 */
// ==========================================================================
void GmTvxSetModelEx(void* model_tvx, NNS_TEXLIST* model_tex,
						VecFx32* pos, VecFx32* scale, u32 flag, Angle16 rotate_z,
						GMS_TVX_EX_WORK* ex_work)
{
	// 描画しないフレームは終了
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	GMS_TVX_DRAW_WORK* work = gm_tvx_draw_work; // プリミティブ描画ワーク
	GMS_TVX_DRAW_STACK* stack = NULL; // スタック
	
	MTM_ASSERT(work);
	MTM_ASSERT(AoTvxIsTvxFile(model_tvx));
	
	u32 tex_num = AoTvxGetTextureNum(model_tvx);
	
	// 頂点処理
	for (u32 num = 0; num < tex_num; num++) {
		MTM_ASSERT(AOD_TVX_PRIMTYPE_TRIANGLESTRIP == AoTvxGetPrimitiveType(model_tvx, num));

		u32 vtx_num = AoTvxGetVertexNum(model_tvx, num);
		s32 tex_id  = AoTvxGetTextureId(model_tvx, num);

		// work 設定
		int i;
		for (i = 0; i < GMD_TVX_DRAW_WORK_NUM; i++) {
			//	ワークへ未登録または該当ID登録済みならテクスチャ情報を登録
			if (
				(work[i].tex == NULL && work[i].tex_id == -1)
				||	(work[i].tex == model_tex && work[i].tex_id == tex_id && work[i].u_wrap == ex_work->u_wrap && work[i].v_wrap == ex_work->v_wrap)
				) {
				if (work[i].stack_num >= GMD_TVX_DRAW_STACK_NUM) {
#if defined(MTD_DEBUG)
					amSystemLog("GmTvxSetModel:stack over.\n");
					MTM_ASSERT(work[i].stack_num < GMD_TVX_DRAW_STACK_NUM);
#endif	//#if defined(MTD_DEBUG)
					return;
				}
				work[i].tex     = model_tex;
				work[i].tex_id  = tex_id;
				work[i].u_wrap = ex_work->u_wrap;
				work[i].v_wrap = ex_work->v_wrap;
				work[i].all_vtx_num += vtx_num;

				stack = &work[i].stack[work[i].stack_num];
				stack->vtx       = (AOS_TVX_VERTEX*)AoTvxGetVertex(model_tvx, num);
				stack->vtx_num   = vtx_num;
				stack->pos       = (*pos);
				stack->scale     = (*scale);
				stack->disp_flag = flag;
				stack->rotate_z  = rotate_z;
				stack->coord     = ex_work->coord;
				stack->color     = ex_work->color;
				
				++work[i].stack_num;
				break;
			}
			//	他テクスチャの場合は次へ
		}
#if defined(MTD_DEBUG)
		if (i >= GMD_TVX_DRAW_WORK_NUM) {
			amSystemLog("GmTvxSetModel:work over.\n");
			MTM_ASSERT(i < GMD_TVX_DRAW_WORK_NUM); // ループを完走した=プリミティブワーク不足
		}
#endif	//#if defined(MTD_DEBUG)
	}
}

// ==========================================================================
// GmTvxExecuteDraw
/*!
 *	TVX 一斉描画
 *
 *	@note GmTvxSetModelで設定したモデルデータについて一斉描画
 *		描画コマンドは OBD_DRAW_CMD_STATE_3DNN
 *		一度実行した後は設定したモデルは初期化される。
 */
// ==========================================================================
void GmTvxExecuteDraw(void)
{
	AMS_PARAM_DRAW_PRIMITIVE dat;
	GMS_TVX_DRAW_WORK* work   = gm_tvx_draw_work; // プリミティブ描画ワーク
	GMS_TVX_DRAW_STACK* stack = NULL; // スタック
	
	// 実体が無い場合は終了
	if (!work) {
		return;
	}
	
	// 先頭のテクスチャで実行を判定
	if (!work->tex) {
		return;
	}
	
	// 疑似ライトカラー設定
	u32 color = GmMainGetLightColor();
	
	// アルファブレンド設定
#if defined(_PC) | defined(_XBOX)
	dat.bldSrc = NNE_BLENDMODE_SRCALPHA;
	dat.bldDst = NNE_BLENDMODE_INVSRCALPHA;
	dat.bldMode = NNE_BLENDOP_ADD;
#else
	dat.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
	dat.bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
	dat.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
	// テスト設定
	dat.aTest = 1;
	dat.zMask = 0;
	dat.zTest = 1;
	
	// ソートしない
	dat.noSort = 1;
	
	dat.format3D = NNE_PRIM3D_FMT_PCT;

	// 描画
	u32 prim;
	for (prim = 0; prim < GMD_TVX_DRAW_WORK_NUM; ++prim) {
		//	登録されてないなら終了
		if (work[prim].tex_id == -1) {
			break;
		}
		// ブレンドフラグ初期化
		dat.ablend = NNE_PRIM_ALPHABLEND_OFF;
		
		// テクスチャ設定
		dat.texlist = work[prim].tex;
		
		// テクスチャクランプ設定
		dat.uwrap = work[prim].u_wrap;
		dat.vwrap = work[prim].v_wrap;
		
		// 描画
		dat.type = NNE_PRIM_TRIANGLE_STRIP;
		dat.count = work[prim].all_vtx_num + work[prim].stack_num * 2 - 2;
		
		NNS_PRIM3D_PCT* v_tbl = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer((s32)(sizeof(NNS_PRIM3D_PCT) * dat.count));
		dat.vtxPCT3D = v_tbl;
		dat.texId = work[prim].tex_id;
		u32 v_tbl_pos = 0;
		
		float dx, dy, dz;
		NNS_MATRIX prim_mtx, u_mtx;
		nnMakeUnitMatrix(&u_mtx);
		for (u32 num = 0; num < work[prim].stack_num; ++num) {
			stack = &(work[prim].stack[num]);
			
			// ブレンドフラグ
			if (stack->disp_flag & GMD_TVX_DISP_BLEND) {
				dat.ablend = NNE_PRIM_ALPHABLEND_ON; // 1つでもブレンド設定のものがあればブレンドに切り替え
			}
			
			// プリミティブ設定
			dx =  FXM_FX32_TO_FLOAT(stack->pos.x);
			dy = -FXM_FX32_TO_FLOAT(stack->pos.y);
			dz =  FXM_FX32_TO_FLOAT(stack->pos.z);
			nnMakeUnitMatrix(&prim_mtx);
			
			// 回転あり
			if (stack->disp_flag & GMD_TVX_DISP_ROTATE) {
				nnRotateZMatrix(&prim_mtx, &prim_mtx, (u16)stack->rotate_z);
			}
			// スケールあり
			if (stack->disp_flag & GMD_TVX_DISP_SCALE) {
				nnScaleMatrix(&prim_mtx, &prim_mtx,
								FXM_FX32_TO_FLOAT(stack->scale.x),
								FXM_FX32_TO_FLOAT(stack->scale.y),
								FXM_FX32_TO_FLOAT(stack->scale.z));
			}
			// 疑似ライト
			u32 light = color;
			if (stack->disp_flag & GMD_TVX_DISP_LIGHT_DISABLE) {
				light = stack->color; // カラー設定
			}
			
			NNS_VECTOR vec;
			
			NNS_PRIM3D_PCT* v = v_tbl;
			AOS_TVX_VERTEX* vtx = stack->vtx;
			
			// 頂点設定 (STRIP対応)
			for (int i = 0; i < stack->vtx_num; i++) {
				vec.x = vtx[i].x;
				vec.y = vtx[i].y;
				vec.z = vtx[i].z;
				
				// 何か設定されていれば行列変換
				// ごみが入っていたとしても単位行列になっているので見た目は崩れない
				if (stack->disp_flag) {
					nnTransformVector(&v[i].Pos, &prim_mtx, &vec);
				}
				else {
					nnCopyVector(&v[i].Pos, (const NNS_VECTOR*)&vec);
				}
				
				v[i].Pos.x += dx;
				v[i].Pos.y += dy;
				v[i].Pos.z += dz;
				
				v[i].Tex.u = vtx[i].u + stack->coord.u;
				v[i].Tex.v = vtx[i].v + stack->coord.v;
				v[i].Col   = vtx[i].c & light;
			}
			
			v_tbl += stack->vtx_num + 2;
			
			// STRIPなデータにするための対応
			NNS_PRIM3D_PCT* v_strip;
			// 先頭のSTRIP以外
			if (num != 0) {
				v_strip = v - 1;
				v_strip[0] = v_strip[1];
			}
			// 最後のSTRIP以外
			if (num != work[prim].stack_num - 1) {
				v_strip = &v[stack->vtx_num - 1];
				v_strip[1] = v_strip[0];
			}
		}
		amMatrixPush(&u_mtx);
		ObjDraw3DNNDrawPrimitive(&dat);
		amMatrixPop();
		
		// 後片付け
		work[prim].tex    = NULL;
		work[prim].tex_id = -1;
		work[prim].stack_num  = 0;
		work[prim].all_vtx_num = 0;
	}
	
}

// ==========================================================================
// GmTvxStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void GmTvxStaticVarInit(void)
{
	gm_tvx_draw_work = NULL;
}

//----- Local Functions -----------------------------------------------------

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
