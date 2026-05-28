// ==========================================================================
/*!
  @file gmGmkGear.cpp
  @brief ギミック歯車

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGmkGear.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 *	ギミック関係図
 *
 *		移動歯車			スイッチ歯車発動終了で、次回生成無し
 *		↑↓発動  ↑監視
 *		移動歯車終端		移動歯車スイッチ発動終了で、次回生成時は歯車化
 *
 *
 *		移動歯車			発動でスイッチ歯車まわし用シーケンス開始	監視でシーケンス終了
 *		↑発動  ↓監視
 *		スイッチ歯車		発動で移動歯車にあわせて回る
 *
 *
 *
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "objObject.h"
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmComEfct.h"
#include "gmGmkSwitch.h"
#include "gmSound.h"
#include "gmPadVib.h"

#include "gmGmkGear.h"

// データヘッダ
#include "common/model/GMK_GEAR_MDL.HMB"
#include "common/model/GMK_GEAR2_MDL.HMB"
#include "common/model/GMK_GEAR2_OPT_MDL.HMB"
#include "common/arc/GMK_GEAR.HMB"

//----- Definitions ---------------------------------------------------------

#define GMD_GMK_GEAR_ADD_DATA_MAX	(IDB_GMK_GEAR_GMK_GEAR2_TXB + 1)	//!< 追加データ最大数 データ最大数が変わったら変更が必要



/* 通常歯車 */
/* eve_rec->left */
// 回転速度
// 0 ～ 3
/* eve_rec->top */
// 同期タイプ
// 0 ～ 3
/* eve_rec->width */
/* eve_rec->height */
/* eve_rec->flag */
//EVE_FLAG_
#define GMD_GMK_EVE_FLAG_GEAR_ROT_LEFT	(0x0001)		//!< 反時計まわり


/* 移動歯車 */
/* eve_rec->left */
/* eve_rec->top */
/* eve_rec->width */
// 連動ID (スイッチ発動タイプの時) 0 ～
/* eve_rec->height */
/* eve_rec->flag */
//EVE_FLAG_
#define GMD_GMK_EVE_FLAG_MGEAR_MOVE_V			(0x0001)		//!< 移動方向 縦 (OFFの時横)
#define GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP		(0x0002)		//!< 移動方向 左・上 (OFFの時右・下)
#define GMD_GMK_EVE_FLAG_MGEAR_CAM_ADJUST_OFF	(0x0004)		//!< カメラ補正OFF
#define GMD_GMK_EVE_FLAG_MGEAR_SPEED_UP			(0x0008)		//!< 移動速度設定 ONで速い

// Local専用
//#define GMD_GMK_EVE_FLAG_MGEAR_LOCAL_CREATE	(0x0080)		//!< ローカル生成イベント(移動終端からの設定)

/* 移動歯車終端 */
/* eve_rec->left */
/* eve_rec->top */
/* eve_rec->width */
/* eve_rec->height */
/* eve_rec->flag */
#define GMD_GMK_EVE_FLAG_MGEAR_END_SWITCH	(0x0001)		//!< 歯車スイッチ用終端
/* eve_rec->byte_param[1] */
// スイッチ歯車発動済みフラグ

/* 歯車スイッチ */
/* eve_rec->left */
/* eve_rec->top */
/* eve_rec->width */
// 連動ID (スイッチ発動タイプの時) 0 ～
/* eve_rec->height */
// 連動する壁が閉まるのにかかる時間 *30フレーム (* 0.5秒)
/* eve_rec->flag */
//EVE_FLAG_
/* eve_rec->byte_param[1] */
// 歯車スイッチ発動済みフラグ

#define GMD_GMK_GEAR_SE_SPEED_MIN	(0.2f)		//!< 歯車 SE速度最小値
#define GMD_GMK_GEAR_SE_SPEED_MAX	(0.8f)		//!< 歯車 SE速度最大値
#define GMD_GMK_GEAR_SE_MIN_DIST	(100.f)		//!< 歯車SE 最小距離
#define GMD_GMK_GEAR_SE_MAX_DIST	(400.f)		//!< 歯車SE 最大距離

#define GMD_GMK_GEAR_MOVE_SE_MIN_SPD	(0x0004)	//!< 移動歯車SE 最小速度
#define GMD_GMK_GEAR_MOVE_SE_MAX_SPD	(0x0360)	//!< 移動歯車SE 最大速度
#define GMD_GMK_GEAR_MOVE_SE_MIN_DIST	(300.f)		//!< 移動歯車SE 最小距離
#define GMD_GMK_GEAR_MOVE_SE_MAX_DIST	(600.f)		//!< 移動歯車SE 最大距離


/// 歯車ワーク
typedef struct tag_GMS_GMK_GEAR_WORK {
	GMS_ENEMY_3D_WORK		gmk_work;

	OBS_ACTION3D_NN_WORK	obj_3d_gear_opt;		//!< 移動歯車 オプション歯車
	OBS_ACTION3D_NN_WORK	obj_3d_gear_opt_ashiba;	//!< 移動歯車 オプション歯車

	// 通常歯車
	u32					col_type;	//!< 地形タイプ(角度依存)
	float				dir_speed;	//!< ギア回転速度
	float				dir_temp;	//!< 回転ローカル加算値

	u16					prev_dir;	//!< 前回描画角度

	// 移動歯車
	u16					move_draw_dir;		//!< 移動歯車描画角度
	u16					old_move_draw_dir;	//!< 移動歯車描画角度
	s16					move_draw_dir_spd;	//!< 移動歯車回転量
	s16					move_draw_dir_ofst;	//!< 移動歯車 演出用描画角度オフセット
	s16					move_draw_dir_limit;	//!< 移動歯車 演出描画角度の回転量MAX保持
	u16					move_stagger_dir_cnt;	//!< 移動歯車 おっとっとゆれ演出用カウンタ
	u16					move_stagger_step;		//!< 移動歯車 おっとっとゆれ演出進行ステップ
	fx32				move_stagger_dir_spd;	//!< 移動歯車演出用回転速度
	fx32				stop_timer;		//!< 停止時間タイマー
	fx32				rect_ret_timer;	//!< 矩形復帰タイマー
	fx32				move_end_x;		//!< 移動限界 X位置
	fx32				move_end_y;		//!< 移動限界 Y位置
	fx32				ret_max_speed;	//!< 復帰時最高速度
	BOOL				vib_end;		//!< 振動済みチェック

	// 歯車スイッチ
	s32					open_rot_dist;			//!< 壁展開に必要な回転量
	u16					gear_sw_dir_base;		//!< スイッチ歯車回転ベース
	s32					close_rot_spd;			//!< 壁を閉じる時の歯車速度


	// オブジェクト保存
	OBS_OBJECT_WORK		*gear_end_obj;	//!< 対になる移動歯車終端 (移動歯車)
	OBS_OBJECT_WORK		*move_gear_obj;	//!< 対になる移動歯車 (移動歯車終端, スイッチ歯車)
	OBS_OBJECT_WORK		*sw_gear_obj;	//!< 対になる移動スイッチ歯車 (移動歯車)

	// 歯車転がりSE用ハンドル
	GSS_SND_SE_HANDLE	*h_snd_gear;	//!< 歯車転がり音

} GMS_GMK_GEAR_WORK;

// 地形設定
#define GMD_GMK_GEAR_COL_RECT_WIDTH		(176)
#define GMD_GMK_GEAR_COL_RECT_HEIGHT	(176)
#define GMD_GMK_GEAR_COL_RECT_OFST_X	(-GMD_GMK_GEAR_COL_RECT_WIDTH/2)
#define GMD_GMK_GEAR_COL_RECT_OFST_Y	(-GMD_GMK_GEAR_COL_RECT_HEIGHT/2)
#define GMD_GMK_GEAR_COL_TYPE_NUM		(8)							//!< 地形タイプ数

// 通常歯車設定
#define GMD_GMK_GEAR_ONE_DIR		(45)												//!< 歯車ひとつあたりの角度
#define GMD_GMK_GEAR_ROT_COL_SPEED	((u16)(0x10000*GMD_GMK_GEAR_ONE_DIR/8/360))			//!< ギア地形回転速度 1単位
#define GMD_GMK_GEAR_ROT_DIV		(32.f)												//!< ギア回転量最小単位算出用
#define GMD_GMK_GEAR_ROT_SPEED_BASE	(GMD_GMK_GEAR_ROT_COL_SPEED/GMD_GMK_GEAR_ROT_DIV)	//!< ギア回転ベース速度
#define GMD_GMK_GEAR_ROT_SPEED_ADD	(GMD_GMK_GEAR_ROT_COL_SPEED/GMD_GMK_GEAR_ROT_DIV)	//!< ギア回転加算速度
#define GMD_GMK_GEAR_ROT_SYNC		(GMD_GMK_GEAR_ROT_COL_SPEED*2)						//!< 同期用回転量

#define GMD_GMK_GEAR_ROT_SPEED_LEVEL_NUM	(4)							//!< ギア回転速度段階数
#define GMD_GMK_GEAR_ROT_SYNC_TYPE_NUM		(4)							//!< ギア回転同期タイプ数

// 移動歯車設定
#define GMD_GMK_MOVE_GEAR_ONE_DIR				(30)						//!< 歯車ひとつあたりの角度
#define GMD_GMK_MOVE_GEAR_BODY_DEF_RECT_LEFT	(-16)						//!< 発動チェック矩形
#define GMD_GMK_MOVE_GEAR_BODY_DEF_RECT_RIGHT	(16)
#define GMD_GMK_MOVE_GEAR_BODY_DEF_RECT_TOP		(-72)
#define GMD_GMK_MOVE_GEAR_BODY_DEF_RECT_BOTTOM	(GMD_GMK_MOVE_GEAR_DEF_RECT_TOP+32)
#define GMD_GMK_MOVE_GEAR_DEF_RECT_LEFT			(-80)						//!< 終端取得矩形
#define GMD_GMK_MOVE_GEAR_DEF_RECT_RIGHT		(80)
#define GMD_GMK_MOVE_GEAR_DEF_RECT_TOP			(-80)
#define GMD_GMK_MOVE_GEAR_DEF_RECT_BOTTOM		(80)

#define GMD_GMK_MOVE_GEAR_FIELD_RECT_LEFT	(-8)						//!< 地形矩形
#define GMD_GMK_MOVE_GEAR_FIELD_RECT_TOP	(-8)
#define GMD_GMK_MOVE_GEAR_FIELD_RECT_RIGHT	(8)
#define GMD_GMK_MOVE_GEAR_FIELD_RECT_BOTTOM	(8)

//#define GMD_GMK_MOVE_GEAR_R					(61-4)						//!< 移動歯車半径
#define GMD_GMK_MOVE_GEAR_R					(64)						//!< 移動歯車半径
#define GMD_GMK_MOVE_GEAR_FORCE_EXT_DIST	(4*FX32_ONE)				//!< 中心からこの距離以内の場合は、即時発動

#define GMD_GMK_MOVE_GEAR_STOP_RET_TIMER	(180*FX32_ONE)				//!< 歯車戻り開始停止時間
#define GMD_GMK_MOVE_GEAR_SPD_DEC			(0x400)						//!< 歯車減速値

#define GMD_GMK_MOVE_GEAR_RET_SPD_ACC		(0x200)						//!< 歯車戻り速度加速度
#define GMD_GMK_MOVE_GEAR_RET_SPD_MAX		(0x4000)					//!< 歯車戻り速度最大速度
#define GMD_GMK_MOVE_GEAR_RECT_RET_TIMER	(16*FX32_ONE)				//!< 矩形復帰待機時間

#define GMD_GMK_MOVE_GEAR_SW_SPD_MIN		(0x4000)					//!< 歯車回し時最低速度
#define GMD_GMK_MOVE_GEAR_SW_SPD_ACC		(0x0100)					//!< 歯車回し時最低加速度

#define GMD_GMK_MOVE_GEAR_SW_FREE_WAIT_TIME	(60)						//!< スイッチ歯車開放待機時間

#define GMD_GMK_MOVE_GEAR_STAGGER_FRAME		(120/2)						//!< おっとっとモーション時間
#if GMD_GMK_GEAR_STAGGER_BACK
#define GMD_GMK_MOVE_GEAR_STAGGER_DIR_AMP	(GMD_GMK_MOVE_GEAR_ONE_DIR/16*0x10000/360)	//!< おっとっと時ゆれ幅
#else
#define GMD_GMK_MOVE_GEAR_STAGGER_DIR_AMP	(GMD_GMK_MOVE_GEAR_ONE_DIR/16*0x10000/360)	//!< おっとっと時ゆれ幅
#endif
#define GMD_GMK_MOVE_GEAR_DIR_SCALE			(2)							//!< 移動歯車回転量スケール

// 移動歯車終端設定
#define GMD_GMK_MOVE_GEAR_END_ATK_RECT_LEFT		(-32)					//!< 終端発動用矩形
#define GMD_GMK_MOVE_GEAR_END_ATK_RECT_TOP		(-32)
#define GMD_GMK_MOVE_GEAR_END_ATK_RECT_RIGHT	(32)
#define GMD_GMK_MOVE_GEAR_END_ATK_RECT_BOTTOM	(32)

// 歯車スイッチ設定
#define GMD_GMK_GEAR_SW_DEF_ROT_NUM			(1)							//!< スイッチ発動に必要な基本回転数
#define GMD_GMK_GEAR_SW_DEF_ROTDIR			(GMD_GMK_GEAR_SW_DEF_ROT_NUM*0x10000)		//!< スイッチ発動に必要な基本回転量

#define GMD_GMK_GEAR_SW_ATK_RECT_LEFT		(-92)						//!< スイッチ発動用矩形
#define GMD_GMK_GEAR_SW_ATK_RECT_TOP		(-92)
#define GMD_GMK_GEAR_SW_ATK_RECT_RIGHT		(92)
#define GMD_GMK_GEAR_SW_ATK_RECT_BOTTOM		(92)

// 移動歯車・歯車スイッチ オプション歯車
#define GMD_GMK_GEAR_OPT_OFST_Z				(GMD_OBJ_DEFAULT_POS_Z_A_FRONT - GMD_OBJ_GIMMICK_POS_Z_BACK)
#define GMD_GMK_GEAR_OPT_ASHIBA_OFST_Z		(GMD_OBJ_DEFAULT_POS_Z_A_BACK - GMD_OBJ_GIMMICK_POS_Z_BACK)

// 振動設定
#define GMD_GMK_MOVE_GEAR_VIB_FRAME	(60.f)	//!< 移動歯車発動時振動

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkGearInFunc(OBS_OBJECT_WORK *obj_work);

//static void gmGmkGearSetColDef(GMS_GMK_GEAR_WORK *gear_work);
static inline void gmGmkGearChangeCol(GMS_GMK_GEAR_WORK *gear_work);
static void gmGmkGearDest(MTS_TASK_TCB *tcb);
static void gmGmkGearFwInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkGearFwMain(OBS_OBJECT_WORK *obj_work);

static void gmGmkMoveGearFwInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearFwMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearMoveInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearMoveMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearRetWaitInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearRetWaitMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearRetInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearRetMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearSwitchExeInit(OBS_OBJECT_WORK *obj_work, s16 cam_ofst_x, s16 cam_ofst_y);
static void gmGmkMoveGearSwitchExeMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearEndStaggerInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearEndStaggerMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearEndInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearEndMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearSwitchRetWaitInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearSwitchRetWaitMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearSwitchRetInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearSwitchRetMain(OBS_OBJECT_WORK *obj_work);
static BOOL gmGmkMoveGearCheckSwitchMove(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearSetSpd(OBS_OBJECT_WORK *obj_work, fx32 spd_m);
static void gmGmkMoveGearBodyDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static void gmGmkMoveGearDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);

static void gmGmkMoveGearEndSwitchFwInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearEndSwitchFwMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkMoveGearEndAtkHitFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);

