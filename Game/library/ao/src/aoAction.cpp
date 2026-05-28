// ===========================================================================
/*!
	@file	aoAction.cpp
	@brief	AoLibrary 2Dアクションランタイム定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"
#include "format/Ama.h"

// ----- Macros ------------------------------------------------（マクロ定義）

// アクションフラグ AOS_ACTION::flag
#define AOD_ACT_FLAG_UPDATE				(BIT_0)	//!< 要更新

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_ACT_SORT
// ---------------------------------------------------------------------------
//!	スプライトソート構造体
// ===========================================================================
typedef struct tag_AOS_ACT_SORT {
	AOS_SPRITE*		sprite;			//!< スプライト
	f32				z;				//!< Z値
} AOS_ACT_SORT; // 8 byte

// ===========================================================================
//	struct AOS_ACT_DRAW
// ---------------------------------------------------------------------------
//!	描画タスクワーク
// ===========================================================================
typedef struct tag_AOS_ACT_DRAW {
	u32				count;			//!< 描画スプライト数
	AOS_SPRITE*		sprite;			//!< 描画スプライト配列

#if defined(AOD_DEBUG)
	BOOL			show_hit;		//!< 真：当たり表示
	u32				hit_color;		//!< 当たり表示色
#endif // defined(AOD_DEBUG)
} AOS_ACT_DRAW; // 8 byt

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// スプライト
static void aoActAcmSprite(AOS_SPRITE* spr);

// アクション
static AOS_ACTION* AoActCreateNodeSub(
	const void* ama, u32 id, f32 frame, BOOL sib);
static void aoActApply(AOS_ACTION* act);

static void aoActSearchTrsKey(const A2S_AMA_ACT* act, f32* frame, s32* key);
static void aoActSearchMtnKey(const A2S_AMA_ACT* act, f32* frame, s32* key);
static void aoActSearchAnmKey(const A2S_AMA_ACT* act, f32* frame, s32* key);
static void aoActSearchMatKey(const A2S_AMA_ACT* act, f32* frame, s32* key);
static void aoActSearchAcmTrsKey(const A2S_AMA_ACT* act, f32* frame, s32* key);
static void aoActSearchAcmMtnKey(const A2S_AMA_ACT* act, f32* frame, s32* key);
static void aoActSearchAcmMatKey(const A2S_AMA_ACT* act, f32* frame, s32* key);
static void aoActSearchHitKey(const A2S_AMA_ACT* act, f32* frame, s32* key);

static void aoActMakeTrs(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_TRS* trs_tbl, s32 key, f32 frame,
	f32* trans_x, f32* trans_y, f32* trans_z);
static void aoActMakeMtn(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_MTN* mtn_tbl, s32 key, f32 frame,
	f32* scale_x, f32* scale_y, f32* rotate);
static void aoActMakeAnm(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_ANM* anm_tbl, s32 key, f32 frame,
	s32* tex_id, AOS_ACT_RECT* rect, u32* clamp);
static void aoActMakeMat(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_MAT* mat_tbl, s32 key, f32 frame,
	AOS_ACT_COL* color, AOS_ACT_COL* fade, u32* blend);
static void aoActMakeAcm(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_ACM* acm_tbl, s32 key, f32 frame,
	f32* tscale_x, f32* tscale_y, f32* scale_x, f32* scale_y, f32* rotate);
static void aoActMakeHit(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_HIT* hit_tbl, s32 key, f32 frame,
	AOS_ACT_HITP* hit);

static void aoActSerachKey(
	const A2S_SUB_KEY* key, u32 key_num, f32* frame, s32* last);
static f32 aoActGetLoopFrame(f32 frame, u32 len);
static BOOL aoActGetInterpolInfo(
	const A2S_SUB_KEY* key_tbl, u32 key_num, u32 frm_num, f32 frame,
	u32 key1, u32* key2, f32* rate);
static void aoActGetInterpolSpline(
	const A2S_SUB_TRS* t0, const A2S_SUB_TRS* t1,
	const A2S_SUB_TRS* t2, const A2S_SUB_TRS* t3, f32 rate,
	f32* trans_x, f32* trans_y, f32* trans_z);
static f32 aoActGetAcceleRate(f32 rate, f32 accele);

// バッファ操作
static AOS_SPRITE* aoActAllocSprite(void);
static void aoActFreeSprite(AOS_SPRITE* spr);
static AOS_ACTION* aoActAllocAction(void);
static void aoActFreeAction(AOS_ACTION* act);

// アクション終了判定
static u32 aoActGetAmaActState(const A2S_AMA_ACT* act, f32 frame);
static BOOL aoActIsAmaActEnd(const A2S_AMA_ACT* act, f32 frame);
static BOOL aoActIsAmaTrsEnd(const A2S_AMA_MTN* mtn, f32 frame);
static BOOL aoActIsAmaMtnEnd(const A2S_AMA_MTN* mtn, f32 frame);
static BOOL aoActIsAmaAnmEnd(const A2S_AMA_ANM* anm, f32 frame);
static BOOL aoActIsAmaMatEnd(const A2S_AMA_ANM* anm, f32 frame);
static BOOL aoActIsAmaAcmTrsEnd(const A2S_AMA_ACM* acm, f32 frame);
static BOOL aoActIsAmaAcmMtnEnd(const A2S_AMA_ACM* acm, f32 frame);
static BOOL aoActIsAmaAcmMatEnd(const A2S_AMA_ACM* acm, f32 frame);
static BOOL aoActIsAmaUsrEnd(const A2S_AMA_USR* usr, f32 frame);
static BOOL aoActIsAmaHitEnd(const A2S_AMA_HIT* hit, f32 frame);

// AMAアクセス
static const A2S_AMA_ACT* aoActGetAmaAct(const AOS_ACTION* act);

// ユーティリティ
static f32 aoActInterpolF32(f32 d1, f32 d2, f32 rate);
static AOS_ACT_COL aoActInterpolCol(A2S_SUB_COL d1, A2S_SUB_COL d2, f32 rate);

// 描画
static void aoActDrawTask(AMS_TCB* tcb);
static void aoActDrawSprState(AOS_SPRITE** spr_tbl, u32 num = 1);
static void aoActDrawSortState(void);
void aoActDrawCorW(NNS_PRIM3D_P* v, u32 vnum, u32 flag);
void aoActDrawCorW(NNS_PRIM3D_PC* v, u32 vnum, u32 flag);
void aoActDrawCorW(NNS_PRIM3D_PCT* v, u32 vnum, u32 flag);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ***************************************************************************
// システム変数
// ***************************************************************************
// ===========================================================================
//! 描画タスク優先度
// ===========================================================================
static u32 g_ao_act_sys_draw_prio = AOD_ACT_SYS_DEF_DTASK_PRIO;

// ===========================================================================
//! 描画ステート有効フラグ
// ===========================================================================
static BOOL g_ao_act_sys_draw_state_enable = FALSE;

// ===========================================================================
//! 描画ステート
// ===========================================================================
static u32 g_ao_act_sys_draw_state = 0;

// ===========================================================================
//! フレームレート
// ===========================================================================
static f32 g_ao_act_sys_frame_rate = 1.0f;

// ===========================================================================
//! 横方向アジャスト値
// ===========================================================================
static f32 g_ao_act_sys_adjust_x = 0.0f;

// ===========================================================================
//! 縦方向アジャスト値
// ===========================================================================
static f32 g_ao_act_sys_adjust_y = 0.0f;

// ===========================================================================
//! メモリ一括確保用バッファ
// ===========================================================================
static u8* g_ao_act_master_buf = NULL;

// ===========================================================================
//! ワイド補正時端位置シフト値
// ===========================================================================
static const s32 g_ao_act_wide_shift = 24;


// ***************************************************************************
// スプライト変数
// ***************************************************************************
// ===========================================================================
//! スプライトバッファ
// ===========================================================================
static AOS_SPRITE* g_ao_act_spr_buf = NULL;

// ===========================================================================
//! スプライト参照バッファ
// ===========================================================================
static AOS_SPRITE** g_ao_act_spr_ref = NULL;

// ===========================================================================
//! スプライト確保位置
// ===========================================================================
static u32 g_ao_act_spr_alloc = 0;

// ===========================================================================
//! スプライト解放位置
// ===========================================================================
static u32 g_ao_act_spr_free = 0;

// ===========================================================================
//! スプライトバッファサイズ
// ===========================================================================
static u32 g_ao_act_spr_buf_size = 0;

// ===========================================================================
//! スプライトバッファ使用数
// ===========================================================================
static u32 g_ao_act_spr_num = 0;

// ===========================================================================
//! スプライトバッファ最大使用数
// ===========================================================================
static u32 g_ao_act_spr_peak = 0;


// ***************************************************************************
// アクション変数
// ***************************************************************************
// ===========================================================================
//! アクションバッファ
// ===========================================================================
static AOS_ACTION* g_ao_act_buf = NULL;

// ===========================================================================
//! アクション参照バッファ
// ===========================================================================
static AOS_ACTION** g_ao_act_ref = NULL;

// ===========================================================================
//! アクション確保位置
// ===========================================================================
static u32 g_ao_act_alloc = 0;

// ===========================================================================
//! アクション解放位置
// ===========================================================================
static u32 g_ao_act_free = 0;

// ===========================================================================
//! アクションバッファサイズ
// ===========================================================================
static u32 g_ao_act_buf_size = 0;

// ===========================================================================
//! アクションバッファ使用数
// ===========================================================================
static u32 g_ao_act_num = 0;

// ===========================================================================
//! アクションバッファ最大使用数
// ===========================================================================
static u32 g_ao_act_peak = 0;


// ***************************************************************************
// ソート変数
// ***************************************************************************
// ===========================================================================
//! ソートバッファ
// ===========================================================================
static AOS_ACT_SORT* g_ao_act_sort_buf = NULL;

// ===========================================================================
//! ソートバッファサイズ
// ===========================================================================
static u32 g_ao_act_sort_buf_size = 0;

// ===========================================================================
//! ソートバッファ使用数
// ===========================================================================
static u32 g_ao_act_sort_num = 0;

// ===========================================================================
//! ソートバッファ最大使用数
// ===========================================================================
static u32 g_ao_act_sort_peak = 0;


// ***************************************************************************
// アキュムレート変数
// ***************************************************************************
// ===========================================================================
//! アキュムレートバッファ
// ===========================================================================
static AOS_ACT_ACM* g_ao_act_acm_buf = NULL;

// ===========================================================================
//! アキュムレートバッファカレント
// ===========================================================================
static AOS_ACT_ACM* g_ao_act_acm_cur = NULL;

// ===========================================================================
//! アキュムレートバッファサイズ
// ===========================================================================
static u32 g_ao_act_acm_buf_size = 0;

// ===========================================================================
//! アキュムレートバッファ使用数
// ===========================================================================
static u32 g_ao_act_acm_num = 0;

// ===========================================================================
//! アキュムレートバッファ最大使用数
// ===========================================================================
static u32 g_ao_act_acm_peak = 0;

// ===========================================================================
//! アキュムレートフラグバッファ
// ===========================================================================
static u32* g_ao_act_acm_flag_buf = NULL;

// ===========================================================================
//! アキュムレートフラグバッファカレント
// ===========================================================================
static u32* g_ao_act_acm_flag_cur = NULL;

// ===========================================================================
//! アキュムレートフラグバッファサイズ
// ===========================================================================
static u32 g_ao_act_acm_flag_buf_size = 0;

// ===========================================================================
//! アキュムレートフラグバッファ使用数
// ===========================================================================
static u32 g_ao_act_acm_flag_num = 0;

// ===========================================================================
//! アキュムレートフラグバッファ最大使用数
// ===========================================================================
static u32 g_ao_act_acm_flag_peak = 0;


// ***************************************************************************
// テクスチャ変数
// ***************************************************************************
// ===========================================================================
//! テクスチャリスト
// ===========================================================================
static NNS_TEXLIST* g_ao_act_texlist = NULL;


#if defined(AOD_DEBUG)

// ***************************************************************************
// デバッグ変数
// ***************************************************************************
// ===========================================================================
//! 当たり表示フラグ
// ===========================================================================
static BOOL g_ao_act_debug_show_hit_flag = FALSE;

// ===========================================================================
//! 当たり表示色
// ===========================================================================
static u32 g_ao_act_debug_show_hit_color = 0xffffffff;

#endif // defined(AOD_DEBUG)

// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! 4:3横解像度
// ===========================================================================
static const f32 g_ao_act_base_w = 960.0f;

// ===========================================================================
//! 4:3縦解像度
// ===========================================================================
static const f32 g_ao_act_base_h = 720.0f;

#if defined(AOD_PLATFORM_IPHONE)
// ===========================================================================
//! 3:2横解像度
// ===========================================================================
static const f32 g_ao_act_wide_w = 1080.0f;
#else
// ===========================================================================
//! 16:9横解像度
// ===========================================================================
static const f32 g_ao_act_wide_w = 1280.0f;
#endif

// ===========================================================================
//! 16:9縦解像度
// ===========================================================================
static const f32 g_ao_act_wide_h = 720.0f;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ***************************************************************************
// システム
// ***************************************************************************
// ===========================================================================
//	AoActSysInit
/*!
	システム初期化

	@param spr_buf_num		[in] スプライトバッファ数
	@param act_buf_num		[in] アクションバッファ数
	@param sort_buf_num		[in] ソートバッファ数
	@param acm_stack_num	[in] アキュムレートスタック数
	@note
	システムの初期化を行い、このモジュールを使用できるようにします。\n
	このモジュール内の関数を使用する前に呼び出すようにして下さい。\n
	\n
	スプライトバッファ数は、
	AoActSprCreate関数で確保できるAOS_SPRITE構造体の数となります。
	(モジュール内部で自動的に確保される数も含みます)\n
	アクションバッファ数は、
	AoActCreate関数で確保できるAOS_ACTION構造体の数となります。
	(ノードを指定した場合は、一度の呼び出しでノード数分確保されます)\n
	ソートバッファ数は、
	AoActSortRegSprite関数で登録できるAOS_SPRITE構造体の数となります。
	(AoActSortRegAction関数を使用した場合はノード数分の登録がされます)\n
	アキュムレートスタック数は、
	AoActAcmPush関数でプッシュできるアキュムレートの数となります。
	また、AoActAcmPushFlag関数でプッシュできるフラグの数も、
	これと同じになります。\n
	\n
	各種バッファは、内部で自動確保され、
	AoActSysExit関数が呼び出されるまで保持されます。\n
	この関数を多重で呼び出した場合はアサートし何も行いません。\n
*/
// ===========================================================================
void AoActSysInit(
	u32 spr_buf_num, u32 act_buf_num, u32 sort_buf_num, u32 acm_stack_num)
{
	// 初期化済み判定
	if (g_ao_act_master_buf) {
		// 初期化済みなので何もしない
		amAssert(0);
		return;
	}

	// 最小値チェック
	if (spr_buf_num == 0) {
		spr_buf_num = 1;
	}
	if (act_buf_num == 0) {
		act_buf_num = 1;
	}
	if (sort_buf_num == 0) {
		sort_buf_num = 1;
	}
	if (acm_stack_num == 0) {
		acm_stack_num = 1;
	}

	// 必要メモリサイズ算出
	u32 size = 0;
	u32 pos = 0;
	size += sizeof(AOS_SPRITE) * spr_buf_num;
	size += sizeof(AOS_SPRITE*) * spr_buf_num;
	size += sizeof(AOS_ACTION) * act_buf_num;
	size += sizeof(AOS_ACTION*) * act_buf_num;
	size += sizeof(AOS_ACT_SORT) * sort_buf_num;
	size += sizeof(AOS_ACT_ACM) * acm_stack_num;
	size += sizeof(u32) * acm_stack_num;

	// メモリ確保
	g_ao_act_master_buf = (u8*)amMemAlloc(size);

	// スプライトバッファ確保
	g_ao_act_spr_buf_size = spr_buf_num;
	if (spr_buf_num > 0) {
		g_ao_act_spr_buf = (AOS_SPRITE*)(g_ao_act_master_buf + pos);
		pos += sizeof(AOS_SPRITE) * spr_buf_num;
		g_ao_act_spr_ref = (AOS_SPRITE**)(g_ao_act_master_buf + pos);
		pos += sizeof(AOS_SPRITE*) * spr_buf_num;
	}
	else {
		g_ao_act_spr_buf = NULL;
		g_ao_act_spr_ref = NULL;
	}

	// アクションバッファ確保
	g_ao_act_buf_size = act_buf_num;
	if (act_buf_num > 0) {
		g_ao_act_buf = (AOS_ACTION*)(g_ao_act_master_buf + pos);
		pos += sizeof(AOS_ACTION) * act_buf_num;
		g_ao_act_ref = (AOS_ACTION**)(g_ao_act_master_buf + pos);
		pos += sizeof(AOS_ACTION*) * act_buf_num;
	}
	else {
		g_ao_act_buf = NULL;
		g_ao_act_ref = NULL;
	}

	// ソートバッファ確保
	g_ao_act_sort_buf_size = sort_buf_num;
	if (sort_buf_num > 0) {
		g_ao_act_sort_buf = (AOS_ACT_SORT*)(g_ao_act_master_buf + pos);
		pos += sizeof(AOS_ACT_SORT) * sort_buf_num;
	}
	else {
		g_ao_act_sort_buf = NULL;
	}

	// アキュムレートバッファ確保
	g_ao_act_acm_buf_size = acm_stack_num;
	if (acm_stack_num > 0) {
		g_ao_act_acm_buf = (AOS_ACT_ACM*)(g_ao_act_master_buf + pos);
		pos += sizeof(AOS_ACT_ACM) * acm_stack_num;
	}
	else {
		g_ao_act_acm_buf = NULL;
	}

	// アキュムレートフラグバッファ確保
	g_ao_act_acm_flag_buf_size = acm_stack_num;
	if (acm_stack_num > 0) {
		g_ao_act_acm_flag_buf = (u32*)(g_ao_act_master_buf + pos);
		pos += sizeof(u32) * acm_stack_num;
	}
	else {
		g_ao_act_acm_flag_buf = NULL;
	}

	amAssert(pos == size);

	// リセット
	AoActSysReset();
}

// ===========================================================================
//	AoActSysReset
/*!
	システムリセット

	@note
	システムの状態を、AoActSysInit関数呼び出し直後の状態に戻します。\n
	このモジュールから確保したデータは
	全て使用できなくなるので注意して下さい。\n
	AoActSysInit関数を呼び出さずに、この関数を呼び出した場合は、
	アサートし何も行いません。\n
*/
// ===========================================================================
void AoActSysReset(void)
{
	// 未初期化判定
	if (!g_ao_act_master_buf) {
		// 未初期化なので何もしない
		amAssert(0);
		return;
	}

	// システム初期化
	g_ao_act_sys_draw_prio = AOD_ACT_SYS_DEF_DTASK_PRIO;
	g_ao_act_sys_frame_rate = 1.0f;
	g_ao_act_sys_adjust_x = 0.0f;
	g_ao_act_sys_adjust_y = 0.0f;

	// スプライトバッファ作成
	if (g_ao_act_spr_buf_size > 0) {
		amZeroMemory(
			g_ao_act_spr_buf, sizeof(AOS_SPRITE) * g_ao_act_spr_buf_size);
		amZeroMemory(
			g_ao_act_spr_ref, sizeof(AOS_SPRITE*) * g_ao_act_spr_buf_size);
		for (u32 i = 0; i < g_ao_act_spr_buf_size; ++i) {
			g_ao_act_spr_ref[i] = &g_ao_act_spr_buf[i];
		}
	}
	g_ao_act_spr_alloc = 0;
	g_ao_act_spr_free = 0;
	g_ao_act_spr_num = 0;

	// アクションバッファ作成
	if (g_ao_act_buf_size > 0) {
		amZeroMemory(g_ao_act_buf, sizeof(AOS_ACTION) * g_ao_act_buf_size);
		amZeroMemory(g_ao_act_ref, sizeof(AOS_ACTION*) * g_ao_act_buf_size);
		for (u32 i = 0; i < g_ao_act_buf_size; ++i) {
			g_ao_act_ref[i] = &g_ao_act_buf[i];
		}
	}
	g_ao_act_alloc = 0;
	g_ao_act_free = 0;
	g_ao_act_num = 0;

	// ソートバッファ作成
	if (g_ao_act_sort_buf_size > 0) {
		amZeroMemory(
			g_ao_act_sort_buf, sizeof(AOS_ACT_SORT) * g_ao_act_sort_buf_size);
	}
	g_ao_act_sort_num = 0;

	// アキュムレートバッファ作成
	g_ao_act_acm_cur = g_ao_act_acm_buf;
	g_ao_act_acm_num = 1;
	if (g_ao_act_acm_buf_size > 0) {
		amZeroMemory(
			g_ao_act_acm_buf, sizeof(AOS_ACT_ACM) * g_ao_act_acm_buf_size);
		AoActAcmInit();
	}

	// アキュムレートフラグバッファ作成
	g_ao_act_acm_flag_cur = g_ao_act_acm_flag_buf;
	g_ao_act_acm_flag_num = 1;
	if (g_ao_act_acm_flag_buf_size > 0) {
		amZeroMemory(
			g_ao_act_acm_flag_buf, sizeof(u32) * g_ao_act_acm_flag_buf_size);
		AoActAcmSetFlag();
	}

	// ピーク使用量クリア
	AoActSysClearPeak();

	// テクスチャ初期化
	g_ao_act_texlist = NULL;
}

// ===========================================================================
//	AoActSysExit
/*!
	システム終了処理

	@note
	システムの状態を、AoActSysInit関数呼び出し前の状態に戻します。\n
	内部で確保したリソースの解放処理も行います。\n
	このモジュールから確保したデータは
	全て使用できなくなるので注意して下さい。\n
	AoActSysInit関数を呼び出さずに、この関数を呼び出した場合は、
	アサートし何も行いません。\n
	多重でこの関数を呼び出した場合は、何も行いません。\n
*/
// ===========================================================================
void AoActSysExit(void)
{
	// メモリ解放
	if (g_ao_act_master_buf) {
		amMemFree(g_ao_act_master_buf);
		g_ao_act_master_buf = NULL;
	}

	// グローバル変数初期化
	g_ao_act_sys_frame_rate = 1.0f;
	g_ao_act_sys_adjust_x = 0.0f;
	g_ao_act_sys_adjust_y = 0.0f;
	g_ao_act_master_buf = NULL;
	g_ao_act_spr_buf = NULL;
	g_ao_act_spr_ref = NULL;
	g_ao_act_spr_alloc = 0;
	g_ao_act_spr_free = 0;
	g_ao_act_spr_buf_size = 0;
	g_ao_act_spr_num = 0;
	g_ao_act_spr_peak = 0;
	g_ao_act_buf = NULL;
	g_ao_act_ref = NULL;
	g_ao_act_alloc = 0;
	g_ao_act_free = 0;
	g_ao_act_buf_size = 0;
	g_ao_act_num = 0;
	g_ao_act_peak = 0;
	g_ao_act_sort_buf = NULL;
	g_ao_act_sort_buf_size = 0;
	g_ao_act_sort_num = 0;
	g_ao_act_sort_peak = 0;
	g_ao_act_acm_buf = NULL;
	g_ao_act_acm_cur = NULL;
	g_ao_act_acm_buf_size = 0;
	g_ao_act_acm_num = 0;
	g_ao_act_acm_peak = 0;
	g_ao_act_acm_flag_buf = NULL;
	g_ao_act_acm_flag_cur = NULL;
	g_ao_act_acm_flag_buf_size = 0;
	g_ao_act_acm_flag_num = 0;
	g_ao_act_acm_flag_peak = 0;
	g_ao_act_texlist = NULL;
}

