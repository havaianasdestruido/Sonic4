// ==========================================================================
/*!
  @file gmGmkSpring.cpp
  @brief ギミック スプリング

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGmkSpring.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date::						   $
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmComEfct.h"
#include "gmGmkSwitch.h"

#include "gmGmkSpring.h"

// データヘッダ
#include "common/model/gmk_spring_mdl.hmb"
#include "common/model/gmk_spring_mtn.hmb"


//----- Definitions ---------------------------------------------------------
/* eve_rec->left */
// バネの力
// 0 ～ 7
#define GMD_GMK_SPRING_INTNSITY_MAX		(7)	//!< 通常バネ強度
#define GMD_GMK_SPRING_LR_INTNSITY_MAX	(5)	//!< 左右バネ強度
/* eve_rec->top */
// 操作無効時間 (フレーム)
/* eve_rec->width */
// スイッチ使用時管理ID
/* eve_rec->height */
// トロッコ時 重力方向変換値
/* eve_rec->flag */
// ばね力係数
#define GMD_GMK_SPRING_SPD		(0x07800)
#define GMD_GMK_SPRING_SPDAD	(0x01800)
//EVE_FLAG_
#define GMD_GMK_EVE_FLAG_SPRING_BEHIND			( 1 << 0) // 隠れバネ
#define GMD_GMK_EVE_FLAG_SPRING_NOPOS			( 1 << 1) // 座標補正を行わない
#define GMD_GMK_EVE_FLAG_SPRING_NODISP			( 1 << 2) // 表示を一切行わない
#define GMD_GMK_EVE_FLAG_SPRING_SPD_CLEAR		( 1 << 3) // ヒット前のプレイヤー速度をクリアする
#define GMD_GMK_EVE_FLAG_SPRING_USE_SWITCH		( 1 << 4) // スイッチリンクあり
#define GMD_GMK_EVE_FLAG_SPRING_TRUCK_CAM_SLOW	( 1 << 5) // トロッコ時接地までカメラ回転をゆっくりに
//#define GMD_GMK_SPRING_TUTORIAL	( 1 << 6) // チュートリアル専用処理 トリックコンボ使用数を減らさない

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkSpringFwInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpringFwMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpringActInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpringActMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpringDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static void gmGmkSpringSwitchOffInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpringSwitchOffMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkSpringSwitchOnMain(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
/// 矩形設定
static s16 gm_gmk_spring_rect[12][MTD_RECT] = {
	// L    T    R    B
	{ -8, -18,   8,   0},	// 上
	{  0, -20,  20,   0},	// 右上
	{  0,  -8,  18,   8},	// 右
	{  0,   0,  20,  20},	// 右下
	{ -8,   0,   8,  18},	// 下
	{-20,   0,   0,  20},	// 左下
	{-18,  -8,   0,   8},	// 左
	{-20, -20,   0,   0},	// 左上
	{ -4, -14,  14,   4},	// 右上うまり B面後ろ
	{-14, -14,   4,   4},	// 左上うまり B面後ろ
	{ -4, -14,  14,   4},	// 右上うまり A面後ろ
	{-14, -14,   4,   4},	// 左上うまり A面後ろ
};
/// 角度設定
static u16 gm_gmk_spring_dir[] = {
	0x0000,	// 上
	0x2000,	// 右上
	0x4000,	// 右
	0x6000,	// 右下
	0x8000,	// 下
	0xA000,	// 左下
	0xC000,	// 左
	0xE000,	// 左上
	0x2000,	// 右上うまり B面後ろ
	0xE000,	// 左上うまり B面後ろ
	0x2000,	// 右上うまり A面後ろ
	0xE000,	// 左上うまり A面後ろ
};

static OBS_ACTION3D_NN_WORK *gm_gmk_spring_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkSpringBuild
/*!
 *	ギミック スプリング データ構築
 */
// ==========================================================================
void GmGmkSpringBuild(void)
{
	gm_gmk_spring_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPRING_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPRING_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkSpringFlush
/*!
 *	ギミック スプリング データ片付け
 */
// ==========================================================================
void GmGmkSpringFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SPRING_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_spring_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkSpringInit
/*!
 *	ギミック スプリング 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *		user_timer : FWアクションID\n
 *		user_work  : 実行アクションID
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSpringInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_RECT_WORK		*rect_work;
	s32					spring_no;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_SPRING");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;


	if (eve_rec->id <= GMD_EVENT_ID_GMK_SPRING_LUG) {
		// 通常
		spring_no = eve_rec->id - GMD_EVENT_ID_GMK_SPRING_U;
	}
	else {
		// 地面埋まり A面後ろ
		spring_no = eve_rec->id - GMD_EVENT_ID_GMK_SPRING_U + 1 -
						(GMD_EVENT_ID_GMK_SPRING_RUG_A - GMD_EVENT_ID_GMK_SPRING_LUG);
	}


	// モデル初期化
	if (eve_rec->id == GMD_EVENT_ID_GMK_SPRING_RU ||
			eve_rec->id == GMD_EVENT_ID_GMK_SPRING_RD ||
			eve_rec->id == GMD_EVENT_ID_GMK_SPRING_LD ||
			eve_rec->id == GMD_EVENT_ID_GMK_SPRING_LU) {
		if (eve_rec->id == GMD_EVENT_ID_GMK_SPRING_RU ||
				eve_rec->id == GMD_EVENT_ID_GMK_SPRING_LD) {
			// 斜めタイプ (壁 右上)
			ObjObjectCopyAction3dNNModel(obj_work,
							&gm_gmk_spring_obj_3d_list[IDB_GMK_SPRING_MDL_GMK_SPRING_TILT_ZNO],
							&gmk_work->obj_3d);
		}
		else {
			// 斜めタイプ (壁 左上)
			ObjObjectCopyAction3dNNModel(obj_work,
							&gm_gmk_spring_obj_3d_list[IDB_GMK_SPRING_MDL_GMK_SPRING_TILT_L_ZNO],
							&gmk_work->obj_3d);
		}

		// モーション初期化
		ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, FALSE/*marge*/,
										ObjDataGet(GMD_DWORK_NO_GMK_SPRING_MTN), NULL/*mtn_data_path*/,
										0/*index*/, NULL/*archive*/);

		// アクションID取得
		obj_work->user_timer= IDB_GMK_SPRING_MTN_GMK_SPRING_TILT_ZNM;		// FW
		obj_work->user_work	= IDB_GMK_SPRING_MTN_GMK_SPRING_TILT_RUN_ZNM;	// RUN
	}
	else {
		//　通常タイプ
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_spring_obj_3d_list[IDB_GMK_SPRING_MDL_GMK_SPRING_ZNO],
						&gmk_work->obj_3d);

		// モーション初期化
		ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, FALSE/*marge*/,
										ObjDataGet(GMD_DWORK_NO_GMK_SPRING_MTN), NULL/*mtn_data_path*/,
										0/*index*/, NULL/*archive*/);

		// アクションID取得
		obj_work->user_timer= IDB_GMK_SPRING_MTN_GMK_SPRING_ZNM;		// FW
		obj_work->user_work	= IDB_GMK_SPRING_MTN_GMK_SPRING_RUN_ZNM;	// RUN
	}

	// 優先設定
	if (eve_rec->id == GMD_EVENT_ID_GMK_SPRING_RUG ||
			eve_rec->id == GMD_EVENT_ID_GMK_SPRING_LUG) {
		// 地面うまりB面後ろ
		obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_BACK;
	}
	else {
		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;
	}


	// 矩形設定
	// 対プレイヤー
	gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	rect_work->ppDef = gmGmkSpringDefFunc;
	rect_work->ppHit = NULL;
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work,
						gm_gmk_spring_rect[spring_no][MTD_LEFT], gm_gmk_spring_rect[spring_no][MTD_TOP],
						gm_gmk_spring_rect[spring_no][MTD_RIGHT], gm_gmk_spring_rect[spring_no][MTD_BOTTOM]);
	rect_work->flag |= OBD_RECT_OUT;

	// BODY◆
	rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work,
						gm_gmk_spring_rect[spring_no][MTD_LEFT], gm_gmk_spring_rect[spring_no][MTD_TOP],
						gm_gmk_spring_rect[spring_no][MTD_RIGHT], gm_gmk_spring_rect[spring_no][MTD_BOTTOM]);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// 方向設定
	obj_work->dir.z = gm_gmk_spring_dir[spring_no];
