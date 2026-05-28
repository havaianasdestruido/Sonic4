
#import "MoreGamesView.h"
#import "MoreGamesViewController.h"
#import <QuartzCore/QuartzCore.h>
#include "MoreGamesView_meshes.h"

#include "MoreGamesDownloader.h"
#include "MoreGamesLauncher.h"


/************************************************************************/
/*																		*/
/*	Internal Layout Constants											*/
/*																		*/
/************************************************************************/

#define MAX_TEXTURESIZE			256		// width and height of texture; power of 2, 256 max
#define MAXTILES			48		// maximum allocated 256x256 tiles in cache
#define VISTILES			6		// # tiles left and right of center tile visible on screen

/*
 *	Parameters to tweak layout and animation behaviors
 */



/************************************************************************/
/*																		*/
/*	Internal FlowCover Object											*/
/*																		*/
/************************************************************************/

@interface FlowCoverRecordMG : NSObject
{
	GLuint	texture;
}
@property GLuint texture;
- (id)initWithTexture:(GLuint)t;
@end

@implementation FlowCoverRecordMG
@synthesize texture;

- (id)initWithTexture:(GLuint)t
{
	if (nil != (self = [super init])) {
		texture = t;
	}
	return self;
}

- (void)dealloc
{
	if (texture) {
		glDeleteTextures(1,&texture);
	}
	[super dealloc];
}

@end

////////////////////////////////////////////////////////////////////////////////////////

@implementation  ScrollSupportViewMG 

- (BOOL)isAccessibilityElement
{
    return YES;
}

@end


////////////////////////////////////////////////////////////////////////////////////////


@implementation MoreGamesView
@synthesize btnSelectGameMG;
@synthesize scrollSupportViewMG;



/************************************************************************/
/*																		*/
/*	OpenGL ES Support													*/
/*																		*/
/************************************************************************/

+ (Class)layerClass
{
	return [CAEAGLLayer class];
}

- (BOOL)createFrameBuffer
{
	// Create an abstract frame buffer
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);

	// Create a render buffer with color, attach to view and attach to frame buffer
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(id<EAGLDrawable>)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
#if TARGET_IPHONE_SIMULATOR
        NSLOG((@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES)));
#endif
        return NO;
    }
    
    return YES;
}



