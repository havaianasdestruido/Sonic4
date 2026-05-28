/*****************************************************************************/
/*      amIPhoneBase.m                   Author : Satoshi Akitomi            */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* iPhone関連ライブラリプログラム Obj-C対応                                       */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090719-       0.01   first version                                        */
/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus*/

/*--- Include Files ---------------------------------------------------------*/
#import <UIKit/UIKit.h>
#include "amIPhoneBase.h"
	
/*--- Macros ----------------------------------------------------------------*/
#define AMD_IPHONE_TOUCH_POS_MAX  (5)

/*--- Definitions -----------------------------------------------------------*/
	typedef struct _AMS_IPHONE_TP_CTRL_DATA AMS_IPHONE_TP_CTRL_DATA;
	
	struct _AMS_IPHONE_TP_CTRL_DATA {
		AMS_IPHONE_TP_DATA tpdata;
		Uint32 addr;
	};

/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/
	AMS_IPHONE_ACCEL_DATA _am_iphone_accel_data;

/*--- Local Variables -------------------------------------------------------*/
	static AMS_IPHONE_TP_CTRL_DATA _am_iphone_tp_ctrl_data[AMD_IPHONE_TOUCH_POS_MAX];
	static AME_IPHONE_DISPLAY_ORIENTATION _am_iphone_display_orientation = AME_IPHONE_DISPLAY_ORIENTATION_NORMAL;
	static AME_IPHONE_DISPLAY_ORIENTATION _am_iphone_screen_orientation = AME_IPHONE_DISPLAY_ORIENTATION_NORMAL;
	
/*--- Local Functions -------------------------------------------------------*/

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* void amIPhoneInitBase(int* Width, int* Height, const char* pDocPath)      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  Obj-Cベース初期化                                              */
/*****************************************************************************/
void amIPhoneInitBase(int* Width, int* Height)
{	
#if (1)
	// ローテーション
	// 設定が合わなければ縦長のまま
	UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
	if (*Width == 480) {
		_am_iphone_screen_orientation = AME_IPHONE_DISPLAY_ORIENTATION_RIGHT;
	}
	
	// 90 or 270
	if (orientation == UIInterfaceOrientationLandscapeLeft || orientation == UIInterfaceOrientationLandscapeRight) {
		_am_iphone_display_orientation = AME_IPHONE_DISPLAY_ORIENTATION_RIGHT;
		// 縦画面モード
		if (*Width == 320) {
			*Height ^= *Width;
			*Width ^= *Height;
			*Height ^= *Width;
			nnSetPrintOrientationMode(NNE_POM_HORIZON_RIGHT);
		}
	}
	/*
	// 270 ただし幅だけでは角度の区別がつかないので、今は90と同じ扱いとする。
	else if (orientation == UIInterfaceOrientationLandscapeLeft) {
		_am_iphone_display_orientation = AME_IPHONE_DISPLAY_ORIENTATION_LEFT;
		nnSetPrintOrientationMode(NNE_POM_HORIZON_LEFT);
	}
	 */
	// 0 or 180
	else {
		_am_iphone_display_orientation = AME_IPHONE_DISPLAY_ORIENTATION_NORMAL;
		// 横画面モード
		if (*Width == 480) {
			*Height ^= *Width;
			*Width ^= *Height;
			*Height ^= *Width;
			nnSetPrintOrientationMode(NNE_POM_HORIZON_LEFT); // 90を元に戻すために回転
		}
	}
#endif // CGAffineTransform
	
	// touch init
	memset((void*)_am_iphone_tp_ctrl_data, 0, sizeof(AMS_IPHONE_TP_CTRL_DATA) * AMD_IPHONE_TOUCH_POS_MAX);
	
	// accel init
	memset((void*)&_am_iphone_accel_data, 0, sizeof(_AMS_IPHONE_ACCEL_DATA));
}
	
	/*****************************************************************************/
	/* int amIPhoneIsDisplayOrientation(AME_IPHONE_DISPLAY_ORIENTATION orientation)      */
	/*---------------------------------------------------------------------------*/
	/* [FUNCTION]  Obj-Cベース初期化                                              */
	/*****************************************************************************/
	int amIPhoneIsDisplayOrientation(AME_IPHONE_DISPLAY_ORIENTATION orientation)
	{
		return (orientation == _am_iphone_display_orientation);
	}
	
	/*****************************************************************************/
	/* void amIPhoneAccelerate(NNS_VECTOR* accel)                                     */
	/*---------------------------------------------------------------------------*/
	/* [FUNCTION] regist accel event                                           */
	/*****************************************************************************/	
	void amIPhoneAccelerate(NNS_VECTOR * accel)
	{
		NNS_VECTOR* core   = &_am_iphone_accel_data.core;
		NNS_VECTOR* sensor = &_am_iphone_accel_data.sensor;
		
		// copy original data
		{
			memcpy((void*)core, (void*)accel, sizeof(NNS_VECTOR));
		}
		// create for display data
		{
			// makeshift
#if (1)
			if (_am_iphone_display_orientation == AME_IPHONE_DISPLAY_ORIENTATION_RIGHT)
			{
				sensor->x = -core->y;
				sensor->y = core->x;
				sensor->z = core->z;
			}
			else if (_am_iphone_display_orientation == AME_IPHONE_DISPLAY_ORIENTATION_HORIZON)
			{
				sensor->x = core->x;
				sensor->y = -core->y;
				sensor->z = core->z;
			}
			else if (_am_iphone_display_orientation == AME_IPHONE_DISPLAY_ORIENTATION_LEFT)
			{
				sensor->x = core->y;
				sensor->y = core->x;
				sensor->z = core->z;
			}
			else
			{
				memcpy((void*)sensor, (void*)core, sizeof(NNS_VECTOR));
			}
#else
			sensor->x = -core->y;
			sensor->y = core->x;
			sensor->z = core->z;
#endif // CGAffineTransform
		}
		
		// create rotate infomation
		{
			// from amPad for Wii
			_am_iphone_accel_data.rot_x = nnArcTan2(-sensor->z, -sensor->y);
			_am_iphone_accel_data.rot_z = nnArcTan2( sensor->x, -sensor->y);
		}
		
	}
	
	/*****************************************************************************/
	/* void amIPhoneRequestTouch(AMS_IPHONE_TP_DATA *DispData, int TouchIndex) */
	/*---------------------------------------------------------------------------*/
	/* [FUNCTION] touch request                                              */
	/*****************************************************************************/	
	extern void amIPhoneRequestTouch(AMS_IPHONE_TP_DATA* DispData, int TouchIndex)
	{
		if (DispData != NULL) {
			memcpy((void*)DispData, (void*)&_am_iphone_tp_ctrl_data[TouchIndex].tpdata, sizeof(AMS_IPHONE_TP_DATA));
		}
	}

