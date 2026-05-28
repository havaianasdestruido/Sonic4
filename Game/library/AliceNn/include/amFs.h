/*****************************************************************************/
/*      amFs.h                                                               */
/*            Copyright(c) 2000-2009 Dimps CORP. All Rights Reserved.        */
/*---------------------------------------------------------------------------*/
/* ファイルシステムヘッダ                                                    */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090319-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_FS_H
#define _AM_FS_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

#include <stdlib.h>

#define AMD_USE_CRIFS		(1)

#if AMD_USE_CRIFS
#include <cri_xpt.h>

#if _WII
#include <cri_audio_wii.h>
#include <cri_file_system_wii.h>
#endif

#include <cri_audio.h>

#include <cri_file_system.h>
#endif

#if _WII && _WIIWARE
#include <revolution/cnt.h>
#endif


/*--- Definitions -----------------------------------------------------------*/

/* デバッグ */
#if AMD_DEBUG
#define AMD_FS_DEBUG				(1)
#else
#define AMD_FS_DEBUG				(0)
#endif

/* 最大キュー容量 */
//#define AMD_FS_MAX_FILES			(128)
#define AMD_FS_MAX_FILES			(_am_fs_max_file)

/* 最大キャッシュ数 */
//#define AMD_FS_CACHE_MAX_FILES	(128)
#define AMD_FS_CACHE_MAX_FILES		(_am_fs_max_cache)

/* 最大AFSパーティション数(1-4) */
//#define AMD_FS_MAX_AFS			(4)
#define AMD_FS_MAX_AFS				(_am_fs_max_afs)

/* 最大AFS内ファイル数(1-65535) */
//#define AMD_FS_MAX_AFS0_FILES		(3000)
//#define AMD_FS_MAX_AFS1_FILES		(3000)
//#define AMD_FS_MAX_AFS2_FILES		(3000)
//#define AMD_FS_MAX_AFS3_FILES		(3000)

/* 最大CNTハンドル数(Wii) */
#define AMD_FS_MAX_CNT				(8)
enum {
	AMD_FS_CNT_SHARED = 0,
	AMD_FS_CNT_SHARED_SOUND,
	AMD_FS_CNT_NWM,
	AMD_FS_CNT_HBM,
	AMD_FS_CNT_USER,
};
enum {
	AMD_FS_CONTENTS_SHARED = 2,
	AMD_FS_CONTENTS_SHARED_SOUND,
	AMD_FS_CONTENTS_NWM,
	AMD_FS_CONTENTS_HBM,
	AMD_FS_CONTENTS_USER,
};

/* オープン、クローズを１イント１回にする */
#if !_IPHONE
#define AMD_FS_COMMAND_VINT		(0)
#else
#define AMD_FS_COMMAND_VINT		(1)
#endif

/* オープンエラーが発生した時にエラーコードを返す(ツール用) */
/* ファイル読み込みはエラーリクエストがクリアされるまで一時停止される */
/* 通常はトレイオープンシーケンスへ移行する */
//#define AMD_FS_OPEN_ERROR			(AMD_DEBUG)
#define AMD_FS_OPEN_ERROR			(1)	// トレイオープンシーケンスに移行しない

/* キャッシュファイルの指定(参考) */
#if AMD_DEVICE & AMD_DEVICE_DISC
#define AMD_FS_CACHE_FILE			"\\CACHE.LST"
#else
#define AMD_FS_CACHE_FILE			"/usr/cache.lst"
#endif

/* デバイス名を前にくっつける */
#define AMD_FS_DEVICE_NAME		(1)

/* 遅延クローズ */
#define AMD_FS_DELAY_CLOSE		(0)
#define AMD_FS_MAX_CLOSE		(64)

/* ファイル用バッファのデフォルトヒープ */
#if _WII
#define AMD_FS_MALLOC_MEM_DEFAULT	(1)
#else
#define AMD_FS_MALLOC_MEM_DEFAULT	(AMD_HEAP_DEFAULT)
#endif

/* CDFSファイルキャッシュ構造体 */
typedef struct {
	Sint8		type;					/* ファイルタイプ */
	Sint8		buf_delete;				/* バッファの消去フラグ */
	char		file_name[58];			/* ファイル名 */
	Sint32		refs;					/* 参照回数 */
	Sint32		file_id;				/* AFSファイルID */
	Sint32		length;					/* ファイルサイズ */
	Sint32		size;					/* バッファサイズ */
	Sint32		read_size;				/* 読み込み済みサイズ (書き込み済サイズ)*/
	void		*buf;					/* 読み込みアドレス */
	Sint32		stat;					/* 状態 */
} AMS_FS_CACHE;						/* 80bytes */

