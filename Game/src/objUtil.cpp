// ================================================================
/*!
  @file objUtil.c
  @brief テンプレート(簡単なファイル説明)

  @author mana
                Copyright(c) 2006 Dimps
  $Id: objUtil.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */

//----- Include Files --------------------------------------------------
#include "pch.h"
#include "fx.h"
#include "objUtil.h"

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------

//----- External Declarations ------------------------------------------

//----- Static Declarations --------------------------------------------
//static void objUtilTblWorkMove( OBS_OBJECT_WORK* pObj, OBS_TBL_WORK * pTbl );
//static void objUtilTblWorkAct( OBS_OBJECT_WORK* pObj, OBS_TBL_WORK * pTbl );
//static void objUtilTblWorkScale( OBS_OBJECT_WORK* pObj, OBS_TBL_WORK * pTbl );
//static void objUtilTblWorkDir( OBS_OBJECT_WORK* pObj, OBS_TBL_WORK * pTbl );

//----- Global Variables -----------------------------------------------
u32 _obj_disp_rand;

//----- Local Variables ------------------------------------------------

//----- Global Functions -----------------------------------------------
// ================================================================
// ObjSpdUpSet
/*!
  オブジェクト加速セット関数
 
  @param lSpd    [in] 現在の速度
  @param sSpd    [in] 加速度
  @param sMaxSpd [in] 最大速度度絶対値 （0で無視)

  @return 速度
 */
// ================================================================
s32 ObjSpdUpSet( s32 lSpd, s32 sSpd, s32 sMaxSpd )
{
    lSpd += FX_Mul(sSpd, g_obj.speed);
    
    if ( sMaxSpd == 0 )
        return lSpd;
    
    // 最大速度チェック
    if ( sSpd >= 0 ){
        if ( lSpd > sMaxSpd ){
            lSpd = sMaxSpd;
        }
    }else{
        if ( lSpd < -sMaxSpd ){
            lSpd = -sMaxSpd;
        }
    }
    return lSpd;
}
// ================================================================
// ObjSpdDownSet
/*!
  オブジェクト減速セット関数
 
  @param lSpd   [in] 現在の速度
  @param sSpd   [in] 減速度

  @return 速度
 */
// ================================================================
s32 ObjSpdDownSet( s32 lSpd, s32 sSpd )
{
    // 速度0チェック
    if ( lSpd > 0 ){
        lSpd -= FX_Mul(sSpd, g_obj.speed);
        if ( lSpd < 0 )
            lSpd = 0;
    }else{
        lSpd += FX_Mul(sSpd, g_obj.speed);
        if ( lSpd > 0 )
            lSpd = 0;
    }
    return lSpd;
}
// ================================================================
// ObjShiftSet
/*!
  目標値シフト計算関数 移動感覚 > Src|       |   | |||Tag
 
  @param lPos    [in] 現在の数値
  @param sTag    [in] 目標値
  @param usShift [in] シフト値
  @param usMax   [in] 最高移動値（0で無視
  @param usMin   [in] 最低移動値（0で無視

  @return 速度
 */
// ================================================================
s32 ObjShiftSet( s32 lPos, s32 sTag, u16 usShift, s32 usMax, s32 usMin )
{
    s32 lTempMove; // 移動量

    // 既に々
    if ( lPos == sTag )
        return lPos;
    if ( !usMin )
        usMin = 1;
    
    // 移動量計算
    lTempMove = ( sTag - lPos) >> usShift;

    // 最高移動値チェック
    if ( usMax ){
        if ( lTempMove > usMax ){
            lTempMove = usMax;
        }
        if ( lTempMove < -usMax ){
            lTempMove = -usMax;
        }
    }
    // 最低移動値チェック
    if ( usMin ){
        if ( lTempMove > 0 ){
            if ( lTempMove < usMin ){
                lTempMove = usMin;
            }
        }else if ( lTempMove < 0 ){
            if ( lTempMove > -usMin ){
                lTempMove = -usMin;
            }
        }else{
            if ( sTag - lPos > 0 )
                if ( lTempMove < usMin )
                    lTempMove = usMin;
            if ( sTag - lPos < 0 )
                if ( lTempMove > -usMin )
                    lTempMove = -usMin;
        }
    }
    
    // 移動
    lPos += lTempMove;
    
    // フローチェック
    if ( lTempMove > 0 ){
        if ( lPos > sTag )
            lPos = sTag;
    } else if ( lTempMove < 0 ){
        if ( lPos < sTag )
            lPos = sTag;
    }
    return lPos;
}
// ================================================================
// ObjDiffSet
/*!
  目標値差分計算関数 移動感覚 > Src||| |   |       |Tag
 
  @param lPos    [in] 現在の数値
  @param sTag    [in] 目標値
  @param sSrc    [in] 最初の値
  @param usShift [in] 移動値のシフト値（高い程ゆっくり移動）
  @param usMax   [in] 最高移動値（0で無視
  @param usMin   [in] 最低移動値（0で無視,1として扱う

  @return 速度
 */
