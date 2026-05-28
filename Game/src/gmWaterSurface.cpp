// ==========================================================================
/*!
  @file gmWaterSurface.cpp
  @brief 

  @author hanaoka
				Copyright(c) 2009 Dimps

  $Id: gmWaterSurface.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "gsMainSys.h"
#include "gmMain.h"
#include "gmGameDat.h"
#include "gmCamera.h"

#include "gmMapFar.h"	//ÉåÉìÉ_É^Å[ÉQÉbÉgÇégÇ¢Ç‹ÇÌÇµÇƒÇ¢ÇÈÇΩÇﬂ

#include "gmWaterSurface.h"

#if _IPHONE
#include "izFade.h"
#include "dbgPadEmu.hpp"
#endif

//----- Definitions ---------------------------------------------------------

// ÉvÉäÉRÉìÉpÉCÉãÉVÉFÅ[É_Å[égóp
#define GMD_WATER_PRECOMPILED		(1)

//êÖñ ï`âÊÉTÉCÉY
#define GMD_WATER_SURFACE_DRAW_WIDTH ( 300.0f )
#define GMD_WATER_SURFACE_DRAW_HIEGHT ( 300.0f )

#if _IPHONE
#define GMD_WATER_SURFACE_TEX_V_HEIGHT (150.0f)
#define GMD_WATER_SURFACE_TEX_U_NUM    (1.0f)
#define GMD_WATER_SURFACE_DRAW_NUM		(3)

#define GMD_WATER_SURFACE_DRAW_TEX_SIZE				(128.0f)	//êÖñ ÉeÉNÉXÉ`ÉÉÇÃÉTÉCÉY
#define GMD_WATER_SURFACE_DRAW_TEX_OFFSET_V			(0.3f)		//ÉLÉâÉLÉâñ ÇÇ∏ÇÁÇ∑ÉIÉtÉZÉbÉg
#define GMD_WATER_SURFACE_DRAW_TEX_BASE_SURFACE_U	(22.0f)		//íPêFêÖñ ópÉeÉNÉXÉ`ÉÉUV
#define GMD_WATER_SURFACE_DRAW_TEX_BASE_SURFACE_V	(64.0f)		//íPêFêÖñ ópÉeÉNÉXÉ`ÉÉUV
#define GMD_WATER_SURFACE_DRAW_TEX_ADJUST_SIZE		(0.5f)		//êÖñ ÉeÉNÉXÉ`ÉÉÉTÉCÉYí≤êÆíl


#define GMD_WATER_COLOR_CONTROL		(0)
#endif // _IPHONE

///ÉfÅ[É^ä«óù
typedef struct tag_GMS_WATER_SURFACE_DATA {
	AMS_AMB_HEADER* amb_header;						///< ambÉwÉbÉ_
	BOOL flag_load_object;
}GMS_WATER_SURFACE_DATA;

///êÖñ 
typedef struct tag_GMS_WATER_SURFACE_INFO
{
	Float now_water_level;
	Float next_water_level;
	u16 water_time;
	u16 water_counter;
	BOOL flag_draw;
	BOOL flag_enable_ref;	//êÖñ ÇÃîΩéÀóLå¯ÉtÉâÉO
}GMS_WATER_SURFACE_INFO;

///êÖñ ä«óù
typedef struct tag_GMS_WATER_SURFACE_MGR {	
	MTS_TASK_TCB* tcb_water;
	AMS_RENDER_TARGET* render_target;	//ÉåÉìÉ_É^Å[ÉQÉbÉg
}GMS_WATER_SURFACE_MGR;




// ==========================================================================
///ÉåÉìÉ_êÖñ 
// ==========================================================================

// êÖÉIÉuÉWÉFÉNÉg
typedef struct {
	AMS_MOTION		*motion;		// ÉÇÅ[ÉVÉáÉì
	NNS_OBJECT		*object;		// ÉIÉuÉWÉFÉNÉg
	NNS_TEXLIST		*texlist;		// ÉeÉNÉXÉ`ÉÉÉäÉXÉg
	void			*texlistbuf;	// ÉeÉNÉXÉ`ÉÉÉäÉXÉgÉoÉbÉtÉ@

	float			frame;			// ÉÇÅ[ÉVÉáÉìÉtÉåÅ[ÉÄ
} DMAP_WATER_OBJ;

// êÖï`âÊÉpÉâÉÅÅ[É^
typedef struct {
	float			frame[2];		// ÉÇÅ[ÉVÉáÉìÉtÉåÅ[ÉÄ
	float			draw_u;			// ï`âÊéûÇÃUVç¿ïW
	float			draw_v;
	float			scale;			// ÉXÉPÅ[Éã
	float			pos_x;			// äÓèÄç¿ïW
	float			pos_y;
	float			pos_dy;			// âÊñ è„êÖà 
	float			repeat_u;		// ÉäÉsÅ[Égíl
	float			repeat_v;
	Angle32			rot_z;			// âÒì]äp
	Uint32			color;			// êÖÇÃêF
} DMAP_PARAM_WATER;

// êÖä«óù
typedef struct {
	AMS_AMB_HEADER	*amb_object;	// ÉIÉuÉWÉFÉNÉgAMB
	AMS_AMB_HEADER	*amb_texture;	// ÉeÉNÉXÉ`ÉÉAMB
	Sint32			regist_index;	// ìoò^ë“ÇøID
	DMAP_WATER_OBJ	object[2];		// êÖñ (0)Ç∆ÉLÉâÉÅÉL(1)
	float			draw_u;			// ï`âÊéûÇÃUVç¿ïW
	float			draw_v;
	float			scale;			// ÉXÉPÅ[Éã
	float			ofst_u;			// ÉAÉjÉÅÅ[ÉVÉáÉìUVç¿ïW
	float			ofst_v;
	float			repeat_u;		// ÉäÉsÅ[Égíl
	float			repeat_v;
	float			speed_u;		// ÉAÉjÉÅÅ[ÉVÉáÉìë¨ìx
	float			speed_v;
#if _IPHONE
	float			speed_surface;	// ï\ñ ë¨ìx
#endif // _IPHONE
	float			pos_x;			// äÓèÄç¿ïW
	float			pos_y;
	float			pos_dy;			// âÊñ è„êÖà 
	Angle32			rot_z;			// âÒì]äp
	Uint32			color;			// êÖÇÃêF
	float			repeat_pos_x;
	DMAP_PARAM_WATER	*draw_param;
#if _PC | _XBOX
	IDirect3DVertexShader9	*shader_VS;
	IDirect3DPixelShader9	*shader_PS;
	LPDIRECT3DTEXTURE9		tex_water;
	LPDIRECT3DTEXTURE9		tex_color;
#elif _PS3
	void			*shader_VS;
	void			*shader_PS;
	NNS_TEXTURE_PS3	*tex_water;
	NNS_TEXTURE_PS3	*tex_color;
#elif _WII
	NVS_GVROBJ		gvrobj_water;
	NVS_GVROBJ		gvrobj_color;
	GXTexObj		tex_water;
	GXTexObj		tex_color;
#elif _IPHONE
	AOS_TEXTURE		tex_color;
#endif
} DMAP_WATER;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

// ==========================================================================
// ÉQÅ[ÉÄÉVÉXÉeÉÄ
// ==========================================================================
static u16 gmWaterSurfaceGameSystemGetWaterLevel( void );
static void gmWaterSurfaceGameSystemSetWaterLevel( u16 water_level );
static GSE_MAIN_STAGE_ID gmWaterSurfaceGameSystemGetStageId( void );
static GSE_MAIN_ZONE_TYPE gmWaterSurfaceGameSystemGetZoneType( GSE_MAIN_STAGE_ID stage_id );

// ==========================================================================
// ÉfÅ[É^ä«óù
// ==========================================================================
#if GMD_WATER_SURFACE_USE_RENDER
static void gmWaterSurfaceDataInit( void );
static void gmWaterSurfaceDataRelease( void );
static GMS_WATER_SURFACE_DATA* gmWaterSurfaceDataGetInfo( void );
static void gmWaterSurfaceDataSetAmbHeader( AMS_AMB_HEADER* amb );
static AMS_AMB_HEADER* gmWaterSurfaceDataGetAmbHeader( void );
static void gmWaterSurfaceDataReleaseAmbHeader( void );

#endif	// GMD_WATER_SURFACE_USE_RENDER
// ==========================================================================
// êÖñ ä«óù
// ==========================================================================
//êÖñ ê›íË
static GMS_WATER_SURFACE_MGR* gmWaterSurfaceGetMgr( void );
static void gmWaterSurfaceInitMgr( void );
static void gmWaterSurfaceExitMgr( void );

// ==========================================================================
// êÖñ 
// ==========================================================================
static MTS_TASK_TCB* gmWaterSurfaceCreateTcb( void );
static void gmWaterSurfaceDeleteTcb( void );
static void gmWaterSurfaceTcbProcPreDrawDT(void *data);
#if GMD_WATER_SURFACE_USE_RENDER
static void gmWaterSurfaceTcbProcDrawDT(void *data);
static void gmWaterSurfaceProc( MTS_TASK_TCB *tcb );
static void gmWaterSurfaceTcbProcPostDrawDT(void *data);

static void gmWaterSurfaceMatrixPush( u32 command_state  );
static void gmWaterSurfaceMatrixPop( u32 command_state  );
#else
static void gmWaterSurfaceProc( MTS_TASK_TCB *tcb );

static void gmWaterSurfaceMatrixPush( u32 command_state  );
static void gmWaterSurfaceMatrixPop( u32 command_state  );
//static void gmWaterSurfaceMatrixPush( void );
//static void gmWaterSurfaceMatrixPop( void );
#endif	//GMD_WATER_SURFACE_USE_RENDER

static void gmWaterSurfaceUserFuncMatrixPush( void* param );
static void gmWaterSurfaceUserFuncPop( void* param );

// ==========================================================================
///ÉåÉìÉ_êÖñ 
// ==========================================================================

#if GMD_WATER_SURFACE_USE_RENDER
static Sint32 dwaterInit(void);
static void dwaterExit(void);
static void dwaterSetObjectAMB(AMS_AMB_HEADER *amb_obj, AMS_AMB_HEADER *amb_tex = NULL);
static Sint32 dwaterLoadObject(NNF_DRAWOBJ objflag);
static Sint32 dwaterLoadTexture(void *water_image, Sint32 water_size,
		void *color_image, Sint32 color_size);
static Sint32 dwaterRelease(void);
static void dwaterSetColor(float r, float g, float b);
static void dwaterUpdate(float speed, float pos_x, float pos_y, float dy,
		Angle32 rot_z, float scale = 1.0f);
static void dwaterGetParam(DMAP_PARAM_WATER *param);
static void dwaterSetParam(void);
static void dwaterDrawReflection(Uint32 state, NNF_DRAWOBJ drawflag = 0);
static void dwaterDrawSurface(Uint32 state, NNF_DRAWOBJ drawflag = 0);
//static void dwaterDrawReflection(NNF_DRAWOBJ drawflag = 0);
//static void dwaterDrawSurface(NNF_DRAWOBJ drawflag);
static void dwaterDrawWater(AMS_RENDER_TARGET *texture);
static void _dwaterSetParam(AMS_TCB *tcbp);

#endif	// GMD_WATER_SURFACE_USE_RENDER

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------

// ==========================================================================
///ÉfÅ[É^ä«óù
// ==========================================================================
//ÉfÅ[É^
static GMS_WATER_SURFACE_DATA g_water_surface_data_real;
static GMS_WATER_SURFACE_DATA* g_water_surface_data = NULL;

// ==========================================================================
///êÖñ ä«óù
// ==========================================================================
//ä«óùèÓïÒ
static GMS_WATER_SURFACE_MGR g_water_surface_mgr_real;
static GMS_WATER_SURFACE_MGR* g_water_surface_mgr = NULL;















// ==========================================================================
///ÉåÉìÉ_êÖñ 
// ==========================================================================

#if GMD_WATER_SURFACE_USE_RENDER

DMAP_WATER	*_dmap_water = NULL;

#if !GMD_WATER_PRECOMPILED
#if _PC | _XBOX
static const CHAR* _dmap_draw_water_code =
	"struct VS_INPUT_PT                                        \n"
	"{                                                         \n"
	"   float4 Pos            : POSITION;                      \n"
	"   float2 UV0            : TEXCOORD0;                     \n"
	"   float2 UV1            : TEXCOORD1;                     \n"
	"   float2 UV2            : TEXCOORD2;                     \n"
	"   float4 Color          : COLOR0;                        \n"
	"};                                                        \n"
	"                                                          \n"
	"struct VS_OUTPUT_PT                                       \n"
	"{                                                         \n"
	"   float4 Pos	          : POSITION;                      \n"
	"   float2 UV0            : TEXCOORD0;                     \n"
	"   float2 UV1            : TEXCOORD1;                     \n"
	"   float2 UV2            : TEXCOORD2;                     \n"
	"   float4 Color          : COLOR0;                        \n"
	"};                                                        \n"
	"                                                          \n"
	"VS_OUTPUT_PT main_VS(VS_INPUT_PT Input)                   \n"
	"{                                                         \n"
	"   VS_OUTPUT_PT Output;                                   \n"
	"   Output.Pos.x  = Input.Pos.x - 0.5;                     \n"
	"   Output.Pos.y  = Input.Pos.y - 0.5;                     \n"
	"   Output.Pos.z  = Input.Pos.z;                           \n"
	"   Output.Pos.w  = Input.Pos.w;                           \n"
	"   Output.UV0    = Input.UV0;                             \n"
	"   Output.UV1    = Input.UV1;                             \n"
	"   Output.UV2    = Input.UV2;                             \n"
	"   Output.Color  = Input.Color;                           \n"
	"   return (Output);                                       \n"
	"}                                                         \n"
	"                                                          \n"
	"                                                          \n"
	"struct PS_INPUT_T                                         \n"
	"{                                                         \n"
 	"   float2 UV0            : TEXCOORD0;                     \n"
 	"   float2 UV1            : TEXCOORD1;                     \n"
 	"   float2 UV2            : TEXCOORD2;                     \n"
	"   float4 Color          : COLOR0;                        \n"
	"};                                                        \n"
	"                                                          \n"
	"sampler2D        s_TexColor  : register(s0);              \n"
	"sampler2D        s_TexOffset : register(s1);              \n"
	"sampler2D        s_TexGray   : register(s2);              \n"
	"                                                          \n"
	"float4 main_PS(PS_INPUT_T Input) : COLOR0                 \n"
	"{                                                         \n"
	"   float4 uv;                                             \n"
	"   uv      = tex2D(s_TexOffset, Input.UV1);               \n"
//	"   uv.zw   = uv.zw * 0.03f - 0.015f;                      \n"
	"   uv.zw   = uv.zw * 0.01f - 0.005f;                      \n"
	"   uv.x	= Input.UV0.x + uv.w;                          \n"
	"   uv.y    = Input.UV0.y + uv.z;                          \n"
	"   uv      = Input.Color * tex2D(s_TexColor, uv);         \n"
	"   uv      *= tex2D(s_TexGray, Input.UV2);                \n"
	"   uv.w	= 1.0f;                                        \n"
 	"   return (uv);                                           \n"
	"}                                                         \n";
#elif _PS3
static const char* _dmap_draw_water_code_VS =
	"struct VS_INPUT_PT                                        \n"
	"{                                                         \n"
	"   float4 Pos            : POSITION;                      \n"
	"   float4 Color          : COLOR0;                        \n"
	"   float2 UV0            : TEXCOORD0;                     \n"
//	"   float2 UV1            : TEXCOORD1;                     \n"
	"};                                                        \n"
	"                                                          \n"
	"struct VS_OUTPUT_PT                                       \n"
	"{                                                         \n"
	"   float4 Pos	          : POSITION;                      \n"
	"   float4 Color          : COLOR0;                        \n"
	"   float2 UV0            : TEXCOORD0;                     \n"
	"   float2 UV1            : TEXCOORD1;                     \n"
	"   float2 UV2            : TEXCOORD2;                     \n"
	"};                                                        \n"
	"                                                          \n"
	"uniform float4x4   u_ProjectionMatrix;                    \n"
	"uniform float4     u_Priority;                            \n"
	"                                                          \n"
	"VS_OUTPUT_PT main_VS(VS_INPUT_PT Input)                   \n"
	"{                                                         \n"
	"   VS_OUTPUT_PT Output;                                   \n"
	"   Input.Pos.z   = u_Priority.x;                          \n"
	"   Output.Pos    = mul(Input.Pos, u_ProjectionMatrix);    \n"
	"   Output.UV0    = Output.Pos.xy * float2(0.5f, -0.5f) + 0.5f;\n"
	"   Output.UV1    = Input.UV0;                             \n"
	"   Output.UV2    = float2(0.5f, Input.Color.w);           \n"
	"   Output.Color  = Input.Color;                           \n"
	"   return (Output);                                       \n"
	"}                                                         \n";

static const char* _dmap_draw_water_code_PS =
	"struct PS_INPUT_T                                         \n"
	"{                                                         \n"
 	"   float2 UV0            : TEXCOORD0;                     \n"
 	"   float2 UV1            : TEXCOORD1;                     \n"
 	"   float2 UV2            : TEXCOORD2;                     \n"
	"   float4 Color          : COLOR0;                        \n"
	"};                                                        \n"
	"                                                          \n"
	"sampler2D        s_TexColor  : register( s0 );            \n"
	"sampler2D        s_TexOffset : register( s1 );            \n"
	"sampler2D        s_TexGray   : register( s2 );            \n"
	"                                                          \n"
	"float4 main_PS(PS_INPUT_T Input) : COLOR0                 \n"
	"{                                                         \n"
	"   float4  uv;                                            \n"
	"   uv      = tex2D(s_TexOffset, Input.UV1);               \n"
//	"   uv.zw   = uv.zw * 0.03f - 0.015f;                      \n"
	"   uv.zw   = uv.zw * 0.01f - 0.005f;                      \n"
	"   uv.x	= Input.UV0.x + uv.w;                          \n"
	"   uv.y    = Input.UV0.y + uv.z;                          \n"
	"   uv      = Input.Color * tex2D(s_TexColor, uv);         \n"
	"   uv      *= tex2D(s_TexGray, Input.UV2);                \n"
	"   uv.w	= 1.0f;                                        \n"
 	"   return (uv);                                           \n"
	"}                                                         \n";
#endif
#else
#if _PC
#include "shader/win32/draw_water_VS.fxh"
#include "shader/win32/draw_water_PS.fxh"
#elif _XBOX
#include "shader/xbox360/draw_water_VS.fxh"
#include "shader/xbox360/draw_water_PS.fxh"
#elif _PS3
#include "shader/ps3/draw_water.vph"
#include "shader/ps3/draw_water.fph"
#endif
#endif

#endif	//GMD_WATER_SURFACE_USE_RENDER

//----- Global Functions ----------------------------------------------------

// ==========================================================================
//ÉfÅ[É^
// ==========================================================================


// ==========================================================================
// GmWaterSurfaceInitData
/*!
 * êÖñ ÉfÅ[É^Çèâä˙âª
 *
 * @param amb ambÉfÅ[É^ÉwÉbÉ_
 */
