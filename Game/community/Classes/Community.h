
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <GameKit/GameKit.h>
#import "CommunityMainMenu.h"

@interface Community : NSObject
{
@public	
	UIButton* scBtn;
	CommunityMainMenu* communityMainMenu;

	NSMutableDictionary* earnedAchievementCache;
	bool bSupported;
	bool bLoginCancelledByUser;//sss
	bool bEnableShowBtn;
	bool bDrawMainMenu;
	bool bAuthenticateStarted;
	int bitmaskAchievementIniArray;//sss - achievement states before game start
}

@property (nonatomic, retain) UIButton* scBtn;
@property (nonatomic, retain) CommunityMainMenu* communityMainMenu;
@property (retain) NSMutableDictionary* earnedAchievementCache;
@property (nonatomic, assign) bool bSupported;
@property (nonatomic, assign) bool bLoginCancelledByUser;
@property (nonatomic, assign) bool bEnableShowBtn;
@property (nonatomic, assign) bool bDrawMainMenu;
@property (nonatomic, assign) bool bAuthenticateStarted;
@property (nonatomic, assign) int bitmaskAchievementIniArray;

+ (Community*) get;
+ (void) kill;
- (id) init;

- (bool) isSupportedAndNotCancelledByUser;
- (bool) isLoadedOrUnsupportedOrCancelled;

- (void) initCommunity;
- (bool) isGameCenterAvailable;
- (void) authenticateLocalPlayer;

- (void) createCommunityButton;
- (void) showCommunityButton;
- (void) hideCommunityButton;
- (void) hideCommunityButtonWithAnimation;

- (bool) isBtnSelected;

- (IBAction)startCommunity:(id)sender;

- (void) achievementSubmitted: (GKAchievement*) ach error:(NSError*) error;
- (void) submitAchievement: (NSString*) identifier percentComplete: (double) percentComplete;
- (void) resetAchievements;
- (void) achievementResetResult: (NSError*) error;

- (void) submitScore: (int64_t) score forCategory: (NSString*) category;
- (void) scoreSubmitted: (NSError*) error;
- (void) submitScore2: (int64_t) score forCategory: (NSString*) category Data:(int*)pData;
- (void) scoreSubmitted2: (NSError*) error Data:(int*)pData;
- (void) loadHighScore;
- (void) loadScoreComplete:(GKLeaderboard*)leaderBoard error:(NSError*) error;

- (void) showAlertWithTitle: (NSString*) title message: (NSString*) message;

- (void) mapPlayerIDtoPlayer: (NSString*) playerID;
- (void) mappedPlayerIDToPlayer: (GKPlayer*) player error: (NSError*) error;

+ (int) getAchievementStringIdentifier:(NSString*) strAchId;
- (void) preloadAchievements;
- (int) getAchievementAlreadySetBitmask;

@end

#if	defined(__cplusplus)
extern "C" {
#endif
	extern void DmSoundPlaySE(char *cue_name);
#if	defined(__cplusplus)
};
#endif
