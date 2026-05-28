// =======================================================================
/*!
  @file	gmBoss4Eggman.cpp
  @brief ボス4 カプセル

  @author K.Yurita(SafariGames)
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Capsule.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmBoss4Capsule.h"
#include "gmBoss4Body.h"
#include "gmBoss4Chibi.h"

//#include "gmBoss4Body.h"
//#include "gmBoss4Eggman.h"
//#include "gmBoss4Effect.h"

#include "gmPlySeq.h"

#include "gmGmkCamScrLim.h"

#if	defined(_WII) && !defined(_PS3)
#pragma warning off (10178)			// 型宣言のプロトタイプのワーニングをOFFにする
									// ただしstatic宣言はつけれないため、問題がある場合は直接修正する
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
#endif	//_WII

/*------ Macros --------------------------------------------------------*/
/*-------------- フラグ GMS_BOSS4_CAP_WORK::flag -----------------------*/
#define		GMD_BOSS4_CAP_FLAG_INVINCIBLE			(1 << 2)				//!< 無敵状態（当たりはあるが、ライフは減らない）
#define		GMD_BOSS4_CAP_FLAG_DEAD					(1 << 3)				//!< 死亡フラグ（ライフが無い状態）
#define		GMD_BOSS4_CAP_FLAG_NOHIT				(1 << 6)				//!< 攻撃矩形オフ

#define		GMD_BOSS4_CAP_FLAG_SIGNAL_DAMAGE		(1 << 30)				//!< body->bodyダメージ通知フラグ
#define		GMD_BOSS4_CAP_FLAG_SIGNAL_DEFEAT		(1 << 31)				//!< 死亡演出開始通知フラグ
/*----------------------------------------------------------------------*/

#define		GMD_BOSS4_CAP_OFFSET_Y					(20.0f)										//!< 中心点のオフセット
#define		GMD_BOSS4_CAP_DAMAGE_FLASH_POS_Z		(GMD_OBJ_DEFAULT_POS_Z_C_FRONT)				//!< カプセルがダメージ点滅するときのZ位置
#define		GMD_BOSS4_CAP_EXPLOSION_POS_Z			(GMD_OBJ_DEFAULT_POS_Z_C_FRONT + FX32_ONE)	//!< カプセルが爆発するときのZ位置

#define		GMD_BOSS4_CAP_FALL_SPD_Y				(0.20f)					//!< 落下スピード
#define		GMD_BOSS4_CAP_FALL_ROTATE_SPD			AKM_DEGtoA16(3.0f)
#define		GMD_BOSS4_CAP_FALL_ANGLE				GMD_BOSS4_LEFTWARD_ANGLE

#define		GMD_BOSS4_CAP_TASK_KILL_TIME			(30)					//!< 爆発後、カプセルのタスクが消えるまでの時間

#define		GMD_BOSS4_CAP_THROW_SPD_X				(-4.0f)					//!< 第二段階カプセル投げるスピード(X)
#define		GMD_BOSS4_CAP_THROW_SPD_Y				(-1.0f)					//!< 第二段階カプセル投げるスピード(Y)

#define		GMD_BOSS4_CAP_BOUND_SPD_X				(0.0f)					//!< 第二段階バウンドしたときのスピード(X)
#define		GMD_BOSS4_CAP_BOUND_SPD_Y				(-1.0f)					//!< 第二段階バウンドしたときのスピード(Y)

/*------ Macro Functions -----------------------------------------------*/
#define	T_FUNC(func, w)			{void func(OBS_OBJECT_WORK*); w->ppFunc = func;}
//#define	T_FUNC_SUB(func, w)		{static void func(GMS_BOSS4_CAP_WORK*); w->proc_update = func;}
#define	SET_FLAG( f, w )		w->flag |= f
#define	RESET_FLAG( f, w )		w->flag &=~f
#define	IS_FLAG( f, w )			(w->flag & f)

// =======================================================================

/*------ Static Declarations -------------------------------------------*/
//############ ボス4 カプセル ###############################################

/* 制御処理 */
//static void gmBoss4CapsuleWaitLoad(OBS_OBJECT_WORK *obj_work);
//static void gmBoss4CapsuleMain(OBS_OBJECT_WORK *obj_work);
//static void gmBoss4CapsuleDamage(OBS_OBJECT_WORK *obj_work);

