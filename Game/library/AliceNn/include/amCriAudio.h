// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amCriAudio.h
	@brief      CRI Audio ライブラリ ヘッダ
	@author	    Syuichi Gotou
	@date	    Date: 2009/05/02 
 */
// ================================================================
#ifndef _AM_CRIAUDIO_H
#define _AM_CRIAUDIO_H


//----- Include Files --------------------------------------------------

#include <cri_xpt.h>

#ifdef XPT_TGT_WII
#include <cri_audio_wii.h>
#else
#include <cri_audio.h>
#endif
#include <cri_file_system.h>

#if _IPHONE
#include "CriSmpSoundOutput_iPhone.h"
#else
#include <CriSmp.h>
#endif

#ifdef XPT_TGT_PC
#define DATA_DIR	"/cri/common/smpdata/criaudio/"
#endif
#ifdef XPT_TGT_XBOX360
#define DATA_DIR	"D:\\"
#endif
#ifdef XPT_TGT_PS3PPU
#include <sys/paths.h>
#define DATA_DIR	SYS_APP_HOME "/cri/common/smpdata/criaudio/"
#endif
#if defined(XPT_TGT_LINDBERGH) || defined(XPT_TGT_LINDBERGH2_6) || defined(XPT_TGT_LINDBERGH_JR)
#define DATA_DIR	""
#endif
#ifdef XPT_TGT_WII
#define DATA_DIR	""
#endif

//----- Definitions ----------------------------------------------------

// ヒープサイズ
#ifdef XPT_TGT_WII
//#define AMD_CRIAUDIO_CSB_SIZE		(5 * 1024 * 1024)   // 
//#define AMD_CRIAUDIO_STREAM_SIZE	(1 * 1024 * 1024)   // 
#define AMD_CRIAUDIO_CSB_SIZE		(4 * 1024 * 1024)   // 
#define AMD_CRIAUDIO_STREAM_SIZE	(512)   // BGMはMIDIを使うため不要 
#elif _IPHONE
#define AMD_CRIAUDIO_CSB_SIZE		(7 * 1024 * 1024)  // 
#define AMD_CRIAUDIO_STREAM_SIZE	(512)  // BGMもCSB単体再生のため不要
#else
#define AMD_CRIAUDIO_CSB_SIZE		(5 * 1024 * 1024)  // 
#define AMD_CRIAUDIO_STREAM_SIZE	(2 * 1024 * 1024)  // 
#endif

//----- Enum Definitions -----------------------------------------------

// ヒープの種類	
typedef enum _AMD_CRIAUDIO_HEAPTYPE
{
	AME_CRIAUDIO_HEAP_CSB = 0,	// SE用
	AME_CRIAUDIO_HEAP_STREAM,	// ストリーム用
	AME_CRIAUDIO_HEAP_MAX,
} AMD_CRIAUDIO_HEAPTYPE;

// CueSheetハンドル定義(CSBの数に関係)	
typedef enum _AMD_CRIAUDIO_CSBTYPE
{
	AME_CRIAUDIO_CSB_SYSTEM = 0,	// システム用（常駐）
	AME_CRIAUDIO_CSB_GAME,			// ゲーム中
	AME_CRIAUDIO_CSB_STREAM,		// ストリームCPK用
	AME_CRIAUDIO_CSB_MAINMENU,		// メインメニュー
	AME_CRIAUDIO_CSB_MAX,
} AMD_CRIAUDIO_CSBTYPE;

// ストリームの種類
typedef enum _AMD_CRIAUDIO_STREAMTYPE
{
	AME_CRIAUDIO_STRM_BGM = 0,		// BGM用
	AME_CRIAUDIO_STRM_BG,			// 背景用 
	AME_CRIAUDIO_STRM_PLAYER,		// プレイヤーボイス
	AME_CRIAUDIO_STRM_ENEMY,		// 敵ボイス
	AME_CRIAUDIO_STRM_EX0,			// 拡張領域0
	AME_CRIAUDIO_STRM_EX1,			// 拡張領域1
	AME_CRIAUDIO_STRM_EX2,			// 拡張領域2
	AME_CRIAUDIO_STRM_EX3,			// 拡張領域3
	AME_CRIAUDIO_STRM_MAX,
} AMD_CRIAUDIO_STREAMTYPE;

//----- Type Definitions -----------------------------------------------

