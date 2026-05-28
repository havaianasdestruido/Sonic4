/*
 *  mppAchievementAlertViewController.h
 *  hog
 *
 *  Created by Vyacheslav Mednonogov on Oct/11/10.
 *  Copyright 2010 MPP. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>

#ifdef _MG_IPAD
	#define ACAL_SCREEN_WIDTH (1024)
	#define ACAL_SCREEN_HEIGHT (768)
#else
	#define ACAL_SCREEN_WIDTH (480)
	#define ACAL_SCREEN_HEIGHT (320)
#endif



@interface AchievementAlertViewController : UIViewController //NSObject
{	
	IBOutlet UIButton* btnClose;
	IBOutlet UITextView* textAch;	
	IBOutlet UIImageView* imgAch;
}

@property (nonatomic, retain) IBOutlet IBOutlet UIButton* btnClose;
@property (nonatomic, retain) IBOutlet IBOutlet UITextView* textAch;
@property (nonatomic, retain) IBOutlet IBOutlet UIImageView* imgAch;

- (void) prepare: (int)nAch;
- (IBAction)actionClose:(id)sender;

@end