//	if (GMD_EVENT_ID_GMK_SPRING_U <= eve_rec->id && 
//			eve_rec->id <= GMD_EVENT_ID_GMK_SPRING_LU) {
//		obj_work->dir.z = (u16)(0x2000 * (eve_rec->id - GMD_EVENT_ID_GMK_SPRING_U));
//	}
//	if (eve_rec->id == GMD_EVENT_ID_GMK_SPRING_LU ||
//			eve_rec->id == GMD_EVENT_ID_GMK_SPRING_RD) {
//		obj_work->dir.y = 0x8000;
//	}


	// スイッチ対応
	if (eve_rec->flag & GMD_GMK_EVE_FLAG_SPRING_USE_SWITCH) {
		if (eve_rec->width >= GMD_GMK_SW_MAX) {
			// 最大値越え
			MTM_ASSERT(!"gmGmkSpring.cpp Error! switch id over\n");
			eve_rec->width = 0;
		}

		// 有効設定
		if (!GmGmkSwitchIsOn(eve_rec->width)) {
			// 無効状態から開始
			gmGmkSpringSwitchOffInit(obj_work);
		}
		else {
			// 有効状態から開始
			gmGmkSpringFwInit(obj_work);
		}
	}
	else {
		// 通常
		gmGmkSpringFwInit(obj_work);
	}
