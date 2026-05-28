// ==========================================================================
/*!
  @file gmEneMereon.cpp
  @brief エネミー メレオン

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEneMereon.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *		IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_L_ZNM と IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_ZNM、
 *		IDB_ENE_MEREON_MTN_ENE_MEREON_B_ATK_L_ZNM と IDB_ENE_MEREON_MTN_ENE_MEREON_B_ATK_ZNM を、 
 *		それぞれ右向きデータ 左向きデータ(逆転) として扱っています。
 *
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
#include "gmEffectCmn.h"
#include "gmEffectEnemy.h"
#include "gmComEfct.h"
#include "gmSound.h"
#include "gmDeco.h"

#include "gmEneCom.h"
#include "gmEneSting.h"
#include "gmEneMereon.h"

// データヘッダ
#include "common/model/ENE_MEREON_MTN.HMB"
#include "common/model/ENE_MEREON_MDL.HMB"
#include "common/model/ENE_MEREON_R_MDL.HMB"


//----- Definitions ---------------------------------------------------------


#if 0
// GMS_EVE_RECORD_EVENT.flag
#define GMD_ENE_MOTORA_EVE_FLAG_RIGHT	(0x0001)	//!< 右向き開始


#define GMD_ENE_MOTORA_MOVE_SPD_X		(0x0800)			//!< 移動速度
#define GMD_ENE_MOTORA_FW_TIME			(15*FX32_ONE)		//!< FW時間

#endif

#define GMD_ENE_MEREON_HIDE_SEARCH_DIST			(96)		//!< プレイヤーサーチ距離
#define GMD_ENE_MEREON_ROCKET_HIDE_SEARCH_DIST	(192)		//!< プレイヤーサーチ距離
#define GMD_ENE_MEREON_APPEAR_TIME				(30)		//!< 出現時間
#define GMD_ENE_MEREON_ROCKET_APPEAR_TIME		(15)		//!< 出現時間

#define GMD_ENE_MEREON_CAMOUFLAGE_APPEAR_BASE	(0.1f)		//!< カモフラージュ出現時ベース量
#define GMD_ENE_MEREON_CAMOUFLAGE_APPEAR_REST	(0.9f)		//!< カモフラージュ出現時残り量
#define GMD_ENE_MEREON_CAMOUFLAGE_HIDE_BASE		(0.0f)		//!< カモフラージュ隠れ時ベース量
#define GMD_ENE_MEREON_CAMOUFLAGE_HIDE_REST		(1.0f)		//!< カモフラージュ隠れ時残り量

#if 0
fx32 bullet_ofst_x = -24 * FX32_ONE;
fx32 bullet_ofst_y = -12 * FX32_ONE;
fx32 bullet_ofst_z = 0 * FX32_ONE;
fx32 bullet_flash_ofst_x = -20 * FX32_ONE;
fx32 bullet_flash_ofst_y = -12 * FX32_ONE;
fx32 bullet_flash_ofst_z = 8 * FX32_ONE;
#define GMD_ENE_MEREON_BULLET_OFST_X			(bullet_ofst_x)			//!< 弾エフェクトオフセット
#define GMD_ENE_MEREON_BULLET_OFST_Y			(bullet_ofst_y)
#define GMD_ENE_MEREON_BULLET_OFST_Z			(bullet_ofst_z)
#define GMD_ENE_MEREON_BULLET_FLASH_OFST_X		(bullet_flash_ofst_x)			//!< 弾発射エフェクトオフセット
#define GMD_ENE_MEREON_BULLET_FLASH_OFST_Y		(bullet_flash_ofst_y)
#define GMD_ENE_MEREON_BULLET_FLASH_OFST_Z		(bullet_flash_ofst_z)
#else
#define GMD_ENE_MEREON_BULLET_OFST_X			(-24 * FX32_ONE)			//!< 弾エフェクトオフセット
#define GMD_ENE_MEREON_BULLET_OFST_Y			(-12 * FX32_ONE)
#define GMD_ENE_MEREON_BULLET_OFST_Z			(0 * FX32_ONE)
#define GMD_ENE_MEREON_BULLET_FLASH_OFST_X		(-20 * FX32_ONE)			//!< 弾発射エフェクトオフセット
#define GMD_ENE_MEREON_BULLET_FLASH_OFST_Y		(-12 * FX32_ONE)
#define GMD_ENE_MEREON_BULLET_FLASH_OFST_Z		(8 * FX32_ONE)
#endif

#define GMD_ENE_MEREON_BULLET_SPD_X				((fx32)(-2.0 * FX32_ONE))	//!< 弾エフェクトオフセット
#define GMD_ENE_MEREON_BULLET_SPD_Y				((fx32)(0 * FX32_ONE))

#define GMD_ENE_MEREON_BULLET_END_WAIT_TIME		(30)						//!< シーケンス移行待機時間

#define GMD_ENE_MEREON_HIDE_TIME				(30)//(8)							//!< 消失時間


#define GMD_ENE_MEREON_ROCKET_FALL_DIR			(0x1000)					//!< 落下回転を行う場合の回転量
#define GMD_ENE_MEREON_ROCKET_SPD				(0x2000)					//!< メレオンロケット速度

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmEneMereonHideSearchInit(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonHideSearchMain(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonAppearInit(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonAppearMain(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonAtkInit(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonAtkMain(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonHideInit(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonHideMain(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonAtkRocketInit(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonRocketFallMain(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonRocketMain(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonCheckFwFlip(OBS_OBJECT_WORK *obj_work);

static void gmEneMereonCamouflageDrawFunc(OBS_OBJECT_WORK *obj_work);
static void gmEneMereonDrawPreUserFunc(void *data);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_mereon_obj_3d_list = NULL;
static OBS_ACTION3D_NN_WORK *gm_ene_mereon_r_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneMereonBuild
/*!
 *	エネミー メレオン データ構築
 */
