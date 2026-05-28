// ==========================================================================
/*!
  @file gmStartMsg.cpp
  @brief ゲーム開始時メッセージ

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmStartMsg.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 *	データロードはギミックと一緒に行っておく
 *
 */


//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "gsEnvironment.h"
#include "gsMainSys.h"
#include "objObject.h"
#include "gmTask.h" 
#include "gmEnemy.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"
#include "gmPlySeq.h"
#include "gmSound.h"

#include "gmStartMsg.h"

// データヘッダ
#if !_IPHONE
//#include "common/arc/G_MSG_Z2.HMB"
//#include "common/arc/G_MSG_Z3.HMB"
//#include "common/arc/G_MSG_SS.HMB"
#else //!_IPHONE
#include "ace/G_MSG.HMA"
#include "ace/G_MSG_Z2_JP.HMA"
#include "ace/G_MSG_Z3_JP.HMA"
#include "ace/G_MSG_SS_JP.HMA"
#endif //!_IPHONE
// データ並び
// AMA, AMB... で言語順
// JP, US, FR, IT, GE, SP
// iPhoneはその後ろにアニメーションデータが入る

//----- Definitions ---------------------------------------------------------
#define GMD_SMSG_OK_FIRST_DISP		(1)	//!< 最初からOKを表示

#if _IPHONE
#define GMD_SMSG_AMA_ACT_MAX		(2)		//!< AMAアクション最大数
#define GMD_SMSG_AMA_ACT_ACTION_MAX (7)		//!< AMAアクション・アクション最大数
#else
#define GMD_SMSG_AMA_ACT_MAX		(2)		//!< AMAアクション最大数
#endif // _IPHONE

// 演出設定
#define GMD_SMSG_KEY_WAIT			(30)	//!< キー入力待機時間

//#define GMD_SMSG_POS_MSG_X		(960/2)	//!< メッセージ表示位置X
//#define GMD_SMSG_POS_MSG_Y		(720/2)	//!< メッセージ表示位置Y
#define GMD_SMSG_SCR_WIDTH			(960)	//!< 画面サイズX
#define GMD_SMSG_SCR_HEIGHT			(720)	//!< 画面サイズY

/* スケール調整 */
#if _WII
#define GMD_SMSG_SCALE				(1.5f)
#define GMD_SMSG_ACT_SCALE			((fx32)(FX32_ONE*GMD_SMSG_SCALE))
#elif _IPHONE
#define GMD_SMSG_SCALE				(1.6875f)//(1.125f)
#define GMD_SMSG_ACT_SCALE			((fx32)(FX32_ONE*GMD_SMSG_SCALE))
#else
#define GMD_SMSG_SCALE				(1.0f)
#define GMD_SMSG_ACT_SCALE			((fx32)(FX32_ONE*GMD_SMSG_SCALE))
#endif

/// メッセージタイプ
typedef enum tag_GME_SMSG_TYPE {
	GMD_SMSG_TYPE_Z2	= 0,
	GMD_SMSG_TYPE_Z3,
	GMD_SMSG_TYPE_SS,

	GMD_SMSG_TYPE_MAX
} GME_SMSG_TYPE;

/// メッセージデータ1セット分並び
enum {
	GMD_SMSG_DATA_MSG_AMA		= 0,
	GMD_SMSG_DATA_MSG_TEX_AMB,

	GMD_SMSG_DATA_MSG_MAX

	// このセットで言語分並んでいる
};

/// ウィンドウデータ並び
enum {
	GMD_SMSG_DATA_WIN_AMA		= 0,
	GMD_SMSG_DATA_WIN_TEX_AMB,

	GMD_SMSG_DATA_WIN_MAX
};
#define GMD_SMSG_DATA_WIN_TEX_ID	(0)		//!< ウィンドウ使用テクスチャID

/// AOSテクスチャタイプ
enum {
	GMD_SMSG_AOSTEX_MSG	= 0,		//!< メッセージテクスチャ
	GMD_SMSG_AOSTEX_WIN,			//!< ウィンドウテクスチャ

#if _IPHONE
	GMD_SMSG_AOSTEX_ACTION,			//!< アクションテクスチャ
#endif // _IPHONE

	GMD_SMSG_AOSTEX_MAX
};

/// 2D描画オブジェクト
typedef struct tag_GMS_SMSG_2D_WORK {
	OBS_OBJECT_WORK			obj_work;
	OBS_ACTION2D_AMA_WORK	obj_2d;
} GMS_SMSG_2D_OBJ_WORK;


/// マネージャーワーク
typedef struct tag_GMS_SMSG_MGR_WORK {
	u32							flag;
	s32							timer;

	GME_SMSG_TYPE				msg_type;			//!< メッセージタイプ


	void (*func)(struct tag_GMS_SMSG_MGR_WORK*);	//!< メイン処理

	float						win_per;			//!< ウィンドウサイズ割合


	GMS_SMSG_2D_OBJ_WORK	*ama_2d_work[GMD_SMSG_AMA_ACT_MAX];	//!< AMAオブジェクト
#if _IPHONE
	GMS_SMSG_2D_OBJ_WORK	*ama_2d_work_act[GMD_SMSG_AMA_ACT_ACTION_MAX]; //!< AMAアクションオブジェクト
#endif // _IPHONE
} GMS_SMSG_MGR_WORK;

// GMS_SMSG_MGR_WORK::flag
#define GMD_SMSG_FLAG_WIN_DISP	(0x00000001)	//!< ウィンドウ描画
#define GMD_SMSG_FLAG_OK_DISP	(0x00000002)	//!< OK 描画
#define GMD_SMSG_FLAG_END		(0x00000004)	//!< 処理終了



//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmStartMsgDest(MTS_TASK_TCB *tcb);
static void gmStartMsgMain(MTS_TASK_TCB *tcb);

static void gmStartMsgMain_StartWait(GMS_SMSG_MGR_WORK *mgr_work);
static void gmStartMsgMain_WindowOpen(GMS_SMSG_MGR_WORK *mgr_work);
static void gmStartMsgMain_KeyWait(GMS_SMSG_MGR_WORK *mgr_work);
static void gmStartMsgMain_WindowClose(GMS_SMSG_MGR_WORK *mgr_work);

static void gmStartMsgObjPost(void);
static void gmStartMsgObjMain(OBS_OBJECT_WORK *obj_work);
static void gmStartMsgDrawWindowPre_DT(void* param);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static AOS_TEXTURE *gm_start_msg_aos_tex = NULL;		//!< AOSテクスチャ

static MTS_TASK_TCB *gm_start_msg_tcb = NULL;			//!< メインTCB
static BOOL gm_start_msg_end_state = TRUE;				//!< 処理終了状況

/// AMAアクション数テーブル
static s16 gm_start_msg_ama_act_num_tbl[GMD_SMSG_TYPE_MAX] = {
#if _IPHONE
	1,	// Z2
	2,	// Z3
	2,	// SS
#else
	2,	// Z2
	2,	// Z3
	2,	// SS
#endif // _IPHONE
};

#if _IPHONE
/// 本体画像アクションIDテーブル(フリック時)
static const s32 gm_start_msg_body_act_id_table[GMD_SMSG_TYPE_MAX][GMD_SMSG_AMA_ACT_ACTION_MAX] = {
	//GMD_SMSG_TYPE_Z2
	{	-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
	},
	//GMD_SMSG_TYPE_Z3
	{	IDA_G_MSG_ACT_IPHONE_2,
		IDA_G_MSG_ACT_ARROW,
		IDA_G_MSG_ACT_YUBI_FLICK,
		IDA_G_MSG_ACT_FLIC_LINE,
		IDA_G_MSG_ACT_YUBI_TAP,
		IDA_G_MSG_ACT_TAP_LINE,
		IDA_G_MSG_ACT_A_BTN,
	},
	//GMD_SMSG_TYPE_SS
	{	IDA_G_MSG_ACT_IPHONE_2,
		IDA_G_MSG_ACT_ARROW,
		IDA_G_MSG_ACT_YUBI_FLICK,
		IDA_G_MSG_ACT_FLIC_LINE,
		IDA_G_MSG_ACT_YUBI_TAP,
		IDA_G_MSG_ACT_TAP_LINE,
		IDA_G_MSG_ACT_A_BTN,
	}
};
#endif // _IPHONE

