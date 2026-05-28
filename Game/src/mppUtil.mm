//
//  mppUtil.mm
//  hog
//
//  Created by Vyacheslav Mednonogov on jul/1/10.
//  Copyright 2010 MPP. All rights reserved.
//

//----- mpp -----------------------------------------------------------------
#import <MediaPlayer/MediaPlayer.h>
#import <AudioToolbox/AudioToolbox.h>

#import "MoreGamesLauncher.h"

#include "mppCheckPointStorage.h"
#include "mppUtil.h"

#ifdef SONIC4_TRIAL
  #include "mppUpsellViewController.h"
#else
  #import "Community.h"
  #import "mppAchievementAlertViewController.h"
  #import "mppAchievementSupport.h"
  #include "mppTimeScores.h"
#endif

#include "dmSndBgmPlayer.h"


static char fullPathBuf_[1024];

const char* mppStorageUtil::getFullPath(const char* path) {
	NSString* baseFileName = [NSString stringWithFormat:@"%s", path];
    // Convert to OS filename
    NSArray*  paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* docsDirectory = [paths objectAtIndex:0];
    NSString* osFileName = [docsDirectory stringByAppendingPathComponent:baseFileName];
	//copy to result buf
	const char* os_path = [osFileName cStringUsingEncoding:NSASCIIStringEncoding];
	strcpy(fullPathBuf_, os_path);
	NSLOG1((@"full path: %s", fullPathBuf_ ));
	return fullPathBuf_;
}

bool mppStorageUtil::isFileExist(const char* path)
{
	const char* fullPath = mppStorageUtil::getFullPath(path);	
	bool fileExists = [[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithFormat:@"%s", fullPath]];
	NSLOG1((@" mppStorageUtil::isFileExist (%s) : res=%i", path, (int)fileExists ));

	return fileExists;
}
bool mppStorageUtil::deleteFile(const char* path)
{
	const char* fullPath = mppStorageUtil::getFullPath(path);	
	int res = remove(fullPath);
	return (0==res);
}

//--------------------------------------------

#define INITIAL_CRC (0xCFE2010)

inline void _calc_crc(const void* buf, const int bufSize, int& crc)
{
	const char *cbuf = (const char*)buf;

	for(int i=bufSize>>2; --i>=0;) {
		crc^=*(cbuf++);
		crc+=*(cbuf++);
		crc-=*(cbuf++);
		crc^=*(cbuf++);
	}

}


bool mppStorageReader::open(const char* path)
{
	const char* fullPath = mppStorageUtil::getFullPath(path);
	f = fopen(fullPath,"rb");
	NSLOG1((@" mppStorageReader::open (%s) : res=%i", path, (int)(f!=NULL) ));
	m_crc = INITIAL_CRC;
	return (f!=NULL);
}
bool mppStorageReader::readRaw(void* buf, const int bufSize)
{
	if(f==NULL) 
		return false;
	int actualSize = fread(buf, 1, bufSize, f);
	if(actualSize<bufSize) {
		close();
		return false;
	}
	_calc_crc(buf,bufSize,m_crc);
	NSLOG1((@"fread %i bytes", bufSize));
	return true;
}

bool mppStorageReader::readAndCheckCrc() {
	int result_crc = m_crc;
	int file_crc;
	read(file_crc);
	return (result_crc==file_crc);
}

void mppStorageReader::close()
{
	NSLOG1((@"mppStorageReader::close()"));
	if(f!=NULL) {
		fclose(f);
	}
	f=NULL;
}



class Saver {
	char* data;
	int dataSize;
	char* curPtr;
public:
	Saver(int initialSize);
	~Saver();
	void Append(const void* ptr, int size);
	void Clear() { curPtr=data; }
	void save(const char* fileName);
};

Saver::Saver(int initialSize) : dataSize(initialSize) {
	data=new char[dataSize];
}
Saver::~Saver() {
	if (data!=NULL) {
		delete[] data;
		data=NULL;
		dataSize=0;
	}
}
void Saver::Append(const void* ptr, int size) {
	if (data+dataSize<curPtr+size) {//grow?
		const int curSize=curPtr-data;
		dataSize=curSize+size+16*1024;//current size + new size + 16k
		char* newData=new char[dataSize];
		memcpy(newData,data,curSize);
		delete[] data;
		data=newData;
		curPtr=data+curSize;
	}
	memcpy(curPtr,ptr,size);
	curPtr+=size;
}
void Saver::save(const char* fileName) {
	
}

