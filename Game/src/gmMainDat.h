// =======================================================================
/*!
  @file	gmMainDat.h
  @brief ÉQÅ[ÉÄÉÅÉCÉì ÉfÅ[É^íËã`

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmMainDat.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// =======================================================================
/*
 * $Log$
 */

/* èdï°ÉCÉìÉNÉãÅ[ÉhâÒîéËñ@ */

#ifndef GM_MAIN_DAT_H_
#define GM_MAIN_DAT_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/
#include "../file/common/arc/EFF_CMN.HMB"
#include "../file/common/arc/EFF_ZONE1.HMB"
#include "../file/common/arc/EFF_ZONE2.HMB"
#include "../file/common/arc/EFF_ZONE3.HMB"
#include "../file/common/arc/EFF_ZONE4.HMB"
#include "../file/common/arc/EFF_ZONE5.HMB"
#include "../file/common/arc/EFF_ZONESS.HMB"
#include "../file/common/arc/EFF_BS_CMN.HMB"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
// ==========================================================================
// ÉfÅ[É^ÉèÅ[ÉN(OBS_DATA_WORK)ê›íË
// ==========================================================================
/// égópÉfÅ[É^ÉèÅ[ÉNNO(ObjÉVÉXÉeÉÄämï€ï™)
enum {
	GMD_DWORK_NO_DUMMY		= 0,

	// FIX
	GMD_DWORK_NO_FIX_START,
	//GMD_DWORK_NO_FIX_ARC = GMD_DWORK_NO_FIX_START,
	GMD_DWORK_NO_FIX_END,

	// ÉäÉìÉO
	GMD_DWORK_NO_RING_START = GMD_DWORK_NO_FIX_END,

	GMD_DWORK_NO_RING_MODEL = GMD_DWORK_NO_RING_START,
	GMD_DWORK_NO_RING_TEX,
#if _IPHONE
	GMD_DWORK_NO_RING_MAT, // iPhone only
#endif

	GMD_DWORK_NO_RING_END,
	
	// ---------------- ÉGÉtÉFÉNÉg --------------------------
	GMD_DWORK_NO_EFFECT_START = GMD_DWORK_NO_RING_END,
	// ÉGÉtÉFÉNÉgÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_ARC_START	= GMD_DWORK_NO_EFFECT_START,
	GMD_DWORK_NO_EFFECT_CMN_ARC	= GMD_DWORK_NO_EFFECT_ARC_START,	// ã§í EFÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_ZONE_ARC,		// 
	GMD_DWORK_NO_EFFECT_ENE_E02_ARC,	// ÉGÉlÉ~Å[êÍópEF ÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_ENE_E04_ARC,	// ÉGÉlÉ~Å[êÍópEF ÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_ENE_E05_ARC,	// ÉGÉlÉ~Å[êÍópEF ÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_ENE_E06_ARC,	// ÉGÉlÉ~Å[êÍópEF ÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_ENE_E07_ARC,	// ÉGÉlÉ~Å[êÍópEF ÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_ENE_E10_ARC,	// ÉGÉlÉ~Å[êÍópEF ÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_ENE_E13_ARC,	// ÉGÉlÉ~Å[êÍópEF ÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_ENE_E14_ARC,	// ÉGÉlÉ~Å[êÍópEF ÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_BOSS_CMN_ARC,	// É{ÉXã§í EFÉAÅ[ÉJÉCÉu
	GMD_DWORK_NO_EFFECT_ARC_END,

	/*
	  à»â∫ÅAã§í ÉGÉtÉFÉNÉgä÷òA
	  ã§í EFÉAÅ[ÉJÉCÉuAMBì‡ÇÕÅAÅuAME*nå¬ Å® TEXAMB Å® OBJECT*må¬Åv Ç∆Ç¢Ç§ï¿Ç—Ç…Ç»Ç¡ÇƒÇ¢ÇÈïKóvÇ™Ç†ÇËÇ‹Ç∑ÅB
	 */
	GMD_DWORK_NO_EFFECT_CMN_AMBTEX,			//!< ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ ÉeÉNÉXÉ`ÉÉÉAÅ[ÉJÉCÉuAMB
	GMD_DWORK_NO_EFFECT_CMN_TEXLIST,		//!< ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ ÉeÉNÉXÉ`ÉÉ (VRAMè„ÇÃÉeÉNÉXÉ`ÉÉÇ…ä÷òAïtÇØ)
	
	GMD_DWORK_NO_EFFECT_CMN_AME_START,		//!< ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ AME
	GMD_DWORK_NO_EFFECT_CMN_AME_00	= GMD_DWORK_NO_EFFECT_CMN_AME_START,
	GMD_DWORK_NO_EFFECT_CMN_AME_END	= GMD_DWORK_NO_EFFECT_CMN_AME_START + IDB_EFF_CMN_EFF_CMN_TEX_AMB,	//! ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ AME èIí[
	
	GMD_DWORK_NO_EFFECT_CMN_MODELDAT_START,	//!< ã§í ÉGÉtÉFÉNÉgÉÇÉfÉãÉfÅ[É^ .*noÉtÉ@ÉCÉã
	GMD_DWORK_NO_EFFECT_CMN_MODELDAT_00	= GMD_DWORK_NO_EFFECT_CMN_MODELDAT_START,
	// àÍâûÅAAMEÇÃêîï™ópà”ÇµÇƒÇ®Ç≠
	GMD_DWORK_NO_EFFECT_CMN_MODELDAT_END	= GMD_DWORK_NO_EFFECT_CMN_MODELDAT_START + IDB_EFF_CMN_EFF_CMN_TEX_AMB,
	
	GMD_DWORK_NO_EFFECT_CMN_OBJECT_START,	//!< ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ NNÉIÉuÉWÉFÉNÉg (VRAMè„ÇÃÉÇÉfÉãÇ…ä÷òAïtÇØ)
	GMD_DWORK_NO_EFFECT_CMN_OBJECT_00 = GMD_DWORK_NO_EFFECT_CMN_OBJECT_START,
	// àÍâûÅAAMEÇÃêîï™ópà”ÇµÇƒÇ®Ç≠
	GMD_DWORK_NO_EFFECT_CMN_OBJECT_END	= GMD_DWORK_NO_EFFECT_CMN_OBJECT_START + IDB_EFF_CMN_EFF_CMN_TEX_AMB,	//!< ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ NNÉIÉuÉWÉFÉNÉg èIí[
	
	GMD_DWORK_NO_EFFECT_CMN_MDL_AMBTEX_START,	//!< ã§í ÉGÉtÉFÉNÉg ÉÇÉfÉãópÉeÉNÉXÉ`ÉÉAMB
	GMD_DWORK_NO_EFFECT_CMN_MDL_AMBTEX_END	= GMD_DWORK_NO_EFFECT_CMN_MDL_AMBTEX_START + IDB_EFF_CMN_EFF_CMN_TEX_AMB,
	
	GMD_DWORK_NO_EFFECT_CMN_MDL_TEXLIST_START,	//!< ã§í ÉGÉtÉFÉNÉg ÉÇÉfÉãópÉeÉNÉXÉ`ÉÉÉäÉXÉg
	GMD_DWORK_NO_EFFECT_CMN_MDL_TEXLIST_END	= GMD_DWORK_NO_EFFECT_CMN_MDL_TEXLIST_START + IDB_EFF_CMN_EFF_CMN_TEX_AMB,
	
	/* É]Å[ÉìêÍópÉGÉtÉFÉNÉgä÷òA */
	GMD_DWORK_NO_EFFECT_ZONE_AMBTEX,		//!< É]Å[ÉìêÍópÉGÉtÉFÉNÉgÉfÅ[É^ ÉeÉNÉXÉ`ÉÉAMB
	GMD_DWORK_NO_EFFECT_ZONE_TEXLIST,		//!< É]Å[ÉìêÍópÉGÉtÉFÉNÉgÉfÅ[É^ ÉeÉNÉXÉ`ÉÉÅiVRAMè„ÇÃÉeÉNÉXÉ`ÉÉÇ…ä÷òAïtÇØÅj
	
