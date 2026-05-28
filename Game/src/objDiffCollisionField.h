// ================================================================
/*!
  @file objDiffCollisionField.h
  @brief 差分テーブル地形取得 stafColからDS向けに引継ぎ

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objDiffCollisionField.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */
#ifndef _H_OBJDIFFCOLLSIONFIELD
#define _H_OBJDIFFCOLLSIONFIELD

#include "objCollision.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------
//----- External Declarations ------------------------------------------

// ================================================================
// ObjSetDiffCollision
/*!
  指定した地形情報アドレステーブルをチェックする地形に設定する
 
  @param pFat [in] OBS_DIFF_COLLISIONポインタ
 
 */
// ================================================================
void ObjSetDiffCollision( const OBS_DIFF_COLLISION* pFat );

// ================================================================
// ObjSetDiffCollision
/*!
  設定されている地形データポインタを取得
 
  @return OBS_DIFF_COLLISIONポインタ
 
 */
// ================================================================
const OBS_DIFF_COLLISION* ObjGetDiffCollision();

// ================================================================
// objFastCollision
/*!
  マップ当たりチェック

  @param pData [in] 地形チェックワークポインタ
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
  座標から指定方向に 1 キャラを調べ、接地面までの距離を求める

 */
// ================================================================
s32 ObjDiffCollisionFast( OBS_COL_CHK_DATA * pData );

// ================================================================
// ObjDiffCollision
/*!
  マップ当たりチェック

  @param pWork [io] オブジェクトワークポインタ
  @param pData [in] 地形チェックワークポインタ
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
  座標から指定方向に ３キャラを調べ、接地面までの距離を求める

 */
// ================================================================
s32 ObjDiffCollision( OBS_COL_CHK_DATA * pData );

// ================================================================
// ObjDiffCollisionDetFast
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
  座標から指定方向に 1 キャラを調べ、接地面までの距離を求める

 */
// ================================================================
s32 ObjDiffCollisionDetFast( s32 lPosX, s32 lPosY, u16 usFlag, u16 usVec, u16* pDir, u32* pAttr);

// ================================================================
// ObjDiffCollisionDet
/*!
  マップ当たりチェック 構造体用いず

  @param lPosX  [in] X座標 1:31
  @param lPosY  [in] Y座標 1:31
  @param usFlag [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param usVec  [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
  @param pDir   [out] 角度ポインタ NULLで設定を行わない 値は(0～2π を 0～256 に写像)
  @param pAttr  [out] 属性ポインタ NULLで設定を行わない
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
  座標から指定方向に ３キャラを調べ、接地面までの距離を求める

 */
// ================================================================
s32 ObjDiffCollisionDet(s32 lPosX, s32 lPosY, u16 usFlag, u16 usVec, u16* pDir, u32* pAttr);

// ================================================================
// ObjGetColDataDir
/*!
  指定した位置の角度を取得する
 
  @param lPosX     [in] X座標
  @param lPosY     [in] Y座標
  @param ucSuf     [in] BG面 
 
  @return   角度
 
 */
// ================================================================
u16 ObjGetColDataDir( s32 lPosY, s32 lPosX, u8 ucSuf );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _H_OBJDIFFCOLLSIONFIELD


/*
 * 角度のみ取得関数追加
 *
 * Revision 1.6  2005/08/11 13:33:09  use1146
 * 単純チェック準備
 *
 * Revision 1.5  2005/07/20 06:51:32  use1146
 * 型変更
 *
 * Revision 1.4  2005/06/10 11:10:03  use1146
 * 画面端可変対応
 *
 * Revision 1.3  2005/02/25 08:25:54  use1146
 * マップ外壁対応
 *
 * Revision 1.2  2005/02/21 09:16:34  use1146
 * 属性取得対応
 *
 * Revision 1.1  2005/01/20 03:09:29  use1146
 * 登録
 *
 */