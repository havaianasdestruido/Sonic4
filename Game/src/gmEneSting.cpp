// ==========================================================================
/*!
  @file gmEneSting.cpp
  @brief エネミー スティンガー

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEneSting.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 *	GMS_EVE_RECORD_EVENT
 *		left		: 移動範囲左
 *		top			: なし
 *		width		: 移動範囲幅
 *		height		: なし
 *
 *		flag
 *			1		: 左右反転設定 ONで右向き
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmEffect.h"
#include "gmEffectEnemy.h"
#include "gmEffectCmn.h"
#include "gmComEfct.h"
#include "gmSound.h"

#include "gmEneCom.h"
#include "gmEneSting.h"

// データヘッダ
#include "common/model/ENE_STING_MTN.HMB"
#include "common/model/ENE_STING_MDL.HMB"


//----- Definitions ---------------------------------------------------------
#define GMD_ENE_STING_JET_S_SMOKE_ON	(0)


// GMS_EVE_RECORD_EVENT.flag
#define GMD_ENE_STING_EVE_FLAG_RIGHT	(0x0001)	//!< 右向き開始


#define GMD_ENE_STING_SPD_X			(0x1000)		//!< 移動速度
#define GMD_ENE_STING_DEC_SPD_DIST	(8*FX32_ONE)	//!< 基本減速開始距離

#define GMD_ENE_STING_SEARCH_DIR_START	(0x10000 * (30) / 360)		//!< サーチ角度 START (逆回転設定)	(30度)
#define GMD_ENE_STING_SEARCH_DIR_END	(0x10000 * (60) / 360)		//!< サーチ角度 END (逆回転設定)	(60度)

#define GMD_ENE_STING_GUN_OFST_Y		(40*FX32_ONE)	//!< 弾発射口オフセット

#define GMD_ENE_STING_BULLET_SPD		((fx32)(1.5 * 1.5 * FX32_ONE))
#define GMD_ENE_STING_BULLET_OFST_X		(0 * FX32_ONE)
#define GMD_ENE_STING_BULLET_OFST_Y		(28 * FX32_ONE)
#define GMD_ENE_STING_BULLET_OFST_Z		(0 * FX32_ONE)
#define GMD_ENE_STING_BULLET_WAIT		(15)
#define GMD_ENE_STING_BULLET_AFTER_WAIT	(30)
#define GMD_ENE_STING_BULLET_FLASH_OFST_X		(8 * FX32_ONE)
#define GMD_ENE_STING_BULLET_FLASH_OFST_Y		(36 * FX32_ONE)
#define GMD_ENE_STING_BULLET_FLASH_OFST_Z		(0 * FX32_ONE)

#define GMD_ENE_STING_BULLET_CLIP_OFST	(16)	//!< 弾エフェクトクリッピングオフセット値

#define GMD_ENE_STING_TURN_FRAME		(40)	//!< ターンモーションフレーム


/// スティンガーワーク
typedef struct tag_GMS_ENE_STING_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	OBS_RECT_WORK			search_rect_work;	//!< サーチ用矩形
	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離

	GMS_EFFECT_3DES_WORK	*efct_r_jet;			//!< バーニア
	GMS_EFFECT_3DES_WORK	*efct_l_jet;			//!< バーニア
	GMS_EFFECT_3DES_WORK	*efct_smoke;		//!< バーニア煙

	fx32					bullet_spd_x;
	fx32					bullet_spd_y;
	Angle16					bullet_dir;

	NNS_MATRIX				jet_r_mtx;
	NNS_MATRIX				jet_l_mtx;
	NNS_MATRIX				gun_mtx;

} GMS_ENE_STING_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmEneStingWalkInit(OBS_OBJECT_WORK *obj_work);
static void gmEneStingWalkMain(OBS_OBJECT_WORK *obj_work);
//static void gmEneStingFwInit(OBS_OBJECT_WORK *obj_work);
//static void gmEneStingFwMain(OBS_OBJECT_WORK *obj_work);
static void gmEneStingFlipInit(OBS_OBJECT_WORK *obj_work);
static void gmEneStingFlipMain(OBS_OBJECT_WORK *obj_work);
static void gmEneStingAtkInit(OBS_OBJECT_WORK *obj_work);
static void gmEneStingAtkMain(OBS_OBJECT_WORK *obj_work);

static BOOL gmEneStingSetWalkSpeed(GMS_ENE_STING_WORK *sting_work);

static void gmEneStingRegRectFunc(OBS_OBJECT_WORK *obj_work);
static void gmEneStingSearchDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);

static void gmEneStingMotionCallback(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param);

static void gmEneStingCreateJetEfct(GMS_ENE_STING_WORK *sting_work);
static void gmEneStingClearJetEfct(GMS_ENE_STING_WORK *sting_work);
static void gmEneStingJetEfctMain(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_sting_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneStingBuild
/*!
 *	エネミー スティンガー データ構築
 */