// 開放用として追加
static void gmBoss4CapsuleExit(MTS_TASK_TCB *tcb);


static	void gmBoss4CapsuleDamageDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);
static	void gmBoss4CapsuleAtkHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);

static	OBS_OBJECT_WORK* GmBoss4CapsuleInit(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, GME_BOSS4_CAP_TYPE_ID type);

#if defined(MTD_DEBUG)
//############ デバッグ ###############################################
#endif /* defined(MTD_DEBUG) */

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

static Angle32	_cap_rot_y = AKM_DEGtoA32( 0 );
static Angle32	_cap_rot_z = AKM_DEGtoA32( 0 );
static Angle32	_cap_rot_x = AKM_DEGtoA32( 0 );
static Sint32	_cap_rot_z_flag	= 0;
static Sint32	_cap_rot_x_flag	= 0;

static float	_cap_len		= GMD_BOSS4_CAP_ZOOM_SMALL;
static float	_cap_len_flag	= 0;
static float	_cap_len_time	= (float)GMD_BOSS4_CAP_ZOOM_TIME;

static Sint32	_cap_no	= 0;

static Sint32	_cap_inv_flag = 0;				// 無敵
static BOOL		_cap_inv_hit  = TRUE;				// 当たり自体の発生

static Sint32	_cap_kill_flag = 0;			// 全部爆破する。その際ちびエッグマンはでない
static Sint32	_cap_count = 0;				// 存在しているカプセルの数

/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// GmBoss4Build
/*!
  ボス4 データ構築
 */
// =======================================================================
void GmBoss4CapsuleBuild(void)
{
	// モーションはなし
	//	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_01_EGG_MTN),
	//						IDB_BOSS01_BOSS01_EGG_MTN_AMB, GMD_BOSS4_ARC);

	_cap_no	= 0;
	_cap_count = 0;

	_cap_inv_flag	= 0;
	_cap_inv_hit	= TRUE;
	_cap_kill_flag	= 0;

}


// =======================================================================
// GmBoss4Flush
/*!
  ボス１ データ片付け
 */
// =======================================================================
void GmBoss4CapsuleFlush(void)
{
	// モーションはなし
	//	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_01_EGG_MTN));
}


// =======================================================================
// GmBoss4CapsuleInit1st
/*!
  カプセル 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4CapsuleInit1st(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);//仮
	return GmBoss4CapsuleInit( eve_rec, pos_x, pos_y, GME_BOSS4_CAP_TYPE_ID_NORMAL );
}

// =======================================================================
// GmBoss4CapsuleInit2nd
/*!
  カプセル 初期化(二段階目)
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4CapsuleInit2nd(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);//仮
	return GmBoss4CapsuleInit( eve_rec, pos_x, pos_y, GME_BOSS4_CAP_TYPE_ID_BOUND );
}



// =======================================================================
// gmBoss4CapsuleSetInvincible
/*!
  カプセル　無敵設定
  
  @param count	[in]	無敵時間(フレーム)
 */
// =======================================================================
void	GmBoss4CapsuleSetInvincible( Sint32 count, BOOL hit )
{
	_cap_inv_flag	= count;
	_cap_inv_hit	= hit;
}

// =======================================================================
// gmBoss4CapsuleGetCount
/*!
  カプセル　現在存在している数
  
 */
// =======================================================================
Sint32	GmBoss4CapsuleGetCount()
{
	return _cap_count;
}

// =======================================================================
// gmBoss4CapsuleClear
/*!
  カプセル　現在存在している数を初期化(先頭で行う処理を入れる)
  
 */
// =======================================================================
void	GmBoss4CapsuleClear()
{
	_cap_count = 0;
}

// =======================================================================
// gmBoss4CapsuleExplosion
/*!
  全カプセルを爆破する
  
 */
// =======================================================================
void	GmBoss4CapsuleExplosion()
{
	_cap_kill_flag = 1;
}

