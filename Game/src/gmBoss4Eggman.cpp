// =======================================================================
/*!
  @file	gmBoss4Eggman.cpp
  @brief ボス4 エッグマン

  @author K.Yurita(SafariGames)
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Eggman.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "gs.h"
#include "gmMain.h"
#include "gmGameDBuild.h"
#include "gmGamedat.h"
#include "gmEventTbl.h"
#include "gmCamera.h"
#include "gmSound.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectBoss.h"
#include "gmBossCommon.h"
#include "gmBoss4.h"
#include "gmBoss4Body.h"
#include "gmBoss4Eggman.h"
#include "gmBoss4Effect.h"

#include "gmPlySeq.h"

#include "gmGmkCamScrLim.h"

/*------ Macros --------------------------------------------------------*/

// 作成マクロ (子供がこのタスクのノード取得が必要になるため、PRIOを変える必要がある)
#if defined (MTD_DEBUG)
#define GMM_BOSS4_EGG_CREATE_WORK(eve_rec, pos_x, pos_y, work_size, name)	\
	(GmEnemyCreateWork(eve_rec, pos_x, pos_y, work_size, GMD_TASK_PRIO_ENEMY+10, name))
#else// (MTD_DEBUG)
#define GMM_BOSS4_EGG_CREATE_WORK(eve_rec, pos_x, pos_y, work_size, name)	\
	(GmEnemyCreateWork(eve_rec, pos_x, pos_y, work_size, GMD_TASK_PRIO_ENEMY+10))
#endif// (MTD_DEBUG)

//############ 汗エフェクト ###############################################
/* 定義値 */
#define GMD_BOSS4_EFF_SWEAT_DISP_OFST_Y				((Float)32.f)			//!< 表示オフセットY


/*------ Macro Functions -----------------------------------------------*/
// =======================================================================
// GMM_BOSS4_MGR
/*!
  ボス1本体ワークから管理ワークを取り出す
  
  @param work	[in]	ボス１本体ワーク
  
  @return 管理ワーク(GMS_BOSS4_MGR_WORK)
 */
// =======================================================================
#define GMM_BOSS4_MGR(work)	((work)->mgr_work)

// =======================================================================

//! ボス１ エッグマン独立アクションIDテーブル
const static GMS_BOSS4_PART_ACT_INFO gm_boss4_egg_act_id_tbl[GME_BOSS4_EGG_ACT_ID_MAX]	= {
	// ACT_ID										IS_MAINTAIN		IS_REPEAT	MTN_SPD	IS_BLEND	BLEND_SPD
	// GME_BOSS4_EGG_ACT_ID_LAUGH_R
	{IDB_BOSS04_EGG_MTN_B04_1_STA_01E_ZNM,		FALSE,			FALSE,		1.0f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},
	// GME_BOSS4_EGG_ACT_ID_LAUGH_L
	{IDB_BOSS04_EGG_MTN_B04_1_STA_02E_ZNM,		FALSE,			FALSE,		1.0f,	TRUE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},
	// GME_BOSS4_EGG_ACT_ID_DAMAGE
	{IDB_BOSS04_EGG_MTN_B04_DMG01_01E_ZNM,		FALSE,			FALSE,		1.0f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},
	// GME_BOSS4_EGG_ACT_ID_THROW
	{IDB_BOSS04_EGG_MTN_B04_2_ATT02_01E_ZNM,	FALSE,			FALSE,		1.0f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},
	// GME_BOSS4_EGG_ACT_ID_THROW_LEFT
	{IDB_BOSS04_EGG_MTN_B04_2_ATT03_01E_ZNM,	FALSE,			FALSE,		1.0f,	FALSE,		GMD_BOSS4_DEFAULT_BLEND_SPD,	FALSE},
};

/*------ Static Declarations -------------------------------------------*/
//############ ボス１エッグマン ###############################################
/* 補助関数 */
static void gmBoss4EggSetActionIndependent(GMS_BOSS4_EGG_WORK *egg_work,
										   GME_BOSS4_EGG_ACT_ID act_id, BOOL force_change=FALSE);