	GMD_DWORK_NO_EFFECT_ZONE_AME_START,		//!< É]Å[ÉìêÍópÉGÉtÉFÉNÉgÉfÅ[É^ AME
	GMD_DWORK_NO_EFFECT_ZONE_AME_00	= GMD_DWORK_NO_EFFECT_ZONE_AME_START,
	GMD_DWORK_NO_EFFECT_ZONE_AME_END	=	// É]Å[ÉìêÍópÇÃëSAMEçáåvêîï™ópà”Åizone1,zone2,...Ç∆èáî‘Ç…égópÅj
		GMD_DWORK_NO_EFFECT_ZONE_AME_START
			+ IDB_EFF_ZONE1_EFF_ZONE1_TEX_AMB + IDB_EFF_ZONE2_EFF_ZONE2_TEX_AMB + IDB_EFF_ZONE3_EFF_ZONE3_TEX_AMB
				+ IDB_EFF_ZONE4_EFF_ZONE4_TEX_AMB + IDB_EFF_ZONE5_EFF_ZONE5_TEX_AMB + IDB_EFF_ZONESS_EFF_ZONESS_TEX_AMB,
	
	GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_START,
	//GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_XXX	= GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_START,
	GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_END,
	
	GMD_DWORK_NO_EFFECT_ZONE_OBJECT_START,
	// GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_***Ç∆ìØêî
	GMD_DWORK_NO_EFFECT_ZONE_OBJECT_END	= GMD_DWORK_NO_EFFECT_ZONE_OBJECT_START + (GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_END - GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_START),
	
	GMD_DWORK_NO_EFFECT_ZONE_MDL_AMBTEX_START,
	// GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_***Ç∆ìØêî
	GMD_DWORK_NO_EFFECT_ZONE_MDL_AMBTEX_END = GMD_DWORK_NO_EFFECT_ZONE_MDL_AMBTEX_START + (GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_END - GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_START),
	
	GMD_DWORK_NO_EFFECT_ZONE_MDL_TEXLIST_START,
	// GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_***Ç∆ìØêî
	GMD_DWORK_NO_EFFECT_ZONE_MDL_TEXLIST_END = GMD_DWORK_NO_EFFECT_ZONE_MDL_TEXLIST_START + (GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_END - GMD_DWORK_NO_EFFECT_ZONE_MODELDAT_START),
	
	
	/* ÉGÉlÉ~Å[êÍópÉGÉtÉFÉNÉgä÷òA */
	GMD_DWORK_NO_EFFECT_ENE_START,
	// ÉeÉNÉXÉ`ÉÉä÷òA
	GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E02	= GMD_DWORK_NO_EFFECT_ENE_START,
	GMD_DWORK_NO_EFFECT_ENE_TEXLIST_E02,
	GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E04,
	GMD_DWORK_NO_EFFECT_ENE_TEXLIST_E04,
	GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E05,
	GMD_DWORK_NO_EFFECT_ENE_TEXLIST_E05,
	GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E06,
	GMD_DWORK_NO_EFFECT_ENE_TEXLIST_E06,
	GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E07,
	GMD_DWORK_NO_EFFECT_ENE_TEXLIST_E07,
	GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E10,
	GMD_DWORK_NO_EFFECT_ENE_TEXLIST_E10,
	GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E13,
	GMD_DWORK_NO_EFFECT_ENE_TEXLIST_E13,
	GMD_DWORK_NO_EFFECT_ENE_AMBTEX_E14,
	GMD_DWORK_NO_EFFECT_ENE_TEXLIST_E14,
	GMD_DWORK_NO_EFFECT_ENE_TEX_END,
	
	// AME&ÉÇÉfÉãä÷òAÅiÉÇÉfÉãÇ∆ÉÇÉfÉãópAMBTEX,TEXLISTÇÃDWORK_NOÇÕÇªÇÍÇºÇÍÇÃAMEÇÃå„ÇÎÇ…ë}ì¸Åj
	GMD_DWORK_NO_EFFECT_ENE_AME_E02_JET_S,
	GMD_DWORK_NO_EFFECT_ENE_AME_E02_JET_S_SMORK,
	GMD_DWORK_NO_EFFECT_ENE_AME_E04_DUMMY1,
	GMD_DWORK_NO_EFFECT_ENE_AME_E04_DUMMY2,
	GMD_DWORK_NO_EFFECT_ENE_AME_E04_MEREON_MISS,
	GMD_DWORK_NO_EFFECT_ENE_AME_E05_GUARD,
	GMD_DWORK_NO_EFFECT_ENE_AME_E06_HARO,
	GMD_DWORK_NO_EFFECT_ENE_AME_E07_MOGU_E,
	GMD_DWORK_NO_EFFECT_ENE_AME_E07_MOGU_W,
	GMD_DWORK_NO_EFFECT_ENE_AME_E10_BUKUBUKU,
	GMD_DWORK_NO_EFFECT_ENE_AME_E13_CROW,
	GMD_DWORK_NO_EFFECT_ENE_AME_E13_T_STAR,
	GMD_DWORK_NO_EFFECT_ENE_AME_E14_JET_H,
	
	GMD_DWORK_NO_EFFECT_ENE_END,
	
	/*
	  à»â∫ÅAÉ{ÉXã§í ÉGÉtÉFÉNÉgä÷òA
	  É{ÉXã§í EFÉAÅ[ÉJÉCÉuAMBì‡ÇÕÅAÅuAME*nå¬ Å® TEXAMB Å® OBJECT*må¬Åv Ç∆Ç¢Ç§ï¿Ç—Ç…Ç»Ç¡ÇƒÇ¢ÇÈïKóvÇ™Ç†ÇËÇ‹Ç∑ÅB
	 */
	GMD_DWORK_NO_EFFECT_BOSS_CMN_START,
	
	GMD_DWORK_NO_EFFECT_BOSS_CMN_AMBTEX = GMD_DWORK_NO_EFFECT_BOSS_CMN_START,	//!< ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ ÉeÉNÉXÉ`ÉÉÉAÅ[ÉJÉCÉuAMB
	GMD_DWORK_NO_EFFECT_BOSS_CMN_TEXLIST,		//!< ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ ÉeÉNÉXÉ`ÉÉ (VRAMè„ÇÃÉeÉNÉXÉ`ÉÉÇ…ä÷òAïtÇØ)
	
	GMD_DWORK_NO_EFFECT_BOSS_CMN_AME_START,		//!< ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ AME
	GMD_DWORK_NO_EFFECT_BOSS_CMN_AME_00	= GMD_DWORK_NO_EFFECT_BOSS_CMN_AME_START,
	GMD_DWORK_NO_EFFECT_BOSS_CMN_AME_END	= GMD_DWORK_NO_EFFECT_BOSS_CMN_AME_START + IDB_EFF_BS_CMN_EFF_BS_TEX_AMB,	//! ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ AME èIí[
	
	GMD_DWORK_NO_EFFECT_BOSS_CMN_MODELDAT_START,	//!< ã§í ÉGÉtÉFÉNÉgÉÇÉfÉãÉfÅ[É^ .*noÉtÉ@ÉCÉã
	GMD_DWORK_NO_EFFECT_BOSS_CMN_MODELDAT_00	= GMD_DWORK_NO_EFFECT_BOSS_CMN_MODELDAT_START,
	// àÍâûÅAAMEÇÃêîï™ópà”ÇµÇƒÇ®Ç≠
	GMD_DWORK_NO_EFFECT_BOSS_CMN_MODELDAT_END	= GMD_DWORK_NO_EFFECT_BOSS_CMN_MODELDAT_START + IDB_EFF_BS_CMN_EFF_BS_TEX_AMB,
	
