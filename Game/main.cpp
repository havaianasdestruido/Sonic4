/*****************************************************************************/
/*      main.cpp                                                             */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* ���C��                                                                    */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090316-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files (Pre Definitions) ---------------------------------------*/
#include "src/pch.h"

#include "/library/include/alice.h"

#if _PS3
#include <sys/spu_initialize.h>
#include <sys/paths.h>

SYS_PROCESS_PARAM(1001, 64 * 1024)
#endif

#include "src/gsMainSys.h"
#include "src/gmMapFar.h"
#include "src/gsSound.h"
#include "src/gsReboot.h"
#include "src/gsEnvironment.h"

/*--- Definitions -----------------------------------------------------------*/

#define AMD_DEBUG_CHAR_MAX		(1200)		// �f�o�b�O������
#define AMD_STANDARD_SHADER_NUM	(256)		// �W���V�F�[�_�[��

#if _PC
#define AMD_NN_ROOT_PATH			""	// nnduv_def.h�@�Ȃ�(PC,Xbox360�ɂ����Ȃ�)
#define AMD_STDSHADER_CODE_PATH		"nnstdshader/"
#ifndef GSD_DEBUG_MODEL_BUILD
	#define AMD_STDSHADER_PATH			AMD_SHADER_NOT_PRECOMPILED		// AMD_NN_ROOT_PATH "nnstdshader/shader/"
	#define AMD_DEFAULT_SHADER_COMPILE	(0)
#else
	#define AMD_STDSHADER_PATH			AMD_NN_ROOT_PATH "nnstdshader/shader/"
	#define AMD_DEFAULT_SHADER_COMPILE	(1)
#endif
#define AMD_HOG_HEAP_SIZE			((128 * 1024) * 1024)
#define AMD_HOG_HEAP_SIZE2			(0)
// Xbox360�̓r���h���Ɏ��@�R�s�[����i�v���p�e�B���R���\�[���f�v���C�����g�j
#elif _XBOX
#define AMD_NN_ROOT_PATH			"d:\\"
#define AMD_STDSHADER_CODE_PATH		"d:\\nnstdshader\\"
#ifndef GSD_DEBUG_MODEL_BUILD
	#define AMD_STDSHADER_PATH			AMD_SHADER_NOT_PRECOMPILED		// "d:\\shader\\"
	#define AMD_DEFAULT_SHADER_COMPILE	(0)
#else
	#define AMD_STDSHADER_PATH			"d:\\shader\\"
	#define AMD_DEFAULT_SHADER_COMPILE	(1)
#endif
#define AMD_HOG_HEAP_SIZE			((128 * 1024) * 1024)
#define AMD_HOG_HEAP_SIZE2			(0)
#elif _PS3
#define AMD_NN_ROOT_PATH			SYS_APP_HOME "/"
#define AMD_STDSHADER_CODE_PATH		SYS_APP_HOME "/nnstdshader/"
#define AMD_STDSHADER_PATH			AMD_SHADER_NOT_PRECOMPILED
#define AMD_DEFAULT_SHADER_COMPILE	(0)
#define AMD_HOG_HEAP_SIZE			((128 * 1024) * 1024)
#define AMD_HOG_HEAP_SIZE2			(0)
#elif _WII
//#define AMD_HOG_HEAP_SIZE			((10 * 1024) * 1024)//(AMD_HEAP_SIZE)
//#define AMD_HOG_HEAP_SIZE2			((50 * 1024) * 1024)//(AMD_HEAP_SIZE2)
#define AMD_HOG_HEAP_SIZE			(18560 * 1024)//((34 * 1024) * 1024)//(AMD_HEAP_SIZE) 18920k		amMemInit�̒��ŋt�̃q�[�v���Q�Ƃ��Ă���H
#define AMD_HOG_HEAP_SIZE2			((17 * 1024) * 1024)//(AMD_HEAP_SIZE2)
#elif _IPHONE
#define AMD_HOG_HEAP_SIZE			(AMD_HEAP_SIZE)
#define AMD_HOG_HEAP_SIZE2			(AMD_HEAP_SIZE2)
#endif

#if _PS3
#if defined(_DLC)
#define AMD_STDSHADER_VS			"/nnstdshader_vsh.cg"
#define AMD_STDSHADER_PS			"/nnstdshader_psh.cg"
#else
#define AMD_STDSHADER_VS			AMD_STDSHADER_CODE_PATH "nnstdshader_vsh.cg"
#define AMD_STDSHADER_PS			AMD_STDSHADER_CODE_PATH "nnstdshader_psh.cg"
#endif
#else
#define AMD_STDSHADER_VS			AMD_STDSHADER_CODE_PATH "nnstdshader.vsh"
#define AMD_STDSHADER_PS			AMD_STDSHADER_CODE_PATH "nnstdshader.psh"
#endif

