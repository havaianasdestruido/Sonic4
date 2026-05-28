/*****************************************************************************/
/*      amBind.cpp                  Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* バインドファイルライブラリプログラム                                      */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090331-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"


/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

/*--- Local Variables -------------------------------------------------------*/

/*--- Inline Functions ------------------------------------------------------*/

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* void amBindConv(Uint8 *amb)                                               */
/*---------------------------------------------------------------------------*/
/* [INPUT]  amb : 変換対象イメージへのポインタ                               */
/* [RETURN] 成功：１ 失敗：0                                                 */
/* [FUNCTION]  AMBファイルのアドレス変換                                     */
/*****************************************************************************/
Sint32 amBindConv(Uint8 *amb)
{
	amAssert(amb);

	AMS_AMB_HEADER		*header;
	AMS_AMB_FILE		*file;
	Sint32		n;

	header		= (AMS_AMB_HEADER *)amb;

	amAssert(!strncmp(&header->file_id[1], "AMB", 3));

	if (header->file_id[0] == AMD_CONVERTED_MARK)
		return 0;

	header->file_id[0]	= AMD_CONVERTED_MARK;

#if 0
	amConvertEndian(&header->header_size);
	amConvertEndian(&header->ver_major);
	amConvertEndian(&header->ver_minor);
	amConvertEndian(&header->file_num);
	amConvertEndian(&header->file);
	amConvertEndian(&header->debug);
#endif

	amConvert(header, AMS_AMB_FILE *, header->file);
	amConvert(header, AMS_AMB_DEBUG *, header->debug);

	n			= header->file_num;
	file		= header->file;
	for (; n > 0; n--, file++)
		amConvert(header, void *, file->data);

	// ファイル名大文字化(オフラインですれば不要)
	AMS_AMB_DEBUG		*debug;
	debug		= header->debug;
	if (debug != NULL) {
		n			= header->file_num;
		for (; n > 0; n--, debug++) {
			Sint32		j;
			char		*cp, ch;
			cp			= debug->filename;
			for (j = 0; j < AMD_BIND_DEBUG_NAME_LEN; j++, cp++) {
				ch			= *cp;
				if ((ch >= 'a') && (ch <= 'z'))
					*cp			= ch & 0xdf;
			}
		}
	}

	return 1;
}


/*****************************************************************************/
/* void amBindConvertAll(Uint8 *header)                                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  header : 変換対象イメージへのポインタ                            */
/* [RETURN] 成功：１ 失敗：0                                                 */
/* [FUNCTION]  AMBファイルのアドレス変換（内部のファイルも変換する）         */
/*****************************************************************************/
Sint32 amBindConvertAll(Uint8 *header)
{
	AMS_AMB_FILE		*file;
	Sint32	n;
	Sint32	ret;

	// AMB自体の変換
	ret = amBindConv((Uint8 *)header);
	if ( ret == 0 ) return 0;

	// 各ファイルの変換
	n			= ((AMS_AMB_HEADER *)header)->file_num;
	file		= ((AMS_AMB_HEADER *)header)->file;
	for (; n > 0; n--, file++)
		amConvertAddress(file->data);

	return 1;
}


/*****************************************************************************/
/* void *amBindGet(AMS_AMB_HEADER *header, Sint32 index,                     */
/*                                                  AMS_AMB_FILE **fileinfo) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  header : AMBファイル                                             */
/*          index  : ファイルインデックス                                    */
/* [OUTPUT] fileinfo : ファイル情報                                          */
/* [RETURN] ファイルインデックスに対応したデータへのポインタ                 */
/* [FUNCTION]  AMBファイルから任意のファイルを取得                           */
/*****************************************************************************/
void *amBindGet(AMS_AMB_HEADER *header, Sint32 index, AMS_AMB_FILE **fileinfo)
{
	amAssert(header);
	amAssert(index >= 0);
	amAssert(index < header->file_num);

	if (fileinfo != NULL)
		*fileinfo		= &header->file[index];

	return	header->file[index].data;
}