// ==========================================================================
void GmWaterSurfaceInitData( AMS_AMB_HEADER* amb )
{
	UNREFERENCED_PARAMETER( amb );

#if GMD_WATER_SURFACE_USE_RENDER
	amAssert( !g_water_surface_data );
	amAssert( amb );

	//ÉfÅ[É^ä«óùèâä˙âª
	gmWaterSurfaceDataInit();

	//ÉRÉìÉoÅ[Ég
	amBindConvertAll( (u8*)amb );

	//ìoò^
	gmWaterSurfaceDataSetAmbHeader( amb );
#endif	// GMD_WATER_SURFACE_USE_RENDER
}

// ==========================================================================
// GmWaterSurfaceBuildData
/*!
 * êÖñ ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmWaterSurfaceBuildData( void )
{
#if GMD_WATER_SURFACE_USE_RENDER
	GSE_MAIN_STAGE_ID stage_id = gmWaterSurfaceGameSystemGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmWaterSurfaceGameSystemGetZoneType(stage_id);
	if ( zone_id == GSD_MAIN_ZONE_TYPE_3 ){
		dwaterInit();

		AMS_AMB_HEADER* amb_header = gmWaterSurfaceDataGetAmbHeader();
		amAssert( amb_header );

#if _IPHONE
		//AMS_AMB_HEADER    *colfile;
		//amBindGet(amb_header, amb_header->file_num - 1, &colfile);
		AoTexBuild(&_dmap_water->tex_color, amBindGet(amb_header, amb_header->file_num - 1)/*(void*)colfile*/);
		AoTexLoad(&_dmap_water->tex_color);
#else
		AMS_AMB_FILE	*texfile, *colfile;
		amBindGet(amb_header, 4, &texfile);
		amBindGet(amb_header, 5, &colfile);
		dwaterLoadTexture(texfile->data, texfile->size, colfile->data, colfile->size);
#endif // _IPHONE
		
		dwaterSetObjectAMB(amb_header, amb_header);

		NNF_DRAWOBJ draw_flag = (NNF_DRAWOBJ)0;
#if (_PC | _XBOX | _PS3)
		draw_flag = NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND;
#endif
		dwaterLoadObject(draw_flag);
	}
#endif	// GMD_WATER_SURFACE_USE_RENDER
}

// ==========================================================================
// GmWaterSurfaceCheckLoading
/*!
 * ì«Ç›çûÇ›ë“ÇøÉ`ÉFÉbÉN
 *
 * @return TRUEÅFì«Ç›çûÇ›èIóπ FALSEÅFì«Ç›çûÇ›ë“Çø
 *
 */
// ==========================================================================
BOOL GmWaterSurfaceCheckLoading( void )
{
	BOOL result = TRUE;

	if (GsMainSysGetDisplayListRegistNum() >= AMD_REGISTLIST_NUM - 64) {
		return FALSE;
	}

#if GMD_WATER_SURFACE_USE_RENDER
	GSE_MAIN_STAGE_ID stage_id = gmWaterSurfaceGameSystemGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmWaterSurfaceGameSystemGetZoneType(stage_id);
	if ( zone_id == GSD_MAIN_ZONE_TYPE_3 ){
		GMS_WATER_SURFACE_DATA* data = gmWaterSurfaceDataGetInfo();
		amAssert(data);
#if _IPHONE
		// ÉeÉNÉXÉ`ÉÉÇ™ì«ÇﬂÇƒÇ»ÇØÇÍÇŒèIóπ
		if (!AoTexIsLoaded(&_dmap_water->tex_color)) {
			result = FALSE;
		} else
#endif
		if ( !data->flag_load_object ){
			NNF_DRAWOBJ draw_flag = (NNF_DRAWOBJ)0;
#if (_PC | _XBOX | _PS3)
			draw_flag = NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND;
#endif
			if ( !dwaterLoadObject(draw_flag) ){
				result &= FALSE;
				data->flag_load_object = FALSE;
			}
			else{
				data->flag_load_object = TRUE;
			}
		}
	}
#endif	// GMD_WATER_SURFACE_USE_RENDER

	return result;
}

// ==========================================================================
// GmWaterSurfaceInit
/*!
 * êÖñ èâä˙âª
 */
// ==========================================================================
void GmWaterSurfaceInit( void )
{
	//ÉQÅ[ÉÄÉVÉXÉeÉÄÇÃílèâä˙âª
	gmWaterSurfaceGameSystemSetWaterLevel( 0xFFFF );

	GSE_MAIN_STAGE_ID stage_id = gmWaterSurfaceGameSystemGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmWaterSurfaceGameSystemGetZoneType(stage_id);
	if ( zone_id == GSD_MAIN_ZONE_TYPE_3 ){

		amAssert(!g_water_surface_mgr);

		//ä«óùèÓïÒèâä˙âª
		gmWaterSurfaceInitMgr();
		
		//êÖñ ï`âÊÉ^ÉXÉNçÏê¨
		gmWaterSurfaceCreateTcb();
	}
}

// ==========================================================================
// GmWaterSurfaceExit
/*!
 * êÖñ èIóπ
 */
// ==========================================================================
void GmWaterSurfaceExit( void )
{
	//êÖñ ä«óùâï˙
	gmWaterSurfaceExitMgr();
}

// ==========================================================================
// GmWaterSurfaceFlushData
/*!
 * êÖñ ç\ízÇµÇΩÉfÅ[É^âï˙
 */
// ==========================================================================
void GmWaterSurfaceFlushData( void )
{
#if GMD_WATER_SURFACE_USE_RENDER
#if _IPHONE
	GSE_MAIN_STAGE_ID stage_id = gmWaterSurfaceGameSystemGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmWaterSurfaceGameSystemGetZoneType(stage_id);
	if ( zone_id == GSD_MAIN_ZONE_TYPE_3 ){
		AoTexRelease(&_dmap_water->tex_color);
	}
#endif // _IPHONE
#endif	// GMD_WATER_SURFACE_USE_RENDER
}

// ==========================================================================
// GmWaterSurfaceRelease
/*!
 * êÖñ ì«Ç›çûÇÒÇæÉfÅ[É^âï˙
 */
// ==========================================================================
void GmWaterSurfaceRelease( void )
{
#if GMD_WATER_SURFACE_USE_RENDER
	GSE_MAIN_STAGE_ID stage_id = gmWaterSurfaceGameSystemGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmWaterSurfaceGameSystemGetZoneType(stage_id);
	if ( zone_id == GSD_MAIN_ZONE_TYPE_3 ){
		//ÉfÅ[É^âï˙
		gmWaterSurfaceDataRelease();
	}
#endif	// GMD_WATER_SURFACE_USE_RENDER
}

// ==========================================================================
// GmWaterSurfaceCheckFlush
/*!
 * ÉfÅ[É^âï˙ë“Çø
 */
// ==========================================================================
BOOL GmWaterSurfaceCheckFlush( void )
{
	if (GsMainSysGetDisplayListRegistNum() >= AMD_REGISTLIST_NUM - 64) {
		return FALSE;
	}

	BOOL result = TRUE;
#if GMD_WATER_SURFACE_USE_RENDER


	GSE_MAIN_STAGE_ID stage_id = gmWaterSurfaceGameSystemGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmWaterSurfaceGameSystemGetZoneType(stage_id);
	if ( zone_id == GSD_MAIN_ZONE_TYPE_3 ){
		if ( _dmap_water ){			
#if _IPHONE
			if (!AoTexIsReleased(&_dmap_water->tex_color)) {
				result = FALSE;
			} else
#endif // _IPHONE
			if ( _dmap_water->regist_index == -1 ){
				_dmap_water->regist_index = dwaterRelease();
				result &= FALSE;
			}
			else if ( !amDrawIsRegistComplete( _dmap_water->regist_index ) ){
				result &= FALSE;
			}
			else{
				dwaterExit();
			}
		}
	}
#endif	// GMD_WATER_SURFACE_USE_RENDER
	return result;
}

// ==========================================================================
// êÖñ ä«óù
// ==========================================================================

// ==========================================================================
// GmWaterSurfaceRequestChangeWaterLevel
/*!
 * êÖñ ÉåÉxÉãïœçXÇéwíË
 *
 * @param u16 water_level êÖñ ÉåÉxÉã
 * @param u32 time ïœçXÇ…ä|ÇØÇÈÉtÉåÅ[ÉÄêî
 * @param u32 flag_add_time éûä‘Çâ¡éZÇ∑ÇÈÉtÉâÉOÅiTRUEÅFâ¡éZÅ@FALSEÅFè„èëÇ´Åj
 *
 */
// ==========================================================================
void GmWaterSurfaceRequestChangeWaterLevel( u16 water_level, u16 time, BOOL flag_add_time )
{
	GSE_MAIN_STAGE_ID stage_id = gmWaterSurfaceGameSystemGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmWaterSurfaceGameSystemGetZoneType(stage_id);
	if ( zone_id == GSD_MAIN_ZONE_TYPE_3 ){
		GMS_WATER_SURFACE_MGR* mgr = gmWaterSurfaceGetMgr();
		if ( !mgr ){
			return; 
		}

		GMS_WATER_SURFACE_INFO* work = (GMS_WATER_SURFACE_INFO*)mtTaskGetTcbWork(mgr->tcb_water);
		amAssert( work );

		//éûä‘â¡éZ
		if ( flag_add_time ){
			work->water_time += time;
		}
		//éûä‘è„èëÇ´
		else{
			work->water_time = time;
			work->water_counter = 0;
		}

		//êÖñ ÉåÉxÉã
		work->next_water_level = (Float)water_level;
		work->now_water_level = (Float)gmWaterSurfaceGameSystemGetWaterLevel();
	}
}
// ==========================================================================
// GmWaterSurfaceRequestAddWatarLevel
/*!
 * êÖñ ÉåÉxÉãïœçXÇéwíË
 *
 * @param Float water_level í«â¡Ç∑ÇÈêÖñ ÉåÉxÉã
 * @param u32 time ïœçXÇ…ä|ÇØÇÈÉtÉåÅ[ÉÄêî
 * @param u32 flag_add_time éûä‘Çâ¡éZÇ∑ÇÈÉtÉâÉOÅiTRUEÅFâ¡éZÅ@FALSEÅFè„èëÇ´Åj
 *
 */