// ==========================================================================
void GmEneMereonBuild(void)
{
	// メレオン
	gm_ene_mereon_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MEREON_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MEREON_TEX),
#if _PC || _PS3 || _XBOX
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON | NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1/*draw_flag*/);
#else
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
#endif

	// ロケットメレオン
	gm_ene_mereon_r_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MEREON_R_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MEREON_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneMereonFlush
/*!
 *	エネミー メレオン データ片付け
 */
// ==========================================================================
void GmEneMereonFlush(void)
{
	AMS_AMB_HEADER	*amb;

	amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MEREON_MODEL);
	GmGameDBuildRegFlushModel(gm_ene_mereon_obj_3d_list, amb->file_num);

	amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MEREON_R_MODEL);
	GmGameDBuildRegFlushModel(gm_ene_mereon_r_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneMereonInit
/*!
 *	エネミー メレオン 初期化関数
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
OBS_OBJECT_WORK* GmEneMereonInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "ENE_MEREON");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_mereon_obj_3d_list[IDB_ENE_MEREON_MDL_ENE_MEREON_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_MEREON_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

	// 描画処理設定
#if _PC || _PS3 || _XBOX
	obj_work->ppOut = gmEneMereonCamouflageDrawFunc;
	obj_work->obj_3d->command_state = OBD_DRAW_CMD_STATE_PRE_WATER;
#endif

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
	ObjRectWorkSet(rect_work, -11, -24, 11, 0);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// くらい
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, -32, 19, 0);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// ボディ
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, -32, 19, 0);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// 地形あたり
	ObjObjectFieldRectSet(obj_work, -4, -8, 4, 0);

	// フラグ
#if _PC || _PS3 || _XBOX
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1;	// 迷彩あり
#else
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_ALPHA;			// アルファ制御あり
#endif
	obj_work->move_flag |= OBD_MOVE_NOCOL;	// 地形あたり無し 落下なし
	obj_work->move_flag &= ~OBD_MOVE_FALL;
	obj_work->flag |= OBD_OBJECT_NOHIT;		// はじめはあたらない
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// 初期アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_ZNM, IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_L_ZNM);

	if (eve_rec->id == GMD_EVENT_ID_ENE_MEREON) {
		// メレオンステルスタイプ はサーチ対象からはずす
		ene_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;
	}

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	// 隠れサーチシーケンスへ移行
	gmEneMereonHideSearchInit(obj_work);
	return (obj_work);
}



//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneMereonHideSearchInit
/*!
 *	エネミー メレオン 隠れサーチ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMereonHideSearchInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	GMS_PLAYER_WORK		*ply_work;

	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	obj_work->disp_flag &= ~OBD_DISP_HFLIP;
	if (ply_work->obj_work.pos.x < obj_work->pos.x) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}

	// アクション設定
	gmEneMereonCheckFwFlip(obj_work);
	obj_work->disp_flag |= OBD_DISP_NODISP;	// 非表示


	// メイン処理
	obj_work->ppFunc = gmEneMereonHideSearchMain;

	//obj_work->move_flag &= ~OBD_MOVE_FRONT;
}

// ==========================================================================
// gmEneMereonHideSearchMain
/*!
 *	エネミー メレオン 隠れサーチ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMereonHideSearchMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_COM_WORK	*ene_com;
	GMS_PLAYER_WORK		*ply_work;
	OBS_RECT_WORK		*rect_work;
	float				ply_pos[MTD_XY], mine_pos[MTD_XY];
	float				dist_x, dist_y;
	float				dist;

	// プレイヤー範囲進入チェック
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	rect_work = &ply_work->rect_work[GMD_PLAYER_RECT_BODY];
	ply_pos[MTD_X] = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.x +
							((rect_work->rect.top + rect_work->rect.bottom) >> 1));
	ply_pos[MTD_Y] = FXM_FX32_TO_FLOAT(ply_work->obj_work.pos.y +
							((rect_work->rect.left + rect_work->rect.right) >> 1));

	ene_com = (GMS_ENEMY_COM_WORK*)obj_work;
	rect_work = &ene_com->rect_work[GMD_ENEMY_RECT_BODY];
	mine_pos[MTD_X] = FXM_FX32_TO_FLOAT(obj_work->pos.x +
							((rect_work->rect.top + rect_work->rect.bottom) >> 1));
	mine_pos[MTD_Y] = FXM_FX32_TO_FLOAT(obj_work->pos.y +
							((rect_work->rect.left + rect_work->rect.right) >> 1));

	dist_x = mine_pos[MTD_X] - ply_pos[MTD_X];
	dist_y = mine_pos[MTD_Y] - ply_pos[MTD_Y];

	if (ene_com->eve_rec->id == GMD_EVENT_ID_ENE_MEREON) {
		dist = GMD_ENE_MEREON_HIDE_SEARCH_DIST;
	}
	else {
		dist = GMD_ENE_MEREON_ROCKET_HIDE_SEARCH_DIST;
	}

	if (dist_x*dist_x + dist_y*dist_y <= dist*dist) {
		// プレイヤー範囲内へ進入
		obj_work->disp_flag &= ~OBD_DISP_NODISP;	// 表示開始

		// 出現へ
		gmEneMereonAppearInit(obj_work);
	}
}

// ==========================================================================
// gmEneMereonAppearInit
/*!
 *	エネミー メレオン 出現初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_timer	: 出現時間
 */
// ==========================================================================
void gmEneMereonAppearInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_COM_WORK	*ene_com;
	GMS_PLAYER_WORK		*ply_work;

	ene_com = (GMS_ENEMY_COM_WORK*)obj_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	obj_work->disp_flag &= ~OBD_DISP_HFLIP;
	if (ene_com->eve_rec->id == GMD_EVENT_ID_ENE_MEREON) {
		// 通常
#if GMD_ENEMY_TEST
		obj_work->dir.y = 0xc000;
#endif
		if (ply_work->obj_work.pos.x > obj_work->pos.x) {
			obj_work->disp_flag |= OBD_DISP_HFLIP;
#if GMD_ENEMY_TEST
			obj_work->dir.y = 0x4000;
#endif
		}
	}	// ロケットタイプは、出現時は必ず右で
	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_ZNM, IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_L_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

#if _PC || _PS3 || _XBOX
	// 表示開始 迷彩
	obj_work->obj_3d->toon_camouflage = GMD_ENE_MEREON_CAMOUFLAGE_APPEAR_BASE;	// 開始量
#else
	// 表示開始 アルファ設定
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;	// ユーザー描画ステート反映
	obj_work->obj_3d->draw_state.alpha.alpha = 0.f;	// 非表示状態から
#endif

	obj_work->user_timer = 0;

	// メイン処理
	obj_work->ppFunc = gmEneMereonAppearMain;
}

