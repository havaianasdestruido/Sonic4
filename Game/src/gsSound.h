// =======================================================================
/*!
  @file	gsSound.h
  @brief プラットフォーム共通サウンドモジュール

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gsSound.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GS_SOUND_H_
#define GS_SOUND_H_

/*------ Include Files -------------------------------------------------*/
#if _WII
#include "akRvlSound.h"
#endif /* _WII */

/*------ Macros --------------------------------------------------------*/
#if _WII

//! サラウンド出力の有無
//  サラウンドに対応する場合は1を指定してください
#define GSD_SND_WII_SUPPORT_SRROUND_OUTPUT			(0)

//! NW4Rが独占的に使用するAXボイス数
//  AX最大ボイス数(AX_MAX_VOICES)からこの数を差し引いた数がCRI Audio(Wii)で使用できる最大ボイス数となります。
//  NW4R側には使用可能なボイス数の上限は設けていません（i.e. AX最大ボイス数まで取得可能）。
#define GSD_SND_NW4R_AX_VOICE_EXCLUSIVE_USAGE_NUM	(32)

//! NW4Rサウンド用のサウンドヒープサイズ
#define GSD_SND_NW4R_SOUND_HEAP_SIZE				(1024 * 1024 + 1024 * 512)
#endif /* _WII */

#define GSD_SND_NO_USE_STREAM_BGM		(1 & _IPHONE)

//############ システムメイン情報関連 #########################################
/* フラグ（GSS_SND_SYS_MAIN_INFO::flag） */
#define GSD_SND_SYS_MAIN_FLAG_USE_SYSTEM_CNT_VOL	(1 << 0)	//!< READ このフラグが立っている時だけシステム制御ボリューム値が反映される（システムが操作）
#define GSD_SND_SYS_MAIN_FLAG_SYSTEM_CNT_VOL_FADING	(1 << 1)	//!< READ システム制御ボリュームによるフェード処理中フラグ
#define GSD_SND_SYS_MAIN_FLAG_ENTER_HBM_FADING		(1 << 2)	//!< READ HBMへ移行時のサウンド操作処理中
#define GSD_SND_SYS_MAIN_FLAG_LEAVE_HBM_FADING		(1 << 3)	//!< READ HBMから復帰時のサウンド操作処理中
// ユーザ操作フラグ
//

//! HBM入出時 フェード中フラグマスク
#define GSD_SND_SYS_MAIN_FLAG_HBM_FADING_MASK		(GSD_SND_SYS_MAIN_FLAG_ENTER_HBM_FADING | \
													 GSD_SND_SYS_MAIN_FLAG_LEAVE_HBM_FADING)

//! システムリセット時にクリアするフラグ
#define GSD_SND_SYS_MAIN_FLAG_RESET_CLEAR_MASK		(GSD_SND_SYS_MAIN_FLAG_SYSTEM_CNT_VOL_FADING)
													 



//############ SCB関連 ########################################################
/* フラグ (GSS_SND_SCB::flag) */
#define GSD_SND_SCB_FLAG_INITIALIZED		(1 << 0)	//!< Read 初期化済みフラグ（システムが操作）
#define GSD_SND_SCB_FLAG_IS_STOP			(1 << 1)	//!< Read 停止中フラグ（システムが操作）
#define GSD_SND_SCB_FLAG_IS_PAUSE			(1 << 2)	//!< Read 一時停止中フラグ（システムが操作）
// ユーザ操作フラグ
#define GSD_SND_SCB_FLAG_STOP_ON_HBM		(1 << 30)	//!< Read/Write このフラグが立っている場合は、HBM移行時に再生を停止する
#define GSD_SND_SCB_FLAG_MUTE_ON_USER_BGM	(1 << 31)	//!< Read/Write このフラグが立っている場合は、システムBGM/XMP有効のときにミュートする

/* 定義値 */
#define GSD_SND_SCB_MAX						(AME_CRIAUDIO_STRM_MAX)	//!< 使用可能なSCBの総数

// SCBポーズレベル定義
typedef enum
{
	GSD_SND_SCB_PAUSE_LEVEL_EACH	=	0x7fffffff,	//!< 原則、LEVEL_EACHよりも大きい値を使用しないこと
	GSD_SND_SCB_PAUSE_LEVEL_ALL_DEF	=	0x00000080,	//!< 全ポーズの基本値
	GSD_SND_SCB_PAUSE_LEVEL_GAME	=	GSD_SND_SCB_PAUSE_LEVEL_ALL_DEF,
	GSD_SND_SCB_PAUSE_LEVEL_HBM		=	0x00000040,	//!< ゲームポーズのレベルを優先するのでそれよりは小さい値に
	
	GSD_SND_SCB_PAUSE_LEVEL_NONE	=	0x00000000,	//!< ポーズレベル未設定
} GSE_SND_SCB_PAUSE_LEVEL;