// ==========================================================================
void GmWaterSurfaceRequestAddWatarLevel( Float water_level, u16 time, BOOL flag_add_time )
{
	GSE_MAIN_STAGE_ID stage_id = gmWaterSurfaceGameSystemGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmWaterSurfaceGameSystemGetZoneType(stage_id);
	if ( zone_id == GSD_MAIN_ZONE_TYPE_3 ){
		GMS_WATER_SURFACE_MGR* mgr = gmWaterSurfaceGetMgr();
		if ( !mgr ){
			return; 
		}

		GMS_WATER_SURFACE_INFO* work = (GMS_WATER_SURFACE_INFO*)mtTaskGetTcbWork(mgr->tcb_water);
		amAssert( work );

		//éûä‘â¡éZ
		if ( flag_add_time ){
			work->water_time += time;
		}
		//éûä‘è„èëÇ´
		else{
			work->water_time = time;
			work->water_counter = 0;
		}

		//êÖñ ÉåÉxÉã
		work->next_water_level += water_level;
		work->now_water_level = (Float)gmWaterSurfaceGameSystemGetWaterLevel();
	}
}

// ==========================================================================
// GmWaterSurfaceSetFlagDraw
/*!
 * êÖñ ï`âÊÉtÉâÉOÇê›íË
 *
 * @param BOOL flag_draw êÖñ ï`âÊÉtÉâÉO
 *
 */
// ==========================================================================
void GmWaterSurfaceSetFlagDraw( BOOL flag_draw )
{
	GSE_MAIN_STAGE_ID stage_id = gmWaterSurfaceGameSystemGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmWaterSurfaceGameSystemGetZoneType(stage_id);
	if ( zone_id == GSD_MAIN_ZONE_TYPE_3 ){
		GMS_WATER_SURFACE_MGR* mgr = gmWaterSurfaceGetMgr();
		if ( !mgr ){
			return; 
		}

		GMS_WATER_SURFACE_INFO* work = (GMS_WATER_SURFACE_INFO*)mtTaskGetTcbWork(mgr->tcb_water);
		amAssert( work );

		//éûä‘â¡éZ
		work->flag_draw = flag_draw;
	}
}

// ==========================================================================
// GmWaterSurfaceSetFlagEnableRef
/*!
 * îΩéÀóLå¯ÉtÉâÉOÇê›íË
 *
 * @param BOOL flag_enable_ref îΩéÀóLå¯ÉtÉâÉO
 *
 */
// ==========================================================================
void GmWaterSurfaceSetFlagEnableRef( BOOL flag_enable_ref )
{
	GSE_MAIN_STAGE_ID stage_id = gmWaterSurfaceGameSystemGetStageId();
	GSE_MAIN_ZONE_TYPE zone_id = gmWaterSurfaceGameSystemGetZoneType(stage_id);
	if ( zone_id == GSD_MAIN_ZONE_TYPE_3 ){
		GMS_WATER_SURFACE_MGR* mgr = gmWaterSurfaceGetMgr();
		if ( !mgr ){
			return; 
		}
		GMS_WATER_SURFACE_INFO* work = (GMS_WATER_SURFACE_INFO*)mtTaskGetTcbWork(mgr->tcb_water);
		amAssert( work );

		//éûä‘â¡éZ
		work->flag_enable_ref = flag_enable_ref;
	}
}


// ==========================================================================
// GmWaterSurfaceGetRenderTarget
/*!
 * ÉåÉìÉ_Å[É^Å[ÉQÉbÉgÇéÊìæ
 *
 * @return  ÉåÉìÉ_Å[É^Å[ÉQÉbÉg
 */
// ==========================================================================
AMS_RENDER_TARGET* GmWaterSurfaceGetRenderTarget( void )
{
	GMS_WATER_SURFACE_MGR* mgr = gmWaterSurfaceGetMgr();
	if ( !mgr ){
		return NULL;
	}

	return mgr->render_target;
}


// ==========================================================================
// GmWaterSurfaceDrawNoWaterField
/*!
 * êÖñ Çï`âÊÇ≥ÇπÇ»Ç¢ÇΩÇﬂÇÃî¬Çï`âÊ
 *
 * @param left ãÈå`
 * @param top ãÈå`
 * @param right ãÈå`
 * @param bottom ãÈå`
 */
// ==========================================================================
void GmWaterSurfaceDrawNoWaterField( 
									Float left,
									Float top,
									Float right,
									Float bottom)
{
	AMS_PARAM_DRAW_PRIMITIVE param;
	amZeroMemory( &param, sizeof(param) );

	param.aTest = 0;
	param.zMask = 0;
	param.zTest = 1;

	//ÉAÉãÉtÉ@
	param.ablend = NNE_PRIM_ALPHABLEND_ON;

#if _PC | _XBOX
	param.bldSrc = NNE_BLENDMODE_SRCALPHA;
	param.bldDst = NNE_BLENDMODE_DSTALPHA;
	param.bldMode = NNE_BLENDOP_ADD;
#elif _PS3
	param.bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
	param.bldDst = NND_BLENDFUNC_PS3_DST_ALPHA;
	param.bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
	param.bldSrc = GX_BL_SRCALPHA;
	param.bldDst = GX_BL_DSTALPHA;
	param.bldMode = GX_BM_BLEND;
#endif
	param.noSort = 1;

	// ÉeÉNÉXÉ`ÉÉÇ»Çµ
	NNS_PRIM3D_PC* poli_data = (NNS_PRIM3D_PC *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PC) * 6);

	Float z = FX_FX32_TO_F32( GMD_OBJ_DEFAULT_POS_Z_N_FRONT+32*FX32_ONE );

	// í∏ì_
	amVectorSet(
			(NNS_VECTOR*)&poli_data[0], 
			left, 
			top, 
			z );
	amVectorSet(
			(NNS_VECTOR*)&poli_data[1], 
			right, 
			top, 
			z );
	amVectorSet(
			(NNS_VECTOR*)&poli_data[2], 
			left, 
			bottom, 
			z );
	amVectorSet(
			(NNS_VECTOR*)&poli_data[5],
			right, 
			bottom, 
			z );

	// ÉJÉâÅ[
	NNS_RGBA8888 color = AMD_RGBA8888( 255, 255, 255, 0);
	poli_data[0].Col = color;
	poli_data[1].Col = color;
	poli_data[2].Col = color;
	poli_data[5].Col = color;

	poli_data[3] = poli_data[1];
	poli_data[4] = poli_data[2];

	// ÉvÉäÉ~ÉeÉBÉuï`âÊê›íË
	param.format3D = NNE_PRIM3D_FMT_PC;
	param.type = NNE_PRIM_TRIANGLE_LIST;
	param.vtxPC3D = poli_data;
	param.texlist = NULL;
	param.texId = 0;
	param.count = 6;
	param.sortZ = -1.0f;
	
	//É}ÉgÉäÉNÉXÉvÉbÉVÉÖÉRÉ}ÉìÉh
	gmWaterSurfaceMatrixPush( OBD_DRAW_CMD_STATE_PRE_WATER );
	
	//ï`âÊÉRÉ}ÉìÉh
	amDrawPrimitive3D( OBD_DRAW_CMD_STATE_PRE_WATER, &param );

	//É}ÉgÉäÉNÉXÉ|ÉbÉvÉRÉ}ÉìÉh
	gmWaterSurfaceMatrixPop( OBD_DRAW_CMD_STATE_PRE_WATER );
}
//----- Local Functions -----------------------------------------------------



// ==========================================================================
// ÉQÅ[ÉÄÉVÉXÉeÉÄ
// ==========================================================================


// ==========================================================================
// gmWaterSurfaceGameSystemGetStageId
/*!
 * ÉXÉeÅ[ÉWIDÇéÊìæ
 *
 * @retun ÉXÉeÅ[ÉWID
 */
// ==========================================================================
GSE_MAIN_STAGE_ID gmWaterSurfaceGameSystemGetStageId( void )
{
	return (GSE_MAIN_STAGE_ID)g_gs_main_sys_info.stage_id;
}

// ==========================================================================
// gmWaterSurfaceGameSystemGetZoneType
/*!
 * É]Å[ÉìÉ^ÉCÉvÇéÊìæ
 *
 * @param stage_id ÉXÉeÅ[ÉWID
 *
 * @return É]Å[ÉìÉ^ÉCÉv
 */
// ==========================================================================
GSE_MAIN_ZONE_TYPE gmWaterSurfaceGameSystemGetZoneType( GSE_MAIN_STAGE_ID stage_id )
{
	amAssert( GSD_MAIN_STAGE_ID_MAX > stage_id );

	return g_gm_gamedat_zone_type_tbl[stage_id];
}

// ==========================================================================
// gmWaterSurfaceGameSystemGetWaterLevel
/*!
 * ÉQÅ[ÉÄÉVÉXÉeÉÄÇ©ÇÁêÖñ ÉåÉxÉãÇéÊìæ
 *
 *	@return	êÖñ ÉåÉxÉã
 */
// ==========================================================================
u16 gmWaterSurfaceGameSystemGetWaterLevel( void )
{
	return g_gm_main_system.water_level;
}

// ==========================================================================
// gmWaterSurfaceGameSystemSetWaterLevel
/*!
 * ÉQÅ[ÉÄÉVÉXÉeÉÄÇ…êÖñ ÉåÉxÉãÇê›íË
 *
 *	@param 	water_level êÖñ ÉåÉxÉã
 */
// ==========================================================================
void gmWaterSurfaceGameSystemSetWaterLevel( u16 water_level )
{
	g_gm_main_system.water_level = water_level;
}

// ==========================================================================
// ÉfÅ[É^ä«óù
// ==========================================================================

// ==========================================================================
// gmWaterSurfaceDataInit
/*!
 * ÉfÅ[É^ä«óùèâä˙âª
 */
// ==========================================================================
void gmWaterSurfaceDataInit( void )
{
	amAssert(!g_water_surface_data);

	amZeroMemory(&g_water_surface_data_real, sizeof(GMS_WATER_SURFACE_DATA));
	g_water_surface_data = &g_water_surface_data_real;
}

// ==========================================================================
// gmWaterSurfaceDataRelease
/*!
 * ì«Ç›çûÇÒÇæÉfÅ[É^âï˙
 */
// ==========================================================================
void gmWaterSurfaceDataRelease( void )
{
	if ( g_water_surface_data ){
#if GMD_WATER_SURFACE_USE_RENDER
		//AMBâï˙
		gmWaterSurfaceDataReleaseAmbHeader();
#endif	// GMD_WATER_SURFACE_USE_RENDER

		g_water_surface_data = NULL;
	}
}

// ==========================================================================
// gmWaterSurfaceDataGetInfo
/*!
 * ÉfÅ[É^ä«óùÇéÊìæ
 *
 * @return  ÉfÅ[É^ä«óù
 */
// ==========================================================================
GMS_WATER_SURFACE_DATA* gmWaterSurfaceDataGetInfo( void )
{
	return g_water_surface_data;
}

// ==========================================================================
// gmWaterSurfaceDataSetAmbHeader
/*!
 * ÉfÅ[É^ambÇê›íË
 *
 * @param amb  ÉfÅ[É^amb
 */
// ==========================================================================
void gmWaterSurfaceDataSetAmbHeader( AMS_AMB_HEADER* amb )
{
	GMS_WATER_SURFACE_DATA* data = gmWaterSurfaceDataGetInfo();
	amAssert(data);
	amAssert(!data->amb_header);

	data->amb_header = amb;
}

// ==========================================================================
// gmWaterSurfaceDataGetAmbHeader
/*!
 * ÉfÅ[É^ambÇéÊìæ
 *
 * @return ÉfÅ[É^amb
 */
// ==========================================================================
AMS_AMB_HEADER* gmWaterSurfaceDataGetAmbHeader( void )
{
	GMS_WATER_SURFACE_DATA* data = gmWaterSurfaceDataGetInfo();
	amAssert(data);

	return data->amb_header;
}

// ==========================================================================
// gmWaterSurfaceDataReleaseAmbHeader
/*!
 * ÉfÅ[É^ambÇâï˙
 */
// ==========================================================================
void gmWaterSurfaceDataReleaseAmbHeader( void )
{
	GMS_WATER_SURFACE_DATA* data = gmWaterSurfaceDataGetInfo();
	amAssert(data);

	if ( data->amb_header ){
		mtMemFreeMain( data->amb_header );
		data->amb_header = NULL;
	}
}

// ==========================================================================
// êÖñ ä«óù
// ==========================================================================

// ==========================================================================
// gmWaterSurfaceGetMgr
/*!
 * êÖñ ä«óùéÊìæ
 *
 * @retun êÖñ ä«óù
 */
// ==========================================================================
GMS_WATER_SURFACE_MGR* gmWaterSurfaceGetMgr( void )
{
	return g_water_surface_mgr;
}

// ==========================================================================
// gmWaterSurfaceInitMgr
/*!
 * êÖñ ä«óùèâä˙âª
 */
// ==========================================================================
void gmWaterSurfaceInitMgr( void )
{
	amAssert(!g_water_surface_mgr);

	amZeroMemory(&g_water_surface_mgr_real, sizeof(GMS_WATER_SURFACE_MGR));
	g_water_surface_mgr = &g_water_surface_mgr_real;
}

// ==========================================================================
// gmWaterSurfaceExitMgr
/*!
 * êÖñ ä«óùçÌèú
 */
// ==========================================================================
void gmWaterSurfaceExitMgr( void )
{
	if ( g_water_surface_mgr ){
		//êÖñ TCB
		gmWaterSurfaceDeleteTcb();

		g_water_surface_mgr = NULL;
	}
}

// ==========================================================================
//êÖñ TCB
// ==========================================================================

// ==========================================================================
// gmWaterSurfaceCreateTcb
/*!
 * êÖñ ï`âÊTCBÇçÏê¨
 *
 * @return êÖñ ï`âÊTCB
 */
// ==========================================================================
MTS_TASK_TCB* gmWaterSurfaceCreateTcb( void )
{
	GMS_WATER_SURFACE_MGR* mgr = gmWaterSurfaceGetMgr();
	amAssert(mgr);
	
	//ä˘Ç…çÏê¨çœÇ›
	amAssert( !mgr->tcb_water );

	//çÏê¨
	mgr->tcb_water = MTM_TASK_MAKE_TCB(
		gmWaterSurfaceProc,
		NULL,
		0,							//flag
		GMD_TASK_PAUSELEVEL_DEF,	//pause_level
		GMD_TASK_PRIO_WATER_SURFACE,
		GMD_TASK_GROUP_DECO_SYS,
		sizeof(GMS_WATER_SURFACE_INFO),							//work_size
		"GM WATER SURFACE" );
	amAssert( mgr->tcb_water );

	GMS_WATER_SURFACE_INFO* work = (GMS_WATER_SURFACE_INFO*)mtTaskGetTcbWork(mgr->tcb_water);
	work->now_water_level = gmWaterSurfaceGameSystemGetWaterLevel();
	work->next_water_level = 0xFFFF;
	work->water_time = 0;
	work->water_counter = 0;	
	work->flag_draw = TRUE;
	work->flag_enable_ref = TRUE;

	return mgr->tcb_water;
}

