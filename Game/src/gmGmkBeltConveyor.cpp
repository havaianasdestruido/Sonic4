// ===========================================================================
/*!
	@file	gmGmkBeltConveyor.cpp
	@brief	ギミック ベルトコンベヤー＠ゾーン４工場

	@author	ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkBeltConveyor.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmPlayer.h"
#include "gmPlySeq.h"
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"

#if _IPHONE
#include "gmTvx.h"
#endif // _IPHONE

#include "gmGmkBeltConveyor.h"

// データヘッダ
#include "common/model/gmk_belt_conv_mdl.hmb"

#if _IPHONE
#define GMD_GMK_BELTCONV_TEST_TVX (1 & _IPHONE)

// commonと構成が変わっている場合に使用する定義
#include "iPhone/model/GMK_BELT_CONV_TVX.HMB"
#endif // _IPHONE



// ----- Struct Definitions --------------------------------------（型の宣言）
// -----

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -----

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// -----

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -----

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_beltconv_obj_3d_list = NULL;
#if GMD_GMK_BELTCONV_TEST_TVX
static AMS_AMB_HEADER* gm_gmk_beltconv_obj_tvx_list = NULL;
#endif // GMD_GMK_BELTCONV_TEST_TVX

// ----- Macros ------------------------------------------------（マクロ定義）
#define	OBJWORK		COMWORK.obj_work
// -----

// ----- Macro Functions -----------------------------------（処理マクロ定義）
#define	chgf(f)		ppFunc = (f)
// -----

// ----- Definitions -------------------------------------------（定数の宣言）
#define	GMD_GMK_BELTCONV_SPEED			(fx32)(2.0)		//	引きずられ速度
#define	GMD_GMK_BELTCONV_COL_HEIGHT		(8)			//	適当に0.5ブロック
#define	GMD_GMK_BELTCONV_COL_OFST_Y		(-16)		//	中心から上面まで

#if _IPHONE
#define	GMD_GMK_BELTCBELT_TEX_WIDTH		(512.0f)		//	テクスチャサイズ
#else
#define	GMD_GMK_BELTCBELT_TEX_WIDTH		(64.0f)		//	テクスチャサイズ
#endif // _IPHONE

#define	GMD_GMK_BELTCONV_ROLL_OFF_Z		(16*FX32_ONE)	// 手前
#define	GMD_GMK_BELTCONV_CONV_OFF_Z		(17*FX32_ONE)	// 真ん中
#define	GMD_GMK_BELTCONV_BELT_OFF_Z		(18*FX32_ONE)	// 奥

// GMS_EVE_RECORD_EVENT::flag
#define GMD_GMK_BELTCONV_SPD_MASK		(0x0F)		// Beltconveyor Speed Type Mask
#define GMD_GMK_BELTCONV_COL_EX_L		(0x01 << 4)	// Collision Extention Flag : Left
#define GMD_GMK_BELTCONV_COL_EX_R		(0x01 << 5)	// Collision Extention Flag : Right

//Collision Extention Length
#define GMD_GMK_BELTCONV_COL_EX_LEN		(16)

// Belt UV Scroll
#if _IPHONE
#define GMD_GMK_BELTCONV_UV_CHECK       (0.125f)
#else
#define GMD_GMK_BELTCONV_UV_CHECK       (1.0f)
#endif // _IPHONE

// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分
// ----- Struct Definitions --------------------------------------（型の宣言）
//! ベルトコンベヤー＠ゾーン４工場
typedef struct tag_GMS_GMK_BELTC_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	BOOL				last_under;		//!< 1/60前のプレイヤー接触

	u16					vect;			//	向き
	s16					width;			//	幅
	fx32				diradd;			//	回転角
	fx32				rolldir;		//	計算用
	fx32				speed;			//	速度
	fx32				roller;			//	もうひとつのローラー位置


	float				tex_u;			//	テクスチャスクロール

}GMS_GMK_BELTC_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
// ---------------------------------------------------------------------------



// ===========================================================================
// gmGmkBeltConveyor*
/*!
	ギミック ベルトコンベヤー＠ゾーン４工場

	@note
		ソニックが乗っかると回転方向へ引っ張られます。
 */
