/*****************************************************************************/
/*      amFs.cpp                    Author : Takashi Nakano                  */
/*            Copyright(c) 2001-2009 Dimps CORP. All Rights Reserved.        */
/*---------------------------------------------------------------------------*/
/* ファイルシステムプログラム                                                */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090319-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"

#if _PC | _XBOX
#include <malloc.h>
#include <io.h>
#elif _PS3
#include <sys/memory.h>
#include <cell/cell_fs.h>
#elif _WII && _WIIWARE
#include <revolution/cx.h>
#endif


/*--- Macros ----------------------------------------------------------------*/

//! CRIエラー処理
#define AMM_CRI_ERROR(err)												\
{																		\
	if (err != CRIERR_OK) {												\
		amSystemLog("amFs::%s Error!! (%d)\n", __func__, err);			\
		amAssert(0);													\
	}																	\
}

//! CRIエラー処理(リターン)
#define AMM_CRI_ERROR_R(err)											\
{																		\
	if (err != CRIERR_OK) {												\
		amSystemLog("amFs::%s Error!! (%d)\n", __func__, err);			\
		amAssert(0);													\
		return;															\
	}																	\
}

//! CRIエラー処理(戻り値有りリターン)
#define AMM_CRI_ERROR_V(err, v)											\
{																		\
	if (err != CRIERR_OK) {												\
		amSystemLog("amFs::%s Error!! (%d)\n", __func__, err);			\
		amAssert(0);													\
		return v;														\
	}																	\
}

/*--- Definitions -----------------------------------------------------------*/

#define AMD_USE_BINDER_HEAP			(1)		// バインダーのワークをCRIヒープからとる

AMS_FS *amFsGetBuffer(void);
void amFsReleaseBuffer(AMS_FS *cdfsp);
void amFsFreeLink(AMS_FS *cdfsp);
AMS_FS_CACHE *amFsGetCacheBuffer(void);
void amFsReleaseCacheBuffer(AMS_FS_CACHE *cachep);
AMS_FS_CACHE *amFsSearchCacheBuffer(AMS_FS *cdfsp);
Sint32 amFsSearchName(Sint8 *fname);
Sint32 amFsReadStart(void);
void amFsReadListNext(AMS_FS *cdfsp);

void amFsConvertPath(char *out_name, char *in_name);

char *amFsGetColumn(char *cp);
char *amFsGetNextColumn(char *cp);
char *amFsGetNumber(char *cp, Sint32 &num);
char *amFsGetString(char *cp, char *str);

#if AMD_USE_CRIFS
static void amFsErrorCallback(const CriChar8 *err_id, CriUint32 p1, CriUint32 p2, CriUint32 *parray);
#endif


/*--- External Valiables ----------------------------------------------------*/

Sint32		_am_cri_run = 0;			/* ADXスレッド実行 */

#if AMD_TASK_THREAD_NUM > 1
AMS_MUTEX	_am_fs_mutex;
#endif

#if _WIIWARE
MEMAllocator	_am_fs_allocator;
#endif


/*--- Global Variables ------------------------------------------------------*/

/* ソフトウェアリセットチェック関数 */
Sint32			(*_am_soft_reset_func)(void);

/* ディスク確認シーケンス */
Sint32			_am_fs_insert;
Sint32			_am_fs_check;
Sint32			_am_fs_malloc;
char			_am_fs_error[80];
Sint32			_am_fs_display_mode = AMD_FS_DEBUG;

#if AMD_FS_DEVICE_NAME
/* デバイス名 */
char			_am_fs_device_name[MAX_PATH];
#endif
Sint32			_am_fs_device;

/* キューバッファ */
Sint32			_am_fs_max_file;
AMS_FS		*_am_fs;
AMS_FS		**_am_fs_buf;

Sint32			_am_fs_num;

AMS_FS		*_am_fs_ptr;
AMS_FS		*_am_fs_end;

/* キャッシュバッファ */
Sint32			_am_fs_max_cache;
AMS_FS_CACHE	*_am_fs_cache;
AMS_FS_CACHE	**_am_fs_cache_buf;

Sint32			_am_fs_cache_num;

#if _WII && _WIIWARE
Sint32			_am_fs_heap_size = 0x1000;

Sint32			_am_fs_cnt_default = 0;
CNTHandle		_am_fs_cnt_handle[AMD_FS_MAX_CNT];
#endif

#if AMD_USE_CRIFS
// CRIヒープ
Uint8			*_am_cri_buf = NULL;

CriHeap			_am_cri_heap;

void			*_am_fs_cri_work = NULL;
#endif

#if AMD_USE_CRIFS
/* CRIファイルローダーハンドル */
CriFsBinderHn		_am_fs_binder = NULL;
CriFsBinderHn		*_am_fs_binder_default = (CriFsBinderHn *)&_am_fs_binder;
CriFsLoaderHn		_am_fs_loader = NULL;
#else
/* ファイルハンドル */
AMS_FS_CTRL	*_am_fs_fctrl;
#endif

/* AFS/CPKパーティション情報 */
Sint32			_am_fs_max_afs;
AMS_FS_PARTITION	*_am_fs_partition;

#if AMD_USE_CRIFS & 0
/* ファイルキャッシュ */
ADXPS2_FCPRM	_am_fs_fcprm	__attribute__((aligned(64)));
Sint8			_am_fs_fcbuf[ADXPS2_DEF_FCSIZE_DVD]	__attribute__((aligned(64)));
#endif

#if AMD_FS_DELAY_CLOSE
/* 遅延クローズバッファ */
Sint32			_am_fs_close_handle_num;
ADXF			_am_fs_close_handle[AMD_FS_MAX_CLOSE];
void			*_am_fs_close_handle_mem[AMD_FS_MAX_CLOSE];
#endif

/* ファイルリスト読み込み */
AMS_FS			*_am_fs_filelist = NULL;
AMS_FS			*_am_fs_file_ptr = NULL;


/*--- Local Variables -------------------------------------------------------*/

#if AMD_FS_DEBUG
const char		*_am_fs_status[] = {
	"WAIT",
	"STOP",
	"READ",
	"COMP",
	"ERR ",
	"!OPN",
	"!MEM",
	"OPEN",
};

const char		*_am_fs_sequence[] = {
	"NORM",
	"OPEN",
	"CLSE",
	"CHCK",
};
#endif


/*--- Global Functions ------------------------------------------------------*/

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
void amFsInit(Sint32 argc, char *argv[], Sint32 device, Sint8 *cache_file, Sint32 max_file, Sint32 max_cache, Sint32 max_afs, Sint32 afs_size[])
{
#if !_IPHONE
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
#endif
	UNREFERENCED_PARAMETER(cache_file);
	UNREFERENCED_PARAMETER(afs_size);

	_am_fs_device		= device;


#if AMD_USE_CRIFS
	criErr_SetCallback(amFsErrorCallback);

	// CRIファイルシステムの初期化
	CriFsConfiguration	config;
	CriSint32		wksize, heapsize;

	criFs_InitializeConfiguration(config);
#if _IPHONE
	//config.thread_model = CRIFS_THREAD_MODEL_SINGLE; // 処理負荷対策
#endif
	criFs_CalculateWorkSize(config, &wksize);

	// CRIヒープの初期化・作成
	criHeap_Initialize();
	heapsize			= 2*1024*1024;		// 仮
	_am_cri_buf			= (Uint8 *)amMemAllocSystem(heapsize);
	_am_cri_heap		= criHeap_Create(_am_cri_buf, heapsize);

	_am_fs_cri_work		= criHeap_AllocFix(_am_cri_heap, wksize, "crifs_work",
			CRIHEAP_DEFAULT_MEM_ALIGN);

	criFs_Initialize(config, _am_fs_cri_work, wksize);

	_am_fs_binder		= NULL;
	criFsLoader_Create(&_am_fs_loader);
#endif

	/* キューバッファの確保 */
	if (max_file <= 0)
		max_file		= 1;
	_am_fs		= (AMS_FS *)amMemAllocSystem(sizeof(AMS_FS) * max_file);
	_am_fs_buf	= (AMS_FS **)amMemAllocSystem(sizeof(AMS_FS *) * max_file);
	_am_fs_max_file	= max_file;

	/* キャッシュバッファの確保 */
	if (max_cache <= 0)
		max_cache		= 1;
	_am_fs_cache	= (AMS_FS_CACHE *)amMemAllocSystem(
			sizeof(AMS_FS_CACHE) * max_cache);
	_am_fs_cache_buf	= (AMS_FS_CACHE **)amMemAllocSystem(
			sizeof(AMS_FS_CACHE *) * max_cache);
	_am_fs_max_cache	= max_cache;

	/* パーティション情報の初期化 */
	if (max_afs <= 0)
		max_afs			= 1;
	_am_fs_partition	= (AMS_FS_PARTITION *)amMemAllocSystem(
			sizeof(AMS_FS_PARTITION) * max_afs);
	_am_fs_max_afs	= max_afs;

	for (Sint32 i = 0; i < max_afs; i++) {
		_am_fs_partition[i].flag		= 0;
		_am_fs_partition[i].name		= NULL;
#if AMD_USE_CRIFS
		_am_fs_partition[i].binder		= NULL;
		_am_fs_partition[i].binder_id	= NULL;
		_am_fs_partition[i].binder_work	= NULL;
#else
		if (afs_size[i] <= 0)
			afs_size[i]			= 1;
		_am_fs_partition[i].files		= afs_size[i];
		_am_fs_partition[i].info		= (Sint8 *)amMemAllocSystem(
				(sizeof(AMS_AFS_HEADER) + (afs_size[i] - 1) * sizeof(AMS_AFS_FILE)
				+ 2047) & ~2047);
#endif
		_am_fs_partition[i].read_name[0]		= 0;
		_am_fs_partition[i].last_name[0]		= 0;
	}

#if AMD_FS_DELAY_CLOSE
	_am_fs_close_handle_num		= 0;
	memset(_am_fs_close_handle, 0, sizeof(ADXF) * AMD_FS_MAX_CLOSE);
	memset(_am_fs_close_handle_mem, 0, sizeof(void *) * AMD_FS_MAX_CLOSE);
#endif

#if AMD_USE_CRIFS
//	ADXM_SetupThrd(NULL);

//	ADXT_Init();
#else
	amFsLL_Init();
	amFsSC_Init(8 + 1);

	_am_fs_fctrl		= amFsSC_Create();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexCreate(&_am_fs_mutex);
#endif

	amFsReset();

#if AMD_USE_CRIFS
	_am_cri_run		= 1;
#endif
}


/*****************************************************************************/
/* void amFsReset(Sint32 init_partition)                                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  init_partition : パーティション情報をクリアするかどうか          */
/* [FUNCTION]  ファイルシステム再初期化                                      */
/*****************************************************************************/
void amFsReset(Sint32 init_partition)
{
	Sint32			i;
	AMS_FS		*cdfsp, **bufp;
	AMS_FS_CACHE	*cachep, **cbufp;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	cdfsp		= _am_fs;
	bufp		= _am_fs_buf;
	for (i = 0; i < AMD_FS_MAX_FILES; i++) {
		cdfsp->type		= AMD_FS_TYPE_FILE;
		cdfsp->buf_delete		= 1;
		cdfsp->file_name[0]		= 0;
		cdfsp->cache	= NULL;
		cdfsp->buf		= NULL;
		cdfsp->stat		= AMD_FS_STAT_WAIT;
		cdfsp->nextp	= NULL;
		cdfsp->prevp	= NULL;
		*(bufp++)		= cdfsp++;
	}

	cachep		= _am_fs_cache;
	cbufp		= _am_fs_cache_buf;
	for (i = 0; i < AMD_FS_CACHE_MAX_FILES; i++) {
		cachep->type	= AMD_FS_TYPE_FILE;
		cachep->buf_delete		= 1;
		cachep->file_name[0]	= 0;
		cachep->refs	= -1;
		cachep->buf		= NULL;
		cachep->stat	= AMD_FS_STAT_WAIT;
		*(cbufp++)		= cachep++;
	}

	_am_soft_reset_func	= NULL;
	_am_fs_num	= 0;
	_am_fs_insert	= 0;
	_am_fs_check	= AMD_FS_SEQ_NORMAL;
	_am_fs_ptr	= NULL;
	_am_fs_end	= NULL;
	_am_fs_cache_num	= 0;
	_am_fs_malloc	= AMD_FS_MALLOC_NORMAL;

	/* パーティション情報の初期化 */
	if (init_partition) {
		for (i = 0; i < AMD_FS_MAX_AFS; i++) {
			_am_fs_partition[i].flag		= 0;
			_am_fs_partition[i].read_name[0]		= 0;
			_am_fs_partition[i].last_name[0]		= 0;
		}
	}

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif
}


