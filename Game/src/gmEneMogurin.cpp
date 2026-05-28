// ==========================================================================
/*!
  @file gmEneMogu.cpp
  @brief エネミー モグリン

  @author Yurita
				Copyright(c) 2009 Dimps

  $Id: gmEneMogurin.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmEneMogurin.h"

#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectEnemy.h"
#include "gmComEfct.h"

#include "gmSound.h"
#include "akMath.h"			// ランダムを使用

// データヘッダ
#include "common/model/ene_mogu_mtn.hmb"
#include "common/model/ene_mogu_mdl.hmb"


#if	defined(_WII) && !defined(_PS3)
#pragma warning off (10178)			// 型宣言のプロトタイプのワーニングをOFFにする
									// ただしstatic宣言はつけれないため、問題がある場合は直接修正する
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
#endif	//_WII

//----- Definitions ---------------------------------------------------------

//===========================================================================
//	ツール設定でのイベント関係
//===========================================================================
//			GMS_EVE_RECORD_EVENT.flag
#define		GMD_ENE_MOGU_EVE_FLAG_RIGHT					(0x0001)			//!< 右向き開始
#define		GMD_ENE_MOGU_EVE_FLAG_NOSEARCH				(0x0002)			//!< 右向き開始


//===========================================================================
//	反転関係
//===========================================================================
#define		GMD_ENE_MOGU_MOVE_SPD_X						(0x0800)			//!< 移動速度
#define		GMD_ENE_MOGU_FW_TIME						(15*FX32_ONE)		//!< FW時間
#define		GMD_ENE_MOGU_TURN_FRAME						(40)				//!< ターンモーションフレーム

//===========================================================================
//	距離関係
//===========================================================================
#define		GMD_ENE_MOGU_GRID							(64)						//!< 1グリッドの大きさ
#define		GMD_ENE_MOGU_LENGTH_FIRST_JUMP				(2.5f  * GMD_ENE_MOGU_GRID)	//!< 最初のジャンプ
#define		GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER			(1.25f * GMD_ENE_MOGU_GRID)	//!< プレイヤーを探査する距離


//===========================================================================
//	アクション関係
//===========================================================================
#define		GMD_ENE_MOGU_EFCT_JUMP_OFS_Y				(-30.0f)					//!< ジャンプエフェクトのYオフセット

#define		GMD_ENE_MOGU_JUMP_SPD_Y						(-6.0f)						//!< ジャンプスピード(Y)
#define		GMD_ENE_MOGU_JUMP_SPD_ADD_Y					(0.16f)						//!< ジャンプ重力加速度
#define		GMD_ENE_MOGU_JUMP_ADD_Y						(4.0f)						//!< 初期の持ち上げ

//===========================================================================
/// モグリンワーク
//===========================================================================
typedef struct tag_GMS_ENE_MOGU_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離
	Sint32					wait_time;
	BOOL					jumpdown;			//!< 下に落ちたとき

	Uint32					flag;				//!< 現状水の検査に使用

} GMS_ENE_MOGU_WORK;

//===========================================================================
//	タスク内フラグ関係
//===========================================================================
// flag
#define		GMD_ENE_MOGU_WATER			(1<<0)					//!< 水中でON
#define		GMD_ENE_MOGU_EFCT_ON		(1<<1)					//!< wait～初回ジャンプ上昇中はOFF

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void		gmEneMoguWaitInit(OBS_OBJECT_WORK *obj_work);	// 初期段階で埋まっている状態
static void		gmEneMoguWaitMain(OBS_OBJECT_WORK *obj_work);	// 初期段階で埋まっている状態

static void		gmEneMoguJumpInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneMoguJumpMain(OBS_OBJECT_WORK *obj_work);
static void		gmEneMoguJumpEnd(OBS_OBJECT_WORK *obj_work);

static void		gmEneMoguWalkInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneMoguWalkMain(OBS_OBJECT_WORK *obj_work);
//static void	gmEneMoguFwInit(OBS_OBJECT_WORK *obj_work);
//static void	gmEneMoguFwMain(OBS_OBJECT_WORK *obj_work);
static void		gmEneMoguFlipInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneMoguFlipMain(OBS_OBJECT_WORK *obj_work);

static BOOL		gmEneMoguSetWalkSpeed(GMS_ENE_MOGU_WORK *mogu_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_mogu_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneMoguBuild
/*!
 *	エネミー モグリン データ構築
 */
