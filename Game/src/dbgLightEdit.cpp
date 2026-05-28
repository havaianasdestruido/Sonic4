// ==========================================================================
/*!
  @file dbgLightEdit.cpp
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dbgLightEdit.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gsMainSys.h"
#include "objObject.h"
#include "gmMain.h"
#include "gmTask.h"
#include "gmPlayer.h"

#include "dbgLightEdit.h"

#if defined (MTD_DEBUG)

//----- Definitions ---------------------------------------------------------
#define DBGD_LIGHT_EDIT_DISP_BASE_POS_X	(32)
#define DBGD_LIGHT_EDIT_DISP_BASE_POS_Y	(14)

#if _WII
#define DBGD_LIGHT_EDIT_LIGHT_MAX		(NNE_LIGHT_MAX + 3)				//!< 編集ライト最大数
#define DBGD_LIGHT_EDIT_AMBIENT_NO		(DBGD_LIGHT_EDIT_LIGHT_MAX - 3)	//!< アンビエントNO
#define DBGD_LIGHT_EDIT_TOON_NO			(DBGD_LIGHT_EDIT_LIGHT_MAX - 2)	//!< トゥーンNO
#define DBGD_LIGHT_EDIT_SUPER_TOON_NO	(DBGD_LIGHT_EDIT_LIGHT_MAX - 1)	//!< スーパーソニックトゥーンNO
#else
#define DBGD_LIGHT_EDIT_LIGHT_MAX		(NNE_LIGHT_MAX + 1)				//!< 編集ライト最大数
#define DBGD_LIGHT_EDIT_AMBIENT_NO		(DBGD_LIGHT_EDIT_LIGHT_MAX - 1)	//!< アンビエントNO
#endif


typedef struct tag_DBGS_LIGHT_EDIT_STATE {
	NNS_VECTOR		light_vec;
	NNS_RGBA		light_col;
	float			intensity;
	NNF_LIGHTTYPE	light_type;
	BOOL			enable;
	char			*light_name;
} DBGS_LIGHT_EDIT_STATE;

typedef struct tag_DBGS_LIGHT_EDIT_STATE_WORK {
	NNS_RGB					ambient_color;

#if _WII
	NNS_VECTOR				toon_light;
	NNS_VECTOR				toon_light_super;
#endif

	DBGS_LIGHT_EDIT_STATE	light_state[NNE_LIGHT_MAX];
} DBGS_LIGHT_EDIT_STATE_WORK;

typedef struct tag_DBGS_LIGHT_EDIT_WORK {
	s32				light_no;	//!< 選択中のライト
	s32				item_no;	//!< 編集中アイテムNO
} DBGS_LIGHT_EDIT_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void dbgLightEditDestFunc(MTS_TASK_TCB *tcb);
static void dbgLightEditMainFunc(MTS_TASK_TCB *tcb);
static void dbgLightEditItemEdit(DBGS_LIGHT_EDIT_WORK *edit_work);
static void dbgLightEditDisp(DBGS_LIGHT_EDIT_WORK *edit_work);
static void dbgLightEditSetLight(void);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static DBGS_LIGHT_EDIT_STATE_WORK	dbg_light_edit_work = {{0}};

/// 初期化設定
char *dbg_light_edit_init_name[GSD_MAIN_STAGE_ID_MAX][NNE_LIGHT_MAX] = {
	// z1-1
	{"DEF LIGHT", "BREAK LAND R", "BREAK LAND L", NULL, NULL, "MAP", "PLAYER", NULL},
	// z1-2
	{"DEF LIGHT", "BREAK LAND R", "BREAK LAND L", NULL, NULL, "MAP", "PLAYER", NULL},
	// z1-3
	{"DEF LIGHT", "BREAK LAND R", "BREAK LAND L", NULL, NULL, "MAP", "PLAYER", NULL},
	// z1-b
	{"DEF LIGHT", "BREAK LAND R", "BREAK LAND L", NULL, NULL, "MAP", "PLAYER", NULL},

	// z2-1
	{"DEF LIGHT", NULL, NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// z2-2
	{"DEF LIGHT", NULL, NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// z2-3
	{"DEF LIGHT", NULL, NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// z2-b
	{"DEF LIGHT", NULL, NULL, NULL, NULL, NULL, "PLAYER", NULL},

	// z3-1
	{"DEF LIGHT", NULL, NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// z3-2
	{"DEF LIGHT", NULL, NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// z3-3
	{"DEF LIGHT", NULL, NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// z3-b
	{"DEF LIGHT", NULL, NULL, NULL, NULL, NULL, "PLAYER", NULL},

#if _WII
	// z4-1
	{"DEF LIGHT", "GEAR", "NEEDLE", NULL, "MAP_EX", "MAP", "PLAYER", NULL},
	// z4-2
	{"DEF LIGHT", "GEAR", "NEEDLE", NULL, "MAP_EX", "MAP", "PLAYER", NULL},
	// z4-3
	{"DEF LIGHT", "GEAR", "NEEDLE", NULL, "MAP_EX", "MAP", "PLAYER", NULL},
	// z4-b
	{"DEF LIGHT", "GEAR", "NEEDLE", NULL, "MAP_EX", "MAP", "PLAYER", NULL},
#else
	// z4-1
	{"DEF LIGHT", "GEAR", "NEEDLE", NULL, NULL, "MAP", "PLAYER", NULL},
	// z4-2
	{"DEF LIGHT", "GEAR", "NEEDLE", NULL, NULL, "MAP", "PLAYER", NULL},
	// z4-3
	{"DEF LIGHT", "GEAR", "NEEDLE", NULL, NULL, "MAP", "PLAYER", NULL},
	// z4-b
	{"DEF LIGHT", "GEAR", "NEEDLE", NULL, NULL, "MAP", "PLAYER", NULL},
#endif

	// z5-1
	{"DEF LIGHT", "BOSS GROUND", "HATCH", NULL, NULL, NULL, "PLAYER", NULL},
	// z5-2 Dummy
	{"DEF LIGHT", "BOSS GROUND", "HATCH", NULL, NULL, NULL, "PLAYER", NULL},
	// z5-3 Dummy
	{"DEF LIGHT", "BOSS GROUND", "HATCH", NULL, NULL, NULL, "PLAYER", NULL},
	// z5-4 Dummy
	{"DEF LIGHT", "BOSS GROUND", "HATCH", NULL, NULL, NULL, "PLAYER", NULL},
	// z5-5 Dummy
	{"DEF LIGHT", "BOSS GROUND", "HATCH", NULL, NULL, NULL, "PLAYER", NULL},

	// ss1
	{"DEF LIGHT", "SS", NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// ss2
	{"DEF LIGHT", "SS", NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// ss3
	{"DEF LIGHT", "SS", NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// ss4
	{"DEF LIGHT", "SS", NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// ss5
	{"DEF LIGHT", "SS", NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// ss6
	{"DEF LIGHT", "SS", NULL, NULL, NULL, NULL, "PLAYER", NULL},
	// ss7
	{"DEF LIGHT", "SS", NULL, NULL, NULL, NULL, "PLAYER", NULL},
};
char *dbg_light_edit_blank_name = "";
#if _WII
char *dbg_light_edit_init_name_wii_spec = "WII SPEC";
#endif

static MTS_TASK_TCB *dbg_light_edit_tcb = NULL;		//!< メイン処理TCB


//----- Global Functions ----------------------------------------------------
// ==========================================================================
// DbgLightEditStateInit
/*!
 *	デバックライトエディット ステータス初期化
 *
 */