int test_init(int argc, char *argv[]);

#if _PC
void getWindowSytle(int argc, char *argv[], int *x, int *y, int *style);
int convertArguments(char *cmd_line, char *app_name, char ***argv);
#endif

#if _PS3
void ps3SysutilCallback(uint64_t status, uint64_t param, void *userdata);
#endif

#if _XBOX
#if 1
// ��ʂ�����i�v���~�e�B�u�����j����̂��߂Ƀ_�~�[�v���~�e�B�u�̕`��
static void xboxDrawDummyPrimitive(void);
#endif
#endif // _XBOX


/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

#define HOGVIEWER		(0)

#if HOGVIEWER
#include "HOGViewer.h"
#endif


/*--- Local Declarations ----------------------------------------------------*/

#if _PC
int init(int argc, char *argv[], HINSTANCE instance);
#else
int init(int argc, char *argv[]);
#endif
void finish(void);

#if _PC
DWORD WINAPI draw_proc(DWORD arg);
#elif _XBOX
DWORD WINAPI draw_proc(DWORD arg);
#elif _PS3
void draw_proc(uint64_t arg);
#elif _WII
void draw_proc(void);
#endif

void initNN(void);
void exitNN(void);

void test(AMS_TCB *tcbp);
void test_malloc(AMS_TCB *tcbp);


/*--- External Variables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

Uint32	_am_system_flag[4];				// �V�X�e���t���O
Uint32	_am_debug_flag[4];				// �f�o�b�O�t���O

Uint32	_am_draw_counter = 0;

void	*_am_debug_print_buf = NULL;

NNS_MATRIXSTACK	_am_default_stack;
void	*_am_default_stack_buf;

void	*_am_shader_manager_buf = NULL;
Uint32	_am_vs_code_size;
Uint32	_am_ps_code_size;
void	*_am_vs_code_buf = NULL;
void	*_am_ps_code_buf = NULL;

#if AMD_USE_DRAW_THREAD
AMS_ALARM		_am_main_timer;
AMS_THREAD		_am_draw_thread;
AMS_THREAD_ID	_am_draw_thread_id = (AMS_THREAD_ID)0;
NNS_MATRIXSTACK	_am_draw_stack;
void	*_am_draw_stack_buf;
#endif


/*--- Local Variables -------------------------------------------------------*/

/*--- Inline Functions ------------------------------------------------------*/
//## Inline Functions

/*--- Global Functions ------------------------------------------------------*/
//## Global Functions

#if !_PC
/*****************************************************************************/
/* int main(int argc, char *argv[])                                          */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  ���C��                                                        */
/*****************************************************************************/
int Sonic4_main(int argc, char *argv[])
{
	init(argc, argv);

#if _PC
	amWinMainLoop();
#elif _XBOX
	amXboxMainLoop();
	return 0; // finish�֐����Ăяo���ƃG���[�ƂȂ邽�ߎb��Ή��Ƃ��đ������^�[��
#elif _PS3
	amPs3MainLoop();
#elif _WII
	amWiiMainLoop();
#endif

	finish();

	return	0;
}
#endif


#if _PC
/*****************************************************************************/
/* int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance,           */
/*                                             LPSTR cmd_line, int cmd_show) */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  ���C��                                                        */
/*****************************************************************************/
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show)
{
	UNREFERENCED_PARAMETER(prev_instance);
	UNREFERENCED_PARAMETER(cmd_show);

	int		argc;
	char	**argv;

	argc	= convertArguments(cmd_line, "AliceNN Application", &argv);
	init(argc, argv, instance);
	free(argv);
	amWinMainLoop();
	finish();

	return	0;
}
#endif


/*--- TCB Functions ---------------------------------------------------------*/
//## TCB Functions

/*--- Local Functions -------------------------------------------------------*/
//## Local Functions

/*****************************************************************************/
/* int init(int argc, char *argv[])                                          */
/* int init(int argc, char *argv[], HINSTANCE instance)                      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  ������                                                        */
/*****************************************************************************/
#if _PC
int init(int argc, char *argv[], HINSTANCE instance)
#else
int init(int argc, char *argv[])
#endif
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	// �@��ʏ�����
#if _PC
	int		x, y, style = AMD_WIN_STYLE_DEFAULT;
	amWinDxSetInitFunc(initNN);		// �f�B�X�v���C���[�h�؂�ւ��R�[���o�b�N
	amWinDxSetExitFunc(exitNN);
	getWindowSytle(argc, argv, &x, &y, &style);
	amWinInitNN("Sonic DL", x, y, D3DFMT_A8R8G8B8, style, instance);


