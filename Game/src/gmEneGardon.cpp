// ==========================================================================
/*!
  @file gmEneGardon.cpp
  @brief エネミー ガードン

  @author Yurita
				Copyright(c) 2009 Dimps

  $Id: gmEneGardon.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmEneGardon.h"
#include "gmPlySeq.h"

#include "gmEffect.h"
#include "gmEffectEnemy.h"
#include "gmEffectCmn.h"

#include "gmSound.h"

// データヘッダ
#include "common/model/ene_gardon_mtn.hmb"
#include "common/model/ene_gardon_mdl.hmb"


#if	defined(_WII) && !defined(_PS3)
#pragma warning off (10178)			// 型宣言のプロトタイプのワーニングをOFFにする
									// ただしstatic宣言はつけれないため、問題がある場合は直接修正する
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
#endif	//_WII

//----- Definitions ---------------------------------------------------------

//===========================================================================
//	シールド状態
//===========================================================================
enum GME_ENE_GARDON_SHIELD
{
			GME_ENE_GARDON_SHIELD_NONE = 0,								//!< シールドなし

			GME_ENE_GARDON_SHIELD_UP,									//!< 上シールド中
			GME_ENE_GARDON_SHIELD_FRONT,								//!< 横シールド中

			GME_ENE_GARDON_SHIELD_MAX,
};

//===========================================================================
//	ツール設定でのイベント関係
//===========================================================================
//			GMS_EVE_RECORD_EVENT.flag
#define		GMD_ENE_GARDON_EVE_FLAG_RIGHT			(0x0001)			//!< 右向き開始


//===========================================================================
//	反転関係
//===========================================================================
//#define		GMD_ENE_GARDON_MOVE_SPD_X				(0x0800)			//!< 移動速度
#define		GMD_ENE_GARDON_MOVE_SPD_X				(0x0400)			//!< 移動速度
//#define		GMD_ENE_GARDON_MOVE_SPD_X				(0x0400)			//!< 移動速度
#define		GMD_ENE_GARDON_FW_TIME					(15*FX32_ONE)		//!< FW時間
#define		GMD_ENE_GARDON_TURN_FRAME				(40)				//!< ターンモーションフレーム

//===========================================================================
//	距離関係
//===========================================================================
#define		GMD_ENE_GARDON_SEARCH_LENGTH			(2)	//(64)				//<! プレイヤーを見つける距離

//===========================================================================
//	ガード関係
//===========================================================================
#define		GMD_ENE_GARDON_GUARD_FLICK_SPD			(1.5f)				//<! プレイヤーをはじく係数(spdに掛け合わされる)
#define		GMD_ENE_GARDON_GUARD_UP_EFCT_Y			(30)				//<! 上ガードエフェクト位置
#define		GMD_ENE_GARDON_GUARD_FRONT_EFCT_X		(30)				//<! 横ガードエフェクト位置(現在未使用 TODO)

//===========================================================================
//	音関係
//===========================================================================
#define		GMD_ENE_GARDON_SE_GUARD					"Casino1"			//!< ガード音

//===========================================================================
/// ガードンワーク
//===========================================================================
typedef struct tag_GMS_ENE_GARDON_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離

	Sint32					timer;
	GME_ENE_GARDON_SHIELD	shield;				//!< 盾状態
} GMS_ENE_GARDON_WORK;
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmEneGardonAtkRectOff(OBS_OBJECT_WORK* obj_work);
static void gmEneGardonAtkRectOn(OBS_OBJECT_WORK* obj_work);

static void		gmEneGardonWalkInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneGardonWalkMain(OBS_OBJECT_WORK *obj_work);

static void		gmEneGardonWalkWait(OBS_OBJECT_WORK *obj_work);

// 少しとまる(Flipにつながる)
static void		gmEneGardonWaitToFlipInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneGardonWaitToFlipMain(OBS_OBJECT_WORK *obj_work);

// とまる(プレイヤーが近くにいなければWalkにつながる)
static void		gmEneGardonWaitToWalkInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneGardonWaitToWalkMain(OBS_OBJECT_WORK *obj_work);

static void		gmEneGardonFlipInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneGardonFlipMain(OBS_OBJECT_WORK *obj_work);

static BOOL		gmEneGardonSetWalkSpeed(GMS_ENE_GARDON_WORK *gardon_work);

static	void	gmEneGardonDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_gardon_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneGardonBuild
/*!
 *	エネミー ガードン データ構築
 */
