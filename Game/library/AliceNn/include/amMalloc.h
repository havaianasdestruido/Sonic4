/************************************************************************/
/*      amMalloc.h                                                      */
/*            Copyright(c) 2000-2009 Dimps CORP. All Rights Reserved.   */
/*----------------------------------------------------------------------*/
/* メモリ管理ライブラリヘッダ                                           */
/*----------------------------------------------------------------------*/
/************************************************************************/

#ifndef _AM_MALLOC_H_
#define _AM_MALLOC_H_

/*----- Update Logs ----------------------------------------------------*/

/*----- Include files --------------------------------------------------*/

#if _PS3
#include <sys/memory.h>
#elif _WII
#include <revolution/mem.h>
#endif

/*----- Macros ---------------------------------------------------------*/

/* デフォルトヒープサイズ */
#define AMD_HEAP_SIZE	((20 * 1024) * 1024)
#define AMD_HEAP_SIZE2	((15 * 1024) * 1024)

#define AMD_HEAP_FLAG_TEMPORARY (0x00000001)

#if !_WII
#define AMD_HEAP_NUM		(1)
#define AMD_HEAP_DEFAULT	(0)
#else
#define AMD_HEAP_NUM		(2)
#define AMD_HEAP_DEFAULT	(1)
#endif


/*----- Enum definitions -----------------------------------------------*/

// コンフィグ
enum _ame_malloc_config_ {
	AMD_MALLOCMODE_FIRSTFIT = 0,
	AMD_MALLOCMODE_LASTFIT,
	AMD_MALLOCMODE_LOW,
	AMD_MALLOCMODE_HIGH,

	AMD_FILENAME_LENGTH = 16,
};

/*----- Type definitions -----------------------------------------------*/

// 確保失敗時のコールバック
typedef void (*MallocErrorCallback)(void);

// heap
typedef struct _ams_heap_head_ AMS_HEAP_HEAD;
typedef struct _ams_heap_tail_ AMS_HEAP_TAIL;

// heap header
struct _ams_heap_head_ {
	Sint32	size;			// ブロックサイズ (負数は使用サイズ)

	// 確保したソースファイル名
	char	fileName[AMD_FILENAME_LENGTH];

	AMS_HEAP_TAIL	*pPrev;	// 前のブロックの Tail
	AMS_HEAP_TAIL	*pTail;	// このブロックの Tail

	Sint32	checkCode;		// ヒープ破壊チェッカ
}; // 32bytes

// heap tail
struct _ams_heap_tail_ {
	Sint32	checkCode;		// ヒープ破壊チェッカ
	Sint32	line;			// 確保した行

	AMS_HEAP_HEAD	*pHead;	// このブロックの Head
	AMS_HEAP_HEAD	*pNext;	// 次のブロックの Head

	Sint32	flag;			// フラグ
	Sint32	heap;			// ヒープID
	Sint8	padding[8];		// もたーいない
}; // 32bytes

// ヒープ管理
typedef struct {
	AMS_HEAP_HEAD	*head;
	AMS_HEAP_TAIL	*tail;
	Sint32			count;	// ヒープブロックカウンタ
	Sint32			count_normal;
	Sint32			total;	// ヒープブロックサイズ
	Sint8			*buf;
	Sint32			all;
	Sint32			swap;
#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
	AMS_MUTEX		mutex;
#endif
} AMS_HEAP_MANAGER;

// ヒープメーター表示パラメータ
typedef struct {
	Sint32	block_num;		// ブロック数
	float	x0, x1;			// 横座標
} AMS_HEAP_MAP_HEADER;

typedef struct {
	Uint32	color;			// 色
	float	y0, y1;			// 縦座標
} AMS_HEAP_MAP_BLOCK;


/*----- External variables ---------------------------------------------*/

extern AMS_HEAP_MANAGER		_am_heap_manager[AMD_HEAP_NUM];

#if _WII
extern MEMHeapHandle		_am_heap_handle[2];
#endif


/*----- External functions ---------------------------------------------*/

/************************************************************************/
/* void *amMemAllocSystem(size_t size, Sint32 heap)                     */
/*----------------------------------------------------------------------*/
/* [INPUT] size : OSから確保するメモリサイズ                            */
/*         heap : OSから確保するヒープの種類                            */
/* [FUNCTION] OSからのメモリ確保                                        */
/************************************************************************/
void *amMemAllocSystem(size_t size, Sint32 heap = AMD_HEAP_DEFAULT);

/************************************************************************/
/* void amMemFreeSystem(void *ptr, Sint32 heap)                         */
/*----------------------------------------------------------------------*/
/* [INPUT] ptr  : 解放するメモリ                                        */
/*         heap : 解放するヒープの種類                                  */
/* [FUNCTION] OSへのメモリ解放                                          */
/************************************************************************/
void amMemFreeSystem(void *ptr, Sint32 heap = AMD_HEAP_DEFAULT);

/************************************************************************/
/* void amMemInit(size_t size, size_t size2)                            */
/*----------------------------------------------------------------------*/
/* [INPUT] size : 管理下に置くヒープサイズ                              */
/* [FUNCTION] メモリ管理システムの初期化                                */
/************************************************************************/
#if !_WII
void amMemInit(size_t size = AMD_HEAP_SIZE, size_t size2 = 0);
#else
void amMemInit(size_t size = AMD_HEAP_SIZE, size_t size2 = AMD_HEAP_SIZE2);
#endif