// ==========================================================================
// gmEneMereonAppearMain
/*!
 *	エネミー メレオン 隠れサーチ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMereonAppearMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_COM_WORK	*ene_com = (GMS_ENEMY_COM_WORK*)obj_work;
	s32					appear_time;

	// 演出タイマー
	obj_work->user_timer = ObjTimeCountUp(obj_work->user_timer);

	// 半透明度設定
	if (ene_com->eve_rec->id == GMD_EVENT_ID_ENE_MEREON) {
		appear_time = GMD_ENE_MEREON_APPEAR_TIME;
	}
	else {
		appear_time = GMD_ENE_MEREON_ROCKET_APPEAR_TIME;
	}
#if _PC || _PS3 || _XBOX
	obj_work->obj_3d->toon_camouflage = GMD_ENE_MEREON_CAMOUFLAGE_APPEAR_BASE +
						(float)obj_work->user_timer / (float)(appear_time * FX32_ONE) * GMD_ENE_MEREON_CAMOUFLAGE_APPEAR_REST;
#else
	obj_work->obj_3d->draw_state.alpha.alpha = (float)obj_work->user_timer / (float)(appear_time * FX32_ONE);
#endif

	if (obj_work->user_timer >= appear_time * FX32_ONE) {
#if _PC || _PS3 || _XBOX
		obj_work->obj_3d->toon_camouflage = 1.f;
#else
		obj_work->obj_3d->draw_state.alpha.alpha = 1.f;
#endif
		obj_work->disp_flag &= ~OBD_DISP_DRAWSTATE;	// ユーザー描画ステート反映終了

		if (ene_com->eve_rec->id == GMD_EVENT_ID_ENE_MEREON) {
			// 通常メレオン
			gmEneMereonAtkInit(obj_work);
		}
		else {
			// ロケットメレオン
			gmEneMereonAtkRocketInit(obj_work);
		}
	}
}


// ==========================================================================
// gmEneMereonAtkInit
/*!
 *	エネミー メレオン 攻撃(弾)初期化
 *
 *	@param obj_work	[in] オブジェクトワークx
 */