//############ SEハンドル関連 #################################################
/* フラグ (GSS_SND_SE_HANDLE::flag) */
// システム操作フラグ
#define GSD_SND_SE_HANDLE_FLAG_INITIALIZED	(1 << 0)	//!< Read 初期化済みフラグ（システムが操作）
#define GSD_SND_SE_HANDLE_FLAG_PLAY_STARTED	(1 << 1)	//!< Read 再生開始済み（システムが操作）
#define GSD_SND_SE_HANDLE_FLAG_IS_STOP		(1 << 2)	//!< Read 停止中フラグ停止中フラグ（システムが操作）
#define GSD_SND_SE_HANDLE_FLAG_IS_PAUSE		(1 << 3)	//!< Read 一時停止中フラグ（システムが操作）
#define GSD_SND_SE_HANDLE_FLAG_FREE_ON_STOP	(1 << 4)	//!< Read 停止時にハンドルを自動解放する
// ユーザ操作フラグ
#define GSD_SND_SE_HANDLE_FLAG_STOP_ON_HBM	(1 << 30)	//!< Read/Write このフラグが立っている場合は、HBM移行時に再生を停止する
#define GSD_SND_SE_HANDLE_FLAG_NO_AUTOCLEAR	(1 << 31)	//!< Read/Write 自動クリア禁止フラグ（ユーザが操作）

/* 定義値 */
//! SEハンドルの最大数
#if _WII
#define GSD_SND_SE_HANDLE_MAX				(32)
#else
#define GSD_SND_SE_HANDLE_MAX				(32)
#endif /* _WII */

// SEポーズレベル定義
typedef enum {
	GSD_SND_SE_HANDLE_PAUSE_LEVEL_EACH		=	0x7fffffff,	//!< 原則、LEVEL_EACHよりも大きい値を使用しないこと
	GSD_SND_SE_HANDLE_PAUSE_LEVEL_ALL_DEF	=	0x00000080,			//!< 全ポーズの基本値
	GSD_SND_SE_HANDLE_PAUSE_LEVEL_GAME		=	GSD_SND_SE_HANDLE_PAUSE_LEVEL_ALL_DEF,
	GSD_SND_SE_HANDLE_PAUSE_LEVEL_HBM		=	0x00000040,			//!< ゲームポーズのレベルを優先するのでそれよりは小さい値に
	
	GSD_SND_SE_HANDLE_PAUSE_LEVEL_NONE		=	0x00000000,			//!< ポーズレベル未設定
} GSE_SND_SE_HANDLE_PAUSE_LEVEL;

//############ HBM関連 ########################################################
/* 定義値 */
#define GSD_SND_WII_HBM_CLEAR_EFFECT_FADEOUT_TIME_MSEC	(40)	//!< エフェクト削除フェードアウト時間（ミリ秒）
#define GSD_SND_WII_HBM_MAINOUT_VOL_FADEOUT_TIME_MSEC	(50)	//!< メイン出力ボリュームフェードアウト時間（ミリ秒）
#define GSD_SND_WII_HBM_MAINOUT_VOL_FADEIN_TIME_MSEC	(50)	//!< メイン出力ボリュームフェードイン時間（ミリ秒）


/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
//! サウンドタイプ
typedef enum
{
	GSE_SND_TYPE_BGM	= 0,
	GSE_SND_TYPE_SE,
	
	GSE_SND_TYPE_MAX
} GSE_SND_TYPE;

//! データタイプ
typedef enum
{
	GSE_SND_DATA_TYPE_CRIAUDIO	= 0,	//!< CRI Audio用データタイプ
	GSE_SND_DATA_TYPE_NW4R,				//!< NW4R用データタイプ
	
	GSE_SND_DATA_TYPE_MAX
} GSE_SND_DATA_TYPE;

//! フェードステート
typedef enum
{
	GSE_SND_FADE_STATE_NORMAL	= 0,
	GSE_SND_FADE_STATE_FADING_IN,
	GSE_SND_FADE_STATE_FADING_OUT_TO_STOP,
	GSE_SND_FADE_STATE_FADING_OUT_TO_PAUSE,
	
	GSE_SND_FADE_STATE_END,
	
	GSE_SND_FADE_STATE_MAX
} GSE_SND_FADE_STATE;


