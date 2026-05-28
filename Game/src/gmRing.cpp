// ==========================================================================
/*!
  @file gmRing.cpp
  @brief リング関連

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmRing.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date::						   $
 */
// ==========================================================================
/*
 * memo
 *
 *
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmRing.h"
#include "gmMain.h"
#include "gmEventMgr.h"
#include "gmGameDat.h"
#include "gmTask.h"
#include "gmMap.h"

#include "gmComEfct.h"



// データヘッダ
#if !_IPHONE
#include "common/model/ring_mdl.hmb"
#else
#include "iPhone/model/RING_MDL.HMB"
#include "iPhone/model/RING_MAT.HMB"
#endif

//mpp
#include "mppAchievementSupport.h"

//----- Macros --------------------------------------------------------------
#define GMD_RING_DRAW_STRIP_TEST

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
// 速度設定
#define GMD_RING_FALL_ACC				(0x00120)			//!< リング落下加速度
#define GMD_RING_MAGNET_ACC				(0x00400)			//!< 磁力加速度
#define GMD_RING_MAGNET_MSPD			(0x1f000)			//!< 磁力最大速度
#define GMD_RING_DUCT_ACC				(0x00100)			//!< 吸い込みダクト加速度
#define GMD_RING_DUCT_MSPD				(0x08000)			//!< 吸い込みダクト最大速度

#define GMD_RING_ROT_SPD				(0x10000*4/360)		//!< リング回転速度

#if _IPHONE
#define GMD_RING_ROT_FRONT              (0x10000*3/4)       //!< リング正面向き(270)
#endif // _IPHONE

// 共通設定
#define GMD_RING_DEF_SIZE				(20)

// 特殊処理適用範囲設定
#define GMD_RING_MAGNET_RANGE			(72)				//!< 磁力適用範囲
#define GMD_RING_DUCT_RANGE				(128)				//!< 吸い込みダクト適用範囲

// ダメージリング設定
#define GMD_RING_TIMER					(256)					//!< リングが消えるまでの時間
#define GMD_RING_NOHIT_TIMER			(GMD_RING_TIMER - 40)	//!< リングが当たり始める時間
#define GMD_RING_BLINK_TIMER			(32)					//!< リングが点滅し始める時間
#define GMD_RING_DAMAGE_NUM_MAX			(4)						//!< ダメージ段階最大値(通常時)

// スロットリング設定
#define GMD_RING_SLOT_SPD				(0x6000)				//!< スロットリング移動速度
#define GMD_RING_SLOT_DIST				(256*FX32_ONE)			//!< スロットリング生成位置
#define GMD_RING_SLOT_DIR_SPD			(0x800)					//!< スロットリング生成位置角度速度
#define GMD_RING_SLOT_INT_TIME			(4)						//!< スロットリング生成間隔

// 取得エフェクト設定
#define GMD_RING_TWINKLE_TIMER			(18)					//!< 取得エフェクト表示時間

// 画面外削除範囲設定
#define GMD_RING_DIE_OFFSET				(gm_ring_die_offset)//72)//(16)	//!< 通常リング 画面外判定オフセット値 ギミックリングと共通にしておく事
#define GMD_RING_DIE_DEF_OFFSET			(72)
#define GMD_RING_MOVE_DIE_OFFSET		(196)		//!< 移動リング 画面外判定オフセット値
#define GMD_RING_SLOT_DIE_OFFSET		(512)		//!< スロットリング 画面外判定オフセット値

#if _IPHONE
//#define GMD_RING_DRAW_MODEL			//!< モデルによるリング描画
#define GMD_RING_DRAW_PRIMITIVE		//!< プリミティブによるリング描画

#define GMD_RING_ROLL_UV_POS_U		(  0 )	//!<	U座標
#define GMD_RING_ROLL_UV_POS_V		(  1 )	//!<	V座標
#define GMD_RING_ROLL_UV_POS_UV		(  2 )	//!<	数
#define GMD_RING_ROLL_UV_POS_NUM	( 16 )	//!<	座標個数
#define GMD_RING_ROLL_UV_FRAME_WAIT	(  4 )	//!<	滞在時間
#define GMD_RING_ROLL_UV_FRAME_MAX	( 64 )	//!<	フレーム最大値
#define GMD_RING_ROLL_UV_SIZE		(0.25f)	//!<	UVサイズ
#endif //_IPHONE

//----- Global Variables ----------------------------------------------------
s16 g_gm_ring_size = GMD_RING_DEF_SIZE;	//!< リングサイズ

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB			*gm_ring_tcb = NULL;					//!< リングシステムTCB
GMS_RING_SYS_WORK			*gm_ring_sys_work = NULL;				//!< リングシステムワーク(TCBワークで取得)

static OBS_ACTION3D_NN_WORK	*gm_ring_obj_3d = NULL;			//!< リング描画用オブジェクト		


#if 1
fx16						gm_ring_fall_acc_x = 0;					//!< リング落下加速度 X
fx16						gm_ring_fall_acc_y = GMD_RING_FALL_ACC;	//!< リング落下加速度 Y
#else
fx16						gm_ring_fall_acc = GMD_RING_FALL_ACC;	//!< リング落下加速度
#endif
static fx32					gm_ring_scale = FX32_ONE;				//!< リング描画時スケール(演出用)
static s16					gm_ring_die_offset = GMD_RING_DIE_DEF_OFFSET;	//!< 通常リング 画面外判定オフセット値

/// 難易度毎のリングダメージ最大段階数
static const u8 gm_ring_damege_num_tbl[GSD_GAME_LEVEL_MAX] = {
	GMD_RING_DAMAGE_NUM_MAX - 1,			// EASY
	GMD_RING_DAMAGE_NUM_MAX,				// NORAML
	GMD_RING_DAMAGE_NUM_MAX + 1,			// HARD
};

#if _IPHONE
/// リング回転UV座標(左上)
static const float gm_ring_roll_uv[GMD_RING_ROLL_UV_POS_UV][GMD_RING_ROLL_UV_POS_NUM] = {
	{0.75f, 0.50f, 0.25f, 0.00f, 0.75f, 0.50f, 0.25f, 0.00f, 0.75f, 0.50f, 0.25f, 0.00f, 0.75f, 0.50f, 0.25f, 0.00f}, // U
	{0.75f, 0.75f, 0.75f, 0.75f, 0.50f, 0.50f, 0.50f, 0.50f, 0.25f, 0.25f, 0.25f, 0.25f, 0.00f, 0.00f, 0.00f, 0.00f}, // V
};
#endif //_IPHONE

//----- Static Declarations -------------------------------------------------
// 終了処理
static void gmRingDest(MTS_TASK_TCB *tcb);
// メイン
static void gmRingMain(MTS_TASK_TCB *tcb);

// リングワーク管理
static GMS_RING_WORK* gmRingAllocRingWork(void);
static void gmRingFreeRingWork(GMS_RING_WORK *ring_work);
// リングリスト管理
static void gmRingAttachRingList(GMS_RING_WORK *ring_work);
static void gmRingDetachRingList(GMS_RING_WORK *ring_work);
//static void gmRingAttachTwinkleList(GMS_RING_WORK *ring_work);
//static void gmRingDetachTwinkleList(GMS_RING_WORK *ring_work);
//static void gmRingAttachMagnetRingList(GMS_RING_WORK *ring_work);
//static void gmRingDetachMagnetRingList(GMS_RING_WORK *ring_work);
static void gmRingAttachDamageRingList(GMS_RING_WORK *ring_work);
static void gmRingDetachDamageRingList(GMS_RING_WORK *ring_work);
static void gmRingAttachSlotRingList(GMS_RING_WORK *ring_work);
static void gmRingDetachSlotRingList(GMS_RING_WORK *ring_work);

// 地形判定
static void gmRingMoveCollsion(GMS_RING_WORK *ring_work);
//static void gmRingMoveCollsionObject(GMS_RING_WORK *ring_work);
//static void gmRingMoveCollsionObjectPlus(GMS_RING_WORK *ring_work);

// 描画
//static void gmRingDrawFuncRing2D(GMS_RING_WORK *ring_work);
static void gmRingDrawFuncRing3D(GMS_RING_WORK *ring_work);
//static void gmRingDrawFuncRingCircle3D(GMS_RING_WORK *ring_work);
//static void gmRingDrawFuncTwinkle2D(GMS_RING_WORK *ring_work);
//static void gmRingDrawFuncTwinkle3D(GMS_RING_WORK *ring_work);
//static void gmRingDrawFuncTwinkleCircle3D(GMS_RING_WORK *ring_work);

#if 0
static void nlRingClearActionInit(s32 lPosX, s32 lPosY, void* pBac, u32 ulVramAddrA, u32 ulVramAddrB);
static void nlRingClearActionMain();

static void nlRingClearAction3dInit( fx32 x, fx32 y, fx32 z, void* pBac );
static void nlRingClearAction3dMain( void );
static void nlRingClearAction3dExit( MTS_TASK_TCB *tcb );


static void nlRingClearFunc2D( GMS_RING_SYS_WORK *ring_work, GMS_RING_WORK *pRing );
static void nlRingClearFunc3D( GMS_RING_SYS_WORK *ring_work, GMS_RING_WORK *pRing );
static void nlRingClearFuncBoss2( GMS_RING_SYS_WORK *ring_work, GMS_RING_WORK *pRing );
static void nlRingClearFuncBoss3( GMS_RING_SYS_WORK *ring_work, GMS_RING_WORK *pRing );
static void nlRingClearFuncBoss4( GMS_RING_SYS_WORK *ring_work, GMS_RING_WORK *pRing );
#endif

// 当たり判定関数
static u16 gmRingHitFuncNormal(OBS_RECT *ply_rect, OBS_RECT *ring_rect);
//static u16 gmRingHitFuncCircle(OBS_RECT *ply_rect, OBS_RECT *ring_rect);

#if 0	// ◆ステージ固有(円形ステージ)
static u16 nlRingRecZ23( OBS_RECT_WORK* obj_work, s32 sX, s32 sY, s16 sLeft, s16 sTop, u16 usWidth, u16 usHeight );
static u16 nlRingRecZ33( OBS_RECT_WORK* obj_work, s32 sX, s32 sY, s16 sLeft, s16 sTop, u16 usWidth, u16 usHeight );
static u16 nlRingRecZ43( OBS_RECT_WORK* obj_work, s32 sX, s32 sY, s16 sLeft, s16 sTop, u16 usWidth, u16 usHeight );
#endif

#if _IPHONE
// 描画開始/終了
static void gmRingDrawBegin(void);
static void gmRingDrawEnd(void);
#endif //_IPHONE


//----- Global Functions ----------------------------------------------------
// ==========================================================================
// データ構築
// ==========================================================================
// ==========================================================================
// GmRingBuild
/*!
 *	リングデータ構築
 */
// ==========================================================================
void GmRingBuild(void)
{
	MTM_ASSERT(gm_ring_obj_3d == NULL);

	// リング描画用オブジェクト取得
	gm_ring_obj_3d = (OBS_ACTION3D_NN_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_NN_WORK));
    MI_CpuClear8(gm_ring_obj_3d, sizeof(OBS_ACTION3D_NN_WORK));

	ObjAction3dNNModelLoad(gm_ring_obj_3d,
				NULL/*data_work*/, NULL/*file_name*/,
				0/*index*/, ObjDataGet(GMD_DWORK_NO_RING_MODEL)->pData,
				NULL/*tex_data_path*/, ObjDataGet(GMD_DWORK_NO_RING_TEX)->pData,
				0/*NNF_DRAWOBJ*/);
}

// ==========================================================================
// GmRingBuildCheck
/*!
 *	リングデータ構築 終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL GmRingBuildCheck(void)
{
	if (ObjAction3dNNModelLoadCheck(gm_ring_obj_3d)) {
#if _IPHONE
#if !defined GMD_DEBUG_NO_CREATE_RING
		// 初回のみ初期化
		if (gm_ring_obj_3d->mat_mtn[0] == NULL) {
			// MaterialMotion
			ObjAction3dNNMaterialMotionLoad(gm_ring_obj_3d,
											0,
											NULL,
											NULL,
											IDB_RING_MAT_RING_INV,
											(void*)ObjDataGet(GMD_DWORK_NO_RING_MAT)->pData);
		}
#endif // GMD_DEBUG_NO_CREATE_RING
#endif // _IPHONE
		// 構築終了
		return (TRUE);
	}
	return (FALSE);
}

// ==========================================================================
// GmRingFlush
/*!
 *	リングデータフラッシュ
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
void GmRingFlush(void)
{
	// データフラッシュ
#if _IPHONE
	ObjAction3dNNMotionRelease(gm_ring_obj_3d);
#endif // _IPHONE
	ObjAction3dNNModelRelease(gm_ring_obj_3d);
}

// ==========================================================================
// GmRingFlushCheck
/*!
 *	リングデータフラッシュ終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL GmRingFlushCheck(void)
{
	if (gm_ring_obj_3d == NULL) {
		return (TRUE);
	}
	if (ObjAction3dNNModelReleaseCheck(gm_ring_obj_3d)) {
		amMemFree(gm_ring_obj_3d);
		gm_ring_obj_3d = NULL;
		return (TRUE);
	}
	return (FALSE);
}

// ==========================================================================
// 初期化
// ==========================================================================
// ==========================================================================
// GmRingInit
/*!
 *	リング管理初期化関数
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
void GmRingInit(void)
{
	s32					i;
	MTS_TASK_TCB		*tcb;
//	GMS_RING_SYS_WORK	*ring_work;
	GMS_RING_WORK		*ring_work;

	tcb = MTM_TASK_MAKE_TCB(gmRingMain, gmRingDest,
							0, GMD_TASK_PAUSELEVEL_DEF, 
							GMD_TASK_PRIO_RING, GMD_TASK_GROUP_RING_SYS,
							sizeof(GMS_RING_SYS_WORK), "GM RING MAIN");

	if (tcb == MTD_TASK_ERROR_ADDR) {
		return;
	}

	gm_ring_tcb = tcb;	// TCB保存

	/*** 管理ワーク初期化 ***/
	gm_ring_sys_work = (GMS_RING_SYS_WORK*)mtTaskGetTcbWork( tcb );
	MI_CpuClear16(gm_ring_sys_work, sizeof(GMS_RING_SYS_WORK));

	/* SEハンドル取得 */
	gm_ring_sys_work->h_snd_ring[0] = GsSoundAllocSeHandle();
	gm_ring_sys_work->h_snd_ring[1] = GsSoundAllocSeHandle();
	gm_ring_sys_work->h_snd_ring[0]->flag |= GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR;
	gm_ring_sys_work->h_snd_ring[1]->flag |= GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR;

	/* 人数設定 */
	gm_ring_sys_work->player_num = 1;