// ==========================================================================
void gmEneMereonAtkInit(OBS_OBJECT_WORK *obj_work)
{
	// アクション設定
	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_MEREON_MTN_ENE_MEREON_B_ATK_ZNM, IDB_ENE_MEREON_MTN_ENE_MEREON_B_ATK_L_ZNM);

	// フラグ
	obj_work->flag &= ~OBD_OBJECT_NOHIT;		// あたりあり

	// メイン処理
	obj_work->ppFunc = gmEneMereonAtkMain;
	obj_work->user_timer = 0;
}

// ==========================================================================
// gmEneMereonAtkMain
/*!
 *	エネミー メレオン 攻撃(弾) メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_timer	: シーケンス移行待機時間
 */
// ==========================================================================
void gmEneMereonAtkMain(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END) {
		fx32	spd_x;
		Angle16	dir;

		spd_x = GMD_ENE_MEREON_BULLET_SPD_X;
		dir = -0x4000;
		if (obj_work->disp_flag & OBD_DISP_HFLIP) {
			spd_x	= -spd_x;
			dir		+= 0x8000;
		}

		// 弾発射
		GmEneStingCreateBullet(obj_work,
				GMD_ENE_MEREON_BULLET_FLASH_OFST_X, GMD_ENE_MEREON_BULLET_FLASH_OFST_Y, GMD_ENE_MEREON_BULLET_FLASH_OFST_Z,
				GMD_ENE_MEREON_BULLET_OFST_X, GMD_ENE_MEREON_BULLET_OFST_Y, GMD_ENE_MEREON_BULLET_OFST_Z,
				spd_x, GMD_ENE_MEREON_BULLET_SPD_Y, dir);

		// SE
		GmSoundPlaySE("Sting");

		// アクション設定
		GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_ZNM, IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_L_ZNM);

		// シーケンス移行待機時間設定
		obj_work->user_timer = GMD_ENE_MEREON_BULLET_END_WAIT_TIME * FX32_ONE;
	}

	if (obj_work->user_timer) {
		obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);
		if (!obj_work->user_timer) {
			// 隠れるへ移行
			gmEneMereonHideInit(obj_work);
		}
	}
}

