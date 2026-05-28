// ==========================================================================
/*!
  @file gmEneTStar.cpp
  @brief エネミー テルスター

  @author Yurita
				Copyright(c) 2009 Dimps

  $Id: gmEneTeruStar.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmEneKaniPunch.h"		// ノードシステム
#include "gmEneTeruStar.h"

#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectEnemy.h"

#include "gmSound.h"

// データヘッダ
#include "common/model/ene_t_star_mtn.hmb"
#include "common/model/ene_t_star_mdl.hmb"
#include "common/model/ene_t_star_mat.hmb"

#include "akMath.h"

#if	defined(_WII) && !defined(_PS3)
#pragma warning off (10178)			// 型宣言のプロトタイプのワーニングをOFFにする
									// ただしstatic宣言はつけれないため、問題がある場合は直接修正する
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
#endif	//_WII

//----- Definitions ---------------------------------------------------------

//===========================================================================
//	ツール設定でのイベント関係(現在機能せず)
//===========================================================================
//			GMS_EVE_RECORD_EVENT.flag
#define		GMD_ENE_T_STAR_EVE_FLAG_RIGHT					(0x0001)		//!< 右向き開始

#define		GMD_ENE_T_STAR_EVE_FLAG_4_8					(0x0001)		//!< 1/2のスピード
#define		GMD_ENE_T_STAR_EVE_FLAG_2_8					(0x0002)		//!< 1/4のスピード
#define		GMD_ENE_T_STAR_EVE_FLAG_1_8					(0x0004)		//!< 1/8のスピード

//===========================================================================
//	反転関係(機能せず)
//===========================================================================
//#define	GMD_ENE_T_STAR_MOVE_SPD_X						(0x0800)		//!< 移動速度
//#define	GMD_ENE_T_STAR_FW_TIME							(15*FX32_ONE)	//!< FW時間
//#define	GMD_ENE_T_STAR_TURN_FRAME						(40)			//!< ターンモーションフレーム

//===========================================================================
//	アクション関係
//===========================================================================
#define		GMD_ENE_T_STAR_MOVE_SPD							(0.5f)			//!< 移動速度
#define		GMD_ENE_T_STAR_SEARCH_LENGTH					(128)			//!< プレイヤーを感知する距離

#define		GME_ENE_T_STAR_WALK_TIME						(60*2)			//!< 移動時間 TODO PAL
#define		GME_ENE_T_STAR_STOP_TIME						(15)			//!< 移動時間 TODO PAL

#define		GME_ENE_T_START_SIZE							(1.25f)			//!< 本体サイズ
//===========================================================================
//	ノード関係
//===========================================================================
//	※アクションがなく、取得不可だったためこの方法は使用せず
#define		GMD_ENE_T_STAR_NODE_0							(4)
#define		GMD_ENE_T_STAR_NODE_1							(5)
#define		GMD_ENE_T_STAR_NODE_2							(6)
#define		GMD_ENE_T_STAR_NODE_3							(7)
#define		GMD_ENE_T_STAR_NODE_4							(8)

#define		GMD_ENE_T_STAR_NODE_MAX							(10)				//!< 上のものより大きな数が必要
//===========================================================================
//	弾関係
//===========================================================================
#define		GMD_ENE_T_STAR_NEEDLE_SPD						(4)				//!< 弾のスピード
#define		GMD_ENE_T_STAR_NEEDLE_ROT_DEG					(72)			//!< 360/5
#define		GMD_ENE_T_STAR_NEEDLE_LIFT_TIME					(60*5)			//!< 弾が存在する時間

#define		GMD_ENE_T_STAR_NEEDLE_RADIUS					(10.0f)			//!< 発生初期の位置(中心からの距離)
//===========================================================================
//	サウンド関係
//===========================================================================
#define		GMD_ENE_T_STAR_SE_BOMB							"Boss2_03"


//===========================================================================
/// テルスターワーク
//===========================================================================
typedef struct tag_GMS_ENE_T_STAR_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離

	Sint32					timer;
	GMS_ENE_NODE_MATRIX		node_work;

	float					fSpd;
	u16						rotate;

} GMS_ENE_T_STAR_WORK;
//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------
// 現在gmEneKaniPunchにある(問題なければgmEneComに移動する)
extern void			GmEneUtilInitNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work, OBS_OBJECT_WORK* obj_work, Sint32 max_node );
extern NNS_MATRIX*	GmEneUtilGetNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work, Sint32 node_id );
extern void			GmEneUtilExitNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work );
extern void			GmEneUtilSetMatrixNN	( OBS_OBJECT_WORK* obj_work, NNS_MATRIX*	w_mtx );

//----- Static Declarations -------------------------------------------------
static void			gmEneTStarWaitInit(OBS_OBJECT_WORK *obj_work);
static void			gmEneTStarWaitMain(OBS_OBJECT_WORK *obj_work);

static void			gmEneTStarWalkInit(OBS_OBJECT_WORK *obj_work);
static void			gmEneTStarWalkMain(OBS_OBJECT_WORK *obj_work);

static void			gmEneTStarStopMain(OBS_OBJECT_WORK *obj_work);		// 9/30 ストップを追加

//static void		gmEneTStarFwInit(OBS_OBJECT_WORK *obj_work);
//static void		gmEneTStarFwMain(OBS_OBJECT_WORK *obj_work);
static void			gmEneTStarFlipInit(OBS_OBJECT_WORK *obj_work);
static void			gmEneTStarFlipMain(OBS_OBJECT_WORK *obj_work);

static void			gmEneTStarAttackInit(OBS_OBJECT_WORK *obj_work);
static void			gmEneTStarAttackMain(OBS_OBJECT_WORK *obj_work);


static void			gmEneTStarNeedleMain(OBS_OBJECT_WORK *obj_work);

static BOOL			gmEneTStarSetWalkSpeed(GMS_ENE_T_STAR_WORK *t_star_work);
static void			gmEneTStarExit(MTS_TASK_TCB *tcb);


//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_t_star_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneTStarBuild
/*!
 *	エネミー テルスター データ構築
 */
