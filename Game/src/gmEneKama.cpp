// ==========================================================================
/*!
  @file gmEneKama.cpp
  @brief エネミー カマキラー

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEneKama.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmEneKama.h"
#include "gmEneKaniPunch.h"		//! ノードシステム
#include "akMath.h"

#include "gmSound.h"

#include "gmGmkAnimal.h"

#include "gmEffect.h"
#include "gmComEfct.h"
#include "gmEffectCmn.h"
#include "gmEffectEnemy.h"

// データヘッダ
#include "common/model/ene_kama_mtn.hmb"
#include "common/model/ene_kama_mdl.hmb"


#if	defined(_WII) && !defined(_PS3)
#pragma warning off (10178)			// 型宣言のプロトタイプのワーニングをOFFにする
									// ただしstatic宣言はつけれないため、問題がある場合は直接修正する
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
#endif	//_WII
//----- Definitions ---------------------------------------------------------

//===========================================================================
//	手の種類
//===========================================================================
enum GME_ENE_KAMA_HAND_TYPE{
			GME_ENE_KAMA_HAND_TYPE_RIGHT = 0,								//!< 右手
			GME_ENE_KAMA_HAND_TYPE_LEFT,									//!< 左手

			GME_ENE_KAMA_HAND_TYPE_MAX,
};

//===========================================================================
//	ツール設定でのイベント関係
//===========================================================================
//			GMS_EVE_RECORD_EVENT.flag
#define		GMD_ENE_KAMA_EVE_FLAG_RIGHT					(0x0001)			//!< 右向き開始
#define		GMD_ENE_KAMA_EVE_FLAG_UP					(0x0002)			//!< 上張り付き開始
#define		GMD_ENE_KAMA_EVE_FLAG_WAIT0					(0x0004)			//!< カマ振り上げ WAIT 10フレーム
#define		GMD_ENE_KAMA_EVE_FLAG_WAIT1					(0x0008)			//!< カマ振り上げ WAIT 20フレーム
#define		GMD_ENE_KAMA_EVE_FLAG_WAIT2					(0x0010)			//!< カマ振り上げ WAIT 30フレーム
#define		GMD_ENE_KAMA_EVE_FLAG_SMALL_WALK			(0x0020)			//!< あたふた(小)

//===========================================================================
//	反転関係
//===========================================================================
#define		GMD_ENE_KAMA_MOVE_SPD_X						(0x0800)			//!< 移動速度
#define		GMD_ENE_KAMA_FW_TIME						(15*FX32_ONE)		//!< FW時間
#define		GMD_ENE_KAMA_TURN_FRAME						(40)				//!< ターンモーションフレーム

//===========================================================================
//	アクション関係
//===========================================================================
#define		GMD_ENE_KAMA_SEARCH_LENGTH					(112)				//!< カマを飛ばすための距離
#define		GMD_ENE_KAMA_ATTACK_WAIT_TIME				(60)				//!< 振りかぶってためモーションまでの時間
#define		GMD_ENE_KAMA_ATTACK_TIME					(7)					//!< 攻撃モーションからカマが飛ぶまでのフレーム

#define		GMD_ENE_KAMA_HAND_SPD						(1.75f)				//!< 飛ぶカマのスピード
#define		GMD_ENE_KAMA_HAND_HOMING_RET				(0.08f)				//!< 飛ぶカマのホーミング能力
#define		GMD_ENE_KAMA_HAND_HOMING					(0.04f)				//!< 飛ぶカマのホーミング能力
#define		GMD_ENE_KAMA_HAND_ROLING					(15)				//!< カマの回転角度(スピード)
#define		GMD_ENE_KAMA_HAND_CENTER_X					(10.0f)				//!< カマの回転の中心位置
#define		GMD_ENE_KAMA_HAND_CENTER_Y					(-10.0f)			//!< カマの回転の中心位置
#define		GMD_ENE_KAMA_HAND_TIME						(60*2)				//!< カマが飛んでいる時間
#define		GMD_ENE_KAMA_HAND_G_SPD						(0.2f)				//!< カマが落ちていく重力加速度

//===========================================================================
//	ノード関係
//===========================================================================
#define		GMD_ENE_KAMA_L_HAND_NODE					(9)				//!< 左手ノード番号
#define		GMD_ENE_KAMA_R_HAND_NODE					(6)				//!< 右手ノード番号

#define		GMD_ENE_KAMA_NODE_NO_MAX					(32)				//!< 上の２つより大きな数が必要
//===========================================================================
//	サウンド関係
//===========================================================================
#define		GMD_ENE_KAMA_SE_KAMA						"Kama"

#define		GMD_ENE_KAMA_SE_BOMB						"Boss2_03"

//===========================================================================
/// カマキラーワーク
//===========================================================================

/// フェードアニメーションパターンデータ
typedef struct tag_GMS_ENE_KAMA_FADE_ANIME_PAT {
	NNS_RGB		col;
	float		intensity;
	fx32		frame;
} GMS_ENE_KAMA_FADE_ANIME_PAT;

/// フェードアニメーションデータ
typedef struct tag_GMS_ENE_KAMA_FADE_ANIME {
	u32								pat_num;	//!< パターン数
	GMS_ENE_KAMA_FADE_ANIME_PAT		*anime_pat;	//!< アニメーションパターンデータ
} GMS_ENE_KAMA_FADE_ANIME;


typedef struct tag_GMS_ENE_KAMA_WORK {
	GMS_ENEMY_3D_WORK		ene_3d_work;

	fx32					spd_dec;			//!< 減速用速度
	fx32					spd_dec_dist;		//!< 減速開始距離

	GME_ENE_KAMA_HAND_TYPE	hand;				//!< 手の種類

	GMS_ENE_NODE_MATRIX		node_work;

	BOOL					attack;				//!< 攻撃判定
	Sint32					timer;

	Angle32					rot_z;
	Angle32					rot_z_add;			//!< 毎フレーム足しこまれる

	Sint32					atk_wait;
	BOOL					walk_s;
	Sint32					ata_futa;

	GMS_ENE_KAMA_FADE_ANIME	*anime_data;
	u32						anime_pat_no;
	fx32					anime_frame;

} GMS_ENE_KAMA_WORK;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------
// 現在gmEneKaniPunchにある(問題なければgmEneComに移動する)
extern void			GmEneUtilInitNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work, OBS_OBJECT_WORK* obj_work, Sint32 max_node );
extern NNS_MATRIX*	GmEneUtilGetNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work, Sint32 node_id );
extern void			GmEneUtilExitNodeMatrix	( GMS_ENE_NODE_MATRIX* node_work );
extern void			GmEneUtilSetMatrixNN	( OBS_OBJECT_WORK* obj_work, NNS_MATRIX*	w_mtx );

//----- Static Declarations -------------------------------------------------
// 点滅
static void		gmEneKamaFadeAnimeSet(GMS_ENE_KAMA_WORK *kama_work, GMS_ENE_KAMA_FADE_ANIME *anime_data);
static void		gmEneKamaFadeAnimeUpdate(GMS_ENE_KAMA_WORK *kama_work, fx32 speed, BOOL repeat);

// -- 本体関係 ---
static void		gmEneKamaWalkInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneKamaWalkMain(OBS_OBJECT_WORK *obj_work);
//static void	gmEneKamaFwInit(OBS_OBJECT_WORK *obj_work);
//static void	gmEneKamaFwMain(OBS_OBJECT_WORK *obj_work);
static void		gmEneKamaFlipInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneKamaFlipMain(OBS_OBJECT_WORK *obj_work);

static void		gmEneKamaAttackInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneKamaAttackPreMain(OBS_OBJECT_WORK *obj_work);
static void		gmEneKamaAttackMain(OBS_OBJECT_WORK *obj_work);
static void		gmEneKamaAttackWait(OBS_OBJECT_WORK *obj_work);

static void		gmEneKamaFlipAtafuta(OBS_OBJECT_WORK *obj_work);

static void		gmEneKamaFlashInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneKamaFlashMain(OBS_OBJECT_WORK *obj_work);
static void		gmEneKamaFlashEnd(OBS_OBJECT_WORK *obj_work);


static void		gmEneKamaExit(MTS_TASK_TCB *tcb);
static BOOL		gmEneKamaSetWalkSpeed(GMS_ENE_KAMA_WORK *kama_work);

// -- カマ関係 ---
static OBS_OBJECT_WORK* gmEneKamaHandInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, GME_ENE_KAMA_HAND_TYPE type);

static void		gmEneKamaHandWaitInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneKamaHandWaitMain(OBS_OBJECT_WORK *obj_work);

static void		gmEneKamaHandAttackInit(OBS_OBJECT_WORK *obj_work);
static void		gmEneKamaHandAttackMain(OBS_OBJECT_WORK *obj_work);


//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_ene_kama_obj_3d_list = NULL;

// 点滅パターン
static GMS_ENE_KAMA_FADE_ANIME_PAT	gm_ene_kama_blink_anime_pat[] = {
	{{1.f, 0.f, 0.f}, 0.02f, 30*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.8f, 4*FX32_ONE},

	{{1.f, 0.f, 0.f}, 0.05f, 30*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.8f, 4*FX32_ONE},

	{{1.f, 0.f, 0.f}, 0.1f, 20*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.8f, 4*FX32_ONE},

	{{1.f, 0.f, 0.f}, 0.15f, 20*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.8f, 4*FX32_ONE},

	{{1.f, 0.f, 0.f}, 0.15f, 10*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.8f, 4*FX32_ONE},

	{{1.f, 0.f, 0.f}, 0.2f, 10*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.8f, 4*FX32_ONE},

	{{1.f, 0.f, 0.f}, 0.3f, 5*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.9f, 4*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.3f, 5*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.9f, 4*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.3f, 5*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.9f, 4*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.3f, 5*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.9f, 4*FX32_ONE},

	{{1.f, 0.f, 0.f}, 0.4f, 2*FX32_ONE},
	{{1.f, 0.f, 0.f}, 1.0f, 4*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.4f, 2*FX32_ONE},
	{{1.f, 0.f, 0.f}, 1.0f, 4*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.4f, 2*FX32_ONE},
	{{1.f, 0.f, 0.f}, 1.0f, 4*FX32_ONE},
	{{1.f, 0.f, 0.f}, 0.4f, 2*FX32_ONE},
	{{1.f, 0.f, 0.f}, 1.0f, 4*FX32_ONE},


};

static GMS_ENE_KAMA_FADE_ANIME gm_ene_kama_blink_anime = {
	sizeof(gm_ene_kama_blink_anime_pat) / sizeof(GMS_ENE_KAMA_FADE_ANIME_PAT),
	gm_ene_kama_blink_anime_pat,
};


//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmEneKamaBuild
/*!
 *	エネミー カマキラー データ構築
 */
