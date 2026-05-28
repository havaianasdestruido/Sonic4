// ==========================================================================
/*!
  @file gmEneUniuni.cpp
  @brief エネミー ウニウニ

  @author Yurita
				Copyright(c) 2009 Dimps

  $Id: gmEneUniuni.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmEneUniuni.h"

#include "akMath.h"

// データヘッダ
#include "common/model/ene_uniuni_mtn.hmb"
#include "common/model/ene_uniuni_mdl.hmb"


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
#define		GMD_ENE_UNIUNI_EVE_FLAG_RIGHT					(0x0001)		//!< 右向き開始

//===========================================================================
//	反転関係(機能せず)
//===========================================================================
#define		GMD_ENE_UNIUNI_MOVE_SPD_X						(0x0600)		//!< 移動速度
#define		GMD_ENE_UNIUNI_FW_TIME							(15*FX32_ONE)	//!< FW時間
#define		GMD_ENE_UNIUNI_TURN_FRAME						(40)			//!< ターンモーションフレーム

//===========================================================================
//	アクション関係(機能せず)
//===========================================================================
#define		GMD_ENE_UNIUNI_SEARCH_LENGTH					(96)


//===========================================================================
//	トゲ関係(攻撃関係は機能せず)
//===========================================================================
#define		GMD_ENE_UNIUNI_NEEDLE_MAX						(4)
#define		GMD_ENE_UNIUNI_NEEDLE_SPD						(1)
#define		GMD_ENE_UNIUNI_NEEDLE_ROTATE_SPD				(1)				//!< トゲが本体の周りを回転するスピード
#define		GMD_ENE_UNIUNI_NEEDLE_DOWN_RATE					(0.98)			//!< トゲが下に来たことを感知する半径の長さに掛ける係数
																			//   Y座標がこれ以上だと一番下に来たとみなす

#define		GMD_ENE_UNIUNI_NEEDLE_LENGTH_S					(17.5f)
#define		GMD_ENE_UNIUNI_NEEDLE_LENGTH_L					(35.5f)
//===========================================================================
/// ウニウニワーク
//===========================================================================
typedef struct tag_GMS_ENE_UNIUNI_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離

	Angle32					rot_x;				//!< 回転角度
	Angle32					rot_y;				//!< 回転角度
	Angle32					rot_z;				//!< 回転角度
	Float					len;				//!< 現在のトゲの幅
	Float					len_target;			//!< この幅に近づける
	Float					len_spd;			//!< 塚付けるスピード

	Sint32					num;				//!< とげナンバー(親は子の数)
	BOOL					attack;				//!< 攻撃命令
	Sint32					timer;

} GMS_ENE_UNIUNI_WORK;
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
//static void gmEneUniuniWaitInit(OBS_OBJECT_WORK *obj_work);
//static void gmEneUniuniWaitMain(OBS_OBJECT_WORK *obj_work);

//static void gmEneUniuniAttackInit(OBS_OBJECT_WORK *obj_work);
//static void gmEneUniuniAttackMain(OBS_OBJECT_WORK *obj_work);

static void gmEneUniuniWalkInit(OBS_OBJECT_WORK *obj_work);
static void gmEneUniuniWalkMain(OBS_OBJECT_WORK *obj_work);
//static void gmEneUniuniFwInit(OBS_OBJECT_WORK *obj_work);
//static void gmEneUniuniFwMain(OBS_OBJECT_WORK *obj_work);
static void gmEneUniuniFlipInit(OBS_OBJECT_WORK *obj_work);
static void gmEneUniuniFlipMain(OBS_OBJECT_WORK *obj_work);

static BOOL gmEneUniuniSetWalkSpeed(GMS_ENE_UNIUNI_WORK *uniuni_work);

static void gmEneUniuniNeedleWaitInit(OBS_OBJECT_WORK *obj_work);
static void gmEneUniuniNeedleWaitMain(OBS_OBJECT_WORK *obj_work);

static void gmEneUniuniNeedleAttackInit(OBS_OBJECT_WORK *obj_work);
static void gmEneUniuniNeedleAttackMain(OBS_OBJECT_WORK *obj_work);


//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_uniuni_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneUniuniBuild
/*!
 *	エネミー ウニウニ データ構築
 */