// ==========================================================================
void GmEneTStarBuild(void)
{
	gm_ene_t_star_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_T_STAR_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_T_STAR_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneTStarFlush
/*!
 *	エネミー テルスター データ片付け
 */
// ==========================================================================
void GmEneTStarFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_T_STAR_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_t_star_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneTStarInit
/*!
 *	エネミー テルスター 初期化関数
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
OBS_OBJECT_WORK* GmEneTStarInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_T_STAR_WORK	*t_star_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_T_STAR_WORK), "ENE_T_STAR");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	t_star_work = (GMS_ENE_T_STAR_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_t_star_obj_3d_list[IDB_ENE_T_STAR_MDL_ENE_T_STAR_M_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_T_STAR_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);


	//マテリアルモーション
	OBS_DATA_WORK* data_mat_motion = ObjDataGet(GMD_DWORK_NO_ENEMY_T_STAR_MAT);
	amAssert( data_mat_motion );
	ObjObjectAction3dNNMaterialMotionLoad( 
			obj_work,
			0,
			data_mat_motion,
			NULL,
			0,
			NULL
	);
	//ObjDraw3DNNModelMaterialMotion

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

	// 優先設定
	// A面の前に設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;	//GMD_OBJ_DEFAULT_POS_Z_N_FRONT;	/*GMD_OBJ_ENEMY_POS_Z;*/
//	obj_work->pos.z = GMD_OBJ_ENEMY_POS_Z;

	//obj_work->dir.y = AKM_DEGtoA16(90);
	// 矩形設定
	// 対プレイヤー
	// 攻撃
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -16, -16, 16, 16);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	// くらい
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -10, -10, 10, 10);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// ボディ
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work, -20, -20, 20, 20);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// 地形あたり
//	ObjObjectFieldRectSet(obj_work, -4, -8, 4, 16/*-2*/);

	// テルスターは常に前を向くようにしておく
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	//obj_work->move_flag |= OBD_MOVE_FALL;
	// 重力なし
	obj_work->move_flag &= ~OBD_MOVE_FALL;
	obj_work->move_flag |= OBD_MOVE_NOCOL;	// 地形あたり無し