static void gmGmkGearSwFwInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkGearSwFwMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkGearSwRotExtWaitInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkGearSwRotExtWaitMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkGearSwRotExtInit(OBS_OBJECT_WORK *obj_work);
static void gmGmkGearSwRotExtMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkGearSwitchAtkHitFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect);
static void gmGmkGearSetRotFlow(OBS_OBJECT_WORK *gear_obj, GMS_PLAYER_WORK *ply_work, Angle32 move_dir);

static void gmGmkGearMoveSwDraw(OBS_OBJECT_WORK *obj_work);

static void gmGmkGearLastFunc(OBS_OBJECT_WORK *obj_work);
static void gmGmkGearMoveLastFunc(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_gear_obj_3d_list = NULL;				//!< 通常
static OBS_ACTION3D_NN_WORK *gm_gmk_gear_move_obj_3d_list = NULL;			//!< 移動タイプ
static OBS_ACTION3D_NN_WORK *gm_gmk_gear_sw_obj_3d_list = NULL;				//!< スイッチタイプ
static OBS_ACTION3D_NN_WORK *gm_gmk_gear_opt_obj_3d_list = NULL;			//!< 移動・スイッチタイプ オプション歯車
static u8	*gm_gmk_gear_add_data[GMD_GMK_GEAR_ADD_DATA_MAX] = {NULL};		//!< 追加データ

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkGearBuild
/*!
 *	ギミック 歯車 データ構築
 */
// ==========================================================================
void GmGmkGearBuild(void)
{
	s32				i;
	AMS_AMB_HEADER	*amb;

	// 地形データ・その他データ取得
	amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR);
	amBindConv((u8*)amb);		// 中のデータはコンバートしない
	for (i = 0; i < GMD_GMK_GEAR_ADD_DATA_MAX; i++) {
		gm_gmk_gear_add_data[i] = (u8*)amBindGet(amb, i);
	}

	// オブジェクト構築
	// 通常
	gm_gmk_gear_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR_TEX),
								0/*draw_flag*/);
	// 移動タイプ
	gm_gmk_gear_move_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR2_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR_TEX),
								0/*draw_flag*/);
	// スイッチタイプ
	gm_gmk_gear_sw_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR2_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR_TEX),
								0/*draw_flag*/,
								gm_gmk_gear_add_data[IDB_GMK_GEAR_GMK_GEAR2_TXB]);

	// オプション歯車
	gm_gmk_gear_opt_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR2_OPT_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkGearFlush
/*!
 *	ギミック 歯車 データ片付け
 */
// ==========================================================================
void GmGmkGearFlush(void)
{
	AMS_AMB_HEADER	*amb;

	// オブジェクト開放
	amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR_MODEL);
	// 通常
	GmGameDBuildRegFlushModel(gm_gmk_gear_obj_3d_list, amb->file_num);

	amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR2_MODEL);
	// 移動タイプ
	GmGameDBuildRegFlushModel(gm_gmk_gear_move_obj_3d_list, amb->file_num);
	// スイッチタイプ
	GmGameDBuildRegFlushModel(gm_gmk_gear_sw_obj_3d_list, amb->file_num);

	amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_GEAR2_OPT_MODEL);
	// オプション歯車
	GmGameDBuildRegFlushModel(gm_gmk_gear_opt_obj_3d_list, amb->file_num);

	// アーカイブ参照をクリア
	MI_CpuClear8(gm_gmk_gear_add_data, sizeof(gm_gmk_gear_add_data));
}

// ==========================================================================
// GmGmkGearSetLight
/*!
 *	ギミック 歯車用ライト設定
 */
// ==========================================================================
void GmGmkGearSetLight(void)
{
	NNS_RGBA	light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	NNS_VECTOR	light_vec;
	float	intensity;
	
	//	light_vec.x = 0.4f;
	//	light_vec.y = 0.0f;
	//	light_vec.z = -1.0f;
	light_vec.x = -0.35f;
	light_vec.y = 2.25f;
	light_vec.z = -0.9f;
	nnNormalizeVector(&light_vec, &light_vec);
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		// Zone4-3のみ暗く
		intensity = GMD_LIGHT_GEAR_DARK_INTENSITY;
	} else {
		// 通常
#if _WII
		intensity = 0.8f;
#else
		intensity = GMD_LIGHT_COMN_INTENSITY;
#endif
	}
	ObjDrawSetParallelLight(NNE_LIGHT_1, &light_col, intensity, &light_vec);
}

// ==========================================================================
// GmGmkGearInit
/*!
 *	ギミック 歯車 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0		1の時はローカル生成タイプ
 *
 *	@note
 *		移動歯車\n
 *			user_flag   : OBD_OBJECT_USER_0 接着プレイヤーの移動速度設定切り替え
 *						: OFFでユーザーシーケンスが設定
 *						: ONでギミックが設定
 *			user_flag   : OBD_OBJECT_USER_1 スイッチ歯車終点設定
 *			user_flag   : OBD_OBJECT_USER_2 速度アップタイプ
 *			user_flag   : OBD_OBJECT_USER_3 縦横移動タイプ
 *
 *			byte_param[1] : スイッチ発動済みフラグ
 *			
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkGearInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;
	GMS_GMK_GEAR_WORK	*gear_work;
	OBS_RECT_WORK		*rect_work;
	OBS_COLLISION_WORK	*col_work;
	VecFx32				temp_pos;

	//UNREFERENCED_PARAMETER(type);

	// オブジェクト生成
	if (eve_rec->id == GMD_EVENT_ID_GMK_GEAR_SWITCH) {
		// スイッチタイプは通常優先
		obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_GEAR_WORK), "GMK_GEAR_SW");
	}
	else {
		obj_work = GMM_ENEMY_CREATE_RIDE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_GEAR_WORK), "GMK_GEAR");
	}
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	if (eve_rec->id == GMD_EVENT_ID_GMK_MOVE_GEAR && eve_rec->byte_param[1]) {
		// 移動歯車 スイッチ発動済みにつきギミックとしての処理なし
		obj_work->disp_flag |= OBD_DISP_NODISP;
		obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;
		obj_work->flag		|= OBD_OBJECT_NOHIT;
		return (obj_work);
	}

	// ppIn差し替え
	obj_work->ppIn = gmGmkGearInFunc;

	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmGmkGearDest);

	// サウンドハンドル取得
	gear_work->h_snd_gear = GsSoundAllocSeHandle();
	// 移動SE
	GmSoundStopSE(gear_work->h_snd_gear);
	GmSoundPlaySE("Gear", gear_work->h_snd_gear);


	// モデル初期化
	if (eve_rec->id == GMD_EVENT_ID_GMK_GEAR) {
		CriFloat32	ctrl_val = GMD_GMK_GEAR_SE_SPEED_MIN;

		// 通常タイプ
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_gear_obj_3d_list[IDB_GMK_GEAR_MDL_GMK_GEAR_ZNO],
						&gmk_work->obj_3d);

		// 優先設定
		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

		// ppLast処理設定
		obj_work->ppLast = gmGmkGearLastFunc;		// SE対応

		// SE速度設定
		ctrl_val += ((GMD_GMK_GEAR_SE_SPEED_MAX - GMD_GMK_GEAR_SE_SPEED_MIN) / (GMD_GMK_GEAR_ROT_SPEED_LEVEL_NUM-1)) *
								MTM_MATH_CLIP(eve_rec->left, 0, (GMD_GMK_GEAR_ROT_SPEED_LEVEL_NUM-1));
		gear_work->h_snd_gear->au_player->SetAisac("Speed", ctrl_val);
	}
	else if (eve_rec->id == GMD_EVENT_ID_GMK_MOVE_GEAR) {
		// 移動タイプ
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_gear_move_obj_3d_list[IDB_GMK_GEAR2_MDL_GMK_GEAR2_ZNO],
						&gmk_work->obj_3d);

		// 優先設定
		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;//GMD_OBJ_GIMMICK_POS_Z_FRONT;

		// オプション歯車(モーション無しなので解放不要)
		ObjCopyAction3dNNModel(&gm_gmk_gear_opt_obj_3d_list[IDB_GMK_GEAR2_OPT_MDL_GMK_GEAR2_NEJI_ZNO],
						&gear_work->obj_3d_gear_opt);
		ObjCopyAction3dNNModel(&gm_gmk_gear_opt_obj_3d_list[IDB_GMK_GEAR2_OPT_MDL_GMK_GEAR2_ASHIBA_ZNO],
						&gear_work->obj_3d_gear_opt_ashiba);

		// ppLast処理設定
		obj_work->ppLast = gmGmkGearMoveLastFunc;		// SE対応
	}
	else {
		// スイッチタイプ
		ObjObjectCopyAction3dNNModel(obj_work,
						&gm_gmk_gear_sw_obj_3d_list[IDB_GMK_GEAR2_MDL_GMK_GEAR2_ZNO],
						&gmk_work->obj_3d);

		// 優先設定
		obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;//GMD_OBJ_GIMMICK_POS_Z_BACK;

		// オプション歯車(モーション無しなので解放不要)
		ObjCopyAction3dNNModel(&gm_gmk_gear_opt_obj_3d_list[IDB_GMK_GEAR2_OPT_MDL_GMK_GEAR2_NEJI_ZNO],
						&gear_work->obj_3d_gear_opt);
		ObjCopyAction3dNNModel(&gm_gmk_gear_opt_obj_3d_list[IDB_GMK_GEAR2_OPT_MDL_GMK_GEAR2_ASHIBA_ZNO],
						&gear_work->obj_3d_gear_opt_ashiba);

		// ppLast処理設定
		obj_work->ppLast = gmGmkGearMoveLastFunc;		// SE対応
	}


	/* 地形標準設定 */
	col_work = &gear_work->gmk_work.ene_com.col_work;

	col_work->obj_col.obj		= (OBS_OBJECT_WORK*)gear_work;
	//col_work->obj_col.diff_data	= (s8*)gm_gmk_gear_add_data[0*2];			// 基本地形情報
	//col_work->obj_col.dir_data	= (s8*)gm_gmk_gear_add_data[0*2+1];		// 基本地形情報
	col_work->obj_col.width		= GMD_GMK_GEAR_COL_RECT_WIDTH;			// 地形サイズ設定(ドット)
	col_work->obj_col.height	= GMD_GMK_GEAR_COL_RECT_HEIGHT;
	col_work->obj_col.ofst_x	= GMD_GMK_GEAR_COL_RECT_OFST_X;
	col_work->obj_col.ofst_y	= GMD_GMK_GEAR_COL_RECT_OFST_Y;
	col_work->obj_col.flag		|= OBD_COLOBJ_NOFREE_DIFF_DATA |		// diff_dataを開放しない
								OBD_COLOBJ_NOFREE_DIR_DATA |			// dir_dataを開放しない
								OBD_COLOBJ_NODIR_PARENT;				// 親の角度無視


	/* フラグ設定 */
	// 共通
	obj_work->move_flag &= ~OBD_MOVE_FALL;				// 落下無し
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// プレイヤーを圧死させない
	gmk_work->ene_com.enemy_flag |= GMD_ENEMY_FLAG_NOPRESSDIE;

	// ライト設定
	gmk_work->obj_3d.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	gmk_work->obj_3d.use_light_flag |= OBD_LIGHT_USE_FLAG_1;