// ==========================================================================
void GmEneMoguBuild(void)
{
	gm_ene_mogu_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MOGU_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MOGU_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneMoguFlush
/*!
 *	エネミー モグリン データ片付け
 */
// ==========================================================================
void GmEneMoguFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_MOGU_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_mogu_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneMoguInit
/*!
 *	エネミー モグリン 初期化関数
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
OBS_OBJECT_WORK* GmEneMoguInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_MOGU_WORK	*mogu_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_MOGU_WORK), "ENE_MOGU");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	mogu_work = (GMS_ENE_MOGU_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_mogu_obj_3d_list[IDB_ENE_MOGU_MDL_ENE_MOGU_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_MOGU_MTN), NULL/*mtn_data_path*/,
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
	//ObjObjectFieldRectSet(obj_work, -4, -8, 4, -2);
	// 初期はもぐる
	ObjObjectFieldRectSet(obj_work, -4, -8-36, 4, -2-36);

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag |= OBD_MOVE_FALL;

	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_MOGU_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
	mogu_work->spd_dec		 = GMD_ENE_MOGU_MOVE_SPD_X / (GMD_ENE_MOGU_TURN_FRAME/2);
	mogu_work->spd_dec_dist = GMD_ENE_MOGU_MOVE_SPD_X * (GMD_ENE_MOGU_TURN_FRAME/2) / 2;

	mogu_work->flag = 0;
	
//	gmEneMoguWalkInit(obj_work);
	gmEneMoguWaitInit(obj_work);

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}