#if _IPHONE
	if (GsGetMainSysInfo()->stage_id == GSD_MAIN_STAGE_ID_4_3) {
		// ライトをソニック用と合わせる@iPhone3GS系Light対策
		obj_work->obj_3d->use_light_flag = 0;
		obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
	}
	else {
		// ライトを切る@iPhone3GS系Light対策
		int num = obj_work->obj_3d->object->nMaterial;
		NNS_MATERIAL_GLES11_DESC* desc = (NNS_MATERIAL_GLES11_DESC*)(obj_work->obj_3d->object->pMatPtrList->pMaterial);
		for (int i = 0; i < num; i++) {
			desc[i].fFlag |= NND_MATFLAG_DISABLE_LIGHTING;
		}
	}
#endif // _IPHONE
#if OBD_OBJECT_USE_NOEXIST
	obj_work->flag |= OBD_OBJECT_NOEXIST_ENABLE;
#endif // OBD_OBJECT_USE_NOEXIST

	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkSpringFwInit
/*!
 *	ギミック スプリング FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
//#define	IDB_GMK_SPRING_MTN_GMK_SPRING_ZNM		0		/*  */
//#define	IDB_GMK_SPRING_MTN_GMK_SPRING_RUN_ZNM		1		/*  */
void gmGmkSpringFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	ObjDrawObjectActionSet(obj_work, obj_work->user_timer);

	// メイン処理
	if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_SPRING_USE_SWITCH) {
		// スイッチタイプ
		obj_work->ppFunc = gmGmkSpringSwitchOnMain;
	}
	else {
		// 通常
		obj_work->ppFunc = gmGmkSpringFwMain;
	}

	if (gmk_work->ene_com.eve_rec->flag & (GMD_GMK_EVE_FLAG_SPRING_BEHIND | GMD_GMK_EVE_FLAG_SPRING_NODISP)) {
		// 非表示設定
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	ギミック スプリング FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSpringFwMain(OBS_OBJECT_WORK *obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);

//	GMS_ENEMY_3D_WORK	*gmk_work;
//	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	/* 子オブジェクトとして作成された時の処理 */
//	if (gmk_work->ene_com.eve_rec->id == GMD_EVE_ROPEC_SP1) {	// 親が巻き込みロープ
//		// 親のフラグに合わせてBG優先を切り替える
//		if ( pWork->obj.parent_obj ){
//			GMS_ENEMY_WORK * pPer = (GMS_ENEMY_WORK *)pWork->obj.parent_obj;
//			if ( pPer->enemy_flag & GMD_GMK_ROPE_C_ENEMY_FLAG_BACK ) {
//				ObjObjectBgPrioritySet(&pWork->obj, GMD_OBJ_BG_PRIO_3D);
//			}
//			else{
//				ObjObjectBgPrioritySet(&pWork->obj, GMD_OBJ_BG_PRIO_B);
//			}
//		}
//	}
}