	GMD_DWORK_NO_EFFECT_BOSS_CMN_OBJECT_START,	//!< ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ NNÉIÉuÉWÉFÉNÉg (VRAMè„ÇÃÉÇÉfÉãÇ…ä÷òAïtÇØ)
	GMD_DWORK_NO_EFFECT_BOSS_CMN_OBJECT_00 = GMD_DWORK_NO_EFFECT_BOSS_CMN_OBJECT_START,
	// àÍâûÅAAMEÇÃêîï™ópà”ÇµÇƒÇ®Ç≠
	GMD_DWORK_NO_EFFECT_BOSS_CMN_OBJECT_END	= GMD_DWORK_NO_EFFECT_BOSS_CMN_OBJECT_START + IDB_EFF_BS_CMN_EFF_BS_TEX_AMB,	//!< ã§í ÉGÉtÉFÉNÉgÉfÅ[É^ NNÉIÉuÉWÉFÉNÉg èIí[
	
	GMD_DWORK_NO_EFFECT_BOSS_CMN_MDL_AMBTEX_START,	//!< ã§í ÉGÉtÉFÉNÉg ÉÇÉfÉãópÉeÉNÉXÉ`ÉÉAMB
	GMD_DWORK_NO_EFFECT_BOSS_CMN_MDL_AMBTEX_END	= GMD_DWORK_NO_EFFECT_BOSS_CMN_MDL_AMBTEX_START + IDB_EFF_BS_CMN_EFF_BS_TEX_AMB,
	
	GMD_DWORK_NO_EFFECT_BOSS_CMN_MDL_TEXLIST_START,	//!< ã§í ÉGÉtÉFÉNÉg ÉÇÉfÉãópÉeÉNÉXÉ`ÉÉÉäÉXÉg
	GMD_DWORK_NO_EFFECT_BOSS_CMN_MDL_TEXLIST_END	= GMD_DWORK_NO_EFFECT_BOSS_CMN_MDL_TEXLIST_START + IDB_EFF_BS_CMN_EFF_BS_TEX_AMB,
	
	GMD_DWORK_NO_EFFECT_BOSS_CMN_END,
	
	GMD_DWORK_NO_EFFECT_END	= GMD_DWORK_NO_EFFECT_BOSS_CMN_END,
	// ---------------- ÉGÉtÉFÉNÉgÇ±Ç±Ç‹Ç≈ --------------------------
	
	// ÉGÉlÉ~Å[
	GMD_DWORK_NO_ENEMY_START = GMD_DWORK_NO_EFFECT_END,

	GMD_DWORK_NO_ENEMY_HARISENBO_MODEL = GMD_DWORK_NO_ENEMY_START,	//!< ÉnÉäÉZÉìÉ{ ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_HARISENBO_TEX,	//!< ÉnÉäÉZÉìÉ{ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_HARISENBO_MTN,	//!< ÉnÉäÉZÉìÉ{ ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_MOTORA_MODEL,	//!< ÉÇÉgÉâ ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_MOTORA_TEX,		//!< ÉÇÉgÉâ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_MOTORA_MTN,		//!< ÉÇÉgÉâ ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_GABU_MODEL,		//!< ÉKÉuÉbÉ`Éá ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_GABU_TEX,		//!< ÉKÉuÉbÉ`Éá ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_GABU_MTN,		//!< ÉKÉuÉbÉ`Éá ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_STING_MODEL,		//!< ÉXÉeÉBÉìÉKÅ[ ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_STING_TEX,		//!< ÉXÉeÉBÉìÉKÅ[ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_STING_MTN,		//!< ÉXÉeÉBÉìÉKÅ[ ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_MEREON_MODEL,	//!< ÉÅÉåÉIÉì ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_MEREON_R_MODEL,	//!< ÉÅÉåÉIÉì ÉçÉPÉbÉg ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_MEREON_TEX,		//!< ÉÅÉåÉIÉì ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_MEREON_MTN,		//!< ÉÅÉåÉIÉì ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_MOGU_MODEL,		//!< ÉÇÉOÉäÉì ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_MOGU_TEX,		//!< ÉÇÉOÉäÉì ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_MOGU_MTN,		//!< ÉÇÉOÉäÉì ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_GARDON_MODEL,	//!< ÉKÅ[ÉhÉì ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_GARDON_TEX,		//!< ÉKÅ[ÉhÉì ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_GARDON_MTN,		//!< ÉKÅ[ÉhÉì ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_T_STAR_MODEL,	//!< ÉeÉãÉXÉ^Å[ ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_T_STAR_TEX,		//!< ÉeÉãÉXÉ^Å[ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_T_STAR_MTN,		//!< ÉeÉãÉXÉ^Å[ ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_T_STAR_MAT,		//!< ÉeÉãÉXÉ^Å[ É}ÉeÉäÉAÉã
	GMD_DWORK_NO_ENEMY_KANI_MODEL,		//!< Ç©Ç…ÉpÉìÉ` ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_KANI_TEX,		//!< Ç©Ç…ÉpÉìÉ` ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_KANI_MTN,		//!< Ç©Ç…ÉpÉìÉ` ÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_ENEMY_HARO_MODEL,		//!< ÉnÉçÉQÉì ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_HARO_TEX,		//!< ÉnÉçÉQÉì ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_HARO_MTN,		//!< ÉnÉçÉQÉì ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_UNIDES_MODEL,	//!< ÉEÉjÉfÉX ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_UNIDES_TEX,		//!< ÉEÉjÉfÉX ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_UNIDES_MTN,		//!< ÉEÉjÉfÉX ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_UNIUNI_MODEL,	//!< ÉEÉjÉEÉj ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_UNIUNI_TEX,		//!< ÉEÉjÉEÉj ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_UNIUNI_MTN,		//!< ÉEÉjÉEÉj ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_BUKU_MODEL,		//!< ÉuÉNÉuÉN ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_BUKU_TEX,		//!< ÉuÉNÉuÉN ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_BUKU_MTN,		//!< ÉuÉNÉuÉN ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_ENEMY_KAMA_MODEL,		//!< ÉJÉ}ÉLÉâÅ[ ÉÇÉfÉã
	GMD_DWORK_NO_ENEMY_KAMA_TEX,		//!< ÉJÉ}ÉLÉâÅ[ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_ENEMY_KAMA_MTN,		//!< ÉJÉ}ÉLÉâÅ[ ÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_ENEMY_END,
	
	// É{ÉX
	GMD_DWORK_NO_BOSS_START,

	GMD_DWORK_NO_BOSS_01_BODY_MTN = GMD_DWORK_NO_BOSS_START,		//!< ñ{ëÃÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_01_CHAIN_MTN,		//!< çΩÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_01_EGG_MTN,		//!< ÉGÉbÉOÉ}ÉìÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_01_EF_SW00_ES,	//!< è’åÇîgÉGÉtÉFÉNÉg00 ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_01_EF_SW01_ES,	//!< è’åÇîgÉGÉtÉFÉNÉg01 ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_01_EF_SW02_ES,	//!< è’åÇîgÉGÉtÉFÉNÉg02 ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_01_EF_SW_AMBTEX,	//!< è’åÇîgÉGÉtÉFÉNÉg ESÉeÉNÉXÉ`ÉÉAMB
	GMD_DWORK_NO_BOSS_01_EF_SW_TEXLIST,	//!< è’åÇîgÉGÉtÉFÉNÉg ESÉeÉNÉXÉ`ÉÉÉäÉXÉg