// =======================================================================
// GmBoss4CapsuleInit
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4CapsuleInit(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, GME_BOSS4_CAP_TYPE_ID type)
{
	_cap_kill_flag	= 0;

//	UNREFERENCED_PARAMETER(type);//仮
	
	OBS_OBJECT_WORK*		obj_work;
	GMS_ENEMY_3D_WORK*		ene_3d;
	GMS_BOSS4_CAP_WORK*		cap_work;
	
	// オブジェクト作成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS4_CAP_WORK),
										"BOSS4_CAP");
	
	ene_3d	=	(GMS_ENEMY_3D_WORK* )obj_work;
	cap_work=	(GMS_BOSS4_CAP_WORK*)obj_work;

	// ユニークなIDを入れる
	cap_work->cap_no = _cap_no++ % GMD_BOSS4_CAP_MAX;	// 0から8が入る

	cap_work->type = type;
	// ワーク設定
	// TODO : 未実装
	
	// ライフ無し
	
	// 地形当たり無し
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	
	ObjObjectCopyAction3dNNModel(obj_work,
									GmBoss4GetObj3D( IDB_BOSS04_MDL_B04_CAPSULE_ZNO ),
									&ene_3d->obj_3d);

	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ユーザ描画ステートを使用
	obj_work->disp_flag	|= OBD_DISP_DRAWSTATE;
		
	obj_work->flag		|= OBD_OBJECT_NOCLIP;	//仮
//	obj_work->disp_flag	|= OBD_DISP_REPEAT;
	obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;
	
	// TODO : 食らい当たり設定     仮
	ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF], -14, -14-16, 14, 14-16);
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].ppDef	= gmBoss4CapsuleDamageDefFunc;

	// TODO : 仮
	ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK], -1, -1-16+8, 1, 1-16+8);
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].ppHit	= gmBoss4CapsuleAtkHitFunc;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_ENABLE;

	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// ホーミングは常に禁止
	ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

	// メイン処理設定
	T_FUNC( gmBoss4CapsuleWaitLoad, obj_work );
	//obj_work->ppFunc	= gmBoss4CapsuleWaitLoad;

	if (cap_work->chibi_type==GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON){
		// 表示を行わない
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}

	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss4CapsuleExit);

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}

// =======================================================================
// GmBoss4CapsuleInit
/*!
  ボス4 カプセル回転
  
  @param    spd    [in]    回転スピード
  
 */
// =======================================================================
void GmBoss4CapsuleUpdateRol( float spd )
{
	_cap_rot_y += AKM_DEGtoA32( spd );
	_cap_rot_y %= AKM_DEGtoA32( 360 );

	if (_cap_rot_z_flag){
		_cap_rot_z += AKM_DEGtoA32(GMD_BOSS4_CPA_ROTATE_Z_SPD);
		if (_cap_rot_z >= AKM_DEGtoA32(GMD_BOSS4_CPA_ROTATE_Z_DEG)){
			_cap_rot_z_flag = 0;
		}
	}else{
		_cap_rot_z -= AKM_DEGtoA32(GMD_BOSS4_CPA_ROTATE_Z_SPD);
		if (_cap_rot_z <= AKM_DEGtoA32(-GMD_BOSS4_CPA_ROTATE_Z_DEG)){
			_cap_rot_z_flag = 1;
		}
	}

	if (_cap_rot_x_flag){
		_cap_rot_x += AKM_DEGtoA32(GMD_BOSS4_CPA_ROTATE_X_SPD);
		if (_cap_rot_x >= AKM_DEGtoA32(GMD_BOSS4_CPA_ROTATE_X_DEG)){
			_cap_rot_x_flag = 0;
		}
	}else{
		_cap_rot_x -= AKM_DEGtoA32(GMD_BOSS4_CPA_ROTATE_X_SPD);
		if (_cap_rot_x <= AKM_DEGtoA32(-GMD_BOSS4_CPA_ROTATE_X_DEG)){
			_cap_rot_x_flag = 1;
		}
	}

	if ( _cap_len_time > 0 ){
		_cap_len_time--;
	}else{
		if (_cap_len_flag){
			_cap_len += 2.0f;
			if (_cap_len >= GMD_BOSS4_CAP_ZOOM_BIG){
				_cap_len_flag = 0;
			}
		}else{
			_cap_len -= 2.0f;
			if (_cap_len <= GMD_BOSS4_CAP_ZOOM_SMALL){
				_cap_len_flag = 1;
				_cap_len_time	= GMD_BOSS4_CAP_ZOOM_TIME + GMD_BOSS4_CAP_ZOOM_TIME_RAND * ((float)(rand() % 256)/256);
			}
		}
	}
	
	// 念のため異常値を解除(あたらなくなってしまうため)
	if (_cap_inv_flag>60*15){
		// 異常値		
		_cap_inv_flag = 0;
	}
	if (_cap_inv_flag>0){
		_cap_inv_flag--;
	}else{
		// マイナスの時(念のため)
		_cap_inv_flag=0;
	}
}

