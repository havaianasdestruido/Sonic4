#import "MoreGamesViewController.h"
#import "MoreGamesDownloader.h"
#import "MGGameDescViewController.h"
#import "MoreGamesLauncher.h"



@implementation MoreGamesViewController

@synthesize mgView;
@synthesize mggdvController;

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



// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	[MoreGamesDownloader get];
	[mgView setController:self];
}

-(void)viewDidDisappear:(BOOL)animated{
    [mgView fixIntermittentPosition];
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
}

- (void)closeAndExit:(bool)success;
{
	
	//stop any downloads (for coverflow)
    MoreGamesDownloader* mgd = [MoreGamesDownloader get];
	[mgd cancelAnyDownloadsForReceiver:mgView];
	//
	if(mggdvController!=nil) {
		//stop any downloads (for game description)
		[mgd cancelAnyDownloadsForReceiver:mggdvController];
		[mggdvController.view removeFromSuperview];
		[mggdvController release];
		mggdvController=nil;
	}
	//exit
	[[MoreGamesLauncher get] exit:success];
}



- (IBAction)actionBackMG:(id)sender
{
	[self closeAndExit: true];
}

- (IBAction)actionGoToMG:(id)sender
{
	//[[self parentViewController] dismissModalViewControllerAnimated:YES];
	
	if(mggdvController==nil) {
	
		NSArray *array = [[NSBundle mainBundle] loadNibNamed:@"iGenMG_GameDesc" owner:[self parentViewController] options:nil];
		for(int nn=0; nn<[array count]; nn++) {
			id elem = [array objectAtIndex:nn];
			if([elem isKindOfClass:[UIViewController class]] == YES) {		
				mggdvController = [array objectAtIndex:nn];
				[mggdvController retain];
				break;
			}
		}
    }
	[mggdvController setParentController: self];
	[mggdvController reinit];
	
	//[self presentModalViewController:mggdvController animated:YES];
	UIView* child_view = mggdvController.view;
	[self.view addSubview: child_view];		
	//
	[UIView setAnimationsEnabled:false];
	CGRect contentRect = CGRectMake(0, -MG_SCREEN_HEIGHT, MG_SCREEN_WIDTH, MG_SCREEN_HEIGHT);
	child_view.bounds = contentRect;
	child_view.center = CGPointMake(MG_SCREEN_WIDTH/2, MG_SCREEN_HEIGHT/2);
	[child_view setBackgroundColor:[UIColor colorWithWhite:0. alpha:0.]];
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationsEnabled:true];
	[UIView setAnimationDuration:MG_VIEW_TRANSITION_TIME ];
	CGRect contentRect2 = CGRectMake(0, 0, MG_SCREEN_WIDTH, MG_SCREEN_HEIGHT);
	child_view.bounds = contentRect2;
	// Commit animation
	[UIView commitAnimations];
}

- (void)animationDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
	UIView* child_view = mggdvController.view;		
	[child_view removeFromSuperview];
}

- (void)closeChildView
{
	UIView* child_view = mggdvController.view;	
	//
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



@end
