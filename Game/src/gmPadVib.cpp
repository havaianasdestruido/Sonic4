// ==========================================================================
/*!
  @file gmPadVib.cpp
  @brief パッド振動

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPadVib.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gsMainSys.h"
#include "objObject.h"
#include "gmTask.h"

#include "gmPadVib.h"

//----- Definitions ---------------------------------------------------------
typedef struct tag_GMS_PAD_VIB_WORK {
	GME_PAD_VIB_TYPE	vib_type;		//!< 振動タイプ
	float				time;			//!< 振動時間
	float				add_dec_time;	//!< 減衰・増幅時間
	float				int_vib_time;	//!< 断続振動時 振動時間
	float				int_stop_time;	//!< 断続振動時 停止時間

	u16					left_vib;		//!< 振動度合い 左
	u16					right_vib;		//!< 振動度合い 右

	u32					flag;			//!< 各種フラグ
	float				time_count;		//!< 演出カウンタ
	float				int_count;		//!< 断続演出カウンタ
	u32					prio;			//!< 振動プライオリティ

} GMS_PAD_VIB_WORK;

#define GMD_PAD_VIB_FLAG_INT_STOP		(0x00000001)	//!< 断続振動 停止中
#define GMD_PAD_VIB_FLAG_PAUSE			(0x00000002)	//!< ポーズ開始

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmPadVibDMain(MTS_TASK_TCB *tcb);
static void gmPadVibDest(MTS_TASK_TCB *tcb);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static MTS_TASK_TCB	*gm_pad_vib_tcb = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmPadVibInit
/*!
 *	パッド振動初期化
 */
// ==========================================================================
void GmPadVibInit(void)
{
#if !_IPHONE
	GMS_PAD_VIB_WORK	*vib_work;

	MTM_ASSERT(gm_pad_vib_tcb == NULL);

#if !_PC
	// システムの方で制御するので常にEnableに
	AoPadEnableVibration(TRUE);
	AoPadSetVibration(0, 0);

#else
	if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_PAD_VIB_ENABLE) {
		AoPadEnableVibration(TRUE);
		AoPadSetVibration(0, 0);
	}
	else {
		AoPadEnableVibration(FALSE);
	}
#endif

	// 処理生成
	//gm_pad_vib_tcb = NULL;
	gm_pad_vib_tcb = MTM_TASK_MAKE_TCB(gmPadVibDMain, gmPadVibDest, 0/*flag*/,
					GMD_TASK_PAUSELEVEL_DEF/*pause_level*/, GMD_TASK_PRIO_PAD_VIB/*prio*/,
					GMD_TASK_GROUP_PAD_VIB/*group*/,
					sizeof(GMS_PAD_VIB_WORK), "GM_PAD_VIB");

	vib_work = (GMS_PAD_VIB_WORK*)mtTaskGetTcbWork(gm_pad_vib_tcb);
	MI_CpuClear8(vib_work, sizeof(GMS_PAD_VIB_WORK));

	// 停止状態で設定
	GMM_PAD_VIB_STOP();
#endif
}

// ==========================================================================
// GmPadVibExit
/*!
 *	パッド振動終了処理
 */
// ==========================================================================
void GmPadVibExit(void)
{
#if !_IPHONE
	if (gm_pad_vib_tcb) {
		mtTaskClearTcb(gm_pad_vib_tcb);
	}

	AoPadSetVibration(0, 0);
	AoPadEnableVibration(FALSE);
#endif
}

// ==========================================================================
// GmPadVibInit
/*!
 *	パッド振動初期化
 *
 *	@param	vib_type		[in]	振動タイプ GME_PAD_VIB_TYPE
 *	@param	time			[in]	振動時間 (フレーム)
 *	@param	left_vib		[in]	左振動度合い
 *	@param	right_vib		[in]	右振動度合い
 *	@param	add_dec_time	[in]	減衰・増幅時間 (フレーム)
 *	@param	int_vib_time	[in]	断続振動 振動時間 (フレーム)
 *	@param	int_stop_time	[in]	断続振動 停止時間 (フレーム)
 *	@param	prio			[in]	振動プライオリティ
 *
 *	@note
 *		time <= -1.f で時間無制限になります。
 */
