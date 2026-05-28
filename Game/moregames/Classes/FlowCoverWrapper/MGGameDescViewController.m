#import "MGGameDescViewController.h"
#import "MoreGamesDownloader.h"
#import "MoreGamesLauncher.h"


@implementation MGGameDescViewController

@synthesize btnVisitGamePageMGGD;
@synthesize labelGameTitleMGGD;
@synthesize imgGameBgImageMGGD;
//@synthesize txtGameDescTextMGGD;
@synthesize scrollGameDescTextMGGD;
@synthesize labelGameDescTextMGGD;

@synthesize imgSS1ImageMGGD;
@synthesize imgSS2ImageMGGD;
@synthesize imgSS3ImageMGGD;
@synthesize imgSS4ImageMGGD;

@synthesize viewGameDescMGGD;

@synthesize imgSSFullscreenPlaceholderMGGD;


/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/


/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


enum IMAGE_CIDS {
    _ICID_GAME_INDEX_MASK  = 0x000000FF,
    _ICID_IMAGE_INDEX_MASK = 0xFFFFFF00,
	ICID_BG			= 0x3F00,
	ICID_SS1		= 0x4100,
	ICID_SS2		= 0x4200,
	ICID_SS3		= 0x4300,
	ICID_SS_VIDEO   = 0x4400,
};


- (void) doFlashScrollIndicators
{
    UIScrollView* scrollView = scrollGameDescTextMGGD;
    if(scrollView) {
	    [scrollView flashScrollIndicators];
    }
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	
    [super viewDidLoad];
    [self reinit];
}

   
- (void) setImageForAllButtonStates:(UIButton*)btn image:(UIImage*)img
{
    [btn setImage: img forState:UIControlStateNormal];		
    [btn setImage: img forState:UIControlStateHighlighted];		
    [btn setImage: img forState:UIControlStateDisabled];		
    [btn setImage: img forState:UIControlStateSelected];
}

-(bool) isButtonLoaded:(UIButton*)btn
{
	return [btn imageForState:UIControlStateNormal] != defaultImage;
}
    
	
- (void) setParentController : (MoreGamesViewController*) parentCtrl
{
	parentController = parentCtrl;
}

- (void) reinit
{
	MoreGamesDownloader* mgd = [MoreGamesDownloader get];
	int index = [mgd getCurrentMG];
	MG_GameDesc* gd = [mgd getGameDesc:index];
	
	[labelGameTitleMGGD setText: gd.Name]; 
    NSString* newText = gd.Description; 
	
    //reset description images to default
	if(defaultImage==nil) {
		defaultImage = [UIImage imageNamed:MG_IMAGE_NA_BG];
		[defaultImage retain]; //os4
	}
    [imgGameBgImageMGGD setImage: defaultImage];	
    [self setImageForAllButtonStates:imgSS1ImageMGGD image:defaultImage];
    [self setImageForAllButtonStates:imgSS2ImageMGGD image:defaultImage];
    [self setImageForAllButtonStates:imgSS3ImageMGGD image:defaultImage];
    [self setImageForAllButtonStates:imgSS4ImageMGGD image:defaultImage];	
    
    //calculate scroll size
    UILabel* scrollLabel = labelGameDescTextMGGD;
    UIScrollView* scrollView = scrollGameDescTextMGGD;
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
            float bottom = /*view_frame.origin.y*/ +  view_frame.size.height ; //bugfix 816
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
	
	imgSSFullscreenPlaceholderMGGD.alpha = 0.f;
    
	
	/*sss	 
	UIButton* btn = btnVisitGamePageMGGD;
	NSString* newTitle = gd.Price;//test
	
	[btn setTitle:newTitle forState:UIControlStateNormal];
	[btn setTitle:newTitle forState:UIControlStateHighlighted];
	[btn setTitle:newTitle forState:UIControlStateDisabled];
	[btn setTitle:newTitle forState:UIControlStateSelected];
	*/
	
	[self startDownloadImageMGGD];
	
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
	return [[MoreGamesLauncher get] shouldAutorotate:interfaceOrientation];
}

/*

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
   [self layoutSubview];
	
}*/


- (void)didReceiveMemoryWarning 
{
    [super didReceiveMemoryWarning];
}


- (void)dealloc 
{
    [super dealloc];

	[moviePCtrl dealloc];
	moviePCtrl = nil;
	
	[defaultImage release];
	defaultImage = nil;

}


-(void) startDownloadImageMGGD
{
	MoreGamesDownloader* mgd = [MoreGamesDownloader get];
	int index = [mgd getCurrentMG];
	MG_GameDesc* gd = [mgd getGameDesc:index];
	[mgd startDownload:gd.BackgroundImage receiver:self  connectionID:(ICID_BG + index)];
	[mgd startDownload:gd.ScreenshotImage1 receiver:self connectionID:(ICID_SS1 + index)];
	[mgd startDownload:gd.ScreenshotImage2 receiver:self connectionID:(ICID_SS2 + index)];
	[mgd startDownload:gd.ScreenshotImage3 receiver:self connectionID:(ICID_SS3 + index)];
	[mgd startDownload:gd.ScreenshotImageVideo receiver:self connectionID:(ICID_SS_VIDEO + index)];
}