	GMD_DWORK_NO_BOSS_02_BODY_MTN,			//!< ñ{ëÃÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_02_BODY_MAT,			//!< ñ{ëÃÉ}ÉeÉäÉAÉã
	GMD_DWORK_NO_BOSS_02_EGG_MTN,			//!< ÉGÉbÉOÉ}ÉìÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_02_EF_BLITZ00_ES,		//!< ÉAÅ[ÉÄópìdåÇÉGÉtÉFÉNÉgÅiÉRÉAÅj ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_02_EF_BLITZ01_ES,		//!< ÉAÅ[ÉÄópìdåÇÉGÉtÉFÉNÉgÅiìdåÇÅj ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_02_EF_BLITZ02_ES,		//!< ÉAÅ[ÉÄópìdåÇÉGÉtÉFÉNÉgÅiÉAÅ[ÉÄÅj ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_02_EF_BALL_ES,		//!< ÉgÉQÉ{Å[Éãï™ó£ÉGÉtÉFÉNÉg ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_02_EF_BALL_PART_ES,	//!< ÉgÉQÉ{Å[Éãï™ó£îjï–ÉGÉtÉFÉNÉg ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_02_EF_ROLLATTACK01_ES,	//!< âÒì]çUåÇÉGÉtÉFÉNÉg ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_02_EF_ROLLATTACK02_ES,	//!< âÒì]çUåÇÉGÉtÉFÉNÉg ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_02_EF_ROLLATTACK03_ES,	//!< âÒì]çUåÇÉGÉtÉFÉNÉg ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_02_EF_AMBTEX,			//!< BOSS2ÉGÉtÉFÉNÉg ESÉeÉNÉXÉ`ÉÉAMB
	GMD_DWORK_NO_BOSS_02_EF_TEXLIST,		//!< BOSS2ÉGÉtÉFÉNÉg ESÉeÉNÉXÉ`ÉÉÉäÉXÉg
	GMD_DWORK_NO_BOSS_02_EF_ROLL_OBJECT,	//!< BOSS2ÉGÉtÉFÉNÉgâÒì]ÉÇÉfÉãNNÉIÉuÉWÉFÉNÉg
	GMD_DWORK_NO_BOSS_02_EF_ROLL_MDL_DATA,	//!< BOSS2ÉGÉtÉFÉNÉgâÒì]ÉÇÉfÉãÉfÅ[É^
	GMD_DWORK_NO_BOSS_02_EF_ROLL_AMBTEX,	//!< BOSS2ÉGÉtÉFÉNÉgâÒì]ÉÇÉfÉã ESÉeÉNÉXÉ`ÉÉAMB
	GMD_DWORK_NO_BOSS_02_EF_ROLL_TEXLIST,	//!< BOSS2ÉGÉtÉFÉNÉgâÒì]ÉÇÉfÉã ESÉeÉNÉXÉ`ÉÉÉäÉXÉg

	GMD_DWORK_NO_BOSS_03_BODY_MTN,			//!< ñ{ëÃÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_03_EGG_MTN,			//!< ÉGÉbÉOÉ}ÉìÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_BOSS_04_BODY_MTN,			//!< ñ{ëÃÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_04_EGG_MTN,			//!< ÉGÉbÉOÉ}ÉìÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_04_CAPSULE_MTN,		//!< ÉJÉvÉZÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_BOSS_04_EF_AMBTEX,			//!< BOSS4ÉGÉtÉFÉNÉg ESÉeÉNÉXÉ`ÉÉAMB
	GMD_DWORK_NO_BOSS_04_EF_TEXLIST,		//!< BOSS4ÉGÉtÉFÉNÉg ESÉeÉNÉXÉ`ÉÉÉäÉXÉg
	GMD_DWORK_NO_BOSS_04_EF_CAP_EX_ES,		//!< ÉJÉvÉZÉãîöîj ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_04_EF_EGG1_EX_ES,		//!< ÇøÇ—ÉGÉbÉOîöîj ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_04_EF_EGG2_EX_ES,		//!< ÇøÇ—ÉGÉbÉOîöîj ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_04_EF_EGG3_EX_ES,		//!< ÇøÇ—ÉGÉbÉOîöîj ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_04_EF_BOOST1_ES,		//!< ÉuÅ[ÉXÉ^Å[ ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_04_EF_BOOST2_ES,		//!< ÉuÅ[ÉXÉ^Å[ ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_04_EF_BOOSTX_ES,		//!< ÉuÅ[ÉXÉ^Å[ëÊÇQå`ë‘ ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_04_EF_BOOSTX2_ES,		//!< ÉuÅ[ÉXÉ^Å[ëÊÇQå`ë‘ ESÉGÉtÉFÉNÉg(â∫)
	GMD_DWORK_NO_BOSS_04_EF_PARTS_EX_ES,	//!< ÉpÅ[Écîöîj ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_04_EF_BOSS_LIGHT_ES,	//!< ÉâÉCÉg ESÉGÉtÉFÉNÉg
	
	GMD_DWORK_NO_BOSS_05_BODY_MTN,			//!< ñ{ëÃÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_05_EGG_MTN,			//!< ÉGÉbÉOÉ}ÉìÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_05_ROCKET_MTN,		//!< ÉçÉPÉbÉgÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_05_CTPLT_MAT,			//!< ÉJÉ^ÉpÉãÉgÉ}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_05_LAND01_MAT,		//!< ë´èÍ01É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_05_LAND02_MAT,		//!< ë´èÍ02É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_05_LAND03_MAT,		//!< ë´èÍ03É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_BOSS_05_EF_BLITZ_FB_00_ES,	//!< Å´BOSS5ÉGÉtÉFÉNÉg ESÉGÉtÉFÉNÉg
	GMD_DWORK_NO_BOSS_05_EF_BLITZ_FB_01_ES,
	GMD_DWORK_NO_BOSS_05_EF_BLITZ_FB_02_ES,
	GMD_DWORK_NO_BOSS_05_EF_BLITZ_FB_03_ES,
	GMD_DWORK_NO_BOSS_05_EF_BLITZ_FB_C_00_ES,
	GMD_DWORK_NO_BOSS_05_EF_BLITZ_FB_C_01_ES,
	GMD_DWORK_NO_BOSS_05_EF_BLITZ_FB_C_02_ES,
	GMD_DWORK_NO_BOSS_05_EF_BLITZ_FB_START_ES,
	GMD_DWORK_NO_BOSS_05_EF_FB_SMORK00_ES,
	GMD_DWORK_NO_BOSS_05_EF_FB_SMORK01_ES,
	GMD_DWORK_NO_BOSS_05_EF_FB_SMORK02_ES,
	GMD_DWORK_NO_BOSS_05_EF_GLASS_ES,
	GMD_DWORK_NO_BOSS_05_EF_JET_FB_ES,
	GMD_DWORK_NO_BOSS_05_EF_JET_FB_SMORK_ES,
	GMD_DWORK_NO_BOSS_05_EF_ROCKET_BLITZ_ES,
	GMD_DWORK_NO_BOSS_05_EF_ROCKET_E_ES,
	GMD_DWORK_NO_BOSS_05_EF_ROCKET_JET_ES,
	GMD_DWORK_NO_BOSS_05_EF_ROCKET_S_ES,
	GMD_DWORK_NO_BOSS_05_EF_ROCKET_SMORK_ES,
	GMD_DWORK_NO_BOSS_05_EF_SHOCK_ES,
	GMD_DWORK_NO_BOSS_05_EF_SHOCK_ATK_ES,
	GMD_DWORK_NO_BOSS_05_EF_TARGET_FB_ES,
	GMD_DWORK_NO_BOSS_05_EF_TARGET_FB_E_ES,
	GMD_DWORK_NO_BOSS_05_EF_TARGET_FB_S_ES,
	GMD_DWORK_NO_BOSS_05_EF_TARGET_FB_W_ES,
	GMD_DWORK_NO_BOSS_05_EF_TARGET_FB_W_E_ES,
	GMD_DWORK_NO_BOSS_05_EF_TARGET_FB_W_S_ES,
	GMD_DWORK_NO_BOSS_05_EF_CMN_AMBTEX,		//!< BOSS5ÉGÉtÉFÉNÉgã§í ÉeÉNÉXÉ`ÉÉAMB
	GMD_DWORK_NO_BOSS_05_EF_CMN_TEXLIST,	//!< BOSS5ÉGÉtÉFÉNÉgã§í ÉeÉNÉXÉ`ÉÉÉäÉXÉg
	GMD_DWORK_NO_BOSS_05_EF_MD_JT_AMBTEX,	//!< BOSS5ÉGÉtÉFÉNÉg ÉWÉFÉbÉgópÉeÉNÉXÉ`ÉÉAMB
	GMD_DWORK_NO_BOSS_05_EF_MD_JT_TEXLIST,	//!< BOSS5ÉGÉtÉFÉNÉg ÉWÉFÉbÉgópÉeÉNÉXÉ`ÉÉÉäÉXÉg
	GMD_DWORK_NO_BOSS_05_EF_TH_AMBTEX,		//!< BOSS5ÉGÉtÉFÉNÉg ìdåÇópÉeÉNÉXÉ`ÉÉAMB
	GMD_DWORK_NO_BOSS_05_EF_TH_TEXLIST,		//!< BOSS5ÉGÉtÉFÉNÉg ìdåÇópÉeÉNÉXÉ`ÉÉÉäÉXÉg
	GMD_DWORK_NO_BOSS_05_EF_JET_MODELDAT,	//!< BOSS5ÉGÉtÉFÉNÉg ÉWÉFÉbÉgópÉÇÉfÉãÉfÅ[É^
	GMD_DWORK_NO_BOSS_05_EF_JET_OBJECT,		//!< BOSS5ÉGÉtÉFÉNÉg ÉWÉFÉbÉgópNNÉIÉuÉWÉFÉNÉg
	GMD_DWORK_NO_BOSS_05_EF_THUNDER_MODELDAT,	//!< BOSS5ÉGÉtÉFÉNÉg ìdåÇópÉÇÉfÉãÉfÅ[É^
	GMD_DWORK_NO_BOSS_05_EF_THUNDER_OBJECT,	//!< BOSS5ÉGÉtÉFÉNÉg ìdåÇópNNÉIÉuÉWÉFÉNÉg
	
