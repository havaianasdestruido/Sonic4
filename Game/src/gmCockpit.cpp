// ===========================================================================
/*!
	@file	gmCockpit.cpp
	@brief	コックピット

	@author	Kazuki Yoshida
				Copyright(c) 2009 Dimps
	$Id: gmCockpit.cpp 2 2011-04-11 05:21:26Z thamada $
	$Date::						   $
	
 */
// ===========================================================================
/*
 *
 *
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "objObject.h"
#include "gmTask.h"
#include "gmObjDef.h"

#include "gmCockpit.h"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

/*------ Global Functions ----------------------------------------------*/
// ==========================================================================
// GmCockpitInit
/*!
 *	コックピット関連 初期化
 *
 *	@note
 *		コックピット関連の初期化を一括して行います
 */
// =========================================================================
void GmCockpitInit(void)
{
	;
}


// ==========================================================================
// GmCockpitExit
/*!
 *	コックピット関連 終了処理
 *
 *	@note
 *		コックピット関連の終了処理を一括して行います
 */
// ==========================================================================
void GmCockpitExit(void)
{
	;
}



// ==========================================================================
// コックピットワーク
// ==========================================================================
// ==========================================================================
// GmCockpitCreateWork
/*!
 *	コックピットワークの作成・初期化
 *
 *	@param	work_size	[in]	取得するTCBワークサイズ
 *	@param	parent_obj	[in]	親オブジェクト(NULL可)
 *	@param	sort_prio	[in]	コックピットソート用のプライオリティ
 *	@param	name		[in]	デバッグ用TCB名(NULL可)
 *
 *	@return	取得したOBS_OBJECT_WORKワークアドレス
 *
 *	@note
 *		指定サイズのワークを持つオブジェクト管理TCBを作成し\n
 *		OBS_OBJECT_WORKの基本設定を行います。\n
 *		work_sizeは sizeof(OBS_OBJECT_WORK) 以上の値を設定して下さい。\n
 */
// ==========================================================================
#if defined(MTD_DEBUG)
OBS_OBJECT_WORK *GmCockpitCreateWork(Uint32 work_size, OBS_OBJECT_WORK *parent_obj, u16 sort_prio, const char *name)
#else
OBS_OBJECT_WORK *GmCockpitCreateWork(Uint32 work_size, OBS_OBJECT_WORK *parent_obj, u16 sort_prio)
#endif /* defined(MTD_DEBUG) */
{
	OBS_OBJECT_WORK	*obj_work;
	
	if (work_size < sizeof(OBS_OBJECT_WORK)) {
#if defined(MTD_DEBUG)
		amAssert(!"gmCockpit::GmCockpitCreateWork() Error! work_size too small\n");
#endif /* defined(MTD_DEBUG) */
		work_size	= sizeof(OBS_OBJECT_WORK);
	}
	
	// オブジェクト取得
//	MTM_ASSERT(GMD_TASK_PRIO_EFFECT < GMD_TASK_PRIO_EFFECT_MAX);
//	MTM_ASSERT((GMD_TASK_PRIO_EFFECT + sort_prio) < GMD_TASK_PRIO_EFFECT_MAX);
#if defined(MTD_DEBUG)
	obj_work	= OBM_OBJECT_TASK_DETAIL_INIT((u16)(GMD_TASK_PRIO_FIX + sort_prio),
											  GMD_TASK_GROUP_FIX,
											  GMD_TASK_PAUSELEVEL_DEF,
											  GMD_OBJ_OBJPAUSELEVEL_DEF,
											  work_size, name);
#else
	obj_work	= OBM_OBJECT_TASK_DETAIL_INIT((u16)(GMD_TASK_PRIO_FIX + sort_prio),
											  GMD_TASK_GROUP_FIX,
											  GMD_TASK_PAUSELEVEL_DEF,
											  GMD_OBJ_OBJPAUSELEVEL_DEF,
											  work_size, NULL);
#endif /* defined(MTD_DEBUG) */
	
	if (obj_work == NULL) {
		MTM_ASSERT(!"gmCockpit::GmCockpitCreateWork() object create error! \n");
		return NULL;
	}
	
	// オブジェクトタイプ
	obj_work->obj_type	= GMD_OBJTYPE_COCKPIT;
	
	// 標準関数設定
	obj_work->ppOut		= ObjDrawActionSummary;
	obj_work->ppOutSub	= NULL;
	obj_work->ppIn		= NULL;
	obj_work->ppMove	= NULL;
	obj_work->ppActCall	= NULL;
	obj_work->ppRec		= NULL;
	obj_work->ppLast	= NULL;
	obj_work->ppViewCheck	= NULL;	// OBM_OBJECT_TASK_DETAIL_INITで設定済み
//	MTM_ASSERT(obj_work->ppViewCheck);
	
	// 落下ステータス設定
	obj_work->spd_fall		= GMD_OBJ_DEF_FALL_SPD;		// 落下加速度
	obj_work->spd_fall_max	= GMD_OBJ_DEF_FALL_SPDMA;	// 落下最大速度
	
	// 親設定
	if (parent_obj) {
		obj_work->parent_obj	= parent_obj;
		obj_work->pos.x	= parent_obj->pos.x;
		obj_work->pos.y	= parent_obj->pos.y;
		obj_work->pos.z	= parent_obj->pos.z;
	}
	
	// フラグ設定
//	obj_work->disp_flag	|= OBD_DISP_NODIR;							// 回転無し
	obj_work->flag		|= OBD_OBJECT_NOHIT | OBD_OBJECT_NOCLIP;	// 矩形登録無し クリッピング無し
	obj_work->move_flag	|= OBD_MOVE_NOCOL;							// 当たり関連無し
	
	return obj_work;
}



// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================
