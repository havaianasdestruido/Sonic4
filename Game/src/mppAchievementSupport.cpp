/*
 *  mppAchievementSupport.cpp
 *  hog
 *
 *  Created by Vyacheslav Mednonogov on 9/30/10.
 *  Copyright 2010 MPP. All rights reserved.
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmMain.h"
#include "gsMainSys.h"
#include "gmGameDat.h"
#include "gmRing.h"
#include "gmCamera.h"

#include "gmEventMgr.h"
#include "gmEventTbl.h"
#include "dmCmnBackup.h"
#include "dmTitle.h"
//----- mpp -----------------------------------------------------------------

#include "mppUtil.h"
#include "mppAchievementSupport.h"
#include "mppTimeScores.h"

//---------------------------------------------------------------------------
static const int BITMASK__ALL_GENERAL_STAGES__NO_BOSS = (0
	| (1<<GSD_MAIN_STAGE_ID_1_1)		
	| (1<<GSD_MAIN_STAGE_ID_1_2)		
	| (1<<GSD_MAIN_STAGE_ID_1_3)		
	// (1<<GSD_MAIN_STAGE_ID_1_BOSS)
	| (1<<GSD_MAIN_STAGE_ID_2_1)
	| (1<<GSD_MAIN_STAGE_ID_2_2)		
	| (1<<GSD_MAIN_STAGE_ID_2_3)		
	// (1<<GSD_MAIN_STAGE_ID_2_BOSS)
	| (1<<GSD_MAIN_STAGE_ID_3_1)
	| (1<<GSD_MAIN_STAGE_ID_3_2)		
	| (1<<GSD_MAIN_STAGE_ID_3_3)		
	// (1<<GSD_MAIN_STAGE_ID_3_BOSS)
	| (1<<GSD_MAIN_STAGE_ID_4_1)
	| (1<<GSD_MAIN_STAGE_ID_4_2)		
	| (1<<GSD_MAIN_STAGE_ID_4_3)		
	// (1<<GSD_MAIN_STAGE_ID_4_BOSS)
);
static const int BITMASK__ALL_GENERAL_STAGES = (0
	| (1<<GSD_MAIN_STAGE_ID_1_1)		
	| (1<<GSD_MAIN_STAGE_ID_1_2)		
	| (1<<GSD_MAIN_STAGE_ID_1_3)		
	| (1<<GSD_MAIN_STAGE_ID_1_BOSS)
	| (1<<GSD_MAIN_STAGE_ID_2_1)
	| (1<<GSD_MAIN_STAGE_ID_2_2)		
	| (1<<GSD_MAIN_STAGE_ID_2_3)		
	| (1<<GSD_MAIN_STAGE_ID_2_BOSS)
	| (1<<GSD_MAIN_STAGE_ID_3_1)
	| (1<<GSD_MAIN_STAGE_ID_3_2)		
	| (1<<GSD_MAIN_STAGE_ID_3_3)		
	| (1<<GSD_MAIN_STAGE_ID_3_BOSS)
	| (1<<GSD_MAIN_STAGE_ID_4_1)
	| (1<<GSD_MAIN_STAGE_ID_4_2)		
	| (1<<GSD_MAIN_STAGE_ID_4_3)		
	| (1<<GSD_MAIN_STAGE_ID_4_BOSS)
);
static const int BITMASK__ALL_FINAL_STAGES = (0     
	| (1<<GSD_MAIN_STAGE_ID_FINAL_1)
);
static const int BITMASK__ALL_SPECIAL_STAGES = (0
	| (1<<GSD_MAIN_STAGE_ID_SS1)
	| (1<<GSD_MAIN_STAGE_ID_SS2)
	| (1<<GSD_MAIN_STAGE_ID_SS3)
	| (1<<GSD_MAIN_STAGE_ID_SS4)
	| (1<<GSD_MAIN_STAGE_ID_SS5)
	| (1<<GSD_MAIN_STAGE_ID_SS6)
	| (1<<GSD_MAIN_STAGE_ID_SS7)
);
static const int BITMASK__ALL_STAGES = 
	(BITMASK__ALL_GENERAL_STAGES | BITMASK__ALL_SPECIAL_STAGES | BITMASK__ALL_FINAL_STAGES);

//---------------------------------------------------------------------------


#define ACHA_FNAME (mppUtil::getFilenameForGKLocalPlayerID("ac_lst_%08x.mpp"))
static const int mppACHA_VERSION =  (0xACA007);

mppAchievementSupport mppAchievementSupport::g_inst;
bool mppAchievementSupport::g_isInitialized = false;

enum FLAGS_FOR_ONE_TIME_EVENTS
{
	FLAG_Sonic4_StoryBegins = (1 << Sonic4_StoryBegins),   
	FLAG_Sonic4_Eggman = (1 << Sonic4_Eggman),   
	FLAG_Sonic4_Chaos1 = (1 << Sonic4_Chaos1), 
	FLAG_Sonic4_Enemy = (1 << Sonic4_Enemy), 
	FLAG_Sonic4_Golden = (1 << Sonic4_Golden),
	FLAG_Sonic4_Cleared = (1 << Sonic4_Cleared), 
	FLAG_Sonic4_Contender = (1 << Sonic4_Contender),
	FLAG_Sonic4_Collector = (1 << Sonic4_Collector),
	FLAG_Sonic4_Immortal = (1 << Sonic4_Immortal),
	FLAG_Sonic4_Super = (1 << Sonic4_Super),
	FLAG_Sonic4_Speed = (1 << Sonic4_Speed),  
	FLAG_Sonic4_Untouchable = (1 << Sonic4_Untouchable)  	
	
};


inline int numberOfSetBits(int i) //known as 'parallel' or 'variable-precision SWAR algorithm'
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return ((i + (i >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

mppAchievementSupport* mppAchievementSupport::get() {
	if(!g_isInitialized) {
		g_isInitialized = true;
		if(!g_inst.loadData()) {
			g_inst.saveData();
		}		
	}
	return &g_inst;
}//singletone


mppAchievementSupport::mppAchievementSupport()
	: m_flags(0)
	, m_flags_alertDlg(0)
	, m_counters(COUNTERS()) {
	}

void mppAchievementSupport::registerAchievement(int eFlag)
{	
	setFlag(eFlag);
	saveData();
}

void mppAchievementSupport::checkFlagAndRegisterAchievement(int eFlag){
	if(!checkFlag(eFlag)) {
		registerAchievement(eFlag);
	}
}


////////////////////////////////


bool mppAchievementSupport::isDataExist()
{
#ifndef SONIC4_TRIAL
	bool res = mppStorageUtil::isFileExist(ACHA_FNAME);
	//OS_TPrintf("mppAchievementSupport::isDataExist... res=%i\n", (int)res);	
	return res;
#else
	return false;
#endif
}

/*
void mppAchievementSupport::removeData()
{
#ifndef SONIC4_TRIAL	
	//OS_TPrintf("mppAchievementSupport::removeData...\n");
	mppStorageUtil::deleteFile(ACHA_FNAME);
#endif
}*/