/*****************************************************************************/
/* void amFsExit(void)                                                       */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  ファイルシステム終了                                          */
/*****************************************************************************/
void amFsExit(void)
{
	Sint32		i;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

#if AMD_USE_CRIFS
//	amAdxExit();
#endif

	amFsInitRequest(1);

#if AMD_FS_DELAY_CLOSE
	/* 遅延クローズ処理 */
	amFsClose(1);
#endif

	amFsReset(1);

	for (i = 0; i < _am_fs_max_afs; i++) {
#if AMD_USE_CRIFS
		if (_am_fs_partition[i].binder_id != NULL) {
			criFsBinder_Unbind(_am_fs_partition[i].binder_id);
			_am_fs_partition[i].binder_id	= NULL;
		}
		if (_am_fs_partition[i].binder_work != NULL) {
#if AMD_USE_BINDER_HEAP
			criHeap_Free(_am_cri_heap, _am_fs_partition[i].binder_work);
#else
			amMemFreeSystem(_am_fs_partition[i].binder_work);
#endif
			_am_fs_partition[i].binder_work	= NULL;
		}
		if (_am_fs_partition[i].binder != NULL) {
			criFsBinder_Destroy(_am_fs_partition[i].binder);
			_am_fs_partition[i].binder		= NULL;
		}
#else
		amMemFreeSystem(_am_fs_partition[i].info);
#endif
	}
	amMemFreeSystem(_am_fs_partition);
	amMemFreeSystem(_am_fs_cache_buf);
	amMemFreeSystem(_am_fs_cache);
	amMemFreeSystem(_am_fs_buf);
	amMemFreeSystem(_am_fs);

#if AMD_USE_CRIFS
	//	ADXF_Finish();
//	ADXT_Finish();
//	ADXPS2_LoadFcacheHost(NULL);
//	ADXPS2_LoadFcacheDvd(NULL);
//	ADXM_ShutdownThrd();

//	_am_fs_loader->Destroy();
	criFsLoader_Destroy(_am_fs_loader);
//	_am_fs_manager->Destroy();

	CriFs::Finalize();

	criFs_Finalize();

	criHeap_Free(_am_cri_heap, _am_fs_cri_work);
	criHeap_Destroy(_am_cri_heap);
	criHeap_Finalize();

	amMemFreeSystem(_am_cri_buf);

	_am_cri_run		= 0;
#else
	amFsSC_Destroy(_am_fs_fctrl);

	amFsSC_Exit();
	amFsLL_Exit();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
	amMutexDelete(&_am_fs_mutex);
#endif
}


#if AMD_FS_DEBUG
/*****************************************************************************/
/* void amFsDebugPrint(Sint32 pos_x, Sint32 pos_y)                           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  pos_x, pos_y : 表示位置                                          */
/* [FUNCTION]  デバッグ文字の表示                                            */
/*****************************************************************************/
void amFsDebugPrint(Sint32 pos_x, Sint32 pos_y)
{
	if (_am_fs_display_mode) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexLock(&_am_fs_mutex);
#endif

		amPrint(pos_x, pos_y, (char *)_am_fs_error);

#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
	}
}
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
void amFsSetMallocMode(Sint32 mode, Sint32 lock)
{
#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#else
	UNREFERENCED_PARAMETER(lock);
#endif

#if _WII
	if (!(mode & AMD_FS_MALLOC_MEM))
		_am_fs_malloc	|= AMD_FS_MALLOC_MEM_DEFAULT << AMD_FS_MALLOC_MEM_BIT;

	_am_fs_malloc	= mode & (AMD_FS_MALLOC_MEM_MASK | AMD_FS_MALLOC_MODE);
#else
	_am_fs_malloc	= mode & AMD_FS_MALLOC_MODE;
#endif

#if AMD_TASK_THREAD_NUM > 1
	if (!lock) {
		amMutexUnlock(&_am_fs_mutex);
		amMutexUnlock(&_am_fs_mutex);
	}
#endif
}


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
AMS_FS *amFsPreOpen(char *file_name, Sint32 flag)
{
#if AMD_USE_CRIFS
	UNREFERENCED_PARAMETER(file_name);
	UNREFERENCED_PARAMETER(flag);

	return	NULL;
#else
	AMS_FS		*cdfsp;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	/* キューバッファの確保 */
	cdfsp		= amFsGetBuffer();
	if (cdfsp == NULL) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
		return	AMD_FS_QUEOVER;
	}

	/* ワークの初期化 */
	cdfsp->type			= AMD_FS_TYPE_FILE;
	cdfsp->buf_delete	= 0;
	cdfsp->open_only	= 1;
	strncpy(cdfsp->file_name, file_name, 57);
	cdfsp->cache		= NULL;
	cdfsp->file_id		= -1;
	cdfsp->read_size	= flag;
	cdfsp->size			= 0;
	cdfsp->buf			= NULL;
	cdfsp->stat			= AMD_FS_STAT_WAIT;

	if (!_am_fs_insert) {
		cdfsp->nextp		= NULL;
		if (_am_fs_end != NULL) {
			cdfsp->prevp		= _am_fs_end;
			_am_fs_end->nextp		= cdfsp;
		} else {
			cdfsp->prevp		= NULL;
			_am_fs_ptr		= cdfsp;
		}
		_am_fs_end		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		cdfsp->nextp		= _am_fs_ptr;
		if (_am_fs_ptr != NULL)
			_am_fs_ptr->prevp		= cdfsp;
		else
			_am_fs_end		= cdfsp;
		_am_fs_ptr		= cdfsp;
	}

	/* ロード要求の発行 */
#if !AMD_FS_COMMAND_VINT
	amFsReadRequest();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	return	cdfsp;
#endif
}


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
AMS_FS *amFsLoadPartition(char *afs_name, Sint32 flag)
{
	return	amFsLoadPartition(0, afs_name, flag);
}


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
AMS_FS *amFsLoadPartition(Sint32 afs_id, char *afs_name, Sint32 flag)
{
	AMS_FS		*cdfsp;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	/* キューバッファの確保 */
	cdfsp		= amFsGetBuffer();
	if (cdfsp == NULL) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
		return	AMD_FS_QUEOVER;
	}

	/* ワークの初期化 */
	cdfsp->type			= AMD_FS_TYPE_PARTITION;
	cdfsp->buf_delete	= 0;
	cdfsp->open_only	= 0;
	strncpy((char *)cdfsp->file_name, afs_name, 57);
	strncpy((char *)_am_fs_partition[afs_id].last_name, afs_name, 57);
	cdfsp->cache		= NULL;
	cdfsp->file_id		= afs_id;
	cdfsp->read_size	= flag & AMD_FS_FLAG_AUTOFREE;
	cdfsp->size			= 0;
	cdfsp->buf			= NULL;
	cdfsp->stat			= AMD_FS_STAT_WAIT;

	if (!_am_fs_insert) {
		cdfsp->nextp		= NULL;
		if (_am_fs_end != NULL) {
			cdfsp->prevp		= _am_fs_end;
			_am_fs_end->nextp		= cdfsp;
		} else {
			cdfsp->prevp		= NULL;
			_am_fs_ptr		= cdfsp;
		}
		_am_fs_end		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		cdfsp->nextp		= _am_fs_ptr;
		if (_am_fs_ptr != NULL)
			_am_fs_ptr->prevp		= cdfsp;
		else
			_am_fs_end		= cdfsp;
		_am_fs_ptr		= cdfsp;
	}

	/* ロード要求の発行 */
#if !AMD_FS_COMMAND_VINT
	amFsReadRequest();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	return	cdfsp;
}


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
AMS_FS *amFsLoadPartitionDir(Sint32 afs_id, char *dir_name, Sint32 flag)
{
	AMS_FS		*cdfsp;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	/* キューバッファの確保 */
	cdfsp		= amFsGetBuffer();
	if (cdfsp == NULL) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
		return	AMD_FS_QUEOVER;
	}

	/* ワークの初期化 */
	cdfsp->type			= AMD_FS_TYPE_PARTITION;
	cdfsp->buf_delete	= 0;
	cdfsp->open_only	= 0;
	strncpy((char *)cdfsp->file_name, dir_name, 57);
	strncpy((char *)_am_fs_partition[afs_id].last_name, dir_name, 57);
	cdfsp->cache		= NULL;
	cdfsp->file_id		= afs_id;
	cdfsp->read_size	= (flag & AMD_FS_FLAG_AUTOFREE) | AMD_FS_FLAG_DIR;
	cdfsp->size			= 0;
	cdfsp->buf			= NULL;
	cdfsp->stat			= AMD_FS_STAT_WAIT;

	if (!_am_fs_insert) {
		cdfsp->nextp		= NULL;
		if (_am_fs_end != NULL) {
			cdfsp->prevp		= _am_fs_end;
			_am_fs_end->nextp		= cdfsp;
		} else {
			cdfsp->prevp		= NULL;
			_am_fs_ptr		= cdfsp;
		}
		_am_fs_end		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		cdfsp->nextp		= _am_fs_ptr;
		if (_am_fs_ptr != NULL)
			_am_fs_ptr->prevp		= cdfsp;
		else
			_am_fs_end		= cdfsp;
		_am_fs_ptr		= cdfsp;
	}

	/* ロード要求の発行 */
#if !AMD_FS_COMMAND_VINT
	amFsReadRequest();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	return	cdfsp;
}


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
AMS_FS *amFsLoadPartitionCNT(Sint32 afs_id, Sint32 handle_id, char *afs_name, Sint32 flag)
{
#if _WII && _WIIWARE
	AMS_FS		*cdfsp;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	/* キューバッファの確保 */
	cdfsp		= amFsGetBuffer();
	if (cdfsp == NULL) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
		return	AMD_FS_QUEOVER;
	}

	/* ワークの初期化 */
	cdfsp->type			= AMD_FS_TYPE_PARTITION;
	cdfsp->buf_delete	= 0;
	cdfsp->open_only	= 0;
	strncpy((char *)cdfsp->file_name, afs_name, 57);
	strncpy((char *)_am_fs_partition[afs_id].last_name, afs_name, 57);
	cdfsp->cache		= NULL;
	cdfsp->file_id		= afs_id;
	cdfsp->read_size	= (flag & AMD_FS_FLAG_AUTOFREE)
						| ((handle_id << 28) & AMD_FS_FLAG_CNT_ID)
						| AMD_FS_FLAG_CNT;
	cdfsp->size			= 0;
	cdfsp->buf			= NULL;
	cdfsp->stat			= AMD_FS_STAT_WAIT;

	if (!_am_fs_insert) {
		cdfsp->nextp		= NULL;
		if (_am_fs_end != NULL) {
			cdfsp->prevp		= _am_fs_end;
			_am_fs_end->nextp		= cdfsp;
		} else {
			cdfsp->prevp		= NULL;
			_am_fs_ptr		= cdfsp;
		}
		_am_fs_end		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		cdfsp->nextp		= _am_fs_ptr;
		if (_am_fs_ptr != NULL)
			_am_fs_ptr->prevp		= cdfsp;
		else
			_am_fs_end		= cdfsp;
		_am_fs_ptr		= cdfsp;
	}

	/* ロード要求の発行 */
#if !AMD_FS_COMMAND_VINT
	amFsReadRequest();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	return	cdfsp;
#else
	UNREFERENCED_PARAMETER(afs_id);
	UNREFERENCED_PARAMETER(handle_id);
	UNREFERENCED_PARAMETER(afs_name);
	UNREFERENCED_PARAMETER(flag);

	return	NULL;
#endif
}


/*****************************************************************************/
/* void amFsSetPartitionCNT(Sint32 afs_id, Sint32 handle_id)                 */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id    : AFSパーティションID                                  */
/*          handle_id : CNTハンドルID                                        */
/* [FUNCTION]  CNTファイルをパーティションとして設定する                     */
/*****************************************************************************/
void amFsSetPartitionCNT(Sint32 afs_id, Sint32 handle_id)
{
#if _WII & _WIIWARE
	AMS_FS_PARTITION	*partition;
	char		fname[16];

	partition	= &_am_fs_partition[afs_id];
	sprintf(fname, "CONTENT_#%d", handle_id);

	if (!(partition->flag & AMD_FS_PFLG_READ) ||
			strncmp((char *)partition->read_name, (char *)fname, 57)) {
		strncpy((char *)partition->read_name, (char *)fname, 57);
		partition->flag		= AMD_FS_PFLG_READ | AMD_FS_PFLG_CNT
							| (handle_id << 28);
#if AMD_USE_CRIFS
		// バインダーの解放
		if (partition->binder_id != NULL) {
			criFsBinder_Unbind(partition->binder_id);
			partition->binder_id	= NULL;
		}
		if (partition->binder_work != NULL) {
#if AMD_USE_BINDER_HEAP
			criHeap_Free(_am_cri_heap, partition->binder_work);
#else
			amMemFreeSystem(partition->binder_work);
#endif
			partition->binder_work	= NULL;
		}
		partition->binder		= NULL;
		partition->binder_worksize	= 0;
#endif
	}
#else
	UNREFERENCED_PARAMETER(afs_id);
	UNREFERENCED_PARAMETER(handle_id);
#endif
}


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
void amFsSetDefaultPartition(Sint32 afs_id)
{
	if (afs_id < 0)
		_am_fs_binder_default		= &_am_fs_binder;
	else
		_am_fs_binder_default		= &_am_fs_partition[afs_id].binder;
}


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
AMS_FS *amFsReadBackground(char *file_name, void *buf, Sint32 cache)
{
	AMS_FS		*cdfsp;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	/* キューバッファの確保 */
	cdfsp		= amFsGetBuffer();
	if (cdfsp == NULL) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
		return	AMD_FS_QUEOVER;
	}

	/* ワークの初期化 */
	cdfsp->type			= AMD_FS_TYPE_FILE;
	cdfsp->buf_delete	= (buf == NULL)? 1: 0;
	cdfsp->malloc_mode	= (Sint8)_am_fs_malloc;
	cdfsp->open_only	= 0;
	cdfsp->stat			= AMD_FS_STAT_WAIT;
	strncpy((char *)cdfsp->file_name, file_name, 57);
	amFsConvertPath(cdfsp->file_name, cdfsp->file_name);
	cdfsp->cache		= (cache != 0)? (AMS_FS_CACHE *)-1: NULL;
	cdfsp->file_id		= -1;
	cdfsp->read_size	= 0;
	cdfsp->size			= 0;
	cdfsp->buf			= buf;
