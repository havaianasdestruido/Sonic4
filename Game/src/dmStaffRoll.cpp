// ===========================================================================
/*!
	@file	dmStaffRoll.cpp
	@brief	デモ・スタッフロール画面

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: dmStaffRoll.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
	スタッフロール作成メモ
	
	１、スタッフロールではAoActionを使用せず、通常プリミティブ描画に
		テクスチャを貼って表示する処理にする。
	
	２、Wii容量削減のため、データを軽くするために書き文字ではなく、
		プログラムで文字列データ(ASCII)を見て、テクスチャから文字を
  		切り出して文字列を表示するやり方にする。
  		(aoYsdFileモジュールを使用)
	
	３、スクリーンショットのデータは一枚ずつデータとして持ち、
  		初めに3枚分読み込んだあとは、一枚表示が終るごとに、
  		次の一枚を読み込むような裏読み方式にて行う。
	
	４、仕様書ではEND画面とスタッフロール画面とで２つあるように記述しているが、
  		処理としては全てこのモジュール内で行うようにする。
	
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"

#include "dmStaffRoll.h"
#include "../library/ao/include/aoTexture.h"
#include "../library/ao/include/aoAction.h"

#include "gs.h"
#include "gsMainSys.h"
#include "objObject.h"
#include "objObjectLoad.h"

#include "gmEventTbl.h"

#include "gmMain.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmCamera.h"
#include "gmObj.h"
#include "gmEnemy.h"
#include "gmPlySeq.h"
#include "gmMain.h"
#include "gmMapFar.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectBoss.h"
#include "gmBossCommon.h"
#include "gmBoss1.h"
#include "dmStaffRollMdlCtrl.h"

#include "gsSound.h"
#include "dmSound.h"

#include "akMath.h"
#include "izFade.h"
#include "aoWinSys.h"
#include "hgTrophy.h"
#include "dmSave.h"

#include "aoYsdFile.h"

// データヘッダ
#include "common/ace/D_STFRL_END.HMA"
#include "common/ace/D_STFRL_END_JP.HMA"

// データヘッダ
#include "common/model/SON_MDL.hmb"

// 共通データヘッダ
#if !_IPHONE
#include "common/ace/D_CMN_WIN.HMA"
#include "common/ace/D_CMN_MSG_JP.HMA"
#else //!_IPHONE
#include "ace/D_CMN_WIN.HMA"
#include "ace/D_CMN_MSG_JP.HMA"
#endif //!_IPHONE

// モデルデータヘッダ
#include "../file/common/arc/BOSS01.hmb"
#include "../file/common/model/BOSS01_MDL.hmb"
#include "../file/common/model/BOSS01_BODY_MTN.hmb"
#include "../file/common/model/BOSS01_EGG_MTN.hmb"

// ----- Macros ------------------------------------------------（マクロ定義）

#define DMD_STFRL_TASK_PAUSELEVEL			(0x7fff)
#define DMD_STFRL_TASK_PRIO_MAIN			(0x3000)
#define DMD_STFRL_TASK_GROUP_MAIN			(10)

#define DMD_STFRL_FILE_PATH_NUM_MAX			(60)

#define DMD_STFRL_SIZE_WIDTH				(960.0f)
#define DMD_STFRL_SIZE_HEIGHT				(720.0f)
#define DMD_STFRL_SIZE_HALF_WIDTH			(480.0f)
#define DMD_STFRL_SIZE_HALF_HEIGHT			(360.0f)


// プライオリティ設定
#define DMD_STFRL_DRAW_PRIO_CHAR			(0x0a00)//(0x5000)
#define DMD_STFRL_DRAW_PRIO_BG				(0x0800)//(0x4000)
#define DMD_STFRL_DRAW_PRIO_FIX				(0x0900)//(0x4800)
#define DMD_STFRL_DRAW_PRIO_WIN				(0x0c00)//(0x6000)

// 表示関連
#define DMD_STFRL_DISP_ONE_CHAR_SIZE		(32.f)
#define DMD_STFRL_DISP_ONE_CHAR_U_RATE		(0.0625f)
#define DMD_STFRL_DISP_ONE_CHAR_V_RATE		(0.125f)//(0.0625f)//(0.125f)
#define DMD_STFRL_CHAR_LINE_NUM				(16)

#define DMD_STFRL_DRAW_STATE_PRIO_BG		(40)
#define DMD_STFRL_DRAW_STATE_PRIO_FIX		(30)
#define DMD_STFRL_DRAW_STATE_PRIO_CHAR		(20)
#define DMD_STFRL_DRAW_STATE_PRIO_WIN		(50)

#define DMD_STFRL_FONT_CENTER_DIST_X		(112.f)
#define DMD_STFRL_SCR_DISP_POS_X			(160.f)	// 仮
#if !_IPHONE
#define DMD_STFRL_SCR_DISP_POS_Y			(180.f)//(216.f)
#else //!_IPHONE
#define DMD_STFRL_SCR_DISP_POS_Y			(216.f)
#endif //!_IPHONE
#define DMD_STFRL_SIZE_SCR_TEXTURE_X		(512.f)//(640.f)//(512.f)
#define DMD_STFRL_SIZE_SCR_TEXTURE_Y		(288.f)//(360.f)//(288.f)
#define DMD_STFRL_UV_U_RATE_TEXTURE			(1.0f)
#define DMD_STFRL_UV_V_RATE_TEXTURE			(0.5625f)

#define DMD_STFRL_DIMPS_LOGO_JP_TEX_ID		(2)
#define DMD_STFRL_SEGA_LOGO_JP_TEX_ID		(3)
#define DMD_STFRL_SEGA_LOGO_US_TEX_ID		(4)

#define DMD_STFRL_BOSS_BODY_NODE_IDX_EGG_CONNECT	(11)	//!< エッグマン接続ノード
#define DMD_STFRL_BOSS_BODY_NODE_IDX_BODY_POSTURE	(2)		//!< 本体姿勢

// ウインドウ関連
#define DMD_STFRL_WINDOW_TEX_ID				(1)
#define DMD_STFRL_WINDOW_SIZE_W				(380.f)
#define DMD_STFRL_WINDOW_SIZE_H				(180.f)
#define DMD_STFRL_WIN_DEF_RATE				(1.0f)
#define DMD_STFRL_WIN_EFCT_TIME				(8.0f)

// フェード関連
#define DMD_STFRL_FADEIN_TIME				(64.0f)
#define DMD_STFRL_FADEOUT_TIME				(64.0f)

#define DMD_STFRL_MODE_FADEIN_TIME			(80.0f)
#define DMD_STFRL_MODE_FADEOUT_TIME			(80.0f)

#define DMD_STFRL_MODE_SND_FADEOUT_TIME		(80)

// 演出関連
#define DMD_STFRL_FIRST_DISP_WAIT_TIME		(240.f)
#define DMD_STFRL_EFCT_FADE_IN_TIME			(32.f)
#define DMD_STFRL_EFCT_FADE_OUT_TIME		(32.f)
#define DMD_STFRL_EFCT_FADE_WAIT_TIME		(32.f)

#define DMD_STFRL_EFCT_QSTN_COLOR_INIT		(255)
#define DMD_STFRL_EFCT_QSTN_ALPHA_INIT		(0)
#define DMD_STFRL_EFCT_QSTN_ALPHA_MAX		(255)
#define DMD_STFRL_EFCT_QSTN_FADE_SPD		(8)

#define DMD_STFRL_DISP_LAST_LIST_PAGE		(17)

#define DMD_STFRL_LOAD_LOOP_TIME			(12.0f)
#define DMD_STFRL_LOADED_WAIT_TIME			(60.0f)

#define DMD_STFRL_SONIC_MOVE_INIT_SPD		(50.f)
#define DMD_STFRL_SONIC_MAIN_INIT_POS		(48.f)
#define DMD_STFRL_SONIC_OTHER_INIT_POS		(0.f)
#define DMD_STFRL_SONIC_MOVE_SPD			(12.f)
#define DMD_STFRL_SONIC_MOVE_ACCEL			(0.8f)

#define DMD_STFRL_BOSS_MOVE_DOWN_SPD			(0.4 * FX32_ONE)
#define DMD_STFRL_BOSS_MOVE_UP_SPD				(-0.4 * FX32_ONE)
#define DMD_STFRL_BOSS_NODISP_HEIGHT_POS_Y		(-180 * FX32_ONE)
#define DMD_STFRL_BOSS_DISP_HEIGHT_POS_Y		(-16 * FX32_ONE)
#define DMD_STFRL_BOSS_COMP_DISP_HEIGHT_POS_Y	(-40 * FX32_ONE)
#define DMD_STFRL_BOSS_EGG_LAUGH_TIME			(180)

#define DMD_STFRL_ONE_AROUND_DIR			(0x10000)
//#define DMD_STFRL_RING_EFCT_DISP_NUM		(8)
#define DMD_STFRL_RING_ROTATE_SPD			(0x200)
#define DMD_STFRL_RING_DISP_TIME			(10)
#define DMD_STFRL_RING_NODISP_TIME			(60)

#define DMD_STFRL_TEX_CONTINUE_DISP_TIME	(240)
#define DMD_STFRL_PLAY_SE_METAL_SONIC_EYE	(140)

#if _WII
#define DMD_STFRL_DISP_SCALE_TEXT			(1.4f)
#elif _IPHONE
#define DMD_STFRL_DISP_SCALE_TEXT			(1.5f * 1.125f)
#endif


// フラグ関連
#define DMD_STFRL_FLAG_EXIT					(1 << 0)		//!< 終了フラグ
#define DMD_STFRL_FLAG_CANCEL				(1 << 1)		//!< キャンセル
#define DMD_STFRL_FLAG_DECIDE				(1 << 2)		//!< 決定フラグ
#define DMD_STFRL_FLAG_BACK_LOAD_SCR_DATA	(1 << 3)		//!< データ裏読み
#define DMD_STFRL_FLAG_IMAGE_FADE_START		(1 << 4)		//!< イメージフェード開始
#define DMD_STFRL_FLAG_IMAGE_FADE_END		(1 << 5)		//!< イメージフェード終了
#define DMD_STFRL_FLAG_DISP_IN_WINDOW		(1 << 6)		//!< ウインドウ表示
#define DMD_STFRL_FLAG_COUNT_TIME			(1 << 7)		//!< 
#define DMD_STFRL_FLAG_POSSIBLE_INPUT		(1 << 8)		//!< 入力可能フラグ
#define DMD_STFRL_FLAG_WIN_EFCT_END			(1 << 9)		//!< 
#define DMD_STFRL_FLAG_WIN_ALL_DRAW			(1 << 10)		//!< 
#define DMD_STFRL_FLAG_MAKE_SND_TASK		(1 << 11)		//!< 
#define DMD_STFRL_FLAG_DISP_CONTINUE_TEX	(1 << 12)		//!<

#define DMD_STFRL_FLAG_ALL_PROC_STOP		(1 << 13)		//!<
#define DMD_STFRL_FLAG_HBM_BGM_STOP			(1 << 14)		//!<
#define DMD_STFRL_FLAG_HBM_BGM_REPLAY		(1 << 15)		//!<

#define DMD_STFRL_FLAG_SET_EVT_SIGN_OUT		(1 << 30)		//!< 
#define DMD_STFRL_FLAG_SIGN_OUT_EXIT		(1 << 31)		//!< 

#define DMD_STFRL_SONIC_FLAG_EFCT_END		(1 << 0)		//!< 

#define DMD_STFRL_BODY_FLAG_COMPLETE_EFCT		(1 << 0)		//!< 
#define DMD_STFRL_BODY_FLAG_MOVE_DOWN_START		(1 << 1)		//!< 
#define DMD_STFRL_BODY_FLAG_MOVE_UP_START		(1 << 2)		//!< 
#define DMD_STFRL_BODY_FLAG_COMP_EFCT_END		(1 << 3)		//!< 
#define DMD_STFRL_BODY_FLAG_M_SONIC_EFCT_START	(1 << 4)		

#define DMS_STFRL_FLAG_CHNG_MTN_EGG_LAUGH_REQ	(1 << 21)		//!< エッグマン笑いモーション切り替えフラグ
#define DMS_STFRL_EGG_FLAG_CHNG_MTN_EGG_LAUGH	(1 << 0)		//!< エッグマン笑いモーション切り替えフラグ

#define DMD_STFRL_RING_FLAG_SPLASH_EFCT_START	(1 << 0)


// ----- Macro Functions -----------------------------------（処理マクロ定義）


// ----- Definitions -------------------------------------------（定数の宣言）
//!< ファイル種別
typedef enum tag_DME_STFRL_DATA_TYPE
{
	DME_STFRL_DATA_TYPE_LIST1_DATA = 0,		//!< スタッフリストデータ1
	DME_STFRL_DATA_TYPE_SCR_DATA,			//!< スクリーンデータ
	DME_STFRL_DATA_TYPE_END_JP_DATA,		//!< END画面データ
	
	DME_STFRL_DATA_TYPE_NUM,
	DME_STFRL_DATA_TYPE_NONE
} DME_STFRL_DATA_TYPE;


//!< フォント表示タイプ
typedef enum tag_DME_STFRL_FONT_DISP_TYPE
{
	DME_STFRL_FONT_DISP_TYPE_POST = 0,		// 役職
	DME_STFRL_FONT_DISP_TYPE_NAME,			// 名前
	DME_STFRL_FONT_DISP_TYPE_SPACE,			// 空行
	DME_STFRL_FONT_DISP_TYPE_WHITE_POST,	// 役職(白)
	DME_STFRL_FONT_DISP_TYPE_SONIC_TEAM,	// ソニックチームロゴ
	DME_STFRL_FONT_DISP_TYPE_DIMPS,			// DIMPSロゴ
	DME_STFRL_FONT_DISP_TYPE_SEGA,			// SEGAロゴ
	DME_STFRL_FONT_DISP_TYPE_SEGA_SPACE,	// SEGA冠文字
	
	DME_STFRL_FONT_DISP_TYPE_NUM,
	DME_STFRL_FONT_DISP_TYPE_NONE,
} DME_STFRL_FONT_DISP_TYPE;


//!< イベント遷移先
typedef enum tag_DME_STFRL_NEXT_EVT
{
	DME_STFRL_NEXT_EVT_MAINMENU = 0,	// メインメニュー
	DME_STFRL_NEXT_EVT_TITLE,			// タイトル
	
	DME_STFRL_NEXT_EVT_NUM,
	DME_STFRL_NEXT_EVT_NONE,
} DME_STFRL_NEXT_EVT;


//!< モード別
typedef enum tag_DME_STFRL_DISP_MODE
{
	DME_STFRL_DISP_MODE_MAIN = 0,	// スタッフロールメイン
	DME_STFRL_DISP_MODE_END,		// END画面
	DME_STFRL_DISP_MODE_WIN_MSG,	// 促しメッセージ画面
	
	DME_STFRL_DISP_MODE_NUM,
	DME_STFRL_DISP_MODE_NONE,
} DME_STFRL_DISP_MODE;


//! ウインドウ表示パターンタイプ
typedef enum tag_DME_STFRL_WIN
{
	DME_STFRL_WIN_GET_EMERALD = 0,	//!< 
//	DME_STFRL_WIN_NOW_SAVE,			//!< 
	
	DME_STFRL_WIN_NUM,
	DME_STFRL_WIN_NONE
} DME_STFRL_WIN;


//!< ファイル種別
/*
typedef enum tag_DME_STFRL_DATA_TYPE
{
	DME_STFRL_DATA_TYPE_LIST1_DATA = 0,		//!< スタッフリストデータ1
	DME_STFRL_DATA_TYPE_SCR_DATA,			//!< スクリーンデータ
	DME_STFRL_DATA_TYPE_END_JP_DATA,		//!< END画面データ
	
	DME_STFRL_DATA_TYPE_NUM,
	DME_STFRL_DATA_TYPE_NONE
} DME_STFRL_DATA_TYPE;
*/

//! アクションテーブル(スタッフリストを含まない)
typedef enum tag_DME_STFRL_ACT
{
	ACT_LIGHT_BG_LT = 0,
	ACT_LIGHT_BG_LB,
	ACT_LIGHT_BG_RT,
	ACT_LIGHT_BG_RB,
	ACT_METAL_SONIC,
	ACT_M_SONIC_EYE,
	ACT_BLACK_BG,
	ACT_WHITE_BG,
	
	// 言語別
	ACT_TEX_TRYAGAIN,
	ACT_TEX_CONTINUED,
	ACT_TEX_WIN_MSG,
	
	// 以下、メニュー共通
#if !_IPHONE
	ACT_WIN_LINE,			//!< 
	
	ACT_TEX_WINTITLE,		//!< 
	ACT_TEX_OK,				//!< 
#endif //!_IPHONE
	
	ACT_NUM,
	
	ACT_NONE
} DME_STFRL_ACT;


typedef struct tag_DMS_STFRL_MAIN_WORK	DMS_STFRL_MAIN_WORK;

//! メインタスクワーク
struct tag_DMS_STFRL_MAIN_WORK {

	DMS_STFRL_DATA_MGR arc_data;
	
	// こちらが本使用のFS保存変数
	void *arc_list_font_amb;	// スタッフリスト用フォントデータAMB
	void *arc_scr_amb_fs;			// 一度に保存しておく分のスクリーンアクションAMB
	void *arc_end_amb_fs;		// フルバージョン時の共通アクション(ENDテキストなど)
	void *arc_end_jp_amb_fs;		// フルバージョン時の共通アクション(ENDテキストなど)
	
	void *arc_cmn_amb_fs[2];		// メニュー共通データ
	
	AMS_FS *arc_list_font_amb_fs;			// スタッフ名簿リストYSDファイル
	
	AMS_FS *staff_list_fs;			// スタッフ名簿リストYSDファイル
	
	// フォント・スクリーンのテクスチャ
	AOS_TEXTURE		font_tex;
	AOS_TEXTURE		scr_tex[3];
	
	// END画面用アクションデータ
	void			*end_ama;
	void			*end_amb;
	AOS_TEXTURE		end_tex;
	void			*end_jp_ama;
	void			*end_jp_amb;
	AOS_TEXTURE		end_jp_tex;
	
	// メニュー共通としてウインドウとメッセージを使用
	void			*cmn_ama[2];						//!< AMAファイル
	void			*cmn_amb[2];						//!< AMBファイル
	AOS_TEXTURE		cmn_tex[2];							//!< メニュー共通テクスチャ
	
	void			*stf_list_ysd;
	
	// メニュー用アクション
	AOS_ACTION 		*act[ACT_NUM];
	
	void (*proc_input)(DMS_STFRL_MAIN_WORK *);		//!< 入力用プロシージャ
	void (*proc_update)(DMS_STFRL_MAIN_WORK *);		//!< メニュー用プロシージャ
	void (*proc_data_load)(DMS_STFRL_MAIN_WORK *);	//!< データ裏読みプロシージャ
	void (*proc_draw)(DMS_STFRL_MAIN_WORK *);		//!< 描画用プロシージャ
	
	float timer;									//!< 汎用タイマー
	u32	flag;										//!< 汎用フラグ
	float efct_timer;
	float fade_timer;
	float win_timer;
	float disp_frm_time;							//!< タイマー更新速度
	u32	disp_mode;									//!< 表示モード(メインかEND画面か)
	BOOL is_eme_comp;								//!< カオスエメラルドコンプリートしてるかどうか
	
