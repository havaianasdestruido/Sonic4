// ==========================================================================
/*!
  @file gmEneUnides.cpp
  @brief エネミー ウニデス

  @author Yurita
				Copyright(c) 2009 Dimps

  $Id: gmEneUnides.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmEneUnides.h"

#include "akMath.h"

// データヘッダ
#include "common/model/ene_unides_mtn.hmb"
#include "common/model/ene_unides_mdl.hmb"


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
#define		GMD_ENE_UNIDES_EVE_FLAG_RIGHT					(0x0001)		//!< 右向き開始

//===========================================================================
//	反転関係(機能せず)
//===========================================================================
#define		GMD_ENE_UNIDES_MOVE_SPD_X						(0x0600)		//!< 移動速度
#define		GMD_ENE_UNIDES_FW_TIME							(15*FX32_ONE)	//!< FW時間
#define		GMD_ENE_UNIDES_TURN_FRAME						(40)			//!< ターンモーションフレーム

//===========================================================================
//	アクション関係
//===========================================================================
#define		GMD_ENE_UNIDES_SEARCH_LENGTH					(96)			//!< プレイヤーを感知し、弾を放つ距離

//===========================================================================
//	トゲ関係
//===========================================================================
#define		GMD_ENE_UNIDES_NEEDLE_MAX						(4)				//!< とげの数
#define		GMD_ENE_UNIDES_NEEDLE_SPD						(1)				//!< とげの飛ぶスピード
#define		GMD_ENE_UNIDES_NEEDLE_ROTATE_SPD				(1)				//!< トゲが本体の周りを回転するスピード
#define		GMD_ENE_UNIDES_NEEDLE_DOWN_RATE					(0.98)			//!< トゲが下に来たことを感知する半径の長さに掛ける係数
																			//   Y座標がこれ以上だと一番下に来たとみなす
//===========================================================================
/// ウニデスワーク
//===========================================================================
typedef struct tag_GMS_ENE_UNIDES_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離

	Angle32					rot_x;				//!< 回転角度
	Angle32					rot_y;				//!< 回転角度
	Angle32					rot_z;				//!< 回転角度
	Float					len;

	Sint32					num;				//!< とげナンバー(親は子の数)
	BOOL					attack_first;		//!< 攻撃命令
	BOOL					attack;				//!< 攻撃命令
	BOOL					zoom_now;			//!< トゲがとんだ瞬間

	BOOL					stop;				//!< とめる
	Sint32					timer;

	float					zoom;
} GMS_ENE_UNIDES_WORK;
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void		gmEneUnidesWaitInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneUnidesWaitMain(OBS_OBJECT_WORK *obj_work);

static void		gmEneUnidesAttackInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneUnidesAttackMain(OBS_OBJECT_WORK *obj_work);

static void		gmEneUnidesWalkInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneUnidesWalkMain(OBS_OBJECT_WORK *obj_work);
//static void	gmEneUnidesFwInit(OBS_OBJECT_WORK *obj_work);
//static void	gmEneUnidesFwMain(OBS_OBJECT_WORK *obj_work);
static void		gmEneUnidesFlipInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneUnidesFlipMain(OBS_OBJECT_WORK *obj_work);

static BOOL		gmEneUnidesSetWalkSpeed(GMS_ENE_UNIDES_WORK *unides_work);

static void		gmEneUnidesNeedleWaitInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneUnidesNeedleWaitMain(OBS_OBJECT_WORK *obj_work);

static void		gmEneUnidesNeedleAttackInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneUnidesNeedleAttackMain(OBS_OBJECT_WORK *obj_work);


//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_unides_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneUnidesBuild
/*!
 *	エネミー ウニデス データ構築
 */
