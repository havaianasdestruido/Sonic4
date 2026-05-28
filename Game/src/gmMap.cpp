// ==========================================================================
/*!
  @file gmMap.cpp
  @brief マップ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmMap.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "gmMain.h"
#include "gmGameDat.h"
#include "gmCamera.h"
#include "objObject.h"
#include "gmObjDef.h"
#include "gmMapFar.h"

#include "map_file_format.h"

#include "gmMap.h"

#include "map_file_format.h"

#if _IPHONE
#include "AoTexture.h"
#include "AoTvxFile.h"

#include "dbgPadEmu.hpp"

// MAP texture
#include "../file/iPhone/model/ZONE2_T.HMB"
#include "../file/iPhone/model/ZONE3_T.HMB"
#include "../file/iPhone/model/ZONE4_T.HMB"
#include "../file/iPhone/model/ZONEF_T.HMB"
#endif //_IPHONE


//----- Definitions ---------------------------------------------------------
#if defined (MTD_DEBUG)
	//#define	GMD_MAP_DEBUG_NO_DRAW
	//#define	GMD_MAP_DEBUG_NO_UPDATE
#endif

#if _IPHONE
#define GMD_MAP_DRAW_STRIP_TEST
#define GMD_MAP_DRAW_INIT_COUNT  (1)

#define GMD_MAP_DRAW_BGM_TIMER   (2)
#endif // _IPHONE

#if _IPHONE
#define GMD_MAP_USE_PRIM_MTX_NUM   (16) // 回転4パターン x フリップ4パターン
#endif // _IPHONE

#define GMD_MAP_OBJECT_NUM		(300)	//!< マップオブジェクト最大数
#define GMD_MAP_OBJECT_START	(1)		//!< マップオブジェクト実有効モデル開始位置

#define GMD_MAP_DRAW_MARGIN		(1)		//!< マップ描画余分
#define GMD_MAP_DRAW_DF_SIZE	(8)		//!< マップ描画ブロックオフセット値

#if _IPHONE
#define GMD_MAP_DRAW_WIDTH_DEF	(6)	//!<	マップ描画幅 基本
#define GMD_MAP_DRAW_HEIGHT_DEF	(6)	//!<	マップ描画高さ 基本

#define GMD_MAP_DRAW_WIDTH		(gm_map_draw_size[MTD_X])	//!<	マップ描画幅
#define GMD_MAP_DRAW_HEIGHT		(gm_map_draw_size[MTD_Y])	//!<	マップ描画高さ

#define GMD_MAP_DRAW_CHECK_WIDTH	(GMD_MAP_DRAW_WIDTH_DEF+GMD_MAP_DRAW_MARGIN*2+GMD_MAP_DRAW_DF_SIZE*2)
#define GMD_MAP_DRAW_CHECK_HEIGHT	(GMD_MAP_DRAW_HEIGHT_DEF+GMD_MAP_DRAW_MARGIN*2+GMD_MAP_DRAW_DF_SIZE*2)
#else
#define GMD_MAP_DRAW_WIDTH		(6)		//!< マップ描画幅
#define GMD_MAP_DRAW_HEIGHT		(6)		//!< マップ描画高さ

#define GMD_MAP_DRAW_CHECK_WIDTH	(GMD_MAP_DRAW_WIDTH+GMD_MAP_DRAW_MARGIN*2+GMD_MAP_DRAW_DF_SIZE*2)
#define GMD_MAP_DRAW_CHECK_HEIGHT	(GMD_MAP_DRAW_HEIGHT+GMD_MAP_DRAW_MARGIN*2+GMD_MAP_DRAW_DF_SIZE*2)
#endif // _IPHONE

//#define GMD_MAP_BLOCK_SIZE		(20.f)	//!< マップブロックデータサイズ
//#define GMD_MAP_BLOCK_DRAW_SIZE	(64.f)	//!< マップブロック描画サイズ

#define GMD_MAP_A_POS_Z			((float)(GMD_OBJ_DEFAULT_POS_Z_A/FX32_ONE))		//!< A面描画時 Z位置
#define GMD_MAP_B_POS_Z			((float)(GMD_OBJ_DEFAULT_POS_Z_B/FX32_ONE))		//!< B面描画時 Z位置
#define GMD_MAP_N_POS_Z			((float)(GMD_OBJ_DEFAULT_POS_Z_N/FX32_ONE))		//!< N面(超近景)描画時 Z位置
#define GMD_MAP_M_POS_Z			((float)(GMD_OBJ_DEFAULT_POS_Z_M/FX32_ONE))		//!< M面(中景)描画時 Z位置
#define GMD_MAP_M1_POS_Z		((float)(GMD_OBJ_DEFAULT_POS_Z_M1/FX32_ONE))	//!< M1面(中景スクロールタイプ)描画時 Z位置
#define GMD_MAP_M2_POS_Z		((float)(GMD_OBJ_DEFAULT_POS_Z_M2/FX32_ONE))	//!< M2面(中景スクロールタイプ)描画時 Z位置
#define GMD_MAP_M3_POS_Z		((float)(GMD_OBJ_DEFAULT_POS_Z_M3/FX32_ONE))	//!< M3面(中景スクロールタイプ)描画時 Z位置

/// 通常近景以外のマップステータス
typedef struct tag_GMS_MAP_OTHER_MAP_STATE {
	s32		map_block_num[MTD_XY];	//!< マップブロックサイズ
	s32		map_size[MTD_XY];		//!< マップサイズ
	float	scrl_scale[MTD_XY];		//!< スクロールスケール
	float	pos_z;					//!< 描画Z位置
	u32		command_state;			//!< 描画コマンドステート

	// ループ系処理用
	float	cam_ofst[MTD_XY];			//!< カメラオフセット
//	float	main_cam_base[MTD_XY];		//!< メインカメラベース	外部よりスクロール値加算の場合
//	float	main_cam_add_scrl[MTD_XY];	//!< メインカメラスクロール加算値 外部よりスクロール値加算の場合
} GMS_MAP_OTHER_MAP_STATE;


typedef struct tag_GMS_MAP_SYS_WORK {
	u32		flag;					//!< GMD_MAP_FLAG_***


	GMS_MAP_OTHER_MAP_STATE	map_state[GME_MAP_ADD_MAP_MAX];	//!< マップスクロール用ステート

	float		main_cam_user_disp[MTD_XY];		// ユーザー指定カメラdisp位置ベース量
	float		main_cam_user_target[MTD_XY];	// ユーザー指定カメラtarget位置ベース量
	float		main_cam_user_ofst[MTD_XY];	// ユーザー指定カメラ位置オフセット量
#if _IPHONE
	BOOL		auto_resize; // マップ検索サイズ自動設定
#endif // _IPHONE
//	NNS_VECTOR	user_cam_pos;			//!< ユーザー指定カメラ位置	外部よりスクロール値加算の場合
//	NNS_VECTOR	user_cam_disp_pos;		//!< ユーザー指定カメラ位置	外部よりスクロール値加算の場合
//	NNS_VECTOR	user_cam_target_pos;	//!< ユーザー指定カメラ位置	外部よりスクロール値加算の場合
//	s32		snear_map_block_num[MTD_XY];	//!< 超近景マップブロックサイズ
//	s32		snear_map_size[MTD_XY];			//!< 超近景マップサイズ
//	float	snear_scrl_scale[MTD_XY];		//!< 超近景スクロールスケール

} GMS_MAP_SYS_WORK;

// GMS_MAP_SYS_WORK::flag
#define GMD_MAP_FLAG_ADD_MAP_1	(0x00000001)	//!< 追加マップ (超近景マップ)表示あり
#define GMD_MAP_FLAG_ADD_MAP_2	(0x00000002)	//!< 追加マップ (中景マップ)表示あり
#define GMD_MAP_FLAG_ADD_MAP_3	(0x00000004)	//!< 追加マップ (中景1(スクロールあり)マップ)表示あり
#define GMD_MAP_FLAG_ADD_MAP_4	(0x00000008)	//!< 追加マップ (中景2(スクロールあり)マップ)表示あり
#define GMD_MAP_FLAG_ADD_MAP_5	(0x00000010)	//!< 追加マップ (中景3(スクロールあり)マップ)表示あり
// 以下連番

#define GMD_MAP_FLAG_DISP_OFF	(0x08000000)	//!< マップ描画OFF
#define GMD_MAP_FLAG_DISP_OFF_B	(0x10000000)	//!< B面描画OFF
#define GMD_MAP_FLAG_ADD_MAP_MAIN_ADD_SCRL_H	(0x20000000)	//!< メインカメラ基点からの加算値ループ
//#define GMD_MAP_FLAG_ADD_MAP_LOOP_V	(0x40000000)	//!< 追加マップ縦ループあり
#define GMD_MAP_FLAG_ADD_MAP_LOOP_X	(0x80000000)	//!< 追加マップ横ループあり


// マップモデルユーザーフラグ設定
#define GMD_MAP_MODEL_USER_FLAG0_WATER		(0x0001)	//!< 滝シェーダー設定
#define GMD_MAP_MODEL_USER_FLAG0_EX_LIGHT	(0x0002)	//!< 専用ライト設定

#if _IPHONE
#if defined GMD_DEBUG_NO_CREATE_NEAR
#define GMD_MAP_ADDMAP_DRAW_START ( GME_MAP_ADD_MAP_MID )
#else
#define GMD_MAP_ADDMAP_DRAW_START ( GME_MAP_ADD_MAP_NEAR )
#endif // GMD_DEBUG_NO_CREATE_NEAR

#if defined GMD_DEBUG_NO_CREATE_M13
#define GMD_MAP_ADDMAP_DRAW_END ( GME_MAP_ADD_MAP_MID1 )
#else
#define GMD_MAP_ADDMAP_DRAW_END ( GME_MAP_ADD_MAP_MAX )
#endif

#define GMD_MAP_PRIM_DRAW_USE_SORT		(1)		//!<	疑似ソート使用
#if GMD_MAP_PRIM_DRAW_USE_SORT

#define GMD_MAP_PRIM_DRAW_SORT_NUM		(8)		//!<	ソートをストックする個数

#define GMD_MAP_PRIM_DRAW_OP_NON		(0)		//!<	ブレンド処理　なし
#define GMD_MAP_PRIM_DRAW_OP_BLEND		(1)		//!<	ブレンド処理　ブレンド
#define GMD_MAP_PRIM_DRAW_OP_ADD		(2)		//!<	ブレンド処理　加算
#endif // GMD_MAP_PRIM_DRAW_USE_SORT

#define GMD_MAP_PRIM_DRAW_WORK_NUM		(32)	//!<	描画ワーク個数(最終的には減る予定)
#define GMD_MAP_PRIM_DRAW_STACK_NUM		(511)	//!<	描画スタック個数(増える？)

typedef struct tag_GMS_MAP_PRIM_DRAW_STACK {
	AOS_TVX_VERTEX*	vtx;		//!<	頂点情報
	u16				vtx_num;	//!<	頂点数
	MP_BLOCK		mp;			//!<	描画情報
	float			dx;			//!<	描画位置X
	float			dy;			//!<	描画位置Y
	float			dz;			//!<	描画位置Z
	u32				rsv[3];		//!<	予約領域
} GMS_MAP_PRIM_DRAW_STACK; // 32byte

typedef struct tag_GMS_MAP_PRIM_DRAW_WORK {
	s32						tex_id;				//!<	使用テクスチャID
	u32						all_vtx_num;		//!<	総頂点数
	u32						stack_num;			//!<	総スタック数
	u32						op;					//!<	ブレンドオペレーション
	GMS_MAP_PRIM_DRAW_STACK	stack[GMD_MAP_PRIM_DRAW_STACK_NUM];		//!<	描画頂点スタック
} GMS_MAP_PRIM_DRAW_WORK;	// 16byte + 32byte * 127 = 4080 byte

/// TVXUVモーション テクスチャID-描画管理インデックス
typedef struct tag_GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX {
	u16 tex_id; //!< テクスチャID
	u16 mgr_id; //!< 描画管理ID
} GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX;

/// TVXUVモーション 描画ID-FRAME管理データ
typedef struct tag_GMS_MAP_PRIM_DRAW_TVX_MGR {
	u16 motion_id;	//!< 使用UVモーションID
	u16 time;		//!< 描画フレーム数
} GMS_MAP_PRIM_DRAW_TVX_MGR;

/// TVXUVモーション 実行時UVモーション管理ワークデータ
typedef struct tag_GMS_MAP_PRIM_DRAW_TVX_UV_WORK {
	s32 mgr_index_tbl_num;		//!< 使用するテクスチャID-描画管理インデックス 個数
	u32 *mgr_index_tbl_addr;	//!< 使用するテクスチャID-描画管理インデックス アドレス (変換して使用する)
	s32 *mgr_tbl_num;			//!< 使用する描画ID-FRAME管理データ 個数
	u32 *mgr_tbl_addr;			//!< 使用する描画ID-FRAME管理データ アドレス (変換して使用する)
	u32 *uv_mgr_tbl_addr;		//!< 描画UV管理データ (変換して使用する)
	u32 *frame_index_tbl;		//!< 管理フレームindexーブル
	u32 *frame_tbl;				//!< 管理フレームテーブル
	s32 *tex_uv_index_tbl;		//!< テクスチャUVインデックステーブル
} GMS_MAP_PRIM_DRAW_TVX_UV_WORK;

#endif //_IPHONE

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
#if _IPHONE
#define GMM_MAP_IS_RANGE(_src, _min, _max) (_min < _src && _src < _max) //!< 指定値の最大/最小範囲チェック
#endif // _IPHONE

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmMapDest(MTS_TASK_TCB *tcb);
static void gmMapMain(MTS_TASK_TCB *tcb);
//static void gmMapDT(void *data);

static void gmMapDrawMapRange(OBS_ACTION3D_NN_WORK *obj3d_tbl, MP_HEADER *mp_header, MD_HEADER *md_header,
				  float trans_x, float trans_y, float trans_z,
				  s32 block_left, s32 block_right, s32 block_top, s32 block_bottom);

#if !_IPHONE
static void gmMapFallShaderSettingPrioPreMidMapUserFunc(void *data);
static void gmMapDrawFallShaderPrioPreMidMapUserFunc(void *data);
#else // !_IPHONE
static void gmMapInitDrawMapTvx(void);
static void gmMapSetDrawMapTvx(void *tvxamb, MP_HEADER *mp_header, MD_HEADER *md_header,
							   float pos_x, float pos_y, float trans_x, float trans_y, float diff_z, BOOL loop_h = FALSE);
static void gmMapSetDrawMapRangeTvx(void *tvxamb, MP_HEADER *mp_header, MD_HEADER *md_header,
				  float trans_x, float trans_y, float trans_z,
				  s32 block_left, s32 block_right, s32 block_top, s32 block_bottom);
static void gmMapExecuteDrawMapTvx(NNS_MATRIX* mtx, NNS_TEXLIST* texlist);
static void gmMapExecuteDrawMapTvxCore(NNS_MATRIX* mtx, GMS_MAP_PRIM_DRAW_WORK* work, AMS_PARAM_DRAW_PRIMITIVE* dat, u32 color);
static void gmMapGetDrawMapTvxTexScrollUV(s32 tex_id, NNS_TEXCOORD* scr_uv);
static void gmMapBuildDrawMapTvxTexScroll(void);
static void gmMapFlushDrawMapTvxTexScroll(void);
static void gmMapUpdateDrawMapTvxTexScroll(void);

static void gmMapCreateUsePrimMatrix(void);
static NNS_MATRIX* gmMapGetUsePrimMatrix(int rot, int flip);
#endif // !_IPHONE

#if _WII
static NNE_BOOL gmMapFallMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param);
#endif
//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_map_obj3d = NULL;			//!< MAP描画用 描画オブジェクト 0番はブランク
static s32					gm_map_reg_obj3d_num = 0;		//!< 登録オブジェクト数
static s32					gm_map_release_obj3d_num = 0;	//!< 開放オブジェクト数
static MTS_TASK_TCB			*gm_map_tcb = NULL;
static u32					gm_map_draw_command_state = OBD_DRAW_CMD_STATE_3DNN;	//!< マップ描画用コマンドステート
#if !_IPHONE
static BOOL					gm_map_use_fall_shader = FALSE;	//!< 滝シェーダー使用
static u32					gm_map_fall_rander_command_state = OBD_DRAW_CMD_STATE_WATER_BACK;	//!< 滝シェーダー用レンダリングコマンドステート
#else // !_IPHONE
static AOS_TEXTURE						gm_map_texture;					//!< 使用するテクスチャリスト
static AMS_AMB_HEADER					*gm_map_model   = NULL;			//!< 使用するモデルリスト
static BOOL								gm_map_tex_load_init = FALSE;	//!< 初回ロード
static int								gm_map_tex_draw_count = 0;	//!< 初回描画
static GMS_MAP_PRIM_DRAW_WORK			gm_map_prim_draw_work[GMD_MAP_PRIM_DRAW_WORK_NUM];	//!< プリミティブ描画ワーク
static NNS_VECTOR						gm_map_prim_draw_base_pos;	//!< 描画基準位置
static GMS_MAP_PRIM_DRAW_TVX_UV_WORK	*gm_map_prim_draw_uv_work;	//!< プリミティブUVモーションワーク

static u32					gm_map_draw_size[MTD_XY]; // map search size
static s32					gm_map_draw_bgm_timer; // bgm制御用タイマー
#endif // !_IPHONE
static u32					gm_map_draw_margin_adjust = 0;	//!< 描画時範囲補正値

static s16					gm_map_block_check[GMD_MAP_DRAW_CHECK_WIDTH][GMD_MAP_DRAW_CHECK_HEIGHT];	//!< 描画時ブロックチェック用ワーク

#if _IPHONE
#include "gmMapTbl.inc"

/// 追加背景用中景使用番号テーブル
static const s32 gm_map_add_tbl_use_no[] = {
	GME_MAP_ADD_MAP_MID1, // Zone1
	GME_MAP_ADD_MAP_MID2, // Zone2
	GME_MAP_ADD_MAP_MID2, // Zone3
	GME_MAP_ADD_MAP_MID3, // Zone4
	GME_MAP_ADD_MAP_MID1, // ZoneF
	GME_MAP_ADD_MAP_MID1, // SS
};

/// 行列計算情報保存用ワーク
static NNS_MATRIX gm_map_use_prim_mtx[GMD_MAP_USE_PRIM_MTX_NUM];

static u32 gm_map_prim_draw_tvx_color; //!< マップ描画に使用する色
static s32* gm_map_prim_draw_tvx_alpha_set; //!< マップ描画に使用するアルファ設定
#endif // _IPHONE

/// 追加背景用カメラIDテーブル
static const s32	gm_map_addmap_camera_tbl[GME_MAP_ADD_MAP_MAX] = {
	GME_CAMERA_NO_SNEAR,	// 超近景
	GME_CAMERA_NO_MAIN,		// 中景
	GME_CAMERA_NO_MID1,		// スクロールタイプ中景1
	GME_CAMERA_NO_MID2,		// スクロールタイプ中景2
	GME_CAMERA_NO_MID3,		// スクロールタイプ中景3
};

/// 追加背景用Z位置標準設定テーブル
static const float	gm_map_addmap_pos_z_tbl[GME_MAP_ADD_MAP_MAX] = {
	GMD_MAP_N_POS_Z,
	GMD_MAP_M_POS_Z,
	GMD_MAP_M1_POS_Z,
	GMD_MAP_M2_POS_Z,
	GMD_MAP_M3_POS_Z,
};

/// 追加背景用描画コマンドステートテーブル
static const u32	gm_map_addmap_command_state_tbl[GME_MAP_ADD_MAP_MAX] = {
	OBD_DRAW_CMD_STATE_NEAR_MAP,
	OBD_DRAW_CMD_STATE_3DNN,
	OBD_DRAW_CMD_STATE_3DNN,
	OBD_DRAW_CMD_STATE_3DNN,
	OBD_DRAW_CMD_STATE_3DNN,
};

/// 追加背景用描画コマンドステートテーブル z1-2, z1-3, z3-1, z3-3専用
static const u32	gm_map_addmap_command_state_z1_act2_3_z3_act1_3_tbl[GME_MAP_ADD_MAP_MAX] = {
	OBD_DRAW_CMD_STATE_NEAR_MAP,
	OBD_DRAW_CMD_STATE_MAPMID,
	OBD_DRAW_CMD_STATE_MAPMID,
	OBD_DRAW_CMD_STATE_MAPMID,
	OBD_DRAW_CMD_STATE_MAPMID,
};

/// 追加背景用描画コマンドステートテーブル ZoneFinal専用
static const u32	gm_map_addmap_command_state_zf_tbl[GME_MAP_ADD_MAP_MAX] = {
	OBD_DRAW_CMD_STATE_NEAR_MAP,
	OBD_DRAW_CMD_STATE_MAPMID,
	OBD_DRAW_CMD_STATE_MAPMID,
	OBD_DRAW_CMD_STATE_MAPMID,
	OBD_DRAW_CMD_STATE_MAPMID,
};

#if _IPHONE
static const u32 gm_map_set_draw_size[GME_MAP_DRAW_SIZE_NUM][MTD_XY] = {
	{ 6, 6}, // 基本
	{ 6, 4}, // 0 or 180
	{ 4, 6}, // 90 or 270
	{ 6, 4}, // BOSS1
	{ 4, 2}, // BOSS2
	{ 4, 2}, // BOSS3
	{ 4, 2}, // BOSS4
	{ 6, 2}, // BOSS4_2
	{ 6, 2}, // BOSSF
};
#endif // _IPHONE

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// データ構築
// ==========================================================================
// ==========================================================================
// GmMapBuildDataInit
/*!
 *	マップデータ構築 準備
 */