//	if (GsGetMainSysInfo()->game_flag & GSD_GAME_FLAG_WM) {
//		gm_ring_sys_work->player_num = 2;
//	}

	/* リングリストバッファ初期化 */
	for (i = 0, ring_work = gm_ring_sys_work->ring_list_buf; i < GMD_RING_MAX_NUM; i++, ring_work++) {
		gm_ring_sys_work->ring_list[i] = ring_work;
	}

	/* 描画時スケール初期化 */
	gm_ring_scale = FX32_ONE;

	/* 重力初期化 */
	gm_ring_fall_acc_x = 0;
	gm_ring_fall_acc_y = GMD_RING_FALL_ACC;

	/* リングサイズ */
	if (GSD_MAIN_STAGE_ID_SS1 <= g_gs_main_sys_info.stage_id &&
				g_gs_main_sys_info.stage_id <= GSD_MAIN_STAGE_ID_SS7) {
		// 回転ステージ
		g_gm_ring_size		= (s16)(((OBD_LCD_X - OBD_LCD_Y) * 1.4) + GMD_RING_DEF_SIZE);
		gm_ring_die_offset	= (s16)(((OBD_LCD_X - OBD_LCD_Y) * 1.4) + GMD_RING_DIE_DEF_OFFSET);
	}
	else {
		// 標準
		g_gm_ring_size		= GMD_RING_DEF_SIZE;
		gm_ring_die_offset	= GMD_RING_DIE_DEF_OFFSET;
	}

	/* 当たり判定関数設定 */
#if 1
	gm_ring_sys_work->rec_func = gmRingHitFuncNormal;
#else
	switch (GsGetMainSysInfo()->stage_id) {
	case GMD_MAIN_STAGE_ID_2_BOSS:
	case GMD_MAIN_STAGE_ID_3_BOSS:
		gm_ring_sys_work->rec_func = gmRingHitFuncCircle;
		break;

	default:
		gm_ring_sys_work->rec_func = gmRingHitFuncNormal;
	}
#endif

	/* 地形判定関数設定 */
#if 1
	gm_ring_sys_work->col_func = gmRingMoveCollsion;
#else
	switch (GsGetMainSysInfo()->stage_id) {
	case GMD_MAIN_STAGE_ID_4_BOSS:
	case GMD_MAIN_STAGE_ID_5_BOSS:
		gm_ring_sys_work->col_func = gmRingMoveCollsionObjectPlus;
		break;

	default:
		gm_ring_sys_work->col_func = gmRingMoveCollsion;
	}
#endif

	/* 跳ね返り速度ベース設定 */
#if 1
	gm_ring_sys_work->ref_spd_base = 0x2000;
#else
	switch (GsGetMainSysInfo()->stage_id) {
	case GMD_MAIN_STAGE_ID_6_BOSS:
		gm_ring_sys_work->ref_spd_base = 0x2000;
		break;
	}
#endif

	/* 描画設定 */
#if 1
	//ObjObjectAction3dNNModelLoad(obj_work, &gmk_work->obj_3d,
	//			NULL/*data_work*/, NULL/*model_data_path*/,
	//			IDB_GMK_SPRING_MDL_GMK_SPRING_ZNO/*index*/,
	//			ObjDataGet(GMD_DWORK_NO_GMK_SPRING_MODEL)->pData/*archive*/,
	//			NULL/*tex_data_path*/, ObjDataGet(GMD_DWORK_NO_GMK_SPRING_TEX)->pData,
	//			0/*NNF_DRAWOBJ*/);
	// リングモデル初期化
//	ObjAction3dNNModelLoad(&gm_ring_sys_work->ring_obj_3d,
//				NULL/*data_work*/, NULL/*file_name*/,
//				0/*index*/, ObjDataGet(GMD_DWORK_NO_RING_MODEL)->pData,
//				NULL/*tex_data_path*/, ObjDataGet(GMD_DWORK_NO_RING_TEX)->pData,
//				0/*NNF_DRAWOBJ*/);

	// 消滅エフェクト

	/* 描画関数設定 */
	gm_ring_sys_work->ring_draw_func	= gmRingDrawFuncRing3D;
//	gm_ring_sys_work->twinkle_draw_func	= gmRingDrawFuncTwinkle3D;
	
#if _IPHONE
	// SE Wait
	gm_ring_sys_work->se_wait = 0;
	// color setting
	gm_ring_sys_work->color = 0xffffffff;
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3 || g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_BOSS) {
		gm_ring_sys_work->color = 0xffe0e0ff;
	}
	else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		gm_ring_sys_work->color = 0xc0c0c0ff;
	}
#if !defined GMD_DEBUG_NO_CREATE_RING
	// ring angle
	gm_ring_sys_work->dir = GMD_RING_ROT_FRONT;
	ObjDrawAction3dActionSet3DNNMaterial(gm_ring_obj_3d, IDB_RING_MAT_RING_INV);
#endif // !defined GMD_DEBUG_NO_CREATE_RING
#endif // _IPHONE

#else
	if (GmMainIsBossStage()) {
	// ボスステージ(3D)
		MTS_ACTION3D_SPRITE	*act_3dspr;

		/* 描画関数設定 */
		switch (GsGetMainSysInfo()->stage_id) {
		case GMD_MAIN_STAGE_ID_2_BOSS:
			gm_ring_sys_work->ring_draw_func	= gmRingDrawFuncRingCircle3D;
			gm_ring_sys_work->twinkle_draw_func	= gmRingDrawFuncTwinkleCircle3D;
		//	gm_ring_sys_work->stage_start_pos	= GMD_BOSS2_MAP_START;
		//	gm_ring_sys_work->stage_end_pos		= GMD_BOSS2_MAP_END;
		//	gm_ring_sys_work->stage_radius		= GMD_BOSS2_MAP_RADIUS;
			break;
		case GMD_MAIN_STAGE_ID_3_BOSS:
			gm_ring_sys_work->ring_draw_func	= gmRingDrawFuncRingCircle3D;
			gm_ring_sys_work->twinkle_draw_func	= gmRingDrawFuncTwinkleCircle3D;
		//	gm_ring_sys_work->stage_start_pos	= GMD_BOSS3_MAP_START;
		//	gm_ring_sys_work->stage_end_pos		= GMD_BOSS3_MAP_END;
		//	gm_ring_sys_work->stage_radius		= GMD_BOSS3_MAP_RADIUS;
			break;

		default:
			gm_ring_sys_work->ring_draw_func	= gmRingDrawFuncRing3D;
			gm_ring_sys_work->twinkle_draw_func	= gmRingDrawFuncTwinkle3D;
		}

		/* データロード */
		bac = ObjDataLoad(NULL, "/ac_itm_ring3d.bac", g_gm_gamedat_act_com_arc);
		// アクション初期化
		// リング
		act_3dspr = &gm_ring_sys_work->ring_act3d;
		mtAct3dInitStructSprite(act_3dspr, 0, bac,
					ACT_RING_KURUKURU_3D_ID, MTD_ACT_FLAG_REPEAT | MTD_ACT_FLAG_NO_REQ_PLT | MTD_ACT_FLAG_CLIP, 
					mtVramAllocTex(ACT_RING_KURUKURU_3D_TEX_SIZE, FALSE),
					mtVramAllocTexPlt(ACT_RING_KURUKURU_3D_TEX_PLT_NUM, FALSE));
		// Y軸ビルボードに設定する
		act_3dspr->a3d.ge_op[0] = MTE_ACT3D_GE_OP_GLB_LOAD_BB_Y;//MTE_ACT3D_GE_OP_BASE_BB_Y;
		act_3dspr->a3d.ge_op[1] = MTE_ACT3D_GE_OP_CUR_LOAD_FLUSH_ONLY_P;//MTE_ACT3D_GE_OP_FLUSH_ONLY_P;
		act_3dspr->a3d.scale.x =
			act_3dspr->a3d.scale.y =
			act_3dspr->a3d.scale.z = FX32_ONE;
		// パレット転送
		mtAct3dUpdateSprite(act_3dspr, NULL, 0);
		act_3dspr->act.flag |= MTD_ACT_FLAG_NO_PLT;			// 以後パレット転送を行わない

		// 消滅エフェクト
		act_3dspr = &gm_ring_sys_work->twinkle_act3d;
		mtAct3dInitStructSprite(act_3dspr, 0, bac,
					ACT_RING_KIRAKIRA_3D_ID, MTD_ACT_FLAG_NO_REQ_CHA | MTD_ACT_FLAG_NO_REQ_PLT | MTD_ACT_FLAG_CLIP, 
					mtVramAllocTex(ACT_RING_KIRAKIRA_3D_TEX_SIZE, FALSE),
					mtVramAllocTexPlt(ACT_RING_KIRAKIRA_3D_TEX_PLT_NUM, FALSE));
		// Y軸ビルボードに設定する
		act_3dspr->a3d.ge_op[0] = MTE_ACT3D_GE_OP_GLB_LOAD_BB_Y;//MTE_ACT3D_GE_OP_BASE_BB_Y;
		act_3dspr->a3d.ge_op[1] = MTE_ACT3D_GE_OP_CUR_LOAD_FLUSH_ONLY_P;//MTE_ACT3D_GE_OP_FLUSH_ONLY_P;
		act_3dspr->a3d.scale.x =
			act_3dspr->a3d.scale.y =
			act_3dspr->a3d.scale.z = FX32_ONE;
		// キャラ, パレット転送
		mtAct3dUpdateSprite(act_3dspr, NULL, 0);
		act_3dspr->act.flag |= MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_PLT | MTD_ACT_FLAG_CARRY_TIMER;	// 以後キャラ, パレット転送を行わない, 再生時間持ち越し

		// パレット設定
//		bac = ObjDataLoad(NULL, "/ac_itm_ring.bac", g_gm_gamedat_act_com_arc);
//		ObjPaletteSet(bac, 0, GMD_PLT_RING, MTE_GE2_A);
//		ObjPaletteSet(bac, 0, GMD_PLT_RING, MTE_GE2_B);
	}
	else {
	// 通常ステージ(2D)
		MTS_ACTION_DS		*act_ds;

		/* 描画関数設定 */
		gm_ring_sys_work->ring_draw_func	= gmRingDrawFuncRing2D;
		gm_ring_sys_work->twinkle_draw_func	= gmRingDrawFuncTwinkle2D;

		/* データロード */
		bac = ObjDataLoad(NULL, "/ac_itm_ring.bac", g_gm_gamedat_act_com_arc);
		// アクション初期化
		// リング
		act_ds = &gm_ring_sys_work->ring_act;
		mtActInitStructDS(act_ds, bac, ACT_RING_KURUKURU_ID,
					0, MTD_ACT_FLAG_REPEAT | MTD_ACT_FLAG_NO_PLT | MTD_ACT_FLAG_CLIP,			// パレット転送無し
					MTE_CHA_VRAM_ADDRESS, (u32)mtVramAllocObj(MTE_GE2_A, ACT_RING_KURUKURU_CHA64KB),
					MTE_PLT_VRAM_ADDRESS, HW_OBJ_PLTT,
					MTE_CHA_VRAM_ADDRESS, (u32)mtVramAllocObj(MTE_GE2_B, ACT_RING_KURUKURU_CHA64KB),
					MTE_PLT_VRAM_ADDRESS, HW_DB_OBJ_PLTT,
					GMD_OBJ_BG_PRIO_3D, GMD_ACT_PRIO_RING);
		// パレットオフセット設定
		act_ds->plt_ofst_no[MTE_GE2_A] = GMD_PLT_RING;
		act_ds->plt_ofst_no[MTE_GE2_B] = GMD_PLT_RING;

		// 消滅エフェクト(OPEデータ)
		act_ds = &gm_ring_sys_work->twinkle_act;
		mtActInitStructDS(act_ds, bac, ACT_RING_KIRAKIRA_ID,
					0, MTD_ACT_FLAG_NO_REQ_CHA | MTD_ACT_FLAG_NO_PLT | MTD_ACT_FLAG_CLIP,		// パレット転送無し
					MTE_CHA_VRAM_ADDRESS, (u32)mtVramAllocObj(MTE_GE2_A, ACT_RING_KIRAKIRA_CHA64KB),
					MTE_PLT_VRAM_ADDRESS, HW_OBJ_PLTT,
					MTE_CHA_VRAM_ADDRESS, (u32)mtVramAllocObj(MTE_GE2_B, ACT_RING_KIRAKIRA_CHA64KB),
					MTE_PLT_VRAM_ADDRESS, HW_DB_OBJ_PLTT,
					GMD_OBJ_BG_PRIO_3D, GMD_ACT_PRIO_RING);
		// パレットオフセット設定
		act_ds->plt_ofst_no[MTE_GE2_A] = GMD_PLT_RING;
		act_ds->plt_ofst_no[MTE_GE2_B] = GMD_PLT_RING;
		// キャラ転送
		mtActUpdateDS(act_ds, NULL, 0);
		act_ds->act.flag |= MTD_ACT_FLAG_NO_CHA | MTD_ACT_FLAG_NO_PLT | MTD_ACT_FLAG_CARRY_TIMER;		// 以後キャラ, パレット転送を行わない, 再生時間持ち越し

		// パレット設定
		ObjPaletteSet(bac, 0, GMD_PLT_RING, MTE_GE2_A);
		ObjPaletteSet(bac, 0, GMD_PLT_RING, MTE_GE2_B);
	}
#endif

}

// ==========================================================================
// 終了処理
// ==========================================================================
// ==========================================================================
// GmRingExit
/*!
 *	リング終了処理
 */
// ==========================================================================
void GmRingExit(void)
{
	if (gm_ring_tcb) {
		mtTaskClearTcb(gm_ring_tcb);
	}
}

// ==========================================================================
// リング作成
// ==========================================================================
// ==========================================================================
// GmRingCreateDamageRing
/*!
 *	ダメージリング作成
 *
 *	@param	pos_x	[in] 発生座標
 *	@param	pos_y	[in]
 *	@param	pos_z	[in]
 *	@param	spd_x	[in] 初速
 *	@param	spd_y	[in]
 *	@param	flag	[in] フラグ
 *
 *	@return	GMS_RING_WORK アドレス
 */
// ==========================================================================
GMS_RING_WORK* GmRingCreateDamageRing(fx32 pos_x, fx32 pos_y, fx32 pos_z, fx32 spd_x, fx32 spd_y, u16 flag)
{
	GMS_RING_WORK	*ring_work;

	if (gm_ring_sys_work == NULL) {
		MTM_ASSERT(0);
		return (NULL);
	}

	// リングワーク取得
	ring_work = gmRingAllocRingWork();
	if (ring_work == NULL) {
		// リングワーク最大数オーバー
		return (NULL);
	}

	ring_work->pos.x	= pos_x;
	ring_work->pos.y	= pos_y;
	ring_work->pos.z	= pos_z;
	ring_work->spd_x	= spd_x;
	ring_work->spd_y	= spd_y;
	ring_work->scale.x	= 
		ring_work->scale.y	= 
		ring_work->scale.z	= FX32_ONE;//gm_ring_scale; 倍角にするには数が多すぎる...
	ring_work->timer	= (s16)(GMD_RING_TIMER + (mtMathRand() & 0x1f));
	ring_work->flag		= flag;
	ring_work->eve_rec	= NULL;
	ring_work->duct_obj	= NULL;

	// 逆重力チェック
//	if (GmReverseCheck(pos_x, pos_y)) {
//		ring_work->flag |= GMD_RING_REVERSE;
//	}
//	else {
//		ring_work->flag &= ~GMD_RING_REVERSE;
//	}

	// ダメージリングリストに追加
	gmRingAttachDamageRingList(ring_work);

	return (ring_work);
}

