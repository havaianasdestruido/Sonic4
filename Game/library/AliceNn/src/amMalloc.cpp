/************************************************************************/
/*      amMalloc.cpp                                                    */
/*            Copyright(c) 2000-2009 Dimps CORP. All Rights Reserved.   */
/*----------------------------------------------------------------------*/
/* メモリ管理ライブラリ                                                 */
/*----------------------------------------------------------------------*/
/* Date      Ver.   Comment                                             */
/* 20001024  0.10   簡易バージョン                                      */
/* 20010208  0.20   メモリ管理機能を付加                                */
/************************************************************************/

/*----- Update Logs ----------------------------------------------------*/

/*----- Macro switches -------------------------------------------------*/

/*----- Include files --------------------------------------------------*/

#include "alice.h"

/*----- Macros ---------------------------------------------------------*/

#define AMD_HEAP_CHECKCODE	(0xD2ACD2AC)
#define AMD_HEAP_ALIGN		(64)		// head + tail
#define AMD_HEAP_MANAGESIZE	(64)		// head + tail


/*----- Type definitions -----------------------------------------------*/

/*----- External variables ---------------------------------------------*/

#if _WII
MEMHeapHandle	_am_heap_handle[2] = {
	NULL, NULL,
};
#endif

AMS_HEAP_MANAGER	_am_heap_manager[AMD_HEAP_NUM] = {
	{
		NULL, NULL, 0, 0, 0, NULL,
	},
};


/*----- Global valiables -----------------------------------------------*/

Sint32	_am_dbg_display_mode = 0;
MallocErrorCallback	_am_dbg_heap_func = NULL;


/*----- Local valiables ------------------------------------------------*/

#if AMD_DEBUG
static Sint32	_am_heap_cdfs_flag = 1;
static Sint32	_am_heap_top_pos = 0;
#endif


/*----- Local declarations ---------------------------------------------*/

AMS_HEAP_HEAD *amm_SearchHeapBlock(AMS_HEAP_MANAGER *manager,
		Sint32 size, Sint32 direction);
AMS_HEAP_HEAD *amm_MakeHeapBlock(AMS_HEAP_HEAD *pHeap, Sint32 size,
		Sint32 direction);

#if !_WII
void amm_DrawHeapMap(AMS_HEAP_MANAGER *manager);
#else
void amm_DrawHeapMap(AMS_HEAP_MANAGER *manager, Sint32 scaling = 0);
#endif
void amm_CtrlHeapList(void);
#if AMD_DEBUG
Sint32 amm_DrawHeapList(AMS_HEAP_MANAGER *manager, Sint32 pos_y = _am_heap_top_pos);
#else
Sint32 amm_DrawHeapList(AMS_HEAP_MANAGER *manager, Sint32 pos_y = 0);
#endif


/*----- Global functions -----------------------------------------------*/

#if _PS3
void *sbl_malloc(size_t size)
{
	return	memalign(128, size);
}

void sbl_free(void *mem)
{
	free(mem);
}

void *sbl_aligned_malloc(size_t size, size_t alignment)
{
	return	memalign(alignment, size);
}

void sbl_aligned_free(void *mem)
{
	free(mem);
}
#endif


/************************************************************************/
/* void *amMemAllocSystem(size_t size, Sint32 heap)                     */
/*----------------------------------------------------------------------*/
/* [INPUT] size : OSから確保するメモリサイズ                            */
/*         heap : OSから確保するヒープの種類                            */
/* [FUNCTION] OSからのメモリ確保                                        */
/************************************************************************/
void *amMemAllocSystem(size_t size, Sint32 heap)
{
#if _PC
	UNREFERENCED_PARAMETER(heap);

	return	_aligned_offset_malloc(size, AMD_HEAP_ALIGN, 0);
#elif _WII
	amAssert(size);

#if 0
{
	size_t	max_size;
	max_size	= MEMGetAllocatableSizeForExpHeapEx(
			_am_heap_handle[heap], AMD_HEAP_ALIGN);
	if (max_size < size)
		amSystemLog("[ERR] SysAlloc Failed (%d / max %d)\n", size, max_size);
}
#endif

	return	MEMAllocFromExpHeapEx(_am_heap_handle[heap], size, AMD_HEAP_ALIGN);
#else
	UNREFERENCED_PARAMETER(heap);

	return	malloc(size);
#endif
}