// ==========================================================================
void GmEneGardonBuild(void)
{
	gm_ene_gardon_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_GARDON_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_GARDON_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneGardonFlush
/*!
 *	エネミー ガードン データ片付け
 */
// ==========================================================================
void GmEneGardonFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_GARDON_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_gardon_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneGardonInit
/*!
 *	エネミー ガードン 初期化関数
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
OBS_OBJECT_WORK* GmEneGardonInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_GARDON_WORK	*gardon_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_GARDON_WORK), "ENE_GARDON");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	gardon_work = (GMS_ENE_GARDON_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_gardon_obj_3d_list[IDB_ENE_GARDON_MDL_ENE_GARDON_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_GARDON_MTN), NULL/*mtn_data_path*/,
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
	rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// くらい
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	rect_work->ppDef = gmEneGardonDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -24, -32, 24, 0);
	rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// ボディ
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, -32, 19, 0);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// 地形あたり
	ObjObjectFieldRectSet(obj_work, -4, -8, 4, -2);

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag |= OBD_MOVE_FALL;

	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_GARDON_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
	gardon_work->spd_dec		 = GMD_ENE_GARDON_MOVE_SPD_X / (GMD_ENE_GARDON_TURN_FRAME/2);
	gardon_work->spd_dec_dist = GMD_ENE_GARDON_MOVE_SPD_X * (GMD_ENE_GARDON_TURN_FRAME/2) / 2;

	gmEneGardonWalkInit(obj_work);

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}



//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneGardonGetLength2N()
/*!
 *	プレイヤーとの距離の自乗を求める
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
Sint32	gmEneGardonGetLength2N( OBS_OBJECT_WORK* obj_work )
{
	GMS_PLAYER_WORK		*ply_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	// 20091021 Dimps Ishizaki
	if (ply_work->player_flag & GMD_PLF_DIE) {
		// プレイヤー死亡時は一番遠い場所にする
		return (0x7FFFFFFF);
	}

	// プレイヤーとの距離を確認して遠い場合は処理をどちらかに切り替える
	fx32	px = ply_work->obj_work.pos.x - obj_work->pos.x;
	fx32	py = ply_work->obj_work.pos.y - obj_work->pos.y;
	
//	fx32	len = FX_Mul( px, px ) + FX_Mul( py, py );
//	return (Sint32)FX_FX32_TO_F32( len );

	float	x = FX_FX32_TO_F32( px );
	float	y = FX_FX32_TO_F32( py );
	
	return (Sint32)( x * x  + y * y );


}

// ==========================================================================
// gmEneGardonIsPlayerAttack()
/*!
 *	プレイヤーが攻撃状態かどうか
 *
 */
// ==========================================================================
BOOL	gmEneGardonIsPlayerAttack()
{
	GMS_PLAYER_WORK		*ply_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if (
		ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING ||	// ホーミングアタック
		ply_work->seq_state == GME_PLY_SEQ_STATE_JUMP	||	// ジャンプアタック
		ply_work->seq_state == GME_PLY_SEQ_STATE_SPIN	)	// ジャンプアタック
	{
	
		return TRUE;
	}
	return FALSE;
}

// ==========================================================================
// gmEneGardonIsPlayerAttack()
/*!
 *	プレイヤーが前にいるか
 *
 */
// ==========================================================================
BOOL gmEneGardonIsPlayerFront( OBS_OBJECT_WORK* obj_work )
{
	GMS_PLAYER_WORK		*ply_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	if (obj_work->disp_flag & OBD_DISP_HFLIP){
		// 左を向いている
		if (obj_work->pos.x > ply_work->obj_work.pos.x){
			// 敵が左にいる
			return TRUE;
		}
	}else{
		if (obj_work->pos.x < ply_work->obj_work.pos.x){
			// 敵が右にいる
			return TRUE;
		}
	}
	return FALSE;
}

