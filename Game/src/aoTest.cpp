// ===========================================================================
/*!
	@file	aoTest.cpp
	@brief	テスト定義

	@author	K.OKUGAWA Copyright (C) 2009 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#if defined (MTD_DEBUG)

#include "aoTest.h"
#include "ao.h"
#include "gs.h"
#include "gsMainSys.h"

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）

// ===========================================================================
//! セーフフレーム描画タスクワーク
// ===========================================================================
typedef struct tag_AOS_TEST_DRAW_SF {
	u32		rgba;		//!< フレーム色
	BOOL	fill;		//!< 塗りつぶしフラグ
} AOS_TEST_DRAW_SF;

// ----- Class Definitions -------------------------------------（クラス宣言）

namespace ao {
namespace test {
namespace safeframe {

// ===========================================================================
//! 設定
// ===========================================================================
class CSetting : public ao::CTask<CSetting>, public ao::CAllocAmNormal
{
public:

	//! コンストラクタ
	CSetting();

protected:

	//! デストラクタ
	virtual ~CSetting();

	//! 選択
	void TaskProcSelect();

	//! 選択番号
	u32 m_select;
};

// ===========================================================================
//! 実行
// ===========================================================================
class CExecute : public ao::CTask<CExecute>, public ao::CAllocAmNormal
{
public:

	enum {
		// タイプ
		TYPE_BLACK	= 0,	//!< 黒
		TYPE_WHITE,			//!< 白
		TYPE_YELLOW,		//!< イエロー
		TYPE_MAGENTA,		//!< マゼンダ
		TYPE_CYAN,			//!< シアン
		TYPE_BLUE,			//!< 青
		TYPE_GREEN,			//!< 緑
		TYPE_RED,			//!< 赤

		TYPE_NUM,			//!< タイプ数
	};

	//! コンストラクタ
	CExecute(u32 type);

	//! デストラクタ
	virtual ~CExecute();

	//! タイプ変更
	void SetType(u32 type);

	//! タイプの色取得
	static u32 GetTypeColor(u32 type);

protected:

	//! タスク
	void TaskProcMain();

	//! タイプ
	u32 m_type;
};

} // namespace safeframe
} // namespace test
} // namespace ao

// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// ===========================================================================
//! セーフフレーム実行クラス
// ===========================================================================
static ao::test::safeframe::CExecute* g_ao_test_safeframe_execute = NULL;

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）

static void aoTestDrawSafeFrameTask(AMS_TCB* tcb);

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//! セーフフレーム表示イベント
// ===========================================================================
void AoTestSafeFrameStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);

	new ao::test::safeframe::CSetting;
}

// ===========================================================================
//! セーフフレーム描画(内部で描画タスク生成)
// ===========================================================================
void AoTestDrawSafeFrame(u32 prio, u32 rgba, BOOL fill)
{
	// 描画タスク生成
	AOS_TEST_DRAW_SF* work =
		(AOS_TEST_DRAW_SF*)amDrawMallocDataBuffer(sizeof(AOS_TEST_DRAW_SF));
	work->rgba = rgba;
	work->fill = fill;
	amDrawMakeTask(aoTestDrawSafeFrameTask, (u16)prio, (u32)work);
}

// ----- Static Functions --------------------（スタティック関数の定義：局所）

namespace ao {
namespace test {
namespace safeframe {

// ***************************************************************************
// 設定
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CSetting::CSetting() : m_select(0)
{
	// 既存の実行削除
	if (g_ao_test_safeframe_execute) {
		delete g_ao_test_safeframe_execute;
		g_ao_test_safeframe_execute = NULL;
	}

	MakeTask(0, "aoTestSafeframe::Setting");
	SetTaskProc(0, &CSetting::TaskProcSelect);
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CSetting::~CSetting()
{
	// デバッグランチャーに戻る
	SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
	SyChangeNextEvt();
}

// ===========================================================================
//! 選択
// ===========================================================================
void CSetting::TaskProcSelect()
{
	// 表題
	amPrintf(4, 4, "PLEASE SELECT SAFEFRAME TYPE.");
	amPrintf(4, 5, "%c:DECIDE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:CANCEL", GsEnvDebugGetCancelKeyChar());

	// 選択項目数
	const u32 sel_num = (u32)(CExecute::TYPE_NUM + 1);

	// 選択切り替え
	if (AoPadSomeoneMRepeat(GSD_KEY_UP) >= 0) {
		m_select = (u32)((m_select + (sel_num - 1)) % sel_num);
	}
	if (AoPadSomeoneMRepeat(GSD_KEY_DOWN) >= 0) {
		m_select = (u32)((m_select + 1) % sel_num);
	}

	// 選択項目描画
	const char* name_tbl[CExecute::TYPE_NUM + 1] = {
		"BLACK",
		"WHITE",
		"YELLOW",
		"MAGENTA",
		"CYAN",
		"BLUE",
		"BREEN",
		"RED",
		"DISABLE",
	};
	for (u32 i = 0; i < sel_num; ++i) {
		if (m_select == i) {
			amPrintColor(0xff0000ff);
			amPrint(4, (s32)(8 + i), ">");
		}
		else {
			amPrintColor(0xffffffff);
		}
		amPrintf(6, (s32)(8 + i), "%s", name_tbl[i]);
	}
	amPrintColor(0xffffffff);

	// 決定判定
	if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {

		// 実行開始
		if (m_select < (u32)CExecute::TYPE_NUM) {
			g_ao_test_safeframe_execute =
				new ao::test::safeframe::CExecute((u32)m_select);
		}

		// 終了
		delete this;
	}

	// 終了判定
	else if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		delete this;
	}

	// 選択したものを表示
	else {
		if (m_select < (u32)CExecute::TYPE_NUM) {
			u32 color = CExecute::GetTypeColor(m_select);
			AoTestDrawSafeFrame(0xffff, color, TRUE);
		}
	}
}


// ***************************************************************************
// 実行
// ***************************************************************************
// ===========================================================================
//! コンストラクタ
// ===========================================================================
CExecute::CExecute(u32 type)
{
	SetType(type);

	MakeTask(0, "aoTestSafeframe::Execute");
	SetTaskProc(0, &CExecute::TaskProcMain);
	StartTask(0);
}

// ===========================================================================
//! デストラクタ
// ===========================================================================
CExecute::~CExecute()
{
	g_ao_test_safeframe_execute = NULL;
}

// ===========================================================================
//! タイプ変更
// ===========================================================================
void CExecute::SetType(u32 type)
{
	m_type = type;
}

// ===========================================================================
//! タイプの色取得
// ===========================================================================
u32 CExecute::GetTypeColor(u32 type)
{
	const u32 color_tbl[TYPE_NUM] = {
		0x0000007f,	// 黒
		0xffffff7f,	// 白
		0xffff007f,	// イエロー
		0xff00ff7f,	// マゼンダ
		0x00ffff7f,	// シアン
		0x0000ff7f,	// 青
		0x00ff007f,	// 緑
		0xff00007f,	// 赤
	};
	return color_tbl[type];
}

// ===========================================================================
//! タスク
// ===========================================================================
void CExecute::TaskProcMain()
{
	AoTestDrawSafeFrame(0xffff, GetTypeColor(m_type), TRUE);
}

} // namespace safeframe
} // namespace test
} // namespace ao


// ***************************************************************************
// その他
// ***************************************************************************
// ===========================================================================
//! セーフフレーム描画
// ===========================================================================
void aoTestDrawSafeFrameTask(AMS_TCB* tcb)
{
	// ワーク取得
	AOS_TEST_DRAW_SF* work = *((AOS_TEST_DRAW_SF**)amTaskGetWork(tcb));

	// プラットフォームごとのセーフフレーム位置を算出
	const f32 disp_w = (f32)AOD_ACT_SCREEN_WIDTH;
	const f32 disp_h = (f32)AOD_ACT_SCREEN_HEIGHT;
	f32	sf_w = 0.0f;
	f32	sf_h = 0.0f;
#if defined(AOD_PLATFORM_WIN32) | defined(AOD_PLATFORM_XBOX360)

	// 規約なしなので、とりあえずPS3と合わせておく
	sf_w = disp_w * (1.0f - 0.85f) * 0.5f;
	sf_h = disp_h * (1.0f - 0.85f) * 0.5f;

#elif defined(AOD_PLATFORM_PS3)

	// 縦横共に85%
	sf_w = disp_w * (1.0f - 0.85f) * 0.5f;
	sf_h = disp_h * (1.0f - 0.85f) * 0.5f;

#elif defined(AOD_PLATFORM_WII)

	// 4:3の場合は横84.4%縦80.8%
	// 16:9の場合は縦横共に87%
	if (_am_draw_video.wide_screen) {
		sf_w = disp_w * (1.0f - 0.844f) * 0.5f;
		sf_h = disp_h * (1.0f - 0.808f) * 0.5f;
	}
	else {
		sf_w = disp_w * (1.0f - 0.87f) * 0.5f;
		sf_h = disp_h * (1.0f - 0.87f) * 0.5f;
	}

#elif defined(AOD_PLATFORM_IPHONE)

	// 未作成

#endif

	// 描画ステート初期化
	amDrawPushState();
	amDrawInitState();

	// 前処理
	AoActDrawPre();

	// Z更新無効 & Zテスト無効 & ブレンド設定
#if defined(AOD_PLATFORM_WIN32) | defined(AOD_PLATFORM_XBOX360)

	nnSetPrimitive3DAlphaTestDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthMaskDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthTestDXG20(NNE_FALSE);
	nnSetPrimitive3DBlendDXG20(
		NNE_BLENDMODE_SRCALPHA, NNE_BLENDMODE_INVSRCALPHA, NNE_BLENDOP_ADD);

#elif defined(AOD_PLATFORM_PS3)

	nnSetPrimitive3DAlphaTestPS3(NNE_FALSE);
	nnSetPrimitive3DDepthMaskPS3(NNE_FALSE);
	nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_ALWAYS);
	nnSetPrimitive3DBlendPS3(
		NND_BLENDFUNC_PS3_SRC_ALPHA,
		NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA,
		NND_BLENDOP_PS3_FUNC_ADD);

#elif defined(AOD_PLATFORM_WII)

	nnSetPrimitive3DAlphaCompareGC(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	nnSetPrimitive3DZModeGC(GX_FALSE, GX_ALWAYS, GX_FALSE);
	nnSetPrimitive3DBlendModeGC(
		GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);

#elif defined(AOD_PLATFORM_IPHONE)

	nnSetPrimitive3DAlphaFuncGL(NND_CMPFUNC_GL_ALWAYS, 0.5f);
	nnSetPrimitive3DDepthMaskGL(FALSE);
	nnSetPrimitive3DDepthFuncGL(NND_CMPFUNC_GL_ALWAYS);
	nnSetPrimitiveBlend(NNE_PRIM_BLEND_BLEND);

#endif

	// フォグ設定
	amDrawSetFog(0);

	// テクスチャ設定
	nnSetPrimitiveTexNum(NULL, -1);

	if (work->fill) {
		// 描画開始
		nnBeginDrawPrimitive3D(
			NNE_PRIM3D_FMT_PC,
			NNE_PRIM_ALPHABLEND_ON,
			NNE_PRIM_LIGHT_DISABLE,
			NNE_PRIM_CULL_NONE);

		// 頂点データ作成
		NNS_PRIM3D_PC v[24];
		for (u32 i = 0; i < 24; ++i) {
			v[i].Col = work->rgba;
			v[i].Pos.z = -2.0f;
		}

		// 左
		NNS_PRIM3D_PC* vl = &v[0];
		vl[0].Pos.x = vl[1].Pos.x = 0.0f;
		vl[2].Pos.x = vl[5].Pos.x = sf_w;
		vl[0].Pos.y = vl[2].Pos.y = 0.0f;
		vl[1].Pos.y = vl[5].Pos.y = disp_h;
		vl[3].Pos = vl[1].Pos;
		vl[4].Pos = vl[2].Pos;

		// 右
		NNS_PRIM3D_PC* vr = &v[6];
		for (u32 i = 0; i < 6; ++i) {
			vr[i].Pos.x = vl[i].Pos.x + (disp_w - sf_w);
			vr[i].Pos.y = vl[i].Pos.y;
		}

		// 上
		NNS_PRIM3D_PC* vt = &v[12];
		vt[0].Pos.x = vt[1].Pos.x = sf_w;
		vt[2].Pos.x = vt[5].Pos.x = disp_w - sf_w;
		vt[0].Pos.y = vt[2].Pos.y = 0.0f;
		vt[1].Pos.y = vt[5].Pos.y = sf_h;
		vt[3].Pos = vt[1].Pos;
		vt[4].Pos = vt[2].Pos;

		// 下
		NNS_PRIM3D_PC* vb = &v[18];
		for (u32 i = 0; i < 6; ++i) {
			vb[i].Pos.x = vt[i].Pos.x;
			vb[i].Pos.y = vt[i].Pos.y + (disp_h - sf_h);
		}

		// 描画
		AoActDrawCorWide(v, 24, AOD_ACT_CORW_NONE);
		nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_LIST, v, 24);

		// 描画終了
		nnEndDrawPrimitive3D();
	}
	else {
		// 頂点データ作成
		NNS_PRIM3D_P v[8];
		v[0].Pos.x = v[2].Pos.x = sf_w;
		v[4].Pos.x = v[6].Pos.x = disp_w - sf_w;
		v[0].Pos.y = v[6].Pos.y = sf_h;
		v[2].Pos.y = v[4].Pos.y = disp_h - sf_h;
		v[0].Pos.z = v[2].Pos.z = v[4].Pos.z = v[6].Pos.z = -2.0f;
		v[1] = v[0];
		v[3] = v[2];
		v[5] = v[4];
		v[7] = v[6];
		AoActDrawCorWide(v, 8, AOD_ACT_CORW_NONE);

		// カラー作成
		NNS_RGBA color;
		color.r = (f32)((f32)((u8)(work->rgba >> 24)) / 255.0f);
		color.g = (f32)((f32)((u8)(work->rgba >> 16)) / 255.0f);
		color.b = (f32)((f32)((u8)(work->rgba >>  8)) / 255.0f);
		color.a = (f32)((f32)((u8)(work->rgba >>  0)) / 255.0f);

		// 描画
		nnBeginDrawPrimitiveLine3D(&color, NNE_PRIM_ALPHABLEND_ON);
		nnDrawPrimitiveLine3D(NNE_PRIM_LINE_LIST, v, 8);
		nnEndDrawPrimitiveLine3D();
	}

	// 描画ステート復帰
	amDrawPopState();
}

#endif // defined (MTD_DEBUG)

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