/*------ Static Functions ----------------------------------------------*/
// ############################################################################
// ボス4 カプセル
// ############################################################################

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
void gmBoss4CapsuleWaitLoad(OBS_OBJECT_WORK *obj_work)
{
//	GMS_BOSS4_BODY_WORK	*parent_body	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS4_CAP_WORK*	cap_work	=	(GMS_BOSS4_CAP_WORK*)obj_work;
//	GMS_ENEMY_3D_WORK*	ene_3d		=	(GMS_ENEMY_3D_WORK* )obj_work;

	if (GmBoss4IsBuilded()){
	//if (GMM_BOSS4_MGR(parent_body)->flag & GMD_BOSS4_MGR_FLAG_LOAD_END) {
		// 更新関数設定

		if (cap_work->type == GME_BOSS4_CAP_TYPE_ID_NORMAL){

			T_FUNC( gmBoss4CapsuleMain, obj_work );
			//obj_work->ppFunc	= gmBoss4CapMain;
		}else{

			obj_work->move_flag	&= ~OBD_MOVE_NOCOLFIELD;
			obj_work->move_flag	|= OBD_MOVE_FALL;

			ObjObjectFieldRectSet( obj_work, -20, -20-20, 20, 20-20 );

#if _IPHONE
			obj_work->dir.y = 0;
#else
			obj_work->dir.y	= (u16)GMD_BOSS4_CAP_FALL_ANGLE;	//(Uint16)_work->cur_angle
#endif // _IPHONE

			// ちびエッグマンを吐き出すタイプを最初に決定する
			cap_work->chibi_type = 
				gmBoss4ChibiGetAttackType( GmBoss4GetLife() );

			T_FUNC( gmBoss4CapsuleMain2nd, obj_work );


			// 鉄球をそのまま投げてみる
			if (cap_work->chibi_type==GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON){
				OBS_OBJECT_WORK *obj_chibi;

				obj_chibi = GmEventMgrLocalEventBirth( GMD_EVENT_ID_BOSS4_CHIBI_2_IRON,
														obj_work->pos.x, obj_work->pos.y,
														0,//flag
														0,0,0,0,
														0);
				obj_chibi->spd.x = FX_F32_TO_FX32( 2 );
				obj_chibi->spd.y = FX_F32_TO_FX32( -3 );

				GmBoss4IncObjCreateCount();

				obj_chibi->parent_obj = obj_work->parent_obj;
				GMM_BS_OBJ(cap_work)->flag	|= OBD_OBJECT_TASKCLEAR_REQUEST;
			}

		}
		
		// 初期シーケンス設定
		//gmBoss4CapProcInit(cap_work);

		// カプセル数
		_cap_count++;

		// ダメージ中として使用
		cap_work->wait = 0;
	}
}


// =======================================================================
// gmBoss4CapsuleExit
/*!
  本体終了処理
  
  @note
 */
// =======================================================================
void gmBoss4CapsuleExit(MTS_TASK_TCB *tcb)
{
//	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	
	// オブジェクト生成数デクリメント
	GmBoss4DecObjCreateCount();
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}

// =======================================================================
// gmBoss4EggMain
/*!
  カプセル メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4CapsuleMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS4_CAP_WORK	*cap_work	= (GMS_BOSS4_CAP_WORK* )obj_work;
	
	// マトリクステスト
	NNS_MATRIX rmat;
	NNS_MATRIX tmat;
	NNS_MATRIX mat;


	// ダメージ点滅更新
	if (cap_work->wait>0){

		obj_work->pos.z	= GMD_BOSS4_CAP_DAMAGE_FLASH_POS_Z;

		GmBoss4UtilUpdateFlicker(obj_work, &cap_work->flk_work);

		if (GmBoss4UtilUpdate1ShotTimer(&cap_work->timer)) {
			// 爆発へ

			// 爆発発生
			VecFx32	pos = obj_work->pos;
			// 前に出す
			pos.z	= GMD_BOSS4_CAP_EXPLOSION_POS_Z/*obj_work->pos.z + GMD_BOSS4_EFF_BOMB_OFST_Z*/;
			GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_CAP_EX_ES, &pos );

			T_FUNC( gmBoss4CapsuleBomb, obj_work );
		}
	}else{
		// エッグマン設置ノードにくっつける
		GmBsCmnUpdateObject3DNNStuckWithNode(obj_work,
											&body_work->node_work.snm_work,
											 body_work->node_work.work[ GMD_BOSS4_BODY_NODE_IDX_EGG_CONNECT ],
#if _IPHONE
											 FALSE); // 板なのでボスの回転は反映させない。
#else
											 TRUE);
