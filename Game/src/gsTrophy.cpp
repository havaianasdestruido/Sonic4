// =======================================================================
/*!
  @file	gsTrophy.cpp
  @brief 実績・トロフィーシステム

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gsTrophy.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "aoTrophy.h"
#include "aoAvatarAward.h"

#include "gsTrophy.h"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
#if _PS3 || _XBOX	// PS3,Xbox360のみ使用
static void gsTrophyResetAcquisitionTable(void);
static void gsTrophyTriggerAquisition(Uint32 trophy_no);
#if _XBOX
static void gsTrophyAvatarTriggerAquisition(Uint32 avaw_no);
#endif /* _XBOX */
#endif /* _PS3 || _XBOX */

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

#if _PS3 || _XBOX	// PS3,Xbox360のみ使用

//! 獲得済みフラグテーブル
static BOOL gs_trophy_acquisition_tbl[GSD_TROPHY_ITEM_NUM]	= {0};

#if _XBOX
static BOOL gs_trophy_avatar_acquisition_tbl[AOD_AVATARITEM_NUM]	= {0};
#endif /* _XBOX */

#if _XBOX
//! Xbox360用実績IDテーブル
const static Uint32 gs_trophy_achievement_id_tbl_xbox[GSD_TROPHY_ITEM_NUM]	= {
	ACHIEVEMENT_ACHIEVEMENT01,
	ACHIEVEMENT_ACHIEVEMENT02,
	ACHIEVEMENT_ACHIEVEMENT03,
	ACHIEVEMENT_ACHIEVEMENT04,
	ACHIEVEMENT_ACHIEVEMENT05,
	ACHIEVEMENT_ACHIEVEMENT06,
	ACHIEVEMENT_ACHIEVEMENT07,
	ACHIEVEMENT_ACHIEVEMENT08,
	ACHIEVEMENT_ACHIEVEMENT09,
	ACHIEVEMENT_ACHIEVEMENT10,
	ACHIEVEMENT_ACHIEVEMENT11,
	ACHIEVEMENT_ACHIEVEMENT12,
};
#endif /* _XBOX */

#endif /* _PS3 || _XBOX */


/*------ Global Functions ----------------------------------------------*/
// =======================================================================
// GsTrophyInit
/*!
  トロフィーシステム初期化
  
  @note
  獲得済み情報がクリアされます。
 */
// =======================================================================
void GsTrophyInit(void)
{
#if _PS3 || _XBOX	// PS3,Xbox360のみ使用
	// 獲得情報テーブルクリア
	gsTrophyResetAcquisitionTable();
#else
	;
#endif /* _PS3 || _XBOX */
}

// =======================================================================
// GsTrophyExit
/*!
  トロフィーシステム終了
 */
// =======================================================================
void GsTrophyExit(void)
{
	;
}

// =======================================================================
// GsTrophyResetForAccount
/*!
  アカウント毎リセット処理
  
  @note
  サインインしたアカウント毎に必要なリセット処理を行います。
  サインインしているアカウントが変わった際には必ず呼び出してください。
  （アカウントが変わっていない場合に呼んでも問題ありません）
 */
// =======================================================================
void GsTrophyResetForAccount(void)
{
	/* REMINDER:
	  本関数ではサインインしているアカウントが変わって「いない」場合に呼ばれても
	  問題ない処理のみ行うこと。
	 */
	
#if _PS3 || _XBOX	// PS3,Xbox360のみ使用
	// 獲得情報テーブルクリア
	gsTrophyResetAcquisitionTable();
#else
	;
#endif /* _PS3 || _XBOX */
}

// =======================================================================
// GsTrophyUpdateAcquisition
/*!
  トロフィー獲得更新
  
  @param trophy_id	[in]	トロフィー番号
  
  @note
  トロフィーの獲得処理を行います。
  GsTrophyInit()以降に同IDで本関数を呼び出し済みである場合は何もしません。
 */
// =======================================================================
void GsTrophyUpdateAcquisition(Uint32 trophy_id)
{
#if _PS3 || _XBOX	// PS3,Xbox360のみ使用
	
	// 獲得済みの場合はスキップ
	if (gs_trophy_acquisition_tbl[trophy_id]) {
		return;
	}
	else {
		// 獲得済み記録
		gs_trophy_acquisition_tbl[trophy_id]	= TRUE;
		// 獲得通知
		gsTrophyTriggerAquisition(trophy_id);
	}
#else
	UNREFERENCED_PARAMETER(trophy_id);
#endif /* _PS3 || _XBOX */
}