// ==========================================================================
// GmRingCreate
/*!
 *	通常リング作成
 *
 *	@param	eve_rec	[in]	レコードポインタ
 *	@param	pos_x	[in]	発生座標
 *	@param	pos_y	[in]
 *	@param	pos_z	[in]
 *
 *	@return	GMS_RING_WORK アドレス
 */
// ==========================================================================
GMS_RING_WORK* GmRingCreate(GMS_EVE_RECORD_RING* eve_rec, s32 pos_x, s32 pos_y, fx32 pos_z)
{
	GMS_RING_WORK	*ring_work;

	if (gm_ring_sys_work == NULL) {
		MTM_ASSERT(0);
		return (NULL);
	}

//	if (GMM_MAIN_IS_MISSION_RING_LIMIT()) {
//		// ミッション リング制限
//		if (eve_rec) {
//			eve_rec->pos_x	= GMD_EVE_RECORD_CMD_SKIP;
//		}
//		return (NULL);
//	}

	// リングワーク取得
	ring_work = gmRingAllocRingWork();
	if (ring_work == NULL) {
		// リングワーク最大数オーバー
		return (NULL);
	}

	ring_work->pos.x	= pos_x;
	ring_work->pos.y	= pos_y;
	ring_work->pos.z	= pos_z;
	ring_work->spd_x	= 0;
	ring_work->spd_y	= 0;
	ring_work->scale.x	=
		ring_work->scale.y	=
		ring_work->scale.z	= gm_ring_scale;
	ring_work->timer	= 0;
	ring_work->flag		= 0;
	if (eve_rec) {
		eve_rec->pos_x	= GMD_EVE_RECORD_CMD_SKIP;
	}
	ring_work->eve_rec	= eve_rec;
	ring_work->duct_obj	= NULL;

//	// 逆重力チェック
//	if (GmReverseCheck(pos_x, pos_y)) {
//		ring_work->flag |= GMD_RING_REVERSE;
//	}
//	else {
//		ring_work->flag &= ~GMD_RING_REVERSE;
//	}

	// 通常リングリストに追加
	gmRingAttachRingList(ring_work);

	return (ring_work);
}

// ==========================================================================
// GmRingCreateSlotRing
/*!
 *	スロットリング作成
 *
 *	@param	pos_x		[in] 発生座標
 *	@param	pos_y		[in]
 *	@param	pos_z		[in]
 *	@param	target_obj	[in] ターゲットオブジェクト
 *
 *	@return	GMS_RING_WORK アドレス
 */
// ==========================================================================
GMS_RING_WORK* GmRingCreateSlotRing(OBS_OBJECT_WORK *target_obj, fx32 dist, u16 dir)
{
	GMS_RING_WORK	*ring_work;

	if (gm_ring_sys_work == NULL) {
		MTM_ASSERT(0);
		return (NULL);
	}

	// リングワーク取得
	ring_work = gmRingAllocRingWork();
	if (ring_work == NULL) {
		// リングワーク最大数オーバー
		return (NULL);
	}

	ring_work->pos.x	= target_obj->pos.x + FX_Mul(dist, mtMathCos(dir));
	ring_work->pos.y	= target_obj->pos.y + FX_Mul(-dist, mtMathSin(dir));
	ring_work->pos.z	= target_obj->pos.z;

	ring_work->spd_x	= (fx32)FX_Mul(GMD_RING_SLOT_SPD, mtMathCos((u16)(dir + 0x8000)));
	ring_work->spd_y	= (fx32)FX_Mul(-GMD_RING_SLOT_SPD, mtMathSin((u16)(dir + 0x8000)));
	ring_work->scale.x	= 
		ring_work->scale.y	= 
		ring_work->scale.z	= FX32_ONE;//gm_ring_scale; 倍角にするには数が多すぎる...
	ring_work->timer	= 0;
	ring_work->flag		= 0;
	ring_work->eve_rec	= NULL;
	ring_work->duct_obj	= NULL;

	// 逆重力チェック
//	if (GmReverseCheck(pos_x, pos_y)) {
//		ring_work->flag |= GMD_RING_REVERSE;
//	}
//	else {
//		ring_work->flag &= ~GMD_RING_REVERSE;
//	}

	// スロットリングリストに追加
	gmRingAttachSlotRingList(ring_work);

	return (ring_work);
}

// ==========================================================================
// GmRingCheckRestSlotRing
/*!
 *	スロットリングが残っているかチェック
 *
 *	@return	TRUE : 未生成スロットリング, もしくは未取得スロットリングあり
 */
// ==========================================================================
BOOL GmRingCheckRestSlotRing(void)
{
	if (gm_ring_sys_work == NULL) {
		MTM_ASSERT(0);
		return (FALSE);
	}

	if (gm_ring_sys_work->wait_slot_ring_num ||
			gm_ring_sys_work->slot_ring_list_start) {
		return (TRUE);
	}
	return (FALSE);
}

// ==========================================================================
// GmEveRingInit
/*!
 *	リングイベント生成初期化関数
 *
 *	@param	eve_rec	[inout]	レコードポインタ
 *	@param	pos_x	[in]	出現座標
 *	@param	pos_y	[in]
 *	@param	type	[in]	処理内容タイプ 通常は0
 *
 *	@note
 *		デバック配置時のコール用
 */
// ==========================================================================
OBS_OBJECT_WORK* GmEveRingInit(GMS_EVE_RECORD_EVENT *eve_rec , fx32 pos_x,  fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);

	//GmRingCreateDamageRing(pos_x, pos_y, 0, 0, GMD_RING_FLAG_B);
	GmRingCreate((GMS_EVE_RECORD_RING*)eve_rec, pos_x, pos_y, 0);

	return NULL;
}

// ==========================================================================
// GmRingDamageSetNum
/*!
 *	ダメージ時の飛び散りリング作成 数指定有り
 *
 *	@param	ply_work	[in]	プレイヤーオブジェクト
 *	@param	ring_num	[in]	飛び散らせるリング数
 *
 *	@note
 *		所持リング数以上の値を設定された場合は所持リングすべてを放出
 */
// ==========================================================================
void GmRingDamageSetNum(GMS_PLAYER_WORK *ply_work, s16 ring_num)
{
#ifdef MPPDEBUG_INFINITE_LIFE	
	{{ //qqq//test -- infinite
		ring_num = ring_num/3;
	}}
#endif
	
	mppAchievementSupport::get()->event_LoseRingInFinalLevel();
	
	fx32			spd_x = 0, spd_y = 0;
//	s32				ring_num;			// 作成リング数
	s32				i, shift;
	s32				angle = 0x0488;		// 下位1バイト : 角度  上位1バイト : 速度シフト値
	u16				flag;
	u16				dir_fall;
	u8				player_id = ply_work->player_id;

	// リング設定フラグ
	flag = (u16)(GMD_RING_DAMAGE | ((player_id << GMD_RING_DAMAGE_PLAYER_SHIFT) & GMD_RING_DAMAGE_PLAYER));

	if (gm_ring_sys_work == NULL) {
		MTM_ASSERT(0);
		return;
	}

#if 1
	// プレイヤーリング減少
	if (ring_num > ply_work->ring_num) {
		ring_num = ply_work->ring_num;
	}
	else if (ring_num < 0) {
#if defined (MTD_DEBUG)
		OS_TPrintf("gmRing::GmRingDamageSetNum() Warning! damage ring_num minus\n");
#endif	// #if defined (MTD_DEBUG)
		return;
	}
	ply_work->ring_num -= ring_num;
	if (ring_num > GMD_RING_DAMAGE_NUM) {	// 最大作成数チェック
		ring_num = GMD_RING_DAMAGE_NUM;
	}

#else
	-/*
	// プレイヤー所持リングクリア
	ring_num = ply_work->ring_num;
	ply_work->ring_num = 0;
	if (ring_num > GMD_RING_DAMAGE_NUM) {	// 最大作成数チェック
		ring_num = GMD_RING_DAMAGE_NUM;
	}*/
#endif

	// ダメージリング取得チェックフラグ設定
	gm_ring_sys_work->flag |= GMD_RING_SYS_FLAG_DAMAGE_CHK_P1 << player_id;

	// 面設定
	if (ply_work->obj_work.flag & OBD_OBJECT_B/* && !(ply_work->gmk_flag & GMD_PLGF_GMK_TABLE_COL)*/) {
		flag |= GMD_RING_FLAG_B;
	}

	// ダメージ回数に合わせ、速度シフト値を変更
	//angle += gm_ring_sys_work->damage_num * (0x10 * 16);
	angle += gm_ring_sys_work->damage_num[player_id] << 8;

	// 重力対応
	dir_fall = ply_work->obj_work.dir_fall;

	for (i = 0; i < ring_num; i++) {
		if (angle >= 0) {
			// 速度シフト値取得
			shift = angle >> 8;
			shift = shift >= 6 ? -shift + 9 : shift;	// 96枚を超えて出る場合は調整

			spd_x = (mtMathSin((u16)(((angle + dir_fall) & 0xFF) << 8)) << 4) >> shift;
			spd_y = (mtMathCos((u16)(((angle + dir_fall) & 0xFF) << 8)) << 4) >> shift;
			spd_x -= spd_x >> 2/* / 4 */;
			spd_y -= spd_y >> 2/* / 4 */;

			angle = angle + 0x10;	// 角度をすすめる
			angle |= 0x80;			// 17 枚目も左上から
		}

		// ダメージリング作成
		if (GmRingCreateDamageRing(ply_work->obj_work.pos.x, ply_work->obj_work.pos.y, 0, spd_x, spd_y, flag) == FALSE) {
			// リング作成失敗
			break;
		}

		// 次のリングは横対象位置
		angle = -angle;
		spd_x = -spd_x;
	}

	// ダメージ散らばり回数加算
	if (gm_ring_sys_work->damage_num[player_id] < gm_ring_damege_num_tbl[g_gs_main_sys_info.level]) {
		gm_ring_sys_work->damage_num[player_id]++;
	}
}

// ==========================================================================
// GmRingSlotSetNum
/*!
 *	スロット用リング作成 数指定有り
 *
 *	@param	ply_work	[in]	プレイヤーオブジェクト
 *	@param	ring_num	[in]	取得するリング数
 *
 *	@note
 *		所持リング数以上の値を設定された場合は所持リングすべてを放出
 */
// ==========================================================================
void GmRingSlotSetNum(GMS_PLAYER_WORK *ply_work, s32 ring_num)
{
	if (gm_ring_sys_work == NULL || ring_num <= 0) {
		MTM_ASSERT(0);
		return;
	}

	// リング生成待機数をセット
	gm_ring_sys_work->wait_slot_ring_num = ring_num;
	// 角度初期化
	gm_ring_sys_work->slot_ring_create_dir = 0x0000;
	// ターゲットオブジェクト保存
	gm_ring_sys_work->slot_target_obj = (OBS_OBJECT_WORK*)ply_work;
}

// ==========================================================================
// SE
// ==========================================================================
// ==========================================================================
// GmRingGetSE
/*!
 *	リング取得SE再生
 */
// ==========================================================================
void GmRingGetSE(void)
{
	BOOL	b_Left = TRUE;

	if (gm_ring_sys_work->ring_se_cnt >= GMD_RING_SE_PLAY_MAX_PER_FRAME) {
		// フレーム当たりの最大回数再生済み
		return;
	}
#if _IPHONE
	if (gm_ring_sys_work->se_wait > 0) {
		return;
	}
#endif // _IPHONE

	if (gm_ring_sys_work) {
		if (gm_ring_sys_work->flag & GMD_RING_SYS_FLAG_GET_SE_FLIP) {
			b_Left = FALSE;
		}

		gm_ring_sys_work->flag ^= GMD_RING_SYS_FLAG_GET_SE_FLIP;
#if _IPHONE
		gm_ring_sys_work->se_wait = GMD_RING_SE_PLAY_WAIT_FRAME;
#endif // _IPHONE
	}

	// SE
#if 1 //!_IPHONE
	if (b_Left) {
		GmSoundPlaySE("Ring1L", gm_ring_sys_work->h_snd_ring[0]);
	}
	else {
		GmSoundPlaySE("Ring1R", gm_ring_sys_work->h_snd_ring[1]);
	}
#else
	GmSoundPlaySE("Ring1", gm_ring_sys_work->h_snd_ring[0]);
#endif

	// SE再生カウントアップ
	gm_ring_sys_work->ring_se_cnt++;
}

// ==========================================================================
// リングシステム関連
// ==========================================================================
// ==========================================================================
// GmRingGetWork
/*!
 *	リングシステムのワークを取得する
 *
 *	@return   ワーク
 */
// ==========================================================================
GMS_RING_SYS_WORK* GmRingGetWork( void )
{
	return gm_ring_sys_work;
}

// ==========================================================================
// リングシステムフラグ関連
// ==========================================================================
// ==========================================================================
// GmRingGetFlag
/*!
 *	リングシステムのフラグを取得する
 *
 *	@return   フラグ
 */
// ==========================================================================
u32 GmRingGetFlag( void )
{
	if (gm_ring_sys_work == NULL) {
		return 0;
	}

	return (gm_ring_sys_work->flag);
}

// ==========================================================================
// GmRingSetFlag
/*!
 *	リングシステムのフラグを設定する
 *
 *	@param	flag	[in]	フラグ
 */
// ==========================================================================
void GmRingSetFlag(u32 flag)
{
	if (gm_ring_sys_work) {
		gm_ring_sys_work->flag = flag;
	}
}

// ==========================================================================
// リングシステム設定関連
// ==========================================================================
// ==========================================================================
// GmRingSetScale
/*!
 *	リングの基本スケールを設定する
 *
 *	@param	scale	[in]	スケール
 *
 *	@note
 *		通常スプライト時は FX32_ONE*2 まで
 */
// ==========================================================================
void GmRingSetScale(fx32 scale)
{
//	if (!GmMainIsBossStage() && scale > FX32_ONE*2) {
//		scale = FX32_ONE*2;
//	}
	gm_ring_scale = scale;
}

// ==========================================================================
// GmRingGetScale
/*!
 *	リングの基本スケールを取得する
 *
 *	@return	スケール
 */
// ==========================================================================
fx32 GmRingGetScale(void)
{
	return (gm_ring_scale);
}

// ==========================================================================
// 特殊処理チェック
// ==========================================================================
// ==========================================================================
// GmRingMagnetCheck
/*!
 *	リング磁力判定、移動値設定処理
 *
 *	@param	ply_work	[in]	プレイヤーワークポインタ
 *
 *	@note
 *		複数人プレイ時には磁力バリアが存在しないので非対応
 */