// ==========================================================================
// gmWaterSurfaceDeleteTcb
/*!
 * êÖñ ï`âÊTCBÇçÌèú
 *
 */
// ==========================================================================
void gmWaterSurfaceDeleteTcb( void )
{
	GMS_WATER_SURFACE_MGR* mgr = gmWaterSurfaceGetMgr();
	amAssert(mgr);
	if ( mgr->tcb_water ){
		mtTaskClearTcb( mgr->tcb_water );
		mgr->tcb_water = NULL;
	}
}

#if GMD_WATER_SURFACE_USE_RENDER

// ==========================================================================
// gmWaterSurfaceTcbProcPreDrawDT
/*!
 * ï`âÊëOèàóùÉÜÅ[ÉUä÷êî
 *
 */
// ==========================================================================
void gmWaterSurfaceTcbProcPreDrawDT(void *data)
{
	UNREFERENCED_PARAMETER( data );
	
	AMS_RENDER_TARGET* target = _am_render_manager.targetp;
	amAssert( target );

	if ( target == &_gm_mapFar_render_work ){
		target = &_am_draw_target;
	}
	else {
		target = &_gm_mapFar_render_work;
	}

	//ÉåÉìÉ_É^Å[ÉQÉbÉgÇ™çÏê¨Ç≥ÇÍÇƒÇ¢Ç»Ç¢
	if ( target->width == 0 ){
		return;
	}
	amDrawEndScene();

	NNS_RGBA_U8 color = {0x00, 0x00, 0x00, 0xFF};	//ÉåÉìÉ_Å[É^Å[ÉQÉbÉgÉJÉâÅ[
	amRenderCopyTarget( target, &color );

	//ÉåÉìÉ_É^Å[ÉQÉbÉgê›íË
	GMS_WATER_SURFACE_MGR* mgr = gmWaterSurfaceGetMgr();
	if (mgr){
		mgr->render_target = target;
	}
}

// ==========================================================================
// gmWaterSurfaceTcbProcDrawDT
/*!
 * ï`âÊèàóùÉÜÅ[ÉUä÷êî
 *
 */
// ==========================================================================
void gmWaterSurfaceTcbProcDrawDT(void *data)
{
	UNREFERENCED_PARAMETER( data );
	
	//ÉåÉìÉ_É^Å[ÉQÉbÉgéÊìæ
	GMS_WATER_SURFACE_MGR* mgr = gmWaterSurfaceGetMgr();

	//ÉåÉìÉ_É^Å[ÉQÉbÉgÇ™çÏê¨Ç≥ÇÍÇƒÇ¢Ç»Ç¢
	if ( !mgr || !mgr->render_target ||  mgr->render_target->width == 0 ){
		return;
	}

	dwaterDrawWater( mgr->render_target );
}

// ==========================================================================
// gmWaterSurfaceTcbProcPostDrawDT
/*!
 * êÖï`âÊå„èàóùÉÜÅ[ÉUä÷êî
 *
 */
// ==========================================================================
void gmWaterSurfaceTcbProcPostDrawDT(void *data)
{
	UNREFERENCED_PARAMETER( data );

#if _PC | _XBOX
	LPDIRECT3DDEVICE9 d3ddev;
#if _PC
	d3ddev = amWinDxGetDirect3DDevice();
#elif _XBOX
	d3ddev = amXboxDxGetDirect3DDevice();
#endif
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	d3ddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
#endif
}

#endif	// GMD_WATER_SURFACE_USE_RENDER

// ==========================================================================
// gmWaterSurfaceProc
/*!
 * êÖñ èàóùÉvÉçÉVÅ[ÉWÉÉ
 *
 */
// ==========================================================================
void gmWaterSurfaceProc( MTS_TASK_TCB *tcb )
{
	GMS_WATER_SURFACE_INFO* work = (GMS_WATER_SURFACE_INFO*)mtTaskGetTcbWork(tcb);
	amAssert( work );

	Float frame_speed = 0;
	if ( !ObjObjectPauseCheck(0) ){

		//-----------------------------------------------
		//êÖñ ÉåÉxÉãïœçX
		//-----------------------------------------------
		Float time = (Float)(work->water_time - work->water_counter);
		if ( time != 0 ){
			Float add_level = (work->next_water_level - work->now_water_level) / time;

			work->now_water_level += add_level;
			
			//éwíËÉåÉxÉãÇ…ìûíB
			if ( 1 > (u16)MTM_MATH_ABS(work->now_water_level - work->next_water_level) ){
				work->water_time = 0;
				work->water_counter = 0;
			}
			//ñ¢ìûíB
			else{
				++work->water_counter;
			}
		}
		else{
			work->now_water_level = work->next_water_level;
		}
		
		//ï`âÊÉtÉâÉOÉ`ÉFÉbÉN
		if ( work->flag_draw ){
			gmWaterSurfaceGameSystemSetWaterLevel( (u16)work->now_water_level );
		}
		else{
			//ï`âÊÇµÇ»Ç¢Ç∆Ç´ÇÕ0xFFFFÇê›íËÇ∑ÇÈ
			gmWaterSurfaceGameSystemSetWaterLevel( 0xFFFF );
			return;
		}

		//çXêVë¨ìx
		frame_speed = amSystemGetFrameRateMain();

#if GMD_WATER_SURFACE_USE_RENDER
	}
	// ÉJÉÅÉâÇ∆ÇÃãóó£
	OBS_CAMERA* obj_camera = ObjCameraGet( GME_CAMERA_NO_WATER );

	Float x = obj_camera->disp_pos.x;
	Float y = obj_camera->disp_pos.y;
	Float height =  -work->now_water_level - y;

	//âÊñ äO
	if ( height < -(OBD_LCD_Y/2+32) ){
		GMS_WATER_SURFACE_MGR* mgr = gmWaterSurfaceGetMgr();
		if (mgr){
			mgr->render_target = NULL;
		}
		return;
	}

	//ÉãÅ[Évèàóù
	BOOL flag_loop = FALSE;
	if ( height > OBD_LCD_Y*0.8f ){
		y = obj_camera->disp_pos.y;
		height = OBD_LCD_Y*0.8f;
		flag_loop = TRUE;
	}
	Angle32 roll = obj_camera->roll;
	Float scale = 1.0f/obj_camera->scale;

	//-----------------------------------------------
	//êÖñ ï`âÊ
	//-----------------------------------------------
	
	//dwaterSetColor(pWk->Ambient.r, pWk->Ambient.g, pWk->Ambient.b);

	// êÖñ ÇÃçXêV
	dwaterUpdate(
			frame_speed,
			x, y, height,
			roll, scale);
	
	if (!GmMainIsDrawEnable()) {
		return;
	}
	
	dwaterSetParam();

	NNF_DRAWOBJ draw_flag = (NNF_DRAWOBJ)0;
#if (_PC | _XBOX | _PS3)
	draw_flag = NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND;
#endif	//(_PC | _XBOX | _PS3)

#if (_IPHONE & 1)
	//----------------------------------------------------------------
	//OBD_DRAW_CMD_STATE_PRE_WATERÇÃèàóù
	//----------------------------------------------------------------
	//ÉJÉÅÉâê›íË
	ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_WATER, NNE_PROJECTION_TYPE_ORTHO, OBD_DRAW_CMD_STATE_PRE_WATER );

	//îΩéÀ
#if 0//_IPHONE
	// iPhoneÇÕÉtÉFÅ[ÉhíÜÇ…êÖñ ÇèëÇ≠Ç∆ïœÇ…Ç»ÇÈÇÃÇ≈ñ≥å¯âªÇ∑ÇÈ
	if ( work->flag_enable_ref && (!flag_loop) && !IzFadeIsExe()){
#else
	if ( work->flag_enable_ref && (!flag_loop)){
#endif // _IPHONE
		dwaterDrawReflection( OBD_DRAW_CMD_STATE_PRE_WATER, draw_flag );
	}
	//ÉåÉìÉ_èàóù
	ObjDraw3DNNUserFunc( gmWaterSurfaceTcbProcPreDrawDT, NULL, 0, OBD_DRAW_CMD_STATE_PRE_WATER );	

	//ÉJÉÅÉâê›íËñﬂÇ∑
	ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, NNE_PROJECTION_TYPE_ORTHO, OBD_DRAW_CMD_STATE_PRE_WATER );
#endif // !_IPHONE

	//----------------------------------------------------------------
	//OBD_DRAW_CMD_STATE_WATERÇÃèàóù
	//----------------------------------------------------------------
	//ÉJÉÅÉâê›íË
	ObjDraw3DNNSetCameraEx(GME_CAMERA_NO_WATER, NNE_PROJECTION_TYPE_ORTHO, OBD_DRAW_CMD_STATE_WATER );

	//êÖ
#if _IPHONE
	// iPhoneêÍópêÖèàóù
	AMS_PARAM_DRAW_PRIMITIVE param;
	
	param.aTest = 0;
	param.zMask = 1; // åªç›ÇÕZèëÇ´çûÇ›ÇçsÇÌÇ»Ç¢
	param.zTest = 1;
	
	//ÉAÉãÉtÉ@
	param.ablend = NNE_PRIM_ALPHABLEND_ON;
	
#if _PC | _XBOX
	param.bldSrc = NNE_BLENDMODE_SRCALPHA;
	param.bldDst = NNE_BLENDMODE_DSTALPHA;
	param.bldMode = NNE_BLENDOP_ADD;
#elif _PS3
    param.bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
	param.bldDst = NND_BLENDFUNC_PS3_DST_ALPHA;
	param.bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
	param.bldSrc = GX_BL_SRCALPHA;
	param.bldDst = GX_BL_DSTALPHA;
	param.bldMode = GX_BM_BLEND;
#elif _IPHONE
    param.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
	param.bldDst = NND_BLENDFUNC_GL_ONE;
//	param.bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
	param.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
	
    // ÉeÉNÉXÉ`ÉÉÇ»Çµ
	NNS_PRIM3D_PCT* poli_data = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6 * GMD_WATER_SURFACE_DRAW_NUM);
	
	
	// ÉJÉÅÉâÇ∆ÇÃãóó£
	OBS_CAMERA* player_camera = ObjCameraGet( 0 );
	x = player_camera->disp_pos.x;
	y = -work->now_water_level;	//Yé≤ÇÃå¸Ç´Ç™ãt
	Float z = FX_FX32_TO_F32( GMD_OBJ_DEFAULT_POS_Z_N_FRONT+32*FX32_ONE);//player_camera->disp_pos.z;
	Float length = player_camera->znear + 1.0f; // ZNearÇÃÉNÉäÉbÉvÇ…à¯Ç¡Ç©Ç©ÇÁÇ»Ç¢ÇÊÇ§í≤êÆ
	
	Float width = GMD_WATER_SURFACE_DRAW_WIDTH;
	height =  y - (player_camera->disp_pos.y - GMD_WATER_SURFACE_DRAW_HIEGHT);
	
	
	
    // ÉJÉâÅ[
#if GMD_WATER_COLOR_CONTROL
	static int color_pal[4] = {0x20, 0xB0, 0x40, 0x40};
	static int color_index = 0;
	
	if (PAD_STAND(0) & KEY_L_UP) {
		color_index = (color_index + 1) % 4;
	}
	if (PAD_STAND(0) & KEY_L_DOWN) {
		color_index = (color_index + 3) % 4;
	}
	if (PAD_REPEAT(0) & KEY_L_LEFT) {
		color_pal[color_index] = (color_pal[color_index] + 8) % 256;
	}
	if (PAD_REPEAT(0) & KEY_L_RIGHT) {
		color_pal[color_index] = (color_pal[color_index] + 248) % 256;
	}
	
	NNS_RGBA8888 color = AMD_RGBA8888(color_pal[0], color_pal[1], color_pal[2], color_pal[3]);
	amPrintf(4, 4, "%d :color %x \n", color_index, color);
#else // GMD_WATER_COLOR_CONTROL
	//	NNS_RGBA8888 color = AMD_RGBA8888( 0x20, 0xB0, 0x40, 0x40);
	NNS_RGBA8888 color = AMD_RGBA8888( 0xff, 0xff, 0xff, 0x60);
