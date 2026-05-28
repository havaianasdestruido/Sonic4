/*
 *  mppAchievementAlertViewController.mm
 *  hog
 *
 *  Created by Vyacheslav Mednonogov on Oct/11/10.
 *  Copyright 2010 MPP. All rights reserved.
 *
 */

#include "mppUtil.h"
#include "mppAchievementSupport.h"
#import "mppAchievementAlertViewController.h"

@implementation AchievementAlertViewController

#define ACAL_TEXT_SIZE (17)

@synthesize btnClose;
@synthesize textAch;	
@synthesize imgAch;

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
	return FALSE ;	
}

bool isAchievementAlertShown_flag = false;

#define ACAL_TRANSITION_DURATION (0.5)

- (void) prepare: (int) nAch //0..11
{	
	[UIView setAnimationsEnabled:false];
	self.view.center = CGPointMake(ACAL_SCREEN_HEIGHT, ACAL_SCREEN_WIDTH/2);
	[self.view setAlpha:0.0f];
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationsEnabled:true];
	[UIView setAnimationDuration:ACAL_TRANSITION_DURATION];
	self.view.center = CGPointMake(ACAL_SCREEN_HEIGHT/2, ACAL_SCREEN_WIDTH/2);
	[self.view setAlpha:1.f];	
	// Commit animation
	[UIView commitAnimations];
	
	
	mppUtil::playSFX_WindowOpen();
	isAchievementAlertShown_flag = true;
	nAch += 1; //1..12	
	
	{{
		NSString* imgName = [NSString stringWithFormat:@"achieve_%03d.png", nAch];
		UIImage* img = [UIImage imageNamed:imgName];
		imgAch.image = img;
	}}
	
	{{
		NSString* txtKey = [NSString stringWithFormat:@"ach_desc_txt_%d", nAch];
		NSString* txt = NSLocalizedString(txtKey, @"");
		NSString* prefix = NSLocalizedString(@"ach_alert_txt", @"");
		NSString* postfix = NSLocalizedString(@"ach_alert_txt_end", @"");
		if(postfix==nil || [postfix length]==0) {
			textAch.text = [NSString stringWithFormat:@"%@\n%@", prefix, txt];	
		}
		else {
			textAch.text = [NSString stringWithFormat:@"%@%@%@", prefix, txt, postfix]; //for jpn
		}
		textAch.font = [UIFont systemFontOfSize:ACAL_TEXT_SIZE];		
	}}
	
}

-(void)realClose
{
	[self.view removeFromSuperview];
	
	if(mppUtil::showAchievementAlertIfNecessary()==false) {
		isAchievementAlertShown_flag = false;
	}
}

- (IBAction)actionClose:(id)sender
{
	mppUtil::playSFX_ButtonPress();
	const int nAch = mppAchievementSupport::get()->getAchievementAlertID();
	if(nAch>=0) { //another acievement present
		[self prepare:nAch];
	}
	else { //else close
		[NSTimer scheduledTimerWithTimeInterval:ACAL_TRANSITION_DURATION target:self selector:@selector(realClose) userInfo:nil repeats:NO];
		//
		[UIView setAnimationsEnabled:false];
		self.view.center = CGPointMake(ACAL_SCREEN_HEIGHT/2, ACAL_SCREEN_WIDTH/2);
		[self.view setAlpha:1.f];
		[UIView beginAnimations:nil context:NULL];
		[UIView setAnimationsEnabled:true];
		[UIView setAnimationDuration:ACAL_TRANSITION_DURATION];
		self.view.center = CGPointMake(0, ACAL_SCREEN_WIDTH/2);
		[self.view setAlpha:0.0f];	
		// Commit animation
		[UIView commitAnimations];		
	}
}

@end



