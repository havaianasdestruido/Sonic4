//
//  Sonic4_hogAppDelegate.m
//  hog
//
//  Created by USE1264 on 09/07/28.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <AudioToolbox/AudioServices.h>
#import	<CommonCrypto/CommonDigest.h>
#include "cri_mw.h"

#import "Sonic4_hogAppDelegate.h"
#import "Sonic4_EAGLView.h"
#import "Sonic4_AppMain.h"
#import "community.h"

#define USE_ADX   		0
#define USE_CRIAUDIO	1

#define ROUTE_LISTENER_TEST (0)

#if USE_ADX
void ap_usr_func(void *obj);
void ap_adx_err_func(void *obj, char *msg);
void apInit(void);
#endif
static void audioInterruptionListenerCallback(void *inUserData, UInt32 interruptionState);

void setupAudioSessionProperty(void);


@implementation hogAppDelegate

@synthesize window;
@synthesize glView;

// ADMOB Report App Download 

- (NSString *)hashedISU {
	NSString *result = nil;
	NSString *isu = [UIDevice currentDevice].uniqueIdentifier;
	
	if(isu) {
		unsigned char digest[16];
		NSData *data = [isu dataUsingEncoding:NSASCIIStringEncoding];
		CC_MD5([data bytes], [data length], digest);
		
		result = [NSString stringWithFormat: @"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
				  digest[0], digest[1], 
				  digest[2], digest[3],
				  digest[4], digest[5],
				  digest[6], digest[7],
				  digest[8], digest[9],
				  digest[10], digest[11],
				  digest[12], digest[13],
				  digest[14], digest[15]];
		result = [result uppercaseString];
	}
	return result;
}

- (void)reportAppOpenToAdMob {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; // we're in a new thread here, so we need our own autorelease pool
	// Have we already reported an app open?
	NSString *documentsDirectory = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
																		NSUserDomainMask, YES) objectAtIndex:0];
	NSString *appOpenPath = [documentsDirectory stringByAppendingPathComponent:@"admob_app_open"];
	NSFileManager *fileManager = [NSFileManager defaultManager];
	if(![fileManager fileExistsAtPath:appOpenPath]) {
		// Not yet reported -- report now
		NSString *appOpenEndpoint = [NSString stringWithFormat:@"http://a.admob.com/f0?isu=%@&md5=1&app_id=%@",
									 [self hashedISU], @"392788790"];
		NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:appOpenEndpoint]];
		NSURLResponse *response;
		NSError *error = nil;
		NSData *responseData = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
		if((!error) && ([(NSHTTPURLResponse *)response statusCode] == 200) && ([responseData length] > 0)) {
			[fileManager createFileAtPath:appOpenPath contents:nil attributes:nil]; // successful report, mark it as such
    }
  }
  [pool release];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeRight animated:NO]; //qqq
	
	  [self performSelectorInBackground:@selector(reportAppOpenToAdMob) withObject:nil];
		
    AppLog("DidFinish begin");
#ifndef GMD_DEBUG_NO_CREATE_CRIAUDIO
	// Initialize for audio session
	AudioSessionInitialize(NULL, NULL, audioInterruptionListenerCallback, self);
	
	// Setup the AudioSession property and activate the AudioSeesion.
	setupAudioSessionProperty();

	// Initialize for background color
	window.backgroundColor = [UIColor colorWithWhite:0.0f alpha:1.0f];
	window.alpha = 1.0f;
	window.opaque = YES;
	glView.backgroundColor = [UIColor colorWithWhite:0.0f alpha:1.0f];
	glView.alpha = 1.0f;
	glView.opaque = YES;

    // Override point for customization after application launch
//  [window makeKeyAndVisible];
	
	// initialization
//	[self initialize];

#if USE_ADX
	// Setup CRIMW
	apInit();
	
	// Create timer event
	NSTimer* timer;
	timer = [NSTimer scheduledTimerWithTimeInterval:1.0f/60.0f target:self selector:@selector(executeServer:) userInfo:nil repeats:YES];
#endif
#endif // GMD_DEBUG_NOCREATE_CRIAUDIO
	
	
	
	// AppMain
	_am_sample_is_suspended = FALSE;
	_am_sample_suspended_count = 0;
	_am_sample_is_ignore_audio_interruption = FALSE;
	
	glView.animationInterval = 1.0 / (60.0 / (double)_am_sample_count);
	[glView startAnimation];
	
    AppLog("DidFinish done");
	
	[glView initEaglDrawable];
}



- (void)applicationWillResignActive:(UIApplication *)application {
	//NSLog(@"WillResign");
    AppLog("WillResign");
	if ([glView checkIgnoreEvent]) {
		AppLog("Ignore WillResign");
		return;
	}
	// AppMain
	AppSuspend();
	_am_sample_is_suspended = TRUE;
	_am_sample_suspended_count = 20; // 20ÉtÉåë“Çø
	
	// sound
	CriSmpSoundOutput_StopSound();
	
	//UInt32 sessionCategory = kAudioSessionCategory_MediaPlayback;
	//AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
	//AudioSessionSetActive(true);
	
	//glView.animationInterval = 1.0 / 5.0; // íxÇ≠Ç»ÇÈ
	glView.animationInterval = 1.0 / (60.0 / (double)_am_sample_count);
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	//NSLog(@"DidBecome");
    AppLog("DidBecome");
	// AppMain
	_am_sample_is_suspended = FALSE;
	
	// Accel
	[glView initAccel];
	
	glView.animationInterval = 1.0 / (60.0 / (double)_am_sample_count);
}