// ==========================================================================
void GmMapBuildDataInit(void)
{
	// SSはマップを使わないので終了
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_SS) {
		return;
	}

	MTM_ASSERT(gm_map_obj3d == NULL);
#if !_IPHONE
	// マップ描画用 描画オブジェクトワーク取得
	gm_map_obj3d = (OBS_ACTION3D_NN_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_NN_WORK) * GMD_MAP_OBJECT_NUM);
	amZeroMemory(gm_map_obj3d, sizeof(OBS_ACTION3D_NN_WORK) * GMD_MAP_OBJECT_NUM);
#endif // !_IPHONE
	// マップブロック0はブランクなので、1から初期化
	gm_map_reg_obj3d_num = GMD_MAP_OBJECT_START;
#if !_IPHONE
	// シェーダー使用状況クリア
	gm_map_use_fall_shader = FALSE;
#else // !_IPHONE
	//	メモリ初期化
	gmMapBuildDrawMapTvxTexScroll();
	//	ロード初期化
	gm_map_tex_load_init = TRUE;
#endif // !_IPHONE
}

// ==========================================================================
// GmMapBuildDataLoop
/*!
 *	マップデータ構築
 *
 *	@return	TRUE : ビルド終了
 */
// ==========================================================================
BOOL GmMapBuildDataLoop(void)
{
#if !_IPHONE
	s32						i;
	AMS_AMB_HEADER			*amb_header;
	OBS_ACTION3D_NN_WORK	*obj_3d;
	void					*mtn_data;
	u32						use_light_flag_and = (u32)~0;
	u32						use_light_flag_or = 0;
	u32						use_ex_light_flag_and = (u32)~0;
	u32						use_ex_light_flag_or = 0;

	AMS_AMB_FILE			*file_info;
#if !_WII
	NNF_DRAWOBJ				drawflag;
#endif

	MTM_ASSERT(gm_map_obj3d);

	// 使用ライト設定
	// Zone1, 4は別ライト
#if _WII
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_1) {
		use_light_flag_and	= (u32)~OBD_LIGHT_USE_FLAG_0;
		use_light_flag_or	= OBD_LIGHT_USE_FLAG_5;
		use_ex_light_flag_and	= (u32)~OBD_LIGHT_USE_FLAG_0;
		use_ex_light_flag_or	= OBD_LIGHT_USE_FLAG_5;
	}
	else if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_4) {
		use_light_flag_and	= (u32)~OBD_LIGHT_USE_FLAG_0;
		use_light_flag_or	= OBD_LIGHT_USE_FLAG_5;
		use_ex_light_flag_and	= (u32)~OBD_LIGHT_USE_FLAG_0;
		use_ex_light_flag_or	= OBD_LIGHT_USE_FLAG_4;
	}
#else
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_1 ||
			GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_4) {
		use_light_flag_and	= (u32)~OBD_LIGHT_USE_FLAG_0;
		use_light_flag_or	= OBD_LIGHT_USE_FLAG_5;
		use_ex_light_flag_and	= (u32)~OBD_LIGHT_USE_FLAG_0;
		use_ex_light_flag_or	= OBD_LIGHT_USE_FLAG_5;
	}
#endif

	amb_header = (AMS_AMB_HEADER*)g_gm_gamedat_map[GMD_GAMEDAT_MAP_MODEL];

	// オブジェクト初期化
	if (gm_map_reg_obj3d_num < amb_header->file_num) {
		if (GsMainSysGetDisplayListRegistNum() >= AMD_REGISTLIST_NUM - 64) {
			return (FALSE);
		}
		for (i = gm_map_reg_obj3d_num, obj_3d = gm_map_obj3d + gm_map_reg_obj3d_num;
						i < amb_header->file_num; i++, obj_3d++) {
			MTM_ASSERT(i < GMD_MAP_OBJECT_NUM);

			amBindGet(amb_header, i, &file_info);
#if !_WII
			drawflag = 0;
			if (file_info->user0 & GMD_MAP_MODEL_USER_FLAG0_WATER) {
			// 滝シェーダーオブジェクト
				// 描画フラグ設定
				drawflag = NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1;
				// 滝シェーダー使用フラグON
				gm_map_use_fall_shader = TRUE;
			}
#endif

			ObjAction3dNNModelLoad(obj_3d,
						NULL/*data_work*/, NULL/*file_name*/,
						i/*index*/, g_gm_gamedat_map[GMD_GAMEDAT_MAP_MODEL],
						NULL/*filename_tex*/, g_gm_gamedat_map[GMD_GAMEDAT_MAP_TEX],
#if !_WII
						drawflag/*NNF_DRAWOBJ drawflag*/);
#else
						0/*NNF_DRAWOBJ drawflag*/);
#endif
			// 使用ライト設定
			if (file_info->user0 & GMD_MAP_MODEL_USER_FLAG0_EX_LIGHT) {
				obj_3d->use_light_flag &= use_ex_light_flag_and;
				obj_3d->use_light_flag |= use_ex_light_flag_or;
			}
			else {
				obj_3d->use_light_flag &= use_light_flag_and;
				obj_3d->use_light_flag |= use_light_flag_or;
			}

#if _WII
			if (file_info->user0 & GMD_MAP_MODEL_USER_FLAG0_WATER) {
			// 滝シェーダーオブジェクト
				// マテリアルコールバック設定
				obj_3d->material_cb_func = gmMapFallMaterialCallback;
				obj_3d->material_cb_param = NULL;
				// 滝シェーダー使用フラグON
				gm_map_use_fall_shader = TRUE;
			}
#endif

			// 表裏反転対応
			obj_3d->drawflag |= NND_DRAWOBJ_DOUBLESIDE;

			// 登録数加算
			gm_map_reg_obj3d_num++;


			if (GsMainSysGetDisplayListRegistNum() >= AMD_REGISTLIST_NUM - 64) {
				break;
			}
		}
	}

	if (gm_map_reg_obj3d_num < amb_header->file_num) {
		// 登録処理中
		return (FALSE);
	}

	for (i = GMD_MAP_OBJECT_START, obj_3d = gm_map_obj3d + GMD_MAP_OBJECT_START;
				i < gm_map_reg_obj3d_num; i++, obj_3d++) {
		if (ObjAction3dNNModelLoadCheck(obj_3d) == FALSE) {
			// まだ登録されていないものあり
			return (FALSE);
		}
		else if (!(obj_3d->mtn[0] || obj_3d->mat_mtn[0])) {
			// モーション登録チェック
			if (g_gm_gamedat_map[GMD_GAMEDAT_MAP_MTN] &&
					i < ((AMS_AMB_HEADER*)g_gm_gamedat_map[GMD_GAMEDAT_MAP_MTN])->file_num) {
				mtn_data = amBindGet((AMS_AMB_HEADER*)g_gm_gamedat_map[GMD_GAMEDAT_MAP_MTN], i, NULL);
				if (mtn_data) {
					// モーション登録
					ObjAction3dNNMotionLoad(obj_3d, 0/*reg_file_id*/, FALSE/*marge*/,
									NULL/*data_work*/, NULL/*filename*/,
									i, g_gm_gamedat_map[GMD_GAMEDAT_MAP_MTN],
									1/*motion_num*/, 1/*mmotion_num*/);
					ObjDrawAction3dActionSet3DNN(obj_3d, 0/*id*/, 0/*mbuf_id*/);
				}
			}
			// マテリアルモーション登録チェック
			if (g_gm_gamedat_map[GMD_GAMEDAT_MAP_MMTN] &&
					i < ((AMS_AMB_HEADER*)g_gm_gamedat_map[GMD_GAMEDAT_MAP_MMTN])->file_num) {
				mtn_data = amBindGet((AMS_AMB_HEADER*)g_gm_gamedat_map[GMD_GAMEDAT_MAP_MMTN], i, NULL);
				if (mtn_data) {
					// マテリアルモーション登録
					ObjAction3dNNMaterialMotionLoad(obj_3d, 0/*reg_file_id*/,
									NULL/*data_work*/, NULL/*filename*/,
									i, g_gm_gamedat_map[GMD_GAMEDAT_MAP_MMTN],
									1/*motion_num*/, 1/*mmotion_num*/);
					ObjDrawAction3dActionSet3DNNMaterial(obj_3d, 0/*id*/);
				}
			}
		}
	}

	// 滝シェーダー使用時コマンドステート設定
	if (gm_map_use_fall_shader) {
		if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_2) {
			gm_map_fall_rander_command_state = OBD_DRAW_CMD_STATE_WATER_BACK;
		}
		else if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL) {
			gm_map_fall_rander_command_state = OBD_DRAW_CMD_STATE_WATER_MAPMID;
		}
		else {
			// 滝シェーダーマップはZONE2, ZoneFinalのみ
			MTM_ASSERT(0);
			gm_map_use_fall_shader = FALSE;
		}
	}

#else //!_IPHONE
	// SSはマップを使わないので終了
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_SS) {
		return TRUE;
	}

	if (_am_displaylist_manager.regist_num >= AMD_REGISTLIST_NUM - 64) {
		return (FALSE);
	}
	//	初回
	if (gm_map_tex_load_init) {
		AoTexBuild(&gm_map_texture, g_gm_gamedat_map[GMD_GAMEDAT_MAP_TEX]);
		u8* txb = (u8*)gm_map_texture.txb;
		amConvertAddress(txb);
		AoTexLoad(&gm_map_texture);
		gm_map_tex_load_init = FALSE;
	}

	// テクスチャロードチェック
	if (!AoTexIsLoaded(&gm_map_texture)) {
		return (FALSE);
	}
	
	gm_map_tex_draw_count = GMD_MAP_DRAW_INIT_COUNT;
#endif //!_IPHONE

	// 登録終了
	return (TRUE);
}

// ==========================================================================
// GmMapBuildColData
/*!
 *	マップ地形データ構築
 */
// ==========================================================================
void GmMapBuildColData(void)
{
	/* 地形用MAP設定 */
	// MAPブロック幅取得
	g_gm_main_system.map_fcol.map_block_num_x = ((MP_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_ATTR_A_MP])->map_w;
	g_gm_main_system.map_fcol.map_block_num_y = ((MP_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_ATTR_A_MP])->map_h;

	g_gm_main_system.map_fcol.block_map_datap[0] = (MP_BLOCK*)(((MP_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_ATTR_A_MP]) + 1);
	g_gm_main_system.map_fcol.block_map_datap[1] = (MP_BLOCK*)(((MP_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_ATTR_B_MP]) + 1);

	g_gm_main_system.map_fcol.diff_block_num	= ((DF_HEADER*)g_gm_gamedat_map_attr_set[GMD_GAMEDAT_ATTRSET_DF])->block_num;
	g_gm_main_system.map_fcol.dir_block_num		= ((DI_HEADER*)g_gm_gamedat_map_attr_set[GMD_GAMEDAT_ATTRSET_DI])->block_num;
	g_gm_main_system.map_fcol.attr_block_num	= ((AT_HEADER*)g_gm_gamedat_map_attr_set[GMD_GAMEDAT_ATTRSET_AT])->block_num;
	g_gm_main_system.map_fcol.cl_diff_datap		= (DF_BLOCK*)(((DF_HEADER*)g_gm_gamedat_map_attr_set[GMD_GAMEDAT_ATTRSET_DF]) + 1);
	g_gm_main_system.map_fcol.direc_datap		= (DI_BLOCK*)(((DI_HEADER*)g_gm_gamedat_map_attr_set[GMD_GAMEDAT_ATTRSET_DI]) + 1);
	g_gm_main_system.map_fcol.char_attr_datap	= (AT_BLOCK*)(((AT_HEADER*)g_gm_gamedat_map_attr_set[GMD_GAMEDAT_ATTRSET_AT]) + 1);

	g_gm_main_system.map_fcol.left = 0;
	g_gm_main_system.map_fcol.top = 0;
	g_gm_main_system.map_fcol.right = g_gm_main_system.map_fcol.map_block_num_x*64;
	g_gm_main_system.map_fcol.bottom = g_gm_main_system.map_fcol.map_block_num_y*64;
	// g_gm_main_system.map_fcol.block_datap = (u16*)context->fs_req->buf;

	// マップサイズ設定
	g_gm_main_system.map_size[MTD_X] = g_gm_main_system.map_fcol.map_block_num_x*64;
	g_gm_main_system.map_size[MTD_Y] = g_gm_main_system.map_fcol.map_block_num_y*64;
}

// ==========================================================================
// GmMapFlushData
/*!
 *	マップデータ片付け
 */
// ==========================================================================
void GmMapFlushData(void)
{
//	MTM_ASSERT(gm_map_obj3d);
//	if (gm_map_obj3d) {
//		amMemFree(gm_map_obj3d);
//		gm_map_obj3d = NULL;
//	}
//	gm_map_reg_obj3d_num = 0;
	gm_map_release_obj3d_num = GMD_MAP_OBJECT_START;

#if _IPHONE
	// SSはマップを使わないので終了
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_SS) {
		return;
	}

	AoTexRelease(&gm_map_texture);
	gmMapFlushDrawMapTvxTexScroll();
#endif //_IPHONE
}

// ==========================================================================
// GmMapFlushDataLoop
/*!
 *	マップデータ片付けループ
 *
 *	@return	TRUE : 片付け終了
 */
// ==========================================================================
BOOL GmMapFlushDataLoop(void)
{
#if !_IPHONE
	s32						i;
	AMS_AMB_HEADER			*amb_header;
	OBS_ACTION3D_NN_WORK	*obj_3d;

	if (gm_map_obj3d == NULL) {
		return (TRUE);
	}

	amb_header = (AMS_AMB_HEADER*)g_gm_gamedat_map[GMD_GAMEDAT_MAP_MODEL];

	// オブジェクト開放
	if (gm_map_release_obj3d_num < gm_map_reg_obj3d_num) {
		if (GsMainSysGetDisplayListRegistNum() >= AMD_REGISTLIST_NUM - 64) {
			return (FALSE);
		}
		for (i = gm_map_release_obj3d_num, obj_3d = gm_map_obj3d + gm_map_release_obj3d_num;
						i < gm_map_reg_obj3d_num; i++, obj_3d++) {
			MTM_ASSERT(i < GMD_MAP_OBJECT_NUM);

			if (obj_3d->motion) {
				// モーション開放
				ObjAction3dNNMotionRelease(obj_3d);
			}

			// モデル解放
			ObjAction3dNNModelRelease(obj_3d);

			// 開放数加算
			gm_map_release_obj3d_num++;

			if (GsMainSysGetDisplayListRegistNum() >= AMD_REGISTLIST_NUM - 64) {
				break;
			}
		}
	}

	if (gm_map_release_obj3d_num < gm_map_reg_obj3d_num) {
		// 開放処理中
		return (FALSE);
	}

	for (i = GMD_MAP_OBJECT_START, obj_3d = gm_map_obj3d + GMD_MAP_OBJECT_START;
				i < gm_map_reg_obj3d_num; i++, obj_3d++) {
		if (ObjAction3dNNModelReleaseCheck(obj_3d) == FALSE) {
			// まだ開放されていないものあり
			return (FALSE);
		}
	}

	if (gm_map_obj3d) {
		amMemFree(gm_map_obj3d);
		gm_map_obj3d = NULL;
		gm_map_reg_obj3d_num = 0;
	}
#else //!_IPHONE
	// SSはマップを使わないので終了
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_SS) {
		return TRUE;
	}

	if (!AoTexIsReleased(&gm_map_texture)) {
		return (FALSE);
	}