// ==========================================================================
void GmEneKamaBuild(void)
{
	gm_ene_kama_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_KAMA_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_KAMA_TEX),
								NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
}

// ==========================================================================
// GmEneKamaFlush
/*!
 *	エネミー カマキラー データ片付け
 */
// ==========================================================================
void GmEneKamaFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetEnemyData(GMD_DWORK_NO_ENEMY_KAMA_MODEL);

	GmGameDBuildRegFlushModel(gm_ene_kama_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmEneKamaInit
/*!
 *	エネミー カマキラー 初期化関数
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
OBS_OBJECT_WORK* GmEneKamaInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_KAMA_WORK	*kama_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_KAMA_WORK), "ENE_KAMA");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	kama_work = (GMS_ENE_KAMA_WORK*)obj_work;


	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_ene_kama_obj_3d_list[IDB_ENE_KAMA_MDL_ENE_KAMA_ZNO],
					&ene_work->obj_3d);

	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_KAMA_MTN), NULL/*mtn_data_path*/,
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
	ObjObjectFieldRectSet(obj_work, -4, -8, 4, +0);

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	obj_work->move_flag |= OBD_MOVE_FALL;

	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_KAMA_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}
	// 上設定
	if ((eve_rec->flag & GMD_ENE_KAMA_EVE_FLAG_UP)) {
		obj_work->disp_flag |= OBD_DISP_VFLIP;
		obj_work->move_flag &= ~OBD_MOVE_FALL;
		obj_work->dir.z = AKM_DEGtoA16( 180 );
		obj_work->disp_flag ^= OBD_DISP_HFLIP;
	}
	// ウエイトフラグ設定
	kama_work->atk_wait = 0;
	if ((eve_rec->flag & GMD_ENE_KAMA_EVE_FLAG_WAIT0)) {
		kama_work->atk_wait += 10;
	}
	if ((eve_rec->flag & GMD_ENE_KAMA_EVE_FLAG_WAIT1)) {
		kama_work->atk_wait += 20;
	}
	if ((eve_rec->flag & GMD_ENE_KAMA_EVE_FLAG_WAIT2)) {
		kama_work->atk_wait += 30;
	}

	kama_work->walk_s = FALSE;
	if ((eve_rec->flag & GMD_ENE_KAMA_EVE_FLAG_SMALL_WALK)) {
		kama_work->walk_s = TRUE;
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
	kama_work->spd_dec		 = GMD_ENE_KAMA_MOVE_SPD_X / (GMD_ENE_KAMA_TURN_FRAME/2);
	kama_work->spd_dec_dist = GMD_ENE_KAMA_MOVE_SPD_X * (GMD_ENE_KAMA_TURN_FRAME/2) / 2;

	gmEneKamaWalkInit(obj_work);


	// 攻撃判定はなし
	kama_work->attack = FALSE;


	// ノードシステムを使用する
	GmEneUtilInitNodeMatrix( &kama_work->node_work, obj_work, GMD_ENE_KAMA_NODE_NO_MAX );
	// ノードシステムを使用するため開放処理を変える(できればGmEneUtilInitNodeMatrixで設定したいが…workの形が個別になるため現在厳しい)
	mtTaskChangeTcbDestructor(obj_work->tcb, gmEneKamaExit);

	// 初期登録
	GmEneUtilGetNodeMatrix( &kama_work->node_work, GMD_ENE_KAMA_L_HAND_NODE );
	GmEneUtilGetNodeMatrix( &kama_work->node_work, GMD_ENE_KAMA_R_HAND_NODE );


	OBS_OBJECT_WORK*	obj_hand;
	// 左手
	obj_hand		= GmEventMgrLocalEventBirth(GMD_EVENT_ID_KAMA_LEFT_HAND,
											obj_work->pos.x, obj_work->pos.y,
											0,//flag
											0,0,0,0,
											0);
	obj_hand->parent_obj = obj_work;

	// 右手
	obj_hand		= GmEventMgrLocalEventBirth(GMD_EVENT_ID_KAMA_RIGHT_HAND,
											obj_work->pos.x, obj_work->pos.y,
											0,//flag
											0,0,0,0,
											0);
	obj_hand->parent_obj = obj_work;

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;

#if OBD_OBJECT_USE_NOEXIST
	obj_work->flag |= OBD_OBJECT_NOEXIST_ENABLE;
#endif // OBD_OBJECT_USE_NOEXIST
#endif // _IPHONE

	return (obj_work);
}


