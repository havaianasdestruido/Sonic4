//#include "dmSound.h"


#import "Community.h"


static Community* communityInst = nil;

@implementation Community

@synthesize scBtn;
@synthesize communityMainMenu;
@synthesize earnedAchievementCache;
@synthesize bSupported;
@synthesize bLoginCancelledByUser;
@synthesize bEnableShowBtn;
@synthesize bDrawMainMenu;
@synthesize bAuthenticateStarted;
@synthesize bitmaskAchievementIniArray;

+(Community*) get
{
	if(communityInst == nil) 
	{
		communityInst = [[Community alloc] init];
	}
	return communityInst;
}

+(void)kill
{
	if(communityInst)
	{
		[communityInst release];
		communityInst = nil;
	}
}

-(id)init
{
	self.bSupported = false;
	self.bLoginCancelledByUser = false;
	self.scBtn = nil;
	self.earnedAchievementCache= nil;
	self.bEnableShowBtn = false;
	self.bDrawMainMenu = true;
	self.bAuthenticateStarted = false;
	return self;
}

- (void)dealloc
{
	self.bSupported = false;
	self.bLoginCancelledByUser = false;
	[self.scBtn removeFromSuperview];
	self.scBtn = nil;
	self.earnedAchievementCache= nil;
	if(self.communityMainMenu)
	{
		[self.communityMainMenu.view removeFromSuperview];
		self.communityMainMenu = nil;
	}
	[super dealloc];
}

- (bool) isGameCenterAvailable
{
    // Check for presence of GKLocalPlayer API.
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
	
    // The device must be running running iOS 4.1 or later.
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    bool osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
	self.bSupported = (gcClass && osVersionSupported);
	
	if(self.bSupported == true) {
		//patch for iPhone 3G //sss
		NSString* machine = [[UIDevice currentDevice] machine]; //called from "MoreGamesDownloader.m"
		if(NSOrderedSame == [machine caseInsensitiveCompare: @"iPhone1,2"]) {//iPhone3G
			self.bSupported = false;
		}			
	}
	
    return self.bSupported;
}

- (void) initCommunity
{
	if ([self isGameCenterAvailable]==false) return;
	[self authenticateLocalPlayer];
}

- (void)showCommunityButton
{
	self.bEnableShowBtn = true;
	if(self.scBtn==nil)
	{
		[self initCommunity];
	}
	if(self.scBtn!=nil) {
		self.scBtn.hidden = false;
		[self.scBtn setAlpha: 1.0f];
	}
}

- (void)hideCommunityButton
{
	self.bEnableShowBtn = false;
	if(self.scBtn==nil)
	{
		[self initCommunity];
	}
	if(self.scBtn!=nil) {
		self.scBtn.hidden = true;
	}
}

#define COMMY_BUTTON_TRANSITION_DURATION (1.0)

- (void)hideCommunityButtonWithAnimation //sss
{
	self.bEnableShowBtn = false;
	if(self.scBtn==nil)
	{
		[self initCommunity];
	}
	//self.scBtn.hidden = true;
	
	[UIView setAnimationsEnabled:false];
	//self.scBtn.hidden = false;
	[self.scBtn setAlpha:1.0f];
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationsEnabled:true];
	[UIView setAnimationDuration:COMMY_BUTTON_TRANSITION_DURATION];
	[self.scBtn setAlpha:0.f];
	//self.scBtn.hidden = true;
	// Commit animation
	[UIView commitAnimations];	
}

- (bool) isBtnSelected
{
	if(self.scBtn)
	{
		return (self.scBtn.selected || self.scBtn.highlighted);
	}
	else return false;
}