//--------------------------------------------------------------
/*
static char p_buff[500000];
static int p_len = 00;
*/

bool mppStorageWriter::open(const char* path)
{
	//p_len = 0;
	const char* fullPath = mppStorageUtil::getFullPath(path);
	f = fopen(fullPath,"wb");
	//NSLOG1((@" mppStorageWriter::open (%s) : res=%i", path, (int)(f!=NULL) ));
	m_crc = INITIAL_CRC;	
	return (f!=NULL);

}
void mppStorageWriter::writeRaw(const void* buf, const int bufSize)
{
	if(f!=NULL) {
		fwrite(buf, 1, bufSize, f);
		/*test {{
			memcpy(p_buff+p_len, buf, bufSize);
			p_len+=bufSize;
		}}*/
		_calc_crc(buf, bufSize, m_crc);
		//NSLOG1((@"fwrite %i bytes", bufSize));		
	}
}
void mppStorageWriter::writeCrc()
{
	write(m_crc);
	const char final[] = "\nEND_OF_FILE\n";
	write(final);
}


void mppStorageWriter::close()
{
	//\\fwrite(p_buff,1,p_len,f);
	//NSLOG1((@"mppStorageWriter::close"));
	if(f!=NULL) {
		fclose(f);
	}
	f=NULL;
	//p_len=0;
	
}


//////////////////////////////

@interface mppUtilDelegateCollection : NSObject<UIAlertViewDelegate, MoreGamesExitReceiver>
-(void) moreGamesExit:(bool)result; //true: ok, false - More Games is inaccesible/connection error
-(void) alertView:(UIAlertView*)view clickedButtonAtIndex:(NSInteger)index;
@end

@implementation mppUtilDelegateCollection

-(void)alertView:(UIAlertView*)view clickedButtonAtIndex:(NSInteger)index
{
	NSLOG1((@" mppLgConfirmationDelegate::alertView... : btn=%i", index ));
	extern int mpp_internal_isNeedToLoadSavedGame;
	
	switch(index) {
		case 0: {
			NSLOG1((@" do not use saved state -- remove it", index ));
			mppCheckPointStorage::removeState();
			mpp_internal_isNeedToLoadSavedGame = -1;			
		}break;
		case 1: {
			NSLOG1((@" need to use saved state ... ", index ));
			mpp_internal_isNeedToLoadSavedGame = +1;
		}break;
	}
}

-(void) moreGamesExit:(bool)result //true: ok, false - More Games is inaccesible/connection error> 
{
	if(result == false) {
		NSString* textString = NSLocalizedString(@"mg_moregames_is_inaccesible", @"");//@"More Games is inaccesible";
		NSString* okString = NSLocalizedString(@"mg_button_ok", @"");//@"Ok";

		
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil
											message:textString
											delegate:nil
											cancelButtonTitle:okString
											otherButtonTitles:nil, nil];
		[alert show];
		[alert release];		
	}
	
	mppUtil::showCommunityButton(true);
}


@end

static mppUtilDelegateCollection* g_ut_delegate = [[mppUtilDelegateCollection alloc] init];

void mppUtil::showLoadGameConfirmation()
{	
	if(g_ut_delegate == nil) {	
		g_ut_delegate = [[mppUtilDelegateCollection alloc] init];
	}
	
	
	NSString* textString = NSLocalizedString(@"mm_load_saved_game_text", @"");// @"Do you want to load saved game?";
	NSString* noString = NSLocalizedString(@"mm_button_no", @"");//@"No";
	NSString* yesString = NSLocalizedString(@"mm_button_yes", @"");//@"Yes";
	
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil
										message:textString
										delegate:g_ut_delegate
										cancelButtonTitle:noString
										otherButtonTitles:yesString, nil];
	[alert show];
	[alert release];	
}

void mppUtil::launchMoreGames()
{
#ifndef SONIC4_TRIAL	
	[Community get].bEnableShowBtn = false;
#endif

	if(g_ut_delegate == nil) {	
		g_ut_delegate = [[mppUtilDelegateCollection alloc] init];
	}
	//MoreGamesLauncher_SetActivityIndicatorRect(228, 12, 24, 24);
	[[MoreGamesLauncher get] enableAutorotateLandscapeLeft: false LandscapeRight: true];

#ifdef _MG_IPAD	
	[[MoreGamesLauncher get] setActivityIndicatorRect:CGRectMake(360, 700, 36, 36)];
#else
	[[MoreGamesLauncher get] setActivityIndicatorRect:CGRectMake(60, 252, 24, 24)];
#endif
	[[MoreGamesLauncher get] start: g_ut_delegate];
}