// ==========================================================================
// GmEneKamaHandInit
/*!
 *	エネミー カマキラーの手 初期化関数
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
OBS_OBJECT_WORK* GmEneKamaLeftHandInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	// 左手生成
	return gmEneKamaHandInit( eve_rec ,pos_x, pos_y, GME_ENE_KAMA_HAND_TYPE_LEFT );
}
OBS_OBJECT_WORK* GmEneKamaRightHandInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	// 右手生成
	return gmEneKamaHandInit( eve_rec ,pos_x, pos_y, GME_ENE_KAMA_HAND_TYPE_RIGHT );
}



//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmEneKamaGetLength2N()
/*!
 *	プレイヤーとの距離の自乗を求める
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
Sint32	gmEneKamaGetLength2N( OBS_OBJECT_WORK* obj_work )
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
// gmEneKamaIsPlayerAttack()
/*!
 *	プレイヤーが前にいるか
 *
 */
// ==========================================================================
BOOL gmEneKamaIsPlayerFront( OBS_OBJECT_WORK* obj_work )
{
	GMS_PLAYER_WORK		*ply_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	// 縦反転している?
	if (obj_work->disp_flag & OBD_DISP_VFLIP){

		// 条件が正反対になる
		if (!(obj_work->disp_flag & OBD_DISP_HFLIP)){
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

	}else{

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
}

// ==========================================================================
// gmEneKamaGetPlayerVector()
/*!
 *	プレイヤーの方向を返す(正規化ずみベクトル)
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
VecFx32	gmEneKamaGetPlayerVectorFx( OBS_OBJECT_WORK* obj_work )
{
	VecFx32				normal;

	GMS_PLAYER_WORK		*ply_work;
	ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];

	fx32	px = ply_work->obj_work.pos.x - obj_work->pos.x;
	fx32	py = ply_work->obj_work.pos.y - obj_work->pos.y;

	// プレイヤーがいない
	if ( px > FX_F32_TO_FX32( 1000 ) || px < FX_F32_TO_FX32( -1000 )){
		px = 1000;
	}
	if ( py > FX_F32_TO_FX32( 1000 ) || py < FX_F32_TO_FX32( -1000 )){
		py = 1000;
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
// gmEneKamaGetParentVector()
/*!
 *	プレイヤーの方向を返す(正規化ずみベクトル)
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
VecFx32	gmEneKamaGetParentVectorFx( OBS_OBJECT_WORK* obj_work )
{
	VecFx32				normal;

//	GMS_ENE_KAMA_WORK*	hand_work = (GMS_ENE_KAMA_WORK*)obj_work;
	GMS_ENE_KAMA_WORK*	kama_work = (GMS_ENE_KAMA_WORK*)obj_work->parent_obj;

	fx32	px, py;
	if (kama_work==NULL){
		// こんなことは無いが・・・。
		px = 1000;
		py = 1000;
	}else{
		px = kama_work->ene_3d_work.ene_com.obj_work.pos.x - obj_work->pos.x;
		py = kama_work->ene_3d_work.ene_com.obj_work.pos.y - obj_work->pos.y;
	}

	// プレイヤーがいない
	if ( px > FX_F32_TO_FX32( 1000 ) || px < FX_F32_TO_FX32( -1000 )){
		px = 1000;
	}
	if ( py > FX_F32_TO_FX32( 1000 ) || py < FX_F32_TO_FX32( -1000 )){
		py = 1000;
	}

	fx32 len = FX_F32_TO_FX32( sqrt(FX_FX32_TO_F32(px) * FX_FX32_TO_F32(px) + FX_FX32_TO_F32(py) + FX_FX32_TO_F32(py)) );
	
//	fx32	len = FX_Mul( px, px ) + FX_Mul( py, py );
//	len = FX_Sqrt(len);

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
// gmEneKamaHandInit
/*!
 *	エネミー カマキラーの手 初期化関数
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
OBS_OBJECT_WORK* gmEneKamaHandInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, GME_ENE_KAMA_HAND_TYPE type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*ene_work;
	OBS_RECT_WORK		*rect_work;
	GMS_ENE_KAMA_WORK	*kama_work;

	//UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENE_KAMA_WORK), "ENE_KAMA");
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;
	kama_work = (GMS_ENE_KAMA_WORK*)obj_work;


	// モデル初期化
	if (type == GME_ENE_KAMA_HAND_TYPE_LEFT){
		// 左手
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_ene_kama_obj_3d_list[IDB_ENE_KAMA_MDL_ENE_KAMA_LSW_ZNO],
						&ene_work->obj_3d);
	}else{
		// 右手
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_ene_kama_obj_3d_list[IDB_ENE_KAMA_MDL_ENE_KAMA_RSW_ZNO],
						&ene_work->obj_3d);
	}
#if 0
	// モーション初期化
	ObjObjectAction3dNNMotionLoad(obj_work, 0/*reg_file_id*/, TRUE/*marge*/,
									ObjDataGet(GMD_DWORK_NO_ENEMY_KAMA_MTN), NULL/*mtn_data_path*/,
									0/*index*/, NULL/*archive*/);
#endif//
	// トゥーン設定
	ObjDrawObjectSetToon(obj_work);

	// 優先設定
	obj_work->pos.z = GMD_OBJ_ENEMY_POS_Z;

	kama_work->ene_3d_work.ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOHOMING;

//#if 0
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
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// くらい
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
	//rect_work->ppDef = gmGmkSpringDefFunc;
	//rect_work->ppHit = NULL;
	//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
	//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, -32, 19, 0);
	//rect_work->flag |= OBD_RECT_OUT;
	rect_work->flag &= ~OBD_RECT_ENABLE;

	ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_ENABLE;

	// ボディ
	rect_work = &ene_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];
	// 仮矩形
	ObjRectWorkSet(rect_work, -19, -32, 19, 0);
	rect_work->flag &= ~OBD_RECT_ENABLE;

	// 地形あたり
	//ObjObjectFieldRectSet(obj_work, -4, -8, 4, -2);
