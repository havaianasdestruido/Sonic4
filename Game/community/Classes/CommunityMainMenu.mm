//
//  CommunityMainMenu.m
//  hog
//
//  Created by Nikolai Lisun on 9/29/10.
//  Copyright 2010 MPP. All rights reserved.
//

//#include "dmSound.h"

#import "Community.h"
#import "CommunityMainMenu.h"
#include "dmSndBgmPlayer.h"


#include "mppTimeScores.h"



@implementation CommunityMainMenu

/*
-(void)fixOrient {
	int MG_SCREEN_WIDTH = 480, MG_SCREEN_HEIGHT = 320;
	CGRect rect  = CGRectMake(0, 0, MG_SCREEN_WIDTH, MG_SCREEN_HEIGHT);
	self.view = [[UIView alloc] initWithFrame:rect];
	self.view.bounds = rect;
	CGAffineTransform t = CGAffineTransformMakeRotation(M_PI/2.);
	self.view.transform = t;
	self.view.center = CGPointMake(MG_SCREEN_HEIGHT/2, MG_SCREEN_WIDTH/2);
	
}*/

 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	//[self fixOrient];
	
	
	//test{mppTimeScores::timeScores.Set(4, 10000);	mppTimeScores::timeScores.Set(2, 20000);mppTimeScores::timeScores.Send();}}
	if(mppTimeScores::timeScores.isNeedToForceSendTimesToLeaderboard()) 
	{
		mppTimeScores::timeScores.Send();
	}
}

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}



- (IBAction)back:(id)sender
{
	DmSoundPlaySE("Ok");
	[Community get].bDrawMainMenu = true;
	[NSTimer scheduledTimerWithTimeInterval:0.05 target:self selector:@selector(realBack) userInfo:nil repeats:NO];
}

- (void) realBack
{
	[self.view removeFromSuperview];
	[[Community get] showCommunityButton];
}

- (IBAction)showLeaderboard:(id)sender
{
	DmSoundPlaySE("Ok");
	DmSndBgmPlayerBgmStop();
	[NSTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(realShowLeaderboard) userInfo:nil repeats:NO];
}

- (void) realShowLeaderboard
{
	DmSoundPlaySE("Window");
	GKLeaderboardViewController *leaderboardController = [[[GKLeaderboardViewController alloc] init] autorelease];
	if (leaderboardController != NULL) 
	{
		leaderboardController.category = nil;
		leaderboardController.timeScope = GKLeaderboardTimeScopeAllTime;
		leaderboardController.leaderboardDelegate = self; 
		[self presentModalViewController: leaderboardController animated: YES];
	}
}

- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController
{
	[self dismissModalViewControllerAnimated: YES];
	[viewController release];
	DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
}

- (IBAction)showAchievements:(id)sender
{
	DmSoundPlaySE("Ok");
	DmSndBgmPlayerBgmStop();
	[NSTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(realShowAchievements) userInfo:nil repeats:NO];
}

- (void) realShowAchievements
{
	DmSoundPlaySE("Window");
	GKAchievementViewController *achievements = [[GKAchievementViewController alloc] init];
	if (achievements != NULL)
	{
		achievements.achievementDelegate = self;
		[self presentModalViewController: achievements animated: YES];
	}
}

- (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController;
{
	[self dismissModalViewControllerAnimated: YES];
	[viewController release];
	DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
}

@end
