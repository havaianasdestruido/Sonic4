//
//  SegaMoreGames.h
//  SegaMoreGamesExampleNew
//
//  Created by Mpp IPhone Developer on 5/18/10.
//  Copyright 2010 MPP. All rights reserved.
//
//  C/CPP wrapper


#ifndef SEGA_MORE_GAMES_LAUNCHER_CPP
#define SEGA_MORE_GAMES_LAUNCHER_CPP
	
typedef void (*MG_ExitCallbackFunction)(bool success);

//[optional] enable autorotation (by default both landscape orientations enabled) 
extern void MoreGamesLauncher_enableAutorotate(bool enableLandscapeLeft, bool enableLandscapeRight);

//[optional] set position and size for loading indicator before start (by default it will be placed in center)
extern void MoreGamesLauncher_SetActivityIndicatorRect(int activityIndX, int activityIndY, int activityIndW, int  activityIndH);

//launch MG for C/CPP with callback
extern void MoreGamesLauncher_Start(MG_ExitCallbackFunction exitCallbackFunction); //exitCallbackFunction can be set to null
	
#endif