#elif _XBOX
#if !AMD_USE_DRAW_THREAD
	amXboxInitNN(D3DFMT_A8R8G8B8);
	initNN();
#endif


#elif _PS3
	sys_spu_initialize(6, 6);
	cellSysutilRegisterCallback(0, ps3SysutilCallback, NULL);
	cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
	cellSysmoduleLoadModule(CELL_SYSMODULE_SYSUTIL_GAME);
#if defined(_DLC)
	AMD_STRCPY_S(_am_ps3_BootDirName, 16, GsEnvGetPs3TitleId());
#endif
	amPs3InitNN(CELL_GCM_SURFACE_A8R8G8B8,
			AMD_PS3_STYLE_DEFAULT_FULLHD | AMD_PS3_STYLE_BUILD_MAIN);
	initNN();


#elif _WII
	amWiiInitNN(GX_PF_RGB8_Z24,
#if 1		// HOME�{�^�����j���[�����X�V����
			AMD_WII_STYLE_DEFAULT | AMD_WII_STYLE_HOMEMENU_BG_FILL |
			AMD_WII_STYLE_HOMEMENU_MSG_ALL,
#elif 0		// HOME�{�^�����j���[���͍X�V���~�߂�
			AMD_WII_STYLE_DEFAULT | AMD_WII_STYLE_HOMEMENU_BG_FILL |
			AMD_WII_STYLE_HOMEMENU_PAUSE | AMD_WII_STYLE_HOMEMENU_MSG_ALL,
#else		// HOME�{�^�����j���[�Ȃ�
			AMD_WII_STYLE_FRAMEBUFFER_MEM2 |
			AMD_WII_STYLE_VERTEXBUFFER_MEM2 |
			AMD_WII_STYLE_TEXTUREBUFFER_MEM2 |
			AMD_WII_STYLE_HOMEMENU_OFF |
			AMD_WII_STYLE_HOMEMENU_MSG_ALL,
#endif
#if defined(_DLC)
			"HomeButton3/");
#else
			"HBM/");
#endif
	initNN();


#endif

	// �}�g���N�X�X�^�b�N�̈�̊m�ہA�ݒ�
	_am_default_stack_buf	= amMemAllocSystem(AMD_MATRIX_STACK_SIZE * sizeof(NNS_MATRIX));
	nnSetUpMatrixStack(&_am_default_stack, _am_default_stack_buf, AMD_MATRIX_STACK_SIZE);
#if AMD_USE_DRAW_THREAD
	_am_draw_stack_buf		= amMemAllocSystem(AMD_MATRIX_STACK_SIZE * sizeof(NNS_MATRIX));
	nnSetUpMatrixStack(&_am_draw_stack, _am_draw_stack_buf, AMD_MATRIX_STACK_SIZE);
#endif

	// �^�X�N�V�X�e���̏�����
	amTaskInitSystem();

	// �`��V�X�e���̏�����
//	amDrawCreateBuffer();		// �����Ńo�b�t�@�T�C�Y���w��
	amDrawCreateBuffer(128 * 1024, 1024 * 1024, 3 * 1024 * 1024);
	amDrawInitDisplayList();

	// �p�b�h�̏�����
#if _WII
	amPadInit(1, AMD_PAD_MODE_ACC);
	// Wii�R���g���[�����莝�����[�h
	amPadSetMapping(-1, AMD_PAD_MAPPING_DOUBLE_HAND);
#else
	amPadInit();
#endif // _WII

	// �`��V�X�e��������
#if AMD_USE_DRAW_THREAD
	// �`��X���b�h�̍쐬
	Sint32	prio;
	Sint32	stack_size = 0x10000;
#if _PC
	prio	= THREAD_PRIORITY_NORMAL;
#elif _XBOX
	prio	= THREAD_PRIORITY_NORMAL;
	stack_size	= 0x10000;
#elif _PS3
	sys_ppu_thread_t	id;
	sys_ppu_thread_get_id(&id);
	sys_ppu_thread_get_priority(id, &prio);
	prio	-= 2;
#elif _WII
	prio	= 14;
#endif
	_am_draw_thread_id	= amThreadCreate(&_am_draw_thread, (void *)draw_proc,
			NULL, AMD_CORE_2A, prio, (size_t)stack_size, "DRAW_THREAD");

	// ���C���^�C�}�[�̍쐬
	amAlarmCreateTimer(&_am_main_timer);
#if 0
	// VSync�Ɠ���
	amAlarmSetTimerVSync(&_am_main_timer);
	amSystemSetFrameRateMain(amSystemGetFrameRateDraw());
