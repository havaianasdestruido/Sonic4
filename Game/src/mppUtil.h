/*
 *  mppUtil.h
 *  hog
 *
 *  Created by Vyacheslav Mednonogov on jul/1/10.
 *  Copyright 2010 MPP. All rights reserved.
 *
 */

#ifndef MPP_UTIL__2010
#define MPP_UTIL__2010

#include "mppDebug.h"

enum //do not change this values
{
    Sonic4_StoryBegins =0,   
    Sonic4_Eggman      =1,   
    Sonic4_Chaos1      =2, 
    Sonic4_Enemy       =3, 
    Sonic4_Golden      =4,
    Sonic4_Cleared     =5, 
    Sonic4_Contender   =6,
    Sonic4_Collector   =7,
    Sonic4_Immortal    =8,
    Sonic4_Super       =9,
    Sonic4_Speed       =10,  
    Sonic4_Untouchable =11,
	//
	Sonic4_MAX_ACHIEVEMENT_
};

struct mppStorageUtil
{
	static const char* getFullPath(const char* path);
	static bool deleteFile(const char* path);
	static bool isFileExist(const char* path);
	static bool isFileValid(const char* path);
};

struct mppUtil
{
	static void showLoadGameConfirmation();
	static void launchMoreGames();
	static int isIPodMusicNowPlaying(); //0 - no, 1 - yes
	static void enableIPodMusic();
	static void launchUpsellScreen(bool fromMenu_or_afterGame); //fromMenu(1), afterGame(0)
	static void showCommunityButton(bool bShow);
	static void hideCommunityButtonWithAnimation();
	static bool showAchievementAlertIfNecessary();
	static bool isCommunityButtonPressed();
	static bool isAchievementAlertShown();
	static void sendAchievement(int type, float percent);
	static void sendScore(int game_type, int stage_id, unsigned int score, int min, int sec, int msec);
	static void sendTotalTimeAttackTime();
	static const char* getLBUkey(int stage_id);
	static void playSFX_ButtonPress();
	static void playSFX_WindowOpen();
	static void disableSpeedAchievementAfterTIMEOVER(bool trueForDisable);
	static bool isSpeedAchievementAfterTIMEOVERDisabled();
#ifndef SONIC4_TRIAL	
	static bool isDrawMainMenu();
	static bool isCommunityEnabled(); //supported & not cancelled by user
	static void startCommunityLoadingIndicator();
	static void stopCommunityLoadingIndicator();
	static bool isCommunityLoadedOrUnsupportedOrCancelled();
	static void regAndDontShowBtn();
	//static void isAchievemntAlreadySetInPreviousGame(int achID);
	static const char* getFilenameForGKLocalPlayerID(const char* file_name_mask); //file_name_mask must contain %08x (for example "myfile_%08x.dat")
#else
  #ifdef SONIC4_TRIAL_EXIBITION
	static void showDemoSplashForTrial();
	static bool isDemoSplashShown();
  #endif
	static const char* getFilenameForGKLocalPlayerID(const char* file_name_mask){return NULL;} //file_name_mask must contain %08x (for example "myfile_%08x.dat")	
#endif
};


struct mppStorageReader //read file with crc
{
	mppStorageReader():f(NULL),m_crc(0){}
	bool open(const char* path);	
	bool readRaw(void* buf, const int bufSize);
	template <class T> bool read(T& dst) {return readRaw(&dst, sizeof(T));}
	bool readAndCheckCrc();
	void close();
protected:
	int m_crc;
	FILE* f;
};

struct mppStorageWriter //save file with crc
{
	mppStorageWriter():f(NULL),m_crc(0){}
	bool open(const char* path);	
	void writeRaw(const void* buf, const int bufSize);
	template <class T> void write(const T& dst) { writeRaw(&dst, sizeof(T));}
	void writeCrc();
	void close();
protected:
	int m_crc;
	FILE* f;
};		

#endif
		