// ==========================================================================
void GmEneUnidesBuild(void)
{
	gm_ene_unides_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_UNIDES_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_UNIDES_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneUnidesFlush
/*!
 *	エネミー ウニデス データ片付け
 */
// ==========================================================================
void GmEneUnidesFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_UNIDES_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_unides_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneUnidesInit
/*!
 *	エネミー ウニデス 初期化関数
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
OBS_OBJECT_WORK* GmEneUnidesInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	int i;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_UNIDES_WORK	*unides_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_UNIDES_WORK), "ENE_UNIDES");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	unides_work = (GMS_ENE_UNIDES_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_unides_obj_3d_list[IDB_ENE_UNIDES_MDL_ENE_UNIDES_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_UNIDES_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

	// 優先設定
	// A面の前に設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;	//GMD_OBJ_DEFAULT_POS_Z_N_FRONT;	/*GMD_OBJ_ENEMY_POS_Z;*/
//	obj_work->pos.z = GMD_OBJ_ENEMY_POS_Z;

	// 矩形設定
	// 対プレイヤー
	// 攻撃
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -8, -16+16, 8, 0+16);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// くらい
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -16, -24+16, 16, 0+16);
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

	// 重力なし
	obj_work->move_flag &= ~OBD_MOVE_FALL;
	obj_work->move_flag |= OBD_MOVE_NOCOL;	// 地形あたり無し

//	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	//obj_work->move_flag |= OBD_MOVE_FALL;

	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_UNIDES_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
		obj_work->dir.y = (u16)AKM_DEGtoA16(45);
	}else{
		obj_work->dir.y = (u16)AKM_DEGtoA16(-45);
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
	unides_work->spd_dec		= GMD_ENE_UNIDES_MOVE_SPD_X / (GMD_ENE_UNIDES_TURN_FRAME/2);
	unides_work->spd_dec_dist	= GMD_ENE_UNIDES_MOVE_SPD_X * (GMD_ENE_UNIDES_TURN_FRAME/2) / 2;

	// とげに対する命令
	unides_work->len	= 17.5f;
	unides_work->rot_x	= AKM_DEGtoA32( 90.0f );
	unides_work->rot_y	= AKM_DEGtoA32( 0.0f );
	unides_work->rot_z	= AKM_DEGtoA32( 0.0f );
	unides_work->num	= 0;

	gmEneUnidesWaitInit(obj_work);

	// ウニデスの弾作成
	for(i=0;i<GMD_ENE_UNIDES_NEEDLE_MAX;i++){
		OBS_OBJECT_WORK* obj_uni;
		obj_uni	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_UNIDES_NEEDLE,
												pos_x, pos_y,
												0,//flag
												0,0,0,0,
												0);
		// 親を本体にしておく(発射したらNULLにする)
		obj_uni->parent_obj = obj_work;

		GMS_ENE_UNIDES_WORK	*ndl_work = (GMS_ENE_UNIDES_WORK*)obj_uni;

		// ニードルの番号(この番号で位置が決まる)
		ndl_work->num = i;
		unides_work->num++;
	}
	// 攻撃まだ
	unides_work->attack			= FALSE;
	unides_work->attack_first	= FALSE;
	unides_work->zoom_now		= 0;
	unides_work->zoom			= 1.0f;

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}

// ==========================================================================
// GmEneUnidesNeedleInit
/*!
 *	エネミー ウニデスとげ 初期化関数
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
OBS_OBJECT_WORK* GmEneUnidesNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_UNIDES_WORK	*unides_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_UNIDES_WORK), "ENE_UNIDES");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	unides_work = (GMS_ENE_UNIDES_WORK*)obj_work;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_unides_obj_3d_list[IDB_ENE_UNIDES_MDL_ENE_UNIDES_N_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_UNIDES_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

	// 優先設定
	// A面の前に設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;	//GMD_OBJ_DEFAULT_POS_Z_N_FRONT;	/*GMD_OBJ_ENEMY_POS_Z;*/
