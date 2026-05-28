// ==========================================================================
/*!
  @file gsMainSys.c
  @brief メインシステム

  @author Ishizaki
                Copyright(c) 2009 Dimps

  $Id: gsMainSys.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
 */
// ==========================================================================
// ==========================================================================
/* [イベントシステム使用時サンプル]
	・ファイル構成
		usrSampleMainSys.c
		usrSampleMainSys.h
			システムユーザー初期化、イベントデータ、イベント変更関数、ダミールーチン、
			デバック機能(ランチャー)等を含む。
			イベント変更関数は、各イベント毎のソース内で直接行っても問題ない。
			システム変数等のみを参照する分岐のみのイベント変更関数はここで管理するとよい。
		usrSampleMSysFunc.h
			不特定多数が更新する内容を定義。(各イベントに登録する関数、ランチャー関数等)
			基本的には、管理者以外が更新する可能性のあるものはここに定義し、
			なるべくusrSampleMainSys.c/hを更新しなくてもよいように保つ。

	・ランチャー
		直接ゲームに関係しない機能(ビュワー等)や、特殊な起動を行いたい場合
		(イベントシステムに登録されていないデバックメニュー等)を起動する際に使用する。
		登録関数は usrSampleMSysFunc.h に定義。

	・デバックメニュー(デモセレクト)
		ゲーム中のイベントを直接呼べるようにして、デモの確認を容易にしたもの。
*/
// ==========================================================================
/*
 * $Log: gsMainSys.c,v $
 *
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "syEvtSys.h"
#include "ao.h"
#include "gsInit.h"
#include "gsSound.h"
#include "hgSoundPlatform.h"

#include "gsMainSys.h"
#include "gsReboot.h"

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
#define GSD_EVTSYS_TCB_PRI					(0x0100)	//!< イベントシステムTCBプライオリティ
#define GSD_EVTSYS_TCB_GROUP				(0x0E)		//!< イベントシステムTCBグループ


#if _PC
#ifndef GSD_DEBUG_MODEL_BUILD
	#define GSD_PRECOMPILED_SHADER_PATH	"NNSTDSHADER/SHADER.AMB"
#endif
#elif _XBOX
#ifndef GSD_DEBUG_MODEL_BUILD
	#define GSD_PRECOMPILED_SHADER_PATH	"NNSTDSHADER/SHADER.AMB"
#endif
#elif _PS3
	//#define GSD_PRECOMPILED_SHADER_PATH	"NNSTDSHADER/shader_1.csb"
	#define GSD_PRECOMPILED_SHADER_PATH	"NNSTDSHADER/SHADER_1.CSB"
#endif
///
enum {
	GSE_SHADER_LOAD_STATE_NOTHING	= 0,
	GSE_SHADER_LOAD_STATE_LOAD,
	GSE_SHADER_LOAD_STATE_REG,
	GSE_SHADER_LOAD_STATE_COMPLETE
};

/// シェーダー設定用
typedef struct tag_GSS_SHADER_WORK {
	AMS_FS			*fs_req;
	MTS_TASK_TCB	*tcb;
	s32				state;
	s32				reg_id;
	void			*buf;
	char			file_path[128];
} GSS_SHADER_WORK;


#if defined (MTD_DEBUG)
/****************************************************************************/
// ランチャー
/****************************************************************************/
/// ランチャー使用キーリスト
enum {
	GSD_DEBUG_LAUNCHER_KEY_UP		= 0,
	GSD_DEBUG_LAUNCHER_KEY_DOWN,
	GSD_DEBUG_LAUNCHER_KEY_LEFT,
	GSD_DEBUG_LAUNCHER_KEY_RIGHT,
	GSD_DEBUG_LAUNCHER_KEY_A,
	GSD_DEBUG_LAUNCHER_KEY_B,

	GSD_DEBUG_LAUNCHER_KEY_NUM
};
#endif // #if defined (MTD_DEBUG)


//----- External Declarations -----------------------------------------------
// alice デバック表示設定
#if defined (MTD_DEBUG)
extern Sint32 _am_dbg_display_mode;
#endif
#if _IPHONE
// AppMainより
extern BOOL _am_sample_is_suspended;
extern BOOL _am_sample_is_sleep; // スリープに移行するか否か
extern BOOL _am_sample_is_accel; // 加速度センサーを使うか否か
#endif // _IPHONE

//----- Static Declarations -------------------------------------------------
static void gsMainSysSystemInitMain(MTS_TASK_TCB *tcb);
static void gsMainSysSystemInitMain2(MTS_TASK_TCB *tcb);

#if defined GSD_PRECOMPILED_SHADER_PATH
static void gsMainSysLoadShaderWait(MTS_TASK_TCB *tcb);
#endif

