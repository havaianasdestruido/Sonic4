// ================================================================
/*!
  @file efQuake.c
  @brief 振動

  @author mana
                Copyright(c) 2004 Dimps

  $Id: efQuake.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 *
 */

//----- Include Files --------------------------------------------------
#include "pch.h"
#ifndef _DS
#include "mt.h"
#include "mi.h"
#endif

#include "efQuake.h"

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------

/// 振動テーブル 小
const s8 _ef_quake_small_tbl[EFD_QUAKE_TIME_SMALL] =
{
    (0) | ( 2 << 4),
    (0) | ( 2 << 4),
    (0) | (-2 << 4),
    (0) | ( 2 << 4),

    (0) | ( 2 << 4),
    (0) | (-2 << 4),
    (0) | ( 2 << 4),
    (0) | ( 2 << 4),

    (0) | (-2 << 4),
    (0) | ( 2 << 4),
    (0) | ( 2 << 4),
    (0) | (-2 << 4),
};
/// 振動テーブル 中
const s8 _ef_quake_middle_tbl[4] =
{
    (0) | ( 2 << 4),
    (0) | ( 2 << 4),
    (0) | (-2 << 4),
    (0) | (-2 << 4),
};
/// 振動テーブル 大
const s8 _ef_quake_big_tbl[EFD_QUAKE_TIME_BIG] =
{
    (-2 & 0x0f) | ( 2 << 4),
    ( 2 & 0x0f) | (-4 << 4),
    ( 2 & 0x0f) | (-4 << 4),
    (-4 & 0x0f) | ( 4 << 4),
    ( 4 & 0x0f) | (-2 << 4),
    ( 4 & 0x0f) | (-2 << 4),
    (-2 & 0x0f) | ( 0 << 4),
    ( 4 & 0x0f) | ( 2 << 4),
    (-4 & 0x0f) | ( 4 << 4),
    ( 2 & 0x0f) | ( 4 << 4),
    ( 2 & 0x0f) | ( 4 << 4),
    ( 2 & 0x0f) | (-4 << 4),
    (-4 & 0x0f) | ( 4 << 4),
    (-4 & 0x0f) | ( 4 << 4),
    ( 4 & 0x0f) | (-2 << 4),
    (-2 & 0x0f) | ( 0 << 4),
    (-2 & 0x0f) | ( 0 << 4),
    ( 4 & 0x0f) | ( 2 << 4),
    (-4 & 0x0f) | (-4 << 4),
    (-4 & 0x0f) | (-4 << 4),
    ( 2 & 0x0f) | ( 2 << 4),
    
};

//----- External Declarations ------------------------------------------

//----- Static Declarations --------------------------------------------
static void efQuakeInit();
#if defined _DS
static void efQuakeMain();
#else
static void efQuakeMain(MTS_TASK_TCB *tcb);
#endif
static void efQuakeEnd();
static void efQuakeExit(MTS_TASK_TCB * pTcb);

//----- Global Variables -----------------------------------------------

//----- Local Variables ------------------------------------------------
static MTS_TASK_TCB* _ef_quake_ptcb = NULL; // 地震振動エフェクトタスクポインタ

//----- Global Functions -----------------------------------------------
// ================================================================
// EfQuake
/*!
  振動エフェクト 発生
    
  @param ucQuakeKind [in] 地震の種類 EFD_QUAKE_S_MINI～

  @return 振動ワークポインタ
    
  @note
    EfQuakeGetX,efQuakeGetYで振動結果を取得する\n
    引数がEFD_QUAKE_CHECKの時、返り値がNULLならば終了、それ以外であれば振動実行中となる。
 */