/************************************************************************/
/* void amMemFreeSystem(void *ptr, Sint32 heap)                         */
/*----------------------------------------------------------------------*/
/* [INPUT] ptr  : 解放するメモリ                                        */
/*         heap : 解放するヒープの種類                                  */
/* [FUNCTION] OSへのメモリ解放                                          */
/************************************************************************/
void amMemFreeSystem(void *ptr, Sint32 heap)
{
	amAssert(ptr);

	if (ptr == NULL)
		return;

#if _PC
	UNREFERENCED_PARAMETER(heap);

	_aligned_free(ptr);
#elif _WII
	MEMFreeToExpHeap(_am_heap_handle[heap], ptr);
#else
	UNREFERENCED_PARAMETER(heap);

	free(ptr);
#endif
}


/************************************************************************/
/* void amMemInit(size_t size, size_t size2)                            */
/*----------------------------------------------------------------------*/
/* [INPUT] size : 管理下に置くヒープサイズ                              */
/* [FUNCTION] メモリ管理システムの初期化                                */
/************************************************************************/
void amMemInit(size_t size, size_t size2)
{
	AMS_HEAP_MANAGER	*manager;
	Sint32		i;

	manager		= &_am_heap_manager[0];

	// malloc manager initialize
	amAssert(manager[0].buf == NULL);

	// hold heap
	for (i = 0; i < AMD_HEAP_NUM; i++, manager++) {
#if AMD_DEBUG
		Sint32		size0;
		size0	= size;
#endif
		size	+= 1024;
		do {
			size	-= 1024;
			manager->buf	= (Sint8 *)amMemAllocSystem(size, 1 - i);
		} while (manager->buf == NULL);
#if AMD_DEBUG
#if _WII
		size_t	max_size;
		max_size	= MEMGetAllocatableSizeForExpHeapEx(
				_am_heap_handle[1 - i], AMD_HEAP_ALIGN);
		if (size0 != size) {
			amSystemLog("[WARN] Heap%d size = %dKB (request %dKB, system %dKB)\n",
					i, size >> 10, size0 >> 10, max_size >> 10);
		} else
			amSystemLog("Heap%d size = %dKB (system %dKB)\n", i, size >> 10, max_size >> 10);
#else
		if (size0 != size) {
			amSystemLog("[WARN] Heap%d size = %dKB (request %dKB)\n",
					i, size >> 10, size0 >> 10);
		} else
			amSystemLog("Heap%d size = %dKB\n", i, size >> 10);
#endif
#endif
	    
		//=== 64byte 境界のために えんやこら
		{
			Sint32	offset;
			Uint32	heap_addr, heap_size;

			heap_addr	= (Uint32)manager->buf;
			heap_size	= size;
	    
			// calc head position
			if ((heap_addr % AMD_HEAP_ALIGN) != 0) {
				offset		= AMD_HEAP_ALIGN - (heap_addr % AMD_HEAP_ALIGN);
				heap_addr	+= offset;
				heap_size	-= offset;
			}
			heap_addr	+= AMD_HEAP_ALIGN - sizeof(AMS_HEAP_HEAD);
			heap_size	-= AMD_HEAP_ALIGN;
			manager->head	= (AMS_HEAP_HEAD *)heap_addr;

			// calc tail position
			heap_size	-= heap_size % AMD_HEAP_ALIGN;
			heap_addr	= ((Uint32)manager->head)
						+ sizeof(AMS_HEAP_HEAD) + heap_size; // 末尾
			heap_addr	-= AMD_HEAP_ALIGN;
			heap_size	-= AMD_HEAP_ALIGN;
			manager->tail	= (AMS_HEAP_TAIL *)heap_addr;

			manager->all	= heap_size;
		}

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
		amMutexCreate(&manager->mutex);
#endif

		// init
		amMemReset(i);

		size	= size2;
	}
	_am_dbg_heap_func	= NULL;

#if AMD_DEBUG
	_am_dbg_display_mode	= 2;
#else
	_am_dbg_display_mode	= 0;
#endif
}