// ==========================================================================
// gmEneGardonAtkRectOff()
/*!
 *	ガードンの攻撃判定をOFF
 *
 */
// ==========================================================================
void gmEneGardonAtkRectOff(OBS_OBJECT_WORK* obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	OBS_RECT_WORK		*rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];

	// 攻撃矩形OFF
	rect_work->flag &= ~OBD_RECT_ENABLE;
}

// ==========================================================================
// gmEneGardonAtkRectOn()
/*!
 *	ガードンの攻撃判定をON
 *
 */
// ==========================================================================
void gmEneGardonAtkRectOn(OBS_OBJECT_WORK* obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	OBS_RECT_WORK		*rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	
	// 攻撃矩形ON
	rect_work->flag |= OBD_RECT_ENABLE;
}

// =======================================================================
// gmBoss4BodyDamageDefFunc
/*!
  本体 プレイヤー攻撃ヒット時くらい処理関数
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmEneGardonDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{

	OBS_OBJECT_WORK*		my_obj		= my_rect->parent_obj;
	OBS_OBJECT_WORK*		your_obj	= your_rect->parent_obj;
	GMS_ENE_GARDON_WORK*	gardon_work	= (GMS_ENE_GARDON_WORK*)my_obj;
	GMS_PLAYER_WORK*		ply_work	= (GMS_PLAYER_WORK*)your_obj;

//	GMS_PLAYER_WORK*	ply_work	= (GMS_PLAYER_WORK*)your_obj;
	GMS_EFFECT_3DES_WORK*	efct_work;
	
	if (your_obj && GMD_OBJTYPE_PLAYER == your_obj->obj_type) {

		// ホーミングかどうかを検査
		if (ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING || ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING_REF){
			// このときは絶対に上ガードにする
			
			if (gmEneGardonIsPlayerFront( my_obj )){
				/*プレイヤーが前にいる*/

				// 盾を上に構える
				GmEneComActionSetDependHFlip( my_obj, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_01_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_L_01_ZNM);
				my_obj->disp_flag &= ~OBD_DISP_REPEAT;
				//ObjDrawObjectActionSet( my_obj, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_01_ZNM );
				gardon_work->shield = GME_ENE_GARDON_SHIELD_UP;

				GmPlySeqAtkReactionInit(ply_work);
				ply_work->obj_work.spd.y = (fx32)(ply_work->obj_work.spd.y * GMD_ENE_GARDON_GUARD_FLICK_SPD);

				// はじきエフェクト
				efct_work = GmEfctEneEsCreate( my_obj, GME_EFCT_ENE_IDX_E05_GUARD);
				efct_work->efct_com.obj_work.pos.x = my_obj->pos.x;
				efct_work->efct_com.obj_work.pos.y = my_obj->pos.y;
				GmEffect3DESAddDispOffset(efct_work, 0, GMD_ENE_GARDON_GUARD_UP_EFCT_Y, 0);
				GmSoundPlaySE( GMD_ENE_GARDON_SE_GUARD );

				gmEneGardonAtkRectOff(my_obj);				// 攻撃矩形OFF
				return;
			}else{
				// 盾が出せないときは撃破される
				GmEnemyDefaultDefFunc( my_rect, your_rect );
				return;
			}
		}


		if (my_obj->pos.y-FX_F32_TO_FX32(20) > your_obj->pos.y ){

			if (gmEneGardonIsPlayerFront( my_obj ) || ((my_obj->disp_flag & OBD_DISP_HFLIP)!=(your_obj->disp_flag & OBD_DISP_HFLIP)) ){
				/*プレイヤーが前にいる*/

				// 盾を上に構える
				GmEneComActionSetDependHFlip( my_obj, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_01_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_L_01_ZNM);
				my_obj->disp_flag &= ~OBD_DISP_REPEAT;
				//ObjDrawObjectActionSet( my_obj, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_01_ZNM );
				gardon_work->shield = GME_ENE_GARDON_SHIELD_UP;

				GmPlySeqAtkReactionInit(ply_work);
				ply_work->obj_work.spd.y = (fx32)(ply_work->obj_work.spd.y * GMD_ENE_GARDON_GUARD_FLICK_SPD);

				// はじきエフェクト
				efct_work = GmEfctEneEsCreate( my_obj, GME_EFCT_ENE_IDX_E05_GUARD);
				efct_work->efct_com.obj_work.pos.x = my_obj->pos.x;
				efct_work->efct_com.obj_work.pos.y = my_obj->pos.y;// - FX_F32_TO_FX32( 30 );

				GmEffect3DESAddDispOffset(efct_work, 0, GMD_ENE_GARDON_GUARD_UP_EFCT_Y, 0);
				GmSoundPlaySE( GMD_ENE_GARDON_SE_GUARD );
				return;
			}else{
				// 盾が出せないときは撃破される
				GmEnemyDefaultDefFunc( my_rect, your_rect );
				return;
			}

		}else{
			// プレイヤーが前にいる?
			if (gmEneGardonIsPlayerFront( my_obj )){
				// 盾を横に構える
				GmEneComActionSetDependHFlip( my_obj, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_01_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_L_01_ZNM);
				my_obj->disp_flag &= ~OBD_DISP_REPEAT;
				//ObjDrawObjectActionSet( my_obj, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_01_ZNM );
				gardon_work->shield = GME_ENE_GARDON_SHIELD_FRONT;

				// ソニックは地上
				ply_work->obj_work.disp_flag ^= OBD_DISP_HFLIP;			// 反転させる
				GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN);
				if (ply_work->obj_work.spd_m) {
					ply_work->obj_work.spd_m = -ply_work->obj_work.spd_m;	// 反転させる
					// 最低速度を設定する(ガードンの矩形範囲に入ったままになる事がある為)
					if (MTM_MATH_ABS(ply_work->obj_work.spd_m) < 8*FX32_ONE) {
						if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
							ply_work->obj_work.spd_m = -8*FX32_ONE;
						}
						else {
							ply_work->obj_work.spd_m = 8*FX32_ONE;
						}
					}

				} else {
					// ソニック停止状態なら位置関係から速度を与える
					if (my_obj->pos.x > ply_work->obj_work.pos.x) {
						ply_work->obj_work.spd_m = (fx32)(-FX32_ONE * 12);
						ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
					} else {
						ply_work->obj_work.spd_m = (fx32)(FX32_ONE * 12);
						ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
					}
				}
				//GmPlySeqAtkReactionInit(ply_work);

				// はじきエフェクト
				efct_work = GmEfctEneEsCreate( my_obj, GME_EFCT_ENE_IDX_E05_GUARD);
				efct_work->efct_com.obj_work.pos.x = my_obj->pos.x;
				efct_work->efct_com.obj_work.pos.y = my_obj->pos.y;
				GmSoundPlaySE( GMD_ENE_GARDON_SE_GUARD );
				return;
			}
			// 盾が出せないときは撃破される
			GmEnemyDefaultDefFunc( my_rect, your_rect );
		}
	}
}