// ==========================================================================
void DbgLightEditStateInit(void)
{
	s32		i;
	float	base;

	amZeroMemory(&dbg_light_edit_work, sizeof(dbg_light_edit_work));

	/* 現在設定されているライトを取得 */
	for (i = 0; i < NNE_LIGHT_MAX - 1; i++) {	// 最後のライトを除く
		if (dbg_light_edit_init_name[g_gs_main_sys_info.stage_id][i]) {
			dbg_light_edit_work.light_state[i].enable = TRUE;
			dbg_light_edit_work.light_state[i].light_type	= g_obj.light[i].light_type;
			// Wii以外は平行光源しか使っていない
			dbg_light_edit_work.light_state[i].light_vec	= g_obj.light[i].parallel.Direction;
			dbg_light_edit_work.light_state[i].light_col	= g_obj.light[i].parallel.Color;
			dbg_light_edit_work.light_state[i].intensity	= g_obj.light[i].parallel.Intensity;

			dbg_light_edit_work.light_state[i].light_name = dbg_light_edit_init_name[g_gs_main_sys_info.stage_id][i];

		//	length = nnLengthVector(&dbg_light_edit_work.light_state[i].light_vec);
		//	dbg_light_edit_work.light_state[i].light_vec.x = length / dbg_light_edit_work.light_state[i].light_vec.x;
		//	dbg_light_edit_work.light_state[i].light_vec.y = length / dbg_light_edit_work.light_state[i].light_vec.y;
		//	dbg_light_edit_work.light_state[i].light_vec.z = length / dbg_light_edit_work.light_state[i].light_vec.z;

			if (dbg_light_edit_work.light_state[i].light_vec.x != 0.f) {
				base = dbg_light_edit_work.light_state[i].light_vec.x;
			}
			else if (dbg_light_edit_work.light_state[i].light_vec.y != 0.f) {
				base = dbg_light_edit_work.light_state[i].light_vec.y;
			}
			else if (dbg_light_edit_work.light_state[i].light_vec.z != 0.f) {
				base = dbg_light_edit_work.light_state[i].light_vec.z;
			}
			else {
				base = 1.f;
			}
			if (base < 0.f) {
				base *= -1.f;
			}
			dbg_light_edit_work.light_state[i].light_vec.x /= base;
			dbg_light_edit_work.light_state[i].light_vec.y /= base;
			dbg_light_edit_work.light_state[i].light_vec.z /= base;
		}
		else {
			dbg_light_edit_work.light_state[i].light_name = dbg_light_edit_blank_name;
		}
	}

#if _WII
	// Wiiスペキュラライト
	dbg_light_edit_work.light_state[NNE_LIGHT_7].enable = TRUE;
	dbg_light_edit_work.light_state[NNE_LIGHT_7].light_type	= g_obj.light[NNE_LIGHT_7].light_type;
	dbg_light_edit_work.light_state[NNE_LIGHT_7].light_vec	= g_obj.light[NNE_LIGHT_7].specular_gc.Direction;
	dbg_light_edit_work.light_state[NNE_LIGHT_7].light_col	= g_obj.light[NNE_LIGHT_7].specular_gc.Color;

	dbg_light_edit_work.light_state[NNE_LIGHT_7].light_name = dbg_light_edit_init_name_wii_spec;

	if (dbg_light_edit_work.light_state[NNE_LIGHT_7].light_vec.x != 0.f) {
		base = dbg_light_edit_work.light_state[NNE_LIGHT_7].light_vec.x;
	}
	else if (dbg_light_edit_work.light_state[NNE_LIGHT_7].light_vec.y != 0.f) {
		base = dbg_light_edit_work.light_state[NNE_LIGHT_7].light_vec.y;
	}
	else if (dbg_light_edit_work.light_state[NNE_LIGHT_7].light_vec.z != 0.f) {
		base = dbg_light_edit_work.light_state[NNE_LIGHT_7].light_vec.z;
	}
	else {
		base = 1.f;
	}
	if (base < 0.f) {
		base *= -1.f;
	}
	dbg_light_edit_work.light_state[NNE_LIGHT_7].light_vec.x /= base;
	dbg_light_edit_work.light_state[NNE_LIGHT_7].light_vec.y /= base;
	dbg_light_edit_work.light_state[NNE_LIGHT_7].light_vec.z /= base;

	// トゥーンライト
	dbg_light_edit_work.toon_light = g_obj.toon_light_vec;

	if (dbg_light_edit_work.toon_light.x != 0.f) {
		base = dbg_light_edit_work.toon_light.x;
	}
	else if (dbg_light_edit_work.toon_light.y != 0.f) {
		base = dbg_light_edit_work.toon_light.y;
	}
	else if (dbg_light_edit_work.toon_light.z != 0.f) {
		base = dbg_light_edit_work.toon_light.z;
	}
	else {
		base = 1.f;
	}
	if (base < 0.f) {
		base *= -1.f;
	}
	dbg_light_edit_work.toon_light.x /= base;
	dbg_light_edit_work.toon_light.y /= base;
	dbg_light_edit_work.toon_light.z /= base;

	// トゥーンライト スーパーソニック
	dbg_light_edit_work.toon_light_super = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_3d_work[GMD_PLY_MODEL_SET_SUPER*GMD_PLY_MODEL_TYPE_MAX+GMD_PLY_MODEL_TYPE_NORMAL].toon_light;

	if (dbg_light_edit_work.toon_light_super.x != 0.f) {
		base = dbg_light_edit_work.toon_light_super.x;
	}
	else if (dbg_light_edit_work.toon_light_super.y != 0.f) {
		base = dbg_light_edit_work.toon_light_super.y;
	}
	else if (dbg_light_edit_work.toon_light_super.z != 0.f) {
		base = dbg_light_edit_work.toon_light_super.z;
	}
	else {
		base = 1.f;
	}
	if (base < 0.f) {
		base *= -1.f;
	}
	dbg_light_edit_work.toon_light_super.x /= base;
	dbg_light_edit_work.toon_light_super.y /= base;
	dbg_light_edit_work.toon_light_super.z /= base;

#endif

	// アンビエントライト
	dbg_light_edit_work.ambient_color = g_obj.ambient_color;
}