#endif //!_IPHONE

	// 開放終了
	return (TRUE);
}

// ==========================================================================
// GmMapFlushColData
/*!
 *	マップ地形データ開放
 */
// ==========================================================================
void GmMapFlushColData(void)
{
}

// ==========================================================================
// GmMapRelease
/*!
 *	マップデータ開放
 */
// ==========================================================================
void GmMapRelease(void)
{
	s32	i;

	for (i = 0; i < GMD_GAMEDAT_MAP_MAX; i++) {
		if (g_gm_gamedat_map[i]) {
			amMemFree(g_gm_gamedat_map[i]);
		}
		g_gm_gamedat_map[i] = NULL;
	}
}

// ==========================================================================
// 初期化
// ==========================================================================
// ==========================================================================
// GmMapInit
/*!
 *	マップ初期化
 */
// ==========================================================================
void GmMapInit(void)
{
	s32							i;
	GMS_MAP_SYS_WORK			*map_sys_work;
	GMS_MAP_OTHER_MAP_STATE		*map_state;

	MTM_ASSERT(gm_map_tcb == NULL);

    // 地形判定データ設定
	ObjSetDiffCollision(&g_gm_main_system.map_fcol);

	// 地形描画
	gm_map_tcb = MTM_TASK_MAKE_TCB(gmMapMain, gmMapDest,
			0, GMD_TASK_PAUSELEVEL_DEF, GMD_TASK_PRIO_MAP, GMD_TASK_GROUP_MAP,
			sizeof(GMS_MAP_SYS_WORK), "GM_MAP_MAIN");
	map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);
	MI_CpuClear8(map_sys_work, sizeof(GMS_MAP_SYS_WORK));

	// 描画コマンドステート初期化
	gm_map_draw_command_state = OBD_DRAW_CMD_STATE_3DNN;

	// 描画時範囲補正値初期化
	gm_map_draw_margin_adjust = 0;

#if _IPHONE
	u32 stage_id = g_gs_main_sys_info.stage_id;
	
	// 画面サイズ設定
	GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_HORI);
	map_sys_work->auto_resize = TRUE;
	switch (stage_id) {
			// Zone1/2はカメラ移動が大きいのでリサイズしない
		case GSD_MAIN_STAGE_ID_1_1:
		case GSD_MAIN_STAGE_ID_1_2:
		case GSD_MAIN_STAGE_ID_1_3:
		case GSD_MAIN_STAGE_ID_2_1:
		//case GSD_MAIN_STAGE_ID_2_2:
		case GSD_MAIN_STAGE_ID_2_3:
			GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_DEF);
			map_sys_work->auto_resize = FALSE;
			break;
			
			// ボスステージは手動でぎりぎりまで最適化するのでリサイズしない
		case GSD_MAIN_STAGE_ID_1_BOSS:
		case GSD_MAIN_STAGE_ID_2_BOSS:
		case GSD_MAIN_STAGE_ID_3_BOSS:
		case GSD_MAIN_STAGE_ID_4_BOSS:
		case GSD_MAIN_STAGE_ID_FINAL_1:
			map_sys_work->auto_resize = FALSE;
			
		default:
			break;
	}
	
	// 計算用マトリックス作成
	gmMapCreateUsePrimMatrix();
	
	// カラー設定
	u32 color = 0xe0e0e0ff; // 基本色
	if ((stage_id == GSD_MAIN_STAGE_ID_1_1) 
		||(stage_id == GSD_MAIN_STAGE_ID_1_2)) {
		color = 0xffffffff; // フルカラー
	}
	if (  (stage_id == GSD_MAIN_STAGE_ID_1_3) 
		||(stage_id == GSD_MAIN_STAGE_ID_1_BOSS)) {
		color = 0xe08A8Aff; // 夕方色
	}
	else if (stage_id == GSD_MAIN_STAGE_ID_4_3) {
		color = 0x606060ff; // 4-3限定色
	}
	gm_map_prim_draw_tvx_color = color;
	
	// アルファ設定
	gm_map_prim_draw_tvx_alpha_set = NULL;
	if ((stage_id == GSD_MAIN_STAGE_ID_2_1)
		|| (stage_id == GSD_MAIN_STAGE_ID_2_2)
		|| (stage_id == GSD_MAIN_STAGE_ID_2_3)
		|| (stage_id == GSD_MAIN_STAGE_ID_2_BOSS)
		) {
		gm_map_prim_draw_tvx_alpha_set = (s32*)gm_map_prim_draw_tvx_alpha_set_z2;
	}
	
	gm_map_draw_bgm_timer = GMD_MAP_DRAW_BGM_TIMER;
#endif // _IPHONE

	map_state = map_sys_work->map_state;
	for (i = 0; i < GME_MAP_ADD_MAP_MAX; i++, map_state++) {
		if (g_gm_gamedat_map_set_add[i*2] && g_gm_gamedat_map_set_add[i*2+1]) {
			// 追加マップあり
			map_sys_work->flag |= (GMD_MAP_FLAG_ADD_MAP_1 << i);

			// 表示Z位置取得
			map_state->pos_z = gm_map_addmap_pos_z_tbl[i];

			// マップサイズ取得
			map_state->map_block_num[MTD_X] = ((MP_HEADER*)g_gm_gamedat_map_set_add[i*2])->map_w;
			map_state->map_block_num[MTD_Y] = ((MP_HEADER*)g_gm_gamedat_map_set_add[i*2])->map_h; 

			map_state->map_size[MTD_X] = map_state->map_block_num[MTD_X] * 64;
			map_state->map_size[MTD_Y] = map_state->map_block_num[MTD_Y] * 64;

			// スクロールスケール取得
			map_state->scrl_scale[MTD_X] = ((float)map_state->map_size[MTD_X] - (float)(OBD_LCD_X)) /
												(g_gm_main_system.map_size[MTD_X] - (float)(OBD_LCD_X));
			map_state->scrl_scale[MTD_Y] = ((float)map_state->map_size[MTD_Y] - (float)(OBD_LCD_Y)) /
												(g_gm_main_system.map_size[MTD_Y] - (float)(OBD_LCD_Y));

			// コマンドステート取得
			if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_2 ||
					g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_3 ||
					g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_1 ||
					g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_3) {
				// Zone1-2, 1-3, 3-1, 3-3専用
				map_state->command_state = gm_map_addmap_command_state_z1_act2_3_z3_act1_3_tbl[i];
			}
			else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_FINAL_1) {
				// ZoneFinal専用
				map_state->command_state = gm_map_addmap_command_state_zf_tbl[i];
			}
			else {
				map_state->command_state = gm_map_addmap_command_state_tbl[i];
			}
		}
	}

	// Zone3-2専用設定
	if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
		// A面レール位置
		map_sys_work->map_state[GME_MAP_ADD_MAP_MID1].pos_z = GMD_MAP_A_POS_Z + 32.f;
		// B面レール位置
		map_sys_work->map_state[GME_MAP_ADD_MAP_MID2].pos_z = GMD_MAP_B_POS_Z + 32.f;
	}
	// ZoneFinal専用設定
	else if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_FINAL_1) {
		// M面より後ろを一律下げる
		map_sys_work->map_state[GME_MAP_ADD_MAP_MID].pos_z += (float)GMD_MAP_ADJUST_POS_Z_FINAL_M;
		map_sys_work->map_state[GME_MAP_ADD_MAP_MID1].pos_z += (float)GMD_MAP_ADJUST_POS_Z_FINAL_M;
		map_sys_work->map_state[GME_MAP_ADD_MAP_MID2].pos_z += (float)GMD_MAP_ADJUST_POS_Z_FINAL_M;
	}
}

// ==========================================================================
// 終了処理
// ==========================================================================
// ==========================================================================
// GmMapExit
/*!
 *	マップ終了処理
 */
// ==========================================================================
void GmMapExit(void)
{
	if (gm_map_tcb) {
		mtTaskClearTcb(gm_map_tcb);
	}
}

// ==========================================================================
// GmMapSetDrawState
/*!
 *	マップ描画コマンドステート設定
 *
 *	@param	command_state	[in]	GmMapDrawMap 使用時の描画コマンドステートを設定
 *
 *	@note
 *		初期設定は OBD_DRAW_CMD_STATE_3DNN です。
 */
// ==========================================================================
void GmMapSetDrawState(u32 command_state)
{
	gm_map_draw_command_state = command_state;
}

// ==========================================================================
// GmMapSetDrawMarginNormal
/*!
 *	マップ描画範囲補正設定 通常状態
 *
 *	@note
 *		描画時範囲補正値を通常状態値にします。
 */
// ==========================================================================
void GmMapSetDrawMarginNormal(void)
{
	gm_map_draw_margin_adjust = 0;
}

// ==========================================================================
// GmMapSetDrawMarginMag
/*!
 *	マップ描画範囲補正設定 範囲拡大状態
 *
 *	@note
 *		描画時範囲補正値を範囲拡大状態にします
 */
// ==========================================================================
void GmMapSetDrawMarginMag(void)
{
	gm_map_draw_margin_adjust = 1;
}

// ==========================================================================
// GmMapDrawMap
/*!
 *	マップ描画
 *
 *	@param	obj3d_tbl	[in]	描画するマップ描画オブジェクトワークテーブル
 *	@param	mp_header	[in]	描画するマップ MPデータ
 *	@param	md_header	[in]	描画するマップ MDデータ
 *	@param	pos_x		[in]	描画領域 左上基点 X (2D座標系)
 *	@param	pos_y		[in]	描画領域 左上基点 Y (2D座標系)
 *	@param	trans_x		[in]	移動量
 *	@param	trans_y		[in]	移動量 (2D座標系)
 *	@param	trans_z		[in]	移動量
 *	@param	loop_h		[in]	横描画領域がはみ出した時にループ描画する
 */
// ==========================================================================
void GmMapDrawMap(OBS_ACTION3D_NN_WORK *obj3d_tbl, MP_HEADER *mp_header, MD_HEADER *md_header, float pos_x, float pos_y,
				  float trans_x, float trans_y, float trans_z, BOOL loop_h/* = FALSE*/)
{
	s32	block_x_center, block_y_center;
	//s32	block_x, block_y;
	s32	block_left, block_right, block_top, block_bottom;
	s32	block_width, block_height;
	float	draw_ofst_x_0 = 0.f, draw_ofst_x_1 = 0.f;

#if defined (MTD_DEBUG)
	// デバッグ マップ非表示
	if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_MAP_NO_DISP) {
		return;
	}
#endif // #if defined (MTD_DEBUG)

	// 描画範囲取得
	block_x_center = ((s32)pos_x) >> (3 + 3);	// > char > block
	block_y_center = ((s32)pos_y) >> (3 + 3);

//	block_x -= GMD_MAP_DRAW_MARGIN;
//	block_y -= GMD_MAP_DRAW_MARGIN;

	block_width = GMD_MAP_DRAW_WIDTH + GMD_MAP_DRAW_MARGIN * 2 + gm_map_draw_margin_adjust * 2;
	block_height= GMD_MAP_DRAW_HEIGHT + GMD_MAP_DRAW_MARGIN * 2 + gm_map_draw_margin_adjust * 2;

	block_left		= block_x_center - (block_width >> 1);
	block_right		= block_x_center + (block_width >> 1);
	block_top		= block_y_center - (block_height >> 1);
	block_bottom	= block_y_center + (block_height >> 1);

#if 0
	if (!loop_h && block_left < 0) {
		block_left = 0;
	}
#else
	if (block_left < 0) {
		if (!loop_h) {
			block_left = 0;
		}
		else {
			block_left += mp_header->map_w;
			draw_ofst_x_0 = (float)-(mp_header->map_w << (3 + 3));
		}
	}
	else if (block_left >= mp_header->map_w) {
		if (!loop_h) {
			block_left = mp_header->map_w - block_width;
		}
		else {
			block_left -= mp_header->map_w;
			draw_ofst_x_0 = (float)+(mp_header->map_w << (3 + 3));
		}
	}
#endif
	if (block_top < 0) {
		block_top = 0;
	}

#if 0
	if (!loop_h && block_right >= mp_header->map_w) {
		block_right = mp_header->map_w - 1;
	}
#else
	if (block_right >= mp_header->map_w) {
		if (!loop_h) {
			block_right = mp_header->map_w - 1;
		}
		else {
			block_right -= mp_header->map_w;
			draw_ofst_x_1 = (float)+(mp_header->map_w << (3 + 3));
		}
	}
	else if (block_right < 0) {
		if (!loop_h) {
			block_right = block_width;
		}
		else {
			block_right += mp_header->map_w;
			draw_ofst_x_1 = (float)-(mp_header->map_w << (3 + 3));
		}
	}
#endif
	if (block_bottom >= mp_header->map_h) {
		block_bottom = mp_header->map_h - 1;
	}

	if (block_left < block_right) {
		// 通常描画
		if (loop_h) {
			gmMapDrawMapRange(obj3d_tbl, mp_header, md_header,
					  trans_x + draw_ofst_x_0/*補正されている場合の対応*/, trans_y, trans_z,
					  block_left, block_right, block_top, block_bottom);
		}
		else {
			gmMapDrawMapRange(obj3d_tbl, mp_header, md_header,
					  trans_x, trans_y, trans_z,
					  block_left, block_right, block_top, block_bottom);
		}
	}
	else {
		// 横ループ描画
		gmMapDrawMapRange(obj3d_tbl, mp_header, md_header,
				  trans_x + draw_ofst_x_0, trans_y, trans_z,
				  block_left, mp_header->map_w - 1, block_top, block_bottom);

		gmMapDrawMapRange(obj3d_tbl, mp_header, md_header,
				  trans_x + draw_ofst_x_1, trans_y, trans_z,
				  0, block_right, block_top, block_bottom);
	}
}





// ==========================================================================
// GmMapDrawMap
/*!
 *	マップ描画
 *
 *	@param	obj3d_tbl	[in]	描画するマップ描画オブジェクトワークテーブル
 *	@param	mp_header	[in]	描画するマップ MPデータ
 *	@param	md_header	[in]	描画するマップ MDデータ
 *	@param	pos_x		[in]	描画領域 左上基点 X (2D座標系)
 *	@param	pos_y		[in]	描画領域 左上基点 Y (2D座標系)
 *	@param	trans_x		[in]	移動量
 *	@param	trans_y		[in]	移動量 (2D座標系)
 *	@param	trans_z		[in]	移動量
 */
