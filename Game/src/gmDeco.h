// ==========================================================================
/*!
  @file gmDeco.h
  @brief 

  @author hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmDeco.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (æœˆ, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_DECO_
#define GM_DECO_

//----- Include Files -------------------------------------------------------


#if	defined(__cplusplus)
extern "C" {
#endif



#include "gmEventMgr.h"

//----- Definitions ---------------------------------------------------------
#if defined(GMD_DEBUG_NO_CREATE_DECO)
	#define GMD_DECO_TEST ( 0 )
#else
	#define GMD_DECO_TEST ( 1 )
#endif //defined(GMD_DEBUG_NO_CREATE_DECO)

///AMBƒf[ƒ^ƒCƒ“ƒfƒNƒX
enum GME_DECO_DATA_INDEX_AMB{
	GMD_DECO_DATA_INDEX_AMB_MDL = 0,
	GMD_DECO_DATA_INDEX_AMB_TEX,
	GMD_DECO_DATA_INDEX_AMB_MTN,
	GMD_DECO_DATA_INDEX_AMB_MAT,
	GMD_DECO_DATA_INDEX_AMB_MDL_RENDER,
	GMD_DECO_DATA_INDEX_AMB_TEX_RENDER,

	GMD_DECO_DATA_INDEX_AMB_NUM,
};

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

// ==========================================================================
//ƒf[ƒ^
// ==========================================================================

// ==========================================================================
// GmDecoInitData
/*!
 * ‘•üƒf[ƒ^‚ğ‰Šú‰»
 *
 * @param amb ambƒf[ƒ^ƒwƒbƒ_
 */
// ==========================================================================
extern void GmDecoInitData( AMS_AMB_HEADER* amb );

// ==========================================================================
// GmDecoBuildData
/*!
 * ‘•üƒf[ƒ^\’z
 */
// ==========================================================================
extern void GmDecoBuildData( void );

// ==========================================================================
// GmDecoCheckLoading
/*!
 * “Ç‚İ‚İ‘Ò‚¿ƒ`ƒFƒbƒN
 *
 * @return TRUEF“Ç‚İ‚İI—¹ FALSEF“Ç‚İ‚İ‘Ò‚¿
 *
 */
// ==========================================================================
extern BOOL GmDecoCheckLoading( void );

// ==========================================================================
// GmDecoInit
/*!
 * ‘•ü‰Šú‰»
 */
// ==========================================================================
extern void GmDecoInit( void );

// ==========================================================================
// GmDecoExit
/*!
 * ‘•üI—¹
 */
// ==========================================================================
extern void GmDecoExit( void );

// ==========================================================================
// GmDecoRelease
/*!
 * ‘•ü\’z‚µ‚½ƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
extern void GmDecoRelease( void );

// ==========================================================================
// GmDecoFlushData
/*!
 * ‘•ü“Ç‚İ‚ñ‚¾ƒf[ƒ^‰ğ•ú
 */
// ==========================================================================
extern void GmDecoFlushData( void );

// ==========================================================================
// GmDecoCheckFlushing
/*!
 * ƒf[ƒ^ŠJ•ú‘Ò‚¿
 *
 * @return TRUEFŠJ•úI—¹ FALSEFŠJ•ú‘Ò‚¿
 *
 */
// ==========================================================================
extern BOOL GmDecoCheckFlushing( void );



// ==========================================================================
//‘•üŠÇ—
// ==========================================================================