// ================================================================
s32 ObjDiffSet( s32 lPos, s32 sTag, s32 sSrc, u16 usShift, s32 usMax, s32 usMin )
{
    s32 lTempMove; // 移動量

    // 既に々
    if ( lPos == sTag )
        return lPos;
    if ( !usMin )
        usMin = 1;

    
    
    lTempMove = ( lPos - sSrc );
    lTempMove >>= usShift;

    if ( sTag > sSrc ){
        if ( lTempMove < 0 )
            lTempMove = 0;
    }
    if ( sTag < sSrc ){
        if ( lTempMove > 0 )
            lTempMove = 0;
    }
    
    // 最高移動値チェック
    if ( usMax ){
        if ( lTempMove > usMax ){
            lTempMove = usMax;
        }
        if ( lTempMove < -usMax ){
            lTempMove = -usMax;
        }
    }
    // 最低移動値チェック
    if ( usMin ){
        if ( lTempMove > 0 ){
            if ( lTempMove < usMin ){
                lTempMove = usMin;
            }
        }else if ( lTempMove < 0 ){
            if ( lTempMove > -usMin ){
                lTempMove = -usMin;
            }
        }else{
            if ( sTag - lPos > 0 )
                if ( lTempMove < usMin )
                    lTempMove = usMin;
            if ( sTag - lPos < 0 )
                if ( lTempMove > -usMin )
                    lTempMove = -usMin;
        }
    }
    
    // 移動
    lPos += lTempMove;

    // フローチェック
    if ( lTempMove > 0 ){
        if ( lPos > sTag )
            lPos = sTag;
    } else if ( lTempMove < 0 ){
        if ( lPos < sTag )
            lPos = sTag;
    }
    return lPos;
}
// ================================================================
// ObjAlphaSet
/*!
  線形補完移動
 
  @param sTag    [in] 目標値
  @param sSrc    [in] 元の位置
  @param usAlpha [in] α値 0x1000で目標値

  @return 値
 */
// ================================================================
s32 ObjAlphaSet( s32 sTag, s32 sSrc, u16 usAlpha )
{
    s32 lPos;
    s32 lTempMove; // 移動量
    
    if ( !usAlpha )
        return sSrc;
    if ( usAlpha == 0x1000 )
        return sTag;

    // 計算
    lTempMove = FX_Mul((sTag - sSrc), usAlpha);
    lPos = sSrc + lTempMove;
    
    return lPos;
}
// ================================================================
// ObjRoopMove8
/*!
  0x100でマスクループする数値を目標値へ速度分近づける

    @param ucDir [in] 今の数値
    @param ucTag [in] 目標値
    @param cSpd  [in] 移動値

    @return 目標値へ速度分近づいた数値
 */
// ================================================================
u8 ObjRoopMove8( u8 ucDir, u8 ucTag, s8 cSpd )
{
    u8 ucDir1, ucDir2; // 比較用ワーク

    if ( ucTag == ucDir )
        return ucTag;
    
    // 距離計算
    ucDir1 = (u8)MTM_MATH_ABS( ucDir - ucTag );

    // 0をまたいだ場合の距離計算
    if ( ucDir > ucTag )
        ucDir2 = (u8)((0x100 - ucDir) + ucTag);
    else
        ucDir2 = (u8)((0x100 - ucTag) + ucDir);

    // 近い方へ移動
    if ( ucDir1 <= ucDir2 ){
        if ( ucDir > ucTag ){
            // 先にフローチェック
            if ( ucTag > ucDir - cSpd)
                ucDir = ucTag;
            else
                ucDir -= cSpd;
            
        }else if ( ucDir < ucTag ) {
            // 先にフローチェック
            if ( ucTag < ucDir + cSpd)
                ucDir = ucTag;
            else
                ucDir += cSpd;
        }
    }else {
        // 0経由の方が近い場合、
        if ( ucDir > ucTag ){
            // 先にフローチェック
            if ( ucTag + 0x100 < ucDir + cSpd)
                ucDir = ucTag;
            else
                ucDir += cSpd;
            
        }else if ( ucDir < ucTag ) {
            // 先にフローチェック
            if ( ucTag > ucDir - cSpd + 0x100)
                ucDir = ucTag;
            else
                ucDir -= cSpd;
        }        
    }
    return ucDir;
}
// ================================================================
// ObjRoopMove16
/*!
  0x10000でマスクループする数値を目標値へ速度分近づける

    @param ucDir [in] 今の数値
    @param ucTag [in] 目標値
    @param cSpd  [in] 移動値

    @return 目標値へ速度分近づいた数値
 */
