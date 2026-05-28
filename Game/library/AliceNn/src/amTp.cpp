// ================================================================
/*!
 @file amTp.c
 @brief TouchPanel coutrol
 
 @author Satoshi Akitomi
 Copyright(c) 2009 Dimps
 $Id: amTp.cpp 8 2011-04-12 01:27:21Z thamada $
 */
// ================================================================


//----- Include Files --------------------------------------------------
#include "alice.h"


//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------
// ================================================================
/*!
 @brief set bit, target Uint16
 
 @param val   [out]   target(Uint16)
 @param shift [in]    shift value
 @param bit   [in]    bit value(0/1)
 */
// ================================================================
#define AMM_TP_BIT_SET16(val, shift, bit)               \
({                                                      \
val = (Uint16)(((val) & ~(1 << (shift)))               \
| ((bit) << (shift)));                    \
})


//----- Definitions ----------------------------------------------------
//----- External Declarations ------------------------------------------
//----- Static Declarations --------------------------------------------
static void _amTpUpdateTouch_req(void);

//----- Global Variables -----------------------------------------------
/// @brief TouchPanel state
/// @note  auto update
AMS_TP_TOUCH_STATUS   _am_tp_touch[AMD_TP_TOUCH_POS_MAX];

//----- Local Variables ------------------------------------------------

//----- Global Functions -----------------------------------------------
// ================================================================
// amTpInit
/*!
 TouchPanel Initialized
 
 @note
 Call once before use. \n
 You don't have to call this method because this is called by system.
 */
// ================================================================
void amTpInit(void)
{
    // init
	memset((void*)_am_tp_touch, 0, sizeof(AMS_TP_TOUCH_STATUS) * AMD_TP_TOUCH_POS_MAX);
}

// ================================================================
// amTpExecute
/*!
 TouchPanel Execution
 
 @note
 Update point, etc...
 */
// ================================================================
void amTpExecute(void)
{
	int i;
	
	// can't use auto sampling
	_amTpUpdateTouch_req();
	
	for (i = 0; i < AMD_TP_TOUCH_POS_MAX; i++) {
		// push/pull
		amTpUpdateStatus(&_am_tp_touch[i], &_am_tp_touch[i].core);
	}
}

// ================================================================
// amTpUpdateStatus
/*!
 TouchPanel state update
 
 @param core   [in]  core info
 @param status [out] status info
 
 @note
 Update soume params.
 Touch 1 pos, 1 status & 1 core.
 */
// ================================================================
extern void amTpUpdateStatus(AMS_TP_TOUCH_STATUS *status, AMS_TP_TOUCH_CORE *core)
{
    // update flag
    {
        BOOL    bit_on      = (core->sampling_flag & AMD_TP_SAMPLING_FLAG_ON) ? TRUE : FALSE;
        BOOL    bit_prev    = (status->flag & AMD_TP_FLAG_ON) >> AMD_TP_FLAG_ON_SHIFT;
        BOOL    bit_on_prev = bit_on ^ bit_prev;
		
        AMM_TP_BIT_SET16(status->flag, AMD_TP_FLAG_INVALID_SHIFT, (core->sampling_flag & AMD_TP_SAMPLING_FLAG_INVALID) ? TRUE : FALSE);
        AMM_TP_BIT_SET16(status->flag, AMD_TP_FLAG_PREV_SHIFT, bit_prev);
        AMM_TP_BIT_SET16(status->flag, AMD_TP_FLAG_ON_SHIFT,   bit_on);
        AMM_TP_BIT_SET16(status->flag, AMD_TP_FLAG_PUSH_SHIFT, bit_on_prev & bit_on);
        AMM_TP_BIT_SET16(status->flag, AMD_TP_FLAG_PULL_SHIFT, bit_on_prev & bit_prev);
    }
	
    // update TouchPanel info
    {
        // update before 1f
        status->prev[AMD_X] = status->on[AMD_X];
        status->prev[AMD_Y] = status->on[AMD_Y];
		
        // now point
        {
			Uint16* core_buf = core->sampling_buf;
			Uint16 latest_buf[AMD_XY];
#if 0
			// makeshift
			if (AMM_IPHONE_IS_DISPLAY_ROT90())
			{
				latest_buf[AMD_Y] = core_buf[AMD_X];
				latest_buf[AMD_X] = 480 - core_buf[AMD_Y];
			}
			else if (AMM_IPHONE_IS_DISPLAY_ROT180())
			{
				latest_buf[AMD_X] = 320 - core_buf[AMD_X];
				latest_buf[AMD_Y] = 480 - core_buf[AMD_Y];
			}
			else if (AMM_IPHONE_IS_DISPLAY_ROT270())
			{
				latest_buf[AMD_X] = core_buf[AMD_Y];
				latest_buf[AMD_Y] = 320 - core_buf[AMD_X];
			}
			else
#endif // CGAffineTransform
			{
				latest_buf[AMD_X] = core_buf[AMD_X];
				latest_buf[AMD_Y] = core_buf[AMD_Y];
			}
            status->on[AMD_X]   = latest_buf[AMD_X];
            status->on[AMD_Y]   = latest_buf[AMD_Y];
        }
		
        // if pull or push, set value.
        if (status->flag & AMD_TP_FLAG_PUSH) {
            status->push[AMD_X] = status->on[AMD_X];
            status->push[AMD_Y] = status->on[AMD_Y];
			
        } else if (status->flag & AMD_TP_FLAG_PULL) {
            status->pull[AMD_X] = status->prev[AMD_X];
            status->pull[AMD_Y] = status->prev[AMD_Y];
        }
		
        // update core
        if (&status->core != core) {
            status->core    = *core;
        }
 }
}

//----- Local Functions ------------------------------------------------
// ================================================================
// _amTpUpdateTouch_req
/*!
 Update TouchPanel, request sampling
 */
// ================================================================
static void _amTpUpdateTouch_req(void)
{
	int i;
    AMS_TP_TOUCH_CORE*   core;
	AMS_IPHONE_TP_DATA DispData;
    
	for (i = 0; i< AMD_TP_TOUCH_POS_MAX; i++) {
		core = &_am_tp_touch[i].core;   // core info
		
		// core update
		amIPhoneRequestTouch(&DispData, i);
		// get from iPhone only parameter.
		// --------------------------------------------------
		if (DispData.touch == AME_IPHONE_TP_TOUCH_ON) {
			// touch
			if (DispData.validity == AME_IPHONE_TP_VALIDITY_VALID) {
				// enable xy point
				core->sampling_buf[AMD_X]    = DispData.x;
				core->sampling_buf[AMD_Y]    = DispData.y;
				core->sampling_flag |= AMD_TP_SAMPLING_FLAG_ON;
				core->sampling_flag &= ~AMD_TP_SAMPLING_FLAG_INVALID;
			} else {
				// unable xy point, can't update
				core->sampling_flag |= AMD_TP_SAMPLING_FLAG_INVALID;
			}
		} else {
			// no touch
			core->sampling_flag &= ~(AMD_TP_SAMPLING_FLAG_ON | AMD_TP_SAMPLING_FLAG_INVALID);
		}
	}
}