//////////////////////////////////////

void mppAchievementSupport::saveData()
{
#ifndef SONIC4_TRIAL
	//OS_TPrintf("mppCheckPointStorage::saveState()... mode=%X\n", save_mode);
	//
	//open
	mppStorageWriter sw;
	sw.open(ACHA_FNAME);
	//
	//save version
	sw.write(mppACHA_VERSION);
	//save data
	sw.write(m_flags);
	sw.write(m_flags_alertDlg);
	sw.write(m_counters);
	//crc
	sw.writeCrc();
	//
	//close
	sw.close();	
#endif
}

bool mppAchievementSupport::loadData()
{
#ifndef SONIC4_TRIAL	
	mppTimeScores::timeScores.Load();
	//
	if(!isDataExist()) {
		return false;
	}
	OS_TPrintf("mppCheckPointStorage::loadState()...\n");
	//
	//open
	mppStorageReader sr;
	mppAchievementSupport _tmp;
	sr.open(ACHA_FNAME);
	//
	//check version & mode
	int ver = -1;
	sr.read(ver);
	if(ver!=mppACHA_VERSION) {OS_TPrintf("wrong version\n"); return false;}
	sr.read(_tmp.m_flags);
	sr.read(_tmp.m_flags_alertDlg);
	sr.read(_tmp.m_counters);
	//
	if(!sr.readAndCheckCrc()) {OS_TPrintf("wrong crc\n"); return false;}
	//
	sr.close();	
	memcpy(&g_inst, &_tmp, sizeof(g_inst));
	OS_TPrintf("ok\n");
	return true;
#else
	return false;
#endif
}

