// ================================================================
/*!
  @file objCamera.c
  @brief オブジェクトカメラ

  @author mana
                Copyright(c) 2005 Dimps

  $Id: objCamera.cpp 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 *
 */

//----- Include Files --------------------------------------------------
#include "pch.h"
#include "objCamera.h"

//----- Definitions ----------------------------------------------------
#define	OBD_CAMERA_PRIORITY	(MTD_TASK_PRIORITY_TAIL-1)		//!< カメラシステムタスク優先

#define MPP_CAM_ASPECT_COEFF (1.0f)


//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- External Declarations ------------------------------------------

//----- Static Declarations --------------------------------------------
static void objCameraDest( MTS_TASK_TCB * pTcb );
#if defined _DS
static void objCameraMain( void );
#else
static void objCameraMain(MTS_TASK_TCB *tcb);
#endif
static void objCameraMove( OBS_CAMERA * obj_camera );
static void objCameraLimitCheck( OBS_CAMERA * obj_camera );
#if defined _DS
static void objCameraPosLimitCheck( OBS_CAMERA * obj_camera, VecFx32 *pPos );
#else
static void objCameraPosLimitCheck( OBS_CAMERA * obj_camera, NNS_VECTOR *pPos );
#endif

//----- Global Variables -----------------------------------------------

//----- Local Variables ------------------------------------------------
MTS_TASK_TCB	*obj_camera_tcb = NULL;			///< カメラTCB
OBS_CAMERA_SYS	*obj_camera_sys = NULL;			///< カメラシステムワーク

//----- Global Functions -----------------------------------------------
// ================================================================
// ObjCameraInit
/*!
  カメラ初期化関数

	@param	cam_id		[in]	生成するカメラID (0 ～ OBD_CAMERA_NUM-1) -1で指定無し
	@param	pos			[in]	カメラ初期座標
	@param	group		[in]	タスクグループ
	@param	pause_level	[in]	ポーズレベル
	@param	prio		[in]	処理優先

	@return カメラインデックス値	-1 で生成失敗
 */
// ================================================================
s32 ObjCameraInit(s32 cam_id, NNS_VECTOR *pos, s32 group, u16 pause_level, s32 prio)
{
	//MTS_TASK_TCB   * pTcb;
	OBS_CAMERA		*obj_camera;
	OBS_CAMERA_SYS	*camera_sys;
	int				c_id;

	MTM_ASSERT(pos);
	MTM_ASSERT(-1 <= cam_id && cam_id < OBD_CAMERA_NUM);

	if (obj_camera_tcb == NULL) {
		// タスク生成
		obj_camera_tcb = MTM_TASK_MAKE_TCB(
			objCameraMain, objCameraDest, 0/*flag*/, pause_level, (u32)(prio),
			group, sizeof(OBS_CAMERA_SYS), "objCamera");

		// ワーク取得
		camera_sys = (OBS_CAMERA_SYS*)mtTaskGetTcbWork(obj_camera_tcb);
		MI_CpuClear8(camera_sys, sizeof(OBS_CAMERA_SYS));

		obj_camera_sys = camera_sys;
	}
	else {
		//camera_sys = (OBS_CAMERA_SYS*)mtTaskGetTcbWork(obj_camera_tcb);
		camera_sys = obj_camera_sys;
	}

	if (camera_sys->camera_num >= OBD_CAMERA_NUM) {
		MTM_ASSERT(!"objCamera.cpp::ObjCameraInit() Error! camera num over\n");
		return (-1);
	}

	// カメラ生成
	if (cam_id < 0) {
		// 空きIDサーチ
		for (c_id = 0; c_id < OBD_CAMERA_NUM; c_id++) {
			if (camera_sys->obj_camera[c_id] == NULL) {
				break;
			}
		}
	}
	else {
		if (camera_sys->obj_camera[cam_id] != NULL) {
			MTM_ASSERT(!"objCamera.cpp::ObjCameraInit() Error! camera cam_id used\n");
			return (-1);
		}
		c_id = cam_id;
	}

	if (c_id >= OBD_CAMERA_NUM) {
		MTM_ASSERT(c_id < OBD_CAMERA_NUM);
		return (-1);
	}
	if (camera_sys->obj_camera[c_id] != NULL) {
		MTM_ASSERT(camera_sys->obj_camera[c_id] == NULL);
		return (-1);
	}

	camera_sys->obj_camera[c_id] = (OBS_CAMERA*)mtMemAllocSys(sizeof(OBS_CAMERA));
	MI_CpuClear8(camera_sys->obj_camera[c_id], sizeof(OBS_CAMERA));
	camera_sys->camera_num++;

	obj_camera = camera_sys->obj_camera[c_id];

	// カメラID保存
	obj_camera->camera_id = c_id;

	// コマンドステート初期化
	obj_camera->command_state = OBD_DRAW_CMD_STATE_3DNN;

    // 標準値設定
	obj_camera->spd_max.x = 16.0f;//FX32_ONE << 4;
	obj_camera->spd_max.y = 16.0f;//FX32_ONE << 4;
	obj_camera->spd_max.z = 4.0f;//FX32_ONE << 2;
	obj_camera->spd_add.x = 3.0f;//FX32_ONE >> 1;
	obj_camera->spd_add.y = 3.0f;//FX32_ONE >> 1;
	obj_camera->spd_add.z = 0.5f;//FX32_ONE >> 1;
	obj_camera->shift   = 1;
    
	obj_camera->limit[OBD_LEFT  ] = 8;
	obj_camera->limit[OBD_TOP   ] = 8;
	obj_camera->limit[OBD_RIGHT ] = obj_camera->limit[OBD_LEFT] + OBD_LCD_X;
	obj_camera->limit[OBD_BOTTOM] = obj_camera->limit[OBD_TOP ] + OBD_LCD_Y;
	obj_camera->limit[OBD_BACK  ] = -0x1000;
	obj_camera->limit[OBD_FRONT ] = 0x1000;
    
    // 初期位置設定
	obj_camera->pos = *pos;
	// 1フレ前も同じ値に設定
	obj_camera->prev_pos = *pos;
        
	obj_camera->disp_pos = *pos;
	obj_camera->prev_disp_pos = *pos;
    
	return (c_id);
}

