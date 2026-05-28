

#import <UIKit/UIKit.h>

#ifdef _MG_IPAD
	#define UPS_SCREEN_WIDTH (1024)
	#define UPS_SCREEN_HEIGHT (768)
#else
	#define UPS_SCREEN_WIDTH (480)
	#define UPS_SCREEN_HEIGHT (320)
#endif



@interface UpsellViewController : UIViewController //NSObject
{
	int currentScreenshot;
	IBOutlet UIButton* btnSS;
	IBOutlet UIScrollView* scrollGameDescTextUPS;
	IBOutlet UILabel* labelGameDescTextUPS;	
	IBOutlet UIButton* imgSSFullscreenPlaceholderUPS;
}

@property (nonatomic, retain) IBOutlet UIButton* btnSS;
@property (nonatomic, retain) IBOutlet UIScrollView* scrollGameDescTextUPS;
@property (nonatomic, retain) IBOutlet UILabel* labelGameDescTextUPS;
@property (nonatomic, retain) IBOutlet UIButton* imgSSFullscreenPlaceholderUPS;



- (void) prepare;

- (IBAction)actionBack:(id)sender;
- (IBAction)actionBuy:(id)sender;	
- (IBAction)actionLeft:(id)sender;	
- (IBAction)actionRight:(id)sender;	
- (IBAction)actionScreenshotMaximize:(id)sender;
- (IBAction)actionCloseScreenShot:(id)sender;

- (void)setBackButtonMode:(bool)fromMenu_or_afterGame; //fromMenu(1), afterGame(0)

@end

///////////////////////////////
#ifdef SONIC4_TRIAL_EXIBITION
@interface DemoSplashViewController : UIViewController //NSObject
{
}

- (void) prepare;

@end
#endif

