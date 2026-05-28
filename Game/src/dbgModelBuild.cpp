// ==========================================================================
/*!
  @file dbgModelBuild.cpp
  @brief デバック用モデルビルド

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: dbgModelBuild.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "gs.h"
#include "gsMainSys.h"
#include "objObject.h"

#include "dbgModelBuild.h"

//----- Definitions ---------------------------------------------------------
#define DBGD_MODEL_BUILD_DRAWFLAG_NUM	(2)
typedef struct tag_DBGS_MODEL_BUILD_DRAWFLAG_SET {
	s32			num;										// drawflag数
	NNF_DRAWOBJ	drawflag[DBGD_MODEL_BUILD_DRAWFLAG_NUM];	// drawflagリスト
} DBGS_MODEL_BUILD_DRAWFLAG_SET;


typedef struct tag_DBGS_MODEL_BUILD_WORK {
	NNS_OBJECT	*object;
	//NNS_OBJECT	*object2;
	s32			reg_index;
	//s32			reg_index2;
	s32			flag_cnt;
	BOOL		b_release;
	char		filename[64];
} DBGS_MODEL_BUILD_WORK;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
#if _PC | _XBOX
static void dbgModelBuildMain(MTS_TASK_TCB *tcb);
#endif

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
#if _PC | _XBOX
HANDLE				dbg_model_build_h_file_search_mdl;
//HANDLE				dbg_model_build_h_file_search_tex;
WIN32_FIND_DATA		dbg_model_build_find_data_mdl;

//static const char*   dbg_model_build_tex_path[] = {
//	DBGD_MODEL_BUILD_DATA_PATH "*.dds",
//};
#if _PC
static const char*	dbg_model_build_dir_name_list[] = {
	"dbg_build/",
	"dbg_build_water/",
	"dbg_build_twater/",
};
#else	// _XBOX
static const char*	dbg_model_build_dir_name_list[] = {
	"d:\\dbg_build\\",
	"d:\\dbg_build_water\\",
	"d:\\dbg_build_twater\\",
};
#endif

static const s32 dbg_model_build_dir_num = sizeof(dbg_model_build_dir_name_list) / sizeof(char*);

#if _PC
static const char*   dbg_model_build_mdl_ext[] = {
	/*DBGD_MODEL_BUILD_DATA_PATH */"*.zno",
};
#elif _XBOX
static const char*   dbg_model_build_mdl_ext[] = {
	/*DBGD_MODEL_BUILD_DATA_PATH */"*.eno",
};
#endif


/// フォルダ別drawflagリスト
DBGS_MODEL_BUILD_DRAWFLAG_SET dbg_model_build_drawflag_list[] = {
	{2,
		{
			(NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND |
								NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT |
								NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND |
								NND_DRAWOBJ_MATCTRL_TEXOFFSET),
			// トゥーン
			(NND_DRAWOBJ_SHADER_USER_PROFILE_TOON |
									NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND |
									NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT |
									NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND |
									NND_DRAWOBJ_MATCTRL_TEXOFFSET),
		},
	},
	{1,
		{
			// 水
			(NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND |
								NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT |
								NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND |
								NND_DRAWOBJ_MATCTRL_TEXOFFSET |
								NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1),
			0
		},
	},
	{1,
		{
			// 迷彩トゥーン
			(NND_DRAWOBJ_SHADER_USER_PROFILE_TOON |
									NND_DRAWOBJ_FRAGPARALIGHT1 | NND_DRAWOBJ_SHADER_VERTEXBLEND |
									NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT |
									NND_DRAWOBJ_MATCTRL_ALPHA | NND_DRAWOBJ_MATCTRL_BLEND |
									NND_DRAWOBJ_MATCTRL_TEXOFFSET |
									NND_DRAWOBJ_MATCTRL_USERSAMPLER2D1),
			0
		},
	},
};