/************************************************************************/
/* void amMemExit(void)                                                 */
/*----------------------------------------------------------------------*/
/* [FUNCTION] メモリ管理システムの終了                                  */
/************************************************************************/
void amMemExit(void)
{
	AMS_HEAP_MANAGER	*manager = &_am_heap_manager[0];
	Sint32		i;

	amAssert(manager[0].buf);

	for (i = 0; i < AMD_HEAP_NUM; i++, manager++) {
#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
		amMutexLock(&manager->mutex);
		amMutexUnlock(&manager->mutex);
		amMutexDelete(&manager->mutex);
#endif
		amMemFreeSystem(manager->buf, 1 - i);

		manager->buf	= NULL;
	}
}


/************************************************************************/
/* void amMemReset(Sint32 heap)                                         */
/*----------------------------------------------------------------------*/
/* [FUNCTION]                                                           */
/*    メモリ管理システム上の全メモリ解放                                */
/*    メインシステム以外から呼ばれることはない(それ以外は呼出禁止)      */
/************************************************************************/
void amMemReset(Sint32 heap)
{
#if _WII
	AMS_HEAP_MANAGER	*manager = &_am_heap_manager[heap];
#else
	UNREFERENCED_PARAMETER(heap);
	AMS_HEAP_MANAGER	*manager = &_am_heap_manager[0];
#endif

	amAssert(manager->head && manager->tail);

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
	amMutexLock(&manager->mutex);
#endif
	// init heap head
	memset(manager->head, 0, sizeof(AMS_HEAP_HEAD));
	manager->head->size			= manager->all;
	manager->head->pTail		= manager->tail;
	manager->head->checkCode	= AMD_HEAP_CHECKCODE;

	// init heap tail
	memset(manager->tail, 0, sizeof(AMS_HEAP_TAIL));
	manager->tail->checkCode	= AMD_HEAP_CHECKCODE;
	manager->tail->pHead		= manager->head;

	// init global
	manager->count			= 0;
	manager->count_normal	= 0;
	manager->total			= 0;
	manager->swap			= 0;

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
	amMutexUnlock(&manager->mutex);
#endif
}


/************************************************************************/
/* void amMemFlip(Sint32 heap)                                          */
/*----------------------------------------------------------------------*/
/* [INPUT] heap : 反転するヒープ                                        */
/* [FUNCTION] ヒープ領域の (論理的な) 反転                              */
/************************************************************************/
void amMemFlip(Sint32 heap)
{
#if _WII
	AMS_HEAP_MANAGER	*manager = &_am_heap_manager[heap];
#else
	UNREFERENCED_PARAMETER(heap);
	AMS_HEAP_MANAGER	*manager = &_am_heap_manager[0];
#endif

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
	amMutexLock(&manager->mutex);
#endif

	// toggle
	manager->swap	^= 1;

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
	amMutexUnlock(&manager->mutex);
#endif
}


/************************************************************************/
/* void amMemSetCallback(MallocErrorCallback pFunc)                     */
/*----------------------------------------------------------------------*/
/* [INPUT] pFunc : コールバック関数のポインタ                           */
/* [FUNCTION] メモリ確保失敗時に呼び出されるコールバック関数の登録      */
/************************************************************************/
void amMemSetCallback(MallocErrorCallback pFunc)
{
#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
	AMS_HEAP_MANAGER	*manager = &_am_heap_manager[0];

#if _WII
	amMutexLock(&manager[0].mutex);
	amMutexLock(&manager[1].mutex);
#else
	amMutexLock(&manager[0].mutex);
#endif
#endif

	_am_dbg_heap_func	= pFunc;

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
#if _WII
	amMutexUnlock(&manager[1].mutex);
	amMutexUnlock(&manager[0].mutex);
#else
	amMutexUnlock(&manager[0].mutex);
#endif
#endif
}


