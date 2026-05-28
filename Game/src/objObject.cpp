// ================================================================
/*!
  @file obObject.c
  @brief オブジェクト

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objObject.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
//#include <String.h>
#ifndef _DS
#include "mt.h"
#endif
#include "objObject.h"
#include "efEffect.h"

#include "objObjectLoad.h"

#include "gmTvx.h"

//----- Definitions ---------------------------------------------------------
#define OBD_OBJECT_DIE_OFFSET			( 64 )				///< 画面外判定オフセット値
#define OBD_OBJECT_COL_MAX				(8 << FX32_SHIFT)	///< １フレームでチェックする移動量MAX(この値を超えた移動量を持つオブジェクトは１フレーム当たり数回に分けてコリジョンチェックする)
#define OBD_CAMERA_2DSPRITE_FOV			( 0x3f00 )			///< OBD_DISP_3D_SPRITEを使って表示する時のFOV値

#define OBD_COL_THROUGH_CHECK_LENGTH	( 5 )				///< すり抜けチェックを行う距離、標準値


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------
#if defined _DS
extern u32     _mt_vram_tex_slot_at_lcdc[4];		// mtVram.c
#endif

extern void GmGmkPulleyDrawServerMain(void);
extern void gmDecoDrawServerMain(MTS_TASK_TCB *tcb);

//----- Global Variables ----------------------------------------------------
OBS_OBJECT g_obj = {{0}};	///< オブジェクトシステム管理ワーク

/// 汎用振動用テーブル
#if 1
const fx16 g_object_vib_tbl[] =
{
    (fx16)(1*FX16_ONE),		(fx16)(1*FX16_ONE),		(fx16)(-1*FX16_ONE),		(fx16)(-1*FX16_ONE),
	(fx16)(0.5*FX16_ONE),	(fx16)(0.5*FX16_ONE),	(fx16)(0.5*FX16_ONE),	(fx16)(-0.5*FX16_ONE),
    (fx16)(1*FX16_ONE),		(fx16)(-1*FX16_ONE),	(fx16)(-1*FX16_ONE),	(fx16)(1*FX16_ONE),
    (fx16)(0.5*FX16_ONE),	(fx16)(-0.5*FX16_ONE),	(fx16)(-0.5*FX16_ONE),	(fx16)(0.5*FX16_ONE),
   // 2, 2, -2,-2,
};
#else
const s8 g_object_vib_tbl[] =
{
    1, 1, -1,-1,
    2, 2, -2,-2,
    4, 4, -4,-4,
   -4, 4,  4,-4,
   // 2, 2, -2,-2,
};
#endif

#if (OBD_USE_ACTION3D_NN)
/// 描画時設定ステータス初期化用データ
AMS_DRAWSTATE g_obj_draw_3dnn_draw_state;
#endif
//----- Local Variables -----------------------------------------------------
MTS_TASK_TCB		*obj_ptcb = NULL;		///< オブジェクトシステムメインタスク

// データワーク退避
OBS_DATA_WORK	*obj_data_work_save = NULL;	///< データワーク退避
s32				obj_data_max_save = 0;		///< データワーク数退避

//----- Static Declarations -------------------------------------------------
#if defined _DS
static void objMain();
#else
static void objMain(MTS_TASK_TCB *tcb);
#endif
static void objObjectDraw(OBS_OBJECT_WORK *pWork);
static void objDestructor( MTS_TASK_TCB *pTcb );
#if !defined _DS
static void objExitWait( MTS_TASK_TCB *pTcb );
#endif
//static void objObjectRevokeObject(OBS_OBJECT_WORK *pWork);


#if !defined _DS
static void objObjectExitDataRelease(MTS_TASK_TCB *tcb);
static void objObjectDataReleaseCheck(MTS_TASK_TCB *tcb);
#endif
static u16 objObjectParent( OBS_OBJECT_WORK *pWork );

#if 0
static void objActDivisionDraw( MTS_ACTION *act, s16 *pOffst );
static void objActDivisionDrawAffine( MTS_ACTION *act, s16 *pOffst, fx32 scale_x, fx32 scale_y );
#endif
void objObjectColRideTouchCheck( OBS_OBJECT_WORK * pWork );
//static void objObjectActionCallBack( const MTS_ACT_COMMAND* pCmd, MTS_ACTION* pAct, u32 ulObjWork);

//----- Global Functions ----------------------------------------------------
// ================================================================
// システム初期化
// ================================================================
// ================================================================
// ObjInit
/*!
 *	オブジェクト設定を初期化、毎フレーム処理を行うタスク生成
 *
 *	@param	group		[in]	オブジェクトシステムメイン管理処理 タスクグループ
 *	@param	prio		[in]	オブジェクトシステムメイン管理処理 タスク優先
 *	@param	pause_level	[in]	オブジェクトシステム タスクポーズレベル
 *	@param	lcd_size_x	[in]	オブジェクトシステム 画面Xサイズ
 *	@param	lcd_size_y	[in]	オブジェクトシステム 画面Yサイズ
 *	@param	disp_width	[in]	表示解像度X
 *	@param	disp_height	[in]	表示解像度Y
 *
 *	@note
 *		既にタスクが存在する時は一度終了します
 */
// ================================================================
void ObjInit(u8 group, u16 prio, u8 pause_level, s16 lcd_size_x, s16 lcd_size_y, float disp_width, float disp_height)
{
	if (obj_ptcb) {
		ObjExit();
	}

    MI_CpuClear8( (void*)&g_obj, sizeof(OBS_OBJECT) );

    ObjDispSRand(0);
    
    g_obj.speed   = FX32_ONE; // オブジェクト処理速度
    g_obj.glb_scale.x = FX32_ONE;	// オブジェクト拡大率 全体スケール
    g_obj.glb_scale.y = FX32_ONE;	// オブジェクト拡大率 全体スケール
    g_obj.glb_scale.z = FX32_ONE;	// オブジェクト拡大率 全体スケール
    g_obj.draw_scale.x = FX32_ONE;	// オブジェクト拡大率 描画スケール
    g_obj.draw_scale.y = FX32_ONE;	// オブジェクト拡大率 描画スケール
    g_obj.draw_scale.z = FX32_ONE;	// オブジェクト拡大率 描画スケール
    g_obj.scale.x = FX32_ONE;		// オブジェクト拡大率 glb_scale * draw_scale
    g_obj.scale.y = FX32_ONE;		// オブジェクト拡大率 glb_scale * draw_scale
    g_obj.scale.z = FX32_ONE;		// オブジェクト拡大率 glb_scale * draw_scale
    g_obj.inv_scale.x = FX32_ONE;	// オブジェクト拡大率 scaleの逆数
    g_obj.inv_scale.y = FX32_ONE;	// オブジェクト拡大率 scaleの逆数
    g_obj.inv_scale.z = FX32_ONE;	// オブジェクト拡大率 scaleの逆数
    g_obj.inv_glb_scale.x = FX32_ONE;	// オブジェクト拡大率 glb_scaleの逆数
    g_obj.inv_glb_scale.y = FX32_ONE;	// オブジェクト拡大率 glb_scaleの逆数
    g_obj.inv_glb_scale.z = FX32_ONE;	// オブジェクト拡大率 glb_scaleの逆数
    g_obj.inv_draw_scale.x = FX32_ONE;	// オブジェクト拡大率 draw_scaleの逆数
    g_obj.inv_draw_scale.y = FX32_ONE;	// オブジェクト拡大率 draw_scaleの逆数
    g_obj.inv_draw_scale.z = FX32_ONE;	// オブジェクト拡大率 draw_scaleの逆数
    g_obj.depth   = FX32_ONE;		// ベルト奥行き率
    g_obj.col_through_dot = OBD_COL_THROUGH_CHECK_LENGTH;
	g_obj.cam_scale_center[0][MTD_X] = (s16)(lcd_size_x / 2);	// 拡大時描画位置拡大中心
	g_obj.cam_scale_center[0][MTD_Y] = (s16)(lcd_size_y / 2);	// 拡大時描画位置拡大中心
	g_obj.cam_scale_center[1][MTD_X] = (s16)(lcd_size_x / 2);	// 拡大時描画位置拡大中心
	g_obj.cam_scale_center[1][MTD_Y] = (s16)(lcd_size_y / 2);	// 拡大時描画位置拡大中心
#if defined _DS
#else
	g_obj.disp_width	= disp_width;
	g_obj.disp_height	= disp_height;
#endif
	g_obj.lcd_size[MTD_X] = lcd_size_x;	// 画面サイズX
	g_obj.lcd_size[MTD_Y] = lcd_size_y;	// 画面サイズY
#if defined _DS
#else
	g_obj.clip_lcd_size[MTD_X] = lcd_size_x;	// イベント生成範囲用サイズX
	g_obj.clip_lcd_size[MTD_Y] = lcd_size_y;	// イベント生成範囲用サイズY
#endif

#if (OBD_USE_ACTION3D_NN)
	// 描画ステート初期化
	//amDrawInitState();	DrawThreadでの初期化に変更
//	g_obj.load_drawflag = (NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND |
//								NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT |
//								NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND |
//								NND_DRAWOBJ_MATCTRL_TEXOFFSET | NND_DRAWOBJ_MATCTRL_SPECULAR |
//								NND_DRAWOBJ_MATCTRL_ENVTEXMTX);
//	g_obj.drawflag		= (NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND |
//								NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT |
//								NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND |
//								NND_DRAWOBJ_MATCTRL_TEXOFFSET | NND_DRAWOBJ_MATCTRL_SPECULAR |
//								NND_DRAWOBJ_MATCTRL_ENVTEXMTX);
#if _PC | _XBOX | _PS3
	//g_obj.load_drawflag = (NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND |
	//							NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT |
	//							NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND |
	//							NND_DRAWOBJ_MATCTRL_TEXOFFSET);
	//g_obj.drawflag		= (NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND |
	//							NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT |
	//							NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND |
	//							NND_DRAWOBJ_MATCTRL_TEXOFFSET);
	g_obj.load_drawflag = (NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND);
	g_obj.drawflag		= (NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND);
#elif _WII | _IPHONE
	//g_obj.load_drawflag = (NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT |
	//							NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND |
	//							NND_DRAWOBJ_MATCTRL_TEXOFFSET);
	//g_obj.drawflag		= (NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT |
	//							NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND |
	//							NND_DRAWOBJ_MATCTRL_TEXOFFSET);
	g_obj.load_drawflag = (0);
	g_obj.drawflag		= (0);
#endif
#endif

#ifndef _DS
	g_obj.glb_camera_id = -1;		// カメラID はじめは無効
#endif
    // デバッグ矩形初期化
    // objDebugRectActionInit();

    // 矩形チェック初期化
    ObjRectCheckInit();
    
    // オブジェクト矩形初期化
    ObjCollisionObjectClear();
    ObjCollisionObjectClear();

	// 描画システム初期化
	ObjDrawInit();

#if defined _DS
    // パレット初期化
    ObjPaletteInit();
#endif

#if OBD_LOAD_INITIAL_DRAW
	ObjLoadSetInitDrawFlag(FALSE); // 基本的には使用しない
#endif // OBD_LOAD_INITIAL_DRAW
	// オブジェクト管理リスト
    
    if ( obj_ptcb == NULL )
        obj_ptcb = MTM_TASK_MAKE_TCB( objMain, objDestructor, 0, pause_level, prio, group, 0, "object" );
}

// ================================================================
// システム終了
// ================================================================
// ================================================================
// ObjExit
/*!
  オブジェクト終了処理

  @note
    初期化されていない場合は何も実行しません。
 */
// ================================================================
void ObjExit(void)
{
#if defined _DS
	if (obj_ptcb) {
#if OBD_USE_TEX_VRAM_DB
		if (g_obj.db_tex_tcb) {
			// テクスチャVRAM ダブルバッファシステム管理処理クリア
			mtTaskClearTcb(g_obj.db_tex_tcb);
			g_obj.db_tex_tcb = NULL;
		}
#endif // #if OBD_USE_TEX_VRAM_DB

		// 管理オブジェクト破棄
		ObjObjectClearAllObject();

		// オブジェクトシステムメイン処理クリア
		mtTaskClearTcb(obj_ptcb);

		// オブジェクトシステム管理ワーククリア
		MI_CpuClear8((void*)&g_obj, sizeof(OBS_OBJECT));

		// ObjDataFree は objDestructor内から呼び出し
		// obj_ptcb のクリアは objDestructor内から呼び出し
	}
#else

	if (obj_ptcb) {
		OBS_OBJECT_WORK	*obj_work;

#if OBD_USE_TEX_VRAM_DB
		if (g_obj.db_tex_tcb) {
			// テクスチャVRAM ダブルバッファシステム管理処理クリア
			mtTaskClearTcb(g_obj.db_tex_tcb);
			g_obj.db_tex_tcb = NULL;
		}
#endif // #if OBD_USE_TEX_VRAM_DB

		// 管理オブジェクト破棄
		obj_work = ObjObjectSearchRegistObject(NULL, 0xFFFF);
		while (obj_work) {
			// 強制終了
			obj_work->flag |= OBD_OBJECT_TASKCLEAR;
			obj_work = ObjObjectSearchRegistObject(obj_work, 0xFFFF);
		}

		// 終了待機
		mtTaskChangeTcbProcedure(obj_ptcb, objExitWait);

		// 終了待機中に
		g_obj.flag |= OBD_OBJ_EXIT_WAIT;
	}

#endif
}

#if OBD_USE_ACTION3D_NN
// ================================================================
// ObjPreExit
/*!
  オブジェクト終了前処理

  @note
    初期化されていない場合は何も実行しません。\n
	処理の終了などの データ開放以外の終了を行います
 */
// ================================================================
void ObjPreExit(void)
{
	// カメラ終了処理
	ObjCameraExit();
	g_obj.glb_camera_id = -1;
}
#endif


// ================================================================
// 実行状況チェック
// ================================================================
// ================================================================
// ObjIsInit
/*!
  オブジェクトシステムが稼動しているかチェック

  @return	TRUE : 稼動中(もしくは終了処理待機中)
 */
// ================================================================
BOOL ObjIsInit(void)
{
	return (obj_ptcb ? TRUE : FALSE);
}

// ================================================================
// ObjIsExitWait
/*!
  オブジェクトシステムが終了処理中かチェック

  @return	TRUE : 終了処理中
 */
// ================================================================
BOOL ObjIsExitWait(void)
{
	if (obj_ptcb && (g_obj.flag & OBD_OBJ_EXIT_WAIT)) {
		return (TRUE);
	}
	return (FALSE);
}

// ================================================================
// オブジェクトポーズ
// ================================================================
// ================================================================
// ObjObjectPause
/*!
  オブジェクトポーズする

	@param	pause_level	[in]	オブジェクトポーズレベル

	@note
		オブジェクトのポーズレベルが指定したポーズレベル以下の場合ポーズになる

 */
// ================================================================
void ObjObjectPause(u16 pause_level)
{
    g_obj.flag |= OBD_OBJ_PAUSE_START;
    g_obj.pause_level = pause_level;
}
// ================================================================
// ObjObjectPauseOut
/*!
  オブジェクトポーズを解除する

 */
// ================================================================
void ObjObjectPauseOut()
{
    g_obj.flag &= ~OBD_OBJ_PAUSE_START;
    g_obj.pause_level = -1;
}

#if 0
// ================================================================
// ObjObjectForcePause
/*!
  強制オブジェクトポーズする

 */
// ================================================================
void ObjObjectForcePause()
{
    g_obj.flag |= OBD_OBJ_FORCE_PAUSE_START;
}
// ================================================================
// ObjObjectForcePauseOut
/*!
  強制オブジェクトポーズを解除する

 */
// ================================================================
void ObjObjectForcePauseOut()
{
    g_obj.flag &= ~OBD_OBJ_FORCE_PAUSE_START;
}
#endif

// ================================================================
// ObjObjectPauseCheck
/*!
  オブジェクトシステムポーズチェック

  @param  ulFlag [in] オブジェクトワークフラグ、無い場合は0を指定
    
  @return 0 ポーズ中では無い 非0 ポーズ中

 */
// ================================================================
u32 ObjObjectPauseCheck( u32 ulFlag )
{
    //return ((( g_obj.flag & (OBD_OBJ_PAUSE|OBD_OBJ_ACTER_FORCE_PAUSE) ) && !( ulFlag & OBD_OBJECT_NOPAUSE )) || ( g_obj.flag & (OBD_OBJ_FORCE_PAUSE) ));
    return (( g_obj.flag & OBD_OBJ_PAUSE ) && !( ulFlag & OBD_OBJECT_NOPAUSE ) );
}

#if 0
// ================================================================
// ObjObjectPauseCheck
/*!
  オブジェクトポーズチェック

  @param  obj_work [in] オブジェクトワーク

  @return TRUE : ポーズ中		FALSE : 実行中

 */
// ================================================================
BOOL ObjObjectPauseCheck(OBS_OBJECT_WORK *obj_work)
{
	if (ObjObjectPauseCheck(obj_work->flag)) {
		if (obj_work->pause_level <= g_obj.pause_level) {
			return (TRUE);
		}
	}

	return (FALSE);
}
#endif


// ================================================================
// データワーク
// ================================================================
// ================================================================
// ObjDataAlloc
/*!
    データファイル管理用のメモリを作成する

  @param num [in] 今ゲームで同時使用するファイルの最大値

 */
// ================================================================
void ObjDataAlloc( s32 num )
{
	if (obj_data_work_save) {
		// 退避データの復帰
		MTM_ASSERT(num == obj_data_max_save);

		g_obj.pData		= obj_data_work_save;
		g_obj.data_max	= obj_data_max_save;
		obj_data_work_save	= NULL;
		obj_data_max_save	= 0;
	}
	else {
		g_obj.data_max = num;
		g_obj.pData = (OBS_DATA_WORK*)mtMemAllocMain( sizeof(OBS_DATA_WORK) * num );
		MI_CpuClear8( g_obj.pData, sizeof(OBS_DATA_WORK) * num );
	}
}

// ================================================================
// ObjDataGet
/*!
  データファイル管理用のメモリアドレスを取得する

  @param index [in] データ管理ID

  @return データ管理ポインタ
 */
// ================================================================
OBS_DATA_WORK* ObjDataGet( s32 index)
{
    MTM_ASSERT( g_obj.pData );

    if ( g_obj.data_max <= index )
        return NULL;
    return &g_obj.pData[index];
}
// ================================================================
// ObjDataFree
/*!
  データファイル管理用のメモリを開放する
 */
// ================================================================
void ObjDataFree()
{
    g_obj.data_max = 0;
#if 1
	// データ管理リスト開放
	if ( g_obj.pData ) {
		mtMemFreeMain( (void*)g_obj.pData );
		g_obj.pData = NULL;
	}
#else
    MTM_ASSERT( g_obj.pData );
    // データ管理リスト開放
    if ( g_obj.pData )
        mtMemFreeMain( (void*)g_obj.pData );
#endif
}

// ================================================================
// オブジェクトシステム設定
// ================================================================
// イベント生成範囲
// ================================================================
// ObjObjectClipLCDSet
/*!
  オブジェクト全体に適用するオフセットを設定

	@param size_x	[in]	イベント生成範囲用サイズX
	@param size_y	[in]	イベント生成範囲用サイズY

 */
// ================================================================
#ifndef _DS
void ObjObjectClipLCDSet(s16 size_x, s16 size_y)
{
	g_obj.clip_lcd_size[MTD_X] = size_x;	// イベント生成範囲用サイズX
	g_obj.clip_lcd_size[MTD_Y] = size_y;	// イベント生成範囲用サイズY
}
#endif


// オフセット
// ================================================================
// ObjObjectOffsetSet
/*!
  オブジェクト全体に適用するオフセットを設定

  @param sX [in] オブジェクトに適用されるオフセット
  @param sY [in] 

 */
// ================================================================
void ObjObjectOffsetSet(s16 sX,s16 sY )
{
    g_obj.offset[MTD_X] = sX;
    g_obj.offset[MTD_Y] = sY;
}

// 速度
// ================================================================
// ObjObjectSpeedSet
/*!
  オブジェクト全体に適用する処理速度を設定

  @param sSpd [in] オブジェクトに適用される処理速度 1:19:12

 */
// ================================================================
void ObjObjectSpeedSet(fx32 sSpd )
{
    g_obj.speed = sSpd;
}
// ================================================================
// ObjObjectSpeedGet
/*!
  オブジェクト全体に適用する処理速度を取得

  @return   処理速度

 */
// ================================================================
fx32 ObjObjectSpeedGet(void)
{
    return g_obj.speed;
}

// オートスクロール
// ================================================================
// ObjObjectScrollSet
/*!
	オブジェクト全体に適用するオートスクロール速度設定

	@param	spd_x	[in]	速度X
	@param	spd_y	[in]	速度Y
 */
// ================================================================
void ObjObjectScrollSet(fx32 spd_x, fx32 spd_y)
{
	g_obj.scroll[MTD_X] = spd_x;
	g_obj.scroll[MTD_Y] = spd_y;
}
// ================================================================
// ObjObjectScrollGetX
/*!
	オブジェクト全体に適用するオートスクロール速度X取得

	@return   スクロール速度X
 */
// ================================================================
fx32 ObjObjectScrollGetX(void)
{
	return (g_obj.scroll[MTD_X]);
}
// ================================================================
// ObjObjectScrollGetY
/*!
	オブジェクト全体に適用するオートスクロール速度X取得

	@return   スクロール速度Y
 */
// ================================================================
fx32 ObjObjectScrollGetY(void)
{
	return (g_obj.scroll[MTD_Y]);
}

// ベルトシステム
// ================================================================
// ObjObjectBeltSetDepth
/*!
	ベルト系描画時奥行き設定

	@param	depth	[in]	奥行き
 */
// ================================================================
void ObjObjectBeltSetDepth(fx32 depth)
{
	g_obj.depth = depth;
}
// ================================================================
// ObjObjectBeltGetDepth
/*!
	ベルト系描画時奥行き取得

	@return	奥行き
 */
// ================================================================
fx32 ObjObjectBeltGetDepth(void)
{
	return (g_obj.depth);
}

// カメラ
// ================================================================
// ObjObjectCameraSet
/*!
  オブジェクト全体に適用するオフセットを設定

  @param x1 [in] カメラ1（グラフィックエンジンA）の X座標 1:19:12
  @param y1 [in] カメラ1（グラフィックエンジンA）の Y座標 1:19:12
  @param x2 [in] カメラ2（グラフィックエンジンB）の X座標 1:19:12
  @param y2 [in] カメラ2（グラフィックエンジンB）の Y座標 1:19:12

 */