// ==========================================================================
void GmEneStingBuild(void)
{
	gm_ene_sting_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_STING_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_STING_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneStingFlush
/*!
 *	エネミー スティンガー データ片付け
 */
// ==========================================================================
void GmEneStingFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_STING_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_sting_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneStingInit
/*!
 *	エネミー スティンガー 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *		user_work	: 左移動限界\n
 *		usre_flag	: 右移動限界
 */
// ==========================================================================
OBS_OBJECT_WORK* GmEneStingInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_STING_WORK	*sting_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_STING_WORK), "ENE_STING");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	sting_work = (GMS_ENE_STING_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_sting_obj_3d_list[IDB_ENE_STING_MDL_ENE_STING_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, FALSE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_STING_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

	// モーションコールバック設定
	ene_work->obj_3d.mtn_cb_func = gmEneStingMotionCallback;
	ene_work->obj_3d.mtn_cb_param= sting_work;

	// 優先設定
	obj_work->pos.z = GMD_OBJ_ENEMY_POS_Z;

	// 矩形設定
	// 対プレイヤー
	// 攻撃
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -10, -8, 20, 8);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// くらい
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -18, -16, 28, 16);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// BODY◆
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work, -18, -16, 28, 16);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// サーチ矩形
	rect_work = &sting_work->search_rect_work;
	rect_work->ppDef = gmEneStingSearchDefFunc;
	//rect_work->ppHit = NULL;
	ObjRectGroupSet(rect_work, GMD_OBJ_RECT_GROUP_ENEMY, GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
	ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	rect_work->parent_obj = obj_work;
	// 仮矩形
	ObjRectWorkSet(rect_work, 0, 0, 128, 128);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE | OBD_RECT_GROUP | OBD_RECT_NODAMAGE |
							OBD_RECT_NOHIT_UP | OBD_RECT_NOAUTO_ENABLEOFF;
	// 矩形登録処理 登録
	obj_work->ppRec = gmEneStingRegRectFunc;

	// 地形あたり
	//ObjObjectFieldRectSet(obj_work, -4, -8, 4, 0);

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag |= OBD_MOVE_NOCOL;	// 地形あたり無し
	obj_work->move_flag	&= ~OBD_MOVE_FALL;	// 落下なし

	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_STING_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
#if 1
	{
		//sting_work->spd_dec_dist = GMD_ENE_STING_SPD_X * (GMD_ENE_STING_TURN_FRAME/2) / 2;
		//sting_work->spd_dec		 = sting_work->spd_dec_dist / (GMD_ENE_STING_TURN_FRAME/2);

		sting_work->spd_dec		 = GMD_ENE_STING_SPD_X / (GMD_ENE_STING_TURN_FRAME/2);
		sting_work->spd_dec_dist = GMD_ENE_STING_SPD_X * (GMD_ENE_STING_TURN_FRAME/2) / 2;

	//	if ((eve_rec->width << FX32_SHIFT) < sting_work->spd_dec_dist) {
	//		sting_work->spd_dec		 = sting_work->spd_dec_dist / (GMD_ENE_STING_TURN_FRAME/2);
	//		sting_work->spd_dec_dist = (eve_rec->width << FX32_SHIFT);
	//	}
	}
#else
	{
		fx32	time;
		if ((eve_rec->width << FX32_SHIFT) < GMD_ENE_STING_DEC_SPD_DIST) {
			// 標準減速距離より狭い
			sting_work->spd_dec_dist = (eve_rec->width << FX32_SHIFT);
		}
		else {
			sting_work->spd_dec_dist = GMD_ENE_STING_DEC_SPD_DIST;
		}

		time = FX_Div(sting_work->spd_dec_dist << 1, GMD_ENE_STING_SPD_X);
		if (time) {
			sting_work->spd_dec = FX_Div(GMD_ENE_STING_SPD_X, time);
		}
		else {
			sting_work->spd_dec = GMD_ENE_STING_SPD_X/2;
		}
	}
#endif

	// 歩きへ移行
	gmEneStingWalkInit(obj_work);

	// バーニアエフェクト生成
	gmEneStingCreateJetEfct(sting_work);

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}