/************************************************************************/
/* void *amMemDebugAlloc(size_t sz, Sint32 mode, Sint32 swap,           */
/*                                      const char *fname, Sint32 line) */
/*----------------------------------------------------------------------*/
/* [FUNCTION] デバッグ用 malloc 実関数                                  */
/************************************************************************/
void *amMemDebugAlloc(size_t sz, Sint32 mode, Sint32 heap, const char *fname, Sint32 line)
{
#if _WII
	AMS_HEAP_MANAGER	*manager = &_am_heap_manager[heap];
#else
	UNREFERENCED_PARAMETER(heap);
	AMS_HEAP_MANAGER	*manager = &_am_heap_manager[0];
#endif
	Sint32			direction;
	void			*pResult;
	AMS_HEAP_HEAD	*pHeap;

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
	amMutexLock(&manager->mutex);
#endif

	// 無意味な確保サイズです
	amAssert(sz > 0 /* bad argument */);

	// 64byte align
	sz		= (sz + AMD_HEAP_ALIGN - 1) & ~(AMD_HEAP_ALIGN - 1);
    
	// set direction
	direction	= 0;
	switch (mode) {
		case	AMD_MALLOCMODE_LOW:
			break;
		case	AMD_MALLOCMODE_HIGH:
			direction	= 1;
			break;
		case	AMD_MALLOCMODE_LASTFIT:
			direction	= 1;
			/* no break */
		default:
#if _WII
			if (_am_heap_manager[1].swap != 0)
#else
			if (_am_heap_manager[0].swap != 0)
#endif
				direction	^= 1;
			break;
	}

	// search block
	pHeap	= amm_SearchHeapBlock(manager, sz, direction);
    
	// メモリ確保失敗
	if (!pHeap) {
		amSystemLog(
				"Total heap size : %dKB\n", (manager->total >> 10));
		amAssert(0 /* cannot allocate memory */);

		// 確保失敗時コールバック
		if (_am_dbg_heap_func)
			_am_dbg_heap_func();

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
		amMutexUnlock(&manager->mutex);
#endif
		return	NULL;
    }
    
	// 切り出し
	pHeap	= amm_MakeHeapBlock(pHeap, sz, direction);

	// allocate counter
	manager->count++;
	if (mode == AMD_MALLOCMODE_FIRSTFIT)
		manager->count_normal++;

	// トータルサイズ
	manager->total	+= -(pHeap->size);
	manager->total	+= AMD_HEAP_MANAGESIZE;

#if AMD_DEBUG
	// set debug information
	pHeap->pTail->line	= line;
	pHeap->checkCode	= AMD_HEAP_CHECKCODE;
	pHeap->pTail->checkCode	= AMD_HEAP_CHECKCODE;
	memset(pHeap->fileName, 0, AMD_FILENAME_LENGTH);
	if (fname != NULL) {
		char	*p0;
		Sint32	index;
		index		= strlen(fname) - 1;
		p0			= (char *)&fname[index];
		while ((index > 0) && (p0[-1] != '/') && (p0[-1] != '\\')) {
			index--;
			p0--;
		}
		strncpy(pHeap->fileName, p0, AMD_FILENAME_LENGTH - 1);
	}
#endif

#if _WII
	pHeap->pTail->heap	= heap;
#endif

    if (mode == AMD_MALLOCMODE_LASTFIT)
		pHeap->pTail->flag	|= AMD_HEAP_FLAG_TEMPORARY;
    
	// convert
	pResult	= (void *)((Uint32)pHeap + sizeof(AMS_HEAP_HEAD));

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
	amMutexUnlock(&manager->mutex);
#endif

	// result
	return	pResult;
}


