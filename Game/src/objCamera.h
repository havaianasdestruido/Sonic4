// ================================================================
/*!
  @file objCamera.h
  @brief オブジェクトカメラ

  @author mana
                Copyright(c) 2005 Dimps

  $Id: objCamera.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 *
 */
/*!
  @page  obj_camera obj カメラ
 
  @section  obj_camera_plain 解説
    　 カメラを制御するシステム。\n
    　 カメラの稼動範囲や、ターゲットの追いかけ具合、拡大縮小などを管理、操作する。\n
    　 複数カメラ対応は未チェック\
    　 3Dカメラ対応は3Dspriteのみチェック済み\n
    \n
    　 ObjCameraInit() で初期化し、3Dカメラ設定が必要であれば、objCamera3dInit() を呼び出す\n
    　 その後 カメラの稼動範囲の設定 ObjCameraLimitSet() を行う\n
    　 カメラが追いかけるものがひとつのオブジェクトのみであれば ObjCameraTargetObjSet()
    計算で中心を求めるのであれば 自作の関数内で ObjCameraTargetPosSet() で中心座標を設定する\n
    \n
    　振動の設定は ObjCameraDispOffsetSet() などで行う\n
    
  @sa objCamera.c objCamera.h

 */
#ifndef _H_OBJCAMERA
#define _H_OBJCAMERA

#include "objObject.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------
#define OBD_CAMERA_NUM ( 8 ) ///< カメラの最大数

#ifndef _DS
typedef enum tag_OBE_CAMERA_TYPE {
	OBE_CAMERA_TYPE_TARGET_ROLL	= 0,		//!< ターゲットロールカメラ
	OBE_CAMERA_TYPE_TARGET_UP_TARGET,		//!< ターゲットアップターゲットカメラ
	OBE_CAMERA_TYPE_TARGET_UP_VEC,			//!< ターゲットアップベクトルカメラ

	OBE_CAMERA_TYPE_MAX
} OBE_CAMERA_TYPE;
#endif	// #if ifndef _DS


/// カメラ構造体
typedef struct tag_OBS_CAMERA
{

#if defined _DS
    VecFx32 disp_pos;		///< カメラ表示座標 オブジェクトの生成等はこの座標を用いる
    VecFx32 prev_disp_pos;	///< カメラ１フレーム前表示座標
    VecFx32 pos;            ///< カメラ座標
    VecFx32 prev_pos;		///< カメラ１フレーム前座標
    VecFx32 ofst;			///< カメラ座標オフセット（外部操作用）
    VecFx32 disp_ofst;		///< カメラ表示座標オフセット（振動用、毎フレーム初期化される）
    VecFx32 target_ofst;	///< ターゲットからのオフセット
    VecFx32 play_ofst_max;	///< カメラを遊ばせる場合の最大遅れ値
    const OBS_OBJECT_WORK* target_obj;///< 注視オブジェクト
    VecFx32 target_pos;		///< 注視座標 注視オブジェクト未設定時に使用
    VecFx32 spd;            ///< ターゲット補足速度
    VecFx32 spd_add;		///< ターゲット補足速度
    VecFx32 spd_max;		///< ターゲット補足速度
    u16     shift;			///< シフト値、1で毎フレーム>>1 2で毎フレーム>>2
    u16     index;			///< シフト値、1で毎フレーム>>1 2で毎フレーム>>2
    VecFx32 work;			///< システム使用

    s32		limit[6];       ///< 稼動範囲
    u32		flag;   ///< フラグ

	MTS_UTIL_CAMERA_LOOKAT	camera;
#else
	s32			camera_id;		///< 自分のID

	NNS_VECTOR	disp_pos;		///< カメラ表示座標 オブジェクトの生成等はこの座標を用いる
	NNS_VECTOR	prev_disp_pos;	///< カメラ１フレーム前表示座標
	NNS_VECTOR	pos;            ///< カメラ座標
	NNS_VECTOR	prev_pos;		///< カメラ１フレーム前座標
	NNS_VECTOR	ofst;			///< カメラ座標オフセット（外部操作用）
	NNS_VECTOR	disp_ofst;		///< カメラ表示座標オフセット（振動用、毎フレーム初期化される）
	NNS_VECTOR	target_ofst;	///< ターゲットからのオフセット
	NNS_VECTOR	play_ofst_max;	///< カメラを遊ばせる場合の最大遅れ値
	NNS_VECTOR	allow;			///< カメラスクロール開始までのを遊び範囲
	NNS_VECTOR	allow_limit;	///< カメラスクロール開始までのを遊び最大値
    const OBS_OBJECT_WORK* target_obj;///< 注視オブジェクト
	NNS_VECTOR	target_pos;		///< 注視座標 注視オブジェクト未設定時に使用
    const OBS_OBJECT_WORK* camup_obj;///< カメラアップオブジェクト
	NNS_VECTOR	camup_pos;		///< カメラアップ カメラアップオブジェクト未設定時に使用
	NNS_VECTOR	spd;            ///< ターゲット補足速度
	NNS_VECTOR	spd_add;		///< ターゲット補足速度
	NNS_VECTOR	spd_max;		///< ターゲット補足速度
	Angle32		roll;			///< カメラ回転量
	Angle32		roll_hist[16];	///< カメラ回転量履歴
	u16			roll_ptr;		///< カメラ回転量履歴ポインタ
	
	u16			shift;			///< シフト値、1で毎フレーム>>1 2で毎フレーム>>2
	u16			index;			///< シフト値、1で毎フレーム>>1 2で毎フレーム>>2
	NNS_VECTOR	work;			///< システム使用

	u32			command_state;	///< 描画コマンド発行時のステート 標準:OBD_DRAW_CMD_STATE_3DNN

	void	(*user_func)(struct tag_OBS_CAMERA*);	///< ユーザーカメラ処理
	void	*user_work;			///< ユーザー処理用ワーク (カメラシステム解放時自動解放あり)
	


	s32			limit[OBD_BOX];	///< 稼動範囲
	u32			flag;			///< フラグ

					
	// 3D用
	//NNE_PROJECTION_TYPE		proj_type;		//!< 射影タイプ
	OBE_CAMERA_TYPE			camera_type;	//!< カメラタイプ

	NNS_MATRIX44			prj_pers_mtx;	//!< 透視射影マトリクス
	NNS_MATRIX44			prj_ortho_mtx;	//!< 正射影マトリクス
	NNS_MATRIX				view_mtx;		//!< ビューマトリクス

	// 透視射影
	Angle32		fovy;		//!< カメラの視野角
	NNS_VECTOR	up_vec;		//!< カメラアップベクトル

	// 正射影
	float	scale;
	float	left;
	float	right;
	float	bottom;
	float	top;

	// 共通
	float	znear;			//!< ニアクリッピング面への距離
	float	zfar;			//!< ファークリッピング面への距離
	float	aspect;			//!< イメージアスペクト比

#endif	// #if defined _DS

} OBS_CAMERA;