static void gmBoss4EggRevertActionIndependent(GMS_BOSS4_EGG_WORK *egg_work);
/* 制御処理 */
static void gmBoss4EggWaitLoad(OBS_OBJECT_WORK *obj_work);
static void gmBoss4EggMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
// 通常時停滞シーケンス
static void gmBoss4EggProcIdleInit(GMS_BOSS4_EGG_WORK *egg_work);
static void gmBoss4EggProcIdleUpdateLoop(GMS_BOSS4_EGG_WORK *egg_work);
// 笑いシーケンス
static void gmBoss4EggProcLaughInit(GMS_BOSS4_EGG_WORK *egg_work);
static void gmBoss4EggProcLaughUpdateLoop(GMS_BOSS4_EGG_WORK *egg_work);
// ダメージシーケンス
static void gmBoss4EggProcDamageInit(GMS_BOSS4_EGG_WORK *egg_work);
static void gmBoss4EggProcDamageUpdateLoop(GMS_BOSS4_EGG_WORK *egg_work);
// 逃亡シーケンス
static void gmBoss4EggProcEscapeInit(GMS_BOSS4_EGG_WORK* egg_work);
static void gmBoss4EggProcEscapeUpdateLoop(GMS_BOSS4_EGG_WORK *egg_work);

static	void gmBoss4EggProcThrowInit(GMS_BOSS4_EGG_WORK *egg_work);
static	void gmBoss4EggProcThrowLeftInit(GMS_BOSS4_EGG_WORK *egg_work);
static	void gmBoss4EggProcThrowUpdateLoop(GMS_BOSS4_EGG_WORK *egg_work);

static	void gmBoss4EggExit(MTS_TASK_TCB *tcb);

#if defined(MTD_DEBUG)
//############ デバッグ ###############################################
#endif /* defined(MTD_DEBUG) */

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
static void gmBoss4EffSweatInit( GMS_BOSS4_EGG_WORK *egg_work);
static void gmBoss4EffSweatProcMain(OBS_OBJECT_WORK *obj_work);

/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// GmBoss4Build
/*!
  ボス4 データ構築
 */
// =======================================================================
void GmBoss4EggmanBuild(void)
{
	// モーション
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EGG_MTN),
						IDB_BOSS04_BOSS04_EGG_MTN_AMB, GMD_BOSS4_ARC);
}


// =======================================================================
// GmBoss4Flush
/*!
  ボス１ データ片付け
 */
// =======================================================================
void GmBoss4EggmanFlush(void)
{
	// モーション
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EGG_MTN));
}

#if 0
// =======================================================================
// GmBoss4EggInit
/*!
  ボス１ エッグマン 初期化
  
  @param    act_id   [in]    アクション番号
  
  @return	設定された=TRUE/ されなかった=FALSE
 */
// =======================================================================
BOOL	gmBoss4EggmanSetAction( Sint32 act_id )
{

	// エッグマンについては、独立アクション中ならばアクション変更しない
	if (i == GME_BOSS4_PART_IDX_EGG) {
		GMS_BOSS4_EGG_WORK	*egg_work	= (GMS_BOSS4_EGG_WORK*)body_work->parts_objs[i];
		
		// 戻るべきモーション番号を記録しておく
		body_work->egg_revert_mtn_id	= pt_act_info[i].act_id;
		
		if (egg_work->flag & GMD_BOSS4_EGG_FLAG_INDP_ACT_SET) {
			return FALSE;
		}

		GmBsCmnSetAction(body_work->parts_objs[i],
						 pt_act_info[i].act_id,
						 pt_act_info[i].is_repeat,
						 pt_act_info[i].is_blend);

	}

	return TRUE;
}
#endif

