/*****************************************************************************/
/*      amConvert.h                 Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* アドレス変換ライブラリヘッダ                                              */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090331-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_CONVERT_H
#define _AM_CONVERT_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

#define AMD_CONV_FUNC_MAX		(16)

#define AMD_CONVERTED_MARK		'!'

#define amConvert(_base, _type, _data)	\
{ \
	if (_data) \
		(_data)		= (_type)((Sint32)(_base) + (Sint32)(_data)); \
}

#define AMD_ENDIAN_INLINE		(1)

// エンディアン定義
#define AMD_ENDIAN_LITTLE		(0)
#define AMD_ENDIAN_BIG			(1)

#if _PC | _IPHONE
// デフォルトのエンディアン
#define AMD_ENDIAN_DEFAULT		AMD_ENDIAN_LITTLE
#else
#define AMD_ENDIAN_DEFAULT		AMD_ENDIAN_BIG
#endif

// ターゲットのエンディアン
#if _PC | _IPHONE
#define AMD_ENDIAN_TARGET		AMD_ENDIAN_LITTLE
#else
#define AMD_ENDIAN_TARGET		AMD_ENDIAN_BIG
#endif

#if AMD_ENDIAN_INLINE
inline Uint32 _amConvertEndian32(Uint32 data)
{
	return	((data & 0xff000000) >> 24) |
			((data & 0x00ff0000) >>  8) |
			((data & 0x0000ff00) <<  8) |
			((data & 0x000000ff) << 24);
}

inline Uint16 _amConvertEndian16(Uint16 data)
{
	return	((data & 0xff00) >> 8) |
			((data & 0x00ff) << 8);
}

inline Uint64 _amConvertEndian64(Uint64 data)
{
#if 0
	return	((data & (Uint64)0xff00000000000000) >> 56) |
			((data & (Uint64)0x00ff000000000000) >> 40) |
			((data & (Uint64)0x0000ff0000000000) >> 24) |
			((data & (Uint64)0x000000ff00000000) >>  8) |
			((data & (Uint64)0x00000000ff000000) <<  8) |
			((data & (Uint64)0x0000000000ff0000) << 24) |
			((data & (Uint64)0x000000000000ff00) << 40) |
			((data & (Uint64)0x00000000000000ff) << 56);
#else
	return	(((data >> 56) & 0xff) <<  0) |
			(((data >> 48) & 0xff) <<  8) |
			(((data >> 40) & 0xff) << 16) |
			(((data >> 32) & 0xff) << 24) |
			(((data >> 24) & 0xff) << 32) |
			(((data >> 16) & 0xff) << 40) |
			(((data >>  8) & 0xff) << 48) |
			(((data >>  0) & 0xff) << 56);
#endif
}

inline Uint32 _amConvertEndian(Uint32 data)
{
	return	_amConvertEndian32(data);
}
inline Sint32 _amConvertEndian(Sint32 data)
{
	return	(Sint32)_amConvertEndian32((Uint32)data);
}
inline Uint16 _amConvertEndian(Uint16 data)
{
	return	_amConvertEndian16(data);
}
inline Sint16 _amConvertEndian(Sint16 data)
{
	return	(Sint16)_amConvertEndian16((Uint16)data);
}
inline Uint64 _amConvertEndian(Uint64 data)
{
	return	_amConvertEndian64(data);
}
inline Sint64 _amConvertEndian(Sint64 data)
{
	return	(Sint64)_amConvertEndian64((Uint64)data);
}
inline void _amConvertEndian(Uint32 *data)
{
	*data	= _amConvertEndian32(*data);
}
inline void _amConvertEndian(Sint32 *data)
{
	*data	= (Sint32)_amConvertEndian32(*(Uint32 *)data);
}
inline void _amConvertEndian(Uint16 *data)
{
	*data	= _amConvertEndian16(*data);
}
inline void _amConvertEndian(Sint16 *data)
{
	*data	= (Sint16)_amConvertEndian16(*(Uint16 *)data);
}
inline void _amConvertEndian(Uint64 *data)
{
	*data	= _amConvertEndian64(*data);
}
inline void _amConvertEndian(Sint64 *data)
{
	*data	= (Sint64)_amConvertEndian64(*(Uint64 *)data);
}
inline void _amConvertEndian(void *data)
{
	*(Uint32 *)data	= _amConvertEndian32(*(Uint32 *)data);
}
#else
Uint32 _amConvertEndian32(Uint32 data);
Uint16 _amConvertEndian16(Uint16 data);
Uint64 _amConvertEndian64(Uint64 data);
Uint32 _amConvertEndian(Uint32 data);
Sint32 _amConvertEndian(Sint32 data);
Uint16 _amConvertEndian(Uint16 data);
Sint16 _amConvertEndian(Sint16 data);
Uint64 _amConvertEndian(Uint64 data);
Sint64 _amConvertEndian(Sint64 data);
void _amConvertEndian(Uint32 *data);
void _amConvertEndian(Sint32 *data);
void _amConvertEndian(Uint16 *data);
void _amConvertEndian(Sint16 *data);
void _amConvertEndian(Uint64 *data);
void _amConvertEndian(Sint64 *data);
void _amConvertEndian(void *data);
#endif

// デフォルトエンディアン→ターゲットエンディアン
#if AMD_ENDIAN_TARGET ^ AMD_ENDIAN_DEFAULT
#define amConvertEndian(_arg)			_amConvertEndian(_arg)
#else
#define amConvertEndian(_arg)			(_arg)
#endif

// 任意のエンディアン→ターゲットエンディアン
#if AMD_ENDIAN_TARGET == AMD_ENDIAN_LITTLE
#define amConvertLittleEndian(_arg)		(_arg)
#define amConvertBigEndian(_arg)		_amConvertEndian(_arg)
#else
#define amConvertLittleEndian(_arg)		_amConvertEndian(_arg)
#define amConvertBigEndian(_arg)		(_arg)
#endif

// 動的な任意のエンディアン→ターゲットエンディアン
#define amConvertDymanicEndian(_endian, _arg)	\
	(((_endian) ^ AMD_ENDIAN_TARGET)? _amConvertEndian(_arg): (_arg) = (_arg))


/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* Sint32 amConvertAddress(void *header)                                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  header : 変換対象イメージへのポインタ                            */
/* [RETURN] 0 : すでに変換済み or 未サポート                                 */
/*          1 : 変換成功                                                     */
/* [FUNCTION]  アドレス変換（未サポートの場合は無視）                        */
/*****************************************************************************/
Sint32 amConvertAddress(void *header);

/*****************************************************************************/
/* Sint32 amConvertRegist(char *file_id, void (*func)(Uint8 *))              */
/*---------------------------------------------------------------------------*/
/* [INPUT]  file_id : ファイル識別ID                                         */
/*          func    : 変換関数                                               */
/* [RETURN] 0 : すでに登録済み                                               */
/*          1 : 登録成功                                                     */
/*         -1 : 登録失敗                                                     */
/* [FUNCTION]  アドレス変換関数の登録                                        */
/*****************************************************************************/
Sint32 amConvertRegist(char *file_id, Sint32 (*func)(Uint8 *));

#endif
