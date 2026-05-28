// ==========================================================================
/*!
  @file gmEneHaro.cpp
  @brief エネミー ハロゲン

  @author Yurita
				Copyright(c) 2009 Dimps

  $Id: gmEneHarogen.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmEneHarogen.h"

#include "gmEffect.h"
#include "gmComEfct.h"
#include "gmEffectEnemy.h"

#include "gmEneKaniPunch.h"		//!< ノードシステム

#include "akMath.h"
#include "gmSound.h"

// データヘッダ
#include "common/model/ene_haro_mtn.hmb"
#include "common/model/ene_haro_mdl.hmb"


#if	defined(_WII) && !defined(_PS3)
#pragma warning off (10178)			// 型宣言のプロトタイプのワーニングをOFFにする
									// ただしstatic宣言はつけれないため、問題がある場合は直接修正する
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
#endif	//_WII

//----- Definitions ---------------------------------------------------------

//===========================================================================
//	ノード関係
//===========================================================================
#define		GMD_ENE_HARO_LIGHT_NODE						(2)

#define		GMD_ENE_HARO_NODE_NO_MAX					(16)				//<! 上のノード番号より大きな数が必要です

//===========================================================================
//	ツール設定でのイベント関係
//===========================================================================
//			GMS_EVE_RECORD_EVENT.flag
#define		GMD_ENE_HARO_EVE_FLAG_RIGHT					(0x0001)			//!< 右向き開始

//===========================================================================
//	反転関係(未使用)
//===========================================================================
#define		GMD_ENE_HARO_MOVE_SPD_X						(0x0800)			//!< 移動速度
#define		GMD_ENE_HARO_FW_TIME						(15*FX32_ONE)		//!< FW時間
#define		GMD_ENE_HARO_TURN_FRAME						(40)				//!< ターンモーションフレーム

//===========================================================================
//	アクション関係
//===========================================================================
#define		GMD_ENE_HARO_NEAR_PLAYER					(100)				//!< アクションが切り替わる距離
#define		GMD_ENE_HARO_CHECK_ACT_TIME					(120)				//!< アクション切り替えのチェック間隔
																			//　 方向の正規化も含むためあまり長いと動きがおかしくなる
#define		GMD_ENE_HARO_SEARCH_PLAYER					(100)				//!< プレイヤーを発見できる距離(9/25 VGAでもある程度画面に入る距離に変更)

//===========================================================================
//	回転関係
//===========================================================================
#define		GMD_ENE_HARO_ANGLE_ADD_SPD					(0.030f)			//!< 回転加速度
#define		GMD_ENE_HARO_ANGLE_ADD_SPD_LIMIT			(0.35f)				//!< 回転加速度のリミット
#define		GMD_ENE_HARO_ANGLE_SPD_LIMIT				(1.3f)				//!< 回転速度のリミット
#define		GMD_ENE_HARD_FUNC_INERTIA					(0.96f)				//!< 慣性の係数
#define		GMD_ENE_HARD_SPEED							(1.5f)				//!< 進むスピード係数
#define		GMD_ENE_HARD_INERTIA_VOLUME					(0.025)				//!< 慣性の足しこみ係数

//===========================================================================
/// ハロゲンワーク
//===========================================================================
typedef struct tag_GMS_ENE_HARO_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離

	VecFx32					vvv;				//!< 間性
	VecFx32					vec;				//!< 進行方向
	Angle32					angle;				//!< 回転
	Angle32					angle_add;			//!< 回転
	fx32					spd;				//!< 移動スピード
	Sint32					timer;				//!< 考える時間の周期

	BOOL					lighton;
//	GMS_EFFECT_3DES_WORK*	eff_light;

	Angle32					targetAngle;			//!< 回転アングル

	GMS_ENE_NODE_MATRIX		node_work;

} GMS_ENE_HARO_WORK;
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
// 現在gmEneKaniPunchにある(問題なければgmEneComに移動する)
extern void			GmEneUtilInitNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work, OBS_OBJECT_WORK* obj_work, Sint32 max_node );
extern NNS_MATRIX*	GmEneUtilGetNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work, Sint32 node_id );
extern void			GmEneUtilExitNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work );
extern void			GmEneUtilSetMatrixNN	( OBS_OBJECT_WORK* obj_work, NNS_MATRIX*	w_mtx );

static void gmEneHaroWaitInit(OBS_OBJECT_WORK *obj_work);
static void gmEneHaroWaitMain(OBS_OBJECT_WORK *obj_work);
static void gmEneHaroWalkInit(OBS_OBJECT_WORK *obj_work);
static void gmEneHaroWalkMain(OBS_OBJECT_WORK *obj_work);
//static void gmEneHaroFwInit(OBS_OBJECT_WORK *obj_work);
//static void gmEneHaroFwMain(OBS_OBJECT_WORK *obj_work);
static void gmEneHaroFlipInit(OBS_OBJECT_WORK *obj_work);
static void gmEneHaroFlipMain(OBS_OBJECT_WORK *obj_work);

static BOOL gmEneHaroSetWalkSpeed(GMS_ENE_HARO_WORK *haro_work);

// 終了宣言
static	void gmEneHaroExit(MTS_TASK_TCB *tcb);

// ハロゲンエフェクトのメイン処理
static	void gmEneEffectMainFuncHarogen(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_haro_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneHaroBuild
/*!
 *	エネミー ハロゲン データ構築
 */