#endif // GMD_WATER_COLOR_CONTROL
	//	if (amTpIsTouchOn(0)) color |= 0xff;
	
    // í∏ì_
	for (int i = 0; i < GMD_WATER_SURFACE_DRAW_NUM; i++) {
		Float add_length = length * (float)(i + 1);
		amVectorSet(
					(NNS_VECTOR*)&poli_data[0 + i * 6], 
					x - width, 
					y, 
					z - add_length);
		amVectorSet(
					(NNS_VECTOR*)&poli_data[1 + i * 6], 
					x + width, 
					y, 
					z - add_length );
		amVectorSet(
					(NNS_VECTOR*)&poli_data[2 + i * 6], 
					x - width, 
					y - height, 
					z - add_length );
		amVectorSet(
					(NNS_VECTOR*)&poli_data[5 + i * 6], 
					x + width, 
					y - height, 
					z - add_length );
		
		
		// ÉJÉâÅ[
		poli_data[0 + i * 6].Col = color;
		poli_data[1 + i * 6].Col = color;
		poli_data[2 + i * 6].Col = color;
		poli_data[5 + i * 6].Col = color;
	
		// UV
		float tex_u[2] = {0.0f,0.0f};
		float tex_v[2] = {0.0f,0.0f};
		float scale_v_sin, scale_v_cos;
		nnSinCos(NNM_DEGtoA32(_dmap_water->speed_surface * 360.0f), &scale_v_sin, &scale_v_cos);
		
		float size_w = width*2.0f/GMD_WATER_SURFACE_DRAW_TEX_SIZE*GMD_WATER_SURFACE_DRAW_TEX_ADJUST_SIZE;
		//ÉLÉâÉLÉâñ ÇPñáñ⁄
		if ( i == 0 ){
			tex_u[0] = x / 270.0f ;
			tex_u[0] -= (float)(s32)tex_u[0] - _dmap_water->speed_surface;
			tex_u[1] = tex_u[0] + size_w;
			
			tex_v[0] = GMD_WATER_SURFACE_DRAW_TEX_OFFSET_V;
			tex_v[1] = tex_v[0] + height/GMD_WATER_SURFACE_DRAW_TEX_SIZE*GMD_WATER_SURFACE_DRAW_TEX_ADJUST_SIZE + scale_v_cos / 5.0f;
		}
		//ÉLÉâÉLÉâñ 2ñáñ⁄
		else if ( i == 1 ){
			tex_u[1] = x / 270.0f;
			tex_u[1] -= (float)(s32)tex_u[1] + _dmap_water->speed_surface*2.0f;
			tex_u[0] = tex_u[1] - size_w*0.75f;
			
			tex_v[0] = 1.0f;
			tex_v[1] = tex_v[0] - height*0.75f/GMD_WATER_SURFACE_DRAW_TEX_SIZE*GMD_WATER_SURFACE_DRAW_TEX_ADJUST_SIZE + scale_v_sin / 5.0f;
		}
		//íPêFñ 
		else{
			//ÉeÉNÉXÉ`ÉÉÇ©ÇÁ1ÉhÉbÉgíäèoÇµÇƒíPêFï`âÊÅv
			tex_u[0] = GMD_WATER_SURFACE_DRAW_TEX_BASE_SURFACE_U/GMD_WATER_SURFACE_DRAW_TEX_SIZE;
			tex_u[1] = tex_u[0] + 1.0f/GMD_WATER_SURFACE_DRAW_TEX_SIZE;
			tex_v[0] = GMD_WATER_SURFACE_DRAW_TEX_BASE_SURFACE_V/GMD_WATER_SURFACE_DRAW_TEX_SIZE;
			tex_v[1] = tex_v[0] + 1.0f/GMD_WATER_SURFACE_DRAW_TEX_SIZE;
			
			poli_data[0 + i * 6].Col = AMD_RGBA8888( 0x20, 0xB0, 0x40, 0x70);
			poli_data[1 + i * 6].Col = poli_data[0 + i * 6].Col;
			poli_data[2 + i * 6].Col = poli_data[0 + i * 6].Col;
			poli_data[5 + i * 6].Col = poli_data[0 + i * 6].Col;
		}
		poli_data[0 + i * 6].Tex.u = tex_u[0];
		poli_data[0 + i * 6].Tex.v = tex_v[0];
		poli_data[1 + i * 6].Tex.u = tex_u[1];
		poli_data[1 + i * 6].Tex.v = tex_v[0];
		poli_data[2 + i * 6].Tex.u = tex_u[0];
		poli_data[2 + i * 6].Tex.v = tex_v[1];
		poli_data[5 + i * 6].Tex.u = tex_u[1];
		poli_data[5 + i * 6].Tex.v = tex_v[1];
		
		poli_data[3 + i * 6] = poli_data[1 + i * 6];
		poli_data[4 + i * 6] = poli_data[2 + i * 6];
	}
	
	// ÉvÉäÉ~ÉeÉBÉuï`âÊê›íË
	param.format3D = NNE_PRIM3D_FMT_PCT;
	param.type = NNE_PRIM_TRIANGLE_LIST;
	param.vtxPCT3D = poli_data;
	param.count = 6 * GMD_WATER_SURFACE_DRAW_NUM;
	param.sortZ = -length;
	
	param.texId = 0;
	param.texlist = _dmap_water->tex_color.texlist;
//	param.texlist = _dmap_water->object[1].texlist;
	param.uwrap = NNE_PRIM_TEXWRAP_REPEAT;
	param.vwrap = NNE_PRIM_TEXWRAP_REPEAT;
	
	//É}ÉgÉäÉNÉXÉvÉbÉVÉÖÉRÉ}ÉìÉh
	gmWaterSurfaceMatrixPush( OBD_DRAW_CMD_STATE_WATER );
	
	//ï`âÊÉRÉ}ÉìÉh
	ObjDraw3DNNDrawPrimitive(&param, OBD_DRAW_CMD_STATE_WATER);
	//	amDrawPrimitive3D( OBD_DRAW_CMD_STATE_WATER, &param );
	
	//É}ÉgÉäÉNÉXÉ|ÉbÉvÉRÉ}ÉìÉh
	gmWaterSurfaceMatrixPop( OBD_DRAW_CMD_STATE_WATER );
#else
	ObjDraw3DNNUserFunc( gmWaterSurfaceTcbProcDrawDT, NULL, 0, OBD_DRAW_CMD_STATE_WATER );
#endif // _IPHONE

	if ( !flag_loop ){
		//êÖñ 
		dwaterDrawSurface( OBD_DRAW_CMD_STATE_WATER, draw_flag );
	}
	
	//ÉJÉÅÉâÇñﬂÇ∑
	ObjDraw3DNNSetCameraEx(g_obj.glb_camera_id, NNE_PROJECTION_TYPE_ORTHO, OBD_DRAW_CMD_STATE_WATER );

	//êÖå„èàóù
	ObjDraw3DNNUserFunc( gmWaterSurfaceTcbProcPostDrawDT, NULL, 0, OBD_DRAW_CMD_STATE_POST_WATER );
	
#else	//GMD_WATER_SURFACE_USE_RENDER
	}

	AMS_PARAM_DRAW_PRIMITIVE param;

	param.aTest = 0;
	param.zMask = 0;
	param.zTest = 1;

	//ÉAÉãÉtÉ@
	param.ablend = NNE_PRIM_ALPHABLEND_ON;

#if _PC | _XBOX
	param.bldSrc = NNE_BLENDMODE_SRCALPHA;
	param.bldDst = NNE_BLENDMODE_DSTALPHA;
	param.bldMode = NNE_BLENDOP_ADD;
#elif _PS3
    param.bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
	param.bldDst = NND_BLENDFUNC_PS3_DST_ALPHA;
	param.bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
	param.bldSrc = GX_BL_SRCALPHA;
	param.bldDst = GX_BL_DSTALPHA;
	param.bldMode = GX_BM_BLEND;
#elif _IPHONE
    param.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
	param.bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
	param.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif

    // ÉeÉNÉXÉ`ÉÉÇ»Çµ
	NNS_PRIM3D_PC* poli_data = (NNS_PRIM3D_PC *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PC) * 6);
//	NNS_PRIM3D_PCT* poli_data = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6);


	// ÉJÉÅÉâÇ∆ÇÃãóó£
	OBS_CAMERA* player_camera = ObjCameraGet( 0 );
	Float x = player_camera->disp_pos.x;
	//float y = player_camera->disp_pos.y;
	Float y = -work->now_water_level;	//Yé≤ÇÃå¸Ç´Ç™ãt
	Float z = FX_FX32_TO_F32( GMD_OBJ_DEFAULT_POS_Z_N_FRONT+32*FX32_ONE);//player_camera->disp_pos.z;
	Float length = player_camera->znear + 1.0f; // ZNearÇÃÉNÉäÉbÉvÇ…à¯Ç¡Ç©Ç©ÇÁÇ»Ç¢ÇÊÇ§í≤êÆ

	Float width = GMD_WATER_SURFACE_DRAW_WIDTH;
	Float height =  y - (player_camera->disp_pos.y - GMD_WATER_SURFACE_DRAW_HIEGHT);

    // í∏ì_
	amVectorSet(
			(NNS_VECTOR*)&poli_data[0], 
			x - width, 
			y, 
			z - length );
	amVectorSet(
			(NNS_VECTOR*)&poli_data[1], 
			x + width, 
			y, 
			z - length );
    amVectorSet(
			(NNS_VECTOR*)&poli_data[2], 
			x - width, 
			y - height, 
			z - length );
    amVectorSet(
			(NNS_VECTOR*)&poli_data[5], 
			x + width, 
			y - height, 
			z - length );

    // ÉJÉâÅ[
	NNS_RGBA8888 color = AMD_RGBA8888( 0x40, 0x80, 0x60, 0x80);
    poli_data[0].Col = color;
    poli_data[1].Col = color;
	color = AMD_RGBA8888( 0x10, 0x20, 0x18, 0x80);
    poli_data[2].Col = color;
    poli_data[5].Col = color;
/*
poli_data[0].Tex.u = 0.0f;
poli_data[0].Tex.v = 0.0f;
poli_data[1].Tex.u = 1.0f;
poli_data[1].Tex.v = 0.0f;
poli_data[2].Tex.u = 0.0f;
poli_data[2].Tex.v = 1.0f;
poli_data[5].Tex.u = 1.0f;
poli_data[5].Tex.v = 1.0f;
*/
    poli_data[3] = poli_data[1];
    poli_data[4] = poli_data[2];

	// ÉvÉäÉ~ÉeÉBÉuï`âÊê›íË
	param.format3D = NNE_PRIM3D_FMT_PC;
//	param.format3D = NNE_PRIM3D_FMT_PCT;
	param.type = NNE_PRIM_TRIANGLE_LIST;
	param.vtxPC3D = poli_data;
//	param.vtxPCT3D = poli_data;
	param.texlist = NULL;
	param.texId = 0;
	param.count = 6;
	param.sortZ = -length;

//param.texId = 2;
//param.texlist = texlist;

	//É}ÉgÉäÉNÉXÉvÉbÉVÉÖÉRÉ}ÉìÉh
	gmWaterSurfaceMatrixPush( OBD_DRAW_CMD_STATE_WATER );
	
	//ï`âÊÉRÉ}ÉìÉh
	amDrawPrimitive3D( OBD_DRAW_CMD_STATE_WATER, &param );

	//É}ÉgÉäÉNÉXÉ|ÉbÉvÉRÉ}ÉìÉh
	gmWaterSurfaceMatrixPop( OBD_DRAW_CMD_STATE_WATER );

#endif	//GMD_WATER_SURFACE_USE_RENDER
}

// ==========================================================================
// gmWaterSurfaceMatrixPush
/*!
 *	É}ÉgÉäÉNÉXÉvÉbÉVÉÖÉRÉ}ÉìÉh
 */
// ==========================================================================
void gmWaterSurfaceMatrixPush( u32 command_state )
{
	ObjDraw3DNNUserFunc( 
			gmWaterSurfaceUserFuncMatrixPush,
			NULL, 
			0, 
			command_state);
}

// ==========================================================================
// gmWaterSurfaceMatrixPop
/*!
 *	É}ÉgÉäÉNÉXÉ|ÉbÉvÉRÉ}ÉìÉh
 */
// ==========================================================================
void gmWaterSurfaceMatrixPop( u32 command_state )
{
	ObjDraw3DNNUserFunc(
		gmWaterSurfaceUserFuncPop,
		NULL, 
		0, 
		command_state);
}

// ==========================================================================
// gmWaterSurfaceUserFuncMatrixPush
/*!
 *	É}ÉgÉäÉNÉXÉvÉbÉVÉÖÉRÉ}ÉìÉh
 */
// ==========================================================================
void gmWaterSurfaceUserFuncMatrixPush( void* param )
{
	UNREFERENCED_PARAMETER( param );
	
	amMatrixPush();
	
	NNS_MATRIX* current_matrix = amMatrixGetCurrent();	

	NNS_MATRIX matrix;
	nnMultiplyMatrix( &matrix, amDrawGetWorldViewMatrix(), current_matrix );
	
	// 3DÉvÉäÉ~ÉeÉBÉuï`âÊópÉ}ÉgÉäÉbÉNÉXÇÉZÉbÉgÅiÉrÉÖÅ[É}ÉgÉäÉNÉXçûÇ›Åj
	nnSetPrimitive3DMatrix( &matrix );
}

// ==========================================================================
// gmWaterSurfaceUserFuncPop
/*!
 *	É}ÉgÉäÉNÉXÉ|ÉbÉvÉRÉ}ÉìÉh
 */
// ==========================================================================
void gmWaterSurfaceUserFuncPop( void* param )
{
	UNREFERENCED_PARAMETER( param );

	amMatrixPop();
}



// ==========================================================================
//ÉåÉìÉ_êÖñ 
// ==========================================================================

#if GMD_WATER_SURFACE_USE_RENDER

/*****************************************************************************/
/* Sint32 dwaterInit(void)                                                   */
/*---------------------------------------------------------------------------*/
/* [RETURN]  ìoò^äÆóπÇÉ`ÉFÉbÉNÇ∑ÇÈÇΩÇﬂÇÃìoò^ID                              */
/* [FUNCTION]  èâä˙âª                                                        */
/*****************************************************************************/
Sint32 dwaterInit(void)
{
	if (_dmap_water == NULL)
		_dmap_water		= (DMAP_WATER *)amMemAlloc(sizeof(DMAP_WATER));

	memset(_dmap_water, 0, sizeof(DMAP_WATER));

	_dmap_water->repeat_u	= 1.0f;
	_dmap_water->repeat_v	= 1.0f;
	_dmap_water->speed_u	= 0.001f;
	_dmap_water->speed_v	= -0.001f;
#if _IPHONE
	_dmap_water->speed_surface = 0.001f;
#endif // _IPHONE
	_dmap_water->regist_index	= -1;
	_dmap_water->repeat_pos_x	= 20.0f * 14.0f;

	dwaterSetColor(0.65f, 1.0f, 0.75f);

#if _PC | _XBOX
#if !GMD_WATER_PRECOMPILED
	size_t		code_size;
	code_size	= strlen(_dmap_draw_water_code);

	_dmap_water->regist_index	= amShaderBuild(
			_dmap_draw_water_code, code_size,
			_dmap_draw_water_code, code_size,
			(void **)&_dmap_water->shader_VS,
			(void **)&_dmap_water->shader_PS);
#else
	_dmap_water->regist_index	= amShaderBuild(
#if _PC
			(void *)g_vs30_draw_water_VS, (void *)g_ps30_draw_water_PS,
#elif _XBOX
			(void *)g_xvs_draw_water_VS,  (void *)g_xps_draw_water_PS,
#endif
			(void **)&_dmap_water->shader_VS,
			(void **)&_dmap_water->shader_PS);
#endif
#elif _PS3
	static NNS_SHADER_PARAM_PS3	shader_param_VS[] = {
		MAKE_NNS_SHADER_PARAM_PS3("u_ProjectionMatrix", 0),
		MAKE_NNS_SHADER_PARAM_PS3("u_Priority", 0),
		MAKE_NNS_SHADER_PARAM_PS3("u_Color", 0),
	};

#if !GMD_WATER_PRECOMPILED
	_dmap_water->regist_index	= amShaderBuild(
			_dmap_draw_water_code_VS, 0,
			_dmap_draw_water_code_PS, 0,
			&_dmap_water->shader_VS, &_dmap_water->shader_PS,
			shader_param_VS, NULL,
			sizeof(shader_param_VS) / sizeof(NNS_SHADER_PARAM_PS3),
			0);
#else
	_dmap_water->regist_index	= amShaderBuild(
			(void *)g_draw_water_VS, (void *)g_draw_water_PS,
			&_dmap_water->shader_VS, &_dmap_water->shader_PS,
			shader_param_VS, NULL,
			sizeof(shader_param_VS) / sizeof(NNS_SHADER_PARAM_PS3),
			0);
#endif
#elif _WII
	_dmap_water->regist_index	= amShaderBuild(
			(char *)NULL, 0, (char *)NULL, 0, (void **)NULL, (void **)NULL);
#endif

	return	_dmap_water->regist_index;
}