#else
	// �Œ�
	amAlarmSetTimer(&_am_main_timer, 1000000 / 60);
	amSystemSetFrameRateMain(1.0f);
#endif
#endif

	// �t�@�C���V�X�e���̏�����
	amFsInit(argc, argv, NULL, NULL, 128, 128, 4, NULL);
#if _PC
#elif _XBOX
	#if defined(_DLC)
		amFsSetDefaultPartition(0);
		amFsLoadPartition(0, "d:/data.cpk", 1);
	#else
		amFsSetDefaultPartition(0);
		amFsLoadPartitionDir(0, "d:\\", 1);
	#endif
#elif _PS3
	#if defined(_DLC)
		char partition[MAX_PATH] = {};
		AMD_STRCPY_S(partition, arrayof(partition), _am_ps3_BootUsrdirPath);
		AMD_STRCAT_S(partition, arrayof(partition), "/data.cpk");
		amFsSetDefaultPartition(0);
		amFsLoadPartition(0, partition, 1);
	#else
		amFsSetDefaultPartition(0);
		amFsLoadPartitionDir(0, SYS_APP_HOME"/", 1);
	#endif
#elif _WII
	#if defined(_DLC)
		amFsOpenCNT(AMD_FS_CNT_USER, AMD_FS_CONTENTS_USER);
		amFsSetDefaultPartition(0);
		amFsLoadPartitionCNT(0, AMD_FS_CNT_USER, "data.cpk", 1);
	#endif
#endif


	// �T�E���h������
#if _WII
	CriSoundRendererWii::ConfigParameter	config;
	// AX�{�C�X�ő吔����NW4R���Ɛ�I�Ɏg�p����{�C�X������������������
	// CRI Audio���g�p�\�ȃ{�C�X�ő吔�Ƃ���
	amAssert(AX_MAX_VOICES >= GSD_SND_NW4R_AX_VOICE_EXCLUSIVE_USAGE_NUM);
	config.max_voices	= AX_MAX_VOICES - GSD_SND_NW4R_AX_VOICE_EXCLUSIVE_USAGE_NUM;
	amCriAudioInit(&config);
	amAssert(AXIsInit() && AXGetMaxVoices() == AX_MAX_VOICES);	// AX���������ɔC�ӂ̍ő�{�C�X�����w�肵�Ȃ�
#else
	amCriAudioInit();
#endif	/* _WII */

	// ������������
	amMemInit(AMD_HOG_HEAP_SIZE, AMD_HOG_HEAP_SIZE2);

#if _WII
	amWiiCreateHBM();
#endif

	// ���[�U�[������
	GsInitUser();

	// �I����L���ɐݒ�
	amSystemExitEnable(1);

	return	0;
}


#if AMD_USE_DRAW_THREAD
/*****************************************************************************/
/* DWORD WINAPI draw_proc(DWORD arg)                                         */
/* void draw_proc(uint64_t arg)                                              */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  �`��X���b�h                                                  */
/*****************************************************************************/
#if _PC
DWORD WINAPI draw_proc(DWORD arg)
#elif _XBOX
DWORD WINAPI draw_proc(DWORD arg)
#elif _PS3
void draw_proc(uint64_t arg)
#elif _WII
void draw_proc(void)
#endif
{
#if !_WII
	UNREFERENCED_PARAMETER(arg);
#endif

#if _WII
	// OSInitFastCast()�̓X���b�h���ɌĂяo�����K�v
	// �܂��A�A�v���N����̑����i�K�ŌĂяo�����K�v�i�x���Ƃ�HBM�J�n�O�j
	OSInitFastCast();
#endif

#if _XBOX
	amXboxInitNN(D3DFMT_A8R8G8B8);
	initNN();
	amXboxDxSetGammaDefault();
#endif

	amThreadOpen(&_am_draw_thread);

#if AMD_DEBUG
	AMS_TIMER	timer;
	amTimerCreate(&timer);
#if AMD_DEBUG_FRAME_RATE
	amTimerStart(&timer);
#endif
#endif

	nnInitLight();

#if !_IPHONE
	{
	// �����_�����O�e�N�X�`���̍쐬
		Sint32	format[1] = {
#if !_WII
				_am_draw_video.draw_format,
#else
				GX_TF_RGB565,
#endif
		};
		amRenderCreate(&_gm_mapFar_render_work,
				(Sint32)AMD_SCREEN_WIDTH, (Sint32)AMD_SCREEN_HEIGHT,
#if !_WII
				1, format, _am_draw_video.draw_format_z,
#else
				1, format, AMD_RENDER_DEPTH_DEFAULT,
#endif
				AMD_RENDER_FLAG_RESOLVE_COLOR0 | //AMD_RENDER_FLAG_RESOLVE_DEPTH |
				AMD_RENDER_FLAG_USE_DEPTH | AMD_RENDER_FLAG_TILED);
	}