	float win_size_rate[2];							//!< ウインドウサイズ倍率
	u32 win_mode;									//!< 
	u32 announce_flag;								//!< ウインドウアナウンスフラグ
	
	float sonic_set_frame;							//!< ソニックの表示フレーム
	float list_disp_pos_x;							//!< ソニックの表示位置X
	AOS_ACT_COL list_col;
	float sonic_move_spd;							//!< ソニックの移動速度
	
	int end_act_frm;
	int continue_act_frm;
	
	s32 load_data_num;
	BOOL is_full_staffroll;							//!< メインゲームのロード時
	u32 draw_state;
	
	u32 cur_disp_scr_id;
	BOOL data_disp_yet[3];							//!< スクリーンの表示済みチェック
	BOOL check_file_load[3];							//!< スクリーンの表示済みチェック
	
	u32 disp_list_page_num;
	u32 cur_disp_list_page;							//!< 
	u32 prev_disp_list_page;						//!< 
	
	u32 disp_page_time;
	u32 cur_disp_image;
	u32 *page_line_type;
	
	u32 cur_page_list_alpha_data;					//!< 現在表示中のページのリスト表示部透過度
	u32 cur_page_scr_alpha_data;					//!< 現在表示中のページのリスト表示部透過度
	
	AOS_ACT_COL question_act_alpha;						//!< 
	
	DMS_STFRL_SONIC_WORK *sonic_work;
	
	DMS_STFRL_BOSS_BODY_WORK *body_work;				//!< ボス本体ワーク
	DMS_STFRL_BOSS_EGG_WORK *egg_work;				//!< ボスエッグマンワーク
	
	DMS_STFRL_RING_WORK *ring_work[3];
	
	// メインゲーム中用のサウンドSCBファイルポインタ
	GSS_SND_SCB *bgm_scb;
	
	GSS_SND_SE_HANDLE *se_handle;
};


//! 管理構造体
typedef struct tag_DMS_STFRL_MGR {
	MTS_TASK_TCB *tcb;	//!< TCB
} DMS_STFRL_MGR;


typedef struct tag_DMS_STFRL_CAMERA_DATA {
	// 共通パラメータ
	NNS_MATRIX					ViewMtx;		//!< ビューマトリクス
	NNS_CAMERA_TARGET_UPVECTOR	Camera;			//!< カメラ
	NNS_MATRIX					projmtx;		//!< プロジェクションマトリクス
	NNS_VECTOR					lookDir;	    //!< 視線ベクトル
	
	// カメラ
	float						fovy;			//!< 視野角
	float						cam_scale;		//!< カメラの表示領域を決めるスケール
} DMS_STFRL_CAMERA_DATA;


typedef struct tag_DMS_STFRL_BUILD_DATA {
	
	AMS_FS*				ambFs;						//!< AMBファイル
	AMS_AMB_HEADER*     amb_header;					//!< AMBファイル先頭
	NNS_TEXFILELIST*    znt_buf;					//!< ZNTファイル先頭
	void*               texlistbuf;					//!< テクスチャリストバッファ
	NNS_TEXLIST*        texlist;					//!< テクスチャリスト
	s32					texId;						//!< テクスチャＩＤ
	s32					regId;						//!< 登録ＩＤ（開放完了チェックに必要）
	s32					drawFlag;					//!< 描画フラグ
	
	
	AMS_FS*				znoFs;						//!< ZNOファイル
	char*               zno_buf;					//!< ZNOファイル先頭
	NNS_OBJECT*         pObj;						//!< オブジェクト管理用
	NNS_RGBA			Diffuse;					//!< 
	NNS_RGB				Ambient;					//!< 
	float				Specular;					//!<
	NNF_DRAWOBJ         drawObjFlag;				//!< オブジェクト描画フラグ
	
	void				*model;
	
	float				tex_u;						//!<
	float				tex_v;						//!<
	float				vel_u;						//!< Uスクロール値
	float				vel_v;						//!< Vスクロール値

	
	AMS_FS*				mtnFs[1];					//!< モーションファイル
	char*				mtn_buf;					//!< モーションファイル先頭
	AMS_MOTION*			motion;						//!< モーション管理用
	float				frame;						//!< フレーム数
	float				mtnSpeed;					//!< モーション速度
	float               startFrm;					//!< モーション開始フレーム
	float               endFrm;						//!< モーション終了フレーム
	float				lerp_frame;					//!< 直列補間検証用
	s32					lerpFlag;					//!< 補間するかどうか
	s32					registFlag; 				//!< モーションを登録したかどうか？
	float				per;						//!< 直列補間率（モーションの繋ぎ）
	float				merge;						//!< 並列補間率（他モーションとの補間）
	void				*mtn;
	
} DMS_STFRL_BUILD_DATA;



// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
static void dmStaffRollInit(void);
static void dmStaffRollProcMain(MTS_TASK_TCB *tcb);
static void dmStaffRollDest(MTS_TASK_TCB *tcb);