// ===========================================================================
//	AoActSysGetSprBufferSize
/*!
	スプライトバッファサイズ取得

	@return スプライトバッファサイズ
	@note
	AoActSysInit関数で指定したspr_buf_numの値がそのまま返ります。\n
*/
// ===========================================================================
u32 AoActSysGetSprBufferSize(void)
{
	return g_ao_act_spr_buf_size;
}

// ===========================================================================
//	AoActSysGetSprBufferRemain
/*!
	スプライトバッファ残りサイズ取得

	@return スプライトバッファ残りサイズ
*/
// ===========================================================================
u32 AoActSysGetSprBufferRemain(void)
{
	return (u32)(g_ao_act_spr_buf_size - g_ao_act_spr_num);
}

// ===========================================================================
//	AoActSysGetSprBufferPeak
/*!
	スプライトバッファ最大使用サイズ取得

	@return スプライトバッファ最大使用サイズ
*/
// ===========================================================================
u32 AoActSysGetSprBufferPeak(void)
{
	return g_ao_act_spr_peak;
}

// ===========================================================================
//	AoActSysGetActBufferSize
/*!
	アクションバッファサイズ取得

	@return アクションバッファサイズ
	@note
	AoActSysInit関数で指定したact_buf_numの値がそのまま返ります。\n
*/
// ===========================================================================
u32 AoActSysGetActBufferSize(void)
{
	return g_ao_act_buf_size;
}

// ===========================================================================
//	AoActSysGetActBufferRemain
/*!
	アクションバッファ残りサイズ取得

	@return アクションバッファ残りサイズ
*/
// ===========================================================================
u32 AoActSysGetActBufferRemain(void)
{
	return (u32)(g_ao_act_buf_size - g_ao_act_num);
}

// ===========================================================================
//	AoActSysGetActBufferPeak
/*!
	アクションバッファ最大使用サイズ取得

	@return アクションバッファ最大使用サイズ
*/
// ===========================================================================
u32 AoActSysGetActBufferPeak(void)
{
	return g_ao_act_peak;
}

// ===========================================================================
//	AoActSysGetSortBufferSize
/*!
	ソートバッファサイズ取得

	@return ソートバッファサイズ
	@note
	AoActSysInit関数で指定したsort_buf_numの値がそのまま返ります。\n
*/
// ===========================================================================
u32 AoActSysGetSortBufferSize(void)
{
	return g_ao_act_sort_buf_size;
}

// ===========================================================================
//	AoActSysGetSortBufferRemain
/*!
	ソートバッファ残りサイズ取得

	@return ソートバッファ残りサイズ
*/
// ===========================================================================
u32 AoActSysGetSortBufferRemain(void)
{
	return (u32)(g_ao_act_sort_buf_size - g_ao_act_sort_num);
}

// ===========================================================================
//	AoActSysGetSortBufferPeak
/*!
	ソートバッファ最大使用サイズ取得

	@return ソートバッファ最大使用サイズ
*/
// ===========================================================================
u32 AoActSysGetSortBufferPeak(void)
{
	return g_ao_act_sort_peak;
}

// ===========================================================================
//	AoActSysGetAcmStackSize
/*!
	アキュムレートスタックサイズ取得

	@return アキュムレートスタックサイズ
	@note
	AoActSysInit関数で指定したacm_stack_numの値がそのまま返ります。\n
*/
// ===========================================================================
u32 AoActSysGetAcmStackSize(void)
{
	return g_ao_act_acm_buf_size;
}

// ===========================================================================
//	AoActSysGetAcmStackRemain
/*!
	アキュムレートスタック残りサイズ取得

	@return アキュムレートスタック残りサイズ
*/
// ===========================================================================
u32 AoActSysGetAcmStackRemain(void)
{
	return (u32)(g_ao_act_acm_buf_size - g_ao_act_acm_num);
}

// ===========================================================================
//	AoActSysGetAcmBufferPeak
/*!
	アキュムレートスタック最大使用サイズ取得

	@return アキュムレートスタック最大使用サイズ
*/
// ===========================================================================
u32 AoActSysGetAcmBufferPeak(void)
{
	return g_ao_act_acm_peak;
}

// ===========================================================================
//	AoActSysGetAcmFlagStackSize
/*!
	アキュムレートフラグスタックサイズ取得

	@return アキュムレートフラグスタックサイズ
	@note
	AoActSysInit関数で指定したacm_stack_numの値がそのまま返ります。\n
*/
// ===========================================================================
u32 AoActSysGetAcmFlagStackSize(void)
{
	return g_ao_act_acm_flag_buf_size;
}

// ===========================================================================
//	AoActSysGetAcmFlagStackRemain
/*!
	アキュムレートフラグスタック残りサイズ取得

	@return アキュムレートフラグスタック残りサイズ
*/
// ===========================================================================
u32 AoActSysGetAcmFlagStackRemain(void)
{
	return (u32)(g_ao_act_acm_flag_buf_size - g_ao_act_acm_flag_num);
}

// ===========================================================================
//	AoActSysGetAcmFlagBufferPeak
/*!
	アキュムレートフラグスタック最大使用サイズ取得

	@return アキュムレートフラグスタック最大使用サイズ
*/
// ===========================================================================
u32 AoActSysGetAcmFlagBufferPeak(void)
{
	return g_ao_act_acm_flag_peak;
}

// ===========================================================================
//	AoActSysClearPeak
/*!
	各種最大使用量のクリア
*/
// ===========================================================================
void AoActSysClearPeak(void)
{
	g_ao_act_spr_peak = 0;
	g_ao_act_peak = 0;
	g_ao_act_sort_peak = 0;
	g_ao_act_acm_peak = 0;
	g_ao_act_acm_flag_peak = 0;
}

// ===========================================================================
//	AoActSysSetDrawTaskPrio
/*!
	描画タスク優先度設定

	@param prio				[in] 描画タスク優先度
*/
// ===========================================================================
void AoActSysSetDrawTaskPrio(u32 prio)
{
	g_ao_act_sys_draw_prio = prio;
}

// ===========================================================================
//	AoActSysSetDrawTaskPrio
/*!
	描画タスク優先度取得

	@return 描画タスク優先度
*/
// ===========================================================================
u32 AoActSysGetDrawTaskPrio(void)
{
	return g_ao_act_sys_draw_prio;
}

// ===========================================================================
//	AoActSysSetDrawStateEnable
/*!
	描画ステート有効設定

	@param enable			[in] 真：有効　偽：無効
	@note
	描画ステートが有効な場合は、描画にamDrawPrimitive3Dを使用します。\n
	描画ステートが無効な場合は、独自の描画スレッドを作成し、
	NN関数を直接呼び出す形で描画を行います。\n
	デフォルトは無効となっています。\n
	描画ステートの設定はAoActSysSetDrawState関数で行って下さい。\n
*/
// ===========================================================================
void AoActSysSetDrawStateEnable(BOOL enable)
{
	g_ao_act_sys_draw_state_enable = enable;
}

// ===========================================================================
//	AoActSysGetDrawStateEnable
/*!
	描画ステート有効判定

	@return 真：有効　偽：無効
*/
// ===========================================================================
BOOL AoActSysGetDrawStateEnable(void)
{
	return g_ao_act_sys_draw_state_enable;
}

// ===========================================================================
//	AoActSysSetDrawState
/*!
	描画ステート設定

	@param state			[in] 描画ステート
*/
// ===========================================================================
void AoActSysSetDrawState(u32 state)
{
	g_ao_act_sys_draw_state = state;
}

// ===========================================================================
//	AoActSysGetDrawState
/*!
	描画ステート取得

	@return 描画ステート
*/
// ===========================================================================
u32 AoActSysGetDrawState(void)
{
	return g_ao_act_sys_draw_state;
}

// ===========================================================================
//	AoActSysSetFrameRate
/*!
	システムフレームレート設定

	@param rate				[in] システムフレームレート
	@note
	アクションのフレームを更新する際に、
	個々のアクションに指定した更新フレームと、
	ここで指定した値を掛け合わせた値が実際に更新されるフレームとなります。
	(0.5を指定すると速度が半分に、2.0を指定すると倍速となります)\n
	デフォルトは1となります。\n
*/
// ===========================================================================
void AoActSysSetFrameRate(f32 rate)
{
	g_ao_act_sys_frame_rate = rate;
}

// ===========================================================================
//	AoActSysGetFrameRate
/*!
	システムフレームレート取得

	@return システムフレームレート
*/
// ===========================================================================
f32 AoActSysGetFrameRate(void)
{
	return g_ao_act_sys_frame_rate;
}

// ===========================================================================
//	AoActSysSetAdjust
/*!
	システムアジャスト設定

	@param x				[in] 横方向アジャスト値
	@param x				[in] 縦方向アジャスト値
	@note
	アクションを描画する際に、
	ここで指定したアジャスト値分だけ位置がオフセットされて表示されます。\n
	各種描画処理を呼び出した段階でのアジャスト値が使用されるので、
	描画処理を呼び出す度に値を変えることも可能です。\n
	デフォルトは、縦横ともに0(アジャストしない)となります。\n
*/
// ===========================================================================
void AoActSysSetAdjust(f32 x, f32 y)
{
	g_ao_act_sys_adjust_x = x;
	g_ao_act_sys_adjust_y = y;
}

// ===========================================================================
//	AoActSysAddAdjust
/*!
	システムアジャスト加算

	@param x				[in] 横方向加算アジャスト値
	@param x				[in] 縦方向加算アジャスト値
	@note
	現在のシステムアジャスト値に指定の値を加算します。\n
*/
// ===========================================================================
void AoActSysAddAdjust(f32 x, f32 y)
{
	g_ao_act_sys_adjust_x += x;
	g_ao_act_sys_adjust_y += y;
}

// ===========================================================================
//	AoActSysGetAdjustX
/*!
	横方向システムアジャスト値取得

	@return 横方向加算アジャスト値
*/
// ===========================================================================
f32 AoActSysGetAdjustX(void)
{
	return g_ao_act_sys_adjust_x;
}

// ===========================================================================
//	AoActSysGetAdjustY
/*!
	縦方向システムアジャスト値取得

	@return 縦方向加算アジャスト値
*/
// ===========================================================================
f32 AoActSysGetAdjustY(void)
{
	return g_ao_act_sys_adjust_y;
}


// ***************************************************************************
// テクスチャ
// ***************************************************************************
// ===========================================================================
//	AoActSetTexture
/*!
	テクスチャ設定

	@param texlist	[in] テクスチャリスト
	@note
	スプライトの生成時、ソート登録時、描画時に、
	ここで設定されているテクスチャが適用されます。\n
*/
// ===========================================================================
void AoActSetTexture(NNS_TEXLIST* texlist)
{
	g_ao_act_texlist = texlist;
}

// ===========================================================================
//	AoActGetTexture
/*!
	テクスチャ取得

	@return テクスチャリスト
*/
// ===========================================================================
NNS_TEXLIST* AoActGetTexture(void)
{
	return g_ao_act_texlist;
}


// ***************************************************************************
// スプライト
// ***************************************************************************
// ===========================================================================
//	AoActSprCreate
/*!
	スプライト作成

	@param ama				[in] AMAファイル
	@param id				[in] アクションID
	@param frame			[in] フレーム
	@return 作成されたスプライト構造体
	@note
	指定AMAファイルの指定IDのアクションが指定フレームの状態での
	スプライトを作成します。\n
	この関数で作成したスプライトは、
	必ずAoActSprDelete関数で削除するようにして下さい。\n
	スプライトバッファが不足している場合は、アサートしNULLを返します。\n
	不正な引数を指定した場合は、アサートしNULLを返します。\n
*/
// ===========================================================================
AOS_SPRITE* AoActSprCreate(const void* ama, u32 id, f32 frame)
{
	// スプライト確保
	AOS_SPRITE* spr = aoActAllocSprite();
	if (spr == NULL) {
		return NULL;
	}

	// アクション反映
	AoActSprApply(spr, ama, id, frame);

	return spr;
}

// ===========================================================================
//	AoActSprDelete
/*!
	スプライト削除

	@param spr				[io] 削除するスプライト構造体
	@note
	AoActSprCreate関数で作成したスプライトを削除します。\n
	ソートバッファに登録されている場合は、ソートバッファから削除されます。\n
	不正なスプライトを指定した場合は、アサートし何も行いません。\n
*/
// ===========================================================================
void AoActSprDelete(AOS_SPRITE* spr)
{
	// 解放
	aoActFreeSprite(spr);
}

// ===========================================================================
//	AoActSprApply
/*!
	スプライトアクション反映

	@param spr				[out] 適用先スプライト
	@param ama				[in]  AMAファイル
	@param id				[in]  アクションID
	@param frame			[in]  フレーム
	@note
	指定のスプライトに指定のAMAファイルの指定IDのアクションが
	指定フレームの状態を適用します。\n
*/
// ===========================================================================
void AoActSprApply(AOS_SPRITE* spr, const void* ama, u32 id, f32 frame)
{
	// 注意：この関数での処理はaoActApply関数とほぼ同じものとなります。
	// この関数を修正した場合はaoActApply関数も修正するようにしてください。
	// 将来的には処理を統合する予定です。

	// AMAヘッダ取得
	const A2S_AMA_HEADER* head = (const A2S_AMA_HEADER*)ama;

	// AMAアクション取得
	const A2S_AMA_ACT* ama_act = head->act_tbl[id];

	// 作成アクション取得とフレーム算出
	while (ama_act->next) {
		if ((f32)ama_act->frm_num > frame) {
			break;
		}
		else {
			frame -= (f32)ama_act->frm_num;
			ama_act = ama_act->next;
		}
	}

	// キーフレーム検索
	f32 trs_f = frame;
	s32 trs_key = -1;
	aoActSearchTrsKey(ama_act, &trs_f, &trs_key);
	f32 mtn_f = frame;
	s32 mtn_key = -1;
	aoActSearchMtnKey(ama_act, &mtn_f, &mtn_key);
	f32 anm_f = frame;
	s32 anm_key = -1;
	aoActSearchAnmKey(ama_act, &anm_f, &anm_key);
	f32 mat_f = frame;
	s32 mat_key = -1;
	aoActSearchMatKey(ama_act, &mat_f, &mat_key);
	f32 hit_f = frame;
	s32 hit_key = -1;
	aoActSearchHitKey(ama_act, &hit_f, &hit_key);

	// スプライト作成
	spr->flag = ama_act->flag;
	spr->offset.left = ama_act->ofst.left;
	spr->offset.right = ama_act->ofst.right;
	spr->offset.top = ama_act->ofst.top;
	spr->offset.bottom = ama_act->ofst.bottom;

	f32 scale_x, scale_y;
	if (ama_act->mtn == NULL) {
		spr->center_x = 0.0f;
		spr->center_y = 0.0f;
		spr->prio = 0.0f;
		spr->rotate = 0.0f;
	}
	else {
		aoActMakeTrs(
			ama_act->mtn->trs_key_num, ama_act->mtn->trs_frm_num,
			ama_act->mtn->trs_key_tbl, ama_act->mtn->trs_tbl,
			trs_key, trs_f,
			&spr->center_x, &spr->center_y, &spr->prio);
		aoActMakeMtn(
			ama_act->mtn->mtn_key_num, ama_act->mtn->mtn_frm_num,
			ama_act->mtn->mtn_key_tbl, ama_act->mtn->mtn_tbl,
			mtn_key, mtn_f,
			&scale_x, &scale_y, &spr->rotate);
		spr->offset.left *= scale_x;
		spr->offset.right *= scale_x;
		spr->offset.top *= scale_y;
		spr->offset.bottom *= scale_y;
	}

	if (ama_act->anm == NULL) {
		spr->tex_id = -1;
		spr->color.r = 255;
		spr->color.g = 255;
		spr->color.b = 255;
		spr->color.a = 255;
		spr->fade.r = 0;
		spr->fade.g = 0;
		spr->fade.b = 0;
		spr->fade.a = 0;
	}
	else {
		aoActMakeAnm(
			ama_act->anm->anm_key_num, ama_act->anm->anm_frm_num,
			ama_act->anm->anm_key_tbl, ama_act->anm->anm_tbl,
			anm_key, anm_f,
			&spr->tex_id, &spr->uv, &spr->clamp);
		aoActMakeMat(
			ama_act->anm->mat_key_num, ama_act->anm->mat_frm_num,
			ama_act->anm->mat_key_tbl, ama_act->anm->mat_tbl,
			mat_key, mat_f,
			&spr->color, &spr->fade, &spr->blend);
	}

	if (ama_act->hit == NULL) {
		spr->hit.type = AOD_ACT_HIT_NONE;
	}
	else {
		aoActMakeHit(
			ama_act->hit->hit_key_num, ama_act->hit->hit_frm_num,
			ama_act->hit->hit_key_tbl, ama_act->hit->hit_tbl,
			hit_key, hit_f,
			&spr->hit);
		spr->hit.scale_x = scale_x;
		spr->hit.scale_y = scale_y;
	}

	// スプライトにアキュムレート反映
	aoActAcmSprite(spr);
}

// ===========================================================================
//	AoActSprDraw
/*!
	スプライト描画

	@param spr				[in] スプライト構造体
	@note
	スプライトの描画を行います。\n
	内部で描画タスクが作成され、実際の描画は描画スレッドにて行われます。\n
	この関数を呼び出した回数分の描画タスクが生成されてしまうので、
	可能であれば、AoActSortDraw関数で描画を行うようにして下さい。\n
	ここで指定するスプライト構造体は、
	AoActSprCreate関数で作成したスプライトでなくても問題ありません。\n
*/
// ===========================================================================
void AoActSprDraw(AOS_SPRITE* spr)
{
	if (g_ao_act_sys_draw_state_enable) {
		// ステート描画
		aoActDrawSprState(&spr);
	}
	else {
		// 必要バッファサイズ算出
		u32 size = sizeof(AOS_SPRITE) + sizeof(AOS_ACT_DRAW);

		// バッファ確保
		u8* buf = (u8*)amDrawMallocDataBuffer((s32)size);

		// 描画タスクワーク作成
		AOS_ACT_DRAW* work = (AOS_ACT_DRAW*)(buf + sizeof(AOS_SPRITE));
		work->count = 1;
		work->sprite = (AOS_SPRITE*)buf;
#if defined(AOD_DEBUG)
		work->show_hit = AoActDebugGetShowHitFlag();
		work->hit_color = AoActDebugGetShowHitColor();
#endif // defined(AOD_DEBUG)

		// スプライトバッファコピー
		amCopyMemory(work->sprite, spr, sizeof(AOS_SPRITE));

		// 描画タスク作成
		amDrawMakeTask(aoActDrawTask, (u16)g_ao_act_sys_draw_prio, (u32)work);
	}
}


// ***************************************************************************
// アクション
// ***************************************************************************
// ===========================================================================
//	AoActCreate
/*!
	アクション作成

	@param ama				[in] AMAファイル
	@param id				[in] アクションID
	@param frame			[in] フレーム
	@return 作成されたアクション構造体
	@note
	指定AMAファイルの指定IDのアクションが指定フレームの状態での
	アクションを作成します。\n
	この関数で作成したアクションは、
	必ずAoActDelete関数で削除するようにして下さい。\n
	アクションバッファが不足している場合は、アサートしNULLを返します。\n
	不正な引数を指定した場合は、アサートしNULLを返します。\n
*/
// ===========================================================================
AOS_ACTION* AoActCreate(const void* ama, u32 id, f32 frame)
{
	// ヘッダ取得
	const A2S_AMA_HEADER* head = (const A2S_AMA_HEADER*)ama;
	if (id >= head->act_num) {
		amAssert(0);
		return NULL;
	}

	// アクション取得
	A2S_AMA_ACT* base = head->act_tbl[id];

	// アクション確保
	AOS_ACTION* act = aoActAllocAction();
	if (act == NULL) {
		return NULL;
	}

	// アクション作成
	AoActAcmPush();
	AoActAcmFlagPush(0, (u32)-1);

	act->data = base;
	act->flag = 0;
	act->state = 0;
	act->type = AOD_ACT_TYPE_ACTION;
	act->frame = frame;
	act->last_key.trs = -1;
	act->last_key.mtn = -1;
	act->last_key.anm = -1;
	act->last_key.mat = -1;
	act->last_key.atrs = -1;
	act->last_key.amtn = -1;
	act->last_key.amat = -1;
	act->last_key.usr = -1;
	act->last_key.hit = -1;
	act->child = NULL;
	act->sibling = NULL;
	act->sprite = AoActSprCreate(ama, id, frame);
	if (act->sprite == NULL) {
		AoActDelete(act);
		act = NULL;
		AoActAcmFlagPop();
		AoActAcmPop();
		return NULL;
	}

	AoActAcmFlagPop();
	AoActAcmPop();

	return act;
}

// ===========================================================================
//	AoActCreateNode
/*!
	アクション作成（ノード）

	@param ama				[in] AMAファイル
	@param id				[in] ノードID
	@param frame			[in] フレーム
	@return 作成されたアクション構造体
	@note
	指定AMAファイルの指定IDのノードが指定フレームの状態での
	アクションを作成します。\n
	ノードが複数の階層を持っている場合、
	2つ以上のアクションが確保されルートノードのアクションを返します。\n
	この関数で作成したアクションは、
	必ずAoActDelete関数で削除するようにして下さい。\n
	アクションバッファが不足している場合は、
	アサートし作成できた分のアクションを返します。(NULLの場合もあります)\n
	不正な引数を指定した場合は、アサートしNULLを返します。\n
*/
// ===========================================================================
AOS_ACTION* AoActCreateNode(const void* ama, u32 id, f32 frame)
{
	return AoActCreateNodeSub(ama, id, frame, FALSE);
}