#if !_WII
// PS3, Xbox, Win32
#define GMD_SMSG_Z2_JP_MSG_H	((s32)(80*GMD_SMSG_SCALE))	//!< Z2 JP メッセージ高さ
#define GMD_SMSG_Z2_JP_MSG_W	((s32)(510*GMD_SMSG_SCALE))	//!< Z2 JP メッセージ幅
#define GMD_SMSG_Z2_US_MSG_H	((s32)(80*GMD_SMSG_SCALE))	//!< Z2 US メッセージ高さ
#define GMD_SMSG_Z2_US_MSG_W	((s32)(330*GMD_SMSG_SCALE))	//!< Z2 US メッセージ幅
#define GMD_SMSG_Z2_FR_MSG_H	((s32)(80*GMD_SMSG_SCALE))	//!< Z2 FR メッセージ高さ
#define GMD_SMSG_Z2_FR_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z2 FR メッセージ幅
#define GMD_SMSG_Z2_IT_MSG_H	((s32)(80*GMD_SMSG_SCALE))	//!< Z2 IT メッセージ高さ
#define GMD_SMSG_Z2_IT_MSG_W	((s32)(324*GMD_SMSG_SCALE))	//!< Z2 IT メッセージ幅
#define GMD_SMSG_Z2_GE_MSG_H	((s32)(80*GMD_SMSG_SCALE))	//!< Z2 GE メッセージ高さ
#define GMD_SMSG_Z2_GE_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z2 GE メッセージ幅
#define GMD_SMSG_Z2_SP_MSG_H	((s32)(110*GMD_SMSG_SCALE))	//!< Z2 SP メッセージ高さ
#define GMD_SMSG_Z2_SP_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z2 SP メッセージ幅

#if _XBOX
// Xbox
#define GMD_SMSG_Z3_JP_MSG_H	((s32)(110*GMD_SMSG_SCALE))	//!< Z3 JP メッセージ高さ
#define GMD_SMSG_Z3_JP_MSG_W	((s32)(610*GMD_SMSG_SCALE))	//!< Z3 JP メッセージ幅
#define GMD_SMSG_Z3_US_MSG_H	((s32)(200*GMD_SMSG_SCALE))	//!< Z3 US メッセージ高さ
#define GMD_SMSG_Z3_US_MSG_W	((s32)(340*GMD_SMSG_SCALE))	//!< Z3 US メッセージ幅
#define GMD_SMSG_Z3_FR_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 FR メッセージ高さ
#define GMD_SMSG_Z3_FR_MSG_W	((s32)(330*GMD_SMSG_SCALE))	//!< Z3 FR メッセージ幅
#define GMD_SMSG_Z3_IT_MSG_H	((s32)(200*GMD_SMSG_SCALE))	//!< Z3 IT メッセージ高さ
#define GMD_SMSG_Z3_IT_MSG_W	((s32)(340*GMD_SMSG_SCALE))	//!< Z3 IT メッセージ幅
#define GMD_SMSG_Z3_GE_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 GE メッセージ高さ
#define GMD_SMSG_Z3_GE_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z3 GE メッセージ幅
#define GMD_SMSG_Z3_SP_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 SP メッセージ高さ
#define GMD_SMSG_Z3_SP_MSG_W	((s32)(310*GMD_SMSG_SCALE))	//!< Z3 SP メッセージ幅

#define GMD_SMSG_SS_JP_MSG_H	((s32)(110*GMD_SMSG_SCALE))	//!< SS JP メッセージ高さ
#define GMD_SMSG_SS_JP_MSG_W	((s32)(610*GMD_SMSG_SCALE))	//!< SS JP メッセージ幅
#define GMD_SMSG_SS_US_MSG_H	((s32)(200*GMD_SMSG_SCALE))	//!< SS US メッセージ高さ
#define GMD_SMSG_SS_US_MSG_W	((s32)(340*GMD_SMSG_SCALE))	//!< SS US メッセージ幅
#define GMD_SMSG_SS_FR_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< SS FR メッセージ高さ
#define GMD_SMSG_SS_FR_MSG_W	((s32)(330*GMD_SMSG_SCALE))	//!< SS FR メッセージ幅
#define GMD_SMSG_SS_IT_MSG_H	((s32)(200*GMD_SMSG_SCALE))	//!< SS IT メッセージ高さ
#define GMD_SMSG_SS_IT_MSG_W	((s32)(340*GMD_SMSG_SCALE))	//!< SS IT メッセージ幅
#define GMD_SMSG_SS_GE_MSG_H	((s32)(200*GMD_SMSG_SCALE))	//!< SS GE メッセージ高さ
#define GMD_SMSG_SS_GE_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< SS GE メッセージ幅
#define GMD_SMSG_SS_SP_MSG_H	((s32)(200*GMD_SMSG_SCALE))	//!< SS SP メッセージ高さ
#define GMD_SMSG_SS_SP_MSG_W	((s32)(310*GMD_SMSG_SCALE))	//!< SS SP メッセージ幅

#elif _IPHONE
#define GMD_SMSG_Z3_JP_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< Z3 JP メッセージ高さ
#define GMD_SMSG_Z3_JP_MSG_W	((s32)(510*GMD_SMSG_SCALE))	//!< Z3 JP メッセージ幅
#define GMD_SMSG_Z3_US_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< Z3 US メッセージ高さ
#define GMD_SMSG_Z3_US_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< Z3 US メッセージ幅
#define GMD_SMSG_Z3_FR_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< Z3 FR メッセージ高さ
#define GMD_SMSG_Z3_FR_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< Z3 FR メッセージ幅
#define GMD_SMSG_Z3_IT_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< Z3 IT メッセージ高さ
#define GMD_SMSG_Z3_IT_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< Z3 IT メッセージ幅
#define GMD_SMSG_Z3_GE_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< Z3 GE メッセージ高さ
#define GMD_SMSG_Z3_GE_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< Z3 GE メッセージ幅
#define GMD_SMSG_Z3_SP_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< Z3 SP メッセージ高さ
#define GMD_SMSG_Z3_SP_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< Z3 SP メッセージ幅

#define GMD_SMSG_SS_JP_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< SS JP メッセージ高さ
#define GMD_SMSG_SS_JP_MSG_W	((s32)(510*GMD_SMSG_SCALE))	//!< SS JP メッセージ幅
#define GMD_SMSG_SS_US_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< SS US メッセージ高さ
#define GMD_SMSG_SS_US_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< SS US メッセージ幅
#define GMD_SMSG_SS_FR_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< SS FR メッセージ高さ
#define GMD_SMSG_SS_FR_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< SS FR メッセージ幅
#define GMD_SMSG_SS_IT_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< SS IT メッセージ高さ
#define GMD_SMSG_SS_IT_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< SS IT メッセージ幅
#define GMD_SMSG_SS_GE_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< SS GE メッセージ高さ
#define GMD_SMSG_SS_GE_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< SS GE メッセージ幅
#define GMD_SMSG_SS_SP_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< SS SP メッセージ高さ
#define GMD_SMSG_SS_SP_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< SS SP メッセージ幅