// ================================================================
// ObjCamera3dInit
/*!
  3Dカメラ初期化関数
 
  @param cam_id   [in] カメラ番号 (ObjCameraInit で初期化したカメラNO)
 */
// ================================================================
void ObjCamera3dInit(s32 cam_id)
{
	OBS_CAMERA	*obj_camera;

    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);

	if (obj_camera_sys == NULL) {
		MTM_ASSERT(!"objCamera.cpp::ObjCamera3dInit() Error! not initialized system\n");
		return;
	}

	if (obj_camera_sys->obj_camera[cam_id] == NULL) {
		s32	temp_id;
#if defined _DS
		VecFx32	pos = {0, 0, 0};
#else
		NNS_VECTOR	pos = {0.0f, 0.0f, 0.0f};
#endif
		// カメラTCBが未生成の場合は標準プライオリティで生成
		temp_id = ObjCameraInit(cam_id, &pos, OBD_TASK_GROUP_SYSTEM, 0/*pause_level*/, OBD_CAMERA_PRIORITY);
		if (temp_id == -1) {
			MTM_ASSERT(!"objCamera.cpp::ObjCamera3dInit() Error! can't init\n");
			return;
		}
	}
	obj_camera = obj_camera_sys->obj_camera[cam_id];


	// 3Dカメラ有効
	obj_camera->flag |= OBD_CAMERA_3D;

	// 共通
	obj_camera->znear	= 1.0f;
	obj_camera->zfar	= 60000.f;
#if defined _DS
	obj_camera->aspect	= (float)OBD_LCD_X / OBD_LCD_Y;
#else
	obj_camera->aspect	= AMD_SCREEN_ASPECT;
#endif

	// 透視射影
	obj_camera->fovy	= NNM_DEGtoA32(45.0f);
	nnMakePerspectiveMatrix(&obj_camera->prj_pers_mtx, obj_camera->fovy,
			obj_camera->aspect, obj_camera->znear, obj_camera->zfar);

	// 正射影
	{
		float	dx, dy;

		obj_camera->scale	= 0.078125f;	// ワイド時に横5ブロック分がこれぐらい

	    dy		= g_obj.disp_height * obj_camera->scale * 0.5f;
	    //dy		= AMD_SCREEN_2D_HEIGHT * obj_camera->scale * 0.5f;
		//dy		= (float)OBD_LCD_Y * obj_camera->scale * 0.5f;
	    dx		= dy * obj_camera->aspect;

		obj_camera->left	= -dx;
		obj_camera->right	= dx;
#ifdef _MG_IPAD			
		const float coeff = MPP_CAM_ASPECT_COEFF;
		obj_camera->bottom	= -dy*coeff;
		obj_camera->top		= dy*coeff;
#else
		obj_camera->bottom	= -dy;
		obj_camera->top		= dy;
#endif

		nnMakeOrthoMatrix(&obj_camera->prj_ortho_mtx,
			obj_camera->left, obj_camera->right, obj_camera->bottom, obj_camera->top,
			obj_camera->znear, obj_camera->zfar);
	}

	// ビューマトリクス取得
	switch (obj_camera->camera_type) {
	default:
		MTM_ASSERT(0);
		// no break;
	case OBE_CAMERA_TYPE_TARGET_ROLL:
		NNS_CAMERA_TARGET_ROLL		troll_camera;

		ObjCameraGetTargetRollCamera(obj_camera, &troll_camera);
		nnMakeTargetRollCameraViewMatrix(&obj_camera->view_mtx, &troll_camera);
		break;

	case OBE_CAMERA_TYPE_TARGET_UP_TARGET:
		NNS_CAMERA_TARGET_UPTARGET	tupt_camera;

		ObjCameraGetTargetUpTargetCamera(obj_camera, &tupt_camera);
		nnMakeTargetUpTargetCameraViewMatrix(&obj_camera->view_mtx, &tupt_camera);
		break;

	case OBE_CAMERA_TYPE_TARGET_UP_VEC:
		NNS_CAMERA_TARGET_UPVECTOR	tupvec_camera;

		ObjCameraGetTargetUpVecCamera(obj_camera, &tupvec_camera);
		nnMakeTargetUpVectorCameraViewMatrix(&obj_camera->view_mtx, &tupvec_camera);
		break;
	}
}

// ================================================================
// ObjCameraExit
/*!
  カメラ終了処理関数
 */
// ================================================================
void ObjCameraExit(void)
{
	if (obj_camera_tcb) {
		mtTaskClearTcb(obj_camera_tcb);
	}
}

// ================================================================
// View設定
// ================================================================
// ================================================================
// ObjCameraGetTargetRollCamera
/*!
  ターゲットロールカメラ取得
 
	@param camera			[in] カメラワーク
	@param troll_camera		[in] ターゲットアロールカメラワーク

	@note
		カメラワーク情報から ターゲットアップベクトルカメラ情報を取得します
 */