/************************************************************************/
/* void amMemDebugFree(void *pBlock)                                    */
/*----------------------------------------------------------------------*/
/* [FUNCTION] デバッグ用 free 実関数                                    */
/************************************************************************/
void amMemDebugFree(void *pBlock)
{
	AMS_HEAP_MANAGER	*manager;
	AMS_HEAP_HEAD		*pHeap;

	// NULLを開放しようとしてます
#if AMD_DEBUG
	amAssert(pBlock);
#else
	// 保険
	if (!pBlock)
		return;
#endif // AMD_DEBUG
    
	// calc heap head
	pHeap	= (AMS_HEAP_HEAD *)((Uint32)pBlock - sizeof(AMS_HEAP_HEAD));

#if _WII
	manager		= &_am_heap_manager[pHeap->pTail->heap];
#else
	manager		= &_am_heap_manager[0];
#endif

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
	amMutexLock(&manager->mutex);
#endif

	// Allocate counter
	manager->count--;
	if (!(pHeap->pTail->flag & AMD_HEAP_FLAG_TEMPORARY))
		manager->count_normal--;

	// 開放し過ぎています
#if AMD_DEBUG
	amAssert(manager->count >= 0);
	amAssert(manager->count_normal >= 0);
#else
	// 保険
	if (manager->count < 0) {
		manager->count = 0;
#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
		amMutexUnlock(&manager->mutex);
#endif
		return;
	}
#endif // AMD_DEBUG

	// not heap
#if AMD_DEBUG
	amAssert((Uint32)pHeap->checkCode == AMD_HEAP_CHECKCODE);
	amAssert((Uint32)pHeap->pTail->checkCode == AMD_HEAP_CHECKCODE);
	amAssert(pHeap->size < 0);
#endif // AMD_DEBUG

	// まず未使用状態に
	pHeap->size		= -pHeap->size;
	manager->total	-= pHeap->size + AMD_HEAP_MANAGESIZE;

	// 前のブロックと接続
	if ((pHeap->pPrev != NULL) && (pHeap->pPrev->pHead->size > 0)) {
		pHeap->pPrev->pHead->size	+= pHeap->size + AMD_HEAP_MANAGESIZE;

		pHeap->pPrev->pHead->pTail	= pHeap->pTail;
		pHeap->pTail->pHead			= pHeap->pPrev->pHead;

		pHeap	= pHeap->pPrev->pHead;
    }

	// 後のブロックと接続
	if ((pHeap->pTail->pNext != NULL) && (pHeap->pTail->pNext->size > 0)) {
		pHeap->size		+= pHeap->pTail->pNext->size + AMD_HEAP_MANAGESIZE;
        
		pHeap->pTail	= pHeap->pTail->pNext->pTail;
		pHeap->pTail->pHead = pHeap;
	}

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
	amMutexUnlock(&manager->mutex);
#endif
}


#if AMD_DEBUG
/************************************************************************/
/* void amMemCheck(void)                                                */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ヒープ破壊検査 (任意呼出可能 … デバッグ時に最適)         */
/************************************************************************/
void amMemCheck(void)
{
	AMS_HEAP_MANAGER	*manager = &_am_heap_manager[0];
	AMS_HEAP_HEAD	*pHeap;
	Sint32			i;

	for (i = 0; i < AMD_HEAP_NUM; i++, manager++) {
#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
		amMutexLock(&manager->mutex);
#endif
		// 前方から
		pHeap	= manager->head;
		while (pHeap) {
			amAssert((Uint32)pHeap->checkCode == AMD_HEAP_CHECKCODE);
			amAssert((Uint32)pHeap->pTail->checkCode == AMD_HEAP_CHECKCODE);

			pHeap	= pHeap->pTail->pNext;
		}

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
		amMutexUnlock(&manager->mutex);
#endif
	}
}
#endif