void mppUtil::launchUpsellScreen(bool fromMenu_or_afterGame)
{
#ifdef SONIC4_TRIAL	
	DmSndBgmPlayerBgmStop();
	static UpsellViewController* uvc = nil;
	if(uvc==nil) {
		uvc = [[UpsellViewController alloc] initWithNibName:@"s4us_view" bundle:nil];
	}
	//
	uvc.view.bounds = CGRectMake(0, 0, UPS_SCREEN_WIDTH, UPS_SCREEN_HEIGHT);
	CGAffineTransform t = CGAffineTransformMakeRotation(M_PI/2.);
	uvc.view.transform = t;
	uvc.view.center = CGPointMake(UPS_SCREEN_HEIGHT/2, UPS_SCREEN_WIDTH/2);	
	//
	[uvc retain];
	[uvc prepare];
	[uvc setBackButtonMode: fromMenu_or_afterGame];
	[[[UIApplication sharedApplication].windows objectAtIndex:0] addSubview:uvc.view];
#endif	
}

bool mppUtil::showAchievementAlertIfNecessary()
{
#ifndef SONIC4_TRIAL
	static AchievementAlertViewController* aavc = nil;
	
	{{//combine acievements //to avoid problems with new ach. alerts after reinstall aplication
		const int achBitMaskFormGameCenter = [[Community get]getAchievementAlreadySetBitmask];//from game center
		mppAchievementSupport::get()->combineAchievementFlags(achBitMaskFormGameCenter);
	}}	
	
	const int nAch = mppAchievementSupport::get()->getAchievementAlertID(); //ret ID (0..11), or -1 if no new allerts occurs
	
	if(mppUtil::isCommunityEnabled()) {	
		if(nAch>=0) {

			if(aavc==nil) {
				aavc = [[AchievementAlertViewController alloc] initWithNibName:@"ach_dlg" bundle:nil];
			}
			//
			aavc.view.bounds = CGRectMake(0, 0, ACAL_SCREEN_WIDTH, ACAL_SCREEN_HEIGHT);
			CGAffineTransform t = CGAffineTransformMakeRotation(M_PI/2.);
			aavc.view.transform = t;
			aavc.view.center = CGPointMake(ACAL_SCREEN_HEIGHT/2, ACAL_SCREEN_WIDTH/2);	
			//
			[aavc retain];
			[aavc prepare: nAch];
			[[[UIApplication sharedApplication].windows objectAtIndex:0] addSubview:aavc.view];
			//
			return true;

		}
	}
	
#endif
	return false;
}

bool mppUtil::isAchievementAlertShown()
{
#ifndef SONIC4_TRIAL	
	extern bool isAchievementAlertShown_flag;
	return isAchievementAlertShown_flag;
#else
	return false;
#endif	
}


#ifdef SONIC4_TRIAL
  #ifdef SONIC4_TRIAL_EXIBITION
void mppUtil::showDemoSplashForTrial()
{
	static DemoSplashViewController* dsvc = nil;	

	if(dsvc==nil) {
		dsvc = [[DemoSplashViewController alloc] initWithNibName:@"DemoSplashScreen" bundle:nil];
	}
	//
	dsvc.view.bounds = CGRectMake(0, 0, UPS_SCREEN_WIDTH, UPS_SCREEN_HEIGHT);
	CGAffineTransform t = CGAffineTransformMakeRotation(M_PI/2.);
	dsvc.view.transform = t;
	dsvc.view.center = CGPointMake(UPS_SCREEN_HEIGHT/2, UPS_SCREEN_WIDTH/2);	
	//
	[dsvc retain];
	[dsvc prepare];
	[[[UIApplication sharedApplication].windows objectAtIndex:0] addSubview:dsvc.view];
}

bool mppUtil::isDemoSplashShown()
{	
	extern bool isDemoSplashShown_flag;
	return isDemoSplashShown_flag;	
}
  #endif
#endif


