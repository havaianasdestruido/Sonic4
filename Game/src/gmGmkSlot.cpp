// =======================================================================
/*!
	@file	gmGmkSlot.c
	@brief	ギミック スロット＠ゾーン２　ストッパーと連動

	@author ei-chi co.ltd
				Copyright(c) 2009 Dimps
	$Id: gmGmkSlot.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date:: 2011-04-11 14:21:26 +0900#$
  
 */
// =======================================================================
/*
 *	memo
 *		当選率の計算は、
 *		第一抽選　…　25/75であたりかはずれを抽選します
 *
 *		はずれ抽選…　はずれの場合、ハズレ２次抽選が行われ、
 *		　　　　　　　10/25でＪＰエッグマンすべりチャンスモードになります。
 *		　　　　　　　ノーマルハズレの場合、
 *		　　　　　　　[ＢＡＲ]以外の絵柄でランダムで決定します。
 *		　　　　　　　1/16のリールの停止マスを決め、
 *		　　　　　　　第一リールと第二リールの柄が並べば
 *		　　　　　　　リーチスロー回転を経てハズレ図柄で停止します。
 *
 *		あたり抽選…　あたりの場合、第二抽選を行います。
 *		　　　　　　　ＪＪＪ・・・ 2/100　はずれのたびに確率を加算。最大 20/100までアップ
 *		　　　　　　　エエエ・・・ 2/100　はずれのたびに確率を加算。最大 20/100までアップ
 *		　　　　　　　ソソソ・・・ 5/100　はずれのたびに確率を加算。最大 30/100までアップ
 *		　　　　　　　ＢＢＢ・・・ 5/100　はずれのたびに確率を加算。最大 40/100までアップ
 *		　　　　　　　リリリ・・・ 5/100　はずれのたびに確率を加算。最大100/100までアップ
 *		　　　　　　　Ｂ－－・・・上記の抽選に外れた場合すべて
 *
 *		さらに！
 *		停止時間内にボタンを押すことで目押しストップが可能です。
 */

// ----- Include Files ---------------------------------------（インクルード）
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmPlySeqGmk.h"
#include "gmPlayer.h"
#include "gmObjDef.h"
#include "gmGameDat.h"
#include "gmGameDBuild.h"

#include "gmEffectCmn.h"

#include "gmRing.h"

#include "gmSound.h"

#include "gmGmkSlot.h"

// データヘッダ
#include "common/model/gmk_slot_mdl.hmb"
#include "common/model/GMK_SLOT_MAT.HMB"



// ----- Struct Definitions --------------------------------------（型の宣言）



// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// -------------

// ----- Static Declarations -----------------（スタティック変数及び関数宣言）

// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// -------------

// ----- Static Variables --------------------（スタティック変数の定義：局所）
static OBS_ACTION3D_NN_WORK *gm_gmk_slot_obj_3d_list = NULL;

// ----- Macros ------------------------------------------------（マクロ定義）
#define	OBJWORK		COMWORK.obj_work
#define	chgf(f)		ppFunc = (f)
// -------------

// ----- Macro Functions -----------------------------------（処理マクロ定義）
// -------------

// ----- Definitions -------------------------------------------（定数の宣言）
// 調整項目



// ---------------------------------------------------------------------------
// オブジェクトタイプ
typedef enum tag_GME_GMK_TYPE{

	GME_GMK_TYPE_DUMMY = 0,
	GME_GMK_TYPE_MAX

}GME_GMK_TYPE;
// ---------------------------------------------------------------------------





// ----- Static Functions ----------------------（スタティック関数の定義）
// ギミックの本質部分

// ----- Struct Definitions --------------------------------------（型の宣言）
typedef enum
{
	SLOT_REEL_JP = 0,
	SLOT_REEL_EGG,
	SLOT_REEL_SONIC,
	SLOT_REEL_RING,
	SLOT_REEL_BAR,
	SLOT_REEL_NUM,

	SLOT_REEL_B__ = SLOT_REEL_NUM,
					//	Bが１つそろいます。
	SLOT_REEL_BB_,
					//	Bが２つそろいます。
					//	３つはそろわないようにします。
	SLOT_REEL_77_,
	SLOT_REEL_EE_,

	SLOT_REEL_MEOSHI_STOP

}SLOT_PROB;
#define	GMD_GMK_SLOT_REEL_NUM	(3)

typedef struct tag_GMS_GMK_SLOT_REEL_STATUS_WORK
{
	fx16	reel;				//	リール回転位置
	fx32	reel_spd;			//	回転速度
	fx32	reel_acc;			//	回転加速度

	int		reel_time;			//	各種待機時間
	int		reel_target_pos;	//	マーク位置
	int		reel_target_mark;	//	マーク

	int		reel_extime;		//	各種演出用
	int		reel_se;			//	SEコール確認用

}GMS_GMK_SLOT_REEL_STATUS_WORK;
typedef struct tag_GMS_GMK_SLOT_WORK
{
	GMS_ENEMY_3D_WORK	gmk_work;		//!< 敵・ギミックオブジェクト 3Dモデル使用 構造体
	GMS_GMK_SLOT_REEL_STATUS_WORK	reel_status[3];
	int								current_reel;

	int		slot_id;		//!< ストッパーとの関連付け

	int		timer;
	int		timer_next;
	int		timer_meoshi_wait;		//!< 目押しがユーザーの意図に反して連続で起こらないよう保険
	int		slot_step;		//!< 抽選ステップ
	int		slot_se;		//!< SEコール対象
#if _IPHONE
	int		slot_se_timer;  //!< SEコール待ち
#endif // _IPHONE

	int		suberi_cnt;			//	すべり有効入力用
	s32		suberi_input;		//	すべり有効入力用

	s16		prob[SLOT_REEL_NUM];
	s16		lotresult;
	BOOL	freestop;

	fx32	ppos_x,ppos_y;

}GMS_GMK_SLOT_WORK;
#define	OBJ_3D		gmk_work.obj_3d
#define	COMWORK		gmk_work.ene_com
#define	reel_L		reel[0]
#define	reel_C		reel[1]
#define	reel_R		reel[2]

#define	prob_jjj	prob[SLOT_REEL_JP]		//!< ジャックポット
#define	prob_eee	prob[SLOT_REEL_EGG]		//!< エッグマン
#define	prob_sss	prob[SLOT_REEL_SONIC]	//!< ソニック
#define	prob_rrr	prob[SLOT_REEL_RING]	//!< リング
#define	prob_bbb	prob[SLOT_REEL_BAR]		//!< バー

static GMS_PLAYER_WORK *slot_start_player;
static int slot_start_call;
static u32 rand_result;
// ---------------------------------------------------------------------------



// ===========================================================================
// gmGmkSlot*
/*!
	ギミック スロット＠ゾーン２　ストッパーと連動

	@note

 */
