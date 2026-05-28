// ==========================================================================
/*!
  @file dmLogoCom.cpp
  @brief デモ ロード

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dmLogoCom.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "dmLogoCom.h"

//----- Definitions ---------------------------------------------------------
#define DMD_LOGO_COM_TASK_PRIO_DATA_LOAD	(0x1000)		//!< ロードタスクプライオリティ
#define DMD_LOGO_COM_TASK_GROUP_DATA_LOAD	(0)				//!< ロードタスクグループ

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static void dmLogoComDataLoadMain(MTS_TASK_TCB *tcb);
static void dmLogoComDataLoadDest(MTS_TASK_TCB *tcb);

//----- Global Functions ----------------------------------------------------
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
MTS_TASK_TCB* DmLogoComLoadFileCreate(MTS_TASK_TCB **load_tcb_addr)
{
	MTS_TASK_TCB			*tcb;
	DMS_LOGO_COM_LOAD_WORK	*load_work;

	MTM_ASSERT(load_tcb_addr);

	// ロード処理生成
	tcb = MTM_TASK_MAKE_TCB(NULL/*はじめは待機*/, dmLogoComDataLoadDest,
						0/*flag*/, 0xFFFF/*pause_level*/,
						DMD_LOGO_COM_TASK_PRIO_DATA_LOAD, DMD_LOGO_COM_TASK_GROUP_DATA_LOAD,
						sizeof(DMS_LOGO_COM_LOAD_WORK), "DM_LC_LOAD");
	load_work = (DMS_LOGO_COM_LOAD_WORK*)mtTaskGetTcbWork(tcb);
    MI_CpuClear8(load_work, sizeof(DMS_LOGO_COM_LOAD_WORK));

	// TCBアドレス格納
	*load_tcb_addr = tcb;
	load_work->load_tcb_addr = load_tcb_addr;

	return (tcb);
}

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
void DmLogoComLoadFileReg(MTS_TASK_TCB *tcb, const DMS_LOGO_COM_LOAD_FILE_INFO *file_info, s32 file_num)
{
	s32							i;
	DMS_LOGO_COM_LOAD_WORK		*load_work;
	DMS_LOGO_COM_LOAD_CONTEXT	*context;

	MTM_ASSERT(tcb);
	MTM_ASSERT(tcb->proc == NULL);
	MTM_ASSERT(tcb->dest == dmLogoComDataLoadDest);

	load_work = (DMS_LOGO_COM_LOAD_WORK*)mtTaskGetTcbWork(tcb);

	// 登録
	context = load_work->context + load_work->context_num;
	for (i = 0; i < file_num && load_work->context_num < DMD_LOGO_COM_LOAD_CONTEXT_MAX;
				i++, load_work->context_num++, context++) {
		context->no			= load_work->context_num;
		context->file_info	= &file_info[i];
		//context->fs_req		= NULLL;
		//context->state		= DMD_TITLEOP_LOAD_STATE_LOAD_WAIT;

		DmLogoComLoadFile(context);
	}

	MTM_ASSERT(i >= file_num);		// 登録バッファが足りない
}

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
void DmLogoComLoadFileStart(MTS_TASK_TCB *tcb)
{
	MTM_ASSERT(tcb);
	MTM_ASSERT(tcb->dest == dmLogoComDataLoadDest);

	mtTaskChangeTcbProcedure(tcb, dmLogoComDataLoadMain);
}

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
DME_LOGO_COM_LOAD_STATE DmLogoComLoadFile(DMS_LOGO_COM_LOAD_CONTEXT *context)
{
	switch (context->state) {
	case DMD_LOGO_COM_LOAD_STATE_LOAD_WAIT:
	// 読み込み待機
		// ファイルパスコピー
		strcpy(context->file_path_buf, context->file_info->file_path);

		// ファイル読み込みリクエスト発行
		context->fs_req = amFsReadBackground(context->file_path_buf, NULL);

		// 読み込み中
		if (context->fs_req) {
			context->state = DMD_LOGO_COM_LOAD_STATE_LOADING;
		}
		break;

	case DMD_LOGO_COM_LOAD_STATE_LOADING:
	// 読み込み中
		if (amFsIsComplete(context->fs_req)) {
			if (context->file_info->post_func) {
				context->file_info->post_func(context);
			}
			amFsClearRequest(context->fs_req);
			context->fs_req = NULL;

			context->state = DMD_LOGO_COM_LOAD_STATE_COMPLETE;
		}
		break;

	case DMD_LOGO_COM_LOAD_STATE_COMPLETE:
		break;

	default:
	case DMD_LOGO_COM_LOAD_STATE_ERROR:
		break;
	}

	return (context->state);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// dmLogoComDataLoadMain
/*!
 *	データロードメイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoComDataLoadMain(MTS_TASK_TCB *tcb)
{
	s32							i;
	DMS_LOGO_COM_LOAD_WORK		*load_work;
	DMS_LOGO_COM_LOAD_CONTEXT	*context;
	DME_LOGO_COM_LOAD_STATE		load_state;

	load_work = (DMS_LOGO_COM_LOAD_WORK*)mtTaskGetTcbWork(tcb);

	// 読み込み待機
	context = load_work->context;
	for (i = 0; i < load_work->context_num; i++, context++) {
		load_state = DmLogoComLoadFile(context);

		if (load_state != DMD_LOGO_COM_LOAD_STATE_COMPLETE) {
			return;
		}
	}

	// 読み込み終了
	mtTaskClearTcb(tcb);
}

// ==========================================================================
// dmLogoComDataLoadDest
/*!
 *	データロードデストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void dmLogoComDataLoadDest(MTS_TASK_TCB *tcb)
{
	DMS_LOGO_COM_LOAD_WORK	*load_work;

	load_work = (DMS_LOGO_COM_LOAD_WORK*)mtTaskGetTcbWork(tcb);

	if (load_work->load_tcb_addr) {
		// TCBアドレス格納バッファクリア
		if (*load_work->load_tcb_addr == tcb) {
			*load_work->load_tcb_addr = NULL;
		}
	}
}



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