/*
	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_T_STAR_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}
*/
	
	if ((eve_rec->flag & (GMD_ENE_T_STAR_EVE_FLAG_4_8|GMD_ENE_T_STAR_EVE_FLAG_2_8|GMD_ENE_T_STAR_EVE_FLAG_1_8)) == 0){
		// フラグが設定されていない場合は通常のスピードにする
		t_star_work->fSpd = 1.0f;	
	}else{
		// フラグ付き
		t_star_work->fSpd = 0.0f;
		if ((eve_rec->flag & GMD_ENE_T_STAR_EVE_FLAG_4_8)) {
			t_star_work->fSpd += 0.5f;
		}
		if ((eve_rec->flag & GMD_ENE_T_STAR_EVE_FLAG_2_8)) {
			t_star_work->fSpd += 0.25f;
		}
		if ((eve_rec->flag & GMD_ENE_T_STAR_EVE_FLAG_1_8)) {
			t_star_work->fSpd += 0.125f;
		}
	}


	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	/*
	// 減速処理設定
	t_star_work->spd_dec		 = GMD_ENE_T_STAR_MOVE_SPD_X / (GMD_ENE_T_STAR_TURN_FRAME/2);
	t_star_work->spd_dec_dist = GMD_ENE_T_STAR_MOVE_SPD_X * (GMD_ENE_T_STAR_TURN_FRAME/2) / 2;
	*/

	gmEneTStarWaitInit(obj_work);


	// ノードシステムを使用する
	GmEneUtilInitNodeMatrix( &t_star_work->node_work, obj_work, GMD_ENE_T_STAR_NODE_MAX );
	// ノードシステムを使用するため開放処理を変える(できればGmEneUtilInitNodeMatrixで設定したいが…workの形が個別になるため厳しい)
	mtTaskChangeTcbDestructor(obj_work->tcb, gmEneTStarExit);

	// 初期登録
	GmEneUtilGetNodeMatrix( &t_star_work->node_work, GMD_ENE_T_STAR_NODE_0 );
	GmEneUtilGetNodeMatrix( &t_star_work->node_work, GMD_ENE_T_STAR_NODE_1 );
	GmEneUtilGetNodeMatrix( &t_star_work->node_work, GMD_ENE_T_STAR_NODE_2 );
	GmEneUtilGetNodeMatrix( &t_star_work->node_work, GMD_ENE_T_STAR_NODE_3 );
	GmEneUtilGetNodeMatrix( &t_star_work->node_work, GMD_ENE_T_STAR_NODE_4 );

	// ホーミングできないようにする
	GMS_ENEMY_3D_WORK*	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

	// テルスター本体のサイズを変更できるようにしておく
	obj_work->scale.x = FX_F32_TO_FX32( GME_ENE_T_START_SIZE );
	obj_work->scale.y = FX_F32_TO_FX32( GME_ENE_T_START_SIZE );
	obj_work->scale.z = FX_F32_TO_FX32( GME_ENE_T_START_SIZE );

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}




