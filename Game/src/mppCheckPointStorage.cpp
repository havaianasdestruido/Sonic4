/*
 *  mppCheckPointStorage.cpp
 *  hog
 *
 *  Created by Vyacheslav Mednonogov on 6/29/10.
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
#include "dmSound.h"

//----- mpp -----------------------------------------------------------------

#include "mppUtil.h"
#include "mppCheckPointStorage.h"

#ifdef SONIC4_TRIAL
////#define CP_FNAME "tr1_cp_st.mpp"
////static const int mppCPS_VERSION =  (0xA77003);
#else
#define CP_FNAME "cp_st.mpp"
static const int mppCPS_VERSION =  (0xC55003);
#endif

bool mppCheckPointStorage::isStateExist()
{
#ifndef SONIC4_TRIAL
	bool res = mppStorageUtil::isFileExist(CP_FNAME);
	OS_TPrintf("mppCheckPointStorage::isStateExist... res=%i\n", (int)res);	
	return res;
#else
	return false;
#endif
}
void mppCheckPointStorage::removeState()
{
#ifndef SONIC4_TRIAL
	OS_TPrintf("mppCheckPointStorage::removeState()...\n");
	mppStorageUtil::deleteFile(CP_FNAME);
#endif
}

struct mppGAME_DATA
{
	MPP_CHECKPOINT_STATE_HEADER myHeader; //must be first
	GMS_PLAYER_WORK ply_work;
	GMS_MAIN_SYSTEM main_system;
	GSS_MAIN_SYS_INFO main_sysinfo;
} mppGD;


void mppCPS_PushPlayerData()
{
	//OS_TPrintf("mppCPS_PushPlayerData()...\n");
	//
	mppGD.myHeader.stage_id = g_gs_main_sys_info.stage_id;
	mppGD.myHeader.level = g_gs_main_sys_info.level;		
	mppGD.myHeader.game_mode = g_gs_main_sys_info.game_mode;	
	//
	memcpy(&(mppGD.main_system), &(g_gm_main_system), sizeof(GMS_MAIN_SYSTEM));
	memcpy(&(mppGD.main_sysinfo), &(g_gs_main_sys_info), sizeof(GSS_MAIN_SYS_INFO));
	//
	GMS_PLAYER_WORK	*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	memcpy(&(mppGD.ply_work), ply_work, sizeof(GMS_PLAYER_WORK));
	
}


void mppCPS_PopPlayerData()
{	
	OS_TPrintf("mppCPS_PopPlayerData()...\n");
	g_gm_main_system.time_save      =mppGD.main_system.time_save; //time
	g_gm_main_system.resume_pos_x   =mppGD.main_system.resume_pos_x; //checkpoint pos
	g_gm_main_system.resume_pos_y   =mppGD.main_system.resume_pos_y; //checkpoint pos
	g_gm_main_system.marker_pri     =mppGD.main_system.marker_pri; //checkpoint #
	g_gm_main_system.water_level    =mppGD.main_system.water_level;	//water level (or 0xFFFF)	
	g_gm_main_system.pseudofall_dir =mppGD.main_system.pseudofall_dir;//??
	//g_gm_main_system.boss_load_no   =mppGD.main_system.boss_load_no;//boss number
	//
	g_gm_main_system.game_time = g_gm_main_system.time_save;  //time
	//
	g_gm_main_system.player_rest_num[GSD_MAIN_PLAYER_1P] 
	  = mppGD.main_system.player_rest_num[GSD_MAIN_PLAYER_1P]; //lives
		
}


void mppCPS_PopPlayerData_FinalPhase()
{
	OS_TPrintf("mppCPS_PopPlayerData_FinalPhase()...\n");
	//
	GMS_PLAYER_WORK* pPW = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	pPW->ring_num = mppGD.ply_work.ring_num;	//rings
	pPW->ring_stage_num = mppGD.ply_work.ring_stage_num;	
	pPW->score = mppGD.ply_work.score; //score
	//
	//pPW->invincible_timer = mppGD.ply_work.				invincible_timer;			
	//pPW->genocide_timer = mppGD.ply_work.					genocide_timer;				//!< アイテム後無敵タイマー
	//pPW->pressure_timer = mppGD.ply_work.					pressure_timer;				//!< 押しつぶされ死亡タイマ
	//pPW->disapprove_item_catch_timer = mppGD.ply_work.	disapprove_item_catch_timer;//!< アイテムが取れないタイマー(連続引き寄せを起こさないためにに引き寄せられた時に使用)
	//pPW->water_timer = mppGD.ply_work.					water_timer;				//!< 水中タイマ
	//pPW->no_key_timer = mppGD.ply_work.					no_key_timer;				//!< キー入力無効タイマー
	//pPW->homing_timer = mppGD.ply_work.					homing_timer;				//!< ホーミング有効化待機タイマー
	//pPW->hi_speed_timer = mppGD.ply_work.					hi_speed_timer;				//!< ハイスピード状態
	//pPW->homing_boost_timer = mppGD.ply_work.				homing_boost_timer;			//!< ホーミング範囲ブーストタイマー
	//pPW->fall_timer = mppGD.ply_work.						fall_timer;					//!< 落下タイマー	(プレイヤー固有動作値)
	//pPW->no_jump_move_timer = mppGD.ply_work.				no_jump_move_timer;			//!< ジャンプ移動不可解除タイマー
	//pPW->maxdash_timer = mppGD.ply_work.					maxdash_timer;				//!< 最大ダッシュ状態解除タイマー
	//pPW->super_sonic_ring_timer = mppGD.ply_work.			super_sonic_ring_timer;		//!< スーパーソニック用リング減算タイマー	
	//
    GmCameraPosSet(g_gm_main_system.resume_pos_x, g_gm_main_system.resume_pos_y, 0); //set camera	
	//load obj under camera (see gmGmkStart)
	OBS_CAMERA	*obj_camera = ObjCameraGet(g_obj.glb_camera_id);
	// オブジェクトカメラ設定
	ObjObjectCameraSet(FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
					   FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)),
					   FXM_FLOAT_TO_FX32(obj_camera->disp_pos.x - (float)(OBD_LCD_X/2)),
					   FXM_FLOAT_TO_FX32(-obj_camera->disp_pos.y - (float)(OBD_LCD_Y/2)));
	// クリッピングカメラ設定
	GmCameraSetClipCamera(obj_camera);

	//GmEventMgrCreateEventEnforce();//sss - for truck?
	//
	if((GMD_PLF_SUPER_SONIC&mppGD.ply_work.player_flag)!=0) { //for super sonic mode
		OS_TPrintf("S.S. mode detected.. \n");
		//sss magic patch
		extern void gmPlySeqTransformSuperMain(GMS_PLAYER_WORK *ply_work);
		pPW->obj_work.user_timer = (60+1)*FX32_ONE;//farsh? 
		gmPlySeqTransformSuperMain(pPW);
		pPW->obj_work.user_timer = (0)*FX32_ONE;//farsh?
		gmPlySeqTransformSuperMain(pPW);
		
		//g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_BGM_PLAY_ENABLE; //disable bgm

	}
	
	if(g_gm_main_system.boss_load_no==-1) {//load final boss if necessary
		if(mppGD.main_system.boss_load_no!=-1) {
			GmMainDatLoadBossBattleStart(mppGD.main_system.boss_load_no);
		}
	}
}

//////////////////////////////////////

void mppCheckPointStorage::saveState(int save_mode)
{
#ifndef SONIC4_TRIAL
	//OS_TPrintf("mppCheckPointStorage::saveState()... mode=%X\n", save_mode);
	//
	//open
	mppStorageWriter sw;
	sw.open(CP_FNAME);
	mppEM_ReserveMapData();
	//
	//save version
	sw.write(mppCPS_VERSION);
	//save mode
	int smod = save_mode;
	if(mppUtil::isSpeedAchievementAfterTIMEOVERDisabled()) {
		smod = -smod; //neg for disable achiv
	}
	sw.write(smod);
	//
	//store data to mem	
	if(save_mode==SAVE_AFTER_CHECKPOINT) {
		mppEM_PushMapData();
		mppEM_ReturnSomeEventsBackToMap();
	}
	mppCPS_PushPlayerData();
	//
	//write data to file
	sw.write(mppGD);
	if(save_mode==SAVE_AFTER_CHECKPOINT) {
		mppEM_SaveMapData(sw);
	}
	sw.writeCrc();
	//
	//close
	mppEM_DeleteMapData();
	sw.close();	
#endif
}


static bool needToFinishInitialization = false;

bool mppCheckPointStorage::loadState()
{
#ifndef SONIC4_TRIAL
	OS_TPrintf("mppCheckPointStorage::loadState()...\n");
	//
	//open
	mppStorageReader sr;
	sr.open(CP_FNAME);
	mppEM_ReserveMapData();
	//
	//check version & mode
	int ver = -1;
	sr.read(ver);
	if(ver!=mppCPS_VERSION) {OS_TPrintf("wrong version\n"); return false;}
	int save_mode = 0;
	sr.read(save_mode);
	if(save_mode<0) {//neg: extension for save speed achievement disabling after TIMEOVER 
		save_mode = -save_mode;
		mppUtil::disableSpeedAchievementAfterTIMEOVER(true);
		OS_TPrintf("disableSpeedAchievementAfterTIMEOVER(true)...\n");
	}
	if(save_mode!=SAVE_AFTER_CHECKPOINT && save_mode!=SAVE_AFTER_RESPAWN) {OS_TPrintf("wrong mode\n"); return false;}
	OS_TPrintf("...mode=%X\n",save_mode);
	//
	//read data from file
	if(!sr.read(mppGD)) {OS_TPrintf("wrong game data\n"); return false;}
	if(save_mode==SAVE_AFTER_CHECKPOINT) {
		if(!mppEM_LoadMapData(sr)) {OS_TPrintf("wrong map data\n"); return false;}
	}
	if(!sr.readAndCheckCrc()) {OS_TPrintf("wrong crc\n"); return false;}
	//
	//apply data to game
	if(save_mode==SAVE_AFTER_CHECKPOINT) {
		mppEM_PopMapData();
		//g_gs_main_sys_info.game_flag &= ~GMD_GAME_FLAG_FINAL_DATA_RELEASE;//for final boss reloading
	}
	else {
		g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_RESTART;
	}
	mppCPS_PopPlayerData();
	//
	//close
	mppEM_DeleteMapData();
	sr.close();	
	OS_TPrintf("ok\n");
	needToFinishInitialization = true;
	return true;
#else
	return false;
#endif
}

void mppCheckPointStorage::postInitializeStateIfNecessary1_inputAndSound()
{
	if(needToFinishInitialization) {
		OS_TPrintf("mppCheckPointStorage::postInitializeStateIfNecessary()_inputAndSound()...\n");
		if(DmCmnBackupIsLoadSuccessed()) { //finalize settings/states loadinfg //sss[37]
			mppDT_dmTitleForceApplyLoadSysData();
		}
	}
}



void mppCheckPointStorage::postInitializeStateIfNecessary2_player()
{
	if(needToFinishInitialization) {
		OS_TPrintf("mppCheckPointStorage::postInitializeStateIfNecessary()_player...\n");
		needToFinishInitialization = false;
		//
		mppCPS_PopPlayerData_FinalPhase();	
	}
}

/////////////////////////////

bool mppCheckPointStorage::loadStateHeader(MPP_CHECKPOINT_STATE_HEADER* pHdr)
{
#ifndef SONIC4_TRIAL
	OS_TPrintf("mppCheckPointStorage::loadStateHeader()...\n");
	//
	//open
	mppStorageReader sr;
	sr.open(CP_FNAME);
	//
	//check version & mode
	int ver = -1;
	sr.read(ver);
	if(ver!=mppCPS_VERSION) {OS_TPrintf("wrong version\n"); return false;}
	int save_mode = 0;
	sr.read(save_mode);
	if(save_mode<0) {
		save_mode = -save_mode;//ext
	}
	if(save_mode!=SAVE_AFTER_CHECKPOINT && save_mode!=SAVE_AFTER_RESPAWN) {OS_TPrintf("wrong mode\n"); return false;}
	//
	//read data from file
	if(!sr.read(mppGD.myHeader)) {OS_TPrintf("wrong header\n"); return false;}
	//
	memcpy(pHdr, &(mppGD.myHeader), sizeof(MPP_CHECKPOINT_STATE_HEADER));
	sr.close();	
	return true;
#else
	return false;
#endif
}


bool mppCheckPointStorage::isStateCompatibleWithCurrentGame()
{
	MPP_CHECKPOINT_STATE_HEADER hdr;
	if(!loadStateHeader(&hdr))
		return false;
	bool res = (hdr.stage_id == g_gs_main_sys_info.stage_id
	   && hdr.level == g_gs_main_sys_info.level		
	   && hdr.game_mode == g_gs_main_sys_info.game_mode);
	OS_TPrintf("mppCheckPointStorage::isStateCompatibleWithCurrentGame()... res=%i\n", (int)res);
	return res;
}


int mpp_internal_isNeedToLoadSavedGame = 0;
int mppCheckPointStorage::isNeedToLoadSavedGame()  //-1;0;+1
{
	/*bool res = mpp_internal_isNeedToLoadSavedGame;
	mpp_internal_isNeedToLoadSavedGame = false;
	return res;*/
	return mpp_internal_isNeedToLoadSavedGame;
}


///////////////////////////////////////

void mppUtil::playSFX_ButtonPress()
{
	DmSoundPlaySE("Ok");
}

void mppUtil::playSFX_WindowOpen()
{
	DmSoundPlaySE("Window");
}