#if 0
	cdfsp->nextp		= NULL;
	if (_am_fs_end != NULL) {
		cdfsp->prevp		= _am_fs_end;
		_am_fs_end->nextp		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		_am_fs_ptr		= cdfsp;
	}
	_am_fs_end		= cdfsp;
#else
	if (!_am_fs_insert) {
		cdfsp->nextp		= NULL;
		if (_am_fs_end != NULL) {
			cdfsp->prevp		= _am_fs_end;
			_am_fs_end->nextp		= cdfsp;
		} else {
			cdfsp->prevp		= NULL;
			_am_fs_ptr		= cdfsp;
		}
		_am_fs_end		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		cdfsp->nextp		= _am_fs_ptr;
		if (_am_fs_ptr != NULL)
			_am_fs_ptr->prevp		= cdfsp;
		else
			_am_fs_end		= cdfsp;
		_am_fs_ptr		= cdfsp;
	}
#endif

	/* ロード要求の発行 */
#if !AMD_FS_COMMAND_VINT
	amFsReadRequest();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	return	cdfsp;
}


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
AMS_FS *amFsReadBackground(Sint32 file_id, void *buf, Sint32 cache)
{
	AMS_FS		*cdfsp;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	/* キューバッファの確保 */
	cdfsp		= amFsGetBuffer();
	if (cdfsp == NULL) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
		return	AMD_FS_QUEOVER;
	}

	/* ワークの初期化 */
	cdfsp->type			= AMD_FS_TYPE_FILE;
	cdfsp->buf_delete	= (buf == NULL);
	cdfsp->malloc_mode	= (Sint8)_am_fs_malloc;
	cdfsp->open_only	= 0;
	cdfsp->stat			= AMD_FS_STAT_WAIT;
	strncpy(cdfsp->file_name, (char *)_am_fs_partition[file_id >> 16].last_name, 57);
	cdfsp->cache		= (cache != 0)? (AMS_FS_CACHE *)-1: NULL;
	cdfsp->file_id		= file_id;
	cdfsp->read_size	= 0;
	cdfsp->size			= 0;
	cdfsp->buf			= buf;
#if 0
	cdfsp->nextp		= NULL;
	if (_am_fs_end != NULL) {
		cdfsp->prevp		= _am_fs_end;
		_am_fs_end->nextp		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		_am_fs_ptr		= cdfsp;
	}
	_am_fs_end		= cdfsp;
#else
	if (!_am_fs_insert) {
		cdfsp->nextp		= NULL;
		if (_am_fs_end != NULL) {
			cdfsp->prevp		= _am_fs_end;
			_am_fs_end->nextp		= cdfsp;
		} else {
			cdfsp->prevp		= NULL;
			_am_fs_ptr		= cdfsp;
		}
		_am_fs_end		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		cdfsp->nextp		= _am_fs_ptr;
		if (_am_fs_ptr != NULL)
			_am_fs_ptr->prevp		= cdfsp;
		else
			_am_fs_end		= cdfsp;
		_am_fs_ptr		= cdfsp;
	}
#endif

	/* ロード要求の発行 */
#if !AMD_FS_COMMAND_VINT
	amFsReadRequest();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	return	cdfsp;
}


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
AMS_FS *amFsReadBackground(Sint32 afs_id, char *file_name, void *buf, Sint32 cache)
{
	AMS_FS		*cdfsp;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	/* キューバッファの確保 */
	cdfsp		= amFsGetBuffer();
	if (cdfsp == NULL) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
		return	AMD_FS_QUEOVER;
	}

	/* ワークの初期化 */
	cdfsp->type			= AMD_FS_TYPE_FILE;
	cdfsp->buf_delete	= (buf == NULL)? 1: 0;
	cdfsp->malloc_mode	= (Sint8)_am_fs_malloc;
	cdfsp->open_only	= 0;
	cdfsp->stat			= AMD_FS_STAT_WAIT;
	strncpy((char *)cdfsp->file_name, file_name, 57);
	amFsConvertPath(cdfsp->file_name, cdfsp->file_name);
	cdfsp->cache		= (cache != 0)? (AMS_FS_CACHE *)-1: NULL;
	cdfsp->file_id		= (afs_id << 16) | 0xffff;
	cdfsp->read_size	= 0;
	cdfsp->size			= 0;
	cdfsp->buf			= buf;
#if 0
	cdfsp->nextp		= NULL;
	if (_am_fs_end != NULL) {
		cdfsp->prevp		= _am_fs_end;
		_am_fs_end->nextp		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		_am_fs_ptr		= cdfsp;
	}
	_am_fs_end		= cdfsp;
#else
	if (!_am_fs_insert) {
		cdfsp->nextp		= NULL;
		if (_am_fs_end != NULL) {
			cdfsp->prevp		= _am_fs_end;
			_am_fs_end->nextp		= cdfsp;
		} else {
			cdfsp->prevp		= NULL;
			_am_fs_ptr		= cdfsp;
		}
		_am_fs_end		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		cdfsp->nextp		= _am_fs_ptr;
		if (_am_fs_ptr != NULL)
			_am_fs_ptr->prevp		= cdfsp;
		else
			_am_fs_end		= cdfsp;
		_am_fs_ptr		= cdfsp;
	}
#endif

	/* ロード要求の発行 */
#if !AMD_FS_COMMAND_VINT
	amFsReadRequest();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	return	cdfsp;
}


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
AMS_FS *amFsReadFileList(char *file_name, void *buf)
{
	AMS_FS		*cdfsp;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	/* キューバッファの確保 */
	cdfsp		= amFsGetBuffer();
	if (cdfsp == NULL) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
		return	AMD_FS_QUEOVER;
	}

	/* ワークの初期化 */
	cdfsp->type			= AMD_FS_TYPE_FILELIST;
	cdfsp->buf_delete	= (buf == NULL)? 1: 0;
	cdfsp->malloc_mode	= (Sint8)_am_fs_malloc;
	cdfsp->open_only	= 0;
	cdfsp->stat			= AMD_FS_STAT_WAIT;
	strncpy((char *)cdfsp->file_name, file_name, 57);
	amFsConvertPath(cdfsp->file_name, cdfsp->file_name);
	cdfsp->cache		= NULL;
	cdfsp->file_id		= -1;
	cdfsp->read_size	= 0;
	cdfsp->size			= 0;
	cdfsp->buf			= buf;
#if 0
	cdfsp->nextp		= NULL;
	if (_am_fs_end != NULL) {
		cdfsp->prevp		= _am_fs_end;
		_am_fs_end->nextp		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		_am_fs_ptr		= cdfsp;
	}
	_am_fs_end		= cdfsp;
#else
	if (!_am_fs_insert) {
		cdfsp->nextp		= NULL;
		if (_am_fs_end != NULL) {
			cdfsp->prevp		= _am_fs_end;
			_am_fs_end->nextp		= cdfsp;
		} else {
			cdfsp->prevp		= NULL;
			_am_fs_ptr		= cdfsp;
		}
		_am_fs_end		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		cdfsp->nextp		= _am_fs_ptr;
		if (_am_fs_ptr != NULL)
			_am_fs_ptr->prevp		= cdfsp;
		else
			_am_fs_end		= cdfsp;
		_am_fs_ptr		= cdfsp;
	}
#endif

	/* ロード要求の発行 */
#if !AMD_FS_COMMAND_VINT
	amFsReadRequest();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	return	cdfsp;
}


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
AMS_FS *amFsReadFileList(Sint32 afs_id, char *file_name, void *buf)
{
	AMS_FS		*cdfsp;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	/* キューバッファの確保 */
	cdfsp		= amFsGetBuffer();
	if (cdfsp == NULL) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
		return	AMD_FS_QUEOVER;
	}

	/* ワークの初期化 */
	cdfsp->type			= AMD_FS_TYPE_FILELIST;
	cdfsp->buf_delete	= (buf == NULL)? 1: 0;
	cdfsp->malloc_mode	= (Sint8)_am_fs_malloc;
	cdfsp->open_only	= 0;
	cdfsp->stat			= AMD_FS_STAT_WAIT;
	strncpy((char *)cdfsp->file_name, file_name, 57);
	amFsConvertPath(cdfsp->file_name, cdfsp->file_name);
	cdfsp->cache		= NULL;
	cdfsp->file_id		= (afs_id << 16) | 0xffff;
	cdfsp->read_size	= 0;
	cdfsp->size			= 0;
	cdfsp->buf			= buf;
#if 0
	cdfsp->nextp		= NULL;
	if (_am_fs_end != NULL) {
		cdfsp->prevp		= _am_fs_end;
		_am_fs_end->nextp		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		_am_fs_ptr		= cdfsp;
	}
	_am_fs_end		= cdfsp;
#else
	if (!_am_fs_insert) {
		cdfsp->nextp		= NULL;
		if (_am_fs_end != NULL) {
			cdfsp->prevp		= _am_fs_end;
			_am_fs_end->nextp		= cdfsp;
		} else {
			cdfsp->prevp		= NULL;
			_am_fs_ptr		= cdfsp;
		}
		_am_fs_end		= cdfsp;
	} else {
		cdfsp->prevp		= NULL;
		cdfsp->nextp		= _am_fs_ptr;
		if (_am_fs_ptr != NULL)
			_am_fs_ptr->prevp		= cdfsp;
		else
			_am_fs_end		= cdfsp;
		_am_fs_ptr		= cdfsp;
	}
#endif

	/* ロード要求の発行 */
#if !AMD_FS_COMMAND_VINT
	amFsReadRequest();
