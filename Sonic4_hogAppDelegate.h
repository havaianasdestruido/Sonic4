//
//  hogAppDelegate.h
//  hog
//
//  Created by USE1264 on 09/07/28.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class Sonic4_EAGLView;

@interface hogAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow        *window;
    Sonic4_EAGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow        *window;
@property (nonatomic, retain) IBOutlet Sonic4_EAGLView *glView;

@end

