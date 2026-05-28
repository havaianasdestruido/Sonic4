/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006-2009 CRI Middleware Co.,Ltd.
 *
 * Library  : CRI File System
 * Module   : Library User's Header for iPhone
 * File     : cri_file_system_iPhone.h
 *
 ****************************************************************************/
/*!
 *	\file		cri_file_system_iPhone.h
 */

/* 多重定義防止					*/
/* Prevention of redefinition	*/
#ifndef	CRI_FILE_SYSTEM_IPHONE_H_INCLUDED
#define	CRI_FILE_SYSTEM_IPHONE_H_INCLUDED

/***************************************************************************
 *      インクルードファイル
 *      Include files
 ***************************************************************************/
#include "cri_xpt.h"
#include "cri_error.h"
#include "cri_file_system.h"

/***************************************************************************
 *      定数マクロ
 *      Macro Constants
 ***************************************************************************/

/***************************************************************************
 *      処理マクロ
 *      Macro Functions
 ***************************************************************************/

/***************************************************************************
 *      データ型宣言
 *      Data Type Declarations
 ***************************************************************************/

/***************************************************************************
 *      変数宣言
 *      Prototype Variables
 ***************************************************************************/

/***************************************************************************
 *      関数宣言
 *      Prototype Functions
 ***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*JP
 * \brief ファイルアクセススレッドのプライオリティ設定
 * \ingroup FSLIB_CRIFS_IPHONE
 * \param[in]	prio	スレッドのプライオリティ
 * \return	CriError	エラーコード
 * \par 説明:
 * ファイルアクセススレッドのプライオリティを設定します。<BR>
 * criFs_Initialize() の呼出し後に設定してください。<BR>
 * アプリケーションのメインスレッドよりも高いプライオリティを指定してください。<BR>
 * プライオリティのデフォルト値は THREAD_PRIORITY_HIGHEST です。<BR>
 */
CriError CRIAPI criFs_SetFileAccessThreadPriority_iPhone(int prio);

/*JP
 * \brief データ展開スレッドのプライオリティ設定
 * \ingroup FSLIB_CRIFS_PC
 * \param[in]	prio	スレッドのプライオリティ
 * \return	CriError	エラーコード
 * \par 説明:
 * データ展開スレッドのプライオリティを設定します。<BR>
 * criFs_Initialize() の呼出し後に設定してください。<BR>
 * アプリケーションのメインスレッドよりも低いプライオリティを指定してください。<BR>
 * プライオリティのデフォルト値は THREAD_PRIORITY_LOWEST です。<BR>
 */
CriError CRIAPI criFs_SetDataDecompressionThreadPriority_iPhone(int prio);




#ifdef __cplusplus
}
#endif

#endif	/* CRI_FILE_SYSTEM_IPHONE_H_INCLUDED */

/* --- end of file --- */