// ==========================================================================
void GmEneUniuniBuild(void)
{
	gm_ene_uniuni_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_UNIUNI_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_UNIUNI_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneUniuniFlush
/*!
 *	エネミー ウニウニ データ片付け
 */
// ==========================================================================
void GmEneUniuniFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_UNIUNI_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_uniuni_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneUniuniInit
/*!
 *	エネミー ウニウニ 初期化関数
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
OBS_OBJECT_WORK* GmEneUniuniInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	int i;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_UNIUNI_WORK	*uniuni_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_UNIUNI_WORK), "ENE_UNIUNI");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	uniuni_work = (GMS_ENE_UNIUNI_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_uniuni_obj_3d_list[IDB_ENE_UNIUNI_MDL_ENE_UNIUNI_ZNO],
					&ene_work->obj_3d);
	//ObjObjectAction3dNNModelLoad(obj_work, &ene_work->obj_3d,
	//			NULL/*data_work*/, NULL/*model_data_path*/,
	//			IDB_ENE_UNIUNI_MDL_ENE_UNIUNI_ZNO/*index*/,
	//			ObjDataGet(GMD_DWORK_NO_ENEMY_UNIUNI_MODEL)->pData/*archive*/,
	//			NULL/*tex_data_path*/, ObjDataGet(GMD_DWORK_NO_ENEMY_UNIUNI_TEX)->pData,
	//			NND_DRAWOBJ_SHADER_USER_PROFILE_TOON);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_UNIUNI_MTN), NULL/*mtn_data_path*/,
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
	if (!(eve_rec->flag & GMD_ENE_UNIUNI_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
		obj_work->dir.y = (u16)AKM_DEGtoA16(45);
	}else{
		obj_work->dir.y = (u16)AKM_DEGtoA16(-45);
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
	uniuni_work->spd_dec		 = GMD_ENE_UNIUNI_MOVE_SPD_X / (GMD_ENE_UNIUNI_TURN_FRAME/2);
	uniuni_work->spd_dec_dist = GMD_ENE_UNIUNI_MOVE_SPD_X * (GMD_ENE_UNIUNI_TURN_FRAME/2) / 2;

	// とげに対する命令
	uniuni_work->len		= GMD_ENE_UNIUNI_NEEDLE_LENGTH_S;	//17.5f;//15.0f;
	uniuni_work->len_target	= GMD_ENE_UNIUNI_NEEDLE_LENGTH_L;	//17.5f;//15.0f;
	uniuni_work->len_spd	= 1.0f;
	uniuni_work->rot_x	= AKM_DEGtoA32( 90.0f );
	uniuni_work->rot_y	= AKM_DEGtoA32( 0.0f );
	uniuni_work->rot_z	= AKM_DEGtoA32( 0.0f );
	uniuni_work->num	= 0;

	// ウニデスから変更
	gmEneUniuniWalkInit(obj_work);

	// ウニウニの弾作成
	for(i=0;i<GMD_ENE_UNIUNI_NEEDLE_MAX;i++){
		OBS_OBJECT_WORK* obj_uni;
		obj_uni	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_UNIUNI_NEEDLE,
												pos_x, pos_y,
												0,//flag
												0,0,0,0,
												0);
		// 親を本体にしておく(発射したらNULLにする)
		obj_uni->parent_obj = obj_work;

		GMS_ENE_UNIUNI_WORK	*ndl_work = (GMS_ENE_UNIUNI_WORK*)obj_uni;

		// ニードルの番号(この番号で位置が決まる)
		ndl_work->num = i;
		uniuni_work->num++;
	}

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}

// ==========================================================================
// GmEneUniuniNeedleInit
/*!
 *	エネミー ウニウニとげ 初期化関数
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
OBS_OBJECT_WORK* GmEneUniuniNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_UNIUNI_WORK	*uniuni_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_UNIUNI_WORK), "ENE_UNIUNI");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	uniuni_work = (GMS_ENE_UNIUNI_WORK*)obj_work;

	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_uniuni_obj_3d_list[IDB_ENE_UNIUNI_MDL_ENE_UNIUNI_N_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_UNIUNI_MTN), NULL/*mtn_data_path*/,
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
	ObjRectWorkSet(rect_work, -4, -4, 4, +4);
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

	gmEneUniuniNeedleWaitInit(obj_work);	

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneUniuniGetLength2N()
/*!
 *	プレイヤーとの距離の自乗を求める
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
Sint32	gmEneUniuniGetLength2N( OBS_OBJECT_WORK* obj_work )
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
#if 0
// ==========================================================================
//	gmEneUniuniWaitInit
/*!
 *	エネミー ウニウニ 待ち処理Init
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	// TODO マテリアルモーションになる
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_MOVE_ZNM, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_MOVE_ZNM);

	//ObjDrawObjectActionSet(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneUniuniWaitMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	obj_work->spd.x = 0;
	obj_work->spd.y = 0;

}
// ==========================================================================
//	gmEneUniuniWaitMain
/*!
 *	エネミー ウニウニ 待ちメイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniWaitMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIUNI_WORK	*uniuni_work;
	uniuni_work	= (GMS_ENE_UNIUNI_WORK*)obj_work;

	// プレイヤーが近くにいる
	if (gmEneUniuniGetLength2N( obj_work ) < (GMD_ENE_UNIUNI_SEARCH_LENGTH * GMD_ENE_UNIUNI_SEARCH_LENGTH) ){
		// とげ発射
		obj_work->ppFunc = gmEneUniuniAttackInit;
	}

	// とげ回転
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		uniuni_work->rot_y += AKM_DEGtoA32( 1 );	
	}else{
		uniuni_work->rot_y += AKM_DEGtoA32( -1 );
	}
	
}
#endif


// ==========================================================================
// gmEneUniuniWalkInit
/*!
 *	エネミー ウニウニ Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIUNI_WORK	*uniuni_work;
	uniuni_work	= (GMS_ENE_UNIUNI_WORK*)obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_MOVE_ZNM, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_MOVE_L_ZNM);
//	ObjDrawObjectActionSet(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneUniuniWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	uniuni_work->timer = 1;
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	エネミー ウニウニ Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniWalkMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIUNI_WORK	*uniuni_work;
	uniuni_work	= (GMS_ENE_UNIUNI_WORK*)obj_work;

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_UNIUNI_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_UNIUNI_MOVE_SPD_X;
	}

	// 幅がせまいときのみ回転する
	if (uniuni_work->len_target == GMD_ENE_UNIUNI_NEEDLE_LENGTH_S){

		// とげ回転
		if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
			uniuni_work->rot_y += AKM_DEGtoA32( GMD_ENE_UNIUNI_NEEDLE_ROTATE_SPD );	
		}else{
			uniuni_work->rot_y += AKM_DEGtoA32( -GMD_ENE_UNIUNI_NEEDLE_ROTATE_SPD );
		}

	}else{

		// とげ回転
		if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
			uniuni_work->rot_y += AKM_DEGtoA32( (float)GMD_ENE_UNIUNI_NEEDLE_ROTATE_SPD/2 );	
		}else{
			uniuni_work->rot_y += AKM_DEGtoA32( -(float)GMD_ENE_UNIUNI_NEEDLE_ROTATE_SPD/2 );
		}

		obj_work->spd.x = 0;
	}
	// 幅
	if (uniuni_work->len_target > uniuni_work->len ){
		uniuni_work->len += uniuni_work->len_spd;
		if (uniuni_work->len_target <= uniuni_work->len){
			uniuni_work->len = uniuni_work->len_target;
		}
		uniuni_work->len_spd += 0.03f;
	}
	if (uniuni_work->len_target < uniuni_work->len ){
		uniuni_work->len -= uniuni_work->len_spd;
		if (uniuni_work->len_target >= uniuni_work->len){
			uniuni_work->len = uniuni_work->len_target;
		}
		uniuni_work->len_spd -= 0.05f;
		if (uniuni_work->len_spd<0.1f){
			uniuni_work->len_spd=0.1f;
		}
	}

	if (uniuni_work->timer>0){
		uniuni_work->timer--;
		return;
	}else{
		if (uniuni_work->len_target == GMD_ENE_UNIUNI_NEEDLE_LENGTH_S){
			uniuni_work->timer = 60*2;
			uniuni_work->len_spd = 0.0f;
			uniuni_work->len_target = GMD_ENE_UNIUNI_NEEDLE_LENGTH_L;
		}else{
			if (uniuni_work->len_target == GMD_ENE_UNIUNI_NEEDLE_LENGTH_L){
				uniuni_work->timer = 60*2;
				uniuni_work->len_spd = 1.0f;
				uniuni_work->len_target = GMD_ENE_UNIUNI_NEEDLE_LENGTH_S;
			}
		}
	}

}


#if 0
// ==========================================================================
// gmEneUniuniFwInit
/*!
 *	エネミー ウニウニ FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_FW_ZNM, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_FW_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_FW_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneUniuniFwMain;

	// 移動速度設定
	obj_work->spd.x = 0;

	// 演出時間
	obj_work->user_timer = GMD_ENE_UNIUNI_FW_TIME;
}
#endif

// ==========================================================================
// gmEneUniuniFwMain
/*!
 *	エネミー ウニウニ FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniFwMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	if (obj_work->user_timer <= 0) {
		// フリップへ移行
		gmEneUniuniFlipInit(obj_work);
	}
}



// ==========================================================================
// gmEneUniuniFlipInit
/*!
 *	エネミー ウニウニ フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_FRIP_ZNM, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_FRIP_L_ZNM);
//	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_FRIP_ZNM, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_FRIP_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneUniuniFlipMain;

	// 移動速度設定
	//obj_work->spd.x = 0;
}

// ==========================================================================
// gmEneUniuniFlipMain
/*!
 *	エネミー ウニウニ フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniFlipMain(OBS_OBJECT_WORK *obj_work)
{
	// 移動速度設定
	gmEneUniuniSetWalkSpeed((GMS_ENE_UNIUNI_WORK*)obj_work);

	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		// Walkへ
		gmEneUniuniWalkInit(obj_work);
	}
}




#if 0
// ==========================================================================
// gmEneUniuniAttackInit
/*!
 *	エネミー ウニウニ Attack初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniAttackInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_MOVE_ZNM, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_MOVE_L_ZNM);
//	ObjDrawObjectActionSet(obj_work, IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneUniuniAttackMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;
/*
	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_UNIUNI_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_UNIUNI_MOVE_SPD_X;
	}
*/
}

