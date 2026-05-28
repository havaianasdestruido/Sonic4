// ===========================================================================
/*!
	@file	aoFont.cpp
	@brief	AoLibrary システムフォントモジュール定義(Wii)

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "ao.h"

#if defined(AOD_PLATFORM_WII)

// ----- Macros ------------------------------------------------（マクロ定義）

#define AOD_FONT_SPACE_WIDTH	(12)	//!< 空白文字のサイズ

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//	struct AOS_FONT_WORK
// ---------------------------------------------------------------------------
//!	構築タスクのワーク
// ===========================================================================
typedef struct tag_AOS_FONT_WORK {
	void*		file;		//!< フォントファイル
	BOOL		file_free;	//!< フォントファイル自動解放フラグ
	u32			buf_size;	//!< フォントバッファサイズ
	FNTHeader*	buf;		//!< フォントバッファ
	AMS_THREAD*	th;			//!< 構築スレッド
} AOS_FONT_WORK;

// ===========================================================================
//	struct AOS_FONT_WORK
// ---------------------------------------------------------------------------
//!	描画タスクのワーク
// ===========================================================================
typedef struct tag_AOS_FONT_DWORK {
	f32				x;		//!< 描画X位置
	f32				y;		//!< 描画Y位置
	AOE_FONT_ALIGN	align;	//!< 行揃えタイプ
	u32				color;	//!< 文字色
	f32				scale;	//!< 拡大率
	f32				cdiff;	//!< 次文字位置のオフセット
	f32				ldiff;	//!< 次行位置のオフセット
	u32				corw;	//!< ワイド補正タイプ
	const char*		str;	//!< 描画文字列
} AOS_FONT_DWORK;

// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// 構築
static void aoFontBuildTaskProcedure00(AMS_TCB* tcb);
static void aoFontBuildTaskDestructor(AMS_TCB* tcb);
static void aoFontBuildThread(void);

// 描画
static void aoFontDrawTask(AMS_TCB* tcb);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//	FNTHeader* g_ao_font_fnt_header
// ---------------------------------------------------------------------------
//!	FNTHeader
// ===========================================================================
static FNTHeader* g_ao_font_fnt_header = NULL;

// ===========================================================================
//	AMS_TCB* g_ao_font_tcb
// ---------------------------------------------------------------------------
//!	構築タスクTCBのポインタ
// ===========================================================================
static AMS_TCB* g_ao_font_tcb = NULL;

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	AoFontBuildStart
/*!
	構築開始

	@param file			[io] フォントファイル
	@param auto_free	[in] 真：構築後フォントファイル自動解放
	@param alloc_temp	[in] フォントバッファのメモリ確保方法
	@param alloc_sub	[in] サブメモリ確保
	@note
	フォントデータを使用するための構築処理を開始します。\n
	構築の完了はAoFontIsBuilded関数で判定して下さい。\n
	構築が完了するまではフォントを使用することができません。\n
	is_auto_freeにTRUEを指定すると、
	構築が完了した段階で自動でフォントファイルを解放します。\n
	is_auto_freeにFALSEを指定した場合は、
	構築の完了を待ってからファイルの解放を行って下さい。\n
	既に構築済みの場合に呼び出すとアサートし何も行いません。
	（その場合is_auto_freeがTRUEであっても
	解放処理が行われないので注意して下さい。）\n
	alloc_tempにTRUEを指定すると、
	フォントバッファの確保にamMemAllocTempを使用します。\n
	alloc_tempにFALSEを指定すると、
	フォントバッファの確保にamMemAllocを使用します。\n
*/
// ===========================================================================
void AoFontBuildStart(
	void* file, BOOL is_auto_free, BOOL alloc_temp, BOOL alloc_sub)
{
	if (g_ao_font_fnt_header || g_ao_font_tcb) {
		amAssert(0);
		return;
	}

	// 構築タスク作成
	g_ao_font_tcb = amTaskMake(
		aoFontBuildTaskProcedure00, aoFontBuildTaskDestructor,
		0, 0, 0, "aoFont::Build");

	// ワーク初期化
	AOS_FONT_WORK* work = (AOS_FONT_WORK*)amTaskGetWork(g_ao_font_tcb);
	work->file = file;
	work->file_free = is_auto_free;
	work->buf_size = FNTGetDataSize(file);
	if (alloc_temp) {
		if (alloc_sub) {
			work->buf = (FNTHeader*)amMemAllocTempHeap(work->buf_size, 1);
		}
		else {
			work->buf = (FNTHeader*)amMemAllocTemp(work->buf_size);
		}
	}
	else {
		if (alloc_sub) {
			work->buf = (FNTHeader*)amMemAllocHeap(work->buf_size, 1);
		}
		else {
			work->buf = (FNTHeader*)amMemAlloc(work->buf_size);
		}
	}

	// 構築スレッド作成
	work->th = (AMS_THREAD*)amMemAlloc(sizeof(AMS_THREAD));
	OSThread* th = OSGetCurrentThread();
	OSPriority prio = OSGetThreadPriority(th);
	prio += 1;
	amThreadCreate(
		work->th, (void*)aoFontBuildThread, NULL,
		(AMD_CORE)0, prio, 0x1000, "aoFont::Build");

	// スレッド開始
	amThreadOpen(work->th);

	// タスク開始
	amTaskStart(g_ao_font_tcb);
}