	GMD_DWORK_NO_BOSS_END,
	
	// ÉMÉ~ÉbÉN
	GMD_DWORK_NO_GMK_START = GMD_DWORK_NO_BOSS_END,

	GMD_DWORK_NO_GMK_ITEM_MODEL = GMD_DWORK_NO_GMK_START,		//!< ÉMÉ~ÉbÉN ÉAÉCÉeÉÄ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_ITEM_TEX,			//!< ÉMÉ~ÉbÉN ÉAÉCÉeÉÄ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SPRING_MODEL,		//!< ÉMÉ~ÉbÉN ÉXÉvÉäÉìÉO ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SPRING_TEX,		//!< ÉMÉ~ÉbÉN ÉXÉvÉäÉìÉO ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SPRING_MTN,		//!< ÉMÉ~ÉbÉN ÉXÉvÉäÉìÉO ÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_B_LAND1_MODEL,		//!< ÉMÉ~ÉbÉN ïˆÇÍÇÈë´èÍ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_B_LAND1_TEX,		//!< ÉMÉ~ÉbÉN ïˆÇÍÇÈë´èÍ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_B_LAND1_MTN,		//!< ÉMÉ~ÉbÉN ïˆÇÍÇÈë´èÍ ïˆÇÍÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_B_WALL_MODEL,		//!< ÉMÉ~ÉbÉN îjâÛâ¬î\Ç»ï« ÉÇÉfÉã
	GMD_DWORK_NO_GMK_B_WALL_TEX,		//!< ÉMÉ~ÉbÉN îjâÛâ¬î\Ç»ï« ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_B_OBJ_MODEL,		//!< ÉMÉ~ÉbÉN îjâÛâ¬ÉIÉuÉWÉF ÉÇÉfÉã
	GMD_DWORK_NO_GMK_B_OBJ_TEX,			//!< ÉMÉ~ÉbÉN îjâÛâ¬ÉIÉuÉWÉF ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_LAND_1_MODEL,		//!< ÉMÉ~ÉbÉN ïÇìá ÉÇÉfÉã
	GMD_DWORK_NO_GMK_LAND_1_TEX,		//!< ÉMÉ~ÉbÉN ïÇìá ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_LAND_2_MODEL,		//!< ÉMÉ~ÉbÉN ïÇìá ÉÇÉfÉã
	GMD_DWORK_NO_GMK_LAND_2_TEX,		//!< ÉMÉ~ÉbÉN ïÇìá ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_LAND_2_MTN,		//!< ÉMÉ~ÉbÉN ïÇìá ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_GMK_LAND_2_MAT,		//!< ÉMÉ~ÉbÉN ïÇìá É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_LAND_3_MODEL,		//!< ÉMÉ~ÉbÉN ïÇìá ÉÇÉfÉã
	GMD_DWORK_NO_GMK_LAND_3_TEX,		//!< ÉMÉ~ÉbÉN ïÇìá ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_LAND_3_ROPE_MAT,	//!< ÉMÉ~ÉbÉN ïÇìáÉçÅ[Év É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
#if _IPHONE
	GMD_DWORK_NO_GMK_LAND_3_TVX,		//!< ÉMÉ~ÉbÉN ïÇìá TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_LAND_4_MODEL,		//!< ÉMÉ~ÉbÉN ïÇìá ÉÇÉfÉã
	GMD_DWORK_NO_GMK_LAND_4_TEX,		//!< ÉMÉ~ÉbÉN ïÇìá ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_LAND_F_MODEL,		//!< ÉMÉ~ÉbÉN ïÇìá ÉÇÉfÉã
	GMD_DWORK_NO_GMK_LAND_F_TEX,		//!< ÉMÉ~ÉbÉN ïÇìá ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_LAND_F_MAT,		//!< ÉMÉ~ÉbÉN ïÇìá É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_ROCK_MODEL,		//!< ÉMÉ~ÉbÉN ëÂä‚ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_ROCK_TEX,			//!< ÉMÉ~ÉbÉN ëÂä‚ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_ROCK_MTN,			//!< ÉMÉ~ÉbÉN É^Å[ÉUÉìÉçÅ[Év ÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_PULLEY_MODEL,		//!< ÉMÉ~ÉbÉN ääé‘ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_PULLEY_TEX,		//!< ÉMÉ~ÉbÉN ääé‘ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_PULLEY_MTN,		//!< ÉMÉ~ÉbÉN ääé‘ ÉÇÅ[ÉVÉáÉì
	
