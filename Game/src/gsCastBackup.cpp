// ============================================================================
/*!
	@file	gsCastBackup.cpp
	@brief	キャスト・バックアップ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsCastBackup.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "gsCastBackup.hpp"



//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace gs {











// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		gs::backup::EStage::Type ← GSE_MAIN_STAGE_ID
 */
// ============================================================================
template <>
gs::backup::EStage::Type public_cast(const GSE_MAIN_STAGE_ID &from)
{
	typedef gs::backup::EStage EStage;
	switch (from) {
	case GSD_MAIN_STAGE_ID_1_1:		return EStage::Zone1Act1;	break;
	case GSD_MAIN_STAGE_ID_1_2:		return EStage::Zone1Act2;	break;
	case GSD_MAIN_STAGE_ID_1_3:		return EStage::Zone1Act3;	break;
	case GSD_MAIN_STAGE_ID_1_BOSS:	return EStage::Zone1Boss;	break;
	case GSD_MAIN_STAGE_ID_2_1:		return EStage::Zone2Act1;	break;
	case GSD_MAIN_STAGE_ID_2_2:		return EStage::Zone2Act2;	break;
	case GSD_MAIN_STAGE_ID_2_3:		return EStage::Zone2Act3;	break;
	case GSD_MAIN_STAGE_ID_2_BOSS:	return EStage::Zone2Boss;	break;
	case GSD_MAIN_STAGE_ID_3_1:		return EStage::Zone3Act1;	break;
	case GSD_MAIN_STAGE_ID_3_2:		return EStage::Zone3Act2;	break;
	case GSD_MAIN_STAGE_ID_3_3:		return EStage::Zone3Act3;	break;
	case GSD_MAIN_STAGE_ID_3_BOSS:	return EStage::Zone3Boss;	break;
	case GSD_MAIN_STAGE_ID_4_1:		return EStage::Zone4Act1;	break;
	case GSD_MAIN_STAGE_ID_4_2:		return EStage::Zone4Act2;	break;
	case GSD_MAIN_STAGE_ID_4_3:		return EStage::Zone4Act3;	break;
	case GSD_MAIN_STAGE_ID_4_BOSS:	return EStage::Zone4Boss;	break;
	case GSD_MAIN_STAGE_ID_FINAL_1:	return EStage::Final;		break;
	case GSD_MAIN_STAGE_ID_FINAL_2:	return EStage::Final;		break;
	case GSD_MAIN_STAGE_ID_FINAL_3:	return EStage::Final;		break;
	case GSD_MAIN_STAGE_ID_FINAL_4:	return EStage::Final;		break;
	case GSD_MAIN_STAGE_ID_FINAL_5:	return EStage::Final;		break;
	default:						return EStage::None;		break;
	}
}

// ============================================================================
// gs::public_cast
/*!
	キャスト

	@note
		gs::backup::ESpecialStage::Type ← GSE_MAIN_STAGE_ID
 */