////////////////////// process events /////////////////////////////////////

void mppAchievementSupport::event_SuccessEndOf1stLevel(int timeInSecX60) //for Sonic4_StoryBegins,	Sonic4_Speed,  
{
	checkFlagAndRegisterAchievement(FLAG_Sonic4_StoryBegins);
	const int ONE_MINUTE = 60 * 60; //(sonic team uses 1/60 sec instead 1/100 sec)
	if(timeInSecX60 < ONE_MINUTE) { //!! "less than a minute" is [00:00:00 to 00:59:99]
		if(mppUtil::isSpeedAchievementAfterTIMEOVERDisabled()) {
			return; //do not register this achievemnt after [TIMEOVER]
		}		
		checkFlagAndRegisterAchievement(FLAG_Sonic4_Speed);
	}
}
void mppAchievementSupport::event_DefeatBoss() //for Sonic4_Eggman,   
{
	checkFlagAndRegisterAchievement(FLAG_Sonic4_Eggman);
}
void mppAchievementSupport::event_ChaosEmeraldCollected() //for Sonic4_Chaos1, Sonic4_Super,
{
	if(g_gs_main_sys_info.stage_id == GSD_MAIN_STAGE_ID_SS1) //sss[144]
	{
		checkFlagAndRegisterAchievement(FLAG_Sonic4_Chaos1);
	}
	//
	if(!checkFlag(FLAG_Sonic4_Super)) {
		const int specialStageN = g_gs_main_sys_info.stage_id;
		m_counters.bitmaskEmeralds |= (1<<specialStageN);
		saveData();
		getPercentageAndSetFlagIf100(FLAG_Sonic4_Super);
	}
}
void mppAchievementSupport::event_DefeatEnemy()//for Sonic4_Enemy, 
{
	if(!checkFlag(FLAG_Sonic4_Enemy))
	{
		m_counters.numOfDefeatedEnemies++;
		//sss[143] probably fixed// if((m_counters.numOfDefeatedEnemies&0x7)==0) 
		{
			saveData();
		}
		getPercentageAndSetFlagIf100(FLAG_Sonic4_Enemy);
	}
}

void mppAchievementSupport::event_SuccessEndOfAnyLevelAsSuperSonic(int level, bool mode_TimeAttack) // Sonic4_Golden,
{
	if(!checkFlag(FLAG_Sonic4_Golden))
	{
		m_counters.bitmaskSuperSonicClearedLevels |= (1<<level);
		saveData();
		getPercentageAndSetFlagIf100(FLAG_Sonic4_Golden);	
	}	
}
void mppAchievementSupport::event_SuccessEndOfAnyLevel(int level, bool mode_TimeAttack) //Sonic4_Contender,
{
	if(!checkFlag(FLAG_Sonic4_Contender)) {
		/*
		if(mode_TimeAttack) {
			m_counters.bitmaskClearedLevels_TimeAttack |= (1<<level);
		}
		else {
			m_counters.bitmaskClearedLevels_Story      |= (1<<level);
		}
		saveData();
		getPercentageAndSetFlagIf100(FLAG_Sonic4_Contender);
		 */
		if(mode_TimeAttack) {
			m_counters.bitmaskClearedLevels_TimeAttack |= (1<<level);
			saveData();
			getPercentageAndSetFlagIf100(FLAG_Sonic4_Contender);
		}
	}
}
void mppAchievementSupport::event_SuccessEndOfSpecialStage1(bool allRingsCollected) // Sonic4_Collector,
{
	if(allRingsCollected) {
		checkFlagAndRegisterAchievement(FLAG_Sonic4_Collector);
	}
}
void mppAchievementSupport::event_CheckLifeCount() //for Sonic4_Immortal,
{
	if(!checkFlag(FLAG_Sonic4_Immortal))
	{
		//test{{:if(g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P]<97) g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P]=97;:}}
		const int cur_lives = g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P];
		if(m_counters.numOfPlayerLives < cur_lives)
		{
			m_counters.numOfPlayerLives = cur_lives;
			saveData();
			getPercentageAndSetFlagIf100(FLAG_Sonic4_Immortal);
		}
	}
}
void mppAchievementSupport::event_LoseRingFinalLevelCounterClear(bool forceClear) //(1) Sonic4_Untouchable
{
	const int game_level = g_gs_main_sys_info.stage_id; //stage & zone combination
	if (game_level==GSD_MAIN_STAGE_ID_FINAL_1 || forceClear) {
		m_counters.numOfDamagesAtFinalLevel = 0;
		saveData();
	}
}
void mppAchievementSupport::event_LoseRingInFinalLevel() //(2) Sonic4_Untouchable
{
	const int game_level = g_gs_main_sys_info.stage_id; //stage & zone combination
	if (game_level==GSD_MAIN_STAGE_ID_FINAL_1) {
		m_counters.numOfDamagesAtFinalLevel++;
		if(m_counters.numOfDamagesAtFinalLevel<=6) {
			saveData();
		}
	}
}
void mppAchievementSupport::event_SuccessEndOfFinalLevel() //for (3) Sonic4_Untouchable  and Sonic4_Cleared, 
{
	checkFlagAndRegisterAchievement(FLAG_Sonic4_Cleared);
	checkFlagAndRegisterAchievement(FLAG_Sonic4_Eggman); //sss[143]
	if(m_counters.numOfDamagesAtFinalLevel==0) {
		checkFlagAndRegisterAchievement(FLAG_Sonic4_Untouchable);
	}
	else {
		m_counters.numOfDamagesAtFinalLevel = 0;
		saveData();
	}
}

