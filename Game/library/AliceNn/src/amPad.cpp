/*****************************************************************************/
/*      amPad.cpp                   Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* パッドライブラリプログラム                                                */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090330-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"

/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

#if _WII
extern "C"
{
void *_amPadMemAlloc(Uint32 size);
Uint8 _amPadMemFree(void *ptr);
};
#endif


/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

AMS_PAD_DATA	_am_pad[AMD_PAD_PORT_MAX];

#if _WII
Sint32			_am_pad_num;
AMS_MUTEX		_am_pad_mutex;
MEMAllocator	_am_pad_allocator;

Uint16			_am_pad_gc_mapping[16] = {
	KEY_L_LEFT,			// 十字キー ←
	KEY_L_RIGHT,		// 十字キー →
	KEY_L_DOWN,			// 十字キー ↓
	KEY_L_UP,			// 十字キー ↑
	KEY_R1,				// Z
	KEY_R2,				// R
	KEY_L2,				// L
	0,
	KEY_R_DOWN,			// A (Xbox360の同名ボタンに対応)
	KEY_R_RIGHT,		// B
	KEY_R_LEFT,			// X
	KEY_R_UP,			// Y
	0, 0, 0,
	KEY_START,			// START
};
#endif

#if _WII | _PC
Uint16			_am_pad_mapping[2][16] = {
	{		// 片手持ち
		KEY_L_LEFT,			// 十字キー ←
		KEY_L_RIGHT,		// 十字キー →
		KEY_L_DOWN,			// 十字キー ↓
		KEY_L_UP,			// 十字キー ↑
		KEY_START,			// ＋
		0, 0, 0,
		KEY_R_RIGHT,		// 2
		KEY_R_DOWN,			// 1
		KEY_R_UP,			// B
		KEY_R_LEFT,			// A
		KEY_SELECT,			// -
		KEY_R2,				// Z (ヌンチャク)
		KEY_R1,				// C (ヌンチャク)
		0,
	}, {	// 両手持ち
		KEY_L_DOWN,			// 十字キー ←
		KEY_L_UP,			// 十字キー →
		KEY_L_RIGHT,		// 十字キー ↓
		KEY_L_LEFT,			// 十字キー ↑
		KEY_START,			// ＋
		0, 0, 0,
		KEY_R_RIGHT,		// 2
		KEY_R_DOWN,			// 1
		KEY_R_UP,			// B
		KEY_R_LEFT,			// A
		KEY_SELECT,			// -
		KEY_R2,				// Z (ヌンチャク)
		KEY_R1,				// C (ヌンチャク)
		0,
	}
};
#endif


/*--- Local Variables -------------------------------------------------------*/

/*--- Local Declarations ----------------------------------------------------*/

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* void amPadInit(Sint32 pad_num, Uint32 format)                             */
/*---------------------------------------------------------------------------*/
/* [INPUT]  pad_num : 対応パッド数(Wiiのみ有効)                              */
/*          format  : 対応デバイスフォーマット(Wiiのみ有効)                  */
/* [FUNCTION]  パッドライブラリの初期化                                      */
/*****************************************************************************/
void amPadInit(Sint32 pad_num, Uint32 format)
{
	AMS_PAD_DATA	*pad;
	Sint32		i;

	// データの初期化
	pad		= &_am_pad[0];
	for (i = 0; i < AMD_PAD_PORT_MAX; i++, pad++) {
		memset(pad, 0, sizeof(AMS_PAD_DATA));
		pad->vib_flag	= 1;
	}

#if _PC
	UNREFERENCED_PARAMETER(format);
	pad		= &_am_pad[0];
	for (i = 0; i < AMD_PAD_PORT_MAX; i++, pad++) {
		pad->mapping	= _am_pad_mapping[0];
		
	}
#elif _XBOX
	UNREFERENCED_PARAMETER(format);
#elif _PS3
	UNREFERENCED_PARAMETER(format);
	cellPadInit(AMD_PAD_PORT_MAX);
	for (i = 0; i < AMD_PAD_PORT_MAX; i++)
		cellPadSetPortSetting(i,
				CELL_PAD_SETTING_PRESS_ON | CELL_PAD_SETTING_SENSOR_ON);
#elif _WII
	_am_pad_num		= pad_num;
	pad		= &_am_pad[0];
	for (i = 0; i < AMD_PAD_PORT_MAX; i++, pad++) {
		if (i < pad_num) {
			WPADControlDpd(i, WPAD_DPD_STD, NULL);
			WPADSetDataFormat(i, format);
			if (format == AMD_PAD_MODE_ACC_DPD)
				KPADEnableDPD(i);
			else
				KPADDisableDPD(i);
		}
		pad->mode		= format;
		pad->mapping	= _am_pad_mapping[0];
		pad->gc_mapping	= _am_pad_gc_mapping;
	}

	amMutexCreate(&_am_pad_mutex);
#endif
}


