// ==========================================================================
/*!
  @file gmEneBuku.cpp
  @brief エネミー ブクブク

  @author Yurita
				Copyright(c) 2009 Dimps

  $Id: gmEneBukuBuku.cpp 2 2011-04-11 05:21:26Z thamada $
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

#include "gmEneCom.h"
#include "gmEneBukubuku.h"

#include "gmEffect.h"
#include "gmComEfct.h"
#include "gmEffectCmn.h"
#include "gmEffectEnemy.h"

// データヘッダ
#include "common/model/ene_buku_mtn.hmb"
#include "common/model/ene_buku_mdl.hmb"


#if	defined(_WII) && !defined(_PS3)
#pragma warning off (10178)			// 型宣言のプロトタイプのワーニングをOFFにする
									// ただしstatic宣言はつけれないため、問題がある場合は直接修正する
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
#endif	//_WII
//----- Definitions ---------------------------------------------------------

//===========================================================================
// ツール設定でのイベント関係
//===========================================================================
// GMS_EVE_RECORD_EVENT.flag
#define		GMD_ENE_BUKU_EVE_FLAG_RIGHT					(0x0001)	//!< 右向き開始


//===========================================================================
//	反転関係
//===========================================================================
#define		GMD_ENE_BUKU_MOVE_SPD_X						(0x0800)			//!< 移動速度
#define		GMD_ENE_BUKU_FW_TIME						(15*FX32_ONE)		//!< FW時間
#define		GMD_ENE_BUKU_TURN_FRAME						(40)				//!< ターンモーションフレーム

//===========================================================================
//	泡関係
//===========================================================================
//#define	GMD_ENE_BUKU_EFCT_BUBBLE_SPD_Y_ACC			(-0x100)		//!< 泡加速度
//#define	GMD_ENE_BUKU_EFCT_BUBBLE_SPD_Y_MAX			(-0x10000)		//!< 泡最大速度
//#define	GMD_ENE_BUKU_EFCT_BUBBLE_SURFACE_ADJUST		(8*FX32_ONE)	//!< 泡水面クリッピング補正値
#define		GMD_ENE_BUKU_EFCT_BUBBLE_X					(-24.0f)			//!< 泡の出現位置
#define		GMD_ENE_BUKU_EFCT_BUBBLE_Y					(-5.0f)			//!< 泡の出現位置

//===========================================================================
/// ブクブクワーク
//===========================================================================
typedef struct tag_GMS_ENE_BUKU_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離

	Sint32					timer;

} GMS_ENE_BUKU_WORK;
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmEneBukuWalkInit(OBS_OBJECT_WORK *obj_work);
static void gmEneBukuWalkMain(OBS_OBJECT_WORK *obj_work);
//static void gmEneBukuFwInit(OBS_OBJECT_WORK *obj_work);
//static void gmEneBukuFwMain(OBS_OBJECT_WORK *obj_work);
static void gmEneBukuFlipInit(OBS_OBJECT_WORK *obj_work);
static void gmEneBukuFlipMain(OBS_OBJECT_WORK *obj_work);

static BOOL gmEneBukuSetWalkSpeed(GMS_ENE_BUKU_WORK *buku_work);

//static void gmEneBukuEfctBubbleMain(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_buku_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneBukuBuild
/*!
 *	エネミー ブクブク データ構築
 */