// ==========================================================================
void GmEneHaroBuild(void)
{
	gm_ene_haro_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_HARO_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_HARO_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneHaroFlush
/*!
 *	エネミー ハロゲン データ片付け
 */
// ==========================================================================
void GmEneHaroFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_HARO_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_haro_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneHaroInit
/*!
 *	エネミー ハロゲン 初期化関数
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
OBS_OBJECT_WORK* GmEneHaroInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_HARO_WORK	*haro_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_HARO_WORK), "ENE_HARO");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	haro_work = (GMS_ENE_HARO_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_haro_obj_3d_list[IDB_ENE_HARO_MDL_ENE_HARO_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_HARO_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

	// 優先設定
//	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_B_FRONT;	// B面の前//	GMD_OBJ_ENEMY_POS_Z;
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;	// A面の前

	// ハロゲン常に前を向くようにしておく
	// 向きを追加したため消去(11/4)
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;


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

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag &= ~OBD_MOVE_FALL;	// 落ちない
	obj_work->move_flag |= OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し

	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_HARO_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
	haro_work->spd_dec		 = GMD_ENE_HARO_MOVE_SPD_X / (GMD_ENE_HARO_TURN_FRAME/2);
	haro_work->spd_dec_dist = GMD_ENE_HARO_MOVE_SPD_X * (GMD_ENE_HARO_TURN_FRAME/2) / 2;

	haro_work->vec.x = 0;
	haro_work->vec.y = FX_F32_TO_FX32( 1.0 );

	haro_work->angle = 0;
	haro_work->angle_add = 0;

	// ハロゲンランプ
//	haro_work->eff_light	= NULL;
	haro_work->lighton		= FALSE;

#if 1	// ハロゲンエフェクトの位置を一定化するため導入
	// ノードシステムを使用する
	GmEneUtilInitNodeMatrix( &haro_work->node_work, obj_work, GMD_ENE_HARO_NODE_NO_MAX );
	// ノードシステムを使用するため開放処理を変える(できればGmEneUtilInitNodeMatrixで設定したいが…workの形が個別になるため現在厳しい)
	mtTaskChangeTcbDestructor(obj_work->tcb, gmEneHaroExit);

	// 初期登録
	GmEneUtilGetNodeMatrix( &haro_work->node_work, GMD_ENE_HARO_LIGHT_NODE );
#endif

	gmEneHaroWaitInit(obj_work);

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}



//----- Local Functions -----------------------------------------------------
// =======================================================================
// gmEneKamaExit
/*!
  本体終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void gmEneHaroExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK*	obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_ENE_HARO_WORK*	haro_work	= (GMS_ENE_HARO_WORK*)obj_work;
	
	GmEneUtilExitNodeMatrix( &haro_work->node_work );
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}

// ==========================================================================
// gmEneHaroGetLength2N()
/*!
 *	プレイヤーとの距離の自乗を求める
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
Sint32	gmEneHaroGetLength2N( OBS_OBJECT_WORK* obj_work )
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
//	gmEneHaroIsPlayerLeft()
/*!
 *	エネミー ハロゲン プレイヤーのいる方向が左かをチェックする
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneHaroIsPlayerLeft(GMS_ENE_HARO_WORK* obj_work)
{
	OBS_OBJECT_WORK		*ply_work;
	ply_work = (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	// 20091021 Dimps Ishizaki
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_DIE) {
		// プレイヤー死亡時は左側にする
		return (TRUE);
	}

	VecFx32	v;
	v.x = ply_work->pos.x - obj_work->ene_3d_work.ene_com.obj_work.pos.x;
	v.y = ply_work->pos.y - obj_work->ene_3d_work.ene_com.obj_work.pos.y;

	fx32 cross = FX_Mul( v.x, obj_work->vec.y ) - FX_Mul( v.y, obj_work->vec.x );

	if (cross > 0){
		return FALSE;
	}
	return TRUE;
}

// ==========================================================================
//	gmEneHaroIsPlayerLeft()
/*!
 *	エネミー ハロゲン プレイヤーのいる方向がほぼその方向かチェック
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneHaroIsPlayerCenter(GMS_ENE_HARO_WORK* obj_work)
{
	OBS_OBJECT_WORK		*ply_work;
	ply_work = (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	// 20091021 Dimps Ishizaki
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_DIE) {
		// プレイヤー死亡時はTRUEにする
		return (TRUE);
	}

	VecFx32	v;
	v.x = ply_work->pos.x - obj_work->ene_3d_work.ene_com.obj_work.pos.x;
	v.y = ply_work->pos.y - obj_work->ene_3d_work.ene_com.obj_work.pos.y;

	fx32 cross = FX_Mul( v.x, obj_work->vec.y ) - FX_Mul( v.y, obj_work->vec.x );

	if ( (cross < FX_F32_TO_FX32( 0.2f )) && (cross > -FX_F32_TO_FX32( 0.2f )) ){
		return TRUE;
	}
	return FALSE;
}

// ==========================================================================
//	gmEneHaroIsPlayerFront()
/*!
 *	エネミー ハロゲン プレイヤーのいる方向が前かをチェックする
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneHaroIsPlayerFront(GMS_ENE_HARO_WORK* obj_work)
{
	OBS_OBJECT_WORK		*ply_work;
	ply_work = (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	// 20091021 Dimps Ishizaki
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_DIE) {
		// プレイヤー死亡時はTRUEにする
		return (TRUE);
	}

	// 相手の方向
	VecFx32	v;
	v.x = ply_work->pos.x - obj_work->ene_3d_work.ene_com.obj_work.pos.x;
	v.y = ply_work->pos.y - obj_work->ene_3d_work.ene_com.obj_work.pos.y;

	// 自分の方向と比べる
	fx32	dot = FX_Mul( v.x, obj_work->vec.x  ) + FX_Mul( v.y, obj_work->vec.y );

	if (dot>0){
		return TRUE;
	}
	return FALSE;
}


// ==========================================================================
// gmEneHaroWaitInit
/*!
 *	エネミー ハロゲン Wait初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneHaroWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_HARO_WORK	*haro_work;
	haro_work	= (GMS_ENE_HARO_WORK*)obj_work;

	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	ObjDrawObjectActionSet(obj_work, /*IDB_ENE_HARO_MTN_ENE_HARO_FW_ZNM*/IDB_ENE_HARO_MTN_ENE_HARO_ST_ZNM);