#endif

	// �����N���A�J���[�ݒ�
	{
		NNS_RGBA_U8 bc = { 0x00, 0x00, 0x00, 0xff };
		_am_draw_bg_color = bc;
	//	amDrawSetBGColor(&bc);
	}

	for (;;) {
		// �I���v���`�F�b�N
		if (amThreadCheckExit(&_am_draw_thread))
			break;

#if _PC
		// �`�拖�`�F�b�N
		if (!amMutexTrylock(&_am_draw_thread.mutex))
			continue;
#endif

#if AMD_DEBUG && !AMD_DEBUG_FRAME_RATE
		amTimerStart(&timer);
#endif

		// �ǂݏo���f�B�X�v���C���X�g�̐ݒ�
#if !_LIB_DEBUG
		if (amDrawGetDisplayList() != -1) {
#else
		static Sint32 id_old = -2;
		Sint32	id = amDrawGetDisplayList();
		if (id == id_old)
			amSystemLog("Drop frame %d %d\n", id, id_old);
		else if (id != ((id_old + 1) % AMD_DISPLAYLIST_NUM))
			amSystemLog("Skip frame %d %d\n", id, id_old);
		id_old	= id;
		if (id != -1) {
#endif
			amDrawInitState();
			amDrawBeginScene();

#if _PC | _XBOX | _PS3
//			if (amDrawBegin(&_am_draw_target,
			if (amDrawBegin(&_gm_mapFar_render_work,
						AMD_RENDER_CLEAR_COLOR | AMD_RENDER_CLEAR_DEPTH,
						amDrawGetBGColor())) {
#if AMD_DEBUG
				nnSetPrintSize(
						AMD_DISPLAY_WIDTH / (float)AMD_TEXT_WIDTH,
						AMD_DISPLAY_HEIGHT / (float)AMD_TEXT_HEIGHT);
#endif
#else
			if (amDrawBegin()) {
#endif

#if _WII
				if (amWiiIsDraw()) {
#endif

					// �^�X�N�̐���
					amDrawExecCommand(AMD_COMMAND_STATE_MAKE_TASK);

#if _XBOX
#if 1
					// �v���~�e�B�u�������
					xboxDrawDummyPrimitive();
#endif
#endif // _XBOX

					// �^�X�N���s
					amDrawExecute();
#if _WII
				}
#endif
				amDrawDisplay();

				// �f�o�b�O�\��
				amDrawExecCommand(AMD_COMMAND_STATE_DEBUG);

#if defined(MTD_DEBUG)
				if (g_gs_main_sys_info.debug_flag & GSD_DEBUG_DEBUG_DISP) {
					amDebugDisplayPerformance(AMD_TEXT_WIDTH - 22, 2);
				}
#endif // defined(MTD_DEBUG)

				// �f�o�b�O�����\��
				amDrawExecCommand(AMD_COMMAND_STATE_DEBUG_PRINT);
#if AMD_DEBUG
				amDebugSetPerformanceDraw(timer.frame);
#if AMD_DEBUG_FRAME_RATE
				nnPrint(7, 1, "%4d", _am_draw_counter++);
#endif
#endif
				nnFlushPrint();

#if _XBOX
#if 1
				// �v���~�e�B�u��������iamDrawEnd()�O�ɂ��ĂԕK�v�����邽�߁j
				xboxDrawDummyPrimitive();
#endif
#endif // _XBOX

				amDrawEnd();
			}

			// �o�^���X�g���s
			amDrawExecRegist();
		}
#if _PC | _XBOX | _PS3
		else {
			if (amDrawBegin(NULL, AMD_RENDER_CLEAR_COLOR | AMD_RENDER_CLEAR_DEPTH, amDrawGetBGColor())) {
				amDrawEnd();
			}
		}
#endif

#if AMD_DEBUG && !AMD_DEBUG_FRAME_RATE
		amTimerEnd(&timer);
#endif

		// V-Sync�҂�
		amDrawWaitVSync();

#if AMD_DEBUG && AMD_DEBUG_FRAME_RATE
		amTimerEnd(&timer);
#endif

#if _PC
		amMutexUnlock(&_am_draw_thread.mutex);
#endif
	}

#if AMD_DEBUG
	amTimerDelete(&timer);
#endif

	amThreadQuit(&_am_draw_thread);

#if _PC
	return	0;
#elif _XBOX
	return	0;
#elif _PS3
#endif
}
#endif


/*****************************************************************************/
/* void finish(void)                                                         */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  �I��                                                          */
/*****************************************************************************/
void finish(void)
{
	// ���[�U�[�I������
	GsExitUser();

#if AMD_USE_DRAW_THREAD
	amThreadDelete(&_am_draw_thread);
	_am_draw_thread_id		= (AMS_THREAD_ID)0;
#endif
	amRenderDelete(&_gm_mapFar_render_work);
	amDrawExitDisplayList();
	amDrawDeleteBuffer();
	amPadExit();
	amTaskExitSystem();
	amCriAudioExit();
	amFsExit();
	amMemExit();

#if _PC
	amWinEndNN();
#elif _XBOX
	exitNN();
#elif _PS3
	exitNN();
	cellSysmoduleUnloadModule(CELL_SYSMODULE_SYSUTIL_GAME);
	cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);
#elif _WII
	exitNN();
#endif

#if AMD_USE_DRAW_THREAD
	amAlarmDelete(&_am_main_timer);
#endif

#if !_WII
	// �V�F�[�_�[�R�[�h�̈�̉��
	if (_am_ps_code_buf != NULL)
		amMemFreeSystem(_am_ps_code_buf);
	if (_am_vs_code_buf != NULL)
		amMemFreeSystem(_am_vs_code_buf);

	// �V�F�[�_�[�}�l�[�W���[�̈�̉��
	if (_am_shader_manager_buf != NULL)
		amMemFreeSystem(_am_shader_manager_buf);
#endif

	// �f�o�b�O�v�����g�̈�̉��
	if (_am_debug_print_buf != NULL)
		amMemFreeSystem(_am_debug_print_buf);

	// �}�g���N�X�X�^�b�N�̈�̉��
#if AMD_USE_DRAW_THREAD
	amMemFreeSystem(_am_draw_stack_buf);
#endif
	amMemFreeSystem(_am_default_stack_buf);

#if _WII
	if (GsRebootIsTitle()) {
		amWiiExit(1);
	}
	else {
		amWiiExit(0);
	}
#endif
}


