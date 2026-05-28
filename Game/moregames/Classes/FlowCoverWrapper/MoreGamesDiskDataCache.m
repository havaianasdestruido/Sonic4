//
//  MoreGamesDownloadedDataCache.m
//  iGenesisGamePack
//
//  Created by MPP Developer on 11/11/09.
//  Copyright 2009 MPP. All rights reserved.
//

#import "MoreGamesDiskDataCache.h"

#define CACHE_EXTENSION_DATA @"sega_mg_cached_data"
#define CACHE_EXTENSION_XML  @"sega_mg_cached_xml"

//test: this value must be 0
#define _DBG_GMT_HOURS_CHNG_ 0

static int CLEAN_EXPIRATION_TIME_in_HOURS = 168; //by default
static int DATA_EXPIRATION_TIME_in_HOURS = 72; //by default
static int XML_EXPIRATION_TIME_in_HOURS = 24; //by default
static bool ENABLE_CACHE = true;


@implementation MoreGamesDiskDataCache

+(void)enableCache:(bool)onoff{
	NSLOG((@"CACHE SETTINGS: enable = %i",onoff));
	ENABLE_CACHE = onoff;
}
+(void)setFileExpirationTimeInHours_Data:(int)timeHrs {
	if(timeHrs>=0) {
		NSLOG((@"CACHE SETTINGS: DATA_EXPIRATION_TIME_in_HOURS = %i", timeHrs));
		DATA_EXPIRATION_TIME_in_HOURS = timeHrs;
	}
}
+(void)setFileExpirationTimeInHours_Xml:(int)timeHrs {
	if(timeHrs>=0) {
		NSLOG((@"CACHE SETTINGS: XML_EXPIRATION_TIME_in_HOURS = %i", timeHrs));
		XML_EXPIRATION_TIME_in_HOURS = timeHrs;
	}
}
+(void)setFileExpirationTimeInHours_Clean:(int)timeHrs {
	if(timeHrs>=0) {
		NSLOG((@"CACHE SETTINGS: CLEAN_EXPIRATION_TIME_in_HOURS = %i", timeHrs));
		CLEAN_EXPIRATION_TIME_in_HOURS = timeHrs;
	}
}


-(NSString*)getCacheDirName
{
	// Convert to OS filename
	NSArray*  paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
	NSString* cacheDirectory = [[NSString alloc] initWithString: [paths objectAtIndex:0]];
	return cacheDirectory;
}

-(NSDate*)getFileModificationDate:(NSString*)fileName
{
	NSDictionary *fileAttributes = [[NSFileManager defaultManager] fileAttributesAtPath:fileName traverseLink:YES];
	NSDate *fileDate = nil;
	//
	if (fileAttributes != nil) {
    fileDate = [fileAttributes objectForKey:NSFileModificationDate];
		NSLOG((@" Modification date: %@\n", fileDate));
	}	
	return fileDate;
}

-(int)getFileSize:(NSString*)fileName
{
	NSDictionary *fileAttributes = [[NSFileManager defaultManager] fileAttributesAtPath:fileName traverseLink:YES];
	//
	if (fileAttributes != nil) {
        NSNumber* fileSize = [fileAttributes objectForKey:NSFileSize];
        int f_size = [fileSize intValue];
		//NSLOG((@" File size: %i\n", f_size));
        return f_size;
	}	
	return 0;
}



-(NSString*)convertUIDtoString:(int)uid
{
	NSString* res = [[NSString alloc] initWithString:@""];
	for(int i=0;i<8;i++)	{
		int d = uid&0xF;
		res = [res stringByAppendingFormat:@"%X",d];
		uid>>=4;
	}
	return res;
}

-(NSString*)convertAnyFileNameToValidString:(NSString*)fname
{
	NSString* res = [[NSString alloc] initWithString:@""];
	if(fname!=nil) {
		for(int i=0;i<[fname length];i++)	{
			int c=[fname characterAtIndex:i];
			bool validChar = (('0'<=c && c<='9') || ('a'<=c && c<='z') || ('A'<=c && c<='Z'));
			if(!validChar) c='_';
			res = [res stringByAppendingFormat:@"%c",c];
		}
	}
	return res;
}


-(NSString*)getUniqueCacheFileName:(NSString*)fullURL
{
	NSString* postfix = [self convertAnyFileNameToValidString:[fullURL lastPathComponent]];
	NSString* extension = CACHE_EXTENSION_DATA;
	if([fullURL hasSuffix:@"xml"]) extension = CACHE_EXTENSION_XML;
	int uid1 = 0x70007000;
	int uid2 = 0x40004000;
	for(int i=0; i<[fullURL length]; i++)
	{
		int c = [fullURL characterAtIndex:i];
		uid1+=c*(i+1)*(12345);
		uid2+=(c<<(i&0x1F))*(1234+i);
	}
	NSString* UID_Str1 = [self convertUIDtoString:uid1];
	NSString* UID_Str2 = [self convertUIDtoString:uid2];
	//
	NSString* result = [@"" stringByAppendingFormat:@"CFILE_%@_%@___%@.%@",UID_Str1,UID_Str2,postfix,extension];
	return result;
}