// ==========================================================================
// gmEneGardonWalkInit
/*!
 *	エネミー ガードン Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGardonWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
//	obj_work->obj_3d->blend_spd = 0.2f;		//5フレーム補完
//	GmEneComActionSet3DNNBlendDependHFlip( obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_MOVE_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_MOVE_L_ZNM );
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_MOVE_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_MOVE_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneGardonWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_GARDON_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_GARDON_MOVE_SPD_X;
	}
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	エネミー ガードン Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGardonWalkMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_GARDON_WORK	*gardon_work;
	gardon_work	= (GMS_ENE_GARDON_WORK*)obj_work;

#if 0
	BOOL				b_dec;

	b_dec = gmEneGardonSetWalkSpeed(gardon_work);

	if (b_dec) {
		// 減速を開始した

		// フリップへ移行
		gmEneGardonFlipInit(obj_work);
	}
#else

//	obj_work->obj_3d->blend_spd = 0.0f;
	if (gardon_work->shield!=GME_ENE_GARDON_SHIELD_NONE){
		obj_work->spd.x = 0;
		// シールド演出が終わっている
		if (obj_work->disp_flag & OBD_DISP_END){
			if (gardon_work->shield == GME_ENE_GARDON_SHIELD_UP){
				//ObjDrawObjectActionSet(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_02_ZNM);
		
	//			GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_02_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_L_02_ZNM );
				GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_02_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_L_02_ZNM);
			}else{
				//ObjDrawObjectActionSet(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_02_ZNM);
	//			GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_02_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_L_02_ZNM);
				GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_02_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_L_02_ZNM);
			}
			gmEneGardonAtkRectOn(obj_work);						// 攻撃矩形ON
			gardon_work->shield = GME_ENE_GARDON_SHIELD_NONE;
			obj_work->ppFunc = gmEneGardonWalkWait;
		}
		return;
	}

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_GARDON_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_GARDON_MOVE_SPD_X;
	}
	if (obj_work->obj_3d->frame[0]>=40 && obj_work->obj_3d->frame[0]<=60){
		obj_work->spd.x = 0;
	}
	if (obj_work->obj_3d->frame[0]>=100 && obj_work->obj_3d->frame[0]<=120){
		obj_work->spd.x = 0;
	}


	// プレイヤーとの距離が近い場合は、とまる処理へ
	if (gmEneGardonGetLength2N( obj_work ) <= ( GMD_ENE_GARDON_SEARCH_LENGTH * GMD_ENE_GARDON_SEARCH_LENGTH ) ){
		// 
		obj_work->ppFunc = gmEneGardonWaitToWalkInit;
		return;
	}

	if (obj_work->move_flag & OBD_MOVE_FRONT ||
			!GmEneComCheckMoveLimit(obj_work, (fx32)obj_work->user_work, (fx32)obj_work->user_flag)) {
		// 前方衝突・移動範囲外

		// フリップへ移行
		gmEneGardonWaitToFlipInit(obj_work);
	}
#endif

}


// ==========================================================================
// gmEneGardonWalkWait
/*!
 *	エネミー ガードン シールド終了待ち
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGardonWalkWait(OBS_OBJECT_WORK *obj_work)
{
	// シールド演出が終わっている
	if (obj_work->disp_flag & OBD_DISP_END){
		obj_work->ppFunc = gmEneGardonWaitToWalkInit;	
	}
}


// ==========================================================================
// gmEneGardonWaitToFlipInit
/*!
 *	エネミー ガードン 待ち処理初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGardonWaitToFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_L_ZNM);
	//GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneGardonWaitToFlipMain;

	// 移動速度設定
	//obj_work->spd.x = 0;

	obj_work->spd.x = 0;

	GMS_ENE_GARDON_WORK*	gardon_work = (GMS_ENE_GARDON_WORK*)obj_work;

	// WAITしなくなった
	gardon_work->timer = 1;//60; // TODO PAL

	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_MOVE_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_MOVE_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FW_ZNM);
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FW_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_FW_L_ZNM);
//	obj_work->disp_flag |= OBD_DISP_REPEAT;
}

// ==========================================================================
// gmEneGardonWaitToFlipMain
/*!
 *	エネミー ガードン 待ち処理 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGardonWaitToFlipMain(OBS_OBJECT_WORK *obj_work)
{

	GMS_ENE_GARDON_WORK*	gardon_work = (GMS_ENE_GARDON_WORK*)obj_work;

	if (gardon_work->timer>0){
		gardon_work->timer--;
		return;
	}

	// 反転へ
	gmEneGardonFlipInit(obj_work);
}

// ==========================================================================
// gmEneGardonWaitToWalkInit
/*!
 *	エネミー ガードン 待ち処理初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGardonWaitToWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_L_ZNM);
	//GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneGardonWaitToWalkMain;

	// 移動速度設定
	//obj_work->spd.x = 0;

	obj_work->spd.x = 0;

	GMS_ENE_GARDON_WORK*	gardon_work = (GMS_ENE_GARDON_WORK*)obj_work;

	gardon_work->timer = 60; // TODO PAL

//	obj_work->obj_3d->blend_spd = 0.2f;		//5フレーム補完
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FW_ZNM);

	// アクションをMOVEにしておく
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_MOVE_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_MOVE_L_ZNM);
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FW_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_FW_L_ZNM);
//	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FW_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_FW_L_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	gmEneGardonAtkRectOn(obj_work);						// 攻撃矩形ON
	gardon_work->shield = GME_ENE_GARDON_SHIELD_NONE;
}

// ==========================================================================
// gmEneGardonWaitToWalkMain
/*!
 *	エネミー ガードン 待ち処理 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGardonWaitToWalkMain(OBS_OBJECT_WORK *obj_work)
{

	GMS_ENE_GARDON_WORK*	gardon_work = (GMS_ENE_GARDON_WORK*)obj_work;

//	obj_work->obj_3d->blend_spd = 0.0f;
	if (gardon_work->shield!=GME_ENE_GARDON_SHIELD_NONE){
		obj_work->spd.x = 0;
		// シールド演出が終わっている
		if (obj_work->disp_flag & OBD_DISP_END){
			if (gardon_work->shield == GME_ENE_GARDON_SHIELD_UP){
				//ObjDrawObjectActionSet(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_02_ZNM);
		
	//			GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_02_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_L_02_ZNM );
				GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_02_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GU_L_02_ZNM);
			}else{
				//ObjDrawObjectActionSet(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_02_ZNM);
	//			GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_02_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_L_02_ZNM);
				GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_02_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_GF_L_02_ZNM);
			}
			gmEneGardonAtkRectOn(obj_work);						// 攻撃矩形ON
			gardon_work->shield = GME_ENE_GARDON_SHIELD_NONE;
			obj_work->ppFunc = gmEneGardonWalkWait;
		}
		return;
	}

	if (gmEneGardonGetLength2N( obj_work ) > ( GMD_ENE_GARDON_SEARCH_LENGTH * GMD_ENE_GARDON_SEARCH_LENGTH ) ){
		// 
		obj_work->ppFunc = gmEneGardonWalkInit;
		return;
	}

}


// ==========================================================================
// gmEneGardonFlipInit
/*!
 *	エネミー ガードン フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGardonFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_L_ZNM);
	//GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneGardonFlipMain;

	// 移動速度設定
	//obj_work->spd.x = FX_F32_TO_FX32( 0.5 );
}

// ==========================================================================
// gmEneGardonFlipMain
/*!
 *	エネミー ガードン フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneGardonFlipMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_GARDON_WORK*	gardon_work = (GMS_ENE_GARDON_WORK*)obj_work;

	// フリップ中にガードしなくてはならなくなったら
	// フリップ終了しておく
	if (gardon_work->shield != GME_ENE_GARDON_SHIELD_NONE)
	{
//		gardon_work->shield = GME_ENE_GARDON_SHIELD_NONE;

		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		obj_work->ppFunc	= gmEneGardonWalkMain;
		return;
	}

	// 移動速度設定
	gmEneGardonSetWalkSpeed((GMS_ENE_GARDON_WORK*)obj_work);

	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		// Walkへ
		gmEneGardonWalkInit(obj_work);
	}
}


// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneGardonSetWalkSpeed
/*!
 *	エネミー ガードン 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneGardonSetWalkSpeed(GMS_ENE_GARDON_WORK *gardon_work)
{
	BOOL			b_dec = FALSE;
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)gardon_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// 左向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_GARDON_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, gardon_work->spd_dec, GMD_ENE_GARDON_MOVE_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + gardon_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, gardon_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -gardon_work->spd_dec) {
					obj_work->spd.x = -gardon_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_GARDON_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -gardon_work->spd_dec, GMD_ENE_GARDON_MOVE_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_GARDON_MTN_ENE_GARDON_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_GARDON_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -gardon_work->spd_dec, GMD_ENE_GARDON_MOVE_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - gardon_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, gardon_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > gardon_work->spd_dec) {
					obj_work->spd.x = gardon_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_GARDON_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, gardon_work->spd_dec, GMD_ENE_GARDON_MOVE_SPD_X);
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
