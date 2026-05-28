/*****************************************************************************/
/*      amPad.h                     Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* パッドライブラリヘッダ                                                    */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090330-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_PAD_H
#define _AM_PAD_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

#if _XBOX
#include <XInput2.h>
#elif _PC
#include <XInput.h>
#elif _PS3
#include <cell/pad.h>
#elif _WII
#include <revolution/wpad.h>
#include <revolution/kpad.h>
#include <revolution/pad.h>

#define AMD_WII_USE_FREESTYLE	(AMD_DEBUG)		// ヌンチャク
#define AMD_WII_USE_CLPAD		(AMD_DEBUG)		// クラシックコントローラ
#define AMD_WII_USE_GCPAD		(0)		// GCコントローラ
#endif


/*--- Definitions -----------------------------------------------------------*/

#if _PS3
#define AMD_PAD_PORT_MAX		(7)
#else
#define AMD_PAD_PORT_MAX		(4)
#endif
#define AMD_PAD_REPEAT_START	(30)
#define AMD_PAD_REPEAT_SPEED	(5)

// デバイスの種類からフォーマットを自動的に設定する
#define AMD_PAD_MODE_AUTO		(1)

#define AMD_PAD_MODE_CORE		(0)		// ボタンのみ
#define AMD_PAD_MODE_ACC		(1)		// ボタンとモーションセンサー
#define AMD_PAD_MODE_ACC_DPD	(2)		// ボタンとモーションセンサーとポインタ

#define AMD_PAD_MAPPING_SINGLE_HAND		((Uint16 *)0)	// 片手持ちデフォルト
#define AMD_PAD_MAPPING_DOUBLE_HAND		((Uint16 *)1)	// 両手持ちデフォルト

#define AMD_PAD_POINT_OUT		(0)		// ポインターが画面外
#define AMD_PAD_POINT_IN		(1)		// ポインターが画面内