// ================================================================
void ObjObjectCameraSet( fx32 x1, fx32 y1, fx32 x2, fx32 y2 )
{
    g_obj.camera[0][MTD_X] = x1;
    g_obj.camera[0][MTD_Y] = y1;
    g_obj.camera[1][MTD_X] = x2;
    g_obj.camera[1][MTD_Y] = y2;
}
#ifndef _DS
// ================================================================
// ObjObjectClipCameraSet
/*!
  クリッピング, イベント生成範囲基カメラ を設定

  @param x		[in] 基点X
  @param y		[in] 基点Y

 */
// ================================================================
void ObjObjectClipCameraSet(fx32 x, fx32 y)
{
	// 設定
    g_obj.clip_camera[MTD_X] = x;
    g_obj.clip_camera[MTD_Y] = y;
}
#endif // #ifndef _DS
// ================================================================
// ObjObjectCameraZSet
/*!
  オブジェクト全体に適用する拡大率を設定

  @param z [in] カメラZ距離 1:19:12

  @note
    0で標準のサイズ、0x1000で半分、-0x1000で倍角
 */
// ================================================================
void ObjObjectCameraZSet( fx32 z )
{
    if ( z > 0 ){
        g_obj.glb_scale.x = FX32_ONE - ( z >> 1); // オブジェクト拡大率
        g_obj.glb_scale.y = FX32_ONE - ( z >> 1); // オブジェクト拡大率
    }else{
        g_obj.glb_scale.x = FX32_ONE - ( z ); // オブジェクト拡大率
        g_obj.glb_scale.y = FX32_ONE - ( z ); // オブジェクト拡大率
    }
    g_obj.glb_scale.z = FX32_ONE; // オブジェクト拡大率
}

// ライト
#if defined _DS
// ================================================================
// ObjObjectSetLight
/*!
	ライトベクトル設定

	@param	light_id	[in] セットするライトID (0 ～ 3)
	@param	vec_x		[in] ライト方向ベクトル X成分
	@param	vec_y		[in] ライト方向ベクトル Y成分
	@param	vec_z		[in] ライト方向ベクトル Z成分

  @note
	ベクトル範囲は -FX16_ONE ～ FX16_ONE-1
 */
// ================================================================
void ObjObjectSetLightVec(u16 light_id, fx16 vec_x, fx16 vec_y, fx16 vec_z)
{
#if defined (MTD_DEBUG)
	if (light_id >= 4) {
		OS_TPrintf("objObject::ObjObjectSetLightVec Error! light_id is outside the range.");
		MTM_ASSERT(0);
	}

	if (vec_x <-FX16_ONE || FX16_ONE-1 < vec_x) {
		OS_TPrintf("objObject::ObjObjectSetLightVec Warning vec_x is outside the range.");
	}
	if (vec_y <-FX16_ONE || FX16_ONE-1 < vec_y) {
		OS_TPrintf("objObject::ObjObjectSetLightVec Warning vec_y is outside the range.");
	}
	if (vec_z <-FX16_ONE || FX16_ONE-1 < vec_z) {
		OS_TPrintf("objObject::ObjObjectSetLightVec Warning vec_z is outside the range.");
	}
#endif // #if defined (MTD_DEBUG)

	g_obj.light_dir[light_id].x = vec_x;
	g_obj.light_dir[light_id].y = vec_y;
	g_obj.light_dir[light_id].z = vec_z;
}

// ================================================================
// ObjObjectSetLightNum
/*!
	使用ライト数設定

	@param	light_num	[in] 使用するライト数 (0 ～ 4)

  @note
	0 でオブジェクトシステムではライトを設定しない
 */
// ================================================================
void ObjObjectSetLightNum(u16 light_num)
{
	g_obj.light_num = light_num;
}
#endif

#if defined _DS
// OBJ VRAM マッピングモード
// ================================================================
// ObjObjectSetVramMapMode
/*!
	OBJ VRAM マッピングモード設定

	@param mmode [in] OBJ VRAM マッピングモード OBE_OBJ_VRAM_MMODE
 */
// ================================================================
void ObjObjectSetVramMapMode(OBE_OBJ_VRAM_MMODE mmode)
{
    g_obj.vram_map_mode = (u16)mmode;
}
#endif

// ================================================================
// テクスチャVRAM ダブルバッファ
// ================================================================
#if OBD_USE_TEX_VRAM_DB
// ================================================================
// ObjObjectSetTexDoubleBuffer
/*!
	テクスチャVRAM ダブルバッファ

	@param	bank1			[in] テクスチャ割り当てVRAMバンク1 GX_VRAM_TEX_***
	@param	bank2			[in] テクスチャ割り当てVRAMバンク2 GX_VRAM_TEX_***
	@param	db_slot_flag	[in] ダブルバッファ使用するスロットフラグ

	@note
		実行前に mtVramSetBankTex で一度初期化しておくこと
 */
// ================================================================
/// テクスチャVRAM LCDC割り当て時アドレス
static u32 obj_object_tex_slot_at_lcdc_tbl[4] = {
	HW_LCDC_VRAM_A,
	HW_LCDC_VRAM_B,
	HW_LCDC_VRAM_C,
	HW_LCDC_VRAM_D,
};
static void objObjectTexDoubleBufferVFunc(void);
static void objObjectTexDoubleBufferDest(MTS_TASK_TCB *tcb);
void ObjObjectSetTexDoubleBuffer(GXVRamTex bank1, GXVRamTex bank2, u8 db_slot_flag)
{
	s32	i, slot_cnt_1, slot_cnt_2;

	if (!obj_ptcb) {
		// システムが初期化されていない
		MTM_ASSERT(0);
		return;
	}

	g_obj.flag |= OBD_OBJ_USE_TEX_VRAM_DB;

	// 使用テクスチャVRAMバンク保存
	g_obj.db_tex_bank[0] = bank1;	// GX_VRAM_A
	g_obj.db_tex_bank[1] = bank2;

	// ダブルバッファ使用するスロットフラグ保存
	g_obj.db_tex_db_slot_flag = db_slot_flag;

	g_obj.db_tex_vram_flip = 0;


	for (i = 0, slot_cnt_1 = 0, slot_cnt_2 = 0; i < 4; i++) {
		if (bank1 & (GX_VRAM_A << i) && slot_cnt_1 < 3) {
			g_obj.db_tex_slot_at_lcdc[0][slot_cnt_1] = obj_object_tex_slot_at_lcdc_tbl[i];
			slot_cnt_1++;
		}
		if (bank2 & (GX_VRAM_A << i) && slot_cnt_2 < 3) {
			g_obj.db_tex_slot_at_lcdc[1][slot_cnt_2] = obj_object_tex_slot_at_lcdc_tbl[i];
			slot_cnt_2++;
		}
	}

	MTM_ASSERT(slot_cnt_1 == slot_cnt_2 && slot_cnt_1 <= 3);

	// 使用スロット数保存
	g_obj.db_tex_slot_num = (u8)slot_cnt_1;

	// bank1 を初期参照VRAMに設定
	//mtVramSetBankTex(bank1);
	// 通常転送可能VRAM設定 (bank2)
	for (i = 0; i < g_obj.db_tex_slot_num; i++) {
		_mt_vram_tex_slot_at_lcdc[i] = g_obj.db_tex_slot_at_lcdc[1][i];
	}
	// 参照テクスチャVRAMバンク切り替え (bank1を接続)
	GX_SetBankForTex(g_obj.db_tex_bank[0]);

	// システムタスク生成
	g_obj.db_tex_tcb = MTM_TASK_MAKE_TCB(objObjectTexDoubleBufferVFunc, objObjectTexDoubleBufferDest,
									0, 0, MTD_TASK_PRIORITY_V_HEAD, 4,	// とりあえず
									0, "OBJ DB VFUNC");
}
// ==========================================================================
// objObjectTexDoubleBufferDest
/*!
 */
// ==========================================================================
void objObjectTexDoubleBufferDest(MTS_TASK_TCB *tcb)
{
}

// ==========================================================================
// objObjectTexDoubleBufferVFunc
/*!
 *
 *	@note
 *		通常転送命令が終了した後に切り替わる
 */
// ==========================================================================
void objObjectTexDoubleBufferVFunc(void)
{
	s32	i;

	// V中で転送する予定のテクスチャはCに置くようにする事

	// テクスチャVRAMバンク切り替えタイプ
#if 0
	if (g_gm_main_system.debug_start_up_mode == GMD_MAIN_STARTUP_MODE_1DISP_VRAM02) {
		if (g_gm_main_system.double_texvram_flip_flag) {
			// 転送先 GX_VRAM_TEX_01_BC を GX_VRAM_TEX_01_AC に切り替え
	        _mt_vram_tex_slot_at_lcdc[0]    = HW_LCDC_VRAM_A;
	        _mt_vram_tex_slot_at_lcdc[1]    = HW_LCDC_VRAM_C;
			GX_SetBankForTex(GX_VRAM_TEX_01_BC);		// 連結VRAMをBCに
		}
		else {
			// 転送先 GX_VRAM_TEX_01_AC を GX_VRAM_TEX_01_BC に切り替え
	        _mt_vram_tex_slot_at_lcdc[0]    = HW_LCDC_VRAM_B;
	        _mt_vram_tex_slot_at_lcdc[1]    = HW_LCDC_VRAM_C;
			GX_SetBankForTex(GX_VRAM_TEX_01_AC);		// 連結VRAMをACに
		}

		g_gm_main_system.double_texvram_flip_flag ^= 0x01;
	}
#endif

	// 通常転送可能VRAMを切り替え
//	mtVramSetBankTex(g_obj.db_tex_bank[g_obj.db_tex_vram_flip]);
	for (i = 0; i < g_obj.db_tex_slot_num; i++) {
		_mt_vram_tex_slot_at_lcdc[i] = g_obj.db_tex_slot_at_lcdc[g_obj.db_tex_vram_flip][i];
	}

	// テクスチャVRAMバンク切り替え
	GX_SetBankForTex(g_obj.db_tex_bank[g_obj.db_tex_vram_flip ^ 0x01]);

	// 反転
	g_obj.db_tex_vram_flip ^= 0x01;

}
#endif // #if OBD_USE_TEX_VRAM_DB

// 地形判定用設定
// ================================================================
// オブジェクト生成
// ================================================================
// ================================================================
// ObjObjectTaskInit
/*!
  オブジェクトタスク生成初期化関数

  @return   ワークポインタ NULLで失敗

  @note
    単純オブジェクト生成
 */
// ================================================================
OBS_OBJECT_WORK * ObjObjectTaskInit(void)
{
    return OBM_OBJECT_TASK_DETAIL_INIT( OBD_TASK_PRIO_OBJECT, OBD_TASK_GROUP_OBJECT, 0, 0, sizeof(OBS_OBJECT_WORK), "object");
}
// ================================================================
// ObjObjectTaskDetailInit
/*!
  オブジェクトタスク生成初期化関数

  @param prio				[in]  タスクプライオリティ
  @param group				[in]  タスクグループ
  @param pause_level		[in]  タスクポーズレベル
  @param obj_pause_level	[in]  オブジェクトポーズレベル
  @param work_size			[in]  タスクワークサイズ OBS_OBJECT_WORKサイズ以上を設定
  @param name				[in]  デバッグ用TCB名(NULL可)
    
  @return   ワークポインタ NULLで失敗
 */
// ================================================================
#if defined (MTD_DEBUG)
OBS_OBJECT_WORK * ObjObjectTaskDetailInit( u16 prio, u8 group, u8 pause_level, u8 obj_pause_level, u32 work_size, const char *name)
#else
OBS_OBJECT_WORK * ObjObjectTaskDetailInit( u16 prio, u8 group, u8 pause_level, u8 obj_pause_level, u32 work_size )
#endif // #if defined (MTD_DEBUG)
{
    MTS_TASK_TCB      * pTcb;
    OBS_OBJECT_WORK  * pWork;

#if defined (MTD_DEBUG)
	// ワークサイズチェック
	if (work_size < sizeof(OBS_OBJECT_WORK)) {
		MTM_ASSERT(0);
		work_size = sizeof(OBS_OBJECT_WORK);
	}
#endif // #if defined (MTD_DEBUG)

#if defined (MTD_DEBUG)
	if (name) {
	    pTcb = MTM_TASK_MAKE_TCB( ObjObjectMain, ObjObjectExit, 0, pause_level, prio,
	                              group, work_size, name );
	}
	else {
	    pTcb = MTM_TASK_MAKE_TCB( ObjObjectMain, ObjObjectExit, 0, pause_level, prio,
	                              group, work_size, "" );
	}
#else
    pTcb = MTM_TASK_MAKE_TCB( ObjObjectMain, ObjObjectExit, 0, pause_level, prio,
                              group, work_size, "" );
#endif // #if defined (MTD_DEBUG)
#if defined _DS
    if ( pTcb == MTD_TASK_ERROR_ADDR )
        return NULL;
#endif

    pWork = (OBS_OBJECT_WORK*)mtTaskGetTcbWork( pTcb );

    MI_CpuClear8( pWork, work_size );

    pWork->tcb = pTcb;
    pWork->pause_level = obj_pause_level;
    
    pWork->scale.x = FX32_ONE;
    pWork->scale.y = FX32_ONE;
    pWork->scale.z = FX32_ONE;

    // 標準フラグ

    // VRAM取得フラグ設定
    if ( !(g_obj.flag & OBD_OBJ_VRAM_AB) ){
        if ( g_obj.flag & OBD_OBJ_VRAM_B )
            pWork->flag |= OBD_OBJECT_ACT_NOA;
        else
            pWork->flag |= OBD_OBJECT_ACT_NOB;
    }

    // 標準ファンクション コアシステム関連のみ
    pWork->ppViewCheck = ObjObjectViewOutCheck;
//	pWork->ppOut		= ObjObjectActionSummary;
//	pWork->ppMove		= ObjObjectMove;
//	pWork->ppActCall	= ObjObjectActionCallBack;
//	pWork->ppRec		= ObjObjectSetRect;

    if ( g_obj.flag & OBD_OBJ_DEFAULT_NOCLIP )
        pWork->flag |= OBD_OBJECT_NOCLIP;

	// 地形あたりチェック補正値標準値設定
	pWork->field_ajst_w_db_f = 2;
	pWork->field_ajst_w_db_b = 4;
	pWork->field_ajst_w_dl_f = 2;
	pWork->field_ajst_w_dl_b = 4;
	pWork->field_ajst_w_dt_f = 2;
	pWork->field_ajst_w_dt_b = 4;
	pWork->field_ajst_w_dr_f = 2;
	pWork->field_ajst_w_dr_b = 4;

	pWork->field_ajst_h_db_r = 1;
	pWork->field_ajst_h_db_l = 1;
	pWork->field_ajst_h_dl_r = 1;
	pWork->field_ajst_h_dl_l = 1;
	pWork->field_ajst_h_dt_r = 1;
	pWork->field_ajst_h_dt_l = 1;
	pWork->field_ajst_h_dr_r = 2;
	pWork->field_ajst_h_dr_l = 2;

	// オブジェクト システム登録
	ObjObjectRegistObject(pWork);

    return pWork;
}

// ================================================================
// ObjObjectTaskNameSet
/*!
  タスクネーム設定（設定しない場合、標準の名前のままでデバッグの時に判り難い）

  @param pObj [in]  オブジェクトポインタ
  @param name [in]  文字列データ（16文字まで）
    
 */
// ================================================================
#if defined(MTD_DEBUG)  // デバッグ版
void ObjObjectTaskNameSet( OBS_OBJECT_WORK* pObj, const char * name )
{
#if defined _DS
    MTM_ASSERT( pObj );
    
    // 初期化
    MI_CpuClear8( pObj->tcb->name, MTD_TASK_MAX_TCB_NAME_LENGTH_DEBUG );

    // コピー
    if( NULL != name )
        strncpy( pObj->tcb->name, name, MTD_TASK_MAX_TCB_NAME_LENGTH_DEBUG );
#else

    MTM_ASSERT( pObj );
    
    // 初期化
    MI_CpuClear8( pObj->tcb->am_tcb->name, AMD_TASK_NAME_LEN );

    // コピー
    if( NULL != name )
        strncpy( pObj->tcb->am_tcb->name, name, AMD_TASK_NAME_LEN );
#endif
}
#endif

// ================================================================
// オブジェクト管理
// ================================================================
// ================================================================
// ObjObjectRegistObject
/*!
	オブジェクトワークをオブジェクトシステムに登録

	@param	pWork	[in] オブジェクトポインタ

	@note
		オブジェクト生成関数を自作した場合は、\n
		必ず最後にこの関数を呼んで登録して下さい。
 */
// ================================================================
void ObjObjectRegistObject(OBS_OBJECT_WORK *pWork)
{
	MTM_ASSERT(pWork);

	pWork->prev = g_obj.obj_list_tail;
	pWork->next = NULL;

	if (pWork->prev) {
		pWork->prev->next = pWork;
	}
	else {
		g_obj.obj_list_head = pWork;
	}

	g_obj.obj_list_tail = pWork;

#if defined (MTD_DEBUG)
	g_obj.register_obj_num++;

	if (!pWork->tcb) {
		OS_TPrintf("objObject::ObjObjectRegistObject Error! No registerd tcb addr");
		MTM_ASSERT(0);
	}
#endif // #if defined (MTD_DEBUG)
}
// ================================================================
// ObjObjectRevokeObject
/*!
	オブジェクトワークをオブジェクトシステムから削除

	@param	pWork	[in] オブジェクトポインタ
 */
// ================================================================
void ObjObjectRevokeObject(OBS_OBJECT_WORK *pWork)
{
	MTM_ASSERT(pWork);
#if defined (MTD_DEBUG)
	if (g_obj.register_obj_num == 0) {
		OS_TPrintf("objObject::ObjObjectRevokeObject Error! No registered object.");
		MTM_ASSERT(0);
	}
#endif // #if defined (MTD_DEBUG)

	if (pWork->prev) {
		pWork->prev->next = pWork->next;
	}
	else {
		g_obj.obj_list_head = pWork->next;
	}
	if (pWork->next) {
		pWork->next->prev = pWork->prev;
	}
	else {
		g_obj.obj_list_tail = pWork->prev;
	}
#if defined (MTD_DEBUG)
	g_obj.register_obj_num--;
#endif
}

// ================================================================
// ObjObjectClearAllObject
/*!
	登録済みオブジェクトを全クリア

	@param	pWork	[in] オブジェクトポインタ

	@note
		オブジェクト生成関数を自作した場合は、\n
		必ず最後にこの関数を呼んで登録して下さい。
 */
// ================================================================
void ObjObjectClearAllObject(void)
{
	OBS_OBJECT_WORK	*pWork, *pWorkNext;

#if OBD_USE_ACTION3D_NN
	for (pWork = g_obj.obj_list_head; pWork; pWork = pWorkNext) {
		pWorkNext = pWork->next;

		// データ開放処理へ
		pWork->flag |= OBD_OBJECT_TASKCLEAR;
		//if (pWork->tcb->proc != objObjectDataReleaseCheck) {
		//	objObjectExitDataRelease(pWork->tcb);
		//}
	}
#else
	for (pWork = g_obj.obj_list_head; pWork; pWork = pWorkNext) {
		pWorkNext = pWork->next;

		// オブジェクトタスククリア
		if (pWork->tcb) {
			mtTaskClearTcb(pWork->tcb);
		}
	}
#endif
}

#if OBD_USE_ACTION3D_NN
// ================================================================
// ObjObjectCheckClearAllObject
/*!
	登録済みオブジェクトがすべてクリアされたかチェック

	@return	TRUE : 開放終了
 */
// ================================================================
BOOL ObjObjectCheckClearAllObject(void)
{
	if (g_obj.obj_list_head) {
		return (FALSE);
	}
	return (TRUE);
}
#endif

// ================================================================
// ObjObjectSearchRegistObject
/*!
	登録済みオブジェクトを取得

	@param	obj_work	[in] サーチ元オブジェクトワーク NULLで先頭からサーチ
	@param	obj_type	[in] 取得するオブジェクトタイプ 0xFFFFですべてHIT

	@return	HITしたオブジェクトワーク 無かった時はNULL

	@note
		前にObjObjectSearchRegistObjectで取得したワークを obj_work で与える事で\n
		続きか検索します。\n
		ex)\n
			obj_work = bjObjectSearchRegistObject(NULL, TYPE);\n
			while (obj_work) {\n
				// 処理\n
				obj_work = bjObjectSearchRegistObject(obj_work, TYPE);\n
			}
 */
// ================================================================
OBS_OBJECT_WORK* ObjObjectSearchRegistObject(OBS_OBJECT_WORK *obj_work, u16 obj_type)
{
	OBS_OBJECT_WORK	*ret_obj;

	if (obj_work == NULL) {
		ret_obj = g_obj.obj_list_head;
	}
	else {
		ret_obj = obj_work->next;
	}

	while (ret_obj) {
		if (ret_obj->obj_type == obj_type ||
				obj_type == 0xFFFF) {
			break;
		}
		ret_obj = ret_obj->next;
	}

	return (ret_obj);
}

// ================================================================
// オブジェクト各種設定
// ================================================================
// ================================================================
// ObjObjectTypeSet
/*!
  オブジェクトタイプ設定

  @param pObj   [io]  オブジェクトポインタ
  @param usType [in]  オブジェクトタイプ
    
 */
// ================================================================
void ObjObjectTypeSet( OBS_OBJECT_WORK* pObj, u16 usType )
{
    MTM_ASSERT( pObj );

    pObj->obj_type = usType;
}

// ================================================================
// ObjObjectParentSet
/*!
  オブジェクト親設定

  @param pObj   [io]  オブジェクトポインタ
  @param pObj   [in]  親オブジェクトポインタ
  @param ulFlag [in]  親子関係フラグ OBD_OBJECT_PARENT_***
    
 */
// ================================================================
void ObjObjectParentSet( OBS_OBJECT_WORK* pObj, OBS_OBJECT_WORK* pParent, u32 ulFlag )
{
    MTM_ASSERT( pObj );
    MTM_ASSERT( pParent );

    pObj->parent_obj = pParent;
    pObj->flag &= ~(OBD_OBJECT_PARENT_NODIE | OBD_OBJECT_PARENT_FIX | OBD_OBJECT_PARENT_ACT_FIX);
    pObj->flag |= ulFlag & (OBD_OBJECT_PARENT_NODIE | OBD_OBJECT_PARENT_FIX | OBD_OBJECT_PARENT_ACT_FIX);
}

// ================================================================
// ObjObjecExWorkAlloc
/*!
  拡張メモリ取得

  @param pObj    [io] オブジェクトワークポインタ
  @param ulSize  [in] 取得メモリサイズ

  @return  取得したメモリのポインタ
    
  @note
    取得したメモリのポインタはpObj->pExWorkに格納されます。
    また、objObjectExitで自動的に解放されます。
    
 */