	GMD_DWORK_NO_GMK_NEEDLE_MODEL,		//!< ÉMÉ~ÉbÉN êj ÉÇÉfÉã
	GMD_DWORK_NO_GMK_NEEDLE_TEX,		//!< ÉMÉ~ÉbÉN êj ÉeÉNÉXÉ`ÉÉ
#if _IPHONE
	GMD_DWORK_NO_GMK_NEEDLE_TVX,		//!< ÉMÉ~ÉbÉN êj TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_DASH_P_MODEL,		//!< ÉMÉ~ÉbÉN É_ÉbÉVÉÖÉpÉlÉã ÉÇÉfÉã
	GMD_DWORK_NO_GMK_DASH_P_TEX,		//!< ÉMÉ~ÉbÉN É_ÉbÉVÉÖÉpÉlÉã ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_DASH_P_MTN,		//!< ÉMÉ~ÉbÉN É_ÉbÉVÉÖÉpÉlÉã ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_GMK_DASH_P_MAT,		//!< ÉMÉ~ÉbÉN É_ÉbÉVÉÖÉpÉlÉã É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_T_ROPE_MODEL,		//!< ÉMÉ~ÉbÉN É^Å[ÉUÉìÉçÅ[Év ÉÇÉfÉã
	GMD_DWORK_NO_GMK_T_ROPE_TEX,		//!< ÉMÉ~ÉbÉN É^Å[ÉUÉìÉçÅ[Év ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_T_ROPE_MTN,		//!< ÉMÉ~ÉbÉN É^Å[ÉUÉìÉçÅ[Év ÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_WATER_SLIDER_MODEL,	//!< ÉMÉ~ÉbÉN ÉEÉHÅ[É^Å[ÉXÉâÉCÉ_Å[ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_WATER_SLIDER_TEX,		//!< ÉMÉ~ÉbÉN ÉEÉHÅ[É^Å[ÉXÉâÉCÉ_Å[ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_WATER_SLIDER_MTN,		//!< ÉMÉ~ÉbÉN ÉEÉHÅ[É^Å[ÉXÉâÉCÉ_Å[ ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_GMK_WATER_SLIDER_MAT,		//!< ÉMÉ~ÉbÉN ÉEÉHÅ[É^Å[ÉXÉâÉCÉ_Å[ É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_GOAL_PNL_MODEL,	//!< ÉMÉ~ÉbÉN ÉSÅ[ÉãÉpÉlÉã ÉÇÉfÉã
	GMD_DWORK_NO_GMK_GOAL_PNL_TEX,		//!< ÉMÉ~ÉbÉN ÉSÅ[ÉãÉpÉlÉã ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_P_MARKER_MODEL,	//!< ÉMÉ~ÉbÉN É|ÉCÉìÉgÉ}Å[ÉJÅ[ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_P_MARKER_TEX,		//!< ÉMÉ~ÉbÉN É|ÉCÉìÉgÉ}Å[ÉJÅ[ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_P_MARKER_MTN,		//!< ÉMÉ~ÉbÉN É|ÉCÉìÉgÉ}Å[ÉJÅ[ ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_GMK_P_MARKER_MAT,		//!< ÉMÉ~ÉbÉN É|ÉCÉìÉgÉ}Å[ÉJÅ[ É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_PISTON_MODEL,		//!< ÉMÉ~ÉbÉN ÉsÉXÉgÉìÅóÉ]Å[ÉìÇSçHèÍÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_PISTON_TEX,		//!< ÉMÉ~ÉbÉN ÉsÉXÉgÉìÅóÉ]Å[ÉìÇSçHèÍÅ@ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_BELTCONV_MODEL,	//!< ÉMÉ~ÉbÉN ÉxÉãÉgÉRÉìÉxÉÑÅ[ÅóÉ]Å[ÉìÇSçHèÍÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_BELTCONV_TEX,		//!< ÉMÉ~ÉbÉN ÉxÉãÉgÉRÉìÉxÉÑÅ[ÅóÉ]Å[ÉìÇSçHèÍÅ@ÉeÉNÉXÉ`ÉÉ
#if _IPHONE
	GMD_DWORK_NO_GMK_BELTCONV_TVX,		//!< ÉMÉ~ÉbÉN ÉxÉãÉgÉRÉìÉxÉÑÅ[ÅóÉ]Å[ÉìÇSçHèÍÅ@TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_STOPPER_MODEL,		//!< ÉMÉ~ÉbÉN ÉXÉgÉbÉpÅ[ÅóÉ]Å[ÉìÇQÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_STOPPER_TEX,		//!< ÉMÉ~ÉbÉN ÉXÉgÉbÉpÅ[ÅóÉ]Å[ÉìÇQÅ@ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_STOPPER_MAT,		//!< ÉMÉ~ÉbÉN ÉXÉgÉbÉpÅ[ÅóÉ]Å[ÉìÇQÅ@É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_UPBUMPER_MODEL,	//!< ÉMÉ~ÉbÉN ìoÇÈÉoÉìÉpÅ[ÅóÉ]Å[ÉìÇSÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_UPBUMPER_TEX,		//!< ÉMÉ~ÉbÉN ìoÇÈÉoÉìÉpÅ[ÅóÉ]Å[ÉìÇSÅ@ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_BUMPER_MODEL,		//!< ÉMÉ~ÉbÉN ÉoÉìÉpÅ[ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_BUMPER_TEX,		//!< ÉMÉ~ÉbÉN ÉoÉìÉpÅ[ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_BUMPER_MTN,		//!< ÉMÉ~ÉbÉN ÉoÉìÉpÅ[ ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_GMK_BUMPER_MAT,		//!< ÉMÉ~ÉbÉN ÉoÉìÉpÅ[ É}ÉeÉäÉAÉã

	GMD_DWORK_NO_GMK_SPEAR_MODEL,		//!< ÉMÉ~ÉbÉNÅ@ëÑÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SPEAR_TEX,			//!< ÉMÉ~ÉbÉNÅ@ëÑÅ@ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_CANNON_MODEL,		//!< ÉMÉ~ÉbÉNÅ@ëÂñCÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_CANNON_TEX,		//!< ÉMÉ~ÉbÉNÅ@ëÂñCÅ@ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_CAPSULE_MODEL,		//!< ÉMÉ~ÉbÉN ÉJÉvÉZÉã ÉÇÉfÉã
	GMD_DWORK_NO_GMK_CAPSULE_TEX,		//!< ÉMÉ~ÉbÉN ÉJÉvÉZÉã ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_CAPSULE_MTN,		//!< ÉMÉ~ÉbÉN ÉJÉvÉZÉã ÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_BOBBIN_MODEL,		//!< ÉMÉ~ÉbÉN É{ÉrÉì ÉÇÉfÉã
	GMD_DWORK_NO_GMK_BOBBIN_TEX,		//!< ÉMÉ~ÉbÉN É{ÉrÉì ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_BOBBIN_MTN,		//!< ÉMÉ~ÉbÉN É{ÉrÉì ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_GMK_BOBBIN_MAT,		//!< ÉMÉ~ÉbÉN É{ÉrÉì É}ÉeÉäÉAÉã

	GMD_DWORK_NO_GMK_FLIPPER_MODEL,		//!< ÉMÉ~ÉbÉN ÉtÉäÉbÉpÅ[ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_FLIPPER_TEX,		//!< ÉMÉ~ÉbÉN ÉtÉäÉbÉpÅ[ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_FLIPPER_MAT,		//!< ÉMÉ~ÉbÉN ÉtÉäÉbÉpÅ[ É}ÉeÉäÉAÉã

	GMD_DWORK_NO_GMK_ANIMAL_MODEL,		//!< ÉMÉ~ÉbÉN ìÆï® ÉÇÉfÉã
	GMD_DWORK_NO_GMK_ANIMAL_TEX,		//!< ÉMÉ~ÉbÉN ìÆï® ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_ANIMAL_MTN,		//!< ÉMÉ~ÉbÉN ìÆï® ÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_SLOT_MODEL,		//!< ÉMÉ~ÉbÉNÅ@ÉXÉçÉbÉgÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SLOT_TEX,			//!< ÉMÉ~ÉbÉNÅ@ÉXÉçÉbÉgÅ@ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SLOT_MAT,			//!< ÉMÉ~ÉbÉNÅ@ÉXÉçÉbÉgÅ@É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_SEESAW_MODEL,		//!< ÉMÉ~ÉbÉN ÉVÅ[É\Å[ÅóÉ]Å[ÉìÇSÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SEESAW_TEX,		//!< ÉMÉ~ÉbÉN ÉVÅ[É\Å[ÅóÉ]Å[ÉìÇSÅ@ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_BRIDGE_MODEL,		//!< ÉMÉ~ÉbÉN ä€ëæã¥ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_BRIDGE_TEX,		//!< ÉMÉ~ÉbÉN ä€ëæã¥ ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_SPL_RING_MODEL,	//!< ÉMÉ~ÉbÉN SPLÉäÉìÉO ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SPL_RING_TEX,		//!< ÉMÉ~ÉbÉN SPLÉäÉìÉO ÉeÉNÉXÉ`ÉÉ
#if _IPHONE
	GMD_DWORK_NO_GMK_SPL_RING_MAT,      //!< ÉMÉ~ÉbÉN SPLÉäÉìÉO É}ÉeÉäÉAÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_SPCTPLT_MODEL,		//!< ÉMÉ~ÉbÉN SPÉJÉ^ÉpÉãÉg ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SPCTPLT_TEX,		//!< ÉMÉ~ÉbÉN SPÉJÉ^ÉpÉãÉg ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SPCTPLT_MTN,		//!< ÉMÉ~ÉbÉN SPÉJÉ^ÉpÉãÉg ÉÇÅ[ÉVÉáÉì
	GMD_DWORK_NO_GMK_SPCTPLT_MAT,		//!< ÉMÉ~ÉbÉN SPÉJÉ^ÉpÉãÉg É}ÉeÉäÉAÉã

