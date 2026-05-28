/*****************************************************************************/
/*      am_mppScreenSize.h                                                   */
/*            Copyright(c) 2010 MPP (SPB1 division). All Rights Reserved.    */
/*---------------------------------------------------------------------------*/                       
/* Date          Ver    Comment                                              */
/* 2010SEP24     1.0    last version                                         */
/*****************************************************************************/

#ifndef _AM_MPPSCREENSIZE_H
#define _AM_MPPSCREENSIZE_H

//sss -- iPad support
#ifdef _MG_IPAD
	#define AM_SCREEN_WIDTH (1024)
	#define AM_SCREEN_HEIGHT (768)	
#else
	#define AM_SCREEN_WIDTH (480)
	#define AM_SCREEN_HEIGHT (320)
#endif



#endif