//	obj_work->pos.z = GMD_OBJ_ENEMY_POS_Z;

	// 矩形設定
	// 対プレイヤー
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	ObjRectWorkSet(rect_work, -6, -12+8, 6, 0+8);
	rect_work->flag |= OBD_RECT_ENABLE;

	// ボディ
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectWorkSet(rect_work, -19, -32+32, 19, 0+32);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// ホーミングさせない
	ene_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

	// 重力なし
	obj_work->move_flag &= ~OBD_MOVE_FALL;
	obj_work->move_flag |= OBD_MOVE_NOCOL;	// 地形あたり無し

	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	gmEneUnidesNeedleWaitInit(obj_work);	

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneUnidesGetLength2N()
/*!
 *	プレイヤーとの距離の自乗を求める
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
Sint32	gmEneUnidesGetLength2N( OBS_OBJECT_WORK* obj_work )
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
//	gmEneUnidesWaitInit
/*!
 *	エネミー ウニデス 待ち処理Init
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	// TODO マテリアルモーションになる
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_MOVE_ZNM, IDB_ENE_UNIDES_MTN_ENE_UNIDES_MOVE_ZNM);

	//ObjDrawObjectActionSet(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneUnidesWaitMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	obj_work->spd.x = 0;
	obj_work->spd.y = 0;

}

// ==========================================================================
//	gmEneUnidesWaitMain
/*!
 *	エネミー ウニデス 待ちメイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesWaitMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIDES_WORK	*unides_work;
	unides_work	= (GMS_ENE_UNIDES_WORK*)obj_work;

	// プレイヤーが近くにいる
	if (gmEneUnidesGetLength2N( obj_work ) < (GMD_ENE_UNIDES_SEARCH_LENGTH * GMD_ENE_UNIDES_SEARCH_LENGTH) ){
		// とげ発射
		obj_work->ppFunc = gmEneUnidesAttackInit;
	}

	// とげ回転
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		unides_work->rot_y += AKM_DEGtoA32( GMD_ENE_UNIDES_NEEDLE_ROTATE_SPD );	
	}else{
		unides_work->rot_y += AKM_DEGtoA32( -GMD_ENE_UNIDES_NEEDLE_ROTATE_SPD );
	}

}


// ==========================================================================
// gmEneUnidesWalkInit
/*!
 *	エネミー ウニデス Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIDES_WORK	*unides_work;
	unides_work	= (GMS_ENE_UNIDES_WORK*)obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_MOVE_ZNM, IDB_ENE_UNIDES_MTN_ENE_UNIDES_MOVE_L_ZNM);
//	ObjDrawObjectActionSet(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneUnidesWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;


	unides_work->timer = 60;	// TODO PAL

}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	エネミー ウニデス Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesWalkMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIDES_WORK	*unides_work;
	unides_work	= (GMS_ENE_UNIDES_WORK*)obj_work;

	if (unides_work->timer>0){
		unides_work->timer--;
		return;
	}

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_UNIDES_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_UNIDES_MOVE_SPD_X;
	}
/*
#if 1
	BOOL				b_dec;
	GMS_ENE_UNIDES_WORK	*unides_work;
	unides_work	= (GMS_ENE_UNIDES_WORK*)obj_work;

	b_dec = gmEneUnidesSetWalkSpeed(unides_work);

	if (b_dec) {
		// 減速を開始した

		// フリップへ移行
		gmEneUnidesFlipInit(obj_work);
	}
#else
	if (obj_work->move_flag & OBD_MOVE_FRONT ||
			!GmEneComCheckMoveLimit(obj_work, (fx32)obj_work->user_work, (fx32)obj_work->user_flag)) {
		// 前方衝突・移動範囲外

		// フリップへ移行
		gmEneUnidesFlipInit(obj_work);
		// FWへ移行
		//gmEneUnidesFwInit(obj_work);
	}
#endif
*/
	// とげ回転
	//unides_work->rot_y += AKM_DEGtoA32( 1 );
}