/*****************************************************************************/
/* void initNN(void)                                                         */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  NN���C�u�����̏�����                                          */
/*             Windows�ł̓f�B�X�v���C���[�h���؂�ւ�閈�ɌĂяo�����     */
/*****************************************************************************/
void initNN(void)
{
	// �f�o�b�O�v�����g������
	Uint32		size;
	if (_am_debug_print_buf == NULL) {
		size		= nnGetPrintBufferSize(AMD_DEBUG_CHAR_MAX);
		_am_debug_print_buf	= amMemAllocSystem(size);
	}
	nnInitPrint(_am_debug_print_buf, AMD_DEBUG_CHAR_MAX, NULL);

#if !(_WII || _IPHONE)
	// �W���V�F�[�_�[�̏�����
	{
		NNS_STDSHADER_CONFIG shaderconfig;
		Uint32				 size;

		// �W���V�F�[�_�}�l�[�W���̏�����
		nnSetUpStdShaderConfigBasic( &shaderconfig );
		shaderconfig.bNormalizeVertexNormal	= NNE_TRUE;					// ���_�@�����K��
		//shaderconfig.bRescaleVertexNormal	= NNE_TRUE;					// ���_�@���ăX�P�[��
		shaderconfig.nMaxParallelLight		= 1;						// �p���������C�g�ő吔
		shaderconfig.nMaxPointLight			= 0;						// �|�C���g���C�g�ő吔
		shaderconfig.nMaxSpotLight			= 0;						// �X�|�b�g���C�g�ő吔
		shaderconfig.bLightAmbient			= NNE_FALSE;				// ���C�g�A���r�G���g
		shaderconfig.PointLightDistAtten	= NNE_ATTEN_CONSTANT;		// �|�C���g���C�g�����������f��
		//shaderconfig.PointLightDistAtten	= NNE_ATTEN_INVLINEAR;
		//shaderconfig.PointLightDistAtten	= NNE_ATTEN_INVQUADRATIC;
		shaderconfig.SpotLightDistAtten		= NNE_ATTEN_CONSTANT;		// �X�|�b�g���C�g�����������f��
		//shaderconfig.SpotLightDistAtten	= NNE_ATTEN_INVLINEAR;
		//shaderconfig.SpotLightDistAtten	= NNE_ATTEN_INVQUADRATIC;
	//	shaderconfig.FogModel				= NNE_FOG_NONE;				// �t�H�O���f��
		shaderconfig.FogModel				= NNE_FOG_LINEAR;
		//shaderconfig.FogModel				= NNE_FOG_EXP;
		//shaderconfig.FogModel				= NNE_FOG_EXP2;
		shaderconfig.bDistanceFog			= NNE_FALSE;				// �����t�H�O or Z�l�t�H�O
	//	shaderconfig.bDistanceFog			= NNE_TRUE;				// �����t�H�O or Z�l�t�H�O
		shaderconfig.bFragmentFog			= NNE_FALSE;				// �t���O�����g�t�H�O
#if (_PC | _XBOX | _PS3)
		shaderconfig.nUserUniform			= 4;
#endif

		if (_am_shader_manager_buf == NULL) {
			size = nnCalcStdShaderManageBufferSize(AMD_STANDARD_SHADER_NUM);
			_am_shader_manager_buf	= amMemAllocSystem(size);
		}
		nnConfigureStdShader(&shaderconfig, _am_shader_manager_buf, AMD_STANDARD_SHADER_NUM);

#if !defined(_DLC) 
	#if AMD_DEFAULT_SHADER_COMPILE
			// �W���V�F�[�_�R�[�h�̃��[�h
			// ���_�V�F�[�_
			if (_am_vs_code_buf == NULL) {
				_am_vs_code_size	= amFsRead(AMD_STDSHADER_VS,
						&_am_vs_code_buf, AMD_FS_MALLOC_SYSTEM);
			}
			// �s�N�Z���V�F�[�_�[
			if (_am_ps_code_buf == NULL) {
				_am_ps_code_size	= amFsRead(AMD_STDSHADER_PS,
						&_am_ps_code_buf, AMD_FS_MALLOC_SYSTEM);
			}

			// �V�F�[�_�쐬
			amShaderBuildStd((const char*)_am_vs_code_buf, _am_vs_code_size,
					(const char*)_am_ps_code_buf, _am_ps_code_size,
					AMD_NN_ROOT_PATH, AMD_STDSHADER_PATH);
	#else
//			_am_draw_vs_code_buf	= _am_vs_code_buf;
//			_am_draw_ps_code_buf	= _am_ps_code_buf;
//			_am_draw_vs_code_size	= _am_vs_code_size;
//			_am_draw_ps_code_size	= _am_ps_code_size;
	#if _PC
//			_am_windx_include_path	= AMD_NN_ROOT_PATH;
	#elif _XBOX
//			_am_xboxdx_include_path	= AMD_NN_ROOT_PATH;
	#elif _PS3
//			_am_ps3_include_path	= AMD_NN_ROOT_PATH;
	#endif
#endif // AMD_DEFAULT_SHADER_COMPILE
#endif // !defined(_DLC) 
	}
#endif // WII
}


