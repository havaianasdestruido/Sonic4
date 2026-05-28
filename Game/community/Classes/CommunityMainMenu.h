//
//  CommunityMainMenu.h
//  hog
//
//  Created by Nikolai Lisun on 9/29/10.
//  Copyright 2010 MPP. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GameKit/GameKit.h>


@interface CommunityMainMenu : UIViewController<GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate>
{

}

- (IBAction)back:(id)sender;
- (IBAction)showLeaderboard:(id)sender;
- (IBAction)showAchievements:(id)sender;
- (void)realShowLeaderboard;
- (void)realShowAchievements;

@end
