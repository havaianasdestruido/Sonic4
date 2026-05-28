/*
 *  mppCheckPointStorage.h
 *  hog
 *
 *  Created by Vyacheslav Mednonogov on 6/29/10.
 *  Copyright 2010 MPP. All rights reserved.
 *
 */

#ifndef MPP_CHECKPOINTSTORAGE_2010
#define MPP_CHECKPOINTSTORAGE_2010

#include "mppDebug.h"

///

struct MPP_CHECKPOINT_STATE_HEADER
{
	int stage_id; //zone with level combination
	int /*GSE_GAME_LEVEL_TYPE*/ level;	//hardness			
	int /*GSE_GAME_MODE*/ game_mode; //normal or time_attack
};

struct mppCheckPointStorage{
	enum SAVE_MODES {
		SAVE_AFTER_CHECKPOINT = 0xFFFAC,
		SAVE_AFTER_RESPAWN    = 0xEEEAD,
	};
	
	static bool isStateExist();
	static void removeState();
	static void saveState(int save_mode);
	static bool loadState();
	static void postInitializeStateIfNecessary1_inputAndSound();
	static void postInitializeStateIfNecessary2_player();
	//
	static bool loadStateHeader(MPP_CHECKPOINT_STATE_HEADER* pHdr);
	static bool isStateCompatibleWithCurrentGame();
	//
	static int isNeedToLoadSavedGame(); //call it from main menu loop (-1-no, 0-unknown, +1-yes)
};

#endif