#define DBGD_MODEL_BUILD_OBJWORK_NUM	(32)
DBGS_MODEL_BUILD_WORK	dbg_build_model_work[DBGD_MODEL_BUILD_OBJWORK_NUM] = {0};
u32						dbg_build_model_use_flag[(DBGD_MODEL_BUILD_OBJWORK_NUM+31)/32] = {0};
void					*dbg_build_model_data[DBGD_MODEL_BUILD_OBJWORK_NUM] = {NULL};

BOOL					dbg_build_file_load_end = FALSE;

s32						dbg_build_dir_cnt = 0;			// ビルド対象フォルダカウンタ
BOOL					dbg_build_dir_set = FALSE;		// FindFirstFile セット済みフラグ
s32						dbg_build_dir_cur_no = 0;		// 対象フォルダNO

#endif // #if _PC | _XBOX
//----- Global Functions ----------------------------------------------------
#if _PC | _XBOX
// ==========================================================================
// DbgDummyInitLogo
/*!
 *	デバッグ用 モデルビルド
 */
// ==========================================================================
void DbgModelBuildInit(void *arg)
{
	UNREFERENCED_PARAMETER(arg);

	MTM_TASK_MAKE_TCB(dbgModelBuildMain, NULL,
					0, 0, 0x2000, 0,
					0, "DBG MODELBUILD");

	dbg_build_dir_cnt = 0;
	dbg_build_dir_set = FALSE;

}


// ==========================================================================
// dbgModelBuildMain
/*!
 *	デバッグ用 モデルビルドメイン処理
 */