//////////////////////////////////////////---------------------------


float mppAchievementSupport::getPercentageAndSetFlagIf100(int eFlag)
{
	const int P100 = 100;
	//
	if(!checkFlag(eFlag)) {
		float pcnt = 0;
		switch(eFlag) {
			case FLAG_Sonic4_Enemy: {
				pcnt = (m_counters.numOfDefeatedEnemies*P100)/1000;
			}break;
			case FLAG_Sonic4_Golden: {
				const int MAX_BITS = numberOfSetBits(BITMASK__ALL_GENERAL_STAGES__NO_BOSS);
				const int numOfBits = numberOfSetBits(m_counters.bitmaskSuperSonicClearedLevels);
				pcnt = (numOfBits*P100)/MAX_BITS;
			}break;
			case FLAG_Sonic4_Contender: {
				/*
				const int MAX_BITS = 2*numberOfSetBits(BITMASK__ALL_STAGES);
				const int numOfBits = numberOfSetBits(m_counters.bitmaskClearedLevels_Story)
									+ numberOfSetBits(m_counters.bitmaskClearedLevels_TimeAttack);
				 */
				const int MAX_BITS = numberOfSetBits(BITMASK__ALL_STAGES);
				const int numOfBits = numberOfSetBits(m_counters.bitmaskClearedLevels_TimeAttack);				
				pcnt = (numOfBits*P100)/MAX_BITS;
			}break;
			case FLAG_Sonic4_Immortal: {
				pcnt = (m_counters.numOfPlayerLives*P100)/(100); //not 99, because HUD shows number_of_lifes-1
			}break;
			case FLAG_Sonic4_Super: {
				const int MAX_BITS = 7;
				const int numOfBits = numberOfSetBits(m_counters.bitmaskEmeralds);
				pcnt = (numOfBits*P100)/MAX_BITS;				
			}break;
			default: {
				//error
			}break;
		}
		//clamp
		if(pcnt<0) pcnt = 0;
		if(pcnt>=P100) {
			pcnt = P100;
			//set flag if necessary
			checkFlagAndRegisterAchievement(eFlag);
		}
		return float(pcnt);
	}
	//
	return float(P100);
}