/* CDFS管理構造体 */
typedef struct tagCdFs {
	Sint8		type;					/* ファイルタイプ */
	Sint8		buf_delete;				/* バッファの消去フラグ */
	Sint8		malloc_mode;			/* メモリ確保モード */
	Sint8		open_only;				/* プレオープンフラグ */
	Sint16		stat;					/* 状態 */
	char		file_name[58];			/* ファイル名 */
	AMS_FS_CACHE	*cache;				/* ファイルキャッシュ */
	Sint32		file_id;				/* AFSファイルID */
	Sint32		length;					/* ファイルサイズ */
	Sint32		size;					/* バッファサイズ(パーティション領域) */
	Sint32		read_size;				/* 読み込み済みサイズ (書き込み済サイズ)*/
	void		*buf;					/* 読み込みアドレス */
	struct tagCdFs	*nextp, *prevp;		/* リンク */
} AMS_FS;							/* 96bytes */

#define AMD_FS_FLAG_AUTOFREE		(0x00000001)
#define AMD_FS_FLAG_DIR				(0x08000000)	/* ディレクトリ */
#define AMD_FS_FLAG_CNT				(0x80000000)	/* CNTファイル */
#define AMD_FS_FLAG_CNT_ID			(0x70000000)	/* CNTファイルID */

/* 共用体 */
typedef union {
	AMS_FS			*cdfs;				/* 読み込み完了まではこっち */
	AMS_FS_CACHE	*cache;				/* 読み込み終わったらこっち */
} AMS_FS_PTR;						/* 4bytes */

/* ファイルタイプ */
#define AMD_FS_TYPE_FILE			(0)					/* ファイル */
#define AMD_FS_TYPE_PARTITION		(1)					/* パーティション */
#define AMD_FS_TYPE_FILELIST		(2)					/* ファイルリスト */

/* 状態 */
#define AMD_FS_STAT_WAIT			(0)					/* キュー待機中 */
#define AMD_FS_STAT_STOP			(1)					/* ADXF_STAT_STOP */
#define AMD_FS_STAT_READ			(2)					/* ADXF_STAT_READING */
#define AMD_FS_STAT_COMPLETE		(3)					/* ADXF_STAT_READEND */
#define AMD_FS_STAT_ERR				(4)					/* ADXF_STAT_ERROR */
#define AMD_FS_STAT_CANTOPEN		(5)					/* オープン不可 */
#define AMD_FS_STAT_NOMEMORY		(6)					/* バッファ確保不可 */
#define AMD_FS_STAT_OPEN			(7)					/* オープン中 */

/* メモリ確保モード */
#define AMD_FS_MALLOC_NORMAL		(AMD_MALLOCMODE_FIRSTFIT)	/* 通常 */
#define AMD_FS_MALLOC_TEMP			(AMD_MALLOCMODE_LASTFIT)	/* テンポラリ */
#define AMD_FS_MALLOC_LOW			(AMD_MALLOCMODE_LOW)		/* 下位アドレス */
#define AMD_FS_MALLOC_HIGH			(AMD_MALLOCMODE_HIGH)		/* 上位アドレス */
#define AMD_FS_MALLOC_MEM0			(0x0000)			/* デフォルトヒープ */
#define AMD_FS_MALLOC_MEM1			(0x0010)			/* MEM1 */
#define AMD_FS_MALLOC_MEM2			(0x0020)			/* MEM2 */
#define AMD_FS_MALLOC_COMP_NONE		(0x0000)
#define AMD_FS_MALLOC_COMP_LZ77		(0x0040)			/* LZ77圧縮(WiiWare) */
#define AMD_FS_MALLOC_COMP_HUFF		(0x0080)			/* Huffman圧縮(WiiWare) */

#define AMD_FS_MALLOC_MODE			(0x0003)
#define AMD_FS_MALLOC_MEM			(0x0030)
#define AMD_FS_MALLOC_MEM_MASK		(0x0020)
#define AMD_FS_MALLOC_MEM_BIT		(5)
#define AMD_FS_MALLOC_COMP			(0x00c0)

// 以下はamFsRead関数呼び出し時専用
#define AMD_FS_MALLOC_HEAP			(0x00)
#define AMD_FS_MALLOC_SYSTEM		(0x10)


/* amFsReadBackground関数戻り値 */
#define AMD_FS_QUEOVER				((AMS_FS *)(-1))	/* キューオーバー */

