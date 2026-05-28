// ================================================================
/*!
  @file objDiffCollisionObject.h
  @brief 地形オブジェクト、登録、差分取得、ライド設定

  @author mana
                Copyright(c) 2004 Dimps
  $Id: objDiffCollisionObject.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */


#ifndef _H_NLOBJDIFFCOLLISIONOBJECT
#define _H_NLOBJDIFFCOLLISIONOBJECT

#include "objObject.h"
#include "objCollision.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------


// OBS_COLLISION_OBJ::pDir
// #define OBD_COLOBJ_DIR_FLIP ( (u8*)0xffffffff ) // 角度データはなしでありながら、フリップに対応した角度設定をさせたい場合、pDirにこの値を設定する



//----- External Declarations ------------------------------------------

// ================================================================
// ObjCollisionObjectRegist
/*!
  地形オブジェクトを登録する
 
  @param pObj  [in] オブジェクトワークポインタ
  @param pDiff [in] 地形差分データ先頭アドレス
 
  @note
    #_obj_collision_tbl_nxのリストに登録し、次に ObjCollisionObjectClear() を呼んだ時、\n
    チェックするリストに移動する。\n
    チェックリストに入るのは登録した次のフレームからなので\n
    オブジェクト初期化時にも実行するとチェック漏れがなくなる。\n
 */
// ================================================================
void ObjCollisionObjectRegist( OBS_COLLISION_OBJ * pObj );

// ================================================================
// ObjCollisionObjectClear
/*!
  登録した地形オブジェクトをクリアする
 
  @note
  メインループ内の全ての地形判定が終わったあと実行する\n
  2回連続で関数を呼び出すとバックバッファもクリアされる
 */
// ================================================================
void ObjCollisionObjectClear();

// ================================================================
// ObjCollisionObjectCheck
/*!
  登録した地形オブジェクトとの差分値をチェックする
 
  @param pObj       [in] チェックするオブジェクトポインタ
  @param pData      [in] OBS_COL_CHK_DATAポインタ
 
  @note
  ヒットしたオブジェクトに対して、\n
  座標から指定方向に 1 キャラを調べ、接地面までの距離を求める
  
 */
// ================================================================
s32 ObjCollisionObjectCheck( OBS_OBJECT_WORK * pObj, const OBS_COL_CHK_DATA *pData );

// ================================================================
// ObjCollisionObjectCheckDet
/*!
  登録した地形オブジェクトとの差分値をチェックする
 
  @param pObj   [in] チェックするオブジェクトポインタ
  @param lPosX  [in] X座標 1:31
  @param lPosY  [in] Y座標 1:31
  @param usFlag [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param usVec  [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
  @param pDir   [out] 角度ポインタ NULLで設定を行わない 値は(0～2π を 0～256 に写像)
  @param pAttr  [out] 属性ポインタ NULLで設定を行わない
 
  @note
  ヒットしたオブジェクトに対して、\n
  座標から指定方向に 地形オブジェクトを調べ、接地面までの距離を求める
  
 */
// ================================================================
s32 ObjCollisionObjectCheckDet( OBS_OBJECT_WORK * pObj, s32 lPosX, s32 lPosY, u16 usFlag, u16 usVec, u16* pDir, u32* pAttr);

// ================================================================
// ObjCollisionObjectFastCheck
/*!
  登録した地形オブジェクトと指定座標との差分値をチェックする
 
  @param pData      [in] OBS_COL_CHK_DATAポインタ
 
  @note
  ヒットしたオブジェクトに対して、\n
  座標から指定方向に 地形オブジェクトを調べ、接地面までの距離を求める
  
 */
// ================================================================
s32 ObjCollisionObjectFastCheck( const OBS_COL_CHK_DATA *pData );

// ================================================================
// ObjCollisionObjectFastCheckDet
/*!
  登録した地形オブジェクトと指定座標との差分値をチェックする
 
  @param lPosX  [in] X座標 1:31
  @param lPosY  [in] Y座標 1:31
  @param usFlag [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param usVec  [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
  @param pDir   [out] 角度ポインタ NULLで設定を行わない 値は(0～2π を 0～256 に写像)
  @param pAttr  [out] 属性ポインタ NULLで設定を行わない
 
  @note
  ヒットしたオブジェクトに対して、\n
  座標から指定方向に 地形オブジェクトを調べ、接地面までの距離を求める
  
 */
// ================================================================
s32 ObjCollisionObjectFastCheckDet( s32 lPosX, s32 lPosY, u16 usFlag, u16 usVec, u16* pDir, u32* pAttr);

// ================================================================
// ObjCollisionDiffObjectGetCollisionObj
/*!
  登録されている地形オブジェクトワークの取得

  @param col_no    [in] 取得する地形オブジェクト情報NO(0～)
 
  @return   登録されている地形オブジェクト情報 NULLでそのNO以降の登録無し
 
  @note
	登録されている地形オブジェクト情報を取得します。\n
	登録されているNO以上が指定されているとNULLを返します。

 */
// ================================================================
extern OBS_COLLISION_OBJ* ObjCollisionDiffObjectGetCollisionObj(u8 col_no);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _H_NLOBJDIFFCOLLISIONOBJECT

/*
 * Revision 1.9  2005/09/27 07:02:49  use1146
 * フラグ修正
 *
 * Revision 1.8  2005/09/17 09:08:38  use1146
 * 角度設定可能状態追加
 *
 * Revision 1.7  2005/09/16 07:42:39  use1146
 * 地形オブジェクトチェック関数追加
 *
 * Revision 1.6  2005/08/08 09:07:32  use1146
 * オフセットなどをcharからshortへ変更
 *
 * Revision 1.5  2005/07/20 06:30:20  use1146
 * オブジェクト押し対応
 *
 * Revision 1.4  2005/03/08 11:11:52  use1146
 * RIDE作成
 *
 * Revision 1.3  2005/02/21 09:16:35  use1146
 * 属性取得対応
 *
 * Revision 1.2  2005/02/03 05:53:38  use1146
 * nxテーブルや、オフセットなどのバグ修正
 *
 * Revision 1.1  2005/01/20 03:09:28  use1146
 * 登録
 *
 */