#else // #if _XBOX
// PS3, Win32
#define GMD_SMSG_Z3_JP_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< Z3 JP メッセージ高さ
#define GMD_SMSG_Z3_JP_MSG_W	((s32)(510*GMD_SMSG_SCALE))	//!< Z3 JP メッセージ幅
#define GMD_SMSG_Z3_US_MSG_H	((s32)(200*GMD_SMSG_SCALE))	//!< Z3 US メッセージ高さ
#define GMD_SMSG_Z3_US_MSG_W	((s32)(330*GMD_SMSG_SCALE))	//!< Z3 US メッセージ幅
#define GMD_SMSG_Z3_FR_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 FR メッセージ高さ
#define GMD_SMSG_Z3_FR_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z3 FR メッセージ幅
#define GMD_SMSG_Z3_IT_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 IT メッセージ高さ
#define GMD_SMSG_Z3_IT_MSG_W	((s32)(324*GMD_SMSG_SCALE))	//!< Z3 IT メッセージ幅
#define GMD_SMSG_Z3_GE_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 GE メッセージ高さ
#define GMD_SMSG_Z3_GE_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z3 GE メッセージ幅
#define GMD_SMSG_Z3_SP_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 SP メッセージ高さ
#define GMD_SMSG_Z3_SP_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z3 SP メッセージ幅

#define GMD_SMSG_SS_JP_MSG_H	((s32)(160*GMD_SMSG_SCALE))	//!< SS JP メッセージ高さ
#define GMD_SMSG_SS_JP_MSG_W	((s32)(510*GMD_SMSG_SCALE))	//!< SS JP メッセージ幅
#define GMD_SMSG_SS_US_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< SS US メッセージ高さ
#define GMD_SMSG_SS_US_MSG_W	((s32)(330*GMD_SMSG_SCALE))	//!< SS US メッセージ幅
#define GMD_SMSG_SS_FR_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< SS FR メッセージ高さ
#define GMD_SMSG_SS_FR_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< SS FR メッセージ幅
#define GMD_SMSG_SS_IT_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< SS IT メッセージ高さ
#define GMD_SMSG_SS_IT_MSG_W	((s32)(324*GMD_SMSG_SCALE))	//!< SS IT メッセージ幅
#define GMD_SMSG_SS_GE_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< SS GE メッセージ高さ
#define GMD_SMSG_SS_GE_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< SS GE メッセージ幅
#define GMD_SMSG_SS_SP_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< SS SP メッセージ高さ
#define GMD_SMSG_SS_SP_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< SS SP メッセージ幅

#endif // #if _XBOX

#else	// #if !_WII

// Wii用設定
#define GMD_SMSG_Z2_JP_MSG_H	((s32)(80*GMD_SMSG_SCALE))	//!< Z2 JP メッセージ高さ
#define GMD_SMSG_Z2_JP_MSG_W	((s32)(510*GMD_SMSG_SCALE))	//!< Z2 JP メッセージ幅
#define GMD_SMSG_Z2_US_MSG_H	((s32)(80*GMD_SMSG_SCALE))	//!< Z2 US メッセージ高さ
#define GMD_SMSG_Z2_US_MSG_W	((s32)(330*GMD_SMSG_SCALE))	//!< Z2 US メッセージ幅
#define GMD_SMSG_Z2_FR_MSG_H	((s32)(80*GMD_SMSG_SCALE))	//!< Z2 FR メッセージ高さ
#define GMD_SMSG_Z2_FR_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z2 FR メッセージ幅
#define GMD_SMSG_Z2_IT_MSG_H	((s32)(80*GMD_SMSG_SCALE))	//!< Z2 IT メッセージ高さ
#define GMD_SMSG_Z2_IT_MSG_W	((s32)(324*GMD_SMSG_SCALE))	//!< Z2 IT メッセージ幅
#define GMD_SMSG_Z2_GE_MSG_H	((s32)(80*GMD_SMSG_SCALE))	//!< Z2 GE メッセージ高さ
#define GMD_SMSG_Z2_GE_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z2 GE メッセージ幅
#define GMD_SMSG_Z2_SP_MSG_H	((s32)(110*GMD_SMSG_SCALE))	//!< Z2 SP メッセージ高さ
#define GMD_SMSG_Z2_SP_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z2 SP メッセージ幅

#define GMD_SMSG_Z3_JP_MSG_H	((s32)(130*GMD_SMSG_SCALE))	//!< Z3 JP メッセージ高さ
#define GMD_SMSG_Z3_JP_MSG_W	((s32)(510*GMD_SMSG_SCALE))	//!< Z3 JP メッセージ幅
#define GMD_SMSG_Z3_US_MSG_H	((s32)(200*GMD_SMSG_SCALE))	//!< Z3 US メッセージ高さ
#define GMD_SMSG_Z3_US_MSG_W	((s32)(330*GMD_SMSG_SCALE))	//!< Z3 US メッセージ幅
#define GMD_SMSG_Z3_FR_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 FR メッセージ高さ
#define GMD_SMSG_Z3_FR_MSG_W	((s32)(380*GMD_SMSG_SCALE))	//!< Z3 FR メッセージ幅
#define GMD_SMSG_Z3_IT_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 IT メッセージ高さ
#define GMD_SMSG_Z3_IT_MSG_W	((s32)(324*GMD_SMSG_SCALE))	//!< Z3 IT メッセージ幅
#define GMD_SMSG_Z3_GE_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 GE メッセージ高さ
#define GMD_SMSG_Z3_GE_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z3 GE メッセージ幅
#define GMD_SMSG_Z3_SP_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< Z3 SP メッセージ高さ
#define GMD_SMSG_Z3_SP_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< Z3 SP メッセージ幅

#define GMD_SMSG_SS_JP_MSG_H	((s32)(160*GMD_SMSG_SCALE))	//!< SS JP メッセージ高さ
#define GMD_SMSG_SS_JP_MSG_W	((s32)(510*GMD_SMSG_SCALE))	//!< SS JP メッセージ幅
#define GMD_SMSG_SS_US_MSG_H	((s32)(200*GMD_SMSG_SCALE))	//!< SS US メッセージ高さ
#define GMD_SMSG_SS_US_MSG_W	((s32)(330*GMD_SMSG_SCALE))	//!< SS US メッセージ幅
#define GMD_SMSG_SS_FR_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< SS FR メッセージ高さ
#define GMD_SMSG_SS_FR_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< SS FR メッセージ幅
#define GMD_SMSG_SS_IT_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< SS IT メッセージ高さ
#define GMD_SMSG_SS_IT_MSG_W	((s32)(324*GMD_SMSG_SCALE))	//!< SS IT メッセージ幅
#define GMD_SMSG_SS_GE_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< SS GE メッセージ高さ
#define GMD_SMSG_SS_GE_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< SS GE メッセージ幅
#define GMD_SMSG_SS_SP_MSG_H	((s32)(230*GMD_SMSG_SCALE))	//!< SS SP メッセージ高さ
#define GMD_SMSG_SS_SP_MSG_W	((s32)(320*GMD_SMSG_SCALE))	//!< SS SP メッセージ幅
#endif	// #if !_WII

#if _XBOX
#define GMD_SMSG_Z2_MSG_TEX_H	((s32)(128*GMD_SMSG_SCALE))	//!< Z2 メッセージテクスチャ高さ
#define GMD_SMSG_Z2_MSG_TEX_W	((s32)(512*GMD_SMSG_SCALE))	//!< Z2 メッセージテクスチャ幅
#define GMD_SMSG_Z3_MSG_TEX_H	((s32)(256*GMD_SMSG_SCALE))	//!< Z3 メッセージテクスチャ高さ
#define GMD_SMSG_Z3_MSG_TEX_W	((s32)(1024*GMD_SMSG_SCALE))//!< Z3 メッセージテクスチャ幅
#define GMD_SMSG_SS_MSG_TEX_H	((s32)(256*GMD_SMSG_SCALE))	//!< SS メッセージテクスチャ高さ
#define GMD_SMSG_SS_MSG_TEX_W	((s32)(1024*GMD_SMSG_SCALE))//!< SS メッセージテクスチャ幅
#else
#define GMD_SMSG_Z2_MSG_TEX_H	((s32)(128*GMD_SMSG_SCALE))	//!< Z2 メッセージテクスチャ高さ
#define GMD_SMSG_Z2_MSG_TEX_W	((s32)(512*GMD_SMSG_SCALE))	//!< Z2 メッセージテクスチャ幅
#define GMD_SMSG_Z3_MSG_TEX_H	((s32)(256*GMD_SMSG_SCALE))	//!< Z3 メッセージテクスチャ高さ
#define GMD_SMSG_Z3_MSG_TEX_W	((s32)(512*GMD_SMSG_SCALE))	//!< Z3 メッセージテクスチャ幅
#define GMD_SMSG_SS_MSG_TEX_H	((s32)(256*GMD_SMSG_SCALE))	//!< SS メッセージテクスチャ高さ
#define GMD_SMSG_SS_MSG_TEX_W	((s32)(512*GMD_SMSG_SCALE))	//!< SS メッセージテクスチャ幅
#endif // #if _XBOX