//! サウンドシステム情報
typedef struct tag_GSS_SND_SYS_MAIN_INFO
{
	Uint32	flag;
	
	//! システム制御ボリューム
	//  （HBM入出時などシステムの都合でボリュームを制御する必要があるときに使用。
	//    ユーザが設定できるな、常時反映されるボリューム「ではない」ので注意。）
	Float	system_cnt_vol;
#if _WII
	//! AUXバスリターンボリューム値 退避変数
	Uint16	aux_a_ret_vol_save;
	Uint16	aux_b_ret_vol_save;
	Uint16	aux_c_ret_vol_save;
	Uint16	reserved[1];
	
	Float	hbm_mainout_vol_save;	//!< メイン出力ボリューム退避変数
	
	void	(*proc_hbm)(void);
#endif /* _WII */
#if _IPHONE
	Sint32	suspend_wait_count; //!< サスペンドからの復帰待ちカウント
#endif // _IPHONE
} GSS_SND_SYS_MAIN_INFO;

//! サウンドコントロールパラメータ（CRI Audio用）
typedef struct tag_GSS_SND_CTRL_PARAM
{
	GSE_SND_FADE_STATE	fade_state;	//!< フェードステート
	Sint32	fade_frame_max;	//!< フェードにかけるフレーム数（CRI AUDIOでのみ使用）
	Sint32	fade_frame_cnt;	//!< フェード経過フレーム数（CRI_AUDIOでのみ使用）
	Float	fade_vol;		//!< フェードボリューム
	Float	fade_sub_vol;	//!< フェードサブボリューム（停止へのフェードアウト中にポーズをかけた場合に、
							//  そのときのフェードボリュームからフェードアウトを続けるために使用。）
							//  TODO: 次回実装では廃止（NW4Rのようにポーズ用と停止用のフェードボリューム値・フェード処理を独立させる予定）
	Float	volume;			//!< ボリューム
} GSS_SND_CTRL_PARAM;

//! サウンドコントロールブロック(SCB)
typedef struct tag_GSS_SND_SCB
{
	Uint32	flag;
	
	GSE_SND_DATA_TYPE	snd_data_type;
	
	GSS_SND_CTRL_PARAM	snd_ctrl_param;	//!< CRIAudioストリームサウンドでのみ使用
	Sint32	auply_no;	//!< AMS_CRIAUDIO_INTERFACE::auply[]のインデックス（CRI_AUDIOでのみ使用）
	
	GSE_SND_SCB_PAUSE_LEVEL	cur_pause_level;
	
#if _WII
	AKS_RVL_SND_HANDLE	*snd_handle;
#endif
#if _IPHONE
	struct {
		Uint32	sample;		//!< 前回のサンプル位置
		Uint32	counter;	//!< エラーカウンター
	} noplay_error_state;
#endif //_IPHONE
} GSS_SND_SCB;

//! データワーク
typedef struct tag_GSS_SND_DATA_WORK
{
	BOOL					is_active;
	
	GSE_SND_DATA_TYPE		data_type;
	
	AMD_CRIAUDIO_CSBTYPE	csb_type;	//!< CSBタイプ（CRI_AUDIOでのみ使用）
#if _WII
	AKS_RVL_SND_LOAD_WORK	load_work;
#endif /* _WII */
} GSS_SND_DATA_WORK;

//! SEハンドル
typedef struct tag_GSS_SND_SE_HANDLE
{
	Uint32	flag;
	CriAuPlayer	*au_player;
	GSS_SND_CTRL_PARAM	snd_ctrl_param;
	
	GSE_SND_SE_HANDLE_PAUSE_LEVEL	cur_pause_level;
} GSS_SND_SE_HANDLE;


/*------ External Declarations -----------------------------------------*/
// =======================================================================
// GsSoundBuildSeInit
/*!
  SEサウンドデータ構築 開始
  
  @param snd_data_work	[in]	GSサウンドデータワーク
  @param csb_type		[in]	CSBタイプ
  @param csb_file_path	[in]	CSBファイルパス
  @param prio			[in]	ロード待ちタスクのプライオリティ
  
  @note
  SEサウンドデータを構築します。
  GsSoundBuildSeUpdate()で完了待ちを行ってください。
 */
// =======================================================================
extern void GsSoundBuildSeInit(GSS_SND_DATA_WORK *snd_data_work, AMD_CRIAUDIO_CSBTYPE csb_type,
							   const char *csb_file_path, Sint32 prio);

