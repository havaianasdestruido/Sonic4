//
//  MoreGamesDownloader.h
//  FlowCover
//
//  Created by MPP Developer on 9/21/09.
//  Copyright 2009 MPP. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Foundation/NSXMLParser.h>

#import "LogDebugOnly.h"

#import "MoreGamesDiskDataCache.h"


@interface MG_GameDesc : NSObject
{
	NSString* GameID;
	NSString* AppStoreURL;
	NSString* Name;
	NSString* Description;
	NSString* CoverflowImage;
	NSString* BackgroundImage;
	NSString* ScreenshotImage1;
	NSString* ScreenshotImage2;
	NSString* ScreenshotImage3;
	NSString* ScreenshotImageVideo;
	NSString* VideoLink;
	NSString* Version;
	NSString* SupportedDevices;
	NSString* PrimaryCategory;
	NSString* Subcategory;
	NSString* Copyright;
	NSString* Price;
	NSString* Regions;
}

@property (nonatomic, retain) NSString* GameID;
@property (nonatomic, retain) NSString* AppStoreURL;
@property (nonatomic, retain) NSString* Name;
@property (nonatomic, retain) NSString* Description;
@property (nonatomic, retain) NSString* CoverflowImage;
@property (nonatomic, retain) NSString* BackgroundImage;
@property (nonatomic, retain) NSString* ScreenshotImage1;
@property (nonatomic, retain) NSString* ScreenshotImage2;
@property (nonatomic, retain) NSString* ScreenshotImage3;
@property (nonatomic, retain) NSString* ScreenshotImageVideo;
@property (nonatomic, retain) NSString* VideoLink;
@property (nonatomic, retain) NSString* Version;
@property (nonatomic, retain) NSString* SupportedDevices;
@property (nonatomic, retain) NSString* PrimaryCategory;
@property (nonatomic, retain) NSString* Subcategory;
@property (nonatomic, retain) NSString* Copyright;
@property (nonatomic, retain) NSString* Price;
@property (nonatomic, retain) NSString* Regions;


@end


enum DOWNLOAD_STATUS
{
	DLST_ERROR = -1,
	DLST_IDLE = 0,
	DLST_LOADING = 1,
	DLST_COMPLETTED = 2,
};

@protocol MoreGamesDataReceiver

- (bool) dataReceived:(NSData*) data connectionID:(int)CID url:(NSString*)URL; //param: data or nil for error //ret: true - data is valid
- (void) processConnectionError;

@end

#define MG_IMAGE_NA @"mg_image_NA.png"
#define MG_IMAGE_NA_BG @"MoreGames_GameDesc_bg.png"


@interface MoreGamesDownloader : NSObject<MoreGamesDataReceiver> 
{
	
	NSString* xmlURL;
	NSXMLParser* xmlParser;
	NSString* newsString;
	//UIImage* img_NA;
	
	NSMutableArray* arrGameDescs;
	int numberOfGameDescs;
	
	//downloading
	NSDictionary* dictConn;

	
	//internals
	NSString* lastString;
	MG_GameDesc* lastGameDesc;
	int mgXmlErrorLine; //0 - no error
	SEL afterInitDelegateMethod;
	
	//current mg
	int currentGameMG;
	
	//disk cache
	MoreGamesDiskDataCache* diskDataCache;

}

+(void) preinit;
+(bool) isPreinitCompletted;
+(MoreGamesDownloader*) get;
//
-(BOOL)isGameDescEnabledForThisDevice: (MG_GameDesc*) gameDesc;
//for moregames coverflow
-(int)getNumberOfGameDescs; 
-(MG_GameDesc*) getGameDesc: (int) index;
//
-(void) startDownload: (NSString*) url receiver:(id<MoreGamesDataReceiver>) receiver connectionID:(int) CID; 
-(void) cancelAnyDownloadsForReceiver:(id<MoreGamesDataReceiver>) receiver;
//mg selection
-(void)setCurrentMG:(int)iCurrent;
-(int)getCurrentMG;
//from protocol MoreGamesDataReceiver
-(bool) dataReceived:(NSData*) data connectionID:(int)CID url:(NSString*)URL; //param: data or nil for error
//data cache
-(MoreGamesDiskDataCache*)getDiskDataCache;



@end