// ==========================================================================
// 弾
// ==========================================================================
// ==========================================================================
// GmEneStingCreateBullet
/*!
 *	エネミー スティンガー 弾 メレオン共用
 *
 *	@param	parent_obj		[in]	親オブジェクト
 *	@parma	ofst_flash_x	[in]	フラッシュエフェクトオフセットX
 *	@parma	ofst_flash_y	[in]	フラッシュエフェクトオフセットY
 *	@parma	ofst_flash_z	[in]	フラッシュエフェクトオフセットZ
 *	@parma	ofst_bul_x		[in]	弾エフェクトオフセットX
 *	@parma	ofst_bul_y		[in]	弾エフェクトオフセットY
 *	@parma	ofst_bul_z		[in]	弾エフェクトオフセットZ
 *	@parma	spd_x			[in]	弾速度X
 *	@parma	spd_y			[in]	弾速度Y
 *	@parma	dir				[in]	弾角度
 */
// ==========================================================================
void GmEneStingCreateBullet(OBS_OBJECT_WORK *parent_obj, fx32 ofst_flash_x, fx32 ofst_flash_y, fx32 ofst_flash_z,
							fx32 ofst_bul_x, fx32 ofst_bul_y, fx32 ofst_bul_z,
							fx32 spd_x, fx32 spd_y, Angle16 dir)
{
	GMS_EFFECT_COM_WORK		*atk_obj;
	OBS_RECT_WORK			*rect_work;
	GMS_EFFECT_3DES_WORK	*efct_work;

	atk_obj = (GMS_EFFECT_COM_WORK*)GmEneComCreateAtkObject(parent_obj, GMD_ENE_STING_BULLET_CLIP_OFST);
	atk_obj->obj_work.parent_obj = NULL;	// 親OFF

	// オフセット反映
	atk_obj->obj_work.pos.x +=
			((parent_obj->disp_flag & OBD_DISP_HFLIP) ? -ofst_bul_x : ofst_bul_x);
	atk_obj->obj_work.pos.y += ofst_bul_y;
	atk_obj->obj_work.pos.z += ofst_bul_z;
							
	// 矩形設定
	rect_work = &atk_obj->rect_work[GME_EFFECT_RECT_ATK];
	ObjRectWorkSet(rect_work, -8, -8, 8, 8);
	rect_work->flag |= OBD_RECT_ENABLE;

	// 速度設定
	atk_obj->obj_work.spd.x = spd_x;
	atk_obj->obj_work.spd.y = spd_y;

	// クリッピング範囲設定
	atk_obj->obj_work.view_out_ofst = 16;

	// エフェクト生成
	// 弾
	efct_work = GmEfctCmnEsCreate(&atk_obj->obj_work, GME_EFCT_CMN_IDX_BULLET_CORE);
	GmComEfctSetDispRotationS(efct_work, 0, 0, dir);
	efct_work->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_FIX;

	// 発射
	efct_work = GmEfctCmnEsCreate(parent_obj, GME_EFCT_CMN_IDX_BULLET);
	GmComEfctSetDispRotationS(efct_work, 0, 0, (Angle16)(dir-0x8000));
	efct_work->efct_com.obj_work.parent_obj = NULL;
	// オフセット反映
	efct_work->efct_com.obj_work.pos.x +=
			((parent_obj->disp_flag & OBD_DISP_HFLIP) ? -ofst_flash_x : ofst_flash_x);
	efct_work->efct_com.obj_work.pos.y += ofst_flash_y;
	efct_work->efct_com.obj_work.pos.z += ofst_flash_z;
}




