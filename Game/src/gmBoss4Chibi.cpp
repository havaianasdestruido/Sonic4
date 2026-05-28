// =======================================================================
/*!
  @file	gmBoss4Eggman.cpp
  @brief ボス4 カプセル

  @author K.Yurita(SafariGames)
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Chibi.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmBoss4Chibi.h"
#include "gmBoss4Body.h"

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
//=======================================================================
/* フラグ	GMS_BOSS4_CAP_WORK::flag */
#define		GMD_BOSS4_CHIBI_FLAG_INVINCIBLE				(1 << 2)							//!< 無敵状態（当たりはあるが、ライフは減らない）
#define		GMD_BOSS4_CHIBI_FLAG_DEAD					(1 << 3)							//!< 死亡フラグ（ライフが無い状態）
#define		GMD_BOSS4_CHIBI_FLAG_NOHIT					(1 << 6)							//!< 攻撃矩形オフ

#define		GMD_BOSS4_CHIBI_FLAG_BOUND_FIRST			(1 << 29)							//!< バウンドが発生したら立ち続ける
#define		GMD_BOSS4_CHIBI_FLAG_SIGNAL_DAMAGE			(1 << 30)							//!< body->bodyダメージ通知フラグ
#define		GMD_BOSS4_CHIBI_FLAG_SIGNAL_DEFEAT			(1 << 31)							//!< 死亡演出開始通知フラグ
//=======================================================================
//	ちびエッグマン表示に関するもの
//=======================================================================
//#define		GMD_BOSS4_CHIBI_BOOST_ENABLE												//!< ブースターを表示する

#define		GMD_BOSS4_CHIBI_LIFE_TIME					GMM_BOSS4_PAL_TIME(60*5)			//!< ちびビッグマンの生存時間
#define		GMD_BOSS4_CHIBI_POS_Z						(GMD_OBJ_DEFAULT_POS_Z_C_FRONT)		//!< ちびエッグマンのZ位置

//=======================================================================
//	ブースターに関するもの
//=======================================================================
// 位置
#define		GMD_BOSS4_CHIBI_ABURNER1_DISP_OFST_X		((Float)0.0f)						//!< 表示オフセットX
#define		GMD_BOSS4_CHIBI_ABURNER1_DISP_OFST_Y		((Float)+8.0f)						//!< 表示オフセットY
#define		GMD_BOSS4_CHIBI_ABURNER1_DISP_OFST_Z		((Float)-20.f)						//!< 表示オフセットZ
#define		GMD_BOSS4_CHIBI_ABURNER1_TURN_X				(AKM_DEGtoA32(-90.f))				//!後ろ向けバーニア

//=======================================================================
// ちびエッグマン同士の衝突に関するもの
//=======================================================================
// 自分たちであたる矩形を3にする
#define		GMD_BOSS4_RECT_GROUP						(OBD_RECT_GROUP_NO_3)				//!< 矩形グループ エネミー
#define		GMD_BOSS4_RECT_TARGET_GROUP					(OBD_RECT_TARGET_G_FLAG_3)			//!< ターゲット エネミー

#define		GMD_BOSS4_CHIBI_HIT_CHIBI_MOVE				(4.0f)								//!< ちびエッグマン同士が当たったときのずらし移動量
#define		GMD_BOSS4_CHIBI_HIT_CHIBI_ADDSPD			GMM_BOSS4_PAL_SPEED(1.0f)			//!< ちびエッグマン同士が当たったときの加速量

//=======================================================================
//	速度に関するもの
//=======================================================================
#define		GMD_BOSS4_CHIBI_SPD_LIMIT					GMM_BOSS4_PAL_SPEED(2.0f)			//!< ちびエッグマンの最高スピード

#define		GMD_BOSS4_CHIBI_BOUND_SPD_Y_NORMAL			GMM_BOSS4_PAL_SPEED(-4.5f)			//!< ちびエッグマン(NORMAL)のバウンド後のスピード(Y)
#define		GMD_BOSS4_CHIBI_BOUND_SPD_Y_BOUND			GMM_BOSS4_PAL_SPEED(-4.5f)			//!< ちびエッグマン(BOUND)のバウンド後のスピード(Y)
#define		GMD_BOSS4_CHIBI_BOUND_SPD_Y_SPEED			GMM_BOSS4_PAL_SPEED(-4.5f)			//!< ちびエッグマン(SPEED)のバウンド後のスピード(Y)
#define		GMD_BOSS4_CHIBI_BOUND_SPD_Y_BIG				GMM_BOSS4_PAL_SPEED(-3.5f)			//!< ちびエッグマン(BIG)のバウンド後のスピード(Y)
#define		GMD_BOSS4_CHIBI_BOUND_SPD_Y_IRON			GMM_BOSS4_PAL_SPEED(-2.0f)			//!< ちびエッグマン(IRON)のバウンド後のスピード(Y)

#define		GMD_BOSS4_CHIBI_FALL_SPD_NORMAL				GMM_BOSS4_PAL_SPEED(0.10f)			//!< ちびエッグマン(NORMAL)の落ちるスピード
#define		GMD_BOSS4_CHIBI_FALL_SPD_BOUND				GMM_BOSS4_PAL_SPEED(0.10f)			//!< ちびエッグマン(BOUND)の落ちるスピード
#define		GMD_BOSS4_CHIBI_FALL_SPD_SPEED				GMM_BOSS4_PAL_SPEED(0.20f)			//!< ちびエッグマン(SPEED)の落ちるスピード
#define		GMD_BOSS4_CHIBI_FALL_SPD_BIG				GMM_BOSS4_PAL_SPEED(0.10f)			//!< ちびエッグマン(BIG)の落ちるスピード
#define		GMD_BOSS4_CHIBI_FALL_SPD_IRON				GMM_BOSS4_PAL_SPEED(0.18f)			//!< ちびエッグマン(BIG)の落ちるスピード