/* ディスク確認シーケンス番号 */
#define AMD_FS_SEQ_NORMAL			(0)					/* 正常状態 */
#define AMD_FS_SEQ_OPEN				(1)					/* オープン待ち */
#define AMD_FS_SEQ_CLOSE			(2)					/* クローズ待ち */
#define AMD_FS_SEQ_CHECK			(3)					/* ディスクチェック中 */

/* ファイルの分割読み込みサイズ */
#define AMD_FS_READ_SECTOR_SIZE		(2048)
#define AMD_FS_READ_SECTOR_BIT		(11)
#define AMD_FS_READ_SECTOR			(256)
#define AMD_FS_READ_SIZE			(AMD_FS_READ_SECTOR_SIZE * AMD_FS_READ_SECTOR)


/* ネームテーブル構造体 */
typedef struct {
	char		name[14];				/* ファイル名 */
	Uint16		id;						/* ファイルID */
} AMS_FS_NAME;						/* 16bytes */

/* パーティション情報構造体 */
typedef struct {
	Uint32		flag;					/* 読み込みフラグ */
	Sint32		files;					/* 最大ファイル数 */
	Sint8		*info;					/* パーティション情報へのポインタ */
	AMS_FS_NAME	*name;					/* ネームテーブル情報へのポインタ */
	char		read_name[58];			/* 現在のAFSファイル名 */
	char		last_name[58];			/* 最終のAFSファイル名 */
#if AMD_USE_CRIFS
	CriFsBinderHn	binder;				/* CriFsバインダー */
	CriFsBinderId	binder_id;			/* CriFsバインダーID */
	Sint32		binder_worksize;		/* CriFsバインダーワークサイズ */
	void		*binder_work;			/* CriFsバインダーワーク */
#endif
} AMS_FS_PARTITION;					/* 124bytes */

#define AMD_FS_PFLG_READ			(0x0001)		/* 読み込みデータあり */
#define AMD_FS_PFLG_RELOAD			(0x0002)		/* リロードする */
#define AMD_FS_PFLG_CNT				(0x80000000)	/* CNTファイル */
#define AMD_FS_PFLG_CNT_ID			(0x70000000)	/* CNTファイルID */


/* AFSファイルフォーマット */
typedef struct {
	Uint32		offset;					/* 先頭オフセット */
	Uint32		size;					/* ファイルサイズ */
} AMS_AFS_FILE;

typedef struct {
	char		file_id[4];				/* AFS固定 */
	Sint32		files;					/* ファイル数 */
	AMS_AFS_FILE	file[1];			/* ファイルデータテーブル */
} AMS_AFS_HEADER;


typedef struct {
	char		path[32];				/* パス名 */
	char		*name;					/* ファイル名 */
	Sint32		id;						/* ファイルID(未使用) */
	Sint32		user_data[3];			/* ユーザーデータ */
	Sint64		length;					/* ファイルサイズ */
	Sint64		offset;					/* オフセット */
} AMS_FS_FILELIST;


/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

extern Sint32		_am_fs_max_file;
extern Sint32		_am_fs_max_cache;
extern Sint32		_am_fs_max_afs;

extern Sint32		_am_fs_check;

extern Sint32		_am_fs_display_mode;

#if AMD_FS_DEVICE_NAME
/* デバイス名 */
extern char			_am_fs_device_name[MAX_PATH];
#endif
extern Sint32		_am_fs_device;

/* AFSパーティション情報 */
extern Sint32				_am_fs_max_afs;
extern AMS_FS_PARTITION	*_am_fs_partition;

#if AMD_USE_CRIFS
extern CriFsBinderHn	*_am_fs_binder_default;
#endif

#if _WII && _WIIWARE
extern MEMAllocator	_am_fs_allocator;
extern Sint32		_am_fs_heap_size;
extern CNTHandle	_am_fs_cnt_handle[AMD_FS_MAX_CNT];
#endif


