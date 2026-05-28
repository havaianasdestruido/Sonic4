/*
 *  mppTimeScores.cpp
 *  hog
 *
 *  Created by Nikolai Lisun on 10/15/10.
 *  Copyright 2010 MPP. All rights reserved.
 *
 */
#import <Foundation/Foundation.h>


#include "mppTimeScores.h"
#include "mppUtil.h"

#ifdef SONIC4_TRIAL
#else
#import "Community.h"
#endif
mppTimeScores mppTimeScores::timeScores;

mppTimeScores::mppTimeScores()
{
	for(int i=0;i<LEVEL_COUNT;++i)
		times[i] = 60000;
}

void mppTimeScores::Set(int level, int time)
{
	if(times[level]>time)
		times[level] = time;
}

#define SCR_FNAME (mppUtil::getFilenameForGKLocalPlayerID("scr_lst_%08x.mpp"))

void mppTimeScores::Load()
{
	if( mppStorageUtil::isFileExist(SCR_FNAME) == false) return;
	int tmp[LEVEL_COUNT];
	mppStorageReader sr;
	sr.open(SCR_FNAME);
	int ver = -1;
	sr.read(ver);
	for(int i=0;i<LEVEL_COUNT;++i)
		sr.read(tmp[i]);
	if(!sr.readAndCheckCrc()) {NSLog(@"mppTimeScores::Load wrong crc"); return;}
	sr.close();	
	memcpy(times, tmp, sizeof(int)*LEVEL_COUNT);
}

void mppTimeScores::Save()
{
	mppStorageWriter sw;
	sw.open(SCR_FNAME);
	sw.write(0);
	for(int i=0;i<LEVEL_COUNT;++i)
		sw.write(times[i]);
	sw.writeCrc();
	sw.close();	
}

void mppTimeScores::Send()
{
#ifdef SONIC4_TRIAL
#else
	for(int i=0;i<LEVEL_COUNT;++i)
		if(times[i]<60000)
		{
			NSString* ukey = [NSString stringWithCString:mppUtil::getLBUkey(i) encoding:NSASCIIStringEncoding];
			if(ukey) {
				[[Community get] submitScore2:times[i] forCategory:ukey Data:&times[i]];
			}
		}
#endif
}