// ================================================================
void* ObjObjecExWorkAlloc ( OBS_OBJECT_WORK* pObj, u32 ulSize )
{
    if ( pObj->ex_work ){
        // 取得済みのため一度解放
        mtMemFreeMain( pObj->ex_work );
        pObj->ex_work = NULL;
    }

    if ( ulSize ){
        // メモリ取得
        pObj->ex_work = mtMemAllocMain( ulSize );
        MI_CpuClear8( pObj->ex_work, ulSize);
        pObj->flag |= OBD_OBJECT_FREE_EX;
    }
    return pObj->ex_work;
}

#if defined _DS
// ================================================================
// ObjObjectSoundHandleGet
/*!
  サウンドハンドルポインタ取得設定

  @param pObj     [io] オブジェクトワークポインタ

  @note
    これで取得した場合、タスク終了時、自動で開放する
 */
// ================================================================
NNSSndHandle* ObjObjectSoundHandleGet ( OBS_OBJECT_WORK* pObj )
{
    // サウンド解放
    if ( pObj->h_snd )
        NNS_SndHandleReleaseSeq( pObj->h_snd );
    if ( pObj->h_snd )
        mtSndFreeHandle( pObj->h_snd );
    
    pObj->h_snd = mtSndAllocHandle();
    
    return pObj->h_snd;
}
#endif

// ================================================================
// ObjObjectTblWorkSet
/*!
  当り設定

  @param pObj  [io] オブジェクトワークポインタ
  @param pTbl  [io] テーブルワークポインタ（NULLの場合、自動的にメモリを確保する、解放も自動で行う）

  @note
 */
// ================================================================
#if 0 // ◆一旦カット
void ObjObjectTblWorkSet ( OBS_OBJECT_WORK* pObj, OBS_TBL_WORK * pTbl )
{
    // 地形ワークチェック
    if ( pTbl == NULL ){
        if ( pObj->tbl_work ){
            pTbl = pObj->tbl_work;
        } else{
            // メモリ取得
            pTbl = (OBS_TBL_WORK*)mtMemAllocMain( sizeof(OBS_TBL_WORK) );
            MI_CpuClear8( pTbl, sizeof(OBS_TBL_WORK));
            pObj->flag |= OBD_OBJECT_FREE_TBL;
        }
    }
    pObj->tbl_work = pTbl;
    
    // 基本的な設定を行う
    ObjTblWorkReset(pObj->tbl_work);
}
#endif

// ================================================================
// オブジェクトメイン処理
// ================================================================
// ================================================================
// ObjObjectMain
/*!
  メイン関数
 */
// ================================================================
#if defined _DS
void ObjObjectMain(void)
#else
void ObjObjectMain(MTS_TASK_TCB *tcb)
#endif
{
    OBS_OBJECT_WORK   *pWork;

#if defined(MTD_DEBUG)  // デバッグ版
    mtSetTaskBarColor(OBD_TASK_OBJ_COLOR);
#endif  // #endif of #if defined(MTD_DEBUG)

#if defined _DS
    pWork = (OBS_OBJECT_WORK*)mtTaskGetOwnTcbWork();
#else
    pWork = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
#endif

    // タスククリアチェック
    if ( pWork->flag & OBD_OBJECT_TASKCLEAR ){
#if defined _DS
		mtTaskClearTcb(tcb);
#else
        objObjectExitDataRelease(tcb);	// データ解放を行ってから終了
#endif
        return;
    }
    // クリアリクエストチェック
    if ( pWork->flag & OBD_OBJECT_TASKCLEAR_REQUEST ){
        pWork->flag |= OBD_OBJECT_TASKCLEAR;
        return;
    }
    
    // 画面外チェック
    if ( !(pWork->flag & OBD_OBJECT_NOCLIP ) && pWork->ppViewCheck ){
        if ( pWork->ppViewCheck( pWork )){
            pWork->flag |= OBD_OBJECT_TASKCLEAR;
            return;
        }
    }

    // 親チェック
    if ( objObjectParent( pWork ) )
        return;

	// データ読み込み終了チェック
#if OBD_USE_ACTION3D_NN
	if (pWork->obj_3d) {
		BOOL	sts;
		sts = ObjAction3dNNModelLoadCheck(pWork->obj_3d);
		if (sts == FALSE && !(pWork->flag & OBD_OBJECT_NOWAITLOAD)) {
			// データ読み込み待機
			return;
		}
	}
#if OBD_USE_ACTION2D_AMA
	else if (pWork->obj_2d) {
		BOOL	sts;
		sts = ObjAction2dAMALoadCheck(pWork->obj_2d);
		if (sts == FALSE && !(pWork->flag & OBD_OBJECT_NOWAITLOAD)) {
			// データ読み込み待機
			return;
		}
	}
#endif
#endif

	// システム前処理
	if (g_obj.ppObjPre) {
		g_obj.ppObjPre(pWork);
	}

    // オブジェクト地形、参照者チェック
    //if (!ObjObjectPauseCheck(pWork->flag)) {
	{
		objObjectColRideTouchCheck(pWork);		// オブジェクトポーズ中もチェックする
    }

	// オブジェクトテーブル(OBS_TBL_WORK)関連処理
    // 座標戻す
    pWork->pos.x -= pWork->prev_temp_ofst.x;
    pWork->pos.y -= pWork->prev_temp_ofst.y;
    pWork->pos.z -= pWork->prev_temp_ofst.z;

    if ( !ObjObjectPauseCheck(pWork->flag) || pWork->flag & OBD_OBJECT_NOPAUSE_IN ){
        // 入力
        if ( pWork->ppIn )
            pWork->ppIn(pWork);
    }
    
#if OBD_OBJECT_USE_NOEXIST
	if ((pWork->disp_flag & OBD_DISP_NOEXIST_ENABLE) && (pWork->flag & OBD_OBJECT_NOEXIST_ENABLE)) {
		pWork->flag |= OBD_OBJECT_NOEXIST;
		pWork->disp_flag |= OBD_DISP_NOEXIST;
	}
	else {
		pWork->flag &= ~OBD_OBJECT_NOEXIST;
		pWork->disp_flag &= ~OBD_DISP_NOEXIST;
	}
#endif // OBD_OBJECT_USE_NOEXIST
	
    // オブジェポーズチェック
    if ( !ObjObjectPauseCheck(pWork->flag) ){
        // オブジェクトシステムタイマーカウントダウン
        if ( pWork->vib_timer ){
			// 振動タイマー
            pWork->vib_timer = ObjTimeCountDown( pWork->vib_timer );
        }
        if ( pWork->hitstop_timer ){
			// ヒットストップタイマー
            pWork->hitstop_timer = ObjTimeCountDown( pWork->hitstop_timer );
            if ( pWork->flag & OBD_OBJECT_NO_HITSTOP )
                pWork->hitstop_timer = 0;
        }

		// ヒットストップチェック
		if ( !(g_obj.flag & OBD_OBJ_HS_FUNC_STOP) || !( pWork->hitstop_timer )){
			// 無敵タイマー
	        if (pWork->invincible_timer) {
				pWork->invincible_timer = ObjTimeCountDown( pWork->invincible_timer );
	        }
#if OBD_OBJECT_USE_NOEXIST
			// メイン処理
			if (!( pWork->flag & (OBD_OBJECT_NOFUNC | OBD_OBJECT_NOEXIST ))){
				if ( pWork->ppFunc ) {
					pWork->ppFunc(pWork);
				}
			}
#else
			// メイン処理
			if (!( pWork->flag & OBD_OBJECT_NOFUNC )){
				if ( pWork->ppFunc ) {
					pWork->ppFunc(pWork);
				}
			}
#endif // OBD_OBJECT_USE_NOEXIST
		}

#if OBD_OBJECT_USE_NOEXIST
		if (!(pWork->flag & OBD_OBJECT_NOEXIST))
#endif // OBD_OBJECT_USE_NOEXIST
		{
			// 移動処理
			if ( !(pWork->move_flag & OBD_MOVE_NOMOVE) ){
				if (pWork->ppMove) {
					pWork->ppMove(pWork);
				}
			}
		
			// 地形判定
	        if ( g_obj.flag & OBD_OBJ_COLMAP && !(pWork->move_flag & OBD_MOVE_NOCOL) &&
	        		(!(g_obj.flag & OBD_OBJ_HS_COL_STOP) || !(pWork->hitstop_timer)) ) {
				// 地形判定
				if (pWork->ppCol) {
					pWork->ppCol(pWork);
				}
				else if (g_obj.ppCollision) {
					g_obj.ppCollision(pWork);
				}
			//	pWork->ride_obj = NULL;
			//	pWork->touch_obj = NULL;
			//	// 地形判定
			//	objObjectCollision(pWork);
			//	objDebugRectDispObject(pWork);
	        }
#if OBD_OBJECT_USE_NOEXIST
		}
#endif // OBD_OBJECT_USE_NOEXIST
    }

	// オブジェクトテーブル(OBS_TBL_WORK)関連処理
    {
        
     //   if ( g_obj.flag & OBD_OBJ_COLMAP && !(pWork->move_flag & OBD_MOVE_NOCOL)){
     //       pWork->ride_obj = NULL;
     //       pWork->touch_obj = NULL;
     //       // 地形判定
     //       objObjectCollision(pWork);
     //       objDebugRectDispObject(pWork);
     //   }
        // 座標移動 (OBS_TBL_WORK 使用時の為の処理)
        pWork->pos.x += pWork->temp_ofst.x;
        pWork->pos.y += pWork->temp_ofst.y;
        pWork->pos.z += pWork->temp_ofst.z;
        
        // 保持 (OBS_TBL_WORK 使用時の為の処理)
        pWork->prev_temp_ofst.x = pWork->temp_ofst.x;
        pWork->prev_temp_ofst.y = pWork->temp_ofst.y;
        pWork->prev_temp_ofst.z = pWork->temp_ofst.z;
    }
	
#if OBD_OBJECT_USE_NOEXIST
	if (!(pWork->flag & OBD_OBJECT_NOEXIST))
#endif // OBD_OBJECT_USE_NOEXIST
	{
    // 表示関連
    // オブジェポーズチェック
    if ( !ObjObjectPauseCheck(pWork->flag) ){
        // バイヴレーション
        if ( pWork->vib_timer && !(pWork->flag & OBD_OBJECT_NO_VIB) ){
            //pWork->ofst.x += g_object_vib_tbl[ ( (pWork->vib_timer >> (FX32_SHIFT + 1) ) + 0) & 0xf ] <<FX32_SHIFT;
            //pWork->ofst.y += g_object_vib_tbl[ ( (pWork->vib_timer >> (FX32_SHIFT + 1) ) + 1) & 0xf ] <<FX32_SHIFT;
            pWork->ofst.x += g_object_vib_tbl[ ( (pWork->vib_timer >> (FX32_SHIFT + 1) ) + 0) & 0xf ];
            pWork->ofst.y += g_object_vib_tbl[ ( (pWork->vib_timer >> (FX32_SHIFT + 1) ) + 1) & 0xf ];
        }
		// 描画はobjMainに移動
    }

    // オブジェポーズチェック
    if ( !ObjObjectPauseCheck(pWork->flag) || pWork->flag & OBD_OBJECT_NOPAUSE_REC ){
		if (g_obj.flag & OBD_OBJ_RECT) {
			// その他矩形類登録・自動登録前処理
			if (pWork->ppRec) {
				pWork->ppRec(pWork);
			}

			// 矩形・地形自動登録
			if (g_obj.ppRegRecAuto) {
				g_obj.ppRegRecAuto(pWork);
			}
		}

#if 0
        // クリア
        pWork->flow.x = 0;		// ObjObjectMove へ移動
        pWork->flow.y = 0;
        pWork->flow.z = 0;
        pWork->prev_ofst.x = pWork->ofst.x;	// objObjectDrawへ移動
        pWork->prev_ofst.y = pWork->ofst.y;
        pWork->prev_ofst.z = pWork->ofst.z;
        pWork->ofst.x = 0;
        pWork->ofst.y = 0;
        pWork->ofst.z = 0;
#endif
	}

    // オブジェポーズチェック
    if ( !ObjObjectPauseCheck(pWork->flag) || pWork->flag & OBD_OBJECT_NOPAUSE_LAST ){
        // 最終処理
        if ( pWork->ppLast )
            pWork->ppLast(pWork);

    }
#if OBD_OBJECT_USE_NOEXIST
	}
#endif // OBD_OBJECT_USE_NOEXIST
	
	// システム後処理
	if (g_obj.ppObjPost) {
		g_obj.ppObjPost(pWork);
	}

#if defined(MTD_DEBUG)  // デバッグ版
    mtSetTaskBarColor( MTD_SYS_TASK_COLOR );
#endif  // #endif of #if defined(MTD_DEBUG)
}

// ================================================================
// ObjObjectExit
/*!
  オブジェクト解放

  @param pTcb [in] タスクポインタ
 */
