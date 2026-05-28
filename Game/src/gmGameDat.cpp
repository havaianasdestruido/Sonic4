// ==========================================================================
/*!
  @file gmGameDat.cpp
  @brief ゲームデータ管理

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmGameDat.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "gmTask.h"
#include "gmMain.h"

#include "gmPlayer.h"
#include "gmMapFar.h"
#include "gmDeco.h"
#include "gmMap.h"
#include "gmWaterSurface.h"
#include "gmGameDBuild.h"

#include "gmGameDat.h"

#include "gmEnemy.h"

//----- Definitions ---------------------------------------------------------
//#define GMD_GAMEDAT_SND_DATA_SIZE_MAX		(380*1024)//((200+120)*1024)	//!< サウンドデータの最大サイズ syMain:SYD_SND_HEAP_SIZE を越えてはならない (400*1024)

#define GMD_GAMEDAT_FILE_PATH_LENGTH_MAX		(256)				//!< ファイルパス長

//#define GMD_GAMEDAT_USE_NET					(0)					//!< ネット機能の使用あり

#define GMD_GAMEDAT_LOAD_CONTEXT_MAX			(96)				//!< 読み込みファイル最大数

#if defined GMD_DEBUG_NO_CREATE_RING || defined GMD_DEBUG_NO_CREATE_COCKPIT
#if _IPHONE
#define GMD_GAMEDAT_LOAD_RING_SKIP  (3)
#else
#define GMD_GAMEDAT_LOAD_RING_SKIP  (2)
#endif // _IPHONE
#endif // GMD_DEBUG_NO_CREATE_RING || defined GMD_DEBUG_NO_CREATE_COCKPIT



/// 固定共通データ(読み込み順)
enum {
	GMD_GAMEDAT_COMMON_RING_MODEL	= 0,	//!< リングモデルAMB
	GMD_GAMEDAT_COMMON_RING_TEX,			//!< リングテクスチャAMB

	GMD_GAMEDAT_COMMON_MAX
};


struct tag_GMS_GAMEDAT_LOAD_CONTEXT;

/// ロードデータ構造体
typedef struct tag_GMS_GAMEDAT_LOAD_DATA {
	const char *path;										//!< ファイルパス
	void*(*alloc)(const char*);								//!< ファイル格納先のヒープ確保関数
	void(*proc_pre)(struct tag_GMS_GAMEDAT_LOAD_CONTEXT*);	//!< ファイル読み込み前の処理関数 NULL可
	void(*proc_post)(struct tag_GMS_GAMEDAT_LOAD_CONTEXT*);	//!< ファイル読み込み後の処理関数 NULL可
	s32	user_data;											//!< ユーザー指定データ 読み込み処理側で自由に使用してよい
	//s32	bb_no;												//!< BBファイル使用時のID 無効時は-1 有効時はpathがBBファイルパス
	//														// BBからの直接読み込みは通信時は使用出来ない
} GMS_GAMEDAT_LOAD_DATA;


/// ロード情報構造体
typedef struct tag_GMS_GAMEDAT_LOAD_INFO {
	const GMS_GAMEDAT_LOAD_DATA		*data_tbl;	//!< ロードデータのテーブル
	s32								num;		//!< テーブルの項目数
} GMS_GAMEDAT_LOAD_INFO;


// ロード状態
typedef enum tag_GME_GAMEDAT_LOAD_STATE {
	GMD_GAMEDAT_LOAD_STATE_LOADING  = 0,	// 読み込み中
	GMD_GAMEDAT_LOAD_STATE_LOADFINISH,		// データ読み込み完了
	GMD_GAMEDAT_LOAD_STATE_COMPLETE,		// 完了(後処理も完了)

	GMD_GAMEDAT_LOAD_STATE_ERROR,			// エラー
											//  非通信時にエラーはありえません
											//  通信時は切断などによりエラー状態になります。

	GMD_GAMEDAT_LOAD_STATE_MAX
} GME_GAMEDAT_LOAD_STATE;


/// データ読み込みコンテキスト構造体
typedef struct tag_GMS_GAMEDAT_LOAD_CONTEXT {

	GME_GAMEDAT_LOAD_STATE		state;						//!< 状態

	char	file_path[GMD_GAMEDAT_FILE_PATH_LENGTH_MAX];	//!< 読み込むファイルのパス
	s32		bb_no;											//!< BBファイル読み込み時のID

    AMS_FS  *fs_req;

	const GMS_GAMEDAT_LOAD_DATA	*load_data;		//!< ロード中のデータ情報

	// 読み込みデータ別ステータス
	u16		char_id;								//!< 使用キャラクターID
	u16		ply_no;									//!< プレイヤーNO
	u16		stage_id;								//!< データ読み込み対象のステージID
	u16		data_no;								//!< 読み込み中のデータNO
	//u16		area_id;								//!< データ読み込み対象のエリアID

//	u16		stage_id;								//!< データ読み込み対象のステージID
//	u16		area_id;								//!< データ読み込み対象のエリアID
//	u16		char_id[GSD_MAIN_PLAYER_MAX];			//!< 使用キャラクターID	0xFFFFで無効
//	u16		ply_no;									//!< 読み込み中のプレイヤーデータNO

	//u16		data_index;			//!< 読み込みデータインデックス

	//u16		progress;			//!< 進捗 0～100


} GMS_GAMEDAT_LOAD_CONTEXT;


/// データロードワーク
typedef struct tag_GMS_GAMEDAT_LOAD_WORK {
	GMS_GAMEDAT_LOAD_CONTEXT	context[GMD_GAMEDAT_LOAD_CONTEXT_MAX];
	s32							context_num;

	GME_GAMEDAT_LOAD_PROC	proc_type;			//!< データ読み込みタイプ

	BOOL						load_finish;		//!< データ読み込み終了フラグ
	BOOL						post_finish;		//!< 後処理終了フラグ

	u16		stage_id;								//!< データ読み込み対象のステージID
	u16		char_id[GSD_MAIN_PLAYER_MAX];			//!< 使用キャラクターID	0xFFFFで無効
} GMS_GAMEDAT_LOAD_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmDataLoadMain(MTS_TASK_TCB *tcb);
static void gmDataLoadMainPostWait(MTS_TASK_TCB *tcb);
static void gmDataLoadDest(MTS_TASK_TCB *tcb);

static void *gmGameDatLoadAllocHead(const char *path);
//static void *gmGameDatLoadAllocTail(const char *path);
static void *gmGameDatLoadAllocHeadSub(const char *path);
//static void *gmGameDatLoadAllocTailSub(const char *path);

static GME_GAMEDAT_LOAD_STATE gmGameDatLoad(GMS_GAMEDAT_LOAD_CONTEXT *context);
static GME_GAMEDAT_LOAD_STATE gmGameDatLoadFileReq(GMS_GAMEDAT_LOAD_CONTEXT *context);

//static void gmGameDatLoadProcPostCommon(GMS_GAMEDAT_LOAD_CONTEXT *context);
//static void gmGameDatLoadProcPostCommonPreRelease(GMS_GAMEDAT_LOAD_CONTEXT *context);
static void gmGameDatLoadProcPostRing(GMS_GAMEDAT_LOAD_CONTEXT *context);
static void gmGameDatLoadProcPostPlayer(GMS_GAMEDAT_LOAD_CONTEXT *context);
static void gmGameDatLoadProcPostCockpit(GMS_GAMEDAT_LOAD_CONTEXT *context);
static void gmGameDatLoadProcPostMap(GMS_GAMEDAT_LOAD_CONTEXT *context);
#if GMD_MAP_FAR_TEST
static void gmGameDatLoadProcPostMapFar(GMS_GAMEDAT_LOAD_CONTEXT *context);
#endif //GMD_MAP_FAR_TEST
static void gmGameDatLoadProcPostEffect(GMS_GAMEDAT_LOAD_CONTEXT *context);
static void gmGameDatLoadProcPostEnemy(GMS_GAMEDAT_LOAD_CONTEXT *context);
static void gmGameDatLoadProcPostBoss(GMS_GAMEDAT_LOAD_CONTEXT *context);
static void gmGameDatLoadProcPostGimmick(GMS_GAMEDAT_LOAD_CONTEXT *context);
#if GMD_DECO_TEST
static void gmGameDatLoadProcPostDeco(GMS_GAMEDAT_LOAD_CONTEXT *context);
#endif //GMD_DECO_TEST
static void gmGameDatLoadProcPostWaterSurface(GMS_GAMEDAT_LOAD_CONTEXT *context);


//----- Global Variables ----------------------------------------------------
#include "gmGameDatTbl.inc"

/* データ読み込みワーク */
void	*g_gm_gamedat_map[GMD_GAMEDAT_MAP_MAX] = {NULL};						//!< MAPデータ
void	*g_gm_gamedat_map_set[GMD_GAMEDAT_MAPSET_MAX] = {NULL};					//!< MAPセットデータ
void	*g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_MAX] = {NULL};	//!< 追加MAPセットデータ
void	*g_gm_gamedat_map_attr_set[GMD_GAMEDAT_ATTRSET_MAX] = {NULL};			//!< MAPアトリビュートセットデータ