/*****************************************************************************/
/* void exitNN(void)                                                         */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  NN���C�u�����̏I��                                            */
/*             Windows�ł̓f�B�X�v���C���[�h���؂�ւ�閈�ɌĂяo�����     */
/*****************************************************************************/
void exitNN(void)
{
	nnExitPrint();

#if !(_WII || _IPHONE)
	// �V�F�[�_�̊J��
	nnReleaseStdShader();			// �r���h�ς݃V�F�[�_���
	nnClearStdShaderProfiles();		// �o�^�ς݃V�F�[�_�̊J��
#endif

#if _PC
	// ���_�o�b�t�@�̏I��
	nnExitVertexBufferDXG20();
#elif _XBOX
	// ���_�o�b�t�@�̏I��
	nnExitVertexBufferDXG20();
	amXboxEndNN();
#elif _PS3
	amPs3ExitNN();
#elif _WII
	amWiiExitNN();
#elif _IPHONE
	amIPhoneExitNN();
#endif
}


#if _PC
/*****************************************************************************/
/* void getWindowSytle(int argc, char *argv[], int *x, int *y, int *style)   */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  �R�}���h���C����������X�^�C�������肷��                      */
/*****************************************************************************/
void getWindowSytle(int argc, char *argv[], int *x, int *y, int *style)
{
	Sint32		i;
	char		*cmdline;

	*x		= 0;
	*y		= 0;

	for (i = 0; i < argc; i++) {
		cmdline		= argv[i];
		if (cmdline[0] != '/')
			continue;
		if (!strncmp(&cmdline[1], "x=", 2))
			*x		= atoi(&cmdline[3]);
		else if (!strncmp(&cmdline[1], "y=", 2))
			*y		= atoi(&cmdline[3]);
		else if (!strcmp(&cmdline[1], "window"))
			*style	&= ~AMD_WIN_STYLE_FULLSCREEN;
		else if (!strcmp(&cmdline[1], "full"))
			*style	|= AMD_WIN_STYLE_FULLSCREEN;
		else if (!strcmp(&cmdline[1], "vga"))
			*style	|= AMD_WIN_STYLE_VGA;
		else if (!strcmp(&cmdline[1], "wvga")) {
			*style	&= ~AMD_WIN_STYLE_VGA;
			*style	|= AMD_WIN_STYLE_WVGA;
		} else if (!strcmp(&cmdline[1], "wii"))
			*style	|= AMD_WIN_STYLE_WII | AMD_WIN_STYLE_WVGA;
		else if (!strcmp(&cmdline[1], "pal"))
			*style	|= AMD_WIN_STYLE_PAL;
		else if (!strcmp(&cmdline[1], "50fps"))
			*style	|= AMD_WIN_STYLE_50FPS;
	}
}