/*****************************************************************************/
/* void dwaterExit(void)                                                     */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  èIóπ                                                          */
/*****************************************************************************/
void dwaterExit(void)
{
	if (_dmap_water != NULL) {
		Sint32		i;
		for (i = 0; i < 2; i++) {
#if !_WII
			if (_dmap_water->object[i].object != NULL)
				amMemFree(_dmap_water->object[i].object);
#endif
			if (_dmap_water->object[i].texlistbuf != NULL)
				amMemFree(_dmap_water->object[i].texlistbuf);
			if (_dmap_water->object[i].motion != NULL)
				amMotionDelete(_dmap_water->object[i].motion);
		}
		amMemFree(_dmap_water);
	}

	_dmap_water		= NULL;
}


/*****************************************************************************/
/* void dwaterSetObjectAMB(AMS_AMB_HEADER *amb_obj, AMS_AMB_HEADER *amb_tex) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  amb_obj : ÉIÉuÉWÉFÉNÉgAMBÉtÉ@ÉCÉãÇ÷ÇÃÉ|ÉCÉìÉ^                    */
/*          amb_tex : ÉeÉNÉXÉ`ÉÉAMBÉtÉ@ÉCÉãÇ÷ÇÃÉ|ÉCÉìÉ^                      */
/* [FUNCTION]  ÉIÉuÉWÉFÉNÉgÅEÉeÉNÉXÉ`ÉÉÉtÉ@ÉCÉãÇÃê›íË                        */
/*****************************************************************************/
void dwaterSetObjectAMB(AMS_AMB_HEADER *amb_obj, AMS_AMB_HEADER *amb_tex)
{
	amAssert(_dmap_water);
	amAssert(amb_obj);

	_dmap_water->amb_object		= amb_obj;
	_dmap_water->amb_texture	= amb_tex;
}


/*****************************************************************************/
/* Sint32 dwaterLoadObject(NNF_DRAWOBJ objflag)                              */
/*---------------------------------------------------------------------------*/
/* [INPUT]  objflag : ÉVÉFÅ[É_Å[ìoò^éûÇ…égópÇ∑ÇÈÉIÉuÉWÉFÉNÉgÉtÉâÉO           */
/* [RETURN]  ÉIÉuÉWÉFÉNÉgÅEÉeÉNÉXÉ`ÉÉÅEÉVÉFÅ[É_Å[ÇÃìoò^Ç™çœÇÒÇæÇ©Ç«Ç§Ç©      */
/* [FUNCTION]  ÉIÉuÉWÉFÉNÉgÅEÉeÉNÉXÉ`ÉÉÅEÉVÉFÅ[É_Å[ÇÃìoò^                    */
/*****************************************************************************/
Sint32 dwaterLoadObject(NNF_DRAWOBJ objflag)
{
	amAssert(_dmap_water);
	amAssert(_dmap_water->amb_object);

	Sint32	ret = 1, i;
	void	*file;
	DMAP_WATER_OBJ		*waterobj;

	waterobj	= &_dmap_water->object[0];

	// ìoò^çœÇ›É`ÉFÉbÉN
	if (_dmap_water->regist_index != -1) {
		if (!amDrawIsRegistComplete(_dmap_water->regist_index))
			return	0;
		_dmap_water->regist_index	= -1;

		if (waterobj->object != NULL) {
			// É}ÉeÉäÉAÉãÉÇÅ[ÉVÉáÉì
			for (i = 0; i < 2; i++, waterobj++) {
				file		= amBindGet(_dmap_water->amb_object, 2 + i);
				waterobj->motion		= amMotionCreate(waterobj->object);
				amMotionMaterialRegistFile(waterobj->motion, 0, file);
				amMotionMaterialSet(waterobj->motion, 0);
				amMotionMaterialSetFrame(waterobj->motion,
						amMotionMaterialGetStartFrame(waterobj->motion, 0));
			}
			return	1;
		}
	}

	// ìoò^
	for (i = 0; i < 2; i++, waterobj++) {
		file	= amBindGet(_dmap_water->amb_object, i);
		_dmap_water->regist_index	= amObjectLoad(
				&waterobj->object, &waterobj->texlist, &waterobj->texlistbuf,
				file, objflag, NULL, _dmap_water->amb_texture);
		ret		= 0;
	}

	return	ret;
}


/*****************************************************************************/
/* Sint32 dwaterLoadTexture(void *water_image, Sint32 water_size,            */
/*                                     void *color_image, Sint32 color_size) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  water_image : ÉCÉìÉ_ÉCÉåÉNÉgÉeÉNÉXÉ`ÉÉÉCÉÅÅ[ÉW                   */
/*          water_size  : ÉCÉìÉ_ÉCÉåÉNÉgÉeÉNÉXÉ`ÉÉÉCÉÅÅ[ÉWÉfÅ[É^ÉTÉCÉY       */
/*          color_image : ê[ìxÉJÉâÅ[ÉeÉNÉXÉ`ÉÉÉCÉÅÅ[ÉW                       */
/*          color_size  : ê[ìxÉJÉâÅ[ÉeÉNÉXÉ`ÉÉÉCÉÅÅ[ÉWÉfÅ[É^ÉTÉCÉY           */
/* [RETURN]  ìoò^äÆóπÇÉ`ÉFÉbÉNÇ∑ÇÈÇΩÇﬂÇÃìoò^ID                              */
/* [FUNCTION]  ÉeÉNÉXÉ`ÉÉÇÃçÏê¨                                              */
/*****************************************************************************/
Sint32 dwaterLoadTexture(void *water_image, Sint32 water_size, void *color_image, Sint32 color_size)
{
#if _PC | _XBOX | _PS3
	amTextureLoad((void **)&_dmap_water->tex_water,
			water_image, water_size, AMD_FILTER_LINEAR, AMD_FILTER_LINEAR,
			AMD_WRAP_REPEAT, AMD_WRAP_REPEAT);
	return	amTextureLoad((void **)&_dmap_water->tex_color,
			color_image, color_size, AMD_FILTER_LINEAR, AMD_FILTER_LINEAR,
			AMD_WRAP_CLAMP, AMD_WRAP_CLAMP);
#elif _WII
	amTextureLoad((void **)&_dmap_water->tex_water,
			water_image, water_size, AMD_FILTER_LINEAR, AMD_FILTER_LINEAR,
			AMD_WRAP_REPEAT, AMD_WRAP_REPEAT, &_dmap_water->gvrobj_water);
	return	amTextureLoad((void **)&_dmap_water->tex_color,
			color_image, color_size, AMD_FILTER_LINEAR, AMD_FILTER_LINEAR,
			AMD_WRAP_CLAMP, AMD_WRAP_CLAMP, &_dmap_water->gvrobj_color);
#elif _IPHONE
#endif
}


/*****************************************************************************/
/* Sint32 dwaterRelease(void)                                                */
/*---------------------------------------------------------------------------*/
/* [RETURN]  âï˙äÆóπÇÉ`ÉFÉbÉNÇ∑ÇÈÇΩÇﬂÇÃìoò^ID(-1Ç»ÇÁÇ»Çµ)                  */
/* [FUNCTION]  ÉäÉ\Å[ÉXÇÃâï˙                                                */
/*****************************************************************************/
Sint32 dwaterRelease(void)
{
	Sint32		regindex = -1;

	if (_dmap_water != NULL) {
		Sint32		i;
		for (i = 0; i < 2; i++) {
			if (_dmap_water->object[i].object != NULL) {
				regindex	= amObjectRelease(_dmap_water->object[i].object,
						_dmap_water->object[i].texlist);
			}
		}
#if _PC | _XBOX | _PS3
#if !_PS3	// PS3ÇæÇØâï˙Ç∑ÇÈÇ∆é~Ç‹ÇÈÇÃÇ≈ÉäÅ[ÉNÇ∑ÇÈÇ™é~Ç‹ÇÁÇ»Ç¢ÇÊÇ§Ç…Ç∑ÇÈ
		if (_dmap_water->tex_color != NULL)
			regindex	= amTextureRelease((void *)_dmap_water->tex_color);
		if (_dmap_water->tex_water != NULL)
			regindex	= amTextureRelease((void *)_dmap_water->tex_water);
#endif
		if (_dmap_water->shader_VS != NULL)
			amShaderRelease(_dmap_water->shader_VS, _dmap_water->shader_PS);
#elif _WII
		amTextureRelease((void *)&_dmap_water->tex_water);
		regindex	= amTextureRelease((void *)&_dmap_water->tex_color);
#endif
	}

	return	regindex;
}


/*****************************************************************************/
/* void dwaterSetColor(float r, float g, float b)                            */
/*---------------------------------------------------------------------------*/
/* [INPUT]  r, g, b : êÖÇÃêF                                                 */
/* [FUNCTION]  êÖÇÃêFÇÃê›íË                                                  */
/*****************************************************************************/
void dwaterSetColor(float r, float g, float b)
{
	Uint32		color_r, color_g, color_b, color_a;

	color_r		= (Uint32)(r * 255.0f) & 0xff;
	color_g		= (Uint32)(g * 255.0f) & 0xff;
	color_b		= (Uint32)(b * 255.0f) & 0xff;
	color_a		= 0xff;

#if _PC | _XBOX | _WII
	// ARGB
	_dmap_water->color	= (color_r << 16) | (color_g << 8) | (color_b << 0)
						| (color_a << 24);
#elif _PS3
	// RGBA
	_dmap_water->color	= (color_r << 24) | (color_g << 16) | (color_b << 8)
						| (color_a << 0);
#endif
}


/*****************************************************************************/
/* void dwaterUpdate(float speed, float pos_x, float pos_y, float dy,        */
/*                                               Angle32 rot_z, float scale) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  speed : çXêVÉtÉåÅ[ÉÄ                                             */
/*          pos_x : äÓèÄXç¿ïW                                                */
/*          pos_y : äÓèÄYç¿ïW                                                */
/*          dy    : âÊñ è„êÖà                                                */
/*          rot_z : âÒì]äp                                                   */
/*          scale : ÉXÉPÅ[Éã                                                 */
/* [FUNCTION]  UVÉXÉNÉçÅ[ÉãÅEÉAÉjÉÅÅ[ÉVÉáÉìÇÃçXêV                            */
/*****************************************************************************/
void dwaterUpdate(float speed, float pos_x, float pos_y, float dy, Angle32 rot_z, float scale)
{
//	static float	scale1 = 0.078125f;	// ÉèÉCÉhéûÇ…â°5ÉuÉçÉbÉNï™Ç™Ç±ÇÍÇÆÇÁÇ¢
	static float	scale1 = 1.0f / (20.0f * 7.5f);	// ÉèÉCÉhéûÇ…â°7.5ÉuÉçÉbÉNï™
	float	spd_x;
	float	f;
	
#if _IPHONE
	_dmap_water->speed_surface += 0.0005f;
	if (_dmap_water->speed_surface > 1.0f) {
		_dmap_water->speed_surface -= 1.0f;
	}
#endif // _IPHONE

	_dmap_water->pos_x		= pos_x;
	_dmap_water->pos_y		= pos_y;
	_dmap_water->pos_dy		= dy;
	_dmap_water->rot_z		= rot_z;
	_dmap_water->scale		= scale;

	// ÉeÉNÉXÉ`ÉÉUVÉAÉjÉÅÅ[ÉVÉáÉì
	f		= _dmap_water->ofst_u + _dmap_water->speed_u * speed;
	if (f >= 0.0f)
		f		-= floorf(f);
	else
		f		= 1.0f - ((-f) - floorf(-f));
	_dmap_water->ofst_u		= f;

	f		= _dmap_water->ofst_v + _dmap_water->speed_v * speed;
	if (f >= 0.0f)
		f		-= floorf(f);
	else
		f		= 1.0f - ((-f) - floorf(-f));
	_dmap_water->ofst_v		= f;

	// UVÉIÉtÉZÉbÉg(íÜêSäÓèÄ)
	spd_x	= scale1 * _dmap_water->repeat_u / 1.15f;
	f		= _dmap_water->ofst_u + pos_x * spd_x;
	if (f >= 0.0f)
		f		-= floorf(f);
	else
		f		= 1.0f - ((-f) - floorf(-f));
	_dmap_water->draw_u		= f;
	_dmap_water->draw_v		= _dmap_water->ofst_v;
	
	// É}ÉeÉäÉAÉãÉAÉjÉÅÅ[ÉVÉáÉì
	Sint32			i;
	DMAP_WATER_OBJ	*waterobj;
	AMS_MOTION		*motion;
	float			f_start, f_end;
	waterobj	= &_dmap_water->object[0];
	for (i = 0; i < 2; i++, waterobj++) {
		motion	= waterobj->motion;
		f		= waterobj->frame + speed;
		f_start	= amMotionMaterialGetStartFrame(motion, 0);
		f_end	= amMotionMaterialGetEndFrame(motion, 0);
		while (f >= f_end)
			f		= f_start + (f - f_end);
		waterobj->frame		= f;
	}
}


/*****************************************************************************/
/* void dwaterGetParam(DMAP_PARAM_WATER *param)                              */
/*---------------------------------------------------------------------------*/
/* [OUTPUT]  param    : ï`âÊÉpÉâÉÅÅ[É^                                       */
/* [FUNCTION]  ï`âÊópÉpÉâÉÅÅ[É^ÇÃéÊìæ                                        */
/*****************************************************************************/
void dwaterGetParam(DMAP_PARAM_WATER *param)
{
	param->frame[0]		= _dmap_water->object[0].frame;
	param->frame[1]		= _dmap_water->object[1].frame;
	param->draw_u		= _dmap_water->draw_u;
	param->draw_v		= _dmap_water->draw_v;
	param->scale		= _dmap_water->scale;
	param->pos_x		= _dmap_water->pos_x;
	param->pos_y		= _dmap_water->pos_y;
	param->pos_dy		= _dmap_water->pos_dy;
	param->repeat_u		= _dmap_water->repeat_u;
	param->repeat_v		= _dmap_water->repeat_v;
	param->rot_z		= _dmap_water->rot_z;
	param->color		= _dmap_water->color;
}