- (void) createCommunityButton
{
#ifdef IPAD_VERSION
	CGRect rect = CGRectMake(0, 0, 392, 88);
#else
	CGRect rect = CGRectMake(0, 0, 170, 37);
#endif
	self.scBtn = [[UIButton alloc] init];
	self.scBtn.frame = rect;
	//self.scBtn.titleLabel.text = @"Community";
    UIImage* img = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"sc_btn" ofType:@"png"]];
    UIImage* img_h = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"sc_btn_hit" ofType:@"png"]];
	[self.scBtn setBackgroundImage:img forState:UIControlStateNormal];
	[self.scBtn setBackgroundImage:img_h forState:UIControlStateHighlighted];
	[self.scBtn setBackgroundImage:img_h forState:UIControlStateSelected];
	[self.scBtn addTarget:self action:@selector(startCommunity:) forControlEvents:UIControlEventTouchUpInside];
	
	CGAffineTransform t = CGAffineTransformMakeRotation(M_PI/2.);
	self.scBtn.center = CGPointMake(0,0);
	self.scBtn.transform = t;
	
	[[[UIApplication sharedApplication].windows objectAtIndex:0] addSubview:scBtn];
	//[[hogAppDelegate getGLView] addSubview:scBtn];
	CGRect r = self.scBtn.frame;
#ifdef IPAD_VERSION
	r.origin.x += 92/2+3;
	r.origin.y += 560; 
#else
	r.origin.x += 37/2+3;
	r.origin.y += 260; 
#endif
	self.scBtn.frame = r;
	if(self.bEnableShowBtn) self.scBtn.hidden = false;
	else self.scBtn.hidden = true;
}

#ifdef _MG_IPAD
 #define CMM_SCREEN_WIDTH (1024)
 #define CMM_SCREEN_HEIGHT (768)
#else
 #define CMM_SCREEN_WIDTH (480)
 #define CMM_SCREEN_HEIGHT (320)
#endif

		

- (IBAction)startCommunity:(id)sender
{
	DmSoundPlaySE("Ok");
	if([GKLocalPlayer localPlayer].authenticated == YES)
	{
		if (self.communityMainMenu) {
			self.bDrawMainMenu = false;
			self.scBtn.hidden = true;
			
			///////////// fix orientation //sss
			self.communityMainMenu.view.bounds = CGRectMake(0, 0, CMM_SCREEN_WIDTH, CMM_SCREEN_HEIGHT);
			CGAffineTransform t = CGAffineTransformMakeRotation(M_PI/2.);
			self.communityMainMenu.view.transform = t;
			self.communityMainMenu.view.center = CGPointMake(CMM_SCREEN_HEIGHT/2, CMM_SCREEN_WIDTH/2);						
			
			[[[UIApplication sharedApplication].windows objectAtIndex:0] addSubview:self.communityMainMenu.view];
		}
	}
}

static int mpp_lastErrorCode = -1;

- (void) authenticateLocalPlayer
{
	if([self isSupportedAndNotCancelledByUser]==false)
		return;
	NSLog(@"BA:GK_LP_ID=%@", [GKLocalPlayer localPlayer].playerID);	
	if(self.bAuthenticateStarted==false && [GKLocalPlayer localPlayer].authenticated == NO)
	{
		self.bAuthenticateStarted = true;
		[[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error) {
			if(self.bAuthenticateStarted)
			{
			if (error == nil)
			{
				NSLog(@"GameCenter: success authentication!!!");
				NSLog(@"SS:GK_LP_ID=%@", [GKLocalPlayer localPlayer].playerID);	
				[self createCommunityButton];
#ifdef IPAD_VERSION
				self.communityMainMenu = [[CommunityMainMenu alloc] initWithNibName:@"CommunityMainMenu_iPad" bundle:nil];
#else
				self.communityMainMenu = [[CommunityMainMenu alloc] initWithNibName:@"CommunityMainMenu" bundle:nil];
#endif
				//[self submitScore:0 forCategory:@"global_score"];
				//[self resetAchievements];
				self.bAuthenticateStarted = false;
				//
				[self preloadAchievements];
			}
			else
			{
				mpp_lastErrorCode = [error code];
				
				self.bAuthenticateStarted = false;
				NSLog(@"GameCenter: error authentication!!!");
				NSLog(@"ER:GK_LP_ID=%@", [GKLocalPlayer localPlayer].playerID);	
				UIAlertView* alert= [[[UIAlertView alloc] initWithTitle: @"Game Center Account Required" 
																message: [error localizedDescription]
															   delegate: self cancelButtonTitle: @"Ok" otherButtonTitles: NULL] autorelease];
				[alert addButtonWithTitle:@"Try Again..."];
				[alert show];
			}
			}
		}];
	}
}

