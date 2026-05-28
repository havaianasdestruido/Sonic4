
#import <UIKit/UIKit.h>


#import "SMUtilities.h"
#import "SCSoundManager.h"
#import "MainMenuState.h"

#import "Sonic4_Utility.h"


@interface Sonic4_Utility (hide)

+ (BOOL)isAlertViewEnabled;
+ (void)startAlertView;
+ (void)timeupAlertView;

+ (void)logoEndAction;

@end

@implementation Sonic4_Utility

static Sonic4_Utility* instance = nil;

@synthesize delegate;
@synthesize alertViewFlag;


+ (void)create
{
	if (instance == nil)
	{
		instance = [[Sonic4_Utility alloc] init];
	}
}

+ (void)release
{
	if (instance != nil)
	{
		[instance release];
		instance = nil;
	}
}

+ (Sonic4_Utility*)getInstance
{
	return instance;
}

+ (BOOL)isAlertViewEnabled
{
	return instance.alertViewFlag;
}

+ (void)startAlertView
{
	instance.alertViewFlag = YES;
	[SMUtil showDialog:instance :@""
					  :LOCALISE(@"SP_LOCALIZE_SONIC4LITE_START")
					  :LOCALISE(@"SP_LOCALIZE_OK")
					  :nil];
}

+ (void)timeupAlertView
{
	instance.alertViewFlag = YES;
	[SMUtil showDialog:instance :@""
					  :LOCALISE(@"SP_LOCALIZE_SONIC4LITE_END")
					  :LOCALISE(@"SP_LOCALIZE_OK")
					  :nil];
}


+ (void)setLogoEndAction:(id <Sonic4_UtilityDelegate>)del
{
	instance.delegate = del;
}

+ (void)logoEndAction
{
	if (instance.delegate != nil)
	{
		[instance.delegate logoEndAction];
		instance.delegate = nil;
	}
}


// Called when a button is clicked. The view will be automatically dismissed after this call returns
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	instance.alertViewFlag = NO;
}

// Called when we cancel a view (eg. the user clicks the Home button). This is not called when the user clicks the cancel button.
// If not defined in the delegate, we simulate a click in the cancel button
- (void)alertViewCancel:(UIAlertView *)alertView
{
	instance.alertViewFlag = NO;
}

- (void)willPresentAlertView:(UIAlertView *)alertView  // before animation and showing view
{
}

- (void)didPresentAlertView:(UIAlertView *)alertView  // after animation
{
}

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex // before animation and hiding view
{
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex  // after animation
{
}

@end

////////////////////////////////////////
// C++

static BOOL s_logoDemoFlag    = NO;
static BOOL s_logoDemoEndFlag = NO;


BOOL Sonic4_IsEnabledAlertView()
{
	return [Sonic4_Utility isAlertViewEnabled];
}

void Sonic4_StartAlertView()
{
	[Sonic4_Utility startAlertView];
}

void Sonic4_TimeupAlertView()
{
	[Sonic4_Utility timeupAlertView];
}

BOOL Sonic4_isSoundFlag()
{
	return [MainMenuState isOptionSound];
}

void Sonic4_SetLogoDemoFlag(BOOL flag)
{
	s_logoDemoFlag = flag;
}

BOOL Sonic4_GetLogoDemoFlag()
{
	return s_logoDemoFlag;
}

void Sonic4_SetLogoDemoEnd()
{
	s_logoDemoEndFlag = YES;
}

BOOL Sonic4_GetLogoDemoEnd()
{
	return s_logoDemoEndFlag;
}

void Sonic4_LogoDemoEnd()
{
	[Sonic4_Utility logoEndAction];
	s_logoDemoFlag    = NO;
	s_logoDemoEndFlag = NO;
}

void Sonic4_LogoDemoSoundPlay()
{
	[SCSoundManager playSE:SOUND_SE_SEGALOGO];
}