/*****************************************************************************/
/* void dwaterSetParam(void)                                                 */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  ï`âÊópÉpÉâÉÅÅ[É^ÇÃê›íË                                        */
/*****************************************************************************/
void dwaterSetParam(void)
{
	DMAP_PARAM_WATER	*param;

	param	= (DMAP_PARAM_WATER *)amDrawMallocDataBuffer(sizeof(DMAP_PARAM_WATER));

	dwaterGetParam(param);

	amDrawMakeTask(_dwaterSetParam, 0x0000, (Uint32)param);
}


/*****************************************************************************/
/* void dwaterDrawReflection(Uint32 state, NNF_DRAWOBJ drawflag)             */
/*---------------------------------------------------------------------------*/
/* [INPUT]  drawflag : ï`âÊÉtÉâÉO                                            */
/* [FUNCTION]  ï`âÊ(îΩéÀÉIÉuÉWÉFÉNÉgÅEÉÅÉCÉìÉXÉåÉbÉhóp)                      */
/*****************************************************************************/
void dwaterDrawReflection(Uint32 state, NNF_DRAWOBJ drawflag)
{
	NNS_MATRIX		*mtx;
	float			pos_x, pos_y;

	amThreadCheckSafe(0, "dwaterDrawReflection");

	amMatrixPush();

	mtx		= amMatrixGetCurrent();

	// ÉÇÅ[ÉVÉáÉìåvéZ
	DMAP_WATER_OBJ	*waterobj;
	AMS_MOTION		*motion;
	waterobj	= &_dmap_water->object[1];
	motion		= waterobj->motion;
	amMotionMaterialSetFrame(motion, _dmap_water->object[1].frame);
	amMotionMaterialCalc(motion);

	// à íuåvéZ
	pos_x	= _dmap_water->pos_x / (_dmap_water->repeat_pos_x * _dmap_water->scale);
	pos_x	= floorf(pos_x - 0.5f) * (_dmap_water->repeat_pos_x * _dmap_water->scale);
	pos_y	= _dmap_water->pos_y + _dmap_water->pos_dy;

	// ÉIÉuÉWÉFÉNÉgï`âÊ
	Sint32		i;
	for (i = 0; i < 2; i++) {
		nnMakeTranslateMatrix(mtx, pos_x, pos_y, FX_FX32_TO_F32(GMD_OBJ_DEFAULT_POS_Z_M_BACK));
		nnScaleMatrix(mtx, mtx, _dmap_water->scale, _dmap_water->scale, 1.0f );
#if _IPHONE
		ObjDraw3DNNMotionMaterialMotion(motion, waterobj->texlist, drawflag, 0, NULL, NULL, state, NULL, NULL);
#else
		amMotionMaterialDraw(state, motion, waterobj->texlist, drawflag);
#endif // _IPHONE
		pos_x	+= _dmap_water->repeat_pos_x * _dmap_water->scale;
	}

	amMatrixPop();
}


/*****************************************************************************/
/* void dwaterDrawSurface(Uint32 state, NNF_DRAWOBJ drawflag)                */
/*---------------------------------------------------------------------------*/
/* [INPUT]  state    : ï`âÊÉXÉeÅ[Ég                                          */
/*          drawflag : ï`âÊÉtÉâÉO                                            */
/* [FUNCTION]  ï`âÊ(êÖñ ÉIÉuÉWÉFÉNÉgÅEÉÅÉCÉìÉXÉåÉbÉhóp)                      */
/*****************************************************************************/
void dwaterDrawSurface(Uint32 state, NNF_DRAWOBJ drawflag)
{
	NNS_MATRIX		*mtx;
	float			pos_x, pos_y;

	amThreadCheckSafe(0, "dwaterDrawSurface");

	amMatrixPush();

	mtx		= amMatrixGetCurrent();

	// ÉÇÅ[ÉVÉáÉìåvéZ
	DMAP_WATER_OBJ	*waterobj;
	AMS_MOTION		*motion;
	waterobj	= &_dmap_water->object[0];
	motion		= waterobj->motion;
	amMotionMaterialSetFrame(motion, _dmap_water->object[0].frame);
	amMotionMaterialCalc(motion);

	// à íuåvéZ
	pos_x	= _dmap_water->pos_x / (_dmap_water->repeat_pos_x * _dmap_water->scale);
	pos_x	= floorf(pos_x - 0.5f) * (_dmap_water->repeat_pos_x * _dmap_water->scale);
	pos_y	= _dmap_water->pos_y + _dmap_water->pos_dy;

	// ÉIÉuÉWÉFÉNÉgï`âÊ
	Sint32		i;
	for (i = 0; i < 2; i++) {
		nnMakeTranslateMatrix(mtx, pos_x, pos_y, FX_FX32_TO_F32(GMD_OBJ_DEFAULT_POS_Z_N_BACK));
		nnScaleMatrix(mtx, mtx, _dmap_water->scale, _dmap_water->scale * 2.0f, 1.0f);
#if _IPHONE
		ObjDraw3DNNMotionMaterialMotion(motion, waterobj->texlist, drawflag, 0, NULL, NULL, state, NULL, NULL);
#else
		amMotionMaterialDraw(state, motion, waterobj->texlist, drawflag);
#endif // _IPHONE
		pos_x	+= _dmap_water->repeat_pos_x * _dmap_water->scale;
	}

	amMatrixPop();
}


/*****************************************************************************/
/* void dwaterDrawReflection(NNF_DRAWOBJ drawflag)                           */
/*---------------------------------------------------------------------------*/
/* [INPUT]  drawflag : ï`âÊÉtÉâÉO                                            */
/* [FUNCTION]  ï`âÊ(îΩéÀÉIÉuÉWÉFÉNÉg)                                        */
/*****************************************************************************/
/*void dwaterDrawReflection(NNF_DRAWOBJ drawflag)
{
	NNS_MATRIX		*mtx;
	float			pos_x, pos_y;
	DMAP_PARAM_WATER	*param = _dmap_water->draw_param;

	amThreadCheckSafe(1, "dwaterDrawReflection");

	amMatrixPush();

	mtx		= amMatrixGetCurrent();

	// ÉÇÅ[ÉVÉáÉìåvéZ
	DMAP_WATER_OBJ	*waterobj;
	AMS_MOTION		*motion;
	waterobj	= &_dmap_water->object[1];
	motion		= waterobj->motion;
	amMotionMaterialSetFrame(motion, param->frame[1]);
	amMotionMaterialCalc(motion);

	// à íuåvéZ
	pos_x	= param->pos_x / (_dmap_water->repeat_pos_x * _dmap_water->scale);
	pos_x	= floorf(pos_x - 0.5f) * (_dmap_water->repeat_pos_x * _dmap_water->scale);
	pos_y	= param->pos_y + param->pos_dy;

	// ÉIÉuÉWÉFÉNÉgï`âÊ
	Sint32		i;
	for (i = 0; i < 2; i++) {
		nnMakeTranslateMatrix(mtx, pos_x, pos_y, FX_FX32_TO_F32(GMD_OBJ_DEFAULT_POS_Z_M_BACK));
		nnScaleMatrix(mtx, mtx, _dmap_water->scale, _dmap_water->scale, 1.0f );
		amMotionMaterialDraw(motion, waterobj->texlist, drawflag);

		pos_x	+= _dmap_water->repeat_pos_x * _dmap_water->scale;
	}

	amMatrixPop();
}*/


/*****************************************************************************/
/* void dwaterDrawSurface(NNF_DRAWOBJ drawflag)                              */
/*---------------------------------------------------------------------------*/
/* [INPUT]  drawflag : ï`âÊÉtÉâÉO                                            */
/* [FUNCTION]  ï`âÊ(êÖñ ÉIÉuÉWÉFÉNÉg)                                        */
/*****************************************************************************/
/*void dwaterDrawSurface(NNF_DRAWOBJ drawflag)
{
	NNS_MATRIX		*mtx;
	float			pos_x, pos_y;
	DMAP_PARAM_WATER	*param = _dmap_water->draw_param;

	amThreadCheckSafe(1, "dwaterDrawSurface");

	amMatrixPush();

	mtx		= amMatrixGetCurrent();

	// ÉÇÅ[ÉVÉáÉìåvéZ
	DMAP_WATER_OBJ	*waterobj;
	AMS_MOTION		*motion;
	waterobj	= &_dmap_water->object[0];
	motion		= waterobj->motion;
	amMotionMaterialSetFrame(motion, param->frame[0]);
	amMotionMaterialCalc(motion);

	// à íuåvéZ
	pos_x	= param->pos_x / (_dmap_water->repeat_pos_x * _dmap_water->scale);
	pos_x	= floorf(pos_x - 0.5f) * (_dmap_water->repeat_pos_x * _dmap_water->scale);
	pos_y	= param->pos_y + param->pos_dy;

	// ÉIÉuÉWÉFÉNÉgï`âÊ
	Sint32		i;
	for (i = 0; i < 2; i++) {
		nnMakeTranslateMatrix(mtx, pos_x, pos_y, FX_FX32_TO_F32(GMD_OBJ_DEFAULT_POS_Z_N_BACK));
		nnScaleMatrix(mtx, mtx, _dmap_water->scale, _dmap_water->scale, 1.0f );
		amMotionMaterialDraw(motion, waterobj->texlist, drawflag);

		pos_x	+= _dmap_water->repeat_pos_x * _dmap_water->scale;
	}

	amMatrixPop();
}*/


/*****************************************************************************/
/* void dwaterDrawWater(AMS_RENDER_TARGET *texture)                          */
/*---------------------------------------------------------------------------*/
/* [INPUT]  texture : é ÇËçûÇ›ÉåÉìÉ_ÉäÉìÉOÉeÉNÉXÉ`ÉÉ                         */
/* [FUNCTION]  ï`âÊ(êÖñ{ëÃ)                                                  */
/*****************************************************************************/
void dwaterDrawWater(AMS_RENDER_TARGET *texture)
{
	DMAP_PARAM_WATER	*param = _dmap_water->draw_param;

	amThreadCheckSafe(1, "dwaterDrawWater");

	amDrawPushState();

	NNS_VECTOR	pos = { 0.0f, param->pos_dy, -0.5f};
	nnTransformVector(&pos, (NNS_MATRIX *)amDrawGetProjectionMatrix(), &pos);

	float	ofst_u  = param->draw_u;
	float	ofst_v  = param->draw_v;
	float	water_u = param->repeat_u * 0.5f * param->scale;
	float	water_v = param->repeat_v * param->scale;
	float	dy      = pos.y;
	float	water_ou = 0.0f;
	Angle32	rot_z   = param->rot_z;
	Uint32	color   = param->color;

#if _PC | _XBOX
	typedef struct {
		float		p[4];
		Uint32		diffuse;
		float		tu0, tv0, tu1, tv1, tu2, tv2;
	} TVERTEX;
	TVERTEX			vtx[4];
	NNS_MATRIX		mtx;
	NNS_VECTOR		pos0, pos1;
	float			dv, ox, oy , z;

	dv		= dy * -0.5f + 0.5f;
	ox		= AMD_SCREEN_WIDTH  * 0.5f;
	oy		= AMD_SCREEN_HEIGHT * 0.5f;
	z		= 0.0125f;

	nnMakeRotateZMatrix(&mtx, rot_z);
	pos0.x	= -1.15f * 0.5f * AMD_SCREEN_WIDTH;
	pos0.y	= dy * 0.5f * AMD_SCREEN_HEIGHT;
	pos0.z	= 0.0f;
	nnTransformVector(&pos1, &mtx, &pos0);
	vtx[0].p[0]		= ox + pos1.x;
	vtx[0].p[1]		= oy - pos1.y;
	vtx[0].p[2]		= z;
	vtx[0].p[3]		= 1.0f;
	vtx[0].diffuse	= color;
	vtx[0].tu0		= vtx[0].p[0] / AMD_SCREEN_WIDTH;
	vtx[0].tv0		= vtx[0].p[1] / AMD_SCREEN_HEIGHT;
	vtx[0].tu1		= water_ou - water_u + ofst_u;
	vtx[0].tv1		=   dv + ofst_v;
	vtx[0].tu2		= 0.5f;
	vtx[0].tv2		= 0.0f;
	pos0.x	= -pos0.x;
	nnTransformVector(&pos1, &mtx, &pos0);
	vtx[1].p[0]		= ox + pos1.x;
	vtx[1].p[1]		= oy - pos1.y;
	vtx[1].p[2]		= z;
	vtx[1].p[3]		= 1.0f;
	vtx[1].diffuse	= color;
	vtx[1].tu0		= vtx[1].p[0] / AMD_SCREEN_WIDTH;
	vtx[1].tv0		= vtx[1].p[1] / AMD_SCREEN_HEIGHT;
	vtx[1].tu1		= water_ou + water_u + ofst_u;
	vtx[1].tv1		=   dv + ofst_v;
	vtx[1].tu2		= 0.5f;
	vtx[1].tv2		= 0.0f;
	pos0.x	= -1.15f * 0.5f * AMD_SCREEN_WIDTH;
	pos0.y	+= pos0.x * 2.0f;
	nnTransformVector(&pos1, &mtx, &pos0);
	vtx[2].p[0]		= ox + pos1.x;
	vtx[2].p[1]		= oy - pos1.y;
	vtx[2].p[2]		= z;
	vtx[2].p[3]		= 1.0f;
	vtx[2].diffuse	= color;
	vtx[2].tu0		= vtx[2].p[0] / AMD_SCREEN_WIDTH;
	vtx[2].tv0		= vtx[2].p[1] / AMD_SCREEN_HEIGHT;
	vtx[2].tu1		= water_ou - water_u + ofst_u;
	vtx[2].tv1		= water_v + dv + ofst_v;
	vtx[2].tu2		= 0.5f;
	vtx[2].tv2		= 1.0f;
	pos0.x	= -pos0.x;
	nnTransformVector(&pos1, &mtx, &pos0);
	vtx[3].p[0]		= ox + pos1.x;
	vtx[3].p[1]		= oy - pos1.y;
	vtx[3].p[2]		= z;
	vtx[3].p[3]		= 1.0f;
	vtx[3].diffuse	= color;
	vtx[3].tu0		= vtx[3].p[0] / AMD_SCREEN_WIDTH;
	vtx[3].tv0		= vtx[3].p[1] / AMD_SCREEN_HEIGHT;
	vtx[3].tu1		= water_ou + water_u + ofst_u;
	vtx[3].tv1		= water_v + dv + ofst_v;
	vtx[3].tu2		= 0.5f;
	vtx[3].tv2		= 1.0f;

	LPDIRECT3DDEVICE9	d3ddev;

#if _PC
	d3ddev	= amWinDxGetDirect3DDevice();
#elif _XBOX
	d3ddev	= amXboxDxGetDirect3DDevice();
#endif

	amRenderSetTexture(NNE_TEXSLOT_0, texture, 0);
	d3ddev->SetTexture(1, _dmap_water->tex_water);
	d3ddev->SetTexture(2, _dmap_water->tex_color);

#if _PC
	d3ddev->SetTextureStageState(0, D3DTSS_COLOROP,	  D3DTOP_SELECTARG1);
	d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3ddev->SetTextureStageState(1, D3DTSS_COLOROP,	  D3DTOP_SELECTARG1);
	d3ddev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3ddev->SetTextureStageState(2, D3DTSS_COLOROP,	  D3DTOP_SELECTARG1);
	d3ddev->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3ddev->SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_DISABLE);