//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneStingWalkInit
/*!
 *	エネミー スティンガー Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneStingWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;

	ene_work	= (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_STING_MTN_ENE_STING_MOVE_ZNM, IDB_ENE_STING_MTN_ENE_STING_MOVE_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_STING_MTN_ENE_STING_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneStingWalkMain;

	//obj_work->move_flag &= ~OBD_MOVE_FRONT;

	
#if GMD_ENEMY_TEST
	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->dir.y = 0x4000;
	}
	else {
		obj_work->dir.y = 0xc000;
	}
#endif
	
#if 0
	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_STING_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_STING_SPD_X;
	}
#else
	//obj_work->spd.x = 0;
#endif
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	エネミー スティンガー Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_work	: 左移動限界\n
 *		user_flag	: 右移動限界
 */
// ==========================================================================
void gmEneStingWalkMain(OBS_OBJECT_WORK *obj_work)
{
#if 1
	GMS_ENE_STING_WORK	*sting_work;
	BOOL				b_dec;

	sting_work	= (GMS_ENE_STING_WORK*)obj_work;

	b_dec = gmEneStingSetWalkSpeed(sting_work);
	if (b_dec) {
		// 減速を開始した

		// フリップへ移行
		gmEneStingFlipInit(obj_work);
	}
#else
	GMS_ENE_STING_WORK	*sting_work;

	sting_work	= (GMS_ENE_STING_WORK*)obj_work;


	if (!obj_work->spd.x ||
			!GmEneComCheckMoveLimit(obj_work, (fx32)obj_work->user_work, (fx32)obj_work->user_flag)) {
		// 速度が0・移動範囲外

		// フリップへ移行
		gmEneStingFlipInit(obj_work);
		return;
	}

	// 減速チェック
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// 左向き
		if (obj_work->pos.x <= ((fx32)obj_work->user_work + sting_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, sting_work->spd_dec);

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -sting_work->spd_dec) {
					obj_work->spd.x = -sting_work->spd_dec;
				}
			}
		}
	}
	else {
		// 右向き
		if (obj_work->pos.x >= ((fx32)obj_work->user_flag - sting_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, sting_work->spd_dec);

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > sting_work->spd_dec) {
					obj_work->spd.x = sting_work->spd_dec;
				}
			}
		}
	}
#endif
}