// ==========================================================================
// gmEneMereonHideInit
/*!
 *	エネミー メレオン 隠れる初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_timer	: 演出時間
 */
// ==========================================================================
void gmEneMereonHideInit(OBS_OBJECT_WORK *obj_work)
{
	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MEREON_MTN_ENE_MEREON_B_ATK_L_ZNM, IDB_ENE_MEREON_MTN_ENE_MEREON_B_ATK_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneMereonHideMain;

	// 表示開始 アルファ設定
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;	// ユーザー描画ステート反映
#if _PC || _PS3 || _XBOX
	obj_work->obj_3d->toon_camouflage = 1.f;		// 表示状態から
#else
	obj_work->obj_3d->draw_state.alpha.alpha = 1.f;	// 表示状態から
#endif

	obj_work->user_timer = GMD_ENE_MEREON_HIDE_TIME * FX32_ONE;
}


// ==========================================================================
// gmEneMereonHideMain
/*!
 *	エネミー メレオン 隠れる メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMereonHideMain(OBS_OBJECT_WORK *obj_work)
{
	// 演出タイマー
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

#if _PC || _PS3 || _XBOX
	// 迷彩設定
		obj_work->obj_3d->toon_camouflage = GMD_ENE_MEREON_CAMOUFLAGE_HIDE_BASE +
					(float)obj_work->user_timer / (float)(GMD_ENE_MEREON_HIDE_TIME * FX32_ONE) * GMD_ENE_MEREON_CAMOUFLAGE_HIDE_REST;
#else
	// 半透明度設定
	obj_work->obj_3d->draw_state.alpha.alpha = (float)obj_work->user_timer / (float)(GMD_ENE_MEREON_HIDE_TIME * FX32_ONE);
#endif

	if (obj_work->user_timer <= 0) {
#if _PC || _PS3 || _XBOX
		obj_work->obj_3d->toon_camouflage = 1.f;
#else
		obj_work->obj_3d->draw_state.alpha.alpha = 1.f;
#endif
		obj_work->disp_flag &= ~OBD_DISP_DRAWSTATE;	// ユーザー描画ステート反映終了
		obj_work->disp_flag |= OBD_DISP_NODISP;				// 非表示へ

		// 通常終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
}

// ==========================================================================
// gmEneMereonAtkRocketInit
/*!
 *	エネミー メレオン 攻撃(ロケット)初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_timer	: 回転速度
 */
// ==========================================================================
void gmEneMereonAtkRocketInit(OBS_OBJECT_WORK *obj_work)
{
	// アクション設定
	ObjDrawObjectActionSet3DNNBlend(obj_work, IDB_ENE_MEREON_MTN_ENE_MEREON_ATK_ZNM);

	// フラグ
	obj_work->move_flag &= ~OBD_MOVE_NOCOL;	// 地形あたり有効
	obj_work->move_flag |= OBD_MOVE_FALL;	// 落下有効
	obj_work->flag &= ~OBD_OBJECT_NOHIT;	// あたりあり
	obj_work->disp_flag &= OBD_DISP_HFLIP;	// 強制右向き(絵的には左向き)

	obj_work->user_timer = 0;
	if (!GmEneComTargetIsLeft(obj_work, &g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work)) {
		// プレイヤーが右にいる
		// 回転演出あり
		obj_work->user_timer = GMD_ENE_MEREON_ROCKET_FALL_DIR;
	}

	obj_work->user_work = 0;

	// メイン処理
	obj_work->ppFunc = gmEneMereonRocketFallMain;
}

// ==========================================================================
// gmEneMereonRocketFallMain
/*!
 *	エネミー メレオン 攻撃(ロケット)落下 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_timer : 回転演出\n
 *		user_work  : 待機タイマ
 */