// ================================================================
u16 ObjRoopMove16( u16 ucDir, u16 ucTag, s16 cSpd )
{
    u16 ucDir1, ucDir2; // 比較用ワーク

    if ( ucTag == ucDir )
        return ucTag;

    // 距離計算
    ucDir1 = (u16)MTM_MATH_ABS( ucDir - ucTag );

    // 0をまたいだ場合の距離計算
    if ( ucDir > ucTag )
        ucDir2 = (u16)((0x10000 - ucDir) + ucTag);
    else
        ucDir2 = (u16)((0x10000 - ucTag) + ucDir);

    // 近い方へ移動
    if ( ucDir1 <= ucDir2 ){
        if ( ucDir > ucTag ){
            // 先にフローチェック
            if ( ucTag > ucDir - cSpd)
                ucDir = ucTag;
            else
                ucDir -= cSpd;
            
        }else if ( ucDir < ucTag ) {
            // 先にフローチェック
            if ( ucTag < ucDir + cSpd)
                ucDir = ucTag;
            else
                ucDir += cSpd;
        }
    }else {
        // 0経由の方が近い場合、
        if ( ucDir > ucTag ){
            // 先にフローチェック
            if ( ucTag + 0x10000 < ucDir + cSpd)
                ucDir = ucTag;
            else
                ucDir += cSpd;
            
        }else if ( ucDir < ucTag ) {
            // 先にフローチェック
            if ( ucTag > ucDir - cSpd + 0x10000)
                ucDir = ucTag;
            else
                ucDir -= cSpd;
        }        
    }
    return ucDir;
}
// ================================================================
// ObjRoopDiff16
/*!
  0x10000でマスクループする数値同士の小さい方の差を求める

    @param usDir1 [in] 数値１
    @param usDir2 [in] 数値２
    
    @return 差
 */
// ================================================================
s16 ObjRoopDiff16( u16 usDir1, u16 usDir2 )
{
    s16 length1,length2;

    // 同じ
    if ( usDir2 == usDir1 )
        return 0;

    // 距離計算
    length1 = (s16)( usDir1 - usDir2 );

    // 0をまたいだ場合の距離計算
    if ( usDir1 > usDir2 )
        length2 = (s16)((0x10000 - usDir1) + usDir2);
    else
        length2 = (s16)((0x10000 - usDir2) + usDir1);

    if ( MTM_MATH_ABS(length1) > MTM_MATH_ABS(length2) )
        return length2;
    return length1;
}
// ================================================================
// ObjSwingEndMove
/*!
  傾き終了処理

  @param pWork      [in] オブジェクトワークポインタ
  @param lSwingWork [in] 現在傾き値
  @param sSpdAdd    [in] 傾き速度増加値
  @param sSpdDow    [in] 傾き速度減算値
  @param sSpdMax    [in] 傾き最大速度

  @return 傾き値
 */