/*****************************************************************************/
/* void amIPhoneTouchBegan(void *touches, void* event, void* view)      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION] 入力開始登録                                              */
/*****************************************************************************/
void amIPhoneTouchBegan(void *touches, void* event, void* view)
{
	NSSet* tmp_touches = (NSSet*)touches;
	UIEvent* tmp_event = (UIEvent*)event;
	UIView* tmp_view = (UIView*)view;
	
	CGPoint point;
//	CGPoint prev_point;
	int i;

	for (UITouch* touch in tmp_touches) {
		point = [touch locationInView:tmp_view];
//		prev_point = [touch previousLocationInView:tmp_view];
		for (i = 0; i < AMD_IPHONE_TOUCH_POS_MAX; i++) {
			// case 0
			if (_am_iphone_tp_ctrl_data[i].addr == 0) {
				AMS_IPHONE_TP_DATA* tpdata = &(_am_iphone_tp_ctrl_data[i].tpdata);
#if (0)
				tpdata->x = (Uint16)point.x;
				tpdata->y = (Uint16)point.y;
#else
				// OS3.X / 2.X用対応
				if (point.y < 0.0f) {
					point.y += 320.0f;
				}
				
				// orientation check
				if (_am_iphone_display_orientation == _am_iphone_screen_orientation) {
					tpdata->x = (Uint16)point.x;
					tpdata->y = (Uint16)point.y;
				}
				else {
					if (_am_iphone_display_orientation == AME_IPHONE_DISPLAY_ORIENTATION_RIGHT) {
						tpdata->x = (Uint16)point.y;
						tpdata->y = 320 - (Uint16)point.x;
					}
					else if (_am_iphone_screen_orientation == AME_IPHONE_DISPLAY_ORIENTATION_RIGHT) {
						tpdata->x = 320 - (Uint16)point.y;
						tpdata->y = (Uint16)point.x;
					}
				}
#endif // CGAffineTransform
				tpdata->touch = AME_IPHONE_TP_TOUCH_ON;
				tpdata->validity = AME_IPHONE_TP_VALIDITY_VALID;
				_am_iphone_tp_ctrl_data[i].addr = (Uint32)touch;
				// end
				break;
			}
		}
	}
}
	