// ボタン定義
typedef enum {
#if _PC
	KEY_L_UP	= XINPUT_GAMEPAD_DPAD_UP,
	KEY_L_DOWN	= XINPUT_GAMEPAD_DPAD_DOWN,
	KEY_L_LEFT	= XINPUT_GAMEPAD_DPAD_LEFT,
	KEY_L_RIGHT	= XINPUT_GAMEPAD_DPAD_RIGHT,
	KEY_R_UP	= XINPUT_GAMEPAD_Y,
	KEY_R_DOWN	= XINPUT_GAMEPAD_A,
	KEY_R_LEFT	= XINPUT_GAMEPAD_X,
	KEY_R_RIGHT	= XINPUT_GAMEPAD_B,
	KEY_L1		= XINPUT_GAMEPAD_LEFT_SHOULDER,
	KEY_L2		= 0x0400,			// LTから生成
	KEY_L3		= XINPUT_GAMEPAD_LEFT_THUMB,
	KEY_R1		= XINPUT_GAMEPAD_RIGHT_SHOULDER,
	KEY_R2		= 0x0800,			// RTから生成
	KEY_R3		= XINPUT_GAMEPAD_RIGHT_THUMB,
	KEY_SELECT	= XINPUT_GAMEPAD_BACK,
	KEY_START	= XINPUT_GAMEPAD_START,
	KEYS_MASK	= 0xf3ff,
#elif _XBOX
	KEY_L_UP	= XINPUT_GAMEPAD_DPAD_UP,
	KEY_L_DOWN	= XINPUT_GAMEPAD_DPAD_DOWN,
	KEY_L_LEFT	= XINPUT_GAMEPAD_DPAD_LEFT,
	KEY_L_RIGHT	= XINPUT_GAMEPAD_DPAD_RIGHT,
	KEY_R_UP	= XINPUT_GAMEPAD_Y,
	KEY_R_DOWN	= XINPUT_GAMEPAD_A,
	KEY_R_LEFT	= XINPUT_GAMEPAD_X,
	KEY_R_RIGHT	= XINPUT_GAMEPAD_B,
	KEY_L1		= XINPUT_GAMEPAD_LEFT_SHOULDER,
	KEY_L2		= 0x0400,			// LTから生成
	KEY_L3		= XINPUT_GAMEPAD_LEFT_THUMB,
	KEY_R1		= XINPUT_GAMEPAD_RIGHT_SHOULDER,
	KEY_R2		= 0x0800,			// RTから生成
	KEY_R3		= XINPUT_GAMEPAD_RIGHT_THUMB,
	KEY_SELECT	= XINPUT_GAMEPAD_BACK,
	KEY_START	= XINPUT_GAMEPAD_START,
	KEYS_MASK	= 0xf3ff,
	KEYS_RESET	= KEY_L2 | KEY_R2 | KEY_R1,
#elif _PS3
	KEY_L_UP	= CELL_PAD_CTRL_UP,
	KEY_L_DOWN	= CELL_PAD_CTRL_DOWN,
	KEY_L_LEFT	= CELL_PAD_CTRL_LEFT,
	KEY_L_RIGHT	= CELL_PAD_CTRL_RIGHT,
	KEY_R_UP	= CELL_PAD_CTRL_TRIANGLE << 8,
	KEY_R_DOWN	= CELL_PAD_CTRL_CROSS << 8,
	KEY_R_LEFT	= CELL_PAD_CTRL_SQUARE << 8,
	KEY_R_RIGHT	= CELL_PAD_CTRL_CIRCLE << 8,
	KEY_L1		= CELL_PAD_CTRL_L1 << 8,
	KEY_L2		= CELL_PAD_CTRL_L2 << 8,
	KEY_L3		= CELL_PAD_CTRL_L3,
	KEY_R1		= CELL_PAD_CTRL_R1 << 8,
	KEY_R2		= CELL_PAD_CTRL_R2 << 8,
	KEY_R3		= CELL_PAD_CTRL_R3,
	KEY_SELECT	= CELL_PAD_CTRL_SELECT,
	KEY_START	= CELL_PAD_CTRL_START,
	KEYS_MASK1	= 0x00ff,
	KEYS_MASK2	= 0x00ff,
#elif _WII
	KEY_L_UP	= WPAD_CL_BUTTON_UP,
	KEY_L_DOWN	= WPAD_CL_BUTTON_DOWN,
	KEY_L_LEFT	= WPAD_CL_BUTTON_LEFT,
	KEY_L_RIGHT	= WPAD_CL_BUTTON_RIGHT,
	KEY_R_UP	= WPAD_CL_BUTTON_X,
	KEY_R_DOWN	= WPAD_CL_BUTTON_B,
	KEY_R_LEFT	= WPAD_CL_BUTTON_Y,
	KEY_R_RIGHT	= WPAD_CL_BUTTON_A,
	KEY_L1		= WPAD_CL_TRIGGER_ZL,
	KEY_L2		= WPAD_CL_TRIGGER_L,
	KEY_L3		= 0,		// 未使用
	KEY_R1		= WPAD_CL_TRIGGER_ZR,
	KEY_R2		= WPAD_CL_TRIGGER_R,
	KEY_R3		= 0,		// 未使用
	KEY_SELECT	= WPAD_CL_BUTTON_MINUS,
	KEY_START	= WPAD_CL_BUTTON_PLUS,
	KEYS_MASK_CORE		= 0x7fff,
	KEYS_MASK_CLASSIC	= 0xf6ff,
#elif _IPHONE
	KEY_L_UP	= 0x0001,
	KEY_L_DOWN	= 0x0002,
	KEY_L_LEFT	= 0x0004,
	KEY_L_RIGHT	= 0x0008,
	KEY_R_UP	= 0x0010,
	KEY_R_DOWN	= 0x0020,
	KEY_R_LEFT	= 0x0040,
	KEY_R_RIGHT	= 0x0080,
	KEY_L1		= 0x0100,
	KEY_L2		= 0x0200,
	KEY_L3		= 0x0400,
	KEY_R1		= 0x0800,
	KEY_R2		= 0x1000,
	KEY_R3		= 0x2000,
	KEY_SELECT	= 0x4000,
	KEY_START	= 0x8000,
	KEYS_MASK1	= 0xffff,
	KEYS_MASK2	= 0xffff,
#endif
	KEYS_LEVER	= (KEY_L_UP | KEY_L_DOWN | KEY_L_LEFT | KEY_L_RIGHT),
	KEYS_LR		= (KEY_L_LEFT | KEY_L_RIGHT),
	KEYS_UD		= (KEY_L_UP | KEY_L_DOWN),
	KEYS_BUTTON	= (~KEYS_LEVER),
} AME_KEYMASK;