/*!
 *		はずれ抽選…　はずれの場合、停止図柄はまったくのランダムで決定します。
 *		　　　　　　　[ＢＡＲ]だけは外れるようになっています。
 *		　　　　　　　1/16のリールの停止マスを決め、
 *		　　　　　　　偶然図柄が並べば「やきもきはずれ演出」を行います。
 *		　　　　　　　実はリールすべりエッグマンはこちらでのみ有効。
 *
 *		あたり抽選…　あたりの場合、第二抽選を行います。
 *		　　　　　　　ＪＪＪ・・・ 2/100　はずれのたびに確率を加算。最大 20/100までアップ
 *		　　　　　　　エエエ・・・ 2/100　はずれのたびに確率を加算。最大 20/100までアップ
 *		　　　　　　　ソソソ・・・ 5/100　はずれのたびに確率を加算。最大 30/100までアップ
 *		　　　　　　　リリリ・・・ 5/100　はずれのたびに確率を加算。最大100/100までアップ
 *		　　　　　　　ＢＢＢ・・・ 5/100　はずれのたびに確率を加算。最大 40/100までアップ
 *		　　　　　　　Ｂ－－・・・上記の抽選に外れた場合すべて
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define		GMD_GMK_SLOT_PROB_JJJ		( 10)
#define		GMD_GMK_SLOT_PROB_EEE		( 10)
#define		GMD_GMK_SLOT_PROB_SSS		( 15)
#define		GMD_GMK_SLOT_PROB_RRR		( 15)
#define		GMD_GMK_SLOT_PROB_BBB		( 20)

#define		GMD_GMK_SLOT_1ST_LOT		( 25)	//	25/100でハズレ
#define		GMD_GMK_SLOT_EGG_LOT		( 10)	//	ハズレのうち、10/100でエッグマンすべりあり

#define		GMD_GMK_SLOT_PROB_JJJ_MAX	( GMD_GMK_SLOT_PROB_JJJ)
#define		GMD_GMK_SLOT_PROB_EEE_MAX	( GMD_GMK_SLOT_PROB_EEE)
#define		GMD_GMK_SLOT_PROB_SSS_MAX	( GMD_GMK_SLOT_PROB_SSS)
#define		GMD_GMK_SLOT_PROB_RRR_MAX	( GMD_GMK_SLOT_PROB_RRR)
#define		GMD_GMK_SLOT_PROB_BBB_MAX	( GMD_GMK_SLOT_PROB_BBB)

#define		GMD_GMK_SLOT_EGG_SUBERI		( 60)

// ---------------------------------------------------------------------------
static void gmGmkSlotStay(OBS_OBJECT_WORK *obj_work);
static void gmGmkSlotGameStart(OBS_OBJECT_WORK *obj_work);
static void gmGmkSlotGameHit(OBS_OBJECT_WORK *obj_work);
static void gmGmkSlotGameLose(OBS_OBJECT_WORK *obj_work);

// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
#define	GMD_GMK_SLOT_REEL_ALLMARK			(16)
#define	GMD_GMK_SLOT_REEL1KOMA_HEIGHT		(FX16_ONE)
#define	GMD_GMK_SLOT_REEL_MAX_SPEED			((GMD_GMK_SLOT_REEL1KOMA_HEIGHT*GMD_GMK_SLOT_REEL_ALLMARK)/112)
#define	GMD_GMK_SLOT_REEL_MIN_SPEED			(GMD_GMK_SLOT_REEL1KOMA_HEIGHT/12)//32)
#define	GMD_GMK_SLOT_REEL_EGG_SPEED			(GMD_GMK_SLOT_REEL1KOMA_HEIGHT/12)//40)
#define	GMD_GMK_SLOT_REEL_ACC				(GMD_GMK_SLOT_REEL1KOMA_HEIGHT/64)
#define	GMD_GMK_SLOT_REEL_BRAKE				(GMD_GMK_SLOT_REEL_ACC/8)///15)
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSlot_ReelControl
/*!
	ギミック ギミック スロット＠ゾーン２　ストッパーと連動　リールの管理

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSlot_ReelControl(GMS_GMK_SLOT_REEL_STATUS_WORK *preel)
{
	//	タイマー確認
	if( preel->reel_time > 0 )
		preel->reel_time -= 1;

	if( preel->reel_time <= 0 )
	{
		if( preel->reel_acc )
		{
			preel->reel_spd += preel->reel_acc;
			if( preel->reel_spd > GMD_GMK_SLOT_REEL_MAX_SPEED )
			{
				preel->reel_spd = GMD_GMK_SLOT_REEL_MAX_SPEED;
				preel->reel_acc = 0;
			}
			else if( preel->reel_spd < 0 )
			{
				preel->reel_spd = 0;
				preel->reel_acc = 0;
			}
		}
	}
	preel->reel += (fx16)preel->reel_spd;	//	16で自動でマスク
//	while( preel->reel >= GMD_GMK_SLOT_REEL_ALLMARK*GMD_GMK_SLOT_REEL1KOMA_HEIGHT )
//	{
//		preel->reel -= GMD_GMK_SLOT_REEL_ALLMARK*GMD_GMK_SLOT_REEL1KOMA_HEIGHT;
//	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSlotStart
/*!
	ギミック ギミック スロット＠ゾーン２　ストッパーと連動　サブ初期化

	@note

 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSlotStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SLOT_WORK *pwork = (GMS_GMK_SLOT_WORK*)obj_work;

	pwork->reel_status[0].reel = (fx16)((u16)(GMD_GMK_SLOT_REEL1KOMA_HEIGHT*15));
	pwork->reel_status[1].reel = (fx16)GMD_GMK_SLOT_REEL1KOMA_HEIGHT*0;
	pwork->reel_status[2].reel = (fx16)GMD_GMK_SLOT_REEL1KOMA_HEIGHT*1;

	pwork->reel_status[0].reel_spd = 0;
	pwork->reel_status[1].reel_spd = 0;
	pwork->reel_status[2].reel_spd = 0;

	pwork->reel_status[0].reel_acc = 0;
	pwork->reel_status[1].reel_acc = 0;
	pwork->reel_status[2].reel_acc = 0;

	pwork->reel_status[0].reel_time = 0;
	pwork->reel_status[1].reel_time = 0;
	pwork->reel_status[2].reel_time = 0;

	pwork->prob_jjj = GMD_GMK_SLOT_PROB_JJJ;
	pwork->prob_eee = GMD_GMK_SLOT_PROB_EEE;
	pwork->prob_sss = GMD_GMK_SLOT_PROB_SSS;
	pwork->prob_bbb = GMD_GMK_SLOT_PROB_BBB;
	pwork->prob_rrr = GMD_GMK_SLOT_PROB_RRR;

	obj_work->chgf(gmGmkSlotStay);
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSlotStay
/*!
	ギミック スロット＠ゾーン２　ストッパーと連動

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSlotStay(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SLOT_WORK *pwork = (GMS_GMK_SLOT_WORK*)obj_work;

	rand_result <<= 16;
	rand_result += mtMathRand();	//	乱数をまわす
	if( slot_start_call == pwork->slot_id )
	{
		gmGmkSlotGameStart(obj_work);
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSlotGameStart
/*!
	ギミック スロット＠ゾーン２　ストッパーと連動

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//#define MEOSHI
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef enum
{
	SLOT_STEP_INIT = 0,

	SLOT_STEP_ROLL1 = 10,	//第一リール(左)回転から停止へ
	SLOT_STEP_ROLL2 = 20,	//第二リール(中)回転から停止へ
	SLOT_STEP_ROLL3 = 30,	//第三リール(右)回転から停止へ

	SLOT_STEP_STOP_NORM = 40,	//リール停止ノーマル

	SLOT_STEP_STOP_HIT = 50,	//第三リール(右)当たり停止

	SLOT_STEP_STOP_EGG = 60,	//第三リール(右)エッグマン停止すべりあり
	SLOT_STEP_EGG_STOP = 69,

	SLOT_STEP_STOP_STOP = 70,	//停止演出
	SLOT_STEP_STOP_STOP_FORCE = 80,	//強制停止

	SLOT_STEP_MAX

}GME_GMK_SLOT_STEP;
#ifndef MEOSHI
#define		GMD_GMK_SLOT_STOP1_TIME			( 10)//30)		// 第一リール停止(減速開始)までの時間
#define		GMD_GMK_SLOT_STOP1_TIME_ADJ		(  1)//30)		// 120～+60まで調整がかかる
#define		GMD_GMK_SLOT_STOP2_TIME			(  0)
#define		GMD_GMK_SLOT_STOP2_TIME_ADJ		(  1)//10)		//  15～+15まで調整がかかる
#define		GMD_GMK_SLOT_STOP3_TIME			(  0)
#define		GMD_GMK_SLOT_STOP3_TIME_ADJ		(  1)//10)		//  15～+15まで調整がかかる
#define		GMD_GMK_SLOT_STOP3_FEABER_TIME	( 30)		//	ゆっくり回る時間
#else
#define		GMD_GMK_SLOT_STOP1_TIME			(300)		// 第一リール停止(減速開始)までの時間
#define		GMD_GMK_SLOT_STOP1_TIME_ADJ		( 30)		// 120～+60まで調整がかかる
#define		GMD_GMK_SLOT_STOP2_TIME			(  0)
#define		GMD_GMK_SLOT_STOP2_TIME_OSHI	(300)
#define		GMD_GMK_SLOT_STOP2_TIME_ADJ		( 10)		//  15～+15まで調整がかかる
#define		GMD_GMK_SLOT_STOP3_TIME			(  0)
#define		GMD_GMK_SLOT_STOP3_TIME_OSHI	(300)
#define		GMD_GMK_SLOT_STOP3_TIME_ADJ		( 10)		//  15～+15まで調整がかかる
#define		GMD_GMK_SLOT_STOP3_FEABER_TIME	( 30)		//	ゆっくり回る時間
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static u8 tbl_gmk_reel_mark[GMD_GMK_SLOT_REEL_NUM][GMD_GMK_SLOT_REEL_ALLMARK] =
{
	{						//		reel_target_pos
		SLOT_REEL_SONIC,	//  1	0
		SLOT_REEL_RING,		//  2	f
		SLOT_REEL_BAR,		//  3	e
		SLOT_REEL_SONIC,	//  4	d
		SLOT_REEL_EGG,		//  5	c
		SLOT_REEL_BAR,		//  6	b
		SLOT_REEL_JP,		//  7	a
		SLOT_REEL_RING,		//  8	9
		SLOT_REEL_SONIC,	//  9	8
		SLOT_REEL_BAR,		// 10	7
		SLOT_REEL_RING,		// 11	6
		SLOT_REEL_SONIC,	// 12	5
		SLOT_REEL_EGG,		// 13	4
		SLOT_REEL_JP,		// 14	3
		SLOT_REEL_BAR,		// 15	2
		SLOT_REEL_RING,		// 16	1
	},
	{
		SLOT_REEL_SONIC,	//  1	0
		SLOT_REEL_EGG,		//  2	f
		SLOT_REEL_BAR,		//  3	e
		SLOT_REEL_SONIC,	//  4	d
		SLOT_REEL_RING,		//  5	c
		SLOT_REEL_BAR,		//  6	b
		SLOT_REEL_SONIC,	//  7	a
		SLOT_REEL_RING,		//  8	9
		SLOT_REEL_JP,		//  9	8
		SLOT_REEL_RING,		// 10	7
		SLOT_REEL_SONIC,	// 11	6
		SLOT_REEL_BAR,		// 12	5
		SLOT_REEL_RING,		// 13	4
		SLOT_REEL_EGG,		// 14	3
		SLOT_REEL_JP,		// 15	2
		SLOT_REEL_BAR,		// 16	1
	},
	{
		SLOT_REEL_SONIC,	//  1	0
		SLOT_REEL_BAR,		//  2	f
		SLOT_REEL_SONIC,	//  3	e
		SLOT_REEL_RING,		//  4	d
		SLOT_REEL_BAR,		//  5	c
		SLOT_REEL_JP,		//  6	b
		SLOT_REEL_EGG,		//  7	a
		SLOT_REEL_SONIC,	//  8	9
		SLOT_REEL_RING,		//  9	8
		SLOT_REEL_BAR,		// 10	7
		SLOT_REEL_JP,		// 11	6
		SLOT_REEL_RING,		// 12	5
		SLOT_REEL_SONIC,	// 13	4
		SLOT_REEL_BAR,		// 14	3
		SLOT_REEL_RING,		// 15	2
		SLOT_REEL_EGG,		// 16	1
	},
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSlotGameStart_100(OBS_OBJECT_WORK *obj_work);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static s16 tbl_gmk_slot_prob[] =
{
		GMD_GMK_SLOT_PROB_JJJ,
		GMD_GMK_SLOT_PROB_EEE,
		GMD_GMK_SLOT_PROB_SSS,
		GMD_GMK_SLOT_PROB_RRR,
		GMD_GMK_SLOT_PROB_BBB,
};
static s16 tbl_gmk_slot_prob_max[] =
{
		GMD_GMK_SLOT_PROB_JJJ_MAX,
		GMD_GMK_SLOT_PROB_EEE_MAX,
		GMD_GMK_SLOT_PROB_SSS_MAX,
		GMD_GMK_SLOT_PROB_RRR_MAX,
		GMD_GMK_SLOT_PROB_BBB_MAX,
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static u32 getRand()
{
	rand_result = ((rand_result%13)*(0xffffffff/13))+(rand_result/13);
	return rand_result;
}
// + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
static void gmGmkSlotGameStart(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SLOT_WORK *pwork = (GMS_GMK_SLOT_WORK*)obj_work;

	pwork->slot_step = SLOT_STEP_INIT;
	pwork->current_reel = 0;

#if _IPHONE
	pwork->slot_se_timer = 0; // SE再生タイミング初期化
#endif // _IPHONE

	//	抽選
	int i;

	//	大きくあたりかハズレかを抽選
	int	rand = (int)(rand_result%100);
	rand -= GMD_GMK_SLOT_1ST_LOT;
	//rand = -GMD_GMK_SLOT_EGG_LOT;
	if( rand >= 0 )
	{
		rand = (rand*100)/(100-GMD_GMK_SLOT_1ST_LOT);
		//	第二抽選
		for( i = 0; i < SLOT_REEL_NUM; i++ )
		{
			rand -= pwork->prob[i];
			if( rand <= 0 )
			{
				pwork->prob[i] = tbl_gmk_slot_prob[i];
				break;
			}
			if( pwork->prob[i] < tbl_gmk_slot_prob_max[i] )
			{
				pwork->prob[i] += tbl_gmk_slot_prob[i];
				if( pwork->prob[i] > tbl_gmk_slot_prob_max[i] )
					pwork->prob[i] = tbl_gmk_slot_prob_max[i];
			}
		}
		pwork->lotresult = (s16)i;

		if( i != SLOT_REEL_B__ )
		{
			pwork->reel_status[0].reel_target_mark =
			pwork->reel_status[1].reel_target_mark =
			pwork->reel_status[2].reel_target_mark = i;
		}
		else
		{
			int b = 0;
			for( i = 0; i < GMD_GMK_SLOT_REEL_NUM; i++ )
			{
				pwork->reel_status[i].reel_target_mark = (int)(getRand()%SLOT_REEL_B__);
				if( pwork->reel_status[i].reel_target_mark == SLOT_REEL_BAR )
					b++;
			}
			int r = (int)(getRand()%GMD_GMK_SLOT_REEL_NUM);
			if( b <= 1 )
			{
				pwork->lotresult = SLOT_REEL_B__;
				if( b == 0 )
					pwork->reel_status[r].reel_target_mark = SLOT_REEL_BAR;
			}
			else if( b >= 2 )
			{
				pwork->lotresult = SLOT_REEL_BB_;
				if( b == GMD_GMK_SLOT_REEL_NUM )
					pwork->reel_status[r].reel_target_mark = (int)(getRand()%SLOT_REEL_BAR);	//	BAR以外に変更
			}
		}
	}
	else if( rand < -GMD_GMK_SLOT_EGG_LOT )
	{
		pwork->lotresult = -1;
		for( int i = 0; i < GMD_GMK_SLOT_REEL_NUM; i++ )
		{
			pwork->reel_status[i].reel_target_mark = (int)(getRand()%SLOT_REEL_BAR);	//	目標マークBAR以外
		}

		if( pwork->reel_status[0].reel_target_mark == pwork->reel_status[1].reel_target_mark )
		{
			if( pwork->reel_status[1].reel_target_mark == pwork->reel_status[2].reel_target_mark )
			{
				if( pwork->reel_status[0].reel_target_mark == SLOT_REEL_EGG )
				{
					pwork->lotresult = SLOT_REEL_EE_;
					//	すべりあり　エッグマン大ハズレ！
				}
				else
				{
					//	第三リールだけ外す
					int target;
					do{
						target = (int)(getRand()%SLOT_REEL_BAR);	//	目標マーク BAR以外
					}while( pwork->reel_status[0].reel_target_mark == target );
					pwork->reel_status[2].reel_target_mark = target;
				}
			}
		}
	}
	else
	{
		pwork->reel_status[0].reel_target_mark = pwork->reel_status[1].reel_target_mark =
			pwork->reel_status[2].reel_target_mark = SLOT_REEL_EGG;
					pwork->lotresult = SLOT_REEL_EE_;
					//	すべりあり　エッグマン大ハズレ！
	}

	// 停止リール位置をセット
	for( i = 0; i < GMD_GMK_SLOT_REEL_NUM; i++ )
	{
		u16 reelpos = (u16)getRand();
		do{
			reelpos = (u16)((reelpos+1)%GMD_GMK_SLOT_REEL_ALLMARK);
			pwork->reel_status[i].reel_target_pos = reelpos;
		}while( tbl_gmk_reel_mark[i][(GMD_GMK_SLOT_REEL_ALLMARK-reelpos)&0x0f] != pwork->reel_status[i].reel_target_mark );
	}

	pwork->freestop = FALSE;		//	自動停止
	gmGmkSlotGameStart_100(obj_work);
	obj_work->chgf(gmGmkSlotGameStart_100);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSlotGameStart_100(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SLOT_WORK *pwork = (GMS_GMK_SLOT_WORK*)obj_work;

	if( pwork->timer <= 0 )
	{
		GMS_GMK_SLOT_REEL_STATUS_WORK *preel;
		preel = &pwork->reel_status[pwork->current_reel];
		//	リール回転開始
		switch( pwork->slot_step )
		{
			case SLOT_STEP_INIT:
				pwork->reel_status[0].reel_time =  0;
				pwork->reel_status[1].reel_time = 15;
				pwork->reel_status[2].reel_time = 30;

				pwork->reel_status[0].reel_acc = GMD_GMK_SLOT_REEL_ACC;
				pwork->reel_status[1].reel_acc = GMD_GMK_SLOT_REEL_ACC;
				pwork->reel_status[2].reel_acc = GMD_GMK_SLOT_REEL_ACC;

				pwork->timer =  GMD_GMK_SLOT_STOP1_TIME;
				pwork->timer += mtMathRand()%GMD_GMK_SLOT_STOP1_TIME_ADJ;
				pwork->slot_step = SLOT_STEP_ROLL1;
				break;

			case SLOT_STEP_ROLL1:
				pwork->current_reel = 0;
#ifdef MEOSHI
				if( pwork->timer < 0 )
				{
					pwork->slot_step = SLOT_STEP_STOP_STOP_FORCE;	//第一リール(左)目押し
					pwork->timer_next = GMD_GMK_SLOT_STOP2_TIME_OSHI;
					pwork->timer_meoshi_wait = 10;
				}
				else
#endif//MEOSHI
				{
					pwork->slot_step = SLOT_STEP_STOP_NORM;	//第一リール(左)停止へ
					pwork->timer_next = GMD_GMK_SLOT_STOP2_TIME;
					pwork->freestop = TRUE;		//	自動停止
				}
				pwork->timer = 0;
				break;

			case SLOT_STEP_ROLL2:
				pwork->current_reel = 1;
#ifdef MEOSHI
				if( pwork->timer < 0 )
				{
					pwork->slot_step = SLOT_STEP_STOP_STOP_FORCE;	//第二リール(左)目押し
					pwork->timer_next = GMD_GMK_SLOT_STOP3_TIME_OSHI;
					pwork->timer_meoshi_wait = 10;
				}
				else
#endif//MEOSHI
				{
					pwork->slot_step = SLOT_STEP_STOP_NORM;	//第二リール(中)停止へ
					pwork->timer_next = GMD_GMK_SLOT_STOP3_TIME;
					pwork->freestop = TRUE;		//	自動停止
				}
				pwork->timer = 0;
				break;

			case SLOT_STEP_ROLL3:
				//	ここで当選や逆ＪＰのすべりやはずれのチェックを行い処理を分ける
				pwork->current_reel = 2;
				if( pwork->timer < 0 )
				{
					pwork->slot_step = SLOT_STEP_STOP_STOP_FORCE;	//第二リール(左)目押し
				}
				else
				{
					if( pwork->freestop )
					{
						//	非目押し
						if( pwork->lotresult < 0 || pwork->lotresult == SLOT_REEL_B__ || pwork->lotresult == SLOT_REEL_BB_ )
						{
							// 外れとBARは通常停止へ
							pwork->timer =  0;
							pwork->slot_step = SLOT_STEP_STOP_NORM;	//第三リール(右)停止へ
						}
						else
						{
							pwork->timer =  60;
							if( pwork->lotresult != SLOT_REEL_EE_ )
								// エッグマン以外の当たりはゆっくり停止
								pwork->slot_step = SLOT_STEP_STOP_HIT;	//第三リール(右)じっくり停止へ
							else
								// エッグマン３揃いは専用
								pwork->slot_step = SLOT_STEP_STOP_EGG;	//エッグマンすべりありへ
						}
					}
					else
					{
						pwork->timer =  0;
						pwork->slot_step = SLOT_STEP_STOP_NORM;	//第三リール(右)停止へ
					}
				}
				break;

			//---- リール減速 ------------
			int	reelpos;
			case SLOT_STEP_STOP_NORM:
#if 0			// 目標のリール位置と同じ目になったら減速し始める
				reelpos = (int)((u16)preel->reel / GMD_GMK_SLOT_REEL1KOMA_HEIGHT)%GMD_GMK_SLOT_REEL_ALLMARK;
				if( reelpos != preel->reel_target_pos)
					break;
#endif
				// 減速開始
				if( !pwork->freestop )
				{
					//	目押し
					preel->reel_spd = GMD_GMK_SLOT_REEL_MIN_SPEED;
					preel->reel_acc = 0;
					pwork->timer = 0;
					pwork->slot_step = SLOT_STEP_STOP_STOP+1;

					break;
				}
				else
				{
					preel->reel_acc = -GMD_GMK_SLOT_REEL_BRAKE;
					pwork->slot_step++;
				}
				//nobreak;
				case SLOT_STEP_STOP_NORM+1:
					if( preel->reel_spd <= GMD_GMK_SLOT_REEL_MIN_SPEED + preel->reel_acc )
					{
						preel->reel_spd = GMD_GMK_SLOT_REEL_MIN_SPEED;
						preel->reel_acc = 0;
						pwork->timer = 0;
						pwork->slot_step = SLOT_STEP_STOP_STOP;
					}
					break;


			case SLOT_STEP_STOP_HIT:
				preel->reel_acc = -GMD_GMK_SLOT_REEL_BRAKE/2;
				pwork->slot_step++;
				//nobreak;
				case SLOT_STEP_STOP_HIT+1:
					if( preel->reel_spd <= GMD_GMK_SLOT_REEL_MIN_SPEED + preel->reel_acc )
					{
						preel->reel_spd = GMD_GMK_SLOT_REEL_MIN_SPEED;
						preel->reel_acc = 0;
						pwork->timer = GMD_GMK_SLOT_STOP3_FEABER_TIME;
						pwork->slot_step = SLOT_STEP_STOP_STOP;
					}
					break;


			case SLOT_STEP_STOP_EGG:
				preel->reel_acc = -GMD_GMK_SLOT_REEL_BRAKE/2;
				pwork->slot_step++;
				//nobreak;
				case SLOT_STEP_STOP_EGG+1:
					if( preel->reel_spd <= GMD_GMK_SLOT_REEL_EGG_SPEED + preel->reel_acc )
					{
						// 減速しきったら停止判定へ
						preel->reel_spd = GMD_GMK_SLOT_REEL_EGG_SPEED;
						preel->reel_acc = 0;
						pwork->slot_step++;
						pwork->suberi_cnt = 0;
						pwork->suberi_input = 0;
					}
					break;

				case SLOT_STEP_STOP_EGG+2:
#if 0 // ユーザー操作によるEEE回避操作は無しに(10/04/19)
					if( (preel->reel/GMD_GMK_SLOT_REEL1KOMA_HEIGHT) != ((preel->reel+preel->reel_spd)/GMD_GMK_SLOT_REEL1KOMA_HEIGHT) )
						pwork->suberi_cnt = 0;

					{
						s32 old = pwork->suberi_input;
						pwork->suberi_input = (s32)GmPlayerKeyGetGimmickRotZ(slot_start_player);
						if( (old <= 0 && pwork->suberi_input > 0)|| (old >= 0 && pwork->suberi_input < 0) )
							pwork->suberi_cnt++;
					}
					reelpos = (int)((u16)preel->reel / GMD_GMK_SLOT_REEL1KOMA_HEIGHT)%GMD_GMK_SLOT_REEL_ALLMARK;
					if( reelpos == preel->reel_target_pos )
					{
						if( pwork->suberi_input >= GMD_GMK_SLOT_EGG_SUBERI )
						{
							//	１つずれる！
							preel->reel_target_pos = (preel->reel_target_pos+1)%GMD_GMK_SLOT_REEL_ALLMARK;
							pwork->slot_step = SLOT_STEP_STOP_STOP;
							pwork->lotresult = -1;
						}
						else
#else
					reelpos = (int)((u16)preel->reel / GMD_GMK_SLOT_REEL1KOMA_HEIGHT)%GMD_GMK_SLOT_REEL_ALLMARK;
					if( reelpos == preel->reel_target_pos )
					{
#endif
						{
							pwork->slot_step = SLOT_STEP_EGG_STOP;
							preel->reel = (fx16)(GMD_GMK_SLOT_REEL1KOMA_HEIGHT*reelpos);
							pwork->lotresult = SLOT_REEL_EGG;
						}
					}
					break;

			//---- 目的の絵柄でリール停止 ------------
			case SLOT_STEP_STOP_STOP:
			//	目的の絵柄までＷＡＩＴ
				if( ((u16)preel->reel/GMD_GMK_SLOT_REEL1KOMA_HEIGHT) == ((u16)(preel->reel - preel->reel_spd)/GMD_GMK_SLOT_REEL1KOMA_HEIGHT) )
					break;	//	前回と図柄に変更がなければ判定しない。(図柄戻りの阻止)

			case SLOT_STEP_STOP_STOP+1:
				reelpos = (int)((u16)preel->reel / GMD_GMK_SLOT_REEL1KOMA_HEIGHT)%GMD_GMK_SLOT_REEL_ALLMARK;
				if( reelpos != preel->reel_target_pos )
					break;
				preel->reel = (fx16)(GMD_GMK_SLOT_REEL1KOMA_HEIGHT*reelpos);
			//	nobreak;
			case SLOT_STEP_EGG_STOP:
				preel->reel_extime = 4 *2;
				// SE
				GmSoundPlaySE("Casino5");
				pwork->slot_step = SLOT_STEP_STOP_STOP+2;	//	がしゃこんへ
			//	nobreak;

				case SLOT_STEP_STOP_STOP+2:
					if( (preel->reel_extime & 0x01) == 0 )
					{
						if( preel->reel_extime == 2*2 )
							preel->reel_spd /= 2;
						preel->reel_spd = -preel->reel_spd;
					}
					preel->reel_extime -= 1;
					if( preel->reel_extime == 0 )
					{
						preel->reel = (fx16)(GMD_GMK_SLOT_REEL1KOMA_HEIGHT*preel->reel_target_pos);
						preel->reel_spd = 0;
						if( pwork->current_reel == 0 )
						{
							//第一リール(左)停止 → 第二リール(中)停止へ
							pwork->timer = pwork->timer_next;
							pwork->timer += mtMathRand()%GMD_GMK_SLOT_STOP2_TIME_ADJ;
							pwork->slot_step = SLOT_STEP_ROLL2;
						}
						else if( pwork->current_reel == 1 )
						{
							//第二リール(中)停止 → 第三リール(右)停止へ
							pwork->timer = pwork->timer_next;
							pwork->timer += mtMathRand()%GMD_GMK_SLOT_STOP3_TIME_ADJ;
							pwork->slot_step = SLOT_STEP_ROLL3;
						}
						else if( pwork->current_reel == 2 )
						{
							//	結果を再確認します
							if( pwork->lotresult == SLOT_REEL_MEOSHI_STOP )
							{
								if( pwork->reel_status[0].reel_target_mark == pwork->reel_status[1].reel_target_mark &&
								        pwork->reel_status[0].reel_target_mark == pwork->reel_status[2].reel_target_mark )
								{
									pwork->lotresult = (s16)(pwork->reel_status[0].reel_target_mark);		//	３つ揃う
								}
								else
								{
									//	１つであたりか２つ揃いであたりのＢＡＲをチェックする
									int i;
									int result = SLOT_REEL_B__;
									for( i = 0;  i < 3; i++ )
									{
										if( pwork->reel_status[i].reel_target_mark == SLOT_REEL_BAR )
										{
											pwork->lotresult = (s16)result;
											result += 1;
										}
									}
								}
							}
							if( pwork->lotresult < 0 || pwork->lotresult == SLOT_REEL_MEOSHI_STOP )
							{
								//	結果確定へ
								obj_work->chgf(gmGmkSlotGameLose);
							}
							else
							{
								//	結果確定へ
								obj_work->chgf(gmGmkSlotGameHit);
							}
						}
					}
					break;

			case SLOT_STEP_STOP_STOP_FORCE:
			//	目押しによる強制停止
				if( ((u16)preel->reel/GMD_GMK_SLOT_REEL1KOMA_HEIGHT) == ((u16)(preel->reel - preel->reel_spd)/GMD_GMK_SLOT_REEL1KOMA_HEIGHT) )
					break;	//	前回と図柄に変更がなければ判定しない。(図柄戻りの阻止)

			//	図柄がスイートスポットに来た
				preel->reel &= ~(FX16_ONE-1);	//	ポジションマスク
				preel->reel_target_pos = preel->reel;
				{
					int rl = (16-((u16)(preel->reel)/GMD_GMK_SLOT_REEL1KOMA_HEIGHT))&0x0f;
					preel->reel_target_mark = tbl_gmk_reel_mark[pwork->current_reel][rl];
				}
				preel->reel_extime = 4 *2;
				preel->reel_spd = GMD_GMK_SLOT_REEL_MIN_SPEED;
				// SE
				GmSoundPlaySE("Casino5");
				pwork->slot_step = SLOT_STEP_STOP_STOP_FORCE+1;	//	がしゃこんへ

				case SLOT_STEP_STOP_STOP_FORCE+1:
					if( (preel->reel_extime & 0x01) == 0 )
					{
						if( preel->reel_extime == 2*2 )
							preel->reel_spd /= 2;
						preel->reel_spd = -preel->reel_spd;
					}
					preel->reel_extime -= 1;
					if( preel->reel_extime == 0 )
					{
						preel->reel = (fx16)preel->reel_target_pos;
						preel->reel_spd = 0;
						if( pwork->current_reel == 0 )
						{
							//第二リール(中)停止へ
							pwork->timer = pwork->timer_next;
							pwork->timer += mtMathRand()%GMD_GMK_SLOT_STOP2_TIME_ADJ;
							pwork->slot_step = SLOT_STEP_ROLL2;
						}
						else if( pwork->current_reel == 1 )
						{
							//第三リール(中)停止へ
							pwork->timer = pwork->timer_next;
							pwork->timer += mtMathRand()%GMD_GMK_SLOT_STOP3_TIME_ADJ;
							pwork->slot_step = SLOT_STEP_ROLL3;
						}
						else if( pwork->current_reel == 2 )
						{
							//	結果を再確認します
							if( pwork->lotresult == SLOT_REEL_MEOSHI_STOP )
							{
								if( pwork->reel_status[0].reel_target_mark == pwork->reel_status[1].reel_target_mark &&
								        pwork->reel_status[0].reel_target_mark == pwork->reel_status[2].reel_target_mark )
								{
									pwork->lotresult = (s16)(pwork->reel_status[0].reel_target_mark);		//	３つ揃う
								}
								else
								{
									//	１つであたりか２つ揃いであたりのＢＡＲをチェックする
									int i;
									int result = SLOT_REEL_B__;
									for( i = 0;  i < 3; i++ )
									{
										if( pwork->reel_status[i].reel_target_mark == SLOT_REEL_BAR )
										{
											pwork->lotresult = (s16)result;
											result += 1;
										}
									}
								}
							}
							if( pwork->lotresult < 0 || pwork->lotresult == SLOT_REEL_MEOSHI_STOP )
							{
								//	結果確定へ
								obj_work->chgf(gmGmkSlotGameLose);
							}
							else
							{
								//	結果確定へ
								obj_work->chgf(gmGmkSlotGameHit);
							}
						}
					}
					break;
		}
	}
	else
	{
		pwork->timer -= 1;
		#ifdef MEOSHI
		if (!pwork->freestop)
		{
			if( pwork->timer_meoshi_wait <= 0 )
			{
				if (GmPlayerKeyCheckJumpKeyOn(slot_start_player) )
				{
					pwork->timer = -1;
					pwork->lotresult = SLOT_REEL_MEOSHI_STOP;
				}
			}
			else
			{
				pwork->timer_meoshi_wait -= 1;
			}
		}
		#endif
	}

	//	SEコール用
	{
#if _IPHONE
		if (pwork->slot_se_timer > 0) {
			pwork->slot_se_timer--;
		}
#endif // _IPHONE

		GMS_GMK_SLOT_REEL_STATUS_WORK *preel;
		int i;
		for( i = 0; i < GMD_GMK_SLOT_REEL_NUM; i++ )
		{
			preel = &pwork->reel_status[i];
			if( preel->reel_spd != 0 )
			{
				fx16 reel_se = (fx16)(preel->reel+preel->reel_spd);
				if( ((reel_se/FX16_ONE) != (preel->reel/FX16_ONE)) && ((reel_se&~(FX16_ONE-1)) != preel->reel_se) )
				{
#if _IPHONE
					// 待ち時間ではない
					if (pwork->slot_se_timer <= 0) {
						GmSoundPlaySE("Casino4");	// SE
						pwork->slot_se_timer = 3; // 再生3フレ待ち
					}
#else
					GmSoundPlaySE("Casino4");	// SE
#endif // _IPHONE
				}
				preel->reel_se = reel_se&~(FX16_ONE-1);
			}
		}
	}

	gmGmkSlot_ReelControl(&pwork->reel_status[0]);
	gmGmkSlot_ReelControl(&pwork->reel_status[1]);
	gmGmkSlot_ReelControl(&pwork->reel_status[2]);
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSlotGameHit
/*!
	ギミック スロット＠ゾーン２　ストッパーと連動

	@note
		スロットゲームの終了です。
		あたりました。
		lotresult の内容を確認して演出を行います。
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSlotGameHit_100(OBS_OBJECT_WORK *obj_work);
static void gmGmkSlotGameHit_200(OBS_OBJECT_WORK *obj_work);	//	エッグマンおおはずれ
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static s32 tbl_slot_bonus_ring[SLOT_REEL_BB_+1] =
{
	150,	//	SLOT_REEL_JP
	0,		//	SLOT_REEL_EGG
	30,		//	SLOT_REEL_SONIC
	10,		//	SLOT_REEL_RING
	20,		//	SLOT_REEL_BAR
	2,		//	SLOT_REEL_B__
	4,		//	SLOT_REEL_BB_
};
static s32 tbl_slot_bonus_score[SLOT_REEL_BB_+1] =
{
	50000,	//	SLOT_REEL_JP
	0,		//	SLOT_REEL_EGG
	30000,	//	SLOT_REEL_SONIC
	10000,	//	SLOT_REEL_RING
	20000,	//	SLOT_REEL_BAR
	2000,	//	SLOT_REEL_B__
	4000,	//	SLOT_REEL_BB_
};
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSlotGameHit(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SLOT_WORK *pwork = (GMS_GMK_SLOT_WORK*)obj_work;
	MTM_ASSERT( slot_start_call == pwork->slot_id );
	MTM_ASSERT( pwork->lotresult <= SLOT_REEL_BB_);							// 意図せぬ値はアサート

	if( pwork->lotresult != SLOT_REEL_EGG )
	{
		GmRingSlotSetNum( slot_start_player, tbl_slot_bonus_ring[pwork->lotresult]);
		obj_work->chgf(gmGmkSlotGameHit_100);
	}
	else
	{
		//	エフェクト生成
		GmEfctCmnEsCreate( &slot_start_player->obj_work, GME_EFCT_CMN_IDX_TOGEBALL);
		obj_work->chgf(gmGmkSlotGameHit_200);
		pwork->timer = 30;	//	メダル減り始めるまで
	}
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSlotGameHit_100(OBS_OBJECT_WORK *obj_work)
{
	if( GmRingCheckRestSlotRing() )
		return;

	GMS_GMK_SLOT_WORK *pwork = (GMS_GMK_SLOT_WORK*)obj_work;
	MTM_ASSERT(slot_start_player);
	GmPlayerAddScore( slot_start_player, tbl_slot_bonus_score[pwork->lotresult], slot_start_player->obj_work.pos.x, slot_start_player->obj_work.pos.y );

	slot_start_call = -1;
	obj_work->chgf(gmGmkSlotStay);
}
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
static void gmGmkSlotGameHit_210(OBS_OBJECT_WORK *obj_work);
// + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
static void gmGmkSlotGameHit_200(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SLOT_WORK *pwork = (GMS_GMK_SLOT_WORK*)obj_work;

	pwork->timer -= 1;
	if( pwork->timer <= 0 )
	{
		pwork->ppos_x = slot_start_player->obj_work.pos.x;
		pwork->ppos_y = slot_start_player->obj_work.pos.y;

		pwork->timer = 100;	//	メダル減る
		obj_work->chgf(gmGmkSlotGameHit_210);
	}
}
// + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
static s16 tbl_dam_ofst_xy[][MTD_XY] =
{
	{  1,  0 },
	{  0,  0 },
	{  0,  1 },
	{  0,  0 },
	{  0, -1 },
	{  0,  0 },
	{  0, +1 },
	{  0,  0 },
};
#define	GMD_SZ_TBL_DAM_OFST		(sizeof(tbl_dam_ofst_xy)/(sizeof(s16)*2))
static void gmGmkSlotGameHit_210(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SLOT_WORK *pwork = (GMS_GMK_SLOT_WORK*)obj_work;

	//	ダメージ
	slot_start_player->obj_work.pos.x = pwork->ppos_x+(tbl_dam_ofst_xy[pwork->timer%GMD_SZ_TBL_DAM_OFST][MTD_X]<<FX32_SHIFT);
    slot_start_player->obj_work.pos.y = pwork->ppos_y+(tbl_dam_ofst_xy[pwork->timer%GMD_SZ_TBL_DAM_OFST][MTD_Y]<<FX32_SHIFT);

	//	リングいっこへらす
	GmPlayerRingDec(slot_start_player, 1);

	// ダメージSEコール
#if _IPHONE
	if (!(pwork->timer % 12)) {		// 12Frameに１回くらい
#else
	if (!(pwork->timer % 6)) {		// 6Frameに１回くらい
#endif // _IPHONE
		GmSoundPlaySE("Damage2");	// 針SE使いまわし
	}

	pwork->timer -= 1;
	if( pwork->timer <= 0 )
	{
		slot_start_player->obj_work.pos.x = pwork->ppos_x;
    	slot_start_player->obj_work.pos.y = pwork->ppos_y;

		slot_start_call = -1;
		obj_work->chgf(gmGmkSlotStay);
	}
}
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// gmGmkSlotGameLose
/*!
	ギミック スロット＠ゾーン２　ストッパーと連動

	@note
		スロットゲームの終了です。
		はずれでした。
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSlotGameLose(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SLOT_WORK *pwork = (GMS_GMK_SLOT_WORK*)obj_work;
	UNREFERENCED_PARAMETER(pwork);
	MTM_ASSERT( slot_start_call == pwork->slot_id );

	slot_start_call = -1;
	obj_work->chgf(gmGmkSlotStay);
}
// ---------------------------------------------------------------------------
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================





// ==========================================================================
// gmGmkSlotRod*
/*!
	ギミック ギミック スロット＠ゾーン２　ストッパーと連動

	@note
 */
