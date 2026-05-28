// ================================================================
/*!
  @file objBlockCollisionField.h
  @brief ブロック地形当たり判定

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objBlockCollisionField.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */
#ifndef _H_OBJBLOCKCOLLSIONFIELD
#define _H_OBJBLOCKCOLLSIONFIELD

#include "objObject.h"
#include "objCollision.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------

//----- External Declarations ------------------------------------------


// ================================================================
// ObjSetBlockCollision
/*!
  指定した地形情報アドレステーブルをチェックする地形に設定する
 
  @param pFat [in] OBS_BLOCK_COLLISIONポインタ
 
 */
// ================================================================
void ObjSetBlockCollision( const OBS_BLOCK_COLLISION* pCol );

// ================================================================
// ObjSetBlockCollision
/*!
  現在設定されているデータポインタを返す
    
  @return OBS_BLOCK_COLLISIONポインタ
 */
// ================================================================
const OBS_BLOCK_COLLISION* ObjGetBlockCollision();

// ================================================================
// ObjBlockCollision
/*!
  マップ当たりチェック

  @param pData     [io] 判定元情報ポインタ
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
    座標から指定方向に 2ブロック(32dot)分調べ、接地面までの距離を求める\n

 */
// ================================================================
s32 ObjBlockCollision( OBS_COL_CHK_DATA * pData );

// ================================================================
// ObjBlockCollisionDet
/*!
  マップ当たりチェック 構造体を用いない

  @param lPosX  [in] X座標 1:31
  @param lPosY  [in] Y座標 1:31
  @param usFlag [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param usVec  [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
  @param pDir   [out] 角度ポインタ NULLで設定を行わない 値は(0～2π を 0～256 に写像)
  @param pAttr  [out] 属性ポインタ NULLで設定を行わない
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
    座標から指定方向に 2ブロック(32dot)分調べ、接地面までの距離を求める\n
 */
// ================================================================
s32 ObjBlockCollisionDet( s32 lPosX, s32 lPosY, u16 usFlag, u16 usVec, u16* pDir, u32* pAttr );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _H_OBJBLOCKCOLLSIONFIELD