// ==========================================================================
// GmEneTStarNeedleInit
/*!
 *	エネミー テルスター(とげ) 初期化関数
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
OBS_OBJECT_WORK* GmEneTStarNeedleInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_T_STAR_WORK	*t_star_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_T_STAR_WORK), "ENE_T_STAR");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	t_star_work = (GMS_ENE_T_STAR_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_t_star_obj_3d_list[IDB_ENE_T_STAR_MDL_ENE_T_STAR_N_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_T_STAR_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);

	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

	// 優先設定
	// A面の前に設定
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;	//GMD_OBJ_DEFAULT_POS_Z_N_FRONT;	/*GMD_OBJ_ENEMY_POS_Z;*/

	//obj_work->dir.y = AKM_DEGtoA16(90);
	// 矩形設定
	// 対プレイヤー
	// 攻撃
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -11, -12, 11, 12);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag |= OBD_RECT_ENABLE;

	ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// ボディ
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, /*-32*/-16, 19, 16);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// 地形あたり
//	ObjObjectFieldRectSet(obj_work, -4, -8, 4, 16/*-2*/);

	// テルスターは常に前を向くようにしておく
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	//obj_work->move_flag |= OBD_MOVE_FALL;
	// 重力なし
	obj_work->move_flag &= ~OBD_MOVE_FALL;
	obj_work->move_flag |= OBD_MOVE_NOCOL;	// 地形あたり無し

	/*
	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_T_STAR_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}
	*/

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	/*
	// 減速処理設定
	t_star_work->spd_dec		 = GMD_ENE_T_STAR_MOVE_SPD_X / (GMD_ENE_T_STAR_TURN_FRAME/2);
	t_star_work->spd_dec_dist = GMD_ENE_T_STAR_MOVE_SPD_X * (GMD_ENE_T_STAR_TURN_FRAME/2) / 2;
	*/

	obj_work->ppFunc = gmEneTStarNeedleMain;
	//gmEneTStarNeedleMain(obj_work);

	// テルスターニードル本体のサイズを変更できるようにしておく
	obj_work->scale.x = FX_F32_TO_FX32( GME_ENE_T_START_SIZE );
	obj_work->scale.y = FX_F32_TO_FX32( GME_ENE_T_START_SIZE );
	obj_work->scale.z = FX_F32_TO_FX32( GME_ENE_T_START_SIZE );

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// =======================================================================
// gmEneTStarExit
/*!
  本体終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void gmEneTStarExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK*		obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_ENE_T_STAR_WORK*	t_star_work	= (GMS_ENE_T_STAR_WORK*)obj_work;
	
	GmEneUtilExitNodeMatrix( &t_star_work->node_work );
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}

// ==========================================================================
// gmEneTStarGetLength2N()
/*!
 *	プレイヤーとの距離の自乗を求める
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
Sint32	gmEneTStarGetLength2N( OBS_OBJECT_WORK* obj_work )
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
// gmEneTStarGetPlayerVector()
/*!
 *	プレイヤーの方向を返す(正規化ずみベクトル)
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
VecFx32	gmEneTStarGetPlayerVectorFx( OBS_OBJECT_WORK* obj_work )
{
	VecFx32				normal;

	GMS_PLAYER_WORK		*ply_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	fx32	px = ply_work->obj_work.pos.x - obj_work->pos.x;
	fx32	py = ply_work->obj_work.pos.y - obj_work->pos.y;

	// 20091021 Dimps Ishizaki
	if (ply_work->player_flag & GMD_PLF_DIE) {
		// プレイヤー死亡時は一番遠い場所にする
		px = 0x2D4000;
		py = 0x2D4000;
	}

	fx32	len = FX_Mul( px, px ) + FX_Mul( py, py );

	len = FX_Sqrt(len);

	if ( len == 0 ){
		// 長さが無い
		normal.x = 0;
		normal.y = 0;
	}
	else{
		fx32 r_length = FX_Div( FX32_ONE, len );
		normal.x = FX_Mul( px, r_length );
		normal.y = FX_Mul( py, r_length );
	}
	normal.z = 0;

	return normal;
}


// ==========================================================================
//	gmEneTStarWaitInit
/*!
 *	エネミー テルスター 待ち処理Init
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	// TODO マテリアルモーションになる
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_MOVE_ZNM, IDB_ENE_T_STAR_MTN_ENE_T_STAR_MOVE_ZNM);

	//ObjDrawObjectActionSet(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneTStarWaitMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	obj_work->spd.x = 0;
	obj_work->spd.y = 0;

}

// ==========================================================================
//	gmEneTStarWaitMain
/*!
 *	エネミー テルスター 待ちメイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarWaitMain(OBS_OBJECT_WORK *obj_work)
{

	// プレイヤーが近くにいる
	if (gmEneTStarGetLength2N( obj_work ) < (GMD_ENE_T_STAR_SEARCH_LENGTH * GMD_ENE_T_STAR_SEARCH_LENGTH) ){
		// 少し寄る
		obj_work->ppFunc = gmEneTStarWalkInit;
	}

}


// ==========================================================================
// gmEneTStarWalkInit
/*!
 *	エネミー テルスター Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	GMS_ENE_T_STAR_WORK*	t_star_work	= (GMS_ENE_T_STAR_WORK*)obj_work;

	// アクション設定
	// TODO マテリアルモーションになる
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_MOVE_ZNM, IDB_ENE_T_STAR_MTN_ENE_T_STAR_MOVE_ZNM);

	//ObjDrawObjectActionSet(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneTStarWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;


	// プレイヤーの方向を取得
	VecFx32	vec = gmEneTStarGetPlayerVectorFx( obj_work );

	obj_work->spd.x = (fx32)(vec.x * GMD_ENE_T_STAR_MOVE_SPD * t_star_work->fSpd );
	obj_work->spd.y = (fx32)(vec.y * GMD_ENE_T_STAR_MOVE_SPD * t_star_work->fSpd );

	// 移動時間を設定
	t_star_work->timer = GME_ENE_T_STAR_WALK_TIME;


	//マテリアル変更
	ObjDrawObjectActionSet3DNNMaterial( obj_work, IDB_ENE_T_STAR_MAT_ENE_T_STAR_M_EYE_ZNV );

	t_star_work->rotate = FALSE;

	/*
	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_T_STAR_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_T_STAR_MOVE_SPD_X;
	}
	*/
}