// ---------------------------------------------------------------------------
static void gmGmkBeltConveyorStay(OBS_OBJECT_WORK *obj_work);
#if !GMD_GMK_BELTCONV_TEST_TVX
static void gmGmkBeltConveyor_CreateBelt(OBS_OBJECT_WORK *parent_obj);
static void gmGmkBeltConveyor_CreateRoller(OBS_OBJECT_WORK *parent_obj);
#endif // !GMD_GMK_BELTCONV_TEST_TVX
// ===========================================================================

// ---------------------------------------------------------------------------
// gmGmkBeltConveyorStay
/*!
	ギミック ベルトコンベヤー＠ゾーン４工場 描画

	@note
		座標だけ変えて回転軸をもうひとつ描画 (TVX未使用)
		ベルトコンベヤーに纏わる全てを描画 (TVX使用)
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBeltConveyor_ppOut(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BELTC_WORK *pwork = (GMS_GMK_BELTC_WORK*)obj_work;

#if GMD_GMK_BELTCONV_TEST_TVX
	// 描画しないフレームは終了
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	// 描画しないなら終了
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	
	// 初期化
	void* tvx_roller     = amBindGet(gm_gmk_beltconv_obj_tvx_list, IDB_MODEL_GMK_BELT_CONV_TVX);
	void* tvx_axis       = amBindGet(gm_gmk_beltconv_obj_tvx_list, IDB_MODEL_GMK_BELT_CONV_AXIS_TVX);
	void* tvx_belt_up    = amBindGet(gm_gmk_beltconv_obj_tvx_list, IDB_MODEL_GMK_BELT_CONV_CH_A_TVX);
	void* tvx_belt_down  = amBindGet(gm_gmk_beltconv_obj_tvx_list, IDB_MODEL_GMK_BELT_CONV_CH_B_TVX);
	NNS_TEXLIST* texlist = obj_work->obj_3d->texlist;
	VecFx32* scale       = &obj_work->scale;
	Angle16  rotate_z    = -obj_work->dir.z;
	VecFx32  pos         = obj_work->pos; // 座標を加工するためコピーしておく
	
	// ベルトコンベア　ローラー & 軸
	// 処理削減のため泥臭い処理
	GmTvxSetModel(tvx_roller, texlist, &pos, scale, GMD_TVX_DISP_ROTATE, rotate_z);
	pos.z += GMD_GMK_BELTCONV_CONV_OFF_Z - GMD_GMK_BELTCONV_ROLL_OFF_Z; // 軸用にzを変更
	GmTvxSetModel(tvx_axis, texlist, &pos, scale, 0, 0);
	pos.x += pwork->roller; // 2個目用にxを変更
	GmTvxSetModel(tvx_axis, texlist, &pos, scale, 0, 0);
	pos.z -= GMD_GMK_BELTCONV_CONV_OFF_Z - GMD_GMK_BELTCONV_ROLL_OFF_Z; // zを戻す
	GmTvxSetModel(tvx_roller, texlist, &pos, scale, GMD_TVX_DISP_ROTATE, rotate_z);
	pos.x -= pwork->roller; // xを戻す
	
	
	
	// ベルトコンベア　ベルト共通
	fx32 offset = (pwork->vect==0x0000)? +64*FX32_ONE:-64*FX32_ONE;
	fx32 width  = pwork->roller;
	// extend
	GMS_TVX_EX_WORK work;
	work.u_wrap = NNE_PRIM_TEXWRAP_CLAMP;
	work.v_wrap = NNE_PRIM_TEXWRAP_CLAMP;
	work.coord.v = 0.0f;
	work.color  = 0; // フラグがないので未使用
	//	座標修正
	pos.y += -16 * FX32_ONE;
	pos.z = GMD_OBJ_GIMMICK_POS_Z - GMD_GMK_BELTCONV_BELT_OFF_Z;
	if( pwork->vect == 0x8000 )	//	反時計周り
		pos.x += offset;
	
	while( width != 0 )
	{
		// 処理負荷軽減の為に泥臭い処理
		// ベルト上
		work.coord.u = pwork->tex_u;
		GmTvxSetModelEx(tvx_belt_up, texlist, &pos, scale, 0, 0, &work);
		// ベルト下
		pos.y -= -32 * FX32_ONE;
		work.coord.u = -pwork->tex_u;
		GmTvxSetModelEx(tvx_belt_down, texlist, &pos, scale, 0, 0, &work);
		
		// パラメータ更新
		pos.y += -32 * FX32_ONE;
		pos.x += offset;
		width -= offset;
	}
#else
	ObjDrawActionSummary(obj_work);
	obj_work->pos.x += pwork->roller;
	ObjDrawActionSummary(obj_work);
	obj_work->pos.x -= pwork->roller;
#endif // GMD_GMK_BELTCONV_TEST_TVX
}
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkBeltConveyorStay
/*!
	ギミック ベルトコンベヤー＠ゾーン４工場 待機

	@note
		ソニックの乗っかりを判定すると強制移動速度へ値を入れます。
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBeltConveyorStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BELTC_WORK *pwork = (GMS_GMK_BELTC_WORK*)obj_work;
	OBS_OBJECT_WORK	*ply_obj_work = &g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work;	

	BOOL	player_under = FALSE;

	// プレイヤーが何かに乗っていて、それが自分か判定
	if (ply_obj_work->ride_obj == obj_work)
	{
		ply_obj_work->flow.x = pwork->speed;
		player_under = TRUE;
	}
	if( pwork->last_under && !player_under )
	{
		if (  (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->seq_state == GME_PLY_SEQ_STATE_WALK)
			&&(!(ply_obj_work->move_flag & OBD_MOVE_UNDER)) ) {
			if (  (  (pwork->speed > 0)
				   &&(ply_obj_work->spd_m < 0)
				   &&(ply_obj_work->pos.x > obj_work->pos.x) )
				||(  (pwork->speed < 0)
				   &&(ply_obj_work->spd_m > 0)
				   &&(ply_obj_work->pos.x < obj_work->pos.x) ) ) {
				// ソニック落下直後に再搭乗抑制対策で速度を入れる(スーパーソニックは空中での速度が速すぎて対策困難か)
				ply_obj_work->spd_m = pwork->speed;
			}
		}
	}
	pwork->last_under = player_under;

	pwork->rolldir += pwork->diradd;
	obj_work->dir.z = (u16)(pwork->rolldir>>FX32_SHIFT);

	pwork->tex_u -= (float)(pwork->speed>>FX32_SHIFT)/(GMD_GMK_BELTCBELT_TEX_WIDTH/4);
	while( pwork->tex_u >= GMD_GMK_BELTCONV_UV_CHECK )
	{
		pwork->tex_u -= GMD_GMK_BELTCONV_UV_CHECK;
	}
	while( pwork->tex_u <= -GMD_GMK_BELTCONV_UV_CHECK )
	{
		pwork->tex_u += GMD_GMK_BELTCONV_UV_CHECK;
	}

}
// ---------------------------------------------------------------------------




// ---------------------------------------------------------------------------
// gmGmkPistonStart
/*!
	ギミック ピストン　サブ初期化

	@note
		
		
		

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBeltConveyorStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BELTC_WORK *pwork = (GMS_GMK_BELTC_WORK*)obj_work;

	//	土地あたり矩形餅オブジェ設定
	//	普段では通り抜けられないように
	pwork->COMWORK.col_work.obj_col.obj       = obj_work;

// diff_dataをセットすることで pixel単位でのチェックとなる。すり抜けにも対応。
// g_gm_default_col を使用する場合は矩形サイズに8dot単位の制限が付きます。
	pwork->COMWORK.col_work.obj_col.diff_data	= (s8*)g_gm_default_col;	// 基本地形情報
	pwork->COMWORK.col_work.obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;	// diff_dataを開放しない

	// 地形矩形設定
	pwork->COMWORK.col_work.obj_col.width  = (u16)pwork->width;	//	中心位置からの幅
	pwork->COMWORK.col_work.obj_col.ofst_x = (s16)((pwork->vect==0x0000)?/*右へ向かう*/ 0:-pwork->width);
	pwork->COMWORK.col_work.obj_col.height = GMD_GMK_BELTCONV_COL_HEIGHT;	// 固定
	pwork->COMWORK.col_work.obj_col.ofst_y = GMD_GMK_BELTCONV_COL_OFST_Y;	// 固定

	// 矩形拡張
	if (((GMS_ENEMY_COM_WORK*)obj_work)->eve_rec->flag & GMD_GMK_BELTCONV_COL_EX_L) {
		pwork->COMWORK.col_work.obj_col.width  += GMD_GMK_BELTCONV_COL_EX_LEN;
		pwork->COMWORK.col_work.obj_col.ofst_x -= GMD_GMK_BELTCONV_COL_EX_LEN;
	}
	if (((GMS_ENEMY_COM_WORK*)obj_work)->eve_rec->flag & GMD_GMK_BELTCONV_COL_EX_R) {
		pwork->COMWORK.col_work.obj_col.width  += GMD_GMK_BELTCONV_COL_EX_LEN;
	}

	pwork->COMWORK.col_work.obj_col.dir = (u16)(0x0000);

	pwork->COMWORK.col_work.obj_col.flag |= OBD_COLOBJ_NODIR_PARENT;
	pwork->COMWORK.col_work.obj_col.attr = OBD_COL_DATA_ATTR_THROUGH;