int mppUtil::isIPodMusicNowPlaying()
{
	/*MPMediaItem *nowPlayingMediaItem = [[MPMusicPlayerController iPodMusicPlayer] nowPlayingItem];	
	return (nowPlayingMediaItem!=nil);*/
	
	UInt32 iPodMusicIsPlaying = 0;
    UInt32 ioDataSize = sizeof( iPodMusicIsPlaying );
    AudioSessionGetProperty( kAudioSessionProperty_OtherAudioIsPlaying, &ioDataSize, &iPodMusicIsPlaying );
	
	NSLOG1((@"iPodMusicIsPlaying = %i", (int)iPodMusicIsPlaying));
	
	return (int)iPodMusicIsPlaying;

}


void mppUtil::enableIPodMusic() {
	OSStatus err;
	UInt32 sessionCategory = kAudioSessionCategory_AmbientSound;
	err = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
}

#ifndef SONIC4_TRIAL

void mppUtil::regAndDontShowBtn()
{
	//\\[[Community get] showCommunityButton];
	[[Community get] hideCommunityButton];
	[Community get].bEnableShowBtn = false;
}

bool mppUtil::isDrawMainMenu(){ 
	return [Community get].bDrawMainMenu; 
}

bool mppUtil::isCommunityEnabled()
{
	return [[Community get] isSupportedAndNotCancelledByUser];
}

bool mppUtil::isCommunityLoadedOrUnsupportedOrCancelled()
{
	return [[Community get] isLoadedOrUnsupportedOrCancelled];
}

static UIActivityIndicatorView* comm_activityIndicator = nil;

void mppUtil::startCommunityLoadingIndicator()
{
	if(!isCommunityLoadedOrUnsupportedOrCancelled()) {
		UIView* rootView = [[UIApplication sharedApplication].windows objectAtIndex:0];
		const int iSZ = 48;
		CGRect screenRect = CGRectMake((320-iSZ)/2,(480-iSZ)/2, iSZ, iSZ);
		comm_activityIndicator = [[UIActivityIndicatorView alloc] initWithFrame:screenRect];
		comm_activityIndicator.activityIndicatorViewStyle = UIActivityIndicatorViewStyleGray;
		[rootView addSubview:comm_activityIndicator];	
		comm_activityIndicator.hidden = false;
		[comm_activityIndicator startAnimating];		
	}
}

void mppUtil::stopCommunityLoadingIndicator()
{
	if(comm_activityIndicator!=nil) {
		[comm_activityIndicator removeFromSuperview];
		[comm_activityIndicator release];
		comm_activityIndicator = nil;
	}
}

const char* mppUtil::getFilenameForGKLocalPlayerID(const char* file_name_mask) //file_name_mask must contain %08x (for example "myfile_%08x.dat")
{
	NSString* playerID = [GKLocalPlayer localPlayer].playerID;
	int crc = -1;
	if(playerID!=nil && [playerID length]>0) {
		crc = 0x1ABCDE;
		for(int i=[playerID length]; --i>=0;) {
			crc = (crc>>1) + (crc<<2) + [playerID characterAtIndex:i];
		}
		crc = (crc&0x1fFFffFF);
	}
	static char _buf[512];
	sprintf(_buf, file_name_mask, crc);
	return _buf;
}

#endif

void mppUtil::showCommunityButton(bool bShow)
{
#ifndef SONIC4_TRIAL
	if(bShow) 
		[[Community get] showCommunityButton];
	else
		[[Community get] hideCommunityButton];
#endif
}

void mppUtil::hideCommunityButtonWithAnimation()
{
#ifndef SONIC4_TRIAL
	[[Community get] hideCommunityButtonWithAnimation];
#endif
}

bool mppUtil::isCommunityButtonPressed()
{
#ifndef SONIC4_TRIAL
	return [[Community get] isBtnSelected];
#else
	return false;
#endif
}