// ==========================================================================
//void gmMapDrawMap(OBS_ACTION3D_NN_WORK *obj3d_tbl, MP_HEADER *mp_header, MD_HEADER *md_header, float pos_x, float pos_y,
//				  float trans_x, float trans_y, float trans_z)
void gmMapDrawMapRange(OBS_ACTION3D_NN_WORK *obj3d_tbl, MP_HEADER *mp_header, MD_HEADER *md_header,
				  float trans_x, float trans_y, float trans_z,
				  s32 block_left, s32 block_right, s32 block_top, s32 block_bottom)
{
	s32	i, x_cnt, y_cnt;
	s32	block_width, block_height;
	MP_BLOCK	*mp_block_top, *mp_block;
	MD_BLOCK	*md_block_top, *md_block;

	MTM_ASSERT(block_left <= block_right);
	MTM_ASSERT(block_top <= block_bottom);

#if defined (GMD_MAP_DEBUG_NO_DRAW)
	return;
#endif

	// 描画チェックバッファクリア
	s16		*p_check;
	for (p_check = &gm_map_block_check[0][0], i = 0; i < GMD_MAP_DRAW_CHECK_WIDTH*GMD_MAP_DRAW_CHECK_HEIGHT; i++, p_check++) {
		*p_check = -1;
	}

//	block_x -= GMD_MAP_DRAW_MARGIN;
//	block_y -= GMD_MAP_DRAW_MARGIN;

	block_width = GMD_MAP_DRAW_WIDTH + GMD_MAP_DRAW_MARGIN * 2;
	block_height= GMD_MAP_DRAW_HEIGHT + GMD_MAP_DRAW_MARGIN * 2;

	mp_block_top = (MP_BLOCK*)(mp_header + 1);
	md_block_top = (MD_BLOCK*)(md_header + 1);

	// 描画
	{
		OBS_ACTION3D_NN_WORK	*obj_3d;
		VecFx32		pos = {0, 0, 0};
		VecU16		dir = {0, 0, 0};
		VecFx32		scale = {FX32_ONE, FX32_ONE, FX32_ONE};
		u32			disp_flag;
		s32			obj_id;				// 描画オブジェクトID
		s32			block_no;
		float		dx, dy;				// ブロック単位描画位置
		s32			ofst_x, ofst_y;		// ブロックID参照位置オフセット

		for (x_cnt = block_left; x_cnt <= block_right; x_cnt++) {

			for (y_cnt = block_top; y_cnt <= block_bottom; y_cnt++) {
				block_no = y_cnt * mp_header->map_w + x_cnt;
				mp_block = mp_block_top + block_no;

				// ブロック単位描画位置
				dx = (float)x_cnt;
				dy = (float)y_cnt;

				// 表示オブジェクトを求める
				obj_id	= mp_block->id;
				if (obj_id == 0) {

					md_block	= md_block_top + block_no;
					ofst_x	= (s32)md_block->ofst_x;
					ofst_y	= (s32)md_block->ofst_y;
					if ((ofst_x | ofst_y) == 0) {
						// ブロック無し
						continue;
					}
					block_no += mp_header->map_w * ofst_y + ofst_x;
					mp_block = mp_block_top + block_no;
					obj_id	= mp_block->id;
					dx		+= ofst_x;	// ブロック単位描画位置更新
					dy		+= ofst_y;
				}

				// 保険
				if ((obj_id == 0) || ((Sint32)obj_id >= gm_map_reg_obj3d_num)) {
					continue;
				}
				// ブロック描画済みチェック
				if (gm_map_block_check[GMD_MAP_DRAW_DF_SIZE + (s32)dx - block_left][GMD_MAP_DRAW_DF_SIZE + (s32)dy - block_top] != -1) {
					// 既に描画済み

					// 同じIDになっていない？
					MTM_ASSERT(gm_map_block_check[GMD_MAP_DRAW_DF_SIZE + (s32)dx - block_left][GMD_MAP_DRAW_DF_SIZE + (s32)dy - block_top] == obj_id);

					continue;
				}
				gm_map_block_check[GMD_MAP_DRAW_DF_SIZE + (s32)dx - block_left][GMD_MAP_DRAW_DF_SIZE + (s32)dy - block_top] = (s16)obj_id;

				// 描画オブジェクト取得
				obj_3d = obj3d_tbl + obj_id;

				// 描画コマンドステート設定
				obj_3d->command_state = gm_map_draw_command_state;

				// 行列を自前計算
				nnMakeUnitMatrix(&obj_3d->user_obj_mtx);
				nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx,
							//FXM_FX32_TO_FLOAT(trans_x) + (dx + 0.5f) * GMD_MAP_BLOCK_SIZE,
							//FXM_FX32_TO_FLOAT(-trans_y) + (-dy - 0.5f) * GMD_MAP_BLOCK_SIZE,
							//FXM_FX32_TO_FLOAT(trans_z));
							trans_x + (dx + 0.5f) * GMD_MAP_BLOCK_SIZE,
							-trans_y + (-dy - 0.5f) * GMD_MAP_BLOCK_SIZE,
							trans_z);

				// 回転
				nnRotateZMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx, (u16)(mp_block->rot * 0x4000));

				// 反転・スケール
				switch (mp_block->flip_h | (mp_block->flip_v << 1)) {
				case 0:
					nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx,
							1.0f*GMD_OBJ_DRAW_SCALE, 1.0f*GMD_OBJ_DRAW_SCALE, 1.0f*GMD_OBJ_DRAW_SCALE);
					break;
				case 1:
					nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx,
							-1.0f*GMD_OBJ_DRAW_SCALE, 1.0f*GMD_OBJ_DRAW_SCALE, 1.0f*GMD_OBJ_DRAW_SCALE);
					break;
				case 2:
					nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx,
							1.0f*GMD_OBJ_DRAW_SCALE, -1.0f*GMD_OBJ_DRAW_SCALE, 1.0f*GMD_OBJ_DRAW_SCALE);
					break;
				case 3:
					nnScaleMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx,
							-1.0f*GMD_OBJ_DRAW_SCALE, -1.0f*GMD_OBJ_DRAW_SCALE, 1.0f*GMD_OBJ_DRAW_SCALE);
					break;
				}

				nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx,
						- obj_3d->object->pNodeList[0].Translation.x
						- (float)GMD_MAP_BLOCK_LOCAL_SIZE * 0.5f,
						- obj_3d->object->pNodeList[0].Translation.y
						- (float)GMD_MAP_BLOCK_LOCAL_SIZE * 0.5f,
						- obj_3d->object->pNodeList[0].Translation.z);
					
//#if !_PC
//				// 暫定対応
//				if (((obj_id >= 35) && (obj_id <= 39)) || (obj_id == 68)) {
//					nnTranslateMatrix(&obj_3d->user_obj_mtx, &obj_3d->user_obj_mtx,
//						0.0f, (float)GMD_MAP_BLOCK_LOCAL_SIZE, 0.0f);
//				}
//#endif


				disp_flag = OBD_DISP_NOPOS | OBD_DISP_NOSCALE |
								OBD_DISP_NOSCALE | OBD_DISP_NODRAWSCALE |
								OBD_DISP_NODIR | OBD_DISP_NODIRFLIP | OBD_DISP_USERMTX |
								OBD_DISP_NOUPDATE;
#if !_IPHONE
				// 滝シェーダー設定
#if !_WII
				if (obj_3d->drawflag & NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1) {
#else
				if (obj_3d->material_cb_func == gmMapFallMaterialCallback) {
#endif
					ObjDraw3DNNUserFunc(gmMapDrawFallShaderPrioPreMidMapUserFunc,
								NULL, 0, obj_3d->command_state/* 中景～近景だけの場合はこれでよい*/);
				}
#endif // !_IPHONE
				ObjDrawAction3DNN(obj_3d, &pos, &dir, &scale, &disp_flag);
			}
		}
	} // 描画
}





// ==========================================================================
// 追加背景用カメラ管理
// ==========================================================================
// ==========================================================================
// GmMapSetAddMapXLoop
/*!
 *	追加マップ (超近景 中景色) 横ループあり
 */
// ==========================================================================
void GmMapSetAddMapXLoop(void)
{
	s32							i;
	GMS_MAP_SYS_WORK			*map_sys_work;
	GMS_MAP_OTHER_MAP_STATE		*map_state;
	if (gm_map_tcb == NULL) {
		MTM_ASSERT(!"gmMap.cpp:GmMapSetAddMapXLoop() Error map sys no init\n");
		return;
	}
	map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);

	map_sys_work->flag |= GMD_MAP_FLAG_ADD_MAP_LOOP_X;

	map_state = map_sys_work->map_state;
	for (i = 0; i < GME_MAP_ADD_MAP_MAX; i++, map_state++) {
		if (map_sys_work->flag & (GMD_MAP_FLAG_ADD_MAP_1 << i)) {
			// 追加マップあり

			// 近景と等倍でない時はスクロールスケールを変更する
			if (map_state->map_size[MTD_X] != g_gm_main_system.map_size[MTD_X]) {
				map_state->scrl_scale[MTD_X] = ((float)map_state->map_size[MTD_X]) /
													((float)g_gm_main_system.map_size[MTD_X]);
			}
		//	if (map_state->map_size[MTD_Y] != g_gm_main_system.map_size[MTD_Y]) {
		//	// スクロールスケール取得
		//		map_state->scrl_scale[MTD_Y] = ((float)map_state->map_size[MTD_Y]) /
		//											((float)g_gm_main_system.map_size[MTD_Y]);
		//	}

		}
	}
}

// ==========================================================================
// GmMapEnableAddMapUserScrlX
/*!
 *	追加マップ (超近景 中景色) ユーザー設定横方向スクロール値使用
 */
// ==========================================================================
void GmMapEnableAddMapUserScrlX(void)
{
	GMS_MAP_SYS_WORK			*map_sys_work;
	OBS_CAMERA					*camera;
	if (gm_map_tcb == NULL) {
		MTM_ASSERT(!"gmMap.cpp:GmMapEnableAddMapUserScrlX() Error map sys no init\n");
		return;
	}
	map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);

	if (map_sys_work->flag & GMD_MAP_FLAG_ADD_MAP_MAIN_ADD_SCRL_H) {
		// 既に設定済み
		return;
	}
	map_sys_work->flag |= GMD_MAP_FLAG_ADD_MAP_MAIN_ADD_SCRL_H;

	// 現在のカメラ座標取得
	camera = ObjCameraGet(g_obj.glb_camera_id);

	map_sys_work->main_cam_user_disp[MTD_X] = camera->disp_pos.x;
	map_sys_work->main_cam_user_disp[MTD_Y] = camera->disp_pos.y;
	//map_sys_work->main_cam_user_target[MTD_X] = camera->target_pos.x;
	//map_sys_work->main_cam_user_target[MTD_Y] = camera->target_pos.y;
	// 初期化時はtargetが設定されていないのでdisp_posから取得
	map_sys_work->main_cam_user_target[MTD_X] = camera->disp_pos.x;
	map_sys_work->main_cam_user_target[MTD_Y] = camera->disp_pos.y;

	// オフセットクリア
	map_sys_work->main_cam_user_ofst[MTD_X] =
		map_sys_work->main_cam_user_ofst[MTD_Y] = 0;
}

// ==========================================================================
// GmMapDisenableAddMapUserScrlX
/*!
 *	追加マップ (超近景 中景色) ユーザー設定横方向スクロール値無効
 */
// ==========================================================================
void GmMapDisenableAddMapUserScrlX(void)
{
	s32							i;
	GMS_MAP_SYS_WORK			*map_sys_work;
	OBS_CAMERA					*camera;
	float						move_cam_x;
	if (gm_map_tcb == NULL) {
		MTM_ASSERT(!"gmMap.cpp:GmMapDisenableAddMapUserScrlX() Error map sys no init\n");
		return;
	}
	map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);

	if (!(map_sys_work->flag & GMD_MAP_FLAG_ADD_MAP_MAIN_ADD_SCRL_H)) {
		// 設定されていない
		return;
	}
	map_sys_work->flag &= ~GMD_MAP_FLAG_ADD_MAP_MAIN_ADD_SCRL_H;

	// 現在のカメラ座標取得
	camera = ObjCameraGet(g_obj.glb_camera_id);

	move_cam_x = map_sys_work->main_cam_user_disp[MTD_X] + map_sys_work->main_cam_user_ofst[MTD_X]
						- camera->disp_pos.x;

	for (i = 0; i < GME_MAP_ADD_MAP_MAX; i++) {
		if (i == GMD_GAMEDAT_MAPSET_ADD_LOCAL_M_MP/2) {
			// 中景はのぞく
			continue;
		}

		// ユーザーが動かした分だけオフセット値として保存
		map_sys_work->map_state[i].cam_ofst[MTD_X] +=
				move_cam_x * map_sys_work->map_state[i].scrl_scale[MTD_X];
	}
}

// ==========================================================================
// GmMapSetAddMapScrlScaleX
/*!
 *	追加マップ (超近景 中景) スクロールスケール 倍率設定
 *
 *	@note
 *		通常 mag == 1
 */
// ==========================================================================
void GmMapSetAddMapScrlScaleMagX(GME_MAP_ADD_MAP map_type, s32 mag)
{
	GMS_MAP_SYS_WORK			*map_sys_work;
	GMS_MAP_OTHER_MAP_STATE		*map_state;

	if (gm_map_tcb == NULL) {
		MTM_ASSERT(!"gmMap.cpp:GmMapDisenableAddMapUserScrlX() Error map sys no init\n");
		return;
	}
	if (map_type == GME_MAP_ADD_MAP_MID ||
			(u32)map_type >= GME_MAP_ADD_MAP_MAX) {
		// 通常中景は設定できない
		MTM_ASSERT(0);
		return;
	}
	if (mag == 0) {
		MTM_ASSERT(0);
		mag = 1;
	}
	map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);

	// スクロールスケール取得
	map_state = &map_sys_work->map_state[map_type];
	if (map_sys_work->flag & GMD_MAP_FLAG_ADD_MAP_LOOP_X) {
		map_state->scrl_scale[MTD_X] = ((float)map_state->map_size[MTD_X]) /
											((float)g_gm_main_system.map_size[MTD_X]) / mag;
	}
	else {
		map_state->scrl_scale[MTD_X] =
				((float)map_state->map_size[MTD_X] - (float)(OBD_LCD_X)) /
								(g_gm_main_system.map_size[MTD_X] - (float)(OBD_LCD_X)) / mag;
	}
}

// ==========================================================================
// GmMapSetAddMapUserScrlXAddSize
/*!
 *	追加マップ (超近景 中景色) ユーザー設定横方向スクロール値使用
 *
 *	@param	move_size	[in]	メインカメラ移動量
 */
// ==========================================================================
void GmMapSetAddMapUserScrlXAddSize(float move_size)
{
	GMS_MAP_SYS_WORK			*map_sys_work;
	float						map_size_x;
	if (gm_map_tcb == NULL) {
		MTM_ASSERT(!"gmMap.cpp:GmMapSetAddMapUserScrlXAddSize() Error map sys no init\n");
		return;
	}
	map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);

	map_sys_work->main_cam_user_ofst[MTD_X]	+= move_size;

	map_size_x = (float)g_gm_main_system.map_size[MTD_X];

	// オフセット
	//if (map_sys_work->main_cam_user_ofst[MTD_X] >= map_size_x) {
	//if (map_sys_work->main_cam_user_disp[MTD_X] + map_sys_work->main_cam_user_ofst[MTD_X] - OBD_LCD_X >= map_size_x) {
	if (map_sys_work->main_cam_user_disp[MTD_X] + map_sys_work->main_cam_user_ofst[MTD_X] - OBD_LCD_X >= map_size_x) {
		map_sys_work->main_cam_user_ofst[MTD_X] -= map_size_x;
	}
	//else if (map_sys_work->main_cam_user_ofst[MTD_X] <= -map_size_x) {
	//else if (map_sys_work->main_cam_user_disp[MTD_X] + map_sys_work->main_cam_user_ofst[MTD_X] + OBD_LCD_X <= -map_size_x) {
	else if (map_sys_work->main_cam_user_disp[MTD_X] + map_sys_work->main_cam_user_ofst[MTD_X] + OBD_LCD_X <= -map_size_x) {
		map_sys_work->main_cam_user_ofst[MTD_X] += map_size_x;
	}
}

// ==========================================================================
// GmMapGetAddMapCameraPos
/*!
 *	メインカメラから追加背景用カメラ位置取得
 *
 *	@param	main_disp_pos	[in]	メインカメラdist座標
 *	@param	main_target_pos [in]	メインカメラtarget座標
 *	@param	dest_disp_pos	[out]	追加背景用座標
 *	@param	dest_target_pos	[out]	追加背景用座標
 *	@param	camera_id		[in]	カメラID
 */
// ==========================================================================
#if 1
void GmMapGetAddMapCameraPos(const NNS_VECTOR *main_disp_pos, const NNS_VECTOR *main_target_pos,
				NNS_VECTOR *dest_disp_pos, NNS_VECTOR *dest_target_pos, s32 camera_id)
{
	s32							i;
	GMS_MAP_SYS_WORK			*map_sys_work;
	float						limit_left, limit_right, limit_top, limit_bottom;
	float						cam_disp_width_half, cam_disp_height_half;
	GMS_MAP_OTHER_MAP_STATE		*map_state;
	NNS_VECTOR					main_camera_pos[2];
	NNS_VECTOR					**dest_camera_pos[2];

	MTM_ASSERT(main_disp_pos);
	MTM_ASSERT(main_target_pos);
	MTM_ASSERT(dest_disp_pos);
	MTM_ASSERT(dest_target_pos);

	if (gm_map_tcb == NULL) {
		MTM_ASSERT(!"gmMap.cpp:GmMapGetAddMapCameraPos() Error map sys no init\n");
		return;
	}
	map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);

	// カメラをワークに設定
	main_camera_pos[0] = *main_disp_pos;
	main_camera_pos[1] = *main_target_pos;
	dest_camera_pos[0] = &dest_disp_pos;
	dest_camera_pos[1] = &dest_target_pos;
	if (map_sys_work->flag & GMD_MAP_FLAG_ADD_MAP_MAIN_ADD_SCRL_H) {
		// メインカメラ位置ユーザースクロール対応
		main_camera_pos[0].x = map_sys_work->main_cam_user_disp[MTD_X] + map_sys_work->main_cam_user_ofst[MTD_X];
		main_camera_pos[1].x = map_sys_work->main_cam_user_target[MTD_X] + map_sys_work->main_cam_user_ofst[MTD_X];
	}
#if !_IPHONE
	cam_disp_width_half	= (float)(GSD_DISP_WIDTH/2) * GMD_CAMERA_SCALE;
	cam_disp_height_half= (float)(GSD_DISP_HEIGHT/2) * GMD_CAMERA_SCALE;
#else //!_IPHONE
	cam_disp_width_half	= (float)(AMD_SCREEN_2D_WIDTH/2) * GMD_CAMERA_SCALE;
	cam_disp_height_half= (float)(AMD_SCREEN_2D_HEIGHT/2) * GMD_CAMERA_SCALE;
#endif //!_IPHONE
	// 限界位置
	limit_left		= 0 + cam_disp_width_half;
	limit_right		= g_gm_main_system.map_size[MTD_X] - cam_disp_width_half;
	limit_top		= 0 + cam_disp_height_half;
	limit_bottom	= g_gm_main_system.map_size[MTD_Y] - cam_disp_height_half;


	if (camera_id == GME_CAMERA_NO_SNEAR) {
		map_state = &map_sys_work->map_state[0];
	}
	else if (camera_id >= GME_CAMERA_NO_MID1) {
		map_state = &map_sys_work->map_state[(camera_id - GME_CAMERA_NO_MID1) + 2/*超近景と中景を飛ばす*/];
	}
	else {
		MTM_ASSERT(0);
		map_state = &map_sys_work->map_state[0];	// 仮に0番にしておく
	}

	for (i = 0; i < 2; i++) {
		if (!(map_sys_work->flag & GMD_MAP_FLAG_ADD_MAP_LOOP_X)) {
			// X
			if (main_camera_pos[i].x <= limit_left) {
				// 限界位置
				(*dest_camera_pos[i])->x = cam_disp_width_half;
			}
			else if (main_camera_pos[i].x >= limit_right) {
				// 限界位置
				(*dest_camera_pos[i])->x = map_state->map_size[MTD_X] - cam_disp_width_half;
			}
			else {
				(*dest_camera_pos[i])->x = cam_disp_width_half +
							(main_camera_pos[i].x - cam_disp_width_half) * map_state->scrl_scale[MTD_X];
			}
		}
		else {
			// ループあり
			//(*dest_camera_pos[i])->x = cam_disp_width_half +
			//			(main_camera_pos[i].x - cam_disp_width_half) * map_state->scrl_scale[MTD_X];
			(*dest_camera_pos[i])->x = main_camera_pos[i].x * map_state->scrl_scale[MTD_X];
		}

		// ユーザースクロール後のオフセット値反映
		(*dest_camera_pos[i])->x += map_state->cam_ofst[MTD_X];

		{
			// Y
			if (-main_camera_pos[i].y <= limit_top) {
				// 限界位置
				(*dest_camera_pos[i])->y = -cam_disp_height_half;
			}
			else if (-main_camera_pos[i].y >= limit_bottom) {
				// 限界位置
				(*dest_camera_pos[i])->y = -(map_state->map_size[MTD_Y] - cam_disp_height_half);
			}
			else {
				(*dest_camera_pos[i])->y = -(cam_disp_height_half +
							(-main_camera_pos[i].y - cam_disp_height_half) * map_state->scrl_scale[MTD_Y]);
			}
		}

		//// ユーザースクロール後のオフセット値反映
		//(*dest_camera_pos[i])->y += map_state->cam_ofst[MTD_Y];

		// Z
		(*dest_camera_pos[i])->z = main_camera_pos[i].z;
	}
}