#if _IPHONE
#define GMD_SMSG_MSG_ACTION_H	(s32)(410)	//!< 操作説明アクション高さ

#define GMD_SMSG_MSG_OK_H		((s32)( 0 ))	//!< OK 高さ
#define GMD_SMSG_MSG_OK_W		((s32)( 0 ))	//!< OK 幅

#define GMD_SMSG_MSGOK_SPACE		(0)	//!< メッセージとOKとのスペース
#else
#define GMD_SMSG_MSG_ACTION_H	(s32)( 0 )	//!< 操作説明アクション高さ

#define GMD_SMSG_MSG_OK_H		((s32)(32*GMD_SMSG_SCALE))	//!< OK 高さ
#define GMD_SMSG_MSG_OK_W		((s32)(48*GMD_SMSG_SCALE))	//!< OK 幅

#define GMD_SMSG_MSGOK_SPACE		(30)	//!< メッセージとOKとのスペース
#endif 

#define GMD_SMSG_WIN_MARGIN			(30)	//!< ウィンドウ端マージン

#define GMD_SMSG_WIN_EDGE_ADJUST	(-32)	//!< ウィンドウ枠補正値


/// アクション表示位置テーブル
static fx32 gm_start_msg_ama_act_pos_tbl[GMD_SMSG_TYPE_MAX][GSD_LANGUAGE_NUM][GMD_SMSG_AMA_ACT_MAX][MTD_XY] = {
#if _IPHONE
	// Z2
	// memo メッセージテクスチャ512*128 として中央基点配置
	{
		{// JP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// US
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE, 
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_US_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// FR
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_FR_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// IT
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_IT_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// GE
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_GE_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// SP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_SP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
	},
	// Z3
	// memo メッセージテクスチャ512*256 として中央基点配置
	{
		{// JP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// US
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// FR
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// IT
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// GE
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// SP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
	},
	// SS
	// memo メッセージテクスチャ512*256 として中央基点配置
	{
		{// JP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// US
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// FR
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// IT
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// GE
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
		{// SP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE + GMD_SMSG_MSG_ACTION_H*FX32_ONE},	// メッセージ
			{0, 0}, // TAP!
		},
	},
#else
	// Z2
	// memo メッセージテクスチャ512*128 として中央基点配置
	{
		{// JP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				//(GMD_SMSG_SCR_HEIGHT/2-(GMD_SMSG_Z2_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)/2+GMD_SMSG_Z2_MSG_TEX_H/2)*FX32_ONE},
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				//(GMD_SMSG_SCR_HEIGHT/2+(GMD_SMSG_Z2_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)/2-GMD_SMSG_MSG_OK_H/2)*FX32_ONE},// OK
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z2_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// US
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE, 
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_US_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z2_US_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// FR
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_FR_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z2_FR_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// IT
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_IT_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z2_IT_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// GE
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_GE_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z2_GE_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// SP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z2_SP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z2_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z2_SP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
	},
	// Z3
	// memo メッセージテクスチャ512*256 として中央基点配置
	{
		{// JP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z3_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z3_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// US
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z3_US_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z3_US_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// FR
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z3_FR_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z3_FR_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// IT
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z3_IT_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z3_IT_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// GE
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z3_GE_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z3_GE_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// SP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_Z3_SP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_Z3_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_Z3_SP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
	},
	// SS
	// memo メッセージテクスチャ512*256 として中央基点配置
	{
		{// JP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_SS_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_SS_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// US
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_SS_US_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_SS_US_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// FR
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_SS_FR_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_SS_FR_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// IT
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_SS_IT_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_SS_IT_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// GE
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_SS_GE_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_SS_GE_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
		{// SP
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT-(GMD_SMSG_SS_SP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)+GMD_SMSG_SS_MSG_TEX_H)/2*FX32_ONE},	// メッセージ
			{GMD_SMSG_SCR_WIDTH/2*FX32_ONE,
				(GMD_SMSG_SCR_HEIGHT+(GMD_SMSG_SS_SP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE)-GMD_SMSG_MSG_OK_H)/2*FX32_ONE},// OK
		},
	},
#endif // _IPHONE
};

/// ウィンドウサイズテーブル
static float gm_start_msg_win_size_tbl[GMD_SMSG_TYPE_MAX][GSD_LANGUAGE_NUM][4] = {
	{// Z2							メッセージ+OK+OKとの隙間+上下マージン
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z2_JP_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z2_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2},		// JP
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z2_US_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z2_US_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2},		// US
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z2_FR_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z2_FR_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2},		// FR
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z2_IT_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z2_IT_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2},		// IT
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z2_GE_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z2_GE_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2},		// GE
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z2_SP_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z2_SP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2},		// SP
	},
	{// Z3
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z3_JP_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z3_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// JP
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z3_US_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z3_US_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// US
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z3_FR_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z3_FR_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// FR
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z3_IT_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z3_IT_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// IT
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z3_GE_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z3_GE_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// GE
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_Z3_SP_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_Z3_SP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// SP
	},
	{// SS
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_SS_JP_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_SS_JP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// JP
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_SS_US_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_SS_US_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// US
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_SS_FR_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_SS_FR_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// FR
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_SS_IT_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_SS_IT_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// IT
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_SS_GE_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_SS_GE_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// GE
		{GMD_SMSG_SCR_WIDTH/2, GMD_SMSG_SCR_HEIGHT/2,
				GMD_SMSG_SS_SP_MSG_W+GMD_SMSG_WIN_MARGIN*2,
				GMD_SMSG_SS_SP_MSG_H+GMD_SMSG_MSG_OK_H+GMD_SMSG_MSGOK_SPACE+GMD_SMSG_WIN_MARGIN*2 + GMD_SMSG_MSG_ACTION_H},	// SP
	},
};

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// データビルド
// ==========================================================================
// ==========================================================================
// GmStartMsgBuild
/*!
 *	ゲーム開始時メッセージ ビルド
 */
// ==========================================================================
void GmStartMsgBuild(void)
{
	s32				i;
	AMS_AMB_HEADER	*msg_amb, *win_amb;
	GSE_LANGUAGE	language = GsEnvGetLanguage();
	void			*tex_amb[GMD_SMSG_AOSTEX_MAX] = {NULL};
	AOS_TEXTURE		*aos_tex;

	MTM_ASSERT(gm_start_msg_aos_tex == NULL);

	// テクスチャ管理バッファ取得
	gm_start_msg_aos_tex = (AOS_TEXTURE*)amMemAlloc(sizeof(AOS_TEXTURE) * GMD_SMSG_AOSTEX_MAX);
	MI_CpuClear8(gm_start_msg_aos_tex, sizeof(AOS_TEXTURE) * GMD_SMSG_AOSTEX_MAX);

	// メッセージAMB取得
	msg_amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_START_EXP_MSG);
	amBindConvertAll((u8*)msg_amb);

	// ウィンドウAMB取得
	win_amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_START_EXP_WIN);
	amBindConvertAll((u8*)win_amb);

	// テクスチャビルド
	// メッセージテクスチャ
	tex_amb[GMD_SMSG_AOSTEX_MSG] = amBindGet(msg_amb, language * GMD_SMSG_DATA_MSG_MAX + GMD_SMSG_DATA_MSG_TEX_AMB);
	// ウィンドウテクスチャ
	tex_amb[GMD_SMSG_AOSTEX_WIN] = amBindGet(win_amb, GMD_SMSG_DATA_WIN_TEX_AMB);