void	*g_gm_gamedat_enemy_arc	= NULL;								//!< エネミーデータアーカイブ

void	*g_gm_gamedat_ring[GMD_DWORK_NO_RING_END - GMD_DWORK_NO_RING_START] = {NULL};		//!< リングデータ
void	*g_gm_gamedat_gimmick[GMD_DWORK_NO_GMK_END - GMD_DWORK_NO_GMK_START] = {NULL};		//!< ギミックデータ
void	*g_gm_gamedat_enemy[GMD_DWORK_NO_ENEMY_END - GMD_DWORK_NO_ENEMY_START] = {NULL};	//!< エネミーデータ
void	*g_gm_gamedat_effect[GMD_DWORK_NO_EFFECT_ARC_END - GMD_DWORK_NO_EFFECT_ARC_START] = {NULL};	//!< エフェクトデータ

void	*g_gm_gamedat_cockpit_main_arc	= NULL;						//!< コックピットデータアーカイブ

/// ボス連戦用ステージIDテーブル
GSE_MAIN_STAGE_ID g_gm_gamedat_bossbattle_stage_id_tbl[GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX] = {
	GSD_MAIN_STAGE_ID_1_BOSS,
	GSD_MAIN_STAGE_ID_2_BOSS,
	GSD_MAIN_STAGE_ID_3_BOSS,
	GSD_MAIN_STAGE_ID_4_BOSS,
	GSD_MAIN_STAGE_ID_FINAL_1,
};
//----- Local Variables -----------------------------------------------------

static MTS_TASK_TCB				*gm_gamedat_load_tcb = NULL;	//!< データロード処理
static GMS_GAMEDAT_LOAD_WORK	*gm_gamedat_load_work = NULL;	//!< データロードTCBワーク

// シェーダー
void	*gm_gamedat_shader = NULL;			//!< シェーダーファイル
s32		gm_gamedat_shader_reg_id = -1;			//!< シェーダー登録チェック用ID

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGameDatLoadInit
/*!
 *	ゲームデータロード 初期化
 *
 *	@param	stage_id		[in]	ロードステージタイプ
 *	@param	area_id			[in]	ロードエリアタイプ
 *	@param	char_id_list	[in]	ロードキャラクタタイプバッファ
 *
 *	@note
 *		char_id_list は GSD_MAIN_PLAYER_MAX長のバッファである事\n
 *		未使用時は -1を格納\n
 *		読み込み終了後は GmGameDatLoadExit で読み込み終了処理を行って下さい。
 */
// ==========================================================================
void GmGameDatLoadInit(GME_GAMEDAT_LOAD_PROC proc_type, u16 stage_id, s16 *char_id_list)
{
	s32							i, char_cnt;
	MTS_TASK_TCB				*tcb;
	GMS_GAMEDAT_LOAD_WORK		*load_work;
	GMS_GAMEDAT_LOAD_CONTEXT	*context;
	const GMS_GAMEDAT_LOAD_INFO	*data_info;
	const GMS_GAMEDAT_LOAD_DATA	*load_data;

	MTM_ASSERT((u32)proc_type < GMD_GAMEDAT_LOAD_PROC_MAX);
	MTM_ASSERT(stage_id < GSD_MAIN_STAGE_ID_MAX);
	MTM_ASSERT(char_id_list);

	// ロード処理生成
	tcb = MTM_TASK_MAKE_TCB(gmDataLoadMain, gmDataLoadDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						GMD_TASK_PRIO_DATA_LOAD, GMD_TASK_GROUP_DATA_LOAD,
						sizeof(GMS_GAMEDAT_LOAD_WORK), "GM_LOAD");
	gm_gamedat_load_tcb = tcb;

	load_work = (GMS_GAMEDAT_LOAD_WORK*)mtTaskGetTcbWork(tcb);
	gm_gamedat_load_work = load_work;
    MI_CpuClear8(load_work, sizeof(GMS_GAMEDAT_LOAD_WORK));

	// ステージID保存
	gm_gamedat_load_work->stage_id	= stage_id;

	// キャラクターID保存
	for (i = 0; i < GSD_MAIN_PLAYER_MAX; i++) {
		MTM_ASSERT(-1 <= *(char_id_list + i) && *(char_id_list + i) < GSD_CHAR_ID_MAX);	// -1で無効
		load_work->char_id[i] = (u16)*(char_id_list + i);
	}

	// ロード処理タイプ
	load_work->proc_type = proc_type;


	/*** ファイル読み込みリクエスト発行 ***/
	context = load_work->context;

	/* 共通データ */
	// 共通データ Zoneタイプで読み分けるようにするかも 今は0番で
	data_info = &gm_gamedat_tbl_common_info_tbl[0];
#if !defined GMD_DEBUG_NO_CREATE_RING
	for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
			i++, context++, load_data++, load_work->context_num++) {
		MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);
#else
	for (i = GMD_GAMEDAT_LOAD_RING_SKIP, load_data = data_info->data_tbl + GMD_GAMEDAT_LOAD_RING_SKIP; i < data_info->num;
			i++, context++, load_data++, load_work->context_num++) {
		MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);
#endif
#if 0
#if defined(GMD_DEBUG_NO_CREATE_COCKPIT)	// コックピット無効の場合はロードをスキップ
		if (i >= GMD_GAMEDAT_LOAD_RING_SKIP) {
			break;
		}
#endif /* defined(GMD_DEBUG_NO_CREATE_COCKPIT) */
#endif // 0
		context->load_data = load_data;
		context->data_no	= (u16)i;
		gmGameDatLoad(context);
	}

	// プレイヤーデータ
	// ◆プレイヤーNO, キャラID依存でなく、すべてロードするようにする方がよいかも
	for (char_cnt = 0; char_cnt < GSD_MAIN_PLAYER_MAX; char_cnt++) {
		if (load_work->char_id[char_cnt] == (u16)GSD_CHAR_ID_INVALID) {
			continue;
		}

		data_info = &gm_gamedat_tbl_player_info_tbl[load_work->char_id[char_cnt]];

		for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
				i++, context++, load_data++, load_work->context_num++) {
			MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);

			context->load_data	= load_data;
			context->char_id	= load_work->char_id[char_cnt];
			context->ply_no		= (u16)char_cnt;
			context->data_no	= (u16)i;
			gmGameDatLoad(context);
		}
	}

	/* ステージデータ */