// ==========================================================================
void GmEneBukuBuild(void)
{
	gm_ene_buku_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_BUKU_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_BUKU_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneBukuFlush
/*!
 *	エネミー ブクブク データ片付け
 */
// ==========================================================================
void GmEneBukuFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_BUKU_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_buku_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneBukuInit
/*!
 *	エネミー ブクブク 初期化関数
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
OBS_OBJECT_WORK* GmEneBukuInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_BUKU_WORK	*buku_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_BUKU_WORK), "ENE_BUKU");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	buku_work = (GMS_ENE_BUKU_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_buku_obj_3d_list[IDB_ENE_BUKU_MDL_ENE_BUKU_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_BUKU_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

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
	ObjRectWorkSet(rect_work, -8, -24+16, 8, 0+8);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// くらい
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -16, -32+16, 16, 0+16);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// ボディ
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, -32+16, 19, 0+16);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// 地形あたり
	//ObjObjectFieldRectSet(obj_work, -4, -8, 4, -2);

	// フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;	// 地形あたり無し
	obj_work->move_flag &= ~OBD_MOVE_FALL;	// 重力なし
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	//obj_work->move_flag |= OBD_MOVE_FALL;
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	//obj_work->move_flag |= OBD_MOVE_FALL;

	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_BUKU_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
	buku_work->spd_dec		 = GMD_ENE_BUKU_MOVE_SPD_X / (GMD_ENE_BUKU_TURN_FRAME/2);
	buku_work->spd_dec_dist = GMD_ENE_BUKU_MOVE_SPD_X * (GMD_ENE_BUKU_TURN_FRAME/2) / 2;

	gmEneBukuWalkInit(obj_work);
#if !_IPHONE
	GMS_EFFECT_3DES_WORK*	efct_work;
	efct_work = GmEfctEneEsCreate( obj_work, GME_EFCT_ENE_IDX_E10_BUKUBUKU);
	// 尻尾
	GmComEfctSetDispOffsetF(efct_work, GMD_ENE_BUKU_EFCT_BUBBLE_X, GMD_ENE_BUKU_EFCT_BUBBLE_Y, 0.0f);
#endif // !_IPHONE

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}



//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneBukuWalkInit
/*!
 *	エネミー ブクブク Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneBukuWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_BUKU_MTN_ENE_BUKU_MOVE_ZNM, IDB_ENE_BUKU_MTN_ENE_BUKU_MOVE_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_BUKU_MTN_ENE_BUKU_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneBukuWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_BUKU_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_BUKU_MOVE_SPD_X;
	}
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	エネミー ブクブク Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneBukuWalkMain(OBS_OBJECT_WORK *obj_work)
{
#if 1
	BOOL				b_dec;
	GMS_ENE_BUKU_WORK	*buku_work;

	buku_work	= (GMS_ENE_BUKU_WORK*)obj_work;

	b_dec = gmEneBukuSetWalkSpeed(buku_work);

	if (b_dec) {
		// 減速を開始した

		// フリップへ移行
		gmEneBukuFlipInit(obj_work);
	}
#else
	if (obj_work->move_flag & OBD_MOVE_FRONT ||
			!GmEneComCheckMoveLimit(obj_work, (fx32)obj_work->user_work, (fx32)obj_work->user_flag)) {
		// 前方衝突・移動範囲外

		// フリップへ移行
		gmEneBukuFlipInit(obj_work);
		// FWへ移行
		//gmEneBukuFwInit(obj_work);
	}
#endif


	if (buku_work->timer>0){
		buku_work->timer--;
		return;
	}

//	GMS_EFFECT_3DES_WORK*	efct_work;
//	efct_work = GmEfctEneEsCreate( obj_work, GME_EFCT_ENE_IDX_E10_BUKUBUKU);
	//efct_work = GmEfctCmnEsCreate( obj_work, GME_EFCT_CMN_IDX_BUBBLE);
//	efct_work->efct_com.obj_work.pos.x = obj_work->pos.x;
//	efct_work->efct_com.obj_work.pos.y = obj_work->pos.y;

	/*
	// 尾っぽにつける
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		GmComEfctSetDispOffsetF(efct_work, 15.f, 0.f, 0.f);
//		GmEffect3DESAddDispOffset(efct_work, 80.0f, 0.0f, 0.0f);
//		efct_work->efct_com.obj_work.pos.x += FX_F32_TO_FX32( 50 );
	}else{
		GmComEfctSetDispOffsetF(efct_work, -15.f, 0.f, 0.f);
//		GmEffect3DESAddDispOffset(efct_work, -80.0f, 0.0f, 0.0f);
//		efct_work->efct_com.obj_work.pos.x -= FX_F32_TO_FX32( 50 );
	}
	efct_work->efct_com.obj_work.ppFunc = gmEneBukuEfctBubbleMain;
	efct_work->efct_com.obj_work.move_flag &= ~OBD_MOVE_NOMOVE;
//	efct_work->obj_3des.flag |= OBD_ACTFLAG_3D_ES_PARTICLE_DEPEND;
	*/

	buku_work->timer = 3600*60 + (s32)(mtMathRand() % 30);
}