// OBS_CAMERA::flag
#define OBD_CAMERA_FIX			( 1 << 0 ) ///< カメラに遊びを持たさない
#define OBD_CAMERA_SHIFT		( 1 << 1 ) ///< カメラの移動をシフト演算で行う、最大速度はvSpdMaxを使用、最低速度はvSpdAddを使用
#define OBD_CAMERA_STOP			( 1 << 2 ) ///< カメラを止める
#define OBD_CAMERA_REVERSE		( 1 << 3 ) ///< カメラ遅れを反転させる（移動方向を先読みしてカメラを動かすようになる）
#define OBD_CAMERA_3D			( 1 << 4 ) ///< 3Dカメラの設定も行う
#define OBD_CAMERA_LIMITCHECK	( 1 << 5 ) ///< カメラ表示位置限界チェックを行う

#define OBD_CAMERA_ROT_OFF		( 1 << 30) ///< カメラ回転OFF（rollの内容を無視する）
#define OBD_CAMERA_ROT_EX		( 1 << 31) ///< 特殊ローテート（ループカメラ用）


typedef struct tag_OBS_CAMERA_SYS {
	OBS_CAMERA	*obj_camera[OBD_CAMERA_NUM];	///< カメラワーク
	int			camera_num;						///< 使用カメラ数
		

} OBS_CAMERA_SYS;

/// ユーザー処理型
typedef void (*OBJF_CAMERA_USER_FUNC)(OBS_CAMERA *);

//----- External Declarations ------------------------------------------
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
extern s32 ObjCameraInit(s32 cam_id, NNS_VECTOR *pos, s32 group, u16 pause_level, s32 prio);

// ================================================================
// ObjCamera3dInit
/*!
  3Dカメラ初期化関数
 
  @param cam_id   [in] カメラ番号 (ObjCameraInit で初期化したカメラNO)
 */
// ================================================================
extern void ObjCamera3dInit(s32 cam_id);

// ================================================================
// ObjCameraExit
/*!
  カメラ終了処理関数
 */
// ================================================================
extern void ObjCameraExit(void);

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
extern void ObjCameraGetTargetRollCamera(OBS_CAMERA *obj_camera, NNS_CAMERA_TARGET_ROLL *troll_camera);

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
extern void ObjCameraGetTargetUpTargetCamera(OBS_CAMERA *obj_camera, NNS_CAMERA_TARGET_UPTARGET *tupt_camera);

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
extern void ObjCameraGetTargetUpVecCamera(OBS_CAMERA *camera, NNS_CAMERA_TARGET_UPVECTOR *tupvec_camera);

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
extern void ObjCameraTargetObjSet(s32 cam_id, const OBS_OBJECT_WORK *target_obj);

// ================================================================
// ObjCameraTargetObjSet
/*!
  カメラターゲット設定関数
 
  @param cam_id	[in] カメラ番号
  @param pos	[in] 座標
 
 */
// ================================================================
#if defined _DS
extern void ObjCameraTargetPosSet(s32 cam_id, const VecFx32 *pos);
#else
extern void ObjCameraTargetPosSet(s32 cam_id, const NNS_VECTOR *pos);
#endif