// =======================================================================
// GsSoundBuildSeUpdate
/*!
  SEサウンドデータ構築 更新
  
  @param snd_data_work	[io]	GSサウンドデータワーク
  
  @retval TRUE	終了
  @retval FALSE	構築中
 */
// =======================================================================
extern BOOL GsSoundBuildSeUpdate(GSS_SND_DATA_WORK *snd_data_work);

// =======================================================================
// GsSoundBuildBgmInit
/*!
  BGMサウンドデータ構築 開始
  
  @param snd_data_work	[in]	GSサウンドデータワーク
  @param file_path		[in]	ファイルパス（CSBもしくはBRSAR）
  @param cpk_file_path	[in]	CPKファイルパス(WIIの場合はNULLを指定)
  @param prio			[in]	ロード待ちタスクのプライオリティ
  @param data_type		[in]	データタイプ(GSE_SND_DATA_TYPE_XXX)
  
  @note
  BGMデータに応じたdata_typeを指定してください。
  Wii以外のプラットフォームの場合にdata_type==GSE_SND_DATA_TYPE_NW4Rを指定すると
  アサートに失敗します。
  GsSoundBuildBgmUpdate()で完了待ちを行ってください。
 */
// =======================================================================
extern void GsSoundBuildBgmInit(GSS_SND_DATA_WORK *snd_data_work,
								const char *file_path, const char *cpk_file_path,
								Sint32 prio, GSE_SND_DATA_TYPE data_type);

// =======================================================================
// GsSoundBuildBgmUpdate
/*!
  BGMサウンドデータ構築 更新
  
  @param snd_data_work	[io]	GSサウンドデータワーク
  
  @retval TRUE	終了
  @retval FALSE	構築中
  
  @note
  snd_data_workは完了時に自動的に初期化されます。
 */
// =======================================================================
extern BOOL GsSoundBuildBgmUpdate(GSS_SND_DATA_WORK *snd_data_work);

// =======================================================================
// GsSoundFlushSe
/*!
  SEサウンドデータフラッシュ
  
  @param snd_data_work	[io]	GSサウンドデータワーク
  
  @note
  完了待ちを行う必要はありません。
 */
// =======================================================================
extern void GsSoundFlushSe(GSS_SND_DATA_WORK *snd_data_work);

// =======================================================================
// GsSoundFlushBgm
/*!
  BGMサウンドデータフラッシュ
  
  @note
  完了待ちを行う必要はありません。
  Wii版以外では何も行いません。
 */
// =======================================================================
extern void GsSoundFlushBgm(void);

// =======================================================================
// GsSoundInitDataWork
/*!
  サウンドデータワーククリア
  
  @param snd_data_work	[io]	GSサウンドデータワーク
  
  @note
  パラメータ等をクリアしてニュートラルな状態にします。
  GsSoundBuild***()後の状態が解放処理で参照されますので、
  未解放のデータと関連付けれられているワークに対しては使用しないでください。
 */
// =======================================================================
extern void GsSoundInitDataWork(GSS_SND_DATA_WORK *snd_data_work);

// =======================================================================
// GsSoundInit
/*!
  サウンドシステム初期化処理
  
  @note
  各種システムの初期化を行います。
  amCriAudioInit()は呼び出されません。
  amCriAudioInit()よりも後に呼び出してください。
 */
// =======================================================================
extern void GsSoundInit(void);

// =======================================================================
// GsSoundHalt
/*!
  サウンド停止
  
  @note
  サウンドの再生などを全て停止します。
  SCB,SEハンドルヒープのリセットは行われません。
  システム自体は終了しないことに注意してください。
 */
// =======================================================================
extern void GsSoundHalt(void);

// =======================================================================
// GsSoundReset
/*!
  サウンドシステムリセット
  
  @note
  各種システムのリセット等を行います。
  全てのユーザ側で確保した全てのSCB,SEハンドルの解放が終えた後に呼び出してください。
 */
// =======================================================================
extern void GsSoundReset(void);

// =======================================================================
// GsSoundExit
/*!
  サウンドシステム終了処理
  
  @note
  各種システムの終了などを行います。
  amCriAudioExit()は呼び出されません。
  amCriAudioExit()よりも前に呼び出してください。
 */
// =======================================================================
extern void GsSoundExit(void);

// =======================================================================
// GsSoundBegin
/*!
  サウンドメイン処理開始
  
  @param task_pause_level	[in]	サウンドタスク ポーズレベル
  @param task_prio			[in]	サウンドタスク プライオリティ
  @param task_group			[in]	サウンドタスク グループ
  
  @note
  サウンドのフレーム更新処理を開始します。
 */