// ================================================================
void ObjCameraGetTargetRollCamera(OBS_CAMERA *obj_camera, NNS_CAMERA_TARGET_ROLL *troll_camera)
{
	MTM_ASSERT(obj_camera);
	MTM_ASSERT(troll_camera);

	troll_camera->User		= 0;
	troll_camera->Fovy		= obj_camera->fovy;
	troll_camera->Aspect	= obj_camera->aspect;
	troll_camera->ZNear		= obj_camera->znear;
	troll_camera->ZFar		= obj_camera->zfar;
	troll_camera->Position.x	= obj_camera->disp_pos.x;
	troll_camera->Position.y	= obj_camera->disp_pos.y;
	troll_camera->Position.z	= obj_camera->disp_pos.z;
	if (obj_camera->target_obj) {
		troll_camera->Target.x = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.x);
		troll_camera->Target.y = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.y);
		troll_camera->Target.z = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.z);
	}
	else {
		troll_camera->Target.x = obj_camera->target_pos.x;
		troll_camera->Target.y = obj_camera->target_pos.y;
		troll_camera->Target.z = obj_camera->target_pos.z;
	}
#if !_IPHONE
	troll_camera->Roll		= obj_camera->roll;
#else // !_IPHONE
	troll_camera->Roll		= obj_camera->roll + 0x4000;
#endif // !_IPHONE
}

// ================================================================
// ObjCameraGetTargetUpTargetCamera
/*!
  ターゲットアップターゲットカメラ取得
 
	@param camera			[in] カメラワーク
	@param troll_camera		[in] ターゲットアロールカメラワーク

	@note
		カメラワーク情報から ターゲットアップベクトルカメラ情報を取得します
 */
// ================================================================
void ObjCameraGetTargetUpTargetCamera(OBS_CAMERA *obj_camera, NNS_CAMERA_TARGET_UPTARGET *tupt_camera)
{
	MTM_ASSERT(obj_camera);
	MTM_ASSERT(tupt_camera);

	tupt_camera->User		= 0;
	tupt_camera->Fovy		= obj_camera->fovy;
	tupt_camera->Aspect		= obj_camera->aspect;
	tupt_camera->ZNear		= obj_camera->znear;
	tupt_camera->ZFar		= obj_camera->zfar;
	tupt_camera->Position.x	= obj_camera->disp_pos.x;
	tupt_camera->Position.y	= obj_camera->disp_pos.y;
	tupt_camera->Position.z	= obj_camera->disp_pos.z;
	if (obj_camera->camup_obj) {
		tupt_camera->Target.x = FXM_FX32_TO_FLOAT(obj_camera->camup_obj->pos.x);
		tupt_camera->Target.y = FXM_FX32_TO_FLOAT(obj_camera->camup_obj->pos.y);
		tupt_camera->Target.z = FXM_FX32_TO_FLOAT(obj_camera->camup_obj->pos.z);
	}
	else {
		tupt_camera->Target.x = obj_camera->camup_pos.x;
		tupt_camera->Target.y = obj_camera->camup_pos.y;
		tupt_camera->Target.z = obj_camera->camup_pos.z;
	}
}

// ================================================================
// ObjCameraGetTargetUpVecCamera
/*!
  ターゲットアップベクトルカメラ取得
 
	@param camera			[in] カメラワーク
	@param tupvec_camera	[in] ターゲットアップベクトルカメラワーク

	@note
		カメラワーク情報から ターゲットアップベクトルカメラ情報を取得します
 */
// ================================================================
void ObjCameraGetTargetUpVecCamera(OBS_CAMERA *obj_camera, NNS_CAMERA_TARGET_UPVECTOR *tupvec_camera)
{
	MTM_ASSERT(obj_camera);
	MTM_ASSERT(tupvec_camera);

	tupvec_camera->User		= 0;
	tupvec_camera->Fovy		= obj_camera->fovy;
	tupvec_camera->Aspect	= obj_camera->aspect;
	tupvec_camera->ZNear	= obj_camera->znear;
	tupvec_camera->ZFar		= obj_camera->zfar;
	tupvec_camera->Position.x	= obj_camera->disp_pos.x;
	tupvec_camera->Position.y	= obj_camera->disp_pos.y;
	tupvec_camera->Position.z	= obj_camera->disp_pos.z;
	if (obj_camera->target_obj) {
		tupvec_camera->Target.x = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.x);
		tupvec_camera->Target.y = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.y);
		tupvec_camera->Target.z = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.z);
	}
	else {
		tupvec_camera->Target.x = obj_camera->target_pos.x;
		tupvec_camera->Target.y = obj_camera->target_pos.y;
		tupvec_camera->Target.z = obj_camera->target_pos.z;
	}
	tupvec_camera->UpVector	= obj_camera->up_vec;
}

// ================================================================
// ステータス設定
// ================================================================
// ================================================================
// ObjCameraTargetObjSet
/*!
  カメラターゲット設定関数
 
  @param cam_id		[in] カメラID
  @param target_obj	[in] オブジェクトワークポインタ
 
 */
// ================================================================
void ObjCameraTargetObjSet(s32 cam_id, const OBS_OBJECT_WORK *target_obj)
{
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera_sys->obj_camera[cam_id]->target_obj = target_obj;
}

// ================================================================
// ObjCameraTargetObjSet
/*!
  カメラターゲット設定関数
 
  @param cam_id	[in] カメラ番号
  @param pos	[in] 座標
 
 */
// ================================================================
#if defined _DS
void ObjCameraTargetPosSet(s32 cam_id, const VecFx32 *pos)
#else
void ObjCameraTargetPosSet(s32 cam_id, const NNS_VECTOR *pos)
#endif
{
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera_sys->obj_camera[cam_id]->target_pos = *pos;
}

// ================================================================
// ObjCameraOffsetSet
/*!
  カメラオフセット設定関数
 
  @param cam_id	[in] カメラ番号
  @param ofst	[in] オフセット座標
 
 */