#endif // _IPHONE

		// ダメージ点滅中ではない場合、位置更新

		// 少し下げる
		obj_work->pos.y += FX_F32_TO_FX32( GMD_BOSS4_CAP_OFFSET_Y );

		Angle32	r = _cap_rot_y;
		
		// エッグポットを包むように配置
		r += ((AKM_DEGtoA32( 360 )/ GMD_BOSS4_CAP_MAX) * cap_work->cap_no);
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

		obj_work->pos.x += FX_F32_TO_FX32( v.x );
		obj_work->pos.y += FX_F32_TO_FX32( v.y );
		obj_work->pos.z += FX_F32_TO_FX32( v.z );
	}

	if (_cap_kill_flag){

		// カプセルを爆破
		VecFx32	pos = obj_work->pos;
		// 前に出す
		pos.z	=	GMD_BOSS4_CAP_EXPLOSION_POS_Z;
		GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_CAP_EX_ES, &pos );

		cap_work->wait = 30;

		// 処理を爆発に切り替える
		T_FUNC( gmBoss4CapsuleBomb, obj_work );
		return;
	}

	// カプセル無敵?
	// 無敵の時は攻撃当たりもはずす
	GMS_ENEMY_3D_WORK*	ene_3d	=	(GMS_ENEMY_3D_WORK* )obj_work;
	if (_cap_inv_flag){
		if (!_cap_inv_hit){
			ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_NOHIT;
			ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
		}
		//ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_NOHIT;
	}else{
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_NOHIT;
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_ENABLE;
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_NOHIT;
		//ene_3d->ene_com.enemy_flag &= ~GMD_ENEMY_FLAG_NOHOMING;
	}

	if (IS_FLAG( GMD_BOSS4_CAP_FLAG_SIGNAL_DAMAGE, cap_work ) ){

		// この地点でカプセルの存在を消す
		_cap_count--;
		// 当たりを消す
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_ENABLE;
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

		RESET_FLAG( GMD_BOSS4_CAP_FLAG_SIGNAL_DAMAGE, cap_work );
		// ダメージ点滅初期化
		GmBoss4UtilInitFlicker( obj_work, &cap_work->flk_work );

		// 少しの間点滅
		GmBoss4UtilInit1ShotTimer(&cap_work->timer, 20);//仮

		cap_work->wait = 60;
	}
}