/************************************************************************/
/* void amMemDisplayInfo(Sint32 pos_x, Sint32 pos_y)                    */
/*----------------------------------------------------------------------*/
/* [FUNCTION] メモリ管理システムの情報表示                              */
/************************************************************************/
void amMemDisplayInfo(Sint32 pos_x, Sint32 pos_y)
{
#if AMD_DEBUG
	AMS_HEAP_MANAGER	*manager = &_am_heap_manager[0];

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
#if _WII
	amMutexLock(&_am_heap_manager[0].mutex);
	amMutexLock(&_am_heap_manager[1].mutex);
#else
	amMutexLock(&_am_heap_manager[0].mutex);
#endif
#endif

#if AMD_USE_KEYBD
	if (_am_module_flag & AMD_MODULE_KEYBD) {
		if (amGetKeybdModStat(KBD_MOD_LCTRL | KBD_MOD_RCTRL) &&
			amGetKeybdStand(KBD_M)) {
			_am_dbg_display_mode	= (_am_dbg_display_mode + 1) % 4;
		}
	}
#endif // AMD_USE_KEYBD

	switch (_am_dbg_display_mode) {
		case	1:
			// debug display
#if !_WII
			amPrintf(pos_x, pos_y, "HEAP SIZE:%5d/%3d",
				manager->total >> 10, manager->count);
#else
			amPrintf(pos_x, pos_y, "HEAP SIZE:%5d/%3d",
				(manager[0].total + manager[1].total) >> 10,
				manager[0].count + manager[1].count);
#endif
			break;

		case	2:
			// debug display
#if !_WII
			amPrintf(pos_x, pos_y, "%5d(%2d:%3d)/%5d",
					manager->total >> 10,
					manager->count_normal,
					manager->count - manager->count_normal,
					(manager->all >> 10));

			// mater
			amm_DrawHeapMap(manager);
#else
{
			Sint32		count, count0;
			count	= manager[0].count + manager[1].count;
			count0	= manager[0].count_normal + manager[1].count_normal;
			amPrintf(pos_x, pos_y, "%5d(%2d:%3d)/%5d",
					(manager[0].total + manager[1].total) >> 10,
					count0,
					count - count0,
					(manager[0].all + manager[1].all) >> 10);

			// mater
			amm_DrawHeapMap(&manager[0], 1);
			amm_DrawHeapMap(&manager[1], 1);
}
#endif
			break;

		case	3:
			// debug display
#if !_WII
			amPrintf(pos_x, pos_y, "%5d(%2d:%3d)/%5d",
					(manager->total >> 10),
					manager->count_normal,
					manager->count - manager->count_normal,
					(manager->all >> 10));

			amm_CtrlHeapList();
			amm_DrawHeapList(manager);
#else
{
			Sint32		count, count0;
			count	= manager[0].count + manager[1].count;
			count0	= manager[0].count_normal + manager[1].count_normal;
			amPrintf(pos_x, pos_y, "%5d(%2d:%3d)/%5d",
					(manager[0].total + manager[1].total) >> 10,
					count0,
					count - count0,
					(manager[0].all + manager[1].all) >> 10);

			amm_CtrlHeapList();
			pos_y	= amm_DrawHeapList(&manager[0]);
			amm_DrawHeapList(&manager[1], pos_y);
}
#endif
			break;

		case	4:
			// debug display
#if !_WII
			amPrintf(pos_x, pos_y, "%5d(%2d:%3d)/%5d",
					(manager->total >> 10),
					manager->count_normal,
					manager->count - manager->count_normal,
					(manager->all >> 10));

			amm_DrawHeapMap(manager);
			amm_CtrlHeapList();
			amm_DrawHeapList(manager);
#else
			amPrintf(pos_x, pos_y, "%5d(%2d:%3d)/%5d",
					(manager[0].total + manager[1].total) >> 10,
					manager[0].count_normal + manager[1].count_normal,
					manager[0].count - manager[1].count_normal,
					(manager[0].all + manager[1].all) >> 10);

			amm_DrawHeapMap(&manager[0], 1);
			amm_DrawHeapMap(&manager[1], 1);
			amm_CtrlHeapList();
			pos_y	= amm_DrawHeapList(&manager[0]);
			amm_DrawHeapList(&manager[1], pos_y);
#endif
			break;
	}

#if AMD_TASK_THREAD_NUM + AMD_USE_DRAW_THREAD > 1
#if _WII
	amMutexUnlock(&_am_heap_manager[1].mutex);
	amMutexUnlock(&_am_heap_manager[0].mutex);
#else
	amMutexUnlock(&_am_heap_manager[0].mutex);
#endif
#endif
#endif // AMD_DEBUG
}


/*----- Local functions ------------------------------------------------*/

/************************************************************************/
/* AMS_HEAP_HEAD *amm_SearchHeapBlock(AMS_HEAP_MANAGER *manager,        */
/*                                    Sint32 size, Sint32 direction)    */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ヒープブロックの検索                                      */
/************************************************************************/
AMS_HEAP_HEAD *amm_SearchHeapBlock(AMS_HEAP_MANAGER *manager, Sint32 size, Sint32 direction)
{
	AMS_HEAP_HEAD	*pHeap;

	if (direction == 0) {
		// 前方から
		pHeap	= manager->head;

		while (pHeap) {
			if (pHeap->size >= size)
				return	pHeap;
			pHeap	= pHeap->pTail->pNext;
		}
	} else {
		// 後方から
		pHeap	= manager->tail->pHead;
        
		while (pHeap) {
			if (pHeap->size >= size)
				return pHeap;
			if (!pHeap->pPrev)
				break;
			pHeap	= pHeap->pPrev->pHead;
		}
	}
    
	return	NULL;
}