//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneMoguCheckWater
/*!
 *	エネミー モグリン 水チェック
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneMoguCheckWater( GMS_ENE_MOGU_WORK* mogu_work, s16 ofst )
{
	OBS_OBJECT_WORK* obj_work = (OBS_OBJECT_WORK*)mogu_work;
//	GMS_EFFECT_3DES_WORK	*efct_work;

	if (GmMainIsWaterLevel()) {
		// 中心位置＋オフセットで水位判定
		if (((obj_work->pos.y >> FX32_SHIFT) - ofst) >= g_gm_main_system.water_level) {
			// 水中
			if (!(mogu_work->flag & GMD_ENE_MOGU_WATER)) {
				if (mogu_work->flag & GMD_ENE_MOGU_EFCT_ON) {
					// 水中に入った瞬間
					// エフェクト 水しぶき
//					efct_work = 
					GmEfctCmnEsCreate( obj_work, GME_EFCT_CMN_IDX_SPRAY );

					// エフェクト 泡
					//GmPlyEfctCreateBubble(ply_work);
					// SE
					GmSoundPlaySE("Spray");
					// BGMチェンジ？
				}
			}
			mogu_work->flag |= GMD_ENE_MOGU_WATER;

			/*
			ランダムで泡発生
			efct_work = GmEfctCmnEsCreate(&ply_work->obj_work, GME_EFCT_CMN_IDX_BUBBLE);
			// 親クリア
			efct_work->efct_com.obj_work.parent_obj = NULL;
			*/
			return TRUE;
		}else{
			if (mogu_work->flag & GMD_ENE_MOGU_WATER) {
				if (mogu_work->flag & GMD_ENE_MOGU_EFCT_ON) {

					// さっきまで水中だった
					// エフェクト 水しぶき
//					efct_work = 
					GmEfctCmnEsCreate( obj_work, GME_EFCT_CMN_IDX_SPRAY);
					// SE
					GmSoundPlaySE("Spray");
					// BGMチェンジ？
				}
				mogu_work->flag &= ~GMD_ENE_MOGU_WATER;
				return TRUE;
			}else{
				return FALSE;
			}
		} 
	}
	return FALSE;
}
// ==========================================================================
// gmEneMoguWaitInit
/*!
 *	エネミー モグリン Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_MOVE_ZNM, IDB_ENE_MOGU_MTN_ENE_MOGU_MOVE_L_ZNM);
	ObjDrawObjectActionSet(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_JUMP_01_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	obj_work->pos.z  = GMD_OBJ_DEFAULT_POS_Z_B_BACK;	// 20091104 Dimps Ishizaki
	// メイン処理(プレイヤーがくるで待ち処理)
	obj_work->ppFunc = gmEneMoguWaitMain;
/*
	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_MOGU_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_MOGU_MOVE_SPD_X;
	}
*/
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	エネミー モグリン Wait メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguWaitMain(OBS_OBJECT_WORK *obj_work)
{
//	BOOL				b_dec;
	GMS_ENE_MOGU_WORK	*mogu_work;

	mogu_work	= (GMS_ENE_MOGU_WORK*)obj_work;

	// プレイヤーが近づいたら出てくる
	GMS_PLAYER_WORK		*ply_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

#if 1	// 20091020 Dimps Ishizaki 画面回転による生成範囲拡張対応
	float	px = FX_FX32_TO_F32(ply_work->obj_work.pos.x - obj_work->pos.x);
	float	py = FX_FX32_TO_F32(ply_work->obj_work.pos.y - obj_work->pos.y);

	float	len = px*px + py*py;

	GMS_EFFECT_3DES_WORK*	effect_work;

	if (len <= GMD_ENE_MOGU_LENGTH_FIRST_JUMP * GMD_ENE_MOGU_LENGTH_FIRST_JUMP){
#else
	fx32	px = ply_work->obj_work.pos.x - obj_work->pos.x;
	fx32	py = ply_work->obj_work.pos.y - obj_work->pos.y;

	fx32	len = FX_Mul( px, px ) + FX_Mul( py, py );

	GMS_EFFECT_3DES_WORK*	effect_work;

	if (len <= FX_F32_TO_FX32( GMD_ENE_MOGU_LENGTH_FIRST_JUMP * GMD_ENE_MOGU_LENGTH_FIRST_JUMP )){
#endif

		obj_work->ppFunc = gmEneMoguJumpInit;

		// 水チェック
		if (!gmEneMoguCheckWater( mogu_work, 48 )) {						// ofst加味して頭くらいの位置で水位判定
			// 水ではないから
			// 土煙を出す
			effect_work = GmEfctEneEsCreate( obj_work, GME_EFCT_ENE_IDX_E07_MOGU_E );
#if _IPHONE
			if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
				effect_work->efct_com.obj_work.pos.z = GMD_OBJ_DEFAULT_POS_Z_A_BACK;
			}
#endif // _IPHONE
		}else{
			effect_work = GmEfctEneEsCreate( obj_work, GME_EFCT_ENE_IDX_E07_MOGU_W );
		}
		// 上に上げる
		GmComEfctSetDispOffsetF(effect_work, 0.f, GMD_ENE_MOGU_EFCT_JUMP_OFS_Y, 0.f);

	}
}

