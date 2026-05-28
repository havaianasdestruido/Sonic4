// ==========================================================================
/*!
  @file gmSpStageBra.cpp
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmSpStageBra.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "syEvtSys.h"
#include "gsMainSys.h"
#include "gmMain.h"

#include "gmSpStageBra.h"

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmSpStageBranchInit
/*!
 *	スペステ分岐初期化
 *
 *	@param	arg	[in]	マップからの遷移時 移行スペステステージID(u16)
 */
// ==========================================================================
void GmSpStageBranchInit(void *arg)
{
	UNREFERENCED_PARAMETER(arg);
	
	SYS_EVT_INFO	*evt_info;

	evt_info = SyGetEvtInfo();
	if (evt_info->old_evt_id == GSD_EVT_ID_MAP) {
		// ステージIDそのまま設定
		//g_gs_main_sys_info.stage_id = *((u16*)arg);

		if (g_gs_main_sys_info.stage_id < GSD_MAIN_STAGE_ID_SS1 ||
				g_gs_main_sys_info.stage_id > GSD_MAIN_STAGE_ID_SS7) {
			MTM_ASSERT(0);
			g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_SS1;
		}
	}
	else {
		if (!GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_SS1)) {
			g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_SS1;
		}
		else if (!GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_SS2)) {
			g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_SS2;
		}
		else if (!GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_SS3)) {
			g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_SS3;
		}
		else if (!GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_SS4)) {
			g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_SS4;
		}
		else if (!GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_SS5)) {
			g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_SS5;
		}
		else if (!GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_SS6)) {
			g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_SS6;
		}
		else {
			g_gs_main_sys_info.stage_id = GSD_MAIN_STAGE_ID_SS7;
		}
	}

	g_gs_main_sys_info.char_id[0] = GSD_CHAR_ID_SP_SONIC;
	g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_SPECIAL_STAGE;

	// ゲーム開始前初期化
	GmMainGSInit();

	// 次のイベントへ
	SyChangeNextEvt();
}

//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