// ==========================================================================
// gmEneTStarWalkMain
/*!
 *	エネミー テルスター Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarWalkMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_T_STAR_WORK*	t_star_work	= (GMS_ENE_T_STAR_WORK*)obj_work;

	obj_work->disp_flag |= OBD_DISP_REPEAT;
//	ObjDrawAction3DNNMaterialUpdate( obj_work->obj_3d, &obj_work->disp_flag );

	if (t_star_work->rotate>0){
		obj_work->dir.z += AKM_DEGtoA16(10);
		t_star_work->rotate--;
		if (t_star_work->rotate==0){
			obj_work->dir.z = 0;
		}
	}

	if (t_star_work->timer>0){
		t_star_work->timer--;

		if (t_star_work->timer==GME_ENE_T_STAR_WALK_TIME/2){
			// 回転する
			t_star_work->rotate = 360 / 10;
		}
		return;
	}

	obj_work->spd.x = 0;
	obj_work->spd.y = 0;

	t_star_work->timer = GME_ENE_T_STAR_STOP_TIME;
	// ストップへ
	obj_work->ppFunc = gmEneTStarStopMain;

}

// ==========================================================================
// gmEneTStarStopMain
/*!
 *	エネミー テルスター Stop メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarStopMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_T_STAR_WORK*	t_star_work	= (GMS_ENE_T_STAR_WORK*)obj_work;

	obj_work->disp_flag |= OBD_DISP_REPEAT;

	if (t_star_work->timer>0){
		t_star_work->timer--;
		return;
	}

	// 爆発へ
	obj_work->ppFunc = gmEneTStarAttackInit;

	GmEfctEneEsCreate( obj_work, GME_EFCT_ENE_IDX_E13_T_STAR );

}


// ==========================================================================
//	gmEneTStarAttackInit
/*!
 *	エネミー テルスター 攻撃処理Init
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarAttackInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_T_STAR_WORK*	t_star_work	= (GMS_ENE_T_STAR_WORK*)obj_work;
//	GMS_ENEMY_3D_WORK*		ene_work;
//	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクションがないとマトリクスが取れない。
	// タスクごと消えるため、Z回転(72度づつで5つ生む)

	NNS_MATRIX	mat;
	NNS_MATRIX	rot;

	nnMakeUnitMatrix( &mat );

	nnMakeUnitMatrix( &rot );
	nnMakeRotateZMatrix( &rot, AKM_DEGtoA32( GMD_ENE_T_STAR_NEEDLE_ROT_DEG ) );


	OBS_OBJECT_WORK* obj_atk;

	for(int i=0;i<5;i++){
		// 弾を生む

		obj_atk		= GmEventMgrLocalEventBirth(GMD_EVENT_ID_T_STAR_NEEDLE,
												obj_work->pos.x, obj_work->pos.y,
												0,//flag
												0,0,0,0,
												0);


		obj_atk->parent_obj = obj_work;
		obj_atk->dir.y = 0xC000;
		obj_atk->dir.z = AKM_DEGtoA16(-GMD_ENE_T_STAR_NEEDLE_ROT_DEG*i);
		/*
		NNS_MATRIX*	mtx	= GmEneUtilGetNodeMatrix( &t_star_work->node_work, GMD_ENE_T_STAR_NODE_0+i );

		NNM_MTX( *mtx, 0, 3 ) = NNM_MTX( *mtx, 0, 3 ) - FX_FX32_TO_F32( obj_work->pos.x );
		NNM_MTX( *mtx, 1, 3 ) = NNM_MTX( *mtx, 1, 3 ) - FX_FX32_TO_F32( -obj_work->pos.y );
		NNM_MTX( *mtx, 2, 3 ) = NNM_MTX( *mtx, 2, 3 ) - FX_FX32_TO_F32( obj_work->pos.z );

		GmEneUtilSetMatrixNN( obj_atk, mtx );

		GmEneUtilSetMatrixNN( obj_atk, &mat );

		// ローカルY軸の(X,Y)要素を傾きに使う
		NNS_VECTOR	v;
		v.x = NNM_MTX( *mtx, 0, 1 );
		v.y = NNM_MTX( *mtx, 1, 1 );
		v.z = 0;

		nnNormalizeVector(&v, &v);
		*/