//#endif

	// フラグ
	//obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	//obj_work->disp_flag |= OBD_DISP_NODIRFLIP;
	//obj_work->move_flag |= OBD_MOVE_FALL;

//	obj_work->move_flag |= OBD_MOVE_NOMOVE;	// 移動無し
	obj_work->move_flag |= OBD_MOVE_NOCOL;	// 地形あたり無し
	obj_work->move_flag &= ~OBD_MOVE_FALL;	// 重力なし
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

//	obj_work->disp_flag |= OBD_DISP_NOOFST;
//	obj_work->disp_flag |= OBD_DISP_3D_COORDINATE;

	/*
	// 方向設定
	if (!(eve_rec->flag & GMD_ENE_KAMA_EVE_FLAG_RIGHT)) {
		obj_work->disp_flag |= OBD_DISP_HFLIP;
	}

	// 移動限界値取得
	obj_work->user_work = (u32)(obj_work->pos.x + (eve_rec->left<<FX32_SHIFT));
	obj_work->user_flag = (u32)(obj_work->pos.x + ((eve_rec->left + eve_rec->width)<<FX32_SHIFT));

	// 減速処理設定
	kama_work->spd_dec		 = GMD_ENE_KAMA_MOVE_SPD_X / (GMD_ENE_KAMA_TURN_FRAME/2);
	kama_work->spd_dec_dist = GMD_ENE_KAMA_MOVE_SPD_X * (GMD_ENE_KAMA_TURN_FRAME/2) / 2;

	gmEneKamaWalkInit(obj_work);
	*/

	kama_work->hand = type;

	// 手にくっつけてみる
	gmEneKamaHandWaitInit(obj_work);

#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return (obj_work);
}

// =======================================================================
// gmEneKamaExit
/*!
  本体終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void gmEneKamaExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK*	obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_ENE_KAMA_WORK*	kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;
	
	GmEneUtilExitNodeMatrix( &kama_work->node_work );
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}

// ==========================================================================
// gmEneKamaWalkInit
/*!
 *	エネミー カマキラー Walk初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaWalkInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_MOVE_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_MOVE_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_MOVE_ZNM);

	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneKamaWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_KAMA_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_KAMA_MOVE_SPD_X;
	}
	// 移動範囲がない
	if (obj_work->user_flag == obj_work->user_work){
		obj_work->spd.x = 0;
		// フットワークにする
		GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_FW_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_FW_L_ZNM);
		obj_work->disp_flag |= OBD_DISP_REPEAT;
	}

	//(obj_work->disp_flag & OBD_DISP_VFLIP)

}

// ==========================================================================
// gmEneKamaWalkMain
/*!
 *	エネミー カマキラー Walk メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaWalkMain(OBS_OBJECT_WORK *obj_work)
{
#if 1
	BOOL				b_dec;
	GMS_ENE_KAMA_WORK	*kama_work;

	kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;

	// 攻撃判定
	// プレイヤーが前にいる?
	if (gmEneKamaIsPlayerFront( obj_work )){
		// 距離が範囲内?
		if (gmEneKamaGetLength2N( obj_work ) <= GMD_ENE_KAMA_SEARCH_LENGTH * GMD_ENE_KAMA_SEARCH_LENGTH ){
			//攻撃に移行する
			obj_work->ppFunc = gmEneKamaAttackInit;
			return;
		}
	}

	// 上重力加速度
	if (obj_work->disp_flag &  OBD_DISP_VFLIP){
		obj_work->move_flag &= ~OBD_MOVE_FALL;
		// マップの上に当たったら移動終了
		if (!(obj_work->move_flag & OBD_MOVE_UNDER)){
			obj_work->spd.y -= obj_work->spd_fall;
		}else{
			obj_work->spd.y = 0;
		}
	}

	// 移動範囲がない
	if (obj_work->user_flag == obj_work->user_work){
#if 0	// プレイヤーの方向を向く 10/29
		if (!gmEneKamaIsPlayerFront( obj_work )){
			// フリップへ移行
			gmEneKamaFlipInit(obj_work);
		}

#endif
		// なにもせずに終了
		return;
	}


	b_dec = gmEneKamaSetWalkSpeed(kama_work);

	if (b_dec) {
		// 減速を開始した

		// フリップへ移行
		gmEneKamaFlipInit(obj_work);
	}
#else
	if (obj_work->move_flag & OBD_MOVE_FRONT ||
			!GmEneComCheckMoveLimit(obj_work, (fx32)obj_work->user_work, (fx32)obj_work->user_flag)) {
		// 前方衝突・移動範囲外

		// フリップへ移行
		gmEneKamaFlipInit(obj_work);
		// FWへ移行
		//gmEneKamaFwInit(obj_work);
	}
#endif

}


#if 0
// ==========================================================================
// gmEneKamaFwInit
/*!
 *	エネミー カマキラー FW初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_FW_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_FW_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_FW_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneKamaFwMain;

	// 移動速度設定
	obj_work->spd.x = 0;

	// 演出時間
	obj_work->user_timer = GMD_ENE_KAMA_FW_TIME;
}
#endif

// ==========================================================================
// gmEneKamaFwMain
/*!
 *	エネミー カマキラー FW メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaFwMain(OBS_OBJECT_WORK *obj_work)
{
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	if (obj_work->user_timer <= 0) {
		// フリップへ移行
		gmEneKamaFlipInit(obj_work);
	}
}



// ==========================================================================
// gmEneKamaFlipInit
/*!
 *	エネミー カマキラー フリップ初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaFlipInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	obj_work->obj_3d->blend_spd	= 0.1f;
	// アクション設定
	//GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_FRIP_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_FRIP_L_ZNM);
	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_FRIP_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_FRIP_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_FRIP_ZNM);

	// メイン処理
	obj_work->ppFunc = gmEneKamaFlipMain;

	// 移動速度設定
	obj_work->spd.x = 0;
}

// ==========================================================================
// gmEneKamaFlipMain
/*!
 *	エネミー カマキラー フリップ メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaFlipMain(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;

		obj_work->obj_3d->blend_spd	= 1.0f;
		// 移動速度設定
		gmEneKamaSetWalkSpeed((GMS_ENE_KAMA_WORK*)obj_work);
		// Walkへ
		gmEneKamaWalkInit(obj_work);
	}
}



// ==========================================================================
// gmEneKamaAttackInit
/*!
 *	エネミー カマキラー Attack初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaAttackInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_KAMA_WORK	*kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;
//	GMS_ENEMY_3D_WORK	*ene_work	= (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_ATK_ST_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_ATK_ST_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_MOVE_ZNM);
	obj_work->disp_flag &= ~OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneKamaAttackPreMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// とまる
	obj_work->spd.x = 0;

	kama_work->timer = kama_work->atk_wait;		//GMD_ENE_KAMA_ATTACK_WAIT_TIME;	// TODO PAL


}

// ==========================================================================
// gmEneKamaAttackPreMain
/*!
 *	エネミー カマキラー Attackため動作
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaAttackPreMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_KAMA_WORK	*kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;
//	GMS_ENEMY_3D_WORK	*ene_work	= (GMS_ENEMY_3D_WORK*)obj_work;

	// カマモーションで待つ
	if (obj_work->disp_flag & OBD_DISP_END) {
		if (kama_work->timer > 0){
			kama_work->timer--;
			return;
		}
	}else{
		return;
	}

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_ATK_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_ATK_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_MOVE_ZNM);
	obj_work->disp_flag &= ~OBD_DISP_REPEAT;
	// メイン処理
	obj_work->ppFunc = gmEneKamaAttackMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// とまる
	obj_work->spd.x = 0;

	kama_work->timer = GMD_ENE_KAMA_ATTACK_TIME;	// TODO PAL
}


// ==========================================================================
// gmEneKamaAttackMain
/*!
 *	エネミー カマキラー Attack メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaAttackMain(OBS_OBJECT_WORK *obj_work)
{
	// 途中のフレームで攻撃判定を出す
	// TODO PAL

	GMS_ENE_KAMA_WORK	*kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;
	
	if (kama_work->timer>0){
		kama_work->timer--;
		return;
	}
	kama_work->attack = TRUE;

	// 待つ処理に変更
	obj_work->ppFunc = gmEneKamaAttackWait;
}

void gmEneKamaAttackWait(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_KAMA_WORK	*kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;
/*
	// カマが戻ってきたら通常処理にする
	if (!kama_work->attack){
		gmEneKamaWalkInit( obj_work );
	}
*/
	if (kama_work->ata_futa)
	{
		// あたふた移動中
		if (kama_work->timer>0){
			kama_work->timer--;
			return;
		}else{

			// プレイヤーが近づいた?
			if (gmEneKamaGetLength2N( obj_work ) <= GMD_ENE_KAMA_SEARCH_LENGTH * GMD_ENE_KAMA_SEARCH_LENGTH ){
				//攻撃に移行する
				obj_work->ppFunc = gmEneKamaFlashInit;
				return;
			}

			// モーションのスピードを２倍にする
			obj_work->obj_3d->speed[0] = 2.0f;
			// Flipへ

			// アクション設定
			GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_FRIP_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_FRIP_L_ZNM);
			obj_work->disp_flag &= ~OBD_DISP_REPEAT;

			// メイン処理
			obj_work->ppFunc = gmEneKamaFlipAtafuta;

			// 移動速度設定
			obj_work->spd.x = 0;

			if (kama_work->walk_s){
				kama_work->timer = 15;
			}else{
				kama_work->timer = 10 + mtMathRand() % 20;
			}
		}
	}

}

