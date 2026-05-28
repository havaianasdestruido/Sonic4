// =======================================================================
/*!
  @file	gmBoss4Effect.cpp
  @brief ボス4 エフェクト類

  @author K.Yurita(SafariGames)
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Effect.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "gmBoss4Effect.h"

#include "gmPlySeq.h"

#include "gmGmkCamScrLim.h"

/*------ Macros --------------------------------------------------------*/
//############ 爆発エフェクト #################################################
/* フラグ */
/* 定義値 */
#define GMD_BOSS4_EFF_BOMB_OFST_Z					((fx32)(FX32_ONE * 32))	//!< 爆発エフェクト表示座標の親からのオフセット座標Z（ボスより手前に表示させるため一律にずらす）

//############ ダメージエフェクト #############################################
/* 定義値 */
#define GMD_BOSS4_EFF_DAMAGE_OFST_Z					((fx32)(FX32_ONE * 32))	//!< ダメージエフェクト表示座標の親からのオフセット（ボスより手前に表示させるため）

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


/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
static	void gmBoss4EffMainFuncDeleteAtEnd(OBS_OBJECT_WORK *obj_work);
//static	void gmBoss4EffMainFuncDeleteAtEndLink(OBS_OBJECT_WORK *obj_work);
static	void gmBoss4EffMainFuncFlagLink(OBS_OBJECT_WORK *obj_work);


#if defined(MTD_DEBUG)
//############ デバッグ ###############################################
#endif /* defined(MTD_DEBUG) */

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! 衝撃波エフェクト 矩形 攻撃属性フラグテーブル
const static Uint16 gm_boss4_eff_sw_atk_flag_tbl[GME_EFFECT_RECT_NUM]	= {
	0,		// 食らい
	(GMD_OBJ_RECT_ATK_FLAG_NORMALATK | GMD_OBJ_RECT_ATK_FLAG_EFCTATK),	// 攻撃
};

//! 衝撃波エフェクト 矩形 防御属性フラグテーブル
const static Uint16 gm_boss4_eff_sw_def_flag_tbl[GME_EFFECT_RECT_NUM]	= {
	GMD_OBJ_RECT_DEF_FLAG_EFCTDEF,	// 食らい矩形
	GMD_OBJ_RECT_DEF_FLAG_EFCTATK,	// 攻撃矩形
};

/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// GmBoss4EffectBuild
/*!
  ボス4 データ構築
 */
// =======================================================================
void GmBoss4EffectBuild(void)
{
	/*
	// モーション
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_BODY_MTN),
						IDB_BOSS04_BOSS04_BODY_MTN_AMB, GMD_BOSS4_ARC);
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_CAPSULE_MTN),
						IDB_BOSS04_BOSS04_CAPSULE_MTN_AMB, GMD_BOSS4_ARC);
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EGG_MTN),
						IDB_BOSS04_BOSS04_EGG_MTN_AMB, GMD_BOSS4_ARC);
	*/
	// カプセル爆発
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_CAP_EX_ES),
						IDB_BOSS04_EFF_BOMB_CP_AME,
						GMD_BOSS4_ARC);

	// ちびエッグマン爆発(NORMAL)
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_EGG1_EX_ES),
						IDB_BOSS04_EFF_BALLOON_R_AME,
						GMD_BOSS4_ARC);
	// ちびエッグマン爆発(SPEED)
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_EGG2_EX_ES),
						IDB_BOSS04_EFF_BALLOON_B_AME,
						GMD_BOSS4_ARC);
	// ちびエッグマン爆発(BIG)
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_EGG3_EX_ES),
						IDB_BOSS04_EFF_BALLOON_Y_AME,
						GMD_BOSS4_ARC);

	// ちびエッグマンバーニア(NORMAL, SPEED)
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_BOOST1_ES),
						IDB_BOSS04_EFF_JET_BALL_01_AME,
						GMD_BOSS4_ARC);
	// ちびエッグマンバーニア(BIG)
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_BOOST2_ES),
						IDB_BOSS04_EFF_JET_BALL_02_AME,
						GMD_BOSS4_ARC);

	// 第２形態エッグマシンバーニア
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_BOOSTX_ES),
						IDB_BOSS04_EFF_JET_Z4_AME,
						GMD_BOSS4_ARC);

	// 第２形態エッグマシンバーニア(下)
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_BOOSTX2_ES),
						IDB_BOSS04_EFF_JET_Z4_02_AME,
						GMD_BOSS4_ARC);

	// パーツ飛び散る
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_PARTS_EX_ES),
						IDB_BOSS04_EFF_PARTS_AME,
						GMD_BOSS4_ARC);

	// ボスライト
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_BOSS_LIGHT_ES),
						IDB_BOSS04_EFF_BOSS_LIGHT_AME,
						GMD_BOSS4_ARC);

	//---------------------------------------
	// エフェクトをVRAMにロード
	//---------------------------------------
	for (int i=0;i<9;i++){
		// データを9個分用意
		GmEfctBossBuildSingleDataReg(IDB_BOSS04_EFF_BS4_TEX_AMB,
									 ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_AMBTEX),
									 ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_TEXLIST),
									 0, NULL, NULL,	//モデルなし
									 GMD_BOSS4_ARC);
	}