//----- Global Variables ----------------------------------------------------
GSS_MAIN_SYS_INFO	g_gs_main_sys_info = {0};				/* メインシステム情報 */

//----- Local Variables -----------------------------------------------------
/// シェーダー用ワーク
GSS_SHADER_WORK		gs_shader_work = {0};

// エメラルドGETステージテーブル
const static u32 gs_main_eme_get_act_no_tbl[13] = {
	 0xffff,		// 取得していないことを示す
	 0,  1,  2,
	 4,  5,  6,
	 8,  9, 10,
	12, 13, 14,
};


/****************************************************************************/
// イベントデータ
/*
	必要となるイベント(デモ, ゲーム等)のデータを登録する
	登録内容は以下の通り
		初期化関数
		終了時実行関数
		ソフトリセット時関数
		システム側初期化関数
		システム側終了関数
		次イベントID
		属性
		オーバーレイ
	関数については最低限、初期化関数を登録する。
	次イベントは、通常イベント移行時に分岐する可能性のあるイベントIDを登録する。
	イベントID定義はヘッダにおいて定義し(USRE_EVT_ID)、SYS_EVT_DATAの内容が
	IDの並びと同じ様にしておく事。
	オーバーレイは使用しない場合は、すべてを (FSOverlayID)-1 に設定する。
	使用する場合は、(FSOverlayID)-1 設定を残さない事。
*/	
/****************************************************************************/
#include "gsMSFunc.inc"

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GsInitUser
/*!
 *	システム初期化ユーザー関数
 */
// ==========================================================================
void GsInitUser(void)
{
	OS_TPrintf("gsMainSys::GsInitUser() Custom user init called.\n");

	//メインシステム情報の初期化
	GsMainSysInfoInit(GsGetMainSysInfo());

	// MT仕様タスクシステム初期化
	mtTaskInitSystem();

#if defined(MTD_DEBUG)
#if defined (GSD_DEBUG_LAUNCHER)
	/* ランチャー */
	{
		u16	key = (u16)PAD_MDIRECT(0);
		s32	i;

		void	(*gs_dbg_launcher_func[GSD_DEBUG_LAUNCHER_KEY_NUM])(void) = {
			GSD_LAUNCHER_UP_FUNC,
			GSD_LAUNCHER_DOWN_FUNC,
			GSD_LAUNCHER_LEFT_FUNC,
			GSD_LAUNCHER_RIGHT_FUNC,
			GSD_LAUNCHER_A_FUNC,
			GSD_LAUNCHER_B_FUNC,
		};

		u16		gs_dbg_launcher_key_list[GSD_DEBUG_LAUNCHER_KEY_NUM] = {
			KEY_L_UP,
			KEY_L_DOWN,
			KEY_L_LEFT,
			KEY_L_RIGHT,
			KEY_R_DOWN,
			KEY_R_RIGHT,
		};

		for (i = 0; i < GSD_DEBUG_LAUNCHER_KEY_NUM; i++) {
			if (key & gs_dbg_launcher_key_list[i]) {
				if (gs_dbg_launcher_func[i]) {
					gs_dbg_launcher_func[i]();
					return;
				}
			}
		}
	}
#endif	// #if defined (GSD_DEBUG_LAUNCHER)
#endif	// #if defined(MTD_DEBUG)

#if defined(HOG_ALPHA_ROM) || _IPHONE
	// デバック表示OFF
#if defined (MTD_DEBUG)
	_am_dbg_display_mode = 0;
#endif // #if defined (MTD_DEBUG)
	_am_fs_display_mode = 0;
#endif //defined(HOG_ALPHA_ROM) || defined(HOG_PRESENT_ROM_IPHONE)

	/* 画面設定初期化 */

	/* イベントシステム初期化 */
	SyInitEvtSys(_gs_evt_data, GSD_EVT_ID_NUM, GSD_EVT_ID_SYS_INIT, TRUE,
				 GSD_EVTSYS_TCB_PRI, GSD_EVTSYS_TCB_GROUP);
#ifndef GMD_DEBUG_NO_CREATE_CRIAUDIO
	// サウンドシステム初期化
	GsSoundInit();
#endif // GMD_DEBUG_NO_CREATE_CRIAUDIO
#if _WII
	// Wii ホームボタンメニュー入出時サウンド操作コールバック設定
	GsSoundRegisterHBMCallbacks(HgSoundPfWiiCallbackEnterHBM, NULL,
								HgSoundPfWiiCallbackLeaveHBM, NULL,
								HgSoundPfWiiCallbackFadeHBM, NULL);
#endif /* _WII */

#if defined (MTD_DEBUG)
//	/* matsuri デバック設定 */
//	_mt_debug_flag &= ~MTD_DEBUG_DISABLE_BGM;	// BGM ON
#endif	// #if defined (MTD_DEBUG)
}


