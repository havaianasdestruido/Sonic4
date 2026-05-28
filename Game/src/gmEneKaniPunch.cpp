// ==========================================================================
/*!
  @file gmEneKani.cpp
  @brief エネミー かにパンチ

  @author Yurita
				Copyright(c) 2009 Dimps

  $Id: gmEneKaniPunch.cpp 2 2011-04-11 05:21:26Z thamada $
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

//----- Pragma  -------------------------------------------------------
#if	defined(_WII) && !defined(_PS3)
#pragma warning off (10178)			// 型宣言のプロトタイプのワーニングをOFFにする
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
									// ただしstatic宣言はつけれないため、問題がある場合は直接修正する
#endif	//_WII

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
#include "gmEneKaniPunch.h"

#include "akMath.h"
#include "gmSound.h"

// データヘッダ
#include "common/model/ene_kani_mtn.hmb"
#include "common/model/ene_kani_mdl.hmb"


//----- Definitions ---------------------------------------------------------

//===========================================================================
//	ツール設定でのイベント関係
//===========================================================================
//			GMS_EVE_RECORD_EVENT.flag
#define		GMD_ENE_KANI_EVE_FLAG_SMALL					(0x0001)			//!< あたふたが少なくなる

//===========================================================================
//	反転関係
//===========================================================================
#define		GMD_ENE_KANI_MOVE_SPD_X						(0x0800)			//!< 移動速度
#define		GMD_ENE_KANI_FW_TIME						(15*FX32_ONE)		//!< FW時間
#define		GMD_ENE_KANI_TURN_FRAME						(40)				//!< ターンモーションフレーム

//===========================================================================
//	アクション関係
//===========================================================================
#define		GMD_ENE_KANI_SEARCH_LENGTH					(92)				//!< プレイヤーを感知する距離

//===========================================================================
//	ノード関係
//===========================================================================
#define		GMD_ENE_KANI_NODE_RIGHT_HAND				(16)				//!< シザーのノード

//===========================================================================
//	サウンド関係
//===========================================================================
#define		GMD_ENE_KANI_SE_PUNCH						"Kani"

//===========================================================================
/// かにパンチワーク
//===========================================================================
typedef struct tag_GMS_ENE_KANI_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離

	GMS_ENE_NODE_MATRIX		node_work;
	Sint32					timer;
	fx32					spd_x;

	BOOL					ata_futa;			//!< あたふたする

	BOOL					walk_s;				//!< あたふたの動きが少なくなる

} GMS_ENE_KANI_WORK;
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void		gmEneKaniWalkInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneKaniWalkMain(OBS_OBJECT_WORK *obj_work);
//static void	gmEneKaniFwInit(OBS_OBJECT_WORK *obj_work);
//static void	gmEneKaniFwMain(OBS_OBJECT_WORK *obj_work);
static void		gmEneKaniFlipInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneKaniFlipMain(OBS_OBJECT_WORK *obj_work);

static void		gmEneKaniAttackInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneKaniAttackMain(OBS_OBJECT_WORK *obj_work);
static void		gmEneKaniAttackEnd(OBS_OBJECT_WORK *obj_work);


static BOOL		gmEneKaniSetWalkSpeed(GMS_ENE_KANI_WORK *kani_work);

// 後に使えるなら共通へ移動させる
void		GmEneUtilInitNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work, OBS_OBJECT_WORK* obj_work, Sint32 max_node );
void		GmEneUtilInitNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work, OBS_OBJECT_WORK* obj_work, Sint32 max_node );
NNS_MATRIX* GmEneUtilGetNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work, Sint32 node_id );
void		GmEneUtilExitNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work );

void gmEneExit(MTS_TASK_TCB *tcb);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_kani_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneKaniBuild
/*!
 *	エネミー かにパンチ データ構築
 */