// ==========================================================================
void GmRingMagnetCheck(GMS_PLAYER_WORK *ply_work)
{
	UNREFERENCED_PARAMETER(ply_work);
#if 0
	OBS_RECT		mag_rect, ring_rect;
	GMS_RING_WORK	*ring_work, *ring_next;

	if (!(ply_work->player_flag & GMD_PLF_MAGNET)) {
		// 吸い寄せ中でない
		if (gm_ring_sys_work->magnet_ring_list_start) {
			// 磁力吸い寄せリングリストのワークをすべて通常リングに移動
			if (gm_ring_sys_work->ring_list_end) {
				gm_ring_sys_work->ring_list_end->post_ring = gm_ring_sys_work->magnet_ring_list_start;
				gm_ring_sys_work->magnet_ring_list_start->pre_ring = gm_ring_sys_work->ring_list_end;
				gm_ring_sys_work->ring_list_end = gm_ring_sys_work->magnet_ring_list_end;
			}
			else {
				gm_ring_sys_work->ring_list_start = gm_ring_sys_work->magnet_ring_list_start;
				gm_ring_sys_work->ring_list_end = gm_ring_sys_work->magnet_ring_list_end;
			}

			gm_ring_sys_work->magnet_ring_list_start =
						gm_ring_sys_work->magnet_ring_list_end = NULL;
		}
		return;
	}

	// リング矩形設定
	ring_rect.left		= GMD_RING_HIT_LEFT;
	ring_rect.top		= GMD_RING_HIT_TOP;
	ring_rect.right		= GMD_RING_HIT_RIGHT;
	ring_rect.bottom	= GMD_RING_HIT_BOTTOM;
	ring_rect.back		= GMD_RING_HIT_BACK;
	ring_rect.front		= GMD_RING_HIT_FRONT;

	// リング磁力影響矩形設定
	mag_rect.pos.x = ply_work->obj_work.pos.x >> FX32_SHIFT;
	mag_rect.pos.y = ply_work->obj_work.pos.y >> FX32_SHIFT;
	mag_rect.pos.z = ply_work->obj_work.pos.z >> FX32_SHIFT;
	mag_rect.left	= -GMD_RING_MAGNET_RANGE;
	mag_rect.top	= -GMD_RING_MAGNET_RANGE;
	mag_rect.right	= GMD_RING_MAGNET_RANGE;
	mag_rect.bottom	= GMD_RING_MAGNET_RANGE;
	mag_rect.front	= -GMD_RING_MAGNET_RANGE;
	mag_rect.back	= GMD_RING_MAGNET_RANGE;

	ring_rect.pos.z = 0;
	for (ring_next = gm_ring_sys_work->ring_list_start; ring_next;) {	// 通常リングのチェック
		ring_work	= ring_next;
		ring_next	= ring_work->post_ring;

		// リング矩形座標設定
		ring_rect.pos.x = ring_work->pos.x;
		ring_rect.pos.y = ring_work->pos.y;
		//ring_rect.pos.x = ring_work->pos.x >> FX32_SHIFT;
		//ring_rect.pos.y = ring_work->pos.y >> FX32_SHIFT;
		//ring_rect.pos.z = ring_work->pos.z >> FX32_SHIFT;

		// 範囲内チェック
		if (gm_ring_sys_work->rec_func(&mag_rect, &ring_rect)) {
		// 吸い寄せ開始
			// 吸い寄せプレイヤー設定
			ring_work->flag &= ~GMD_RING_MAGNET_PLAYER_MASK;
			ring_work->flag |= ply_work->player_id & GMD_RING_MAGNET_PLAYER_MASK;

			// 磁力吸い寄せリングリストに再接続
			gmRingDetachRingList(ring_work);
			gmRingAttachMagnetRingList(ring_work);
		}
	}
#endif
}

// ==========================================================================
// GmRingDuctCheck
/*!
 *	リングダクト判定、移動値設定処理
 *
 *	@param	obj_work	[in]	オブジェクトワークポインタ
 *
 *	@return	0 : 吸い込み無し	1 : 吸い込み有り
 */
// ==========================================================================
u16 GmRingDuctCheck(OBS_OBJECT_WORK* obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);
#if 0	// ◆
 //   GMS_RING_SYS_WORK  * ring_work;
	GMS_RING_WORK * pRing;
	OBS_RECT_WORK tMag = {0}; // 判定矩形
	u16 ucRingNumTemp;   // リング枚数
	u16 i;
	u16 usRet = 0;
	if ( gm_ring_sys_work == NULL )
		return usRet;
//	ring_work = (GMS_RING_SYS_WORK*)mtTaskGetTcbWork( gm_ring_tcb );

	// マグネットフラグを立てる
	gm_ring_sys_work->flag |= GMD_RING_SYS_FLAG_MAGNET;
	
	// 矩形設定
	tMag.sPosX = (obj_work->lPosX >> 8);
	tMag.sPosY = (obj_work->lPosY >> 8);
	objRectSet( &tMag, -GMD_RING_DUCT_RANGE, -GMD_RING_DUCT_RANGE, GMD_RING_DUCT_RANGE, GMD_RING_DUCT_RANGE);
	
	// Checkする枚数
	ucRingNumTemp = gm_ring_sys_work->ucRingNum;

	// リング最大数分ループ
	for ( i = 0; i < GMD_RING_MOVE_NUM; ++i ){
		if ( !ucRingNumTemp )
			break;
		
		pRing = &gm_ring_sys_work->tRing[ i ];
		// リング存在チェック
		if ( pRing->timer ){
			--ucRingNumTemp; // 存在枚数チェック用

			// ダメージリングなら判定を行う
			if ( pRing->flag & GMD_RING_DAMAGE && !(pRing->flag & (GMD_RING_DUCT)) &&
				 (GMD_RING_TIMER - GMD_RING_NOHIT_TIMER) > pRing->timer ){
				// 矩形判定
				if (objRectCheckDetail( &tMag, (pRing->lPosX >> 8), (pRing->lPosY >> 8),
										GMD_RING_HIT_LEFT,GMD_RING_HIT_TOP, GMD_RING_HIT_WIDTH , GMD_RING_HIT_HEIGHT)){

					// 吸い込まれ状態へ
					pRing->flag |= GMD_RING_NOFALL;
					pRing->flag |= GMD_RING_NOMOVE;
					pRing->flag |= GMD_RING_NOCOL;
					pRing->flag |= GMD_RING_NOHIT;
					pRing->flag |= GMD_RING_DUCT;
					pRing->sSpdX = 0;
					pRing->sScaleX = 0x0100;
					pRing->sScaleY = 0x0100;
					pRing->duct_obj = obj_work;
					// 吸い込みあり
					usRet = 1;
				}
			}
			if ( pRing->flag & GMD_RING_DUCT && pRing->duct_obj == obj_work ){
				s16 sSpdX,sSpdY;
				u16 usDir;
				// 向きを計算
				usDir = mtMathAtan2( (tMag.sPosY - (pRing->lPosY >> 8)), (tMag.sPosX - (pRing->lPosX >> 8)) );

				usDir += 0x1000;
				
				// 加速
				pRing->sSpdX = (s16)objSpdUpSet( pRing->sSpdX, GMD_RING_DUCT_ACC, GMD_RING_DUCT_MSPD );
				
				sSpdX = (s16)( ((pRing->sSpdX ) * mtMathCos( usDir )) >> 12 );
				sSpdY = (s16)( ((pRing->sSpdX ) * mtMathSin( usDir )) >> 12 );
				
				// 移動を行う
				pRing->lPosX += sSpdX;
				pRing->lPosY += sSpdY;
				pRing->sScaleX -= 0x0002;
				if ( pRing->sScaleX < 0x0040 )
					pRing->sScaleX = 0x0040;
				pRing->sScaleY -= 0x0002; 
				if ( pRing->sScaleY < 0x0040 )
					pRing->sScaleY = 0x0040;
				
				// 近づきすぎたので
				if ( MTM_MATH_ABS(pRing->lPosX - obj_work->lPosX) < 0x400 &&
					 MTM_MATH_ABS(pRing->lPosY - obj_work->lPosY) < 0x400 ){
					// 消える
					pRing->timer = 0;
				}
				
				// 吸い込みあり
				usRet = 1;
			}
		}
	}
	return usRet;
#else
	return 0;
#endif
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// リングワーク管理
// ==========================================================================
// ==========================================================================
// gmRingAllocRingWork
//! リングワーク取得
/*!
 *	@return		リングワーク
 */
// ==========================================================================
GMS_RING_WORK* gmRingAllocRingWork(void)
{
	GMS_RING_WORK	*ring_work;

	if (gm_ring_sys_work->ring_list_cnt >= GMD_RING_MAX_NUM) {
		OS_TPrintf("gmRing : ring object full");
		MTM_ASSERT(0);
		return (NULL);
	}

	ring_work = gm_ring_sys_work->ring_list[gm_ring_sys_work->ring_list_cnt];
	gm_ring_sys_work->ring_list_cnt++;

	return (ring_work);
}
// ==========================================================================
// gmRingFreeRingWork
//! リングワーク解放
/*!
 *	@param	ring_work	[in]	解放するリングワーク
 */
// ==========================================================================
void gmRingFreeRingWork(GMS_RING_WORK *ring_work)
{
	MTM_ASSERT(gm_ring_sys_work->ring_list_cnt > 0);

	gm_ring_sys_work->ring_list_cnt--;
	gm_ring_sys_work->ring_list[gm_ring_sys_work->ring_list_cnt] = ring_work;
}

// ==========================================================================
// リングリスト管理
// ==========================================================================
// 通常リング
// ==========================================================================
// gmRingAttachRingList
//! 通常固定リングリストにリングワークを連結
/*!
 *	@param	ring_work	[in]	連結するリングワーク
 */
// ==========================================================================
void gmRingAttachRingList(GMS_RING_WORK *ring_work)
{
	if (gm_ring_sys_work->ring_list_end) {
		gm_ring_sys_work->ring_list_end->post_ring = ring_work;
		ring_work->pre_ring		= gm_ring_sys_work->ring_list_end;
		ring_work->post_ring	= NULL;
		gm_ring_sys_work->ring_list_end = ring_work;
	}
	else {
		gm_ring_sys_work->ring_list_start = gm_ring_sys_work->ring_list_end = ring_work;
		ring_work->pre_ring = ring_work->post_ring = NULL;
	}
}
// ==========================================================================
// gmRingDetachRingList
//! 通常固定リングリストからリングワークを切り離し
/*!
 *	@param	ring_work	[in]	切り離すリングワーク
 */
// ==========================================================================
void gmRingDetachRingList(GMS_RING_WORK *ring_work)
{
	if (ring_work->pre_ring == NULL) {
	// 先頭
		gm_ring_sys_work->ring_list_start = ring_work->post_ring;
	}
	else {
		ring_work->pre_ring->post_ring = ring_work->post_ring;
	}

	if (ring_work->post_ring == NULL) {
		gm_ring_sys_work->ring_list_end = ring_work->pre_ring;
	}
	else {
		ring_work->post_ring->pre_ring = ring_work->pre_ring;
	}
}

// 取得エフェクト
#if 0
// ==========================================================================
// gmRingAttachTwinkleList
//! 取得エフェクトリストにリングワークを連結
/*!
 *	@param	ring_work	[in]	連結するリングワーク
 */
// ==========================================================================
void gmRingAttachTwinkleList(GMS_RING_WORK *ring_work)
{
	if (gm_ring_sys_work->twinkle_list_end) {
		gm_ring_sys_work->twinkle_list_end->post_ring = ring_work;
		ring_work->pre_ring	= gm_ring_sys_work->twinkle_list_end;
		ring_work->post_ring	= NULL;
		gm_ring_sys_work->twinkle_list_end = ring_work;
	}
	else {
		gm_ring_sys_work->twinkle_list_start = gm_ring_sys_work->twinkle_list_end = ring_work;
		ring_work->pre_ring = ring_work->post_ring = NULL;
	}
}
// ==========================================================================
// gmRingDetachTwinkleList
//! 取得エフェクトリストからリングワークを切り離し
/*!
 *	@param	ring_work	[in]	切り離すリングワーク
 */
// ==========================================================================
void gmRingDetachTwinkleList(GMS_RING_WORK *ring_work)
{
	if (ring_work->pre_ring == NULL) {
	// 先頭
		gm_ring_sys_work->twinkle_list_start = ring_work->post_ring;
	}
	else {
		ring_work->pre_ring->post_ring = ring_work->post_ring;
	}

	if (ring_work->post_ring == NULL) {
		gm_ring_sys_work->twinkle_list_end = ring_work->pre_ring;
	}
	else {
		ring_work->post_ring->pre_ring = ring_work->pre_ring;
	}
}
#endif

// 磁力吸い寄せリング
#if 0
// ==========================================================================
// gmRingAttachMagnetRingList
//! 磁力吸い寄せリングリストにリングワークを連結
/*!
 *	@param	ring_work	[in]	連結するリングワーク
 */
// ==========================================================================
void gmRingAttachMagnetRingList(GMS_RING_WORK *ring_work)
{
	if (gm_ring_sys_work->magnet_ring_list_end) {
		gm_ring_sys_work->magnet_ring_list_end->post_ring = ring_work;
		ring_work->pre_ring		= gm_ring_sys_work->magnet_ring_list_end;
		ring_work->post_ring	= NULL;
		gm_ring_sys_work->magnet_ring_list_end = ring_work;
	}
	else {
		gm_ring_sys_work->magnet_ring_list_start = gm_ring_sys_work->magnet_ring_list_end = ring_work;
		ring_work->pre_ring = ring_work->post_ring = NULL;
	}
}
// ==========================================================================
// gmRingDetachMagnetRingList
//! 磁力吸い寄せリングリストからリングワークを切り離し
/*!
 *	@param	ring_work	[in]	切り離すリングワーク
 */
// ==========================================================================
void gmRingDetachMagnetRingList(GMS_RING_WORK *ring_work)
{
	if (ring_work->pre_ring == NULL) {
	// 先頭
		gm_ring_sys_work->magnet_ring_list_start = ring_work->post_ring;
	}
	else {
		ring_work->pre_ring->post_ring = ring_work->post_ring;
	}

	if (ring_work->post_ring == NULL) {
		gm_ring_sys_work->magnet_ring_list_end = ring_work->pre_ring;
	}
	else {
		ring_work->post_ring->pre_ring = ring_work->pre_ring;
	}
}
#endif

// ダメージ飛び散りリング
// ==========================================================================
// gmRingAttachDamageRingList
//! ダメージ飛び散りリングリストにリングワークを連結
/*!
 *	@param	ring_work	[in]	連結するリングワーク
 */
// ==========================================================================
void gmRingAttachDamageRingList(GMS_RING_WORK *ring_work)
{
	if (gm_ring_sys_work->damage_ring_list_end) {
		gm_ring_sys_work->damage_ring_list_end->post_ring = ring_work;
		ring_work->pre_ring	= gm_ring_sys_work->damage_ring_list_end;
		ring_work->post_ring	= NULL;
		gm_ring_sys_work->damage_ring_list_end = ring_work;
	}
	else {
		gm_ring_sys_work->damage_ring_list_start = gm_ring_sys_work->damage_ring_list_end = ring_work;
		ring_work->pre_ring = ring_work->post_ring = NULL;
	}
}
// ==========================================================================
// gmRingDetachDamageRingList
//! ダメージ飛び散りリングリストからリングワークを切り離し
/*!
 *	@param	ring_work	[in]	切り離すリングワーク
 */
// ==========================================================================
void gmRingDetachDamageRingList(GMS_RING_WORK *ring_work)
{
	if (ring_work->pre_ring == NULL) {
	// 先頭
		gm_ring_sys_work->damage_ring_list_start = ring_work->post_ring;
	}
	else {
		ring_work->pre_ring->post_ring = ring_work->post_ring;
	}

	if (ring_work->post_ring == NULL) {
		gm_ring_sys_work->damage_ring_list_end = ring_work->pre_ring;
	}
	else {
		ring_work->post_ring->pre_ring = ring_work->pre_ring;
	}
}

