// ==========================================================================
/*!
  @file gmScore.cpp
  @brief スコア表示関連

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmScore.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "objObject.h"
#include "gmMain.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmComEfct.h"
#include "gmPlayer.h"

#include "gmScore.h"

//----- Definitions ---------------------------------------------------------
#define GMD_SCORE_FIG_MAX			(5)		//!< 最大桁数
#define GMD_SCORE_MAX				(99999)	//!< 最大スコア
#define GMD_SCORE_DEF_SIZE_X		(11)	//!< 標準サイズX
#define GMD_SCORE_DEF_INT_X			(1)		//!< 標準間隔X
#define GMD_SCORE_DEF_OFST_X		(GMD_SCORE_DEF_SIZE_X + GMD_SCORE_DEF_INT_X)		//!< 標準オフセットX
#define GMD_SCORE_DEF_OFST_Y		(-16)	//!< 標準オフセットY
#define GMD_SCORE_DEF_RIZE_DIST		(-32)	//!< 標準上昇距離
#define GMD_SCORE_RIZE_DIST_SCALE	(-8)	//!< 上昇距離 スケール差分
#define GMD_SCORE_RIZE_TIME			(30)	//!< 上昇時間
#define GMD_SCORE_POS_Z				(GMD_OBJ_DEFAULT_POS_Z_N_FRONT)	//!< スコア表示位置
#define GMD_SCORE_DISP_TIME			(45)	//!< 表示時間

#define GMD_SCORE_VIB_TBL_NUM		(8)		//!< 振動テーブル数
#define GMD_SCORE_VIB_TBL_MASK		(0x07)	//!< 振動テーブル数マスク

#define GMD_SCORE_COMBO_VIB_LEVEL_NUM	(5)


typedef struct tag_GMS_SCORE_DISP_WORK {
	OBS_OBJECT_WORK			obj_work;
	GMS_EFFECT_3DES_WORK	*efct_work[GMD_SCORE_FIG_MAX];	//!< 1桁目～MAX桁
//	fx32					ofst_x[GMD_SCORE_FIG_MAX];		//!< オフセット

	GME_SCORE_VIB_LEVEL		vib_level;
	fx32					scale;			//!< スケール
	VecFx32					base_pos;		//!< 基本座標
	fx32					rise_dist;		//!< 上昇距離
	fx32					rise_spd;		//!< 上昇速度
	fx32					rise_dec;		//!< 上昇現速度
	fx32					vib_timer;		//!< 振動タイマー
	fx32					timer;			//!< 生存タイマー

} GMS_SCORE_DISP_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmScoreMainFunc(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
/// 振動テーブル
static fx32	gm_score_vib_tbl[GMD_SCORE_VIB_TBL_NUM][MTD_XY] = {
	{-1*FX32_ONE,  0*FX32_ONE},
	{ 0*FX32_ONE, -1*FX32_ONE},
	{ 1*FX32_ONE,  0*FX32_ONE},
	{ 0*FX32_ONE,  1*FX32_ONE},
	{-1*FX32_ONE,  0*FX32_ONE},
	{ 0*FX32_ONE,  1*FX32_ONE},
	{ 1*FX32_ONE,  0*FX32_ONE},
	{ 0*FX32_ONE, -1*FX32_ONE},
};

/// 振動スケール
static fx32 gm_score_vib_scale_tbl[GME_SCORE_VIB_LEVEL_MAX] = {
	0,
	(fx32)(FX32_ONE * 1.0),
	(fx32)(FX32_ONE * 1.5),
	(fx32)(FX32_ONE * 2.0),
};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmScoreCreateScore
/*!
 *	スコア表示生成
 *
 *	@param	score		[in]	スコア
 *	@param	pos_x		[in]	表示位置X
 *	@param	pos_y		[in]	表示位置Y
 *	@param	scale		[in]	表示スケール
 *	@pamra	vib_level	[in]	振動レベル
 */