// ==========================================================================
void GmEneKaniBuild(void)
{
	gm_ene_kani_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_KANI_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_KANI_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneKaniFlush
/*!
 *	エネミー かにパンチ データ片付け
 */
// ==========================================================================
void GmEneKaniFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_KANI_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_kani_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneKaniInit
/*!
 *	エネミー かにパンチ 初期化関数
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
OBS_OBJECT_WORK* GmEneKaniInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_KANI_WORK	*kani_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_KANI_WORK), "ENE_KANI");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	kani_work = (GMS_ENE_KANI_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_kani_obj_3d_list[IDB_ENE_KANI_MDL_ENE_KANI_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_KANI_MTN), NULL/*mtn_data_path*/,
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
	ObjObjectFieldRectSet(obj_work, -4, -8, 4, -2);

	// かにパンチは常に前を向くようにしておく
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag |= OBD_MOVE_FALL;

	// あたふたが少なくなる
	kani_work->walk_s = FALSE;
	if ((eve_rec->flag & GMD_ENE_KANI_EVE_FLAG_SMALL)) {
		kani_work->walk_s = TRUE;
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
	kani_work->spd_dec		 = GMD_ENE_KANI_MOVE_SPD_X / (GMD_ENE_KANI_TURN_FRAME/2);
	kani_work->spd_dec_dist = GMD_ENE_KANI_MOVE_SPD_X * (GMD_ENE_KANI_TURN_FRAME/2) / 2;

	gmEneKaniWalkInit(obj_work);

	// ノードシステムを使用する
	GmEneUtilInitNodeMatrix( &kani_work->node_work, obj_work, 3 );
	// ノードシステムを使用するため開放処理を変える
	mtTaskChangeTcbDestructor(obj_work->tcb, gmEneExit);

	// 初期登録
	GmEneUtilGetNodeMatrix( &kani_work->node_work, GMD_ENE_KANI_NODE_RIGHT_HAND );

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;

#if OBD_OBJECT_USE_NOEXIST
	obj_work->flag |= OBD_OBJECT_NOEXIST_ENABLE;
#endif // OBD_OBJECT_USE_NOEXIST
#endif // _IPHONE

	kani_work->ata_futa = FALSE;
	return (obj_work);
}



//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneKaniGetLength2N()
/*!
 *	プレイヤーとの距離の自乗を求める
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
Sint32	gmEneKaniGetLength2N( OBS_OBJECT_WORK* obj_work )
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
// gmEneKaniIsPlayerLeft()
/*!
 *	プレイヤーとの距離の自乗を求める
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
Sint32	gmEneKaniIsPlayerLeft( OBS_OBJECT_WORK* obj_work )
{
	GMS_PLAYER_WORK		*ply_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	return GmEneComTargetIsLeft( obj_work, &ply_work->obj_work );
}

// ==========================================================================
// gmEneKaniWalkInit
/*!
 *	エネミー かにパンチ Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKaniWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KANI_MTN_ENE_KANI_MOVE_ZNM, IDB_ENE_KANI_MTN_ENE_KANI_MOVE_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_KANI_MTN_ENE_KANI_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneKaniWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_KANI_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_KANI_MOVE_SPD_X;
	}


	OBS_RECT_WORK		*rect_work;
	// 攻撃は体につける
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	// 仮矩形
	ObjRectWorkSet(rect_work, -11, -24, 11, 0);
	rect_work->flag |= OBD_RECT_ENABLE;

	// 1秒歩く
	GMS_ENE_KANI_WORK	*kani_work = (GMS_ENE_KANI_WORK*)obj_work;
	
	if (kani_work->walk_s){
		kani_work->timer = 15;
	}else{
		kani_work->timer = 10 + mtMathRand() % 20;
	}
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	エネミー かにパンチ Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKaniWalkMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_KANI_WORK	*kani_work = (GMS_ENE_KANI_WORK*)obj_work;

	if (kani_work->ata_futa){
		// あたふた移動中
		if (kani_work->timer>0){
			kani_work->timer--;
			return;
		}else{
			// モーションのスピードを２倍にする
			obj_work->obj_3d->speed[0] = 2.0f;
			// あたふた方向切り替え
			obj_work->disp_flag ^= OBD_DISP_HFLIP;
			// Walkへ
			// 移動速度設定
			if (obj_work->disp_flag & OBD_DISP_HFLIP) {
				obj_work->spd.x = -GMD_ENE_KANI_MOVE_SPD_X;
			}
			else {
				obj_work->spd.x = GMD_ENE_KANI_MOVE_SPD_X;
			}
			obj_work->ppFunc = gmEneKaniWalkMain;

			if (kani_work->walk_s){
				kani_work->timer = 15;
			}else{
				kani_work->timer = 10 + mtMathRand() % 20;
			}
		}
	}else{
		// アクションスピードを戻す
		obj_work->obj_3d->speed[0] = 1.0f;

		if (obj_work->move_flag & OBD_MOVE_FRONT || !GmEneComCheckMoveLimit(obj_work, (fx32)obj_work->user_work, (fx32)obj_work->user_flag)) 
		{
			// 前方衝突・移動範囲外
			gmEneKaniFlipInit(obj_work);
			kani_work->timer=0;
		}
	}

	// 気づく方向にいるか?
	if (gmEneKaniIsPlayerLeft(obj_work)){
		// 左は気づく
		kani_work->ata_futa = FALSE;
		if (gmEneKaniGetLength2N(obj_work) < GMD_ENE_KANI_SEARCH_LENGTH * GMD_ENE_KANI_SEARCH_LENGTH ){
			// 距離が近くにいるので、攻撃態勢に入る
			obj_work->ppFunc = gmEneKaniAttackInit;
		}
	}else{
#if 1	// あたふたアクション
		kani_work->ata_futa = TRUE;
#endif
	}
/*
#if 1
	BOOL				b_dec;
	GMS_ENE_KANI_WORK	*kani_work;

	kani_work	= (GMS_ENE_KANI_WORK*)obj_work;

	b_dec = gmEneKaniSetWalkSpeed(kani_work);

	if (b_dec) {
		// 減速を開始した

		// フリップへ移行
		gmEneKaniFlipInit(obj_work);
	}
#else
	if (obj_work->move_flag & OBD_MOVE_FRONT ||
			!GmEneComCheckMoveLimit(obj_work, (fx32)obj_work->user_work, (fx32)obj_work->user_flag)) {
		// 前方衝突・移動範囲外

		// フリップへ移行
		gmEneKaniFlipInit(obj_work);
		// FWへ移行
		//gmEneKaniFwInit(obj_work);
	}
#endif
*/
}