// ==========================================================================
// DbgLightEditInit
/*!
 *	デバックライトエディット 初期化
 *
 */
// ==========================================================================
void DbgLightEditInit(void)
{
	DBGS_LIGHT_EDIT_WORK	*edit_work;

	dbg_light_edit_tcb = MTM_TASK_MAKE_TCB(dbgLightEditMainFunc, dbgLightEditDestFunc,
				0/*flag*/, GMD_TASK_PAUSELEVEL_DEF,
				GMD_TASK_PRIO_MAIN_POST + 1, GMD_TASK_GROUP_NO_GAMESYS,
				sizeof(DBGS_LIGHT_EDIT_WORK), "DBG_LIGHT_EDIT");

	edit_work = (DBGS_LIGHT_EDIT_WORK*)mtTaskGetTcbWork(dbg_light_edit_tcb);
	amZeroMemory(edit_work, sizeof(DBGS_LIGHT_EDIT_WORK));

	// ステータス初期化
	DbgLightEditStateInit();
}

// ==========================================================================
// DbgLightEditExit
/*!
 *	デバックライトエディット 終了処理
 *
 */
// ==========================================================================
void DbgLightEditExit(void)
{
	if (dbg_light_edit_tcb) {
		mtTaskClearTcb(dbg_light_edit_tcb);
		dbg_light_edit_tcb = NULL;
	}
}