// ボタンインデックス
typedef enum {
#if _PC
	KEY_ID_L_UP	= 0,
	KEY_ID_L_DOWN,
	KEY_ID_L_LEFT,
	KEY_ID_L_RIGHT,
	KEY_ID_START,
	KEY_ID_SELECT,
	KEY_ID_L3,
	KEY_ID_R3,
	KEY_ID_L1,
	KEY_ID_R1,
	KEY_ID_L2,
	KEY_ID_R2,
	KEY_ID_R_DOWN,
	KEY_ID_R_RIGHT,
	KEY_ID_R_LEFT,
	KEY_ID_R_UP,
#elif _XBOX
	KEY_ID_L_UP	= 0,
	KEY_ID_L_DOWN,
	KEY_ID_L_LEFT,
	KEY_ID_L_RIGHT,
	KEY_ID_START,
	KEY_ID_SELECT,
	KEY_ID_L3,
	KEY_ID_R3,
	KEY_ID_L1,
	KEY_ID_R1,
	KEY_ID_L2,
	KEY_ID_R2,
	KEY_ID_R_DOWN,
	KEY_ID_R_RIGHT,
	KEY_ID_R_LEFT,
	KEY_ID_R_UP,
#elif _PS3
	KEY_ID_SELECT = 0,
	KEY_ID_L3,
	KEY_ID_R3,
	KEY_ID_START,
	KEY_ID_L_UP,
	KEY_ID_L_RIGHT,
	KEY_ID_L_DOWN,
	KEY_ID_L_LEFT,
	KEY_ID_L2,
	KEY_ID_R2,
	KEY_ID_L1,
	KEY_ID_R1,
	KEY_ID_R_UP,
	KEY_ID_R_RIGHT,
	KEY_ID_R_DOWN,
	KEY_ID_R_LEFT,
#elif _WII
	KEY_ID_L_UP = 0,
	KEY_ID_L_LEFT,
	KEY_ID_R1,
	KEY_ID_R_UP,
	KEY_ID_R_RIGHT,
	KEY_ID_R_LEFT,
	KEY_ID_R_DOWN,
	KEY_ID_L1,
	KEY_ID_L3,			// 未使用
	KEY_ID_R2,
	KEY_ID_START,
	KEY_ID_R3,			// 未使用
	KEY_ID_SELECT,
	KEY_ID_L2,
	KEY_ID_L_DOWN,
	KEY_ID_L_RIGHT,
#endif
	KEY_ID_MAX,

	// Wiiコントローラボタンマッピング用
	KEY_MAP_ID_LEFT = 0,
	KEY_MAP_ID_RIGHT,
	KEY_MAP_ID_DOWN,
	KEY_MAP_ID_UP,
	KEY_MAP_ID_PLUS,
	KEY_MAP_ID_2 = 8,
	KEY_MAP_ID_1,
	KEY_MAP_ID_B,
	KEY_MAP_ID_A,
	KEY_MAP_ID_MINUS,
	KEY_MAP_ID_Z,			// ヌンチャク
	KEY_MAP_ID_C,
	KEY_MAP_ID_MAX = 16,

	// GCコントローラボタンマッピング用
	KEY_MAP_GC_LEFT = 0,
	KEY_MAP_GC_RIGHT,
	KEY_MAP_GC_DOWN,
	KEY_MAP_GC_UP,
	KEY_MAP_GC_Z,
	KEY_MAP_GC_R,
	KEY_MAP_GC_L,
	KEY_MAP_GC_A = 8,
	KEY_MAP_GC_B,
	KEY_MAP_GC_X,
	KEY_MAP_GC_Y,
	KEY_MAP_GC_START = 15,
	KEY_MAP_GC_MAX = 16,
} AME_KEYID;