// ==========================================================================
// gmEneKaniAttackInit
/*!
 *	エネミー かにパンチ Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKaniAttackInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// 攻撃アクション設定
	ObjDrawObjectActionSet(obj_work, IDB_ENE_KANI_MTN_ENE_KANI_ATK_01_ZNM);

	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KANI_MTN_ENE_KANI_MOVE_ZNM, IDB_ENE_KANI_MTN_ENE_KANI_MOVE_ZNM);
	// 繰り返さない
	obj_work->disp_flag &= ~OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneKaniAttackMain;

	obj_work->spd.x = 0;

	// パンチSE
	GmSoundPlaySE( GMD_ENE_KANI_SE_PUNCH );

}

// ==========================================================================
// gmEneKaniAttackMain
/*!
 *	エネミー かにパンチ Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKaniAttackMain(OBS_OBJECT_WORK *obj_work)
{
	OBS_RECT_WORK		*rect_work;

	// 攻撃当たりをつける
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	// 仮矩形

	GMS_ENE_KANI_WORK*	kani_work	= (GMS_ENE_KANI_WORK*)obj_work;

	NNS_MATRIX*	mtx = GmEneUtilGetNodeMatrix( &kani_work->node_work, GMD_ENE_KANI_NODE_RIGHT_HAND );
	NNS_VECTOR	v;

	v.x = NNM_MTX( *mtx, 0, 3 ) - FX_FX32_TO_F32( obj_work->pos.x );
	v.y = NNM_MTX( *mtx, 1, 3 ) - FX_FX32_TO_F32( -obj_work->pos.y );
	v.z = NNM_MTX( *mtx, 2, 3 ) - FX_FX32_TO_F32( obj_work->pos.z );

	// 反転しないようにする
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		v.x = -v.x;
	}

	// ***  はさみ(シザー)に当たりを発生させる ***
	ObjRectWorkSet(rect_work, (s16)(-11+(s16)v.x), (s16)(-24-(s16)v.y), (s16)(11+(s16)v.x), (s16)(0-(s16)v.y));
	rect_work->flag |= OBD_RECT_ENABLE;

	if (GmBsCmnIsActionEnd(obj_work)){
		obj_work->ppFunc = gmEneKaniAttackEnd;
		// 戻るアクションに移行
		ObjDrawObjectActionSet(obj_work, IDB_ENE_KANI_MTN_ENE_KANI_ATK_02_ZNM);
	}
}

// ==========================================================================
// gmEneKaniAttackEnd
/*!
 *	エネミー かにパンチ 攻撃戻り
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKaniAttackEnd(OBS_OBJECT_WORK *obj_work)
{
	OBS_RECT_WORK		*rect_work;

	// 攻撃当たりをつける
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	// 仮矩形

	GMS_ENE_KANI_WORK*	kani_work	= (GMS_ENE_KANI_WORK*)obj_work;

	NNS_MATRIX*	mtx = GmEneUtilGetNodeMatrix( &kani_work->node_work, GMD_ENE_KANI_NODE_RIGHT_HAND );
	NNS_VECTOR	v;

	v.x = NNM_MTX( *mtx, 0, 3 ) - FX_FX32_TO_F32( obj_work->pos.x );
	v.y = NNM_MTX( *mtx, 1, 3 ) - FX_FX32_TO_F32( -obj_work->pos.y );
	v.z = NNM_MTX( *mtx, 2, 3 ) - FX_FX32_TO_F32( obj_work->pos.z );

	// 反転しないようにする
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		v.x = -v.x;
	}

	ObjRectWorkSet(rect_work, (s16)(-11+(s16)v.x), (s16)(-24-(s16)v.y), (s16)(11+(s16)v.x), (s16)(0-(s16)v.y));
	rect_work->flag |= OBD_RECT_ENABLE;

	if (GmBsCmnIsActionEnd(obj_work)){
		obj_work->ppFunc = gmEneKaniWalkInit;
		// 当たりを戻す
	}
}


#if 0
// ==========================================================================
// gmEneKaniFwInit
/*!
 *	エネミー かにパンチ FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKaniFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KANI_MTN_ENE_KANI_FW_ZNM, IDB_ENE_KANI_MTN_ENE_KANI_FW_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_KANI_MTN_ENE_KANI_FW_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneKaniFwMain;

	// 移動速度設定
	obj_work->spd.x = 0;

	// 演出時間
	obj_work->user_timer = GMD_ENE_KANI_FW_TIME;
}
#endif

// ==========================================================================
// gmEneKaniFwMain
/*!
 *	エネミー かにパンチ FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKaniFwMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	if (obj_work->user_timer <= 0) {
		// フリップへ移行
		gmEneKaniFlipInit(obj_work);
	}
}



// ==========================================================================
// gmEneKaniFlipInit
/*!
 *	エネミー かにパンチ フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKaniFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KANI_MTN_ENE_KANI_FRIP_ZNM, IDB_ENE_KANI_MTN_ENE_KANI_FRIP_L_ZNM);

	// TODO Flip
//	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_KANI_MTN_ENE_KANI_FRIP_ZNM, IDB_ENE_KANI_MTN_ENE_KANI_FRIP_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_KANI_MTN_ENE_KANI_FRIP_ZNM);

	obj_work->spd.x = 0;

	// メイン処理
	obj_work->ppFunc = gmEneKaniFlipMain;

	// 移動速度設定
	//obj_work->spd.x = 0;
}

// ==========================================================================
// gmEneKaniFlipMain
/*!
 *	エネミー かにパンチ フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKaniFlipMain(OBS_OBJECT_WORK *obj_work)
{
	// 移動速度設定
	gmEneKaniSetWalkSpeed((GMS_ENE_KANI_WORK*)obj_work);

	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		// Walkへ
		gmEneKaniWalkInit(obj_work);
	}
}


// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneKaniSetWalkSpeed
/*!
 *	エネミー かにパンチ 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneKaniSetWalkSpeed(GMS_ENE_KANI_WORK *kani_work)
{
	BOOL			b_dec = FALSE;
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)kani_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// 左向き
		
		/* TODO 振り向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_KANI_MTN_ENE_KANI_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_KANI_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, kani_work->spd_dec, GMD_ENE_KANI_MOVE_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + kani_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, kani_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -kani_work->spd_dec) {
					obj_work->spd.x = -kani_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_KANI_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -kani_work->spd_dec, GMD_ENE_KANI_MOVE_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_KANI_MTN_ENE_KANI_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_KANI_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -kani_work->spd_dec, GMD_ENE_KANI_MOVE_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - kani_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, kani_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > kani_work->spd_dec) {
					obj_work->spd.x = kani_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_KANI_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, kani_work->spd_dec, GMD_ENE_KANI_MOVE_SPD_X);
		}
		*/
	}

	return (b_dec);
}