void mppUtil::sendAchievement(int type, float percent)
{
#ifndef SONIC4_TRIAL
	NSString* strAchId = nil;
	
#ifndef ASIA_APPSTORE	
	switch(type)
	{
		default: break;
		case Sonic4_StoryBegins: strAchId = @"Sonic4_StoryBegins"; break;
		case Sonic4_Eggman: strAchId = @"Sonic4_Eggman"; break;
		case Sonic4_Chaos1: strAchId = @"Sonic4_Chaos1"; break;
		case Sonic4_Enemy: strAchId = @"Sonic4_Enemy"; break;
		case Sonic4_Golden: strAchId = @"Sonic4_Golden"; break;
		case Sonic4_Cleared: strAchId = @"Sonic4_Cleared"; break;
		case Sonic4_Contender: strAchId = @"Sonic4_Contender"; break;
		case Sonic4_Collector: strAchId = @"Sonic4_Collector"; break;
		case Sonic4_Immortal: strAchId = @"Sonic4_Immortal"; break;
		case Sonic4_Super: strAchId = @"Sonic4_Super"; break;
		case Sonic4_Speed: strAchId = @"Sonic4_Speed"; break;
		case Sonic4_Untouchable: strAchId = @"Sonic4_Untouchable"; break;
	}
#else
	switch(type)
	{
		default: break;
/*??
		case Sonic4_StoryBegins: strAchId = @"Sonic4_StoryBegins_ASIA"; break;
		case Sonic4_Eggman: strAchId = @"Sonic4_Eggman_ASIA"; break;
		case Sonic4_Chaos1: strAchId = @"Sonic4_Chaos1_ASIA"; break;
		case Sonic4_Enemy: strAchId = @"Sonic4_Enemy_ASIA"; break;
		case Sonic4_Golden: strAchId = @"Sonic4_Golden_ASIA"; break;
		case Sonic4_Cleared: strAchId = @"Sonic4_Cleared_ASIA"; break;
		case Sonic4_Contender: strAchId = @"Sonic4_Contender_ASIA"; break;
		case Sonic4_Collector: strAchId = @"Sonic4_Collector_ASIA"; break;
		case Sonic4_Immortal: strAchId = @"Sonic4_Immortal_ASIA"; break;
		case Sonic4_Super: strAchId = @"Sonic4_Super_ASIA"; break;
		case Sonic4_Speed: strAchId = @"Sonic4_Speed_ASIA"; break;
		case Sonic4_Untouchable: strAchId = @"Sonic4_Untouchable_ASIA"; break;
*/
		 case Sonic4_StoryBegins: strAchId = @"The_Story_Begins_ASIA"; break;
		 case Sonic4_Eggman: strAchId = @"Crush_Dr._Eggman_ASIA"; break;
		 case Sonic4_Chaos1: strAchId = @"The_First_Chaos_Emerald_ASIA"; break;
		 case Sonic4_Enemy: strAchId = @"Enemy_Hunter_ASIA"; break;
		 case Sonic4_Golden: strAchId = @"Enemy_Hunter_ASIA"; break;
		 case Sonic4_Cleared: strAchId = @"All_Stages_Cleared_ASIA"; break;
		 case Sonic4_Contender: strAchId = @"Contender_ASIA"; break;
		 case Sonic4_Collector: strAchId = @"Ring_Collector_ASIA"; break;
		 case Sonic4_Immortal: strAchId = @"Immortal_ASIA"; break;
		 case Sonic4_Super: strAchId = @"Super_Sonic_Genesis_ASIA"; break;
		 case Sonic4_Speed: strAchId = @"Speeds_My_Game_ASIA"; break;
		 case Sonic4_Untouchable: strAchId = @"Untouchable_ASIA"; break;

	}
#endif
	if(strAchId)
	{
		//NSLog(@"%@ %f", strAchId, percent);//sss //test
		[[Community get] submitAchievement:strAchId percentComplete:percent];
	}
#endif
}