#endif

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	return	cdfsp;
}


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
Sint32 amFsRead(char *file_name, void **buf, Sint32 malloc_mode)
{
#if AMD_USE_CRIFS
	if (_am_cri_heap) {
		CriFsBinderHn	binder;
		CriFsLoaderHn	loader;
		Sint64			length;
		Sint32			ret = -1, delete_flag = 0;

		criFsLoader_Create(&loader);

		binder		= *_am_fs_binder_default;

		criFsBinder_GetFileSize(binder, file_name, &length);
		if (length < 0) {
			amSystemLog("[WARN] '%s' can't open.\n", file_name);
			return	0;
		}

		if (*buf == NULL) {
			if (malloc_mode & AMD_FS_MALLOC_SYSTEM) {
				*buf	= (void *)amMemAllocSystem((size_t)length);
				delete_flag	= 1;
			} else {
				char	*p0;
				Sint32	index;
				index		= strlen(file_name) - 1;
				p0			= &file_name[index];
				while ((index > 0) && (p0[-1] != '/')) {
					index--;
					p0--;
				}
				*buf	= (void *)amMemDebugAlloc((size_t)length,
					malloc_mode & AMD_FS_MALLOC_MODE, AMD_HEAP_DEFAULT, p0, -1);
				delete_flag	= 2;
			}
		}

		amSystemLog("Read '%s'.\n", file_name);

#if _WII
//		DCInvalidateRange(*buf, length);
		DCFlushRange(*buf, length);
#endif

		criFsLoader_Load(loader, binder, file_name, 0, length, *buf, length);

		while (ret == -1) {
			CriFsLoaderStatus	status;
			criFsLoader_GetStatus(loader, &status);
			switch (status) {
				case	CRIFSLOADER_STATUS_LOADING:
				case	CRIFSLOADER_STATUS_STOP:
					criFs_ExecuteMain();
					amThreadSleep(10);
					break;
				case	CRIFSLOADER_STATUS_COMPLETE:
					ret		= (Sint32)length;
#if _WII
					DCStoreRange(*buf, length);
#endif
					break;
				case	CRIFSLOADER_STATUS_ERROR:
					amSystemLog("[ERR] '%s' read error.\n", file_name);
					switch (delete_flag) {
						case	1:
							amMemFreeSystem(*buf);
							*buf	= NULL;
							break;
						case	2:
							amMemFree(*buf);
							*buf	= NULL;
							break;
					}
					ret		= 0;
					break;
			}
		}

		criFsLoader_Destroy(loader);

		return	ret;
	}
#endif

#if _PC
	FILE	*file;
	Uint32	length;

	if (fopen_s(&file, file_name, "rb") != 0) {
		amSystemLog("[WARN] '%s' can't open.\n", file_name);
		return	0;
	}

	length	= _filelength(_fileno(file));
	if (*buf == NULL) {
		if (malloc_mode & AMD_FS_MALLOC_SYSTEM)
			*buf	= (void *)amMemAllocSystem(length);
		else {
			char	*p0;
			Sint32	index;
			index		= strlen(file_name) - 1;
			p0			= &file_name[index];
			while ((index > 0) && (p0[-1] != '/')) {
				index--;
				p0--;
			}
			*buf	= (void *)amMemDebugAlloc(length,
				malloc_mode & AMD_FS_MALLOC_MODE, AMD_HEAP_DEFAULT, p0, -1);
		}
	}

	amSystemLog("Read '%s'.\n", file_name);

	length	= fread_s(*buf, length, 1, length, file);

	fclose(file);

	return	length;
#elif _XBOX
	FILE	*file;
	char	fname[MAX_PATH];
	Uint32	length;

	amFsConvertPath(fname, file_name);

	if ((file = fopen(fname, "rb")) == NULL) {
		amSystemLog("[WARN] '%s' can't open.\n", file_name);
		return	0;
	}

	length	= _filelength(_fileno(file));
	if (*buf == NULL) {
		if (malloc_mode & AMD_FS_MALLOC_SYSTEM)
			*buf	= (void *)amMemAllocSystem(length);
		else {
			char	*p0;
			Sint32	index;
			index		= strlen(fname) - 1;
			p0			= &fname[index];
			while ((index > 0) && (p0[-1] != '\\')) {
				index--;
				p0--;
			}
			*buf	= (void *)amMemDebugAlloc(length,
				malloc_mode & AMD_FS_MALLOC_MODE, AMD_HEAP_DEFAULT, p0, -1);
		}
	}

	amSystemLog("Read '%s'.\n", file_name);

	length	= fread(*buf, 1, length, file);

	fclose(file);

	return	length;
#elif _PS3
	Sint32	file;
	char	fname[MAX_PATH];
	Uint64	length, tmp64;

#if _PS3HDDBOOT
	AMD_STRCPY_S(fname, MAX_PATH, _am_ps3_BootUsrdirPath);
	AMD_STRCAT_S(fname, MAX_PATH, file_name);
#else
	amFsConvertPath(fname, file_name);
#endif

	if (cellFsOpen(fname, CELL_FS_O_RDONLY, &file, NULL, 0)
			!= CELL_FS_SUCCEEDED) {
		amSystemLog("[WARN] '%s' can't open.\n", file_name);
		return	0;
	}

	cellFsLseek(file, 0, CELL_FS_SEEK_END, &length);
	cellFsLseek(file, 0, CELL_FS_SEEK_SET, &tmp64);

	if (*buf == NULL) {
		if (malloc_mode & AMD_FS_MALLOC_SYSTEM)
			*buf	= (void *)amMemAllocSystem(length);
		else {
			char	*p0;
			Sint32	index;
			index		= strlen(fname) - 1;
			p0			= &fname[index];
			while ((index > 0) && (p0[-1] != '\\')) {
				index--;
				p0--;
			}
			*buf	= (void *)amMemDebugAlloc(length,
				malloc_mode & AMD_FS_MALLOC_MODE, AMD_HEAP_DEFAULT, p0, -1);
		}
	}

	amSystemLog("Read '%s'.\n", file_name);

	cellFsRead(file, *buf, length, &length);

	cellFsClose(file);

	return	length;
#elif _WII
	DVDFileInfo	file;
	char	fname[MAX_PATH];
	Uint32	length, size;

	amFsConvertPath(fname, file_name);

	if (!DVDOpen(fname, &file)) {
		amSystemLog("[WARN] '%s' can't open.\n", file_name);
		return	0;
	}

	length		= DVDGetLength(&file);
	size		= (length + 31) & ~31;

	if (*buf == NULL) {
		if (malloc_mode & AMD_FS_MALLOC_SYSTEM)
			*buf	= (void *)amMemAllocSystem(size);
		else {
			char	*p0;
			Sint32	index;
			index		= strlen(fname) - 1;
			p0			= &fname[index];
			while ((index > 0) && (p0[-1] != '\\')) {
				index--;
				p0--;
			}
			if (!(malloc_mode & AMD_FS_MALLOC_MEM)) {
				malloc_mode		|=
						AMD_FS_MALLOC_MEM_DEFAULT << AMD_FS_MALLOC_MEM_BIT;
			}
			*buf	= (void *)amMemDebugAlloc(size,
					malloc_mode & AMD_FS_MALLOC_MODE,
					(malloc_mode >> AMD_FS_MALLOC_MEM_BIT) & 1,
					p0, -1);
		}
	}

	amSystemLog("Read '%s'.\n", file_name);

//	DCInvalidateRange(*buf, size);
	DCFlushRange(*buf, size);

	length		= DVDRead(&file, *buf, size, 0);

	DCStoreRange(*buf, size);

	DVDClose(&file);

	return	length;
#elif _IPHONE
	FILE	*file;
	char	fname[MAX_PATH];
	Uint32	length;
	
//	amFsConvertPath(fname, file_name);
	AMD_STRCPY_S(fname, MAX_PATH, _am_fs_device_name);
	AMD_STRCAT_S(fname, MAX_PATH, file_name);
	
	if ((file = fopen(fname, "rb")) == NULL) {
		amSystemLog("[WARN] '%s' can't open.\n", file_name);
		return	0;
	}
	
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	rewind(file);
//	length = 524340; // direct value
	
	if (*buf == NULL) {
		if (malloc_mode & AMD_FS_MALLOC_SYSTEM)
			*buf	= (void *)amMemAllocSystem(length);
		else {
			char	*p0;
			Sint32	index;
			index		= strlen(fname) - 1;
			p0			= &fname[index];
			while ((index > 0) && (p0[-1] != '\\')) {
				index--;
				p0--;
			}
			*buf	= (void *)amMemDebugAlloc(length,
											  malloc_mode & AMD_FS_MALLOC_MODE, AMD_HEAP_DEFAULT, p0, -1);
		}
	}
	
	amSystemLog("Read '%s'.\n", file_name);
	
	length	= fread(*buf, 1, length, file);

	fclose(file);

	return	length;
#endif
}


/*****************************************************************************/
/* void amFsOpenCNT(Sint32 handle_id, Uint32 cnt_index)                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]  handle_id : CNTハンドルID                                        */
/*          cnt_index : コンテンツファイル番号(2～511)                       */
/* [FUNCTION]  コンテンツファイルのオープン(WiiWare)                         */
/*****************************************************************************/
void amFsOpenCNT(Sint32 handle_id, Uint32 cnt_index)
{
#if _WII && _WIIWARE
	Sint32	result;

	result	= CNTInitHandle(cnt_index, &_am_fs_cnt_handle[handle_id],
			&_am_fs_allocator);

	if (result != CNT_RESULT_OK) {
		amSystemLog("[ERR] CNTInitHandle() = %d\n", result);
		amAssert(0);
	}

	amSystemLog("CNT = content%d\n", cnt_index);
	_am_fs_cnt_default	= handle_id;
	criFs_SetCntHandle_WII(&_am_fs_cnt_handle[handle_id]);
#else
	UNREFERENCED_PARAMETER(handle_id);
	UNREFERENCED_PARAMETER(cnt_index);
#endif
}


/*****************************************************************************/
/* void amFsCloseCNT(Sint32 handle_id)                                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  handle_id : CNTハンドルID                                        */
/* [FUNCTION]  コンテンツファイルのクローズ(WiiWare)                         */
/*****************************************************************************/
void amFsCloseCNT(Sint32 handle_id)
{
#if _WII && _WIIWARE
	Sint32	result;

	result	= CNTReleaseHandle(&_am_fs_cnt_handle[handle_id]);

	if (result != CNT_RESULT_OK) {
		amSystemLog("[ERR] CNTReleaseHandle() = %d\n", result);
		amAssert(0);
	}
#else
	UNREFERENCED_PARAMETER(handle_id);
#endif
}


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
Sint32 amFsReadCNT(Sint32 handle_id, char *file_name, void **buf, Sint32 malloc_mode)
{
#if _WII && _WIIWARE
	CNTFileInfo		file;
	Uint32		length, size, read_length;
	void		*work = NULL, *wbuf;
	BOOL		move = FALSE;

	if (CNTOpen(&_am_fs_cnt_handle[handle_id], file_name, &file) != CNT_RESULT_OK) {
		amSystemLog("[WARN] '%s' in CNT%d can't open.\n", file_name, handle_id);
		return	0;
	}

	length		= CNTGetLength(&file);
	size		= (length + 31) & ~31;

	if ((malloc_mode & AMD_FS_MALLOC_COMP) != AMD_FS_MALLOC_COMP_NONE) {
		if (*buf == NULL) {
			if (malloc_mode & AMD_FS_MALLOC_SYSTEM)
				work	= (void *)amMemAllocSystem(size);
			else {
				if (!(malloc_mode & AMD_FS_MALLOC_MEM)) {
					malloc_mode		|=
							AMD_FS_MALLOC_MEM_DEFAULT << AMD_FS_MALLOC_MEM_BIT;
				}
				work	= (void *)amMemDebugAlloc(size,
						(malloc_mode & AMD_FS_MALLOC_MODE) ^ 1,
						(malloc_mode >> AMD_FS_MALLOC_MEM_BIT) & 1,
						"FILETEMP", -1);
			}
		} else {
			work	= *buf;
			move	= TRUE;
		}

		amSystemLog("Read '%s' in CNT%d.\n", file_name, handle_id);

//		DCInvalidateRange(work, size);
		DCFlushRange(work, size);

		read_length	= CNTRead(&file, work, size);

		CNTClose(&file);

		if (read_length < length) {
			if (!move) {
				if (malloc_mode & AMD_FS_MALLOC_SYSTEM)
					amMemFreeSystem(work);
				else
					amMemFree(work);
			}

#if AMD_DEBUG
			switch (read_length) {
				case	CNT_RESULT_AUTHENTICATION:
					amSystemLog("[ERR] '%s' in CNT%d AUTHENTICATION error.\n"
						"Display message 'WARE_01' and Stop application.\n",
						file_name, handle_id);
					break;
				case	CNT_RESULT_CORRUPT:
					amSystemLog("[ERR] '%s' in CNT%d CORRUPT error.\n",
						"Display message 'WARE_01' and Stop application.\n",
						file_name, handle_id);
					break;
				case	CNT_RESULT_ECC_CRIT:
					amSystemLog("[ERR] '%s' in CNT%d ECC_CRIT error.\n"
						"Display message 'WARE_01' and Stop application.\n",
						file_name, handle_id);
					break;
				case	CNT_RESULT_INVALID:
					amSystemLog("[ERR] '%s' in CNT%d INVALID error.\n",
						file_name, handle_id);
					amAssert(0);
					break;
				case	CNT_RESULT_OUT_OF_MEMORY:
					amSystemLog("[ERR] '%s' in CNT%d OUT_OF_MEMORY error.\n"
						"Stop application and Report to Nintendo.\n",
						file_name, handle_id);
					break;
				case	CNT_RESULT_UNKNOWN:
					amSystemLog("[ERR] '%s' in CNT%d UNKNOWN error.\n"
						"Stop application and Report to Nintendo.\n",
						file_name, handle_id);
					break;
				case	CNT_RESULT_FATAL:
					amSystemLog("[ERR] '%s' in CNT%d FATAL error.\n", file_name, handle_id);
					amAssert(0);
					break;
				default:
					break;
			}
#endif
			return	read_length;
		}

		size		= CXGetUncompressedSize(work);
	}

	if (*buf == NULL) {
		if (malloc_mode & AMD_FS_MALLOC_SYSTEM)
			*buf	= (void *)amMemAllocSystem(size);
		else {
			char	*p0;
			Sint32	index;
			index		= strlen(file_name) - 1;
			p0			= &file_name[index];
			while ((index > 0) && (p0[-1] != '\\')) {
				index--;
				p0--;
			}
			if (!(malloc_mode & AMD_FS_MALLOC_MEM)) {
				malloc_mode		|=
						AMD_FS_MALLOC_MEM_DEFAULT << AMD_FS_MALLOC_MEM_BIT;
			}
			*buf	= (void *)amMemDebugAlloc(size,
					malloc_mode & AMD_FS_MALLOC_MODE,
					(malloc_mode >> AMD_FS_MALLOC_MEM_BIT) & 1,
					p0, -1);
		}
	}

	if (work == NULL) {
		amSystemLog("Read '%s' in CNT%d.\n", file_name, handle_id);

//		DCInvalidateRange(*buf, size);
		DCFlushRange(*buf, size);

		read_length		= CNTRead(&file, *buf, size);
		if (read_length < length)
			length			= read_length;

		CNTClose(&file);
	} else {
		if (move)
			wbuf	= (void *)((Uint8 *)work + ((length + 31) & ~31));
		else
			wbuf	= *buf;

		switch (malloc_mode & AMD_FS_MALLOC_COMP) {
			case	AMD_FS_MALLOC_COMP_LZ77:
				CXUncompressLZ(work, wbuf);
				break;
			case	AMD_FS_MALLOC_COMP_HUFF:
				CXUncompressHuffman(work, wbuf);
				break;
		}
		DCFlushRange(wbuf, size);

		if (move)
			memcpy(*buf, wbuf, size);
		else {
			if (malloc_mode & AMD_FS_MALLOC_SYSTEM)
				amMemFreeSystem(work);
			else
				amMemFree(work);
		}
		length		= size;
	}

	return	length;
#else
	UNREFERENCED_PARAMETER(handle_id);
	UNREFERENCED_PARAMETER(file_name);
	UNREFERENCED_PARAMETER(buf);
	UNREFERENCED_PARAMETER(malloc_mode);

	return	0;
#endif
}


/*****************************************************************************/
/* AMS_FS_NAME *amFsSetNameTable(Sint32 afs_id, AMS_FS_NAME *name)           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  afs_id : AFS_ID                                                  */
/*          name   : ネームテーブルデータ                                    */
/* [RETURN] 設定されていたネームテーブルデータへのポインタ                   */
/* [FUNCTION]  AFS内のファイルに名前でアクセス可能にするためのテーブル登録   */
/*****************************************************************************/
AMS_FS_NAME *amFsSetNameTable(Sint32 afs_id, AMS_FS_NAME *name)
{
#if AMD_USE_CRIFS
	UNREFERENCED_PARAMETER(afs_id);
	UNREFERENCED_PARAMETER(name);

	amAssert(0);
	return	NULL;
#else
	AMS_FS_NAME	*ret;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	ret		= _am_fs_partition[afs_id].name;
	_am_fs_partition[afs_id].name		= name;

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	return	ret;
#endif
}