/*
	// 衝撃波00
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW00_ES),
						IDB_BOSS04_EFF_BOSSZ1_00_AME,
						GMD_BOSS4_ARC);
	
	
	// 衝撃波01
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW01_ES),
						IDB_BOSS04_EFF_BOSSZ1_01_AME,
						GMD_BOSS4_ARC);
	
	// エフェクトをVRAMにロード
	// 衝撃波00
	GmEfctBossBuildSingleDataReg(IDB_BOSS04_EFF_BS1_TEX_AMB,
								 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_AMBTEX),
								 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_TEXLIST),
								 0, NULL, NULL,	//モデルなし
								 GMD_BOSS4_ARC);
	
	// 衝撃波01
	GmEfctBossBuildSingleDataReg(IDB_BOSS04_EFF_BS1_TEX_AMB,
								 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_AMBTEX),
								 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_TEXLIST),
								 0, NULL, NULL,	//モデルなし
								 GMD_BOSS4_ARC);
*/
}


// =======================================================================
// GmBoss4EffectFlush
/*!
  ボス１ データ片付け
 */
// =======================================================================
void GmBoss4EffectFlush(void)
{

	// エフェクト解放
	GmEfctBossFlushSingleDataInit();	// ボス専用EFFフラッシュ開始

	// カプセル爆発
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_CAP_EX_ES));

	// ちびエッグ爆発
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_EGG1_EX_ES));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_EGG2_EX_ES));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_EGG3_EX_ES));

	// ブースター類
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_BOOST1_ES));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_BOOST2_ES));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_BOOSTX_ES));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_BOOSTX2_ES));

	// 飛び散るパーツ
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_PARTS_EX_ES));

	// ボスライト
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_BOSS_LIGHT_ES));
	/*
	// エフェクト解放
	GmEfctBossFlushSingleDataInit();	// ボス専用EFFフラッシュ開始
	
	// 衝撃波01
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW01_ES));
	// 衝撃波00
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW00_ES));
*/
}

// =======================================================================
// GmBoss4EffChangeType
/*!
  エフェクトタイプの変更
  
  @param efct_work	[io]	エフェクトワーク
  @param type		[in]	エフェクトタイプ
  @param flag		[in]	エフェクトフラグ
  
  @note
	初期化した後、表示する前にタイプを変更する。
	自動的に高速スクロールに対応する
 */
