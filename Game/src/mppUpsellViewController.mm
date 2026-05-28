#include "dmTitle.h"
#import "mppUpsellViewController.h"

#include "dmSndBgmPlayer.h"
#include "MainMenuConst.h"

@implementation UpsellViewController

static const int MAX_NUMBER_OF_SSCREENSHOTS = 5;

@synthesize btnSS;
@synthesize scrollGameDescTextUPS;
@synthesize labelGameDescTextUPS;
@synthesize imgSSFullscreenPlaceholderUPS;


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
	return FALSE ;	
}


- (void) doFlashScrollIndicators
{
    UIScrollView* scrollView = scrollGameDescTextUPS;
    if(scrollView) {
	    [scrollView flashScrollIndicators];
    }
}

- (void) setImageForAllButtonStates:(UIButton*)btn image:(UIImage*)img
{
    [btn setImage: img forState:UIControlStateNormal];		
    [btn setImage: img forState:UIControlStateHighlighted];		
    [btn setImage: img forState:UIControlStateDisabled];		
    [btn setImage: img forState:UIControlStateSelected];
}

- (void) loadScreenshot
{
	NSString* fname = [NSString stringWithFormat: @"s4us_ss_%i.png", (currentScreenshot+1)];
	UIImage* img = [UIImage imageNamed:fname];
	[self setImageForAllButtonStates:btnSS image:img];
}

- (void) prepare
{
	
	currentScreenshot = 0;
	[self loadScreenshot];
	
	NSString* newText = NSLocalizedString(@"upsell_desc1", @"");
	
   
    //calculate scroll size
    UILabel* scrollLabel = labelGameDescTextUPS;
    UIScrollView* scrollView = scrollGameDescTextUPS;
    if( scrollView )
    {
        
        // Reset scroll size
        scrollView.contentSize = CGSizeMake( 1, 1 );        
        scrollView.contentOffset = CGPointMake( 0, 0); 
        
        [scrollLabel setText: newText];
        
        CGRect frame = CGRectMake(0.0, 0.0, scrollLabel.frame.size.width, 1000.0); 
        CGSize calcSize = [newText sizeWithFont:scrollLabel.font
                              constrainedToSize:frame.size lineBreakMode: UILineBreakModeWordWrap];            
        
        //adjust the label the the new height.
        CGRect newFrame = scrollLabel.frame;
        newFrame.size.height = calcSize.height+1;
        scrollLabel.frame = newFrame;
        
        
        // Find height of contents
        float height = 0.0f;
        for( UIView* view in scrollView.subviews)
        {
            CGRect view_frame =  view.frame;
            float bottom =  +  view_frame.size.height ; //bugfix 816
            if( bottom > height )
                height = bottom;
        }
        
        // Set scroll size
        scrollView.contentSize = CGSizeMake( scrollView.frame.size.width, height );
        scrollView.contentOffset = CGPointMake( scrollView.contentOffset.x, 0);
        
        // Make scroll indicators be white
        scrollView.indicatorStyle = UIScrollViewIndicatorStyleWhite;
        
        //flash scroll indicator 3 times
        [self doFlashScrollIndicators];        
        [NSTimer scheduledTimerWithTimeInterval:1.5 target:self selector:@selector(doFlashScrollIndicators) userInfo:nil repeats:NO];
        [NSTimer scheduledTimerWithTimeInterval:3.0 target:self selector:@selector(doFlashScrollIndicators) userInfo:nil repeats:NO];
    }  	
	
	imgSSFullscreenPlaceholderUPS.alpha = 0.f;
	
}

static int s_backButtonMode = 0; //1-from menu, 0-after game

- (void)setBackButtonMode:(bool)fromMenu_or_afterGame //fromMenu(1), afterGame(0)
{
	s_backButtonMode = fromMenu_or_afterGame;
}



- (IBAction)actionBack:(id)sender
{
	[self.view removeFromSuperview];
	if(s_backButtonMode) {
		mppUS_UpsellScreenFinished(false);
	}
	else {
		DmSndBgmPlayerPlayBgm(DME_SND_BGM_PLAYER_IDX_MENU);
	}
}