- (void)applicationDidEnterBackground:(UIApplication *)application {//os4
	AppLog( "applicationDidEnterBackground - os4Å_n" );
	// sound
	//CriSmpSoundOutput_StopSound();	
	[NSThread sleepForTimeInterval:5.55]; //sss - fix a lot of multitask bugs
	
}

- (void)applicationWillEnterForeground:(UIApplication *)application {//os4
	AppLog( "applicationWillEnterForeground - os4Å_n" );
	
	/* does not work
	 
	//check game center connection
	[Community kill];
	///[NSThread sleepForTimeInterval:0.777];
	[[Community get] initCommunity];
	 */
	
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
	//printf("Memory Warning...\n");
}

- (void)dealloc {
	[window release];
	[glView release];
	[super dealloc];
}


#if USE_ADX
/***
 * This function will be called 1/60 sec interval.
 *  CRI Audio server function
 */
- (void)executeServer:(NSTimer*)timer
{
	ADXM_ExecVint(0);
}
#endif

@end

#if USE_ADX
/* Callback from ADX UserMain thread */
void ap_usr_func(void *obj)
{
	ADXM_ExecMain();
//	int i;
//	i++;
}

/* Callback function when an error in ADXT */
void ap_adx_err_func(void *obj, char *msg)
{
	int i;
	i++;
}

void apInit(void)
{
	AdxiPhoneSprmFs sprm;

	memset(&sprm, 0, sizeof(sprm));
	sprm.rtdir=NULL;
	ADXIPHONE_SetupFileSystem(&sprm);
	
	ADXM_SetupFramework(ADXM_FRAMEWORK_DEFAULT, NULL);
	ADXM_SetCbErr(ap_adx_err_func, NULL);
	ADXIPHONE_SetUsrMainFunc(ap_usr_func, NULL);
	
	ADXT_Init();
}
#endif

// CRIÉTÉìÉvÉãÇéQçlÇ…ÇµÇƒÇ¢Ç‹Ç∑
void audioInterruptionListenerCallback(void *inUserData, UInt32 interruptionState)
{
#ifndef GMD_DEBUG_NO_CREATE_CRIAUDIO
	if (!_am_sample_is_ignore_audio_interruption) {
#if (USE_CRIAUDIO) // CriAudio
		if (interruptionState == kAudioSessionBeginInterruption) {
			//NSLog(@"Begin");
			CriSmpSoundOutput_StopSound();
			_am_sample_suspended_count = 25;
		}
#elif (USE_ADX) // CriADX
		if (interruptionState == kAudioSessionBeginInterruption) {
			ADXIPHONE_StopSound();
			_am_sample_suspended_count = 25;
		}
#endif //(USE_CRIAUDIO) // CriAudio
	}
#endif // GMD_DEBUG_NOCREATE_CRIAUDIO
}

#if ROUTE_LISTENER_TEST
void propertyListenerCallback(void *inUserData, AudioSessionPropertyID inPropertyID,
							  UInt32 inPropertyValueSize, const void *inPropertyValue) {
	CFStringRef ref;
	UInt32 size = sizeof(CFStringRef);
	AudioSessionGetProperty(kAudioSessionProperty_AudioRoute, &size, &ref);
	char *routeStr = CFStringGetCStringPtr(ref, CFStringGetSystemEncoding());
	
	NSLog(@"ID %d ROUTE %s \n", inPropertyID, routeStr);
	if (inPropertyID == 'roch') {
		CFDictionaryRef ref = (CFDictionaryRef)inPropertyValue;
		//CFShow(ref);
		
		CFNumberRef num = CFDictionaryGetValue(ref, CFSTR("OutputDeviceDidChange_Reason"));
		if (num) {
			SInt32 temp;
			CFNumberGetValue(num, kCFNumberSInt32Type, &temp);
			NSLog(@"Value %x", temp);
			char log[256];
			memset(log, 9, 256);
			sprintf(log, "Value %x", temp);
			AppLog(log);
		}
	}
}
#endif // ROUTE_LISTENER_TEST

/* Setup the AudioSession property and activate the AudioSeesion. */
void setupAudioSessionProperty(void)
{
#if (USE_CRIAUDIO) // CRIAudio
	UInt32 sessionCategory = kAudioSessionCategory_AmbientSound; //qqq - for iPod enable// kAudioSessionCategory_SoloAmbientSound;
	AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
	AudioSessionSetActive(true);
	
	// Set IO Buffer size
	//Float32 buffersize = 512.0/CRISMP_SOUNDOUTPUT_FREQ;
	Float32 buffersize = 512.0/22050.0;
	UInt32 size = sizeof(buffersize);
	AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, size, &buffersize);
	
#elif (USE_ADX) // CRIADX
	// AudioSession Category
	UInt32 category = kAudioSessionCategory_AmbientSound;
	AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);

	// Activate
	AudioSessionSetActive(true);

	// IO Buffer Size
	//Float32 buffersize = 512.0/CRISMP_SOUNDOUTPUT_FREQ;
	Float32 buffersize = 512.0/22050.0;
	UInt32 size = sizeof(buffersize);
	AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, size, &buffersize);
	
#endif
#if ROUTE_LISTENER_TEST
	AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, propertyListenerCallback, NULL);
#endif // ROUTE_LISTENER_TEST
}