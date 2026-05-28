/*****************************************************************************/
/*      amBind.h                    Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* バインドファイルライブラリヘッダ                                          */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090331-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_BIND_H
#define _AM_BIND_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

#define AMD_BIND_DEBUG_NAME_LEN		(32)	//!< デバッグファイル名長

// ファイル情報
typedef struct {
	void		*data;				// 実データ
	Sint32		size;				// データサイズ

	Sint32		type;				// タイプ
	Sint16		user0;				// ユーザーデータ
	Sint16		user1;
} AMS_AMB_FILE;

// デバッグ情報
typedef struct {
	char		filename[AMD_BIND_DEBUG_NAME_LEN];		// ファイル名
} AMS_AMB_DEBUG;

// ファイルヘッダ
typedef struct {
	char		file_id[4];			// "#AMB"
	Sint32		header_size;		// ヘッダサイズ
	Sint16		ver_major;			// メジャーバージョン
	Sint16		ver_minor;			// マイナーバージョン
	Uint8		flag[4];			// フラグ
	Sint32		file_num;			// ファイル数
	AMS_AMB_FILE	*file;			// ファイル情報
	Sint32		reserved[1];
	AMS_AMB_DEBUG	*debug;			// デバッグ情報
} AMS_AMB_HEADER;

// ファイルヘッダフラグ
#define AMD_AMB_FLAG0_ENDIAN_BIG	BIT_0	// ビッグエンディアン


/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* void amBindConv(Uint8 *amb)                                               */
/*---------------------------------------------------------------------------*/
/* [INPUT]  file : 変換対象イメージへのポインタ                              */
/* [FUNCTION]  AMBファイルのアドレス変換                                     */
/*****************************************************************************/
Sint32 amBindConv(Uint8 *amb);

/*****************************************************************************/
/* void amBindConvertAll(Uint8 *header)                                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  header : 変換対象イメージへのポインタ                            */
/* [FUNCTION]  AMBファイルのアドレス変換（内部のファイルも変換する）         */
/*****************************************************************************/
Sint32 amBindConvertAll(Uint8 *header);

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
void *amBindGet(AMS_AMB_HEADER *header, Sint32 index,
		AMS_AMB_FILE **fileinfo = NULL);

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
void *amBindSearch(AMS_AMB_HEADER *header, char *filename,
		AMS_AMB_FILE **fileinfo = NULL);

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
void *amBindSearchEx(AMS_AMB_HEADER *header, char *exname, void *top = NULL,
		AMS_AMB_FILE **fileinfo = NULL);

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
void *amBindSearchID(AMS_AMB_HEADER *header, char *file_id, void *top = NULL,
		AMS_AMB_FILE **fileinfo = NULL);


#endif
