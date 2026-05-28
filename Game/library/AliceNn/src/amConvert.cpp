/*****************************************************************************/
/*      amConvert.cpp               Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* アドレス変換ライブラリプログラム                                          */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090331-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"


/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

typedef struct {
	Sint32 (*func)(Uint8 *);			// 変換関数
	char	file_id[4];				// ファイル識別ID
} AMS_CONV_DATA;


/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

/*--- Local Variables -------------------------------------------------------*/

Sint32		_am_conv_num = 3;
AMS_CONV_DATA	_am_conv_func[AMD_CONV_FUNC_MAX + 1] = {
	{ amBindConvertAll, "AMB"},
	{ amAMEConv, "AME"},
	{ amTxbConv, "TXB"},
	{ NULL, ""},
};


/*--- Global Functions ------------------------------------------------------*/

#if !AMD_ENDIAN_INLINE
Uint32 _amConvertEndian32(Uint32 data)
{
	return	((data & 0xff000000) >> 24) |
			((data & 0x00ff0000) >>  8) |
			((data & 0x0000ff00) <<  8) |
			((data & 0x000000ff) << 24);
}

Uint16 _amConvertEndian16(Uint16 data)
{
	return	((data & 0xff00) >> 8) |
			((data & 0x00ff) << 8);
}

Uint64 _amConvertEndian64(Uint64 data)
{
	return	((data & 0xff00000000000000) >> 56) |
			((data & 0x00ff000000000000) >> 40) |
			((data & 0x0000ff0000000000) >> 24) |
			((data & 0x000000ff00000000) >>  8) |
			((data & 0x00000000ff000000) <<  8) |
			((data & 0x0000000000ff0000) << 24) |
			((data & 0x000000000000ff00) << 40) |
			((data & 0x00000000000000ff) << 56);
}

Uint32 _amConvertEndian(Uint32 data)
{
	return	_amConvertEndian32(data);
}
Sint32 _amConvertEndian(Sint32 data)
{
	return	(Sint32)_amConvertEndian32((Uint32)data);
}
Uint16 _amConvertEndian(Uint16 data)
{
	return	_amConvertEndian16(data);
}
Sint16 _amConvertEndian(Sint16 data)
{
	return	(Sint16)_amConvertEndian16((Uint16)data);
}
Uint64 _amConvertEndian(Uint64 data)
{
	return	_amConvertEndian64(data);
}
Sint64 _amConvertEndian(Sint64 data)
{
	return	(Sint64)_amConvertEndian64((Uint64)data);
}
void _amConvertEndian(Uint32 *data)
{
	*data	= _amConvertEndian32(*data);
}
void _amConvertEndian(Sint32 *data)
{
	*data	= (Sint32)_amConvertEndian32(*(Uint32 *)data);
}
void _amConvertEndian(Uint16 *data)
{
	*data	= _amConvertEndian16(*data);
}
void _amConvertEndian(Sint16 *data)
{
	*data	= (Sint16)_amConvertEndian16(*(Uint16 *)data);
}
void _amConvertEndian(Uint64 *data)
{
	*data	= _amConvertEndian64(*data);
}
void _amConvertEndian(Sint64 *data)
{
	*data	= (Sint64)_amConvertEndian64(*(Uint64 *)data);
}
void _amConvertEndian(void *data)
{
	*(Uint32 *)data	= _amConvertEndian32(*(Uint32 *)data);
}
#endif

/*****************************************************************************/
/* Sint32 amConvertAddress(void *header)                                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  header : 変換対象イメージへのポインタ                            */
/* [RETURN] 0 : すでに変換済み or 未サポート                                 */
/*          1 : 変換成功                                                     */
/* [FUNCTION]  アドレス変換（未サポートの場合は無視）                        */
/*****************************************************************************/
Sint32 amConvertAddress(void *header)
{
	amAssert(header);

	AMS_CONV_DATA	*conv;
	char	*file_id = (char *)header;
	Sint32	(*func)(Uint8 *) = NULL;

	// 変換済みチェック
	if (*file_id == AMD_CONVERTED_MARK)
		return	0;

	file_id++;

	// フォーマットチェック
	for (conv = &_am_conv_func[0]; conv->func != NULL; conv++) {
		if (!strncmp(file_id, conv->file_id, 3)) {
			func	= conv->func;
			break;
		}
	}
	if (func == NULL) {
		return 0;
	}

	// 変換
	func((Uint8 *)header);
	*(Uint8 *)header	= AMD_CONVERTED_MARK;

	return	1;
}


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
Sint32 amConvertRegist(char *file_id, Sint32 (*func)(Uint8 *))
{
	AMS_CONV_DATA	*conv;

	// 最大登録数チェック
	if (_am_conv_num >= AMD_CONV_FUNC_MAX)
		return	-1;

	// 重複登録チェック
	conv	= &_am_conv_func[0];
	for (; conv->func != NULL; conv++) {
		if (!strncmp(file_id, conv->file_id, 3))
			return	0;
	}

	// 登録
	memset(conv, 0, sizeof(AMS_CONV_DATA));
	conv->func	= func;
	strncpy((char *)conv->file_id, file_id, 3);

	return	1;
}


/*--- Local Functions -------------------------------------------------------*/