// ===========================================================================
//! AoActCreateNode関数のサブ処理(弟ノード処理の有無判定付)
// ===========================================================================
AOS_ACTION* AoActCreateNodeSub(const void* ama, u32 id, f32 frame, BOOL sib)
{
	// ヘッダ取得
	const A2S_AMA_HEADER* head = (const A2S_AMA_HEADER*)ama;
	if (id >= head->node_num) {
		amAssert(0);
		return NULL;
	}

	// ノード取得
	A2S_AMA_NODE* base = head->node_tbl[id];

	// アクション確保
	AOS_ACTION* act = aoActAllocAction();
	if (act == NULL) {
		return NULL;
	}

	// アクション作成
	AoActAcmPush();
	AoActAcmFlagPush(0, (u32)-1);

	act->data = base;
	act->flag = 0;
	act->state = 0;
	act->type = AOD_ACT_TYPE_NODE;
	act->frame = frame;
	act->last_key.trs = -1;
	act->last_key.mtn = -1;
	act->last_key.anm = -1;
	act->last_key.mat = -1;
	act->last_key.atrs = -1;
	act->last_key.amtn = -1;
	act->last_key.amat = -1;
	act->last_key.usr = -1;
	act->last_key.hit = -1;
	act->child = NULL;
	act->sibling = NULL;
	if (base->act) {
		act->sprite = AoActSprCreate(ama, base->act->id, frame);
		if (act->sprite == NULL) {
			AoActDelete(act);
			act = NULL;
			AoActAcmFlagPop();
			AoActAcmPop();
			return NULL;
		}
	}
	else {
		act->sprite = NULL;
	}

	// 子ノードを処理
	if (base->child) {
		act->child = AoActCreateNodeSub(ama, base->child->id, frame, TRUE);
		if (act->child == NULL) {
			AoActAcmFlagPop();
			AoActAcmPop();
			return act;
		}
	}

	AoActAcmFlagPop();
	AoActAcmPop();

	// 弟ノードを処理
	if (sib && base->sibling) {
		act->sibling = AoActCreateNodeSub(ama, base->sibling->id, frame, TRUE);
		if (act->sibling == NULL) {
			return act;
		}
	}

	return act;
}

// ===========================================================================
//	AoActDelete
/*!
	アクション削除

	@param spr				[io] 削除するアクション構造体
	@note
	AoActCreate関数、もしくはAoActCreateNode関数で作成した
	アクションを削除します。\n
	ソートバッファに登録されている場合は、ソートバッファから削除されます。\n
	不正なアクションを指定した場合は、アサートし何も行いません。\n
*/
// ===========================================================================
void AoActDelete(AOS_ACTION* act)
{
	// 弟ノードを削除
	if (act->sibling) {
		AoActDelete(act->sibling);
		act->sibling = NULL;
	}

	// 子ノードを削除
	if (act->child) {
		AoActDelete(act->child);
		act->child = NULL;
	}

	// 自身のスプライトを削除
	if (act->sprite) {
		AoActSprDelete(act->sprite);
		act->sprite = NULL;
	}

	// 自身を削除
	aoActFreeAction(act);
}

// ===========================================================================
//	AoActSetFrame
/*!
	アクションフレーム削除

	@param act				[io] アクション構造体
	@param frame			[in] 設定するフレーム
	@note
	指定のアクションを指定のフレームでの状態にします。\n
	通常、1フレームずつ更新する場合などは、
	AoActUpdate関数を使用したほうが高速に処理されます。\n
*/
// ===========================================================================
void AoActSetFrame(AOS_ACTION* act, f32 frame)
{
	do {
		act->frame = frame;
		act->flag |= AOD_ACT_FLAG_UPDATE;

		if (act->child) {
			AoActSetFrame(act->child, frame);
		}

		act = act->sibling;

	} while (act);
}

// ===========================================================================
//	AoActUpdate
/*!
	アクション更新

	@param act				[io] アクション構造体
	@param frame			[in] 更新するフレーム数
	@note
	指定のアクションを指定のフレームだけ更新した状態にします。\n
	実際に更新されるフレーム数は、
	frameにシステムフレームレートを掛け合わせた値となります。\n
*/
// ===========================================================================
void AoActUpdate(AOS_ACTION* act, f32 frame)
{
	// フレーム更新
	act->frame += g_ao_act_sys_frame_rate * frame;
	if (act->frame < 0.0f) {
		act->frame = 0.0f;
	}
	act->flag |= AOD_ACT_FLAG_UPDATE;

	// 子にも同じフレームを設定
	if (act->child) {
		AoActSetFrame(act->child, act->frame);
	}

	// スプライト反映
	aoActApply(act);

	// 状態更新
	act->state = aoActGetAmaActState(aoActGetAmaAct(act), act->frame);
}

// ===========================================================================
//	AoActDraw
/*!
	アクション描画

	@param act				[io] アクション構造体
	@param sort				[in] 真：ソートする　偽：ソートしない
	@note
	アクションの描画を行います。\n
	内部で描画タスクが作成され、実際の描画は描画スレッドにて行われます。\n
	この関数を呼び出した回数分の描画タスクが生成されてしまうので、
	可能であれば、AoActSortDraw関数で描画を行うようにして下さい。\n
*/
// ===========================================================================
void AoActDraw(AOS_ACTION* act, BOOL sort)
{
	// ソートバッファに空きが無ければ無理
	if (g_ao_act_sort_num >= g_ao_act_sort_buf_size) {
		amAssert(0);
		return;
	}

	// 無理やりソートバッファを借りて処理する

	// 現状のソートバッファを退避
	AOS_ACT_SORT* sort_buf = g_ao_act_sort_buf;
	u32 sort_buf_size = g_ao_act_sort_buf_size;
	u32 sort_num = g_ao_act_sort_num;
	u32 sort_peak = g_ao_act_sort_peak;

	// ソートバッファを変更
	g_ao_act_sort_buf = &g_ao_act_sort_buf[g_ao_act_sort_num];
	g_ao_act_sort_buf_size -= g_ao_act_sort_num;
	g_ao_act_sort_num = 0;
	g_ao_act_sort_peak = 0;

	// ソートバッファ登録
	AoActSortRegAction(act);

	// 使用バッファ数保持
	u32 use = g_ao_act_sort_num;

	if (sort) {
		// ソート実行
		AoActSortExecute();
	}

	// 描画
	AoActSortDraw();

	// ソートバッファクリア
	AoActSortUnregAll();

	// ソートバッファを復帰
	g_ao_act_sort_buf = sort_buf;
	g_ao_act_sort_buf_size = sort_buf_size;
	g_ao_act_sort_num = sort_num;
	g_ao_act_sort_peak = sort_peak;
	if ((g_ao_act_sort_num + use) > g_ao_act_sort_peak) {
		g_ao_act_sort_peak = g_ao_act_sort_num + use;
	}
}

// ===========================================================================
//	AoActGetState
/*!
	アクション状態取得

	@param act				[in] アクション構造体
	@return アクション状態(AOD_ACT_STATE_***_END)
*/
// ===========================================================================
u32 AoActGetState(const AOS_ACTION* act)
{
	if (act) {
		return act->state;
	}
	return AOD_ACT_STATE_ALL_END;
}

// ===========================================================================
//	AoActIsEnd
/*!
	アクション終了判定

	@param act				[in] アクション構造体
	@return 真：終了済み　偽：それ以外
*/
// ===========================================================================
BOOL AoActIsEnd(const AOS_ACTION* act)
{
	if (act) {
		return aoActIsAmaActEnd(aoActGetAmaAct(act), act->frame);
	}
	return TRUE;
}

// ===========================================================================
//	AoActIsEndTrs
/*!
	アクション トランス終了判定

	@param act				[in] アクション構造体
	@return 真：終了済み　偽：それ以外
*/
// ===========================================================================
BOOL AoActIsEndTrs(const AOS_ACTION* act)
{
	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);
	if (ama_act) {
		return aoActIsAmaTrsEnd(ama_act->mtn, act->frame);
	}
	return TRUE;
}

// ===========================================================================
//	AoActIsEndMtn
/*!
	アクション モーション終了判定

	@param act				[in] アクション構造体
	@return 真：終了済み　偽：それ以外
*/
// ===========================================================================
BOOL AoActIsEndMtn(const AOS_ACTION* act)
{
	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);
	if (ama_act) {
		return aoActIsAmaMtnEnd(ama_act->mtn, act->frame);
	}
	return TRUE;
}

// ===========================================================================
//	AoActIsEndAnm
/*!
	アクション アニメ終了判定

	@param act				[in] アクション構造体
	@return 真：終了済み　偽：それ以外
*/
// ===========================================================================
BOOL AoActIsEndAnm(const AOS_ACTION* act)
{
	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);
	if (ama_act) {
		return aoActIsAmaAnmEnd(ama_act->anm, act->frame);
	}
	return TRUE;
}

// ===========================================================================
//	AoActIsEndMat
/*!
	アクション マテリアル終了判定

	@param act				[in] アクション構造体
	@return 真：終了済み　偽：それ以外
*/
// ===========================================================================
BOOL AoActIsEndMat(const AOS_ACTION* act)
{
	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);
	if (ama_act) {
		return aoActIsAmaMatEnd(ama_act->anm, act->frame);
	}
	return TRUE;
}

// ===========================================================================
//	AoActIsEndAcmTrs
/*!
	アクション 継承トランス終了判定

	@param act				[in] アクション構造体
	@return 真：終了済み　偽：それ以外
*/
// ===========================================================================
BOOL AoActIsEndAcmTrs(const AOS_ACTION* act)
{
	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);
	if (ama_act) {
		return aoActIsAmaAcmTrsEnd(ama_act->acm, act->frame);
	}
	return TRUE;
}

// ===========================================================================
//	AoActIsEndAcmMtn
/*!
	アクション 継承モーション終了判定

	@param act				[in] アクション構造体
	@return 真：終了済み　偽：それ以外
*/
// ===========================================================================
BOOL AoActIsEndAcmMtn(const AOS_ACTION* act)
{
	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);
	if (ama_act) {
		return aoActIsAmaAcmMtnEnd(ama_act->acm, act->frame);
	}
	return TRUE;
}

// ===========================================================================
//	AoActIsEndAcmMat
/*!
	アクション 継承マテリアル終了判定

	@param act				[in] アクション構造体
	@return 真：終了済み　偽：それ以外
*/
// ===========================================================================
BOOL AoActIsEndAcmMat(const AOS_ACTION* act)
{
	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);
	if (ama_act) {
		return aoActIsAmaAcmMatEnd(ama_act->acm, act->frame);
	}
	return TRUE;
}

// ===========================================================================
//	AoActIsEndUsr
/*!
	アクション ユーザ終了判定

	@param act				[in] アクション構造体
	@return 真：終了済み　偽：それ以外
*/
// ===========================================================================
BOOL AoActIsEndUsr(const AOS_ACTION* act)
{
	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);
	if (ama_act) {
		return aoActIsAmaUsrEnd(ama_act->usr, act->frame);
	}
	return TRUE;
}

// ===========================================================================
//	AoActIsEndHit
/*!
	アクション 当たり終了判定

	@param act				[in] アクション構造体
	@return 真：終了済み　偽：それ以外
*/
// ===========================================================================
BOOL AoActIsEndHit(const AOS_ACTION* act)
{
	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);
	if (ama_act) {
		return aoActIsAmaHitEnd(ama_act->hit, act->frame);
	}
	return TRUE;
}


// ***************************************************************************
// ソート
// ***************************************************************************
// ===========================================================================
//	AoActSortRegSprite
/*!
	スプライトのソートバッファ登録

	@param spr				[in] スプライト構造体
	@note
	ソートバッファに指定のスプライトを登録します。\n
	登録したスプライトはAoActSortUnregSprite関数、
	もしくはAoActSortUnregAll関数を呼び出すまで
	ソートバッファに登録されたままになりますので、複数回の登録は不要です。\n
	既に登録済みのスプライトを指定した場合は何も行いません。\n
	ソートバッファが不足している場合は、アサートし何も行いません。\n
*/
// ===========================================================================
void AoActSortRegSprite(AOS_SPRITE* spr)
{
	// オーバーチェック
	if (g_ao_act_sort_num >= g_ao_act_sort_buf_size) {
		amAssert(0);
		return;
	}

	// 登録
	g_ao_act_sort_buf[g_ao_act_sort_num].sprite = spr;
	g_ao_act_sort_buf[g_ao_act_sort_num].z = spr->prio;

	// 使用数更新
	g_ao_act_sort_num += 1;
	if (g_ao_act_sort_num > g_ao_act_sort_peak) {
		g_ao_act_sort_peak = g_ao_act_sort_num;
	}
}

// ===========================================================================
//	AoActSortUnregSprite
/*!
	スプライトのソートバッファ登録解除

	@param spr				[in] スプライト構造体
	@note
	ソートバッファに登録されている指定のスプライトを登録解除します。\n
	指定のスプライトが登録されていない場合は、何も行いません。\n
*/
// ===========================================================================
void AoActSortUnregSprite(AOS_SPRITE* spr)
{
	// 検索
	for (u32 i = 0; i < g_ao_act_sort_num; ++i) {
		if (g_ao_act_sort_buf[i].sprite == spr) {
			amCopyMemory(
				&g_ao_act_sort_buf[i], &g_ao_act_sort_buf[i + 1],
				sizeof(AOS_ACT_SORT) * (g_ao_act_sort_num - i - 1));

			// 使用数更新
			g_ao_act_sort_num -= 1;
			if (g_ao_act_sort_num > g_ao_act_sort_peak) {
				g_ao_act_sort_peak = g_ao_act_sort_num;
			}
			break;
		}
	}
}

// ===========================================================================
//	AoActSortRegAction
/*!
	アクションのソートバッファ登録

	@param act				[in] アクション構造体
	@note
	ソートバッファに指定のアクションを登録します。\n
	登録したアクションはAoActSortUnregAction関数、
	もしくはAoActSortUnregAll関数を呼び出すまで
	ソートバッファに登録されたままになりますので、複数回の登録は不要です。\n
	既に登録済みのアクションを指定した場合は何も行いません。\n
	ソートバッファが不足している場合は、
	アサートし登録できる分だけを登録します。\n
*/
// ===========================================================================
void AoActSortRegAction(AOS_ACTION* act)
{
	if (act->sprite) {
		AoActSortRegSprite(act->sprite);
	}
	if (act->child) {
		AoActSortRegAction(act->child);
	}
	if (act->sibling) {
		AoActSortRegAction(act->sibling);
	}
}

// ===========================================================================
//	AoActSortUnregAction
/*!
	アクションのソートバッファ登録解除

	@param act				[in] アクション構造体
	@note
	ソートバッファに登録されている指定のアクションを登録解除します。\n
	指定のアクションが登録されていない場合は、何も行いません。\n
*/
// ===========================================================================
void AoActSortUnregAction(AOS_ACTION* act)
{
	if (act->sprite) {
		AoActSortUnregSprite(act->sprite);
	}
	if (act->child) {
		AoActSortUnregAction(act->child);
	}
	if (act->sibling) {
		AoActSortUnregAction(act->sibling);
	}
}

// ===========================================================================
//	AoActSortUnregAction
/*!
	ソートバッファの全要素の登録解除

	@note
	ソートバッファに登録されている全ての要素を登録解除します。\n
	ソートバッファに何も登録されていない場合は、何も行いません。\n
*/
// ===========================================================================
void AoActSortUnregAll(void)
{
	g_ao_act_sort_num = 0;
}

// ===========================================================================
//	AoActSortExecute
/*!
	ソート実行

	@note
	ソートバッファに登録されている要素をZ順にソートします。\n
	ソート結果は保持されるので、Z順が変わらないのであれば、
	以降のソート処理は不要です。\n
*/
// ===========================================================================
void AoActSortExecute(void)
{
	// バブルソート(zの小さいほうが前になるように)
	AOS_ACT_SORT* buf = g_ao_act_sort_buf;
	for (u32 i = 0; i < g_ao_act_sort_num; ++i) {
		for (u32 j = i + 1; j < g_ao_act_sort_num; ++j) {
			if (buf[j].z > buf[j - 1].z) {
				AOS_ACT_SORT temp = buf[j];
				buf[j] = buf[j - 1];
				buf[j - 1] = temp;
			}
		}
	}
}

#if defined(AOD_PLATFORM_IPHONE)
// ===========================================================================
//	AoActSortExecuteFix
/*!
	ソート実行(不具合修正)

	@note
	AoActSortExecuteでは不具合に依りソートしきらない場合があります。
	この関数はその不具合修正版です。
*/
// ===========================================================================
extern void AoActSortExecuteFix(void)
{
	// バブルソート(zの小さいほうが前になるように)
	AOS_ACT_SORT* buf = g_ao_act_sort_buf;
	for (u32 i = 0; i < g_ao_act_sort_num; ++i) {
		for (u32 j = g_ao_act_sort_num - 1; i < j; --j) {
			if (buf[j].z > buf[j - 1].z) {
				AOS_ACT_SORT temp = buf[j];
				buf[j] = buf[j - 1];
				buf[j - 1] = temp;
			}
		}
	}
}
#endif //defined(AOD_PLATFORM_IPHONE)

// ===========================================================================
//	AoActSortDraw
/*!
	ソートバッファ内要素の描画

	@note
	ソートバッファに登録されている全ての要素の描画処理を行います。\n
	描画は、この関数呼び出し時点でのソートバッファの順番に行われるので、
	事前にAoActSortExecute関数を呼び出し、
	描画順序が適切なものになるようにして下さい。\n
	内部で描画タスクを生成し、描画スレッドにて実際の描画処理が行われます。\n
*/
// ===========================================================================
void AoActSortDraw(void)
{
	if (g_ao_act_sort_num == 0) {
		return;
	}

	if (g_ao_act_sys_draw_state_enable) {
		// ステート描画
		aoActDrawSortState();
	}
	else {
		// 必要バッファサイズ算出
		u32 spr_size = sizeof(AOS_SPRITE) * g_ao_act_sort_num;
		u32 size = spr_size + sizeof(AOS_ACT_DRAW);

		// バッファ確保
		u8* buf = (u8*)amDrawMallocDataBuffer((s32)size);

		// 描画タスクワーク作成
		AOS_ACT_DRAW* work = (AOS_ACT_DRAW*)(buf + spr_size);
		work->count = g_ao_act_sort_num;
		work->sprite = (AOS_SPRITE*)buf;
#if defined(AOD_DEBUG)
		work->show_hit = AoActDebugGetShowHitFlag();
		work->hit_color = AoActDebugGetShowHitColor();
#endif // defined(AOD_DEBUG)

		// スプライトバッファコピー
		for (u32 i = 0; i < g_ao_act_sort_num; ++i) {
			amCopyMemory(
				&work->sprite[i],
				g_ao_act_sort_buf[i].sprite,
				sizeof(AOS_SPRITE));
		}

		// 描画タスク作成
		amDrawMakeTask(aoActDrawTask, (u16)g_ao_act_sys_draw_prio, (u32)work);
	}
}


// ***************************************************************************
// アキュムレートスタック
// ***************************************************************************
// ===========================================================================
//	AoActAcmInit
/*!
	アキュムレート初期化

	@param acm				[out] 初期化するアキュムレート(NULLならカレント)
*/
// ===========================================================================
void AoActAcmInit(AOS_ACT_ACM* acm)
{
	if (acm == NULL) {
		acm = g_ao_act_acm_cur;
	}
	acm->trans_x = 0.0f;
	acm->trans_y = 0.0f;
	acm->trans_z = 0.0f;
	acm->color.r = 255;
	acm->color.g = 255;
	acm->color.b = 255;
	acm->color.a = 255;
	acm->fade.r = 0;
	acm->fade.g = 0;
	acm->fade.b = 0;
	acm->fade.a = 0;
	acm->trans_scale_x = 1.0f;
	acm->trans_scale_y = 1.0f;
	acm->scale_x = 1.0f;
	acm->scale_y = 1.0f;
	acm->rotate = 0.0f;
}

// ===========================================================================
//	AoActAcmPush
/*!
	アキュムレートプッシュ

	@param acm				[in] プッシュするアキュムレート(NULLならカレント)
	@note
	スタックを超えてプッシュした場合は、アサートし何も行いません。\n
*/
// ===========================================================================
void AoActAcmPush(const AOS_ACT_ACM* acm)
{
	// オーバーチェック
	if (g_ao_act_acm_num >= g_ao_act_acm_buf_size) {
		amAssert(0);
		return;
	}

	// プッシュ
	if (acm == NULL) {
		acm = g_ao_act_acm_cur;
	}
	g_ao_act_acm_cur += 1;
	*g_ao_act_acm_cur = *acm;

	// 使用数更新
	g_ao_act_acm_num += 1;
	if (g_ao_act_acm_num > g_ao_act_acm_peak) {
		g_ao_act_acm_peak = g_ao_act_acm_num;
	}
}

// ===========================================================================
//	AoActAcmPop
/*!
	アキュムレートポップ

	@param count			[in] ポップする回数
	@note
	指定の回数アキュムレートスタックをポップします。\n
	プッシュ回数よりも多くポップした場合は、アサートし何も行いません。\n
*/
// ===========================================================================
void AoActAcmPop(u32 count)
{
	while (count > 0) {
		// アンダーチェック
		if (g_ao_act_acm_cur == g_ao_act_acm_buf) {
			amAssert(0);
			return;
		}
		amAssert(g_ao_act_acm_num > 1);

		g_ao_act_acm_cur -= 1;
		g_ao_act_acm_num -= 1;

		count -= 1;
	}
	if (g_ao_act_acm_num > g_ao_act_acm_peak) {
		g_ao_act_acm_peak = g_ao_act_acm_num;
	}
}

// ===========================================================================
//	AoActAcmSet
/*!
	アキュムレート設定

	@param acm				[in] 設定するアキュムレート
	@note
	カレントアキュムレートに指定したアキュムレートを設定します。\n
*/
// ===========================================================================
void AoActAcmSet(const AOS_ACT_ACM* acm)
{
	*g_ao_act_acm_cur = *acm;
}

// ===========================================================================
//	AoActAcmApply
/*!
	アキュムレート適用

	@param acm				[in] 適用するアキュムレート
	@note
	カレントアキュムレートに指定したアキュムレートを掛け合わせます。\n
*/
// ===========================================================================
void AoActAcmApply(const AOS_ACT_ACM* acm)
{
	AoActAcmApplyTrans(acm->trans_x, acm->trans_y, acm->trans_z);
	AoActAcmApplyColor(acm->color);
	AoActAcmApplyFade(acm->fade);
	AoActAcmApplyTransScale(acm->trans_scale_x, acm->trans_scale_y);
	AoActAcmApplyScale(acm->scale_x, acm->scale_y);
	AoActAcmApplyRotate(acm->rotate);
}

// ===========================================================================
//	AoActAcmApplyTrans
/*!
	アキュムレート トランス適用

	@param trs				[in] 適用するトランス
*/
// ===========================================================================
void AoActAcmApplyTrans(const AMS_VECTOR* trs)
{
	AoActAcmApplyTrans(trs->x, trs->y, trs->z);
}

// ===========================================================================
//	AoActAcmApplyTrans
/*!
	アキュムレート トランス適用

	@param x				[in] 適用するトランスX成分
	@param y				[in] 適用するトランスY成分
	@param z				[in] 適用するトランスZ成分
*/
// ===========================================================================
void AoActAcmApplyTrans(f32 x, f32 y, f32 z)
{
	g_ao_act_acm_cur->trans_x += g_ao_act_acm_cur->trans_scale_x * x;
	g_ao_act_acm_cur->trans_y += g_ao_act_acm_cur->trans_scale_y * y;
	g_ao_act_acm_cur->trans_z += z;
}

