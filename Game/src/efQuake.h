// ================================================================
/*!
  @file efQuake.h
  @brief 振動

  @author mana
                Copyright(c) 2004 Dimps

  $Id: efQuake.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 *
 */
/*!
  @page  eff_quake eff 振動
 
  @section eff_quake 使い方
    　 efQuake() を呼び出し、その後 EfQuakeGetX() 、 EfQuakeGetY() で振動値を取得、\n
    　その値をそれぞれのスプライトやBGに足しこむ。\n
    \n
    　または EfQuakeCycle() で減衰振動を起こす事ができる、こちらはテーブルによる振動ではなく、計算によって振動値を求める\n
    　これも EfQuakeGetX() 、 EfQuakeGetY() で振動値を取得する。\n
    \n
    　振動終了をチェックするには efQuake(EFD_QUAKE_CHECK)で返り値をチェックする\n
  @sa efQuake.c efQuake.h
*/

#ifndef _H_EFQUAKE
#define _H_EFQUAKE



#ifndef _DS
#include "fx.h"
#include "typedef.h"
#endif

#include "efEffect.h"


#if	defined(__cplusplus)
extern "C" {
#endif
//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

/*----- Definitions ----------------------------------------------------*/

/// 地震 構造体 
typedef struct tag_EFS_QUAKE_WORK {
    // 計算用
    s32 spd;		///< 振動値     1:19:12
    s32 spd_cycle;	///< 振動減速値 1:19:12
    s32 spd_down;	///< 振動減速値 1:19:12
    u32	 angle;		///< 周期

    u8  size;		///< 地震大きさ
    u8  dummy;		///< ダミー(align合わせ)

    u16 flag;		///< 画面振動フラグ
    s16 timer;		///< 地震タイマー

    s32 vib_x;		///< 振動値X 1:19:12
    s32 vib_y;		///< 振動値Y 1:19:12
} EFS_QUAKE_WORK;

// 地震フラグ EFS_QUAKE_WORK::flag
#define EFD_QUAKE_FLAG_RUN      ( 1 <<  0 )  ///< 地震実行
#define EFD_QUAKE_FLAG_CONTINUE ( 1 <<  1 )  ///< 継続地震フラグ
#define EFD_QUAKE_FLAG_STOP     ( 1 <<  2 )  ///< 一時停止フラグ

// 振動種類 EFS_QUAKE_WORK::size
#define EFD_QUAKE_END      ( 0 ) ///< 地震 強制終了
#define EFD_QUAKE_S_MINI   ( 1 ) ///< 地震 極小一瞬
#define EFD_QUAKE_S_SMALL  ( 2 ) ///< 地震 小一瞬
#define EFD_QUAKE_S_MIDDLE ( 3 ) ///< 地震 中一瞬
#define EFD_QUAKE_S_BIG    ( 4 ) ///< 地震 大一瞬
#define EFD_QUAKE_S_HUGE   ( 5 ) ///< 地震 巨大一瞬
#define EFD_QUAKE_M_MINI   ( 6 ) ///< 地震 極小
#define EFD_QUAKE_M_SMALL  ( 7 ) ///< 地震 小
#define EFD_QUAKE_M_MIDDLE ( 8 ) ///< 地震 中
#define EFD_QUAKE_M_BIG    ( 9 ) ///< 地震 大
#define EFD_QUAKE_M_HUGE   (10 ) ///< 地震 巨大
#define EFD_QUAKE_L_MINI   (11 ) ///< 地震 極小継続
#define EFD_QUAKE_L_SMALL  (12 ) ///< 地震 小継続
#define EFD_QUAKE_L_MIDDLE (13 ) ///< 地震 中継続
#define EFD_QUAKE_L_BIG    (14 ) ///< 地震 大継続
#define EFD_QUAKE_L_HUGE   (15 ) ///< 地震 巨大継続
#define EFD_QUAKE          (16 ) ///< 減衰振動
#define EFD_QUAKE_CHECK    (17 ) ///< 振動終了チェック

// EFS_QUAKE_WORK::spd_cycle
#define EFD_QUAKE_CYCLE_ONE ( 1 << 12 )
#define EFD_QUAKE_CYCLE ( EFD_QUAKE_CYCLE_ONE * 0x4000 ) ///< 標準周期値

#define EFD_QUAKE_TIME_SHORT   ( 8 )  ///< 一瞬の振動 時間
#define EFD_QUAKE_TIME_SMALL   ( 12 ) ///< 短い振動 時間
#define EFD_QUAKE_TIME_MIDDLE  ( 16 ) ///< 通常の振動 時間
#define EFD_QUAKE_TIME_BIG     ( 22 ) ///< 長い振動 時間

//----- External Declarations ------------------------------------------
// ================================================================
// EfQuake
/*!
  振動エフェクト
    
  @param ucQuakeKind [in] 地震の種類 EFD_QUAKE_S_MINI～

  @return 振動ワークポインタ
    
  @note
    EfQuakeGetX,efQuakeGetYで振動結果を取得する\n
    引数がEFD_QUAKE_CHECKの時、返り値がNULLならば終了、それ以外であれば振動実行中となる。
    
 */
// ================================================================
EFS_QUAKE_WORK* EfQuake( u8 ucQuakeKind );

// ================================================================
// EfQuakeCycle
/*!
  振動エフェクト
    
  @param spd      [in] 振動初速度   1:19:12 （ 0に設定すると速度値を上書きしない
  @param spd_cycle [in] 振動周期速度（角度値） 1:31  ( 0に設定すると一時停止
  @param spd_down  [in] 振動減衰速度 1:19:12  ( 0以下で延々と振動する

  @note EfQuakeGetX,efQuakeGetYで振動結果を取得する\n
        減衰速度が 0、もしくはマイナスの値の場合、ゆれが収まらないので\n
        任意でefQuake(EFD_QUAKE_END)か別の振動を呼び出して終了させる事\n
       （タスクオールクリアでも消える）

 */
// ================================================================
EFS_QUAKE_WORK* EfQuakeCycle( s32 spd, s32 spd_cycle, s32 spd_down );

// ================================================================
// EfQuakeGetX
/*!
  振動エフェクト 振動値取得
    
  @return   振動値 1:19:12
 */
// ================================================================
s32 EfQuakeGetX();
// ================================================================
// EfQuakeGetY
/*!
  振動エフェクト 振動値取得
    
  @return   振動値 1:19:12
 */
// ================================================================
s32 EfQuakeGetY();


#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _H_EFQUAKE