// ================================================================
EFS_QUAKE_WORK* EfQuake( u8 ucQuakeKind )
{
    EFS_QUAKE_WORK * pWork;

    // 振動終了チェック
    if ( ucQuakeKind == EFD_QUAKE_CHECK ){
        if ( _ef_quake_ptcb )
            return (EFS_QUAKE_WORK*)mtTaskGetTcbWork(_ef_quake_ptcb);
        return NULL;
    }
    
    // 未初期化
    if ( _ef_quake_ptcb == NULL && ucQuakeKind != EFD_QUAKE_END){
        // 初期化
        efQuakeInit();
        
        // タスク生成できず
        if ( _ef_quake_ptcb == NULL )
            return NULL;
    }

	if (_ef_quake_ptcb) {
		pWork = (EFS_QUAKE_WORK*)mtTaskGetTcbWork(_ef_quake_ptcb);
	}
    
    if ( ucQuakeKind == EFD_QUAKE_END ){
        efQuakeEnd();
        return NULL;
    }

    if ( pWork->size > ucQuakeKind )
        return pWork;
    
    pWork->size = ucQuakeKind;
    pWork->flag = EFD_QUAKE_FLAG_RUN;
    pWork->timer = 0;

    return pWork;
}
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
EFS_QUAKE_WORK* EfQuakeCycle( s32 spd, s32 spd_cycle, s32 spd_down )
{
    EFS_QUAKE_WORK * pWork;

    // 未初期化
    if ( _ef_quake_ptcb == NULL ){
        // 初期化
        efQuakeInit();
        
        // タスク生成できず
        if ( _ef_quake_ptcb == NULL )
            return NULL;
    }

    pWork = (EFS_QUAKE_WORK*)mtTaskGetTcbWork(_ef_quake_ptcb);
    
    pWork->size = EFD_QUAKE;
    pWork->flag = EFD_QUAKE_FLAG_RUN;
    if ( spd )
        pWork->spd = spd;
    pWork->spd_cycle = spd_cycle;
    pWork->spd_down = spd_down;

    return pWork;
}


// ================================================================
// EfQuakeGetX
/*!
  振動エフェクト 振動値取得
    
  @return   振動値X  1:19:12
 */
// ================================================================
s32 EfQuakeGetX()
{
    EFS_QUAKE_WORK * pWork;

    // 未初期化
    if ( _ef_quake_ptcb == NULL )
        return 0;

    pWork = (EFS_QUAKE_WORK*)mtTaskGetTcbWork(_ef_quake_ptcb);
    
    return pWork->vib_x;
}

// ================================================================
// EfQuakeGetY
/*!
  振動エフェクト 振動値取得
    
  @return   振動値Y  1:19:12
 */
// ================================================================
s32 EfQuakeGetY()
{
    EFS_QUAKE_WORK * pWork;

    // 未初期化
    if ( _ef_quake_ptcb == NULL )
        return 0;

    pWork = (EFS_QUAKE_WORK*)mtTaskGetTcbWork(_ef_quake_ptcb);
    
    return pWork->vib_y;
    
}
//----- Local Functions ------------------------------------------------
// ================================================================
// DBLG_QuakeInit
/*!
  振動エフェクト 初期化
 */
// ================================================================
static void efQuakeInit()
{
    EFS_QUAKE_WORK* pWork;
    MTS_TASK_TCB * pTcb;
    
    pTcb = MTM_TASK_MAKE_TCB( efQuakeMain, efQuakeExit, 0, 0, EFD_TASK_PRIO,
                              EFD_TASK_GROUP, sizeof(EFS_QUAKE_WORK), "EF_QUAKE" );
    
    if ( pTcb == MTD_TASK_ERROR_ADDR )
        return ;
    
    _ef_quake_ptcb = pTcb;
    
    pWork = (EFS_QUAKE_WORK*)mtTaskGetTcbWork( pTcb );

    MI_CpuClear16( pWork, sizeof(EFS_QUAKE_WORK) );

}

// ================================================================
// efQuakeEnd
/*!
  振動エフェクト 終了
 */
// ================================================================
static void efQuakeEnd()
{
    EFS_QUAKE_WORK * pWork;

    // 未初期化
    if ( _ef_quake_ptcb == NULL )
        return;

    pWork = (EFS_QUAKE_WORK*)mtTaskGetTcbWork(_ef_quake_ptcb);

    // クリア
    MI_CpuClear16( pWork, sizeof(EFS_QUAKE_WORK) );
    
}
// ================================================================
// dblg_QuakeMain
/*!
  振動エフェクト
 */
