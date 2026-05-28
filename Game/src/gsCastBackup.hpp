// ============================================================================
/*!
	@file	gsCastBackup.hpp
	@brief	キャスト・バックアップ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsCastBackup.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page gsCastBackupMain キャスト・バックアップ

	@section gsCastBackupSummary 概要
		キャスト・バックアップを提供します。
 */

#pragma once
#if	defined(__cplusplus)
extern "C" {
#endif

//------ C Include Files -------------- インクルード ---------------------------******CIF*
//------ C Macro ---------------------- マクロ ---------------------------------******CMC*
//------ C External Definitions ------- グローバル変数及び関数の宣言 -----------******CED*


#if	defined(__cplusplus)
} // extern "C"
#endif

#if	defined(__cplusplus)
//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "gsCast.hpp"
#include "gsMainSys.h"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace gs {











// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		gs::backup::EStage::Type ← GSE_MAIN_STAGE_ID
		失敗した場合は gs::backup::EStage::None を返します
 */
// ============================================================================
template <>
gs::backup::EStage::Type public_cast(const GSE_MAIN_STAGE_ID &from);

// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		gs::backup::ESpecialStage::Type ← GSE_MAIN_STAGE_ID
		失敗した場合は gs::backup::ESpecialStage::None を返します
 */
// ============================================================================
template <>
gs::backup::ESpecialStage::Type public_cast(const GSE_MAIN_STAGE_ID &from);

// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		gs::backup::SSpecialSolo::EEmeraldStage::Type ← GSE_MAIN_STAGE_ID
		失敗した場合は gs::backup::SSpecialSolo::EEmeraldStage::None を返します
 */
// ============================================================================
template <>
gs::backup::SSpecialSolo::EEmeraldStage::Type public_cast(const GSE_MAIN_STAGE_ID &from);

#if defined(MTD_DEBUG)
// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		const char * ← GSE_MAIN_STAGE_ID
		失敗した場合は "" を返します
 */
// ============================================================================
template <>
const char *public_cast(const GSE_MAIN_STAGE_ID &from);
#endif //defined(MTD_DEBUG)






// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		GSE_MAIN_STAGE_ID ← gs::backup::EStage::Type
		失敗した場合は GSD_MAIN_STAGE_ID_MAX を返します
 */
// ============================================================================
template <>
GSE_MAIN_STAGE_ID public_cast(const gs::backup::EStage::Type &from);

#if defined(MTD_DEBUG)
// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		const char * ← gs::backup::EStage::Type
		失敗した場合は "" を返します
 */
// ============================================================================
template <>
const char *public_cast(const gs::backup::EStage::Type &from);
#endif //defined(MTD_DEBUG)






// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		GSE_MAIN_STAGE_ID ← gs::backup::ESpecialStage::Type
		失敗した場合は GSD_MAIN_STAGE_ID_MAX を返します
 */
// ============================================================================
template <>
GSE_MAIN_STAGE_ID public_cast(const gs::backup::ESpecialStage::Type &from);

#if defined(MTD_DEBUG)
// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		const char * ← gs::backup::ESpecialStage::Type
		失敗した場合は "" を返します
 */
// ============================================================================
template <>
const char *public_cast(const gs::backup::ESpecialStage::Type &from);
#endif //defined(MTD_DEBUG)






// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		GSE_MAIN_STAGE_ID ← gs::backup::SSpecialSolo::EEmeraldStage::Type
		失敗した場合は GSD_MAIN_STAGE_ID_MAX を返します
 */
// ============================================================================
template <>
GSE_MAIN_STAGE_ID public_cast(const gs::backup::SSpecialSolo::EEmeraldStage::Type &from);

#if defined(MTD_DEBUG)
// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		const char * ← gs::backup::SSpecialSolo::EEmeraldStage::Type
		失敗した場合は "" を返します
 */
// ============================================================================
template <>
const char *public_cast(const gs::backup::SSpecialSolo::EEmeraldStage::Type &from);
#endif //defined(MTD_DEBUG)

















} //namespace gs


























#endif //#if	defined(__cplusplus)

	// ============================================================================
	// gs::Cast::Backup
	/*!
		関数の説明
	 
		@param	org1	[io]	引数１の説明
		@param	org2	[in]	引数２の説明
		@param	org3	[out]	引数３の説明
	 
		@return	戻り値の説明
			or
		@retval	0	正常
		@retval	!0	異常
	 
		@exception 例外

		@note
			補足説明
	 */
	// ============================================================================