/************************************************************************/
/* AMS_HEAP_HEAD *amm_MakeHeapBlock(                                    */
/*                AMS_HEAP_HEAD *pHeap, Sint32 size, Sint32 direction)  */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ヒープブロックの検索                                      */
/************************************************************************/
AMS_HEAP_HEAD *amm_MakeHeapBlock(AMS_HEAP_HEAD *pHeap, Sint32 size, Sint32 direction)
{
	Sint32	size1, size2;
	AMS_HEAP_TAIL	*pTemp;

	amAssert(pHeap->size > 0);
    
	// 分割しない
	if (pHeap->size <= size + AMD_HEAP_MANAGESIZE) {
		pHeap->size		= -pHeap->size;
		pHeap->pTail->flag	= 0;

		return	pHeap;
	}

	// save
	pTemp	= pHeap->pTail;

	if (direction == 0) {
		// 前方取得
		size1	= size;
		size2	= pHeap->size - size - AMD_HEAP_MANAGESIZE;
	} else {
		// 後方取得
		size1	= pHeap->size - size - AMD_HEAP_MANAGESIZE;
		size2	= size;
	}

	// 繋ぎ換え
	pHeap->pTail	= (AMS_HEAP_TAIL *)((Uint32)pHeap + size1 + sizeof(AMS_HEAP_HEAD));
    pHeap->pTail->pNext	= (AMS_HEAP_HEAD *)((Uint32)pHeap->pTail + sizeof(AMS_HEAP_TAIL));

	pHeap->pTail->pHead	= pHeap;
	pHeap->pTail->pNext->pPrev	= pHeap->pTail;
	pHeap->pTail->pNext->pTail	= pTemp;
	pTemp->pHead	= pHeap->pTail->pNext;

	// update size
	pHeap->size		= size1;
	pHeap->pTail->pNext->size	= size2;

	// set return value
	if (direction) {
#if AMD_DEBUG
		// set debug information
		pHeap->pTail->checkCode	= AMD_HEAP_CHECKCODE;
#endif
		pHeap	= pHeap->pTail->pNext;
	} else {
#if AMD_DEBUG
		// set debug information
		pHeap->pTail->pNext->checkCode	= AMD_HEAP_CHECKCODE;
#endif
	}

	// init heap
	pHeap->size	= -pHeap->size;
	pHeap->pTail->flag	= 0;

	return	pHeap;
}


/************************************************************************/
/* void amm_DrawHeapMap(AMS_HEAP_MANAGER *manager, Sint32 scaling)      */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ヒープメーター描画                                        */
/************************************************************************/
#if !_WII
void amm_DrawHeapMap(AMS_HEAP_MANAGER *manager)
#else
void amm_DrawHeapMap(AMS_HEAP_MANAGER *manager, Sint32 scaling)
#endif
{
#if AMD_DEBUG
	AMS_HEAP_HEAD	*pHeap;
	Sint32		start_offset = 0, end_offset, size, draw;
	float		rate;
	Uint32		*col, *col_last = NULL;
	float		x0, x1, y0, y1;
	AMS_HEAP_MAP_HEADER	*header = NULL;
	AMS_HEAP_MAP_BLOCK	*block = NULL;

	const	Uint32 col0 = 0x191919ff;
	const	Uint32 col1 = 0x4c66b2ff;
	const	Uint32 col2 = 0xb2664cff;

	draw	= amThreadCheckDraw();

	pHeap	= manager->head;
	end_offset	= 0;
#if _PC
	x0		= AMD_DISPLAY_WIDTH - 16.0f;
#else
	x0		= AMD_DISPLAY_WIDTH - 32.0f;
#endif
	x1		= AMD_DISPLAY_WIDTH;
	rate	= (AMD_DISPLAY_HEIGHT / (float)(manager->all + 64));
#if _WII
	if (scaling) {
		rate	= (AMD_DISPLAY_HEIGHT
				/ (float)(_am_heap_manager[0].all + _am_heap_manager[1].all
				+ 64 + 64));
		if (manager != &_am_heap_manager[0])
			end_offset	= _am_heap_manager[0].all + 64;
	}
#endif

	if (draw)
		nnBeginDrawPrimitive2D(NNE_PRIM2D_FMT_PC, NNE_PRIM_ALPHABLEND_OFF);
	else {
		header	= (AMS_HEAP_MAP_HEADER *)amDrawGetDataBuffer();
		block	= (AMS_HEAP_MAP_BLOCK *)(header + 1);
		header->block_num	= 0;
		header->x0	= x0;
		header->x1	= x1;
	}
	while (pHeap) {
		size	= pHeap->size;
		if (size < 0)
			size	= -size;

		col		= (Uint32 *)&col0;
		if (pHeap->size < 0)
			col		= (pHeap->pTail->flag) ?(Uint32 *)&col2 : (Uint32 *)&col1;

		if (col != col_last) {
			if (col_last != NULL) {
				y0		= (float)start_offset * rate;
				y1		= (float)  end_offset * rate;
				if (draw) {
					NNS_PRIM2D_PC	vtx[4] = {
						{ { x0, y0,}, *col_last, },
						{ { x1, y0,}, *col_last, },
						{ { x0, y1,}, *col_last, },
						{ { x1, y1,}, *col_last, },
					};
					nnDrawPrimitive2D(NNE_PRIM_TRIANGLE_STRIP, vtx, 4, -1.0f);
				} else {
					block->color	= *col_last;
					block->y0		= y0;
					block->y1		= y1;
					block++;
					header->block_num++;
				}
			}
			start_offset	= end_offset;
			end_offset		= start_offset + (size + 64);
			col_last		= col;
		} else
			end_offset		= end_offset + (size + 64);

		if ((Uint32)pHeap->pTail->pNext > (Uint32)manager->tail)
			amAssert(0);
		pHeap	= pHeap->pTail->pNext;
	}
	y0		= (float)start_offset * rate;
	y1		= (float)  end_offset * rate;
	if (draw) {
		NNS_PRIM2D_PC	vtx[4] = {
			{ { x0, y0,}, *col_last, },
			{ { x1, y0,}, *col_last, },
			{ { x0, y1,}, *col_last, },
			{ { x1, y1,}, *col_last, },
		};
		nnDrawPrimitive2D(NNE_PRIM_TRIANGLE_STRIP, vtx, 4, -1.0f);
		nnEndDrawPrimitive2D();
	} else {
		block->color	= *col_last;
		block->y0		= y0;
		block->y1		= y1;
		block++;
		header->block_num++;
		amDrawSetDataBuffer((char *)block);

		amDrawRegistCommand(
				AMD_COMMAND_STATE_DEBUG,
				AMD_COMMAND_DEBUG_MEMORY,
				header);
	}
#endif
}