// =======================================================================
// GmBoss4EggInit
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4EggInit(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);//仮
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_3D_WORK	*ene_3d;
	GMS_BOSS4_EGG_WORK	*egg_work;
	
	// オブジェクト作成
	obj_work	= GMM_BOSS4_EGG_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS4_EGG_WORK),
										"Boss4_EGG");
	
	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	egg_work	= (GMS_BOSS4_EGG_WORK*)obj_work;

	// ホーミングは常に禁止(当たりはないがこれをしないとロックカーソルが出てしまう)
	ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

	// ワーク設定
	// TODO : 未実装
	
	// ライフ無し
	
	// 地形当たり無し
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	
	ObjObjectCopyAction3dNNModel(obj_work,
								 //&gm_boss4_obj_3d_list[IDB_BOSS01_MDL_EGGMAN_ZNO],
								 GmBoss4GetObj3D(IDB_BOSS04_MDL_EGGMAN_ZNO),
								 &ene_3d->obj_3d);
	
	// エッグマンモーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,// TODO : 仮 登録モーションID
								  TRUE,	// TODO : 仮 マージ
								  ObjDataGet(GMD_DWORK_NO_BOSS_04_EGG_MTN),
								  NULL,
								  0,
								  NULL);
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= 0.125f;// TODO : 仮
	
	// ユーザ描画ステートを使用
	obj_work->disp_flag	|= OBD_DISP_DRAWSTATE;
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss4EggWaitLoad;
	
	obj_work->flag	|= OBD_OBJECT_NOCLIP;	//仮
	obj_work->disp_flag	|= OBD_DISP_REPEAT;
	obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;

	// エッグマンの向きを標準化
	egg_work->dir_work.direction = GME_BOSS4_DIR_RIGHT;
	GmBoss4UtilInitTurnGently( &egg_work->dir_work, 0, 1, FALSE);// TODO : 仮
	GmBoss4UtilUpdateTurnGently( &egg_work->dir_work );

	// エッグマン終了処理変更
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss4EggExit);

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}


/*------ Static Functions ----------------------------------------------*/


// ############################################################################
// ボス1 エッグマン
// ############################################################################

// =======================================================================
// gmBoss4EggExit
/*!
  本体終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void gmBoss4EggExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_BOSS4_EGG_WORK	*egg_work	= (GMS_BOSS4_EGG_WORK*)obj_work;

	// オブジェクト生成数デクリメント
	GmBoss4DecObjCreateCount();

	GmBoss4UtilExitNodeMatrix( &egg_work->node_work );
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}

// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss4EggSetActionIndependent
/*!
  エッグマン 独立アクション設定
  
  @param egg_work	[io]	本体ワーク
  @param act_id		[in]	全体アクションインデックス(GME_BOSS4_ACT_ID_XXX)
  
  @note
  エッグマンの独立アクションを設定します。
  gmBoss4EggRevertActionIndependent()を呼ぶと全体アクションの設定に戻ります。
 */
// =======================================================================
void gmBoss4EggSetActionIndependent(GMS_BOSS4_EGG_WORK *egg_work,
									GME_BOSS4_EGG_ACT_ID act_id, BOOL force_change/*=FALSE*/)
{
	const GMS_BOSS4_PART_ACT_INFO	*pt_act_info	= &gm_boss4_egg_act_id_tbl[act_id];
	OBS_OBJECT_WORK	*obj_egg	= GMM_BS_OBJ(egg_work);
	GMS_BOSS4_BODY_WORK	*parent_body	= (GMS_BOSS4_BODY_WORK*)obj_egg->parent_obj;
	
	// エッグマン独立アクション禁止なら何もしない
	if (parent_body->flag & GMD_BOSS4_BODY_FLAG_INDP_ACT_FORBIDDEN) {
		return;
	}
	
	// 独立アクション設定中に指定アクション設定済みなら何もしない
	if (!force_change &&
		((egg_work->flag & GMD_BOSS4_EGG_FLAG_INDP_ACT_SET)&&
		 (egg_work->egg_act_id == act_id))) {
		return;
	}
	
	// アクションID設定
	egg_work->egg_act_id	= act_id;
	
	// 独立アクション設定中
	egg_work->flag	|= GMD_BOSS4_EGG_FLAG_INDP_ACT_SET;
	
	// 構成パーツのアクション設定を反映
	
	
	// 継続フラグがオフの時のみ、新たなアクションを設定
	if (FALSE == pt_act_info->is_maintain) {
		GmBsCmnSetAction(obj_egg,
						 pt_act_info->act_id,
						 pt_act_info->is_repeat,
						 pt_act_info->is_blend);
	}
	else if (pt_act_info->is_repeat) {
		// リピートフラグは継続フラグの有無に関わらず反映
		GMM_BS_OBJ(egg_work)->disp_flag	|= OBD_DISP_REPEAT;
	}
	
#if defined(MTD_DEBUG)	// とりあえずエッグマンではサポートしない
	// 手動マージチェック
	if (pt_act_info->is_blend) {
		if (pt_act_info->is_merge_manual) {
			MTM_ASSERT(!"gmBoss4.cpp::gmBoss4EggSetActionIndependent() manual merge not supported\n");
		}
	}
#endif /* defined(MTD_DEBUG) */
	
	// モーション速度設定
	obj_egg->obj_3d->speed[0]	= pt_act_info->mtn_spd;
	
	// ブレンド速度設定
	obj_egg->obj_3d->blend_spd	= pt_act_info->blend_spd;
}