// ===========================================================================
//	AoActAcmApplyColor
/*!
	アキュムレート カラー適用

	@param col				[in] 適用するカラー
*/
// ===========================================================================
void AoActAcmApplyColor(AOS_ACT_COL col)
{
	AOS_ACT_COL& cur = g_ao_act_acm_cur->color;
	cur.r = (u8)(((u32)cur.r * (u32)col.r) >> 8);
	cur.g = (u8)(((u32)cur.g * (u32)col.g) >> 8);
	cur.b = (u8)(((u32)cur.b * (u32)col.b) >> 8);
	cur.a = (u8)(((u32)cur.a * (u32)col.a) >> 8);
}

// ===========================================================================
//	AoActAcmApplyFade
/*!
	アキュムレート フェードカラー適用

	@param fade				[in] 適用するフェードカラー
*/
// ===========================================================================
void AoActAcmApplyFade(AOS_ACT_COL fade)
{
	AOS_ACT_COL& cur = g_ao_act_acm_cur->fade;
	u32 temp;
	temp = (u32)(cur.r + fade.r);
	if (temp > 255) {
		cur.r = 255;
	}
	else {
		cur.r = (u8)temp;
	}
	temp = (u32)(cur.g + fade.g);
	if (temp > 255) {
		cur.g = 255;
	}
	else {
		cur.g = (u8)temp;
	}
	temp = (u32)(cur.b + fade.b);
	if (temp > 255) {
		cur.b = 255;
	}
	else {
		cur.b = (u8)temp;
	}
	temp = (u32)(cur.a + fade.a);
	if (temp > 255) {
		cur.a = 255;
	}
	else {
		cur.a = (u8)temp;
	}
}

// ===========================================================================
//	AoActAcmApplyTransScale
/*!
	アキュムレート トランススケール適用

	@param tscl				[in] 適用するトランススケール
*/
// ===========================================================================
void AoActAcmApplyTransScale(const AMS_VECTOR* tscl)
{
	AoActAcmApplyTransScale(tscl->x, tscl->y);
}

// ===========================================================================
//	AoActAcmApplyTransScale
/*!
	アキュムレート トランススケール適用

	@param x				[in] 適用するトランススケールX成分
	@param y				[in] 適用するトランススケールY成分
*/
// ===========================================================================
void AoActAcmApplyTransScale(f32 x, f32 y)
{
	g_ao_act_acm_cur->trans_scale_x *= x;
	g_ao_act_acm_cur->trans_scale_y *= y;
}

// ===========================================================================
//	AoActAcmApplyScale
/*!
	アキュムレート スケール適用

	@param scl				[in] 適用するスケール
*/
// ===========================================================================
void AoActAcmApplyScale(const AMS_VECTOR* scl)
{
	AoActAcmApplyScale(scl->x, scl->y);
}

// ===========================================================================
//	AoActAcmApplyScale
/*!
	アキュムレート スケール適用

	@param x				[in] 適用するスケールX成分
	@param y				[in] 適用するスケールY成分
*/
// ===========================================================================
void AoActAcmApplyScale(f32 x, f32 y)
{
	g_ao_act_acm_cur->scale_x *= x;
	g_ao_act_acm_cur->scale_y *= y;
}

// ===========================================================================
//	AoActAcmApplyRotate
/*!
	アキュムレート 回転適用

	@param rot				[in] 適用する回転
*/
// ===========================================================================
void AoActAcmApplyRotate(f32 rot)
{
	g_ao_act_acm_cur->rotate += rot;
}

// ===========================================================================
//	AoActAcmSetFlag
/*!
	アキュムレートフラグ設定

	@param flag				[in] 設定するアキュムレートフラグ
	@note
	指定のアキュムレートフラグをカレントに設定します。\n
*/
// ===========================================================================
void AoActAcmSetFlag(u32 flag)
{
	*g_ao_act_acm_flag_cur = flag;
}

// ===========================================================================
//	AoActAcmGetFlag
/*!
	アキュムレートフラグ取得

	@param flag				[in] カレントアキュムレートフラグ
*/
// ===========================================================================
u32 AoActAcmGetFlag(void)
{
	return *g_ao_act_acm_flag_cur;
}

// ===========================================================================
//	AoActAcmFlagPush
/*!
	アキュムレートフラグプッシュ

	@param on				[in] 有効にするアキュムレートフラグ
	@param off				[in] 無効にするアキュムレートフラグ
	@note
	カレントフラグをプッシュした後に、
	カレントに対して指定のフラグ操作を行います。\n
	スタックを超えてプッシュした場合は、アサートし何も行いません。\n
*/
// ===========================================================================
void AoActAcmFlagPush(u32 on, u32 off)
{
	// オーバーチェック
	if (g_ao_act_acm_flag_num >= g_ao_act_acm_flag_buf_size) {
		amAssert(0);
		return;
	}

	// プッシュ
	u32 prev = *g_ao_act_acm_flag_cur;
	g_ao_act_acm_flag_cur += 1;
	prev |= on;
	prev &= ~off;
	*g_ao_act_acm_flag_cur = prev;

	// 使用数更新
	g_ao_act_acm_flag_num += 1;
	if (g_ao_act_acm_flag_num > g_ao_act_acm_flag_peak) {
		g_ao_act_acm_flag_peak = g_ao_act_acm_flag_num;
	}
}

// ===========================================================================
//	AoActAcmFlagPop
/*!
	アキュムレートフラグポップ

	@param count			[in] ポップする回数
	@note
	指定の回数アキュムレートフラグスタックをポップします。\n
	プッシュ回数を超えてポップした場合は、アサートし何も行いません。\n
*/
// ===========================================================================
void AoActAcmFlagPop(u32 count)
{
	while (count > 0) {
		// アンダーチェック
		if (g_ao_act_acm_flag_cur == g_ao_act_acm_flag_buf) {
			amAssert(0);
			return;
		}
		amAssert(g_ao_act_acm_flag_num > 1);

		g_ao_act_acm_flag_cur -= 1;
		g_ao_act_acm_flag_num -= 1;

		count -= 1;
	}
	if (g_ao_act_acm_flag_num > g_ao_act_acm_flag_peak) {
		g_ao_act_acm_flag_peak = g_ao_act_acm_flag_num;
	}
}


// ***************************************************************************
// 当たり
// ***************************************************************************
// ===========================================================================
//	AoActGetHitSpr
/*!
	スプライトの当たり情報取得

	@param hit				[out] 当たり情報
	@param spr				[in]  スプライト構造体
	@return 真：有効な当たり情報あり　偽：なし
	@note
	指定のスプライトに含まれる当たり情報に、
	移動や拡大を適用させた最終結果をhitに格納ます。\n
	当たり情報が無い場合と、無効な当たり情報が設定されている場合は
	FALSEを返します。\n
*/
// ===========================================================================
BOOL AoActGetHitSpr(AOS_ACT_HIT* hit, const AOS_SPRITE* spr)
{
	if ((u32)spr->hit.type >= AOD_ACT_TYPE_NUM) {
		hit->type = AOD_ACT_HIT_NONE;
		return FALSE;
	}

	hit->type = spr->hit.type;
	hit->center_x = spr->center_x;
	hit->center_y = spr->center_y;
	hit->scale_x = spr->hit.scale_x;
	hit->scale_y = spr->hit.scale_y;
	if (spr->hit.flag & A2D_AMA_HIT_FLAG_ROT_DISABLE) {
		hit->rotate = 0.0f;
	}
	else {
		hit->rotate = spr->rotate;
	}
	switch (hit->type) {
	case AOD_ACT_HIT_RECT:
		hit->rect.left = spr->hit.rect.left;
		hit->rect.top = spr->hit.rect.top;
		hit->rect.right = spr->hit.rect.right;
		hit->rect.bottom = spr->hit.rect.bottom;
		break;

	case AOD_ACT_HIT_CIRCLE:
		hit->circle.center_x = spr->hit.circle.center_x;
		hit->circle.center_y = spr->hit.circle.center_y;
		hit->circle.radius = spr->hit.circle.radius;
		break;

	default:
		amAssert(0);
		hit->type = AOD_ACT_HIT_NONE;
		return FALSE;
		break;
	}

	return TRUE;
}

// ===========================================================================
//	AoActGetHitAct
/*!
	アクションの当たり情報取得

	@param hit				[out] 当たり情報
	@param act				[in]  アクション構造体
	@return 真：有効な当たり情報あり　偽：なし
	@note
	指定のアクションに含まれる当たり情報に、
	移動や拡大を適用させた最終結果をhitに格納ます。\n
	当たり情報が無い場合と、無効な当たり情報が設定されている場合は
	FALSEを返します。\n
	アクションの親子関係は探索しません。\n
*/
// ===========================================================================
BOOL AoActGetHitAct(AOS_ACT_HIT* hit, const AOS_ACTION* act)
{
	return AoActGetHitSpr(hit, AoActUtilGetSprFromAct(act));
}

// ===========================================================================
//	AoActGetHitNum
/*!
	アクション(親子階層)の当たり情報数取得

	@param act				[in]  アクション構造体
	@return 真：当たり情報数
	@note
	指定アクションの親子階層をたどり、
	有効な当たり情報がいくつあるかを算出します。\n
	この関数の戻り値が、
	AoActUtilGetActNum関数の戻り値より多くなることはありません。\n
*/
// ===========================================================================
u32 AoActGetHitNum(const AOS_ACTION* act)
{
	// 当たり数
	u32 hit_num = 0;

	// 自身の当たりが有効か判定
	if ((u32)AoActUtilGetSprFromAct(act)->hit.type < AOD_ACT_TYPE_NUM) {
		hit_num += 1;
	}

	// 子をチェック
	if (act->child) {
		hit_num += AoActGetHitNum(act->child);
	}

	// 弟をチェック
	if (act->sibling) {
		hit_num += AoActGetHitNum(act->sibling);
	}

	// 結果を返す
	return hit_num;
}

// ===========================================================================
//	AoActGetHitTbl
/*!
	アクション(親子階層)の当たり情報配列取得

	@param hit_tbl			[out] 当たり情報配列
	@param act				[in]  アクション構造体
	@return 真：当たり情報数
	@note
	指定アクションの親子階層をたどり、
	有効な当たり情報をhit_tblに格納し、
	hit_tblに格納した当たり情報数を返します。\n
	hit_tblの要素数は、AoActGetHitNum以上を用意するようにしてください。\n
*/
// ===========================================================================
u32 AoActGetHitTbl(AOS_ACT_HIT* hit_tbl, const AOS_ACTION* act)
{
	return AoActGetHitTbl(hit_tbl, 0xffffffff, act);
}

// ===========================================================================
//	AoActGetHitTbl
/*!
	アクション(親子階層)の当たり情報配列取得

	@param hit_tbl			[out] 当たり情報配列
	@param size				[in]  当たり情報配列の要素数
	@param act				[in]  アクション構造体
	@return 真：当たり情報数
	@note
	指定アクションの親子階層をたどり、
	有効な当たり情報をhit_tblに格納し、
	hit_tblに格納した当たり情報数を返します。\n
	当たり情報数がsizeよりも多かった場合は、
	sizeまでを格納した時点で処理を終了します。\n
*/
// ===========================================================================
u32 AoActGetHitTbl(AOS_ACT_HIT* hit_tbl, u32 size, const AOS_ACTION* act)
{
	// 当たり数
	u32 hit_num = 0;

	// 自身の当たりが有効か判定
	if ((u32)AoActUtilGetSprFromAct(act)->hit.type < AOD_ACT_TYPE_NUM) {

		// 当たり情報を格納可能かチェック
		if (size < 1) {
			return hit_num;
		}

		// 自身の当たり情報取得
		AoActGetHitSpr(&hit_tbl[hit_num], AoActUtilGetSprFromAct(act));
		hit_num += 1;
		size -= 1;
	}

	// 子をチェック
	if (act->child) {
		u32 temp = AoActGetHitTbl(&hit_tbl[hit_num], size, act->child);
		hit_num += temp;
		if (size > temp) {
			size -= temp;
		}
		else {
			return hit_num;
		}
	}

	// 弟をチェック
	if (act->sibling) {
		u32 temp = AoActGetHitTbl(&hit_tbl[hit_num], size, act->sibling);
		hit_num += temp;
		if (size > temp) {
			size -= temp;
		}
		else {
			return hit_num;
		}
	}

	// 結果を返す
	return hit_num;
}

// ===========================================================================
//	AoActGetHitActId
/*!
	アクションの当たり情報取得

	@param hit				[out] 当たり情報
	@param act				[in]  アクション構造体
	@param id				[in]  AMAアクションID
	@return 真：有効な当たり情報あり　偽：なし
	@note
	指定のアクションの親子関係を探査し、
	指定のAMAアクションIDのアクションが有る場合は、
	そのアクションの当たり情報をhitに格納ます。\n
	当たり情報が無い場合と、無効な当たり情報が設定されている場合は
	FALSEを返します。\n
*/
// ===========================================================================
BOOL AoActGetHitActId(AOS_ACT_HIT* hit, const AOS_ACTION* act, u32 id)
{
	const AOS_ACTION* search = AoActUtilGetActFromId(act, id);
	if (search == NULL) {
		hit->type = AOD_ACT_HIT_NONE;
		return FALSE;
	}
	return AoActGetHitAct(hit, search);
}

// ===========================================================================
//	AoActHitTest
/*!
	hittest

	@param hit				[in] 当たり情報
	@param x				[in] 当たり判定X位置
	@param y				[in] 当たり判定Y位置
	@return 真：当たりあり　偽：なし
	@note
	指定の当たり情報に指定の判定位置が当たっているかどうか
	(当たり情報の内側かどうか)
	を判定します。\n
	座標補正を行なわずに判定する点に注意して下さい。\n
*/
// ===========================================================================
BOOL AoActHitTest(const AOS_ACT_HIT* hit, f32 x, f32 y)
{
	// 無効当たりなら処理しない
	if ((u32)hit->type >= AOD_ACT_HIT_NUM) {
		return FALSE;
	}

	// 判定位置を補正
	x -= hit->center_x;
	y -= hit->center_y;
	f32 sn, cs, tx, ty;
	amSinCos(NNM_DEGtoA32(-hit->rotate), &sn, &cs);
	tx = x * cs - y * sn;
	ty = x * sn + y * cs;
	x = tx / hit->scale_x;
	y = ty / hit->scale_y;

	// タイプ別に判定
	switch (hit->type) {
	case AOD_ACT_HIT_RECT:
		if ((x >= hit->rect.left) && (x <= hit->rect.right) &&
			(y >= hit->rect.top) && (y <= hit->rect.bottom))
		{
			return TRUE;
		}
		break;

	case AOD_ACT_HIT_CIRCLE:
		tx = x - hit->circle.center_x;
		ty = y - hit->circle.center_y;
		if (((tx * tx) + (ty * ty)) <=
			(hit->circle.radius * hit->circle.radius))
		{
			return TRUE;
		}
		break;

	default:
		break;
	}

	return FALSE;
}

// ===========================================================================
//	AoActHitTestCorReverse
/*!
	hittest(座標補正あり) ※座標補正はiPhone版のみ対応

	@param hit				[in] 当たり情報
	@param x				[in] 当たり判定X位置
	@param y				[in] 当たり判定Y位置
	@return 真：当たりあり　偽：なし
	@note
	AoActHitTest関数の座標補正あり版です。\n
	画面座標として判定位置を指定することで、
	内部で現在の画面モードを判定し、適切な当たり判定を行います。\n
*/
// ===========================================================================
BOOL AoActHitTestCorReverse(const AOS_ACT_HIT* hit, f32 x, f32 y)
{
	// 座標補正
	AoActCorReverse(&x, &y);

	// 判定
	return AoActHitTest(hit, x, y);
}


// ***************************************************************************
// ユーティリティ
// ***************************************************************************
// ===========================================================================
//	AoActUtilGetActNum
/*!
	アクション数取得

	@param act				[in] アクション構造体
	@return アクション数
	@note
	指定のアクションの親子階層を探査し、
	いくつのアクションで構成されているか調べます。\n
	同じアクションの場合は、この関数の戻り値が変わることはありません。\n
*/
// ===========================================================================
u32 AoActUtilGetActNum(const AOS_ACTION* act)
{
	if (act == NULL) {
		return 0;
	}
	u32 count = 1;
	count += AoActUtilGetActNum(act->child);
	count += AoActUtilGetActNum(act->sibling);
	return count;
}

// ===========================================================================
//	AoActUtilGetActFromId
/*!
	AMAアクションIDよりアクション取得

	@param act				[in] アクション構造体
	@param id				[in] AMAアクションID
	@return アクション(NULL:該当なし)
	@note
	指定のアクションの親子階層を探査し、
	指定のAMAアクションIDを持つアクションを返します。\n
	該当するアクションが無い場合はNULLを返します。\n
	戻り値のアクションは参照のみ可能です。(変更不可)\n
*/
// ===========================================================================
const AOS_ACTION* AoActUtilGetActFromId(const AOS_ACTION* act, u32 id)
{
	const AOS_ACTION* ret = NULL;

	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);
	if (ama_act->id == id) {
		ret = act;
	}
	else {
		// 子を探索
		if (act->child) {
			ret = AoActUtilGetActFromId(act->child, id);
		}

		// 弟を探索
		if ((ret == NULL) && (act->sibling)) {
			ret = AoActUtilGetActFromId(act->sibling, id);
		}
	}

	return ret;
}

// ===========================================================================
//	AoActUtilGetSprFromAct
/*!
	アクションよりスプライト取得

	@param act				[in] アクション構造体
	@return スプライト
	@note
	指定のアクションに含まれるスプライトを返します。\n
	戻り値のスプライトは参照のみ可能です。(変更不可)\n
*/
// ===========================================================================
const AOS_SPRITE* AoActUtilGetSprFromAct(const AOS_ACTION* act)
{
	return act->sprite;
}

// ===========================================================================
//	AoActDrawCorWide
/*!
	頂点データのワイド補正

	@param v		[io] 頂点配列
	@param v_num	[in] 頂点数
	@param type		[in] 補正タイプ
*/
// ===========================================================================
void AoActDrawCorWide(NNS_PRIM3D_P* v, u32 v_num, AOE_ACT_CORW type)
{
#if defined(AOD_PLATFORM_IPHONE)
	if (TRUE) {
#else
	if (_am_draw_video.wide_screen) {
#endif // AOD_PLATFORM_IPHONE
		switch (type) {
		case AOD_ACT_CORW_NONE:
			{
				const f32 temp = g_ao_act_wide_w / g_ao_act_base_w;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x *= temp;
				}
			}
			break;
		case AOD_ACT_CORW_CENTER:
			{
				const f32 temp = (g_ao_act_wide_w - g_ao_act_base_w) * 0.5f;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += temp;
				}
			}
			break;
		case AOD_ACT_CORW_LEFT:
			// empty
			break;
		case AOD_ACT_CORW_RIGHT:
			{
				f32 temp = g_ao_act_wide_w - g_ao_act_base_w;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += temp;
				}
			}
			break;
		case AOD_ACT_CORW_LEFT_S:
			{
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += g_ao_act_wide_shift;
				}
			}
			break;
		case AOD_ACT_CORW_RIGHT_S:
			{
				f32 temp = g_ao_act_wide_w - g_ao_act_base_w;
				temp -= g_ao_act_wide_shift;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += temp;
				}
			}
			break;
		default:
			break;
		}
	}
	else {
#if defined(AOD_PLATFORM_WII)
		// Wii
		// empty
#elif defined(AOD_PLATFORM_IPHONE)
		// iPhone
		// empty
#else
		// other
		{
			const f32 temp = (g_ao_act_wide_w - g_ao_act_base_w) * 0.5f;
			for (u32 i = 0; i < v_num; ++i) {
				v[i].Pos.x += temp;
			}
		}
#endif
	}
}

// ===========================================================================
//	AoActDrawCorWide
/*!
	頂点データのワイド補正

	@param v		[io] 頂点配列
	@param v_num	[in] 頂点数
	@param type		[in] 補正タイプ
*/
// ===========================================================================
void AoActDrawCorWide(NNS_PRIM3D_PC* v, u32 v_num, AOE_ACT_CORW type)
{
#if defined(AOD_PLATFORM_IPHONE)
	if (TRUE) {
#else
	if (_am_draw_video.wide_screen) {
#endif // AOD_PLATFORM_IPHONE
		switch (type) {
		case AOD_ACT_CORW_NONE:
			{
				const f32 temp = g_ao_act_wide_w / g_ao_act_base_w;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x *= temp;
				}
			}
			break;
		case AOD_ACT_CORW_CENTER:
			{
				const f32 temp = (g_ao_act_wide_w - g_ao_act_base_w) * 0.5f;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += temp;
				}
			}
			break;
		case AOD_ACT_CORW_LEFT:
			// empty
			break;
		case AOD_ACT_CORW_RIGHT:
			{
				f32 temp = g_ao_act_wide_w - g_ao_act_base_w;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += temp;
				}
			}
			break;
		case AOD_ACT_CORW_LEFT_S:
			{
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += g_ao_act_wide_shift;
				}
			}
			break;
		case AOD_ACT_CORW_RIGHT_S:
			{
				f32 temp = g_ao_act_wide_w - g_ao_act_base_w;
				temp -= g_ao_act_wide_shift;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += temp;
				}
			}
			break;
		default:
			break;
		}
	}
	else {
#if defined(AOD_PLATFORM_WII)
		// Wii
		// empty
#elif defined(AOD_PLATFORM_IPHONE)
		// iPhone
		// empty
#else
		// other
		{
			const f32 temp = (g_ao_act_wide_w - g_ao_act_base_w) * 0.5f;
			for (u32 i = 0; i < v_num; ++i) {
				v[i].Pos.x += temp;
			}
		}
#endif
	}
}

// ===========================================================================
//	AoActDrawCorWide
/*!
	頂点データのワイド補正

	@param v		[io] 頂点配列
	@param v_num	[in] 頂点数
	@param type		[in] 補正タイプ
*/
// ===========================================================================
void AoActDrawCorWide(NNS_PRIM3D_PCT* v, u32 v_num, AOE_ACT_CORW type)
{
#if defined(AOD_PLATFORM_IPHONE)
	if (TRUE) {
#else
	if (_am_draw_video.wide_screen) {
#endif // AOD_PLATFORM_IPHONE
		switch (type) {
		case AOD_ACT_CORW_NONE:
			{
				const f32 temp = g_ao_act_wide_w / g_ao_act_base_w;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x *= temp;
				}
			}
			break;
		case AOD_ACT_CORW_CENTER:
			{
				const f32 temp = (g_ao_act_wide_w - g_ao_act_base_w) * 0.5f;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += temp;
				}
			}
			break;
		case AOD_ACT_CORW_LEFT:
			// empty
			break;
		case AOD_ACT_CORW_RIGHT:
			{
				f32 temp = g_ao_act_wide_w - g_ao_act_base_w;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += temp;
				}
			}
			break;
		case AOD_ACT_CORW_LEFT_S:
			{
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += g_ao_act_wide_shift;
				}
			}
			break;
		case AOD_ACT_CORW_RIGHT_S:
			{
				f32 temp = g_ao_act_wide_w - g_ao_act_base_w;
				temp -= g_ao_act_wide_shift;
				for (u32 i = 0; i < v_num; ++i) {
					v[i].Pos.x += temp;
				}
			}
			break;
		default:
			break;
		}
	}
	else {
#if defined(AOD_PLATFORM_WII)
		// Wii
		// empty
#elif defined(AOD_PLATFORM_IPHONE)
		// iPhone
		// empty
#else
		// other
		{
			const f32 temp = (g_ao_act_wide_w - g_ao_act_base_w) * 0.5f;
			for (u32 i = 0; i < v_num; ++i) {
				v[i].Pos.x += temp;
			}
		}
#endif
	}
}

