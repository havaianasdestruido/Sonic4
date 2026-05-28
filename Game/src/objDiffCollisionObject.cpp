// ================================================================
/*!
  @file objDiffCollisionObject.c
  @brief 地形オブジェクト、登録、差分取得、ライド設定
  @author mana
                Copyright(c) 2004 Dimps

  $Id: objDiffCollisionObject.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */

//----- Include Files --------------------------------------------------
#include "pch.h"
#include "objDiffCollisionObject.h"
//#include "objDiffCollisionField.h"
#include "gsMainSys.h"
#include "gmPlayer.h"

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------
//#define OBD_COLLISION_OBJECT_MAX ( 32 ) ///< 地形オブジェクト最大登録数（越えた場合はアサート＋無視）
#define OBD_COLLISION_OBJECT_MAX ( 144 ) ///< 地形オブジェクト最大登録数（越えた場合はアサート＋無視）

#define OBD_COL_THROUGH       ( 0x80) ///< サーフェイスに一時的に設定

#define OBD_COLOBJ_OFFSET		( 24 )     ///< チェック範囲オフセット
#define OBD_COLOBJ_OFFSET_SS	( 24 )     ///< チェック範囲オフセット(スペステ）

//----- External Declarations ------------------------------------------

//----- Static Declarations --------------------------------------------

//----- Global Variables -----------------------------------------------

//----- Local Variables ------------------------------------------------
static OBS_COLLISION_OBJ * _obj_collision_tbl[OBD_COLLISION_OBJECT_MAX]    = {0}; ///< 登録オブジェポインタリスト 実際にチェックするリスト
static OBS_COLLISION_OBJ * _obj_collision_tbl_nx[OBD_COLLISION_OBJECT_MAX] = {0}; ///< 登録オブジェポインタリスト 登録直後
static u8                  _obj_collision_num = 0; ///< 登録オブジェ数
static u8                  _obj_collision_num_nx = 0; ///< 登録オブジェ数 直後


//----- Global Functions -----------------------------------------------

//----- Local Functions ------------------------------------------------
static s32 objGetColDataX( OBS_COLLISION_OBJ * pObj, s32 lPosX, s32 lPosY, u16 ucSuf, u16* pDir, u32 *pAttr  );
static  s32 objGetColDataY( OBS_COLLISION_OBJ * pObj, s32 lPosX, s32 lPosY, u16 ucSuf, u16* pDir, u32 *pAttr );
inline s32 objMapGetDiff( s32 lCol, s8 sPix, s8 );
inline s32 objMapGetForward(s8 sPix, s8 sDelta );
inline s32 objMapGetBack(s8 sPix, s8 sDelta );
inline s32 objMapGetForwardRev(s8 sPix, s8 sDelta );
static s32 objFastCollisionDiffObject(OBS_COLLISION_OBJ *pColObj, const OBS_COL_CHK_DATA *pData );
static s32 objCollisionDiffObject(OBS_COLLISION_OBJ *pColObj, const OBS_COL_CHK_DATA *pData );
static void objCollsionOffsetSet( OBS_COLLISION_OBJ * pCol, s16 * cOfstX, s16 * cOfstY );

// ================================================================
// ObjCollisionObjectRegist
/*!
  地形オブジェクトを登録する
 
  @param pObj  [in] オブジェクトコリジョンポインタ

  @note
    #_obj_collision_tbl_nxのリストに登録し、次に ObjCollisionObjectClear() を呼んだ時、\n
    チェックするリストに移動する。\n
    チェックリストに入るのは登録した次のフレームからなので\n
    オブジェクト初期化時にも実行するとチェック漏れがなくなる。\n
 */
// ================================================================
void ObjCollisionObjectRegist( OBS_COLLISION_OBJ * pObj )
{
    // 登録数オーバーチェック
    if ( _obj_collision_num_nx >= OBD_COLLISION_OBJECT_MAX ){
        MTM_ASSERT(0);
        return;
    }
    // 登録するオブジェクトの死亡チェック
    if ( pObj->obj &&
         pObj->obj->flag & (OBD_OBJECT_TASKCLEAR | OBD_OBJECT_TASKCLEAR_REQUEST) )
        return;
    
    // 降りチェック
    if ( pObj->rider_obj ){
        // 乗っていたオブジェクトがすでに自分に乗っていなければクリア
        if ( pObj->rider_obj->ride_obj != pObj->obj )
            pObj->rider_obj = NULL;
    }
    // 触りチェック
    if ( pObj->toucher_obj ){
        // すでに触っていなければクリア
        if ( pObj->toucher_obj->touch_obj != pObj->obj )
            pObj->toucher_obj = NULL;
    }

    // 登録
    _obj_collision_tbl_nx[_obj_collision_num_nx]  = pObj;
    
    // 登録数追加
    ++_obj_collision_num_nx;

    // 以下、先行計算
    
    // 座標設定
    {
        VecFx32 vPos = {0}; // 座標
        vPos = pObj->pos;
        if ( pObj->obj && !(pObj->flag & OBD_COLOBJ_NOPOS_PARENT) ){
            vPos.x += pObj->obj->pos.x;
            vPos.y += pObj->obj->pos.y;
            vPos.z += pObj->obj->pos.z;
        }
        pObj->check_pos = vPos;
    }
    // フラグ設定
    {
        pObj->flag &= ~( OBD_COLOBJ_SYS_HFLIP | OBD_COLOBJ_SYS_VFLIP );
        if ( pObj->obj ){
            // 親の設定をセット
            if ( pObj->obj->disp_flag & OBD_DISP_HFLIP )
                pObj->flag |= OBD_COLOBJ_SYS_HFLIP;
            if ( pObj->obj->disp_flag & OBD_DISP_VFLIP )
                pObj->flag |= OBD_COLOBJ_SYS_VFLIP;
        }else{
            // 自分の設定をセット
            if ( pObj->flag & OBD_COLOBJ_HFLIP )
                pObj->flag |= OBD_COLOBJ_SYS_HFLIP;
            if ( pObj->flag & OBD_COLOBJ_VFLIP )
                pObj->flag |= OBD_COLOBJ_SYS_VFLIP;
        }
    }
    // オフセット設定
    {
        objCollsionOffsetSet( pObj, &pObj->check_ofst_x, &pObj->check_ofst_y);
    }
    {
        // 各位置設定
        pObj->left   = ((pObj->check_pos.x >> FX32_SHIFT) + pObj->check_ofst_x);
        pObj->top    = ((pObj->check_pos.y >> FX32_SHIFT) + pObj->check_ofst_y);
        pObj->right  = ((pObj->check_pos.x >> FX32_SHIFT) + pObj->width  + pObj->check_ofst_x);
        pObj->bottom = ((pObj->check_pos.y >> FX32_SHIFT) + pObj->height + pObj->check_ofst_y);
    }
    
    // 角度設定
    {
        if ( !(pObj->flag & OBD_COLOBJ_NODIR) )
            pObj->check_dir = pObj->dir;
        // 親角度チェック
        if ( !(pObj->flag & OBD_COLOBJ_NODIR_PARENT) && pObj->obj ){
            pObj->check_dir += pObj->obj->dir.z + pObj->obj->dir_fall;
        }
    }
}
// ================================================================
// ObjCollisionObjectClear
/*!
  登録した地形オブジェクトをクリアする
 
  @note
  メインループ内の全ての地形判定が終わったあと実行する\n
  2回連続で関数を呼び出すとバックバッファもクリアされる
 */