// ==========================================================================
// gmEneStingFlipInit
/*!
 *	エネミー スティンガー フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneStingFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_STING_WORK	*sting_work;
	sting_work = (GMS_ENE_STING_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_STING_MTN_ENE_STING_FRIP_ZNM, IDB_ENE_STING_MTN_ENE_STING_FRIP_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_STING_MTN_ENE_STING_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneStingFlipMain;

	// 移動速度設定
	//obj_work->spd.x = 0;

	// サーチ矩形OFF
	sting_work->search_rect_work.flag &= ~OBD_RECT_ENABLE;

	// エフェクトOFF
	gmEneStingClearJetEfct(sting_work);
}

// ==========================================================================
// gmEneStingFlipMain
/*!
 *	エネミー スティンガー フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneStingFlipMain(OBS_OBJECT_WORK *obj_work)
{
	// 移動速度設定
	gmEneStingSetWalkSpeed((GMS_ENE_STING_WORK*)obj_work);

	if (obj_work->disp_flag & OBD_DISP_END) {
		GMS_ENE_STING_WORK	*sting_work;
		sting_work = (GMS_ENE_STING_WORK*)obj_work;

		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		// エフェクト再生成
		gmEneStingCreateJetEfct(sting_work);

#if GMD_ENE_STING_JET_S_SMOKE_ON
		sting_work->efct_smoke->efct_com.obj_work.disp_flag &= ~OBD_DISP_NODISP;
#endif
		// Walkへ
		gmEneStingWalkInit(obj_work);
		// サーチ矩形ON
		sting_work	= (GMS_ENE_STING_WORK*)obj_work;
		sting_work->search_rect_work.flag |= OBD_RECT_ENABLE;
	}

	{
		NNS_TRS	*trs_data, *trs_data_work;

		trs_data = obj_work->obj_3d->motion->data;
		//motion->data	= (NNS_TRS *)(motion->mmtn + mmotion_num);
		trs_data_work = trs_data;
	}
}


// ==========================================================================
// gmEneStingAtkInit
/*!
 *	エネミー スティンガー 攻撃初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneStingAtkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_STING_WORK	*sting_work;
	sting_work = (GMS_ENE_STING_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_STING_MTN_ENE_STING_ATK_01_ZNM, IDB_ENE_STING_MTN_ENE_STING_ATK_L_01_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_STING_MTN_ENE_STING_ATK_01_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneStingAtkMain;

	//obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	obj_work->spd.x = 0;

	// エフェクトOFF
	//gmEneStingClearJetEfct(sting_work);
}

// ==========================================================================
// gmEneStingAtkMain
/*!
 *	エネミー スティンガー 攻撃 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneStingAtkMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_STING_WORK	*sting_work;

	sting_work	= (GMS_ENE_STING_WORK*)obj_work;

	if (obj_work->user_timer) {
		obj_work->user_timer--;

		if (obj_work->user_timer) {
			return;
		}

		if (obj_work->obj_3d->act_id[0] == IDB_ENE_STING_MTN_ENE_STING_ATK_01_ZNM ||
				obj_work->obj_3d->act_id[0] == IDB_ENE_STING_MTN_ENE_STING_ATK_L_01_ZNM) {
			// 弾発射
			GmEneStingCreateBullet(obj_work,
						GMD_ENE_STING_BULLET_FLASH_OFST_X, GMD_ENE_STING_BULLET_FLASH_OFST_Y, GMD_ENE_STING_BULLET_FLASH_OFST_Z,
						GMD_ENE_STING_BULLET_OFST_X, GMD_ENE_STING_BULLET_OFST_Y, GMD_ENE_STING_BULLET_OFST_Z,
						sting_work->bullet_spd_x, sting_work->bullet_spd_y, sting_work->bullet_dir);
			// アクション設定
			GmEneComActionSetDependHFlip(obj_work, IDB_ENE_STING_MTN_ENE_STING_ATK_02_ZNM, IDB_ENE_STING_MTN_ENE_STING_ATK_L_02_ZNM);
			//ObjDrawObjectActionSet(obj_work, IDB_ENE_STING_MTN_ENE_STING_ATK_02_ZNM);
			// SE
			GmSoundPlaySE("Sting");
		}
		else {
			// アクション設定
			GmEneComActionSetDependHFlip(obj_work, IDB_ENE_STING_MTN_ENE_STING_ATK_03_ZNM, IDB_ENE_STING_MTN_ENE_STING_ATK_L_03_ZNM);
			//ObjDrawObjectActionSet(obj_work, IDB_ENE_STING_MTN_ENE_STING_ATK_03_ZNM);
		}
	}

	if (obj_work->disp_flag & OBD_DISP_END) {
		switch (obj_work->obj_3d->act_id[0]) {
		case IDB_ENE_STING_MTN_ENE_STING_ATK_01_ZNM:
		case IDB_ENE_STING_MTN_ENE_STING_ATK_L_01_ZNM:
			// 弾発射前待機
			obj_work->user_timer = GMD_ENE_STING_BULLET_WAIT;
			//// アクション設定
			//ObjDrawObjectActionSet(obj_work, IDB_ENE_STING_MTN_ENE_STING_ATK_02_ZNM);
			//// 弾発射
			//GmEneStingCreateBullet(obj_work, sting_work->bullet_spd_x, sting_work->bullet_spd_y);
			break;

		case IDB_ENE_STING_MTN_ENE_STING_ATK_02_ZNM:
		case IDB_ENE_STING_MTN_ENE_STING_ATK_L_02_ZNM:
			// 弾発射後待機
			obj_work->user_timer = GMD_ENE_STING_BULLET_AFTER_WAIT;
			// アクション設定
			//ObjDrawObjectActionSet(obj_work, IDB_ENE_STING_MTN_ENE_STING_ATK_03_ZNM);
			break;

		case IDB_ENE_STING_MTN_ENE_STING_ATK_03_ZNM:
		case IDB_ENE_STING_MTN_ENE_STING_ATK_L_03_ZNM:
			// エフェクト再生成
			//gmEneStingCreateJetEfct(sting_work);

			// 歩きへ戻る
			gmEneStingWalkInit(obj_work);
			break;
		default:
			break;
		}
	}
}


// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneStingRegRectFunc
/*!
 *	エネミー スティンガー 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneStingSetWalkSpeed(GMS_ENE_STING_WORK *sting_work)
{
	BOOL			b_dec = FALSE;
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)sting_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// 左向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_STING_MTN_ENE_STING_FRIP_L_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_STING_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, sting_work->spd_dec, GMD_ENE_STING_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + sting_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, sting_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -sting_work->spd_dec) {
					obj_work->spd.x = -sting_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_STING_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -sting_work->spd_dec, GMD_ENE_STING_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_STING_MTN_ENE_STING_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_STING_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -sting_work->spd_dec, GMD_ENE_STING_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - sting_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, sting_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > sting_work->spd_dec) {
					obj_work->spd.x = sting_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_STING_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, sting_work->spd_dec, GMD_ENE_STING_SPD_X);
		}
	}

	return (b_dec);
}





// ==========================================================================
// 矩形関連
// ==========================================================================
// ==========================================================================
// gmEneStingRegRectFunc
/*!
 *	エネミー スティンガー 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneStingRegRectFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_STING_WORK	*sting_work;

	sting_work	= (GMS_ENE_STING_WORK*)obj_work;

	ObjObjectRectRegist(obj_work, &sting_work->search_rect_work);
//	if (obj_work->disp_flag & OBD_DISP_END) {
//		// フリップ終了
//		obj_work->disp_flag ^= OBD_DISP_HFLIP;
//		// Walkへ
//		gmEneStingWalkInit(obj_work);
//	}
}

// ==========================================================================
// gmEneStingSearchDefFunc
/*!
 *	エネミー スティンガー サーチ矩形発動処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 */
