// ================================================================
/*!
  @file objUtil.h
  @brief オブジェクト用ユーティリティ

  @author mana
                Copyright(c) 2006 Dimps

  $Id: objUtil.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */

/*!
  @page obj_util obj オブジェクト用ユーティリティ
 
  @section obj_util_plain テーブルワーク
    　 あらかじめ用意したテーブルの設定通りにオブジェクトを動かす。\n
    　 #OBS_TBL_WORK を用意し、( objObjectTblWorkSet() ) \n
    　 データテーブルを メモリ上に用意した場合 objUtilTblWorkActSet() を\n
    　 ファイルで用意した場合 ObjUtilTblWorkActLoad()で設定する。\n
    　 あとはメイン処理の中で ObjObjectUtilTblWork() を呼び出すと、指定の動作が行われる。\n
    　 開放はオブジェクト死亡時に自動で行われる\n
    \n
  @section obj_object_rand 乱数
    　表示のみでゲーム進行に影響しない事象には ObjDispRand() を用いる。\n
    　通信プレイ時での乱数同期ズレを軽減させる。\n
    \n
  @section obj_object_num_code 数値をコード化
    　 ObjNumCodeSet() を使って 数値データを10進数の1桁ずつのゾーンパック進数に変換できる\n
    　 数値をゲーム中に表示する時に便利\n
    \n

  @sa objUtil.h objUtil.cpp
 */

#ifndef _H_OBJUTIL
#define _H_OBJUTIL


#include "objObject.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------

//----- External Declarations ------------------------------------------
extern u32 _obj_disp_rand; ///< オブジェ乱数値 表示用のため、同期を気にしない


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
s32 ObjSpdUpSet( s32 lSpd, s32 sSpd, s32 sMaxSpd );

// ================================================================
// ObjSpdDownSet
/*!
  オブジェクト減速セット関数
 
  @param lSpd   [in] 現在の速度
  @param sSpd   [in] 減速度

  @return 速度
 */
// ================================================================
s32 ObjSpdDownSet( s32 lSpd, s32 sSpd );

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
s32 ObjShiftSet( s32 lPos, s32 sTag, u16 usShift, s32 usMax, s32 usMin );

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
s32 ObjDiffSet( s32 lPos, s32 sTag, s32 sSrc, u16 usShift, s32 usMax, s32 usMin );

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
s32 ObjAlphaSet( s32 sTag, s32 sSrc, u16 usAlpha );

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
u8 ObjRoopMove8( u8 ucDir, u8 ucTag, s8 cSpd );

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
u16 ObjRoopMove16( u16 ucDir, u16 ucTag, s16 cSpd );

// ================================================================
// ObjRoopDiff16
/*!
  0x10000でマスクループする数値同士の小さい方の差を求める

    @param usDir1 [in] 数値１
    @param usDir2 [in] 数値２
    
    @return 差
 */
// ================================================================
s16 ObjRoopDiff16( u16 usDir1, u16 usDir2 );

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

  @note
    オブジェワークのsSpdMを使用します
 */
// ================================================================
s32 ObjSwingEndMove( OBS_OBJECT_WORK* pWork, s32 lSwingWork, s32 sSpdAdd, s32 sSpdDow, s32 sSpdMax );

// ================================================================
// ObjNumCodeSet
/*!
  数値を最大値に従って数値コード（ゾーンパック16進）に変換する
 
  @param    pNum  [out] 数値コード設定先（最大8桁）
  @param    sNum  [in]  設定する数値
  @param    ulMax [in]  最大値（ただし10の倍数である事） 100 と設定した場合99が最大となる
 
 */
// ================================================================
void ObjNumCodeSet( u32* pNum, u32 sNum, u32 ulMax);

// ================================================================
// ObjDispSRand
/*!
  表示用ランダムシードを設定する、キー通信時にずれが生じても構わないものにのみ使用
  
  @param    seed    [in] ランダムシード
 */
// ================================================================
inline void ObjDispSRand( u32 seed )
{
    _obj_disp_rand   = seed;
}
// ================================================================
// ObjDispRand
/*!
  表示用ランダム値を取得する、キー通信時にずれが生じても構わないものにのみ使用
  
  @return   ランダム値
 */
// ================================================================
inline u16 ObjDispRand( void )
{
    _obj_disp_rand   = 1663525L * _obj_disp_rand + 1013904223L;
    return (u16)( _obj_disp_rand >> 16 );
}

// ================================================================
// ObjObjectTouchCheck
/*!
  タッチパネルOnと矩形のチェックを行う
 
  @param pWork [in] オブジェクトワークポインタ
  @param index [in] 対象矩形番号

  @return 0 NOHIT 1 HIT
 */
// ================================================================
u16 ObjObjectTouchCheck( OBS_OBJECT_WORK *pObj, u16 index );

// ================================================================
// objObjectTouchCheckPush
/*!
  タッチパネルPushと矩形のチェックを行う
 
  @param pWork [in] オブジェクトワークポインタ
  @param index [in] 対象矩形番号

  @return 0 NOHIT 1 HIT
 */
// ================================================================
u16 ObjObjectTouchCheckPush( OBS_OBJECT_WORK *pObj, u16 index );

// ================================================================
// ObjTouchCheck
/*!
  タッチパネルOnと矩形のチェックを行う
 
  @param pWork [in] オブジェクトワークポインタ
  @param pRect [in] 対象矩形

  @return 0 NOHIT 1 HIT
 */
// ================================================================
u16 ObjTouchCheck( OBS_OBJECT_WORK *pWork, struct _OBS_RECT_WORK* pRect );

// ================================================================
// ObjTouchCheckPush
/*!
  タッチパネルPushと矩形のチェックを行う
 
  @param pWork [in] オブジェクトワークポインタ
  @param pRect [in] 対象矩形

  @return 0 NOHIT 1 HIT
 */
// ================================================================
u16 ObjTouchCheckPush( OBS_OBJECT_WORK *pWork, struct _OBS_RECT_WORK* pRect );

// ================================================================
// ObjUtilGetRotPosXY
/*!
  XYをZ軸で回転した値の取得
 
  @param pWork [in] オブジェクトワークポインタ
  @param pRect [in] 対象矩形

  @return 0 NOHIT 1 HIT
 */
// ================================================================
extern void ObjUtilGetRotPosXY(fx32 pos_x, fx32 pos_y, fx32 *dest_x, fx32 *dest_y, u16 dir);


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
extern float ObjSpdUpSetF(float spd, float add_apd, float max_spd );

// ================================================================
// ObjSpdDownSetF
/*!
  オブジェクト減速セット関数
 
  @param lSpd   [in] 現在の速度
  @param sSpd   [in] 減速度

  @return 速度
 */
// ================================================================
extern float ObjSpdDownSetF(float spd, float spd_dec);

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
extern float ObjShiftSetF( float pos, float tag, s32 shift, float max, float min );
#endif

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _H_OBJUTIL