// ==========================================================================
// gmEneKamaAtafuta
/*!
 *	エネミー カマキラー あたふた メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaFlipAtafuta(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END) {
		// フリップ終了
		obj_work->disp_flag ^= OBD_DISP_HFLIP;

		// 移動速度設定
		gmEneKamaSetWalkSpeed((GMS_ENE_KAMA_WORK*)obj_work);

		// Walkへ
		GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_MOVE_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_MOVE_L_ZNM);
		obj_work->disp_flag |= OBD_DISP_REPEAT;

		// 移動速度設定
		if (obj_work->disp_flag & OBD_DISP_HFLIP) {
			obj_work->spd.x = -GMD_ENE_KAMA_MOVE_SPD_X;
		}
		else {
			obj_work->spd.x = GMD_ENE_KAMA_MOVE_SPD_X;
		}

		obj_work->ppFunc =	gmEneKamaAttackWait;
	}
}

// ==========================================================================
// gmEneKamaFlashInit
/*!
 *	エネミー カマキラー 最後の攻撃処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaFlashInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_KAMA_WORK	*kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;

	obj_work->spd.x = 0;

	// モーションのスピードを戻す
	obj_work->obj_3d->speed[0] = 1.0f;

	// フェードクリア
	GmBsCmnClearObject3DNNFadedColor(obj_work);
	obj_work->disp_flag |= OBD_DISP_DRAWSTATE;
	// フェードアニメーション用意
	gmEneKamaFadeAnimeSet( kama_work, &gm_ene_kama_blink_anime );

	obj_work->obj_3d->blend_spd	= 0.125f;
	// フットワークにしてとめる
	GmEneComActionSet3DNNBlendDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_END_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_END_L_ZNM);
//	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_END_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_END_L_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// フェードアニメーションクリア
//	GmBsCmnClearObject3DNNFadedColor(obj_work);
//	obj_work->disp_flag &= ~OBD_DISP_DRAWSTATE;

	// 3秒の猶予。
	kama_work->timer = 3 * 60;

	obj_work->ppFunc =	gmEneKamaFlashMain;
}

// ==========================================================================
// gmEneKamaFlashMain
/*!
 *	エネミー カマキラー 最後の攻撃処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaFlashMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_KAMA_WORK	*kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;
	/*
	// とめアクション(10フレームは補完)
	if (kama_work->timer < 3*60-10){
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
	}

	// 少し縮小する
	obj_work->scale.x -= FX_F32_TO_FX32( 0.0003f );
	obj_work->scale.y -= FX_F32_TO_FX32( 0.0003f );
	obj_work->scale.z -= FX_F32_TO_FX32( 0.0003f );
	*/
	// フェードアニメーション更新
	gmEneKamaFadeAnimeUpdate((GMS_ENE_KAMA_WORK*)obj_work, FX32_ONE, TRUE);

	if (kama_work->timer--<0){
		// 爆発
		GMS_EFFECT_3DES_WORK* eff_work = GmEfctCmnEsCreate( obj_work, GME_EFCT_CMN_IDX_BOMB_KAMA );
//		GMS_EFFECT_3DES_WORK* eff_work = GmEfctEneEsCreate( obj_work, GME_EFCT_ENE_IDX_E13_T_STAR );

		if (obj_work->disp_flag & OBD_DISP_VFLIP){
			GmComEfctSetDispOffsetF( eff_work, 0.f,  20.f, 0.f);
		}else{
			GmComEfctSetDispOffsetF( eff_work, 0.f, -20.f, 0.f);
		}

		// 死亡フラグは立てる
		kama_work->ene_3d_work.ene_com.enemy_flag |= GMD_ENEMY_FLAG_DIE;

		kama_work->timer = 3 * 60;

		// 仮サウンド(テルスターのを当てておく)
		// 爆発SE
		GmSoundPlaySE( GMD_ENE_KAMA_SE_BOMB );

		// 表示は消す
		obj_work->disp_flag |= OBD_DISP_NODISP;

		obj_work->ppFunc =	gmEneKamaFlashEnd;

		// 一瞬だけ大きな当たりを作る
		OBS_RECT_WORK*	rect_work = &kama_work->ene_3d_work.ene_com.rect_work[GMD_ENEMY_RECT_ATK];
		// 仮矩形
		ObjRectWorkSet(rect_work, -30, -30, 30, 10);
		//rect_work->flag |= OBD_RECT_OUT;
		rect_work->flag |= OBD_RECT_ENABLE;

		// 動物生成
		GmGmkAnimalInit( obj_work, 0, 0, 0, 0, 0, 0);

		// 体当たりをなくす
		rect_work = &kama_work->ene_3d_work.ene_com.rect_work[GMD_ENEMY_RECT_DEF];
		rect_work->flag &= ~OBD_RECT_ENABLE;
		rect_work = &kama_work->ene_3d_work.ene_com.rect_work[GMD_ENEMY_RECT_BODY];
		rect_work->flag &= ~OBD_RECT_ENABLE;
	}
}