// ==========================================================================
// DbgLightEditIsEdit
/*!
 *	デバックライトエディット 編集中チェック
 *
 */
// ==========================================================================
BOOL DbgLightEditIsEdit(void)
{
	if (dbg_light_edit_tcb) {
		return (TRUE);
	}
	return (FALSE);
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// dbgLightEditDestFunc
/*!
 *	デバックライトエディット デストラクタ
 *
 */
// ==========================================================================
void dbgLightEditDestFunc(MTS_TASK_TCB *tcb)
{
	if (tcb == dbg_light_edit_tcb) {
		dbg_light_edit_tcb = NULL;
	}
}

// ==========================================================================
// dbgLightEditMainFunc
/*!
 *	デバックライトエディット メイン処理
 *
 */
// ==========================================================================
void dbgLightEditMainFunc(MTS_TASK_TCB *tcb)
{
	DBGS_LIGHT_EDIT_WORK	*edit_work;

	edit_work = (DBGS_LIGHT_EDIT_WORK*)mtTaskGetTcbWork(tcb);

	/* 編集 */
	dbgLightEditItemEdit(edit_work);

	/* 状態表示 */
	dbgLightEditDisp(edit_work);

	/* ライト設定 */
	dbgLightEditSetLight();
}


// ==========================================================================
// dbgLightEditItemEdit
/*!
 *	ライト編集
 *
 *	@param	deit_work	[in]	エディットワーク
 */
// ==========================================================================
void dbgLightEditItemEdit(DBGS_LIGHT_EDIT_WORK *edit_work)
{
	s32		item_max = 1;
	float	*item_param[8] = {NULL};
	float	max_lim = 1.f, min_lim = 0.f;

	// 最大アイテム数取得
	// パラメータアドレス取得
	if (edit_work->light_no < NNE_LIGHT_MAX) {
		// 通常ライト
		if (dbg_light_edit_work.light_state[edit_work->light_no].light_type == NND_LIGHTTYPE_PARALLEL) {
			item_max = 8 + 1;
			item_param[0] = &dbg_light_edit_work.light_state[edit_work->light_no].light_vec.x;
			item_param[1] = &dbg_light_edit_work.light_state[edit_work->light_no].light_vec.y;
			item_param[2] = &dbg_light_edit_work.light_state[edit_work->light_no].light_vec.z;
			item_param[3] = &dbg_light_edit_work.light_state[edit_work->light_no].light_col.r;
			item_param[4] = &dbg_light_edit_work.light_state[edit_work->light_no].light_col.g;
			item_param[5] = &dbg_light_edit_work.light_state[edit_work->light_no].light_col.b;
			item_param[6] = &dbg_light_edit_work.light_state[edit_work->light_no].light_col.a;
			item_param[7] = &dbg_light_edit_work.light_state[edit_work->light_no].intensity;
		}
#if _WII
		else if (dbg_light_edit_work.light_state[edit_work->light_no].light_type == NND_LIGHTTYPE_SPECULAR_GC) {
			item_max = 7 + 1;
			item_param[0] = &dbg_light_edit_work.light_state[edit_work->light_no].light_vec.x;
			item_param[1] = &dbg_light_edit_work.light_state[edit_work->light_no].light_vec.y;
			item_param[2] = &dbg_light_edit_work.light_state[edit_work->light_no].light_vec.z;
			item_param[3] = &dbg_light_edit_work.light_state[edit_work->light_no].light_col.r;
			item_param[4] = &dbg_light_edit_work.light_state[edit_work->light_no].light_col.g;
			item_param[5] = &dbg_light_edit_work.light_state[edit_work->light_no].light_col.b;
			item_param[6] = &dbg_light_edit_work.light_state[edit_work->light_no].light_col.a;
		}
#endif
	}
#if _WII
	else if (edit_work->light_no == DBGD_LIGHT_EDIT_TOON_NO) {
		// トゥーンライト
		item_max = 3 + 1;
		item_param[0] = &dbg_light_edit_work.toon_light.x;
		item_param[1] = &dbg_light_edit_work.toon_light.y;
		item_param[2] = &dbg_light_edit_work.toon_light.z;
	}
	else if (edit_work->light_no == DBGD_LIGHT_EDIT_SUPER_TOON_NO) {
		// スーパーソニックトゥーンライト
		item_max = 3 + 1;
		item_param[0] = &dbg_light_edit_work.toon_light_super.x;
		item_param[1] = &dbg_light_edit_work.toon_light_super.y;
		item_param[2] = &dbg_light_edit_work.toon_light_super.z;
	}
#endif
	else {
		// アンビエントライト
		item_max = 3 + 1;
		item_param[0] = &dbg_light_edit_work.ambient_color.r;
		item_param[1] = &dbg_light_edit_work.ambient_color.g;
		item_param[2] = &dbg_light_edit_work.ambient_color.b;
	}

	// 項目選択
	if (AoPadRepeat() & KEY_L_UP) {
		edit_work->item_no--;
		if (edit_work->item_no < 0) {
			edit_work->item_no = item_max - 1;
		}
	}
	else if (AoPadRepeat() & KEY_L_DOWN) {
		edit_work->item_no++;
		if (edit_work->item_no >= item_max) {
			edit_work->item_no = 0;
		}
	}

	// ライトタイプ選択
	if (edit_work->item_no == 0) {
		if (AoPadRepeat() & KEY_L_LEFT) {
			edit_work->light_no--;
			if (edit_work->light_no < 0) {
				edit_work->light_no = DBGD_LIGHT_EDIT_LIGHT_MAX - 1;
			}
#if _WII
			while (!(DBGD_LIGHT_EDIT_AMBIENT_NO <= edit_work->light_no && edit_work->light_no <= DBGD_LIGHT_EDIT_SUPER_TOON_NO) &&
#else
			while ((edit_work->light_no != DBGD_LIGHT_EDIT_AMBIENT_NO) &&
#endif
					dbg_light_edit_work.light_state[edit_work->light_no].enable == FALSE) {
				// 有効なライトがくるまで検索
				edit_work->light_no--;
				if (edit_work->light_no < 0) {
					edit_work->light_no = DBGD_LIGHT_EDIT_LIGHT_MAX - 1;
				}
			}
			edit_work->item_no = 0;	// アイテムNOリセット
		}
		else if (AoPadRepeat() & KEY_L_RIGHT) {
			edit_work->light_no++;
			if (edit_work->light_no >= DBGD_LIGHT_EDIT_LIGHT_MAX) {
				edit_work->light_no = 0;
			}
#if _WII
			while (!(DBGD_LIGHT_EDIT_AMBIENT_NO <= edit_work->light_no && edit_work->light_no <= DBGD_LIGHT_EDIT_SUPER_TOON_NO) &&
#else
			while ((edit_work->light_no != DBGD_LIGHT_EDIT_AMBIENT_NO) &&
#endif
					dbg_light_edit_work.light_state[edit_work->light_no].enable == FALSE) {
				edit_work->light_no++;
				if (edit_work->light_no >= DBGD_LIGHT_EDIT_LIGHT_MAX) {
					edit_work->light_no = 0;
				}
			}
			edit_work->item_no = 0;	// アイテムNOリセット
		}
	}

	// ステート変更
	else if (edit_work->item_no < item_max && item_param[edit_work->item_no - 1]) {
		// リミット値変更
#if _WII
		if (((edit_work->light_no < NNE_LIGHT_MAX) && ((edit_work->item_no - 1) < 3/*vecの時*/)) ||
				edit_work->light_no == DBGD_LIGHT_EDIT_TOON_NO ||
				edit_work->light_no == DBGD_LIGHT_EDIT_SUPER_TOON_NO) {
#else
		if ((edit_work->light_no < NNE_LIGHT_MAX) && ((edit_work->item_no - 1) < 3/*vecの時*/)) {
#endif
			max_lim = 100.f;
			min_lim = -100.f;
		}

		if (AoPadRepeat() & KEY_L_LEFT) {
			if (AoPadDirect() & KEY_R_UP) {
				*item_param[edit_work->item_no - 1] -= 0.5f;
			}
			else if (AoPadDirect() & KEY_R_DOWN) {
				*item_param[edit_work->item_no - 1] -= 0.01f;
			}
			else {
				*item_param[edit_work->item_no - 1] -= 0.1f;
			}
			if (*item_param[edit_work->item_no - 1] <= min_lim) {
				*item_param[edit_work->item_no - 1] = min_lim;
			}
		}
		else if (AoPadRepeat() & KEY_L_RIGHT) {
			if (AoPadDirect() & KEY_R_UP) {
				*item_param[edit_work->item_no - 1] += 0.5f;
			}
			else if (AoPadDirect() & KEY_R_DOWN) {
				*item_param[edit_work->item_no - 1] += 0.01f;
			}
			else {
				*item_param[edit_work->item_no - 1] += 0.1f;
			}
			if (*item_param[edit_work->item_no - 1] >= max_lim) {
				*item_param[edit_work->item_no - 1] = max_lim;
			}
		}
	}
}


// ==========================================================================
// dbgLightEditDisp
/*!
 *	ライト状態表示
 *
 *	@param	deit_work	[in]	エディットワーク
 */
// ==========================================================================
void dbgLightEditDisp(DBGS_LIGHT_EDIT_WORK *edit_work)
{
	s32		pos_x, pos_y;

	pos_x = DBGD_LIGHT_EDIT_DISP_BASE_POS_X;
	pos_y = DBGD_LIGHT_EDIT_DISP_BASE_POS_Y;

	// 項目表示
	if (edit_work->light_no < NNE_LIGHT_MAX) {
		amPrintf(pos_x, pos_y, "LIGHT : %s", dbg_light_edit_work.light_state[edit_work->light_no].light_name);
	}
#if _WII
	else if (edit_work->light_no == DBGD_LIGHT_EDIT_TOON_NO) {
		// トゥーンライト
		amPrintf(pos_x, pos_y, "TOON");
	}
	else if (edit_work->light_no == DBGD_LIGHT_EDIT_SUPER_TOON_NO) {
		// トゥーンライト スーパーソニック
		amPrintf(pos_x, pos_y, "TOON SUPER SONIC");
	}
#endif
	else {
		// アンビエントライト
		amPrintf(pos_x, pos_y, "AMBIENT");
	}
	pos_y++;

	// ステート表示
	if (edit_work->light_no == DBGD_LIGHT_EDIT_AMBIENT_NO) {
		// アンビエントライト
		// AMB R
		amPrintf(pos_x + 1, pos_y, "AMB R : %f", dbg_light_edit_work.ambient_color.r);
		pos_y++;
		// AMB G
		amPrintf(pos_x + 1, pos_y, "AMB G : %f", dbg_light_edit_work.ambient_color.g);
		pos_y++;
		// AMB B
		amPrintf(pos_x + 1, pos_y, "AMB B : %f", dbg_light_edit_work.ambient_color.b);
		pos_y++;
	}
#if _WII
	else if (edit_work->light_no == DBGD_LIGHT_EDIT_TOON_NO) {
		// トゥーンライト
		// TOON X
		amPrintf(pos_x + 1, pos_y, "TOON X : %f", dbg_light_edit_work.toon_light.x);
		pos_y++;
		// TOON Y
		amPrintf(pos_x + 1, pos_y, "TOON Y : %f", dbg_light_edit_work.toon_light.y);
		pos_y++;
		// TOON Z
		amPrintf(pos_x + 1, pos_y, "TOON Z : %f", dbg_light_edit_work.toon_light.z);
		pos_y++;
	}
	else if (edit_work->light_no == DBGD_LIGHT_EDIT_SUPER_TOON_NO) {
		// トゥーンライト スーパーソニック
		// TOON X
		amPrintf(pos_x + 1, pos_y, "TOON X : %f", dbg_light_edit_work.toon_light_super.x);
		pos_y++;
		// TOON Y
		amPrintf(pos_x + 1, pos_y, "TOON Y : %f", dbg_light_edit_work.toon_light_super.y);
		pos_y++;
		// TOON Z
		amPrintf(pos_x + 1, pos_y, "TOON Z : %f", dbg_light_edit_work.toon_light_super.z);
		pos_y++;
	}
#endif
	else if (dbg_light_edit_work.light_state[edit_work->light_no].light_type == NND_LIGHTTYPE_PARALLEL) {
		// vec X
		amPrintf(pos_x + 1, pos_y, "VEC X : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_vec.x);
		pos_y++;
		// vec Y
		amPrintf(pos_x + 1, pos_y, "VEC Y : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_vec.y);
		pos_y++;
		// vec Z
		amPrintf(pos_x + 1, pos_y, "VEC Z : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_vec.z);
		pos_y++;
		// col R
		amPrintf(pos_x + 1, pos_y, "COL R : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_col.r);
		pos_y++;
		// col G
		amPrintf(pos_x + 1, pos_y, "COL G : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_col.g);
		pos_y++;
		// col B
		amPrintf(pos_x + 1, pos_y, "COL B : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_col.b);
		pos_y++;
		// col A
		amPrintf(pos_x + 1, pos_y, "COL A : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_col.a);
		pos_y++;
		// intensity
		amPrintf(pos_x + 1, pos_y, "INTEN : %f", dbg_light_edit_work.light_state[edit_work->light_no].intensity);
		pos_y++;
	}
#if _WII
	else if (dbg_light_edit_work.light_state[edit_work->light_no].light_type == NND_LIGHTTYPE_SPECULAR_GC) {
		// vec X
		amPrintf(pos_x + 1, pos_y, "VEC X : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_vec.x);
		pos_y++;
		// vec Y
		amPrintf(pos_x + 1, pos_y, "VEC Y : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_vec.y);
		pos_y++;
		// vec Z
		amPrintf(pos_x + 1, pos_y, "VEC Z : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_vec.z);
		pos_y++;
		// col R
		amPrintf(pos_x + 1, pos_y, "COL R : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_col.r);
		pos_y++;
		// col G
		amPrintf(pos_x + 1, pos_y, "COL G : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_col.g);
		pos_y++;
		// col B
		amPrintf(pos_x + 1, pos_y, "COL B : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_col.b);
		pos_y++;
		// col A
		amPrintf(pos_x + 1, pos_y, "COL A : %f", dbg_light_edit_work.light_state[edit_work->light_no].light_col.a);
		pos_y++;
	}
#endif

	// カーソル表示
	amPrintf(pos_x-1, DBGD_LIGHT_EDIT_DISP_BASE_POS_Y + edit_work->item_no, ">");
}

// ==========================================================================
// dbgLightEditSetLight
/*!
 *	ライト設定
 */
// ==========================================================================
void dbgLightEditSetLight(void)
{
	s32						i;
	NNS_VECTOR				normal_vec;

	// アンビエントライト設定
	g_obj.ambient_color = dbg_light_edit_work.ambient_color;

	// 各種ライト設定
	for (i = 0; i < NNE_LIGHT_MAX; i++) {
		if (dbg_light_edit_work.light_state[i].enable == FALSE) {
			continue;
		}
		if (dbg_light_edit_work.light_state[i].light_type == NND_LIGHTTYPE_PARALLEL) {
			nnNormalizeVector(&normal_vec, &dbg_light_edit_work.light_state[i].light_vec);
			ObjDrawSetParallelLight((NNE_LIGHT)i,
							&dbg_light_edit_work.light_state[i].light_col,
							dbg_light_edit_work.light_state[i].intensity,
							&normal_vec);
		}
#if _WII
		else if (dbg_light_edit_work.light_state[i].light_type == NND_LIGHTTYPE_SPECULAR_GC) {
			nnNormalizeVector(&normal_vec, &dbg_light_edit_work.light_state[i].light_vec);
			ObjDrawSetSpecularGCLight((NNE_LIGHT)i,
							&dbg_light_edit_work.light_state[i].light_col,
							&normal_vec);
		}
#endif
	}

#if _WII
	// トゥーンライト
	nnNormalizeVector(&normal_vec, &dbg_light_edit_work.toon_light);
	g_obj.toon_light_vec = normal_vec;
	{
		OBS_OBJECT_WORK	*obj_work;
		obj_work = ObjObjectSearchRegistObject(NULL, 0xFFFF);
		while (obj_work) {
			if (!(obj_work->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST)) &&
					obj_work->obj_3d) {

				obj_work->obj_3d->toon_light = g_obj.toon_light_vec;
			}
			obj_work = ObjObjectSearchRegistObject(obj_work, 0xFFFF);
		}
	}
	// ソニックトゥーンライト
	g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_3d_work[GMD_PLY_MODEL_SET_SUPER*GMD_PLY_MODEL_TYPE_MAX+GMD_PLY_MODEL_TYPE_NORMAL].toon_light = g_obj.toon_light_vec;
	g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_3d_work[GMD_PLY_MODEL_SET_SUPER*GMD_PLY_MODEL_TYPE_MAX+GMD_PLY_MODEL_TYPE_SPIN].toon_light = g_obj.toon_light_vec;

	// スーパーソニックトゥーンライト
	nnNormalizeVector(&normal_vec, &dbg_light_edit_work.toon_light_super);
	g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_3d_work[GMD_PLY_MODEL_SET_SUPER*GMD_PLY_MODEL_TYPE_MAX+GMD_PLY_MODEL_TYPE_NORMAL].toon_light = normal_vec;
	g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->obj_3d_work[GMD_PLY_MODEL_SET_SUPER*GMD_PLY_MODEL_TYPE_MAX+GMD_PLY_MODEL_TYPE_SPIN].toon_light = normal_vec;

#endif
}


#endif // #if defined (MTD_DEBUG)

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