//	gear_work->obj_3d_gear_opt.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
//	gear_work->obj_3d_gear_opt.use_light_flag |= OBD_LIGHT_USE_FLAG_1;
	gear_work->obj_3d_gear_opt_ashiba.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	gear_work->obj_3d_gear_opt_ashiba.use_light_flag |= OBD_LIGHT_USE_FLAG_1;
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
		gear_work->obj_3d_gear_opt.use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
		gear_work->obj_3d_gear_opt.use_light_flag |= OBD_LIGHT_USE_FLAG_1;
	}

	/* タイプ別設定 */
	if (eve_rec->id == GMD_EVENT_ID_GMK_GEAR) {
	// 通常歯車
		float	div;
		s32		rot_time;

		// フラグ設定
		obj_work->move_flag |= OBD_MOVE_NOMOVE;				// 移動無し
		obj_work->move_flag |= OBD_MOVE_NOCOL;				// 地形あたり無し


		// 速度取得
		gear_work->dir_speed = GMD_GMK_GEAR_ROT_SPEED_BASE +
						MTM_MATH_CLIP(eve_rec->left, 0, (GMD_GMK_GEAR_ROT_SPEED_LEVEL_NUM-1)) * GMD_GMK_GEAR_ROT_SPEED_ADD;
		if (eve_rec->flag & GMD_GMK_EVE_FLAG_GEAR_ROT_LEFT) {
			gear_work->dir_speed	= -gear_work->dir_speed;
		}

		// 同期設定
		rot_time = (s32)(0x10000 / MTM_MATH_ABS(gear_work->dir_speed));
		gear_work->dir_temp = (float)((g_gm_main_system.sync_time % rot_time) * gear_work->dir_speed);

		//gear_work->dir_temp = (float)((g_gm_main_system.sync_time%0x10000) * MTM_MATH_ABS(gear_work->dir_speed));
		gear_work->dir_temp += MTM_MATH_CLIP(eve_rec->top, 0, (GMD_GMK_GEAR_ROT_SYNC_TYPE_NUM-1)) * GMD_GMK_GEAR_ROT_SYNC;
		div = gear_work->dir_temp / 0x10000;
		if (MTM_MATH_ABS(div) >= 1.0f) {
			gear_work->dir_temp -= div*0x10000;
		}

		// 初期描画回転量設定
		if (gear_work->dir_speed > 0) {
			obj_work->dir.z = (u16)(((u32)nnRoundOff(gear_work->dir_temp) / GMD_GMK_GEAR_ROT_COL_SPEED) * GMD_GMK_GEAR_ROT_COL_SPEED);
		}
		else {
			obj_work->dir.z = (u16)(((u32)(nnRoundOff(gear_work->dir_temp) + (GMD_GMK_GEAR_ROT_COL_SPEED-1)) / GMD_GMK_GEAR_ROT_COL_SPEED) * GMD_GMK_GEAR_ROT_COL_SPEED);
		}
		gear_work->prev_dir = obj_work->dir.z;

		// 地形データ設定
		gmGmkGearChangeCol(gear_work);

		// 通常歯車シーケンスへ
		gmGmkGearFwInit(obj_work);
	}
	else if (eve_rec->id == GMD_EVENT_ID_GMK_MOVE_GEAR) {
	// 移動歯車
		// 描画処理変更
		obj_work->ppOut = gmGmkGearMoveSwDraw;

		// 地形情報設定
		col_work->obj_col.diff_data	= (s8*)gm_gmk_gear_add_data[IDB_GMK_GEAR_GMK_GEAR_C_DF];
		col_work->obj_col.dir_data	= (u8*)gm_gmk_gear_add_data[IDB_GMK_GEAR_GMK_GEAR_C_DI];

		// A面指定へ
		obj_work->flag &= ~OBD_OBJECT_B;

		// 回転描画無し ユーザー行列反映あり
		obj_work->disp_flag |= OBD_DISP_NODIR | OBD_DISP_USERMTX_RIGHT;

		// 地形チェックあり
		obj_work->move_flag &= ~OBD_MOVE_NOCOL;
		// オブジェクト地形チェックなし 角度移動あり 落下あり
		obj_work->move_flag |= OBD_MOVE_NOCOLOBJ | OBD_MOVE_DIR | OBD_MOVE_FALL;

		if (eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_SPEED_UP) {
			// 移動速度速いタイプ
			obj_work->user_flag |= OBD_OBJECT_USER_2;
		}

		/* 矩形設定 */
		// ギミック発動
		rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_BODY];	// サーチターゲットでないので位置がずれても問題ない
		rect_work->ppDef = gmGmkMoveGearBodyDefFunc;
		rect_work->ppHit = NULL;
		//ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
		//ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		// 仮矩形
		ObjRectWorkSet(rect_work,
							GMD_GMK_MOVE_GEAR_BODY_DEF_RECT_LEFT, GMD_GMK_MOVE_GEAR_BODY_DEF_RECT_TOP,
							GMD_GMK_MOVE_GEAR_BODY_DEF_RECT_RIGHT, GMD_GMK_MOVE_GEAR_BODY_DEF_RECT_BOTTOM);

		// 終端チェック
		rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_DEF];
		rect_work->ppDef = gmGmkMoveGearDefFunc;
		rect_work->ppHit = NULL;
		ObjRectAtkSet(rect_work, 0/*flag*/, 0/*power*/);
		ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_EX_SETTING, GMD_OBJ_RECT_DEF_POWER_DEFAULT);	// 特殊設定のみあたる
		// 仮矩形
		ObjRectWorkSet(rect_work,
							GMD_GMK_MOVE_GEAR_DEF_RECT_LEFT, GMD_GMK_MOVE_GEAR_DEF_RECT_TOP,
							GMD_GMK_MOVE_GEAR_DEF_RECT_RIGHT, GMD_GMK_MOVE_GEAR_DEF_RECT_BOTTOM);
		//rect_work->flag |= OBD_RECT_OUT;
		rect_work->flag |= OBD_RECT_GROUP;


		/* 地形あたり矩形設定 */
		ObjObjectFieldRectSet(obj_work, GMD_GMK_MOVE_GEAR_FIELD_RECT_LEFT,
										GMD_GMK_MOVE_GEAR_FIELD_RECT_TOP,
										GMD_GMK_MOVE_GEAR_FIELD_RECT_RIGHT,
										GMD_GMK_MOVE_GEAR_FIELD_RECT_BOTTOM);

		if (type == 0) {
		// 通常生成
			// 付近にある地形へ接着する
			obj_work->move_flag |= OBD_MOVE_UNDER;
			obj_work->prev_pos = obj_work->pos;
			if (eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
				// 縦方向移動
				// 右へ接着
				obj_work->dir.z = 0x4000;
				obj_work->pos.x += 8*FX32_ONE;
				obj_work->prev_pos.x -= 8*FX32_ONE;
				obj_work->move.x = +16*FX32_ONE;
				obj_work->spd.x = +16*FX32_ONE;

				obj_work->move_flag &= ~OBD_MOVE_FALL;	// 落下しない
				obj_work->user_flag |= OBD_OBJECT_USER_3; //縦横移動タイプ

				// 重力方向変更
				//obj_work->dir_fall = 0xC000;
			}
			else {
				// 横方向移動
				// 下へ接着
				//obj_work->dir.z = 0x0000;
				obj_work->pos.y += 8*FX32_ONE;
				obj_work->prev_pos.y -= 8*FX32_ONE;
				obj_work->move.y = +16*FX32_ONE;
				obj_work->spd.y = +16*FX32_ONE;
				obj_work->user_flag &= ~OBD_OBJECT_USER_3; //縦横移動タイプ
			}
			// 地面方向地形判定
			g_obj.ppCollision(obj_work);
			// 強制接地
			obj_work->move_flag |= OBD_MOVE_UNDER; 

			// 後ろ方向チェック(配置時めり込みチェック)
			temp_pos = obj_work->pos;		// 退避
			obj_work->prev_pos = obj_work->pos;
			obj_work->move.x = obj_work->move.y = 0;
			obj_work->spd.x = obj_work->spd.y = 0;
			if (eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
				if (eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) {
					// 後方移動
					obj_work->prev_pos.y -= 8*FX32_ONE; 
					obj_work->move.y	= +8*FX32_ONE;
					obj_work->spd.y		= +8*FX32_ONE;
					obj_work->disp_flag |= OBD_DISP_HFLIP;
				}
				else {
					// 前方移動
					obj_work->prev_pos.y += 8*FX32_ONE; 
					obj_work->move.y	= -8*FX32_ONE;
					obj_work->spd.y		= -8*FX32_ONE;
				}
			}
			else {
				if (eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) {
					// 後方移動
					obj_work->prev_pos.x -= 8*FX32_ONE; 
					obj_work->move.x	= +8*FX32_ONE;
					obj_work->spd.x		= +8*FX32_ONE;
					obj_work->disp_flag |= OBD_DISP_HFLIP;
				}
				else {
					// 前方移動
					obj_work->prev_pos.x += 8*FX32_ONE; 
					obj_work->move.x	= -8*FX32_ONE;
					obj_work->spd.x		= -8*FX32_ONE;
				}
			}
			// 地面方向地形判定
			g_obj.ppCollision(obj_work);

			if (obj_work->move_flag & OBD_MOVE_BACK) {
				if (eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
					obj_work->prev_pos.y = temp_pos.y;		// 縦方向座標復旧
				}
				else {
					obj_work->prev_pos.x = temp_pos.x;		// 横方向座標復旧
				}
			}
			else {
				obj_work->pos = temp_pos;
			}

			// 終端位置初期化
			if (eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) {
				gear_work->move_end_x = gear_work->move_end_y = 0;
			}
			else {
				gear_work->move_end_x = gear_work->move_end_y = 0x7FFFFFFF;
			}

			// 不要ステータスクリア
			obj_work->prev_pos = obj_work->pos;
			obj_work->move.x = obj_work->move.y = 0;
			obj_work->spd.x = obj_work->spd.y = 0;
			obj_work->disp_flag &= ~OBD_DISP_HFLIP;

			// 強制接地
			obj_work->move_flag |= OBD_MOVE_UNDER; 

			// 移動歯車シーケンスへ
			gmGmkMoveGearFwInit(obj_work);
		}
		else {
			// ローカル生成 (移動歯車終端が生成)

			// 角度をスイッチ歯車にあうように変更
			// 行列設定
			nnMakeRotateZMatrix(&obj_work->obj_3d->user_obj_mtx_r,
						0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360/2);

			// 終了状態に
			gmGmkMoveGearEndInit(obj_work);
		}
	}
	else {
		// 描画処理変更
		obj_work->ppOut = gmGmkGearMoveSwDraw;
		// スイッチ歯車
		// 地形OFF
		//col_work->obj_col.obj = NULL;

		//// 地形情報設定
		col_work->obj_col.diff_data	= (s8*)gm_gmk_gear_add_data[IDB_GMK_GEAR_GMK_GEAR_C_DF];
		col_work->obj_col.dir_data	= (u8*)gm_gmk_gear_add_data[IDB_GMK_GEAR_GMK_GEAR_C_DI];

		// 矩形設定
		rect_work = &gmk_work->ene_com.rect_work[GMD_ENEMY_RECT_ATK];
		// プレイヤー側あたり扱いで設定
		ObjRectGroupSet(rect_work, GMD_OBJ_RECT_GROUP_PLAYER, GMD_OBJ_RECT_TARGET_GROUPFLAG_ENEMY);
		rect_work->ppDef = NULL;
		rect_work->ppHit = gmGmkGearSwitchAtkHitFunc;
		ObjRectAtkSet(rect_work, GMD_OBJ_RECT_ATK_FLAG_EX_SETTING, GMD_OBJ_RECT_ATK_POWER_DEFAULT);			// 特殊設定あたり
		ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_NOHIT, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		// 仮矩形
		ObjRectWorkSet(rect_work,
							GMD_GMK_GEAR_SW_ATK_RECT_LEFT, GMD_GMK_GEAR_SW_ATK_RECT_TOP,
							GMD_GMK_GEAR_SW_ATK_RECT_RIGHT, GMD_GMK_GEAR_SW_ATK_RECT_BOTTOM);
		rect_work->flag |= OBD_RECT_OUT;


		// フラグ設定
		obj_work->disp_flag |= OBD_DISP_NODIR | OBD_DISP_USERMTX_RIGHT;	// 回転描画無し ユーザー行列反映あり
		obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;		// 移動無し 地形あたり無し

		// 壁展開に必要な回転量
#if 1
		gear_work->open_rot_dist = GMD_GMK_GEAR_SW_DEF_ROTDIR;
		if (eve_rec->height > 0) {
			// 復帰時速度設定
			gear_work->close_rot_spd = GMD_GMK_GEAR_SW_DEF_ROTDIR / (eve_rec->height/*1あたり30フレーム*/ * 30);
		}
#else
		if (eve_rec->height > 0) {
			gear_work->open_rot_dist = 0x10000 * eve_rec->height;
		}
		else {
			gear_work->open_rot_dist = 0x10000 * GMD_GMK_GEAR_SW_DEF_ROT_NUM;
		}
#endif

		// 歯車スイッチ待機シーケンスへ
		gmGmkGearSwFwInit(obj_work);
	}

	return (obj_work);
}

// ==========================================================================
// 移動歯車終端
// ==========================================================================
// ==========================================================================
// GmGmkGearMoveEndInit
/*!
 *	ギミック 移動歯車終端 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *		byte_param[1] スイッチ稼動用移動歯車用終端発動済みチェック用
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkGearMoveEndInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_COM_WORK	*ene_com;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);
	obj_work = GMM_ENEMY_CREATE_RIDE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_GEAR_WORK), "GMK_GEAR_END");
																// 描画等は行わないが、共通のワークを使用したいため
	ene_com = (GMS_ENEMY_COM_WORK*)obj_work;

	if (eve_rec->byte_param[1]) {
		OBS_OBJECT_WORK	*parent_obj;

		// スイッチが発動しているので移動歯車を生成
		parent_obj = GmEventMgrLocalEventBirth(GMD_EVENT_ID_GMK_MOVE_GEAR,
						pos_x, pos_y, 0/*flag*/,
						0/*left*/, 0/*top*/, 0/*width*/, 0/*height*/, 1/*type*/);

		// 移動歯車を親にして、親死亡時破棄に変更
		obj_work->parent_obj = parent_obj;

		// 矩形あたり無し クリッピング無し
		obj_work->flag |= OBD_OBJECT_NOHIT | OBD_OBJECT_NOCLIP;
	}
	else {
		// ppIn差し替え
		obj_work->ppIn = gmGmkGearInFunc;

		// 矩形設定
		rect_work = &ene_com->rect_work[GMD_ENEMY_RECT_ATK];
		// プレイヤー側あたり扱いで設定
		ObjRectGroupSet(rect_work, GMD_OBJ_RECT_GROUP_PLAYER, GMD_OBJ_RECT_TARGET_GROUPFLAG_ENEMY);
		rect_work->ppDef = NULL;
		rect_work->ppHit = gmGmkMoveGearEndAtkHitFunc;
		ObjRectAtkSet(rect_work, GMD_OBJ_RECT_ATK_FLAG_EX_SETTING, GMD_OBJ_RECT_ATK_POWER_DEFAULT);			// 特殊設定あたり
		ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_NOHIT, GMD_OBJ_RECT_DEF_POWER_DEFAULT);
		// 仮矩形
		ObjRectWorkSet(rect_work,
							GMD_GMK_MOVE_GEAR_END_ATK_RECT_LEFT, GMD_GMK_MOVE_GEAR_END_ATK_RECT_TOP,
							GMD_GMK_MOVE_GEAR_END_ATK_RECT_RIGHT, GMD_GMK_MOVE_GEAR_END_ATK_RECT_BOTTOM);
		rect_work->flag |= OBD_RECT_OUT;

		// 処理初期化
		gmGmkMoveGearEndSwitchFwInit(obj_work);
	}

	/* フラグ設定 */
	// 共通
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// 移動無し 地形あたり無し
	obj_work->disp_flag |= OBD_DISP_NODISP;						// 描画無し

	return (obj_work);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// 共通処理
// ==========================================================================
// ==========================================================================
// gmGmkGearInFunc
/*!
 *	歯車 共通処理 ppIn
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		歯車共通処理 ppIn登録
 */
// ==========================================================================
void gmGmkGearInFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// 汎用処理
	GmEnemyDefaultInFunc(obj_work);

	// 保存オブジェクト終了チェック
	if (gear_work->gear_end_obj) {
		if (gear_work->gear_end_obj->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) {
			gear_work->gear_end_obj = NULL;
		}
	}
	if (gear_work->move_gear_obj) {
		if (gear_work->move_gear_obj->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) {
			gear_work->move_gear_obj = NULL;
		}
	}
	if (gear_work->sw_gear_obj) {
		if (gear_work->sw_gear_obj->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) {
			gear_work->sw_gear_obj = NULL;
		}
	}

	
#if 0
#if defined (MTD_DEBUG)
	// 専用ライト設定
	{
		NNS_RGBA	light_col = {
			1.0f, 1.0f, 1.0f, 1.0f,
		};
		static NNS_VECTOR	debug_light_vec = {-1.f, 1.f, -1.f};
		NNS_VECTOR	light_vec;

		if (AoPadDirect() & KEY_R1) {
			if (AoPadStand() & KEY_L_LEFT) {
				debug_light_vec.x += 0.05;
			}
			else if (AoPadStand() & KEY_L_RIGHT) {
				debug_light_vec.x -= 0.05;
			}
			else if (AoPadStand() & KEY_L_UP) {
				debug_light_vec.y -= 0.05;
			}
			else if (AoPadStand() & KEY_L_DOWN) {
				debug_light_vec.y += 0.05;
			}
			else if (AoPadStand() & KEY_R_UP) {
				debug_light_vec.z -= 0.05;
			}
			else if (AoPadStand() & KEY_R_LEFT) {
				debug_light_vec.z += 0.05;
			}
		}

		nnNormalizeVector(&light_vec, &debug_light_vec);
		ObjDrawSetParallelLight(NNE_LIGHT_1, &light_col, 1.f, &light_vec);
	}
#endif
#endif
}

// ==========================================================================
// デストラクタ
// ==========================================================================
// ==========================================================================
// gmGmkGearDest
/*!
 *	歯車 デストラクタ
 *
 *	@param tcb	[in] TCBワーク
 */
// ==========================================================================
void gmGmkGearDest(MTS_TASK_TCB *tcb)
{
	GMS_GMK_GEAR_WORK	*gear_work;

	gear_work = (GMS_GMK_GEAR_WORK*)mtTaskGetTcbWork(tcb);

	// サウンドハンドル解放
	if (gear_work->h_snd_gear) {
		GmSoundStopSE(gear_work->h_snd_gear);
		GsSoundFreeSeHandle(gear_work->h_snd_gear);
		gear_work->h_snd_gear = NULL;
	}
	
	// 汎用終了処理
	GmEnemyDefaultExit(tcb);
}