/*****************************************************************************/
/* void amFsInitRequest(Sint32 close)                                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  close : 即時にクローズするかどうかのフラグ                       */
/* [FUNCTION]  全要求の取り消し                                              */
/*****************************************************************************/
void amFsInitRequest(Sint32 close)
{
	AMS_FS		*cdfsp;
	Sint32			i;

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

#if 0
	/* 先頭キューの開放 */
	cdfsp			= _am_fs_ptr;
	if (cdfsp != NULL)
		amFsClearRequest(cdfsp, close);

	/* 全ワークの開放 */
	cdfsp			= _am_fs;
	for (i = 0; i < AMD_FS_MAX_FILES; i++, cdfsp++) {
		if (cdfsp->buf != NULL)
			amFree(cdfsp->buf);
	}
#else
	/* 全キューの開放 */
	cdfsp			= _am_fs;
	for (i = 0; i < AMD_FS_MAX_FILES; i++, cdfsp++) {
		if (cdfsp->file_name[0])
			amFsClearRequest(cdfsp, close);
	}
#endif

	_am_fs_filelist		= NULL;
	_am_fs_file_ptr		= NULL;

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif

	/* システム初期化 */
//	amFsReset();
}


/*****************************************************************************/
/* void amFsClearRequest(AMS_FS *cdfsp, Sint32 close)                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  cdfsp : 要求を取り消すキューバッファへのポインタ                 */
/*          close : 即時にクローズするかどうかのフラグ                       */
/* [FUNCTION]  読み込み要求の取り消し                                        */
/*             読み込み用バッファ、キャッシュも開放するので、必要であれば    */
/*             ポインタのコピーを取った上で、NULLを設定すること              */
/*****************************************************************************/
void amFsClearRequest(AMS_FS *cdfsp, Sint32 close)
{
	UNREFERENCED_PARAMETER(close);

#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	/* 状態を復帰 */
	if (_am_fs_ptr == cdfsp) {
		if (_am_fs_check == AMD_FS_SEQ_OPEN)
			_am_fs_check		= AMD_FS_SEQ_NORMAL;
	}

	/* 状態の確認 */
	switch (cdfsp->stat) {
		case	AMD_FS_STAT_STOP:
		case	AMD_FS_STAT_READ:
		case	AMD_FS_STAT_OPEN:
			switch (cdfsp->type) {
				case	AMD_FS_TYPE_FILELIST:
					if (_am_fs_filelist == cdfsp) {
						_am_fs_filelist		= NULL;
						amFsClearRequest(_am_fs_file_ptr, close);
						_am_fs_file_ptr		= NULL;
						break;
					}
					/* no break */

				case	AMD_FS_TYPE_FILE:
#if AMD_USE_CRIFS
					criFsLoader_Stop(_am_fs_loader);
#else
					amFsSC_Cancel(_am_fs_fctrl);
#endif

#if AMD_USE_CRIFS
#if AMD_FS_DELAY_CLOSE
					if (!close && ADXF_GetStatRead(_am_fs_adxf)) {
						/* 遅延クローズ */
						if (_am_fs_close_handle_num >= AMD_FS_MAX_CLOSE) {
							amSystemLog("CDFS: Close buffer over.\n");
							for (;;) ;
						}
						_am_fs_close_handle_num++;
						for (Sint32 i = 0;; i++) {
							if (_am_fs_close_handle[i] == NULL) {
								_am_fs_close_handle[i]	= _am_fs_adxf;
								if (cdfsp->buf_delete) {
									_am_fs_close_handle_mem[i]	= cdfsp->buf;
									cdfsp->buf		= NULL;
								}
								break;
							}
						}
					} else {
						/* 即時クローズ */
						ADXF_Close(_am_fs_adxf);
					}
#else
//					ADXF_Close(_am_fs_adxf);
#endif
#else
					amFsSC_Close(_am_fs_fctrl, 1);
#endif
					break;
			}
			/* no break */
		case	AMD_FS_STAT_WAIT:
		case	AMD_FS_STAT_ERR:
		case	AMD_FS_STAT_CANTOPEN:
		case	AMD_FS_STAT_NOMEMORY:
			amFsFreeLink(cdfsp);
			break;
	}

	/* キャッシュの開放 */
	if ((Sint32)cdfsp->cache > 0)
		amFsFreeCache(cdfsp->cache);

	/* バッファの開放 */
	amFsReleaseBuffer(cdfsp);

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif
}


/*****************************************************************************/
/* Sint32 amFsClose(Sint32 wait)                                             */
/*---------------------------------------------------------------------------*/
/* [INPUT]  wait : 遅延クローズバッファが空になるまで待つかどうかのフラグ    */
/* [RETURN]  遅延クローズバッファに存在するハンドル数                        */
/* [FUNCTION]  引数waitの値によって動作が異なります                          */
/*               0 : 遅延クローズバッファに存在するハンドル数を返す          */
/*              !0 : 遅延クローズバッファが空になるまでブロックする          */
/*****************************************************************************/
Sint32 amFsClose(Sint32 wait)
{
	UNREFERENCED_PARAMETER(wait);

#if AMD_USE_CRIFS
#if AMD_FS_DELAY_CLOSE
	Sint32		i;

	if (wait) {
#if AMD_TASK_THREAD_NUM > 1
		amMutexLock(&_am_fs_mutex);
#endif
		for (i = 0; i < AMD_FS_MAX_CLOSE; i++) {
			if (_am_fs_close_handle[i] == NULL)
				continue;
			ADXF_Close(_am_fs_close_handle[i]);
			_am_fs_close_handle[i]		= NULL;
			_am_fs_close_handle_num--;
			if (_am_fs_close_handle_mem[i] != NULL) {
				amFree(_am_fs_close_handle_mem[i]);
				_am_fs_close_handle_mem[i]	= NULL;
			}
		}
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
	}

	return	_am_fs_close_handle_num;
#else
	return	0;
#endif
#else
	return	0;
#endif
}


/*****************************************************************************/
/* void amFsFreeCache(AMS_FS_CACHE *cache)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  cache : 開放するキャッシュバッファへのポインタ                   */
/* [FUNCTION]  キャッシュの開放                                              */
/*****************************************************************************/
void amFsFreeCache(AMS_FS_CACHE *cache)
{
#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif

	cache->refs--;
	if (cache->refs <= 0)
		amFsReleaseCacheBuffer(cache);

#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif
}


/*****************************************************************************/
/* void amFsReadRequest(void)                                                */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  ロード要求の発行                                              */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
void amFsReadRequest(void)
{
	AMS_FS		*cdfsp;
	AMS_FS_CACHE	*cache;
	AMS_FS_PARTITION	*partition;

	cdfsp		= _am_fs_ptr;

	/* ディスク確認シーケンスチェック */
	if (_am_fs_check != AMD_FS_SEQ_NORMAL)
		return;

	/* キューの存在確認 */
	if (cdfsp == NULL)
		return;

	/* 先頭キューの状態確認 */
	if (cdfsp->stat != AMD_FS_STAT_WAIT)
		return;

#if AMD_USE_CRIFS & 0
	if (_am_fs_device & AMD_DEVICE_DISC) {
		/* コマンドが終了していない */
		if (sceCdSync(1) == 1)
			return;

		/* ドライブが準備できていない */
		if (sceCdDiskReady(1) == SCECdNotReady)
			return;
	}
#endif

	switch (cdfsp->type) {
		case	AMD_FS_TYPE_PARTITION:
			/* パーティション情報のロード */
			partition		= &_am_fs_partition[cdfsp->file_id];
			if (!(partition->flag & AMD_FS_PFLG_READ) ||
					strncmp((char *)partition->read_name, (char *)cdfsp->file_name, 57)) {
				strncpy((char *)partition->read_name, (char *)cdfsp->file_name, 57);
				partition->flag		&= ~(AMD_FS_PFLG_READ
						| AMD_FS_PFLG_CNT | AMD_FS_PFLG_CNT_ID);
				partition->flag		|= AMD_FS_PFLG_RELOAD;
#if AMD_USE_CRIFS
				// バインダーの作成
				if (partition->binder_id != NULL) {
					criFsBinder_Unbind(partition->binder_id);
					partition->binder_id	= NULL;
				}
				if (partition->binder_work != NULL) {
#if AMD_USE_BINDER_HEAP
					criHeap_Free(_am_cri_heap, partition->binder_work);
					partition->binder_work	= NULL;
#else
					amMemFreeSystem(partition->binder_work);
#endif
				}
				if (partition->binder == NULL)
					criFsBinder_Create(&partition->binder);
#endif

#if AMD_FS_DEVICE_NAME
				char		fname[MAX_PATH];
				if (_am_fs_device & AMD_DEVICE_ADDNAME) {
					strcpy(fname, _am_fs_device_name);
					strcat(fname, cdfsp->file_name);
				} else
					strcpy(fname, cdfsp->file_name);
#endif
#if AMD_USE_CRIFS
#if _WII && _WIIWARE
				if (cdfsp->read_size & AMD_FS_FLAG_CNT) {
					criFs_SetCntHandle_WII(&_am_fs_cnt_handle[
							(cdfsp->read_size & AMD_FS_FLAG_CNT_ID) >> 28]);
				}
#endif
				if (cdfsp->read_size & AMD_FS_FLAG_DIR) {
					criFsBinder_GetWorkSizeForBindDirectory(NULL, fname,
							(CriSint32*)&partition->binder_worksize);
					partition->binder_worksize		+= cdfsp->size;
#if AMD_USE_BINDER_HEAP
					partition->binder_work	=
							criHeap_AllocFix(_am_cri_heap, partition->binder_worksize,
									"binder_work", 64);
#else
					partition->binder_work	=
							amMemAllocSystem(partition->binder_worksize);
#endif
					criFsBinder_BindDirectory(partition->binder, NULL, fname,
							partition->binder_work, partition->binder_worksize,
							&partition->binder_id);
				} else {
					criFsBinder_GetWorkSizeForBindCpk(NULL, fname,
							(CriSint32*)&partition->binder_worksize);
					partition->binder_worksize		+= cdfsp->size;
#if AMD_USE_BINDER_HEAP
					partition->binder_work	=
							criHeap_AllocFix(_am_cri_heap, partition->binder_worksize,
									"binder_work", 64);
#else
					partition->binder_work	=
							amMemAllocSystem(partition->binder_worksize);
#endif
					criFsBinder_BindCpk(partition->binder, NULL, fname,
							partition->binder_work, partition->binder_worksize,
							&partition->binder_id);
				}
#if _WII && _WIIWARE
				if (cdfsp->read_size & AMD_FS_FLAG_CNT)
					criFs_SetCntHandle_WII(&_am_fs_cnt_handle[_am_fs_cnt_default]);
#endif
				cdfsp->stat		= AMD_FS_STAT_READ;
#else
				switch (amFsSC_Open(_am_fs_fctrl, fname)) {
					case	AMD_FS_REQ_OK:
						cdfsp->stat		= AMD_FS_STAT_OPEN;
						break;
					case	AMD_FS_REQ_END:
						amFsReadStart();
						break;
					case	AMD_FS_REQ_CANCEL:
						break;
				}
#endif
			} else {
				cdfsp->stat		= AMD_FS_STAT_COMPLETE;
				amFsFreeLink(cdfsp);
				if (cdfsp->read_size)
					amFsClearRequest(cdfsp);
				amFsReadRequest();
				return;
			}
			break;

		case	AMD_FS_TYPE_FILE:
		case	AMD_FS_TYPE_FILELIST:

			/* キャッシュ */
			if (cdfsp->cache != NULL) {
				cache		= amFsSearchCacheBuffer(cdfsp);
				/* キャッシュヒット */
				if (cache != NULL) {
					cache->refs++;
					cdfsp->cache		= cache;
					cdfsp->buf_delete	= 0;
					cdfsp->length		= cache->length;
					cdfsp->size			= cache->size;
					cdfsp->read_size	= cache->read_size;
					cdfsp->buf			= cache->buf;
					cdfsp->stat			= (Sint16)cache->stat;
					amFsFreeLink(cdfsp);
					amFsReadRequest();
					return;
				}
				/* キャッシュミス */
				cache		= amFsGetCacheBuffer();
				cdfsp->cache		= cache;
				cache->type			= cdfsp->type;
				cache->buf_delete	= 0;
				strncpy(cache->file_name, cdfsp->file_name, 57);
				cache->refs			= 1;
				cache->file_id		= cdfsp->file_id & 0xffff;
				cache->buf			= cdfsp->buf;
				cache->stat			= cdfsp->stat;
			}

			/* ファイルオープン */
#if !AMD_USE_CRIFS
			if (cdfsp->file_id == -1) {
				Sint32		file_id;
				file_id		= amFsSearchName(cdfsp->file_name);
				if (file_id != -1) {
					cdfsp->file_id		= file_id;
					strncpy(cdfsp->file_name, _am_fs_partition[file_id >> 16].last_name, 57);
				}
			}

			if (cdfsp->file_id == -1) {
#if AMD_FS_DEVICE_NAME
				if (_am_fs_device & AMD_DEVICE_ADDNAME) {
					Sint8		fname[MAX_PATH];
					strcpy(fname, _am_fs_device_name);
					strcat(fname, cdfsp->file_name);
					if (!cdfsp->open_only) {
						if (amFsSC_Open(_am_fs_fctrl, fname) == AMD_FS_REQ_OK) {
							cdfsp->stat		= AMD_FS_STAT_OPEN;
							return;
						}
					} else {
						if (amFsSC_PreOpen(_am_fs_fctrl, fname) == AMD_FS_REQ_OK) {
							cdfsp->stat		= AMD_FS_STAT_OPEN;
							return;
						}
					}
				} else
#endif
				if (!cdfsp->open_only) {
					if (amFsSC_Open(_am_fs_fctrl, cdfsp->file_name)
							== AMD_FS_REQ_OK) {
						cdfsp->stat		= AMD_FS_STAT_OPEN;
						return;
					}
				} else {
					if (amFsSC_PreOpen(_am_fs_fctrl, cdfsp->file_name)
							== AMD_FS_REQ_OK) {
						cdfsp->stat		= AMD_FS_STAT_OPEN;
						return;
					}
				}
			} else {
				if (amFsSC_Open(_am_fs_fctrl, cdfsp->file_id) == AMD_FS_REQ_OK) {
					cdfsp->stat		= AMD_FS_STAT_OPEN;
					return;
				}
			}
#endif

			amFsReadStart();
			break;
	}
}