// ==========================================================================
// gmEneKamaFlashMain
/*!
 *	エネミー カマキラー 最後の攻撃処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaFlashEnd(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_KAMA_WORK	*kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;

	OBS_RECT_WORK*	rect_work = &kama_work->ene_3d_work.ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	rect_work->flag &= ~OBD_RECT_ENABLE;


	// 爆発分待って終了
	if (kama_work->timer--<0){

		// いなくなる
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR_REQUEST;
		
	}

}


// ==========================================================================
// 速度設定
// ==========================================================================
// ==========================================================================
// gmEneKamaSetWalkSpeed
/*!
 *	エネミー カマキラー 矩形登録処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
BOOL gmEneKamaSetWalkSpeed(GMS_ENE_KAMA_WORK *kama_work)
{
	BOOL			b_dec = FALSE;
	OBS_OBJECT_WORK	*obj_work = (OBS_OBJECT_WORK*)kama_work;

	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// 左向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_KAMA_MTN_ENE_KAMA_FRIP_L_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_KAMA_TURN_FRAME/2) {
			// 右への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, kama_work->spd_dec, GMD_ENE_KAMA_MOVE_SPD_X);
		}
		else if (obj_work->pos.x <= ((fx32)obj_work->user_work + kama_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, kama_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x > (fx32)obj_work->user_work)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_work - obj_work->pos.x;
				if (obj_work->spd.x < -kama_work->spd_dec) {
					obj_work->spd.x = -kama_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x > -GMD_ENE_KAMA_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -kama_work->spd_dec, GMD_ENE_KAMA_MOVE_SPD_X);
		}
	}
	else {
		// 右向き
		if (obj_work->obj_3d->act_id[0] == IDB_ENE_KAMA_MTN_ENE_KAMA_FRIP_ZNM
					&& obj_work->obj_3d->frame[0] >= GMD_ENE_KAMA_TURN_FRAME/2) {
			// 左への加速状態に
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, -kama_work->spd_dec, GMD_ENE_KAMA_MOVE_SPD_X);
		}
		else if (obj_work->pos.x >= ((fx32)obj_work->user_flag - kama_work->spd_dec_dist)) {
			obj_work->spd.x = ObjSpdDownSet(obj_work->spd.x, kama_work->spd_dec);
			b_dec = TRUE;

			if (!obj_work->spd.x && (obj_work->pos.x < (fx32)obj_work->user_flag)) {
				// 減速終了時に距離が残っていた場合
				obj_work->spd.x = (fx32)obj_work->user_flag - obj_work->pos.x;
				if (obj_work->spd.x > kama_work->spd_dec) {
					obj_work->spd.x = kama_work->spd_dec;
				}
			}
		}
		else if (obj_work->spd.x < GMD_ENE_KAMA_MOVE_SPD_X) {
			// 加速
			obj_work->spd.x = ObjSpdUpSet(obj_work->spd.x, kama_work->spd_dec, GMD_ENE_KAMA_MOVE_SPD_X);
		}
	}

	return (b_dec);
}



// ==========================================================================
// gmEneKamaHandWaitInit
/*!
 *	エネミー カマキラー 手の初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaHandWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_KAMA_WORK*kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;

	kama_work->rot_z = 0;

	OBS_RECT_WORK		*rect_work;
	rect_work = &kama_work->ene_3d_work.ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	// 仮矩形
	rect_work->flag &= ~OBD_RECT_ENABLE;

	obj_work->ofst.x = 0;
	obj_work->ofst.y = 0;
	obj_work->dir.z = 0;

	// 親に戻す
	obj_work->flag		&= ~OBD_OBJECT_PARENT_NODIE;				// 親が死んでも生き抜く

	/*
	GMS_ENEMY_3D_WORK	*ene_work;
	ene_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// アクション設定
	GmEneComActionSetDependHFlip(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_MOVE_ZNM, IDB_ENE_KAMA_MTN_ENE_KAMA_MOVE_L_ZNM);
	//ObjDrawObjectActionSet(obj_work, IDB_ENE_KAMA_MTN_ENE_KAMA_MOVE_ZNM);
	obj_work->disp_flag |= OBD_DISP_REPEAT;

	// メイン処理
	obj_work->ppFunc = gmEneKamaWalkMain;

	obj_work->move_flag &= ~OBD_MOVE_FRONT;

	// 移動速度設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		obj_work->spd.x = -GMD_ENE_KAMA_MOVE_SPD_X;
	}
	else {
		obj_work->spd.x = GMD_ENE_KAMA_MOVE_SPD_X;
	}
	*/
	obj_work->ppFunc = gmEneKamaHandWaitMain;
}

// ==========================================================================
// gmEneKamaHandWaitMain
/*!
 *	エネミー カマキラー 手の待機 メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaHandWaitMain(OBS_OBJECT_WORK *obj_work)
{
/*
#if 1
	BOOL				b_dec;
	GMS_ENE_KAMA_WORK	*kama_work;

	kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;

	b_dec = gmEneKamaSetWalkSpeed(kama_work);

	if (b_dec) {
		// 減速を開始した

		// フリップへ移行
		gmEneKamaFlipInit(obj_work);
	}
#else
	if (obj_work->move_flag & OBD_MOVE_FRONT ||
			!GmEneComCheckMoveLimit(obj_work, (fx32)obj_work->user_work, (fx32)obj_work->user_flag)) {
		// 前方衝突・移動範囲外

		// フリップへ移行
		gmEneKamaFlipInit(obj_work);
		// FWへ移行
		//gmEneKamaFwInit(obj_work);
	}
#endif
*/
	GMS_ENE_KAMA_WORK	*kama_work;
	GMS_ENE_KAMA_WORK	*parent_work;
	kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;
	parent_work	= (GMS_ENE_KAMA_WORK*)obj_work->parent_obj;
	NNS_MATRIX* mtx;
	NNS_MATRIX msm;

	if (obj_work->parent_obj==NULL){
		// 落ちていこう
		obj_work->spd.x = 0;
		obj_work->spd_fall = FX_F32_TO_FX32( GMD_ENE_KAMA_HAND_G_SPD );
		obj_work->move_flag |= OBD_MOVE_FALL;
		return;
	}

	if (kama_work->hand == GME_ENE_KAMA_HAND_TYPE_LEFT){
		mtx = GmEneUtilGetNodeMatrix( &parent_work->node_work, GMD_ENE_KAMA_L_HAND_NODE );
	}else{
		mtx = GmEneUtilGetNodeMatrix( &parent_work->node_work, GMD_ENE_KAMA_R_HAND_NODE );
	}

	// マトリクスがまだ出来上がっていないときにつけると(いなくなってしまう)
	if (NNM_MTX( *mtx, 0, 3 ) == 0 && NNM_MTX( *mtx, 1, 3 ) == 0){
		return;	
	}

	nnMakeScaleMatrix( &msm, 1.0f, 1.0f, 1.0f );
	nnMultiplyMatrix( &msm, mtx, &msm );

	GmEneUtilSetMatrixNN( obj_work, &msm );

