//
//  Sonic4_EAGLView.m
//  hog
//
//  Created by USE1264 on 09/07/28.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//



#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import <AudioToolbox/AudioServices.h>
#import <nn.h>

#import "Sonic4_EAGLView.h"
#import "Sonic4_AppMain.h"
#import "Sonic4_AppVar.h"

#import "SMUtil.h"

#include "CriSmpSoundOutput_iPhone.h"
#include "amIPhoneBase.h"


#define EAGLVIEW_EVENT_IGNORE_COUNT (5)

static BOOL app_init = NO;

// A class extension to declare private methods
@interface Sonic4_EAGLView ()

@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) NSTimer *animationTimer;

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;

- (NSString*) copyImageFileToDocDir;
- (NSString*) getResDir;

@end


@implementation Sonic4_EAGLView

@synthesize context;
@synthesize animationTimer;
@synthesize animationInterval;


// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}


//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder {
#if 0
    AppLog("init begin");
    if ((self = [super initWithCoder:coder])) {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.opacity = 1.0f;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat, nil];
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context]) {
            [self release];
            return nil;
        }
        
		// Set up the ability to track multiple touches.
		[self setMultipleTouchEnabled:YES];
		
		// accel
		app_accel = FALSE;
		//[self initAccel];
		
		// sleep
		app_sleep = TRUE;
		_am_sample_is_sleep = TRUE;
		[UIApplication sharedApplication].idleTimerDisabled = NO;
		
		animationCount = _am_sample_count;
        animationInterval = 1.0 / (60.0 / (double)animationCount);
		
		// event ignore
		ignoreEventCount = EAGLVIEW_EVENT_IGNORE_COUNT;
    }
	AppLog("init done");
    return self;
#else
	
	self = [self initWithFrame:CGRectMake(0, 0, 480, 320)];
    return self;
	
#endif
}

- (void)drawView {
	// check accel
	if (app_accel) {
		if (++accelCount > 10) {
			[self initAccel];
		}
	}
	
	// check event ignore
	if (ignoreEventCount > 0) {
		--ignoreEventCount;
	}		
	
#if 0
	CFStringRef ref;
	UInt32 size = sizeof(CFStringRef);
	AudioSessionGetProperty(kAudioSessionProperty_AudioRoute, &size, &ref);
	char *routeStr = CFStringGetCStringPtr(ref, CFStringGetSystemEncoding());
	if (strcmp("HeadphonesBT", routeStr)) {
		printf("ROUTE %s \n", routeStr);
	}
#endif // 0
	
    // Replace the implementation of this method to do your own custom drawing
    [EAGLContext setCurrentContext:context];

    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
	
    if (Sonic4_AppMainLoop() == 0)
	{
		if (animationCount != _am_sample_count) {
			animationCount = _am_sample_count;
			[self setAnimationInterval:(1.0 / (60.0 / (double)animationCount))];
			[self initAccel];
		}

		if (app_sleep != _am_sample_is_sleep) {
			app_sleep = _am_sample_is_sleep;
			if (app_sleep) {
				[UIApplication sharedApplication].idleTimerDisabled = NO;
			}
			else {
				[UIApplication sharedApplication].idleTimerDisabled = YES;
			}
		}
		
		if (app_accel != _am_sample_is_accel) {
			app_accel = _am_sample_is_accel;
			if (app_accel) {
				[self initAccel];
			}
			else {
				[[UIAccelerometer sharedAccelerometer] setDelegate:nil]; // accel off
			}
		}

		glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
		[context presentRenderbuffer:GL_RENDERBUFFER_OES];
	}
}

- (void)initAccel {
	if (!app_accel) {
		//NSLog(@"No Init Accel [1/%d]", animationCount);
		return;
	}
	//NSLog(@"Init Accel [1/%d]", animationCount);
	UIAccelerometer* accel;
	
	// Set up the ability to accel
	accel = [UIAccelerometer sharedAccelerometer];
	
	[accel setDelegate:nil];
	[accel setUpdateInterval:(1.0 / (60.0 / (double)animationCount))];
	//[accel setUpdateInterval:(1.0 / 60.f)];
	[accel setDelegate:(self)];
	
	accelCount = 0; // init
}

- (void)layoutSubviews {
    AppLog("layoutSubviews begin");
#if 0
    [EAGLContext setCurrentContext:context];
//  AppFinish();
    AppLog("destroyFramebuffer");
    [self destroyFramebuffer];
    AppLog("createFramebuffer");
    [self createFramebuffer];
	
    AppLog("AppInit");
    NSString* pImgDir = [self getResDir];
    Sonic4_AppInit( backingWidth, backingHeight, [pImgDir cStringUsingEncoding:-2147483647] );

    [self drawView];
#endif // 0
    AppLog("layoutSubviews done");
}

- (void)initEaglDrawable {
    AppLog("initEaglDrawable begin");
    [EAGLContext setCurrentContext:context];
    AppLog("destroyFramebuffer");
    [self destroyFramebuffer];
    AppLog("createFramebuffer");
    [self createFramebuffer];
	
    AppLog("AppInit");
    NSString* pImgDir = [self getResDir];
    Sonic4_AppInit( backingWidth, backingHeight, [pImgDir cStringUsingEncoding:-2147483647] );
	app_init = YES;
	
    [self drawView];
    AppLog("initEaglDrawable done");
}