#if _IPHONE
	s32 count = GMD_SMSG_AOSTEX_MAX - 1;
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_2_2) {
		// アクションは常にお尻に設定されている
		tex_amb[GMD_SMSG_AOSTEX_ACTION] = amBindGet(msg_amb, msg_amb->file_num - 1);
		count = GMD_SMSG_AOSTEX_MAX;
	}
#endif // _IPHONE

	aos_tex = gm_start_msg_aos_tex;
	for (i = 0; i < count; i++, aos_tex++) {
		AoTexBuild(aos_tex, tex_amb[i]);
		AoTexLoad(aos_tex);
	}
}

// ==========================================================================
// GmStartMsgBuildCheck
/*!
 *	ゲーム開始時メッセージ ビルド終了チェック
 *
 *	@return	TRUE : ビルド終了
 */
// ==========================================================================
BOOL GmStartMsgBuildCheck(void)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

#if _IPHONE
	s32 count = GMD_SMSG_AOSTEX_MAX - 1;
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_2_2) {
		// アクションは常にお尻に設定されている
		count = GMD_SMSG_AOSTEX_MAX;
	}
#endif // _IPHONE

	// テクスチャビルドチェック
	if (gm_start_msg_aos_tex) {
		aos_tex = gm_start_msg_aos_tex;
		for (i = 0; i < count; i++, aos_tex++) {
			if (!AoTexIsLoaded(aos_tex)) {
				b_sts = FALSE;
			}
		}
	}

	return (b_sts);
}


// ==========================================================================
// データフラッシュ
// ==========================================================================
// ==========================================================================
// GmStartMsgFlush
/*!
 *	ゲーム開始時メッセージ フラッシュ
 */
// ==========================================================================
void GmStartMsgFlush(void)
{
	s32				i;
	AOS_TEXTURE		*aos_tex;

#if _IPHONE
	s32 count = GMD_SMSG_AOSTEX_MAX - 1;
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_2_2) {
		// アクションは常にお尻に設定されている
		count = GMD_SMSG_AOSTEX_MAX;
	}
#endif // _IPHONE

	// テクスチャフラッシュ
	aos_tex = gm_start_msg_aos_tex;
	for (i = 0; i < count; i++, aos_tex++) {
		AoTexRelease(aos_tex);
	}
}

// ==========================================================================
// GmStartMsgBuildCheck
/*!
 *	ゲーム開始時メッセージ ビルド終了チェック
 *
 *	@return	TRUE : ビルド終了
 */
// ==========================================================================
BOOL GmStartMsgFlushCheck(void)
{
	s32			i;
	AOS_TEXTURE	*aos_tex;
	BOOL		b_sts = TRUE;

#if _IPHONE
	s32 count = GMD_SMSG_AOSTEX_MAX - 1;
	if (g_gs_main_sys_info.stage_id != GSD_MAIN_STAGE_ID_2_2) {
		// アクションは常にお尻に設定されている
		count = GMD_SMSG_AOSTEX_MAX;
	}
#endif // _IPHONE

	// テクスチャフラッシュチェック
	if (gm_start_msg_aos_tex) {
		aos_tex = gm_start_msg_aos_tex;
		for (i = 0; i < count; i++, aos_tex++) {
			if (!AoTexIsReleased(aos_tex)) {
				b_sts = FALSE;
			}
		}
		if (b_sts) {
			amMemFree(gm_start_msg_aos_tex);
			gm_start_msg_aos_tex = NULL;
		}
	}

	return (b_sts);
}


// ==========================================================================
// デモ実行チェック
// ==========================================================================
// ==========================================================================
// GmStartMsgIsExe
/*!
 *	ゲーム開始時メッセージ 実行確認
 *
 *	@return		TRUE : デモ要実行
 */
// ==========================================================================
BOOL GmStartMsgIsExe(void)
{
	BOOL	exe_sts = FALSE;
#if _IPHONE
	GSS_MAIN_SYS_INFO *main_sys_info = GsGetMainSysInfo();
#endif _IPHONE

	// リスタート状態でなく 該当ステージをクリアしていなければ実行
	if (!(g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_RESTART)) {
#if !_IPHONE
		switch (g_gs_main_sys_info.stage_id) {
		case GSD_MAIN_STAGE_ID_2_2:
			exe_sts = !GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_2_2);
			break;
		case GSD_MAIN_STAGE_ID_3_2:
			exe_sts = !GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_3_2);
			break;
		case GSD_MAIN_STAGE_ID_SS1:
			// フリック操作判定
			if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK) {
				exe_sts = !GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_SS1);
			}
			break;
		}
#else //!_IPHONE
		using namespace gs::backup;
		SSystem &system = SSystem::CreateInstance();

		switch (main_sys_info->stage_id) {
		case GSD_MAIN_STAGE_ID_2_2:
			exe_sts = !GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_2_2);
			break;
		case GSD_MAIN_STAGE_ID_3_2:
			if (!GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_3_2)) {
				//未クリアなら表示
				exe_sts = TRUE;
			} else if (GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK & main_sys_info->game_flag) {
				//クリア時でもフリック側メッセージを1回も表示していないなら表示
				if (!system.IsAnnounce(SSystem::EAnnounce::TruckFlick)) {
					exe_sts = TRUE;
				}
			} else {
				//クリア時でも傾斜側メッセージを1回も表示していないなら表示
				if (!system.IsAnnounce(SSystem::EAnnounce::TruckTilt)) {
					exe_sts = TRUE;
				}
			}
			//表示フラグの設定
			if (!exe_sts) {
				//表示しないなら
				//セーブデータは変更しない
			} else if (GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK & main_sys_info->game_flag) {
				//クリア時でもフリック側メッセージを1回も表示していないなら表示
				system.SetAnnounce(SSystem::EAnnounce::TruckFlick, true);
			} else {
				//クリア時でも傾斜側メッセージを1回も表示していないなら表示
				system.SetAnnounce(SSystem::EAnnounce::TruckTilt, true);
			}
			break;
		case GSD_MAIN_STAGE_ID_SS1:
			if (!GsMainSysIsStageClear(GSD_MAIN_STAGE_ID_SS1)) {
				//未クリアなら表示
				exe_sts = TRUE;
			}
			//nobreak;
		case GSD_MAIN_STAGE_ID_SS2:	case GSD_MAIN_STAGE_ID_SS3:	case GSD_MAIN_STAGE_ID_SS4:
		case GSD_MAIN_STAGE_ID_SS5:	case GSD_MAIN_STAGE_ID_SS6:	case GSD_MAIN_STAGE_ID_SS7:
			if (exe_sts) {
			} else if (GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK & main_sys_info->game_flag) {
				//クリア時でもフリック側メッセージを1回も表示していないなら表示
				if (!system.IsAnnounce(SSystem::EAnnounce::SpecialStageFlick)) {
					exe_sts = TRUE;
				}
			} else {
				//クリア時でも傾斜側メッセージを1回も表示していないなら表示
				if (!system.IsAnnounce(SSystem::EAnnounce::SpecialStageTilt)) {
					exe_sts = TRUE;
				}
			}
			//表示フラグの設定
			if (!exe_sts) {
				//表示しないなら
				//セーブデータは変更しない
			} else if (GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK & main_sys_info->game_flag) {
				//クリア時でもフリック側メッセージを1回も表示していないなら表示
				system.SetAnnounce(SSystem::EAnnounce::SpecialStageFlick, true);
			} else {
				//クリア時でも傾斜側メッセージを1回も表示していないなら表示
				system.SetAnnounce(SSystem::EAnnounce::SpecialStageTilt, true);
			}
			break;
		}