#if 0
// ==========================================================================
// gmEneBukuEfctBubbleMain
/*!
 *	エネミー ブクブク 泡処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneBukuEfctBubbleMain(OBS_OBJECT_WORK *obj_work)
{
	// 速度設定
	obj_work->spd.y += GMD_ENE_BUKU_EFCT_BUBBLE_SPD_Y_ACC;
	if (obj_work->spd.y < GMD_ENE_BUKU_EFCT_BUBBLE_SPD_Y_MAX) {
		obj_work->spd.y = GMD_ENE_BUKU_EFCT_BUBBLE_SPD_Y_MAX;
	}
	
	if ((obj_work->pos.y + obj_work->spd.y) <
			(((s32)g_gm_main_system.water_level << FX32_SHIFT) + GMD_ENE_BUKU_EFCT_BUBBLE_SURFACE_ADJUST)) {
		obj_work->spd.y =
				((s32)g_gm_main_system.water_level << FX32_SHIFT) + GMD_ENE_BUKU_EFCT_BUBBLE_SURFACE_ADJUST - obj_work->pos.y;
	}
	
	//obj_work->pos.y += obj_work->spd.y;

	obj_work->user_timer = ObjTimeCountUp(obj_work->user_timer);

	if ((obj_work->user_timer >> FX32_SHIFT) & 0x03) {
		obj_work->spd.x = (mtMathRand() & 0xFFF) - 0x800;
	}

	// デフォルト処理
	GmEffectDefaultMainFuncDeleteAtEnd(obj_work);
}
#endif

#if 0
// ==========================================================================
// gmEneBukuFwInit
/*!
 *	エネミー ブクブク FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneBukuFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_BUKU_MTN_ENE_BUKU_FW_ZNM, IDB_ENE_BUKU_MTN_ENE_BUKU_FW_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_BUKU_MTN_ENE_BUKU_FW_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneBukuFwMain;

	// 移動速度設定
	obj_work->spd.x = 0;

	// 演出時間
	obj_work->user_timer = GMD_ENE_BUKU_FW_TIME;
}
#endif

// ==========================================================================
// gmEneBukuFwMain
/*!
 *	エネミー ブクブク FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneBukuFwMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	if (obj_work->user_timer <= 0) {
		// フリップへ移行
		gmEneBukuFlipInit(obj_work);
	}
}



// ==========================================================================
// gmEneBukuFlipInit
/*!
 *	エネミー ブクブク フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneBukuFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_BUKU_MTN_ENE_BUKU_FRIP_ZNM, IDB_ENE_BUKU_MTN_ENE_BUKU_FRIP_L_ZNM);
	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_BUKU_MTN_ENE_BUKU_FRIP_ZNM, IDB_ENE_BUKU_MTN_ENE_BUKU_FRIP_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_BUKU_MTN_ENE_BUKU_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneBukuFlipMain;

	// 移動速度設定
	//obj_work->spd.x = 0;
}

// ==========================================================================
// gmEneBukuFlipMain
/*!
 *	エネミー ブクブク フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneBukuFlipMain(OBS_OBJECT_WORK *obj_work)
{
	// 移動速度設定
	gmEneBukuSetWalkSpeed((GMS_ENE_BUKU_WORK*)obj_work);

	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		// Walkへ
		gmEneBukuWalkInit(obj_work);
	}
}


// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneBukuSetWalkSpeed
/*!
 *	エネミー ブクブク 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneBukuSetWalkSpeed(GMS_ENE_BUKU_WORK *buku_work)
{
	BOOL			b_dec = FALSE;
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)buku_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// 左向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_BUKU_MTN_ENE_BUKU_FRIP_L_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_BUKU_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, buku_work->spd_dec, GMD_ENE_BUKU_MOVE_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + buku_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, buku_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -buku_work->spd_dec) {
					obj_work->spd.x = -buku_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_BUKU_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -buku_work->spd_dec, GMD_ENE_BUKU_MOVE_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_BUKU_MTN_ENE_BUKU_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_BUKU_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -buku_work->spd_dec, GMD_ENE_BUKU_MOVE_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - buku_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, buku_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > buku_work->spd_dec) {
					obj_work->spd.x = buku_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_BUKU_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, buku_work->spd_dec, GMD_ENE_BUKU_MOVE_SPD_X);
		}
	}

	return (b_dec);
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