// ============================================================================
template <>
gs::backup::ESpecialStage::Type public_cast(const GSE_MAIN_STAGE_ID &from)
{
	typedef gs::backup::ESpecialStage ESpecialStage;
	switch (from) {
	case GSD_MAIN_STAGE_ID_SS1:	return ESpecialStage::Stage1;	break;
	case GSD_MAIN_STAGE_ID_SS2:	return ESpecialStage::Stage2;	break;
	case GSD_MAIN_STAGE_ID_SS3:	return ESpecialStage::Stage3;	break;
	case GSD_MAIN_STAGE_ID_SS4:	return ESpecialStage::Stage4;	break;
	case GSD_MAIN_STAGE_ID_SS5:	return ESpecialStage::Stage5;	break;
	case GSD_MAIN_STAGE_ID_SS6:	return ESpecialStage::Stage6;	break;
	case GSD_MAIN_STAGE_ID_SS7:	return ESpecialStage::Stage7;	break;
	default:					return ESpecialStage::None;		break;
	}
}

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
gs::backup::SSpecialSolo::EEmeraldStage::Type public_cast(const GSE_MAIN_STAGE_ID &from)
{
	typedef gs::backup::SSpecialSolo::EEmeraldStage EEmeraldStage;
	switch (from) {
	case GSD_MAIN_STAGE_ID_1_1:	return EEmeraldStage::Zone1Act1;	break;
	case GSD_MAIN_STAGE_ID_1_2:	return EEmeraldStage::Zone1Act2;	break;
	case GSD_MAIN_STAGE_ID_1_3:	return EEmeraldStage::Zone1Act3;	break;
	case GSD_MAIN_STAGE_ID_2_1:	return EEmeraldStage::Zone2Act1;	break;
	case GSD_MAIN_STAGE_ID_2_2:	return EEmeraldStage::Zone2Act2;	break;
	case GSD_MAIN_STAGE_ID_2_3:	return EEmeraldStage::Zone2Act3;	break;
	case GSD_MAIN_STAGE_ID_3_1:	return EEmeraldStage::Zone3Act1;	break;
	case GSD_MAIN_STAGE_ID_3_2:	return EEmeraldStage::Zone3Act2;	break;
	case GSD_MAIN_STAGE_ID_3_3:	return EEmeraldStage::Zone3Act3;	break;
	case GSD_MAIN_STAGE_ID_4_1:	return EEmeraldStage::Zone4Act1;	break;
	case GSD_MAIN_STAGE_ID_4_2:	return EEmeraldStage::Zone4Act2;	break;
	case GSD_MAIN_STAGE_ID_4_3:	return EEmeraldStage::Zone4Act3;	break;
	default:					return EEmeraldStage::None;			break;
	}
}

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
const char *public_cast(const GSE_MAIN_STAGE_ID &from)
{
	switch (from) {
	case GSD_MAIN_STAGE_ID_1_1:		return "1-1";	break;
	case GSD_MAIN_STAGE_ID_1_2:		return "1-2";	break;
	case GSD_MAIN_STAGE_ID_1_3:		return "1-3";	break;
	case GSD_MAIN_STAGE_ID_1_BOSS:	return "1-B";	break;
	case GSD_MAIN_STAGE_ID_2_1:		return "2-1";	break;
	case GSD_MAIN_STAGE_ID_2_2:		return "2-2";	break;
	case GSD_MAIN_STAGE_ID_2_3:		return "2-3";	break;
	case GSD_MAIN_STAGE_ID_2_BOSS:	return "2-B";	break;
	case GSD_MAIN_STAGE_ID_3_1:		return "3-1";	break;
	case GSD_MAIN_STAGE_ID_3_2:		return "3-2";	break;
	case GSD_MAIN_STAGE_ID_3_3:		return "3-3";	break;
	case GSD_MAIN_STAGE_ID_3_BOSS:	return "3-B";	break;
	case GSD_MAIN_STAGE_ID_4_1:		return "4-1";	break;
	case GSD_MAIN_STAGE_ID_4_2:		return "4-2";	break;
	case GSD_MAIN_STAGE_ID_4_3:		return "4-3";	break;
	case GSD_MAIN_STAGE_ID_4_BOSS:	return "4-B";	break;
	case GSD_MAIN_STAGE_ID_FINAL_1:	return "F";		break;
	case GSD_MAIN_STAGE_ID_FINAL_2:	return "F-2";	break;
	case GSD_MAIN_STAGE_ID_FINAL_3:	return "F-3";	break;
	case GSD_MAIN_STAGE_ID_FINAL_4:	return "F-4";	break;
	case GSD_MAIN_STAGE_ID_FINAL_5:	return "F-5";	break;
	case GSD_MAIN_STAGE_ID_SS1:		return "Sp1";	break;
	case GSD_MAIN_STAGE_ID_SS2:		return "Sp2";	break;
	case GSD_MAIN_STAGE_ID_SS3:		return "Sp3";	break;
	case GSD_MAIN_STAGE_ID_SS4:		return "Sp4";	break;
	case GSD_MAIN_STAGE_ID_SS5:		return "Sp5";	break;
	case GSD_MAIN_STAGE_ID_SS6:		return "Sp6";	break;
	case GSD_MAIN_STAGE_ID_SS7:		return "Sp7";	break;
	default:						return "";		break;
	}
}
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
GSE_MAIN_STAGE_ID public_cast(const gs::backup::EStage::Type &from)
{
	typedef gs::backup::EStage EStage;
	switch (from) {
	case EStage::Zone1Act1:	return GSD_MAIN_STAGE_ID_1_1;		break;
	case EStage::Zone1Act2:	return GSD_MAIN_STAGE_ID_1_2;		break;
	case EStage::Zone1Act3:	return GSD_MAIN_STAGE_ID_1_3;		break;
	case EStage::Zone1Boss:	return GSD_MAIN_STAGE_ID_1_BOSS;	break;
	case EStage::Zone2Act1:	return GSD_MAIN_STAGE_ID_2_1;		break;
	case EStage::Zone2Act2:	return GSD_MAIN_STAGE_ID_2_2;		break;
	case EStage::Zone2Act3:	return GSD_MAIN_STAGE_ID_2_3;		break;
	case EStage::Zone2Boss:	return GSD_MAIN_STAGE_ID_2_BOSS;	break;
	case EStage::Zone3Act1:	return GSD_MAIN_STAGE_ID_3_1;		break;
	case EStage::Zone3Act2:	return GSD_MAIN_STAGE_ID_3_2;		break;
	case EStage::Zone3Act3:	return GSD_MAIN_STAGE_ID_3_3;		break;
	case EStage::Zone3Boss:	return GSD_MAIN_STAGE_ID_3_BOSS;	break;
	case EStage::Zone4Act1:	return GSD_MAIN_STAGE_ID_4_1;		break;
	case EStage::Zone4Act2:	return GSD_MAIN_STAGE_ID_4_2;		break;
	case EStage::Zone4Act3:	return GSD_MAIN_STAGE_ID_4_3;		break;
	case EStage::Zone4Boss:	return GSD_MAIN_STAGE_ID_4_BOSS;	break;
	case EStage::Final:		return GSD_MAIN_STAGE_ID_FINAL_1;	break;
	default:				return GSD_MAIN_STAGE_ID_MAX;		break;
	}
}

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
const char *public_cast(const gs::backup::EStage::Type &from)
{
	return public_cast<const char *>(public_cast<GSE_MAIN_STAGE_ID>(from));
}
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
GSE_MAIN_STAGE_ID public_cast(const gs::backup::ESpecialStage::Type &from)
{
	typedef gs::backup::ESpecialStage ESpecialStage;
	switch (from) {
	case ESpecialStage::Stage1:	return GSD_MAIN_STAGE_ID_SS1;	break;
	case ESpecialStage::Stage2:	return GSD_MAIN_STAGE_ID_SS2;	break;
	case ESpecialStage::Stage3:	return GSD_MAIN_STAGE_ID_SS3;	break;
	case ESpecialStage::Stage4:	return GSD_MAIN_STAGE_ID_SS4;	break;
	case ESpecialStage::Stage5:	return GSD_MAIN_STAGE_ID_SS5;	break;
	case ESpecialStage::Stage6:	return GSD_MAIN_STAGE_ID_SS6;	break;
	case ESpecialStage::Stage7:	return GSD_MAIN_STAGE_ID_SS7;	break;
	default:					return GSD_MAIN_STAGE_ID_MAX;	break;
	}
}

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
const char *public_cast(const gs::backup::ESpecialStage::Type &from)
{
	return public_cast<const char *>(public_cast<GSE_MAIN_STAGE_ID>(from));
}
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
GSE_MAIN_STAGE_ID public_cast(const gs::backup::SSpecialSolo::EEmeraldStage::Type &from)
{
	typedef gs::backup::SSpecialSolo::EEmeraldStage EEmeraldStage;
	switch (from) {
	case EEmeraldStage::Zone1Act1:	return GSD_MAIN_STAGE_ID_1_1;	break;
	case EEmeraldStage::Zone1Act2:	return GSD_MAIN_STAGE_ID_1_2;	break;
	case EEmeraldStage::Zone1Act3:	return GSD_MAIN_STAGE_ID_1_3;	break;
	case EEmeraldStage::Zone2Act1:	return GSD_MAIN_STAGE_ID_2_1;	break;
	case EEmeraldStage::Zone2Act2:	return GSD_MAIN_STAGE_ID_2_2;	break;
	case EEmeraldStage::Zone2Act3:	return GSD_MAIN_STAGE_ID_2_3;	break;
	case EEmeraldStage::Zone3Act1:	return GSD_MAIN_STAGE_ID_3_1;	break;
	case EEmeraldStage::Zone3Act2:	return GSD_MAIN_STAGE_ID_3_2;	break;
	case EEmeraldStage::Zone3Act3:	return GSD_MAIN_STAGE_ID_3_3;	break;
	case EEmeraldStage::Zone4Act1:	return GSD_MAIN_STAGE_ID_4_1;	break;
	case EEmeraldStage::Zone4Act2:	return GSD_MAIN_STAGE_ID_4_2;	break;
	case EEmeraldStage::Zone4Act3:	return GSD_MAIN_STAGE_ID_4_3;	break;
	default:						return GSD_MAIN_STAGE_ID_MAX;	break;
	}
}

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
const char *public_cast(const gs::backup::SSpecialSolo::EEmeraldStage::Type &from)
{
	return public_cast<const char *>(public_cast<GSE_MAIN_STAGE_ID>(from));
}
#endif //defined(MTD_DEBUG)




















































} //namespace gs

// =============================================================================
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
// ==========================================================================