// =======================================================================
// gmBoss4CapsuleMain2nd
/*!
  カプセル メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4CapsuleMain2nd(OBS_OBJECT_WORK *obj_work)
{
//	GMS_BOSS4_BODY_WORK*	body_work	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS4_CAP_WORK*		cap_work	= (GMS_BOSS4_CAP_WORK* )obj_work;

	obj_work->move_flag &= ~OBD_MOVE_LIMIT_OUT;
	obj_work->spd_fall	=	FX_F32_TO_FX32( GMD_BOSS4_CAP_FALL_SPD_Y );

	obj_work->move_flag |=	OBD_MOVE_FALL;

	obj_work->pos.x += GmBoss4GetScrollOffset();
#if !_IPHONE
	if (cap_work->chibi_type == GME_BOSS4_CHIBI_TYPE_ID_BOUND){
		// 通常のちびエッグマン
		obj_work->dir.y	+= GMD_BOSS4_CAP_FALL_ROTATE_SPD;
	}else if (cap_work->chibi_type == GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD){
		// 高速エッグマン
		obj_work->dir.y	-= GMD_BOSS4_CAP_FALL_ROTATE_SPD;
	}else {
		// デカエッグマン、鉄球
		// できれば鉄球は色を変えたい・・・。
	}
#endif // !_IPHONE
	// 下接触したら爆破状態にする
	if (obj_work->move_flag & OBD_MOVE_UNDER){

		VecFx32	pos = obj_work->pos;
		// 前に出す
		pos.z	=	GMD_BOSS4_CAP_EXPLOSION_POS_Z;
		GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_CAP_EX_ES, &pos );

		cap_work->wait	=	GMD_BOSS4_CAP_TASK_KILL_TIME;

		obj_work->spd.x		=	FX_F32_TO_FX32( GMD_BOSS4_CAP_BOUND_SPD_X );
		obj_work->spd.y		=	FX_F32_TO_FX32( GMD_BOSS4_CAP_BOUND_SPD_Y );
		obj_work->move_flag &= ~OBD_MOVE_UNDER;
		obj_work->move_flag |=	OBD_MOVE_NOCOL;

		// ちびエッグマンを吐き出す
//		GME_BOSS4_CHIBI_TYPE_ID	type = 
//			gmBoss4ChibiGetAttackType( GmBoss4GetLife() );

		OBS_OBJECT_WORK *obj_chibi;

		u16	chibiType = GMD_EVENT_ID_BOSS4_CHIBI_2;
		switch( cap_work->chibi_type ){
		default:
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND:
			// GmBoss4ChibiInit2nd()の呼び出し
			chibiType = GMD_EVENT_ID_BOSS4_CHIBI_2;
			break;
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD:
			// GmBoss4ChibiInit2ndSpeed()の呼び出し
			chibiType = GMD_EVENT_ID_BOSS4_CHIBI_2_SPD;
			break;
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG:
			// GmBoss4ChibiInit2ndBig()の呼び出し
			chibiType = GMD_EVENT_ID_BOSS4_CHIBI_2_BIG;
			break;
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON:
			chibiType = GMD_EVENT_ID_BOSS4_CHIBI_2_IRON;
			break;
		}
		obj_chibi = GmEventMgrLocalEventBirth( chibiType,
												obj_work->pos.x, obj_work->pos.y,
												0,//flag
												0,0,0,0,
												0);

		obj_chibi->parent_obj = obj_work->parent_obj;
		GmBoss4IncObjCreateCount();

		// 基本の方向
		obj_chibi->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CAP_THROW_SPD_X );
		obj_chibi->spd.x = FX_F32_TO_FX32( GMD_BOSS4_CAP_THROW_SPD_Y );

		T_FUNC( gmBoss4CapsuleBomb2nd, obj_work );
	}


}


// =======================================================================
// gmBoss1ChainAtkHitFunc
/*!
  鎖 攻撃ヒット時処理関数
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss4CapsuleAtkHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)my_rect->parent_obj->parent_obj;

	// ヒットしたことを本体に知らせる
	body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_C2E_ATK_HIT;
	
	// エネミー標準攻撃ヒット処理
	GmEnemyDefaultAtkFunc(my_rect, your_rect);
}

// =======================================================================
// gmBoss4BodyDamageDefFunc
/*!
  本体 プレイヤー攻撃ヒット時くらい処理関数
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss4CapsuleDamageDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	OBS_OBJECT_WORK*	my_obj		= my_rect->parent_obj;
	OBS_OBJECT_WORK*	your_obj	= your_rect->parent_obj;
	GMS_BOSS4_CAP_WORK*	cap_work	= (GMS_BOSS4_CAP_WORK*)my_obj;

	GMS_BOSS4_BODY_WORK*	body_work	= (GMS_BOSS4_BODY_WORK*)my_obj->parent_obj;

//	GMS_PLAYER_WORK*	ply_work	= (GMS_PLAYER_WORK*)your_obj;
	
	if (your_obj && GMD_OBJTYPE_PLAYER == your_obj->obj_type) {

		// 全カプセル無敵中
		if (_cap_inv_flag>0)
			return;

		//　プレイヤーのリアクションを設定
		GmBoss4UtilSetPlayerAttackReaction( your_obj, my_obj );
		
		// ダメージSE再生(仮)
		GmSoundPlaySE("Enemy");
		//GmSoundPlaySE("Boss0_01");
		
		// 一気にカプセルがなくなるので1つあたったら数フレーム間別のカプセルはつぶせないようにしておく
		GmBoss4CapsuleSetInvincible( 30 );
		// 本体の方にもダメージがいかないようにする
		GmBoss4UtilInitNoHitTimer( &body_work->nohit_work, (GMS_ENEMY_COM_WORK*)body_work, 25 );

		// ダメージエフェクト生成
		//gmBoss4EffDamageInit(body_work);

		// 無敵ではないときだけダメージ処理
		if (!IS_FLAG( GMD_BOSS4_CAP_FLAG_INVINCIBLE, cap_work )){
			
			// ダメージフラグを立てる
			SET_FLAG( GMD_BOSS4_CAP_FLAG_SIGNAL_DAMAGE, cap_work );

			// 降下中ではない
			if (!(body_work->flag & GMD_BOSS4_BODY_FLAG_DOWN_AVOID)){
				// 避けフラグを立てる
				body_work->flag |= GMD_BOSS4_BODY_FLAG_UP_AVOID;
				body_work->avoid_timer = 30*3;
			}
		}
	}
}


// 撃破ステート更新 爆発処理
void gmBoss4CapsuleBomb(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_CAP_WORK	*cap_work	= (GMS_BOSS4_CAP_WORK* )obj_work;

	// 消す
	obj_work->disp_flag |= OBD_DISP_NODISP;

	if (cap_work->wait > 0) {
		cap_work->wait--;

		// Bomの更新
//		gmBoss4EffBombUpdateCreate( &cap_work->bomb ); 

		// TODO PAL
		if (cap_work->wait==30){
			// 全消去フラグがたっている場合はここで終了
			if (_cap_kill_flag)
				return;

			// ちびエッグマンを吐き出す
			OBS_OBJECT_WORK	*obj_chibi;

			// GmBoss4ChibiInit1st()の呼び出し
			obj_chibi = GmEventMgrLocalEventBirth( GMD_EVENT_ID_BOSS4_CHIBI_1,
													obj_work->pos.x, obj_work->pos.y,
													0,//flag
													0,0,0,0,
													0);
			GmBoss4IncObjCreateCount();
			/*
			static int rnd=0;
			if (rnd==1 ){
				obj_chibi = GmBoss4ChibiInit2ndBig( NULL, obj_work->pos.x, obj_work->pos.y, 0 );
			} else if (rnd==0){
				obj_chibi = GmBoss4ChibiInit1st( NULL, obj_work->pos.x, obj_work->pos.y, 0 );
			} else {
				obj_chibi = GmBoss4ChibiInit2ndSpeed( NULL, obj_work->pos.x, obj_work->pos.y, 0 );
			}
			rnd++;
			if (rnd>2){
				rnd=0;
			}
			*/
			obj_chibi->parent_obj = obj_work->parent_obj;
		}
		return;
	}

	// 終了
	GMM_BS_OBJ(cap_work)->flag	|= OBD_OBJECT_TASKCLEAR_REQUEST;

