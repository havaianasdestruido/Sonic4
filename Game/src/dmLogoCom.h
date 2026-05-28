// ==========================================================================
/*!
  @file dmLogoCom.h
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dmLogoCom.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef DM_LOGO_COM_H_
#define DM_LOGO_COM_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#define DMD_LOGO_COM_FILE_PATH_LENGTH_MAX	(256)			//!< ファイルパス長
#define DMD_LOGO_COM_LOAD_CONTEXT_MAX		(10)			//!< 読み込みファイル最大数


/// ロード状態
typedef enum tag_DME_LOGO_COM_LOAD_STATE {
	DMD_LOGO_COM_LOAD_STATE_LOAD_WAIT  = 0,	// 読み込み待機中
	DMD_LOGO_COM_LOAD_STATE_LOADING,			// 読み込み中
	DMD_LOGO_COM_LOAD_STATE_COMPLETE,		// 完了(後処理も完了)

	DMD_LOGO_COM_LOAD_STATE_ERROR,			// エラー
	DMD_LOGO_COM_LOAD_STATE_MAX
} DME_LOGO_COM_LOAD_STATE;


/// データファイル情報
typedef struct tag_DMS_LOGO_COM_LOAD_FILE_INFO {
	const char	*file_path;									//!< 読み込むファイルのパス
	void (*post_func)(struct tag_DMS_COM_LOAD_CONTEXT*);//!< ロード後後処理
} DMS_LOGO_COM_LOAD_FILE_INFO;


/// データ読み込みコンテキスト構造体
typedef struct tag_DMS_COM_LOAD_CONTEXT {

	DME_LOGO_COM_LOAD_STATE					state;				//!< ロード状態
	s32										no;					//!< コンテキストNO

	const DMS_LOGO_COM_LOAD_FILE_INFO		*file_info;			//!< ファイルインフォ

	char	file_path_buf[DMD_LOGO_COM_FILE_PATH_LENGTH_MAX];	//!< データロードリクエスト発行時ファイル名バッファ

    AMS_FS  *fs_req;

} DMS_LOGO_COM_LOAD_CONTEXT;

/// データロードワーク
typedef struct tag_DMS_TITLEOP_LOAD_WORK {
	MTS_TASK_TCB				**load_tcb_addr;		//!< データロードTCBアドレス格納先 (NULL可)

	s32							context_num;

	DMS_LOGO_COM_LOAD_CONTEXT	context[DMD_LOGO_COM_LOAD_CONTEXT_MAX];

} DMS_LOGO_COM_LOAD_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// DmLogoComLoadFileCreate
/*!
 *	データロード処理生成
 *
 *	@param	load_tcb_addr	[in]	TCBアドレス格納バッファ
 *
 *	@return	DMS_LOGO_COM_LOAD_WORK
 */
// ==========================================================================
extern MTS_TASK_TCB* DmLogoComLoadFileCreate(MTS_TASK_TCB **load_tcb_addr);

// ==========================================================================
// DmLogoComLoadFileReg
/*!
 *	データロード ロードファイル登録
 *
 *	@param	load_tcb_addr	[in]	TCBアドレス格納バッファ
 *	@param	file_info		[in]	ファイルインフォ
 *	@param	file_num		[in]	ファイル数
 *
 *	@return	ロード状態
 */
// ==========================================================================
extern void DmLogoComLoadFileReg(MTS_TASK_TCB *tcb, const DMS_LOGO_COM_LOAD_FILE_INFO *file_info, s32 file_num);

// ==========================================================================
// DmLogoComLoadFileStart
/*!
 *	データロード ロードチェック開始
 *
 *	@param	file_info		[in]	ファイルインフォ
 *	@param	file_num		[in]	ファイル数
 *	@param	load_tcb_addr	[in]	TCBアドレス格納バッファ
 *
 *	@return	ロード状態
 */
// ==========================================================================
extern void DmLogoComLoadFileStart(MTS_TASK_TCB *tcb);

// ==========================================================================
// DmLogoComLoadFile
/*!
 *	データロード処理
 *
 *	@param	context	[in]	コンテキスト
 *
 *	@return	ロード状態
 */
// ==========================================================================
extern DME_LOGO_COM_LOAD_STATE DmLogoComLoadFile(DMS_LOGO_COM_LOAD_CONTEXT *context);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // DM_LOGO_COM_H_

//----- Include Files -------------------------------------------------------