// ================================================================
#if defined _DS
void ObjCameraOffsetSet(s32 cam_id, const VecFx32 *ofst)
#else
void ObjCameraOffsetSet(s32 cam_id, const NNS_VECTOR *ofst)
#endif
{
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera_sys->obj_camera[cam_id]->ofst = *ofst;
}

// ================================================================
// ObjCameraDispOffsetSet
/*!
  カメラオフセット設定関数
 
  @param cam_id		[in] カメラ番号
  @param disp_ofst	[in] 表示オフセット座標
 
 */
// ================================================================
#if defined _DS
void ObjCameraDispOffsetSet(s32 cam_id, const VecFx32 *disp_ofst)
#else
void ObjCameraDispOffsetSet(s32 cam_id, const NNS_VECTOR *disp_ofst)
#endif
{
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera_sys->obj_camera[cam_id]->disp_ofst = *disp_ofst;
}

// ================================================================
// ObjCameraTargetOffsetSet
/*!
  カメラターゲットオフセット設定関数
 
  @param cam_id			[in] カメラ番号
  @param target_ofst	[in] オフセット座標
 
 */
// ================================================================
#if defined _DS
void ObjCameraTargetOffsetSet(s32 cam_id, const VecFx32 *target_ofst)
#else
void ObjCameraTargetOffsetSet(s32 cam_id, const NNS_VECTOR *target_ofst)
#endif
{
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera_sys->obj_camera[cam_id]->target_ofst = *target_ofst;
}

// ================================================================
// ObjCameraLimitSet
/*!
  カメラ範囲設定関数
 
  @param cam_id			[in] カメラ番号
  @param lLeft			[in] カメラ範囲左端座標 1:31
  @param lTop			[in] カメラ範囲上端座標 1:31
  @param lRight			[in] カメラ範囲右端座標 1:31
  @param lBottom		[in] カメラ範囲下端座標 1:31
 
 */
// ================================================================
void ObjCameraLimitSet(s32 cam_id, s32 lLeft, s32 lTop, s32 lRight, s32 lBottom )
{
	OBS_CAMERA  *obj_camera;

	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera = obj_camera_sys->obj_camera[cam_id];
    
	obj_camera->limit[OBD_LEFT  ] = lLeft   ;
	obj_camera->limit[OBD_TOP   ] = lTop    ;
	obj_camera->limit[OBD_RIGHT ] = lRight  ;
	obj_camera->limit[OBD_BOTTOM] = lBottom ;
}

// ================================================================
// ObjCameraSpdSet
/*!
  カメラ速度設定関数
 
  @param cam_id	[in] カメラ番号
  @param add	[in] 速度増加値
  @param max	[in] 速度最大値
 
 */
// ================================================================
#if defined _DS
void ObjCameraSpdSet(s32 cam_id, const VecFx32 *spd_add, const VecFx32 *spd_max)
#else
void ObjCameraSpdSet(s32 cam_id, const NNS_VECTOR *spd_add, const NNS_VECTOR *spd_max)
#endif
{
	OBS_CAMERA  *obj_camera;

	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera = obj_camera_sys->obj_camera[cam_id];

	obj_camera->spd_add = *spd_add;
	obj_camera->spd_max = *spd_max;
}

// ================================================================
// ObjCameraPlaySet
/*!
  カメラ遊び範囲設定関数
 
  @param cam_id	[in] カメラ番号
  @param ofst	[in] 遊び範囲
 
 */
// ================================================================
#if defined _DS
void ObjCameraPlaySet(s32 cam_id, const VecFx32 *ofst)
#else
void ObjCameraPlaySet(s32 cam_id, const NNS_VECTOR *ofst)
#endif
{
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera_sys->obj_camera[cam_id]->play_ofst_max = *ofst;
}

// ================================================================
// ObjCameraAllowSet
/*!
  カメラスクロール開始遊び範囲設定関数
 
  @param cam_id	[in] カメラ番号
  @param allow	[in] 遊び範囲
 
 */
// ================================================================
void ObjCameraAllowSet(s32 cam_id, const NNS_VECTOR *allow)
{
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera_sys->obj_camera[cam_id]->allow.x = 0;
	obj_camera_sys->obj_camera[cam_id]->allow.y = 0;
	obj_camera_sys->obj_camera[cam_id]->allow.z = 0;
	obj_camera_sys->obj_camera[cam_id]->allow_limit = *allow;
}

// ================================================================
// ObjCameraDispPosGet
/*!
  カメラ表示座標取得関数
 
  @param cam_id		[in] カメラ番号
  @param disp_pos	[out] 座標格納ポインタ
 
 */
// ================================================================
#if defined _DS
void ObjCameraDispPosGet(s32 cam_id,  VecFx32 *disp_pos)
#else
void ObjCameraDispPosGet(s32 cam_id,  NNS_VECTOR *disp_pos)
#endif
{
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);
	MTM_ASSERT(disp_pos);

	*disp_pos = obj_camera_sys->obj_camera[cam_id]->disp_pos;
}

// ================================================================
// ObjCameraDispCenterPosGet
/*!
  カメラ表示中心座標取得関数
 
  @param cam_id		[in] カメラ番号
  @param disp_pos	[out] 座標格納ポインタ
 
 */