// ==========================================================================
void dbgModelBuildMain(MTS_TASK_TCB *tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	s32						work_no, i;
	DBGS_MODEL_BUILD_WORK	*model_work;
	BOOL					b_sts;
	char					filepath[256];
	NNS_TEXFILELIST			*texfilelist;
	NNS_OBJECT				*obj_file;

	DBGS_MODEL_BUILD_DRAWFLAG_SET	*drawflag_set;
	const char						*build_path;

	if (!dbg_build_dir_set) {
		char find_str[256];

		amZeroMemory(find_str, sizeof(find_str));
		strcpy(find_str, dbg_model_build_dir_name_list[dbg_build_dir_cnt]);
		strcat(find_str, dbg_model_build_mdl_ext[0]);
		dbg_model_build_h_file_search_mdl =
			FindFirstFile(find_str, &dbg_model_build_find_data_mdl);

		dbg_build_dir_cur_no = dbg_build_dir_cnt;

		dbg_build_dir_cnt++;
		dbg_build_dir_set = TRUE;
		dbg_build_file_load_end = FALSE;
	}

	// 現在使用中のdrawflagセットと対象フォルダ
	drawflag_set = &dbg_model_build_drawflag_list[dbg_build_dir_cur_no];
	build_path = dbg_model_build_dir_name_list[dbg_build_dir_cur_no];


	for (work_no = 0; work_no < DBGD_MODEL_BUILD_OBJWORK_NUM; work_no++) {
		if (dbg_build_model_use_flag[work_no/32] & (1 << (work_no % 32))) {
			model_work = &dbg_build_model_work[work_no];

			if (model_work->reg_index == -1) {
				// モデルデータビルド
				OS_Printf("Build Start : %s type %d\n", model_work->filename, model_work->flag_cnt);

				amObjectSetup(&obj_file, &texfilelist, dbg_build_model_data[work_no]);
				model_work->reg_index = amObjectLoad(&model_work->object, obj_file, (NNF_DRAWOBJ)drawflag_set->drawflag[model_work->flag_cnt]);
				model_work->b_release = FALSE;
			}
			else if (!model_work->b_release) {
				// ビルド中
				if (amDrawIsRegistComplete(model_work->reg_index)) {
					// ロード終了
					OS_Printf("Build End : %s type %d\n", model_work->filename, model_work->flag_cnt);
					// 開放
					model_work->reg_index = amObjectRelease(model_work->object);
					model_work->b_release = TRUE;
				}
			}
			else {
				// 開放中
				if (amDrawIsRegistComplete(model_work->reg_index)) {
					// 開放終了
					OS_Printf("Release End : %s type %d\n", model_work->filename, model_work->flag_cnt);

					model_work->flag_cnt++;
					if (model_work->flag_cnt >= drawflag_set->num) {
						// 終了
						dbg_build_model_use_flag[work_no/32] &= ~(1 << (work_no % 32));
						amMemFree(dbg_build_model_data[work_no]);
						dbg_build_model_data[work_no] = NULL;
					}
					else {
						// 次へ
						model_work->reg_index = -1;
					}
				}
			}
		}
	}


	while (!dbg_build_file_load_end && dbg_model_build_h_file_search_mdl != INVALID_HANDLE_VALUE) {
		// 
		for (work_no = 0, model_work = NULL; work_no < DBGD_MODEL_BUILD_OBJWORK_NUM; work_no++) {
			if (!(dbg_build_model_use_flag[work_no/32] & (1 << (work_no % 32)))) {
				// ワーク取得
				model_work = &dbg_build_model_work[work_no];
				dbg_build_model_use_flag[work_no/32] |= (1 << (work_no % 32));
				break;
			}
		}

		if (model_work == NULL) {
			// 待機
			return;
		}

		MI_CpuClear8(model_work, sizeof(DBGS_MODEL_BUILD_WORK));
		model_work->reg_index = -1;

		// モデルデータ読み込み
		dbg_build_model_data[work_no] = NULL;
		AMD_STRCPY_S(filepath, 256, build_path);
		AMD_STRCAT_S(filepath, 256, dbg_model_build_find_data_mdl.cFileName);
		amFsRead(filepath, &dbg_build_model_data[work_no], AMD_FS_MALLOC_NORMAL);

		// ファイルパス保存
		AMD_STRCPY_S(model_work->filename, 63, dbg_model_build_find_data_mdl.cFileName);
		
		// 次のファイル
		b_sts = FindNextFile(dbg_model_build_h_file_search_mdl, &dbg_model_build_find_data_mdl);
		if (b_sts == FALSE) {
			// 終了
			dbg_build_file_load_end = TRUE;
		//	mtTaskChangeTcbProcedure(tcb, NULL);
		//	OS_Printf("Debug Model Build End\n");
			return;
		}
	}

	for (i = 0; i < DBGD_MODEL_BUILD_OBJWORK_NUM/32; i++) {
		if (dbg_build_model_use_flag[i]) {
			// まだビルド中
			return;
		}
	}

	if (dbg_build_file_load_end) {
		if (dbg_build_dir_cnt < dbg_model_build_dir_num) {
			// 次のフォルダ
			dbg_build_dir_set = FALSE;
		}
		else {
			// 終了
			mtTaskChangeTcbProcedure(tcb, NULL);
			OS_Printf("Debug Model Build End\n");
		}
		return;
	}

#if 0

	// 見つからない
	if (pWk->hFind[type] == INVALID_HANDLE_VALUE) {
		pWk->FileName[type][0][0] = 0;
		pWk->FileNum[type] = 0;
		pWk->FileId[type] = 0;
	    return 1;
	}
	// 見つかった場合さらに検索
	else
	{
		hgv_SetFileNameToWork( type, FindFileData.cFileName);

		for (int i = 0; i < HGVD_FILENUM_MAX; i++)
		{
		    // 次のファイルを探す
            BOOL ret = FindNextFile(pWk->hFind[type], &FindFileData);
	        if ( ret != 0 )
	        {
		        hgv_SetFileNameToWork( type, FindFileData.cFileName);
		    }
			else
			{
				break;
			}
		}
		FindClose(pWk->hFind[type]);
	}

	// ファイル数以上のＩＤを選べないようにしておく
	if ( (pWk->FileId[type] > 0) && (pWk->FileId[type] >= pWk->FileNum[type]) )
	{
        pWk->FileId[type] = pWk->FileNum[type]-1;
	}
#endif

}
#endif // #if _PC | _XBOX


//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