#if 1
	// ステージデータ
	data_info = &gm_gamedat_tbl_map_info_tbl[stage_id];
	for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
			i++, context++, load_data++, load_work->context_num++) {
		MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);

		context->load_data	= load_data;
		context->stage_id	= stage_id;
		context->data_no	= (u16)i;
		gmGameDatLoad(context);
	}
#endif

#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクトデータ
	data_info = &gm_gamedat_tbl_effect_info_tbl[stage_id];
	for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
		 	i++, context++, load_data++, load_work->context_num++) {
		MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);

		context->load_data	= load_data;
		context->stage_id	= stage_id;
		//context->data_no	= (u16)i;
		gmGameDatLoad(context);
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */

#if defined GMD_DEBUG_NO_CREATE_ENEMY
	if (0) {
#else
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_FINAL_1) {
#endif
		// 敵データ
		MTM_ASSERT(NULL == g_gm_gamedat_enemy_arc);	// ボス用アーカイブ参照
		data_info = &gm_gamedat_tbl_enemy_info_tbl[stage_id];
		for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
				i++, context++, load_data++, load_work->context_num++) {
			MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);

			context->load_data	= load_data;
			context->stage_id	= stage_id;
			//context->data_no	= (u16)i;
			gmGameDatLoad(context);
		}
	}

#if defined GMD_DEBUG_NO_CREATE_GIMMICK
	if (0) {
#else
	{
#endif
		// 共通ギミックデータ
		data_info = &gm_gamedat_tbl_gimmick_common_info_tbl[0];
		for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
				i++, context++, load_data++, load_work->context_num++) {
			MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);

			context->load_data	= load_data;
			context->stage_id	= stage_id;
			//context->data_no	= (u16)i;
			gmGameDatLoad(context);
		}

		// ギミックデータ
		data_info = &gm_gamedat_tbl_gimmick_info_tbl[stage_id];
		for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
				i++, context++, load_data++, load_work->context_num++) {
			MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);

			context->load_data	= load_data;
			context->stage_id	= stage_id;
			//context->data_no	= (u16)i;
			gmGameDatLoad(context);
		}
	}
	// サウンドデータ

}


// ==========================================================================
// GmGameDatLoadPost
/*!
 *	ゲームデータロード 後処理
 *
 *	@note
 *		GMD_GAMEDAT_LOAD_PROC_PRE_LOAD 設定でデータ読み込みをした場合の、
 *		後処理一括実行
 */
// ==========================================================================
void GmGameDatLoadPost(void)
{
	GMS_GAMEDAT_LOAD_WORK		*load_work = gm_gamedat_load_work;

	if (load_work == NULL) {
		return;
	}
	MTM_ASSERT(gm_gamedat_load_tcb);

	// 通常モードに切り替え(Post処理はgmDataLoadMain内で実行)
	load_work->proc_type = GMD_GAMEDAT_LOAD_PROC_NORMAL;
}


// ==========================================================================
// GmGameDatLoadCheck
/*!
 *	ゲームデータロード 読み込みチェック
 *
 *	@return		読み込み状況 GME_GAMEDAT_LOAD_PROGRESS
 */
// ==========================================================================
GME_GAMEDAT_LOAD_PROGRESS GmGameDatLoadCheck(void)
{
	if (gm_gamedat_load_work) {
		MTM_ASSERT(gm_gamedat_load_tcb);
		if (gm_gamedat_load_work->post_finish) {
			// すべて終了
			return (GMD_GAMEDAT_LOAD_PROGRESS_COMPLETE);
		}
		else if (gm_gamedat_load_work->load_finish) {
			// データ読み込みまで終了
			return (GMD_GAMEDAT_LOAD_PROGRESS_LOADFINISH);
		}
		// 読み込み中
		return (GMD_GAMEDAT_LOAD_PROGRESS_LOADING);
	}

	// 読み込みしていない
	return (GMD_GAMEDAT_LOAD_PROGRESS_NOLOAD);
}


// ==========================================================================
// GmGameDatLoadExit
/*!
 *	ゲームデータロード 処理終了
 */
// ==========================================================================
void GmGameDatLoadExit(void)
{
	if (gm_gamedat_load_tcb) {
		mtTaskClearTcb(gm_gamedat_load_tcb);
		OS_TPrintf( "--- GmGameDatLoadExit ---\n");
	}
}

// ==========================================================================
// データリリース
// ==========================================================================
// ==========================================================================
// GmGameDatRelease
/*!
 *	ゲームデータリリース
 *
 *	@note
 *		読み込んだデータを解放したりしています。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。
 */
// ==========================================================================
void GmGameDatRelease(void)
{
	// 後で分ける可能性もあるので、一旦中から呼び出しにしておく
	GmGameDatReleaseStandard();
	GmGameDatReleaseArea();
}

// ==========================================================================
// GmGameDatReleaseStandard
/*!
 *	ゲームデータリリース 標準データ
 *
 *	@note
 *		読み込んだデータを解放したりしています。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。
 */
// ==========================================================================
void GmGameDatReleaseStandard(void)
{
	s32	i;

	// プレイヤー
	GmPlayerRelease();


	/* サウンド */

	/* 共通データ開放 */
	// FIX, クリアデモなど
	{
		if (g_gm_gamedat_cockpit_main_arc) {
			amMemFree(g_gm_gamedat_cockpit_main_arc);
			g_gm_gamedat_cockpit_main_arc = NULL;
		}
	}
	
	// エフェクト
	// リング
	for (i = 0; i < GMD_DWORK_NO_RING_END - GMD_DWORK_NO_RING_START; i++) {
		if (g_gm_gamedat_ring[i]) {
			amMemFree(g_gm_gamedat_ring[i]);
			g_gm_gamedat_ring[i] = NULL;
		}
	}

	/* 共通先行開放データ開放 */
}

// ==========================================================================
// GmGameDatReleaseArea
/*!
 *	ゲームデータリリース エリアデータ
 *
 *	@note
 *		読み込んだデータを解放したりしています。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。
 */
// ==========================================================================
void GmGameDatReleaseArea(void)
{
	s32	i;

	/* 背景 */
	// MAP
	GmMapRelease();
	// 遠景
#if GMD_MAP_FAR_TEST
	GmMapFarRelease();
#endif //GMD_MAP_FAR_TEST

	// 装飾
#if GMD_DECO_TEST
	GmDecoRelease();
#endif //GMD_DECO_TEST
	//水面
	GmWaterSurfaceRelease();
	// イベント

	/* エフェクト */
	for (i = 0; i < GMD_DWORK_NO_EFFECT_ARC_END - GMD_DWORK_NO_EFFECT_ARC_START; ++i) {
		if (g_gm_gamedat_effect[i]) {
			amMemFree(g_gm_gamedat_effect[i]);
			g_gm_gamedat_effect[i]	= NULL;
		}
	}

	/* 敵 */
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_FINAL_1) {
		for (i = 0; i < GMD_DWORK_NO_ENEMY_END - GMD_DWORK_NO_ENEMY_START; i++) {
			if (g_gm_gamedat_enemy[i]) {
				amMemFree(g_gm_gamedat_enemy[i]);
				g_gm_gamedat_enemy[i] = NULL;
			}
		}
	}
	
	if (g_gm_gamedat_enemy_arc != NULL) {
		mtMemFreeMain(g_gm_gamedat_enemy_arc);
		g_gm_gamedat_enemy_arc = NULL;
	}
	
	/* ギミック */
	for (i = 0; i < GMD_DWORK_NO_GMK_END - GMD_DWORK_NO_GMK_START; i++) {
		if (g_gm_gamedat_gimmick[i]) {
			amMemFree(g_gm_gamedat_gimmick[i]);
			g_gm_gamedat_gimmick[i] = NULL;
		}
	}
}