// =======================================================================
extern void GsSoundBegin(Uint16 task_pause_level, Uint32 task_prio, Sint32 task_group);

// =======================================================================
// GsSoundEnd
/*!
  サウンドメイン処理終了
  
  @note
  サウンドのフレーム更新処理を終了します。
 */
// =======================================================================
extern void GsSoundEnd(void);

// =======================================================================
// GsSoundIsRunning
/*!
  サウンドメイン処理 実行中判定
  
  @retval TRUE	実行中
  @retval FALSE	実行中でない
 
  @note
  サウンドのフレーム更新処理が実行中か判定します。
 */
// =======================================================================
extern BOOL GsSoundIsRunning(void);

// =======================================================================
// GsSoundGetSysMainInfo
/*!
  サウンドシステムメイン情報取得
  
  @return サウンドシステムメイン情報
 */
// =======================================================================
extern GSS_SND_SYS_MAIN_INFO* GsSoundGetSysMainInfo(void);

// =======================================================================
// GsSoundPlaySe
/*!
  SE再生
  
  @param se_name	[in]	再生するSEのキュー名
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
extern void GsSoundPlaySe(const char *se_name, GSS_SND_SE_HANDLE *se_handle=NULL,
						  Sint32 fade_frame=0);

// =======================================================================
// GsSoundPlaySeById
/*!
  SE再生(ID指定)
  
  @param se_id		[in]	再生するSEのキューID
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
extern void GsSoundPlaySeById(Uint32 se_id, GSS_SND_SE_HANDLE *se_handle=NULL,
							  Sint32 fade_frame=0);

#if _IPHONE
// =======================================================================
// GsSoundPlaySeForce
/*!
  SE再生 強制
  
  @param se_name	[in]	再生するSEのキュー名
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
  
  @note 通常のSE再生と違い、現在の発音数による再生拒否が発生しない
 */
// =======================================================================
extern void GsSoundPlaySeForce(const char *se_name, GSS_SND_SE_HANDLE *se_handle=NULL,
				   Sint32 fade_frame=0);

// =======================================================================
// GsSoundPlaySeByIdForce
/*!
  SE再生(ID指定) 強制
  
  @param se_id		[in]	再生するSEのキューID
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
  
  @note 通常のSE再生と違い、現在の発音数による再生拒否が発生しない
 */
// =======================================================================
extern void GsSoundPlaySeByIdForce(Uint32 se_id, GSS_SND_SE_HANDLE *se_handle=NULL,
					   Sint32 fade_frame=0);
#endif // _IPHONE


// =======================================================================
// GsSoundStopSe
/*!
  SE停止
  
  @param fade_frame		[in]	フェードアウトに掛けるフレーム数（デフォルト:0）
  @param is_immediate	[in]	即時停止フラグ
  
  @note
  再生中の全てのSEを停止します。
  is_immediateをTRUEに指定すると、fade_frame==0だった場合、
  データにリリースタイムが設定されていてもそれを無視して即時に停止します。
 */
// =======================================================================
extern void GsSoundStopSe(Sint32 fade_frame=0, BOOL is_immediate=FALSE);

// =======================================================================
// GsSoundPauseSe
/*!
  SE一時停止
  
  @param pause_level	[in]	ポーズレベル
  @param fade_frame		[in]	フェードアウトに掛けるフレーム数（デフォルト:0）
  
  @note
  再生中の全てのSEを一時停止します。
 */
// =======================================================================
extern void GsSoundPauseSe(GSE_SND_SE_HANDLE_PAUSE_LEVEL pause_level,
						   Sint32 fade_frame=0);

// =======================================================================
// GsSoundResumeSe
/*!
  SE再開
  
  @param pause_level	[in]	ポーズレベル
  @param fade_frame		[in]	フェードインに掛けるフレーム数（デフォルト:0）
  
  @note
  一時停止中の全てのSEを再開します。
 */
// =======================================================================
extern void GsSoundResumeSe(GSE_SND_SE_HANDLE_PAUSE_LEVEL pause_level,
							Sint32 fade_frame=0);

// =======================================================================
// GsSoundStopSeHandle
/*!
  指定SEハンドルに関連付けられたSEを停止
  
  @param se_handle	[io]	SEハンドル
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
extern void GsSoundStopSeHandle(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame=0);

#if 0
/*
  REMINDER :
  複数のCUEが関連付けられているハンドルでは、最後にSetCueされたサウンド以外は
  ボリュームをコントロールすることができません。
  HBM移行時のサウンドミュート＆ポーズが行われた状態のときに上記のようなハンドルを
  GsSoundResumeSeHandle()で再開させると、最後にSetCueされたサウンド以外は
  ボリューム0が反映されずに鳴ってしまう恐れがあります。
  そのため、CriAuPlayerで再生されている全てのサウンドボリュームをコントロールする
  機能が実装されない限りは、下記の２つの関数を使用禁止とします。
 */
