//
//  MoreGamesDownloadedDataCache.h
//  iGenesisGamePack
//
//  Created by MPP Developer on 11/11/09.
//  Copyright 2009 MPP. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LogDebugOnly.h"


@interface MoreGamesDiskDataCache : NSObject {

}

-(NSData*)getDataForKey:(NSString*)fullURL checkExpirationDate:(bool)check;//nil if file not present in cache
-(void)storeDataForKey:(NSString*)fullURL data:(NSData*)data;

-(void)maintenance;

+(void)enableCache:(bool)onoff;
+(void)setFileExpirationTimeInHours_Data:(int)timeHrs;
+(void)setFileExpirationTimeInHours_Xml:(int)timeHrs;
+(void)setFileExpirationTimeInHours_Clean:(int)timeHrs;

@end
