//
//  SegaMoreGames.m
//  SegaMoreGamesExampleNew
//
//  Created by Mpp IPhone Developer on 5/18/10.
//  Copyright 2010 MPP. All rights reserved.
//

#import "MoreGamesLauncher.h"
#import "MoreGamesLauncherCpp.h"

#define MG_ACTIVITY_INDICATOR_DEFAULT_X    (MG_SCREEN_WIDTH/2)
#define MG_ACTIVITY_INDICATOR_DEFAULT_Y    (MG_SCREEN_HEIGHT/2)
#define MG_ACTIVITY_INDICATOR_DEFAULT_SZ (32)


//////////////////////////////

@interface MGRootViewController : UIViewController {
}
@end

@implementation MGRootViewController //for support screen orientation

-(void)loadView {
	CGRect rect  = CGRectMake(0, 0, MG_SCREEN_WIDTH, MG_SCREEN_HEIGHT);
	self.view = [[UIView alloc] initWithFrame:rect];
	self.view.bounds = rect;
	CGAffineTransform t = CGAffineTransformMakeRotation(M_PI/2.);
	self.view.transform = t;
	self.view.center = CGPointMake(MG_SCREEN_HEIGHT/2, MG_SCREEN_WIDTH/2);
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return [[MoreGamesLauncher get] shouldAutorotate:interfaceOrientation];
}

@end

//////////////////////////////

MG_ExitCallbackFunction _exitCallbackFunction = NULL;

void MoreGamesLauncher_Start(MG_ExitCallbackFunction exitCallbackFunction)
{
	_exitCallbackFunction = exitCallbackFunction;
	[[MoreGamesLauncher get] start];
}

void MoreGamesLauncher_SetActivityIndicatorRect(int activityIndX, int activityIndY, int activityIndW, int  activityIndH)
{
	[[MoreGamesLauncher get] setActivityIndicatorRect:CGRectMake(activityIndX, activityIndY, activityIndW, activityIndH)];
}

void MoreGamesLauncher_enableAutorotate(bool enableLandscapeLeft, bool enableLandscapeRight)
{
	[[MoreGamesLauncher get] enableAutorotateLandscapeLeft:enableLandscapeLeft LandscapeRight:enableLandscapeRight];

}



		
static 	MoreGamesLauncher* mglInst = nil;
		
@implementation MoreGamesLauncher

+(MoreGamesLauncher*) get
{
	if(mglInst == nil) 
	{
		mglInst = [[MoreGamesLauncher alloc] init];
	}
	return mglInst;
}
-(id)init
{
    mgvController = nil;
    activityIndicator = nil;	
	rootCtrl = nil;
	exitReceiver = nil;
	activityIndicatorRect = CGRectMake(MG_ACTIVITY_INDICATOR_DEFAULT_X-MG_ACTIVITY_INDICATOR_DEFAULT_SZ/2, MG_ACTIVITY_INDICATOR_DEFAULT_Y-MG_ACTIVITY_INDICATOR_DEFAULT_SZ/2, MG_ACTIVITY_INDICATOR_DEFAULT_SZ, MG_ACTIVITY_INDICATOR_DEFAULT_SZ);
	isLandscapeLeftEnabled = true;
	isLandscapeRightEnabled = true;
	parentView = nil;
	return self;
}

- (void)closeMoreGamesAndPassResultToCallbackFunction:(bool) result 
{
	[activityIndicator removeFromSuperview];
	[activityIndicator release];
	activityIndicator = nil;
	
	/*
	[rootCtrl.view removeFromSuperview];
	[rootCtrl release];
	rootCtrl = nil;
	 */
	rootCtrl.view.hidden = true;

	
	NSLOG((@"MG EXIT -- %@" , (result?@"OK":@"FAILED")));
	if(_exitCallbackFunction!=NULL) {
		(*_exitCallbackFunction)(result);
		_exitCallbackFunction=NULL;
	}
	if(exitReceiver!=nil) {
		[exitReceiver moreGamesExit:result];
	}	
}

- (void)animationDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
	UIView* child_view = mgvController.view;		
	[child_view removeFromSuperview];
	//
	[self closeMoreGamesAndPassResultToCallbackFunction: exitResult];
}

-(void)exit: (bool) success
{
	exitResult = success;
	UIView* child_view = mgvController.view;	
	
	[UIView setAnimationsEnabled:false];
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationsEnabled:true];
	[UIView setAnimationDuration: MG_VIEW_TRANSITION_TIME ];
	CGRect contentRect2 = CGRectMake(0, -MG_SCREEN_HEIGHT, MG_SCREEN_WIDTH, MG_SCREEN_HEIGHT);
	child_view.bounds = contentRect2;
	// Commit animation
	[UIView setAnimationDelegate:self];
	[UIView commitAnimations];	

}