// ==========================================================================
void GmPadVibSet(GME_PAD_VIB_TYPE vib_type, float time, u16 left_vib, u16 right_vib, float add_dec_time, float int_vib_time, float int_stop_time, u32 prio/*=0*/)
{
#if !_IPHONE
	GMS_PAD_VIB_WORK	*vib_work;

	MTM_ASSERT((u32)vib_type < GME_PAD_VIB_TYPE_MAX);

	if (gm_pad_vib_tcb == NULL) {
		// 初期化されていない時は設定なし
		return;
	}

#if _PC
	// システムの方で制御するので常にEnableに
	if (!(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_PAD_VIB_ENABLE)) {
		// 振動設定なし
		return;
	}
#endif

	// 値チェック
	MTM_ASSERT(vib_type == GME_PAD_VIB_TYPE_STOP || (time >= 1.f || time <= -1.f));
	MTM_ASSERT(vib_type == GME_PAD_VIB_TYPE_STOP || (time <= -1.f || time >= add_dec_time));

	// ワーク取得
	vib_work = (GMS_PAD_VIB_WORK*)mtTaskGetTcbWork(gm_pad_vib_tcb);
	if (vib_type != GME_PAD_VIB_TYPE_STOP/* STOP最優先 */ && (vib_work->prio > prio)) {
		// 先の振動の方が優先が高い
		return;
	}

	// 演出内容設定
	vib_work->vib_type		= vib_type;
	vib_work->time			= time;
	vib_work->left_vib		= left_vib;
	vib_work->right_vib		= right_vib;
	vib_work->add_dec_time	= add_dec_time;
	vib_work->int_vib_time	= int_vib_time;
	vib_work->int_stop_time	= int_stop_time;
	vib_work->prio			= prio;

	// 初期化
	vib_work->time_count = 0.f;
	vib_work->flag &= ~(GMD_PAD_VIB_FLAG_INT_STOP);

	switch (vib_type) {
	case GME_PAD_VIB_TYPE_NORMAL:
	case GME_PAD_VIB_TYPE_DEC:
	case GME_PAD_VIB_TYPE_INT:
		// 振動開始
		AoPadSetVibration(left_vib, right_vib);
		break;

	case GME_PAD_VIB_TYPE_STOP:
		vib_work->time = -1.f;	// 無限期間
		// no break;
	case GME_PAD_VIB_TYPE_ACC:
	default:
		// 振動停止
		AoPadSetVibration(0, 0);
		break;
	}
#endif
}