//=======================================================================
//	バウンドに関するもの
//=======================================================================
#define		GMD_BOSS4_CHIBI_BOUND_FRAME					GMM_BOSS4_PAL_TIME(10)				//!< バウンドのフレーム数
#define		GMD_BOSS4_CHIBI_BOUND_ADD_Y					(-4.0f)								//!< 地面に当たった瞬間に戻すYの量
// 第一段階用
#define		GMD_BOSS4_CHIBI_BOUND_MULTI_SPD_X			GMM_BOSS4_PAL_SPEED(0.8f)			//!< 地面に当たった瞬間にXスピードに掛け合わす係数
#define		GMD_BOSS4_CHIBI_BOUND_ADD_SPD_X				GMM_BOSS4_PAL_SPEED(1.5f)			//!< 地面に当たった瞬間にプレイヤー方向にXスピードを足しこむ係数
// 第二段階用
#define		GMD_BOSS4_CHIBI_BOUND_SPD_X_BOUND			GMM_BOSS4_PAL_SPEED(-1.2f)			//!< ちびエッグマン(BOUND)のバウンド後のスピード(X)
#define		GMD_BOSS4_CHIBI_BOUND_SPD_X_SPEED			GMM_BOSS4_PAL_SPEED(-2.5f)			//!< ちびエッグマン(SPEED)のバウンド後のスピード(X)
#define		GMD_BOSS4_CHIBI_BOUND_SPD_X_BIG				GMM_BOSS4_PAL_SPEED(-2.0f)			//!< ちびエッグマン(BIG)のバウンド後のスピード(X)
#define		GMD_BOSS4_CHIBI_BOUND_SPD_X_IRON			GMM_BOSS4_PAL_SPEED(-2.0f)			//!< ちびエッグマン(BIG)のバウンド後のスピード(X)

#define		GMD_BOSS4_CHIBI_BOUND_OUT_Y					GMM_BOSS4_PAL_ZOOM(1.04f)			//!< バウンドしたあと、Yの大きさを戻すための係数
#define		GMD_BOSS4_CHIBI_BOUND_OUT_X					GMM_BOSS4_PAL_ZOOM(0.95f)			//!< バウンドしたあと、Xの大きさを戻すための係数

#define		GMD_BOSS4_CHIBI_BOUND_IN_Y					GMM_BOSS4_PAL_ZOOM(0.90f)			//!< バウンド中、Yの大きさに掛け合わせる係数
#define		GMD_BOSS4_CHIBI_BOUND_IN_X					GMM_BOSS4_PAL_ZOOM(1.15f)			//!< バウンド中、Xの大きさに掛け合わせる係数
#define		GMD_BOSS4_CHIBI_BOUND_IN_Y_LIMIT			(0.60f)								//!< バウンド中、Yがこれ以上小さくならないリミットサイズ
#define		GMD_BOSS4_CHIBI_BOUND_IN_X_LIMIT			(1.50f)								//!< バウンド中、Yがこれ以上大きくならないリミットサイズ

//=======================================================================
//	爆発に関するもの
//=======================================================================
// ちびエッグマンと爆発の中心位置がずれているため位置をずらす
#define		GMD_BOSS4_CHIBI_EXPLOSION_ADD_Y				(-16.0f)							//!< ちびエッグマンが爆発するときに足しこまれるY位置
#define		GMD_BOSS4_CHIBI_EXPLOSION_POS_Z				(GMD_OBJ_DEFAULT_POS_Z_C_FRONT \
																		+ FX32_ONE)			//!< ちびエッグマンが爆発するときのZ位置

#define		GMD_BOSS4_CHIBI_EXPLOSION_BASE_HIDE_TIME	GMM_BOSS4_PAL_TIME(2)				//!< 爆発が発生してから本体を消す(見えなくするだけ)フレーム数(TODO:PAL)
#define		GMD_BOSS4_CHIBI_EXPLOSION_TASK_KILL_TIME	GMM_BOSS4_PAL_TIME(60)				//!< 爆発が発生してからタスクを消すフレーム数(TODO:PAL)

//=======================================================================
//	その他
//=======================================================================
// 発生テーブル
#define		GME_BOSS4_CHIBI_ATK_TBL						(20)								//!< 第2形態の時の攻撃タイプのテーブルの数 
#define		GME_BOSS4_CHIBI_THW_TBL						(20)								//!< 第2形態の時の投げ方のテーブルの数 



/*------ Macro Functions -----------------------------------------------*/
#define	T_FUNC(func, w)			{void func(OBS_OBJECT_WORK*); w->ppFunc = func;}
//#define	T_FUNC_SUB(func, w)		{static void func(GMS_BOSS4_CAP_WORK*); w->proc_update = func;}
#define	SET_FLAG( f, w )		w->flag |= f
#define	RESET_FLAG( f, w )		w->flag &=~f
#define	IS_FLAG( f, w )			(w->flag & f)

// =======================================================================

/*------ Static Declarations -------------------------------------------*/
//############ ボス4 カプセル ###############################################

static	void gmBoss4ChibiAtkHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);
static	void gmBoss4ChibiDefHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);


#if defined(MTD_DEBUG)
//############ デバッグ ###############################################
#endif /* defined(MTD_DEBUG) */

static OBS_OBJECT_WORK* GmBoss4ChibiInit(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, GME_BOSS4_CHIBI_TYPE_ID type);

static	void gmBoss4ChibiExit(MTS_TASK_TCB *tcb);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
const	NNS_RGB		gm_boss4_color_red ={ 1.0, 0.0f, 0.0f };

static	BOOL		gm_chibi_inv_flag		= FALSE;
static	BOOL		gm_chibi_exp_flag		= FALSE;

/*------ Global Functions ----------------------------------------------*/