/************************************************************************/
/* void amm_CtrlHeapList(void)                                          */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ヒープリスト描画制御                                      */
/************************************************************************/
void amm_CtrlHeapList(void)
{
#if AMD_DEBUG
#if AMD_USE_KEYBD
	if (_am_module_flag & AMD_MODULE_KEYBD) {
		if (amGetKeybdModStat(KBD_MOD_LCTRL | KBD_MOD_RCTRL)) {
			if (amGetKeybdStand(KBD_COMMA)) {
				_am_heap_top_pos++;
				if (_am_heap_top_pos > 0)
					_am_heap_top_pos	= 0;
			}
			if (amGetKeybdStand(KBD_DOT)) {
				_am_heap_top_pos--;
			}
			if (amGetKeybdStand(KBD_SLASH)) {
				_am_heap_cdfs_flag	= !_am_heap_cdfs_flag;
				_am_heap_top_pos	= 0;
			}
		}
	}
#endif // AMD_USE_KEYBD
#endif
}


/************************************************************************/
/* Sint32 amm_DrawHeapList(AMS_HEAP_MANAGER *manager, Sint32 pos_y)     */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ヒープリスト描画                                          */
/************************************************************************/
Sint32 amm_DrawHeapList(AMS_HEAP_MANAGER *manager, Sint32 pos_y)
{
#if AMD_DEBUG
	AMS_HEAP_HEAD	*pHeap;
	Sint32		i, size;

	pHeap	= manager->head;

	for (i = 0; pHeap != NULL; pHeap = pHeap->pTail->pNext) {
		size	= pHeap->size;
		if (size >= 0)
			continue;

		if (!_am_heap_cdfs_flag) {
			if (!strcmp(pHeap->fileName, "amFs.cpp")) {
				i++;
				continue;
			}
		}

		if ((pos_y < 0) || (pos_y >= 20)) {
			i++;
			continue;
		}

		size	= -size;

		amPrintf(1, 1 + pos_y, "%3d %8X %16s-%4d", i, size, pHeap->fileName,
				pHeap->pTail->line);
		pos_y++;

		i++;
	}
#endif

	return	pos_y;
}