// ==========================================================================
// GmDecoInitModel
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitModel( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitModelMotion
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitModelMotion( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitModelNodeMotion
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitModelMotionTouch( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitModelMaterial
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitModelMaterial( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitModelMotionMaterial
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitModelMotionMaterial( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitModelMotionMaterialTouch
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitModelMotionMaterialTouch( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitModelLoop
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitModelLoop( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitModelEffect
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitModelEffect( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitPrimitive3D
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitPrimitive3D( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitFall
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitFall( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitEffect
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitEffect( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitEffectBlock
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitEffectBlock( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );

// ==========================================================================
// GmDecoInitEffectBlockAndNext
/*!
 * ‘•üì¬
 *
 * @param dec_rec ƒCƒxƒ“ƒgƒŒƒR[ƒh
 * @param x À•WX
 * @param y À•WY
 * @param type ƒ^ƒCƒv
 *
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmDecoInitEffectBlockAndNext( GMS_EVE_RECORD_DECORATE* dec_rec, fx32 x, fx32 y, u8 type );




// ==========================================================================
// GmDecoSetFrameMotion
/*!
 * ‹¤’Êƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ğİ’è
 *
 * @param frame ƒtƒŒ[ƒ€
 * @param index ƒtƒŒ[ƒ€ƒCƒ“ƒfƒNƒX
 *
 */
// ==========================================================================
extern void GmDecoSetFrameMotion( s32 frame, s32 index );

// ==========================================================================
// GmDecoStartEffectFinalBossLight
/*!
 * ƒtƒ@ƒCƒiƒ‹ƒ{ƒX—pƒ‰ƒCƒgŠJn
 */
// ==========================================================================
extern void GmDecoStartEffectFinalBossLight( void );


// ==========================================================================
// GmDecoGetFallBackRenderTarget
/*!
 * ‰œ‚Ì‘ê—p‚ÌƒŒƒ“ƒ_[ƒ^[ƒQƒbƒg‚ğæ“¾
 *
 *	è‘O‚Ì‘ê‚ª¶¬Ï‚İ‚È‚çè‘O‚ÌƒeƒNƒXƒ`ƒƒ‚ğA
 *	è‘O‚Ì‘ê‚ª‚È‚­AŒã‚ë‚Ì‘ê‚ª¶¬Ï‚İ‚È‚çŒã‚ë‚ÌƒeƒNƒXƒ`ƒƒA
 *	‚Ç‚¿‚ç‚à‚È‚¢‚È‚çAè‘O‚ÌƒeƒNƒXƒ`ƒƒ‚ğ¶¬‚µ‚Ä•Ô‚·B
 *
 *	•`‰æƒXƒŒƒbƒh‚ÅŒÄ‚ñ‚Å‚­‚¾‚³‚¢B
 *
 * @return ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒg
 *
 */
// ==========================================================================
extern AMS_RENDER_TARGET* GmDecoGetFallRenderTarget( void );

// =======================================================================
// GmDecoSetLightFinalZone
/*!
 * ‘•ü—pƒ‰ƒCƒg
 */
// =======================================================================
extern void GmDecoSetLightFinalZone(void);

// =======================================================================
// GmDecoSetFlagLoop
/*!
 * ƒ‹[ƒvæ‚Ìƒ‚ƒfƒ‹‚Éƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ğŒp³‚·‚éİ’è
 */
// =======================================================================
extern void GmDecoSetLoopState( void );

// =======================================================================
// GmDecoClearLoopState
/*!
 * ƒ‹[ƒvæ‚Ìƒ‚ƒfƒ‹‚Éƒ‚[ƒVƒ‡ƒ“ƒtƒŒ[ƒ€‚ğŒp³‚·‚éİ’è‚ğƒNƒŠƒA
 */
// =======================================================================
extern void GmDecoClearLoopState( void );

// =======================================================================
// GmDecoStartLoop
/*!
 * ƒ‹[ƒvŠJn
 */
// =======================================================================
extern void GmDecoStartLoop( void );

// =======================================================================
// GmDecoSetFlagLoop
/*!
 * ƒ‹[ƒvI—¹
 */
// =======================================================================
extern void GmDecoEndLoop( void );


#if	defined(__cplusplus)
} /* extern "C" */
#endif




#endif // GM_DECO_

//----- Include Files -------------------------------------------------------