// ===========================================================================
//	AoActDrawCorWide
/*!
	座標データのワイド補正

	@param x		[io] X座標
	@param y		[io] Y座標
	@param type		[in] 補正タイプ
*/
// ===========================================================================
void AoActDrawCorWide(f32* x, f32* y, AOE_ACT_CORW type)
{
	UNREFERENCED_PARAMETER(y);

	f32 rx;
	if (x) {
		rx = *x;
	}
	else {
		rx = 0.0f;
	}

#if defined(AOD_PLATFORM_IPHONE)
	if (TRUE) {
#else
	if (_am_draw_video.wide_screen) {
#endif // AOD_PLATFORM_IPHONE
		switch (type) {
		case AOD_ACT_CORW_NONE:
			rx *= g_ao_act_wide_w / g_ao_act_base_w;
			break;
		case AOD_ACT_CORW_CENTER:
			rx += (g_ao_act_wide_w - g_ao_act_base_w) * 0.5f;
			break;
		case AOD_ACT_CORW_LEFT:
			// empty
			break;
		case AOD_ACT_CORW_RIGHT:
			rx += g_ao_act_wide_w - g_ao_act_base_w;
			break;
		case AOD_ACT_CORW_LEFT_S:
			rx += g_ao_act_wide_shift;
			break;
		case AOD_ACT_CORW_RIGHT_S:
			rx += (g_ao_act_wide_w - g_ao_act_base_w) - g_ao_act_wide_shift;
			break;
		default:
			break;
		}
	}
	else {
#if defined(AOD_PLATFORM_WII)
		// Wii
		// empty
#elif defined(AOD_PLATFORM_IPHONE)
		// iPhone
		// empty
#else
		// other
		rx += (g_ao_act_wide_w - g_ao_act_base_w) * 0.5f;
#endif
	}

	if (x) {
		*x = rx;
	}
}

// ===========================================================================
//	AoActCorWideR
/*!
	座標の逆補正 ※iPhone版のみ対応

	@param x	[io] 補正X座標
	@param y	[io] 補正Y座標
	@note
	画面座標を指定することで、内部で適切な補正を行い、
	aoAction内で使用している座標系に変換します。\n
	aoAction内では
	横AOD_ACT_SCREEN_WIDTHx縦AOD_ACT_SCREEN_HEIGHT
	の解像度でデータを扱っており、
	画面解像度とは必ずしも一致しないので、
	必要に応じて、この関数で座標変換を行なうようにして下さい。\n
*/
// ===========================================================================
void AoActCorReverse(f32* x, f32* y)
{
#if defined(AOD_PLATFORM_IPHONE)

	f32 rx;
	f32 ry;
	if (x) {
		rx = *x;
	}
	else {
		rx = 0.0f;
	}
	if (y) {
		ry = *y;
	}
	else {
		ry = 0.0f;
	}

	// 画面解像度からaoAction解像度に変換
	rx *= g_ao_act_wide_w / 480.0f;
	ry *= g_ao_act_base_h / 320.0f;

	// 3:2 -> 4:3に補正
	rx *= g_ao_act_base_w / g_ao_act_wide_w;

	if (x) {
		*x = rx;
	}
	if (y) {
		*y = ry;
	}

#else

	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);

#endif // defined(AOD_PLATFORM_IPHONE)
}

// ===========================================================================
//	AoActUtilGetChild
/*!
	指定アクションの子アクション取得

	@param act		[in] アクション
	@return 指定アクションの子アクション
*/
// ===========================================================================
const AOS_ACTION* AoActUtilGetChild(const AOS_ACTION* act)
{
	amAssert(act);
	return act->child;
}

// ===========================================================================
//	AoActUtilGetSibling
/*!
	指定アクションの弟アクション取得

	@param act		[in] アクション
	@return 指定アクションの弟アクション
*/
// ===========================================================================
const AOS_ACTION* AoActUtilGetSibling(const AOS_ACTION* act)
{
	amAssert(act);
	return act->sibling;
}


