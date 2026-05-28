/*****************************************************************************/
/*      amIPhone.cpp                   Author : Syuichi Gotou                */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* iPhone関連ライブラリプログラム                                            */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090629-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "../alice.h"

/*--- Macros ----------------------------------------------------------------*/

#define PVRTOOLS_DEBUG_OUTPUT( x )		NNM_TRACE( x )

#define PVRT_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define PVRT_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define PVRT_CLAMP(x, l, h)      (PVRT_MIN((h), PVRT_MAX((x), (l))))

//#define FREE(X)		{ if(X) { free(X); (X) = 0; } }

/*--- Definitions -----------------------------------------------------------*/

/*!***************************************************************************
 Describes the header of a PVR header-texture
 *****************************************************************************/
typedef struct PVR_Header_Texture_TAG
{
	unsigned int dwHeaderSize;			/*!< size of the structure */
	unsigned int dwHeight;				/*!< height of surface to be created */
	unsigned int dwWidth;				/*!< width of input surface */
	unsigned int dwMipMapCount;			/*!< number of mip-map levels requested */
	unsigned int dwpfFlags;				/*!< pixel format flags */
	unsigned int dwTextureDataSize;		/*!< Total size in bytes */
	unsigned int dwBitCount;			/*!< number of bits per pixel  */
	unsigned int dwRBitMask;			/*!< mask for red bit */
	unsigned int dwGBitMask;			/*!< mask for green bits */
	unsigned int dwBBitMask;			/*!< mask for blue bits */
	unsigned int dwAlphaBitMask;		/*!< mask for alpha channel */
	unsigned int dwPVR;					/*!< magic number identifying pvr file */
	unsigned int dwNumSurfs;			/*!< the number of surfaces present in the pvr */
} PVR_Texture_Header;

typedef enum PixelType_TAG
{
	MGLPT_ARGB_4444 = 0x00,
	MGLPT_ARGB_1555,
	MGLPT_RGB_565,
	MGLPT_RGB_555,
	MGLPT_RGB_888,
	MGLPT_ARGB_8888,
	MGLPT_ARGB_8332,
	MGLPT_I_8,
	MGLPT_AI_88,
	MGLPT_1_BPP,
	MGLPT_VY1UY0,
	MGLPT_Y1VY0U,
	MGLPT_PVRTC2,
	MGLPT_PVRTC4,
	MGLPT_PVRTC2_2,
	MGLPT_PVRTC2_4,
		
	OGL_RGBA_4444= 0x10,
	OGL_RGBA_5551,
	OGL_RGBA_8888,
	OGL_RGB_565,
	OGL_RGB_555,
	OGL_RGB_888,
	OGL_I_8,
	OGL_AI_88,
	OGL_PVRTC2,
	OGL_PVRTC4,
		
	// OGL_BGRA_8888 extension
	OGL_BGRA_8888,
		
	D3D_DXT1 = 0x20,
	D3D_DXT2,
	D3D_DXT3,
	D3D_DXT4,
	D3D_DXT5,
		
	D3D_RGB_332,
	D3D_AI_44,
	D3D_LVU_655,
	D3D_XLVU_8888,
	D3D_QWVU_8888,
		
	//10 bits per channel
	D3D_ABGR_2101010,
	D3D_ARGB_2101010,
	D3D_AWVU_2101010,
		
	//16 bits per channel
	D3D_GR_1616,
	D3D_VU_1616,
	D3D_ABGR_16161616,
		
	//HDR formats
	D3D_R16F,
	D3D_GR_1616F,
	D3D_ABGR_16161616F,
		
	//32 bits per channel
	D3D_R32F,
	D3D_GR_3232F,
	D3D_ABGR_32323232F,
		
	// Ericsson
	ETC_RGB_4BPP,
	ETC_RGBA_EXPLICIT,
	ETC_RGBA_INTERPOLATED,
		
	MGLPT_NOTYPE = 0xff
		
} PixelType;