#else
void GmMapGetAddMapCameraPos(const NNS_VECTOR *main_camera_pos, NNS_VECTOR *dest_camera, s32 camera_id)
{
	GMS_MAP_SYS_WORK			*map_sys_work;
	float						limit_left, limit_right, limit_top, limit_bottom;
	float						cam_disp_width_half, cam_disp_height_half;
	GMS_MAP_OTHER_MAP_STATE		*map_state;

	MTM_ASSERT(main_camera_pos);
	MTM_ASSERT(dest_camera);

	if (gm_map_tcb == NULL) {
		MTM_ASSERT(!"gmMap.cpp:GmMapGetAddMapCameraPos() Error map sys no init\n");
		return;
	}

	cam_disp_width_half	= (float)(GSD_DISP_WIDTH/2) * GMD_CAMERA_SCALE;
	cam_disp_height_half= (float)(GSD_DISP_HEIGHT/2) * GMD_CAMERA_SCALE;

	// 限界位置
#if 1
	limit_left		= 0 + cam_disp_width_half;
	limit_right		= g_gm_main_system.map_size[MTD_X] - cam_disp_width_half;
	limit_top		= 0 + cam_disp_height_half;
	limit_bottom	= g_gm_main_system.map_size[MTD_Y] - cam_disp_height_half;
#else
	limit_left		= g_gm_main_system.map_fcol.left + cam_disp_width_half;
	limit_right		= g_gm_main_system.map_fcol.right - cam_disp_width_half;
	limit_top		= g_gm_main_system.map_fcol.top + cam_disp_height_half;
	limit_bottom	= g_gm_main_system.map_fcol.bottom - cam_disp_height_half;
#endif

	map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);

	if (camera_id == GME_CAMERA_NO_SNEAR) {
		map_state = &map_sys_work->map_state[0];
	}
	else if (camera_id >= GME_CAMERA_NO_MID1) {
		map_state = &map_sys_work->map_state[(camera_id - GME_CAMERA_NO_MID1) + 2/*超近景と中景を飛ばす*/];
	}

//	if (map_sys_work->flag & GMD_MAP_FLAG_ADD_MAP_1) {
		// X
		if (main_camera_pos->x <= limit_left) {
			// 限界位置
			dest_camera->x = cam_disp_width_half;
		}
		else if (main_camera_pos->x >= limit_right) {
			// 限界位置
			dest_camera->x = map_state->map_size[MTD_X] - cam_disp_width_half;
		}
		else {
			dest_camera->x = cam_disp_width_half +
						(main_camera_pos->x - cam_disp_width_half) * map_state->scrl_scale[MTD_X];
		}
		// Y
		if (-main_camera_pos->y <= limit_top) {
			// 限界位置
			dest_camera->y = -cam_disp_height_half;
		}
		else if (-main_camera_pos->y >= limit_bottom) {
			// 限界位置
			dest_camera->y = -(map_state->map_size[MTD_Y] - cam_disp_height_half);
		}
		else {
			dest_camera->y = -(cam_disp_height_half +
						(-main_camera_pos->y - cam_disp_height_half) * map_state->scrl_scale[MTD_Y]);
		}
		dest_camera->z = main_camera_pos->z;
//	}
//	else {
//		// 超近景なしなので、メインカメラを返しておく
//		*dest_camera = *main_camera_pos;
//	}
}
#endif

// ==========================================================================
// GmMapGetAddMapCameraPos
/*!
 *	メインカメラから追加背景用カメラ位置取得
 *
 *	@param	main_camera_pos [in]	メインカメラ座標
 *	@param	dest_camera		[out]	超近景用座標
 */
// ==========================================================================
#if 0
void GmMapGetAddMapCameraPos(const NNS_VECTOR *main_camera_pos, NNS_VECTOR *dest_camera, s32 camera_no)
{
	GMS_MAP_SYS_WORK	*map_sys_work;
	float				limit_left, limit_right, limit_top, limit_bottom;
	float				cam_disp_width_half, cam_disp_height_half;

	MTM_ASSERT(main_camera_pos);
	MTM_ASSERT(dest_camera);

	if (gm_map_tcb == NULL) {
		MTM_ASSERT(!"gmMap.cpp:GmMapGetAddMapCameraPos() Error map sys no init\n");
		return;
	}

	cam_disp_width_half	= (float)(GSD_DISP_WIDTH/2) * GMD_CAMERA_SCALE;
	cam_disp_height_half= (float)(GSD_DISP_HEIGHT/2) * GMD_CAMERA_SCALE;

	// 限界位置
#if 1
	limit_left		= 0 + cam_disp_width_half;
	limit_right		= g_gm_main_system.map_size[MTD_X] - cam_disp_width_half;
	limit_top		= 0 + cam_disp_height_half;
	limit_bottom	= g_gm_main_system.map_size[MTD_Y] - cam_disp_height_half;
#else
	limit_left		= g_gm_main_system.map_fcol.left + cam_disp_width_half;
	limit_right		= g_gm_main_system.map_fcol.right - cam_disp_width_half;
	limit_top		= g_gm_main_system.map_fcol.top + cam_disp_height_half;
	limit_bottom	= g_gm_main_system.map_fcol.bottom - cam_disp_height_half;
#endif

	map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);

	if (map_sys_work->flag & GMD_MAP_FLAG_ADD_MAP_1) {
		// X
		if (main_camera_pos->x <= limit_left) {
			// 限界位置
			dest_camera->x = cam_disp_width_half;
		}
		else if (main_camera_pos->x >= limit_right) {
			// 限界位置
			dest_camera->x = map_sys_work->snear_map_size[MTD_X] - cam_disp_width_half;
		}
		else {
			dest_camera->x = cam_disp_width_half +
						(main_camera_pos->x - cam_disp_width_half) * map_sys_work->snear_scrl_scale[MTD_X];
		}
		// Y
		if (-main_camera_pos->y <= limit_top) {
			// 限界位置
			dest_camera->y = -cam_disp_height_half;
		}
		else if (-main_camera_pos->y >= limit_bottom) {
			// 限界位置
			dest_camera->y = -(map_sys_work->snear_map_size[MTD_Y] - cam_disp_height_half);
		}
		else {
			dest_camera->y = -(cam_disp_height_half +
						(-main_camera_pos->y - cam_disp_height_half) * map_sys_work->snear_scrl_scale[MTD_Y]);
		}
		dest_camera->z = main_camera_pos->z;
	}
	else {
		// 超近景なしなので、メインカメラを返しておく
		*dest_camera = *main_camera_pos;
	}
}
#endif

// ==========================================================================
// GmMapSetDispB
/*!
 *	マップ表示設定
 *
 *	@param	disp	[in]	表示設定 TRUE : 表示  FALSE : 非表示
 */
// ==========================================================================
void GmMapSetDispB(BOOL disp)
{
	GMS_MAP_SYS_WORK	*map_sys_work;

	if (gm_map_tcb) {
		map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);

		if (disp) {
			map_sys_work->flag &= ~GMD_MAP_FLAG_DISP_OFF_B;
		}
		else {
			map_sys_work->flag |= GMD_MAP_FLAG_DISP_OFF_B;
		}
	}
}

// ==========================================================================
// GmMapSetDisp
/*!
 *	マップ全体表示設定
 *
 *	@param	disp	[in]	表示設定 TRUE : 表示  FALSE : 非表示
 */
// ==========================================================================
void GmMapSetDisp(BOOL disp)
{
	GMS_MAP_SYS_WORK	*map_sys_work;

	if (gm_map_tcb) {
		map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(gm_map_tcb);

		if (disp) {
			map_sys_work->flag &= ~GMD_MAP_FLAG_DISP_OFF;
		}
		else {
			map_sys_work->flag |= GMD_MAP_FLAG_DISP_OFF;
		}
	}
}

// ==========================================================================
// GmMapIsDrawEnableMMapBack
/*!
 *	M面以降を描画するか否か
 *
 *  @return TRUE:描画する　FALSE:しない
 *
 *  @note メインカメラ位置から描画するか否かを判定します。
 */
// ==========================================================================
BOOL GmMapIsDrawEnableMMapBack(void)
{
	BOOL flag = TRUE;
	float x, y;
	OBS_CAMERA* camera = ObjCameraGet(g_obj.glb_camera_id);
	x = camera->disp_pos.x;
	y = -camera->disp_pos.y;
	
	// ゾーン-アクト毎に描画を制限する区域を設定
	GSE_MAIN_STAGE_ID stage_id = (GSE_MAIN_STAGE_ID)g_gs_main_sys_info.stage_id;
	switch (stage_id) {
			// 1-1
		case GSD_MAIN_STAGE_ID_1_1:
			if (GMM_MAP_IS_RANGE(x, 5450.0f, 5725.0f) && GMM_MAP_IS_RANGE(y, 1010.0f, 1520.0f)) {
				flag = FALSE;
			}
			else if (GMM_MAP_IS_RANGE(x, 8010.0f, 8500.0f) && GMM_MAP_IS_RANGE(y, 1200.0f, 1650.0f)) {
				flag = FALSE;
			}
			else if (GMM_MAP_IS_RANGE(x, 10055.0f, 10695.0f) && GMM_MAP_IS_RANGE(y, 1025.0f, 1400.0f)) {
				flag = FALSE;
			}
			break;
			
			// 1-2
		case GSD_MAIN_STAGE_ID_1_2:
			if (GMM_MAP_IS_RANGE(x, 3975.0f, 4650.0f) && GMM_MAP_IS_RANGE(y, 1555.0f, 2200.0f)) {
				flag = FALSE;
			}
			else if (x > 12415.0f) {
				flag = FALSE;
			}
			break;
		
			// Final
		case GSD_MAIN_STAGE_ID_FINAL_1:
			if (x < 2450.0f) {
				flag = FALSE;
			}
			else if (GMM_MAP_IS_RANGE(x, 3020.0f, 5600.0f)) {
				flag = FALSE;
			}
			else if (GMM_MAP_IS_RANGE(x, 6590.0f, 9200.0f)) {
				flag = FALSE;
			}
			break;
			
			// Other
		default:
			break;
	}
	
	return flag;
}

// ==========================================================================
// ライトセット
// ==========================================================================
// ==========================================================================
// GmMapSetLight
/*!
 *	マップライト設定 (NNE_LIGHT_5 を使用)
 */
// ==========================================================================
void GmMapSetLight(void)
{
	NNS_RGBA	light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};
	NNS_VECTOR	light_vec = {
		1.0f, 1.0f, 1.0f,
	};
	float		intensity = 1.f;

	// ここで指定のステージ以外は NNE_LIGHT_0 を使用
	// 対応ステージを増やす場合は、GmMapBuildDataLoop でのライト設定を修正する事
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_1) {
#if _WII
		if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_1 ||
				g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_1_2) {
			// ZONE1-1, ZONE1-2
			light_vec.x = 0.f;
			light_vec.y = 0.f;
			light_vec.z = -1.0f;
		}
		else {
			// ZONE1-3
			light_vec.x = -1.0f;
			light_vec.y = -1.0f;
			light_vec.z = -1.0f;
		}
#else
		light_vec.x = -1.0f;
		light_vec.y = -1.0f;
		light_vec.z = -1.0f;
#endif
		light_col.r = 1.0f;
		light_col.g = 1.0f;
		light_col.b = 1.0f;
		light_col.a = 1.0f;
		intensity = 1.0f;
	}
	else if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_4) {
		light_vec.x = -0.2f;
		light_vec.y = 0.25f;
		light_vec.z = -1.0f;
		light_col.r = 1.0f;
		light_col.g = 1.0f;
		light_col.b = 1.0f;
		light_col.a = 1.0f;
		if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
			// Zone4-3のみ暗く
			intensity = GMD_LIGHT_MAP_DARK_INTENSITY;
		}
		else {
			intensity = GMD_LIGHT_COMN_INTENSITY;
		}
	}
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_5, &light_col, intensity, &light_vec);

	
	// EXライト
#if _WII
	if (GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_4) {
		light_vec.x = -0.1f;	// 調整対象◆
		light_vec.y = 0.15f;
		light_vec.z = -1.0f;
		light_col.r = 1.0f;
		light_col.g = 1.0f;
		light_col.b = 1.0f;
		light_col.a = 1.0f;
		if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_4_3) {
			// Zone4-3のみ暗く
			intensity = GMD_LIGHT_EX_MAP_DARK_INTENSITY;	// 調整対象◆
		}
		else {
			intensity = GMD_LIGHT_COMN_INTENSITY;
		}
	}
	else {
		light_vec.x = -1.0f;
		light_vec.y = -1.0f;
		light_vec.z = -1.0f;
		light_col.r = 1.0f;
		light_col.g = 1.0f;
		light_col.b = 1.0f;
		light_col.a = 1.0f;
		intensity = GMD_LIGHT_COMN_INTENSITY;
	}
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_4, &light_col, intensity, &light_vec);
#endif
}

#if _IPHONE
// ==========================================================================
// GmMapSetMapDrawSize
/*!
 *	マップの生成範囲を設定する
 *
 * @param size GME_MAP_DRAW_SIZEで定義される値
 */
// ==========================================================================
void GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE size)
{
	gm_map_draw_size[MTD_X] = gm_map_set_draw_size[size][MTD_X];
	gm_map_draw_size[MTD_Y] = gm_map_set_draw_size[size][MTD_Y];
}
#endif // _IPHONE

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmMapDest
/*!
 *	マップデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmMapDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	gm_map_tcb = NULL;
}

#if _IPHONE
// ==========================================================================
// gmMapMain
/*!
 *	マップメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmMapMain(MTS_TASK_TCB *tcb)
{
	s32					i, model_num;
	GMS_MAP_SYS_WORK	*map_sys_work;
	GMS_MAP_OTHER_MAP_STATE		*map_state;
	OBS_CAMERA			*camera;
	float				pos_x, pos_y;
	BOOL				loop_h;

	void				*tvxamb;
	NNS_TEXLIST			*texlist;
	GSE_MAIN_ZONE_TYPE  zone_no = GMM_MAIN_GET_ZONE_TYPE();

	// SSはマップを使わないので終了
	if (zone_no == GSD_MAIN_ZONE_TYPE_SS) {
		// 処理的にココでBGM再生対応を行わざるを得ない
		if (g_gm_main_system.game_flag & GMD_GAME_FLAG_BGM_PLAY_WAIT) {
			if (--gm_map_draw_bgm_timer <= 0) {
				g_gm_main_system.game_flag	|= GMD_GAME_FLAG_BGM_PLAY_ENABLE;
				g_gm_main_system.game_flag	&= ~GMD_GAME_FLAG_BGM_PLAY_WAIT;
			}
		}
		return;
	}

#if defined(AMD_DEBUG)
	static bool is_nodisp_map = false;
	static bool is_nodisp_mid_near = false;
#if 01
	if (g_gm_main_system.debug_flag & GMD_GAME_DEBUG_FLAG_DEBUG_PAUSE) {
		if (PAD_STAND(0) & KEY_L_LEFT) {
			is_nodisp_map = !is_nodisp_map;
		}
		if (PAD_STAND(0) & KEY_L_RIGHT) {
			is_nodisp_mid_near = !is_nodisp_mid_near;
		}
		//ちょっと間借り
		if (PAD_STAND(0) & KEY_L_UP) {
			g_gm_main_system.debug_flag ^= GMD_DEBUG_FLAG_NODISP_FAR;
		}
		if (PAD_STAND(0) & KEY_L_DOWN) {
			g_gm_main_system.debug_flag ^= GMD_DEBUG_FLAG_NODISP_FIX;
		}
	}
#endif // 01

	int start = 0;
	int end   = gm_map_add_tbl_use_no[zone_no] + 1;
	if (is_nodisp_mid_near) {
		start = 1;
		end = 2;
	}
#else //defined(AMD_DEBUG)
	int start = 0;
	int end   = gm_map_add_tbl_use_no[zone_no] + 1;
#endif //defined(AMD_DEBUG)

	map_sys_work = (GMS_MAP_SYS_WORK*)mtTaskGetTcbWork(tcb);

	if (map_sys_work->flag & GMD_MAP_FLAG_DISP_OFF) {
		// マップ描画OFF
		return;
	}

	// モーションアップデート
	if (!ObjObjectPauseCheck(0)) {
		gmMapUpdateDrawMapTvxTexScroll();
	}
	
	// モーション更新後に終了判定
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	// 処理的にココでBGM再生対応を行わざるを得ない
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_BGM_PLAY_WAIT) {
		if (--gm_map_draw_bgm_timer <= 0) {
			g_gm_main_system.game_flag	|= GMD_GAME_FLAG_BGM_PLAY_ENABLE;
			g_gm_main_system.game_flag	&= ~GMD_GAME_FLAG_BGM_PLAY_WAIT;
		}
	}
	
	tvxamb  = g_gm_gamedat_map[GMD_GAMEDAT_MAP_MODEL];
	texlist = AoTexGetTexList(&gm_map_texture);
	
	NNS_MATRIX mtx;
	nnMakeUnitMatrix(&mtx);

	// カメラ角度チェック
	if (map_sys_work->auto_resize) {
		camera = ObjCameraGet(g_obj.glb_camera_id);
		if (camera->roll & 0x3ffff) {
			GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_DEF);
		}
		else {
			if (camera->roll & 0x4000) {
				GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_VERT);
			}
			else {
				GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_HORI);
			}
		}	
	}
#ifdef MTD_DEBUG
	//if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
		//amPrintf(3, 3, "X:%d Y:%d", gm_map_draw_size[MTD_X], gm_map_draw_size[MTD_Y]);
	//}
#endif // MTD_DEBUG
	
	// 追加背景
	map_state = map_sys_work->map_state;
	for (i = end - 1; i >= start/*GME_MAP_ADD_MAP_MAX*/; i--) {
//	for (i = start; i < end/*GME_MAP_ADD_MAP_MAX*/; i++, map_state++) {
		if (!(map_sys_work->flag & (GMD_MAP_FLAG_ADD_MAP_1 << i))) {
			continue;
		}
		if (i >= GME_MAP_ADD_MAP_MID1)
		{
			if (!GmMapIsDrawEnableMMapBack()) {
				continue;
			}
		}
#if defined(AMD_DEBUG)
		if (is_nodisp_map) {
			if (i == GME_MAP_ADD_MAP_MID) {
				continue;
			}
		}
#endif //defined(AMD_DEBUG)
	
		// ループ描画設定
		loop_h = FALSE;
		if (i != GMD_GAMEDAT_MAPSET_ADD_LOCAL_M_MP/2 &&		// 通常中景は除く
				map_sys_work->flag & GMD_MAP_FLAG_ADD_MAP_LOOP_X) {
			loop_h = TRUE;
		}
	
		// カメラセット
		//ObjDraw3DNNSetCamera(gm_map_addmap_camera_tbl[i], NNE_PROJECTION_TYPE_ORTHO);
		ObjDraw3DNNSetCameraEx(gm_map_addmap_camera_tbl[i], NNE_PROJECTION_TYPE_ORTHO, map_state[i].command_state);
		camera = ObjCameraGet(gm_map_addmap_camera_tbl[i]);
	
		pos_x = camera->disp_pos.x;
		pos_y = -camera->disp_pos.y;
	
		// マップ描画コマンドステート設定
		GmMapSetDrawState(map_state[i].command_state);
	
		MP_HEADER *mp_header = (MP_HEADER*)g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_N_MP + i * 2];
		MD_HEADER *md_header = (MD_HEADER*)g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_N_MD + i * 2];
		
		// 頂点初期化
		gmMapInitDrawMapTvx();
		gmMapSetDrawMapTvx(tvxamb, mp_header, md_header, pos_x, pos_y, 0, 0, map_state[i].pos_z, loop_h);
		// 頂点描画実行
		gmMapExecuteDrawMapTvx(&mtx, texlist);
	}
		
	// マップ描画コマンドステート復帰
	GmMapSetDrawState(OBD_DRAW_CMD_STATE_3DNN);

	// カメラ設定
	ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, g_obj.glb_camera_type, OBD_DRAW_CMD_STATE_3DNN);
	
	camera = ObjCameraGet(g_obj.glb_camera_id);
	pos_x = camera->disp_pos.x;
	pos_y = -camera->disp_pos.y;
			