// ================================================================
#if defined _DS
void ObjCameraDispCenterPosGet(s32 cam_id, VecFx32 *disp_pos)
{
	OBS_CAMERA  *obj_camera;
	fx32		width, height;

	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);
	MTM_ASSERT(disp_pos);

	obj_camera = obj_camera_sys->obj_camera[cam_id];

    width  = (OBD_LCD_X + ((OBD_LCD_X * obj_camera->disp_pos.z) >> FX32_SHIFT));
    height = (OBD_LCD_Y + ((OBD_LCD_Y * obj_camera->disp_pos.z) >> FX32_SHIFT));
    disp_pos->x = obj_camera->disp_pos.x + ((width  >> 1) << FX32_SHIFT);
    disp_pos->y = obj_camera->disp_pos.y + ((height >> 1) << FX32_SHIFT);
    disp_pos->z = obj_camera->disp_pos.z;
}
#else
void ObjCameraDispCenterPosGet(s32 cam_id, NNS_VECTOR *disp_pos)
{
	OBS_CAMERA  *obj_camera;
	float		width, height;

	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);
	MTM_ASSERT(disp_pos);

	obj_camera = obj_camera_sys->obj_camera[cam_id];

    width  = (OBD_LCD_X + (OBD_LCD_X * obj_camera->disp_pos.z));
    height = (OBD_LCD_Y + (OBD_LCD_Y * obj_camera->disp_pos.z));
	disp_pos->x	= obj_camera->disp_pos.x + width / 2.f;
	disp_pos->y	= obj_camera->disp_pos.y + height / 2.f;
	disp_pos->z	= obj_camera->disp_pos.z;
}
#endif

// ================================================================
// ObjCameraDispScaleGet
/*!
  カメラ拡大率取得関数
 
  @param cam_id   [in] カメラ番号

  @return 拡大率
 */
// ================================================================
#if defined _DS
fx32 ObjCameraDispScaleGet(s32 cam_id)
{
	OBS_CAMERA  *obj_camera;

	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera = obj_camera_sys->obj_camera[cam_id];

	if (camera->disp_pos.z >= 0) {
        return (0x1000 - ((obj_camera->disp_pos.z) >> 1));
	}
	else {
        return (0x1000 - ((obj_camera->disp_pos.z)));
	}
}
#else
float ObjCameraDispScaleGet(s32 cam_id)
{
	OBS_CAMERA  *obj_camera;

	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	obj_camera = obj_camera_sys->obj_camera[cam_id];

	if ( obj_camera->disp_pos.z >= 0 ) {
        return (1.0f - obj_camera->disp_pos.z / 2);
	}
	else {
        return (1.0f - obj_camera->disp_pos.z);
	}
}
#endif

// ================================================================
// ObjCameraPrevDispPosGet
/*!
  １フレーム前カメラ表示座標取得関数
 
  @param cam_id			[in] カメラ番号
  @param prev_disp_pos	[out] 座標格納ポインタ
 
 */
// ================================================================
#if defined _DS
void ObjCameraPrevDispPosGet(s32 cam_id, VecFx32 *prev_pos)
#else
void ObjCameraPrevDispPosGet(s32 cam_id, NNS_VECTOR *prev_disp_pos)
#endif
{
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);
	MTM_ASSERT(prev_disp_pos);

	*prev_disp_pos = obj_camera_sys->obj_camera[cam_id]->prev_disp_pos;
}

// ================================================================
// ObjCameraFallPosGet
/*!
  カメラ下 高さを取得（落下死等使用）
 
  @param cam_id				[in] カメラ番号

  @return カメラ下限界位置
 */
// ================================================================
s32 ObjCameraFallPosGet(s32 cam_id)
{
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

	return (obj_camera_sys->obj_camera[cam_id]->limit[OBD_BOTTOM]);
}

// ================================================================
// ObjCameraGet
/*!
  カメラワークポインタ取得関数
 
  @param cam_id				[in] カメラ番号

  @return カメラワークポインタ
 */
// ================================================================
OBS_CAMERA* ObjCameraGet(s32 cam_id)
{
#if 1
	MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
	if (obj_camera_sys) {
		return (obj_camera_sys->obj_camera[cam_id]);
	}
	return (NULL);

#else
	MTM_ASSERT(obj_camera_sys);
    MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
    MTM_ASSERT(obj_camera_sys->obj_camera[cam_id]);

    return (obj_camera_sys->obj_camera[cam_id]);
#endif
}

// ================================================================
// ObjCameraTargetObjSet
/*!
  カメラターゲット設定関数
 
  @param cam_id		[in] カメラID
  @param user_func	[in] ユーザー処理関数
 
 */
// ================================================================
void ObjCameraSetUserFunc(s32 cam_id, OBJF_CAMERA_USER_FUNC user_func)
{
#ifndef _DS
	MTM_ASSERT((u32)cam_id < OBD_CAMERA_NUM);
	if (obj_camera_sys && obj_camera_sys->obj_camera[cam_id]) {
		obj_camera_sys->obj_camera[cam_id]->user_func = user_func;
	}
#endif
}

//----- Local Functions ------------------------------------------------
// ================================================================
// objCameraDest
/*!
  バトル先頭解放関数
 */
// ================================================================
static void objCameraDest( MTS_TASK_TCB * pTcb )
{
	s32				i;
	OBS_CAMERA_SYS	*camera_sys;

	obj_camera_tcb = NULL;
	obj_camera_sys = NULL;

	camera_sys = (OBS_CAMERA_SYS*)mtTaskGetTcbWork( pTcb );

	for (i = 0; i < OBD_CAMERA_NUM; i++) {
		if (camera_sys->obj_camera[i]) {
			if (camera_sys->obj_camera[i]->user_work) {
				mtMemFreeMain(camera_sys->obj_camera[i]->user_work);
			}
			mtMemFreeSys(camera_sys->obj_camera[i]);
		}
	}
}

// ================================================================
// objCameraMain
/*!
  バトル先頭メイン関数
 */