/*****************************************************************************/
/* int convertArguments(char *cmd_line, char *app_name, char ***argv)        */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  �R�}���h���C��������ϊ�����                                  */
/*****************************************************************************/
int convertArguments(char *cmd_line, char *app_name, char ***argv)
{
	int		argc, i;
	char	*pch, ch;

	argc	= 1;
	pch		= cmd_line;
	while (*pch != 0) {
		while (*pch == ' ')
			pch++;
		if (*pch == 0)
			break;
		argc++;
		ch		= ' ';
		if (*pch == '"') {
			ch		= '"';
			pch++;
		}
		while ((*pch != 0) && (*pch != ch))
			pch++;
		if (*pch == 0)
			break;
		pch++;
	}

	*argv	= (char **)malloc(sizeof(char *) * argc);
	(*argv)[0]	= app_name;
	i		= 1;
	pch		= cmd_line;
	while (*pch != 0) {
		while (*pch == ' ')
			pch++;
		if (*pch == 0)
			break;
		ch		= ' ';
		if (*pch == '"') {
			ch		= '"';
			pch++;
		}
		(*argv)[i++]	= pch;
		while ((*pch != 0) && (*pch != ch))
			pch++;
		if (*pch == 0)
			break;
		*pch	= 0;
		pch++;
	}

	return	argc;
}
#endif

#if _PS3
/*****************************************************************************/
/* void ps3SysutilCallback(uint64_t status, uint64_t param, void *userdata)  */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  �V�X�e�����[�e�B���e�B�R�[���o�b�N                            */
/*****************************************************************************/
void ps3SysutilCallback(uint64_t status, uint64_t param, void *userdata)
{
	switch (status) {
		case	CELL_SYSUTIL_REQUEST_EXITGAME:
			// �Q�[���I���v��
			amPs3ReqExit();
			break;
		case	CELL_SYSUTIL_DRAWING_BEGIN:
			// �N���X���f�B�A�o�[�\���J�n
			break;
		case	CELL_SYSUTIL_DRAWING_END:
			// �N���X���f�B�A�o�[�\���I��
			break;
		default:
			break;
	}
}
#endif


#if _XBOX
#if 1
#include "ao.h"
// ===========================================================================
//! ��ʂ�����i�v���~�e�B�u�����j����̂��߂Ƀ_�~�[�v���~�e�B�u�̕`��
// ===========================================================================
void xboxDrawDummyPrimitive(void)
{
	if (!GsMainSysCheckLoadShaderFinished()) {
		return;
	}
	amDrawPushState();
	amDrawInitState();
	AoActDrawPre();
	nnSetPrimitive3DAlphaTestDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthMaskDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthTestDXG20(NNE_FALSE);
	nnSetPrimitive3DBlendDXG20(
		NNE_BLENDMODE_SRCALPHA, NNE_BLENDMODE_INVSRCALPHA, NNE_BLENDOP_ADD);
	amDrawSetFog(0);
	nnSetPrimitiveTexNum(NULL, -1);
	nnBeginDrawPrimitive3D(
		NNE_PRIM3D_FMT_PC,
		NNE_PRIM_ALPHABLEND_ON,
		NNE_PRIM_LIGHT_DISABLE,
		NNE_PRIM_CULL_NONE);
	NNS_PRIM3D_PC v[3];
	v[0].Col = v[1].Col = v[2].Col = 0x00000000;
	v[0].Pos.x = v[1].Pos.x = -8.0f;
	v[2].Pos.x = -4.0f;
	v[0].Pos.y = v[2].Pos.y = -8.0f;
	v[1].Pos.y = -4.0f;
	v[0].Pos.z = v[1].Pos.z = v[2].Pos.z = -2.0f;
	AoActDrawCorWide(v, 3, AOD_ACT_CORW_NONE);
	nnDrawPrimitive3D(NNE_PRIM_TRIANGLE_LIST, v, 3);
	nnEndDrawPrimitive3D();
	amDrawPopState();
	amDrawEndScene();
}
#endif
#endif // _XBOX