//	obj_work->pos.x = parent_work->ene_3d_work.ene_com.obj_work.pos.x;
//	obj_work->pos.y = parent_work->ene_3d_work.ene_com.obj_work.pos.y;

//	obj_work->disp_flag &= ~OBD_DISP_HFLIP;
//	if (parent_work->ene_3d_work.ene_com.obj_work.disp_flag & OBD_DISP_HFLIP)
//		obj_work->disp_flag |= OBD_DISP_HFLIP;

	// 攻撃開始
	if (parent_work->attack){
		obj_work->ppFunc = gmEneKamaHandAttackInit;
	}
}


// ==========================================================================
// gmEneKamaHandAttackInit
/*!
 *	エネミー カマキラー 手の攻撃初期化
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaHandAttackInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_KAMA_WORK	*kama_work;
	GMS_ENE_KAMA_WORK	*parent_work;
	kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;
	parent_work	= (GMS_ENE_KAMA_WORK*)obj_work->parent_obj;

	// オリジナルは
	// プレイヤーがいる方向の左右のどちらかにとび
	// 上下にある程度ゆれながら徐々にプレイヤーの方に進む
	// とりあえずそれに従う

	// プレイヤーのベクトル方向に飛ぶ
	VecFx32	vec = gmEneKamaGetPlayerVectorFx( obj_work );

	obj_work->spd.x = (fx32)(vec.x * GMD_ENE_KAMA_HAND_SPD);
	obj_work->spd.y = (fx32)(vec.y * GMD_ENE_KAMA_HAND_SPD);

	obj_work->ppFunc = gmEneKamaHandAttackMain;

	kama_work->timer = GMD_ENE_KAMA_HAND_TIME;	// PAL

	if (parent_work->ene_3d_work.ene_com.obj_work.disp_flag & OBD_DISP_VFLIP){
		obj_work->disp_flag |= OBD_DISP_VFLIP;
	}
	if (parent_work->ene_3d_work.ene_com.obj_work.disp_flag & OBD_DISP_HFLIP){
		kama_work->rot_z_add = -AKM_DEGtoA32( GMD_ENE_KAMA_HAND_ROLING );
	}else{
		kama_work->rot_z_add = AKM_DEGtoA32( GMD_ENE_KAMA_HAND_ROLING );
	}

	// 当たりを発生させる
	OBS_RECT_WORK		*rect_work;
	rect_work = &kama_work->ene_3d_work.ene_com.rect_work[GMD_ENEMY_RECT_ATK];
	// 仮矩形
	ObjRectWorkSet(rect_work, -8, -8, 8, 8);
	rect_work->flag |= OBD_RECT_ENABLE;

	obj_work->flag		|= OBD_OBJECT_PARENT_NODIE;				// 親が死んでも生き抜く
	// 親から切り離す
//	kama_work->parent_obj = obj_work->parent_obj;
//	obj_work->parent_obj = NULL;

	// 優先変更 09/11/25
	obj_work->pos.z = GMD_OBJ_DEFAULT_POS_Z_A_FRONT;	//GMD_OBJ_ENEMY_POS_Z;

	// カマSE
	GmSoundPlaySE( GMD_ENE_KAMA_SE_KAMA );
	
}

// ==========================================================================
// gmEneKamaHandAttackMain
/*!
 *	エネミー カマキラー 手の攻撃メイン処理
 *
 *	@param obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void gmEneKamaHandAttackMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENE_KAMA_WORK	*kama_work;
//	GMS_ENE_KAMA_WORK	*parent_work;
	kama_work	= (GMS_ENE_KAMA_WORK*)obj_work;

	//obj_work->dir.z += AKM_DEGtoA32( 15 );
	
	kama_work->rot_z += kama_work->rot_z_add;

	NNS_MATRIX rmat;
	NNS_MATRIX tmat;
	NNS_MATRIX mat;

	nnMakeRotateZMatrix( &rmat, kama_work->rot_z );

	if (obj_work->disp_flag & OBD_DISP_VFLIP){
		nnMakeTranslateMatrix( &tmat, 10.0f, 10.0f, 0.0f );
	}else{
		nnMakeTranslateMatrix( &tmat, 10.0f, -10.0f, 0.0f );
	}

	nnMultiplyMatrix( &mat, &rmat, &tmat );

	obj_work->ofst.x = FX_F32_TO_FX32( NNM_MTX( mat, 0, 3 ) );
	obj_work->ofst.y = FX_F32_TO_FX32( NNM_MTX( mat, 1, 3 ) );
	obj_work->dir.z = (u16)(kama_work->rot_z);


	if (kama_work->timer>0){
		
		// 方向の設定
		VecFx32	vec = gmEneKamaGetPlayerVectorFx( obj_work );

		fx32 fx = (fx32)(obj_work->spd.x / GMD_ENE_KAMA_HAND_SPD);
		fx32 fy = (fx32)(obj_work->spd.y / GMD_ENE_KAMA_HAND_SPD);

		// 外積から一定方向に回す
		fx32	cross	= FX_Mul( vec.x, fy ) - FX_Mul( vec.y, fx );

		if (cross<0){
//			fx = FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(2.0f) ), fx ) - FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(2.0f) ), fy );
//			fy = FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(2.0f) ), fx ) + FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(2.0f) ), fy );
			fx = FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(1.0f) ), fx ) - FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(1.0f) ), fy );
			fy = FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(1.0f) ), fx ) + FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(1.0f) ), fy );
			//x = cos()*x - sin()*y;
			//y = sin()*x + cos()*y;
		}else{
//			fx = FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(-2.0f) ), fx ) - FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(-2.0f) ), fy );
//			fy = FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(-2.0f) ), fx ) + FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(-2.0f) ), fy );
			fx = FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(-1.0f) ), fx ) - FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(-1.0f) ), fy );
			fy = FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(-1.0f) ), fx ) + FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(-1.0f) ), fy );
		}

		/*
		// 処理を簡単にする
		if (fx < vec.x){
			fx += FX_F32_TO_FX32( GMD_ENE_KAMA_HAND_HOMING );
		}else{
			fx += FX_F32_TO_FX32( -GMD_ENE_KAMA_HAND_HOMING );
		}
		if (fy < vec.y){
			fy += FX_F32_TO_FX32( GMD_ENE_KAMA_HAND_HOMING );
		}else{
			fy += FX_F32_TO_FX32( -GMD_ENE_KAMA_HAND_HOMING );
		}

		fx32 len = FX_Sqrt( FX_Mul( fx, fx ) + FX_Mul( fy, fy ) );
		if (len==0){
			fx = 0;
			fy = 0;
		}else{
			fx = FX_Div( fx, len );
			fy = FX_Div( fy, len );
		}
		obj_work->spd.x = (fx32)(fx * GMD_ENE_KAMA_HAND_SPD);
		obj_work->spd.y = (fx32)(fy * GMD_ENE_KAMA_HAND_SPD);
		*/
		obj_work->spd.x = (fx32)( fx * GMD_ENE_KAMA_HAND_SPD );
		obj_work->spd.y = (fx32)( fy * GMD_ENE_KAMA_HAND_SPD );

		kama_work->timer--;
	}else{

#if	0	// 10/29 戻っていく

		// 親に戻す
		obj_work->flag		|= OBD_OBJECT_PARENT_NODIE;				// 親が死んでも生き抜く

		if (obj_work->parent_obj==NULL){
			// 落ちていく
			obj_work->spd.x = 0;
			obj_work->spd_fall = FX_F32_TO_FX32( GMD_ENE_KAMA_HAND_G_SPD );
			obj_work->move_flag |= OBD_MOVE_FALL;
			return;
		}

		VecFx32	vec = gmEneKamaGetParentVectorFx( obj_work );

		fx32 fx = (fx32)(obj_work->spd.x / GMD_ENE_KAMA_HAND_SPD);
		fx32 fy = (fx32)(obj_work->spd.y / GMD_ENE_KAMA_HAND_SPD);

		// 外積から一定方向に回す
		fx32	cross	= FX_Mul( vec.x, fy ) - FX_Mul( vec.y, fx );

		if (cross<0){
			fx = FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(2.0f) ), fx ) - FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(2.0f) ), fy );
			fy = FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(2.0f) ), fx ) + FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(2.0f) ), fy );
			//x = cos()*x - sin()*y;
			//y = sin()*x + cos()*y;
		}else{
			fx = FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(-2.0f) ), fx ) - FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(-2.0f) ), fy );
			fy = FX_Mul( FX_Sin( (Angle16)AKM_DEGtoA32(-2.0f) ), fx ) + FX_Mul( FX_Cos( (Angle16)AKM_DEGtoA32(-2.0f) ), fy );
		}