// スロットリング
// ==========================================================================
// gmRingAttachSlotRingList
//! スロットリングリストにリングワークを連結
/*!
 *	@param	ring_work	[in]	連結するリングワーク
 */
// ==========================================================================
void gmRingAttachSlotRingList(GMS_RING_WORK *ring_work)
{
	if (gm_ring_sys_work->slot_ring_list_end) {
		gm_ring_sys_work->slot_ring_list_end->post_ring = ring_work;
		ring_work->pre_ring		= gm_ring_sys_work->slot_ring_list_end;
		ring_work->post_ring	= NULL;
		gm_ring_sys_work->slot_ring_list_end = ring_work;
	}
	else {
		gm_ring_sys_work->slot_ring_list_start = gm_ring_sys_work->slot_ring_list_end = ring_work;
		ring_work->pre_ring = ring_work->post_ring = NULL;
	}
}
// ==========================================================================
// gmRingDetachSlotRingList
//! スロットリングリストからリングワークを切り離し
/*!
 *	@param	ring_work	[in]	切り離すリングワーク
 */
// ==========================================================================
void gmRingDetachSlotRingList(GMS_RING_WORK *ring_work)
{
	if (ring_work->pre_ring == NULL) {
	// 先頭
		gm_ring_sys_work->slot_ring_list_start = ring_work->post_ring;
	}
	else {
		ring_work->pre_ring->post_ring = ring_work->post_ring;
	}

	if (ring_work->post_ring == NULL) {
		gm_ring_sys_work->slot_ring_list_end = ring_work->pre_ring;
	}
	else {
		ring_work->post_ring->pre_ring = ring_work->pre_ring;
	}
}

// ==========================================================================
// 終了処理
// ==========================================================================
// ==========================================================================
// gmRingDest
/*!
 *	リング管理タスク解放
 *
 *	@param	tcb	[in]	タスクポインタ
 */
// ==========================================================================
void gmRingDest(MTS_TASK_TCB *tcb)
{
	s32				i;
	GMS_RING_WORK	*ring_work;

	UNREFERENCED_PARAMETER(tcb);

	// 取得していない通常リングイベントを復旧する
	for (ring_work = gm_ring_sys_work->ring_list_start; ring_work; ring_work = ring_work->post_ring) {
		if (ring_work->eve_rec) {
			ring_work->eve_rec->pos_x = (u8)((ring_work->pos.x >> FX32_SHIFT) & 0xFF);
		}
	}

	/* SEハンドル破棄 */
	for (i = 0; i < 2; i++) {
		if (gm_ring_sys_work->h_snd_ring[i]) {
			GmSoundStopSE(gm_ring_sys_work->h_snd_ring[i]);
			GsSoundFreeSeHandle(gm_ring_sys_work->h_snd_ring[i]);
			gm_ring_sys_work->h_snd_ring[i] = NULL;
		}
	}

	/* リングアクション開放 */
#if 0
	if (GmMainIsBossStage()) {
		// ボスステージ(3D)
		mtAct3dReleaseStructSprite(&gm_ring_sys_work->ring_act3d);
		mtAct3dReleaseStructSprite(&gm_ring_sys_work->twinkle_act3d);
	}
	else {
		// 通常ステージ(2D)
		mtActReleaseStructDS(&gm_ring_sys_work->ring_act);
		mtActReleaseStructDS(&gm_ring_sys_work->twinkle_act);
	}
#endif

	gm_ring_tcb = NULL;
	gm_ring_sys_work = NULL;
}

// ==========================================================================
// メイン
// ==========================================================================
// ==========================================================================
// gmRingMain
/*!
 *	リング管理メイン
 */