	GMD_DWORK_NO_GMK_GEAR,				//!< ÉMÉ~ÉbÉN éïé‘ ínå`ÉfÅ[É^ìô
	GMD_DWORK_NO_GMK_GEAR_MODEL,		//!< ÉMÉ~ÉbÉN éïé‘ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_GEAR_TEX,			//!< ÉMÉ~ÉbÉN éïé‘ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_GEAR2_MODEL,		//!< ÉMÉ~ÉbÉN éïé‘2 ÉÇÉfÉã
	GMD_DWORK_NO_GMK_GEAR2_OPT_MODEL,	//!< ÉMÉ~ÉbÉN éïé‘2 ÉIÉvÉVÉáÉìéïé‘ ÉÇÉfÉã

	GMD_DWORK_NO_GMK_PRESSWALL_MODEL,	//!< ÉMÉ~ÉbÉN îóÇÈï« ÉÇÉfÉã
	GMD_DWORK_NO_GMK_PRESSWALL_TEX,		//!< ÉMÉ~ÉbÉN îóÇÈï« ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_PRESSWALL_MTN,		//!< ÉMÉ~ÉbÉN îóÇÈï« ÉÇÅ[ÉVÉáÉì(Zone4)
	GMD_DWORK_NO_GMK_PRESSWALL_MAT,		//!< ÉMÉ~ÉbÉN îóÇÈï« É}ÉeÉäÉAÉã(Zone4)

	GMD_DWORK_NO_GMK_SS_SQUARE_MODEL,	//!< ÉMÉ~ÉbÉN SpecialStage éläpíå ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SS_SQUARE_TEX,		//!< ÉMÉ~ÉbÉN SpecialStage éläpíå ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SS_SQUARE_MAT,		//!< ÉMÉ~ÉbÉN SpecialStage éläpíå É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
#if _IPHONE
	GMD_DWORK_NO_GMK_SS_SQUARE_TVX,		//!< ÉMÉ~ÉbÉN SpecialStage éläpíå TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_SS_CIRCLE_MODEL,	//!< ÉMÉ~ÉbÉN SpecialStage ä€íå ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SS_CIRCLE_TEX,		//!< ÉMÉ~ÉbÉN SpecialStage ä€íå ÉeÉNÉXÉ`ÉÉ
#if _IPHONE
	GMD_DWORK_NO_GMK_SS_CIRCLE_TVX,		//!< ÉMÉ~ÉbÉN SpecialStage ä€íå TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_SS_ENDURANCE_MODEL,//!< ÉMÉ~ÉbÉN SpecialStage ëœãvíå ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SS_ENDURANCE_TEX,	//!< ÉMÉ~ÉbÉN SpecialStage ëœãvíå ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SS_ENDURANCE_MAT,	//!< ÉMÉ~ÉbÉN SpecialStage ëœãvíå É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
#if _IPHONE
	GMD_DWORK_NO_GMK_SS_ENDURANCE_TVX,	//!< ÉMÉ~ÉbÉN SpecialStage ëœãvíå TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_SS_GOAL_MODEL,		//!< ÉMÉ~ÉbÉN SpecialStage ÉSÅ[Éã ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SS_GOAL_TEX,		//!< ÉMÉ~ÉbÉN SpecialStage ÉSÅ[Éã ÉeÉNÉXÉ`ÉÉ
#if _IPHONE
	GMD_DWORK_NO_GMK_SS_GOAL_TVX,	//!< ÉMÉ~ÉbÉN SpecialStage ÉSÅ[Éã TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_SS_EMERALD_MODEL,	//!< ÉMÉ~ÉbÉN SpecialStage ÉJÉIÉXÉGÉÅÉâÉãÉh ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SS_EMERALD_TEX,	//!< ÉMÉ~ÉbÉN SpecialStage ÉJÉIÉXÉGÉÅÉâÉãÉh ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SS_EMERALD_MTN,	//!< ÉMÉ~ÉbÉN SpecialStage ÉJÉIÉXÉGÉÅÉâÉãÉh ÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_SS_1UP_MODEL,		//!< ÉMÉ~ÉbÉN SpecialStage 1up ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SS_1UP_TEX,		//!< ÉMÉ~ÉbÉN SpecialStage 1up ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_SS_TIME_MODEL,		//!< ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SS_TIME_TEX,		//!< ÉMÉ~ÉbÉN SpecialStage éûä‘ÉpÉlÉã ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_SS_RINGGATE_MODEL,	//!< ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SS_RINGGATE_TEX,	//!< ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SS_RINGGATE_MAT,	//!< ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
#if _IPHONE
	GMD_DWORK_NO_GMK_SS_RINGGATE_TVX,	//!< ÉMÉ~ÉbÉN SpecialStage ÉäÉìÉOÉQÅ[Ég TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_STEAMPIPE_MODEL,	//!< ÉMÉ~ÉbÉN ÉXÉ`Å[ÉÄÉpÉCÉvÅóÉ]Å[ÉìÇSçHèÍÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_STEAMPIPE_TEX,		//!< ÉMÉ~ÉbÉN ÉXÉ`Å[ÉÄÉpÉCÉvÅóÉ]Å[ÉìÇSçHèÍÅ@ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_DRAIN_TANK_MODEL,	//!< ÉMÉ~ÉbÉN îrâtëïíuÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_DRAIN_TANK_TEX,	//!< ÉMÉ~ÉbÉN îrâtëïíuÅ@ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_DRAIN_TANK_MAT,	//!< ÉMÉ~ÉbÉN îrâtëïíuÅ@É}ÉeÉäÉAÉã

	GMD_DWORK_NO_GMK_POPSTEAM_MODEL,	//!< ÉMÉ~ÉbÉN É|ÉbÉvÉXÉ`Å[ÉÄÅóÉ]Å[ÉìÇSçHèÍÅ@ÉÇÉfÉã
	GMD_DWORK_NO_GMK_POPSTEAM_TEX,		//!< ÉMÉ~ÉbÉN É|ÉbÉvÉXÉ`Å[ÉÄÅóÉ]Å[ÉìÇSçHèÍÅ@ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_TRUCK_MODEL,		//!< ÉMÉ~ÉbÉN ÉgÉçÉbÉR ÉÇÉfÉã
	GMD_DWORK_NO_GMK_TRUCK_TEX,			//!< ÉMÉ~ÉbÉN ÉgÉçÉbÉR ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_TRUCK_MTN,			//!< ÉMÉ~ÉbÉN ÉgÉçÉbÉR ÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_SWITCH_MODEL,		//!< ÉMÉ~ÉbÉN ÉXÉCÉbÉ` ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SWITCH_TEX,		//!< ÉMÉ~ÉbÉN ÉXÉCÉbÉ` ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SWITCH_MAT,		//!< ÉMÉ~ÉbÉN ÉXÉCÉbÉ` É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_SW_WALL_MODEL,		//!< ÉMÉ~ÉbÉN ÉXÉCÉbÉ`ï« ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SW_WALL_TEX,		//!< ÉMÉ~ÉbÉN ÉXÉCÉbÉ`ï« ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SW_WALL_MAT,		//!< ÉMÉ~ÉbÉN ÉXÉCÉbÉ`ï« É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
#if _IPHONE
	GMD_DWORK_NO_GMK_SW_WALL_TVX,		//!< ÉMÉ~ÉbÉN ÉXÉCÉbÉ`ï« TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_SHUTTER_MODEL,		//!< ÉMÉ~ÉbÉN ÉVÉÉÉbÉ^Å[ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SHUTTER_TEX,		//!< ÉMÉ~ÉbÉN ÉVÉÉÉbÉ^Å[ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SHUTTER_MAT,		//!< ÉMÉ~ÉbÉN ÉVÉÉÉbÉ^Å[ É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
	