// ==========================================================================
// 通常歯車
// ==========================================================================
// ==========================================================================
// gmGmkGearFwInit
/*!
 *	通常歯車 シーケンス初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkGearFwInit(OBS_OBJECT_WORK *obj_work)
{
//	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	obj_work->ppFunc = gmGmkGearFwMain;
}


// ==========================================================================
// gmGmkGearFwMain
/*!
 *	通常歯車 シーケンスメイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
#if defined (MTD_DEBUG)
static s32 gm_gmk_gear_debug = 0;
static s32 gm_gmk_gear_debug_col_type = 1;
#endif
void gmGmkGearFwMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// 回転
#if defined (MTD_DEBUG)
	if (!gm_gmk_gear_debug) {
#endif
	gear_work->dir_temp += gear_work->dir_speed;
#if defined (MTD_DEBUG)
	}
#endif

	if (gear_work->dir_temp < (float)-0x10000) {
		gear_work->dir_temp += (float)0x10000;
	}
	else if (gear_work->dir_temp > (float)0x10000) {
		gear_work->dir_temp -= (float)0x10000;
	}

	// 描画回転量反映
	gear_work->prev_dir = obj_work->dir.z;
	if (gear_work->dir_speed > 0) {
		obj_work->dir.z = (u16)(((s32)nnRoundOff(gear_work->dir_temp) / GMD_GMK_GEAR_ROT_COL_SPEED) * GMD_GMK_GEAR_ROT_COL_SPEED);
	}
	else {
		obj_work->dir.z = (u16)((((s32)nnRoundOff(gear_work->dir_temp) + (GMD_GMK_GEAR_ROT_COL_SPEED-1)) / GMD_GMK_GEAR_ROT_COL_SPEED) * GMD_GMK_GEAR_ROT_COL_SPEED);
	}

	// 地形設定
	gear_work->col_type = (u32)((obj_work->dir.z / GMD_GMK_GEAR_ROT_COL_SPEED) % GMD_GMK_GEAR_COL_TYPE_NUM);
#if defined (MTD_DEBUG)
	if (gm_gmk_gear_debug) {
		//gear_work->dir_speed = 0;
		gear_work->dir_temp = (float)(gm_gmk_gear_debug_col_type * GMD_GMK_GEAR_ROT_COL_SPEED);
		gear_work->col_type = (u32)(gm_gmk_gear_debug_col_type);
	}
#endif
	gmGmkGearChangeCol(gear_work);

	// 回転した時
	if (gear_work->prev_dir != obj_work->dir.z) {
		// プレイヤーが乗っている場合は移動値設定
		if (obj_work->col_work->obj_col.rider_obj &&
					obj_work->col_work->obj_col.rider_obj->obj_type == GMD_OBJTYPE_PLAYER) {
			OBS_OBJECT_WORK	*ply_obj = obj_work->col_work->obj_col.rider_obj;

			float	dist_x, dist_y;
			float	new_dist_x, new_dist_y;
			Angle32	dir;

			dist_x = FXM_FX32_TO_FLOAT(ply_obj->pos.x - obj_work->pos.x);
			dist_y = -FXM_FX32_TO_FLOAT(ply_obj->pos.y - obj_work->pos.y);

			dir = obj_work->dir.z - gear_work->prev_dir;

			new_dist_x =  nnCos(dir) * dist_x + nnSin(dir) * dist_y;
			new_dist_y = -nnSin(dir) * dist_x + nnCos(dir) * dist_y;

			//ply_obj->flow.x += FXM_FLOAT_TO_FX32(new_dist_x - dist_x);	押し流しだと地形に引っかかってしまう...
			//ply_obj->flow.y += -FXM_FLOAT_TO_FX32(new_dist_y - dist_y);
			ply_obj->prev_pos.x = ply_obj->pos.x;
			ply_obj->prev_pos.y = ply_obj->pos.y;
			ply_obj->pos.x += FXM_FLOAT_TO_FX32(new_dist_x - dist_x);
			ply_obj->pos.y += -FXM_FLOAT_TO_FX32(new_dist_y - dist_y);
			ply_obj->move.x = ply_obj->pos.x - ply_obj->prev_pos.x;
			ply_obj->move.y = ply_obj->pos.y - ply_obj->prev_pos.y;
		}
	}
}

#if 0
// ==========================================================================
// gmGmkGearSetColDef
/*!
 *	通常歯車地形データ設定
 *
 *	@param	gear_work	[in]	ギアワーク
 */
// ==========================================================================
void gmGmkGearSetColDef(GMS_GMK_GEAR_WORK *gear_work)
{
	OBS_COLLISION_WORK	*col_work;

	col_work = &gear_work->gmk_work.ene_com.col_work;

	/* 地形設定 */
	col_work->obj_col.obj		= (OBS_OBJECT_WORK*)gear_work;
	//col_work->obj_col.diff_data	= (s8*)gm_gmk_gear_add_data[0*2];			// 基本地形情報
	//col_work->obj_col.dir_data	= (s8*)gm_gmk_gear_add_data[0*2+1];		// 基本地形情報
	col_work->obj_col.width		= GMD_GMK_GEAR_COL_RECT_WIDTH;		// 地形サイズ設定(ドット)
	col_work->obj_col.height	= GMD_GMK_GEAR_COL_RECT_HEIGHT;
	col_work->obj_col.ofst_x	= GMD_GMK_GEAR_COL_RECT_OFST_X;
	col_work->obj_col.ofst_y	= GMD_GMK_GEAR_COL_RECT_OFST_Y;
	col_work->obj_col.flag		|= OBD_COLOBJ_NOFREE_DIFF_DATA |		// diff_dataを開放しない
								OBD_COLOBJ_NOFREE_DIR_DATA |			// dir_dataを開放しない
								OBD_COLOBJ_NODIR_PARENT;				// 親の角度無視

//void ObjObjectCollisionDifSet ( OBS_OBJECT_WORK* pObj, const char* pPath, OBS_DATA_WORK* pData, void * pArchive )
//void ObjObjectCollisionDirSet ( OBS_OBJECT_WORK* pObj, const char* pPath, OBS_DATA_WORK* pData, void * pArchive )

}
#endif

// ==========================================================================
// gmGmkGearChangeCol
/*!
 *	通常歯車地形データ 変更
 *
 *	@param	gear_work	[in]	ギアワーク
 */
// ==========================================================================
void gmGmkGearChangeCol(GMS_GMK_GEAR_WORK *gear_work)
{
//	OBS_COLLISION_WORK	*col_work;

	MTM_ASSERT(gear_work->col_type < GMD_GMK_GEAR_COL_TYPE_NUM);

//	col_work = &gear_work->gmk_work->ene_com.col_work;
	gear_work->gmk_work.ene_com.col_work.obj_col.diff_data	= (s8*)gm_gmk_gear_add_data[gear_work->col_type*2];			// 基本地形情報
}


// ==========================================================================
// 移動歯車
// ==========================================================================
// ==========================================================================
// gmGmkMoveGearFwInit
/*!
 *	移動歯車 FW初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// くらいHIT ON
	if (!gear_work->rect_ret_timer) {
		gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_NOHIT;
		gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_NOHIT;
	}

	// クリッピング再開
	obj_work->flag &= ~OBD_OBJECT_NOCLIP;

	// 速度クリア
	obj_work->spd_m = 0;
	obj_work->spd.x = obj_work->spd.y = 0;
	obj_work->spd_add.x = obj_work->spd_add.y = 0;

	// 振動クリア
	gear_work->vib_end = FALSE;

	obj_work->ppFunc = gmGmkMoveGearFwMain;
}

// ==========================================================================
// gmGmkMoveGearFwMain
/*!
 *	移動歯車 FWメイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearFwMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// 矩形復帰タイマー
	if (gear_work->rect_ret_timer) {
		gear_work->rect_ret_timer = ObjTimeCountDown(gear_work->rect_ret_timer);
		
		if (!gear_work->rect_ret_timer) {
			// 矩形復帰
			gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_NOHIT;
			gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_NOHIT;
		}
	}
}

// ==========================================================================
// gmGmkMoveGearMoveInit
/*!
 *	移動歯車 移動初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 }
 *	@note
 *		user_flag   : OBD_OBJECT_USER_0 ユーザー強制移動フラグ
 *		user_flag   : OBD_OBJECT_USER_1 スイッチ歯車終点設定
 *		user_flag   : OBD_OBJECT_USER_2 速度アップタイプ
 */
// ==========================================================================
void gmGmkMoveGearMoveInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// ユーザー強制移動解除
	obj_work->user_flag &= ~OBD_OBJECT_USER_0;

	// くらいHIT OFF
	gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_NOHIT;

	// 対終端用 矩形復帰
	gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_NOHIT;

	// クリッピングOFF
	obj_work->flag |= OBD_OBJECT_NOCLIP;

	// 移動開始
	obj_work->move_flag &= ~OBD_MOVE_NOMOVE;

	// 帰り待機時間設定
	gear_work->stop_timer = GMD_GMK_MOVE_GEAR_STOP_RET_TIMER;

	// メイン処理設定
	obj_work->ppFunc = gmGmkMoveGearMoveMain;
}

// ==========================================================================
// gmGmkMoveGearMoveMain
/*!
 *	移動歯車 移動メイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearMoveMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK		*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	GMS_PLAYER_WORK			*ply_work;
//	GMS_EVE_RECORD_EVENT	*eve_rec;

	// 終了チェック
	ply_work = (GMS_PLAYER_WORK*)gear_work->gmk_work.ene_com.target_obj;
	if (!ply_work || ply_work->gmk_obj != obj_work) {
		// 終了
		gear_work->gmk_work.ene_com.target_obj = NULL;

		// 帰り待機シーケンスへ
		gmGmkMoveGearRetWaitInit(obj_work);
		return;
	}

	// 移動・回転
	//gmGmkMoveGearSetSpd(obj_work, -ply_work->obj_work.spd_m << 1);
	gmGmkMoveGearSetSpd(obj_work, -ply_work->obj_work.spd_m);
//	obj_work->spd_m = -ply_work->obj_work.spd_m << 1;
//
//	// 回転
//	gear_work->move_draw_dir += (u16)(obj_work->spd_m >> (FX32_SHIFT - 6));
//	// 行列設定
//	nnMakeRotateZMatrix(&obj_work->obj_3d->user_obj_mtx_r,
//				gear_work->move_draw_dir);


	// 設置方向を重力方向に
	//obj_work->dir_fall = obj_work->dir.z;
//	eve_rec = gear_work->gmk_work.ene_com.eve_rec;
//	if (eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_LEFT_UP) {
//	}
//	else {
//	}

	// 振動設定
	if (obj_work->spd_m && !gear_work->vib_end) {
		// 振動
		GMM_PAD_VIB_SMALL_TIME(GMD_GMK_MOVE_GEAR_VIB_FRAME);
		gear_work->vib_end = TRUE;
	}

	if (!ply_work->obj_work.spd_m) {
		gear_work->stop_timer = ObjTimeCountDown(gear_work->stop_timer);
	}
	else {
		gear_work->stop_timer = GMD_GMK_MOVE_GEAR_STOP_RET_TIMER;
	}

	if (!gear_work->stop_timer) {
		// 歯車戻り開始

		// プレイヤー開放
		gear_work->gmk_work.ene_com.enemy_flag |= GMD_ENEMY_FLAG_USER1;	// 単純開放
		gear_work->gmk_work.ene_com.target_obj = NULL;

		// 歯車戻りシーケンスへ
		gmGmkMoveGearRetInit(obj_work);
	}
}


// ==========================================================================
// gmGmkMoveGearRetWaitInit
/*!
 *	移動歯車 帰り待機初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearRetWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// 矩形復帰タイマー
	gear_work->rect_ret_timer = GMD_GMK_MOVE_GEAR_RECT_RET_TIMER;

	// 矩形HIT ON
	obj_work->flag &= ~OBD_OBJECT_NOHIT;

	obj_work->ppFunc = gmGmkMoveGearRetWaitMain;
}

// ==========================================================================
// gmGmkMoveGearRetWaitMain
/*!
 *	移動歯車 帰り待機メイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearRetWaitMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	if (gear_work->rect_ret_timer) {
		gear_work->rect_ret_timer = ObjTimeCountDown(gear_work->rect_ret_timer);
		
		if (!gear_work->rect_ret_timer) {
			// 矩形復帰
			gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_NOHIT;
			gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_NOHIT;
		}
	}

	// 減速
	gmGmkMoveGearSetSpd(obj_work, ObjSpdDownSet(obj_work->spd_m, GMD_GMK_MOVE_GEAR_SPD_DEC));
	if (!obj_work->spd_m) {
		gear_work->stop_timer = ObjTimeCountDown(gear_work->stop_timer);

		if (!gear_work->stop_timer) {
			// 歯車戻りへ移行
			gmGmkMoveGearRetInit(obj_work);
		}
	}
}

// ==========================================================================
// gmGmkMoveGearRetInit
/*!
 *	移動歯車 帰り初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearRetInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// 矩形が未復帰の場合は、復帰処理設定
	gear_work->rect_ret_timer = 0;
	if (gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag & OBD_RECT_NOHIT) {
		gear_work->rect_ret_timer = GMD_GMK_MOVE_GEAR_RECT_RET_TIMER;
	}

	if ((!(gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) &&
				obj_work->pos.x == gear_work->gmk_work.ene_com.born_pos_x) ||
			((gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) &&
				obj_work->pos.y == gear_work->gmk_work.ene_com.born_pos_y)) {
		// 初期位置にいるので即時FW復帰
		gmGmkMoveGearFwInit(obj_work);
		return;
	}

	// 復帰時最高速度クリア
	gear_work->ret_max_speed = 0;

	obj_work->ppFunc = gmGmkMoveGearRetMain;
}

// ==========================================================================
// gmGmkMoveGearRetMain
/*!
 *	移動歯車 帰りメイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearRetMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK	*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	GMS_ENEMY_COM_WORK	*ene_com = (GMS_ENEMY_COM_WORK*)obj_work;
	BOOL				b_ret_end = FALSE;
	fx32				spd_m = obj_work->spd_m;

	// 矩形復帰タイマー
	if (gear_work->rect_ret_timer) {
		gear_work->rect_ret_timer = ObjTimeCountDown(gear_work->rect_ret_timer);
		
		if (!gear_work->rect_ret_timer) {
			// 矩形復帰
			gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_NOHIT;
			gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag &= ~OBD_RECT_NOHIT;
		}
	}

	if (ene_com->eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) {
		// 目標方向マイナス
		if (ene_com->eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
			// 縦
			if (obj_work->pos.y < ene_com->born_pos_y) {
				// 加速
				spd_m = ObjSpdUpSet(-obj_work->spd_m, -GMD_GMK_MOVE_GEAR_RET_SPD_ACC, GMD_GMK_MOVE_GEAR_RET_SPD_MAX);
			}
			else {
				// 復帰終了
				b_ret_end = TRUE;
			}

			if (obj_work->move_flag & OBD_MOVE_FRONT) {
				// 復帰終了(壁あたり)
				b_ret_end = TRUE;
			}
		}
		else {
			// 横
			if (obj_work->pos.x < ene_com->born_pos_x) {
				// 加速
				spd_m = ObjSpdUpSet(obj_work->spd_m, GMD_GMK_MOVE_GEAR_RET_SPD_ACC, GMD_GMK_MOVE_GEAR_RET_SPD_MAX);
			}
			else {
				// 復帰終了
				b_ret_end = TRUE;
			}

			if (obj_work->move_flag & OBD_MOVE_FRONT) {
				// 復帰終了(壁あたり)
				b_ret_end = TRUE;
			}
		}
	}
	else {
		// 目標方向プラス
		if (ene_com->eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
			// 縦
			if (obj_work->pos.y > ene_com->born_pos_y) {
				// 加速
				spd_m = ObjSpdUpSet(-obj_work->spd_m, GMD_GMK_MOVE_GEAR_RET_SPD_ACC, GMD_GMK_MOVE_GEAR_RET_SPD_MAX);
			}
			else {
				// 復帰終了
				b_ret_end = TRUE;
			}

			if (obj_work->move_flag & OBD_MOVE_BACK) {
				// 復帰終了(壁あたり)
				b_ret_end = TRUE;
			}
		}
		else {
			// 横
			if (obj_work->pos.x > ene_com->born_pos_x) {
				// 加速
				spd_m = ObjSpdUpSet(obj_work->spd_m, -GMD_GMK_MOVE_GEAR_RET_SPD_ACC, GMD_GMK_MOVE_GEAR_RET_SPD_MAX);
			}
			else {
				// 復帰終了
				b_ret_end = TRUE;
			}

			if (obj_work->move_flag & OBD_MOVE_BACK) {
				// 復帰終了(壁あたり)
				b_ret_end = TRUE;
			}
		}
	}

	// 最高速度保存
	if (MTM_MATH_ABS(obj_work->spd_m) > gear_work->ret_max_speed) {
		gear_work->ret_max_speed = MTM_MATH_ABS(obj_work->spd_m);
	}

	if (b_ret_end) {
		// 復帰終了

		// ガコン演出
		if (gear_work->ret_max_speed >= 0x3800) {
			obj_work->vib_timer = 16*FX32_ONE;
		}

		// 速度・回転設定
		gmGmkMoveGearSetSpd(obj_work, 0);

		gmGmkMoveGearFwInit(obj_work);
	}
	else {

		// 速度・回転設定
		gmGmkMoveGearSetSpd(obj_work, spd_m);
	}

}

// ==========================================================================
// gmGmkMoveGearSwitchExeInit
/*!
 *	移動歯車 スイッチ歯車発動初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		user_flag   : OBD_OBJECT_USER_0 ユーザー強制移動フラグ
 *		user_flag   : OBD_OBJECT_USER_1 スイッチ歯車終点設定
 *		user_flag   : OBD_OBJECT_USER_2 速度アップタイプ
 */