// ==========================================================================
// GsExitUser
/*!
 *	システム終了処理ユーザー関数
 */
// ==========================================================================
void GsExitUser(void)
{
	// その他の終了処理
	GsOtherExit();

#if _WII
	// Wii ホームボタンメニュー入出時サウンド操作コールバッククリア
	GsSoundRegisterHBMCallbacks(NULL, NULL,
								NULL, NULL,
								NULL, NULL);
#endif /* _WII */
	// サウンドシステム終了
	GsSoundExit();

#if defined GSD_PRECOMPILED_SHADER_PATH
	// プリコンパイルシェーダーファイル開放
	amMemFree(gs_shader_work.buf);
	gs_shader_work.buf = NULL;
#endif
}

// ==========================================================================
// システム初期化
// ==========================================================================
// ==========================================================================
// GsMainSysSystemInitEvent
/*!
 *	システム初期化用イベント
 */
// ==========================================================================
void GsMainSysSystemInitEvent(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	MTM_TASK_MAKE_TCB(gsMainSysSystemInitMain, NULL/*dest*/,
						0/*flag*/, 0xFFFF/*pause_level*/,
						0x1000/*prio*/, 0/*group*/,
						0/*work_size*/, "GS_SYS_INIT");

#if defined GSD_PRECOMPILED_SHADER_PATH
	// シェーダー
	GsMainSysLoadShader(GSS_BASE_PATH GSD_PRECOMPILED_SHADER_PATH);
#endif
}

// ==========================================================================
// gsMainSysSystemInitMain
/*!
 *	システム初期化
 *
 *	@param	tcb	[in]
 */
// ==========================================================================
void gsMainSysSystemInitMain(MTS_TASK_TCB *tcb)
{
	// 初期化チェック
	if (!GsMainSysCheckLoadShaderFinished()) {
		return;
	}

	// その他の初期化開始
	GsInitOtherStart();

	// 初期化2へ遷移
	mtTaskChangeTcbProcedure(tcb, gsMainSysSystemInitMain2);
}

// ==========================================================================
// gsMainSysSystemInitMain2
/*!
 *	システム初期化2
 *
 *	@param	tcb	[in]
 */