const unsigned int PVRTEX_MIPMAP		= (1<<8);		// has mip map levels
const unsigned int PVRTEX_TWIDDLE		= (1<<9);		// is twiddled
const unsigned int PVRTEX_BUMPMAP		= (1<<10);		// has normals encoded for a bump map
const unsigned int PVRTEX_TILING		= (1<<11);		// is bordered for tiled pvr
const unsigned int PVRTEX_CUBEMAP		= (1<<12);		// is a cubemap/skybox
const unsigned int PVRTEX_FALSEMIPCOL	= (1<<13);		//
const unsigned int PVRTEX_VOLUME		= (1<<14);
const unsigned int PVRTEX_PIXELTYPE		= 0xff;			// pixel type is always in the last 16bits of the flags
const unsigned int PVRTEX_IDENTIFIER	= 0x21525650;	// the pvr identifier is the characters 'P','V','R'

const unsigned int PVRTEX_V1_HEADER_SIZE = 44;			// old header size was 44 for identification purposes

const unsigned int PVRTC2_MIN_TEXWIDTH		= 16;
const unsigned int PVRTC2_MIN_TEXHEIGHT		= 8;
const unsigned int PVRTC4_MIN_TEXWIDTH		= 8;
const unsigned int PVRTC4_MIN_TEXHEIGHT		= 8;
const unsigned int ETC_MIN_TEXWIDTH			= 4;
const unsigned int ETC_MIN_TEXHEIGHT		= 4;


/*
 GL_IMG_texture_compression_pvrtc
 */
/* Tokens */
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG			0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG			0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG			0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG			0x8C03

/*
 GL_IMG_texture_format_BGRA8888 
 */
/* Tokens */
#define GL_BGRA										0x80E1




/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

BOOL		_am_iPhone_quit_req = FALSE;
BOOL		_am_iPhone_quit = FALSE;

#if AMD_DEBUG && AMD_DEBUG_FRAME_RATE
Sint32	_am_main_counter = 0;
#endif


/*--- Local Variables -------------------------------------------------------*/


/*--- Local Functions -------------------------------------------------------*/

static NNE_BOOL IsGLExtensionSupported(const char *extension);

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* void amIPhoneInitNN(int Width, int Height, const char* pDocPath)          */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  初期化                                                         */
/*****************************************************************************/
void amIPhoneInitNN(int Width, int Height)
{
    NNS_CONFIG_GL nnconfig;
	NNS_MATRIX44 projmtx;
	
	/* Globals */
	NNS_LIGHT_PARALLEL			gLight;		/* ライト */
	NNS_VECTOR					gLightDir = { 0.f, 0.f, -1.f };	/* XYZ */
	NNS_RGBA					gLightCol = { 1.f, 1.f, 1.f, 1.f };	/* RGBA */
	Float						gLightInten = 1.f;	/* Intensity */
	
    // NNライブラリの初期化
	memset(&nnconfig, 0, sizeof(nnconfig));
	nnconfig.WindowWidth	= Width;
	nnconfig.WindowHeight	= Height;
	nnConfigureSystemGL(&nnconfig);
	
	// プロジェクション設定
	// 視野角45度 アスペクト比480:320 Near 1.f Far 10000.f
	nnMakePerspectiveMatrix( &projmtx, NNM_DEGtoA32( 45.f ), (float)Width/(float)Height, 1.f, 10000.f );
	
	// これを呼んでいないと _nngClipPlane が無いと言われビルドできない
	nnSetProjection( (const NNS_MATRIX44*)&projmtx, NNE_PROJECTION_TYPE_PERSPECTIVE );
	
	_am_draw_video.draw_aspect		= (float)Width / (float)Height;
	
	// ここで縦横が入れ替わる可能性あり
	amIPhoneInitBase(&Width, &Height);
	
	_am_draw_video.draw_width		= Width;
	_am_draw_video.draw_height		= Height;
//	_am_draw_video.draw_format		= format;
//	_am_draw_video.draw_format_z	= CELL_GCM_SURFACE_Z24S8;
	
//	_am_draw_video.disp_width		= nnconfig.WindowWidth;
//	_am_draw_video.disp_width_2d	= nnconfig.WindowWidth;
//	_am_draw_video.disp_height		= nnconfig.WindowHeight;
	_am_draw_video.disp_width		= Width;
	_am_draw_video.disp_height		= Height;

#if 1
	// iPhone実解像度で2D描画
	_am_draw_video.width_2d			= Width;
	_am_draw_video.height_2d		= Height;
	_am_draw_video.scale_x_2d		= 1.0f;
	_am_draw_video.scale_y_2d		= 1.0f;
	_am_draw_video.base_x_2d		= 0.0f;
	_am_draw_video.base_y_2d		= 0.0f;
#else
	// 他機種と同じ仮想解像度1280x720で2D描画（サイドカット）
	_am_draw_video.width_2d			= 1280.0f;
	_am_draw_video.height_2d		= 720.0f;
	float	vw = _am_draw_video.draw_aspect * _am_draw_video.height_2d;
	_am_draw_video.scale_x_2d		= _am_draw_video.draw_width / vw;
	_am_draw_video.scale_y_2d		= _am_draw_video.draw_height
									/ _am_draw_video.height_2d;
	_am_draw_video.base_x_2d		= (vw - _am_draw_video.width_2d) * 0.5f
									* _am_draw_video.scale_x_2d;
	_am_draw_video.base_y_2d		= 0.0f;
#endif
	
	_am_draw_video.wide_screen		= FALSE;
	
	_am_draw_video.refresh_rate		= 60.0f;
	
	amSystemSetFrameRateDraw(60.0f / floor(_am_draw_video.refresh_rate + 0.5f));
	amSystemSetFrameRateMain(amSystemGetFrameRateDraw());
	
	amRenderInit();
	
}