#if defined(AMD_DEBUG)
		if (amTpIsTouchOn(2)) {
			//3点タップなら強制表示
			switch (main_sys_info->stage_id) {
			case GSD_MAIN_STAGE_ID_2_2:
			case GSD_MAIN_STAGE_ID_3_2:
			case GSD_MAIN_STAGE_ID_SS1:
			case GSD_MAIN_STAGE_ID_SS2:	case GSD_MAIN_STAGE_ID_SS3:	case GSD_MAIN_STAGE_ID_SS4:
			case GSD_MAIN_STAGE_ID_SS5:	case GSD_MAIN_STAGE_ID_SS6:	case GSD_MAIN_STAGE_ID_SS7:
				exe_sts = TRUE;
				break;
			}
		}
#endif //defined(AMD_DEBUG)
#endif //!_IPHONE
	}

#if _IPHONE
	if (exe_sts) {
		// メッセージ表示ならポーズフラグを降ろす
		GmMainClearSuspendedPause();
	}
#endif // _IPHONE

	return (exe_sts);
}


// ==========================================================================
// 初期化・終了処理
// ==========================================================================
// ==========================================================================
// GmStartMsgInit
/*!
 *	ゲーム開始時メッセージ初期化
 */
// ==========================================================================
void GmStartMsgInit(void)
{
	s32						i;
	GMS_SMSG_MGR_WORK	*mgr_work;
	AMS_AMB_HEADER			*msg_amb;
	GSE_LANGUAGE			language = GsEnvGetLanguage();
	NNS_TEXLIST				*texlist;
#if _IPHONE
	GSS_MAIN_SYS_INFO	*main_sys_info = GsGetMainSysInfo();
#endif //_IPHONE

	// ゲーム開始時メッセージ表示開始
	g_gm_main_system.game_flag |= GMD_GAME_FLAG_START_MSG;

	// 処理終了状況クリア
	gm_start_msg_end_state = FALSE;

	// マネージャタスク生成
	gm_start_msg_tcb = MTM_TASK_MAKE_TCB(gmStartMsgMain, gmStartMsgDest,
						0/*flag*/, GMD_TASK_NO_GAME_PAUSE/*pause_level*/,
						GMD_TASK_PRIO_START_MSG - 10/*マネージャーは優先をあげておく*/, GMD_TASK_GROUP_START_MSG,
						sizeof(GMS_SMSG_MGR_WORK)/*work_size*/, "GM_S_MSG_MGR");
	mgr_work = (GMS_SMSG_MGR_WORK*)mtTaskGetTcbWork(gm_start_msg_tcb);
	MI_CpuClear8(mgr_work, sizeof(GMS_SMSG_MGR_WORK));

	// コマンドステート設定 ゲーム開始時メッセージ特殊
	ObjDrawSetNNCommandStateTbl(OBD_DRAW_SET_NN_CMD_STATE_TBL_MSG_START, OBD_DRAW_CMD_STATE_GMSG_WIN, TRUE);	// ゲーム開始時メッセージウィンドウ
	ObjDrawSetNNCommandStateTbl(OBD_DRAW_SET_NN_CMD_STATE_TBL_MSG_START + 1, OBD_DRAW_CMD_STATE_GMSG_MSG, TRUE);	// ゲーム開始時メッセージ

	// オブジェクトシステムPost処理設定
	g_obj.ppPost = gmStartMsgObjPost;

	// 演出タイプ取得
	switch (g_gs_main_sys_info.stage_id) {
	default:
		MTM_ASSERT(0);
		// no break;
	case GSD_MAIN_STAGE_ID_2_2:
		mgr_work->msg_type = GMD_SMSG_TYPE_Z2;
		break;
	case GSD_MAIN_STAGE_ID_3_2:
		mgr_work->msg_type = GMD_SMSG_TYPE_Z3;
		break;
	case GSD_MAIN_STAGE_ID_SS1:
	case GSD_MAIN_STAGE_ID_SS2:	case GSD_MAIN_STAGE_ID_SS3:	case GSD_MAIN_STAGE_ID_SS4:
	case GSD_MAIN_STAGE_ID_SS5:	case GSD_MAIN_STAGE_ID_SS6:	case GSD_MAIN_STAGE_ID_SS7:
		mgr_work->msg_type = GMD_SMSG_TYPE_SS;
		break;
	}

	// メッセージAMB取得
	msg_amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_START_EXP_MSG);
	amBindConvertAll((u8*)msg_amb);

	// AMAアクション初期化
	texlist = AoTexGetTexList(&gm_start_msg_aos_tex[GMD_SMSG_AOSTEX_MSG]);
	for (i = 0; i < gm_start_msg_ama_act_num_tbl[mgr_work->msg_type]; i++) {
		mgr_work->ama_2d_work[i] =
			(GMS_SMSG_2D_OBJ_WORK*)OBM_OBJECT_TASK_DETAIL_INIT(
										GMD_TASK_PRIO_START_MSG, GMD_TASK_GROUP_START_MSG,
										GMD_TASK_PAUSELEVEL_DEF, GMD_TASK_NO_GAME_PAUSE,
										sizeof(GMS_SMSG_2D_OBJ_WORK), "GM_SMSG");
#if !_IPHONE
		ObjObjectAction2dAMALoadSetTexlist(&mgr_work->ama_2d_work[i]->obj_work,
										&mgr_work->ama_2d_work[i]->obj_2d,
										NULL/*data_work*/, NULL/*file_name*/,
										language * GMD_SMSG_DATA_MSG_MAX + GMD_SMSG_DATA_MSG_AMA,
										msg_amb,
										texlist, (u32)i, FALSE/*type_node*/);
#else //!_IPHONE
		u32 act_id = (u32)i;
		if (GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK & main_sys_info->game_flag) {
			switch (mgr_work->msg_type) {
			case GMD_SMSG_TYPE_Z3:
				if (IDA_G_MSG_Z3_JP_ACT_TEX_Z3_1 == act_id) {
					act_id = IDA_G_MSG_Z3_JP_ACT_TEX_Z3_2;
				}
				break;
			case GMD_SMSG_TYPE_SS:
				if (IDA_G_MSG_SS_JP_ACT_TEX_SS_1 == act_id) {
					act_id = IDA_G_MSG_SS_JP_ACT_TEX_SS_2;
				}
				break;
			default:
				break;
			}
		}
		ObjObjectAction2dAMALoadSetTexlist(&mgr_work->ama_2d_work[i]->obj_work,
										&mgr_work->ama_2d_work[i]->obj_2d,
										NULL/*data_work*/, NULL/*file_name*/,
										language * GMD_SMSG_DATA_MSG_MAX + GMD_SMSG_DATA_MSG_AMA,
										msg_amb,
										texlist, act_id, FALSE/*type_node*/);
#endif //!_IPHONE

		// 標準設定
		mgr_work->ama_2d_work[i]->obj_work.ppOut = NULL;

		// 処理設定
		mgr_work->ama_2d_work[i]->obj_work.ppFunc = gmStartMsgObjMain;

		// はじめは非表示
		mgr_work->ama_2d_work[i]->obj_work.disp_flag |= OBD_DISP_NODISP;

		// その他設定
		mgr_work->ama_2d_work[i]->obj_work.flag |= OBD_OBJECT_NOCLIP | OBD_OBJECT_NOHIT;
		mgr_work->ama_2d_work[i]->obj_work.move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;
		mgr_work->ama_2d_work[i]->obj_work.disp_flag |= OBD_DISP_NOMAP | OBD_DISP_NODIR | OBD_DISP_NODRAWSCALE;

		// 表示位置
		mgr_work->ama_2d_work[i]->obj_work.pos.x =
				gm_start_msg_ama_act_pos_tbl[mgr_work->msg_type][language][i][MTD_X];
		mgr_work->ama_2d_work[i]->obj_work.pos.y =
				gm_start_msg_ama_act_pos_tbl[mgr_work->msg_type][language][i][MTD_Y];

		// スケール設定
#if _IPHONE
		if (i == 0) {
			mgr_work->ama_2d_work[i]->obj_work.scale.x = GMD_SMSG_ACT_SCALE;
			mgr_work->ama_2d_work[i]->obj_work.scale.y = GMD_SMSG_ACT_SCALE;
		}
#else
		mgr_work->ama_2d_work[i]->obj_work.scale.x = GMD_SMSG_ACT_SCALE;
		mgr_work->ama_2d_work[i]->obj_work.scale.y = GMD_SMSG_ACT_SCALE;
#endif // _IPHONE
	}	