// 初期化設定関連
static void dmStaffRollProcInit(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcCreateAct(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollSetNextEvt(DMS_STFRL_MAIN_WORK *main_work);

// メニュー用プロシージャ
static void dmStaffRollProcStopDraw(DMS_STFRL_MAIN_WORK *main_work);

static void dmStaffRollProcFadeIn(DMS_STFRL_MAIN_WORK *main_work);

static void dmStaffRollProcDispWaitStaffList(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcSetChangeData(DMS_STFRL_MAIN_WORK *main_work);

static void dmStaffRollProcLoadData(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcDataBuild(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcNowStaffRoll(DMS_STFRL_MAIN_WORK *main_work);

static void dmStaffRollProcModeFadeOut(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcModeFadeIn(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcEndModeIdle(DMS_STFRL_MAIN_WORK *main_work);

static void dmStaffRollProcWinModeFadeOut(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcWinModeFadeIn(DMS_STFRL_MAIN_WORK *main_work);

static void dmStaffRollProcWindowNodispIdle(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcWindowOpenEfct(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcWindowAnnounceIdle(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcWindowCloseEfct(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcTrophyCheck(DMS_STFRL_MAIN_WORK *main_work);

static void dmStaffRollProcFadeOut(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcDataRelease(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollProcFinish(DMS_STFRL_MAIN_WORK *main_work);

static void dmStaffRollProcSaveEndCheck(DMS_STFRL_MAIN_WORK *main_work);

// 入力処理関連
static void dmStaffRollInputProcStaffRollMain(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollInputProcWin(DMS_STFRL_MAIN_WORK *main_work);

// 描画関連処理
static void dmStaffRollProcActDraw(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollBGDraw(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollStaffListDraw(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollStaffListOneLineDraw(DMS_STFRL_MAIN_WORK *main_work, u32 disp_pos_y, u32 cur_line);
static void dmStaffRollStageScrDraw(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollTaskDraw(AMS_TCB* tcb);
static void dmStaffRollTaskBgDraw(AMS_TCB* tcb);
static void dmStaffRollStageScrTaskDraw(AMS_TCB* tcb);

static void dmStaffRollEndActDraw(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollEndActTaskDraw(AMS_TCB* tcb);
static void dmStaffRollWinActDraw(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollWinActTaskDraw(AMS_TCB* tcb);

static void dmStaffRollSetWinOpenEfct(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollSetWinCloseEfct(DMS_STFRL_MAIN_WORK *main_work);

static void dmStaffRollSetObjSystemData(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollInitLight(void);

static void dmStaffRollSetupEndModel(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollNodispEndModel(DMS_STFRL_MAIN_WORK *main_work);

static void dmStaffRollCameraInit(void);
static void dmStaffRollCameraFunc(OBS_CAMERA *obj_camera);

static void dmStaffRollSetBossObj(DMS_STFRL_MAIN_WORK *main_work);

// 演出関連設定処理
static void dmStaffRollSetInitData(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollSetEfctChngAlphaListData(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollSetFadePageInfoEfctData(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollDataClearRequestFull(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollDataClearRequestEasy(DMS_STFRL_MAIN_WORK *main_work);
static void dmStaffRollDataBuildEasy(DMS_STFRL_MAIN_WORK *main_work);

#if _WII
static void dmStaffRollIsHBMDraw(DMS_STFRL_MAIN_WORK *main_work);
#endif

static s32 dmStaffRollIsDataLoad(DMS_STFRL_MAIN_WORK *main_work);
static s32 dmStaffRollIsTexLoad(void);
static s32 dmStaffRollIsTexRelease(void);

// ----- Global Variables ----------------------（グローバル変数の定義：外部）

// ----- Static Variables --------------------（スタティック変数の定義：局所）
// 各国別AMBファイルパスID
const static s32 dm_stfrl_lng_amb_id_tbl[GSD_LANGUAGE_NUM] = {
	GMD_DWORK_NO_GMK_STFRL_END_TEX_JP,
	GMD_DWORK_NO_GMK_STFRL_END_TEX_US,
	GMD_DWORK_NO_GMK_STFRL_END_TEX_FR,
	GMD_DWORK_NO_GMK_STFRL_END_TEX_IT,
	GMD_DWORK_NO_GMK_STFRL_END_TEX_GE,
	GMD_DWORK_NO_GMK_STFRL_END_TEX_SP,
};

// 各国別共通メッセージAMBファイルパスID
const static s32 dm_stfrl_cmn_msg_lng_amb_id_tbl[GSD_LANGUAGE_NUM] = {
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_JP,
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_US,
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_FR,
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_IT,
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_GE,
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_SP,
};



// 描画する文字種類分のASCII変更IDテーブル
const static char dm_stfrl_list_font_id_tbl[128] = {
	
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 	// 0 ～ 15
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 	// 16 ～ 31
	79, 67, -1, -1, -1, -1, 69, 77, 73, 74, -1, -1, 75, 62, 65, 66, 	// 32 ～ 47
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 63, 64, 71, -1, 72, 68, 	// 48 ～ 63
	70,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 	// 64 ～ 79
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, 	// 80 ～ 95
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 	// 96 ～ 111
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, 78, 76, 	// 112 ～ 127
	
};


// 描画する文字種類分の各文字幅テーブル
const static char dm_stfrl_font_width_length_tbl[128] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 	//   0 ～  15
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 	//  16 ～  31
	16, 11, -1, -1, -1, -1, 19, 10, 11, 11, -1, -1, 10, 13, 10, 12, 	//  32 ～  47
	19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 12, 12, 19, -1, 19, 19, 	//  48 ～  63
	23, 18, 19, 20, 21, 18, 18, 21, 20,  8, 12, 18, 17, 24, 20, 21, 	//  64 ～  79
	19, 21, 19, 18, 16, 20, 18, 24, 17, 16, 18, -1, -1, -1, -1, -1, 	//  80 ～  95
	-1, 16, 16, 15, 17, 15,  8, 17, 17,  7,  8, 14,  8, 22, 16, 17, 	//  96 ～ 111
	17, 17,  9, 14,  9, 17, 14, 20, 14, 14, 14, -1, -1, -1, 16, 26, 	// 112 ～ 127
	
};

#if _IPHONE
static const float dm_stfrl_list_id_font_size_scale = 1.4f; //<フォントサイズスケール
#endif //_IPHONE

// リストの種別IDごとの基本スケールサイズテーブル
const static float dm_stfrl_list_id_font_size_tbl[DME_STFRL_FONT_DISP_TYPE_NUM] = {
#if !_IPHONE
	0.8f,	// 役職
	1.0f,	// 名前
	0.25f,	// 空行
	0.8f,	// 白色役職
	1.0f,	// SONIC TEAMロゴ
	1.0f,	// DIMPSロゴ
	1.0f,	// SEGAロゴ
	1.2f,	// SEGA冠文字
#else //!_IPHONE
	0.8f * dm_stfrl_list_id_font_size_scale,	// 役職
	1.0f * dm_stfrl_list_id_font_size_scale,	// 名前
	0.25f* dm_stfrl_list_id_font_size_scale,	// 空行
	0.8f * dm_stfrl_list_id_font_size_scale,	// 白色役職
	1.0f * dm_stfrl_list_id_font_size_scale,	// SONIC TEAMロゴ
	1.0f * dm_stfrl_list_id_font_size_scale,	// DIMPSロゴ
	1.0f * dm_stfrl_list_id_font_size_scale,	// SEGAロゴ
	1.2f * dm_stfrl_list_id_font_size_scale,	// SEGA冠文字
#endif //!_IPHONE
};


#if _IPHONE
static const float dm_stfrl_list_id_font_height_scale = dm_stfrl_list_id_font_size_scale * 0.9f; //<フォント高スケール
#endif //_IPHONE

// リストの種別IDごとの1行分の高さ(行間の空白部分も含む)
const static u32 dm_stfrl_list_id_font_height_tbl[DME_STFRL_FONT_DISP_TYPE_NUM] = {
#if !_IPHONE
	24,		// 役職
	32,		// 名前
	8,		// 空行
	24,		// 白色役職
	64,		// SONIC TEAMロゴ
	64,		// DIMPSロゴ
	128,	// SEGAロゴ
	32,		// SEGA冠文字		※※※要調整
#else //!_IPHONE
	24 * dm_stfrl_list_id_font_height_scale * 1.2f,	// 役職
	32 * dm_stfrl_list_id_font_height_scale,	// 名前
	8  * dm_stfrl_list_id_font_height_scale,	// 空行
	24 * dm_stfrl_list_id_font_height_scale,	// 白色役職
	64,											// SONIC TEAMロゴ
	64,											// DIMPSロゴ
	128,										// SEGAロゴ
	32,											// SEGA冠文字		※※※要調整
#endif //!_IPHONE
};


// リストの種別IDごとの1行分の高さ(行間の空白部分も含む)
const static u32 dm_stfrl_list_id_font_color_tbl[DME_STFRL_FONT_DISP_TYPE_NUM][3] = {
	{255, 255,   0},	// 役職
	{255, 255, 255},	// 名前
	{255, 255, 255},	// 空行
	{255, 255, 255},	// 役職(白)
	{255, 255, 255},	// SONIC TEAMロゴ
	{255, 255, 255},	// Dimpsロゴ
	{255, 255, 255},	// SEGAロゴ
	{255, 255, 255},	// SEGA冠文字
};



// リストの種別IDごとの1行分の高さ(行間の空白部分も含む)
const static u32 dm_stfrl_list_logo_width_tbl[DME_STFRL_FONT_DISP_TYPE_NUM] = {
	0,		// 役職
	0,		// 名前
	0,		// 空行
	0,		// 白色役職
	256,	// SONIC TEAMロゴ
	256,	// DIMPSロゴ
	256,	// SEGAロゴ
	0,		// SEGA冠文字
};



const static float dm_stfrl_win_act_pos_tbl[8][2] = {
//	{0.0f, 0.0f},		// ウインドウ背景
	{DMD_STFRL_SIZE_HALF_WIDTH + 42.f, 280.0f},		// ウインドウ内のライン
	{DMD_STFRL_SIZE_HALF_WIDTH - 80.f, 274.0f},		// タイトルテキスト
	{DMD_STFRL_SIZE_HALF_WIDTH, 420.0f},			// OK
	{DMD_STFRL_SIZE_HALF_WIDTH, 340.0f},			// メッセージ
//	{DMD_STFRL_SIZE_HALF_WIDTH, 360.0f},			// メッセージ
//	{DMD_RANK_SIZE_HALF_WIDTH + 112.f, 264.0f},		// キャンセルボタン
};


// アクションIDテーブル(初期状態)
const static u32 g_dm_act_id_tbl[ACT_NUM] = {
	// 言語共通
	IDA_D_STFRL_END_ACT_LIGHT_LT,
	IDA_D_STFRL_END_ACT_LIGHT_LB,
	IDA_D_STFRL_END_ACT_LIGHT_RT,
	IDA_D_STFRL_END_ACT_LIGHT_RB,
	IDA_D_STFRL_END_ACT_METAL,
	IDA_D_STFRL_END_ACT_EYE01,
	IDA_D_STFRL_END_ACT_BG_BLACK2,
	IDA_D_STFRL_END_ACT_BG_WHITE,
	
	// 言語別
	IDA_D_STFRL_END_JP_ACT_TEX_TRYAGAIN,
	IDA_D_STFRL_END_JP_ACT_TEX_CONTI,
	IDA_D_STFRL_END_JP_ACT_TEX_WIN_MSG,

	
#if !_IPHONE
	IDA_D_CMN_WIN_ACT_WIN_LINE,				//!< 
	
	IDA_D_CMN_MSG_JP_ACT_TEX_WINTITLE,		//!< 
	IDA_D_CMN_MSG_JP_ACT_TEX_OK,			//!< 
#endif //!_IPHONE
};


//管理情報
static DMS_STFRL_MGR dm_stfrl_mgr;
static DMS_STFRL_MGR *dm_stfrl_mgr_p = NULL;

static DMS_STFRL_FS_DATA_MGR dm_stfrl_fs_data_mgr;
static DMS_STFRL_FS_DATA_MGR *dm_stfrl_fs_data_mgr_p = NULL;

static DMS_STFRL_DATA_MGR dm_stfrl_data_mgr;
static DMS_STFRL_DATA_MGR *dm_stfrl_data_mgr_p = NULL;

static void *dm_stfrl_font_amb;
static AOS_TEXTURE dm_stfrl_font_tex;

static void *dm_stfrl_scr_amb;
static AOS_TEXTURE dm_stfrl_scr_tex;

static void *dm_stfrl_end_cmn_ama;
static void *dm_stfrl_end_cmn_amb;
static AOS_TEXTURE dm_stfrl_end_tex;

static void *dm_stfrl_end_lng_ama;
static void *dm_stfrl_end_lng_amb;
static AOS_TEXTURE dm_stfrl_end_jp_tex;

static void *dm_stfrl_cmn_ama[2];
static void *dm_stfrl_cmn_amb[2];
static AOS_TEXTURE dm_stfrl_cmn_tex[2];

static BOOL dm_stfrl_is_full_staffroll = FALSE;
static BOOL dm_stfrl_is_pause_maingame = FALSE;



#if defined(MTD_DEBUG)
BOOL dm_stfrl_is_full_disp_efct = FALSE;
#endif

// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ==========================================================================
// DmStaffRollBuildForGame
/*!
 *	スタッフロールデータ構築(メインゲームから呼び出す専用)
 */
// ==========================================================================
#if 1
void DmStaffRollBuildForGame(void)
{
	s32 lang_id = 0;
	
	// 管理情報初期化
	amZeroMemory(&dm_stfrl_fs_data_mgr, sizeof(DMS_STFRL_MGR));
	dm_stfrl_fs_data_mgr_p = &dm_stfrl_fs_data_mgr;
	
	lang_id = (s32)GsEnvGetLanguage();
	
	// フォントデータ
	dm_stfrl_fs_data_mgr_p->arc_list_font_amb_fs
		= GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STFRL_FONT_TEX);
	
	// スクリーンショット
	dm_stfrl_fs_data_mgr_p->arc_scr_amb_fs
		= GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STFRL_SCR_IMG_TEX);
	
	// END画面用テクスチャ
	dm_stfrl_fs_data_mgr_p->arc_end_amb_fs
		= GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STFRL_END_TEX);
	dm_stfrl_fs_data_mgr_p->arc_end_jp_amb_fs
		= GmGameDatGetGimmickData(dm_stfrl_lng_amb_id_tbl[lang_id]);
	
	// メニュー共通データ
	dm_stfrl_fs_data_mgr_p->arc_cmn_amb_fs[0]
		= GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_STFRL_CMN_WIN_TEX);
	dm_stfrl_fs_data_mgr_p->arc_cmn_amb_fs[1]
		= GmGameDatGetGimmickData(dm_stfrl_cmn_msg_lng_amb_id_tbl[lang_id]);
	
	// スタッフ名簿リストデータ
//	dm_stfrl_fs_data_mgr_p->staff_list_fs
//		= amFsReadBackground(GSS_BASE_PATH "DEMO/STFRL/STAFF_LIST.YSD");
	
}
#endif


// ==========================================================================
// DmStaffRollIsBuildForGame
/*!
 *	スタッフロールデータ構築(メインゲームから呼び出す専用)
 */
// ==========================================================================
#if 0
BOOL DmStaffRollIsBuildForGame(void)
{
	if (!dmStaffRollIsDataLoadForGame()) {
		return FALSE;
	}
	
}
#endif


// ==========================================================================
// DmStaffRollBuild
/*!
 *	スタッフロールデータ構築
  	(ファイルの読込みは呼び出し側で行い、引数でポインタを渡してこちらでデータ構築)
 */
// ==========================================================================
void DmStaffRollBuild(DMS_STFRL_DATA_MGR *data_mgr)
{
	s16 tmp_cur_evt = 0;
	
	tmp_cur_evt = SyGetEvtInfo()->cur_evt_id;
	
	amZeroMemory(&dm_stfrl_data_mgr, sizeof(DMS_STFRL_DATA_MGR));
	dm_stfrl_data_mgr_p = &dm_stfrl_data_mgr;

	// データ構造体取得
	dm_stfrl_data_mgr_p = data_mgr;
	
	// テクスチャAMBのみのデータを取得
	// フォントAMBファイルロード
	dm_stfrl_font_amb = data_mgr->arc_font_amb;
	
	// アドレス変換
	amConvertAddress(dm_stfrl_font_amb);
	
	// テクスチャ構築
	AoTexBuild(&dm_stfrl_font_tex, dm_stfrl_font_amb);
	AoTexLoad(&dm_stfrl_font_tex);
	
	
	// スクリーンキャプチャAMBファイルロード
	dm_stfrl_scr_amb = data_mgr->arc_scr_amb;
	
	// アドレス変換
	amConvertAddress(dm_stfrl_scr_amb);
	
	// テクスチャ構築
	AoTexBuild(&dm_stfrl_scr_tex, dm_stfrl_scr_amb);
	AoTexLoad(&dm_stfrl_scr_tex);
	
	
	// 以下はデータがアーカイブ化しているものをファイル別に取得
	
	// END画面用AMBファイルロード
	amBindConv((u8 *)data_mgr->arc_end_amb);
	
	dm_stfrl_end_cmn_ama = amBindGet((AMS_AMB_HEADER*)data_mgr->arc_end_amb
									  , 0
									  );
	
	dm_stfrl_end_cmn_amb = amBindGet((AMS_AMB_HEADER*)data_mgr->arc_end_amb
									  , 1
									  );
	
	// END画面用AMBファイルロード
	amBindConv((u8 *)data_mgr->arc_end_jp_amb);
	
	dm_stfrl_end_lng_ama = amBindGet((AMS_AMB_HEADER*)data_mgr->arc_end_jp_amb
									  , 0
									  );
	
	dm_stfrl_end_lng_amb = amBindGet((AMS_AMB_HEADER*)data_mgr->arc_end_jp_amb
									  , 1
									  );
	
	// アドレス変換
	amConvertAddress(dm_stfrl_end_cmn_ama);
	amConvertAddress(dm_stfrl_end_cmn_amb);
	amConvertAddress(dm_stfrl_end_lng_ama);
	amConvertAddress(dm_stfrl_end_lng_amb);
	
	// テクスチャ構築
	AoTexBuild(&dm_stfrl_end_tex, dm_stfrl_end_cmn_amb);
	AoTexLoad(&dm_stfrl_end_tex);
	AoTexBuild(&dm_stfrl_end_jp_tex, dm_stfrl_end_lng_amb);
	AoTexLoad(&dm_stfrl_end_jp_tex);
	
	// メニュー共通AMBファイルロード
	for (int i = 0; i < 2; i++) {
		amBindConv((u8 *)data_mgr->arc_cmn_amb[i]);
		
		dm_stfrl_cmn_ama[i] = amBindGet((AMS_AMB_HEADER*)data_mgr->arc_cmn_amb[i]
										, 0
										);
		
		dm_stfrl_cmn_amb[i] = amBindGet((AMS_AMB_HEADER*)data_mgr->arc_cmn_amb[i]
										, 1
										);
		
		// アドレス変換
		amConvertAddress(dm_stfrl_cmn_ama[i]);
		amConvertAddress(dm_stfrl_cmn_amb[i]);
		
		// テクスチャ構築
		AoTexBuild(&dm_stfrl_cmn_tex[i], dm_stfrl_cmn_amb[i]);
		AoTexLoad(&dm_stfrl_cmn_tex[i]);
		
	}
}



// ==========================================================================
// DmStaffRollBuildCheck
/*!
 *	スタッフロールデータ構築 終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmStaffRollBuildCheck(void)
{
	// テクスチャ構築チェック
	if (!dmStaffRollIsTexLoad()) {
		// フラグ扱いでON
		return (FALSE);
	}
	
	return (TRUE);
}


// ==========================================================================
// DmStaffRollFlush
/*!
 *	スタッフロールデータフラッシュ
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
void DmStaffRollFlush(void)
{
	// テクスチャ解放
	AoTexRelease(&dm_stfrl_font_tex);
	
	if (dm_stfrl_is_full_staffroll) {
		AoTexRelease(&dm_stfrl_scr_tex);
		
		AoTexRelease(&dm_stfrl_end_tex);
		AoTexRelease(&dm_stfrl_end_jp_tex);
		
		for (int i = 0; i < 2; i++) {
			AoTexRelease(&dm_stfrl_cmn_tex[i]);
		}
		
		// 3Dモデルの解放処理を以下に記述
		
	}
	
}


// ==========================================================================
// DmStaffRollFlushCheck
/*!
 *	スタッフロールデータフラッシュ終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
BOOL DmStaffRollFlushCheck(void)
{
	// テクスチャ解放
	if (dmStaffRollIsTexRelease()) {
		
		return (TRUE);
	}
	
	return (FALSE);
}


// ==========================================================================
// DmStaffRollStart
/*!
	スタッフロール画面開始処理
 */
// ==========================================================================
void DmStaffRollStart(void *arg)
{
	UNREFERENCED_PARAMETER(arg);
	
	s16 tmp_prev_evt = 0;
	s16 tmp_cur_evt = 0;
	
	tmp_prev_evt = SyGetEvtInfo()->old_evt_id;
	
	if (tmp_prev_evt == GSD_EVT_ID_ENDING) {
		dm_stfrl_is_full_staffroll = TRUE;
	}
	else {
		dm_stfrl_is_full_staffroll = FALSE;
	}
	
	if (dm_stfrl_mgr_p == NULL) {
		dm_stfrl_mgr_p = &dm_stfrl_mgr;
	}
	
#if defined (GSD_DEBUG_DEMO_SELECT) || defined (GSD_DEBUG_DEMO_SELECT_TOP)
	if (tmp_prev_evt == GSD_EVT_ID_DEBUG_DEMO) {
		dm_stfrl_is_full_staffroll = FALSE;
	}
#endif
	
	// 現在ロードしているイベントがメインゲームかどうかの設定
	tmp_cur_evt = SyGetEvtInfo()->cur_evt_id;
	
	if (tmp_cur_evt == GSD_EVT_ID_MAINGAME
		|| tmp_cur_evt == GSD_EVT_ID_SPSTAGE_BRANCH) {
		// ポーズメニューからの遷移フラグON
		dm_stfrl_is_pause_maingame = TRUE;
	}
	else {
		dm_stfrl_is_pause_maingame = FALSE;
	}
	
	// スタッフロール開始
	dmStaffRollInit();
}



// ==========================================================================
// DmStaffRollIsExit
/*!
	スタッフロール画面の終了確認処理
 */
// ==========================================================================
BOOL DmStaffRollIsExit(void)
{
	if (dm_stfrl_mgr_p) {
		if (dm_stfrl_mgr_p->tcb == NULL) {
			return TRUE;
		}
	}
	else {
		return TRUE;
	}

	return FALSE;
}


// ==========================================================================
// DmStaffRollExit
/*!
	スタッフロール画面の終了処理
 */
// ==========================================================================
void DmStaffRollExit(void)
{
	// タスククリア
	if (dm_stfrl_mgr_p->tcb) {
		mtTaskClearTcb(dm_stfrl_mgr_p->tcb);

		dm_stfrl_mgr_p->tcb = NULL;
	}
}



// ----- Static Functions --------------------（スタティック関数の定義：局所）
// ==========================================================================
// dmStaffRollInit
/*!
	スタッフロール画面初期化処理
 */
// ==========================================================================
void dmStaffRollInit(void)
{
	DMS_STFRL_MAIN_WORK	*main_work;
	
	// メインタスク作成
	dm_stfrl_mgr_p->tcb = MTM_TASK_MAKE_TCB(dmStaffRollProcMain
											   , dmStaffRollDest
											   , 0
											   , DMD_STFRL_TASK_PAUSELEVEL
											   , DMD_STFRL_TASK_PRIO_MAIN
											   , DMD_STFRL_TASK_GROUP_MAIN
											   , sizeof(DMS_STFRL_MAIN_WORK)
											   , "STAFFROLL_MAIN"
											   );
	
	// ワーク初期化
	main_work = (DMS_STFRL_MAIN_WORK *)mtTaskGetTcbWork(dm_stfrl_mgr_p->tcb);
	
//	main_work->draw_state = AoActSysGetDrawState();

	AoActSysSetDrawStateEnable(TRUE);
	AoActSysSetDrawState(AoActSysGetDrawState());
//	AoActSysSetDrawState(0);

	// 初期化処理があればここに記述
	dmStaffRollSetInitData(main_work);
	
	// プロシージャ設定
	main_work->proc_update = dmStaffRollProcInit;
}



// ==========================================================================
// dmStaffRollSetInitData
/*!
	スタッフロールに必要な初期化設定処理
 */
// ==========================================================================
void dmStaffRollSetInitData(DMS_STFRL_MAIN_WORK *main_work)
{
	
	main_work->disp_mode = DME_STFRL_DISP_MODE_MAIN;
	main_work->disp_frm_time = 1.0f;
	
	main_work->question_act_alpha.r = DMD_STFRL_EFCT_QSTN_COLOR_INIT;
	main_work->question_act_alpha.g = DMD_STFRL_EFCT_QSTN_COLOR_INIT;
	main_work->question_act_alpha.b = DMD_STFRL_EFCT_QSTN_COLOR_INIT;
	main_work->question_act_alpha.a = DMD_STFRL_EFCT_QSTN_ALPHA_INIT;
	
	// カオスエメラルドをコンプリートしてる状態かどうか
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD) {
		main_work->is_eme_comp = TRUE;
	}
	else {
		main_work->is_eme_comp = FALSE;
	}
	
	
#if 0//defined(MTD_DEBUG)
	if (dm_stfrl_is_full_disp_efct) {
		main_work->is_eme_comp = TRUE;
	}
	else {
		main_work->is_eme_comp = FALSE;
	}
#endif
}



// ==========================================================================
// dmStaffRollProcMain
/*!
	スタッフロール画面メインプロシージャ処理
 */
// ==========================================================================
void dmStaffRollProcMain(MTS_TASK_TCB *tcb)
{
	DMS_STFRL_MAIN_WORK	*main_work;

	// ワーク取得
	main_work = (DMS_STFRL_MAIN_WORK *)mtTaskGetTcbWork(tcb);
	
	// 終了処理
	if (main_work->flag & DMD_STFRL_FLAG_EXIT) {
		// タスククリア
		DmStaffRollExit();
		
		// イベント遷移用設定(イベント遷移するのはフルバージョンのときのみ)
		if (dm_stfrl_is_full_staffroll) {
			dmStaffRollSetNextEvt(main_work);
		}
		
		return;
	}
	
#if _WII
	// ホームボタンメニューを押されたときの処理
	dmStaffRollIsHBMDraw(main_work);
#endif
	
	// システム関連処理(サインアウト時はタイトルへ戻す)
	if (main_work->flag & DMD_STFRL_FLAG_SIGN_OUT_EXIT
		&& !AoAccountIsCurrentEnable()) {
		
		main_work->proc_update = dmStaffRollProcFadeOut;
		
		// サインアウト終了フラグOFF
		main_work->flag &= ~DMD_STFRL_FLAG_SIGN_OUT_EXIT;
		main_work->flag |= DMD_STFRL_FLAG_SET_EVT_SIGN_OUT;
		
		// フェード終了		※問題あれば有効にする
//		IzFadeExit();
		
		if (dm_stfrl_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_TAKEOEVER
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_STFRL_MODE_FADEOUT_TIME
								, TRUE
								);
		}
		else {
			// フェードアウト開始
			IzFadeInitEasy(IZE_FADE_SET_TYPE_TAKEOEVER
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_STFRL_FADEOUT_TIME
						   );
		}
		
		// BGMフェードアウト開始
		if (main_work->bgm_scb) {
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_STFRL_MODE_SND_FADEOUT_TIME - 1
						   );
		}
		
		// ウインドウ遷移関連設定
		main_work->flag &= ~DMD_STFRL_FLAG_DECIDE;
		main_work->flag &= ~DMD_STFRL_FLAG_CANCEL;
		main_work->proc_input = NULL;
		main_work->win_timer = 0;
		main_work->win_mode = DME_STFRL_WIN_GET_EMERALD;
	}
	
	// 更新処理用プロシージャ
	if (main_work->proc_update) {
#if _WII
		if (!(main_work->flag & DMD_STFRL_FLAG_ALL_PROC_STOP)) {
			main_work->proc_update(main_work);
		}
#else
		main_work->proc_update(main_work);
#endif
	}

	// 描画設定プロシージャ
	if (main_work->proc_draw) {
		main_work->proc_draw(main_work);
	}

}


// ==========================================================================
// dmStaffRollDest
/*!
	スタッフロール画面終了処理
 */
// ==========================================================================
void dmStaffRollDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	if (dm_stfrl_is_full_staffroll) {
		// エフェクトシステム終了
		MTM_ASSERT(ObjDrawESEffectSystemIsActive());
		ObjDrawESEffectSystemExit();
		
		ObjCameraExit();
		
		// メインデータ解放処理
		GmMainExitForStaffroll();
	}
	else {
		// 処理なし
	}
}



// ==========================================================================
// dmStaffRollSetNextEvt
/*!
	次のイベント遷移設定処理
 */
// ==========================================================================
void dmStaffRollSetNextEvt(DMS_STFRL_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
	s16 next_evt = 0;
	
	if (dm_stfrl_is_full_staffroll) {
		next_evt = (s16)DME_STFRL_NEXT_EVT_MAINMENU;
		
		if (main_work->flag & DMD_STFRL_FLAG_SET_EVT_SIGN_OUT) {
			next_evt = (s16)DME_STFRL_NEXT_EVT_TITLE;
		}
		
		// 次のイベント設定
		SyDecideEvtCase(next_evt);
	}
	else {
		next_evt = SyGetEvtInfo()->old_evt_id;
		
		if (main_work->flag & DMD_STFRL_FLAG_SET_EVT_SIGN_OUT) {
			next_evt = (s16)DME_STFRL_NEXT_EVT_TITLE;
		}
		
		SyDecideEvt(next_evt);
	}
	
	// イベント遷移処理はデータ解放タスク内で行っているため、ここでは行わない
//	SyChangeNextEvt();
}



// ==========================================================================
// dmStaffRollProcInit
/*!
	テクスチャ構築待ち処理
 */
// ==========================================================================
void dmStaffRollProcInit(DMS_STFRL_MAIN_WORK *main_work)
{
	// ここでフルバージョンか簡易バージョンかで分かれる
	if (dm_stfrl_is_full_staffroll) {
		main_work->proc_update = dmStaffRollProcLoadData;
		DmStaffRollBuildForGame();
		
		// フォントデータ
		main_work->arc_list_font_amb
			= dm_stfrl_fs_data_mgr_p->arc_list_font_amb_fs;
		
		// スクリーンショット
		main_work->arc_scr_amb_fs
			= dm_stfrl_fs_data_mgr_p->arc_scr_amb_fs;
		
		// END画面用テクスチャ
		main_work->arc_end_amb_fs
			= dm_stfrl_fs_data_mgr_p->arc_end_amb_fs;
		main_work->arc_end_jp_amb_fs
			= dm_stfrl_fs_data_mgr_p->arc_end_jp_amb_fs;
		
		// メニュー共通データ
		main_work->arc_cmn_amb_fs[0]
			= dm_stfrl_fs_data_mgr_p->arc_cmn_amb_fs[0];
		main_work->arc_cmn_amb_fs[1]
			= dm_stfrl_fs_data_mgr_p->arc_cmn_amb_fs[1];
		
		// スタッフ名簿リストデータ
		main_work->staff_list_fs
			= amFsReadBackground(GSS_BASE_PATH "DEMO/STFRL/STAFF_LIST.YSD");
	}
	else {
		// フォントデータ
		main_work->arc_list_font_amb_fs
			= amFsReadBackground(GSS_BASE_PATH "DEMO/STFRL/D_STFRL_FONT.AMB");
		
		// スタッフ名簿リストデータ
		main_work->staff_list_fs
			= amFsReadBackground(GSS_BASE_PATH "DEMO/STFRL/STAFF_LIST.YSD");
		
		main_work->proc_update = dmStaffRollProcLoadData;
	}
#if _IPHONE
	GsMainSysSetSleepFlag(FALSE); // 触ると終了してしまうのでスリープに入らないようにする
#endif // _IPHONE
}



// ==========================================================================
// dmStaffRollProcLoadData
/*!
	テクスチャ構築待ち処理
 */
// ==========================================================================
void dmStaffRollProcLoadData(DMS_STFRL_MAIN_WORK *main_work)
{
	if (dmStaffRollIsDataLoad(main_work)) {
		
		if (dm_stfrl_is_full_staffroll) {
			dmStaffRollDataClearRequestFull(main_work);
			
			// テクスチャ構築
			DmStaffRollBuild(&main_work->arc_data);
		}
		else {
			dmStaffRollDataClearRequestEasy(main_work);
			
			// 簡易版テクスチャ構築
			dmStaffRollDataBuildEasy(main_work);
		}
		
		// テクスチャ構築終了チェックへ遷移
		main_work->proc_update = dmStaffRollProcDataBuild;
	}
}



// ==========================================================================
// dmStaffRollProcDataBuild
/*!
	テクスチャ構築待ち処理
 */
// ==========================================================================
void dmStaffRollProcDataBuild(DMS_STFRL_MAIN_WORK *main_work)
{
	if (DmStaffRollBuildCheck()) {
		if (dm_stfrl_is_full_staffroll) {
			// OBJシステム関連の初期化設定
			dmStaffRollSetObjSystemData(main_work);
			main_work->ring_work[0] = DmStfrlMdlCtrlSetRingObj( 0, 0);
			main_work->ring_work[1] = DmStfrlMdlCtrlSetRingObj(20, 3);
			main_work->ring_work[2] = DmStfrlMdlCtrlSetRingObj(40, 6);
			
			main_work->proc_update = dmStaffRollProcCreateAct;
		}
		else {
			// ログアウトチェックフラグON
			main_work->flag |= DMD_STFRL_FLAG_SIGN_OUT_EXIT;
			
			// イベント遷移
			main_work->proc_update = dmStaffRollProcFadeIn;
			
			// テクスチャセットまで出来たので描画プロシージャを設定
			main_work->proc_draw = dmStaffRollProcActDraw;
			
			// フェードイン開始
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEIN
						   , DMD_STFRL_FADEIN_TIME
						   );
		}
		
		// ここですでにYSDファイル読込み済みなので必要な初期設定をする
		// YSDファイルかどうか
		if (!AoYsdFileIsYsdFile(dm_stfrl_data_mgr_p->stf_list_ysd)) {
			MTM_ASSERT(0);
		}
		
		// 読み込んだYSDファイルのページ数を取得
		main_work->disp_list_page_num
			= AoYsdFileGetPageNum(dm_stfrl_data_mgr_p->stf_list_ysd);
		
		// サウンドSCBアサイン
#if _WII
		main_work->bgm_scb = GsSoundAssignScb(GSE_SND_DATA_TYPE_NW4R);
#else
		main_work->bgm_scb = GsSoundAssignScb(GSE_SND_DATA_TYPE_CRIAUDIO);
#endif
		
		// ユーザーBGM再生時のミュート対応
		main_work->bgm_scb->flag |= GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM;
		
		// SEハンドル確保
		main_work->se_handle = GsSoundAllocSeHandle();
		
		if (!GsSoundIsRunning()) {
			// フレーム更新処理開始
			GsSoundBegin(0x1000,
						 1,
						 3);
			
			main_work->flag |= DMD_STFRL_FLAG_MAKE_SND_TASK;
		}
	
	}
	
}



// ==========================================================================
// dmStaffRollProcCreateAct
/*!
	アクション生成処理

  	※cur_fileを設定する際は必ずcrsr_idxとcur_vrtcl_fileを設定して
  	それらの和を設定すること。
 */
// ==========================================================================
void dmStaffRollProcCreateAct(DMS_STFRL_MAIN_WORK *main_work)
{
	// ファイル選別
	const void *ama = NULL;
	AOS_TEXTURE *tex = NULL;
	
	// アクション構築
	for (u32 i = 0; i < ACT_NUM; ++i) {
#if !_IPHONE
		if (i >= ACT_TEX_WINTITLE) {
			ama = dm_stfrl_cmn_ama[1];
			tex = &dm_stfrl_cmn_tex[1];
		}
		else if (i >= ACT_WIN_LINE) {
			ama = dm_stfrl_cmn_ama[0];
			tex = &dm_stfrl_cmn_tex[0];
		}
#else //!_IPHONE
		if (false) {
		}
#endif //!_IPHONE
		else if (i >= ACT_TEX_TRYAGAIN) {
			ama = dm_stfrl_end_lng_ama;
			tex = &dm_stfrl_end_jp_tex;
		}
		else {
			ama = dm_stfrl_end_cmn_ama;
			tex = &dm_stfrl_end_tex;
		}
		
		// 構築
		AoActSetTexture(AoTexGetTexList(tex));
		main_work->act[i] = AoActCreate(ama, g_dm_act_id_tbl[i]);
	}
	
	// ログアウトチェックフラグON
	main_work->flag |= DMD_STFRL_FLAG_SIGN_OUT_EXIT;
	
	// イベント遷移
	main_work->proc_update = dmStaffRollProcFadeIn;
	
	// テクスチャセットまで出来たので描画プロシージャを設定
	main_work->proc_draw = dmStaffRollProcActDraw;
	
	// フェードイン開始
	IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
				   , IZE_FADE_TYPE_BLACK_FADEIN
				   , DMD_STFRL_FADEIN_TIME
				   );
}



// ==========================================================================
// dmStaffRollProcFadeIn
/*!
	スタッフロール時のフェードイン中処理(開始時)
 */
// ==========================================================================
void dmStaffRollProcFadeIn(DMS_STFRL_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
		IzFadeExit();
		
		// プロシージャ切り替え
		main_work->proc_update = dmStaffRollProcNowStaffRoll;
		
		// フェード演出ページ情報設定
		dmStaffRollSetFadePageInfoEfctData(main_work);
		
		main_work->timer = DMD_STFRL_EFCT_FADE_WAIT_TIME;
		
		// ここでサウンド再生
		
		if (dm_stfrl_is_full_staffroll) {
			GsSoundPlayBgm(main_work->bgm_scb
						   , "snd_sng_ending"
						   , 0
						   );
		}
		else {
			GsSoundPlayBgm(main_work->bgm_scb
						   , "snd_sng_z1a1"
						   , 0
						   );
		}
		
		// ここはフルでないか、フルで一回見たあとかの場合のみ設定
		main_work->proc_input = dmStaffRollInputProcStaffRollMain;
	}
}



// ==========================================================================
// dmStaffRollProcDispWaitStaffList
/*!
	スタッフロール表示中処理
 */
// ==========================================================================
void dmStaffRollProcDispWaitStaffList(DMS_STFRL_MAIN_WORK *main_work)
{
	// スキップ入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}
	
	if (main_work->flag & DMD_STFRL_FLAG_DECIDE) {
		// ページ番号を範囲内に丸める
//		main_work->cur_disp_list_page = main_work->disp_list_page_num - 1;
		
		// 
		if (dm_stfrl_is_full_staffroll) {
			main_work->proc_update = dmStaffRollProcModeFadeOut;
		}
		else {
			main_work->proc_update = dmStaffRollProcFadeOut;
		}
		
		// フェード処理開始
		if (dm_stfrl_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_STFRL_MODE_FADEOUT_TIME
								, TRUE
								);
			
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_STFRL_MODE_SND_FADEOUT_TIME
						   );
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_STFRL_MODE_FADEOUT_TIME
						   );
			
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_STFRL_MODE_SND_FADEOUT_TIME
						   );
		}
		
		// フェード処理開始
		
		main_work->flag &= ~DMD_STFRL_FLAG_DECIDE;
		
		return;
	}
	
	
	// 最後のページまで一定時間ごとにスタッフリストを表示切替
	if (main_work->timer <= 0.f) {
		// 表示処理へ切り替え
		main_work->proc_update = dmStaffRollProcNowStaffRoll;
		
		// 必要な初期設定をここで行う(フラグ設定や初めの表示時間など)
		
		main_work->timer = 0.f;
	}
	
	// タイマー更新
	main_work->timer -= main_work->disp_frm_time;
	
	
}



// ==========================================================================
// dmStaffRollProcNowStaffRoll
/*!
	スタッフロール時のスタッフロール中処理
 */
// ==========================================================================
void dmStaffRollProcNowStaffRoll(DMS_STFRL_MAIN_WORK *main_work)
{
	// スキップ入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}
	
	if (main_work->flag & DMD_STFRL_FLAG_DECIDE) {
		// ページ番号を範囲内に丸める
//		main_work->cur_disp_list_page = main_work->disp_list_page_num - 1;
		
		if (dm_stfrl_is_full_staffroll) {
			main_work->proc_update = dmStaffRollProcModeFadeOut;
		}
		else {
			main_work->proc_update = dmStaffRollProcFadeOut;
		}
		
		// フェード処理開始
		if (dm_stfrl_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_STFRL_MODE_FADEOUT_TIME
								, TRUE
								);
			
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_STFRL_MODE_SND_FADEOUT_TIME
						   );
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_STFRL_MODE_FADEOUT_TIME
						   );
			
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_STFRL_MODE_SND_FADEOUT_TIME
						   );
		}
		
		main_work->flag &= ~DMD_STFRL_FLAG_DECIDE;
		
		return;
	}
	
	
	// フェードアウトの演出設定部分でフェードアウトが終ったらデータを切り替え
	if (main_work->fade_timer <= 0.f) {
		main_work->proc_update = dmStaffRollProcSetChangeData;
		
		main_work->fade_timer = 0.f;
		
		return;
	}
	
	// データを切り替えたあとはそのまま待ちに入るプロシージャを挟む
	
	
	// α度フェード演出設定処理
	dmStaffRollSetEfctChngAlphaListData(main_work);
	
	// 表示時間計測
	main_work->fade_timer -= main_work->disp_frm_time;
	
}



// ==========================================================================
// dmStaffRollProcSetChangeData
/*!
	スタッフロール表示切り替え設定処理
 */
// ==========================================================================
void dmStaffRollProcSetChangeData(DMS_STFRL_MAIN_WORK *main_work)
{
	// スキップ入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}
	
	if (main_work->flag & DMD_STFRL_FLAG_DECIDE) {
		// ページ番号を範囲内に丸める
//		main_work->cur_disp_list_page = main_work->disp_list_page_num - 1;
		
		if (dm_stfrl_is_full_staffroll) {
			main_work->proc_update = dmStaffRollProcModeFadeOut;
		}
		else {
			main_work->proc_update = dmStaffRollProcFadeOut;
		}
		
		// フェード処理開始
		if (dm_stfrl_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_STFRL_MODE_FADEOUT_TIME
								, TRUE
								);
			
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_STFRL_MODE_SND_FADEOUT_TIME
						   );
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_STFRL_MODE_FADEOUT_TIME
						   );
			
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_STFRL_MODE_SND_FADEOUT_TIME
						   );
		}
		
		main_work->flag &= ~DMD_STFRL_FLAG_DECIDE;
		
		return;
	}
	
	
	// ページ番号更新(リストは常に、スクリーンはフラグONのときのみ
	main_work->cur_disp_list_page++;
	
	// ページ番号が最後のページ以上になった場合
	if (main_work->cur_disp_list_page > (u32)(main_work->disp_list_page_num - 1)) {
		// ページ番号を範囲内に丸める
		main_work->cur_disp_list_page = main_work->disp_list_page_num - 1;
		
		if (dm_stfrl_is_full_staffroll) {
			main_work->proc_update = dmStaffRollProcModeFadeOut;
		}
		else {
			main_work->proc_update = dmStaffRollProcFadeOut;
		}
		
		// フェード処理開始
		if (dm_stfrl_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_STFRL_MODE_FADEOUT_TIME
								, TRUE
								);
			
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_STFRL_MODE_SND_FADEOUT_TIME
						   );
		}
		else {
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_STFRL_MODE_FADEOUT_TIME
						   );
			
			GsSoundStopBgm(main_work->bgm_scb
						   , DMD_STFRL_MODE_SND_FADEOUT_TIME
						   );
		}
		
		return;
	}
	
	// フェード演出ページ情報設定
	dmStaffRollSetFadePageInfoEfctData(main_work);
	
	
	main_work->timer = DMD_STFRL_EFCT_FADE_WAIT_TIME;
	
	
	main_work->proc_update = dmStaffRollProcDispWaitStaffList;
	
	
	UNREFERENCED_PARAMETER(main_work);
}



// ==========================================================================
// dmStaffRollProcModeFadeOut
/*!
	モード切替フェードアウト中処理
 */
// ==========================================================================
void dmStaffRollProcModeFadeOut(DMS_STFRL_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
//		IzFadeExit();
		
		main_work->proc_update = dmStaffRollProcModeFadeIn;
		
		// モード切替
		main_work->disp_mode = DME_STFRL_DISP_MODE_END;
		
		if (dm_stfrl_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEIN
								, DMD_STFRL_MODE_FADEIN_TIME
								, TRUE
								);
		}
		else {
			// フェードイン開始
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEIN
						   , DMD_STFRL_MODE_FADEIN_TIME
						   );
		}
		
		// ここでEND画面用のモデル構築設定処理
		dmStaffRollSetupEndModel(main_work);
	}
}