// =======================================================================
// gmBoss4ChibiGetAttackType
/*!
	攻撃タイプを返す

  @param my_rect	[in]	ライフ
*/
// =======================================================================
// 体力を引数にちびエッグマンのタイプを返す関数
GME_BOSS4_CHIBI_TYPE_ID gmBoss4ChibiGetAttackType(int life)
{
	UNREFERENCED_PARAMETER(life);

	static int _index = 0;
	int			type = 0;

	// テーブルの見易さを優先し、後にタイプに変換する
	// 0 = 通常タイプ 1 = スピードタイプ 2 = ビッグタイプ

	// ライフが残り2のとき
	static const int life2_tbl[ GME_BOSS4_CHIBI_ATK_TBL	]={
		0,0,0,0,1,
		0,0,0,0,1,
		0,0,0,0,1,
		0,0,1,0,0,
	};

	// ライフが残り1のとき
	static const int life1_tbl[ GME_BOSS4_CHIBI_ATK_TBL	]={
		0,1,0,1,0,
		2,1,2,1,2,
		0,1,2,1,0,
		2,1,0,1,2,
	};

	//----- FINAL用 ------
	// ライフが残り4のとき
	static const int life3_tbl_f[ GME_BOSS4_CHIBI_ATK_TBL	]={
		0,0,3,0,0,
		0,0,3,0,0,
		0,0,3,0,0,
		0,0,3,0,0,
	};

	// ライフが残り3のとき
	static const int life2_tbl_f[ GME_BOSS4_CHIBI_ATK_TBL	]={
		0,0,3,0,1,
		0,0,3,0,1,
		0,0,3,0,1,
		0,1,3,0,0,
	};

	// ライフが残り2-1のとき
	static const int life1_tbl_f[ GME_BOSS4_CHIBI_ATK_TBL	]={
		0,1,3,1,0,
		2,1,0,1,2,
		3,1,0,1,3,
		2,1,3,1,2,
	};



	// 安全のため先にマスク
	_index %= GME_BOSS4_CHIBI_ATK_TBL;

	if (GmBoss4GetLife() >= GME_BOSS4_LIFE_H ){
		// 一番多いとき
		if (GmBoss4CheckBossRush()){
			type = life3_tbl_f[ _index ];
		}else{
			// このときは常に通常弾
			return GME_BOSS4_CHIBI_TYPE_ID_BOUND;
		}
	}else {

		if (GmBoss4CheckBossRush()){
			// FINAL
			// ライフチェック
			if ((GmBoss4GetLife() < GME_BOSS4_LIFE_H ) && (GmBoss4GetLife() > GME_BOSS4_LIFE_L )) {
				// 中間のとき
				type = life2_tbl_f[ _index ];

			} else {
				// 少ないとき
				type = life1_tbl_f[ _index ];
			}
		}else{
			// ライフチェック
			if ((GmBoss4GetLife() < GME_BOSS4_LIFE_H ) && (GmBoss4GetLife() > GME_BOSS4_LIFE_L )) {
				// 中間のとき
				type = life2_tbl[ _index ];

			} else {
				// 少ないとき
				type = life1_tbl[ _index ];
			}
		}
	}
	// テーブルが一定化するように
	_index++;

	switch( type ){
	default:
	case 0:
		return GME_BOSS4_CHIBI_TYPE_ID_BOUND;
		// break;
	case 1:
		return GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD;
		// break;
	case 2:
		return GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG;
		// break;
	case 3:
		return GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON;
		// break;
	}

	// 念のため(実際にはこない)
	return GME_BOSS4_CHIBI_TYPE_ID_BOUND;	
}



// =======================================================================
// gmBoss4ChibiGetThrowType
/*!
	投げる方法を返す

  @param my_rect	[in]	type　(念のため攻撃タイプを入れれるようにしておく)
*/
// =======================================================================
Sint32 gmBoss4ChibiGetThrowType( GME_BOSS4_CHIBI_TYPE_ID t )
{
	UNREFERENCED_PARAMETER(t);

	static int _index = 0;
	Sint32	type;

	// テーブルの見易さを優先し、後にタイプに変換する
	// 0 = 通常タイプ 1 = 特殊タイプ

	static const int _tbl[ GME_BOSS4_CHIBI_THW_TBL	]={
		0,0,1,0,0,
		0,0,0,1,0,
		0,1,0,0,0,
		0,0,1,0,0,
	};

	// 安全のため先にマスク
	_index %= GME_BOSS4_CHIBI_THW_TBL;

	type = _tbl[ _index ];

	_index++;

	return type;
}

// =======================================================================
// GmBoss4Build
/*!
  ボス4 データ構築
 */
// =======================================================================
void GmBoss4ChibiBuild(void)
{
	// 仮でエッグマンを使う
	// モーション
//	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_01_EGG_MTN),
//						IDB_BOSS01_BOSS01_EGG_MTN_AMB, GMD_BOSS4_ARC);

	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_CAPSULE_MTN),
						IDB_BOSS04_BOSS04_CAPSULE_MTN_AMB, GMD_BOSS4_ARC);

}


// =======================================================================
// GmBoss4Flush
/*!
  ボス１ データ片付け
 */
// =======================================================================
void GmBoss4ChibiFlush(void)
{
	// モーション
//	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_01_EGG_MTN));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_CAPSULE_MTN));

}

// =======================================================================
// GmBoss4CapsuleInit1st
/*!
  ボス１ エッグマン 初期化(第一段階)
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4ChibiInit1st(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);

	return GmBoss4ChibiInit(eve_rec, pos_x, pos_y+16*FX32_ONE, GME_BOSS4_CHIBI_TYPE_ID_NORMAL );

}


// =======================================================================
// GmBoss4CapsuleInitNormal
/*!
  ボス１ エッグマン 初期化(第２段階 通常)
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4ChibiInit2nd(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);

	return GmBoss4ChibiInit(eve_rec, pos_x, pos_y+16*FX32_ONE, GME_BOSS4_CHIBI_TYPE_ID_BOUND );

}


// =======================================================================
// GmBoss4CapsuleInitSpeed
/*!
  ボス１ エッグマン 初期化(第２段階 スピード)
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4ChibiInit2ndSpeed(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);

	return GmBoss4ChibiInit(eve_rec, pos_x, pos_y+16*FX32_ONE, GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD );

}


// =======================================================================
// GmBoss4CapsuleInitBig
/*!
  ボス１ エッグマン 初期化(第２段階 巨大)
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4ChibiInit2ndBig(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);

	return GmBoss4ChibiInit(eve_rec, pos_x, pos_y+16*FX32_ONE, GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG );

}


// =======================================================================
// GmBoss4CapsuleInitIron
/*!
  ボス１ エッグマン 初期化(第２段階 鉄球)
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss4ChibiInit2ndIron(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);

	return GmBoss4ChibiInit(eve_rec, pos_x, pos_y+16*FX32_ONE, GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON );

}

// =======================================================================
// gmBoss4ChibiInvincible
/*!
  ちびエッグマン　無敵設定
  
  @param inv	[in]	無敵時間
 */
// =======================================================================
void	GmBoss4ChibiSetInvincible( BOOL flg )
{
	gm_chibi_inv_flag = flg;
}

// =======================================================================
// GmBoss4ChibiExplosion
/*!
  全ちびエッグマンを爆破する
  
 */