// ==========================================================================
void gsMainSysSystemInitMain2(MTS_TASK_TCB *tcb)
{
	// その他の初期化完了チェック
	if (!GsInitOtherIsInitialized()) {
		return;
	}

	// システム初期化終了
	mtTaskClearTcb(tcb);

#if defined (MTD_DEBUG)
#if !_IPHONE
	if (PAD_DIRECT(0) & KEY_R_UP) {
#else
	if (amTpIsTouchOn(3)) {
#endif
		// デバッグメニューへ
		SyDecideEvtCase(3);
	}
	else if (!GsRebootIsTitleReboot()) {
		// 通常遷移
#if _PC || _XBOX
		if (GsEnvGetRegion() == GSD_REGION_US) {
			if (GsEnvIsRegionAsia()) {
				// KR
				SyDecideEvtCase(0);
			}
			else {
				// US
				SyDecideEvtCase(1);
			}
		}
		else {
			SyDecideEvtCase(0);
		}
#else
		SyDecideEvtCase(0);
#endif
	}
	else {
		// タイトルへスキップ
		SyDecideEvtCase(2);
	}
#else
	if (!GsRebootIsTitleReboot()) {
		// 通常遷移
#if _PC || _XBOX
		if (GsEnvGetRegion() == GSD_REGION_US) {
			if (GsEnvIsRegionAsia()) {
				// KR
				SyDecideEvtCase(0);
			}
			else {
				// US
				SyDecideEvtCase(1);
			}
		}
		else {
			SyDecideEvtCase(0);
		}
#else
		SyDecideEvtCase(0);
#endif
	}
	else {
		// タイトルへスキップ
		SyDecideEvtCase(2);
	}
#endif // defined (MTD_DEBUG)

	// イベント遷移
	SyChangeNextEvt();
}

#if _IPHONE
// ==========================================================================
// GsMainSysSetSleepFlag
/*!
 *	ハード的にスリープに入るべきか否かを設定します。
 *	標準は入る設定になっています。
 *	
 *	@param	flag	[in]	TRUE:スリープに入る	FALSE:入らない
 */
// ==========================================================================
void GsMainSysSetSleepFlag(BOOL flag)
{
	_am_sample_is_sleep = flag;
}

// ==========================================================================
// GsMainSysIsSuspendedSystem
/*!
 *	ハード的にサスペンドさせられたかどうかをチェックする
 *	
 *	@return	TRUE : サスペンドしている
 *			FALSE: サスペンドしてない
 */
// ==========================================================================
BOOL GsMainSysIsSuspendedSystem(void)
{
	return _am_sample_is_suspended;
}

// ==========================================================================
// GsMainSysSetSuspendedFlag
/*!
 *	ハード的なサスペンドのフラグを設定する
 *	ゲーム側からは呼ばないようにしてください。
 *	
 *	@param flag [in] TRUE:前フレームまでサスペンドした / FALSE:してない
 *	
 */
// ==========================================================================
void GsMainSysSetSuspendedFlag(BOOL flag)
{
	if (flag) {
		amFlagOn(g_gs_main_sys_info.sys_flag, GSD_MAINSYS_SYS_FLAG_SUSPEND);
	}
	else {
		amFlagOff(g_gs_main_sys_info.sys_flag, GSD_MAINSYS_SYS_FLAG_SUSPEND);
	}
}

// ==========================================================================
// GsMainSysGetSuspendedFlag
/*!
 *	前フレームでサスペンドしていたか否かを取得
 *	
 *	@return TRUE:サスペンドした履歴があった
 *			FALSE:無い
 *	
 */
// ==========================================================================
BOOL GsMainSysGetSuspendedFlag(void)
{
	BOOL flag = FALSE;
	
	if (g_gs_main_sys_info.sys_flag & GSD_MAINSYS_SYS_FLAG_SUSPEND) {
		flag = TRUE;
	}
	
	return flag;
}

// ==========================================================================
// GsMainSysSetAccelFlag
/*!
 *	ハード的に加速度センサーを使うか否かを設定します。
 *	標準は無効になっています。
 *	
 *	@param	flag	[in]	TRUE:使用	FALSE:使用しない
 */
// ==========================================================================
void GsMainSysSetAccelFlag(BOOL flag)
{
	_am_sample_is_accel = flag;
}
#endif // _IPHONE

// ==========================================================================
// Utility
// ==========================================================================
// ==========================================================================
// GsMainSysInfoInit
/*!
 *	メインシステム情報の初期化
 *
 *	@return	メインシステム情報構造体アドレス
 */
// ==========================================================================
void GsMainSysInfoInit(GSS_MAIN_SYS_INFO *gs_main)
{
	MTM_ASSERT(gs_main);

	memset(gs_main, 0, sizeof(GSS_MAIN_SYS_INFO));

	// ゲーム解像度設定
#if _IPHONE
//	gs_main->sys_disp_width		= 960.f;
//	gs_main->sys_disp_height	= 640.f;
	gs_main->sys_disp_width		= 480.f;
	gs_main->sys_disp_height	= 320.f;
#else
	if (_am_draw_video.wide_screen) {
		gs_main->sys_disp_width	= 1280.f;//(f32)((960 * 16) / 12);
		gs_main->sys_disp_height	= 720.f;
	}
	else {
		gs_main->sys_disp_width	= 960.f;
		gs_main->sys_disp_height	= 720.f;
	}
#endif

	// 標準ゲーム難易度設定
	gs_main->level = GSD_GAME_LEVEL_NORMAL;

	// 各種初期化(構造体etc...)
	//セーブデータ展開データ
	gs_main->backup.Init();
}


// ==========================================================================
// GsMainSysIsStageClear
/*!
 *	引数に指定したステージがクリア済みかどうかを返す関数
 *	※この関数ではソニックかスーパーソニックのどちらか片方で
 *	クリアしているとTRUEを返します。
 *
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
BOOL GsMainSysIsStageClear(s32 stage_id)
{
	u32 tmp_stage_id = 0;
	
	MTM_ASSERT(stage_id >= 0 && stage_id < GSD_MAIN_STAGE_ID_MAX);
	
	// スペステの場合
	if (stage_id >= GSD_MAIN_STAGE_ID_SS1) {
		tmp_stage_id = (u32)(stage_id - (GSD_MAIN_STAGE_ID_SS1 - gs::backup::ESpecialStage::Stage1));
		
		// スペシャルステージデータインスタンス作成
		gs::backup::SSpecial &spe_data
			= gs::backup::SSpecial::CreateInstance();
		
		// 通常ソニックのハイスコアが初期値の場合
		if (spe_data[tmp_stage_id].GetHighScore()
			!= gs::backup::SSpecialSolo::c_high_score_max_limit
			&& spe_data[tmp_stage_id].IsGetEmerald() == true) {
			// クリア済み
			return TRUE;
		}
		
		// レコードタイムが初期値の場合
		if (spe_data[tmp_stage_id].GetFastTime()
			!= gs::backup::SSpecialSolo::c_fast_time_max_limit) {
			// クリア済み
			return TRUE;
		}
	}
	
	// 通常ステージの場合
	else {
		// ステージデータインスタンス
		gs::backup::SStage &data
			= gs::backup::SStage::CreateInstance();
		
		// ファイナルステージID補正
		if (stage_id >= GSD_MAIN_STAGE_ID_FINAL_1) {
			tmp_stage_id = GSD_MAIN_STAGE_ID_FINAL_1;
		}
		else {
			tmp_stage_id = (u32)stage_id;
		}
		
		// 通常ソニックのハイスコアが初期値の場合
		if (data[tmp_stage_id].GetHighScore(false)
			!= gs::backup::SStageSolo::c_high_score_max_limit) {
			// クリア済み
			return TRUE;
		}
		
		// スーパーソニックのハイスコアが初期値の場合
		if (data[tmp_stage_id].GetHighScore(true)
			!= gs::backup::SStageSolo::c_high_score_max_limit) {
			// クリア済み
			return TRUE;
		}
		
		
		// レコードタイムが初期値の場合
		if (data[tmp_stage_id].GetFastTime(false)
			!= gs::backup::SStageSolo::c_fast_time_max_limit) {
			// クリア済み
			return TRUE;
		}
		
		// レコードタイムが初期値の場合(スーパーソニック)
		if (data[tmp_stage_id].GetFastTime(true)
			!= gs::backup::SStageSolo::c_fast_time_max_limit) {
			// クリア済み
			return TRUE;
		}
	}
	
	
	// 未クリア
	return FALSE;
}



// ==========================================================================
// GsMainSysIsStageSonicClear
/*!
 *	引数に指定したステージが
	ソニックを使用してクリア済みかどうかを返す関数
 *
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
BOOL GsMainSysIsStageSonicClear(s32 stage_id)
{
	u32 tmp_stage_id = 0;
	
	MTM_ASSERT(stage_id >= 0 && stage_id < GSD_MAIN_STAGE_ID_MAX);
	MTM_ASSERT(stage_id != GSD_MAIN_STAGE_ID_ENDING);
	
	// スペステの場合
	if (stage_id >= GSD_MAIN_STAGE_ID_SS1) {
		tmp_stage_id = (u32)(stage_id - (GSD_MAIN_STAGE_ID_SS1 - gs::backup::ESpecialStage::Stage1));
		
		// スペシャルステージデータインスタンス作成
		gs::backup::SSpecial &spe_data
			= gs::backup::SSpecial::CreateInstance();
		
		// 通常ソニックのハイスコアが初期値の場合
		if (spe_data[tmp_stage_id].GetHighScore()
			!= gs::backup::SSpecialSolo::c_high_score_max_limit
			&& spe_data[tmp_stage_id].IsGetEmerald() == true) {
			// クリア済み
			return TRUE;
		}
		
		// レコードタイムが初期値の場合
		if (spe_data[tmp_stage_id].GetFastTime()
			!= gs::backup::SSpecialSolo::c_fast_time_max_limit) {
			// クリア済み
			return TRUE;
		}
	}
	
	// 通常ステージの場合
	else {
		// ステージデータインスタンス
		gs::backup::SStage &data
			= gs::backup::SStage::CreateInstance();
		
		// ファイナルステージID補正
		if (stage_id >= GSD_MAIN_STAGE_ID_FINAL_1) {
			tmp_stage_id = GSD_MAIN_STAGE_ID_FINAL_1;
		}
		else {
			tmp_stage_id = (u32)stage_id;
		}
		
		// 通常ソニックのハイスコアが初期値の場合
		if (data[tmp_stage_id].GetHighScore(false)
			!= gs::backup::SStageSolo::c_high_score_max_limit) {
			// クリア済み
			return TRUE;
		}
		
		// スーパーソニックのハイスコアが初期値の場合
		if (data[tmp_stage_id].GetHighScore(true)
			!= gs::backup::SStageSolo::c_high_score_max_limit) {
			// クリア済み
			return TRUE;
		}
		
		
		// レコードタイムが初期値の場合
		if (data[tmp_stage_id].GetFastTime(false)
			!= gs::backup::SStageSolo::c_fast_time_max_limit) {
			// クリア済み
			return TRUE;
		}
		
		// レコードタイムが初期値の場合(スーパーソニック)
		if (data[tmp_stage_id].GetFastTime(true)
			!= gs::backup::SStageSolo::c_fast_time_max_limit) {
			// クリア済み
			return TRUE;
		}
	}
	
	
	// 未クリア
	return FALSE;
}



// ==========================================================================
// GsMainSysIsStageSuperSonicClear
/*!
 *	引数に指定したステージがスーパーソニックにて
 *	クリア済みかどうかを返す関数
 *	
 *	※引数のステージIDはZONE1～4とZONE FINALのステージIDを指定してください。
 *	それ以外のステージIDだとアサートになります。
 *	
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
BOOL GsMainSysIsStageSuperSonicClear(s32 stage_id)
{
	u32 tmp_stage_id = 0;
	
	MTM_ASSERT(stage_id >= 0 && stage_id <= GSD_MAIN_STAGE_ID_FINAL_5);
	MTM_ASSERT(stage_id != GSD_MAIN_STAGE_ID_ENDING);
	
	tmp_stage_id = (u32)stage_id;
	
	// もしステージIDがファイナル以上の場合、1-1に置き換え
	if (tmp_stage_id > GSD_MAIN_STAGE_ID_FINAL_5) {
		tmp_stage_id = 0;
	}
	
	// ステージデータインスタンス
	gs::backup::SStage &data
		= gs::backup::SStage::CreateInstance();
	
	// ファイナルステージID補正
	if (stage_id >= GSD_MAIN_STAGE_ID_FINAL_1) {
		tmp_stage_id = GSD_MAIN_STAGE_ID_FINAL_1;
	}
	else {
		tmp_stage_id = (u32)stage_id;
	}
	
	// スーパーソニックのハイスコアが初期値の場合
	if (data[tmp_stage_id].GetHighScore(true)
		!= gs::backup::SStageSolo::c_high_score_max_limit) {
		// クリア済み
		return TRUE;
	}
	
	// レコードタイムが初期値の場合(スーパーソニック)
	if (data[tmp_stage_id].GetFastTime(true)
		!= gs::backup::SStageSolo::c_fast_time_max_limit) {
		// クリア済み
		return TRUE;
	}
	
	
	// 未クリア
	return FALSE;
}



// ==========================================================================
// GsMainSysIsStageGoalAsSuperSonic
/*!
 *	引数に指定したステージがスーパーソニックにて
 *	クリア済みかどうかを返す関数
 *	
 *	※引数のステージIDはZONE1～4とZONE FINALのステージIDを指定してください。
 *	それ以外のステージIDだとアサートになります。
 *	
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
BOOL GsMainSysIsStageGoalAsSuperSonic(s32 stage_id)
{
	u32 tmp_stage_id = 0;
	
	MTM_ASSERT(stage_id >= 0 && stage_id <= GSD_MAIN_STAGE_ID_FINAL_5);
	MTM_ASSERT(stage_id != GSD_MAIN_STAGE_ID_ENDING);
	
	tmp_stage_id = (u32)stage_id;
	
	// もしステージIDがファイナル以上の場合、1-1に置き換え
	if (tmp_stage_id > GSD_MAIN_STAGE_ID_FINAL_5) {
		tmp_stage_id = 0;
	}
	
	// ステージデータインスタンス
	gs::backup::SStage &data
		= gs::backup::SStage::CreateInstance();
	
	// ファイナルステージID補正
	if (stage_id >= GSD_MAIN_STAGE_ID_FINAL_1) {
		tmp_stage_id = GSD_MAIN_STAGE_ID_FINAL_1;
	}
	else {
		tmp_stage_id = (u32)stage_id;
	}
	
	// スーパーソニック使用フラグチェック
	if (data[tmp_stage_id].IsUseSuperSonicOnce()) {
		// クリア済み
		return TRUE;
	}
	
	
	// 未クリア
	return FALSE;
}



// ==========================================================================
// GsMainSysIsStageScoreUploadOnce
/*!
 *	引数に指定したステージがハイスコアデータを一度でも
 *	アップロードしたかどうかを取得する関数
 *	
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
BOOL GsMainSysIsStageScoreUploadOnce(s32 stage_id)
{
	u32 tmp_stage_id = 0;
	
	MTM_ASSERT(stage_id >= 0 && stage_id < GSD_MAIN_STAGE_ID_MAX);
	MTM_ASSERT(stage_id != GSD_MAIN_STAGE_ID_ENDING);
	
	// スペステの場合
	if (stage_id >= GSD_MAIN_STAGE_ID_SS1) {
		tmp_stage_id = (u32)(stage_id - (GSD_MAIN_STAGE_ID_SS1 - gs::backup::ESpecialStage::Stage1));
		
		// スペシャルステージデータインスタンス作成
		gs::backup::SSpecial &spe_data
			= gs::backup::SSpecial::CreateInstance();
		
		// ハイスコアを一度でもアップロードしたかどうか
		if (spe_data[tmp_stage_id].IsScoreUploadedOnce()) {
			// クリア済み
			return TRUE;
		}
	}
	
	// 通常ステージの場合
	else {
		// ステージデータインスタンス
		gs::backup::SStage &data
			= gs::backup::SStage::CreateInstance();
		
		// ファイナルステージID補正
		if (stage_id >= GSD_MAIN_STAGE_ID_FINAL_1) {
			tmp_stage_id = GSD_MAIN_STAGE_ID_FINAL_1;
		}
		else {
			tmp_stage_id = (u32)stage_id;
		}
		
		// ハイスコアを一度でもアップロードしたかどうか
		if (data[tmp_stage_id].IsScoreUploadedOnce()) {
			// アップロード済み
			return TRUE;
		}
	}
	
	
	// 未クリア
	return FALSE;
}



// ==========================================================================
// GsMainSysIsStageTimeUploadOnce
/*!
 *	引数に指定したステージがタイムレコードデータを一度でも
 *	アップロードしたかどうかを取得する関数
 *	
 *	@return	TRUE : クリア済み
 *			FALSE: 未クリア
 */
// ==========================================================================
BOOL GsMainSysIsStageTimeUploadOnce(s32 stage_id)
{
	u32 tmp_stage_id = 0;
	
	MTM_ASSERT(stage_id >= 0 && stage_id < GSD_MAIN_STAGE_ID_MAX);
	MTM_ASSERT(stage_id != GSD_MAIN_STAGE_ID_ENDING);
	
	// スペステの場合
	if (stage_id >= GSD_MAIN_STAGE_ID_SS1) {
		tmp_stage_id = (u32)(stage_id - (GSD_MAIN_STAGE_ID_SS1 - gs::backup::ESpecialStage::Stage1));
		
		// スペシャルステージデータインスタンス作成
		gs::backup::SSpecial &spe_data
			= gs::backup::SSpecial::CreateInstance();
		
		// ハイスコアを一度でもアップロードしたかどうか
		if (spe_data[tmp_stage_id].IsTimeUploadedOnce()) {
			// クリア済み
			return TRUE;
		}
	}
	
	// 通常ステージの場合
	else {
		// ステージデータインスタンス
		gs::backup::SStage &data
			= gs::backup::SStage::CreateInstance();
		
		// ファイナルステージID補正
		if (stage_id >= GSD_MAIN_STAGE_ID_FINAL_1) {
			tmp_stage_id = GSD_MAIN_STAGE_ID_FINAL_1;
		}
		else {
			tmp_stage_id = (u32)stage_id;
		}
		
		// レコードタイムを一度でもアップロードしたかどうか
		if (data[tmp_stage_id].IsTimeUploadedOnce()) {
			// アップロード済み
			return TRUE;
		}
	}
	
	
	// 未クリア
	return FALSE;
}



// ==========================================================================
// GsMainSysIsSpecialStageClearedAct
/*!
 *	引数に指定したステージで既に一度スペステを
 *	クリアしたACTかどうかを返す関数
 *	
 *	@return	TRUE : 引数のステージではスペステをクリア済み
 *			FALSE: 引数のステージではスペステを未クリア
 */
// ==========================================================================
BOOL GsMainSysIsSpecialStageClearedAct(s32 stage_id)
{
	s32 tmp_stage_id = 0;
	
	// スペステ以外はアサート
	MTM_ASSERT(stage_id < GSD_MAIN_STAGE_ID_FINAL_1);
	
	// スペシャルステージデータインスタンス作成
	gs::backup::SSpecial &spe_data
		= gs::backup::SSpecial::CreateInstance();
	
	// 全てのスペステでクリアしたACTを取得・チェック
	for (int i = 0; i < 7; i++) {
		tmp_stage_id = gs_main_eme_get_act_no_tbl[spe_data[i].GetEmeraldStage()];
		
		// 引数のステージでi番目のスペステをクリアしていたらTRUE
		if (stage_id == tmp_stage_id) {
			return TRUE;
		}
	}
	
	return FALSE;
}



// ==========================================================================
// シェーダー
// ==========================================================================
// ==========================================================================
// GsMainSysLoadShader
/*!
 *	シェーダー初期化
 *
 *	@param	file_path	[in]	シェーダーファイルパス
 */
// ==========================================================================
void GsMainSysLoadShader(char *file_path)
{
#if defined GSD_PRECOMPILED_SHADER_PATH

	MTM_ASSERT(strlen(file_path) < 128);

	if (gs_shader_work.state == GSE_SHADER_LOAD_STATE_LOAD) {
		// ロード中
		MTM_ASSERT(!"GsMainSysLoadShader: now loading\n");
		return;
	}

	// 読み込み開始
	gs_shader_work.state = GSE_SHADER_LOAD_STATE_LOAD;

	// 待機処理
	gs_shader_work.tcb = MTM_TASK_MAKE_TCB(gsMainSysLoadShaderWait, NULL/*dest*/,
						0/*flag*/, 0xFFFF/*pause_level*/,
						0x1000/*prio*/, 0/*group*/,
						0/*work_size*/, "GS_SHADER_LOAD");

	// ファイル読み込み
	strcpy(gs_shader_work.file_path, file_path);
	amFsSetMallocMode(AMD_FS_MALLOC_TEMP, 1);
	gs_shader_work.fs_req = amFsReadBackground(gs_shader_work.file_path);
	amFsSetMallocMode(AMD_FS_MALLOC_NORMAL, 0);

#else
	UNREFERENCED_PARAMETER(file_path);
#endif
}

// ==========================================================================
// GsMainSysCheckLoadShaderFinished
/*!
 *	シェーダー初期化終了チェック
 *
 *	@return	TRUE : 終了
 */
// ==========================================================================
BOOL GsMainSysCheckLoadShaderFinished(void)
{
#if defined GSD_PRECOMPILED_SHADER_PATH
	if (gs_shader_work.state == GSE_SHADER_LOAD_STATE_COMPLETE) {
		return (TRUE);
	}
	return (FALSE);
#else
	return (TRUE);
#endif
}

// ==========================================================================
// gsMainSysLoadShaderWait
/*!
 *	シェーダーファイル読み込み待機
 *
 *	@param	tcb	[in]
 */
// ==========================================================================
#if defined GSD_PRECOMPILED_SHADER_PATH
void gsMainSysLoadShaderWait(MTS_TASK_TCB *tcb)
{
	switch (gs_shader_work.state) {
	default:
	case GSE_SHADER_LOAD_STATE_NOTHING:
		MTM_ASSERT(!"gsMainSysLoadShaderWait: error state\n");
		break;

	case GSE_SHADER_LOAD_STATE_LOAD:
		// ロード終了待機
		if (amFsIsComplete(gs_shader_work.fs_req)) {
			gs_shader_work.state = GSE_SHADER_LOAD_STATE_REG;

			gs_shader_work.buf = gs_shader_work.fs_req->buf;
			gs_shader_work.fs_req->buf = NULL;
			amFsClearRequest(gs_shader_work.fs_req);
#if _PC | _XBOX
			amBindConv((Uint8 *)gs_shader_work.buf);
#endif

			// 登録
			gs_shader_work.reg_id = amShaderLoadStd(gs_shader_work.buf);
		}
		break;

	case GSE_SHADER_LOAD_STATE_REG:
		// 登録中
		if (amDrawIsRegistComplete(gs_shader_work.reg_id)) {
			// ランタイムコンパイルしない
			amDrawSetShaderCompile(0);

			//amMemFree(gs_shader_work.buf);
			//gs_shader_work.buf = NULL;

			// 終了
			gs_shader_work.state = GSE_SHADER_LOAD_STATE_COMPLETE;

			gs_shader_work.tcb = NULL;
			mtTaskClearTcb(tcb);
		}
		break;
	}
}
#endif


#if 0
/****************************************************************************/
// イベントチェンジ
/****************************************************************************/
void GsDecideLogoNextEvt(void);
void GsDecideTitleNextEvt(s32 title_cnt);
// ==========================================================================
// ロゴ
// ==========================================================================
enum {
	GSD_LOGO_NEXT_EVT_TITLE		= 0
};
// ==========================================================================
// GsDecideLogoNextEvt
/*!
 *	イベントチェンジ ロゴ
 */
// ==========================================================================
void GsDecideLogoNextEvt(void)
{
	syDecideEvtCase(GSD_LOGO_NEXT_EVT_TITLE);
}

// ==========================================================================
// タイトル
// ==========================================================================
enum {
	GSD_TITLE_NEXT_EVT_LOGO		= 0
};
// ==========================================================================
// GsDecideTitleNextEvt
/*!
 *	イベントチェンジ タイトル
 *
 *	@param	title_cnt	[in]	イベントカウンタ
 */
// ==========================================================================
void GsDecideTitleNextEvt(s32 title_cnt)
{
	if (title_cnt & 0x01) {
		syDecideEvtCase(GSD_TITLE_NEXT_EVT_LOGO);
	}
	else {
		syDecideEvtCase(GSD_TITLE_NEXT_EVT_LOGO);
	}
}
#endif	// #if 0


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// デバック
// ==========================================================================
#if defined (MTD_DEBUG)


#if 0
// ==========================================================================
// _mtDebugStep
/*!
 *	デバッグステップ実行関数
 */
// ==========================================================================
BOOL _mtDebugStep(const MTS_PAD_KEY_STATUS *key)
{
	static u16 time = 0;
	if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_SLOW) {
		++time;
		if (time >= 8) {
			time = 0;
			return (FALSE);
		}
	}
	else {
		if (PAD_BUTTON_R & key->repeat || PAD_BUTTON_DEBUG & key->repeat) {
			return (FALSE);   // 実行
		}
	}

	return (TRUE);

}
#endif
#endif	// #if defined (MTD_DEBUG)
/* デバッグここまで */

// ==========================================================================
// GsMainSysStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void GsMainSysStaticVarInit(void)
{
	memset(&g_gs_main_sys_info, 0, sizeof(g_gs_main_sys_info));		/* メインシステム情報 */
	
	/// シェーダー用ワーク
	memset(&gs_shader_work, 0, sizeof(gs_shader_work));
}


// ==========================================================================
// _sy
/*! 
 *	
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