/*****************************************************************************/
/* void amFsVint(void)                                                       */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  VINT内実行関数（１イントに１回実行）                          */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
void amFsVint(void)
{
	AMS_FS		*cdfsp;
	AMS_FS_CACHE	*cache;
	AMS_FS_PARTITION	*partition;
//	Sint32			ret, stat, i;

	cdfsp		= _am_fs_ptr;

#if AMD_FS_DEBUG
//	strcpy(_am_fs_error, "");
//	strcpy(_am_fs_error, _am_fs_status[cdfsp->stat]);
	// 無効にしたい場合は _am_fs_display_mode に 0 を入れてください
	if (cdfsp != NULL) {
		sprintf(_am_fs_error, "%s %s %s (%d)",
			_am_fs_sequence[_am_fs_check],
			_am_fs_status[cdfsp->stat],
			cdfsp->file_name,
			(cdfsp->file_id != -1)? cdfsp->file_id & 0xffff: -1);
	} else {
		strcpy(_am_fs_error, _am_fs_sequence[_am_fs_check]);
	}
#endif


#if AMD_FS_DELAY_CLOSE
	/* 遅延クローズ処理 */
{
	Sint32		num;
	ADXF		adxf;
	num			= _am_fs_close_handle_num;
	for (i = 0; num > 0; i++) {
		if ((adxf = _am_fs_close_handle[i]) != NULL) {
			num--;
			if (ADXF_GetStatRead(adxf) == 0) {
				ADXF_Close(adxf);
				_am_fs_close_handle[i]		= NULL;
				if (_am_fs_close_handle_mem[i] != NULL) {
					amFree(_am_fs_close_handle_mem[i]);
					_am_fs_close_handle_mem[i]	= NULL;
				}
				_am_fs_close_handle_num--;
			}
		}
	}
}
#endif

#if 0
	/* 状態別処理 */
	switch (_am_fs_check) {
		case	AMD_FS_SEQ_NORMAL:				/* 正常 */
			break;
		case	AMD_FS_SEQ_OPEN:					/* オープン待ち */
			ret		= sceCdStatus();
			if (ret == SCECdStatShellOpen)
				_am_fs_check		= AMD_FS_SEQ_CLOSE;
			return;
		case	AMD_FS_SEQ_CLOSE:					/* クローズ待ち */
			ret		= sceCdStatus();
			if ((ret != SCECdStatShellOpen) && (ret != -1) && (ret != SCECdStatEmg)) {
				_am_fs_check		= AMD_FS_SEQ_CHECK;
			}
			return;
		case	AMD_FS_SEQ_CHECK:					/* チェック中 */
			ret		= sceCdStatus();
			if (ret == SCECdStatShellOpen) {
				_am_fs_check		= AMD_FS_SEQ_CLOSE;
				return;
			}
			ret		= sceCdDiskReady(1);
			if (ret != SCECdComplete)
				return;
			_am_fs_check		= AMD_FS_SEQ_NORMAL;

#if 0
			/* パーティション情報の再読み込み */
			partition		= &_am_fs_partition[0];
			for (i = 0; i < AMD_FS_MAX_AFS; i++, partition++) {
				if (partition->flag & AMD_FS_PFLG_RELOAD) {
					_am_fs_insert		= 1;
					partition->flag		&= ~AMD_FS_PFLG_READ;
					amFsLoadPartition(i, partition->read_name, 1);
					_am_fs_insert		= 0;
				}
			}
#endif
			return;
	}
#endif

	/* キューの存在確認 */
	if (cdfsp == NULL)
		return;

	/* 先頭キューの状態確認 */
	if (cdfsp->stat == AMD_FS_STAT_WAIT) {
		amFsReadRequest();
		return;
	}
	if ((cdfsp->stat >= AMD_FS_STAT_ERR) && (cdfsp->stat != AMD_FS_STAT_OPEN))
		return;

	cache		= cdfsp->cache;

	/* 状態の取得 */
#if AMD_USE_CRIFS
	switch (cdfsp->type) {
		case	AMD_FS_TYPE_FILE:
		case	AMD_FS_TYPE_FILELIST:
{
			CriFsLoaderStatus	status;
			criFsLoader_GetStatus(_am_fs_loader, &status);
			switch (status) {
				case	CRIFSLOADER_STATUS_LOADING:
					cdfsp->stat		= AMD_FS_STAT_READ;
					break;
				case	CRIFSLOADER_STATUS_STOP:
					cdfsp->stat		= AMD_FS_STAT_STOP;
					break;
				case	CRIFSLOADER_STATUS_COMPLETE:
					cdfsp->stat		= AMD_FS_STAT_COMPLETE;
					break;
				case	CRIFSLOADER_STATUS_ERROR:
					cdfsp->stat		= AMD_FS_STAT_ERR;
					break;
			}
			if (cache != NULL)
				cache->stat		= cdfsp->stat;
}
			break;
		case	AMD_FS_TYPE_PARTITION:
{
			CriFsBinderStatus		status;
			partition	= &_am_fs_partition[cdfsp->file_id];
			criFsBinder_GetStatus(partition->binder_id, &status);
			switch (status) {
				case	CRIFSBINDER_STATUS_ANALYZE:
					cdfsp->stat		= AMD_FS_STAT_READ;
					break;
				case	CRIFSBINDER_STATUS_COMPLETE:
					cdfsp->stat		= AMD_FS_STAT_COMPLETE;
					break;
				default:
					cdfsp->stat		= AMD_FS_STAT_ERR;
					break;
			}
}
			break;
	}
#else
	switch (amFsSC_GetStat(_am_fs_fctrl)) {
		case	AMD_FSCTRL_STAT_READY:
			if (cdfsp->stat != AMD_FS_STAT_OPEN)
				cdfsp->stat		= AMD_FS_STAT_COMPLETE;
			break;
		case	AMD_FSCTRL_STAT_WAIT:
//			cdfsp->stat		= AMD_FS_STAT_STOP;
//			break;
		case	AMD_FSCTRL_STAT_EXEC:
			if (cdfsp->stat == AMD_FS_STAT_OPEN)
				return;
			cdfsp->stat		= AMD_FS_STAT_READ;
			break;
		case	AMD_FSCTRL_STAT_ERROR:
		case	AMD_FSCTRL_STAT_NONE:
		default:
			cdfsp->stat		= AMD_FS_STAT_ERR;
			break;
	}
#endif

	/* 状態別処理 */
	switch (cdfsp->stat) {
		case	AMD_FS_STAT_COMPLETE:				/* 完了 */
			switch (cdfsp->type) {
				case	AMD_FS_TYPE_FILE:		/* ファイル */
#if AMD_USE_CRIFS
					// 圧縮ファイルは分割読みできない
					cdfsp->read_size		+= cdfsp->length;
					if (1) {									/* 全セクタ完了 */
#else
					cdfsp->read_size		+= AMD_FS_READ_SIZE;
					if (cdfsp->read_size >= cdfsp->length) {	/* 全セクタ完了 */
#endif
#if AMD_USE_CRIFS
//						ADXF_Close(_am_fs_adxf);
#else
						amFsSC_Close(_am_fs_fctrl, 1);
#endif
#if _WII
						DCStoreRange(cdfsp->buf, cdfsp->length);
#endif
						if (_am_fs_filelist == NULL) {
							amFsFreeLink(cdfsp);
							if (cache != NULL) {
								cache->buf_delete	= cdfsp->buf_delete;
								cdfsp->buf_delete	= 0;
								cache->length		= cdfsp->length;
								cache->size			= cdfsp->size;
								cache->read_size	= cdfsp->read_size;
							}
						} else {
							amFsFreeLink(cdfsp);
							amFsReleaseBuffer(cdfsp);
							amFsReadListNext(_am_fs_filelist);
						}
#if !AMD_FS_COMMAND_VINT
						amFsReadRequest();
#endif
					} else {									/* 次のセクタへ */
						Sint32		size;
						size		= cdfsp->length - cdfsp->read_size;
						if (size > AMD_FS_READ_SIZE)
							size		= AMD_FS_READ_SIZE;
#if AMD_USE_CRIFS
						CriFsBinderHn	binder;
						char			fname[MAX_PATH];

						if (cdfsp->file_id == -1) {
							binder		= *_am_fs_binder_default;
#if AMD_FS_DEVICE_NAME
							if ((_am_fs_device & AMD_DEVICE_ADDNAME) && (binder == NULL)) {
								strcpy(fname, _am_fs_device_name);
								strcat(fname, cdfsp->file_name);
							} else
#endif
								strcpy(fname, cdfsp->file_name);
						} else {
							binder		= _am_fs_partition[cdfsp->file_id >> 16].binder;
							strcpy(fname, cdfsp->file_name);
						}
						if ((cdfsp->file_id & 0xffff) == 0xffff) {
							criFsLoader_Load(_am_fs_loader, binder, fname,
									cdfsp->read_size, size,
									(char *)cdfsp->buf + cdfsp->read_size,
									cdfsp->size * AMD_FS_READ_SECTOR_SIZE - cdfsp->read_size);
						} else {
							criFsLoader_LoadById(_am_fs_loader, binder,
									cdfsp->file_id & 0xffff,
									cdfsp->read_size, size,
									(char *)cdfsp->buf + cdfsp->read_size,
									cdfsp->size * AMD_FS_READ_SECTOR_SIZE - cdfsp->read_size);
						}
#else
						amFsSC_Read(_am_fs_fctrl,
								(Uint8 *)cdfsp->buf + cdfsp->read_size, size);
#endif
						cdfsp->stat	= AMD_FS_STAT_READ;
					}
					break;
				case	AMD_FS_TYPE_PARTITION:	/* パーティション */
					partition	= &_am_fs_partition[cdfsp->file_id];
#if !AMD_USE_CRIFS
					amFsSC_Close(_am_fs_fctrl, 1);
#endif
					partition->flag		&= ~AMD_FS_PFLG_RELOAD;
					partition->flag		|= AMD_FS_PFLG_READ;
					amFsFreeLink(cdfsp);
					if (cdfsp->read_size)
						amFsClearRequest(cdfsp);
#if !AMD_FS_COMMAND_VINT
					amFsReadRequest();
#endif
					break;
				case	AMD_FS_TYPE_FILELIST:	/* ファイルリストファイル */
{
					Sint32		files, i;
					Sint64		fsize;
					size_t		total;
					char		*cp, fname[MAX_PATH];
					AMS_FS_FILELIST		*listbuf, *list;
					AMS_FS_PARTITION	*partition;
					AMS_AMB_HEADER		*amb;
					AMS_AMB_FILE		*file;
					AMS_AMB_DEBUG		*debug;
#if AMD_USE_CRIFS
					CriFsBinderHn		binder;
#endif

					files		= 0;
					cp			= (char *)cdfsp->buf;

					cp			= amFsGetColumn(cp);
					while (cp != NULL) {
						files++;
						cp			= amFsGetNextColumn(cp);
					}

					listbuf		= (AMS_FS_FILELIST *)amMemAllocTemp(sizeof(AMS_FS_FILELIST) * files);
					cp			= (char *)cdfsp->buf;
					list		= listbuf;
					total		= sizeof(AMS_AMB_HEADER)
								+ (sizeof(AMS_AMB_DEBUG) + sizeof(AMS_AMB_FILE) + 64) * files;
					total		= (total + 63) & ~63;

#if AMD_USE_CRIFS
					if (cdfsp->file_id == -1) {
						partition	= NULL;
						binder		= *_am_fs_binder_default;
					} else {
						partition	= &_am_fs_partition[cdfsp->file_id >> 16];
						binder		= partition->binder;
					}
#else
					if (cdfsp->file_id == -1)
						partition		= NULL;
					else
						partition		= &_am_fs_partition[cdfsp->file_id >> 16];
#endif

					cp			= amFsGetColumn(cp);
					for (i = 0; i < files; i++, list++) {
						cp			= amFsGetString(cp, list->path);
						list->name	= list->path + strlen(list->path);
						while ((list->name != list->path) && (list->name[-1] != '\\'))
							list->name--;
						cp			= amFsGetNumber(cp, list->id);
						cp			= amFsGetNumber(cp, list->user_data[0]);
						cp			= amFsGetNumber(cp, list->user_data[1]);
						cp			= amFsGetNumber(cp, list->user_data[2]);
						cp			= amFsGetNextColumn(cp);

						/* ファイルサイズの取得 */
#if AMD_USE_CRIFS
						if (partition == NULL) {
#if AMD_FS_DEVICE_NAME
							if ((_am_fs_device & AMD_DEVICE_ADDNAME) && (binder == NULL)) {
								strcpy(fname, _am_fs_device_name);
								strcat(fname, list->path);
							} else
#endif
								strcpy(fname, list->path);
							criFsBinder_GetFileSize(binder, fname, &fsize);
						} else {
							strcpy(fname, list->path);
#if _WII && _WIIWARE
							if (!(partition->flag & AMD_FS_PFLG_CNT)) {
#endif
								criFsBinder_GetFileSize(binder, fname, &fsize);
#if _WII && _WIIWARE
							} else {
								criFs_SetCntHandle_WII(&_am_fs_cnt_handle[
										(partition->flag & AMD_FS_PFLG_CNT_ID) >> 28]);
								criFsBinder_GetFileSize(binder, fname, &fsize);
								criFs_SetCntHandle_WII(&_am_fs_cnt_handle[_am_fs_cnt_default]);
							}
#endif
						}
#else
						if (partition == NULL)
							fsize			= _am_fs_fctrl->file.size;
						else
							fsize			= ((AMS_AFS_HEADER *)partition->info)->file[cdfsp->file_id & 0xffff].size;
#endif
						if (fsize < 0)
							fsize			= 0;
						list->length	= fsize;
						list->offset	= total;
						total		+= (fsize + AMD_FS_READ_SECTOR_SIZE - 1) & ~((1 << AMD_FS_READ_SECTOR_BIT) - 1);
					}

					if (cdfsp->buf_delete) {
						amMemFree(cdfsp->buf);
						cdfsp->buf			= (void *)amMemAlloc(total);
					}
					amb			= (AMS_AMB_HEADER *)cdfsp->buf;

					memset(amb, 0, sizeof(AMS_AMB_HEADER));
					strcpy(amb->file_id, "#AMB");
					amb->header_size	= sizeof(AMS_AMB_HEADER);
					amb->file_num		= files;
					amb->file			= (AMS_AMB_FILE *)sizeof(AMS_AMB_HEADER);
					amb->debug			= (AMS_AMB_DEBUG *)(sizeof(AMS_AMB_HEADER) + sizeof(AMS_AMB_FILE) * files);

					file	= (AMS_AMB_FILE *)((char *)amb + (Uint32)amb->file);
					debug	= (AMS_AMB_DEBUG *)((char *)amb + (Uint32)amb->debug);
					list	= listbuf;
					cp		= (char *)(debug + files);
					for (i = 0; i < files; i++, file++, debug++, list++, cp += 64) {
						file->data		= (void *)list->offset;
						file->size		= (Sint32)list->length;
						file->type		= list->user_data[0];
						file->user0		= (Sint16)list->user_data[1];
						file->user1		= (Sint16)list->user_data[2];
						strcpy(debug->filename, list->name);
						strcpy(cp, list->path);
					}

					amMemFree(listbuf);

					cdfsp->length		= total;
					cdfsp->size			= (total + AMD_FS_READ_SECTOR_SIZE - 1)
							>> AMD_FS_READ_SECTOR_BIT;
					cdfsp->read_size	= 0;

					amFsFreeLink(cdfsp);
					cdfsp->stat			= AMD_FS_STAT_READ;
					amFsReadListNext(cdfsp);
#if !AMD_FS_COMMAND_VINT
					amFsReadRequest();
#endif
}
					break;
			}
			break;

#if !AMD_USE_CRIFS
		case	AMD_FS_STAT_OPEN:					/* オープン完了 */
			if (!cdfsp->open_only)
				amFsReadStart();
			else {
				amFsSC_Close(_am_fs_fctrl, 1);
				amFsFreeLink(cdfsp);
				if (cdfsp->read_size)
					amFsClearRequest(cdfsp);
#if !AMD_FS_COMMAND_VINT
				amFsReadRequest();
#endif
			}
			break;
#endif

		case	AMD_FS_STAT_STOP:
		case	AMD_FS_STAT_READ:					/* 読み込み中 */
			break;

		case	AMD_FS_STAT_ERR:					/* エラー */
		default:
			if (cdfsp->type == AMD_FS_TYPE_FILE) {
#if AMD_USE_CRIFS
				criFsLoader_Stop(_am_fs_loader);
#else
				amFsSC_Cancel(_am_fs_fctrl);
#endif

#if AMD_USE_CRIFS
#if AMD_FS_DELAY_CLOSE
				if (_am_fs_close_handle_num >= AMD_FS_MAX_CLOSE) {
					printf("CDFS: Close buffer over.\n");
					for (;;) ;
				}
				_am_fs_close_handle_num++;
				for (Sint32 i = 0;; i++) {
					if (_am_fs_close_handle[i] == NULL) {
						_am_fs_close_handle[i]	= _am_fs_adxf;
						if (cdfsp->buf_delete) {
							_am_fs_close_handle_mem[i]	= cdfsp->buf;
							cdfsp->buf		= NULL;
						}
						break;
					}
				}
#else
//				ADXF_Close(_am_fs_adxf);
#endif
#else
				amFsSC_Close(_am_fs_fctrl, 1);
#endif
			}
			if ((Sint32)cache > 0) {
				amFsFreeCache(cache);
				cdfsp->cache	= (AMS_FS_CACHE *)-1;
			}
			if ((cdfsp->buf_delete != 0) && (cdfsp->buf != NULL)) {
				amMemFree(cdfsp->buf);
				cdfsp->buf		= NULL;
			}
			cdfsp->stat		= AMD_FS_STAT_WAIT;
#if !AMD_FS_OPEN_ERROR
			_am_fs_check	= AMD_FS_SEQ_OPEN;
#else
			_am_fs_check	= AMD_FS_SEQ_NORMAL;
#endif
			break;
	}
}


/*****************************************************************************/
/* void amFsServer(void)                                                     */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  CDFSサーバー関数                                              */
/*****************************************************************************/
void amFsServer(void)
{
#if AMD_USE_CRIFS
	if (_am_cri_run) {
		/* ADX */
//		ADXM_ExecMain();

		criFs_ExecuteMain();
#if _IPHONE
#if AMD_USE_CRIAUDIO
		amCriAudioExcuteMain();
#elif AMD_USE_CRIADX
		// 検証用 : 通常は AppDelegate.m で呼ばれるため不要
		// ADXM_ExecMain();
#endif
#else
		amCriAudioExcuteMain();
#endif

		/* ファイルシステム */
#if AMD_TASK_THREAD_NUM > 1
		amMutexLock(&_am_fs_mutex);
#endif
		amFsVint();
#if AMD_TASK_THREAD_NUM > 1
		amMutexUnlock(&_am_fs_mutex);
#endif
	}
#else
	amFsSC_Server();
#if AMD_TASK_THREAD_NUM > 1
	amMutexLock(&_am_fs_mutex);
#endif
	amFsVint();
#if AMD_TASK_THREAD_NUM > 1
	amMutexUnlock(&_am_fs_mutex);
#endif
#endif
}


/*--- Local Functions -------------------------------------------------------*/

/*****************************************************************************/
/* AMS_FS *amFsGetBuffer(void)                                               */
/*---------------------------------------------------------------------------*/
/* [RETURN]  確保されたキューバッファへのポインタ                            */
/* [FUNCTION]  キューバッファの確保                                          */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
AMS_FS *amFsGetBuffer(void)
{
	/* キューバッファオーバーチェック */
	if (_am_fs_num >= AMD_FS_MAX_FILES) {
#if AMD_DEBUG
		printf("CDFS: Que Buffer Over\n");
		for (;;) ;
#else
		return	NULL;
#endif
	}

	return	_am_fs_buf[_am_fs_num++];
}


/*****************************************************************************/
/* void amFsReleaseBuffer(AMS_FS *cdfsp)                                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]  cdfsp : 開放するキューバッファへのポインタ                       */
/* [FUNCTION]  キューバッファの開放                                          */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
void amFsReleaseBuffer(AMS_FS *cdfsp)
{
	/* ワークの開放 */
	if ((cdfsp->buf_delete != 0) && (cdfsp->buf != NULL))
		amMemFree(cdfsp->buf);

#if AMD_FS_DEBUG
	if (cdfsp->file_name[0] == 0) {
		printf("CDFS : Release Buffer Error\n");
		for (;;) ;
	}
#endif

	cdfsp->buf_delete	= 1;
	cdfsp->file_name[0]	= 0;
	cdfsp->cache		= NULL;
	cdfsp->size			= 0;
	cdfsp->buf			= NULL;
	cdfsp->stat			= AMD_FS_STAT_WAIT;
	cdfsp->prevp		= NULL;
	cdfsp->nextp		= NULL;

	_am_fs_buf[--_am_fs_num]	= cdfsp;
}


/*****************************************************************************/
/* void amFsFreeLink(AMS_FS *cdfsp)                                          */
/*---------------------------------------------------------------------------*/
/* [INPUT]  cdfsp : リンクから切り離すキューバッファへのポインタ             */
/* [FUNCTION]  リンクからキューバッファを切り離す                            */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
void amFsFreeLink(AMS_FS *cdfsp)
{
	if (cdfsp->nextp != NULL)
		cdfsp->nextp->prevp		= cdfsp->prevp;
	else
		_am_fs_end			= cdfsp->prevp;

	if (cdfsp->prevp != NULL)
		cdfsp->prevp->nextp		= cdfsp->nextp;
	else
		_am_fs_ptr			= cdfsp->nextp;

	cdfsp->prevp		= NULL;
	cdfsp->nextp		= NULL;
}


/*****************************************************************************/
/* AMS_FS_CACHE *amFsGetCacheBuffer(void)                                    */
/*---------------------------------------------------------------------------*/
/* [RETURN]  確保されたキャッシュバッファへのポインタ                        */
/* [FUNCTION]  キャッシュバッファの確保                                      */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
AMS_FS_CACHE *amFsGetCacheBuffer(void)
{
	/* キャッシュバッファオーバーチェック */
	if (_am_fs_cache_num >= AMD_FS_CACHE_MAX_FILES)
		return	NULL;

	return	_am_fs_cache_buf[_am_fs_cache_num++];
}


/*****************************************************************************/
/* void amFsReleaseCacheBuffer(AMS_FS_CACHE *cache)                          */
/*---------------------------------------------------------------------------*/
/* [INPUT]  cache : 開放するキャッシュバッファへのポインタ                   */
/* [FUNCTION]  キャッシュバッファの開放                                      */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
void amFsReleaseCacheBuffer(AMS_FS_CACHE *cache)
{
	/* ワークの開放 */
	if ((cache->buf_delete != 0) && (cache->buf != NULL))
		amMemFree(cache->buf);

#if AMD_FS_DEBUG
	if (cache->file_name[0] == 0) {
		printf("CDFS : Release Cache Buffer Error\n");
		for (;;) ;
	}
#endif

	cache->file_name[0]	= 0;
	cache->refs			= -1;
	cache->buf			= NULL;
	cache->stat			= AMD_FS_STAT_WAIT;

	_am_fs_cache_buf[--_am_fs_cache_num]	= cache;
}


/*****************************************************************************/
/* AMS_FS_CACHE *amFsSearchCacheBuffer(AMS_FS *cdfsp)                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  cache : 検索するキャッシュバッファへのポインタ                   */
/* [RETURN] 該当するキャッシュバッファへのポインタ（該当無しはNULLを返す）   */
/* [FUNCTION]  キャッシュバッファの検索                                      */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
AMS_FS_CACHE *amFsSearchCacheBuffer(AMS_FS *cdfsp)
{
	Sint32			i, id;
	AMS_FS_CACHE	*cp;

	id		= cdfsp->file_id & 0xffff;

	for (i = 0, cp = _am_fs_cache; i < AMD_FS_CACHE_MAX_FILES; i++, cp++) {
		if (cp->file_id != id)
			continue;
		if (strncmp(cp->file_name, cdfsp->file_name, 57))
			continue;
		return	cp;
	}

	return	NULL;
}


/*****************************************************************************/
/* Sint32 amFsSearchPartition(char *pname)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]  pname : 検索するAFSファイル名                                    */
/* [RETURN] パーティションファイル名から該当するパーティションIDを返す       */
/*          該当無しは-1を返す                                               */
/* [FUNCTION]  パーティションの検索                                          */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
Sint32 amFsSearchPartition(char *pname)
{
	Sint32			i;

	for (i = 0; i < AMD_FS_MAX_AFS; i++) {
		if (strncmp(_am_fs_partition[i].last_name, pname, 57))
			continue;
		return	i;
	}

	return	-1;
}


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
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
Sint32 amFsGetPartitionStat(Sint32 afs_id, char *pname)
{
	AMS_FS_PARTITION		*partition;

	partition		= &_am_fs_partition[afs_id];

	if (pname == NULL)
		pname			= partition->last_name;

	if (strncmp(partition->read_name, pname, 57))
		return	AMD_FS_PSTAT_WAIT;

	if (partition->flag & AMD_FS_PFLG_RELOAD)
		return	AMD_FS_PSTAT_READING;

	return	AMD_FS_PSTAT_OK;
}


/*****************************************************************************/
/* Sint32 amFsSearchName(char *fname)                                        */
/*---------------------------------------------------------------------------*/
/* [INPUT]  fname  : ファイル名                                              */
/* [RETURN] ファイルID(上位16ビットはAFS_ID、存在しない場合は-1)             */
/* [FUNCTION]  AFSファイル内に該当するファイル名が存在するか探す             */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
Sint32 amFsSearchName(char *fname)
{
	Sint32		i, id, id0, id1, cp;
	AMS_FS_NAME	*name;

	/* '/'で始まるファイル名は検索対象外 */
	if (fname[0] == '/')
		return	-1;

	for (i = 0; i < AMD_FS_MAX_AFS; i++) {
		if ((name = _am_fs_partition[i].name) == NULL)
			continue;

		id1		= _am_fs_partition[i].files - 1;
		id0		= 0;

		for (;;) {
			id		= (id0 + id1) >> 1;
			cp		= strcmp(fname, name[id].name);
			if (cp == 0)
				return	(i << 16) | (Uint32)name[id].id;
			if (cp > 0)
				id0		= id + 1;
			else
				id1		= id - 1;
			if (id1 < id0)
				break;
		}
	}

	return	-1;
}


/*****************************************************************************/
/* Sint32 amFsReadStart(void)                                                */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  オープン確認後にリード                                        */
/*             TCB外もしくはミューテックスロック状態で呼び出すこと           */
/*****************************************************************************/
Sint32 amFsReadStart(void)
{
	AMS_FS		*cdfsp;
	AMS_FS_CACHE	*cache;
	AMS_FS_PARTITION	*partition;
	Sint32			size;
#if AMD_USE_CRIFS
	CriFsBinderHn	binder;
	Sint64			fsize;
	char			fname[MAX_PATH];
#endif

	cdfsp		= _am_fs_ptr;

#if AMD_USE_CRIFS
	if (_am_fs_loader == NULL) {
#else
	if (!_am_fs_fctrl->file.open_flag) {
#endif
#if AMD_FS_OPEN_ERROR
		cdfsp->stat		= AMD_FS_STAT_CANTOPEN;
#else
		if (cdfsp->cache != NULL) {
			amFsFreeCache(cdfsp->cache);
			cdfsp->cache		= (AMS_FS_CACHE *)-1;
		}
		_am_fs_check	= AMD_FS_SEQ_OPEN;
#endif
		return	0;
	}

	/* ファイルサイズの取得 */
#if AMD_USE_CRIFS
	if (cdfsp->file_id == -1) {
		partition	= NULL;
		binder		= *_am_fs_binder_default;
#if AMD_FS_DEVICE_NAME
		if ((_am_fs_device & AMD_DEVICE_ADDNAME) && (binder == NULL)) {
			strcpy(fname, _am_fs_device_name);
			strcat(fname, cdfsp->file_name);
		} else
#endif
			strcpy(fname, cdfsp->file_name);
			criFsBinder_GetFileSize(binder, fname, &fsize);
	} else {
		partition	= &_am_fs_partition[cdfsp->file_id >> 16];
		binder		= partition->binder;
		strcpy(fname, cdfsp->file_name);
#if _WII && _WIIWARE
		if (!(partition->flag & AMD_FS_PFLG_CNT)) {
#endif
			if ((cdfsp->file_id & 0xffff) == 0xffff)
				criFsBinder_GetFileSize(binder, fname, &fsize);
			else
				criFsBinder_GetFileSizeById(binder, cdfsp->file_id & 0xffff, &fsize);
#if _WII && _WIIWARE
		} else {
			criFs_SetCntHandle_WII(&_am_fs_cnt_handle[
					(partition->flag & AMD_FS_PFLG_CNT_ID) >> 28]);
			criFsBinder_GetFileSize(binder, fname, &fsize);
			criFs_SetCntHandle_WII(&_am_fs_cnt_handle[_am_fs_cnt_default]);
		}
#endif
	}
	if (fsize < 0) {
		cdfsp->stat		= AMD_FS_STAT_CANTOPEN;
		amSystemLog("[ERR] '%s' can't open.\n", fname);
		return	0;
	}
	cdfsp->size		= (Sint32)((fsize + AMD_FS_READ_SECTOR_SIZE - 1)
			>> AMD_FS_READ_SECTOR_BIT);
	cdfsp->length	= cdfsp->size * AMD_FS_READ_SECTOR_SIZE;
#else
	if (cdfsp->type == AMD_FS_TYPE_FILE) {
		if (cdfsp->file_id == -1) {
			partition		= NULL;
			cdfsp->size		=
					(_am_fs_fctrl->file.size + AMD_FS_READ_SECTOR_SIZE - 1)
					>> AMD_FS_READ_SECTOR_BIT;
			cdfsp->length	= _am_fs_fctrl->file.size;
		} else {
			partition		= &_am_fs_partition[cdfsp->file_id >> 16];
			cdfsp->length	= ((AMS_AFS_HEADER *)partition->info)->file[cdfsp->file_id & 0xffff].size;
			cdfsp->size		= (cdfsp->length + AMD_FS_READ_SECTOR_SIZE - 1)
					>> AMD_FS_READ_SECTOR_BIT;
		}
	} else {
		partition		= &_am_fs_partition[cdfsp->file_id];
		cdfsp->length	= (partition->files - 1)
				* sizeof(AMS_AFS_FILE) + sizeof(AMS_AFS_HEADER);
		cdfsp->size		= (cdfsp->length + AMD_FS_READ_SECTOR_SIZE - 1)
				>> AMD_FS_READ_SECTOR_BIT;
		cdfsp->buf		= partition->info;
	}
#endif

	/* バッファの確保 */
	if (cdfsp->buf == NULL) {
#if AMD_DEBUG
{
		char	*p0;
		Sint32	index;
		index		= strlen(cdfsp->file_name) - 1;
		p0			= &cdfsp->file_name[index];
		while ((index > 0) && (p0[-1] != '/')) {
			index--;
			p0--;
		}
		cdfsp->buf		= (void *)amMemDebugAlloc(cdfsp->size * AMD_FS_READ_SECTOR_SIZE,
#if _WII
				cdfsp->malloc_mode & AMD_FS_MALLOC_MODE,
				(cdfsp->malloc_mode >> AMD_FS_MALLOC_MEM_BIT) & 1, p0,
#else
				cdfsp->malloc_mode, AMD_HEAP_DEFAULT, p0,
#endif
				(cdfsp->file_id != -1)? (cdfsp->file_id & 0xffff): -1);
}
#else
		cdfsp->buf		= (void *)amMemDebugAlloc(cdfsp->size * AMD_FS_READ_SECTOR_SIZE,
#if _WII
				cdfsp->malloc_mode & AMD_FS_MALLOC_MODE,
				(cdfsp->malloc_mode >> AMD_FS_MALLOC_MEM_BIT) & 1);
#else
				cdfsp->malloc_mode, AMD_HEAP_DEFAULT);
#endif
#endif
		if (cdfsp->buf == NULL) {
#if AMD_DEBUG
//			_am_fs_check	= AMD_FS_SEQ_OPEN;
			cdfsp->stat			= AMD_FS_STAT_NOMEMORY;
#else
			/* メモリが確保できるまで待つ(≒無限ループ) */
#if AMD_USE_CRIFS
//			ADXF_Close(_am_fs_adxf);
#else
			amFsSC_Close(_am_fs_fctrl, 1);
#endif
#endif
			if (cdfsp->cache != NULL) {
				amFsFreeCache(cdfsp->cache);
				cdfsp->cache		= (AMS_FS_CACHE *)-1;
			}
			return	0;
		}
#if AMD_FS_DEBUG
	} else if ((Uint32)cdfsp->buf & 63) {
		printf("Not 64byte aligned buffer\n");
		for (;;) ;
#endif
	}

	/* 読み込みサイズの計算 */
#if AMD_USE_CRIFS
	// 圧縮ファイルは分割読みできない
	size			= cdfsp->length;
#else
	if ((cdfsp->type == AMD_FS_TYPE_FILE) && (size > AMD_FS_READ_SIZE))
		size			= AMD_FS_READ_SIZE;
#endif

	/* 読み込み要求の発行 */
	amSystemLog("Read '%s'.\n", fname);
#if _WII && _WIIWARE
//	DCInvalidateRange(cdfsp->buf, cdfsp->length);
	DCFlushRange(cdfsp->buf, cdfsp->length);
#endif
#if AMD_USE_CRIFS
	if ((cdfsp->file_id & 0xffff) == 0xffff) {
#if _WII && _WIIWARE
		if ((partition == NULL) || !(partition->flag & AMD_FS_PFLG_CNT)) {
#endif
			criFsLoader_Load(_am_fs_loader, binder, fname,
					0, size, cdfsp->buf, cdfsp->size * AMD_FS_READ_SECTOR_SIZE);
#if _WII && _WIIWARE
		} else {
			criFs_SetCntHandle_WII(&_am_fs_cnt_handle[
					(partition->flag & AMD_FS_PFLG_CNT_ID) >> 28]);
			criFsLoader_Load(_am_fs_loader, binder, fname,
					0, size, cdfsp->buf, cdfsp->size * AMD_FS_READ_SECTOR_SIZE);
			criFs_SetCntHandle_WII(&_am_fs_cnt_handle[_am_fs_cnt_default]);
		}
#endif
	} else {
		criFsLoader_LoadById(_am_fs_loader, binder, cdfsp->file_id & 0xffff,
				0, size, cdfsp->buf, cdfsp->size * AMD_FS_READ_SECTOR_SIZE);
	}
#else
	amFsSC_Read(_am_fs_fctrl, cdfsp->buf, size);
#endif

	cdfsp->stat		= AMD_FS_STAT_READ;

	/* キャッシュに反映 */
	cache			= cdfsp->cache;
	if (cdfsp->cache != NULL) {
		cache->buf		= cdfsp->buf;
		cache->stat		= cdfsp->stat;
	}

	return	1;
}


/*****************************************************************************/
/* void amFsReadListNext(AMS_FS *cdfsp)                                      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  ファイルリスト内ファイルの読み込み                            */
/*****************************************************************************/
void amFsReadListNext(AMS_FS *cdfsp)
{
	if (cdfsp == NULL)
		return;

	if (cdfsp->type != AMD_FS_TYPE_FILELIST)
		return;

	Sint32		index;
	AMS_AMB_HEADER		*amb;
	AMS_AMB_FILE		*file;
	AMS_AMB_DEBUG		*debug;
	char		*fname;
	void		*buf;

	index	= cdfsp->read_size;
	amb		= (AMS_AMB_HEADER *)cdfsp->buf;

	if (index >= amb->file_num) {
		if (_am_fs_filelist == cdfsp) {
			cdfsp->stat			= AMD_FS_STAT_COMPLETE;
			_am_fs_filelist		= NULL;
			_am_fs_file_ptr		= NULL;
		}
		return;
	}

	file	= (AMS_AMB_FILE *)((char *)amb + (Uint32)amb->file);
	debug	= (AMS_AMB_DEBUG *)((char *)amb + (Uint32)amb->debug);
	fname	= (char *)(debug + amb->file_num);
	file	+= index;
	debug	+= index;
	fname	+= 64 * index;
	buf		= (void *)((char *)amb + (Uint32)file->data);

	_am_fs_insert	= 1;
	if (cdfsp->file_id == -1)
		_am_fs_file_ptr		= amFsReadBackground(fname, buf);
	else
		_am_fs_file_ptr		= amFsReadBackground(cdfsp->file_id, fname, buf);
	_am_fs_insert	= 0;

	cdfsp->read_size++;

	_am_fs_filelist		= cdfsp;
}


#if !_XBOX
#define AMD_FS_PATH_CHAR		'/'
#define AMD_FS_PATH_CONV		'\\'
#else
#define AMD_FS_PATH_CHAR		'\\'
#define AMD_FS_PATH_CONV		'/'
#endif

/*****************************************************************************/
/* void amFsConvertPath(char *out_name, char *in_name)                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  in_name  : 変換元ファイル名                                      */
/* [OUTPUT] out_name : 変換結果ファイル名                                    */
/* [FUNCTION]  ファイル名の変換                                              */
/*****************************************************************************/
void amFsConvertPath(char *out_name, char *in_name)
{
	amAssert(out_name);
	amAssert(in_name);

	char	ch;

	for (; (ch = *in_name) != '\0'; in_name++, out_name++) {
		if (ch == AMD_FS_PATH_CONV)
			ch	= AMD_FS_PATH_CHAR;
		*out_name	= ch;
	}
	*out_name	= ch;
}


char *amFsGetColumn(char *cp)
{
	char	ch = *cp;

	while ((ch == '\t') || (ch == ' ')) {
		cp++;
		ch		= *cp;
	}

	if (ch == '$')
		return	NULL;

	if ((ch == 13) || (ch == 10) || (ch == '#') || (ch == '<'))
		return	amFsGetNextColumn(cp);

	return	cp;
}


char *amFsGetNextColumn(char *cp)
{
	while (*cp != 10) {
		if (*cp == '$')
			return	NULL;
		cp++;
	}
	cp++;

	return	amFsGetColumn(cp);
}


char *amFsGetNumber(char *cp, Sint32 &num)
{
	char	ch;

	ch		= *cp;
	while ((ch == '\t') || (ch == ' '))
		ch		= *(++cp);

	if (ch == '#') {
		num		= 0;
		return	cp;
	}

	num		= atoi(cp);

	while ((ch != '\t') && (ch != ' ') && (ch != 13) && (ch != 10))
		ch		= *(++cp);

	return	cp;
}


char *amFsGetString(char *cp, char *str)
{
	char	ch;

	ch		= *cp;
	while ((ch == '\t') || (ch == ' '))
		ch		= *(++cp);

	if (ch != '#') {
		while ((ch != '\t') && (ch != ' ') && (ch != 13) && (ch != 10)) {
			*(str++)	= ch;
			ch		= *(++cp);
		}
	}
	*str	= 0;

	return	cp;
}


#if AMD_USE_CRIFS
static void amFsErrorCallback(const CriChar8 *err_id, CriUint32 p1, CriUint32 p2, CriUint32 *parray)
{
	UNREFERENCED_PARAMETER(parray);

	const char	*msg;

	msg		= criErr_ConvertIdToMessage(err_id, p1, p2);

#if AMD_DEBUG
#if !_WII
	static Sint32	msg_log = 1;
#else
	static Sint32	msg_log = 0;	// WIIは処理落ちするのでデフォルトOFF
#endif
	if (msg_log)
		amSystemLog("%s\n", msg);
#endif

	if (!strncmp(err_id, "E2008020104F", 12)) {
		if (_am_fs != NULL)
			_am_fs->size	+= p1;
	}
}
#endif
