/*
 *  mppAchievementSupport.h
 *  hog
 *
 *  Created by Vyacheslav Mednonogov on 9/30/10.
 *  Copyright 2010 MPP. All rights reserved.
 *
 */

#ifndef MPP_ACHIEVEMENT_SUPPORT_2010
#define MPP_ACHIVEEMENT_SUPPORT_2010



struct mppAchievementSupport {
public:

	int m_flags;//internal enum FLAGS_FOR_ONE_TIME_EVENTS;
	int m_flags_alertDlg;//shown ach. alerts
	
	struct COUNTERS {
		int numOfDefeatedEnemies; //Sonic4_Enemy, 
		int bitmaskSuperSonicClearedLevels;// Sonic4_Golden,
		//int bitmaskClearedLevels_Story;// Sonic4_Contender, - unused for Contender (15/oct/2010)
		int bitmaskClearedLevels_TimeAttack;// Sonic4_Contender,
		int bitmaskEmeralds;//Sonic4_Super
		int numOfPlayerLives; //Sonic4_Immortal,
		int numOfDamagesAtFinalLevel; //Sonic4_Untouchable  
		//
		COUNTERS() 
			:numOfDefeatedEnemies(0)
			,bitmaskSuperSonicClearedLevels(0)
			//,bitmaskClearedLevels_Story(0)
			,bitmaskClearedLevels_TimeAttack(0)
			,bitmaskEmeralds(0)
			,numOfPlayerLives(0)
			,numOfDamagesAtFinalLevel(0) {}
	} m_counters;
	
public:	
	static mppAchievementSupport* get();//singletone
	
	mppAchievementSupport();
	bool loadData();
	void saveData();
	
	//events
	void event_SuccessEndOf1stLevel(int timeInSecX60); //for Sonic4_StoryBegins,	Sonic4_Speed,  
	void event_DefeatBoss(); //for Sonic4_Eggman,   
	void event_ChaosEmeraldCollected(); //for Sonic4_Chaos1, Sonic4_Super,
	void event_DefeatEnemy();//for Sonic4_Enemy, 
	void event_SuccessEndOfAnyLevelAsSuperSonic(int level, bool mode_TimeAttack); //for Sonic4_Golden,
	void event_SuccessEndOfAnyLevel(int level, bool mode_TimeAttack); //for Sonic4_Contender,
	void event_SuccessEndOfSpecialStage1(bool allRingsCollected); // for Sonic4_Collector,
	void event_CheckLifeCount(); //for Sonic4_Immortal,
	
	void event_LoseRingFinalLevelCounterClear(bool forceClear); //(1) Sonic4_Untouchable
	void event_LoseRingInFinalLevel(); //(2) Sonic4_Untouchable
	void event_SuccessEndOfFinalLevel(); //for (3) Sonic4_Untouchable  and Sonic4_Cleared, 
	
	//events percentage
	float getPercentageAndSetFlagIf100(int eFlag);
	
	//flush data to Game Center (Apple Community)
	void flushAchievementsToGlobalNet();
	
	//achievement allert support
	void combineAchievementFlags(int prevFlags);
	int getAchievementAlertID(); //ret ID (0..11), or -1 if no new allerts occurs
	
private:
	bool isDataExist();
	//void removeData();
	//
public:
	bool checkFlag(int eFlag) {return (m_flags & eFlag)!=0;}
private:
	void setFlag(int eFlag) { m_flags |= eFlag; }
	void registerAchievement(int eFlag);
	void checkFlagAndRegisterAchievement(int eFlag);
	//
	static mppAchievementSupport g_inst;
	static bool g_isInitialized;
};

//helpers 
extern void mpp_checkAchievementsForSuccessLevelEnd();
extern void mpp_flushAchievementsToGlobalNet(bool showAchievementAlertIfPresent);

extern bool mpp_calcTotalTimeAttackTime(int& min, int& sec, int& sec100);  //true - all levels opened




#endif





