//		GmEneUtilSetMatrixNN( obj_atk, &mat );

		// ローカルY軸の(X,Y)要素を傾きに使う
		NNS_VECTOR	v;
		v.x = NNM_MTX( mat, 0, 1 );
		v.y = NNM_MTX( mat, 1, 1 );
		v.z = 0;

		obj_atk->spd.x = FX_F32_TO_FX32( v.x * GMD_ENE_T_STAR_NEEDLE_SPD );
		obj_atk->spd.y = -FX_F32_TO_FX32( v.y * GMD_ENE_T_STAR_NEEDLE_SPD );

		obj_atk->pos.x += FX_F32_TO_FX32( v.x * GMD_ENE_T_STAR_NEEDLE_RADIUS );
		obj_atk->pos.y += -FX_F32_TO_FX32( v.y * GMD_ENE_T_STAR_NEEDLE_RADIUS );

		// 72度足しこむ
		nnMultiplyMatrix( &mat, &mat, &rot );

		// ホーミングできないようにする
		GMS_ENEMY_3D_WORK*	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_atk;
		ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

#if 01
		// エフェクト追加
		GMS_EFFECT_3DES_WORK* eff_work = GmEfctEneEsCreate( obj_atk, GME_EFCT_ENE_IDX_E13_CROW );

		// 仮で親につくように変更
//		eff_work->efct_com.obj_work.ppFunc = NULL;
//		GmEffect3DESSetupBase( eff_work, GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND, GMD_EFFECT_3DES_FLAG_STICKPARENT );

		// 表示角度設定
		eff_work->efct_com.obj_work.dir.z =AKM_DEGtoA16(-GMD_ENE_T_STAR_NEEDLE_ROT_DEG*i);
/*
		GmEffect3DESSetDispRotation( eff_work,	AKM_DEGtoA16( 0 ),
												AKM_DEGtoA16( 0 ),
												AKM_DEGtoA16(-GMD_ENE_T_STAR_NEEDLE_ROT_DEG*i) );
*/
#endif
		//obj_atk->dir.z = AKM_DEGtoA16(-GMD_ENE_T_STAR_NEEDLE_ROT_DEG*i);
	}

	obj_work->disp_flag |= OBD_DISP_NODISP;


	// アクション設定
	// TODO マテリアルモーションになる
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_MOVE_ZNM, IDB_ENE_T_STAR_MTN_ENE_T_STAR_MOVE_ZNM);

	//ObjDrawObjectActionSet(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneTStarAttackMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	obj_work->spd.x = 0;
	obj_work->spd.y = 0;

	// 弾が飛ぶ時間
	t_star_work->timer = GMD_ENE_T_STAR_NEEDLE_LIFT_TIME;

	// 当たりを消す
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	ene_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
	ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
	ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// 爆発SE
	GmSoundPlaySE( GMD_ENE_T_STAR_SE_BOMB );

	// 復活しないように死亡したことにする
	ene_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_DIE;


}