// =======================================================================
void GmBoss4EffChangeType( GMS_EFFECT_3DES_WORK* efct_work, GME_EFFECT_3DES_POS_TYPE type, Uint32 init_flag )
{
	// タイプ変更する
	efct_work->efct_com.obj_work.ppFunc = NULL;
	GmEffect3DESSetupBase(efct_work, type, init_flag );
	// 処理変更する
	efct_work->efct_com.obj_work.ppFunc = gmBoss4EffMainFuncDeleteAtEnd;
}

// =======================================================================
// GmBoss4EffCommonInit
/*!
  エフェクト共通出力 初期化
  
  @param body_work	[io]	本体ワーク
  
  @return 3desエフェクトワーク
  
  @note
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* GmBoss4EffCommonInit(Sint32 id, VecFx32* pos, OBS_OBJECT_WORK* parent_obj,
										   GME_EFFECT_3DES_POS_TYPE type, Uint32 flag, GMS_BOSS4_NODE_MATRIX* mtx, Sint32 link,
										   VecFx32* rot, Uint32* ctrl_flag, Uint32 mask)
{
	GMS_EFFECT_3DES_WORK*			eff_work;
	OBS_OBJECT_WORK*				obj_work;
	GMS_BOSS4_EFF_COMMON_WORK*		eff_com;

	// BOSS4オブジェクト
	obj_work	= GMM_EFFECT_CREATE_WORK(sizeof(GMS_BOSS4_EFF_COMMON_WORK),
										 parent_obj,
										 NULL,
										 "B04_CapOver");
	
	eff_work	= (GMS_EFFECT_3DES_WORK*)obj_work;
	eff_com		= (GMS_BOSS4_EFF_COMMON_WORK*)obj_work;

	// ESエフェクトデータロード
	ObjObjectAction3dESEffectLoad(GMM_BS_OBJ(eff_work),
								  &eff_work->obj_3des,
								  ObjDataGet(id/*GMD_DWORK_NO_BOSS_04_EF_CAP_EX_ES*/),
								  NULL,//filename
								  0,//amb_index
								  NULL);
	
	// ESテクスチャデータロード
	ObjObjectAction3dESTextureLoad(GMM_BS_OBJ(eff_work),
								   &eff_work->obj_3des,
								   ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_AMBTEX),
								   NULL,//filename
								   0,//amb_index
								   NULL,
								   FALSE);	// 転送しない
	// ロード済みESテクスチャセット
	ObjObjectAction3dESTextureSetByDwork(obj_work,
										 ObjDataGet(GMD_DWORK_NO_BOSS_04_EF_TEXLIST));

	// 基本設定
	GmEffect3DESSetupBase(eff_work, type, flag);
	
	if (pos!=NULL){
		VEC_Set( &obj_work->pos, pos->x, pos->y, pos->z );
	}
	/*	
	// 当たり設定
	obj_work->flag	&= ~OBD_OBJECT_NOHIT;
	GmEffectRectInit(&eff_3des->efct_com,
					 gm_boss1_eff_sw_atk_flag_tbl,
					 gm_boss1_eff_sw_def_flag_tbl,
					 GMD_OBJ_RECT_GROUP_ENEMY,
					 GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
	
	// 矩形サイズ設定
	ObjRectWorkSet(&eff_3des->efct_com.rect_work[GMD_ENEMY_RECT_ATK],
				   -64, -32, 64, 32);//仮  要調整！！
	*/

	// 処理関数設定
	//obj_work->ppFunc	= gmBoss4EffMainFuncFlagLink;
	obj_work->ppFunc	= gmBoss4EffMainFuncFlagLink;

	obj_work->flag	|= OBD_OBJECT_NOPAUSE;

	eff_com->lookflag	= ctrl_flag;
	eff_com->lookmask	= mask;

	if (eff_com->lookflag!=NULL){
		*eff_com->lookflag |= eff_com->lookmask;
	}

	eff_com->link		= -1;
	if (link>=0){
		// リンク情報を取得
		eff_com->link		= link;
		eff_com->node_work	= mtx;
/*
		amVectorSet( &eff_com->rot,
				rot->x,
				rot->y,
				rot->z );

		amVectorSet( &eff_com->ofs,
				pos->x / FX32_ONE,
				pos->y / FX32_ONE,
				pos->z / FX32_ONE );
*/
		if (rot!=NULL){
			// 表示角度設定
			GmEffect3DESSetDispRotation( eff_work,	AKM_DEGtoA16( FX_FX32_TO_F32(rot->x) ),
													AKM_DEGtoA16( FX_FX32_TO_F32(rot->y) ),
													AKM_DEGtoA16( FX_FX32_TO_F32(rot->z) ) );
		}

		if (pos!=NULL){
			// 表示オフセット調整
			GmEffect3DESAddDispOffset( eff_work,	FX_FX32_TO_F32( pos->x ),
													FX_FX32_TO_F32( pos->y ),
													FX_FX32_TO_F32( pos->z ) );
		}		
	}
	
	return eff_work;
}