// ================================================================
void ObjCollisionObjectClear()
{
    u16 i;

    // 今フレームの登録を設定する
    for ( i = 0; i < _obj_collision_num_nx; ++i ){
        _obj_collision_tbl[ i ] = _obj_collision_tbl_nx[i];
    }
    for ( ; i < OBD_COLLISION_OBJECT_MAX; ++i )
        _obj_collision_tbl_nx[ i ] = NULL;
    
    // 登録数を移動する
    _obj_collision_num = _obj_collision_num_nx;
    _obj_collision_num_nx = 0;

}
// ================================================================
// objCollsionOffsetSet
/*!
  オフセットを設定する
 
  @param pObj      [in] コリジョンポインタ
  @param cOfstX    [out] X座標
  @param cOfstY    [out] Y座標
 
 */
// ================================================================
static void objCollsionOffsetSet( OBS_COLLISION_OBJ * pCol, s16 * cOfstX, s16 * cOfstY )
{
    *cOfstX = (s16)pCol->ofst_x;
    *cOfstY = (s16)pCol->ofst_y;

    // 反転
    if ( pCol->flag & OBD_COLOBJ_SYS_HFLIP )
        *cOfstX = (s16)(-pCol->ofst_x - (pCol->width ));
    if ( pCol->flag & OBD_COLOBJ_SYS_VFLIP )
        *cOfstY = (s16)(-pCol->ofst_y - (pCol->height ));
}

// ================================================================
// ObjCollisionObjectFastCheckDet
/*!
  登録した地形オブジェクトと指定座標との差分値をチェックする
 
  @param lPosX  [in] X座標 1:31
  @param lPosY  [in] Y座標 1:31
  @param usFlag [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param usVec  [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
  @param pDir   [out] 角度ポインタ NULLで設定を行わない 値は(0〜2π を 0〜256 に写像)
  @param pAttr  [out] 属性ポインタ NULLで設定を行わない
 
  @note
  ヒットしたオブジェクトに対して、\n
  座標から指定方向に 地形オブジェクトを調べ、接地面までの距離を求める
  
 */
// ================================================================
s32 ObjCollisionObjectFastCheckDet( s32 lPosX, s32 lPosY, u16 usFlag, u16 usVec, u16* pDir, u32* pAttr)
{
    OBS_COL_CHK_DATA tData;
    
    tData.pos_x = lPosX;
    tData.pos_y = lPosY;
    tData.dir = pDir;
    tData.attr = pAttr;
    tData.flag = usFlag;
    tData.vec = usVec;

    return ObjCollisionObjectFastCheck( &tData);
}
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
s32 ObjCollisionObjectFastCheck( const OBS_COL_CHK_DATA *pData )
{
    u16 i = 0;
    s32 lDiff = 24; // もっとも地面から離れている値に設定
    s32 lDiffCheck = 0;
    s32 lPosX = pData->pos_x;
    s32 lPosY = pData->pos_y;
//    s32 lPosBackX = lPosX;
//    s32 lPosBackY = lPosY;
    OBS_COLLISION_OBJ * pCol; // 判定中データポインタ
    OBS_COL_CHK_DATA oData;

    // データコピー
    MI_CpuCopy8( pData, &oData, sizeof(OBS_COL_CHK_DATA) );

    //s16 cOfstX = 0;
    //s16 cOfstY = 0;

    // なしCheck
    if ( !_obj_collision_num )
        return lDiff;

    // まず矩形判定でチェックを行い、HITしたオブジェクトだけ差分値を取得する
    for ( i = 0; i < _obj_collision_num; ++i ){

        pCol = _obj_collision_tbl[i];

        if ( pCol->obj == NULL )
            continue;
        if ( pCol->flag & OBD_COLOBJ_NOHIT )
            continue;

        // 値復帰
        oData.pos_x = pData->pos_x;
        oData.pos_y = pData->pos_y;

        // オフセット設定
        //objCollsionOffsetSet( pCol, &cOfstX, &cOfstY );

        // 回転チェック
        if ( pCol->flag & OBD_COLOBJ_DIR && pCol->check_dir ){

#if defined _DS
            s32 lPosCenterOfstX;
            s32 lPosCenterOfstY;
            MtxFx33 mRot;
            VecFx32 vPosTemp;

            // 中心からの相対座標
            lPosCenterOfstX = (oData.pos_x <<12)- (pCol->check_pos.x /*- (cOfstX << 12 )*/);
            lPosCenterOfstY = (oData.pos_y <<12)- (pCol->check_pos.y /*- (cOfstY << 12 )*/);

            // 座標回転
            VEC_Set(&vPosTemp, lPosCenterOfstX, lPosCenterOfstY, 0 );
            MTX_RotZ33(&mRot,  mtMathSin((u16)-(pCol->check_dir)), mtMathCos((u16)-(pCol->check_dir)) );
            MTX_MultVec33( &vPosTemp, &mRot, &vPosTemp );
            lPosCenterOfstX = vPosTemp.x;
            lPosCenterOfstY = vPosTemp.y;

            // 相対から絶対に戻す
            oData.pos_x = (lPosCenterOfstX + (pCol->check_pos.x /*- (cOfstX << 12 )*/)>>12);
            oData.pos_y = (lPosCenterOfstY + (pCol->check_pos.y /*- (cOfstY << 12 )*/)>>12);

#else
            s32 lPosCenterOfstX;
            s32 lPosCenterOfstY;

            // 中心からの相対座標
            lPosCenterOfstX = (oData.pos_x << FX32_SHIFT)- (pCol->check_pos.x /*- (cOfstX << 12 )*/);
            lPosCenterOfstY = (oData.pos_y << FX32_SHIFT)- (pCol->check_pos.y /*- (cOfstY << 12 )*/);

            // 座標回転
			ObjUtilGetRotPosXY(lPosCenterOfstX, lPosCenterOfstY, &lPosCenterOfstX, &lPosCenterOfstY, (u16)-(pCol->check_dir));

            // 相対から絶対に戻す
            oData.pos_x = (lPosCenterOfstX + ((pCol->check_pos.x /*- (cOfstX << 12 )*/)>>FX32_SHIFT));
            oData.pos_y = (lPosCenterOfstY + ((pCol->check_pos.y /*- (cOfstY << 12 )*/)>>FX32_SHIFT));


#endif
            // 回転値によってはチェック方向フラグも再設定する
        }
        // 矩形判定しない方が軽い？
        /*
        if ( lPosX >= (pCol->check_pos.x >> 12) - OBD_COLOBJ_OFFSET + pCol->sCheckOfstX &&
             lPosX <= (pCol->check_pos.x >> 12) + (pCol->width ) + OBD_COLOBJ_OFFSET + pCol->sCheckOfstX){
            if ( lPosY >= (pCol->check_pos.y >> 12) - OBD_COLOBJ_OFFSET + pCol->sCheckOfstY &&
                 lPosY <= (pCol->check_pos.y >> 12) + (pCol->height ) + OBD_COLOBJ_OFFSET + pCol->sCheckOfstY ){
         */
        {
            {
                if ( pCol->diff_data ){
                    // HIT 差分値を求める
                    lDiffCheck = objFastCollisionDiffObject( pCol, &oData );
                    // 挟まりチェック用にその方向へ移動している地形と密着した時は1ドットめり込ませる
                    if ( !lDiffCheck ){
                        switch ( pData->vec ){
                        case OBD_COL_LEFT:
                            if ( pCol->obj->move.x > 0 )
                                lDiffCheck -= 1;
                            break;
                        case OBD_COL_RIGHT:
                            if ( pCol->obj->move.x < 0 )
                                lDiffCheck -= 1;
                            break;
                        }
                    }
                }else{
                    // そのまま判定
                    switch ( pData->vec ){
                    case OBD_COL_DOWN:
                        lDiffCheck = ( pCol->top ) - lPosY;
                        break;
                    case OBD_COL_UP:
                        lDiffCheck = lPosY - ( pCol->bottom );
                        break;
                    case OBD_COL_LEFT:
                        lDiffCheck = lPosX - (pCol->right);
                        if ( !lDiffCheck && pCol->obj->move.x > 0 )
                            lDiffCheck -= 1;
                        break;
                    case OBD_COL_RIGHT:
                        lDiffCheck = pCol->left - lPosX;
                        if ( !lDiffCheck && pCol->obj->move.x < 0 )
                            lDiffCheck -= 1;
                        break;
                    }
                    lDiffCheck = MTM_MATH_CLIP(lDiffCheck, -31, 31);
                }
                // 反映チェック
                if ( lDiff > lDiffCheck ){
                    lDiff = lDiffCheck;
                }
            }
        }
    }

    // 一番埋まっている差分値を返す
    return lDiff;
}


