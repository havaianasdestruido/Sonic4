#ifndef	_CRI_ALLOCATOR_H_INCLUDED
#define	_CRI_ALLOCATOR_H_INCLUDED
/****************************************************************************
 *																			*
 *				CRI Allocator												*
 *																			*
 *				2006-01-05	written by M.Oshimi								*
 *																			*
 ****************************************************************************/

/**************************************************************************** 
 *      Include file
 ****************************************************************************/
#include <stdio.h>
#include "cri_xpt.h"
#include "cri_heap.h"

/****************************************************************************
 *      Class Declaration
 ****************************************************************************/
#ifdef __cplusplus

class CriAllocator
{
public:
	static const CriSint32 DEFAULT_ALIGNMENT = CRIHEAP_DEFAULT_MEM_ALIGN;
	

	static void* CRIAPI operator new(size_t size, CriHeap heap) throw ();
	static void* CRIAPI operator new(size_t size, CriHeap heap, CriHeapType heap_type) throw ();
	static void* CRIAPI operator new(size_t size, CriHeap heap, const CriChar8 *name, CriUint32 align=DEFAULT_ALIGNMENT) throw ();
	static void* CRIAPI operator new(size_t size, CriHeap heap, CriHeapType heap_type, const CriChar8 *name, CriUint32 align=DEFAULT_ALIGNMENT) throw ();

	static void CRIAPI operator delete(void *p, size_t size);
	
	static void* CRIAPI operator new(size_t size, void *work, CriSint32 wksize) throw();
	static void CRIAPI operator delete(void *p, void *work, CriSint32 wksize) throw();
	static void* CRIAPI operator new(size_t size, void *work, CriSint32 wksize, CriUint32 align) throw();
	static void CRIAPI operator delete(void *p, void *work, CriSint32 wksize, CriUint32 align) throw();

#if !defined(XPT_TGT_BCB)
	static void CRIAPI operator delete(void *p, CriHeap heap) throw ();
	static void CRIAPI operator delete(void *p, CriHeap heap, CriHeapType heap_type) throw ();
	static void CRIAPI operator delete(void *p, CriHeap heap, const CriChar8 *name, CriUint32 align) throw ();
	static void CRIAPI operator delete(void *p, CriHeap heap, CriHeapType heap_type, const CriChar8 *name, CriUint32 align) throw ();
#endif

	static CriSint32 GetWorstExtraSize(CriUint32 align);
};
#endif // _CRI_ALLOCATOR_H_INCLUDED

#endif /* end of __cplusplus */

/* --- end of file --- */