// ================================================================
void objCameraMain(MTS_TASK_TCB *tcb)
{
	s32				i;
	OBS_CAMERA		*obj_camera;
    NNS_VECTOR		temp;

	UNREFERENCED_PARAMETER(tcb);

	MTM_ASSERT(obj_camera_sys);

	if (ObjObjectPauseCheck(0)) {
		// ポーズ中
		return;
	}

	for (i = 0; i < OBD_CAMERA_NUM; i++) {
		obj_camera = obj_camera_sys->obj_camera[i];
		if (obj_camera == NULL) {
			continue;
		}

		// 前フレーム表示座標保持
		obj_camera->prev_disp_pos.x = obj_camera->disp_pos.x;
		obj_camera->prev_disp_pos.y = obj_camera->disp_pos.y;
		obj_camera->prev_disp_pos.z = obj_camera->disp_pos.z;

		// ユーザー処理
		if (obj_camera->user_func) {
			obj_camera->user_func(obj_camera);
		}
		else {
			// カメラ移動
			if (!(obj_camera->flag & OBD_CAMERA_STOP)) {
				objCameraMove(obj_camera);
			}

			// クッション変数へセット
			temp.x = obj_camera->pos.x;
			temp.y = obj_camera->pos.y;
			temp.z = obj_camera->pos.z;
		    
			if (obj_camera->flag & OBD_CAMERA_REVERSE) {
				// リバース
				temp.x -= ((temp.x - obj_camera->work.x) * 2);
				temp.y -= ((temp.y - obj_camera->work.y) * 2);
				temp.z -= ((temp.z - obj_camera->work.z) * 2);
			}
		    
			// 表示位置を設定
			obj_camera->disp_pos.x = temp.x + obj_camera->ofst.x;
			obj_camera->disp_pos.y = temp.y + obj_camera->ofst.y;
			obj_camera->disp_pos.z = temp.z + obj_camera->ofst.z;
		}


		// 表示位置オーバーチェック
		if (obj_camera->flag & OBD_CAMERA_LIMITCHECK) {
			objCameraLimitCheck(obj_camera);
		}

		obj_camera->disp_pos.x += obj_camera->disp_ofst.x;
		obj_camera->disp_pos.y += obj_camera->disp_ofst.y;
		obj_camera->disp_pos.z += obj_camera->disp_ofst.z;

		obj_camera->disp_ofst.x = 0;
		obj_camera->disp_ofst.y = 0;
		obj_camera->disp_ofst.z = 0;


		if (obj_camera->flag & OBD_CAMERA_3D) {
			float						dx, dy;

			// 透視射影
			nnMakePerspectiveMatrix(&obj_camera->prj_pers_mtx, obj_camera->fovy,
					obj_camera->aspect, obj_camera->znear, obj_camera->zfar);


			// 正射影
#if !_IPHONE
			dy		= g_obj.disp_height * obj_camera->scale * 0.5f;
			//dy		= AMD_SCREEN_2D_HEIGHT * obj_camera->scale * 0.5f;
			//dy		= (float)OBD_LCD_Y*2 * obj_camera->scale * 0.5f;
#else //!_IPHONE
			// 画面を無理矢理横にまわしているのでアスペクト比がおかしくなる都合上パラメータを入れ替えて使う
			//dy		= g_obj.disp_width * obj_camera->scale * 0.5f;
			dy = AMD_SCREEN_2D_WIDTH * obj_camera->scale * 0.5f;
#endif //!_IPHONE
			dx		= dy * obj_camera->aspect;

			obj_camera->left	= -dx;
			obj_camera->right	= dx;
#ifdef _MG_IPAD			
			const float coeff = MPP_CAM_ASPECT_COEFF;
			obj_camera->bottom	= -dy*coeff;
			obj_camera->top		= dy*coeff;
#else
			obj_camera->bottom	= -dy;
			obj_camera->top		= dy;
#endif

			nnMakeOrthoMatrix(&obj_camera->prj_ortho_mtx,
				obj_camera->left, obj_camera->right, obj_camera->bottom, obj_camera->top,
				obj_camera->znear, obj_camera->zfar);

			// ビューマトリクス取得
			switch (obj_camera->camera_type) {
			default:
				MTM_ASSERT(0);
				// no break
			case OBE_CAMERA_TYPE_TARGET_ROLL:
				NNS_CAMERA_TARGET_ROLL		troll_camera;

				Angle32 roll_temp;
				roll_temp = obj_camera->roll;								// カメラ回転角をtempに保持
				if (obj_camera->flag & OBD_CAMERA_ROT_OFF) {
					obj_camera->roll = 0;									// カメラ回転OFF
				}
				
				ObjCameraGetTargetRollCamera(obj_camera, &troll_camera);
				nnMakeTargetRollCameraViewMatrix(&obj_camera->view_mtx, &troll_camera);

				obj_camera->roll = roll_temp;								// カメラ回転角を戻す
				break;

			case OBE_CAMERA_TYPE_TARGET_UP_TARGET:
				NNS_CAMERA_TARGET_UPTARGET	tupt_camera;

				ObjCameraGetTargetUpTargetCamera(obj_camera, &tupt_camera);
				nnMakeTargetUpTargetCameraViewMatrix(&obj_camera->view_mtx, &tupt_camera);
				break;

			case OBE_CAMERA_TYPE_TARGET_UP_VEC:
				NNS_CAMERA_TARGET_UPVECTOR	tupvec_camera;

				ObjCameraGetTargetUpVecCamera(obj_camera, &tupvec_camera);
				nnMakeTargetUpVectorCameraViewMatrix(&obj_camera->view_mtx, &tupvec_camera);
				break;
			}
		}
	}
}

// ================================================================
// objCameraMove
/*!
  カメラ移動関数
 */