// ================================================================
void ObjObjectExit( MTS_TASK_TCB *pTcb )
{
    OBS_OBJECT_WORK  * pWork;
    pWork = (OBS_OBJECT_WORK*)mtTaskGetTcbWork( pTcb );

    // サウンド解放
#if defined _DS
    if ( pWork->h_snd )
        NNS_SndHandleReleaseSeq( pWork->h_snd );
    if ( pWork->h_snd )
        mtSndFreeHandle( pWork->h_snd );
#endif

#if OBD_USE_ACTION3D_NN
	if (pWork->obj_3d) {
#if 1
		// モーション開放
		ObjAction3dNNMotionRelease(pWork->obj_3d);
#else
		// モーション解放
		if (pWork->obj_3d->motion) {
			amMotionDelete(pWork->obj_3d->motion);
		}
		// 共用解放
		// モーション
		// マテリアルモーション
		for (i = 0; i < AMD_MOTION_FILE_MAX; i++) {
			// モーション
			if (pWork->obj_3d->mtn_data_work[i]) {
				ObjDataRelease(pWork->obj_3d->mtn_data_work[i]);
				pWork->obj_3d->mtn_data_work[i] = NULL;
			}
			else {
				if (pWork->obj_3d->mtn[i] && !(pWork->obj_3d->flag & (OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_0 << i))) {
					mtMemFreeMain(pWork->obj_3d->mtn[i]);
				}
			}
			//obj_3d->flag &= ~(OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_0 << i);
			pWork->obj_3d->mtn[i] = NULL;

			// マテリアルモーション
			if (pWork->obj_3d->mat_mtn_data_work[i]) {
				ObjDataRelease(pWork->obj_3d->mat_mtn_data_work[i]);
				pWork->obj_3d->mat_mtn_data_work[i] = NULL;
			}
			else {
				if (pWork->obj_3d->mat_mtn[i] && !(pWork->obj_3d->flag & (OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_0 << i))) {
					mtMemFreeMain(pWork->obj_3d->mat_mtn[i]);
				}
			}
			//obj_3d->flag &= ~(OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_0 << i);
			pWork->obj_3d->mat_mtn[i] = NULL;
		}

		// モデル
		if (pWork->obj_3d->model_data_work) {
			ObjDataRelease(pWork->obj_3d->model_data_work);
			pWork->obj_3d->model_data_work = NULL;
		}
		else {
			if (pWork->obj_3d->model && !(pWork->obj_3d->flag & OBD_ACTFLAG_3D_NN_ARCHIVE)) {
				mtMemFreeMain(pWork->obj_3d->model);
			}
		}
		//obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_ARCHIVE;
		pWork->obj_3d->model = NULL;
#endif

		if (!(pWork->flag & OBD_OBJECT_NORELEASE_3D)) {
			// オブジェクト
			if (pWork->obj_3d->object) {
#if !_WII
				mtMemFreeMain(pWork->obj_3d->object);
#endif
				pWork->obj_3d->object = NULL;
			}
			// テクスチャリストバッファ
			if (pWork->obj_3d->texlistbuf) {
				mtMemFreeMain(pWork->obj_3d->texlistbuf);
				pWork->obj_3d->texlistbuf = NULL;
			}
		}
	}
#endif // #if OBD_USE_ACTION3D_NN


#if OBD_USE_ACTION3D_ES
	if (pWork->obj_3des) {
		
		// オブジェクト
		if (pWork->obj_3des->object) {
#if !_WII
			mtMemFreeMain(pWork->obj_3des->object);
#endif
			pWork->obj_3des->object	= NULL;
		}
		
		// モデル
		if (pWork->obj_3des->model_data_work) {
			ObjDataRelease(pWork->obj_3des->model_data_work);
			pWork->obj_3des->model_data_work = NULL;
		}
		else {
			if (pWork->obj_3des->model && !(pWork->obj_3des->flag & OBD_ACTFLAG_3D_ES_MODEL_ARCHIVE)) {
				mtMemFreeMain(pWork->obj_3des->model);
			}
		}
		//obj_3des->flag &= ~OBD_ACTFLAG_3D_ES_MODEL_ARCHIVE;
		pWork->obj_3des->model = NULL;
		
		
		// テクスチャリストバッファ
		if (pWork->obj_3des->texlistbuf) {
			mtMemFreeMain(pWork->obj_3des->texlistbuf);
			pWork->obj_3des->texlistbuf = NULL;
		}
		
		// ESテクスチャ
		if (pWork->obj_3des->ambtex_data_work) {
			ObjDataRelease(pWork->obj_3des->ambtex_data_work);
			pWork->obj_3des->ambtex_data_work = NULL;
		}
		else {
			if (pWork->obj_3des->ambtex && !(pWork->obj_3des->flag & OBD_ACTFLAG_3D_ES_AMBTEX_ARCHIVE)) {
				mtMemFreeMain(pWork->obj_3des->ambtex);
			}
		}
		//obj_3d->flag &= ~OBD_ACTFLAG_3D_ES_AMBTEX_ARCHIVE;
		pWork->obj_3des->ambtex	= NULL;
		
		
		// ECB
		if (pWork->obj_3des->ecb) {
			amEffectDelete(pWork->obj_3des->ecb);
			pWork->obj_3des->ecb = NULL;
		}
		
		// ESエフェクト
		if (pWork->obj_3des->eff_data_work) {
			ObjDataRelease(pWork->obj_3des->eff_data_work);
			pWork->obj_3des->eff_data_work = NULL;
		}
		else {
			if (pWork->obj_3des->eff && !(pWork->obj_3des->flag & OBD_ACTFLAG_3D_ES_EFF_ARCHIVE)) {
				mtMemFreeMain(pWork->obj_3des->eff);
			}
		}
		//obj_3d->flag &= ~OBD_ACTFLAG_3D_ES_EFF_ARCHIVE;
		pWork->obj_3des->eff	= NULL;
	}
#endif // #if OBD_USE_ACTION3D_ES

#if OBD_USE_ACTION2D_AMA
	if (pWork->obj_2d) {
		if (pWork->obj_2d->act) {
			AoActDelete(pWork->obj_2d->act);
			pWork->obj_2d->act = NULL;
		}
	}
#endif // #if OBD_USE_ACTION2D_AMA


#if defined _DS
    // VRAMアドレス開放
    ObjObjectVramRelease(pWork);

#if OBD_USE_ACTION2D
    // 2Dデータ解放
    if ( pWork->obj_2d ){
        // パレットアニメ終了
        if ( pWork->flag & OBD_OBJECT_PLT_ANIME )
            EfSpritePltAnimeEnd( pWork->obj_2d->act_spr.plt_ofst_no[0] );
        // パレット解放
        if ( pWork->flag & OBD_OBJECT_PLT_B )
            ObjPaletteRelease( (u8)(pWork->obj_2d->act_spr.plt_ofst_no[0] +0x10));
        else
            ObjPaletteRelease( (u8)pWork->obj_2d->act_spr.plt_ofst_no[0] );
        
        // 使用メモリ解放
        if ( pWork->obj_2d->bac_data_work ){
            ObjDataRelease( pWork->obj_2d->bac_data_work );
        }else{
            // 管理未登録オブジェクトの場合、即時アクション構造体から解放する
            if ( pWork->obj_2d->act_spr.act.bac_addr && !(pWork->obj_2d->flag & OBD_ACTFLAG_2D_ARCHIVE) ){
                mtMemFreeMain((void*)pWork->obj_2d->act_spr.act.bac_addr);
            }
        }

		// 解凍・転送分離ワーク開放
		if (pWork->obj_2d->act_uncomp) {
			if (pWork->obj_2d->flag & OBD_ACTFLAG_FREE_UNCOMP) {
				mtMemFreeMain(pWork->obj_2d->act_uncomp);
			}
			// バッファ開放
			mtMemFreeMain(pWork->obj_2d->act_uncomp->cha_uncomp);
		}
    }
#endif // #if OBD_USE_ACTION2D

#if OBD_USE_ACTION3D_NNS
    // 3Dオブジェクト読み込み済みなら解放
    if ( pWork->obj_3d ){
        MTS_ACTION3D *pAct;
        pAct= &pWork->obj_3d->act_3d.a3d;

        // 解放
        mtAct3dReleaseStructAll( pAct );
        
        // 共用解放
        if ( pWork->obj_3d->model_data_work ){
            // 共有データの最終解放(アーカイブからの場合はusNumは32768から)
            if ( pWork->obj_3d->model_data_work->num == 1)
                // Textureなど解放
                NNS_G3dResDefaultRelease( pWork->obj_3d->model );
            // 共用データ解放
            ObjDataRelease( pWork->obj_3d->model_data_work );
            pWork->obj_3d->model = NULL; 
        }else{
            // 個人解放
            if ( pWork->obj_3d->model && !(pWork->obj_3d->flag & OBD_ACTFLAG_3D_ARCHIVE)){
                // Textureなど解放
                NNS_G3dResDefaultRelease( pWork->obj_3d->model );
                mtMemFreeMain(pWork->obj_3d->model);
                pWork->obj_3d->model = NULL; 
            }
        }

        for ( i = 0; i < MTE_ACT3D_NNS_ANIM_MAX; ++i ){
            // 共用解放
            if ( pWork->obj_3d->anime_data_work[i] ){
                // 共有データの最終解放(アーカイブからの場合はusNumは32768から)
                if ( pWork->obj_3d->anime_data_work[i]->num == 1)
                    // 現在内部的に処理は無い
                    NNS_G3dResDefaultRelease( pWork->obj_3d->anime_data_work[i] );
                ObjDataRelease( pWork->obj_3d->anime_data_work[i] );
                pWork->obj_3d->anime[i] = NULL;
            }else{
                // 個人解放
                if ( pWork->obj_3d->anime[i] && !(pWork->obj_3d->flag & (OBD_ACTFLAG_3D_ARCHIVE_CA << i))){
                    // 現在内部的に処理は無い
                    NNS_G3dResDefaultRelease( pWork->obj_3d->anime_data_work[i] );
                    mtMemFreeMain(pWork->obj_3d->anime[i]);
                    pWork->obj_3d->anime[i] = NULL;
                }
            }
        }
       
    }
#endif // #if OBD_USE_ACTION3D_NNS

#if OBD_USE_ACTION3D_1M1S
    // 1M1Sモデル解放
    if ( pWork->obj_s3d ){
        OBS_ACTION3D_SIMPLE_WORK * pSimpleTemp = pWork->obj_s3d;
        MTS_ACTION3D *pAct;
        for(;;){
            pAct= &pSimpleTemp->act_s3d.a3d;

            // 解放、内容なし
            mtAct3dReleaseStructAll( pAct );

            // 共用解放
            if ( pSimpleTemp->model_data_work ){
                // 共有データの最終解放チェック(アーカイブからの場合はusNumは32768から)
                if ( pSimpleTemp->model_data_work->num == 1)
                    // Textureなど解放
                    NNS_G3dResDefaultRelease( pSimpleTemp->model );

                // 共用データ解放
                ObjDataRelease( pSimpleTemp->model_data_work );
                pSimpleTemp->model = NULL;
            }else{
                // 個人解放、共用データポインタをもたない
                if ( pSimpleTemp->model && !(pSimpleTemp->flag & OBD_ACTFLAG_3D_ARCHIVE)){
                    // Textureなど解放
                    NNS_G3dResDefaultRelease( pSimpleTemp->model );
                    // データ解放
                    mtMemFreeMain(pSimpleTemp->model);
                    pSimpleTemp->model = NULL;
                }
            }
            if ( pSimpleTemp->next )
                pSimpleTemp = pSimpleTemp->next;
            else
                break;
        }
    }
#endif // #if OBD_USE_ACTION3D_1M1S

#if OBD_USE_ACTION3D_SPR
    // ビルボード解放
    if ( pWork->obj_3dspr ){
        // 共用解放
        if ( pWork->obj_3dspr->bac_data_work ){
            // 共用データ解放
            ObjDataRelease( pWork->obj_3dspr->bac_data_work );
            pWork->obj_3dspr->bac = NULL;
        }else{
            // 個人解放、共用データポインタをもたない
            if ( pWork->obj_3dspr->bac && !(pWork->obj_3dspr->flag & OBD_ACTFLAG_3D_ARCHIVE)){
                // データ解放
                mtMemFreeMain(pWork->obj_3dspr->bac);
                pWork->obj_3dspr->bac = NULL;
            }
        }

		// 解凍・転送分離ワーク開放
		if (pWork->obj_3dspr->act_uncomp) {
			if (pWork->obj_3dspr->flag & OBD_ACTFLAG_FREE_UNCOMP) {
				mtMemFreeMain(pWork->obj_3dspr->act_uncomp);
			}
			// バッファ開放
			mtMemFreeMain(pWork->obj_3dspr->act_uncomp->cha_uncomp);
		}
    }
#endif // #if OBD_USE_ACTION3D_SPR

#if OBD_USE_ACTION3D_SS
	// ソフトウェアスプライト開放
    if ( pWork->obj_3dss ){
        // 共用解放
        if ( pWork->obj_3dss->bac_data_work ) {
            // 共用データ解放
			ObjDataRelease( pWork->obj_3dss->bac_data_work );
			pWork->obj_3dss->bac = NULL;
        }
        else {
            // 個人解放、共用データポインタをもたない
            if ( pWork->obj_3dss->bac && !(pWork->obj_3dss->flag & OBD_ACTFLAG_3D_ARCHIVE)){
                // データ解放
                mtMemFreeMain(pWork->obj_3dss->bac);
                pWork->obj_3dss->bac = NULL;
            }
        }

		// 解凍・転送分離ワーク開放
		if (pWork->obj_3dss->act_uncomp) {
			if (pWork->obj_3dss->flag & OBD_ACTFLAG_FREE_UNCOMP) {
				mtMemFreeMain(pWork->obj_3dss->act_uncomp);
			}
			// バッファ開放
			mtMemFreeMain(pWork->obj_3dss->act_uncomp->cha_uncomp);
		}
    }
#endif // #if OBD_USE_ACTION3D_SS

#if OBD_USE_ACTION3D_POLY
	// ポリゴンアクション解放
    if ( pWork->obj_3dpoly ){
        // 共用解放
        if ( pWork->obj_3dpoly->plm_data_work ) {
            // 共用データ解放
			ObjDataRelease( pWork->obj_3dpoly->plm_data_work );
			pWork->obj_3dpoly->plm = NULL;
        }
        else {
            // 個人解放、共用データポインタをもたない
            if ( pWork->obj_3dpoly->plm && !(pWork->obj_3dpoly->flag & OBD_ACTFLAG_3D_ARCHIVE)){
                // データ解放
                mtMemFreeMain(pWork->obj_3dpoly->plm);
                pWork->obj_3dpoly->plm = NULL;
            }
        }

		// PLAアクション開放
		IzPolyActReleaseStruct(&pWork->obj_3dpoly->act_poly);

#if 0
		// 解凍・転送分離ワーク開放
		if (pWork->obj_3dpoly->act_uncomp) {
			if (pWork->obj_3dpoly->flag & OBD_ACTFLAG_FREE_UNCOMP) {
				mtMemFreeMain(pWork->obj_3dpoly->act_uncomp);
			}
			// バッファ開放
			mtMemFreeMain(pWork->obj_3dpoly->act_uncomp->cha_uncomp);
		}
#endif
    }
#endif // OBD_USE_ACTION3D_POLY

#if OBD_USE_ACTION3D_SMA
	// 3D SMA 開放
	if (pWork->obj_3dsma) {
		// SMM
		if (pWork->obj_3dsma->smm_data_work) {
			// 共用データ解放
			ObjDataRelease(pWork->obj_3dsma->smm_data_work);
			pWork->obj_3dsma->smm = NULL;
		}
		else {
			// 個人解放、共用データポインタをもたない
			if (pWork->obj_3dsma->smm && !(pWork->obj_3dsma->flag & OBD_ACTFLAG_SMA_ARCHIVE_SMM)) {
				// データ解放
				mtMemFreeMain(pWork->obj_3dsma->smm);
				pWork->obj_3dsma->smm = NULL;
			}
		}
		// SMG
		if (pWork->obj_3dsma->smg_data_work) {
			// 共用データ解放
			ObjDataRelease(pWork->obj_3dsma->smg_data_work);
			pWork->obj_3dsma->smg = NULL;
		}
		else {
			// 個人解放、共用データポインタをもたない
			if (pWork->obj_3dsma->smg && !(pWork->obj_3dsma->flag & OBD_ACTFLAG_SMA_ARCHIVE_SMG)) {
				// データ解放
				mtMemFreeMain(pWork->obj_3dsma->smg);
				pWork->obj_3dsma->smg = NULL;
			}
		}
		// SMP
		if (pWork->obj_3dsma->smp_data_work) {
			// 共用データ解放
			ObjDataRelease(pWork->obj_3dsma->smp_data_work);
			pWork->obj_3dsma->smp = NULL;
		}
		else {
			// 個人解放、共用データポインタをもたない
			if (pWork->obj_3dsma->smp && !(pWork->obj_3dsma->flag & OBD_ACTFLAG_SMA_ARCHIVE_SMP)) {
				// データ解放
				mtMemFreeMain(pWork->obj_3dsma->smp);
				pWork->obj_3dsma->smp = NULL;
			}
		}
		// SMC
		if (pWork->obj_3dsma->smc_data_work) {
			// 共用データ解放
			ObjDataRelease(pWork->obj_3dsma->smc_data_work);
			pWork->obj_3dsma->smc = NULL;
		}
		else {
			// 個人解放、共用データポインタをもたない
			if (pWork->obj_3dsma->smc && !(pWork->obj_3dsma->flag & OBD_ACTFLAG_SMA_ARCHIVE_SMC)) {
				// データ解放
				mtMemFreeMain(pWork->obj_3dsma->smc);
				pWork->obj_3dsma->smc = NULL;
			}
		}

		// SMAアクション開放
		if (pWork->obj_3dsma->act_sma) {
			mtSmaExitObject(pWork->obj_3dsma->act_sma);
			pWork->obj_3dsma->act_sma = NULL;
		}

		// 解凍・転送分離ワーク開放
		if (pWork->obj_3dsma->act_uncomp && pWork->obj_3dsma->flag & OBD_ACTFLAG_FREE_UNCOMP) {
			mtMemFreeMain(pWork->obj_3dsma->act_uncomp);
		}
	}
#endif // #if OBD_USE_ACTION3D_SMA
#endif // #if defined _DS



    // 地形データファイル解放
    if ( pWork->col_work ){
        // それぞれ共有データか、単独読み込みデータかチェックして解放する
        if ( pWork->col_work->diff_data_work )
            ObjDataRelease( pWork->col_work->diff_data_work );
        else if ( pWork->col_work->obj_col.diff_data && !(pWork->col_work->obj_col.flag & OBD_COLOBJ_NOFREE_DIFF_DATA))
            mtMemFreeMain( pWork->col_work->obj_col.diff_data);
            
        if ( pWork->col_work->dir_data_work )
            ObjDataRelease( pWork->col_work->dir_data_work );
        else if ( pWork->col_work->obj_col.dir_data && !(pWork->col_work->obj_col.flag & OBD_COLOBJ_NOFREE_DIR_DATA))
            mtMemFreeMain( pWork->col_work->obj_col.dir_data);

        if ( pWork->col_work->attr_data_work )
            ObjDataRelease( pWork->col_work->attr_data_work );
        else if ( pWork->col_work->obj_col.attr_data && !(pWork->col_work->obj_col.flag & OBD_COLOBJ_NOFREE_ATTR_DATA))
            mtMemFreeMain( pWork->col_work->obj_col.attr_data);
    }

    
    // ワーク解放
#if 0
	if ( pWork->flag & ( OBD_OBJECT_FREE_2D | OBD_OBJECT_FREE_3D | OBD_OBJECT_FREE_3DS | OBD_OBJECT_FREE_3DSP | 
    				OBD_OBJECT_FREE_3DSS | OBD_OBJECT_FREE_3DSMA | OBD_OBJECT_FREE_COL | OBD_OBJECT_FREE_HIT )){
#else
	if ( pWork->flag & ( OBD_OBJECT_FREE_2D | OBD_OBJECT_FREE_3D | OBD_OBJECT_FREE_3DES | 
						 OBD_OBJECT_FREE_COL | OBD_OBJECT_FREE_HIT )){
#endif

		
        // 3Dワーク解放
		if (pWork->flag & OBD_OBJECT_FREE_3D) {
			if (pWork->obj_3d) {
				mtMemFreeMain(pWork->obj_3d);
			}
        }
		
		// ESワーク解放
		if (pWork->flag & OBD_OBJECT_FREE_3DES) {
			if (pWork->obj_3des) {
				mtMemFreeMain(pWork->obj_3des);
			}
		}

		// 2D AMAワーク開放
		if (pWork->flag & OBD_OBJECT_FREE_2D) {
			if (pWork->obj_2d) {
				mtMemFreeMain(pWork->obj_2d);
			}
		}

#if defined _DS
#if OBD_USE_ACTION2D
        // 2Dワーク解放
        if ( pWork->flag & OBD_OBJECT_FREE_2D){
            if ( pWork->obj_2d )
                mtMemFreeMain( pWork->obj_2d );
        }
#endif
#if OBD_USE_ACTION3D_NNS
        // 3Dワーク解放
        if ( pWork->flag & OBD_OBJECT_FREE_3D){
            if ( pWork->obj_3d )
                mtMemFreeMain( pWork->obj_3d );
        }
#endif
#if OBD_USE_ACTION3D_1M1S
        // 3Dシンプルワーク解放
        if ( pWork->flag & OBD_OBJECT_FREE_3DS){
            if ( pWork->obj_s3d )
                mtMemFreeMain( pWork->obj_s3d );
        }
#endif
#if OBD_USE_ACTION3D_SPR
        // 3Dスプライトワーク解放
        if ( pWork->flag & OBD_OBJECT_FREE_3DSP){
            if ( pWork->obj_3dspr )
                mtMemFreeMain( pWork->obj_3dspr );
        }
#endif
#if OBD_USE_ACTION3D_SS
        // ソフトウェアスプライトワーク解放
        if ( pWork->flag & OBD_OBJECT_FREE_3DSS){
            if ( pWork->obj_3dss )
                mtMemFreeMain( pWork->obj_3dss );
        }
#endif // #if OBD_USE_ACTION3D_SS
#if OBD_USE_ACTION3D_POLY
		// OBD_USE_ACTION3D_POLY では現状自動ワーク取得をしていない
#endif // #if OBD_USE_ACTION3D_POLY
#if OBD_USE_ACTION3D_SMA
        // 3D SMAワーク解放
		if ( pWork->flag & OBD_OBJECT_FREE_3DSMA) {
			if ( pWork->obj_3dsma )
				mtMemFreeMain( pWork->obj_3dsma );
		}
#endif // #if OBD_USE_ACTION3D_SMA
#endif // #if defined _DS



        // 地形ワーク解放
        if ( pWork->flag & OBD_OBJECT_FREE_COL){
            if ( pWork->col_work ){
                mtMemFreeMain( pWork->col_work );
            }
        }
        // 当たり矩形ワーク解放
        if ( pWork->flag & OBD_OBJECT_FREE_HIT){
			if (pWork->rect_work) {
				mtMemFreeMain(pWork->rect_work);
			}
		}
    }
    // 拡張ワークの解放チェック
    if ( pWork->ex_work ) {
        if ( pWork->flag & OBD_OBJECT_FREE_EX ) {
            mtMemFreeMain( pWork->ex_work );
		}
	}
    // テーブルワークの解放チェック
#if 0 // ◆一旦カット
    if ( pWork->tbl_work ) {
        if ( pWork->flag & OBD_OBJECT_FREE_TBL ){
            ObjTblWorkRelease( pWork->tbl_work );
            mtMemFreeMain( pWork->tbl_work );
        }
	}
#endif

	// オブジェクト システム登録開放
	ObjObjectRevokeObject(pWork);
}

// ================================================================
// 標準関数
// ================================================================
// ================================================================
// ObjObjectViewOutCheck
/*!
  座標画面外チェック

  @param pWork [in] オブジェクトワークポインタ

  @return 0 画面内、 1画面外
 */
// ================================================================
s32 ObjObjectViewOutCheck( OBS_OBJECT_WORK * pWork )
{
    return ObjViewOutCheck( pWork->pos.x, pWork->pos.y,
                            pWork->view_out_ofst,
                            pWork->view_out_ofst_plus[OBD_LEFT  ],
                            pWork->view_out_ofst_plus[OBD_TOP   ],
                            pWork->view_out_ofst_plus[OBD_RIGHT ],
                            pWork->view_out_ofst_plus[OBD_BOTTOM] );
}

// ================================================================
// 矩形
// ================================================================
// ================================================================
// ObjObjectRectRegist
/*!
  オブジェクト矩形登録

  @param pWork [in] オブジェワークポインタ
  @param pRec  [in] 登録矩形ポインタ
 */
// ================================================================
void ObjObjectRectRegist( OBS_OBJECT_WORK *pWork, OBS_RECT_WORK* pRec )
{
    // 死亡中
    if ( pWork->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST))
        return;
    if ( !(g_obj.flag & OBD_OBJ_RECT) )
        return;
    if ( pWork->flag & OBD_OBJECT_NOHIT )
        return;
    
    // データセット
    pRec->parent_obj = pWork;
    
    //pRec->flag &= ~OBD_RECT_NOHIT;
    
    pRec->flag &= ~( OBD_RECT_HFLIP | OBD_RECT_VFLIP );

    // 重力反転時の反転設定
    if ( ObjObjectDirFallReverseCheck(pWork->dir_fall) ){
        pRec->flag ^= OBD_RECT_VFLIP;
        pRec->flag ^= OBD_RECT_HFLIP;
    }
    
    // 無事登録
    ObjRectRegist( pRec );

}

// ================================================================
// ObjObjectGetRectBuf
/*!
	当り用バッファ取得

	@param pWork			[io] オブジェクトワークポインタ
	@param rect_work		[io] 矩形ワークバッファポインタ（NULLの場合、自動的にメモリを確保する、解放も自動で行う）
	@param rect_num			[in] 登録バッファ数 pRectがNULLの場合は取得するバッファ数 最大 OBD_OBJ_RECT_MAX まで

	@note
		既にObjObjectGetRectBufで取得したバッファがある場合は先に開放する。\n
		ユーザー設定のバッファの場合は何もしないで戻る
 */
// ================================================================
void ObjObjectGetRectBuf(OBS_OBJECT_WORK* pWork, struct _OBS_RECT_WORK *rect_work, u16 rect_num)
{
//	if (!rect_num || rect_num > OBD_OBJ_RECT_MAX) {
	if ((u16)(rect_num-1) > OBD_OBJ_RECT_MAX-1) {
		MTM_ASSERT(0);
		return;
	}

	if (pWork->rect_work) {
		if (!(pWork->flag & OBD_OBJECT_FREE_HIT)) {
#if defined (MTD_DEBUG)
			OS_Printf("objObject.c:ObjObjectGetRectBuf Warning! OBS_OBJECT_WORK->rect_work != NULL\n");
#endif // #if defined (MTD_DEBUG)
			return;
		}

		// バッファ開放
		ObjObjectReleaseRectBuf(pWork);
	}

	// 地形ワークチェック
	if (rect_work == NULL) {
		// メモリ取得
		rect_work = (OBS_RECT_WORK*)mtMemAllocMain(sizeof(OBS_RECT_WORK) * rect_num);
		MI_CpuClear8(rect_work, sizeof(OBS_RECT_WORK) * rect_num);
		pWork->flag |= OBD_OBJECT_FREE_HIT;
	}
	pWork->rect_num			= rect_num;
	pWork->rect_work		= rect_work;
}

// ================================================================
// ObjObjectReleaseRectBuf
/*!
	当り用バッファ開放

	@param pWork			[io] オブジェクトワークポインタ
 */
// ================================================================
void ObjObjectReleaseRectBuf(OBS_OBJECT_WORK* pWork)
{
	if ((pWork->flag & OBD_OBJECT_FREE_HIT) && pWork->rect_work) {
		mtMemFreeMain(pWork->rect_work);
		pWork->rect_work = NULL;
		pWork->flag &= ~OBD_OBJECT_FREE_HIT;
	}
}

// ================================================================
// ObjObjectSetRectWork
/*!
	当りワーク設定

	@param	pWork		[io] オブジェクトワークポインタ
	@param	rect_work	[in] 矩形ワーク

  @note ワークには基本的な攻撃、防御設定が行われるので\n
        特殊な設定のものはこの関数の後で設定する
 */
// ================================================================
void ObjObjectSetRectWork(OBS_OBJECT_WORK* pWork, OBS_RECT_WORK *rect_work)
{
	// 親設定
	rect_work->parent_obj = pWork;

	// 基本的な設定を行う
	rect_work->group_no			= OBD_RECT_GROUP_NO_1;
	rect_work->target_g_flag	= OBD_RECT_TARGET_G_FLAG_1;	// 同じグループにあたる
//	rect_work->user_flag		= (OBD_RECT_USE1 | OBD_RECT_USE1_A);
	rect_work->hit_power		= OBD_RECT_HIT_POWER_DEFAULT;
	rect_work->def_power		= OBD_RECT_DEF_POWER_DEFAULT;
	rect_work->hit_flag			= (OBD_HIT_NORMAL);
	rect_work->def_flag			= (OBD_HIT_BODY);
}

#if 0
// ================================================================
// ObjObjectSetRectWorkIndex
/*!
	インデックス指定当り設定

	@param	pWork			[io] オブジェクトワークポインタ
	@param	index			[in] 矩形登録番号
	@param	rect_set_act	[in] TRUE アクションデータから矩形データを取得、FALSE 自動で矩形を設定しない

  @note ワークには基本的な攻撃、防御設定が行われるので\n
        特殊な設定のものはこの関数の後で設定する\n
        ObjObjectRectBufSetで設定済みのバッファに対して情報を設定します。\n
  		indexはrect_num(ObjObjectRectBufSet指定)を超えることは出来ません。
 */
// ================================================================
void ObjObjectSetRectWorkIndex(OBS_OBJECT_WORK* pWork, u16 index, BOOL rect_set_act)
{
	OBS_RECT_WORK	*rect_work;
	u32				rect_act_flag;

	if (index >= pWork->rect_num || !pWork->rect_work) {
		MTM_ASSERT(0);
		return;
	}

	rect_work = pWork->rect_work + index;

	// 設定
	ObjObjectSetRect(pWork, rect_work);

	// アクションデータからの矩形情報設定フラグ
	rect_act_flag = (u32)(0x01 << index);
	if (rect_set_act) {
		pWork->rect_act_flag |= rect_act_flag;
	}
	else {
		pWork->rect_act_flag &= ~rect_act_flag;
	}
}

// ================================================================
// ObjObjectRectSet
/*!
  当り設定

  @param pObj     [io] オブジェクトワークポインタ
  @param pRect    [io] 矩形ワークポインタ（NULLの場合、自動的にメモリを確保する、解放も自動で行う）
  @param usIndex  [in] 矩形登録番号（アクションの矩形番号でもある）
  @param bAction  [in] TRUE アクションデータから矩形データを取得、FALSE 自動で矩形を設定しない

  @note ワークには基本的な攻撃、防御設定が行われるので\n
        特殊な設定のものはこの関数の後で設定する
  
 */
// ================================================================
void ObjObjectRectSet ( OBS_OBJECT_WORK* pObj, OBS_RECT_WORK * pRect, u16 usIndex, BOOL bAction )
{
	MTM_ASSERT(usIndex < OBD_OBJ_RECT_MAX);	// 最大使用数チェックのみ

    // 地形ワークチェック
    if ( pRect == NULL ){
        if ( pObj->rect_work[usIndex] ){
            pRect = pObj->rect_work[usIndex];
        } else{
            // メモリ取得
            pRect = mtMemAllocMain( sizeof(OBS_RECT_WORK) );
            MI_CpuClear8( pRect, sizeof(OBS_RECT_WORK));
            pObj->flag |= OBD_OBJECT_FREE_HIT;
        }
    }
    pObj->rect_work[usIndex] = pRect;
    pWork->rect_act_flag[usIndex] = (u16)bAction;

    // 親設定
    pObj->rect_work[usIndex]->parent_obj  = pObj;
    
    // 基本的な設定を行う
    pObj->rect_work[usIndex]->user_flag = (OBD_RECT_USE1 | OBD_RECT_USE1_A);
    pObj->rect_work[usIndex]->hit_power = OBD_RECT_HIT_POWER_DEFAULT;
    pObj->rect_work[usIndex]->def_power = OBD_RECT_DEF_POWER_DEFAULT;
    pObj->rect_work[usIndex]->hit_flag = (OBD_HIT_NORMAL);
    pObj->rect_work[usIndex]->def_flag = (OBD_HIT_BODY);
    
}
#endif