// ############################################################################
// 爆発エフェクト
// ############################################################################
// =======================================================================
// GmBoss4EffBombInitCreate
/*!
  爆発エフェクト 生成処理初期化
 
  @param bomb_work		[io]	本体ワーク
  @param bomb_type		[in]	爆発タイプ
  @param pos_x			[in]	生成範囲中心座標X
  @param pos_y			[in]	生成範囲中心座標Y
  @param width			[in]	生成範囲幅
  @param height			[in]	生成範囲高さ
  @param interval_min	[in]	生成間隔最短時間
  @param interval_max	[in]	生成間隔最長時間
 */
// =======================================================================
void GmBoss4EffBombInitCreate(GMS_BOSS4_EFF_BOMB_WORK *bomb_work,
							  GME_BOSS4_EFF_BOMB_TYPE bomb_type,
							  OBS_OBJECT_WORK *parent_obj,
							  fx32 pos_x, fx32 pos_y, fx32 width, fx32 height,
							  Uint32 interval_min, Uint32 interval_max)
{
	MTM_ASSERT(bomb_work);
	MTM_ASSERT(parent_obj);
	
	bomb_work->parent_obj	= parent_obj;
	bomb_work->bomb_type	= bomb_type;
	bomb_work->interval_timer	= 0;
	bomb_work->interval_min	= interval_min;
	bomb_work->interval_max	= interval_max;
	bomb_work->pos[MTD_X]	= pos_x;
	bomb_work->pos[MTD_Y]	= pos_y;
	bomb_work->area[MTD_WIDTH]	= width;
	bomb_work->area[MTD_HEIGHT]	= height;
#if _IPHONE
	bomb_work->interval_timer_sound = 0;
#endif // _IPHONE
}

// =======================================================================
// GmBoss4EffBombUpdateCreate
/*!
  爆発エフェクト 生成処理更新
  
  @param bomb_work	[io]	爆発ワーク
  
  @note
  初期化時に指定されたパラメータで爆発エフェクトを生成します。
  生成期間中は毎フレーム呼び出してください。
 */
// =======================================================================
void GmBoss4EffBombUpdateCreate(GMS_BOSS4_EFF_BOMB_WORK *bomb_work)