// ================================================================
static void objCameraMove( OBS_CAMERA * obj_camera )
{
    NNS_VECTOR pos;

    if ( obj_camera->target_obj ){
//		pos.x = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.x - ((OBD_LCD_X>>1) << FX32_SHIFT)) + obj_camera->target_ofst.x;
//		pos.y = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.y - ((OBD_LCD_Y>>1) << FX32_SHIFT)) + obj_camera->target_ofst.y;
//		pos.z = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.z) + obj_camera->target_ofst.z;
		pos.x = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.x) + obj_camera->target_ofst.x;
		pos.y = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.y - (((OBD_LCD_Y<<1)+200) << FX32_SHIFT)) + obj_camera->target_ofst.y;
		pos.z = FXM_FX32_TO_FLOAT(obj_camera->target_obj->pos.z) + obj_camera->target_ofst.z;
    }else{
        pos.x = obj_camera->target_pos.x - FXM_FX32_TO_FLOAT(((OBD_LCD_X>>1) << FX32_SHIFT)) + obj_camera->target_ofst.x;
        pos.y = obj_camera->target_pos.y - FXM_FX32_TO_FLOAT(((OBD_LCD_Y>>1) << FX32_SHIFT)) + obj_camera->target_ofst.y;
        pos.z = obj_camera->target_pos.z + obj_camera->target_ofst.z;
    }

    // 目的値を記録
    obj_camera->work.x = pos.x;
    obj_camera->work.y = pos.y;
    obj_camera->work.z = pos.z;

    obj_camera->prev_pos.x = obj_camera->pos.x;
    obj_camera->prev_pos.y = obj_camera->pos.y;
    obj_camera->prev_pos.z = obj_camera->pos.z;

    if ( obj_camera->flag & OBD_CAMERA_FIX ){
        if ( obj_camera->target_obj ){
            obj_camera->pos.x = pos.x;
            obj_camera->pos.y = pos.y;
            obj_camera->pos.z = pos.z;
        }

        return;
    }

    if ( obj_camera->flag & OBD_CAMERA_SHIFT ){
        // シフトで各軸移動
        obj_camera->pos.x = ObjShiftSetF( obj_camera->pos.x, pos.x, obj_camera->shift, obj_camera->spd_max.x, obj_camera->spd_add.x );
        obj_camera->pos.y = ObjShiftSetF( obj_camera->pos.y, pos.x, obj_camera->shift, obj_camera->spd_max.y, obj_camera->spd_add.y );
        obj_camera->pos.z = ObjShiftSetF( obj_camera->pos.z, pos.x, obj_camera->shift, obj_camera->spd_max.z, obj_camera->spd_add.z );
    }else{
        // 移動速度設定、加減速
        if ( pos.x != obj_camera->pos.x ){
            obj_camera->spd.x = ObjSpdUpSetF( obj_camera->spd.x, obj_camera->spd_add.x, obj_camera->spd_max.x);
        }else{
            obj_camera->spd.x = ObjSpdDownSetF( obj_camera->spd.x, obj_camera->spd_add.x);
        }
        if ( pos.y != obj_camera->pos.y ){
            obj_camera->spd.y = ObjSpdUpSetF( obj_camera->spd.y, obj_camera->spd_add.y, obj_camera->spd_max.y);
        }else{
            obj_camera->spd.y = ObjSpdDownSetF( obj_camera->spd.y, obj_camera->spd_add.y);
        }
        if ( pos.z != obj_camera->pos.z ){
            obj_camera->spd.z = ObjSpdUpSetF( obj_camera->spd.z, obj_camera->spd_add.z, obj_camera->spd_max.z);
        }else{
            obj_camera->spd.z = ObjSpdDownSetF( obj_camera->spd.z, obj_camera->spd_add.z);
        }
        // 各軸移動
        if ( pos.x > obj_camera->pos.x ){
            obj_camera->pos.x += obj_camera->spd.x;
            if ( obj_camera->pos.x > pos.x )
                obj_camera->pos.x = pos.x;
        }else{
            obj_camera->pos.x -= obj_camera->spd.x;
            if ( obj_camera->pos.x < pos.x )
                obj_camera->pos.x = pos.x;
        }
        if ( pos.y > obj_camera->pos.y ){
            obj_camera->pos.y += obj_camera->spd.y;
            if ( obj_camera->pos.y > pos.y )
                obj_camera->pos.y = pos.y;
        }else{
            obj_camera->pos.y -= obj_camera->spd.y;
            if ( obj_camera->pos.y < pos.y )
                obj_camera->pos.y = pos.y;
        }
        if ( pos.z > obj_camera->pos.z ){
            obj_camera->pos.z += obj_camera->spd.z;
            if ( obj_camera->pos.z > pos.z )
                obj_camera->pos.z = pos.z;
        }else{
            obj_camera->pos.z -= obj_camera->spd.z;
            if ( obj_camera->pos.z < pos.z )
                obj_camera->pos.z = pos.z;
        }
    }

    // 離れオーバーチェック
    if ( MTM_MATH_ABS(pos.x - obj_camera->pos.x) > obj_camera->play_ofst_max.x ){
        if ( pos.x > obj_camera->pos.x ){
            obj_camera->pos.x = pos.x - obj_camera->play_ofst_max.x;
        }else{
            obj_camera->pos.x = pos.x + obj_camera->play_ofst_max.x;
        }

    }
    if ( MTM_MATH_ABS(pos.y - obj_camera->pos.y) > obj_camera->play_ofst_max.y ){
        if ( pos.y > obj_camera->pos.y ){
            obj_camera->pos.y = pos.y - obj_camera->play_ofst_max.y;
        }else{
            obj_camera->pos.y = pos.y + obj_camera->play_ofst_max.y;
        }

    }
    if ( MTM_MATH_ABS(pos.z - obj_camera->pos.z) > obj_camera->play_ofst_max.z ){
        if ( pos.z > obj_camera->pos.z ){
            obj_camera->pos.z = pos.z - obj_camera->play_ofst_max.z;
        }else{
            obj_camera->pos.z = pos.z + obj_camera->play_ofst_max.z;
        }
    }

    // 範囲オーバーチェック