// =======================================================================
// GsTrophyIsAcquired
/*!
  獲得済み判定
  
  @param trophy_id	[in]	トロフィー番号
  
  @retval TRUE	獲得済み
  @retval FALSE	未獲得
  
  @note
  GsTrophyInit()呼び出し以降に獲得済みか判定します。
  基本的には、連続呼び出しを防ぐ等の目的のために使用するものであり、
  本関数が参照する獲得済み記録テーブルは任意のタイミングでクリアされている場合があります
  （サインインしているアカウントが変わった場合など）。
  そのため、ユーザに情報表示するためなどの目的で本関数を使用しないでください。
 */
// =======================================================================
BOOL GsTrophyIsAcquired(Uint32 trophy_id)
{
#if _PS3 || _XBOX	// PS3,Xbox360のみ使用
	if (gs_trophy_acquisition_tbl[trophy_id]) {
		return TRUE;
	}
	else {
		return FALSE;
	}
#else
	UNREFERENCED_PARAMETER(trophy_id);
	return TRUE;
#endif /* _PS3 || _XBOX */
}


// =======================================================================
// GsTrophyAvatarUpdateAcquisition
/*!
  アバターアワード獲得更新
  
  @param trophy_id	[in]	アバターアワード番号
  
  @note
  アバターアワードの獲得処理を行います。
  GsTrophyInit()以降に同IDで本関数を呼び出し済みである場合は何もしません。
 */
// =======================================================================
void GsTrophyAvatarUpdateAcquisition(Uint32 avaw_id)
{
#if _XBOX	// Xbox360のみ使用
	
	// 獲得済みの場合はスキップ
	if (gs_trophy_avatar_acquisition_tbl[avaw_id]) {
		return;
	}
	else {
		// 獲得済み記録
		gs_trophy_avatar_acquisition_tbl[avaw_id]	= TRUE;
		// 獲得通知
		gsTrophyAvatarTriggerAquisition(avaw_id);
	}
#else
	UNREFERENCED_PARAMETER(avaw_id);
#endif /* _XBOX */
}

// =======================================================================
// GsTrophyAvatarIsAcquired
/*!
  アバターアワード獲得済み判定
  
  @param avaw_id	[in]	アバターアワード番号
  
  @retval TRUE	獲得済み
  @retval FALSE	未獲得
  
  @note
  GsTrophyInit()呼び出し以降に獲得済みか判定します。
  基本的には、連続呼び出しを防ぐ等の目的のために使用するものであり、
  本関数が参照する獲得済み記録テーブルは任意のタイミングでクリアされている場合があります
  （サインインしているアカウントが変わった場合など）。
  そのため、ユーザに情報表示するためなどの目的で本関数を使用しないでください。
 */
// =======================================================================
BOOL GsTrophyAvatarIsAcquired(Uint32 avaw_id)
{
#if _XBOX	// Xbox360のみ使用
	if (gs_trophy_avatar_acquisition_tbl[avaw_id]) {
		return TRUE;
	}
	else {
		return FALSE;
	}
#else
	UNREFERENCED_PARAMETER(avaw_id);
	return TRUE;
#endif /* _XBOX */
}

/*------ Static Functions ----------------------------------------------*/
#if _PS3 || _XBOX	// PS3,Xbox360のみ使用

// =======================================================================
// gsTrophyResetAcquisitionTable
/*!
  獲得済み情報クリア
 
  @note
  ゲーム起動中のみ有効な獲得済みフラグテーブルをクリアします。
 */
// =======================================================================
void gsTrophyResetAcquisitionTable(void)
{
	amZeroMemory(gs_trophy_acquisition_tbl, sizeof(BOOL) * GSD_TROPHY_ITEM_NUM);
#if _XBOX
	amZeroMemory(gs_trophy_avatar_acquisition_tbl, sizeof(BOOL) * AOD_AVATARITEM_NUM);
#endif /* _XBOX */
}

// =======================================================================
// gsTrophyTriggerAquisition
/*!
  獲得通知
  
  @param torophy_no	[in]	トロフィー番号
  
  @note
  獲得をPFの実績・トロフィーシステムに通知します。
 */
// =======================================================================
void gsTrophyTriggerAquisition(Uint32 trophy_no)
{
#if _XBOX
	// Xbox360の場合は実績IDに変換
	AoTrophyAcquisition(gs_trophy_achievement_id_tbl_xbox[trophy_no]);
#else
	AoTrophyAcquisition(trophy_no);
#endif
}


#if _XBOX
// =======================================================================
// gsTrophyAvatarTriggerAquisition
/*!
  アバターアワード獲得通知
  
  @param avaw_no	[in]	アバターアワード番号
  
  @note
  獲得をアバターアワードのシステムに通知します。
 */
// =======================================================================
void gsTrophyAvatarTriggerAquisition(Uint32 avaw_no)
{
	AoAvatarAwardGetStart((AOE_AVATARITEM)avaw_no);
}
#endif /* _XBOX */

#endif /* _PS3 || _XBOX */


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