// =======================================================================
// GsSoundPauseSeHandle
/*!
  指定SEハンドルに関連付けられたSEを一時停止
  
  @param se_handle		[io]	SEハンドル
  @param fade_frame		[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  GSD_SND_SE_HANDLE_PAUSE_LEVEL_EACH のポーズレベルでポーズをかけます。
  GsSoundResumeSeHandle()で再開します。
 */
// =======================================================================
extern void GsSoundPauseSeHandle(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame=0);

// =======================================================================
// GsSoundResumeSeHandle
/*!
  指定SEハンドルに関連付けられたSEを再開
  
  @param se_handle		[io]	SEハンドル
  @param fade_frame		[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  既定ポーズレベル値以下の値のレベルでポーズされている場合のみ再開されます。
 */
// =======================================================================
extern void GsSoundResumeSeHandle(GSS_SND_SE_HANDLE *se_handle, Sint32 fade_frame=0);
#endif /* 0 */

// =======================================================================
// GsSoundIsSeStop
/*!
  SE停止中判定
  
  @param se_handle	[in]	SEハンドル（NULL不可）
  
  @retval TRUE	停止中
  @retval FALSE	停止中でない（再生中・ポーズ中...）
 */
// =======================================================================
inline BOOL GsSoundIsSeStop(const GSS_SND_SE_HANDLE *se_handle)
{
	MTM_ASSERT(se_handle);
	
	if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
		if (!(se_handle->flag & GSD_SND_SE_HANDLE_FLAG_IS_STOP)) {
			return FALSE;
		}
	}
	
	return TRUE;
}

// =======================================================================
// GsSoundIsSePause
/*!
  SE一時停止中判定
  
  @param se_handle	[in]	SEハンドル（NULL不可）
  
  @retval TRUE	一時停止中
  @retval FALSE	一時停止中ではない（再生中・停止中...）
  
  @note
  現状ではSEハンドルにフェード機能がないため、
  フェード中の判定をどうするかは考慮していません。
 */
// =======================================================================
inline BOOL GsSoundIsSePause(const GSS_SND_SE_HANDLE *se_handle)
{
	MTM_ASSERT(se_handle);
	
	if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_INITIALIZED) {
		if (se_handle->flag & GSD_SND_SE_HANDLE_FLAG_IS_PAUSE) {
			return TRUE;
		}
	}
	
	return FALSE;
}

// =======================================================================
// GsSoundPlayBgm
/*!
  BGM再生
  
  @param scb		[io]	SCB（NULL可）
  @param bgm_name	[in]	再生するBGMの名前
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
  
  @note
  scbにNULLを指定した場合、フェードインは行われません。
 */
// =======================================================================
extern void GsSoundPlayBgm(GSS_SND_SCB *scb, const char *bgm_name, Sint32 fade_frame=0);

// =======================================================================
// GsSoundStopBgm
/*!
  BGM停止
  
  @param scb		[io]	SCB（NULL不可）
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
 */
// =======================================================================
extern void GsSoundStopBgm(GSS_SND_SCB *scb, Sint32 fade_frame=0);

// =======================================================================
// GsSoundPauseBgm
/*!
  BGM一時停止
  
  @param scb		[io]	SCB（NULL不可）
  @param fade_frame	[in]	フェードアウトにかけるフレーム数（デフォルト:0）
  
  @note
  GSD_SND_SCB_PAUSE_LEVEL_EACH のポーズレベルでポーズをかけます。
 */
// =======================================================================
extern void GsSoundPauseBgm(GSS_SND_SCB *scb, Sint32 fade_frame=0);

// =======================================================================
// GsSoundResumeBgm
/*!
  BGM再開
  
  @param scb		[io]	SCB（NULL不可）
  @param fade_frame	[in]	フェードインにかけるフレーム数（デフォルト:0）
  
  @note
  既定ポーズレベル値以下のポーズレベルでポーズされている場合のみ再開されます。
 */
// =======================================================================
extern void GsSoundResumeBgm(GSS_SND_SCB *scb, Sint32 fade_frame=0);