- (BOOL)checkIgnoreEvent {
	if (ignoreEventCount > 0) {
		return TRUE;
	}
	return FALSE;
}

- (BOOL)createFramebuffer {
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);

    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);

    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);


    glGenRenderbuffersOES(1, &depthRenderbuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
    glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);

    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }
	glEnable(GL_NORMALIZE);
	return YES;
}


- (void)destroyFramebuffer {
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;

    if(depthRenderbuffer) {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}

- (NSString*) copyImageFileToDocDir {
    // パスの取得
    NSArray* pPaths    = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
    NSString *pDocDir  = [pPaths objectAtIndex:0];
    NSString *pTexFile = [pDocDir stringByAppendingPathComponent:@"teapotenv.pvr"];
    
    // テクスチャファイルがドキュメントディレクトリにあるか判定
    NSFileManager* pFileMgr = [NSFileManager defaultManager];
    BOOL           success  = [pFileMgr fileExistsAtPath:pTexFile];
    NSError *error;
    if( !success ){
        NSString *pDefTexPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"teapotenv.pvr"];
        success = [pFileMgr copyItemAtPath:pDefTexPath toPath:pTexFile error:&error];
        if( !success ){
            NSLog([error localizedDescription]);
        }
    }
    return(pDocDir);
}

- (NSString*) getResDir{
    NSString *pDefTexPath = [[NSBundle mainBundle] resourcePath];
    return(pDefTexPath);
}


//------------ Timer Setting method ------------//
- (void)startAnimation {
    self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(drawView) userInfo:nil repeats:YES];
}


- (void)stopAnimation {
	[animationTimer invalidate];
	animationTimer = nil;
}


- (void)setAnimationTimer:(NSTimer *)newTimer {
    [animationTimer invalidate];
    animationTimer = newTimer;
}


- (void)setAnimationInterval:(NSTimeInterval)interval {
    
    animationInterval = interval;
    if (animationTimer) {
        [self stopAnimation];
        [self startAnimation];
    }
}


- (void)dealloc {
#if 0
    [self stopAnimation];
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [context release];
    [super dealloc];
#else
 	[self dispose];
    [super dealloc];
#endif
}


// 入力処理追加
-(void)accelerometer:(UIAccelerometer*)accel
	   didAccelerate:(UIAcceleration*)acceleration
{
	NNS_VECTOR vec;
	vec.x = acceleration.x;
	vec.y = acceleration.y;
	vec.z = acceleration.z;
	amIPhoneAccelerate(&vec);
	
	accelCount = 0; // init 
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	amIPhoneTouchBegan((void*)touches, (void*)event, (void*)self);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	amIPhoneTouchEnded((void*)touches, (void*)event, (void*)self);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	amIPhoneTouchCanceled((void*)touches, (void*)event, (void*)self);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	amIPhoneTouchMoved((void*)touches, (void*)event, (void*)self);
}


///////////////////////////
#pragma mark -
#pragma mark SMSceneProtocol

- (id)initWithFrame:(CGRect)frame
{
	if([super initWithFrame:frame])
	{
		[SMUtil constractor];
		
		self.frame = frame;		
		
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.opacity = 1.0f;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat, nil];
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context]) {
            [self release];
            return nil;
        }
		
		
		// Set up the ability to track multiple touches.
		[self setMultipleTouchEnabled:YES];
		
		// accel
		app_accel = FALSE;
		//[self initAccel];
		
		// sleep
		app_sleep = TRUE;
		_am_sample_is_sleep = TRUE;
		[UIApplication sharedApplication].idleTimerDisabled = NO;
		
		animationCount = _am_sample_count;
		animationInterval = 1.0 / (60.0 / (double)animationCount);
		
		// event ignore
		ignoreEventCount = EAGLVIEW_EVENT_IGNORE_COUNT;
	}
	AppLog("init done");
 	return self;
}

/*! デバイスの回転を対応する命令 */
- (void)layoutForCurrentOrientation:(BOOL)isLandscape :(BOOL)animated
{
	// 対応不明
}

/*! バックグラウンド処理へ移行する時の処理 */
- (void)background
{
    AppLog("WillResign");
	[self stopAnimation];
 	
	if ([self checkIgnoreEvent]) {
		AppLog("Ignore WillResign");
		return;
	}
	// AppMain
	Sonic4_AppSuspend();
	_am_sample_is_suspended = TRUE;
	_am_sample_suspended_count = 20; // 20フレ待ち
	
	// sound
	CriSmpSoundOutput_StopSound();
	
	self.animationInterval = 1.0 / (60.0 / (double)_am_sample_count);
}

/*! バックグラウンドから復帰するときの処理 */
- (void)resume
{
    AppLog("DidBecome");
	[self startAnimation];
	// AppMain
	_am_sample_is_suspended = FALSE;
	
	// Accel
	[self initAccel];
	
	self.animationInterval = 1.0 / (60.0 / (double)_am_sample_count);
}

/*! サウンドの再生を制御するフラグを変更する */
- (void)setSoundFlg:(BOOL)flg
{
}

- (void)dispose
{
    [self stopAnimation];
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
		[context release];
    }
	if (app_init)
	{
		Sonic4_AppFinish();
		app_init = NO;
	}
}

@end