// ==========================================================================
// gmGmkSpringActInit
/*!
 *	ギミック スプリング ばね発動状態設定
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSpringActInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	ObjDrawObjectActionSet(obj_work, obj_work->user_work);

	// 処理関数セット
	obj_work->ppFunc = gmGmkSpringActMain;

	// 表示状態設定
	if (gmk_work->ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_SPRING_NODISP) {
		obj_work->disp_flag |= OBD_DISP_NODISP;	// 隠しスプリング
	}
	else {
		obj_work->disp_flag &= ~OBD_DISP_NODISP;	// 通常スプリング
	}
}

// ==========================================================================
// gmGmkSpringActMain
/*!
 *	ギミック スプリング ばね発動状態 メイン関数
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSpringActMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// モーション終了
	if (obj_work->disp_flag & OBD_DISP_END) {
		// 連続HIT回避用フラグ寝かす
		gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~(OBD_RECT_DAMAGE | OBD_RECT_FRAMEHIT | OBD_RECT_FRAMEOUT);
		gmGmkSpringFwInit(obj_work);
	}
}

// ==========================================================================
// gmGmkSpringDefFunc
/*!
 *	ギミック スプリング 矩形 くらい処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 *		ppDefに登録\n
 *		eve_rec->left はバネの力 0 ～ 7
 */
// ==========================================================================
void gmGmkSpringDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	fx32				spd_x, spd_y;
	GMS_ENEMY_COM_WORK	*com_work = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;
	s32					spring_intensity;
	s32					fall_dir = -1;

	if (com_work == NULL) {
		return;
	}
	if (ply_work == NULL || ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		return;
	}

	// バネアクションへ
	gmGmkSpringActInit((OBS_OBJECT_WORK*)com_work);

	// バネ強度取得
	spring_intensity = MTM_MATH_CLIP(com_work->eve_rec->left, 0, GMD_GMK_SPRING_INTNSITY_MAX);
	if (com_work->eve_rec->id == GMD_EVENT_ID_GMK_SPRING_L || com_work->eve_rec->id == GMD_EVENT_ID_GMK_SPRING_R) {
		// 左右スプリングは接地後の速度が0xF000を超えてしまうので、5までにまるめる
		spring_intensity = MTM_MATH_CLIP(spring_intensity, 0, GMD_GMK_SPRING_LR_INTNSITY_MAX);
	}

	spd_x = GMD_GMK_SPRING_SPD;
	spd_x += GMD_GMK_SPRING_SPDAD * spring_intensity;
	spd_y = -spd_x;

	if (com_work->eve_rec->id == GMD_EVENT_ID_GMK_SPRING_D || com_work->eve_rec->id == GMD_EVENT_ID_GMK_SPRING_U) {
		// 上下
		spd_x = 0;
	}
	else if (com_work->eve_rec->id == GMD_EVENT_ID_GMK_SPRING_R || com_work->eve_rec->id == GMD_EVENT_ID_GMK_SPRING_L) {
		// 左右
		spd_y = 0;
	}
	else {
		// 斜め
		// 斜めの物はcos45sin45を掛ける
		spd_x = (spd_x * 0x00b5) >> 8;//FX_Mul(spd_x, 0x00b50);
		spd_y = (spd_y * 0x00b5) >> 8;//FX_Mul(spd_y, 0x00b50);
	}

	// 向きにあわせる
	if (GMD_EVENT_ID_GMK_SPRING_RD <= com_work->eve_rec->id &&
					com_work->eve_rec->id <= GMD_EVENT_ID_GMK_SPRING_LD) {
		spd_y = -spd_y;
	}
	if (GMD_EVENT_ID_GMK_SPRING_LD <= com_work->eve_rec->id &&
			com_work->eve_rec->id <= GMD_EVENT_ID_GMK_SPRING_LU ||
			com_work->eve_rec->id == GMD_EVENT_ID_GMK_SPRING_LUG ||
			com_work->eve_rec->id == GMD_EVENT_ID_GMK_SPRING_LUG_A) {
		spd_x = -spd_x;
	}

	// 重力対応
	//ObjObjectSpdDirFall(&spd_x, &spd_y, -g_gm_main_system.pseudofall_dir);

	// 横向きバネの時、位置設定
	if (com_work->eve_rec->id == GMD_EVENT_ID_GMK_SPRING_L || com_work->eve_rec->id == GMD_EVENT_ID_GMK_SPRING_R) {
		if (!(com_work->eve_rec->flag & GMD_GMK_EVE_FLAG_SPRING_NOPOS)) {
			ply_work->obj_work.pos.y = ply_work->obj_work.pos.y + 0x2000;
		}
	}

	// 重力方向取得
	if (1 <= com_work->eve_rec->height && com_work->eve_rec->height <= 4) {
		fall_dir = (com_work->eve_rec->height - 1) * 0x4000;
	}

	// プレイヤーをジャンプ
	GmPlySeqInitSpringJump(ply_work, spd_x, spd_y,
			com_work->eve_rec->flag & GMD_GMK_EVE_FLAG_SPRING_SPD_CLEAR ? TRUE : FALSE,
			com_work->eve_rec->top >= 0 ? com_work->eve_rec->top*FX32_ONE : 0, fall_dir,
			com_work->eve_rec->flag & GMD_GMK_EVE_FLAG_SPRING_TRUCK_CAM_SLOW ? TRUE : FALSE);

	// エフェクト
	GmComEfctCreateSpring(&com_work->obj_work,
				(mine_rect->rect.left + mine_rect->rect.right)*FX32_ONE/2,
				(mine_rect->rect.top + mine_rect->rect.bottom)*FX32_ONE/2);
	
