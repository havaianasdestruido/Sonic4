/*---------------------------------------------------------------------------

    NV header

    Copyright (C) 2002 - 2004 SEGA Corporation Creative Center Graphics Sect.
    All Rights Reserved.

    Module  : NV header
    File    : nv.h
    Create  : 2002/04/16
    Modify  : 2003/03/28
    Modify  : 2003/10/08 Xbox,PCã§í âªå¸ÇØÇÃèCê≥
    Modify  : 2004/06/16 DX8,DX9î≈ìùçá
    Modify  : 2004/08/05 NND_PLATFORM_XENONí«â¡
    Modify  : 2008/05/21 NND_PLATFORM_DXG20í«â¡
    Version : 1.16.11
    Note    : 

---------------------------------------------------------------------------*/

#ifndef	__NV_H__
#define	__NV_H__

#if ( NND_PLATFORM == NND_PLATFORM_PS2 )
#include <nvs.h>
#elif ( NND_PLATFORM == NND_PLATFORM_GC )
#include <nvg.h>
#elif ( NND_PLATFORM == NND_PLATFORM_XB || NND_PLATFORM == NND_PLATFORM_DX8 || NND_PLATFORM == NND_PLATFORM_DX9 || NND_PLATFORM == NND_PLATFORM_XENON || NND_PLATFORM == NND_PLATFORM_DXG20 )
#include <nvx.h>
#endif


#endif //__NV_H__

/* End of file */