#if 0
// ==========================================================================
// gmEneUnidesFwInit
/*!
 *	エネミー ウニデス FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_FW_ZNM, IDB_ENE_UNIDES_MTN_ENE_UNIDES_FW_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_FW_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneUnidesFwMain;

	// 移動速度設定
	obj_work->spd.x = 0;

	// 演出時間
	obj_work->user_timer = GMD_ENE_UNIDES_FW_TIME;
}
#endif

// ==========================================================================
// gmEneUnidesFwMain
/*!
 *	エネミー ウニデス FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesFwMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	if (obj_work->user_timer <= 0) {
		// フリップへ移行
		gmEneUnidesFlipInit(obj_work);
	}
}



// ==========================================================================
// gmEneUnidesFlipInit
/*!
 *	エネミー ウニデス フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_FRIP_ZNM, IDB_ENE_UNIDES_MTN_ENE_UNIDES_FRIP_L_ZNM);
//	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_FRIP_ZNM, IDB_ENE_UNIDES_MTN_ENE_UNIDES_FRIP_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneUnidesFlipMain;

	// 移動速度設定
	//obj_work->spd.x = 0;
}

// ==========================================================================
// gmEneUnidesFlipMain
/*!
 *	エネミー ウニデス フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesFlipMain(OBS_OBJECT_WORK *obj_work)
{
	// 移動速度設定
	gmEneUnidesSetWalkSpeed((GMS_ENE_UNIDES_WORK*)obj_work);

	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		// Walkへ
		gmEneUnidesWalkInit(obj_work);
	}
}


// ==========================================================================
// gmEneUnidesAttackInit
/*!
 *	エネミー ウニデス Attack初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesAttackInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_MOVE_ZNM, IDB_ENE_UNIDES_MTN_ENE_UNIDES_MOVE_L_ZNM);
//	ObjDrawObjectActionSet(obj_work, IDB_ENE_UNIDES_MTN_ENE_UNIDES_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneUnidesAttackMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;
}

// ==========================================================================
// gmEneUnidesAttackMain
/*!
 *	エネミー ウニデス Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesAttackMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIDES_WORK	*unides_work;
	unides_work	= (GMS_ENE_UNIDES_WORK*)obj_work;

	// とげ回転
	if ( !unides_work->stop ){
		if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
			unides_work->rot_y += AKM_DEGtoA32( GMD_ENE_UNIDES_NEEDLE_ROTATE_SPD );	
		}else{
			unides_work->rot_y += AKM_DEGtoA32( -GMD_ENE_UNIDES_NEEDLE_ROTATE_SPD );
		}
	}
	// 一番したのとげにアタック命令
	unides_work->attack = TRUE;

	// 攻撃時に巨大化する
	if ( unides_work->zoom_now == 1){
		unides_work->zoom += 0.07f;
		if (unides_work->zoom > 1.4f){
			unides_work->zoom_now = 2;
		}
	} else if ( unides_work->zoom_now >=2 && unides_work->zoom_now <=12){
		unides_work->zoom_now++;
		unides_work->zoom -= 0.07f;

	} else if ( unides_work->zoom_now >=13 && unides_work->zoom_now <=23){
		unides_work->zoom_now++;
		unides_work->zoom += 0.07f;

	}else {
		if (unides_work->zoom>1.0f){
			unides_work->zoom-=0.07f;
			if (unides_work->zoom <= 1.0f){
				unides_work->zoom = 1.0f;
				unides_work->stop = FALSE;
				unides_work->zoom_now = 0;
			}			
		}
	}
	obj_work->scale.x = FX_F32_TO_FX32( unides_work->zoom );
	obj_work->scale.y = FX_F32_TO_FX32( unides_work->zoom );
	obj_work->scale.z = FX_F32_TO_FX32( unides_work->zoom );

	// 全弾を吐き出したら移動開始
	if (unides_work->num==0 && unides_work->zoom==1.0f){
		obj_work->ppFunc = gmEneUnidesWalkInit;	
	}
}


// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneUnidesSetWalkSpeed
/*!
 *	エネミー ウニデス 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneUnidesSetWalkSpeed(GMS_ENE_UNIDES_WORK *unides_work)
{
	BOOL			b_dec = FALSE;
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)unides_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
/*
		// 左向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_UNIDES_MTN_ENE_UNIDES_FRIP_L_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_UNIDES_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, unides_work->spd_dec, GMD_ENE_UNIDES_MOVE_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + unides_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, unides_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -unides_work->spd_dec) {
					obj_work->spd.x = -unides_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_UNIDES_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -unides_work->spd_dec, GMD_ENE_UNIDES_MOVE_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_UNIDES_MTN_ENE_UNIDES_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_UNIDES_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -unides_work->spd_dec, GMD_ENE_UNIDES_MOVE_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - unides_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, unides_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > unides_work->spd_dec) {
					obj_work->spd.x = unides_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_UNIDES_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, unides_work->spd_dec, GMD_ENE_UNIDES_MOVE_SPD_X);
		}
*/
	}

	return (b_dec);
}



