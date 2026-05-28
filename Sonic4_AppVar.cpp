
#include <alice.h>

#include "Sonic4_AppMain.h"
#include "Sonic4_AppVar.h"

extern Uint32			_am_system_flag[4];		// システムフラグ
extern Uint32			_am_debug_flag[4];		// デバッグフラグ

extern Uint32			_am_draw_counter;

extern void*			_am_debug_print_buf;

extern NNS_MATRIXSTACK	_am_default_stack;
extern void*			_am_default_stack_buf;

#if AMD_USE_DRAW_THREAD
extern AMS_ALARM		_am_main_timer;
extern AMS_THREAD		_am_game_thread;
extern AMS_THREAD_ID	_am_game_thread_id;
extern NNS_MATRIXSTACK	_am_game_stack;
extern AMS_THREAD		_am_cri_thread;
extern AMS_THREAD_ID	_am_cri_thread_id;
extern void*			_am_game_stack_buf;
#else
extern AMS_THREAD		_am_cri_thread;
extern AMS_THREAD_ID	_am_cri_thread_id;
#endif

#if !AMD_DEBUG_FRAME_RATE
extern float _am_performance_GPU;
#endif // !AMD_DEBUG_FRAME_RATE

extern BOOL _am_sample_end_suspended;
#if APPD_MAIN_CRIAUDIO_INIT_DELAY
extern BOOL _am_sample_init;
#endif // APPD_MAIN_CRIAUDIO_INIT_DELAY

// ----------------------------------------
// amTask
extern AMS_TASK* _am_default_taskp;
extern const AMS_TASKLIST_OWNER* _am_owner_list;
extern int _am_szOwnerList;
extern Sint32 _am_display_tasklist;		// Task List 0:off !0:on
extern Sint32 _am_tlist_cline;			// 選択中のタスクのインデックス
extern Sint32 _am_tlist_dline;			// 表示タスクリストの先頭インデックス

///////////////////////////////////////////

extern void amTaskStaticVarInit(void);
extern void GsSoundStaticVarInit(void);
extern void GsMainSysStaticVarInit(void);
extern void objObjectStaticVarInit(void);
extern void objCameraStaticVarInit(void);
extern void izFadeStaticVarInit(void);
extern void syEvtStaticVarInit(void);

extern void GmMainStaticVarInit(void);
extern void GmMapFarStaticVarInit(void);
extern void GmDecoStaticVarInit(void);
extern void GmTvxStaticVarInit(void);
extern void GmFixStaticVarInit(void);
extern void GmStartDemoStaticVarInit(void);
extern void GmOverStaticVarInit(void);
extern void GmRingStaticVarInit(void);
extern void GmEventMgrStaticVarInit(void);
extern void GmEfctCmnStaticVarInit(void);
extern void GmEfctZoneStaticVarInit(void);
extern void GmMapStaticVarInit(void);
extern void GmPlyEfctStaticVarInit(void);
extern void GmPlayerStaticVarInit(void);
extern void GmGameDatStaticVarInit(void);

extern void mppEM_StaticVarInit(void);

extern void DmFileSlctStaticVarInit(void);
extern void DmSoundStaticVarInit(void);
extern void DmSndBgmPlayerStaticVarInit(void);
extern void DmLoadingStaticVarInit(void);
extern void DmLogoESRBStaticVarInit(void);
extern void DmLogoSegaStaticVarInit(void);
extern void DmLogoSonicStaticVarInit(void);
extern void DmTitleStaticVarInit(void);
extern void DmTitleOpStaticVarInit(void);
extern void DmOptionStaticVarInit(void);