/************************************************************************/
/* void amMemExit(void)                                                 */
/*----------------------------------------------------------------------*/
/* [FUNCTION] メモリ管理システムの終了                                  */
/************************************************************************/
void amMemExit(void);

/************************************************************************/
/* void amMemReset(Sint32 heap)                                         */
/*----------------------------------------------------------------------*/
/* [FUNCTION]                                                           */
/*    メモリ管理システム上の全メモリ解放                                */
/*    メインシステム以外から呼ばれることはない(それ以外は呼出禁止)      */
/************************************************************************/
void amMemReset(Sint32 heap = AMD_HEAP_DEFAULT);

/************************************************************************/
/* void amMemFlip(Sint32 heap)                                          */
/*----------------------------------------------------------------------*/
/* [INPUT] heap : 反転するヒープ                                        */
/* [FUNCTION] ヒープ領域の (論理的な) 反転                              */
/************************************************************************/
void amMemFlip(Sint32 heap = AMD_HEAP_DEFAULT);

/************************************************************************/
/* void amMemSetCallback(MallocErrorCallback pFunc)                     */
/*----------------------------------------------------------------------*/
/* [INPUT] pFunc : コールバック関数のポインタ                           */
/* [FUNCTION] メモリ確保失敗時に呼び出されるコールバック関数の登録      */
/************************************************************************/
void amMemSetCallback(MallocErrorCallback pFunc);

/************************************************************************/
/* void *amMemDebugAlloc(size_t sz, Sint32 mode, Sint32 swap,           */
/*                                      const char *fname, Sint32 line) */
/*----------------------------------------------------------------------*/
/* [FUNCTION] デバッグ用 malloc 実関数                                  */
/************************************************************************/
void *amMemDebugAlloc(size_t sz, Sint32 mode, Sint32 heap = AMD_HEAP_DEFAULT,
		const char *fname = __FILE__, Sint32 line = __LINE__);
#if AMD_DEBUG
#define amMemAlloc(_sz)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_FIRSTFIT, AMD_HEAP_DEFAULT, \
			__FILE__, __LINE__)
#define amMemAllocHeap(_sz, _heap)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_FIRSTFIT, (_heap), \
			__FILE__, __LINE__)
#else
#define amMemAlloc(_sz)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_FIRSTFIT, AMD_HEAP_DEFAULT, \
			NULL, 0)
#define amMemAllocHeap(_sz, _heap)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_FIRSTFIT, (_heap), \
			NULL, 0)
#endif


/************************************************************************/
/* void * amMemAallocTemp(size_t size)                                  */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 後方から取得する malloc                                   */
/************************************************************************/
#if AMD_DEBUG
#define amMemAllocTemp(_sz)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_LASTFIT, AMD_HEAP_DEFAULT, \
			__FILE__, __LINE__)
#define amMemAllocTempHeap(_sz, _heap)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_LASTFIT, (_heap), \
			__FILE__, __LINE__)
#else
#define amMemAllocTemp(_sz)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_LASTFIT, AMD_HEAP_DEFAULT, \
			NULL, 0)
#define amMemAllocTempHeap(_sz, _heap)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_LASTFIT, (_heap), \
			NULL, 0)
#endif

/************************************************************************/
/* void * amMemAllocLow(size_t size)                                    */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 下位から取得する malloc                                   */
/************************************************************************/
#if AMD_DEBUG
#define amMemAllocLow(_sz)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_LOW, AMD_HEAP_DEFAULT, \
			__FILE__, __LINE__)
#define amMemAllocLowHeap(_sz, _heap)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_LOW, (_heap), \
			__FILE__, __LINE__)
#else
#define amMemAllocLow(_sz)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_LOW, AMD_HEAP_DEFAULT, \
			NULL, 0)
#define amMemAllocLowHeap(_sz, _heap)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_LOW, (_heap), \
			NULL, 0)
#endif

/************************************************************************/
/* void * amMemAllocHigh(size_t size)                                   */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 上位から取得する malloc                                   */
/************************************************************************/
#if AMD_DEBUG
#define amMemAllocHigh(_sz)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_HIGH, AMD_HEAP_DEFAULT, \
			__FILE__, __LINE__)
#define amMemAllocHighHeap(_sz, _heap)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_HIGH, (_heap), \
			__FILE__, __LINE__)
#else
#define amMemAllocHigh(_sz)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_HIGH, AMD_HEAP_DEFAULT, \
			NULL, 0)
#define amMemAllocHighHeap(_sz, _heap)	\
	amMemDebugAlloc((_sz), AMD_MALLOCMODE_HIGH, (_heap), \
			NULL, 0)
#endif

/************************************************************************/
/* void amMemFree(void *block)                                          */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 通常の free と使い方は同じ                                */
/************************************************************************/
void amMemDebugFree(void *pBlock);
#define amMemFree(_blk)   amMemDebugFree(_blk)

/************************************************************************/
/* void amMemCheck(void)                                                */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ヒープ破壊検査 (任意呼出可能 … デバッグ時に最適)         */
/************************************************************************/
#if AMD_DEBUG
void amMemCheck(void);
#else
#define amMemCheck()  ((void)0)
#endif

/************************************************************************/
/* void amMemDisplayInfo(Sint32 pos_x, Sint32 pos_y)                    */
/*----------------------------------------------------------------------*/
/* [FUNCTION] メモリ管理システムの情報表示                              */
/************************************************************************/
void amMemDisplayInfo(Sint32 pos_x = AMD_TEXT_WIDTH - 20, Sint32 pos_y = 1);

#endif // _AM_MALLOC_H_