// ==========================================================================
void gmEneMereonRocketFallMain(OBS_OBJECT_WORK *obj_work)
{
	// 回転演出
	if (obj_work->user_timer) {
		obj_work->dir.y = (u16)(obj_work->dir.y + obj_work->user_timer);
		if (obj_work->dir.y >= 0x8000) {
			obj_work->dir.y = 0x8000;
			obj_work->user_timer = 0;
		}
	}

	// 接地でロケットに変身
	if (obj_work->user_work) {
		obj_work->user_work = (u32)ObjTimeCountDown((fx32)obj_work->user_work);

		if (!obj_work->user_work) {
			GMS_EFFECT_3DES_WORK	*efct_work;

			// 変身
			GMS_ENEMY_3D_WORK	*ene_work;
			ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

			// モーション一旦開放
			ObjAction3dNNMotionRelease(obj_work->obj_3d);
			// 通常モデル解放
			ObjObjectAction3dNNModelReleaseCopy(obj_work);
			// ロケットモデルに切り替え
			ObjObjectCopyAction3dNNModel(obj_work,
							&gm_ene_mereon_r_obj_3d_list[IDB_ENE_MEREON_R_MDL_ENE_MEREON_R_ZNO],
							&ene_work->obj_3d);
#if _PC || _PS3 || _XBOX
			obj_work->obj_3d->toon_camouflage = 1.f;
#endif
			// モーション初期化
			ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, FALSE/*marge*/,
											ObjDataGet(GMD_DWORK_NO_ENEMY_MEREON_MTN), NULL/*mtn_data_path*/,
											0/*index*/, NULL/*archive*/);

			// トゥーン設定
			ObjDrawObjectSetToon(obj_work);

	// 描画処理設定
#if _PC || _PS3 || _XBOX
			obj_work->ppOut = ObjDrawActionSummary;		// 描画処理を戻す
//			obj_work->obj_3d->command_state = OBD_DRAW_CMD_STATE_PRE_WATER;
#endif
			// 地形あたり再設定
			ObjObjectFieldRectSet(obj_work, -4, -4, 4, 4);

			// アクション設定
			ObjDrawObjectActionSet(obj_work, IDB_ENE_MEREON_MTN_ENE_MEREON_ROCKET_ZNM);
			obj_work->disp_flag |= OBD_DISP_REPEAT;

			if (obj_work->dir.y || obj_work->user_timer) {
				// 右向き
				obj_work->dir.y = 0;
				obj_work->disp_flag &= ~OBD_DISP_HFLIP;
			}
			else {
				// 左向き
				obj_work->disp_flag |= OBD_DISP_HFLIP;
			}

			// 速度設定
			obj_work->move_flag |= OBD_MOVE_DIR;
			if (obj_work->disp_flag & OBD_DISP_HFLIP) {
				obj_work->spd_m = -GMD_ENE_MEREON_ROCKET_SPD;
			}
			else {
				obj_work->spd_m = GMD_ENE_MEREON_ROCKET_SPD;
			}

			obj_work->move_flag |= OBD_MOVE_NOCOL_W;	// 角度移動あり 左右の地形チェックを行わない

			// メイン処理設定
			obj_work->ppFunc = gmEneMereonRocketMain;
			obj_work->user_flag = 0;
			obj_work->user_timer = 0;

			// エフェクト バーニア
			efct_work = GmEfctEneEsCreate(obj_work, GME_EFCT_ENE_IDX_E02_JET_S_SMORK);
			GmComEfctSetDispOffsetF(efct_work, -38.f, -11.f, 0.f);
			efct_work->efct_com.obj_work.flag |= OBD_OBJECT_PARENT_FIX_NODISP;
		}
	}
	else if ((obj_work->disp_flag & OBD_DISP_END) && (obj_work->move_flag & OBD_MOVE_UNDER)) {
		GMS_EFFECT_3DES_WORK	*efct_work;

		// 変身待機タイマー設定
		obj_work->user_work = 1 * FX32_ONE;

		// 変身エフェクト
		efct_work = GmEfctEneEsCreate(obj_work, GME_EFCT_ENE_IDX_E04_MEREON_MISS);
		GmComEfctAddDispOffsetF(efct_work, 0.f, -16.f, 16.f);
	}

}

// ==========================================================================
// gmEneMereonRocketMain
/*!
 *	エネミー メレオン 攻撃(ロケット) メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		user_flag : 角度補正状態\n
 *		user_timer: 状態復帰待機タイマー
 */