// --------------------------------------------------------------------------
typedef struct tag_GMS_GMK_SLOTPARTS_WORK
{
	GMS_EFFECT_3DNN_WORK eff_work;
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。
	GMS_GMK_SLOT_WORK	*slot_work;

	int					reel_id;

	float				tex_v;	//仮

}GMS_GMK_SLOTPARTS_WORK;
#define	OBJ_3D		eff_work.obj_3d
#define	COMWORK		eff_work.efct_com
// ---------------------------------------------------------------------------
static void gmGmkSlotReel(OBS_OBJECT_WORK *obj_work);
// --------------------------------------------------------------------------



// ---------------------------------------------------------------------------
// gmGmkSlot_CreateReel
/*!
	ギミック スロット　リール生成

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static u16 tbl_gmk_slot_reelmodel_id[] =
{
	IDB_GMK_SLOT_MDL_GMK_SLOT_REEL_L_ZNO,
	IDB_GMK_SLOT_MDL_GMK_SLOT_REEL_S_ZNO,
	IDB_GMK_SLOT_MDL_GMK_SLOT_REEL_R_ZNO,
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSlot_CreateReel(GMS_GMK_SLOT_WORK *pwork)
{
	GMS_GMK_SLOTPARTS_WORK *parts;
	OBS_OBJECT_WORK *parent_obj;
	OBS_OBJECT_WORK *obj_work;

	parent_obj = (OBS_OBJECT_WORK*)pwork;
	//	リール
	for( int i = 0; i < GMD_GMK_SLOT_REEL_NUM; i++ )
	{
		obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_SLOTPARTS_WORK),
		                                  NULL,
		                                  0,
		                                 "Gmk_SlotReel");
		parts =(GMS_GMK_SLOTPARTS_WORK*)obj_work;
		// モデル初期化
		ObjObjectCopyAction3dNNModel(obj_work,
		                             &gm_gmk_slot_obj_3d_list[tbl_gmk_slot_reelmodel_id[i]],
		                             &parts->eff_work.obj_3d);
		obj_work->parent_obj = parent_obj;

		obj_work->pos.x = parent_obj->pos.x +((48*i)-48)*FX32_ONE;
		obj_work->pos.y = parent_obj->pos.y;
		obj_work->pos.z = parent_obj->pos.z;
//		obj_work->dir.z = parent_obj->dir.z;
		// フラグ
		obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_TEXOFFSET;	// UVスクロール使用 20090924 Dimps Ishizaki
		obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;		// 佇む
		obj_work->flag |= OBD_OBJECT_NOHIT;				// 矩形あたり無し◆
		obj_work->move_flag |= OBD_MOVE_NOCOL;			// 移動無し 地形あたりチェック無し
		// 描画フラグ
		obj_work->disp_flag &= ~OBD_DISP_NODIR;
		obj_work->disp_flag |= (OBD_DISP_NODIRFLIP|OBD_DISP_DRAWSTATE);
		obj_work->chgf(gmGmkSlotReel);

		parts->reel_id = i;
		parts->slot_work = pwork;
	}
}
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static s16 tbl_reel_tex_u[4] =
{
	2,	//  0- 1- 2- 3- 4
	1,	//  5- 6- 7- 8- 9
	0,	// 10-11-12-13-14
	3,	// 15-
};
static s16 tbl_reel_tex_v[4][5] =
{
//     0- 1- 2- 3- 4- 
	{  2, 3, 4, 5, 6 },	// 0-
	{  2, 3, 4, 5, 6 },	// 5-
	{  2, 3, 4, 5, 6 },	//10-
	{  6, 0, 0, 0, 0 },
};
static void gmGmkSlotReel(OBS_OBJECT_WORK *obj_work)
{
	GMS_GMK_SLOTPARTS_WORK *pwork = (GMS_GMK_SLOTPARTS_WORK*)obj_work;

	u16 tex;
	s32 tex_v;
	s32 tex_u;

	tex = (u16)(((u16)pwork->slot_work->reel_status[pwork->reel_id].reel)>>FX32_SHIFT);
	tex_u = tbl_reel_tex_u[tex/5];
	tex_v = tbl_reel_tex_v[tex_u][tex%5]<<FX16_SHIFT;
	tex_v += pwork->slot_work->reel_status[pwork->reel_id].reel&(FX16_ONE-1);

	float tu,tv;

	tv = (float)tex_v /(8.0f*(FX32_ONE));
#if _IPHONE
	tu = (float)tex_u /(8.0f);
#else
	tu = (float)tex_u /(4.0f);
#endif // _IPHONE

	pwork->OBJ_3D.draw_state.texoffset[0].v = -tv;
	pwork->OBJ_3D.draw_state.texoffset[0].u =  tu;
}
// ==========================================================================
#undef	OBJ_3D
#undef	COMWORK
// ==========================================================================



#if 0
// ==========================================================================
// gmGmkSlotRod*
/*!
	ギミック ギミック スロット＠ゾーン２　ボーナスリングと罰ゲームトゲトゲ

	@note
 */