// =======================================================================
//	GmEneUtilInitNodeMatrix()
/*!
	ノード参照システム簡易化
  
	@param	obj_work	[io]	オブジェクトワーク
	@param	node_work	[io]	ノードワーク
	@param	max_node	[in]	使用できるNodeMatrixの数

 */
// =======================================================================
void GmEneUtilInitNodeMatrix( GMS_ENE_NODE_MATRIX* node_work, OBS_OBJECT_WORK* obj_work, Sint32 max_node )
{
	// ノード閲覧システム導入
	node_work->initCount	= max_node;
	node_work->useCount		= 0;

	// BMCBシステム初期化
	GmBsCmnInitBossMotionCBSystem( obj_work, &node_work->mtn_mgr );
	
	// ノードマトリクス取得初期化
	GmBsCmnCreateSNMWork(	&node_work->snm_work,
							obj_work->obj_3d->object,
							(Uint16)max_node );

	// モーションコールバックを実行リストに追加
	GmBsCmnAppendBossMotionCallback( &node_work->mtn_mgr,
									 &node_work->snm_work.bmcb_link );


	node_work->obj_work = obj_work;

	for( int i=0;i< GMD_ENE_SNM_NO_MAX; i++){
		node_work->work[ i ] = -1;
	}

	// 初期化されたことを示す
	strcpy( node_work->_id, "SNM SYS" );

}