// ==========================================================================
void gmEneMereonRocketMain(OBS_OBJECT_WORK *obj_work)
{
	// 角度補正チェック
	// 45度を越えた時点で壁抜けに
	if (!obj_work->user_flag &&
			((u16)(obj_work->dir.z + 0x2000) > 0x4000)) {
		
		// 角度補正開始
		obj_work->user_flag = TRUE;
		obj_work->move_flag &= ~OBD_MOVE_DIR;
		obj_work->move_flag |= OBD_MOVE_NOCOL | OBD_MOVE_UNDER;
		obj_work->spd.x = obj_work->spd_m;
	}

	// 角度補正
	if (obj_work->user_timer) {
		obj_work->user_timer--;
		if (!obj_work->user_timer) {
			// 角度移動再開
			//obj_work->move_flag &= ~OBD_MOVE_NOCOL;	// 地形あたりは復活させない
			obj_work->move_flag |= OBD_MOVE_DIR;
			obj_work->user_flag = FALSE;
			obj_work->spd.x = 0;
		}
	}
	else if (obj_work->user_flag) {
		obj_work->dir.z = ObjRoopMove16(obj_work->dir.z, 0, 0x0400);
		if (obj_work->dir.z == 0) {
			obj_work->user_timer = 4;
		}
	}
}

// ==========================================================================
// gmEneMereonCheckFwFlip
/*!
 *	エネミー メレオン Fwフリップチェック
 *
 *	@param obj_work	[in] オブジェクトワーク
 *
 *	@note
 *		プレイヤーの位置でフリップします。\n
 *		フットワークアクションのみです
 */
// ==========================================================================
void gmEneMereonCheckFwFlip(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK		*ply_work;

	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if (ply_work->obj_work.pos.x < obj_work->pos.x) {
		if (obj_work->obj_3d->act_id[0] != IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_ZNM) {
			// アクション設定
			ObjDrawObjectActionSet(obj_work,IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_ZNM);
			obj_work->disp_flag &= ~OBD_DISP_HFLIP;
		}
	}
	else if (ply_work->obj_work.pos.x > obj_work->pos.x) {
		if (obj_work->obj_3d->act_id[0] != IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_L_ZNM) {
			// アクション設定
			ObjDrawObjectActionSet(obj_work,IDB_ENE_MEREON_MTN_ENE_MEREON_B_FW_L_ZNM);
			obj_work->disp_flag |= OBD_DISP_HFLIP;
		}
	}

	obj_work->disp_flag |= OBD_DISP_REPEAT;
}


#if _PC || _PS3 || _XBOX
// ==========================================================================
// gmEneMereonCamouflageDrawFunc
/*!
 *	メレオン 描画
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmEneMereonCamouflageDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->obj_3d->drawflag & NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1) {
		// 迷彩描画あり
		ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, NNE_PROJECTION_TYPE_ORTHO, obj_work->obj_3d->command_state);
		ObjDraw3DNNUserFunc(gmEneMereonDrawPreUserFunc,
						NULL, 0, obj_work->obj_3d->command_state);
	}

	ObjDrawActionSummary(obj_work);
}

// ==========================================================================
// gmEneMereonDrawPreUserFunc
/*!
 *	メレオン描画前 迷彩用処理
 */