// ==========================================================================
void gmGmkMoveGearSwitchExeInit(OBS_OBJECT_WORK *obj_work, s16 cam_ofst_x, s16 cam_ofst_y)
{
	//GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	GMS_ENEMY_COM_WORK	*ene_com = (GMS_ENEMY_COM_WORK*)obj_work;

	// 歯車のユーザー強制移動開始
	obj_work->user_flag |= OBD_OBJECT_USER_0;

	// 矩形HIT OFF
	obj_work->flag |= OBD_OBJECT_NOHIT;

	if (ene_com->target_obj && ene_com->target_obj->obj_type == GMD_OBJTYPE_PLAYER) {
		GMS_PLAYER_WORK	*ply_work = (GMS_PLAYER_WORK*)ene_com->target_obj;

		// カメラ注視点変更
		GmPlayerCameraOffsetSet(ply_work,
				(s16)(ply_work->gmk_camera_center_ofst_x + cam_ofst_x),
				(s16)(ply_work->gmk_camera_center_ofst_y + cam_ofst_y));
	}

	obj_work->ppFunc = gmGmkMoveGearSwitchExeMain;
}

// ==========================================================================
// gmGmkMoveGearSwitchExeMain
/*!
 *	移動歯車 スイッチ歯車発動メイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearSwitchExeMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK	*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	GMS_ENEMY_COM_WORK	*ene_com = (GMS_ENEMY_COM_WORK*)obj_work;
	GMS_PLAYER_WORK		*ply_work;
	GMS_GMK_GEAR_WORK	*sw_gear_work;
	fx32				spd_acc;

	if (gear_work->sw_gear_obj == NULL) {
		// 異常終了
		MTM_ASSERT(0);

		// 動かないようにしておく
		gmGmkMoveGearEndInit(obj_work);
		return;
	}

	sw_gear_work = (GMS_GMK_GEAR_WORK*)gear_work->sw_gear_obj;
	//if (sw_gear_work->gmk_work.ene_com.eve_rec->byte_param[1]) {
	if (sw_gear_work->open_rot_dist <= 0) {
		// 移動歯車によるスイッチ歯車発動終了

		// 速度・回転設定
		gmGmkMoveGearSetSpd(obj_work, 0);

#if 1
		if (sw_gear_work->gmk_work.ene_com.eve_rec->height == 0) {
			// 発動終了済み設定
			ene_com->eve_rec->byte_param[1] = 1;
		}
		gmGmkMoveGearEndStaggerInit(obj_work);
#else
		if (sw_gear_work->gmk_work.ene_com.eve_rec->height == 0) {
			// スイッチの復帰無し
			// 発動終了済み設定
			ene_com->eve_rec->byte_param[1] = 1;

			// 動作終了処理へ移行
			gmGmkMoveGearEndInit(obj_work);
		}
		else {
			// スイッチの復帰あり

			// スイッチ復帰待機へ
			gmGmkMoveGearSwitchRetWaitInit(obj_work);
		}
#endif
		return;
	}

	// 歯車回転最低速度チェック
	ply_work = (GMS_PLAYER_WORK*)gear_work->gmk_work.ene_com.target_obj;
	if (!ply_work || (ply_work->player_flag & GMD_PLF_DIE)) {
		// なくならないはず
		MTM_ASSERT(ply_work);

		// 帰り待機へ
		gear_work->gmk_work.ene_com.target_obj = NULL;
		gmGmkMoveGearRetWaitInit(obj_work);
		return;
	}

	spd_acc = 0;
	if (ene_com->eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V){
		if (!(ene_com->eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) &&
				ply_work->obj_work.spd_m < GMD_GMK_MOVE_GEAR_SW_SPD_MIN) {
			spd_acc = GMD_GMK_MOVE_GEAR_SW_SPD_ACC;
		}
		else if ((ene_com->eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) &&
				ply_work->obj_work.spd_m > -GMD_GMK_MOVE_GEAR_SW_SPD_MIN) {
			spd_acc = -GMD_GMK_MOVE_GEAR_SW_SPD_ACC;
		}
	}
	else {
		if (!(ene_com->eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) &&
				ply_work->obj_work.spd_m > -GMD_GMK_MOVE_GEAR_SW_SPD_MIN) {
			spd_acc = -GMD_GMK_MOVE_GEAR_SW_SPD_ACC;
		}
		else if ((ene_com->eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) &&
				ply_work->obj_work.spd_m < GMD_GMK_MOVE_GEAR_SW_SPD_MIN) {
			spd_acc = GMD_GMK_MOVE_GEAR_SW_SPD_ACC;
		}
	}
	if (spd_acc) {
		ply_work->obj_work.spd_m = ObjSpdUpSet(ply_work->obj_work.spd_m, spd_acc, GMD_GMK_MOVE_GEAR_SW_SPD_MIN);
	}

	// 速度・回転設定
	gmGmkMoveGearSetSpd(obj_work, -ply_work->obj_work.spd_m);
}

// ==========================================================================
// gmGmkMoveGearEndStaggerInit
/*!
 *	移動歯車 移動終了後プレイヤーおっとっと初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		スイッチ歯車発動後、プレイヤーおっとっと演出を行います
 *		user_flag   : OBD_OBJECT_USER_1 スイッチ歯車終点設定
 *		user_flag   : OBD_OBJECT_USER_2 速度アップタイプ
 */
// ==========================================================================
void gmGmkMoveGearEndStaggerInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	GMS_PLAYER_WORK	  *ply_work  = (GMS_PLAYER_WORK*)gear_work->gmk_work.ene_com.target_obj;

	// プレイヤー強制移動OFF
	obj_work->user_flag &= ~OBD_OBJECT_USER_0;

	// スイッチ歯車終点到着
	obj_work->user_flag |= OBD_OBJECT_USER_1;

	// 移動等なし
	obj_work->flag |= OBD_OBJECT_NOHIT;
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

	// カウンタクリア
	gear_work->move_stagger_dir_cnt = 0;

	// おっとっと開始前の回転速度を保持
	gear_work->move_stagger_dir_spd = ply_work->obj_work.spd_m;
	obj_work->user_timer = (s32)(gear_work->move_stagger_dir_spd);		// プレイヤーに状況伝えるため、利便性の高いuser_timerを利用

	// 演出状況進行ステップ初期化
	gear_work->move_stagger_step = 0;
	obj_work->user_work = (u32)(gear_work->move_stagger_step);	// プレイヤーに状況伝えるため、利便性の高いuser_workを利用
	
	obj_work->ppFunc = gmGmkMoveGearEndStaggerMain;
}

// ==========================================================================
// gmGmkMoveGearEndStaggerMain
/*!
 *	移動歯車 移動終了後プレイヤーおっとっとメイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearEndStaggerMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK	*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	GMS_PLAYER_WORK		*ply_work;
	GMS_GMK_GEAR_WORK	*sw_gear_work;
	fx32	add_spd;

#if 0	// ■■■■終点挙動(ishizaki ver)■■■■
	// プレイヤーから解放されたら終了状態へ
	ply_work = (GMS_PLAYER_WORK*)gear_work->gmk_work.ene_com.target_obj;
	if (!ply_work || ply_work->gmk_obj != obj_work) {
		// スイッチ歯車終点到着クリア
		obj_work->user_flag &= ~OBD_OBJECT_USER_1;

		sw_gear_work = (GMS_GMK_GEAR_WORK*)gear_work->sw_gear_obj;
		if (sw_gear_work->gmk_work.ene_com.eve_rec->height == 0) {
			// スイッチの復帰無し
			// 発動終了済み設定
			gear_work->gmk_work.ene_com.eve_rec->byte_param[1] = 1;

			// 動作終了処理へ移行
			gmGmkMoveGearEndInit(obj_work);
		}
		else {
			// スイッチの復帰あり

			// スイッチ復帰待機へ
			gmGmkMoveGearSwitchRetWaitInit(obj_work);
		}
		return;
	}
	// おっとっと用ゆれチェック
#if GMD_GMK_GEAR_STAGGER_BACK
	if (ply_work->act_state == GME_PLY_ACT_STATE_STAGGER_B) {
#else
	if (ply_work->act_state == GME_PLY_ACT_STATE_STAGGER_F) {
#endif
		// ゆれ演出
		s32		dir_cnt;
		//gear_work->move_stagger_dir_cnt = (u16)(gear_work->move_stagger_dir_cnt + 0x10000/GMD_GMK_MOVE_GEAR_STAGGER_FRAME);
		gear_work->move_stagger_dir_cnt++;
		if (gear_work->move_stagger_dir_cnt >= GMD_GMK_MOVE_GEAR_STAGGER_FRAME) {
			gear_work->move_stagger_dir_cnt = 0;
		}
#if GMD_GMK_GEAR_STAGGER_BACK
		dir_cnt = (u16)((gear_work->move_stagger_dir_cnt * 0x10000/GMD_GMK_MOVE_GEAR_STAGGER_FRAME) & 0x7FFF);
#else
		dir_cnt = gear_work->move_stagger_dir_cnt * 0x10000/GMD_GMK_MOVE_GEAR_STAGGER_FRAME;
#endif
		gear_work->move_draw_dir_ofst = (s16)(nnSin(dir_cnt) * (float)GMD_GMK_MOVE_GEAR_STAGGER_DIR_AMP);

		if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
			gear_work->move_draw_dir_ofst = (s16)-gear_work->move_draw_dir_ofst;
		}
	}
	else if (gear_work->move_draw_dir_ofst) {
		// オフセット値戻し
		gear_work->move_draw_dir_ofst = (s16)ObjSpdDownSet(gear_work->move_draw_dir_ofst, 0x080);

		gear_work->move_stagger_dir_cnt = 0;
	}

	// 速度・回転設定
	gmGmkMoveGearSetSpd(obj_work, 0);
#else	// ■■■■終点挙動(kuramoto ver)■■■■
	// プレイヤーから解放されたら終了状態へ
	ply_work = (GMS_PLAYER_WORK*)gear_work->gmk_work.ene_com.target_obj;
	if (!ply_work || ply_work->gmk_obj != obj_work) {
		// スイッチ歯車終点到着クリア
		obj_work->user_flag &= ~OBD_OBJECT_USER_1;

		sw_gear_work = (GMS_GMK_GEAR_WORK*)gear_work->sw_gear_obj;
		if (sw_gear_work->gmk_work.ene_com.eve_rec->height == 0) {
			// スイッチの復帰無し
			// 発動終了済み設定
			gear_work->gmk_work.ene_com.eve_rec->byte_param[1] = 1;

			// 動作終了処理へ移行
			gmGmkMoveGearEndInit(obj_work);
		}
		else {
			// スイッチの復帰あり

			// スイッチ復帰待機へ
			gmGmkMoveGearSwitchRetWaitInit(obj_work);
		}
		return;
	}

	if (gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
		// 縦移動タイプ
		add_spd = (fx32)(-FX32_ONE *3/2 / 64);
	} else {
		// 横移動タイプ
		add_spd = (fx32)( FX32_ONE *3/2 / 64);
	}
	switch(gear_work->move_stagger_step) {
		case 0:
			// 順回転（初回）
			gear_work->move_stagger_dir_spd = gear_work->move_stagger_dir_spd * 9 / 10;
			gear_work->move_draw_dir_ofst += (s16)(gear_work->move_stagger_dir_spd >> (FX32_SHIFT - 7));
			if (MTM_MATH_ABS(gear_work->move_stagger_dir_spd) <= 0x0080) {
				// 速度が一定以下に落ちたら逆回転へ
				gear_work->move_stagger_step++;
				gear_work->move_stagger_dir_spd = 0;
				gear_work->move_stagger_dir_cnt = 0;
				gear_work->move_draw_dir_limit = gear_work->move_draw_dir_ofst;
			}
			break;

		case 1:
			// 逆回転（加速）
			gear_work->move_stagger_dir_spd += add_spd * 3 / 2;
			gear_work->move_draw_dir_ofst += (s16)(gear_work->move_stagger_dir_spd >> (FX32_SHIFT - 7));
//			gear_work->move_stagger_dir_cnt++;

			if (!gear_work->move_stagger_dir_cnt) {
				if (gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
					// 縦移動タイプ
					if (gear_work->move_draw_dir_ofst < (gear_work->move_draw_dir_limit/2) ) {
#if 1
						// 回転量の半分戻ったら少し余剰回転
						gear_work->move_stagger_dir_cnt = 1;
#else
						// 回転量の半分戻ったら減速へ
						gear_work->move_stagger_step++;
#endif
					}
				} else {
					// 横移動タイプ
					if (gear_work->move_draw_dir_ofst > (gear_work->move_draw_dir_limit/2) ) {
#if 1
						// 回転量の半分戻ったら少し余剰回転
						gear_work->move_stagger_dir_cnt = 1;
#else
						// 回転量の半分戻ったら減速へ
						gear_work->move_stagger_step++;
#endif
					}
				}
			} else {
				gear_work->move_stagger_dir_cnt++;
				if (gear_work->move_stagger_dir_cnt == 3) {
					// 回転量の半分+余剰回転終わったら減速へ
					gear_work->move_stagger_step++;
				}
			}
			break;

		case 2:
			// 逆回転（減速）
			gear_work->move_stagger_dir_spd -= add_spd * 3 / 2;
			gear_work->move_draw_dir_ofst += (s16)(gear_work->move_stagger_dir_spd >> (FX32_SHIFT - 7));
#if 1
			if (MTM_MATH_ABS(gear_work->move_stagger_dir_spd) <= 0x0080) {
				// 一定速度以下に減速したら順回転へ
#else
			gear_work->move_stagger_dir_cnt--;
			if (!gear_work->move_stagger_dir_cnt) {
				// 加速に掛かった時間分減速したら終了
#endif
				gear_work->move_stagger_dir_spd = 0;
				if (  (  (gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V)	// 縦移動タイプで
					   &&(GmPlayerKeyCheckWalkRight(ply_work)) )										//   右入力か
					||(  (!(gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V))	// 横移動タイプで
					   &&(GmPlayerKeyCheckWalkLeft(ply_work)) ) ) {										//   左入力なら
					// 次の揺らしに移行
					gear_work->move_stagger_step++;
				} else {
					// 揺り返して停止に移行
					gear_work->move_stagger_step = 5;
				}
			}
			break;

		case 3:
			// 順回転（２回目以降：加速）
			gear_work->move_stagger_dir_spd -= add_spd;
			gear_work->move_draw_dir_ofst += (s16)(gear_work->move_stagger_dir_spd >> (FX32_SHIFT - 7));

			if (MTM_MATH_ABS(gear_work->move_draw_dir_ofst) > (s16)(0x10000 * 20 / 2 / 360)) {	// 20度の半分(10度)回転したら減速へ
				// ２回目以降：減速へ
				gear_work->move_stagger_step++;
			}
			break;

		case 4:
			// 順回転（２回目以降：減速）
			gear_work->move_stagger_dir_spd += add_spd;
			gear_work->move_draw_dir_ofst += (s16)(gear_work->move_stagger_dir_spd >> (FX32_SHIFT - 7));

			if (MTM_MATH_ABS(gear_work->move_stagger_dir_spd) <= 0x0080) {
				gear_work->move_stagger_step = 1;
				gear_work->move_stagger_dir_spd = 0;
				gear_work->move_stagger_dir_cnt = 0;
				gear_work->move_draw_dir_limit = gear_work->move_draw_dir_ofst;
			}
			break;

		case 5:
			// 停止へ（２回目以降：加速）
			gear_work->move_stagger_dir_spd -= add_spd;
			gear_work->move_draw_dir_ofst += (s16)(gear_work->move_stagger_dir_spd >> (FX32_SHIFT - 7));

			if (MTM_MATH_ABS(gear_work->move_draw_dir_ofst) <= (s16)(0x10000 * 1.7f / 360)) {	// 約３度→１.５度まで回転したら減速へ
				// 停止減速へ
				gear_work->move_stagger_step++;
			}
			break;
		
		case 6:
			// 停止へ（２回目以降：減速速）
			gear_work->move_stagger_dir_spd += add_spd;
			gear_work->move_draw_dir_ofst += (s16)(gear_work->move_stagger_dir_spd >> (FX32_SHIFT - 7));

			if (MTM_MATH_ABS(gear_work->move_stagger_dir_spd) <= 0x0080) {
				// 一定速度以下に減速したら順回転へ
				gear_work->move_stagger_dir_spd = 0;
				gear_work->move_draw_dir_ofst   = (s16)(gear_work->move_draw_dir_ofst / 2);	// 誤差なだらかに吸収
				gear_work->move_stagger_step++;
			}
			break;

		case 7:
			// 停止状態
			if (gear_work->move_draw_dir_ofst) {
				gear_work->move_draw_dir_ofst   = (s16)(gear_work->move_draw_dir_ofst / 2);	// 誤差なだらかに吸収
			}
			if (  (  (gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V)	// 縦移動タイプで
				   &&(GmPlayerKeyCheckWalkRight(ply_work)) )										//   右入力か
				||(  (!(gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V))	// 横移動タイプで
				   &&(GmPlayerKeyCheckWalkLeft(ply_work)) ) ) {										//   左入力なら
				// 次の揺らしに移行
				gear_work->move_stagger_step = 3;
				gear_work->move_draw_dir_ofst= 0;
			}
			break;

	}
	obj_work->user_work  = (u32)(gear_work->move_stagger_step);		// プレイヤーに状況伝えるため、利便性の高いuser_workを利用
	obj_work->user_timer = (s32)(gear_work->move_stagger_dir_spd);	// プレイヤーに状況伝えるため、利便性の高いuser_timerを利用
/*
	// おっとっと用ゆれチェック
	if (ply_work->act_state == GME_PLY_ACT_STATE_STAGGER_F) {
		// ゆれ演出
		s32		dir_cnt;
		//gear_work->move_stagger_dir_cnt = (u16)(gear_work->move_stagger_dir_cnt + 0x10000/GMD_GMK_MOVE_GEAR_STAGGER_FRAME);
		gear_work->move_stagger_dir_cnt++;
		if (gear_work->move_stagger_dir_cnt >= GMD_GMK_MOVE_GEAR_STAGGER_FRAME) {
			gear_work->move_stagger_dir_cnt = 0;
		}
		dir_cnt = gear_work->move_stagger_dir_cnt * 0x10000/GMD_GMK_MOVE_GEAR_STAGGER_FRAME;
		gear_work->move_draw_dir_ofst = (s16)(nnSin(dir_cnt) * (float)GMD_GMK_MOVE_GEAR_STAGGER_DIR_AMP);

		if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
			gear_work->move_draw_dir_ofst = (s16)-gear_work->move_draw_dir_ofst;
		}
	}
	else if (gear_work->move_draw_dir_ofst) {
		// オフセット値戻し
		gear_work->move_draw_dir_ofst = (s16)ObjSpdDownSet(gear_work->move_draw_dir_ofst, 0x080);

		gear_work->move_stagger_dir_cnt = 0;
	}
*/
	// 速度・回転設定
	gmGmkMoveGearSetSpd(obj_work, 0);