void mppAchievementSupport::flushAchievementsToGlobalNet()
{
#ifndef SONIC4_TRIAL
	////if(mppUtil::isCommunityEnable()) 
	{
		const float FULL_PERC = 100.0f;
		//
		if(checkFlag(FLAG_Sonic4_StoryBegins)) mppUtil::sendAchievement(Sonic4_StoryBegins, FULL_PERC);;  //no percenage
		//
		if(checkFlag(FLAG_Sonic4_Eggman)) mppUtil::sendAchievement(Sonic4_Eggman, FULL_PERC);;  //no percenage    
		//
		if(checkFlag(FLAG_Sonic4_Chaos1)) mppUtil::sendAchievement(Sonic4_Chaos1, FULL_PERC);;  //no percenage 	
		//
		mppUtil::sendAchievement(Sonic4_Enemy, getPercentageAndSetFlagIf100(FLAG_Sonic4_Enemy));;  //pct
		//
		mppUtil::sendAchievement(Sonic4_Golden, getPercentageAndSetFlagIf100(FLAG_Sonic4_Golden));;  //pct 
		//
		if(checkFlag(FLAG_Sonic4_Cleared)) mppUtil::sendAchievement(Sonic4_Cleared, FULL_PERC);;   //no percenage
		//
		mppUtil::sendAchievement(Sonic4_Contender, getPercentageAndSetFlagIf100(FLAG_Sonic4_Contender));;  //pct 
		//
		if(checkFlag(FLAG_Sonic4_Collector)) mppUtil::sendAchievement(Sonic4_Collector, FULL_PERC);;  //no percenage
		//
		mppUtil::sendAchievement(Sonic4_Immortal, getPercentageAndSetFlagIf100(FLAG_Sonic4_Immortal));;  //pct 
		//
		mppUtil::sendAchievement(Sonic4_Super, getPercentageAndSetFlagIf100(FLAG_Sonic4_Super));;  //pct
		//
		if(checkFlag(FLAG_Sonic4_Speed)) mppUtil::sendAchievement(Sonic4_Speed, FULL_PERC);; //no percenage  
		//
		if(checkFlag(FLAG_Sonic4_Untouchable)) mppUtil::sendAchievement(Sonic4_Untouchable, FULL_PERC);;  //no percenage 
	}
	mppTimeScores::timeScores.Send();
#endif
}

int mppAchievementSupport::getAchievementAlertID()
{
	if(m_flags != m_flags_alertDlg) {
		for(int i=0; i<Sonic4_MAX_ACHIEVEMENT_; i++) {
			const int bit = 1<<i;
			if( (m_flags&bit)!=0 && (m_flags_alertDlg&bit)==0 ) {
				m_flags_alertDlg |= bit;
				saveData();	
				return i;
			}
		}
	}
	return -1;
}

void mppAchievementSupport::combineAchievementFlags(int prevFlags)
{
	if(m_flags_alertDlg!=prevFlags) {//to avoid problems with new ach. alerts after reinstall aplication
		m_flags |= prevFlags;
		m_flags_alertDlg |= prevFlags;
		saveData();
	}
}



//-------------------------------------------------------------------

void mpp_checkAchievementsForSuccessLevelEnd()
{
#ifndef SONIC4_TRIAL
	mppAchievementSupport *pASUP = mppAchievementSupport::get();
	GMS_PLAYER_WORK* pPlayer = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	const int game_level =g_gs_main_sys_info.stage_id; //stage & zone combination		
	const bool mode_TimeAttack = (g_gs_main_sys_info.game_mode==GSD_GAME_MODE_TIME_ATTACK); //mode: normal or time attack	
	const int level_timeInSecX60 = g_gm_main_system.game_time;
	
	//end of any level
	pASUP->event_SuccessEndOfAnyLevel(game_level, mode_TimeAttack);
	pASUP->event_CheckLifeCount();
	
	//end of beginning level
	if(game_level==GSD_MAIN_STAGE_ID_1_1) {
		pASUP->event_SuccessEndOf1stLevel(level_timeInSecX60);
	}
	
	//end as super sonic
	if( (pPlayer->player_flag & GMD_PLF_SUPER_SONIC)!=0) { 
		pASUP->event_SuccessEndOfAnyLevelAsSuperSonic(game_level, mode_TimeAttack);
	}
	
	//end special stage 1
	if(game_level==GSD_MAIN_STAGE_ID_SS1)	{
		const int level_ring_num = GmEventMgrGetRingNum();
		const bool isAllRingCollected = pPlayer->ring_num>=level_ring_num;
		pASUP->event_SuccessEndOfSpecialStage1(isAllRingCollected);
	}
	
	//end final
	if (game_level==GSD_MAIN_STAGE_ID_FINAL_1) {
		pASUP->event_SuccessEndOfFinalLevel();
	}
	pASUP->event_LoseRingFinalLevelCounterClear(true);	//call always after stage end
	
	pASUP->saveData();
	mppTimeScores::timeScores.Save();
#endif	
 }


