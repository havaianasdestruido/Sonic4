// ================================================================
/*!
  @file objDiffCollisionCheck.h
  @brief 地形判定 差分値テーブル地形用 判定ルーチン

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objDiffCollisionCheck.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */

#ifndef _H_OBJDIFFCOLLSIONCHECK
#define _H_OBJDIFFCOLLSIONCHECK

#include "objObject.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------

//----- External Declarations ------------------------------------------

// ================================================================
// ObjDiffCollisionEarthCheck
/*!
  オブジェクトの地面チェック 
 
  @param pWork [io] オブジェクトワークポインタ
 
  @note
    オブジェクトの地形矩形を元にオブジェクトを地面へ吸着します。\n
    オブジェクトの座標データを補正します。\n
    下り坂では速度に比例した差分値以上で地面から離れます。\n
    角度を用いるオブジェクトは角度情報も更新されます。\n
    内部で分岐する簡易チェックは未作成
 */
// ================================================================
void ObjDiffCollisionEarthCheck( OBS_OBJECT_WORK * pWork );


// ================================================================
// ObjCollisionUnion
/*!
  マップ当たりチェック 統合

  @param pWork [io] オブジェクトワークポインタ
  @param pData [in] 地形チェックワークポインタ
 
  @return 接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
    座標から指定方向に 地形をチェックし、地形とオブジェクト地形の接地面までの距離を求める
 */
// ================================================================
s32 ObjCollisionUnion( OBS_OBJECT_WORK * pWork, OBS_COL_CHK_DATA* pData );

// ================================================================
// ObjCollisionFastUnion
/*!
  簡易マップ当たりチェック 統合

  @param pData [in] 地形チェックワークポインタ
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
    座標から指定方向に 地形をチェックし、地形とオブジェクト地形の接地面までの距離を求める\n
    簡易チェックのため複雑な地形や速度で挙動がおかしくなる。

 */
// ================================================================
s32 ObjCollisionFastUnion( OBS_COL_CHK_DATA* pData );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _H_TESTCOLLSIONCHECK

/*
 * Revision 1.2  2005/09/16 07:42:40  use1146
 * 地形オブジェクトチェック関数追加
 *
 * Revision 1.1  2005/01/20 03:09:30  use1146
 * 登録
 *
 */