// ==========================================================================
//	gmEneUnidesNeedleWaitInit
/*!
 *	エネミー ウニデス 待ち処理Init
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesNeedleWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneUnidesNeedleWaitMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	obj_work->spd.x = 0;
	obj_work->spd.y = 0;
}

// ==========================================================================
//	gmEneUnidesWaitMain
/*!
 *	エネミー ウニデス 待ちメイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesNeedleWaitMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIDES_WORK	*unides_work	= (GMS_ENE_UNIDES_WORK*)obj_work;
	GMS_ENE_UNIDES_WORK	*parent_work	= (GMS_ENE_UNIDES_WORK*)obj_work->parent_obj;

	NNS_MATRIX rmat;
	NNS_MATRIX tmat;
	NNS_MATRIX mat;

	// 回る

	Angle32	r = parent_work->rot_y;
	Angle32	_cap_rot_x = parent_work->rot_x;
	Angle32	_cap_rot_z = parent_work->rot_z;
	Float	_cap_len   = parent_work->len;
	
	// エッグポットを包むように配置
	r += ((AKM_DEGtoA32( 360 )/ GMD_ENE_UNIDES_NEEDLE_MAX) * unides_work->num);
	r %= AKM_DEGtoA32( 360 );

	// 少しこちらに傾ける
	nnMakeRotateXMatrix( &rmat, _cap_rot_x );

	nnRotateZMatrix( &rmat, &rmat, _cap_rot_z );
	// 回転
	nnRotateYMatrix( &rmat, &rmat, r );

	nnMakeTranslateMatrix( &tmat, _cap_len, 0.0f, 0.0f );

	nnMultiplyMatrix( &mat, &rmat, &tmat );

	NNS_VECTOR	v;
	nnCopyMatrixTranslationVector( &v, &mat );

	obj_work->pos.x = FX_F32_TO_FX32( v.x ) + parent_work->ene_3d_work.ene_com.obj_work.pos.x;
	obj_work->pos.y = FX_F32_TO_FX32( v.y ) + parent_work->ene_3d_work.ene_com.obj_work.pos.y;
//	obj_work->pos.z = FX_F32_TO_FX32( v.z );
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;	//GMD_OBJ_DEFAULT_POS_Z_N_FRONT;	/*GMD_OBJ_ENEMY_POS_Z;*/

	// 弾になるチェック
	if (parent_work->attack){
		// 一番下にきた?
		if (v.y >= _cap_len * GMD_ENE_UNIDES_NEEDLE_DOWN_RATE){

			if (!parent_work->stop){
				if (parent_work->attack_first){
					obj_work->ppFunc = gmEneUnidesNeedleAttackInit;
				}else{
					// 最初
					parent_work->zoom_now		= 1;
					parent_work->attack_first	= TRUE;
					parent_work->stop			= TRUE;
				}
			}
		}
	}

//	if (unides_work->num==0){
//		amPrintf( 20,10, "VY:%2.2f", v.y );
//	}
}

// ==========================================================================
//	gmEneUnidesNeedleAttackInit
/*!
 *	エネミー ウニデスとげ 攻撃処理Init
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesNeedleAttackInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIDES_WORK	*parent_work	= (GMS_ENE_UNIDES_WORK*)obj_work->parent_obj;

	parent_work->num--;

	if ( parent_work->ene_3d_work.ene_com.obj_work.disp_flag & OBD_DISP_HFLIP ){
		// 左に飛ばす
		obj_work->spd.x = -FX_F32_TO_FX32( GMD_ENE_UNIDES_NEEDLE_SPD );

	}else{
		// 右に飛ばす
		obj_work->spd.x = FX_F32_TO_FX32( GMD_ENE_UNIDES_NEEDLE_SPD );
	}

	// 親を切り離す
	obj_work->parent_obj = NULL;

	obj_work->ppFunc = gmEneUnidesNeedleAttackMain;

}

// ==========================================================================
//	gmEneUnidesNeedleAttackMain
/*!
 *	エネミー ウニデスとげ 攻撃処理Main
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUnidesNeedleAttackMain(OBS_OBJECT_WORK *obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);
	// なにもしない(回転させる?)
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