// ================================================================
// ObjCollisionObjectCheckDet
/*!
  登録した地形オブジェクトとの差分値をチェックする
 
  @param pObj  [in] チェックするオブジェクトポインタ
  @param lPosX  [in] X座標 1:31
  @param lPosY  [in] Y座標 1:31
  @param usFlag [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param usVec  [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
  @param pDir   [out] 角度ポインタ NULLで設定を行わない 値は(0〜2π を 0〜256 に写像)
  @param pAttr  [out] 属性ポインタ NULLで設定を行わない
 
  @note
  ヒットしたオブジェクトに対して、\n
  座標から指定方向に 地形オブジェクトを調べ、接地面までの距離を求める
  
 */
// ================================================================
s32 ObjCollisionObjectCheckDet( OBS_OBJECT_WORK * pObj, s32 lPosX, s32 lPosY, u16 usFlag, u16 usVec, u16* pDir, u32* pAttr)
{
    OBS_COL_CHK_DATA tData;
    
    tData.pos_x = lPosX;
    tData.pos_y = lPosY;
    tData.dir = pDir;
    tData.attr = pAttr;
    tData.flag = usFlag;
    tData.vec = usVec;

    return ObjCollisionObjectCheck(pObj, &tData);
}
// ================================================================
// ObjCollisionObjectCheck
/*!
  登録した地形オブジェクトとの差分値をチェックする
 
  @param pObj       [in] チェックするオブジェクトポインタ
  @param pData      [in] OBS_COL_CHK_DATAポインタ
 
  @note
  ヒットしたオブジェクトに対して、\n
  座標から指定方向に 地形オブジェクトを調べ、接地面までの距離を求める
  
 */