// ***************************************************************************
// ファイル
// ***************************************************************************
// ===========================================================================
//	AoActIsAma
/*!
	AMAファイル判定

	@param file				[in] 判定するファイル
	@return 真：AMAファイル　偽：それ以外
	@note
	引数で指定したファイルがAMAファイルであるか判定します。\n
	ヘッダ情報のみを判定するので、
	全てのデータが正常であるかの判定は行いません。\n
	AoActAmaConv関数を呼び出した後のAMAファイルであっても、
	AMAファイルと判定されます。\n
*/
// ===========================================================================
BOOL AoActIsAma(const void* file)
{
	// ヘッダ(識別子)のみチェック
	const char* masic = (const char*)file;
	const char* base = A2D_AMA_MASIC;
	if ((masic[1] == base[1]) &&
		(masic[2] == base[2]) &&
		(masic[3] == base[3]))
	{
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoActAmaConv
/*!
	アドレス変換

	@param file				[io] アドレス変換するAMAファイル
	@return 1:正常に変換 0:既に変換済みorそれ以外
	@note
	引数で指定したAMAファイルのアドレス変換を行います。\n
	正常に変換が完了した場合は1を返します。\n
	既に変換済みの場合は何も行わずに0を返します。\n
	AMAファイル以外のファイルを指定した場合は、
	アサートし何も行わず0を返します。\n
*/
// ===========================================================================
s32 AoActAmaConv(u8* file)
{
	if (!AoActIsAma(file)) {
		amAssert(0);
		return 0;
	}

	// ヘッダ取得
	A2S_AMA_HEADER* head = (A2S_AMA_HEADER*)file;

	// 変換済み判定
	if (head->masic[0] == AMD_CONVERTED_MARK) {
		return 0;
	}

	// 変換済み設定
	head->masic[0] = AMD_CONVERTED_MARK;

	// ヘッダ変換
	amConvert(file, A2S_AMA_NODE**, head->node_tbl);
	amConvert(file, A2S_AMA_ACT**, head->act_tbl);
	amConvert(file, char**, head->node_name_tbl);
	amConvert(file, char**, head->act_name_tbl);

	// ノードポインタ配列変換
	for (u32 i = 0; i < head->node_num; ++i) {
		amConvert(file, A2S_AMA_NODE*, head->node_tbl[i]);
	}

	// アクションポインタ配列変換
	for (u32 i = 0; i < head->act_num; ++i) {
		amConvert(file, A2S_AMA_ACT*, head->act_tbl[i]);
	}

	// ノード名ポインタ配列変換
	if (head->node_name_tbl) {
		for (u32 i = 0; i < head->node_num; ++i) {
			amConvert(file, char*, head->node_name_tbl[i]);
		}
	}

	// アクション名ポインタ配列変換
	if (head->act_name_tbl) {
		for (u32 i = 0; i < head->act_num; ++i) {
			amConvert(file, char*, head->act_name_tbl[i]);
		}
	}

	// ノード変換
	for (u32 i = 0; i < head->node_num; ++i) {
		amConvert(file, A2S_AMA_NODE*, head->node_tbl[i]->child);
		amConvert(file, A2S_AMA_NODE*, head->node_tbl[i]->sibling);
		amConvert(file, A2S_AMA_NODE*, head->node_tbl[i]->parent);
		amConvert(file, A2S_AMA_ACT*, head->node_tbl[i]->act);
	}

	// アクション変換
	// AMAのフォーマットとしては
	// 複数のアクションで同じデータを参照しているケースもありえるが
	// 現状のAceConvでは重複はないので簡易的に総当りで変換する
	// 今後は対応が必要になるかも
	for (u32 i = 0; i < head->act_num; ++i) {
		A2S_AMA_ACT* act = head->act_tbl[i];

		amConvert(file, A2S_AMA_MTN*, act->mtn);
		amConvert(file, A2S_AMA_ANM*, act->anm);
		amConvert(file, A2S_AMA_ACM*, act->acm);
		amConvert(file, A2S_AMA_USR*, act->usr);
		amConvert(file, A2S_AMA_HIT*, act->hit);
		amConvert(file, A2S_AMA_ACT*, act->next);

		if (act->mtn) {
			amConvert(file, A2S_SUB_KEY*, act->mtn->mtn_key_tbl);
			amConvert(file, A2S_SUB_MTN*, act->mtn->mtn_tbl);
			amConvert(file, A2S_SUB_KEY*, act->mtn->trs_key_tbl);
			amConvert(file, A2S_SUB_TRS*, act->mtn->trs_tbl);
		}

		if (act->anm) {
			amConvert(file, A2S_SUB_KEY*, act->anm->anm_key_tbl);
			amConvert(file, A2S_SUB_ANM*, act->anm->anm_tbl);
			amConvert(file, A2S_SUB_KEY*, act->anm->mat_key_tbl);
			amConvert(file, A2S_SUB_MAT*, act->anm->mat_tbl);
		}

		if (act->acm) {
			amConvert(file, A2S_SUB_KEY*, act->acm->acm_key_tbl);
			amConvert(file, A2S_SUB_ACM*, act->acm->acm_tbl);
			amConvert(file, A2S_SUB_KEY*, act->acm->trs_key_tbl);
			amConvert(file, A2S_SUB_TRS*, act->acm->trs_tbl);
			amConvert(file, A2S_SUB_KEY*, act->acm->mat_key_tbl);
			amConvert(file, A2S_SUB_MAT*, act->acm->mat_tbl);
		}

		if (act->usr) {
			amConvert(file, A2S_SUB_KEY*, act->usr->usr_key_tbl);
			amConvert(file, A2S_SUB_USR*, act->usr->usr_tbl);
		}

		if (act->hit) {
			amConvert(file, A2S_SUB_KEY*, act->hit->hit_key_tbl);
			amConvert(file, A2S_SUB_HIT*, act->hit->hit_tbl);
		}
	}

	return 1;
}

// ===========================================================================
//	AoActRegAliceAmaConv
/*!
	AliceへAMAアドレス変換登録

	@note
	amConvertAddress関数でAMAファイルのアドレス変換を行えるように、
	AliceにAMAを登録します。\n
*/
// ===========================================================================
void AoActRegAliceAmaConv(void)
{
	amConvertRegist("AMA", AoActAmaConv);
}


// ***************************************************************************
// 描画スレッド用
// ***************************************************************************
// ===========================================================================
//	AoActDrawPre
/*!
	ステート描画時の描画前処理

	@note
	ステート描画を使用してアクションの描画を行う際には、
	描画コマンドを実行する前にこの関数を呼び出して下さい。\n
	内部で射影やカメラの設定を行っています。\n
*/
// ===========================================================================
void AoActDrawPre(void)
{
	// 射影設定
	{
		NNS_MATRIX44 mtx;

#if defined(AOD_PLATFORM_WII)
		if (_am_draw_video.wide_screen) {
			nnMakeOrthoMatrix(
				&mtx,
				0.0f, g_ao_act_wide_w, g_ao_act_wide_h, 0.0f, 1.0f, 3.0f);
		}
		else {
			nnMakeOrthoMatrix(
				&mtx,
				0.0f, g_ao_act_base_w, g_ao_act_base_h, 0.0f, 1.0f, 3.0f);
		}
#elif defined(AOD_PLATFORM_IPHONE)
		nnMakeOrthoMatrix(
			&mtx, 0.0f, g_ao_act_wide_h, g_ao_act_wide_w, 0.0f, 1.0f, 3.0f);
#else
		nnMakeOrthoMatrix(
			&mtx, 0.0f, g_ao_act_wide_w, g_ao_act_wide_h, 0.0f, 1.0f, 3.0f);
#endif
		amDrawSetProjection(&mtx, NNE_PROJECTION_TYPE_ORTHO);
	}

	// 行列設定
	{
		NNS_MATRIX mtx;
		nnMakeUnitMatrix(&mtx);
#if defined(AOD_PLATFORM_IPHONE)
		// 90回転 & 描画基準位置移動
		nnRotateZMatrix(&mtx, &mtx, NNM_DEGtoA32(90.0f));
		nnTranslateMatrix(&mtx, &mtx, 0.0f, -g_ao_act_wide_h, 0.0f);
#endif // AOD_PLATFORM_IPHONE
		amDrawSetWorldViewMatrix(&mtx);
		nnSetPrimitive3DMatrix(&mtx);
	}
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// スプライト
// ***************************************************************************
// ===========================================================================
//! スプライトにアキュムレート反映
// ===========================================================================
void aoActAcmSprite(AOS_SPRITE* spr)
{
	// アキュムレート取得
	AOS_ACT_ACM* acm = g_ao_act_acm_cur;
	u32 acm_flag = *g_ao_act_acm_flag_cur;

	if (!(acm_flag & AOD_ACT_ACM_FLAG_IGNORE_ACM)) {

		// スケール適用
		spr->offset.left *= acm->scale_x;
		spr->offset.right *= acm->scale_x;
		spr->offset.top *= acm->scale_y;
		spr->offset.bottom *= acm->scale_y;

		// 当たりにスケール適用
		if (!(spr->hit.flag & A2D_AMA_HIT_FLAG_SCL_DISABLE)) {
			spr->hit.scale_x *= acm->scale_x;
			spr->hit.scale_y *= acm->scale_y;
		}

		// 回転適用
		if (acm->rotate != 0.0f) {
			f32 sn, cs, x, y;
			amSinCos(NNM_DEGtoA32(acm->rotate), &sn, &cs);
			x = spr->center_x * cs - spr->center_y * sn;
			y = spr->center_x * sn + spr->center_y * cs;
			spr->center_x = x;
			spr->center_y = y;
			spr->rotate += acm->rotate;
		}

		// トランススケール適用
		spr->center_x *= acm->trans_scale_x;
		spr->center_y *= acm->trans_scale_y;
	}

	if (!(acm_flag & AOD_ACT_ACM_FLAG_IGNORE_TRS)) {

		// トランス適用
		spr->center_x += acm->trans_x;
		spr->center_y += acm->trans_y;
		spr->prio += acm->trans_z;
	}

	if (!(acm_flag & AOD_ACT_ACM_FLAG_IGNORE_MAT)) {

		// カラー適用
		spr->color.r = (u8)(((u32)spr->color.r * (u32)acm->color.r) >> 8);
		spr->color.g = (u8)(((u32)spr->color.g * (u32)acm->color.g) >> 8);
		spr->color.b = (u8)(((u32)spr->color.b * (u32)acm->color.b) >> 8);
		spr->color.a = (u8)(((u32)spr->color.a * (u32)acm->color.a) >> 8);

		// フェードカラー適用
		u32 temp;
		temp = (u32)(spr->fade.r + acm->fade.r);
		if (temp > 255) {
			spr->fade.r = 255;
		}
		else {
			spr->fade.r = (u8)temp;
		}
		temp = (u32)(spr->fade.g + acm->fade.g);
		if (temp > 255) {
			spr->fade.g = 255;
		}
		else {
			spr->fade.g = (u8)temp;
		}
		temp = (u32)(spr->fade.b + acm->fade.b);
		if (temp > 255) {
			spr->fade.b = 255;
		}
		else {
			spr->fade.b = (u8)temp;
		}
		temp = (u32)(spr->fade.a + acm->fade.a);
		if (temp > 255) {
			spr->fade.a = 255;
		}
		else {
			spr->fade.a = (u8)temp;
		}
	}
}


// ***************************************************************************
// アクション
// ***************************************************************************
// ===========================================================================
//! アクション反映
// ===========================================================================
void aoActApply(AOS_ACTION* act)
{
	// 注意：この関数での処理はAoActSprApply関数とほぼ同じものとなります。
	// この関数を修正した場合はAoActSprApply関数も修正するようにしてください。
	// 将来的には処理を統合する予定です。

	// AMAアクション取得
	const A2S_AMA_ACT* ama_act = aoActGetAmaAct(act);

	// アキュムレート退避
	AoActAcmPush();

	// 更新
	if (ama_act && (act->flag & AOD_ACT_FLAG_UPDATE)) {

		// 再生アクション取得とフレーム算出
		f32 frame = act->frame;
		while (ama_act->next) {
			if ((f32)ama_act->frm_num > frame) {
				break;
			}
			else {
				frame -= (f32)ama_act->frm_num;
				ama_act = ama_act->next;
			}
		}

		// キーフレーム検索
		f32 trs_f = frame;
		aoActSearchTrsKey(ama_act, &trs_f, &act->last_key.trs);
		f32 mtn_f = frame;
		aoActSearchMtnKey(ama_act, &mtn_f, &act->last_key.mtn);
		f32 anm_f = frame;
		aoActSearchAnmKey(ama_act, &anm_f, &act->last_key.anm);
		f32 mat_f = frame;
		aoActSearchMatKey(ama_act, &mat_f, &act->last_key.mat);
		f32 hit_f = frame;
		aoActSearchHitKey(ama_act, &hit_f, &act->last_key.hit);

		// スプライト作成
		AOS_SPRITE* spr = act->sprite;
		spr->flag = ama_act->flag;
		spr->offset.left = ama_act->ofst.left;
		spr->offset.right = ama_act->ofst.right;
		spr->offset.top = ama_act->ofst.top;
		spr->offset.bottom = ama_act->ofst.bottom;

		f32 scale_x, scale_y;
		if (ama_act->mtn == NULL) {
			spr->center_x = 0.0f;
			spr->center_y = 0.0f;
			spr->prio = 0.0f;
			scale_x = 1.0f;
			scale_y = 1.0f;
			spr->rotate = 0.0f;
		}
		else {
			aoActMakeTrs(
				ama_act->mtn->trs_key_num, ama_act->mtn->trs_frm_num,
				ama_act->mtn->trs_key_tbl, ama_act->mtn->trs_tbl,
				act->last_key.trs, trs_f,
				&spr->center_x, &spr->center_y, &spr->prio);
			aoActMakeMtn(
				ama_act->mtn->mtn_key_num, ama_act->mtn->mtn_frm_num,
				ama_act->mtn->mtn_key_tbl, ama_act->mtn->mtn_tbl,
				act->last_key.mtn, mtn_f,
				&scale_x, &scale_y, &spr->rotate);
			spr->offset.left *= scale_x;
			spr->offset.right *= scale_x;
			spr->offset.top *= scale_y;
			spr->offset.bottom *= scale_y;
		}

		if (ama_act->anm == NULL) {
			spr->tex_id = -1;
			spr->color.r = 255;
			spr->color.g = 255;
			spr->color.b = 255;
			spr->color.a = 255;
			spr->fade.r = 0;
			spr->fade.g = 0;
			spr->fade.b = 0;
			spr->fade.a = 0;
		}
		else {
			aoActMakeAnm(
				ama_act->anm->anm_key_num, ama_act->anm->anm_frm_num,
				ama_act->anm->anm_key_tbl, ama_act->anm->anm_tbl,
				act->last_key.anm, anm_f,
				&spr->tex_id, &spr->uv, &spr->clamp);
			aoActMakeMat(
				ama_act->anm->mat_key_num, ama_act->anm->mat_frm_num,
				ama_act->anm->mat_key_tbl, ama_act->anm->mat_tbl,
				act->last_key.mat, mat_f,
				&spr->color, &spr->fade, &spr->blend);
		}

		if (ama_act->hit == NULL) {
			spr->hit.type = AOD_ACT_HIT_NONE;
		}
		else {
			aoActMakeHit(
				ama_act->hit->hit_key_num, ama_act->hit->hit_frm_num,
				ama_act->hit->hit_key_tbl, ama_act->hit->hit_tbl,
				act->last_key.hit, hit_f,
				&spr->hit);
			spr->hit.scale_x = scale_x;
			spr->hit.scale_y = scale_y;
		}

		// テクスチャはスプライト生成時にのみ適用させる
		spr->texlist = g_ao_act_texlist;

		// スプライトにアキュムレート反映
		aoActAcmSprite(spr);

		// アキュムレート処理
		if (ama_act->acm) {

			// キーフレーム検索
			f32 atrs_f = frame;
			aoActSearchAcmTrsKey(ama_act, &atrs_f, &act->last_key.atrs);
			f32 amtn_f = frame;
			aoActSearchAcmMtnKey(ama_act, &amtn_f, &act->last_key.amtn);
			f32 amat_f = frame;
			aoActSearchAcmMatKey(ama_act, &amat_f, &act->last_key.amat);

			// アキュムレート作成
			AOS_ACT_ACM acm;
			if (ama_act->acm->flag & A2D_AMA_ACM_FLAG_TRS_BODY) {
				acm.trans_x = spr->center_x;
				acm.trans_y = spr->center_y;
				acm.trans_z = spr->prio;
			}
			else {
				aoActMakeTrs(
					ama_act->acm->trs_key_num, ama_act->acm->trs_frm_num,
					ama_act->acm->trs_key_tbl, ama_act->acm->trs_tbl,
					act->last_key.atrs, atrs_f,
					&acm.trans_x, &acm.trans_y, &acm.trans_z);
			}
			if (ama_act->acm->flag & A2D_AMA_ACM_FLAG_ACM_BODY) {
				acm.trans_scale_x = 1.0f;
				acm.trans_scale_y = 1.0f;
				acm.scale_x = scale_x;
				acm.scale_y = scale_y;
				acm.rotate = spr->rotate;
			}
			else {
				aoActMakeAcm(
					ama_act->acm->acm_key_num, ama_act->acm->acm_frm_num,
					ama_act->acm->acm_key_tbl, ama_act->acm->acm_tbl,
					act->last_key.amtn, amtn_f,
					&acm.trans_scale_x, &acm.trans_scale_y,
					&acm.scale_x, &acm.scale_y, &acm.rotate);
			}
			if (ama_act->acm->flag & A2D_AMA_ACM_FLAG_MAT_BODY) {
				acm.color = spr->color;
				acm.fade = spr->fade;
			}
			else {
				u32 blend;
				aoActMakeMat(
					ama_act->acm->mat_key_num, ama_act->acm->mat_frm_num,
					ama_act->acm->mat_key_tbl, ama_act->acm->mat_tbl,
					act->last_key.amat, amat_f,
					&acm.color, &acm.fade, &blend);
			}

			// アキュムレート適用
			AoActAcmApply(&acm);
		}
	}

	// 更新フラグを落とす
	act->flag &= ~AOD_ACT_FLAG_UPDATE;

	// 子を処理
	if (act->child) {
		aoActApply(act->child);
	}

	// アキュムレート復帰
	AoActAcmPop();

	// 弟を処理
	if (act->sibling) {
		aoActApply(act->sibling);
	}
}

// ===========================================================================
//! トランスキー検索
// ===========================================================================
void aoActSearchTrsKey(const A2S_AMA_ACT* act, f32* frame, s32* key)
{
	// 無効判定
	if ((act == NULL) || (act->mtn == NULL) ||
		(act->mtn->trs_key_tbl == NULL) || (act->mtn->trs_frm_num == 0))
	{
		*frame = 0.0f;
		*key = -1;
		return;
	}

	// フレーム算出
	const A2S_AMA_MTN* dat = act->mtn;
	if (dat->flag & A2D_AMA_MTN_FLAG_TRS_LOOP) {
		*frame = aoActGetLoopFrame(*frame, dat->trs_frm_num);
	}
	else if (*frame >= (f32)dat->trs_frm_num) {
		*frame = (f32)dat->trs_frm_num;
	}

	// キー検索
	aoActSerachKey(dat->trs_key_tbl, dat->trs_key_num, frame, key);
}

// ===========================================================================
//! モーションキー検索
// ===========================================================================
void aoActSearchMtnKey(const A2S_AMA_ACT* act, f32* frame, s32* key)
{
	// 無効判定
	if ((act == NULL) || (act->mtn == NULL) ||
		(act->mtn->mtn_key_tbl == NULL) || (act->mtn->mtn_frm_num == 0))
	{
		*frame = 0.0f;
		*key = -1;
		return;
	}

	// フレーム算出
	const A2S_AMA_MTN* dat = act->mtn;
	if (dat->flag & A2D_AMA_MTN_FLAG_MTN_LOOP) {
		*frame = aoActGetLoopFrame(*frame, dat->mtn_frm_num);
	}
	else if (*frame >= (f32)dat->mtn_frm_num) {
		*frame = (f32)dat->mtn_frm_num;
	}

	// キー検索
	aoActSerachKey(dat->mtn_key_tbl, dat->mtn_key_num, frame, key);
}

// ===========================================================================
//! アニメキー検索
// ===========================================================================
void aoActSearchAnmKey(const A2S_AMA_ACT* act, f32* frame, s32* key)
{
	// 無効判定
	if ((act == NULL) || (act->anm == NULL) ||
		(act->anm->anm_key_tbl == NULL) || (act->anm->anm_frm_num == 0))
	{
		*frame = 0.0f;
		*key = -1;
		return;
	}

	// フレーム算出
	const A2S_AMA_ANM* dat = act->anm;
	if (dat->flag & A2D_AMA_ANM_FLAG_ANM_LOOP) {
		*frame = aoActGetLoopFrame(*frame, dat->anm_frm_num);
	}
	else if (*frame >= (f32)dat->anm_frm_num) {
		*frame = (f32)dat->anm_frm_num;
	}

	// キー検索
	aoActSerachKey(dat->anm_key_tbl, dat->anm_key_num, frame, key);
}

// ===========================================================================
//! マテリアルキー検索
// ===========================================================================
void aoActSearchMatKey(const A2S_AMA_ACT* act, f32* frame, s32* key)
{
	// 無効判定
	if ((act == NULL) || (act->anm == NULL) ||
		(act->anm->mat_key_tbl == NULL) || (act->anm->mat_frm_num == 0))
	{
		*frame = 0.0f;
		*key = -1;
		return;
	}

	// フレーム算出
	const A2S_AMA_ANM* dat = act->anm;
	if (dat->flag & A2D_AMA_ANM_FLAG_MAT_LOOP) {
		*frame = aoActGetLoopFrame(*frame, dat->mat_frm_num);
	}
	else if (*frame >= (f32)dat->mat_frm_num) {
		*frame = (f32)dat->mat_frm_num;
	}

	// キー検索
	aoActSerachKey(dat->mat_key_tbl, dat->mat_key_num, frame, key);
}

// ===========================================================================
//! 継承トランスキー検索
// ===========================================================================
void aoActSearchAcmTrsKey(const A2S_AMA_ACT* act, f32* frame, s32* key)
{
	// 無効判定
	if ((act == NULL) || (act->acm == NULL) ||
		(act->acm->trs_key_tbl == NULL) || (act->acm->trs_frm_num == 0) ||
		(act->acm->flag & A2D_AMA_ACM_FLAG_TRS_BODY))
	{
		*frame = 0.0f;
		*key = -1;
		return;
	}

	// フレーム算出
	const A2S_AMA_ACM* dat = act->acm;
	if (dat->flag & A2D_AMA_ACM_FLAG_TRS_LOOP) {
		*frame = aoActGetLoopFrame(*frame, dat->trs_frm_num);
	}
	else if (*frame >= (f32)dat->trs_frm_num) {
		*frame = (f32)dat->trs_frm_num;
	}

	// キー検索
	aoActSerachKey(dat->trs_key_tbl, dat->trs_key_num, frame, key);
}

// ===========================================================================
//! 継承モーションキー検索
// ===========================================================================
void aoActSearchAcmMtnKey(const A2S_AMA_ACT* act, f32* frame, s32* key)
{
	// 無効判定
	if ((act == NULL) || (act->acm == NULL) ||
		(act->acm->acm_key_tbl == NULL) || (act->acm->acm_frm_num == 0) ||
		(act->acm->flag & A2D_AMA_ACM_FLAG_ACM_BODY))
	{
		*frame = 0.0f;
		*key = -1;
		return;
	}

	// フレーム算出
	const A2S_AMA_ACM* dat = act->acm;
	if (dat->flag & A2D_AMA_ACM_FLAG_ACM_LOOP) {
		*frame = aoActGetLoopFrame(*frame, dat->acm_frm_num);
	}
	else if (*frame >= (f32)dat->acm_frm_num) {
		*frame = (f32)dat->acm_frm_num;
	}

	// キー検索
	aoActSerachKey(dat->acm_key_tbl, dat->acm_key_num, frame, key);
}

// ===========================================================================
//! 継承マテリアルキー検索
// ===========================================================================
void aoActSearchAcmMatKey(const A2S_AMA_ACT* act, f32* frame, s32* key)
{
	// 無効判定
	if ((act == NULL) || (act->acm == NULL) ||
		(act->acm->mat_key_tbl == NULL) || (act->acm->mat_frm_num == 0) ||
		(act->acm->flag & A2D_AMA_ACM_FLAG_MAT_BODY))
	{
		*frame = 0.0f;
		*key = -1;
		return;
	}

	// フレーム算出
	const A2S_AMA_ACM* dat = act->acm;
	if (dat->flag & A2D_AMA_ACM_FLAG_MAT_LOOP) {
		*frame = aoActGetLoopFrame(*frame, dat->mat_frm_num);
	}
	else if (*frame >= (f32)dat->mat_frm_num) {
		*frame = (f32)dat->mat_frm_num;
	}

	// キー検索
	aoActSerachKey(dat->mat_key_tbl, dat->mat_key_num, frame, key);
}

// ===========================================================================
//! 当たりキー検索
// ===========================================================================
void aoActSearchHitKey(const A2S_AMA_ACT* act, f32* frame, s32* key)
{
	// 無効判定
	if ((act == NULL) || (act->hit == NULL) ||
		(act->hit->hit_key_tbl == NULL) || (act->hit->hit_frm_num == 0))
	{
		*frame = 0.0f;
		*key = -1;
		return;
	}

	// フレーム算出
	const A2S_AMA_HIT* dat = act->hit;
	if (dat->flag & A2D_AMA_HIT_FLAG_HIT_LOOP) {
		*frame = aoActGetLoopFrame(*frame, dat->hit_frm_num);
	}
	else if (*frame >= (f32)dat->hit_frm_num) {
		*frame = (f32)dat->hit_frm_num;
	}

	// キー検索
	aoActSerachKey(dat->hit_key_tbl, dat->hit_key_num, frame, key);
}

// ===========================================================================
//! トランス作成
// ===========================================================================
void aoActMakeTrs(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_TRS* trs_tbl, s32 key, f32 frame,
	f32* trans_x, f32* trans_y, f32* trans_z)
{
	if ((key < 0) || (key_num == 0)) {
		*trans_x = 0.0f;
		*trans_y = 0.0f;
		*trans_z = 0.0f;
		return;
	}

	// 2つのキーと補間率を算出する
	u32 key1 = (u32)key;
	amAssert((u32)key1 < key_num);
	u32 key2;
	f32 rate;
	if (!aoActGetInterpolInfo(
		key_tbl, key_num, frm_num, frame, key1, &key2, &rate))
	{
		// 補間しない
		const A2S_SUB_TRS* trs1 = &trs_tbl[key1];
		*trans_x = trs1->trs_x;
		*trans_y = trs1->trs_y;
		*trans_z = trs1->trs_z;
		return;
	}

	// 補間する
	const A2S_SUB_TRS* trs1 = &trs_tbl[key1];
	const A2S_SUB_TRS* trs2 = &trs_tbl[key2];

	// 補間率補正
	rate = aoActGetAcceleRate(rate, trs1->trs_accele);

	if ((key_tbl[key1].interpol == A2D_AMA_INT_SPLINE) && (key_num >= 4)) {
		// スプライン補間
		s32 key0 = (s32)key1 - 1;
		if (key0 < 0) {
			key0 = (s32)key_num - 1;
		}
		s32 key3 = (s32)key2 + 1;
		if (key3 >= (s32)key_num) {
			key3 = 0;
		}
		const A2S_SUB_TRS* trs0 = &trs_tbl[key0];
		const A2S_SUB_TRS* trs3 = &trs_tbl[key3];
		aoActGetInterpolSpline(
			trs0, trs1, trs2, trs3, rate, trans_x, trans_y, trans_z);
	}
	else {
		// 線形補間
		*trans_x = aoActInterpolF32(trs1->trs_x, trs2->trs_x, rate);
		*trans_y = aoActInterpolF32(trs1->trs_y, trs2->trs_y, rate);
		*trans_z = aoActInterpolF32(trs1->trs_z, trs2->trs_z, rate);
	}
}

// ===========================================================================
//! モーション作成
// ===========================================================================
void aoActMakeMtn(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_MTN* mtn_tbl, s32 key, f32 frame,
	f32* scale_x, f32* scale_y, f32* rotate)
{
	if ((key < 0) || (key_num == 0)) {
		*scale_x = 1.0f;
		*scale_y = 1.0f;
		*rotate = 0.0f;
		return;
	}

	// 2つのキーと補間率を算出する
	u32 key1 = (u32)key;
	amAssert((u32)key1 < key_num);
	u32 key2;
	f32 rate;
	if (!aoActGetInterpolInfo(
		key_tbl, key_num, frm_num, frame, key1, &key2, &rate))
	{
		// 補間しない
		const A2S_SUB_MTN* mtn1 = &mtn_tbl[key1];
		*scale_x = mtn1->scl_x;
		*scale_y = mtn1->scl_y;
		*rotate = mtn1->rot;
		return;
	}

	// 補間する
	const A2S_SUB_MTN* mtn1 = &mtn_tbl[key1];
	const A2S_SUB_MTN* mtn2 = &mtn_tbl[key2];

	// 補間率補正
	f32 s_rate = aoActGetAcceleRate(rate, mtn1->scl_accele);
	f32 r_rate = aoActGetAcceleRate(rate, mtn1->rot_accele);

	// 線形補間
	*scale_x = aoActInterpolF32(mtn1->scl_x, mtn2->scl_x, s_rate);
	*scale_y = aoActInterpolF32(mtn1->scl_y, mtn2->scl_y, s_rate);
	*rotate = aoActInterpolF32(mtn1->rot, mtn2->rot, r_rate);
}

// ===========================================================================
//! アニメ作成
// ===========================================================================
void aoActMakeAnm(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_ANM* anm_tbl, s32 key, f32 frame,
	s32* tex_id, AOS_ACT_RECT* rect, u32* clamp)
{
	if ((key < 0) || (key_num == 0)) {
		*tex_id = -1;
		return;
	}

	// 2つのキーと補間率を算出する
	u32 key1 = (u32)key;
	amAssert((u32)key1 < key_num);
	u32 key2;
	f32 rate;
	if (!aoActGetInterpolInfo(
		key_tbl, key_num, frm_num, frame, key1, &key2, &rate))
	{
		// 補間しない
		const A2S_SUB_ANM* anm1 = &anm_tbl[key1];
		*tex_id = anm1->tex_id;
		rect->left = anm1->texel.left;
		rect->top = anm1->texel.top;
		rect->right = anm1->texel.right;
		rect->bottom = anm1->texel.bottom;
		*clamp = anm1->clamp;
		return;
	}

	// 補間する
	const A2S_SUB_ANM* anm1 = &anm_tbl[key1];
	const A2S_SUB_ANM* anm2 = &anm_tbl[key2];

	// 補間率補正
	rate = aoActGetAcceleRate(rate, anm1->texel_accele);

	// 線形補間
	*tex_id = anm1->tex_id;
	rect->left = aoActInterpolF32(anm1->texel.left, anm2->texel.left, rate);
	rect->top = aoActInterpolF32(anm1->texel.top, anm2->texel.top, rate);
	rect->right = aoActInterpolF32(anm1->texel.right, anm2->texel.right, rate);
	rect->bottom =
		aoActInterpolF32(anm1->texel.bottom, anm2->texel.bottom, rate);
	*clamp = anm1->clamp;
}

// ===========================================================================
//! マテリアル作成
// ===========================================================================
void aoActMakeMat(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_MAT* mat_tbl, s32 key, f32 frame,
	AOS_ACT_COL* color, AOS_ACT_COL* fade, u32* blend)
{
	if ((key < 0) || (key_num == 0)) {
		color->r = 255;
		color->g = 255;
		color->b = 255;
		color->a = 255;
		fade->r = 0;
		fade->g = 0;
		fade->b = 0;
		fade->a = 0;
		*blend = A2D_AMA_BLEND_NORMAL;
		return;
	}

	// 2つのキーと補間率を算出する
	u32 key1 = (u32)key;
	amAssert((u32)key1 < key_num);
	u32 key2;
	f32 rate;
	if (!aoActGetInterpolInfo(
		key_tbl, key_num, frm_num, frame, key1, &key2, &rate))
	{
		// 補間しない
		const A2S_SUB_MAT* mat1 = &mat_tbl[key1];
		color->r = mat1->base.r;
		color->g = mat1->base.g;
		color->b = mat1->base.b;
		color->a = mat1->base.a;
		fade->r = mat1->fade.r;
		fade->g = mat1->fade.g;
		fade->b = mat1->fade.b;
		fade->a = mat1->fade.a;
		*blend = mat1->blend;
		return;
	}

	// 補間する
	const A2S_SUB_MAT* mat1 = &mat_tbl[key1];
	const A2S_SUB_MAT* mat2 = &mat_tbl[key2];

	// 補間率補正
	f32 b_rate = aoActGetAcceleRate(rate, mat1->base_accele);
	f32 f_rate = aoActGetAcceleRate(rate, mat1->fade_accele);

	// 線形補間
	*color = aoActInterpolCol(mat1->base, mat2->base, b_rate);
	*fade = aoActInterpolCol(mat1->fade, mat2->fade, f_rate);
	*blend = mat1->blend;
}

// ===========================================================================
//! アキュム作成
// ===========================================================================
void aoActMakeAcm(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_ACM* acm_tbl, s32 key, f32 frame,
	f32* tscale_x, f32* tscale_y, f32* scale_x, f32* scale_y, f32* rotate)
{
	if ((key < 0) || (key_num == 0)) {
		*tscale_x = 1.0f;
		*tscale_y = 1.0f;
		*scale_x = 1.0f;
		*scale_y = 1.0f;
		*rotate = 0.0f;
		return;
	}

	// 2つのキーと補間率を算出する
	u32 key1 = (u32)key;
	amAssert((u32)key1 < key_num);
	u32 key2;
	f32 rate;
	if (!aoActGetInterpolInfo(
		key_tbl, key_num, frm_num, frame, key1, &key2, &rate))
	{
		// 補間しない
		const A2S_SUB_ACM* acm1 = &acm_tbl[key1];
		*tscale_x = acm1->trs_scl_x;
		*tscale_y = acm1->trs_scl_y;
		*scale_x = acm1->scl_x;
		*scale_y = acm1->scl_y;
		*rotate = acm1->rot;
		return;
	}

	// 補間する
	const A2S_SUB_ACM* acm1 = &acm_tbl[key1];
	const A2S_SUB_ACM* acm2 = &acm_tbl[key2];

	// 補間率補正
	f32 t_rate = aoActGetAcceleRate(rate, acm1->trs_scl_accele);
	f32 s_rate = aoActGetAcceleRate(rate, acm1->scl_accele);
	f32 r_rate = aoActGetAcceleRate(rate, acm1->rot_accele);

	// 線形補間
	*tscale_x = aoActInterpolF32(acm1->trs_scl_x, acm2->trs_scl_x, t_rate);
	*tscale_y = aoActInterpolF32(acm1->trs_scl_y, acm2->trs_scl_y, t_rate);
	*scale_x = aoActInterpolF32(acm1->scl_x, acm2->scl_x, s_rate);
	*scale_y = aoActInterpolF32(acm1->scl_y, acm2->scl_y, s_rate);
	*rotate = aoActInterpolF32(acm1->rot, acm2->rot, r_rate);
}

// ===========================================================================
//! 当たり作成
// ===========================================================================
void aoActMakeHit(
	u32 key_num, u32 frm_num,
	const A2S_SUB_KEY* key_tbl, const A2S_SUB_HIT* hit_tbl, s32 key, f32 frame,
	AOS_ACT_HITP* hit)
{
	hit->scale_x = 1.0f;
	hit->scale_y = 1.0f;
	if ((key < 0) || (key_num == 0)) {
		hit->type = AOD_ACT_HIT_NONE;
		return;
	}

	// 2つのキーと補間率を算出する
	u32 key1 = (u32)key;
	amAssert((u32)key1 < key_num);
	u32 key2;
	f32 rate;
	if (!aoActGetInterpolInfo(
		key_tbl, key_num, frm_num, frame, key1, &key2, &rate))
	{
		// 補間しない
		const A2S_SUB_HIT* hit1 = &hit_tbl[key1];
		hit->flag = hit1->flag;
		switch (hit1->type) {
		case A2D_AMA_HIT_TYPE_RECT:
			hit->type = AOD_ACT_HIT_RECT;
			amCopyMemory(&hit->rect, &hit1->rect, sizeof(AOS_ACT_RECT));
			break;
		case A2D_AMA_HIT_TYPE_CIRCLE:
			hit->type = AOD_ACT_HIT_CIRCLE;
			amCopyMemory(&hit->circle, &hit1->circle, sizeof(AOS_ACT_CIRCLE));
			break;
		default:
			hit->type = AOD_ACT_HIT_NONE;
			break;
		}
		return;
	}

	// 補間する
	const A2S_SUB_HIT* hit1 = &hit_tbl[key1];
	const A2S_SUB_HIT* hit2 = &hit_tbl[key2];

	// タイプが違ったら補間しない
	if (hit1->type != hit2->type) {
		hit->flag = hit1->flag;
		switch (hit1->type) {
		case A2D_AMA_HIT_TYPE_RECT:
			hit->type = AOD_ACT_HIT_RECT;
			amCopyMemory(&hit->rect, &hit1->rect, sizeof(AOS_ACT_RECT));
			break;
		case A2D_AMA_HIT_TYPE_CIRCLE:
			hit->type = AOD_ACT_HIT_CIRCLE;
			amCopyMemory(&hit->circle, &hit1->circle, sizeof(AOS_ACT_CIRCLE));
			break;
		default:
			hit->type = AOD_ACT_HIT_NONE;
			break;
		}
		return;
	}

	// 補間率補正
	rate = aoActGetAcceleRate(rate, hit1->hit_accele);

	// 線形補間
	hit->flag = hit1->flag;
	switch (hit1->type) {
	case A2D_AMA_HIT_TYPE_RECT:
		hit->type = AOD_ACT_HIT_RECT;
		hit->rect.left =
			aoActInterpolF32(hit1->rect.left, hit2->rect.left, rate);
		hit->rect.top =
			aoActInterpolF32(hit1->rect.top, hit2->rect.top, rate);
		hit->rect.right =
			aoActInterpolF32(hit1->rect.right, hit2->rect.right, rate);
		hit->rect.bottom =
			aoActInterpolF32(hit1->rect.bottom, hit2->rect.bottom, rate);
		break;
	case A2D_AMA_HIT_TYPE_CIRCLE:
		hit->type = AOD_ACT_HIT_CIRCLE;
		hit->circle.center_x = aoActInterpolF32(
			hit1->circle.center_x, hit2->circle.center_x, rate);
		hit->circle.center_y = aoActInterpolF32(
			hit1->circle.center_y, hit2->circle.center_y, rate);
		hit->circle.radius = aoActInterpolF32(
			hit1->circle.radius, hit2->circle.radius, rate);
		break;
	default:
		hit->type = AOD_ACT_HIT_NONE;
		break;
	}
}

// ===========================================================================
//! キー検索
// ===========================================================================
void aoActSerachKey(const A2S_SUB_KEY* key, u32 key_num, f32* frame, s32* last)
{
	if (*frame < 1.0f) {
		*frame = 0.0f;
		*last = 0;
		return;
	}

	u32 frm = (u32)*frame;
	u32 id;

	if (*last < 0) {
		id = key_num / 2;
		u32 num = (key_num + 1) / 2;
		while (num > 1) {
			if (key[id].frm <= frm) {
				id += num / 2;
			}
			else {
				id -= num / 2;
			}
			num = (num + 1) / 2;
		}
		if (key[id].frm > frm) {
			id--;
		}
	}
	else {
		id = (u32)*last;
		while (1) {
			if (key[id].frm <= frm) {
				if (key[id + 1].frm > frm) {
					break;
				}
				else {
					id += 1;
				}
			}
			else {
				id -= 1;
			}
			if (id <= 0) {
				id = 0;
				break;
			}
			if (id >= (key_num - 1)) {
				id = key_num - 1;
				break;
			}
		}
	}

	*last = (s32)id;
	*frame -= (f32)key[id].frm;
}

// ===========================================================================
//! ループ時のフレーム取得
// ===========================================================================
f32 aoActGetLoopFrame(f32 frame, u32 len)
{
	u32 uf = (u32)frame % len;
	f32 ff = frame - (f32)((u32)frame);
	return (f32)uf + ff;
}

// ===========================================================================
//! 補間情報取得
// ===========================================================================
BOOL aoActGetInterpolInfo(
	const A2S_SUB_KEY* key_tbl, u32 key_num, u32 frm_num, f32 frame,
	u32 key1, u32* key2, f32* rate)
{
	const A2S_SUB_KEY* k1 = &key_tbl[key1];
	if (((k1->interpol != A2D_AMA_INT_LINEAR) &&
		 (k1->interpol != A2D_AMA_INT_SPLINE)) ||
		(key_num <= 1))
	{
		return FALSE;
	}

	if ((key1 + 1) < key_num) {
		*key2 = key1 + 1;
		const A2S_SUB_KEY* k2 = k1 + 1;
		*rate = frame / (f32)(k2->frm - k1->frm);
	}
	else {
		*key2 = 0;
		*rate = frame / (f32)(frm_num - k1->frm);
	}
	return TRUE;
}

// ===========================================================================
//! スプライン補間
// ===========================================================================
void aoActGetInterpolSpline(
	const A2S_SUB_TRS* t0, const A2S_SUB_TRS* t1,
	const A2S_SUB_TRS* t2, const A2S_SUB_TRS* t3, f32 rate,
	f32* trans_x, f32* trans_y, f32* trans_z)
{
	f32 r1 = rate;
	f32 r2 = r1 * r1;
	f32 r3 = r2 * r1;

	f32 a = (-0.5f * (r3 + r1)) + r2;
	f32 b = (1.5f * r3) - (2.5f * r2) + 1.0f;
	f32 c = (-1.5f * r3) + (2.0f * r2) + (0.5f * r1);
	f32 d = 0.5f * (r3 - r2);

	*trans_x = t0->trs_x * a + t1->trs_x * b + t2->trs_x * c + t3->trs_x * d;
	*trans_y = t0->trs_y * a + t1->trs_y * b + t2->trs_y * c + t3->trs_y * d;
	*trans_z = t0->trs_z * a + t1->trs_z * b + t2->trs_z * c + t3->trs_z * d;
}

// ===========================================================================
//! 加速度補間率算出
// ===========================================================================
f32 aoActGetAcceleRate(f32 rate, f32 accele)
{
	return (rate * accele) + ((rate * rate) * (1.0f - accele));
}


// ***************************************************************************
// バッファ操作
// ***************************************************************************
// ===========================================================================
//! スプライト確保
// ===========================================================================
AOS_SPRITE* aoActAllocSprite(void)
{
	// バッファオーバーチェック
	if (g_ao_act_spr_num >= g_ao_act_spr_buf_size) {
		amAssert(0);
		return NULL;
	}

	// 確保
	AOS_SPRITE* spr = g_ao_act_spr_ref[g_ao_act_spr_alloc];
	g_ao_act_spr_alloc += 1;
	if (g_ao_act_spr_alloc >= g_ao_act_spr_buf_size) {
		g_ao_act_spr_alloc = 0;
	}

	// 使用数更新
	g_ao_act_spr_num += 1;
	if (g_ao_act_spr_num > g_ao_act_spr_peak) {
		g_ao_act_spr_peak = g_ao_act_spr_num;
	}

	return spr;
}

// ===========================================================================
//! スプライト解放
// ===========================================================================
void aoActFreeSprite(AOS_SPRITE* spr)
{
	// バッファアンダーチェック
	if (g_ao_act_spr_num == 0) {
		amAssert(0);
		return;
	}

	// 管理しているスプライトか判定
	if ((u32)((u32)spr - (u32)g_ao_act_spr_buf) >=
		(sizeof(AOS_SPRITE) * g_ao_act_spr_buf_size))
	{
		amAssert(0);
		return;
	}

	// ソートバッファから削除
	AoActSortUnregSprite(spr);

	// 解放
	g_ao_act_spr_ref[g_ao_act_spr_free] = spr;
	g_ao_act_spr_free += 1;
	if (g_ao_act_spr_free >= g_ao_act_spr_buf_size) {
		g_ao_act_spr_free = 0;
	}

	// 使用数更新
	g_ao_act_spr_num -= 1;
	if (g_ao_act_spr_num > g_ao_act_spr_peak) {
		g_ao_act_spr_peak = g_ao_act_spr_num;
	}
}

// ===========================================================================
//! アクション確保
// ===========================================================================
AOS_ACTION* aoActAllocAction(void)
{
	// バッファオーバーチェック
	if (g_ao_act_num >= g_ao_act_buf_size) {
		amAssert(0);
		return NULL;
	}

	// 確保
	AOS_ACTION* act = g_ao_act_ref[g_ao_act_alloc];
	g_ao_act_alloc += 1;
	if (g_ao_act_alloc >= g_ao_act_buf_size) {
		g_ao_act_alloc = 0;
	}

	// 使用数更新
	g_ao_act_num += 1;
	if (g_ao_act_num > g_ao_act_peak) {
		g_ao_act_peak = g_ao_act_num;
	}

	return act;
}

// ===========================================================================
//! アクション解放
// ===========================================================================
void aoActFreeAction(AOS_ACTION* act)
{
	// バッファアンダーチェック
	if (g_ao_act_num == 0) {
		amAssert(0);
		return;
	}

	// 管理しているアクションか判定
	if ((u32)((u32)act - (u32)g_ao_act_buf) >=
		(sizeof(AOS_ACTION) * g_ao_act_buf_size))
	{
		amAssert(0);
		return;
	}

	// ソートバッファから削除
	AoActSortUnregAction(act);

	// 解放
	g_ao_act_ref[g_ao_act_free] = act;
	g_ao_act_free += 1;
	if (g_ao_act_free >= g_ao_act_buf_size) {
		g_ao_act_free = 0;
	}

	// 使用数更新
	g_ao_act_num -= 1;
	if (g_ao_act_num > g_ao_act_peak) {
		g_ao_act_peak = g_ao_act_num;
	}
}


// ***************************************************************************
// アクション終了判定
// ***************************************************************************
// ===========================================================================
//! アクション終了判定
// ===========================================================================
u32 aoActGetAmaActState(const A2S_AMA_ACT* act, f32 frame)
{
	u32 state = 0;
	if (act) {
		while (act->next) {
			f32 frames = (f32)act->frm_num;
			if (frames > frame) {
				break;
			}
			frame -= frames;
			act = act->next;
		}
		if (aoActIsAmaActEnd(act, frame)) {
			state |= AOD_ACT_STATE_ACT_END;
		}
		if (aoActIsAmaTrsEnd(act->mtn, frame)) {
			state |= AOD_ACT_STATE_TRS_END;
		}
		if (aoActIsAmaMtnEnd(act->mtn, frame)) {
			state |= AOD_ACT_STATE_MTN_END;
		}
		if (aoActIsAmaAnmEnd(act->anm, frame)) {
			state |= AOD_ACT_STATE_ANM_END;
		}
		if (aoActIsAmaMatEnd(act->anm, frame)) {
			state |= AOD_ACT_STATE_MAT_END;
		}
		if (act->acm) {
			if (act->acm->flag & A2D_AMA_ACM_FLAG_TRS_BODY) {
				if (aoActIsAmaTrsEnd(act->mtn, frame)) {
					state |= AOD_ACT_STATE_ATRS_END;
				}
			}
			else {
				if (aoActIsAmaAcmTrsEnd(act->acm, frame)) {
					state |= AOD_ACT_STATE_ATRS_END;
				}
			}
			if (act->acm->flag & A2D_AMA_ACM_FLAG_ACM_BODY) {
				if (aoActIsAmaMtnEnd(act->mtn, frame)) {
					state |= AOD_ACT_STATE_AMTN_END;
				}
			}
			else {
				if (aoActIsAmaAcmMtnEnd(act->acm, frame)) {
					state |= AOD_ACT_STATE_AMTN_END;
				}
			}
			if (act->acm->flag & A2D_AMA_ACM_FLAG_MAT_BODY) {
				if (aoActIsAmaMatEnd(act->anm, frame)) {
					state |= AOD_ACT_STATE_AMAT_END;
				}
			}
			else {
				if (aoActIsAmaAcmMatEnd(act->acm, frame)) {
					state |= AOD_ACT_STATE_AMAT_END;
				}
			}
		}
		if (aoActIsAmaUsrEnd(act->usr, frame)) {
			state |= AOD_ACT_STATE_USR_END;
		}
		if (aoActIsAmaHitEnd(act->hit, frame)) {
			state |= AOD_ACT_STATE_HIT_END;
		}
	}
	return state;
}

// ===========================================================================
//! トランス終了判定
// ===========================================================================
BOOL aoActIsAmaActEnd(const A2S_AMA_ACT* act, f32 frame)
{
	if (act) {
		if ((f32)act->frm_num > frame) {
			return FALSE;
		}
	}
	return TRUE;
}

// ===========================================================================
//! トランス終了判定
// ===========================================================================
BOOL aoActIsAmaTrsEnd(const A2S_AMA_MTN* mtn, f32 frame)
{
	if (mtn && mtn->trs_key_tbl) {
		if (mtn->flag & A2D_AMA_MTN_FLAG_TRS_LOOP) {
			return FALSE;
		}
		if ((f32)mtn->trs_frm_num > frame) {
			return FALSE;
		}
	}
	return TRUE;
}

// ===========================================================================
//! モーション終了判定
// ===========================================================================
BOOL aoActIsAmaMtnEnd(const A2S_AMA_MTN* mtn, f32 frame)
{
	if (mtn && mtn->mtn_key_tbl) {
		if (mtn->flag & A2D_AMA_MTN_FLAG_MTN_LOOP) {
			return FALSE;
		}
		if ((f32)mtn->mtn_frm_num > frame) {
			return FALSE;
		}
	}
	return TRUE;
}

// ===========================================================================
//! アニメ終了判定
// ===========================================================================
BOOL aoActIsAmaAnmEnd(const A2S_AMA_ANM* anm, f32 frame)
{
	if (anm && anm->anm_key_tbl) {
		if (anm->flag & A2D_AMA_ANM_FLAG_ANM_LOOP) {
			return FALSE;
		}
		if ((f32)anm->anm_frm_num > frame) {
			return FALSE;
		}
	}
	return TRUE;
}

// ===========================================================================
//! マテリアル終了判定
// ===========================================================================
BOOL aoActIsAmaMatEnd(const A2S_AMA_ANM* anm, f32 frame)
{
	if (anm && anm->mat_key_tbl) {
		if (anm->flag & A2D_AMA_ANM_FLAG_MAT_LOOP) {
			return FALSE;
		}
		if ((f32)anm->mat_frm_num > frame) {
			return FALSE;
		}
	}
	return TRUE;
}

// ===========================================================================
//! 継承トランス終了判定
// ===========================================================================
BOOL aoActIsAmaAcmTrsEnd(const A2S_AMA_ACM* acm, f32 frame)
{
	if (acm && acm->trs_key_tbl) {
		if (acm->flag & A2D_AMA_ACM_FLAG_TRS_LOOP) {
			return FALSE;
		}
		if ((f32)acm->trs_frm_num > frame) {
			return FALSE;
		}
	}
	return TRUE;
}

// ===========================================================================
//! 継承モーション終了判定
// ===========================================================================
BOOL aoActIsAmaAcmMtnEnd(const A2S_AMA_ACM* acm, f32 frame)
{
	if (acm && acm->acm_key_tbl) {
		if (acm->flag & A2D_AMA_ACM_FLAG_ACM_LOOP) {
			return FALSE;
		}
		if ((f32)acm->acm_frm_num > frame) {
			return FALSE;
		}
	}
	return TRUE;
}

// ===========================================================================
//! 継承マテリアル終了判定
// ===========================================================================
BOOL aoActIsAmaAcmMatEnd(const A2S_AMA_ACM* acm, f32 frame)
{
	if (acm && acm->mat_key_tbl) {
		if (acm->flag & A2D_AMA_ACM_FLAG_MAT_LOOP) {
			return FALSE;
		}
		if ((f32)acm->mat_frm_num > frame) {
			return FALSE;
		}
	}
	return TRUE;
}

// ===========================================================================
//! ユーザ終了判定
// ===========================================================================
BOOL aoActIsAmaUsrEnd(const A2S_AMA_USR* usr, f32 frame)
{
	if (usr && usr->usr_key_tbl) {
		if (usr->flag & A2D_AMA_USR_FLAG_USR_LOOP) {
			return FALSE;
		}
		if ((f32)usr->usr_frm_num > frame) {
			return FALSE;
		}
	}
	return TRUE;
}

// ===========================================================================
//! 当たり終了判定
// ===========================================================================
BOOL aoActIsAmaHitEnd(const A2S_AMA_HIT* hit, f32 frame)
{
	if (hit && hit->hit_key_tbl) {
		if (hit->flag & A2D_AMA_HIT_FLAG_HIT_LOOP) {
			return FALSE;
		}
		if ((f32)hit->hit_frm_num > frame) {
			return FALSE;
		}
	}
	return TRUE;
}


// ***************************************************************************
// AMAアクセス
// ***************************************************************************
// ===========================================================================
//! AMAアクション取得
// ===========================================================================
const A2S_AMA_ACT* aoActGetAmaAct(const AOS_ACTION* act)
{
	A2S_AMA_ACT* ret = NULL;
	if (act == NULL) {
		return NULL;
	}
	switch (act->type) {
	case AOD_ACT_TYPE_ACTION:
		ret = (A2S_AMA_ACT*)act->data;
		break;

	case AOD_ACT_TYPE_NODE:
		ret = ((A2S_AMA_NODE*)act->data)->act;
		break;

	default:
		// empty
		break;
	}
	return ret;
}


// ***************************************************************************
// ユーティリティ
// ***************************************************************************
// ===========================================================================
//! f32補間
// ===========================================================================
f32 aoActInterpolF32(f32 d1, f32 d2, f32 rate)
{
	return (d1 * (1.0f - rate)) + (d2 * rate);
}

// ===========================================================================
//! カラー補間
// ===========================================================================
AOS_ACT_COL aoActInterpolCol(A2S_SUB_COL d1, A2S_SUB_COL d2, f32 rate)
{
	AOS_ACT_COL c;
	s32 temp = (s32)(255.0f * rate);
	if (temp < 0) {
		temp = 0;
	}
	else if (temp > 255) {
		temp = 255;
	}
	s32 temp2 = 255 - temp;

	c.r = (u8)((((s32)d1.r * temp2) + ((s32)d2.r * temp)) / 255);
	c.g = (u8)((((s32)d1.g * temp2) + ((s32)d2.g * temp)) / 255);
	c.b = (u8)((((s32)d1.b * temp2) + ((s32)d2.b * temp)) / 255);
	c.a = (u8)((((s32)d1.a * temp2) + ((s32)d2.a * temp)) / 255);
	return c;
}


// ***************************************************************************
// 描画
// ***************************************************************************
// ===========================================================================
//! 描画タスク
// ===========================================================================
void aoActDrawTask(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_ACT_DRAW* work = *((AOS_ACT_DRAW**)amTaskGetWork(tcb));

	AOS_SPRITE* spr;
	AOS_SPRITE* sprnext;
	NNE_PRIM_TEXWRAP wrap_u, wrap_v;
	BOOL is_fade;

	// 描画ステート初期化
	amDrawPushState();
	amDrawInitState();

	// 前処理
	AoActDrawPre();

	// Z更新無効 & Zテスト無効
#if defined(AOD_PLATFORM_WIN32) | defined(AOD_PLATFORM_XBOX360)

	nnSetPrimitive3DAlphaTestDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthMaskDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthTestDXG20(NNE_FALSE);

#elif defined(AOD_PLATFORM_PS3)

	nnSetPrimitive3DAlphaTestPS3(NNE_FALSE);
	nnSetPrimitive3DDepthMaskPS3(NNE_FALSE);
	nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_ALWAYS);

#elif defined(AOD_PLATFORM_WII)

	nnSetPrimitive3DAlphaCompareGC(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	nnSetPrimitive3DZModeGC(GX_FALSE, GX_ALWAYS, GX_FALSE);

#elif defined(AOD_PLATFORM_IPHONE)

	nnSetPrimitive3DAlphaFuncGL(NND_CMPFUNC_GL_ALWAYS, 0.5f);
	nnSetPrimitive3DDepthMaskGL(FALSE);
	nnSetPrimitive3DDepthFuncGL(NND_CMPFUNC_GL_ALWAYS);

#endif

	// スプライト描画
	for (u32 i = 0; i < work->count; ++i) {
		spr = &work->sprite[i];

		// ブレンド設定
		switch (spr->blend) {
		case A2D_AMA_BLEND_ADD:
#if defined(AOD_PLATFORM_WIN32) | defined(AOD_PLATFORM_XBOX360)
			nnSetPrimitive3DBlendDXG20(
				NNE_BLENDMODE_SRCCOL, NNE_BLENDMODE_DSTCOL, NNE_BLENDOP_ADD);
#elif defined(AOD_PLATFORM_PS3)
			nnSetPrimitive3DBlendPS3(
				NND_BLENDFUNC_PS3_SRC_COLOR, NND_BLENDFUNC_PS3_DST_COLOR,
				NND_BLENDOP_PS3_FUNC_ADD);
#elif defined(AOD_PLATFORM_WII)
			nnSetPrimitive3DBlendModeGC(
				GX_BM_BLEND, GX_BL_SRCCLR, GX_BL_DSTCLR, GX_LO_NOOP);
#elif defined(AOD_PLATFORM_IPHONE)
			nnSetPrimitiveBlend(NNE_PRIM_BLEND_ADD);
#endif
			break;

		default:
#if defined(AOD_PLATFORM_WIN32) | defined(AOD_PLATFORM_XBOX360)
			nnSetPrimitive3DBlendDXG20(
				NNE_BLENDMODE_SRCALPHA, NNE_BLENDMODE_INVSRCALPHA,
				NNE_BLENDOP_ADD);
#elif defined(AOD_PLATFORM_PS3)
			nnSetPrimitive3DBlendPS3(
				NND_BLENDFUNC_PS3_SRC_ALPHA,
				NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA,
				NND_BLENDOP_PS3_FUNC_ADD);
#elif defined(AOD_PLATFORM_WII)
			nnSetPrimitive3DBlendModeGC(
				GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
#elif defined(AOD_PLATFORM_IPHONE)
			nnSetPrimitiveBlend(NNE_PRIM_BLEND_BLEND);
#endif
			break;
		}

		// フェード設定
		if (spr->fade.a > 0) {
			amDrawSetFogColor(
				((f32)spr->fade.r / 255.0f),
				((f32)spr->fade.g / 255.0f),
				((f32)spr->fade.b / 255.0f));
			f32 n = 2.0f - ((f32)spr->fade.a / 255.0f);
			amDrawSetFogRange(n, n + 1.0f);
			amDrawSetFog(1);
			is_fade = TRUE;
		}
		else {
			amDrawSetFog(0);
			is_fade = FALSE;
		}

		// テクスチャの有無で分岐
		if ((spr->tex_id >= 0) && spr->texlist) {

			// テクスチャ設定
			nnSetPrimitiveTexNum(spr->texlist, spr->tex_id);

			if (spr->clamp & A2D_AMA_CLAMP_U) {
				wrap_u = NNE_PRIM_TEXWRAP_CLAMP;
			}
			else {
				wrap_u = NNE_PRIM_TEXWRAP_REPEAT;
			}
			if (spr->clamp & A2D_AMA_CLAMP_V) {
				wrap_v = NNE_PRIM_TEXWRAP_CLAMP;
			}
			else {
				wrap_v = NNE_PRIM_TEXWRAP_REPEAT;
			}
			nnSetPrimitiveTexState(
				NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV,
				wrap_u, wrap_v);

			NNE_PRIM_ALPHABLEND ablend;
			if (spr->blend == A2D_AMA_BLEND_NONE) {
				ablend = NNE_PRIM_ALPHABLEND_OFF;
			}
			else {
				ablend = NNE_PRIM_ALPHABLEND_ON;
			}
			nnBeginDrawPrimitive3D(
				NNE_PRIM3D_FMT_PCT,
				ablend,
				NNE_PRIM_LIGHT_DISABLE,
				NNE_PRIM_CULL_NONE);
			NNS_PRIM3D_PCT v[6];

			do {

				// UV座標設定
				v[0].Tex.u = v[1].Tex.u = spr->uv.left;
				v[2].Tex.u = v[3].Tex.u = spr->uv.right;
				v[0].Tex.v = v[2].Tex.v = spr->uv.top;
				v[1].Tex.v = v[3].Tex.v = spr->uv.bottom;

				// カラー設定
				v[0].Col = (u32)(
					(spr->color.r << 24) |
					(spr->color.g << 16) |
					(spr->color.b <<  8) |
					(spr->color.a <<  0));
				v[1].Col = v[2].Col = v[3].Col = v[0].Col;

				// 頂点設定
				v[0].Pos.x = v[1].Pos.x = spr->offset.left;
				v[2].Pos.x = v[3].Pos.x = spr->offset.right;
				v[0].Pos.y = v[2].Pos.y = spr->offset.top;
				v[1].Pos.y = v[3].Pos.y = spr->offset.bottom;
				v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.0f;

				// 回転
				if (spr->rotate != 0) {
					f32 sn, cs, x, y;
					amSinCos(NNM_DEGtoA32(spr->rotate), &sn, &cs);
					for (int r = 0; r < 4; ++r) {
						x = v[r].Pos.x * cs - v[r].Pos.y * sn;
						y = v[r].Pos.x * sn + v[r].Pos.y * cs;
						v[r].Pos.x = x;
						v[r].Pos.y = y;
					}
				}

				// 移動
				v[0].Pos.x += spr->center_x;
				v[1].Pos.x += spr->center_x;
				v[2].Pos.x += spr->center_x;
				v[3].Pos.x += spr->center_x;
				v[0].Pos.y += spr->center_y;
				v[1].Pos.y += spr->center_y;
				v[2].Pos.y += spr->center_y;
				v[3].Pos.y += spr->center_y;

				// ワイド補正
				aoActDrawCorW(v, 4, spr->flag);

				// 6頂点化
				v[5] = v[3];
				v[4] = v[1];
				v[3] = v[2];

				// 描画
				nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_LIST, v, 6);

				// 次スプライトが同じ描画ステートなら連続描画
				if ((i + 1) < work->count) {
					sprnext = &work->sprite[i + 1];
					if ((spr->blend == sprnext->blend) &&
						(spr->fade.c == sprnext->fade.c) &&
						(spr->texlist == sprnext->texlist) &&
						(spr->tex_id == sprnext->tex_id) &&
						(spr->clamp == sprnext->clamp))
					{
						spr = sprnext;
						i += 1;
					}
					else {
						spr = NULL;
					}
				}
				else {
					spr = NULL;
				}
			} while (spr);

			nnEndDrawPrimitive3D();
		}
		else {

			// テクスチャ設定
			nnSetPrimitiveTexNum(NULL, -1);

			NNE_PRIM_ALPHABLEND ablend;
			if (spr->blend == A2D_AMA_BLEND_NONE) {
				ablend = NNE_PRIM_ALPHABLEND_OFF;
			}
			else {
				ablend = NNE_PRIM_ALPHABLEND_ON;
			}
			nnBeginDrawPrimitive3D(
				NNE_PRIM3D_FMT_PC,
				ablend,
				NNE_PRIM_LIGHT_DISABLE,
				NNE_PRIM_CULL_NONE);
			NNS_PRIM3D_PC v[6];

			do {

				// カラー設定
				v[0].Col = (u32)(
					(spr->color.r << 24) |
					(spr->color.g << 16) |
					(spr->color.b <<  8) |
					(spr->color.a <<  0));
				v[1].Col = v[2].Col = v[3].Col = v[0].Col;

				// 頂点設定
				v[0].Pos.x = v[1].Pos.x = spr->offset.left;
				v[2].Pos.x = v[3].Pos.x = spr->offset.right;
				v[0].Pos.y = v[2].Pos.y = spr->offset.top;
				v[1].Pos.y = v[3].Pos.y = spr->offset.bottom;
				v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.0f;

				// 回転
				if (spr->rotate != 0) {
					f32 sn, cs, x, y;
					amSinCos(NNM_DEGtoA32(spr->rotate), &sn, &cs);
					for (int r = 0; r < 4; ++r) {
						x = v[r].Pos.x * cs - v[r].Pos.y * sn;
						y = v[r].Pos.x * sn + v[r].Pos.y * cs;
						v[r].Pos.x = x;
						v[r].Pos.y = y;
					}
				}

				// 移動
				v[0].Pos.x += spr->center_x;
				v[1].Pos.x += spr->center_x;
				v[2].Pos.x += spr->center_x;
				v[3].Pos.x += spr->center_x;
				v[0].Pos.y += spr->center_y;
				v[1].Pos.y += spr->center_y;
				v[2].Pos.y += spr->center_y;
				v[3].Pos.y += spr->center_y;

				// ワイド補正
				aoActDrawCorW(v, 4, spr->flag);

				// 6頂点化
				v[5] = v[3];
				v[4] = v[1];
				v[3] = v[2];

				// 描画
				nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_LIST, v, 6);

				// 次スプライトが同じ描画ステートなら連続描画
				if ((i + 1) < work->count) {
					sprnext = &work->sprite[i + 1];
					if ((spr->blend == sprnext->blend) &&
						(spr->fade.c == sprnext->fade.c) &&
						((sprnext->texlist == NULL) || (sprnext->tex_id < 0)))
					{
						spr = sprnext;
						i += 1;
					}
					else {
						spr = NULL;
					}
				}
				else {
					spr = NULL;
				}
			} while (spr);

			nnEndDrawPrimitive3D();
		}

		// フェード後処理
		if (is_fade) {
			amDrawSetFog(0);
		}
	}

#if defined(AOD_DEBUG)

	// 当たり描画(簡単に作ったので多少重い)
	if (work->show_hit) {
		nnSetPrimitiveBlend(NNE_PRIM_BLEND_BLEND);
		nnSetPrimitiveTexNum(NULL, -1);

		f32 sn, cs, x, y;
		NNS_RGBA c;
		c.r = (f32)((work->hit_color >> 24) & 0xff) / 255.0f;
		c.g = (f32)((work->hit_color >> 16) & 0xff) / 255.0f;
		c.b = (f32)((work->hit_color >>  8) & 0xff) / 255.0f;
		c.a = (f32)((work->hit_color >>  0) & 0xff) / 255.0f;
		NNS_PRIM3D_P v[17];

		if ((work->hit_color & 0x000000ff) < 255) {
			nnBeginDrawPrimitiveLine3D(&c, NNE_PRIM_ALPHABLEND_ON);
		}
		else {
			nnBeginDrawPrimitiveLine3D(&c, NNE_PRIM_ALPHABLEND_OFF);
		}
		for (u32 i = 0; i < work->count; ++i) {
			AOS_ACT_HIT hit;
			if (!AoActGetHitSpr(&hit, &work->sprite[i])) {
				continue;
			}

			amSinCos(NNM_DEGtoA32(hit.rotate), &sn, &cs);

			switch (hit.type) {
			case AOD_ACT_HIT_RECT:
				v[0].Pos.x = v[1].Pos.x = hit.rect.left * hit.scale_x;
				v[2].Pos.x = v[3].Pos.x = hit.rect.right * hit.scale_x;
				v[0].Pos.y = v[3].Pos.y = hit.rect.top * hit.scale_y;
				v[1].Pos.y = v[2].Pos.y = hit.rect.bottom * hit.scale_y;
				for (u32 j = 0; j < 4; ++j) {
					x = v[j].Pos.x * cs - v[j].Pos.y * sn;
					y = v[j].Pos.x * sn + v[j].Pos.y * cs;
					v[j].Pos.x = x + hit.center_x;
					v[j].Pos.y = y + hit.center_y;
					v[j].Pos.z = -1.0f;
				}
				v[4].Pos = v[0].Pos;
				aoActDrawCorW(v, 5, work->sprite[i].flag);
				nnDrawPrimitiveLine3D(NNE_PRIM_LINE_STRIP, v, 5);
				break;

			case AOD_ACT_HIT_CIRCLE:
				for (u32 j = 0; j < 16; ++j) {
					f32 sn2, cs2;
					amSinCos(NNM_DEGtoA32((360 * j) / 16), &sn2, &cs2);
					v[j].Pos.x = hit.circle.radius * sn2;
					v[j].Pos.y = hit.circle.radius * cs2;
					v[j].Pos.x *= hit.scale_x;
					v[j].Pos.y *= hit.scale_y;
					x = v[j].Pos.x * cs - v[j].Pos.y * sn;
					y = v[j].Pos.x * sn + v[j].Pos.y * cs;
					v[j].Pos.x = x + hit.center_x;
					v[j].Pos.y = y + hit.center_y;
					v[j].Pos.z = -1.0f;
				}
				v[16].Pos = v[0].Pos;
				aoActDrawCorW(v, 17, work->sprite[i].flag);
				nnDrawPrimitiveLine3D(NNE_PRIM_LINE_STRIP, v, 17);
				break;

			default:
				// empty
				break;
			}
		}
		nnEndDrawPrimitiveLine3D();
	}

#endif // defined(AOD_DEBUG)

	// 描画ステート復帰
	amDrawPopState();
}

// ===========================================================================
//! スプライトステート描画
// ===========================================================================
void aoActDrawSprState(AOS_SPRITE** spr_tbl, u32 num)
{
	u32 spr_no = 0;
	while (spr_no < num) {

		// スプライト取得
		AOS_SPRITE* spr = spr_tbl[spr_no];

		// 同じ描画ステートのスプライトの連続数を算出
		u32 draw_num = 1;
		for (u32 i = (u32)(spr_no + 1); i < num; ++i, ++draw_num) {
			AOS_SPRITE* next = spr_tbl[i];
			if ((spr->blend != next->blend) ||
				(spr->fade.c != next->fade.c) ||
				(spr->texlist != next->texlist) ||
				(spr->tex_id != next->tex_id) ||
				(spr->clamp != next->clamp))
			{
				break;
			}
		}

		AMS_PARAM_DRAW_PRIMITIVE dat;
		amZeroMemory(&dat, sizeof(AMS_PARAM_DRAW_PRIMITIVE));

		// フェード設定
		// ※ソートされるとフォグが聞かなくなるので解決されるまで無効化
		if (spr->fade.a > 0) {
			amDrawSetFogColor(
				g_ao_act_sys_draw_state,
				((f32)spr->fade.r / 255.0f),
				((f32)spr->fade.g / 255.0f),
				((f32)spr->fade.b / 255.0f));
			f32 n = 2.0f - ((f32)spr->fade.a / 255.0f);
			amDrawSetFogRange(g_ao_act_sys_draw_state, n, n + 1.0f);
			amDrawSetFog(g_ao_act_sys_draw_state, 1);
		}
		else {
			amDrawSetFog(g_ao_act_sys_draw_state, 0);
		}

		// ベースマトリックスなし
		dat.mtx = NULL;

		// プリミティブ設定
		dat.type = NNE_PRIM_TRIANGLE_LIST;
		dat.count = (s32)(6 * draw_num);

		// アルファブレンド設定
		if (spr->blend == A2D_AMA_BLEND_NONE) {
			dat.ablend = NNE_PRIM_ALPHABLEND_OFF;
		}
		else {
			dat.ablend = NNE_PRIM_ALPHABLEND_ON;
		}

		// ソート設定
		dat.sortZ = 0.0f;

	// ブレンド設定
#if defined(AOD_PLATFORM_WIN32) | defined(AOD_PLATFORM_XBOX360)

		switch (spr->blend) {
		case A2D_AMA_BLEND_ADD:
			dat.bldSrc = NNE_BLENDMODE_SRCCOL;
			dat.bldDst = NNE_BLENDMODE_DSTCOL;
			dat.bldMode = NNE_BLENDOP_ADD;
		default:
			dat.bldSrc = NNE_BLENDMODE_SRCALPHA;
			dat.bldDst = NNE_BLENDMODE_INVSRCALPHA;
			dat.bldMode = NNE_BLENDOP_ADD;
		}

#elif defined(AOD_PLATFORM_PS3)

		switch (spr->blend) {
		case A2D_AMA_BLEND_ADD:
			dat.bldSrc = NND_BLENDFUNC_PS3_SRC_COLOR;
			dat.bldDst = NND_BLENDFUNC_PS3_DST_COLOR;
			dat.bldMode = NND_BLENDOP_PS3_FUNC_ADD;
		default:
			dat.bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
			dat.bldDst = NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA;
			dat.bldMode = NND_BLENDOP_PS3_FUNC_ADD;
		}

#elif defined(AOD_PLATFORM_WII)

		switch (spr->blend) {
		case A2D_AMA_BLEND_ADD:
			dat.bldSrc = GX_BL_SRCCLR;
			dat.bldDst = GX_BL_DSTCLR;
			dat.bldMode = GX_BM_BLEND;
		default:
			dat.bldSrc = GX_BL_SRCALPHA;
			dat.bldDst = GX_BL_INVSRCALPHA;
			dat.bldMode = GX_BM_BLEND;
		}

#elif defined(AOD_PLATFORM_IPHONE)

		switch (spr->blend) {
		case A2D_AMA_BLEND_ADD:
			dat.bldSrc = NND_BLENDFUNC_GL_SRC_COLOR;
			dat.bldDst = NND_BLENDFUNC_GL_DST_COLOR;
			dat.bldMode = NND_BLENDOP_GL_FUNC_ADD;
		default:
			dat.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
			dat.bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
			dat.bldMode = NND_BLENDOP_GL_FUNC_ADD;
		}

#endif

		// テスト設定
		dat.aTest = 0;
		dat.zMask = 1;
		dat.zTest = 0;

		// ソートしない
		dat.noSort = 1;

		// テクスチャの有無で分岐
		if ((spr->tex_id >= 0) && spr->texlist) {

			// テクスチャ設定
			dat.texlist = spr->texlist;
			dat.texId = spr->tex_id;

			// テクスチャクランプ設定
			if (spr->clamp & A2D_AMA_CLAMP_U) {
				dat.uwrap = NNE_PRIM_TEXWRAP_CLAMP;
			}
			else {
				dat.uwrap = NNE_PRIM_TEXWRAP_REPEAT;
			}
			if (spr->clamp & A2D_AMA_CLAMP_V) {
				dat.vwrap = NNE_PRIM_TEXWRAP_CLAMP;
			}
			else {
				dat.vwrap = NNE_PRIM_TEXWRAP_REPEAT;
			}

			// 頂点データ作成
			NNS_PRIM3D_PCT* v_tbl = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer(
				(s32)(sizeof(NNS_PRIM3D_PCT) * (6 * draw_num)));
			dat.vtxPCT3D = v_tbl;
			dat.format3D = NNE_PRIM3D_FMT_PCT;

			for (u32 i = 0; i < draw_num; ++i) {

				AOS_SPRITE* s = spr_tbl[spr_no + i];
				NNS_PRIM3D_PCT* v = v_tbl + (6 * i);

				// UV座標設定
				v[0].Tex.u = v[1].Tex.u = s->uv.left;
				v[2].Tex.u = v[3].Tex.u = s->uv.right;
				v[0].Tex.v = v[2].Tex.v = s->uv.top;
				v[1].Tex.v = v[3].Tex.v = s->uv.bottom;

				// カラー設定
				v[0].Col = (u32)(
					(s->color.r << 24) |
					(s->color.g << 16) |
					(s->color.b <<  8) |
					(s->color.a <<  0));
				v[1].Col = v[2].Col = v[3].Col = v[0].Col;

				// 頂点設定
				v[0].Pos.x = v[1].Pos.x = s->offset.left;
				v[2].Pos.x = v[3].Pos.x = s->offset.right;
				v[0].Pos.y = v[2].Pos.y = s->offset.top;
				v[1].Pos.y = v[3].Pos.y = s->offset.bottom;
				v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.0f;

				// 回転
				if (s->rotate != 0) {
					f32 sn, cs, x, y;
					amSinCos(NNM_DEGtoA32(s->rotate), &sn, &cs);
					for (int r = 0; r < 4; ++r) {
						x = v[r].Pos.x * cs - v[r].Pos.y * sn;
						y = v[r].Pos.x * sn + v[r].Pos.y * cs;
						v[r].Pos.x = x;
						v[r].Pos.y = y;
					}
				}

				// 移動
				v[0].Pos.x += s->center_x;
				v[1].Pos.x += s->center_x;
				v[2].Pos.x += s->center_x;
				v[3].Pos.x += s->center_x;
				v[0].Pos.y += s->center_y;
				v[1].Pos.y += s->center_y;
				v[2].Pos.y += s->center_y;
				v[3].Pos.y += s->center_y;

				// ワイド補正
				aoActDrawCorW(v, 4, s->flag);

				// 6頂点化
				v[5] = v[3];
				v[4] = v[1];
				v[3] = v[2];
			}
		}
		else {

			// テクスチャ設定
			dat.texlist = NULL;
			dat.texId = -1;

			// 頂点データ作成
			NNS_PRIM3D_PC* v_tbl = (NNS_PRIM3D_PC*)amDrawMallocDataBuffer(
				(s32)(sizeof(NNS_PRIM3D_PC) * (6 * draw_num)));
			dat.vtxPC3D = v_tbl;
			dat.format3D = NNE_PRIM3D_FMT_PC;

			for (u32 i = 0; i < draw_num; ++i) {

				AOS_SPRITE* s = spr_tbl[spr_no + i];
				NNS_PRIM3D_PC* v = v_tbl + (6 * i);

				// カラー設定
				v[0].Col = (u32)(
					(s->color.r << 24) |
					(s->color.g << 16) |
					(s->color.b <<  8) |
					(s->color.a <<  0));
				v[1].Col = v[2].Col = v[3].Col = v[0].Col;

				// 頂点設定
				v[0].Pos.x = v[1].Pos.x = s->offset.left;
				v[2].Pos.x = v[3].Pos.x = s->offset.right;
				v[0].Pos.y = v[2].Pos.y = s->offset.top;
				v[1].Pos.y = v[3].Pos.y = s->offset.bottom;
				v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.0f;

				// 回転
				if (s->rotate != 0) {
					f32 sn, cs, x, y;
					amSinCos(NNM_DEGtoA32(s->rotate), &sn, &cs);
					for (int r = 0; r < 4; ++r) {
						x = v[r].Pos.x * cs - v[r].Pos.y * sn;
						y = v[r].Pos.x * sn + v[r].Pos.y * cs;
						v[r].Pos.x = x;
						v[r].Pos.y = y;
					}
				}

				// 移動
				v[0].Pos.x += s->center_x;
				v[1].Pos.x += s->center_x;
				v[2].Pos.x += s->center_x;
				v[3].Pos.x += s->center_x;
				v[0].Pos.y += s->center_y;
				v[1].Pos.y += s->center_y;
				v[2].Pos.y += s->center_y;
				v[3].Pos.y += s->center_y;

				// ワイド補正
				aoActDrawCorW(v, 4, s->flag);

				// 6頂点化
				v[5] = v[3];
				v[4] = v[1];
				v[3] = v[2];
			}
		}

		// コマンド登録
		amDrawPrimitive3D(g_ao_act_sys_draw_state, &dat);

		spr_no += draw_num;
	}

	// フォグ設定をリセット
	amDrawSetFog(g_ao_act_sys_draw_state, 0);
}

// ===========================================================================
//! ソートバッファステート描画
// ===========================================================================
void aoActDrawSortState(void)
{
	if (g_ao_act_sort_num > 0) {
		AOS_SPRITE** spr_tbl =
			(AOS_SPRITE**)amMemAlloc(sizeof(AOS_SPRITE*) * g_ao_act_sort_num);
		for (u32 i = 0; i < g_ao_act_sort_num; ++i) {
			spr_tbl[i] = g_ao_act_sort_buf[i].sprite;
		}
		aoActDrawSprState(spr_tbl, g_ao_act_sort_num);
		amMemFree(spr_tbl);
	}
}

// ===========================================================================
//! 頂点のワイド補正計算
// ===========================================================================
void aoActDrawCorW(NNS_PRIM3D_P* v, u32 vnum, u32 flag)
{
	switch (flag & A2D_AMA_ACT_FLAG_W_MASK) {
	case A2D_AMA_ACT_FLAG_W_NONE:
		AoActDrawCorWide(v, vnum, AOD_ACT_CORW_NONE);
		break;
	case A2D_AMA_ACT_FLAG_W_CENTER:
		AoActDrawCorWide(v, vnum, AOD_ACT_CORW_CENTER);
		break;
	case A2D_AMA_ACT_FLAG_W_LEFT:
		if (flag & A2D_AMA_ACT_FLAG_W_SHIFT) {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_LEFT_S);
		}
		else {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_LEFT);
		}
		break;
	case A2D_AMA_ACT_FLAG_W_RIGHT:
		if (flag & A2D_AMA_ACT_FLAG_W_SHIFT) {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_RIGHT_S);
		}
		else {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_RIGHT);
		}
		break;
	}
}