// =======================================================================
// GsSoundIsBgmStop
/*!
  BGM停止中判定
  
  @param scb	[in]	SCB（NULL不可）
  
  @retval TRUE	停止中
  @retval FALSE	停止中ではない（再生中・ポーズ中・フェード中...）
 */
// =======================================================================
inline BOOL GsSoundIsBgmStop(const GSS_SND_SCB *scb)
{
	MTM_ASSERT(scb);
	
	if (scb->flag & GSD_SND_SCB_FLAG_INITIALIZED) {
		if (!(scb->flag & GSD_SND_SCB_FLAG_IS_STOP)) {
			return FALSE;
		}
	}
	
	return TRUE;
}

// =======================================================================
// GsSoundIsBgmPause
/*!
  BGM一時停止中判定
  
  @param scb	[in]	SCB（NULL不可）
  
  @retval TRUE	一時停止中（一時停止にフェードアウト中も含む）
  @retval FALSE	一時停止中ではない（再生中・停止中・フェードイン中...）
  
  @note
  一時停止へ移行する際のフェードアウト中は一時停止中と判定されます。
  一時停止から再開する際のフェードイン中は一時停止中とは判定されません。
  GSD_SND_SCB_PAUSE_LEVEL_EACH以外のポーズレベルで一時停止されている場合は
  一時停止中とは判定されません。
 */
