// ================================================================
/*!
  @file objBlockCollisionField.c
  @brief ブロック地形当たり判定

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objBlockCollisionField.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */

//----- Include Files --------------------------------------------------
#include "pch.h"
#include "objBlockCollisionField.h"
#include "objDiffCollisionField.h"

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------
#define OBD_MAP_BLOCK_SIZE ( 16 ) ///< 1ブロックのサイズ dot単位 2のべき乗である事
#define OBD_MAP_BLOCK_MASK ( OBD_MAP_BLOCK_SIZE - 1 ) ///< 1ブロックのマスク 


static s32 objBlockColLimit( const OBS_COL_CHK_DATA * pData );
static s32 objBlockColEmpty( const OBS_COL_CHK_DATA * pData );
static s32 objBlockColBlockFill( const OBS_COL_CHK_DATA * pData );
static s32 objBlockColBlockFillThrough( const OBS_COL_CHK_DATA * pData );

void ObjSetBlockCollision( const OBS_BLOCK_COLLISION* pCol );
s32 ObjBlockCollision( OBS_COL_CHK_DATA * pData );
static s32 objGetBlockColData( const OBS_COL_CHK_DATA * pData );

/// 地形の種類
typedef enum _OBE_BLOCK_COL_ID
{
    OBD_BLOCK_COL_EMPTY = 0,     ///< 空
    OBD_BLOCK_COL_FILL,          ///< ■ 地形
    OBD_BLOCK_COL_FILL_THROUGH,  ///< □ 地形、すり抜け属性
    
    OBD_BLOCK_COL_ID_MAX ///< 地形種類最大値
} OBE_BLOCK_COL_ID;

/// 当たりブロック関数テーブル
static s32 (*const _obj_block_collision_func[])( const OBS_COL_CHK_DATA * ) = 
{
    objBlockColEmpty,
    objBlockColBlockFill,
    objBlockColBlockFillThrough,
    //objBlockColBlockHalfDown,
    //objBlockColBlockHalfUp,
};

//----- External Declarations ------------------------------------------

//----- Static Declarations --------------------------------------------

//----- Global Variables -----------------------------------------------
/// ブロック当たり判定データポインタ
const OBS_BLOCK_COLLISION* _obj_bcol = NULL;

//----- Local Variables ------------------------------------------------

//----- Global Functions -----------------------------------------------

// ================================================================
// ObjSetBlockCollision
/*!
  指定した地形情報アドレステーブルをチェックする地形に設定する
 
  @param pFat [in] OBS_BLOCK_COLLISIONポインタ
 
 */
// ================================================================
void ObjSetBlockCollision( const OBS_BLOCK_COLLISION* pCol )
{
    _obj_bcol = pCol;
}

// ================================================================
// ObjSetBlockCollision
/*!
  現在設定されているデータポインタを返す
    
  @return OBS_BLOCK_COLLISIONポインタ
 */