// ==========================================================================
// dmStaffRollProcModeFadeIn
/*!
	モード切替フェードイン中処理
 */
// ==========================================================================
void dmStaffRollProcModeFadeIn(DMS_STFRL_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
		IzFadeExit();
		
		// プロシージャ切り替え
		main_work->proc_update = dmStaffRollProcEndModeIdle;
		main_work->proc_input = dmStaffRollInputProcWin;
		
		main_work->timer = DMD_STFRL_EFCT_FADE_WAIT_TIME;
		
		main_work->flag |= DMD_STFRL_FLAG_COUNT_TIME;
		
		
//		GmRingInit();
		
	}
}



// ==========================================================================
// dmStaffRollProcEndModeIdle
/*!
	エンド画面モード入力待ち中処理
 */
// ==========================================================================
void dmStaffRollProcEndModeIdle(DMS_STFRL_MAIN_WORK *main_work)
{
	if (main_work->flag & DMD_STFRL_FLAG_POSSIBLE_INPUT
		&& main_work->proc_input) {
		main_work->proc_input(main_work);
	}
	
	// 遷移フラグONならば
	if (main_work->flag & DMD_STFRL_FLAG_DECIDE) {
		
		// プロシージャ切り替え
		if (dm_stfrl_is_full_staffroll) {
			main_work->proc_update = dmStaffRollProcWinModeFadeOut;
			main_work->proc_input = NULL;
		}
		else {
			main_work->proc_update = dmStaffRollProcFadeOut;
			main_work->proc_input = NULL;
		}
		
		main_work->flag &= ~DMD_STFRL_FLAG_DECIDE;
		
		if (dm_stfrl_is_pause_maingame) {
			IzFadeInitEasyColor(IZD_FADE_TASK_GROUP_DEF
								, 0x7fff
								, IZD_FADE_DT_PRIO_DEF
								, IZD_FADE_DRAW_STATE_DEF
								, IZE_FADE_SET_TYPE_NORMAL
								, IZE_FADE_TYPE_BLACK_FADEOUT
								, DMD_STFRL_MODE_FADEOUT_TIME
								, TRUE
								);
		}
		else {
			// フェード処理開始
			IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
						   , IZE_FADE_TYPE_BLACK_FADEOUT
						   , DMD_STFRL_MODE_FADEOUT_TIME
						   );
		}
		
		return;
	}
	
	
	// カオスエメラルドをコンプリートしてる場合
	if (main_work->is_eme_comp) {
		// END画面の全演出が終了したら
		if (main_work->body_work->flag & DMD_STFRL_BODY_FLAG_COMP_EFCT_END) {
			// END画面の？の透過度を加算していく
			main_work->question_act_alpha.a += DMD_STFRL_EFCT_QSTN_FADE_SPD;
			
			// 不透明になったとき
			if (main_work->question_act_alpha.a > DMD_STFRL_EFCT_QSTN_ALPHA_MAX - DMD_STFRL_EFCT_QSTN_FADE_SPD) {
				main_work->question_act_alpha.a = DMD_STFRL_EFCT_QSTN_ALPHA_MAX;
				
				main_work->body_work->flag &= ~DMD_STFRL_BODY_FLAG_COMP_EFCT_END;
			}
		}
		
		if (main_work->body_work->flag & DMD_STFRL_BODY_FLAG_M_SONIC_EFCT_START) {
			main_work->end_act_frm++;
		}
		
		if (main_work->end_act_frm > DMD_STFRL_TEX_CONTINUE_DISP_TIME) {
			main_work->flag |= DMD_STFRL_FLAG_DISP_CONTINUE_TEX;
		}
	}
	else {
		// TRY AGAINではアクションを進ませない(保険)
		main_work->end_act_frm = 0;
	}
	
	// メタルソニックの目が光るSE再生の時間になったら
	if (main_work->end_act_frm == DMD_STFRL_PLAY_SE_METAL_SONIC_EYE) {
		GsSoundPlaySe("Metal_Sonic", main_work->se_handle);
	}
	
	
	// 表示演出
	if (main_work->sonic_work != NULL
		&& main_work->body_work != NULL) {
		if (main_work->sonic_work->flag & DMD_STFRL_SONIC_FLAG_EFCT_END) {
			main_work->body_work->flag |= DMD_STFRL_BODY_FLAG_MOVE_DOWN_START;
			
			main_work->sonic_work->flag &= ~DMD_STFRL_SONIC_FLAG_EFCT_END;
		}
	}
	
	
	// タイマー更新
	if (main_work->flag & DMD_STFRL_FLAG_COUNT_TIME) {
		main_work->timer -= main_work->disp_frm_time;
		
		if (main_work->timer <= 0) {
			main_work->flag &= ~DMD_STFRL_FLAG_COUNT_TIME;
			main_work->flag |= DMD_STFRL_FLAG_POSSIBLE_INPUT;
			
			main_work->timer = 0;
		}
	}
}


// ==========================================================================
// dmStaffRollProcWinModeFadeOut
/*!
	ウインドウモード切替フェードアウト中処理
 */
// ==========================================================================
void dmStaffRollProcWinModeFadeOut(DMS_STFRL_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
//		IzFadeExit();
		
		// ここでEND画面のモデル演出終了設定
		dmStaffRollNodispEndModel(main_work);
		
		main_work->proc_update = dmStaffRollProcWinModeFadeIn;
		
		// モード切替
		main_work->disp_mode = DME_STFRL_DISP_MODE_WIN_MSG;
		
		// フェードイン開始
		IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
					   , IZE_FADE_TYPE_BLACK_FADEIN
					   , DMD_STFRL_MODE_FADEIN_TIME
					   );
		
		
		GsSoundStopBgm(main_work->bgm_scb
					   , 0
					   );
	}
}



// ==========================================================================
// dmStaffRollProcWinModeFadeIn
/*!
	ウインドウモード切替フェードイン中処理
 */
// ==========================================================================
void dmStaffRollProcWinModeFadeIn(DMS_STFRL_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
		IzFadeExit();
		
		// プロシージャ切り替え
		main_work->proc_update = dmStaffRollProcWindowNodispIdle;
		
		// ウインドウ表示フラグ設定
		// カオスエメラルド全て所持でない場合
		if (!(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD)) {
			main_work->announce_flag |= 1 << DME_STFRL_WIN_GET_EMERALD;
		}
		
		// セーブ中メッセージフラグ
//		main_work->announce_flag |= 1 << DME_STFRL_WIN_NOW_SAVE;
	}
}



// ==========================================================================
// dmStaffRollProcWindowNodispIdle
/*!
	ウインドウ非表示待ち中処理
 */
// ==========================================================================
void dmStaffRollProcWindowNodispIdle(DMS_STFRL_MAIN_WORK *main_work)
{
	// アナウンスフラグがあればウインドウオープン
	if (main_work->announce_flag) {
		main_work->proc_update = dmStaffRollProcWindowOpenEfct;

		// 通常処理の入力処理をなくす(二重入力を防ぐため)
		main_work->proc_input = NULL;

		// ウインドウ演出用タイマー初期化
		main_work->win_timer = 0;

		// ウインドウ選択変数設定
		for (u32 i = DME_STFRL_WIN_GET_EMERALD; i < DME_STFRL_WIN_NUM; i++) {
			if (main_work->announce_flag & 1 << i) {
				main_work->win_mode = (u32)i;
				break;
			}
		}
		
		// ウインドウ表示フラグON
		main_work->flag |= DMD_STFRL_FLAG_WIN_ALL_DRAW;
		
		GsSoundPlaySe("Window", main_work->se_handle);
		
		// ウインドウ演出中フラグON
//		main_work->flag &= ~DMD_RANK_FLAG_DISP_MENU;
//		main_work->flag |= DMD_RANK_FLAG_WIN_EFCT;
	}
	
	// フラグがなければ終了フェード
	else {
		// プロシージャ切り替え
		main_work->proc_update = dmStaffRollProcTrophyCheck;
	}
}