{
	MTM_ASSERT(bomb_work->parent_obj);	// Z位置を決めるのに親を使用

	// 高速スクロール時、ワープで発生位置をずらす
//	if (GmBoss4GetScrollOffset() < 0){
		bomb_work->pos[MTD_X] += GmBoss4GetScrollOffset();
//	}

	if (bomb_work->interval_timer) {
		bomb_work->interval_timer--;
	}
	else {
		GMS_EFFECT_3DES_WORK	*efct_work	= NULL;
		OBS_OBJECT_WORK	*obj_work;
		fx32	width	= bomb_work->area[MTD_WIDTH];
		fx32	height	= bomb_work->area[MTD_HEIGHT];
		fx32	rand_x;
		fx32	rand_y;
		
		// (pos_x, pos_y) が中心となるwidth x heightの長方形内のランダムな座標を取得
		rand_x	= FX_Mul(AkMathRandFx(), width);
		rand_y	= FX_Mul(AkMathRandFx(), height);
/*		
	GMD_DWORK_NO_BOSS_04_EF_CAP_EX_ES,		//!< カプセル爆破 ESエフェクト
	GMD_DWORK_NO_BOSS_04_EF_EGG1_EX_ES,		//!< ちびエッグ爆破 ESエフェクト
	GMD_DWORK_NO_BOSS_04_EF_EGG2_EX_ES,		//!< ちびエッグ爆破 ESエフェクト
	GMD_DWORK_NO_BOSS_04_EF_EGG3_EX_ES,		//!< ちびエッグ爆破 ESエフェクト
	GMD_DWORK_NO_BOSS_04_EF_BOOST1_ES,		//!< ブースター ESエフェクト
	GMD_DWORK_NO_BOSS_04_EF_BOOST2_ES,		//!< ブースター ESエフェクト
	GMD_DWORK_NO_BOSS_04_EF_BOOSTX_ES,		//!< ブースター第２形態 ESエフェクト
	GMD_DWORK_NO_BOSS_04_EF_PARTS_EX_ES,	//!< パーツ爆破 ESエフェクト
*/
		// エフェクト生成
		switch (bomb_work->bomb_type) {

		// 共通小爆発(念のためおいておく)
		case GME_BOSS4_EFF_BOMB_TYPE_SMALL:	// 小爆発
			efct_work	= GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_BOMB);

			// 依存タイプに変更する
			efct_work->efct_com.obj_work.ppFunc = NULL;
			GmEffect3DESSetupBase(efct_work, GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
						  GMD_EFFECT_3DES_FLAG_NOFLIP );
			efct_work->efct_com.obj_work.ppFunc = gmBoss4EffMainFuncDeleteAtEnd;
			
#if _IPHONE
			if (--bomb_work->interval_timer_sound > 0) {
				break;
			}
			bomb_work->interval_timer_sound = 3;
#endif // _IPHONE
			// 小爆発SE再生
			GmSoundPlaySE("Boss0_02");
			break;
/*
		// カプセル爆破
		case GME_BOSS4_EFF_BOMB_TYPE_CAPSULE:
			efct_work	= GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_CAP_EX_ES, NULL );

			// 小爆発SE再生
			GmSoundPlaySE("Boss0_02");
			break;

		// ちびエッグマン(通常)爆破
		case GME_BOSS4_EFF_BOMB_TYPE_CHIBI_01:
			efct_work	= GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_EGG1_EX_ES, NULL );
			
			// 小爆発SE再生
			GmSoundPlaySE("Boss0_02");
			break;

		// ちびエッグマン(スピード)爆破
		case GME_BOSS4_EFF_BOMB_TYPE_CHIBI_02:
			efct_work	= GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_EGG2_EX_ES, NULL );
			
			// 小爆発SE再生
			GmSoundPlaySE("Boss0_02");
			break;

		// ちびエッグマン(スピード)爆破
		case GME_BOSS4_EFF_BOMB_TYPE_CHIBI_03:
			efct_work	= GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_EGG3_EX_ES, NULL );
			
			// 小爆発SE再生
			GmSoundPlaySE("Boss0_02");
			break;
*/
		// パーツ爆破
		case GME_BOSS4_EFF_BOMB_TYPE_PARTS:
			efct_work	= GmBoss4EffCommonInit( GMD_DWORK_NO_BOSS_04_EF_PARTS_EX_ES, NULL );
			efct_work->efct_com.obj_work.ppFunc = NULL;
			
			GmEffect3DESSetupBase(efct_work, GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,
						  GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT/*GMD_EFFECT_3DES_FLAG_NOFLIP*/);
			
			efct_work->efct_com.obj_work.ppFunc = gmBoss4EffMainFuncDeleteAtEnd;
			// 小爆発SE再生
			//GmSoundPlaySE("Boss0_02");

			break;

		default:
			MTM_ASSERT(!"gmBoss4::gmBoss4EffBombUpdateCreate() invalid bomb type\n");
			return;
		}
		
		obj_work	= GMM_BS_OBJ(efct_work);
		
		MTM_ASSERT(obj_work);

		// 座標設定
		obj_work->pos.x	= bomb_work->pos[MTD_X] - (width >> 1) + rand_x;
		obj_work->pos.y	= bomb_work->pos[MTD_Y] - (height >> 1) + rand_y;
		// Z方向に一律にずらす
		obj_work->pos.z	= GMM_BS_OBJ(bomb_work->parent_obj)->pos.z + GMD_BOSS4_EFF_BOMB_OFST_Z;
		
		// 次の生成までのインターバルを設定
		{
			Uint32	rand_ofst;
			rand_ofst	= (Uint32)((AkMathRandFx() * (bomb_work->interval_max - bomb_work->interval_max)) >> FX32_SHIFT);
			bomb_work->interval_timer = bomb_work->interval_min + rand_ofst;
		}
	}
}