// =======================================================================
// gmBoss4EggRevertActionIndependent
/*!
  エッグマン 独立アクションから復帰
  
  @param egg_work	[io]	本体ワーク
  
  @note
  エッグマンの独立アクションから、本来のアクションに設定を戻します。
 */
// =======================================================================
void gmBoss4EggRevertActionIndependent(GMS_BOSS4_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_egg	= GMM_BS_OBJ(egg_work);
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)obj_egg->parent_obj;
	
	MTM_ASSERT(egg_work->flag & GMD_BOSS4_EGG_FLAG_INDP_ACT_SET);
	
	// 独立アクション解除
	egg_work->flag	&= ~GMD_BOSS4_EGG_FLAG_INDP_ACT_SET;
	
	// アクション設定
	GmBsCmnSetAction(obj_egg, GmBoss4GetActInfo( body_work->egg_revert_mtn_id, GME_BOSS4_PART_IDX_EGG)->act_id,
					GmBoss4GetActInfo( body_work->egg_revert_mtn_id, GME_BOSS4_PART_IDX_EGG)->is_repeat,
					//gm_boss4_act_id_tbl[body_work->whole_act_id][GME_BOSS4_PART_IDX_EGG].is_repeat,
					TRUE);
	
	// 本体の経過フレームに合わせる
	obj_egg->obj_3d->frame[0]	= GMM_BS_OBJ(body_work)->obj_3d->frame[0];
}


// ============================================================================
// 制御処理
// ============================================================================

// =======================================================================
// gmBoss4EggWaitLoad
/*!
  エッグマン ロード完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4EggWaitLoad(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK	*parent_body	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS4_EGG_WORK	*egg_work	= (GMS_BOSS4_EGG_WORK*)obj_work;
	
//	if (GmBoss4IsBuilded()){
	if (GMM_BOSS4_MGR(parent_body)->flag & GMD_BOSS4_MGR_FLAG_LOAD_END) {
		// 更新関数設定
		obj_work->ppFunc	= gmBoss4EggMain;
		
		// 初期シーケンス設定
		gmBoss4EggProcIdleInit(egg_work);

		// ノードマトリクスシステム初期化
		GmBoss4UtilInitNodeMatrix( &egg_work->node_work, obj_work, GMD_BOSS4_EGG_NODE_SNM_NUM );
	}
}

// =======================================================================
// gmBoss4EggMain
/*!
  エッグマン メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4EggMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK*	body_work	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS4_EGG_WORK*		egg_work	= (GMS_BOSS4_EGG_WORK*)obj_work;
	OBS_OBJECT_WORK*		body_obj	= (OBS_OBJECT_WORK*)body_work;

	// エッグマン設置ノードにくっつける(ワープ対策つき)
	const NNS_MATRIX* mtx1 = GmBoss4UtilGetNodeMatrix( &body_work->node_work, GMD_BOSS4_BODY_NODE_IDX_EGG_CONNECT );	// エッグマン設置場所
	const NNS_MATRIX* mtx2 = GmBoss4UtilGetNodeMatrix( &body_work->node_work, GMD_BOSS4_BODY_NODE_IDX_BODY_POSTURE );	// 中心

	// 姿勢はmtx1を使ったまま、位置の差分をもらい、現在のポジションを足しこむようにする。

	NNS_MATRIX	mtx;

	nnCopyMatrix( &mtx, mtx1 );

	NNM_MTX( mtx, 0, 3 ) = NNM_MTX( *mtx1, 0, 3 ) - NNM_MTX( *mtx2, 0, 3 ) + (Float)body_obj->pos.x / FX32_ONE;
//	NNM_MTX( *mtx1, 1, 3 ) = NNM_MTX( *mtx1, 1, 3 ) - NNM_MTX( *mtx2, 1, 3 ) + (Float)body_obj->pos.y / FX32_ONE;
//	NNM_MTX( *mtx1, 2, 3 ) = NNM_MTX( *mtx1, 2, 3 ) - NNM_MTX( *mtx2, 2, 3 ) + (Float)body_obj->pos.z / FX32_ONE;

	GmBoss4UtilSetMatrixNN( obj_work, &mtx );

	// エッグマン設置ノードにくっつける
//	GmBsCmnUpdateObject3DNNStuckWithNode(obj_work,
//											&body_work->node_work.snm_work,
//											 body_work->node_work.work[ GMD_BOSS4_BODY_NODE_IDX_EGG_CONNECT],
//											 TRUE);

	// 角度反映
	GmBoss4UtilUpdateTurnGently( &egg_work->dir_work );
	GmBoss4UtilUpdateDirection(&egg_work->dir_work, obj_work);

	// 更新処理
	if (egg_work->proc_update) {
		egg_work->proc_update(egg_work);
	}
	
	// 逃亡演出開始チェック
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_ESCAPE) {
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_ESCAPE;
		gmBoss4EggProcEscapeInit(egg_work);
	}
	
	// 投げる演出開始チェック
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_THROW) {
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_THROW;
		gmBoss4EggProcThrowInit(egg_work);
	}
	// 投げる演出開始チェック
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_THROW_L) {
		body_work->flag	&= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_THROW_L;
		gmBoss4EggProcThrowLeftInit(egg_work);
	}

	// ダメージ演出開始チェック
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_DAMAGE) {
		body_work->flag &= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_DAMAGE;
		gmBoss4EggProcDamageInit(egg_work);
	}
	
	// 黒こげチェック
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_BURNT) {
		body_work->flag &= ~GMD_BOSS4_BODY_FLAG_SIGNAL_B2E_EGG_BURNT;
		// エッグマンを黒こげテクスチャに設定
		gmBoss4SetPartTextureBurnt(obj_work);
	}
	
	// 親アニメーションストップチェック
	if (GMM_BS_OBJ(body_work)->disp_flag & OBD_DISP_STOP) {
		obj_work->disp_flag	|= OBD_DISP_STOP;
	}
	else {
		obj_work->disp_flag	&= ~OBD_DISP_STOP;
	}
}


// ============================================================================
// シーケンス処理
// ============================================================================

// =======================================================================
// gmBoss4EggProcIdle***
/*!
  通常時停滞シーケンス処理関数
 */