//! CRIAudio管理構造体
typedef struct _AMS_CRIAUDIO_INTERFACE {
	CriFsConfiguration	fs_config;	//!<
	CriHeap				heap[AME_CRIAUDIO_HEAP_MAX];	//!< ヒープ
	void*				fs_work[AME_CRIAUDIO_HEAP_MAX];	//!< Allocate work area
#if _IPHONE
	CriSoundRendererBasic*	sndrndr;	//!< サウンドレンダラ
#else
	CriSoundRenderer*	sndrndr;	//!< サウンドレンダラ
#endif
	CriSmpSoundOutput*	sndout;		//!< サウンド出力
	CriAuObj*			auobj;		//!< CRI Audio Object（複数CueSheet登録可能）
	CriAuCueSheet*		CueSheet[AME_CRIAUDIO_CSB_MAX];	//!< CueSheetハンドル
	Sint32				loadState[AME_CRIAUDIO_CSB_MAX];
	CriError			err;		//!< エラーを格納

	CriAuPlayer*		auply[AME_CRIAUDIO_STRM_MAX];

	// Bind CPK file
	CriFsBinderHn		binder;
	CriFsBinderId		binder_id;		//!< criFsBinder_GetStatusに必要
	CriFsBinderStatus	binder_status;	//!<
	void*				bndr_work;		//!< Allocate work area
	
	
} AMS_CRIAUDIO_INTERFACE;

//----- External Functions ---------------------------------------------
#if _IPHONE
// ================================================================
/*!
 CRI SoundOutput 作成
 外部からoutput用クラスを作りたい場合に呼ぶこと
 
 @return 実体があるか否か TRUEなら実体あり
 */
// ================================================================
BOOL amCriAudioCreateSmpOutput(void);
#endif // _IPHONE

// ================================================================
/*!
	CRI Audio システム初期化(amFsInitの後で呼ぶこと)
*/
// ================================================================
#if _WII
void amCriAudioInit(const CriSoundRendererWii::ConfigParameter *config=NULL);
#else
void amCriAudioInit(const CriSoundRendererBasic::ConfigParameter *config=NULL);
#endif

// ================================================================
/*!
	CRI Audio 管理構造体の取得

	@param output [output] CRI Audio 管理構造体へのポインタ
	@note これがあればなんでもできますが、ライブラリとの競合に注意すること
	      基本的には参照用で、内容の書き換えは行わないこと
*/
// ================================================================
AMS_CRIAUDIO_INTERFACE* amCriAudioGetGlobal();

// ================================================================
/*!
	CRI Audio システム終了（amFsExitの前で呼ぶこと）
*/
// ================================================================
void amCriAudioExit();

// ================================================================
/*!
	CueSheet を生成する（内部で読み込み待ちタスクを生成）

	@param	filePath [input]		CueSheetへのファイルパス名
	@param  csbType  [input]		CueSheetの種類（AMD_CRIAUDIO_CSBTYPE）
	@param  prio     [input]		読み込み待ちタスクの優先度
*/
// ================================================================
void amCriAudioCreateCueSheet(char* filePath, Sint32 csbType, Sint32 prio);

// ================================================================
/*!
	CueSheet の登録解除および削除

	@param  csbType  [input]		CueSheetの種類（AMD_CRIAUDIO_CSBTYPE）
*/
// ================================================================
void amCriAudioDestroyCueSheet(Sint32 csbType);

// ================================================================
/*!
	CPKファイルのバインド

	@param	filePath [input]		CueSheetへのファイルパス名
	@param  prio     [input]		読み込み待ちタスクの優先度

	@note  対応するcsbファイルをロードしてから呼ぶのが望ましい
*/
// ================================================================
void amCriAudioBindCPK(char* filePath, Sint32 prio);

// ================================================================
/*!
	メイン実行（amFsServerで常に呼ばれる）
*/
// ================================================================
void amCriAudioExcuteMain();

// ================================================================
/*!
	Cueの単純再生
*/
// ================================================================
void amCriAudioPlay(char* CueName);

// ================================================================
/*!
	CueIDによるCueの単純再生
*/
// ================================================================
void amCriAudioPlayById(Uint32 cueId);

// ================================================================
/*!
	ストリームCueの単純再生

	@param	Id		[input]		オーディオプレイヤーのＩＤ
	@param  CueName [input]		Cueの名前
*/
// ================================================================
void amCriAudioStrmPlay(Uint32 Id, char* CueName);

#endif	// _AM_CRIAUDIO_H