//	ObjDrawObjectActionSet(obj_work, /*IDB_ENE_HARO_MTN_ENE_HARO_FW_ZNM*/IDB_ENE_HARO_MTN_ENE_HARO_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneHaroWaitMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;


	// 正規化
	fx32 len = FX_Sqrt( FX_Mul( haro_work->vec.x, haro_work->vec.x ) + FX_Mul( haro_work->vec.y, haro_work->vec.y ) );
	if (len==0){
		haro_work->vec.x = 0;
		haro_work->vec.y = FX_F32_TO_FX32(1);
	}else{
		haro_work->vec.x = FX_Div( haro_work->vec.x, len );
		haro_work->vec.y = FX_Div( haro_work->vec.y, len );
	}

	/*
	// ハロゲンエフェクト生成
	GMS_EFFECT_3DES_WORK	*efct_work;
	if (!haro_work->lighton){	
		efct_work = GmEfctEneEsCreate( obj_work, GME_EFCT_ENE_IDX_E06_HARO);
		haro_work->lighton = TRUE;
	}
	*/
}

// ==========================================================================
// gmEneHaroWaitMain
/*!
 *	エネミー ハロゲン Wait メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneHaroWaitMain(OBS_OBJECT_WORK *obj_work)
{

	// 近い場合
	if (gmEneHaroGetLength2N( obj_work ) <= (GMD_ENE_HARO_SEARCH_PLAYER * GMD_ENE_HARO_SEARCH_PLAYER )){

		// 移動開始
		GmSoundPlaySE("Halogen");

		obj_work->obj_3d->blend_spd = 0.05f;
		ObjDrawObjectActionSet3DNNBlend( obj_work, IDB_ENE_HARO_MTN_ENE_HARO_MOVE_ZNM );
//		ObjDrawObjectActionSet(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_MOVE_ZNM);
		obj_work->disp_flag |= OBD_DISP_REPEAT;

		// 移動開始処理
		obj_work->ppFunc = gmEneHaroWalkInit;
		
	}
}



// ==========================================================================
// gmEneHaroWalkInit
/*!
 *	エネミー ハロゲン Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneHaroWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_HARO_WORK	*haro_work;
	haro_work	= (GMS_ENE_HARO_WORK*)obj_work;

	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_MOVE_ZNM, IDB_ENE_HARO_MTN_ENE_HARO_MOVE_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneHaroWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;


	// 正規化
	fx32 len = FX_Sqrt( FX_Mul( haro_work->vec.x, haro_work->vec.x ) + FX_Mul( haro_work->vec.y, haro_work->vec.y ) );
	if (len==0){
		haro_work->vec.x = 0;
		haro_work->vec.y = FX_F32_TO_FX32(1);
	}else{
		haro_work->vec.x = FX_Div( haro_work->vec.x, len );
		haro_work->vec.y = FX_Div( haro_work->vec.y, len );
	}

	haro_work->timer = GMD_ENE_HARO_CHECK_ACT_TIME;

	// ハロゲンエフェクト生成
	if (!haro_work->lighton){	

		GMS_EFFECT_3DES_WORK*	eff_light;
		eff_light = GmEfctEneEsCreate( obj_work, GME_EFCT_ENE_IDX_E06_HARO);
		// 実行アドレスの変更を行う
		eff_light->efct_com.obj_work.ppFunc = gmEneEffectMainFuncHarogen;

		haro_work->lighton = TRUE;
	}


	// 近い場合
	if (gmEneHaroGetLength2N( obj_work ) <= (GMD_ENE_HARO_NEAR_PLAYER * GMD_ENE_HARO_NEAR_PLAYER )){

		// SE
		GmSoundPlaySE("Halogen");

		ObjDrawObjectActionSet(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_MOVE_ZNM);
		obj_work->disp_flag |= OBD_DISP_REPEAT;

	}else{

		ObjDrawObjectActionSet(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_MOVE_ZNM);
//		ObjDrawObjectActionSet(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_FW_ZNM);
		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}
}

// ==========================================================================
// gmGmkSpringFwMain
/*!
 *	エネミー ハロゲン Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneHaroWalkMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_HARO_WORK	*haro_work;
	haro_work	= (GMS_ENE_HARO_WORK*)obj_work;

	if (gmEneHaroIsPlayerCenter( haro_work)){

	}else{

		if (gmEneHaroIsPlayerLeft( haro_work )){
			// 左回転
			haro_work->angle_add -= AKM_DEGtoA32( GMD_ENE_HARO_ANGLE_ADD_SPD );
			if (haro_work->angle_add < -AKM_DEGtoA32( GMD_ENE_HARO_ANGLE_ADD_SPD_LIMIT )){
				haro_work->angle_add = -AKM_DEGtoA32( GMD_ENE_HARO_ANGLE_ADD_SPD_LIMIT );
			}
			haro_work->angle += haro_work->angle_add;
		}else{
			// 右回転
			haro_work->angle_add += AKM_DEGtoA32( GMD_ENE_HARO_ANGLE_ADD_SPD );
			if (haro_work->angle_add > AKM_DEGtoA32( GMD_ENE_HARO_ANGLE_ADD_SPD_LIMIT )){
				haro_work->angle_add = AKM_DEGtoA32( GMD_ENE_HARO_ANGLE_ADD_SPD_LIMIT );
			}
			haro_work->angle += haro_work->angle_add;
		}
		if (haro_work->angle < -AKM_DEGtoA32( GMD_ENE_HARO_ANGLE_SPD_LIMIT )){
			haro_work->angle = -AKM_DEGtoA32( GMD_ENE_HARO_ANGLE_SPD_LIMIT );
		}
		if (haro_work->angle > AKM_DEGtoA32( GMD_ENE_HARO_ANGLE_SPD_LIMIT )){
			haro_work->angle = AKM_DEGtoA32( GMD_ENE_HARO_ANGLE_SPD_LIMIT );
		}
	}

	fx32 cos = FX_Cos( haro_work->angle );
	fx32 sin = FX_Sin( haro_work->angle );

	haro_work->vec.x = FX_Mul( haro_work->vec.x, cos ) + FX_Mul( haro_work->vec.y, sin );
	haro_work->vec.y = FX_Mul( haro_work->vec.x, -sin) + FX_Mul( haro_work->vec.y, cos );

	haro_work->vvv.x = (fx32)(haro_work->vvv.x * GMD_ENE_HARD_FUNC_INERTIA);
	haro_work->vvv.y = (fx32)(haro_work->vvv.x * GMD_ENE_HARD_FUNC_INERTIA);
	haro_work->vvv.x += haro_work->vec.x;
	haro_work->vvv.y += haro_work->vec.y;

	haro_work->spd = FX_F32_TO_FX32( GMD_ENE_HARD_SPEED );

	obj_work->spd.x = FX_Mul( haro_work->vec.x, haro_work->spd );
	obj_work->spd.y = FX_Mul( haro_work->vec.y, haro_work->spd );

	obj_work->spd.x += FX_Mul( haro_work->vvv.x, FX_F32_TO_FX32(GMD_ENE_HARD_INERTIA_VOLUME) );
	obj_work->spd.y += FX_Mul( haro_work->vvv.y, FX_F32_TO_FX32(GMD_ENE_HARD_INERTIA_VOLUME) );
/*
	// とりあえず
	if (AoPadStand() & KEY_L1) {
		if (g_obj.flag & OBD_OBJ_RECT_D){
			g_obj.flag &= ~OBD_OBJ_RECT_D;
		}else{
			g_obj.flag |= OBD_OBJ_RECT_D;
		}
	}
*/
	if (haro_work->timer>0){
		haro_work->timer--;
	}else{
		obj_work->ppFunc = gmEneHaroWalkInit;	
	}
	
	// むりから反転
	if (haro_work->vec.x < 0){
		obj_work->disp_flag &= ~OBD_DISP_HFLIP;
		haro_work->targetAngle = AKM_DEGtoA32(90+90+50+20);

	}else{
		obj_work->disp_flag &= ~OBD_DISP_HFLIP;
		haro_work->targetAngle = AKM_DEGtoA32(310+20);
	}
	
	if (obj_work->dir.y > haro_work->targetAngle){
		obj_work->dir.y-= AKM_DEGtoA32(5);
	}
	if (obj_work->dir.y < haro_work->targetAngle){
		obj_work->dir.y+= AKM_DEGtoA32(5);
	}

