// ================================================================
/*!
  @file gmGmkFlagChange.h
  @brief フラグ切り替えギミック AB面や接地フラグなど

  @author mana
  @author modifier Ishizaki
                Copyright(c) 2005 Dimps
  $Id: gmGmkFlagChange.h 2 2011-04-11 05:21:26Z thamada $
 */
// ================================================================
/*
 * $Log: gmGmkFlagChange.h,v $
 * Revision 1.3  2007/05/28 03:18:33  use1081
 * グラインドIDマスクフラグ領域修整
 *
 * Revision 1.2  2007/03/05 05:55:58  use1081
 * 直値を定義に置き換え
 *
 * Revision 1.1  2006/04/19 09:38:38  use1081
 * 新規追加
 *
 *
 */

#ifndef GM_GMK_FLAG_CHANGE_H_
#define GM_GMK_FLAG_CHANGE_H_

//----- Include Files --------------------------------------------------
#include "gmEventTbl.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ----------------------------------------------------
// GMS_EVE_RECORD_EVENT : flag

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- External Declarations ------------------------------------------
// ================================================================
// GmGmkFlagChangeInit
/*!
  フラグ変更系ギミック初期化関数

  @param pEve   [io] レコードポインタ
  @param fPosX  [in] 出現座標
  @param fPosY  [in] 
  @param ucType [in] 処理内容タイプ 通常は0
 */
// ================================================================
OBS_OBJECT_WORK* GmGmkFlagChangeInit(GMS_EVE_RECORD_EVENT * pEve , s32 fPosX,  s32 fPosY, u8 ucType);

    
#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_GMK_FLAG_CHANGE_H_