// ==========================================================================
void gmRingMain(MTS_TASK_TCB *tcb)
{
	OBS_RECT		ply_rect[GSD_MAIN_PLAYER_MAX], ring_rect = {0};
	s32				i;
	BOOL			is_hit;								// リングヒット状況
	GMS_RING_WORK	*ring_work, *ring_next;
	GMS_PLAYER_WORK	*ply_work;

	UNREFERENCED_PARAMETER(tcb);

	// 非表示
	if (gm_ring_sys_work->flag & GMD_RING_SYS_FLAG_NODISP) {
		// SE再生カウンタクリア
		gm_ring_sys_work->ring_se_cnt = 0;
		return;
	}
	
	// カメラ設定
	if (g_obj.glb_camera_id >= 0) {
		ObjDraw3DNNSetCamera(g_obj.glb_camera_id, g_obj.glb_camera_type);
	}

#if _IPHONE
	if (gm_ring_sys_work->se_wait > 0) {
		gm_ring_sys_work->se_wait--;
	}
	gmRingDrawBegin();
#endif //_IPHONE

	// ポーズ中チェック
	if (ObjObjectPauseCheck(0)) {
		// 描画のみ行う
		/* 通常固定リング */
		for (ring_next = gm_ring_sys_work->ring_list_start; ring_next;) {
			ring_work	= ring_next;
			ring_next	= ring_work->post_ring;
			// 描画
			gm_ring_sys_work->ring_draw_func(ring_work);
		}

		/* ダメージ飛び散りリング */
		for (ring_next = gm_ring_sys_work->damage_ring_list_start; ring_next;) {
			ring_work	= ring_next;
			ring_next	= ring_work->post_ring;
			// 描画
			if (ring_work->timer > GMD_RING_BLINK_TIMER || ring_work->timer & 0x02) {
				gm_ring_sys_work->ring_draw_func(ring_work);
			}
		}
		/* スロットギフトリング */
		for (ring_next = gm_ring_sys_work->slot_ring_list_start; ring_next;) {
			ring_work	= ring_next;
			ring_next	= ring_work->post_ring;
			// 描画
			gm_ring_sys_work->ring_draw_func(ring_work);
		}
#if _IPHONE
		gmRingDrawEnd();
#endif //_IPHONE

		// SE再生カウンタクリア
		gm_ring_sys_work->ring_se_cnt = 0;
		return;
	}

	// プレイヤー矩形取得
	MTM_ASSERT(gm_ring_sys_work->player_num <= GSD_MAIN_PLAYER_MAX);
	for (i = 0; i < gm_ring_sys_work->player_num; i++) {
		OBS_RECT_WORK	*rect_work;
		s16			temp1, temp2;

		ply_work		= g_gm_main_system.ply_work[i];
		rect_work	= &ply_work->rect_work[GMD_PLAYER_RECT_BODY];

		// 座標
		//ply_rect[i].pos.x = ply_work->obj_work.pos.x >> FX32_SHIFT;
		//ply_rect[i].pos.y = ply_work->obj_work.pos.y >> FX32_SHIFT;
		//ply_rect[i].pos.z = ply_work->obj_work.pos.z >> FX32_SHIFT;
		ply_rect[i].pos.x = ply_work->obj_work.pos.x;
		ply_rect[i].pos.y = ply_work->obj_work.pos.y;
		ply_rect[i].pos.z = ply_work->obj_work.pos.z;

		// X
		if ((ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) ^ (rect_work->flag & OBD_DISP_HFLIP)) {
			temp1 = (s16)(-rect_work->rect.right);
			temp2 = (s16)(-rect_work->rect.left);
		}
		else {
			temp1 = rect_work->rect.left;
			temp2 = rect_work->rect.right;
		}
		if (ply_work->obj_work.scale.x != FX32_ONE) {
			temp1 = (s16)FX_Mul(temp1, ply_work->obj_work.scale.x);
			temp2 = (s16)FX_Mul(temp2, ply_work->obj_work.scale.x);
		}
		ply_rect[i].left = temp1;
		ply_rect[i].right = temp2;

		// Y
		if ((ply_work->obj_work.disp_flag & OBD_DISP_VFLIP) ^ (rect_work->flag & OBD_RECT_VFLIP)) {
			temp1 = (s16)(-rect_work->rect.bottom);
			temp2 = (s16)(-rect_work->rect.top);
		}
		else {
			temp1 = rect_work->rect.top;
			temp2 = rect_work->rect.bottom;
		}
		if (ply_work->obj_work.scale.y != FX32_ONE) {
			temp1 = (s16)FX_Mul(temp1, ply_work->obj_work.scale.y);
			temp2 = (s16)FX_Mul(temp2, ply_work->obj_work.scale.y);
		}
		ply_rect[i].top = temp1;
		ply_rect[i].bottom = temp2;

		// Z
		//temp1 = rect_work->rect.back;
		//temp2 = rect_work->rect.front;
		//if (ply_work->obj_work.scale.z != FX32_ONE) {
		//	temp1 = (s16)FX_Mul(temp1, ply_work->obj_work.scale.z);
		//	temp2 = (s16)FX_Mul(temp2, ply_work->obj_work.scale.z);
		//}
		//ply_rect[i].back = temp1;
		//ply_rect[i].front = temp2;
		ply_rect[i].back = 0;
		ply_rect[i].front = 0;
	}
	// リング矩形設定
	ring_rect.left		= GMD_RING_HIT_LEFT;
	ring_rect.top		= GMD_RING_HIT_TOP;
	ring_rect.right		= GMD_RING_HIT_RIGHT;
	ring_rect.bottom	= GMD_RING_HIT_BOTTOM;
	ring_rect.back		= GMD_RING_HIT_BACK;
	ring_rect.front		= GMD_RING_HIT_FRONT;

#if 0	// 特殊当たり関係チェック◆
	// ZONE3-1特殊処理
	if ( _nl_global.zone == 2 ){
		// ベルトアクション時は地形当たり変更
		// if ( _obj_flag & OBD_OBJ_BELT ){
		if ( _nl_player[0]->ulGimmickFlag & NLD_PLGF_GMK_TABLE_COL ){
			objActionBgPrioritySet( &gm_ring_sys_work->mAct[0], &gm_ring_sys_work->mActB[0], 1 );
			gm_ring_sys_work->col_func	= gmRingMoveCollsionObject;
		}
		else{
			objActionBgPrioritySet( &gm_ring_sys_work->mAct[0], &gm_ring_sys_work->mActB[0], GMD_OBJ_BG_PRIO_B );
			gm_ring_sys_work->col_func	= gmRingMoveCollsion;
		}
	}
#endif

	/* リングアクション更新 */
#if !_IPHONE
	gm_ring_sys_work->dir += GMD_RING_ROT_SPD;
#else //!_IPHONE
#if !defined GMD_DEBUG_NO_CREATE_RING
#if defined GMD_RING_DRAW_MODEL
	u32 motion_flag = OBD_DISP_REPEAT;
	ObjDrawAction3DNNMaterialUpdate(gm_ring_obj_3d, &motion_flag);
#endif // GMD_RING_DRAW_MODEL
#if defined GMD_RING_DRAW_PRIMITIVE
	if (++gm_ring_sys_work->draw_ring_uv_frame >= GMD_RING_ROLL_UV_FRAME_MAX) {
		gm_ring_sys_work->draw_ring_uv_frame = 0;
	}
#endif // GMD_RING_DRAW_PRIMITIVE
#endif // GMD_DEBUG_NO_CREATE_RING
#endif //!_IPHONE
	// 回転
//	if (GmMainIsBossStage()) {
//		// ボスステージ(3D)
//		mtAct3dUpdateSprite(&gm_ring_sys_work->ring_act3d, NULL, 0);
//	}
//	else {
//		// 通常ステージ(2D)
//		mtActUpdateDS(&gm_ring_sys_work->ring_act, NULL, 0);
//	}

////
	/* リング重力更新 */
	if (g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->player_flag & GMD_PLF_TRUCK_RIDE) {
		// トロッコの時は重力方向を変更

		gm_ring_fall_acc_x = (fx16)FX_Mul(-GMD_RING_FALL_ACC,
						mtMathSin(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work.dir_fall));
		gm_ring_fall_acc_y = (fx16)FX_Mul(GMD_RING_FALL_ACC,
						mtMathCos(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_work.dir_fall));
	}

	/*** リング処理 ***/
	/* スロットリング生成チェック */
	if (gm_ring_sys_work->wait_slot_ring_num) {
		gm_ring_sys_work->slot_ring_timer--;
		if (gm_ring_sys_work->slot_ring_timer <= 0) {
			if (gm_ring_sys_work->wait_slot_ring_num >= 2) {
				GmRingCreateSlotRing(gm_ring_sys_work->slot_target_obj,
								GMD_RING_SLOT_DIST,
								gm_ring_sys_work->slot_ring_create_dir);
				GmRingCreateSlotRing(gm_ring_sys_work->slot_target_obj,
								GMD_RING_SLOT_DIST,
								(u16)(gm_ring_sys_work->slot_ring_create_dir + 0x8000-GMD_RING_SLOT_DIR_SPD));
				gm_ring_sys_work->wait_slot_ring_num -= 2;
			}
			else {
				GmRingCreateSlotRing(gm_ring_sys_work->slot_target_obj,
								GMD_RING_SLOT_DIST,
								gm_ring_sys_work->slot_ring_create_dir);
				gm_ring_sys_work->wait_slot_ring_num--;
			}

			gm_ring_sys_work->slot_ring_create_dir -= GMD_RING_SLOT_DIR_SPD;
			gm_ring_sys_work->slot_ring_timer = GMD_RING_SLOT_INT_TIME;
		}
	}

#if 0
	/* 取得エフェクト */
	if (GmMainIsBossStage()) {
		MTS_ACTION3D_SPRITE		*twinkle_act3d = &gm_ring_sys_work->twinkle_act3d;

		// ボスステージ(3D)
		mtActResetStruct(&twinkle_act3d->act, ACT_RING_KIRAKIRA_3D_ID);	// エフェクトアクションリセット
		for (ring_work = gm_ring_sys_work->twinkle_list_end, update_frame = 0; ring_work; ring_work = ring_work->pre_ring) {
			// フレーム更新の都合上、新しく登録されたワークから描画するので最後尾から処理
			// 違う連結リストに繋ぎなおさないので、next(prev)管理しなくてもよい
			// アクション更新
			if (ring_work->timer - update_frame) {
				mtActUpdateTime(&twinkle_act3d->act, (ring_work->timer - update_frame) << FX32_SHIFT, NULL, 0);
				update_frame = ring_work->timer;
			}

			// 描画
			gm_ring_sys_work->twinkle_draw_func(ring_work);

			// 終了チェック
			ring_work->timer++;
			if (ring_work->timer >= GMD_RING_TWINKLE_TIMER) {
				// エフェクト終了
				gmRingDetachTwinkleList(ring_work);		// リングワーク切り離し
				gmRingFreeRingWork(ring_work);			// リングワーク解放
			}
		}
	}
	else {
		// 通常ステージ(2D)
		MTS_ACTION_DS			*twinkle_act = &gm_ring_sys_work->twinkle_act;

		mtActResetStructDS(twinkle_act, ACT_RING_KIRAKIRA_ID);	// エフェクトアクションリセット
		for (ring_work = gm_ring_sys_work->twinkle_list_end, update_frame = 0; ring_work; ring_work = ring_work->pre_ring) {
			// フレーム更新の都合上、新しく登録されたワークから描画するので最後尾から処理
			// 違う連結リストに繋ぎなおさないので、next(prev)管理しなくてもよい
			// アクション更新
			if (ring_work->timer - update_frame) {
				mtActUpdateTimeDS(twinkle_act, (ring_work->timer - update_frame) << FX32_SHIFT, NULL, 0);
				update_frame = ring_work->timer;
			}

			// 描画
			gm_ring_sys_work->twinkle_draw_func(ring_work);

			// 終了チェック
			ring_work->timer++;
			if (ring_work->timer >= GMD_RING_TWINKLE_TIMER) {
				// エフェクト終了
				gmRingDetachTwinkleList(ring_work);		// リングワーク切り離し
				gmRingFreeRingWork(ring_work);			// リングワーク解放
			}
		}
	}
#endif

	/* 通常固定リング */
	for (ring_next = gm_ring_sys_work->ring_list_start; ring_next;) {
		ring_work	= ring_next;
		ring_next	= ring_work->post_ring;

		// 画面外チェック
		if (ObjViewOutCheck(ring_work->pos.x, ring_work->pos.y, GMD_RING_DIE_OFFSET, 0, 0, 0, 0)) {
		// 画面外破棄
			// リングイベント状態を蘇生させる
			if (ring_work->eve_rec) {
				ring_work->eve_rec->pos_x = (u8)((ring_work->pos.x >> FX32_SHIFT) & 0x000000ff);
			}
			// リングワーク開放
			gmRingDetachRingList(ring_work);
			gmRingFreeRingWork(ring_work);
			continue;
		}

		// 描画
		gm_ring_sys_work->ring_draw_func(ring_work);

		// プレイヤーとの当たり判定
		ring_rect.pos.z = 0;
		for (i = 0, is_hit = FALSE; i < gm_ring_sys_work->player_num; i++) {	// 1つのリングを複数プレイヤーが同時取得可能にする
			ply_work = g_gm_main_system.ply_work[i];
			//flag_temp = ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag;			// 体矩形 フラグ退避
			//ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag &= ~OBD_RECT_NOHIT;	// 強制HIT
			if (ply_work->player_flag & GMD_PLF_DIE) {
				continue;
			}

			// リング矩形座標設定
			ring_rect.pos.x = ring_work->pos.x;
			ring_rect.pos.y = ring_work->pos.y;
			//ring_rect.pos.x = ring_work->pos.x >> FX32_SHIFT;
			//ring_rect.pos.y = ring_work->pos.y >> FX32_SHIFT;
			//ring_rect.pos.z = ring_work->pos.z >> FX32_SHIFT;
			if (gm_ring_sys_work->rec_func(&ply_rect[i], &ring_rect)) {
				// プレイヤーにHIT
				// 取得エフェクトへの変更
				is_hit = TRUE;

				// プレイヤーにリング加算
				GmPlayerRingGet(ply_work, 1);

				// エフェクト リング
				GmComEfctCreateRing(ring_rect.pos.x, ring_rect.pos.y);
				//if (GmMainIsBossStage()) {
				//// ボスステージのみ
				//	// 通常固定リングを取得するとリングダメージ値が減少
				//	if (gm_ring_sys_work->damage_num[i]) {
				//		gm_ring_sys_work->damage_num[i]--;
				//	}
				//}
			}

			//ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag = flag_temp;			// 体矩形 フラグ復帰
		}
		if (is_hit) {
			// 取得エフェクトに変更
			ring_work->timer = 0;						// エフェクト表示タイマー初期化
			gmRingDetachRingList(ring_work);			// リングワーク切り離し
			//gmRingAttachTwinkleList(ring_work);			// 取得エフェクトリストに追加
			// エフェクト追加時に対応
			gmRingFreeRingWork(ring_work);				// リングワーク解放
		}
	}

	/* 磁力吸い寄せリング */
#if 0
	for (ring_next = gm_ring_sys_work->magnet_ring_list_start; ring_next;) {
		ring_work	= ring_next;
		ring_next	= ring_work->post_ring;

		// 画面外チェック
		if (ObjViewOutCheck(ring_work->pos.x, ring_work->pos.y, GMD_RING_DIE_OFFSET, 0, 0, 0, 0)) {
		// 画面外破棄
			// リングイベント状態を蘇生させる
			if (ring_work->eve_rec) {
				ring_work->eve_rec->pos_x = (u8)((ring_work->pos.x >> FX32_SHIFT) & 0x000000ff);
			}
			// リングワーク開放
			gmRingDetachMagnetRingList(ring_work);
			gmRingFreeRingWork(ring_work);
			continue;
		}

		// 移動
		{
			u16			dir;
			OBS_RECT	*target_rect = &ply_rect[ring_work->flag & GMD_RING_MAGNET_PLAYER_MASK];

			dir = mtMathAtan2(target_rect->pos.y - (ring_work->pos.y >> FX32_SHIFT),
							target_rect->pos.x - (ring_work->pos.x >> FX32_SHIFT));
			// 加速
			// spd_x をマスタースピード代わりに
			ring_work->spd_x = ObjSpdUpSet(ring_work->spd_x, GMD_RING_MAGNET_ACC, GMD_RING_MAGNET_MSPD);
			ring_work->pos.x += (mtMathCos(dir) * (ring_work->spd_x >> 8)) >> 4;	// 簡易計算
			ring_work->pos.y += (mtMathSin(dir) * (ring_work->spd_x >> 8)) >> 4;

#if 1
			if (ring_work->pos.z != (target_rect->pos.z << FX32_SHIFT)) {
				// Z座標を近づける
				fx32	ring_pos_z;
				ring_pos_z = ObjShiftSet(ring_work->pos.z, (target_rect->pos.z << FX32_SHIFT),
															1, 0x8000, 0x1000);

				ring_work->scale.x += (ring_pos_z - ring_work->pos.z) >> (FX32_SHIFT - 4);
				if (ring_work->scale.x > FX32_ONE*2) {
					ring_work->scale.x = FX32_ONE*2;
				}

				//ring_work->scale.x = FX32_ONE + ObjSpdDownSet(ring_work->scale.x - FX32_ONE,
				//							MTM_MATH_ABS(ring_pos_z - ring_work->pos.z) >> (FX32_SHIFT - 4));

				ring_work->scale.y = ring_work->scale.z = ring_work->scale.x;
				ring_work->pos.z = ring_pos_z;
			}

#else
			if (ring_work->pos.z) {
				// Z座標を0に戻す
				ring_work->pos.z = ObjSpdDownSet(ring_work->pos.z, 0x4000);
				ring_work->scale.x = 0x1000 + ObjSpdDownSet(ring_work->scale.x - 0x1000, 0x4000 >> (FX32_SHIFT - 4));
				ring_work->scale.y = ring_work->scale.z = ring_work->scale.x;
			}
#endif
		}

		// 描画
		gm_ring_sys_work->ring_draw_func(ring_work);

		// プレイヤーとの当たり判定
		ring_rect.pos.z = 0;
		for (i = 0, is_hit = FALSE; i < gm_ring_sys_work->player_num; i++) {	// 1つのリングを複数プレイヤーが同時取得可能 ◆OK？
			ply_work = g_gm_main_ply_obj_list[i];
			//flag_temp = ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag;			// 体矩形 フラグ退避
			//ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag &= ~OBD_RECT_NOHIT;	// 強制HIT

			// リング矩形座標設定
			ring_rect.pos.x = ring_work->pos.x;
			ring_rect.pos.y = ring_work->pos.y;
			//ring_rect.pos.x = ring_work->pos.x >> FX32_SHIFT;
			//ring_rect.pos.y = ring_work->pos.y >> FX32_SHIFT;
			//ring_rect.pos.z = ring_work->pos.z >> FX32_SHIFT;
			if (gm_ring_sys_work->rec_func(&ply_rect[i], &ring_rect)) {
				// プレイヤーにHIT
				// 取得エフェクトへの変更
				is_hit = TRUE;

				// プレイヤーにリング加算
				GmPlayerRingGet(ply_work, 1);
			}

			//ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag = flag_temp;			// 体矩形 フラグ復帰
		}
		if (is_hit) {
			// 取得エフェクトに変更
			ring_work->timer = 0;						// エフェクト表示タイマー初期化
			gmRingDetachMagnetRingList(ring_work);		// 磁力吸い寄せリングワーク切り離し
			//gmRingAttachTwinkleList(ring_work);			// 取得エフェクトリストに追加
			gmRingFreeRingWork(ring_work);				// リングワーク解放
		}
	}
#endif

	/* ダメージ飛び散りリング */
	for (ring_next = gm_ring_sys_work->damage_ring_list_start; ring_next;) {
		ring_work	= ring_next;
		ring_next	= ring_work->post_ring;

		// 画面外チェック
		if (ObjViewOutCheck(ring_work->pos.x, ring_work->pos.y, GMD_RING_MOVE_DIE_OFFSET, 0, 0, 0, 0)) {
		// 画面外破棄
			// リングワーク開放
			gmRingDetachDamageRingList(ring_work);
			gmRingFreeRingWork(ring_work);
			continue;
		}

		// 移動
		ring_work->pos.x += ring_work->spd_x;
		if (ring_work->flag & GMD_RING_REVERSE) {
			// 逆重力
			ring_work->pos.y -= ring_work->spd_y;
		}
		else {
			ring_work->pos.y += ring_work->spd_y;
		}
		// 落下加速
		ring_work->spd_x += gm_ring_fall_acc_x;
		ring_work->spd_y += gm_ring_fall_acc_y;

		// 地形判定
		gm_ring_sys_work->col_func(ring_work);

		ring_work->timer--;
		if (ring_work->timer == 0) {
			// ダメージリング寿命終了
			gmRingDetachDamageRingList(ring_work);	// リングワーク切り離し
			gmRingFreeRingWork(ring_work);
			continue;
		}
		else if (ring_work->timer <= GMD_RING_NOHIT_TIMER) {	// HIT可能時間内であるか
			// プレイヤーとの当たり判定
			ring_rect.pos.z = 0;
			for (i = 0, is_hit = FALSE; i < gm_ring_sys_work->player_num; i++) {	// 1つのリングを複数プレイヤーが同時取得可能 ◆OK？
				ply_work = g_gm_main_system.ply_work[i];
				//flag_temp = ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag;			// 体矩形 フラグ退避
				//ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag &= ~OBD_RECT_NOHIT;	// 強制HIT

				if (ply_work->player_flag & GMD_PLF_DIE) {
					continue;
				}
#if 1
				// リング矩形座標設定
				ring_rect.pos.x = ring_work->pos.x;
				ring_rect.pos.y = ring_work->pos.y;
				//ring_rect.pos.x = ring_work->pos.x >> FX32_SHIFT;
				//ring_rect.pos.y = ring_work->pos.y >> FX32_SHIFT;
				//ring_rect.pos.z = 0;
				if (gm_ring_sys_work->rec_func(&ply_rect[i], &ring_rect)) {
#else
				if (gm_ring_sys_work->rec_func(&ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY], ring_work->pos.x, ring_work->pos.y,
						GMD_RING_HIT_LEFT, GMD_RING_HIT_TOP, GMD_RING_HIT_WIDTH , GMD_RING_HIT_HEIGHT)) {
#endif
					// プレイヤーにHIT
					// 取得エフェクトへの変更
					is_hit = TRUE;

					// プレイヤーにリング加算
					{
						s16	ring_stage_num = ply_work->ring_stage_num;
						GmPlayerRingGet(ply_work, 1);
						if (ring_stage_num < 999) {
							ply_work->ring_stage_num--;
						}
					}

					// エフェクト リング
					GmComEfctCreateRing(ring_rect.pos.x, ring_rect.pos.y);

					if (gm_ring_sys_work->flag & (GMD_RING_SYS_FLAG_DAMAGE_CHK_P1 << i)) {
						// ダメージリング取得により、ダメージ回数クリアは無し
						gm_ring_sys_work->flag &= ~(GMD_RING_SYS_FLAG_DAMAGE_CHK_P1 << i);
					}
				}

				//ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag = flag_temp;			// 体矩形 フラグ復帰
			}

			// 取得エフェクトチェック
			if (is_hit) {
				// 取得エフェクトに変更
				ring_work->timer = 0;					// エフェクト表示タイマー初期化
				gmRingDetachDamageRingList(ring_work);	// リングワーク切り離し
				//gmRingAttachTwinkleList(ring_work);		// 取得エフェクトリストに追加
				gmRingFreeRingWork(ring_work);			// リングワーク解放
			}
		}

		// 描画
		if (ring_work->timer > GMD_RING_BLINK_TIMER || ring_work->timer & 0x02) {
			gm_ring_sys_work->ring_draw_func(ring_work);
		}
	}
	// ダメージ回数クリアチェック
//	if (GsGetMainSysInfo()->game_mode == GSD_GAME_MODE_CONTEST &&
//			GsGetMainSysInfo()->contest_type == GSD_CONTEST_TYPE_RING) {
//		// リング対戦時は常に0クリア
//		for (i = 0; i < gm_ring_sys_work->player_num; i++) {
//			gm_ring_sys_work->damage_num[i] = 0;
//		}
//	}
//	else {
		if (!gm_ring_sys_work->damage_ring_list_start) {
			for (i = 0; i < gm_ring_sys_work->player_num; i++) {
				if (gm_ring_sys_work->flag & (GMD_RING_SYS_FLAG_DAMAGE_CHK_P1 << i)) {
					// ダメージリングを取らなかったのでダメージ回数クリア
					gm_ring_sys_work->damage_num[i] = 0;
					gm_ring_sys_work->flag &= ~(GMD_RING_SYS_FLAG_DAMAGE_CHK_P1 << i);
				}
			}
		}
//	}

	/* マグネット状態リング◆ */



		
	/* スロットギフトリング */
	for (ring_next = gm_ring_sys_work->slot_ring_list_start; ring_next;) {
		ring_work	= ring_next;
		ring_next	= ring_work->post_ring;

		// クリッピングなし

		// 画面外チェック
		// おそらく発生しないが、極端に遠くへ移動する事があった場合は終了する
		if (ObjViewOutCheck(ring_work->pos.x, ring_work->pos.y, GMD_RING_SLOT_DIE_OFFSET, 0, 0, 0, 0)) {
		// 画面外破棄
			// リングワーク開放
			gmRingDetachSlotRingList(ring_work);
			gmRingFreeRingWork(ring_work);
			continue;
		}

		// 移動
		ring_work->pos.x += ring_work->spd_x;
		ring_work->pos.y += ring_work->spd_y;

		// 地形判定なし
		
		// 描画
		gm_ring_sys_work->ring_draw_func(ring_work);

		// プレイヤーとの当たり判定
		ring_rect.pos.z = 0;
		for (i = 0, is_hit = FALSE; i < gm_ring_sys_work->player_num; i++) {	// 1つのリングを複数プレイヤーが同時取得可能 ◆OK？
			ply_work = g_gm_main_system.ply_work[i];
			//flag_temp = ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag;			// 体矩形 フラグ退避
			//ply_work->rect_work[GMD_PLAYER_RECT_ID_BODY].flag &= ~OBD_RECT_NOHIT;	// 強制HIT

			if (ply_work->player_flag & GMD_PLF_DIE) {
				continue;
			}

			// リング矩形座標設定
			ring_rect.pos.x = ring_work->pos.x;
			ring_rect.pos.y = ring_work->pos.y;
			//ring_rect.pos.x = ring_work->pos.x >> FX32_SHIFT;
			//ring_rect.pos.y = ring_work->pos.y >> FX32_SHIFT;
			//ring_rect.pos.z = 0;
			if (gm_ring_sys_work->rec_func(&ply_rect[i], &ring_rect)) {
				// プレイヤーにHIT
				// 取得エフェクトへの変更
				is_hit = TRUE;

				// プレイヤーにリング加算
				GmPlayerRingGet(ply_work, 1);
			//	{		// スロットリングもステージリングに換算してよいのか？
			//		s16	ring_stage_num = ply_work->ring_stage_num;
			//		GmPlayerRingGet(ply_work, 1);
			//		if (ring_stage_num < 999) {
			//			ply_work->ring_stage_num--;
			//		}
			//	}

				// エフェクト リング
				GmComEfctCreateRing(ring_rect.pos.x, ring_rect.pos.y);
			}
		}

		// 取得エフェクトチェック
		if (is_hit) {
			// 取得エフェクトに変更
			ring_work->timer = 0;					// エフェクト表示タイマー初期化
			gmRingDetachSlotRingList(ring_work);	// リングワーク切り離し
			//gmRingAttachTwinkleList(ring_work);		// 取得エフェクトリストに追加
			gmRingFreeRingWork(ring_work);			// リングワーク解放
		}
	}
#if _IPHONE
	gmRingDrawEnd();
#endif //_IPHONE

	// SE再生カウンタクリア
	gm_ring_sys_work->ring_se_cnt = 0;
}

