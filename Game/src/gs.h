// ==========================================================================
/*!
  @file gs.h
  @brief ゲームシステム

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gs.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GS_H_
#define GS_H_



//----- Include Files -------------------------------------------------------
#include <alice.h>
#include "typedef.h"
#include "fx.h"
#include "mi.h"
#include "mt.h"
#include "syEvtSys.h"
#include "gsEnvironment.h"
#include "gsMemFile.h"
#include "gsFont.h"
#include "gsTrial.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
// 拡張子 設定
#if _PC
	#define NND_MID			"Z"
#elif _XBOX
	#define NND_MID			"E"
#elif _PS3
	#define NND_MID			"C"
#elif _WII
	#define NND_MID			"G"
#elif _IPHONE
	#define NND_MID			"I"
#else
#endif

#define GSS_OBJECT_EXT		NND_MID "NO"		//!< オブジェクトファイル拡張子
#define GSS_TEXTURE_EXT		"AMB"				//!< テクスチャファイル拡張子
#define GSS_MOTION_EXT		NND_MID "NM"		//!< モーションファイル拡張子
#define GSS_MOTIONS_EXT		"AMB"				//!< 複数モーションファイル拡張子

// ベースパス
#if _PC
	#define GSS_BASE_PATH	""
#elif _XBOX
	#define GSS_BASE_PATH	""
#elif _PS3
	#define GSS_BASE_PATH	""
#elif _WII
	#define GSS_BASE_PATH	""
#elif _IPHONE
	#define GSS_BASE_PATH	"Sonic4/Target_iPhone/"
#else
#endif


#if 0
/* シェーダープロファイル */
// objObject.h へ引越し
#if (_WII || _IPHONE)
#define GSD_SHADER_USER_PROFILE_ID_TOON			(0)			//!< トゥーン
#define NND_DRAWOBJ_SHADER_USER_PROFILE_TOON	(0)

#else
#define GSD_SHADER_USER_PROFILE_ID_TOON			(1)			//!< トゥーン
#define NND_DRAWOBJ_SHADER_USER_PROFILE_TOON	(NND_DRAWOBJ_SHADER_USER_PROFILE(GSD_SHADER_USER_PROFILE_ID_TOON))

#endif
#endif

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

#if	defined(__cplusplus)
} /* extern "C" */
#endif


#endif // GS_H_

//----- Include Files -------------------------------------------------------