/*
		// 処理を簡単にする
		if (fx < vec.x){
			fx += FX_F32_TO_FX32( GMD_ENE_KAMA_HAND_HOMING );
		}else{
			fx += FX_F32_TO_FX32( -GMD_ENE_KAMA_HAND_HOMING );
		}
		if (fy < vec.y){
			fy += FX_F32_TO_FX32( GMD_ENE_KAMA_HAND_HOMING );
		}else{
			fy += FX_F32_TO_FX32( -GMD_ENE_KAMA_HAND_HOMING );
		}

		fx32 len = FX_Sqrt( FX_Mul( fx, fx ) + FX_Mul( fy, fy ) );
		if (len==0){
			fx = 0;
			fy = 0;
		}else{
			fx = FX_Div( fx, len );
			fy = FX_Div( fy, len );
		}
*/
		obj_work->spd.x = (fx32)(fx * GMD_ENE_KAMA_HAND_SPD);
		obj_work->spd.y = (fx32)(fy * GMD_ENE_KAMA_HAND_SPD);

		// 本体との距離を見て、近ければ引っ付ける
		fx32	px = obj_work->parent_obj->pos.x - obj_work->pos.x;
		fx32	py = obj_work->parent_obj->pos.y - obj_work->pos.y;

		if ((FX_FX32_TO_F32( px ) * FX_FX32_TO_F32( px )) + (FX_FX32_TO_F32( py ) * FX_FX32_TO_F32( py )) < 32*32){
			gmEneKamaHandWaitInit( obj_work );
			GMS_ENE_KAMA_WORK* kama_parent = (GMS_ENE_KAMA_WORK*)(obj_work->parent_obj);
			kama_parent->attack = FALSE;
		}
#else
		// 落ちていく
		obj_work->spd.x = 0;
		obj_work->spd_fall = FX_F32_TO_FX32( GMD_ENE_KAMA_HAND_G_SPD );
		obj_work->move_flag |= OBD_MOVE_FALL;

		GMS_ENE_KAMA_WORK* kama_parent = (GMS_ENE_KAMA_WORK*)(obj_work->parent_obj);
		if (kama_parent!=NULL)
			kama_parent->ata_futa = TRUE;
#endif
	}
}



// ==========================================================================
// フェードアニメーション
// ==========================================================================
// ==========================================================================
// gmEneKamaFadeAnimeSet
/*!
 *	フェードアニメーション設定
 *
 *	@param	kama_work	[in]	ハリセンボワーク
 *	@param	anime_data	[in]	アニメーションデータ
 */
// ==========================================================================
void gmEneKamaFadeAnimeSet(GMS_ENE_KAMA_WORK *kama_work, GMS_ENE_KAMA_FADE_ANIME *anime_data)
{
	// フェードアニメーション用意
	kama_work->anime_data	= anime_data;
	kama_work->anime_pat_no	= 0;
	kama_work->anime_frame	= 0;
}

// ==========================================================================
// gmEneKamaFadeAnimeUpdate
/*!
 *	フェードアニメーション更新
 *
 *	@param	kama_work	[in]	ハリセンボワーク
 *	@param	speed		[in]	アニメーション速度
 *	@param	repeat		[in]	リピート設定
 */
// ==========================================================================
void gmEneKamaFadeAnimeUpdate(GMS_ENE_KAMA_WORK *kama_work, fx32 speed, BOOL repeat)
{
	GMS_ENE_KAMA_FADE_ANIME		*anime_data;
	GMS_ENE_KAMA_FADE_ANIME_PAT	*pat_data;

	anime_data	= kama_work->anime_data;
	pat_data	= &anime_data->anime_pat[kama_work->anime_pat_no];

	kama_work->anime_frame += speed;
	// パターン繰り
	while (kama_work->anime_frame >= pat_data->frame) {
		kama_work->anime_frame -= pat_data->frame;

		kama_work->anime_pat_no++;
		if (kama_work->anime_pat_no < anime_data->pat_num) {
			// 次のパターン
			pat_data = &anime_data->anime_pat[kama_work->anime_pat_no];
		}
		else {
			if (repeat) {
				// リピート
				kama_work->anime_pat_no = 0;
				pat_data = &anime_data->anime_pat[kama_work->anime_pat_no];
			}
			else {
				// 終了
				kama_work->anime_pat_no = anime_data->pat_num - 1;
				kama_work->anime_frame = pat_data->frame - 1;
			}
		}
	}

	// カラーセット
	GmBsCmnSetObject3DNNFadedColor((OBS_OBJECT_WORK*)kama_work,
									   &pat_data->col,
									   pat_data->intensity);
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