// ==========================================================================
// gmEneMoguJumpInit
/*!
 *	エネミー モグリン Jump 初期化処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguJumpInit(OBS_OBJECT_WORK *obj_work)
{
//	BOOL				b_dec;

	GMS_ENE_MOGU_WORK*	mogu_work;
	mogu_work	= (GMS_ENE_MOGU_WORK*)obj_work;

	ObjDrawObjectActionSet(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_JUMP_01_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// 近づいた
	// ジャンプして出てくる
	obj_work->spd.y = FX_F32_TO_FX32( GMD_ENE_MOGU_JUMP_SPD_Y );
	obj_work->spd_fall = FX_F32_TO_FX32( GMD_ENE_MOGU_JUMP_SPD_ADD_Y );

	obj_work->pos.y -= FX_F32_TO_FX32( GMD_ENE_MOGU_JUMP_ADD_Y );

	// 下に落ちるように変更
	obj_work->move_flag |=	OBD_MOVE_FALL;
	obj_work->move_flag &= ~OBD_MOVE_UNDER;
	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	obj_work->ppFunc = gmEneMoguJumpMain;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_MOGU_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_MOGU_MOVE_SPD_X;
	}
	// 水チェック
	gmEneMoguCheckWater( mogu_work, 0 );

	
	mogu_work->jumpdown = FALSE;
}

// ==========================================================================
// gmEneMoguJumpMain
/*!
 *	エネミー モグリン Jump メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguJumpMain(OBS_OBJECT_WORK *obj_work)
{

	GMS_ENE_MOGU_WORK*	mogu_work = (GMS_ENE_MOGU_WORK*)obj_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_MOGU_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_MOGU_MOVE_SPD_X;
	}

	if (obj_work->spd.y > 0){

		if (!mogu_work->jumpdown){

			mogu_work->jumpdown = TRUE;

			// アクションを変化させる
			ObjDrawObjectActionSet(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_JUMP_02_ZNM);
			obj_work->disp_flag |= OBD_DISP_REPEAT;

			// 落ちてきたときに当たりをつける
			obj_work->move_flag &= ~OBD_MOVE_NOCOL;

			ObjObjectFieldRectSet(obj_work, -4, -8, 4, -2);
			// 落ちてきたときに優先を変更する
			obj_work->pos.z = GMD_OBJ_ENEMY_POS_Z;

			// 以後は着水/離水エフェクト生成します
			mogu_work->flag |= GMD_ENE_MOGU_EFCT_ON;
		}

		// 壁にぶつかってたら反転する(とりあえずの対処)
		if (obj_work->move_flag & OBD_MOVE_FRONT){
			if (obj_work->disp_flag & OBD_DISP_HFLIP){
				obj_work->disp_flag &= ~OBD_DISP_HFLIP;
			}else{
				obj_work->disp_flag |= OBD_DISP_HFLIP;
			}
		}

	}
	// 地面についたらアクション変化
	if (obj_work->move_flag & OBD_MOVE_UNDER){
		GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_DOWN_ZNM, IDB_ENE_MOGU_MTN_ENE_MOGU_DOWN_L_ZNM);
	
//		ObjDrawObjectActionSet(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_JUMP_02_ZNM);
		obj_work->ppFunc = gmEneMoguJumpEnd;
//		obj_work->ppFunc = gmEneMoguWalkInit;
	}
	// 水チェック
	gmEneMoguCheckWater( mogu_work, 0 );

}

// ==========================================================================
// gmEneMoguJumpEnd
/*!
 *	エネミー モグリン Jump着地
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguJumpEnd(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_MOGU_WORK*	mogu_work = (GMS_ENE_MOGU_WORK*)obj_work;
	// 水チェック
	gmEneMoguCheckWater( mogu_work, 0 );

	// アクションが終了していたら
	if (obj_work->disp_flag & OBD_DISP_END){
		obj_work->ppFunc = gmEneMoguWalkInit;
	}
}



// ==========================================================================
// gmEneMoguWalkInit
/*!
 *	エネミー モグリン Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_MOVE_ZNM, IDB_ENE_MOGU_MTN_ENE_MOGU_MOVE_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneMoguWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_MOGU_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_MOGU_MOVE_SPD_X;
	}
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	エネミー モグリン Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguWalkMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK		*ply_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	GMS_ENE_MOGU_WORK	*mogu_work;
	mogu_work	= (GMS_ENE_MOGU_WORK*)obj_work;

	// 水チェック
	gmEneMoguCheckWater( mogu_work, 0 );


	// プレイヤーとの距離を確認して遠い場合は処理をどちらかに切り替える
#if 1	// 20091020 Dimps Ishizaki 画面回転による生成範囲拡張対応
	float	px = FX_FX32_TO_F32( ply_work->obj_work.pos.x - obj_work->pos.x );
	float	py = FX_FX32_TO_F32( ply_work->obj_work.pos.y - obj_work->pos.y );

	float	len = px*px + py*py;

	GMS_ENEMY_COM_WORK*	ene_com = (GMS_ENEMY_COM_WORK*)obj_work;

	// プレイヤーを見ないかどうか
	if (ene_com->eve_rec->flag & GMD_ENE_MOGU_EVE_FLAG_NOSEARCH) {
//		len = FX_F32_TO_FX32( GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER * GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER )+1;
		len = GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER * GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER + 1;	// 型変換warningの修正
		mogu_work->wait_time = 60*60*60;
	}

	if (len > GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER * GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER ){
#else
	fx32	px = ply_work->obj_work.pos.x - obj_work->pos.x;
	fx32	py = ply_work->obj_work.pos.y - obj_work->pos.y;

	fx32	len = FX_Mul( px, px ) + FX_Mul( py, py );

	GMS_ENEMY_COM_WORK*	ene_com = (GMS_ENEMY_COM_WORK*)obj_work;

	// プレイヤーを見ないかどうか
	if (ene_com->eve_rec->flag & GMD_ENE_MOGU_EVE_FLAG_NOSEARCH) {
		len = FX_F32_TO_FX32( GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER * GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER )+1;
		mogu_work->wait_time = 60*60*60;
	}

	if (len > FX_F32_TO_FX32( GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER * GMD_ENE_MOGU_LENGTH_SEARCH_PLAYER )){
#endif

		if (mogu_work->wait_time>0){
			mogu_work->wait_time--;
			//　小さな壁にぶち当たっている?
			if (obj_work->move_flag & OBD_MOVE_FRONT){
				gmEneMoguJumpInit(obj_work);
			}
			return;
		}

		if (AkMathRandFx() > FX_F32_TO_FX32(0.5f)){
			// 50%の確率で振り向く
			gmEneMoguFlipInit(obj_work);
		}

		// 自動では向き直らないように(大きな値を入れておく)
		mogu_work->wait_time = 60*60*60; // TODO PAL
		return;
		
	}

	mogu_work->wait_time = 0;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {

		//自分が左向いていてプレイヤーが右にいる
		if (!GmEneComTargetIsLeft( obj_work, &ply_work->obj_work )){
			// フリップへ移行
			gmEneMoguFlipInit(obj_work);	
		}

	}else{
		//自分が右向いていてプレイヤーが左にいる
		if (GmEneComTargetIsLeft( obj_work, &ply_work->obj_work )){
			// フリップへ移行
			gmEneMoguFlipInit(obj_work);	
		}
	}

	//　小さな壁にぶち当たっている?
	if (obj_work->move_flag & OBD_MOVE_FRONT){
		gmEneMoguJumpInit(obj_work);
	}
}


#if 0
// ==========================================================================
// gmEneMoguFwInit
/*!
 *	エネミー モグリン FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_FW_ZNM, IDB_ENE_MOGU_MTN_ENE_MOGU_FW_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_FW_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneMoguFwMain;

	// 移動速度設定
	obj_work->spd.x = 0;

	// 演出時間
	obj_work->user_timer = GMD_ENE_MOGU_FW_TIME;
}
#endif

// ==========================================================================
// gmEneMoguFwMain
/*!
 *	エネミー モグリン FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguFwMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	if (obj_work->user_timer <= 0) {
		// フリップへ移行
		gmEneMoguFlipInit(obj_work);
	}
}



// ==========================================================================
// gmEneMoguFlipInit
/*!
 *	エネミー モグリン フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_FRIP_ZNM, IDB_ENE_MOGU_MTN_ENE_MOGU_FRIP_L_ZNM);
	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_FRIP_ZNM, IDB_ENE_MOGU_MTN_ENE_MOGU_FRIP_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_MOGU_MTN_ENE_MOGU_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneMoguFlipMain;

	// 移動速度設定
	//obj_work->spd.x = 0;
}

// ==========================================================================
// gmEneMoguFlipMain
/*!
 *	エネミー モグリン フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneMoguFlipMain(OBS_OBJECT_WORK *obj_work)
{
	// 移動速度設定
	gmEneMoguSetWalkSpeed((GMS_ENE_MOGU_WORK*)obj_work);

	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		// Walkへ
		gmEneMoguWalkInit(obj_work);
	}
}


// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneMoguSetWalkSpeed
/*!
 *	エネミー モグリン 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneMoguSetWalkSpeed(GMS_ENE_MOGU_WORK *mogu_work)
{
	BOOL			b_dec = FALSE;
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)mogu_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// 左向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_MOGU_MTN_ENE_MOGU_FRIP_L_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_MOGU_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, mogu_work->spd_dec, GMD_ENE_MOGU_MOVE_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + mogu_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, mogu_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -mogu_work->spd_dec) {
					obj_work->spd.x = -mogu_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_MOGU_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -mogu_work->spd_dec, GMD_ENE_MOGU_MOVE_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_MOGU_MTN_ENE_MOGU_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_MOGU_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -mogu_work->spd_dec, GMD_ENE_MOGU_MOVE_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - mogu_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, mogu_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > mogu_work->spd_dec) {
					obj_work->spd.x = mogu_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_MOGU_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, mogu_work->spd_dec, GMD_ENE_MOGU_MOVE_SPD_X);
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