// ================================================================
// ObjObjectRectGet
/*!
  当りポインタ設定

  @param pObj     [io] オブジェクトワークポインタ
  @param usIndex  [in] 矩形登録番号（アクションの矩形番号でもある）

 */
// ================================================================
OBS_RECT_WORK* ObjObjectRectGet ( OBS_OBJECT_WORK* pWork, u16 usIndex )
{
#if defined (MTD_DEBUG)
	if (usIndex >= pWork->rect_num) {
		MTM_ASSERT(0);
		return (NULL);
	}
#endif

    return (pWork->rect_work + usIndex);
}

// ================================================================
// 画面外チェック
// ================================================================
// ================================================================
// ObjViewOutCheck
/*!
  座標画面外チェック

  @param lPosX [in] チェックする座標 1:19:12
  @param lPosY [in] 
  @param sOfst [in] 画面外オフセット 1:15
  @param sLeft   [in] 画面外オフセット 1:15
  @param sTop    [in] 画面外オフセット 1:15
  @param sRight  [in] 画面外オフセット 1:15
  @param sBottom [in] 画面外オフセット 1:15

  @return 0 画面内、 1画面外
 */
// ================================================================
s32 ObjViewOutCheck( s32 lPosX, s32 lPosY, s16 sOfst, s16 sLeft, s16 sTop, s16 sRight, s16 sBottom )
{
    s32 lLeftest, lTopest;
    s32 usWidth, usHeight;
#if _DS
    s16 sLcdX = OBD_LCD_X, sLcdY = OBD_LCD_Y;
#else
    s16 sLcdX = OBD_OBJ_CLIP_LCD_X, sLcdY = OBD_OBJ_CLIP_LCD_Y;
#endif
    
    if ( g_obj.glb_scale.x != 0x1000 )
        sLcdX = (s16)FX_Mul( sLcdX, (0x2000 - g_obj.glb_scale.x));
    if ( g_obj.glb_scale.y != 0x1000 )
        sLcdY = (s16)FX_Mul( sLcdY, (0x2000 - g_obj.glb_scale.y));
        
    //if ( !sOfst )
    //    sOfst = OBD_OBJECT_DIE_OFFSET;

    if ( !(g_obj.flag & OBD_OBJ_CAMERA) )
        return 0;

#if defined _DS
    if ( g_obj.flag & OBD_OBJ_CAMERA_STICK ){
        // 上下画面吸着時
        if ( g_obj.camera[0][MTD_Y] < g_obj.camera[1][MTD_Y]){
            lLeftest = (g_obj.camera[0][MTD_X] >> FX32_SHIFT) - sOfst;
            lTopest  = (g_obj.camera[0][MTD_Y] >> FX32_SHIFT) - sOfst;
            usWidth  = sLcdX + (sOfst << 1);
            usHeight = ((g_obj.camera[1][MTD_Y] >> FX32_SHIFT)-(g_obj.camera[0][MTD_Y] >> FX32_SHIFT)) + (sLcdY) + (sOfst << 1);
        }else{
            lLeftest = (g_obj.camera[1][MTD_X] >> FX32_SHIFT) - sOfst;
            lTopest  = (g_obj.camera[1][MTD_Y] >> FX32_SHIFT) - sOfst;
            usWidth  = sLcdX + (sOfst << 1);
            usHeight = ((g_obj.camera[0][MTD_Y] >> FX32_SHIFT)-(g_obj.camera[1][MTD_Y] >> FX32_SHIFT)) + (sLcdY) + (sOfst << 1);
        }
        // オフセット加算
        lLeftest += sLeft;
        lTopest  += sTop;
        usHeight += -sTop + sBottom;
        usWidth  += -sLeft + sRight;
        
        if( (( lLeftest <= (lPosX >> FX32_SHIFT)) && (lLeftest + usWidth >= (lPosX >> FX32_SHIFT) )) &&
            (( lTopest  <= (lPosY >> FX32_SHIFT)) && (lTopest  + usHeight >= (lPosY >> FX32_SHIFT) )) ){
            // 生成範囲内
            return 0;
        }
    }else{
        // 上下離れ時
        lLeftest = (g_obj.camera[0][MTD_X] >> FX32_SHIFT) - sOfst;
        lTopest  = (g_obj.camera[0][MTD_Y] >> FX32_SHIFT) - sOfst;
        usWidth  = sLcdX + (sOfst << 1);
        usHeight = sLcdY + (sOfst << 1);
        // オフセット加算
        lLeftest += sLeft;
        lTopest  += sTop;
        usHeight += -sTop + sBottom;
        usWidth  += -sLeft + sRight;
        
        if( (( lLeftest <= (lPosX >> FX32_SHIFT)) && (lLeftest + usWidth >= (lPosX >> FX32_SHIFT) )) &&
            (( lTopest  <= (lPosY >> FX32_SHIFT)) && (lTopest  + usHeight >= (lPosY >> FX32_SHIFT) )) ){
            // 生成範囲内
            return 0;
        }
        lLeftest = (g_obj.camera[1][MTD_X] >> FX32_SHIFT) - sOfst;
        lTopest  = (g_obj.camera[1][MTD_Y] >> FX32_SHIFT) - sOfst;
        // オフセット加算
        lLeftest += sLeft;
        lTopest  += sTop;

        if( (( lLeftest <= (lPosX >> FX32_SHIFT)) && (lLeftest + usWidth >= (lPosX >> FX32_SHIFT) )) &&
            (( lTopest  <= (lPosY >> FX32_SHIFT)) && (lTopest  + usHeight >= (lPosY >> FX32_SHIFT) )) ){
            // 生成範囲内
            return 0;
        }

    }
#else	// #if defined _DS
	//lLeftest = (g_obj.camera[0][MTD_X] >> FX32_SHIFT) - sOfst;
	//lTopest  = (g_obj.camera[0][MTD_Y] >> FX32_SHIFT) - sOfst;
	lLeftest = (g_obj.clip_camera[MTD_X] >> FX32_SHIFT) - sOfst;
	lTopest  = (g_obj.clip_camera[MTD_Y] >> FX32_SHIFT) - sOfst;
	usWidth  = sLcdX + (sOfst << 1);
	usHeight = sLcdY + (sOfst << 1);

	// オフセット加算
	lLeftest += sLeft;
	lTopest  += sTop;
	usHeight += -sTop + sBottom;
	usWidth  += -sLeft + sRight;
        
	if ( (( lLeftest <= (lPosX >> FX32_SHIFT)) && (lLeftest + usWidth >= (lPosX >> FX32_SHIFT) )) &&
			(( lTopest  <= (lPosY >> FX32_SHIFT)) && (lTopest  + usHeight >= (lPosY >> FX32_SHIFT) )) ){
		// 生成範囲内
		return 0;
	}
#endif	// #if defined _DS
    
   // OS_TPrintf( "■範囲外死亡\n" );
    // 範囲外なのでTRUE
    return 1;
}

// ================================================================
// Utility
// ================================================================
// ================================================================
// ObjObjectSpdDirFall
/*!
  重力方向計算

  @param sSpdX     [io] 速度Xポインタ
  @param sSpdY     [io] 速度Yポインタ
  @param ucDirFall [in] 重力角度
 */
// ================================================================
void ObjObjectSpdDirFall( s32 *sSpdX, s32 *sSpdY, u16 ucDirFall )
{
	s32			temp_x = 0, temp_y = 0;
	float		sin, cos;
	float		x_sin, x_cos, y_sin, y_cos;

	if (sSpdX) {
		temp_x = *sSpdX;
	}
	if (sSpdY) {
		temp_y = *sSpdY;
	}

	sin = nnSin(ucDirFall);
	cos = nnCos(ucDirFall);
	x_sin = temp_x * sin;
	x_cos = temp_x * cos;
	y_sin = temp_y * sin;
	y_cos = temp_y * cos;

	if (sSpdX) {
		*sSpdX = (s32)nnRoundOff(x_cos - y_sin);
	}
	if (sSpdY) {
		*sSpdY = (s32)nnRoundOff(x_sin + y_cos);
	}
#if 0

    MtxFx33     mRot;
    VecFx32     vPos;
    s32 sTempX = 0, sTempY = 0;
    
    if ( sSpdX )
        sTempX = *sSpdX;
    if ( sSpdY )
        sTempY = *sSpdY;
    
    MTX_Identity33( &mRot );
    MTX_RotZ33( &mRot, mtMathSin( ucDirFall ), mtMathCos( ucDirFall) );
    VEC_Set(&vPos, sTempX, sTempY , 0 );
    MTX_MultVec33( &vPos, &mRot, &vPos );

    if ( sSpdX )
        *sSpdX = (vPos.x );
    if ( sSpdY )
        *sSpdY = (vPos.y );
#endif
}

// ================================================================
// ObjObjectDirFallReverseCheck
/*!
  重力反転Check

  @return 1 反転中、 0 反転以外
 */
// ================================================================
u32 ObjObjectDirFallReverseCheck( u16 ucDirFall )
{
    if ( ucDirFall > 0x6000 && ucDirFall < 0xa000){
        return 1;
    }
    return 0;
}

// ================================================================
// ObjTimeCountGet
/*!
  カウント値を処理速度にあわせる関数（スロー効果などを使いたい時は全てのタイマーで使用必須）
 
  @param time [in] カウント値 1:19:12
 
  @return   現在の処理速度にあわせたカウント値
 
 */
// ================================================================
fx32 ObjTimeCountGet( fx32 count )
{
    if ( g_obj.speed != 0x1000 )
        return FX_Mul(count, g_obj.speed);
    return count;
}

// ================================================================
// ObjTimeCountDown
/*!
  タイマーを1(0x1000)ずつカウントダウンする
 
  @param time [in] タイマー値 1:19:12
 
  @return   現在の処理速度にあわせて0x1000カウントダウンしたタイマー値
 
 */
// ================================================================
fx32 ObjTimeCountDown( fx32 timer )
{
    if ( g_obj.speed == 0x1000 )
        timer -= 0x1000;
    else
        timer -= FX_Mul(0x1000, g_obj.speed);	// フレームレートの影響あり◆

    if ( timer < 0)
        timer = 0;
    return timer;
}

// ================================================================
// ObjTimeCountUp
/*!
  タイマーを1(0x1000)ずつカウントアップする
 
  @param time [in] タイマー値 1:19:12
 
  @return   現在の処理速度にあわせて0x1000カウントアップしたタイマー値
 
 */
// ================================================================
fx32 ObjTimeCountUp( fx32 timer )
{
    if ( g_obj.speed == 0x1000 )
        timer += 0x1000;
    else
        timer += FX_Mul(0x1000, g_obj.speed);

    if ( timer < 0)
        timer = 0;
    return timer;
}

// ================================================================
// ObjTimeCountDownF
/*!
  タイマーを1.fずつカウントダウンする
 
  @param time [in] タイマー値
 
  @return   現在の処理速度にあわせて1.fカウントダウンしたタイマー値
 
 */
// ================================================================
float ObjTimeCountDownF(float timer)
{
	if ( g_obj.speed == 0x1000 ) {
		timer -= 1.f;
	}
	else {
		timer -= 1.f * g_obj.speed / FX32_ONE;	// フレームレートの影響あり◆
	}

	if (timer < 0.f) {
		timer = 0.f;
	}
	return (timer);
}

// ================================================================
// ObjTimeCountUpF
/*!
  タイマーを1.fずつカウントアップする
 
  @param time [in] タイマー値
 
  @return   現在の処理速度にあわせて1.fカウントアップしたタイマー値
 
 */
// ================================================================
float ObjTimeCountUpF(float timer)
{
	if ( g_obj.speed == 0x1000 ) {
		timer += 1.f;
	}
	else {
		timer += 1.f * (float)g_obj.speed / (float)FX32_ONE;	// フレームレートの影響あり◆
	}

	if (timer < 0.f) {
		timer = 0.f;
	}
	return (timer);
}

// ================================================================
// ObjObjectMapOutCheck
/*!
  マップ外チェック

  @param pWork [in] オブジェクトワークポインタ

  @return 0 マップ内、 1 マップ外
 */
// ================================================================
s32 ObjObjectMapOutCheck( OBS_OBJECT_WORK * pWork )
{
    return ObjMapOutCheck( pWork->pos.x, pWork->pos.y,
                            pWork->view_out_ofst,
                            pWork->view_out_ofst_plus[OBD_LEFT  ],
                            pWork->view_out_ofst_plus[OBD_TOP   ],
                            pWork->view_out_ofst_plus[OBD_RIGHT ],
                            pWork->view_out_ofst_plus[OBD_BOTTOM] );
}

// ================================================================
// ObjMapOutCheck
/*!
  マップ外チェック

  @param lPosX [in] チェックする座標 1:19:12
  @param lPosY [in] 
  @param sOfst [in] マップ外オフセット 1:15
  @param sLeft   [in] マップ外オフセット 1:15
  @param sTop    [in] マップ外オフセット 1:15
  @param sRight  [in] マップ外オフセット 1:15
  @param sBottom [in] マップ外オフセット 1:15

  @return 0 マップ内、 1 マップ外
 */
// ================================================================
s32 ObjMapOutCheck( s32 lPosX, s32 lPosY, s16 sOfst, s16 sLeft, s16 sTop, s16 sRight, s16 sBottom )
{
    s32 lLeftest, lTopest;
    s32 usWidth, usHeight;

#ifndef _DS
	UNREFERENCED_PARAMETER(sLeft);
	UNREFERENCED_PARAMETER(sTop);
	UNREFERENCED_PARAMETER(sRight);
	UNREFERENCED_PARAMETER(sBottom);
#endif
    
    if ( g_obj.flag & OBD_OBJ_COL_BLOCK ){
        // ブロック地形の時
        const OBS_BLOCK_COLLISION* pCol;
        pCol = ObjGetBlockCollision();
        
        // 地形データなし
        if ( pCol ==NULL )
            return FALSE;
        
        lLeftest = pCol->left - sOfst;
        lTopest  = pCol->top - sOfst;
        usWidth  = ( pCol->right - pCol->left) + (sOfst << 1);
        usHeight = ( pCol->bottom - pCol->top) + (sOfst << 1);
    }else{
        // ブロック地形の時
        const OBS_DIFF_COLLISION* pCol;
        pCol = ObjGetDiffCollision();
        
        // 地形データなし
        if ( pCol ==NULL )
            return FALSE;
        
        lLeftest = pCol->left - sOfst;
        lTopest  = pCol->top - sOfst;
        usWidth  = ( pCol->right - pCol->left) + (sOfst << 1);
        usHeight = ( pCol->bottom - pCol->top) + (sOfst << 1);

    }

        
    if( (( lLeftest <= (lPosX >> FX32_SHIFT)) && (lLeftest + usWidth >= (lPosX >> FX32_SHIFT) )) &&
        (( lTopest  <= (lPosY >> FX32_SHIFT)) && (lTopest  + usHeight >= (lPosY >> FX32_SHIFT) )) ){
        // 生成範囲内
        return 0;
    }

    // 範囲外なのでTRUE
    return 1;
}

#if 0
// ================================================================
// ObjObjectActDsGet
/*!
  アクションポインタ取得

  @param pObj     [io] オブジェクトワークポインタ

 */
// ================================================================
MTS_ACTION_DS* ObjObjectActDsGet ( OBS_OBJECT_WORK* pObj)
{
    if ( pObj->obj_2d)
        return &pObj->obj_2d->act_spr;
    return NULL;
}
#endif

//----- Local Functions -----------------------------------------------------
// ================================================================
// オブジェクトシステム処理
// ================================================================
// ================================================================
// objMain
/*!
  オブジェクト毎フレーム実行処理

 */
// ================================================================
#if defined _DS
static void objMain()
#else
static void objMain(MTS_TASK_TCB *tcb)
#endif
{
	OBS_OBJECT_WORK	*pWork;
//	u32				disp_flag_work;

#if defined(MTD_DEBUG)  // デバッグ版
	u16				color;
    mtSetTaskBarColor(OBD_TASK_COLOR);
#endif  // #endif of #if defined(MTD_DEBUG)

#ifndef _DS
	UNREFERENCED_PARAMETER(tcb);
#endif

	// 処理高速化用 スケール算出
	g_obj.scale.x = FX_Mul(g_obj.glb_scale.x, g_obj.draw_scale.x);
	g_obj.scale.y = FX_Mul(g_obj.glb_scale.y, g_obj.draw_scale.y);
	g_obj.scale.z = FX_Mul(g_obj.glb_scale.z, g_obj.draw_scale.z);
	// 逆数
	g_obj.inv_scale.x = FX_Div(FX32_ONE, g_obj.scale.x);
	g_obj.inv_scale.y = FX_Div(FX32_ONE, g_obj.scale.y);
	g_obj.inv_scale.z = FX_Div(FX32_ONE, g_obj.scale.z);
	// 逆数
//	g_obj.inv_glb_scale.x = FX_Div(FX32_ONE, g_obj.glb_scale.x);
//	g_obj.inv_glb_scale.y = FX_Div(FX32_ONE, g_obj.glb_scale.y);
//	g_obj.inv_glb_scale.z = FX_Div(FX32_ONE, g_obj.glb_scale.z);


	// システム 前処理
	if (g_obj.ppPre) {
		g_obj.ppPre();
	}

	//if (!(g_obj.flag & (OBD_OBJ_FORCE_PAUSE|OBD_OBJ_ACTER_FORCE_PAUSE))) {
	{	// ◆オブジェクト毎にポーズレベルによってチェックするかどうか判定するようにするかも
	    // 矩形判定
	    if ( g_obj.flag & OBD_OBJ_RECT ) {
#if defined(MTD_DEBUG)  // デバッグ版
	   	 mtSetTaskBarColor(MTD_PLT_COLOR_GREEN);
#endif  // #endif of #if defined(MTD_DEBUG)

	        ObjRectCheckAllGroup();

#if defined(MTD_DEBUG)  // デバッグ版
	  	  mtSetTaskBarColor(OBD_TASK_COLOR);
#endif  // #endif of #if defined(MTD_DEBUG)
		}
	}	//!(g_obj.flag & (OBD_OBJ_FORCE_PAUSE|OBD_OBJ_ACTER_FORCE_PAUSE))


	// 描画
#if (OBD_USE_ACTION3D_NN)
	// カメラ設定
	if (g_obj.glb_camera_id >= 0) {
#if _IPHONE
		ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, g_obj.glb_camera_type, OBD_DRAW_CMD_STATE_3DNN_PRE);
		ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, g_obj.glb_camera_type, OBD_DRAW_CMD_STATE_3DNN);
#else
		ObjDraw3DNNSetCamera(g_obj.glb_camera_id, g_obj.glb_camera_type);
#endif // _IPHONE
	}
#endif

#if defined(MTD_DEBUG)  // デバッグ版
	color = 0;//0x7C00;
#endif  // #endif of #if defined(MTD_DEBUG)
	if (g_obj.ppDrawSort) {
		// ソートあり描画
	//	g_obj.ppDrawSort(&g_obj.obj_list_head, &g_obj.obj_list_tail, &g_obj.obj_draw_list_head);
		g_obj.ppDrawSort();

		for (pWork = g_obj.obj_draw_list_head; pWork; pWork = pWork->draw_next) {
#if defined(MTD_DEBUG)  // デバッグ版
			mtSetTaskBarColor((u16)(0x7C00 + color));
			color = (u16)((color + 0x0042) & 0x03FF);
#endif  // #endif of #if defined(MTD_DEBUG)

			objObjectDraw(pWork);
		}
	//	for (pWork = g_obj.obj_draw_alpha_list_head; pWork; pWork = pWork->draw_next) {
	//		objObjectDraw(pWork);
	//	}
	}
	else {
		// ソート無し描画
		for (pWork = g_obj.obj_list_head; pWork; pWork = pWork->next) {
#if defined(MTD_DEBUG)  // デバッグ版
			mtSetTaskBarColor((u16)(0x7C00 + color));
			color = (u16)((color + 0x0042) & 0x03FF);
#endif  // #endif of #if defined(MTD_DEBUG)

			objObjectDraw(pWork);
		}
		GmGmkPulleyDrawServerMain();
		GmTvxExecuteDraw();
		gmDecoDrawServerMain(NULL);
	}

#if (OBD_USE_ACTION2D_AMA)
	// 2D AMA 描画開始命令発行
	ObjDrawAction2DAMADrawStart();
#endif

#if (OBD_USE_ACTION3D_NN)
	// NN系データ描画開始命令発行
	ObjDrawNNStart();
#endif

#if defined(MTD_DEBUG)  // デバッグ版
    mtSetTaskBarColor( MTD_PLT_COLOR_DARK_YELLOW );
#endif  // #endif of #if defined(MTD_DEBUG)


//	if (!(g_obj.flag & (OBD_OBJ_FORCE_PAUSE|OBD_OBJ_ACTER_FORCE_PAUSE))) {
	if (!(g_obj.flag & (OBD_OBJ_PAUSE))) {
	    // オブジェクト地形クリア
	    ObjCollisionObjectClear();
	}

	// オブジェクトタイマ
    g_obj.timer_fx += ObjTimeCountGet( 0x1000 );

    g_obj.flag |= OBD_OBJ_TIME_MOVE;
    // 初めてチェック
    if ( g_obj.timer == (u32)(g_obj.timer_fx >>FX32_SHIFT)) {
        g_obj.flag &= ~OBD_OBJ_TIME_MOVE;
    }
    g_obj.timer = (u32)(g_obj.timer_fx >> FX32_SHIFT);

    // ポーズチェック
    if ( g_obj.flag & OBD_OBJ_PAUSE_START ){
        g_obj.flag |= OBD_OBJ_PAUSE;
    }else{
        g_obj.flag &= ~OBD_OBJ_PAUSE;
    }
//    if ( g_obj.flag & OBD_OBJ_FORCE_PAUSE_START ){
//        g_obj.flag |= OBD_OBJ_FORCE_PAUSE;
//    }else{
//        g_obj.flag &= ~OBD_OBJ_FORCE_PAUSE;
//    }
//    if ( g_obj.flag & OBD_OBJ_ACTER_FORCE_PAUSE_START ){
//        g_obj.flag |= OBD_OBJ_ACTER_FORCE_PAUSE;
//    }else{
//        g_obj.flag &= ~OBD_OBJ_ACTER_FORCE_PAUSE;
//    }

	// システム 後処理
	if (g_obj.ppPost) {
		g_obj.ppPost();
	}