// パッドデータ
typedef struct {
	// パッドの状態
	Uint32	state;

#if _PC
	Uint16	mdirect;		// マウスボタン直値
	Uint16	*mapping;		// ボタンマッピング
#elif _WII
	Uint16	mode;			// データモード
	Uint16	*mapping;		// ボタンマッピング(Wiiコントローラ)
	Uint16	*gc_mapping;	// ボタンマッピング(GCコントローラ)
	Uint32	pad_stand;		// KPAD:スタンド(HOMEボタンのみ)
	KPADStatus	kpad_status;	// KPAD
	KPADStatus	kpad_HBM;	// KPAD for HBM
#endif

	// ボタンの状態
	Uint16	direct;			// 直値
	Uint16	stand;			// スタンドエッジ
	Uint16	release;		// リリースエッジ
	Uint16	repeat;			// リピート

	Sint16	timer_lv;		// リピートタイマー（方向）
	Sint16	timer_btn;		// リピートタイマー（ボタン）

	Sint32	keep_time[KEY_ID_MAX];	// 押下時間
	Sint32	last_time[KEY_ID_MAX];	// 押下してた時間

	Sint16	alx, aly;		// 左アナログ
	Sint16	arx, ary;		// 右アナログ
	Sint16	al2, ar2;		// 左右トリガー

	// アナログボタンの状態
	Uint16	adirect;		// 直値
	Uint16	astand;			// スタンドエッジ
	Uint16	arelease;		// リリースエッジ
	Uint16	arepeat;		// リピート

	Sint32	keep_atime[KEY_ID_MAX];	// 押下時間
	Sint32	last_atime[KEY_ID_MAX];	// 押下してた時間

	// センサー情報 (PS3/Wiiのみデバイス直値が入る)
#if _WII
	float	sensor_x;		// X (Wii)
	float	sensor_y;		// Y (Wii)
	float	sensor_z;		// Z (Wii)
#else
	Sint16	sensor_x;		// X (PS3)
	Sint16	sensor_y;		// Y (PS3)
	Sint16	sensor_z;		// Z (PS3)
#endif
	Sint16	sensor_g;		// G (PS3)

	// 角度情報
	Angle32	rot_x;			// X軸回転角 (PS3/Wii)
	Angle32	rot_y;			// Y軸回転角 (PS3/Wii) ※計測不可
	Angle32	rot_z;			// Z軸回転角 (PS3/Wii)

	// ポインター情報
	Sint16	point_flag;		// ポインターフラグ
	Sint16	point_x;		// X (Wii/PC)
	Sint16	point_y;		// Y (Wii/PC)
	float	point_z;		// Z (Wii) センサーバーからの距離(m)

	// 振動フラグ
	Sint32	vib_flag;		// 振動フラグ
} AMS_PAD_DATA;

#define AMD_PAD_STAT_DEVICE_CONNECT		(0x0001)	// デバイスが接続されている
#define AMD_PAD_STAT_IGNORE_INPUT		(0x0002)	// 入力を無視する


/*--- Macros ----------------------------------------------------------------*/

#define PAD_CONNECT(_port)	(_am_pad[_port].state & AMD_PAD_STAT_DEVICE_CONNECT)

#define PAD_DIRECT(_port)	(_am_pad[_port].direct)
#define PAD_STAND(_port)	(_am_pad[_port].stand)
#define PAD_REPEAT(_port)	(_am_pad[_port].repeat)
#define PAD_RELEASE(_port)	(_am_pad[_port].release)

#define PAD_ADIRECT(_port)	(_am_pad[_port].adirect)
#define PAD_ASTAND(_port)	(_am_pad[_port].astand)
#define PAD_AREPEAT(_port)	(_am_pad[_port].arepeat)
#define PAD_ARELEASE(_port)	(_am_pad[_port].arelease)

#define PAD_MDIRECT(_port)	(_am_pad[_port].direct | _am_pad[_port].adirect)
#define PAD_MSTAND(_port)	(_am_pad[_port].stand | _am_pad[_port].astand)
#define PAD_MREPEAT(_port)	(_am_pad[_port].repeat | _am_pad[_port].arepeat)
#define PAD_MRELEASE(_port)	(_am_pad[_port].release | _am_pad[_port].arelease)

#define PAD_A_LX(_port)		(_am_pad[_port].alx)
#define PAD_A_LY(_port)		(_am_pad[_port].aly)
#define PAD_A_RX(_port)		(_am_pad[_port].arx)
#define PAD_A_RY(_port)		(_am_pad[_port].ary)
#define PAD_A_L2(_port)		(_am_pad[_port].al2)
#define PAD_A_R2(_port)		(_am_pad[_port].ar2)

#define PAD_AXIS_X(_port)	(_am_pad[_port].sensor_x)
#define PAD_AXIS_Y(_port)	(_am_pad[_port].sensor_y)
#define PAD_AXIS_Z(_port)	(_am_pad[_port].sensor_z)
#define PAD_AXIS_G(_port)	(_am_pad[_port].sensor_g)

#define PAD_ROT_X(_port)	(_am_pad[_port].rot_x)
#define PAD_ROT_Y(_port)	(_am_pad[_port].rot_y)
#define PAD_ROT_Z(_port)	(_am_pad[_port].rot_z)