///////////////////////////////////////////
void Sonic4_AppVar::Init(void)
{
	// alice
	{
		memset(_am_system_flag, 0, sizeof(_am_system_flag));	// システムフラグ
		memset(_am_debug_flag,  0, sizeof(_am_debug_flag));		// デバッグフラグ
		_am_draw_counter      = 0;
		_am_debug_print_buf   = NULL;
		memset(&_am_default_stack, 0, sizeof(_am_default_stack));
		_am_default_stack_buf = NULL;
#if AMD_USE_DRAW_THREAD
		memset(&_am_main_timer,  0, sizeof(_am_main_timer));
		memset(&_am_game_thread, 0, sizeof(_am_game_thread));
		_am_game_thread_id = (AMS_THREAD_ID)0;
		memset(&_am_game_stack,  0, sizeof(_am_game_stack));
		memset(&_am_cri_thread,  0, sizeof(_am_cri_thread));
		_am_cri_thread_id = (AMS_THREAD_ID)0;
		_am_game_stack_buf = NULL;
#else
		memset(&_am_cri_thread, 0, sizeof(_am_cri_thread));
		_am_cri_thread_id = (AMS_THREAD_ID)0;
#endif
		_am_sample_end_suspended = FALSE;
#if APPD_MAIN_CRIAUDIO_INIT_DELAY
		_am_sample_init = FALSE;
#endif // APPD_MAIN_CRIAUDIO_INIT_DELAY
	
#if APPD_MAIN_DRAW_FRAME
		_am_dbg_display_mode = 0;
#endif // APPD_MAIN_DRAW_FRAME
	
#if !AMD_DEBUG_FRAME_RATE
		_am_performance_GPU  = 0.0f;
#endif // !AMD_DEBUG_FRAME_RATE
		
		_am_sample_count = AMD_SAMPLE_COUNT_DEFAULT;
		_am_sample_draw_enable = FALSE;
		_am_sample_is_suspended = FALSE;
		_am_sample_suspended_count = 0;
		_am_sample_is_sleep = FALSE;
		_am_sample_is_accel = FALSE;
		_am_sample_is_ignore_audio_interruption = FALSE;
	}
	// ----------------------------------------
	// amTask
	{
		_am_default_taskp = NULL;
		_am_owner_list = NULL;
		_am_szOwnerList = 0;
		_am_display_tasklist = 1;		// Task List 0:off !0:on
		_am_tlist_cline = 0;			// 選択中のタスクのインデックス
		_am_tlist_dline = 0;			// 表示タスクリストの先頭インデックス
	}
	// ----------------------------------------
	// gsSound
	GsSoundStaticVarInit();
	// gsMainSys
	GsMainSysStaticVarInit();
	// ----------------------------------------
	// objObject
	objObjectStaticVarInit();
	// objCamera
	objCameraStaticVarInit();
	// ----------------------------------------
	// iZFade
	izFadeStaticVarInit();
	// ----------------------------------------
	// syEvt
	syEvtStaticVarInit();
	// ----------------------------------------
	// dmFileSlct
#if _WII || _PC	
	DmFileSlctStaticVarInit();
#endif // #if _WII || _PC
	// dm_loading
	DmLoadingStaticVarInit();
	// dmLogoESRB
#if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR)) || _XBOX || _PC	
	DmLogoESRBStaticVarInit();
#endif // #if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR)) || _XBOX || _PC
	// dmSound
	DmSoundStaticVarInit();
	// DmSndBgmPlayer
	DmSndBgmPlayerStaticVarInit();
	// dmLogoSega
	DmLogoSegaStaticVarInit();
	// dmLogoSonic
	DmLogoSonicStaticVarInit();
	// dmTitle
	DmTitleStaticVarInit();
	// dmTitleOp
	DmTitleOpStaticVarInit();
	// dmOption
	DmOptionStaticVarInit();
	// ----------------------------------------
	// gmMain
	GmMainStaticVarInit();
	// gmMapFar
	GmMapFarStaticVarInit();
	// gmDeco
	GmDecoStaticVarInit();
	// gmTvx
	GmTvxStaticVarInit();
	// gmFix
	GmFixStaticVarInit();
	// gmStartDemo
	GmStartDemoStaticVarInit();
	// gmOver
	GmOverStaticVarInit();
	// gmRing
	GmRingStaticVarInit();
	// gmEventMgr
	GmEventMgrStaticVarInit();
	mppEM_StaticVarInit();
	// gmEfctCmn
	GmEfctCmnStaticVarInit();
	// gmEfctZone
	GmEfctZoneStaticVarInit();
	// gmMap
	GmMapStaticVarInit();
	// gmPlyEfct
	GmPlyEfctStaticVarInit();
	// gmPlayer
	GmPlayerStaticVarInit();
	// gmGameDBuild
	GmGameDatStaticVarInit();
	// ----------------------------------------
};

void Sonic4_AppVar::Exit(void)
{
	Init();
}