/*****************************************************************************/
/* void amIPhoneExitNN(void)                                                 */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  終了                                                          */
/*****************************************************************************/
void amIPhoneExitNN(void)
{
	
}


/*****************************************************************************/
/* int amIPhoneMainLoop(void)                                                */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  メインループ                                                  */
/* [RETURN] 0 : 正常 /  1 : 異常あり                                         */
/*****************************************************************************/
int amIPhoneMainLoop(void)
{
	int		result = 0;

#if !AMD_USE_DRAW_THREAD
	// シングルスレッド
#if AMD_DEBUG
	AMS_TIMER	timer;
	amTimerCreate(&timer);
#if AMD_DEBUG_FRAME_RATE
	amTimerStart(&timer);
#endif
#endif

	// 終了チェック
	if (_amIPhoneCheckExit()) {
		return 1;
	}
	
	amDrawBeginScene();
	if (amDrawBegin()) {
		// メイン
#if AMD_DEBUG && !AMD_DEBUG_FRAME_RATE
		amTimerStart(&timer);
#endif
		amPadGetData();
		amTpExecute();
		
		amFsServer();
		amTaskExecute();
		amDrawCloseDisplayList();
		amDrawGetDisplayList();
		amMemDisplayInfo(AMD_TEXT_WIDTH - 22, 2);
#if AMD_DEBUG && !AMD_DEBUG_FRAME_RATE
		amTimerEnd(&timer);
		amDebugSetPerformanceMain(timer.frame);
#endif
		// 描画
		amDrawExecRegist();
		amDrawExecCommand(AMD_COMMAND_STATE_MAKE_TASK);
		amDrawExecute();
		amDrawDisplay();
		amDrawExecCommand(AMD_COMMAND_STATE_DEBUG);
#if AMD_DEBUG && !AMD_DEBUG_FRAME_RATE
		amTimerEnd(&timer);
		amDebugSetPerformanceDraw(timer.frame);
#endif
		amDebugDisplayPerformance(AMD_TEXT_WIDTH - 17, 1);
		nnFlushPrint();
		amDrawEnd();
#if AMD_DEBUG && AMD_DEBUG_FRAME_RATE
		amTimerEnd(&timer);
		amDebugSetPerformanceMain(timer.frame);
		amDebugSetPerformanceDraw(timer.frame);
#endif
	}
	
#if AMD_DEBUG
	amTimerDelete(&timer);
#endif

	amDrawWaitVSync();
#else
	// マルチスレッド
#if AMD_DEBUG
	static AMS_TIMER	timer;
	static BOOL			timerInit = FALSE;
	if (!timerInit) {
		amTimerCreate(&timer);
		timerInit		= TRUE;
#if AMD_DEBUG_FRAME_RATE
		amTimerStart(&timer);
#endif
	}
#endif

	// タイマー待ち
	amAlarmWaitVSync(&_am_main_timer);
//	amAlarmSetTimer(&_am_main_timer,
//			(Uint32)(amSystemGetFrameRateMain() * 1000000.0f / 60.0f));

#if AMD_DEBUG
#if AMD_DEBUG_FRAME_RATE
	amTimerEnd(&timer);
	amDebugSetPerformanceMain(timer.frame);
#else
	amTimerStart(&timer);
#endif
#endif

	amDrawCloseDisplayList();

	amPadGetData();
	amTpExecute();

	amFsServer();
	amFsDebugPrint();

	amTaskExecute(_am_default_taskp);
	amMemDisplayInfo(AMD_TEXT_WIDTH - 22, 3);

//	amAlarmUpdateTimer(&_am_main_timer);

#if AMD_DEBUG
#if !AMD_DEBUG_FRAME_RATE
	amTimerEnd(&timer);
	amDebugSetPerformanceMain(timer.frame);
#else
	amPrintf(2, 1, "%4d", _am_main_counter++);
#endif
#endif
#endif

	return	result;
}