#endif

	d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	d3ddev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	d3ddev->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	d3ddev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	d3ddev->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	d3ddev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	d3ddev->SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	d3ddev->SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	D3DVIEWPORT9 vp;
	d3ddev->GetViewport(&vp);
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
#if _XBOX | _PC
	d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	d3ddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
#endif
	d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
#if _PC
	d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX3 | D3DFVF_DIFFUSE |
			D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE2(1) |
			D3DFVF_TEXCOORDSIZE2(2));
#elif _XBOX
	d3ddev->SetRenderState(D3DRS_VIEWPORTENABLE, FALSE);
	d3ddev->SetFVF(D3DFVF_XYZW | D3DFVF_TEX3 | D3DFVF_DIFFUSE |
			D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE2(1) |
			D3DFVF_TEXCOORDSIZE2(2));
#endif
	d3ddev->SetVertexShader(_dmap_water->shader_VS);
	d3ddev->SetPixelShader(_dmap_water->shader_PS);
	d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(TVERTEX));
#if _XBOX
	d3ddev->SetRenderState(D3DRS_VIEWPORTENABLE, TRUE);
#endif
	//
	nnResetVertexDeclarationDXG20();
#elif _PS3
	NNS_PRIM2D_PCT	vtx[4];
	NNS_MATRIX		mtx;
	NNS_VECTOR		pos0, pos1;
	float			dv, ox, oy, z;

	dv		= dy * -0.5f + 0.5f;
	ox		= AMD_SCREEN_WIDTH  * 0.5f;
	oy		= AMD_SCREEN_HEIGHT * 0.5f;
	z		= -800.0f;

	nnMakeRotateZMatrix(&mtx, rot_z);
	pos0.x	= -1.15f * 0.5f * AMD_SCREEN_WIDTH;
	pos0.y	= dy * 0.5f * AMD_SCREEN_HEIGHT;
	pos0.z	= 0.0f;
	nnTransformVector(&pos1, &mtx, &pos0);
	vtx[0].Pos.x	= ox + pos1.x;
	vtx[0].Pos.y	= oy - pos1.y;
	vtx[0].Col		= color & 0xffffff00;
	vtx[0].Tex.u	= water_ou - water_u + ofst_u;
	vtx[0].Tex.v	=   dv + ofst_v;
	pos0.x	= -pos0.x;
	nnTransformVector(&pos1, &mtx, &pos0);
	vtx[1].Pos.x	= ox + pos1.x;
	vtx[1].Pos.y	= oy - pos1.y;
	vtx[1].Col		= color & 0xffffff00;
	vtx[1].Tex.u	= water_ou + water_u + ofst_u;
	vtx[1].Tex.v	=   dv + ofst_v;
	pos0.x	= -1.15f * 0.5f * AMD_SCREEN_WIDTH;
	pos0.y	+= pos0.x * 2.0f;
	nnTransformVector(&pos1, &mtx, &pos0);
	vtx[2].Pos.x	= ox + pos1.x;
	vtx[2].Pos.y	= oy - pos1.y;
	vtx[2].Col		= color | 0x000000ff;
	vtx[2].Tex.u	= water_ou - water_u + ofst_u;
	vtx[2].Tex.v	= water_v + dv + ofst_v;
	pos0.x	= -pos0.x;
	nnTransformVector(&pos1, &mtx, &pos0);
	vtx[3].Pos.x	= ox + pos1.x;
	vtx[3].Pos.y	= oy - pos1.y;
	vtx[3].Col		= color | 0x000000ff;
	vtx[3].Tex.u	= water_ou + water_u + ofst_u;
	vtx[3].Tex.v	= water_v + dv + ofst_v;

	// ÉeÉNÉXÉ`ÉÉê›íËópÉeÉNÉXÉ`ÉÉÉäÉXÉgÇÃê∂ê¨
	NNS_TEXLIST		texlist;
	NNS_TEXINFO		texinfo[2];
	texlist.nTex			= 2;
	texlist.pTexInfoList	= texinfo;
	texinfo[0].pSbglTexture	= _dmap_water->tex_water;
	texinfo[0].Flag			= 0;
	texinfo[1].pSbglTexture	= _dmap_water->tex_color;
	texinfo[1].Flag			= 0;

	amRenderSetTexture(NNE_TEXSLOT_0, texture, 0);
	nnSetPrimitiveTexNumPS3(NNE_TEXSLOT_1, &texlist, 0);
	nnSetPrimitiveTexNumPS3(NNE_TEXSLOT_2, &texlist, 1);

	nnSetPrimitiveTexState(
			NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV,
			NNE_PRIM_TEXWRAP_CLAMP, NNE_PRIM_TEXWRAP_CLAMP);
	nnSetPrimitiveTexStatePS3(NNE_TEXSLOT_1,
			NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV,
			NNE_PRIM_TEXWRAP_REPEAT, NNE_PRIM_TEXWRAP_REPEAT);
	nnSetPrimitiveTexStatePS3(NNE_TEXSLOT_2,
			NNE_PRIM_TEXBLEND_MODULATE, NNE_PRIM_TEXCOORD_UV,
			NNE_PRIM_TEXWRAP_CLAMP, NNE_PRIM_TEXWRAP_CLAMP);

	nnSetPrimitive2DDepthFuncPS3(NND_CMPFUNC_PS3_LEQUAL);
	nnSetPrimitive2DDepthMaskPS3(NNE_OFF);

	nnSetPrimitive2DShaderPS3(_dmap_water->shader_VS, _dmap_water->shader_PS);

	nnBeginDrawPrimitive2D(NNE_PRIM2D_FMT_PCT, NNE_PRIM_ALPHABLEND_OFF);
	nnDrawPrimitive2D(NNE_PRIM_TRIANGLE_STRIP, vtx, 4, z);
	nnEndDrawPrimitive2D();

	nnSetPrimitive2DShaderPS3(NULL, NULL);

	nnSetPrimitive2DDepthMaskPS3(NNE_ON);
	nnSetPrimitive2DDepthFuncPS3(NND_CMPFUNC_PS3_LEQUAL);
#elif _WII
	nnPutProjection2DGC();

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX2, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX3, GX_DIRECT);

	GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_POS, GX_POS_XYZ, GX_F32, GX_U8);
	GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, GX_U8);
	GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_TEX0, GX_TEX_ST, GX_F32, GX_U8);
	GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_TEX1, GX_TEX_ST, GX_F32, GX_U8);
	GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_TEX2, GX_TEX_ST, GX_F32, GX_U8);
	GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_TEX3, GX_TEX_ST, GX_F32, GX_U8);

	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0,
			GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
	GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xff, 0xff, 0xff, 0xff});
	GXSetChanAmbColor(GX_COLOR0A0, (GXColor){0xff, 0xff, 0xff, 0xff});

	GXSetNumTexGens(4);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY);
	GXSetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, GX_IDENTITY);
	GXSetTexCoordGen(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX3, GX_IDENTITY);

	GXSetTevSwapModeTable(GX_TEV_SWAP1,
			GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
	GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP1, GX_TEV_SWAP1);
	GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP1, GX_TEV_SWAP1);

	GXSetNumIndStages(1);
	GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
	GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);

	GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);

	GXSetNumTevStages(2);

	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);

	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD3, GX_TEXMAP2, GX_COLOR_NULL);
	GXSetTevColorOp(GX_TEVSTAGE1,
			GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
	GXSetTevColorIn(GX_TEVSTAGE1,
			GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
	GXSetTevAlphaOp(GX_TEVSTAGE1,
			GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE1,
			GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);

	Mtx		mtx;
	MTXIdentity(mtx);
	GXLoadPosMtxImm(mtx, GX_PNMTX0);
	GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);

	float	indMtx[2][3] = {
		{ 0.5f, 0.0f, 0.0f},
		{ 0.0f, 0.5f, 0.0f},
	};
	GXSetIndTexMtx(GX_ITM_0, indMtx, -5+1);

	amRenderSetTexture(NNE_TEXSLOT_0, texture, 0);

	GXLoadTexObj(&_dmap_water->tex_water, GX_TEXMAP1);
	GXLoadTexObj(&_dmap_water->tex_color, GX_TEXMAP2);

	Uint8	r, g, b;
	r		= (u8)((color >> 16) & 0xff);
	g		= (u8)((color >>  8) & 0xff);
	b		= (u8)((color >>  0) & 0xff);

	NNS_VECTOR		pos0, pos1;
	float			dv, ox, oy, z;

	dv		= dy * -0.5f + 0.5f;
	ox		= AMD_SCREEN_WIDTH  * 0.5f;
	oy		= AMD_SCREEN_HEIGHT * 0.5f;
	z		= -800.0f;

//	nnMakeRotateZMatrix(&mtx, rot_z);
	float		sn = sinf((float)rot_z / (float)0x7fff * 3.14159265f);
	float		cs = cosf((float)rot_z / (float)0x7fff * 3.14159265f);
	mtx[0][0]	= cs;
	mtx[0][1]	= -sn;
	mtx[0][2]	= 0.0f;
	mtx[0][3]	= 0.0f;
	mtx[1][0]	= sn;
	mtx[1][1]	= cs;
	mtx[1][2]	= 0.0f;
	mtx[1][3]	= 0.0f;
	mtx[2][0]	= 0.0f;
	mtx[2][1]	= 0.0f;
	mtx[2][2]	= 1.0f;
	mtx[2][3]	= 0.0f;

	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

	GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT7, 4);

	pos0.x	= -1.15f * 0.5f * AMD_SCREEN_WIDTH;
	pos0.y	= dy * 0.5f * AMD_SCREEN_HEIGHT;
	pos0.z	= 0.0f;
	nnTransformVector(&pos1, &mtx, &pos0);
	pos1.x	= ox + pos1.x;
	pos1.y	= oy - pos1.y;
	GXPosition3f32(pos1.x, pos1.y, z);
	GXColor3u8(r, g, b);
	GXTexCoord2f32((pos1.x+0) / AMD_SCREEN_WIDTH, pos1.y / AMD_SCREEN_HEIGHT);	//çÏê¨ÇµÇΩtextureÇÃï£Ç…ÉSÉ~Ç™Ç†ÇÈÇÃÇ≈uví≤êÆÇ≈âÒîÇµÇƒÇ¢ÇΩÇ™ÅAï ìrèCê≥Ç≥ÇÍÇΩÇÃÇ≈uvï‚ê≥ílÇ0Ç…ñﬂÇµÇΩ
	GXTexCoord2f32(water_ou - water_u + ofst_u, dv + ofst_v);
	GXTexCoord2f32(0.5f, 0.0f);
	GXTexCoord2f32(0.0f, 0.0f);

	pos0.x	= -pos0.x;
	nnTransformVector(&pos1, &mtx, &pos0);
	pos1.x	= ox + pos1.x;
	pos1.y	= oy - pos1.y;
	GXPosition3f32(pos1.x, pos1.y, z);
	GXColor3u8(r, g, b);
	GXTexCoord2f32((pos1.x+0) / AMD_SCREEN_WIDTH, pos1.y / AMD_SCREEN_HEIGHT);	//çÏê¨ÇµÇΩtextureÇÃï£Ç…ÉSÉ~Ç™Ç†ÇÈÇÃÇ≈uví≤êÆÇ≈âÒîÇµÇƒÇ¢ÇΩÇ™ÅAï ìrèCê≥Ç≥ÇÍÇΩÇÃÇ≈uvï‚ê≥ílÇ0Ç…ñﬂÇµÇΩ
	GXTexCoord2f32(water_ou + water_u + ofst_u, dv + ofst_v);
	GXTexCoord2f32(0.5f, 0.0f);
	GXTexCoord2f32(0.0f, 0.0f);

	pos0.x	= -1.15f * 0.5f * AMD_SCREEN_WIDTH;
	pos0.y	+= pos0.x * 2.0f;
	nnTransformVector(&pos1, &mtx, &pos0);
	pos1.x	= ox + pos1.x;
	pos1.y	= oy - pos1.y;
	GXPosition3f32(pos1.x, pos1.y, z);
	GXColor3u8(r, g, b);
	GXTexCoord2f32((pos1.x+0) / AMD_SCREEN_WIDTH, pos1.y / AMD_SCREEN_HEIGHT);	//çÏê¨ÇµÇΩtextureÇÃï£Ç…ÉSÉ~Ç™Ç†ÇÈÇÃÇ≈uví≤êÆÇ≈âÒîÇµÇƒÇ¢ÇΩÇ™ÅAï ìrèCê≥Ç≥ÇÍÇΩÇÃÇ≈uvï‚ê≥ílÇ0Ç…ñﬂÇµÇΩ
	GXTexCoord2f32(water_ou - water_u + ofst_u, water_v + dv + ofst_v);
	GXTexCoord2f32(0.5f, 1.0f);
	GXTexCoord2f32(0.0f, 1.0f);

	pos0.x	= -pos0.x;
	nnTransformVector(&pos1, &mtx, &pos0);
	pos1.x	= ox + pos1.x;
	pos1.y	= oy - pos1.y;
	GXPosition3f32(pos1.x, pos1.y, z);
	GXColor3u8(r, g, b);
	GXTexCoord2f32((pos1.x+0) / AMD_SCREEN_WIDTH, pos1.y / AMD_SCREEN_HEIGHT);	//çÏê¨ÇµÇΩtextureÇÃï£Ç…ÉSÉ~Ç™Ç†ÇÈÇÃÇ≈uví≤êÆÇ≈âÒîÇµÇƒÇ¢ÇΩÇ™ÅAï ìrèCê≥Ç≥ÇÍÇΩÇÃÇ≈uvï‚ê≥ílÇ0Ç…ñﬂÇµÇΩ
	GXTexCoord2f32(water_ou + water_u + ofst_u, water_v + dv + ofst_v);
	GXTexCoord2f32(0.5f, 1.0f);
	GXTexCoord2f32(0.0f, 1.0f);

	GXEnd();

	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);

	GXSetTevDirect(GX_TEVSTAGE0);
#endif

	amDrawPopState();
}







void _dwaterSetParam(AMS_TCB *tcbp)
{
	DMAP_PARAM_WATER	*param;

	param		= *(DMAP_PARAM_WATER **)amTaskGetWork(tcbp);

	_dmap_water->draw_param		= param;
}

#endif	// GMD_WATER_SURFACE_USE_RENDER

// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