// ===========================================================================
//! 頂点のワイド補正計算
// ===========================================================================
void aoActDrawCorW(NNS_PRIM3D_PC* v, u32 vnum, u32 flag)
{
	switch (flag & A2D_AMA_ACT_FLAG_W_MASK) {
	case A2D_AMA_ACT_FLAG_W_NONE:
		AoActDrawCorWide(v, vnum, AOD_ACT_CORW_NONE);
		break;
	case A2D_AMA_ACT_FLAG_W_CENTER:
		AoActDrawCorWide(v, vnum, AOD_ACT_CORW_CENTER);
		break;
	case A2D_AMA_ACT_FLAG_W_LEFT:
		if (flag & A2D_AMA_ACT_FLAG_W_SHIFT) {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_LEFT_S);
		}
		else {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_LEFT);
		}
		break;
	case A2D_AMA_ACT_FLAG_W_RIGHT:
		if (flag & A2D_AMA_ACT_FLAG_W_SHIFT) {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_RIGHT_S);
		}
		else {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_RIGHT);
		}
		break;
	}
}

// ===========================================================================
//! 頂点のワイド補正計算
// ===========================================================================
void aoActDrawCorW(NNS_PRIM3D_PCT* v, u32 vnum, u32 flag)
{
	switch (flag & A2D_AMA_ACT_FLAG_W_MASK) {
	case A2D_AMA_ACT_FLAG_W_NONE:
		AoActDrawCorWide(v, vnum, AOD_ACT_CORW_NONE);
		break;
	case A2D_AMA_ACT_FLAG_W_CENTER:
		AoActDrawCorWide(v, vnum, AOD_ACT_CORW_CENTER);
		break;
	case A2D_AMA_ACT_FLAG_W_LEFT:
		if (flag & A2D_AMA_ACT_FLAG_W_SHIFT) {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_LEFT_S);
		}
		else {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_LEFT);
		}
		break;
	case A2D_AMA_ACT_FLAG_W_RIGHT:
		if (flag & A2D_AMA_ACT_FLAG_W_SHIFT) {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_RIGHT_S);
		}
		else {
			AoActDrawCorWide(v, vnum, AOD_ACT_CORW_RIGHT);
		}
		break;
	}
}