// ==========================================================================
void gmEneStingSearchDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_PLAYER_WORK		*ply_work;
	GMS_ENE_STING_WORK	*sting_work;
	OBS_RECT_WORK		*rect_work;
	Angle32				angle_s, angle_e;
	float				dist_x, dist_y;
	float				target_pos_x, target_pos_y;
	Angle32				target_angle;
	float				mine_pos_x, mine_pos_y;


	if (match_rect->parent_obj->obj_type != GMD_OBJTYPE_PLAYER) {
		// 相手がプレイヤーでなかった
		return;
	}

	sting_work	= (GMS_ENE_STING_WORK*)mine_rect->parent_obj;
	ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;

	// プレイヤーポジション
	rect_work = &ply_work->rect_work[GMD_PLAYER_RECT_BODY];
	target_pos_x = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.x) +
						(float)((rect_work->rect.left + rect_work->rect.right) >> 1);
	target_pos_y = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.y) +
						(float)((rect_work->rect.top + rect_work->rect.bottom) >> 1);

	// エネミー(自分)ポジション
	rect_work = &sting_work->ene_3d_work.ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	mine_pos_x = FXM_FX32_TO_FLOAT(sting_work->ene_3d_work.ene_com.obj_work.pos.x) +
						(float)((rect_work->rect.left + rect_work->rect.right) >> 1);
	mine_pos_y = FXM_FX32_TO_FLOAT(sting_work->ene_3d_work.ene_com.obj_work.pos.y + GMD_ENE_STING_GUN_OFST_Y) +
						(float)((rect_work->rect.top + rect_work->rect.bottom) >> 1);

	// サーチ範囲内チェック
	// サーチ範囲角度取得
	if (sting_work->ene_3d_work.ene_com.obj_work.disp_flag & OBD_DISP_HFLIP) {
		angle_s = 0x8000 - GMD_ENE_STING_SEARCH_DIR_START;
		angle_e = 0x8000 - GMD_ENE_STING_SEARCH_DIR_END;
	}
	else {
		angle_s = GMD_ENE_STING_SEARCH_DIR_START;
		angle_e = GMD_ENE_STING_SEARCH_DIR_END;
	}
	if (angle_e < angle_s) {
		MTM_MATH_SWAP(angle_s, angle_e);
	}

	// サーチ範囲角度チェック
#if 0
	dist_x = target_pos_x - mine_pos_x;
	dist_y = target_pos_y - mine_pos_y;
	target_angle = nnArcTan2(-dist_y, dist_x);	// 逆回転で判定
	if (target_angle < angle_s ||
			target_angle > angle_e) {
		// 範囲外
		return;
	}

	// サーチに引っかかった

	// 弾速度保存
	sting_work->bullet_spd_x = (fx32)(GMD_ENE_STING_BULLET_SPD * nnCos(target_angle));
	sting_work->bullet_spd_y = (fx32)(GMD_ENE_STING_BULLET_SPD * nnSin(target_angle));
	if (sting_work->ene_3d_work.ene_com.obj_work.disp_flag & OBD_DISP_HFLIP) {
		sting_work->bullet_dir = (Angle16)(target_angle - 0x8000);
	}
	else {
		sting_work->bullet_dir = (Angle16)(target_angle);
	}

