//
//  MoreGamesDownloader.m
//
//  Created by MPP Developer on 9/21/09.
//  Copyright 2009 MPP. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MoreGamesDownloader.h"

enum {
	CID_XML_BASIC = 877012,
	CID_XML_LOCAL = 877011,
};

////////////////////////// UIDevice extension /////////////////////// (source: http://devmac.ru/2009/08/26/opredelenie-tipa-ustrojstva/)

#include <sys/types.h>
#include <sys/sysctl.h>


@interface UIDevice(machine)
- (NSString *)machine;
@end

@implementation UIDevice(machine)

- (NSString *)machine
{
	/* return:	 
		iPhone Simulator = i386
		iPhone = iPhone1,1
		3G iPhone = iPhone1,2
		3GS iPhone = iPhone2,1
		1st Gen iPod = iPod1,1
		2nd Gen iPod = iPod2,1
	*/
	
	size_t size;
	
	sysctlbyname("hw.machine", NULL, &size, NULL, 0); 
	
	char *name = (char*)malloc(size);
	
	sysctlbyname("hw.machine", name, &size, NULL, 0);
	
	NSString *machine = [NSString stringWithCString:name];
	
	free(name);
	
	return machine;
}

@end

/////////////////////////////////////////////////////////////////////

@implementation MG_GameDesc


@synthesize GameID;
@synthesize AppStoreURL;
@synthesize Name;
@synthesize Description;
@synthesize CoverflowImage;
@synthesize BackgroundImage;
@synthesize ScreenshotImage1;
@synthesize ScreenshotImage2;
@synthesize ScreenshotImage3;
@synthesize ScreenshotImageVideo;
@synthesize VideoLink;
@synthesize Version;
@synthesize SupportedDevices;
@synthesize PrimaryCategory;
@synthesize Subcategory;
@synthesize Copyright;
@synthesize Price;
@synthesize Regions;



-(MG_GameDesc*) initByDefault
{
	return self;
}


@end;

#define TIMEOUT_INTERVAL (20.0f)
#define MAX_NUMBER_OF_CONNECTIONS (32)

struct CONN_DESC {
	int CID;
	NSURLConnection* connection; // nil - desc is empty
	NSMutableData* resultData;
	NSString* url;
	id<MoreGamesDataReceiver> receiver;
};

static struct CONN_DESC connArray[MAX_NUMBER_OF_CONNECTIONS];



@implementation MoreGamesDownloader

static MoreGamesDownloader* mgd = nil;
static BOOL mgd_isInitialized = FALSE;

+(MoreGamesDownloader*) get
{
	return mgd;
}

+(bool) isPreinitCompletted
{
	return  mgd_isInitialized == TRUE;
}

-(NSString*) getDeviceRegionString
{
	//check if we are us or else
    NSLocale* appLocale   = [NSLocale currentLocale];
    NSString* appLocaleId = [appLocale localeIdentifier];
    NSString* lowercaseAppLocale   = [appLocaleId lowercaseString];
	//([lowercaseAppLocale compare:@"en_us"]==NSOrderedSame)//English in US
	NSString *s=([lowercaseAppLocale hasSuffix:@"_us"]) ?//any language in us
	@"us_" : @"eu_";
	return s;
}

-(NSString*) getDeviceLanguage
{
	//wrong: return [[NSLocale currentLocale] objectForKey:NSLocaleLanguageCode];

	//get 2 letter code
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
	NSString *currentLanguage = [languages objectAtIndex:0];

	currentLanguage = [[self getDeviceRegionString] stringByAppendingString:currentLanguage];
	return currentLanguage;		
}

-(NSString*) getDeviceRegion
{
	return [[NSLocale currentLocale] objectForKey:NSLocaleCountryCode];
}