/*****************************************************************************/
/* void amPadExit(void)                                                      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  パッドライブラリの終了                                        */
/*****************************************************************************/
void amPadExit(void)
{
#if _PC
#elif _XBOX
#elif _PS3
	cellPadEnd();
#elif _WII
	amMutexDelete(&_am_pad_mutex);
#endif
}


/*****************************************************************************/
/* void amPadSetDeviceFormat(Sint32 port, Uint32 format)                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port   : パッドポートID(-1ならば全ポート)                        */
/*          format : 対応デバイスフォーマット                                */
/* [FUNCTION]  デバイスフォーマットの設定(Wiiのみ有効)                       */
/*****************************************************************************/
void amPadSetDeviceFormat(Sint32 port, Uint32 format)
{
#if _WII
	Sint32		p1, dfmt;
	Uint32		devtype;

	if (port != -1) {
		p1		= port;
	} else {
		port	= 0;
//		p1		= AMD_PAD_PORT_MAX - 1;
		p1		= _am_pad_num - 1;
	}

	for (; port <= p1; port++) {
#if 0
#if AMD_PAD_MODE_AUTO
		if (WPADProbe(port, &devtype) != WPAD_ERR_NONE)
			devtype		= WPAD_DEV_CORE;
		switch (devtype) {
			case	WPAD_DEV_CORE:
				dfmt	= WPAD_FMT_CORE;
				break;
			case	WPAD_DEV_FREESTYLE:
				dfmt	= WPAD_FMT_FREESTYLE;
				break;
			case	WPAD_DEV_CLASSIC:
				dfmt	= WPAD_FMT_CLASSIC;
				break;
			default:
				dfmt	= WPAD_FMT_CORE;
				break;
		}
		dfmt	+= format;
#else
		dfmt	= format;
#endif

		if ((format ^ _am_pad[port].mode) & AMD_PAD_MODE_ACC_DPD) {
			WPADControlDpd(port, (format == AMD_PAD_MODE_ACC_DPD)?
					WPAD_DPD_STD: WPAD_DPD_OFF, NULL);
		}
		WPADSetDataFormat(port, dfmt);
#else
		if (format == AMD_PAD_MODE_ACC_DPD)
			KPADEnableDPD(port);
		else
			KPADDisableDPD(port);
#endif
		_am_pad[port].mode		= format;
	}
#else
	UNREFERENCED_PARAMETER(port);
	UNREFERENCED_PARAMETER(format);
#endif
}


/*****************************************************************************/
/* void amPadSetMapping(Sint32 port, Uint16 *mapping)                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port    : パッドポートID(-1ならば全ポート)                       */
/*          mapping : ボタンマッピングテーブル                               */
/*                    AMD_PAD_MAPPING_SINGLE_HAND : 片手持ちデフォルト       */
/*                    AMD_PAD_MAPPING_DOUBLE_HAND : 両手持ちデフォルト       */
/* [FUNCTION]  ボタンマッピングテーブルの設定(Wii/PCのみ有効)                */
/*****************************************************************************/
void amPadSetMapping(Sint32 port, Uint16 *mapping)
{
#if _WII | _PC
	if (mapping == AMD_PAD_MAPPING_SINGLE_HAND)
		mapping		= _am_pad_mapping[0];
	else if (mapping == AMD_PAD_MAPPING_DOUBLE_HAND)
		mapping		= _am_pad_mapping[1];

	if (port != -1)
		_am_pad[port].mapping	= mapping;
	else {
		for (port = 0; port < AMD_PAD_PORT_MAX; port++)
			_am_pad[port].mapping	= mapping;
	}
#else
	UNREFERENCED_PARAMETER(port);
	UNREFERENCED_PARAMETER(mapping);
#endif
}