#define CONN_RETRY_INTERVAL (15)
#define ERR___INTERNET_IS_ABSENT (-1009)

-(void) stopReconnectionTry
{
	NSLog(@"Retry Connection Time is out");
	if([self isLoadedOrUnsupportedOrCancelled] == false) {
		if(mpp_lastErrorCode == ERR___INTERNET_IS_ABSENT)
			self.bLoginCancelledByUser = true;
	}
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if(buttonIndex) {
		[self authenticateLocalPlayer];
		[NSTimer scheduledTimerWithTimeInterval:CONN_RETRY_INTERVAL target:self selector:@selector(stopReconnectionTry) userInfo:nil repeats:NO];
	}
	else {
		NSLog(@"Login cancelled by user");
		self.bLoginCancelledByUser = true;
	}
}

- (void) submitAchievement: (NSString*) identifier percentComplete: (double) percentComplete
{
	if([self isSupportedAndNotCancelledByUser]==false) 
		return;
	if([GKLocalPlayer localPlayer].authenticated == NO) 
		return;
	if(percentComplete<=0.005) 
		return;
	if(self.earnedAchievementCache == NULL)
	{
		[GKAchievement loadAchievementsWithCompletionHandler: ^(NSArray *scores, NSError *error)
		 {
			 if(error == NULL)
			 {
				 NSMutableDictionary* tempCache= [NSMutableDictionary dictionaryWithCapacity: [scores count]];
				 for(int i=0,e=[scores count];i<e;++i)
				 {
					 GKAchievement* ach = [scores objectAtIndex:i];
					 [tempCache setObject: ach forKey: ach.identifier];
				 }
				 self.earnedAchievementCache= tempCache;
				 [self submitAchievement: identifier percentComplete: percentComplete];
			 }
			 else
			 {
				 [self achievementSubmitted:NULL error:error];
			 }
			 
		 }];
	}
	else
	{
		GKAchievement* achievement= [self.earnedAchievementCache objectForKey: identifier];
		if(achievement != NULL)
		{
			if((achievement.percentComplete >= 100.0) || (achievement.percentComplete >= percentComplete))
			{
				achievement= NULL;
			}
			else achievement.percentComplete= percentComplete;
		}
		else
		{
			achievement= [[[GKAchievement alloc] initWithIdentifier: identifier] autorelease];
			achievement.percentComplete= percentComplete;
			[self.earnedAchievementCache setObject: achievement forKey: achievement.identifier];
		}
		if(achievement!= NULL)
		{
			[achievement reportAchievementWithCompletionHandler: ^(NSError *error)
			 {
				 [self achievementSubmitted:achievement error:error];
			 }];
		}
	}
}

- (void) achievementSubmitted: (GKAchievement*) ach error:(NSError*) error
{
	if((error == NULL) && (ach != NULL))
	{
		/*
		if(ach.percentComplete >= 100.0)
		{
			[GKAchievementDescription loadAchievementDescriptionsWithCompletionHandler: ^(NSArray *descriptions, NSError *error)
			 {
				 if (error == nil)
				 {
					 if (descriptions != nil)
					 {
						 for(int i=0,e=[descriptions count];i<e;++i)
						 {
							 GKAchievementDescription* desc = [descriptions objectAtIndex:i];
							 if ([ach.identifier compare:desc.identifier]==NSOrderedSame) {
								 [self showAlertWithTitle: @"" message: [NSString stringWithFormat: NSLocalizedString(@"ach_alert_txt", nil), desc.title]];
								 break;
							 }
						 }
					 }
				 }
			 }];
		}*/
	}
	else if(!(error == NULL && ach != NULL))
	{
		/*sss - annoying
		[self showAlertWithTitle: @"Achievement Submission Failed!"
						 message: [error localizedDescription]];
		 */
	}
}