- (IBAction)actionBuy:(id)sender
{
	mppUS_UpsellScreenFinished(true);
	NSURL* url = [NSURL URLWithString: NSLocalizedString(@"SONIC4_BUY_URL", @"")];
	[[UIApplication sharedApplication] openURL: url];
	[NSThread sleepForTimeInterval:0.5];
	exit(0);
}

- (IBAction)actionRight:(id)sender
{
	currentScreenshot++;
	if(currentScreenshot>=MAX_NUMBER_OF_SSCREENSHOTS) currentScreenshot = 0;
	[self loadScreenshot];
}

- (IBAction)actionLeft:(id)sender
{
	currentScreenshot--;
	if(currentScreenshot<0) currentScreenshot = MAX_NUMBER_OF_SSCREENSHOTS-1;
	[self loadScreenshot];
}


#define MG_SSHOT_TRANSITION_DURATION (0.333f)

-(void)showScreenshot:(UIImage*) img
{
	UIButton* btn = imgSSFullscreenPlaceholderUPS;
	[btn setImage: img forState:UIControlStateNormal];		
	[btn setImage: img forState:UIControlStateHighlighted];		
	[btn setImage: img forState:UIControlStateDisabled];		
    [btn setImage: img forState:UIControlStateSelected];
	
	btn.hidden = FALSE;
	CGRect r = {{0,0},{UPS_SCREEN_WIDTH, UPS_SCREEN_HEIGHT}};
	btn.frame = r;
	
	[UIView setAnimationsEnabled:false];
	[btn setAlpha:0.0f];
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationsEnabled:true];
	[UIView setAnimationDuration:MG_SSHOT_TRANSITION_DURATION];
	[btn setAlpha:1.f];	
	// Commit animation
	[UIView commitAnimations];	
	
}

- (IBAction)actionScreenshotMaximize:(id)sender
{
	UIImage* img = [btnSS imageForState:UIControlStateNormal];	
	[self showScreenshot: img];
}


- (IBAction)actionCloseScreenShot:(id)sender
{
	UIButton* btn = imgSSFullscreenPlaceholderUPS;
	
	//btn.hidden = TRUE;
	
	[UIView setAnimationsEnabled:false];
	[btn setAlpha:1.0f];
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationsEnabled:true];
	[UIView setAnimationDuration:MG_SSHOT_TRANSITION_DURATION];
	[btn setAlpha:0.f];	
	// Commit animation
	[UIView commitAnimations];	
}



@end

////////////////////////////////////////////////////////////////////////

#ifdef SONIC4_TRIAL_EXIBITION
@implementation DemoSplashViewController

#define DDSP_TRANSITION_DURATION (0.5f)
#define DDSP_SHOW_TIME (3.5f)
bool isDemoSplashShown_flag = false;

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
	return FALSE ;	
}

- (void) realClose
{
	isDemoSplashShown_flag = false;
}

- (void) fadeOut
{
	[UIView setAnimationsEnabled:false];
	[self.view setAlpha:1.0f];
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationsEnabled:true];
	[UIView setAnimationDuration:DDSP_TRANSITION_DURATION];
	[self.view setAlpha:0.f];	
	// Commit animation
	[UIView commitAnimations];	
	
	[NSTimer scheduledTimerWithTimeInterval:DDSP_TRANSITION_DURATION target:self selector:@selector(realClose) userInfo:nil repeats:NO];
	

}

- (void) prepare
{
	isDemoSplashShown_flag = true;
	
	[UIView setAnimationsEnabled:false];
	[self.view setAlpha:0.0f];
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationsEnabled:true];
	[UIView setAnimationDuration:DDSP_TRANSITION_DURATION];
	[self.view setAlpha:1.f];	
	// Commit animation
	[UIView commitAnimations];	
	
	[NSTimer scheduledTimerWithTimeInterval:DDSP_SHOW_TIME target:self selector:@selector(fadeOut) userInfo:nil repeats:NO];
	
}

@end
#endif