#if defined(MTD_DEBUG)  // デバッグ版
    mtSetTaskBarColor( MTD_SYS_TASK_COLOR );
#endif  // #endif of #if defined(MTD_DEBUG)
}

// ================================================================
// objObjectDraw
/*!
  オブジェクト描画

  @param	pWork	[in]	オブジェクトワーク

 */
// ================================================================
static void objObjectDraw(OBS_OBJECT_WORK *pWork)
{
	u32				disp_flag_work = 0;

	if (!(pWork->flag & OBD_OBJECT_TASKCLEAR)) {
		// ヒットストップ時・オブジェクトポーズ時 アニメを止める
		if ((pWork->hitstop_timer && !(pWork->flag & OBD_OBJECT_NO_HITSTOP)) || ObjObjectPauseCheck(pWork->flag)) {
			disp_flag_work = pWork->disp_flag & OBD_DISP_STOP;	// OBD_DISP_STOPフラグ保存
			pWork->disp_flag |= OBD_DISP_STOP;
		}

		// メイン出力処理
		if (pWork->ppOut) {
			pWork->ppOut(pWork);
		}
		// サブ出力処理
		if (pWork->ppOutSub) {
			pWork->ppOutSub(pWork);
		}

		// ヒットストップ時、フラグを戻す
		if ((pWork->hitstop_timer && !(pWork->flag & OBD_OBJECT_NO_HITSTOP)) || ObjObjectPauseCheck(pWork->flag)) {
			pWork->disp_flag &= ~OBD_DISP_STOP;
			pWork->disp_flag |= disp_flag_work;					// OBD_DISP_STOPフラグ復帰
		}

		// 各種オフセット値等クリア
#if 0
		pWork->flow.x = 0;
		pWork->flow.y = 0;
		pWork->flow.z = 0;
#endif
		if (!ObjObjectPauseCheck(pWork->flag)) {
			pWork->prev_ofst.x = pWork->ofst.x;
			pWork->prev_ofst.y = pWork->ofst.y;
			pWork->prev_ofst.z = pWork->ofst.z;
			pWork->ofst.x = 0;
			pWork->ofst.y = 0;
			pWork->ofst.z = 0;
		}
	}
}

// ================================================================
// objDestructor
/*!
  オブジェクト解放処理

 */
// ================================================================
static void objDestructor( MTS_TASK_TCB *pTcb )
{
#ifndef _DS
	UNREFERENCED_PARAMETER(pTcb);
#endif

    obj_ptcb = NULL;

    // 地形ポインタクリア
    ObjSetBlockCollision( NULL );
    ObjSetDiffCollision( NULL );
    
    // デバッグ矩形解放
    ObjDebugRectActionExit();

    // データ管理リスト開放
	if (g_obj.flag & OBD_OBJ_SAVE_DATAWORK) {
		// データワークを開放せずに保持する
		obj_data_max_save	= g_obj.data_max;
		obj_data_work_save	= g_obj.pData;

		g_obj.data_max	= 0;
		g_obj.pData		= NULL;
	}
	else {
		ObjDataFree();
	}
}

#if !defined _DS
// ================================================================
// objExitWait
/*!
  オブジェクトシステム終了待機

 */
// ================================================================
static void objExitWait( MTS_TASK_TCB *pTcb )
{
	OBS_OBJECT_WORK	*obj_work;

#ifndef _DS
	UNREFERENCED_PARAMETER(pTcb);
#endif

	obj_work = ObjObjectSearchRegistObject(NULL, 0xFFFF);
	if (obj_work == NULL) {
	// オブジェクト破棄終了
		// オブジェクトシステムメイン処理クリア
		mtTaskClearTcb(obj_ptcb);

		// オブジェクトシステム管理ワーククリア
		MI_CpuClear8((void*)&g_obj, sizeof(OBS_OBJECT));

		// ObjDataFree は objDestructor内から呼び出し
		// obj_ptcb のクリアは objDestructor内から呼び出し
	}
	else {
		// 終了命令再発行
		while (obj_work) {
			// 強制終了
			obj_work->flag |= OBD_OBJECT_TASKCLEAR;
			obj_work = ObjObjectSearchRegistObject(obj_work, 0xFFFF);
		}
	}
}
#endif

// ================================================================
// オブジェクト管理
// ================================================================
#if !defined (_DS)
// ================================================================
// objObjectExitDataRelease
/*!
  データ解放処理
 */
// ================================================================
void objObjectExitDataRelease(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK		*obj_work;
	BOOL				b_release_wait = FALSE;

	obj_work = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);

	if (obj_work->ppUserRelease) {
		if (obj_work->ppUserRelease(obj_work)) {
			// 待機あり
			b_release_wait = TRUE;
		}
	}

	if (obj_work->obj_3d && !(obj_work->flag & OBD_OBJECT_NORELEASE_3D)) {
		ObjAction3dNNModelRelease(obj_work->obj_3d);
		b_release_wait = TRUE;
	}
	
	// ESエフェクト
	if (obj_work->obj_3des) {
		// ESテクスチャ解放
		if (obj_work->obj_3des->texlist) {
			ObjAction3dESTextureRelease(obj_work->obj_3des);
			b_release_wait	= TRUE;
		}
		
		// ESオブジェクトの解放
		if (obj_work->obj_3des->model) {
			ObjAction3dESModelRelease(obj_work->obj_3des);
			b_release_wait	= TRUE;
		}
	}

#if OBD_USE_ACTION2D_AMA
	if (obj_work->obj_2d && obj_work->obj_2d->ao_tex.texlist) {
		AoTexRelease(&obj_work->obj_2d->ao_tex);
		b_release_wait = TRUE;
	}
#endif

	if (b_release_wait) {
		// 終了待機処理へ
		mtTaskChangeTcbProcedure(tcb, objObjectDataReleaseCheck);
	}
	else {
		// 解放待機するデータが無いので終了
        mtTaskClearTcb(tcb);
	}
}

// ================================================================
// objObjectDataReleaseCheck
/*!
  データ解放処理関数
 */
// ================================================================
void objObjectDataReleaseCheck(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work;
	BOOL			b_obj_user_end = TRUE;		// ユーザー開放待機
	BOOL			b_obj_3d_end = TRUE;		// 3DNN開放待機
	BOOL			b_obj_3des_end	= TRUE;		// 3DES開放待機
	BOOL			b_obj_2d_end	= TRUE;		// 2D開放待機

	obj_work = (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);

	if (obj_work->ppUserReleaseWait) {
		if (obj_work->ppUserReleaseWait(obj_work)) {
			// 待機中
			b_obj_user_end = FALSE;
		}
	}

	if (obj_work->obj_3d && !(obj_work->flag & OBD_OBJECT_NORELEASE_3D)) {
		if (ObjAction3dNNModelReleaseCheck(obj_work->obj_3d)) {
			b_obj_3d_end = TRUE;
			obj_work->obj_3d->reg_index = -1;
		}
		else {
			b_obj_3d_end = FALSE;
		}
	}
	
	if (obj_work->obj_3des) {
		b_obj_3des_end	= TRUE;
		
		// ESエフェクト(ECB)解放
		if (obj_work->obj_3des->ecb) {
			ObjAction3dESEffectRelease(obj_work->obj_3des);
		}
		
		// ESモデル解放待ち
		if (!ObjAction3dESModelReleaseCheck(obj_work->obj_3des)) {
			b_obj_3des_end	= FALSE;
		}
		
		// ESテクスチャ解放待ち
		if (!ObjAction3dESTextureReleaseCheck(obj_work->obj_3des)) {
			b_obj_3des_end	= FALSE;
		}
	}

#if OBD_USE_ACTION2D_AMA
	if (obj_work->obj_2d && obj_work->obj_2d->ao_tex.texlist) {
		if (!AoTexIsReleased(&obj_work->obj_2d->ao_tex)) {
			b_obj_2d_end = FALSE;
		}
	}
#endif

	if (b_obj_3d_end && b_obj_3des_end && b_obj_user_end && b_obj_2d_end) {
		// 終了
        mtTaskClearTcb(tcb);
	}
}
#endif //#if !defined (_DS)

// ================================================================
// objObjectParent
/*!
  親関係処理

  @param pWork [in] オブジェワークポインタ

  @return TRUE タスク消去フラグが立った FALSE 立たなかった
 */
// ================================================================
static u16 objObjectParent( OBS_OBJECT_WORK *pWork )
{
    OBS_OBJECT_WORK * pParent = pWork->parent_obj;

    // 親存在チェック
    if ( pParent ){
        // 死亡フラグをチェック
        if ( pParent->flag & OBD_OBJECT_TASKCLEAR ){
            if (!( pWork->flag & OBD_OBJECT_PARENT_NODIE )){
                // 親と一緒に死ぬ
                pWork->flag |= OBD_OBJECT_TASKCLEAR;
                pWork->parent_obj = NULL;
                return TRUE;
            }
            // 親死亡
            pWork->parent_obj = NULL;
        }
        
		if (!ObjObjectPauseCheck(pWork->flag)) {
			// 座標吸着
			if ( pWork->flag & OBD_OBJECT_PARENT_FIX ){
				// 向きコピー
				if ( !(pWork->flag & OBD_OBJECT_PARENT_FIX_NOFLIP) ) {
					pWork->disp_flag &= ~( OBD_DISP_HFLIP | OBD_DISP_VFLIP); 
					pWork->disp_flag |= pParent->disp_flag & ( OBD_DISP_HFLIP | OBD_DISP_VFLIP); 
				}

				// 表示コピー
				if ( !(pWork->flag & OBD_OBJECT_PARENT_FIX_NODISP) ) {
					pWork->disp_flag &= ~(OBD_DISP_NODISP); 
					pWork->disp_flag |= pParent->disp_flag & (OBD_DISP_NODISP); 
				}

				// 座標コピー
				pWork->pos.x = pParent->pos.x + pWork->parent_ofst.x;
				pWork->pos.y = pParent->pos.y + pWork->parent_ofst.y;
				pWork->pos.z = pParent->pos.z + pWork->parent_ofst.z;
				if ( pWork->disp_flag & OBD_DISP_HFLIP )
					pWork->pos.x = pParent->pos.x - pWork->parent_ofst.x;
				if ( pWork->disp_flag & OBD_DISP_VFLIP )
					pWork->pos.y = pParent->pos.y - pWork->parent_ofst.y;
	                
				pWork->ofst.x = pParent->prev_ofst.x;
				pWork->ofst.y = pParent->prev_ofst.y;
				pWork->ofst.z = pParent->prev_ofst.z;

				// ヒットストップコピー
				if ( pParent->hitstop_timer ){
					pWork->hitstop_timer = pParent->hitstop_timer;
					// 経過フレーム分を増加させる
					pWork->hitstop_timer += ObjTimeCountGet( FX32_ONE );
				}
	            
			}
			// アクション同期
			if ( pWork->flag & OBD_OBJECT_PARENT_ACT_FIX ){
#if OBD_USE_ACTION3D_SPR | OBD_USE_ACTION2D
				u16 check = 0;
				MTS_ACTION * pAct, * pActPar;
				// アクションチェック
				if( (pWork->obj_3dspr && pParent->obj_3dspr) ||
					(pWork->obj_2d && pParent->obj_2d) ){
	                
					// ポインタ取得
					if( pWork->obj_3dspr ){
						pAct = &pWork->obj_3dspr->act_3dspr.act;
						pActPar = &pParent->obj_3dspr->act_3dspr.act;
					}else{
						pAct = &pWork->obj_2d->act_spr.act;
						pActPar = &pParent->obj_2d->act_spr.act;
				   }

					// アクション速度コピー
					pAct->speed = pActPar->speed;

					// アクションが異なっているかチェック
					if ( pAct->act_id != pActPar->act_id )
						check = TRUE;
					// パターン番号チェック
					else if ( pAct->ptn_no > pActPar->ptn_no )
						check = TRUE;
					// IDも同じ、パターン番号も同じ時はタイマチェックを行う
					else if ( pAct->ptn_no == pActPar->ptn_no && pAct->timer < pActPar->timer )
						check = TRUE;
	                
					if ( check )
						ObjDrawObjectActionSet(pWork, pActPar->act_id);
				}
#endif // #if OBD_USE_ACTION3D_SPR | OBD_USE_ACTION2D
				// 関連フラグのコピー
				pWork->disp_flag &= ~(OBD_DISP_REPEAT | OBD_DISP_END | OBD_DISP_STOP | OBD_DISP_NODISP );
				pWork->disp_flag |=  pParent->disp_flag & (OBD_DISP_REPEAT | OBD_DISP_END | OBD_DISP_STOP | OBD_DISP_NODISP );
			}
		}	// if (!ObjObjectPauseCheck(pWork->flag))
    }

    return FALSE;
}

// ================================================================
// ObjObjectCollision
/*!
  オブジェクト分解地形チェック

  @param pWork [in] オブジェワークポインタ

 */
// ================================================================
void ObjObjectCollision( OBS_OBJECT_WORK *pWork )
{
    s32 lPosX  = pWork->pos.x;
    s32 lPosY  = pWork->pos.y;
    s32 lTempX = pWork->pos.x;
    s32 lTempY = pWork->pos.y;
    s32 lPrevX = pWork->prev_pos.x;
    s32 lPrevY = pWork->prev_pos.y;
    u16 usColMax = OBD_OBJECT_COL_MAX;
    
    u32 ulColFlag = 0;
    u32 ulMoveFlag = 0;
    
#if defined(MTD_DEBUG)  // デバッグ版
    mtSetTaskBarColor(GX_RGB(16,8,0));
#endif  // #endif of #if defined(MTD_DEBUG)
    
    pWork->col_flag_prev = pWork->col_flag;
    pWork->col_flag = 0;
    if ( !(pWork->move_flag & OBD_MOVE_NOCOL) ){

		// 地形フラグのリセット
		pWork->move_flag &= ~OBD_MOVE_UNDERPREV;
		if ( pWork->move_flag & OBD_MOVE_UNDER )
			pWork->move_flag |= OBD_MOVE_UNDERPREV;
		pWork->move_flag &= ~OBD_MOVE_COL_MASK;

        // 厳密チェックオブジェクト
        //if ( pWork->move_flag & OBD_MOVE_UNDERALL ){

			// ────────────────────────────────
            // 　１フレーム当たりの移動量が１キャラ単位を超える場合は
            // 　移動量に応じ複数回に分割してコリジョンチェックを行う

			if ( !(pWork->move_flag & OBD_MOVE_UNDER) )		// 地上なら8ドット単位でチェック
				usColMax >>= 1;								// 空中なら4ドット単位？

            if (  (MTM_MATH_ABS(pWork->pos.x - pWork->prev_pos.x) > usColMax)
            	||(MTM_MATH_ABS(pWork->pos.y - pWork->prev_pos.y) > usColMax) ) {
                pWork->pos.x = pWork->prev_pos.x;
                pWork->pos.y = pWork->prev_pos.y;
	            // 指定ドット以上の移動があった場合
                // 分解してチェックする
                for (;;){

                    if ( MTM_MATH_ABS(pWork->pos.x - lPosX) > usColMax ){
                        pWork->prev_pos.x = pWork->pos.x;
                        if ( lPosX > pWork->prev_pos.x)
                            pWork->pos.x = pWork->prev_pos.x + usColMax;
                        else
                            pWork->pos.x = pWork->prev_pos.x - usColMax;
                    }else{
                        pWork->pos.x = lPosX;
                    }
                    if ( MTM_MATH_ABS(pWork->pos.y - lPosY) > usColMax ){
                        pWork->prev_pos.y = pWork->pos.y;
                        if ( lPosY > pWork->prev_pos.y)
                            pWork->pos.y = pWork->prev_pos.y + usColMax;
                        else
                            pWork->pos.y = pWork->prev_pos.y - usColMax;
                    }else{
                        pWork->pos.y = lPosY;
                    }

                    if (  (pWork->pos.x == lPosX)
                    	&&(pWork->pos.y == lPosY) )
                        break;

                    lTempX = pWork->pos.x;
                    lTempY = pWork->pos.y;

                    // 地形チェック
                    ObjDiffCollisionEarthCheck(pWork);

                    ulColFlag |= pWork->col_flag;
                    ulMoveFlag |= pWork->move_flag & OBD_MOVE_COL_MASK;
                    // 地形HITで座標が変更された時
                    if ( lTempX != pWork->pos.x)
                        lPosX = pWork->pos.x;
                    if ( lTempY != pWork->pos.y)
                        lPosY = pWork->pos.y;
                }
            }
			// ────────────────────────────────
        //}
		// 地形チェック
		ObjDiffCollisionEarthCheck(pWork);
		ulColFlag |= pWork->col_flag;

        pWork->col_flag = ulColFlag;
        pWork->move_flag |= ulMoveFlag;

        if ( pWork->move_flag & OBD_MOVE_THROUGH )
            if ( !(pWork->col_flag & OBD_COLAT_THROUGH) )
                pWork->move_flag &= ~OBD_MOVE_THROUGH;
    }
    // 戻す
    pWork->prev_pos.x = lPrevX;
    pWork->prev_pos.y = lPrevY;

#if defined(MTD_DEBUG)  // デバッグ版
    mtSetTaskBarColor( MTD_SYS_TASK_COLOR );
#endif  // #endif of #if defined(MTD_DEBUG)
}

// ================================================================
// ObjObjectMove
/*!
  オブジェクト移動

  @param pWork [in] オブジェワークポインタ
 */
// ================================================================
void ObjObjectMove( OBS_OBJECT_WORK *pWork )
{
    fx32 sSpdX = 0,sSpdY = 0, sSpdZ = 0;
    fx32 sFlowX;
    fx32 sFlowY;

    // 前座標保持
    pWork->prev_pos.x = pWork->pos.x;
    pWork->prev_pos.y = pWork->pos.y;
    pWork->prev_pos.z = pWork->pos.z;

    // sFlowの影響を受けないフラグをチェック
    if ( pWork->move_flag & OBD_MOVE_NOFLOW ){
        pWork->flow.x = 0;
        pWork->flow.y = 0;
        pWork->flow.z = 0;
    }
    sFlowX = pWork->flow.x;
    sFlowY = pWork->flow.y;
    
    if ( sFlowX || sFlowY ){
        if ( pWork->dir_fall )
            ObjObjectSpdDirFall( &sFlowX, &sFlowY, pWork->dir_fall );
    }
    
    if ( pWork->hitstop_timer ){
        // HIT STOP時移動処理
        pWork->move.x = FX_Mul( sFlowX        , g_obj.speed);
        pWork->move.y = FX_Mul( sFlowY        , g_obj.speed);
        pWork->move.z = FX_Mul( pWork->flow.z, g_obj.speed);
        
    }else{
        //　通常移動処理
        if (!( pWork->move_flag & OBD_MOVE_UNDER )){
            // 落下加速
            if ( pWork->move_flag & OBD_MOVE_FALL && !(pWork->move_flag & OBD_MOVE_UNDER)){
                pWork->spd.y += FX_Mul(pWork->spd_fall, g_obj.speed);
            }
            if ( pWork->move_flag & OBD_MOVE_FALL ){
                // 落下速度オーバーチェック
                if ( pWork->spd.y >  pWork->spd_fall_max )
                    pWork->spd.y = pWork->spd_fall_max;
            }
        }

        // マスタースピードを角度に合わせて分解
        if( pWork->move_flag & OBD_MOVE_DIR ){
            if ( pWork->move_flag & OBD_MOVE_SLOPE && (pWork->spd_m || !(pWork->move_flag & OBD_MOVE_SLOPE_STICK )) ){
                // 角度チェック
#if 1
				// 20090817 重力反映
//				if ( (((pWork->dir.z - pWork->dir_fall + pWork->dir_slope) & 0xffff) >= ( pWork->dir_slope << 1)) ){
#if 1
				fx32	spd_temp;

				spd_temp = FX_Mul(pWork->spd_slope, mtMathSin(pWork->dir.z));
				if (spd_temp) {
					pWork->spd_m = ObjSpdUpSet(pWork->spd_m, spd_temp, pWork->spd_slope_max);
				}
				else {
					if (pWork->spd_m > 0) {
						if (pWork->spd_m > pWork->spd_slope_max) {
							pWork->spd_m = pWork->spd_slope_max;
						}
					}
					else {
						if (pWork->spd_m < -pWork->spd_slope_max) {
							pWork->spd_m = -pWork->spd_slope_max;
						}
					}
				}
		//		if ( (((pWork->dir.z + pWork->dir_slope) & 0xffff) >= ( pWork->dir_slope << 1)) ){
		//			if (pWork->dir.z > 0x8000 ) {
		//				pWork->spd_m = ObjSpdUpSet( pWork->spd_m,  
		//											FX_Mul(pWork->spd_slope, mtMathSin(pWork->dir.z)),
		//											pWork->spd_slope_max );
		//			}
		//			else {
		//				pWork->spd_m = ObjSpdUpSet( pWork->spd_m,  
		//											FX_Mul(-pWork->spd_slope, mtMathSin(pWork->dir.z)), 
		//											pWork->spd_slope_max );
		//			}
		//		}
#else
				if ( (((pWork->dir.z + pWork->dir_slope) & 0xffff) >= ( pWork->dir_slope << 1)) ){
//不要ぽいのでCUT	if ( (u16)(pWork->dir.z - pWork->dir_fall) > 0 )
						pWork->spd_m = ObjSpdUpSet( pWork->spd_m,  
//													FX_Mul(pWork->spd_slope, mtMathSin( (u16)(pWork->dir.z - pWork->dir_fall)) ),
													FX_Mul(pWork->spd_slope, mtMathSin( (u16)(pWork->dir.z)) ),
													pWork->spd_slope_max );
//不要ぽいのでCUT	else
//不要ぽいのでCUT		pWork->spd_m = ObjSpdUpSet( pWork->spd_m,  
//不要ぽいのでCUT									FX_Mul(-pWork->spd_slope, mtMathSin( (u16)(pWork->dir.z + pWork->dir_fall) )), 
//不要ぽいのでCUT									pWork->spd_slope_max );
                }
#endif
#else
                if ( (((pWork->dir.z + pWork->dir_slope) & 0xffff) >= ( pWork->dir_slope << 1)) ){
                    if ( pWork->dir.z > 0 )
                        pWork->spd_m = ObjSpdUpSet( pWork->spd_m,  FX_Mul( pWork->spd_slope, mtMathSin( pWork->dir.z )), pWork->spd_slope_max );
                    else
                        pWork->spd_m = ObjSpdUpSet( pWork->spd_m,  FX_Mul(-pWork->spd_slope, mtMathSin( pWork->dir.z )), pWork->spd_slope_max );
                }
#endif

            }
            if( !(pWork->move_flag & OBD_MOVE_NOSPDM) ){
                sSpdX = FX_Mul((pWork->spd_m ), mtMathCos( pWork->dir.z ));
                sSpdY = FX_Mul((pWork->spd_m ), mtMathSin( pWork->dir.z ));
            }
        }

        if ( pWork->move_flag & OBD_MOVE_NO_AUTO_SCROLL ){
            pWork->move.x = FX_Mul((pWork->spd.x + sSpdX + sFlowX ), g_obj.speed);
            pWork->move.y = FX_Mul((pWork->spd.y + sSpdY + sFlowY ), g_obj.speed);
        }else{
            pWork->move.x = FX_Mul((pWork->spd.x + sSpdX + sFlowX + g_obj.scroll[0]), g_obj.speed);
            pWork->move.y = FX_Mul((pWork->spd.y + sSpdY + sFlowY + g_obj.scroll[1]), g_obj.speed);
        }
        pWork->move.z = FX_Mul((pWork->spd.z + sSpdZ + pWork->flow.z), g_obj.speed);

        {
            // 重力チェックあり
            ObjObjectSpdDirFall( &pWork->move.x, &pWork->move.y, pWork->dir_fall );
        }

        

    }
    // 実移動
    pWork->pos.x += pWork->move.x;
    pWork->pos.y += pWork->move.y;
    pWork->pos.z += pWork->move.z;
    
    // 加速
    pWork->spd.x += pWork->spd_add.x;
    pWork->spd.y += pWork->spd_add.y;
    pWork->spd.z += pWork->spd_add.z;

	// flowクリア
	pWork->flow.x = 0;
	pWork->flow.y = 0;
	pWork->flow.z = 0;
}