// =======================================================================
// gmEneExit
/*!
  本体終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void gmEneExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK*	obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_ENE_KANI_WORK*	kani_work	= (GMS_ENE_KANI_WORK*)obj_work;
	
	GmEneUtilExitNodeMatrix( &kani_work->node_work );
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}


// =======================================================================
//	GmEneUtilExitNodeMatrix()
/*!
	ノード参照システム終了
  
	@param	obj_work	[io]	オブジェクトワーク
	@param	node_work	[io]	ノードワーク
	@param	max_node	[in]	使用できるNodeMatrixの数

 */
// =======================================================================
void GmEneUtilExitNodeMatrix( GMS_ENE_NODE_MATRIX* node_work )
{
	if (strcmp( node_work->_id, "SNM SYS" )!=0){
		// 初期化されていないか異常
#ifdef	_DEBUG
		amSystemLog( "初期化されていないノードシステムを開放しようとしています" );
#endif	//_DEBUG
		return;
	}

	// ボスモーションコールバックシステム解除・クリア
	GmBsCmnClearBossMotionCBSystem( node_work->obj_work );
	
	// ノードマトリクス取得関連解放
	GmBsCmnDeleteSNMWork( &node_work->snm_work );
	
	// 初期化してない状態に変更
	node_work->_id[0] = '\0';
}