// ================================================================
const OBS_BLOCK_COLLISION* ObjGetBlockCollision()
{
    return _obj_bcol;
}
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
s32 ObjBlockCollisionDet( s32 lPosX, s32 lPosY, u16 usFlag, u16 usVec, u16* pDir, u32* pAttr )
{
    OBS_COL_CHK_DATA tData;
    
    tData.pos_x  = lPosX;
    tData.pos_y  = lPosY;
    tData.dir   = pDir;
    tData.attr  = pAttr;
    tData.flag = usFlag;
    tData.vec  = usVec;
    
    return ObjBlockCollision(&tData); 
}
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
s32 ObjBlockCollision( OBS_COL_CHK_DATA * pData )
{
    u16  usDir; // 前角度保持
    u32  ulAttr; // 前属性保持
    s32 lCol = 0;
    s16 sOfstX = 0,sOfstY = 0;
    s16 ans = 1;

    MTM_ASSERT(pData);
    
    // 単純チェック
    if ( ! _obj_bcol->pData[0] ){
        switch ( pData->vec ){
        case OBD_COL_DOWN:
            lCol = _obj_bcol->bottom - pData->pos_y;
            break;
        case OBD_COL_UP:
            lCol = pData->pos_y - _obj_bcol->top;
            break;
        case OBD_COL_RIGHT:
            lCol = _obj_bcol->right - pData->pos_x;
            break;
        case OBD_COL_LEFT:
            lCol = pData->pos_x - _obj_bcol->left;
            break;
        }
        return (s32)MTM_MATH_CLIP(lCol, -31, 31);
    }

    // 前角度保持
    if ( pData->dir )
        usDir = *pData->dir;
    // 前属性保持
    if ( pData->attr )
        ulAttr = *pData->attr;
    
    // 地形差分値、角度を取得
    lCol = objGetBlockColData( pData );

    // 次のブロックまでの距離が返ってきた場合は、次のブロックとチェック
    if ( pData->vec & OBD_COL_Y ){
        ans = (s16)(pData->pos_y & OBD_MAP_BLOCK_MASK );
        if ( pData->vec & OBD_COL_MINUS )
            ans -= (s16)lCol;
        else 
            ans += (s16)lCol;
        if ( ans == 0 )
            sOfstY = -OBD_MAP_BLOCK_SIZE;
        else if ( ans == OBD_MAP_BLOCK_MASK )
            sOfstY =  OBD_MAP_BLOCK_SIZE;
    }
    else{
        ans = (s16)(pData->pos_x & OBD_MAP_BLOCK_MASK );
        if ( pData->vec & OBD_COL_MINUS )
            ans -= (s16)lCol;
        else 
            ans += (s16)lCol;
        if ( ans == 0 )
            sOfstX = -OBD_MAP_BLOCK_SIZE;
        else if ( ans == OBD_MAP_BLOCK_MASK )
            sOfstX =  OBD_MAP_BLOCK_SIZE;
    }
    
    if ( sOfstX || sOfstY ){
        u32 ulPrevAttr = 0;
        u16 usPrevDir = 0;

        if ( pData->dir )
            usPrevDir = *pData->dir;
        if ( pData->attr )
            ulPrevAttr = *pData->attr;
        
        // 次のブロックチェック
        pData->pos_x += sOfstX;
        pData->pos_y += sOfstY;
        lCol = objGetBlockColData( pData );
        pData->pos_x -= sOfstX;
        pData->pos_y -= sOfstY;
        
        if ( lCol >= 0 ){
            // 浮いていたので前ブロックの属性を再設定
            if ( pData->dir )
                *pData->dir = usPrevDir;
            if ( pData->attr )
                *pData->attr = ulPrevAttr;
        }
        // 補正
        if ( sOfstX < 0 )
            sOfstX += 1;
        if ( sOfstX > 0 )
            sOfstX -= 1;
        if ( sOfstY < 0 )
            sOfstY += 1;
        if ( sOfstY > 0 )
            sOfstY -= 1;
        
        if ( pData->vec & OBD_COL_MINUS )
            lCol -= sOfstX + sOfstY;
        else
            lCol += sOfstX + sOfstY;
    }    

    /*if ( lCol > 0) {
        // 地形にHITしなかったので角度を前の状態に戻す
        if ( pData->dir )
            *pData->dir = usDir;
        if ( pData->attr )
            *pData->attr = ulAttr;
    }*/
    
    return lCol;
}
//----- Local Functions ------------------------------------------------
// ================================================================
// objGetBlockColData
/*!
  指定した位置の地形データからを差分値等を取得する
 
  @param pData     [in] 判定元情報ポインタ
 
  @return   地形差分値
 
 */
// ================================================================
static s32 objGetBlockColData( const OBS_COL_CHK_DATA * pData )
{
    s32 lCol;   // 地形差分値
    s32 lPosX = 0,lPosY = 0;
//    s16 sOfstX = 0,sOfstY = 0;
    u32 ulBlockPos; // ブロック番号
    u16 usBlockNo; // ブロック番号
    
    if ( pData->flag & OBD_COL_LIMITWALL ){
        // マップ外を壁
        lCol = objBlockColLimit(pData);
        if ( lCol < 0 )
            return lCol;
    }else{
        // 座標クリッピング
        lPosX = (s32)MTM_MATH_CLIP(pData->pos_x, _obj_bcol->left, _obj_bcol->right - 1);
        lPosY = (s32)MTM_MATH_CLIP(pData->pos_y, _obj_bcol->top, _obj_bcol->bottom - 1);
    }
    
    // 当たりデータ番号を取得
    ulBlockPos = ((lPosY >> 4) * _obj_bcol->width) + (lPosX >> 4);
    usBlockNo = _obj_bcol->pData[pData->flag & OBD_COL_B][ulBlockPos];
    
    // 関数呼び出し
    lCol = _obj_block_collision_func[usBlockNo](pData);

    return lCol;
}
// ================================================================
// objBlockColLimit
/*!
  マップ端ブロック当たりチェック
 
  @param pData     [in] 判定元情報ポインタ
 
  @return   地形差分値

 */