// ==========================================================================
// dmStaffRollProcWindowOpenEfct
/*!
	ウインドウオープン中処理
 */
// ==========================================================================
void dmStaffRollProcWindowOpenEfct(DMS_STFRL_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_STFRL_FLAG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_update = dmStaffRollProcWindowAnnounceIdle;
		
//		if (main_work->win_mode == DME_RANK_WIN_NOW_REGIST) {
//			main_work->proc_input = dmRankInputProcWinUploadCancel;
//		}
		main_work->proc_input = dmStaffRollInputProcWin;
		
		// ウインドウ内アクション表示フラグON
		main_work->flag |= DMD_STFRL_FLAG_DISP_IN_WINDOW;
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_STFRL_FLAG_WIN_EFCT_END;
	}
	else {
		// ウインドウオープン演出処理
		dmStaffRollSetWinOpenEfct(main_work);
	}
}



// ==========================================================================
// dmStaffRollProcWindowAnnounceIdle
/*!
	ウインドウ入力待ち処理
 */
// ==========================================================================
void dmStaffRollProcWindowAnnounceIdle(DMS_STFRL_MAIN_WORK *main_work)
{
	// ウインドウ入力処理
	if (main_work->proc_input) {
		main_work->proc_input(main_work);
	}

	// ウインドウのパターン分の処理をここに記述
	// ACT決定ウインドウ以外の場合
	if (main_work->win_mode == DME_STFRL_WIN_GET_EMERALD) {
		// メニュー遷移フラグONならば
		if (main_work->flag & DMD_STFRL_FLAG_DECIDE) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_STFRL_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->flag &= ~DMD_STFRL_FLAG_DISP_IN_WINDOW;

			main_work->proc_update = dmStaffRollProcWindowCloseEfct;
			
			GsSoundPlaySe("Ok", main_work->se_handle);
			
			// フラグOFF
			main_work->flag &= ~DMD_STFRL_FLAG_DECIDE;
			main_work->flag &= ~DMD_STFRL_FLAG_CANCEL;
		}
	}
	
#if 0
	else if (main_work->win_mode == DME_STFRL_WIN_NOW_SAVE) {
		// メニュー遷移フラグONならば
		if (main_work->flag & DMD_STFRL_FLAG_DECIDE) {
			// 通常処理の入力処理をなくす(二重入力を防ぐため)
			main_work->proc_input = NULL;

			// ウインドウ演出時間設定
			main_work->win_timer = DMD_STFRL_WIN_EFCT_TIME;
			
			// ウインドウ内アクション表示フラグOFF
			main_work->flag &= ~DMD_STFRL_FLAG_DISP_IN_WINDOW;

			main_work->proc_update = dmStaffRollProcWindowCloseEfct;
			
			GsSoundPlaySe("Ok", main_work->se_handle);
			
			// フラグOFF
			main_work->flag &= ~DMD_STFRL_FLAG_DECIDE;
			main_work->flag &= ~DMD_STFRL_FLAG_CANCEL;
		}
	}
#endif
}



// ==========================================================================
// dmStaffRollProcWindowCloseEfct
/*!
	ウインドウクローズ中処理
 */
// ==========================================================================
void dmStaffRollProcWindowCloseEfct(DMS_STFRL_MAIN_WORK *main_work)
{
	// 演出終了チェック
	if (main_work->flag & DMD_STFRL_FLAG_WIN_EFCT_END) {
		// ウインドウのプロシージャ設定
		main_work->proc_update = dmStaffRollProcWindowNodispIdle;
		
		// アナウンス分のフラグOFF
		main_work->announce_flag &= ~(1 << main_work->win_mode);

//		if (main_work->win_mode == DME_STFRL_WIN_GET_EMERALD) {
//			// 入力処理設定
//			main_work->announce_flag |= 1 << DME_STFRL_WIN_NOW_SAVE;
//		}
		
		// ウインドウ表示フラグOFF
		main_work->flag &= ~DMD_STFRL_FLAG_WIN_ALL_DRAW;
		
		// ウインドウ演出中フラグOFF
		main_work->flag &= ~DMD_STFRL_FLAG_WIN_EFCT_END;
	}
	
	// ウインドウオープン演出処理
	dmStaffRollSetWinCloseEfct(main_work);
}



// ==========================================================================
// dmStaffRollProcTrophyCheck
/*!
	セーブ中チェック処理
 */
// ==========================================================================
void dmStaffRollProcTrophyCheck(DMS_STFRL_MAIN_WORK *main_work)
{
	// プロシージャ切り替え
	main_work->proc_update = dmStaffRollProcFadeOut;
	
	// 実績チェック
	if (dm_stfrl_is_full_staffroll) {
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_END_CREDITS_FINISHED);
	}
	
	// フェード処理開始
	IzFadeInitEasy(IZE_FADE_SET_TYPE_NORMAL
				   , IZE_FADE_TYPE_BLACK_FADEOUT
				   , DMD_STFRL_MODE_FADEOUT_TIME
				   );
}



// ==========================================================================
// dmStaffRollProcFadeOut
/*!
	フェードアウト中処理
 */
// ==========================================================================
void dmStaffRollProcFadeOut(DMS_STFRL_MAIN_WORK *main_work)
{
	if (IzFadeIsEnd()) {
		// フェード終了
//		IzFadeExit();
		
		if (main_work->flag & DMD_STFRL_FLAG_MAKE_SND_TASK) {
			// サウンド停止処理
			GsSoundHalt();
			
			// フレーム更新処理終了
			GsSoundEnd();
		}
		
		// SCB破棄
		if (main_work->bgm_scb) {
			GsSoundStopBgm(main_work->bgm_scb, 0);
			GsSoundResignScb(main_work->bgm_scb);
			main_work->bgm_scb	= NULL;
		}
		
		if (main_work->se_handle) {
			GsSoundFreeSeHandle(main_work->se_handle);
			main_work->se_handle = NULL;
		}
		
		main_work->proc_update = dmStaffRollProcStopDraw;
		main_work->proc_draw = NULL;
	}
}



// ==========================================================================
// dmStaffRollProcStopDraw
/*!
	描画停止処理
 */
// ==========================================================================
void dmStaffRollProcStopDraw(DMS_STFRL_MAIN_WORK *main_work)
{
	for (int i = 0; i < ACT_NUM; i++) {
		if (main_work->act[i]) {
			AoActDelete(main_work->act[i]);
			main_work->act[i] = NULL;
		}
	}
	
//	if (dm_stfrl_is_full_staffroll) {
		main_work->proc_update = dmStaffRollProcDataRelease;
//	}
	
//	else {
//		main_work->proc_update = NULL;
//		main_work->flag |= DMD_STFRL_FLAG_EXIT;
//	}
}



// ==========================================================================
// dmStaffRollProcDataRelease
/*!
	ファイル解放リクエスト処理
 */
// ==========================================================================
void dmStaffRollProcDataRelease(DMS_STFRL_MAIN_WORK *main_work)
{
	// テクスチャ解放
	DmStaffRollFlush();
	
//	DmSoundFlush();
	
	if (dm_stfrl_is_full_staffroll) {
		// 管理オブジェクト破棄
		ObjObjectClearAllObject();
		// オブジェクトシステム終了前処理
		ObjPreExit();
	}
	
	// 次へ遷移
	main_work->proc_update = dmStaffRollProcFinish;
}


// ==========================================================================
// dmStaffRollProcFinish
/*!
	終了処理
 */
// ==========================================================================
void dmStaffRollProcFinish(DMS_STFRL_MAIN_WORK *main_work)
{
	// テクスチャ解放完了判定
	if (DmStaffRollFlushCheck()) {
		for (int i = 0; i < ACT_NUM; i++) {
			if (main_work->act[i]) {
				AoActDelete(main_work->act[i]);
				main_work->act[i] = NULL;
			}
		}
		
		// フルバージョンの場合
		if (dm_stfrl_is_full_staffroll) {
			// ファイル解放はしない(メイン終了時に解放)
			if (dm_stfrl_data_mgr_p->arc_font_amb) {
				dm_stfrl_data_mgr_p->arc_font_amb = NULL;
			}
			
			// ファイル解放はしない(メイン終了時に解放)
			if (dm_stfrl_data_mgr_p->arc_scr_amb) {
				dm_stfrl_data_mgr_p->arc_scr_amb = NULL;
			}
			
			// ファイル解放はしない(メイン終了時に解放)
			if (dm_stfrl_data_mgr_p->arc_end_amb) {
				dm_stfrl_data_mgr_p->arc_end_amb = NULL;
			}
			
			// ファイル解放はしない(メイン終了時に解放)
			if (dm_stfrl_data_mgr_p->arc_end_jp_amb) {
				dm_stfrl_data_mgr_p->arc_end_jp_amb = NULL;
			}
			
			for (int i = 0; i < 2; i++) {
				// ファイル解放はしない(メイン終了時に解放)
				if (dm_stfrl_data_mgr_p->arc_cmn_amb[i]) {
					dm_stfrl_data_mgr_p->arc_cmn_amb[i] = NULL;
				}
			}
		}
		// 簡易版の場合
		else {
			if (dm_stfrl_font_amb) {
				amMemFree(dm_stfrl_font_amb);
				dm_stfrl_font_amb = NULL;
			}
		}
		
		// YSDファイル解放
		if (dm_stfrl_data_mgr_p->stf_list_ysd) {
			amMemFree(dm_stfrl_data_mgr_p->stf_list_ysd);
			dm_stfrl_data_mgr_p->stf_list_ysd = NULL;
		}
		
		
		if (main_work->page_line_type) {
			amMemFree(main_work->page_line_type);
			main_work->page_line_type = NULL;
		}
		
		
		if (dm_stfrl_is_full_staffroll) {
			// 終了処理へ
			main_work->proc_update = dmStaffRollProcSaveEndCheck;
			
			// ここに仮のセーブ待ち処理追加
			DmSaveMenuStart(TRUE);
		}
		else {
			main_work->flag |= DMD_STFRL_FLAG_EXIT;
			main_work->proc_update = NULL;
		}
	}
#if _IPHONE
	GsMainSysSetSleepFlag(TRUE); // スリープに入れるようになった
#endif // _IPHONE
}



// ==========================================================================
// dmStaffRollProcSaveEndCheck
/*!
	セーブ中チェック処理
 */
// ==========================================================================
void dmStaffRollProcSaveEndCheck(DMS_STFRL_MAIN_WORK *main_work)
{
	if (DmSaveIsExit()) {
		// 終了処理へ
		main_work->flag |= DMD_STFRL_FLAG_EXIT;
		main_work->proc_update = NULL;
		
		if (main_work->flag & DMD_STFRL_FLAG_MAKE_SND_TASK) {
			// サウンドシステムリセット
			GsSoundReset();
		}
		
		// ※※※セーブが終了したら必ずフォントをリリースさせる※※※(ここだけメモリ１から確保してるため)
		GsFontRelease();
	}
}



// ==========================================================================
// dmStaffRollInputProcStaffRollMain
/*!
	スタッフロールメイン用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmStaffRollInputProcStaffRollMain(DMS_STFRL_MAIN_WORK *main_work)
{
	// スキップ判定
#if !_IPHONE
	if (AoPadStand() & KEY_START) {
#else //!_IPHONE
	if (amTpIsTouchPush(0)) {
#endif //!_IPHONE
		// フラグOFF
		main_work->flag |= DMD_STFRL_FLAG_DECIDE;
	}
}



// ==========================================================================
// dmStaffRollInputProcWin
/*!
	ウインドウ用入力プロシージャ処理(据え置き版)
 */
// ==========================================================================
void dmStaffRollInputProcWin(DMS_STFRL_MAIN_WORK *main_work)
{
	// スキップ判定
#if !_IPHONE
	if (AoPadStand() & GSD_KEY_CANCEL
		|| AoPadStand() & GSD_KEY_DECIDE) {
#else //!_IPHONE
	if (amTpIsTouchPush(0)) {
#endif //!_IPHONE
		// フラグOFF
		main_work->flag |= DMD_STFRL_FLAG_DECIDE;
	}
}



// ==========================================================================
// dmStaffRollProcActDraw
/*!
	描画設定プロシージャ処理
 */
// ==========================================================================
void dmStaffRollProcActDraw(DMS_STFRL_MAIN_WORK *main_work)
{
	// 背景描画処理
	dmStaffRollBGDraw(main_work);
	
	// スタッフロールメイン
	if (main_work->disp_mode == DME_STFRL_DISP_MODE_MAIN) {
		// 共通描画処理は描画時は常に設定
		dmStaffRollStaffListDraw(main_work);
		
		if (dm_stfrl_is_full_staffroll) {
			// スクリーン描画設定
			dmStaffRollStageScrDraw(main_work);
		}
	}
	
	// END画面描画
	else if (main_work->disp_mode == DME_STFRL_DISP_MODE_END) {
		if (dm_stfrl_is_full_staffroll) {
			dmStaffRollEndActDraw(main_work);
		}
	}
	
	// ウインドウメッセージ描画
	if (main_work->flag & DMD_STFRL_FLAG_WIN_ALL_DRAW) {		// ウインドウメッセージ
		if (dm_stfrl_is_full_staffroll) {
			dmStaffRollWinActDraw(main_work);
		}
	}
}



// ==========================================================================
// dmStaffRollBGDraw
/*!
	BG描画設定処理
 */
// ==========================================================================
void dmStaffRollBGDraw(DMS_STFRL_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
	AMS_PARAM_DRAW_PRIMITIVE param;
	
	amZeroMemory(&param, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	
	// 背景描画処理
	param.mtx = NULL;
	param.vtxPC3D = (NNS_PRIM3D_PC *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PC) * 4);
	NNS_PRIM3D_PC* v = param.vtxPC3D;
	
	v[0].Pos.x = v[1].Pos.x = -160.0f;
	v[2].Pos.x = v[3].Pos.x = v[0].Pos.x + 1280.0f;
	v[0].Pos.y = v[2].Pos.y = 0.0f;
	v[1].Pos.y = v[3].Pos.y = 720.0f;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.f;
	v[0].Col = v[1].Col = v[2].Col = v[3].Col = AMD_RGBA8888(0, 0, 0, 255);
	
	param.format3D = NNE_PRIM3D_FMT_PC;
	param.type = NNE_PRIM_TRIANGLE_STRIP;
	param.count = 4;

	param.ablend = NNE_PRIM_ALPHABLEND_ON;
//	param.zOffset = -1.0f;

#if 1
	amDrawGetPrimBlendParam(AMDRAWE_BLENDTYPE_NORMAL, &param);
#elif _PC | _XBOX
	param.bldSrc = NNE_BLENDMODE_SRCALPHA;
	param.bldDst = NNE_BLENDMODE_INVSRCALPHA;
	param.bldMode = NNE_BLENDOP_ADD;
#elif _PS3
	param.bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
	param.bldDst = NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA;
	param.bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
	param.bldSrc = GX_BL_SRCALPHA;
	param.bldDst = GX_BL_INVSRCALPHA;
	param.bldMode = GX_BM_BLEND;
#endif
	param.aTest = 0;
	param.zMask = 1;
	param.zTest = 0;
	
	AoActDrawCorWide(v, 4, AOD_ACT_CORW_NONE);

	
	amDrawPrimitive3D(DMD_STFRL_DRAW_STATE_PRIO_BG, &param);
	
	// 描画タスク生成
	amDrawMakeTask(dmStaffRollTaskBgDraw, (u16)DMD_STFRL_DRAW_PRIO_BG, (u32)0);
}



// ==========================================================================
// dmStaffRollTaskBgDraw
/*!
	スタッフロール画面の背景描画タスク
 */
// ==========================================================================
void dmStaffRollTaskBgDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(DMD_STFRL_DRAW_STATE_PRIO_BG);
	amDrawEndScene();
}



// ==========================================================================
// dmStaffRollStageScrDraw
/*!
	ステージスクリーン描画設定処理
 */
// ==========================================================================
void dmStaffRollStageScrDraw(DMS_STFRL_MAIN_WORK *main_work)
{
	AMS_PARAM_DRAW_PRIMITIVE param;
	
	amZeroMemory(&param, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	
	param.mtx = NULL;
	param.vtxPCT3D = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6);
	NNS_PRIM3D_PCT* v = param.vtxPCT3D;

	v[0].Pos.x = v[1].Pos.x = DMD_STFRL_SCR_DISP_POS_X;
	v[2].Pos.x = v[3].Pos.x = DMD_STFRL_SCR_DISP_POS_X + DMD_STFRL_SIZE_SCR_TEXTURE_X;
	v[0].Pos.y = v[2].Pos.y = DMD_STFRL_SCR_DISP_POS_Y;
	v[1].Pos.y = v[3].Pos.y = DMD_STFRL_SCR_DISP_POS_Y + DMD_STFRL_SIZE_SCR_TEXTURE_Y;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.f;
	v[0].Col = v[1].Col = v[2].Col = v[3].Col = AMD_RGBA8888(255
															 , 255
															 , 255
															 , main_work->cur_page_scr_alpha_data
															 );
	
	v[0].Tex.u = v[1].Tex.u = 0.0f;
	v[2].Tex.u = v[3].Tex.u = DMD_STFRL_UV_U_RATE_TEXTURE;
	v[0].Tex.v = v[2].Tex.v = 0.0f;
	v[1].Tex.v = v[3].Tex.v = DMD_STFRL_UV_V_RATE_TEXTURE;

	param.format3D = NNE_PRIM3D_FMT_PCT;
	param.type = NNE_PRIM_TRIANGLE_STRIP;
	param.count = 4;
	
	param.texlist = AoTexGetTexList(&dm_stfrl_scr_tex);
	param.texId = (s32)main_work->cur_disp_image;//0;
	param.ablend = NNE_PRIM_ALPHABLEND_ON;
	param.zOffset = -1.0f;

#if _PC | _XBOX
	param.bldSrc = NNE_BLENDMODE_SRCALPHA;
	param.bldDst = NNE_BLENDMODE_INVSRCALPHA;
	param.bldMode = NNE_BLENDOP_ADD;
#elif _PS3
	param.bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
	param.bldDst = NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA;
	param.bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
	param.bldSrc = GX_BL_SRCALPHA;
	param.bldDst = GX_BL_INVSRCALPHA;
	param.bldMode = GX_BM_BLEND;
#endif
	param.aTest = 0;
	param.zMask = 1;
	param.zTest = 0;
	
	
	AoActDrawCorWide(v, 4, AOD_ACT_CORW_LEFT);

	amDrawPrimitive3D(DMD_STFRL_DRAW_STATE_PRIO_FIX, &param);
	
	// 描画タスク生成
	amDrawMakeTask(dmStaffRollStageScrTaskDraw, (u16)DMD_STFRL_DRAW_PRIO_FIX, (u32)0);
}