// --------------------------------------------------------------------------
typedef struct tag_GMS_GMK_SLOTBONUS_WORK
{
	GMS_EFFECT_3DNN_WORK eff_work;
										//		gmk_work の先頭にOBS_OBJECT_WORK が含まれます。

	fx32	width;
	fx32	height;
	fx32	angle;

	int		num;

}GMS_GMK_SLOTBONUS_WORK;
#define	OBJ_3D		eff_work.obj_3d
#define	COMWORK		eff_work.efct_com
// ---------------------------------------------------------------------------



// --------------------------------------------------------------------------
// gmGmkSlotBonusCreate
/*!
	ボーナスリングを生成します。
	一度に呼べる個数は４です。

	@note
 */
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static void gmGmkSlotBonusCreate(int num)
{
	GMS_GMK_SLOTBONUS_WORK *pwork;
	OBS_OBJECT_WORK *obj_work;

	obj_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_GMK_SLOTBONUS_WORK),
	                                  NULL,
	                                  0,
	                                 "Gmk_SlotBonusRing");
	pwork =(GMS_GMK_SLOTBONUS_WORK*)obj_work;
		// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_slot_obj_3d_list[tbl_gmk_slot_reelmodel_id[i]],
	                             &parts->eff_work.obj_3d);
		obj_work->parent_obj = parent_obj;

		obj_work->pos.x = parent_obj->pos.x;/* +((48*i)-48)*FX32_ONE;*/
		obj_work->pos.y = parent_obj->pos.y;
		obj_work->pos.z = parent_obj->pos.z;