// ==========================================================================
void GmScoreCreateScore(s32 score, fx32 pos_x, fx32 pos_y, fx32 scale, GME_SCORE_VIB_LEVEL vib_level)
{
	s32						i, score_temp, score_num;
	OBS_OBJECT_WORK			*obj_work;
	GMS_SCORE_DISP_WORK		*score_work;
	s32						score_dig[GMD_SCORE_FIG_MAX] = {0};
	s32						div[GMD_SCORE_FIG_MAX] = {10000, 1000, 100, 10, 1};
	BOOL					enable;
	fx32					ofst_x, add_ofst_x;

	MTM_ASSERT((u32)vib_level < GME_SCORE_VIB_LEVEL_MAX);

	if (score <= 0) {
		OS_Printf("gmScore::GmScoreCreateScore() Warning! score 0\n");
		return;
	}

	// オブジェクト取得
#if defined (MTD_DEBUG)
	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(GMD_TASK_PRIO_SCORE, GMD_TASK_GROUP_SCORE, GMD_TASK_PAUSELEVEL_DEF, GMD_OBJ_OBJPAUSELEVEL_DEF, sizeof(GMS_SCORE_DISP_WORK), "GM_SCORE");
#else
	obj_work = OBM_OBJECT_TASK_DETAIL_INIT(GMD_TASK_PRIO_SCORE, GMD_TASK_GROUP_SCORE, GMD_TASK_PAUSELEVEL_DEF, GMD_OBJ_OBJPAUSELEVEL_DEF, sizeof(GMS_SCORE_DISP_WORK), NULL);
#endif

	score_work = (GMS_SCORE_DISP_WORK*)obj_work;

	obj_work->pos.x = pos_x;
	obj_work->pos.y = pos_y + GMD_SCORE_DEF_OFST_Y*FX32_ONE;
	obj_work->pos.z = GMD_SCORE_POS_Z;
	obj_work->flag |= OBD_OBJECT_NOCLIP | OBD_OBJECT_NOHIT;
	obj_work->move_flag |= OBD_MOVE_NOCOL;
	obj_work->ppFunc = gmScoreMainFunc;

	score_work->vib_level	= vib_level;
	score_work->base_pos	= obj_work->pos;
	score_work->scale		= scale;
	score_work->rise_dist	= GMD_SCORE_DEF_RIZE_DIST * FX32_ONE +
									GMD_SCORE_RIZE_DIST_SCALE * (scale - FX32_ONE);
	score_work->rise_spd	= score_work->rise_dist * 2 / GMD_SCORE_RIZE_TIME;
	score_work->rise_dec	= -score_work->rise_spd / GMD_SCORE_RIZE_TIME;
	score_work->timer		= GMD_SCORE_DISP_TIME * FX32_ONE;

	if (score > GMD_SCORE_MAX) {
		OS_Printf("gmScore::GmScoreCreateScore() Warning! score over\n");
		score = GMD_SCORE_MAX;
	}

	// 数値取得
	score_temp = score;
	enable = FALSE;
	score_num = 0;	// 有効桁数
	for (i = 0; i < GMD_SCORE_FIG_MAX; i++) {
		score_dig[GMD_SCORE_FIG_MAX - 1 - i/*位置を反転しておく*/] = score_temp / div[i];
		score_temp -= score_dig[GMD_SCORE_FIG_MAX - 1 - i] * div[i];

		if (!enable) {
			if (score_dig[GMD_SCORE_FIG_MAX - 1 - i] == 0) {
				score_dig[GMD_SCORE_FIG_MAX - 1 - i] = -1;
			}
			else {
				enable = TRUE;
				score_num++;
			}
		}
		else {
			score_num++;
		}
	}

	MTM_ASSERT(score_num > 0);

	// エフェクト生成
	// 下位の桁から描画
	//ofst_x = ((score_num * GMD_SCORE_DEF_SIZE_X + (score_num - 1) * GMD_SCORE_DEF_INT_X) * scale) >> 1;
	//add_ofst_x = -GMD_SCORE_DEF_OFST_X * scale;
	ofst_x = (((score_num * GMD_SCORE_DEF_SIZE_X + (score_num - 1) * GMD_SCORE_DEF_INT_X) * FX32_ONE) >> 1)
								- GMD_SCORE_DEF_SIZE_X*FX32_ONE/2;
	add_ofst_x = -GMD_SCORE_DEF_OFST_X * FX32_ONE;
	for (i = 0; i < GMD_SCORE_FIG_MAX && score_dig[i] != -1; i++, ofst_x += add_ofst_x) {
		score_work->efct_work[i] = GmEfctCmnEsCreate(obj_work,
										(GME_EFCT_CMN_IDX)(GME_EFCT_CMN_IDX_SCORE_0 + score_dig[i]));
		score_work->efct_work[i]->efct_com.obj_work.scale.x = 
			score_work->efct_work[i]->efct_com.obj_work.scale.y = 
			score_work->efct_work[i]->efct_com.obj_work.scale.z = scale;
		score_work->efct_work[i]->obj_3des.command_state = OBD_DRAW_CMD_STATE_3DFIX;
		GmComEfctSetDispOffset(score_work->efct_work[i], ofst_x, 0, 0);

		// オフセット保存
		//score_work->ofst_x[i] = ofst_x;
	}
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmScoreMainFunc
/*!
 *	スコア表示メイン
 *
 *	@param	obj_work	[in]	オブジェクトワーク
 *
 *
 */
// ==========================================================================
void gmScoreMainFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_SCORE_DISP_WORK		*score_work;
	s32						tbl_no;

	score_work = (GMS_SCORE_DISP_WORK*)obj_work;

	// 上昇
	score_work->base_pos.y += score_work->rise_spd;
	score_work->rise_spd += score_work->rise_dec;
	if (score_work->rise_spd > 0) {
		score_work->rise_spd = 0;
	}
	obj_work->pos = score_work->base_pos;

	// 振動
	if (score_work->rise_spd) {
		// 上昇中のみ
		score_work->vib_timer = ObjTimeCountUp(score_work->vib_timer);
		tbl_no = (score_work->vib_timer >> FX32_SHIFT) & GMD_SCORE_VIB_TBL_MASK;
		obj_work->pos.x += FX_Mul(gm_score_vib_tbl[tbl_no][MTD_X], gm_score_vib_scale_tbl[score_work->vib_level]);
		obj_work->pos.y += FX_Mul(gm_score_vib_tbl[tbl_no][MTD_Y], gm_score_vib_scale_tbl[score_work->vib_level]);
	}

	score_work->timer = ObjTimeCountDown(score_work->timer);
	if (score_work->timer <= 0) {
		// 終了
		s32	i;
		// エフェクト終了
		for (i = 0; i < GMD_SCORE_FIG_MAX; i++) {
			if (score_work->efct_work[i]) {
				score_work->efct_work[i]->efct_com.obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
			}
		}
		// タスク破棄
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
	}
}