// ================================================================
s32 ObjSwingEndMove( OBS_OBJECT_WORK* pWork, s32 lSwingWork, s32 sSpdAdd, s32 sSpdDow, s32 sSpdMax )
{

    if ( MTM_MATH_ABS( lSwingWork ) < 0x1000 && MTM_MATH_ABS( pWork->spd_m ) < 0x0800 ){
        lSwingWork = 0;
        pWork->spd_m = 0;
    }else{
        if ( lSwingWork < 0 ){
            if ( pWork->spd_m > 0 )
                pWork->spd_m = ObjSpdDownSet( pWork->spd_m, sSpdDow );
            // 加速
            pWork->spd_m = ObjSpdUpSet( pWork->spd_m,-sSpdAdd, sSpdMax);

        }else{
            if ( pWork->spd_m < 0 )
                pWork->spd_m = ObjSpdDownSet( pWork->spd_m, sSpdDow );
            // 加速
            pWork->spd_m = ObjSpdUpSet( pWork->spd_m, sSpdAdd, sSpdMax);
        }
        lSwingWork -= pWork->spd_m;
        pWork->dir.z = (u16)( lSwingWork & 0xffff );
    }

    return lSwingWork;
}
// ================================================================
// ObjNumCodeSet
/*!
  数値を最大値に従って数値コード（ゾーンパック16進）に変換する
 
  @param    pNum  [out] 数値コード設定先（最大8桁）
  @param    sNum  [in]  設定する数値
  @param    ulMax [in]  最大値（ただし10の倍数である事） 100 と設定した場合99が最大となる
 
 */
// ================================================================
void ObjNumCodeSet( u32* pNum, u32 sNum, u32 ulMax)
{
    u32 time = 0; // ワーク
    s16 i = 0,j = 0;
    u32 ulTemp1 = 0;
    u32 ulTemp2 = 0;

    MTM_ASSERT( pNum );

#if defined (MTD_DEBUG)
	if (!(ulMax % 10)) {	// Warningよけ
		MTM_ASSERT(0);
	}
#endif
    
    // 表示最大値
    if ( sNum >= ulMax ){
        if ( ulMax == 0)
            sNum = 0;
        else
            sNum =   ulMax - 1;
    }
    *pNum = 0;
    
    if ( ulMax >= 100 ){
        ulMax   = (u32)FX_DivS32( (s32)ulMax, 10 );
        
        for ( i = 0; ; ) {
            if ( sNum >= ulMax ) {
                time    = (u32)FX_DivS32( (s32)sNum, (s32)ulMax );
                // セット
                *pNum |= time << i;
                sNum -= time * ulMax;
            }
            i+= 4;
            if ( ulMax <= 10 )
                break;
            ulMax   = (u32)FX_DivS32( (s32)ulMax, 10 );
        }
    }
    *pNum |= sNum << i;
    j = i;
    // 入れ替え
    for ( ; i >= (j >>1); i-=4){
        ulTemp1 = ulTemp2 = 0;

        ulTemp1 = (((*pNum & ( 0xf << ( j-i))) >> ( j-i) ) << i);
        ulTemp2 = (((*pNum & ( 0xf << i )) >> i) << (j-i));

        *pNum &= ~((0xf << ( j -i))| (0xf << ( i )));

        *pNum |= ulTemp1 | ulTemp2;
        
    }
}
// ================================================================
// objObjectTouchCheck
/*!
  タッチパネルOnと矩形のチェックを行う
 
  @param pWork [in] オブジェクトワークポインタ
  @param index [in] 対象矩形番号

  @return 0 NOHIT 1 HIT
 */
// ================================================================
u16 ObjObjectTouchCheck( OBS_OBJECT_WORK *pObj, u16 index )
{
    OBS_RECT_WORK* pRect;
    pRect = ObjObjectRectGet(pObj, index);
    
    if ( pRect ){
        return ObjTouchCheck( pObj, pRect );
    }
    return FALSE;
}

// ================================================================
// objObjectTouchCheckPush
/*!
  タッチパネルPushと矩形のチェックを行う
 
  @param pWork [in] オブジェクトワークポインタ
  @param index [in] 対象矩形番号

  @return 0 NOHIT 1 HIT
 */
// ================================================================
u16 ObjObjectTouchCheckPush( OBS_OBJECT_WORK *pObj, u16 index )
{
    OBS_RECT_WORK* pRect;
    pRect = ObjObjectRectGet(pObj, index);
    
    if ( pRect ){
        return (u16)ObjTouchCheckPush( pObj, pRect );
    }
    return FALSE;
}

// ================================================================
// objTouchCheck
/*!
  タッチパネルOnと矩形のチェックを行う
 
  @param pWork [in] オブジェクトワークポインタ
  @param pRect [in] 対象矩形

  @return 0 NOHIT 1 HIT
 */