#if _IPHONE
	// フラグが立っていたら強制処理
	if ((com_work->eve_rec->flag & (1 << 6)) && (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK)) {
		ply_work->gmk_flag2 |= GMD_PLGF2_TRUCK_JUMP_MOVE_ROT;
	}
#endif // _IPHONE
	
#if 0
	// トリックギミック設定
	if (pWork->eve_rec->id == GMD_EVE_ROPEC_SP1) {
		// 巻き込みロープスプリング
		if (pWork->obj.parent_obj) {
			GmPlayerGmkSetTrickGimmick(pPlayer, (GMS_ENEMY_WORK*)pWork->obj.parent_obj);
		}
	}
	else {
		GmPlayerGmkSetTrickGimmick(pPlayer, pWork);
	}

	// チュートリアル専用処理
	if (pWork->eve_rec->flag & GMD_GMK_SPRING_TUTORIAL) {
		pPlayer->trick_combo = 0;		// コンボ切断
		// 強制切断
		GmTrickTensionComboFailureSet(pPlayer);
	}
#endif
}


// ==========================================================================
// gmGmkSpringSwitchOffInit
/*!
 *	ギミック スプリング スイッチOFF状態初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSpringSwitchOffInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	ObjDrawObjectActionSet(obj_work, obj_work->user_timer);

	// メイン処理
	obj_work->ppFunc = gmGmkSpringSwitchOffMain;

	// 非表示設定
	obj_work->disp_flag |= OBD_DISP_NODISP;

	// 矩形無効設定
	obj_work->flag |= OBD_OBJECT_NOHIT;
}

// ==========================================================================
// gmGmkSpringSwitchOffMain
/*!
 *	ギミック スプリング スイッチOFF状態 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSpringSwitchOffMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	if (GmGmkSwitchIsOn(gmk_work->ene_com.eve_rec->width)) {
		// 有効化
		// 表示設定
		obj_work->disp_flag &= ~OBD_DISP_NODISP;

		// 矩形有効設定
		obj_work->flag &= ~OBD_OBJECT_NOHIT;

		// エフェクト
		GmComEfctCreateSpring(obj_work,
					(gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].rect.left +
						gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].rect.right)*FX32_ONE/2,
					(gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].rect.top +
						gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].rect.bottom)*FX32_ONE/2);

		// 通常状態へ
		gmGmkSpringFwInit(obj_work);
	}
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	ギミック スプリング スイッチON状態 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkSpringSwitchOnMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	if (!GmGmkSwitchIsOn(gmk_work->ene_com.eve_rec->width)) {
		// スイッチOFFに
		gmGmkSpringSwitchOffInit(obj_work);
	}
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
