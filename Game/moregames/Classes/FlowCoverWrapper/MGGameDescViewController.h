

#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayer.h>

#import "MoreGamesDownloader.h"

@class MoreGamesViewController;

@interface MGGameDescViewController : UIViewController<MoreGamesDataReceiver> 
{
	IBOutlet UIButton* btnVisitGamePageMGGD;
	IBOutlet UILabel* labelGameTitleMGGD;
	IBOutlet UIImageView* imgGameBgImageMGGD;
	//IBOutlet UITextView* txtGameDescTextMGGD; //remove it
	IBOutlet UIScrollView* scrollGameDescTextMGGD;
	IBOutlet UILabel* labelGameDescTextMGGD;

	IBOutlet UIButton* imgSS1ImageMGGD;
	IBOutlet UIButton* imgSS2ImageMGGD;
	IBOutlet UIButton* imgSS3ImageMGGD;
	IBOutlet UIButton* imgSS4ImageMGGD;
	
	IBOutlet UIView* viewGameDescMGGD;
	
	IBOutlet UIButton* imgSSFullscreenPlaceholderMGGD;
	
	MPMoviePlayerController* moviePCtrl;
	
	MoreGamesViewController* parentController;
	UIImage* defaultImage;
	
}


@property (nonatomic, retain) IBOutlet UIButton* btnVisitGamePageMGGD;
@property (nonatomic, retain) IBOutlet UILabel* labelGameTitleMGGD;
@property (nonatomic, retain) IBOutlet UIImageView* imgGameBgImageMGGD;
//@property (nonatomic, retain) IBOutlet UITextView* txtGameDescTextMGGD;
@property (nonatomic, retain) IBOutlet UIScrollView* scrollGameDescTextMGGD;
@property (nonatomic, retain) IBOutlet UILabel* labelGameDescTextMGGD;


@property (nonatomic, retain) IBOutlet UIButton* imgSS1ImageMGGD;
@property (nonatomic, retain) IBOutlet UIButton* imgSS2ImageMGGD;
@property (nonatomic, retain) IBOutlet UIButton* imgSS3ImageMGGD;
@property (nonatomic, retain) IBOutlet UIButton* imgSS4ImageMGGD;

@property (nonatomic, retain) IBOutlet UIView* viewGameDescMGGD;

@property (nonatomic, retain) IBOutlet UIButton* imgSSFullscreenPlaceholderMGGD;

- (IBAction)actionBack_MGGD:(id)sender;
- (IBAction)actionGoToURL_MGGD:(id)sender;

- (IBAction)actionScreenShot1MG:(id)sender;;
- (IBAction)actionScreenShot2MG:(id)sender;;
- (IBAction)actionScreenShot3MG:(id)sender;;
- (IBAction)actionShowVideoMG:(id)sender;;
- (IBAction)actionCloseScreenShotMG:(id)sender;


-(void) startDownloadImageMGGD;
-(void) setParentController: (MoreGamesViewController*) parentCtrl;
-(void) reinit;
-(bool) dataReceived:(NSData*) data connectionID:(int)CID url:(NSString*)URL; //param: data or nil for error



@end