// ***************************************************************************
// デバッグ
// ***************************************************************************
#if defined(AOD_DEBUG)

// ===========================================================================
//	AoActDebugShowInfo
/*!
	システム情報表示

	@param x				[in] デバッグテキスト表示左上X位置
	@param y				[in] デバッグテキスト表示左上Y位置
*/
// ===========================================================================
void AoActDebugShowInfo(u32 x, u32 y)
{
	amPrintColor(0xffcfaf00);
	amPrint((s32)(x + 0), (s32)(y + 0), "[2D ACTION INFO]");
	amPrintColor(0xffffffff);

	u32 size;
	u32 now;
	u32 peak;

	// スプライトバッファ
	size = AoActSysGetSprBufferSize();
	now = size - AoActSysGetSprBufferRemain();
	peak = AoActSysGetSprBufferPeak();
	amPrintf(
		(s32)(x + 0), (s32)(y + 1), "SPR  : %d / %d (%d)", now, size, peak);

	// アクションバッファ
	size = AoActSysGetActBufferSize();
	now = size - AoActSysGetActBufferRemain();
	peak = AoActSysGetActBufferPeak();
	amPrintf(
		(s32)(x + 0), (s32)(y + 2), "ACT  : %d / %d (%d)", now, size, peak);

	// ソートバッファ
	size = AoActSysGetSortBufferSize();
	now = size - AoActSysGetSortBufferRemain();
	peak = AoActSysGetSortBufferPeak();
	amPrintf(
		(s32)(x + 0), (s32)(y + 3), "SORT : %d / %d (%d)", now, size, peak);

	// アキュムレートスタック
	size = AoActSysGetAcmStackSize();
	now = size - AoActSysGetAcmStackRemain();
	peak = AoActSysGetAcmBufferPeak();
	amPrintf(
		(s32)(x + 0), (s32)(y + 4), "ACM  : %d / %d (%d)", now, size, peak);

	// アキュムレートフラグスタック
	size = AoActSysGetAcmFlagStackSize();
	now = size - AoActSysGetAcmFlagStackRemain();
	peak = AoActSysGetAcmFlagBufferPeak();
	amPrintf(
		(s32)(x + 0), (s32)(y + 5), "ACMF : %d / %d (%d)", now, size, peak);
}

// ===========================================================================
//	AoActDebugSetShowHitFlag
/*!
	当たり表示設定

	@param enable			[in] 真：表示　偽：非表示
*/
// ===========================================================================
void AoActDebugSetShowHitFlag(BOOL enable)
{
	g_ao_act_debug_show_hit_flag = enable;
}

// ===========================================================================
//	AoActDebugGetShowHitFlag
/*!
	当たり表示取得

	@return 真：表示　偽：非表示
*/
// ===========================================================================
BOOL AoActDebugGetShowHitFlag(void)
{
	return g_ao_act_debug_show_hit_flag;
}

// ===========================================================================
//	AoActDebugSetShowHitColor
/*!
	当たり表示色設定

	@param color			[in] 当たり表示色(RGBA8888)
*/
// ===========================================================================
void AoActDebugSetShowHitColor(u32 color)
{
	g_ao_act_debug_show_hit_color = color;
}

// ===========================================================================
//	AoActDebugGetShowHitColor
/*!
	当たり表示色取得

	@return 当たり表示色(RGBA8888)
*/
// ===========================================================================
u32 AoActDebugGetShowHitColor(void)
{
	return g_ao_act_debug_show_hit_color;
}

#endif // defined(AOD_DEBUG)

// ===========================================================================
//	function
/*!
	説明

	@param param0	[in] 入力引数0説明
	@param param1	[out] 出力ポインタ引数1説明
	@param param2	[io] 入出力ポインタ引数2説明
	@return 返値説明
	@note 補足説明
*/
// ===========================================================================

// ===========================================================================
//	int variable
// ---------------------------------------------------------------------------
//!	変数説明
// ===========================================================================

	// =======================================================================
	//	function
	/*!
		説明

		@param param0	[in] 入力引数0説明
		@param param1	[out] 出力ポインタ引数1説明
		@param param2	[io] 入出力ポインタ引数2説明
		@return 返値説明
		@note 補足説明
	*/
	// =======================================================================

// ***************************************************************************
// ラベル
// ***************************************************************************