#else
	dist_x = target_pos_x - mine_pos_x;
	dist_y = target_pos_y - mine_pos_y;
	target_angle = nnArcTan2(dist_y, dist_x);	// 逆回転で判定
	if (target_angle < angle_s ||
			target_angle > angle_e) {
		// 範囲外
		return;
	}

	// サーチに引っかかった

	// 弾速度保存
	sting_work->bullet_spd_x = (fx32)(GMD_ENE_STING_BULLET_SPD * nnCos(target_angle));
	sting_work->bullet_spd_y = (fx32)(GMD_ENE_STING_BULLET_SPD * nnSin(target_angle));
	if (sting_work->ene_3d_work.ene_com.obj_work.disp_flag & OBD_DISP_HFLIP) {
		sting_work->bullet_dir = (Angle16)(target_angle - 0x8000);
	}
	else {
		sting_work->bullet_dir = (Angle16)(target_angle);
	}
#endif

	// 攻撃開始
	gmEneStingAtkInit((OBS_OBJECT_WORK*)sting_work);

	// サーチ矩形OFF
	sting_work->search_rect_work.flag &= ~OBD_RECT_ENABLE;
}


// ==========================================================================
// モーションコールバック
// ==========================================================================
// ==========================================================================
// gmEneStingMotionCallback
/*!
 *	エネミー スティンガー モーションコールバック
 *
 *	@param motion	[in] モーション
 *	@param object	[in] オブジェクト
 *	@param param	[in] パラメータ
 */
// ==========================================================================
#define GMD_ENE_STING_NODE_ID_R_JET		(7)
#define GMD_ENE_STING_NODE_ID_L_JET		(8)
#define GMD_ENE_STING_NODE_ID_GUN		(4)
void gmEneStingMotionCallback(const AMS_MOTION *motion, const NNS_OBJECT *object, void *param)
{
	/* Node 4 : sting_gun */
	/* Node 5 : fire */
	/* Node 7 : R_jet */
	/* Node 8 : L_jet */

	NNS_MATRIX			node_mtx, base_mtx;
	GMS_ENE_STING_WORK	*sting_work = (GMS_ENE_STING_WORK*)param;

	// ベースマトリクス取得
	nnMakeUnitMatrix(&base_mtx);
	nnMultiplyMatrix(&base_mtx, &base_mtx, amMatrixGetCurrent());
	
	// 階層マトリクスを求める
	// R_jet
	nnCalcNodeMatrixTRSList(&node_mtx, object, 
			GMD_ENE_STING_NODE_ID_R_JET, 
			motion->data, &base_mtx);
	MI_CpuCopy8(&node_mtx, &sting_work->jet_r_mtx, sizeof(NNS_MATRIX));

	// L_jet
	nnCalcNodeMatrixTRSList(&node_mtx, object, 
			GMD_ENE_STING_NODE_ID_L_JET, 
			motion->data, &base_mtx);
	MI_CpuCopy8(&node_mtx, &sting_work->jet_l_mtx, sizeof(NNS_MATRIX));

	// fire 5 sting_gun 4
	nnCalcNodeMatrixTRSList(&node_mtx, object, 
			GMD_ENE_STING_NODE_ID_GUN, 
			motion->data, &base_mtx);
	MI_CpuCopy8(&node_mtx, &sting_work->gun_mtx, sizeof(NNS_MATRIX));
}



// ==========================================================================
// gmEneStingCreateJetEfct
/*!
 *	エネミー スティンガー ジェットエフェクト生成
 *
 *	@param sting_work	[in] スティンガーワーク
 */
