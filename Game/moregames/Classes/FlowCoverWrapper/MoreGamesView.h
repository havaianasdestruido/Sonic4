

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "DataCache.h"
#import "MoreGamesDownloader.h"



/*	FlowCoverView
 *
 *		The flow cover view class; this is a drop-in view which calls into
 *	a delegate callback which controls the contents. This emulates the CoverFlow
 *	thingy from Apple.
 */

@class MoreGamesViewController;

@interface ScrollSupportViewMG : UIView //for VoiceOver support
{
}
@end

@interface MoreGamesView : UIView <MoreGamesDataReceiver>
{
	// Current state support
	double offset;
	
	NSTimer *timer;
	double startTime;
	double startOff;
	double startPos;
	double startSpeed;
	double runDelta;
	BOOL touchFlag;
	CGPoint startTouch;
	
	double lastPos;
	

	DataCache *cache;
	
	// OpenGL ES support
    GLint backingWidth;
    GLint backingHeight;
    EAGLContext *context;
    GLuint viewRenderbuffer, viewFramebuffer;
    GLuint depthRenderbuffer;
	
	IBOutlet UIButton* btnSelectGameMG;
    IBOutlet ScrollSupportViewMG* scrollSupportViewMG;
	
	MoreGamesViewController* controller;
    
    float Aspects_YonX[256];
}

@property (nonatomic, retain) IBOutlet UIButton* btnSelectGameMG;
@property (nonatomic, retain) IBOutlet ScrollSupportViewMG* scrollSupportViewMG;
- (void)draw;					// Draw the FlowCover view with current state

-(bool) dataReceived:(NSData*) data connectionID:(int)CID url:(NSString*)URL; //param: data or nil for error
-(void) startDownloadImages;
-(void) setController:(MoreGamesViewController*)cntrl;

- (void) fixIntermittentPosition;

@end