- (void)destroyFrameBuffer
{
	
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;
    
    if(depthRenderbuffer) {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}

- (void)layoutSubviews
{
    [EAGLContext setCurrentContext:context];
    [self destroyFrameBuffer];
    [self createFrameBuffer];
	[self draw];
}

/************************************************************************/
/*																		*/
/*	Construction/Destruction											*/
/*																		*/
/************************************************************************/

/*	internalInit
 *
 *		Handles the common initialization tasks from the initWithFrame
 *	and initWithCoder routines
 */


- (id)internalInit
{
	CAEAGLLayer *eaglLayer;
	
	eaglLayer = (CAEAGLLayer *)self.layer;
	eaglLayer.opaque = YES;
	
	context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	if (!context || ![EAGLContext setCurrentContext:context] || ![self createFrameBuffer]) {
		[self release];
		return nil;
	}
	self.backgroundColor = [UIColor colorWithWhite:0 alpha:0];
	
	cache = [[DataCache alloc] initWithCapacity:MAXTILES];
	offset = 0;
	
    
    for(int i=0; i<256; i++) Aspects_YonX[i] = DEFAULT_ASPECT_YonX;
	[self startDownloadImages];
	
	return self;
}

- (id)initWithFrame:(CGRect)frame 
{
    if (self = [super initWithFrame:frame]) {
		self = [self internalInit];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)coder 
{
    if (self = [super initWithCoder:coder]) {
		self = [self internalInit];
    }
    return self;
}

- (void)dealloc 
{
	controller = nil;
	
    [EAGLContext setCurrentContext:context];

	[self destroyFrameBuffer];
	[cache release];

	//sss!!!! [EAGLContext setCurrentContext:nil]; //farsh????
    
	[context release];
    context = nil;
	[super dealloc];
}



/************************************************************************/
/*																		*/
/*	nonDelegate Calls														*/
/*																		*/
/************************************************************************/


- (int)numTiles
{
	return [[MoreGamesDownloader get] getNumberOfGameDescs];
}


- (NSString *)flowCoverGameName: (int)index
{
	return [[MoreGamesDownloader get] getGameDesc:index].Name;
}

/*
- (UIImage *)tileImage:(int)index
{
	//return [UIImage imageNamed: mg_img_names[index]];
	return [[MoreGamesDownloader get] getImageByURL: [[MoreGamesDownloader get] getGameDesc:index].CoverflowImage];
}
*/

- (void)touchAtIndex:(int)index
{
	[controller actionGoToMG:nil];
}

-(void) setController:(MoreGamesViewController*)cntrl
{
	controller = cntrl;
}

/************************************************************************/
/*																		*/
/*	Tile Management														*/
/*																		*/
/************************************************************************/

static void *GData = NULL;

- (GLuint)imageToTexture:(UIImage *)image
{
	/*
	 *	Set up off screen drawing
	 */
	
	int TEXTURESIZE = MAX_TEXTURESIZE;
	CGSize size = image.size;
	int maxImgDim = MAX(4, MAX(size.width, size.height));
	
	while(maxImgDim<=TEXTURESIZE/2) {
		TEXTURESIZE/=2;
	}
	
	if (GData == NULL) GData = malloc(4 * TEXTURESIZE * TEXTURESIZE);
	//	void *data = malloc(TEXTURESIZE * TEXTURESIZE * 4);
	CGColorSpaceRef cref = CGColorSpaceCreateDeviceRGB();
	CGContextRef gc = CGBitmapContextCreate(GData,
											TEXTURESIZE,TEXTURESIZE,
											8,TEXTURESIZE*4,
											cref,kCGImageAlphaPremultipliedLast);
	CGColorSpaceRelease(cref);
	UIGraphicsPushContext(gc);
	
	/*
	 *	Set to transparent
	 */
	
	[[UIColor colorWithWhite:0 alpha:0] setFill];
	CGRect r = CGRectMake(0, 0, TEXTURESIZE, TEXTURESIZE);
	UIRectFill(r);
	
	/*
	 *	Draw the image scaled to fit in the texture.
	 */
	
	
	if (size.width > size.height) {
		size.height = TEXTURESIZE * (size.height / size.width);
		size.width = TEXTURESIZE;
	} else {
		size.width = TEXTURESIZE * (size.width / size.height);
		size.height = TEXTURESIZE;
	}
	
	r.origin.x = (TEXTURESIZE - size.width)/2;
	r.origin.y = (TEXTURESIZE - size.height)/2;
	r.size = size;
	[image drawInRect:r];
	
	/*
	 *	Create the texture
	 */
	
	UIGraphicsPopContext();
	CGContextRelease(gc);
	
	GLuint texture = 0;
	glGenTextures(1,&texture);
	[EAGLContext setCurrentContext:context];
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,TEXTURESIZE,TEXTURESIZE,0,GL_RGBA,GL_UNSIGNED_BYTE,GData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	free(GData);
	GData = NULL;
	
	/*
	 *	Done.
	 */
	NSLOG((@"created GL texture: %ix%i",TEXTURESIZE,TEXTURESIZE));
	
	
	return texture;
}


- (FlowCoverRecordMG *)getTileAtIndex:(int)index  //ret: may be nil, if texture downloading is in progress
{
	NSNumber *num = [NSNumber numberWithInt:index];
	FlowCoverRecordMG *fcr = [cache objectForKey:num];
	
	return fcr;
}

#define CONNID0 (0x2200)

-(void) startDownloadImages
{	
	MoreGamesDownloader* mgd = [MoreGamesDownloader get];
	for(int i=0; i<[self numTiles]; i++) {
		if([self getTileAtIndex:i]==nil) {
			NSString* imgURL = [mgd getGameDesc:i].CoverflowImage;
			int CID = i + CONNID0;
			[mgd startDownload:imgURL receiver:self connectionID:CID];
		}	
	}
}



-(bool) dataReceived:(NSData*) data connectionID:(int)CID url:(NSString*)URL //param: data or nil for error
{
	int index = CID - CONNID0;
	NSNumber* num = [NSNumber numberWithInt:index];
	UIImage* img = nil;
	if(data!=nil) {
		img = [[UIImage alloc] initWithData:data];
		if(img == nil) {
			data = [[[MoreGamesDownloader get] getDiskDataCache] getDataForKey:URL checkExpirationDate:false];
			img = [[UIImage alloc] initWithData:data];
            RELEASE_NSDATA(data);
		}        
	}

	if(img == nil) {
		return false;
	}
		
    float aspect = img.size.height/img.size.width;

    Aspects_YonX[index] = aspect;
	GLuint texture = [self imageToTexture: img];
	FlowCoverRecordMG *fcr = [[[FlowCoverRecordMG alloc] initWithTexture:texture] autorelease];
	[cache setObject:fcr forKey:num];
  
    [img release];//bugfix for memory leaks
	
	//update view
	///[self draw];//???
	[self setNeedsDisplay];
	[self setNeedsLayout];
	
	return true;
	
}


- (void)processConnectionError
{	
	[controller closeAndExit: false];
}

////////////////////// button reactions ////////////////////


/************************************************************************/
/*																		*/
/*	Drawing																*/
/*																		*/
/************************************************************************/
/*default:
#define SPREADIMAGE			0.1		// spread between images (screen measured from -1 to 1)
#define FLANKSPREAD			0.4		// flank spread out; this is how much an image moves way from center
#define FRICTION			10.0	// friction
#define MAXSPEED			10.0	// throttle speed to this value
*/
#define SPREADIMAGE			0.19		// spread between images (screen measured from -1 to 1)
#define FLANKSPREAD			0.37		// flank spread out; this is how much an image moves way from center
#define FRICTION			10.0	// friction
#define MAXSPEED			6.0	// throttle speed to this value


#define COPPER_CARTRIGE_SHIFT_Y (0.18)
#define COPPER_CARTRIGE_SHIFT_Z (2.6)
#define COPPER_SHADOW_SHIFT_Y (-1.55)
#define COPPER_CARTRIGE_MIN_ANGLE_Z (M_PI_2*0.7)
#define COPPER_SCALE_COEFF (0.45)
#define COPPER_CAMERA_FOV (40.)
#define COPPER_FLANK_POWER (0.5555)

/************************************************************************/
/*																		*/
/*	Model Constants														*/
/*																		*/
/************************************************************************/



static BOOL prepareRenderData = FALSE;
static GLfloat MG_GReflect[IMG_VERTEX_NUM * 4];

#define NA_TEXTURE_KEY @"*DUMMY*"


- (void)drawTile:(int)index atOffset:(double)off
{
    float AspectYonX = Aspects_YonX[index];

    GLfloat* iMG_Mesh = createMGMesh(AspectYonX);
    GLfloat* iMG_MeshTextureCoords = createMGTexCoords(AspectYonX);
    
	if(!prepareRenderData) {
		
		for(int iv=0; iv<IMG_VERTEX_NUM; iv++) {

			if(iMG_Mesh[iv*3+1] < 0 ) {
				//top
				MG_GReflect[iv*4+0] = 0.75f;
				MG_GReflect[iv*4+1] = 0.75f;
				MG_GReflect[iv*4+2] = 0.75f;
				MG_GReflect[iv*4+3] = 0.80f;			
			}
			else {
				//bot
				MG_GReflect[iv*4+0] = 0.15f;
				MG_GReflect[iv*4+1] = 0.15f;
				MG_GReflect[iv*4+2] = 0.15f;
				MG_GReflect[iv*4+3] = 0.0f;			
			}
			
			//\\//iMG_MeshTextureCoords[iv*2+1] = 1 - iMG_MeshTextureCoords[iv*2+1];
		}
		//prepare empty texture
		GLuint texture = [self imageToTexture: [UIImage imageNamed:@"mg_image_NA.png"]];
		FlowCoverRecordMG *fcr = [[[FlowCoverRecordMG alloc] initWithTexture:texture] autorelease];
		[cache setObject:fcr forKey:NA_TEXTURE_KEY];
		
		prepareRenderData = TRUE;
	}
	
	//3d cartrige
	FlowCoverRecordMG *fcr = [self getTileAtIndex:index];
	if(fcr==nil) {
		fcr = [cache objectForKey:NA_TEXTURE_KEY];
		if(fcr==nil) {
			return;
		}
	}

	GLfloat m[16];
	memset(m,0,sizeof(m));
	m[10] = 1;
	m[15] = 1;
	m[0] = 1;
	m[5] = 1;
	double trans = off * SPREADIMAGE;
	
	double f = off * FLANKSPREAD;
	if (f < -FLANKSPREAD) {
		f = -FLANKSPREAD;
	} else if (f > FLANKSPREAD) {
		f = FLANKSPREAD;
	}
	/*
	m[3] = -f;
	m[0] = 1-fabs(f);
	*/
	double alphaMax = -M_PI_2;
	double alphaMin = alphaMax+COPPER_CARTRIGE_MIN_ANGLE_Z*(f<0?1:-1);
	double alpha = (alphaMax + (alphaMin-alphaMax) * ( pow(fabs(f/FLANKSPREAD),COPPER_FLANK_POWER)));
	double sina = sin(alpha);
	double cosa = cos(alpha);
	m[0]=cosa; m[2]=sina;
	m[8]=-sina; m[10]=cosa;
	
	double sc = COPPER_SCALE_COEFF * (1 - fabs(f));
	trans += f * 1;
	
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glVertexPointer(3,GL_FLOAT,0,iMG_Mesh);
	glTexCoordPointer(2, GL_FLOAT, 0, iMG_MeshTextureCoords);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_DEPTH_TEST);				// using painters algorithm
	glDisable(GL_ALPHA_TEST);
	//glDisable(GL_BLEND);
	
	
	
	//glEnable(GL_CULL_FACE);
	{
		
		glPushMatrix();
		glEnable( GL_TEXTURE_2D );
		
		//mg image 
		//glCullFace(GL_FRONT);
		glTranslatef(trans, COPPER_CARTRIGE_SHIFT_Y, COPPER_CARTRIGE_SHIFT_Z);
		glScalef(sc,sc,sc);
		glMultMatrixf(m);
		{
			glBindTexture(GL_TEXTURE_2D, fcr.texture);
			glDrawArrays(GL_TRIANGLES,0,IMG_VERTEX_NUM);
		}
		glDisableClientState(GL_COLOR_ARRAY);
		

		// reflect	
		glCullFace(GL_BACK);
		glTranslatef(0,COPPER_SHADOW_SHIFT_Y,0);
		glScalef(1,-1,1);
		{
			glColorPointer(4, GL_FLOAT, 0, MG_GReflect);
			glEnableClientState(GL_COLOR_ARRAY);	
		
			glBindTexture(GL_TEXTURE_2D, fcr.texture);	
			glDrawArrays(GL_TRIANGLES,0,IMG_VERTEX_NUM);

		
		}
		glDisableClientState(GL_COLOR_ARRAY);
		
	
		
		//		
		glPopMatrix();		
	}
	
	
	//glDisable(GL_CULL_FACE);
    
    free(iMG_Mesh);
    free(iMG_MeshTextureCoords);

}



-(void)setSelectedItem: (int)index
{
	NSString* newTitle = [[MoreGamesDownloader get] getGameDesc:index].Name;
	
	UIButton* btn = btnSelectGameMG;

	[btn setTitle:newTitle forState:UIControlStateNormal];
	[btn setTitle:newTitle forState:UIControlStateHighlighted];
	[btn setTitle:newTitle forState:UIControlStateDisabled];
	[btn setTitle:newTitle forState:UIControlStateSelected];
}



- (void)draw
{
	/*
	 *	Get the current aspect ratio and initialize the viewport
	 */
	
	double aspect = ((double)backingWidth)/backingHeight;
	
	glViewport(0,0,backingWidth,backingHeight);
	glDisable(GL_DEPTH_TEST);				// using painters algorithm
	
	glClearColor(0,0,0,0);
	
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	/*
	 *	Setup for clear
	 */
	
	[EAGLContext setCurrentContext:context];
	
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glClear(GL_COLOR_BUFFER_BIT);
	
	/*
	 *	Set up the basic coordinate system
	 */
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glScalef(1,aspect,1);
	{{
		GLfloat m[16];
		memset(m,0,sizeof(m));

		//ASSERT( MinDistance>0.0f && MinDistance<MaxDistance);
		
		float MinDistance=0.1f;
		float MaxDistance=10.f;
		float FOV = COPPER_CAMERA_FOV;
		float Focus=1./tan(M_PI*FOV/360.);;
		
		const float DeltaD=MaxDistance-MinDistance;
		const float Q=MaxDistance/DeltaD;
		//
		m[0]=Focus;     m[1]=0.0f;          m[2]=0.0f;            m[3]=0.0f;
		m[4]=0.0f;      m[5]=Focus*aspect;  m[6]=0.0f;            m[7]=0.0f;
		m[8]=0.0f;      m[9]=0.0f;          m[10]=Q;              m[11]=1.0f;
		m[12]=0.0f;     m[13]=0.0f;         m[14]=-Q*MinDistance; m[15]=0.0f;
		
		glMultMatrixf(m);
	}}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	
	int i,len = [self numTiles];
	int mid = (int)floor(offset + 0.5);
	int iStartPos = mid - VISTILES;
	if (iStartPos<0) {
		iStartPos=0;
	}
	
	for (i = iStartPos; i < mid; ++i) {
		[self drawTile:i atOffset:i-offset];
	}
	
	int iEndPos=mid + VISTILES;
	if (iEndPos >= len) {
		iEndPos = len-1;
	}
	for (i = iEndPos; i > mid; --i) {
		[self drawTile:i atOffset:i-offset];
	}
	[self drawTile:mid atOffset:mid-offset];
	
	[self setSelectedItem:mid];
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
	
	[[MoreGamesDownloader get] setCurrentMG:mid];
}

/************************************************************************/
/*																		*/
/*	Animation															*/
/*																		*/
/************************************************************************/

- (void)updateAnimationAtTime:(double)elapsed
{
	int max = [self numTiles] - 1;
	
	if (elapsed > runDelta) elapsed = runDelta;
	double delta = fabs(startSpeed) * elapsed - FRICTION * elapsed * elapsed / 2;
	if (startSpeed < 0) delta = -delta;
	offset = startOff + delta;
	
	if (offset > max) offset = max;
	if (offset < 0) offset = 0;
	
	[self draw];
}

- (void)endAnimation
{
	if (timer) {
		int max = [self numTiles] - 1;
		offset = floor(offset + 0.5);
		if (offset > max) offset = max;
		if (offset < 0) offset = 0;
		[self draw];
		
		[timer invalidate];
		timer = nil;
	}
}

- (void)driveAnimation
{
	double elapsed = CACurrentMediaTime() - startTime;
	if (elapsed >= runDelta) {
		[self endAnimation];
	} else {
		[self updateAnimationAtTime:elapsed];
	}
}

- (void)startAnimation:(double)speed
{
	if (timer) [self endAnimation];
	
	/*
	 *	Adjust speed to make this land on an even location
	 */
	
	//NSLOG((@"speed: %lf",speed));
	double delta = speed * speed / (FRICTION * 2);
	if (speed < 0) delta = -delta;
	double nearest = startOff + delta;
	nearest = floor(nearest + 0.5);
	startSpeed = sqrt(fabs(nearest - startOff) * FRICTION * 2);
	if (nearest < startOff) startSpeed = -startSpeed;
	
	runDelta = fabs(startSpeed / FRICTION);
	startTime = CACurrentMediaTime();
	
	//NSLOG((@"startSpeed: %lf",startSpeed));
	//NSLOG((@"runDelta: %lf",runDelta));
	timer = [NSTimer scheduledTimerWithTimeInterval:0.03
					target:self
					selector:@selector(driveAnimation)
					userInfo:nil
					repeats:YES];
}


/************************************************************************/
/*																		*/
/*	Touch																*/
/*																		*/
/************************************************************************/

#define  MG_DELTA_POS_SCALE (4.5f)

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	CGRect r = self.bounds;
	UITouch *t = [touches anyObject];
	CGPoint where = [t locationInView:self];
	startPos = (where.x / r.size.width) * MG_DELTA_POS_SCALE - MG_DELTA_POS_SCALE/2;;
	startOff = offset;
	
	touchFlag = YES;
	startTouch = where;
	
	startTime = CACurrentMediaTime();
	lastPos = startPos;
	
	[self endAnimation];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	CGRect r = self.bounds;
	UITouch *t = [touches anyObject];
	CGPoint where = [t locationInView:self];
	double pos = (where.x / r.size.width) * MG_DELTA_POS_SCALE - MG_DELTA_POS_SCALE/2;
	
	if (touchFlag == YES) {
		// Touched location; only accept on touching inner 256x256 area
		r.origin.x += (r.size.width - 256)/2;
		r.origin.y += (r.size.height - 256)/2;
		r.size.width = 256;
		r.size.height = 256;
		
		if (CGRectContainsPoint(r, where)) {
			[self touchAtIndex:(int)floor(offset + 0.01)];	// make sure .99 is 1
		}
	} else {
		// Start animation to nearest
		startOff += (startPos - pos);
		offset = startOff;

		double time = CACurrentMediaTime();
		double speed = (lastPos - pos)/(time - startTime);
		if (speed > MAXSPEED) speed = MAXSPEED;
		if (speed < -MAXSPEED) speed = -MAXSPEED;
		
		[self startAnimation:speed];
	}
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	CGRect r = self.bounds;
	UITouch *t = [touches anyObject];
	CGPoint where = [t locationInView:self];
	double pos = (where.x / r.size.width) * MG_DELTA_POS_SCALE - MG_DELTA_POS_SCALE/2;

	if (touchFlag) {
		// determine if the user is dragging or not
		int dx = fabs(where.x - startTouch.x);
		int dy = fabs(where.y - startTouch.y);
		if ((dx < 3) && (dy < 3)) return;
		touchFlag = NO;
	}
	
	int max = [self numTiles]-1;
	
	offset = startOff + (startPos - pos);
	if (offset > max) offset = max;
	if (offset < 0) offset = 0;
	[self draw];
	
	double time = CACurrentMediaTime();
	if (time - startTime > 0.2) {
		startTime = time;
		lastPos = pos;
	}
	
}

- (void) fixIntermittentPosition
{
    [self endAnimation];
	
	startTime=0;
	startOff=0;
	startPos=0;
	startSpeed=0;
	runDelta=0;
	touchFlag=false;
	startTouch.x=0;
	startTouch.y=0;
	lastPos=0;    
    
    int i_offset = offset+0.5;
    offset = i_offset;
    
    [self draw];
}

@end