// ==========================================================================
// 地形判定
// ==========================================================================
// ==========================================================================
// gmRingMoveCollsion
/*!
 *	リング地形判定
 *
 *	@param	ring_work	[in]	リングワーク
 */
// ==========================================================================
void gmRingMoveCollsion(GMS_RING_WORK *ring_work)
{
	s32					diff = 0;
	//u8					suf = 0;
	fx32				spd_y = ring_work->spd_y;
	OBS_COL_CHK_DATA	col_data;

	// 当たりチェックなし
//	if ( ring_work->flag & GMD_RING_NOCOL ){
  ///	  return;
 //   }

	// リング速度反転チェック
	if (ring_work->flag & GMD_RING_REVERSE) {
		spd_y = -spd_y;
	}

	/* 地形チェック構造体設定 */
	// 判定面設定
	if (ring_work->flag & GMD_RING_FLAG_B) {
		col_data.flag = OBD_OBJECT_B;
	}
	else {
		col_data.flag = 0;
	}
	col_data.dir	= NULL;
	col_data.attr	= NULL;

	/* 地形縦判定 */
	// 座標設定
	col_data.pos_x	= ring_work->pos.x >> FX32_SHIFT;
	col_data.pos_y	= ring_work->pos.y >> FX32_SHIFT;
	if ( ring_work->spd_y > 0 ){
#if 1
		col_data.pos_y	+= GMD_RING_HIT_BOTTOM;
		col_data.vec	= OBD_COL_DOWN;
		diff = ObjDiffCollisionFast(&col_data);
#else
		diff = objFastCollision( ring_work->lPosX >> 8, ring_work->lPosY >> 8,  suf, OBD_COL_DOWN, NULL, NULL);
#endif
		// 地面埋まりチェック
		if ( diff < 0 ) {
			if (ring_work->flag & GMD_RING_REVERSE) {
				ring_work->pos.y -= diff << FX32_SHIFT;
			}
			else {
				ring_work->pos.y += diff << FX32_SHIFT;
			}
		}
	}
	else if (ring_work->spd_y < 0) {
#if 1
		col_data.pos_y	+= GMD_RING_HIT_TOP;
		col_data.vec	= OBD_COL_UP;
		diff = ObjDiffCollisionFast(&col_data);
#else
		diff = objFastCollision( ring_work->lPosX >> 8, ring_work->lPosY >> 8,  suf, OBD_COL_UP, NULL, NULL);
#endif
		// 地面埋まりチェック
		if (diff < 0) {
			if (ring_work->flag & GMD_RING_REVERSE) {
				ring_work->pos.y += diff << FX32_SHIFT;
			}
			else {
				ring_work->pos.y -= diff << FX32_SHIFT;
			}
		}
	}

	if ( diff < 0 ) {	// 地面HIT時
		// 速度反射
		ring_work->spd_y -= ring_work->spd_y >> 2;
		ring_work->spd_y = -ring_work->spd_y;
		// ◆現在地形のみの接触チェックでは跳ね返り最大速度チェックが必要なものがないので
		// 処理を省いている
		// 必要な場合は gmRingMoveCollsionObjectPlus を参照の事
	}

	/* 地形横判定 */
	diff = 0;
	// 座標設定(Yが変更されている可能性があるので再設定)
	//col_data.pos_x	= ring_work->pos.x >> FX32_SHIFT;
	col_data.pos_y	= ring_work->pos.y >> FX32_SHIFT;
	if (ring_work->spd_x > 0) {
#if 1
		col_data.pos_x	+= GMD_RING_HIT_RIGHT;
		col_data.vec	= OBD_COL_RIGHT;
		diff = ObjDiffCollisionFast(&col_data);
#else
		diff = objFastCollision( ring_work->lPosX >> 8, ring_work->lPosY >> 8,  suf, OBD_COL_RIGHT, NULL, NULL);
#endif
		// 地面埋まりチェック
		if (diff < 0) {
			ring_work->pos.x += diff << FX32_SHIFT;
		}
	}
	else if (ring_work->spd_x < 0) {
#if 1
		col_data.pos_x	+= GMD_RING_HIT_LEFT;
		col_data.vec	= OBD_COL_LEFT;
		diff = ObjDiffCollisionFast(&col_data);
#else
		diff = objFastCollision( ring_work->lPosX >> 8, ring_work->lPosY >> 8,  suf, OBD_COL_LEFT, NULL, NULL);
#endif
		// 地面埋まりチェック
		if (diff < 0) {
			ring_work->pos.x -= diff << FX32_SHIFT;
		}
	}

	if (diff < 0) {	// 地面HIT時
		// 速度反射
		ring_work->spd_x -= ring_work->spd_x >> 2;
		ring_work->spd_x = -ring_work->spd_x;
	}
}

#if 0
// ==========================================================================
// gmRingMoveCollsionObjectPlus
/*!
 *	リング地形判定、オブジェクト地形チェック付き
 *
 *	@param	ring_work	[in]	リングワーク
 */
// ==========================================================================
void gmRingMoveCollsionObjectPlus(GMS_RING_WORK *ring_work)
{
	s32					diff = 0;
	u8					suf = 0;
	fx32				spd_y = ring_work->spd_y;
	OBS_COL_CHK_DATA	col_data;

	// リング速度反転チェック
	if (ring_work->flag & GMD_RING_REVERSE) {
		spd_y = -spd_y;
	}

	/* 地形チェック構造体設定 */
	// 判定面設定
	if (ring_work->flag & GMD_RING_FLAG_B) {
		col_data.flag = OBD_OBJECT_B;
	}
	else {
		col_data.flag = 0;
	}
	col_data.dir	= NULL;
	col_data.attr	= NULL;

	/* 地形縦判定 */
	// 座標設定
	col_data.pos_x	= ring_work->pos.x >> FX32_SHIFT;
	col_data.pos_y	= ring_work->pos.y >> FX32_SHIFT;
	if ( ring_work->spd_y > 0 ){
		col_data.vec	= OBD_COL_DOWN;
		diff = ObjCollisionFastUnion(&col_data);

		// 地面埋まりチェック
		if ( diff < 0 ) {
			if (ring_work->flag & GMD_RING_REVERSE) {
				ring_work->pos.y -= diff << FX32_SHIFT;
			}
			else {
				ring_work->pos.y += diff << FX32_SHIFT;
			}
		}
	}
	else if (ring_work->spd_y < 0) {
		col_data.vec	= OBD_COL_UP;
		diff = ObjCollisionFastUnion(&col_data);

		// 地面埋まりチェック
		if (diff < 0) {
			if (ring_work->flag & GMD_RING_REVERSE) {
				ring_work->pos.y += diff << FX32_SHIFT;
			}
			else {
				ring_work->pos.y -= diff << FX32_SHIFT;
			}
		}
	}

	if ( diff < 0 ) {	// 地面HIT時
		// 速度反射
		ring_work->spd_y -= ring_work->spd_y >> 2;
		if (gm_ring_sys_work->ref_spd_base && ring_work->flag & GMD_RING_DAMAGE) {
			fx32	max_spd;

			// ダメージリング時跳ね返り最大速度チェック
			max_spd = gm_ring_sys_work->ref_spd_base *
						gm_ring_sys_work->damage_num[(ring_work->flag & GMD_RING_DAMAGE_PLAYER) >> GMD_RING_DAMAGE_PLAYER];
			if (ring_work->spd_y > max_spd) {
				ring_work->spd_y = max_spd;
			}
		}
		ring_work->spd_y = -ring_work->spd_y;
	}

	/* 地形横判定 */
	diff = 0;

	// 座標設定(Yが変更されている可能性があるので再設定)
	//col_data.pos_x	= ring_work->pos.x >> FX32_SHIFT;
	col_data.pos_y	= ring_work->pos.y >> FX32_SHIFT;

	if (ring_work->spd_x > 0) {
		col_data.vec	= OBD_COL_RIGHT;
		diff = ObjCollisionFastUnion(&col_data);

		// 地面埋まりチェック
		if (diff < 0) {
			ring_work->pos.x += diff << FX32_SHIFT;
		}
	}
	else if (ring_work->spd_x < 0) {
		col_data.vec	= OBD_COL_LEFT;
		diff = ObjCollisionFastUnion(&col_data);

		// 地面埋まりチェック
		if (diff < 0) {
			ring_work->pos.x -= diff << FX32_SHIFT;
		}
	}

	if (diff < 0) {	// 地面HIT時
		// 速度反射
		ring_work->spd_x -= ring_work->spd_x >> 2;
		ring_work->spd_x = -ring_work->spd_x;
	}
}
#endif

#if 0
// ==========================================================================
// gmRingMoveCollsionObject
/*!
 *	リング地形判定、オブジェクト地形チェックのみ
 *
 *	@param	ring_work	[in]	リングワーク
 */
// ==========================================================================
void gmRingMoveCollsionObject(GMS_RING_WORK *ring_work)
{
#if 0	// ◆あとで
	s32 lDiff = 0;
	u8 ucSuf = 0;
	s16 sSpdY = ring_work->sSpdY;
	// 当たりチェックなし
	if ( ring_work->flag & GMD_RING_NOCOL ){
		return;
	}
	
	// 判定面設定
	if ( ring_work->flag & GMD_RING_FLAG_B )ucSuf = 1;
	else							 ucSuf = 0;

	if ( ring_work->flag & GMD_RING_REVERSE )
		sSpdY = (s16)-sSpdY;
	
	// 地形縦判定
	if ( ring_work->sSpdY > 0 ){
		lDiff = objCollisionObjectFastCheck( ring_work->lPosX >> 8, ring_work->lPosY >> 8,  ucSuf, OBD_COL_DOWN, NULL, NULL);
		// 地面埋まり
		if ( lDiff < 0 ){
			if ( ring_work->flag & GMD_RING_REVERSE )
				ring_work->lPosY -= lDiff << 8;
			else
				ring_work->lPosY += lDiff << 8;
		}
		
	}else if ( ring_work->sSpdY < 0 ){
		lDiff = objCollisionObjectFastCheck( ring_work->lPosX >> 8, ring_work->lPosY >> 8,  ucSuf, OBD_COL_UP, NULL, NULL);
		// 地面埋まり
		if ( lDiff < 0 ){
			if ( ring_work->flag & GMD_RING_REVERSE )
				ring_work->lPosY += lDiff << 8;
			else
				ring_work->lPosY -= lDiff << 8;
		}
				
	}
	if ( lDiff < 0 ) {
		// 速度反射
		ring_work->sSpdY -= ring_work->sSpdY >> 2;
		ring_work->sSpdY = (s16)(-ring_work->sSpdY);
	}
	// 初期化
	lDiff = 0;

	// 地形横判定
	if ( ring_work->sSpdX > 0 ){
		lDiff = objCollisionObjectFastCheck( ring_work->lPosX >> 8, ring_work->lPosY >> 8,  ucSuf, OBD_COL_RIGHT, NULL, NULL);
		// 地面埋まり
		if ( lDiff < 0 )
			ring_work->lPosX += lDiff << 8;
		
	}else if ( ring_work->sSpdX < 0 ){
		lDiff = objCollisionObjectFastCheck( ring_work->lPosX >> 8, ring_work->lPosY >> 8,  ucSuf, OBD_COL_LEFT, NULL, NULL);
		// 地面埋まり
		if ( lDiff < 0 )
			ring_work->lPosX -= lDiff << 8;

	}
	if ( lDiff < 0 ) {
		// 速度反射
		ring_work->sSpdX -= ring_work->sSpdX >> 2;
		ring_work->sSpdX = (s16)(-ring_work->sSpdX);
	}
#endif
}
#endif


// ==========================================================================
// 描画関数
// ==========================================================================
// 通常リング
#if 0
// ==========================================================================
// gmRingDrawFuncRing2D
/*!
 *	2Dリング描画関数
 */
// ==========================================================================
void gmRingDrawFuncRing2D(GMS_RING_WORK *ring_work)
{
	u32	disp_flag = MTD_ACT_FLAG_REPEAT | OBD_DISP_NOUPDATE;

	if (ring_work->flag & GMD_RING_REVERSE) {
		// 逆重力対応
		disp_flag |= OBD_DISP_VFLIP;
	}

	// 倍角設定
	if (ring_work->scale.x <= FX32_ONE) {
		gm_ring_sys_work->ring_act.act.flag &= ~MTD_ACT_FLAG_DOUBLE;
	}
	else {
		gm_ring_sys_work->ring_act.act.flag |= MTD_ACT_FLAG_DOUBLE;
	}

	ObjAction(&gm_ring_sys_work->ring_act, &ring_work->pos, NULL, &ring_work->scale, &disp_flag, NULL, NULL);
}
#endif

// ==========================================================================
// gmRingDrawFuncRing3D
/*!
 *	3Dリング描画関数
 */
// ==========================================================================
void gmRingDrawFuncRing3D(GMS_RING_WORK *ring_work)
{
	u32						disp_flag = 0;
	VecU16					dir;

	dir.x = 0;
	dir.y = gm_ring_sys_work->dir;
#if 0	// Ｚ軸回転固定(既存処理)
	dir.z = 0;
#else	// カメラ回転に合わせてリングＺ軸回転
	{
		OBS_CAMERA	*camera = ObjCameraGet(g_obj.glb_camera_id);
		dir.z = (u16)-camera->roll;
	}
#endif
#if !_IPHONE
#if !defined GMD_DEBUG_NO_CREATE_RING
	ObjDrawAction3DNN(gm_ring_obj_3d, &ring_work->pos, &dir, &ring_work->scale, &disp_flag);
#endif // GMD_DEBUG_NO_CREATE_RING
#else //!_IPHONE
	// 勝手に回られるとポーズがかけられない＆回転が安定しない
	disp_flag = OBD_DISP_NOUPDATE;

	int count = gm_ring_sys_work->draw_ring_count;
	
	gm_ring_sys_work->draw_ring_pos[count].x = ring_work->pos.x;
	gm_ring_sys_work->draw_ring_pos[count].y = ring_work->pos.y;
	gm_ring_sys_work->draw_ring_pos[count].z = ring_work->pos.z;

	++gm_ring_sys_work->draw_ring_count;
#endif //!_IPHONE
}