-(void) preinit:(int)step
{
	NSString* urlNoTail = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"iGenesis_MoregamesXmlUrl_withoutTail"];
	if(urlNoTail==nil) {
		NSLog(@"!!!ERROR!!!: tag 'iGenesis_MoregamesXmlUrl_withoutTail' is absent in your Info.plist file");
		return;
	}
    //\\//urlNoTail = @"http://wrong.address.com/file"; //test
	if(step==0) {
		NSString* xmlURL1 = [[urlNoTail stringByAppendingString: [mgd getDeviceLanguage]] stringByAppendingString:@".xml"];
		NSLOG((@"Try to download XML: %@", xmlURL1));
		[mgd startDownload: xmlURL1 receiver:self connectionID:CID_XML_LOCAL];
	}
	else {
		NSString *s=[self getDeviceRegionString];//us or eu
		s=[s stringByAppendingString:@"en.xml"];//us_en.xml or eu_en.xml
		NSString* xmlURL2 = [urlNoTail stringByAppendingString:s];
		NSLOG((@"Try to download basic XML: %@", xmlURL2));
		[mgd startDownload: xmlURL2 receiver:self connectionID:CID_XML_BASIC];
	}
}


-(BOOL) tryToParseXML:(NSData*) data
{
	numberOfGameDescs = 0;
	mgXmlErrorLine = 0;
	
	xmlParser =[[NSXMLParser alloc] initWithData: data];
	{//start parser
		[xmlParser setDelegate: self];
		bool success = [xmlParser parse];
		if(!success) 
		{
			NSError* error = [xmlParser parserError];
			NSLOG((@"MGD: ********************** XML PARSER ERROR (SEE LINE ABOVE) ***************************"));
			if(error!=nil) {
				NSLOG((@"MGD: ErrorCode=%i    ErrorDomain=%@", [error code], [error domain]));
				NSLOG((@"MGD: ErrorDescription=%@", [error localizedDescription]));
				NSLOG((@"MGD: ErrorFailureReason=%@", [error localizedFailureReason]));
				NSLOG((@"MGD: ErrorRecoverySuggestion=%@", [error localizedRecoverySuggestion]));
			}
			NSLOG((@"MGD: ************************************************************************************"));
			return FALSE;
		}
		if(mgXmlErrorLine>0) 
		{
			NSLOG((@"MGD: ********************** XML PARSER ERROR (SEE LINE: %d) ***************************", mgXmlErrorLine));			
			return FALSE;
		}
	}
	
	NSLOG((@"MGD: device_region=%@, lang=%@", [self getDeviceRegion], [self getDeviceLanguage]));
	
	NSLOG((@"MGD: init -- ok, device is %@", [[UIDevice currentDevice] machine]));		
	
	return TRUE;
	
}

-(bool) dataReceived:(NSData*) data connectionID:(int)CID url:(NSString*)URL
{
	if(data==nil || [self tryToParseXML:data]==false) {
		NSLOG((@"Download - failed"));
		data = [diskDataCache getDataForKey:URL checkExpirationDate: false]; //try to get from cache
		if(data==nil || [self tryToParseXML:data]==false) {
			NSLOG((@"Load from cache - failed"));
			switch(CID) {
				case CID_XML_LOCAL:{
					[self preinit:1];
					return false;
				}break;
				case CID_XML_BASIC:{
					mgd = nil;
					NSLOG((@"------------------ moregames is not initialised"));
				}break;
			}
		}											
	}
	//	
	[diskDataCache maintenance];
	//
	mgd_isInitialized = TRUE;
	//
	return (mgd!=nil);
}

-(void)processConnectionError
{
	//none for XML loader
}

-(void)preinitCache
{
	diskDataCache = [[MoreGamesDiskDataCache alloc] init];
}
		
+(void) preinit
{
	if(!mgd_isInitialized) {
		mgd = [MoreGamesDownloader alloc];
	  //mgd.afterInitDelegateMethod = afterInitDelegateMethod_;
		[mgd preinitCache];
		[mgd preinit:0];
	}
}