#if !GMD_GMK_BELTCONV_TEST_TVX
	//	ベルトを作る
	gmGmkBeltConveyor_CreateBelt(&pwork->OBJWORK);
	//	ローラーを作る
	gmGmkBeltConveyor_CreateRoller(&pwork->OBJWORK);
#endif // !GMD_GMK_BELTCONV_TEST_TVX

	// 固有のメンバ初期化
	pwork->last_under = FALSE;
	pwork->tex_u = 0.0f;

	obj_work->chgf(gmGmkBeltConveyorStay);
}
#if !GMD_GMK_BELTCONV_TEST_TVX
// --------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ===========================================================================



// ==========================================================================
// gmGmkBeltConveyorBelt*
/*!
	ギミック ベルトコンベヤーのベルト＠ゾーン４工場

	@note

 */
// --------------------------------------------------------------------------
typedef struct tag_GMS_GMK_BELTCBELT_WORK
{
	GMS_EFFECT_3DNN_WORK eff_work;

	u16		up_down;		//	↑(0)？/↓(1)？
#if GMD_GMK_BELTCONV_TEST_TVX
	u16     tvx_index;		//!< TVX model index
#endif // GMD_GMK_BELTCONV_TEST_TVX
}GMS_GMK_BELTCBELT_WORK;
#define	OBJ_3D		eff_work.obj_3d
#define	COMWORK		eff_work.efct_com
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// gmGmkBeltConveyorbelt_ppOut
/*!
	ギミック ベルトコンベヤーのベルト＠ゾーン４工場

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBeltConveyorbelt_ppOut(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BELTC_WORK *pbelt = (GMS_GMK_BELTC_WORK*)obj_work->parent_obj;
	GMS_GMK_BELTCBELT_WORK *pwork = (GMS_GMK_BELTCBELT_WORK*)obj_work;

	fx32 width = pbelt->roller;
	fx32 offset = (pbelt->vect==0x0000)? +64*FX32_ONE:-64*FX32_ONE;

	if( pwork->up_down == 0 )
		pwork->OBJ_3D.draw_state.texoffset[0].u = pbelt->tex_u;
	else
		pwork->OBJ_3D.draw_state.texoffset[0].u = -pbelt->tex_u;

	//	座標修正
	obj_work->pos.x = obj_work->parent_obj->pos.x;
	if( pbelt->vect == 0x8000 )	//	反時計周り
		obj_work->pos.x += offset;

#if GMD_GMK_BELTCONV_TEST_TVX
	void* tvx = amBindGet(gm_gmk_beltconv_obj_tvx_list, pwork->tvx_index);
	NNS_TEXLIST* texlist = obj_work->obj_3d->texlist;
	VecFx32* pos         = &obj_work->pos;
	VecFx32* scale       = &obj_work->scale;
	
	// extend
	GMS_TVX_EX_WORK work;
	work.u_wrap = NNE_PRIM_TEXWRAP_CLAMP;
	work.v_wrap = NNE_PRIM_TEXWRAP_CLAMP;
	work.coord.u = pwork->OBJ_3D.draw_state.texoffset[0].u;// - GMD_GMK_BELTCONV_UV_CHECK;
	work.coord.v = 0.0f;
	work.color  = 0; // フラグがないので未使用
#endif // GMD_GMK_BELTCONV_TEST_TVX
	while( width != 0 )
	{
#if GMD_GMK_BELTCONV_TEST_TVX
		GmTvxSetModelEx(tvx, texlist, pos, scale, 0, 0, &work);
#else
		ObjDrawActionSummary(obj_work);
#endif // GMD_GMK_BELTCONV_TEST_TVX
		obj_work->pos.x += offset;
		width -= offset;
	}
}
// --------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkBeltConveyor_CreateBelt
/*!
	ギミック ベルトコンベヤー　ベルト生成

	@note
		基本ベルト部分の本体処理はありません。
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const u16 tbl_gmk_beltconv_belt[2][2] =
{
	{ IDB_GMK_BELT_CONV_MDL_GMK_BELT_CONV_CH_A_ZNO, (u16)-16 },
	{ IDB_GMK_BELT_CONV_MDL_GMK_BELT_CONV_CH_B_ZNO, (u16)+16 },
};
static void gmGmkBeltConveyor_CreateBelt(OBS_OBJECT_WORK *parent_obj)
{
	GMS_GMK_BELTCBELT_WORK *pwork;
	OBS_OBJECT_WORK *obj_work;
	int i;
	for (i = 0; i < 2; i++)
	{
		pwork = (GMS_GMK_BELTCBELT_WORK*)GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_BELTCBELT_WORK),
		                                                        NULL,
		                                                        0,
		                                                        "Gmk_BeltConveyorBelt");
		obj_work = (OBS_OBJECT_WORK*)pwork;
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_beltconv_obj_3d_list[tbl_gmk_beltconv_belt[i][0]],
		                             &pwork->eff_work.obj_3d);

		// 優先設定
		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - GMD_GMK_BELTCONV_BELT_OFF_Z;
		// 描画変更
		obj_work->ppOut = gmGmkBeltConveyorbelt_ppOut;

		obj_work->parent_obj = parent_obj;
		obj_work->pos.y = parent_obj->pos.y +(s16)(tbl_gmk_beltconv_belt[i][1])*FX32_ONE;

		// フラグ
		obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;						// 親付随なし

		obj_work->move_flag |= OBD_MOVE_NOCOL;						// 移動無し 地形あたりチェック無し
		obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
		obj_work->disp_flag |= OBD_DISP_NODIR;
		obj_work->disp_flag |= OBD_DISP_DRAWSTATE;
		obj_work->flag |= OBD_OBJECT_NOHIT;								// 矩形あたり無し◆

		obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_TEXOFFSET;	// UVスクロール使用 20090924 Dimps Ishizaki

		pwork->up_down = (u16)i;
#if GMD_GMK_BELTCONV_TEST_TVX
		pwork->tvx_index = (u16)(IDB_MODEL_GMK_BELT_CONV_CH_A_TVX + i);
#endif // GMD_GMK_BELTCONV_TEST_TVX
		obj_work->chgf(NULL);
	}
}
// --------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================



// ==========================================================================
// gmGmkBeltConveyorBelt*
/*!
	ギミック ベルトコンベヤーのローラー＠ゾーン４工場

	@note

 */