// ==========================================================================
// GmGameDatReleaseCheck
/*!
 *	ゲームデータリリース 開放チェック
 *
 *	@return		TRUE : 開放処理終了
 */
// ==========================================================================
BOOL GmGameDatReleaseCheck(void)
{
	return (TRUE);
}

// ==========================================================================
// データ取得
// ==========================================================================
// ==========================================================================
// GmGameDatGetEnemyData
/*!
 *	エネミーデータ取得
 *
 *	@param	data_no	[in]	GMD_DWORK_NO_ENEMY_****
 *
 *	@return		データアドレス
 */
// ==========================================================================
void* GmGameDatGetEnemyData(s32 data_no)
{
	MTM_ASSERT(GMD_DWORK_NO_ENEMY_START <= data_no);
	MTM_ASSERT((u32)data_no < GMD_DWORK_NO_ENEMY_END);

	return (g_gm_gamedat_enemy[data_no - GMD_DWORK_NO_ENEMY_START]);
}

// ==========================================================================
// GmGameDatGetGimmickData
/*!
 *	ギミックデータ取得
 *
 *	@param	data_no	[in]	GMD_DWORK_NO_GMK_****
 *
 *	@return		データアドレス
 */
// ==========================================================================
void* GmGameDatGetGimmickData(s32 data_no)
{
	MTM_ASSERT(GMD_DWORK_NO_GMK_START <= data_no);
	MTM_ASSERT((u32)data_no < GMD_DWORK_NO_GMK_END);

	return (g_gm_gamedat_gimmick[data_no - GMD_DWORK_NO_GMK_START]);
}

// ==========================================================================
// ボス戦用データビルド
// ==========================================================================
// ==========================================================================
// GmGameDatLoadBoosBattleInit
/*!
 *	ゲームデータロード 初期化 ボス連戦用
 *
 *	@param	boss_type		[in]	ロードボスタイプ
 *
 *	@note
 *		GmGameDatLoadCheck でロードチェック
 *		GmGameDatLoadExit でロード終了
 */
// ==========================================================================
void GmGameDatLoadBoosBattleInit(GME_GAMEDAT_LOAD_BOSS_TYPE boss_type)
{
	s32							i;//, char_cnt;
	MTS_TASK_TCB				*tcb;
	GMS_GAMEDAT_LOAD_WORK		*load_work;
	GMS_GAMEDAT_LOAD_CONTEXT	*context;
	const GMS_GAMEDAT_LOAD_INFO	*data_info;
	const GMS_GAMEDAT_LOAD_DATA	*load_data;
	u16							stage_id;

	MTM_ASSERT((u32)boss_type < GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX);

	// ステージID取得
	stage_id = (u16)g_gm_gamedat_bossbattle_stage_id_tbl[boss_type];

	// ロード処理生成
	tcb = MTM_TASK_MAKE_TCB(gmDataLoadMain, gmDataLoadDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						GMD_TASK_PRIO_DATA_LOAD, GMD_TASK_GROUP_DATA_LOAD,
						sizeof(GMS_GAMEDAT_LOAD_WORK), "GM_LOAD_BB");
	gm_gamedat_load_tcb = tcb;

	load_work = (GMS_GAMEDAT_LOAD_WORK*)mtTaskGetTcbWork(tcb);
	gm_gamedat_load_work = load_work;
    MI_CpuClear8(load_work, sizeof(GMS_GAMEDAT_LOAD_WORK));

	// ステージID保存
	gm_gamedat_load_work->stage_id	= stage_id;

#if 0
	// キャラクターID保存
	for (i = 0; i < GSD_MAIN_PLAYER_MAX; i++) {
		MTM_ASSERT(-1 <= *(char_id_list + i) && *(char_id_list + i) < GSD_CHAR_ID_MAX);	// -1で無効
		load_work->char_id[i] = (u16)*(char_id_list + i);
	}
#endif

	// ロード処理タイプ
	load_work->proc_type = GMD_GAMEDAT_LOAD_PROC_NORMAL;	// 通常ロード固定


	/*** ファイル読み込みリクエスト発行 ***/
	context = load_work->context;

#if 0
#if !defined(GMD_DEBUG_NO_CREATE_EFFECT)
	// エフェクトデータ
	data_info = &gm_gamedat_tbl_effect_info_tbl[stage_id];
	for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
		 	i++, context++, load_data++, load_work->context_num++) {
		MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);

		context->load_data	= load_data;
		context->stage_id	= stage_id;
		//context->data_no	= (u16)i;
		gmGameDatLoad(context);
	}
#endif /* !defined(GMD_DEBUG_NO_CREATE_EFFECT) */
#endif

#if defined GMD_DEBUG_NO_CREATE_ENEMY
	if (0) {
#else
	{
#endif
		// 敵データ
		MTM_ASSERT(NULL == g_gm_gamedat_enemy_arc);	// ボス用アーカイブ参
#if _IPHONE
		// データロード高速化のため、専用のデータ配列を用意
		data_info = &gm_gamedat_tbl_enemy_final_info_tbl[stage_id];
#else
		data_info = &gm_gamedat_tbl_enemy_info_tbl[stage_id];
#endif 
		for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
				i++, context++, load_data++, load_work->context_num++) {
			MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);

			context->load_data	= load_data;
			context->stage_id	= stage_id;
			//context->data_no	= (u16)i;
			gmGameDatLoad(context);
		}
	}

#if 0
#if defined GMD_DEBUG_NO_CREATE_GIMMICK
	if (0) {
#else
	{
#endif
		// 共通ギミックデータ
		data_info = &gm_gamedat_tbl_gimmick_common_info_tbl[0];
		for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
				i++, context++, load_data++, load_work->context_num++) {
			MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);

			context->load_data	= load_data;
			context->stage_id	= stage_id;
			//context->data_no	= (u16)i;
			gmGameDatLoad(context);
		}

		// ギミックデータ
		data_info = &gm_gamedat_tbl_gimmick_info_tbl[stage_id];
		for (i = 0, load_data = data_info->data_tbl; i < data_info->num;
				i++, context++, load_data++, load_work->context_num++) {
			MTM_ASSERT(load_work->context_num < GMD_GAMEDAT_LOAD_CONTEXT_MAX);

			context->load_data	= load_data;
			context->stage_id	= stage_id;
			//context->data_no	= (u16)i;
			gmGameDatLoad(context);
		}
	}
#endif
}

// ==========================================================================
// ボス連戦用 データリリース
// ==========================================================================
// ==========================================================================
// GmGameDatBoosBattleRelease
/*!
 *	ゲームデータリリース ボス連戦用
 *
 *	@note
 *		読み込んだデータを解放したりしています。\n
 *		ゲーム構造体に対象のステージを設定しておいてください。
 */