// ==========================================================================
void gmEneMereonDrawPreUserFunc(void *data)
{
	NNS_MATRIX44		*proj_mtx;
	AMS_RENDER_TARGET	*render_target;

	UNREFERENCED_PARAMETER(data);

	// レンダターゲットを取得
	render_target = GmDecoGetFallRenderTarget();

	// レンダターゲットが作成されていない
	if (!render_target || render_target->width == 0 ) {
		return;
	}

	proj_mtx = amDrawGetProjectionMatrix();

#if _WII
	if (proj_mtx != NULL) {
		memcpy(_am_draw_fall_projmtx, proj_mtx, sizeof(NNS_MATRIX44));
	}
#endif	//_WII

	// テクスチャ設定
#if _PC | _XBOX
	{
		Uint32	state[NNE_SAMPLERSTATETYPE_MAX];
		NNS_MATRIX44	mtx;
		nnMakeUnitMatrix(&mtx);
		nnInitMaterialControlUserSamplerDXG20();
		nnGetMaterialControlUserSamplerDefaultStateDXG20(state);
		state[NNE_SAMPLERSTATETYPE_ADDRESSU]	= D3DTADDRESS_CLAMP;
		state[NNE_SAMPLERSTATETYPE_ADDRESSV]	= D3DTADDRESS_CLAMP;
		state[NNE_SAMPLERSTATETYPE_ADDRESSW]	= D3DTADDRESS_CLAMP;
		state[NNE_SAMPLERSTATETYPE_MAGFILTER]	= D3DTEXF_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MINFILTER]	= D3DTEXF_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MIPFILTER]	= D3DTEXF_NONE;
		nnSetMaterialControlUserSamplerDXG20(NNE_USER_SAMPLER_2D_1,
				render_target->texture_color[0], &mtx, state);

		nnSetUserUniformDXG20(0,
				NNM_MTX(*proj_mtx, 0, 0), NNM_MTX(*proj_mtx, 1, 0),
				NNM_MTX(*proj_mtx, 2, 0), NNM_MTX(*proj_mtx, 3, 0));
		nnSetUserUniformDXG20(1,
				NNM_MTX(*proj_mtx, 0, 1), NNM_MTX(*proj_mtx, 1, 1),
				NNM_MTX(*proj_mtx, 2, 1), NNM_MTX(*proj_mtx, 3, 1));
		nnSetUserUniformDXG20(2,
				NNM_MTX(*proj_mtx, 0, 2), NNM_MTX(*proj_mtx, 1, 2),
				NNM_MTX(*proj_mtx, 2, 2), NNM_MTX(*proj_mtx, 3, 2));
		nnSetUserUniformDXG20(3,
				NNM_MTX(*proj_mtx, 0, 3), NNM_MTX(*proj_mtx, 1, 3),
				NNM_MTX(*proj_mtx, 2, 3), NNM_MTX(*proj_mtx, 3, 3));
	}
#elif _PS3
	{

		Uint32	state[NNE_SAMPLERSTATETYPE_MAX];
		NNS_MATRIX44	mtx;

		nnMakeUnitMatrix(&mtx);
		nnInitMaterialControlUserSamplerPS3();
		nnGetMaterialControlUserSamplerDefaultStatePS3(state);
		state[NNE_SAMPLERSTATETYPE_ADDRESSU]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_ADDRESSV]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_ADDRESSW]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_MAGFILTER]	= CELL_GCM_TEXTURE_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MINFILTER]	= CELL_GCM_TEXTURE_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MIPFILTER]	= CELL_GCM_TEXTURE_NEAREST;
		nnSetMaterialControlUserSamplerPS3(NNE_USER_SAMPLER_2D_1,
				render_target->texture_color[0], &mtx, state);

		nnSetUserUniformPS3(0,
				NNM_MTX(*proj_mtx, 0, 0), NNM_MTX(*proj_mtx, 1, 0),
				NNM_MTX(*proj_mtx, 2, 0), NNM_MTX(*proj_mtx, 3, 0));
		nnSetUserUniformPS3(1,
				NNM_MTX(*proj_mtx, 0, 1), NNM_MTX(*proj_mtx, 1, 1),
				NNM_MTX(*proj_mtx, 2, 1), NNM_MTX(*proj_mtx, 3, 1));
		nnSetUserUniformPS3(2,
				NNM_MTX(*proj_mtx, 0, 2), NNM_MTX(*proj_mtx, 1, 2),
				NNM_MTX(*proj_mtx, 2, 2), NNM_MTX(*proj_mtx, 3, 2));
		nnSetUserUniformPS3(3,
				NNM_MTX(*proj_mtx, 0, 3), NNM_MTX(*proj_mtx, 1, 3),
				NNM_MTX(*proj_mtx, 2, 3), NNM_MTX(*proj_mtx, 3, 3));
	}
#elif _WII
	{
		memcpy(&_am_draw_render_work, render_target, sizeof(AMS_RENDER_TARGET));
	}
#endif
}
#endif // #if _PC || _PS3 || _XBOX


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
