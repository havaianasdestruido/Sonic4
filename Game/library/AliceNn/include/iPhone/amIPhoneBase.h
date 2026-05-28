//
//  amIPhoneBase.h
//  HOGViewer
//
//  Created by use1227 on 09/07/19.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#ifndef _AM_IPHONE_BASE_H
#define _AM_IPHONE_BASE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus*/
		
/*--- Include Files (Pre Definitions) ---------------------------------------*/
#include "NN.h"
	
/*--- Definitions -----------------------------------------------------------*/
typedef enum {
	AME_IPHONE_DISPLAY_ORIENTATION_NORMAL = 0, //!< noamal
	AME_IPHONE_DISPLAY_ORIENTATION_RIGHT, //!< RIGHT orient
	AME_IPHONE_DISPLAY_ORIENTATION_HORIZON, //!< HORIZONE orient, no use
	AME_IPHONE_DISPLAY_ORIENTATION_LEFT, //!< LEFT orient, no use
	AME_IPHONE_DISPLAY_ORIENTATION_NUM
} AME_IPHONE_DISPLAY_ORIENTATION;

	typedef enum {
		AME_IPHONE_TP_TOUCH_OFF = 0,
		AME_IPHONE_TP_TOUCH_ON,
		AME_IPHONE_TP_TOUCH_NUM
	} AME_IPHONE_TP_TOUCH;
	
	typedef enum {
		AME_IPHONE_TP_VALIDITY_INVALID = 0,
		AME_IPHONE_TP_VALIDITY_VALID,
		AME_IPHONE_TP_VALIDITY_NUM
	} AME_IPHONE_TP_VALIDITY;
	
	typedef struct _AMS_IPHONE_TP_DATA {
		Uint16 touch; //!< on or off
		Uint16 validity; //!< valid or invalid
		Uint16 x; //!< point x
		Uint16 y; //!< point y
	} AMS_IPHONE_TP_DATA;
	
	typedef struct _AMS_IPHONE_ACCEL_DATA {
		NNS_VECTOR core; // direct infomation
		NNS_VECTOR sensor; // infomation for display
		Angle32 rot_x; // rotate infomation x
		Angle32 rot_y; // rotate infomation y
		Angle32 rot_z; // rotate infomation z
	} AMS_IPHONE_ACCEL_DATA;
	
	
/*--- Macros ----------------------------------------------------------------*/
	
/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/
extern AMS_IPHONE_ACCEL_DATA _am_iphone_accel_data;

/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* void amIPhoneInitBase(int Width, int Height, const char* pDocPath)      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  Obj-Cƒx[ƒX‰Šú‰»                                              */
/*****************************************************************************/
extern void amIPhoneInitBase(int* Width, int* Height);
	
	/*****************************************************************************/
	/* void amIPhoneRequestTouch(AMS_IPHONE_TP_DATA *DispData, int TouchIndex) */
	/*---------------------------------------------------------------------------*/
	/* [FUNCTION] touch request                                              */
	/*****************************************************************************/	
	extern void amIPhoneRequestTouch(AMS_IPHONE_TP_DATA *DispData, int TouchIndex);
	
	/*****************************************************************************/
	/* void amIPhoneAccelerate(NNS_VECTOR* accel)                                     */
	/*---------------------------------------------------------------------------*/
	/* [FUNCTION] regist accel event                                           */
	/*****************************************************************************/	
	extern void amIPhoneAccelerate(NNS_VECTOR * accel);
	
	/*****************************************************************************/
	/* void amIPhoneTouchBegan(void *touches, void* event, void* view)      */
	/*---------------------------------------------------------------------------*/
	/* [FUNCTION] “ü—ÍŠJŽn“o˜^                                              */
	/*****************************************************************************/
	extern void amIPhoneTouchBegan(void *touches, void* event, void* view);
	
	/*****************************************************************************/
	/* void amIPhoneTouchMoved(void *touches, void* event, void* view)      */
	/*---------------------------------------------------------------------------*/
	/* [FUNCTION] ˆÚ“®“o˜^                                              */
	/*****************************************************************************/
	extern void amIPhoneTouchMoved(void *touches, void* event, void* view);
	
	/*****************************************************************************/
	/* void amIPhoneTouchCanceled(void *touches, void* event, void* view)      */
	/*---------------------------------------------------------------------------*/
	/* [FUNCTION] ƒLƒƒƒ“ƒZƒ‹“o˜^                                              */
	/*****************************************************************************/
	extern void amIPhoneTouchCanceled(void *touches, void* event, void* view);
	
	/*****************************************************************************/
	/* void amIPhoneTouchEnded(void *touches, void* event, void* view)      */
	/*---------------------------------------------------------------------------*/
	/* [FUNCTION] I—¹“o˜^                                              */
	/*****************************************************************************/
	extern void amIPhoneTouchEnded(void *touches, void* event, void* view);
	
	
#ifdef __cplusplus
};
#endif /* __cplusplus*/




#endif // _AM_IPHONE_BASE_H
