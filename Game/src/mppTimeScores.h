/*
 *  mppTimeScores.h
 *  hog
 *
 *  Created by Nikolai Lisun on 10/15/10.
 *  Copyright 2010 MPP. All rights reserved.
 *
 */

#ifndef MPP_TIME_SCORES_H
#define MPP_TIME_SCORES_H

#include "mppUtil.h"

enum{ LEVEL_COUNT = 29 };

class mppTimeScores {
	int times[LEVEL_COUNT];
public:
	mppTimeScores();

	void Set(int level, int time);
	void Load();
	void Save();
	void Send();
	
	void SetLeaderboardTimesFromGameAtFirstLaunchIfNecessary(); 
	bool isNeedToForceSendTimesToLeaderboard();//ret: true -- need to send data

	static mppTimeScores timeScores;
};
#endif//MPP_TIME_SCORES_H