// ================================================================
s32 ObjCollisionObjectCheck( OBS_OBJECT_WORK * pObj, const OBS_COL_CHK_DATA *pData)
{
    s32 lDiff = 24; // もっとも地面から離れている値に設定
    s32 lDiffCheck;
    s16 sFloat = 0; // 浮いてるか乗っているか閾値
    u16 ucLandingCheck = 0; // 着地処理を行う判定
    u16 i = 0;
    u16 dir_work;
    OBS_COLLISION_OBJ * pCol; // 判定中データポインタ
    OBS_COL_CHK_DATA oData;
	s16 col_chk_area;			// チェック範囲領域

    // データコピー
    MI_CpuCopy8( pData, &oData, sizeof(OBS_COL_CHK_DATA) );
    
    //s16 cOfstX = 0;
    //s16 cOfstY = 0;

    // なしCheck
    if ( !_obj_collision_num )
        return lDiff;
    
#if 0	// 090908dir_fall対応テスト
    if ( !(pObj->move_flag & OBD_MOVE_JUMP) ){
        sFloat = 1;
#if 0//dir_fall対応テスト
		dir_work = (u16)(pObj->dir_fall + pObj->dir.z);
#else//dir_fall対応テスト
//		dir_work = (u16)(pObj->dir.z - pObj->dir_fall);
		dir_work = pObj->dir_fall;
#endif//dir_fall対応テスト
    }else{
		dir_work = pObj->dir_fall;
    }
#else	// 090908dir_fall対応テスト
	if ( !(pObj->move_flag & OBD_MOVE_JUMP) ){
		sFloat = 1;
		dir_work = 0;
	} else {
		dir_work = (u16)(pObj->dir.z);
	}
#endif	// 090908dir_fall対応テスト
    
    // 落下方向にあわせて乗りチェック
    switch ( ( ((dir_work + 0x2000) & 0xc000) >> 14 ) ){
    default:
        // 下向き落下
        if ( pData->vec == OBD_COL_DOWN && (!(pObj->move_flag & OBD_MOVE_JUMP) || pObj->move.y >= 0) )
             ucLandingCheck = 1;
        break;
    case 1:
        // 左向き落下
        if ( pData->vec == OBD_COL_LEFT && (!(pObj->move_flag & OBD_MOVE_JUMP) ||  pObj->move.x < 0 ) )
            ucLandingCheck = 1;
        break;
    case 2:
        // 上向き落下
        if ( pData->vec == OBD_COL_UP && (!(pObj->move_flag & OBD_MOVE_JUMP) ||  pObj->move.y < 0 ) )
            ucLandingCheck = 1;
        break;
    case 3:
        // 右向き落下
        if ( pData->vec == OBD_COL_RIGHT && (!(pObj->move_flag & OBD_MOVE_JUMP) ||  pObj->move.x > 0 ) )
            ucLandingCheck = 1;
        break;
        
    }

	// チェック範囲の設定（処理高速化のため、HOGスペステは範囲をより限定
	//					HOGスペステは移動速度制限から範囲を狭めても大丈夫そう）
	if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
		// スペステ用
		col_chk_area = OBD_COLOBJ_OFFSET_SS;
	} else {
		// 一般用
		col_chk_area = OBD_COLOBJ_OFFSET;
	}
		
    // 落下向きチェック
    
    // まず矩形判定でチェックを行い、HITしたオブジェクトだけ差分値を取得する
    for ( i = 0; i < _obj_collision_num; ++i ){

        pCol = _obj_collision_tbl[i];

        // 登録がキャンセルされた
        if ( pCol->obj == NULL )
            continue;
        // 自分
        if ( pCol->obj == pObj )
            continue;
        if ( pCol->flag & OBD_COLOBJ_NOHIT )
            continue;
        
        // 値復帰
        oData.pos_x = pData->pos_x;
        oData.pos_y = pData->pos_y;
        
        // オフセット設定
        // objCollsionOffsetSet( pCol, &cOfstX, &cOfstY );


        // 回転チェック
        if ( pCol->flag & OBD_COLOBJ_DIR && pCol->check_dir ){

#if defined _DS
            s32 lPosCenterOfstX;
            s32 lPosCenterOfstY;
            MtxFx33 mRot;
            VecFx32 vPosTemp;

            // 中心からの相対座標
            lPosCenterOfstX = (oData.pos_x <<12)- (pCol->check_pos.x /*- (cOfstX << 12 )*/);
            lPosCenterOfstY = (oData.pos_y <<12)- (pCol->check_pos.y /*- (cOfstY << 12 )*/);

            // 座標回転
            VEC_Set(&vPosTemp, lPosCenterOfstX, lPosCenterOfstY, 0 );
            MTX_RotZ33(&mRot,  mtMathSin((u16)-(pCol->check_dir)), mtMathCos((u16)-(pCol->check_dir)) );
            MTX_MultVec33( &vPosTemp, &mRot, &vPosTemp );
            lPosCenterOfstX = vPosTemp.x;
            lPosCenterOfstY = vPosTemp.y;

            // 相対から絶対に戻す
            oData.pos_x = (lPosCenterOfstX + (pCol->check_pos.x /*- (cOfstX << 12 )*/)>>12);
            oData.pos_y = (lPosCenterOfstY + (pCol->check_pos.y /*- (cOfstY << 12 )*/)>>12);

            // 回転値によってはチェック方向フラグも再設定する
#else
            s32 lPosCenterOfstX;
            s32 lPosCenterOfstY;

            // 中心からの相対座標
            lPosCenterOfstX = (oData.pos_x << FX32_SHIFT)- (pCol->check_pos.x /*- (cOfstX << 12 )*/);
            lPosCenterOfstY = (oData.pos_y << FX32_SHIFT)- (pCol->check_pos.y /*- (cOfstY << 12 )*/);

            // 座標回転
			ObjUtilGetRotPosXY(lPosCenterOfstX, lPosCenterOfstY, &lPosCenterOfstX, &lPosCenterOfstY, (u16)-(pCol->check_dir));

            // 相対から絶対に戻す
            oData.pos_x = (lPosCenterOfstX + ((pCol->check_pos.x /*- (cOfstX << 12 )*/)>>FX32_SHIFT));
            oData.pos_y = (lPosCenterOfstY + ((pCol->check_pos.y /*- (cOfstY << 12 )*/)>>FX32_SHIFT));

            // 回転値によってはチェック方向フラグも再設定する
#endif

        }
#if 0
        // 矩形判定しない方が軽い？
        /*
        if ( lPosX >= (pCol->check_pos.x >> 12) - col_chk_area + pCol->sCheckOfstX &&
             lPosX <= (pCol->check_pos.x >> 12) + (pCol->width ) + col_chk_area + pCol->sCheckOfstX){
            if ( lPosY >= (pCol->check_pos.y >> 12) - col_chk_area + pCol->sCheckOfstY &&
                 lPosY <= (pCol->check_pos.y >> 12) + (pCol->height ) + col_chk_area + pCol->sCheckOfstY ){
         */
        {
            {
#else
		// 事前に座標からチェク対象を絞り込み
		if ( oData.pos_x >= (pCol->check_pos.x >> 12) - col_chk_area + pCol->check_ofst_x &&
			 oData.pos_x <= (pCol->check_pos.x >> 12) + (pCol->width ) + col_chk_area + pCol->check_ofst_x){
			if ( oData.pos_y >= (pCol->check_pos.y >> 12) - col_chk_area + pCol->check_ofst_y &&
				 oData.pos_y <= (pCol->check_pos.y >> 12) + (pCol->height ) + col_chk_area + pCol->check_ofst_y ){
#endif
                // HIT 差分値を求める
                if ( pCol->diff_data ){
                    // HIT 差分値を求める
                    lDiffCheck = objCollisionDiffObject( pCol, &oData );
                    // 挟まりチェック用にその方向へ移動している地形と密着した時は1ドットめり込ませる
                    if ( !lDiffCheck ){
                        switch ( pData->vec ){
                        case OBD_COL_LEFT:
                            if ( pCol->obj && pCol->obj->move.x > 0 )
                                lDiffCheck -= 1;
                            break;
                        case OBD_COL_RIGHT:
                            if ( pCol->obj && pCol->obj->move.x < 0 )
                                lDiffCheck -= 1;
                            break;
                        }
                    }
                }else{
                    lDiffCheck = 24;
                    // 矩形データを地形として判定する
                    switch ( pData->vec ){
                    case OBD_COL_DOWN:
                        if ( oData.pos_y < ( pCol->bottom ) && oData.pos_x > pCol->left && oData.pos_x < pCol->right )
                            lDiffCheck = ( pCol->top ) - oData.pos_y;
                        break;
                    case OBD_COL_UP:
                        if ( ( pCol->top ) < oData.pos_y && oData.pos_x > pCol->left && oData.pos_x < pCol->right )
                            lDiffCheck = oData.pos_y - ( pCol->bottom );
                        break;
                    case OBD_COL_LEFT:
                        if ( (pCol->left) < oData.pos_x && oData.pos_y > pCol->top && oData.pos_y < pCol->bottom ){
                            lDiffCheck = oData.pos_x - (pCol->right);
                            if ( !lDiffCheck && pCol->obj && pCol->obj->move.x > 0 )
                                lDiffCheck -= 1;
                        }
                        break;
                    case OBD_COL_RIGHT:
                        if ( oData.pos_x < pCol->right && oData.pos_y > pCol->top && oData.pos_y < pCol->bottom ){
                            lDiffCheck = pCol->left - oData.pos_x;
                            if ( !lDiffCheck && pCol->obj && pCol->obj->move.x < 0 )
                                lDiffCheck -= 1;
                        }
                        break;
                    }
                    lDiffCheck = MTM_MATH_CLIP(lDiffCheck, -31, 31);
                }
                
                // 反映チェック
                if ( lDiff > lDiffCheck ){
                    lDiff = lDiffCheck;
                    if ( lDiff <= sFloat ){
                        // 触っているオブジェクトとして設定する
                        pObj->touch_obj = pCol->obj;
                        pCol->toucher_obj = pObj;
                    }

                    if ( lDiff <= sFloat && ucLandingCheck ){
                        // 乗っているオブジェクトとして設定する
                        pObj->ride_obj = pCol->obj;
                        pCol->rider_obj = pObj;
                        if ( pData->dir )
#if 1//dir_fall対応テスト
							*pData->dir += pCol->check_dir;
#else//dir_fall対応テスト
							*pData->dir += pCol->check_dir - pObj->dir_fall;
#endif//dir_fall対応テスト
                        if ( pData->attr && !(pCol->flag & OBD_COLOBJ_NOATTR) )
                            *pData->attr |= (u8)pCol->attr;
                    }/* 
                    }else{
                        if ( pDir ){
                            *pDir += pCol->check_dir;
                        }
                    }*/
                }
            }
        }
    }

    // 一番埋まっている差分値を返す
    return lDiff;
}


// ================================================================
// ObjCollisionDiffObjectGetCollisionObj
/*!
  登録されている地形オブジェクトワークの取得

  @param col_no    [in] 取得する地形オブジェクト情報NO(0〜)
 
  @return   登録されている地形オブジェクト情報 NULLでそのNO以降の登録無し
 
  @note
	登録されている地形オブジェクト情報を取得します。\n
	登録されているNO以上が指定されているとNULLを返します。

 */
// ================================================================
OBS_COLLISION_OBJ* ObjCollisionDiffObjectGetCollisionObj(u8 col_no)
{
	if (col_no < _obj_collision_num) {
		return (_obj_collision_tbl[col_no]);
	}
	return (NULL);
}

// ================================================================
// objFastCollisionDiffObject
/*!
  地形オブジェクト当たりチェック

  @param pColObj    [in] チェックするオブジェクトコリジョンポインタ
  @param pData      [in] OBS_COL_CHK_DATAポインタ
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
  座標から指定方向に 地形オブジェクトを調べ、接地面までの距離を求める

 */
// ================================================================
static s32 objFastCollisionDiffObject(OBS_COLLISION_OBJ *pColObj, const OBS_COL_CHK_DATA *pData )
{
    s32 lCol;
    u16 usDir = 0; // 前角度保持
    u32 ulAttr = 0; 
    s8  sPix;
    s8  cDelta = 8; // 進行方向値

    // 前角度保持
    if ( pData->dir )
        usDir = *pData->dir;
    // 前属性保持
    if ( pData->attr )
        ulAttr = *pData->attr;
    
    // フラグから進行方向の設定
    if ( pData->vec & OBD_COL_MINUS )
        cDelta = -8;
    
    
    // 地形差分値、角度を取得
    if ( pData->vec & OBD_COL_Y ){
        lCol = objGetColDataY( pColObj, pData->pos_x, pData->pos_y, pData->flag, pData->dir, pData->attr ); 
        sPix = (s8)((pData->pos_y - pColObj->top )  & 0x00000007);
    }else{
        lCol = objGetColDataX( pColObj, pData->pos_x, pData->pos_y, pData->flag, pData->dir, pData->attr );
        sPix = (s8)((pData->pos_x - pColObj->left )  & 0x00000007);
    }
    
    if ( lCol == 0) {
        // 地形にHITしなかったので角度を前の状態に戻す
        if ( pData->dir)
            *pData->dir = usDir;
        if ( pData->attr )
            *pData->attr = ulAttr;
        return objMapGetForward(sPix, cDelta);
    }
    else if ( lCol == 8) {
        // 隙間無しブロックにHIT
        return objMapGetBack(sPix, cDelta);
    }
    else {
        // 隙間有りブロックにHIT
        return objMapGetDiff( lCol, sPix, cDelta);
    }
}
// ================================================================
// objCollisionDiffObject
/*!
  マップ当たりチェック

  @param pDir      [out] 角度ポインタ NULLで設定を行わない 値は(0〜2π を 0〜256 に写像)
  @param pColObj   [in] データ元となるOBS_COLLISION_OBJポインタ
  @param lPosX     [in] X座標
  @param lPosY     [in] Y座標
  @param ucSuf     [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param ucFlag    [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
  座標から指定方向に ３キャラを調べ、接地面までの距離を求める

 */
// ================================================================
static s32 objCollisionDiffObject(OBS_COLLISION_OBJ *pColObj, const OBS_COL_CHK_DATA *pData  )
{
    s32 lCol;
    s32 lMoveX = 0;
    s32 lMoveY = 0;
    u32  ulAttr = 0; 
    s32 (*pFunc)(OBS_COLLISION_OBJ *, s32, s32, u16, u16*, u32* );
    u16  usDir = 0; 
    s8  cDeltaX = 0; // 進行方向値
    s8  cDeltaY = 0; // 進行方向値
    s8  sPix;

    // 前角度保持
    if ( pData->dir )
        usDir = *pData->dir;
    // 前属性保持
    if ( pData->attr )
        ulAttr = *pData->attr;
    
    // フラグから進行方向の設定
    if ( pData->vec & OBD_COL_Y ){
        cDeltaY = 8;
        if ( pData->vec & OBD_COL_MINUS )
            cDeltaY = -8;
    }else{
        cDeltaX = 8;
        if ( pData->vec & OBD_COL_MINUS )
            cDeltaX = -8;
    }
    
    // 地形差分値、角度を取得
    if ( pData->vec & OBD_COL_Y ){
        sPix = (s8)((pData->pos_y - pColObj->top )  & 0x00000007);
        pFunc = objGetColDataY;
    }else{
        sPix = (s8)((pData->pos_x - pColObj->left )  & 0x00000007);
        pFunc = objGetColDataX;
    }
    
    // 地形判定
    lCol = pFunc( pColObj, pData->pos_x, pData->pos_y, pData->flag, pData->dir, pData->attr );
    
    if ( lCol == 0) {
		// lCol == 0 で地形情報はNULL
        // 地形にHITしなかったので次をチェック
        lMoveX += cDeltaX; // 移動分を追加
        lMoveY += cDeltaY;

        // 地形取得
        lCol = pFunc( pColObj, pData->pos_x + lMoveX, pData->pos_y + lMoveY, pData->flag, pData->dir, pData->attr );
        
        if ( lCol == 0) {
            lMoveX += cDeltaX; // 移動分を追加
            lMoveY += cDeltaY;

            // 地形取得
            lCol = pFunc( pColObj, pData->pos_x + lMoveX, pData->pos_y + lMoveY, pData->flag, pData->dir, pData->attr );

            // それぞれ２キャラ分プラスして値を返す
            if ( lCol == 0) {
                if ( pData->dir)
                    *pData->dir = usDir;
                if ( pData->attr)
                    *pData->attr = ulAttr;
                return objMapGetForward(sPix, (s8)(cDeltaX + cDeltaY)) + 16;
            }else if ( lCol == 8 ){
                return objMapGetBack(sPix, (s8)(cDeltaX + cDeltaY)) + 16;
            }else{
                return objMapGetDiff(lCol, sPix, (s8)(cDeltaX + cDeltaY)) + 16;
            }
        }
        // それぞれ１キャラ分プラスして値を返す
        else if ( lCol == 8 ){
            return objMapGetBack(sPix, (s8)(cDeltaX + cDeltaY)) + 8;
        }else{
            return objMapGetDiff(lCol, sPix, (s8)(cDeltaX + cDeltaY)) + 8;
        }
    }
    else if ( lCol == 8) {
		// lCol == 8 で地形情報はSOLID
        if ( pData->dir )
            usDir = *pData->dir;
        if ( pData->attr )
            ulAttr = *pData->attr;

        lMoveX -= cDeltaX; // 移動分を追加
        lMoveY -= cDeltaY;

        // 地形取得
        lCol = pFunc( pColObj, pData->pos_x + lMoveX, pData->pos_y + lMoveY, pData->flag, pData->dir, pData->attr );

        if ( lCol == 8) {
            if ( pData->dir )
                usDir = *pData->dir;
            if ( pData->attr )
                ulAttr = *pData->attr;
            lMoveX -= cDeltaX; // 移動分を追加
            lMoveY -= cDeltaY;

            // 地形取得
            lCol = pFunc( pColObj, pData->pos_x + lMoveX, pData->pos_y + lMoveY, pData->flag, pData->dir, pData->attr );

            // それぞれ２キャラ分マイナスして値を返す
            if ( lCol == 0) {
                if ( pData->dir)
                    *pData->dir = usDir;
                if ( pData->attr )
                    *pData->attr = ulAttr;
                return objMapGetForwardRev(sPix, (s8)(cDeltaX + cDeltaY)) - 16;
            }else if ( lCol == 8 ){
                return objMapGetBack(sPix, (s8)(cDeltaX + cDeltaY)) - 16;
            }else{
                return objMapGetDiff(lCol, sPix, (s8)(cDeltaX + cDeltaY)) - 16;
            }
        }
        // それぞれ１キャラ分マイナスして値を返す
        else if ( lCol == 0 ){
            // 追加
            if ( pData->dir)
                *pData->dir = usDir;
            if ( pData->attr)
                *pData->attr = ulAttr;
            return objMapGetForwardRev(sPix, (s8)(cDeltaX + cDeltaY)) - 8;
        }else{
            return objMapGetDiff(lCol, sPix, (s8)(cDeltaX + cDeltaY)) - 8;
        }
    }
    else {
        // 隙間有りブロックにHIT
        return objMapGetDiff( lCol, sPix, (s8)(cDeltaX + cDeltaY));
    }
    
}

// ================================================================
// objGetColDataX
/*!
  指定した位置の地形データを取得する
 
  @param pColObj   [in] データ元となるOBS_COLLISION_OBJポインタ
  @param lPosX     [in] X座標
  @param lPosY     [in] Y座標
  @param ucSuf     [in] BG面 0x80のビットが立っている場合はすり抜け床をすり抜ける
  @param pDir      [out] 角度情報
 
  @return   X軸の地形差分値
 
 */
// ================================================================
static s32 objGetColDataX( OBS_COLLISION_OBJ *pColObj, s32 lPosX, s32 lPosY, u16 ucSuf, u16* pDir, u32 *pAttr  )
{
    s32 ulPix; // キャラ辺りのドット位置
    s8  cCol = 0;   // 地形差分値
    u16 usCharNo; // キャラ番号

    // クリッピング
    if ( lPosX <  pColObj->left ||
         lPosX >= pColObj->right )
        return cCol;
    if ( lPosY <  pColObj->top ||
         lPosY >= pColObj->bottom )
        return cCol;

    // 差分番号抽出 各座標と中心オフセット値から計算する
    {
        u16 x,y;
        x = (u16)( ( lPosX - pColObj->left ) >> 3 );
        y = (u16)( ( lPosY - pColObj->top  ) >> 3 );

        // 反転
        if ( pColObj->flag & OBD_COLOBJ_SYS_HFLIP )
            x = (u16)(((pColObj->width >> 3) - 1) - x);
        if ( pColObj->flag & OBD_COLOBJ_SYS_VFLIP )
            y = (u16)(((pColObj->height >> 3) - 1) - y);
        
        // キャラ番号取得
        usCharNo = (u16)( x + y * (pColObj->width >> 3));
    }
    // １キャラ辺りのドット位置を取得
    ulPix = ( lPosY - pColObj->top ) & 0x07;

    if ( pColObj->flag & OBD_COLOBJ_SYS_VFLIP ){
        ulPix = 7 - ulPix; // 座標もVフリップ
    }

    // 地形差分テーブルから指定座標の差分値を取得
    cCol =  pColObj->diff_data[ (usCharNo << 3) + ulPix ];
    cCol &=  0x0f; // Y軸の差分値を削除
    
    // 8bitのマイナス値に設定（データは4bitなので自前でマイナスにする）
    if (cCol & 0x08)
        cCol |= ~0x0f;

    // 指定差分値が地形のみの値（-8）なら絶対値を設定
    if (cCol == -8)
        cCol = 8;
    
    // すり抜け地形をすり抜け設定で、属性がすり抜け地形であれば地形は空にする
    if ( pColObj->attr_data ){
        if ((ucSuf & OBD_COL_THROUGH) && ((pColObj->attr_data[usCharNo >> 3] & OBD_COL_DATA_ATTR_THROUGH) || (pColObj->attr & OBD_COL_DATA_ATTR_THROUGH ) ) )
            cCol = 0;
    }else{
        if ((ucSuf & OBD_COL_THROUGH) && ( (pColObj->attr & OBD_COL_DATA_ATTR_THROUGH ) ) )
            cCol = 0;
    }
    
    if ( pColObj->flag & OBD_COLOBJ_SYS_HFLIP ){
        // 地形差分値をHフリップ
        if (!( cCol == 8 || cCol == 0 )){
            if ( cCol > 0 ){
                cCol -= 8;
            }else{
                cCol += 8;
            }
        }
    }

    
    // 角度取得チェック
    if ( pDir && cCol ){
        u16 ucDir;
        // 角度データを取得
        if ( pColObj->dir_data )
            ucDir = (u16)(*(pColObj->dir_data + usCharNo) << 8 );
        else
            ucDir = 0;
        if ( pColObj->flag & OBD_COLOBJ_DIR ){
            ucDir += pColObj->check_dir;
        }
        // 角度変化
        if ( pColObj->flag & OBD_COLOBJ_DIR_FLIP ){
            if ( (pColObj->flag & OBD_COLOBJ_SYS_HFLIP) )
                ucDir = (u16)(-(s16)ucDir);

            if ( pColObj->flag & OBD_COLOBJ_SYS_VFLIP )
                ucDir = (u16)(-((s16)ucDir + 0x4000) - 0x4000);
        }            
        
        // 角度情報を設定
        *pDir = ucDir;
    }
    if ( pAttr && cCol ){
        if ( pColObj->attr_data )
            *pAttr = (u32)(pColObj->attr_data[usCharNo >> 3] | pColObj->attr);
        else
            *pAttr = (u32)pColObj->attr;
    }
    
    return cCol;
}
// ================================================================
// objGetColDataY
/*!
  指定した位置の地形データを取得する
 
  @param pColObj   [in] データ元となるOBS_COLLISION_OBJポインタ
  @param lPosX     [in] X座標
  @param lPosY     [in] Y座標
  @param ucSuf     [in] BG面 0x80のビットが立っている場合はすり抜け床をすり抜ける
  @param pDir      [out] 角度情報
 
  @return   X軸の地形差分値
 
 */
// ================================================================
static s32 objGetColDataY( OBS_COLLISION_OBJ *pColObj, s32 lPosX, s32 lPosY, u16 ucSuf, u16* pDir, u32 *pAttr  )
{

    s8 cCol = 0;   // 地形差分値
    s32 ulPix; // キャラ辺りのドット位置
    u16 usCharNo; // キャラ番号

    // クリッピング
    if ( lPosX <  pColObj->left ||
         lPosX >= pColObj->right  )
        return cCol;
    if ( lPosY <  pColObj->top ||
         lPosY >= pColObj->bottom )
        return cCol;

    // 差分番号抽出 各座標と中心オフセット値から計算する
    {
        u16 x,y;
        x = (u16)( ( lPosX - pColObj->left ) >> 3 );
        y = (u16)( ( lPosY - pColObj->top  ) >> 3 );
        
        if ( pColObj->flag & OBD_COLOBJ_SYS_HFLIP )
            x = (u16)(((pColObj->width >> 3) - 1) - x);
        if ( pColObj->flag & OBD_COLOBJ_SYS_VFLIP )
            y = (u16)(((pColObj->height >> 3) - 1) - y);
        
        usCharNo = (u16)( x + y * (pColObj->width >> 3));
    }
    //usCharNo = (u16)( ((  lPosX - ( pColObj->pObj->lPosX >> 12) - cOfstX ) >> 3 )
    //                  + ((lPosY - ( pColObj->pObj->lPosY >> 12) - cOfstY ) >> 3 ) * (pColObj->width >> 3));

    // １キャラ辺りのドット位置を取得
    ulPix = ( lPosX - pColObj->left ) & 0x07;

    if ( pColObj->flag & OBD_COLOBJ_SYS_HFLIP )
        ulPix = 7 - ulPix; // 座標もVフリップ

    // 地形差分テーブルから指定座標の差分値を取得
    cCol =  pColObj->diff_data[ (usCharNo << 3) + ulPix ];
    cCol >>= 4; // X軸の差分値を削除
    cCol &=  0x0f;
    
    // 8bitのマイナス値に設定（データは4bitなので自前でマイナスにする）
    if (cCol & 0x08)
        cCol |= ~0x0f;

    // 指定差分値が地形のみの値（-8）なら絶対値を設定
    if (cCol == -8)
        cCol = 8;
    
    // すり抜け地形をすり抜け設定で、属性がすり抜け地形であれば地形は空にする
    if ( pColObj->attr_data ){
        if ((ucSuf & OBD_COL_THROUGH) && ((pColObj->attr_data[usCharNo >> 3] & OBD_COL_DATA_ATTR_THROUGH) || (pColObj->attr & OBD_COL_DATA_ATTR_THROUGH ) ) )
            cCol = 0;
    }else{
        if ((ucSuf & OBD_COL_THROUGH) && ( (pColObj->attr & OBD_COL_DATA_ATTR_THROUGH ) ) )
            cCol = 0;
    }
    if ( pColObj->flag & OBD_COLOBJ_SYS_VFLIP ){
        // 地形差分値をVフリップ
        if (!( cCol == 8 || cCol == 0 )){
            if ( cCol > 0 ){
                cCol -= 8;
            }else{
                cCol += 8;
            }
        }
    }
    

    // 角度取得チェック
    if ( pDir && cCol ){
        u16 ucDir;
        // 角度データを取得
        if ( pColObj->dir_data )
            ucDir = (u16)(*(pColObj->dir_data + usCharNo) << 8 );
        else
            ucDir = 0;

        if ( pColObj->flag & OBD_COLOBJ_DIR ){
            ucDir += pColObj->check_dir;
        }
        // 角度変化
        if ( pColObj->flag & OBD_COLOBJ_DIR_FLIP ){

           if ( (pColObj->flag & OBD_COLOBJ_SYS_HFLIP) )
                ucDir = (u16)(-(s16)ucDir);
            
            if ( pColObj->flag & OBD_COLOBJ_SYS_VFLIP )
                ucDir = (u16)(-((s16)ucDir + 0x4000) - 0x4000);
        }
        // 角度情報を設定
        *pDir = ucDir;
    }
    if ( pAttr && cCol ){
        if ( pColObj->attr_data )
            *pAttr = (u32)(pColObj->attr_data[usCharNo >> 3] | pColObj->attr);
        else
            *pAttr = (u32)pColObj->attr;
    }

    return cCol;
}
// ================================================================
// objMapGetDiff
/*!
  地表までの距離を求める
 
  @param lCol [in] 地形差分値
  @param sPix [in] キャラ辺りの座標位置
 
  @return   距離 ドット単位
 
  @note
  当たりあり
 */
// ================================================================
inline s32 objMapGetDiff( s32 lCol, s8 sPix, s8 sDelta )
{
    s32 lRet;

    if ( lCol > 0){
        if ( sDelta > 0 ){
            // 浮いている
            lRet = lCol - ( sPix + 1 );
        }else{
            // 埋まっている
            lRet = (8 - sPix);
        }
    }else{
        if ( sDelta > 0 ){
            // 埋まっている
            lRet = -(sPix + 1);
        }else{
            // 浮いている
            lRet = lCol + sPix;
        }
    }
    
    return lRet;
}


// ================================================================
// objMapGetForward
/*!
  地表までの距離を求める
 
  @param sPix [in] キャラ辺りの座標位置
  @param sDelta [in] 進行方向
 
  @return   距離 ドット単位
 
  @note
  当たりなし、次のキャラへ移行する 
 */
// ================================================================
inline s32 objMapGetForward(s8 sPix, s8 sDelta )
{
    s32 lRet;

    
    if ( sDelta > 0){
        
        lRet = 8 - sPix;
    }else{
        lRet = 1 + sPix;
    }
    
    return lRet;
}

// ================================================================
// objMapGetBack
/*!
  地表までの距離を求める
 
  @param sPix [in] キャラ辺りの座標位置
  @param sDelta [in] 進行方向
 
  @return   距離 ドット単位
 
  @note
  当たり全埋まり、次のキャラへ移行する 
 */
// ================================================================
inline s32 objMapGetBack(s8 sPix, s8 sDelta )
{
    s32 lRet;

    if ( sDelta > 0){
        lRet = -(sPix + 1);
    }else{
        lRet = sPix - 8;
    }
    
    return lRet;
}
// ================================================================
// objMapGetForwardRev
/*!
  地表までの距離を求める
 
  @param sPix [in] キャラ辺りの座標位置
  @param sDelta [in] 進行方向
 
  @return   距離 ドット単位
 
  @note
  当たりなしだが，次のキャラへは移行しない
 */
// ================================================================
inline s32 objMapGetForwardRev(s8 sPix, s8 sDelta )
{
    s32 lRet;

    if ( sDelta > 0){
        lRet = 8 - (sPix + 1);
    }else{
        lRet = sPix;
    }
    
    return lRet;
}
// ================================================================
// test_func
/*!
  テスト関数
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return   返値説明
 
  @note
  補足説明
 */
// ================================================================


/*
 * Revision 1.25  2005/09/27 07:02:50  use1146
 * フラグ修正
 *
 * Revision 1.24  2005/09/17 09:08:38  use1146
 * 角度設定可能状態追加
 *
 * Revision 1.23  2005/09/16 10:03:29  use1146
 * 向きが逆だったのを修正
 *
 * Revision 1.22  2005/09/16 07:42:40  use1146
 * 地形オブジェクトチェック関数追加
 *
 * Revision 1.21  2005/09/15 11:36:56  use1146
 * フリップオブジェクトの角度対応
 *
 * Revision 1.20  2005/09/06 14:00:51  use1173
 * プリコンパイルヘッダ対応
 *
 * Revision 1.19  2005/08/17 02:45:22  use1159
 * 下向き落下の修正
 *
 * Revision 1.18  2005/08/11 13:32:53  use1146
 * サイズが0のものはチェックしないように対応
 *
 * Revision 1.17  2005/08/08 09:07:32  use1146
 * オフセットなどをcharからshortへ変更
 *
 * Revision 1.16  2005/07/20 06:30:20  use1146
 * オブジェクト押し対応
 *
 * Revision 1.15  2005/06/28 12:27:29  use1146
 * 坂道処理修正
 *
 * Revision 1.14  2005/06/13 07:46:32  use1146
 * 乗り判定更新
 *
 * Revision 1.13  2005/06/10 11:10:40  use1146
 * BELT修正
 *
 * Revision 1.12  2005/05/31 08:53:11  use1146
 * 逆重力対応
 *
 * Revision 1.11  2005/05/26 07:47:21  use1146
 * 回転対応
 *
 * Revision 1.10  2005/05/12 11:59:11  use1146
 * すり抜けチェック守勢
 *
 * Revision 1.9  2005/03/29 05:33:21  use1146
 * 登録時にCLEARチェック追加
 *
 * Revision 1.8  2005/03/15 08:47:57  use1146
 * 1キャラの地形に対応
 *
 * Revision 1.7  2005/03/10 11:27:53  use1146
 * フリップバグ修正
 *
 * Revision 1.6  2005/03/08 11:11:53  use1146
 * RIDE作成
 *
 * Revision 1.5  2005/03/01 05:32:29  use1146
 * RIDE対応
 *
 * Revision 1.4  2005/02/21 09:16:34  use1146
 * 属性取得対応
 *
 * Revision 1.3  2005/02/10 10:22:49  use1146
 * FLIP対応
 *
 * Revision 1.2  2005/02/03 05:53:38  use1146
 * nxテーブルや、オフセットなどのバグ修正
 *
 * Revision 1.1  2005/01/20 03:09:28  use1146
 * 登録
 *
 */