/*!***************************************************************************
 @Function		amIPhonePVRTLoadPartialTextureFromPointer
 @Input			pointer			Pointer to header-texture's structure
 @Input			texPtr			If null, texture follows header, else texture is here.
 @Input			nLoadFromLevel	Which mipmap level to start loading from (0=all)
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.
 @Return		flags from texture on success, 0 on failure
 @Description	Allows textures to be stored in C header files and loaded in.  Can load parts of a
				mipmaped texture (ie skipping the highest detailed levels).  Release texture by calling
				PVRTReleaseTexture. This function is the copy of PVRTLoadPartialTextureFromPointer, hence
				when PVRTLoadPartialTextureFromPointer is changed, this function is changed too.
				In OpenGL Cube Map, each texture's up direction is defined as next (view direction, up direction),
				(+x,-y)(-x,-y)(+y,+z)(-y,-z)(+z,-y)(-z,-y).
*****************************************************************************/
unsigned int amIPhonePVRTLoadPartialTextureFromPointer(const void * const pointer,
											   const void * const texPtr,
											   const unsigned int nLoadFromLevel,
											   GLuint * const texName,
											   const void *psTextureHeader)
{
	PVR_Texture_Header* psPVRHeader = (PVR_Texture_Header*)pointer;
	unsigned int u32NumSurfs;

	GLuint textureName;
	GLenum textureFormat = 0;
	GLenum textureType = GL_RGB;

	NNE_BOOL IsPVRTCSupported = IsGLExtensionSupported("GL_IMG_texture_compression_pvrtc");
	NNE_BOOL IsBGRA8888Supported  = IsGLExtensionSupported("GL_IMG_texture_format_BGRA8888");

	NNE_BOOL IsCompressedFormatSupported = NNE_FALSE, IsCompressedFormat = NNE_FALSE;

	unsigned int i;

	*texName = 0;	// install warning value

	// perform checks for old PVR psPVRHeader
	if(psPVRHeader->dwHeaderSize!=sizeof(PVR_Texture_Header))
	{	// Header V1
		if(psPVRHeader->dwHeaderSize==PVRTEX_V1_HEADER_SIZE)
		{	// react to old psPVRHeader: i.e. fill in numsurfs as this is missing from old header
			PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer warning: this is an old pvr"
				" - you can use PVRTexTool to update its header.\n");
			if(psPVRHeader->dwpfFlags&PVRTEX_CUBEMAP)
				u32NumSurfs = 6;
			else
				u32NumSurfs = 1;
		}
		else
		{	// not a pvr at all
			PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: not a valid pvr.\n");
			return 0;
		}
	}
	else
	{	// Header V2
		if(psPVRHeader->dwNumSurfs<1)
		{	// encoded with old version of PVRTexTool before zero numsurfs bug found.
			if(psPVRHeader->dwpfFlags & PVRTEX_CUBEMAP)
				u32NumSurfs = 6;
			else
				u32NumSurfs = 1;
		}
		else
		{
			u32NumSurfs = psPVRHeader->dwNumSurfs;
		}
	}

	/* Only accept untwiddled data UNLESS texture format is PVRTC */
	if ( ((psPVRHeader->dwpfFlags & PVRTEX_TWIDDLE) == PVRTEX_TWIDDLE)
		&& ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)!=OGL_PVRTC2)
		&& ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)!=OGL_PVRTC4) )
	{
		// We need to load untwiddled textures -- hw will twiddle for us.
		PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: texture should be untwiddled.\n");
		return 0;
	}

	switch(psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)
	{
	case OGL_RGBA_4444:
		textureFormat = GL_UNSIGNED_SHORT_4_4_4_4;
		textureType = GL_RGBA;
		break;

	case OGL_RGBA_5551:
		textureFormat = GL_UNSIGNED_SHORT_5_5_5_1;
		textureType = GL_RGBA;
		break;

	case OGL_RGBA_8888:
		textureFormat = GL_UNSIGNED_BYTE;
		textureType = GL_RGBA;
		break;

	/* New OGL Specific Formats Added */

	case OGL_RGB_565:
		textureFormat = GL_UNSIGNED_SHORT_5_6_5;
		textureType = GL_RGB;
		break;

	case OGL_RGB_555:
		PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: pixel type OGL_RGB_555 not supported.\n");
		return 0; // Deal with exceptional case

	case OGL_RGB_888:
		textureFormat = GL_UNSIGNED_BYTE;
		textureType = GL_RGB;
		break;

	case OGL_I_8:
		textureFormat = GL_UNSIGNED_BYTE;
		textureType = GL_LUMINANCE;
		break;

	case OGL_AI_88:
		textureFormat = GL_UNSIGNED_BYTE;
		textureType = GL_LUMINANCE_ALPHA;
		break;

	case OGL_PVRTC2:
		if(IsPVRTCSupported)
		{
			IsCompressedFormatSupported = IsCompressedFormat = NNE_TRUE;
			textureFormat = psPVRHeader->dwAlphaBitMask==0 ? GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG ;	// PVRTC2
		}
		else
		{
			IsCompressedFormatSupported = NNE_FALSE;
			IsCompressedFormat = NNE_TRUE;
			textureFormat = GL_UNSIGNED_BYTE;
			textureType = GL_RGBA;
			PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer warning: PVRTC2 not supported. Converting to RGBA8888 instead.\n");
		}
		break;

	case OGL_PVRTC4:
		if(IsPVRTCSupported)
		{
			IsCompressedFormatSupported = IsCompressedFormat = NNE_TRUE;
			textureFormat = psPVRHeader->dwAlphaBitMask==0 ? GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG ;	// PVRTC4
		}
		else
		{
			IsCompressedFormatSupported = NNE_FALSE;
			IsCompressedFormat = NNE_TRUE;
			textureFormat = GL_UNSIGNED_BYTE;
			textureType = GL_RGBA;
			PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer warning: PVRTC4 not supported. Converting to RGBA8888 instead.\n");
		}
		break;

	case OGL_BGRA_8888:
		if(IsBGRA8888Supported)
		{
			textureFormat = GL_UNSIGNED_BYTE;
			textureType   = GL_BGRA;
			break;
		}
		else
		{
			PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: Unable to load GL_BGRA texture as extension GL_IMG_texture_format_BGRA8888 is unsupported.\n");
			return 0;
		}
	default:											// NOT SUPPORTED
		PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: pixel type not supported.\n");
		return 0;
	}

	// load the texture up
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);				// Never have row-aligned in psPVRHeaders

	glGenTextures(1, &textureName);

	//  check that this data is cube map data or not.
	if(psPVRHeader->dwpfFlags & PVRTEX_CUBEMAP)
	{ // not in OGLES you don't
		PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: cube map textures are not available in OGLES1.x.\n");
		return 0;
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, textureName);
	}

	if(glGetError())
	{
		PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: glBindTexture() failed.\n");
		return 0;
	}

	for(i=0; i<u32NumSurfs; i++)
	{
		char *theTexturePtr = (texPtr? (char*)texPtr :  (char*)psPVRHeader + psPVRHeader->dwHeaderSize) + psPVRHeader->dwTextureDataSize * i;
		char *theTextureToLoad = 0;
		int		nMIPMapLevel;
		int		nTextureLevelsNeeded = (psPVRHeader->dwpfFlags & PVRTEX_MIPMAP)? psPVRHeader->dwMipMapCount : 0;
		unsigned int		nSizeX= psPVRHeader->dwWidth, nSizeY = psPVRHeader->dwHeight;
		unsigned int		CompressedImageSize = 0;

		for(nMIPMapLevel = 0; nMIPMapLevel <= nTextureLevelsNeeded; nSizeX = PVRT_MAX(nSizeX/2, (unsigned int)1), nSizeY = PVRT_MAX(nSizeY/2, (unsigned int)1), nMIPMapLevel++)
		{
			// Do Alpha-swap if needed

			theTextureToLoad = theTexturePtr;

			// Load the Texture

			/* If the texture is PVRTC then use GLCompressedTexImage2D */
			if(IsCompressedFormat)
			{
				/* Calculate how many bytes this MIP level occupies */
				if ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)==OGL_PVRTC2)
				{
					CompressedImageSize = ( PVRT_MAX(nSizeX, PVRTC2_MIN_TEXWIDTH) * PVRT_MAX(nSizeY, PVRTC2_MIN_TEXHEIGHT) * psPVRHeader->dwBitCount + 7) / 8;
				}
				else
				{// PVRTC4 case
					CompressedImageSize = ( PVRT_MAX(nSizeX, PVRTC4_MIN_TEXWIDTH) * PVRT_MAX(nSizeY, PVRTC4_MIN_TEXHEIGHT) * psPVRHeader->dwBitCount + 7) / 8;
				}

				if(((signed int)nMIPMapLevel - (signed int)nLoadFromLevel) >= 0)
				{
					if(IsCompressedFormatSupported)
					{
						//if(psPVRHeader->dwpfFlags&PVRTEX_CUBEMAP)
						//{
						//	/* Load compressed texture data at selected MIP level */
						//	glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, nMIPMapLevel-nLoadFromLevel, textureFormat, nSizeX, nSizeY, 0,
						//					CompressedImageSize, theTextureToLoad);
						//}
						//else
						{
							/* Load compressed texture data at selected MIP level */
							glCompressedTexImage2D(GL_TEXTURE_2D, nMIPMapLevel-nLoadFromLevel, textureFormat, nSizeX, nSizeY, 0,
											CompressedImageSize, theTextureToLoad);

						}
					}
					else
					{
#if 0
						// Convert PVRTC to 32-bit
						unsigned char *u8TempTexture = (unsigned char*)malloc(nSizeX*nSizeY*4);
						if ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)==OGL_PVRTC2)
						{
							PVRTCDecompress(theTextureToLoad, 1, nSizeX, nSizeY, u8TempTexture);
						}
						else
						{// PVRTC4 case
							PVRTCDecompress(theTextureToLoad, 0, nSizeX, nSizeY, u8TempTexture);
						}


						//if(psPVRHeader->dwpfFlags&PVRTEX_CUBEMAP)
						//{// Load compressed cubemap data at selected MIP level
						//	// Upload the texture as 32-bits
						//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,nMIPMapLevel-nLoadFromLevel,GL_RGBA,
						//		nSizeX,nSizeY,0, GL_RGBA,GL_UNSIGNED_BYTE,u8TempTexture);
						//	FREE(u8TempTexture);
						//}
						//else
						{// Load compressed 2D data at selected MIP level
							// Upload the texture as 32-bits
							glTexImage2D(GL_TEXTURE_2D,nMIPMapLevel-nLoadFromLevel,GL_RGBA,
								nSizeX,nSizeY,0, GL_RGBA,GL_UNSIGNED_BYTE,u8TempTexture);
							FREE(u8TempTexture);
						}
#else
						NNM_ASSERT( 0, "Texture Convert is not supported" );
						return 0;
#endif
					}
				}
			}
			else
			{
				if(((signed int)nMIPMapLevel - (signed int)nLoadFromLevel) >= 0)
				{
					//if(psPVRHeader->dwpfFlags&PVRTEX_CUBEMAP)
					//{
					//	/* Load uncompressed texture data at selected MIP level */
					//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,nMIPMapLevel-nLoadFromLevel,textureType,nSizeX,nSizeY,
					//		0, textureType,textureFormat,theTextureToLoad);
					//}
					//else
					{
						/* Load uncompressed texture data at selected MIP level */
						glTexImage2D(GL_TEXTURE_2D,nMIPMapLevel-nLoadFromLevel,textureType,nSizeX,nSizeY,0, textureType,textureFormat,theTextureToLoad);
					}
				}
			}



			if(glGetError())
			{
				PVRTOOLS_DEBUG_OUTPUT("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: glBindTexture() failed.\n");
				return 0;
			}

			// offset the texture pointer by one mip-map level

			/* PVRTC case */
			if ( IsCompressedFormat )
			{
				theTexturePtr += CompressedImageSize;
			}
			else
			{
				/* New formula that takes into account bit counts inferior to 8 (e.g. 1 bpp) */
				theTexturePtr += (nSizeX * nSizeY * psPVRHeader->dwBitCount + 7) / 8;
			}
		}
	}

	*texName = textureName;

	if(psTextureHeader)
	{
		*(PVR_Texture_Header*)psTextureHeader = *psPVRHeader;
		((PVR_Texture_Header*)psTextureHeader)->dwPVR = PVRTEX_IDENTIFIER;
		((PVR_Texture_Header*)psTextureHeader)->dwNumSurfs = u32NumSurfs;
	}

	return psPVRHeader->dwpfFlags|0x80000000;		// PVR psPVRHeader flags with topmost bit set so that it is non-zero
}