// =======================================================================
void	GmBoss4ChibiExplosion()
{
	gm_chibi_exp_flag = TRUE;
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
OBS_OBJECT_WORK* GmBoss4ChibiInit(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, GME_BOSS4_CHIBI_TYPE_ID type)
{
//	UNREFERENCED_PARAMETER(type);//仮
	
	OBS_OBJECT_WORK*		obj_work;
	GMS_ENEMY_3D_WORK*		ene_3d;
	GMS_BOSS4_CHIBI_WORK*		chibi_work;
	
	// オブジェクト作成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS4_CHIBI_WORK),
										"BOSS4_C.E");
	
	ene_3d	=	(GMS_ENEMY_3D_WORK* )obj_work;
	chibi_work=	(GMS_BOSS4_CHIBI_WORK*)obj_work;


	chibi_work->type = type;

	// ワーク設定
	// TODO : 未実装
	
	// ライフ無し
	
	// 地形当たり無し
	//obj_work->move_flag	|= OBD_MOVE_NOCOL;

	switch (chibi_work->type){
	default:
	case GME_BOSS4_CHIBI_TYPE_ID_NORMAL:
	case GME_BOSS4_CHIBI_TYPE_ID_BOUND:
		// 本体モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
									GmBoss4GetObj3D(IDB_BOSS04_MDL_B04_M_EGG1_ZNO),
									&ene_3d->obj_3d);
		break;
	case GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD:
		// 本体モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
									GmBoss4GetObj3D(IDB_BOSS04_MDL_B04_M_EGG2_ZNO),
									&ene_3d->obj_3d);
		break;
	case GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG:
		// 本体モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
									GmBoss4GetObj3D(IDB_BOSS04_MDL_B04_M_EGG3_ZNO),
									&ene_3d->obj_3d);
		break;
	case GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON:
		// 本体モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
									GmBoss4GetObj3D(IDB_BOSS04_MDL_B04_M_EGG4_ZNO),
									&ene_3d->obj_3d);
		break;
	}

	
	// モーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,// TODO : 仮 登録モーションID
								  TRUE,	// TODO : 仮 マージ
								  ObjDataGet(GMD_DWORK_NO_BOSS_04_CAPSULE_MTN),
								  NULL,
								  0,
								  NULL);
	

	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ユーザ描画ステートを使用
	obj_work->disp_flag	|= OBD_DISP_DRAWSTATE;
		
	obj_work->flag		|= OBD_OBJECT_NOCLIP;	//仮
//	obj_work->disp_flag	|= OBD_DISP_REPEAT;
	obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;

//	obj_work->move_flag	|= OBD_MOVE_NOCOLFIELD;
	obj_work->move_flag	&= ~OBD_MOVE_NOCOLFIELD;
	obj_work->move_flag	|= OBD_MOVE_FALL;

	// ホーミングは常に禁止
	ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

	ObjObjectFieldRectSet( obj_work, -20, -20-24, 20, 20-24 );

#if 0
	// TODO : 仮
	ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK], -20, -20-20, 20, 20-20);
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].ppHit	= gmBoss4ChibiAtkHitFunc;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_MOVE_PUSH_COL;

	ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY], -24, -24-20, 24, 24-20);
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].ppHit = gmBoss4ChibiDefHitFunc;
	ObjRectGroupSet( &ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY], GMD_BOSS4_RECT_GROUP, GMD_BOSS4_RECT_TARGET_GROUP );
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_NOHIT;	
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_ENABLE;	
#endif	
	/*
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag  = OBD_RECT_GROUP | OBD_RECT_NODAMAGE | OBD_RECT_NOHIT_UP | OBD_RECT_NOAUTO_ENABLEOFF;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].ppHit = gmBoss4ChibiDefHitFunc;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_ENABLE;
	ObjRectGroupSet( &ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY], GMD_OBJ_RECT_GROUP_PLAYER, GMD_OBJ_RECT_TARGET_GROUPFLAG_ENEMY );
	*/
//	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// TODO : 食らい当たり設定     仮
//	ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF], -24, -24, 24, 24);
//	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].ppDef	= gmBoss4CapsuleDamageDefFunc;

	// メイン処理設定
	T_FUNC( gmBoss4ChibiWaitLoad, obj_work );
	//obj_work->ppFunc	= gmBoss4CapsuleWaitLoad;

	// 爆破フラグは落とす
	gm_chibi_exp_flag = FALSE;

	// まだバウンドしていない(当たりを調整するため)
	RESET_FLAG( GMD_BOSS4_CHIBI_FLAG_BOUND_FIRST, chibi_work );

	// 第一段階でなければバウンドしたことにする(当たりを調整するため)
	if (chibi_work->type!=GME_BOSS4_CHIBI_TYPE_ID_NORMAL){
		SET_FLAG( GMD_BOSS4_CHIBI_FLAG_BOUND_FIRST, chibi_work );
	}

	// エッグマン終了処理変更
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss4ChibiExit);

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}

/*------ Static Functions ----------------------------------------------*/