// ================================================================
// objObjectColRideTouchCheck
/*!
  オブジェクト地形オブジェクト外部者チェック

  @param pWork [in] オブジェワークポインタ
 */
// ================================================================
void objObjectColRideTouchCheck( OBS_OBJECT_WORK * pWork )
{
    // 地形ワーク登録ありなら
    if ( pWork->col_work ){
        // 自分に載っている人をチェック
        if ( pWork->col_work->obj_col.rider_obj ){
            if ( pWork->col_work->obj_col.rider_obj->flag & OBD_OBJECT_TASKCLEAR )
                pWork->col_work->obj_col.rider_obj = NULL;
        }
        // 自分を触っている人をチェック 自分に乗っている人は無視
        if ( pWork->col_work->obj_col.toucher_obj &&
             pWork->col_work->obj_col.toucher_obj != pWork->col_work->obj_col.rider_obj ){
			if ( pWork->col_work->obj_col.toucher_obj) {
				if (!ObjObjectPauseCheck(pWork->flag) &&	// ポーズ中はチェックしない
						pWork->col_work->obj_col.toucher_obj != pWork->col_work->obj_col.rider_obj ){
					// 押されているか、押せるオブジェクトかチェック
					if ( pWork->col_work->obj_col.toucher_obj->move_flag & OBD_MOVE_PUSH && pWork->move_flag & OBD_MOVE_PUSH_COL ){
						fx32 sPush = pWork->col_work->obj_col.toucher_obj->move.x;

						sPush = MTM_MATH_CLIP(sPush, -pWork->col_work->obj_col.toucher_obj->push_max, pWork->col_work->obj_col.toucher_obj->push_max);
						sPush = MTM_MATH_CLIP(sPush, -pWork->push_max, pWork->push_max);
						pWork->flow.x += sPush;
						// ■ 縦押しを実装したい場合は、SpdMとucDirからの速度方向とsSpdYをチェックし、押している事をチェックする
						// pWork->sFlowY += MTM_MATH_CLIP(pWork->col_work->obj_col.toucher_obj->sMoveY, -pWork->col_work->obj_col.toucher_obj->push_max, pWork->col_work->obj_col.toucher_obj->push_max);
					}
				}

				if ( pWork->col_work->obj_col.toucher_obj->flag & OBD_OBJECT_TASKCLEAR )
					pWork->col_work->obj_col.toucher_obj = NULL;
			}
		}
    }
    
    // 自分が触っているものをチェック
    if ( pWork->touch_obj ){
        // 押せるオブジェクトを押しているかチェック
        if (!ObjObjectPauseCheck(pWork->flag) &&	// ポーズ中はチェックしない
        		 pWork->touch_obj->move_flag & OBD_MOVE_PUSH_COL && pWork->move_flag & OBD_MOVE_PUSH ){
            
            pWork->flow.x += (s16)(pWork->touch_obj->pos.x - pWork->touch_obj->prev_pos.x);
            // ■ 縦押しを実装したい場合は、SpdMとucDirからの速度方向とsSpdYをチェックし、押している事をチェックする
            //pWork->sFlowY += (s16)(pWork->touch_obj->lPosY - pWork->touch_obj->lPrevPosY);

			if ( pWork->touch_obj->move.x & 0x00000fff) {
                if ( pWork->touch_obj->move.x > 0 )
                    pWork->flow.x += 0x1000 - (pWork->touch_obj->move.x & 0x00000fff);
                else
                    pWork->flow.x -= 0x1000 + (pWork->touch_obj->move.x & 0x00000fff);
			}
            //if ( pWork->touch_obj->sMoveY & 0x00ff)
            //    pWork->sFlowY += 0x0100 - (pWork->touch_obj->sMoveY & 0x00ff);
            
        }
        if ( pWork->touch_obj->flag & OBD_OBJECT_TASKCLEAR ){
            pWork->touch_obj = NULL;
        }
    }
    // 自分が乗っているものをチェック
    if ( pWork->ride_obj ){
        if ( pWork->ride_obj->flag & OBD_OBJECT_TASKCLEAR ){
            pWork->ride_obj = NULL;
        }
        else if (!ObjObjectPauseCheck(pWork->flag)) {	// ポーズ中はチェックしない
            // 乗っているものから速度を取得
            if ( pWork->ride_obj->move_flag & OBD_MOVE_NOCOL ){
                pWork->flow.x += pWork->ride_obj->move.x;
                pWork->flow.y += pWork->ride_obj->move.y;
                pWork->flow.z += pWork->ride_obj->move.z;
            }else{
                pWork->flow.x += (s16)(pWork->ride_obj->pos.x - pWork->ride_obj->prev_pos.x);
                pWork->flow.y += (s16)(pWork->ride_obj->pos.y - pWork->ride_obj->prev_pos.y);
                pWork->flow.z += (s16)(pWork->ride_obj->pos.z - pWork->ride_obj->prev_pos.z);
            }
            if ( pWork->ride_obj->move.y & 0x00000fff)
                pWork->flow.y += 0x1000 - (pWork->ride_obj->move.y & 0x00000fff);
        }
    }
}

// ================================================================
// ObjObjectFieldRectSet
/*!
  地形当り設定

  @param pObj    [io] オブジェクトワークポインタ
  @param cLeft   [in] 左端値
  @param cTop    [in] 上端値
  @param cRight  [in] 右端値
  @param cBottom [in] 下端値
    
 */
// ================================================================
void ObjObjectFieldRectSet( OBS_OBJECT_WORK* pObj, s16 cLeft, s16 cTop, s16 cRight, s16 cBottom)
{
    pObj->field_rect[OBD_LEFT   ] = cLeft;
    pObj->field_rect[OBD_TOP    ] = cTop;
    pObj->field_rect[OBD_RIGHT  ] = cRight;
    pObj->field_rect[OBD_BOTTOM ] = cBottom;

    pObj->move_flag &= ~OBD_MOVE_NOCOL;
}

// ================================================================
// ObjObjectFallSet
/*!
  地形当り設定

  @param pObj    [io] オブジェクトワークポインタ
  @param cLeft   [in] 左端値
  @param cTop    [in] 上端値
  @param cRight  [in] 右端値
  @param cBottom [in] 下端値
    
 */
// ================================================================
void ObjObjectFallSet( OBS_OBJECT_WORK* pObj, fx32 fSpdFall, fx32 fSpdFallMax )
{
    pObj->spd_fall = fSpdFall;
    pObj->spd_fall_max = fSpdFallMax;
    pObj->move_flag |= OBD_MOVE_FALL;
}


// ==========================================================================
// objObjectStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void objObjectStaticVarInit(void)
{
	memset(&g_obj, 0, sizeof(g_obj));	///< オブジェクトシステム管理ワーク
	
#if (OBD_USE_ACTION3D_NN)
	/// 描画時設定ステータス初期化用データ
	memset(&g_obj_draw_3dnn_draw_state, 0, sizeof(g_obj_draw_3dnn_draw_state));
#endif
	obj_ptcb = NULL;		///< オブジェクトシステムメインタスク
	
	// データワーク退避
	obj_data_work_save = NULL;	///< データワーク退避
	obj_data_max_save = 0;		///< データワーク数退避
}


// ================================================================
// objActDivisionDraw
/*!
  分割アクションを描画する
  
  @param    act     [in] 対象アクション
  @param    pOffst  [in] 座標テーブル XYXYXY… 1:15

  @note
    pOffstのテーブル数に注意してください\n
    実際の絵のサイズではなく、スプライトのサイズ分使用します\n
    足りなかった場合、フローした箇所のメモリを参照します（表示位置がおかしくなります）\n
    OPEでは正しく動作しません\n
 */
// ================================================================
/*static void objActDivisionDraw( MTS_ACTION *act, s16 *pOffst )
{
    MTS_GE2_CONTEXT     *ge = _mt_ge2_table[act->ge_type];
    const MTS_ACT_BAC_CONVERSION_PATTERN  *cnv_ptn  =
        (const MTS_ACT_BAC_CONVERSION_PATTERN*)( (u32)mtActGetConversionSectionFromBac( act->bac_addr ) + act->cnv_ofst );
    s32     center_pos[MTD_XY];     // 表示中心座標
    s32     size[MTD_RECT];         // ローカル座標系でのパターン全体サイズ
    u32     cha_name;               // 開始キャラクタネーム
    u16     attrib0_flag    = 0;    // アトリビュート0のフラグ
    u16     attrib1_flag    = 0;    // アトリビュート1のフラグ
    u8     oam_max_size_x      = 0;    // 最大横サイズ
    u8     oam_max_size_y      = 0;    // 最大縦サイズ

#if defined(MTD_DEBUG)
    // スプライトで扱えるキャラクタフォーマットかチェックする
    switch( mtActGetActionSectionFromBac( act->bac_addr )->info[act->act_id].cha_format )
    {
    case MTE_CHA_FORMAT_16:
    case MTE_CHA_FORMAT_256:
    case MTE_CHA_FORMAT_DIRECT:
        break;
    default:
        MTM_ASSERT( !"mtAction::mtActDraw() Error! Invalid character format!\n" );
        return;
    }
#endif  // #endif of #if defined(MTD_DEBUG)

    // 表示するオブジェクトが無ければ何もせずに終了する
    if( 0 == cnv_ptn->num_obj )
        return ;

    // 表示中心座標を求める
    center_pos[MTD_X]   = act->pos[MTD_X];
    center_pos[MTD_Y]   = act->pos[MTD_Y];
    size[MTD_LEFT]      = (s32)cnv_ptn->size[MTD_LEFT];
    size[MTD_TOP]       = (s32)cnv_ptn->size[MTD_TOP];
    size[MTD_RIGHT]     = (s32)cnv_ptn->size[MTD_RIGHT];
    size[MTD_BOTTOM]    = (s32)cnv_ptn->size[MTD_BOTTOM];

    // 必要なOAMの数を計算
    oam_max_size_x =(u8)(((size[MTD_RIGHT] - size[MTD_LEFT]) / (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ) );
    oam_max_size_y =(u8)(((size[MTD_BOTTOM] - size[MTD_TOP]) >> 3) );

    if ( !oam_max_size_x )
        oam_max_size_x = 1;
    if ( !oam_max_size_y )
        oam_max_size_y = 1;
    
    if( MTD_ACT_FLAG_ENABLE_OFFSET & act->flag )
    {
        center_pos[MTD_X]   += ge->spr_offset[MTD_X];
        center_pos[MTD_Y]   += ge->spr_offset[MTD_Y];
    }

    // フリップを設定する
    if( MTD_ACT_FLAG_FLIP_H & act->flag )
    {
        attrib1_flag        |= MTD_OBJ_ATTR1_FLIP_H;
        size[MTD_LEFT]      = -(s32)cnv_ptn->size[MTD_RIGHT];
        size[MTD_RIGHT]     = -(s32)cnv_ptn->size[MTD_LEFT];
    }
    if( MTD_ACT_FLAG_FLIP_V & act->flag )
    {
        attrib1_flag        |= MTD_OBJ_ATTR1_FLIP_V;
        size[MTD_TOP]       = -(s32)cnv_ptn->size[MTD_BOTTOM];
        size[MTD_BOTTOM]    = -(s32)cnv_ptn->size[MTD_TOP];
    }

    // モザイクを設定する
    if( MTD_ACT_FLAG_MOSAIC & act->flag )
        attrib0_flag    |= MTD_OBJ_ATTR0_MOSAIC;

    // OBJ-VRAMアドレスから基準キャラクタネームを求める
    if( MTD_OBJ_ATTR0_MODE_BMP_OBJ == ( MTD_OBJ_ATTR0_MODE_BMP_OBJ & cnv_ptn->obj_info[0].attribute[0] ) )
        cha_name    = MTM_ACT_BMP_CHA_NAME_FROM_OFST(
                        act->cha_addr - _mt_obj_vram_addr[act->ge_type], ge );
    else
        cha_name    = MTM_ACT_CHA_NAME_FROM_OFST(
                        act->cha_addr - _mt_obj_vram_addr[act->ge_type], ge );

    // 各オブジェ情報からOAMソートを作成する
    {
        const MTS_ACT_BAC_CONVERSION_OBJ    *obj_info   = cnv_ptn->obj_info;
        const u16   *obj_size;          // オブジェサイズ
        s32         obj_pos[MTD_XY];    // オブジェ座標
        MTS_SPR_OAM_SORT    *oam_sort;
        u32                 dest_name_shift;    // キャラクタネームシフト回数
        u32                 i,j,k,l = 0;
        u8 cha_name_add = 0;
        u8     oam_size_x      = 0;    // 横サイズ
        u8     oam_size_y      = 0;    // 縦サイズ
        
        // キャラクタフォーマットに対応したキャラクタネームシフト回数を求める
        if( MTD_OBJ_ATTR0_MODE_BMP_OBJ == ( MTD_OBJ_ATTR0_MODE_BMP_OBJ & cnv_ptn->obj_info[0].attribute[0] ) )
            dest_name_shift = _mt_ge2_table[act->ge_type]->vram_obj_bmp_unit_shift;
        else
            dest_name_shift = _mt_ge2_table[act->ge_type]->vram_obj_unit_shift;

        for( k = 0; k < cnv_ptn->num_obj; ++k, ++obj_info ) {
            // オブジェサイズを求める
            obj_size    =
                _mt_act_obj_size[ MTM_ACT_TBL_INDEX_FROM_SHAPE_SIZE( obj_info->attribute[0], obj_info->attribute[1] ) ];

            // サイズから繰り返し数を取得
            oam_size_x =(u8)((obj_size[MTD_WIDTH ] / (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ) );
            oam_size_y =(u8)((obj_size[MTD_HEIGHT] >> 3) );

            if ( !oam_size_x )
                oam_size_x = 1;
            if ( !oam_size_y )
                oam_size_y = 1;
            cha_name_add = 0;

            // 現表示物座標からグローバル座標カウンタを設定する 
            l = 0;

            
            // このスプライトを分割して登録する
            for( j = 0; j < oam_size_y; ++j ){
                
                l = (u8)((( ( (s32)obj_info->attribute[1] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) >> 3) >> _mt_ge2_table[act->ge_type]->vram_obj_unit_shift);
                l += (u8)(((( ( (s32)obj_info->attribute[0] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) >> 3) + j) * oam_max_size_x);
                
                for( i = 0; i < oam_size_x; ++i ){
                    
                    // フリップ時を考慮しつつオブジェの座標を求める
                    if( MTD_OBJ_ATTR1_FLIP_H & attrib1_flag )
                        obj_pos[MTD_X]  = (s32)(center_pos[MTD_X] + pOffst[l * 2 + MTD_X] + cnv_ptn->center[MTD_X] - ( ( (s32)obj_info->attribute[1] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) - ( i * ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift ) ) - (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift));
                    else
                        obj_pos[MTD_X]  = (s32)(center_pos[MTD_X] + pOffst[l * 2 + MTD_X] - cnv_ptn->center[MTD_X] + ( ( (s32)obj_info->attribute[1] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) + ( i * ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ));
                    if( MTD_OBJ_ATTR1_FLIP_V & attrib1_flag )
                        obj_pos[MTD_Y]  = (s32)(center_pos[MTD_Y] + pOffst[l * 2 + MTD_Y] + cnv_ptn->center[MTD_Y] - ( ( (s32)obj_info->attribute[0] << ( 32 - 8 ) ) >> ( 32 - 8 ) ) - ( j * 8 ) - 8);
                    else
                        obj_pos[MTD_Y]  = (s32)(center_pos[MTD_Y] + pOffst[l * 2 + MTD_Y] - cnv_ptn->center[MTD_Y] + ( ( (s32)obj_info->attribute[0] << ( 32 - 8 ) ) >> ( 32 - 8 ) ) + ( j * 8 ));

                    // オブジェ1つ1つをクリッピングチェックする
                    if( MTD_ACT_FLAG_ENABLE_CLIP & act->flag ) {
                        if(
                            0 >= obj_pos[MTD_X] + ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift )
                            || OBD_LCD_X <= obj_pos[MTD_X]
                            || 0 >= obj_pos[MTD_Y] + 8
                            || OBD_LCD_Y <= obj_pos[MTD_Y] )
                            {
                                // 画面外にあるため描画する必要なし
                                ++cha_name_add;
                                ++l;
                                continue;
                            }
                    }

                    // OAMソートに登録する
                    oam_sort    = mtSprGetOamSort( act->ge_type, act->priority );

                    oam_sort->attribute[0]  = (u16)( 1 << MTD_OBJ_ATTR0_SHAPE_SHIFT| ( obj_pos[MTD_Y] & MTD_OBJ_ATTR0_Y_MASK ) );
                    oam_sort->attribute[0]  ^= attrib0_flag;

                    oam_sort->attribute[1]  = (u16)( ( obj_pos[MTD_X] & MTD_OBJ_ATTR1_X_MASK ) );
                    oam_sort->attribute[1]  ^= attrib1_flag;

                    oam_sort->attribute[2]  = (u16)(
                        ( ( (( ( obj_info->attribute[2] & MTD_OBJ_ATTR2_NAME_MASK ) + ( 1 << dest_name_shift ) - 1 ) >> dest_name_shift)+ cha_name + cha_name_add ) & MTD_OBJ_ATTR2_NAME_MASK )
                        | ( ( ( act->flag & MTD_ACT_FLAG_BG_NO_MASK ) >> MTD_ACT_FLAG_BG_NO_SHIFT ) << MTD_OBJ_ATTR2_PRIORITY_BG_SHIFT )
                        | ( ( ( act->plt_ofst_no << MTD_OBJ_ATTR2_PALETTE_SHIFT ) ) & MTD_OBJ_ATTR2_PALETTE_MASK ) );
                    ++cha_name_add;
                    ++l;
                }
            }
        }
    }
}*/

// ================================================================
// objActDivisionDrawAffine
/*!
  分割アクションを拡縮描画する
  
  @param    act     [in] 対象アクション
  @param    pOffst  [in] 座標テーブル XYXYXY… 1:15
  @param    scale_x [in] Ｘ方向のスケール
  @param    scale_y [in] Ｙ方向のスケール

  @note
    pOffstのテーブル数に注意してください\n
    実際の絵のサイズではなく、スプライトのサイズ分使用します\n
    足りなかった場合、フローした箇所のメモリを参照します（表示位置がおかしくなります）\n
    OPEでは正しく動作しません\n
    回転は使用できません\n
 */