//		obj_work->dir.z = parent_obj->dir.z;
		// フラグ
		obj_work->flag &= ~OBD_OBJECT_PARENT_FIX;		// 佇む
		obj_work->flag |= OBD_OBJECT_NOHIT;				// 矩形あたり無し◆
		obj_work->move_flag |= OBD_MOVE_NOCOL;			// 移動無し 地形あたりチェック無し
		// 描画フラグ
		obj_work->disp_flag &= ~OBD_DISP_NODIR;
		obj_work->disp_flag |= (OBD_DISP_NODIRFLIP|OBD_DISP_DRAWSTATE);
		obj_work->chgf(gmGmkSlotReel);

		parts->reel_id = i;
		parts->slot_work = pwork;
	}



}
// --------------------------------------------------------------------------
// ==========================================================================
#endif




// ----- Global Functions ----------------------（グローバル関数の定義：外部）
// ==========================================================================
// GmGmkSlotStartRequest
/*!
	ギミック スロット＠ゾーン２　ストッパーと連動

	@note
		ストッパーからスロットゲームの開始を要求します
		安全のため戻り値がFALSEだった場合、スロットの起動に失敗のため
		ソニックを開放しなくてはなりません。
		引数にスロットのＩＤを指定してください。
 */
// ==========================================================================
BOOL GmGmkSlotStartRequest(int slot_id, GMS_PLAYER_WORK *ply_work)
{
	if( slot_start_call != -1 )
		return FALSE;

	//	スロット待機中　起動成功
	slot_start_call = slot_id;
	slot_start_player = ply_work;
	return TRUE;
}
// ==========================================================================