// ==========================================================================
void GmGameDatBoosBattleRelease(GME_GAMEDAT_LOAD_BOSS_TYPE boss_type)
{
	s32	i, stage_id;

	MTM_ASSERT((u32)boss_type < GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX);

	// ステージID取得
	stage_id = g_gm_gamedat_bossbattle_stage_id_tbl[boss_type];

#if 0
	/* エフェクト */
	for (i = 0; i < GMD_DWORK_NO_EFFECT_ARC_END - GMD_DWORK_NO_EFFECT_ARC_START; ++i) {
		if (g_gm_gamedat_effect[i]) {
			amMemFree(g_gm_gamedat_effect[i]);
			g_gm_gamedat_effect[i]	= NULL;
		}
	}
#endif

	/* 敵 */
	for (i = 0; i < GMD_DWORK_NO_ENEMY_END - GMD_DWORK_NO_ENEMY_START; i++) {
		if (g_gm_gamedat_enemy[i]) {
			amMemFree(g_gm_gamedat_enemy[i]);
			g_gm_gamedat_enemy[i] = NULL;
		}
	}
	
	if (g_gm_gamedat_enemy_arc != NULL) {
		mtMemFreeMain(g_gm_gamedat_enemy_arc);
		g_gm_gamedat_enemy_arc = NULL;
	}

#if 0
	/* ギミック */
	for (i = 0; i < GMD_DWORK_NO_GMK_END - GMD_DWORK_NO_GMK_START; i++) {
		if (g_gm_gamedat_gimmick[i]) {
			amMemFree(g_gm_gamedat_gimmick[i]);
			g_gm_gamedat_gimmick[i] = NULL;
		}
	}
#endif
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmDataLoadMain
/*!
 *	データロードメイン処理
 *
 */
// ==========================================================================
void gmDataLoadMain(MTS_TASK_TCB *tcb)
{
	s32							i;
	GMS_GAMEDAT_LOAD_CONTEXT	*context;
	GME_GAMEDAT_LOAD_STATE		load_state;
	GME_GAMEDAT_LOAD_STATE		load_level;
	GMS_GAMEDAT_LOAD_WORK		*load_work = (GMS_GAMEDAT_LOAD_WORK*)mtTaskGetTcbWork(tcb);

	if (load_work->proc_type == GMD_GAMEDAT_LOAD_PROC_PRE_LOAD) {
		load_level = GMD_GAMEDAT_LOAD_STATE_LOADFINISH;
	}
	else {
		load_level = GMD_GAMEDAT_LOAD_STATE_COMPLETE;
	}

	// データ読み込み終了チェック
	for (i = 0, context = load_work->context; i < load_work->context_num; i++, context++) {
		load_state = gmGameDatLoad(context);
		if (load_state < load_level) {
			// 読み込み中
			return;
		}
	}

	// データ読み込み終了
	load_work->load_finish = TRUE;

	if (load_work->proc_type == GMD_GAMEDAT_LOAD_PROC_PRE_LOAD) {
		// GmGameDatLoadPost が呼ばれるまで待機
		mtTaskChangeTcbProcedure(tcb, gmDataLoadMainPostWait);
	}
	else {
		// 読み込み終了
		load_work->post_finish = TRUE;

		// メイン処理クリア
		mtTaskChangeTcbProcedure(tcb, NULL);
	}
}

// ==========================================================================
// gmDataLoadMainPostWait
/*!
 *	データロードメイン 待機処理
 *
 *	@note
 *		GMD_GAMEDAT_LOAD_PROC_PRE_LOADの時、GmGameDatLoadPostが呼ばれるまで待機
 */
// ==========================================================================
void gmDataLoadMainPostWait(MTS_TASK_TCB *tcb)
{
	GMS_GAMEDAT_LOAD_WORK	*load_work = (GMS_GAMEDAT_LOAD_WORK*)mtTaskGetTcbWork(tcb);

	if (load_work->proc_type == GMD_GAMEDAT_LOAD_PROC_NORMAL) {
		// 処理復帰
		mtTaskChangeTcbProcedure(tcb, gmDataLoadMain);
		// 先に一度実行しておく
		gmDataLoadMain(tcb);
	}
}

// ==========================================================================
// gmDataLoadDest
/*!
 *	データロードデストラクタ
 */
// ==========================================================================
void gmDataLoadDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	gm_gamedat_load_tcb = NULL;
	gm_gamedat_load_work = NULL;
}




// ==========================================================================
// アロケータ
// ==========================================================================
// @param	path	[in]	データパス
// @return	バッファ or データ読み込みタイプ
// ==========================================================================
// gmGameDatLoadAllocHead
/*!
 *	ゲームデータロードの標準メモリ確保関数 先頭確保
 */
// ==========================================================================
void *gmGameDatLoadAllocHead(const char *path)
{
	UNREFERENCED_PARAMETER(path);

	// 確保はamFsに任せる
#if _WII
	return ((void*)(AMD_FS_MALLOC_NORMAL | AMD_FS_MALLOC_MEM2));
#else
	return ((void*)AMD_FS_MALLOC_NORMAL);
#endif
}

#if 0
// ==========================================================================
// gmGameDatLoadAllocTail
/*!
 *	ゲームデータロードの標準メモリ確保関数 末尾確保
 */
// ==========================================================================
void *gmGameDatLoadAllocTail(const char *path)
{
	UNREFERENCED_PARAMETER(path);

	// 確保はamFsに任せる
#if _WII
	return ((void*)(AMD_FS_MALLOC_TEMP | AMD_FS_MALLOC_MEM2));
#else
	return ((void*)AMD_FS_MALLOC_TEMP);
#endif
}
#endif

// ==========================================================================
// gmGameDatLoadAllocHeadSub
/*!
 *	ゲームデータロードの標準メモリ確保関数 先頭確保
 */
// ==========================================================================
void *gmGameDatLoadAllocHeadSub(const char *path)
{
	UNREFERENCED_PARAMETER(path);

	// 確保はamFsに任せる
#if _WII
	return ((void*)(AMD_FS_MALLOC_NORMAL | AMD_FS_MALLOC_MEM1));
#else
	return ((void*)AMD_FS_MALLOC_NORMAL);
#endif
}

#if 0
// ==========================================================================
// gmGameDatLoadAllocTailSub
/*!
 *	ゲームデータロードの標準メモリ確保関数 末尾確保
 */
// ==========================================================================
void *gmGameDatLoadAllocTailSub(const char *path)
{
	UNREFERENCED_PARAMETER(path);

	// 確保はamFsに任せる
#if _WII
	return ((void*)(AMD_FS_MALLOC_TEMP | AMD_FS_MALLOC_MEM1));
#else
	return ((void*)AMD_FS_MALLOC_TEMP);
#endif
}
#endif

#if 0
// ==========================================================================
// gmGameDatLoadAllocSnd
/*!
 *	ゲームデータロードのサウンドメモリ確保関数
 */
// ==========================================================================
void* gmGameDatLoadAllocSnd(const char *path)
{
#if defined (MTD_DEBUG)
#ifndef MTD_CHILD
	if (mtFsGetFileSize(path) > GMD_GAMEDAT_SND_DATA_SIZE_MAX) {
		OS_TPrintf("gmGameDat::gmGameDatLoadAllocSnd() data size over!\n");
		MTM_ASSERT(0);
	}

	MTM_ASSERT(mtFsGetFileSize(path) <= GMD_GAMEDAT_SND_DATA_SIZE_MAX);
#endif
#endif // #if defined (MTD_DEBUG)

	// サウンドヒープは固定長で確保する
	return NNS_SndHeapAlloc(_mt_snd_heap_hadle, GMD_GAMEDAT_SND_DATA_SIZE_MAX, NULL, 0, 0);
}
#endif


// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// gmGameDatLoad
/*!
 *	ゲームデータロード 共通
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 */
// ==========================================================================
GME_GAMEDAT_LOAD_STATE gmGameDatLoad(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
//	const GMS_GAMEDAT_LOAD_INFO	*load_info = context->load_data;
//	GMS_GAMEDAT_LOAD_CONTEXT	*context = &gm_gamedat_load_context;


	return (gmGameDatLoadFileReq(context));
#if 0
	if (load_info->data_tbl[context->data_index].bb_no == -1) {
	// 通常ファイル
		return (gmGameDatLoadFile(load_info));
	}
	else {
	// BBファイル
		return (gmGameDatLoadBB(load_info));
	}
#endif
}

#if 0
// ==========================================================================
// gmGameDatLoadFile
/*!
 *	ゲームデータロード 通常ファイル
 *
 *	@param	load_info	[in]	ロード情報構造体
 */
// ==========================================================================
//GME_GAMEDAT_LOAD_STATE gmGameDatLoadFile(const GMS_GAMEDAT_LOAD_INFO *load_info)
GME_GAMEDAT_LOAD_STATE gmGameDatLoadFile(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	const GMS_GAMEDAT_LOAD_DATA	*load_data;
	//MTS_FS_REQUEST				fs_req;
	BOOL						auto_buf;

	if (context->state == GMD_GAMEDAT_LOAD_STATE_COMPLETE ||
			context->state == GMD_GAMEDAT_LOAD_STATE_ERROR) {
		// 処理なし
		return (context->state);
	}

	// 現在のロードデータ
	load_data = context->load_data;

	strcpy(context->file_path, load_data->path);

	// 読み込み前処理
	if (NULL != load_data->proc_pre) {
		load_data->proc_pre(context);
	}

	// ファイル読み込み
	// リクエスト読み込みタイプと合わせる為、こちらもリクエスト構造体を使用する
	MI_CpuClear8(&fs_req, sizeof(fs_req));

	// バッファ確保
	auto_buf = FALSE;
	fs_req.buf = load_data->alloc(context->file_path);
	if (fs_req.buf == MTD_FS_DEST_AUTO_ALLOC_HEAD ||
			fs_req.buf == MTD_FS_DEST_AUTO_ALLOC_TAIL) {
		auto_buf = TRUE;		// 自動取得
	}

	OS_TPrintf( "--- Load Start %s ---\n", context->file_path );
	fs_req.buf = mtFsLoadFile(context->file_path, fs_req.buf);
	OS_TPrintf( "--- Load End %s --- %d \n", context->file_path, WFS_GetStatus() );

#if GMD_GAMEDAT_USE_NET
	// FS通信をやっている場合はエラーチェックを行う
	//  ファイルオープン中に通信が切れると完了扱いになる
	if ( AZE_NET_TYPE_FS == azNetGetType() ) {
		// 親が居なくなったらエラーとする
		if (WFS_STATE_ERROR == WFS_GetStatus()
				|| WFS_STATE_STOP == WFS_GetStatus()) {

			mtFsClearRequest( context->fs_req );
			context->fs_req	= NULL;
			context->state	= GMD_GAMEDAT_LOAD_STATE_ERROR;
			return (context->state);
		}
	}
#endif // "if GMD_GAMEDAT_USE_NET

	// 読み込み完了
	// 読み込み後処理
	if (NULL != load_data->proc_post) {
		load_data->proc_post(&fs_req);
	}

	if (auto_buf && fs_req.buf) {
		// 自動開放
		mtMemFreeMain(fs_req.buf);
	}

	++context->data_index;
	if (load_info->num <= context->data_index) {
		context->state = GMD_GAMEDAT_LOAD_STATE_COMPLETE;
	}

	return (context->state);
}
#endif

// ==========================================================================
// gmGameDatLoadFileReq
/*!
 *	ゲームデータロード リクエスト発行タイプ
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 */
// ==========================================================================
GME_GAMEDAT_LOAD_STATE gmGameDatLoadFileReq(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	const GMS_GAMEDAT_LOAD_DATA	*load_data;
	void						*buf;
	BOOL						change_mallock_mode = FALSE;

	MTM_ASSERT(gm_gamedat_load_work);

	if (context->state == GMD_GAMEDAT_LOAD_STATE_COMPLETE ||
			context->state == GMD_GAMEDAT_LOAD_STATE_ERROR) {
		// 処理のなし
		return (context->state);
	}

	// 現在のロードデータ
	load_data = context->load_data;

	if (NULL == context->fs_req) {		// リクエストが発行されていない

		strcpy(context->file_path, load_data->path);

		// 読み込み前処理
		if (NULL != load_data->proc_pre) {
			load_data->proc_pre(context);
		}

		OS_TPrintf( "--- Load Start %s ---\n", context->file_path );

		// ファイル読み込みリクエスト発行
		buf = load_data->alloc(context->file_path);
		if (((s32)buf & ~AMD_FS_MALLOC_MEM) == AMD_FS_MALLOC_TEMP) {
			change_mallock_mode = TRUE;
			amFsSetMallocMode((s32)buf, 1);
			buf = NULL;
		}
		else if (((s32)buf & ~AMD_FS_MALLOC_MEM) == AMD_FS_MALLOC_NORMAL) {
			change_mallock_mode = TRUE;
			amFsSetMallocMode((s32)buf, 1);
			buf = NULL;
		}
		context->fs_req = amFsReadBackground(context->file_path, buf);
		if (change_mallock_mode) {
			amFsSetMallocMode(AMD_FS_MALLOC_NORMAL, 0);
		}

		// 進行割合を更新する◆
		//context->progress = gmGameDatComputeProgress((u16)load_info->num, context->data_index, 0, 0 );

		//OS_TPrintf( "■■■■■ NORMAL:%d%% %d/%d ■■■■■\n",
		//		GmGameDatLoadProgress(),
		//		context->data_index, load_info->num );

		// ロード開始
		context->state = GMD_GAMEDAT_LOAD_STATE_LOADING;
	}
	else if (amFsIsComplete(context->fs_req)) {


#if GMD_GAMEDAT_USE_NET
		//OS_TPrintf( "--- Load End %s --- %d \n", context->file_path, WFS_GetStatus() );

		// FS通信をやっている場合はエラーチェックを行う
		//  ファイルオープン中に通信が切れると完了扱いになる
		if ( AZE_NET_TYPE_FS == azNetGetType() ) {
			// 親が居なくなったらエラーとする
			if (WFS_STATE_ERROR == WFS_GetStatus()
					|| WFS_STATE_STOP == WFS_GetStatus()) {

				mtFsClearRequest( context->fs_req );
				context->fs_req	= NULL;
				context->state	= GMD_GAMEDAT_LOAD_STATE_ERROR;
				//return (context->state);
				return;
			}
		}
#else
		OS_TPrintf( "--- Load End %s --- \n", context->file_path);
#endif // "if GMD_GAMEDAT_USE_NET

		// 読み込み完了
		context->state = GMD_GAMEDAT_LOAD_STATE_COMPLETE;


		if (gm_gamedat_load_work->proc_type != GMD_GAMEDAT_LOAD_PROC_PRE_LOAD &&
				context->fs_req->buf) {
			// 読み込み後処理
			if (NULL != load_data->proc_post) {
				load_data->proc_post(context);
			}
			amFsClearRequest(context->fs_req);
			context->fs_req = NULL;
		}

		//++context->data_index;

		// 進行割合を更新する◆
	//	context->progress   = gmGameDatComputeProgress((u16)load_info->num, context->data_index, 0, 0 );

		//OS_TPrintf( "■■■■■ NORMAL(COMP):%d%% %d/%d ■■■■■\n",
		//	GmGameDatLoadProgress(),
		//	context->data_index, load_info->num );
	}
	else {
		// 進行割合を更新する◆
		//context->progress = gmGameDatComputeProgress((u16)load_info->num, context->data_index, 0, 0 );

		//OS_TPrintf( "■■■■■ NORMAL(LOADING):%d%% %d/%d ■■■■■\n",
		//		GmGameDatLoadProgress(),
		//		context->data_index, load_info->num );
	}

//	if (load_info->num <= context->data_index) {
//		context->state = GMD_GAMEDAT_LOAD_STATE_COMPLETE;
//	}

	return (context->state);
}