// =======================================================================
// 通常時停滞シーケンス 初期化
void gmBoss4EggProcIdleInit(GMS_BOSS4_EGG_WORK *egg_work)
{
	// 処理関数設定
	egg_work->proc_update	= gmBoss4EggProcIdleUpdateLoop;
}

// 通常時停滞シーケンス 更新
void gmBoss4EggProcIdleUpdateLoop(GMS_BOSS4_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_egg	= GMM_BS_OBJ(egg_work);
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)obj_egg->parent_obj;
		
	// ヒット通知確認
	if (body_work->flag & GMD_BOSS4_BODY_FLAG_SIGNAL_C2E_ATK_HIT) {
		body_work->flag &= ~GMD_BOSS4_BODY_FLAG_SIGNAL_C2E_ATK_HIT;
		
		// 笑いシーケンス初期化
		gmBoss4EggProcLaughInit(egg_work);
	}
}

// =======================================================================
// gmBoss4EggProcLaugh***
/*!
  笑いシーケンス処理関数
 */
// =======================================================================
// 笑いシーケンス初期化
void gmBoss4EggProcLaughInit(GMS_BOSS4_EGG_WORK *egg_work)
{
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)((OBS_OBJECT_WORK*)egg_work)->parent_obj;

	// エッグマン独立アクション設定
	if (body_work->dir.direction == GME_BOSS4_DIR_RIGHT){
		gmBoss4EggSetActionIndependent(egg_work, GME_BOSS4_EGG_ACT_ID_LAUGH_R);
	}else{
		gmBoss4EggSetActionIndependent(egg_work, GME_BOSS4_EGG_ACT_ID_LAUGH_L);
	}
	
	// 処理関数設定
	egg_work->proc_update	= gmBoss4EggProcLaughUpdateLoop;
}

// 笑いシーケンス更新
void gmBoss4EggProcLaughUpdateLoop(GMS_BOSS4_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		// アクションを元に戻す
		gmBoss4EggRevertActionIndependent(egg_work);
		
		// 通常停滞に戻る
		gmBoss4EggProcIdleInit(egg_work);
	}
}

// =======================================================================
// gmBoss4EggProcThrow***
/*!
  投げるシーケンス処理関数
 */