/*****************************************************************************/
/* void amIPhoneTouchMoved(void *touches, void* event, void* view)     */
/*---------------------------------------------------------------------------*/
/* [FUNCTION] 移動登録                                              */
/*****************************************************************************/
void amIPhoneTouchMoved(void *touches, void* event, void* view)
{
	NSSet* tmp_touches = (NSSet*)touches;
	UIEvent* tmp_event = (UIEvent*)event;
	UIView* tmp_view = (UIView*)view;
	
	CGPoint point;
	//	CGPoint prev_point;
	int i;
	
	for (UITouch* touch in tmp_touches) {
		point = [touch locationInView:tmp_view];
		//		prev_point = [touch previousLocationInView:tmp_view];
		for (i = 0; i < AMD_IPHONE_TOUCH_POS_MAX; i++) {
			// compare address
			if (_am_iphone_tp_ctrl_data[i].addr == (Uint32)touch) {
				AMS_IPHONE_TP_DATA* tpdata = &(_am_iphone_tp_ctrl_data[i].tpdata);
#if (0)
				tpdata->x = (Uint16)point.x;
				tpdata->y = (Uint16)point.y;
#else
				// OS3.X / 2.X用対応
				if (point.y < 0.0f) {
					point.y += 320.0f;
				}
				// orientation check
				if (_am_iphone_display_orientation == _am_iphone_screen_orientation) {
					tpdata->x = (Uint16)point.x;
					tpdata->y = (Uint16)point.y;
				}
				else {
					if (_am_iphone_display_orientation == AME_IPHONE_DISPLAY_ORIENTATION_RIGHT) {
						tpdata->x = (Uint16)point.y;
						tpdata->y = 320 - (Uint16)point.x;
					}
					else if (_am_iphone_screen_orientation == AME_IPHONE_DISPLAY_ORIENTATION_RIGHT) {
						tpdata->x = 320 - (Uint16)point.y;
						tpdata->y = (Uint16)point.x;
					}
				}
#endif // CGAffineTransform
				tpdata->touch = AME_IPHONE_TP_TOUCH_ON;
				tpdata->validity = AME_IPHONE_TP_VALIDITY_VALID;
			}
		}
	}	
}
	
/*****************************************************************************/
/* void amIPhoneTouchCanceled(NSSet *touches, UIEvent* event, void* view)      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION] キャンセル登録                                              */
/*****************************************************************************/
void amIPhoneTouchCanceled(void *touches, void* event, void* view)
{
	int i;
	// all clear
	for (i = 0; i < AMD_IPHONE_TOUCH_POS_MAX; i++) {
		_am_iphone_tp_ctrl_data[i].addr = 0;
		_am_iphone_tp_ctrl_data[i].tpdata.touch = AME_IPHONE_TP_TOUCH_OFF;
		_am_iphone_tp_ctrl_data[i].tpdata.validity = AME_IPHONE_TP_VALIDITY_INVALID;
	}
}
	
/*****************************************************************************/
/* void amIPhoneTouchEnded(NSSet *touches, UIEvent* event, void* view)      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION] 終了登録                                              */
/*****************************************************************************/
void amIPhoneTouchEnded(void *touches, void* event, void* view)
{
	NSSet* tmp_touches = (NSSet*)touches;
	UIEvent* tmp_event = (UIEvent*)event;
	UIView* tmp_view = (UIView*)view;
	
	CGPoint point;
	//	CGPoint prev_point;
	int i;
	int flag[AMD_IPHONE_TOUCH_POS_MAX] = {0};
	
	for (UITouch* touch in tmp_touches) {
		point = [touch locationInView:tmp_view];
		//		prev_point = [touch previousLocationInView:tmp_view];
		for (i = 0; i < AMD_IPHONE_TOUCH_POS_MAX; i++) {
			// compare address
			if (_am_iphone_tp_ctrl_data[i].addr == (Uint32)touch) {
				flag[i] = TRUE;
			}
		}
	}
	// check addr and release touch
	for (i = 0; i < AMD_IPHONE_TOUCH_POS_MAX; i++) {
		if (flag[i]) {
			_am_iphone_tp_ctrl_data[i].addr = 0;
			_am_iphone_tp_ctrl_data[i].tpdata.touch = AME_IPHONE_TP_TOUCH_OFF;
		}
	}
}

/*--- Local Functions -------------------------------------------------------*/
	
#ifdef __cplusplus
};
#endif /* __cplusplus*/