//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmPadVibDMain
/*!
 *	パッド振動メイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmPadVibDMain(MTS_TASK_TCB *tcb)
{
	GMS_PAD_VIB_WORK	*vib_work;
	float				per;
	u16					left_vib, right_vib;

	vib_work = (GMS_PAD_VIB_WORK*)mtTaskGetTcbWork(tcb);

	// ポーズチェック
	if (ObjObjectPauseCheck(0)) {
		// ポーズ中
		if (!(vib_work->flag & GMD_PAD_VIB_FLAG_PAUSE)) {
			vib_work->flag |= GMD_PAD_VIB_FLAG_PAUSE;
			// 振動停止
			AoPadSetVibration(0, 0);
		}
		return;
	}
	else {//if (vib_work->flag & GMD_PAD_VIB_FLAG_PAUSE) {
		// ポーズ終了
		vib_work->flag &= ~GMD_PAD_VIB_FLAG_PAUSE;
		// 振動再開は後にまかせる
	}

#if _XBOX
	// カレントアカウントが無効になった場合は振動をOFFにする
	if (!AoAccountIsCurrentEnableRealXbox360()) {
		AoPadSetVibration(0, 0);
		return;
	}
#endif
#if 0
	else if (vib_work->flag & GMD_PAD_VIB_FLAG_PAUSE) {
		// ポーズ終了
		vib_work->flag &= ~GMD_PAD_VIB_FLAG_PAUSE;

		if (vib_work->vib_type == GME_PAD_VIB_TYPE_NORMAL) {
			// 通常振動は振動再開
			AoPadSetVibration(vib_work->left_vib, vib_work->right_vib);
		}
	}
#endif

	// 時間終了チェック
	if (vib_work->time > 0.f) {
		vib_work->time_count = ObjTimeCountUpF(vib_work->time_count);
		if (vib_work->time_count >= vib_work->time) {
			// 終了
#if 1
			// 停止無限時間
			vib_work->vib_type = GME_PAD_VIB_TYPE_STOP;
			vib_work->prio = 0;
			vib_work->time = -1.f;
#else
			mtTaskClearTcb(tcb);
			return;
#endif
		}
	}

	// 振動強度取得
	switch (vib_work->vib_type) {
	case GME_PAD_VIB_TYPE_NORMAL:
		// 通常振動
		left_vib = vib_work->left_vib;
		right_vib = vib_work->right_vib;
		break;
	case GME_PAD_VIB_TYPE_DEC:
		// 減衰
		if ((vib_work->time - vib_work->time_count) < vib_work->add_dec_time) {
			per = (vib_work->time - vib_work->time_count) / vib_work->add_dec_time;
			left_vib	= (u16)nnRoundOff(vib_work->left_vib * per + 0.5f);
			right_vib	= (u16)nnRoundOff(vib_work->right_vib * per + 0.5f);
		}
		else {
			// 減衰前
			left_vib = vib_work->left_vib;
			right_vib = vib_work->right_vib;
		}
		break;
	case GME_PAD_VIB_TYPE_ACC:
		// 増幅
		if (vib_work->time_count < vib_work->add_dec_time) {
			per = vib_work->time_count / vib_work->add_dec_time;
			left_vib	= (u16)nnRoundOff(vib_work->left_vib * per + 0.5f);
			right_vib	= (u16)nnRoundOff(vib_work->right_vib * per + 0.5f);
		}
		else {
			// 増幅終了
			left_vib = vib_work->left_vib;
			right_vib = vib_work->right_vib;
		}
		break;
	case GME_PAD_VIB_TYPE_INT:
		// 断続
		vib_work->int_count = ObjTimeCountUpF(vib_work->int_count);
		if (vib_work->flag & GMD_PAD_VIB_FLAG_INT_STOP) {
			// 停止中
			if (vib_work->int_count >= vib_work->int_stop_time) {
				// 停止終了
				left_vib	= vib_work->left_vib;
				right_vib	= vib_work->right_vib;

				vib_work->int_count = 0;
				vib_work->flag &= ~GMD_PAD_VIB_FLAG_INT_STOP;
			}
			else {
				left_vib	= 0;
				right_vib	= 0;
			}
		}
		else {
			// 振動中
			if (vib_work->int_count >= vib_work->int_stop_time) {
				// 振動終了
				left_vib	= 0;
				right_vib	= 0;

				vib_work->int_count = 0;
				vib_work->flag |= GMD_PAD_VIB_FLAG_INT_STOP;
			}
			else {
				left_vib	= vib_work->left_vib;
				right_vib	= vib_work->right_vib;
			}
		}
		break;
	case GME_PAD_VIB_TYPE_STOP:
		// 停止中
		// no break;
	default:
		left_vib = 0;
		right_vib = 0;
		break;
	}

	// 振動設定
	AoPadSetVibration(left_vib, right_vib);
}

// ==========================================================================
// gmPadVibDest
/*!
 *	パッド振動デストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmPadVibDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// 振動の停止
	AoPadSetVibration(0, 0);

	gm_pad_vib_tcb = NULL;
}



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