// ===========================================================================
//	AoFontIsBuilded
/*!
	構築完了判定

	@return 真：構築完了　偽：構築中
	@note
	AoFontBuildStart関数で開始した構築処理が完了したかどうかを判定します。\n
*/
// ===========================================================================
BOOL AoFontIsBuilded(void)
{
	if (g_ao_font_fnt_header && !g_ao_font_tcb) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//	AoFontRelease
/*!
	解放

	@note
	内部リソースを全て解放し、
	AoFontBuildStart関数呼び出し以前の状態に戻します。\n
	構築処理中であっても呼び出し可能です。\n
	AoFontBuildStart関数のis_auto_free引数にTRUEを指定して構築を開始した場合、
	構築が完了する前にこの関数を呼び出すと、ファイルの自動解放を行います。\n
	この関数は完了復帰です。\n
*/
// ===========================================================================
void AoFontRelease(void)
{
	// タスク実行中ならタスク終了
	if (g_ao_font_tcb) {
		amTaskDelete(g_ao_font_tcb);
		g_ao_font_tcb = NULL;
	}

	// フォントバッファ解放
	if (g_ao_font_fnt_header) {
		amMemFree(g_ao_font_fnt_header);
		g_ao_font_fnt_header = NULL;
	}
}

// ===========================================================================
//	AoFontGetFNTHeader
/*!
	FNTHeader取得

	@return FNTHeader構造体のポインタ
	@note
	モジュール内部で保持しているFNTHeaderのポインタを返します。\n
	ユーザ側でFNTHeaderの解放を行わないように注意して下さい。\n
*/
// ===========================================================================
FNTHeader* AoFontGetFNTHeader(void)
{
	amAssert(g_ao_font_fnt_header);
	return g_ao_font_fnt_header;
}

// ===========================================================================
//	AoFontDrawString
/*!
	フォント(文字列)描画

	@param x		[in] 描画X位置
	@param y		[in] 描画Y位置
	@param align	[in] 行揃えタイプ
	@param color	[in] 文字色(RGBA)
	@param scale	[in] 拡大率
	@param corw		[in] ワイド補正タイプ(AOE_ACT_CORW)
	@param str		[in] 描画文字列
	@param cdiff	[in] 次文字位置のオフセット
	@param ldiff	[in] 次行位置のオフセット
	@note
	指定パラメータで指定文字列を描画します。\n
	描画位置は、行揃えの方法により変わります。\n
	それぞれの行揃えタイプの基点位置（左揃えなら左端）が
	引数で指定する描画位置になります。\n
	次文字へのオフセットは、文字のグリフ幅にcdiffを加算した値となります。\n
	改行文字が含まれる場合は、自動で改行し、
	文字の高さ分だけY位置をずらした位置にldiffを加算した位置を
	次行のY位置として描画します。\n
	文字列データは描画タスクが生存している間は保持するようにしてください。\n
	この関数は描画タスクから呼び出すようにして下さい。\n
*/
// ===========================================================================
void AoFontDrawString(
	f32 x, f32 y, AOE_FONT_ALIGN align, u32 color, f32 scale,
	f32 cdiff, f32 ldiff,
	u32 corw, const char* str)
{
	// フォント取得
	FNTHeader* font = g_ao_font_fnt_header;
	if (font == NULL) {
		return;
	}

	// エンコード方法設定
	FNTSetEncoding(font, FNT_ENCODING_UTF8);

	// 描画ステート初期化
	amDrawPushState();
	amDrawInitState();

	// 射影設定
	{
		Mtx44 m;
		f32 r;
		if (_am_draw_video.wide_screen) {
			r = (f32)((AOD_ACT_SCREEN_WIDTH * 16) / 12);
		}
		else {
			r = (f32)AOD_ACT_SCREEN_WIDTH;
		}
		MTXOrtho(m, 0.0f, (f32)AOD_ACT_SCREEN_HEIGHT, 0.0f, r, 1.0f, 3.0f);
		GXSetProjection(m, GX_ORTHOGRAPHIC);
	}

	// 変換行列設定
	{
		Mtx m;
		MTXIdentity(m);
		GXLoadPosMtxImm(m, GX_PNMTX0);
		GXSetCurrentMtx(GX_PNMTX0);
	}

	// 描画設定
	nnSetZModeGC(GX_FALSE, GX_ALWAYS, GX_FALSE);
	nnSetAlphaCompareGC(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	nnSetBlendModeGC(
		GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	nnSetCullModeGC(GX_CULL_NONE);

    GXSetNumChans(1);
    GXSetChanCtrl(
		GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
		GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(
		GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
		GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(
		GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(
		GX_TEVSTAGE0, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(
		GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevColorOp(
		GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(
		GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);

	const u32 TONE_SHIFT = 15;
	const u32 TONE = (1 << 15);

	// 頂点フォーマット設定
	GXSetVtxAttrFmt(GX_VTXFMT5, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT5, GX_VA_TEX0, GX_TEX_ST, GX_U16, TONE_SHIFT);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS , GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXColor c;
	c.r = (u8)((color >> 24) & 0xff);
	c.g = (u8)((color >> 16) & 0xff);
	c.b = (u8)((color >>  8) & 0xff);
	c.a = (u8)((color >>  0) & 0xff);
	GXSetChanMatColor(GX_COLOR0A0, c);

	const char* line_str = str;

	// 行ごとに処理
	f32 pos_y = y;
	void* tex_p = NULL;
	while (1) {
		s32 height = 0;
		const char* s;
		const char* s_end;
		FNTTexture tex;

		// 行の長さと文字数を算出
		u32 c_num = 0;
		f32 len = 0.0f;
		s = line_str;
		while ((*s != '\n') && (*s != '\0')) {
			s = FNTGetTexture(font, s, &tex);
			if (tex.glyphWidth == 0) {
				tex.glyphWidth = AOD_FONT_SPACE_WIDTH;
			}
			len += ((f32)tex.glyphWidth * scale) + cdiff;
			if (tex.charHeight > height) {
				height = tex.charHeight;
			}
			c_num += 1;
		}
		if (c_num > 0) {
			len -= cdiff;
		}
		s_end = s;

		// 描画開始位置算出
		f32 pos_x;
		switch (align) {
		case AOD_FONT_ALIGN_LEFT:
			pos_x = x;
			break;
		case AOD_FONT_ALIGN_CENTER:
			pos_x = (f32)x - (f32)(len * 0.5f);
			break;
		case AOD_FONT_ALIGN_RIGHT:
			pos_x = (f32)x - len;
			break;
		default:
			pos_x = (f32)x;
			break;
		}

		// 描画
		s = line_str;
		for (u32 i = 0; i < c_num; ++i) {
			s = FNTGetTexture(font, s, &tex);
			if (tex.glyphWidth == 0) {
				pos_x += (f32)AOD_FONT_SPACE_WIDTH * scale;
				continue;
			}

			f32 font_w = (f32)tex.glyphWidth * scale;
			f32 font_h = (f32)tex.charHeight * scale;

			// テクスチャロード
			if (tex.image != tex_p) {
				GXTexObj tex_obj;
				GXInitTexObj(
					&tex_obj, tex.image, tex.texWidth, tex.texHeight,
					tex.texFormat, GX_CLAMP, GX_CLAMP, GX_FALSE);
				GXInitTexObjLOD(
					&tex_obj, GX_LINEAR, GX_LINEAR, 0, 0, 0,
					GX_DISABLE, GX_DISABLE, GX_ANISO_1);
				GXLoadTexObj(&tex_obj, GX_TEXMAP0);
				tex_p = tex.image;
			}

			// テクスチャ座標作成
			u16 tex_l = (u16)(tex.cellX * TONE / tex.texWidth);
			u16 tex_t = (u16)(tex.cellY * TONE / tex.texHeight);
			u16 tex_r = (u16)(
				(tex.cellX + tex.glyphWidth) * TONE / tex.texWidth);
			u16 tex_b = (u16)(
				(tex.cellY + tex.charHeight) * TONE / tex.texHeight);

			// 頂点データ作成
			NNS_PRIM3D_P v[4];
			v[0].Pos.x = v[3].Pos.x = pos_x;
			v[1].Pos.x = v[2].Pos.x = pos_x + font_w;
			v[0].Pos.y = v[1].Pos.y = pos_y;
			v[2].Pos.y = v[3].Pos.y = pos_y + font_h;
			v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.0f;

			// ワイド補正
			AoActDrawCorWide(v, 4, (AOE_ACT_CORW)corw);

			// 描画
			GXBegin(GX_QUADS, GX_VTXFMT5, 4);
			GXPosition3f32(v[0].Pos.x, v[0].Pos.y, v[0].Pos.z);
			GXTexCoord2u16(tex_l, tex_t);
			GXPosition3f32(v[1].Pos.x, v[1].Pos.y, v[1].Pos.z);
			GXTexCoord2u16(tex_r, tex_t);
			GXPosition3f32(v[2].Pos.x, v[2].Pos.y, v[2].Pos.z);
			GXTexCoord2u16(tex_r, tex_b);
			GXPosition3f32(v[3].Pos.x, v[3].Pos.y, v[3].Pos.z);
			GXTexCoord2u16(tex_l, tex_b);
			GXEnd();

			// 次位置
			pos_x += font_w + cdiff;
		}

		if (*s_end == '\0') {
			break;
		}

		line_str = s_end + 1;
		pos_y += (s32)((f32)height * scale) + ldiff;
	}

	nnSetZModeGC(GX_TRUE, GX_LEQUAL, GX_TRUE);

	// 描画ステート復帰
	amDrawPopState();

	// GX設定(for NN)
	nnSetGXDefaultVertex();
	nnResetRegModeFlagGC();
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetColorUpdate(GX_TRUE);
}

// ===========================================================================
//	AoFontDrawStringTask
/*!
	フォント(文字列)描画を行う描画タスク作成

	@param prio		[in] 描画タスク優先度
	@param x		[in] 描画X位置
	@param y		[in] 描画Y位置
	@param align	[in] 行揃えタイプ
	@param color	[in] 文字色(RGBA)
	@param scale	[in] 拡大率
	@param cdiff	[in] 次文字位置のオフセット
	@param ldiff	[in] 次行位置のオフセット
	@param corw		[in] ワイド補正タイプ(AOE_ACT_CORW)
	@param str		[in] 描画文字列
	@note
	AoFontDrawString関数を呼び出す描画タスクを内部で作成します。\n
	引数の意味はAoFontDrawString関数と同じとなります。\n
*/
// ===========================================================================
void AoFontDrawStringTask(
	u16 prio,
	f32 x, f32 y, AOE_FONT_ALIGN align, u32 color, f32 scale,
	f32 cdiff, f32 ldiff,
	u32 corw, const char* str)
{
	// 描画タスク用ワーク確保
	AOS_FONT_DWORK* work =
		(AOS_FONT_DWORK*)amDrawMallocDataBuffer(sizeof(AOS_FONT_DWORK));

	// 描画タスク用ワーク作成
	work->x = x;
	work->y = y;
	work->align = align;
	work->color = color;
	work->scale = scale;
	work->cdiff = cdiff;
	work->ldiff = ldiff;
	work->corw = corw;
	work->str = str;

	// 描画タスク作成
	amDrawMakeTask(aoFontDrawTask, prio, (u32)work);
}

// ===========================================================================
//	AoFontUtilGetStringInfo
/*!
	文字列情報取得

	@param str		[in]  文字列
	@param scale	[in]  拡大率
	@param cdiff	[in]  次文字位置のオフセット
	@param ldiff	[in]  次行位置のオフセット
	@param str_w	[out] 文字列横サイズ(NULL可)
	@param str_h	[out] 文字列縦サイズ(NULL可)
	@param char_num	[out] 文字数(NULL可)
	@param line_num	[out] 行数(NULL可)
*/
// ===========================================================================
void AoFontUtilGetStringInfo(
	const char* str, f32 scale, f32 cdiff, f32 ldiff,
	f32* str_w, f32* str_h, u32* char_num, u32* line_num)
{
	// フォント取得
	FNTHeader* font = g_ao_font_fnt_header;
	if (font == NULL) {
		if (str_w) {
			*str_w = 1;
		}
		if (str_h) {
			*str_h = 1;
		}
		if (char_num) {
			*char_num = 1;
		}
		if (line_num) {
			*line_num = 1;
		}
		return;
	}

	// エンコード方法設定
	FNTSetEncoding(font, FNT_ENCODING_UTF8);

	f32 max_w = 0.0f;
	f32 h = 0.0f;
	u32 chars = 0;
	u32 line = 1;

	f32 w = 0.0f;
	f32 max_h = 0.0f;
	const char* s = str;
	while (*s != '\0') {
		if (*s == '\n') {
			if (w >= cdiff) {
				w -= cdiff;
			}
			if (w > max_w) {
				max_w = w;
			}
			w = 0.0f;

			h += max_h;
			max_h = 0.0f;

			line += 1;
			s += 1;
		}
		else {
			FNTTexture info;
			s = FNTGetTexture(font, s, &info);
			if (info.glyphWidth == 0) {
				info.glyphWidth = AOD_FONT_SPACE_WIDTH;
			}
			w += ((f32)info.glyphWidth * scale) + cdiff;
			f32 temp = (f32)info.charHeight * scale;
			if (temp > max_h) {
				max_h = temp;
			}
			chars += 1;
		}
	}
	{
		if (w >= cdiff) {
			w -= cdiff;
		}
		if (w > max_w) {
			max_w = w;
		}
		h += max_h;
	}
	if (line > 0) {
		h += ldiff * (f32)(line - 1);
	}

	if (str_w) {
		*str_w = max_w;
	}
	if (str_h) {
		*str_h = h;
	}
	if (char_num) {
		*char_num = chars;
	}
	if (line_num) {
		*line_num = line;
	}
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

// ***************************************************************************
// 構築
// ***************************************************************************
// ===========================================================================
//! 構築タスクプロシージャ00
// ===========================================================================
void aoFontBuildTaskProcedure00(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_FONT_WORK* work = (AOS_FONT_WORK*)amTaskGetWork(tcb);

	// スレッド終了判定
	if (amThreadCheckQuit(work->th)) {

		// スレッド削除
		amThreadDelete(work->th);
		amMemFree(work->th);
		work->th = NULL;

		// フォントバッファ設定
		g_ao_font_fnt_header = work->buf;

		// タスク終了
		amTaskDelete(tcb);
		g_ao_font_tcb = NULL;
	}
}

// ===========================================================================
//! 構築タスクデストラクタ
// ===========================================================================
void aoFontBuildTaskDestructor(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_FONT_WORK* work = (AOS_FONT_WORK*)amTaskGetWork(tcb);

	// スレッド実行中なら終了を待つ
	if (work->th) {
		amThreadExit(work->th);
		amThreadWaitQuit(work->th);
		amThreadDelete(work->th);
		amMemFree(work->th);
		work->th = NULL;
	}

	// フォントファイル自動解放
	if (work->file_free) {
		if (work->file) {
			amMemFree(work->file);
			work->file = NULL;
		}
	}

	// TCBクリア
	g_ao_font_tcb = NULL;
}

// ===========================================================================
//! 構築スレッド
// ===========================================================================
void aoFontBuildThread(void)
{
	// ワーク取得
	AOS_FONT_WORK* work = (AOS_FONT_WORK*)amTaskGetWork(g_ao_font_tcb);

	// フォント展開
	FNTConstruct(work->buf, work->file);

	// 終了
	amThreadQuit(work->th);
}

// ***************************************************************************
// 描画
// ***************************************************************************
// ===========================================================================
//! 描画タスク
// ===========================================================================
void aoFontDrawTask(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_FONT_DWORK* work = *((AOS_FONT_DWORK**)amTaskGetWork(tcb));

	// 描画
	AoFontDrawString(
		work->x, work->y, work->align, work->color, work->scale,
		work->cdiff, work->ldiff, work->corw, work->str);
}

#endif // defined(AOD_PLATFORM_WII)

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
