// ===========================================================================
/*!
	@file	gsStrapImage.cpp
	@brief	Wiiストラップ画面モジュール定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "gsStrapImage.h"

#include "ao.h"
#include "gs.h"
#include "izFade.h"

#if defined(MTD_DEBUG)
#include "gsMainSys.h"
#endif // defined(MTD_DEBUG)

#if _WII

#include <revolution/cx.h>

#endif // _WII

// ----- Macros ------------------------------------------------（マクロ定義）

#if defined(_DLC)
#define GSD_STRAP_USE_CNT	(1)	//!< CNT使用(無効にする場合はコメントアウト)
#endif // defined(_DLC)

#if defined(GSD_STRAP_USE_CNT)
#define GSD_STRAP_CNT_HANDLE_ID	(AMD_FS_CNT_USER + 2)		//!< CNTハンドルID
#define GSD_STRAP_CNT_ID			(AMD_FS_CONTENTS_USER + 2)	//!< CNTID
#endif // defined(GSD_STRAP_USE_CNT)

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）

namespace gs {
namespace strap {

#if _WII

// ===========================================================================
//! ストラップ画像ファイル読み込みクラス
// ===========================================================================
class CLoad : public ao::CTask<CLoad>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CLoad(GSS_STRAP_IMAGE& work);

protected:

	//! デストラクタ
	virtual ~CLoad();

	//! 読み込み待ちタスクプロシージャ
	void TaskProcWait();

	//! ワーク
	GSS_STRAP_IMAGE& m_work;

#if !defined(GSD_STRAP_USE_CNT)
	//! ファイル読み込み用
	AMS_FS* m_fs;
#endif // !defined(GSD_STRAP_USE_CNT)
};

// ===========================================================================
//! ストラップ画像表示クラス
// ===========================================================================
class CShow :
	public ao::CProc<CShow>, public ao::CTask<CShow>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CShow(GSS_STRAP_IMAGE& work);

protected:

	//! デストラクタ
	virtual ~CShow();

	//! タスクプロシージャ
	void TaskProcMain();

	//! フェードイン
	void ProcFadeIn();

	//! 入力無効期間
	void ProcInputDisable();

	//! 入力待ち期間
	void ProcInputWait();

	//! フェードアウト
	void ProcFadeOut();

#if defined(HOG_RGN_US) && !defined(HOG_RGN_KR)
	//! フェードアウト2
	void ProcFadeOut2();
#endif // defined(HOG_RGN_US) && !defined(HOG_RGN_KR)

	//! 終了待機
	void ProcEndWait();

	//! ストラップ画面描画タスク
	static void DrawTask(AMS_TCB* tcb);

	//! ワーク
	GSS_STRAP_IMAGE& m_work;
};

#endif // _WII

#if defined(MTD_DEBUG)

namespace debug {

// ===========================================================================
//! デバッグ用イベントクラス
// ===========================================================================
class CMain :
	public ao::CProc<CMain>, public ao::CTask<CMain>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CMain();

protected:

	//! デストラクタ
	virtual ~CMain();

	//! タスクプロシージャ
	void TaskProcMain();

#if _WII

	//! ロード
	void ProcLoad();

	//! ユーザ入力
	void ProcInput();

	//! 表示
	void ProcShow();

	//! ワーク
	GSS_STRAP_IMAGE m_work;

#endif // _WII
};

} // namespace debug

#endif // defined(MTD_DEBUG)

} // namespace strap
} // namespace gs

#if _WII

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

// ===========================================================================
//! ストラップ画像ファイル名(JP)
// ===========================================================================
static const char* g_gs_strap_file_name_jp = "strapImage_jp_LZ.bin";

// ===========================================================================
//! ストラップ画像ファイル名(EN)
// ===========================================================================
static const char* g_gs_strap_file_name_en = "strapImage_En_LZ.bin";

// ===========================================================================
//! ストラップ画像ファイル名(FR)
// ===========================================================================
static const char* g_gs_strap_file_name_fr = "strapImage_Fr_LZ.bin";

// ===========================================================================
//! ストラップ画像ファイル名(IT)
// ===========================================================================
static const char* g_gs_strap_file_name_it = "strapImage_It_LZ.bin";

// ===========================================================================
//! ストラップ画像ファイル名(GE)
// ===========================================================================
static const char* g_gs_strap_file_name_ge = "strapImage_Ge_LZ.bin";

// ===========================================================================
//! ストラップ画像ファイル名(SP)
// ===========================================================================
static const char* g_gs_strap_file_name_sp = "strapImage_Sp_LZ.bin";

// ===========================================================================
//! ストラップ画像ファイル名(DU)
// ===========================================================================
static const char* g_gs_strap_file_name_du = "strapImage_Du_LZ.bin";

// ===========================================================================
//! ストラップ画像ファイル名配列
// ===========================================================================
static const char* g_gs_strap_file_name_tbl[GSD_WII_LANGUAGE_NUM] = {
	g_gs_strap_file_name_jp,
	g_gs_strap_file_name_en,
	g_gs_strap_file_name_fr,
	g_gs_strap_file_name_it,
	g_gs_strap_file_name_ge,
	g_gs_strap_file_name_sp,
	g_gs_strap_file_name_du,
};

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! 初期化処理(アプリケーション起動時に一度だけ呼び出し)
// ===========================================================================
void GsStrapImageInit(void)
{
#if defined(GSD_STRAP_USE_CNT)
	amFsOpenCNT(GSD_STRAP_CNT_HANDLE_ID, GSD_STRAP_CNT_ID);
#endif // defined(GSD_STRAP_USE_CNT)
}

// ===========================================================================
//! 終了処理(アプリケーション終了時に一度だけ呼び出し)
// ===========================================================================
void GsStrapImageExit(void)
{
#if defined(GSD_STRAP_USE_CNT)
	amFsCloseCNT(GSD_STRAP_CNT_HANDLE_ID);
#endif // defined(GSD_STRAP_USE_CNT)
}

// ===========================================================================
//! ワーク初期化
// ===========================================================================
void GsStrapImageInitWork(GSS_STRAP_IMAGE& work)
{
	amZeroMemory(&work, sizeof(GSS_STRAP_IMAGE));
}

// ===========================================================================
//! ファイル読み込み開始
// ===========================================================================
void GsStrapImageLoadStart(GSS_STRAP_IMAGE& work)
{
	new gs::strap::CLoad(work);
}

// ===========================================================================
//! ファイル読み込み終了判定
// ===========================================================================
BOOL GsStrapImageLoadIsFinished(const GSS_STRAP_IMAGE& work)
{
	if (work.file) {
		return TRUE;
	}
	return FALSE;
}

// ===========================================================================
//! 表示開始
// ===========================================================================
void GsStrapImageShowStart(GSS_STRAP_IMAGE& work)
{
	new gs::strap::CShow(work);
}

// ===========================================================================
//! 表示終了判定
// ===========================================================================
BOOL GsStrapImageShowIsFinished(const GSS_STRAP_IMAGE& work)
{
	if (work.tcb) {
		return FALSE;
	}
	return TRUE;
}

// ===========================================================================
//! ワーク解放
// ===========================================================================
void GsStrapImageExitWork(GSS_STRAP_IMAGE& work)
{
	if (work.tcb) {
		amTaskDelete(work.tcb);
		work.tcb = NULL;
	}
	if (work.file) {
		amMemFree(work.file);
		work.file = NULL;
	}
	GsStrapImageInitWork(work);
}

#endif // _WII

#if defined(MTD_DEBUG)
// ===========================================================================
//! デバッグ用イベント
// ===========================================================================
void GsStrapImageDebugEvent(void* arg)
{
	UNREFERENCED_PARAMETER(arg);
	new gs::strap::debug::CMain;

}
#endif // defined(MTD_DEBUG)

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace gs {
namespace strap {

#if _WII

// ***************************************************************************
//! ストラップ画像ファイル読み込みクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CLoad::CLoad(GSS_STRAP_IMAGE& work) : m_work(work)
{
	amAssert(m_work.tcb == NULL);
	amAssert(m_work.file == NULL);

#if !defined(GSD_STRAP_USE_CNT)

	// ファイル読み込みリクエスト
	char path[256];
	sprintf(
		path, "SYS_SHARED/%s",
		g_gs_strap_file_name_tbl[GsEnvGetWiiSystemLanguage()]);
	m_fs = amFsReadBackground(path);

#endif // !defined(GSD_STRAP_USE_CNT)

	// タスク作成
	MakeTask(0, "gsStrapImage::Load");
	m_work.tcb = GetTcb(0);

	// タスクプロシージャ設定
	SetTaskProc(0, &CLoad::TaskProcWait);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CLoad::~CLoad()
{
#if !defined(GSD_STRAP_USE_CNT)
	if (m_fs) {
		amFsClearRequest(m_fs);
	}
#endif // !defined(GSD_STRAP_USE_CNT)

	m_work.tcb = NULL;
}

// ===========================================================================
//! 読み込み待ちタスクプロシージャ
// ===========================================================================
void CLoad::TaskProcWait()
{
#if defined(GSD_STRAP_USE_CNT)

	char path[256];
	void* fbuf = NULL;

	// ファイル読み込み
	strcpy(path, g_gs_strap_file_name_tbl[GsEnvGetWiiSystemLanguage()]);
	amFsSetMallocMode(AMD_FS_MALLOC_TEMP | AMD_FS_MALLOC_MEM2);
	amFsReadCNT(
		GSD_STRAP_CNT_HANDLE_ID, path, &fbuf, AMD_FS_MALLOC_COMP_LZ77);
	amFsSetMallocMode(AMD_FS_MALLOC_NORMAL);

	// 準備
	TPLBind((TPLPalettePtr)fbuf);

	m_work.file = fbuf;

	// 終了
	delete this;

#else

	// 読み込み待ち
	if (amFsIsComplete(m_fs)) {

		// ファイルバッファ取得
		void* fbuf = m_fs->buf;
		m_fs->buf = NULL;
		amFsClearRequest(m_fs);
		m_fs = NULL;

		// 展開
		u32 size = CXGetUncompressedSize(fbuf);
		void* fbuf2 = amMemAllocTemp(size);
		CXUncompressLZ(fbuf, fbuf2);
		amMemFree(fbuf);
		DCStoreRange(fbuf2, size);

		// 準備
		TPLBind((TPLPalettePtr)fbuf2);

		m_work.file = fbuf2;

		// 終了
		delete this;
	}

#endif // defined(GSD_STRAP_USE_CNT)
}


// ***************************************************************************
//! ストラップ画像表示クラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CShow::CShow(GSS_STRAP_IMAGE& work) : m_work(work)
{
	amAssert(m_work.tcb == NULL);
	amAssert(m_work.file != NULL);

	// タスク作成
	MakeTask(0, "gsStrapImage::Show");
	m_work.tcb = GetTcb(0);

	// タスクプロシージャ設定
	SetTaskProc(0, &CShow::TaskProcMain);

	// プロシージャ設定
	SetProc(0, &CShow::ProcFadeIn);

	// タスク開始
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CShow::~CShow()
{
	m_work.tcb = NULL;
}

// ===========================================================================
//! タスクプロシージャ
// ===========================================================================
void CShow::TaskProcMain()
{
	// プロシージャ確認
	if (IsProcNone(0)) {
		// 終了
		delete this;
	}
	else {

		if (GetProc(0) != &CShow::ProcEndWait) {
			// 描画タスク作成
			amDrawMakeTask(&CShow::DrawTask, 0, (u32)(&m_work));
		}

		// 実行
		Call(0);
	}
}

// ===========================================================================
//! フェードイン
// ===========================================================================
void CShow::ProcFadeIn()
{
	// フェードイン開始
	if (GetCount() == 0) {
		IzFadeInitEasyTask(
			IZE_FADE_SET_TYPE_TAKEOEVER,
			255, 255, 255, 255, 255, 255, 255, 0, 30.0f);
	}

	// フェード終了判定
	if (IzFadeIsEnd()) {
		// 遷移
		SetOwnProc(&CShow::ProcInputDisable);
	}
}

// ===========================================================================
//! 入力無効期間
// ===========================================================================
void CShow::ProcInputDisable()
{
	// 規定の時間待機する
	if (GetCount() >= (1 * 60)) {
		SetOwnProc(&CShow::ProcInputWait);
	}
}

// ===========================================================================
//! 入力待ち期間
// ===========================================================================
void CShow::ProcInputWait()
{
	// ユーザ入力か規定の時間経過で次へ遷移
	if (GetCount() >= ((20 - 1) * 60)) {
		SetOwnProc(&CShow::ProcFadeOut);
	}
	else {
		u32 input = 0;
		for (u32 i = 0; i < AMD_PAD_PORT_MAX; ++i) {
			if (!PAD_CONNECT(i)) {
				continue;
			}
			input |= PAD_STAND(i);
		}
		const u32 mask =
			KEY_L_UP | KEY_L_DOWN | KEY_L_LEFT | KEY_L_RIGHT |
			KEY_R_UP | KEY_R_DOWN | KEY_R_LEFT | KEY_R_RIGHT |
			KEY_SELECT | KEY_START;
		if (input & mask) {
			SetOwnProc(&CShow::ProcFadeOut);
		}
	}
}

// ===========================================================================
//! フェードアウト
// ===========================================================================
void CShow::ProcFadeOut()
{
	// フェードアウト開始
	if (GetCount() == 0) {
		IzFadeInitEasyTask(
			IZE_FADE_SET_TYPE_TAKEOEVER,
			255, 255, 255, 0, 255, 255, 255, 255, 30.0f);
	}

	// フェード終了判定
	if (IzFadeIsEnd()) {
		// 遷移
#if defined(HOG_RGN_US) && !defined(HOG_RGN_KR)
		SetOwnProc(&CShow::ProcFadeOut2);
#else
		SetOwnProc(&CShow::ProcEndWait);
#endif // defined(HOG_RGN_US) && !defined(HOG_RGN_KR)
	}
}

#if defined(HOG_RGN_US) && !defined(HOG_RGN_KR)
// ===========================================================================
//! フェードアウト
// ===========================================================================
void CShow::ProcFadeOut2()
{
	// 白→黒フェード開始
	if (GetCount() == 0) {
		IzFadeInitEasyTask(
			IZE_FADE_SET_TYPE_TAKEOEVER,
			255, 255, 255, 0, 0, 0, 0, 255, 30.0f);
	}

	// フェード終了判定
	if (IzFadeIsEnd()) {
		// 遷移
		SetOwnProc(&CShow::ProcEndWait);
	}
}
#endif // defined(HOG_RGN_US) && !defined(HOG_RGN_KR)

// ===========================================================================
//! 終了待機
// ===========================================================================
void CShow::ProcEndWait()
{
	// 一定時間待機
	if (GetCount() >= 4) {
		SetOwnProcNone();
	}
}

// ===========================================================================
//! ストラップ画面描画タスク
// ===========================================================================
void CShow::DrawTask(AMS_TCB* tcb)
{
	// 外枠を白で塗りつぶし
	amDrawPushState();
	amDrawInitState();
	AoActDrawPre();
	nnSetPrimitive3DAlphaCompareGC(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	nnSetPrimitive3DZModeGC(GX_FALSE, GX_ALWAYS, GX_FALSE);
			nnSetPrimitive3DBlendModeGC(
				GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	amDrawSetFog(0);
	nnSetPrimitiveTexNum(NULL, -1);
	nnBeginDrawPrimitive3D(
		NNE_PRIM3D_FMT_PC,
		NNE_PRIM_ALPHABLEND_ON,
		NNE_PRIM_LIGHT_DISABLE,
		NNE_PRIM_CULL_NONE);
	NNS_PRIM3D_PC vv[6];
	vv[0].Col = vv[1].Col = vv[2].Col = vv[5].Col = 0xffffffff;
	vv[0].Pos.x = vv[1].Pos.x = 0.0f;
	vv[2].Pos.x = vv[5].Pos.x = (f32)AOD_ACT_SCREEN_WIDTH;
	vv[0].Pos.y = vv[2].Pos.y = 0.0f;
	vv[1].Pos.y = vv[5].Pos.y = (f32)AOD_ACT_SCREEN_HEIGHT;
	vv[0].Pos.z = vv[1].Pos.z = vv[2].Pos.z = vv[5].Pos.z = -2.0f;
	vv[3] = vv[1];
	vv[4] = vv[2];
	AoActDrawCorWide(vv, 6, AOD_ACT_CORW_NONE);
	nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_LIST, vv, 6);
	nnEndDrawPrimitive3D();
	amDrawPopState();
	amDrawEndScene();

	// 画像ファイル取得
	void* file = (*((GSS_STRAP_IMAGE**)amTaskGetWork(tcb)))->file;

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
		GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
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
	GXSetTevOp(GX_TEVSTAGE0, GX_DECAL);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
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
	c.r = 255;
	c.g = 255;
	c.b = 255;
	c.a = 255;
	GXSetChanMatColor(GX_COLOR0A0, c);

	// テクスチャロード
	{
		TPLDescriptorPtr tdp;
		GXBool mipMapFlag;
		u32 fmt;
		u32 tex_w;
		u32 tex_h;
		GXTexObj tex_obj;

		tdp = TPLGet((TPLPalettePtr)file, 0);
		if (tdp->textureHeader->minLOD == tdp->textureHeader->maxLOD) {
			mipMapFlag = GX_FALSE;
		}
		else {
			mipMapFlag = GX_TRUE;
		}
		fmt = (u32)tdp->textureHeader->format;
		tex_w = tdp->textureHeader->width;
		tex_h = tdp->textureHeader->height;

		GXInitTexObj(
			&tex_obj,
			tdp->textureHeader->data,
			tdp->textureHeader->width,
			tdp->textureHeader->height,
			(GXTexFmt)fmt,
			tdp->textureHeader->wrapS,
			tdp->textureHeader->wrapT,
			mipMapFlag);

		GXInitTexObjLOD(
			&tex_obj,
			tdp->textureHeader->minFilter,
			tdp->textureHeader->magFilter,
			tdp->textureHeader->minLOD,
			tdp->textureHeader->maxLOD,
			tdp->textureHeader->LODBias,
			GX_FALSE,
			tdp->textureHeader->edgeLODEnable,
			GX_ANISO_1);

		GXLoadTexObj(&tex_obj, GX_TEXMAP0);
	}

	// 頂点データ作成
	NNS_PRIM3D_P v[4];
	v[0].Pos.x = v[3].Pos.x = 0.0f;
	v[1].Pos.x = v[2].Pos.x = (f32)AOD_ACT_SCREEN_WIDTH;
	v[0].Pos.y = v[1].Pos.y = 0.0f;
	v[2].Pos.y = v[3].Pos.y = (f32)AOD_ACT_SCREEN_HEIGHT;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.0f;

	// ワイド補正
	AoActDrawCorWide(v, 4, AOD_ACT_CORW_CENTER);

	// 描画
	GXBegin(GX_QUADS, GX_VTXFMT5, 4);
	GXPosition3f32(v[0].Pos.x, v[0].Pos.y, v[0].Pos.z);
	GXTexCoord2u16(0, 0);
	GXPosition3f32(v[1].Pos.x, v[1].Pos.y, v[1].Pos.z);
	GXTexCoord2u16(TONE, 0);
	GXPosition3f32(v[2].Pos.x, v[2].Pos.y, v[2].Pos.z);
	GXTexCoord2u16(TONE, TONE);
	GXPosition3f32(v[3].Pos.x, v[3].Pos.y, v[3].Pos.z);
	GXTexCoord2u16(0, TONE);
	GXEnd();

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

#endif // _WII

#if defined(MTD_DEBUG)

namespace debug {

// ***************************************************************************
// デバッグ用イベントクラス
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CMain::CMain()
{
	MakeTask(0, "gsStrapImage::Debug");
	SetTaskProc(0, &CMain::TaskProcMain);
#if _WII
	GsStrapImageInitWork(m_work);
	SetProc(0, &CMain::ProcLoad);
#else
	SetProcNone(0);
#endif // _WII
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CMain::~CMain()
{
#if _WII
	GsStrapImageExitWork(m_work);
#endif // _WII

	// デバッグランチャーに戻る
	SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
	SyChangeNextEvt();
}

// ===========================================================================
//! タスクプロシージャ
// ===========================================================================
void CMain::TaskProcMain()
{
	if (IsProcNone(0)) {
		delete this;
	}
	else {
		Call(0);
	}
}

#if _WII

// ===========================================================================
//! ロード
// ===========================================================================
void CMain::ProcLoad()
{
	amPrint(4, 4, "NOW LOADING...");

	if (GetCount() == 0) {
		GsStrapImageLoadStart(m_work);
	}

	if (GsStrapImageLoadIsFinished(m_work)) {
		SetOwnProc(&CMain::ProcInput);
	}
}

// ===========================================================================
//! ユーザ入力
// ===========================================================================
void CMain::ProcInput()
{
	amPrint(4, 4, "PLEASE PUSH ANY KEY.");
	amPrintf(4, 5, "%c:START", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		GsStrapImageShowStart(m_work);
		SetOwnProc(&CMain::ProcShow);
	}
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		SetOwnProcNone();
	}
}

// ===========================================================================
//! 表示
// ===========================================================================
void CMain::ProcShow()
{
	if (GsStrapImageShowIsFinished(m_work)) {
		SetOwnProc(&CMain::ProcInput);
	}
}

#endif // _WII

} // namespace debug

#endif // defined(MTD_DEBUG)

} // namespace strap
} // namespace gs

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