// ================================================================
#if defined _DS
static void efQuakeMain()
{
    EFS_QUAKE_WORK * pWork;
    pWork = (EFS_QUAKE_WORK*)mtTaskGetOwnTcbWork();
#else
static void efQuakeMain(MTS_TASK_TCB *tcb)
{
    EFS_QUAKE_WORK * pWork;
    pWork = (EFS_QUAKE_WORK*)mtTaskGetTcbWork(tcb);
#endif

    // タスク終了チェック
    if ( !(pWork->flag & EFD_QUAKE_FLAG_RUN) ){
#if defined _DS
        mtTaskClearOwnTcb();
#else
        mtTaskClearTcb(tcb);
#endif
        return;
    }
    
    if ( (pWork->flag & EFD_QUAKE_FLAG_STOP) ){
        return;
    }
    // 振動初期化
    pWork->vib_x = 0;
    pWork->vib_y = 0;

    // 地震実行
    if ( pWork->flag & EFD_QUAKE_FLAG_RUN ){

        // 種類に合わせて振動
        switch ( pWork->size){
        case EFD_QUAKE:
            // 減衰振動
            pWork->vib_y = ((pWork->spd * mtMathCos( (u16)pWork->angle )) >> FX32_SHIFT);
            pWork->spd -= pWork->spd_down;
            if ( pWork->spd < 0 )
                pWork->spd = 0;
            pWork->angle += pWork->spd_cycle;

            // 終了
            if ( !pWork->spd )
                efQuakeEnd();
            break;
        case EFD_QUAKE_S_MINI:
            
            if ( pWork->timer > EFD_QUAKE_TIME_SHORT  ){
                efQuakeEnd();
                break;
            }
            // no- break;
        case EFD_QUAKE_M_MINI:
            if ( pWork->timer >= EFD_QUAKE_TIME_SMALL ){
                efQuakeEnd();
                break;
            }
            // no- break;
        case EFD_QUAKE_L_MINI:
            if ( pWork->timer >= EFD_QUAKE_TIME_SMALL )
                pWork->timer = 0;
            // 設定
            pWork->vib_x = ((s8)((_ef_quake_small_tbl[pWork->timer] & 0x0f) >> 1)) << FX32_SHIFT;
            pWork->vib_y = ((s8)((_ef_quake_small_tbl[pWork->timer] >> 4  ) >> 1)) << FX32_SHIFT;
            break;
            
            // 小 地震
        case EFD_QUAKE_S_SMALL:
            if ( pWork->timer > EFD_QUAKE_TIME_SHORT  ){
                efQuakeEnd();
                break;
            }
                
            // no- break;
        case EFD_QUAKE_M_SMALL:
            if ( pWork->timer >= EFD_QUAKE_TIME_SMALL ){
                efQuakeEnd();
                break;
            }
            // no- break;
        case EFD_QUAKE_L_SMALL:
            if ( pWork->timer >= EFD_QUAKE_TIME_SMALL )
                pWork->timer = 0;

            // 設定
#if 1
			pWork->vib_x = ((s8)(_ef_quake_small_tbl[pWork->timer] & 0x0f)) << FX32_SHIFT;
			pWork->vib_y = ((s8)(_ef_quake_small_tbl[pWork->timer] >> 4  )) << FX32_SHIFT;
			if ( pWork->vib_x & 0x08*FX32_ONE ) {
				pWork->vib_x = -(16*FX32_ONE - pWork->vib_x);
			}
#else
            pWork->vib_x = ((s8)(_ef_quake_small_tbl[pWork->timer] & 0x0f)) << FX32_SHIFT;
            pWork->vib_y = ((s8)(_ef_quake_small_tbl[pWork->timer] >> 4  )) << FX32_SHIFT;
            if ( pWork->vib_x & 0x08 )
                pWork->vib_x = (s8)-(16 - pWork->vib_x);
#endif
            break;

            
            // 中 地震
        case EFD_QUAKE_S_MIDDLE:
            if ( pWork->timer >= EFD_QUAKE_TIME_SHORT){
                efQuakeEnd();
                break;
            }
            // no- break;
        case EFD_QUAKE_M_MIDDLE:
            if ( pWork->timer >= EFD_QUAKE_TIME_MIDDLE ){
                efQuakeEnd();
                break;
            }
            // no- break;
        case EFD_QUAKE_L_MIDDLE:
            if ( pWork->timer >= EFD_QUAKE_TIME_MIDDLE )
                pWork->timer = 0;
            
            // 設定
#if 1
			pWork->vib_x = ((s8)(_ef_quake_middle_tbl[pWork->timer & 0x3] & 0x0f)) << FX32_SHIFT;
			pWork->vib_y = ((s8)(_ef_quake_middle_tbl[pWork->timer & 0x3] >> 4  )) << FX32_SHIFT;
			if ( pWork->vib_x & 0x08*FX32_ONE ) {
				pWork->vib_x = -(16*FX32_ONE - pWork->vib_x);
			}
#else
            pWork->vib_x = ((s8)(_ef_quake_middle_tbl[pWork->timer & 0x3] & 0x0f)) << FX32_SHIFT;
            pWork->vib_y = ((s8)(_ef_quake_middle_tbl[pWork->timer & 0x3] >> 4  )) << FX32_SHIFT;
            if ( pWork->vib_x & 0x08 )
                pWork->vib_x = (s8)-(16 - pWork->vib_x);
#endif
            break;

            
        // 大 地震
        case EFD_QUAKE_S_BIG:
            if ( pWork->timer >= EFD_QUAKE_TIME_SHORT ){
                efQuakeEnd();
                break;
            }
            // no- break;
        case EFD_QUAKE_M_BIG:
            if ( pWork->timer >= EFD_QUAKE_TIME_BIG ){
                efQuakeEnd();
                break;
            }
            // no- break;
        case EFD_QUAKE_L_BIG:
            if ( pWork->timer >= EFD_QUAKE_TIME_BIG )
                pWork->timer = 0;
            
            // 設定
#if 1
			pWork->vib_x = ((s8)(_ef_quake_big_tbl[pWork->timer] & 0x0f)) << FX32_SHIFT;
			pWork->vib_y = ((s8)(_ef_quake_big_tbl[pWork->timer] >> 4  )) << FX32_SHIFT;
			if ( pWork->vib_x & 0x08*FX32_ONE ) {
				pWork->vib_x = -(16*FX32_ONE - pWork->vib_x);
			}
#else
            pWork->vib_x = ((s8)(_ef_quake_big_tbl[pWork->timer] & 0x0f)) << FX32_SHIFT;
            pWork->vib_y = ((s8)(_ef_quake_big_tbl[pWork->timer] >> 4  )) << FX32_SHIFT;
            if ( pWork->vib_x & 0x08 )
                pWork->vib_x = (s8)-(16 - pWork->vib_x);
#endif
            break;
        
        // 巨大地震
        case EFD_QUAKE_S_HUGE:
            if ( pWork->timer >= EFD_QUAKE_TIME_SHORT ){
                efQuakeEnd();
                break;
            }
            // no- break;
        case EFD_QUAKE_M_HUGE:
            if ( pWork->timer >= EFD_QUAKE_TIME_BIG ){
                efQuakeEnd();
                break;
            }
            // no- break;
        case EFD_QUAKE_L_HUGE:
            if ( pWork->timer >= EFD_QUAKE_TIME_BIG )
                pWork->timer = 0;
            
            // 設定
#if 1
			pWork->vib_x = ((s8)(_ef_quake_big_tbl[pWork->timer] & 0x0f)) << FX32_SHIFT;
			pWork->vib_y = ((s8)(_ef_quake_big_tbl[pWork->timer] >> 4  )) << FX32_SHIFT;
			if ( pWork->vib_x & 0x08*FX32_ONE ) {
				pWork->vib_x = -(16*FX32_ONE - pWork->vib_x);
			}
#else
            pWork->vib_x = ((s8)(_ef_quake_big_tbl[pWork->timer] & 0x0f)) << FX32_SHIFT;
            pWork->vib_y = ((s8)(_ef_quake_big_tbl[pWork->timer] >> 4  )) << FX32_SHIFT;
            if ( pWork->vib_x & 0x08 )
                pWork->vib_x = (s8)-(16 - pWork->vib_x);
#endif
            pWork->vib_x <<= 1;
            pWork->vib_y <<= 1;
            break;
            
        }

        // タイマー
        ++pWork->timer;
    }
}

// ================================================================
// efQuakeExit
/*!
  振動エフェクト タスク終了

  @param pTcb [in] タスクポインタ
 */
// ================================================================
static void efQuakeExit(MTS_TASK_TCB * pTcb)
{
#ifndef _DS
	UNREFERENCED_PARAMETER(pTcb);
#endif
    _ef_quake_ptcb = NULL;
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