// --------------------------------------------------------------------------
typedef struct tag_GMS_GMK_BELTCROLLER_WORK
{
	GMS_EFFECT_3DNN_WORK eff_work;

}GMS_GMK_BELTCROLLER_WORK;
#define	OBJ_3D		eff_work.obj_3d
#define	COMWORK		eff_work.efct_com
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// gmGmkBeltConveyorbelt_ppOut
/*!
	ギミック ベルトコンベヤーのローラー＠ゾーン４工場
		座標だけ変えて回転軸をもうひとつ描画
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBeltConveyorRoller_ppOut(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_BELTC_WORK *pwork = (GMS_GMK_BELTC_WORK*)obj_work->parent_obj;
	
#if GMD_GMK_BELTCONV_TEST_TVX
	void* tvx            = amBindGet(gm_gmk_beltconv_obj_tvx_list, IDB_MODEL_GMK_BELT_CONV_AXIS_TVX);
	NNS_TEXLIST* texlist = obj_work->obj_3d->texlist;
	VecFx32* pos         = &obj_work->pos;
	VecFx32* scale       = &obj_work->scale;
	
	GmTvxSetModel(tvx, texlist, pos, scale, 0, 0);
	pos->x += pwork->roller;
	GmTvxSetModel(tvx, texlist, pos, scale, 0, 0);
	pos->x -= pwork->roller;
#else
	ObjDrawActionSummary(obj_work);
	obj_work->pos.x += pwork->roller;
	ObjDrawActionSummary(obj_work);
	obj_work->pos.x -= pwork->roller;
#endif // GMD_GMK_BELTCONV_TEST_TVX
}
// --------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkBeltConveyor_CreateRoller
/*!
	ギミック ベルトコンベヤー　ローラー生成

	@note
		基本ローラー部分の本体処理はありません。
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkBeltConveyor_CreateRoller(OBS_OBJECT_WORK *parent_obj)
{
	OBS_OBJECT_WORK *obj_work;
	GMS_EFFECT_3DNN_WORK *eff_work;

	obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_BELTCBELT_WORK),
		                              NULL,
		                              0,
		                              "Gmk_BeltConveyorRoller");
	eff_work = (GMS_EFFECT_3DNN_WORK*)obj_work;
	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_beltconv_obj_3d_list[IDB_GMK_BELT_CONV_MDL_GMK_BELT_CONV_AXIS_ZNO],
	                             &eff_work->obj_3d);

	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - GMD_GMK_BELTCONV_ROLL_OFF_Z;
	// 描画変更
	obj_work->ppOut = gmGmkBeltConveyorRoller_ppOut;

	obj_work->parent_obj = parent_obj;
	obj_work->pos.x = parent_obj->pos.x;
	obj_work->pos.y = parent_obj->pos.y;
//	obj_work->pos.z = parent_obj->pos.z;

	// フラグ
	obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;						// 親付随なし
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->disp_flag |= OBD_DISP_NODIR;
	obj_work->flag |= OBD_OBJECT_NOHIT;								// 矩形あたり無し◆
	obj_work->chgf(NULL);
}
// --------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================
#endif // !GMD_GMK_BELTCONV_TEST_TVX


// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ===========================================================================
// GmGmkBeltConveyorInit
/*!
	ギミック ベルトコンベヤー＠ゾーン４工場 初期化関数
	
	@param eve_rec	[io] レコードポインタ
	@param pos_x	[in] 出現座標X
	@param pos_y	[in] 出現座標Y
	@param type		[in] 処理内容タイプ 通常は0
	
	@note

 */