#endif	// ■■■■終点挙動■■■■
}

// ==========================================================================
// gmGmkMoveGearEndInit
/*!
 *	移動歯車 移動終了初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		以後移動させない場合に呼び出します。
 */
// ==========================================================================
void gmGmkMoveGearEndInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// 移動等なし
	obj_work->flag |= OBD_OBJECT_NOHIT;
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

	// プレイヤー開放
	gear_work->gmk_work.ene_com.enemy_flag |= GMD_ENEMY_FLAG_USER1;	// 単純開放
	gear_work->gmk_work.ene_com.target_obj = NULL;

	obj_work->ppFunc = gmGmkMoveGearEndMain;
}

// ==========================================================================
// gmGmkMoveGearEndMain
/*!
 *	移動歯車 移動終了メイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearEndMain(OBS_OBJECT_WORK *obj_work)
{
	//UNREFERENCED_PARAMETER(obj_work);
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// オフセット値戻し
	if (gear_work->move_draw_dir_ofst) {
		gear_work->move_draw_dir_ofst = (s16)ObjSpdDownSet(gear_work->move_draw_dir_ofst, 0x040);

		// 速度・回転設定
		gmGmkMoveGearSetSpd(obj_work, 0);
	}
}

// ==========================================================================
// gmGmkMoveGearSwitchRetWaitInit
/*!
 *	移動歯車 スイッチ復帰待機初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@note
 *		以後移動させない場合に呼び出します。
 */
// ==========================================================================
void gmGmkMoveGearSwitchRetWaitInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// 移動等なし スイッチ復帰までクリッピングOFF
	obj_work->flag |= OBD_OBJECT_NOHIT | OBD_OBJECT_NOCLIP;
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;

	// プレイヤー開放
	gear_work->gmk_work.ene_com.enemy_flag |= GMD_ENEMY_FLAG_USER1;	// 単純開放
	gear_work->gmk_work.ene_com.target_obj = NULL;

	// プレイヤー存在チェック待機用
	obj_work->user_timer = GMD_GMK_MOVE_GEAR_SW_FREE_WAIT_TIME*FX32_ONE;

	obj_work->ppFunc = gmGmkMoveGearSwitchRetWaitMain;
}

// ==========================================================================
// gmGmkMoveGearSwitchRetWaitMain
/*!
 *	移動歯車 スイッチ復帰待機メイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearSwitchRetWaitMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	if (gear_work->sw_gear_obj == NULL) {
		// 異常終了
		MTM_ASSERT(0);

		// クリッピング再開
		obj_work->flag &= ~OBD_OBJECT_NOCLIP;

		// 動かないようにしておく
		gmGmkMoveGearEndInit(obj_work);
		return;
	}

	// プレイヤーが通常接地状態に戻るまでの待機
	obj_work->user_timer = ObjTimeCountDown(obj_work->user_timer);

	if (!obj_work->col_work->obj_col.rider_obj) {
		if (!obj_work->user_timer) {
			// 乗っているものがなくなった
			// スイッチ復帰開始
			gmGmkMoveGearSwitchRetInit(obj_work);
		}
	}
	else {
		// タイマ復帰
		obj_work->user_timer = GMD_GMK_MOVE_GEAR_SW_FREE_WAIT_TIME*FX32_ONE;
	}

	// オフセット値戻し
	if (gear_work->move_draw_dir_ofst) {
		gear_work->move_draw_dir_ofst = (s16)ObjSpdDownSet(gear_work->move_draw_dir_ofst, 0x040);

		// 速度・回転設定
		gmGmkMoveGearSetSpd(obj_work, 0);
	}
}

// ==========================================================================
// gmGmkMoveGearSwitchRetInit
/*!
 *	移動歯車 スイッチ復帰初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearSwitchRetInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// 不要フラグOFF
	obj_work->flag &= ~OBD_OBJECT_NOHIT;
	obj_work->move_flag &= ~(OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);
	gear_work->gmk_work.ene_com.enemy_flag &= ~(GMD_ENEMY_FLAG_USER1 | GMD_ENEMY_FLAG_USER2 |
										GMD_ENEMY_FLAG_USER3 | GMD_ENEMY_FLAG_USER4);
	obj_work->user_flag &= ~OBD_OBJECT_USER_0;

	// 対歯車スイッチの矩形はOFFに
	gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag |= OBD_RECT_NOHIT;

	// 矩形が未復帰の場合は、復帰処理設定
	gear_work->rect_ret_timer = 0;
	if (gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag & OBD_RECT_NOHIT) {
		gear_work->rect_ret_timer = GMD_GMK_MOVE_GEAR_RECT_RET_TIMER;
	}

	obj_work->ppFunc = gmGmkMoveGearSwitchRetMain;
}

// ==========================================================================
// gmGmkMoveGearSwitchRetMain
/*!
 *	移動歯車 スイッチ復帰メイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearSwitchRetMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK	*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	GMS_GMK_GEAR_WORK	*sw_gear_work;
	s32					close_rot_spd;

	if (gear_work->sw_gear_obj == NULL) {
		// 異常終了
		MTM_ASSERT(0);

		// クリッピング再開
		obj_work->flag &= ~OBD_OBJECT_NOCLIP;

		// 動かないようにしておく
		gmGmkMoveGearEndInit(obj_work);
		return;
	}

	// 矩形復帰タイマー
	if (gear_work->rect_ret_timer) {
		gear_work->rect_ret_timer = ObjTimeCountDown(gear_work->rect_ret_timer);
		
		if (!gear_work->rect_ret_timer) {
			// 矩形復帰
			gear_work->gmk_work.ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag &= ~OBD_RECT_NOHIT;
		}
	}

	sw_gear_work = (GMS_GMK_GEAR_WORK*)gear_work->sw_gear_obj;
	if (sw_gear_work->open_rot_dist >= GMD_GMK_GEAR_SW_DEF_ROTDIR) {
		// 移動歯車によるスイッチ歯車発動終了

		// 速度・回転設定
		gmGmkMoveGearSetSpd(obj_work, 0);

		// クリッピング再開
		//obj_work->flag &= ~OBD_OBJECT_NOCLIP;

		// 歯車戻りシーケンスへ
		gmGmkMoveGearRetInit(obj_work);

		return;
	}

	close_rot_spd = sw_gear_work->close_rot_spd;

	// 回転方向反転
	if (gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
		if (!(gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP)) {
			close_rot_spd = -close_rot_spd;
		}
	}
	else {
		if ((gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP)) {
			close_rot_spd = -close_rot_spd;
		}
	}

	// 速度・回転設定
	gmGmkMoveGearSetSpd(obj_work, -close_rot_spd << (FX32_SHIFT - 7));

	// スイッチが戻るまでは移動しない
	obj_work->spd_m = 0;
}

// ==========================================================================
// gmGmkMoveGearCheckSwitchMove
/*!
 *	移動歯車 スイッチ発動可能チェック
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *	@return	TRUE : 発動可   FALSE : 発動不可
 *
 *	@note
 *		移動歯車が、歯車スイッチを発動できる状態かチェックします。
 */
// ==========================================================================
BOOL gmGmkMoveGearCheckSwitchMove(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work;

	if (obj_work->obj_type != GMD_OBJTYPE_GIMMICK ||
			((GMS_ENEMY_COM_WORK*)obj_work)->eve_rec->id != GMD_EVENT_ID_GMK_MOVE_GEAR) {
		// 移動歯車でない
		MTM_ASSERT(!"gmGmkGear.cpp::gmGmkMoveGearCheckSwitchMove() ERROR event id\n");
		return (FALSE);
	}

	gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	if (gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
		// 縦
		if (gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) {
			// 上移動
			if (obj_work->pos.y <= gear_work->move_end_y) {
				return (TRUE);
			}
		}
		else {
			// 下移動
			if (obj_work->pos.y >= gear_work->move_end_y) {
				return (TRUE);
			}
		}
	}
	else {
		// 横
		if (gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) {
			// 左移動
			if (obj_work->pos.x <= gear_work->move_end_x) {
				return (TRUE);
			}
		}
		else {
			// 右移動
			if (obj_work->pos.x >= gear_work->move_end_x) {
				return (TRUE);
			}
		}
	}

	return (FALSE);
}

// ==========================================================================
// gmGmkMoveGearSetSpd
/*!
 *	移動歯車速度設定
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *	@param	spd_m		[in]	設定速度
 *
 *	@note
 *		速度・回転量を設定します。\n
 *		ギミック未発動のプレイヤーが乗っている場合は、流し速度も設定します。
 */
// ==========================================================================
void gmGmkMoveGearSetSpd(OBS_OBJECT_WORK *obj_work, fx32 spd_m)
{
	GMS_GMK_GEAR_WORK	*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	fx32				dist, end_dist;
	fx32				org_spd_m;

	// 速度退避
	org_spd_m = spd_m;

	// 速度調整 (終点チェック)
	if (gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
		dist = gear_work->gmk_work.ene_com.born_pos_y - obj_work->pos.y;
		end_dist = gear_work->move_end_y - obj_work->pos.y;

		// 縦は逆方向
		spd_m = -spd_m;
	}
	else {
		dist = gear_work->gmk_work.ene_com.born_pos_x - obj_work->pos.x;
		end_dist = gear_work->move_end_x - obj_work->pos.x;
	}
	if (gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) {
		// 目的地 マイナス方向
		if (spd_m < 0) {
			// 進行方向
			if (end_dist > spd_m) {
				spd_m = end_dist;
			}
		}
		else {
			// 戻り方向
			if (dist < spd_m) {
				spd_m = dist;
			}
		}
	}
	else {
		// 目的地 プラス方向
		if (spd_m > 0) {
			// 進行方向
			if (end_dist < spd_m) {
				spd_m = end_dist;
			}
		}
		else {
			// 戻り方向
			if (dist > spd_m) {
				spd_m = dist;
			}
		}
	}

	// 移動
	obj_work->spd_m = spd_m;

	if (spd_m && gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
		// 縦向き移動で速度がある場合は、右側へ重力をかける
		obj_work->spd.x += 0x1000;//FX_Mul(obj_work->spd_fall, g_obj.speed);
	}

	// 回転
	gear_work->old_move_draw_dir = gear_work->move_draw_dir;		// 前回の移動量保存
	gear_work->move_draw_dir_spd = (s16)(-org_spd_m >> (FX32_SHIFT - 7));
	//gear_work->move_draw_dir_spd = (s16)(-org_spd_m / test_div_num);
	//gear_work->move_draw_dir_spd = (s16)(-org_spd_m >> 4);		// (/ 0x10)
	
	gear_work->move_draw_dir += (u16)gear_work->move_draw_dir_spd;
	// 行列設定
	nnMakeRotateZMatrix(&obj_work->obj_3d->user_obj_mtx_r,
				gear_work->move_draw_dir*GMD_GMK_MOVE_GEAR_DIR_SCALE + gear_work->move_draw_dir_ofst);

	// ギミック未発動のプレイヤーが乗っている場合は流す
	if (gear_work->old_move_draw_dir != gear_work->move_draw_dir &&
			((obj_work->col_work->obj_col.rider_obj &&
				obj_work->col_work->obj_col.rider_obj->obj_type == GMD_OBJTYPE_PLAYER &&
				((GMS_PLAYER_WORK*)obj_work->col_work->obj_col.rider_obj)->gmk_obj == NULL) ||
			(obj_work->col_work->obj_col.toucher_obj &&
				obj_work->col_work->obj_col.toucher_obj->obj_type == GMD_OBJTYPE_PLAYER &&
				((GMS_PLAYER_WORK*)obj_work->col_work->obj_col.toucher_obj)->gmk_obj == NULL)) &&
			gear_work->gmk_work.ene_com.target_obj == NULL) {

		GMS_PLAYER_WORK *ply_work = (GMS_PLAYER_WORK*)obj_work->col_work->obj_col.rider_obj;
		if (!ply_work) {
			ply_work = (GMS_PLAYER_WORK*)obj_work->col_work->obj_col.toucher_obj;
		}
//		Angle32			ply_dir, new_dir;
//		fx32			old_pos_x, old_pos_y, new_pos_x, new_pos_y, flow_x;
//		float			old_pos_x_f, old_pos_y_f, r_dist;

		if (!ply_work || ply_work->gmk_obj == obj_work) {
			// プレイヤーが取得できなかった or 発動中
			return;
		}

		// 流す
		gmGmkGearSetRotFlow(obj_work, ply_work, ((s16)gear_work->move_draw_dir - (s16)gear_work->old_move_draw_dir)*GMD_GMK_MOVE_GEAR_DIR_SCALE);
	}
}