#if defined(AMD_DEBUG)
	if (!is_nodisp_map) {
		//}
#endif //defined(AMD_DEBUG)
	// 頂点初期化
	gmMapInitDrawMapTvx();
		
	// B面頂点登録
	if (!(map_sys_work->flag & GMD_MAP_FLAG_DISP_OFF_B)) {
		gmMapSetDrawMapTvx(tvxamb, (MP_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_B_MP],
					(MD_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_B_MD],
					pos_x, pos_y, 0, 0, GMD_MAP_B_POS_Z);
	}
		
	// A面頂点登録
	gmMapSetDrawMapTvx(tvxamb, (MP_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_A_MP],
				(MD_HEADER*)g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_A_MD],
				pos_x, pos_y, 0, 0, GMD_MAP_A_POS_Z);
	
	// 頂点描画実行
	gmMapExecuteDrawMapTvx(&mtx, texlist);
#if defined(AMD_DEBUG)
	//if (!is_nodisp_map) {
	}
#endif //defined(AMD_DEBUG)
	
#if OBD_LOAD_INITIAL_DRAW
	if (gm_map_tex_draw_count > 0/* && !(g_gm_main_system.game_flag & GMD_GAME_FLAG_START_DEMO)*/)
	{
		--gm_map_tex_draw_count;
		ObjLoadInitDraw();
		if (gm_map_tex_draw_count == 0) {
			ObjLoadClearDraw();
		}
		for (int i = 0; i < gm_map_texture.texlist->nTex; i++) {
			AMS_PARAM_DRAW_PRIMITIVE dat;
			
			dat.type = NNE_PRIM_TRIANGLE_LIST;
			dat.ablend = NNE_PRIM_ALPHABLEND_ON;
			// アルファブレンド設定
#if defined(_PC) | defined(_XBOX)
			dat.bldSrc = NNE_BLENDMODE_SRCALPHA;
			dat.bldDst = NNE_BLENDMODE_INVSRCALPHA;
			dat.bldMode = NNE_BLENDOP_ADD;
#else
			dat.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
			dat.bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
			dat.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
			// テスト設定
			dat.aTest = 1;
			dat.zMask = 0;
			dat.zTest = 1;
			
			// ソートしない
			dat.noSort = 1;
			
			// テクスチャクランプ設定
			dat.uwrap = NNE_PRIM_TEXWRAP_CLAMP;
			dat.vwrap = NNE_PRIM_TEXWRAP_CLAMP;
			
			dat.format3D = NNE_PRIM3D_FMT_PCT;
			
			// テクスチャ設定
			dat.texlist = texlist;
			
			// 描画
			NNS_PRIM3D_PCT* v_tbl = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer((s32)(sizeof(NNS_PRIM3D_PCT) * 4));
			dat.vtxPCT3D = v_tbl;
			dat.count = 4;
			dat.texId = i;
			
			v_tbl[0].Pos.x = v_tbl[1].Pos.x = -1.0f;
			v_tbl[2].Pos.x = v_tbl[3].Pos.x =  1.0f;
			v_tbl[0].Pos.y = v_tbl[2].Pos.y = -1.0f;
			v_tbl[1].Pos.y = v_tbl[3].Pos.y =  1.0f;
			v_tbl[0].Pos.z = v_tbl[1].Pos.z = v_tbl[2].Pos.z = v_tbl[3].Pos.z = -1.0f;
			v_tbl[0].Tex.u = v_tbl[1].Tex.u = 0.0f;
			v_tbl[2].Tex.u = v_tbl[3].Tex.u = 1.0f;
			v_tbl[0].Tex.v = v_tbl[2].Tex.v = 0.0f;
			v_tbl[1].Tex.v = v_tbl[3].Tex.v = 1.0f;
			v_tbl[0].Col = v_tbl[1].Col = v_tbl[2].Col = v_tbl[3].Col = 0xffffffff;
			
			ObjDraw3DNNDrawPrimitive(&dat, gm_map_draw_command_state, NNE_PRIM_LIGHT_DISABLE, NNE_PRIM_CULL_NONE);
		}
	}
	else {
	//	ObjLoadClearDraw();
	}
//	printf("\n");
#endif // OBD_LOAD_INITIAL_DRAW

	// カメラ復旧は最後に使うカメラが同じなので不要
//	ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, g_obj.glb_camera_type, OBD_DRAW_CMD_STATE_3DNN);
}
#else
#endif // _IPHONE

#if _IPHONE
// ==========================================================================
// gmMapInitDrawMapTvx
/*!
 *	マップ描画 tvx使用 初期化
 *
 */
// ==========================================================================
void gmMapInitDrawMapTvx(void)
{
	int i;
	
	// スタック初期化
	GMS_MAP_PRIM_DRAW_WORK *p_work = gm_map_prim_draw_work;	// プリミティブ描画ワーク
	for (i = 0; i < GMD_MAP_PRIM_DRAW_WORK_NUM; i++) {
		p_work->tex_id      = -1;
		p_work->all_vtx_num = 0;
		p_work->stack_num   = 0;
		p_work++;
	}
}
#endif //_IPHONE

#if _IPHONE
// ==========================================================================
// gmMapDrawMapTvx
/*!
 *	マップ描画 tvx使用 行列設定
 *
 *	@param	tvxamb		[in]	描画するマップデータ(AMBアーカイブ)
 *	@param	mp_header	[in]	描画するマップ MPデータ
 *	@param	md_header	[in]	描画するマップ MDデータ
 *	@param	pos_x		[in]	描画領域 左上基点 X (2D座標系)
 *	@param	pos_y		[in]	描画領域 左上基点 Y (2D座標系)
 *	@param	trans_x		[in]	移動量
 *	@param	trans_y		[in]	移動量 (2D座標系)
 *	@param	trans_z		[in]	移動量
 *	@param	loop_h		[in]	横描画領域がはみ出した時にループ描画する
 */
// ==========================================================================
void gmMapSetDrawMapTvx(void *tvxamb, MP_HEADER *mp_header, MD_HEADER *md_header, float pos_x, float pos_y,
							float trans_x, float trans_y, float trans_z, BOOL loop_h/* = FALSE*/)
{
	s32	block_x_center, block_y_center;
	//s32	block_x, block_y;
	s32	block_left, block_right, block_top, block_bottom;
	s32	block_width, block_height;
	float	draw_ofst_x_0 = 0.f, draw_ofst_x_1 = 0.f;

	// 描画範囲取得
	block_x_center = ((s32)pos_x) >> (3 + 3);	// > char > block
	block_y_center = ((s32)pos_y) >> (3 + 3);

	block_width = GMD_MAP_DRAW_WIDTH + GMD_MAP_DRAW_MARGIN * 2 + gm_map_draw_margin_adjust * 2;
	block_height= GMD_MAP_DRAW_HEIGHT + GMD_MAP_DRAW_MARGIN * 2 + gm_map_draw_margin_adjust * 2;

	block_left		= block_x_center - (block_width >> 1);
	block_right		= block_x_center + (block_width >> 1);
	block_top		= block_y_center - (block_height >> 1);
	block_bottom	= block_y_center + (block_height >> 1);

#if 0
	if (!loop_h && block_left < 0) {
		block_left = 0;
	}
#else
	if (block_left < 0) {
		if (!loop_h) {
			block_left = 0;
		}
		else {
			block_left += mp_header->map_w;
			draw_ofst_x_0 = (float)-(mp_header->map_w << (3 + 3));
		}
	}
	else if (block_left >= mp_header->map_w) {
		if (!loop_h) {
			block_left = mp_header->map_w - block_width;
		}
		else {
			block_left -= mp_header->map_w;
			draw_ofst_x_0 = (float)+(mp_header->map_w << (3 + 3));
		}
	}
#endif
	if (block_top < 0) {
		block_top = 0;
	}

#if 0
	if (!loop_h && block_right >= mp_header->map_w) {
		block_right = mp_header->map_w - 1;
	}
#else
	if (block_right >= mp_header->map_w) {
		if (!loop_h) {
			block_right = mp_header->map_w - 1;
		}
		else {
			block_right -= mp_header->map_w;
			draw_ofst_x_1 = (float)+(mp_header->map_w << (3 + 3));
		}
	}
	else if (block_right < 0) {
		if (!loop_h) {
			block_right = block_width;
		}
		else {
			block_right += mp_header->map_w;
			draw_ofst_x_1 = (float)-(mp_header->map_w << (3 + 3));
		}
	}
#endif
	if (block_bottom >= mp_header->map_h) {
		block_bottom = mp_header->map_h - 1;
	}

	if (block_left < block_right) {
		// 通常描画
		if (loop_h) {
			gmMapSetDrawMapRangeTvx(tvxamb, mp_header, md_header,
					  trans_x + draw_ofst_x_0, trans_y, trans_z, block_left, block_right, block_top, block_bottom);
		}
		else {
			gmMapSetDrawMapRangeTvx(tvxamb, mp_header, md_header,
					  trans_x, trans_y, trans_z, block_left, block_right, block_top, block_bottom);
		}
	}
	else {
		// 横ループ描画
		gmMapSetDrawMapRangeTvx(tvxamb, mp_header, md_header,
				  trans_x + draw_ofst_x_0, trans_y, trans_z,
				  block_left, mp_header->map_w - 1, block_top, block_bottom);

		gmMapSetDrawMapRangeTvx(tvxamb, mp_header, md_header,
				  trans_x + draw_ofst_x_1, trans_y, trans_z,
				  0, block_right, block_top, block_bottom);
	}
}
#endif //_IPHONE

#if _IPHONE
// ==========================================================================
// gmMapDrawMapRangeTvx
/*!
 *	マップ描画
 *
 *	@param	tvxamb			[in]	描画するマップデータ(AMBアーカイブ)
 *	@param	mp_header		[in]	描画するマップ MPデータ
 *	@param	md_header		[in]	描画するマップ MDデータ
 *	@param	trans_x			[in]	移動量
 *	@param	trans_y			[in]	移動量 (2D座標系)
 *	@param	trans_z			[in]	移動量
 *	@param	block_left		[in]	描画開始ブロック 左
 *	@param	block_right		[in]	描画開始ブロック 右
 *	@param	block_top		[in]	描画開始ブロック 上
 *	@param	block_bottom	[in]	描画開始ブロック 下
 */
// ==========================================================================
void gmMapSetDrawMapRangeTvx(void *tvxamb, MP_HEADER *mp_header, MD_HEADER *md_header,
				  float trans_x, float trans_y, float trans_z,
				  s32 block_left, s32 block_right, s32 block_top, s32 block_bottom)
{
	s32	i, x_cnt, y_cnt;
	s32	block_width, block_height;
	u32 num, color;
	MP_BLOCK	*mp_block_top, *mp_block;
	MD_BLOCK	*md_block_top, *md_block;

	MTM_ASSERT(block_left <= block_right);
	MTM_ASSERT(block_top <= block_bottom);

#if defined (GMD_MAP_DEBUG_NO_DRAW)
	return;
#endif

	// 描画チェックバッファクリア
	s16		*p_check;
	for (p_check = &gm_map_block_check[0][0], i = 0; i < GMD_MAP_DRAW_CHECK_WIDTH*GMD_MAP_DRAW_CHECK_HEIGHT; i++, p_check++) {
		*p_check = -1;
	}

	block_width = GMD_MAP_DRAW_WIDTH + GMD_MAP_DRAW_MARGIN * 2;
	block_height= GMD_MAP_DRAW_HEIGHT + GMD_MAP_DRAW_MARGIN * 2;

	mp_block_top = (MP_BLOCK*)(mp_header + 1);
	md_block_top = (MD_BLOCK*)(md_header + 1);
	
	// 描画
	void			*obj_vtx_file;
	u32				tex_num;
	s32				tex_id;
	u32				vtx_num;
	GMS_MAP_PRIM_DRAW_WORK *work = gm_map_prim_draw_work;	// プリミティブ描画ワークs
	GMS_MAP_PRIM_DRAW_STACK *stack;

	s32			obj_id;				// 描画オブジェクトID
	s32			block_no;
	float		dx, dy;				// ブロック単位描画位置
	s32			ofst_x, ofst_y;		// ブロックID参照位置オフセット
	
	// 頂点数取得
	vtx_num = 0;
	for (x_cnt = block_left; x_cnt <= block_right; x_cnt++) {
		for (y_cnt = block_top; y_cnt <= block_bottom; y_cnt++) {
			block_no = y_cnt * mp_header->map_w + x_cnt;
			mp_block = mp_block_top + block_no;

			// ブロック単位描画位置
			dx = (float)x_cnt;
			dy = (float)y_cnt;

			// 表示オブジェクトを求める
			obj_id	= mp_block->id;
			if (obj_id == 0) {

				md_block	= md_block_top + block_no;
				ofst_x	= (s32)md_block->ofst_x;
				ofst_y	= (s32)md_block->ofst_y;
				if ((ofst_x | ofst_y) == 0) {
					// ブロック無し
					continue;
				}
				block_no += mp_header->map_w * ofst_y + ofst_x;
				mp_block = mp_block_top + block_no;
				obj_id	= mp_block->id;
				dx		+= ofst_x;	// ブロック単位描画位置更新
				dy		+= ofst_y;
			}

			// 保険
			if (obj_id == 0) {
				continue;
			}
			// ブロック描画済みチェック
			if (gm_map_block_check[GMD_MAP_DRAW_DF_SIZE + (s32)dx - block_left][GMD_MAP_DRAW_DF_SIZE + (s32)dy - block_top] != -1) {
				// 既に描画済み

				// 同じIDになっていない？
				MTM_ASSERT(gm_map_block_check[GMD_MAP_DRAW_DF_SIZE + (s32)dx - block_left][GMD_MAP_DRAW_DF_SIZE + (s32)dy - block_top] == obj_id);

				continue;
			}
			gm_map_block_check[GMD_MAP_DRAW_DF_SIZE + (s32)dx - block_left][GMD_MAP_DRAW_DF_SIZE + (s32)dy - block_top] = (s16)obj_id;

			// 描画オブジェクト取得
			--obj_id; // 1→0でアクセスする為に減算
			obj_vtx_file = amBindGet((AMS_AMB_HEADER*)tvxamb, obj_id);

			MTM_ASSERT(AoTvxIsTvxFile(obj_vtx_file));

			tex_num = AoTvxGetTextureNum(obj_vtx_file);

			// 頂点処理
			for (num = 0; num < tex_num; num++) {
				MTM_ASSERT(AOD_TVX_PRIMTYPE_TRIANGLESTRIP == AoTvxGetPrimitiveType(obj_vtx_file, num));

				vtx_num = AoTvxGetVertexNum(obj_vtx_file, num);
				tex_id  = AoTvxGetTextureId(obj_vtx_file, num);
				//tex_id = 0; // 強制テクスチャ１枚化

				// work 設定
				for (i = 0; i < GMD_MAP_PRIM_DRAW_WORK_NUM; i++) {
					//	ワークへ未登録または該当ID登録済みならテクスチャ情報を登録
					if (work[i].tex_id == -1 || work[i].tex_id == tex_id) {
						work[i].tex_id = tex_id;
						work[i].all_vtx_num += vtx_num;

						stack = &work[i].stack[work[i].stack_num];
						//AOS_TVX_VERTEX* vtx = (AOS_TVX_VERTEX*)AoTvxGetVertex(obj_vtx_file, num);
						//stack->vtx     = (*vtx);
						stack->vtx     = (AOS_TVX_VERTEX*)AoTvxGetVertex(obj_vtx_file, num);
						stack->vtx_num = (u16)vtx_num;
						stack->mp      = (*mp_block);
						stack->dx      = trans_x + (dx + 0.5f) * GMD_MAP_BLOCK_SIZE;
						stack->dy      = -trans_y + (-dy - 0.5f) * GMD_MAP_BLOCK_SIZE;
						stack->dz      = trans_z;
						
						++work[i].stack_num;

						MTM_ASSERT(work[i].stack_num < GMD_MAP_PRIM_DRAW_STACK_NUM);

						break;
					}
				}
				MTM_ASSERT(i < GMD_MAP_PRIM_DRAW_WORK_NUM); // ループを完走した=プリミティブワーク不足
			}
		}
	}
}
#endif //_IPHONE