-(bool)isFileExpired:(NSString*)fileName checkForCleanup:(bool) checkForCleanup
{
	NSDate* dat = [NSDate date];
	const NSTimeInterval curtime = [dat timeIntervalSince1970] ;	
	//
	int expirationTime = CLEAN_EXPIRATION_TIME_in_HOURS;
	if(!checkForCleanup) {
		if([fileName hasSuffix:CACHE_EXTENSION_DATA]) expirationTime = DATA_EXPIRATION_TIME_in_HOURS;
		if([fileName hasSuffix:CACHE_EXTENSION_XML]) expirationTime = XML_EXPIRATION_TIME_in_HOURS;
	}
	//
	NSLOG((@"CACHE: CHECK FOR CLEANUP (%i hrs): file \"%@\"", expirationTime, fileName ));	
	NSDate* filedate = [self getFileModificationDate:fileName];
	if(filedate!=nil) {
		NSTimeInterval  filetime = [filedate timeIntervalSince1970];
		int HRS = (curtime - filetime);
		HRS/=(3600);
		NSLOG((@"  ^---- file stored %i hrs",HRS));
		if(HRS >= expirationTime) {
			return true;
		}				
		return false;
	}
	NSLOG((@"  ^---- file is absent"));
	return false;
}

/*not necessary
-(NSData*) loadFileContent:(NSString*)fileName //try to avoid memory leak problem
{
    NSData* data = nil;
    const int f_size = [self getFileSize:fileName];
    if(f_size>0) {
        char* mem = (char*)malloc(f_size);
        const char* szFileName = [fileName cStringUsingEncoding:NSASCIIStringEncoding];
        FILE* hFile  = fopen( szFileName, "rb" );

        if(hFile!=NULL) {
            fread( mem, 1, f_size, hFile );
            fclose(hFile);
            data = [[NSData alloc] initWithBytes:mem length:f_size];
            NSLOG((@"file load -- ok: len=%i", f_size));
        }
        free(mem);
    }
    return data;
}*/

-(bool)isFileExpired:(NSString*)uniqueFileName
{
	return [self isFileExpired:uniqueFileName checkForCleanup:false];
}

-(NSData*)getDataForKey:(NSString*)fullURL checkExpirationDate:(bool)checkExp 
{
	if(ENABLE_CACHE) {
		NSString* uniqueFileName = 	[[self getCacheDirName] stringByAppendingPathComponent:[self getUniqueCacheFileName:fullURL]];
		NSLOG((@"CACHE: try to load file \"%@\" as \"%@",fullURL,uniqueFileName));
		if(checkExp) {
			if([self isFileExpired:uniqueFileName]==true) {
				NSLOG((@"    ^---- file expired"));
				return nil;
			}
		}

        /*not necessary
		NSData* fileContentData = [self loadFileContent:uniqueFileName];
		NSLOG((@"CACHE: loading result = %@, retainCounter = %i", fileContentData==nil?@"failed":@"OK!", [fileContentData retainCount]));
		return fileContentData;*/
        
        //currently:
		NSData* fileContentData = [[NSData alloc] initWithContentsOfFile:uniqueFileName];
		NSLOG((@"CACHE: loading result = %@, retainCounter = %i", fileContentData==nil?@"failed":@"OK!", [fileContentData retainCount]));
		return fileContentData;
	}
	else {
		return nil;
	}
}
-(void)storeDataForKey:(NSString*)fullURL data:(NSData*)data
{
	if(ENABLE_CACHE) {
		NSString* uniqueFileName = 	[[self getCacheDirName] stringByAppendingPathComponent:[self getUniqueCacheFileName:fullURL]];
		if([[NSFileManager defaultManager] fileExistsAtPath:uniqueFileName]==false || [self isFileExpired:uniqueFileName]) {
			NSLOG((@"CACHE: save new file \"%@\" as \"%@",fullURL,uniqueFileName));
			[data writeToFile:uniqueFileName atomically:YES];
		}
	}
	else{}
}

-(void)maintenance
{
	NSLOG((@"CACHE: maintenance (BEGIN) ... {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{"));
	NSString *path = [self getCacheDirName];
	NSError *error = nil;
	NSArray *array = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:path error:&error];
	if (array == nil) {
    // Handle the error
	}
	else {
		for(int i=0; i<[array count]; i++)
		{
			NSString* fileName = [array objectAtIndex:i];
			const bool isThisMoreGameCacheFile = ([fileName hasSuffix:CACHE_EXTENSION_DATA]) || ([fileName hasSuffix:CACHE_EXTENSION_XML]);
			//
			if(isThisMoreGameCacheFile)
			{
				NSString* fullFileName = [[self getCacheDirName] stringByAppendingPathComponent:fileName];
				if([self isFileExpired: fullFileName checkForCleanup:true]) {
					NSLOG((@"    ^---- file expired and has been removed"));
					[[NSFileManager defaultManager] removeItemAtPath:fullFileName error:NULL];
				}				
			}
		}
	}
	NSLOG((@"CACHE: ... maintenance (END) }}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}} "));
	
}



@end