/*------ Static Functions ----------------------------------------------*/

// =======================================================================
// gmBoss4EffMainFuncFlagLink
/*!
  エフェクト メイン処理関数 アニメーション終了時削除 親にリンクする
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  OBD_DISP_ENDフラグがった時に自分をクリアします。
 */
// =======================================================================
void gmBoss4EffMainFuncFlagLink(OBS_OBJECT_WORK *obj_work)
{
//	GMS_BOSS4_BODY_WORK*		parent_body	= (GMS_BOSS4_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS4_EFF_COMMON_WORK*	eff_com		= (GMS_BOSS4_EFF_COMMON_WORK*)obj_work;

	// ポーズ中はUPDATEしないようにする
	obj_work->disp_flag &= ~OBD_DISP_NOUPDATE;
	if (g_obj.flag & OBD_OBJ_PAUSE){
		obj_work->disp_flag |= OBD_DISP_NOUPDATE;
	}else{
		// 画面が強制移動したとき
		obj_work->pos.x += GmBoss4GetScrollOffset();
	}
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;

		// フラグを落とす
		if (eff_com->lookflag!=NULL){
			*eff_com->lookflag &= ~eff_com->lookmask;
		}
	}
	if (eff_com->lookflag!=NULL){
		if ((*eff_com->lookflag & eff_com->lookmask)==0){
			// 終了
			ObjDrawKillAction3DES(obj_work);
		}
	}

	if (eff_com->link>=0){
		// 本体SNMマトリクスでくっつける
		GmBsCmnUpdateObject3DESStuckWithNode( obj_work,
										&eff_com->node_work->snm_work,
										eff_com->node_work->work[ eff_com->link ],
										TRUE);
	}
}

// =======================================================================
// GmEffectDefaultMainFuncDeleteAtEnd
/*!
  エフェクト メイン処理関数 アニメーション終了時削除
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  OBD_DISP_ENDフラグがった時に自分をクリアします。
 */
// =======================================================================
void gmBoss4EffMainFuncDeleteAtEnd(OBS_OBJECT_WORK *obj_work)
{
	// 画面が強制移動したとき
	obj_work->pos.x += GmBoss4GetScrollOffset();

	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}




// ############################################################################
// ダメージエフェクト
// ############################################################################
// =======================================================================
// gmBoss4EffDamageInit
/*!
  ダメージエフェクト初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  破片の出るエフェクトです。
 */
// =======================================================================
void gmBoss4EffDamageInit(/*GMS_BOSS4_BODY_WORK*/ void* body_work)
{
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(body_work);
	GMS_EFFECT_3DES_WORK	*efct_work;
	efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_BOSS_DM);
	GMM_BS_OBJ(efct_work)->pos.z	+= GMD_BOSS4_EFF_DAMAGE_OFST_Z;
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
