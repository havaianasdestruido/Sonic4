// ==========================================================================
/*!
  @file mtMemory.h
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: mtMemory.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef MT_MEMORY_H_
#define MT_MEMORY_H_



//----- Include Files -------------------------------------------------------
#include <alice.h>

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
// ================================================================
// mtMemAllocSys
/*!
  システムヒープからメモリを確保する
  
  @param    size        [in] 確保するメモリサイズ（バイト単位）。
 */
// ================================================================
#if _WII
#define mtMemAllocSys(size)	amMemAllocHeap(size, 0)
#else
#define mtMemAllocSys(size)	amMemAlloc(size)
#endif

// ================================================================
// mtMemAllocSysTail
/*!
  システムヒープの後方からメモリを確保する
  
  @param    size        [in] 確保するメモリサイズ（バイト単位）
 */
// ================================================================
#if _WII
#define mtMemAllocSysTail(size)	amMemAllocTempHeap(size, 0)
#else
#define mtMemAllocSysTail(size)	amMemAllocTemp(size)
#endif

// ================================================================
// mtMemFreeSys
/*!
  システムヒープから確保したメモリを解放する
  
  @param    addr    [in] 解放するメモリのアドレス
 */
// ================================================================
#define	mtMemFreeSys(addr)		amMemFree(addr)


// ================================================================
// mtMemAllocMain
/*!
  メインヒープからメモリを確保する
  
  @param    size        [in] 確保するメモリサイズ（バイト単位）
 */
// ================================================================
#if _WII
#define mtMemAllocMain(size)	amMemAllocHeap(size, 0)
#else
#define mtMemAllocMain(size)	amMemAlloc(size)
#endif

// ================================================================
// mtMemAllocMainTail
/*!
  メインヒープの後方からメモリを確保する
  
  @param    size        [in] 確保するメモリサイズ（バイト単位）
 */
// ================================================================
#if _WII
#define mtMemAllocMainTail(size)	amMemAllocTempHeap(size, 0)
#else
#define mtMemAllocMainTail(size)	amMemAllocTemp(size)
#endif

// ================================================================
// mtMemFreeMain
/*!
  メインヒープから確保したメモリを解放する
  
  @param    addr    [in] 解放するメモリのアドレス
 */
// ================================================================
#define	mtMemFreeMain(addr)		amMemFree(addr)

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------


#if	defined(__cplusplus)
} /* extern "C" */
#endif


#endif // MT_MEMORY_H_

//----- Include Files -------------------------------------------------------