// ==========================================================================
//	gmEneTStarAttackMain
/*!
 *	エネミー テルスター 待ちメイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarAttackMain(OBS_OBJECT_WORK *obj_work)
{
	// 本体は GMD_ENE_T_STAR_NEEDLE_LIFT_TIME でその時間生き、その間だけ弾を生かしておく
	GMS_ENE_T_STAR_WORK*	t_star_work	= (GMS_ENE_T_STAR_WORK*)obj_work;

	if (t_star_work->timer>0){
		t_star_work->timer--;
		return;
	}
	
	// いなくなる
	obj_work->flag	|= OBD_OBJECT_TASKCLEAR_REQUEST;
}

// ==========================================================================
//	gmEneTStarNeedleMain
/*!
 *	エネミー テルスター 待ちメイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarNeedleMain(OBS_OBJECT_WORK *obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);
}


#if 0
// ==========================================================================
// gmEneTStarFwInit
/*!
 *	エネミー テルスター FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_FW_ZNM, IDB_ENE_T_STAR_MTN_ENE_T_STAR_FW_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_FW_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneTStarFwMain;

	// 移動速度設定
	obj_work->spd.x = 0;

	// 演出時間
	obj_work->user_timer = GMD_ENE_T_STAR_FW_TIME;
}
#endif

// ==========================================================================
// gmEneTStarFwMain
/*!
 *	エネミー テルスター FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarFwMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	if (obj_work->user_timer <= 0) {
		// フリップへ移行
		gmEneTStarFlipInit(obj_work);
	}
}



// ==========================================================================
// gmEneTStarFlipInit
/*!
 *	エネミー テルスター フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_FRIP_ZNM, IDB_ENE_T_STAR_MTN_ENE_T_STAR_FRIP_L_ZNM);

	// TODO マテリアルモーション
//	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_FRIP_ZNM, IDB_ENE_T_STAR_MTN_ENE_T_STAR_FRIP_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_T_STAR_MTN_ENE_T_STAR_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneTStarFlipMain;

	// 移動速度設定
	//obj_work->spd.x = 0;
}

// ==========================================================================
// gmEneTStarFlipMain
/*!
 *	エネミー テルスター フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneTStarFlipMain(OBS_OBJECT_WORK *obj_work)
{
	// 移動速度設定
	gmEneTStarSetWalkSpeed((GMS_ENE_T_STAR_WORK*)obj_work);

	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
		// Walkへ
		gmEneTStarWalkInit(obj_work);
	}
}


// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneTStarSetWalkSpeed
/*!
 *	エネミー テルスター 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneTStarSetWalkSpeed(GMS_ENE_T_STAR_WORK *t_star_work)
{
	BOOL			b_dec = FALSE;

	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)t_star_work;

	// TODO 左右移動
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// 左向き
/*
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_T_STAR_MTN_ENE_T_STAR_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_T_STAR_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, t_star_work->spd_dec, GMD_ENE_T_STAR_MOVE_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + t_star_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, t_star_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -t_star_work->spd_dec) {
					obj_work->spd.x = -t_star_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_T_STAR_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -t_star_work->spd_dec, GMD_ENE_T_STAR_MOVE_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_T_STAR_MTN_ENE_T_STAR_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_T_STAR_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -t_star_work->spd_dec, GMD_ENE_T_STAR_MOVE_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - t_star_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, t_star_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > t_star_work->spd_dec) {
					obj_work->spd.x = t_star_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_T_STAR_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, t_star_work->spd_dec, GMD_ENE_T_STAR_MOVE_SPD_X);
		}
*/
	}

	return (b_dec);
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