/////////////////////// XML PARSER -- BEG ////////////////////////////////////////

#define TAG_SEGA_MOREGAMES_DESCRIPTION_FILE @"SEGA_MOREGAMES_DESCRIPTION_FILE"
#define TAG_SEGA_NEWS @"SEGA_NEWS"
//
#define TAG_LIST_OF_GAMES @"LIST_OF_GAMES"
#define PARAM_NUMBER_OF_GAMES @"NUMBER_OF_GAMES"
//
#define TAG_GAME @"GAME"
#define PARAM_GameID @"GameID"
//
#define TAG_AppStoreURL @"AppStoreURL"
#define TAG_Name @"Name"
#define TAG_Description @"Description"
#define TAG_BackgroundImage @"BackgroundImage"
#define TAG_CoverflowImage @"CoverflowImage"
#define TAG_ScreenshotImage1 @"ScreenshotImage1"
#define TAG_ScreenshotImage2 @"ScreenshotImage2"
#define TAG_ScreenshotImage3 @"ScreenshotImage3"
#define TAG_ScreenshotImageVideo @"ScreenshotImageVideo"
#define TAG_VideoLink @"VideoLink"
#define TAG_Version @"Version"
#define TAG_SupportedDevices @"SupportedDevices"
#define TAG_PrimaryCategory @"PrimaryCategory"
#define TAG_Subcategory @"Subcategory"
#define TAG_Copyright @"Copyright"
#define TAG_Price @"Price"
#define TAG_Regions @"Regions"
//
#define TAG_CacheOn @"CacheOn"
#define TAG_CacheTimeHoursXml @"CacheTimeHoursXml"
#define TAG_CacheTimeHoursData @"CacheTimeHoursData"
#define TAG_CacheCleanTimeHours @"CacheCleanTimeHours"


- (void)parserDidStartDocument:(NSXMLParser *)parser
{
	NSLOG((@"[[[[[[[[[[[[[[[[[[[[[[[[[[[[****** parserDidStartDocument ******]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]"));
}

- (void)parserDidEndDocument:(NSXMLParser *)parser
{
	NSLOG((@"[[[[[[[[[[[[[[[[[[[[[[[[[[[[[****** parserDidEndDocument ******]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]"));
}

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qualifiedName attributes:(NSDictionary *)attributeDict
{
	//NSLOG((@"didStartElem <%@>, nmspURI=%@, qualName=%@, attrNum=%i", elementName, namespaceURI, qualifiedName, [attributeDict count]);
	NSLOG((@"%04d:  TAG BEGIN <%@>", [parser lineNumber], elementName));
	
	if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_LIST_OF_GAMES]){
		NSString* snum = [attributeDict objectForKey: PARAM_NUMBER_OF_GAMES];
		int numItems = [snum intValue];
		arrGameDescs = [[NSMutableArray alloc] initWithCapacity: numItems];
		//
		numberOfGameDescs = 0;
	}
	if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_GAME]){
		lastGameDesc = [[MG_GameDesc alloc] initByDefault];
		NSString* gameID = [attributeDict objectForKey: PARAM_GameID];
		lastGameDesc.GameID = gameID;
	}
}
	


- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
{
	NSLOG((@"%04d:  TAG END </%@>", [parser lineNumber], elementName));
	if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_SEGA_NEWS]){
		newsString = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_AppStoreURL]){
		lastGameDesc.AppStoreURL = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_Name]){
		lastGameDesc.Name = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_Description]){
		lastGameDesc.Description = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_BackgroundImage]){
		lastGameDesc.BackgroundImage = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_CoverflowImage]){
		lastGameDesc.CoverflowImage = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_ScreenshotImage1]){
		lastGameDesc.ScreenshotImage1 = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_ScreenshotImage2]){
		lastGameDesc.ScreenshotImage2 = lastString;	
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_ScreenshotImage3]){
		lastGameDesc.ScreenshotImage3 = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_ScreenshotImageVideo]){
		lastGameDesc.ScreenshotImageVideo = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_VideoLink]){
		lastGameDesc.VideoLink = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_Version]){
		lastGameDesc.Version = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_SupportedDevices]){
		lastGameDesc.SupportedDevices = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_PrimaryCategory]){
		lastGameDesc.PrimaryCategory = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_Subcategory]){
		lastGameDesc.Subcategory = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_Copyright]){
		lastGameDesc.Copyright = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_Price]){
		lastGameDesc.Price = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_Regions]){
		lastGameDesc.Regions = lastString;
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_CacheOn]){
		if(NSOrderedSame == [lastString caseInsensitiveCompare:@"yes"]
			 || NSOrderedSame == [lastString caseInsensitiveCompare:@"on"]
			 || NSOrderedSame == [lastString caseInsensitiveCompare:@"1"])	{
			[MoreGamesDiskDataCache enableCache:true];
		}
		else {
			[MoreGamesDiskDataCache enableCache:false];			
		}
	}
	//cache
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_CacheTimeHoursData]){
		[MoreGamesDiskDataCache setFileExpirationTimeInHours_Data: [lastString intValue]];			
	}	
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_CacheTimeHoursXml]){
		[MoreGamesDiskDataCache setFileExpirationTimeInHours_Xml: [lastString intValue]];			
	}	
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_CacheCleanTimeHours]){
		[MoreGamesDiskDataCache setFileExpirationTimeInHours_Clean: [lastString intValue]];			
	}	
	//
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_GAME]){
		if([self isGameDescEnabledForThisDevice: lastGameDesc]) {
			[arrGameDescs addObject: lastGameDesc];
			lastGameDesc = nil;
			//
			numberOfGameDescs++;
		}
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_LIST_OF_GAMES]){
	  //none	
	}
	else if(NSOrderedSame == [elementName caseInsensitiveCompare: TAG_SEGA_MOREGAMES_DESCRIPTION_FILE]){
		//none
	}
	else 
	{
		NSLOG((@"MGD: ********************** XML PARSER ERROR (SEE ABOVE LINE) ***************************"));
		NSLOG((@"Error: this tag is unknown"));
		NSLOG((@"MGD: ************************************************************************************"));
		mgXmlErrorLine = [parser lineNumber];
	}
	
	lastString = @"";
}

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string
{
	NSCharacterSet* charset1 = [NSCharacterSet characterSetWithCharactersInString:@"\t\n\r "];
	NSCharacterSet* charset2 = [NSCharacterSet characterSetWithCharactersInString:@"\""];
	string = [string stringByTrimmingCharactersInSet: charset1];
	string = [string stringByTrimmingCharactersInSet: charset2];
	if(string!=nil && [string length]>0) {
		if(lastString==nil) lastString=@"";
		lastString = [lastString  stringByAppendingString:string]; //xml parser bugfix: dividing string by &,',<,>,"
		NSLOG((@"%04d:      DATA: %@", [parser lineNumber], lastString));
	}
}

- (void)parser:(NSXMLParser *)parser foundCDATA:(NSData *)CDATABlock
{
	NSString* string = [[NSString alloc] initWithData: CDATABlock encoding: NSUTF8StringEncoding];
	if(string!=nil && [string length]>0) {
		lastString = [lastString  stringByAppendingString:string];
		NSLOG((@"%04d:      [[CDATA]]: %@", [parser lineNumber], lastString));
	}
}

/////////////////////// XML PARSER -- END ////////////////////////////////////////

#define PSEUDO_DEVICE_FOR_EMULATOR @"all"
//#define PSEUDO_DEVICE_FOR_EMULATOR @"iPhone3G"
//#define PSEUDO_DEVICE_FOR_EMULATOR @"iPod1stGen"