-(bool) dataReceived:(NSData*) data connectionID:(int)CID url:(NSString*)URL
{
	UIImage* img = nil;
	bool dataIsValid ;
    
	MoreGamesDownloader* mgd = [MoreGamesDownloader get];
	int index = [mgd getCurrentMG];    
    
    //check -- recived data is not for current game
    
    if((CID&_ICID_GAME_INDEX_MASK) != index) {
        NSLOG((@"CID=%i is inconsistent with game=%i. Download data skipped..", CID, index));
        return false;
    }
    
    CID = (CID&_ICID_IMAGE_INDEX_MASK);
	
	if(data!=nil) {
		img = [[UIImage alloc] initWithData:data];
		if(img == nil) {
			data = [[mgd getDiskDataCache] getDataForKey:URL checkExpirationDate:false];
			img = [[UIImage alloc] initWithData:data];            
            RELEASE_NSDATA(data);
		}
	}    
	
	if(img == nil) {
		if(CID==ICID_BG) {
			img = [UIImage imageNamed:MG_IMAGE_NA_BG];
		}
		else {
			img = [UIImage imageNamed:MG_IMAGE_NA];
		}
		dataIsValid = false;
	}
	else {
		dataIsValid = true;
	}

	switch (CID) {
		case ICID_BG: {
			[imgGameBgImageMGGD setImage: img];		
			[imgGameBgImageMGGD setNeedsDisplay];
		}break;
		case ICID_SS1: 
		case ICID_SS2:
		case ICID_SS3:
		case ICID_SS_VIDEO: {
			
			UIButton* btn = nil;
			switch(CID) {
				case ICID_SS1: btn = imgSS1ImageMGGD; break;
				case ICID_SS2: btn = imgSS2ImageMGGD; break;
				case ICID_SS3: btn = imgSS3ImageMGGD; break;
				case ICID_SS_VIDEO: btn = imgSS4ImageMGGD; break;					
			}
			
			[self setImageForAllButtonStates:btn image:img];
			[btn setNeedsDisplay];
			 
		}break;
			
		default:
			break;
	}
    
    if(dataIsValid) {
	    [img release];//bugfix for memory leaks
    }
	
	[viewGameDescMGGD setNeedsLayout];
	[viewGameDescMGGD setNeedsDisplay];
	
	return dataIsValid;
}

- (void)processConnectionError
{

	[parentController closeAndExit:false];

}

////////////////////// button reactions ////////////////////

- (IBAction)actionBack_MGGD:(id)sender
{
    //stop any downloads for this game
    MoreGamesDownloader* mgd = [MoreGamesDownloader get];
	[mgd cancelAnyDownloadsForReceiver:self];
	//close view
	[parentController closeChildView];
}

- (IBAction)actionGoToURL_MGGD:(id)sender
{
	
	MoreGamesDownloader* mgd = [MoreGamesDownloader get];
	int index = [mgd getCurrentMG];
	MG_GameDesc* gd = [mgd getGameDesc:index];
	NSURL* url = [NSURL URLWithString: gd.AppStoreURL];
	[[UIApplication sharedApplication] openURL: url];
}


#define MG_SSHOT_TRANSITION_DURATION (0.3f)

-(void)showScreenshot:(UIImage*) img
{
	UIButton* btn = imgSSFullscreenPlaceholderMGGD;
	[btn setImage: img forState:UIControlStateNormal];		
	[btn setImage: img forState:UIControlStateHighlighted];		
	[btn setImage: img forState:UIControlStateDisabled];		
    [btn setImage: img forState:UIControlStateSelected];
	
	btn.hidden = FALSE;
	CGRect r = {{0,0},{MG_SCREEN_WIDTH,MG_SCREEN_HEIGHT}};
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

- (IBAction)actionCloseScreenShotMG:(id)sender
{
	UIButton* btn = imgSSFullscreenPlaceholderMGGD;

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


- (IBAction)actionScreenShot1MG:(id)sender
{
	if([self isButtonLoaded: imgSS1ImageMGGD]) {
		UIImage* img = [imgSS1ImageMGGD imageForState:UIControlStateNormal];	
		[self showScreenshot: img];
	}
}

- (IBAction)actionScreenShot2MG:(id)sender
{
	if([self isButtonLoaded: imgSS2ImageMGGD]) {
		UIImage* img = [imgSS2ImageMGGD imageForState:UIControlStateNormal];	
		[self showScreenshot: img];
	}
}

- (IBAction)actionScreenShot3MG:(id)sender
{
	if([self isButtonLoaded: imgSS3ImageMGGD]) {
		UIImage* img = [imgSS3ImageMGGD imageForState:UIControlStateNormal];	
		[self showScreenshot: img];
	}
}

- (IBAction)actionShowVideoMG:(id)sender
{
	if([self isButtonLoaded: imgSS4ImageMGGD]) {
		MoreGamesDownloader* mgd = [MoreGamesDownloader get];
		int index = [mgd getCurrentMG];
		MG_GameDesc* gd = [mgd getGameDesc:index];
		NSString *movieURL = gd.VideoLink;
		
		if(movieURL==nil || [movieURL length]==0) {
			NSLOG((@"show as simple ss"));
			UIImage* img = [imgSS4ImageMGGD imageForState:UIControlStateNormal];	
			[self showScreenshot: img];        
		}
		else {
		
			[moviePCtrl dealloc];
			moviePCtrl = nil;
			
			NSURL* cnt_url = [NSURL URLWithString: movieURL];
			
			moviePCtrl = [[MPMoviePlayerController alloc] initWithContentURL: cnt_url];
			moviePCtrl.scalingMode = MPMovieScalingModeAspectFill;//MPMovieScalingModeNone;
			moviePCtrl.movieControlMode = MPMovieControlModeDefault;
			
			NSLog(@"begin playback");
			[moviePCtrl play];
			NSLog(@"end playback");
		}
	}
}



@end