/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* void amFsInit(Sint32 argc, char *argv[], Sint32 device,                   */
/*                 Sint8 *cache_file, Sint32 max_file, Sint32 max_cache,     */
/*                     Sint32 max_afs, Sint32 afs_size[])                    */
/*---------------------------------------------------------------------------*/
/* [INPUT]  argc, argv : mainに渡される引数(デバイス名を取得するため)        */
/*          device     : デバイスタイプ                                      */
/*          cache_file : キャッシュファイル名                                */
/*          max_file   : ファイルキューの最大数                              */
/*          max_cache  : ファイルキャッシュの最大数                          */
/*          max_afs    : パーティションファイルの最大数                      */
/*          afs_size   : 各パーティションファイル内ファイルの最大数          */
/* [FUNCTION]  ファイルシステム初期化                                        */
/*****************************************************************************/
void amFsInit(Sint32 argc, char *argv[], Sint32 device,
		Sint8 *cache_file, Sint32 max_file, Sint32 max_cache, Sint32 max_afs,
		Sint32 afs_size[]);

#define AMD_DEVICE_HOST			(0x0001)	/* ホスト  */
#define AMD_DEVICE_CDROM		(0x0002)	/* CD-ROM  */
#define AMD_DEVICE_DVDROM		(0x0004)	/* DVD-ROM */
#define AMD_DEVICE_DISC			(0x0006)
#define AMD_DEVICE_ADDNAME		(0x0008)	/* デバイス名追加  */


/*****************************************************************************/
/* void amFsReset(Sint32 init_partition)                                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  init_partition : パーティション情報をクリアするかどうか          */
/* [FUNCTION]  ファイルシステム再初期化                                      */
/*****************************************************************************/
void amFsReset(Sint32 init_partition = 0);

/*****************************************************************************/
/* void amFsExit(void)                                                       */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  ファイルシステム終了                                          */
/*****************************************************************************/
void amFsExit(void);

/*****************************************************************************/
/* void amFsDebugPrint(Sint32 pos_x, Sint32 pos_y)                           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  pos_x, pos_y : 表示位置                                          */
/* [FUNCTION]  デバッグ文字の表示                                            */
/*****************************************************************************/
#if AMD_FS_DEBUG
void amFsDebugPrint(Sint32 pos_x = 2, Sint32 pos_y = AMD_TEXT_HEIGHT - 2);
#else
#define amFsDebugPrint(...)		(void)0
#endif

/*****************************************************************************/
/* void amFsSetMallocMode(Sint32 mode, Sint32 lock)                          */
/*---------------------------------------------------------------------------*/
/* [INPUT]  mode : メモリ確保モード                                          */
/*              AMD_FS_MALLOC_NORMAL : 通常モード                            */
/*              AMD_FS_MALLOC_TEMP   : テンポラリモード                      */
/*              AMD_FS_MALLOC_BOTTOM : 下位アドレスモード                    */
/*              AMD_FS_MALLOC_TOP    : 上位アドレスモード                    */
/*              AMD_FS_MALLOC_MEM0   : デフォルトヒープから確保(デフォルト)  */
/*              AMD_FS_MALLOC_MEM1   : ヒープ１から確保(Wiiのみ有効)         */
/*              AMD_FS_MALLOC_MEM2   : ヒープ２から確保(Wiiのみ有効)         */
/*          lock : ファイルアクセスロックフラグ                              */
/* [FUNCTION]  メモリを確保するときの動作の指定                              */
/*****************************************************************************/
void amFsSetMallocMode(Sint32 mode, Sint32 lock = 0);

/*****************************************************************************/
/* AMS_FS *amFsPreOpen(char *file_name, Sint32 flag)                         */
/*---------------------------------------------------------------------------*/
/* [INPUT]  file_name : AFSファイル名                                        */
/*          flag      : オープン終了時にキューを自動開放するかどうかのフラグ */
/* [RETURN]  AMD_FS_QUEOVER : キューオーバー                                 */
/* [FUNCTION]  ファイルのプレオープン                                        */
/*             前もってファイルをオープン、クローズしておくことで            */
/*             以降のファイルアクセスを高速化できる                          */
/*             戻り値のポインタを利用して、オープン状況を把握する            */
/*             オープン終了後、amFsClearRequest関数を使用してキューを        */
/*             開放すること（自動開放しない場合）                            */
/*             この関数はADXを使用する場合はなにもしない（NULLを返す）       */
/*****************************************************************************/
AMS_FS *amFsPreOpen(char *file_name, Sint32 flag = 0);

/*****************************************************************************/
/* AMS_FS *amFsLoadPartition(char *afs_name, Sint32 flag)                    */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_name : AFSファイル名                                         */
/*          flag     : リード終了時にキューを自動開放するかどうかのフラグ    */
/* [RETURN]  AMD_FS_QUEOVER : キューオーバー                                 */
/* [FUNCTION]  AFSファイルのパーティション情報の読み込み                     */
/*             即時復帰型パーティションリード関数                            */
/*             戻り値のポインタを利用して、リード状況を把握する              */
/*             リード終了後、amFsClearRequest関数を使用してキューを          */
/*             開放すること（自動開放しない場合）                            */
/*             この関数は従来版との互換性のために用意されている              */
/*****************************************************************************/
AMS_FS *amFsLoadPartition(char *afs_name, Sint32 flag = 0);

/*****************************************************************************/
/* AMS_FS *amFsLoadPartition(Sint32 afs_id, char *afs_name, Sint32 flag)     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id   : AFSパーティションID                                   */
/*          afs_name : AFSファイル名                                         */
/*          flag     : リード終了時にキューを自動開放するかどうかのフラグ    */
/* [RETURN]  AMD_FS_QUEOVER : キューオーバー                                 */
/* [FUNCTION]  AFSファイルのパーティション情報の読み込み                     */
/*             即時復帰型パーティションリード関数                            */
/*             戻り値のポインタを利用して、リード状況を把握する              */
/*             リード終了後、amFsClearRequest関数を使用してキューを          */
/*             開放すること（自動開放しない場合）                            */
/*****************************************************************************/
AMS_FS *amFsLoadPartition(Sint32 afs_id, char *afs_name, Sint32 flag = 0);

/*****************************************************************************/
/* AMS_FS *amFsLoadPartitionDir(Sint32 afs_id, char *dir_name, Sint32 flag)  */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id   : AFSパーティションID                                   */
/*          dir_name : ディレクトリ名                                        */
/*          flag     : リード終了時にキューを自動開放するかどうかのフラグ    */
/* [RETURN]  AMD_FS_QUEOVER : キューオーバー                                 */
/* [FUNCTION]  ディレクトリをパーティションとして扱えるようにします          */
/*             即時復帰型パーティションリード関数                            */
/*             戻り値のポインタを利用して、リード状況を把握する              */
/*             リード終了後、amFsClearRequest関数を使用してキューを          */
/*             開放すること（自動開放しない場合）                            */
/*****************************************************************************/
AMS_FS *amFsLoadPartitionDir(Sint32 afs_id, char *dir_name, Sint32 flag = 0);

/*****************************************************************************/
/* AMS_FS *amFsLoadPartitionCNT(Sint32 afs_id, Sint32 handle_id,             */
/*                                             char *afs_name, Sint32 flag)  */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id    : AFSパーティションID                                  */
/*          handle_id : CNTハンドルID                                        */
/*          afs_name  : AFSファイル名                                        */
/*          flag      : リード終了時にキューを自動開放するかどうかのフラグ   */
/* [RETURN]  AMD_FS_QUEOVER : キューオーバー                                 */
/* [FUNCTION]  AFSファイルのパーティション情報の読み込み                     */
/*             即時復帰型パーティションリード関数                            */
/*             戻り値のポインタを利用して、リード状況を把握する              */
/*             リード終了後、amFsClearRequest関数を使用してキューを          */
/*             開放すること（自動開放しない場合）                            */
/*****************************************************************************/
AMS_FS *amFsLoadPartitionCNT(Sint32 afs_id, Sint32 handle_id, char *afs_name,
		Sint32 flag = 0);

/*****************************************************************************/
/* void amFsSetPartitionCNT(Sint32 afs_id, Sint32 handle_id)                 */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id    : AFSパーティションID                                  */
/*          handle_id : CNTハンドルID                                        */
/* [FUNCTION]  CNTファイルをパーティションとして設定する                     */
/*****************************************************************************/
void amFsSetPartitionCNT(Sint32 afs_id, Sint32 handle_id);

/*****************************************************************************/
/* void amFsSetDefaultPartition(Sint32 afs_id)                               */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id   : AFSパーティションID(-1ならば普通に読み込む)           */
/* [FUNCTION]  デフォルトパーティションの設定                                */
/*             パーティションIDを指定せずに読み込みをした場合に              */
/*             任意のパーティションから読み込ませる                          */
/*             開発時はパッキングせずに読み込み、リリース時はパッキングして  */
/*             読み込む際に読み込み側のソースを変更せずに対応できる          */
/*****************************************************************************/
void amFsSetDefaultPartition(Sint32 afs_id = -1);

/*****************************************************************************/
/* AMD_FS *amFsReadBackground(char *file_name, void *buf, Sint32 cache)      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  file_name : ファイル名                                           */
/*          buf       : 読み込みバッファ指定(NULLならば自動)                 */
/*          cache     : キャッシュを使用するかどうかのフラグ                 */
/* [RETURN] AMD_FS_QUEOVER : キューオーバー                                  */
/*          その他 : キューへのポインタ（状態の確認等に使用）                */
/* [FUNCTION]  バックグラウンドファイルロードリクエスト                      */
/*             即時復帰型ファイルリード関数                                  */
/*             戻り値のポインタを利用して、リード状況を把握する              */
/*             リード終了後、amFsClearRequest関数を使用してキュー、          */
/*             バッファの開放（非キャッシュ、自動確保時）を行うこと          */
/*             キャッシュ使用時は、リード終了後にcacheメンバを保存しておき、 */
/*             データが不要になったら、amFsFreeCache関数で開放すること       */
/*****************************************************************************/
AMS_FS *amFsReadBackground(char *file_name, void *buf = NULL,
		Sint32 cache = 0);

/*****************************************************************************/
/* AMD_FS *amFsReadBackground(Sint32 file_id, void *buf, Sint32 cache)       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  file_id : ファイルID(上位16ビットはAFS_ID)                       */
/*          buf     : 読み込みバッファ指定(NULLならば自動)                   */
/*          cache   : キャッシュを使用するかどうかのフラグ                   */
/* [RETURN] AMD_FS_QUEOVER : キューオーバー                                  */
/*          その他 : キューへのポインタ（状態の確認等に使用）                */
/* [FUNCTION]  バックグラウンドファイルロードリクエスト（AFS使用）           */
/*             即時復帰型ファイルリード関数                                  */
/*             戻り値のポインタを利用して、リード状況を把握する              */
/*             リード終了後、amFsClearRequest関数を使用してキュー、          */
/*             バッファの開放（非キャッシュ、自動確保時）を行うこと          */
/*             キャッシュ使用時は、リード終了後にcacheメンバを保存しておき、 */
/*             データが不要になったら、amFsFreeCache関数で開放すること       */
/*****************************************************************************/
AMS_FS *amFsReadBackground(Sint32 file_id, void *buf = NULL,
		Sint32 cache = 0);

/*****************************************************************************/
/* AMD_FS *amFsReadBackground(Sint32 afs_id, char *file_name, void *buf,     */
/*                                                             Sint32 cache) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id    : AFS_ID                                               */
/*          file_name : ファイル名                                           */
/*          buf       : 読み込みバッファ指定(NULLならば自動)                 */
/*          cache     : キャッシュを使用するかどうかのフラグ                 */
/* [RETURN] AMD_FS_QUEOVER : キューオーバー                                  */
/*          その他 : キューへのポインタ（状態の確認等に使用）                */
/* [FUNCTION]  バックグラウンドファイルロードリクエスト                      */
/*             即時復帰型ファイルリード関数                                  */
/*             戻り値のポインタを利用して、リード状況を把握する              */
/*             リード終了後、amFsClearRequest関数を使用してキュー、          */
/*             バッファの開放（非キャッシュ、自動確保時）を行うこと          */
/*             キャッシュ使用時は、リード終了後にcacheメンバを保存しておき、 */
/*             データが不要になったら、amFsFreeCache関数で開放すること       */
/*****************************************************************************/
AMS_FS *amFsReadBackground(Sint32 afs_id, char *file_name, void *buf = NULL,
		Sint32 cache = 0);

/*****************************************************************************/
/* AMD_FS *amFsReadFileList(char *file_name, void *buf)                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  file_name : ファイルリストファイル名                             */
/*          buf       : 読み込みバッファ指定(NULLならば自動)                 */
/* [RETURN] AMD_FS_QUEOVER : キューオーバー                                  */
/*          その他 : キューへのポインタ（状態の確認等に使用）                */
/* [FUNCTION]  バックグラウンドファイルロードリクエスト                      */
/*             即時復帰型ファイルリード関数                                  */
/*             戻り値のポインタを利用して、リード状況を把握する              */
/*             リード終了後、amFsClearRequest関数を使用してキュー、          */
/*             バッファの開放（自動確保時）を行うこと                        */
/*****************************************************************************/
AMS_FS *amFsReadFileList(char *file_name, void *buf = NULL);

/*****************************************************************************/
/* AMD_FS *amFsReadFileList(Sint32 afs_id, char *file_name, void *buf)       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id    : AFS_ID                                               */
/*          file_name : ファイル名                                           */
/*          buf       : 読み込みバッファ指定(NULLならば自動)                 */
/* [RETURN] AMD_FS_QUEOVER : キューオーバー                                  */
/*          その他 : キューへのポインタ（状態の確認等に使用）                */
/* [FUNCTION]  バックグラウンドファイルロードリクエスト                      */
/*             即時復帰型ファイルリード関数                                  */
/*             戻り値のポインタを利用して、リード状況を把握する              */
/*             リード終了後、amFsClearRequest関数を使用してキュー、          */
/*             バッファの開放（自動確保時）を行うこと                        */
/*****************************************************************************/
AMS_FS *amFsReadFileList(Sint32 afs_id, char *file_name, void *buf = NULL);

/*****************************************************************************/
/* Sint32 amFsRead(char *file_name, void **buf, Sint32 malloc_mode)          */
/*---------------------------------------------------------------------------*/
/* [INPUT]  file_name : ファイル名                                           */
/*          buf       : 読み込みバッファ指定(NULLならば自動)                 */
/*          malloc_mode : メモリ確保モード                                   */
/* [OUTPUT] buf       : 確保したバッファ                                     */
/* [RETURN] 読み込みファイルサイズ                                           */
/* [FUNCTION]  ファイルロード                                                */
/*****************************************************************************/
Sint32 amFsRead(char *file_name, void **buf, Sint32 malloc_mode = 0);

/*****************************************************************************/
/* void amFsOpenCNT(Sint32 handle_id, Uint32 cnt_index)                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  handle_id : CNTハンドルID                                        */
/*          cnt_index : コンテンツファイル番号(2～511)                       */
/* [FUNCTION]  コンテンツファイルのオープン(WiiWare)                         */
/*****************************************************************************/
void amFsOpenCNT(Sint32 handle_id, Uint32 cnt_index);

/*****************************************************************************/
/* void amFsCloseCNT(Sint32 handle_id)                                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  handle_id : CNTハンドルID                                        */
/* [FUNCTION]  コンテンツファイルのクローズ(WiiWare)                         */
/*****************************************************************************/
void amFsCloseCNT(Sint32 handle_id);

/*****************************************************************************/
/* Sint32 amFsReadCNT(Sint32 handle_id,                                      */
/*                          char *file_name, void **buf, Sint32 malloc_mode) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  handle_id : CNTハンドルID                                        */
/*          file_name : ファイル名                                           */
/*          buf       : 読み込みバッファ指定(NULLならば自動)                 */
/*          malloc_mode : メモリ確保モード                                   */
/* [OUTPUT] buf       : 確保したバッファ                                     */
/* [RETURN] 読み込みファイルサイズ                                           */
/*          負の数の場合は以下のエラーコードに該当します                     */
/*            CNT_RESULT_AUTHENTICATION (WARE_01を表示して停止)              */
/*            CNT_RESULT_CORRUPT        (WARE_01を表示して停止)              */
/*            CNT_RESULT_ECC_CRIT       (WARE_01を表示して停止)              */
/*            CNT_RESULT_INVALID        (引数等のエラー)                     */
/*            CNT_RESULT_OUT_OF_MEMORY  (ライブラリ内部でメモリ確保失敗)     */
/*            CNT_RESULT_UNKNOWN        (未知のエラー)                       */
/*            CNT_RESULT_FATAL          (プログラムエラー)                   */
/* [FUNCTION]  コンテンツファイル内ファイルロード(WiiWare)                   */
/*****************************************************************************/
Sint32 amFsReadCNT(Sint32 handle_id, char *file_name, void **buf,
		Sint32 malloc_mode = 0);

/*****************************************************************************/
/* AMS_FS_NAME *amFsSetNameTable(Sint32 afs_id, AMS_FS_NAME *name)           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id : AFS_ID                                                  */
/*          name   : ネームテーブルデータ                                    */
/* [RETURN] 設定されていたネームテーブルデータへのポインタ                   */
/* [FUNCTION]  AFS内のファイルに名前でアクセス可能にするためのテーブル登録   */
/*****************************************************************************/
AMS_FS_NAME *amFsSetNameTable(Sint32 afs_id, AMS_FS_NAME *name);

/*****************************************************************************/
/* Sint32 amFileId(Sint32 afs_id, Sint32 index)                              */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id  : AFSパーティションID                                    */
/*          file_id : AFSファイル内ファイルインデックス                      */
/* [RETURN] ファイルID(上位16ビットはAFS_ID)                                 */
/* [FUNCTION]  AFSパーティションとインデックスからファイルIDを取得する       */
/*****************************************************************************/
#define amFileId(afs_id, index) \
	((Sint32)((afs_id << 16) | index))


/*****************************************************************************/
/* Sint32 amFsGetStat(AMS_FS *cdfsp)                                         */
/*---------------------------------------------------------------------------*/
/* [INPUT]  cdfsp : リード状況を取得するキューへのポインタ                   */
/* [RETURN] キューの状態                                                     */
/* [FUNCTION]  キューの状態を取得する                                        */
/*             読み込みの完了待ちに使用する                                  */
/*****************************************************************************/
inline Sint32 amFsGetStat(AMS_FS *cdfsp)
{
	return	cdfsp->stat;
}


/*****************************************************************************/
/* Sint32 amFsIsComplete(AMS_FS *cdfsp)                                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  cdfsp : リード状況を取得するキューへのポインタ                   */
/* [RETURN] 読み込みが完了(AMD_FS_STAT_COMPLETE)していれば1、それ以外は0     */
/* [FUNCTION]  読み込み完了のチェック                                        */
/*****************************************************************************/
inline Sint32 amFsIsComplete(AMS_FS *cdfsp)
{
	return	(Sint32)(cdfsp->stat == AMD_FS_STAT_COMPLETE);
}


/*****************************************************************************/
/* Sint32 amFsGetSequence(void)                                              */
/*---------------------------------------------------------------------------*/
/* [RETURN] ファイルシステムの状態                                           */
/*  netcdfs時: ソケットの状態                                                */
/* [FUNCTION]  ファイルシステムの状態を取得する                              */
/*****************************************************************************/
inline Sint32 amFsGetSequence(void)
{
	return	_am_fs_check;
}


/*****************************************************************************/
/* Sint32 amFsSearchPartition(char *pname)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  pname : 検索するAFSファイル名                                    */
/* [RETURN] パーティションファイル名から該当するパーティションIDを返す       */
/*          該当無しは-1を返す                                               */
/* [FUNCTION]  パーティションの検索                                          */
/*****************************************************************************/
Sint32 amFsSearchPartition(char *pname);

/*****************************************************************************/
/* Sint32 amFsGetPartitionStat(Sint32 afs_id, char *pname)                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id : パーティションID                                        */
/*          pname  : AFSファイル名                                           */
/* [RETURN] パーティションファイルの読み込み状況を返す                       */
/*              AMD_FS_PSTAT_OK      : 読み込み済み                          */
/*              AMD_FS_PSTAT_READING : 読み込み中                            */
/*              AMD_FS_PSTAT_WAIT    : まだ別のファイルが読み込まれている    */
/* [FUNCTION]  パーティション情報の読み込み状況の取得                        */
/*****************************************************************************/
Sint32 amFsGetPartitionStat(Sint32 afs_id, char *pname = NULL);

#define AMD_FS_PSTAT_OK				(0)
#define AMD_FS_PSTAT_READING		(-1)
#define AMD_FS_PSTAT_WAIT			(-2)

/*****************************************************************************/
/* void amFsInitRequest(Sint32 close)                                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  close : 即時にクローズするかどうかのフラグ                       */
/* [FUNCTION]  全要求の取り消し                                              */
/*****************************************************************************/
void amFsInitRequest(Sint32 close = 0);

/*****************************************************************************/
/* void amFsClearRequest(AMS_FS *cdfsp, Sint32 close)                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  cdfsp : 要求を取り消すキューバッファへのポインタ                 */
/*          close : 即時にクローズするかどうかのフラグ                       */
/* [FUNCTION]  読み込み要求の取り消し                                        */
/*             読み込み用バッファ、キャッシュも開放するので、必要であれば    */
/*             ポインタのコピーを取った上で、NULLを設定すること              */
/*****************************************************************************/
void amFsClearRequest(AMS_FS *cdfsp, Sint32 close = 0);

/*****************************************************************************/
/* Sint32 amFsClose(Sint32 wait)                                             */
/*---------------------------------------------------------------------------*/
/* [INPUT]  wait : 遅延クローズバッファが空になるまで待つかどうかのフラグ    */
/* [RETURN]  遅延クローズバッファに存在するハンドル数                        */
/* [FUNCTION]  引数waitの値によって動作が異なります                          */
/*               0 : 遅延クローズバッファに存在するハンドル数を返す          */
/*              !0 : 遅延クローズバッファが空になるまでブロックする          */
/*****************************************************************************/
Sint32 amFsClose(Sint32 wait);

/*****************************************************************************/
/* void amFsFreeCache(AMS_FS_CACHE *cache)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  cache : 開放するキャッシュバッファへのポインタ                   */
/* [FUNCTION]  キャッシュの開放                                              */
/*****************************************************************************/
void amFsFreeCache(AMS_FS_CACHE *cache);

/*****************************************************************************/
/* void amFsReadRequest(void)                                                */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  ロード要求の発行                                              */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
void amFsReadRequest(void);

/*****************************************************************************/
/* void amFsVint(void)                                                       */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  VINT内実行関数（１イントに１回実行）                          */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
void amFsVint(void);

/*****************************************************************************/
/* void amFsServer(void)                                                     */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  CDFSサーバー関数                                              */
/*****************************************************************************/
void amFsServer(void);

/*****************************************************************************/
/* void amFsConvertPath(char *out_name, char *in_name)                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  in_name  : 変換元ファイル名                                      */
/* [OUTPUT] out_name : 変換結果ファイル名                                    */
/* [FUNCTION]  ファイル名の変換                                              */
/*****************************************************************************/
void amFsConvertPath(char *out_name, char *in_name);

#endif // _am_fs_H