// ================================================================
u16 ObjTouchCheck( OBS_OBJECT_WORK *pWork, OBS_RECT_WORK* pRect )
{
#if defined _DS
    s32 sMapPosX, sMapPosY;

    if (mtTpIsTouchOn()) {
        pRect->parent_obj = pWork;

        if ( g_obj.camera[0][MTD_Y] > g_obj.camera[1][MTD_Y] ){
            sMapPosX = (g_obj.camera[0][MTD_X] >> FX32_SHIFT);
            sMapPosY = (g_obj.camera[0][MTD_Y] >> FX32_SHIFT);
        }else{
            sMapPosX = (g_obj.camera[1][MTD_X] >> FX32_SHIFT);
            sMapPosY = (g_obj.camera[1][MTD_Y] >> FX32_SHIFT);
        }
        return ObjRectWorkPointCheck( pRect,
                                      (_mt_tp_touch.on[MTD_X] + sMapPosX),
                                      (_mt_tp_touch.on[MTD_Y] + sMapPosY), 0 );
    }
    return (0);
#elif defined _IPHONE
    s32 sMapPosX, sMapPosY;
	
	// 現在はシングルのみ
    if (amTpIsTouchOn(0)) {
        pRect->parent_obj = pWork;
		
        if ( g_obj.camera[0][MTD_Y] > g_obj.camera[1][MTD_Y] ){
            sMapPosX = (g_obj.camera[0][MTD_X] >> FX32_SHIFT);
            sMapPosY = (g_obj.camera[0][MTD_Y] >> FX32_SHIFT);
        }else{
            sMapPosX = (g_obj.camera[1][MTD_X] >> FX32_SHIFT);
            sMapPosY = (g_obj.camera[1][MTD_Y] >> FX32_SHIFT);
        }
        return ObjRectWorkPointCheck( pRect,
									 (_am_tp_touch[0].on[MTD_X] * OBD_LCD_X / AMD_DISPLAY_WIDTH + sMapPosX),
									 (_am_tp_touch[0].on[MTD_Y] * OBD_LCD_Y / AMD_DISPLAY_HEIGHT + sMapPosY), 0 );
    }
    return (0);
#else
	// ◆暫定カット
	UNREFERENCED_PARAMETER(pWork);
	UNREFERENCED_PARAMETER(pRect);
	return (0);
#endif // _DS
}

// ================================================================
// ObjTouchCheckPush
/*!
  タッチパネルPushと矩形のチェックを行う
 
  @param pWork [in] オブジェクトワークポインタ
  @param pRect [in] 対象矩形

  @return 0 NOHIT 1 HIT
 */
// ================================================================
u16 ObjTouchCheckPush( OBS_OBJECT_WORK *pWork, OBS_RECT_WORK* pRect )
{
#if defined _DS
    s32 sMapPosX, sMapPosY;

    if (mtTpIsTouchPush()) {
        pRect->parent_obj = pWork;

        if ( g_obj.camera[0][MTD_Y] > g_obj.camera[1][MTD_Y] ){
            sMapPosX = (g_obj.camera[0][MTD_X] >> FX32_SHIFT);
            sMapPosY = (g_obj.camera[0][MTD_Y] >> FX32_SHIFT);
        }else{
            sMapPosX = (g_obj.camera[1][MTD_X] >> FX32_SHIFT);
            sMapPosY = (g_obj.camera[1][MTD_Y] >> FX32_SHIFT);
        }
        return ObjRectWorkPointCheck( pRect,
                                      (_mt_tp_touch.on[MTD_X] + sMapPosX),
                                      (_mt_tp_touch.on[MTD_Y] + sMapPosY), 0 );
    }
    return (0);
#elif defined _IPHONE
    s32 sMapPosX, sMapPosY;
	
	// 現在はシングルのみ
    if (amTpIsTouchPush(0)) {
        pRect->parent_obj = pWork;
		
        if ( g_obj.camera[0][MTD_Y] > g_obj.camera[1][MTD_Y] ){
            sMapPosX = (g_obj.camera[0][MTD_X] >> FX32_SHIFT);
            sMapPosY = (g_obj.camera[0][MTD_Y] >> FX32_SHIFT);
        }else{
            sMapPosX = (g_obj.camera[1][MTD_X] >> FX32_SHIFT);
            sMapPosY = (g_obj.camera[1][MTD_Y] >> FX32_SHIFT);
        }
        return ObjRectWorkPointCheck( pRect,
									 (_am_tp_touch[0].on[MTD_X] * OBD_LCD_X / AMD_DISPLAY_WIDTH + sMapPosX),
									 (_am_tp_touch[0].on[MTD_Y] * OBD_LCD_Y / AMD_DISPLAY_HEIGHT + sMapPosY), 0 );
    }
    return (0);
#else
	// ◆暫定カット
	UNREFERENCED_PARAMETER(pWork);
	UNREFERENCED_PARAMETER(pRect);
	return (0);
#endif // _DS
}