void mpp_flushAchievementsToGlobalNet(bool showAchievementAlertIfPresent)
{
#ifndef SONIC4_TRIAL
	if(showAchievementAlertIfPresent) {
		mppUtil::showAchievementAlertIfNecessary();
	}
	//
	mppAchievementSupport *pASUP = mppAchievementSupport::get();
	pASUP->flushAchievementsToGlobalNet();
#endif
}


#define DMD_STGSLCT_INIT_RECORD_TIME_NUM (36000)

bool mpp_calcTotalTimeAttackTime(int& min, int& sec, int& sec100) { //true - all levels opened

	bool success = false;
#ifndef SONIC4_TRIAL
	
	mppAchievementSupport *pASUP = mppAchievementSupport::get();
	if(pASUP->checkFlag(FLAG_Sonic4_Contender)) {
	
		gs::backup::SStage &data
			= gs::backup::SStage::CreateInstance();
		
		int total_time = 0;
		success = true;
		
		//====TIME ATTACK -- 16 STAGES + 1 FINAL
		
		
		for (u32 i = 0; i <= GSD_MAIN_STAGE_ID_FINAL_1; i++) {
			
			// 通常ソニックのレコード取得
			int nn_time = (s32)data[i].GetFastTime(false);
			
			// スーパーソニックのレコード取得
			int ss_time = (s32)data[i].GetFastTime(true);
			
			int ff_time = 0;
			
			// ２種のソニックのレコードタイムがどちらも初期値でない場合
			if (nn_time != DMD_STGSLCT_INIT_RECORD_TIME_NUM
				&& ss_time != DMD_STGSLCT_INIT_RECORD_TIME_NUM) {
				// ハイスコアが高い方を設定
				if (nn_time <= ss_time) {
					// 通常ソニックの方が高い場合(同値含む)
					ff_time = nn_time;
				}
				else {
					// スーパーソニックの方が高い場合
					ff_time = ss_time;
				}
			}
			
			else {
				if (nn_time == DMD_STGSLCT_INIT_RECORD_TIME_NUM
					&& ss_time == DMD_STGSLCT_INIT_RECORD_TIME_NUM) {
					// 初期値設定(どちらも初期値のため、通常ソニックの値を設定)
					ff_time = nn_time;
					success = false;
				}			
				else if (nn_time == DMD_STGSLCT_INIT_RECORD_TIME_NUM) {
					// 通常ソニックが初期値のため、スパソニのスコアを設定
					ff_time = ss_time;
				}
				else if (ss_time == DMD_STGSLCT_INIT_RECORD_TIME_NUM) {
					// スパソニが初期値のため、通常ソニックのスコアを設定
					ff_time = nn_time;
				}
				else {
					MTM_ASSERT(0);
					// ここに来る場合はありえないためASSERTをかけ、保険で通常ソニックの値を設定
					ff_time = nn_time;
					success = false;
				}
			}
			
			total_time +=ff_time;
		}
		
		//====TIME ATTACK -- 7 SPECIAL STAGES
		gs::backup::SSpecial &spe_data
			= gs::backup::SSpecial::CreateInstance();
		
		for (u32 i = 0; i < 7; i++) {
		
			// スペシャルステージのレコードタイム取得
			int ff_time = (s32)spe_data[i].GetFastTime();
			
			if(ff_time == DMD_STGSLCT_INIT_RECORD_TIME_NUM) {
				success = false;
			}
			else {
				ff_time = 90*60 - ff_time; //sub from 1'30"
			}
			
			total_time += ff_time;
		}	
		
		//====TOTAL RESULT
		{{
			sec100 = ((total_time%60)*100)/60; //sss[94] bugfix (sonic team uses 1/60 sec instead 1/100 sec)
			total_time/=60;
			sec = total_time%60;
			total_time/=60;
			min = total_time;
		}}
	}
	
#endif	
	return (success);

}
		