enum ENUM_OF_STAGES{ //from gsMainSys.h
	GSD_MAIN_STAGE_ID_1_1,			//!< ZONE1-1
	GSD_MAIN_STAGE_ID_1_2,			//!< ZONE1-2
	GSD_MAIN_STAGE_ID_1_3,			//!< ZONE1-3
	GSD_MAIN_STAGE_ID_1_BOSS,		//!< ZONE1-BOSS
	GSD_MAIN_STAGE_ID_2_1,			//!< ZONE2-1
	GSD_MAIN_STAGE_ID_2_2,			//!< ZONE2-2
	GSD_MAIN_STAGE_ID_2_3,			//!< ZONE2-3
	GSD_MAIN_STAGE_ID_2_BOSS,		//!< ZONE2-BOSS
	GSD_MAIN_STAGE_ID_3_1,			//!< ZONE3-1
	GSD_MAIN_STAGE_ID_3_2,			//!< ZONE3-2
	GSD_MAIN_STAGE_ID_3_3,			//!< ZONE3-3
	GSD_MAIN_STAGE_ID_3_BOSS,		//!< ZONE3-BOSS
	GSD_MAIN_STAGE_ID_4_1,			//!< ZONE4-1
	GSD_MAIN_STAGE_ID_4_2,			//!< ZONE4-2
	GSD_MAIN_STAGE_ID_4_3,			//!< ZONE4-3
	GSD_MAIN_STAGE_ID_4_BOSS,		//!< ZONE4-BOSS
	GSD_MAIN_STAGE_ID_FINAL_1,		//!< ZONEFinal-1  final stage
	GSD_MAIN_STAGE_ID_FINAL_2,		//!< ZONEFinal-2  (//sss - unused on iPhone)
	GSD_MAIN_STAGE_ID_FINAL_3,		//!< ZONEFinal-3  (//sss - unused on iPhone)
	GSD_MAIN_STAGE_ID_FINAL_4,		//!< ZONEFinal-4  (//sss - unused on iPhone)
	GSD_MAIN_STAGE_ID_FINAL_5,		//!< ZONEFinal-5  (//sss - unused on iPhone)
	
	GSD_MAIN_STAGE_ID_SS1,			//!< SpeclalStage1
	GSD_MAIN_STAGE_ID_SS2,			//!< SpeclalStage2
	GSD_MAIN_STAGE_ID_SS3,			//!< SpeclalStage3
	GSD_MAIN_STAGE_ID_SS4,			//!< SpeclalStage4
	GSD_MAIN_STAGE_ID_SS5,			//!< SpeclalStage5
	GSD_MAIN_STAGE_ID_SS6,			//!< SpeclalStage6
	GSD_MAIN_STAGE_ID_SS7,			//!< SpeclalStage7
	
	
	GSD_MAIN_STAGE_ID_ENDING,		//!< エンディングステージ
	
	GSD_MAIN_STAGE_ID_MAX
	
};