#define PAD_POS_ENABLE(_port)	(_am_pad[_port].point_flag)
#define PAD_POS_X(_port)	(_am_pad[_port].point_x)
#define PAD_POS_Y(_port)	(_am_pad[_port].point_y)
#define PAD_POS_Z(_port)	(_am_pad[_port].point_z)

#define PAD_KEEP_TIME(_port, _key_id)	(_am_pad[_port].keep_time[_key_id])
#define PAD_LAST_TIME(_port, _key_id)	(_am_pad[_port].last_time[_key_id])
#define PAD_KEEP_ATIME(_port, _key_id)	(_am_pad[_port].keep_atime[_key_id])
#define PAD_LAST_ATIME(_port, _key_id)	(_am_pad[_port].last_atime[_key_id])


/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

extern AMS_PAD_DATA	_am_pad[AMD_PAD_PORT_MAX];

#if _WII
extern AMS_MUTEX	_am_pad_mutex;
extern MEMAllocator	_am_pad_allocator;

extern Sint32		_am_pad_num;
#endif


/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* void amPadInit(Sint32 pad_num, Uint32 format)                             */
/*---------------------------------------------------------------------------*/
/* [INPUT]  pad_num : 対応パッド数(Wiiのみ有効)                              */
/*          format  : 対応デバイスフォーマット(Wiiのみ有効)                  */
/* [FUNCTION]  パッドライブラリの初期化                                      */
/*****************************************************************************/
#if _WII
void amPadInit(Sint32 pad_num = 1, Uint32 format = AMD_PAD_MODE_ACC_DPD);
#else
void amPadInit(Sint32 pad_num = 0, Uint32 format = 0);
#endif

/*****************************************************************************/
/* void amPadExit(void)                                                      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  パッドライブラリの終了                                        */
/*****************************************************************************/
void amPadExit(void);

/*****************************************************************************/
/* void amPadSetDeviceFormat(Sint32 port, Uint32 format)                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port   : パッドポートID(-1ならば全ポート)                        */
/*          format : 対応デバイスフォーマット                                */
/* [FUNCTION]  デバイスフォーマットの設定(Wiiのみ有効)                       */
/*****************************************************************************/
void amPadSetDeviceFormat(Sint32 port, Uint32 format);

/*****************************************************************************/
/* void amPadSetMapping(Sint32 port, Uint16 *mapping)                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port    : パッドポートID(-1ならば全ポート)                       */
/*          mapping : ボタンマッピングテーブル                               */
/*                    AMD_PAD_MAPPING_SINGLE_HAND : 片手持ちデフォルト       */
/*                    AMD_PAD_MAPPING_DOUBLE_HAND : 両手持ちデフォルト       */
/* [FUNCTION]  ボタンマッピングテーブルの設定(Wii/PCのみ有効)                */
/*****************************************************************************/
void amPadSetMapping(Sint32 port, Uint16 *mapping);

/*****************************************************************************/
/* void amPadSetMappingGC(Sint32 port, Uint16 *mapping)                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port    : パッドポートID(-1ならば全ポート)                       */
/*          mapping : ボタンマッピングテーブル                               */
/*                    NULL : デフォルト                                      */
/* [FUNCTION]  ボタンマッピングテーブルの設定(GCコントローラ用、Wiiのみ有効) */
/*****************************************************************************/
void amPadSetMappingGC(Sint32 port, Uint16 *mapping);

/*****************************************************************************/
/* void amPadEnableInput(Sint32 port, Sint32 flag)                           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port : パッドポートID(-1ならば全ポート)                          */
/*          flag : 入力を有効にするかどうかのフラグ                          */
/* [FUNCTION]  パッド入力の有効・無効の設定                                  */
/*****************************************************************************/
void amPadEnableInput(Sint32 port, Sint32 flag);

/*****************************************************************************/
/* void amPadGetData(void)                                                   */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  パッド情報の取得                                              */
/*****************************************************************************/
void amPadGetData(void);

/*****************************************************************************/
/* void amPadEnableVibration(Sint32 port, Sint32 flag)                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  port  : パッドポートID(-1ならば全ポート)                         */
/*          flag  : 振動を有効にするかどうか                                 */
/* [FUNCTION]  バイブレーションの有効・無効設定                              */
/*****************************************************************************/
void amPadEnableVibration(Sint32 port, Sint32 flag);

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
void amPadSetVibration(Sint32 port, Uint16 left, Uint16 right);

#endif