-(BOOL)isDeviceCompatibled: (MG_GameDesc*) gameDesc
{
	NSString* devList = gameDesc.SupportedDevices;
	
	if(devList==nil || [devList length]==0)
		return TRUE;
	
	NSString* machine = [[UIDevice currentDevice] machine];
	NSString* devXmlName = @"all";
	
	if(NSOrderedSame == [machine caseInsensitiveCompare: @"i386"]) {
		devXmlName = PSEUDO_DEVICE_FOR_EMULATOR;
	}
	else if(NSOrderedSame == [machine caseInsensitiveCompare: @"iPhone1,1"]) {
		devXmlName = @"iPhone";
	}
	else if(NSOrderedSame == [machine caseInsensitiveCompare: @"iPhone1,2"]) {
		devXmlName = @"iPhone3G";
	}
	else  if(NSOrderedSame == [machine caseInsensitiveCompare: @"iPhone2,1"]) {
		devXmlName = @"iPhone3GS";
	}
	else  if(NSOrderedSame == [machine caseInsensitiveCompare: @"iPod1,1"]) {
		devXmlName = @"iPod1stGen";

	}
	else  if(NSOrderedSame == [machine caseInsensitiveCompare: @"iPod2,1"]) {
		devXmlName = @"iPod2ndGen";
	}
	//
	if(NSOrderedSame == [devXmlName caseInsensitiveCompare: @"all"]) {
		return TRUE;
	}
	
	if([devList rangeOfString:devXmlName options:NSCaseInsensitiveSearch].location!=NSNotFound)
		return TRUE;
	
	if([devList rangeOfString:@"all" options:NSCaseInsensitiveSearch].location!=NSNotFound)
		return TRUE;
	
	return FALSE;
}

-(BOOL)isRegionCompatibled: (MG_GameDesc*) gameDesc
{
	NSString* devReg = [self getDeviceRegion];
	NSString* listOfRegions = gameDesc.Regions;
	
	if(listOfRegions==nil || [listOfRegions length]==0)
		return TRUE;	
	
	if([listOfRegions rangeOfString:devReg options:NSCaseInsensitiveSearch].location!=NSNotFound)
		return TRUE;
	
	if([listOfRegions rangeOfString:@"all" options:NSCaseInsensitiveSearch].location!=NSNotFound)
		return TRUE;
	
	return FALSE;
}

-(BOOL)isGameDescEnabledForThisDevice: (MG_GameDesc*) gameDesc
{
	return [self isDeviceCompatibled:gameDesc] && [self isRegionCompatibled:gameDesc] ;
}

-(int)getNumberOfGameDescs {
	return numberOfGameDescs;
}

-(MG_GameDesc*) getGameDesc: (int) index {
	return [arrGameDescs objectAtIndex:index];
}

/////////////// downloading //////////////////


-(void)startDownload: (NSString*) url receiver:(id<MoreGamesDataReceiver>) receiver connectionID:(int) CID; //return connection id
{
	//\\//static int jj=0; if(((++jj)&0x1F)==4) url=@"wrong_url"; //test
	NSData* data = [diskDataCache getDataForKey:url checkExpirationDate: true]; //try to get from cache (if not expired)
	if(data!=nil) {
		[receiver dataReceived: data connectionID:CID url:url];
        RELEASE_NSDATA(data);
		return;
	}
	
	NSLOG((@"startDownload: url=%@", url));

	NSURLRequest *theRequest=[NSURLRequest requestWithURL:[NSURL URLWithString: url]
										cachePolicy:NSURLRequestReturnCacheDataElseLoad // - with cache
											//cachePolicy:NSURLRequestReloadIgnoringCacheData // - ignore cahce
											timeoutInterval:TIMEOUT_INTERVAL];
	// create the connection with the request
	// and start loading the data
	NSURLConnection* theConnection=[[NSURLConnection alloc] initWithRequest:theRequest delegate:self];
	if (theConnection) {
		for(int i=0; i<MAX_NUMBER_OF_CONNECTIONS; i++) {
			if(connArray[i].connection==nil) {
				connArray[i].connection = theConnection;
				connArray[i].CID = CID;
				connArray[i].url = url;
				connArray[i].receiver = receiver;
				connArray[i].resultData = [[NSMutableData data] retain];
				[connArray[i].resultData setLength:0];
				return;
			}
		}
	}
	[receiver dataReceived: nil connectionID: CID url:nil];
	NSLOG((@"...failed or no empty connections"));
	[receiver processConnectionError];	
}