#define FLAU_FNAME "cfeet.zxs"

static bool needToForceSendTimesToLeaderboard_ = false;

void mppTimeScores::SetLeaderboardTimesFromGameAtFirstLaunchIfNecessary() { //true - all levels opened
	
#ifndef SONIC4_TRIAL
	const bool isFirstLaunch = (mppStorageUtil::isFileExist(FLAU_FNAME)==false); //is marker file present?
	
	if(isFirstLaunch) {
		
		{{//save marker file (with dummy info)
			mppStorageWriter sw;
			sw.open(FLAU_FNAME);
			sw.write(1);
			sw.writeCrc();
			sw.close();	
		}}
		
		gs::backup::SStage &data
		= gs::backup::SStage::CreateInstance();
		
		
		//====TIME ATTACK -- 16 STAGES + 1 FINAL
		
		
		for (u32 i = 0; i <= GSD_MAIN_STAGE_ID_FINAL_1; i++) {
			
			bool success = true;
			
			// 通常ソニックのレコード取得
			int nn_time = (s32)data[i].GetFastTime(false);
			
			// スーパーソニックのレコード取得
			int ss_time = (s32)data[i].GetFastTime(true);
			
			int ff_time = 0;
			
			// ２種のソニックのレコードタイムがどちらも初期値でない場合
			if (nn_time != DMD_STGSLCT_INIT_RECORD_TIME_NUM
				&& ss_time != DMD_STGSLCT_INIT_RECORD_TIME_NUM) {
				// ハイスコアが高い方を設定
				if (nn_time <= ss_time) {
					// 通常ソニックの方が高い場合(同値含む)
					ff_time = nn_time;
				}
				else {
					// スーパーソニックの方が高い場合
					ff_time = ss_time;
				}
			}
			
			else {
				if (nn_time == DMD_STGSLCT_INIT_RECORD_TIME_NUM
					&& ss_time == DMD_STGSLCT_INIT_RECORD_TIME_NUM) {
					// 初期値設定(どちらも初期値のため、通常ソニックの値を設定)
					ff_time = nn_time;
					success = false;
				}			
				else if (nn_time == DMD_STGSLCT_INIT_RECORD_TIME_NUM) {
					// 通常ソニックが初期値のため、スパソニのスコアを設定
					ff_time = ss_time;
				}
				else if (ss_time == DMD_STGSLCT_INIT_RECORD_TIME_NUM) {
					// スパソニが初期値のため、通常ソニックのスコアを設定
					ff_time = nn_time;
				}
				else {
					MTM_ASSERT(0);
					// ここに来る場合はありえないためASSERTをかけ、保険で通常ソニックの値を設定
					ff_time = nn_time;
					success = false;
				}
			}
			
			if(success==true) {
				int time100 = ff_time*100/60; //(sonic team uses 1/60 sec instead 1/100 sec)
				Set(i+000000, time100);
			}
		}
		
		//====TIME ATTACK -- 7 SPECIAL STAGES
		gs::backup::SSpecial &spe_data
		= gs::backup::SSpecial::CreateInstance();
		
		for (u32 i = 0; i < 7; i++) {
			
			bool success=true;
			
			// スペシャルステージのレコードタイム取得
			int ff_time = (s32)spe_data[i].GetFastTime();
			
			if(ff_time == DMD_STGSLCT_INIT_RECORD_TIME_NUM) {
				success = false;
			}
			else {
				ff_time = 90*60 - ff_time; //sub from 1'30"
			}
			
			if(success==true) {
				int time100 = ff_time*100/60; //(sonic team uses 1/60 sec instead 1/100 sec)
				Set(i+GSD_MAIN_STAGE_ID_SS1, time100);
			}
		}	
		
		needToForceSendTimesToLeaderboard_ = true;
			
	}
	
#endif	
	
	
}

bool  mppTimeScores::isNeedToForceSendTimesToLeaderboard() {
	
	bool res = needToForceSendTimesToLeaderboard_;
	needToForceSendTimesToLeaderboard_ = false;
	return res;
	
}




		