// =======================================================================
//	GmEneUtilGetNodeMatrix()
/*!
	ノード参照システム簡易化
  
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
NNS_MATRIX* GmEneUtilGetNodeMatrix( GMS_ENE_NODE_MATRIX* node_work, Sint32 node_id )
{
#ifdef	_DEBUG
	if (node_id < 0){
		amSystemLog( "参照しようとしているノード番号が異常です:%d", node_id );
		return NULL;
	}
	if (node_id >= GMD_ENE_SNM_NO_MAX){
		amSystemLog( "参照しようとしているノード番号がGMD_ENE_SNM_NO_MAXを越えています : %d >= %d", node_id, GMD_ENE_SNM_NO_MAX );
		return NULL;
	}
#endif

	if (node_work->work[ node_id ]<0){

#ifdef	_DEBUG
		// ノード登録が可能?
		if ( node_work->initCount <= node_work->useCount ){
			amSystemLog( "ノード登録ができません。GmEneUtilInitNodeMatrix()の際にワークを増やしてください(現在=%d)", node_work->initCount );
			return NULL;		
		}
		node_work->useCount++;
#endif	//_DEBUG

		// ノードマトリクス取得ノード追加
		node_work->work[ node_id ] =
			GmBsCmnRegisterSNMNode( &node_work->snm_work, node_id );

		// 注意: 初期1フレームはきちんとした値が入らない。
		// obj_3dでobj_mtxを確保することでこの呼び出し方ができるのだが・・・。
		/*
		// 初期は即コールバックを呼ぶ
		amMatrixPush(&obj_mtx);
		node_work->obj_work->obj_3d->mtn_cb_func(	node_work->obj_work->obj_3d->motion, 
													node_work->obj_work->obj_3d->object,
													node_work->obj_work->obj_3d->mtn_cb_param);

		amMatrixPop();
		*/
	}
	
	NNS_MATRIX	*w_mtx;

	// ノードのワールドマトリクス取得
	w_mtx	= GmBsCmnGetSNMMtx( &node_work->snm_work,
								   node_work->work[ node_id ] );

	return w_mtx;
}




// =======================================================================
//	GmEneUtilSetMatrixNN()
/*!
	マトリクスにくっつける
  
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
void	GmEneUtilSetMatrixNN( OBS_OBJECT_WORK* obj_work, NNS_MATRIX*	w_mtx )
{
	NNS_MATRIX	*user_obj_mtx_r;
//	NNS_MATRIX	msm;
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	
	// ユーザマトリクス領域取得
	user_obj_mtx_r	= &obj_work->obj_3d->user_obj_mtx_r;
	
	// ノードにくっつける
	obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));
	obj_work->pos.y	= -FX_F32_TO_FX32((NNM_MTX(*w_mtx, 1, 3)));
	obj_work->pos.z	= FX_F32_TO_FX32((NNM_MTX(*w_mtx, 2, 3)));
	
	// 回転
	if (1){	//b_rotation) {
		// ノードの回転を反映
		obj_work->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
		AkMathNormalizeMtx(user_obj_mtx_r, w_mtx);
	}
	else {
		// 単位行列をセット
		obj_work->disp_flag	&= ~OBD_DISP_USERMTX_RIGHT;
		nnMakeUnitMatrix(user_obj_mtx_r);
	}
}



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