#if _IPHONE
	{
		for (i = 0; i < GMD_SMSG_AMA_ACT_ACTION_MAX; i++) {
			s32 act_id = gm_start_msg_body_act_id_table[mgr_work->msg_type][i];
			//傾斜操作時切り換え
			if (!(GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK & main_sys_info->game_flag)) {
				switch (act_id) {
				case IDA_G_MSG_ACT_IPHONE_2:
					act_id = IDA_G_MSG_ACT_IPHONE;
					break;
				case IDA_G_MSG_ACT_YUBI_TAP:
				case IDA_G_MSG_ACT_TAP_LINE:
				case -1:
					break;
				default:
					//殆ど削除
					act_id = -1;
					break;
				}
			}
			if (act_id < 0) {
				mgr_work->ama_2d_work_act[i] = NULL;
				continue;
			}
			mgr_work->ama_2d_work_act[i] =
				(GMS_SMSG_2D_OBJ_WORK*)OBM_OBJECT_TASK_DETAIL_INIT(
											GMD_TASK_PRIO_START_MSG, GMD_TASK_GROUP_START_MSG,
											GMD_TASK_PAUSELEVEL_DEF, GMD_TASK_NO_GAME_PAUSE,
											sizeof(GMS_SMSG_2D_OBJ_WORK), "GM_SMSG");
	
			texlist = AoTexGetTexList(&gm_start_msg_aos_tex[GMD_SMSG_AOSTEX_ACTION]);
			ObjObjectAction2dAMALoadSetTexlist(&mgr_work->ama_2d_work_act[i]->obj_work,
											&mgr_work->ama_2d_work_act[i]->obj_2d,
											NULL/*data_work*/, NULL/*file_name*/,
											msg_amb->file_num - 2,
											msg_amb,
											texlist, (u32)act_id, FALSE/*type_node*/);
	
			// 標準設定
			mgr_work->ama_2d_work_act[i]->obj_work.ppOut = NULL;
	
			// 処理設定
			mgr_work->ama_2d_work_act[i]->obj_work.ppFunc = gmStartMsgObjMain;
	
			// はじめは非表示
			mgr_work->ama_2d_work_act[i]->obj_work.disp_flag |= OBD_DISP_NODISP;
	
			// その他設定
			mgr_work->ama_2d_work_act[i]->obj_work.flag |= OBD_OBJECT_NOCLIP | OBD_OBJECT_NOHIT;
			mgr_work->ama_2d_work_act[i]->obj_work.move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;
			mgr_work->ama_2d_work_act[i]->obj_work.disp_flag |= OBD_DISP_NOMAP | OBD_DISP_NODIR | OBD_DISP_NODRAWSCALE;
		}
	}
#endif // _IPHONE

	// 処理設定
	mgr_work->func = gmStartMsgMain_StartWait;

	// 先に一度実行しておく
	gmStartMsgMain_StartWait(mgr_work);
}

// ==========================================================================
// GmStartMsgExit
/*!
 *	ゲーム開始時メッセージ終了処理
 */
// ==========================================================================
void GmStartMsgExit(void)
{
	if (gm_start_msg_tcb) {
		// 終了処理
		mtTaskClearTcb(gm_start_msg_tcb);
		gm_start_msg_tcb = NULL;

		// コマンドステート 未使用に設定
		ObjDrawSetNNCommandStateTbl(OBD_DRAW_SET_NN_CMD_STATE_TBL_MSG_START, OBD_DRAW_CMD_STATE_INVALID, FALSE);	// ゲーム開始時メッセージウィンドウ
		ObjDrawSetNNCommandStateTbl(OBD_DRAW_SET_NN_CMD_STATE_TBL_MSG_START + 1, OBD_DRAW_CMD_STATE_INVALID, FALSE);	// ゲーム開始時メッセージ

		// オブジェクトシステムPost処理クリア
		g_obj.ppPost = NULL;

		// ゲーム開始時メッセージ表示終了
		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_START_MSG;
	}
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmStartMsgDest
/*!
 *	ゲーム開始時メッセージ デストラクタ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmStartMsgDest(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);
//	GMS_SMSG_MGR_WORK	*mgr_work;
//
//	mgr_work = (GMS_SMSG_MGR_WORK*)mtTaskGetTcbWork(tcb);

	// オブジェクト破棄
	gm_start_msg_end_state = TRUE;		// フラグで通知(外部から破棄される可能性があるため)
//	for (i = 0; i < GMD_SMSG_AMA_ACT_MAX; i++) {
//		if (mgr_work->ama_2d_work[i]) {
//			mgr_work->ama_2d_work[i]->obj_work.disp_flag |= OBD_DISP_NODISP;
//			mgr_work->ama_2d_work[i]->obj_work.flag		 |= OBD_OBJECT_TASKCLEAR_REQUEST;
//		}
//	}
}

// ==========================================================================
// gmStartMsgMain
/*!
 *	ゲーム開始時メッセージ メイン処理
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmStartMsgMain(MTS_TASK_TCB *tcb)
{
	GMS_SMSG_MGR_WORK	*mgr_work;
	GSE_LANGUAGE			language = GsEnvGetLanguage();

	mgr_work = (GMS_SMSG_MGR_WORK*)mtTaskGetTcbWork(tcb);

	// メイン処理
	if (mgr_work->func) {
		mgr_work->func(mgr_work);
	}

	// 終了チェック
	if (mgr_work->flag & GMD_SMSG_FLAG_END) {
		// 処理終了
		GmStartMsgExit();

		// プレイヤーFWへ
		GmPlySeqChangeFw(g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]);
		g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P]->no_key_timer = 8 * FX32_ONE;	// 少しだけキー取得を行わない

		// オブジェクトポーズ終了
		ObjObjectPauseOut();

		// タイマー開始
		g_gm_main_system.game_flag |= (GMD_GAME_FLAG_COUNT_GAME_TIME |
											GMD_GAME_FLAG_COUNT_SYNC_TIME);
		return;
	}


	//　ウィンドウ描画
	if (mgr_work->flag & GMD_SMSG_FLAG_WIN_DISP) {
		// ウィンドウ描画
		ObjDraw3DNNUserFunc(
			gmStartMsgDrawWindowPre_DT, NULL, 0, OBD_DRAW_CMD_STATE_GMSG_WIN);

		AoWinSysDrawState(AOD_WIN_TYPE_A,
				AoTexGetTexList(&gm_start_msg_aos_tex[GMD_SMSG_AOSTEX_WIN]),
				GMD_SMSG_DATA_WIN_TEX_ID,
				gm_start_msg_win_size_tbl[mgr_work->msg_type][language][0],
				gm_start_msg_win_size_tbl[mgr_work->msg_type][language][1],
				(gm_start_msg_win_size_tbl[mgr_work->msg_type][language][2] + GMD_SMSG_WIN_EDGE_ADJUST) * mgr_work->win_per,
				(gm_start_msg_win_size_tbl[mgr_work->msg_type][language][3] + GMD_SMSG_WIN_EDGE_ADJUST) * mgr_work->win_per,
				OBD_DRAW_CMD_STATE_GMSG_WIN);
	}

}

// ==========================================================================
// gmStartMsgMain_StartWait
/*!
 *	ゲーム開始時メッセージ 演出開始待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmStartMsgMain_StartWait(GMS_SMSG_MGR_WORK *mgr_work)
{
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_START_DEMO) {
		return;
	}

	// ウィンドウ描画ON
	mgr_work->flag |= GMD_SMSG_FLAG_WIN_DISP;

	// オブジェクトポーズ開始
	ObjObjectPause(GMD_TASK_GAME_PAUSE_LEVEL);

	// タイムカウント系フラグをOFF
	g_gm_main_system.game_flag &= ~(GMD_GAME_FLAG_COUNT_GAME_TIME |
										GMD_GAME_FLAG_COUNT_SYNC_TIME);

	// ウィンドウサイズ割合設定
	mgr_work->win_per = 0;

#if _IPHONE
	//SE再生
	GmSoundPlaySE("Window");
#endif //_IPHONE

	// 処理設定
	mgr_work->timer	= 0;
	mgr_work->func	= gmStartMsgMain_WindowOpen;
}

// ==========================================================================
// gmStartMsgMain_WindowOpen
/*!
 *	ゲーム開始時メッセージ ウィンドウオープン
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmStartMsgMain_WindowOpen(GMS_SMSG_MGR_WORK *mgr_work)
{
	mgr_work->timer++;

	if (mgr_work->timer >= GSD_MAINSYS_WIN_EFCT_FRAME) {
		s32	i;

		mgr_work->win_per = 1.f;

		// 演出開始
#if _IPHONE
		for (i = 0; i < gm_start_msg_ama_act_num_tbl[mgr_work->msg_type]; i++) {
#else
#if GMD_SMSG_OK_FIRST_DISP
		for (i = 0; i < gm_start_msg_ama_act_num_tbl[mgr_work->msg_type]; i++) {
#else
		for (i = 0; i < gm_start_msg_ama_act_num_tbl[mgr_work->msg_type] - 1/*OKは表示しない*/; i++) {
#endif
#endif // _IPHONE
			// 表示開始
			mgr_work->ama_2d_work[i]->obj_work.disp_flag &= ~OBD_DISP_NODISP;
		}