// =======================================================================
inline BOOL GsSoundIsBgmPause(const GSS_SND_SCB *scb)
{
	MTM_ASSERT(scb);
	
	if (scb->flag & GSD_SND_SCB_FLAG_INITIALIZED) {
		if (scb->cur_pause_level == GSD_SND_SCB_PAUSE_LEVEL_EACH) {
			if (scb->flag & GSD_SND_SCB_FLAG_IS_PAUSE) {
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

// =======================================================================
// GsSoundSetVolumeFromMainSysInfo
/*!
  メインシステム情報からボリューム設定
  
  @note
  メインシステムで管理している情報(GSS_MAIN_SYS_INFO)に設定されている
  ボリューム値をgsSoundで管理しているボリュームに反映します。
 */
// =======================================================================
extern void GsSoundSetVolumeFromMainSysInfo(void);

// =======================================================================
// GsSoundGetVolume
/*!
  ボリューム取得
  
  @param snd_type	[in]	サウンドタイプ
  
  @return ボリューム値
  
  @note
  サウンドタイプ毎のボリュームを取得します。
 */
// =======================================================================
extern Float GsSoundGetVolume(GSE_SND_TYPE snd_type);

// =======================================================================
// GsSoundSetVolume
/*!
  ボリューム設定
  
  @param snd_type	[in]	サウンドタイプ
  @param vol		[in]	ボリューム値
  
  @note
  サウンドタイプ毎のボリュームを設定します。
 */
// =======================================================================
extern void GsSoundSetVolume(GSE_SND_TYPE snd_type, Float vol);

// =======================================================================
// GsSoundScbSetVolume
/*!
  SCBボリューム設定
  
  @param vol	[in]	ボリューム値
  
  @note
  SCB単位でのボリュームを設定します。
 */
// =======================================================================
extern void GsSoundScbSetVolume(GSS_SND_SCB *scb, Float vol);

// =======================================================================
// GsSoundScbSetSeqMute
/*!
  SCBのシーケンスミュート設定（ノートオン抑制）
  
  @param scb		[io]	SCB
  @param mute_on	[in]	ミュートフラグ
  
  @note
  シーケンスのノートオンを抑制することでミュートを行います。
  SCBのデータタイプが GSE_SND_DATA_TYPE_CRIAUDIOの場合は何もしません。
  ボリュームによるミュートだけではボイスを消費しますが、
  再生を継続しつつシーケンスを鳴らさない場合は、この関数を利用することで
  消費ボイスと負荷を軽減できます。
 */
// =======================================================================
extern void GsSoundScbSetSeqMute(GSS_SND_SCB *scb, BOOL mute_on);

// =======================================================================
// GsSoundAssignScb
/*!
  SCB割り当て
  
  @param snd_data_type	[in]	サウンドデータタイプ
  								GSE_SND_DATA_TYPE_CRIAUDIO → CRIAudioストリームサウンド
  								GSE_SND_DATA_TYPE_NW4R     → NW4Rシーケンスサウンド
  
  @return SCB
  
  @note
  GsSound管理下のSCBが割り当てられます。
  この関数で得られたSCBは毎フレーム更新処理が適用されます。
  使用後はGsSoundResignScb()で破棄してください。
 */
// =======================================================================
extern GSS_SND_SCB* GsSoundAssignScb(GSE_SND_DATA_TYPE snd_data_type);

// =======================================================================
// GsSoundResignScb
/*!
  SCB破棄
  
  @param scb	[io]	SCB（NULL不可）
  
  @note
  GsSound管理下のSCBを破棄します。
  この関数を呼ぶことで、指定のSCBへのフレーム毎更新処理が適用されなくなります。
 */
// =======================================================================
extern void GsSoundResignScb(GSS_SND_SCB *scb);

// =======================================================================
// GsSoundAllocSeHandle
/*!
  SEハンドルを確保
  
  @return SEハンドル
  
  @note
  SEハンドルの確保に失敗した場合GSD_SND_ERROR_SE_HANDLEを返します。
  使用後はGsSoundFreeSeHandle()で破棄してください。
 */
// =======================================================================
extern GSS_SND_SE_HANDLE* GsSoundAllocSeHandle(void);

// =======================================================================
// GsSoundFreeSeHandle
/*!
  SEハンドルを解放
  
  @param se_handle	[io]	SEハンドル
 */
// =======================================================================
extern void GsSoundFreeSeHandle(GSS_SND_SE_HANDLE *se_handle);

// =======================================================================
// GsSoundRequestFreeSeHandle
/*!
  SEハンドルを解放リクエスト（再生停止時に解放）
  
  @param se_handle	[io]	SEハンドル
  
  @note
  SEハンドルに関連付けられているサウンドの再生が停止した時点でハンドルを解放します。
  再生開始していないハンドルに対しても有効です。
  解放リクエスト後はハンドルに対して操作は行わないでください。
  ループサウンドの場合、事前に停止操作を行なわないと
  いつまでも解放されなくなりますので注意してください。
  メイン処理が動いていないと解放されません。
  リクエスト後、再生が停止する前にメイン処理が終了した場合や、
  メイン処理終了後にリクエストした分については
  SEハンドルヒープリセット処理が行われるGsSoundReset()で解放する必要があります。
 */
// =======================================================================
extern void GsSoundRequestFreeSeHandle(GSS_SND_SE_HANDLE *se_handle);

#if _WII
// =======================================================================
// GsSoundRegisterHBMCallbacks
/*!
  HBM入出時 サウンド処理用コールバック登録
  
  @param enter_func	[in]	HBMへ移行時 コールバック関数
  @param enter_arg	[io]	HBMへ移行時 コールバック引数
  @param leave_func	[in]	HBMから復帰時 コールバック関数
  @param leave_arg	[io]	HBMから復帰時 コールバック引数
  @param fade_func	[in]	HBMフェード中 コールバック関数
  @param fade_arg	[io]	HBMフェード中 コールバック引数
  
  @note
  HBM入出時に呼ばれるコールバック関数を一括登録します。
 */
// =======================================================================
inline void GsSoundRegisterHBMCallbacks(AMF_WII_CALLBACK_ENTER_HBM enter_func,
										void *enter_arg,
										AMF_WII_CALLBACK_LEAVE_HBM leave_func,
										void *leave_arg,
										AMF_WII_CALLBACK_FADE_HBM fade_func,
										void *fade_arg)
{
	amWiiSetCallbackEnterHBM(enter_func, enter_arg);
	amWiiSetCallbackLeaveHBM(leave_func, leave_arg);
	amWiiSetCallbackFadeHBM(fade_func, fade_arg);
}
#endif /* _WII */

// =======================================================================
// GsSoundEnterHBM
/*!
  ホームボタンメニューへ移行時 サウンド操作開始処理
  
  @param arg	[io]	引数
  
  @note
  Wii以外では何もしません。コールバック関数ではありません。
 */
// =======================================================================
extern void GsSoundEnterHBM(void *arg);

// =======================================================================
// GsSoundLeaveHBM
/*!
  ホームボタンメニューから復帰時 サウンド操作開始処理
  
  @param arg	[io]	引数
  
  @note
  Wii以外では何もしません。コールバック関数ではありません。
 */
// =======================================================================
extern void GsSoundLeaveHBM(void *arg);

// =======================================================================
// GsSoundFadeHBM
/*!
  ホームボタンメニューフェード中 サウンド操作更新処理
  
  @param arg	[io]	引数
  
  @retval 1		フェード中
  @retval -1	フェード中ではない
  
  @note
  Wii以外では何もしません。コールバック関数ではありません。
 */
// =======================================================================
extern Sint32 GsSoundFadeHBM(void *arg);

// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================

#endif /* GS_SOUND_H_ */