// ==========================================================================
// gmGmkMoveGearBodyDefFunc
/*!
 *	ギミック 移動歯車 矩形 くらい処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 *		ppDefに登録
 */
// ==========================================================================
void gmGmkMoveGearBodyDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_PLAYER_WORK			*ply_work = (GMS_PLAYER_WORK*)match_rect->parent_obj;
	GMS_GMK_GEAR_WORK		*gear_work = (GMS_GMK_GEAR_WORK*)mine_rect->parent_obj;
	OBS_OBJECT_WORK			*obj_work = mine_rect->parent_obj;

	if (ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		// プレイヤーでない
		return;
	}

	if (ply_work->gmk_obj == obj_work ||
			!(ply_work->obj_work.move_flag & OBD_MOVE_UNDER) ||
			ply_work->seq_state == GME_PLY_SEQ_STATE_FW ||
			(GME_PLY_SEQ_STATE_LOOKUP_ST <= ply_work->seq_state &&
				ply_work->seq_state <= GME_PLY_SEQ_STATE_SQUAT_END)) {
		// 既に発動中
		// 接地していない
		// FW状態である(ユーザーの意思での接触でない)
		// しゃがみ・見上げ状態である
		return;
	}

	if (!(ply_work->obj_work.pos.x > obj_work->pos.x - GMD_GMK_MOVE_GEAR_FORCE_EXT_DIST &&
				ply_work->obj_work.pos.x < obj_work->pos.x + GMD_GMK_MOVE_GEAR_FORCE_EXT_DIST)) {
		// 強制範囲外
		if (!((ply_work->obj_work.prev_pos.x <= obj_work->pos.x && ply_work->obj_work.pos.x >= obj_work->pos.x) ||
				(ply_work->obj_work.prev_pos.x >= obj_work->pos.x && ply_work->obj_work.pos.x <= obj_work->pos.x))) {
			// 中心をまたいで通過していない
			return;		// 今回発動無し
		}
	}


	// プレイヤーを移動歯車シーケンスへ

	// プレイヤー接着角度
	//gear_work->gmk_work.ene_com.target_dp_dir.x = 0;
	//gear_work->gmk_work.ene_com.target_dp_dir.y = 0;
	//gear_work->gmk_work.ene_com.target_dp_dir.z = 0;

	// プレイヤー接着オフセット座標
	gear_work->gmk_work.ene_com.target_dp_pos.x = 0;
	gear_work->gmk_work.ene_com.target_dp_pos.y = -GMD_GMK_MOVE_GEAR_R*FX32_ONE - ply_work->obj_work.field_rect[MTD_BOTTOM]*FX32_ONE;
	gear_work->gmk_work.ene_com.target_dp_pos.z = ply_work->obj_work.pos.z - obj_work->pos.z;

	// プレイヤー接着距離
	//gear_work->gmk_work.ene_com.target_dp_dist.x = 0;
	//gear_work->gmk_work.ene_com.target_dp_dist.y = 0;
	//gear_work->gmk_work.ene_com.target_dp_dist.z = 0;

	// 終了条件フラグクリア
	gear_work->gmk_work.ene_com.enemy_flag &= ~(GMD_ENEMY_FLAG_USER1 | GMD_ENEMY_FLAG_USER2 |
													GMD_ENEMY_FLAG_USER3 | GMD_ENEMY_FLAG_USER4);

	// プレイヤーギミックシーケンス発動
	GmPlySeqInitMoveGear(ply_work, obj_work,
		gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_CAM_ADJUST_OFF ? FALSE : TRUE);

	// ターゲットプレイヤー保存
	gear_work->gmk_work.ene_com.target_obj = (OBS_OBJECT_WORK*)ply_work;

	// ギミック移動開始
	gmGmkMoveGearMoveInit(obj_work);
}

// ==========================================================================
// gmGmkMoveGearDefFunc
/*!
 *	ギミック 移動歯車 矩形 くらい処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 *		ppDefに登録\n
 *		移動終端を取得
 *		対終端			gmGmkMoveGearEndAtkHitFunc
 *		対スイッチ歯車	gmGmkGearSwitchAtkHitFunc
 */
// ==========================================================================
void gmGmkMoveGearDefFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_GMK_GEAR_WORK		*gear_work = (GMS_GMK_GEAR_WORK*)mine_rect->parent_obj;
	GMS_ENEMY_COM_WORK		*match_ene_com;

	if (match_rect->parent_obj->obj_type != GMD_OBJTYPE_GIMMICK) {
		// ギミックでない
		return;
	}

	match_ene_com = (GMS_ENEMY_COM_WORK*)match_rect->parent_obj;
	if (match_ene_com->eve_rec->id == GMD_EVENT_ID_GMK_MOVE_GEAR_END) {
		// 移動終端を取得する
		gear_work->move_end_x = match_rect->parent_obj->pos.x;
		gear_work->move_end_y = match_rect->parent_obj->pos.y;

		// 歯車スイッチ用終端チェック
		if (match_ene_com->eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_END_SWITCH) {
			// オブジェクト保存
			gear_work->gear_end_obj = match_rect->parent_obj;
		}
	}
	else if (((GMS_ENEMY_COM_WORK*)match_rect->parent_obj)->eve_rec->id == GMD_EVENT_ID_GMK_GEAR_SWITCH) {
		// 歯車スイッチ
		// オブジェクト保存
		gear_work->sw_gear_obj = match_rect->parent_obj;
	}
}


// ==========================================================================
// 移動歯車終端
// ==========================================================================
// ==========================================================================
// gmGmkMoveGearEndSwitchFwInit
/*!
 *	移動歯車終端 スイッチ用 FW初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearEndSwitchFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// スイッチ用の時のみ
	// 監視未終了時
	if ((gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_END_SWITCH) &&
				!gear_work->gmk_work.ene_com.eve_rec->byte_param[1]) {
		obj_work->ppFunc = gmGmkMoveGearEndSwitchFwMain;
	}
}

// ==========================================================================
// gmGmkMoveGearEndSwitchFwMain
/*!
 *	移動歯車終端 スイッチ用 FW
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkMoveGearEndSwitchFwMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK		*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// 移動歯車
	if (gear_work->move_gear_obj) {
		// 移動歯車のスイッチ発動終了を監視する
		GMS_ENEMY_COM_WORK	*move_gear_ene_com = (GMS_ENEMY_COM_WORK*)gear_work->move_gear_obj;

		if (move_gear_ene_com->eve_rec->byte_param[1]) {
			// 次回生成時は、スイッチ起動終了後の移動歯車を生成
			gear_work->gmk_work.ene_com.eve_rec->byte_param[1] = 1;

			// 移動歯車を親にして クリッピングOFF (先に死んで移動歯車)
			obj_work->parent_obj = gear_work->move_gear_obj;
			obj_work->flag |= OBD_OBJECT_NOCLIP;

			// 処理終了
			obj_work->ppFunc = NULL;
		}
	}
}

// ==========================================================================
// gmGmkMoveGearEndAtkHitFunc
/*!
 *	ギミック 移動歯車終端 矩形 あたり処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 *		ppHitに登録 \n
 *		移動歯車の終端を設定する
 *		対移動歯車	gmGmkMoveGearDefFunc
 */
// ==========================================================================
void gmGmkMoveGearEndAtkHitFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_GMK_GEAR_WORK	*gear_work;
	GMS_ENEMY_COM_WORK	*match_ene_com, *ene_com;

	if (match_rect->parent_obj->obj_type != GMD_OBJTYPE_GIMMICK) {
		// ギミックでない
		ObjRectFuncNoHit(mine_rect, match_rect);	// HITキャンセル
		return;
	}

	match_ene_com = (GMS_ENEMY_COM_WORK*)match_rect->parent_obj;
	if (match_ene_com->eve_rec->id != GMD_EVENT_ID_GMK_MOVE_GEAR) {
		// 移動歯車でない
		ObjRectFuncNoHit(mine_rect, match_rect);	// HITキャンセル
		return;
	}

	// 歯車スイッチ用終端チェック
	ene_com = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	if (ene_com->eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_END_SWITCH) {
		// 移動歯車オブジェクト保存
		gear_work = (GMS_GMK_GEAR_WORK*)mine_rect->parent_obj;
		gear_work->move_gear_obj = match_rect->parent_obj;
	}
	// 自分は何もしない
	// オブジェクト破棄(通常破棄 画面切り替えで再生成)

}


// ==========================================================================
// 歯車スイッチ
// ==========================================================================
// ==========================================================================
// gmGmkGearSwFwInit
/*!
 *	歯車スイッチ FW初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkGearSwFwInit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	// 展開回転量初期化
	gear_work->open_rot_dist = GMD_GMK_GEAR_SW_DEF_ROTDIR;

	obj_work->ppFunc = gmGmkGearSwFwMain;

	// 保持移動歯車クリア
	gear_work->move_gear_obj = NULL;
}

// ==========================================================================
// gmGmkGearSwFwMain
/*!
 *	歯車スイッチ FWメイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkGearSwFwMain(OBS_OBJECT_WORK *obj_work)
{
	UNREFERENCED_PARAMETER(obj_work);
}

// ==========================================================================
// gmGmkGearSwRotExtWaitInit
/*!
 *	歯車スイッチ 発動回転待機初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkGearSwRotExtWaitInit(OBS_OBJECT_WORK *obj_work)
{
//	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	if (obj_work->ppFunc == gmGmkGearSwFwMain) {
		// SE
		GmSoundPlaySE("Gear2", NULL);	// 衝突音
	}

	obj_work->ppFunc = gmGmkGearSwRotExtWaitMain;
}

// ==========================================================================
// gmGmkGearSwRotExtWaitMain
/*!
 *	歯車スイッチ 発動回転待機メイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkGearSwRotExtWaitMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK	*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	GMS_GMK_GEAR_WORK	*move_gear_work;
	s32		div_dir_old, div_dir_new;	

	if (gear_work->move_gear_obj == NULL) {
		// 異常終了
		MTM_ASSERT(0);

		// FW状態に戻しておく
		gmGmkGearSwFwInit(obj_work);
		return;
	}

	move_gear_work = (GMS_GMK_GEAR_WORK*)gear_work->move_gear_obj;

	// 実発動開始チェック
//	div_dir_old = (u16)((move_gear_work->old_move_draw_dir - (0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360/2))
//								/ (0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360)*GMD_GMK_MOVE_GEAR_DIR_SCALE);		// 歯車半分ずらしてチェック
	div_dir_old = (u16)((gear_work->move_draw_dir - (0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360))				// ＳＷ歯車側回転角度
								/ (0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360)*GMD_GMK_MOVE_GEAR_DIR_SCALE);		//   歯車半分ずらしてチェック
	div_dir_new = (u16)((move_gear_work->move_draw_dir - (0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360/2))			// 移動歯車側回転角度
								/ (0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360)*GMD_GMK_MOVE_GEAR_DIR_SCALE);

	// ギアがかみ合ったら回転開始
	if (div_dir_old != div_dir_new) {
		// 回転ベース設定
		//gear_work->gear_sw_dir_base = (move_gear_work->move_draw_dir / (0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360)) *
		//									(0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360);

		// 初期回転量設定
		gear_work->move_draw_dir = (u16)-(move_gear_work->move_draw_dir -
												(((0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360/2) -
												div_dir_new*(0x10000*GMD_GMK_MOVE_GEAR_ONE_DIR/360))/GMD_GMK_MOVE_GEAR_DIR_SCALE));

		gmGmkGearSwRotExtInit(obj_work);
	}
}

// ==========================================================================
// gmGmkGearSwRotExtInit
/*!
 *	歯車スイッチ 発動回転初期化
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkGearSwRotExtInit(OBS_OBJECT_WORK *obj_work)
{
//	GMS_GMK_GEAR_WORK *gear_work = (GMS_GMK_GEAR_WORK*)obj_work;

	obj_work->ppFunc = gmGmkGearSwRotExtMain;
}

// ==========================================================================
// gmGmkGearSwRotExtMain
/*!
 *	歯車スイッチ 発動回転メイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 */
// ==========================================================================
void gmGmkGearSwRotExtMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK	*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	GMS_GMK_GEAR_WORK	*move_gear_work;
	s32					dir;

	if (gear_work->move_gear_obj == NULL) {
		// 異常終了
		MTM_ASSERT(0);

		// クリッピング再開
		obj_work->flag &= ~OBD_OBJECT_NOCLIP;

		// FW状態に戻しておく
		gmGmkGearSwFwInit(obj_work);
		return;
	}

	move_gear_work = (GMS_GMK_GEAR_WORK*)gear_work->move_gear_obj;

	//dir = move_gear_work->move_draw_dir - move_gear_work->old_move_draw_dir;
	dir = move_gear_work->move_draw_dir_spd;

	// 回転
	gear_work->move_draw_dir = (u16)(gear_work->move_draw_dir - dir);
	// 行列設定
#if 0	// ■■■■終点挙動(ishizaki ver)■■■■
	nnMakeRotateZMatrix(&obj_work->obj_3d->user_obj_mtx_r,
				gear_work->move_draw_dir*GMD_GMK_MOVE_GEAR_DIR_SCALE);
#else	// ■■■■終点挙動(kuramoto ver)■■■■
	gear_work->move_draw_dir_ofst = (s16)(0 - move_gear_work->move_draw_dir_ofst);
	nnMakeRotateZMatrix(&obj_work->obj_3d->user_obj_mtx_r,
				gear_work->move_draw_dir*GMD_GMK_MOVE_GEAR_DIR_SCALE + gear_work->move_draw_dir_ofst);
#endif	// ■■■■終点挙動■■■■

	// プレイヤーが乗っていた時流す
	if (gear_work->gmk_work.ene_com.col_work.obj_col.rider_obj &&
				gear_work->gmk_work.ene_com.col_work.obj_col.rider_obj->obj_type == GMD_OBJTYPE_PLAYER) {
		gmGmkGearSetRotFlow(obj_work,
				(GMS_PLAYER_WORK*)gear_work->gmk_work.ene_com.col_work.obj_col.rider_obj,
				-dir*GMD_GMK_MOVE_GEAR_DIR_SCALE);
	}
	else if (gear_work->gmk_work.ene_com.col_work.obj_col.toucher_obj &&
				gear_work->gmk_work.ene_com.col_work.obj_col.toucher_obj->obj_type == GMD_OBJTYPE_PLAYER) {
		gmGmkGearSetRotFlow(obj_work,
				(GMS_PLAYER_WORK*)gear_work->gmk_work.ene_com.col_work.obj_col.toucher_obj,
				-dir*GMD_GMK_MOVE_GEAR_DIR_SCALE);
	}
	
	// 回転向きを調整
	if (move_gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_V) {
		if (!(move_gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP)) {
			dir = -dir;
		}
	}
	else {
		if (move_gear_work->gmk_work.ene_com.eve_rec->flag & GMD_GMK_EVE_FLAG_MGEAR_MOVE_LEFT_UP) {
			dir = -dir;
		}
	}

	//gear_work->open_rot_dist -= MTM_MATH_ABS(dir);
	gear_work->open_rot_dist += dir;

	// スイッチ設定
	{
		fx32	per;

		per = MTM_MATH_CLIP(gear_work->open_rot_dist, 0, GMD_GMK_GEAR_SW_DEF_ROTDIR);
		if (dir <= 0) {
			GmGmkSwitchSetOnGearSwitch(gear_work->gmk_work.ene_com.eve_rec->width,
					FX_Div(per, GMD_GMK_GEAR_SW_DEF_ROTDIR));
		}
		else {
			GmGmkSwitchSetOffGearSwitch(gear_work->gmk_work.ene_com.eve_rec->width,
					FX_Div(per, GMD_GMK_GEAR_SW_DEF_ROTDIR));
		}
	}

	// 終了チェック
	if (gear_work->open_rot_dist <= 0) {
		// スイッチ発動終了
		if (gear_work->gmk_work.ene_com.eve_rec->height == 0) {
			// スイッチの復帰無し
			// 発動終了済み設定
			gear_work->gmk_work.ene_com.eve_rec->byte_param[1] = 1;

			// 処理終了
			obj_work->ppFunc = NULL;

			// 移動歯車保持クリア
			gear_work->move_gear_obj = NULL;
		}
		else {
			// スイッチの復帰あり
			// スイッチ復帰までクリッピングOFF
			obj_work->flag |= OBD_OBJECT_NOCLIP;
		}
	}
	else if (gear_work->open_rot_dist >= GMD_GMK_GEAR_SW_DEF_ROTDIR) {
		// スイッチ復帰
		// クリッピング再開
		obj_work->flag &= ~OBD_OBJECT_NOCLIP;
		// FWに戻す
		gmGmkGearSwFwInit(obj_work);
	}
}