/*****************************************************************************/
/* void amPadSetMappingGC(Sint32 port, Uint16 *mapping)                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port    : パッドポートID(-1ならば全ポート)                       */
/*          mapping : ボタンマッピングテーブル                               */
/*                    NULL : デフォルト                                      */
/* [FUNCTION]  ボタンマッピングテーブルの設定(GCコントローラ用、Wiiのみ有効) */
/*****************************************************************************/
void amPadSetMappingGC(Sint32 port, Uint16 *mapping)
{
#if _WII
	if (mapping == NULL)
		mapping		= _am_pad_gc_mapping;

	if (port != -1)
		_am_pad[port].gc_mapping	= mapping;
	else {
		for (port = 0; port < AMD_PAD_PORT_MAX; port++)
			_am_pad[port].gc_mapping	= mapping;
	}
#else
	UNREFERENCED_PARAMETER(port);
	UNREFERENCED_PARAMETER(mapping);
#endif
}


/*****************************************************************************/
/* void amPadEnableInput(Sint32 port, Sint32 flag)                           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port : パッドポートID(-1ならば全ポート)                          */
/*          flag : 入力を有効にするかどうかのフラグ                          */
/* [FUNCTION]  パッド入力の有効・無効の設定                                  */
/*****************************************************************************/
void amPadEnableInput(Sint32 port, Sint32 flag)
{
	if (port != -1) {
		if (flag)
			_am_pad[port].state		&= ~AMD_PAD_STAT_IGNORE_INPUT;
		else
			_am_pad[port].state		|= AMD_PAD_STAT_IGNORE_INPUT;
	} else {
		if (flag) {
			for (port = 0; port < AMD_PAD_PORT_MAX; port++)
				_am_pad[port].state		&= ~AMD_PAD_STAT_IGNORE_INPUT;
		} else {
			for (port = 0; port < AMD_PAD_PORT_MAX; port++)
				_am_pad[port].state		|= AMD_PAD_STAT_IGNORE_INPUT;
		}
	}
}


