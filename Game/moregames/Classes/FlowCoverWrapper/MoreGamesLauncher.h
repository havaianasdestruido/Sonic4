//
//  SegaMoreGames.h
//  SegaMoreGamesExampleNew
//
//  Created by Mpp IPhone Developer on 5/18/10.
//  Copyright 2010 MPP. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "MoreGamesViewController.h"

#define MG_VIEW_TRANSITION_TIME (0.6)

@protocol MoreGamesExitReceiver
-(void) moreGamesExit:(bool)result; //true: ok, false - More Games is inaccesible/connection error
@end

@class MGRootViewController; //internal

@interface MoreGamesLauncher : NSObject 
{
	id<MoreGamesExitReceiver> exitReceiver;
    MoreGamesViewController* mgvController;
    UIActivityIndicatorView* activityIndicator;	
	MGRootViewController* rootCtrl;
	CGRect activityIndicatorRect;
	bool exitResult;
	bool isLandscapeLeftEnabled, isLandscapeRightEnabled;
	UIView* parentView;
}

+ (MoreGamesLauncher*) get;
//before start (optional)
- (void)setParentViewController:(UIViewController*)parent;//set parent view if any UIViewController is used in your application (for avoid this problem http://developer.apple.com/iphone/library/qa/qa2010/qa1688.html ) (by default the key UIWindow will be used as parent view).
- (void)enableAutorotateLandscapeLeft:(bool)enableLeft LandscapeRight:(bool)enableRight;//enable autorotation (by default both landscape orientations are enabled) 
- (void)setActivityIndicatorRect:(CGRect)rect;//set position and size for loading indicator before start (by default it will be placed in center)							  
//start
- (void)start; //launch MG for ObjC without callback
- (void)start:(id<MoreGamesExitReceiver>)receiver; //launch MG for ObjC with callback

//for internal using only
- (id)init;
- (void)exit:(bool) result;
- (BOOL)shouldAutorotate:(UIInterfaceOrientation)interfaceOrientation;

@end