#if _IPHONE
// ==========================================================================
// gmMapExecuteDrawMapTvx
/*!
 *	マップ描画
 *
 *	@param	mtx		[in]	描画基準になるマトリックス
 *	@param	texlist	[in]	描画するマップ用テクスチャ
 */
// ==========================================================================
void gmMapExecuteDrawMapTvx(NNS_MATRIX* mtx, NNS_TEXLIST* texlist)
{
	AMS_PARAM_DRAW_PRIMITIVE dat;
	GMS_MAP_PRIM_DRAW_WORK *work = gm_map_prim_draw_work;	// プリミティブ描画ワーク

	s32			i;
	s32*		blend = gm_map_prim_draw_tvx_alpha_set; // アルファ使用設定

#if GMD_MAP_PRIM_DRAW_USE_SORT
	GMS_MAP_PRIM_DRAW_WORK *sort_work[GMD_MAP_PRIM_DRAW_SORT_NUM] = {0};
	u32 sort_num = 0;
#endif // GMD_MAP_PRIM_DRAW_USE_SORT

	u32 color = gm_map_prim_draw_tvx_color;

#ifdef GMD_MAP_DRAW_STRIP_TEST
	dat.type = NNE_PRIM_TRIANGLE_STRIP;
#else
	dat.type = NNE_PRIM_TRIANGLE_LIST;
#endif
	
	// アルファブレンド設定(出来るものだけ)
	dat.ablend = NNE_PRIM_ALPHABLEND_OFF;
#if defined(_PC) | defined(_XBOX)
	dat.bldMode = NNE_BLENDOP_ADD;
#else
	dat.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
	dat.aTest = 1;
	
	// Zテスト設定
	dat.zMask = 0;
	dat.zTest = 1;
	
	// ソートしない
	dat.noSort = 1;
	
	// テクスチャクランプ設定
	dat.uwrap = NNE_PRIM_TEXWRAP_CLAMP;
	dat.vwrap = NNE_PRIM_TEXWRAP_CLAMP;
	
	dat.format3D = NNE_PRIM3D_FMT_PCT;

	// テクスチャ設定
	dat.texlist = texlist;
	
	// 描画
	u32 prim;
	for (prim = 0; prim < GMD_MAP_PRIM_DRAW_WORK_NUM; ++prim) {
		//	登録されてないなら終了
		if (work[prim].tex_id == -1) {
			break;
		}
		
#if GMD_MAP_PRIM_DRAW_USE_SORT
		if (blend) {
			MTM_ASSERT(work[prim].tex_id >= 0);
			
			if (blend[work[prim].tex_id]) {
				if (sort_num >= GMD_MAP_PRIM_DRAW_SORT_NUM) {
					MTM_ASSERT(0);
					break;
				}
				sort_work[sort_num] = &work[prim];
				sort_work[sort_num]->op = blend[work[prim].tex_id];
				sort_num++;
				continue; // 現在のワークを保存して終了
			}
		}
#endif // GMD_MAP_PRIM_DRAW_USE_SORT
		// 描画
		gmMapExecuteDrawMapTvxCore(mtx, &work[prim], &dat, color);
	}

#if GMD_MAP_PRIM_DRAW_USE_SORT
	if (blend) {
		for (i = 0; i < sort_num; i++) {
			switch (sort_work[i]->op) {
				// アルファブレンド設定(半透明ブレンド)
				case GMD_MAP_PRIM_DRAW_OP_BLEND:
#if defined(_PC) | defined(_XBOX)
					dat.bldSrc = NNE_BLENDMODE_SRCALPHA;
					dat.bldDst = NNE_BLENDMODE_INVSRCALPHA;
#else
					dat.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
					dat.bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
#endif
					dat.ablend = NNE_PRIM_ALPHABLEND_ON;
					dat.aTest = 1;
					break;
				
				// アルファブレンド設定(加算ブレンド)
				case GMD_MAP_PRIM_DRAW_OP_ADD:
#if defined(_PC) | defined(_XBOX)
					dat.bldSrc = NNE_BLENDMODE_SRCALPHA;
					dat.bldDst = NNE_BLENDMODE_ONE;
#else
					dat.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
					dat.bldDst = NND_BLENDFUNC_GL_ONE;
#endif
					dat.ablend = NNE_PRIM_ALPHABLEND_ON;
					dat.aTest = 0;
					break;
				
				// 特になし
				default:
					break;
			}
			// 疑似ソート描画
			gmMapExecuteDrawMapTvxCore(mtx, sort_work[i], &dat, color);
		}
	}
#endif // GMD_MAP_PRIM_DRAW_USE_SORT

}
#endif //_IPHONE

#if _IPHONE
static void gmMapExecuteDrawMapTvxCore(NNS_MATRIX* mtx, GMS_MAP_PRIM_DRAW_WORK* work, AMS_PARAM_DRAW_PRIMITIVE* dat, u32 color)
{
	GMS_MAP_PRIM_DRAW_STACK *stack;
	float		dx, dy, dz;				// ブロック単位描画位置
	// 描画
#ifdef GMD_MAP_DRAW_STRIP_TEST
	dat->count = work->all_vtx_num + work->stack_num * 2 - 2;
#else
	dat->count = work->.all_vtx_num;
#endif
	NNS_PRIM3D_PCT* v_tbl = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer((s32)(sizeof(NNS_PRIM3D_PCT) * dat->count));
	dat->vtxPCT3D = v_tbl;
	dat->texId = work->tex_id;
	u32 v_tbl_pos = 0;
	NNS_TEXCOORD scr_uv;
	gmMapGetDrawMapTvxTexScrollUV(dat->texId, &scr_uv);

	for (u32 num = 0; num < work->stack_num; ++num) {
		stack = &(work->stack[num]);
		// プリミティブ設定
		s32 draw_num = stack->vtx_num / 3;
		dx = stack->dx;
		dy = stack->dy;
		dz = stack->dz;

		NNS_VECTOR vec;
		NNS_MATRIX *prim_mtx;
#if 01
		prim_mtx = gmMapGetUsePrimMatrix(stack->mp.rot, (stack->mp.flip_h | (stack->mp.flip_v << 1)));
#else
		NNS_MATRIX use_mtx;
		nnMakeUnitMatrix(&use_mtx);
		
		// 回転
		if (stack->mp.rot != 0) {
			nnRotateZMatrix(&use_mtx, &use_mtx, (u16)(stack->mp.rot * 0x4000));
		}
		
		// 反転・スケール(拡大はすでにデータで対応されている)
		switch (stack->mp.flip_h | (stack->mp.flip_v << 1)) {
		case 0:
			nnScaleMatrix(&use_mtx, &use_mtx,  1.0f,  1.0f, 1.0f);
			break;
		case 1:
			nnScaleMatrix(&use_mtx, &use_mtx, -1.0f,  1.0f, 1.0f);
			break;
		case 2:
			nnScaleMatrix(&use_mtx, &use_mtx,  1.0f, -1.0f, 1.0f);
			break;
		case 3:
			nnScaleMatrix(&use_mtx, &use_mtx, -1.0f, -1.0f, 1.0f);
			break;
		}

		// 拡大率に合わせて位置ずらし
		nnTranslateMatrix(&use_mtx, &use_mtx, -(float)GMD_MAP_BLOCK_LOCAL_SIZE * GMD_OBJ_DRAW_SCALE * 0.5f,
				-(float)GMD_MAP_BLOCK_LOCAL_SIZE * GMD_OBJ_DRAW_SCALE * 0.5f, 0.0f);
		
		prim_mtx = &use_mtx;
#endif // 01

#ifdef GMD_MAP_DRAW_STRIP_TEST
		NNS_PRIM3D_PCT* v = v_tbl;
		AOS_TVX_VERTEX* vtx = stack->vtx;
		
		// 頂点設定 (STRIP対応)
		for (int i = 0; i < stack->vtx_num; i++) {
			vec.x = vtx[i].x;
			vec.y = vtx[i].y;
			vec.z = vtx[i].z;
			
			// ベクトルを変換
			nnTransformVector(&v[i].Pos, prim_mtx, &vec);
			
			v[i].Pos.x += dx;
			v[i].Pos.y += dy;
			v[i].Pos.z += dz;
			
			v[i].Tex.u = vtx[i].u + scr_uv.u;
			v[i].Tex.v = vtx[i].v + scr_uv.v;
			v[i].Col   = vtx[i].c & color;
		}
		
		v_tbl += stack->vtx_num + 2;
		
		// STRIPなデータにするための対応
		NNS_PRIM3D_PCT* v_strip;
		// 先頭のSTRIP以外
		if (num != 0) {
			v_strip = v - 1;
			v_strip[0] = v_strip[1];
		}
		// 最後のSTRIP以外
		if (num != work->stack_num - 1) {
			v_strip = &v[stack->vtx_num - 1];
			v_strip[1] = v_strip[0];
		}
		
#else // GMD_MAP_DRAW_STRIP_TEST
		// 頂点データ作成
		for (i = 0; i < draw_num; ++i) {
			NNS_PRIM3D_PCT* v = v_tbl + (3 * v_tbl_pos);
			AOS_TVX_VERTEX* vtx = stack->vtx + (3 * i);

			// UV座標設定 (暫定)
			for (u32 j = 0; j < 3; j++) {
				vec.x = vtx[j].x;
				vec.y = vtx[j].y;
				vec.z = vtx[j].z;

				// ベクトルを変換
				nnTransformVector(&v[j].Pos, prim_mtx, &vec);

				v[j].Pos.x += dx;
				v[j].Pos.y += dy;
				v[j].Pos.z += dz;

				v[j].Tex.u = vtx[j].u;
				v[j].Tex.v = vtx[j].v;
				v[j].Col   = vtx[j].c & color;
			}
			++v_tbl_pos;
		}
#endif // GMD_MAP_DRAW_STRIP_TEST
	}

	amMatrixPush(mtx);
	ObjDraw3DNNDrawPrimitive(dat, gm_map_draw_command_state, NNE_PRIM_LIGHT_DISABLE, NNE_PRIM_CULL_NONE);
	amMatrixPop();
}
#endif // _IPHONE

#if _IPHONE
// ==========================================================================
// gmMapBuildDrawMapTvxTexScroll
/*!
 *	TVX用テクスチャUVスクロール処理 構築
 *
 *	@note MAP描画を開始する前に呼び出す。<BR>
 *		UVスクロールが無い場合は、この関数の実行後
 *		gm_map_prim_draw_uv_workがNULLの場合もある。
 */
// ==========================================================================
static void gmMapBuildDrawMapTvxTexScroll(void)
{
	GMS_MAP_PRIM_DRAW_TVX_UV_WORK* uv_work = gm_map_prim_draw_uv_work;
	
	MTM_ASSERT(!uv_work);
	
	// ゾーン毎設定初期化
	s32 motion_num; // 総モーション数が入る
	s32 tex_uv_index_tbl_num; // 総テクスチャ数が入る
	GSE_MAIN_ZONE_TYPE zone_type = g_gm_gamedat_zone_type_tbl[g_gs_main_sys_info.stage_id];
	switch (zone_type) {
		case GSD_MAIN_ZONE_TYPE_1:
		case GSD_MAIN_ZONE_TYPE_SS:
			return; // 何もしないで終了
			
		case GSD_MAIN_ZONE_TYPE_2:
			motion_num           = gm_map_prim_draw_tvx_mgr_index_tbl_z2_num;
			tex_uv_index_tbl_num = gm_map_prim_draw_tvx_texture_z2_num;
			break;
		
		case GSD_MAIN_ZONE_TYPE_3:
			motion_num           = gm_map_prim_draw_tvx_mgr_index_tbl_z3_num;
			tex_uv_index_tbl_num = gm_map_prim_draw_tvx_texture_z3_num;
			break;
		
		case GSD_MAIN_ZONE_TYPE_4:
			motion_num           = gm_map_prim_draw_tvx_mgr_index_tbl_z4_num;
			tex_uv_index_tbl_num = gm_map_prim_draw_tvx_texture_z4_num;
			break;
			
		case GSD_MAIN_ZONE_TYPE_FINAL:
			motion_num           = gm_map_prim_draw_tvx_mgr_index_tbl_zf_num;
			tex_uv_index_tbl_num = gm_map_prim_draw_tvx_texture_zf_num;
			break;

		default:
			MTM_ASSERT(0); // 不正なZoneをAssertion
			break;
	}
	
	// メモリ確保と初期化
	u32 work_size          = sizeof(GMS_MAP_PRIM_DRAW_TVX_UV_WORK);               // 基本構造体
	u32 work_fi_size       = work_size + sizeof(u32) * motion_num;                // 管理フレームテーブルindex分追加
	u32 work_fi_f_size     = work_fi_size + sizeof(u32) * motion_num;             // 管理フレームテーブル分追加
	u32 work_fi_f_tex_size = work_fi_f_size + sizeof(s32) * tex_uv_index_tbl_num; // テクスチャUVインデックステーブル分追加
	
	uv_work = (GMS_MAP_PRIM_DRAW_TVX_UV_WORK*)amMemAlloc(work_fi_f_tex_size);
	amZeroMemory(uv_work, work_fi_f_tex_size);
	gm_map_prim_draw_uv_work = uv_work;
	
	uv_work->mgr_index_tbl_num = motion_num;
	uv_work->frame_index_tbl   = (u32*)((u32)uv_work + work_size);
	uv_work->frame_tbl         = (u32*)((u32)uv_work + work_fi_size);
	uv_work->tex_uv_index_tbl  = (s32*)((u32)uv_work + work_fi_f_size);
	memset(uv_work->tex_uv_index_tbl, -1, (work_fi_f_tex_size - work_fi_f_size)); // テクスチャUVインデックスには-1を設定しておく
	
	switch (zone_type) {
		case GSD_MAIN_ZONE_TYPE_1:
		case GSD_MAIN_ZONE_TYPE_SS:
			return; // 何もしないで終了
			
		case GSD_MAIN_ZONE_TYPE_2:
			uv_work->mgr_index_tbl_addr = (u32*)(gm_map_prim_draw_tvx_mgr_index_tbl_z2);
			uv_work->mgr_tbl_num        = (s32*)gm_map_prim_draw_tvx_mgr_tbl_z2_num;
			uv_work->mgr_tbl_addr       = (u32*)(gm_map_prim_draw_tvx_mgr_tbl_z2);
			uv_work->uv_mgr_tbl_addr    = (u32*)(gm_map_prim_draw_tvx_uv_mgr_tbl_z2);
			break;
		
		case GSD_MAIN_ZONE_TYPE_3:
			uv_work->mgr_index_tbl_addr = (u32*)(gm_map_prim_draw_tvx_mgr_index_tbl_z3);
			uv_work->mgr_tbl_num        = (s32*)gm_map_prim_draw_tvx_mgr_tbl_z3_num;
			uv_work->mgr_tbl_addr       = (u32*)(gm_map_prim_draw_tvx_mgr_tbl_z3);
			uv_work->uv_mgr_tbl_addr    = (u32*)(gm_map_prim_draw_tvx_uv_mgr_tbl_z3);
			break;
			
		case GSD_MAIN_ZONE_TYPE_4:
			uv_work->mgr_index_tbl_addr = (u32*)(gm_map_prim_draw_tvx_mgr_index_tbl_z4);
			uv_work->mgr_tbl_num        = (s32*)gm_map_prim_draw_tvx_mgr_tbl_z4_num;
			uv_work->mgr_tbl_addr       = (u32*)(gm_map_prim_draw_tvx_mgr_tbl_z4);
			uv_work->uv_mgr_tbl_addr    = (u32*)(gm_map_prim_draw_tvx_uv_mgr_tbl_z4);
			break;
			
		case GSD_MAIN_ZONE_TYPE_FINAL:
			uv_work->mgr_index_tbl_addr = (u32*)(gm_map_prim_draw_tvx_mgr_index_tbl_zf);
			uv_work->mgr_tbl_num        = (s32*)gm_map_prim_draw_tvx_mgr_tbl_zf_num;
			uv_work->mgr_tbl_addr       = (u32*)(gm_map_prim_draw_tvx_mgr_tbl_zf);
			uv_work->uv_mgr_tbl_addr    = (u32*)(gm_map_prim_draw_tvx_uv_mgr_tbl_zf);
			break;
			
		default:
			MTM_ASSERT(0); // 不正なZoneをAssertion
			break;
	}
	
	// テクスチャ使用判定処理
	GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX* mgr_index_tbl = (GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX*)(uv_work->mgr_index_tbl_addr);
	s32* tex_uv_index_tbl                          = uv_work->tex_uv_index_tbl;
	for (int i = 0; i < motion_num; i++) {
		u16 tex_id = mgr_index_tbl[i].tex_id;
		u16 mgr_id = mgr_index_tbl[i].mgr_id;
		for (int j = 0; j < tex_uv_index_tbl_num; j++) {
			if (tex_id == j) {
				tex_uv_index_tbl[j] = mgr_id;
			}
		}
	}
}
#endif // _IPHONE