// ==========================================================================
// GmGmkSlotIsStatus
/*!
	ギミック スロット＠ゾーン２　ストッパーと連動

	@note
		スロット要求に成功した後の、スロットの終了判定となります。
		引数のidと違うidになった場合は何らかのエラーです。

 */
// ==========================================================================
BOOL GmGmkSlotIsStatus(int slot_id)
{
	if( slot_start_call == -1 )
	{
		//	スロット回転終了
		return TRUE;
	}

	MTM_ASSERT(slot_start_call == slot_id);
	UNREFERENCED_PARAMETER(slot_id);

	return FALSE;
}
// ==========================================================================


// ==========================================================================
// GmGmkSlot?Init
/*!
 *	ギミック スロット＠ゾーン２　ストッパーと連動 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標X
 *	@param pos_y	[in] 出現座標Y
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkSlotInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_GMK_SLOT_WORK	*pwork;

	OBS_OBJECT_WORK		*obj_work;
	GMS_ENEMY_3D_WORK	*gmk_work;

	UNREFERENCED_PARAMETER(type);

	pwork = (GMS_GMK_SLOT_WORK*)GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_GMK_SLOT_WORK), "Gmk_Slot");
	obj_work = (OBS_OBJECT_WORK*)pwork /*&pwork->gmk_work.ene_com.obj_work*/;
	gmk_work = (GMS_ENEMY_3D_WORK*)pwork /*&pwork->gmk_work*/;
	// モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
	                             &gm_gmk_slot_obj_3d_list[IDB_GMK_SLOT_MDL_GMK_SLOT_ZNO],
	                             &gmk_work->obj_3d);
	// マテリアルモーション
	ObjAction3dNNMaterialMotionLoad(&gmk_work->obj_3d, 0/*reg_file_id*/,
								NULL/*data_work*/, NULL/*filename*/,
								IDB_GMK_SLOT_MAT_GMK_SLOT_ZNV/*index*/, ObjDataGet(GMD_DWORK_NO_GMK_SLOT_MAT)->pData,
								1/*motion_num*/, 1/*mmotion_num*/);
	ObjDrawAction3dActionSet3DNNMaterial(&gmk_work->obj_3d, 0);
	//	こいつは筐体

	// 優先設定
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK - FX32_ONE;

	// 移動フラグ
	obj_work->move_flag |= OBD_MOVE_NOCOL;							// 移動無し 地形あたりチェック無し
	// 描画フラグ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP | OBD_DISP_REPEAT;

	//	設定の反映
	pwork->slot_id = eve_rec->left;		//	IDの取得
	if ( slot_start_call == 0 )
		slot_start_call = -1;

	//	リールを生成
	gmGmkSlot_CreateReel(pwork);

	gmGmkSlotStart(obj_work);

	return obj_work;
}
// ==========================================================================



// ===========================================================================
// GmGmkSlotBuild
/*!
	ギミック スロット＠ゾーン２　ストッパーと連動 データロード
	
	
	@note

 */
// ===========================================================================
void GmGmkSlotBuild(void)
{
	gm_gmk_slot_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SLOT_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SLOT_TEX),
								0/*draw_flag*/);
	slot_start_call = 0;
}
// ===========================================================================


// ===========================================================================
// GmGmkSlotFlush
/*!
	ギミック スロット＠ゾーン２　ストッパーと連動 データ破棄
	
	
	@note

 */
// ===========================================================================
void GmGmkSlotFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_SLOT_MODEL);
	GmGameDBuildRegFlushModel(gm_gmk_slot_obj_3d_list, amb->file_num);
}
// ===========================================================================
