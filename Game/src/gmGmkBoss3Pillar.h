// ==========================================================================
/*!
  @file gmGmkBoss3Pillar.h
  @brief ƒMƒ~ƒbƒNƒ{ƒX3’Œ

  @author Hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmGmkBoss3Pillar.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_GMK_BOSS3_PILLAR_H_
#define GM_GMK_BOSS3_PILLAR_H_


#if	defined(__cplusplus)
extern "C" {
#endif

//----- Include Files -------------------------------------------------------

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------


#define GMD_GMK_BOSS3_PILLAR_PATTERN_NUM	(7)	//ƒpƒ^[ƒ“”

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

// ==========================================================================
// GmGmkBoss3PillarBuild
/*!
 *	ƒMƒ~ƒbƒN ƒ{ƒX3’Œ ƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmGmkBoss3PillarBuild(void);

// ==========================================================================
// GmGmkBoss3PillarFlush
/*!
 *	ƒMƒ~ƒbƒN ƒ{ƒX3’Œ ƒf[ƒ^•Ğ•t‚¯
 */
// ==========================================================================
extern void GmGmkBoss3PillarFlush(void);

// ==========================================================================
// GmGmkBoss3PillarInitManager
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒ{ƒX3’ŒŠÇ—
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkBoss3PillarInitManager( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkBoss3PillarInitParts
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒ{ƒX3’Œ
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkBoss3PillarInitParts( GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkBoss3PillarInitWall
/*!
 *	ƒMƒ~ƒbƒN‰Šú‰»ŠÖ”@ƒ{ƒX3•Ç
 *
 *	@param eve_rec	[io] ƒŒƒR[ƒhƒ|ƒCƒ“ƒ^
 *	@param pos_x	[in] oŒ»À•W
 *	@param pos_y	[in] 
 *	@param type		[in] ˆ—“à—eƒ^ƒCƒv ’Êí‚Í0
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmGmkBoss3PillarInitWall( GMS_EVE_RECORD_EVENT* eve_rec ,fx32 pos_x, fx32 pos_y, u8 type );

// ==========================================================================
// GmGmkBoss3PillarChangeModeActive
/*!
 *	ƒAƒNƒeƒBƒu‚É•ÏX@ƒ{ƒX3’Œ
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param pattern_no	[in] ƒpƒ^[ƒ“”Ô†
 */
// ==========================================================================
extern void GmGmkBoss3PillarChangeModeActive( OBS_OBJECT_WORK* obj_work, s32 pattern_no );

// ==========================================================================
// GmGmkBoss3PillarChangeModeHurry
/*!
 *	ƒXƒs[ƒhƒAƒbƒv‚É•ÏX@ƒ{ƒX3’Œ
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 *	@param type		[in] ƒ^ƒCƒv
 */
// ==========================================================================
extern void GmGmkBoss3PillarChangeModeHurry( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
// GmGmkBoss3PillarChangeModeWait
/*!
 *	‘Ò‹@‚É•ÏX@ƒ{ƒX3’Œ
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern void GmGmkBoss3PillarChangeModeReturn( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
// GmGmkBoss3PillarChangeModeActive
/*!
 *	ƒAƒNƒeƒBƒu‚É•ÏX@ƒ{ƒX3’Œ
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern void GmGmkBoss3PillarChangeModeDelete( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
// GmGmkBoss3PillarGetActiveTime
/*!
 *	ƒAƒNƒeƒBƒuó‘Ô‚ÌƒtƒŒ[ƒ€”‚ğæ“¾
 *
 *	@param pattern_no	[in] ƒpƒ^[ƒ“”Ô†
 */
// ==========================================================================
extern s32 GmGmkBoss3PillarGetActiveTime( s32 pattern_no );



// ==========================================================================
// GmGmkBoss3PillarWallChangeModeActive
/*!
 *	ƒAƒNƒeƒBƒu‚É•ÏX@ƒ{ƒX3•Ç
 *
 *	@param obj_work		[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern void GmGmkBoss3PillarWallChangeModeActive( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
// GmGmkBoss3PillarWallChangeModeReturn
/*!
 *	‘Ò‹@‚É•ÏX@ƒ{ƒX3•Ç
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern void GmGmkBoss3PillarWallChangeModeReturn( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
// GmGmkBoss3PillarWallClearFlagNoPressDie
/*!
 *	ƒvƒŒƒCƒ„‚ğˆ³€‚³‚¹‚È‚¢ƒtƒ‰ƒO‚ğƒNƒŠƒA‚·‚éi=ˆ³€‚³‚¹‚éj
 *
 *	@param obj_work	[io] ƒIƒuƒWƒFƒNƒgƒ[ƒN
 */
// ==========================================================================
extern void GmGmkBoss3PillarWallClearFlagNoPressDie( OBS_OBJECT_WORK* obj_work );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

//----- Include Files -------------------------------------------------------

#endif // GM_GMK_BOSS3_PILLAR_H_
