// ==========================================================================
/*!
  @file mi.h
  @brief メモリ操作関連

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: mi.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef MI_H_
#define MI_H_



//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
// ==========================================================================
// MI_CpuClear8
// MI_CpuClear16
// MI_CpuClear32
/*!
 *	メモリクリア
 *
 *	@param	addr		[in]	クリアバッファアドレス
 *	@param	size		[in]	クリアサイズ
 */
// ==========================================================================
#define MI_CpuClear8(addr, size)	amZeroMemory(addr, size)
#define MI_CpuClear16(addr, size)	amZeroMemory(addr, size)
#define MI_CpuClear32(addr, size)	amZeroMemory(addr, size)

// ==========================================================================
// MI_CpuCopy8
// MI_CpuCopy16
// MI_CpuCopy32
/*!
 *	メモリコピー
 *
 *	@param	src			[in]	コピー元
 *	@param	dest		[in]	コピー先
 *	@param	size		[in]	コピーサイズ
 */
// ==========================================================================
#define MI_CpuCopy8(src, dest, size)	memcpy((void*)dest, (const void*)src, size)
#define MI_CpuCopy16(src, dest, size)	memcpy((void*)dest, (const void*)src, size)
#define MI_CpuCopy32(src, dest, size)	memcpy((void*)dest, (const void*)src, size)

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------


#if	defined(__cplusplus)
} /* extern "C" */
#endif


#endif // MI_H_

//----- Include Files -------------------------------------------------------