-(void)tryToOpenMG_Loop
{
	
	activityIndicator.hidden = false;
	[rootCtrl.view setNeedsLayout];
	[rootCtrl.view setNeedsDisplay];
	//
	if([MoreGamesDownloader isPreinitCompletted]==false) {		
		[NSTimer scheduledTimerWithTimeInterval:0.3
                                         target:self
                                       selector:@selector(tryToOpenMG_Loop)
                                       userInfo:nil
                                        repeats:NO];		
	}
	else {		
		if( [MoreGamesDownloader get]!=nil &&  [[MoreGamesDownloader get] getNumberOfGameDescs]>0) {
            
			if(mgvController==nil) //optimization: create one time only
            {
				//mgvController = [[MoreGamesViewController alloc] initWithNibName:@"iGenMG" bundle:nil];
			
				NSArray *array = [[NSBundle mainBundle] loadNibNamed:@"iGenMG" owner:nil options:nil];
				for(int nn=0; nn<[array count]; nn++) {
					id elem = [array objectAtIndex:nn];
					if([elem isKindOfClass:[UIViewController class]] == YES) {		
						mgvController = [array objectAtIndex:nn];
						[mgvController retain];
						break;
					}
				}	
            }
			else {
				[mgvController.mgView startDownloadImages];
			}
            [mgvController retain];
			//
			UIView* child_view = mgvController.view;
			//
			child_view.center = CGPointMake(MG_SCREEN_WIDTH/2, MG_SCREEN_HEIGHT/2);						
			/*
			else
				CGAffineTransform t = CGAffineTransformMakeRotation(M_PI/2.);
				child_view.transform = t;
				child_view.center = CGPointMake(MG_SCREEN_HEIGHT/2, MG_SCREEN_WIDTH/2);						
			}*/
			//
			[rootCtrl.view addSubview: child_view];		
			
			[UIView setAnimationsEnabled:false];
			CGRect contentRect = CGRectMake(0, -MG_SCREEN_HEIGHT, MG_SCREEN_WIDTH, MG_SCREEN_HEIGHT);
			child_view.bounds = contentRect;
			[child_view setBackgroundColor:[UIColor colorWithWhite:0. alpha:0.]];
			//[child_view setAlpha:0.0f];
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationsEnabled:true];
			[UIView setAnimationDuration:MG_VIEW_TRANSITION_TIME ];
			//[child_view setAlpha:1.f];	
			CGRect contentRect2 = CGRectMake(0, 0, MG_SCREEN_WIDTH, MG_SCREEN_HEIGHT);
			child_view.bounds = contentRect2;
			// Commit animation
			[UIView commitAnimations];
			activityIndicator.hidden = true;
		}
		else {			
			[self closeMoreGamesAndPassResultToCallbackFunction: false];
		}
	}
}

-(void)tryToOpenMG_LoopEntry
{
	[UIView setAnimationsEnabled:true];
	[MoreGamesDownloader preinit];
	[self tryToOpenMG_Loop];
}


- (void)setActivityIndicatorRect:(CGRect)rect
{
	activityIndicatorRect = rect;
}

- (void)enableAutorotateLandscapeLeft:(bool)enableLeft LandscapeRight:(bool)enableRight
{
	isLandscapeLeftEnabled = enableLeft;
	isLandscapeRightEnabled = enableRight;

}

- (void)setParentViewController:(UIViewController*)parent
{
	parentView = parent.view;
}

- (BOOL)shouldAutorotate:(UIInterfaceOrientation)interfaceOrientation
{
	if(isLandscapeLeftEnabled && !isLandscapeRightEnabled) {
		return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft);
	}
	else if(!isLandscapeLeftEnabled && isLandscapeRightEnabled) {
		return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
	}
	else {
		return ((interfaceOrientation == UIInterfaceOrientationLandscapeLeft)
			||(interfaceOrientation == UIInterfaceOrientationLandscapeRight));
	}

}


- (void)start:(id<MoreGamesExitReceiver>)receiver //for ObjC
{
	exitReceiver = receiver;
	[self start];
}

- (void)start
{	
	exitResult = false;
	
	//[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
	if(rootCtrl == nil) {
		rootCtrl = [[MGRootViewController alloc] init];
		[rootCtrl.view setBackgroundColor:[UIColor colorWithWhite:0. alpha:0.]];
		if(parentView==nil) {
			[[[UIApplication sharedApplication].windows objectAtIndex:0] addSubview:rootCtrl.view];
		}
		else {
			[parentView addSubview:rootCtrl.view];
		}
	}
	else {
		rootCtrl.view.hidden = false;
	}
	//[[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
	
	//bring More Games view to front
	if(parentView==nil) {
		[[[UIApplication sharedApplication].windows objectAtIndex:0] bringSubviewToFront:rootCtrl.view];
	}
	else {
		[parentView bringSubviewToFront:rootCtrl.view];
	}	
	
	if(activityIndicator == nil) {
		//CGRect screenRect = CGRectMake(MG_SCREEN_HEIGHT-activityIndicatorRect.origin.y-activityIndicatorRect.size.height, activityIndicatorRect.origin.x, activityIndicatorRect.size.height, activityIndicatorRect.size.width);
		CGRect screenRect = CGRectMake(activityIndicatorRect.origin.x, activityIndicatorRect.origin.y, activityIndicatorRect.size.width, activityIndicatorRect.size.height);
		activityIndicator = [[UIActivityIndicatorView alloc] initWithFrame:screenRect];
		[rootCtrl.view addSubview:activityIndicator];
	}
	
	[activityIndicator startAnimating];
	//activityIndicator.hidden = true;	
	
	
	
	[rootCtrl.view setNeedsLayout];
	[rootCtrl.view setNeedsDisplay];
	
	[NSTimer scheduledTimerWithTimeInterval:0.1
									 target:self
								   selector:@selector(tryToOpenMG_LoopEntry)
								   userInfo:nil
									repeats:NO];
	

}





@end
		