// ===========================================================================
OBS_OBJECT_WORK* GmGmkBeltConveyorInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_BELTC_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_BELTC_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_BELTC_WORK), "Gmk_BeltConveyor");

	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_beltconv_obj_3d_list[IDB_GMK_BELT_CONV_MDL_GMK_BELT_CONV_ZNO],
	                             &gmk_work->obj_3d);
	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z - GMD_GMK_BELTCONV_CONV_OFF_Z;
	// 描画変更
	obj_work->ppOut = gmGmkBeltConveyor_ppOut;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE|OBD_MOVE_NOCOL;			// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->flag |= OBD_OBJECT_NOHIT;							// 矩形あたり無し◆

//	MTM_ASSERT ((eve_rec->left < 0 && (eve_rec->left+eve_rec->width) == 0) || (eve_rec->left == 0 && eve_rec->width > 0));
	//	範囲開始座標が回転部分から外れていたら停止
//	MTM_ASSERT ((u16)eve_rec->width >= 64 && (eve_rec->width & 0x3f == 0));

	//	width 倍角
	pwork->width = (s16)(eve_rec->width*2);
	if( eve_rec->left < 0 )		//	半時計周り
	{
		pwork->vect = 0x8000;		//	左へ向かう
		pwork->roller = (fx32)(-pwork->width*FX32_ONE);		//	２点間の距離
	}
	else
	{
		pwork->vect = 0x0000;		//	右へ向かう
		pwork->roller = (fx32)(pwork->width*FX32_ONE);		//	２点間の距離
	}

	pwork->speed = GMD_GMK_BELTCONV_SPEED*FX32_ONE;		//	スピード 標準が２．０

	fx32 add_spd;
	if( (eve_rec->flag & GMD_GMK_BELTCONV_SPD_MASK) < GMD_GMK_BELTCONV_SPD_MASK )
		add_spd = (fx32)((eve_rec->flag & GMD_GMK_BELTCONV_SPD_MASK)<<(FX32_SHIFT-1));
	else
		add_spd = -(fx32)(0.5*FX32_ONE);

	pwork->speed += add_spd;

	pwork->rolldir = 0;
	if( pwork->vect == 0x8000 )		//	反時計周り
		pwork->speed = -pwork->speed;
	pwork->diradd = ((0x10000*pwork->speed)/6/16);

	gmGmkBeltConveyorStart(obj_work);

	return obj_work;
}
// ===========================================================================



// ===========================================================================
// GmGmkBeltConveyorBuild
/*!
	ギミック ベルトコンベヤー＠ゾーン４工場 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkBeltConveyorBuild(void)
{
	gm_gmk_beltconv_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_BELTCONV_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_BELTCONV_TEX),
								0/*draw_flag*/);
	
#if GMD_GMK_BELTCONV_TEST_TVX
	void* tvx = GmGameDatGetGimmickData( GMD_DWORK_NO_GMK_BELTCONV_TVX );
	amBindConv((Uint8*)tvx);
	gm_gmk_beltconv_obj_tvx_list = (AMS_AMB_HEADER*)tvx;
#endif // GMD_GMK_BELTCONV_TEST_TVX
}
// ===========================================================================


// ===========================================================================
// GmGmkBeltConveyorFlush
/*!
	ギミック ベルトコンベヤー＠ゾーン４工場 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkBeltConveyorFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_BELTCONV_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_beltconv_obj_3d_list, amb->file_num);
	gm_gmk_beltconv_obj_3d_list = NULL;
#if GMD_GMK_BELTCONV_TEST_TVX
	gm_gmk_beltconv_obj_tvx_list = NULL;
#endif // GMD_GMK_BELTCONV_TEST_TVX
}
// ===========================================================================