// ================================================================
// ObjUtilGetRotPosXY
/*!
  XYをZ軸で回転した値の取得
 
  @param pWork [in] オブジェクトワークポインタ
  @param pRect [in] 対象矩形

  @return 0 NOHIT 1 HIT
 */
// ================================================================
void ObjUtilGetRotPosXY(fx32 pos_x, fx32 pos_y, fx32 *dest_x, fx32 *dest_y, u16 dir)
{
	fx32	x_sin, x_cos, y_sin, y_cos;

	MTM_ASSERT(dest_x && dest_y);

	x_sin = FX_Mul(pos_x, mtMathSin(dir));
	x_cos = FX_Mul(pos_x, mtMathCos(dir));
	y_sin = FX_Mul(pos_y, mtMathSin(dir));
	y_cos = FX_Mul(pos_y, mtMathCos(dir));

	*dest_x = x_cos - y_sin;
	*dest_y = x_sin + y_cos;
}


#ifndef _DS
// ================================================================
// ObjSpdUpSetF
/*!
  オブジェクト加速セット関数
 
  @param spd		[in] 現在の速度
  @param add_apd    [in] 加速度
  @param max_spd	[in] 最大速度度絶対値 （0で無視)

  @return 速度
 */
// ================================================================
float ObjSpdUpSetF(float spd, float add_apd, float max_spd )
{
    spd += add_apd * FXM_FX32_TO_FLOAT(g_obj.speed);
    
    if ( amIsZerof(max_spd) )
        return spd;
    
    // 最大速度チェック
    if ( add_apd >= 0.000f ){
        if ( spd > max_spd ){
            spd = max_spd;
        }
    }else{
        if ( spd < -max_spd ){
            spd = -max_spd;
        }
    }
    return spd;
}

// ================================================================
// ObjSpdDownSetF
/*!
  オブジェクト減速セット関数
 
  @param lSpd   [in] 現在の速度
  @param sSpd   [in] 減速度

  @return 速度
 */
// ================================================================
float ObjSpdDownSetF(float spd, float spd_dec)
{
    // 速度0チェック
    if ( spd > 0.000f ){
        spd -= spd_dec * FXM_FX32_TO_FLOAT(g_obj.speed);
        if ( spd < 0.000f )
            spd = 0.f;
    }else{
        spd += spd_dec * FXM_FX32_TO_FLOAT(g_obj.speed);
        if ( spd > 0.000f )
            spd = 0.f;
    }
    return spd;
}

// ================================================================
// ObjShiftSetF
/*!
  目標値シフト計算関数 移動感覚 > Src|       |   | |||Tag
 
  @param pos	[in] 現在の数値
  @param tag	[in] 目標値
  @param shift	[in] シフト値
  @param max	[in] 最高移動値（0で無視
  @param min	[in] 最低移動値（0で無視

  @return 速度
 */
// ================================================================
float ObjShiftSetF( float pos, float tag, s32 shift, float max, float min )
{
    float temp_move; // 移動量

    // 既に々
    if ( pos == tag )
        return pos;
    if ( !min )
        min = 1.f;
    
    // 移動量計算
    temp_move = (tag - pos) / (1 << shift);

    // 最高移動値チェック
    if ( max ){
        if ( temp_move > max ){
            temp_move = max;
        }
        if ( temp_move < -max ){
            temp_move = -max;
        }
    }
    // 最低移動値チェック
    if ( min ){
        if ( temp_move > 0.000f ){
            if ( temp_move < min ){
                temp_move = min;
            }
        }else if ( temp_move < 0.000f ){
            if ( temp_move > -min ){
                temp_move = -min;
            }
        }else{
            if ( tag - pos > 0.000f )
                if ( temp_move < min )
                    temp_move = min;
            if ( tag - pos < 0.000f )
                if ( temp_move > -min )
                    temp_move = -min;
        }
    }
    
    // 移動
    pos += temp_move;
    
    // フローチェック
    if ( temp_move > 0.000f ){
        if ( pos > tag )
            pos = tag;
    } else if ( temp_move < 0.000f ){
        if ( pos < tag )
            pos = tag;
    }
    return pos;
}
#endif // #ifndef _DS


//----- Local Functions ------------------------------------------------
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