// ================================================================
// ObjCameraOffsetSet
/*!
  カメラオフセット設定関数
 
  @param cam_id	[in] カメラ番号
  @param ofst	[in] オフセット座標
 
 */
// ================================================================
#if defined _DS
extern void ObjCameraOffsetSet(s32 cam_id, const VecFx32 *ofst);
#else
extern void ObjCameraOffsetSet(s32 cam_id, const NNS_VECTOR *ofst);
#endif

// ================================================================
// ObjCameraDispOffsetSet
/*!
  カメラオフセット設定関数
 
  @param cam_id		[in] カメラ番号
  @param disp_ofst	[in] 表示オフセット座標
 
 */
// ================================================================
#if defined _DS
extern void ObjCameraDispOffsetSet(s32 cam_id, const VecFx32 *disp_ofst);
#else
extern void ObjCameraDispOffsetSet(s32 cam_id, const NNS_VECTOR *disp_ofst);
#endif

// ================================================================
// ObjCameraTargetOffsetSet
/*!
  カメラターゲットオフセット設定関数
 
  @param cam_id			[in] カメラ番号
  @param target_ofst	[in] オフセット座標
 
 */
// ================================================================
#if defined _DS
extern void ObjCameraTargetOffsetSet(s32 cam_id, const VecFx32 *target_ofst);
#else
extern void ObjCameraTargetOffsetSet(s32 cam_id, const NNS_VECTOR *target_ofst);
#endif

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
extern void ObjCameraLimitSet(s32 cam_id, s32 lLeft, s32 lTop, s32 lRight, s32 lBottom );

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
extern void ObjCameraSpdSet(s32 cam_id, const VecFx32 *spd_add, const VecFx32 *spd_max);
#else
extern void ObjCameraSpdSet(s32 cam_id, const NNS_VECTOR *spd_add, const NNS_VECTOR *spd_max);
#endif

// ================================================================
// ObjCameraPlaySet
/*!
  カメラ遊び範囲設定関数
 
  @param cam_id	[in] カメラ番号
  @param ofst	[in] 遊び範囲
 
 */
// ================================================================
#if defined _DS
extern void ObjCameraPlaySet(s32 cam_id, const VecFx32 *ofst);
#else
extern void ObjCameraPlaySet(s32 cam_id, const NNS_VECTOR *ofst);
#endif

// ================================================================
// ObjCameraAllowSet
/*!
  カメラスクロール開始遊び範囲設定関数
 
  @param cam_id	[in] カメラ番号
  @param allow	[in] 遊び範囲
 
 */
// ================================================================
extern void ObjCameraAllowSet(s32 cam_id, const NNS_VECTOR *allow);

// ================================================================
// ObjCameraDispPosGet
/*!
  カメラ表示座標取得関数
 
  @param cam_id		[in] カメラ番号
  @param disp_pos	[out] 座標格納ポインタ
 
 */
// ================================================================
#if defined _DS
extern void ObjCameraDispPosGet(s32 cam_id,  VecFx32 *disp_pos);
#else
extern void ObjCameraDispPosGet(s32 cam_id,  NNS_VECTOR *disp_pos);
#endif

// ================================================================
// ObjCameraDispCenterPosGet
/*!
  カメラ表示中心座標取得関数
 
  @param cam_id		[in] カメラ番号
  @param disp_pos	[out] 座標格納ポインタ
 
 */
// ================================================================
#if defined _DS
extern void ObjCameraDispCenterPosGet(s32 cam_id, VecFx32 *disp_pos);
#else
extern void ObjCameraDispCenterPosGet(s32 cam_id, NNS_VECTOR *disp_pos);
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
extern fx32 ObjCameraDispScaleGet(s32 cam_id);
#else
extern float ObjCameraDispScaleGet(s32 cam_id);
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
extern void ObjCameraPrevDispPosGet(s32 cam_id, VecFx32 *prev_disp_pos);
#else
extern void ObjCameraPrevDispPosGet(s32 cam_id, NNS_VECTOR *prev_disp_pos);
#endif

// ================================================================
// ObjCameraFallPosGet
/*!
  カメラ下 高さを取得（落下死等使用）
 
  @param cam_id				[in] カメラ番号

  @return カメラ下限界位置
 */
// ================================================================
extern s32 ObjCameraFallPosGet(s32 cam_id);

// ================================================================
// ObjCameraGet
/*!
  カメラワークポインタ取得関数
 
  @param cam_id				[in] カメラ番号

  @return カメラワークポインタ
 */
// ================================================================
extern OBS_CAMERA* ObjCameraGet(s32 cam_id);

// ================================================================
// ObjCameraTargetObjSet
/*!
  カメラターゲット設定関数
 
  @param cam_id		[in] カメラID
  @param user_func	[in] ユーザー処理関数
 
 */
// ================================================================
extern void ObjCameraSetUserFunc(s32 cam_id, OBJF_CAMERA_USER_FUNC user_func);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _H_OBJCAMERA