/*****************************************************************************/
/* void amPadGetData(void)                                                   */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  パッド情報の取得                                              */
/*****************************************************************************/
void amPadGetData(void)
{
	AMS_PAD_DATA	*pad;
	Sint32		i, j, *count, *acount;
	Uint16		direct, adirect, change, achange;

#if _WII
	BOOL		connect = FALSE;

	amMutexLock(&_am_pad_mutex);

#if AMD_WII_USE_GCPAD
 	PADStatus	gc_stat[PAD_MAX_CONTROLLERS];
	Uint32		padreset = 0;

	PADRead(gc_stat);
#endif
#endif

	pad		= &_am_pad[0];
	for (i = 0; i < AMD_PAD_PORT_MAX; i++, pad++) {
		// データの取得
		direct	= 0;
#if _PC | _XBOX
		XINPUT_STATE	state;

		if (XInputGetState(i, &state) == ERROR_SUCCESS)
			pad->state	|= AMD_PAD_STAT_DEVICE_CONNECT;
		else
			pad->state	&= ~AMD_PAD_STAT_DEVICE_CONNECT;

		if ((pad->state & (AMD_PAD_STAT_DEVICE_CONNECT | AMD_PAD_STAT_IGNORE_INPUT))
				== AMD_PAD_STAT_DEVICE_CONNECT) {
			direct		= state.Gamepad.wButtons & KEYS_MASK;
#if _PC
			direct		|= pad->mdirect;
#endif
			pad->alx	= state.Gamepad.sThumbLX;
			pad->aly	= state.Gamepad.sThumbLY;
			pad->arx	= state.Gamepad.sThumbRX;
			pad->ary	= state.Gamepad.sThumbRY;
			pad->al2	= state.Gamepad.bLeftTrigger;
			pad->ar2	= state.Gamepad.bRightTrigger;
			if (pad->al2 > 128)
				direct	|= KEY_L2;
			if (pad->ar2 > 128)
				direct	|= KEY_R2;
		} else {
			direct		= 0;
			pad->alx	= 0;
			pad->aly	= 0;
			pad->arx	= 0;
			pad->ary	= 0;
			pad->al2	= 0;
			pad->ar2	= 0;
		}
#elif _PS3
		CellPadData		state;
		if (cellPadGetData(i, &state) == CELL_PAD_OK)
			pad->state	|= AMD_PAD_STAT_DEVICE_CONNECT;
		else
			pad->state	&= ~AMD_PAD_STAT_DEVICE_CONNECT;

		if ((pad->state & (AMD_PAD_STAT_DEVICE_CONNECT | AMD_PAD_STAT_IGNORE_INPUT))
				== AMD_PAD_STAT_DEVICE_CONNECT) {
			if (state.len == 0) {
				direct	= pad->direct;
			} else {
				direct	= (state.button[2] & KEYS_MASK1)
						| ((state.button[3] & KEYS_MASK2) << 8);
				pad->alx	= ((Sint16)state.button[6] - 128) << 8;
				pad->aly	= -((Sint16)state.button[7] - 127) << 8;
				pad->arx	= ((Sint16)state.button[4] - 128) << 8;
				pad->ary	= -((Sint16)state.button[5] - 127) << 8;
				pad->al2	= (Sint16)state.button[18];
				pad->ar2	= (Sint16)state.button[19];
				pad->sensor_x	= (Sint16)state.button[20];
				pad->sensor_y	= (Sint16)state.button[21];
				pad->sensor_z	= (Sint16)state.button[22];
				pad->sensor_g	= (Sint16)state.button[23];
				pad->rot_x	= nnArcTan2(
						pad->sensor_z - 0x200,
						0x220 - pad->sensor_y);
				pad->rot_z	= nnArcTan2(
						0x200 - pad->sensor_x,
						0x220 - pad->sensor_y);
			}
		} else {
			direct		= 0;
			pad->alx	= 0;
			pad->aly	= 0;
			pad->arx	= 0;
			pad->ary	= 0;
			pad->al2	= 0;
			pad->ar2	= 0;
			pad->sensor_x	= 0;
			pad->sensor_y	= 0;
			pad->sensor_z	= 0;
			pad->sensor_g	= 0;
			pad->rot_x	= 0;
			pad->rot_z	= 0;
		}
#elif _WII
		Uint32			devtype;
		Sint32			data_num;
		Sint32			errcode;
		KPADStatus		state;
#if AMD_WII_USE_GCPAD
		PADStatus		*gc_state = &gc_stat[i];
#endif

		// 未使用コントローラ
		if (i >= _am_pad_num) {
			Sint32		probe_state;
			probe_state		= WPADProbe(i, &devtype);
			if (probe_state != WPAD_ERR_NO_CONTROLLER)
				WPADDisconnect(i);
			continue;
		}

		data_num	= KPADReadEx(i, &state, 1, &errcode);

		if (data_num) {
			pad->kpad_status	= state;
			pad->pad_stand		= state.trig & WPAD_BUTTON_HOME;
#if AMD_WII_USE_CLPAD
			switch (state.data_format) {
				case	WPAD_FMT_CLASSIC:
				case	WPAD_FMT_CLASSIC_ACC:
				case	WPAD_FMT_CLASSIC_ACC_DPD:
					pad->pad_stand	|= 
							state.ex_status.cl.trig & WPAD_CL_BUTTON_HOME;
					break;
			}
#endif
		}

		Sint32		probe_state;
		probe_state		= WPADProbe(i, &devtype);
		if (probe_state != WPAD_ERR_NO_CONTROLLER)
			pad->state	|= AMD_PAD_STAT_DEVICE_CONNECT;
		else {
			pad->state	&= ~AMD_PAD_STAT_DEVICE_CONNECT;
			connect		= TRUE;
		}

		if (amWiiIsPauseHBM())
			continue;

		if (!amWiiIsDrawHBM() && !(pad->state & AMD_PAD_STAT_IGNORE_INPUT)) {
			if (data_num && (probe_state == WPAD_ERR_NONE)) {
#if AMD_PAD_MODE_AUTO & 0
				Sint32		dfmt;
				switch (devtype) {
					case	WPAD_DEV_CORE:
						dfmt	= WPAD_FMT_CORE;
						break;
					case	WPAD_DEV_FREESTYLE:
						dfmt	= WPAD_FMT_FREESTYLE;
						break;
					case	WPAD_DEV_CLASSIC:
						dfmt	= WPAD_FMT_CLASSIC;
						break;
					default:
						dfmt	= WPAD_FMT_CORE;
						break;
				}
				dfmt	+= pad->mode;
				if (dfmt != WPADGetDataFormat(i))
					WPADSetDataFormat(i, dfmt);
#endif
#if AMD_WII_USE_GCPAD
				if (gc_state->err == PAD_ERR_NO_CONTROLLER) {
#else
				if (1) {
#endif
					// GCコントローラがなければWiiコントローラ
					Uint16	button = state.hold;
					Uint16	*mapping = &pad->mapping[0];
					for (j = 0; j < 16; j++, button >>= 1, mapping++) {
						if (button & 1)
							direct		|= *mapping;
					}
					switch (state.data_format) {
						case	WPAD_FMT_CORE_ACC_DPD:
						case	WPAD_FMT_FREESTYLE_ACC_DPD:
						case	WPAD_FMT_CLASSIC_ACC_DPD:
							float	xx, yy, dx;
							xx		= AMD_SCREEN_2D_WIDTH  * 0.5f;
							yy		= AMD_SCREEN_2D_HEIGHT * 0.5f;
							dx		= xx * 0.73f;
							pad->point_x	= (Sint16)
									(state.pos.x * dx + xx);
							pad->point_y	= (Sint16)
									(state.pos.y * yy + yy);
							pad->point_z	= state.dist;
							if ((pad->point_x < _am_draw_video.point_x0) ||
									(pad->point_x >= _am_draw_video.point_x1) ||
									(pad->point_y < _am_draw_video.point_y0) ||
									(pad->point_y >= _am_draw_video.point_y1) ||
									(state.dpd_valid_fg == 0)) {
								pad->point_flag		= AMD_PAD_POINT_OUT;
							} else
								pad->point_flag		= AMD_PAD_POINT_IN;
							// no break
						case	WPAD_FMT_CORE_ACC:
						case	WPAD_FMT_FREESTYLE_ACC:
						case	WPAD_FMT_CLASSIC_ACC:
							pad->sensor_x	= state.acc.x;
							pad->sensor_y	= state.acc.y;
							pad->sensor_z	= state.acc.z;
							pad->rot_x		= nnArcTan2(-state.acc.z, -state.acc.y);
							pad->rot_z		= nnArcTan2(state.acc.x, -state.acc.y);
							// no break
						default:
							break;
					}
					switch (state.data_format) {
#if AMD_WII_USE_FREESTYLE
						case	WPAD_FMT_FREESTYLE:
						case	WPAD_FMT_FREESTYLE_ACC:
						case	WPAD_FMT_FREESTYLE_ACC_DPD:
							pad->alx		= (Sint16)
									(state.ex_status.fs.stick.x * (float)0x7fff);
							pad->aly		= (Sint16)
									(state.ex_status.fs.stick.y * (float)0x7fff);
							break;
#endif
#if AMD_WII_USE_CLPAD
						case	WPAD_FMT_CLASSIC:
						case	WPAD_FMT_CLASSIC_ACC:
						case	WPAD_FMT_CLASSIC_ACC_DPD:
							pad->alx		= (Sint16)
									(state.ex_status.cl.lstick.x * (float)0x7fff);
							pad->aly		= (Sint16)
									(state.ex_status.cl.lstick.y * (float)0x7fff);
							pad->arx		= (Sint16)
									(state.ex_status.cl.rstick.x * (float)0x7fff);
							pad->ary		= (Sint16)
									(state.ex_status.cl.rstick.y * (float)0x7fff);
							pad->al2		= (Sint16)
									(state.ex_status.cl.ltrigger * 255.0f);
							pad->ar2		= (Sint16)
									(state.ex_status.cl.rtrigger * 255.0f);
							direct			|= state.ex_status.cl.hold;
							break;
#endif
						default:
							break;
					}
				}
			}
#if AMD_WII_USE_GCPAD
			if (gc_state->err == PAD_ERR_NONE) {
				// GCコントローラ
				Uint16	button = gc_state->button;
				Uint16	*mapping = &pad->gc_mapping[0];
				for (j = 0; j < 16; j++, button >>= 1, mapping++) {
					if (button & 1)
						direct		|= *mapping;
				}
				pad->alx		= (Sint16)gc_state->stickX << 8;
				pad->aly		= (Sint16)gc_state->stickY << 8;
				pad->arx		= (Sint16)gc_state->substickX << 8;
				pad->ary		= (Sint16)gc_state->substickY << 8;
				pad->al2		= (Sint16)gc_state->triggerLeft;
				pad->ar2		= (Sint16)gc_state->triggerRight;
			} else
#endif
				if (data_num == 0)
					direct		= pad->direct;
		} else {
			direct		= 0;
			pad->alx	= 0;
			pad->aly	= 0;
			pad->arx	= 0;
			pad->ary	= 0;
			pad->al2	= 0;
			pad->ar2	= 0;
			pad->rot_x	= 0;
			pad->rot_y	= 0;
			pad->rot_z	= 0;
		}

#if AMD_WII_USE_GCPAD
		if (gc_state->err <= PAD_ERR_NO_CONTROLLER)
			padreset	|= 1 << i;
#endif
#endif

		// データの加工
		change			= pad->direct ^ direct;
		pad->stand		= change & direct;
		pad->release	= change & ~direct;
		pad->direct		= direct;
		adirect			= 0;
		if (pad->alx < -0x4000)
			adirect			|= KEY_L_LEFT;
		else if (pad->alx > 0x4000)
			adirect			|= KEY_L_RIGHT;
		if (pad->aly < -0x4000)
			adirect			|= KEY_L_DOWN;
		else if (pad->aly > 0x4000)
			adirect			|= KEY_L_UP;
		if (pad->arx < -0x4000)
			adirect			|= KEY_R_LEFT;
		else if (pad->arx > 0x4000)
			adirect			|= KEY_R_RIGHT;
		if (pad->ary < -0x4000)
			adirect			|= KEY_R_DOWN;
		else if (pad->ary > 0x4000)
			adirect			|= KEY_R_UP;
		achange			= pad->adirect ^ adirect;
		pad->astand		= achange & adirect;
		pad->arelease	= achange & ~adirect;
		pad->adirect	= adirect;

		// リピート
		pad->repeat		= 0;
		pad->arepeat	= 0;
		if ((pad->stand | pad->astand) & KEYS_LEVER) {
			pad->repeat		= direct & KEYS_LEVER;
			pad->arepeat	= adirect & KEYS_LEVER;
			pad->timer_lv	= AMD_PAD_REPEAT_START;
		} else if (--pad->timer_lv == 0) {
			pad->repeat		= direct & KEYS_LEVER;
			pad->arepeat	= adirect & KEYS_LEVER;
			pad->timer_lv	= AMD_PAD_REPEAT_SPEED;
		}
		if ((pad->stand | pad->astand) & KEYS_BUTTON) {
			pad->repeat		|= direct & KEYS_BUTTON;
			pad->arepeat	|= adirect & KEYS_BUTTON;
			pad->timer_btn	= AMD_PAD_REPEAT_START;
		} else if (--pad->timer_btn == 0) {
			pad->repeat		|= direct & KEYS_BUTTON;
			pad->arepeat	|= adirect & KEYS_BUTTON;
			pad->timer_btn	= AMD_PAD_REPEAT_SPEED;
		}

		// 押下時間
		count	= &pad->keep_time[0];
		acount	= &pad->keep_atime[0];
		for (j = 0; j < KEY_ID_MAX; j++,
				count++, acount++, change >>= 1, achange >>= 1) {
			if (change & 1) {
				pad->last_time[j]	= *count;
				*count	= 1;
			} else
				(*count)++;
			if (achange & 1) {
				pad->last_atime[j]	= *acount;
				*acount	= 1;
			} else
				(*acount)++;
		}

#if AMD_DEBUG && _XBOX
		if ((pad->direct & KEYS_RESET) == KEYS_RESET)
			XLaunchNewImage("", 0);
#endif
	}

#if _WII
#if AMD_WII_USE_GCPAD
	if (padreset)
		PADReset(padreset << 28);
#endif
	if (connect) {
		if (!WPADGetAcceptConnection())
			WPADSetAcceptConnection(TRUE);
	} else {
		if (WPADGetAcceptConnection())
			WPADSetAcceptConnection(FALSE);
	}
	amMutexUnlock(&_am_pad_mutex);
#endif
}


/*****************************************************************************/
/* void amPadEnableVibration(Sint32 port, Sint32 flag)                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port  : パッドポートID(-1ならば全ポート)                         */
/*          flag  : 振動を有効にするかどうか                                 */
/* [FUNCTION]  バイブレーションの有効・無効設定                              */
/*****************************************************************************/
void amPadEnableVibration(Sint32 port, Sint32 flag)
{
	if (!flag)
		amPadSetVibration(port, 0, 0);

	if (port == -1) {
		for (port = 0; port < AMD_PAD_PORT_MAX; port++)
			_am_pad[port].vib_flag	= flag;
	} else
		_am_pad[port].vib_flag	= flag;
}


/*****************************************************************************/
/* void amPadSetVibration(Sint32 port, Uint16 left, Uint16 right)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port  : パッドポートID(-1ならば全ポート)                         */
/*          left  : 左モーターの強度(0～65535)                               */
/*          right : 右モーターの強度(0～65535)                               */
/* [FUNCTION]  バイブレーションの設定                                        */
/*          Xbox360 : 左(高周波)右(低周波)ともに指定値で制御                 */
/*          PS3     : 右(低周波)はON/OFFのみ、左(高周波)は255段階(上位8bit)  */
/*          WII     : モーターが1つでON/OFFのみ(0のときのみOFF)              */
/*****************************************************************************/
void amPadSetVibration(Sint32 port, Uint16 left, Uint16 right)
{
#if _PC | _XBOX
	XINPUT_VIBRATION	vib_data;
	vib_data.wLeftMotorSpeed	= left;
	vib_data.wRightMotorSpeed	= right;
	if (port == -1) {
		for (port = 0; port < AMD_PAD_PORT_MAX; port++) {
			if (_am_pad[port].vib_flag)
				XInputSetState(port, &vib_data);
		}
	} else if (_am_pad[port].vib_flag)
		XInputSetState(port, &vib_data);
#elif _PS3
	CellPadActParam		vib_data;
	memset(&vib_data, 0, sizeof(CellPadActParam));
	vib_data.motor[0]	= (right == 0)? 0: 1;
	vib_data.motor[1]	= (Uint8)(left >> 8);
	if (port == -1) {
		for (port = 0; port < AMD_PAD_PORT_MAX; port++) {
			if (_am_pad[port].vib_flag)
				cellPadSetActDirect((Uint32)port, &vib_data);
		}
	} else if (_am_pad[port].vib_flag)
		cellPadSetActDirect((Uint32)port, &vib_data);
#elif _WII
	Uint32		vib_data;
	vib_data	= (left + right == 0)? WPAD_MOTOR_STOP: WPAD_MOTOR_RUMBLE;
	if (port == -1) {
		for (port = 0; port < _am_pad_num; port++) {
#if AMD_WII_USE_GCPAD
#else
			if (_am_pad[port].vib_flag && WPADIsMotorEnabled())
				WPADControlMotor((Uint32)port, vib_data);
#endif
		}
	} else if (_am_pad[port].vib_flag) {
#if AMD_WII_USE_GCPAD
#else
		WPADControlMotor((Uint32)port, vib_data);
#endif
	}
#endif
}


/*--- Local Functions -------------------------------------------------------*/

#if _WII
extern "C"
{

void *_amPadMemAlloc(Uint32 size)
{
	return	MEMAllocFromAllocator(&_am_pad_allocator, size);
}

Uint8 _amPadMemFree(void *ptr)
{
	MEMFreeToAllocator(&_am_pad_allocator, ptr);

	return	1;
}

};
#endif