// ==========================================================================
// 前処理
// ==========================================================================
// ==========================================================================
// 後処理
// ==========================================================================
#if 0
// ==========================================================================
// gmGameDatLoadProcPostCommon
/*!
 *	ゲームデータロードの共通データ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostCommon(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	UNREFERENCED_PARAMETER(context);
}

// ==========================================================================
// gmGameDatLoadProcPostCommonPreRelease
/*!
 *	ゲームデータロードの共通先行開放データ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		ここで初期化(あるいはデータコピー)を行って破棄するデータです。
 */
// ==========================================================================
void gmGameDatLoadProcPostCommonPreRelease(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	UNREFERENCED_PARAMETER(context);
}
#endif

// ==========================================================================
// gmGameDatLoadProcPostRing
/*!
 *	ゲームデータロードの共通データ(リング)後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostRing(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	//OBS_DATA_WORK	*data_work;
	//s32				data_no;

	MTM_ASSERT(GMD_DWORK_NO_RING_START <= (u32)context->load_data->user_data);
	MTM_ASSERT((u32)context->load_data->user_data < GMD_DWORK_NO_RING_END);

	// データ本格納
	g_gm_gamedat_ring[context->load_data->user_data - GMD_DWORK_NO_RING_START] = context->fs_req->buf;

	context->fs_req->buf = NULL;
}

// ==========================================================================
// gmGameDatLoadProcPostPlayer
/*!
 *	ゲームデータロードのプレイヤーデータ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostPlayer(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	// データをデータワークに取得
	ObjDataSet(&g_gm_player_data_work[context->ply_no][context->data_no], context->fs_req->buf);
	context->fs_req->buf = NULL;
#if _WII
	//◆暫定処理
	if (context->data_no == GMD_PLAYER_DATA_SON_MDL ||
				context->data_no == GMD_PLAYER_DATA_SSON_MDL) {
		// 無理やり NND_NODETYPE_RESET_SCALING_X _Y _Z を落とす
		AMS_AMB_HEADER	*amb;
		void			*data;
		NNS_OBJECT		*object;
		NNS_TEXFILELIST	*texfilelist;
		NNS_NODE		*node;
		s32				node_num;
		s32				i, amb_cnt;

		amb = (AMS_AMB_HEADER*)g_gm_player_data_work[context->ply_no][context->data_no].pData;
		amBindConv((u8*)amb);
		for (amb_cnt = 0; amb_cnt < amb->file_num; amb_cnt++) {
			data = amBindGet(amb, amb_cnt);

			amObjectSetup(&object, &texfilelist, data);

			node	= object->pNodeList;
			node_num= object->nNode;
			for (i = 0; i < node_num; i++, node++) {
				node->fType &= ~(NND_NODETYPE_RESET_SCALING_X |
								NND_NODETYPE_RESET_SCALING_Y |
								NND_NODETYPE_RESET_SCALING_Z);
			}
		}
	}
#endif
}

// ==========================================================================
// gmGameDatLoadProcPostCockpit
/*!
 *	ゲームデータロードのコックピット（FIX,クリアデモ等）データ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostCockpit(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	AMS_AMB_HEADER	*amb	= (AMS_AMB_HEADER*)context->fs_req->buf;
	
	amBindConvertAll((u8*)amb);
	
	g_gm_gamedat_cockpit_main_arc	= amb;
	context->fs_req->buf	= NULL;
}

// ==========================================================================
// gmGameDatLoadProcPostMap
/*!
 *	ゲームデータロードの背景データ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostMap(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	//UNREFERENCED_PARAMETER(context);

#if 1

	//MTM_ASSERT(context->data_no < GMD_GAMEDAT_MAP_MAX);

	g_gm_gamedat_map[context->data_no] = context->fs_req->buf;

	switch (context->data_no) {
	case GMD_GAMEDAT_MAP_MAP_SET:
		// GME_GAMEDAT_MAPSET
		{
			AMS_AMB_HEADER	*amb;
			s32	i;

			amb = (AMS_AMB_HEADER*)context->fs_req->buf;
			amBindConv((u8*)amb);
			for (i = 0; i < GMD_GAMEDAT_MAPSET_MAX && i < amb->file_num; i++) {
				g_gm_gamedat_map_set[i] = amBindGet(amb, i);
			}

			// 追加面 (超近景 中景)
#if 0
			// 超近景
			if (amb->file_num >= GMD_GAMEDAT_MAPSET_ADD_N_MD + 1) {
				g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_N_MP] =
												amBindGet(amb, GMD_GAMEDAT_MAPSET_ADD_N_MP);
				g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_N_MD] =
												amBindGet(amb, GMD_GAMEDAT_MAPSET_ADD_N_MD);
			}
			else {
				g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_N_MP] = NULL;
				g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_N_MD] = NULL;
			}
			// 中景
			if (amb->file_num >= GMD_GAMEDAT_MAPSET_ADD_M_MD + 1) {
				g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_M_MP] =
												amBindGet(amb, GMD_GAMEDAT_MAPSET_ADD_M_MP);
				g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_M_MD] =
												amBindGet(amb, GMD_GAMEDAT_MAPSET_ADD_M_MD);
			}
			else {
				g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_M_MP] = NULL;
				g_gm_gamedat_map_set_add[GMD_GAMEDAT_MAPSET_ADD_LOCAL_M_MD] = NULL;
			}
#else
			for (i = 0; i < GMD_GAMEDAT_MAPSET_ADD_LOCAL_MAX; i += 2) {
				if (amb->file_num >= GMD_GAMEDAT_MAPSET_ADD_START + 2 + i) {
					g_gm_gamedat_map_set_add[i] =
									amBindGet(amb, GMD_GAMEDAT_MAPSET_ADD_START + i);
					g_gm_gamedat_map_set_add[i + 1] =
									amBindGet(amb, GMD_GAMEDAT_MAPSET_ADD_START + i + 1);
				}
				else {
					g_gm_gamedat_map_set_add[i]		= NULL;
					g_gm_gamedat_map_set_add[i + 1] = NULL;
				}
			}
#endif
		}
		break;
	case GMD_GAMEDAT_MAP_MODEL:
	case GMD_GAMEDAT_MAP_TEX:
#if !defined GMD_GAMEDAT_NO_CREATE_MAP_MOTION
	case GMD_GAMEDAT_MAP_MTN:
	case GMD_GAMEDAT_MAP_MMTN:
#endif // GMD_GAMEDAT_NO_CREATE_MAP_MOTION
		// データのコンバートだけでよい
		amBindConv((u8*)context->fs_req->buf);
		break;
	case GMD_GAMEDAT_MAP_ATTR:
		// GME_GAMEDAT_ATTRSET
		{
			AMS_AMB_HEADER	*amb;
			s32	i, j;
			u32 block_cnt;

			amb = (AMS_AMB_HEADER*)context->fs_req->buf;
			amBindConv((u8*)amb);
			for (i = 0; i < GMD_GAMEDAT_ATTRSET_MAX && i < amb->file_num; i++) {
				g_gm_gamedat_map_attr_set[i] = amBindGet(amb, i);
			}

			if (g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_3_2) {
				// トロッコ用に崖フラグを強制的に落とす
				AT_HEADER	*at_header	= (AT_HEADER*)g_gm_gamedat_map_attr_set[GMD_GAMEDAT_ATTRSET_AT];
				AT_BLOCK	*at_block	= (AT_BLOCK*)(((AT_HEADER*)g_gm_gamedat_map_attr_set[GMD_GAMEDAT_ATTRSET_AT]) + 1);
				u8			*at;

				for (block_cnt = 0; block_cnt < at_header->block_num; block_cnt++, at_block++) {
					at = &at_block->at[0][0];
					for (j = 0; j < 8*8; j++, at++) {
						*at &= ~OBD_COL_DATA_ATTR_CLIFF;
					}
				}
			}
		}
		break;
#if defined (MTD_DEBUG)
#if GMD_DEBUG_DUMMY_MAP_TEX
	case (GMD_GAMEDAT_MAP_ATTR + 1):
		// デバック用 TXB
		ObjDataSet(ObjDataGet(GMD_DWORK_NO_DEBUG_MAP_TXB), context->fs_req->buf);
		break;
#endif
#endif
	default:
		MTM_ASSERT(0);
		return;
	}

	context->fs_req->buf = NULL;

	// g_gm_gamedat_map


#else
	// ◆仮設定
	switch (context->data_no) {
	case GMD_GAMEDAT_MAP_MAP_A:
		g_gm_main_system.map_fcol.map_block_num_x = ((u16*)context->fs_req->buf)[0];
		g_gm_main_system.map_fcol.map_block_num_y = ((u16*)context->fs_req->buf)[1];
		g_gm_main_system.map_fcol.block_map_datap[0] = &(((u16*)context->fs_req->buf)[2]);
		break;
	case GMD_GAMEDAT_MAP_MAP_B:
		g_gm_main_system.map_fcol.block_map_datap[1] = &(((u16*)context->fs_req->buf)[2]);
		break;
	case GMD_GAMEDAT_MAP_MAP_BK:
		g_gm_main_system.map_fcol.block_datap = (u16*)context->fs_req->buf;
		break;
	case GMD_GAMEDAT_MAP_MAP_DF:
		g_gm_main_system.map_fcol.cl_diff_datap = (s8*)context->fs_req->buf;
		break;
	case GMD_GAMEDAT_MAP_MAP_DI:
		g_gm_main_system.map_fcol.direc_datap = (u8*)context->fs_req->buf;
		break;
	case GMD_GAMEDAT_MAP_MAP_AT:
		g_gm_main_system.map_fcol.char_attr_datap = (u8*)context->fs_req->buf;
		break;
	default:
		MTM_ASSERT(0);
	}

	// g_gm_gamedat_map

	context->fs_req->buf = NULL;
#endif
#if 0
    s8*  cl_diff_datap;     ///< 4bytes: 当たり差分データ先頭アドレス
    u8*  direc_datap;       ///< 4bytes: 当たり角度データ先頭アドレス
    u16* block_datap;       ///< 4bytes: ブロックデータ先頭アドレス 
    u16* block_map_datap[2];///< 4bytes: マップ先頭アドレス
    u8*  char_attr_datap;   ///< 4bytes: キャラクタ属性データ先頭アドレス 
    u16  map_block_num_x;   ///< 2bytes: マップ ブロック数X
    u16  map_block_num_y;   ///< 2bytes: マップ ブロック数Y
    s32  left;              ///< 4bytes: マップ開始位置 1:31
    s32  top;               ///< 4bytes: マップ開始位置 1:31
    s32  right;             ///< 4bytes: マップ終了位置 1:31
#endif
}
// ==========================================================================
// gmGameDatLoadProcPostMapFar
/*!
 *	ゲームデータロードの遠景データ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostMapFar(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	//ambコンバート
	GmMapFarInitData( (AMS_AMB_HEADER*)context->fs_req->buf );
	context->fs_req->buf = NULL;
}

// ==========================================================================
// gmGameDatLoadProcPostEffect
/*!
 *	ゲームデータロードのエフェクトデータ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostEffect(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
#if 0
	OBS_DATA_WORK	*data_work;
	AMS_AMB_HEADER	*amb	= (AMS_AMB_HEADER*)context->fs_req->buf;
	
	amBindConv((u8*)amb);
	
	data_work	= ObjDataGet(context->load_data->user_data);
	MTM_ASSERT(data_work->num == 0);
	ObjDataSet(data_work, (void*)amb);
	context->fs_req->buf	= NULL;
#else
	AMS_AMB_HEADER	*amb	= (AMS_AMB_HEADER*)context->fs_req->buf;
	
	amBindConv((u8*)amb);
	
	// データ本格納
	g_gm_gamedat_effect[context->load_data->user_data - GMD_DWORK_NO_EFFECT_START] = (void*)amb;
	context->fs_req->buf	= NULL;
#endif
}

// ==========================================================================
// gmGameDatLoadProcPostEnemy
/*!
 *	ゲームデータロードの敵(ザコ)データ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostEnemy(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	//OBS_DATA_WORK	*data_work;

	MTM_ASSERT(GMD_DWORK_NO_ENEMY_START <= (u32)context->load_data->user_data);
	MTM_ASSERT((u32)context->load_data->user_data < GMD_DWORK_NO_ENEMY_END);

	// データ本格納
	g_gm_gamedat_enemy[context->load_data->user_data - GMD_DWORK_NO_ENEMY_START] = context->fs_req->buf;

	context->fs_req->buf = NULL;
}

// ==========================================================================
// gmGameDatLoadProcPostBoss
/*!
 *	ゲームデータロードのボスデータ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostBoss(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	UNREFERENCED_PARAMETER(context);
	
	AMS_AMB_HEADER	*amb	= (AMS_AMB_HEADER*)context->fs_req->buf;
	
	amBindConv((u8*)amb);
	g_gm_gamedat_enemy_arc	= amb;
	context->fs_req->buf	= NULL;
}

// ==========================================================================
// gmGameDatLoadProcPostGimmick
/*!
 *	ゲームデータロードのギミックデータ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostGimmick(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	//OBS_DATA_WORK	*data_work;

	MTM_ASSERT(GMD_DWORK_NO_GMK_START <= (u32)context->load_data->user_data);
	MTM_ASSERT((u32)context->load_data->user_data < GMD_DWORK_NO_GMK_END);

	// データ本格納
	g_gm_gamedat_gimmick[context->load_data->user_data - GMD_DWORK_NO_GMK_START] = context->fs_req->buf;

	context->fs_req->buf = NULL;
}

// ==========================================================================
// gmGameDatLoadProcPostDeco
/*!
 *	ゲームデータロードの装飾データ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostDeco(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	//ambコンバート
	GmDecoInitData( (AMS_AMB_HEADER*)context->fs_req->buf );
	context->fs_req->buf = NULL;
}

// ==========================================================================
// gmGameDatLoadProcPostWaterSurface
/*!
 *	ゲームデータロードの水面データ後処理
 *
 *	@param	context	[in]	データ読み込みコンテキスト構造体
 *
 *	@note
 *		fs_req->bufに読み込まれたデータが格納されています。\n
 *		自動メモリ確保している場合、req->buf!=NULLだとメモリが解放されます。\n
 *		解放されたくない場合はアドレスを控えてreq->bufにNULLを代入してください。
 */
// ==========================================================================
void gmGameDatLoadProcPostWaterSurface(GMS_GAMEDAT_LOAD_CONTEXT *context)
{
	//ambコンバート
	GmWaterSurfaceInitData( (AMS_AMB_HEADER*)context->fs_req->buf );
	context->fs_req->buf = NULL;
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