// ================================================================
static s32 objBlockColLimit( const OBS_COL_CHK_DATA * pData )
{
    // 方向に合わせて距離計算
    switch( pData->vec ){
    case OBD_COL_DOWN:
        if ( (_obj_bcol->bottom - 1) < pData->pos_y )
            return (_obj_bcol->bottom - 1) - pData->pos_y;
    case OBD_COL_UP:
        if ( (_obj_bcol->top) > pData->pos_y )
            return pData->pos_y - _obj_bcol->top;
    case OBD_COL_RIGHT:
        if ( (_obj_bcol->right - 1) < pData->pos_x )
            return (_obj_bcol->right - 1) - pData->pos_x;
    case OBD_COL_LEFT:
        if ( (_obj_bcol->left) > pData->pos_x )
            return pData->pos_x - _obj_bcol->left;
            
    }
    // マップ端には当たらず
    return 1;
}
// ================================================================
// objBlockCalcEmpty
/*!
  当たりチェック（空）
 
  @param pData [in] チェック位置データ
 
  @return   地形差分値
 */
// ================================================================
static s32 objBlockCalcEmpty( const OBS_COL_CHK_DATA * pData )
{
    // 方向に合わせて距離計算
    switch( pData->vec ){
    case OBD_COL_DOWN:
        return OBD_MAP_BLOCK_MASK - (pData->pos_y & OBD_MAP_BLOCK_MASK);
    case OBD_COL_UP:
        return (pData->pos_y & OBD_MAP_BLOCK_MASK);
    case OBD_COL_RIGHT:
        return OBD_MAP_BLOCK_MASK - (pData->pos_x & OBD_MAP_BLOCK_MASK);
    case OBD_COL_LEFT:
        return (pData->pos_x & OBD_MAP_BLOCK_MASK);
    }
    // 向き不明
    return OBD_MAP_BLOCK_MASK;
}
// ================================================================
// objBlockCalcFill
/*!
  当たりチェック（満）
 
  @param pData [in] チェック位置データ
 
  @return   地形差分値
 */
// ================================================================
static s32 objBlockCalcFill( const OBS_COL_CHK_DATA * pData )
{
    // 方向に合わせて距離計算
    switch( pData->vec ){
    case OBD_COL_DOWN:
        return -(pData->pos_y & OBD_MAP_BLOCK_MASK);
    case OBD_COL_UP:
        return -(OBD_MAP_BLOCK_MASK - (pData->pos_y & OBD_MAP_BLOCK_MASK));
    case OBD_COL_RIGHT:
        return -(pData->pos_x & OBD_MAP_BLOCK_MASK);
    case OBD_COL_LEFT:
        return -(OBD_MAP_BLOCK_MASK - (pData->pos_x & OBD_MAP_BLOCK_MASK));
    }
    // 向き不明
    return OBD_MAP_BLOCK_MASK;

}
// ================================================================
// objBlockColEmpty
/*!
  ブロック当たりチェック（空）
 
  @param pData [in] チェック位置データ
 
  @return   地形差分値
 */
// ================================================================
static s32 objBlockColEmpty( const OBS_COL_CHK_DATA * pData )
{
    // 角度
    if ( pData->dir)
        *pData->dir = 0;

    // 属性
    //if ( pData->attr)
    //    *pData->attr = 0;
    
    return objBlockCalcEmpty(pData);
}
// ================================================================
// objBlockColBlockFill
/*!
  ブロック当たりチェック（四角）
 
  @param pData [in] チェック位置データ
 
  @return   地形差分値
 */
// ================================================================
static s32 objBlockColBlockFill( const OBS_COL_CHK_DATA * pData )
{
    // 角度
    if ( pData->dir)
        *pData->dir = 0;

    // 属性
    //if ( pData->attr)
    //    *pData->attr = 0;
    
    return objBlockCalcFill( pData );
}
// ================================================================
// objBlockColBlockFillThrough
/*!
  ブロックすり抜け当たりチェック（四角）
 
  @param pData [in] チェック位置データ
 
  @return   地形差分値
 */
// ================================================================
static s32 objBlockColBlockFillThrough( const OBS_COL_CHK_DATA * pData )
{
    // 角度
    if ( pData->dir)
        *pData->dir = 0;

    // 属性
    if ( pData->attr)
        *pData->attr |= OBD_COLAT_THROUGH;

    if ( pData->flag & OBD_COL_THROUGH )
        return objBlockCalcEmpty(pData);
    
    return objBlockCalcFill( pData );
}