// ==========================================================================
// gmEneUniuniAttackMain
/*!
 *	エネミー ウニウニ Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniAttackMain(OBS_OBJECT_WORK *obj_work)
{
/*
#if 1
	BOOL				b_dec;
	GMS_ENE_UNIUNI_WORK	*uniuni_work;
	uniuni_work	= (GMS_ENE_UNIUNI_WORK*)obj_work;

	b_dec = gmEneUniuniSetWalkSpeed(uniuni_work);

	if (b_dec) {
		// 減速を開始した

		// フリップへ移行
		gmEneUniuniFlipInit(obj_work);
	}
#else
	if (obj_work->move_flag & OBD_MOVE_FRONT ||
			!GmEneComCheckMoveLimit(obj_work, (fx32)obj_work->user_work, (fx32)obj_work->user_flag)) {
		// 前方衝突・移動範囲外

		// フリップへ移行
		gmEneUniuniFlipInit(obj_work);
		// FWへ移行
		//gmEneUniuniFwInit(obj_work);
	}
#endif
*/
	GMS_ENE_UNIUNI_WORK	*uniuni_work;
	uniuni_work	= (GMS_ENE_UNIUNI_WORK*)obj_work;

	// とげ回転
	if ( obj_work->disp_flag & OBD_DISP_HFLIP ){
		uniuni_work->rot_y += AKM_DEGtoA32( 1 );	
	}else{
		uniuni_work->rot_y += AKM_DEGtoA32( -1 );
	}

	// 一番したのとげにアタック命令
	uniuni_work->attack = TRUE;

	// 全弾を吐き出したら移動開始
	if (uniuni_work->num==0){
		obj_work->ppFunc = gmEneUniuniWalkInit;	
	}
}
#endif

// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneUniuniSetWalkSpeed
/*!
 *	エネミー ウニウニ 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneUniuniSetWalkSpeed(GMS_ENE_UNIUNI_WORK *uniuni_work)
{
	BOOL			b_dec = FALSE;
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)uniuni_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
/*
		// 左向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_FRIP_L_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_UNIUNI_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, uniuni_work->spd_dec, GMD_ENE_UNIUNI_MOVE_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + uniuni_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, uniuni_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -uniuni_work->spd_dec) {
					obj_work->spd.x = -uniuni_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_UNIUNI_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -uniuni_work->spd_dec, GMD_ENE_UNIUNI_MOVE_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_UNIUNI_MTN_ENE_UNIUNI_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_UNIUNI_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -uniuni_work->spd_dec, GMD_ENE_UNIUNI_MOVE_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - uniuni_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, uniuni_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > uniuni_work->spd_dec) {
					obj_work->spd.x = uniuni_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_UNIUNI_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, uniuni_work->spd_dec, GMD_ENE_UNIUNI_MOVE_SPD_X);
		}
*/
	}

	return (b_dec);
}



// ==========================================================================
//	gmEneUniuniNeedleWaitInit
/*!
 *	エネミー ウニウニ 待ち処理Init
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniNeedleWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneUniuniNeedleWaitMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	obj_work->spd.x = 0;
	obj_work->spd.y = 0;

	//gmEneUniuniNeedleWaitMain(obj_work);
}

// ==========================================================================
//	gmEneUniuniWaitMain
/*!
 *	エネミー ウニウニ 待ちメイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniNeedleWaitMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIUNI_WORK	*uniuni_work	= (GMS_ENE_UNIUNI_WORK*)obj_work;
	GMS_ENE_UNIUNI_WORK	*parent_work	= (GMS_ENE_UNIUNI_WORK*)obj_work->parent_obj;

	NNS_MATRIX rmat;
	NNS_MATRIX tmat;
	NNS_MATRIX mat;

	// 回る

	Angle32	r = parent_work->rot_y;
	Angle32	_cap_rot_x = parent_work->rot_x;
	Angle32	_cap_rot_z = parent_work->rot_z;
	Float	_cap_len   = parent_work->len;
	
	// エッグポットを包むように配置
	r += ((AKM_DEGtoA32( 360 )/ GMD_ENE_UNIUNI_NEEDLE_MAX) * uniuni_work->num);
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
		if (v.y >= _cap_len* GMD_ENE_UNIUNI_NEEDLE_DOWN_RATE){
			obj_work->ppFunc = gmEneUniuniNeedleAttackInit;
		}
	}

//	if (uniuni_work->num==0){
//		amPrintf( 20,10, "VY:%2.2f", v.y );
//	}
}

// ==========================================================================
//	gmEneUniuniNeedleAttackInit
/*!
 *	エネミー ウニウニとげ 攻撃処理Init
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniNeedleAttackInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_UNIUNI_WORK	*parent_work	= (GMS_ENE_UNIUNI_WORK*)obj_work->parent_obj;

	parent_work->num--;

	if ( parent_work->ene_3d_work.ene_com.obj_work.disp_flag & OBD_DISP_HFLIP ){
		// 左に飛ばす
		obj_work->spd.x = -FX_F32_TO_FX32( GMD_ENE_UNIUNI_NEEDLE_SPD );

	}else{
		// 右に飛ばす
		obj_work->spd.x = FX_F32_TO_FX32( GMD_ENE_UNIUNI_NEEDLE_SPD );
	}

	// 親を切り離す
	obj_work->parent_obj = NULL;

	obj_work->ppFunc = gmEneUniuniNeedleAttackMain;

}

// ==========================================================================
//	gmEneUniuniNeedleAttackMain
/*!
 *	エネミー ウニウニとげ 攻撃処理Main
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneUniuniNeedleAttackMain(OBS_OBJECT_WORK *obj_work)
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