#if _IPHONE
		// アクションも表示開始
		for (i = 0; i < GMD_SMSG_AMA_ACT_ACTION_MAX; i++) {
			if (mgr_work->ama_2d_work_act[i]) {
				mgr_work->ama_2d_work_act[i]->obj_work.disp_flag &= ~OBD_DISP_NODISP;
			}
		}
#endif // _IPHONE

		// 処理設定
		mgr_work->timer	= GMD_SMSG_KEY_WAIT;
		mgr_work->func	= gmStartMsgMain_KeyWait;
	}
	else {
		// ウィンドウサイズ設定
		mgr_work->win_per = (float)mgr_work->timer / (float)GSD_MAINSYS_WIN_EFCT_FRAME;
	}
}

// ==========================================================================
// gmStartMsgMain_KeyWait
/*!
 *	ゲーム開始時メッセージ キー入力待機
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmStartMsgMain_KeyWait(GMS_SMSG_MGR_WORK *mgr_work)
{
	if (mgr_work->timer) {
		mgr_work->timer--;
		
#if !_IPHONE
#if !GMD_SMSG_OK_FIRST_DISP
		if (!mgr_work->timer) {
			// OK表示開始開始
			mgr_work->ama_2d_work[gm_start_msg_ama_act_num_tbl[mgr_work->msg_type] - 1]->obj_work.disp_flag &= ~OBD_DISP_NODISP;
		}
#endif
#endif // !_IPHONE
		return;
	}

	// キー入力受付
#if _IPHONE
	if (amTpIsTouchOn(0)) {
#else
	if (AoPadStand() & GSD_KEY_DECIDE) {
#endif // _IPHONE
		s32	i;
		
		// メッセージ表示終了
		for (i = 0; i < gm_start_msg_ama_act_num_tbl[mgr_work->msg_type]; i++) {
			// 表示終了
			mgr_work->ama_2d_work[i]->obj_work.disp_flag |= OBD_DISP_NODISP;
		}

#if _IPHONE
		// アクションも表示終了
		for (i = 0; i < GMD_SMSG_AMA_ACT_ACTION_MAX; i++) {
			if (mgr_work->ama_2d_work_act[i]) {
				mgr_work->ama_2d_work_act[i]->obj_work.disp_flag |= OBD_DISP_NODISP;
			}
		}
#endif // _IPHONE


		// 処理設定
		mgr_work->timer	= GSD_MAINSYS_WIN_EFCT_FRAME;
		mgr_work->func	= gmStartMsgMain_WindowClose;
	}
}

// ==========================================================================
// gmStartMsgMain_WindowClose
/*!
 *	ゲーム開始時メッセージ ウィンドウクローズ
 *
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
void gmStartMsgMain_WindowClose(GMS_SMSG_MGR_WORK *mgr_work)
{
	mgr_work->timer--;

	if (mgr_work->timer <= 0) {
		mgr_work->win_per = 0.f;

		// 処理設定
		mgr_work->func	= NULL;

		// メッセージ終了
		mgr_work->flag |= GMD_SMSG_FLAG_END;
	}
	else {
		// ウィンドウサイズ設定
		mgr_work->win_per = (float)mgr_work->timer / (float)GSD_MAINSYS_WIN_EFCT_FRAME;
	}
}

// ==========================================================================
// オブジェクト
// ==========================================================================
// ==========================================================================
// gmStartMsgObjPost
/*!
 *	ゲーム開始時メッセージ オブジェクトシステムPOST処理
 *
 *	@note
 *		g_obj.ppPost に登録
 */
// ==========================================================================
void gmStartMsgObjPost(void)
{
	s32					i;
	GMS_SMSG_MGR_WORK	*mgr_work;

	if (gm_start_msg_tcb == NULL) {
		return;
	}

	mgr_work = (GMS_SMSG_MGR_WORK*)mtTaskGetTcbWork(gm_start_msg_tcb);

	// 前処理登録
	ObjDraw3DNNUserFunc(
			gmStartMsgDrawWindowPre_DT, NULL, 0, OBD_DRAW_CMD_STATE_GMSG_MSG);

	// 専用描画ステート設定
	AoActSysSetDrawState(OBD_DRAW_CMD_STATE_GMSG_MSG);

	// 描画発行
	for (i = 0; i < gm_start_msg_ama_act_num_tbl[mgr_work->msg_type]; i++) {
		if (mgr_work->ama_2d_work[i] == NULL) {
			continue;
		}

		ObjDrawActionSummary(&mgr_work->ama_2d_work[i]->obj_work);
	}
#if _IPHONE
	for (i = 0; i < GMD_SMSG_AMA_ACT_ACTION_MAX; i++) {
		if (mgr_work->ama_2d_work_act[i] == NULL) {
			continue;
		}

		ObjDrawActionSummary(&mgr_work->ama_2d_work_act[i]->obj_work);
	}
#endif // _IPHONE

	// アクションのソート
	AoActSortExecute();

	// 描画
	AoActSortDraw();

	// 登録解除
	AoActSortUnregAll();

	// 描画ステート復帰
	AoActSysSetDrawState(OBD_DRAW_CMD_STATE_2DAMA);
}

// ==========================================================================
// gmStartMsgObjMain
/*!
 *	ゲーム開始時メッセージ オブジェクトメイン処理
 *
 *	@param	obj_work	[in]	OBS_OBJECT_WORK
 */
// ==========================================================================
void gmStartMsgObjMain(OBS_OBJECT_WORK *obj_work)
{
	if (gm_start_msg_end_state) {
		// 終了
		obj_work->flag |= OBD_OBJECT_TASKCLEAR;
		obj_work->disp_flag |= OBD_DISP_NODISP;
	}
}


// ==========================================================================
// ウィンドウ
// ==========================================================================
// ==========================================================================
// gmStartMsgDrawWindowPre_DT
/*!
 *	ウィンドウ描画用前処理
 *
 *	@param	param	[in]	空
 */
// ==========================================================================
void gmStartMsgDrawWindowPre_DT(void* param)
{
	UNREFERENCED_PARAMETER(param);
	AoActDrawPre();
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================

