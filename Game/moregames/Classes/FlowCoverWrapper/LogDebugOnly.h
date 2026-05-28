/*
 *  DebugLogOnly.h
 *  iGenesisGamePack
 *
 *  Created by MPP Developer on 11/24/09.
 *  Copyright 2010 MPP. All rights reserved.
 *
 */

#ifdef MOREGAMES_CONFIG_DEBUG
  #define NSLOG(x) NSLog x
#else
  #define NSLOG(x)
#endif

//#define RELEASE_NSDATA(X) 
#define RELEASE_NSDATA(X) {if(X!=nil) {[X release];X = nil;}}

#ifdef _MG_IPAD
	#define MG_SCREEN_WIDTH (1024)
	#define MG_SCREEN_HEIGHT (768)	
#else
	#define MG_SCREEN_WIDTH (480)
	#define MG_SCREEN_HEIGHT (320)
#endif
