

#import <UIKit/UIKit.h>
#import "MoreGamesView.h"
#import "LogDebugOnly.h"

@class MGGameDescViewController;

@interface MoreGamesViewController : UIViewController 
{
	IBOutlet MoreGamesView* mgView;
    
    MGGameDescViewController *mggdvController;
	
}

@property (nonatomic, retain) IBOutlet MoreGamesView* mgView;
@property (nonatomic, retain) MGGameDescViewController *mggdvController;

- (IBAction)actionBackMG:(id)sender;
- (IBAction)actionGoToMG:(id)sender;

- (void)closeAndExit:(bool)success;
- (void)closeChildView;


@end