//	objCameraPosLimitCheck(obj_camera, &obj_camera->pos );

//	obj_camera->target_pos = obj_camera->pos;
	obj_camera->pos.z += obj_camera->ofst.z;
	// ステータス表示
	{
		s32	y = 8;

		amPrintf(2, y, "CAM X : %f", obj_camera->pos.x);	y++;
		amPrintf(2, y, "CAM Y : %f", obj_camera->pos.y);	y++;
		amPrintf(2, y, "CAM Z : %f", obj_camera->pos.z);	y++;
		amPrintf(2, y, "TAR X : %f", obj_camera->target_pos.x);	y++;
		amPrintf(2, y, "TAR Y : %f", obj_camera->target_pos.y);	y++;
		amPrintf(2, y, "TAR Z : %f", obj_camera->target_pos.z);	y++;
		amPrintf(2, y, "ROT X : %d", AoPadRotX());			y++;
		amPrintf(2, y, "ROT Z : %d", AoPadRotZ());			y++;
	}
}

// ================================================================
// objCameraLimitCheck
/*!
  カメラ移動範囲チェック関数
 */
// ================================================================
static void objCameraLimitCheck( OBS_CAMERA * obj_camera )
{
    objCameraPosLimitCheck( obj_camera, &obj_camera->disp_pos);
}
// ================================================================
// objCameraPosLimitCheck
/*!
  カメラ移動範囲チェック関数
 */
// ================================================================
#if defined _DS
static void objCameraPosLimitCheck( OBS_CAMERA * obj_camera, VecFx32 *pPos )
#else
static void objCameraPosLimitCheck( OBS_CAMERA * obj_camera, NNS_VECTOR *pPos )
#endif
{
    s32 lcd = 0;
    float scale;
    MTM_ASSERT( pPos );

    if ( pPos->z ){
        // Z値オーバーチェック
        if ( (float)obj_camera->limit[OBD_BACK] > pPos->z )
            pPos->z = (float)obj_camera->limit[OBD_BACK];
        if ( (float)obj_camera->limit[OBD_FRONT] < pPos->z )
            pPos->z = (float)obj_camera->limit[OBD_FRONT];
        
        // 縮小した時の画面サイズがカメラ範囲を下回る時はZ値を補正する
		if ((1.f/ObjCameraDispScaleGet(obj_camera->index) * OBD_LCD_X) >
					(float)(obj_camera->limit[OBD_RIGHT] - obj_camera->limit[OBD_LEFT])) {
            scale = (float)(obj_camera->limit[OBD_RIGHT] - obj_camera->limit[OBD_LEFT]) / OBD_LCD_X;
            scale = 1.f / scale;
            if ( pPos->z >= 0.000f)
                pPos->z = (scale - 1.f) * -2.f;
            else
                pPos->z = (scale - 1.f) * -1.f;
        }
		if ((1.f/ObjCameraDispScaleGet(obj_camera->index)) * OBD_LCD_Y >
				(float)(obj_camera->limit[OBD_BOTTOM] - obj_camera->limit[OBD_TOP])) {
            scale = (float)(obj_camera->limit[OBD_BOTTOM] - obj_camera->limit[OBD_TOP]) / OBD_LCD_X;
            scale = 1.f / scale;
            if ( pPos->z >= 0.000f)
                pPos->z = (scale - 1.f) * -2.f;
            else
                pPos->z = (scale - 1.f) * -1.f;
		}
        
    }
    if ( pPos->z > 0.000f){
        // 拡大縮小後の画面サイズ計算
        lcd = FXM_FLOAT_TO_FX32((1.f/ObjCameraDispScaleGet(obj_camera->index)) * OBD_LCD_X);
        // 画面サイズのはみ出し分を計算
        lcd -= (OBD_LCD_X << FX32_SHIFT);
        lcd >>= 1 + FX32_SHIFT;
    }
    
    // 左右オーバーチェック
    if ( (float)(obj_camera->limit[OBD_LEFT  ] + lcd) > pPos->x )
        pPos->x = (float)(obj_camera->limit[OBD_LEFT  ] + lcd );

    if ( (float)(obj_camera->limit[OBD_RIGHT ] - OBD_LCD_X - lcd) < pPos->x )
        pPos->x = (float)(obj_camera->limit[OBD_RIGHT ] - OBD_LCD_X - lcd);

    if ( pPos->z > 0){
        // 拡大縮小後の画面サイズ計算
        lcd = FXM_FLOAT_TO_FX32((1.f/ObjCameraDispScaleGet(obj_camera->index)) * OBD_LCD_Y);
        // 画面サイズのはみ出し分を計算
        lcd -= (OBD_LCD_Y << FX32_SHIFT);
        lcd >>= 1 + FX32_SHIFT;
        // lcd = FX_DivS32(pPos->z, 42 );
    }
    // 上下オーバーチェック
    if ( (float)(obj_camera->limit[OBD_TOP   ] + lcd) > pPos->y )
        pPos->y = (float)(obj_camera->limit[OBD_TOP   ] + lcd);
    if ( (float)(obj_camera->limit[OBD_BOTTOM] - OBD_LCD_Y - lcd) < pPos->y )
        pPos->y = (float)(obj_camera->limit[OBD_BOTTOM] - OBD_LCD_Y - lcd);

}


// ==========================================================================
// objCameraStaticVarInit
/*!
 *	static変数の初期化
 */
// ==========================================================================
void objCameraStaticVarInit(void)
{
	obj_camera_tcb = NULL;		///< カメラTCB
	obj_camera_sys = NULL;		///< カメラシステムワーク
}


// ================================================================
// test_func
/*!
  テスト関数
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return   返値説明
 
  @note
  補足説明
 */
// ================================================================