#if 0
// ==========================================================================
// gmRingDrawFuncRingCircle3D
/*!
 *	3Dリング円形ステージ描画関数
 */
// ==========================================================================
void gmRingDrawFuncRingCircle3D(GMS_RING_WORK *ring_work)
{
	MTS_ACTION3D_SPRITE		*act_3dspr = &gm_ring_sys_work->ring_act3d;

//	act_3dspr->a3d.trans.x =  ring_work->pos.x;
	act_3dspr->a3d.trans.y = -ring_work->pos.y;
//	act_3dspr->a3d.trans.z =  ring_work->pos.z;

	GmBossComputeCirclePosFromLine(ring_work->pos.x,
					g_gm_map.circle_start_pos, g_gm_map.circle_end_pos, g_gm_map.circle_radius,
					&act_3dspr->a3d.trans.x, &act_3dspr->a3d.trans.z);

	mtAct3dDrawSprite(act_3dspr);
}
#endif

// リング取得エフェクト
#if 0
// ==========================================================================
// gmRingDrawFuncTwinkle2D
/*!
 *	2Dリング取得エフェクト描画関数
 */
// ==========================================================================
void gmRingDrawFuncTwinkle2D(GMS_RING_WORK *ring_work)
{
	u32	disp_flag = MTD_ACT_FLAG_REPEAT | OBD_DISP_NOUPDATE;

	if (ring_work->flag & GMD_RING_REVERSE) {
		// 逆重力対応
		disp_flag |= OBD_DISP_VFLIP;
	}

	// 倍角設定
	if (ring_work->scale.x <= FX32_ONE) {
		gm_ring_sys_work->twinkle_act.act.flag &= ~MTD_ACT_FLAG_DOUBLE;
	}
	else {
		gm_ring_sys_work->twinkle_act.act.flag |= MTD_ACT_FLAG_DOUBLE;
	}

	ObjAction(&gm_ring_sys_work->twinkle_act, &ring_work->pos, NULL, &ring_work->scale, &disp_flag, NULL, NULL);
}
#endif

#if 0
// ==========================================================================
// gmRingDrawFuncTwinkle3D
/*!
 *	3Dリング取得エフェクト描画関数
 */
// ==========================================================================
void gmRingDrawFuncTwinkle3D(GMS_RING_WORK *ring_work)
{
#if 0
	MTS_ACTION3D_SPRITE		*act_3dspr = &gm_ring_sys_work->twinkle_act3d;
#if 1
	act_3dspr->a3d.trans.x =  ring_work->pos.x;
	act_3dspr->a3d.trans.y = -ring_work->pos.y;
	act_3dspr->a3d.trans.z =  ring_work->pos.z;

	mtAct3dDrawSprite(act_3dspr);
#else
	u32	disp_flag = MTD_ACT_FLAG_REPEAT | OBD_DISP_NOUPDATE | OBD_DISP_NODIR;

	ObjAction3D(&act_3dspr->a3d, &ring_work->pos, NULL, &ring_work->scale, &disp_flag, NULL, NULL, 0);
#endif
#endif
}
#endif

#if 0
// ==========================================================================
// gmRingDrawFuncTwinkleCircle3D
/*!
 *	3Dリング取得エフェクト円形ステージ描画関数
 */
// ==========================================================================
void gmRingDrawFuncTwinkleCircle3D(GMS_RING_WORK *ring_work)
{
	MTS_ACTION3D_SPRITE		*act_3dspr = &gm_ring_sys_work->twinkle_act3d;

//	act_3dspr->a3d.trans.x =  ring_work->pos.x;
	act_3dspr->a3d.trans.y = -ring_work->pos.y;
//	act_3dspr->a3d.trans.z =  ring_work->pos.z;

	GmBossComputeCirclePosFromLine(ring_work->pos.x,
					g_gm_map.circle_start_pos, g_gm_map.circle_end_pos, g_gm_map.circle_radius,
					&act_3dspr->a3d.trans.x, &act_3dspr->a3d.trans.z);

	mtAct3dDrawSprite(act_3dspr);
}
#endif

// ==========================================================================
// 当たり判定関数
// ==========================================================================
// ==========================================================================
// gmRingHitFuncNormal
/*!
 *	通常ステージのリングとプレイヤーの当たり処理
 *
 *	@param	ply_rect	[in]	プレイヤー矩形情報
 *	@param	ring_rect	[in]	リング矩形情報
 *
 *	@return	0 : HIT無し		それ以外 : HIT有り
 */
// ==========================================================================
u16	gmRingHitFuncNormal(OBS_RECT *ply_rect, OBS_RECT *ring_rect)
//u16 gmRingHitFuncNormal(OBS_RECT_WORK *obj_work, s32 pos_x, s32 pos_y, s16 left, s16 top, u16 width, u16 height)
{
	return ObjRectCheck(ply_rect, ring_rect);
  //  return objRectCheckDetail(obj_work, pos_x, pos_y, left, top, width, height);
}

// ==========================================================================
// gmRingHitFuncCircle
/*!
 *	円形ステージのリングとプレイヤーの当たり処理
 *
 *	@param	ply_rect	[in]	プレイヤー矩形情報
 *	@param	ring_rect	[in]	リング矩形情報
 *
 *	@return	0 : HIT無し		それ以外 : HIT有り
 */
// ==========================================================================
#if 0
u16	gmRingHitFuncCircle(OBS_RECT *ply_rect, OBS_RECT *ring_rect)
{
	u16	result;

	result = ObjRectCheck(ply_rect, ring_rect);
	if (!result) {
		fx32	pos_x = ring_rect->pos.x;
		ring_rect->pos.x = GmBossComputeLoopPos(ring_rect->pos.x << FX32_SHIFT,
					g_gm_map.circle_start_pos, g_gm_map.circle_end_pos) >> FX32_SHIFT;
		result = ObjRectCheck(ply_rect, ring_rect);
		ring_rect->pos.x = pos_x;
	}

	return (result);
}
#endif

#if _IPHONE
// ==========================================================================
// gmRingDrawBegin
/*!
 *	リング描画開始(描画パラメータを初期化するのみ)
 *
 */
// ==========================================================================
static void gmRingDrawBegin(void)
{
	// 初期化
	gm_ring_sys_work->draw_ring_count = 0;
}
#endif //_IPHONE

#if _IPHONE
// ==========================================================================
// gmRingDrawEnd
/*!
 *	リング描画終了(登録されたリング情報を元に描画。Model, Primitive対応)
 *
 */
// ==========================================================================
static void gmRingDrawEnd(void)
{
	//	0個なら終了
	if (gm_ring_sys_work->draw_ring_count <= 0) {
		return;
	}
	
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	VecU16					dir;
	dir.x = 0;
	dir.y = gm_ring_sys_work->dir;
	dir.z = GmMainGetObjectRotation();
	
#if defined GMD_RING_DRAW_MODEL
	VecFx32					scale;
	u32						disp_flag = OBD_DISP_STOP;
	
	scale.x = FX32_ONE;
	scale.y = FX32_ONE;
	scale.z = FX32_ONE;
	
	//	model draw
	for (int i = 0; i < gm_ring_sys_work->draw_ring_count; ++i) {
		ObjDrawAction3DNN(gm_ring_obj_3d, &gm_ring_sys_work->draw_ring_pos[i], &dir, &scale, &disp_flag);
	}
#endif // GMD_RING_DRAW_MODEL

#if defined GMD_RING_DRAW_PRIMITIVE
	//	primitive draw
	GMS_RING_SYS_WORK* ring_sys = gm_ring_sys_work;
	u32 draw_num = gm_ring_sys_work->draw_ring_count;
	NNS_VECTOR camera_pos;
	ObjCameraDispPosGet(0, &camera_pos);
	
	// カメラによるリング角度設定
	Float sin45, cos45, sin135, cos135;
	nnSinCos(dir.z + (-0x2000), &sin45, &cos45);
	sin45 *= 4.24262f * 3.2f;
	cos45 *= 4.24262f * 3.2f;
	nnSinCos(dir.z + (-0x6000), &sin135, &cos135);
	sin135 *= 4.24262f * 3.2f;
	cos135 *= 4.24262f * 3.2f;
	
	//	プリミティブとして登録
	AMS_PARAM_DRAW_PRIMITIVE dat;
	amZeroMemory(&dat, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	
	// ベースマトリックス
	VecFx32 vec = ring_sys->draw_ring_pos[0];
	
	NNS_MATRIX mtx;
	nnMakeUnitMatrix(&mtx);
	nnTranslateMatrix(&mtx, &mtx, FX_FX32_TO_F32(vec.x), -FX_FX32_TO_F32(vec.y), FX_FX32_TO_F32(vec.z));
	
	// プリミティブ設定
#ifdef GMD_RING_DRAW_STRIP_TEST
	dat.type = NNE_PRIM_TRIANGLE_STRIP;
	dat.count = (s32)(6 * draw_num - 2);
#else
	dat.type = NNE_PRIM_TRIANGLE_LIST;
	dat.count = (s32)(6 * draw_num);
#endif
	
	dat.ablend = NNE_PRIM_ALPHABLEND_OFF;
	// アルファブレンド設定
#if defined(_PC) | defined(_XBOX)
	
	dat.bldSrc = NNE_BLENDMODE_SRCCOL;
	dat.bldDst = NNE_BLENDMODE_DSTCOL;
	dat.bldMode = NNE_BLENDOP_ADD;
#else
	dat.bldSrc = NND_BLENDFUNC_GL_SRC_COLOR;
	dat.bldDst = NND_BLENDFUNC_GL_DST_COLOR;
	dat.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
	// テスト設定
	dat.aTest = 1;
	dat.zMask = 0;
	dat.zTest = 1;
	
	// ソートしない
	dat.noSort = 1;
	
	// テクスチャ設定
	dat.texlist = gm_ring_obj_3d->texlist;
	dat.texId = 0;
	
	// テクスチャクランプ設定
	dat.uwrap = NNE_PRIM_TEXWRAP_CLAMP;
	dat.vwrap = NNE_PRIM_TEXWRAP_CLAMP;
	
	// 色設定
	u32 color = gm_ring_sys_work->color;
	
	// 頂点データ作成
	NNS_PRIM3D_PCT* v_tbl = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer((s32)(sizeof(NNS_PRIM3D_PCT) * (dat.count)));
	dat.vtxPCT3D = v_tbl;
	dat.format3D = NNE_PRIM3D_FMT_PCT;
	
	for (u32 i = 0; i < draw_num; ++i) {
		NNS_PRIM3D_PCT* v = v_tbl + (6 * i);
		Uint16 frame = gm_ring_sys_work->draw_ring_uv_frame / GMD_RING_ROLL_UV_FRAME_WAIT;
		//gm_ring_roll_uv
		// UV座標設定 (暫定)
		v[0].Tex.u = v[1].Tex.u = gm_ring_roll_uv[GMD_RING_ROLL_UV_POS_U][frame];
		v[2].Tex.u = v[3].Tex.u = gm_ring_roll_uv[GMD_RING_ROLL_UV_POS_U][frame] + GMD_RING_ROLL_UV_SIZE;
		v[0].Tex.v = v[2].Tex.v = gm_ring_roll_uv[GMD_RING_ROLL_UV_POS_V][frame];
		v[1].Tex.v = v[3].Tex.v = gm_ring_roll_uv[GMD_RING_ROLL_UV_POS_V][frame] + GMD_RING_ROLL_UV_SIZE;
		
		// カラー設定
		v[0].Col = color;
		v[1].Col = v[2].Col = v[3].Col = v[0].Col;
				
		// 頂点設定 & 移動
#if 0
		v[0].Pos.x = v[1].Pos.x = -(-3.0f * 3.2f) + FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].x - vec.x);
		v[2].Pos.x = v[3].Pos.x =  (-3.0f * 3.2f) + FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].x - vec.x);
		v[0].Pos.y = v[2].Pos.y = -(-3.0f * 3.2f) - FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].y - vec.y);
		v[1].Pos.y = v[3].Pos.y =  (-3.0f * 3.2f) - FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].y - vec.y);
#else
		v[0].Pos.x =  (sin45)  + FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].x - vec.x);
		v[1].Pos.x =  (sin135) + FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].x - vec.x);
		v[2].Pos.x = -(sin135) + FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].x - vec.x);
		v[3].Pos.x = -(sin45)  + FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].x - vec.x);
		v[0].Pos.y =  (cos45)  - FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].y - vec.y);
		v[1].Pos.y =  (cos135) - FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].y - vec.y);
		v[2].Pos.y = -(cos135) - FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].y - vec.y);
		v[3].Pos.y = -(cos45)  - FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].y - vec.y);
#endif // 0
		v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z =  -1.0f + FX_FX32_TO_F32(ring_sys->draw_ring_pos[i].z - vec.z);
		
#ifdef GMD_RING_DRAW_STRIP_TEST
		if (i != 0)				v[-1] = v[0];
		if (i + 1 < draw_num)	v[ 4] = v[3]; 
#else // GMD_RING_DRAW_STRIP_TEST
		// 6頂点化
		v[5] = v[3];
		v[4] = v[1];
		v[3] = v[2];
#endif // GMD_RING_DRAW_STRIP_TEST
	}
	
	// ソート設定
	dat.sortZ = nnDistanceVector(&v_tbl->Pos, &camera_pos);

	amMatrixPush(&mtx);
	ObjDraw3DNNDrawPrimitive(&dat);
	amMatrixPop();
#endif // GMD_RING_DRAW_PRIMITIVE
}
#endif //_IPHONE

// ================================================================
// GmRingStaticVarInit
/*!
 static変数の初期化
 */
// ================================================================
void GmRingStaticVarInit(void)
{
	g_gm_ring_size = GMD_RING_DEF_SIZE;	//!< リングサイズ
	
	gm_ring_tcb = NULL;					//!< リングシステムTCB
	gm_ring_sys_work = NULL;			//!< リングシステムワーク(TCBワークで取得)
	
	gm_ring_obj_3d = NULL;				//!< リング描画用オブジェクト		
	
#if 1
	gm_ring_fall_acc_x = 0;							//!< リング落下加速度 X
	gm_ring_fall_acc_y = GMD_RING_FALL_ACC;			//!< リング落下加速度 Y
#else
	gm_ring_fall_acc = GMD_RING_FALL_ACC;			//!< リング落下加速度
#endif
	gm_ring_scale = FX32_ONE;						//!< リング描画時スケール(演出用)
	gm_ring_die_offset = GMD_RING_DIE_DEF_OFFSET;	//!< 通常リング 画面外判定オフセット値
}

// ================================================================
// test_func
/*!
  テスト関数
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return   返値説明
 
  @note
  補足説明
 */
// ================================================================