// =======================================================================
// シーケンス初期化
void gmBoss4EggProcThrowInit(GMS_BOSS4_EGG_WORK *egg_work)
{
	// エッグマン独立アクション設定
	gmBoss4EggSetActionIndependent(egg_work, GME_BOSS4_EGG_ACT_ID_THROW);
	
	// 処理関数設定
	egg_work->proc_update	= gmBoss4EggProcThrowUpdateLoop;
}

void gmBoss4EggProcThrowLeftInit(GMS_BOSS4_EGG_WORK *egg_work)
{
	// エッグマン独立アクション設定
	gmBoss4EggSetActionIndependent(egg_work, GME_BOSS4_EGG_ACT_ID_THROW_LEFT);
	
	// 処理関数設定
	egg_work->proc_update	= gmBoss4EggProcThrowUpdateLoop;
}


// 投げる共通シーケンス更新
void gmBoss4EggProcThrowUpdateLoop(GMS_BOSS4_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		// アクションを元に戻す
		gmBoss4EggRevertActionIndependent(egg_work);
		
		// 通常停滞に戻る
		gmBoss4EggProcIdleInit(egg_work);
	}
}


// =======================================================================
// gmBoss4EggProcDamage***
/*!
  ダメージシーケンス処理関数
 */
// =======================================================================
// ダメージシーケンス初期化
void gmBoss4EggProcDamageInit(GMS_BOSS4_EGG_WORK *egg_work)
{
	// エッグマン独立アクション設定
	gmBoss4EggSetActionIndependent(egg_work, GME_BOSS4_EGG_ACT_ID_DAMAGE);
	
	// 汗エフェクト生成
	gmBoss4EffSweatInit(egg_work);
	
	// 処理関数設定
	egg_work->proc_update	= gmBoss4EggProcDamageUpdateLoop;
}

// ダメージシーケンス更新
void gmBoss4EggProcDamageUpdateLoop(GMS_BOSS4_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		
		// 汗終了
		egg_work->flag	&= ~GMD_BOSS4_EGG_FLAG_SWEAT_ACTIVE;
		
		// アクションを元に戻す
		gmBoss4EggRevertActionIndependent(egg_work);
		
		// 通常停滞に戻る
		gmBoss4EggProcIdleInit(egg_work);
	}
}

// =======================================================================
// gmBoss4EggProcDefeat***
/*!
  撃破シーケンス処理関数
 */
// =======================================================================
// 撃破シーケンス初期化
void gmBoss4EggProcEscapeInit(GMS_BOSS4_EGG_WORK* egg_work)
{
	if (!(egg_work->flag & GMD_BOSS4_EGG_FLAG_SWEAT_ACTIVE)) {
		// 汗エフェクト生成
		gmBoss4EffSweatInit(egg_work);
	}
	
	// 処理関数設定
	egg_work->proc_update	= gmBoss4EggProcEscapeUpdateLoop;
}

// 撃破シーケンス更新
void gmBoss4EggProcEscapeUpdateLoop(GMS_BOSS4_EGG_WORK *egg_work)
{
	UNREFERENCED_PARAMETER(egg_work);
}



// =======================================================================
// gmBoss4EffSweatInit
/*!
  汗エフェクト 初期化
  
  @param egg_work	[io]	エッグマンワーク
 */
// =======================================================================
void gmBoss4EffSweatInit( GMS_BOSS4_EGG_WORK *egg_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	efct_work	= GmEfctCmnEsCreate(GMM_BS_OBJ(egg_work), GME_EFCT_CMN_IDX_SWEAT);
	
	// 位置調整
	GmEffect3DESAddDispOffset(efct_work, 0, GMD_BOSS4_EFF_SWEAT_DISP_OFST_Y, 0);//仮
	
	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss4EffSweatProcMain;

	egg_work->flag	|= GMD_BOSS4_EGG_FLAG_SWEAT_ACTIVE;
}

// =======================================================================
// gmBoss4EffSweatProcMain
/*!
  汗エフェクト メイン更新処理関数
 */
// =======================================================================
void gmBoss4EffSweatProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_EGG_WORK	*parent_egg	= (GMS_BOSS4_EGG_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_egg);
	
	if (!(parent_egg->flag & GMD_BOSS4_EGG_FLAG_SWEAT_ACTIVE)) {
		ObjDrawKillAction3DES(obj_work);
	}
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================