- (void) submitScore: (int64_t) score forCategory: (NSString*) category 
{
	if([self isSupportedAndNotCancelledByUser]==false) 
		return;
	if([GKLocalPlayer localPlayer].authenticated == NO) 
		return;
	GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:category] autorelease];	
	scoreReporter.value = score;
	[scoreReporter reportScoreWithCompletionHandler: ^(NSError *error) 
	 {
		 [self scoreSubmitted:error];
	 }];
}

- (void) scoreSubmitted: (NSError*) error
{
	/*if(error == NULL)
	{
		[self showAlertWithTitle: @"High Score Reported!"
						 message: [NSString stringWithFormat: @"", [error localizedDescription]]];
	}
	else*/
	if(error)
	{
		[self showAlertWithTitle: @"Score Report Failed"
						 message: [error localizedDescription]];
	}
}
//////////////////////
- (void) submitScore2: (int64_t) score forCategory: (NSString*) category Data:(int*)pData
{
	if([self isSupportedAndNotCancelledByUser]==false) 
		return;
	if([GKLocalPlayer localPlayer].authenticated == NO) 
		return;
	GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:category] autorelease];	
	scoreReporter.value = score;
	[scoreReporter reportScoreWithCompletionHandler: ^(NSError *error) 
	 {
		 [self scoreSubmitted2:error Data:pData];
	 }];
}

- (void) scoreSubmitted2: (NSError*) error Data:(int*)pData
{
	if(error == NULL)
	{
		*pData = 60000;
	}
}
//////////////////////

- (void) loadHighScore
{
	GKLeaderboard* leaderBoard= [[[GKLeaderboard alloc] init] autorelease];
	leaderBoard.category= @"global_score";
	leaderBoard.timeScope= GKLeaderboardTimeScopeAllTime;
	leaderBoard.range= NSMakeRange(1, 1);
	
	[leaderBoard loadScoresWithCompletionHandler:^(NSArray *scores, NSError *error)
	 {
		 [self loadScoreComplete:leaderBoard error:error];
	 }];
}

- (void) loadScoreComplete:(GKLeaderboard*)leaderBoard error:(NSError*) error
{
	/*if(error == NULL)
	{
		int64_t personalBest= leaderBoard.localPlayerScore.value;
		//self.personalBestScoreDescription= @"Your Best:";
		//self.personalBestScoreString= [NSString stringWithFormat: @"%ld", personalBest];
		if([leaderBoard.scores count] >0)
		{
			//self.leaderboardHighScoreDescription=  @"-";
			//self.leaderboardHighScoreString=  @"";
			GKScore* allTime= [leaderBoard.scores objectAtIndex: 0];
			//self.cachedHighestScore= allTime.formattedValue;
			//[self mapPlayerIDtoPlayer: allTime.playerID];
		}
	}
	else
	{
		//self.personalBestScoreDescription= @"GameCenter Scores Unavailable";
		//self.personalBestScoreString=  @"-";
		//self.leaderboardHighScoreDescription= @"GameCenter Scores Unavailable";
		//self.leaderboardHighScoreDescription=  @"-";
		[self showAlertWithTitle: @"Score Reload Failed!"
						 message: [NSString stringWithFormat: @"Reason: %@", [error localizedDescription]]];
	}*/
}

- (void) mapPlayerIDtoPlayer: (NSString*) playerID
{
	[GKPlayer loadPlayersForIdentifiers: [NSArray arrayWithObject: playerID] withCompletionHandler:^(NSArray *playerArray, NSError *error)
	 {
		 GKPlayer* player= NULL;
		 for (GKPlayer* tempPlayer in playerArray)
		 {
			 if([tempPlayer.playerID isEqualToString: playerID])
			 {
				 player= tempPlayer;
				 break;
			 }
		 }
		 [self mappedPlayerIDToPlayer:player error:error];
	 }];
	
}