// ==========================================================================
// dmStaffRollStageScrTaskDraw
/*!
	ステージスクリーンの描画タスク
 */
// ==========================================================================
void dmStaffRollStageScrTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(DMD_STFRL_DRAW_STATE_PRIO_FIX);
	amDrawEndScene();
}



// ==========================================================================
// dmStaffRollStaffListDraw
/*!
	スタッフ名リスト描画設定処理
  	1ページ分の描画設定処理
 */
// ==========================================================================
void dmStaffRollStaffListDraw(DMS_STFRL_MAIN_WORK *main_work)
{
	u32 page_line_num = 0;
	u32 str_height = 0;			// 1ページ分の表示領域高さ
	u32 cal_str_height = 0;
	u32 disp_str_pos_y = 0;
	
//	main_work->cur_disp_list_page = 8;
	
	// 現在のページの全行数を取得
	page_line_num = AoYsdFileGetLineNum(dm_stfrl_data_mgr_p->stf_list_ysd
										, main_work->cur_disp_list_page
										);
	
	// １ページ分の各行の種別IDを保存する領域を確保
	main_work->page_line_type = (u32 *)amMemAlloc(sizeof(u32) * page_line_num);
	amZeroMemory(main_work->page_line_type, sizeof(u32) * page_line_num);
	
	// １ページ分の各行の種別IDを取得
	for (u32 i = 0; i < page_line_num; i++) {
		main_work->page_line_type[i] = (u32)AoYsdFileGetLineId(dm_stfrl_data_mgr_p->stf_list_ysd
								 						  , main_work->cur_disp_list_page
								 						  , i
								 						  );
		
		// 取得したIDの行の高さを加算していく
		str_height += dm_stfrl_list_id_font_height_tbl[main_work->page_line_type[i]];
	}
	
	// スタッフロール表示領域の縦幅を計算
	cal_str_height = str_height / 2;
	disp_str_pos_y = (u32)(DMD_STFRL_SIZE_HALF_HEIGHT - cal_str_height);
	
	
	// １ページ分の描画設定
	for (u32 i = 0; i < page_line_num; i++) {
		// 表示したい行の高さと行番号を指定して1行分の文字を描画設定
		dmStaffRollStaffListOneLineDraw(main_work, disp_str_pos_y, i);
		
		// 表示した行の高さを加算
		disp_str_pos_y += dm_stfrl_list_id_font_height_tbl[main_work->page_line_type[i]];
	}
	
	// 1ページごとに解放
	amMemFree(main_work->page_line_type);
	main_work->page_line_type = NULL;
	
	
	// 描画タスク生成
	amDrawMakeTask(dmStaffRollTaskDraw, (u16)DMD_STFRL_DRAW_PRIO_CHAR, (u32)0);
}



// ==========================================================================
// dmStaffRollStaffListOneLineDraw
/*!
	スタッフ名リスト１行分描画設定処理
 */
// ==========================================================================
void dmStaffRollStaffListOneLineDraw(DMS_STFRL_MAIN_WORK *main_work, u32 disp_pos_y, u32 cur_line)
{
	const char *disp_str = NULL;
	u32 disp_type = 0;
	u32 char_cnt = 0;
	int i = 0;
	AMS_PARAM_DRAW_PRIMITIVE param;
	u32 char_id = 0;
	u32 str_length = 0;			// 1行分の文字列幅
	u32 cal_str_length = 0;
	u32 disp_str_pos_x = 0;
	char disp_char = 0;
	
	char tmp_char_no = 0;
	char char_line_no = 0;
	char char_column_no = 0;
	
	u32 disp_color[3] = {0, 0, 0};
	s32 tmp_tex_id = 0;
	
	float tmp_font_size = 0.f;
	float full_disp_width = 0.f;
	
	if (dm_stfrl_is_full_staffroll
		&& main_work->cur_disp_list_page != (u32)(main_work->disp_list_page_num - 1)) {
#if !_IPHONE
		full_disp_width = DMD_STFRL_FONT_CENTER_DIST_X;
#else //!_IPHONE
		const u32 c_flag_option_no_shift = 0x01; //右シフトしないフラグ
		u32 option = AoYsdFileGetPageOption(dm_stfrl_data_mgr_p->stf_list_ysd
												, main_work->cur_disp_list_page
												);
		if (c_flag_option_no_shift & option) {
			//画像付きスタッフロールでもシフトしない
			full_disp_width = 0.f;
		} else {
			//画像付きスタッフロールなら右にシフトする
			full_disp_width = DMD_STFRL_FONT_CENTER_DIST_X;
		}
#endif //!_IPHONE
	}
	else {
		full_disp_width = 0.f;
	}
	
	
	// 表示する行の種類を特定する(役職か名前かロゴか)
	disp_type = AoYsdFileGetLineId(dm_stfrl_data_mgr_p->stf_list_ysd
								   , main_work->cur_disp_list_page
								   , cur_line
								   );
	
	if (disp_type >= DME_STFRL_FONT_DISP_TYPE_SONIC_TEAM
		&& disp_type <= DME_STFRL_FONT_DISP_TYPE_SEGA) {
		
		// 求めた距離を中心Xから引いて文字の表示開始位置を求める
		str_length = dm_stfrl_list_logo_width_tbl[disp_type];
		cal_str_length = str_length / 2;
		disp_str_pos_x = (u32)(DMD_STFRL_SIZE_HALF_WIDTH - cal_str_length + full_disp_width);
		
		for (int i = 0; i < 3; i++) {
			disp_color[i] = 255;
		}
		
		// SEGAロゴの場合
		if (disp_type == DME_STFRL_FONT_DISP_TYPE_SEGA) {
			if (GsEnvGetRegion() == GSD_REGION_JP) {
				tmp_tex_id = DMD_STFRL_SEGA_LOGO_JP_TEX_ID;
			}
			else {
				tmp_tex_id = DMD_STFRL_SEGA_LOGO_US_TEX_ID;
			}
		}
		// DIMPSロゴならば
		else if (disp_type == DME_STFRL_FONT_DISP_TYPE_DIMPS) {
			tmp_tex_id = DMD_STFRL_DIMPS_LOGO_JP_TEX_ID;
		}
		// SEGAロゴ、DIMPSロゴ以外ならば
		else {
			tmp_tex_id = (s32)(disp_type - DME_STFRL_FONT_DISP_TYPE_SONIC_TEAM + 1);
		}
		
		
		// 1文字分の描画処理
		param.mtx = NULL;
		param.vtxPCT3D = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 4);
		NNS_PRIM3D_PCT* v = param.vtxPCT3D;
		
		// 各文字の表示位置設定
		v[0].Pos.x = v[1].Pos.x = (float)disp_str_pos_x;
		v[2].Pos.x = v[3].Pos.x = (float)(disp_str_pos_x + str_length);// + dm_stfrl_font_width_length_tbl[disp_char]);
		v[0].Pos.y = v[2].Pos.y = (float)disp_pos_y;
		v[1].Pos.y = v[3].Pos.y = (float)(disp_pos_y + dm_stfrl_list_id_font_height_tbl[disp_type]);//dm_stfrl_list_id_font_height_tbl[disp_char]);
		v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.f;
		
		// 種別IDごとに加算させるRGB値を設定
		v[0].Col = v[1].Col = v[2].Col = v[3].Col = AMD_RGBA8888(disp_color[0]
																 , disp_color[1]
																 , disp_color[2]
																 , main_work->cur_page_list_alpha_data
																 );
		
		// 各ロゴのUV座標を指定
		v[0].Tex.u = v[1].Tex.u = 0.f;
		v[2].Tex.u = v[3].Tex.u = 1.f;
		v[0].Tex.v = v[2].Tex.v = 0.f;
		v[1].Tex.v = v[3].Tex.v = (float)(dm_stfrl_list_id_font_height_tbl[disp_type] / 128.f);

		param.format3D = NNE_PRIM3D_FMT_PCT;
		param.type = NNE_PRIM_TRIANGLE_STRIP;
		param.count = 4;

		param.texlist = AoTexGetTexList(&dm_stfrl_font_tex);
		param.texId = tmp_tex_id;
		param.ablend = NNE_PRIM_ALPHABLEND_ON;
//		param.zOffset = -1.0f;
		
#if _PC | _XBOX
		param.bldSrc = NNE_BLENDMODE_SRCALPHA;
		param.bldDst = NNE_BLENDMODE_INVSRCALPHA;
		param.bldMode = NNE_BLENDOP_ADD;
#elif _PS3
		param.bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
		param.bldDst = NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA;
		param.bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
		param.bldSrc = GX_BL_SRCALPHA;
		param.bldDst = GX_BL_INVSRCALPHA;
		param.bldMode = GX_BM_BLEND;
#endif
		param.aTest = 0;
		param.zMask = 1;
		param.zTest = 0;
		
		// ワイド補正
		AoActDrawCorWide(v, 4, AOD_ACT_CORW_CENTER);
		
		amDrawPrimitive3D(DMD_STFRL_DRAW_STATE_PRIO_CHAR, &param);
		
		
		return;
	}
	
#if _IPHONE
	int length = 0; //１行の文字数
#endif //_IPHONE
	
	// フォントの表示サイズを設定
	tmp_font_size = 32.f * dm_stfrl_list_id_font_size_tbl[disp_type];
	
	// 表示する行のタイプから加算させるRGB値を取得
	for (int i = 0; i < 3; i++) {
		disp_color[i] = dm_stfrl_list_id_font_color_tbl[disp_type][i];
	}
	
	// ここで１行分の文字を全て取得
	disp_str = AoYsdFileGetLineString(dm_stfrl_data_mgr_p->stf_list_ysd
									 , main_work->cur_disp_list_page
									 , cur_line
									 );
	
	// １行分の文字の幅を足して左端から中心までの距離を求める
	while (disp_str[i] != '\0') {
		float tmp_font_width = 0.f;
		
		// 表示する文字ID(ASCIIコード)を取得
		char_id = disp_str[i];
		tmp_font_width = dm_stfrl_font_width_length_tbl[char_id] * dm_stfrl_list_id_font_size_tbl[disp_type];
		
		// 指定した文字の幅を表示幅変数に加算していく
		str_length += (u32)tmp_font_width;//dm_stfrl_font_width_length_tbl[char_id];
		
#if _IPHONE
		++length;
#endif //_IPHONE
	
		// カウンタ更新
		i++;
	}
	
	// 求めた距離を中心Xから引いて文字の表示開始位置を求める
	cal_str_length = str_length / 2;
	disp_str_pos_x = (u32)(DMD_STFRL_SIZE_HALF_WIDTH - cal_str_length + full_disp_width);
	
	// ここでフルバージョンの場合に表示位置を中心より右にずらすように設定する。
	
	
	// 上記の設定だけだと2行に出来ないので、オプションで何かの値なら
	// 2行目にするなどを入れるようにする		◆
	
#if _IPHONE
	NNS_PRIM3D_PCT *prim_buf = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6 * length);
#endif //_IPHONE
	
	// 1行分の文字の描画設定処理
	while (disp_str[char_cnt] != '\0') {
		// 1文字描画毎にプリミティブ設定をクリア
		amZeroMemory(&param, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
		
		// 文字列内に文字があるということなので、ここで１文字分を描画
		disp_char = disp_str[char_cnt];
		
		// 表示する文字のIDテーブルにおける行番号と列番号を求めるために一時変数に格納
		tmp_char_no = dm_stfrl_list_font_id_tbl[(s32)disp_char];
		
		// 表示する文字のテーブルにおける行番号を求める
		if (tmp_char_no > 0) {
			char_line_no = (char)(tmp_char_no % DMD_STFRL_CHAR_LINE_NUM);
		}
		else {
			char_line_no = 0;
		}
		
		// 表示する文字のテーブルにおける列番号を求める
		if (tmp_char_no > 0) {
			char_column_no = (char)(tmp_char_no / DMD_STFRL_CHAR_LINE_NUM);
		}
		else {
			char_column_no = 0;
		}
		
		// 1文字分の描画処理
		param.mtx = NULL;
#if !_IPHONE
		param.vtxPCT3D = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 4);
#else //!_IPHONE
		NNS_PRIM3D_PCT prim_temp[4];
		param.vtxPCT3D = prim_temp;
#endif //!_IPHONE
		NNS_PRIM3D_PCT* v = param.vtxPCT3D;
		
		// 各文字の表示位置設定
		v[0].Pos.x = v[1].Pos.x = (float)disp_str_pos_x;
		v[2].Pos.x = v[3].Pos.x = (float)(disp_str_pos_x + tmp_font_size);// + dm_stfrl_font_width_length_tbl[disp_char]);
		v[0].Pos.y = v[2].Pos.y = (float)disp_pos_y;
		v[1].Pos.y = v[3].Pos.y = (float)(disp_pos_y + tmp_font_size);//dm_stfrl_list_id_font_height_tbl[disp_char]);
		v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = v[3].Pos.z = -2.f;
		
		float tmp_font_width = 0.f;
		tmp_font_width = dm_stfrl_font_width_length_tbl[(s32)disp_char] * dm_stfrl_list_id_font_size_tbl[disp_type];
		
		// X座標更新
		disp_str_pos_x = (u32)(disp_str_pos_x + tmp_font_width);//dm_stfrl_font_width_length_tbl[disp_char];
		
		// 種別IDごとに加算させるRGB値を設定
		v[0].Col = v[1].Col = v[2].Col = v[3].Col = AMD_RGBA8888(disp_color[0]
																 , disp_color[1]
																 , disp_color[2]
																 , main_work->cur_page_list_alpha_data
																 );
		
		// 各文字のUV座標を指定(UV指定するのは512x256(フォントテクスチャサイズ)での割合)
		v[0].Tex.u = v[1].Tex.u = char_line_no * DMD_STFRL_DISP_ONE_CHAR_U_RATE;
		v[2].Tex.u = v[3].Tex.u = v[0].Tex.u + DMD_STFRL_DISP_ONE_CHAR_U_RATE;
		v[0].Tex.v = v[2].Tex.v = char_column_no * DMD_STFRL_DISP_ONE_CHAR_V_RATE;
		v[1].Tex.v = v[3].Tex.v = v[0].Tex.v + DMD_STFRL_DISP_ONE_CHAR_V_RATE;

		param.format3D = NNE_PRIM3D_FMT_PCT;
		param.type = NNE_PRIM_TRIANGLE_STRIP;
		param.count = 4;

		param.texlist = AoTexGetTexList(&dm_stfrl_font_tex);
		param.texId = 0;
		param.ablend = NNE_PRIM_ALPHABLEND_ON;
//		param.zOffset = -1.0f;
		
#if _PC | _XBOX
		param.bldSrc = NNE_BLENDMODE_SRCALPHA;
		param.bldDst = NNE_BLENDMODE_INVSRCALPHA;
		param.bldMode = NNE_BLENDOP_ADD;
#elif _PS3
		param.bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
		param.bldDst = NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA;
		param.bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
		param.bldSrc = GX_BL_SRCALPHA;
		param.bldDst = GX_BL_INVSRCALPHA;
		param.bldMode = GX_BM_BLEND;
#endif
		param.aTest = 0;
		param.zMask = 1;
		param.zTest = 0;
		
		// ワイド補正
		AoActDrawCorWide(v, 4, AOD_ACT_CORW_CENTER);
		
#if !_IPHONE
		amDrawPrimitive3D(DMD_STFRL_DRAW_STATE_PRIO_CHAR, &param);
#else //!_IPHONE
		{
			prim_buf[char_cnt * 6 + 0] = v[0];
			prim_buf[char_cnt * 6 + 1] = v[0];
			prim_buf[char_cnt * 6 + 2] = v[1];
			prim_buf[char_cnt * 6 + 3] = v[2];
			prim_buf[char_cnt * 6 + 4] = v[3];
			prim_buf[char_cnt * 6 + 5] = v[3];
		}
		//最後なら描画
		if (disp_str[char_cnt+1] == '\0') {
			param.vtxPCT3D = prim_buf;
			param.count = 6 * length;
			amDrawPrimitive3D(DMD_STFRL_DRAW_STATE_PRIO_CHAR, &param);
		}
#endif //!_IPHONE
		
		char_cnt++;
	}
	
	
	
}



// ==========================================================================
// dmStaffRollTaskDraw
/*!
	スタッフロール画面の描画タスク
 */
// ==========================================================================
void dmStaffRollTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(DMD_STFRL_DRAW_STATE_PRIO_CHAR);
	amDrawEndScene();
}



// ==========================================================================
// dmStaffRollEndActDraw
/*!
	END画面描画設定処理
 */
// ==========================================================================
void dmStaffRollEndActDraw(DMS_STFRL_MAIN_WORK *main_work)
{
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_STFRL_DRAW_PRIO_FIX);
	AoActSysSetDrawStateEnable(TRUE);
	AoActSysSetDrawState(DMD_STFRL_DRAW_STATE_PRIO_FIX);
	
	if (main_work->flag & DMD_STFRL_FLAG_DISP_CONTINUE_TEX) {
		main_work->continue_act_frm++;
	}
	
	// アクション登録
	if (main_work->is_eme_comp) {
		// アクション更新
		AoActSetTexture(AoTexGetTexList(&dm_stfrl_end_tex));
		
		if (main_work->end_act_frm > 0) {
			for (int i = ACT_LIGHT_BG_LT; i <= ACT_M_SONIC_EYE; i++) {
				AoActSortRegAction(main_work->act[i]);
			}
		}
		
		for (int i = ACT_BLACK_BG; i <= ACT_WHITE_BG; i++) {
			AoActSortRegAction(main_work->act[i]);
		}
		
		// アクション更新
		AoActSetTexture(AoTexGetTexList(&dm_stfrl_end_jp_tex));
		
		AoActSortRegAction(main_work->act[ACT_TEX_CONTINUED]);
		
		AoActSetFrame(main_work->act[ACT_TEX_CONTINUED], (f32)main_work->end_act_frm);
	}
	else {
		AoActSortRegAction(main_work->act[ACT_TEX_TRYAGAIN]);
	}
	
	for (int i = ACT_LIGHT_BG_LT; i <= ACT_WHITE_BG; i++) {
		AoActSetFrame(main_work->act[i], (f32)main_work->end_act_frm);
	}
	
	for (int i = 0; i < ACT_NUM; i++) {
		if (i <= ACT_WHITE_BG) {
			AoActSetTexture(AoTexGetTexList(&dm_stfrl_end_tex));
		}
		else {
			AoActSetTexture(AoTexGetTexList(&dm_stfrl_end_jp_tex));
		}
		
		AoActUpdate(main_work->act[i], 0.0f);
	}
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
	
	
	// 描画タスク生成
	amDrawMakeTask(dmStaffRollEndActTaskDraw, (u16)DMD_STFRL_DRAW_PRIO_FIX, (u32)0);
}



// ==========================================================================
// dmStaffRollEndActTaskDraw
/*!
	END画面のアクションの描画タスク
 */
// ==========================================================================
void dmStaffRollEndActTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(DMD_STFRL_DRAW_STATE_PRIO_FIX);
	amDrawEndScene();
}



// ==========================================================================
// dmStaffRollWinActDraw
/*!
	ウインドウ画面描画設定処理
 */