#if _IPHONE
// ==========================================================================
// gmMapFlushDrawMapTvxTexScroll
/*!
 *	TVX用テクスチャUVスクロール処理 後片付け
 *
 *	@note MAP描画が終わったら呼び出す。
 *		gmMapBuildDrawMapTvxTexScrollの後始末に使うこと。
 */
// ==========================================================================
static void gmMapFlushDrawMapTvxTexScroll(void)
{
	GMS_MAP_PRIM_DRAW_TVX_UV_WORK* uv_work = gm_map_prim_draw_uv_work;
	
	if (uv_work) {
		amMemFree(uv_work);
		gm_map_prim_draw_uv_work = NULL;
	}
}
#endif // _IPHONE

#if _IPHONE
// ==========================================================================
// gmMapUpdateDrawMapTvxTexScroll
/*!
 *	TVX用テクスチャUVスクロール処理 スクロール更新
 *
 *	@note UVスクロールを更新する。
 *		gm_map_prim_draw_uv_workがNULLの場合は何もしない。
 */
// ==========================================================================
static void gmMapUpdateDrawMapTvxTexScroll(void)
{
	GMS_MAP_PRIM_DRAW_TVX_UV_WORK* uv_work = gm_map_prim_draw_uv_work;
	
	// ZONEによってはモーションが無いので判定を入れる
	if (uv_work) {
		s32 index_tbl_num    = uv_work->mgr_index_tbl_num;
		s32* mgr_tbl_num     = uv_work->mgr_tbl_num;
		u32* frame_index_tbl = uv_work->frame_index_tbl;
		u32* frame_tbl       = uv_work->frame_tbl;
		GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX* mgr_index_tbl = (GMS_MAP_PRIM_DRAW_TVX_MGR_INDEX*)uv_work->mgr_index_tbl_addr;
		for (int i = 0; i < index_tbl_num; i++) {
			//	テーブルを取り出してIDからアクセスするデータを設定
			u16 mgr_index                      = mgr_index_tbl[i].mgr_id;
			GMS_MAP_PRIM_DRAW_TVX_MGR* mgr_tbl = (GMS_MAP_PRIM_DRAW_TVX_MGR*)(uv_work->mgr_tbl_addr[mgr_index]);
			
			//	規定フレームを超えたら更新
			if (++frame_tbl[mgr_index] >= mgr_tbl[frame_index_tbl[mgr_index]].time) {
				frame_tbl[mgr_index] = 0;
				frame_index_tbl[mgr_index] = (frame_index_tbl[mgr_index] + 1) % mgr_tbl_num[mgr_index];
			}
		}
	}
}
#endif // _IPHONE

#if _IPHONE
// ==========================================================================
// gmMapGetDrawMapTvxTexScrollUV
/*!
 *	TVX用テクスチャUVスクロール処理 スクロール値入手
 *
 *	@param tex_id [in]  UV値を要求するテクスチャID
 *	@param scr_uv [out] 設定されたUV
 *
 *	@note UVスクロール値を入手する<BR>
 *		UV値が無い場合は scr_uvの uと vに 0.0fが設定される。
 *		tex_idにUVスクロール情報が設定されていない場合は scr_uvの uと vに 0.0fが設定される。
 *		gm_map_prim_draw_uv_workがNULLの場合は何もしない。
 */
// ==========================================================================
static void gmMapGetDrawMapTvxTexScrollUV(s32 tex_id, NNS_TEXCOORD* scr_uv) {
	GMS_MAP_PRIM_DRAW_TVX_UV_WORK* uv_work = gm_map_prim_draw_uv_work;
	
	// 初期化
	scr_uv->u = 0.0f;
	scr_uv->v = 0.0f;
	
	// ZONEによってはモーションが無いので判定を入れる
	if (uv_work) {
		// 使用する indexが設定されていれば設定
		s32 tex_uv_index = uv_work->tex_uv_index_tbl[tex_id];
		
		if (-1 != tex_uv_index) {
			GMS_MAP_PRIM_DRAW_TVX_MGR* mgr_tbl = (GMS_MAP_PRIM_DRAW_TVX_MGR*)(uv_work->mgr_tbl_addr[tex_uv_index]);
			NNS_TEXCOORD* uv_mgr_tbl = (NNS_TEXCOORD*)(uv_work->uv_mgr_tbl_addr[tex_uv_index]);
			u32 frame_index_tbl      = uv_work->frame_index_tbl[tex_uv_index];
			u16 motion_id            = mgr_tbl[frame_index_tbl].motion_id; // 使用するモーションを取り出す
			scr_uv->u = uv_mgr_tbl[motion_id].u;
			scr_uv->v = uv_mgr_tbl[motion_id].v;
		}
	}
}
#endif // _IPHONE

#if _IPHONE
// ==========================================================================
// gmMapCreateUsePrimMatrix
/*!
 *	マップ描画に使用するプリミティブ用行列の作成
 *
 *	@note マップ描画に使用するプリミティブ用の行列をあらかじめ作成し、<BR>
 *		描画の際に計算コストが発生しないようにします。<BR>
 *		作成された用行列は gmMapGetUsePrimMatrixにて入手してください。<BR>
 */
// ==========================================================================
static void gmMapCreateUsePrimMatrix(void)
{
	NNS_MATRIX base_mtx;
	NNS_MATRIX *prim_mtx;
	for (s32 i = 0; i < 4; i++) {
		nnMakeUnitMatrix(&base_mtx);
		// 回転
		nnRotateZMatrix(&base_mtx, &base_mtx, i * 0x4000);
		
		for (s32 j = 0; j < 4; j++) {
			prim_mtx = &gm_map_use_prim_mtx[i * 4 + j];
			nnCopyMatrix(prim_mtx, &base_mtx);
			// 反転・スケール(拡大はすでにデータで対応されている)
			switch (j) {
			case 0:
				nnScaleMatrix(prim_mtx, prim_mtx,  1.0f,  1.0f, 1.0f);
				break;
			case 1:
				nnScaleMatrix(prim_mtx, prim_mtx, -1.0f,  1.0f, 1.0f);
				break;
			case 2:
				nnScaleMatrix(prim_mtx, prim_mtx,  1.0f, -1.0f, 1.0f);
				break;
			case 3:
				nnScaleMatrix(prim_mtx, prim_mtx, -1.0f, -1.0f, 1.0f);
				break;
			}
			// 拡大率に合わせて位置ずらし
			nnTranslateMatrix(prim_mtx, prim_mtx, -(float)GMD_MAP_BLOCK_LOCAL_SIZE * GMD_OBJ_DRAW_SCALE * 0.5f,
					-(float)GMD_MAP_BLOCK_LOCAL_SIZE * GMD_OBJ_DRAW_SCALE * 0.5f, 0.0f);
		}
	}
}

// ==========================================================================
// gmMapGetUsePrimMatrix
/*!
 *	マップ描画に使用するプリミティブ用行列の入手
 *
 *	@param rot [in] 回転情報(rot * 0x4000 を想定した行列)
 *	@param flip [in] 反転情報(0:反転なし 1:X軸反転 2:Y軸反転 3:XY軸反転)
 *
 *	@note gmMapCreateUsePrimMatrixで作成した行列を入手します。<BR>
 *		gmMapCreateUsePrimMatrixを呼ぶ前にこの関数を呼んだ場合、<BR>
 *		返り値は不定です。
 */
// ==========================================================================
static NNS_MATRIX* gmMapGetUsePrimMatrix(int rot, int flip)
{
	MTM_ASSERT(rot >= 0);
	MTM_ASSERT(flip >= 0);
	
	return &gm_map_use_prim_mtx[rot * 4 + flip];
}
#endif // _IPHONE

// ==========================================================================
// gmMapFallShaderSettingPrioPreMidMapUserFunc
/*!
 *	滝シェーダー テクスチャ生成
 */
// ==========================================================================
void gmMapFallShaderSettingPrioPreMidMapUserFunc(void *data)
{
	AMS_RENDER_TARGET	*render_target;
	NNS_RGBA_U8			color = {0x00, 0x00, 0x00, 0xFF};

	UNREFERENCED_PARAMETER( data );

	// レンダターゲットを取得
#if 1
	render_target = _am_render_manager.targetp;
	amAssert(render_target);

	if ( render_target == &_gm_mapFar_render_work ){
		render_target = &_am_draw_target;
	}
	else {
		render_target = &_gm_mapFar_render_work;
	}
#else
#if _WII
	render_target = &_gm_mapFar_render_work;
#else
	render_target = &_am_draw_target;
#endif
#endif
	// レンダターゲットが作成されていない
	if (render_target->width == 0 ) {
		return;
	}

	// レンダリングターゲットのコピー
#if 1
	amRenderCopyTarget(render_target, &color);
#else
#if _WII
	amRenderCopyTarget(render_target);
#elif _XBOX
	amRenderSetTarget(render_target);
#else
	amRenderSetTarget(render_target,
			AMD_RENDER_CLEAR_COLOR | AMD_RENDER_CLEAR_DEPTH);
#endif
#endif
}

// ==========================================================================
// gmMapDrawFallShaderPrioPreMidMapUserFunc
/*!
 *	滝シェーダーモデル 描画時設定
 */
// ==========================================================================
void gmMapDrawFallShaderPrioPreMidMapUserFunc(void *data)
{
	NNS_MATRIX44		*proj_mtx;
	AMS_RENDER_TARGET	*render_target;

	UNREFERENCED_PARAMETER(data);

	// レンダターゲットを取得
#if 1
	render_target = _am_render_manager.targetp;
	amAssert(render_target);

	if ( render_target == &_gm_mapFar_render_work ){
		render_target = &_am_draw_target;
	}
	else {
		render_target = &_gm_mapFar_render_work;
	}
#else
#if _WII
	render_target = &_gm_mapFar_render_work;
#else
	render_target = &_am_draw_target;
#endif
#endif
	// レンダターゲットが作成されていない
	if (render_target->width == 0 ) {
		return;
	}

	proj_mtx = amDrawGetProjectionMatrix();

#if _WII
	if (proj_mtx != NULL) {
		memcpy(_am_draw_fall_projmtx, proj_mtx, sizeof(NNS_MATRIX44));
	}
#endif	//_WII

	// テクスチャ設定
#if _PC | _XBOX
	{
		Uint32	state[NNE_SAMPLERSTATETYPE_MAX];
		NNS_MATRIX44	mtx;
		nnMakeUnitMatrix(&mtx);
		nnInitMaterialControlUserSamplerDXG20();
		nnGetMaterialControlUserSamplerDefaultStateDXG20(state);
		state[NNE_SAMPLERSTATETYPE_ADDRESSU]	= D3DTADDRESS_CLAMP;
		state[NNE_SAMPLERSTATETYPE_ADDRESSV]	= D3DTADDRESS_CLAMP;
		state[NNE_SAMPLERSTATETYPE_ADDRESSW]	= D3DTADDRESS_CLAMP;
		state[NNE_SAMPLERSTATETYPE_MAGFILTER]	= D3DTEXF_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MINFILTER]	= D3DTEXF_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MIPFILTER]	= D3DTEXF_NONE;
		nnSetMaterialControlUserSamplerDXG20(NNE_USER_SAMPLER_2D_1,
				render_target->texture_color[0], &mtx, state);

		nnSetUserUniformDXG20(0,
				NNM_MTX(*proj_mtx, 0, 0), NNM_MTX(*proj_mtx, 1, 0),
				NNM_MTX(*proj_mtx, 2, 0), NNM_MTX(*proj_mtx, 3, 0));
		nnSetUserUniformDXG20(1,
				NNM_MTX(*proj_mtx, 0, 1), NNM_MTX(*proj_mtx, 1, 1),
				NNM_MTX(*proj_mtx, 2, 1), NNM_MTX(*proj_mtx, 3, 1));
		nnSetUserUniformDXG20(2,
				NNM_MTX(*proj_mtx, 0, 2), NNM_MTX(*proj_mtx, 1, 2),
				NNM_MTX(*proj_mtx, 2, 2), NNM_MTX(*proj_mtx, 3, 2));
		nnSetUserUniformDXG20(3,
				NNM_MTX(*proj_mtx, 0, 3), NNM_MTX(*proj_mtx, 1, 3),
				NNM_MTX(*proj_mtx, 2, 3), NNM_MTX(*proj_mtx, 3, 3));
	}
#elif _PS3
	{

		Uint32	state[NNE_SAMPLERSTATETYPE_MAX];
		NNS_MATRIX44	mtx;

		nnMakeUnitMatrix(&mtx);
		nnInitMaterialControlUserSamplerPS3();
		nnGetMaterialControlUserSamplerDefaultStatePS3(state);
		state[NNE_SAMPLERSTATETYPE_ADDRESSU]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;	// NND_WRAPMODE_PS3_CLAMP (NND_WRAPMODE_PS3_***)
		state[NNE_SAMPLERSTATETYPE_ADDRESSV]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_ADDRESSW]	= CELL_GCM_TEXTURE_CLAMP_TO_EDGE;
		state[NNE_SAMPLERSTATETYPE_MAGFILTER]	= CELL_GCM_TEXTURE_LINEAR;	// NNE_FILTER_LINEAR (NNE_FILTER)
		state[NNE_SAMPLERSTATETYPE_MINFILTER]	= CELL_GCM_TEXTURE_LINEAR;
		state[NNE_SAMPLERSTATETYPE_MIPFILTER]	= CELL_GCM_TEXTURE_NEAREST;	// NNE_FILTER_NEAREST (NNE_FILTER) (NNE_FILTER_NONE?)
		nnSetMaterialControlUserSamplerPS3(NNE_USER_SAMPLER_2D_1,
				render_target->texture_color[0], &mtx, state);

		nnSetUserUniformPS3(0,
				NNM_MTX(*proj_mtx, 0, 0), NNM_MTX(*proj_mtx, 1, 0),
				NNM_MTX(*proj_mtx, 2, 0), NNM_MTX(*proj_mtx, 3, 0));
		nnSetUserUniformPS3(1,
				NNM_MTX(*proj_mtx, 0, 1), NNM_MTX(*proj_mtx, 1, 1),
				NNM_MTX(*proj_mtx, 2, 1), NNM_MTX(*proj_mtx, 3, 1));
		nnSetUserUniformPS3(2,
				NNM_MTX(*proj_mtx, 0, 2), NNM_MTX(*proj_mtx, 1, 2),
				NNM_MTX(*proj_mtx, 2, 2), NNM_MTX(*proj_mtx, 3, 2));
		nnSetUserUniformPS3(3,
				NNM_MTX(*proj_mtx, 0, 3), NNM_MTX(*proj_mtx, 1, 3),
				NNM_MTX(*proj_mtx, 2, 3), NNM_MTX(*proj_mtx, 3, 3));
	}
#elif _WII
	{
		memcpy(&_am_draw_render_work, render_target, sizeof(AMS_RENDER_TARGET));
	}
#endif
}

#if _WII
// ==========================================================================
// gmMapFallMaterialCallback
/*!
 *	滝シェーダ用 マテリアルコールバック(OBF_MATERIAL_CB)
 *
 *	@param	val		[in]	ドローコールバック変数
 *	@param	param	[in]	ユーザーパラメータ
 */
// ==========================================================================
NNE_BOOL gmMapFallMaterialCallback(NNS_DRAWCALLBACK_VAL *val, void *param)
{
	UNREFERENCED_PARAMETER(param);
	return (amDrawWaterFallMaterial(val));
}
#endif	//_WII

// ==========================================================================
// GmMapStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void GmMapStaticVarInit(void)
{
	gm_map_obj3d = NULL;				//!< MAP描画用 描画オブジェクト 0番はブランク
	gm_map_reg_obj3d_num = 0;			//!< 登録オブジェクト数
	gm_map_release_obj3d_num = 0;		//!< 開放オブジェクト数
	gm_map_tcb = NULL;
	gm_map_draw_command_state = OBD_DRAW_CMD_STATE_3DNN;				//!< マップ描画用コマンドステート
#if !_IPHONE
	gm_map_use_fall_shader = FALSE;		//!< 滝シェーダー使用
	gm_map_fall_rander_command_state = OBD_DRAW_CMD_STATE_WATER_BACK;	//!< 滝シェーダー用レンダリングコマンドステート
#else // !_IPHONE
	memset(&gm_map_texture, 0, sizeof(gm_map_texture));					//!< 使用するテクスチャリスト
	gm_map_model   = NULL;				//!< 使用するモデルリスト
	gm_map_tex_load_init = FALSE;		//!< 初回ロード
	gm_map_tex_draw_count = 0;			//!< 初回描画
	memset(gm_map_prim_draw_work, 0, sizeof(gm_map_prim_draw_work));			//!< プリミティブ描画ワーク
	memset(&gm_map_prim_draw_base_pos, 0, sizeof(gm_map_prim_draw_base_pos));	//!< 描画基準位置
	gm_map_prim_draw_uv_work = NULL;	//!< プリミティブUVモーションワーク
	
	memset(gm_map_draw_size, 0, sizeof(gm_map_draw_size));		// map search size
	gm_map_draw_bgm_timer = 0;		// bgm制御用タイマー
#endif // !_IPHONE
	gm_map_draw_margin_adjust = 0;	//!< 描画時範囲補正値
	
	memset(gm_map_block_check, 0, sizeof(gm_map_block_check));	//!< 描画時ブロックチェック用ワーク
	
	/// 行列計算情報保存用ワーク
	memset(gm_map_use_prim_mtx, 0, sizeof(gm_map_use_prim_mtx));
	
	gm_map_prim_draw_tvx_color = 0;			//!< マップ描画に使用する色
	gm_map_prim_draw_tvx_alpha_set = NULL;	//!< マップ描画に使用するアルファ設定
}

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