//	エフェクトのズレを簡易で補完する
// 上下動が一定でないため、完全補完できていませんが、
// 見栄えは少しましになるため、とりあえず入れておきます。
// 現在、20フレームで１ターンを行っているため、それにあわせています。	
//	yurita
#if	0
#define	GMD_ENE_HARO_TURN_UP_DOWN	(20)
	// エフェクト追尾
	if (haro_work->lighton){
		float cnt = haro_work->ene_3d_work.obj_3d.frame[0];
		if (cnt > GMD_ENE_HARO_TURN_UP_DOWN) 
			cnt-= GMD_ENE_HARO_TURN_UP_DOWN;
		if (cnt>GMD_ENE_HARO_TURN_UP_DOWN/2)
			cnt = GMD_ENE_HARO_TURN_UP_DOWN-cnt;

		// 現在直値で対応
		GmComEfctSetDispOffsetF( haro_work->eff_light, 0.f, -0.5f + cnt*0.4f, 8.f);
	}
#endif

}


#if 0
// ==========================================================================
// gmEneHaroFwInit
/*!
 *	エネミー ハロゲン FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneHaroFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_FW_ZNM, IDB_ENE_HARO_MTN_ENE_HARO_FW_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_FW_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneHaroFwMain;

	// 移動速度設定
	obj_work->spd.x = 0;

	// 演出時間
	obj_work->user_timer = GMD_ENE_HARO_FW_TIME;
}
#endif

// ==========================================================================
// gmEneHaroFwMain
/*!
 *	エネミー ハロゲン FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneHaroFwMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	if (obj_work->user_timer <= 0) {
		// フリップへ移行
		gmEneHaroFlipInit(obj_work);
	}
}



// ==========================================================================
// gmEneHaroFlipInit
/*!
 *	エネミー ハロゲン フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneHaroFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_FRIP_ZNM, IDB_ENE_HARO_MTN_ENE_HARO_FRIP_L_ZNM);
	//GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_FRIP_ZNM, IDB_ENE_HARO_MTN_ENE_HARO_FRIP_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_HARO_MTN_ENE_HARO_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneHaroFlipMain;

	// 移動速度設定
	//obj_work->spd.x = 0;
}

// ==========================================================================
// gmEneHaroFlipMain
/*!
 *	エネミー ハロゲン フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneHaroFlipMain(OBS_OBJECT_WORK *obj_work)
{
	// 移動速度設定
	gmEneHaroSetWalkSpeed((GMS_ENE_HARO_WORK*)obj_work);

	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		// Walkへ
		gmEneHaroWalkInit(obj_work);
	}
}


// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneHaroSetWalkSpeed
/*!
 *	エネミー ハロゲン 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneHaroSetWalkSpeed(GMS_ENE_HARO_WORK *haro_work)
{
	BOOL			b_dec = FALSE;
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)haro_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
/*
		// 左向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_HARO_MTN_ENE_HARO_FRIP_L_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_HARO_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, haro_work->spd_dec, GMD_ENE_HARO_MOVE_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + haro_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, haro_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -haro_work->spd_dec) {
					obj_work->spd.x = -haro_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_HARO_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -haro_work->spd_dec, GMD_ENE_HARO_MOVE_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_HARO_MTN_ENE_HARO_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_HARO_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -haro_work->spd_dec, GMD_ENE_HARO_MOVE_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - haro_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, haro_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > haro_work->spd_dec) {
					obj_work->spd.x = haro_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_HARO_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, haro_work->spd_dec, GMD_ENE_HARO_MOVE_SPD_X);
		}
*/
	}

	return (b_dec);
}