	GMD_DWORK_NO_GMK_BOSS3_PILLAR_MODEL,	//!< ÉMÉ~ÉbÉN É{ÉX3ópíå ÉÇÉfÉã
	GMD_DWORK_NO_GMK_BOSS3_PILLAR_TEX,		//!< ÉMÉ~ÉbÉN É{ÉX3ópíå ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_BOSS3_PILLAR_MAT,		//!< ÉMÉ~ÉbÉN É{ÉX3ópíå É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
#if _IPHONE
	GMD_DWORK_NO_GMK_BOSS3_PILLAR_TVX,		//!< ÉMÉ~ÉbÉN É{ÉX3ópíå TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE
	GMD_DWORK_NO_GMK_BOSS3_WALL_MODEL,		//!< ÉMÉ~ÉbÉN É{ÉX3ópï« ÉÇÉfÉã
	GMD_DWORK_NO_GMK_BOSS3_WALL_TEX,		//!< ÉMÉ~ÉbÉN É{ÉX3ópï« ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_BOSS3_WALL_MAT,		//!< ÉMÉ~ÉbÉN É{ÉX3ópï« É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
#if _IPHONE
	GMD_DWORK_NO_GMK_BOSS3_WALL_TVX,		//!< ÉMÉ~ÉbÉN É{ÉX3ópï« TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_END_SONIC_MODEL,	//!< ÉMÉ~ÉbÉN ÉGÉìÉfÉBÉìÉOÉ\ÉjÉbÉNÇPñáäGï\é¶ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_END_SONIC_TEX,		//!< ÉMÉ~ÉbÉN ÉGÉìÉfÉBÉìÉOÉ\ÉjÉbÉNÇPñáäGï\é¶ ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_PRESS_PILLAR_MODEL,//!< ÉMÉ~ÉbÉN îóÇËèoÇ∑íå ÉÇÉfÉã
	GMD_DWORK_NO_GMK_PRESS_PILLAR_TEX,	//!< ÉMÉ~ÉbÉN îóÇËèoÇ∑íå ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_PRESS_PILLAR_MAT,	//!< ÉMÉ~ÉbÉN îóÇËèoÇ∑íå É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_DSIGN_MODEL,		//!< ÉMÉ~ÉbÉN äÎåØçêímä≈î¬ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_DSIGN_TEX,			//!< ÉMÉ~ÉbÉN äÎåØçêímä≈î¬ ÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_STFRL_RING_MODEL,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópÉäÉìÉOÉÇÉfÉã
	GMD_DWORK_NO_GMK_STFRL_RING_TEX,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópÉäÉìÉOÉeÉNÉXÉ`ÉÉ

	GMD_DWORK_NO_GMK_STFRL_FONT_TEX,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópÉtÉHÉìÉgÉfÅ[É^
	GMD_DWORK_NO_GMK_STFRL_CMN_WIN_TEX,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[Éãópã§í ÉEÉCÉìÉhÉE
	GMD_DWORK_NO_GMK_STFRL_SCR_IMG_TEX,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópÉXÉNÉäÅ[ÉìÉCÉÅÅ[ÉW
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_JP,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[Éãópã§í ÉÅÉbÉZÅ[ÉWJP
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_US,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[Éãópã§í ÉÅÉbÉZÅ[ÉWUS
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_FR,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[Éãópã§í ÉÅÉbÉZÅ[ÉWFR
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_IT,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[Éãópã§í ÉÅÉbÉZÅ[ÉWIT
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_GE,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[Éãópã§í ÉÅÉbÉZÅ[ÉWGE
	GMD_DWORK_NO_GMK_STFRL_CMN_MSG_SP,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[Éãópã§í ÉÅÉbÉZÅ[ÉWSP
	GMD_DWORK_NO_GMK_STFRL_END_TEX,		//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópENDâÊñ ópÉeÉNÉXÉ`ÉÉã§í 
	GMD_DWORK_NO_GMK_STFRL_END_TEX_JP,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópENDâÊñ ópÉeÉNÉXÉ`ÉÉJP
	GMD_DWORK_NO_GMK_STFRL_END_TEX_US,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópENDâÊñ ópÉeÉNÉXÉ`ÉÉUS
	GMD_DWORK_NO_GMK_STFRL_END_TEX_FR,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópENDâÊñ ópÉeÉNÉXÉ`ÉÉFR
	GMD_DWORK_NO_GMK_STFRL_END_TEX_IT,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópENDâÊñ ópÉeÉNÉXÉ`ÉÉIT
	GMD_DWORK_NO_GMK_STFRL_END_TEX_GE,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópENDâÊñ ópÉeÉNÉXÉ`ÉÉGE
	GMD_DWORK_NO_GMK_STFRL_END_TEX_SP,	//!< ÉMÉ~ÉbÉN ÉXÉ^ÉbÉtÉçÅ[ÉãópENDâÊñ ópÉeÉNÉXÉ`ÉÉSP
	
	GMD_DWORK_NO_GMK_SS_ARROW_MODEL,	//!< ÉMÉ~ÉbÉN SpecialStage ñÓàÛ ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SS_ARROW_TEX,		//!< ÉMÉ~ÉbÉN SpecialStage ñÓàÛ ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SS_ARROW_MAT,		//!< ÉMÉ~ÉbÉN SpecialStage ñÓàÛ É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì

	GMD_DWORK_NO_GMK_SS_OBLONG_MODEL,	//!< ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå ÉÇÉfÉã
	GMD_DWORK_NO_GMK_SS_OBLONG_TEX,		//!< ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå ÉeÉNÉXÉ`ÉÉ
	GMD_DWORK_NO_GMK_SS_OBLONG_MAT,		//!< ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
#if _IPHONE
	GMD_DWORK_NO_GMK_SS_OBLONG_TVX,		//!< ÉMÉ~ÉbÉN SpecialStage RingGateèIí[íå TVXÉÇÉfÉã(iPhone only)
#endif // _IPHONE

	GMD_DWORK_NO_GMK_START_EXP_MSG,		//!< ÉMÉ~ÉbÉNÇ…ï™óﬁ ÉQÅ[ÉÄäJénéûëÄçÏê‡ñæÉÅÉbÉZÅ[ÉWÉfÅ[É^
	GMD_DWORK_NO_GMK_START_EXP_WIN,		//!< ÉMÉ~ÉbÉNÇ…ï™óﬁ ÉQÅ[ÉÄäJénéûëÄçÏê‡ñæÉEÉBÉìÉhÉEÉfÅ[É^

	GMD_DWORK_NO_GMK_END,
	
	// ëïè¸
	GMD_DWORK_NO_DEC_START = GMD_DWORK_NO_GMK_END,
	//GMD_DWORK_NO_DEC_ = GMD_DWORK_NO_DEC_START,
	GMD_DWORK_NO_DEC_END,


	// ÉfÉoÉbÉOóp
//#if defined (MTD_DEBUG)
//	GMD_DWORK_NO_DEBUG_MAP_TXB,			// ÉfÉoÉbÉNóp
//#endif
	
	GMD_DWORK_NO_MAX = GMD_DWORK_NO_DEC_END		//!< égópÉfÅ[É^ÉèÅ[ÉNç≈ëÂêî
};

/*------ External Declarations -----------------------------------------*/
// =======================================================================
// test_func
/*!
  ä÷êîã@î\
 
  @param param0 [in] ì¸óÕà¯êî0ê‡ñæ
  @param param1 [out] èoóÕÉ|ÉCÉìÉ^à¯êî1ê‡ñæ
  @param param2 [io] ì¸èoóÕÉ|ÉCÉìÉ^à¯êî2ê‡ñæ
 
  @return ï‘ílê‡ñæ
 
  @note
  ï‚ë´ê‡ñæ
 */
// =======================================================================

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* GM_MAIN_DAT_H_ */