/*!***************************************************************************
 @Function		amIPhonePVRTLoadTextureFromPointer
 @Input			pointer			Pointer to header-texture's structure
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.
 @Return		true on success
 @Description	Allows textures to be stored in C header files and loaded in.  Loads the whole texture.
				Release texture by calling PVRTReleaseTexture.
*****************************************************************************/
unsigned int amIPhonePVRTLoadTextureFromPointer(const void* pointer, GLuint *const texName, const void *psTextureHeader)
{
	return amIPhonePVRTLoadPartialTextureFromPointer(pointer, 0, 0, texName, psTextureHeader);
}


/*****************************************************************************/
/* void amIPhoneSetTextureAttribute(AMS_PARAM_LOAD_TEXTURE *param)           */
/*---------------------------------------------------------------------------*/
/* [INPUT] param : テクスチャ登録パラメータ                                      */
/* [FUNCTION]  テクスチャ属性の設定                                             */
/*****************************************************************************/
void amIPhoneSetTextureAttribute(AMS_PARAM_LOAD_TEXTURE *param)
{
	GLint		filter;
	GLfloat		aniso;
	
	PVR_Texture_Header* psPVRHeader;
	bool		bMipmap;

	psPVRHeader = (PVR_Texture_Header*)param->tex;
	bMipmap     = ( (psPVRHeader->dwpfFlags & PVRTEX_MIPMAP) ? psPVRHeader->dwMipMapCount : 0 ) != 0;
	
/* バイリニア、トライリニア設定 */
#if ( NND_PLATFORM == NND_PLATFORM_GLES11 )
	switch (param->minfilter) {
		case	NND_MIN_LINEAR:
		case	NND_MIN_LINEAR_MIPMAP_LINEAR:
			filter = ( bMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR );
			break;
		case	NND_MIN_LINEAR_MIPMAP_NEAREST:
			filter = ( bMipmap ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR );
			break;
		case	NND_MIN_NEAREST_MIPMAP_LINEAR:
			filter = ( bMipmap ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST );
			break;
		case	NND_MIN_NEAREST:
		case	NND_MIN_NEAREST_MIPMAP_NEAREST:
		default:
			filter = ( bMipmap ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST );
			break;
	}
#else
	// 縮小フィルタ
	switch (param->minfilter) {
		case	NND_MIN_NEAREST:
		default:
			filter = GL_NEAREST;
			break;
		case	NND_MIN_LINEAR:
		case	NND_MIN_ANISOTROPIC2:
		case	NND_MIN_ANISOTROPIC4:
		case	NND_MIN_ANISOTROPIC8:
			filter = GL_LINEAR;
			break;
		case	NND_MIN_NEAREST_MIPMAP_NEAREST:
			filter = GL_NEAREST_MIPMAP_NEAREST;
			break;
		case	NND_MIN_NEAREST_MIPMAP_LINEAR:
			filter = GL_NEAREST_MIPMAP_LINEAR;
			break;
		case	NND_MIN_LINEAR_MIPMAP_NEAREST:
		case	NND_MIN_ANISOTROPIC2_MIPMAP_NEAREST:
		case	NND_MIN_ANISOTROPIC4_MIPMAP_NEAREST:
		case	NND_MIN_ANISOTROPIC8_MIPMAP_NEAREST:
			filter = GL_LINEAR_MIPMAP_NEAREST;
			break;
		case	NND_MIN_LINEAR_MIPMAP_LINEAR:
		case	NND_MIN_ANISOTROPIC2_MIPMAP_LINEAR:
		case	NND_MIN_ANISOTROPIC4_MIPMAP_LINEAR:
		case	NND_MIN_ANISOTROPIC8_MIPMAP_LINEAR:
			filter = GL_LINEAR_MIPMAP_LINEAR;
			break;
	}
#endif
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );

	// 異方性フィルタ
	switch (param->minfilter) {
		case	NND_MIN_ANISOTROPIC2:
		case	NND_MIN_ANISOTROPIC2_MIPMAP_NEAREST:
		case	NND_MIN_ANISOTROPIC2_MIPMAP_LINEAR:
			aniso = 2.f;
			break;
		case	NND_MIN_ANISOTROPIC4:
		case	NND_MIN_ANISOTROPIC4_MIPMAP_NEAREST:
		case	NND_MIN_ANISOTROPIC4_MIPMAP_LINEAR:
			aniso = 4.f;
			break;
		case	NND_MIN_ANISOTROPIC8:
		case	NND_MIN_ANISOTROPIC8_MIPMAP_NEAREST:
		case	NND_MIN_ANISOTROPIC8_MIPMAP_LINEAR:
			aniso = 8.f;
			break;
		default:
			aniso = 1.f;
			break;
	}