void gmEneEffectMainFuncHarogen(OBS_OBJECT_WORK *obj_work)
{
	NNS_MATRIX*	mtx = NULL;
	// 親のノードに引っ付く
	if ( obj_work->parent_obj!=NULL ){

		GMS_ENE_HARO_WORK*	haro_work = (GMS_ENE_HARO_WORK*)obj_work->parent_obj;

		mtx = GmEneUtilGetNodeMatrix( &haro_work->node_work, GMD_ENE_HARO_LIGHT_NODE );

		if (mtx!=NULL){

			float x = NNM_MTX( *mtx, 0, 3 ) - FX_FX32_TO_F32( haro_work->ene_3d_work.ene_com.obj_work.pos.x );
			float y = NNM_MTX( *mtx, 1, 3 ) + FX_FX32_TO_F32( haro_work->ene_3d_work.ene_com.obj_work.pos.y );
			float z = NNM_MTX( *mtx, 2, 3 ) - FX_FX32_TO_F32( haro_work->ene_3d_work.ene_com.obj_work.pos.z );


			// ハロゲンの移動スピードを足しこむ
			x += FX_FX32_TO_F32(haro_work->ene_3d_work.ene_com.obj_work.spd.x);
			y -= FX_FX32_TO_F32(haro_work->ene_3d_work.ene_com.obj_work.spd.y);
			z += FX_FX32_TO_F32(haro_work->ene_3d_work.ene_com.obj_work.spd.z);

//			GMS_EFFECT_3DES_WORK*	eff_work = (GMS_EFFECT_3DES_WORK *)obj_work;

//			GmComEfctSetDispOffsetF( (GMS_EFFECT_3DES_WORK *)obj_work, 
//				x, -y + 15.0f/*+3.0f*/, z + 10.0f
//			);
			obj_work->parent_ofst.x = FX_F32_TO_FX32( x );
			obj_work->parent_ofst.y = -FX_F32_TO_FX32( y -10.0f );
			obj_work->parent_ofst.z = FX_F32_TO_FX32( z );


		}
		
	}else{
		// 念のため親がいないときは消去
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}


	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