// ================================================================
/*static void objActDivisionDrawAffine( MTS_ACTION *act, s16 *pOffst, fx32 scale_x, fx32 scale_y )
{
    MTS_GE2_CONTEXT     *ge = _mt_ge2_table[act->ge_type];
    const MTS_ACT_BAC_CONVERSION_PATTERN  *cnv_ptn  =
        (const MTS_ACT_BAC_CONVERSION_PATTERN*)( (u32)mtActGetConversionSectionFromBac( act->bac_addr ) + act->cnv_ofst );
    MtxFx22 affine_flip[4];         // フリップを考慮したアフィン変換
    MtxFx22 affine_inv_flip[4];     // フリップを考慮したアフィン変換

    s32     center_pos[MTD_XY];     // 表示中心座標
    s32     size[MTD_RECT];         // ローカル座標系でのパターン全体サイズ
    u32     cha_name;               // 開始キャラクタネーム
    u16     attrib0_flag    = 0;    // アトリビュート0のフラグ
    u16     attrib1_flag    = 0;    // アトリビュート1のフラグ
    u8     oam_max_size_x      = 0;    // 最大横サイズ
    u8     oam_max_size_y      = 0;    // 最大縦サイズ

#if defined(MTD_DEBUG)
    // スプライトで扱えるキャラクタフォーマットかチェックする
    switch( mtActGetActionSectionFromBac( act->bac_addr )->info[act->act_id].cha_format )
    {
    case MTE_CHA_FORMAT_16:
    case MTE_CHA_FORMAT_256:
    case MTE_CHA_FORMAT_DIRECT:
        break;
    default:
        MTM_ASSERT( !"mtAction::mtActDraw() Error! Invalid character format!\n" );
        return;
    }
#endif  // #endif of #if defined(MTD_DEBUG)

    // 表示するオブジェクトが無ければ何もせずに終了する
    if( 0 == cnv_ptn->num_obj )
        return ;

    // スケールがゼロに近い場合は描画しない
    if( MTD_MATH_AFFINE_SCALE_MIN > MTM_MATH_ABS( scale_x ) || MTD_MATH_AFFINE_SCALE_MIN > MTM_MATH_ABS( scale_y ) )
        return ;

    // アフィンパラメータが不足している場合は通常描画に切り替える
    // 最高でも通常、H、V、HVフリップの４パターンが必要になる
    {
        u16     need_ap_max = 4 > cnv_ptn->num_obj ? cnv_ptn->num_obj  : (u16)4;

        if( ge->spr_affine_base + ge->spr_affine_num + need_ap_max >= 32 )
        {
#if defined(MTD_DEBUG)
            if( MTD_DEBUG_ASSERT_AFFINE_OVER & _mt_debug_flag )
            {
                MTM_ASSERT( !"mtAction::mtActDrawAffine() Error! Affine parameter limit!\n" );
            }
#endif  // #endif of #if defined(MTD_DEBUG)
            mtActDraw( act );
            return;
        }
    }

    // 全パターンのアフィンを作成する
    //  この行列はアフィン行列ではなくAGBの回転拡縮パラメータのためスケールを逆数で指定する
    {
        fx32    scale_x_inv = FX_Inv( scale_x );
        fx32    scale_y_inv = FX_Inv( scale_y );

        MTX_Identity22( &affine_flip[0] );

        // Hフリップ
        MTX_ScaleApply22( &affine_flip[0], &affine_inv_flip[1], -scale_x_inv, scale_y_inv );
        MTX_ScaleApply22( &affine_flip[0], &affine_flip[1],     -scale_x,     scale_y );

        // Vフリップ
        MTX_ScaleApply22( &affine_flip[0], &affine_inv_flip[2], scale_x_inv, -scale_y_inv );
        MTX_ScaleApply22( &affine_flip[0], &affine_flip[2],     scale_x,     -scale_y );

        // HVフリップ
        MTX_ScaleApply22( &affine_flip[0], &affine_inv_flip[3], -scale_x_inv, -scale_y_inv );
        MTX_ScaleApply22( &affine_flip[0], &affine_flip[3],     -scale_x,     -scale_y );

        // フリップなし
        MTX_ScaleApply22( &affine_flip[0], &affine_inv_flip[0], scale_x_inv, scale_y_inv );
        MTX_ScaleApply22( &affine_flip[0], &affine_flip[0],     scale_x,     scale_y );
    }


    // 表示中心座標を求める
    center_pos[MTD_X]   = act->pos[MTD_X];
    center_pos[MTD_Y]   = act->pos[MTD_Y];
    size[MTD_LEFT]      = (s32)cnv_ptn->size[MTD_LEFT];
    size[MTD_TOP]       = (s32)cnv_ptn->size[MTD_TOP];
    size[MTD_RIGHT]     = (s32)cnv_ptn->size[MTD_RIGHT];
    size[MTD_BOTTOM]    = (s32)cnv_ptn->size[MTD_BOTTOM];

    // 必要なOAMの数を計算
    oam_max_size_x =(u8)(((size[MTD_RIGHT] - size[MTD_LEFT]) / (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ) );
    oam_max_size_y =(u8)(((size[MTD_BOTTOM] - size[MTD_TOP]) >> 3) );

    if ( !oam_max_size_x )
        oam_max_size_x = 1;
    if ( !oam_max_size_y )
        oam_max_size_y = 1;
    
    if( MTD_ACT_FLAG_ENABLE_OFFSET & act->flag )
    {
        center_pos[MTD_X]   += ge->spr_offset[MTD_X];
        center_pos[MTD_Y]   += ge->spr_offset[MTD_Y];
    }

    // フリップを設定する
    if( MTD_ACT_FLAG_FLIP_H & act->flag )
    {
        attrib1_flag        |= MTD_OBJ_ATTR1_FLIP_H;
        size[MTD_LEFT]      = -(s32)cnv_ptn->size[MTD_RIGHT];
        size[MTD_RIGHT]     = -(s32)cnv_ptn->size[MTD_LEFT];
    }
    if( MTD_ACT_FLAG_FLIP_V & act->flag )
    {
        attrib1_flag        |= MTD_OBJ_ATTR1_FLIP_V;
        size[MTD_TOP]       = -(s32)cnv_ptn->size[MTD_BOTTOM];
        size[MTD_BOTTOM]    = -(s32)cnv_ptn->size[MTD_TOP];
    }

    // モザイクを設定する
    if( MTD_ACT_FLAG_MOSAIC & act->flag )
        attrib0_flag    |= MTD_OBJ_ATTR0_MOSAIC;

    // 倍角を設定する
    if( MTD_ACT_FLAG_BAIKAKU & act->flag )
        attrib0_flag    |= MTD_OBJ_ATTR0_BAIKAKU;

    // OBJ-VRAMアドレスから基準キャラクタネームを求める
    if( MTD_OBJ_ATTR0_MODE_BMP_OBJ == ( MTD_OBJ_ATTR0_MODE_BMP_OBJ & cnv_ptn->obj_info[0].attribute[0] ) )
        cha_name    = MTM_ACT_BMP_CHA_NAME_FROM_OFST(
                        act->cha_addr - _mt_obj_vram_addr[act->ge_type], ge );
    else
        cha_name    = MTM_ACT_CHA_NAME_FROM_OFST(
                        act->cha_addr - _mt_obj_vram_addr[act->ge_type], ge );

    // 各オブジェ情報からOAMソートを作成する
    {
        const MTS_ACT_BAC_CONVERSION_OBJ    *obj_info   = cnv_ptn->obj_info;
        const u16   *obj_size;          // オブジェサイズ
        s32         obj_pos[MTD_XY];    // オブジェ座標
        fx32        local_pos[MTD_XY];  // スプライト回転時のローカル座標
        u16         use_affine_param[4] = { (u16)-1, (u16)-1, (u16)-1, (u16)-1 };
        MTS_SPR_OAM_SORT    *oam_sort;
        u32                 dest_name_shift;    // キャラクタネームシフト回数
        u32                 i,j,k,l = 0;
        u32                 affine_index;
        u8 cha_name_add = 0;
        u8     oam_size_x      = 0;    // 横サイズ
        u8     oam_size_y      = 0;    // 縦サイズ
        
        // キャラクタフォーマットに対応したキャラクタネームシフト回数を求める
        if( MTD_OBJ_ATTR0_MODE_BMP_OBJ == ( MTD_OBJ_ATTR0_MODE_BMP_OBJ & cnv_ptn->obj_info[0].attribute[0] ) )
            dest_name_shift = _mt_ge2_table[act->ge_type]->vram_obj_bmp_unit_shift;
        else
            dest_name_shift = _mt_ge2_table[act->ge_type]->vram_obj_unit_shift;

        for( k = 0; k < cnv_ptn->num_obj; ++k, ++obj_info ) {
            // オブジェサイズを求める
            obj_size    =
                _mt_act_obj_size[ MTM_ACT_TBL_INDEX_FROM_SHAPE_SIZE( obj_info->attribute[0], obj_info->attribute[1] ) ];

            // サイズから繰り返し数を取得
            oam_size_x =(u8)((obj_size[MTD_WIDTH ] / (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ) );
            oam_size_y =(u8)((obj_size[MTD_HEIGHT] >> 3) );

            if ( !oam_size_x )
                oam_size_x = 1;
            if ( !oam_size_y )
                oam_size_y = 1;
            cha_name_add = 0;

            // 現表示物座標からグローバル座標カウンタを設定する 
            l = 0;

            
            // このスプライトを分割して登録する
            for( j = 0; j < oam_size_y; ++j ){
                
                l = (u8)((( ( (s32)obj_info->attribute[1] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) >> 3) >> _mt_ge2_table[act->ge_type]->vram_obj_unit_shift);
                l += (u8)(((( ( (s32)obj_info->attribute[0] << ( 32 - 9 ) ) >> ( 32 - 9 ) ) >> 3) + j) * oam_max_size_x);
                
                // 使用するアフィンのインデックスを求める
                affine_index    = (u32)( ( ( attrib1_flag ^ obj_info->attribute[1] ) & ( MTD_OBJ_ATTR1_FLIP_H | MTD_OBJ_ATTR1_FLIP_V ) )
                                         >> MTD_OBJ_ATTR1_FLIP_SHIFT );
                for( i = 0; i < oam_size_x; ++i ){
                    
                    local_pos[MTD_X]    = (fx32)( ( ( (fx32)obj_info->attribute[1] << ( 32 - 9 ) ) >> ( 32 - 9 ) )    // アトリビュートの座標を符号拡張
                                            - cnv_ptn->center[MTD_X] + pOffst[l * 2 + MTD_X]
                                            + ( i * ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) )
                                            + ( (8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) >> 1) )  << FX32_DEC_SIZE;
                    
                    local_pos[MTD_Y]    = (fx32)( ( ( (fx32)obj_info->attribute[0] << ( 32 - 8 ) ) >> ( 32 - 8 ) )    // アトリビュートの座標を符号拡張
                                            - cnv_ptn->center[MTD_Y] + pOffst[l * 2 + MTD_Y]
                                            + ( j * 8 )
                                            + ( 8 >> 1 ))   << FX32_DEC_SIZE;
                        
                    obj_pos[MTD_X]  = center_pos[MTD_X]     // 表示座標
                        - ( (fx32)( ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift) ) >> 1 )    // 半スプライトサイズ分ずらしたので元に戻している
                            + (( FX_Mul( affine_flip[affine_index]._00, local_pos[MTD_X] ) + FX_Mul( affine_flip[affine_index]._10, local_pos[MTD_Y] ))>> FX32_DEC_SIZE );    // ローカル座標を回転拡縮している
                    obj_pos[MTD_Y]  = center_pos[MTD_Y]     // 表示座標
                        - ( (fx32)( 8 ) >> 1 )    // 半スプライトサイズ分ずらしたので元に戻している
                            + (( FX_Mul( affine_flip[affine_index]._01, local_pos[MTD_X] ) + FX_Mul( affine_flip[affine_index]._11, local_pos[MTD_Y] ) )>> FX32_DEC_SIZE );    // ローカル座標を回転拡縮している

                    // 倍角している場合は座標を補正する
                    if( MTD_OBJ_ATTR0_BAIKAKU & attrib0_flag ) {
                        obj_pos[MTD_X]  -= (fx32)obj_size[MTD_X] << ( FX32_DEC_SIZE - 1 );
                        obj_pos[MTD_Y]  -= (fx32)obj_size[MTD_Y] << ( FX32_DEC_SIZE - 1 );
                        *(u32*)obj_size <<= 1;
                    }
                    // オブジェ1つ1つをクリッピングチェックする
                    if( MTD_ACT_FLAG_ENABLE_CLIP & act->flag ) {
                        if(
                            0 >= obj_pos[MTD_X] + ( 8 << _mt_ge2_table[act->ge_type]->vram_obj_unit_shift )
                            || OBD_LCD_X <= obj_pos[MTD_X]
                            || 0 >= obj_pos[MTD_Y] + 8
                            || OBD_LCD_Y <= obj_pos[MTD_Y] )
                            {
                                // 画面外にあるため描画する必要なし
                                ++cha_name_add;
                                ++l;
                                continue;
                            }
                    }
                    // まだ設定していないアフィンであればパラメータを設定する
                    if( 0 > (s16)use_affine_param[affine_index] ) {

                        use_affine_param[affine_index]  =
                            mtSprGetAffine( act->ge_type, &affine_inv_flip[affine_index] );

                        if( 0 <= (s16)use_affine_param[affine_index] )
                            {
                                G2_SetOBJAffine(
                                    (GXOamAffine*)&ge->spr_oam_buffer[use_affine_param[affine_index] << 2],
                                    &affine_inv_flip[affine_index] );
                            }
                        else
                            {
                                // アフィンパラメータが足りなかったため描画をあきらめる
                                ++cha_name;continue;
                            }
                    }

                    // OAMソートに登録する
                    oam_sort    = mtSprGetOamSort( act->ge_type, act->priority );

                    oam_sort->attribute[0]  = (u16)( 1 << MTD_OBJ_ATTR0_SHAPE_SHIFT| ( obj_pos[MTD_Y] & MTD_OBJ_ATTR0_Y_MASK ) | MTD_OBJ_ATTR0_ZOOM);
                    oam_sort->attribute[0]  ^= attrib0_flag;

                    oam_sort->attribute[1]  = (u16)( ( obj_pos[MTD_X] & MTD_OBJ_ATTR1_X_MASK ) );
                    oam_sort->attribute[1]  = (u16)( ( oam_sort->attribute[1] & ~MTD_OBJ_ATTR1_ZOOM_PARAM )
                                                     | ( use_affine_param[affine_index] << MTD_OBJ_ATTR1_ZOOM_PARAM_SHIFT ) );

                    oam_sort->attribute[2]  = (u16)(
                        ( ( (( ( obj_info->attribute[2] & MTD_OBJ_ATTR2_NAME_MASK ) + ( 1 << dest_name_shift ) - 1 ) >> dest_name_shift)+ cha_name + cha_name_add ) & MTD_OBJ_ATTR2_NAME_MASK )
                        | ( ( ( act->flag & MTD_ACT_FLAG_BG_NO_MASK ) >> MTD_ACT_FLAG_BG_NO_SHIFT ) << MTD_OBJ_ATTR2_PRIORITY_BG_SHIFT )
                        | ( ( ( act->plt_ofst_no << MTD_OBJ_ATTR2_PALETTE_SHIFT ) ) & MTD_OBJ_ATTR2_PALETTE_MASK ) );
                    ++cha_name_add;
                    ++l;
                }
            }
        }
    }
}*/





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

/*
 * Revision 1.84  2005/09/26 09:50:31  use1146
 * 3dオブジェクト拡大率の設定が無かったのを修正
 *
 * Revision 1.83  2005/09/23 11:17:41  use1146
 * 範囲外チェックのオフセットを各要素ずつに設定できるように対応
 *
 * Revision 1.82  2005/09/22 08:59:09  use1146
 * 押し条件に自分に乗っていない時を追加
 *
 * Revision 1.81  2005/09/19 09:03:46  use1146
 * 保持座標を32bit化
 *
 * Revision 1.80  2005/09/17 09:08:20  use1146
 * 振動無視フラグ追加、FLOW無視フラグ追加
 *
 * Revision 1.79  2005/09/06 14:00:50  use1173
 * プリコンパイルヘッダ対応
 *
 * Revision 1.78  2005/08/30 13:56:34  use1146
 * BELT時のZ値増幅
 *
 * Revision 1.77  2005/08/29 10:09:50  use1146
 * サウンド位置設定関数追加
 *
 * Revision 1.76  2005/08/24 09:58:44  use1146
 * 3DDISPフラグを落としていたのを修正
 *
 * Revision 1.75  2005/08/22 11:21:53  use1159
 * objDataLoad関数の不文字列バッファが足りていなかったので拡張
 *
 * Revision 1.74  2005/08/19 05:20:04  use1146
 * BELT奥行き修正
 *
 * Revision 1.73  2005/08/11 13:32:35  use1146
 * 揺れ移動関数、引き数追加
 *
 * Revision 1.72  2005/08/08 09:07:10  use1146
 * パレットアニメ、オートスクロール対応
 *
 * Revision 1.71  2005/08/04 06:05:54  use1159
 * アニメーションタイプ名変更による対応
 *
 * Revision 1.70  2005/07/27 04:32:37  use1146
 * Flow値をDirFall参照した値にするように対応
 *
 * Revision 1.69  2005/07/26 07:41:59  use1146
 * 揺れ動き関数追加、押しY軸削除
 *
 * Revision 1.68  2005/07/20 06:30:19  use1146
 * オブジェクト押し対応
 *
 * Revision 1.67  2005/07/19 06:24:02  use1146
 * アクション優先設定関数作成
 *
 * Revision 1.66  2005/07/13 06:53:57  use1146
 * DISPフラグ落とし修正ミスを修正
 *
 * Revision 1.65  2005/07/12 10:59:26  use1146
 * ActionLoad修正
 *
 * Revision 1.64  2005/07/11 10:38:51  use1146
 * NARCからファイル取り出し関数作成、モデルファイルからテクスチャのみ解放関数作成
 *
 * Revision 1.63  2005/07/05 05:02:47  use1146
 * BELT削除対応
 *
 * Revision 1.62  2005/06/29 09:59:29  use1146
 * オブジェクトの処理バーを黒く
 *
 * Revision 1.61  2005/06/28 08:51:25  use1146
 * ファイル読み込みにDMA未使用時はWaitを行わない
 *
 * Revision 1.60  2005/06/28 06:09:00  use1146
 * RenderObjのRECORD対応
 *
 * Revision 1.59  2005/06/27 07:37:32  use1146
 * 画面外チェックミス修正
 *
 * Revision 1.58  2005/06/24 09:19:20  use1146
 * 画面外判定、2画面別々対応
 *
 * Revision 1.57  2005/06/16 05:07:50  use1146
 * 角度ループ16修正
 *
 * Revision 1.56  2005/06/13 11:07:11  use1146
 * AUTOキャラサイズ未設定の関数があったので修正
 *
 * Revision 1.55  2005/06/10 11:09:44  use1146
 * 3Dスプライト共有テクスチャ対応
 *
 * Revision 1.54  2005/06/08 03:14:59  use1146
 * 回転修正
 *
 * Revision 1.53  2005/06/03 02:45:08  use1146
 * DiffSet関数追加
 *
 * Revision 1.52  2005/05/31 09:06:15  use1146
 * BELT対応
 *
 * Revision 1.51  2005/05/26 07:47:01  use1146
 * 複数シェイプ表示対応
 *
 * Revision 1.50  2005/05/19 08:37:56  use1146
 * 重力変化対応、死亡範囲可変対応
 *
 * Revision 1.49  2005/05/13 10:26:09  use1146
 * AUTOキャラサイズ追加
 *
 * Revision 1.48  2005/05/12 11:58:52  use1146
 * 速度関数変更
 *
 * Revision 1.47  2005/05/09 07:22:45  use1146
 * デバッグレクトキャラdefine修正
 *
 * Revision 1.46  2005/05/09 05:19:19  use1146
 * 座標無視フラグ追加
 *
 * Revision 1.45  2005/04/28 13:06:06  use1146
 * 地上では地形チェックを8ドット区切りにする
 *
 * Revision 1.44  2005/04/28 05:12:45  use1146
 * 地形チェック段階j変更
 *
 * Revision 1.43  2005/04/27 12:57:27  use1146
 * タッチパネルチェックのカメラ修正
 *
 * Revision 1.42  2005/04/27 08:15:58  use1146
 * FALLMAX修正
 *
 * Revision 1.41  2005/04/25 06:23:13  use1146
 * NOUPDATE対応、タッチパネルと矩形チェック追加
 *
 * Revision 1.40  2005/04/22 05:56:03  use1146
 * NOHIT修正
 *
 * Revision 1.39  2005/04/22 02:43:42  use1146
 * ポーズ中、アニメ停止追加
 *
 * Revision 1.38  2005/04/20 03:36:43  use1146
 * ポーズ追加
 *
 * Revision 1.37  2005/04/18 09:16:56  use1159
 * ライトベクトルを外部から設定できるように変更
 *
 * Revision 1.36  2005/04/15 02:15:56  use1146
 * 3dアクション外部使用に対応した引数に修正
 *
 * Revision 1.35  2005/04/12 04:52:46  use1159
 * 3Dデバッグ矩形表示関数のコンパイルエラーの修正
 *
 * Revision 1.34  2005/04/12 04:47:45  use1159
 * 3D矩形あたり表示関数の修正
 * カメラのパースをスケールWに対応
 *
 * Revision 1.33  2005/04/11 03:26:54  use1159
 * 3DスプライトのLEFTフラグが毎回フリップされる不具合の修正
 * アクションセット関数を3Dスプライトなどに対応
 *
 * Revision 1.32  2005/04/08 12:48:58  use1146
 * ブレンドフラグ修正
 *
 * Revision 1.31  2005/04/07 06:28:52  use1146
 * 各3Dオブジェクト描画、データ管理対応
 *
 * Revision 1.30  2005/04/06 10:00:35  use1146
 * 拡縮対応
 *
 * Revision 1.29  2005/03/31 06:37:40  use1146
 * 分割地形チェックで地形フラグが消去されるバグ修正
 *
 * Revision 1.28  2005/03/31 05:40:48  use1146
 * 傾斜調整
 *
 * Revision 1.27  2005/03/30 07:24:58  use1146
 * 動物パレット追加
 *
 * Revision 1.26  2005/03/29 07:50:54  use1159
 * デバッグ矩形が多重解放されてしまう不具合に対応
 *
 * Revision 1.25  2005/03/29 05:28:58  use1146
 * 親ポインタ追加
 *
 * Revision 1.24  2005/03/24 06:05:22  use1146
 * ヒットストップ対応
 *
 * Revision 1.23  2005/03/22 06:32:28  use1146
 * ＯＡＭ分割表示追加
 *
 * Revision 1.22  2005/03/17 11:35:26  use1146
 * 角度追加
 *
 * Revision 1.21  2005/03/16 12:39:17  use1159
 * プレイヤーを３つのライトで照らすように対応
 *
 * Revision 1.20  2005/03/15 10:36:48  use1159
 * アクションのアフィン描画の引数の変更による修正
 *
 * Revision 1.19  2005/03/15 08:48:49  use1146
 * デバッグ矩形表示を一括化
 *
 * Revision 1.18  2005/03/14 03:49:43  use1159
 * フル3D時の当たり表示の追加
 *
 * Revision 1.17  2005/03/08 11:15:09  use1146
 * RIDE対応、共有VRAM対応
 *
 * Revision 1.16  2005/03/04 09:49:23  use1146
 * 画面外無視フラグのチェックが抜けていたのを修正
 *
 * Revision 1.15  2005/03/04 02:29:31  use1146
 * 3Dオブジェクトの2D表示対応
 *
 * Revision 1.14  2005/03/03 13:03:06  use1159
 * オブジェクトをカメラの正面に向ける処理の大幅変更
 * 描画時のカメラのポインタを格納する変数の追加
 *
 * Revision 1.13  2005/03/02 12:02:30  use1146
 * アーカイブ、3D対応
 *
 * Revision 1.12  2005/03/01 05:32:28  use1146
 * RIDE対応
 *
 * Revision 1.11  2005/02/25 08:33:46  use1146
 * 移動量保持など対応
 *
 * Revision 1.10  2005/02/21 09:18:28  use1146
 * 複数パーツ表示対応
 *
 * Revision 1.9  2005/02/14 09:09:21  use1146
 * 反転修正
 *
 * Revision 1.8  2005/02/10 10:22:36  use1146
 * 死亡判定修正
 *
 * Revision 1.7  2005/02/09 11:35:10  use1146
 * 3Dオブジェクトの矩形アクション対応
 *
 * Revision 1.6  2005/02/07 13:56:01  use1159
 * objObjectAction3D関数が正しく動作するように修正
 *
 * Revision 1.5  2005/02/03 06:38:09  use1146
 * オブジェクト機能追加
 *
 * Revision 1.4  2005/01/27 05:47:07  use1146
 * 坂対応
 *
 * Revision 1.3  2005/01/20 06:09:18  use1146
 * アクションアドレス修正
 *
 * Revision 1.2  2005/01/20 06:02:23  use1146
 * アクションヘッダアドレス修正
 *
 * Revision 1.1  2005/01/20 03:09:27  use1146
 * 登録
 *
 */