// ==========================================================================
void gmEneStingCreateJetEfct(GMS_ENE_STING_WORK *sting_work)
{
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// バーニアエフェクト生成
	if (!sting_work->efct_r_jet) {
		sting_work->efct_r_jet = GmEfctEneEsCreate((OBS_OBJECT_WORK*)sting_work, GME_EFCT_ENE_IDX_E02_JET_S);
		GmComEfctAddDispOffsetF(sting_work->efct_r_jet, -11.f, -9.f, 0);
		sting_work->efct_r_jet->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_FIX_NODISP | OBD_OBJECT_NOCLIP;
		sting_work->efct_r_jet->efct_com.obj_work.user_work = (u32)&sting_work->jet_r_mtx;
		// メイン処理差し替え
		sting_work->efct_r_jet->efct_com.obj_work.ppFunc = gmEneStingJetEfctMain;
		// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	}
	if (!sting_work->efct_l_jet) {
		sting_work->efct_l_jet = GmEfctEneEsCreate((OBS_OBJECT_WORK*)sting_work, GME_EFCT_ENE_IDX_E02_JET_S);
		GmComEfctAddDispOffsetF(sting_work->efct_l_jet, -11.f, -9.f, 0);
		sting_work->efct_l_jet->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_FIX_NODISP | OBD_OBJECT_NOCLIP;
		sting_work->efct_l_jet->efct_com.obj_work.user_work = (u32)&sting_work->jet_l_mtx;
		// メイン処理差し替え
		sting_work->efct_l_jet->efct_com.obj_work.ppFunc = gmEneStingJetEfctMain;
		// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
	}

#if GMD_ENE_STING_JET_S_SMOKE_ON
	if (!sting_work->efct_smoke) {
		sting_work->efct_smoke = GmEfctEneEsCreate((OBS_OBJECT_WORK*)sting_work, GME_EFCT_ENE_IDX_E02_JET_S_SMORK);
		GmComEfctAddDispOffsetF(sting_work->efct_smoke, -9.f, -8.f, 0);
		sting_work->efct_smoke->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_FIX_NODISP;
		// メイン処理差し替え
		sting_work->efct_jet->efct_com.obj_work.ppFunc = gmEneStingJetEfctMain;
	}
#endif
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
}

// ==========================================================================
// gmEneStingClearJetEfct
/*!
 *	エネミー スティンガー ジェットエフェクト破棄
 *
 *	@param sting_work	[in] スティンガーワーク
 */
// ==========================================================================
void gmEneStingClearJetEfct(GMS_ENE_STING_WORK *sting_work)
{
	// バーニアエフェクト破棄
	if (sting_work->efct_r_jet) {
		ObjDrawKillAction3DES(&sting_work->efct_r_jet->efct_com.obj_work);
		//sting_work->efct_jet->efct_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		sting_work->efct_r_jet = NULL;
	}
	if (sting_work->efct_l_jet) {
		ObjDrawKillAction3DES(&sting_work->efct_l_jet->efct_com.obj_work);
		//sting_work->efct_jet->efct_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		sting_work->efct_l_jet = NULL;
	}

#if GMD_ENE_STING_JET_S_SMOKE_ON
	if (sting_work->efct_smoke) {
		ObjDrawKillAction3DES(&sting_work->efct_smoke->efct_com.obj_work);
		//sting_work->efct_smoke->efct_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		sting_work->efct_smoke = NULL;
	}
#endif
}

// ==========================================================================
// gmEneStingJetEfctMain
/*!
 *	スティンガージェットエフェクト
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_work	: NNS_MATRIX*
 */
// ==========================================================================
void gmEneStingJetEfctMain(OBS_OBJECT_WORK *obj_work)
{
	NNS_MATRIX		*mtx = (NNS_MATRIX*)obj_work->user_work;
	NNS_VECTOR		vec;

	MTM_ASSERT(mtx);

	if (obj_work->parent_obj == NULL) {
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		return;
	}

	vec.x = NNM_MTX(*mtx, 0, 3) - FXM_FX32_TO_FLOAT(obj_work->parent_obj->pos.x);
	vec.y = -NNM_MTX(*mtx, 1, 3) - FXM_FX32_TO_FLOAT(obj_work->parent_obj->pos.y);
	vec.z = NNM_MTX(*mtx, 2, 3) - FXM_FX32_TO_FLOAT(obj_work->parent_obj->pos.z);


	if (obj_work->parent_obj->disp_flag & OBD_DISP_HFLIP) {
		vec.x = -vec.x;
		vec.z = -vec.z;
	}
	vec.x += -3.f;	// オフセット

	GmComEfctSetDispOffsetF((GMS_EFFECT_3DES_WORK*)obj_work,
				vec.x, vec.y, vec.z);


	// 汎用処理
	GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(obj_work);
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