- (void) mappedPlayerIDToPlayer: (GKPlayer*) player error: (NSError*) error
{
	/*if((error == NULL) && (player != NULL))
	{
		self.leaderboardHighScoreDescription= [NSString stringWithFormat: @"%@ got:", player.alias];
		
		if(self.cachedHighestScore != NULL)
		{
			self.leaderboardHighScoreString= self.cachedHighestScore;
		}
		else
		{
			self.leaderboardHighScoreString= @"-";
		}
		
	}
	else
	{
		self.leaderboardHighScoreDescription= @"GameCenter Scores Unavailable";
		self.leaderboardHighScoreDescription=  @"-";
	}*/
}

- (void) showAlertWithTitle: (NSString*) title message: (NSString*) message
{
	UIAlertView* alert= [[[UIAlertView alloc] initWithTitle: title message: message 
												   delegate: NULL cancelButtonTitle: @"OK" otherButtonTitles: NULL] autorelease];
	[alert show];
}

- (void) resetAchievements
{
	self.earnedAchievementCache= NULL;
	[GKAchievement resetAchievementsWithCompletionHandler: ^(NSError *error) 
	 {
		 //[self achievementResetResult:error];
	 }];
}

- (void) achievementResetResult: (NSError*) error
{
	//self.currentScore= 0;
	if(error != NULL)
	{
		[self showAlertWithTitle: @"Achievement Reset Failed!"
						 message: [error localizedDescription]];
	}
}

/*
- (bool) isSupported
{
	return self.bSupported;
}*/

- (bool) isSupportedAndNotCancelledByUser
{
	return (self.bSupported) && (!self.bLoginCancelledByUser);
}

- (bool) isLoadedOrUnsupportedOrCancelled
{
	if([self isSupportedAndNotCancelledByUser]==false)
		return true;
	if(self.scBtn!=nil && self.communityMainMenu!=nil)
		return true;
	
	return false;
}

+(int)getAchievementStringIdentifier:(NSString*) strAchId
{
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_StoryBegins"])	return 0;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Eggman"]) return 1;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Chaos1"]) return 2;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Enemy"]) return 3;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Golden"]) return 4;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Cleared"]) return 5;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Contender"]) return 6;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Collector"]) return 7;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Immortal"]) return 8;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Super"]) return 9;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Speed"]) return 10;
	if(NSOrderedSame == [strAchId  compare:@"Sonic4_Untouchable"]) return 11;
	return -1;
}

- (void) preloadAchievements //return: bit flags (0/1-ach off/on)
{//to avoid problems with new ach. alerts after reinstall aplication
	//if(self.earnedAchievementCache == NULL)
	{
		[GKAchievement loadAchievementsWithCompletionHandler: ^(NSArray *scores, NSError *error)
		 {
			 if(error == NULL)
			 {
				 NSMutableDictionary* tempCache= [NSMutableDictionary dictionaryWithCapacity: [scores count]];
				 for(int i=0,e=[scores count];i<e;++i)
				 {
					 GKAchievement* ach = [scores objectAtIndex:i];
					 [tempCache setObject: ach forKey: ach.identifier];
					 
					 if(ach.percentComplete >= 100.0)
					 {{
						 int achID = [Community getAchievementStringIdentifier: ach.identifier];
						 if(achID>=0) {
							 self.bitmaskAchievementIniArray |= (1<<achID);
						 }
					 }}
					 
				 }
				 self.earnedAchievementCache = tempCache;
			 }
			 else {
				 //failed to preload achievements
			 }			 
		 }];
	}	
}

-(int) getAchievementAlreadySetBitmask
{
	return (bitmaskAchievementIniArray);
}



@end


		