// ==========================================================================
void dmStaffRollWinActDraw(DMS_STFRL_MAIN_WORK *main_work)
{
	f32 tmp_win_size[2] = {0.f, 0.f};
	
	// ゾーンテーブル用AO描画プライオリティ設定
	AoActSysSetDrawTaskPrio(DMD_STFRL_DRAW_PRIO_WIN);
	AoActSysSetDrawStateEnable(TRUE);
	AoActSysSetDrawState(DMD_STFRL_DRAW_STATE_PRIO_WIN);
	
#if _WII || _IPHONE
	tmp_win_size[0] = DMD_STFRL_WINDOW_SIZE_W * DMD_STFRL_DISP_SCALE_TEXT;
	tmp_win_size[1] = DMD_STFRL_WINDOW_SIZE_H * DMD_STFRL_DISP_SCALE_TEXT;
#else
	tmp_win_size[0] = DMD_STFRL_WINDOW_SIZE_W;
	tmp_win_size[1] = DMD_STFRL_WINDOW_SIZE_H;
#endif
	
	// ウインドウ描画(設定ウインドウ)
	AoWinSysDrawState(AOD_WIN_TYPE_A
					 , AoTexGetTexList(&dm_stfrl_cmn_tex[0])
					 , DMD_STFRL_WINDOW_TEX_ID
					 , DMD_STFRL_SIZE_HALF_WIDTH					// ウインドウ中心X
					 , DMD_STFRL_SIZE_HALF_HEIGHT					// ウインドウ中心Y
					 , tmp_win_size[0] * main_work->win_size_rate[0]	// ウインドウ横サイズ
					 , tmp_win_size[1] * main_work->win_size_rate[1]	// ウインドウ縦サイズ
					 , DMD_STFRL_DRAW_STATE_PRIO_WIN				// 描画STATE
					 );
	
	if (main_work->flag & DMD_STFRL_FLAG_DISP_IN_WINDOW) {
		// アクション登録
		AoActSetTexture(AoTexGetTexList(&dm_stfrl_cmn_tex[0]));
//		AoActSortRegAction(main_work->act[ACT_WIN_LINE]);
		
		AoActSetTexture(AoTexGetTexList(&dm_stfrl_cmn_tex[1]));
//		AoActSortRegAction(main_work->act[ACT_TEX_WINTITLE]);
#if !_IPHONE
		AoActSortRegAction(main_work->act[ACT_TEX_OK]);
#endif //!_IPHONE
		
		AoActSetTexture(AoTexGetTexList(&dm_stfrl_end_jp_tex));
		AoActSortRegAction(main_work->act[ACT_TEX_WIN_MSG]);
	}
	
#if !_IPHONE
	AoActSetFrame(main_work->act[ACT_TEX_OK], 0.f);
#endif //!_IPHONE
	
	
	AoActAcmPush();
	
#if !_IPHONE
	// 
	AoActAcmInit();
	
	AoActAcmApplyTrans(dm_stfrl_win_act_pos_tbl[0][0]
					   , dm_stfrl_win_act_pos_tbl[0][1]
					   , 0
					   );
	
	AoActSetTexture(AoTexGetTexList(&dm_stfrl_cmn_tex[0]));
	AoActUpdate(main_work->act[ACT_WIN_LINE], 0.f);
	
	// 
	AoActAcmInit();
	
	AoActAcmApplyTrans(dm_stfrl_win_act_pos_tbl[1][0]
					   , dm_stfrl_win_act_pos_tbl[1][1]
					   , 0
					   );
	
	AoActSetTexture(AoTexGetTexList(&dm_stfrl_cmn_tex[1]));
	AoActUpdate(main_work->act[ACT_TEX_WINTITLE], 0.f);
	
	// 
	AoActAcmInit();
	
#if _WII
	AoActAcmApplyScale(DMD_STFRL_DISP_SCALE_TEXT
					   , DMD_STFRL_DISP_SCALE_TEXT);
#endif
	
	AoActAcmApplyTrans(dm_stfrl_win_act_pos_tbl[2][0]
					   , dm_stfrl_win_act_pos_tbl[2][1]
					   , 0
					   );
	
	AoActSetTexture(AoTexGetTexList(&dm_stfrl_cmn_tex[1]));
	AoActUpdate(main_work->act[ACT_TEX_OK], 0.f);
#endif //!_IPHONE
	
	// 
	AoActAcmInit();
	
#if _WII || _IPHONE
	AoActAcmApplyScale(DMD_STFRL_DISP_SCALE_TEXT
					   , DMD_STFRL_DISP_SCALE_TEXT);
#endif
	
	AoActAcmApplyTrans(dm_stfrl_win_act_pos_tbl[3][0]
					   , dm_stfrl_win_act_pos_tbl[3][1]
					   , 0
					   );
	
	AoActSetTexture(AoTexGetTexList(&dm_stfrl_end_jp_tex));
	AoActUpdate(main_work->act[ACT_TEX_WIN_MSG], 0.f);
	
	AoActAcmPop();
	
	
	// ソート実行
	AoActSortExecute();

	// ソートバッファ描画
	AoActSortDraw();

	// ソートバッファ全解除
	AoActSortUnregAll();
	
	
	// 描画タスク生成
	amDrawMakeTask(dmStaffRollWinActTaskDraw, (u16)DMD_STFRL_DRAW_PRIO_WIN, (u32)0);
}



// ==========================================================================
// dmStaffRollWinActTaskDraw
/*!
	END画面のアクションの描画タスク
 */
// ==========================================================================
void dmStaffRollWinActTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(DMD_STFRL_DRAW_STATE_PRIO_WIN);
	amDrawEndScene();
}



// ==========================================================================
// dmStaffRollIsDataLoad
/*!
	データ読み込み完了チェック処理			◆表示テストが終り次第、要修正
 */
// ==========================================================================
s32 dmStaffRollIsDataLoad(DMS_STFRL_MAIN_WORK *main_work)
{
	// ここでフルバージョンか簡易バージョンかで分かれる
	if (dm_stfrl_is_full_staffroll) {
#if 0		
		// フォントデータ
		if (!amFsIsComplete(main_work->arc_list_font_amb)) {
			return 0;
		}
		
		// スクリーンキャプチャ
		if (!amFsIsComplete(main_work->arc_scr_amb_fs)) {
			return 0;
		}
		
		// END画面用データ
		if (!amFsIsComplete(main_work->arc_end_amb_fs)) {
			return 0;
		}
		
		// END画面用データ
		if (!amFsIsComplete(main_work->arc_end_jp_amb_fs)) {
			return 0;
		}
		
		// メニュー共通データ
		for (int i = 0; i < 2; i++) {
			if (!amFsIsComplete(main_work->arc_cmn_amb_fs[i])) {
				return 0;
			}
		}
#endif
		
		// スタッフリストYSDファイル
		if (!amFsIsComplete(main_work->staff_list_fs)) {
			return 0;
		}
		
	}
	
	else {
		// フォントデータ
		if (!amFsIsComplete(main_work->arc_list_font_amb_fs)) {
			return 0;
		}
		
		// スタッフリストYSDファイル
		if (!amFsIsComplete(main_work->staff_list_fs)) {
			return 0;
		}
	}
	
	
	return 1;
}



// ==========================================================================
// dmStaffRollIsDataLoadForGame
/*!
	データ読み込み完了チェック処理(ゲーム専用)
 */
// ==========================================================================
#if 0
s32 dmStaffRollIsDataLoadForGame(void)
{
//	s32 result = 0;
	
	// フォントデータ
	if (!amFsIsComplete(dm_stfrl_fs_data_mgr_p->arc_list_font_amb_fs)) {
		return 0;
	}
	
	// スクリーンキャプチャ
	if (!amFsIsComplete(dm_stfrl_fs_data_mgr_p->arc_scr_amb_fs)) {
		return 0;
	}
	
	// END画面用データ
	if (!amFsIsComplete(dm_stfrl_fs_data_mgr_p->arc_end_amb_fs)) {
		return 0;
	}
	
	// END画面用データ
	if (!amFsIsComplete(dm_stfrl_fs_data_mgr_p->arc_end_jp_amb_fs)) {
		return 0;
	}
	
	// メニュー共通データ
	for (int i = 0; i < 2; i++) {
		if (!amFsIsComplete(dm_stfrl_fs_data_mgr_p->arc_cmn_amb_fs[i])) {
			return 0;
		}
	}
	
	// スタッフリストYSDファイル
	if (!amFsIsComplete(dm_stfrl_fs_data_mgr_p->staff_list_fs)) {
		return 0;
	}
	
	return 1;
}
#endif


// ==========================================================================
// dmStaffRollDataClearRequestFull
/*!
	データクリアリクエスト処理(ゲーム専用)
 */
// ==========================================================================
void dmStaffRollDataClearRequestFull(DMS_STFRL_MAIN_WORK *main_work)
{
	// ファイル取得
	// フォントデータ
	main_work->arc_data.arc_font_amb
		= main_work->arc_list_font_amb;
#if 0
	main_work->arc_list_font_amb_fs->buf = NULL;
	amFsClearRequest(main_work->arc_list_font_amb_fs);
	main_work->arc_list_font_amb_fs = NULL;
#endif
	
	// スクリーンキャプチャ
	main_work->arc_data.arc_scr_amb = main_work->arc_scr_amb_fs;
	
	// END画面用データ
	main_work->arc_data.arc_end_amb = main_work->arc_end_amb_fs;
	main_work->arc_data.arc_end_jp_amb = main_work->arc_end_jp_amb_fs;
	
	// メニュー共通データ
	for (u32 i = 0; i < 2; i++) {
		main_work->arc_data.arc_cmn_amb[i] = main_work->arc_cmn_amb_fs[i];
	}
	
	// スタッフ名簿リスト
	main_work->arc_data.stf_list_ysd = main_work->staff_list_fs->buf;
	main_work->staff_list_fs->buf = NULL;
	amFsClearRequest(main_work->staff_list_fs);
	main_work->staff_list_fs = NULL;
	
}



// ==========================================================================
// dmStaffRollDataClearRequestEasy
/*!
	データクリアリクエスト処理(ゲーム専用)
 */
// ==========================================================================
void dmStaffRollDataClearRequestEasy(DMS_STFRL_MAIN_WORK *main_work)
{
	// ファイル取得
	// フォントデータ
	main_work->arc_data.arc_font_amb
		= main_work->arc_list_font_amb_fs->buf;
	main_work->arc_list_font_amb_fs->buf = NULL;
	amFsClearRequest(main_work->arc_list_font_amb_fs);
	main_work->arc_list_font_amb_fs = NULL;
	
	// スタッフ名簿リスト
	main_work->arc_data.stf_list_ysd = main_work->staff_list_fs->buf;
	main_work->staff_list_fs->buf = NULL;
	amFsClearRequest(main_work->staff_list_fs);
	main_work->staff_list_fs = NULL;
}



// ==========================================================================
// dmStaffRollDataBuildEasy
/*!
	簡易データ構築処理
 */
// ==========================================================================
void dmStaffRollDataBuildEasy(DMS_STFRL_MAIN_WORK *main_work)
{
	// ファイル取得
	// フォントデータ
	dm_stfrl_font_amb = main_work->arc_data.arc_font_amb;
	
	// アドレス変換
	amConvertAddress(dm_stfrl_font_amb);
	
	// テクスチャ構築
	AoTexBuild(&dm_stfrl_font_tex, dm_stfrl_font_amb);
	AoTexLoad(&dm_stfrl_font_tex);
	
	dm_stfrl_data_mgr_p = &main_work->arc_data;
}



// ==========================================================================
// dmStaffRollDataClearRequestForGame
/*!
	データクリアリクエスト処理(ゲーム専用)
 */
// ==========================================================================
#if 0
s32 dmStaffRollDataClearRequestForGame(void)
{
	// ここでデータロード済みなのでファイルリクエストクリア
	// フォントデータ
	main_work->arc_data.arc_font_amb
		= main_work->arc_list_font_amb_fs->buf;
	main_work->arc_list_font_amb_fs->buf = NULL;
	amFsClearRequest(main_work->arc_list_font_amb_fs);
	main_work->arc_list_font_amb_fs = NULL;
	
	// スクリーンキャプチャ
	main_work->arc_data.arc_scr_amb = main_work->arc_scr_amb_fs->buf;
	main_work->arc_scr_amb_fs->buf = NULL;
	amFsClearRequest(main_work->arc_scr_amb_fs);
	main_work->arc_scr_amb_fs = NULL;
	
	// END画面用データ
	main_work->arc_data.arc_end_amb = main_work->arc_end_amb_fs->buf;
	main_work->arc_end_amb_fs->buf = NULL;
	amFsClearRequest(main_work->arc_end_amb_fs);
	main_work->arc_end_amb_fs = NULL;
	
	main_work->arc_data.arc_end_jp_amb = main_work->arc_end_jp_amb_fs->buf;
	main_work->arc_end_jp_amb_fs->buf = NULL;
	amFsClearRequest(main_work->arc_end_jp_amb_fs);
	main_work->arc_end_jp_amb_fs = NULL;
	
	// メニュー共通データ
	for (u32 i = 0; i < 2; i++) {
		main_work->arc_data.arc_cmn_amb[i] = main_work->arc_cmn_amb_fs[i]->buf;
		main_work->arc_cmn_amb_fs[i]->buf = NULL;
		amFsClearRequest(main_work->arc_cmn_amb_fs[i]);
		main_work->arc_cmn_amb_fs[i] = NULL;
	}
	
	// スタッフ名簿リスト
	main_work->arc_data.stf_list_ysd = main_work->staff_list_fs->buf;
	main_work->staff_list_fs->buf = NULL;
	amFsClearRequest(main_work->staff_list_fs);
	main_work->staff_list_fs = NULL;
	
}
#endif



// ==========================================================================
// dmStaffRollIsTexLoad
/*!
	テクスチャ構築完了チェック処理
 */