// =======================================================================
// gmBoss1ChibiAtkHitFunc
/*!
  攻撃ヒット時処理関数
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss4ChibiAtkHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{

	GMS_BOSS4_BODY_WORK	*body_work	= (GMS_BOSS4_BODY_WORK*)my_rect->parent_obj->parent_obj;
	GMS_BOSS4_CHIBI_WORK*	chibi_work	= (GMS_BOSS4_CHIBI_WORK* )my_rect->parent_obj;

	
	// ヒットしたことを本体に知らせる
	body_work->flag	|= GMD_BOSS4_BODY_FLAG_SIGNAL_C2E_ATK_HIT;
	
	// エネミー標準攻撃ヒット処理
	GmEnemyDefaultAtkFunc(my_rect, your_rect);

	// 第2段階のエッグマン?
	if (chibi_work->type != GME_BOSS4_CHIBI_TYPE_ID_NORMAL){
		//方向を右に向かせる
		GMS_PLAYER_WORK* ply = (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();

		// 左を向いていたら反転
		if (GmPlayerKeyCheckWalkLeft( ply )){
			GmPlayerSetReverse( ply );
		}
	}

	// 自分は消える
	SET_FLAG( GMD_BOSS4_CHIBI_FLAG_SIGNAL_DAMAGE, chibi_work );
}




// =======================================================================
// gmBoss1ChibiAtkHitFunc
/*!
  別のちびエッグマンとのヒット時処理関数
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss4ChibiDefHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect){

	GMS_BOSS4_CHIBI_WORK*	my_work		= (GMS_BOSS4_CHIBI_WORK*)my_rect->parent_obj;
	GMS_BOSS4_CHIBI_WORK*	your_work	= (GMS_BOSS4_CHIBI_WORK*)your_rect->parent_obj;

	OBS_OBJECT_WORK*	my_obj		= (OBS_OBJECT_WORK*)my_work;
	OBS_OBJECT_WORK*	your_obj	= (OBS_OBJECT_WORK*)your_work;

	// まだバウンドしてないときは当たりはなし
	if (!IS_FLAG( GMD_BOSS4_CHIBI_FLAG_BOUND_FIRST, my_work )) return;
	if (!IS_FLAG( GMD_BOSS4_CHIBI_FLAG_BOUND_FIRST, your_work )) return;

	// 自分が右のときは
	if (my_obj->pos.x > your_obj->pos.x){
		my_obj->pos.x += FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_MOVE);
		my_obj->spd.x += FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_ADDSPD);
	}
	if (my_obj->pos.x < your_obj->pos.x){
		my_obj->pos.x -= FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_MOVE);
		my_obj->spd.x -= FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_ADDSPD);
	}
	// 同じ場合は念のためy座標で左右に分ける
	if (my_obj->pos.x == your_obj->pos.x){
		if (my_obj->pos.y < my_obj->pos.y){
			my_obj->pos.x += FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_MOVE);
			my_obj->spd.x += FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_ADDSPD);
		}else{
			my_obj->pos.x -= FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_MOVE);
			my_obj->spd.x -= FX_F32_TO_FX32( GMD_BOSS4_CHIBI_HIT_CHIBI_ADDSPD);
		}
	}

	// リミッター
	if (my_obj->spd.x > FX_F32_TO_FX32( GMD_BOSS4_CHIBI_SPD_LIMIT ) ){
		my_obj->spd.x = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_SPD_LIMIT );
	}
	if (my_obj->spd.x < -FX_F32_TO_FX32( GMD_BOSS4_CHIBI_SPD_LIMIT )){
		my_obj->spd.x = -FX_F32_TO_FX32( GMD_BOSS4_CHIBI_SPD_LIMIT );
	}
}



// ############################################################################
// ボス4 ちびエッグマン
// ############################################################################

// ============================================================================
// 制御処理
// ============================================================================

// =======================================================================
// gmBoss4ChibiWaitLoad
/*!
  ちびエッグマン ロード完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4ChibiWaitLoad(OBS_OBJECT_WORK *obj_work)
{
//	GMS_BOSS4_BODY_WORK	*parent_body	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS4_CHIBI_WORK	*chibi_work	= (GMS_BOSS4_CHIBI_WORK*)obj_work;
	
	if (GmBoss4IsBuilded()){
	//if (GMM_BOSS4_MGR(parent_body)->flag & GMD_BOSS4_MGR_FLAG_LOAD_END) {
		// 更新関数設定
		T_FUNC( gmBoss4ChibiMain, obj_work );
		//obj_work->ppFunc	= gmBoss4CapMain;
		
		// 初期シーケンス設定
		//gmBoss4CapProcInit(cap_work);
		GmBoss4UtilInit1ShotTimer( &chibi_work->timer, GMD_BOSS4_CHIBI_LIFE_TIME );		// 5秒を設定

		// 点滅設定
		GmBoss4UtilInitFlicker( obj_work, &chibi_work->flk_work, 1, 60*3, 4, (chibi_work->timer.timer/20)*3, &gm_boss4_color_red );

		chibi_work->count = -1;

		// 初期アクションの設定
		switch (chibi_work->type){
		default:
		case GME_BOSS4_CHIBI_TYPE_ID_NORMAL:
			GmBsCmnSetAction( obj_work, FALSE, IDB_BOSS04_CAPSULE_MTN_B04_1_BOM_01_ZNM );
			obj_work->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_Y_NORMAL );
			break;
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND:
			GmBsCmnSetAction( obj_work, FALSE, IDB_BOSS04_CAPSULE_MTN_B04_1_BOM_01_ZNM );
			obj_work->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_Y_BOUND );
			break;
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD:
			GmBsCmnSetAction( obj_work, FALSE, IDB_BOSS04_CAPSULE_MTN_B04_1_BOM_02_ZNM );
			obj_work->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_Y_SPEED );
			break;
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG:
			GmBsCmnSetAction( obj_work, FALSE, IDB_BOSS04_CAPSULE_MTN_B04_1_BOM_03_ZNM );
			obj_work->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_Y_BIG );
			break;
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON:
			GmBsCmnSetAction( obj_work, FALSE, IDB_BOSS04_CAPSULE_MTN_B04_1_BOM_04_ZNM );
			obj_work->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_Y_IRON );
			break;
		}

		// 落ちるスピード
		obj_work->spd_fall = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_FALL_SPD_NORMAL );

		// 優先を手前に
		obj_work->pos.z = GMD_BOSS4_CHIBI_POS_Z;

		//col_flag
		
//		VecFx32 spd;			///< 移動速度 1:19:12
 //   VecFx32 spd_add;		///< 移動加速度 1:19:12

		chibi_work->bound = 0;


		// ノードマトリクスシステム初期化
		GmBoss4UtilInitNodeMatrix( &chibi_work->node_work, obj_work, GMD_BOSS4_CHIBI_NODE_SNM_NUM );

		const NNS_MATRIX*	mtx;
		mtx = GmBoss4UtilGetNodeMatrix( &chibi_work->node_work, GMD_BOSS4_CHIBI_NODE_IDX_BASE );

		gmBoss4ChibiBoosterCreate( chibi_work );

		// ちびエッグマン出現SE再生
		GmSoundPlaySE("Boss4_01");

	}
}

// =======================================================================
// gmBoss4ChibiExit
/*!
  本体終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void gmBoss4ChibiExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK*		obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_BOSS4_CHIBI_WORK*	chibi_work	= (GMS_BOSS4_CHIBI_WORK*)obj_work;

	// オブジェクト生成数デクリメント
	GmBoss4DecObjCreateCount();

	GmBoss4UtilExitNodeMatrix( &chibi_work->node_work );
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}


// =======================================================================
// gmBoss4ChibiMain
/*!
  ちびエッグマン メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss4ChibiMain(OBS_OBJECT_WORK *obj_work)
{
//	GMS_BOSS4_BODY_WORK*	body_work	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS4_CHIBI_WORK*	chibi_work	= (GMS_BOSS4_CHIBI_WORK* )obj_work;



	// 出現アクション終了と同時に当たりを発生させる
	if (GmBsCmnIsActionEnd(obj_work)) {

		GMS_ENEMY_3D_WORK* ene_3d	=	(GMS_ENEMY_3D_WORK* )obj_work;
		// TODO : 仮
		ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK], -10, -10-12, 10, 10-12);
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].ppHit	= gmBoss4ChibiAtkHitFunc;
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_MOVE_PUSH_COL;

		ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY], -14, -14-12, 18, 14-12);
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].ppHit = gmBoss4ChibiDefHitFunc;
		ObjRectGroupSet( &ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY], GMD_BOSS4_RECT_GROUP, GMD_BOSS4_RECT_TARGET_GROUP );
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_NOHIT;	
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_ENABLE;	


		// 初期アクションの設定
		switch (chibi_work->type){
		default:
		case GME_BOSS4_CHIBI_TYPE_ID_NORMAL:
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND:
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD:
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON:
			break;
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG:
			ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK], -30, -30-30, 30, 30-30);
			ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY], -44, -34-30, 44, 34-30);
			break;
		}
	}


	// タイプによる制限
	switch( chibi_work->type ){
	default:
	case GME_BOSS4_CHIBI_TYPE_ID_NORMAL: 
		obj_work->spd_fall	=	FX_F32_TO_FX32( GMD_BOSS4_CHIBI_FALL_SPD_NORMAL );
		break;

	case GME_BOSS4_CHIBI_TYPE_ID_BOUND:
		obj_work->move_flag &= ~OBD_MOVE_LIMIT_OUT;
		obj_work->spd_fall	=	FX_F32_TO_FX32( GMD_BOSS4_CHIBI_FALL_SPD_BOUND );
		obj_work->pos.x		+=	GmBoss4GetScrollOffset();
		break;

	case GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD:
		obj_work->move_flag &= ~OBD_MOVE_LIMIT_OUT;
		obj_work->spd_fall	=	FX_F32_TO_FX32( GMD_BOSS4_CHIBI_FALL_SPD_SPEED );
		obj_work->pos.x += GmBoss4GetScrollOffset();

		// バウンドしてない場合は倍足しこむ
		if (chibi_work->bound==0){
			obj_work->pos.y += obj_work->spd.y;
		}
		break;

	case GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG:
		obj_work->move_flag &= ~OBD_MOVE_LIMIT_OUT;
		obj_work->spd_fall	=	FX_F32_TO_FX32( GMD_BOSS4_CHIBI_FALL_SPD_BIG );
		obj_work->pos.x		+= GmBoss4GetScrollOffset();
		break;

	case GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON:
		obj_work->move_flag &= ~OBD_MOVE_LIMIT_OUT;
		obj_work->spd_fall	=	FX_F32_TO_FX32( GMD_BOSS4_CHIBI_FALL_SPD_IRON );
		obj_work->pos.x		+= GmBoss4GetScrollOffset();
		obj_work->pos.x		-= FX_F32_TO_FX32(4);		
		break;
	}
	
	// 変形しているものを標準の大きさに近づける処理
	if (obj_work->scale.y < FX_F32_TO_FX32(1.0f)){
		obj_work->scale.y = (fx32)(obj_work->scale.y * GMD_BOSS4_CHIBI_BOUND_OUT_Y);	//1.04f;
		if (obj_work->scale.y > FX_F32_TO_FX32(1.0f) ){
			obj_work->scale.y = FX_F32_TO_FX32(1.0f);
		}
	};
	if ( obj_work->scale.x > FX_F32_TO_FX32(1.0f) ){
		obj_work->scale.x = (fx32)( obj_work->scale.x * GMD_BOSS4_CHIBI_BOUND_OUT_X);	//0.95f;
		if (obj_work->scale.x < FX_F32_TO_FX32(1.0f)){
			obj_work->scale.x = FX_F32_TO_FX32(1.0f);
		}
	};

	// 爆破命令が出ている場合は爆発するフラグを立てる
	if (gm_chibi_exp_flag){
		// 爆破する
		SET_FLAG( GMD_BOSS4_CHIBI_FLAG_SIGNAL_DAMAGE, chibi_work );		
	}


	GMS_ENEMY_3D_WORK* ene_3d	=	(GMS_ENEMY_3D_WORK* )obj_work;
	// ホーミングは常に禁止
	//ene_3d->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

	// あたらない状態
	if (gm_chibi_inv_flag){
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_NOHIT;
	}else{
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_NOHIT;
	}

	// 下接触したらバウンド状態にする
	if (obj_work->move_flag & OBD_MOVE_UNDER){

		// ほかのチビエッグマンとの当たりを発生させる
		SET_FLAG( GMD_BOSS4_CHIBI_FLAG_BOUND_FIRST, chibi_work );

//		obj_work->pos.y += FX_F32_TO_FX32(-1.0);
		obj_work->move_flag &= ~OBD_MOVE_UNDER;

		// 下に移動しない、次にもう一度あたらないように変更
		obj_work->move_flag &= ~OBD_MOVE_FALL;
		obj_work->move_flag |= OBD_MOVE_NOCOL;

		chibi_work->bnd_xspd = obj_work->spd.x;		// バウンドのスピードを確保
		obj_work->spd.y = 0;	//FX32_ONE * -4.0f;
		obj_work->spd.x = 0;	//FX32_ONE * -4.0f;
		obj_work->spd_fall	= 0;

		if (chibi_work->type == GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON){
			chibi_work->bound = 1000;
		}else{
			chibi_work->bound = GMD_BOSS4_CHIBI_BOUND_FRAME;
		}

		if (chibi_work->type == GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG){
			// ギガエッグマン地面はねSE再生
			GmSoundPlaySE("Boss4_03");
		}else{
			if (chibi_work->type == GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON){
				// TODO 鉄球地面SE再生

			}else{
				// ちびエッグマン地面はねSE再生
				GmSoundPlaySE("Boss4_02");
			}
		}
	}

	// バウンド状態終了?
	if ( chibi_work->bound > 0 ){
		if ( --chibi_work->bound == 0){
			// 少し戻す
			obj_work->pos.y += FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_ADD_Y );	//(FX32_ONE * -4.0);

			// 下に落ちるように変更
			obj_work->move_flag |=	OBD_MOVE_FALL;
			obj_work->move_flag &= ~OBD_MOVE_NOCOL;
			obj_work->move_flag &= ~OBD_MOVE_UNDER;

			// バウンドスピード(X)を戻す
			obj_work->spd.x = chibi_work->bnd_xspd;

			// タイプにより跳ね返り量を設定
			switch( chibi_work->type ){
			default:
			case GME_BOSS4_CHIBI_TYPE_ID_NORMAL: 
				obj_work->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_Y_NORMAL );

				// プレイヤーの方に移動する
				if (GmBsCmnGetPlayerObj()->pos.x < obj_work->pos.x) {
					obj_work->spd.x = (fx32)(obj_work->spd.x * GMD_BOSS4_CHIBI_BOUND_MULTI_SPD_X) - FX_F32_TO_FX32(GMD_BOSS4_CHIBI_BOUND_ADD_SPD_X);
				}else{
					obj_work->spd.x = (fx32)(obj_work->spd.x * GMD_BOSS4_CHIBI_BOUND_MULTI_SPD_X) + FX_F32_TO_FX32(GMD_BOSS4_CHIBI_BOUND_ADD_SPD_X);
				}
				// リミッター
				if (obj_work->spd.x > FX_F32_TO_FX32( GMD_BOSS4_CHIBI_SPD_LIMIT ) ){
					obj_work->spd.x = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_SPD_LIMIT );
				}
				if (obj_work->spd.x < -FX_F32_TO_FX32( GMD_BOSS4_CHIBI_SPD_LIMIT )){
					obj_work->spd.x = -FX_F32_TO_FX32( GMD_BOSS4_CHIBI_SPD_LIMIT );
				}
				break;
			case GME_BOSS4_CHIBI_TYPE_ID_BOUND:
				obj_work->spd.x = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_X_BOUND );
				obj_work->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_Y_BOUND );
				break;
			case GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD:
				obj_work->spd.x = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_X_SPEED );
				obj_work->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_Y_SPEED );
				break;
			case GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG:
				obj_work->spd.x = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_X_BIG );
				obj_work->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_Y_BIG );
				break;
			case GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON:
				obj_work->spd.x = FX_F32_TO_FX32( 0.0 );
				obj_work->spd.y = FX_F32_TO_FX32( 0.0 );
				break;
			}


		} else {
			if (chibi_work->type == GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON){
				// 鉄球はなにもしない

			}else{
				// バウンドアクション
				obj_work->scale.y = (fx32)(obj_work->scale.y * GMD_BOSS4_CHIBI_BOUND_IN_Y);
				obj_work->scale.x = (fx32)(obj_work->scale.x * GMD_BOSS4_CHIBI_BOUND_IN_X);
				obj_work->spd_fall	= 0;
				
				// 念のためリミッターを用意
				if (obj_work->scale.y < FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_IN_Y_LIMIT )){
					obj_work->scale.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_IN_Y_LIMIT );
				}
				if (obj_work->scale.x > FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_IN_X_LIMIT )){
					obj_work->scale.x = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_IN_X_LIMIT );
				}
			}
		}
	}

	// 第一段階のみ方向を変換する
	if (chibi_work->type != GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON){
		GmBoss4UtilLookAtPlayer(&chibi_work->dir, obj_work, 5);
	}
	// 時間で消滅
	if (GmBoss4UtilUpdate1ShotTimer(&chibi_work->timer)) {
		// ダメージを受けたことにする
		SET_FLAG( GMD_BOSS4_CHIBI_FLAG_SIGNAL_DAMAGE, chibi_work );		
	}

	// 点滅処理(第一段階のみ)
	if (chibi_work->type == GME_BOSS4_CHIBI_TYPE_ID_NORMAL){
		if (GmBoss4UtilUpdateFlicker(obj_work, &chibi_work->flk_work)){

			// 点滅間隔を徐々に縮める
			int w = (chibi_work->timer.timer/20)*3;
			GmBoss4UtilInitFlicker( obj_work, &chibi_work->flk_work, 1, w, 2, 0, &gm_boss4_color_red );

		}
	}

	// ダメージを受けたら爆発する
	if (IS_FLAG( GMD_BOSS4_CHIBI_FLAG_SIGNAL_DAMAGE, chibi_work ) ){

		RESET_FLAG( GMD_BOSS4_CHIBI_FLAG_SIGNAL_DAMAGE, chibi_work );

#if 0	// 鉄球は上に跳ね飛ぶver
		if (chibi_work->type == GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON){
			// 爆発時間を0に設定(足しこむ方式)
//			chibi_work->wait = 0;
			// 鉄球のなにもしない処理切り替え
//			T_FUNC( gmBoss4ChibiBomb2, obj_work );		

			// ほんの少し動く
			obj_work->move_flag |=	OBD_MOVE_FALL;
			obj_work->move_flag &= ~OBD_MOVE_NOCOL;
			obj_work->move_flag &= ~OBD_MOVE_UNDER;

//			obj_work->spd.y = FX_F32_TO_FX32( -3.0f );
			obj_work->spd.y = FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_SPD_Y_IRON );
			obj_work->pos.y += FX_F32_TO_FX32( GMD_BOSS4_CHIBI_BOUND_ADD_Y );	//(FX32_ONE * -4.0);
//			obj_work->spd_fall = FX_F32_TO_FX32( 0.3f );
			obj_work->spd_fall	=	FX_F32_TO_FX32( GMD_BOSS4_CHIBI_FALL_SPD_IRON );

			return;
		}
#endif	// 鉄球は上に跳ね飛ぶver
		Sint32	b_type = GMD_DWORK_NO_BOSS_04_EF_EGG1_EX_ES;
		switch (chibi_work->type){
		default:
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON:			// 鉄球も風船同様爆発させてみる
		case GME_BOSS4_CHIBI_TYPE_ID_NORMAL:
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND:
			b_type = GMD_DWORK_NO_BOSS_04_EF_EGG1_EX_ES;
			break;
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD:
			b_type = GMD_DWORK_NO_BOSS_04_EF_EGG2_EX_ES;
			break;
		case GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG:
			b_type = GMD_DWORK_NO_BOSS_04_EF_EGG3_EX_ES;
			break;
		}
		// 一回だけ爆破する
		VecFx32	pos = obj_work->pos;
		// 前に出す
		pos.y	+=	FX_F32_TO_FX32(GMD_BOSS4_CHIBI_EXPLOSION_ADD_Y);
		pos.z	=	GMD_BOSS4_CHIBI_EXPLOSION_POS_Z;
		GmBoss4EffCommonInit( b_type, &pos );

		GMS_ENEMY_3D_WORK*	ene_3d	=	(GMS_ENEMY_3D_WORK* )obj_work;
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_ENABLE;
		ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

		obj_work->spd_fall		=	0;
		obj_work->spd.x			=	0;
		obj_work->spd.y			=	0;
		obj_work->move_flag		&= ~OBD_MOVE_UNDER;
		obj_work->move_flag		|=	OBD_MOVE_NOCOL;
		obj_work->move_flag		|=	OBD_MOVE_NOCOL;

		// 爆発時間を0に設定(足しこむ方式)
		chibi_work->wait = 0;
		// 爆発へ処理切り替え
		T_FUNC( gmBoss4ChibiBomb, obj_work );

		// ちびエッグマン爆発SE再生
		GmSoundPlaySE("Boss4_04");
	}
	
	// 方向Update
#if _IPHONE
	GmBoss4UtilUpdateDirection( &chibi_work->dir, obj_work, TRUE);
#else
	GmBoss4UtilUpdateDirection( &chibi_work->dir, obj_work );
#endif // _IPHONE
}


// =======================================================================
// gmBoss4ChibiBomb
/*!
  ちびエッグマン 爆発処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
// 撃破ステート更新 爆発処理
void gmBoss4ChibiBomb(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_CHIBI_WORK	*chibi_work	= (GMS_BOSS4_CHIBI_WORK* )obj_work;

	// 高速スクロール対応
	obj_work->pos.x += GmBoss4GetScrollOffset();

	chibi_work->wait++;

	// 少したったときに本体は消す
	if (chibi_work->wait >= 2){
		// 消す
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}

	// 一定フレームしたらタスクを消す(エフェクトの親の場合の対処として入れてます)
	if (chibi_work->wait < 60) {
		return;
	}
	// 終了
	GMM_BS_OBJ(chibi_work)->flag	|= OBD_OBJECT_TASKCLEAR_REQUEST;
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


void gmBoss4ChibiFuncBoost(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS4_CHIBI_WORK*	parent_body	= (GMS_BOSS4_CHIBI_WORK*)obj_work->parent_obj;
	OBS_OBJECT_WORK*		parent_obj	= (OBS_OBJECT_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_body->node_work.snm_work.reg_node_max);
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}

	// 親が非表示
	if (parent_obj->disp_flag & OBD_DISP_NODISP){
		gmBoss4ChibiBoosterDelete( parent_body );
	}

	obj_work->disp_flag &= ~OBD_DISP_NODISP;
	// 親の角度が中間に存在する
	if (parent_body->dir.cur_angle < AKM_DEGtoA16(50) && parent_body->dir.cur_angle > AKM_DEGtoA16(-50)){
		// オブジェクトを消しておく
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}

	// 本体SNMマトリクスでくっつける
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
									&parent_body->node_work.snm_work,
									parent_body->node_work.work[ GMD_BOSS4_CHIBI_NODE_IDX_BASE ],
									TRUE);

	// ポーズ中はUPDATEしないようにする
	obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
	if (g_obj.flag & OBD_OBJ_PAUSE){
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
	}else{
		// 親の移動値を足す
		obj_work->pos.x += parent_obj->spd.x;
		obj_work->pos.y += parent_obj->spd.y;
		// 高速スクロール分
		obj_work->pos.x += GmBoss4GetScrollOffset();// * FX32_ONE;
	}
}

void gmBoss4ChibiBoosterCreate( GMS_BOSS4_CHIBI_WORK* chibi )
{
#ifdef	GMD_BOSS4_CHIBI_BOOST_ENABLE
	// すでに存在している場合はなにもしない
	if (chibi->boost!=NULL)
		return;

	GMS_EFFECT_3DES_WORK* efct_work;

	switch (chibi->type){
	default:
	case GME_BOSS4_CHIBI_TYPE_ID_NORMAL:
	case GME_BOSS4_CHIBI_TYPE_ID_BOUND:
	case GME_BOSS4_CHIBI_TYPE_ID_BOUND_SPD:
		efct_work	= GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_BOOST1_ES, NULL, (OBS_OBJECT_WORK*)chibi );
		break;
	case GME_BOSS4_CHIBI_TYPE_ID_BOUND_BIG:
		efct_work	= GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_BOOST2_ES, NULL, (OBS_OBJECT_WORK*)chibi );
		break;
	case GME_BOSS4_CHIBI_TYPE_ID_BOUND_IRON:
		// 鉄球の場合は炎なし
		return;
	}

	GMM_BS_OBJ(efct_work)->ppFunc	= NULL;

	GmEffect3DESSetupBase( efct_work, GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND/*GME_EFFECT_3DES_POS_TYPE_EMT*/,
						  /*GMD_EFFECT_3DES_FLAG_NOFLIP |*/ GMD_EFFECT_3DES_FLAG_STICKPARENT );

	// 表示角度設定
	GmEffect3DESSetDispRotation( efct_work, GMD_BOSS4_CHIBI_ABURNER1_TURN_X, 0, 0);


	// 表示オフセット調整
	GmEffect3DESAddDispOffset(efct_work,	GMD_BOSS4_CHIBI_ABURNER1_DISP_OFST_X,
											GMD_BOSS4_CHIBI_ABURNER1_DISP_OFST_Y,
											GMD_BOSS4_CHIBI_ABURNER1_DISP_OFST_Z);

	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss4ChibiFuncBoost;

	chibi->boost = efct_work;

#else	//GMD_BOSS4_CHIBI_BOOST_ENABLE
	UNREFERENCED_PARAMETER(chibi);
#endif	//GMD_BOSS4_CHIBI_BOOST_ENABLE
}

void gmBoss4ChibiBoosterDelete( GMS_BOSS4_CHIBI_WORK* chibi )
{
	if (chibi->boost != NULL){
		// エフェクトワークを指定
		ObjDrawKillAction3DES( (OBS_OBJECT_WORK*)chibi->boost );
		chibi->boost = NULL;
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