#if defined(GL_EXT_texture_filter_anisotropic)
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso );
#endif	// defined(GL_EXT_texture_filter_anisotropic)

	// 拡大フィルタ
	switch (param->magfilter) {
		case	NND_MAG_NEAREST:
		default:
			filter = GL_NEAREST;
			break;
		case	NND_MAG_LINEAR:
			filter = GL_LINEAR;
			break;
	}
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );

	param->pTexInfo->Bank			= param->bank;
	param->pTexInfo->GlobalIndex	= param->globalIndex;
	param->pTexInfo->Flag			= 0;
}

/*--- Local Functions -------------------------------------------------------*/

/*****************************************************************************/
/* BOOL _amIPhoneCheckExit(void)                                             */
/*---------------------------------------------------------------------------*/
/* [RETURN] ゲームを終了するかどうか                                         */
/* [FUNCTION]  ゲームの終了をチェックする                                    */
/*****************************************************************************/
BOOL _amIPhoneCheckExit(void)
{
extern Sint32	_am_exit_game;

	return (_am_iPhone_quit_req && _am_exit_game);
}

// The recommended technique for querying OpenGL extensions;
// from http://opengl.org/resources/features/OGLextensions/
static NNE_BOOL IsGLExtensionSupported(const char *extension)
{
    const GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;
	
    /* Extension names should not have spaces. */
    where = (GLubyte *) strchr(extension, ' ');
    if (where || *extension == '＼0')
        return NNE_FALSE;
	
    extensions = glGetString(GL_EXTENSIONS);
	
    /* It takes a bit of care to be fool-proof about parsing the
	 OpenGL extensions string. Don't be fooled by sub-strings, etc. */
    start = extensions;
    for (;;) {
        where = (GLubyte *) strstr((const char *) start, extension);
        if (!where)
            break;
        terminator = where + strlen(extension);
        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '＼0')
                return NNE_TRUE;
        start = terminator;
    }
    return NNE_FALSE;
}