// ==========================================================================
s32 dmStaffRollIsTexLoad(void)
{
	if (dm_stfrl_is_full_staffroll) {
		if (!AoTexIsLoaded(&dm_stfrl_font_tex)) {
			// フラグ扱いでON
			return 0;
		}
		
		if (!AoTexIsLoaded(&dm_stfrl_scr_tex)) {
			// フラグ扱いでON
			return 0;
		}
		
		if (!AoTexIsLoaded(&dm_stfrl_end_tex)) {
			// フラグ扱いでON
			return 0;
		}
		
		if (!AoTexIsLoaded(&dm_stfrl_end_jp_tex)) {
			// フラグ扱いでON
			return 0;
		}
		
		for (int i = 0; i < 2; i++) {
			if (!AoTexIsLoaded(&dm_stfrl_cmn_tex[i])) {
				// フラグ扱いでON
				return 0;
			}
		}
		
		
		// 3Dモデルのロード関連を以下に記述
		
	}
	
	else {
		if (!AoTexIsLoaded(&dm_stfrl_font_tex)) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	return 1;
}


// ==========================================================================
// dmStaffRollIsTexRelease
/*!
	テクスチャ解放完了チェック処理
 */
// ==========================================================================
s32 dmStaffRollIsTexRelease(void)
{
	if (dm_stfrl_is_full_staffroll) {
		if (!AoTexIsReleased(&dm_stfrl_font_tex)) {
			// フラグ扱いでON
			return 0;
		}
		
		if (!AoTexIsReleased(&dm_stfrl_scr_tex)) {
			// フラグ扱いでON
			return 0;
		}
		
		if (!AoTexIsReleased(&dm_stfrl_end_tex)) {
			// フラグ扱いでON
			return 0;
		}
		
		if (!AoTexIsReleased(&dm_stfrl_end_jp_tex)) {
			// フラグ扱いでON
			return 0;
		}
		
		for (int i = 0; i < 2; i++) {
			if (!AoTexIsReleased(&dm_stfrl_cmn_tex[i])) {
				// フラグ扱いでON
				return 0;
			}
		}
		
		
		// 3Dモデルの解放関連を以下に記述
		
	}
	
	else {
		if (!AoTexIsReleased(&dm_stfrl_font_tex)) {
			// フラグ扱いでON
			return 0;
		}
	}
	
	return 1;
}



// ==========================================================================
// dmStaffRollSetFadePageInfoEfctData
/*!
	スタッフロール再生中でのフェード演出設定処理
 */
// ==========================================================================
void dmStaffRollSetFadePageInfoEfctData(DMS_STFRL_MAIN_WORK *main_work)
{
	// 表示時間取得し、設定
	main_work->disp_page_time = AoYsdFileGetPageTime(dm_stfrl_data_mgr_p->stf_list_ysd
													, main_work->cur_disp_list_page
													);
#if _IPHONE
#if defined(AMD_DEBUG)
	if (0.8 <= _am_iphone_accel_data.sensor.z) {
		//液晶を下方向に向けると高速化
		main_work->disp_page_time = DMD_STFRL_EFCT_FADE_WAIT_TIME + 10;;
	}
#endif //defined(AMD_DEBUG)
#endif //_IPHONE
	
	main_work->disp_page_time = (u32)(main_work->disp_page_time - DMD_STFRL_EFCT_FADE_WAIT_TIME - 2);
	
	// タイマーに演出時間を設定
	main_work->fade_timer = (float)main_work->disp_page_time;
	
	// 表示スクリーンイメージ番号取得
//	main_work->cur_disp_image = AoYsdFileGetPageShowImageNo(dm_stfrl_data_mgr_p->stf_list_ysd
//															, main_work->cur_disp_list_page
//															);
	
	// イメージ表示フラグONならばフラグ設定
	if (AoYsdFileIsPageShowImage(dm_stfrl_data_mgr_p->stf_list_ysd
								 , main_work->cur_disp_list_page
								 )) {
		main_work->flag |= DMD_STFRL_FLAG_IMAGE_FADE_START;
		
		// 表示スクリーンイメージ番号取得
		main_work->cur_disp_image = AoYsdFileGetPageShowImageNo(dm_stfrl_data_mgr_p->stf_list_ysd
																, main_work->cur_disp_list_page
																);
		
		// ここでリングエフェクト開始フラグON
		for (int i = 0; i < 3; i++) {
			if (main_work->ring_work[i]) {
				main_work->ring_work[i]->flag |= DMD_STFRL_RING_FLAG_SPLASH_EFCT_START;
			}
		}
	}
	
	// イメージ非表示フラグONならば非表示フラグ設定
	if (AoYsdFileIsPageHideImage(dm_stfrl_data_mgr_p->stf_list_ysd
								 , main_work->cur_disp_list_page
								 )) {
		main_work->flag |= DMD_STFRL_FLAG_IMAGE_FADE_END;
	}
	
}



// ==========================================================================
// dmStaffRollSetEfctChngAlphaListData
/*!
	スタッフロール再生中での表示透過度の演出切り替え処理
 */
// ==========================================================================
void dmStaffRollSetEfctChngAlphaListData(DMS_STFRL_MAIN_WORK *main_work)
{
	int tmp_list_alpha = 0;
	int tmp_scr_alpha = 0;
	
	// 計算用変数に各透過度を格納
	tmp_list_alpha = (int)main_work->cur_page_list_alpha_data;
	tmp_scr_alpha = (int)main_work->cur_page_scr_alpha_data;
	
	
	if (main_work->fade_timer >= (float)(main_work->disp_page_time - DMD_STFRL_EFCT_FADE_IN_TIME)) {
		tmp_list_alpha += 8;
		
		// イメージのフェード開始ならば
		if (main_work->flag & DMD_STFRL_FLAG_IMAGE_FADE_START) {
			tmp_scr_alpha += 8;
			
			// 最大透過度を超えたら
			if (tmp_scr_alpha >= 255) {
				// 最大透過度に設定し、イメージの透過度設定フラグをOFF
				tmp_scr_alpha = 255;
				
				main_work->flag &= ~DMD_STFRL_FLAG_IMAGE_FADE_START;
			}
		}
	}
	
	
	else if (main_work->fade_timer <= DMD_STFRL_EFCT_FADE_OUT_TIME) {
		tmp_list_alpha -= 8;
		
		// イメージのフェード終了ならば
		if (main_work->flag & DMD_STFRL_FLAG_IMAGE_FADE_END) {
			tmp_scr_alpha -= 8;
			
			
			if (tmp_scr_alpha <= 0) {
				tmp_scr_alpha = 0;
				
				main_work->flag &= ~DMD_STFRL_FLAG_IMAGE_FADE_END;
			}
		}
	}
	
	
	if (tmp_list_alpha >= 255) {
		tmp_list_alpha = 255;
	}
	
	if (tmp_list_alpha <= 0) {
		tmp_list_alpha = 0;
	}
	
	
	// 計算後の透過度を設定
	main_work->cur_page_list_alpha_data = (u32)tmp_list_alpha;
	main_work->cur_page_scr_alpha_data = (u32)tmp_scr_alpha;
	
}



// ==========================================================================
// dmStaffRollSetWinOpenEfct
/*!
	ウインドウ入り演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmStaffRollSetWinOpenEfct(DMS_STFRL_MAIN_WORK *main_work)
{
	if (main_work->win_timer > DMD_STFRL_WIN_EFCT_TIME) {
		// ウインドウ演出終了
		main_work->flag |= DMD_STFRL_FLAG_WIN_EFCT_END;

		main_work->win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->win_size_rate[i] = DMD_STFRL_WIN_DEF_RATE;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer++;
	}
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_STFRL_WIN_EFCT_TIME;
		}
		else {
			main_work->win_size_rate[i] = 1.0f;
		}

		if (main_work->win_size_rate[i] > 1.0f) {
			main_work->win_size_rate[i] = 1.0f;
		}
	}

	

}



// ==========================================================================
// dmStaffRollSetWinCloseEfct
/*!
	ウインドウ閉め演出時のウインドウサイズ設定処理
 */
// ==========================================================================
void dmStaffRollSetWinCloseEfct(DMS_STFRL_MAIN_WORK *main_work)
{
	
	// 掃け演出分のサイズ更新
	for (u32 i = 0; i < 2; i++) {
		if (main_work->win_timer) {
			main_work->win_size_rate[i] = main_work->win_timer / DMD_STFRL_WIN_EFCT_TIME;
		}
		else {
			main_work->win_size_rate[i] = 0.0f;
		}
	}

	if (main_work->win_timer < 0.0f) {
		// ウインドウ演出終了
		main_work->flag |= DMD_STFRL_FLAG_WIN_EFCT_END;

		main_work->win_timer = 0.0f;

		for (u32 i = 0; i < 2; i++) {
			main_work->win_size_rate[i] = 0.0f;
		}
	}
	else {
		// タイマー更新(但しフレームレートが可変になるように修正すること)	◆
		main_work->win_timer--;
	}
}







// ==========================================================================
// dmStaffRollSetObjSystemData
/*!
	スタッフロール用のOBJシステム設定処理
 */
// ==========================================================================
void dmStaffRollSetObjSystemData(DMS_STFRL_MAIN_WORK *main_work)
{
	UNREFERENCED_PARAMETER(main_work);
	
	/* ライト設定 */
#if !GMD_DEBUG_LIGTH_MULTI
#if _WII
	g_obj.def_user_light_flag |= OBD_LIGHT_USE_FLAG_7/*スペキュラ*/;
#endif	// #if _WII

#else
#if !_WII
	g_obj.load_drawflag &= ~NND_DRAWOBJ_FRAGPARALIGHT1;
	g_obj.drawflag		&= ~NND_DRAWOBJ_FRAGPARALIGHT1;
	g_obj.load_drawflag |= (NND_DRAWOBJ_FRAGPARALIGHT3);
	g_obj.drawflag		|= (NND_DRAWOBJ_FRAGPARALIGHT3);
#endif

#if !_WII
	g_obj.def_user_light_flag = OBD_LIGHT_USE_FLAG_0 | OBD_LIGHT_USE_FLAG_1 | OBD_LIGHT_USE_FLAG_2;
#else
	g_obj.def_user_light_flag = OBD_LIGHT_USE_FLAG_0 | OBD_LIGHT_USE_FLAG_1 | OBD_LIGHT_USE_FLAG_2 |
								OBD_LIGHT_USE_FLAG_7/*スペキュラ*/;
#endif	// #if !_WII
#endif	// #if !GMD_DEBUG_LIGTH_MULTI


	// エフェクトシステム起動
	ObjDrawESEffectSystemInit(GMD_TASK_PAUSELEVEL_DEF,
							  GMD_TASK_PRIO_EFFECT_SERVER,
							  GMD_TASK_GROUP_EFFECT_SERVER);

	//描画順序設定
	
	ObjDrawSetNNCommandStateTbl( 0, OBD_DRAW_CMD_STATE_PRE_MAPFAR, FALSE );
	ObjDrawSetNNCommandStateTbl( 1, OBD_DRAW_CMD_STATE_MAPFAR, FALSE );
	ObjDrawSetNNCommandStateTbl( 2, OBD_DRAW_CMD_STATE_POST_MAPFAR, TRUE );
	ObjDrawSetNNCommandStateTbl( 3, OBD_DRAW_CMD_STATE_WATER_BACK, TRUE );
	ObjDrawSetNNCommandStateTbl( 4, OBD_DRAW_CMD_STATE_MAPMID, TRUE );			// 中景
	ObjDrawSetNNCommandStateTbl( 5, OBD_DRAW_CMD_STATE_WATER_MAPMID, TRUE );	// 中景前水
//	ObjDrawSetNNCommandStateTbl( 6, OBD_DRAW_CMD_STATE_3DNN, TRUE );
	ObjDrawSetNNCommandStateTbl( 6, OBD_DRAW_CMD_STATE_PRE_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl( 7, OBD_DRAW_CMD_STATE_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl( 8, OBD_DRAW_CMD_STATE_POST_WATER, TRUE );
	ObjDrawSetNNCommandStateTbl( 9, OBD_DRAW_CMD_STATE_NEAR_MAP, TRUE );
	ObjDrawSetNNCommandStateTbl(10, OBD_DRAW_CMD_STATE_3DFIX, TRUE );	// 3D描画最前面 FIX系
	ObjDrawSetNNCommandStateTbl(11, OBD_DRAW_CMD_STATE_2DAMA, TRUE );	// 2D描画 必ず一番最後に
	ObjDrawSetNNCommandStateTbl(12, OBD_DRAW_CMD_STATE_3DNN, TRUE );
	
	// aliceのEndSceneでソートが行われなくなったので、必ず最後にソート描画する事
	
	
	// タイムオーバーからの復帰ゲームはタイムをクリア
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_TIMEOVER) {
		g_gm_main_system.game_time = 0;
	}
	
	// ゲーム開始時 不要フラグクリア
	g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_START_CLEAR_MASK;
	
	g_obj.flag = OBD_OBJ_CAMERA | OBD_OBJ_COL_DIFF | OBD_OBJ_RECT | OBD_OBJ_RECT_NOUSE_DRAWSCALE/* | OBD_OBJ_HS_FUNC_STOP*/;
	
	g_obj.ppPre			= NULL;						// システム前処理
	g_obj.ppPost		= NULL;						// システム後処理
	//g_obj.ppDrawSort	= GmObjDrawSort;			// 描画前オブジェクトソート
	g_obj.ppCollision	= NULL;						// あたり処理
	g_obj.ppObjPre		= GmObjObjPreFunc;			// オブジェクト共通前処理
	g_obj.ppObjPost		= GmObjObjPostFunc;			// オブジェクト共通後処理
	g_obj.ppRegRecAuto	= NULL;						// 矩形自動登録処理
	
	// 描画スケール設定
	g_obj.draw_scale.x = g_obj.draw_scale.y = g_obj.draw_scale.z = GMD_OBJ_DRAW_SCALE_FX;
	// 逆数保存
	g_obj.inv_draw_scale.x = g_obj.inv_draw_scale.y = g_obj.inv_draw_scale.z = FX_Div(FX32_ONE, g_obj.draw_scale.x);
	// 画面奥行き度
	g_obj.depth = 0x0080;
	
	/* ライト初期化 */
	dmStaffRollInitLight();
	
	// カメラ初期化
	dmStaffRollCameraInit();
	
	// ゲームモード設定		仮
//	g_gs_main_sys_info.game_mode = GSD_GAME_MODE_STORY;
	
	// 共通エフェクト初期化
	GmEfctCmnBuildDataInit();
}



// ==========================================================================
// dmStaffRollSetupEndModel
/*!
	スタッフロール用のOBJデータ初期化設定処理
 */
// ==========================================================================
void dmStaffRollSetupEndModel(DMS_STFRL_MAIN_WORK *main_work)
{
	OBS_OBJECT_WORK	*obj_work = NULL;
	
	// END画面に遷移する際にリングのOBJをクリアしておく(使用しないため)
	for (int i = 0; i < 3; i++) {
		if (main_work->ring_work[i]) {
			obj_work = (OBS_OBJECT_WORK	*)main_work->ring_work[i];
			obj_work->ppOut = NULL;
			obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		}
	}
	
	// ボス生成(END画面では常時生成)
	dmStaffRollSetBossObj(main_work);
	
	// カオスエメラルドコンプリート時はソニックを生成
	if (main_work->is_eme_comp) {
		// ソニック生成
		main_work->sonic_work = DmStfrlMdlCtrlSetSonicObj();
	}
}



// ==========================================================================
// dmStaffRollNodispEndModel
/*!
	スタッフロール用のOBJデータ非表示設定処理
 */
// ==========================================================================
void dmStaffRollNodispEndModel(DMS_STFRL_MAIN_WORK *main_work)
{
	OBS_OBJECT_WORK	*obj_work = NULL;
	
	// エッグマンモデル非表示設定
	if (main_work->body_work) {
		obj_work = (OBS_OBJECT_WORK	*)main_work->body_work;
		obj_work->ppOut = NULL;
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
	}
	
	if (main_work->egg_work) {
		obj_work = (OBS_OBJECT_WORK	*)main_work->egg_work;
		obj_work->ppOut = NULL;
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
	}
	
	for (int i = 0; i < 3; i++) {
		if (main_work->ring_work[i]) {
			obj_work = (OBS_OBJECT_WORK	*)main_work->ring_work[i];
			obj_work->ppOut = NULL;
			obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
		}
	}
	
	// ソニックモデル非表示設定
	if (main_work->sonic_work) {
		obj_work = (OBS_OBJECT_WORK	*)main_work->sonic_work;
	    obj_work->ppOut = NULL;
		
		obj_work->flag |= OBD_OBJECT_TASKCLEAR_REQUEST;
	}
}



// ==========================================================================
// dmStaffRollSetBossObj
/*!
	スタッフロール用のボスOBJ設定処理
 */
// ==========================================================================
void dmStaffRollSetBossObj(DMS_STFRL_MAIN_WORK *main_work)
{
	// ボス本体OBJ生成
	main_work->body_work = DmStfrlMdlCtrlSetBodyObj();
//	dmStaffRollSetBodyObj(main_work);
	
	// ボスエッグマンOBJ生成
	main_work->egg_work = DmStfrlMdlCtrlSetEggObj((OBS_OBJECT_WORK *)main_work->body_work);
//	dmStaffRollSetEggObj(main_work);
	
	// ボスの処理切り分け設定
	if (main_work->is_eme_comp) {
		// 専用描画処理設定
		main_work->body_work->flag |= DMD_STFRL_BODY_FLAG_COMPLETE_EFCT;
	}
}



// ==========================================================================
// dmStaffRollIsHBMDraw
/*!
	HBM表示チェックでの必要設定処理
 */
// ==========================================================================
#if _WII
void dmStaffRollIsHBMDraw(DMS_STFRL_MAIN_WORK *main_work)
{
	
	if (main_work->flag & DMD_STFRL_FLAG_HBM_BGM_STOP) {
		if (main_work->bgm_scb) {
			GsSoundPauseBgm(main_work->bgm_scb, 0);
		}
		main_work->flag &= ~DMD_STFRL_FLAG_HBM_BGM_STOP;
	}
	
	if (amWiiIsDrawHBM()) {
		if (!(main_work->flag & DMD_STFRL_FLAG_ALL_PROC_STOP)) {
			main_work->flag |= DMD_STFRL_FLAG_HBM_BGM_STOP;
		}
		
		main_work->flag |= DMD_STFRL_FLAG_ALL_PROC_STOP;
	}
	else {
		if (main_work->flag & DMD_STFRL_FLAG_ALL_PROC_STOP) {
			main_work->flag |= DMD_STFRL_FLAG_HBM_BGM_REPLAY;
		}
		
		main_work->flag &= ~DMD_STFRL_FLAG_ALL_PROC_STOP;
	}
	
	if (main_work->flag & DMD_STFRL_FLAG_HBM_BGM_REPLAY) {
		if (main_work->bgm_scb) {
			GsSoundResumeBgm(main_work->bgm_scb, 0);
		}
		main_work->flag &= ~DMD_STFRL_FLAG_HBM_BGM_REPLAY;
	}
	
}
#endif


// ==========================================================================
// ライト設定
// ==========================================================================
// ==========================================================================
// dmStaffRollInitLight
/*!
 *	ライト初期化
 */
// ==========================================================================
void dmStaffRollInitLight(void)
{
	/* ライト */
#if !GMD_DEBUG_LIGTH_MULTI
	NNS_RGBA	light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	NNS_VECTOR	light_vec;

	// 標準使用ライト
	// アンビエントカラー
	g_obj.ambient_color.r = 0.8f;
	g_obj.ambient_color.g = 0.8f;
	g_obj.ambient_color.b = 0.8f;

	// パラレルライト
	light_vec.x = -1.0f;
	light_vec.y = -1.0f;
	light_vec.z = -1.0f;
	
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_0, &light_col, 1.f, &light_vec);

	// 標準ライト設定を保存
	g_gm_main_system.def_light_vec = light_vec;
	g_gm_main_system.def_light_col = light_col;


	// プレイヤー用パラレルライト
	ObjDrawSetParallelLight(NNE_LIGHT_6, &light_col, 1.f, &light_vec);


#if _WII
	// Wii用スペキュラーGCライト
	light_col.r = 1.f;
	light_col.g = 1.f;
	light_col.b = 1.f;
	// light_vec はパラレルライトを調整
	light_vec.x /= 2.f;
	//light_vec.y = -1.f;
	light_vec.z = 0.f;
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetSpecularGCLight(NNE_LIGHT_7, &light_col, &light_vec);

	// Wii用トゥーンライト
	g_obj.toon_light_vec = light_vec;
#endif

#else // #if GMD_DEBUG_LIGTH_MULTI

	// 平行光源テスト
	NNS_RGBA	light_col = {
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	NNS_VECTOR	light_vec;

	// アンビエントカラー
	g_obj.ambient_color.r = 0.2f;
	g_obj.ambient_color.g = 0.2f;
	g_obj.ambient_color.b = 0.2f;

	// パラレルライト1
	light_vec.x = -1.0f;
	light_vec.y = 1.0f;
	light_vec.z = -1.0f;
	light_col.r = 1.f;
	light_col.g = 0.f;
	light_col.b = 0.f;
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_0, &light_col, 1.f, &light_vec);

#if _WII
	// Wii用トゥーンライト
	g_obj.toon_light_vec = light_vec;
#endif

	// パラレルライト2
	light_vec.x = 1.0f;
	light_vec.y = 0.0f;
	light_vec.z = -1.0f;
	light_col.r = 0.f;
	light_col.g = 1.f;
	light_col.b = 0.f;
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_1, &light_col, 1.f, &light_vec);

	// パラレルライト3
	light_vec.x = 0.0f;
	light_vec.y = -1.0f;
	light_vec.z = -1.0f;
	light_col.r = 0.f;
	light_col.g = 0.f;
	light_col.b = 1.f;
	nnNormalizeVector(&light_vec, &light_vec);
	ObjDrawSetParallelLight(NNE_LIGHT_2, &light_col, 1.f, &light_vec);

#endif // #if GMD_DEBUG_LIGTH_MULTI
}



// ==========================================================================
// dmStaffRollCameraInit
/*!
 *	メインカメラ初期化
 */
// ==========================================================================
void dmStaffRollCameraInit(void)
{
	OBS_CAMERA	*camera;
	// 仮
	NNS_VECTOR	cam_pos = {0.f, 0.f, 50.f};

	// カメラ初期化
	ObjCameraInit(GME_CAMERA_NO_MAIN, &cam_pos, GMD_TASK_GROUP_OBJSYS, GMD_TASK_PAUSE_LEVEL_CAMERA, GMD_TASK_PRIO_CAMERA);
	ObjCamera3dInit(GME_CAMERA_NO_MAIN);
	g_obj.glb_camera_id = GME_CAMERA_NO_MAIN;
	g_obj.glb_camera_type = NNE_PROJECTION_TYPE_ORTHO;	// 正射影
	GmCameraDelayReset();
	GmCameraAllowReset();
	ObjCameraSetUserFunc(GME_CAMERA_NO_MAIN, dmStaffRollCameraFunc);	// ユーザー処理

	camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
	camera->scale = GMD_CAMERA_SCALE;
	camera->ofst.z = 1000.0f;
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// dmStaffRollCameraFunc
/*!
 *	メインカメラ
 *	@param	obj_camera	[in]	オブジェクトカメラ
 */
// ==========================================================================
void dmStaffRollCameraFunc(OBS_CAMERA *obj_camera)
{
    NNS_VECTOR pos;
	
	pos.x = FXM_FX32_TO_FLOAT(0 * FX32_ONE);
    pos.y = FXM_FX32_TO_FLOAT(0 * FX32_ONE);
    pos.z = FXM_FX32_TO_FLOAT(100 * FX32_ONE);
	
    // 目的値を記録
    obj_camera->work.x = pos.x;
    obj_camera->work.y = pos.y;
    obj_camera->work.z = pos.z;

    obj_camera->prev_pos.x = obj_camera->pos.x;
    obj_camera->prev_pos.y = obj_camera->pos.y;
    obj_camera->prev_pos.z = obj_camera->pos.z;

	
	obj_camera->pos.x = 0.0f;
	obj_camera->pos.y = 0.0f;
	obj_camera->pos.z = 50.0f;

	obj_camera->disp_pos.x = obj_camera->pos.x;
	obj_camera->disp_pos.y = obj_camera->pos.y;
	obj_camera->disp_pos.z = obj_camera->pos.z;

	obj_camera->target_pos = obj_camera->disp_pos;
	obj_camera->target_pos.z -= 50.0f;

	
	// オブジェクトカメラ設定
	ObjObjectCameraSet(FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)),
						FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
						FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)));
	
	// クリッピングカメラ設定
	GmCameraSetClipCamera(obj_camera);
}



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