#if 0
	// 全消去フラグがたっている場合はここで終了
	if (_cap_kill_flag)
		return;

	// ちびエッグマンを吐き出す
	OBS_OBJECT_WORK	*obj_chibi;
		
	obj_chibi = GmBoss4ChibiInit1st( NULL, obj_work->pos.x, obj_work->pos.y, 0 );
	obj_chibi->parent_obj = obj_work->parent_obj;

#endif
}


// 撃破ステート更新 爆発処理
void gmBoss4CapsuleBomb2nd(OBS_OBJECT_WORK *obj_work)
{

	GMS_BOSS4_CAP_WORK	*cap_work	= (GMS_BOSS4_CAP_WORK* )obj_work;

//	obj_work->obj_3d->draw_state.alpha.alpha -= 0.03f;	// 半透明

	// ポーズ中はUPDATEしないようにする
	obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
	if (g_obj.flag & OBD_OBJ_PAUSE){
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
	}else{
		obj_work->pos.x += GmBoss4GetScrollOffset();// * FX32_ONE;
	}

	if (cap_work->wait > 0) {
		cap_work->wait--;

		if (cap_work->wait < 36){
			// カプセルを消す
			obj_work->disp_flag |= OBD_DISP_NODISP;
		}

		// Bomの更新
//		gmBoss4EffBombUpdateCreate( &cap_work->bomb ); 

		return;
	}

	// 終了
	GMM_BS_OBJ(cap_work)->flag	|= OBD_OBJECT_TASKCLEAR_REQUEST;

}


#if 0
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
#endif




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