-(void) cancelAnyDownloadsForReceiver:(id<MoreGamesDataReceiver>) receiver 
{
	for(int i=0; i<MAX_NUMBER_OF_CONNECTIONS; i++) {
        NSURLConnection* theConnection=connArray[i].connection;
		if(theConnection!=nil) {
            id<MoreGamesDataReceiver> theReceiver = connArray[i].receiver;
            if(receiver==theReceiver) {
                [theConnection cancel];
                NSLog((@"............ cancel connection: cid=%i"), connArray[i].CID);                     
                [connArray[i].resultData release];
                connArray[i].resultData = nil;
                connArray[i].connection = nil;
                connArray[i].CID = 0;
				connArray[i].url = nil;
				connArray[i].receiver = nil;                
                [theConnection release];
            }
		}
	}    
}

-(void) downloadDataIncrementally: (NSURLConnection *)connection data:(NSData *)data
{
	for(int i=0; i<MAX_NUMBER_OF_CONNECTIONS; i++) {
		if(connArray[i].connection!=nil) {
			if([connArray[i].connection isEqual: connection]) {
//				int CID = connArray[i].CID;
				[connArray[i].resultData appendData:data];
				NSLOG((@"downloadDataIncrementally: CID=%i total=%db", CID, [connArray[i].resultData length]));
			}
		}
	}
}

-(void) downloadCompletted: (NSURLConnection *)connection success:(BOOL)success
{
	for(int i=0; i<MAX_NUMBER_OF_CONNECTIONS; i++) {
		if(connArray[i].connection!=nil) {
			if([connArray[i].connection isEqual: connection]) {
				int CID = connArray[i].CID;
                //\\//\\//\\if(CID>0x2F00) break;//test
				id<MoreGamesDataReceiver> receiver = connArray[i].receiver;
				NSData* data = nil;
				if(success==YES) {
					data = connArray[i].resultData;
				}
				NSLOG((@"downloadCompletted: CID=%i total=%db success=%i", CID, [connArray[i].resultData length], success));
				bool dataIsValid = [receiver dataReceived: data connectionID: CID url:connArray[i].url]; //send data to receiver
				if(dataIsValid) {
					[diskDataCache storeDataForKey:connArray[i].url data:data];//store valid data into cache
				}
				[connArray[i].resultData release];
				connArray[i].resultData = nil;
				connArray[i].connection = nil;
                connArray[i].CID = 0;
				connArray[i].url = nil;
				connArray[i].receiver = nil;                
				[connection release];
				
				//if(!dataIsValid || CID==0x3F00) {//test!!
				if(!dataIsValid) {
					NSLOG((@"...connection error or wrong data"));
					[receiver processConnectionError];
				}
			}
		}
	}
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
	NSLOG((@"connection didReceiveData: len=%i", [data length]));
	[self downloadDataIncrementally:connection data:data];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
	[self downloadCompletted:connection success:YES];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	NSLOG((@"connection didFailWithError: err=%@", [error localizedDescription]));
	[self downloadCompletted:connection success:NO];
}

-(MoreGamesDiskDataCache*)getDiskDataCache
{
	return diskDataCache;
}

////////////////////////////////////////selection
-(void)setCurrentMG:(int)iCurrent
{
	currentGameMG = iCurrent;
}
-(int)getCurrentMG
{
	return currentGameMG;
}

@end