// ==========================================================================
// gmGmkGearSwitchAtkHitFunc
/*!
 *	ギミック 歯車スイッチ 矩形 あたり処理
 *
 *	@param mine_rect	[in] 自分くらい矩形
 *	@param match_rect	[in] 相手攻撃矩形
 *
 *	@note
 *		ppHitに登録 \n
 *		対移動歯車	gmGmkMoveGearDefFunc
 */
// ==========================================================================
void gmGmkGearSwitchAtkHitFunc(OBS_RECT_WORK *mine_rect, OBS_RECT_WORK *match_rect)
{
	GMS_ENEMY_COM_WORK	*match_ene_com, *mine_ene_com;
	GMS_GMK_GEAR_WORK	*gear_work;

	if (match_rect->parent_obj->obj_type != GMD_OBJTYPE_GIMMICK) {
		// ギミックでない
		ObjRectFuncNoHit(mine_rect, match_rect);	// HITキャンセル
		return;
	}

	match_ene_com = (GMS_ENEMY_COM_WORK*)match_rect->parent_obj;

	if (match_ene_com->eve_rec->id != GMD_EVENT_ID_GMK_MOVE_GEAR) {
		// 移動歯車でない
		ObjRectFuncNoHit(mine_rect, match_rect);	// HITキャンセル
		return;
	}

	if (match_ene_com->target_obj == NULL) {
		// プレイヤーが乗っていない
		ObjRectFuncNoHit(mine_rect, match_rect);	// HITキャンセル
		return;
	}

	// 連動IDチェック
	mine_ene_com = (GMS_ENEMY_COM_WORK*)mine_rect->parent_obj;
	if (match_ene_com->eve_rec->width != mine_ene_com->eve_rec->width) {
		// 連動IDが違う
		return;
	}

	// 発動チェック
	if (gmGmkMoveGearCheckSwitchMove(match_rect->parent_obj) == FALSE) {
		// まだ発動できない
		ObjRectFuncNoHit(mine_rect, match_rect);	// HITキャンセル
		return;
	}

	// 発動可能だった
	gear_work = (GMS_GMK_GEAR_WORK*)mine_rect->parent_obj;

	// 移動歯車保存
	gear_work->move_gear_obj = match_rect->parent_obj;

	// 移動歯車スイッチ回転発動
	gmGmkMoveGearSwitchExeInit(match_rect->parent_obj,
				(s16)(mine_ene_com->eve_rec->left), (s16)(-mine_ene_com->eve_rec->top));	// 歯車スイッチの保持する値でオフセット変更

	// 移動歯車にリンクさせて回転開始
	gmGmkGearSwRotExtWaitInit(mine_rect->parent_obj);
}


// ==========================================================================
// gmGmkGearSetRotFlow
/*!
 *	ギミック 歯車スイッチ プレイヤー流し
 *
 *	@param gear_obj		[in] ギアオブジェクトワーク
 *	@param ply_work		[in] プレイヤーワーク
 *
 *	@note
 *		ppHitに登録 \n
 *		対移動歯車	gmGmkMoveGearDefFunc
 */
// ==========================================================================
void gmGmkGearSetRotFlow(OBS_OBJECT_WORK *gear_obj, GMS_PLAYER_WORK *ply_work, Angle32 move_dir)
{
	Angle32			ply_dir, new_dir;
	fx32			old_pos_x, old_pos_y, new_pos_x, new_pos_y, flow_x;
	float			old_pos_x_f, old_pos_y_f, r_dist;


	old_pos_x = ply_work->obj_work.pos.x - gear_obj->pos.x;
	old_pos_y = ply_work->obj_work.pos.y - gear_obj->pos.y;
	old_pos_x_f = FXM_FX32_TO_FLOAT(old_pos_x);
	old_pos_y_f = FXM_FX32_TO_FLOAT(old_pos_y);
	ply_dir = nnArcTan2(-old_pos_y_f, old_pos_x_f);
	r_dist = nnSqrt(old_pos_x_f*old_pos_x_f + old_pos_y_f*old_pos_y_f);	// 半径
	//ply_dir += gear_work->move_draw_dir - gear_work->old_move_draw_dir;
	//new_dir = ply_dir + (gear_work->move_draw_dir - gear_work->old_move_draw_dir);
	new_dir = ply_dir + move_dir;

	new_pos_x = FXM_FLOAT_TO_FX32(nnCos(new_dir) * r_dist);
	new_pos_y = -FXM_FLOAT_TO_FX32(nnSin(new_dir) * r_dist);

	flow_x = FX_Mul(new_pos_x - old_pos_x, 0x1400);	// 横移動量を少し多めに
	if (MTM_MATH_ABS(flow_x) < 0x1000) {
		if (flow_x > 0 || gear_obj->spd_m > 0) {
			flow_x = 0x1000;
		}
		else {
			flow_x = -0x1000;
		}
	}
//	if (MTM_MATH_ABS(flow_x) < 0x2000) {
//		if (flow_x > 0) {
//			flow_x = 0x2000;
//		}
//		else {
//			flow_x = -0x2000;
//		}
//	}
#if 0
	ply_work->obj_work.flow.x += flow_x;
	ply_work->obj_work.flow.y += new_pos_y - old_pos_y;

#else
	// 状態によっては歯車からはがす
	if (ply_work->obj_work.ride_obj == gear_obj) {
		ply_work->obj_work.flow.x += flow_x;
		ply_work->obj_work.flow.y += new_pos_y - old_pos_y;
	}
	else if (!ply_work->obj_work.ride_obj && ply_work->obj_work.touch_obj == gear_obj) {
		if ( (move_dir > 0 && ply_work->obj_work.spd_m < -0x8000 && (ply_work->obj_work.pos.y > gear_obj->pos.y)) ||	// 下移動時
				(move_dir < 0 && ply_work->obj_work.spd_m > 0x8000 && (ply_work->obj_work.pos.y < gear_obj->pos.y))		// 上移動時
		) {
			// プレイヤー速度が一定値以上で
			// 歯車下移動時はプレイヤーが歯車の下
			// 歯車上移動時はプレイヤーが歯車の上

			// はがれた
			ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;
		}
		else if ( (ply_work->obj_work.move_flag & OBD_MOVE_UNDER) &&
				((move_dir < 0 && ply_work->obj_work.spd_m >= 0 && (ply_work->obj_work.pos.x > gear_obj->pos.x)) ||		// 右移動時
				(move_dir > 0 && ply_work->obj_work.spd_m <= 0 && (ply_work->obj_work.pos.x < gear_obj->pos.x)))		// 左移動時
		) {
			// 歯車右移動時はプレイヤーが歯車の右
			// 歯車左移動時はプレイヤーが歯車の左

			ply_work->obj_work.flow.x += (flow_x + gear_obj->move.x) * 2;
			ply_work->obj_work.flow.y += new_pos_y - old_pos_y;

			// はがれた
			ply_work->obj_work.move_flag &= ~OBD_MOVE_UNDER;
		}
	}
#endif
}


// ==========================================================================
// gmGmkGearMoveSwDraw
/*!
 *	ギミック 移動歯車 歯車スイッチ 描画処理
 *
 *	@param obj_work		[in] オブジェクトワーク
 */
// ==========================================================================
void gmGmkGearMoveSwDraw(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK	*gear_obj = (GMS_GMK_GEAR_WORK*)obj_work;
	VecFx32				pos;

	// 本体
	ObjDrawActionSummary(obj_work);

	// オプション歯車描画
	// 最前面小さい歯車
	pos.x = obj_work->pos.x;
	pos.y = obj_work->pos.y;
	pos.z = obj_work->pos.z + GMD_GMK_GEAR_OPT_OFST_Z;
	memcpy(&gear_obj->obj_3d_gear_opt.user_obj_mtx_r,
			&obj_work->obj_3d->user_obj_mtx_r,
			sizeof(obj_work->obj_3d->user_obj_mtx_r));
	ObjDrawAction3DNN(&gear_obj->obj_3d_gear_opt, &pos, &obj_work->dir, &obj_work->scale, &obj_work->disp_flag);

	// 足場歯車
	//pos.x = obj_work->pos.x;
	//pos.y = obj_work->pos.y;
	pos.z = obj_work->pos.z + GMD_GMK_GEAR_OPT_ASHIBA_OFST_Z;
	memcpy(&gear_obj->obj_3d_gear_opt_ashiba.user_obj_mtx_r,
			&obj_work->obj_3d->user_obj_mtx_r,
			sizeof(obj_work->obj_3d->user_obj_mtx_r));
	ObjDrawAction3DNN(&gear_obj->obj_3d_gear_opt_ashiba, &pos, &obj_work->dir, &obj_work->scale, &obj_work->disp_flag);
}

// ==========================================================================
// サウンド
// ==========================================================================
// ==========================================================================
// gmGmkGearLastFunc
/*!
 *	ギミック 歯車 移動SE設定
 *
 *	@param	h_snd		[in] SE再生中ハンドル
 *	@param	spd			[in] 移動速度
 */
// ==========================================================================
void gmGmkGearLastFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK	*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	OBS_CAMERA	*camera;
	float		volume = 1.0f;
	float		dist_x, dist_y, dist;

	MTM_ASSERT(obj_work);

	if (obj_work->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) {
		return;
	}
	if (!gear_work->h_snd_gear) {
		return;
	}

	// 画面外に出る時の消音対応
	camera = ObjCameraGet(g_obj.glb_camera_id);
	dist_x = (FXM_FX32_TO_FLOAT(gear_work->gmk_work.ene_com.obj_work.pos.x) - camera->disp_pos.x);
	dist_y = (FXM_FX32_TO_FLOAT(gear_work->gmk_work.ene_com.obj_work.pos.y) - (-camera->disp_pos.y));

	if (dist_x < GMD_GMK_GEAR_SE_MAX_DIST && dist_y < GMD_GMK_GEAR_SE_MAX_DIST) {
		dist = dist_x*dist_x + dist_y*dist_y;
		if (dist <= GMD_GMK_GEAR_SE_MIN_DIST*GMD_GMK_GEAR_SE_MIN_DIST) {
			volume = 1.0f;
		}
		else if (dist <= GMD_GMK_GEAR_SE_MAX_DIST*GMD_GMK_GEAR_SE_MAX_DIST) {
			volume = (GMD_GMK_GEAR_SE_MAX_DIST*GMD_GMK_GEAR_SE_MAX_DIST - dist) /
							((GMD_GMK_GEAR_SE_MAX_DIST - GMD_GMK_GEAR_SE_MIN_DIST)*(GMD_GMK_GEAR_SE_MAX_DIST - GMD_GMK_GEAR_SE_MIN_DIST));
			if (volume > 1.0f) {
				volume = 1.0f;
			}
			else if (volume < 0.0f) {
				volume = 0.0f;
			}
		}
		else {
			volume = 0.0f;
		}
	}
	else {
		volume = 0.0f;
	}

	gear_work->h_snd_gear->snd_ctrl_param.volume = volume;
}

// ==========================================================================
// gmGmkGearMoveLastFunc
/*!
 *	ギミック 移動歯車 移動SE設定
 *
 *	@param	h_snd		[in] SE再生中ハンドル
 *	@param	spd			[in] 移動速度
 */
// ==========================================================================
void gmGmkGearMoveLastFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_GEAR_WORK	*gear_work = (GMS_GMK_GEAR_WORK*)obj_work;
	OBS_CAMERA	*camera;
	CriFloat32	ctrl_val = 0.0f;
	fx32		abs_spd_m;
	float		volume = 1.0f;
	float		dist_x, dist_y, dist;

	MTM_ASSERT(obj_work);

	if (obj_work->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) {
		return;
	}
	if (!gear_work->h_snd_gear) {
		return;
	}

	if (gear_work->gmk_work.ene_com.eve_rec->id == GMD_EVENT_ID_GMK_MOVE_GEAR) {
		s16	dir_ofst_work;
		// 移動歯車
		abs_spd_m = MTM_MATH_ABS(gear_work->move_draw_dir_spd);
		dir_ofst_work = (s16)(gear_work->move_draw_dir_ofst >> 3);
		abs_spd_m += MTM_MATH_ABS(dir_ofst_work);
	}
	else {
		// スイッチ歯車
		if (gear_work->move_gear_obj) {
			abs_spd_m = MTM_MATH_ABS(((GMS_GMK_GEAR_WORK*)gear_work->move_gear_obj)->move_draw_dir_spd);
		}
		else {
			abs_spd_m = 0;
		}
	}

	if (abs_spd_m >= GMD_GMK_GEAR_MOVE_SE_MIN_SPD) {
		if (abs_spd_m >= GMD_GMK_GEAR_MOVE_SE_MAX_SPD) {
			ctrl_val = 1.0f;
		}
		else {
			ctrl_val = FXM_FX32_TO_FLOAT(FX_Div(abs_spd_m - GMD_GMK_GEAR_MOVE_SE_MIN_SPD,
										GMD_GMK_GEAR_MOVE_SE_MAX_SPD - GMD_GMK_GEAR_MOVE_SE_MIN_SPD));
			if (ctrl_val > 1.0f) {
				ctrl_val = 1.0f;
			}
		}
	}
	gear_work->h_snd_gear->au_player->SetAisac("Speed", ctrl_val);


	// 画面外に出る時の消音対応
	camera = ObjCameraGet(g_obj.glb_camera_id);
	dist_x = (FXM_FX32_TO_FLOAT(gear_work->gmk_work.ene_com.obj_work.pos.x) - camera->disp_pos.x);
	dist_y = (FXM_FX32_TO_FLOAT(gear_work->gmk_work.ene_com.obj_work.pos.y) - (-camera->disp_pos.y));

	if (dist_x < GMD_GMK_GEAR_MOVE_SE_MAX_DIST && dist_y < GMD_GMK_GEAR_MOVE_SE_MAX_DIST) {
		dist = dist_x*dist_x + dist_y*dist_y;
		if (dist <= GMD_GMK_GEAR_MOVE_SE_MIN_DIST*GMD_GMK_GEAR_MOVE_SE_MIN_DIST) {
			volume = 1.0f;
		}
		else if (dist <= GMD_GMK_GEAR_MOVE_SE_MAX_DIST*GMD_GMK_GEAR_MOVE_SE_MAX_DIST) {
			volume = (GMD_GMK_GEAR_MOVE_SE_MAX_DIST*GMD_GMK_GEAR_MOVE_SE_MAX_DIST - dist) /
							((GMD_GMK_GEAR_MOVE_SE_MAX_DIST - GMD_GMK_GEAR_MOVE_SE_MIN_DIST)*(GMD_GMK_GEAR_MOVE_SE_MAX_DIST - GMD_GMK_GEAR_MOVE_SE_MIN_DIST));
			if (volume > 1.0f) {
				volume = 1.0f;
			}
			else if (volume < 0.0f) {
				volume = 0.0f;
			}
		}
		else {
			volume = 0.0f;
		}
	}
	else {
		volume = 0.0f;
	}

	gear_work->h_snd_gear->snd_ctrl_param.volume = volume;
}