/*****************************************************************************/
/* void *amBindSearch(AMS_AMB_HEADER *header, char *filename,                */
/*                                                  AMS_AMB_FILE **fileinfo) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  header   : AMBファイル                                           */
/*          filename : 検索ファイル名                                        */
/* [OUTPUT] fileinfo : ファイル情報                                          */
/* [RETURN] ファイル名に対応したデータへのポインタ                           */
/* [FUNCTION]  AMBファイルから任意のファイル名を探す                         */
/*****************************************************************************/
void *amBindSearch(AMS_AMB_HEADER *header, char *filename, AMS_AMB_FILE **fileinfo)
{
	amAssert(header);
	amAssert(filename);

	AMS_AMB_DEBUG	*debug;
	AMS_AMB_FILE	*file;
	char		*name;
	Sint32		n;

	n		= header->file_num;
	file	= header->file;
	debug	= header->debug;

	for (; n > 0; n--, debug++, file++) {
		name	= debug->filename + (AMD_BIND_DEBUG_NAME_LEN - 1);
		for (; name != debug->filename; name--) {
			if (*name == '\\') {
				name++;
				break;
			}
		}
		if (!strcmp(name, filename)) {
			if (fileinfo != NULL)
				*fileinfo	= file;
			return	file->data;
		}
	}

	if (fileinfo != NULL)
		*fileinfo	= NULL;

	return	NULL;
}


/*****************************************************************************/
/* void *amBindSearchEx(AMS_AMB_HEADER *header, char *exname, void *top,     */
/*                                                  AMS_AMB_FILE **fileinfo) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  header   : AMBファイル                                           */
/*          exname   : 検索拡張子名                                          */
/*          top      : 前回の検索結果                                        */
/* [OUTPUT] fileinfo : ファイル情報                                          */
/* [RETURN] 拡張子名に対応したデータへのポインタ                             */
/* [FUNCTION]  AMBファイルから任意の拡張子のファイルを探す                   */
/*****************************************************************************/
void *amBindSearchEx(AMS_AMB_HEADER *header, char *exname, void *top, AMS_AMB_FILE **fileinfo)
{
	amAssert(header);
	amAssert(exname);

	AMS_AMB_DEBUG	*debug;
	AMS_AMB_FILE	*file;
	char		*name;
	Sint32		n;

	n		= header->file_num;
	file	= header->file;
	debug	= header->debug;

	if (top != NULL) {
		for (; n > 0; n--, debug++, file++) {
			if (file->data == top)
				break;
		}
		file++;
		debug++;
		n--;
		if (n <= 0) {
			if (fileinfo != NULL)
				*fileinfo	= NULL;
			return	NULL;
		}
	}

	for (; n > 0; n--, debug++, file++) {
		name	= debug->filename + (AMD_BIND_DEBUG_NAME_LEN - 1);
		for (; name != debug->filename; name--) {
			if (*name == '.') {
				name++;
				break;
			}
			if (*name == '\\') {
				name	= debug->filename + (AMD_BIND_DEBUG_NAME_LEN - 1);
				break;
			}
		}
		if (!strcmp(name, exname)) {
			if (fileinfo != NULL)
				*fileinfo	= file;
			return	file->data;
		}
	}

	if (fileinfo != NULL)
		*fileinfo	= NULL;

	return	NULL;
}


/*****************************************************************************/
/* void *amBindSearchID(AMS_AMB_HEADER *header, char *file_id, void *top,    */
/*                                                  AMS_AMB_FILE **fileinfo) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  header  : AMBファイル                                            */
/*          file_id : 検索ファイル識別ID                                     */
/*          top     : 前回の検索結果                                         */
/* [OUTPUT] fileinfo : ファイル情報                                          */
/* [RETURN] ファイルIDに対応したデータへのポインタ                           */
/* [FUNCTION]  AMBファイルから任意のファイル識別IDのファイルを探す           */
/*****************************************************************************/
void *amBindSearchID(AMS_AMB_HEADER *header, char *file_id, void *top, AMS_AMB_FILE **fileinfo)
{
	amAssert(header);

	AMS_AMB_FILE	*file;
	Sint32		n;

	n		= header->file_num;
	file	= header->file;

	if (top != NULL) {
		for (; n > 0; n--, file++) {
			if (file->data == top)
				break;
		}
		file++;
		n--;
		if (n <= 0) {
			if (fileinfo != NULL)
				*fileinfo	= NULL;
			return	NULL;
		}
	}

	for (; n > 0; n--, file++) {
		if (strncmp(&((char *)file->data)[1], file_id, 3)) {
			if (fileinfo != NULL)
				*fileinfo	= file;
			return	file->data;
		}
	}

	if (fileinfo != NULL)
		*fileinfo	= NULL;

	return	NULL;
}


/*--- Local Functions -------------------------------------------------------*/