const char* mppUtil::getLBUkey(int stage_id)
{
	
#ifndef ASIA_APPSTORE
	switch(stage_id)
	{
		case GSD_MAIN_STAGE_ID_1_1:
			return "Splash_Hill_Act_1_TIME";
		case GSD_MAIN_STAGE_ID_1_2:
			return "Splash_Hill_Act_2_TIME";
		case GSD_MAIN_STAGE_ID_1_3:
			return "Splash_Hill_Act_3_TIME";
		case GSD_MAIN_STAGE_ID_1_BOSS:
			return "Splash_Hill_BOSS_TIME";
		case GSD_MAIN_STAGE_ID_2_1:
			return "Casino_Street_Act_1_TIME";
		case GSD_MAIN_STAGE_ID_2_2:
			return "Casino_Street_Act_2_TIME";
		case GSD_MAIN_STAGE_ID_2_3:
			return "Casino_Street_Act_3_TIME";
		case GSD_MAIN_STAGE_ID_2_BOSS:
			return "Casino_Street_BOSS_TIME";
		case GSD_MAIN_STAGE_ID_3_1:
			return "Lost_Labyrinth_Act_1_TIME";
		case GSD_MAIN_STAGE_ID_3_2:
			return "Lost_Labyrinth_Act_2_TIME";
		case GSD_MAIN_STAGE_ID_3_3:
			return "Lost_Labyrinth_Act_3_TIME";
		case GSD_MAIN_STAGE_ID_3_BOSS:
			return "Lost_Labyrinth_BOSS_TIME";
		case GSD_MAIN_STAGE_ID_4_1:
			return "Mad_Gear_Act_1_TIME";
		case GSD_MAIN_STAGE_ID_4_2:
			return "Mad_Gear_Act_2_TIME";
		case GSD_MAIN_STAGE_ID_4_3:
			return "Mad_Gear_Act_3_TIME";
		case GSD_MAIN_STAGE_ID_4_BOSS:
			return "Mad_Gear_BOSS_TIME";
		case GSD_MAIN_STAGE_ID_FINAL_1:
			return "EGG_Station_TIME";
		case GSD_MAIN_STAGE_ID_SS1:
			return "Special_Stage_1_TIME";
		case GSD_MAIN_STAGE_ID_SS2:
			return "Special_Stage_2_TIME";
		case GSD_MAIN_STAGE_ID_SS3:
			return "Special_Stage_3_TIME";
		case GSD_MAIN_STAGE_ID_SS4:
			return "Special_Stage_4_TIME";
		case GSD_MAIN_STAGE_ID_SS5:
			return "Special_Stage_5_TIME";
		case GSD_MAIN_STAGE_ID_SS6:
			return "Special_Stage_6_TIME";
		case GSD_MAIN_STAGE_ID_SS7:
			return "Special_Stage_7_TIME";
	}
#else //ASIA 
	switch(stage_id)
	{
		case GSD_MAIN_STAGE_ID_1_1:
			return "Splash_Hill_Act_1_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_1_2:
			return "Splash_Hill_Act_2_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_1_3:
			return "Splash_Hill_Act_3_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_1_BOSS:
			return "Splash_Hill_BOSS_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_2_1:
			return "Casino_Street_Act_1_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_2_2:
			return "Casino_Street_Act_2_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_2_3:
			return "Casino_Street_Act_3_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_2_BOSS:
			return "Casino_Street_BOSS_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_3_1:
			return "Lost_Labyrinth_Act_1_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_3_2:
			return "Lost_Labyrinth_Act_2_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_3_3:
			return "Lost_Labyrinth_Act_3_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_3_BOSS:
			return "Lost_Labyrinth_BOSS_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_4_1:
			return "Mad_Gear_Act_1_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_4_2:
			return "Mad_Gear_Act_2_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_4_3:
			return "Mad_Gear_Act_3_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_4_BOSS:
			return "Mad_Gear_BOSS_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_FINAL_1:
			return "E.G.G._Station_TIME_ASIA";//"EGG_Station_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_SS1:
			return "Special_Stage_1_TIME_TAKEN_ASIA";//"Special_Stage_1_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_SS2:
			return "Special_Stage_2_TIME_TAKEN_ASIA";//"Special_Stage_2_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_SS3:
			return "Special_Stage_3_TIME_TAKEN_ASIA";//"Special_Stage_3_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_SS4:
			return "Special_Stage_4_TIME_TAKEN_ASIA";//"Special_Stage_4_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_SS5:
			return "Special_Stage_5_TIME_TAKEN_ASIA";//"Special_Stage_5_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_SS6:
			return "Special_Stage_6_TIME_TAKEN_ASIA";//"Special_Stage_6_TIME_ASIA";
		case GSD_MAIN_STAGE_ID_SS7:
			return "Special_Stage_7_TIME_TAKEN_ASIA";//"Special_Stage_7_TIME_ASIA";
	}	
#endif
	return NULL;
}

void mppUtil::sendScore(int game_type, int stage_id, unsigned int score, int min, int sec, int msec) //msec - as 1/100 of sec
{
#ifndef SONIC4_TRIAL
	if(game_type == 1/*GSD_GAME_MODE_TIME_ATTACK*/)
	{

		NSString* ukey = [NSString stringWithCString:getLBUkey(stage_id) encoding:NSASCIIStringEncoding];
		float value = ((min*60.0+sec)*100.0+msec);
		if(ukey) {
			mppTimeScores::timeScores.Set(stage_id, value);
			[[Community get] submitScore:value forCategory:ukey];
			mppTimeScores::timeScores.Save();
		}
		
		sendTotalTimeAttackTime();	
	}
#endif
}

void mppUtil::sendTotalTimeAttackTime()
{
#ifndef SONIC4_TRIAL
	//send total time
	int minT=0, secT=0, sec100T=0;
	if( mpp_calcTotalTimeAttackTime(minT, secT, sec100T) ) {
		float value = ((minT*60.0+secT)*100.0+ sec100T);
#ifndef ASIA_APPSTORE		
		NSString* ukey = @"TOTAL_TIME";
#else
		NSString* ukey = @"TOTAL_TIME_ASIA";
#endif
		[[Community get] submitScore:value forCategory:ukey];
	}
#endif
	
}

static	bool isSpeedAchievementAfterTIMEOVERDisabled__ = false;	

void mppUtil::disableSpeedAchievementAfterTIMEOVER(bool trueForDisable) {
	isSpeedAchievementAfterTIMEOVERDisabled__=trueForDisable;
}
bool mppUtil::isSpeedAchievementAfterTIMEOVERDisabled() {
	return isSpeedAchievementAfterTIMEOVERDisabled__;
}




